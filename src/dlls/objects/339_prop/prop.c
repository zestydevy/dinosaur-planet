#include "common.h"

/* NOTE: official DLL name unknown (used by many basic prop objects)
 * The "DFturbinelever" object's DLL appears to have been switched from DLL 331 (genprops)
 * to this DLL when more basic behaviour was required (controlled by sequences), so the name
 * "prop" was chosen here since it seems to be a more general-purpose version of that DLL. 
 */

typedef struct {
    ObjSetup base;
    u8 roll;
    u8 pitch;
    u8 yaw;
    u8 scale;
} Prop_Setup;

// offset: 0x0 | ctor
void dll_339_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_339_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_339_setup(Object* self, Prop_Setup* objSetup, s32 arg2) {
    self->srt.roll = objSetup->roll << 8;
    self->srt.pitch = objSetup->pitch << 8;
    self->srt.yaw = objSetup->yaw << 8;
    
    if (objSetup->scale) {
        self->srt.scale = objSetup->scale / 255.0f;
        if (self->srt.scale == 0.0f) {
            self->srt.scale = 1.0f;
        }
        self->srt.scale *= self->def->scale;
    }
    self->unkB0 |= 0xA000;
}

// offset: 0xD4 | func: 1 | export: 1
void dll_339_control(Object *self) { }

// offset: 0xE0 | func: 2 | export: 2
void dll_339_update(Object *self) { }

// offset: 0xEC | func: 3 | export: 3
void dll_339_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility){
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x140 | func: 4 | export: 4
void dll_339_free(Object *self, s32 a1) { }

// offset: 0x150 | func: 5 | export: 5
u32 dll_339_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x160 | func: 6 | export: 6
u32 dll_339_get_data_size(Object *self, u32 a1){
    return 0;
}
