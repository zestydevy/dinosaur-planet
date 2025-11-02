#include "PR/gbi.h"
#include "PR/os.h"
#include "PR/ultratypes.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/74_picmenu.h"
#include "game/gametexts.h"
#include "sys/fonts.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/texture.h"
#include "sys/memory.h"
#include "sys/menu.h"
#include "dll.h"
#include "functions.h"
#include "macros.h"
#include "types.h"

#define NAME_ENTRY_PICMENU_ITEM(textX, textY, itemX, itemY, flags, upLink, downLink, leftLink, rightLink) \
    NEW_PICMENU_ITEM(textX, textY, 0x3C, itemX, itemY, 0x316, 0x34, flags, FONT_DINO_MEDIUM_FONT_OUT, FONT_DINO_MEDIUM_FONT_IN, upLink, downLink, leftLink, rightLink, -1)

static PicMenuItem sMenuItems[] = {
    /*0*/NAME_ENTRY_PICMENU_ITEM(0x4D, 0xC2, 0x4D, 0xB6, 0x2, -1, 9,  8, 1), // A
    /*1*/NAME_ENTRY_PICMENU_ITEM(0,    0xC2, 0,    0xB6, 0xA, -1, 10, 0, 2), // B
    /*2*/NAME_ENTRY_PICMENU_ITEM(0,    0xC2, 0,    0xB6, 0xA, -1, 11, 1, 3), // C
    /*3*/NAME_ENTRY_PICMENU_ITEM(0,    0xC2, 0,    0xB6, 0xA, -1, 12, 2, 4), // D
    /*4*/NAME_ENTRY_PICMENU_ITEM(0,    0xC2, 0,    0xB6, 0xA, -1, 13, 3, 5), // E
    /*5*/NAME_ENTRY_PICMENU_ITEM(0,    0xC2, 0,    0xB6, 0xA, -1, 14, 4, 6), // F
    /*6*/NAME_ENTRY_PICMENU_ITEM(0,    0xC2, 0,    0xB6, 0xA, -1, 15, 5, 7), // G
    /*7*/NAME_ENTRY_PICMENU_ITEM(0,    0xC2, 0,    0xB6, 0xA, -1, 16, 6, 8), // H
    /*8*/NAME_ENTRY_PICMENU_ITEM(0,    0xC2, 0,    0xB6, 0xA, -1, 16, 7, 0), // I

    /*9*/ NAME_ENTRY_PICMENU_ITEM(0x5C, 0xF8, 0x5C, 0xEC, 0x2, 0, 17, 16, 10), // J
    /*10*/NAME_ENTRY_PICMENU_ITEM(0,    0xF8, 0,    0xEC, 0xA, 1, 18, 9,  11), // K
    /*11*/NAME_ENTRY_PICMENU_ITEM(0,    0xF8, 0,    0xEC, 0xA, 2, 19, 10, 12), // L
    /*12*/NAME_ENTRY_PICMENU_ITEM(0,    0xF8, 0,    0xEC, 0xA, 3, 20, 11, 13), // M
    /*13*/NEW_PICMENU_ITEM(0, 0xF8, 0x3B, 0, 0xEC, 0x316, 0x34, 0xA, FONT_DINO_MEDIUM_FONT_OUT, FONT_DINO_MEDIUM_FONT_IN, 4, 21, 12, 14, -1), // N
    /*14*/NAME_ENTRY_PICMENU_ITEM(0,    0xF8, 0,    0xEC, 0xA, 5, 22, 13, 15), // O
    /*15*/NAME_ENTRY_PICMENU_ITEM(0,    0xF8, 0,    0xEC, 0xA, 6, 23, 14, 16), // P
    /*16*/NAME_ENTRY_PICMENU_ITEM(0,    0xF8, 0,    0xEC, 0xA, 7, 24, 15, 9),  // Q

    /*17*/NAME_ENTRY_PICMENU_ITEM(0x4D, 0x12E, 0x4D, 0x122, 0x2, 9,  26, 25, 18), // R
    /*18*/NAME_ENTRY_PICMENU_ITEM(0,    0x12E, 0,    0x122, 0xA, 10, 26, 17, 19), // S
    /*19*/NAME_ENTRY_PICMENU_ITEM(0,    0x12E, 0,    0x122, 0xA, 11, 26, 18, 20), // T
    /*20*/NAME_ENTRY_PICMENU_ITEM(0,    0x12E, 0,    0x122, 0xA, 12, 27, 19, 21), // U
    /*21*/NAME_ENTRY_PICMENU_ITEM(0,    0x12E, 0,    0x122, 0xA, 13, 28, 20, 22), // V
    /*22*/NAME_ENTRY_PICMENU_ITEM(0,    0x12E, 0,    0x122, 0xA, 14, 28, 21, 23), // W
    /*23*/NAME_ENTRY_PICMENU_ITEM(0,    0x12E, 0,    0x122, 0xA, 15, 29, 22, 24), // X
    /*24*/NAME_ENTRY_PICMENU_ITEM(0,    0x12E, 0,    0x122, 0xA, 16, 29, 23, 25), // Y
    /*25*/NAME_ENTRY_PICMENU_ITEM(0,    0x12E, 0,    0x122, 0xA, 16, 29, 24, 17), // Z

    /*26*/NAME_ENTRY_PICMENU_ITEM(0xA6, 0x164, 0xA6, 0x158, 0x2, 19, -1, 29, 27), // .
    /*27*/NAME_ENTRY_PICMENU_ITEM(0,    0x164, 0,    0x158, 0xA, 20, -1, 26, 28), // !
    /*28*/NEW_PICMENU_ITEM(0, 0x164, 0, 0, 0x158, -1, 0, 0x1E, FONT_DINO_MEDIUM_FONT_OUT, FONT_DINO_MEDIUM_FONT_IN, 21, -1, 27, 29, -1), // <-
    /*29*/NEW_PICMENU_ITEM(0, 0x164, 0, 0, 0x158, -1, 0, 0x1E, FONT_DINO_MEDIUM_FONT_OUT, FONT_DINO_MEDIUM_FONT_IN, 23, -1, 28, 26, -1) // END
};

