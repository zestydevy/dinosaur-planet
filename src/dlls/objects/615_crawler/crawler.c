#include "common.h"
#include "sys/objtype.h"

//Used by Skeetlas/Scorpions

typedef struct {
    u8 _unk0[0x298];
} DLL615_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000002, 0x00040001, 0x000c03f7, 0x0167050c
};
/*0x10*/ static f32 _data_10 = 0.1;
/*0x14*/ static u32 _data_14 = 0x00000000;
/*0x18*/ static u32 _data_18[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x24*/ static f32 _data_24 = 10;

// offset: 0x0 | ctor
void dll_615_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_615_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_615_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/615_crawler/dll_615_setup.s")

// offset: 0x288 | func: 1 | export: 1
void dll_615_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/615_crawler/dll_615_control.s")

// offset: 0x17B0 | func: 2 | export: 2
void dll_615_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/615_crawler/dll_615_update.s")

// offset: 0x1888 | func: 3 | export: 3
void dll_615_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/615_crawler/dll_615_print.s")

// offset: 0x194C | func: 4 | export: 4
void dll_615_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/615_crawler/dll_615_free.s")

// offset: 0x19D8 | func: 5 | export: 5
u32 dll_615_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x19E8 | func: 6 | export: 6
u32 dll_615_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL615_Data);
}

// offset: 0x19FC | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/615_crawler/dll_615_func_19FC.s")

// offset: 0x1AC8 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/615_crawler/dll_615_func_1AC8.s")

// offset: 0x1AE8 | func: 9
void dll_615_func_1AE8(Object* self) {
    ObjSetup *objSetup;
    
    objSetup = self->setup;
    
    if (objSetup->uID == 0){
        func_800267A4(self);
        obj_destroy_object(self);
        return;
    }
    
    obj_free_tick(self);
    func_800267A4(self);
    obj_free_object_type(self, 4);    
    self->srt.flags |= 0x4000;
}
