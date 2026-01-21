#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "functions.h"
#include "game/gamebits.h"
#include "sys/joypad.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
} SHswaplift_Setup;

// offset: 0x0 | ctor
void SHswaplift_ctor(void *dll) { }

// offset: 0xC | dtor
void SHswaplift_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHswaplift_setup(Object *self, SHswaplift_Setup *setup, s32 arg2) {
    self->srt.yaw = setup->yaw << 8;
    if (map_get_map_id_from_xz_ws(self->srt.transl.x, self->srt.transl.z) == MAP_SWAPSTONE_CIRCLE) {
        self->unkDC = MAP_SWAPSTONE_CIRCLE;
    }
}

// offset: 0x7C | func: 1 | export: 1
void SHswaplift_control(Object *self) {
    s32 index;
    Object *listedObject;
    s32 dostuff;

    dostuff = FALSE;
    if (self->unk58->unk10F > 0) {
        for (index = 0; index < self->unk58->unk10F; index++) {
            listedObject = (Object*)self->unk58->unk100[index];
            if (listedObject->group == GROUP_UNK1) {
                dostuff = TRUE;
            }
        }
    }
    if (dostuff) {
        self->unkAF &= ~0x8;

        if (self->unkAF & 0x1) {
            if (self->unkDC == MAP_SWAPSTONE_CIRCLE) {
                main_set_bits(BIT_Play_Seq_0107_Rocky_Intro_Unused, 1);
            } else {
                main_set_bits(BIT_Play_Seq_035F_Rocky_Intro, 1);
            }

            joy_set_button_mask(0, A_BUTTON);
        }
    } else {
        self->unkAF |= 0x8;
    }
}

// offset: 0x188 | func: 2 | export: 2
void SHswaplift_update(Object *self) { }

// offset: 0x194 | func: 3 | export: 3
void SHswaplift_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1E8 | func: 4 | export: 4
void SHswaplift_free(Object *self, s32 a1) { }

// offset: 0x1F8 | func: 5 | export: 5
u32 SHswaplift_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x208 | func: 6 | export: 6
u32 SHswaplift_get_data_size(Object *self, u32 a1) {
    return 0;
}
