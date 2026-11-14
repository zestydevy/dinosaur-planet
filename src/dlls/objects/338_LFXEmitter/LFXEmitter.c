#include "dlls/engine/26_curves.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "game/objects/setups/102_LFXEmitter.h"
#include "sys/asset.h"
#include "sys/lfx.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/pi.h"
#include "dll.h"

/*0x0*/ static u32 data_0[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static LightAction sLastAction;

static void LFXEmitter_copyLightAction(LightAction* src, LightAction* dst);

// offset: 0x0 | ctor
void LFXEmitter_ctor(void* dll) {
    sLastAction.id = 10000;
}

// offset: 0x24 | dtor
void LFXEmitter_dtor(void* dll) { }

// offset: 0x30 | func: 0 | export: 0
void LFXEmitter_obj_Setup(Object* self, LFXEmitter_Setup* setup, s32 reset) {
    LFXEmitter_Data* objdata = self->data;
    s32 curveType = 0x21;
    
    self->srt.scale = 2.0f * self->def->scale;
    objdata->actionIndex = setup->actionIndex;
    objdata->lifetime = setup->lifetime;
    objdata->unk114 = -2;
    objdata->enableBit = setup->enableBit;
    objdata->rollRate = setup->rollRate;
    objdata->pitchRate = setup->pitchRate;
    objdata->yawRate = setup->yawRate;
    self->srt.transl.x = setup->base.x;
    self->srt.transl.y = setup->base.y;
    self->srt.transl.z = setup->base.z;
    if (objdata->lifetime != 0) {
        objdata->hasLifetimeTimer = TRUE;
    } else {
        objdata->hasLifetimeTimer = FALSE;
    }
    if (setup->followCurve) {
        objdata->flags |= LFXEMITTER_FollowCurve;
        objdata->curveSpeed = (f32) setup->curveSpeed / 10.0f;
        gDLL_26_Curves->vtbl->func_4288(&objdata->unk0, self, 1000.0f, &curveType, -1);
    }
    objAddObjectType(self, OBJTYPE_LFXEmitter);
}

// offset: 0x180 | func: 1 | export: 1
void LFXEmitter_obj_Control(Object* self) {
    LFXEmitter_Data* objdata;
    s32 _pad;
    Object* player;

    objdata = self->data;
    player = objGetPlayer();
    self->srt.yaw += objdata->yawRate;
    self->srt.roll += objdata->rollRate;
    self->srt.pitch += objdata->pitchRate;
    if (objdata->flags & LFXEMITTER_FollowCurve) {
        if ((curves_func_800053B0(&objdata->unk0.unk0, objdata->curveSpeed) != 0) || (objdata->unk0.unk0.unk10 != 0)) {
            gDLL_26_Curves->vtbl->func_4704(&objdata->unk0);
        }
        self->srt.transl.x = objdata->unk0.unk0.unk68.x;
        self->srt.transl.y = objdata->unk0.unk0.unk68.y;
        self->srt.transl.z = objdata->unk0.unk0.unk68.z;
    } else {
        self->srt.transl.x += self->velocity.x * gUpdateRateF;
        self->srt.transl.y += self->velocity.y * gUpdateRateF;
        self->srt.transl.z += self->velocity.z * gUpdateRateF;
        if ((objdata->flags & LFXEMITTER_HasGravity) && (self->velocity.y > -15.0f)) {
            self->velocity.y += (-0.01f * gUpdateRateF);
        }
    }
    if ((player != NULL) && (((objdata->enableBit == -1)) || (mainGetBits(objdata->enableBit) != 0))) {
        if (objdata->hasLifetimeTimer) {
            objdata->lifetime -= gUpdateRate;
            if (objdata->lifetime <= 0) {
                objFreeObject(self);
            }
        }
        if (!objdata->actionApplied) {
            if ((objdata != NULL) && ((objdata->actionIndex + 1) == sLastAction.id)) {
                // Skip ROM read, use already loaded action since we were requested to do the same action twice in a row
                objdata->action = mmAlloc(sizeof(LightAction), ALLOC_TAG_LFX_COL, NULL);
                if (objdata->action != NULL) {
                    LFXEmitter_copyLightAction(&sLastAction, objdata->action);
                }
            } else {
                objdata->action = mmAlloc(sizeof(LightAction), ALLOC_TAG_LFX_COL, NULL);
                assetRomLoadSection((void**)objdata->action, LACTIONS_BIN, objdata->actionIndex * sizeof(LightAction), sizeof(LightAction));
                if (objdata->action != NULL) {
                    LFXEmitter_copyLightAction(objdata->action, &sLastAction);
                }
            }
            if (objdata->action != NULL) {
                objdata->action->slot = ~1;
                dll_newlfx->DoAction(self, self, objdata->action, 0, 0, NULL);
            }
            objdata->actionApplied = TRUE;
        }
    }
}

// offset: 0x4A8 | func: 2 | export: 2
void LFXEmitter_obj_Update(Object* self) { }

// offset: 0x4B4 | func: 3 | export: 3
void LFXEmitter_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x4CC | func: 4 | export: 4
void LFXEmitter_obj_Free(Object* self, s32 onlySelf) {
    LFXEmitter_Data* objdata = self->data;
    
    if (objdata->action != NULL) {
        objdata->action->flags = 2;
        objdata->action->id = 0;
        objdata->action->type = LACTION_TYPE_NEWLFX;
        dll_newlfx->DoAction(self, self, objdata->action, 0, 0, NULL);
        mmFree(objdata->action);
    }
    objFreeObjectType(self, OBJTYPE_LFXEmitter);
}

// offset: 0x588 | func: 5 | export: 5
u32 LFXEmitter_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x598 | func: 6 | export: 6
u32 LFXEmitter_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(LFXEmitter_Data);
}

// offset: 0x5AC | func: 7
static void LFXEmitter_copyLightAction(LightAction* src, LightAction* dst) {
    s32 i;

    dst->timer = src->timer;
    dst->unk2 = src->unk2;
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    dst->unka = src->unka;
    dst->unkC = src->unkC;
    dst->id = src->id;
    dst->slot = src->slot;
    dst->flags = src->flags;
    dst->flags2 = src->flags2;
    // @bug: no flags3 copy? not used by newlfx anyway i guess?
    dst->type = src->type;
    dst->unk1c = src->unk1c;
    dst->max = src->max;
    dst->min = src->min;
    dst->maxRange = src->maxRange;
    dst->minRange = src->minRange;
    dst->unk21 = src->unk21;
    dst->speed = src->speed;
    for (i = 0; i < 6; i++) {
        ((u8*)&dst->r1)[i] = ((u8*)&src->r1)[i];
        // @bug: this reads and writes out of bounds on the last iteration!
        ((u8*)&dst->unk23)[i] = ((u8*)&src->unk23)[i];
    }
}

// offset: 0x6B8 | func: 8 | export: 7
s32 LFXEmitter_GetActionSlot(Object* self) {
    LFXEmitter_Data* objdata = self->data;

    if (objdata->action != NULL) {
        return objdata->action->slot;
    } else {
        return -1;
    }
}

// offset: 0x6DC | func: 9 | export: 8
s32 LFXEmitter_Func_6DC(Object* self, f32 opacity) {
    LFXEmitter_Data* objdata = self->data;

    if (objdata->action != NULL) {
        gDLL_11_Newlfx->vtbl->Func_BB8(objdata->action->slot, 0, opacity);
        return 1;
    } else {
        return 0;
    }
}
