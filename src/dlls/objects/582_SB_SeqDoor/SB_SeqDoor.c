#include "functions.h"
#include "dlls/objects/214_animobj.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
/*19*/ s8 unk19;
} SB_SeqDoor_Setup;

static int SB_SeqDoor_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void SB_SeqDoor_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_SeqDoor_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_SeqDoor_setup(Object *self, SB_SeqDoor_Setup *setup, s32 arg2) {
    self->animCallback = SB_SeqDoor_anim_callback;
    self->srt.yaw = setup->yaw << 8;
    self->modelInstIdx = setup->unk19 != 0;
}

// offset: 0x54 | func: 1 | export: 1
void SB_SeqDoor_control(Object *self) {
    self->unkAF |= 0x10;
}

// offset: 0x68 | func: 2 | export: 2
void SB_SeqDoor_update(Object *self) { }

// offset: 0x74 | func: 3 | export: 3
void SB_SeqDoor_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xC8 | func: 4 | export: 4
void SB_SeqDoor_free(Object *self, s32 a1) { }

// offset: 0xD8 | func: 5 | export: 5
u32 SB_SeqDoor_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xE8 | func: 6 | export: 6
u32 SB_SeqDoor_get_data_size(Object *self, u32 a1) {
    return 0;
}

// offset: 0xFC | func: 7
int SB_SeqDoor_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    animObjData->unk7A = -2;
    animObjData->unk62 = 0;
    return 0;
}
