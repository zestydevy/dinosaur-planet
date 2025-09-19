#include "PR/ultratypes.h"
#include "functions.h"
#include "game/objects/object.h"

// offset: 0x0 | ctor
void area_ctor(void *dll){
}

// offset: 0xC | dtor
void area_dtor(void *dll){
}

// offset: 0x18 | func: 0 | export: 0
void area_create(Object* self, s32 arg1) {
    self->unk0xb0 |= 0xA000;
}

// offset: 0x30 | func: 1 | export: 1
void area_update(Object *self){  
}

// offset: 0x3C | func: 2 | export: 2
void area_func_3C(Object *self){  
}

// offset: 0x48 | func: 3 | export: 3
void area_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
}

// offset: 0x60 | func: 4 | export: 4
void area_destroy(Object *self, s32 arg1){  
}

// offset: 0x70 | func: 5 | export: 5
s32 area_func_70(Object *self){  
    return 0;
}

// offset: 0x80 | func: 6 | export: 6
s32 area_get_state_size(Object *self, s32 arg1){  
    return 0;
}
