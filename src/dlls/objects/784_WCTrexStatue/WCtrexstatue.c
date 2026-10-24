#include "game/objects/setups/51_Generic.h"
#include "common.h"

static int WCTrexStatue_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void WCTrexStatue_ctor(void* dll) { }

// offset: 0xC | dtor
void WCTrexStatue_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCTrexStatue_obj_Setup(Object* self, WCTrexStatue_Setup* objSetup, s32 reset) {
    TextureAnimator* texAnim;

    self->animCallback = WCTrexStatue_animCallback;
    
    self->modelInstIdx = objSetup->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    
    self->srt.yaw = objSetup->yaw << 8;
    
    //Start out in raised position in Act 2 (after RedEye Tooth inserted)
    if ((reset == FALSE) && (gDLL_29_Gplay->vtbl->get_act(self->mapID) == 2)) {
        self->srt.transl.y += 30.0f;
    }
    
    //Animate texture frame
    if (mainGetBits(objSetup->gamebitActivated)) {
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            texAnim->frame = 0x100;
        }
        
        self->unkDC = 1;
    }
}

// offset: 0x114 | func: 1 | export: 1
void WCTrexStatue_obj_Control(Object* self) { }

// offset: 0x120 | func: 2 | export: 2
void WCTrexStatue_obj_Update(Object* self) {
    if ((self->unkDC != 0) && (mathRnd(0, 5) == 0)) {
        if (self->modelInstIdx == 0) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_73F, NULL, 2, -1, NULL);
        } else {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_740, NULL, 2, -1, NULL);
        }
    }
}

// offset: 0x1F4 | func: 3 | export: 3
void WCTrexStatue_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x248 | func: 4 | export: 4
void WCTrexStatue_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x258 | func: 5 | export: 5
s32 WCTrexStatue_obj_GetModelFlags(Object* self) {
    WCTrexStatue_Setup* objSetup = (WCTrexStatue_Setup*)self->setup;
    s8 modelIndex;

    modelIndex = objSetup->modelIdx;
    if (modelIndex >= self->def->numModels) {
        modelIndex = 0;
    }
    
    return MODFLAGS_MODEL_INDEX(modelIndex) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x28C | func: 6 | export: 6
u32 WCTrexStatue_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}

// offset: 0x2A0 | func: 7
int WCTrexStatue_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    TextureAnimator* texAnim;
    s32 i;

    for (i = 0; i < animData->messageCount; i++) {
        if (animData->messages[i] == 1) {
            texAnim = objExprGetTexAnimator(self, 0, 0);
            if (texAnim != NULL) {
                texAnim->frame = 0x100;
            }
            self->unkDC = 1;
        }
    }
    
    return 0;
}
