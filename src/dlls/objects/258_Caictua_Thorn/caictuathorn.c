#include "common.h"
#include "game/objects/object.h"
#include "sys/math.h"

typedef struct {
    ObjSetup base;
} Caictua_Thorn_Setup;

// offset: 0x0 | ctor
void Caictua_Thorn_ctor(void* dll) { }

// offset: 0xC | dtor
void Caictua_Thorn_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void Caictua_Thorn_setup(Object* self, Caictua_Thorn_Setup* objSetup, s32 reset) {
    self->unkDC = 180;
    self->opacity = OBJECT_OPACITY_MAX;
    func_800267A4(self);

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        self->shadow->maxDistScale = self->shadow->scale * 2.5f;
    }

    self->unkE0 = dll_amSfx->Play(self, SOUND_849, MAX_VOLUME, NULL, NULL, 0, NULL);
}

// offset: 0xDC | func: 1 | export: 1
void Caictua_Thorn_control(Object* self) {
    s32 i;

    self->unkDC -= gUpdateRateF;
    if ((self->unkDC < 0) || self->opacity < gUpdateRate) {
        self->opacity = 0;
        objFreeObject(self);
        return;
    }
    
    if (self->opacity && (self->opacity != OBJECT_OPACITY_MAX)) {
        self->opacity -= gUpdateRate;
        return;
    }
    
    //Motion
    {
        //Fall
        self->velocity.y -= 0.03f * gUpdateRateF;

        //Spin
        self->srt.roll += M_5_DEGREES;

        //Set orientation from velocity vector
        self->srt.yaw = mathAtan2f(self->velocity.f[0], self->velocity.f[2]); //doesn't change, so may not need to be updated every tick?
        self->srt.pitch = mathAtan2f(sqrtf(SQ(self->velocity.x) + SQ(self->velocity.z)), self->velocity.y) - M_90_DEGREES;

        //Move
        objMove(self, self->velocity.x * gUpdateRateF, self->velocity.y * gUpdateRateF, self->velocity.z * gUpdateRateF);
    }

    if (self->unkE0 != 0) {
        dll_amSfx->UpdateDoppler(self->unkE0, self, objGetPlayer(), 150.0f);
    }
    
    func_80026128(self, 0xA, 1, 0);
    func_80026940(self, 2);
    func_8002674C(self);

    //Check if the thorn hit the player or the sidekick
    if ((self->objhitInfo->unk48 != NULL) && 
        ((objGetPlayer() == self->objhitInfo->unk48) || (objGetSidekick() == self->objhitInfo->unk48))
    ) {
        camUseShake();
        camSetShakeOffset(1.0f);
        dll_amSfx->Play(self, SOUND_721, MAX_VOLUME, NULL, NULL, 0, NULL);

        i = 0x19;
        while (i--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_343, NULL, 1, -1, NULL);
        }
        
        self->unkDC = 0x78;
        self->opacity = 0;
        func_800267A4(self);

        if (self->unkE0) {
            dll_amSfx->Stop(self->unkE0);
            self->unkE0 = 0;
        }
    }
}

// offset: 0x438 | func: 2 | export: 2
void Caictua_Thorn_update(Object* self) {
    if (self->objhitInfo->unk9D == 0) {
        return;
    }

    //React to collision
    self->srt.transl.x = self->objhitInfo->unk34;
    self->srt.transl.y = self->objhitInfo->unk38;
    self->srt.transl.z = self->objhitInfo->unk3C;
    self->unkDC = 120;
    self->opacity = OBJECT_OPACITY_MAX - 1;
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
    func_800267A4(self);
    dll_amSfx->Play(self, SOUND_722_Impact_Wobble, 0x40, NULL, NULL, 0, NULL);

    //Stop sound loop
    if (self->unkE0 != 0) {
        dll_amSfx->Stop(self->unkE0);
        self->unkE0 = 0;
    }
}

// offset: 0x530 | func: 3 | export: 3
void Caictua_Thorn_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x584 | func: 4 | export: 4
void Caictua_Thorn_free(Object* self, s32 onlySelf) {
    if (self->unkE0 != 0) {
        dll_amSfx->Stop(self->unkE0);
        self->unkE0 = 0;
    }
    
    camIgnoreShake();
}

// offset: 0x600 | func: 5 | export: 5
u32 Caictua_Thorn_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x610 | func: 6 | export: 6
u32 Caictua_Thorn_get_data_size(Object* self, u32 offsetAddr){
    return 0;
}
