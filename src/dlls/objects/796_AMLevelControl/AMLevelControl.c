#include "common.h"

/*0x0*/ static s16 _data_0[] = {
    0x07d7, 0x07d8, 0x07d9, 0x07da, 0x07db, 0x0000
};

/*0xC*/ static s16 _data_C[] = {
    1, 1, 0, 1, 0, 0
};

/*0x18*/ static Vec3f _data_18[] = {
    12457.1f, -1474.9f, -6690.4f,
    12457.1f, -1474.9f, -6690.4f,
    11181.0f, -1211.4f, -7948.7f,
    12691.3f, -1.9f, -8580.6f,
    12691.3f, -1.9f, -8580.6f,
    0.0f, 0.0f, 0.0f
};

static int dll_796_func_1E8(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3);

// offset: 0x0 | ctor
void dll_796_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_796_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_796_setup(Object* self, ObjSetup* setup, s32 arg2) {
    u8 sp27;

    self->animCallback = dll_796_func_1E8;
    sp27 = main_demo_state();
    gDLL_29_Gplay->vtbl->set_obj_group_status(5, sp27 & 0xFF, 1);
    main_set_bits(_data_0[sp27], 1U);
}

// offset: 0xB8 | func: 1 | export: 1
void dll_796_control(Object* self) {
    u8 temp_a0;

    if (self->unkDC != 0) {
        temp_a0 = main_demo_next();
        main_demo_start(_data_18[temp_a0].x, _data_18[temp_a0].y, _data_18[temp_a0].z, _data_C[temp_a0]);
        self->unkDC = 0;
    }
}


// offset: 0x154 | func: 2 | export: 2
void dll_796_update(Object *self) { }

// offset: 0x160 | func: 3 | export: 3
void dll_796_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1B4 | func: 4 | export: 4
void dll_796_free(Object *self, s32 a1) { }

// offset: 0x1C4 | func: 5 | export: 5
u32 dll_796_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1D4 | func: 6 | export: 6
u32 dll_796_get_data_size(Object *self, u32 a1) {
    return 0;
}

// offset: 0x1E8 | func: 7
static int dll_796_func_1E8(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    arg0->unkDC = 1;
    return 0;
}

