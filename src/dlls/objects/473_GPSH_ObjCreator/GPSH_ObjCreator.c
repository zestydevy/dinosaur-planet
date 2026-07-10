#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/475_GPSH_flybaddie.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "dll.h"
#include "macros.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 _unk18[0x1A - 0x18];
/*1A*/ s16 type;
/*1C*/ u8 _unk1C[0x1E - 0x1C];
/*1E*/ s8 rotation; // yaw >> 8
} GPSH_ObjCreator_Setup;

typedef struct {
/*0*/ s16 timer;
/*2*/ s16 timerRate;
/*4*/ u8 type;
/*5*/ u8 spawned;
} GPSH_ObjCreator_Data;

// Shared between GPSHpickobj* objects?
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18;
/*1A*/ s16 unk1A;
/*1C*/ u8 _unk1C[0x24 - 0x1C];
} GPSH_PickObj_Setup;

/*0x0*/ static s16 data_0[] = {
    0x0028, // GPSHpickobjroot
    0x0028, // GPSHpickobjnugg
    0x0030, // GPSHpickobjegg
    0x0030, // GPSHpickobjgem
    0x002d, // GPSHpickobjbarr
    0x002d  // GPSHpickobjplan
};

// offset: 0x0 | ctor
void GPSH_ObjCreator_ctor(void* dll) { }

// offset: 0xC | dtor
void GPSH_ObjCreator_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void GPSH_ObjCreator_setup(Object* self, GPSH_ObjCreator_Setup* setup, s32 reset) {
    GPSH_ObjCreator_Data* objdata = self->data;

    self->srt.yaw = setup->rotation << 8;
    self->unkE0 = 0;
    objdata->timer = 100;
    objdata->timerRate = 0;
    objdata->type = setup->type;
    STUBBED_PRINTF("type %d\n", objdata->type);
    objdata->spawned = FALSE;
    self->opacityWithFade = 0xFF;
    self->opacity = 0xFF;
}

// offset: 0x5C | func: 1 | export: 1
void GPSH_ObjCreator_control(Object* self) {
    GPSH_ObjCreator_Setup* setup;
    GPSH_PickObj_Setup* pickObjSetup;
    GPSH_flybaddie_Setup* flybaddieSetup;
    GPSH_ObjCreator_Data* objdata;
    s32 _pad;
    DLL_IModgfx* modgfx;

    setup = (GPSH_ObjCreator_Setup*)self->setup;
    objdata = self->data;
    if (mainGetBits(BIT_5AF) != 0) {
        // Reset
        self->unkE0 = 0;
        objdata->timer = 100;
        objdata->timerRate = 0;
        objdata->spawned = FALSE;
        self->opacityWithFade = 0xFF;
        self->opacity = 0xFF;
    }
    if (!objdata->spawned) {
        if ((objdata->type == 6) && (self->unkE0 == 0) && (mainGetBits(BIT_5AC) != 0)) {
            // Start flybaddie spawn
            modgfx = dllLoadDeferred(DLL_ID_146, 1);
            modgfx->vtbl->func0(self, 3, 0, 1, -1, 0);
            modgfx->vtbl->func0(self, 4, 0, 1, -1, 0);
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_303, MAX_VOLUME, NULL, NULL, 0, NULL);
            dllFree(modgfx);
            objdata->timerRate = 1;
            self->unkE0 = 1;
        } else if ((self->unkE0 == 0) && (mainGetBits(BIT_148) != 0)) {
            // Start pickup spawn
            modgfx = dllLoadDeferred(DLL_ID_146, 1);
            modgfx->vtbl->func0(self, 3, 0, 1, -1, 0);
            modgfx->vtbl->func0(self, 4, 0, 1, -1, 0);
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_303, MAX_VOLUME, NULL, NULL, 0, NULL);
            dllFree(modgfx);
            objdata->timerRate = 1;
            self->unkE0 = 1;
        }
        if (objdata->timerRate != 0) {
            objdata->timer -= (objdata->timerRate * gUpdateRate);
        }
        if (objdata->type != 6) {
            if (objdata->timer <= 0) {
                // Spawn pickup
                pickObjSetup = obj_alloc_setup(sizeof(GPSH_PickObj_Setup), objdata->type + OBJ_GPSHpickobjroot);
                objdata->spawned = TRUE;
                pickObjSetup->base.fadeDistance = 0xFF;
                pickObjSetup->base.loadFlags = OBJSETUP_LOAD_FLAG20;
                pickObjSetup->base.fadeFlags = OBJSETUP_FADE_MAIN;
                pickObjSetup->base.x = self->srt.transl.x;
                pickObjSetup->base.y = self->srt.transl.y;
                pickObjSetup->base.z = self->srt.transl.z;
                pickObjSetup->base.objId = objdata->type + OBJ_GPSHpickobjroot;
                pickObjSetup->unk18 = (s8) (self->srt.yaw >> 8);
                pickObjSetup->unk1A = data_0[objdata->type];
                obj_create((ObjSetup*)pickObjSetup, OBJINIT_FLAG4 | OBJINIT_STANDALONE, self->mapID, -1, self->parent)
                    ->modelInstIdx = 0;
                objdata->timer = 100;
                objdata->timerRate = 0;
            }
        } else {
            if (objdata->timer <= 0) {
                // Spawn flybaddie
                flybaddieSetup = obj_alloc_setup(sizeof(GPSH_flybaddie_Setup), OBJ_GPSH_flybaddie);
                flybaddieSetup->base.x = setup->base.x;
                flybaddieSetup->base.y = setup->base.y + 50.0f;
                flybaddieSetup->base.z = setup->base.z;
                flybaddieSetup->base.objId = OBJ_GPSH_flybaddie;
                flybaddieSetup->base.uID = -1;
                flybaddieSetup->base.loadFlags = setup->base.loadFlags;
                flybaddieSetup->base.byte5 = setup->base.byte5;
                flybaddieSetup->base.byte6 = setup->base.byte6;
                flybaddieSetup->base.fadeDistance = setup->base.fadeDistance;
                flybaddieSetup->unk1A = -0x3C;
                obj_create((ObjSetup*)flybaddieSetup, OBJINIT_FLAG4 | OBJINIT_STANDALONE, self->mapID, -1, self->parent);
                objdata->timer = 100;
                objdata->timerRate = 0;
            }
        }
    }
}

// offset: 0x494 | func: 2 | export: 2
void GPSH_ObjCreator_update(Object* self) { }

// offset: 0x4A0 | func: 3 | export: 3
void GPSH_ObjCreator_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4F4 | func: 4 | export: 4
void GPSH_ObjCreator_free(Object* self, s32 onlySelf) { }

// offset: 0x504 | func: 5 | export: 5
u32 GPSH_ObjCreator_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x514 | func: 6 | export: 6
u32 GPSH_ObjCreator_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(GPSH_ObjCreator_Data);
}
