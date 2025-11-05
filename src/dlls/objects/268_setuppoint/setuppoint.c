#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "functions.h"
#include "game/objects/object.h"
#include "types.h"
#include "sys/gfx/model.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18; //unused?
/*19*/ u8 unk19; //assigned to Object->unkE0
/*1A*/ u8 yaw;
} SetupPoint_Setup;

// offset: 0x0 | ctor
void setuppoint_ctor(void *dll){ }

// offset: 0xC | dtor
void setuppoint_dtor(void *dll){ }

// offset: 0x18 | func: 0 | export: 0
void setuppoint_setup(Object* self, SetupPoint_Setup* objsetup, s32 arg2) {
    objsetup->base.loadFlags = OBJSETUP_LOAD_FLAG8;
    objsetup->base.loadDistance = 0;
    objsetup->base.fadeDistance = 0;
    self->unkE0 = objsetup->unk19;
    self->srt.yaw = (objsetup->yaw & 0x3F) << 0xA;
    self->unkB0 |= 0x2000;
}

// offset: 0x58 | func: 1 | export: 1
void setuppoint_control(Object *self){
    
}

// offset: 0x64 | func: 2 | export: 2
void setuppoint_update(Object *self){
    
}

// offset: 0x70 | func: 3 | export: 3
void setuppoint_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xC4 | func: 4 | export: 4
void setuppoint_free(Object *self, s32 arg1){ }

// offset: 0xD4 | func: 5 | export: 5
u32 setuppoint_get_model_flags(Object *self){
    return MODFLAGS_NONE;
}

// offset: 0xE4 | func: 6 | export: 6
u32 setuppoint_get_data_size(Object *self, s32 arg1){
    return 0;
}
