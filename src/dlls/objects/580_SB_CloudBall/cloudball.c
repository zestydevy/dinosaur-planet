#include "common.h"

// offset: 0x0 | ctor
void dll_580_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_580_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_580_obj_Setup(Object* self, ObjSetup* setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/580_SB_CloudBall/dll_580_obj_Setup.s")

// offset: 0x40 | func: 1 | export: 1
void dll_580_obj_Control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/580_SB_CloudBall/dll_580_obj_Control.s")

// offset: 0x4C0 | func: 2 | export: 2
void dll_580_obj_Update(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/580_SB_CloudBall/dll_580_obj_Update.s")

// offset: 0x644 | func: 3 | export: 3
void dll_580_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x65C | func: 4 | export: 4
void dll_580_obj_Free(Object* self, s32 onlySelf);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/580_SB_CloudBall/dll_580_obj_Free.s")

// offset: 0x704 | func: 5 | export: 5
u32 dll_580_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x714 | func: 6 | export: 6
u32 dll_580_obj_GetDataSize(Object* self, u32 offsetAddr);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/580_SB_CloudBall/dll_580_obj_GetDataSize.s")
