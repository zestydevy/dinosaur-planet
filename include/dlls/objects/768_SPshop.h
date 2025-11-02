#ifndef _DLLS_768_H
#define _DLLS_768_H

#include "PR/ultratypes.h"
#include "dlls/engine/29_gplay.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "sys/main.h"
#include "types.h"
#include "game/gamebits.h"

#define NONE 0xffff
#define EMPTY_ITEM {0, {0, 0, 0}, 0, 0, NONE, NONE, NONE, NONE, NONE}
#define GAMETEXT_BASE 63 //lineID of first shop string in inventory Gametext file
#define TEXT(roomID, itemID) (63 + itemID)

#define COLD_STORAGE 0
#define MAIN_ROOM 20
#define MAP_ROOM 40

DLL_INTERFACE(DLL_768_SPShop) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ u8 (*return_unk0)(Object* self);
    /*8*/ void (*play_sequence)(Object* self, s32 playSequence, s32 sequenceIndex);
    /*9*/ int (*is_item_shown)(Object* self, s32 itemIndex);
    /*10*/ int (*is_item_hidden)(Object* self, s32 itemIndex);
    /*11*/ u8 (*get_minimum_price)(Object* self, s32 itemIndex);
    /*12*/ u8 (*get_special_price)(Object* self, s32 itemIndex);
    /*13*/ u8 (*get_initial_price)(Object* self, s32 itemIndex);
    /*14*/ s16 (*get_item_gametext_index)(Object* self, s32 itemIndex);
    /*15*/ void (*set_current_item_index)(Object* self, s32 itemIndex);
    /*16*/ u8 (*get_current_item_index)(Object* self);
    /*17*/ void (*buy_item)(Object* self, s32 cost);
    /*18*/ void (*func18)(Object* self, s32 unk4);
    /*19*/ void (*func19)(Object* self, s32 dUnk3, s32 dUnk2);
    /*20*/ void (*func20)(Object* self, s32* getUnk3, s32* getUnk2, s32* getUnk4);
};

#endif //_DLLS_768_H
