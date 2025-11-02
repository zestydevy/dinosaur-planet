#ifndef _DLLS_768_H
#define _DLLS_768_H

#include "PR/ultratypes.h"
#include "dlls/engine/29_gplay.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "sys/main.h"
#include "types.h"
#include "game/gamebits.h"

#define GAMETEXT_BASE 63 //lineID of first shop string in inventory Gametext file

#define ITEM_COLD_STORAGE 0
#define ITEM_MAIN_ROOM 20
#define ITEM_MAP_ROOM 40

typedef enum {
    //Cold storage
    SPItem_Fruit = ITEM_COLD_STORAGE + 0,
    SPItem_Energy_Egg = ITEM_COLD_STORAGE + 1,
    SPItem_Red_Mushroom = ITEM_COLD_STORAGE + 2,
    SPItem_Red_Maggot = ITEM_COLD_STORAGE + 3,
    SPItem_Green_Magic = ITEM_COLD_STORAGE + 4,
    SPItem_Blue_Magic = ITEM_COLD_STORAGE + 5,

    //Main room
    SPItem_Fishing_Net = ITEM_MAIN_ROOM + 0,
    SPItem_Firefly_Lantern = ITEM_MAIN_ROOM + 1,
    SPItem_Translator = ITEM_MAIN_ROOM + 2,
    SPItem_Small_Sidekick_Foodbag = ITEM_MAIN_ROOM + 3,
    SPItem_Medium_Sidekick_Foodbag = ITEM_MAIN_ROOM + 4,
    SPItem_Medium_Magic_Chest = ITEM_MAIN_ROOM + 5,
    SPItem_Large_Magic_Chest_= ITEM_MAIN_ROOM + 6,
    SPItem_Small_Player_Foodbag = ITEM_MAIN_ROOM + 7,
    SPItem_Medium_Player_Foodbag = ITEM_MAIN_ROOM + 8,
    SPItem_Large_Player_Foodbag = ITEM_MAIN_ROOM + 9,

    //Map room
    SPItem_Dark_Ice_Mines_Map = ITEM_MAP_ROOM + 0,
    SPItem_Dragon_Rock_Map = ITEM_MAP_ROOM + 1,
    SPItem_SwapStone_Circle_Map = ITEM_MAP_ROOM + 2,
    SPItem_Moon_Mountain_Map_= ITEM_MAP_ROOM + 3,
    SPItem_Warlock_Replay_Disk = ITEM_MAP_ROOM + 4,
    SPItem_Ice_Mountain_Replay_Disk = ITEM_MAP_ROOM + 5,
    SPItem_Ice_Mountain_Map = ITEM_MAP_ROOM + 6
} ShopItemIndices;

typedef enum {
    //Cold storage
    SPText_Fruit = GAMETEXT_BASE + 0,
    SPText_Energy_Egg = GAMETEXT_BASE + 1,
    SPText_Red_Mushroom = GAMETEXT_BASE + 2,
    SPText_Red_Maggot = GAMETEXT_BASE + 3,
    SPText_Green_Magic = GAMETEXT_BASE + 4,
    SPText_Blue_Magic = GAMETEXT_BASE + 5,

    //Main room
    SPText_Fishing_Net = GAMETEXT_BASE + 6,
    SPText_Firefly_Lantern = GAMETEXT_BASE + 7,
    SPText_Translator = GAMETEXT_BASE + 8,
    SPText_Sidekick_Foodbag_S = GAMETEXT_BASE + 9,
    SPText_Sidekick_Foodbag_M = GAMETEXT_BASE + 10,
    SPText_Magic_Chest_M = GAMETEXT_BASE + 11,
    SPText_Magic_Chest_L= GAMETEXT_BASE + 12,
    SPText_Player_Foodbag_Small = GAMETEXT_BASE + 13,
    SPText_Player_Foodbag_Medium = GAMETEXT_BASE + 14,
    SPText_Player_Foodbag_Large = GAMETEXT_BASE + 15,

    //Map room
    SPText_Dark_Ice_Mines_Map = GAMETEXT_BASE + 16,
    SPText_Dragon_Rock_Map = GAMETEXT_BASE + 17,
    SPText_SwapStone_Circle_Map = GAMETEXT_BASE + 18,
    SPText_Moon_Mountain_Map= GAMETEXT_BASE + 19,
    SPText_Warlock_Replay_Disk = GAMETEXT_BASE + 20,
    SPText_Ice_Mountain_Replay_Disk = GAMETEXT_BASE + 21,
    SPText_Ice_Mountain_Map = GAMETEXT_BASE + 22
} ShopItemStrings;

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
