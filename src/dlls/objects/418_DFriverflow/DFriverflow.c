#include "common.h"
#include "macros.h"
#include "sys/objtype.h"

#include "dlls/objects/418_DFriverflow.h"

// offset: 0x0 | ctor
void DFriverflow_ctor(void *dll) { }

// offset: 0xC | dtor
void DFriverflow_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFriverflow_setup(Object* self, DFriverflow_Setup* objSetup, s32 arg2) {
    DFriverflow_Data* objData;

    if (objSetup->toggleGamebit <= 0) {
        obj_add_object_type(self, OBJTYPE_22);
        objData = self->data;
        objData->activated = TRUE;
    }
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.scale = self->def->scale;
    self->srt.scale += (u8)objSetup->range * 0.001953125f;
    if (self->srt.scale < 0.01f) {
        self->srt.scale = 0.01f;
    }
    if (objSetup->flags == 0) {
        objSetup->flags = 0xFF;
    }
}

// offset: 0x108 | func: 1 | export: 1
void DFriverflow_control(Object* self) {
    DFriverflow_Data* objData;
    DFriverflow_Setup* objSetup;

    objSetup = (DFriverflow_Setup*)self->setup;
    if (objSetup->toggleGamebit > 0) {
        objData = self->data;
        if (main_get_bits(objSetup->toggleGamebit)) {
            if (objData->activated) {
                STUBBED_PRINTF("tryint to deactivate the river flow\n");
                objData->activated = FALSE;
                obj_free_object_type(self, OBJTYPE_22);
            }
        } else if (!objData->activated) {
            objData->activated = TRUE;
            obj_add_object_type(self, OBJTYPE_22);
        }
    }
}

// offset: 0x1C0 | func: 2 | export: 2
void DFriverflow_update(Object *self) { }

// offset: 0x1CC | func: 3 | export: 3
void DFriverflow_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1E4 | func: 4 | export: 4
void DFriverflow_free(Object* self, s32 arg1) {
    DFriverflow_Data* objData = self->data;
    if (objData->activated) {
        obj_free_object_type(self, OBJTYPE_22);
    }
}

// offset: 0x234 | func: 5 | export: 5
u32 DFriverflow_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x244 | func: 6 | export: 6
u32 DFriverflow_get_data_size(Object *self, u32 a1){
    return sizeof(DFriverflow_Data);
}
