//"NWmammoth.c" seems to have been Rare's name!

#include "PR/gbi.h"
#include "PR/os.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/26_curves.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/227_tumbleweed.h"
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
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objlib.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/objprint.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "sys/objlib.h"
#include "types.h"
#include "sys/objexpr.h"

#ifdef DEBUG
#define FROSTWEED_QUEST_CHEAT 1  
#else  
#define FROSTWEED_QUEST_CHEAT 0  
#endif 

enum SnowHornAnims {
    MODANIM_SnowHorn_Idle = 0,
    MODANIM_SnowHorn_Talk = 2,
    MODANIM_SnowHorn_Walk = 3,
    MODANIM_SnowHorn_Sleep_Intro = 4,
    MODANIM_SnowHorn_Sleep = 5,
    MODANIM_SnowHorn_Wake_Up = 6,
    MODANIM_SnowHorn_Hit_React = 47
};

enum SnowHornTutorialSequences {
    SEQ_0157_SnowHorn_Chat_BeforeDefeatingSharpClaw = 0, //Hmph, shouldn't you help your friend?
    SEQ_0157_SnowHorn_Chat_BeforeDiggingUpRoot = 1, //I'm too hungry to talk to strangers!
    SEQ_0158_SnowHorn_Chat_AfterDiggingUpRoot = 2, //If you press the C-right button you can feed me that root!
    SEQ_0159_SnowHorn_Cutscene_FeedingRoot1 = 3, //One root is never enough, I'm starving! To get out of the clearing...
    SEQ_0625_SnowHorn_Chat_AfterEatingRoot1 = 4, //One root is never enough, I'm starving!
    SEQ_0248_SnowHorn_Cutscene_FeedingRoot2 = 5, //Ahh, that hit the spot! Tell you what, young 'un, I'll help you out!
    SEQ_0626_SnowHorn_Chat_AfterEatingRoot2 = 6  //I'm sorry lad, you're on your own now.
};

#define GARUNDA_TE_WEEDS_NEEDED 12

typedef struct{
/*0x10*/ ObjSetup base;
/*0x18*/ s16 unkRadius;
/*0x1A*/ s16 unk1A;
/*0x1C*/ s8 yaw;
/*0x1D*/ s8 unk1D;
} SnowHorn_Setup;

typedef struct {
/*000*/ s32 *unk0;
/*004*/ s16 unkRadius;
/*006*/ s16 unk6;
/*008*/ s16 unk8;
/*00A*/ s16 sleepTimer; //randomly-assigned value?
/*00c*/ u16 flags;
/*00e*/ u16 unkE; //yaw?
/*010*/ f32 unk10;
/*014*/ Vec3f playerPositionCopy;
/*020*/ f32 distanceFromPlayer;
/*024*/ s8 unk24;
/*025*/ s8 unk25;
/*026*/ s8 unk26;
/*027*/ s8 unk27;
/*028*/ Object* frostWeed;
/*02c*/ s16 unk2C;
/*02e*/ s16 unk2E;
/*030*/ s32 unk30;
/*034*/ s32 unk34;
/*038*/ f32 unk38;
/*03c*/ f32 unk3C;
/*040*/ f32 unk40;
/*044*/ s16* anims;
/*048*/ f32* animSpeeds;
/*04c*/ s32* chatSequenceList;
/*050*/ f32 animSpeed;
/*054*/ f32 unk54;
/*058*/ f32 walkSpeed; //has something to do with the struct at 0x60?
/*05C*/ s32 unk5C;
/*060*/ UnkCurvesStruct unk60;
/*168*/ s32 unk168;
/*16C*/ s32 unk16C;
/*170*/ DLL27_Data collider;
/*3d0*/ s8 _unk3D0[0x3E0-0x3D0];
/*3e0*/ u32 unk3e0;
/*3e4*/ u32 unk3e4;
/*3e8*/ u32 unk3e8;
/*3ec*/ u32 unk3ec;
/*3f0*/ u32 unk3f0;
/*3f4*/ u32 unk3f4;
/*3f8*/ u32 unk3f8;
/*3fc*/ u32 unk3fc;
/*400*/ HeadAnimation lookAtUnk;
/*424*/ u8 unk424;
/*425*/ u8 chatSequenceIdx;
/*426*/ u8 chatSequenceCount;
/*427*/ u8 unk427;
/*428*/ s8 garundaTe_weedsEaten;
/*429*/ s8 unk429;
/*42A*/ s8 unk42A;
/*42B*/ s8 unk42B;
} SnowHorn_Data;

