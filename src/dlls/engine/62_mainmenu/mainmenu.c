#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/74_picmenu.h"
#include "game/gametexts.h"
#include "sys/fonts.h"
#include "sys/map_enums.h"
#include "sys/vi.h"
#include "sys/gfx/textable.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/memory.h"
#include "sys/rcp.h"
#include "types.h"

typedef enum {
    //Overrides for item7 (Language)
    MainMenu_ITEM_0_English = 0,
    MainMenu_ITEM_1_Francais = 1,
    MainMenu_ITEM_2_Deutsch = 2,
    MainMenu_ITEM_3_Espanol = 3,
    MainMenu_ITEM_5_Italiano = 4,

    MainMenu_ITEM_5_Start = 5,
    MainMenu_ITEM_6_Options = 6,
    MainMenu_ITEM_7_Language = 7 //Gets overridden with items 0-4
} MainMenuItemIndices;

typedef enum {
    MMTextIdx_0_English = 0,
    MMTextIdx_1_Francais = 1,
    MMTextIdx_2_Deutsch = 2,
    MMTextIdx_3_Espanol = 3,
    MMTextIdx_4_Italiano = 4,
    MMTextIdx_5_Japanese = 5,
    MMTextIdx_6_Press_Start = 6,
    MMTextIdx_7_Start = 7,
    MMTextIdx_8_Options = 8
} MainMenuTextStrings;

#define MENU_TOTAL_PICMENU_ITEMS 8
#define MENU_TRANSITION_THRESHOLD 12
#define MENU_TRANSITION_DURATION 35
#define NONE -1

/*0x0*/ static PicMenuItem pressStartItem[] = {
    /*0*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 165, /*innerWidth*/ 0,
    /*itemX*/ 160, /*itemY*/ 175, /*textureID*/ NONE, /*outerWidth*/ 0,
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT,
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
    /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x3C*/ static PicMenuItem mainMenuItems[] = {
    //Language (English)
    /*0*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0,
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ NONE, /*outerWidth*/ 0,
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT,
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
    /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ MainMenu_ITEM_5_Italiano, /*rightLink*/ MainMenu_ITEM_1_Francais, /*overrideWith*/ NONE
    ),

    //Language (Français)
    /*1*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0,
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0,
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT,
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
    /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ MainMenu_ITEM_0_English, /*rightLink*/ MainMenu_ITEM_2_Deutsch, /*overrideWith*/ NONE
    ),

    //Language (Deutsch)
    /*2*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0,
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ NONE, /*outerWidth*/ 0,
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT,
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
    /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ MainMenu_ITEM_1_Francais, /*rightLink*/ MainMenu_ITEM_3_Espanol, /*overrideWith*/ NONE
    ),

    //Language (Español)
    /*3*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0,
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ NONE, /*outerWidth*/ 0,
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT,
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
    /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ MainMenu_ITEM_2_Deutsch, /*rightLink*/ MainMenu_ITEM_5_Italiano, /*overrideWith*/ NONE
    ),

    //Language (Italiano)
    /*4*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0,
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ NONE, /*outerWidth*/ 0,
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT,
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
    /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ MainMenu_ITEM_3_Espanol, /*rightLink*/ MainMenu_ITEM_0_English, /*overrideWith*/ NONE
    ),

    //Start
    /*5*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 150, /*innerWidth*/ 0,
    /*itemX*/ 160, /*itemY*/ 180, /*textureID*/ NONE, /*outerWidth*/ 0,
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT,
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
    /*upLink*/ NONE, /*downLink*/ MainMenu_ITEM_6_Options, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    //Options
    /*6*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 165, /*innerWidth*/ 0,
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ NONE, /*outerWidth*/ 0,
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT,
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
    /*upLink*/ MainMenu_ITEM_5_Start, /*downLink*/ MainMenu_ITEM_7_Language, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    //Language
    /*7*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0,
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ NONE, /*outerWidth*/ 0,
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT,
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
    /*upLink*/ MainMenu_ITEM_6_Options, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ MainMenu_ITEM_0_English
    ),
};

/*0x21C*/ static s16 gametextLineIndices[] = {
    MMTextIdx_0_English,
    MMTextIdx_1_Francais,
    MMTextIdx_2_Deutsch,
    MMTextIdx_3_Espanol,
    MMTextIdx_4_Italiano,
    MMTextIdx_7_Start,
    MMTextIdx_8_Options,
    MMTextIdx_0_English
};

/*0x0*/ static u8 showDPLogo;
/*0x4*/ static GameTextChunk* gametext;
/*0x8*/ static GplayOptions* options;
/*0xC*/ static s8 sExitTransitionTimer;
/*0xD*/ static s8 nextMenuID;
/*0x10*/ static Texture* logoDinosaurPlanet;

static void mainmenu_clean_up(void);

