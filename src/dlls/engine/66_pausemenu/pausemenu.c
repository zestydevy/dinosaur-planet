#include "PR/os.h"
#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/74_picmenu.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/21_gametext.h"
#include "functions.h"
#include "sys/camera.h"
#include "sys/controller.h"
#include "sys/fonts.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/map.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/menu.h"

#include "dlls/engine/66_pausemenu.h"
#include "types.h"

/*0x0*/ static const char formatCompletionPercentage[] = "%3d%%";
/*0x8*/ static const char formatGameplayTime[] = "%3d:%02d:%02d";
/*0x18*/ static const char formatSpellStoneCount[] = "%1d";
/*0x1C*/ static const char formatDusterCount[] = "%1d";
/*0x20*/ static const char formatSpiritCount[] = "%1d";

/** UI elements for "Continue" and "Save" options */
static PicMenuItem pauseMenuItems[2] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 161, /*textY*/ 86, /*innerWidth*/ 0, 
        /*itemX*/ 161, /*itemY*/ 86, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT | PICMENU_ALWAYS_REDRAW, 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ -1, /*downLink*/ 1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 161, /*textY*/ 109, /*innerWidth*/ 0, 
        /*itemX*/ 161, /*itemY*/ 86, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT | PICMENU_ALWAYS_REDRAW, 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ 0, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    )
};

static PicMenuSounds pauseMenuSounds = {
    SFX_PAUSE_MENU_MOVE,
    NO_SOUND,
    SFX_PAUSE_MENU_BACK
};

/*0x0*/ static GameTextChunk* gametext;
/*0x4*/ static Texture* textureSpellStone;
/*0x8*/ static Texture* textureDuster;
/*0xC*/ static Texture* textureSpirit;
/*0x10*/ static char gameplayTime[0xa];
/*0x1A*/ static char spiritCount[0x2];
/*0x1C*/ static char spellStoneCount[0x2];
/*0x1E*/ static char dusterCount[0x2];
/*0x20*/ static char _bss_20[0x4];
/*0x24*/ static char completionPercentage[4];
/*0x28*/ static char _bss_28[0x2];
/*0x2A*/ static u8 pauseScreenState;
/*0x2B*/ static s8 gameSavedMessageTimer;
/*0x2C*/ static s16 pauseMenuOpacity;

// offset: 0x0 | ctor
void n_pausemenu_ctor(s32 arg0) {
    GameTextChunk* temp_v0;

    textureSpellStone = queue_load_texture_proxy(0x31E);
    textureDuster = queue_load_texture_proxy(0x310);
    textureSpirit = queue_load_texture_proxy(0x31F);
    gametext = gDLL_21_Gametext->vtbl->get_chunk(0);
    pauseMenuItems[0].text = gametext->strings[0];
    pauseMenuItems[1].text = gametext->strings[1];
    font_load(FONT_FUN_FONT);
    gDLL_74_Picmenu->vtbl->set_items(pauseMenuItems, 2, 0, &pauseMenuSounds, 0, 0, 0x5B, 0x45, 0x30, 0xFF, 0xD7, 0x3D);
    pauseMenuOpacity = 0;
    pauseScreenState = 0;
}

// offset: 0x14C | dtor
void n_pausemenu_dtor(s32 arg0) {
    texture_destroy(textureSpellStone);
    texture_destroy(textureDuster);
    texture_destroy(textureSpirit);
    mmFree((void*)gametext);
    font_unload(FONT_FUN_FONT);
    gDLL_74_Picmenu->vtbl->clear_items();
}

