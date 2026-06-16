#include "common.h"
#include "sys/gfx/model.h"
#include "sys/objtype.h"
#include "sys/gfx/modgfx.h"

// offset: 0x0 | ctor
void FirePole_ctor(void *dll) { }

// offset: 0xC | dtor
void FirePole_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 scale; //0x2000 is 1x scale
    s16 roll;
} FirePole_Setup;

void FirePole_setup(Object* self, FirePole_Setup* setup, s32 reset) {
    s32 objID;
    DLL_IModgfx* modGfxDLL;
    SRT fxTransform;

    objID = setup->base.objId;
    obj_add_object_type(self, OBJTYPE_Torch);
    if (objID == OBJ_FirePole) {
        self->srt.yaw = setup->yaw << 8;
        self->srt.pitch = 0;
        self->srt.roll = setup->roll;
        if (setup->scale > 0) {
            self->srt.scale = setup->scale * (1.0f/0x2000);
        } else {
            self->srt.scale = 0.1f;
        }

        fxTransform.transl.y = 2.0f;

        modGfxDLL = dll_load_deferred(DLL_ID_121, 1);
        modGfxDLL->vtbl->func0(self, 0, &fxTransform, 0x10004, -1, 0);
        dll_unload(modGfxDLL);
    }
}

// offset: 0x13C | func: 1 | export: 1
void FirePole_control() {
}

// offset: 0x144 | func: 2 | export: 2
void FirePole_update(Object *self) { }

// offset: 0x150 | func: 3 | export: 3
void FirePole_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1A4 | func: 4 | export: 4
void FirePole_free(Object* self, s32 onlySelf) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
    obj_free_object_type(self, OBJTYPE_Torch);
}

// offset: 0x22C | func: 5 | export: 5
u32 FirePole_get_model_flags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x23C | func: 6 | export: 6
u32 FirePole_get_data_size(Object *self, u32 offsetAddr) {
    return 0;
}
