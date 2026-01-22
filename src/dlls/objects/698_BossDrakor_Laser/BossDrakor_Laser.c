#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/13_expgfx.h"
#include "dlls/engine/14_modgfx.h"
#include "dlls/engine/17_partfx.h"
#include "game/objects/object_id.h"
#include "sys/gfx/modgfx.h"
#include "sys/dll.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/newshadows.h"
#include "dll.h"
#include "functions.h"

typedef struct {
/*00*/ s16 unk0;
/*02*/ s16 unk2;
/*04*/ s16 unk4;
/*08*/ u32 unk8;
/*0C*/ u32 unkC;
/*10*/ s16 unk10;
/*12*/ s16 unk12;
} BossDrakor_Laser_Data;

static void dll_698_func_6E4(Object *self, Vec3f *arg1);

// offset: 0x0 | ctor
void dll_698_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_698_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_698_setup(Object *self, ObjSetup *setup, s32 arg2) {
    self->opacity = 0;
    self->unkB0 |= 0x6000;
}

// offset: 0x38 | func: 1 | export: 1
void dll_698_control(Object *self) {
    BossDrakor_Laser_Data *objdata;
    MtxF sp4C;
    Vec3f sp40;
    Camera *camera;
    f32 sp38;
    f32 sp34;

    objdata = self->data;
    if ((objdata->unk2 != 0) || (objdata->unk4 != 0)) {
        matrix_from_srt(&sp4C, &self->srt);
        sp40.f[0] = 0.0f;
        sp40.f[1] = 0.0f;
        sp40.f[2] = 1000.0f;
        vec3_transform(&sp4C, sp40.f[0], sp40.f[1], sp40.f[2], &sp40.f[0], &sp40.f[1], &sp40.f[2]);
        dll_698_func_6E4(self, &sp40);
        if (objdata->unk4 != 0) {
            objdata->unk4 -= (s16) gUpdateRateF;
            if (objdata->unk4 <= 0) {
                objdata->unk4 = 0;
                gDLL_14_Modgfx->vtbl->func10(self);
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk8);
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->unkC);
                objdata->unk8 = 0;
                objdata->unkC = 0;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9A1, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        }
        if (objdata->unk2 != 0) {
            objdata->unk2 -= (s16) gUpdateRateF;
            if (objdata->unk2 <= 0) {
                objdata->unk2 = 0;
                objdata->unk4 = objdata->unk0;
                gDLL_14_Modgfx->vtbl->func10(self);
            }
        }
        if ((objdata->unk8 != 0) || (objdata->unkC != 0)) {
            camera = get_camera();
            sp40.f[0] = 0.0f;
            sp40.f[1] = 0.0f;
            sp34 = vec3_distance(&self->srt.transl, (Vec3f* ) &camera->tx);
            sp40.f[2] = sp34;
            vec3_transform(&sp4C, sp40.f[0], sp40.f[1], sp40.f[2], &sp40.f[0], &sp40.f[1], &sp40.f[2]);
            sp38 = vec3_distance_xz(&sp40, &camera->srt.transl);
            if (objdata->unk8 != 0) {
                objdata->unk10 = (s16) (s32) ((2.0f - ((2.0f * sp38) / sp34)) * 127.0f);
                if (objdata->unk10 < 0) {
                    objdata->unk10 = 0;
                }
                if (objdata->unk10 >= 0x80) {
                    objdata->unk10 = 0x7F;
                }
                gDLL_6_AMSFX->vtbl->func_860(objdata->unk8, objdata->unk10);
                gDLL_6_AMSFX->vtbl->func_954(objdata->unk8, 1.0625f - ((f32) objdata->unk10 * (1.0f / 1024.0f)));
            }
            if (objdata->unkC != 0) {
                objdata->unk12 = (s16) (s32) ((2.0f - (sp38 / sp34)) * 127.0f);
                if (objdata->unk12 < 0) {
                    objdata->unk12 = 0;
                }
                if (objdata->unk12 >= 0x80) {
                    objdata->unk12 = 0x7F;
                }
                gDLL_6_AMSFX->vtbl->func_860(objdata->unkC, (objdata->unk12 >> 1));
                gDLL_6_AMSFX->vtbl->func_954(objdata->unkC, ((f32) objdata->unk12 * (1.0f / 1024.0f)) + 0.9365f);
            }
        }
    }
}

