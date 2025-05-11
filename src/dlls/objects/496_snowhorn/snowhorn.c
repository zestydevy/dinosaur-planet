#include "PR/ultratypes.h"

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
static const u32 _rodata_28[] = {
    0x3ba3d70a
};
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
static const u32 _rodata_90[] = {
    0x3f266666
};
static const u32 _rodata_94[] = {
    0x3ccccccd
};
static const u32 _rodata_98[] = {
    0x3ccccccd
};
static const u32 _rodata_9C[] = {
    0x3d4ccccd
};
static const u32 _rodata_A0[] = {
    0x3ccccccd
};
static const u32 _rodata_A4[] = {
    0x3dcccccd
};
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
static u32 _data_270[] = {
    0x00000000
};
static s8 _data_274[] = {0,0,0,0, 0,0,0,0};
static u32 _data_27C[] = {
    0x00000003
};
static u32 _data_280[] = {
    0x3ba3d70a, 0x00000000
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
/*000*/ s32 unk0;
/*004*/ s16 unk4;
/*006*/ s16 unk6;
/*008*/ s32 unk8;
/*00c*/ s16 unkC; //Garunda Te quest progress?
/*00e*/ s16 unkE;
/*010*/ s32 unk10;

/*014*/ f32 positionCopyX;
/*014*/ f32 positionCopyY;
/*014*/ f32 positionCopyZ;

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
/*050*/ s8 unk50[0x390];

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

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496/dll_496_func_18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496/dll_496_func_24C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496/dll_496_func_84C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496/dll_496_func_980.s")

/** Updates the SnowHorn's player position reference (for the look-at behaviour) when nearby */
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
    arg1->unkC = 0;
    arg1->unk424 = (u8) (arg1->unk424 | 0x44);
    arg1->unk4 = (s16) arg2->unk18;
}

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496/dll_496_func_D80.s")

void dll_496_func_11C4(s32 arg0, SnowHornState* arg1, s32 arg2) {
    arg1->unk424 = (u8) (arg1->unk424 | 0x44);
}

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496/dll_496_func_11E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496/dll_496_func_174C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496/dll_496_func_1980.s")

enum GameBits {
    FLAG_Always_1 = 0x95,
    FLAG_Always_0 = 0x96,
    FLAG_Garunda_Te_Quest_Progress = 0x115,
    FLAG_Garunda_Te_NumFrostWeedsEaten = 0x48B,
    FLAG_SpellStone_1_Activated = 0x123
};

/** snowHorn_handleGarundaTeFlags? */
void dll_496_func_1CA0(s32 arg0, SnowHornState* state, SnowHornCreateInfo* arg2) {
    u16 questValue;
    u16 questEnd = 6;

    questValue = (state->unkC = get_gplay_bitstring(FLAG_Garunda_Te_Quest_Progress));
    
    /** If the 1st SpellStone is activated, makes sure Garunda Te's flags are in their end state */
    if (questValue < questEnd && get_gplay_bitstring(FLAG_SpellStone_1_Activated)){
        state->unkC = questEnd;
        questValue = state->unkC;
        set_gplay_bitstring(FLAG_Garunda_Te_Quest_Progress, questValue);
    }
    
    state->unk44 = _data_288;
    state->unk48 = _data_298;
    state->garundaTe_weedsEaten = get_gplay_bitstring(FLAG_Garunda_Te_NumFrostWeedsEaten);
}

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/496/dll_496_func_1D68.s")

void dll_496_func_22E4(s32 arg0, SnowHornState* arg1, s32 arg2) {
    arg1->unk44 = _data_2B8;
    arg1->unk48 = _data_2BC;
}

void dll_496_func_2318(s32 arg0, s32 arg1, s32 arg2) {
}
