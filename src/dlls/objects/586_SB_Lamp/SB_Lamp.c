#include "dll.h"
#include "functions.h"
#include "sys/dll.h"
#include "sys/rand.h"
#include "sys/objects.h"
#include "sys/gfx/gx.h"

typedef struct {
/*00*/ s32 _unk0;
/*04*/ Object *obj;
/*08*/ u8 _unk8[0x1C - 0x8];
/*1C*/ f32 unk1C;
/*20*/ s32 _unk20;
/*24*/ f32 unk24;
/*28*/ u8 unk28[0x2E - 0x28];
/*2E*/ s16 unk2E;
/*30*/ s16 unk30;
/*32*/ u8 _unk32;
/*33*/ u8 unk33;
/*34*/ u8 _pad[0x40 - 0x34];
} SB_Lamp_Data;

/*0x0*/ static void *sDLL107 = NULL;

static int SB_Lamp_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void SB_Lamp_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_Lamp_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_Lamp_setup(Object *self, ObjSetup *setup, s32 arg2) {
    SB_Lamp_Data *objdata;

    objdata = self->data;
    self->srt.yaw = 0;
    self->srt.pitch = 0;
    self->srt.roll = 0;
    self->unkDC = 0;
    self->unkE0 = 0;
    objdata->unk2E = 0;
    objdata->unk30 = 0;
    objdata->unk33 = 1;
    objdata->obj = NULL;
    objdata->unk24 = 600.0f;
    objdata->unk1C = 0.5f;
    self->animCallback = SB_Lamp_anim_callback;
    sDLL107 = dll_load(DLL_ID_107, 1, FALSE);
}

// offset: 0xB8 | func: 1 | export: 1
void SB_Lamp_control(Object *self) {
    if (self->unkE0 == 0) {
        gDLL_3_Animation->vtbl->func17(0, self, -1);
        self->unkE0 = 1;
    }
}

// offset: 0x124 | func: 2 | export: 2
void SB_Lamp_update(Object *self) { }

// offset: 0x130 | func: 3
int SB_Lamp_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    Object *player;
    f32 pdx;
    f32 pdz;
    f32 pdy;
    f32 range1;
    s8 pad[0x1C];
    f32 range2;
    f32 dz;
    SB_Lamp_Data *objdata;
    Camera *camera;
    s8 pad2[0x30];
    f32 dy;
    f32 dx;
    f32 x;
    f32 y;
    f32 z;
    f32 ox1;
    f32 oy1;
    f32 oz1;
    s32 ox;
    s32 oy;
    s32 oz;
    s32 sp30;

    pdx = 0.0f;
    pdz = 0.0f;
    pdy = 0.0f;
    objdata = self->data;
    camera = get_camera();
    if (rand_next(0, 1) != 0) {
        animObjData->unk9D = 4;
    } else {
        animObjData->unk9D = 8;
    }
    animObjData->unk7A = -1;
    animObjData->unk62 = 0;
    animObjData->unk7A &= ~0x20;
    player = get_player();
    if (!player) {
        return 0;
    }
    func_80025F40(self, NULL, NULL, NULL);
    pdx = player->positionMirror.x - self->positionMirror.x;
    pdz = player->positionMirror.z - self->positionMirror.z;
    pdy = player->positionMirror.y - self->positionMirror.y;
    range1 = SQ(pdx) + SQ(pdz) + SQ(pdy);
    if (range1 != 0.0f) {
        range1 = sqrtf(range1);
    }
    dx = camera->srt.transl.x - self->positionMirror.x;
    dz = camera->srt.transl.z - self->positionMirror.z;
    dy = camera->srt.transl.y - self->positionMirror.y;
    range2 = SQ(dx) + SQ(dz) + SQ(dy);
    if (range2 != 0.0f) {
        range2 = sqrtf(range2);
    }
    if ((range1 < 75.0f) && (objdata->unk33 == 1)) {
        objdata->unk33 = 0;
        func_80000450(self, self, 92, 0, 0, 0);
    } else if ((range1 > 75.0f) && (objdata->unk33 == 0)) {
        objdata->unk33 = 1;
        func_80000450(self, self, 93, 0, 0, 0);
    }
    if ((self->opacity > 128) && (range2 < 400.0f)) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_8C, NULL, PARTFXFLAG_10000 | PARTFXFLAG_2, -1, NULL);
    }
    x = self->positionMirror.x - gWorldX;
    z = self->positionMirror.z - gWorldZ;
    y = self->positionMirror.y;
    camera_project_point(x, y, z, &ox1, &oy1, &oz1);
    camera_clip_to_screen(ox1, oy1, oz1, &ox, &oy, NULL);
    sp30 = vi_func_8005DD4C(ox, oy, (s32)self);
    get_vec3_to_camera_normalized(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &pdx, &pdy, &pdz);
    x += (pdx * 20.0f);
    y += (pdy * 20.0f);
    z += (pdz * 20.0f);
    camera_project_point(x, y, z, &ox1, &oy1, &oz1);
    camera_clip_to_screen(ox1, oy1, oz1, NULL, NULL, &oz);
    if ((vi_contains_point(ox, oy)) && (oz > 0) && (oz < sp30)) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_8D, NULL, PARTFXFLAG_10000 | PARTFXFLAG_2, -1, NULL);
    }
    return 0;
}

// offset: 0x5E4 | func: 4 | export: 3
void SB_Lamp_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x638 | func: 5 | export: 4
void SB_Lamp_free(Object *self, s32 a1) {
    SB_Lamp_Data *objdata;

    objdata = self->data;
    gDLL_13_Expgfx->vtbl->func5(self);
    if (objdata->obj) {
        obj_destroy_object(objdata->obj);
    }
    objdata->obj = NULL;
    dll_unload(sDLL107);
}

// offset: 0x6C8 | func: 6 | export: 5
u32 SB_Lamp_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x6D8 | func: 7 | export: 6
u32 SB_Lamp_get_data_size(Object *self, u32 a1) {
    return sizeof(SB_Lamp_Data);
}
