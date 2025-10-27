#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "segment_334F0.h"
#include "functions.h"
#include "dll.h"
#include "macros.h"

typedef struct {
    u32 unk0;
    u8 _unk4[0x8 - 0x4];
    s8 unk8;
    s8 unk9;
    f32 unkC;
    f32 unk10;
    u8 unk14;
} KT_RexFloorSwitch_Data;

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    s16 unk1C;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
} KT_RexFloorSwitch_Setup;

/*0x0*/ static s32 _data_0[] = { 0x19 };

static void dll_703_func_A7C(Object* self, s32 arg1);

// offset: 0x0 | ctor
void dll_703_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_703_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_703_setup(Object* self, KT_RexFloorSwitch_Setup* setup, s32 arg2) {
    KT_RexFloorSwitch_Data *objdata = (KT_RexFloorSwitch_Data*)self->data;
    
    self->srt.yaw = setup->unk18 << 8;
    self->unkE0 = 1;
    self->unkDC = 1;
    objdata->unkC = (f32) setup->unk19;
    dll_703_func_A7C(self, 0);
}

// offset: 0xA4 | func: 1 | export: 1
void dll_703_control(Object* self) {
    KT_RexFloorSwitch_Setup* objsetup;
    KT_RexFloorSwitch_Data* objdata;
    s32 var_v0;
    s8 sp4B;
    f32 temp_fv0;
    u8 temp_v0_3;
    s32* sp3C;

    objsetup = (KT_RexFloorSwitch_Setup*)self->setup;
    objdata = (KT_RexFloorSwitch_Data*)self->data;
    self->unkE0 = self->unkDC;
    self->unkDC = main_get_bits(objsetup->unk1C);
    sp3C = func_800348A0(self, 0, 0);
    if (self->unkDC < 2) {
        *sp3C = 0;
        if ((self->unkDC == 0) && (self->unkE0 != 0)) {
            objdata->unk14 |= 4;
        }
        if ((self->unkDC != 0) && (self->unkE0 == 0)) {
            objdata->unk14 |= 2;
            self->srt.transl.y = objsetup->base.y - (f32) objsetup->unk1F;
            dll_703_func_A7C(self, main_get_bits(BIT_572_KT_FightProgress) >> 1);
        }
        if (!(objdata->unk14 & 6)) {
            return;
        }
    } else {
        if (self->unkE0 != 0) {
            *sp3C = 0x100;
            objdata->unk14 &= ~0x1;
        } else {
            objdata->unk14 |= 2;
            self->srt.transl.y = objsetup->base.y - (f32) objsetup->unk1F;
            dll_703_func_A7C(self, main_get_bits(BIT_572_KT_FightProgress) >> 1);
        }
    }
    objdata->unk8 -= 1;
    if (objdata->unk8 < 0) {
        objdata->unk8 = 0;
    }
    if ((self->unk58->unk10f > 0) && (self->unkDC == 2)) {
        objdata->unk8 = 5;
    }
    sp4B = 0;
    if (objdata->unk14 & 4) {
        temp_fv0 = objsetup->base.y - (f32) objsetup->unk1F;
        if (temp_fv0 < self->srt.transl.y) {
            self->srt.transl.y -= (0.075f * gUpdateRateF);
            if (self->srt.transl.y <= temp_fv0) {
                self->srt.transl.y = temp_fv0;
                objdata->unk14 = objdata->unk14 & ~0x4;
            } else {
                sp4B = 1;
                gDLL_17->vtbl->func1(self, 0x488, NULL, 2, -1, NULL);
            }
        }
    } else if (objdata->unk14 & 2) {
        if (self->srt.transl.y < objsetup->base.y) {
            self->srt.transl.y += (0.075f * gUpdateRateF);
            if (objsetup->base.y <= self->srt.transl.y) {
                self->srt.transl.y = objsetup->base.y;
                objdata->unk14 = objdata->unk14 & ~0x2;
            } else {
                sp4B = 1;
                gDLL_17->vtbl->func1(self, 0x488, NULL, 2, -1, NULL);
            }
        }
    } else if ((objdata->unk8 != 0) && !(objdata->unk14 & 1)) {
        temp_fv0 = objsetup->base.y - (f32) objsetup->unk1E;
        if (temp_fv0 < self->srt.transl.y) {
            self->srt.transl.y -= (0.125f * gUpdateRateF);
            if (self->srt.transl.y < temp_fv0) {
                self->srt.transl.y = temp_fv0;
            } else {
                sp4B = 1;
            }
        }
        if (objdata->unkC < 0.0f) {
            objdata->unkC = (f32) objsetup->unk19;
            temp_v0_3 = main_get_bits(objsetup->unk1A);
            if (temp_v0_3 < 0xF) {
                main_set_bits(objsetup->unk1A, ++temp_v0_3);
            } else {
                objdata->unk14 |= 9;
            }
        }
        objdata->unkC -= gUpdateRateF;
    } else {
        self->srt.transl.y += 0.125f * gUpdateRateF;
        if (objsetup->base.y < self->srt.transl.y) {
            self->srt.transl.y = objsetup->base.y;
        } else {
            sp4B = 1;
        }
    }
    if (objdata->unk14 & 8) {
        if (main_get_bits(BIT_55A) != 0) {
            main_set_bits(BIT_55A, 0);
            main_set_bits(BIT_55B, 1);
        } else {
            main_set_bits(BIT_55A, 1);
            main_set_bits(BIT_55B, 0);
        }
        main_set_bits(BIT_55C, 1);
        objdata->unk14 &= ~0x8;
        main_set_bits(objsetup->unk1A, 0);
    } else if (objdata->unk9 != objdata->unk8) {
        main_set_bits(objsetup->unk1A, 0);
    }
    if (sp4B != 0) {
        if (objdata->unk0 == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_6E2, MAX_VOLUME, &objdata->unk0, NULL, 0, NULL);
        }
    } else {
        if (objdata->unk0 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk0);
            objdata->unk0 = 0;
        }
    }
    if (self->unkDC == 2) {
        if (objdata->unk8 != 0) {
            if (objdata->unk10 == 0.0f) {
                objdata->unk10 = 8.0f;
            }
            var_v0 = (s32) ((f32) *sp3C + (gUpdateRateF * objdata->unk10));
            if (var_v0 >= 0x201) {
                var_v0 = 0x400 - var_v0;
                objdata->unk10 = -objdata->unk10;
            } else if (var_v0 < 0x100) {
                var_v0 = 0x200 - var_v0;
                objdata->unk10 = -objdata->unk10;
            }
            *sp3C = var_v0;
        } else {
            var_v0 = (s32) ((f32) *sp3C + (gUpdateRateF * objdata->unk10));
            if (var_v0 >= 0x201) {
                var_v0 = 0x400 - var_v0;
                objdata->unk10 = -objdata->unk10;
            } else if (var_v0 < 0x100) {
                var_v0 = 0x100;
                objdata->unk10 = 0.0f;
            }
            *sp3C = var_v0;
        }
        if (!(objdata->unk14 & 6)) {
            gDLL_17->vtbl->func1(self, 0x486, NULL, 2, -1, NULL);
        }
    } else {
        if (*sp3C != 0) {
            var_v0 = (s32) ((f32) *sp3C + (gUpdateRateF * objdata->unk10));
            if (var_v0 >= 0x201) {
                var_v0 = 0x400 - var_v0;
                objdata->unk10 = -objdata->unk10;
            } else if (var_v0 < 0x100) {
                var_v0 = 0;
            }
            *sp3C = var_v0;
        }
    }
    objdata->unk9 = objdata->unk8;
}

