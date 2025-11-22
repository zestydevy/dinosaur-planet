#include "sys/objanim.h"

// offset: 0x0 | ctor
void SB_ShipMast_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_ShipMast_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_ShipMast_setup(Object *self, ObjSetup *setup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void SB_ShipMast_control(Object *self) {
    s32 parentDC;
    f32 animTickDelta;

    if (self->parent) {
        parentDC = self->parent->unkDC;
        if (self->parent->id == OBJ_WL_Galleon) {
            if ((parentDC >= 10) && (parentDC < 13)) {
                if (self->curModAnimId != 0) {
                    func_80023D30(self, 0, 0.0f, 0);
                }
                if (parentDC >= 12) {
                    animTickDelta = -0.003f;
                } else {
                    animTickDelta = 0.003f;
                }
            } else {
                if (self->curModAnimId != 1) {
                    func_80023D30(self, 1, 0.0f, 0);
                }
                animTickDelta = 0.03f;
            }
        } else {
            if (self->curModAnimId != 1) {
                func_80023D30(self, 1, 0.0f, 0);
            }
            animTickDelta = 0.03f;
        }
        func_80024108(self, animTickDelta, gUpdateRate, NULL);
    }
}

// offset: 0x190 | func: 2 | export: 2
void SB_ShipMast_update(Object *self) { }

// offset: 0x19C | func: 3 | export: 3
void SB_ShipMast_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1F0 | func: 4 | export: 4
void SB_ShipMast_free(Object *self, s32 a1) { }

// offset: 0x200 | func: 5 | export: 5
u32 SB_ShipMast_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x210 | func: 6 | export: 6
u32 SB_ShipMast_get_data_size(Object *self, u32 a1) {
    return 0;
}
