#ifndef _DLLS_1_H
#define _DLLS_1_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "game/objects/object.h"
#include "types.h"

typedef struct {
/*0*/ s16 gamebitObtained;  //Gamebit that adds item to inventory
/*2*/ s16 gamebitHide;      //Gamebit that removes item from inventory
/*4*/ s16 textureID;        //Mapped through TEXTABLE.bin
/*6*/ s16 sidekickCommand;  //Usually -1, except on sidekick commands
/*8*/ s16 textID;           //Gametext lineID (in either file #3 or 568, depending on upper byte)
/*A*/ s8 openPage;          //Command menu page to open after use (-1 to close)
/*B*/ u8 unkB;
} InventoryItem;

/** For use with dll_1_func_F24 */
typedef enum {
    PAGE_1 = 1,
    PAGE_Sidekick = 2,
    PAGE_Inventory = 3,
    PAGE_Spells = 4,
    PAGE_5 = 5,
    PAGE_Foodbag = 6
} Cmdmenu_Pages;

DLL_INTERFACE(DLL_1_cmdmenu) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ s32 (*func_308)(void);
/*1*/ void (*func_35C)(void);
/*2*/ void (*print)(Gfx **gdl, Mtx **mtxs, Vertex **vtxs);
/*3*/ void (*func_1290)(void);
/*4*/ void (*func_12EC)(void);
/*5*/ s32 (*func5)(Object** a0, s32 a1, s32 a2, s32 a3, f32 a4); //unsure of types
/*6*/ s32 (*func_DC4)(void);
/*7*/ s32 (*func_DF4)(s32 itemGamebitID); //Checks if a particular item was selected in the inventory (e.g. when giving roots to SnowHorn)?
/*8*/ s32 (*func_E2C)(s32 *arg0, s32 arg1);
/*9*/ s8 (*func_F24)(void);
/*10*/ s16 (*func_F40)(void); //get gamebitID of active submenu?
/*11*/ void (*func_70A0)(u8 arg0);
/*12*/ void (*func_130C)(u32 arg0, u32 arg1, u32 arg2);
/*13*/ void (*func_1338)(s32 gametextID, s32 arg1, s32 arg2);
/*14*/ void (*func_13F4)(void);
/*15*/ void (*disable_buttons)(u16 mask);
/*16*/ void (*func_6984)(s32 arg0);
/*17*/ void (*func_69F8)(s32 arg0, s32 arg1, s32 arg2);
/*18*/ void (*func_6B00)(s16 textureID, s32 arg1, s32 arg2);
/*19*/ void (*toggle_forced_stats_display)(u8 force);
/*20*/ void (*energy_bar_create)(s32 minEnergy, s32 maxEnergy, s32 arg2, s32 arg3, s32 arg4);
/*21*/ void (*energy_bar_set)(s32 energy);
/*22*/ void (*energy_bar_free)(void);
/*23*/ void (*energy_bar_fadeout)(void);
};

#endif //_DLLS_1_H
