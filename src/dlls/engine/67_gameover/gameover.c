#include "common.h"
#include "game/gametexts.h"
#include "macros.h"
#include "sys/fonts.h"
#include "sys/menu.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/28_screen_fade.h"
#include "dlls/engine/74_picmenu.h"
#include "dlls/objects/210_player.h"

#define NONE -1

typedef enum {
    STATE_0_Use_Duster,             //"Use Duster?" choice (initial screen if the player had any upon dying). Can exit to gameplay or advance to save choice screen.
    STATE_1_Continue_Playing,       //Shown when player isn't revived. Choice between reloading save or returning to boot screen.
    STATE_2_Do_You_Wish_to_Save,    //Shown when player isn't revived. Advances to "Continue Playing?" choice.
    STATE_3_Game_Saved              //"Game Saved" message displayed. Advances to "Continue Playing?" choice.
} GameOver_ScreenStates;

typedef enum {
    ITEM_0_Game_Over,
    ITEM_1_Yes,
    ITEM_2_No,
    ITEM_3_Current_Choice       //Overridden with ITEM_1_Yes/ITEM_2_No
} GameOver_PicMenuItems;

typedef enum {
    EXIT_TO_RELOADED_SAVE = 1,  //The player didn't use a Duster, but decided to reload their save and continue playing
    EXIT_TO_BOOT_SCREEN = 2,    //The player didn't use a Duster, and decided to stop playing
    EXIT_TO_GAMEPLAY = 3        //The player used a Duster, gameplay continues immediately
} GameOver_Destinations;

typedef enum {
    STRING_0_CONTINUE,
    STRING_1_PLAYING,
    STRING_2_GAME_OVER,
    STRING_3_DO_YOU_WISH,
    STRING_4_TO_SAVE,
    STRING_5_GAME_SAVED,
    STRING_6_USE_DUSTER,
    STRING_7_YES,
    STRING_8_NO
} GameOver_Strings;

#define CENTRE_X (320/2)

#define GAME_OVER_Y 20
#define TEXT1_Y 97
#define TEXT2_Y 123
#define CHOICE_Y 172
#define GAME_SAVED_Y 115

#define USE_DUSTER_TEXT_X 18
#define USE_DUSTER_TEXT_Y 18
#define USE_DUSTER_BOX_X 15
#define USE_DUSTER_BOX_Y 36
#define USE_DUSTER_COUNT_X (USE_DUSTER_BOX_X + 55)
#define USE_DUSTER_COUNT_Y (USE_DUSTER_BOX_Y + 22)

#define TEXT_OFFSET(var) (var + 20)

#define SHADOW(var) (var - 1)
#define SHADOW_OPACITY(opacity) ((((u8)opacity) * 0x96) >> 8)

/*0x0*/ static PicMenuItem dPicmenuItems[] = {
    //GAME OVER
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ CENTRE_X, /*textY*/ TEXT_OFFSET(GAME_OVER_Y), /*innerWidth*/ 0, 
        /*itemX*/ CENTRE_X, /*itemY*/ GAME_OVER_Y, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),
    
    //YES
    /*1*/ NEW_PICMENU_ITEM( 
        /*textX*/ CENTRE_X, /*textY*/ TEXT_OFFSET(CHOICE_Y), /*innerWidth*/ 0, 
        /*itemX*/ CENTRE_X, /*itemY*/ CHOICE_Y, /*textureID*/ NONE, /*outerWidth*/ 100,
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_FLAG_INVISIBLE | PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ ITEM_2_No, /*rightLink*/ ITEM_2_No, /*overrideWith*/ NONE
    ),
    
    //NO
    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ CENTRE_X, /*textY*/ TEXT_OFFSET(CHOICE_Y), /*innerWidth*/ 0, 
        /*itemX*/ CENTRE_X, /*itemY*/ CHOICE_Y, /*textureID*/ NONE, /*outerWidth*/ 100,
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_FLAG_INVISIBLE | PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ ITEM_1_Yes, /*rightLink*/ ITEM_1_Yes, /*overrideWith*/ NONE
    ), 
    
    //Override: YES/NO
    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ CENTRE_X, /*textY*/ TEXT_OFFSET(CHOICE_Y), /*innerWidth*/ 0, 
        /*itemX*/ CENTRE_X, /*itemY*/ CHOICE_Y, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,  
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ ITEM_1_Yes
    )
};

