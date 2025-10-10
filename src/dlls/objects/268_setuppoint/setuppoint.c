#include "PR/ultratypes.h"
#include "functions.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ ObjCreateInfo base;
/*18*/ s8 unk18; //unused?
/*19*/ u8 unk19; //assigned to Object->unk_0xe0
/*1A*/ u8 yaw;
} SetupPointCreateInfo;

// offset: 0x0 | ctor
void setuppoint_ctor(void *dll){
}

// offset: 0xC | dtor
void setuppoint_dtor(void *dll){
}

// offset: 0x18 | func: 0 | export: 0
void setuppoint_create(Object* self, SetupPointCreateInfo* createInfo, s32 arg2) {
    createInfo->base.loadParamA = 8;
    createInfo->base.loadDistance = 0;
    createInfo->base.fadeDistance = 0;
    self->unk_0xe0 = createInfo->unk19;
    self->srt.yaw = (createInfo->yaw & 0x3F) << 0xA;
    self->unk0xb0 |= 0x2000;
}

// offset: 0x58 | func: 1 | export: 1
void setuppoint_update(Object *self){
    
}

// offset: 0x64 | func: 2 | export: 2
void setuppoint_func_64(Object *self){
    
}

// offset: 0x70 | func: 3 | export: 3
void setuppoint_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xC4 | func: 4 | export: 4
void setuppoint_destroy(Object *self, s32 arg1){
}

// offset: 0xD4 | func: 5 | export: 5
u32 setuppoint_get_model_flags(Object *self){
    return MODFLAGS_NONE;
}

// offset: 0xE4 | func: 6 | export: 6
u32 setuppoint_get_state_size(Object *self, s32 arg1){
    return 0;
}
