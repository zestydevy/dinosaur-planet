//"NWmammoth.c" seems to have been Rare's name!

#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/1_ui.h"
#include "dlls/engine/27_head_turn.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/rand.h"
#include "variables.h"
#include "functions.h"
#include "types.h"

/*0x0*/ static const char str_0[] = "MAM: curve setup failed\n";

static u32 _data_0[] = {
    0x0377ffff, 0x002f0030, 0x00000000, 0x3c449ba6, 0x3ba3d70a, 0x0377ffff, 0x002f0030, 0x00000000, 
    0x3c449ba6, 0x3ba3d70a, 0x0377ffff, 0x002a0030, 0x00000000, 0x3c343958, 0x3ba3d70a, 0x0677ffff, 
    0xffffffff, 0x01000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 
    0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 
    0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0677ffff, 0xffffffff, 0x01000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 
    0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 
    0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 
    0x00000000, 0x0377ffff, 0x002a0030, 0x00000000, 0x3c343958, 0x3ba3d70a, 0x0377ffff, 0x002a0030, 
    0x00000000, 0x3c343958, 0x3ba3d70a, 0x0377ffff, 0x002a0030, 0x00000000, 0x3c343958, 0x3ba3d70a, 
    0x0377ffff, 0x002e0030, 0x00000000, 0x3c449ba6, 0x3ba3d70a, 0x0377ffff, 0xffffffff, 0x00000000, 
    0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 
    0x002d0030, 0x00000000, 0x3c449ba6, 0x3ba3d70a, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 
    0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0x002c0030, 
    0x00000000, 0x3c75c28f, 0x3ba3d70a, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0x002b0030, 0x00000000, 
    0x3c75c28f, 0x3ba3d70a, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 
    0xffffffff, 0x00000000, 0x00000000, 0x00000000
};
static u32 _data_230[] = {
    0xc1400000, 0x00000000, 0xc1a00000, 0x41400000, 0x00000000, 0xc1a00000, 0x41400000, 0x00000000, 
    0x41a00000, 0xc1400000, 0x00000000, 0x41a00000
};
static u32 _data_260[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/** Boolean - decides whether all SnowHorn should go to sleep */
static u8 _data_270 = 0;
static u8 _data_274[] = {0,0,0,0, 0,0,0,0};
static u32 _data_27C[] = {
    0x00000003
};
static f32 _data_280[] = {
    0.005, 0.0
};
static u32 _data_288[] = {
    0x00230023, 0x00230023, 0x00290000, 0x00000000
};
static f32 _data_298[] = {
    0.005, 0.005, 0.005, 0.005, 0.008, 0.005, 0.005, 0.005
};
static u32 _data_2B8[] = {0};
static f32 _data_2BC[] = {0.005};
static u32 _data_2C0[] = {
    0x00000000, 0x00000001
};
static u32 _data_2C8[] = {
    0x00000002, 0x00000003
};
static u32 _data_2D0[] = {
    0x00000004
};
static u32 _data_2D4[] = {
    0x00000005
};
static u32 _data_2D8[] = {
    0x00000006
};
static u32 _data_2DC[] = {
    0x00000007
};
static u32 _data_2E0[] = {
    0x00000008, 0x00000009
};
static u32 _data_2E8[] = {
    0x00000009, 0x0000000b, 0x0000000c, 0x0000000d, 0x0000000e, 0x0000000f
};
static u32 _data_300[] = {
    0x01010101, 0x00000000, 0x00000000, 0x00000000
};

typedef struct {
/*000*/ s32 *unk0;
/*004*/ s16 unkRadius;
/*006*/ s16 unk6;
/*008*/ s16 unk8;
/*00A*/ s16 sleepTimer; //randomly-assigned value?
/*00c*/ union {
    u16 quest; //on tutorial SnowHorn
    u16 isWalking; //only on walking SnowHorn
    u16 GarundaTeQuest; //only on Garunda Te
};
/*00e*/ s16 unkE;
/*010*/ s32 unk10;

/*014*/ f32 positionCopyX; //player position
/*018*/ f32 positionCopyY; //player position
/*01C*/ f32 positionCopyZ; //player position

/*020*/ f32 distanceFromPlayer;
/*024*/ s8 unk24;
/*025*/ s8 unk25;
/*026*/ s8 unk26;
/*027*/ s8 unk27;
/*028*/ s32 unk28;
/*02c*/ s32 unk2C;
/*030*/ s32 unk30;
/*034*/ s32 unk34;
/*038*/ s32 unk38;
/*03c*/ s32 unk3C;
/*040*/ s32 unk40;
/*044*/ u32* unk44;
/*048*/ f32* unk48;
/*04c*/ s32 unk4C;
/*050*/ f32 unk50;
/*054*/ s32 unk54;
/*058*/ f32 walkSpeed; //has something to do with the struct at 0x60?
/*05C*/ s32 unk5C;

/*060*/ UnkCurvesStruct60 unk60; //SEEMS TO BE THE START OF SOME NESTED STRUCT HERE! (Curves-related?)

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

/*400*/ s8 lookAtUnk; //something to do with the look-at behaviour!
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

/*59c*/ s16 unk59C; //eyeR_u
/*59e*/ s16 unk59e; //eyeR_v
/*600*/ u32 unk600;
/*604*/ u32 unk604;
/*608*/ u32 unk608;
/*60c*/ s16 unk60C;  //eyeL_u
/*60e*/ s16 unk60e;  //eyeL_v
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

void dll_496_ctor(s32 arg0) {
}

void dll_496_dtor(s32 arg0) {
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_18.s")
#else
//NOTE: dll_496_func_18 itself matches, it's just that these declared functions need to also be matched in order to be static
s32 dll_496_func_84C(Object* snowHorn, s32 arg1, UnkStruct2* arg2, s8 arg3);
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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_24C.s")
#else

void func_80025780(Object*, f32, s32*, void*);
s32 func_80026DF4(Object*, u32*, u32, s32, void*);
void func_800328F0(Object*, void*, f32);
void func_80032A08(Object*, void*);
    
s16 rand_next(s32, s32);
void set_button_mask(int port, u16 mask);

void dll_496_func_11E0(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
void dll_496_func_1980(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
void dll_496_func_1D68(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
void dll_496_func_2318(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);
s32 dll_496_func_980(Object* snowhorn);
void dll_496_func_CC4(Object *snowHorn, s32 lookAt);
void dll_496_func_D80(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj);

void dll_496_func_24C(Object* snowhorn) {
    void* sp68; //don't know the type
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
    u16 var_v0;
    Vec3f* sp34;

    state = snowhorn->state;
    mapsObj = (SnowHornCreateInfo*)snowhorn->createInfo;
    player = get_player();   
    
    if (vec3_distance_xz_squared(&snowhorn->positionMirror, &player->positionMirror) < (2.0f * (f32) (state->unkRadius * state->unkRadius))) {
        var_v0 = state->unk424;
        temp_t9 = var_v0 | 0x80;
        if (!(var_v0 & 0x80)) {
            state->unk424 = temp_t9;
            var_v0 = temp_t9;
        }
    } else {
        var_v0 = state->unk424;
        temp_t1 = var_v0 & -129;
        if (var_v0 & 0x80) {
            state->unk424 = temp_t1;
            var_v0 = temp_t1 & 0xFF;
        }
    }
    
    if (var_v0 & 0x40) {
        dll_496_func_CC4(snowhorn, var_v0 & 4);
        func_800328F0(snowhorn, &state->lookAtUnk, state->walkSpeed);
    }
    func_80032A08(snowhorn, &state->lookAtUnk);
    if (state->isWalking & 0x4000) {
        var_v0 = 1;
    } else {
        var_v0 = 0;
    }
    if (func_80026DF4(snowhorn, _data_0, 0x1C, var_v0 & 0xFF, (s8*)state + 0x54) != 0) {
        state->isWalking = (u16) (state->isWalking | 0x4000);
        return;
    }
    state->isWalking &= 0xBFFF;
    state->unk427 = gDLL_29_Gplay->vtbl->func_143C(snowhorn->mapID);
    _data_270 = gDLL_7_Newday->vtbl->func8((s32)&sp68); //check if night?

    var_v0 = state->isWalking & 0x8000;
    if (var_v0 && (dll_496_func_980(snowhorn) != 0)){
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
        sp34 = state->unk170;

        gDLL_27->vtbl->head_turn_func_1e8(snowhorn, sp34, delayFloat);
        gDLL_27->vtbl->head_turn_func_5a8(snowhorn, sp34);
        gDLL_27->vtbl->head_turn_func_624(snowhorn, sp34, delayFloat);
    }

    if (state->someAnimIDList) {
        animIndex = state->isWalking & 0xFFFF7FFF;
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
        func_80025780(snowhorn, delayFloat, &sp44, 0);
    }
    if ((state->unk0 != 0) && (snowhorn->unk0xaf & 1)) {
        if (state->unk424 & 0x20) {
            seqIndex = rand_next(0, state->unk426 - 1);
        } else {
            seqIndex = state->unk425;
        }
        state->unk425++;
        if (state->unk425 >= state->unk426) {
            state->unk425 = 0;
        }
        gDLL_3_Animation->vtbl->func17(state->unk0[seqIndex], snowhorn, -1);
        set_button_mask(0, 0x8000);
    }

}
#endif

void dll_496_func_770(u32 a0){
}

s32 func_80031F6C(void*, s32, f32*, f32*, f32*, s32);

void dll_496_func_77C(Object* snowHorn, Gfx **arg1, Mtx **arg2, Vertex **arg3, Triangle **arg4, s8 doDraw) {
    SnowHornState* state;
    u32 addr;

    state = snowHorn->state;
    if (doDraw) {
        draw_object(snowHorn, arg1, arg2, arg3, arg4, 1.0f);
        func_80031F6C(snowHorn, 1, &state->positionCopyX, &state->positionCopyY, &state->positionCopyZ, 0);
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

enum GameBits {
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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_84C.s")
#else
s32 dll_496_func_84C(Object* snowHorn, s32 arg1, UnkStruct2* arg2, s8 arg3) {
    SnowHornState* state;
    UnkStruct2* var_s0;
    void* var_v0;
    s8 temp;

    state = snowHorn->state;
    //blink behaviour?
    if (arg3 != 0) {
        func_80024108(snowHorn, 0.005f, delayFloat, 0);
    }
    if (state->unk424 & 1) {
        //lookAt behaviour! This DLL must have reusable functions for it throughout the objects
        gDLL_27_HeadTurn->vtbl->head_turn_func_fb8(snowHorn, state->unk170);
    }
    snowHorn->unk0xaf |= 8;
    temp = arg2->unk98;
    arg2->unk62 = 0;

    for (temp = 0; arg2->unk98[temp] > 0; temp++){
        if (arg2->unk8E[temp] == 3){
            set_gplay_bitstring(FLAG_SnowHorn_Unknown, 1);
        }   
    }
    
    state->walkSpeed = 0.0f;
    return 0;
}
#endif

typedef struct {
  f32 unk0[3];
  s16 unkc[3];
  u8 unk12;
  s8 unk13[8];
  s8 unk1B; // current length of unk13
} UnkFunc_80024108Struct;

s32* func_800348A0(Object*, s32, s32);

enum SoundIDs {
    SFX_SNOWHORN_YAWN1 = 0x129,
    SFX_SNOWHORN_SNOREHORN = 0x12A,
    SFX_SNOWHORN_YAWN2 = 0x12B
};

enum SnowHornAnims {
    MODANIM_SnowHorn_Idle = 0,
    MODANIM_SnowHorn_Talk = 2,
    MODANIM_SnowHorn_Walk = 3,
    MODANIM_SnowHorn_Sleep_Intro = 4,
    MODANIM_SnowHorn_Sleep = 5,
    MODANIM_SnowHorn_Wake_Up = 6,
    MODANIM_SnowHorn_Hit_React = 47
};

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
        //new_var = temp1;
        case MODANIM_SnowHorn_Sleep_Intro:
            if (playSound) {
                gDLL_6_AMSFX->vtbl->func2((void*)snowhorn, SFX_SNOWHORN_YAWN1, 0x7F, 0, 0, 0, 0);
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
                gDLL_6_AMSFX->vtbl->func2((void*)snowhorn, SFX_SNOWHORN_SNOREHORN, 0x7F, 0, 0, 0, 0);
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
                gDLL_6_AMSFX->vtbl->func2((void*)snowhorn, SFX_SNOWHORN_YAWN2, 0x7F, 0, 0, 0, 0);
            }
            if (animIsFinished) {
                func_80023D30(snowhorn, MODANIM_SnowHorn_Idle, 0.0f, 0); //Play idle animation
                state->isWalking &= 0x7FFF;
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
    state->isWalking = 0;
    state->unk424 |= 0x44;
    state->unkRadius = mapsObj->unkRadius;
}

void set_button_mask(int port, u16 mask);

enum SnowHornTutorialSequences {
    SEQ_0157_SnowHorn_Chat_BeforeDefeatingSharpClaw = 0, //Hmph, shouldn't you help your friend?
    SEQ_0157_SnowHorn_Chat_BeforeDiggingUpRoot = 1, //I'm too hungry to talk to strangers!
    SEQ_0158_SnowHorn_Chat_AfterDiggingUpRoot = 2, //If you press the C-right button you can feed me that root!
    SEQ_0159_SnowHorn_Cutscene_FeedingRoot1 = 3, //One root is never enough, I'm starving! To get out of the clearing...
    SEQ_0625_SnowHorn_Chat_AfterEatingRoot1 = 4, //One root is never enough, I'm starving!
    SEQ_0248_SnowHorn_Cutscene_FeedingRoot2 = 5, //Ahh, that hit the spot! Tell you what, young 'un, I'll help you out!
    SEQ_0626_SnowHorn_Chat_AfterEatingRoot2 = 6  //I'm sorry lad, you're on your own now.
};

/** Called by the standing SnowHorn (not by the ones that walk around) */
void dll_496_func_D80(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
    Object* player;

    if (_data_270 != 0) {
        state->sleepTimer = rand_next(0, 300);
        state->quest |= 0x8000;

        snowhorn->unk0xaf |= 8;
        snowhorn->unk0xaf &= -2;
        return;
    }
    
    state->quest &= 0x7FFF;
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

    switch (state->quest) {
        u32 rootsEaten;
        case 0:
            if (get_gplay_bitstring(FLAG_SnowHorn_Tutorial_Defeated_SharpClaw)) {
                state->quest = 1;
            }
            break;    
        case 1:
            rootsEaten = get_gplay_bitstring(FLAG_SnowHorn_Tutorial_NumRootsFed);
            switch (rootsEaten) {
                case 0:
                    if (get_gplay_bitstring(FLAG_SnowHorn_Tutorial_GotAlpineRoot1) || 
                        get_gplay_bitstring(FLAG_SnowHorn_Tutorial_GotAlpineRoot2))
                    {
                        state->quest = 2;
                    }
                    break;
                case 1:
                    state->quest = 4;
                    break;
                default:
                    state->quest = 6;
                    break;
            }
            break;
        case 2:
            if ((snowhorn->unk0xaf & 4) && gDLL_1_UI->vtbl->ui_func_df4(INVENTORY_Alpine_Root)) {
                set_gplay_bitstring(FLAG_SnowHorn_Tutorial_NumRootsFed, 1);
                decrement_gplay_bitstring(INVENTORY_Alpine_Root);
                gDLL_3_Animation->vtbl->func17(SEQ_0159_SnowHorn_Cutscene_FeedingRoot1, snowhorn, -1);
                state->quest = 4;
                return;
            }
            break;
        case 4:
            if ((snowhorn->unk0xaf & 4) && gDLL_1_UI->vtbl->ui_func_df4(INVENTORY_Alpine_Root)) {
                set_gplay_bitstring(FLAG_SnowHorn_Tutorial_NumRootsFed, 2);
                decrement_gplay_bitstring(INVENTORY_Alpine_Root);
                gDLL_3_Animation->vtbl->func17(SEQ_0248_SnowHorn_Cutscene_FeedingRoot2, snowhorn, -1);
                state->quest = 6;
                return;
            }
            break;
        case 6:
            break;
    }
    
    //Play chat sequence
    if (snowhorn->unk0xaf & 1) {
        snowhorn->unk0xaf &= 0xFFFE;
        if (state->quest < 7) {
            gDLL_3_Animation->vtbl->func17(state->quest, snowhorn, -1);
            set_button_mask(0, 0x8000);
        }
    }

}

void dll_496_func_11C4(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
    state->unk424 |= 0x44;
}

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_11E0.s")

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_174C.s")
#else
typedef struct {
/*0x00*/ s32 unk0;
/*0x04*/ s32 unk4;
/*0x08*/ s32 unk8;
/*0x0C*/ s32 unkC;
/*0x10*/ s32 unk10;
/*0x14*/ s32 unk14;
/*0x18*/ s32 unk18;
/*0x1C*/ s8 unk1C;
/*0x1D*/ s8 unk1D;
} UnknownSnowHornStruct; //Possibly to do with talking to them, or controller inputs? TO-DO: figure out what this is!

/** Need to figure out the struct for arg2! */
void dll_496_func_174C(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj){
    s32 sp2C;
    
    sp2C = 0x19;
    
    state->unk424 |= 0x45;
    state->unk44 = _data_27C;
    state->unk48 = _data_280;

    if (gDLL_26_Curves->vtbl->curves_func_4288(&state->unk60, snowhorn, 1000.0f, &sp2C, -1) == 0){
        snowhorn->srt.transl.x = state->unk60.unk68;
        snowhorn->srt.transl.z = state->unk60.unk70;
        
        state->isWalking = 1;
        state->walkSpeed = 0.5f;
    } else {
        state->isWalking = 0;
    }
    
    if (mapsObj->unk1D == 1){ //Regalloc on the 1, argh!
        if (get_gplay_bitstring(FLAG_DarkIce_Mines_Leap_of_Faith_Completed)){
            state->unk4C = (s32)_data_2E8;
            state->unk426 = 2.0f;
        } else if (get_gplay_bitstring(FLAG_SpellStone_1_Activated)){
            state->unk4C = (s32)_data_2DC;
            state->unk426 = 1.0f;
        } else if (get_gplay_bitstring(FLAG_Tricky_Distract_Learned)){
            state->unk4C = (s32)_data_2D4;
            state->unk426 = 1.0f;
        } else {
            state->unk4C = (s32)_data_2C8;
            state->unk426 = 2.0f;
        }
    } else {
        if (get_gplay_bitstring(FLAG_DarkIce_Mines_Leap_of_Faith_Completed)){
            state->unk4C = (s32)_data_2E0;
            state->unk426 = 2.0f;
        } else if (get_gplay_bitstring(FLAG_SpellStone_1_Activated)){
            state->unk4C = (s32)_data_2D8;
            state->unk426 = 1.0f;
        } else if (get_gplay_bitstring(FLAG_Tricky_Distract_Learned)){
            state->unk4C = (s32)_data_2D0;
            state->unk426 = 1.0f;
        } else {
            state->unk4C = (s32)_data_2C0;
            state->unk426 = 2.0f;
        }
    }
}
#endif

s32 func_800053B0(void*, f32);
s32 func_8002493C(void*, f32, void*);
s32 func_80031BBC(f32, f32, f32);

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
            state->isWalking |= 0x8000;
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

    if (state->isWalking != 0) {
        curveStruct = &state->unk60;
        if (state->isWalking != 1) {
            state->isWalking = 0;
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
            state->isWalking = 0;
        }
    } else {
        state->unk424 = (u8) (state->unk424 | 4);
        if (0.1f < state->walkSpeed) {
            state->isWalking = 1;
        }
    }
}

/** snowHorn_handleGarundaTeFlags? */
void dll_496_func_1CA0(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
    u16 questValue;
    u16 questEnd = 6;

    questValue = (state->GarundaTeQuest = get_gplay_bitstring(FLAG_Garunda_Te_Quest_Progress));
    
    /** If the 1st SpellStone is activated, makes sure Garunda Te's flags are in their end state */
    if (questValue < questEnd && get_gplay_bitstring(FLAG_SpellStone_1_Activated)){
        state->GarundaTeQuest = questEnd;
        questValue = state->GarundaTeQuest;
        set_gplay_bitstring(FLAG_Garunda_Te_Quest_Progress, questValue);
    }
    
    state->unk44 = _data_288;
    state->unk48 = _data_298;
    state->garundaTe_weedsEaten = get_gplay_bitstring(FLAG_Garunda_Te_NumFrostWeedsEaten);
}

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_1D68.s")

void dll_496_func_22E4(Object *snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
    state->unk44 = _data_2B8;
    state->unk48 = _data_2BC;
}

void dll_496_func_2318(Object* snowhorn, SnowHornState* state, SnowHornCreateInfo* mapsObj) {
}