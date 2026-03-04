#include "common.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    u8 _unk18;
    u8 unk19;
    u8 unk1A;
    u8 _unk1B;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
}DLL715_Setup;

typedef struct {
    u8 unk0;
    u8 unk1;
    f32 unk4;
} DLL715_Data;

// offset: 0x0 | ctor
void dll_715_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_715_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_715_setup(Object* self, DLL715_Setup* setup, s32 arg2) {
    f32 var_ft1;
    u8 temp_t3;
    DLL715_Data* objdata;

    objdata = self->data;
    self->srt.yaw = -setup->unk1C;
    self->srt.pitch = -setup->unk1E;
    self->srt.roll = -setup->unk20;
    objdata->unk0 = setup->unk19;
    objdata->unk4 = setup->unk1A;
    objdata->unk1 = 0;
    if (arg2 == 0) {
        obj_add_object_type(self, 8);
    }
}

// offset: 0xB0 | func: 1 | export: 1
void dll_715_control(Object *self) { }

// offset: 0xBC | func: 2 | export: 2
void dll_715_update(Object *self) { }

// offset: 0xC8 | func: 3 | export: 3
void dll_715_print(Object* arg0, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle** arg4, s8 arg5) {
    if (arg5 != 0) {
        draw_object(arg0, arg1, arg2, arg3, arg4, 1.0f);
    }
}

// offset: 0x11C | func: 4 | export: 4
void dll_715_free(Object* self, s32 a1) {
    obj_free_object_type(self, 8);
}

// offset: 0x15C | func: 5 | export: 5
u32 dll_715_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x16C | func: 6 | export: 6
u32 dll_715_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL715_Data);
}