/*0x0*/ static GameTextChunk* sGameOverText;
/*0x4*/ static Texture* sTexDusterCounter;
/*0x8*/ static s16 sOpacity;
/*0xC*/ static f32 sFadeInDelayTimer;
/*0x10*/ static f32 sGameSavedTimer;
/*0x14*/ static u8 sGameOverScreenState;
/*0x15*/ static u8 sDestination;
/*0x16*/ static s8 sTransitionTimer;
/*0x18*/ static char sDusterCountStr[8];

// offset: 0x0 | ctor
void gameover_ctor(void* dll) {
    PlayerStats* stats = gDLL_29_Gplay->vtbl->get_player_stats();
    
    //Load textures, font, and gametext
    sTexDusterCounter = tex_load_deferred(TEXTABLE_310);
    font_load(FONT_FUN_FONT);
    sGameOverText = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_200_Death_Screen);
    
    //@debug: set Duster count to 1 when player dies
    stats->dusters = 1;

    //Start off at the "Use Duster?" screen, if the player has any
    if ((func_80010028() == TRUE) && (stats->dusters > 0)) {
        sGameOverScreenState = STATE_0_Use_Duster;
        dPicmenuItems[0].flags |= PICMENU_INTANGIBLE;

    } else {
        //Otherwise, start out at the save option screen
        sGameOverScreenState = STATE_2_Do_You_Wish_to_Save;
    }
    
    //Set up Picmenu items
    {
        dPicmenuItems[ITEM_0_Game_Over].text = sGameOverText->strings[STRING_2_GAME_OVER];
        dPicmenuItems[ITEM_1_Yes].text = sGameOverText->strings[STRING_7_YES];
        dPicmenuItems[ITEM_2_No].text = sGameOverText->strings[STRING_8_NO];
        dPicmenuItems[ITEM_3_Current_Choice].overrideWith = ITEM_1_Yes;

        gDLL_74_Picmenu->vtbl->set_items(dPicmenuItems, 
            ARRAYCOUNT(dPicmenuItems), 
            ITEM_3_Current_Choice, 
            NULL, 
            0, 0, 
            0xB7, 0x8B, 0x61, 
            0xFF, 0xD7, 0x3D
        );
    }
}

// offset: 0x198 | dtor
void gameover_dtor(void *dll) {
    tex_free(sTexDusterCounter);
    font_unload(FONT_FUN_FONT);
    mmFree(sGameOverText);
}

