#include "common.h"
#include "sys/objtype.h"
#include "sys/segment_1050.h"

typedef struct{
    s32 _unk0;
    Object* unk4;
    s32 _unk8[5];
    f32 unk1C;
    s32 _unk20;
    f32 unk24;
    s32 _unk28;
    s16 _unk2C;
    s16 unk2E;
    s16 unk30;
    s8 _unk32;
    u8 unk33;
    s32 _unk34[3];
} CFLamp_Data;

typedef struct{
    ObjSetup base;
    s16 _unk18;
    s16 unk1A;
} CFLamp_Setup;

static int dll_405_func_FC(Object*, Object*, AnimObj_Data*, s8);

// offset: 0x0 | ctor
void dll_405_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_405_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_405_setup(Object* self, ObjSetup* setup, s32 arg2) {
    CFLamp_Data* objdata;

    objdata = self->data;
    self->srt.yaw = 0;
    self->srt.pitch = 0;
    self->srt.roll = 0;
    self->unkDC = 0;
    self->unkE0 = 0;
    objdata->unk2E = 0;
    objdata->unk30 = 0;
    objdata->unk33 = 1;
    objdata->unk4 = 0;
    objdata->unk24 = 600.0f;
    objdata->unk1C = 0.5f;
    self->animCallback = dll_405_func_FC;
}

// offset: 0x84 | func: 1 | export: 1
void dll_405_control(Object* self) {
    if (self->unkE0 == 0) {
        gDLL_3_Animation->vtbl->func17(0, self, -1);
        self->unkE0 = 1;
    }
}

// offset: 0xF0 | func: 2 | export: 2
void dll_405_update(Object *self) { }

// offset: 0xFC | func: 3
static int dll_405_func_FC(Object* self, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    f32 tempx;
    f32 tempy;
    f32 tempz;
    f32 sp60;
    CFLamp_Data* objdata;
    Camera* camera;
    CFLamp_Setup* setup;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    
    sp7C = 0.0f;
    sp78 = 0.0f;
    sp74 = 0.0f;
    sp70 = 0.0f;
    objdata = self->data;
    camera = get_camera();
    setup = (CFLamp_Setup*)self->setup;
    if (rand_next(0, 1) != 0) {
        arg2->unk9D = 4;
    } else {
        arg2->unk9D = 8;
    }
    arg2->unk7A = -1;
    arg2->unk62 = 0;
    arg2->unk7A &= ~0x20;
    sp70 = (f32) setup->unk1A;
    tempx = camera->srt.transl.x - self->globalPosition.x;
    tempz = camera->srt.transl.z - self->globalPosition.z;
    tempy = camera->srt.transl.y - self->globalPosition.y;
    sp60 = SQ(tempx) + SQ(tempz) + SQ(tempy);
    if (sp60 != 0) { 
        sp60 = sqrtf(sp60);
    }
    obj_get_nearest_type_to(OBJTYPE_56, self, &sp70);
    if ((sp70 < setup->unk1A) && (objdata->unk33 == 1)) {
        objdata->unk33 = 0;
        func_80000450(self, self, 0x5C, 0, 0, 0);
    } else if ((setup->unk1A < sp70) && (objdata->unk33 == 0)) {
        objdata->unk33 = 1;
        func_80000450(self, self, 0x5D, 0, 0, 0);
    }
    if ((self->opacity >= 0x81) && (sp60 < 400.0f)) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_8C, NULL, PARTFXFLAG_10000 | PARTFXFLAG_2, -1, NULL);
    }
    sp50 = self->globalPosition.x - gWorldX;
    sp48 = self->globalPosition.z - gWorldZ;
    sp4C = self->globalPosition.y;
    camera_project_point(sp50, sp4C, sp48, &sp44, &sp40, &sp3C);
    camera_clip_to_screen(sp44, sp40, sp3C, &sp38, &sp34, NULL);
    sp2C = vi_obj_depth(sp38, sp34, self);
    get_vec3_to_camera_normalized(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &sp7C, &sp74, &sp78);
    sp50 += (sp7C * 20.0f);
    sp4C += (sp74 * 20.0f); 
    sp48 += (sp78 * 20.0f);
    camera_project_point(sp50, sp4C, sp48, &sp44, &sp40, &sp3C);
    camera_clip_to_screen(sp44, sp40, sp3C, NULL, NULL, &sp30);
    if ((vi_contains_point(sp38, sp34) != 0) && (sp30 > 0) && (sp30 < sp2C)) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_8D, NULL, PARTFXFLAG_10000 | PARTFXFLAG_2, -1, NULL);
    }
    arg2->unk8D = 0;
    return 0;
}


// offset: 0x534 | func: 4 | export: 3
void dll_405_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x588 | func: 5 | export: 4
void dll_405_free(Object* self, s32 a1) {
    CFLamp_Data* objdata;

    objdata = self->data;
    gDLL_13_Expgfx->vtbl->func5(self);
    if (objdata->unk4 != NULL) {
        obj_destroy_object(objdata->unk4);
    }
    objdata->unk4 = NULL;
    if (objdata->unk33 == 0) {
        func_80000450(self, self, 0x5DU, 0, 0, 0);
    }
}


// offset: 0x634 | func: 6 | export: 5
u32 dll_405_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x644 | func: 7 | export: 6
u32 dll_405_get_data_size(Object* self, u32 a1) {
    return sizeof(CFLamp_Data);
}
