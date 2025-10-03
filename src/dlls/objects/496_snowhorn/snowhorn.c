//"NWmammoth.c" seems to have been Rare's name!

#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/227_tumbleweed.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "variables.h"
#include "functions.h"
#include "types.h"

s32 func_800053B0(void*, f32);
s32 func_8002493C(void*, f32, void*);
s32 func_80026DF4(Object*, u32*, u32, s32, void*);
s32 func_80031BBC(f32, f32, f32);
s32 func_80032538(Object* self);
void func_800328F0(Object*, void*, f32);
void func_80032A08(Object*, void*);
s16* func_80034804(Object* obj, s32 sequenceBoneID);
s32* func_800348A0(Object*, s32, s32);
void set_button_mask(int port, u16 mask);

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

enum SnowHornGameBits {
    FLAG_Always_1 = 0x95,
    FLAG_Always_0 = 0x96,
    INVENTORY_Alpine_Root = 0x576,
    FLAG_SnowHorn_Tutorial_Defeated_SharpClaw = 0xD3,
    FLAG_SnowHorn_Tutorial_NumRootsFed = 0x578,
    FLAG_SnowHorn_Tutorial_GotAlpineRoot1 = 0x8E,
    FLAG_SnowHorn_Tutorial_GotAlpineRoot2 = 0x116,
    FLAG_Garunda_Te_Quest_Progress = 0x115,
    FLAG_Garunda_Te_NumFrostWeedsEaten = 0x48B,
    FLAG_SnowHorn_Unknown = 0x5A0, //Maybe intended to hide SnowHorn Wastes' minimap until SnowHorn gifts map?
    FLAG_DarkIce_Mines_Leap_of_Faith_Completed = 0x22B,
    FLAG_Tricky_Distract_Learned = 0x9E,
    FLAG_SpellStone_1_Activated = 0x123
};

#define GARUNDA_TE_WEEDS_NEEDED 12

typedef struct {
/*000*/ s32 *unk0;
/*004*/ s16 unkRadius;
/*006*/ s16 unk6;
/*008*/ s16 unk8;
/*00A*/ s16 sleepTimer; //randomly-assigned value?
/*00c*/ u16 flags;
/*00e*/ u16 unkE; //yaw?
/*010*/ s32 unk10;
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
/*038*/ s32 unk38;
/*03c*/ s32 unk3C;
/*040*/ s32 unk40;
/*044*/ s16* someAnimIDList;
/*048*/ f32* unk48;
/*04c*/ s32* chatSequenceList;
/*050*/ f32 unk50;
/*054*/ s32 unk54;
/*058*/ f32 walkSpeed; //has something to do with the struct at 0x60?
/*05C*/ s32 unk5C;
/*060*/ UnkCurvesStruct60 unk60;
/*0E0*/ s32 unkE0;
/*0E4*/ s32 unkE4;
/*0E8*/ s32 unkE8;
/*0EC*/ s32 unkEC;
/*0F0*/ s32 unkF0;
/*0F4*/ s32 unkF4;
/*0F8*/ s32 unkF8;
/*0FC*/ s32 unkFC;
/*100*/ s32 unk100;
/*104*/ s32 unk104;
/*108*/ s32 unk108;
/*10C*/ s32 unk10C;
/*110*/ s32 unk110;
/*114*/ s32 unk114;
/*118*/ s32 unk118;
/*11C*/ s32 unk11C;
/*110*/ s32 unk120;
/*114*/ s32 unk124;
/*118*/ s32 unk128;
/*11C*/ s32 unk12C;
/*110*/ s32 unk130;
/*114*/ s32 unk134;
/*118*/ s32 unk138;
/*11C*/ s32 unk13C;
/*110*/ s32 unk140;
/*114*/ s32 unk144;
/*118*/ s32 unk148;
/*11C*/ s32 unk14C;
/*110*/ s32 unk150;
/*114*/ s32 unk154;
/*118*/ s32 unk158;
/*11C*/ s32 unk15C;
/*110*/ s32 unk160;
/*114*/ s32 unk164;
/*118*/ s32 unk168;
/*11C*/ s32 unk16C;
/*110*/ Vec3f* unk170;
/*114*/ s32 unk174;
/*118*/ Vec3f unk178[8]; //position samples/deltas - maybe for walk-related calculus?
/*1d8*/ s8 unk1d8[0x208];
/*3e0*/ u32 unk3e0;
/*3e4*/ u32 unk3e4;
/*3e8*/ u32 unk3e8;
/*3ec*/ u32 unk3ec;
/*3f0*/ u32 unk3f0;
/*3f4*/ u32 unk3f4;
/*3f8*/ u32 unk3f8;
/*3fc*/ u32 unk3fc;
/*400*/ s8 lookAtUnk;
/*401*/ s8 unk401;
/*402*/ s8 unk402;
/*403*/ s8 unk403;
/*404*/ f32 copyPlayerX;
/*408*/ f32 copyPlayerY;
/*40c*/ f32 copyPlayerZ;
/*410*/ f32 unk410;
/*414*/ f32 unk414;
/*418*/ f32 unk418;
/*41C*/ f32 unk41C;
/*420*/ f32 unk420;
/*424*/ u8 unk424;
/*425*/ u8 unk425;
/*426*/ u8 unk426;
/*427*/ u8 unk427;
/*428*/ s8 garundaTe_weedsEaten;
/*429*/ s8 unk429;
/*42A*/ s8 unk42A;
/*42B*/ s8 unk42B;
/*42C*/ f32 unk42C;
/*42C*/ s8 unk430[0x16c];
/*59c*/ s16 rEyeU;
/*59e*/ s16 rEyeV;
/*600*/ u32 unk600;
/*604*/ u32 unk604;
/*608*/ u32 unk608;
/*60c*/ s16 lEyeU;
/*60e*/ s16 lEyeV;
} SnowHornState;

