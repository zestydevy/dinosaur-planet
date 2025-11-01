#ifndef _DLL_H
#define _DLL_H

#include "dlls/engine/1_ui.h"
#include "dlls/engine/2_camera.h"
#include "dlls/engine/3_animation.h"
#include "dlls/engine/4_race.h"
#include "dlls/engine/5_amseq.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/7_newday.h"
#include "dlls/engine/8.h"
#include "dlls/engine/9_newclouds.h"
#include "dlls/engine/10_newstars.h"
#include "dlls/engine/11_newlfx.h"
#include "dlls/engine/12_minic.h"
#include "dlls/engine/13_expgfx.h"
#include "dlls/engine/14_modgfx.h"
#include "dlls/engine/15_projgfx.h"
#include "dlls/engine/16.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/20_screens.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/22_subtitles.h"
#include "dlls/engine/23.h"
#include "dlls/engine/24_waterfx.h"
#include "dlls/engine/25.h"
#include "dlls/engine/26_curves.h"
#include "dlls/engine/27.h"
#include "dlls/engine/28_screen_fade.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/30_task.h"
#include "dlls/engine/31_flash.h"
#include "dlls/engine/32.h"
#include "dlls/engine/33.h"
#include "dlls/engine/54.h"
#include "dlls/engine/56.h"
#include "dlls/engine/57.h"
#include "dlls/engine/58.h"
#include "dlls/engine/59_minimap.h"
#include "dlls/engine/74_picmenu.h"
#include "dlls/engine/75.h"
#include "dlls/engine/76.h"

#include "dll_def.h"
#include "types.h"

// Note: These are DLL IDs not indices
enum DLL_ID {
    // Engine
    DLL_ID_UI = 1,
    DLL_ID_CAMERA = 2,
    DLL_ID_ANIM = 3,
    DLL_ID_RACE = 4,
    DLL_ID_AMSEQ = 5,
    DLL_ID_AMSFX = 6,
    DLL_ID_NEWDAY = 7,
    DLL_ID_8 = 8,
    DLL_ID_NEWCLOUDS = 9,
    DLL_ID_NEWSTARS = 10,
    DLL_ID_NEWLFX = 11,
    DLL_ID_MINIC = 12,
    DLL_ID_EXPGFX = 13,
    DLL_ID_MODGFX = 14,
    DLL_ID_PROJGFX = 15,
    DLL_ID_16 = 16,
    DLL_ID_PARTFX = 17,
    DLL_ID_18 = 18,
    DLL_ID_SCREENS = 20,
    DLL_ID_TEXT = 21,
    DLL_ID_SUBTITLES = 22,
    DLL_ID_23 = 23,
    DLL_ID_WATERFX = 24,
    DLL_ID_25 = 25,
    DLL_ID_CURVES = 26,
    DLL_ID_27 = 27,
    DLL_ID_SCREEN_FADE = 28,
    DLL_ID_GPLAY = 29,
    DLL_ID_TASK = 30,
    DLL_ID_FLASH = 31,
    DLL_ID_32 = 32,
    DLL_ID_33 = 33,
    DLL_ID_34 = 34, // partfx related
    DLL_ID_35 = 35, // partfx related
    DLL_ID_36 = 36, // partfx related
    DLL_ID_37 = 37, // partfx related
    DLL_ID_38 = 38, // partfx related
    DLL_ID_39 = 39, // partfx related
    DLL_ID_40 = 40, // partfx related
    DLL_ID_41 = 41, // partfx related
    DLL_ID_42 = 42, // partfx related
    DLL_ID_43 = 43, // partfx related
    DLL_ID_44 = 44, // partfx related
    DLL_ID_45 = 45, // partfx related
    DLL_ID_46 = 46, // partfx related
    DLL_ID_47 = 47, // partfx related
    DLL_ID_48 = 48, // partfx related
    DLL_ID_49 = 49, // partfx related
    DLL_ID_50 = 50, // partfx related
    DLL_ID_51 = 51, // partfx related
    DLL_ID_52 = 52, // partfx related
    DLL_ID_54 = 54,
    DLL_ID_56 = 56,
    DLL_ID_57 = 57,
    DLL_ID_58 = 58,
    DLL_ID_MINIMAP = 59,
    DLL_ID_PICMENU = 74,
    DLL_ID_75 = 75,
    DLL_ID_76 = 76,

    // Mod gfx
    DLL_ID_106 = 0x1002,
    DLL_ID_107 = 0x1003,
    DLL_ID_122 = 0x1012,
    DLL_ID_136 = 0x1020,
    DLL_ID_147 = 0x102B,
    DLL_ID_148 = 0x102C,
    DLL_ID_149 = 0x102D,
    DLL_ID_155 = 0x1033,
    DLL_ID_170 = 0x1042,
    DLL_ID_182 = 0x104E,

    // Objects
    DLL_ID_CANNON_CLAW = 0x800E
};

// TODO: the below should be moved to main.h

// "Static" DLLs
// These are loaded on game init, are never unloaded, and are referenced by
// their global variable instead of from loading the DLL.

extern DLL_1_UI *gDLL_1_UI;
extern DLL_2_camera *gDLL_2_Camera;
extern DLL_3_animation *gDLL_3_Animation;
extern DLL_5_AMSEQ *gDLL_5_AMSEQ;
extern DLL_5_AMSEQ *gDLL_5_AMSEQ2;
extern DLL_6_AMSFX *gDLL_6_AMSFX;
extern DLL_7_newday *gDLL_7_Newday;
extern DLL_12_minic *gDLL_12_Minic;
extern DLL_16 *gDLL_16;
extern DLL_20_screens *gDLL_20_Screens;
extern DLL_21_gametext *gDLL_21_Gametext;
extern DLL_22_subtitles *gDLL_22_Subtitles;
extern DLL_23 *gDLL_23;
extern DLL_24_waterfx *gDLL_24_Waterfx;
extern DLL_25 *gDLL_25;
extern DLL_26_curves *gDLL_26_Curves;
extern DLL_27 *gDLL_27;
extern DLL_28_screen_fade *gDLL_28_ScreenFade;
extern DLL_29_gplay *gDLL_29_Gplay;
extern DLL_30_task *gDLL_30_Task;
extern DLL_31_flash *gDLL_31_Flash;
extern DLL_74_picmenu *gDLL_74_Picmenu;
extern DLL_8 *gDLL_8;
extern DLL_9_newclouds *gDLL_9_Newclouds;
extern DLL_10_newstars *gDLL_10_Newstars;
extern DLL_4_race *gDLL_4_Race;
extern DLL_11_newlfx *gDLL_11_Newlfx;
extern DLL_57 *gDLL_57;
extern DLL_58 *gDLL_58;
extern DLL_13_expgfx *gDLL_13_Expgfx;
extern DLL_14_modgfx *gDLL_14_Modgfx;
extern DLL_15_projgfx *gDLL_15_Projgfx;
extern DLL_17 *gDLL_17_partfx;
extern DLL_18_objfsa *gDLL_18_objfsa;
extern DLL_75 *gDLL_75;
extern DLL_56 *gDLL_56;
extern DLL_32 *gDLL_32;
extern DLL_33 *gDLL_33;
extern DLL_59_minimap *gDLL_59_Minimap;
extern DLL_54 *gDLL_54;

// Other common DLLs that aren't always loaded but are accessed globally

extern DLL_76 *gDLL_76;

#endif //_DLL_H
