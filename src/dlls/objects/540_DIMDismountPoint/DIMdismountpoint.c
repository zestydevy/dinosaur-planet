#include "common.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
} DIMDismountPoint_Setup;

typedef struct {
    Vec3f direction;
    f32 dotBias;
} DIMDismountPoint_Data;

// offset: 0x0 | ctor
void DIMDismountPoint_ctor(void* dll) { }

// offset: 0xC | dtor
void DIMDismountPoint_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMDismountPoint_obj_Setup(Object* self, DIMDismountPoint_Setup* objSetup, s32 reset) {
    DIMDismountPoint_Data* objData;

    objAddObjectType(self, OBJTYPE_DismountPoint);
    
    self->srt.yaw = objSetup->yaw << 8;
    
    objData = self->data;
    objData->direction.x = mathSinfInterp(self->srt.yaw);
    objData->direction.y = 0.0f;
    objData->direction.z = mathCosfInterp(self->srt.yaw);
    objData->dotBias = -DOT_PRODUCT(objData->direction, self->srt.transl);

    self->stateFlags |= 0xE000;
}

// offset: 0xFC | func: 1 | export: 1
void DIMDismountPoint_obj_Control(Object* self) { }

// offset: 0x108 | func: 2 | export: 2
void DIMDismountPoint_obj_Update(Object* self) { }

// offset: 0x114 | func: 3 | export: 3
void DIMDismountPoint_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x168 | func: 4 | export: 4
void DIMDismountPoint_obj_Free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_DismountPoint);
}

// offset: 0x1A8 | func: 5 | export: 5
u32 DIMDismountPoint_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1B8 | func: 6 | export: 6
u32 DIMDismountPoint_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DIMDismountPoint_Data);
}

// offset: 0x1CC | func: 7 | export: 7
s32 DIMDismountPoint_Func_1CC(Object* self) {
    Object* player;
    DIMDismountPoint_Data* objData;
    s32 objSeqIdx;
    f32 dot;
    
    player = objGetPlayer();
    objData = self->data;

    //Play a different sequence depending on which way the player's facing
    dot = DOT_PRODUCT(objData->direction, player->srt.transl) + objData->dotBias;
    if (dot >= 0) {
        objSeqIdx = 0;
    } else {
        objSeqIdx = 1;
    }
    
    gDLL_3_Animation->vtbl->start_obj_sequence(objSeqIdx, self, -1);
    
    return objSeqIdx;
}

// offset: 0x28C | func: 8 | export: 8
void DIMDismountPoint_Func_28C(Object* self, s32 arg1) {
    gDLL_3_Animation->vtbl->start_obj_sequence((arg1 ^ 1) + 2, self, -1);
}
