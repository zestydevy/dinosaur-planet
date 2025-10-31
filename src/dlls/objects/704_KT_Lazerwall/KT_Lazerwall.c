
#include "dlls/engine/6_amsfx.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/rand.h"
#include "dll.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u32 unk4;
    u32 unk8;
    f32 unkC;
    f32 unk10;
} KT_Lazerwall_Data;

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} KT_Lazerwall_Setup;

/*0x0*/ static DLL_IModgfx *_data_0 = NULL;

// offset: 0x0 | ctor
void dll_704_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_704_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_704_setup(Object* self, KT_Lazerwall_Setup* setup, s32 arg2) {
    KT_Lazerwall_Data* objdata = (KT_Lazerwall_Data*)self->data;

    _data_0 = (DLL_IModgfx*)dll_load_deferred(DLL_ID_170, 1);
    self->srt.yaw = setup->unk18 << 8;
    objdata->unk10 = 0.0f;
}

// offset: 0x90 | func: 1 | export: 1
void dll_704_control(Object* self) {
    KT_Lazerwall_Setup* objsetup;
    KT_Lazerwall_Data* objdata;
    s16 temp_v0_2;
    s32 sp48;
    s32 sp44;

    objdata = (KT_Lazerwall_Data*)self->data;
    objsetup = (KT_Lazerwall_Setup*)self->setup;
    objdata->unk1 = objdata->unk0;
    objdata->unk0 &= ~0x3;
    temp_v0_2 = main_get_bits(objsetup->unk1A);
    if (temp_v0_2 >= objsetup->unk1C) {
        objdata->unk0 |= 0x4;
    } else {
        objdata->unk0 &= ~0x4;

        if (main_get_bits(objsetup->unk1E) == 0) {
            return;
        }
    }

    self->srt.roll += 0x38E;
    if (temp_v0_2 >= 0xF) {
        if (objdata->unkC <= 0.0f) {
            main_set_bits(objsetup->unk1E, 1);
            objdata->unk0 |= 9;
        }
        objdata->unkC = 120.0f;
    }
    if (objdata->unkC > 0.0f) {
        objdata->unkC -= gUpdateRateF;
        if (objdata->unkC <= 0.0f) {
            main_set_bits(objsetup->unk1E, 0);
            objdata->unk0 &= ~0x8;
        }
    }
    if (objdata->unk0 & 1) {
        if (_data_0 != NULL) {
            _data_0->vtbl->func0(self, 0, NULL, 4, -1, NULL);
            _data_0->vtbl->func0(self, 0, NULL, 4, -1, NULL);
            _data_0->vtbl->func0(self, 0, NULL, 4, -1, NULL);
            _data_0->vtbl->func0(self, 0, NULL, 4, -1, NULL);
        }
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_47E, NULL, PARTFXFLAG_2, -1, NULL);
        for (sp48 = 10; sp48 != 0; sp48--) {
            sp44 = 2;
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_48C, NULL, PARTFXFLAG_2, -1, &sp44);
        }
        objdata->unk10 = (f32) rand_next(1, 0x3C);
    }
    if (objdata->unk0 & 4) {
        sp44 = 0;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_48C, NULL, PARTFXFLAG_2, -1, &sp44);
        sp44 = 1;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_48C, NULL, PARTFXFLAG_2, -1, &sp44);
        if (!(objdata->unk1 & 4)) {
            objdata->unk8 = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_694_KT_Laserwall_PowerUp, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->func_954(objdata->unk8, (((f32) objsetup->unk1C / 15.0f) * 0.5f) + 1.0f);
        }
    }
    if (objdata->unk0 & 8) {
        sp44 = 0;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_48C, NULL, PARTFXFLAG_2, -1, &sp44);
        sp44 = 2;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_48C, NULL, PARTFXFLAG_2, -1, &sp44);
    }
    if (!(objdata->unk0 & 8) && (objdata->unk1 & 8)) {
        objdata->unk4 = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_696_KT_Laserwall_PowerDown, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (objdata->unk10 > 0.0f) {
        objdata->unk10 -= gUpdateRateF;
        if (objdata->unk10 <= 0.0f) {
            objdata->unk4 = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_695_KT_Laserwall_Zap, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->unk10 = 0.0f;
        }
    }
}

// offset: 0x650 | func: 2 | export: 2
void dll_704_update(Object *self) { }

// offset: 0x65C | func: 3 | export: 3
void dll_704_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x674 | func: 4 | export: 4
void dll_704_free(Object* self, s32 a1) {
    KT_Lazerwall_Data* objdata = (KT_Lazerwall_Data*)self->data;
    
    if (objdata->unk4 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk4);
    }
    if (objdata->unk8 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk8);
    }
    gDLL_14_Modgfx->vtbl->func5(self);
    dll_unload(_data_0);
}

// offset: 0x73C | func: 5 | export: 5
u32 dll_704_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x74C | func: 6 | export: 6
u32 dll_704_get_data_size(Object *self, u32 a1) {
    return sizeof(KT_Lazerwall_Data);
}
