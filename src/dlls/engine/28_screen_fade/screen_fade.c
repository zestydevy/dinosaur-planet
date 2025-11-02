#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "sys/camera.h"
#include "sys/main.h"
#include "dll.h"
#include "sys/gfx/map.h"
#include "functions.h"
#include "sys/objects.h"
#include "types.h"
#include "dlls/engine/28_screen_fade.h"

static f32 sFadeAlpha; // 0-255
static f32 sFadeSpeed;
// After a non-reverse fade is complete, starts ticking up until 180 then does a reverse black fade
static f32 sAutoReverseTimer;
static u8 sFadeType;
static u8 sIsComplete;
static u8 sDelayTimer;

void screen_fade_fade_reversed(s32 duration, s32 type);
static void screen_fade_draw_simple_black(Gfx **gdl, Mtx **mtxs, Vertex **vtxs);
static void screen_fade_draw_simple(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, s32 red, s32 green, s32 blue);
static void screen_fade_draw_radial(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, u8 red, u8 green, u8 blue);

// offset: 0x0 | ctor
void screen_fade_ctor(void *self) {
    sFadeAlpha = 0;
    sFadeSpeed = 0;
    sAutoReverseTimer = 0;
    sFadeType = 0;
    sIsComplete = 0;
    sDelayTimer = 0;
}

// offset: 0x4C | dtor
void screen_fade_dtor(void *self) { }

// offset: 0x58 | func: 0 | export: 0
void screen_fade_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    Object *player;

    if (sDelayTimer != 0) {
        sDelayTimer--;
    } else {
        if (sAutoReverseTimer >= 180.0f) {
            player = get_player();
            if (player == NULL || (player->unkB0 & 0x1000) == 0) {
                gDLL_28_ScreenFade->vtbl->fade_reversed(30, SCREEN_FADE_BLACK);
                sAutoReverseTimer = 0.0f;
            }
        }

        sFadeAlpha += sFadeSpeed * gUpdateRateF;
        if (sFadeAlpha < 0.0f) {
            sFadeAlpha = 0.0f;
            sIsComplete = 1;
            return;
        }

        if (sFadeAlpha > 255.0f) {
            sFadeAlpha = 255.0f;
            sIsComplete = 1;
            sAutoReverseTimer += gUpdateRateF;
        } else {
            sIsComplete = 0;
        }
    }

    switch (sFadeType) {
        case SCREEN_FADE_BLACK:
            screen_fade_draw_simple_black(gdl, mtxs, vtxs);
            break;
        case SCREEN_FADE_WHITE:
            screen_fade_draw_simple(gdl, mtxs, vtxs, 0xFF, 0xFF, 0xFF);
            break;
        case SCREEN_FADE_WHITE_RADIAL:
            screen_fade_draw_radial(gdl, mtxs, vtxs, 0xFF, 0xFF, 0xFF);
            break;
        case SCREEN_FADE_RED:
            screen_fade_draw_simple(gdl, mtxs, vtxs, 0xFF, 0, 0);
            break;
    }
}

// offset: 0x294 | func: 1 | export: 1
void screen_fade_fade(s32 duration, s32 type) {
    if (sFadeSpeed <= 0.0f || sFadeAlpha == 255.0f) {
        sFadeAlpha = 0.0f;
    }

    sFadeSpeed = 256.0f / duration;
    sAutoReverseTimer = 0.0f;
    sFadeType = type;
    sDelayTimer = 0;
}

// offset: 0x320 | func: 2 | export: 2
void screen_fade_fade_reversed(s32 duration, s32 type) {
    if (sFadeSpeed >= 0.0f || sFadeAlpha == 0.0f) {
        sFadeAlpha = 255.0f;
    }

    sFadeSpeed = -256.0f / duration;
    sAutoReverseTimer = 0.0f;
    sFadeType = type;
    sDelayTimer = 1;
}

// offset: 0x3AC | func: 3 | export: 3
void screen_fade_func_3AC(s32 param1, s32 type, f32 alphaFactor) {
    sFadeAlpha = alphaFactor * 255.0f;
    sFadeSpeed = -(alphaFactor * 256.0f) / param1;
    sAutoReverseTimer = 0.0f;
    sFadeType = type;
    sDelayTimer = 1;
}

// offset: 0x41C | func: 4 | export: 5
f32 screen_fade_get_progress(void) {
    return sFadeAlpha;
}

// offset: 0x438 | func: 5 | export: 4
s32 screen_fade_is_complete(void) {
    return sIsComplete;
}

// offset: 0x454 | func: 6
static void screen_fade_draw_simple_black(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;

    func_80002130(&ulx, &uly, &lrx, &lry);

    gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);

    gDPSetCombineMode(*gdl, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    dl_apply_combine(gdl);

    gDPSetOtherMode(*gdl,
                    G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP |
                        G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2);
    dl_apply_other_mode(gdl);

    dl_set_prim_color(gdl, 0, 0, 0, (u8)sFadeAlpha);

    gDPFillRectangle((*gdl)++, ulx, uly, lrx, lry);

    gDLBuilder->needsPipeSync = TRUE;
    func_80002490(gdl);
}

