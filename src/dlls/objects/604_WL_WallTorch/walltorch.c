#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/object.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "sys/gfx/modgfx.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 mode;
    s16 scale;
    s16 flags;
    s16 gamebit;
} WLWallTorch_Setup;

typedef struct {
    Object* lfxEmitter;
    u32 soundHandle;
    s16 viCheckInterval;
    u8 mode;
    u8 isLit;
    u8 playLitSound;
    u8 wasLit;
    s16 unkE;
    s32 gamebit;
} WLWallTorch_Data;

typedef enum {
    WLWallTorch_MODE_0_Always_Lit,
    WLWallTorch_MODE_1_Lightable
} WLWallTorch_Modes;

typedef enum {
    WLWallTorch_FLAG_1 = 1,
    WLWallTorch_FLAG_2_Alternate_Model = 2,
    WLWallTorch_FLAG_4_Blue_Flame = 4
} WLWallTorch_Flags;

// offset: 0x0 | ctor
void WL_WallTorch_ctor(void* dll) { }

// offset: 0xC | dtor
void WL_WallTorch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_WallTorch_obj_Setup(Object* self, WLWallTorch_Setup* objSetup, s32 reset) {
    DLL_IModgfx* modGfxDLL;
    SRT fxTransform;
    s32 effectIdx;
    WLWallTorch_Data* objData;

    objData = self->data;
    self->srt.yaw = objSetup->yaw << 8 << 2;

    if (objSetup->scale > 0) {
        self->srt.scale = objSetup->scale / 8192.0f;
    } else {
        self->srt.scale = 0.1f;
    }
    
    objData->mode = objSetup->mode;
    objData->gamebit = objSetup->gamebit;
    self->modelInstIdx = (objSetup->flags & WLWallTorch_FLAG_2_Alternate_Model) >> 1;
    fxTransform.transl.y = -2.0f;
    
    if (objData->mode == WLWallTorch_MODE_0_Always_Lit) {
        objData->isLit = TRUE;
        modGfxDLL = dllLoad(DLL_ID_121, 1);

        if (objSetup->flags & WLWallTorch_FLAG_4_Blue_Flame) {
            effectIdx = 2;
        } else if (objSetup->flags & WLWallTorch_FLAG_1) {
            effectIdx = 1;
        } else {
            effectIdx = 0;
        }
        
        modGfxDLL->vtbl->Func0(self, effectIdx, &fxTransform, 0x10004, -1, 0);
        dllFree(modGfxDLL);
    }
    
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x174 | func: 1 | export: 1
void WL_WallTorch_obj_Control(Object* self) {
    WLWallTorch_Data* objData;
    f32 playerDistance;
    LFXEmitter_Setup* lfxSetup;
    s32 i;
    DLL_IModgfx* modGfxDLL;
    SRT fxTransform;

    objData = self->data;
    playerDistance = vec3Distance(&objGetPlayer()->globalPosition, &self->globalPosition);
    
    if (objData->soundHandle == 0) {
        if ((playerDistance < 90.0f) && objData->isLit) {
            dll_amSfx->Play(self, SOUND_1D3_Fire_Crackling_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
        }
    } else if ((playerDistance >= 90.0f) && objData->isLit) {
        dll_amSfx->Stop(objData->soundHandle);
        objData->soundHandle = 0;
    }

    if ((objData->lfxEmitter != NULL) && (objData->lfxEmitter->stateFlags & OBJSTATE_DESTROYED)) {
        objData->lfxEmitter = NULL;
    }

    switch (objData->mode) {
    case WLWallTorch_MODE_0_Always_Lit:
        if ((playerDistance < 75.0f) && (objData->lfxEmitter == NULL)) {
            lfxSetup = objAllocSetup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
            lfxSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
            lfxSetup->base.x = self->srt.transl.x;
            lfxSetup->base.y = self->srt.transl.y;
            lfxSetup->base.z = self->srt.transl.z;
            lfxSetup->unk1E = 140;
            lfxSetup->unk22 = -1;
            objData->lfxEmitter = objSetupObject(&lfxSetup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
        } else if (playerDistance >= 95.0f) {
            if (objData->lfxEmitter != NULL) {
                objFreeObject(objData->lfxEmitter);
                objData->lfxEmitter = NULL;
            }
        }
        break;
    case WLWallTorch_MODE_1_Lightable:
        if (1) {}
        
        fxTransform.transl.y = -2.0f;
        objData->wasLit = objData->isLit;
        
        //Check for sources of damage, and toggle between lit/unlit
        if (func_80025F40(self, NULL, NULL, NULL)) {
            objData->isLit = 1 - objData->isLit;
        }
        
        if (objData->isLit && (objData->viCheckInterval <= 0) && objData->playLitSound) {
            objData->playLitSound = FALSE;
            dll_amSfx->Play(self, SOUND_424_Flame_Lighting, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        
        if (objData->isLit == objData->wasLit) {
            break;
        }
        
        if (objData->isLit) {
            modGfxDLL = dllLoad(DLL_ID_121, 1);
            modGfxDLL->vtbl->Func0(self, 1, &fxTransform, 0x10004, -1, 0);
            dllFree(modGfxDLL);
            
            for (i = 0; i < 100; i++) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A3, NULL, 0, -1, NULL);
            }
            
            if ((objData->gamebit != NO_GAMEBIT) && (mainGetBits(objData->gamebit) == FALSE)) {
                mainSetBits(objData->gamebit, TRUE);
            }
            
            objData->playLitSound = TRUE;
            objData->viCheckInterval = 1;
            if (objData->lfxEmitter == NULL) {
                lfxSetup = objAllocSetup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
                lfxSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
                lfxSetup->base.x = self->srt.transl.x;
                lfxSetup->base.y = self->srt.transl.y;
                lfxSetup->base.z = self->srt.transl.z;
                lfxSetup->unk1E = 0x12A;
                lfxSetup->unk22 = -1;
                objData->lfxEmitter = objSetupObject(&lfxSetup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
                return;
            }
        } else {
            if (objData->soundHandle != 0) {
                dll_amSfx->Stop(objData->soundHandle);
                objData->soundHandle = 0;
            }
            
            dll_modgfx->Func5(self);
            gDLL_13_Expgfx->vtbl->func4(self);
            
            if ((objData->gamebit != NO_GAMEBIT) && (mainGetBits(objData->gamebit))) {
                mainSetBits(objData->gamebit, FALSE);
            }

            if (objData->lfxEmitter != NULL) {
                objFreeObject(objData->lfxEmitter);
                objData->lfxEmitter = NULL;
            }
        }
        break;
    }
}

// offset: 0x6BC | func: 2 | export: 2
void WL_WallTorch_obj_Update(Object* self) { }

// offset: 0x6C8 | func: 3 | export: 3
void WL_WallTorch_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    WLWallTorch_Data* objData;
    f32 pointX;
    f32 pointY;
    f32 pointZ;
    f32 projectedX;
    f32 projectedY;
    f32 projectedZ;
    f32 uViewX;
    f32 uViewY;
    f32 uViewZ;
    s32 screenX;
    s32 screenY;
    s32 zDepth;
    s32 zDepthOcclude;
    SRT fxTransform;

    objData = self->data;

    if (visibility == 0) {
        objData->viCheckInterval = 0;
        return;
    }
    
    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    
    if (objData->isLit == FALSE) {
        return;
    }

    //Check if the torch's flare should be drawn (occlusion depth check)
    pointX = self->srt.transl.x - gWorldX;
    pointY = self->srt.transl.y;
    pointZ = self->srt.transl.z - gWorldZ;
    camProjectPoint(pointX, pointY, pointZ, &projectedX, &projectedY, &projectedZ);
    camClipToScreen(projectedX, projectedY, projectedZ, &screenX, &screenY, NULL);
    zDepthOcclude = viObjDepth(screenX, screenY, self);
    camGetVec3ToCameraNormalized(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &uViewX, &uViewY, &uViewZ);
    camProjectPoint(pointX += (uViewX * 20.0f), pointY += (uViewY * 20.0f), pointZ += (uViewZ * 20.0f), &projectedX, &projectedY, &projectedZ);
    camClipToScreen(projectedX, projectedY, projectedZ, NULL, NULL, &zDepth);

    if (objData->viCheckInterval > 0) {
        objData->viCheckInterval -= gUpdateRate;
        return;
    }
    
    if (viContainsPoint(screenX, screenY) && (zDepth > 0) && (zDepth < zDepthOcclude)) {
        fxTransform.transl.x = 0.0f;
        fxTransform.transl.z = 0.0f;
        fxTransform.transl.y = 13.0f;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1F7, &fxTransform, 0x12, -1, NULL);
    }
    
    objData->viCheckInterval = mathRnd(-10, 10) + 60;
}

// offset: 0x950 | func: 4 | export: 4
void WL_WallTorch_obj_Free(Object* self, s32 onlySelf) {
    WLWallTorch_Data* objData;

    objData = self->data;
    
    if (objData->soundHandle != 0) {
        dll_amSfx->Stop(objData->soundHandle);
    }
    
    if (onlySelf == FALSE && objData->lfxEmitter != NULL) {
        objFreeObject(objData->lfxEmitter);
    }
    
    if (objData->mode == WLWallTorch_MODE_0_Always_Lit) {
        dll_modgfx->Func5(self);
    }
    
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0xA38 | func: 5 | export: 5
s32 WL_WallTorch_obj_GetModelFlags(Object* self) {
    WLWallTorch_Setup* objSetup = (WLWallTorch_Setup*)self->setup;

    return MODFLAGS_LOAD_SINGLE_MODEL | MODFLAGS_1 | ((objSetup->flags & WLWallTorch_FLAG_2_Alternate_Model) << 10);
}

// offset: 0xA54 | func: 6 | export: 6
u32 WL_WallTorch_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WLWallTorch_Data);
}
