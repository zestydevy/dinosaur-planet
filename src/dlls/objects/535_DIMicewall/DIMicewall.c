#include "common.h"
#include "sys/objtype.h"
#include "dlls/objects/211_tricky.h"

typedef struct {
    u16 unk0;
    s8 unk2;
    s8 unk3;
    s8 unk4;
} DLL535_Data;

typedef struct {
    ObjSetup base;
    u8 _unk18;
    s16 unk1A ;
    s16 unk1C ;
    s16 unk1E ;   
} DLL535_Setup;

// offset: 0x0 | ctor
void dll_535_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_535_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_535_setup(Object* self, DLL535_Setup* setup, s32 arg2) {
    DLL535_Data* DLL535data;

    DLL535data = self->data;
    DLL535data->unk2 = (s8) setup->unk1A;
    DLL535data->unk4 = (s8) setup->unk1C;
    DLL535data->unk0 = (s16) setup->unk1E;
    obj_add_object_type(self, 0xE);
    obj_add_object_type(self, 0x33);
    self->unkB0 |= 0x6000;
}

// offset: 0xA4 | func: 1 | export: 1
void dll_535_control(Object* self) {
    DLL535_Data* DLL535data;
    Object* sp20;
    s16 temp_a0;

    DLL535data = self->data;
    if (DLL535data->unk3 == 0) {
        if ((DLL535data->unk2 <= 0) && (DLL535data->unk3 == 0)) {
            temp_a0 = DLL535data->unk0;
            if (temp_a0 != -1) {
                main_set_bits((s32) temp_a0, 1U);
                DLL535data->unk3 = 1;
            }
        } else {
            sp20 = get_sidekick();
            if (sp20 != NULL) {
                if (vec3_distance_squared(&self->positionMirror, &get_player()->positionMirror) <= (f32) (DLL535data->unk4 * DLL535data->unk4)) {
                    ((DLL_211_Tricky*)sp20->dll)->vtbl->base.func14(sp20, 4);
                }
            }
        }
    }
}

// offset: 0x1D0 | func: 2 | export: 2
void dll_535_update(Object *self) { }

// offset: 0x1DC | func: 3 | export: 3
void dll_535_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1F4 | func: 4 | export: 4
void dll_535_free(Object* self, s32 a1) {
    obj_free_object_type(self, 0xE);
    obj_free_object_type(self, 0x33);
}

// offset: 0x254 | func: 5 | export: 5
u32 dll_535_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x264 | func: 6 | export: 6
u32 dll_535_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL535_Data);
}

// offset: 0x278 | func: 7 | export: 7
s32 dll_535_func_278(Object* self, s32 arg1) {
    DLL535_Data* DLL535data;

    DLL535data = self->data;
    DLL535data->unk2 = (s8) (DLL535data->unk2 - arg1);
    if (DLL535data->unk2 <= 0) {
        return 1;
    }
    return 0;
}