// offset: 0x960 | func: 2 | export: 2
void dll_703_update(Object *self) { }

// offset: 0x96C | func: 3 | export: 3
void dll_703_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    KT_RexFloorSwitch_Setup* objsetup = (KT_RexFloorSwitch_Setup*)self->setup;
    
    if (visibility != 0) {
        func_80036F6C(objsetup->unk20, objsetup->unk21, objsetup->unk22);
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x9FC | func: 4 | export: 4
void dll_703_free(Object* self, s32 a1) {
    KT_RexFloorSwitch_Data *objdata = (KT_RexFloorSwitch_Data*)self->data;
    
    if (objdata->unk0 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk0);
    }
}

// offset: 0xA58 | func: 5 | export: 5
u32 dll_703_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xA68 | func: 6 | export: 6
u32 dll_703_get_data_size(Object *self, u32 a1) {
    return sizeof(KT_RexFloorSwitch_Data);
}

// offset: 0xA7C | func: 7
static void dll_703_func_A7C(Object* self, s32 arg1) {
    CurveSetup* curve;
    ObjSetup* objsetup;
    s32 curveUID;

    objsetup = self->setup;
    curveUID = gDLL_26_Curves->vtbl->curves_func_1e4(objsetup->x, objsetup->y, objsetup->z, _data_0, ARRAYCOUNT(_data_0), arg1);
    if (curveUID != -1) {
        curve = gDLL_26_Curves->vtbl->curves_func_39c(curveUID);
        if (curve != NULL) {
            self->srt.transl.x = curve->base.x;
            self->srt.transl.z = curve->base.z;
        }
    }
}

/*0x0*/ static const char str_0[] = "Could not find node [%d]\n";
