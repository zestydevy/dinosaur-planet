#include "common.h"

typedef struct {
/*0x00*/ u8 _unk0;
}DLL730_Data;


typedef struct {
/*0x00*/ s16 objId;
/*0x02*/ u8 quarterSize; // Bits 0-7: Exclude from map setups 1-8
/*0x03*/ u8 setupExclusions1;
/*0x04*/ u8 loadFlags;
/*0x05*/ union {
u8 byte5; // Bits 7-4 (note the reversal): Exclude from map setups 9-12
u8 setupExclusions2; // Bits 0-3: Fade flags
u8 fadeFlags; };
/*0x06*/ union {
u8 byte6; // If loadFlags & 0x10 IS set, the map object group this object is a part of.
u8 mapObjGroup; // If loadFlags & 0x10 is NOT set, maximum distance object is loaded at (divided by 8).
u8 loadDistance; }; // Max distance object is visible at (divided by 8).
/*0x07*/ u8 fadeDistance;
/*0x08*/ f32 x;
/*0x0c*/ f32 y;
/*0x10*/ f32 z;
/*0x14*/ s32 uID;
} DLL730_Setup;

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
