#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/rand.h"
#include "types.h"

typedef struct {
/*000*/ s8 unk0[0x33A - 0];
/*33A*/ s8 unk33A;
/*33B*/ s8 unk33B[0x348 - 0x33B];
/*348*/ s8 unk348; //health?
/*349*/ s8 unk349[0x3B6 - 0x349];
/*3B6*/ s16 unk3B6;
/*3B6*/ s8 unk3B8[0x3FC - 0x3B8];
} SabreBaddieState;

/*0x0*/ static u32 _data_0[] = {
    0x00990000, 0x0000000b, 0x01ff0000, 0x3c9374bc, 0xbf800000, 0xbf800000, 0xbf800000, 0x3e4ccccd, 
    0x3f000000, 0x3e99999a, 0x3eb33333, 0x06610000, 0x00000000, 0x00000000, 0x00000000, 0x009a0000, 
    0x0000000c, 0x01020000, 0x3c54fdf4, 0x3dcccccd, 0x3ecccccd, 0x3f000000, 0x3e9eb852, 0x3efae148, 
    0x3e800000, 0x3e99999a, 0x06620000, 0x00000000, 0x00000000, 0x00000000, 0x009b0000, 0x0000000a, 
    0x01ff0000, 0x3c449ba6, 0xbf800000, 0xbf800000, 0xbf800000, 0x3e99999a, 0x3f0a3d71, 0x3e851eb8, 
    0x3eb851ec, 0x06630000, 0x00000000, 0x00000000, 0x00000000, 0x009c0000, 0x0000000b, 0x01ff0000, 
    0x3c449ba6, 0x00000000, 0x3eb33333, 0x3f1eb852, 0x3ea3d70a, 0x3f19999a, 0x3e851eb8, 0x3ed70a3d, 
    0x06610000, 0x00000000, 0x00000000, 0x00000000, 0x009d0000, 0x00000009, 0x01ff0000, 0x3c5d2f1b, 
    0x00000000, 0x3ecccccd, 0x3f0ccccd, 0x3dcccccd, 0x3f07ae14, 0x3e75c28f, 0x3eb33333, 0x06610000, 
    0x00000000, 0x00000000, 0x00000000, 0x009e0000, 0x0000000c, 0x01ff0000, 0x3c343958, 0xbf800000, 
    0xbf800000, 0xbf800000, 0x3e851eb8, 0x3ebd70a4, 0x3eb33333, 0x3eeb851f, 0x06620000, 0x00000000, 
    0x00000000, 0x00000000
};
/*0x168*/ static u32 _data_168[] = {
    0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 
    0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 
    0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000c, 
    0x0000000c, 0x0000000c
};
/*0x1D0*/ static u32 _data_1D0[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffff0000
};
/*0x1EC*/ static u16 _data_1EC[] = {
    0x0089, 0x008c, 0x0090
};
/*0x1F4*/ static f32 _data_1F4[] = {
    0, 0.05, 0.03, 0.7, 0.68, 1.05
};
/*0x20C*/ static u16 _data_20C[] = {
    0x067a, 0x067b, 0x067c
};

/*0x0*/ static u8 _bss_0[0x38];
/*0x38*/ static u8 _bss_38[0x8];
/*0x40*/ static u8 _bss_40[0x20];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_0.s")

// offset: 0x12C | ctor
void dll_225_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_ctor.s")

// offset: 0x16C | dtor
void dll_225_dtor(void *dll) { }

// offset: 0x178 | func: 1 | export: 0
void dll_225_create(Object *self, ObjSetup* setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_create.s")

// offset: 0x2A8 | func: 2 | export: 1
void dll_225_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_update.s")

// offset: 0x450 | func: 3 | export: 2
void dll_225_func_450(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_450.s")

// offset: 0x4A0 | func: 4 | export: 3
void dll_225_draw(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_draw.s")

// offset: 0x500 | func: 5 | export: 4
void dll_225_destroy(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_destroy.s")

// offset: 0x5A4 | func: 6 | export: 5
u32 dll_225_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8 | MODFLAGS_EVENTS | MODFLAGS_100;
}

// offset: 0x5B4 | func: 7 | export: 6
u32 dll_225_get_state_size(Object *self, u32 a1) {
    return sizeof(SabreBaddieState);
}

// offset: 0x5C8 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_5C8.s")

// offset: 0x864 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_864.s")

// offset: 0xA84 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_A84.s")

// offset: 0xC10 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_C10.s")

// offset: 0xD08 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_D08.s")

// offset: 0xDB4 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_DB4.s")

// offset: 0xF38 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_F38.s")

// offset: 0x10EC | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_10EC.s")

// offset: 0x11A4 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_11A4.s")

// offset: 0x1264 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1264.s")

// offset: 0x134C | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_134C.s")

// offset: 0x1434 | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1434.s")

// offset: 0x151C | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_151C.s")

// offset: 0x1604 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1604.s")

// offset: 0x1878 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1878.s")

// offset: 0x1AEC | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1AEC.s")

// offset: 0x1D60 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1D60.s")

// offset: 0x1E60 | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1E60.s")

// offset: 0x1F20 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_1F20.s")

// offset: 0x1F38 | func: 27
s32 dll_225_func_1F38(Object* self, SabreBaddieState* state, s32 arg2) {
    SabreBaddieState* state2;

    state2 = self->state;

    if (state->unk348 <= 0) {
        return 5;
    }

    if (state->unk348 < 5) {
        main_set_bits(0x391, 1);
        state->unk348 = 1;
        return 3;
    }

    if (state->unk33A) {
        if (state->unk348 < rand_next(2, 4)) {
            return 4;
        }
        state2->unk3B6 = 300;
        return 8;
    }
    return 0;
}

// offset: 0x2010 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_2010.s")

// offset: 0x2028 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_2028.s")

// offset: 0x209C | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_209C.s")

// offset: 0x2190 | func: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_2190.s")

// offset: 0x21F8 | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_21F8.s")

// offset: 0x2404 | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/225_SabreBaddie/dll_225_func_2404.s")
