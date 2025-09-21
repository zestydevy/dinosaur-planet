#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/74_picmenu.h"
#include "functions.h"
#include "sys/fonts.h"
#include "sys/gfx/gx.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/memory.h"

/*0x0*/ static PicMenuItem pressStartItem[] = {
    /*0*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 165, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 175, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    )
};
/*0x3C*/ static PicMenuItem mainMenuItems[] = {
    /*0*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 4, /*rightLink*/ 1, /*overrideWith*/ -1
    ),
    /*1*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 0, /*rightLink*/ 2, /*overrideWith*/ -1
    ),
    /*2*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 1, /*rightLink*/ 3, /*overrideWith*/ -1
    ),
    /*3*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 2, /*rightLink*/ 4, /*overrideWith*/ -1
    ),
    /*4*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 3, /*rightLink*/ 0, /*overrideWith*/ -1
    ),
    /*5*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 150, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 180, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ 6, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*6*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 165, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ 5, /*downLink*/ 7, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*7*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ 6, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ 0
    ),
};
/*0x21C*/ static s16 gametextLineIndices[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0007, 0x0008
};

/*0x0*/ static u8 showDPLogo;
/*0x4*/ static GameTextChunk* gametext;
/*0x8*/ static GplayOptions* options;
/*0xC*/ static s8 sExitTransitionTimer;
/*0xD*/ static s8 nextMenuID;
/*0x10*/ static Texture* logoDinosaurPlanet;

#define MENU_TRANSITION_THRESHOLD 12
#define MENU_TRANSITION_DURATION 35

static void mainmenu_clean_up(void);

// offset: 0x0 | ctor
void mainmenu_ctor(s32 arg0) {
    int total_strings;
    s32 index;

    total_strings = 8;
    
    logoDinosaurPlanet = queue_load_texture_proxy(0xC5);
    func_800379D0(0, 0, 0);

    //Set language and get text
    options = gDLL_29_Gplay->vtbl->func_930();
    gDLL_21_Gametext->vtbl->set_bank(options->languageID);
    gametext = gDLL_21_Gametext->vtbl->get_chunk(238);

    //Set "Press Start" text
    pressStartItem->text = gametext->strings[6];

    for (index = 0; index < total_strings; index++){
        mainMenuItems[index].text = gametext->strings[gametextLineIndices[index]];
    }
    
    //Hide the DP logo at first when entering from the Rareware screen 
    if (menu_get_previous() == MENU_RAREWARE) {
        gDLL_74_Picmenu->vtbl->set_items(pressStartItem, 1, 0, 0, 0, 0, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
        showDPLogo = FALSE;
    } else {
        mainMenuItems[7].overrideWith = options->languageID; //Set language index
        gDLL_74_Picmenu->vtbl->set_items(mainMenuItems, 8, 5, 0, 0, 0, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
        showDPLogo = TRUE;
    }
    
    nextMenuID = NULL;
    sExitTransitionTimer = 0;
}

// offset: 0x310 | dtor
void mainmenu_dtor(void *dll){
}

// offset: 0x31C | func: 0 | export: 0
s32 mainmenu_update(void) {
    s32 items;
    s32 index;
    s32 action;
    s32 lineIndex;
    s8 prevExitTransitionTimer;
    s32 delay;

    prevExitTransitionTimer = sExitTransitionTimer;

    delay = delayByte;
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
            func_8005D410(14, get_ossched(), 0);
            mainmenu_clean_up();
            func_80041D20(0);
            func_80041C6C(0);
            if (nextMenuID == MENU_GAME_SELECT) {
                gDLL_29_Gplay->vtbl->func_8D8();
            }
        } else if (sExitTransitionTimer < 1) {
            func_800141A4(1, 0, 1, nextMenuID);
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
                mainMenuItems[7].overrideWith = options->languageID;
                gDLL_74_Picmenu->vtbl->set_items(mainMenuItems, 8, 5, NULL, 0, 0, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
                showDPLogo = TRUE;
            }
        } else {
            if (action == PICMENU_ACTION_SELECT) {
    
                items = 1;
                switch (gDLL_74_Picmenu->vtbl->get_selected_item()) {
                    case 5:
                        gDLL_28_ScreenFade->vtbl->fade(20, items);
                        nextMenuID = MENU_GAME_SELECT;
                        sExitTransitionTimer = MENU_TRANSITION_DURATION;
                        return 0;
                    case 6:
                        gDLL_28_ScreenFade->vtbl->fade(20, items);
                        nextMenuID = MENU_OPTIONS;
                        sExitTransitionTimer = MENU_TRANSITION_DURATION;
                        return 0;
                }
            }
    
            //Changing language
            options->languageID = gDLL_74_Picmenu->vtbl->get_item_override(7);
            if (gDLL_21_Gametext->vtbl->curr_bank() != options->languageID) {
                gDLL_21_Gametext->vtbl->set_bank(options->languageID);
                mmFree(gametext);
                gametext = gDLL_21_Gametext->vtbl->get_chunk(238);
    
                items = 8;
                for (index = 0; index < items; index++){
                    mainMenuItems[index].text = gametext->strings[gametextLineIndices[index]];
                }
                
                gDLL_74_Picmenu->vtbl->update_text(mainMenuItems);
            }
        }
        
        return 0;
    }
}

// offset: 0x764 | func: 1 | export: 1
void mainmenu_func_764(void){
}

// offset: 0x76C | func: 2 | export: 2
void mainmenu_draw(Gfx** gfx, Mtx** mtx, Vertex** vtx) {
    if (!nextMenuID || sExitTransitionTimer >= (MENU_TRANSITION_THRESHOLD - 1)) {
        font_window_set_coords(1, 0, 0, RESOLUTION_WIDTH(get_some_resolution_encoded()), RESOLUTION_HEIGHT(get_some_resolution_encoded()));
        font_window_flush_strings(1);
        gDLL_74_Picmenu->vtbl->draw(gfx);
        if (func_80014C60()) {
            func_8003825C(gfx, logoDinosaurPlanet, 50, 50, 0, 0, 0xFF, 0);
        }
        font_window_draw(gfx, NULL, NULL, 1);
    }
}

// offset: 0x8AC | func: 3
void mainmenu_clean_up(void) {
    texture_destroy(logoDinosaurPlanet);
    gDLL_74_Picmenu->vtbl->clear_items();
    mmFree(gametext);
}
