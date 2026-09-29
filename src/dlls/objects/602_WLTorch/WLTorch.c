#include "common.h"
#include "sys/gfx/modgfx.h"

typedef struct {
    ObjSetup base;
    u8 _unk18;
    s8 mode;
    s16 unk1A;
    s16 unk1C;
} WLTorch_Setup;

typedef struct {
    Object* unk0;
    f32 unk4;
    u32 soundHandle;
    s16 viCheckInterval;
    s16 unkE;
    u8 mode; 
    u8 _unk11;
    s16 _unk12; 
} WLTorch_Data;

typedef enum {
    WLTorch_MODE_0 = 0,
    WLTorch_MODE_2_Spin = 2,
    WLTorch_MODE_7F_Blue = 0x7F
} WLTorch_Modes;

// offset: 0x0 | ctor
void WLTorch_ctor(void* dll) { }

// offset: 0xC | dtor
void WLTorch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WLTorch_obj_Setup(Object* self, WLTorch_Setup* setup, s32 reset) {
    DLL_IModgfx* modGfxDLL;
    SRT fxTransform;
    WLTorch_Data* objdata;

    objdata = self->data;

    if (setup->unk1A != 0) {
        objdata->unk4 = setup->unk1A;
    } else {
        objdata->unk4 = 75.0f;
    }

    if (setup->unk1C != 0) {
        objdata->unkE = setup->unk1C;
    } else {
        objdata->unkE = 140;
    }

    objdata->mode = setup->mode;

    fxTransform.transl.y = -2.0f;
    if (objdata->mode == WLTorch_MODE_0) {
        modGfxDLL = dllLoad(DLL_ID_121, 1);
        self->srt.scale *= 0.5f;
        modGfxDLL->vtbl->func0(self, 1, &fxTransform, 0x10004, -1, 0);
    } else if (objdata->mode == WLTorch_MODE_7F_Blue) {
        modGfxDLL = dllLoad(DLL_ID_121, 1);
        self->srt.scale *= 0.5f;
        modGfxDLL->vtbl->func0(self, 2, &fxTransform, 0x10004, -1, 0);
    } else {
        modGfxDLL = dllLoad(DLL_ID_115, 1);
        self->srt.scale *= 0.5f;
        modGfxDLL->vtbl->func0(self, 2, &fxTransform, 0x10004, -1, 0);
    }

    self->srt.scale *= 2.0f;

    dllFree(modGfxDLL);

    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x224 | func: 1 | export: 1
void WLTorch_obj_Control(Object* self) {
    WLTorch_Data* objdata;
    f32 playerDistance;
    SRT fxTransform;
    s32 screenX;
    s32 screenY;
    s32 zDepth;
    s32 zDepthOcclude;
    f32 projectedX;
    f32 projectedY;
    f32 projectedZ;
    f32 pointX;
    f32 pointY;
    f32 pointZ;
    f32 uViewX;
    f32 uViewY;
    f32 uViewZ;

    objdata = self->data;

    if (objdata->mode == WLTorch_MODE_2_Spin) {
        self->srt.yaw += 50;
    }

    playerDistance = vec3Distance(&objGetPlayer()->globalPosition, &self->globalPosition);

    if (objdata->soundHandle == 0) {
        if (playerDistance < 90.0f) {
            dll_amSfx->Play(self, SOUND_1D3_Fire_Crackling_Loop, MAX_VOLUME, &objdata->soundHandle, NULL, 0, NULL);
        }
    } else if (playerDistance >= 90.0f) {
        dll_amSfx->Stop(objdata->soundHandle);
        objdata->soundHandle = 0;
    }
    
    //Check if the torch's flare should be drawn (occlusion depth check)
    if (objdata->mode != WLTorch_MODE_2_Spin) {
        pointX = self->srt.transl.x - gWorldX;
        pointY = self->srt.transl.y;
        pointZ = self->srt.transl.z - gWorldZ;
        camProjectPoint(pointX, pointY, pointZ, &projectedX, &projectedY, &projectedZ);
        camClipToScreen(projectedX, projectedY, projectedZ, &screenX, &screenY, NULL);
        zDepthOcclude = viObjDepth(screenX, screenY, self);
        camGetVec3ToCameraNormalized(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &uViewX, &uViewY, &uViewZ);
        camProjectPoint(pointX += (uViewX * 20.0f), pointY += (uViewY * 20.0f), pointZ += (uViewZ * 20.0f), &projectedX, &projectedY, &projectedZ);
        camClipToScreen(projectedX, projectedY, projectedZ, NULL, NULL, &zDepth);

        if (objdata->viCheckInterval > 0) {
           objdata->viCheckInterval -= gUpdateRate;
        } else {
            if ((viContainsPoint(screenX, screenY)) && (zDepth > 0) && (zDepth < zDepthOcclude)) {
                fxTransform.transl.x = 0.0f;
                fxTransform.transl.z = 0.0f;
                fxTransform.transl.y = 13.0f;
                if (objdata->mode == WLTorch_MODE_7F_Blue) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_741, &fxTransform, PARTFXFLAG_10 | PARTFXFLAG_2, -1, NULL);
                } else {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1F7, &fxTransform, PARTFXFLAG_10 | PARTFXFLAG_2, -1, NULL);
                }
            }

            objdata->viCheckInterval = 60;
        }
    }
}

// offset: 0x594 | func: 2 | export: 2
void WLTorch_obj_Update(Object* self) { }

// offset: 0x5A0 | func: 3 | export: 3
void WLTorch_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x5B8 | func: 4 | export: 4
void WLTorch_obj_Free(Object* self, s32 onlySelf) {
    WLTorch_Data* objdata = self->data;

    if (objdata->soundHandle != 0) {
        dll_amSfx->Stop(objdata->soundHandle);
    }

    if (onlySelf == FALSE) {
        if (objdata->unk0 != NULL) {
            objFreeObject(objdata->unk0);
        }
    }

    dll_modgfx->Func5(self);
    gDLL_13_Expgfx->vtbl->func4(self);
}


// offset: 0x68C | func: 5 | export: 5
s32 WLTorch_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x69C | func: 6 | export: 6
u32 WLTorch_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WLTorch_Data);
}