// offset: 0x4AC | func: 2 | export: 2
void dll_698_update(Object *self) { }

// offset: 0x4B8 | func: 3 | export: 3
void dll_698_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x4D0 | func: 4 | export: 4
void dll_698_free(Object *self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
    gDLL_14_Modgfx->vtbl->func4(self);
}

// offset: 0x540 | func: 5 | export: 5
u32 dll_698_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x550 | func: 6 | export: 6
u32 dll_698_get_data_size(Object *self, u32 a1) {
    return sizeof(BossDrakor_Laser_Data);
}

// offset: 0x564 | func: 7 | export: 7
void dll_698_func_564(Object *arg0, s32 arg1, s32 arg2) {
    BossDrakor_Laser_Data *objdata;
    DLL_IModgfx *sp30;

    objdata = arg0->data;
    if ((arg1 <= 0) || (arg2 <= 0)) {
        objdata->unk0 = 0;
        objdata->unk2 = 0;
        objdata->unk4 = 0;
        return;
    }
    objdata->unk0 = arg2;
    objdata->unk2 = arg1;
    objdata->unk4 = 0;
    sp30 = dll_load_deferred(DLL_ID_177, 1);
    sp30->vtbl->func0(arg0, 0, 0, 4, -1, 0);
    dll_unload(sp30);
    objdata->unk10 = 1;
    objdata->unk12 = 1;
    objdata->unk8 = gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_98F, (u8) objdata->unk10, NULL, NULL, 0, NULL);
    objdata->unkC = gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_990, (u8) objdata->unk12, NULL, NULL, 0, NULL);
    gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_9A0, MAX_VOLUME, NULL, NULL, 0, NULL);
}

// offset: 0x6E4 | func: 8
static void dll_698_func_6E4(Object *self, Vec3f *arg1) {
    s32 objIdx;
    s32 objListLength;
    f32 var_fv1;
    Object **objList;
    Vec4f spC0;
    Vec3f spB4;
    Vec3f spA8;
    Vec3f sp9C;
    SRT sp84;
    s32 i;

    objList = get_world_objects(&objIdx, &objListLength);
    while (objIdx < objListLength) {
        if (self != objList[objIdx]) {
            switch (objList[objIdx]->id) {
                case OBJ_BossDrakorFlatR:
                    spC0.x = 0.0f;
                    spC0.y = -1.0f;
                    spC0.z = 0.0f;
                    spC0.w = objList[objIdx]->srt.transl.y;
                    var_fv1 = shadows_func_800528AC(&self->srt.transl, arg1, &sp9C, &spC0);
                    if (self->srt.transl.y < objList[objIdx]->srt.transl.y) {
                        var_fv1 = -var_fv1;
                    }
                    if (var_fv1 >= 0.0f && vec3_distance(&objList[objIdx]->srt.transl, &sp9C) < 100.0f) {
                        sp84.transl.x = sp9C.x;
                        sp84.transl.y = sp9C.y;
                        sp84.transl.z = sp9C.z;
                        sp84.scale = 1.0f;
                        sp84.roll = 0;
                        sp84.pitch = 0;
                        sp84.yaw = 0;
                        for (i = 0; i < 10; i++) {
                            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_6DB, &sp84, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, (void* )1);
                        }
                        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_6DB, &sp84, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_6DC, &sp84, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                        break;
                    }
                    // @bug?
                    /* fallthrough */
                case OBJ_BossDrakorAster:
                    spA8.x = arg1->x - self->srt.transl.x;
                    spA8.y = arg1->y - self->srt.transl.y;
                    spA8.z = arg1->z - self->srt.transl.z;
                    if (func_80056BCC(&self->srt.transl, &spA8, &spB4, &spC0, &objList[objIdx]->srt.transl, 
                                      SQ((f32) objList[objIdx]->objhitInfo->unk52 * 4.0f)) != 0) {
                        func_8002635C(objList[objIdx], self, 0xA, 1, 1);
                    }
                    break;
            }
        }
        
        objIdx++;
    }
}
