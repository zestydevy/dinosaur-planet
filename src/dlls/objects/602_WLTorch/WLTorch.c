#include "common.h"
#include "sys/gfx/modgfx.h"

typedef struct {
    Object* unk0;
    f32 unk4;
    u32 unk8;
    s16 unkC;
    s16 unkE;
    u8 unk10; 
    u8 _unk11;
    s16 _unk12; 
} DLL602_Data;

typedef struct {
    ObjSetup base;
    u8 _unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
}DLL602_Setup;

// offset: 0x0 | ctor
void dll_602_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_602_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_602_setup(Object* self, DLL602_Setup* setup, s32 arg2) {
    DLL_IModgfx* sp4C;
    SRT sp34;
    DLL602_Data* objdata;

    objdata = self->data;
    if (setup->unk1A != 0) {
        objdata->unk4 = setup->unk1A;
    } else {
        objdata->unk4 = 75.0f;
    }
    if (setup->unk1C != 0) {
        objdata->unkE = setup->unk1C;
    } else {
        objdata->unkE = 0x8C;
    }
    objdata->unk10 = setup->unk19;
    sp34.transl.y = -2.0f;
    if (objdata->unk10 == 0) {
        sp4C = dll_load_deferred(DLL_ID_121, 1U);
        self->srt.scale *= 0.5f;
        sp4C->vtbl->func0(self, 1, &sp34, 0x10004, -1, 0);
    } else if (objdata->unk10 == 0x7F) {
        sp4C = dll_load_deferred(DLL_ID_121, 1U);
        self->srt.scale *= 0.5f;
        sp4C->vtbl->func0(self, 2, &sp34, 0x10004, -1, 0);
    } else {
        sp4C = dll_load_deferred(DLL_ID_115, 1U);
        self->srt.scale *= 0.5f;
        sp4C->vtbl->func0(self, 2, &sp34, 0x10004, -1, 0);
    }
    self->srt.scale *= 2.0f;
    dll_unload(sp4C);
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x224 | func: 1 | export: 1
void dll_602_control(Object* self) {
    DLL602_Data* objdata;
    f32 temp_fv0;
    SRT sp78;
    s32 sp74;
    s32 sp70;
    s32 sp6C;
    s32 sp68;
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;

    objdata = self->data;
    if (objdata->unk10 == 2) {
        self->srt.yaw += 0x32;
    }
    temp_fv0 = vec3_distance(&get_player()->globalPosition, &self->globalPosition);
    if (objdata->unk8 == 0) {
        if (temp_fv0 < 90.0f) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_1D3, MAX_VOLUME, &objdata->unk8, NULL, 0, NULL);
        }
    } else if (temp_fv0 >= 90.0f) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk8);
        objdata->unk8 = 0U;
    }
    if (objdata->unk10 != 2) {
        sp58 = self->srt.transl.f[0] - gWorldX;
        sp54 = self->srt.transl.f[1];
        sp50 = self->srt.transl.f[2] - gWorldZ;
        camera_project_point(sp58, sp54, sp50, &sp64, &sp60, &sp5C);
        camera_clip_to_screen(sp64, sp60, sp5C, &sp74, &sp70, NULL);
        sp68 = vi_obj_depth(sp74, sp70, self);
        get_vec3_to_camera_normalized(self->srt.transl.f[0], self->srt.transl.f[1], self->srt.transl.f[2], &sp4C, &sp48, &sp44);
        camera_project_point(sp58 += (sp4C * 20.0f), sp54 += (sp48 * 20.0f), sp50 += (sp44 * 20.0f), &sp64, &sp60, &sp5C);
        camera_clip_to_screen(sp64, sp60, sp5C, NULL, NULL, &sp6C);
        if (objdata->unkC > 0) {
           objdata->unkC -= gUpdateRate;
        } else {
            if ((vi_contains_point(sp74, sp70) != 0) && (sp6C > 0) && (sp6C < sp68)) {
                sp78.transl.f[0] = 0.0f;
                sp78.transl.f[2] = 0.0f;
                sp78.transl.f[1] = 13.0f;
                if (objdata->unk10 == 0x7F) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_741, &sp78, PARTFXFLAG_10 | PARTFXFLAG_2, -1, NULL);
                } else {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1F7, &sp78, PARTFXFLAG_10 | PARTFXFLAG_2, -1, NULL);
                }
            }
            objdata->unkC = 0x3C;
        }
    }
}

// offset: 0x594 | func: 2 | export: 2
void dll_602_update(Object *self) { }

// offset: 0x5A0 | func: 3 | export: 3
void dll_602_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x5B8 | func: 4 | export: 4
void dll_602_free(Object* self, s32 a1) {
    DLL602_Data* objdata;
    Object* temp_a0;
    u32 temp_a2;

    objdata = self->data;
    temp_a2 = objdata->unk8;
    if (temp_a2 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(temp_a2);
    }
    if (a1 == 0) {
        temp_a0 = objdata->unk0;
        if (temp_a0 != NULL) {
            obj_destroy_object(temp_a0);
        }
    }
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func4(self);
}


// offset: 0x68C | func: 5 | export: 5
s32 dll_602_get_model_flags(s32 arg0) {
    return MODFLAGS_1;
}

// offset: 0x69C | func: 6 | export: 6
u32 dll_602_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL602_Data);
}
