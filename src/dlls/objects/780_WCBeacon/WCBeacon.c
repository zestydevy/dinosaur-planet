#include "game/objects/object.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/objexpr.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "dll.h"

typedef struct {
/*0*/ f32 timer;
/*4*/ u8 state;
} WCBeacon_Data;

enum WCBeaconState {
    WCBEACON_STATE_0_Underground = 0,
    WCBEACON_STATE_1_Unlit = 1,
    WCBEACON_STATE_2_Lighting_Up = 2,
    WCBEACON_STATE_3_Lit = 3
};

#define MAX_FRAME 0x100

// offset: 0x0 | ctor
void WCBeacon_ctor(void* dll) { }

// offset: 0xC | dtor
void WCBeacon_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCBeacon_obj_Setup(Object* self, WCBeacon_Setup* setup, s32 reset) {
    WCBeacon_Data* objdata = self->data;
    TextureAnimator* animatedTexture;
    
    self->srt.yaw = setup->yaw << 8;
    
    //Set model
    self->modelInstIdx = setup->modelIndex;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }

    //Restore state
    if (mainGetBits(setup->gamebitRise)) {
        if (mainGetBits(setup->gamebitLit)) {
            objdata->state = WCBEACON_STATE_3_Lit;
        } else {
            objdata->state = WCBEACON_STATE_1_Unlit;
        }
    }

    //Restore texture frame
    animatedTexture = objExprGetTexAnimator(self, 0, 0);
    if ((animatedTexture != NULL) && (objdata->state == WCBEACON_STATE_3_Lit)) {
        animatedTexture->frame = MAX_FRAME;
    }
}

// offset: 0x11C | func: 1 | export: 1
void WCBeacon_obj_Control(Object* self) {
    WCBeacon_Setup* setup;
    WCBeacon_Data* objdata;
    TextureAnimator* texAnim;
    s16 frameBlend;

    objdata = (WCBeacon_Data*)self->data;
    setup = (WCBeacon_Setup*)self->setup;
    
    if (objdata->state == WCBEACON_STATE_1_Unlit) {
        //Descend if the pressure switch gamebit unsets (player didn't light beacon in time)
        if (mainGetBits(setup->gamebitRise) == FALSE) {
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            objdata->state = WCBEACON_STATE_0_Underground;
        }

        //Become lit when hit by any source of damage (@bug: not just flames!)
        if (func_80025F40(self, NULL, NULL, NULL)) {
            dll_amSfx->Play(self, SOUND_50a_Fire_Burning_Low_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
            dll_amSfx->Play(self, SOUND_50b_Fire_Burning_High_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->state = WCBEACON_STATE_2_Lighting_Up;
            objdata->timer = 0.0f;
        }
    } else if (objdata->state == WCBEACON_STATE_0_Underground) {
        //Rise up when the beacon's corresponding pressure switch is activated
        if (mainGetBits(setup->gamebitRise)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
            objdata->state = WCBEACON_STATE_1_Unlit;
        }
    } else if (objdata->state == WCBEACON_STATE_2_Lighting_Up) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_73A, NULL, PARTFXFLAG_2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_73B, NULL, PARTFXFLAG_2, -1, NULL);

        //Count up until advancing state
        objdata->timer += gUpdateRateF;
        if (objdata->timer >= 90.0f) {
            objdata->state = WCBEACON_STATE_3_Lit;
        }

        //Fade in the lit texture frame
        {
            frameBlend = (objdata->timer / 45.0f) * MAX_FRAME;
            if (frameBlend > MAX_FRAME) {
                frameBlend = MAX_FRAME;
            }

            texAnim = objExprGetTexAnimator(self, 0, 0);
            if (texAnim != NULL) {
                texAnim->frame = frameBlend;
            }
        }
    } else if (objdata->state == WCBEACON_STATE_3_Lit) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_73A, NULL, PARTFXFLAG_2, -1, NULL);
        mainSetBits(setup->gamebitLit, TRUE);

        //Keep texture animator on lit frame
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            texAnim->frame = MAX_FRAME;
        }

        if (self->unkDC == 0) {
            gDLL_3_Animation->vtbl->preempt_sequence_time(self, 105);
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, 1);
        }
    }

    self->unkDC = 1;
}

// offset: 0x4E8 | func: 2 | export: 2
void WCBeacon_obj_Update(Object* self) { }

// offset: 0x4F4 | func: 3 | export: 3
void WCBeacon_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x548 | func: 4 | export: 4
void WCBeacon_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x558 | func: 5 | export: 5
u32 WCBeacon_obj_GetModelFlags(Object* self) {
    WCBeacon_Setup* setup = (WCBeacon_Setup*)self->setup;
    s8 modelIdx;

    modelIdx = setup->modelIndex;
    if (modelIdx >= self->def->numModels) {
        modelIdx = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x58C | func: 6 | export: 6
u32 WCBeacon_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCBeacon_Data);
}
