#include "common.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s32 _unk18;
    s16 unk1C;
}DLL670_Setup;

typedef struct {
    u8 unk0;
} DLL670_Data;

// offset: 0x0 | ctor
void dll_670_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_670_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_670_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/670_DBExpbit/dll_670_setup.s")

// offset: 0x98 | func: 1 | export: 1
void dll_670_control(Object* self) {
    DLL670_Data* objdata;
    objdata = self->data;
    objdata->unk0 = NULL;
}

// offset: 0xA8 | func: 2 | export: 2
void dll_670_update(Object *self) { }

// offset: 0xB4 | func: 3 | export: 3
void dll_670_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL670_Data* objdata;
    DLL670_Setup* setup;

    setup = (DLL670_Setup*)self->setup;
    objdata = self->data;
    
    if (visibility != 0 && ( setup->unk1C != 0 || objdata->unk0 != 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0x12C | func: 4 | export: 4
void dll_670_free(Object* arg0, s32 arg1) {
    obj_free_object_type(arg0, OBJTYPE_41);
}

// offset: 0x16C | func: 5 | export: 5
u32 dll_670_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x17C | func: 6 | export: 6
u32 dll_670_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL670_Data);
}
