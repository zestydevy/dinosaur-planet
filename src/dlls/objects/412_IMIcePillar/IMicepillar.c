#include "common.h"

typedef struct {
    s32 unk0;
} IMIcePillar_Data;

// offset: 0x0 | ctor
void IMIcePillar_ctor(void *dll) { }

// offset: 0xC | dtor
void IMIcePillar_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void IMIcePillar_obj_Setup(Object* self, ObjSetup* setup, s32 reset) { }

// offset: 0x2C | func: 1 | export: 1
void IMIcePillar_obj_Control(Object* self) { }

// offset: 0x38 | func: 2 | export: 2
void IMIcePillar_obj_Update(Object* self) { }

// offset: 0x44 | func: 3 | export: 3
void IMIcePillar_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x98 | func: 4 | export: 4
void IMIcePillar_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0xA8 | func: 5 | export: 5
u32 IMIcePillar_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xB8 | func: 6 | export: 6
u32 IMIcePillar_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(IMIcePillar_Data);
}

