#ifndef _DLLS_1_H
#define _DLLS_1_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "game/objects/object.h"
#include "types.h"

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
    /*09*/ UnknownDLLFunc ui_func_f24;
    /*10*/ UnknownDLLFunc ui_func_f40;
    /*11*/ UnknownDLLFunc ui_func_70a0;
    /*12*/ UnknownDLLFunc ui_func_130c;
    /*13*/ UnknownDLLFunc ui_func_1338;
    /*14*/ UnknownDLLFunc ui_func_13f4;
    /*15*/ UnknownDLLFunc ui_func_2b8;
    /*16*/ UnknownDLLFunc ui_func_6984;
    /*17*/ UnknownDLLFunc ui_func_69f8;
    /*18*/ UnknownDLLFunc ui_func_6b00;
    /*19*/ UnknownDLLFunc ui_func_2e0;
    /*20*/ UnknownDLLFunc ui_func_70c8;
    /*21*/ UnknownDLLFunc ui_func_7208;
    /*22*/ UnknownDLLFunc ui_func_7550;
    /*23*/ UnknownDLLFunc ui_func_75cc;
};

#endif //_DLLS_1_H
