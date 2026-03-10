#include "common.h"

#include "sys/gfx/modgfx.h"

/*0x0*/ static DLL_Unknown* _data_0 = NULL;

typedef struct {
    ObjSetup base;
    s8 fxRadius;        //Player radius for creating effects (stored quartered)
    s8 modGfxFuncArg1;
    s16 modGfxIndex;    //DLL index in the modgfx bank
    s16 animateRoll;    //Whether to advance object roll while player inside fxRadius 
} WMvein_Setup;

typedef struct {
    f32 fxRadius;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 modGfxFuncArg1; //arg1 for modgfx DLL function call
    s16 modGfxDLLIndex; //DLL index in bank 1
    s16 animateRoll;
    s16 effectCreated;
    s16 rollSpeed;
    s16 unk1E;
    s16 unk20;
    s16 modgfxReturnVal; //return value from modgfx DLL function call, used again when leaving fxRadius
} WMvein_Data;

// offset: 0x0 | ctor
void WMvein_ctor(void *dll) { }

// offset: 0xC | dtor
void WMvein_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WMvein_setup(Object* self, WMvein_Setup* objsetup, s32 arg2) {
    WMvein_Data* objdata;

    objdata = self->data;
    
    self->srt.yaw = 0;
    
    objdata->fxRadius = objsetup->fxRadius * 4;
    objdata->modGfxFuncArg1 = objsetup->modGfxFuncArg1;
    objdata->modGfxDLLIndex = objsetup->modGfxIndex;
    objdata->animateRoll = objsetup->animateRoll;
    objdata->effectCreated = 0;
    objdata->unk4 = 0.0f;
    objdata->unk8 = 0.0f;
    objdata->unkC = 0.0f;
    objdata->unk10 = 0.0f;
    objdata->rollSpeed = rand_next(400, 600);
    objdata->unk1E = 0;
    objdata->unk20 = rand_next(100, 200);
    objdata->modgfxReturnVal = -1;
}

// offset: 0xEC | func: 1 | export: 1
void WMvein_control(Object* self) {
    WMvein_Data* objdata;
    Vec3f delta;
    Object* player;
    DLL_Unknown* modGfxDLL;
    f32 distance;

    objdata = self->data;
    
    player = get_player();
    if (player == NULL) {
        return;
    }
    
    delta.f[0] = self->positionMirror.f[0] - player->positionMirror.f[0];
    delta.f[1] = self->positionMirror.f[1] - player->positionMirror.f[1];
    delta.f[2] = self->positionMirror.f[2] - player->positionMirror.f[2];
    distance = sqrtf((SQ(delta.f[0]) + SQ(delta.f[1]) + SQ(delta.f[2])));

    if (distance <= objdata->fxRadius) {
        if (objdata->effectCreated == FALSE) {

            modGfxDLL = dll_load_deferred(DLL_ID_MODGFX_BASE + objdata->modGfxDLLIndex, 1);
            objdata->modgfxReturnVal = ((DLL_IModgfx*)modGfxDLL)->vtbl->func0(self, objdata->modGfxFuncArg1, 0, 1, -1, 0);
            dll_unload(modGfxDLL);
            objdata->effectCreated = TRUE;
        }
        if (objdata->animateRoll == TRUE) {
            self->srt.roll += objdata->rollSpeed * gUpdateRate;
        }
    } else if ((distance > objdata->fxRadius) && (objdata->effectCreated == 1)) {
        gDLL_14_Modgfx->vtbl->func7(&objdata->modgfxReturnVal);
        objdata->effectCreated = FALSE;
    }
}

// offset: 0x2A4 | func: 2 | export: 2
void WMvein_update(Object *self) { }

// offset: 0x2B0 | func: 3 | export: 3
void WMvein_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x2C8 | func: 4 | export: 4
void WMvein_free(Object* self, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(self);
    gDLL_14_Modgfx->vtbl->func4(self);
}

// offset: 0x338 | func: 5 | export: 5
u32 WMvein_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x348 | func: 6 | export: 6
u32 WMvein_get_data_size(Object *self, u32 a1){
    return sizeof(WMvein_Data);
}
