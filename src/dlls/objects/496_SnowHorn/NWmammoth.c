//"NWmammoth.c" seems to have been Rare's name!

#include "PR/gbi.h"
#include "PR/os.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/26_curves.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/227_tumbleweed.h"
#include "dlls/objects/496_SnowHorn.h"
#include "game/gamebits.h"
#include "game/gametexts.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/model.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objexpr.h"
#include "sys/objhits.h"
#include "sys/objlib.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "types.h"

#ifdef DEBUG
#define FROSTWEED_QUEST_CHEAT 1  
#else  
#define FROSTWEED_QUEST_CHEAT 0  
#endif 

#define GARUNDA_TE_WEEDS_NEEDED 12

typedef struct {
/*000*/ s32 unk0;
/*004*/ s16 playerNearbyRange;  //`SnowHorn_FLAG_80_Player_Nearby` is set when the player is inside a multiple of this range (not used for anything, though)
/*006*/ s16 squirtInterval;     //For the Blue SnowHorn: the time (in frames) between squirting water at the player
/*008*/ s16 timer;              //A timer for calling out to the player (Garunda Te), or squirting water (Blue SnowHorn)

//A randomised delay before a SnowHorn wakes at daytime (to stagger nearby SnowHorn's wake animations).
//Also used by the Grumpy SnowHorn to attempt to play an out-of-bounds ObjSeq when the player is nearby.
/*00A*/ s16 sleepTimer;         
/*00c*/ u16 state;              //Used as a State Machine value and for secondary flags (in an overlapping way)
/*00e*/ u16 headYaw;            //The yaw of the SnowHorn's head seqJoint
/*010*/ s32 unk10;
/*014*/ Vec3f trunkAttachPoint; //WS coordinates of the SnowHorn's trunk, stored when drawn
/*020*/ f32 playerDistance;     //Used for head look-at behaviour
/*024*/ s32 unk24;              
/*028*/ Object* frostWeed;      //A FrostWeed, referenced while Garunda Te is inhaling it
/*02c*/ s16 aimYaw;             //Unused trajectory angle, presumably for the Blue SnowHorn's missing water squirt particles
/*02e*/ s16 unk2E;           
/*030*/ s32 unk30;           
/*034*/ s32 unk34;           
/*038*/ Vec3f aimTarget;        //Unused aim target coordinates, presumably for the Blue SnowHorn's missing water squirt particles
/*044*/ s16* anims;             //State Machine values' modAnims
/*048*/ f32* animSpeeds;        //State Machine values' animSpeeds
/*04c*/ s32* chatSequenceList;  //ObjSeqs to play when the player interacts
/*050*/ f32 animSpeed;          //Animation playback speed
/*054*/ f32 animSpeedFlinch;    //Animation playback speed (during hitSphere flinching)
/*058*/ f32 walkSpeed;          //Walking SnowHorns' current speed
/*05C*/ s32 unk5C;
/*060*/ UnkCurvesStruct curves; //Walking SnowHorns' curve data
/*168*/ s32 unk168;
/*16C*/ s32 unk16C;
/*170*/ DLL27_Data collider;    //Terrain collider data
/*3d0*/ s8 _unk3D0[0x400-0x3D0];
/*400*/ HeadAnimation headAnim; //Procedural head animator data
/*424*/ u8 flags;               //See `SnowHorn_Flags`
/*425*/ u8 chatSequenceIdx;     //The index of the ObjSeq to play (from objData->chatSequenceList) when the player interacts
/*426*/ u8 chatSequenceCount;   //The number of ObjSeqs in `objData->chatSequenceList`
/*427*/ u8 mapAct;              //The current map act (queried by Walking SnowHorns, but not used for anything)
/*428*/ s8 garundaTeWeedsEaten; //FrostWeed minigame progress
} SnowHorn_Data;

typedef enum {
    SnowHorn_IDX_0_Grumpy,     //The hungry SnowHorn in the geyser area (object seems to have been called GrumpyMammoth in older builds)
    SnowHorn_IDX_1_WalkingA,   //The SnowHorn who wanders near SwapStone Hollow's entry tunnel
    /* The SnowHorn guarding the entrance to the Krazoa Shrine Transporter, outside the Ice Cave. 
       Not blue by default, but an unused blue SnowHorn model exists which seems intended for this character,
       since the SnowHorn at the equivalent location in SFA is a blue SilverBacked SnowHorn. */
    SnowHorn_IDX_2_Blue,
    SnowHorn_IDX_3_WalkingB,   //The SnowHorn who wanders at a distance from SwapStone Hollow's entry tunnel
    SnowHorn_IDX_4_Garunda_Te, //The SpellStone Guardian
    SnowHorn_IDX_5_Belina_Te   //The SnowHorns' Saviour
} SnowHorn_CharacterIndices;

typedef enum {
    SnowHorn_STATEFLAG_Flinch = 0x4000,         //The SnowHorn's reacting to being hit
    SnowHorn_STATEFLAG_Sleep = 0x8000           //It's the SnowHorn's bedtime
} SnowHorn_StateFlags; //NOTE: these flags temporarily block the character from running their State Machines

typedef enum {
    SnowHorn_FLAG_1_Use_Collider = 1,           //Use terrain collider
    SnowHorn_FLAG_2 = 2,                        //UNUSED
    SnowHorn_FLAG_4_Look_at_Player = 4,         //Enables procedural head aim animation when the player's nearby (provided `SnowHorn_FLAG_40_Enable_Head_Anims` is also set)
    SnowHorn_FLAG_8_Animation_Finished = 8,     //The current animation has ended
    SnowHorn_FLAG_10 = 0x10,                    //UNUSED
    SnowHorn_FLAG_20_Pick_Random_Chat = 0x20,   //Pick a chat sequence index at random when talked to, instead of playing them in order (unused)
    SnowHorn_FLAG_40_Enable_Head_Anims = 0x40,  //Enables procedural head turns/look-ats
    SnowHorn_FLAG_80_Player_Nearby = 0x80       //Set when the player approaches (but not used for anything)
} SnowHorn_Flags;

//NOTE: these State Machine indices skip over some values in order to sync up with the chat ObjSeq index associated with each state.
typedef enum {
    GrumpySnowHorn_STATE_0_Before_Tricky_Command_Tutorial = 0,
    GrumpySnowHorn_STATE_1_Asking_for_Roots = 1,
    GrumpySnowHorn_STATE_2_Waiting_for_1st_Root = 2,
    GrumpySnowHorn_STATE_4_Waiting_for_2nd_Root = 4,
    GrumpySnowHorn_STATE_6_Well_Fed = 6
} GrumpySnowHorn_States; 

typedef enum {
    //Unfinished aiming states: rapidly ascends through them (changing state per tick) until entering firing state
    SnowHornSquirt_STATE_0_Aim_Nearest,
    SnowHornSquirt_STATE_1_Aim_Near,
    SnowHornSquirt_STATE_2_Aim_Far,
    SnowHornSquirt_STATE_3_Aim_Farthest,

    SnowHornSquirt_STATE_4_Firing,
    SnowHornSquirt_STATE_5_Idle
} BlueSnowHorn_States;

typedef enum {
    WalkingSnowHorn_STATE_0_Stopped,
    WalkingSnowHorn_STATE_1_Walking
} WalkingSnowHorn_States;