// offset: 0x1F8 | func: 0 | export: 0
s32 n_pausemenu_update(void) {
    s32 action;
    s32 index;
    s32 selectedItem;
    
    if (pauseScreenState == PAUSE_MENU_MAIN) {

        action = gDLL_74_Picmenu->vtbl->update();
        selectedItem = gDLL_74_Picmenu->vtbl->get_selected_item();
        
        if (action == PICMENU_ACTION_SELECT) {
            if (selectedItem == 0) {
                gDLL_6_AMSFX->vtbl->play_sound(0, SOUND_B73, MAX_VOLUME, 0, 0, 0, 0);
                menu_set(MENU_GAMEPLAY);
                unpause();
                set_button_mask(0, A_BUTTON | B_BUTTON);
            } else {
                gDLL_6_AMSFX->vtbl->play_sound(0, SOUND_B72, MAX_VOLUME, 0, 0, 0, 0);
                gameSavedMessageTimer = 0;
                pauseScreenState = 1;
                
                for (index = 0; index < 2; index++){
                    pauseMenuItems[index].flags |= 0x1000;
                }
                
                gDLL_74_Picmenu->vtbl->update_flags(pauseMenuItems);
            }
        } else if (action == PICMENU_ACTION_BACK) {
            menu_set(MENU_GAMEPLAY);
            unpause();
            set_button_mask(0, A_BUTTON | B_BUTTON);
        }

    } else if (pauseScreenState == PAUSE_MENU_GAME_SAVED) {

        if (gameSavedMessageTimer == 0) {
            gDLL_29_Gplay->vtbl->save_game();
        }

        gameSavedMessageTimer += delayFloat;
        if (gameSavedMessageTimer >= 120.0f) {
            pauseScreenState = 0;

            for (index = 0; index < 2; index++){
                pauseMenuItems[index].flags &= ~0x1000;
            }
            
            gDLL_74_Picmenu->vtbl->update_flags(pauseMenuItems);
            gDLL_74_Picmenu->vtbl->set_selected_item(0);
        }
    }
    
    //Gradually fade in BG overlay (and the UI elements, which depend on this value)
    pauseMenuOpacity += delayByte * 8;
    if (pauseMenuOpacity > BG_OVERLAY_MAX_OPACITY) {
        pauseMenuOpacity = BG_OVERLAY_MAX_OPACITY;
    }
    
    return 0;
}

// offset: 0x524 | func: 1 | export: 1
void n_pausemenu_func_524(void){
}

