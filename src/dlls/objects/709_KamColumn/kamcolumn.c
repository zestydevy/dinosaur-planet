#include "dll.h"
#include "game/gamebits.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objlib.h"
#include "sys/objprint.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    u8 yaw;
} KamColumn_Setup;

typedef struct {
    s16 yPhaseAngle;
    s16 yBase;
    u32 soundHandle;
} KamColumn_Data;

// offset: 0x0 | ctor
void KamColumn_ctor(void* dll) { }

// offset: 0xC | dtor
void KamColumn_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void KamColumn_obj_Setup(Object* self, KamColumn_Setup* objSetup, s32 reset) {
    KamColumn_Data* objData = self->data;

    self->srt.yaw = objSetup->yaw << 8;
    objData->yPhaseAngle = -M_90_DEGREES;
    objData->yBase = self->srt.transl.y;
    self->srt.transl.y -= 200.0f;
}

// offset: 0x70 | func: 1 | export: 1
void KamColumn_obj_Control(Object* self) {
    f32 distance;
    Object* boss;
    KamColumn_Data* objData;
    f32 temp;
    f32 yDiff;
    s32 gamebit;

    distance = 10000000;
    boss = objFindClosestObject(self, OBJ_KamerianBoss, &distance);
    objData = self->data;

    gamebit = (self->globalPosition.x < boss->globalPosition.x) ? BIT_7E4 : BIT_7E3;

    if (mainGetBits(gamebit)) {
        objData->yPhaseAngle += gUpdateRate * 0x60;
        if (objData->yPhaseAngle > M_90_DEGREES) {
            objData->yPhaseAngle = M_90_DEGREES;
        }
    } else {
        objData->yPhaseAngle -= gUpdateRate << 5;
        if (objData->yPhaseAngle < -M_90_DEGREES) {
            objData->yPhaseAngle = -M_90_DEGREES;
        }
    }

    yDiff = self->srt.transl.y;    
    self->srt.transl.y = (((Sinf(objData->yPhaseAngle) * 0.5f) - 0.5f) * 200.0f) + objData->yBase;

    yDiff = self->srt.transl.y - yDiff;
    if (yDiff < 0.0f) {
        yDiff = -yDiff;
    }    
    if (yDiff != 0.0f) {
        if (objData->soundHandle == 0) {
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_9A7, (s32) (yDiff * 23.0f), &objData->soundHandle, NULL, 0, NULL);
        } else {
            f32 temp;
            gDLL_6_AMSFX->vtbl->SetVol(objData->soundHandle, (s32) (yDiff * 23.0f) );
        }
        return;
    }

    if (objData->soundHandle) {
        gDLL_6_AMSFX->vtbl->Stop(objData->soundHandle);
        objData->soundHandle = 0;
    }
}

// offset: 0x2D0 | func: 2 | export: 2
void KamColumn_obj_Update(Object* self) { }

// offset: 0x2DC | func: 3 | export: 3
void KamColumn_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x330 | func: 4 | export: 4
void KamColumn_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x340 | func: 5 | export: 5
u32 KamColumn_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x350 | func: 6 | export: 6
u32 KamColumn_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(KamColumn_Data);
}
