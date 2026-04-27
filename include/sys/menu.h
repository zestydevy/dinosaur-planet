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
    MENU_10 = 10,
    MENU_11 = 11,
    MENU_12 = 12,
    MENU_13 = 13,
    MENU_14 = 14,
    MENU_CREDITS = 15, // credits
    MENU_SELECTION = 16, // used by SwapStone and ShopKeeper
    MENU_17 = 17
};

extern s8 D_800A7D94;

void menu_init(void);
void menu_set(s32 id);
s32 menu_get_current(void);
DLL_IMenu *menu_get_active_dll(void);
s32 menu_get_previous(void);
void menu_do_menu_swap(void);
s32 menu_update1(void);
void menu_update2(void);
void menu_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols);
void func_8000F64C(s8 param1, s32 param2);
void func_8000F6CC(void);
void func_8000F9DC(void);
void func_8000FA2C(void);
void func_8000FAC8(void);
s32 func_8000FB1C(void);
void func_8000FB2C(Gfx **gdl);
s32 func_80010008(void);
void func_80010018(s32 param1);
s32 func_80010028(void);
void func_80010038(s32 param1);
s32 func_80010048(void);
void func_80010058(s32 param1);
void set_save_game_idx(s32 index);
s32  get_save_game_idx(void);
void func_80010088(void);
void func_800100D4(s32 param1, s32 param2, s32 param3, s32 param4);
void func_80010158(s32 *param1, s32 *param2, s32 *param3, s32 *param4);

#endif
