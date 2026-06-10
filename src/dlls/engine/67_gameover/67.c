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
/*0x8*/ static f32 bss_8;
/*0xC*/ static f32 bss_C;
/*0x10*/ static f32 bss_10;
/*0x14*/ static s8 bss_14;
/*0x15*/ static u8 bss_15;
/*0x16*/ static s8 bss_16;
/*0x17*/ static s8 bss_17;
/*0x18*/ static u8 bss_18[0x8];

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
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/67_gameover/dll_67_func_200.s")
#else
s32 dll_67_func_200(void) {
    s32 action; //2C
    s32 delay;
    PlayerStats* stats;
    Object* player; //20
    s8 prevTimer;
    s32 timer;
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
#endif

// offset: 0x868 | func: 1 | export: 1
void dll_67_func_868(void) {
    return;
}

// offset: 0x870 | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/67_gameover/dll_67_func_870.s")