// offset: 0x0 | ctor
void mainmenu_ctor(void *dll) {
    int total_strings;
    s32 index;

    total_strings = 8;

    logoDinosaurPlanet = tex_load_deferred(TEXTABLE_C5_DinosaurPlanetLogo);
    rcp_set_border_color(0, 0, 0);

    //Set language and get text
    options = gDLL_29_Gplay->vtbl->get_game_options();
    gDLL_21_Gametext->vtbl->set_bank(options->languageID);
    gametext = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_0EE_Menu_Title_Screen);

    //Set "Press Start" text
    pressStartItem->text = gametext->strings[MMTextIdx_6_Press_Start];

    for (index = 0; index < total_strings; index++){
        mainMenuItems[index].text = gametext->strings[gametextLineIndices[index]];
    }

    //Hide the DP logo at first when entering from the Rareware screen
    if (menu_get_previous() == MENU_RAREWARE) {
        gDLL_74_Picmenu->vtbl->set_items(pressStartItem, 1, 0, 0, 0, 0, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
        showDPLogo = FALSE;
    } else {
        mainMenuItems[MainMenu_ITEM_7_Language].overrideWith = options->languageID; //Set language index
        gDLL_74_Picmenu->vtbl->set_items(mainMenuItems, 8, 5, 0, 0, 0, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
        showDPLogo = TRUE;
    }

    nextMenuID = NULL;
    sExitTransitionTimer = 0;
}

// offset: 0x310 | dtor
void mainmenu_dtor(void *dll){ }

// offset: 0x31C | func: 0 | export: 0
s32 mainmenu_update1(void) {
    s32 temp;
    s32 index;
    s32 action;
    s32 lineIndex;
    s8 prevExitTransitionTimer;
    s32 delay;

    prevExitTransitionTimer = sExitTransitionTimer;

    delay = gUpdateRate;
    if (delay > 3) {
        delay = 3;
    }

    if (sExitTransitionTimer > 0) {
        sExitTransitionTimer -= delay;
    }

    //Transitioning to different page once timer runs out
    if (nextMenuID) {
        if (prevExitTransitionTimer > MENU_TRANSITION_THRESHOLD && sExitTransitionTimer <= MENU_TRANSITION_THRESHOLD) {
            //Change resolution for game select
            vi_init(14, get_ossched(), FALSE);
            mainmenu_clean_up();
            track_set_z_buffer_on(FALSE);
            track_set_sky_on(FALSE);
            if (nextMenuID == MENU_GAME_SELECT) {
                gDLL_29_Gplay->vtbl->save_game_options();
            }
        } else if (sExitTransitionTimer < 1) {
            main_change_map(MAP_FRONT_END2, 0, PLAYER_KRYSTAL, nextMenuID);
        }

        if (sExitTransitionTimer <= MENU_TRANSITION_THRESHOLD) {
            return 1;
        } else {
            return 0;
        }
    } else {
        action = gDLL_74_Picmenu->vtbl->update();

        //When DP logo not visible and button pressed
        if (showDPLogo == FALSE){
            if (action == PICMENU_ACTION_SELECT) {
                gDLL_74_Picmenu->vtbl->clear_items();
                mainMenuItems[MainMenu_ITEM_7_Language].overrideWith = options->languageID;
                gDLL_74_Picmenu->vtbl->set_items(mainMenuItems, 8, 5, NULL, 0, 0, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
                showDPLogo = TRUE;
            }
        } else {
            if (action == PICMENU_ACTION_SELECT) {

                temp = 1;
                switch (gDLL_74_Picmenu->vtbl->get_selected_item()) {
                    case MainMenu_ITEM_5_Start:
                        gDLL_28_ScreenFade->vtbl->fade(20, temp);
                        nextMenuID = MENU_GAME_SELECT;
                        sExitTransitionTimer = MENU_TRANSITION_DURATION;
                        return 0;
                    case MainMenu_ITEM_6_Options:
                        gDLL_28_ScreenFade->vtbl->fade(20, temp);
                        nextMenuID = MENU_OPTIONS;
                        sExitTransitionTimer = MENU_TRANSITION_DURATION;
                        return 0;
                }
            }

            //Changing language
            options->languageID = gDLL_74_Picmenu->vtbl->get_item_override(MainMenu_ITEM_7_Language);
            if (gDLL_21_Gametext->vtbl->curr_bank() != options->languageID) {
                gDLL_21_Gametext->vtbl->set_bank(options->languageID);
                mmFree(gametext);
                gametext = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_0EE_Menu_Title_Screen);

                temp = MENU_TOTAL_PICMENU_ITEMS;
                for (index = 0; index < temp; index++){
                    mainMenuItems[index].text = gametext->strings[gametextLineIndices[index]];
                }

                gDLL_74_Picmenu->vtbl->update_text(mainMenuItems);
            }
        }

        return 0;
    }
}

// offset: 0x764 | func: 1 | export: 1
void mainmenu_update2(void){ }

// offset: 0x76C | func: 2 | export: 2
void mainmenu_draw(Gfx** gfx, Mtx** mtx, Vertex** vtx) {
    if (!nextMenuID || sExitTransitionTimer >= (MENU_TRANSITION_THRESHOLD - 1)) {
        font_window_set_coords(1, 0, 0, GET_VIDEO_WIDTH(vi_get_current_size()), GET_VIDEO_HEIGHT(vi_get_current_size()));
        font_window_flush_strings(1);
        gDLL_74_Picmenu->vtbl->draw(gfx);
        if (main_demo_finished()) {
            rcp_screen_full_write(gfx, logoDinosaurPlanet, 50, 50, 0, 0, 0xFF, SCREEN_WRITE_TRANSLUCENT);
        }
        font_window_draw(gfx, NULL, NULL, 1);
    }
}

// offset: 0x8AC | func: 3
void mainmenu_clean_up(void) {
    tex_free(logoDinosaurPlanet);
    gDLL_74_Picmenu->vtbl->clear_items();
    mmFree(gametext);
}
