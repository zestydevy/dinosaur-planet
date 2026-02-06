#include "common.h"

typedef struct {
/*00*/ u8 _unk0;
} DLL730_Data;

/*0x0*/ static u32 _data_0[] = {
    0x049a049a, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_730_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_730_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_730_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/730/CRSpellStone/dll_730_setup.s")

// offset: 0x80 | func: 1 | export: 1
void dll_730_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/730/CRSpellStone/dll_730_control.s")

// offset: 0x218 | func: 2 | export: 2
void dll_730_update(Object *self) { }

// offset: 0x224 | func: 3 | export: 3
void dll_730_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/730/CRSpellStone/dll_730_print.s")

// offset: 0x288 | func: 4 | export: 4
void dll_730_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/730/CRSpellStone/dll_730_free.s")

// offset: 0x2C8 | func: 5 | export: 5
u32 dll_730_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2D8 | func: 6 | export: 6
u32 dll_730_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL730_Data);
}

// offset: 0x2EC | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/730/CRSpellStone/dll_730_func_2EC.s")

// offset: 0x334 | func: 8 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/730/CRSpellStone/dll_730_func_334.s")

// offset: 0x34C | func: 9
s32 dll_730_func_34C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return 0;
}

/*0x0*/ static const char str_0[] = " DO Win BIT ";
/*0x10*/ static const char str_10[] = "\n\n Hit By ONVEHICLE HIT \n\n";
/*0x2C*/ static const char str_2C[] = "";
