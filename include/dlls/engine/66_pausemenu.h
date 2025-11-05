#ifndef _DLLS_66_H
#define _DLLS_66_H

#include "PR/ultratypes.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/74_picmenu.h"
#include "sys/menu.h"
#include "dll_def.h"

enum PauseScreenStates {
    PAUSE_MENU_MAIN = 0,
    PAUSE_MENU_GAME_SAVED = 1
};

#define GAME_TIME_CAPTION_X 74
#define GAME_TIME_CAPTION_Y 21

#define GAME_TIME_X 74
#define GAME_TIME_Y 36

#define COMPLETION_CAPTION_X 264
#define COMPLETION_CAPTION_Y 21

#define COMPLETION_X 264
#define COMPLETION_Y 36

#define SPELLSTONE_X 44
#define SPELLSTONE_Y 136

#define DUSTER_X 127
#define DUSTER_Y 162

#define SPIRIT_X 216
#define SPIRIT_Y 137

#define GAME_SAVED_X 160
#define GAME_SAVED_Y 115

#define DROP_SHADOW_X -1
#define DROP_SHADOW_Y -1

#define BEIGE_R 0xB7
#define BEIGE_G 0x8B
#define BEIGE_B 0x61
#define BEIGE_A 0xFF

#define DROP_SHADOW_R 0
#define DROP_SHADOW_G 0
#define DROP_SHADOW_B 0
#define DROP_SHADOW_A 0xFF

#define BG_OVERLAY_R 0
#define BG_OVERLAY_G 0
#define BG_OVERLAY_B 0

#define BG_OVERLAY_MAX_OPACITY 140
#define DROP_SHADOW_MULTIPLIER 150

DLL_INTERFACE(DLL_66_pausemenu) {
    DLL_INTERFACE_BASE(DLL_IMenu);
};

#endif //_DLLS_66_H
