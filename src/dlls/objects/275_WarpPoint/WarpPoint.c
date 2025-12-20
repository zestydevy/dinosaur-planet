#include "dll.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"

typedef struct {
/*00*/ s16 counter;
/*02*/ s16 gamebit;
/*04*/ s16 objectSeqIndex;
/*08*/ f32 dist;
/*0C*/ u8 unkC;
} WarpPoint_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 unk19;
/*1A*/ s8 warpID;
/*1B*/ s8 objectSeqIndex;
/*1C*/ s8 unk1C;
/*1D*/ s8 unk1D;
/*1E*/ s8 dist;
/*20*/ s16 gamebit;
} WarpPoint_Setup;

enum Unk1D {
    UNK1D_0,
    UNK1D_1,
    UNK1D_2,
    UNK1D_3,
    UNK1D_4
};

static int WarpPoint_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void WarpPoint_ctor(void *dll) { }

// offset: 0xC | dtor
void WarpPoint_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WarpPoint_setup(Object *self, WarpPoint_Setup *setup, s32 arg2) {
    WarpPoint_Data *objdata;

    objdata = self->data;
    self->animCallback = WarpPoint_anim_callback;
    self->srt.yaw = setup->yaw << 8;
    objdata->counter = 30;
    objdata->dist = setup->dist * 4;
    objdata->gamebit = setup->gamebit;
    objdata->objectSeqIndex = setup->objectSeqIndex;
    if (setup->unk1C != 0) {
        objdata->unkC = 0;
    } else {
        objdata->unkC = 1;
    }
    if (setup->unk1D == UNK1D_2) {
        objdata->counter = 0;
    }
}

// offset: 0xAC | func: 1 | export: 1
void WarpPoint_control(Object *self) {
    Object *player;
    WarpPoint_Setup *setup;
    WarpPoint_Data *objdata;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;

    setup = (WarpPoint_Setup*)self->setup;
    objdata = self->data;
    player = get_player();
    objdata->counter -= gUpdateRate;
    if (objdata->counter < 0) {
        objdata->counter = 0;
    }
    switch (setup->unk1D) {
    case UNK1D_0:
        if (D_800B4A5E >= 0) {
            dx = player->srt.transl.x - self->srt.transl.x;
            dy = player->srt.transl.y - self->srt.transl.y;
            dz = player->srt.transl.z - self->srt.transl.z;
            dist = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
            if ((objdata->unkC == 0) && (setup->unk1C != 0) && (dist < objdata->dist) && (player->parent == self->parent)) {
                gDLL_3_Animation->vtbl->func17(objdata->objectSeqIndex, self, -1);
                D_80092A78 = 2;
                objdata->unkC = 1;
            }
        }
        if ((setup->warpID >= 0) && (vec3_distance(&self->positionMirror, &player->positionMirror) < objdata->dist)) {
            warpPlayer(setup->warpID, TRUE);
        }
        break;
    case UNK1D_1:
        dx = player->srt.transl.x - self->srt.transl.x;
        dy = player->srt.transl.y - self->srt.transl.y;
        dz = player->srt.transl.z - self->srt.transl.z;
        dist = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
        if ((D_800B4A5E >= 0) && (setup->unk1C != 0) && (dist < 100.0f) && (player->parent == self->parent)) {
            gDLL_3_Animation->vtbl->func17(1, self, -1);
            D_80092A78 = 2;
        }
        if ((objdata->counter == 0) && (dist < setup->dist)) {
            if ((setup->warpID >= 0) && (setup->warpID >= 0)) {
                gDLL_3_Animation->vtbl->func17(0, self, -1);
            }
        }
        break;
    case UNK1D_2:
        if (objdata->dist != 0.0f) {
            dx = player->positionMirror.x - self->positionMirror.x;
            dy = player->positionMirror.y - self->positionMirror.y;
            dz = player->positionMirror.z - self->positionMirror.z;
            dist = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
        } else {
            dist = objdata->dist;
        }
        if ((main_get_bits(objdata->gamebit)) && (objdata->unkC == 0) && (setup->unk1C != 0) && (dist <= objdata->dist) && (player->parent == self->parent)) {
            gDLL_3_Animation->vtbl->func17(objdata->objectSeqIndex, self, -1);
            objdata->unkC = 1;
        } else if (objdata->unkC == 1) {
            if ((main_get_bits(objdata->gamebit)) && (objdata->counter == 0) && (dist <= objdata->dist) && (setup->warpID >= 0)) {
                main_set_bits(objdata->gamebit, 0);
                warpPlayer(setup->warpID, FALSE);
            }
        }
        break;
    case UNK1D_3:
        dx = player->srt.transl.x - self->srt.transl.x;
        dy = player->srt.transl.y - self->srt.transl.y;
        dz = player->srt.transl.z - self->srt.transl.z;
        dist = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
        if ((main_get_bits(objdata->gamebit)) && (objdata->unkC == 0) && (setup->unk1C != 0) && (dist < objdata->dist) && (player->parent == self->parent)) {
            main_set_bits(objdata->gamebit, 0);
            gDLL_3_Animation->vtbl->func17(objdata->objectSeqIndex, self, -1);
            objdata->unkC = 1;
        }
        break;
    case UNK1D_4:
        if (objdata->dist != 0.0f) {
            dx = player->positionMirror.x - self->positionMirror.x;
            dy = player->positionMirror.y - self->positionMirror.y;
            dz = player->positionMirror.z - self->positionMirror.z;
            dist = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
        } else {
            dist = objdata->dist;
        }
        if ((D_800B4A5E >= 0) && (objdata->unkC == 0) && (setup->unk1C != 0) && (dist < objdata->dist) && (player->parent == self->parent)) {
            gDLL_3_Animation->vtbl->func17(objdata->objectSeqIndex, self, -1);
            D_80092A78 = 2;
            objdata->unkC = 1;
        }
        if ((main_get_bits(objdata->gamebit)) && (objdata->counter == 0) && (dist <= objdata->dist) && (setup->warpID >= 0)) {
            main_set_bits(objdata->gamebit, 0);
            warpPlayer(setup->warpID, TRUE);
        }
        break;
    }
}

// offset: 0x83C | func: 2 | export: 2
void WarpPoint_update(Object *self) { }

// offset: 0x848 | func: 3 | export: 3
void WarpPoint_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        if (self) { }
    }
}

// offset: 0x868 | func: 4 | export: 4
void WarpPoint_free(Object *self, s32 a1) { }

// offset: 0x878 | func: 5 | export: 5
u32 WarpPoint_get_model_flags(Object *self) {
    return MODFLAGS_1;
}

// offset: 0x888 | func: 6 | export: 6
u32 WarpPoint_get_data_size(Object *self, u32 a1) {
    return sizeof(WarpPoint_Data);
}

// offset: 0x89C | func: 7
int WarpPoint_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    WarpPoint_Setup *setup;

    setup = (WarpPoint_Setup*)self->setup;
    if ((setup->unk1D != UNK1D_2) && (animObjData->unk8D == 1)) {
        if (setup->warpID >= 0) {
            warpPlayer(setup->warpID, 1);
            animObjData->unk8D = 0;
        }
    }
    return 0;
}

/*0x0*/ static const char str_0[] = "\tbit %d  set %d %d %d %d %d %f %f %x\n";
/*0x28*/ static const char str_28[] = "\tdoseq warp %x %d\n";
/*0x3C*/ static const char str_3C[] = "\tdowarp warp %x %d\n";
