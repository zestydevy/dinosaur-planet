#include "common.h"
#include "sys/objtype.h"
#include "dlls/objects/211_tricky.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 range;
/*1A*/ s16 gamebit;
} TrickyGuard_Setup;

// offset: 0x0 | ctor
void TrickyGuard_ctor(void *dll) { }

// offset: 0xC | dtor
void TrickyGuard_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void TrickyGuard_setup(Object *self, TrickyGuard_Setup *setup, s32 arg2) {
    self->srt.yaw = setup->yaw << 8;
    obj_add_object_type(self, OBJTYPE_51);
}

// offset: 0x6C | func: 1 | export: 1
void TrickyGuard_control(Object* self) {
    TrickyGuard_Setup *objSetup;
    Object *player;
    Object *tricky;

    objSetup = (TrickyGuard_Setup*)self->setup;
    
    if (objSetup->gamebit != NO_GAMEBIT && !main_get_bits(objSetup->gamebit)) {
        return;
    }

    player = get_player();
    tricky = get_sidekick();
    if (tricky == NULL) {
        return;
    }

    if (((DLL_211_Tricky*)tricky->dll)->vtbl->func25(tricky) == 0) {
        if (vec3_distance_squared(&self->positionMirror, &player->positionMirror) <= SQ(objSetup->range)) {
            ((DLL_211_Tricky*)tricky->dll)->vtbl->base.func14(tricky, 3);
        }
    }
}

// offset: 0x18C | func: 2 | export: 2
void TrickyGuard_update(Object *self) { }

// offset: 0x198 | func: 3 | export: 3
void TrickyGuard_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1B0 | func: 4 | export: 4
void TrickyGuard_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_51);
}

// offset: 0x1F0 | func: 5 | export: 5
u32 TrickyGuard_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x200 | func: 6 | export: 6
u32 TrickyGuard_get_data_size(Object *self, u32 a1) {
    return 0;
}
