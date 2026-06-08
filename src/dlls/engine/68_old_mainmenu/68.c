#include "dlls/engine/20_screens.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/73.h"
#include "game/gametexts.h"
#include "sys/fonts.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/menu.h"
#include "sys/rcp.h"

extern DLL_20_screens *gDLL_20_Screens;
extern DLL_21_gametext *gDLL_21_Gametext;
extern DLL_73* dll_throw_fault; //NOTE: BROKEN! This is a function now, not a DLL 73.

extern s8 D_8008C8B4;

#define END {NULL, -1, 0, 0}

/* Possibly TextureTiles for the old Dinosaur Planet logo? Unused! */
/*0x0*/ static TextureTile dTexTiles[] = {
    /* Row 0 */
    {NULL,   0,   -96, -32}, //missing
    {NULL,   0,   -64, -32}, //TEX0 688
    {NULL,   0,   -32, -32}, //TEX0 689
    {NULL,   0,   0,   -32}, //TEX0 690 
    {NULL,   0,   32,  -32}, //TEX0 691
    {NULL,   0,   64,  -32}, //TEX0 692

    /* Row 1 */
    {NULL,   0,   -96, 0}, //TEX0 693
    {NULL,   0,   -64, 0}, //TEX0 694 
    {NULL,   0,   -32, 0}, //TEX0 695
    {NULL,   0,   0,   0}, //TEX0 696
    {NULL,  0,   32,  0}, //TEX0 697
    {NULL,  0,   64,  0}, //TEX0 698

    /* Row 2 */
    {NULL,  0,   -96, 32}, //TEX0 699 
    {NULL,  0,   -64, 32}, //TEX0 700
    {NULL,  0,   -32, 32}, //TEX0 701
    {NULL,  0,   0,   32}, //TEX0 702
    {NULL,  0,   32,  32}, //TEX0 703
    {NULL,  0,   64,  32}, //TEX0 704

    END
};

/*0x0*/ static GameTextChunk* sGametext;
/*0x4*/ static s32 sIndexSelected;
/*0x8*/ static s32 sTextTimer;
/*0xC*/ static s32 sButtonsEnabled;
/*0x10*/ static s32 sTimer;

// offset: 0x0 | ctor
void old_mainmenu_ctor(void* dll) {
    sGametext = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_005_Title_Menu);
    sIndexSelected = 0;
    sTextTimer = 0;
    sButtonsEnabled = FALSE;
    sTimer = 0;
    func_80010018(2);
}

// offset: 0x84 | dtor
void old_mainmenu_dtor(void* dll) {
    mmFree(sGametext);
}

// offset: 0xC4 | func: 0 | export: 0
s32 old_mainmenu_update1(void) {
    return 0;
}

// offset: 0xD0 | func: 1 | export: 1
void old_mainmenu_update2(void) {

}

// offset: 0xD8 | func: 2 | export: 2
void old_mainmenu_draw(Gfx** gdl, Mtx** mtx, Vertex** vtx) {   
    D_8008C8B4 = 1;
    
    sTimer += gUpdateRate;    
    if (sTimer >= 41) {
        sTimer = 41;
    }

    // There were more than 2 SCREENS.bin images at one stage!
    // Maybe the static Rare logo from the One Hour Footage?
    gDLL_20_Screens->vtbl->show_screen(2);
    
    func_80014508(20);

    // NOTE: this section is very broken, since it's expecting DLL 73 to be at the address of `dll_throw_fault`!
    // Maybe it used to call a DLL for the old title screen text seen in the One Hour Footage?
    dll_throw_fault->vtbl->enable_joy_buttons(sButtonsEnabled);
    dll_throw_fault->vtbl->init_text_window(180);
    dll_throw_fault->vtbl->add_string(0, sGametext->strings[0], 20, sIndexSelected);
    if (dll_throw_fault->vtbl->handle_joystick_and_buttons(&sIndexSelected) == FALSE) {
        menu_set(MENU_13);
    }
    
    // Maybe this was the blinking "PRESS START" text?
    {
        if (sTextTimer > 20) {
            font_window_draw(gdl, NULL, NULL, 1);
        }
        sTextTimer = (gUpdateRate + sTextTimer) & 0x3F;
    }

    sButtonsEnabled = TRUE;
}
