#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/engine/28_screen_fade.h"
#include "sys/camera.h"
#include "sys/gfx/map.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "dll.h"
#include "functions.h"

static f32 sFadeAlpha; // 0-255
static f32 sFadeSpeed;
// After a non-reverse fade is complete, starts ticking up until 180 then does a reverse black fade
static f32 sAutoReverseTimer;
static u8 sFadeType;
static u8 sIsComplete;
static u8 sDelayTimer;

void screen_fade_fade_reversed(s32, s32);
/*static*/ void screen_fade_draw_simple_black(Gfx **gdl, Mtx **mtxs, Vertex **vtxs);
/*static*/ void screen_fade_draw_simple(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, s32 red, s32 green, s32 blue);
/*static*/ void screen_fade_draw_radial(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, s32 red, s32 green, s32 blue);

void screen_fade_ctor(void *self) {
    sFadeAlpha = 0;
    sFadeSpeed = 0;
    sAutoReverseTimer = 0;
    sFadeType = 0;
    sIsComplete = 0;
    sDelayTimer = 0;
}

void screen_fade_dtor(void *self) {
}

// called functions just need to be static, then it matches
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/28_screen_fade/screen_fade_draw.s")
#else
void screen_fade_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    Object *player;

    if (sDelayTimer != 0) {
        sDelayTimer--;
    } else {
        if (sAutoReverseTimer >= 180.0f) {
            player = get_player();
            if (player == NULL || (player->unk0xb0 & 0x1000) == 0) {
                gDLL_28_ScreenFade->vtbl->fade_reversed(30, SCREEN_FADE_BLACK);
                sAutoReverseTimer = 0.0f;
            }
        }

        sFadeAlpha += sFadeSpeed * delayFloat;
        if (sFadeAlpha < 0.0f) {
            sFadeAlpha = 0.0f;
            sIsComplete = 1;
            return;
        }

        if (sFadeAlpha > 255.0f) {
            sFadeAlpha = 255.0f;
            sIsComplete = 1;
            sAutoReverseTimer += delayFloat;
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
#endif

void screen_fade_fade(s32 param1, s32 param2) {
    if (sFadeSpeed <= 0.0f || sFadeAlpha == 255.0f) {
        sFadeAlpha = 0.0f;
    }

    sFadeSpeed = 256.0f / param1;
    sAutoReverseTimer = 0.0f;
    sFadeType = param2;
    sDelayTimer = 0;
}

void screen_fade_fade_reversed(s32 param1, s32 param2) {
    if (sFadeSpeed >= 0.0f || sFadeAlpha == 0.0f) {
        sFadeAlpha = 255.0f;
    }

    sFadeSpeed = -256.0f / param1;
    sAutoReverseTimer = 0.0f;
    sFadeType = param2;
    sDelayTimer = 1;
}

void screen_fade_func_3AC(s32 param1, s32 param2, f32 param3) {
    sFadeAlpha = param3 * 255.0f;
    sFadeSpeed = -(param3 * 256.0f) / param1;
    sAutoReverseTimer = 0.0f;
    sFadeType = param2;
    sDelayTimer = 1;
}

f32 screen_fade_get_progress() {
    return sFadeAlpha;
}

s32 screen_fade_is_complete() {
    return sIsComplete;
}

/*static */ void screen_fade_draw_simple_black(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
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

/*static */ void screen_fade_draw_simple(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, s32 red, s32 green, s32 blue) {
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

// https://decomp.me/scratch/WVuzl
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/28_screen_fade/screen_fade_draw_radial.s")
