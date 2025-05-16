//"NWmammoth.c" seems to have been Rare's name!

#include "PR/ultratypes.h"
#include "dll.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "variables.h"
#include "functions.h"
#include "types.h"

static const u32 _rodata_0[] = {
    0x4d414d3a, 0x20637572, 0x76652073, 0x65747570, 0x20666169, 0x6c65640a, 0x00000000
};
static const u32 _rodata_1C[] = {
    0x6e6f7765, 0x6564733d, 0x25640a00
};
static const f32 _rodata_28[] = {0.005};
static const u32 _rodata_jtbl_2C[] = {
    0xffffddb8, 0xffffddf8, 0xffffddd8, 0xffffddf8, 0xffffde18, 0xffffde38
};
static const u32 _rodata_jtbl_44[] = {
    0xffffe158, 0xffffe198, 0xffffe178, 0xffffe198, 0xffffe1b8, 0xffffe1d8
};
static const u32 _rodata_jtbl_5C[] = {
    0xffffec40, 0xffffec68, 0xffffece0, 0xffffee20, 0xffffed7c, 0xffffee20, 0xffffee18
};
static const u32 _rodata_jtbl_78[] = {
    0xffffef54, 0xffffefe8, 0xfffff094, 0xfffff140, 0xfffff1d0, 0xfffff200
};
static const f32 _rodata_90[] = {0.65};
static const f32 _rodata_94[] = {0.025};
static const f32 _rodata_98[] = {0.025};
static const f32 _rodata_9C[] = {0.05};
static const f32 _rodata_A0[] = {0.025};
static const f32 _rodata_A4[] = {0.1};
static const u32 _rodata_jtbl_A8[] = {
    0xfffffa94, 0xfffffb38, 0xfffffba0, 0xfffffd04, 0xfffffd44, 0xfffffe1c, 0xfffffebc, 0xffffff64, 
    0x00000000, 0x00000000
};

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

