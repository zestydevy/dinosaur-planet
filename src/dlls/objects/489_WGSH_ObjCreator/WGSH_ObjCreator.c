#include "dlls/engine/6_amsfx.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    u8 _unk18[0x1E - 0x18];
    s8 rotation;
    s8 disableSpawner;
} WGSH_ObjCreator_Setup;

typedef struct {
    s32 unk0;
    s16 timer;
    s16 countdownRate;
} WGSH_ObjCreator_Data;

// offset: 0x0 | ctor
void WGSH_ObjCreator_ctor(void *dll) { }

// offset: 0xC | dtor
void WGSH_ObjCreator_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WGSH_ObjCreator_setup(Object *self, WGSH_ObjCreator_Setup *setup, s32 arg2) {
    WGSH_ObjCreator_Data *objdata = self->data;

    self->srt.yaw = setup->rotation << 8;
    self->unkE0 = 0;
    objdata->timer = 100;
    objdata->countdownRate = 0;
    objdata->unk0 = 0;
    self->opacityWithFade = 0xFF;
    self->opacity = 0xFF;
}

// offset: 0x54 | func: 1 | export: 1
void WGSH_ObjCreator_control(Object *self) {
    WGSH_ObjCreator_Setup *setup;
    WGSH_ObjCreator_Data *objdata;
    ObjSetup *flybaddieSetup;
    DLL_IModgfx *sp38;

    setup = (WGSH_ObjCreator_Setup*)self->setup;
    objdata = self->data;
    if ((self->unkE0 != 0) && (main_get_bits(BIT_1D4) != 0)) {
        self->unkE0 = 0;
    }
    if ((self->unkE0 == 0) && (main_get_bits(BIT_1D3) != 0)) {
        sp38 = dll_load_deferred(DLL_ID_146, 1);
        sp38->vtbl->func0(self, 0, 0, 1, -1, 0);
        sp38->vtbl->func0(self, 1, 0, 1, -1, 0);
        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_303, MAX_VOLUME, NULL, NULL, 0, NULL);
        dll_unload(sp38);
        objdata->countdownRate = 1;
        self->unkE0 = 1;
    }
    if (objdata->countdownRate != 0) {
        objdata->timer -= (objdata->countdownRate * gUpdateRate);
    }
    if ((objdata->timer <= 0) && !setup->disableSpawner) {
        // @bug: Does not allocate enough bytes for flybaddie setup (should be 0x1C bytes)
        flybaddieSetup = obj_alloc_setup(0x18, OBJ_WGSH_flybaddie);
        flybaddieSetup->x = setup->base.x;
        flybaddieSetup->y = setup->base.y + 50.0f;
        flybaddieSetup->z = setup->base.z;
        flybaddieSetup->objId = OBJ_WGSH_flybaddie;
        flybaddieSetup->uID = -1;
        flybaddieSetup->loadFlags = setup->base.loadFlags;
        flybaddieSetup->byte5 = setup->base.byte5;
        flybaddieSetup->byte6 = setup->base.byte6;
        flybaddieSetup->fadeDistance = setup->base.fadeDistance;
        obj_create(flybaddieSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
        objdata->timer = 100;
        objdata->countdownRate = 0;
    }
}

// offset: 0x294 | func: 2 | export: 2
void WGSH_ObjCreator_update(Object *self) { }

// offset: 0x2A0 | func: 3 | export: 3
void WGSH_ObjCreator_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2F4 | func: 4 | export: 4
void WGSH_ObjCreator_free(Object *self, s32 a1) { }

// offset: 0x304 | func: 5 | export: 5
u32 WGSH_ObjCreator_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x314 | func: 6 | export: 6
u32 WGSH_ObjCreator_get_data_size(Object *self, u32 a1) {
    return sizeof(WGSH_ObjCreator_Data);
}