typedef enum {
    GarundaTe_STATE_0_Trapped_Under_Ice,
    GarundaTe_STATE_1_Trapped_Highlighted,
    GarundaTe_STATE_2_FrostWeed_Minigame,
    GarundaTe_STATE_3_Inhaling_a_FrostWeed,
    GarundaTe_STATE_4_Eating_a_FrostWeed,
    GarundaTe_STATE_5_Broken_Free,
    GarundaTe_STATE_6_Post_DarkIce_Mines,
    GarundaTe_STATE_7_Post_SpellStone_Activation
} GarundaTe_States;

typedef enum  {
    SEQ_157_GrumpySnowHorn_CHAT_Before_Tricky_Command_Tutorial,  //Hmph, shouldn't you help your friend?
    SEQ_157_GrumpySnowHorn_CHAT_Asking_for_Roots,                //I'm too hungry to talk to strangers!
    SEQ_158_GrumpySnowHorn_CHAT_Waiting_for_1st_Root,            //If you press the C-right button you can feed me that root!
    SEQ_159_GrumpySnowHorn_ITEM_Waiting_for_1st_Root,            //One root is never enough, I'm starving! To get out of the clearing...
    SEQ_625_GrumpySnowHorn_CHAT_Waiting_for_2nd_Root,            //One root is never enough, I'm starving!
    SEQ_248_GrumpySnowHorn_ITEM_Waiting_for_2nd_Root,            //Ahh, that hit the spot! Tell you what, young 'un, I'll help you out!
    SEQ_626_GrumpySnowHorn_CHAT_Well_Fed                         //I'm sorry lad, you're on your own now.
} GrumpySnowHorn_ObjSeqIDs;

typedef enum  {
    SEQ_0A0_WalkingSnowHornA_CHAT_1A,       //[ENGLISH ACCENT] The SharpClaws have captured our leader!
    SEQ_0A1_WalkingSnowHornA_CHAT_1B,       //[ENGLISH ACCENT] Beyond the Hollow lies the Prince's homelands.
    SEQ_0A2_WalkingSnowHornB_CHAT_1A,       //[US ACCENT] Do you hear that?
    SEQ_0A3_WalkingSnowHornB_CHAT_1B,       //[US ACCENT] Follow the path to SwapStone Hollow.
    
    SEQ_23D_WalkingSnowHornB_CHAT_2,        //[US ACCENT] Beware the depths of the mine. A monster hides in the ice!
    SEQ_24A_WalkingSnowHornA_CHAT_2,        //[ENGLISH ACCENT] If you help our tribe, I'm sure they'll reward you.
    
    SEQ_270_WalkingSnowHornB_CHAT_3,        //[US ACCENT] Hey, shouldn't you get that SpellStone activated?
    SEQ_3E9_WalkingSnowHornA_CHAT_3,        //[ENGLISH ACCENT] After all that hard work, you have to let the Guardian do his work!
    
    SEQ_261_WalkingSnowHornB_CHAT_4A,       //[US ACCENT] Take the SpellStone to Volcano Force Point.
    SEQ_248_WalkingSnowHornB_CHAT_4B,       //[US ACCENT] As your journey ends, you will meet us again.
    SEQ_249_WalkingSnowHornA_CHAT_4A,       //[ENGLISH ACCENT] You did such a good job, you deserve this!
    SEQ_247_WalkingSnowHornA_CHAT_4B,       //[ENGLISH ACCENT] When you are ready, the sacred lands await.
    
    SEQ_1C8_WalkingSnowHornB_CHAT_5A,       //[US ACCENT] The Ice Cave holds many dangers.
    SEQ_1C0_WalkingSnowHornB_CHAT_5B,       //[US ACCENT] Do not always believe what your eyes tell you is true!
    SEQ_1BF_WalkingSnowHornA_CHAT_5A,       //[ENGLISH ACCENT] Fighting isn't always the answer!
    SEQ_1C1_WalkingSnowHornA_CHAT_5B        //[ENGLISH ACCENT] Your final test awaits!
} WalkingSnowHorn_ObjSeqIDs;