static s32 _data_2C0[] = {
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
static s32 _data_2E8[] = {
    0x00000009, 0x0000000b, 0x0000000c, 0x0000000d, 0x0000000e, 0x0000000f
};
static u32 _data_300[] = {
    0x01010101, 0x00000000, 0x00000000, 0x00000000
};


typedef struct {
/*000*/ s32 unk0;
/*004*/ s16 unk4;
/*006*/ s16 unk6;
/*008*/ s16 unk8;
/*00A*/ s16 unkA; //randomly-assigned value?
/*00c*/ union {
    u16 GarundaTeQuestProgress; //only on Garunda Te
    u16 isWalking; //only on walking SnowHorn
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
/*050*/ s32 unk50;
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
/*110*/ s32 unk170;
/*114*/ s32 unk174;
/*118*/ Vec3f unk178[8]; //position samples/deltas - maybe for walk-related calculus?

/*0E0*/ s8 unk1d8[0x208];



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
/*0x18*/ s16 unk18;
}
SnowHornCreateInfo;

void dll_496_ctor(s32 arg0) {
}


void dll_496_dtor(s32 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_24C.s")

void dll_496_func_770(u32 a0){
}

s32 func_80031F6C(void*, s32, f32*, f32*, f32*, s32);

void dll_496_func_77C(Object* snowHorn, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s8 doDraw) {
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
    FLAG_Garunda_Te_Quest_Progress = 0x115,
    FLAG_Garunda_Te_NumFrostWeedsEaten = 0x48B,
    FLAG_SnowHorn_Unknown = 0x5A0, //Maybe intended to hide SnowHorn Wastes' minimap until SnowHorn gifts it?
    FLAG_DarkIce_Mines_Leap_of_Faith_Completed = 0x22B,
    FLAG_Tricky_Distract_Learned = 0x9E,
    FLAG_SpellStone_1_Activated = 0x123
};

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_84C.s")
#else

typedef struct {
/*000*/ s8 unk0[0x62];

/*062*/ s8 unk62;
/*063*/ s8 unk63;

/*064*/ s8 unk64[0x2A];

/*08E*/ u8 unk8E[10];

/*098*/ u8 unk98[10];
} UnkStruct2;

extern DLLInst_Unknown *gDLL_27;

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
        gDLL_27->exports->func[7].withTwoArgs((s32)snowHorn, (s32)state + 0x170);
    }
    snowHorn->unk0xaf |= 8;
    temp = arg2->unk98;
    arg2->unk62 = 0;

    for (temp = 0; arg2->unk98[temp] > 0; temp++){
        if (arg2->unk8E[temp] == 3){
            set_gplay_bitstring(FLAG_SnowHorn_Unknown, 1);
        }   
    }
    
    state->unk58 = 0.0f;
    return 0;
}

#endif

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_980.s")

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

//arg2 type is just a guess!
void dll_496_func_D5C(s32 arg0, SnowHornState* arg1, SnowHornCreateInfo* arg2) {
    arg1->isWalking = 0;
    arg1->unk424 |= 0x44;
    arg1->unk4 = (s16) arg2->unk18;
}

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_D80.s")

void dll_496_func_11C4(s32 arg0, SnowHornState* arg1, s32 arg2) {
    arg1->unk424 |= 0x44;
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

extern DLLInst_26_curves *gDLL_CURVES;

/** Need to figure out the struct for arg2! */
void dll_496_func_174C(Object *snowHorn, SnowHornState *state, UnknownSnowHornStruct *arg2){
    s32 sp2C;
    
    sp2C = 0x19;
    
    state->unk424 |= 0x45;
    state->unk44 = _data_27C;
    state->unk48 = _data_280;

    if (gDLL_CURVES->exports->curves_func_4288(&state->unk60, snowHorn, 1000.0f, &sp2C, -1) == 0){
        snowHorn->srt.transl.x = state->unk60.unk68;
        snowHorn->srt.transl.z = state->unk60.unk70;
        
        state->isWalking = 1;
        state->walkSpeed = 0.5f;
    } else {
        state->isWalking = 0;
    }
    
    if (arg2->unk1D == 1){ //Regalloc on the 1, argh!
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

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_1980.s")
#else

s32 func_800053B0(void*, f32);
s32 func_8002493C(void*, f32, void*);
s32 func_80031BBC(f32, f32, f32);
s16 rand_next(s32, s32); 
extern u8 __data0;
extern f32 __rodata0;
extern DLLInst_Unknown* gDLL_ANIM;
extern DLLInst_26_curves* gDLL_CURVES;

void dll_496_func_1980(Object* snowhorn, SnowHornState* state, s32 arg2) {
    f32 temp_walkSpeed;
    f32 dx;
    f32 dz;
    f32 speed;
    UnkCurvesStruct60* curveStruct;

    if (_data_270 != 0) {
        temp_walkSpeed = state->walkSpeed;
        if (state->walkSpeed > 0.0f) {
            state->walkSpeed = temp_walkSpeed - _rodata_94[0];
        } else {
            state->isWalking |= 0x8000;
            state->walkSpeed = 0.0f;
            state->unkA = rand_next(0, 300);
            return;
        }
    } else if (snowhorn->unk0xaf & 4 || state->distanceFromPlayer < 80.0f) {
        state->walkSpeed -= _rodata_98[0];
        if (state->walkSpeed < _rodata_9C[0]) {
            state->walkSpeed = 0.0f;
        }
    } else {
        state->walkSpeed += _rodata_A0[0];
        if (state->walkSpeed > 0.5f) {
            state->walkSpeed = 0.5f;
        }
    }
    
    if (_data_274[0] != 0 && 
        func_80031BBC(snowhorn->positionMirror.x, snowhorn->positionMirror.y, snowhorn->positionMirror.z) == 0xA){
        gDLL_ANIM->exports->func[17].withThreeArgs(0x10, (s32)snowhorn, -1); //setAnimation?
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
            gDLL_CURVES->exports->curves_func_4704(curveStruct);
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
        if (_rodata_A4[0] < state->walkSpeed) {
            state->isWalking = 1;
        }
    }
}
#endif


/** snowHorn_handleGarundaTeFlags? */
void dll_496_func_1CA0(s32 arg0, SnowHornState* state, SnowHornCreateInfo* arg2) {
    u16 questValue;
    u16 questEnd = 6;

    questValue = (state->GarundaTeQuestProgress = get_gplay_bitstring(FLAG_Garunda_Te_Quest_Progress));
    
    /** If the 1st SpellStone is activated, makes sure Garunda Te's flags are in their end state */
    if (questValue < questEnd && get_gplay_bitstring(FLAG_SpellStone_1_Activated)){
        state->GarundaTeQuestProgress = questEnd;
        questValue = state->GarundaTeQuestProgress;
        set_gplay_bitstring(FLAG_Garunda_Te_Quest_Progress, questValue);
    }
    
    state->unk44 = _data_288;
    state->unk48 = _data_298;
    state->garundaTe_weedsEaten = get_gplay_bitstring(FLAG_Garunda_Te_NumFrostWeedsEaten);
}

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496_snowhorn/dll_496_func_1D68.s")

void dll_496_func_22E4(s32 arg0, SnowHornState* arg1, s32 arg2) {
    arg1->unk44 = _data_2B8;
    arg1->unk48 = _data_2BC;
}

void dll_496_func_2318(s32 arg0, s32 arg1, s32 arg2) {
}