typedef struct{
/*0x10*/ ObjCreateInfo base;
/*0x18*/ s16 unkRadius;
/*0x1A*/ s16 unk1A;
/*0x1C*/ s8 rotation;
/*0x1D*/ s8 unk1D;
}
SnowHornCreateInfo;

typedef struct {
/*000*/ s8 unk0[0x62];
/*062*/ s8 unk62;
/*063*/ s8 unk63;
/*064*/ s8 unk64[0x2A];
/*08E*/ u8 unk8E[10];
/*098*/ u8 unk98[10];
} UnkStruct2;

typedef struct {
s16 soundIDA;
s16 soundIDB;
s16 unk4;
s16 unk6;
s8 unk8; //Boolean: seems to choose between using DLL17 (particles) or DLL106 (modgfx)
f32 unkC;
f32 unk10;
} SnowHornData0;

static const char _rodata_0[] = "MAM: curve setup failed\n";

static SnowHornData0 _data_0[] = {
    {SOUND_377, SOUND_FFFF, 0x002f, 0x0030, 0x00,  0.012, 0.005},
    {SOUND_377, SOUND_FFFF, 0x002f, 0x0030, 0x00,  0.012, 0.005},
    {SOUND_377, SOUND_FFFF, 0x002a, 0x0030, 0x00,  0.011, 0.005},
    {SOUND_677, SOUND_FFFF, 0xffff, 0xffff, 0x01,  0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00,  0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00,  0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00,  0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00,  0.0, 0.0},
    {SOUND_677, SOUND_FFFF, 0xffff, 0xffff, 0x01,  0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00,  0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0x002a, 0x0030, 0x00, 0.011, 0.005},
    {SOUND_377, SOUND_FFFF, 0x002a, 0x0030, 0x00, 0.011, 0.005},
    {SOUND_377, SOUND_FFFF, 0x002a, 0x0030, 0x00, 0.011, 0.005},
    {SOUND_377, SOUND_FFFF, 0x002e, 0x0030, 0x00, 0.012, 0.005},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0x002d, 0x0030, 0x00, 0.012, 0.005},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0x002c, 0x0030, 0x00, 0.015, 0.005},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0x002b, 0x0030, 0x00, 0.015, 0.005},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0},
    {SOUND_377, SOUND_FFFF, 0xffff, 0xffff, 0x00, 0.0, 0.0}
};

static f32 _data_230[] = {
    -12, 0, -20, 
    12, 0, -20, 
    12, 0, 20, 
    -12, 0, 20
};
static u32 _data_260[] = {
    0, 0, 0, 0
};
/** Boolean - decides whether all SnowHorn should go to sleep */
static u8 _data_270 = 0;
static u8 _data_274[] = {0,0,0,0, 0,0,0,0};
static s32 _data_27C[] = {
    3
};
static f32 _data_280[] = {
    0.005, 0.0
};
static s16 _data_288[] = {
    35, 35, 35, 35, 41, 0, 0, 0
};
static f32 _data_298[] = {
    0.005, 0.005, 0.005, 0.005, 0.008, 0.005, 0.005, 0.005
};
static s16 _data_2B8[] = {0};
static f32 _data_2BC[] = {0.005};
static s32 _data_2C0[] = {
    0x0, 0x1
};
static s32 _data_2C8[] = {
    0x2, 0x3
};
static s32 _data_2D0[] = {
    0x4
};
static s32 _data_2D4[] = {
    0x5
};
static s32 _data_2D8[] = {
    0x6
};
static s32 _data_2DC[] = {
    0x7
};
static s32 _data_2E0[] = {
    0x8, 0x9
};
static s32 _data_2E8[] = {
    0x9, 0xb, 0xc, 0xd, 0xe, 0xf
};
static s8 _data_300[] = {
    1,1,1,1
};

void dll_496_ctor(s32 arg0) {
}

void dll_496_dtor(s32 arg0) {
}

