#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "PR/os.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/28_screen_fade.h"
#include "game/gamebits.h"
#include "game/gametexts.h"
#include "sys/fonts.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/menu.h"
#include "dll.h"
#include "functions.h"
#include "types.h"

static u8 data_0 = 0;
static u8 data_4 = 0;
static Texture *data_8 = NULL;
static Texture *data_C = NULL;
static Texture *data_10 = NULL;
static u32 _data_14 = 0x02040103;
static u32 data_18 = 0xe49c44ff;

static f32 bss_0;
static s8 bss_4;
static s8 bss_5;
static s8 bss_6;
static f32 bss_8;
static f32 bss_C;
static f32 bss_10;
static GameTextChunk *splashGametext;
static Texture *bss_18;
static Texture *bss_1C;

void dll_60_ctor(void *self) {
    font_load(FONT_FUN_FONT);

    if (osMemSize != 0x800000) {
        data_4 = 1;
        data_10 = queue_load_texture_proxy(0x2da);
        splashGametext = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_0F3_Expansion_Pak_Needed);
    } else {
        bss_4 = 0;
        bss_0 = 0.0f;
        font_load(FONT_DINO_MEDIUM_FONT_IN);
        font_load(FONT_DINO_MEDIUM_FONT_OUT);
        func_80014B1C();
        data_4 = 0;
        func_800141A4(1, 0, PLAYER_NONE, -1);
        gDLL_5_AMSEQ->vtbl->func0(NULL, 0x20, 0, 0x2f, 0);
        main_set_bits(BIT_44F, 1);
        gDLL_2_Camera->vtbl->func21(30, 1);
        data_8 = queue_load_texture_proxy(0x2d2);
        data_C = queue_load_texture_proxy(0x2d3);
        bss_18 = queue_load_texture_proxy(0x2db);
        bss_1C = queue_load_texture_proxy(0x2dc);
        splashGametext = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_0F2_Splash_Screen);
        gDLL_29_Gplay->vtbl->get_game_options(); // ignoring return value
    }

    bss_6 = 0;
    bss_5 = 2;
}

void dll_60_dtor(void *self) {
    if (data_4 == 1) {
        texture_destroy(data_10);
    } else {
        texture_destroy(data_8);
        texture_destroy(data_C);
        texture_destroy(bss_18);
        texture_destroy(bss_1C);
    }

    mmFree(splashGametext);
}

s32 dll_60_update1() {
    if (data_4 == 1) {
        return 0;
    } else {
        bss_0 += gUpdateRateF;

        if (bss_6 == 0 && bss_0 > 205.0f) {
            gDLL_28_ScreenFade->vtbl->fade(30, SCREEN_FADE_BLACK);
            bss_6 = 1;
        } else if (bss_6 == 1 && bss_0 > 245.0f) {
            gDLL_28_ScreenFade->vtbl->fade_reversed(30, SCREEN_FADE_BLACK);
            bss_6 = 2;
        }

        if (data_0 > 1) {
            bss_8 -= gUpdateRateF;
        }
        if (data_0 > 2) {
            bss_C -= gUpdateRateF;
        }
        if (data_0 > 3) {
            bss_10 -= gUpdateRateF;
        }

        if (bss_0 > 720.0f) {
            bss_4 = 1;
        }

        if (bss_4 != 0) {
            bss_4 = 0;
            bss_0 = 0.0f;
            menu_set(MENU_RAREWARE);
        }

        return 0;
    } 
}

void dll_60_update2() { }

void dll_60_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    u32 local4;
    u8 _stackPad[4];
    f32 var5;
    s32 fontYSpacing;

    local4 = data_18;

    font_window_set_coords(2, 0, 0, 
        (GET_VIDEO_WIDTH(vi_get_current_size())) - 50,
        (GET_VIDEO_HEIGHT(vi_get_current_size())));
    
    font_window_flush_strings(2);
    font_window_use_font(2, FONT_FUN_FONT);
    func_80037A14(gdl, mtxs, 1);

    if (data_4 == 1) {
        fontYSpacing = font_get_y_spacing(FONT_FUN_FONT);

        font_window_set_text_colour(2, 183, 139, 97, 255, 255);
        font_window_add_string_xy(2, 320, 198,                splashGametext->strings[0], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(2, 320, 272,                splashGametext->strings[1], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(2, 320, fontYSpacing + 272, splashGametext->strings[2], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(2, 320, 356,                splashGametext->strings[3], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(2, 320, fontYSpacing + 356, splashGametext->strings[4], 1, ALIGN_TOP_CENTER);
        func_8003825C(gdl, data_10, 0xfd, 0x42, 0, 0, 0xff, 0);
    } else {
        gDLL_76->vtbl->func2(gdl, mtxs);

        if (bss_0 < 240.0f) {
            font_window_enable_wordwrap(2);
            font_window_set_text_colour(2, 183, 139, 97, 255, 255);
            font_window_add_string_xy(2, 57, 54,  splashGametext->strings[0], 1, ALIGN_TOP_LEFT);
            font_window_add_string_xy(2, 179, 88, splashGametext->strings[1], 1, ALIGN_TOP_LEFT);
            font_window_add_string_xy(2, 57, 172, splashGametext->strings[2], 1, ALIGN_TOP_LEFT);
            font_window_add_string_xy(2, 57, 222, splashGametext->strings[3], 1, ALIGN_TOP_LEFT);
            font_window_add_string_xy(2, 57, 381, splashGametext->strings[4], 1, ALIGN_TOP_LEFT);
            func_8003825C(gdl, bss_18, 0x3a, 0x65, 0, 0, 0xff, 0);
            func_8003825C(gdl, bss_1C, 0x16d, 0x68, 0, 0, 0xff, 0);
        }

        if (bss_0 > 240.0f && data_0 == 0) {
            data_0 = 1;
            gDLL_76->vtbl->func0(0x41a, 0x2c9, 0xf, -470, 0xffffffee, 1.0f, &local4, 0xe002, 5, 1, 0, 0);
        }

        if (bss_0 > 280.0f && data_0 == 1) {
            data_0 = 3;
            bss_8 = 425.0f;
        }
        
        if (bss_0 > 450.0f && data_0 == 3) {
            data_0 = 4;
            bss_10 = 200.0f;
        }
        
        if (data_0 > 3) {
            if (bss_10 <= 100.0f) {
                var5 = 1.0f - ((100.0f - bss_10) / 100.0f);
            } else {
                var5 = 1.0f - ((bss_10 - 100.0f) / 100.0f);
            }

            if (var5 < 0.0f) {
                var5 = 0.0f;
            } else if (var5 > 1.0f) {
                var5 = 1.0f;
            }
            
            func_8003825C(gdl, data_C, 0x5d, 0xc6, 0, 0, (s16)(255.0f * var5), 0);
        }

        if (data_0 >= 2) {
            if (bss_8 <= 106.0f) {
                var5 = 1.0f - ((106.0f - bss_8) / 106.0f);
            } else {
                var5 = 1.0f - ((bss_8 - 319.0f) / 106.0f);
            }

            if (var5 < 0.0f) {
                var5 = 0.0f;
            } else if (var5 > 1.0f) {
                var5 = 1.0f;
            }

            func_8003825C(gdl, data_8, 0x5f, 200, 0, 0, (u32)(255.0f * var5) & 0xFF, 0);
        }
    }

    font_window_draw(gdl, NULL, NULL, 2);
    bss_5 -= 1;
    if (bss_5 < 0) {
        bss_5 = 0;
    }
}

static void dll_60_func_D0C() { }
