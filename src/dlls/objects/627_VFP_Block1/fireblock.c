#include "dll.h"
#include "game/objects/setups/51_Generic.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objhits.h"

typedef struct {
    s16 gamebit;
    u32 soundHandle;
} VFP_Block1_Data;

// offset: 0x0 | ctor
void VFP_Block1_ctor(void* dll) { }

// offset: 0xC | dtor
void VFP_Block1_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void VFP_Block1_obj_Setup(Object* self, VFP_Block1_Setup* objSetup, s32 reset) {
    VFP_Block1_Data* objData = self->data;

    self->srt.yaw = objSetup->yaw << 8;
    objData->gamebit = objSetup->gamebit;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED;
}

// offset: 0x48 | func: 1 | export: 1
void VFP_Block1_obj_Control(Object* self) {
    VFP_Block1_Data* objData;
    f32 distance;
    Object* hitBy;
    SRT fxTransform;

    objData = self->data;

    hitBy = NULL;
    if (mainGetBits(objData->gamebit) == FALSE) {
        if (func_80025F40(self, &hitBy, NULL, NULL) && (hitBy != NULL) && (hitBy->id == OBJ_projball)) {
            mainSetBits(objData->gamebit, TRUE);
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_424_Flame_Lighting, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        return;
    }

    fxTransform.yaw = 0;
    fxTransform.pitch = 0;
    fxTransform.roll = 0;
    fxTransform.transl.f[2] = 0.0f;
    fxTransform.transl.f[1] = 8.0f;
    fxTransform.transl.f[0] = 20.0f;
    fxTransform.scale = 1.0f;
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3A5, &fxTransform, 4, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3A6, &fxTransform, 4, -1, NULL);

    distance = vec3Distance(&objGetPlayer()->globalPosition, &self->globalPosition); 
    if (objData->soundHandle == 0) {
        if (distance < 90.0f) {
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_1D3_Fire_Crackling_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
        }
    } else {
        if (distance >= 90.0f) {
            gDLL_6_AMSFX->vtbl->Stop(objData->soundHandle);
            objData->soundHandle = 0;
        }
    }
}

// offset: 0x2A0 | func: 2 | export: 2
void VFP_Block1_obj_Update(Object* self) { }

// offset: 0x2AC | func: 3 | export: 3
void VFP_Block1_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x2C4 | func: 4 | export: 4
void VFP_Block1_obj_Free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x30C | func: 5 | export: 5
u32 VFP_Block1_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x31C | func: 6 | export: 6
u32 VFP_Block1_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(VFP_Block1_Data);
}