static GameTextChunk *sGameTextChunk = NULL;

static Texture *sLetterBgBoxTexture;
static u8 sNumNameLetters;
static char sNameLetters[10]; // 5 strings 1 character long + null terminator
static s8 sMainRedrawFrames;
static s8 sNameLettersRedrawFrames;
static Texture *sBackgroundTexture;

static void dll_64_clean_up();
static void dll_64_draw_letters(Gfx **gdl, s32 x, s32 y);

void dll_64_ctor(void *dll) {
    s32 itemCount;
    s32 i;
    
    itemCount = ARRAYCOUNT(sMenuItems);
    sBackgroundTexture = queue_load_texture_proxy(0x2DE);

    if (sGameTextChunk == NULL) {
        sGameTextChunk = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_0ED_Menu_Name_Entry);
    }


    for (i = 0; i < itemCount; i++) {
        sMenuItems[i].text = sGameTextChunk->strings[i];
    }

    sLetterBgBoxTexture = queue_load_texture_proxy(0x316);
    gDLL_74_Picmenu->vtbl->set_items(sMenuItems, itemCount, 
        /*defaultItem*/0, 
        /*sounds*/NULL, 
        /*param5*/5, 
        /*param6*/4, 
        /*textColor*/0, 0, 0, 
        /*textHighlight*/0, 0, 0);
    
    sMainRedrawFrames = sNameLettersRedrawFrames = 2;
}

void dll_64_dtor(void *dll) {
    dll_64_clean_up();
}

s32 dll_64_update1() {
    s32 action;
    s32 selected;
    char name[10];
    s32 i;

    action = gDLL_74_Picmenu->vtbl->update();
    if (action != PICMENU_ACTION_NONE) {
        selected = gDLL_74_Picmenu->vtbl->get_selected_item();
        if (action == PICMENU_ACTION_SELECT) {
            if (selected < 28 && sNumNameLetters < 5) {
                bcopy(sMenuItems[selected].text, &sNameLetters[sNumNameLetters << 1], 2);
                sNumNameLetters += 1;
                sNameLettersRedrawFrames = 2;
            } else if (selected == 28 && sNumNameLetters != 0) {
                if (!sNumNameLetters){} // fakematch
                sNumNameLetters -= 1;
                sNameLettersRedrawFrames = 2;
            } else if (selected == 29) {
                for (i = 0; i < sNumNameLetters; i++) {
                    name[i] = sNameLetters[i << 1];
                }
                name[sNumNameLetters] = '\0';

                gDLL_29_Gplay->vtbl->init_save(get_save_game_idx(), name);
                menu_set(MENU_GAME_SELECT);
                sNameLettersRedrawFrames = 2;
            }
        } else {
            if (sNumNameLetters != 0) {
                if (!sNumNameLetters){} // fakematch
                sNumNameLetters -= 1;
                sNameLettersRedrawFrames = 2;
            }
        }
    }

    return 0;
}

