#include "PR/ultratypes.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "game/objects/object_id.h"
#include "functions.h"
#include "dlls/objects/793_BWLog.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
/*19*/ s8 spawnLogDisabled;
} DFdockpoint_Setup;

// offset: 0x0 | ctor
void DFdockpoint_ctor(void *dll) { }

// offset: 0xC | dtor
void DFdockpoint_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFdockpoint_setup(Object *self, DFdockpoint_Setup *objsetup, s32 arg2) {
    obj_add_object_type(self, OBJTYPE_23);
    self->srt.yaw = (objsetup->yaw & 0xFF) << 8;
}

// offset: 0x78 | func: 1 | export: 1
void DFdockpoint_control(Object *self) {
    DFdockpoint_Setup *setup;
    BWLog_Setup *logsetup;
    s32 logCount;

    setup = (DFdockpoint_Setup*)self->setup;

    if (!setup->spawnLogDisabled) {
        obj_get_all_of_type(OBJTYPE_11, &logCount);
        if (logCount == 0) {
            logsetup = obj_alloc_create_info(sizeof(BWLog_Setup), OBJ_BWLog);
            logsetup->base.quarterSize = 9;
            logsetup->base.loadParamA = 4;
            logsetup->base.loadDistance = 50;
            logsetup->base.loadParamB = 2;
            logsetup->base.fadeDistance = 45;
            logsetup->base.x = self->srt.transl.x;
            logsetup->base.y = self->srt.transl.y;
            logsetup->base.z = self->srt.transl.z;
            logsetup->yaw = setup->yaw;
            obj_create((ObjSetup*)logsetup, 5, self->mapID, -1, self->parent);
        }
    }
}

// offset: 0x16C | func: 2 | export: 2
void DFdockpoint_update(Object *self) { }

// offset: 0x178 | func: 3 | export: 3
void DFdockpoint_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1CC | func: 4 | export: 4
void DFdockpoint_free(Object *self, s32 arg1) {
    obj_free_object_type(self, OBJTYPE_23);
}

// offset: 0x20C | func: 5 | export: 5
u32 DFdockpoint_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x21C | func: 6 | export: 6
u32 DFdockpoint_get_data_size(Object *self, u32 arg1) {
    return 0;
}
