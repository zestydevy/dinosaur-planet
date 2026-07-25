#ifndef _SYS_MENU_H
#define _SYS_MENU_H

#include "PR/ultratypes.h"
#include "sys/vi.h"
#include "dll_def.h"
#include "types.h"

// Common interface of menu DLLs
DLL_INTERFACE(DLL_IMenu) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ s32 (*update1)(void);
    /*1*/ void (*update2)(void);
    /*2*/ void (*draw)(Gfx **gdl, Mtx **mtxs, Vertex **vtxs);
};

/** States for Menu 16 (DLL 79) */
typedef enum {
    SelectionMenu_STATE_0_Fade_Out,             //Fades out the current selection screen
    SelectionMenu_STATE_1_SwapStone_Choices,    //SwapStone's top-level selection (Swap/Shop/Warp)
    SelectionMenu_STATE_2_Confirm_Right,        //SwapStone's Swap confirm selection
    SelectionMenu_STATE_3_Confirm_Left,         //SwapStone's Warp confirm selection
    SelectionMenu_STATE_4_Confirm_ShopKeeper    //ShopKeeper haggling selection
} SelectionMenu_States;

DLL_INTERFACE(DLL_Menu16) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IMenu);
    /*3*/ void (*set_selection_state)(SelectionMenu_States state); //Sets which selection screen to show, or whether to fade out the current selection screen (see `SelectionMenu_States`)
};

// indices into gMenuDLLIDs
enum MenuID {
    MENU_NONE = 0,
    MENU_GAMEPLAY = 1,
    MENU_POST = 2,
    MENU_RAREWARE = 3,
    MENU_TITLE_SCREEN = 4,
    MENU_GAME_SELECT = 5,
    MENU_ENTER_NAME = 6,
    MENU_OPTIONS = 7,
    MENU_PAUSE = 8,
    MENU_GAME_OVER = 9,
    MENU_OLD_SELECTION = 10,    // SwapStone choices
    MENU_OLD_TITLE_SCREEN = 11,
    MENU_12 = 12,               // empty
    MENU_OLD_LEVEL_SELECT = 13,
    MENU_SCARAB = 14,           // shop minigame counter
    MENU_CREDITS = 15,
    MENU_SELECTION = 16,        // used by SwapStone and ShopKeeper
    MENU_17 = 17                // empty
};

extern s8 D_800A7D94;

void menuInit(void);
void menuSet(s32 id);
s32 menuGetCurrent(void);
DLL_IMenu *menuGetActiveDLL(void);
s32 menuGetPrevious(void);
void menuDoMenuSwap(void);
s32 menuUpdate1(void);
void menuUpdate2(void);
void menuDraw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols);
void menu_func_8000F64C(s8 param1, s32 param2);
void menu_func_8000F6CC(void);
void menu_func_8000F9DC(void);
void menu_func_8000FA2C(void);
void menu_func_8000FAC8(void);
s32 menu_func_8000FB1C(void);
void menu_func_8000FB2C(Gfx **gdl);
s32 menu_func_80010008(void);
void menu_func_80010018(s32 param1);
s32 menu_func_80010028(void);
void menu_func_80010038(s32 param1);
s32 menu_func_80010048(void);
void menu_func_80010058(s32 param1);
void menuSetSaveGameIdx(s32 index);
s32  menuGetSaveGameIdx(void);
void menu_func_80010088(void);
void menu_func_800100D4(s32 param1, s32 param2, s32 param3, s32 param4);
void menu_func_80010158(s32 *param1, s32 *param2, s32 *param3, s32 *param4);

#endif