//https://decomp.me/scratch/tHX03
//NOTE: dll_496_func_18 itself matches, it's just that these declared functions need to also be matched in order to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_18.s")
#else
s32 dll_496_func_84C(Object* self, Object* overrideObject, AnimObjState* animObjState, s8 arg3);
void dll_496_func_D5C(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
void dll_496_func_11C4(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
void dll_496_func_174C(Object *snowHorn, SnowHornState *state, SnowHornCreateInfo* mapsObj);
void dll_496_func_1CA0(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
void dll_496_func_22E4(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);

void dll_496_func_18(Object* snowhorn, SnowHornCreateInfo* mapsObj, s32 arg2) {
    SnowHornState* state;
    u8* temp_a0;
    s32 sp34;

    state = snowhorn->state;
    sp34 = *_data_300;
    snowhorn->srt.yaw = mapsObj->rotation << 8;
    snowhorn->unk0xbc = (void*)dll_496_func_84C;
    
    if (arg2 == 0) {
        obj_add_object_type(snowhorn, 0xC);
        state->unk50 = 0.005f;
        state->unkRadius = mapsObj->unkRadius;
        state->unk6 = mapsObj->unk1A * 0x3C;
        snowhorn->unk0xaf |= 8;

        switch (mapsObj->unk1D) {
            case 0:
                dll_496_func_D5C(snowhorn, state, mapsObj);
                break;
            case 2:
                dll_496_func_11C4(snowhorn, state, mapsObj);
                break;
            case 1:
            case 3:
                dll_496_func_174C(snowhorn, state, mapsObj);
                break;
            case 4:
                dll_496_func_1CA0(snowhorn, state, mapsObj);
                break;
            case 5:
                dll_496_func_22E4(snowhorn, state, mapsObj);
                break;
        }

        //to do with setting up look-at behaviour?
        if (state->unk424 & 1) {
            temp_a0 = (u8*)state + 0x170;
            gDLL_27_HeadTurn->vtbl->head_turn_func_18((Vec3f*)temp_a0, 0x06000000, 0, 1);
            gDLL_27_HeadTurn->vtbl->head_turn_func_c0((Vec3f*)temp_a0, 4, (s32)_data_230, (s32)_data_260, (s32)&sp34);
            gDLL_27_HeadTurn->vtbl->head_turn_func_fb8(snowhorn, (Vec3f*)temp_a0);
        }
        snowhorn->ptr0x64->flags |= 0xA10;
    }
}
#endif

//https://decomp.me/scratch/eOLnX
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_24C.s")
#else
void dll_496_func_11E0(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
void dll_496_func_1980(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
void dll_496_func_1D68(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
void dll_496_func_2318(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
s32 dll_496_func_980(Object* snowhorn);
void dll_496_func_CC4(Object *snowHorn, s32 lookAt);
void dll_496_func_D80(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);

void dll_496_func_24C(Object* snowhorn) {
    f32 daytime; //don't know the type
    SnowHornState* state;
    SnowHornCreateInfo* mapsObj;
    Object* player;
    s16 animID;
    Vec3f* temp_a0;
    u16 temp_t1;
    u32 temp_t9;
    u32 seqIndex;
    s32 sp44; //don't know the type
    s32 animIndex;
    Vec3f* sp34;

    state = snowhorn->state;
    mapsObj = (SnowHornCreateInfo*)snowhorn->createInfo;
    player = get_player();

    if (vec3_distance_xz_squared(&snowhorn->positionMirror, &player->positionMirror) 
            < 2.0f * (state->unkRadius * state->unkRadius)) {
        if (!(state->unk424 & 0x80)) {
            state->unk424 |= 0x80;
        }
    } else {
        if (state->unk424 & 0x80) {
            state->unk424 &= 0xFF7F;
        }
    }
    
   if (state->unk424 & 0x40) {
        dll_496_func_CC4(snowhorn, state->unk424 & 4);
        func_800328F0(snowhorn, &state->lookAtUnk, state->walkSpeed);
    }
    func_80032A08(snowhorn, &state->lookAtUnk);

    if (func_80026DF4(snowhorn, (u32*)_data_0, 0x1C, (state->flags & 0x4000 ? 1 : 0) & 0xFF, 
                      (s8*)state + 0x54) != 0) {
        state->flags |= 0x4000;
        return;
    }
    
    state->flags &= 0xBFFF;
    state->unk427 = gDLL_29_Gplay->vtbl->get_map_setup(snowhorn->mapID);
    _data_270 = gDLL_7_Newday->vtbl->func8(&daytime); //check if night

    if (state->flags & 0x8000 && (dll_496_func_980(snowhorn) != 0)){
        return;
    }

    state->distanceFromPlayer = vec3_distance(&snowhorn->positionMirror, &player->positionMirror);
    switch (mapsObj->unk1D) {
        case 0:
            dll_496_func_D80(snowhorn, state, mapsObj);
            break;
        case 1:
        case 2:
            dll_496_func_11E0(snowhorn, state, mapsObj);
            break;
        case 3:
            dll_496_func_1980(snowhorn, state, mapsObj);
            break;
        case 4:
            dll_496_func_1D68(snowhorn, state, mapsObj);
            break;
        case 5:
            dll_496_func_2318(snowhorn, state, mapsObj);
            break;
    }
    
    if (state->unk424 & 1) {
        gDLL_27_HeadTurn->vtbl->head_turn_func_1e8(snowhorn, &state->unk170, delayFloat);
        gDLL_27_HeadTurn->vtbl->head_turn_func_5a8(snowhorn, &state->unk170);
        gDLL_27_HeadTurn->vtbl->head_turn_func_624(snowhorn, &state->unk170, delayFloat);
    }

    if (state->someAnimIDList) {
        animIndex = state->flags & 0xFFFF7FFF;
        if (snowhorn->curModAnimId != state->someAnimIDList[animIndex]) {
            func_80023D30(snowhorn, state->someAnimIDList[animIndex], 0.0f, 0);

            if (state->unk48[animIndex] >= 0.0f) {
                state->unk50 = state->unk48[animIndex];
            }
            state->unk424 &= 0xFFF7;
        }
        if (func_80024108(snowhorn, state->unk50, delayFloat, (s32)&sp44) != 0) {
            state->unk424 |= 8;
        } else {
            state->unk424 &= 0xFFF7;
        }
        func_80025780(snowhorn, delayFloat, (void*)&sp44, 0);
    }

    if ((state->chatSequenceList != 0) && (snowhorn->unk0xaf & 1)) {
        if (state->unk424 & 0x20) {
            seqIndex = rand_next(0, state->unk426 - 1);
        } else {
            seqIndex = state->unk425;
        }
        state->unk425++;
        if (state->unk425 >= state->unk426) {
            state->unk425 = 0;
        }
        gDLL_3_Animation->vtbl->func17(state->chatSequenceList[seqIndex], snowhorn, -1);
        set_button_mask(0, A_BUTTON);
    }

}
#endif

void dll_496_func_770(u32 a0){
}

s32 func_80031F6C(void*, s32, f32*, f32*, f32*, s32);

void dll_496_func_77C(Object* self, Gfx **gfx, Mtx **mtx, Vertex **vtx, Triangle **pols, s8 visibility) {
    SnowHornState* state;
    u32 addr;

    state = self->state;
    if (visibility) {
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
        func_80031F6C(self, 1, &state->playerPositionCopy.x, &state->playerPositionCopy.y, &state->playerPositionCopy.z, 0);
    }
}

void dll_496_func_804(s32 arg0, s32 arg1) {
    _data_274[0] = 0;
}

s32 dll_496_func_828(s32 arg0) {
    return 0xCB;
}

s32 dll_496_func_838(s32 arg0, s32 arg1) {
    return 0x42C;
}

/** When talking with SnowHorn */
s32 dll_496_func_84C(Object* self, Object* overrideObject, AnimObjState* animObjState, s8 arg3) {
    SnowHornState* state;
    s32 i;

    state = self->state;
    if (arg3 != 0) {
        func_80024108(self, 0.005f, delayFloat, 0);
    }
    if (state->unk424 & 1) {
        gDLL_27_HeadTurn->vtbl->head_turn_func_fb8(self, &state->unk170);
    }
    self->unk0xaf |= 8;
    animObjState->unk62 = 0;

    for (i = 0; i < animObjState->unk98; i++){
        if (animObjState->unk8E[i] == 3) {
            set_gplay_bitstring(0x5A0, 1);
            continue;
        }        
    }
    
    state->walkSpeed = 0.0f;
    return 0;
}

/** SnowHorn sleep state machine: handles anims and sounds */
s32 dll_496_func_980(Object* snowhorn) {
    UnkFunc_80024108Struct sp4c;
    SnowHornState* state;
    s32* temp1;
    s32* temp2;
    s32 animIsFinished;
    s32 playSound; //toggles between 0 and 1 (when ready to play sound another time)

    state = (SnowHornState*)snowhorn->state;
    animIsFinished = func_80024108(snowhorn, 0.006f, delayFloat, (s32)&sp4c);
    
    if (sp4c.unk1B != 0) {
        playSound = sp4c.unk13[0] == 0;
    } else {
        playSound = 0;
    }
    
    temp1 = func_800348A0(snowhorn, MODANIM_SnowHorn_Sleep, 0);
    temp2 = func_800348A0(snowhorn, MODANIM_SnowHorn_Sleep_Intro, 0);
    snowhorn->unk0xaf |= 8;    
    
    switch (snowhorn->curModAnimId) {
        case MODANIM_SnowHorn_Sleep_Intro:
            if (playSound) {
                gDLL_6_AMSFX->vtbl->play_sound(snowhorn, SFX_SNOWHORN_YAWN1, MAX_VOLUME, 0, 0, 0, 0);
            }
            if (animIsFinished) {
                func_80023D30(snowhorn, MODANIM_SnowHorn_Sleep, 0.0f, 0); //play next animation
                if (temp1 != NULL) {
                    *temp1 = 0x200;
                }
                if (temp2 != NULL) {
                    *temp2 = 0x200;
                }
                state->sleepTimer = rand_next(0, 300);
            }
            break;
        case MODANIM_SnowHorn_Sleep:
            if (playSound) {
                gDLL_6_AMSFX->vtbl->play_sound(snowhorn, SFX_SNOWHORN_SNOREHORN, MAX_VOLUME, 0, 0, 0, 0);
            }
            state->sleepTimer-= delayByte;
            if ((_data_270 == 0) && state->sleepTimer <= 0) {  //if daytime rolls around
                func_80023D30(snowhorn, MODANIM_SnowHorn_Wake_Up, 0.0f, 0); //play wake-up animation
                if (temp1 != NULL) {
                    *temp1 = 0;
                }
                if (temp2 != NULL) {
                    *temp2 = 0;
                }
            }
            break;
        case MODANIM_SnowHorn_Wake_Up:
            if (playSound) {
                gDLL_6_AMSFX->vtbl->play_sound(snowhorn, SFX_SNOWHORN_YAWN2, MAX_VOLUME, 0, 0, 0, 0);
            }
            if (animIsFinished) {
                func_80023D30(snowhorn, MODANIM_SnowHorn_Idle, 0.0f, 0); //Play idle animation
                state->flags &= 0x7FFF;
                snowhorn->unk0xaf &= 0xFFF7;
                return 0;
            }
            break;
        default:
            state->sleepTimer -= delayByte;
            if (state->sleepTimer <= 0) { //Go back to sleep if interrupted (by flinch anim etc.)
                func_80023D30(snowhorn, MODANIM_SnowHorn_Sleep_Intro, 0.0f, 0);
                state->walkSpeed = 0.0f;
            }
            break;
    }
    return 1;
}

/** Updates the SnowHorn's player position reference when nearby (for the look-at behaviour) */
void dll_496_func_CC4(Object *snowHorn, s32 lookAt){
    SnowHornState *state;
    Object *player;
      
    state = snowHorn->state;
    player = get_player();
      
    if (lookAt && player && state->distanceFromPlayer < 200.0f){
      state->lookAtUnk = 1;
      state->copyPlayerX = player->srt.transl.x;
      state->copyPlayerY = player->srt.transl.y;
      state->copyPlayerZ = player->srt.transl.z;
      return;
    }
  
    state->lookAtUnk = 0;
}

void dll_496_func_D5C(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
    state->flags = 0;
    state->unk424 |= 0x44;
    state->unkRadius = mapsObj->unkRadius;
}

/** Called by the standing SnowHorn (not by the ones that walk around) */
void dll_496_func_D80(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
    Object* player;

    if (_data_270 != 0) {
        state->sleepTimer = rand_next(0, 300);
        state->flags |= 0x8000;

        snowhorn->unk0xaf |= 8;
        snowhorn->unk0xaf &= -2;
        return;
    }
    
    state->flags &= 0x7FFF;
    if (snowhorn->curModAnimId != 0) {
        func_80023D30(snowhorn, 0, 0.0f, 0);
    }

    func_80024108(snowhorn, state->unk50, delayByte, 0);
    player = get_player();
    if (!player) 
        return;
    
    if ((f32)state->unkRadius*state->unkRadius < vec3_distance_squared(&snowhorn->positionMirror, &player->positionMirror)) {
        state->sleepTimer += delayByte;
        if (state->sleepTimer > 900) {
            gDLL_3_Animation->vtbl->func17(7, snowhorn, -1); //play seq 7?
            state->sleepTimer = (s16) -rand_next(0, 50);
        }
        return;
    } 
    
    state->sleepTimer = 0;
    snowhorn->unk0xaf &= 0xFFF7;

    switch (state->flags) {
        u32 rootsEaten;
        case 0:
            if (get_gplay_bitstring(FLAG_SnowHorn_Tutorial_Defeated_SharpClaw)) {
                state->flags = 1;
            }
            break;    
        case 1:
            rootsEaten = get_gplay_bitstring(FLAG_SnowHorn_Tutorial_NumRootsFed);
            switch (rootsEaten) {
                case 0:
                    if (get_gplay_bitstring(FLAG_SnowHorn_Tutorial_GotAlpineRoot1) || 
                        get_gplay_bitstring(FLAG_SnowHorn_Tutorial_GotAlpineRoot2))
                    {
                        state->flags = 2;
                    }
                    break;
                case 1:
                    state->flags = 4;
                    break;
                default:
                    state->flags = 6;
                    break;
            }
            break;
        case 2:
            if ((snowhorn->unk0xaf & 4) && gDLL_1_UI->vtbl->func7(INVENTORY_Alpine_Root)) {
                set_gplay_bitstring(FLAG_SnowHorn_Tutorial_NumRootsFed, 1);
                decrement_gplay_bitstring(INVENTORY_Alpine_Root);
                gDLL_3_Animation->vtbl->func17(SEQ_0159_SnowHorn_Cutscene_FeedingRoot1, snowhorn, -1);
                state->flags = 4;
                return;
            }
            break;
        case 4:
            if ((snowhorn->unk0xaf & 4) && gDLL_1_UI->vtbl->func7(INVENTORY_Alpine_Root)) {
                set_gplay_bitstring(FLAG_SnowHorn_Tutorial_NumRootsFed, 2);
                decrement_gplay_bitstring(INVENTORY_Alpine_Root);
                gDLL_3_Animation->vtbl->func17(SEQ_0248_SnowHorn_Cutscene_FeedingRoot2, snowhorn, -1);
                state->flags = 6;
                return;
            }
            break;
        case 6:
            break;
    }
    
    //Play chat sequence
    if (snowhorn->unk0xaf & 1) {
        snowhorn->unk0xaf &= 0xFFFE;
        if (state->flags < 7) {
            gDLL_3_Animation->vtbl->func17(state->flags, snowhorn, -1);
            set_button_mask(0, 0x8000);
        }
    }

}

void dll_496_func_11C4(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
    state->unk424 |= 0x44;
}

//https://decomp.me/scratch/KhEPZ
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_11E0.s")
#else
void dll_496_func_11E0(Object* self, SnowHornState* snowHornState, SnowHornCreateInfo* createInfo) {
    SnowHornState* state;
    u32 questValue;
    u16 seqBoneAngle;
    SRT srt;
    Vec3f v;
    s32 pad[3];
    Object* player;
    // Vec3f* position;
    u8 temp_t3;
    s32 var_v1;
    s32 temp_v0;
    s16* temp_v0_2;

    // object = self;
    state = self->state;
    
    temp_v0 = func_80024108(self, 0.005f, delayByte, 0);
    questValue = state->flags;

    // position = &state->playerPositionCopy;
    switch (questValue) {
        case 0:
            v.x = 0.0f;
            v.y = 10.0f;
            v.z = -25.0f;
            srt.transl.z = 0.0f;
            srt.transl.y = 0.0f;
            srt.transl.x = 0.0f;
            srt.roll = 0;
            srt.pitch = 0;
            srt.yaw = state->unkE;
            srt.scale = 0.0f;
            rotate_vec3(&srt, &v);
            srt.transl.x = v.x + state->playerPositionCopy.x;
            srt.transl.y = v.y + self->srt.transl.y;
            srt.transl.z = v.z + state->playerPositionCopy.z;
            srt.yaw = 0;
            state->flags = 1;
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
                srt.yaw = state->unkE;
                srt.scale = 0.0f;
                rotate_vec3(&srt, &v);
                srt.transl.x = v.x + state->playerPositionCopy.x;
                srt.transl.y = v.y + self->srt.transl.y;
                srt.transl.z = v.z + state->playerPositionCopy.z;
                srt.yaw = 0;
                state->flags = 2;
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
                srt.yaw = state->unkE;
                srt.scale = 0.0f;
                rotate_vec3(&srt, &v);
                self->positionMirror.x = state->unk10 + 40.0f;
                self->positionMirror.z = self->srt.transl.z;
                self->positionMirror.y = state->playerPositionCopy.x;

                srt.yaw = 0;
                self->positionMirror.x = self->positionMirror.x;                
                self->positionMirror.y = self->positionMirror.y;
                self->positionMirror.z = self->positionMirror.z;
                state->flags = 3;
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
            srt.yaw = state->unkE;
            srt.scale = 0.0f;
            rotate_vec3(&srt, &v);
            srt.transl.x = v.x + state->playerPositionCopy.x;
            srt.transl.y = v.y + self->srt.transl.y;
            srt.transl.z = v.z + state->playerPositionCopy.z;
            srt.yaw = 0;
            state->flags = 4;
            break;
        case 4:
            if (temp_v0 != 0) {
                func_80023D30(self, 0, 0.0f, 0);
                state->flags = 5;
                return;
            }
            break;
        case 5:
            questValue |= 0x8000;
            if (_data_270 != 0) {
                state->flags = questValue;
                temp_t3 = self->unk0xaf | 8;
                self->unk0xaf = temp_t3;
                self->unk0xaf = temp_t3 & 0xFFFE;
                break;
            }
            state->unk8 = (s16) (state->unk8 + delayByte);
            if (state->unk6 < state->unk8) {
                state->unk8 = 0;
                temp_v0_2 = func_80034804(self, 0);

                state->unkE = (u16) (0x8000 - *&temp_v0_2[1]);
                player = get_player();
                
                var_v1 = (arctan2_f((player->positionMirror.x + (player->speed.x * 60.0f)) - self->positionMirror.x, (player->positionMirror.z + (player->speed.z * 60.0f)) - self->positionMirror.z) - (self->srt.yaw & 0xFFFF)) + 0x8000;
                if (var_v1 >= 0x8001) {
                    var_v1 += 0xFFFF0001;
                }
                if (var_v1 < -0x8000) {
                    var_v1 += 0xFFFF;
                }
                
                func_80023D30(self, 1, 0.0f, 0);
                if ((var_v1 >= -0xBB7) && (var_v1 < 0xBB8)) {
                    state->unk38 = player->srt.transl.x;
                    state->unk3C = player->srt.transl.y;
                    state->unk40 = player->srt.transl.z;
                    state->unk2C = state->unkE;
                } else {
                    seqBoneAngle = self->srt.yaw + *&temp_v0_2[1];
                    state->unk38 = self->srt.transl.x - (fsin16_precise(seqBoneAngle) * 250.0f);
                    state->unk3C = self->srt.transl.y;
                    state->unk40 = self->srt.transl.z - (fsin16_precise(seqBoneAngle) * 250.0f);
                    state->unk2C = 0;
                }
                state->flags = 0;
            }
            break;
    }
}
#endif

//https://decomp.me/scratch/fe0rJ
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_174C.s")
#else
void dll_496_func_174C(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* createInfo){
    s32 sp2C = 0x19;
    s32 result;
    
    state->unk424 |= 0x45;
    state->someAnimIDList = (s16*)_data_27C;
    state->unk48 = _data_280;

    result = gDLL_26_Curves->vtbl->curves_func_4288(&state->unk60, snowhorn, 1000.0f, &sp2C, -1);
    if (!result){
        snowhorn->srt.transl.x = state->unk60.unk68;
        snowhorn->srt.transl.z = state->unk60.unk70;
        
        state->flags = 1;
        state->walkSpeed = 0.5f;
    } else {
        state->flags = 0;
    }

    if (createInfo->unk1D == result){
        if (get_gplay_bitstring(FLAG_DarkIce_Mines_Leap_of_Faith_Completed)){
            state->chatSequenceList = _data_2E8;
            state->unk426 = 2.0f;
        } else if (get_gplay_bitstring(FLAG_SpellStone_1_Activated)){
            state->chatSequenceList = _data_2DC;
            state->unk426 = 1.0f;
        } else if (get_gplay_bitstring(FLAG_Tricky_Distract_Learned)){
            state->chatSequenceList = _data_2D4;
            state->unk426 = 1.0f;
        } else {
            state->chatSequenceList = _data_2C8;
            state->unk426 = 2.0f;
        }
    } else {
        if (get_gplay_bitstring(FLAG_DarkIce_Mines_Leap_of_Faith_Completed)){
            state->chatSequenceList = _data_2E0;
            state->unk426 = 2.0f;
        } else if (get_gplay_bitstring(FLAG_SpellStone_1_Activated)){
            state->chatSequenceList = _data_2D8;
            state->unk426 = 1.0f;
        } else if (get_gplay_bitstring(FLAG_Tricky_Distract_Learned)){
            state->chatSequenceList = _data_2D0;
            state->unk426 = 1.0f;
        } else {
            state->chatSequenceList = _data_2C0;
            state->unk426 = 2.0f;
        }
    }
}
#endif

void dll_496_func_1980(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* arg2) {
    f32 temp_walkSpeed;
    f32 dx;
    f32 dz;
    f32 speed;
    UnkCurvesStruct60* curveStruct;

    if (_data_270) {
        temp_walkSpeed = state->walkSpeed;
        if (state->walkSpeed > 0.0f) {
            state->walkSpeed = temp_walkSpeed - 0.025f;
        } else {
            state->flags |= 0x8000;
            state->walkSpeed = 0.0f;
            state->sleepTimer = rand_next(0, 300);
            return;
        }
    } else if (snowhorn->unk0xaf & 4 || state->distanceFromPlayer < 80.0f) {
        state->walkSpeed -= 0.025f;
        if (state->walkSpeed < 0.05f) {
            state->walkSpeed = 0.0f;
        }
    } else {
        state->walkSpeed += 0.025f;
        if (state->walkSpeed > 0.5f) {
            state->walkSpeed = 0.5f;
        }
    }
    
    if (_data_274[0] != 0 && 
        func_80031BBC(snowhorn->positionMirror.x, snowhorn->positionMirror.y, snowhorn->positionMirror.z) == 0xA){
        gDLL_3_Animation->vtbl->func17(0x10, snowhorn, -1); //setAnimation?
        return;
    }
    snowhorn->unk0xaf &= 0xFFF7;

    if (state->flags != 0) {
        curveStruct = &state->unk60;
        if (state->flags != 1) {
            state->flags = 0;
            return;
        }
        if ((func_800053B0(curveStruct, state->walkSpeed) != 0) || (curveStruct->unk10 != 0)) {
            gDLL_26_Curves->vtbl->curves_func_4704(curveStruct);
        }
        dx = curveStruct->unk68 - snowhorn->srt.transl.x;
        dz = curveStruct->unk70 - snowhorn->srt.transl.z;

        //a1 for func_8002493C seems to be speed (obtained by dividing magnitude of dPos by dt)!
        speed = sqrtf((dx * dx) + (dz * dz)) * inverseDelay;
        func_8002493C(snowhorn, speed, &state->unk50);
        snowhorn->srt.yaw = arctan2_f(curveStruct->unk74, curveStruct->unk7C) + 0x8000;
        snowhorn->srt.transl.x = curveStruct->unk68;
        snowhorn->srt.transl.z = curveStruct->unk70;
        state->unk424 &= 0xFFFB;
        
        if (state->walkSpeed <= 0.0f) {
            state->flags = 0;
        }
    } else {
        state->unk424 = (u8) (state->unk424 | 4);
        if (0.1f < state->walkSpeed) {
            state->flags = 1;
        }
    }
}

/** snowHorn_handleGarundaTeFlags? */
void dll_496_func_1CA0(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
    u16 questValue;
    u16 questEnd = 6;

    questValue = (state->flags = get_gplay_bitstring(FLAG_Garunda_Te_Quest_Progress));
    
    /** If the 1st SpellStone is activated, makes sure Garunda Te's flags are in their end state */
    if (questValue < questEnd && get_gplay_bitstring(FLAG_SpellStone_1_Activated)){
        state->flags = questEnd;
        questValue = state->flags;
        set_gplay_bitstring(FLAG_Garunda_Te_Quest_Progress, questValue);
    }
    
    state->someAnimIDList = _data_288;
    state->unk48 = _data_298;
    state->garundaTe_weedsEaten = get_gplay_bitstring(FLAG_Garunda_Te_NumFrostWeedsEaten);
}

void dll_496_func_1D68(Object* self, SnowHornState* state, SnowHornCreateInfo* createInfo) {
    Object* frostWeed;
    s32 weeds;
    
    self->unk0xaf &= 0xFFF7;
    switch (state->flags) {
        case 0:
            //Calling out to the player periodically
            state->unk8 += delayByte;
            if (state->unk8 >= 0x3E9) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_1E2, MAX_VOLUME, 0, 0, 0, 0);
                gDLL_22_Subtitles->vtbl->func_368(0xA);
                state->unk8 = 0;
            }
            if (self->unk0xaf & 4) {
                state->flags = 1;
            }
            break;
        case 1:
            if (func_80032538(self)) {
                gDLL_3_Animation->vtbl->func17(0, self, -1);
                state->flags = 2;
                set_gplay_bitstring(0x115, state->flags);
            }
            break;
        case 2:
            //Eating FrostWeeds?
            if (func_80032538(self)) {
                gDLL_3_Animation->vtbl->func17(1, self, -1);
            }
            
            frostWeed = obj_get_nearest_type_to(4, self, 0);
            createInfo = (SnowHornCreateInfo*)self->createInfo;
            if (frostWeed && frostWeed->id == 0x3FB && vec3_distance_xz_squared(&self->positionMirror, &frostWeed->positionMirror) < createInfo->unkRadius * createInfo->unkRadius) {
                if (!((DLL_227_Tumbleweed*)frostWeed->dll)->vtbl->func4(frostWeed)) {
                    ((DLL_227_Tumbleweed*)(frostWeed->dll))->vtbl->func3(frostWeed, &state->playerPositionCopy);
                    state->frostWeed = frostWeed;
                    if (0){
                        state->garundaTe_weedsEaten = GARUNDA_TE_WEEDS_NEEDED;
                    }
                    state->garundaTe_weedsEaten++;
                    if (state->garundaTe_weedsEaten > GARUNDA_TE_WEEDS_NEEDED) {
                        state->garundaTe_weedsEaten = GARUNDA_TE_WEEDS_NEEDED;
                    }
                    set_gplay_bitstring(0x48B, state->garundaTe_weedsEaten);
                    state->flags = 3;
                }
            }
            break;
        case 3:
            if (vec3_distance_xz_squared(&state->playerPositionCopy, &state->frostWeed->positionMirror) < 6.25f) {
                state->flags = 4;
            }
            break;
        case 4:
            if (state->unk424 & 8) {
                weeds = state->garundaTe_weedsEaten;
                if (weeds >= GARUNDA_TE_WEEDS_NEEDED) {
                    set_gplay_bitstring(0x102, 1);
                    state->flags = 5;
                    set_gplay_bitstring(0x115, state->flags);
                    break;
                }
                if (weeds % 3 == 0) {
                    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_74B, MAX_VOLUME, 0, 0, 0, 0);
                    gDLL_22_Subtitles->vtbl->func_368(2);
                }
                state->flags = 2;
            }
            break;
        case 5:
            if (func_80032538(self)) {
                if (state->unk425 % 2) {
                    gDLL_3_Animation->vtbl->func17(3, self, -1);
                } else {
                    gDLL_3_Animation->vtbl->func17(2, self, -1);
                }
                state->unk425 += 1;
            }
            break;
        case 6:
            //SpellStone activation
            if (func_80032538(self)) {
                gDLL_3_Animation->vtbl->func17(4, self, -1);
            } else if (gDLL_1_UI->vtbl->func7(0x123)) {
                set_gplay_bitstring(0x22B, 1);
                state->flags = 7;
                set_gplay_bitstring(0x115, state->flags);
            }
            break;
        case 7:
            self->unk0xaf |= 8;
            break;
    }
    
    if (state->flags >= 2 && state->flags < 5) {
        diPrintf("noweeds=%d\n", state->garundaTe_weedsEaten);
    }
}

void dll_496_func_22E4(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
    state->someAnimIDList = _data_2B8;
    state->unk48 = _data_2BC;
}

void dll_496_func_2318(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
}