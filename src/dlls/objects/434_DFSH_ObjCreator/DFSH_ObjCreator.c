#include "dlls/engine/33_BaddieControl.h"
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
/*00*/ ObjSetup base;
/*18*/ u8 _unk18[0x1E - 0x18];
/*1E*/ s8 rotation; // yaw >> 8
/*1F*/ s8 type;
} DFSH_ObjCreator_Setup;

typedef struct {
/*0*/ s16 timer;
/*2*/ s16 timerRate;
} DFSH_ObjCreator_Data;

// offset: 0x0 | ctor
void DFSH_ObjCreator_ctor(void* dll) { }

// offset: 0xC | dtor
void DFSH_ObjCreator_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFSH_ObjCreator_setup(Object* self, DFSH_ObjCreator_Setup* setup, s32 reset) {
    DFSH_ObjCreator_Data* objdata = self->data;

    self->srt.yaw = setup->rotation << 8;
    self->unkE0 = 0;
    objdata->timer = 100;
    objdata->timerRate = 0;
    self->opacityWithFade = 0xFF;
    self->opacity = 0xFF;
}

// offset: 0x50 | func: 1 | export: 1
void DFSH_ObjCreator_control(Object* self) {
    DFSH_ObjCreator_Setup* setup = (DFSH_ObjCreator_Setup*)self->setup;
    DFSH_ObjCreator_Data* objdata = self->data;
    Baddie_Setup* sharpClawSetup;
    Object* sharpClaw;
    Baddie* sharpClawBaddie;
    DLL_IModgfx* modgfx;

    if (main_get_bits(BIT_589) != 0) {
        self->unkE0 = 0;
        return;
    }
    if ((self->unkE0 == 0) && (main_get_bits(setup->type + BIT_F6) != 0)) {
        modgfx = dllLoadDeferred(DLL_ID_146, 1);
        modgfx->vtbl->func0(self, 0, 0, 1, -1, 0);
        modgfx->vtbl->func0(self, 1, 0, 1, -1, 0);
        gDLL_6_AMSFX->vtbl->play(NULL, SOUND_303, MAX_VOLUME, NULL, NULL, 0, NULL);
        dllFree(modgfx);
        objdata->timerRate = 1;
        self->unkE0 = 1;
    }
    if (objdata->timerRate != 0) {
        objdata->timer -= (objdata->timerRate * (s16) gUpdateRateF);
    }
    if (objdata->timer <= 0) {
        sharpClawSetup = obj_alloc_setup(sizeof(Baddie_Setup), OBJ_ClubSharpClaw);
        sharpClawSetup->base.x = setup->base.x;
        sharpClawSetup->base.y = setup->base.y;
        sharpClawSetup->base.z = setup->base.z;
        sharpClawSetup->base.loadFlags = setup->base.loadFlags;
        sharpClawSetup->base.byte5 = setup->base.byte5;
        sharpClawSetup->base.byte6 = setup->base.byte6;
        sharpClawSetup->base.fadeDistance = setup->base.fadeDistance;
        sharpClawSetup->initialWeaponID = 3;
        sharpClawSetup->unk18 = BIT_1E7;
        sharpClawSetup->unk30 = -1;
        sharpClawSetup->unk2A = (s8) (self->srt.yaw >> 8);
        sharpClawSetup->unk2B = 2;
        if (main_get_bits(BIT_FC) != 0) {
            sharpClawSetup->unk22 = 0x49;
        } else {
            sharpClawSetup->unk22 = -1;
        }
        sharpClawSetup->unk29 = 0xFF;
        sharpClawSetup->unk2E = -1;
        sharpClawSetup->unk34 = 0xFFFF;
        switch (setup->type) {
        default:
            sharpClawSetup->quarterHitpoints = 3;
            break;
        case 0:
            sharpClawSetup->quarterHitpoints = 5;
            break;
        case 1:
            sharpClawSetup->quarterHitpoints = 3;
            break;
        case 2:
            sharpClawSetup->quarterHitpoints = 4;
            break;
        case 3:
            sharpClawSetup->quarterHitpoints = 3;
            break;
        }
        sharpClaw = obj_create((ObjSetup*)sharpClawSetup, OBJINIT_FLAG4 | OBJINIT_STANDALONE, self->mapID, -1, self->parent);
        if (sharpClaw != NULL) {
            sharpClawBaddie = sharpClaw->data;
            if (sharpClawBaddie != NULL) {
                switch (setup->type) {
                default:
                    sharpClawBaddie->unk3B0 = 0x20;
                    break;
                case 0:
                    sharpClawBaddie->unk3B0 = 0x20;
                    break;
                case 1:
                    sharpClawBaddie->unk3B0 = 0x20;
                    break;
                case 2:
                    sharpClawBaddie->unk3B0 = 0xA0;
                    break;
                case 3:
                    sharpClawBaddie->unk3B0 = 0xA0;
                    break;
                }
            }
        }
        objdata->timer = 100;
        objdata->timerRate = 0;
    }
}

// offset: 0x3D8 | func: 2 | export: 2
void DFSH_ObjCreator_update(Object* self) { }

// offset: 0x3E4 | func: 3 | export: 3
void DFSH_ObjCreator_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x438 | func: 4 | export: 4
void DFSH_ObjCreator_free(Object* self, s32 onlySelf) { }

// offset: 0x448 | func: 5 | export: 5
u32 DFSH_ObjCreator_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x458 | func: 6 | export: 6
u32 DFSH_ObjCreator_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DFSH_ObjCreator_Data);
}
