#include "common.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 spinSpeed;
    s16 rollSpeed;
} IMSpaceRing_Setup;

// offset: 0x0 | ctor
void IMSpaceRing_ctor(void* dll) { }

// offset: 0xC | dtor
void IMSpaceRing_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void IMSpaceRing_obj_Setup(Object* self, IMSpaceRing_Setup* objSetup, s32 reset) {
    self->srt.yaw = objSetup->yaw << 8;
    self->unkDC = mathRnd(0, 1);
}

// offset: 0x7C | func: 1 | export: 1
void IMSpaceRing_obj_Control(Object* self) {
    IMSpaceRing_Setup* objSetup;
    Object* parent;
    f32 dx;
    f32 dy;
    f32 dz;

    objSetup = (IMSpaceRing_Setup*)self->setup;
    
    if (self->unkDC != 0) {
        self->srt.yaw += objSetup->spinSpeed * gUpdateRate;
    } else {
        self->srt.pitch += objSetup->spinSpeed * gUpdateRate;
    }
    
    self->srt.roll += objSetup->rollSpeed * gUpdateRate;
    
    parent = self->unkC4;
    if (parent != NULL) {
        self->opacity = parent->opacity;

        dx = parent->srt.transl.x - self->srt.transl.x;
        dy = parent->srt.transl.y - self->srt.transl.y;
        dz = parent->srt.transl.z - self->srt.transl.z;
        
        objMove(self, dx, dy, dz);
    }
}

// offset: 0x164 | func: 2 | export: 2
void IMSpaceRing_obj_Update(Object* self) { }

// offset: 0x170 | func: 3 | export: 3
void IMSpaceRing_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1C4 | func: 4 | export: 4
void IMSpaceRing_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x1D4 | func: 5 | export: 5
u32 IMSpaceRing_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1E4 | func: 6 | export: 6
u32 IMSpaceRing_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}
