#include "dlls/objects/214_animobj.h"
#include "sys/objanim.h"
#include "segment_334F0.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s8 unk1E;
    u8 unk1F;
    u8 unk20;
    s16 unk22;
} GP_ShrinePillar_Setup;

typedef struct {
    u8 unk0;
    u8 unk1;
    f32 unk4;
} GP_ShrinePillar_Data;

static int dll_549_func_264(Object* a0, Object* a1, AnimObj_Data* a2, void* a3);

// offset: 0x0 | ctor
void dll_549_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_549_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_549_setup(Object* self, GP_ShrinePillar_Setup* setup, s32 arg2) {
    s32* temp_v0;
    GP_ShrinePillar_Data* objdata;

    objdata = (GP_ShrinePillar_Data*)self->data;
    if (setup->unk1A != -1) {
        if (main_get_bits(setup->unk1A) != 0) {
            objdata->unk0 = 6;
        }
    } else {
        objdata->unk0 = 0;
    }
    objdata->unk1 = 1;
    self->srt.yaw = setup->unk1F << 8;
    self->unk0xbc = (ObjectCallback)dll_549_func_264;
    temp_v0 = func_800348A0(self, 0, 0);
    if (temp_v0 != NULL) {
        *temp_v0 = 0;
    }
}

// offset: 0xE0 | func: 1 | export: 1
void dll_549_control(Object* self) {
    GP_ShrinePillar_Data* objdata;
    GP_ShrinePillar_Setup* setup;
    s32 var_a2;

    objdata = (GP_ShrinePillar_Data*)self->data;
    setup = (GP_ShrinePillar_Setup*)self->setup;
    diPrintf("control\n");
    if (objdata->unk1 != 0) {
        if ((setup->unk1C != 0) && (objdata->unk0 != 0)) {
            var_a2 = setup->unk20;
            gDLL_3_Animation->vtbl->func20(self, setup->unk1C);
        } else {
            var_a2 = -1; 
        }
        if (setup->unk1E != -1) {
            gDLL_3_Animation->vtbl->func17(setup->unk1E, self, var_a2);
        }
        objdata->unk1 = 0;
    }
}

static const char str_1[] = " PREMPT %i seqtime %i \n\n";

// offset: 0x1D0 | func: 2 | export: 2
void dll_549_update(Object *self) { }

// offset: 0x1DC | func: 3 | export: 3
void dll_549_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x230 | func: 4 | export: 4
void dll_549_free(Object *self, s32 a1) { }

// offset: 0x240 | func: 5 | export: 5
u32 dll_549_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x250 | func: 6 | export: 6
u32 dll_549_get_data_size(Object *self, u32 a1) {
    return sizeof(GP_ShrinePillar_Data);
}

// offset: 0x264 | func: 7
int dll_549_func_264(Object* a0, Object* a1, AnimObj_Data* a2, void* a3) {
    GP_ShrinePillar_Setup* setup;
    GP_ShrinePillar_Data* objdata;
    s32* temp_v0_2;
    s32* temp_v0;
    s32 var_v1_3;
    s32 var_v0_2;
    s32 var_v1_4;

    objdata = (GP_ShrinePillar_Data*)a0->data;
    setup = (GP_ShrinePillar_Setup*)a0->setup;
    diPrintf("override %d\n", objdata->unk0);
    switch (objdata->unk0) {
    case 0:
        if (main_get_bits(setup->unk18) != 0) {
            objdata->unk0 = 1;
        }
        break;
    case 1:
        for (var_v0_2 = 0; var_v0_2 < a2->unk98; var_v0_2++) {
            if (a2->unk8E[var_v0_2] == 1) {
                objdata->unk0 = 6;
                if (setup->unk1A != -1) {
                    main_set_bits(setup->unk1A, 1);
                }
            }
        }
        break;
    case 6:
        if (main_get_bits(setup->unk22) != 0) {
            objdata->unk0 = 7;
        }
        break;
    case 7:
        for (var_v0_2 = 0; var_v0_2 < a2->unk98; var_v0_2++) {
            if (a2->unk8E[var_v0_2] == 2) {
                objdata->unk0 = 2;
            }
        }
        break;
    case 2:
        if (func_80025F40(a0, NULL, NULL, NULL) == 0x19) {
            objdata->unk0 = 4;
        }
        break;
    case 3:
        objdata->unk4 -= delayFloat;
        if (objdata->unk4 <= 0.0f) {
            objdata->unk0 = 5;
        }
        break;
    case 4:
        temp_v0 = func_800348A0(a0, 0, 0);
        if (temp_v0 != NULL) {
            var_v1_3 = *temp_v0 + (delayByte * 8);
            if (var_v1_3 > 0x100) {
                var_v1_3 = 0x100;
                objdata->unk0 = 3;
                objdata->unk4 = 800.0f;
            }
            *temp_v0 = var_v1_3;
        }
        break;
    case 5:
        temp_v0_2 = func_800348A0(a0, 0, 0);
        if (temp_v0_2 != NULL) {
            var_v1_4 = *temp_v0_2 - (delayByte * 8);
            if (var_v1_4 < 0) {
                var_v1_4 = 0;
                objdata->unk0 = 2;
            }
            *temp_v0_2 = var_v1_4;
        }
        break;
    }
    if ((objdata->unk0 == 1) || (objdata->unk0 == 7) || (objdata->unk0 == 8)) {
        return 0;
    } else {
        return 1;
    }
}
