#include "PR/ultratypes.h"
#include "dlls/engine/29_gplay.h"
#include "sys/lfx.h"
#include "sys/main.h"
#include "sys/rand.h"
#include "dll.h"

// size: 0x28
typedef struct {
    u8 _unk0[0x4 - 0x0];
    s32 unk4;
    u8 _unk8[0xC - 0x8];
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    u8 _unk26[0x28 - 0x26];
} BSS30;

typedef struct {
    s32 r;
    s32 g;
    s32 b;
} Rgb32;

/*0x0*/ static Rgb32 data_0[] = {
    {255, 255, 255}, 
    {255, 255, 255}
};
/*0x18*/ static f32 sTValue[] = {
    1.0f, 1.0f
};

/*0x0*/ static Rgb32 bss_0[2];
/*0x18*/ static Rgb32 bss_18[2];
/*0x30*/ static BSS30 bss_30[2];

static void dll_57_func_9A0(LightAction* action);
static void dll_57_func_BB0(LightAction* action);

// offset: 0x0 | ctor
void dll_57_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_57_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_57_Func_18(void) {
    data_0[0].r = 0;
    data_0[0].g = 0;
    data_0[0].b = 0;
    data_0[1].r = 0;
    data_0[1].g = 0;
    data_0[1].b = 0;
    bss_30[0].unk24 = 0;
    bss_30[1].unk24 = 0;
    sTValue[0] = 1.0f;
    sTValue[1] = 1.0f;
}

// offset: 0x74 | func: 1 | export: 1
void dll_57_Func_74(LightAction* laction) {
    PlayerLightActions* lactions;

    lactions = gDLL_29_Gplay->vtbl->get_current_player_lactions();
    if (laction->unk10 >= 2) {
        lactions->unkC[laction->unk10 - 2] = (s16) (laction->unke - 1);
        laction->unk10 = 1;
    }
    if (laction->unk12 & 1) {
        if (bss_30[laction->unk10].unk24 == 0) {
            dll_57_func_9A0(laction);
        } else {
            dll_57_func_BB0(laction);
        }
    } else if (laction->unk12 & 2) {
        bss_30[laction->unk10].unk24 = 0;
        lactions->unk8[laction->unk10] = (s16) (laction->unke - 1);
    }
}

// offset: 0x190 | func: 2 | export: 2
void dll_57_Func_190(s32* r1, s32* g1, s32* b1, s32* r2, s32* g2, s32* b2) {
    *r1 = data_0[0].r;
    *g1 = data_0[0].g;
    *b1 = data_0[0].b;
    *r2 = data_0[1].r;
    *g2 = data_0[1].g;
    *b2 = data_0[1].b;
}

// offset: 0x1E4 | func: 3 | export: 3
void dll_57_Func_1E4(void) {
    BSS30* temp_s0;
    f32 temp_fv0_3;
    f32 temp_fv1;
    f32 var_fa0;
    s32 _tempR;
    f32 multiplier = 1.0f;
    s16 _tempRgb[3];
    s16 var_ft1;
    s16 i;
    s16 counter;
    s16 r;
    s16 g;
    s16 b;
    s32 _tempG;
    s32 _tempB;

    counter = 2;
    i = 0;
    do {
        temp_s0 = &bss_30[i];
        if (temp_s0->unk1A & 8) {
            temp_s0->unkC += 180.0f * temp_s0->unk10 * (f32) gUpdateRate;
            if (temp_s0->unkC > 180.0f) {
                temp_s0->unkC = 0.0f;
                if (temp_s0->unk21 == 0) {
                    temp_s0->unk22 = (u8) ((f32) (temp_s0->unk18 - mathRnd(temp_s0->unk16, temp_s0->unk18)) * 0.5f);
                } else {
                    temp_s0->unk22 = (u8) (mathRnd(temp_s0->unk16, temp_s0->unk21) + temp_s0->unk16);
                }
            }
            var_fa0 = mathCosInterp((s16) (temp_s0->unkC * 182.04166f)) / 65535.0f;
            if (var_fa0 < 0.0f) {
                var_fa0 *= -1.0f;
            }
            var_ft1 = temp_s0->unk18 - (temp_s0->unk22 * (1.0f - var_fa0));
            if (sTValue[i] < 1.0f) {
                _tempR = bss_0[i].r;
                _tempG = bss_0[i].g;
                _tempB = bss_0[i].b;
                // Lerp bss_0[i] -> bss_18[i] via sTValue[i]
                r = bss_0[i].r + (sTValue[i] * ((bss_18[i].r - bss_0[i].r)));
                g = bss_0[i].g + (sTValue[i] * ((bss_18[i].g - bss_0[i].g)));
                b = bss_0[i].b + (sTValue[i] * ((bss_18[i].b - bss_0[i].b)));
                _tempRgb[0] = data_0[i].r = r;
                _tempRgb[1] = data_0[i].g = g;
                _tempRgb[2] = data_0[i].b = b;
                sTValue[i] += 0.02f; // @bug: framerate-dependent
                if (sTValue[i] > 1.0f) {
                    sTValue[i] = 1.0f;
                }
            } else {
                temp_fv1 = (temp_s0->unk22 + temp_s0->unk16) / 255.0f;
                temp_fv0_3 = 1.0f - ((((var_ft1 / 255.0f) - temp_fv1) * (0, var_fa0)) + temp_fv1);
                data_0[i].r = ((temp_s0->unk1B - temp_s0->unk1E) * temp_fv0_3) + temp_s0->unk1E;
                data_0[i].g = ((temp_s0->unk1C - temp_s0->unk1F) * temp_fv0_3) + temp_s0->unk1F;
                data_0[i].b = ((temp_s0->unk1D - temp_s0->unk20) * temp_fv0_3) + temp_s0->unk20;
            }
            if (multiplier != 1.0f) {
                data_0[i].r *= multiplier;
                data_0[i].g *= multiplier;
                data_0[i].b *= multiplier;
            }
        } else {
            if (sTValue[i] < 1.0f) {
                _tempR = bss_0[i].r;
                _tempG = bss_0[i].g;
                _tempB = bss_0[i].b;
                // Lerp bss_0[i] -> bss_18[i] via sTValue[i]
                r = bss_0[i].r + (sTValue[i] * ((bss_18[i].r - bss_0[i].r)));
                g = bss_0[i].g + (sTValue[i] * ((bss_18[i].g - bss_0[i].g)));
                b = bss_0[i].b + (sTValue[i] * ((bss_18[i].b - bss_0[i].b)));
                _tempRgb[0] = data_0[i].r = r;
                _tempRgb[1] = data_0[i].g = g;
                _tempRgb[2] = data_0[i].b = b;
                sTValue[i] += 0.02f; // @bug: framerate-dependent
                if (sTValue[i] > 1.0f) {
                    sTValue[i] = 1.0f;
                }
            } else {
                data_0[i].r = bss_18[i].r;
                data_0[i].g = bss_18[i].g;
                data_0[i].b = bss_18[i].b;
            }
            if (multiplier != 1.0f) {
                data_0[i].r *= multiplier;
                data_0[i].g *= multiplier;
                data_0[i].b *= multiplier;
            }
        }
        if (temp_s0){} // @fake
        if (data_0[i].r > 255) {
            data_0[i].r = 255;
        } else if (data_0[i].r < 0) {
            data_0[i].r = 0;
        }
        if (data_0[i].g > 255) {
            data_0[i].g = 255;
        } else if (data_0[i].g < 0) {
            data_0[i].g = 0;
        }
        if (data_0[i].b > 255) {
            data_0[i].b = 255;
        } else if (data_0[i].b < 0) {
            data_0[i].b = 0;
        }
        i += 1;
        counter -= 1;
    } while (counter != 0);
}