// offset: 0x200 | func: 0 | export: 0
s32 gameover_update1(void) {
    s32 action;
    s32 delay;
    PlayerStats* stats;
    Object* player;
    s8 prevTimer;
    s32 selectedIdx;
    u32 index;

    player = get_player();

    //Handle leaving the Game Over screen 
    {
        prevTimer = sTransitionTimer;
        
        delay = gUpdateRate;
        if (delay > 3) {
            delay = 3;
        }
        
        if (sTransitionTimer > 0) {
            sTransitionTimer -= delay;
        }    
        
        if (sDestination == EXIT_TO_RELOADED_SAVE) {
            if (gDLL_28_ScreenFade->vtbl->is_complete()) {
                gDLL_29_Gplay->vtbl->start_loaded_game();
            }
            return 0;
        }
        
        if (sDestination == EXIT_TO_GAMEPLAY) {
            return 0;
        }
        
        if (sDestination == EXIT_TO_BOOT_SCREEN) {
            if ((prevTimer >= 13) && (sTransitionTimer < 13)) {
                func_80013FB4();
            } else if (sTransitionTimer <= 0) {
                menu_set(MENU_POST);
            }
            
            if (sTransitionTimer < 13) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    
    //Handle player's choices
    action = gDLL_74_Picmenu->vtbl->update();
    if (action != PICMENU_ACTION_NONE) {
        selectedIdx = gDLL_74_Picmenu->vtbl->get_selected_item();
        switch (sGameOverScreenState) {
        case STATE_1_Continue_Playing:
            if (action == PICMENU_ACTION_SELECT) {
                //Player chooses to reload their save and continue playing
                if (gDLL_74_Picmenu->vtbl->get_item_override(selectedIdx) == ITEM_1_Yes) {
                    gDLL_28_ScreenFade->vtbl->fade(30, SCREEN_FADE_BLACK);
                    sDestination = EXIT_TO_RELOADED_SAVE;

                //Player chooses to stop playing
                } else {
                    gDLL_28_ScreenFade->vtbl->fade(30, SCREEN_FADE_BLACK);
                    sDestination = EXIT_TO_BOOT_SCREEN;
                    sTransitionTimer = 45;
                }
            }
            break;
        case STATE_2_Do_You_Wish_to_Save:
            if (action == PICMENU_ACTION_SELECT) {
                //Player chooses to save
                if (gDLL_74_Picmenu->vtbl->get_item_override(selectedIdx) == ITEM_1_Yes) {
                    sGameSavedTimer = 0.0f;
                    sGameOverScreenState = STATE_3_Game_Saved;

                    //Hide all Picmenu items
                    for (index = 0; index < ARRAYCOUNT(dPicmenuItems); index++){
                        dPicmenuItems[index].flags |= PICMENU_INTANGIBLE;
                    }
                    
                    //Start with "Yes" option selected
                    gDLL_74_Picmenu->vtbl->set_item_override(ITEM_3_Current_Choice, ITEM_1_Yes);
                    gDLL_74_Picmenu->vtbl->update_flags(dPicmenuItems);

                //Player chooses not to save
                } else {
                    //Advance to "Continue Playing?" screen
                    sGameOverScreenState = STATE_1_Continue_Playing;

                    //Start with "Yes" option selected
                    gDLL_74_Picmenu->vtbl->set_item_override(ITEM_3_Current_Choice, ITEM_1_Yes);
                }
            }
            break;
        case STATE_0_Use_Duster:
            if (action == PICMENU_ACTION_SELECT) {
                //Player chooses to use a Duster
                if (gDLL_74_Picmenu->vtbl->get_item_override(selectedIdx) == ITEM_1_Yes) {
                    //Remove one Duster
                    stats = gDLL_29_Gplay->vtbl->get_player_stats();
                    stats->dusters--;

                    //Restore max health
                    ((DLL_210_Player*)player->dll)->vtbl->set_health(
                        player, 
                        ((DLL_210_Player*)player->dll)->vtbl->get_health_max(player)
                    );

                    //Restore max magic
                    ((DLL_210_Player*)player->dll)->vtbl->set_magic(
                        player, 
                        ((DLL_210_Player*)player->dll)->vtbl->get_magic_max(player)
                    );

                    //Revive and continue with gameplay immediately
                    sDestination = EXIT_TO_GAMEPLAY;

                //Player chooses not to be revived
                } else {
                    //Advance to save option screen
                    sGameOverScreenState = STATE_2_Do_You_Wish_to_Save;

                    //Unhide "GAME OVER" Picmenu item
                    dPicmenuItems[ITEM_0_Game_Over].flags &= ~PICMENU_INTANGIBLE;

                    //Start with "Yes" option selected
                    gDLL_74_Picmenu->vtbl->set_item_override(ITEM_3_Current_Choice, ITEM_1_Yes);
                    gDLL_74_Picmenu->vtbl->update_flags(dPicmenuItems);
                }
            }
            break;
        }
    }
    
    //Handle displaying a "Game Saved" message
    if (sGameOverScreenState == STATE_3_Game_Saved) {
        //Save the game
        if (sGameSavedTimer == 0.0f) {
            gDLL_29_Gplay->vtbl->save_game();
        }
        
        //Advance to the "Continue Playing?" screen after 2 seconds
        sGameSavedTimer += gUpdateRateF;
        if (sGameSavedTimer >= 120.0f) {
            //Advance to "Continue Playing?" screen
            sGameOverScreenState = STATE_1_Continue_Playing;
            
            //Unhide all Picmenu items
            for (index = 0; index < ARRAYCOUNT(dPicmenuItems); index++){
                dPicmenuItems[index].flags &= ~PICMENU_INTANGIBLE;
            }
            
            gDLL_74_Picmenu->vtbl->update_flags(dPicmenuItems);
        }
    }
    
    //Start fading in the Game Over screen after a 1 second delay
    sFadeInDelayTimer += gUpdateRateF;
    if (sFadeInDelayTimer >= 60.0f) {
        sOpacity += gUpdateRate * 4;
        if (sOpacity > 140) {
            sOpacity = 140;
        }
        sFadeInDelayTimer = 60.0f;
    }

    return 0;
}

// offset: 0x868 | func: 1 | export: 1
void gameover_update2(void) {
    return;
}

// offset: 0x870 | func: 2 | export: 2
void gameover_draw(Gfx** gfx, Mtx** mtx, Vertex** vtx) {
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;
    f32 opacity;
    PlayerStats* stats;

    stats = gDLL_29_Gplay->vtbl->get_player_stats();
    
    //Stop drawing just before exiting back to the boot screen
    if ((sDestination == EXIT_TO_BOOT_SCREEN) && (sTransitionTimer < 11)) {
        return;
    }

    opacity = ((f32)sOpacity / 140.0f) * 255.0f;
    
    viewport_get_full_rect(&ulx, &uly, &lrx, &lry);
    gDPSetCombineMode(*gfx, G_CC_PRIMITIVE, G_CC_PRIMITIVE); 
    dl_apply_combine(gfx);
    gDPSetOtherMode(*gfx, 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | 
        G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | 
        G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2);
    dl_apply_other_mode(gfx);
    dl_set_prim_color(gfx, 0, 0, 0, sOpacity);
    gDPFillRectangle((*gfx)++, ulx, uly, lrx, lry);
    gDLBuilder->needsPipeSync = TRUE;
    
    font_window_set_coords(1, 0, 0, 
        GET_VIDEO_WIDTH(vi_get_current_size()), 
        GET_VIDEO_HEIGHT(vi_get_current_size())
    );
    font_window_flush_strings(1);
    font_window_use_font(1, FONT_FUN_FONT);
    font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, opacity);
    
    switch (sGameOverScreenState) {
    case STATE_1_Continue_Playing:
        //Print "CONTINUE PLAYING?" text, with drop-shadow
        font_window_add_string_xy(1, CENTRE_X, TEXT1_Y, sGameOverText->strings[STRING_0_CONTINUE], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, CENTRE_X, TEXT2_Y, sGameOverText->strings[STRING_1_PLAYING], 1, ALIGN_TOP_CENTER);
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, SHADOW_OPACITY(opacity));
        font_window_add_string_xy(1, SHADOW(CENTRE_X), SHADOW(TEXT1_Y), sGameOverText->strings[STRING_0_CONTINUE], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, SHADOW(CENTRE_X), SHADOW(TEXT2_Y), sGameOverText->strings[STRING_1_PLAYING], 1, ALIGN_TOP_CENTER);
        break;
    case STATE_2_Do_You_Wish_to_Save:
        //Print "DO YOU WISH TO SAVE?" text, with drop-shadow
        font_window_add_string_xy(1, CENTRE_X, TEXT1_Y, sGameOverText->strings[STRING_3_DO_YOU_WISH], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, CENTRE_X, TEXT2_Y, sGameOverText->strings[STRING_4_TO_SAVE], 1, ALIGN_TOP_CENTER);
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, SHADOW_OPACITY(opacity));
        font_window_add_string_xy(1, SHADOW(CENTRE_X), SHADOW(TEXT1_Y), sGameOverText->strings[STRING_3_DO_YOU_WISH], 1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, SHADOW(CENTRE_X), SHADOW(TEXT2_Y), sGameOverText->strings[STRING_4_TO_SAVE], 1, ALIGN_TOP_CENTER);
        break;
    case STATE_3_Game_Saved:
        //Print "GAME SAVED" text, with drop-shadow
        font_window_add_string_xy(1, CENTRE_X, GAME_SAVED_Y, sGameOverText->strings[STRING_5_GAME_SAVED], 1, ALIGN_TOP_CENTER);
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, SHADOW_OPACITY(opacity));
        font_window_add_string_xy(1, SHADOW(CENTRE_X), SHADOW(GAME_SAVED_Y), sGameOverText->strings[STRING_5_GAME_SAVED], 1, ALIGN_TOP_CENTER);
        break;
    case STATE_0_Use_Duster:
        //Print "USE DUSTER?" text, with drop-shadow
        font_window_add_string_xy(1, USE_DUSTER_TEXT_X, USE_DUSTER_TEXT_Y, sGameOverText->strings[STRING_6_USE_DUSTER], 1, ALIGN_TOP_LEFT);
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, SHADOW_OPACITY(opacity));
        font_window_add_string_xy(1, SHADOW(USE_DUSTER_TEXT_X), SHADOW(USE_DUSTER_TEXT_Y), sGameOverText->strings[STRING_6_USE_DUSTER], 1, ALIGN_TOP_LEFT);
        
        //Draw Duster counter
        rcp_screen_full_write(gfx, sTexDusterCounter, USE_DUSTER_BOX_X, USE_DUSTER_BOX_Y, 0, 0, opacity, 0);
        font_window_use_font(1, FONT_DINO_SUBTITLE_FONT_1);
        sprintf(sDusterCountStr, "%02d", stats->dusters);
        font_window_add_string_xy(1, USE_DUSTER_COUNT_X, USE_DUSTER_COUNT_Y, sDusterCountStr, 1, ALIGN_TOP_RIGHT);
        break;
    }
    
    gDLL_74_Picmenu->vtbl->set_opacity(opacity);
    gDLL_74_Picmenu->vtbl->draw(gfx);
    font_window_draw(gfx, NULL, NULL, 1);
}
