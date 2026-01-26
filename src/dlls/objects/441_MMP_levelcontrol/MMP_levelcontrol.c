#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "sys/objects.h"
#include "dll.h"
#include "functions.h"

static int MMP_levelcontrol_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void MMP_levelcontrol_ctor(void *dll) { }

// offset: 0xC | dtor
void MMP_levelcontrol_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void MMP_levelcontrol_setup(Object *self, ObjSetup *setup, s32 arg2) {
    self->unkB0 |= 0x6000;
    self->unkDC = 1;
    self->animCallback = MMP_levelcontrol_anim_callback;
}

// offset: 0x54 | func: 1 | export: 1
void MMP_levelcontrol_control(Object *self) {
    Object *player;

    if (self->unkDC != 0) {
        player = get_player();
        gDLL_6_AMSFX->vtbl->water_falls_set_flags(0x10);
        func_80000860(self, player, 0x13A, 0);
        func_80000860(self, player, 0x138, 0);
        func_80000860(self, player, 0x139, 0);
        func_80000450(self, player, 0x1DC, 0, 0, 0);
        gDLL_5_AMSEQ2->vtbl->set(self, 0xF, 0, 0, 0);
        self->unkDC = 0;
    }
    gDLL_6_AMSFX->vtbl->func_16E0();
}

// offset: 0x1A0 | func: 2 | export: 2
void MMP_levelcontrol_update(Object *self) { }

// offset: 0x1AC | func: 3 | export: 3
void MMP_levelcontrol_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x200 | func: 4 | export: 4
void MMP_levelcontrol_free(Object *self, s32 a1) {
    gDLL_5_AMSEQ2->vtbl->free(self, 0xF, 0, 0, 0);
    gDLL_5_AMSEQ2->vtbl->set(self, 0x107, 0, 0, 0);
}

// offset: 0x290 | func: 5 | export: 5
u32 MMP_levelcontrol_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2A0 | func: 6 | export: 6
u32 MMP_levelcontrol_get_data_size(Object *self, u32 a1) {
    return 0;
}

// offset: 0x2B4 | func: 7
int MMP_levelcontrol_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    s32 i;
    Object *player;

    player = get_player();
    animObjData->unk62 = 0;
    for (i = 0; i < animObjData->unk98; i++) {
        switch (animObjData->unk8E[i]) {
        case 1:
            func_80000860(self, player, 0x13B, 0);
            func_80000450(self, player, 0x263, 0, 0, 0);
            func_80000450(self, player, 0x264, 0, 0, 0);
            break;
        case 2:
            func_80000860(self, player, 0x138, 0);
            func_80000450(self, player, 0x1DC, 0, 0, 0);
            break;
        default:
            break;
        }
    }

    MMP_levelcontrol_control(self);
    return 0;
}
