#include "common.h"
#include "sys/objtype.h"
#include "sys/gfx/modgfx.h"

// offset: 0x0 | ctor
void dll_336_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_336_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
typedef struct {
    ObjSetup base;
    s8 unk18;
    s16 unk1A;
    s16 unk1C;
} DLL336Setup;

void dll_336_setup(Object* self, DLL336Setup* setup, s32 arg2) {
    s32 sp4C;
    DLL_IModgfx* sp48;
    SRT sp30;

    sp4C = (s32) setup->base.objId;
    obj_add_object_type(self, OBJTYPE_55);
    if (sp4C == OBJ_FirePole) {
        self->srt.yaw = setup->unk18 << 8;
        self->srt.pitch = 0;
        self->srt.roll = setup->unk1C;
        if (setup->unk1A > 0) {
            self->srt.scale = setup->unk1A / 8192.0f;
        } else {
            self->srt.scale = 0.1f;
        }
        sp30.transl.y = 2.0f;
        sp48 = dll_load_deferred(0x1011, 1);
        sp48->vtbl->func0(self, 0, &sp30, 0x10004, -1, 0);
        dll_unload(sp48);
    }
}

// offset: 0x13C | func: 1 | export: 1
void dll_336_control() {
}

// offset: 0x144 | func: 2 | export: 2
void dll_336_update(Object *self) { }

// offset: 0x150 | func: 3 | export: 3
void dll_336_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1A4 | func: 4 | export: 4
void dll_336_free(Object* self, s32 a1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
    obj_free_object_type(self, OBJTYPE_55);
}

// offset: 0x22C | func: 5 | export: 5
u32 dll_336_get_model_flags(Object* self) {
    return 1U;
}

// offset: 0x23C | func: 6 | export: 6
u32 dll_336_get_data_size(Object *self, u32 a1) {
    return 0;
}
