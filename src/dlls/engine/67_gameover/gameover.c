#include "common.h"
#include "game/gametexts.h"
#include "macros.h"
#include "sys/fonts.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/74_picmenu.h"
#include "dlls/objects/210_player.h"
#include "sys/menu.h"

#define NONE -1

typedef enum {
    GAMEOVER_STRING0_CONTINUE,
    GAMEOVER_STRING1_PLAYING,
    GAMEOVER_STRING2_GAME_OVER,
    GAMEOVER_STRING3_DO_YOU_WISH,
    GAMEOVER_STRING4_TO_SAVE,
    GAMEOVER_STRING5_GAME_SAVED,
    GAMEOVER_STRING6_USE_DUSTER,
    GAMEOVER_STRING7_YES,
    GAMEOVER_STRING8_NO
} GameOver_Strings;

/*0x0*/ static PicMenuItem data_0[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 160, /*textY*/ 40, /*innerWidth*/ 0, 
        /*itemX*/ 160, /*itemY*/ 20, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),
    
    /*1*/ NEW_PICMENU_ITEM( 
        /*textX*/ 160, /*textY*/ 192, /*innerWidth*/ 0, 
        /*itemX*/ 160, /*itemY*/ 172, /*textureID*/ NONE, /*outerWidth*/ 100,
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 0x40 | PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ 2, /*rightLink*/ 2, /*overrideWith*/ NONE
    ),
    
    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 160, /*textY*/ 192, /*innerWidth*/ 0, 
        /*itemX*/ 160, /*itemY*/ 172, /*textureID*/ NONE, /*outerWidth*/ 100,
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 0x40 | PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ 1, /*rightLink*/ 1, /*overrideWith*/ NONE
    ), 
    
    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ 160, /*textY*/ 192, /*innerWidth*/ 0, 
        /*itemX*/ 160, /*itemY*/ 172, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,  
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ 1
    )
};

/*0x0*/ static GameTextChunk* bss_0;
/*0x4*/ static Texture* bss_4; //Duster counter box
/*0x8*/ static s16 bss_8;
/*0xC*/ static f32 bss_C;
/*0x10*/ static f32 bss_10;
/*0x14*/ static u8 bss_14;
/*0x15*/ static u8 bss_15;
/*0x16*/ static s8 bss_16;
/*0x17*/ static s8 bss_17;
/*0x18*/ static char bss_18[0x8];

// offset: 0x0 | ctor
void dll_67_ctor(void* dll) {
    PlayerStats* stats;

    stats = gDLL_29_Gplay->vtbl->get_player_stats();
    
    bss_4 = tex_load_deferred(TEXTABLE_310);

    font_load(FONT_FUN_FONT);

    bss_0 = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_200_Death_Screen);
    
    //@debug: set Duster count to 1 when player dies
    stats->dusters = 1;

    if ((func_80010028() == 1) && (stats->dusters > 0)) {
        bss_14 = 0;
        data_0[0].flags |= PICMENU_INTANGIBLE;
    } else {
        bss_14 = 2;
    }
    
    data_0[0].text = bss_0->strings[GAMEOVER_STRING2_GAME_OVER];
    data_0[1].text = bss_0->strings[GAMEOVER_STRING7_YES];
    data_0[2].text = bss_0->strings[GAMEOVER_STRING8_NO];
    data_0[3].overrideWith = 1;

    gDLL_74_Picmenu->vtbl->set_items(data_0, 
        ARRAYCOUNT(data_0), 
        3, 
        NULL, 
        0, 0, 
        0xB7, 0x8B, 0x61, 
        0xFF, 0xD7, 0x3D
    );
}

// offset: 0x198 | dtor
void dll_67_dtor(void *dll) {
    tex_free(bss_4);
    font_unload(FONT_FUN_FONT);
    mmFree(bss_0);
}

