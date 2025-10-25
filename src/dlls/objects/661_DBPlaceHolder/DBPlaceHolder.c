#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "types.h"
#include "dll.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/main.h"
#include "functions.h"
#include "dlls/objects/214_animobj.h"

typedef struct {
    u8 flags;
} DBPlaceHolder_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebit1; // egg/diamond bit - 0x3C5, 0x3C6 and 0x3C8 (should be 0x3C7 according to DBlevelControl?)
/*1A*/ s16 gamebit2; // picked up egg bit? always 0x3C4 - checked in DBlevelControl
/*1C*/ u8 yaw;
/*1D*/ u8 flags;
/*1E*/ s8 objectSeqIndex;
/*1F*/ s8 unk1F;
/*20*/ s16 unk20;
/*22*/ u16 unk22;
} DBPlaceHolder_Setup;

static int DBPlaceHolder_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void DBPlaceHolder_ctor(void *dll) { }

// offset: 0xC | dtor
void DBPlaceHolder_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBPlaceHolder_setup(Object *self, DBPlaceHolder_Setup *setup, s32 arg2) {
    DBPlaceHolder_Data *objdata;

    objdata = self->data;
    self->srt.yaw = setup->yaw << 8;
    self->animCallback = (AnimationCallback)DBPlaceHolder_anim_callback;
    objdata->flags = 0;
    if (main_get_bits(setup->gamebit1)) {
        self->unk0xaf |= 8;
        objdata->flags |= 1;
        if (setup->unk20 != 0) {
            objdata->flags |= 0x10;
        }
    }
    if (arg2 != 0) {
        objdata->flags |= 8;
    }
}

// offset: 0xD8 | func: 1 | export: 1
void DBPlaceHolder_control(Object *self) {
    DBPlaceHolder_Data *objdata;
    DBPlaceHolder_Setup *setup;
    Object *player;
    f32 x;
    f32 z;

    setup = (DBPlaceHolder_Setup*)self->setup;
    objdata = self->data;
    if ((setup->unk1F == 1) && !(objdata->flags & 4) && main_get_bits(setup->gamebit2)) {
        obj_add_object_type(self, OBJTYPE_39);
        objdata->flags |= 4;
    }
    if (objdata->flags & 2) {
        main_set_bits(setup->gamebit1, 1);
        self->unk0xaf |= 8;
        objdata->flags &= ~2;
        objdata->flags |= 1;
    }
    if (!(objdata->flags & 1) && (self->unk0xaf & 1) && main_get_bits(setup->gamebit2)) {
        player = get_player();
        x = player->srt.transl.x - self->srt.transl.x;
        z = player->srt.transl.z - self->srt.transl.z;
        self->srt.yaw = arctan2s(x, z);
        self->unk0xaf |= 8;
        objdata->flags |= 2;
        gDLL_3_Animation->vtbl->func17(setup->objectSeqIndex, self, -1);
    }
    if (objdata->flags & 0x10) {
        gDLL_3_Animation->vtbl->func20(self, setup->unk20);
        if (setup->flags & 0x10) {
            gDLL_3_Animation->vtbl->func17(setup->objectSeqIndex, self, setup->unk22);
        } else {
            gDLL_3_Animation->vtbl->func17(setup->objectSeqIndex, self, 1);
        }
        objdata->flags &= ~0x10;
    }
    objdata->flags &= ~8;
}

// offset: 0x338 | func: 2 | export: 2
void DBPlaceHolder_update(Object *self) { }

// offset: 0x344 | func: 3 | export: 3
void DBPlaceHolder_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DBPlaceHolder_Data *objdata;

    objdata = self->data;
    if (visibility) {
        if (objdata->flags & 8) {
            self->srt.scale = 1.0f;
        } else {
            self->srt.scale = 0.00001f;
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3CC | func: 4 | export: 4
void DBPlaceHolder_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_39);
}

// offset: 0x40C | func: 5 | export: 5
u32 DBPlaceHolder_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x41C | func: 6 | export: 6
u32 DBPlaceHolder_get_data_size(Object *self, u32 a1) {
    return sizeof(DBPlaceHolder_Data);
}

// offset: 0x430 | func: 7
int DBPlaceHolder_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    DBPlaceHolder_Setup *setup;

    setup = (DBPlaceHolder_Setup*)self->setup;
    if (animObjData->unk8D == 2) {
        main_set_bits(setup->gamebit1, 1);
    }
    return 0;
}