// offset: 0x52C | func: 2 | export: 2
void n_pausemenu_draw(Gfx** gfx, Mtx** mtx, Vertex** vtx) {
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;
    s16 hours;
    s16 minutes;
    s16 seconds;
    f32 opacity_main;
    s32 opacity_drop_shadow;

    //Draw background and dimming overlay
    func_80002130(&ulx, &uly, &lrx, &lry);
    gDPSetCombineMode(*gfx, G_CC_PRIMITIVE, G_CC_PRIMITIVE);    
    dl_apply_combine(gfx);
    gDPSetOtherMode(*gfx, 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | 
        G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | 
        G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2);
    dl_apply_other_mode(gfx);
    dl_set_prim_color(gfx, BG_OVERLAY_R, BG_OVERLAY_G, BG_OVERLAY_B, pauseMenuOpacity);
    gDPFillRectangle((*gfx)++, ulx, uly, lrx, lry);
    gDLBuilder->needsPipeSync = TRUE;
    
    font_window_set_coords(1, 0, 0, get_some_resolution_encoded() & 0xFFFF, get_some_resolution_encoded() >> 0x10);
    font_window_flush_strings(1);
    opacity_main = ((f32) pauseMenuOpacity / BG_OVERLAY_MAX_OPACITY) * 255.0f;

    gDLL_74_Picmenu->vtbl->set_opacity((u8)opacity_main);
    gDLL_74_Picmenu->vtbl->draw(gfx);

    //Draw icons and text
    font_window_set_text_colour(1, BEIGE_R, BEIGE_G, BEIGE_B, BEIGE_A, opacity_main);
    font_window_use_font(1, FONT_FUN_FONT);

    switch (pauseScreenState){
        case PAUSE_MENU_GAME_SAVED:
            //Draw "Game Saved" message
            font_window_add_string_xy(1, GAME_SAVED_X, GAME_SAVED_Y, gametext->strings[4], 1, ALIGN_TOP_CENTER);
    
            //Draw "Game Saved" message drop-shadow
            font_window_set_text_colour(1, DROP_SHADOW_R, DROP_SHADOW_G, DROP_SHADOW_B, DROP_SHADOW_A, ((u8)opacity_main * DROP_SHADOW_MULTIPLIER) >> 8);
            font_window_add_string_xy(1, GAME_SAVED_X + DROP_SHADOW_X, GAME_SAVED_Y + DROP_SHADOW_Y, gametext->strings[4], 1, ALIGN_TOP_CENTER);
            break;
        default:   
            //Draw icons
            func_8003825C(gfx, textureSpellStone, SPELLSTONE_X, SPELLSTONE_Y, 0, 0, opacity_main, 0);
            func_8003825C(gfx, textureDuster, DUSTER_X, DUSTER_Y, 0, 0, opacity_main, 0);
            func_8003825C(gfx, textureSpirit, SPIRIT_X, SPIRIT_Y, 0, 0, opacity_main, 0);
            
            //Draw completion percentage
            sprintf(completionPercentage, formatCompletionPercentage, gDLL_30_Task->vtbl->get_completion_percentage());
            font_window_add_string_xy(1, COMPLETION_X, COMPLETION_Y, completionPercentage, 1, ALIGN_TOP_CENTER);
    
            //@bug: forgot to add a drop-shadow for the completion percentage
    
            //Draw gameplay time
            gDLL_7_Newday->vtbl->convert_ticks_to_real_time(gDLL_29_Gplay->vtbl->get_time_played(), &hours, &minutes, &seconds);
            sprintf(gameplayTime, formatGameplayTime, hours, minutes, seconds);
            font_window_add_string_xy(1, GAME_TIME_X, GAME_TIME_Y, gameplayTime, 1, ALIGN_TOP_CENTER);
            
            //Draw gameplay time drop-shadow
            opacity_drop_shadow = ((u8)opacity_main * DROP_SHADOW_MULTIPLIER) >> 8;
            font_window_set_text_colour(1, DROP_SHADOW_R, DROP_SHADOW_G, DROP_SHADOW_B, DROP_SHADOW_A, opacity_drop_shadow);
            font_window_add_string_xy(1, GAME_TIME_X + DROP_SHADOW_X, GAME_TIME_Y + DROP_SHADOW_Y, gameplayTime, 2, ALIGN_TOP_CENTER);
    
            //Change font
            font_window_use_font(1, FONT_DINO_SUBTITLE_FONT_1);
    
            //@bug: counters weren't yet connected up to anything, and always read as 0
            
            //Draw SpellStone count
            sprintf(spellStoneCount, formatSpellStoneCount, 0);
            font_window_add_string_xy(1, SPELLSTONE_X + 41, SPELLSTONE_Y + 20, spellStoneCount, 1, ALIGN_TOP_LEFT);
    
            //Draw Duster count
            sprintf(dusterCount, formatDusterCount, 0);
            font_window_add_string_xy(1, DUSTER_X + 55, DUSTER_Y + 22, dusterCount, 1, ALIGN_TOP_RIGHT);
    
            //Draw Spirit count
            sprintf(spiritCount, formatSpiritCount, 0);
            font_window_add_string_xy(1, SPIRIT_X + 16, SPIRIT_Y + 19, spiritCount, 1, ALIGN_TOP_LEFT);
    
            //Draw "Game Time" and "Complete" strings
            font_window_set_text_colour(1, BEIGE_R, BEIGE_G, BEIGE_B, BEIGE_A, opacity_main);
            font_window_add_string_xy(1, COMPLETION_CAPTION_X, COMPLETION_CAPTION_Y, gametext->strings[3], 2, ALIGN_TOP_CENTER);
            font_window_add_string_xy(1, GAME_TIME_CAPTION_X, GAME_TIME_CAPTION_Y, gametext->strings[2], 2, ALIGN_TOP_CENTER);
            
            //Draw "Game Time" and "Complete" strings' drop-shadows
            font_window_set_text_colour(1, DROP_SHADOW_R, DROP_SHADOW_G, DROP_SHADOW_B, DROP_SHADOW_A, opacity_drop_shadow);
            font_window_add_string_xy(1, COMPLETION_CAPTION_X + DROP_SHADOW_X, COMPLETION_CAPTION_Y + DROP_SHADOW_Y, gametext->strings[3], 2, ALIGN_TOP_CENTER);
            font_window_add_string_xy(1, GAME_TIME_CAPTION_X + DROP_SHADOW_X, GAME_TIME_CAPTION_Y + DROP_SHADOW_Y, gametext->strings[2], 2, ALIGN_TOP_CENTER);
            break;
    }

    font_window_draw(gfx, 0, 0, 1);
}
