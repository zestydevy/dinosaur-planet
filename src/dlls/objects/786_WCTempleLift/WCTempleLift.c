#include "dlls/objects/210_player.h"
#include "sys/objects.h"
#include "dll.h"
#include "functions.h"

typedef struct {
    f32 unk0;
    u8 unk4;
} WCTempleLift_Data;

typedef struct {
    ObjSetup base;
    s8 unk18;
} WCTempleLift_Setup;

// offset: 0x0 | ctor
void dll_786_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_786_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_786_setup(Object *self, WCTempleLift_Setup *setup, s32 arg2) {
    self->srt.yaw = setup->unk18 << 8;
}

// offset: 0x30 | func: 1 | export: 1
void dll_786_control(Object *self) {
    WCTempleLift_Data *objdata;
    Object *player;
    s32 i;
    Object *obj;

    objdata = self->data;
    objdata->unk0 -= gUpdateRateF;
    if (objdata->unk0 < 0.0f) {
        objdata->unk0 = 0.0f;
    }
    if (objdata->unk4 == 0) {
        if (self->unk58->unk10F > 0) {
            for (i = 0; i < self->unk58->unk10F; i++) {
                obj = self->unk58->unk100[i];
                if (obj->id == 0) {
                    player = get_player();
                    if ((objdata->unk0 <= 0.0f) && (((DLL_210_Player*)player->dll)->vtbl->func70(player) == 0x21)) {
                        gDLL_3_Animation->vtbl->func17(0, self, -1);
                        objdata->unk4 = 1;
                        objdata->unk0 = 300.0f;
                    }
                }
            }
        }
    } else {
        if (self->unk58->unk10F > 0) {
            for (i = 0; i < self->unk58->unk10F; i++) {
                obj = self->unk58->unk100[i];
                if (obj->id == 0) {
                    player = get_player();
                    if ((objdata->unk0 <= 0.0f) && (((DLL_210_Player*)player->dll)->vtbl->func70(player) == 0x21)) {
                        gDLL_3_Animation->vtbl->func17(1, self, -1);
                        objdata->unk4 = 0;
                        objdata->unk0 = 300.0f;
                    }
                }
            }
        }
    }
}

// offset: 0x284 | func: 2 | export: 2
void dll_786_update(Object *self) { }

// offset: 0x290 | func: 3 | export: 3
void dll_786_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2E4 | func: 4 | export: 4
void dll_786_free(Object *self, s32 a1) { }

// offset: 0x2F4 | func: 5 | export: 5
u32 dll_786_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x304 | func: 6 | export: 6
u32 dll_786_get_data_size(Object *self, u32 a1) {
    return sizeof(WCTempleLift_Data);
}
