#include "common.h"
#include "game/objects/curve_types.h"

/*0x0*/ static s32 _data_0 = -1;

/*0x0*/ static s32 _bss_0;
/*0x4*/ static s32 _bss_4;
/*0x8*/ static s32 _bss_8;

// offset: 0x0 | ctor
void curve_ctor(void *dll) {
    _bss_4 = -1;
}

// offset: 0x24 | dtor
void curve_dtor(void *dll) { }

// offset: 0x30 | func: 0 | export: 0
void curve_setup(Object* self, CurveSetup* curveSetup, s32 arg2) {
    self->srt.yaw = curveSetup->unk2C << 8;
    self->srt.pitch = curveSetup->unk2D << 8;
    
    if (curveSetup->unk19 == CURVE_TYPE_8 || curveSetup->unk19 == CURVE_TYPE_Camera_1A) {
        self->srt.roll = *((s16*)((s8*)curveSetup + 0x38)); //TO-DO: update struct
    }
    
    if (curveSetup->unk19 == CURVE_TYPE_15) {
        self->srt.scale = 1.25f;
        return;
    }

    if (curveSetup->unk19 == CURVE_TYPE_16) {
        self->srt.scale = 1.1f;
        return;
    }
    
    self->srt.scale = self->def->scale;
}

// offset: 0xC4 | func: 1 | export: 1
void curve_control(Object *self) { }

// offset: 0xD0 | func: 2 | export: 2
void curve_update(Object *self) { }

// offset: 0xDC | func: 3 | export: 3
void curve_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x130 | func: 4 | export: 4
void curve_free(Object *self, s32 a1) { }

// offset: 0x140 | func: 5 | export: 5
u32 curve_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x150 | func: 6 | export: 6
u32 curve_get_data_size(Object *self, u32 a1) {
    return 0;
}

// offset: 0x164 | func: 7 | export: 7
void curve_func_164(s32 arg0) {

}

// offset: 0x170 | func: 8 | export: 8
s32 curve_func_170(void) {
    return _bss_4;
}
