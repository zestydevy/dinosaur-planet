#include "common.h"
#include "sys/objtype.h"

typedef struct {
    Object* unk0[20];
    u8 unk50;
    s8 unk51;
} DLL303_Data;

// offset: 0x0 | ctor
void dll_303_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_303_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_303_setup(Object *self, ObjSetup *setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/303_FireFlyLantern/dll_303_setup.s")

// offset: 0x10C | func: 1 | export: 1
void dll_303_control(Object *self);
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/303_FireFlyLantern/dll_303_control.s")
#else
void dll_303_control(Object* self) {
    DLL303_Data* objData;
    s32 i;
    
    objData = self->data;
    
    if (objData->unk51 >= 0) {
        return;
    }
    
    for (i = 0; i < objData->unk50; i++) {
        obj_destroy_object(objData->unk0[i]);
    }
    
    obj_destroy_object(self);
}
#endif

// offset: 0x1B0 | func: 2 | export: 2
void dll_303_update(Object *self) { }

// offset: 0x1BC | func: 3 | export: 3
void dll_303_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1D4 | func: 4 | export: 4
void dll_303_free(Object* self, s32 onlySelf) {
    obj_free_object_type(self, 0x11);
}

// offset: 0x214 | func: 5 | export: 5
u32 dll_303_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x224 | func: 6 | export: 6
u32 dll_303_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DLL303_Data);
}

// offset: 0x238 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/303_FireFlyLantern/dll_303_func_238.s")

// offset: 0x39C | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/303_FireFlyLantern/dll_303_func_39C.s")