static int SnowHorn_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static s32 SnowHorn_sleep(Object* self);
static void SnowHorn_lookAtPlayerWhenNearby(Object *self, s32 enable);
static void SnowHorn_grumpySetup(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_grumpyControl(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_blueSetup(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_blueControl(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_walkingSetup(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_walkingControl(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_garundaTeSetup(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_garundaTeControl(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_belinaTeSetup(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_belinaTeControl(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);

/*000*/ static Unk80026DF4 dJointHitData[] = {
    {SOUND_377_Metal_Smack,  NO_SOUND, SnowHorn_MODANIM0_47_Flinch_Neutral, SnowHorn_MODANIM0_48_Counterattack, 0,    0.012, 0.005},
    {SOUND_377_Metal_Smack,  NO_SOUND, SnowHorn_MODANIM0_47_Flinch_Neutral, SnowHorn_MODANIM0_48_Counterattack, 0,    0.012, 0.005},
    {SOUND_377_Metal_Smack,  NO_SOUND, SnowHorn_MODANIM0_42_Flinch_Up,      SnowHorn_MODANIM0_48_Counterattack, 0,    0.011, 0.005},
    {SOUND_677_Metal_Clang,  NO_SOUND, -1,                                  -1,                                 TRUE, 0,     0},
    {SOUND_377_Metal_Smack,  NO_SOUND, -1,                                  -1,                                 0,    0,     0},
    {SOUND_377_Metal_Smack,  NO_SOUND, -1,                                  -1,                                 0,    0,     0},
    {SOUND_377_Metal_Smack,  NO_SOUND, -1,                                  -1,                                 0,    0,     0},
    {SOUND_377_Metal_Smack,  NO_SOUND, -1,                                  -1,                                 0,    0,     0},
    {SOUND_677_Metal_Clang,  NO_SOUND, -1,                                  -1,                                 TRUE, 0,     0},
    {SOUND_377_Metal_Smack,  NO_SOUND, -1,                                  -1,                                 0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, SnowHorn_MODANIM0_42_Flinch_Up,      SnowHorn_MODANIM0_48_Counterattack,  0,    0.011, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, SnowHorn_MODANIM0_42_Flinch_Up,      SnowHorn_MODANIM0_48_Counterattack,  0,    0.011, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, SnowHorn_MODANIM0_42_Flinch_Up,      SnowHorn_MODANIM0_48_Counterattack,  0,    0.011, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, SnowHorn_MODANIM0_46_Flinch_Down_R,  SnowHorn_MODANIM0_48_Counterattack,  0,    0.012, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, SnowHorn_MODANIM0_45_Flinch_Down_L,  SnowHorn_MODANIM0_48_Counterattack,  0,    0.012, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, SnowHorn_MODANIM0_44_Flinch_Right,   SnowHorn_MODANIM0_48_Counterattack,  0,    0.015, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, SnowHorn_MODANIM0_43_Flinch_Left,    SnowHorn_MODANIM0_48_Counterattack,  0,    0.015, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1,                                  -1,                                  0,    0,     0}
};

/*230*/ static Vec3f dTerrainTestPoints[] = {
    VEC3F(-12, 0, -20),
    VEC3F(12, 0, -20),
    VEC3F(12, 0, 20),
    VEC3F(-12, 0, 20)
};
/*260*/ static f32 dTerrainRadii[] = {
    0, 0, 0, 0
};
/*270*/ static u8 dIsNightTime = FALSE; //Decides whether SnowHorn should go to sleep
/*274*/ static u8 dPlayAreaObjSeq = FALSE;
/*278*/ static s32 _data_278 = 0;

//State Machine animations for the Walking SnowHorns
/*27C*/ static s16 dWalkingAnims[] = {
    SnowHorn_MODANIM0_0_Idle_LOOP, 
    SnowHorn_MODANIM0_3_Walk_LOOP
};
/*280*/ static f32 dWalkingAnimSpeeds[] = {
    0.005, 
    0.0
};

//State Machine animations for Garunda Te
/*288*/ static s16 dGarundaTeAnims[] = {
    SnowHorn_MODANIM0_35_Garunda_Trapped_Idle_LOOP, 
    SnowHorn_MODANIM0_35_Garunda_Trapped_Idle_LOOP, 
    SnowHorn_MODANIM0_35_Garunda_Trapped_Idle_LOOP, 
    SnowHorn_MODANIM0_35_Garunda_Trapped_Idle_LOOP, 
    SnowHorn_MODANIM0_41_Garunda_Trapped_Eat, 
    SnowHorn_MODANIM0_0_Idle_LOOP, 
    SnowHorn_MODANIM0_0_Idle_LOOP, 
    SnowHorn_MODANIM0_0_Idle_LOOP
};
/*298*/ static f32 dGarundaTeAnimSpeeds[] = {
    0.005, 
    0.005, 
    0.005, 
    0.005, 
    0.008, 
    0.005, 
    0.005, 
    0.005
};

//State Machine animations for Belina Te
/*2B8*/ static s16 dBelinaTeAnims[] = {
    SnowHorn_MODANIM0_0_Idle_LOOP
};
/*2BC*/ static f32 dBelinaTeAnimSpeeds[] = {
    0.005
};

/*2C0*/ static s32 dWalkingBChatSeqs1[] = {
    SEQ_0A0_WalkingSnowHornA_CHAT_1A, //@bug: dWalkingAChatSeqs1 and dWalkingBChatSeqs1's values should be swapped (wrong voice for the character)!
    SEQ_0A1_WalkingSnowHornA_CHAT_1B
};
/*2C8*/ static s32 dWalkingAChatSeqs1[] = {
    SEQ_0A2_WalkingSnowHornB_CHAT_1A, 
    SEQ_0A3_WalkingSnowHornB_CHAT_1B
};
/*2D0*/ static s32 dWalkingBChatSeqs2[] = {
    SEQ_23D_WalkingSnowHornB_CHAT_2
};
/*2D4*/ static s32 dWalkingAChatSeqs2[] = {
    SEQ_24A_WalkingSnowHornA_CHAT_2
};
/*2D8*/ static s32 dWalkingBChatSeqs3[] = {
    SEQ_270_WalkingSnowHornB_CHAT_3
};
/*2DC*/ static s32 dWalkingAChatSeqs3[] = {
    SEQ_3E9_WalkingSnowHornA_CHAT_3
};
/*2E0*/ static s32 dWalkingBChatSeqs4[] = {
    SEQ_261_WalkingSnowHornB_CHAT_4A, 
    SEQ_248_WalkingSnowHornB_CHAT_4B
};
/*2E8*/ static s32 dWalkingAChatSeqs4[] = {
    SEQ_248_WalkingSnowHornB_CHAT_4B, //@bug: should be #10 (SEQ_249_WalkingSnowHornA_CHAT_4A)
    SEQ_247_WalkingSnowHornA_CHAT_4B
};
/*2F0*/ static s32 dWalkingBChatSeqs5[] = { //UNUSED
    SEQ_1C8_WalkingSnowHornB_CHAT_5A, 
    SEQ_1C0_WalkingSnowHornB_CHAT_5B
};
/*2F8*/ static s32 dWalkingAChatSeqs5[] = { //UNUSED
    SEQ_1BF_WalkingSnowHornA_CHAT_5A, 
    SEQ_1C1_WalkingSnowHornA_CHAT_5B
};

// offset: 0x0 | ctor
void SnowHorn_ctor(void* dll) { }

// offset: 0xC | dtor
void SnowHorn_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void SnowHorn_obj_Setup(Object* self, SnowHorn_Setup* objSetup, s32 reset) {
    s32 pad;
    SnowHorn_Data* objData = self->data;
/*300*/ u8 dTerrainColliderArgs[4] = {1, 1, 1, 1};

    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = SnowHorn_animCallback;
    
    if (reset) {
        return;
    }

    objAddObjectType(self, OBJTYPE_SnowHorn);

    objData->animSpeed = 0.005f;
    objData->playerNearbyRange = objSetup->playerNearbyRange;
    objData->squirtInterval = objSetup->squirtInverval * 60;

    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    //Character-specific setup
    switch (objSetup->characterIdx) {
    case SnowHorn_IDX_0_Grumpy:
        SnowHorn_grumpySetup(self, objData, objSetup);
        break;
    case SnowHorn_IDX_2_Blue:
        SnowHorn_blueSetup(self, objData, objSetup);
        break;
    case SnowHorn_IDX_1_WalkingA:
    case SnowHorn_IDX_3_WalkingB:
        SnowHorn_walkingSetup(self, objData, objSetup);
        break;
    case SnowHorn_IDX_4_Garunda_Te:
        SnowHorn_garundaTeSetup(self, objData, objSetup);
        break;
    case SnowHorn_IDX_5_Belina_Te:
        SnowHorn_belinaTeSetup(self, objData, objSetup);
        break;
    }

    //Terrain collider setup
    if (objData->flags & SnowHorn_FLAG_1_Use_Collider) {
        gDLL_27->vtbl->init(&objData->collider, DLL27FLAG_2000000 | DLL27FLAG_4000000, DLL27FLAG_NONE, DLL27MODE_1);
        gDLL_27->vtbl->setup_terrain_collider(&objData->collider, ARRAYCOUNT(dTerrainTestPoints), dTerrainTestPoints, dTerrainRadii, dTerrainColliderArgs);
        gDLL_27->vtbl->reset(self, &objData->collider);
    }

    //Shadows
    self->shadow->flags |= (OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR);
}

// offset: 0x24C | func: 1 | export: 1
void SnowHorn_obj_Control(Object* self) {
    SnowHorn_Data* objData;
    f32 time;
    SnowHorn_Setup* objSetup;
    Object* player;
    UnkFunc_80024108Struct animInfo;
    s32 stateNumber;
    s32 seqIndex;

    objData = self->data;
    objSetup = (SnowHorn_Setup*)self->setup;
    player = objGetPlayer();

    //Check whether the player is nearby
    if (vec3DistanceXZSquared(&self->globalPosition, &player->globalPosition) < (2.0f * SQ(objData->playerNearbyRange))) {
        if ((objData->flags & SnowHorn_FLAG_80_Player_Nearby) == FALSE) {
            objData->flags |= SnowHorn_FLAG_80_Player_Nearby;
        }
    } else {
        if (objData->flags & SnowHorn_FLAG_80_Player_Nearby) {
            objData->flags &= ~SnowHorn_FLAG_80_Player_Nearby;
        }
    }
    
    //Handle procedural blinking/head turn animations 
    if (objData->flags & SnowHorn_FLAG_40_Enable_Head_Anims) {
        SnowHorn_lookAtPlayerWhenNearby(self, (objData->flags & SnowHorn_FLAG_4_Look_at_Player));
        objExpr_func_800328F0(self, &objData->headAnim, objData->walkSpeed);
    }
    objExprEyeIdle(self, &objData->headAnim);

    //React to being hit
    if (func_80026DF4(self, 
        dJointHitData, ARRAYCOUNT(dJointHitData), 
        (objData->state & SnowHorn_STATEFLAG_Flinch ? TRUE : FALSE), 
        &objData->animSpeedFlinch)
    ) {
        objData->state |= SnowHorn_STATEFLAG_Flinch;
        return;
    }
    objData->state &= ~SnowHorn_STATEFLAG_Flinch;

    objData->mapAct = gDLL_29_Gplay->vtbl->get_act(self->mapID);

    //Check whether it's nighttime
    dIsNightTime = gDLL_7_Newday->vtbl->func8(&time);

    //Return early when asleep
    if ((objData->state & SnowHorn_STATEFLAG_Sleep) && SnowHorn_sleep(self)) {
        return;
    }

    //Store player distance
    objData->playerDistance = vec3Distance(&self->globalPosition, &player->globalPosition);

    //Handle character-specific behaviour
    switch (objSetup->characterIdx) {
    case SnowHorn_IDX_0_Grumpy:
        SnowHorn_grumpyControl(self, objData, objSetup);
        break;
    case SnowHorn_IDX_2_Blue:
        SnowHorn_blueControl(self, objData, objSetup);
        break;
    case SnowHorn_IDX_1_WalkingA:
    case SnowHorn_IDX_3_WalkingB:
        SnowHorn_walkingControl(self, objData, objSetup);
        break;
    case SnowHorn_IDX_4_Garunda_Te:
        SnowHorn_garundaTeControl(self, objData, objSetup);
        break;
    case SnowHorn_IDX_5_Belina_Te:
        SnowHorn_belinaTeControl(self, objData, objSetup);
        break;
    }
    
    //Handle terrain collider
    if (objData->flags & SnowHorn_FLAG_1_Use_Collider) {
        gDLL_27->vtbl->func_1E8(self, &objData->collider, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, &objData->collider);
        gDLL_27->vtbl->func_624(self, &objData->collider, gUpdateRateF);
    }

    //Handle animations (synced with State Machine value)
    if (objData->anims) {
        stateNumber = objData->state & ~SnowHorn_STATEFLAG_Sleep;
        if (self->curModAnimId != objData->anims[stateNumber]) {
            objAnimSet(self, objData->anims[stateNumber], 0.0f, 0);

            if (objData->animSpeeds[stateNumber] >= 0.0f) {
                objData->animSpeed = objData->animSpeeds[stateNumber];
            }
            objData->flags &= ~SnowHorn_FLAG_8_Animation_Finished;
        }

        if (objAnimAdvance(self, objData->animSpeed, gUpdateRateF, &animInfo)) {
            objData->flags |= SnowHorn_FLAG_8_Animation_Finished;
        } else {
            objData->flags &= ~SnowHorn_FLAG_8_Animation_Finished;
        }
        objAnim_func_80025780(self, gUpdateRateF, &animInfo, 0);
    }

    //Handle chat sequences
    if (objData->chatSequenceList && (self->unkAF & ARROW_FLAG_1_Interacted)) {
        //Pick a chat objSeq at random (unused), or pick them in sequence
        if (objData->flags & SnowHorn_FLAG_20_Pick_Random_Chat) {
            seqIndex = mathRnd(0, objData->chatSequenceCount - 1);
        } else {
            seqIndex = objData->chatSequenceIdx;
        }

        objData->chatSequenceIdx++;
        if (objData->chatSequenceIdx >= objData->chatSequenceCount) {
            objData->chatSequenceIdx = 0;
        }

        gDLL_3_Animation->vtbl->start_obj_sequence(objData->chatSequenceList[seqIndex], self, -1);
        joyDisableButtons(0, A_BUTTON);
    }

}

// offset: 0x770 | func: 2 | export: 2
void SnowHorn_obj_Update(Object* self){ }

// offset: 0x77C | func: 3 | export: 3
void SnowHorn_obj_Print(Object* self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    SnowHorn_Data* objData = self->data;

    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);

        //Get the coordinates of the trunk's end attachPoint
        objGetAttachPointWorldSpace(self, 1, &objData->trunkAttachPoint.x, &objData->trunkAttachPoint.y, &objData->trunkAttachPoint.z, 0);
    }
}

// offset: 0x804 | func: 4 | export: 4
void SnowHorn_obj_Free(Object* self, s32 onlySelf) {
    dPlayAreaObjSeq = FALSE;
}

// offset: 0x828 | func: 5 | export: 5
u32 SnowHorn_obj_GetModelFlags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8 | MODFLAGS_EVENTS | MODFLAGS_80;
}

// offset: 0x838 | func: 6 | export: 6
u32 SnowHorn_obj_GetDataSize(Object *self, s32 offsetAddr) {
    return sizeof(SnowHorn_Data);
}

// offset: 0x84C | func: 9
static int SnowHorn_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    SnowHorn_Data* objdata;
    s32 i;

    objdata = self->data;

    if (prevCallbackValue != 0) {
        objAnimAdvance(self, 0.005f, gUpdateRateF, NULL);
    }

    if (objdata->flags & SnowHorn_FLAG_1_Use_Collider) {
        gDLL_27->vtbl->reset(self, &objdata->collider);
    }

    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    animData->unk62 = 0;

    //Grant SnowHorn Wastes' map via a sequence message
    for (i = 0; i < animData->messageCount; i++){
        if (animData->messages[i] == 3) {
            mainSetBits(BIT_Map_SW, 1);
            continue;
        }        
    }
    
    objdata->walkSpeed = 0.0f;
    
    return 0;
}

// offset: 0x980 | func: 10
/** SnowHorn sleep state machine: handles anims and sounds */
static s32 SnowHorn_sleep(Object* self) {
    UnkFunc_80024108Struct animInfo;
    SnowHorn_Data* objData;
    TextureAnimator* eyelidR;
    TextureAnimator* eyelidL;
    s32 animIsFinished;
    s32 playSound; //toggles between 0 and 1 (when ready to play sound another time)

    objData = self->data;
    animIsFinished = objAnimAdvance(self, 0.006f, gUpdateRateF, &animInfo);
    
    if (animInfo.unk1B != 0) {
        playSound = animInfo.unk13[0] == 0;
    } else {
        playSound = FALSE;
    }
    
    eyelidR = objExprGetTexAnimator(self, 5, 0);
    eyelidL = objExprGetTexAnimator(self, 4, 0);

    self->unkAF |= ARROW_FLAG_8_No_Targetting;    
    
    switch (self->curModAnimId) {
    case SnowHorn_MODANIM0_4_Sleep_Intro:
        if (playSound) {
            dll_amSfx->Play(self, SOUND_129_SnowHorn_Yawn_1, MAX_VOLUME, 0, 0, 0, 0);
        }

        if (animIsFinished) {
            objAnimSet(self, SnowHorn_MODANIM0_5_Sleep_LOOP, 0.0f, 0);
            if (eyelidR != NULL) {
                eyelidR->frame = 0x200;
            }
            if (eyelidL != NULL) {
                eyelidL->frame = 0x200;
            }
            objData->sleepTimer = mathRnd(0, 300);
        }
        break;
    case SnowHorn_MODANIM0_5_Sleep_LOOP:
        if (playSound) {
            dll_amSfx->Play(self, SOUND_12A_SnowHorn_SnoreHorn, MAX_VOLUME, 0, 0, 0, 0);
        }

        objData->sleepTimer -= gUpdateRate;

        //Play wake-up animation when it's daytime
        if ((dIsNightTime == FALSE) && (objData->sleepTimer <= 0)) {  
            objAnimSet(self, SnowHorn_MODANIM0_6_Sleep_Outro, 0.0f, 0);
            if (eyelidR != NULL) {
                eyelidR->frame = 0;
            }
            if (eyelidL != NULL) {
                eyelidL->frame = 0;
            }
        }
        break;
    case SnowHorn_MODANIM0_6_Sleep_Outro: //Waking up
        if (playSound) {
            dll_amSfx->Play(self, SOUND_12B_SnowHorn_Yawn_2, MAX_VOLUME, 0, 0, 0, 0);
        }

        if (animIsFinished) {
            objAnimSet(self, SnowHorn_MODANIM0_0_Idle_LOOP, 0.0f, 0); //Play idle animation
            objData->state &= ~SnowHorn_STATEFLAG_Sleep;
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            return 0;
        }
        break;
    default:
        objData->sleepTimer -= gUpdateRate;

        //Go to sleep
        if (objData->sleepTimer <= 0) { 
            objAnimSet(self, SnowHorn_MODANIM0_4_Sleep_Intro, 0.0f, 0);
            objData->walkSpeed = 0.0f;
        }
        break;
    }

    return 1;
}

// offset: 0xCC4 | func: 11
static void SnowHorn_lookAtPlayerWhenNearby(Object *self, s32 enable){
    SnowHorn_Data *objData;
    Object *player;
      
    objData = self->data;
    player = objGetPlayer();
      
    if (enable && (player != NULL) && (objData->playerDistance < 200.0f)){
        objData->headAnim.aimIsActive = TRUE;
        objData->headAnim.headAimX = player->srt.transl.x;
        objData->headAnim.headAimY = player->srt.transl.y;
        objData->headAnim.headAimZ = player->srt.transl.z;
    } else {
        objData->headAnim.aimIsActive = FALSE;
    }
}

// offset: 0xD5C | func: 12
static void SnowHorn_grumpySetup(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    objData->state = 0;
    objData->flags |= SnowHorn_FLAG_40_Enable_Head_Anims | SnowHorn_FLAG_4_Look_at_Player;
    objData->playerNearbyRange = objSetup->playerNearbyRange;
}

// offset: 0xD80 | func: 13
static void SnowHorn_grumpyControl(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    Object* player;

    if (dIsNightTime) {
        objData->sleepTimer = mathRnd(0, 300);
        objData->state |= SnowHorn_STATEFLAG_Sleep;

        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        self->unkAF &= ~ARROW_FLAG_1_Interacted;
        return;
    }
    objData->state &= ~SnowHorn_STATEFLAG_Sleep;

    //Handle animations
    if (self->curModAnimId != SnowHorn_MODANIM0_0_Idle_LOOP) {
        objAnimSet(self, SnowHorn_MODANIM0_0_Idle_LOOP, 0.0f, 0);
    }
    objAnimAdvance(self, objData->animSpeed, gUpdateRate, NULL);

    player = objGetPlayer();
    if (!player) 
        return;
    
    //Try to play a sequence when the player is a distance away
    if (vec3DistanceSquared(&self->globalPosition, &player->globalPosition) > SQ((f32)objData->playerNearbyRange)) {
        objData->sleepTimer += gUpdateRate;
        if (objData->sleepTimer > 900) {
            gDLL_3_Animation->vtbl->start_obj_sequence(7, self, -1); //NOTE: out-of-bounds ObjSeqIdx!
            objData->sleepTimer = -mathRnd(0, 50);
        }
        return;
    } 
    
    objData->sleepTimer = 0;
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

    switch (objData->state) {
    u32 rootsEaten;
    case GrumpySnowHorn_STATE_0_Before_Tricky_Command_Tutorial:
        //Wait for Sabre to defeat the SharpClaw chasing Tricky
        if (mainGetBits(BIT_SnowHorn_Tutorial_Defeated_SharpClaw)) {
            objData->state = GrumpySnowHorn_STATE_1_Asking_for_Roots;
        }
        break;    
    case GrumpySnowHorn_STATE_1_Asking_for_Roots:
        //Check how many roots have been eaten
        rootsEaten = mainGetBits(BIT_SnowHorn_Tutorial_NumRootsFed);
        switch (rootsEaten) {
        case 0:
            //Advance state if no roots have been eaten and an Alpine Root has been collected
            if (mainGetBits(BIT_SnowHorn_Tutorial_GotAlpineRoot1) || 
                mainGetBits(BIT_SnowHorn_Tutorial_GotAlpineRoot2))
            {
                objData->state = GrumpySnowHorn_STATE_2_Waiting_for_1st_Root;
            }
            break;
        case 1:
            //Advance state if one Alpine Root has been eaten
            objData->state = GrumpySnowHorn_STATE_4_Waiting_for_2nd_Root;
            break;
        default:
            //Advance state if both Alpine Roots have been eaten
            objData->state = GrumpySnowHorn_STATE_6_Well_Fed;
            break;
        }
        break;
    case GrumpySnowHorn_STATE_2_Waiting_for_1st_Root:
        //Play the first root eating sequence when an Alpine Root is offered
        if ((self->unkAF & ARROW_FLAG_4_Highlighted) && gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_SW_Alpine_Roots)) {
            mainSetBits(BIT_SnowHorn_Tutorial_NumRootsFed, 1);
            mainDecrementBits(BIT_SW_Alpine_Roots);
            gDLL_3_Animation->vtbl->start_obj_sequence(SEQ_159_GrumpySnowHorn_ITEM_Waiting_for_1st_Root, self, -1);
            objData->state = GrumpySnowHorn_STATE_4_Waiting_for_2nd_Root;
            return;
        }
        break;
    case GrumpySnowHorn_STATE_4_Waiting_for_2nd_Root:
        //Play the second root eating sequence when an Alpine Root is offered
        if ((self->unkAF & ARROW_FLAG_4_Highlighted) && gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_SW_Alpine_Roots)) {
            mainSetBits(BIT_SnowHorn_Tutorial_NumRootsFed, 2);
            mainDecrementBits(BIT_SW_Alpine_Roots);
            gDLL_3_Animation->vtbl->start_obj_sequence(SEQ_248_GrumpySnowHorn_ITEM_Waiting_for_2nd_Root, self, -1);
            objData->state = GrumpySnowHorn_STATE_6_Well_Fed;
            return;
        }
        break;
    case GrumpySnowHorn_STATE_6_Well_Fed:
        break;
    }
    
    //Play chat sequence
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        self->unkAF &= ~ARROW_FLAG_1_Interacted;
        if (objData->state <= GrumpySnowHorn_STATE_6_Well_Fed) {
            gDLL_3_Animation->vtbl->start_obj_sequence(objData->state, self, -1);
            joyDisableButtons(0, A_BUTTON);
        }
    }
}

// offset: 0x11C4 | func: 14
static void SnowHorn_blueSetup(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    objData->flags |= SnowHorn_FLAG_40_Enable_Head_Anims | SnowHorn_FLAG_4_Look_at_Player;
}

// offset: 0x11E0 | func: 15
static void SnowHorn_blueControl(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    SnowHorn_Data* squirtData;
    SeqJoint* seqJoint;
    Object* player;
    SRT fxTransform;
    Vec3f v;
    u32 pad;
    s32 squirtYaw;
    s32 animFinished;
    s16 *rotate;
    s32 aimYawDiff;
    Vec3f f;

    squirtData = self->data;
    
    animFinished = objAnimAdvance(self, 0.005f, gUpdateRate, NULL);

    //Squirt State Machine (unfinished!)
    //Seems intended to shoot water at the player, but the aiming calculations go unused
    switch (squirtData->state) {
    case SnowHornSquirt_STATE_0_Aim_Nearest:
        //Rotate an aim vector based on the head joint's yaw (aiming far and low)
        {
            v.x = 0.0f;
            v.y = 10.0f;
            v.z = -25.0f;

            fxTransform.transl.z = 0.0f;
            fxTransform.transl.y = 0.0f;
            fxTransform.transl.x = 0.0f;
            fxTransform.roll = 0;
            fxTransform.pitch = 0;
            fxTransform.yaw = squirtData->headYaw;
            fxTransform.scale = 0.0f;
            mathRotateRPY(&fxTransform, v.f);
        }

        //Convert rotated aim vector into worldSpace SRT, by adding trunk's worldSpace XZ and SnowHorn's Y
        {
            fxTransform.transl.x = squirtData->trunkAttachPoint.x + v.f[0];
            fxTransform.transl.y = self->srt.transl.y + v.f[1];
            fxTransform.transl.z = squirtData->trunkAttachPoint.z + v.f[2];
            fxTransform.yaw = 0;
        }

        squirtData->state = SnowHornSquirt_STATE_1_Aim_Near;
        break;
    case SnowHornSquirt_STATE_1_Aim_Near:
        if (self->animProgress > 0.25f) {
            //Rotate an aim vector based on the head joint's yaw (aiming a little higher)
            {
                v.x = 0.0f;
                v.y = 20.0f;
                v.z = -20.0f;

                fxTransform.transl.z = 0.0f;
                fxTransform.transl.y = 0.0f;
                fxTransform.transl.x = 0.0f;
                fxTransform.roll = 0;
                fxTransform.pitch = 0;
                fxTransform.yaw = squirtData->headYaw;
                fxTransform.scale = 0.0f;
                mathRotateRPY(&fxTransform, v.f);
            }

            //Convert rotated aim vector into worldSpace SRT, by adding trunk's worldSpace XZ and SnowHorn's Y
            {
                fxTransform.transl.f[0] = squirtData->trunkAttachPoint.f[0] + v.f[0];
                fxTransform.transl.f[1] = self->srt.transl.f[1] + v.f[1];
                fxTransform.transl.f[2] = squirtData->trunkAttachPoint.f[2] + v.f[2];
                fxTransform.yaw = 0;
            }

            squirtData->state = SnowHornSquirt_STATE_2_Aim_Far;
        }
        break;
    case SnowHornSquirt_STATE_2_Aim_Far:
        if (self->animProgress > 0.65f) {
            //Rotate an aim vector based on the head joint's yaw (aiming straight ahead)
            {
                v.x = 0.0f;
                v.y = 0.0f;
                v.z = -40.0f;

                fxTransform.transl.z = 0.0f;
                fxTransform.transl.y = 0.0f;
                fxTransform.transl.x = 0.0f;
                fxTransform.roll = 0;
                fxTransform.pitch = 0;
                fxTransform.yaw = squirtData->headYaw;
                fxTransform.scale = 0.0f;
                mathRotateRPY(&fxTransform, v.f);
            }

            //Store the SnowHorn's global position
            f.x = self->globalPosition.x;
            f.y = self->globalPosition.y;
            f.z = self->globalPosition.z;

            //Set the SnowHorn's globalPosition to the endpoint of their trunk!
            //(Is this a mistake? Maybe they meant to set fxTransform's translation here?)
            self->globalPosition.x = squirtData->trunkAttachPoint.x;
            self->globalPosition.y = self->srt.transl.y + 40.0f;
            self->globalPosition.z = squirtData->trunkAttachPoint.z;
            fxTransform.yaw = 0;

            //Set the SnowHorn's globalPosition back to its previous value? (Very odd!)
            self->globalPosition.x = f.x;
            self->globalPosition.y = f.y;
            self->globalPosition.z = f.z;

            squirtData->state = SnowHornSquirt_STATE_3_Aim_Farthest;
        }
        break;
    case SnowHornSquirt_STATE_3_Aim_Farthest:
        //Rotate an aim vector based on the head joint's yaw (aiming way ahead)
        {
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = -60.0f;

            fxTransform.transl.z = 0.0f;
            fxTransform.transl.y = 0.0f;
            fxTransform.transl.x = 0.0f;
            fxTransform.roll = 0;
            fxTransform.pitch = 0;
            fxTransform.yaw = squirtData->headYaw;
            fxTransform.scale = 0.0f;
            mathRotateRPY(&fxTransform, v.f);
        }

        //Store the SnowHorn's global position
        f.x = self->globalPosition.x;
        f.y = self->globalPosition.y;
        f.z = self->globalPosition.z;

        //Set the SnowHorn's globalPosition to the endpoint of their trunk!
        //(Is this a mistake? Maybe they meant to set fxTransform's translation here?)
        self->globalPosition.x = squirtData->trunkAttachPoint.f[0] + v.f[0];
        self->globalPosition.y = self->globalPosition.f[1] + v.f[1];
        self->globalPosition.z = squirtData->trunkAttachPoint.f[2] + v.f[2];
        fxTransform.yaw = 0;

        //Set the SnowHorn's globalPosition back to its previous value? (Very odd!)
        self->globalPosition.x = f.x;
        self->globalPosition.y = f.y;
        self->globalPosition.z = f.z;

        squirtData->state = SnowHornSquirt_STATE_4_Firing;
        break;
    case SnowHornSquirt_STATE_4_Firing:
        //Return to idle state at end of squirt animation
        if (animFinished) {
            objAnimSet(self, SnowHorn_MODANIM0_0_Idle_LOOP, 0.0f, 0);
            squirtData->state = SnowHornSquirt_STATE_5_Idle;
        }
        break;
    case SnowHornSquirt_STATE_5_Idle:
        //Go to sleep at night
        if (dIsNightTime) {
            squirtData->state |= SnowHorn_STATEFLAG_Sleep;
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            self->unkAF &= ~ARROW_FLAG_1_Interacted;
            break;
        }

        //Squirt water at the player
        squirtData->timer += gUpdateRate;
        if (squirtData->timer > squirtData->squirtInterval) {
            squirtData->timer = 0;

            //Get the head seqJoint's yaw
            seqJoint = objExpr_func_80034804(self, 0);
            squirtData->headYaw = M_180_DEGREES - seqJoint->yaw;

            player = objGetPlayer();
            
            //Get the angle to the location the player will be at 1 second in the future
            aimYawDiff = (
                mathAtan2f((player->globalPosition.x + (player->velocity.x * 60.0f)) - self->globalPosition.x, 
                           (player->globalPosition.z + (player->velocity.z * 60.0f)) - self->globalPosition.z) 
                - (self->srt.yaw & 0xFFFF)) + M_180_DEGREES;

            CIRCLE_WRAP(aimYawDiff);
            
            objAnimSet(self, SnowHorn_MODANIM0_1_Squirt, 0.0f, 0);

            if ((-3000 < aimYawDiff) && (aimYawDiff < 3000)) {
                squirtData->aimTarget.x = player->srt.transl.x;
                squirtData->aimTarget.y = player->srt.transl.y;
                squirtData->aimTarget.z = player->srt.transl.z;
                squirtData->aimYaw = squirtData->headYaw;
            } else {
                rotate = &self->srt.yaw;
                squirtYaw = seqJoint->yaw + rotate[0];
                squirtData->aimTarget.x = self->srt.transl.x - (mathSinfInterp(squirtYaw) * 250.0f);
                squirtData->aimTarget.y = self->srt.transl.y;
                squirtData->aimTarget.z = self->srt.transl.z - (mathCosfInterp(squirtYaw) * 250.0f);
                squirtData->aimYaw = 0;
            }

            squirtData->state = SnowHornSquirt_STATE_0_Aim_Nearest;
        }
        break;
    }
}

// offset: 0x174C | func: 16
static void SnowHorn_walkingSetup(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup){
    s32 curveType = 0x19;
    
    objData->flags |= SnowHorn_FLAG_40_Enable_Head_Anims | SnowHorn_FLAG_4_Look_at_Player | SnowHorn_FLAG_1_Use_Collider;

    objData->anims = dWalkingAnims;
    objData->animSpeeds = dWalkingAnimSpeeds;

    //Set up walk curves
    if (gDLL_26_Curves->vtbl->func_4288(&objData->curves, self, 1000.0f, &curveType, -1) == 0){
        self->srt.transl.x = objData->curves.unk0.unk68.x;
        self->srt.transl.z = objData->curves.unk0.unk68.z;
        
        objData->state = WalkingSnowHorn_STATE_1_Walking;
        objData->walkSpeed = 0.5f;
    } else {
        objData->state = WalkingSnowHorn_STATE_0_Stopped;

        STUBBED_PRINTF("MAM: curve setup failed\n");
    }

    switch (objData->mapAct) {
    case 1:
        break;
    }

    //Set up different chat sequences based on game progress
    if (objSetup->characterIdx == SnowHorn_IDX_1_WalkingA){
        //NOTE: an unused 5th chat list exists too (dWalkingAChatSeqs5), intended for before the Test of Sacrifice!

        if (mainGetBits(BIT_SpellStone_DIM_Activated)){
            //List 4: After activating the SpellStone
            objData->chatSequenceList = dWalkingAChatSeqs4;
            objData->chatSequenceCount = ARRAYCOUNT(dWalkingAChatSeqs4);
        } else if (mainGetBits(BIT_SpellStone_DIM)){
            //List 3: After DarkIce Mines
            objData->chatSequenceList = dWalkingAChatSeqs3;
            objData->chatSequenceCount = ARRAYCOUNT(dWalkingAChatSeqs3);
        } else if (mainGetBits(BIT_Tricky_Learned_Distract)){
            //List 2: After Distract
            objData->chatSequenceList = dWalkingAChatSeqs2;
            objData->chatSequenceCount = ARRAYCOUNT(dWalkingAChatSeqs2);
        } else {
            //List 1: Initial (@bug: uses wrong voice actor compared to other chatSequenceLists)
            objData->chatSequenceList = dWalkingAChatSeqs1;
            objData->chatSequenceCount = ARRAYCOUNT(dWalkingAChatSeqs1);
        }
    } else {
        //NOTE: an unused 5th chat list exists too (dWalkingBChatSeqs5), intended for before the Test of Sacrifice!

        if (mainGetBits(BIT_SpellStone_DIM_Activated)){
            //List 4: After activating the SpellStone
            objData->chatSequenceList = dWalkingBChatSeqs4;
            objData->chatSequenceCount = ARRAYCOUNT(dWalkingBChatSeqs4);
        } else if (mainGetBits(BIT_SpellStone_DIM)){
            //List 3: After DarkIce Mines
            objData->chatSequenceList = dWalkingBChatSeqs3;
            objData->chatSequenceCount = ARRAYCOUNT(dWalkingBChatSeqs3);
        } else if (mainGetBits(BIT_Tricky_Learned_Distract)){
            //List 2: After Distract
            objData->chatSequenceList = dWalkingBChatSeqs2;
            objData->chatSequenceCount = ARRAYCOUNT(dWalkingBChatSeqs2);
        } else {
            //List 1: Initial (@bug: uses wrong voice actor compared to other chatSequenceLists)
            objData->chatSequenceList = dWalkingBChatSeqs1;
            objData->chatSequenceCount = ARRAYCOUNT(dWalkingBChatSeqs1);
        }
    }
}

// offset: 0x1980 | func: 17
static void SnowHorn_walkingControl(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    s32 pad;
    f32 dx;
    f32 dz;
    f32 speed;
    UnkCurvesStruct* curveStruct;

    //Handle walk speed
    if (dIsNightTime) {
        //Slow to a stop, and go to sleep
        if (objData->walkSpeed > 0.0f) {
            objData->walkSpeed -= 0.025f;
        } else {
            objData->state |= SnowHorn_STATEFLAG_Sleep;
            objData->walkSpeed = 0.0f;
            objData->sleepTimer = mathRnd(0, 300);
            return;
        }
    } else if ((self->unkAF & ARROW_FLAG_4_Highlighted) || (objData->playerDistance < 80.0f)) {
        //Gradually come to a stop when near the player
        objData->walkSpeed -= 0.025f;
        if (objData->walkSpeed < 0.05f) {
            objData->walkSpeed = 0.0f;
        }
    } else {
        //Accelerate back to walking speed
        objData->walkSpeed += 0.025f;
        if (objData->walkSpeed > 0.5f) {
            objData->walkSpeed = 0.5f;
        }
    }
    
    //Play a sequence when entering an Area object with a specific value (unused?)
    if (dPlayAreaObjSeq && objGetAreaValueAtPoint(self->globalPosition.x, self->globalPosition.y, self->globalPosition.z) == 0xA){
        //NOTE: objSeqID out of bounds! Maybe a deleted/planned sequence?
        gDLL_3_Animation->vtbl->start_obj_sequence(16, self, -1);
        return;
    }

    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

    //Handle walk states
    if (objData->state != WalkingSnowHorn_STATE_0_Stopped) {
        curveStruct = &objData->curves;

        //Return early if state has any `SnowHorn_StateFlags` set
        if (objData->state != WalkingSnowHorn_STATE_1_Walking) {
            objData->state = WalkingSnowHorn_STATE_0_Stopped;
            return;
        }

        //Move along the walk curve
        if (curves_func_800053B0(&curveStruct->unk0, objData->walkSpeed) || curveStruct->unk0.unk10) {
            gDLL_26_Curves->vtbl->func_4704(curveStruct);
        }

        //Advance animation based on curve position delta
        dx = curveStruct->unk0.unk68.x - self->srt.transl.x;
        dz = curveStruct->unk0.unk68.z - self->srt.transl.z;
        speed = sqrtf(SQ(dx) + SQ(dz)) * gUpdateRateInverseF;
        objGetAnimChange(self, speed, &objData->animSpeed);

        //Set yaw based on curve tangent
        self->srt.yaw = mathAtan2f(curveStruct->unk0.unk74, curveStruct->unk0.unk7C) + M_180_DEGREES;
        
        //Set position using curve point
        self->srt.transl.x = curveStruct->unk0.unk68.x;
        self->srt.transl.z = curveStruct->unk0.unk68.z;

        //Don't look at the player while walking
        objData->flags &= ~SnowHorn_FLAG_4_Look_at_Player;
        
        //Switch to stopped state
        if (objData->walkSpeed <= 0.0f) {
            objData->state = WalkingSnowHorn_STATE_0_Stopped;
        }
    } else {
        //Look at nearby player while stopped
        objData->flags |= SnowHorn_FLAG_4_Look_at_Player;

        //Switch to walking state
        if (objData->walkSpeed > 0.1f) {
            objData->state = WalkingSnowHorn_STATE_1_Walking;
        }
    }
}

// offset: 0x1CA0 | func: 18
static void SnowHorn_garundaTeSetup(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    objData->state = mainGetBits(BIT_Garunda_Te_Quest_Progress);
    
    //Make sure Garunda Te is in his end state if DarkIce Mines' SpellStone is activated
    if ((objData->state < GarundaTe_STATE_6_Post_DarkIce_Mines) && (mainGetBits(BIT_SpellStone_DIM))){
        objData->state = GarundaTe_STATE_6_Post_DarkIce_Mines;
        mainSetBits(BIT_Garunda_Te_Quest_Progress, objData->state);
    }
    
    objData->anims = dGarundaTeAnims;
    objData->animSpeeds = dGarundaTeAnimSpeeds;

    objData->garundaTeWeedsEaten = mainGetBits(BIT_Garunda_Te_Weeds_Eaten);
}

// offset: 0x1D68 | func: 19
static void SnowHorn_garundaTeControl(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    Object* frostWeed;
    
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

    switch (objData->state) {
    case GarundaTe_STATE_0_Trapped_Under_Ice:
        //Call out to the player periodically
        objData->timer += gUpdateRate;
        if (objData->timer > 1000) {
            dll_amSfx->Play(self, SOUND_1E2_Garunda_Te_Will_somebody_get_me_out_of_here, MAX_VOLUME, 0, 0, 0, 0);
            gDLL_22_Subtitles->vtbl->func_368(GAMETEXT_00A_SW_Calling_for_help);
            objData->timer = 0;
        }

        //Advance state when the player approaches
        if (self->unkAF & ARROW_FLAG_4_Highlighted) {
            objData->state = GarundaTe_STATE_1_Trapped_Highlighted;
        }
        break;
    case GarundaTe_STATE_1_Trapped_Highlighted:
        //Play Garunda Te's intro sequence when talked to, and advance state
        if (objCheckPlayerInteract(self)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
            objData->state = GarundaTe_STATE_2_FrostWeed_Minigame;
            mainSetBits(BIT_Garunda_Te_Quest_Progress, objData->state);
        }
        break;
    case GarundaTe_STATE_2_FrostWeed_Minigame:
        //Chat with the player when interacted with
        if (objCheckPlayerInteract(self)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
        }
    
        //Eat FrostWeeds when they're nearby
        frostWeed = objGetNearestTypeTo(OBJTYPE_Baddie, self, 0);
        objSetup = (SnowHorn_Setup*)self->setup;
        if ((frostWeed != NULL) && (frostWeed->id == OBJ_Tumbleweed2) && 
            (vec3DistanceXZSquared(&self->globalPosition, &frostWeed->globalPosition) < SQ(objSetup->playerNearbyRange))
        ) {
            if (dll_tumbleweed(frostWeed)->is_gravitating(frostWeed) == FALSE) {
                //Attract the FrostWeed towards Garunda Te's trunk
                dll_tumbleweed(frostWeed)->gravitate_towards_point(frostWeed, &objData->trunkAttachPoint);
                objData->frostWeed = frostWeed;

                //@debug code (skipping FrostWeed quest)
                if (FROSTWEED_QUEST_CHEAT){
                    objData->garundaTeWeedsEaten = GARUNDA_TE_WEEDS_NEEDED;
                }

                //Increment weed count
                objData->garundaTeWeedsEaten++;
                if (objData->garundaTeWeedsEaten > GARUNDA_TE_WEEDS_NEEDED) {
                    objData->garundaTeWeedsEaten = GARUNDA_TE_WEEDS_NEEDED;
                }

                mainSetBits(BIT_Garunda_Te_Weeds_Eaten, objData->garundaTeWeedsEaten);
                objData->state = GarundaTe_STATE_3_Inhaling_a_FrostWeed;
            }
        }
        break;
    case GarundaTe_STATE_3_Inhaling_a_FrostWeed:
        //Wait for the FrostWeed to finish being attracted
        if (vec3DistanceXZSquared(&objData->trunkAttachPoint, &objData->frostWeed->globalPosition) < SQ(2.5f)) {
            objData->state = GarundaTe_STATE_4_Eating_a_FrostWeed;
        }
        break;
    case GarundaTe_STATE_4_Eating_a_FrostWeed:
        if (objData->flags & SnowHorn_FLAG_8_Animation_Finished) {
            //Finish the FrostWeed quest when Garunda Te has eaten 12 weeds
            if (objData->garundaTeWeedsEaten >= GARUNDA_TE_WEEDS_NEEDED) {
                mainSetBits(BIT_Garunda_Te_Fed, TRUE);

                objData->state = GarundaTe_STATE_5_Broken_Free;
                mainSetBits(BIT_Garunda_Te_Quest_Progress, objData->state);
                break;
            }

            //Talk after each 3 FrostWeeds
            if ((objData->garundaTeWeedsEaten % 3) == 0) {
                dll_amSfx->Play(self, SOUND_74B_Garunda_Te_That_tastes_great_Hurry_up_boy, MAX_VOLUME, 0, 0, 0, 0);
                gDLL_22_Subtitles->vtbl->func_368(GAMETEXT_002_SW_FrostWeed_feast);
            }

            //Continue on with the FrostWeed minigame if 12 FrostWeeds haven't been eaten yet
            objData->state = GarundaTe_STATE_2_FrostWeed_Minigame;
        }
        break;
    case GarundaTe_STATE_5_Broken_Free:
        //Play a chat sequence when interacted with after the SpellStone Guardians sequence
        if (objCheckPlayerInteract(self)) {
            if (objData->chatSequenceIdx % 2) {
                gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
            } else {
                gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
            }
            objData->chatSequenceIdx++;
        }
        break;
    case GarundaTe_STATE_6_Post_DarkIce_Mines:
        //Chat when talked to, or activate the SpellStone when it's picked in the inventory
        if (objCheckPlayerInteract(self)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
        } else if (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_SpellStone_DIM)) {
            mainSetBits(BIT_SpellStone_DIM_Activated, 1);
            objData->state = GarundaTe_STATE_7_Post_SpellStone_Activation;
            mainSetBits(BIT_Garunda_Te_Quest_Progress, objData->state);
        }
        break;
    case GarundaTe_STATE_7_Post_SpellStone_Activation:
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        break;
    }
    
    //Print the progress through the Frostweeds minigame
    if ((objData->state >= GarundaTe_STATE_2_FrostWeed_Minigame) && 
        (objData->state <= GarundaTe_STATE_4_Eating_a_FrostWeed)
    ) {
        diPrintf("noweeds=%d\n", objData->garundaTeWeedsEaten);
    }
}

// offset: 0x22E4 | func: 20
static void SnowHorn_belinaTeSetup(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    objData->anims = dBelinaTeAnims;
    objData->animSpeeds = dBelinaTeAnimSpeeds;
}

// offset: 0x2318 | func: 21
static void SnowHorn_belinaTeControl(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) { }