// offset: 0x6DC | func: 7
static void screen_fade_draw_simple(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, s32 red, s32 green, s32 blue) {
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;

    func_80002130(&ulx, &uly, &lrx, &lry);

    gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);

    gDPSetCombineMode(*gdl, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    dl_apply_combine(gdl);

    gDPSetOtherMode(*gdl,
                    G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP |
                        G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2);
    dl_apply_other_mode(gdl);

    dl_set_prim_color(gdl, red, green, blue, (u8)sFadeAlpha);
    gDPFillRectangle((*gdl)++, ulx, uly, lrx, lry);

    gDLBuilder->needsPipeSync = TRUE;
    func_80002490(gdl);
}

// offset: 0x968 | func: 8
static void screen_fade_draw_radial(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, u8 red, u8 green, u8 blue) {
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;
    u16 temp_s4;
    u16 height;
    u16 temp_s5;
    u16 spB0;
    u16 spAE;
    u16 sp64;
    u16 temp_t7;
    u16 spA8;
    u16 width;
    u8 var_a1;
    u8 spA4;
    s32 pad;

    func_80002130(&ulx, &uly, &lrx, &lry);
    width = lrx - ulx;
    height = lry - uly;
    if (sFadeAlpha > 127.0f) {
        spA4 = 0xFF;
        sp64 = sFadeAlpha - 127.0f;
    } else {
        spA4 = 2.0f * sFadeAlpha;
        sp64 = 0;
    }
    temp_s5 = width / 2;
    // 0.0078125f = (1 / 128)
    temp_t7 = (sp64 * temp_s5) * 0.0078125f;
    if (temp_t7 == temp_s5) {
        screen_fade_draw_simple(gdl, mtxs, vtxs, red & 0xFF, blue, green);
    temp_s4 = temp_s5 - temp_t7 - 1;
        return;
    }
    spA8 = temp_s5 - temp_t7; // going left from center
    spAE = temp_s5 + temp_t7; // going right from center
    temp_s4 = temp_s5 - temp_t7 - 1;
    gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
    gDPSetCombineMode(*gdl, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    dl_apply_combine(gdl);
    gDPSetOtherMode(*gdl, 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | 
            G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2);
    dl_apply_other_mode(gdl);

    dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF,  spA4);
    // fill centered rect expanding out horz
    gDPFillRectangle((*gdl)++, temp_s4 + ulx + 1, uly, spAE + ulx, lry);
    gDLBuilder->needsPipeSync = 1;

    var_a1 = spA8 / (temp_s5 / 6);
    if (var_a1 <= 0) {
        var_a1 = 1;
    }
    for (spB0 = 0; spB0 < (spA8 - var_a1); spB0 += var_a1, spAE += var_a1, temp_s4 -= var_a1) {
        dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, ((temp_s5 - spB0) * spA4) / temp_s5);

        gDPFillRectangle((*gdl)++, spAE + ulx, uly, spAE + ulx + var_a1, lry);
        gDLBuilder->needsPipeSync = 1;

        gDPFillRectangle((*gdl)++, ((temp_s4 + ulx) - var_a1) + 1, uly, temp_s4 + ulx + 1, lry);
        gDLBuilder->needsPipeSync = 1;
    }
    dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, ((temp_s5 - spB0) * spA4) / temp_s5);

    gDPFillRectangle((*gdl)++, spAE + ulx, uly, lrx, lry);
    gDLBuilder->needsPipeSync = 1;

    gDPFillRectangle((*gdl)++, ulx, uly, temp_s4 + ulx + 1, lry);
    gDLBuilder->needsPipeSync = 1;

    temp_s5 = height / 2;
    temp_t7 = (sp64 * temp_s5) * 0.0078125f;
    spA8 = temp_s5 - temp_t7; // going down from center
    spAE = temp_s5 + temp_t7; // going up from center
    temp_s4 = temp_s5 - temp_t7 - 1;

    dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, spA4);

    gDPFillRectangle((*gdl)++, ulx, uly + temp_s4 + 1, lrx, uly + spAE);
    gDLBuilder->needsPipeSync = 1;

    var_a1 = spA8 / (temp_s5 / 8);
    if (var_a1 <= 0) {
        var_a1 = 1;
    }
    for (spB0 = 0; spB0 < (spA8 - var_a1); spB0 += var_a1, spAE += var_a1, temp_s4 -= var_a1) {
        dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, ((temp_s5 - spB0) * spA4) / temp_s5);

        gDPFillRectangle((*gdl)++, ulx, uly + spAE, lrx, uly + spAE + var_a1);
        gDLBuilder->needsPipeSync = 1;

        gDPFillRectangle((*gdl)++, ulx, ((uly + temp_s4) - var_a1) + 1, lrx, uly + temp_s4 + 1);
        gDLBuilder->needsPipeSync = 1;
    }

    dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, ((temp_s5 - spB0) * spA4) / temp_s5);

    gDPFillRectangle((*gdl)++, ulx, uly + spAE, lrx, lry);
    gDLBuilder->needsPipeSync = 1;

    gDPFillRectangle((*gdl)++, ulx, uly, lrx, uly + temp_s4 + 1);
    gDLBuilder->needsPipeSync = 1;

    func_80002490(gdl);
}