// offset: 0x200 | func: 0 | export: 0
s32 dll_67_func_200(void) {
    s32 action; //2C
    s32 delay;
    PlayerStats* stats;
    Object* player; //20
    s8 prevTimer;
    s32 selectedIdx;
    s32 index;

    player = get_player();

    prevTimer = bss_16;
    
    delay = gUpdateRate;
    if (delay > 3) {
        delay = 3;
    }
    
    if (bss_16 > 0) {
        bss_16 -= delay;
    }    
    
    if (bss_15 == 1) {
        if (gDLL_28_ScreenFade->vtbl->is_complete()) {
            gDLL_29_Gplay->vtbl->start_loaded_game();
        }
        return 0;
    }
    
    if (bss_15 == 3) {
        return 0;
    }
    
    if (bss_15 == 2) {
        if ((prevTimer >= 13) && (bss_16 < 13)) {
            func_80013FB4();
        } else if (bss_16 <= 0) {
            menu_set(MENU_POST);
        }
        
        if (bss_16 < 13) {
            return 1;
        } else {
            return 0;
        }
    }
    
    action = gDLL_74_Picmenu->vtbl->update();
    if (action != PICMENU_ACTION_NONE) {
        selectedIdx = gDLL_74_Picmenu->vtbl->get_selected_item();
        switch (bss_14) {
        case 1:
            if (action == PICMENU_ACTION_SELECT) {
                if (gDLL_74_Picmenu->vtbl->get_item_override(selectedIdx) == 1) {
                    gDLL_28_ScreenFade->vtbl->fade(0x1E, 1);
                    bss_15 = 1;
                } else {
                    gDLL_28_ScreenFade->vtbl->fade(0x1E, 1);
                    bss_15 = 2;
                    bss_16 = 0x2D;
                }
            }
            break;
        case 2:
            if (action == PICMENU_ACTION_SELECT) {
                if (gDLL_74_Picmenu->vtbl->get_item_override(selectedIdx) == 1) {
                    bss_10 = 0.0f;
                    bss_14 = 3;

                    for (index = 0; index < 4; index++){
                        data_0[index].flags |= PICMENU_INTANGIBLE;
                    }
                    
                    gDLL_74_Picmenu->vtbl->set_item_override(3, 1);
                    gDLL_74_Picmenu->vtbl->update_flags(data_0);
                } else {
                    bss_14 = 1;
                    gDLL_74_Picmenu->vtbl->set_item_override(3, 1);
                }
            }
            break;
        case 0:
            if (action == PICMENU_ACTION_SELECT) {
                if (gDLL_74_Picmenu->vtbl->get_item_override(selectedIdx) == 1) {
                    stats = gDLL_29_Gplay->vtbl->get_player_stats();
    
                    //Remove one Duster
                    stats->dusters--;

                    ((DLL_210_Player*)player->dll)->vtbl->set_health(
                        player, 
                        ((DLL_210_Player*)player->dll)->vtbl->get_health_max(player)
                    );
                    ((DLL_210_Player*)player->dll)->vtbl->set_magic(
                        player, 
                        ((DLL_210_Player*)player->dll)->vtbl->get_magic_max(player)
                    );
                    bss_15 = 3;
                } else {
                    bss_14 = 2;
                    data_0[0].flags &= ~PICMENU_INTANGIBLE;
                    gDLL_74_Picmenu->vtbl->set_item_override(3, 1);
                    gDLL_74_Picmenu->vtbl->update_flags(data_0);
                }
            }
            break;
        }
    }
    
    if (bss_14 == 3) {
        if (bss_10 == 0.0f) {
            gDLL_29_Gplay->vtbl->save_game();
        }
        
        bss_10 += gUpdateRateF;
        if (bss_10 >= 120.0f) {
            bss_14 = 1;
            
            for (index = 0; index < 4; index++){
                data_0[index].flags &= ~PICMENU_INTANGIBLE;
            }
            
            gDLL_74_Picmenu->vtbl->update_flags(data_0);
        }
    }
    
    bss_C += gUpdateRateF;
    if (bss_C >= 60.0f) {
        bss_8 += gUpdateRate * 4;
        if (bss_8 > 140) {
            bss_8 = 140;
        }
        bss_C = 60.0f;
    }

    return 0;
}