// offset: 0x9A0 | func: 4
void dll_57_func_9A0(LightAction* action) {
    s16 temp_a3 = (s16) action->unk10;
    bss_30[temp_a3].unk25 = (u8) temp_a3;
    bss_30[temp_a3].unk14 = action->unk1b;
    bss_30[temp_a3].unk1A = action->unk12;
    bss_30[temp_a3].unk4 = action->unk0;
    bss_30[temp_a3].unk1B = action->unk15;
    bss_30[temp_a3].unk1C = action->unk16;
    bss_30[temp_a3].unk1D = action->unk17;
    bss_30[temp_a3].unk1E = action->unk18;
    bss_30[temp_a3].unk1F = action->unk19;
    bss_30[temp_a3].unk20 = action->unk1a;
    bss_30[temp_a3].unkC = (f32) mathRnd(0, 180);
    bss_30[temp_a3].unk22 = mathRnd(action->unk1e, action->unk1d);
    if (action->unk22 == 0xA) {
        bss_30[temp_a3].unk10 = 0.16666667f;
    } else if (action->unk22) {
        bss_30[temp_a3].unk10 = 1.0f / ((f32) action->unk22 * 60.0f);
    } else {
        bss_30[temp_a3].unk10 = 1.0f / ((f32) mathRnd(1, 10) * 60.0f);
    }
    bss_30[temp_a3].unk23 = action->unk22;
    bss_30[temp_a3].unk18 = action->unk1d;
    bss_30[temp_a3].unk16 = action->unk1e;
    bss_30[temp_a3].unk21 = action->unk23;
    sTValue[temp_a3] = 0.0f;
    bss_18[temp_a3].r = action->unk18;
    bss_18[temp_a3].g = action->unk19;
    bss_18[temp_a3].b = action->unk1a;
    bss_30[temp_a3].unk24 = 1;
}

// offset: 0xBB0 | func: 5
void dll_57_func_BB0(LightAction* action) {
    s16 i = (s16) action->unk10;
    bss_0[i].r = bss_18[i].r;
    bss_0[i].g = bss_18[i].g;
    bss_0[i].b = bss_18[i].b;
    sTValue[i] = 0.0f;
    bss_18[i].r = action->unk18;
    bss_18[i].g = action->unk19;
    bss_18[i].b = action->unk1a;
    bss_30[i].unk1B = action->unk15;
    bss_30[i].unk1C = action->unk16;
    bss_30[i].unk1D = action->unk17;
    bss_30[i].unk1E = action->unk18;
    bss_30[i].unk1F = action->unk19;
    bss_30[i].unk20 = action->unk1a;
    bss_30[i].unk18 = action->unk1d;
    bss_30[i].unk16 = action->unk1e;
    bss_30[i].unk22 = mathRnd(action->unk1e, action->unk1d);
    if (action->unk22 == 0xA) {
        bss_30[i].unk10 = 0.16666667f;
    } else if (action->unk22) {
        bss_30[i].unk10 = 1.0f / ((f32) action->unk22 * 60.0f);
    } else {
        bss_30[i].unk10 = 1.0f / ((f32) mathRnd(1, 10) * 60.0f);
    }
    bss_30[i].unk23 = action->unk22;
    bss_30[i].unk18 = action->unk1d;
    bss_30[i].unk16 = action->unk1e;
    bss_30[i].unk21 = action->unk23;
}
