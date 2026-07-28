#include "common.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} VFPCorePlat_Setup;

typedef struct {
    s16 unk0;
    s16 unk2;
} VFPCorePlat_Data;

static int VFPCorePlat_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void VFPCorePlat_ctor(void* dll) { }

// offset: 0xC | dtor
void VFPCorePlat_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void VFPCorePlat_obj_Setup(Object* self, VFPCorePlat_Setup* objSetup, s32 reset) {
    VFPCorePlat_Data* objData = self->data;
    
    self->srt.yaw = objSetup->yaw << 8;
    objData->unk0 = objSetup->unk20;
    self->animCallback = VFPCorePlat_animCallback;
    
    if (self->id == OBJ_VFP_coreplathol) {
        if (mainGetBits(BIT_DB_Unlock_Act_Two)) {
            self->srt.scale = self->def->scale * 0.7f;
        }
        if (mainGetBits(BIT_DB_Unlock_Act_Three)) {
            self->srt.scale = self->def->scale * 0.45f;
        }
    }
    
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0xF8 | func: 1 | export: 1
void VFPCorePlat_obj_Control(Object* self) { }

// offset: 0x104 | func: 2 | export: 2
void VFPCorePlat_obj_Update(Object* self) { }

// offset: 0x110 | func: 3 | export: 3
void VFPCorePlat_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0x158 | func: 4 | export: 4
void VFPCorePlat_obj_Free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x1A0 | func: 5 | export: 5
u32 VFPCorePlat_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1B0 | func: 6 | export: 6
u32 VFPCorePlat_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(VFPCorePlat_Data);
}

// offset: 0x1C4 | func: 7
int VFPCorePlat_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    return 0;
}