// offset: 0x868 | func: 1 | export: 1
void dll_67_func_868(void) {
    return;
}

#define SHADOW_OPACITY(opacity) ((((u8)opacity) * 0x96) >> 8)

// offset: 0x870 | func: 2 | export: 2
void dll_67_func_870(Gfx** gfx, Mtx** mtx, Vertex** vtx) {
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;
    f32 opacity;
    PlayerStats* stats;

    stats = gDLL_29_Gplay->vtbl->get_player_stats();
    
    if ((bss_15 == 2) && (bss_16 < 0xB)) {
        return;
    }

    opacity = ((f32)bss_8 / 140.0f) * 255.0f;
    
    viewport_get_full_rect(&ulx, &uly, &lrx, &lry);
    gDPSetCombineMode(*gfx, G_CC_PRIMITIVE, G_CC_PRIMITIVE); 
    dl_apply_combine(gfx);
    gDPSetOtherMode(*gfx, 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | 
        G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | 
        G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2);
    dl_apply_other_mode(gfx);
    dl_set_prim_color(gfx, 0, 0, 0, bss_8);
    gDPFillRectangle((*gfx)++, ulx, uly, lrx, lry);
    gDLBuilder->needsPipeSync = TRUE;
    
    font_window_set_coords(1, 0, 0, 
        GET_VIDEO_WIDTH(vi_get_current_size()), 
        GET_VIDEO_HEIGHT(vi_get_current_size())
    );
    font_window_flush_strings(1);
    font_window_use_font(1, 0);
    font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, opacity);
    
    switch (bss_14) {
    case 1:
        font_window_add_string_xy(1, 0xA0, 0x61, bss_0->strings[0], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, 0xA0, 0x7B, bss_0->strings[1], 1, ALIGN_TOP_CENTER);
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, SHADOW_OPACITY(opacity));
        font_window_add_string_xy(1, 0x9F, 0x60, bss_0->strings[0], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, 0x9F, 0x7A, bss_0->strings[1], 1, ALIGN_TOP_CENTER);
        break;
    case 2:
        font_window_add_string_xy(1, 0xA0, 0x61, bss_0->strings[3], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, 0xA0, 0x7B, bss_0->strings[4], 1, ALIGN_TOP_CENTER);
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, SHADOW_OPACITY(opacity));
        font_window_add_string_xy(1, 0x9F, 0x60, bss_0->strings[3], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, 0x9F, 0x7A, bss_0->strings[4], 1, ALIGN_TOP_CENTER);
        break;
    case 3:
        font_window_add_string_xy(1, 0xA0, 0x73, bss_0->strings[5], 1, ALIGN_TOP_CENTER);
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, SHADOW_OPACITY(opacity));
        font_window_add_string_xy(1, 0x9F, 0x72, bss_0->strings[5], 1, ALIGN_TOP_CENTER);
        break;
    case 0:
        font_window_add_string_xy(1, 0x12, 0x12, bss_0->strings[6], 1, ALIGN_TOP_LEFT);
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, SHADOW_OPACITY(opacity));
        font_window_add_string_xy(1, 0x11, 0x11, bss_0->strings[6], 1, ALIGN_TOP_LEFT);
        rcp_screen_full_write(gfx, bss_4, 0xF, 0x24, 0, 0, opacity, 0);
        font_window_use_font(1, 1);
        sprintf(bss_18, "%02d", stats->dusters);
        font_window_add_string_xy(1, 0x46, 0x3A, bss_18, 1, ALIGN_TOP_RIGHT);
        break;
    }
    
    gDLL_74_Picmenu->vtbl->set_opacity(opacity);
    gDLL_74_Picmenu->vtbl->draw(gfx);
    font_window_draw(gfx, NULL, NULL, 1);
}