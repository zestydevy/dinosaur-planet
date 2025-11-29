#ifndef _DLLS_1_H
#define _DLLS_1_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "game/objects/object.h"
#include "types.h"

typedef struct {
/*0*/ s16 flagObtained; //Gamebit that adds item to inventory
/*2*/ s16 flagHide; //Gamebit that removes item from inventory
/*4*/ s16 textureID; //Mapped through TEXTABLE.bin
/*6*/ u16 sidekickCommand; //Usually 0xFFFF, except on sidekick commands
/*8*/ s16 textID; //Gametext lineID (in either file #3 or 568, depending on upper byte)
/*A*/ s8 openPage; //Command menu page to open after use (-1 to close)
/*B*/ u8 unkB;
} InventoryItem;

typedef struct {
/*00*/ s32 unk0;
/*04*/ s32 unk4;
/*08*/ u32 unk8;
/*0C*/ s32 unkC;
/*10*/ f32 unk10;
/*14*/ s8 unk14;
/*15*/ s8 unk15;
/*16*/ s16 unk16;
/*18*/ Texture* unk18;
/*1C*/ s32 unk1C;
/*20*/ s32 unk20;
/*24*/ s32 unk24;
/*28*/ s32 unk28;
/*2C*/ s32 unk2C;
/*30*/ Texture* unk30;
} EnergyBar;

DLL_INTERFACE(DLL_1_UI) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ s32 (*func_308)(void);
/*1*/ UnknownDLLFunc func_35C;
/*2*/ UnknownDLLFunc func_BB8;
/*3*/ void (*func_1290)(void);
/*4*/ void (*func_12EC)(void);
/*5*/ UnknownDLLFunc func_F5C;
/*6*/ s32 (*func_DC4)(void);
/*7*/ s32 (*func_DF4)(s32 itemGamebitID); //Checks if a particular item was selected in the inventory (e.g. when giving roots to SnowHorn)?
/*8*/ s32 (*func_E2C)(s32 *arg0, s32 arg1);
/*9*/ s8 (*func_F24)(void);
/*10*/ s16 (*func_F40)(void); //get gamebitID of active submenu?
/*11*/ void (*func_70A0)(u8 arg0);
/*12*/ void (*func_130C)(u32 arg0, u32 arg1, u32 arg2);
/*13*/ void (*func_1338)(s32 gametextID, s32 arg1, s32 arg2);
/*14*/ void (*func_13F4)(void);
/*15*/ void (*func_2B8)(u16 arg0);
/*16*/ void (*func_6984)(s32 arg0);
/*17*/ void (*func_69F8)(s32 arg0, s32 arg1, s32 arg2);
/*18*/ void (*func_6B00)(s16 textureID, s32 arg1, s32 arg2);
/*19*/ void (*func_2E0)(u8 arg0);
/*20*/ UnknownDLLFunc func_70C8;
/*21*/ void (*func_7208)(s32 arg0);
/*22*/ void (*func_7550)(void);
/*23*/ UnknownDLLFunc func_75CC;
};

#endif //_DLLS_1_H
