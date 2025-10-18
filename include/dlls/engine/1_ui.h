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
    /*00*/ UnknownDLLFunc ui_func_308;
    /*01*/ UnknownDLLFunc ui_func_35c;
    /*02*/ UnknownDLLFunc ui_func_bb8;
    /*03*/ UnknownDLLFunc ui_func_1290;
    /*04*/ UnknownDLLFunc ui_func_12ec;
    /*05*/ UnknownDLLFunc ui_func_f5c;
    /*06*/ s32 (*func6)(void);
    /*07*/ s32 (*func7)(s32 inventory_item_flag); //Checks if a particular item was selected in the inventory (e.g. when giving roots to SnowHorn)?
    /*08*/ UnknownDLLFunc ui_func_e2c;
    /*09*/ s32 (*ui_func_f24)(void);
    /*10*/ UnknownDLLFunc ui_func_f40;
    /*11*/ UnknownDLLFunc ui_func_70a0;
    /*12*/ void (*ui_func_130c)(s32, s32, s32);
    /*13*/ UnknownDLLFunc ui_func_1338;
    /*14*/ UnknownDLLFunc ui_func_13f4;
    /*15*/ void (*ui_func_2b8)(s32);
    /*16*/ void (*ui_func_6984)(s32);
    /*17*/ UnknownDLLFunc ui_func_69f8;
    /*18*/ UnknownDLLFunc ui_func_6b00;
    /*19*/ UnknownDLLFunc ui_func_2e0;
    /*20*/ UnknownDLLFunc ui_func_70c8;
    /*21*/ UnknownDLLFunc ui_func_7208;
    /*22*/ UnknownDLLFunc ui_func_7550;
    /*23*/ UnknownDLLFunc ui_func_75cc;
};

#endif //_DLLS_1_H
