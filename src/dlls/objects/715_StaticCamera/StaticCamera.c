#include "common.h"
#include "dlls/objects/715_StaticCamera.h"
#include "sys/objtype.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    f32 fov;
} StaticCamera_Data;

// offset: 0x0 | ctor
void StaticCamera_ctor(void* dll) { }

// offset: 0xC | dtor
void StaticCamera_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void StaticCamera_obj_Setup(Object* self, StaticCamera_Setup* setup, s32 reset) {
    StaticCamera_Data* objdata = self->data;

    self->srt.yaw = -setup->yaw;
    self->srt.pitch = -setup->pitch;
    self->srt.roll = -setup->roll;

    objdata->unk0 = setup->unk19;
    objdata->fov = setup->fov;
    objdata->unk1 = 0;

    if (reset == FALSE) {
        objAddObjectType(self, OBJTYPE_StaticCamera);
    }
}

// offset: 0xB0 | func: 1 | export: 1
void StaticCamera_obj_Control(Object* self) { }

// offset: 0xBC | func: 2 | export: 2
void StaticCamera_obj_Update(Object* self) { }

// offset: 0xC8 | func: 3 | export: 3
void StaticCamera_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x11C | func: 4 | export: 4
void StaticCamera_obj_Free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_StaticCamera);
}

// offset: 0x15C | func: 5 | export: 5
u32 StaticCamera_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x16C | func: 6 | export: 6
u32 StaticCamera_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(StaticCamera_Data);
}
