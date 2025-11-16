#include "dll.h"
#include "functions.h"
#include "sys/objects.h"
#include "game/objects/object_id.h"
#include "game/gamebits.h"
#include "dlls/objects/210_player.h"

// offset: 0x0 | ctor
void dll_762_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_762_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_762_setup(Object *self, ObjSetup *setup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void dll_762_control(Object *self) {
    Object *player;
    Object *obj;

    player = get_player();
    obj = ((DLL_210_Player*)player->dll)->vtbl->get_vehicle(player);
    if ((obj) && (obj->id == OBJ_DR_CloudRunner)) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 9, 1);
    } else {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 9, 0);
        if ((!main_get_bits(BIT_7C8)) && (main_get_bits(BIT_667)) && (vec3_distance(&player->positionMirror, &self->positionMirror) < 100.0f)) {
            main_set_bits(BIT_7C7, 1);
        }
    }
}

// offset: 0x174 | func: 2 | export: 2
void dll_762_update(Object *self) { }

// offset: 0x180 | func: 3 | export: 3
void dll_762_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1D4 | func: 4 | export: 4
void dll_762_free(Object *self, s32 a1) { }

// offset: 0x1E4 | func: 5 | export: 5
u32 dll_762_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1F4 | func: 6 | export: 6
u32 dll_762_get_data_size(Object *self, u32 a1) {
    return 4;
}
