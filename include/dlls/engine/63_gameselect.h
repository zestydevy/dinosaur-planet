#ifndef _DLLS_63_H
#define _DLLS_63_H

#include "PR/ultratypes.h"
#include "dlls/engine/74_pic_menu.h"
#include "sys/menu.h"
#include "dll_def.h"

typedef struct {
    /*0*/ u8 isEmpty;
    /*1*/ u8 character;
    /*2*/ u8 spiritBits;
    /*3*/ u8 unk3;
    /*4*/ s16 timeHours;
    /*6*/ s16 timeMinutes;
    /*8*/ s16 timeSeconds;
    /*A*/ u16 unkA;
    /*C*/ char filename[6];
} GameSelectSaveInfo;

typedef struct {
    PicMenuItem *menuItems;
    s8 *textIDs;
    u8 count;
    u8 titleTextIdx;
    u8 buttonLegendTextIdx;
    u8 unkB;
} GameSelectSubmenu;

typedef enum {
    SUBMENU_GAME_SELECT = 0,
    SUBMENU_GAME_CONFIRM = 1,
    SUBMENU_COPY_SRC_SELECT = 2,
    SUBMENU_COPY_DST_SELECT = 3,
    SUBMENU_COPY_CONFIRM = 4,
    SUBMENU_ERASE_SELECT = 5,
    SUBMENU_ERASE_CONFIRM = 6,
    SUBMENU_GAME_RECAP = 7
} GameSelectSubmenuID;

typedef DLLInst_Menu DLLInst_63_gameselect;

#endif //_DLLS_63_H
