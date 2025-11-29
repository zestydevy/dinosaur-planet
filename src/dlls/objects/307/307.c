#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "dll.h"
#include "functions.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 unkC;
    u8 unkD;
    u8 unkE;
} DLL307_Data;

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    u8 unk28;
    u8 unk29;
} DLL307_Setup;

static int dll_307_func_33C(Object *self, Object *a1, AnimObj_Data *a2, s8 a3);
static void dll_307_func_6E4(DLL307_Data *objdata, DLL307_Setup *setup);

// offset: 0x0 | ctor
void dll_307_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_307_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_307_setup(Object* self, DLL307_Setup* setup, s32 arg2) {
    DLL307_Data* objdata = (DLL307_Data*)self->data;
    
    objdata->unkD = 1;
    self->srt.yaw = setup->unk1F << 8;
    self->animCallback = dll_307_func_33C;
    self->unkB0 |= 0x2000;
    self->srt.scale = (f32) setup->unk21 * 0.015625f;
    if (self->srt.scale == 0.0f) {
        self->srt.scale = 1.0f;
    }
    self->srt.scale *= self->def->scale;
    if (setup->unk1A != -1) {
        objdata->unkC = main_get_bits(setup->unk1A);
    } else {
        objdata->unkC = 0;
    }
    objdata->unk0 = fsin16_precise(self->srt.yaw);
    objdata->unk4 = fcos16_precise(self->srt.yaw);
    objdata->unk8 = (f32) -((objdata->unk0 * self->srt.transl.x) + (objdata->unk4 * self->srt.transl.z));
    objdata->unkE = 0;
    if (main_get_bits(setup->unk18) != 0) {
        objdata->unkE |= 1;
    }
    if (main_get_bits(setup->unk22) != 0) {
        objdata->unkE |= 2;
    }
}

// offset: 0x1C8 | func: 1 | export: 1
void dll_307_control(Object* self) {
    DLL307_Data* objdata;
    DLL307_Setup* setup;
    s32 var_a2;

    setup = (DLL307_Setup*)self->setup;
    objdata = (DLL307_Data*)self->data;
    if (objdata->unkD != 0) {
        if ((setup->unk1C != 0) && (objdata->unkC != 0)) {
            var_a2 = setup->unk20;
            gDLL_3_Animation->vtbl->func20(self, setup->unk1C);
        } else {
            var_a2 = -1;
        }
        
        if (setup->unk1E != -1) {
            gDLL_3_Animation->vtbl->func17(setup->unk1E, self, var_a2);
        }
        objdata->unkD = 0;
    }
}

// offset: 0x2A8 | func: 2 | export: 2
void dll_307_update(Object *self) { }

// offset: 0x2B4 | func: 3 | export: 3
void dll_307_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x308 | func: 4 | export: 4
void dll_307_free(Object *self, s32 a1) { }

// offset: 0x318 | func: 5 | export: 5
u32 dll_307_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x328 | func: 6 | export: 6
u32 dll_307_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL307_Data);
}

// offset: 0x33C | func: 7
static int dll_307_func_33C(Object *self, Object *a1, AnimObj_Data *a2, s8 a3) {
    DLL307_Data *objdata;
    DLL307_Setup *setup;
    s32 *temp_v0;
    u32 temp_v0_3;
    s32 var_s1;
    s32 var_s1_2;

    objdata = (DLL307_Data*)self->data;
    setup = (DLL307_Setup*)self->setup;
    if (self->def->numAnimatedFrames != 0) {
        if (objdata->unkE & 1) {
            temp_v0 = func_800348A0(self, 0, 0);
            if (temp_v0 != NULL) {
                *temp_v0 = 0x100;
            }
        }
        if (objdata->unkE & 2) {
            temp_v0 = func_800348A0(self, 1, 0);
            if (temp_v0 != NULL) {
                *temp_v0 = 0x100;
            }
        }
    }
    if (objdata->unkC == 0) {
        temp_v0_3 = main_get_bits(setup->unk18);
        var_s1 = 0;
        if ((setup->unk22 == -1) || (main_get_bits(setup->unk22) != 0)) {
            var_s1 = 1;
        }
        if ((temp_v0_3 != 0) && !(objdata->unkE & 1)) {
            if (self->def->numAnimatedFrames != 0) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9A3_Magic_Reverse_Cymbal, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            objdata->unkE |= 1;
        }
        if ((var_s1 != 0) && !(objdata->unkE & 2)) {
            if (self->def->numAnimatedFrames != 0) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9A3_Magic_Reverse_Cymbal, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            objdata->unkE |= 2;
        }
        if (objdata->unkE == 3) {
            dll_307_func_6E4(objdata, setup);
            objdata->unkC = 2;
        }
    } else if ((objdata->unkC == 1) && (main_get_bits(setup->unk18) == 0)) {
        objdata->unkC = 3;
    }
    if (objdata->unkC == 2) {
        for (var_s1_2 = 0; var_s1_2 < a2->unk98; var_s1_2++) {
            if (a2->unk8E[var_s1_2] == 2) {
                objdata->unkC = 1;
                if (setup->unk1A != -1) {
                    main_set_bits(setup->unk1A, 1);
                }
            }
        }
    } else if (objdata->unkC == 3) {
        for (var_s1_2 = 0; var_s1_2 < a2->unk98; var_s1_2++) {
            if (a2->unk8E[var_s1_2] == 1) {
                dll_307_func_6E4(objdata, setup);
                objdata->unkC = 0;
                objdata->unkE = 0;
                if (setup->unk1A != -1) {
                    main_set_bits(setup->unk1A, 0);
                }
            }
        }
    }

    return !(objdata->unkC == 2) && !(objdata->unkC == 3);
}

// offset: 0x6E4 | func: 8
static void dll_307_func_6E4(DLL307_Data *objdata, DLL307_Setup *setup) {
    Camera *camera;
    f32 temp;
    u8 temp2;

    camera = get_main_camera();
    temp = (objdata->unk0 * camera->srt.transl.x) + (objdata->unk4 * camera->srt.transl.z) + objdata->unk8;
    if (temp < 0.0f) {
        if (setup->unk24 != -1) {
            temp2 = main_get_bits(setup->unk24);
            temp2 ^= setup->unk28;
            main_set_bits(setup->unk24, temp2);
        }
    } else {
        if (setup->unk26 != -1) {
            temp2 = main_get_bits(setup->unk26);
            temp2 ^= setup->unk29;
            main_set_bits(setup->unk26, temp2);
        }
    }
}
