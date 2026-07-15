#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "PR/os.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/28_screen_fade.h"
#include "game/gamebits.h"
#include "game/gametexts.h"
#include "sys/fonts.h"
#include "sys/map_enums.h"
#include "sys/vi.h"
#include "sys/gfx/textable.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/menu.h"
#include "dll.h"
#include "sys/rcp.h"
#include "types.h"

#include "prevent_bss_reordering.h"

static u8 data_0 = 0;
static u8 dExpansionPakMissing = 0;
static Texture *dTexN64Logo = NULL;
static Texture *dTexN64LogoShadow = NULL;
static Texture *dTexExpansionPak = NULL;
static u32 _data_14 = 0x02040103;

static f32 bss_0;
static s8 bss_4;
static s8 bss_5;
static s8 bss_6;
static f32 bss_8;
static f32 bss_C;
static f32 bss_10;
static GameTextChunk *splashGametext;
static Texture *sTexDolbyBig;
static Texture *sTexDolbySmall;

void dll_60_ctor(void *dll) {
    fontLoad(FONT_FUN_FONT);

    if (osMemSize != 0x800000) {
        dExpansionPakMissing = TRUE;
        dTexExpansionPak = texLoadTexture(TEXTABLE_2DA_ExpansionPak);
        splashGametext = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_0F3_Expansion_Pak_Needed);
    } else {
        bss_4 = 0;
        bss_0 = 0.0f;
        fontLoad(FONT_DINO_MEDIUM_FONT_IN);
        fontLoad(FONT_DINO_MEDIUM_FONT_OUT);
        mainLoadFrontend();
        dExpansionPakMissing = FALSE;
        mainChangeMap(MAP_FRONT_END2, 0, PLAYER_NONE, -1);
        gDLL_5_AMSEQ->vtbl->set(NULL, 0x20, 0, 0x2f, 0);
        mainSetBits(BIT_Menus_Selection_Blocked, 1);
        gDLL_2_Camera->vtbl->set_letterbox_goal(30, 1);
        dTexN64Logo = texLoadTexture(TEXTABLE_2D2_N64LogoFull);
        dTexN64LogoShadow = texLoadTexture(TEXTABLE_2D3_N64LogoFull_Shadow);
        sTexDolbyBig = texLoadTexture(TEXTABLE_2DB_DolbySurround);
        sTexDolbySmall = texLoadTexture(TEXTABLE_2DC_DolbySurround_SmallLogo);
        splashGametext = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_0F2_Splash_Screen);
        gDLL_29_Gplay->vtbl->get_game_options(); // ignoring return value
    }

    bss_6 = 0;
    bss_5 = 2;
}

void dll_60_dtor(void *dll) {
    if (dExpansionPakMissing == TRUE) {
        texFreeTexture(dTexExpansionPak);
    } else {
        texFreeTexture(dTexN64Logo);
        texFreeTexture(dTexN64LogoShadow);
        texFreeTexture(sTexDolbyBig);
        texFreeTexture(sTexDolbySmall);
    }

    mmFree(splashGametext);
}

s32 dll_60_update1(void) {
    if (dExpansionPakMissing == TRUE) {
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
            menuSet(MENU_RAREWARE);
        }

        return 0;
    } 
}

void dll_60_update2(void) { }

void dll_60_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    u8 dColourTint[4] = {0xe4, 0x9c, 0x44, 0xff}; //Colour multiplier for the greyscale background
    u8 _stackPad[4];
    f32 var5;
    s32 fontYSpacing;

    fontWindowSetCoords(2, 0, 0, 
        (GET_VIDEO_WIDTH(viGetCurrentSize())) - 50,
        (GET_VIDEO_HEIGHT(viGetCurrentSize())));
    
    fontWindowFlushStrings(2);
    fontWindowUseFont(2, FONT_FUN_FONT);
    rcpClearScreen(gdl, mtxs, CLEAR_COLOR);

    if (dExpansionPakMissing == TRUE) {
        fontYSpacing = fontGetYSpacing(FONT_FUN_FONT);

        fontWindowSetTextColour(2, 183, 139, 97, 255, 255);
        fontWindowAddStringXY(2, 320, 198,                splashGametext->strings[0], 1, ALIGN_TOP_CENTER);
        fontWindowAddStringXY(2, 320, 272,                splashGametext->strings[1], 1, ALIGN_TOP_CENTER);
        fontWindowAddStringXY(2, 320, fontYSpacing + 272, splashGametext->strings[2], 1, ALIGN_TOP_CENTER);
        fontWindowAddStringXY(2, 320, 356,                splashGametext->strings[3], 1, ALIGN_TOP_CENTER);
        fontWindowAddStringXY(2, 320, fontYSpacing + 356, splashGametext->strings[4], 1, ALIGN_TOP_CENTER);
        rcpScreenFullWrite(gdl, dTexExpansionPak, 0xfd, 0x42, 0, 0, 0xff, SCREEN_WRITE_TRANSLUCENT);
    } else {
        gDLL_76->vtbl->func2(gdl, mtxs);

        if (bss_0 < 240.0f) {
            fontWindowEnableWordwrap(2);
            fontWindowSetTextColour(2, 183, 139, 97, 255, 255);
            fontWindowAddStringXY(2, 57, 54,  splashGametext->strings[0], 1, ALIGN_TOP_LEFT);
            fontWindowAddStringXY(2, 179, 88, splashGametext->strings[1], 1, ALIGN_TOP_LEFT);
            fontWindowAddStringXY(2, 57, 172, splashGametext->strings[2], 1, ALIGN_TOP_LEFT);
            fontWindowAddStringXY(2, 57, 222, splashGametext->strings[3], 1, ALIGN_TOP_LEFT);
            fontWindowAddStringXY(2, 57, 381, splashGametext->strings[4], 1, ALIGN_TOP_LEFT);
            rcpScreenFullWrite(gdl, sTexDolbyBig, 0x3a, 0x65, 0, 0, 0xff, SCREEN_WRITE_TRANSLUCENT);
            rcpScreenFullWrite(gdl, sTexDolbySmall, 0x16d, 0x68, 0, 0, 0xff, SCREEN_WRITE_TRANSLUCENT);
        }

        if (bss_0 > 240.0f && data_0 == 0) {
            data_0 = 1;
            gDLL_76->vtbl->func0(0x41a, 0x2c9, 0xf, -470, 0xffffffee, 1.0f, (u32*)&dColourTint, 0xe002, 5, 1, 0, 0);
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
            
            rcpScreenFullWrite(gdl, dTexN64LogoShadow, 0x5d, 0xc6, 0, 0, (s16)(255.0f * var5), SCREEN_WRITE_TRANSLUCENT);
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

            rcpScreenFullWrite(gdl, dTexN64Logo, 0x5f, 200, 0, 0, (u32)(255.0f * var5) & 0xFF, SCREEN_WRITE_TRANSLUCENT);
        }
    }

    fontWindowDraw(gdl, NULL, NULL, 2);
    bss_5 -= 1;
    if (bss_5 < 0) {
        bss_5 = 0;
    }
}

static void dll_60_func_D0C() { }
