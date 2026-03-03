#include "common.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s16 particleID;
    s16 particleCount;
} WLWorm_Setup;

typedef struct {
    f32 unk0;           //Value set via objSetup, but unused?
    s16 particleID;     //ID of particles to create
    s16 unused6;
    s16 particleCount;  //Number of particles to create
    s16 unusedA;
    s16 unkC;           //Set to 1 in control when objSetup particleCount is 0
    Vec3f home;
} WLWorm_Data;

/*0x0*/ static u32 _data_0[] = {0, 0, 0, 0};

// offset: 0x0 | ctor
void WLWorm_ctor(void *dll) { }

// offset: 0xC | dtor
void WLWorm_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WLWorm_setup(Object* self, WLWorm_Setup* setup, s32 arg2) {
    WLWorm_Data* objData = self->data;
    
    self->srt.yaw = 0;
    
    objData->unk0 = setup->unk18 * 4;
    objData->particleID = setup->particleID;
    objData->particleCount = setup->particleCount;
    objData->unkC = FALSE;

    if (objData->particleCount <= 0) {
        self->unkDC = objData->particleCount;
    } else {
        self->unkDC = 0;
    }
    
    objData->home.x = self->srt.transl.x;
    objData->home.y = self->srt.transl.y;
    objData->home.z = self->srt.transl.z;
}

// offset: 0x88 | func: 1 | export: 1
void WLWorm_control(Object* self) {
    WLWorm_Data *objData;
    Object *player;
    s16 i;
    Vec3f delta;
    Vec3f vUnused;
    
    objData = self->data;
    player = get_player();
    
    if (!player){
        return;
    }
    
    //Return home when player not near object's initial position
    if (vec3_distance_xz(&player->positionMirror, (Vec3f*)&self->setup->x) > 440.0f){
        self->srt.transl.f[0] = objData->home.f[0];
        self->srt.transl.f[1] = objData->home.f[1];
        self->srt.transl.f[2] = objData->home.f[2];
        return;
    }

    //Get vector towards player (unnormalised)
    delta.f[0] = player->positionMirror.f[0] - self->srt.transl.f[0];
    delta.f[1] = player->positionMirror.f[1] - self->srt.transl.f[1];
    delta.f[2] = player->positionMirror.f[2] - self->srt.transl.f[2];

    //Move towards player
    if ((delta.f[0] > 0.0f) || (delta.f[0] < 0.0f)) {
        self->srt.transl.f[0] += (delta.f[0] * 0.01f) * gUpdateRateF;
    }
    if ((delta.f[1] > 0.0f) || (delta.f[1] < 0.0f)) {
        self->srt.transl.f[1] += (delta.f[1] * 0.01f) * gUpdateRateF;
    }
    if ((delta.f[2] > 0.0f) || (delta.f[2] < 0.0f)){
        self->srt.transl.f[2] += (delta.f[2] * 0.01f) * gUpdateRateF;
    }

    //Unused?
    vUnused.f[0] = (delta.f[0] * 0.01f) * gUpdateRateF;
    vUnused.f[1] = (delta.f[1] * 0.01f) * gUpdateRateF;
    vUnused.f[2] = (delta.f[2] * 0.01f) * gUpdateRateF;

    if ((objData->particleCount >= 0) || 
        ((objData->particleCount < 0) && (self->unkDC <= 0))
    ) {
        if (objData->particleCount == 0) {
            objData->unkC = TRUE;
        }
        
        //Spin and create particles
        self->srt.yaw += 300;
        if (objData->particleCount > 0){
            for (i = 0; i < objData->particleCount; i++){
                gDLL_17_partfx->vtbl->spawn(self, objData->particleID, NULL, 4, -1, NULL);
            }
        } else {
            gDLL_17_partfx->vtbl->spawn(self, objData->particleID, NULL, 4, -1, NULL);
        }
        
        self->unkDC = -objData->particleCount;
    } else if (objData->particleCount < 0){
        if (self->unkDC > 0){
            self->unkDC -= gUpdateRate;
        }
    }
}

// offset: 0x388 | func: 2 | export: 2
void WLWorm_update(Object *self) { }

// offset: 0x394 | func: 3 | export: 3
void WLWorm_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x3AC | func: 4 | export: 4
void WLWorm_free(Object* self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x3F4 | func: 5 | export: 5
u32 WLWorm_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x404 | func: 6 | export: 6
u32 WLWorm_get_data_size(Object* self, u32 a1) {
    return sizeof(WLWorm_Data);
}
