#include "dlls/engine/20_screens.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/29_gplay.h"
#include "game/gametexts.h"
#include "sys/fonts.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/memory.h"
#include "sys/menu.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

extern DLL_29_gplay *gDLL_29_Gplay;
extern DLL_Unknown* dll_throw_fault; //NOTE: BROKEN! This is a function now, not a DLL.

extern s8 D_8008C8B4;

typedef enum {
    SwapStone_Old_Continue,
    SwapStone_Old_Swap,
    SwapStone_Old_To_Warlock
} SwapStone_Old_Options;

/*0x0*/ static s32 bss_0;
/*0x4*/ static s32 bss_4;

// offset: 0x0 | ctor
void dll_71_ctor(void *dll) {
    bss_0 = 0;
    bss_4 = 0;
}

// offset: 0x28 | dtor
void dll_71_dtor(void *dll) { }

// offset: 0x34 | func: 0 | export: 0
s32 dll_71_update1(void) {
    return 0;
}

// offset: 0x40 | func: 1 | export: 1
void dll_71_update2(void) {

}

// offset: 0x48 | func: 2 | export: 2
void dll_71_draw(Gfx** gdl, Mtx** mtx, Vertex** vtx) {
    s32 warpID;
    s32 playerNumber;

    playerNumber = gDLL_29_Gplay->vtbl->get_playerno();
    D_8008C8B4 = FALSE;
    
    ((DLL_Unknown*)dll_throw_fault)->vtbl->func[7].withOneArg(bss_4);
    ((DLL_Unknown*)dll_throw_fault)->vtbl->func[0].withOneArg(60);
    ((DLL_Unknown*)dll_throw_fault)->vtbl->func[2].withFourArgs(0, "CONTINUE",   20, bss_0);
    ((DLL_Unknown*)dll_throw_fault)->vtbl->func[2].withFourArgs(1, "SWAP",       20, bss_0);
    ((DLL_Unknown*)dll_throw_fault)->vtbl->func[2].withFourArgs(2, "TO WARLOCK", 20, bss_0);
    ((DLL_Unknown*)dll_throw_fault)->vtbl->func[4].withOneArg(0);
    
    switch (((DLL_Unknown*)dll_throw_fault)->vtbl->func[5].withOneArgS32((s32)&bss_0)) {
    default:
        D_8008C8B4 = TRUE;
        break;
    case SwapStone_Old_Continue:
        obj_send_mesg_many(/* OBJ_16 (deleted) */16, OBJMSG_SEND_FILTER_ID, NULL, 0, 0);
        menu_set(MENU_GAMEPLAY);
        break;
    case SwapStone_Old_Swap:
        warpID = (playerNumber == PLAYER_SABRE) ? WARP_SC_RUBBLE_PODIUM : WARP_SH_ROCKY_PODIUM;
        gDLL_29_Gplay->vtbl->set_playerno((1 - playerNumber));
        warpPlayer(warpID, TRUE);
        menu_set(MENU_GAMEPLAY);
        break;
    case SwapStone_Old_To_Warlock:
        warpID = (playerNumber == PLAYER_SABRE) ? WARP_WM_SABRE_SIDE : WARP_WM_KRYSTAL_SIDE;
        warpPlayer(warpID, TRUE);
        break;
    }
    
    font_window_set_coords(3, 100, 25, 220, 140);
    font_window_set_bg_colour(3, 0, 0, 0, 0x80);
    font_window_flush_strings(3);
    font_window_draw(gdl, 0, 0, 3);
    font_window_draw(gdl, 0, 0, 1);
    bss_4 = TRUE;
}