static int SnowHorn_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static s32 SnowHorn_func_980(Object* self);
static void SnowHorn_func_CC4(Object *self, s32 doLookAt);
static void SnowHorn_func_D5C(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_func_D80(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_func_11C4(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_func_11E0(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_func_174C(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_func_1980(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_func_1CA0(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_func_1D68(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_func_22E4(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);
static void SnowHorn_func_2318(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup);

static const char _rodata_0[] = "MAM: curve setup failed\n";

/*000*/ static Unk80026DF4 dJointHitSounds[] = {
    {SOUND_377_Metal_Smack,  NO_SOUND, 47, 48, FALSE, 0.012, 0.005},
    {SOUND_377_Metal_Smack,  NO_SOUND, 47, 48, FALSE, 0.012, 0.005},
    {SOUND_377_Metal_Smack,  NO_SOUND, 42, 48, FALSE, 0.011, 0.005},
    {SOUND_677_Metal_Clang,  NO_SOUND, -1, -1, TRUE,  0.0,   0.0},
    {SOUND_377_Metal_Smack,  NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack,  NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack,  NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack,  NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_677_Metal_Clang,  NO_SOUND, -1, -1, TRUE,  0.0,   0.0},
    {SOUND_377_Metal_Smack,  NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 42, 48, FALSE, 0.011, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 42, 48, FALSE, 0.011, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 42, 48, FALSE, 0.011, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, 46, 48, FALSE, 0.012, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 45, 48, FALSE, 0.012, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 44, 48, FALSE, 0.015, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, 43, 48, FALSE, 0.015, 0.005},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, FALSE, 0.0,   0.0}
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
/*274*/ static u8 _data_274 = 0;
/*278*/ static s32 _data_278 = 0;
/*27C*/ static s16 _data_27C[] = {
    0, 3
};
/*280*/ static f32 _data_280[] = {
    0.005, 0.0
};
/*288*/ static s16 _data_288[] = {
    35, 35, 35, 35, 41, 0, 0, 0
};
/*298*/ static f32 _data_298[] = {
    0.005, 0.005, 0.005, 0.005, 0.008, 0.005, 0.005, 0.005
};
/*2B8*/ static s16 _data_2B8[] = {0};
/*2BC*/ static f32 _data_2BC[] = {0.005};
/*2C0*/ static s32 _data_2C0[] = {
    0x0, 0x1
};
/*2C8*/ static s32 _data_2C8[] = {
    0x2, 0x3
};
/*2D0*/ static s32 _data_2D0[] = {
    0x4
};
/*2D4*/ static s32 _data_2D4[] = {
    0x5
};
/*2D8*/ static s32 _data_2D8[] = {
    0x6
};
/*2DC*/ static s32 _data_2DC[] = {
    0x7
};
/*2E0*/ static s32 _data_2E0[] = {
    0x8, 0x9
};
/*2E8*/ static s32 _data_2E8[] = {
    0x9, 0xb
};
/*2F0*/ static s32 _data_2F0[] = {
    0xc, 0xd, 0xe, 0xf
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
    objData->unkRadius = objSetup->unkRadius;
    objData->unk6 = objSetup->unk1A * 60;
    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    switch (objSetup->unk1D) {
        case 0:
            SnowHorn_func_D5C(self, objData, objSetup);
            break;
        case 2:
            SnowHorn_func_11C4(self, objData, objSetup);
            break;
        case 1:
        case 3:
            SnowHorn_func_174C(self, objData, objSetup);
            break;
        case 4:
            SnowHorn_func_1CA0(self, objData, objSetup);
            break;
        case 5:
            SnowHorn_func_22E4(self, objData, objSetup);
            break;
    }

    if (objData->unk424 & 1) {
        gDLL_27->vtbl->init(&objData->collider, DLL27FLAG_2000000 | DLL27FLAG_4000000, DLL27FLAG_NONE, DLL27MODE_1);
        gDLL_27->vtbl->setup_terrain_collider(&objData->collider, ARRAYCOUNT(dTerrainTestPoints), dTerrainTestPoints, dTerrainRadii, dTerrainColliderArgs);
        gDLL_27->vtbl->reset(self, &objData->collider);
    }

    self->shadow->flags |= (OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR);
}

// offset: 0x24C | func: 1 | export: 1
void SnowHorn_obj_Control(Object* self) {
    SnowHorn_Data* objData;
    f32 time;
    SnowHorn_Setup* objSetup;
    Object* player;
    UnkFunc_80024108Struct animInfo;
    s32 animIndex;
    s32 seqIndex;

    objData = self->data;
    objSetup = (SnowHorn_Setup*)self->setup;
    player = objGetPlayer();

    if (vec3DistanceXZSquared(&self->globalPosition, &player->globalPosition) < (2.0f * SQ(objData->unkRadius))) {
        if (!(objData->unk424 & 0x80)) {
            objData->unk424 |= 0x80;
        }
    } else {
        if (objData->unk424 & 0x80) {
            objData->unk424 &= ~0x80;
        }
    }
    
   if (objData->unk424 & 0x40) {
        SnowHorn_func_CC4(self, objData->unk424 & 4);
        objExpr_func_800328F0(self, &objData->lookAtUnk, objData->walkSpeed);
    }
    objExprEyeIdle(self, &objData->lookAtUnk);

    if (func_80026DF4(self, dJointHitSounds, 0x1C, (objData->flags & 0x4000 ? 1 : 0), &objData->unk54)) {
        objData->flags |= 0x4000;
        return;
    }
    
    objData->flags &= ~0x4000;
    objData->unk427 = gDLL_29_Gplay->vtbl->get_act(self->mapID);

    //Check whether it's nighttime
    dIsNightTime = gDLL_7_Newday->vtbl->func8(&time);

    if (objData->flags & 0x8000 && SnowHorn_func_980(self)){
        return;
    }

    objData->distanceFromPlayer = vec3Distance(&self->globalPosition, &player->globalPosition);
    switch (objSetup->unk1D) {
        case 0:
            SnowHorn_func_D80(self, objData, objSetup);
            break;
        case 2:
            SnowHorn_func_11E0(self, objData, objSetup);
            break;
        case 1:
        case 3:
            SnowHorn_func_1980(self, objData, objSetup);
            break;
        case 4:
            SnowHorn_func_1D68(self, objData, objSetup);
            break;
        case 5:
            SnowHorn_func_2318(self, objData, objSetup);
            break;
    }
    
    if (objData->unk424 & 1) {
        gDLL_27->vtbl->func_1E8(self, &objData->collider, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, &objData->collider);
        gDLL_27->vtbl->func_624(self, &objData->collider, gUpdateRateF);
    }

    if (objData->anims) {
        animIndex = objData->flags & ~0x8000;
        if (self->curModAnimId != objData->anims[animIndex]) {
            objAnimSet(self, objData->anims[animIndex], 0.0f, 0);

            if (objData->animSpeeds[animIndex] >= 0.0f) {
                objData->animSpeed = objData->animSpeeds[animIndex];
            }
            objData->unk424 &= ~8;
        }

        if (objAnimAdvance(self, objData->animSpeed, gUpdateRateF, &animInfo)) {
            objData->unk424 |= 8;
        } else {
            objData->unk424 &= ~8;
        }
        objAnim_func_80025780(self, gUpdateRateF, &animInfo, 0);
    }

    if (objData->chatSequenceList && (self->unkAF & ARROW_FLAG_1_Interacted)) {
        if (objData->unk424 & 0x20) {
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
        objGetAttachPointWorldSpace(self, 1, &objData->playerPositionCopy.x, &objData->playerPositionCopy.y, &objData->playerPositionCopy.z, 0);
    }
}

// offset: 0x804 | func: 4 | export: 4
void SnowHorn_obj_Free(Object* self, s32 onlySelf) {
    _data_274 = 0;
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

    if (objdata->unk424 & 1) {
        gDLL_27->vtbl->reset(self, &objdata->collider);
    }

    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    animData->unk62 = 0;

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
static s32 SnowHorn_func_980(Object* self) {
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
        case MODANIM_SnowHorn_Sleep_Intro:
            if (playSound) {
                dll_amSfx->Play(self, SOUND_129_SnowHorn_Yawn_1, MAX_VOLUME, 0, 0, 0, 0);
            }

            if (animIsFinished) {
                objAnimSet(self, MODANIM_SnowHorn_Sleep, 0.0f, 0);
                if (eyelidR != NULL) {
                    eyelidR->frame = 0x200;
                }
                if (eyelidL != NULL) {
                    eyelidL->frame = 0x200;
                }
                objData->sleepTimer = mathRnd(0, 300);
            }
            break;
        case MODANIM_SnowHorn_Sleep:
            if (playSound) {
                dll_amSfx->Play(self, SOUND_12A_SnowHorn_SnoreHorn, MAX_VOLUME, 0, 0, 0, 0);
            }

            objData->sleepTimer-= gUpdateRate;

            //Play wake-up animation when it's daytime
            if ((dIsNightTime == FALSE) && (objData->sleepTimer <= 0)) {  
                objAnimSet(self, MODANIM_SnowHorn_Wake_Up, 0.0f, 0);
                if (eyelidR != NULL) {
                    eyelidR->frame = 0;
                }
                if (eyelidL != NULL) {
                    eyelidL->frame = 0;
                }
            }
            break;
        case MODANIM_SnowHorn_Wake_Up:
            if (playSound) {
                dll_amSfx->Play(self, SOUND_12B_SnowHorn_Yawn_2, MAX_VOLUME, 0, 0, 0, 0);
            }

            if (animIsFinished) {
                objAnimSet(self, MODANIM_SnowHorn_Idle, 0.0f, 0); //Play idle animation
                objData->flags &= ~0x8000;
                self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                return 0;
            }
            break;
        default:
            objData->sleepTimer -= gUpdateRate;

            //Go to sleep
            if (objData->sleepTimer <= 0) { 
                objAnimSet(self, MODANIM_SnowHorn_Sleep_Intro, 0.0f, 0);
                objData->walkSpeed = 0.0f;
            }
            break;
    }

    return 1;
}

// offset: 0xCC4 | func: 11
/** Updates the SnowHorn's player position reference when nearby (for the look-at behaviour) */
static void SnowHorn_func_CC4(Object *self, s32 doLookAt){
    SnowHorn_Data *objData;
    Object *player;
      
    objData = self->data;
    player = objGetPlayer();
      
    if (doLookAt && (player != NULL) && (objData->distanceFromPlayer < 200.0f)){
        objData->lookAtUnk.aimIsActive = TRUE;
        objData->lookAtUnk.headAimX = player->srt.transl.x;
        objData->lookAtUnk.headAimY = player->srt.transl.y;
        objData->lookAtUnk.headAimZ = player->srt.transl.z;
    } else {
        objData->lookAtUnk.aimIsActive = FALSE;
    }
}

// offset: 0xD5C | func: 12
static void SnowHorn_func_D5C(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    objData->flags = 0;
    objData->unk424 |= 0x40 | 4;
    objData->unkRadius = objSetup->unkRadius;
}

// offset: 0xD80 | func: 13
/** Called by the standing SnowHorn (not by the ones that walk around) */
static void SnowHorn_func_D80(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    Object* player;

    if (dIsNightTime) {
        objData->sleepTimer = mathRnd(0, 300);
        objData->flags |= 0x8000;

        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        self->unkAF &= ~ARROW_FLAG_1_Interacted;
        return;
    }
    
    objData->flags &= ~0x8000;

    if (self->curModAnimId != 0) {
        objAnimSet(self, 0, 0.0f, 0);
    }
    objAnimAdvance(self, objData->animSpeed, gUpdateRate, NULL);

    player = objGetPlayer();
    if (!player) 
        return;
    
    if (SQ((f32)objData->unkRadius) < vec3DistanceSquared(&self->globalPosition, &player->globalPosition)) {
        objData->sleepTimer += gUpdateRate;
        if (objData->sleepTimer > 900) {
            gDLL_3_Animation->vtbl->start_obj_sequence(7, self, -1);
            objData->sleepTimer = -mathRnd(0, 50);
        }
        return;
    } 
    
    objData->sleepTimer = 0;
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

    switch (objData->flags) {
        u32 rootsEaten;
        case 0:
            if (mainGetBits(BIT_SnowHorn_Tutorial_Defeated_SharpClaw)) {
                objData->flags = 1;
            }
            break;    
        case 1:
            rootsEaten = mainGetBits(BIT_SnowHorn_Tutorial_NumRootsFed);
            switch (rootsEaten) {
                case 0:
                    if (mainGetBits(BIT_SnowHorn_Tutorial_GotAlpineRoot1) || 
                        mainGetBits(BIT_SnowHorn_Tutorial_GotAlpineRoot2))
                    {
                        objData->flags = 2;
                    }
                    break;
                case 1:
                    objData->flags = 4;
                    break;
                default:
                    objData->flags = 6;
                    break;
            }
            break;
        case 2:
            if ((self->unkAF & ARROW_FLAG_4_Highlighted) && gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_SW_Alpine_Roots)) {
                mainSetBits(BIT_SnowHorn_Tutorial_NumRootsFed, 1);
                mainDecrementBits(BIT_SW_Alpine_Roots);
                gDLL_3_Animation->vtbl->start_obj_sequence(SEQ_0159_SnowHorn_Cutscene_FeedingRoot1, self, -1);
                objData->flags = 4;
                return;
            }
            break;
        case 4:
            if ((self->unkAF & ARROW_FLAG_4_Highlighted) && gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_SW_Alpine_Roots)) {
                mainSetBits(BIT_SnowHorn_Tutorial_NumRootsFed, 2);
                mainDecrementBits(BIT_SW_Alpine_Roots);
                gDLL_3_Animation->vtbl->start_obj_sequence(SEQ_0248_SnowHorn_Cutscene_FeedingRoot2, self, -1);
                objData->flags = 2 | 4;
                return;
            }
            break;
        case 6:
            break;
    }
    
    //Play chat sequence
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        self->unkAF &= ~ARROW_FLAG_1_Interacted;
        if (objData->flags < (1 | 2 | 4)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(objData->flags, self, -1);
            joyDisableButtons(0, A_BUTTON);
        }
    }

}

// offset: 0x11C4 | func: 14
static void SnowHorn_func_11C4(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    objData->unk424 |= 0x40 | 4;
}

// offset: 0x11E0 | func: 15
static void SnowHorn_func_11E0(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    SnowHorn_Data* objData2;
    SeqJoint* seqJoint;
    Object* player;
    SRT srt;
    Vec3f v;
    u32 pad;
    s32 seqJointAngle;
    s32 animFinished;
    s16 *temp;
    s32 var_v1;
    Vec3f f;

    objData2 = self->data;
    
    animFinished = objAnimAdvance(self, 0.005f, gUpdateRate, NULL);

    switch (objData2->flags) {
    case 0:
        v.x = 0.0f;
        v.y = 10.0f;
        v.z = -25.0f;
        srt.transl.z = 0.0f;
        srt.transl.y = 0.0f;
        srt.transl.x = 0.0f;
        srt.roll = 0;
        srt.pitch = 0;
        srt.yaw = objData2->unkE;
        srt.scale = 0.0f;
        mathRotateRPY(&srt, v.f);
        srt.transl.f[0] = objData2->playerPositionCopy.f[0] + v.f[0];
        srt.transl.f[1] = self->srt.transl.f[1] + v.f[1];
        srt.transl.f[2] = objData2->playerPositionCopy.f[2] + v.f[2];
        srt.yaw = 0;
        objData2->flags = 1;
        break;
    case 1:
        if (self->animProgress > 0.25f) {
            v.x = 0.0f;
            v.y = 20.0f;
            v.z = -20.0f;
            srt.transl.z = 0.0f;
            srt.transl.y = 0.0f;
            srt.transl.x = 0.0f;
            srt.roll = 0;
            srt.pitch = 0;
            srt.yaw = objData2->unkE;
            srt.scale = 0.0f;
            mathRotateRPY(&srt, v.f);
            srt.transl.f[0] = objData2->playerPositionCopy.f[0] + v.f[0];
            srt.transl.f[1] = self->srt.transl.f[1] + v.f[1];
            srt.transl.f[2] = objData2->playerPositionCopy.f[2] + v.f[2];
            srt.yaw = 0;
            objData2->flags = 2;
        }
        break;
    case 2:
        if (self->animProgress > 0.65f) {
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = -40.0f;
            srt.transl.z = 0.0f;
            srt.transl.y = 0.0f;
            srt.transl.x = 0.0f;
            srt.roll = 0;
            srt.pitch = 0;
            srt.yaw = objData2->unkE;
            srt.scale = 0.0f;
            mathRotateRPY(&srt, v.f);
            f.x = self->globalPosition.x;
            f.y = self->globalPosition.y;
            f.z = self->globalPosition.z;

            self->globalPosition.x = objData2->playerPositionCopy.x;
            self->globalPosition.y = self->srt.transl.y + 40.0f;
            self->globalPosition.z = objData2->playerPositionCopy.z;
            srt.yaw = 0;

            self->globalPosition.x = f.x;
            self->globalPosition.y = f.y;
            self->globalPosition.z = f.z;
            objData2->flags = 3;
        }
        break;
    case 3:
        v.x = 0.0f;
        v.y = 0.0f;
        v.z = -60.0f;
        srt.transl.z = 0.0f;
        srt.transl.y = 0.0f;
        srt.transl.x = 0.0f;
        srt.roll = 0;
        srt.pitch = 0;
        srt.yaw = objData2->unkE;
        srt.scale = 0.0f;
        mathRotateRPY(&srt, v.f);

        f.x = self->globalPosition.x;
        f.y = self->globalPosition.y;
        f.z = self->globalPosition.z;

        self->globalPosition.x = objData2->playerPositionCopy.f[0] + v.f[0];
        self->globalPosition.y = self->globalPosition.f[1] + v.f[1];
        self->globalPosition.z = objData2->playerPositionCopy.f[2] + v.f[2];
        srt.yaw = 0;

        self->globalPosition.x = f.x;
        self->globalPosition.y = f.y;
        self->globalPosition.z = f.z;
        objData2->flags = 4;
        break;
    case 4:
        if (animFinished != 0) {
            objAnimSet(self, 0, 0.0f, 0);
            objData2->flags = 5;
        }
        break;
    case 5:
        if (dIsNightTime != 0) {
            objData2->flags |= M_180_DEGREES;
            self->unkAF |= 8;
            self->unkAF &= ~1;
            break;
        }
        objData2->unk8 += gUpdateRate;
        if (objData2->unk6 < objData2->unk8) {
            objData2->unk8 = 0;
            seqJoint = objExpr_func_80034804(self, 0);
            objData2->unkE = M_180_DEGREES - seqJoint->yaw;

            player = objGetPlayer();
            
            var_v1 = (mathAtan2f((player->globalPosition.x + (player->velocity.x * 60.0f)) - self->globalPosition.x, (player->globalPosition.z + (player->velocity.z * 60.0f)) - self->globalPosition.z) - (self->srt.yaw & 0xFFFF)) + 0x8000;
            CIRCLE_WRAP(var_v1)
            
            objAnimSet(self, 1, 0.0f, 0);

            if ((var_v1 > -3000) && (var_v1 < 3000)) {
                objData2->unk38 = player->srt.transl.x;
                objData2->unk3C = player->srt.transl.y;
                objData2->unk40 = player->srt.transl.z;
                objData2->unk2C = objData2->unkE;
            } else {
                temp = &self->srt.yaw;
                seqJointAngle = seqJoint->yaw + *temp;
                objData2->unk38 = self->srt.transl.x - (mathSinfInterp(seqJointAngle) * 250.0f);
                objData2->unk3C = self->srt.transl.y;
                objData2->unk40 = self->srt.transl.z - (mathCosfInterp(seqJointAngle) * 250.0f);
                objData2->unk2C = 0;
            }

            objData2->flags = 0;
        }
        break;
    }
}

// offset: 0x174C | func: 16
static void SnowHorn_func_174C(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup){
    s32 sp2C = 0x19;
    
    objData->unk424 |= 0x40 | 4 | 1;
    objData->anims = _data_27C;
    objData->animSpeeds = _data_280;

    if (!gDLL_26_Curves->vtbl->func_4288(&objData->unk60, self, 1000.0f, &sp2C, -1)){
        self->srt.transl.x = objData->unk60.unk0.unk68.x;
        self->srt.transl.z = objData->unk60.unk0.unk68.z;
        
        objData->flags = 1;
        objData->walkSpeed = 0.5f;
    } else {
        objData->flags = 0;
    }

    switch (objData->unk427) {
        case 1:
            break;
    }

    if (objSetup->unk1D == 1){
        if (mainGetBits(BIT_SpellStone_DIM_Activated)){
            objData->chatSequenceList = _data_2E8;
            objData->chatSequenceCount = ARRAYCOUNT(_data_2E8);
        } else if (mainGetBits(BIT_SpellStone_DIM)){
            objData->chatSequenceList = _data_2DC;
            objData->chatSequenceCount = ARRAYCOUNT(_data_2DC);
        } else if (mainGetBits(BIT_Tricky_Learned_Distract)){
            objData->chatSequenceList = _data_2D4;
            objData->chatSequenceCount = ARRAYCOUNT(_data_2D4);
        } else {
            objData->chatSequenceList = _data_2C8;
            objData->chatSequenceCount = ARRAYCOUNT(_data_2C8);
        }
    } else {
        if (mainGetBits(BIT_SpellStone_DIM_Activated)){
            objData->chatSequenceList = _data_2E0;
            objData->chatSequenceCount = ARRAYCOUNT(_data_2E0);
        } else if (mainGetBits(BIT_SpellStone_DIM)){
            objData->chatSequenceList = _data_2D8;
            objData->chatSequenceCount = ARRAYCOUNT(_data_2D8);
        } else if (mainGetBits(BIT_Tricky_Learned_Distract)){
            objData->chatSequenceList = _data_2D0;
            objData->chatSequenceCount = ARRAYCOUNT(_data_2D8);
        } else {
            objData->chatSequenceList = _data_2C0;
            objData->chatSequenceCount = ARRAYCOUNT(_data_2C0);
        }
    }
}

// offset: 0x1980 | func: 17
static void SnowHorn_func_1980(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    s32 pad;
    f32 dx;
    f32 dz;
    f32 speed;
    UnkCurvesStruct* curveStruct;

    if (dIsNightTime) {
        if (objData->walkSpeed > 0.0f) {
            objData->walkSpeed = objData->walkSpeed - 0.025f;
        } else {
            objData->flags |= 0x8000;
            objData->walkSpeed = 0.0f;
            objData->sleepTimer = mathRnd(0, 300);
            return;
        }
    } else if ((self->unkAF & ARROW_FLAG_4_Highlighted) || (objData->distanceFromPlayer < 80.0f)) {
        //Gradually come to a stop when near the player
        objData->walkSpeed -= 0.025f;
        if (objData->walkSpeed < 0.05f) {
            objData->walkSpeed = 0.0f;
        }
    } else {
        objData->walkSpeed += 0.025f;
        if (objData->walkSpeed > 0.5f) {
            objData->walkSpeed = 0.5f;
        }
    }
    
    if (_data_274 != 0 && 
        objGetAreaValueAtPoint(self->globalPosition.x, self->globalPosition.y, self->globalPosition.z) == 0xA){
        gDLL_3_Animation->vtbl->start_obj_sequence(0x10, self, -1);
        return;
    }

    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

    if (objData->flags != 0) {
        curveStruct = &objData->unk60;
        if (objData->flags != 1) {
            objData->flags = 0;
            return;
        }

        if ((curves_func_800053B0(&curveStruct->unk0, objData->walkSpeed) != 0) || (curveStruct->unk0.unk10 != 0)) {
            gDLL_26_Curves->vtbl->func_4704(curveStruct);
        }

        dx = curveStruct->unk0.unk68.x - self->srt.transl.x;
        dz = curveStruct->unk0.unk68.z - self->srt.transl.z;

        speed = sqrtf(SQ(dx) + SQ(dz)) * gUpdateRateInverseF;
        objGetAnimChange(self, speed, &objData->animSpeed);

        self->srt.yaw = mathAtan2f(curveStruct->unk0.unk74, curveStruct->unk0.unk7C) + M_180_DEGREES;
        self->srt.transl.x = curveStruct->unk0.unk68.x;
        self->srt.transl.z = curveStruct->unk0.unk68.z;
        objData->unk424 &= ~0x4;
        
        if (objData->walkSpeed <= 0.0f) {
            objData->flags = 0;
        }
    } else {
        objData->unk424 |= 4;
        if (objData->walkSpeed > 0.1f) {
            objData->flags = 1;
        }
    }
}

// offset: 0x1CA0 | func: 18
static void SnowHorn_func_1CA0(Object *self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    objData->flags = mainGetBits(BIT_Garunda_Te_Quest_Progress);
    
    // If the 1st SpellStone is activated, make sure Garunda Te is in his end state
    if (objData->flags < 6 && mainGetBits(BIT_SpellStone_DIM)){
        objData->flags = 6;
        mainSetBits(BIT_Garunda_Te_Quest_Progress, objData->flags);
    }
    
    objData->anims = _data_288;
    objData->animSpeeds = _data_298;
    objData->garundaTe_weedsEaten = mainGetBits(BIT_Garunda_Te_Weeds_Eaten);
}

// offset: 0x1D68 | func: 19
static void SnowHorn_func_1D68(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    Object* frostWeed;
    
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

    switch (objData->flags) {
        case 0:
            //Calling out to the player periodically
            objData->unk8 += gUpdateRate;
            if (objData->unk8 > 1000) {
                dll_amSfx->Play(self, SOUND_1E2_Garunda_Te_Will_somebody_get_me_out_of_here, MAX_VOLUME, 0, 0, 0, 0);
                gDLL_22_Subtitles->vtbl->func_368(GAMETEXT_00A_SW_Calling_for_help);
                objData->unk8 = 0;
            }

            if (self->unkAF & ARROW_FLAG_4_Highlighted) {
                objData->flags = 1;
            }
            break;
        case 1:
            if (objCheckPlayerInteract(self)) {
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                objData->flags = 2;
                mainSetBits(BIT_Garunda_Te_Quest_Progress, objData->flags);
            }
            break;
        case 2:
            //Chat with the player when interacted with
            if (objCheckPlayerInteract(self)) {
                gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            }
        
            //Eat nearby FrostWeeds
            frostWeed = objGetNearestTypeTo(OBJTYPE_Baddie, self, 0);
            objSetup = (SnowHorn_Setup*)self->setup;
            if ((frostWeed != NULL) && (frostWeed->id == OBJ_Tumbleweed2) && 
                (vec3DistanceXZSquared(&self->globalPosition, &frostWeed->globalPosition) < SQ(objSetup->unkRadius))
            ) {
                if (dll_tumbleweed(frostWeed)->is_gravitating(frostWeed) == FALSE) {
                    dll_tumbleweed(frostWeed)->gravitate_towards_point(frostWeed, &objData->playerPositionCopy);
                    objData->frostWeed = frostWeed;

                    //@debug code (skipping FrostWeed quest)
                    if (FROSTWEED_QUEST_CHEAT){
                        objData->garundaTe_weedsEaten = GARUNDA_TE_WEEDS_NEEDED;
                    }

                    objData->garundaTe_weedsEaten++;
                    if (objData->garundaTe_weedsEaten > GARUNDA_TE_WEEDS_NEEDED) {
                        objData->garundaTe_weedsEaten = GARUNDA_TE_WEEDS_NEEDED;
                    }
                    mainSetBits(BIT_Garunda_Te_Weeds_Eaten, objData->garundaTe_weedsEaten);
                    objData->flags = 3;
                }
            }
            break;
        case 3:
            if (vec3DistanceXZSquared(&objData->playerPositionCopy, &objData->frostWeed->globalPosition) < SQ(2.5f)) {
                objData->flags = 4;
            }
            break;
        case 4:
            if (objData->unk424 & 8) {
                if (objData->garundaTe_weedsEaten >= GARUNDA_TE_WEEDS_NEEDED) {
                    mainSetBits(BIT_Garunda_Te_Fed, 1);
                    objData->flags = 5;
                    mainSetBits(BIT_Garunda_Te_Quest_Progress, objData->flags);
                    break;
                }

                if (objData->garundaTe_weedsEaten % 3 == 0) {
                    dll_amSfx->Play(self, SOUND_74B_Garunda_Te_That_tastes_great_Hurry_up_boy, MAX_VOLUME, 0, 0, 0, 0);
                    gDLL_22_Subtitles->vtbl->func_368(2);
                }

                objData->flags = 2;
            }
            break;
        case 5:
            if (objCheckPlayerInteract(self)) {
                if (objData->chatSequenceIdx % 2) {
                    gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
                } else {
                    gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
                }
                objData->chatSequenceIdx += 1;
            }
            break;
        case 6:
            //SpellStone activation
            if (objCheckPlayerInteract(self)) {
                gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
            } else if (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_SpellStone_DIM)) {
                mainSetBits(BIT_SpellStone_DIM_Activated, 1);
                objData->flags = 7;
                mainSetBits(BIT_Garunda_Te_Quest_Progress, objData->flags);
            }
            break;
        case 7:
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            break;
    }
    
    if (objData->flags >= 2 && objData->flags < 5) {
        diPrintf("noweeds=%d\n", objData->garundaTe_weedsEaten);
    }
}

// offset: 0x22E4 | func: 20
static void SnowHorn_func_22E4(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) {
    objData->anims = _data_2B8;
    objData->animSpeeds = _data_2BC;
}

// offset: 0x2318 | func: 21
static void SnowHorn_func_2318(Object* self, SnowHorn_Data* objData, SnowHorn_Setup* objSetup) { }
