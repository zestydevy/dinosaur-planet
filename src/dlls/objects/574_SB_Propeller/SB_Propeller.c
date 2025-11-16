#include "dll.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/objects.h"
#include "sys/main.h"
#include "functions.h"
#include "dlls/objects/572_SB_Galleon.h"

/*0x0*/ static u32 _data_0[] = { 0x00000258, 0x00000000, 0x00000000, 0x00000000 }; // ?

typedef struct {
/*00*/ u32 soundHandle;
/*04*/ f32 torqueFloat;
/*08*/ s32 torque;
/*0C*/ s16 theta;
/*0E*/ u8 _unkE;
/*0F*/ u8 counter;
} SB_Propeller_Data;

// offset: 0x0 | ctor
void SB_Propeller_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_Propeller_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_Propeller_setup(Object *self, ObjSetup *setup, s32 arg2) {
    SB_Propeller_Data *objdata;

    objdata = self->data;
    objdata->torque = 1600;
    objdata->torqueFloat = 1.0f;
}

// offset: 0x4C | func: 1 | export: 1
void SB_Propeller_control(Object *self) {
    s32 _pad;
    SB_Propeller_Data *objdata;
    Object *player;
    Object *parent;
    s32 sp54;
    s32 torque;
    s32 parent_unkDC;
    Object *sp48;
    s32 var_v1;
    s32 i;

    parent = self->parent;
    objdata = self->data;

    if (!parent)
        return;

    parent_unkDC = self->parent->unkDC;
    if (self->parent->id == OBJ_WL_Galleon) {
        self->objhitInfo->unk58 &= ~1;
        if (!objdata->soundHandle) {
            if (parent_unkDC > 10) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_290, MAX_VOLUME, &objdata->soundHandle, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->func_860(objdata->soundHandle, 1);
                objdata->counter = 1;
            }
        } else {
            objdata->counter++;
            if (objdata->counter > 80) {
                objdata->counter = 80;
            }
            gDLL_6_AMSFX->vtbl->func_860(objdata->soundHandle, objdata->counter);
        }
    } else {
        sp54 = ((DLL_572_SB_Galleon*)self->parent->dll)->vtbl->func8(self->parent);
        if (parent_unkDC < 4) {
            torque = (1800 - objdata->torque) >> 3;
            CLAMP(torque, -2, 2);
            objdata->torque += torque * gUpdateRate;
            objdata->torqueFloat = objdata->torque / 1600.0f;
            if (objdata->torqueFloat < 0.0f) {
                objdata->torqueFloat = -objdata->torqueFloat;
            }
            if (objdata->torqueFloat < 0.2f) {
                objdata->torqueFloat = 0.2f;
            }
        } else {
            if (objdata->soundHandle) {
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
            }
        }
        self->unkDC -= gUpdateRate;
        if (self->unkDC < 0) {
            self->unkDC = 0;
        }
        var_v1 = func_80025F40(self, &sp48, NULL, NULL);
        if (var_v1 == 0) {
            if (self->objhitInfo->unk48) {
                var_v1 = 1;
                sp48 = (Object*)self->objhitInfo->unk48;
            }
        }
        if ((var_v1 != 0) && (self->unkDC == 0) && sp48) {
            player = get_player();
            if (sp48 != player) {
                self->unkDC = 20;
                if ((self->parent) && ((sp54 == 2) || (sp54 == 5))) {
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_177, MAX_VOLUME, NULL, NULL, 0, NULL);
                    objdata->torque /= 3;
                    ((DLL_572_SB_Galleon*)self->parent->dll)->vtbl->func7(self->parent);
                    for (i = 10; i != 0; i--) {
                        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_Explosion, NULL, PARTFXFLAG_2, -1, NULL);
                    }
                }
            } else {
                self->unkDC = 60;
            }
        }
        if (self->unkDC == 0) {
            self->objhitInfo->unk5F = 6;
            self->objhitInfo->unk60 = 1;
            self->objhitInfo->unk40 = 0x10;
            self->objhitInfo->unk44 = 0x10;
        } else {
            self->objhitInfo->unk5D = 0;
        }
    }
    self->srt.roll += objdata->torque * gUpdateRate;
}

// offset: 0x464 | func: 2 | export: 2
void SB_Propeller_update(Object *self) {
    SB_Propeller_Data *objdata;

    objdata = self->data;
    objdata->theta += gUpdateRate * 1200;
}

// offset: 0x4AC | func: 3 | export: 3
void SB_Propeller_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    SB_Propeller_Data *objdata;
    Object *parent;
    ObjSetup *setup;
    f32 temp_fv1;

    setup = self->setup;
    objdata = self->data;
    self->srt.transl.x = setup->x;
    self->srt.transl.y = setup->y;
    self->srt.transl.z = setup->z;

    if (visibility) {
        parent = self->parent;
        if (!self->parent || self->parent->unkDC != 10) {
            parent = self->parent;
            if (parent && parent->id == OBJ_SB_Galleon && parent) {
                temp_fv1 = (1.0f - fsin16_precise(objdata->theta)) * 255.0f;
                if (temp_fv1 < 0.0f) {
                    temp_fv1 = 0.0f;
                } else if (temp_fv1 > 255.0f) {
                    temp_fv1 = 255.0f;
                }
                if (((DLL_572_SB_Galleon*)parent->dll)->vtbl->func8(parent) == 2) {
                    func_80036F6C(0xFF, (u8)temp_fv1, (u8)temp_fv1);
                }
            }
            draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x6B8 | func: 4 | export: 4
void SB_Propeller_free(Object *self, s32 arg1) {
    SB_Propeller_Data *objdata;
    s32 soundHandle;

    objdata = self->data;
    soundHandle = objdata->soundHandle;
    if (soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(soundHandle);
    }
}

// offset: 0x714 | func: 5 | export: 5
u32 SB_Propeller_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x724 | func: 6 | export: 6
u32 SB_Propeller_get_data_size(Object *self, u32 a1) {
    return sizeof(SB_Propeller_Data);
}
