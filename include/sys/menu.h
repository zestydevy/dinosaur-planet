#ifndef _SYS_MENU_H
#define _SYS_MENU_H

#include "PR/ultratypes.h"
#include "sys/gfx/gx.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE_BEGIN(Menu)
    /*0*/ s32 (*update1)();
    /*1*/ void (*update2)();
    /*2*/ void (*draw)(Gfx **gdl, Mtx **mtxs, Vertex **vtxs);
DLL_INTERFACE_END()

// indices into gMenuDLLIDs
enum MenuID {
    MENU_NONE = 0,
    MENU_1 = 1, // active during gameplay?
    MENU_2 = 2, // copyright/n64 logos
    MENU_3 = 3, // rare logo
    MENU_4 = 4, // main menu
    MENU_5 = 5, // game select
    MENU_6 = 6, // enter name screen
    MENU_7 = 7, // options menu
    MENU_8 = 8, // pause screen
    MENU_9 = 9, // gameover screen
    MENU_10 = 10,
    MENU_11 = 11,
    MENU_12 = 12,
    MENU_13 = 13,
    MENU_14 = 14,
    MENU_15 = 15,
    MENU_16 = 16,
    MENU_17 = 17
};

void menu_init();
void menu_set(s32 id);
s32 menu_get_current();
DLLInst_Menu *menu_get_active_dll();
s32 menu_get_previous();
void menu_do_menu_swap();
s32 menu_update1();
void menu_update2();
void menu_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols);
void func_8000F64C(s8 param1, s32 param2);
void func_8000F6CC();
void func_8000F9DC();
void func_8000FA2C();
void func_8000FAC8();
s32 func_8000FB1C();
void func_8000FB2C(Gfx **gdl);
s32 func_80010008();
void func_80010018(s32 param1);
s32 func_80010028();
void func_80010038(s32 param1);
s32 func_80010048();
void func_80010058(s32 param1);
void set_save_game_idx(s32 index);
s32  get_save_game_idx();
void func_80010088();
void func_800100D4(s32 param1, s32 param2, s32 param3, s32 param4);
void func_80010158(s32 *param1, s32 *param2, s32 *param3, s32 *param4);

#endif