void dll_64_update2() { }

void dll_64_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    s32 ulx;
    s32 lrx;
    s32 uly;
    s32 lry;
    
    font_window_set_coords(1, 0, 0, 
        GET_VIDEO_WIDTH(vi_get_current_size()), 
        GET_VIDEO_HEIGHT(vi_get_current_size()));
    font_window_flush_strings(1);
    font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);

    if (sMainRedrawFrames != 0) {
        func_8003825C(gdl, sBackgroundTexture, 0, 0, 0, 0, 0xFF, 2);
        
        font_window_set_text_colour(1, 255, 255, 255, 0, 255);
        font_window_add_string_xy(1, 320, 73, sGameTextChunk->strings[0x1E], 1, ALIGN_TOP_CENTER);

        font_window_set_text_colour(1, 0, 0, 0, 255, 255);
        font_window_add_string_xy(1, 315, 68, sGameTextChunk->strings[0x1E], 2, ALIGN_TOP_CENTER);

        font_window_use_font(1, FONT_FUN_FONT);
        font_window_set_text_colour(1, 183, 139, 97, 255, 255);

        font_window_add_string_xy(1, 320, 405, sGameTextChunk->strings[0x1F], 1, ALIGN_TOP_CENTER);
        font_window_set_text_colour(1, 0, 0, 0, 255, 255);
        font_window_add_string_xy(1, 318, 403, sGameTextChunk->strings[0x1F], 2, ALIGN_TOP_CENTER);
    } else {
        // Always redraw background in case picmenu redraws
        func_80010158(&ulx, &lrx, &uly, &lry);
        func_800382AC(gdl, sBackgroundTexture, 0, 0, uly, lry, 0xFF, 2);
    }

    gDLL_74_Picmenu->vtbl->draw(gdl);

    if (sNameLettersRedrawFrames != 0) {
        if (sMainRedrawFrames == 0) {
            // Make sure we at least redraw the background behind the name letters
            lry = (sLetterBgBoxTexture->height | ((sLetterBgBoxTexture->unk1B & 0xF) << 8));
            func_800382AC(gdl, sBackgroundTexture, 0, 0, 110, lry + 110, 0xFF, 2);
        }

        dll_64_draw_letters(gdl, 179, 110);
    }

    font_window_draw(gdl, NULL, NULL, 1);

    sMainRedrawFrames -= 1;
    if (sMainRedrawFrames < 0) {
        sMainRedrawFrames = 0;
    }

    sNameLettersRedrawFrames -= 1;
    if (sNameLettersRedrawFrames < 0) {
        sNameLettersRedrawFrames = 0;
    }
}

static void dll_64_clean_up() {
    gDLL_74_Picmenu->vtbl->clear_items();
    mmFree(sGameTextChunk);
    texture_destroy(sLetterBgBoxTexture);
    texture_destroy(sBackgroundTexture);
}

static void dll_64_draw_letters(Gfx **gdl, s32 x, s32 y) {
    s32 i;
    s32 x2;
    char *letterPtr;

    letterPtr = sNameLetters;
    x2 = x;

    for (i = 0; i < 5; i++) {
        func_8003825C(gdl, sLetterBgBoxTexture, x2, y, 0, 0, 0xFF, 0);
        x2 += 52;
    }

    font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);
    font_window_set_text_colour(1, 255, 255, 255, 0, 255);
    
    x2 = x;

    for (i = 0; i < sNumNameLetters; i++) {
        font_window_add_string_xy(1, x2 + 30, y + 12, letterPtr, 2, ALIGN_TOP_CENTER);
        x2 += 52;
        letterPtr += 2;
    }
}
