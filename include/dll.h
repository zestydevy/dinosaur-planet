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
#include "dlls/engine/17.h"
#include "dlls/engine/18.h"
#include "dlls/engine/20_screens.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/22_subtitles.h"
#include "dlls/engine/23.h"
#include "dlls/engine/24_waterfx.h"
#include "dlls/engine/25.h"
#include "dlls/engine/26_curves.h"
#include "dlls/engine/27_head_turn.h"
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
#include "dlls/engine/74_pic_menu.h"
#include "dlls/engine/75.h"
#include "dlls/engine/76.h"

#include "dll_def.h"
#include "types.h"

enum DLL_ID
{
    DLL_UI = 1,
    DLL_CAMERA,
    DLL_ANIM,
    DLL_RACE,
    DLL_AMSEQ,
    DLL_AMSFX,
    DLL_NEWDAY,
    DLL_8,
    DLL_NEWCLOUDS,
    DLL_NEWSTARS,
    DLL_NEWLFX,
    DLL_MINIC,
    DLL_EXPGFX,
    DLL_MODGFX,
    DLL_PROJGFX,
    DLL_16,
    DLL_17,
    DLL_18,
    DLL_SCREENS = 20,
    DLL_TEXT,
    DLL_SUBTITLES,
    DLL_23,
    DLL_WATERFX,
    DLL_25,
    DLL_CURVES,
    DLL_27,
    DLL_SCREEN_FADE,
    DLL_GPLAY,
    DLL_TASK,
    DLL_FLASH,
    DLL_32,
    DLL_33,
    DLL_54 = 54,
    DLL_56 = 56,
    DLL_57,
    DLL_58,
    DLL_MINIMAP,
    DLL_PICMENU = 74,
    DLL_75,
    DLL_76,
    DLL_CANNON_CLAW = 223
};

// TODO: the below should be moved to main.h

// "Static" DLLs
// These are loaded on game init, are never unloaded, and are referenced by
// their global variable instead of from loading the DLL.

extern DLLInst_1_UI *gDLL_1_UI;
extern DLLInst_2_camera *gDLL_2_Camera;
extern DLLInst_3_animation *gDLL_3_Animation;
extern DLLInst_5_AMSEQ *gDLL_5_AMSEQ;
extern DLLInst_5_AMSEQ *gDLL_5_AMSEQ2;
extern DLLInst_6_AMSFX *gDLL_6_AMSFX;
extern DLLInst_7_newday *gDLL_7_Newday;
extern DLLInst_12_minic *gDLL_12_Minic;
extern DLLInst_16 *gDLL_16;
extern DLLInst_20_screens *gDLL_20_Screens;
extern DLLInst_21_gametext *gDLL_21_Gametext;
extern DLLInst_22_subtitles *gDLL_22_Subtitles;
extern DLLInst_23 *gDLL_23;
extern DLLInst_24_waterfx *gDLL_24_Waterfx;
extern DLLInst_25 *gDLL_25;
extern DLLInst_26_curves *gDLL_26_Curves;
extern DLLInst_27_head_turn *gDLL_27_HeadTurn; //"lookAt" DLL? (seems to have functions for aiming character necks etc!)
extern DLLInst_28_screen_fade *gDLL_28_ScreenFade;
extern DLLInst_29_gplay *gDLL_29_Gplay;
extern DLLInst_30_task *gDLL_30_Task;
extern DLLInst_31_flash *gDLL_31_Flash;
extern DLLInst_74_pic_menu *gDLL_74_PicMenu;
extern DLLInst_8 *gDLL_8;
extern DLLInst_9_newclouds *gDLL_9_Newclouds;
extern DLLInst_10_newstars *gDLL_10_Newstars;
extern DLLInst_4_race *gDLL_4_Race;
extern DLLInst_11_newlfx *gDLL_11_Newlfx;
extern DLLInst_57 *gDLL_57;
extern DLLInst_58 *gDLL_58;
extern DLLInst_13_expgfx *gDLL_13_Expgfx;
extern DLLInst_14_modgfx *gDLL_14_Modgfx;
extern DLLInst_15_projgfx *gDLL_15_Projgfx;
extern DLLInst_17 *gDLL_17;
extern DLLInst_18 *gDLL_18;
extern DLLInst_75 *gDLL_75;
extern DLLInst_56 *gDLL_56;
extern DLLInst_32 *gDLL_32;
extern DLLInst_33 *gDLL_33;
extern DLLInst_59_minimap *gDLL_59_Minimap;
extern DLLInst_54 *gDLL_54;

// Other common DLLs that aren't always loaded but are accessed globally

extern DLLInst_76 *gDLL_76;

#endif //_DLL_H
