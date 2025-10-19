#include "PR/ultratypes.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/main.h"
#include "functions.h"
#include "dlls/objects/211_tricky.h"

typedef struct {
/*0x00*/ ObjSetup base;
/*0x18*/ u8 yaw;
/*0x1A*/ s16 range;
/*0x1C*/ s8 unk1C[0x20 - 0x1C];
/*0x20*/ s16 gamebit;
} TrickyWarp_Setup;

// offset: 0x0 | ctor
void TrickyWarp_ctor(void *dll) { }

// offset: 0xC | dtor
void TrickyWarp_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void TrickyWarp_setup(Object *self, TrickyWarp_Setup *setup, s32 arg2) {
    self->srt.yaw = setup->yaw << 8;
    obj_add_object_type(self, OBJTYPE_51);
}

// offset: 0x6C | func: 1 | export: 1
void TrickyWarp_control(Object *self) {
    TrickyWarp_Setup *setup;
    Object *player, *sidekick;

    setup = (TrickyWarp_Setup*)self->setup;

    if (setup->gamebit == -1 || main_get_bits(setup->gamebit)) {
        player = get_player();
        sidekick = get_sidekick();

        if (sidekick && vec3_distance_squared(&self->positionMirror, &player->positionMirror) <= SQ(setup->range)) {
            ((DLL_211_Tricky*)(sidekick->dll))->vtbl->base.func14(sidekick, 1);
        }
    }
}

// offset: 0x164 | func: 2 | export: 2
void TrickyWarp_update(Object *self) { }

// offset: 0x170 | func: 3 | export: 3
void TrickyWarp_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1C4 | func: 4 | export: 4
void TrickyWarp_free(Object *self, s32 arg1) {
    obj_free_object_type(self, OBJTYPE_51);
}

// offset: 0x204 | func: 5 | export: 5
u32 TrickyWarp_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x214 | func: 6 | export: 6
u32 TrickyWarp_get_data_size(Object *self, u32 arg1) {
    return 0;
}
