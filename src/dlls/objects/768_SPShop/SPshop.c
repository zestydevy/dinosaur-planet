#include "common.h"
#include "game/gamebits.h"
#include "game/objects/object_id.h"
#include "sys/map.h"
#include "sys/objtype.h"
#include "dlls/objects/210_player.h"

#include "dlls/objects/768_SPshop.h"

#define TOTAL_ITEMS 60

typedef struct {
/*0*/ u8 min;
/*1*/ u8 possibilities[3];
/*4*/ u8 specialOffer;
/*5*/ u8 initial;
} Price;

typedef struct {
/*0*/ s16 show;
/*2*/ s16 hide;
} CharacterSpecificGamebits;

typedef struct {
/*0*/ Price price;
/*6*/ CharacterSpecificGamebits sabre;
/*A*/ CharacterSpecificGamebits krystal;
/*C*/ s16 gametextLine;
} ShopItem;

typedef struct {
    s8 unk0;
    u8 itemIndex;
    s8 unk2;
    s8 unk3;
    s8 unk4;
} SPShop_Data;

static void SPShop_set_random_prices(void);

#define NONE 0xffff
#define EMPTY_ITEM {0, {0, 0, 0}, 0, 0, NONE, NONE, NONE, NONE, NONE}

/*0x0*/ static ShopItem shopItemData[TOTAL_ITEMS] = {
    {3,  {3,  4,  5},  1, 0, {BIT_ALWAYS_1, NONE},       {BIT_ALWAYS_1, NONE},       SPText_Fruit},
    {5,  {5,  7,  10}, 3, 0, {BIT_ALWAYS_1, NONE},       {BIT_ALWAYS_1, NONE},       SPText_Energy_Egg},
    {10, {13, 15, 18}, 0, 0, {BIT_ALWAYS_1, NONE},       {BIT_ALWAYS_0, NONE},       SPText_Red_Mushroom},
    {10, {13, 15, 18}, 0, 0, {BIT_ALWAYS_0, NONE},       {BIT_ALWAYS_1, NONE},       SPText_Red_Maggot},
    {8,  {8,  10, 12}, 6, 0, {BIT_SP_Green_Magic, NONE}, {BIT_SP_Green_Magic, NONE}, SPText_Green_Magic},
    {15, {18, 20, 25}, 0, 0, {BIT_SP_Blue_Magic, NONE},  {BIT_SP_Blue_Magic, NONE},  SPText_Blue_Magic},

    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,

    {25,  {28,  30,  35},  0,  0, {BIT_ALWAYS_1, NONE},                         {BIT_ALWAYS_1, NONE},                          SPText_Fishing_Net},
    {35,  {38,  40,  45},  30, 0, {BIT_ALWAYS_1, BIT_Bought_Firefly_Lantern},   {BIT_ALWAYS_1, BIT_Bought_Firefly_Lantern},    SPText_Firefly_Lantern},
    {100, {100, 100, 100}, 0,  0, {BIT_SP_Krazoa_Translator, NONE},             {BIT_SP_Krazoa_Translator, NONE},              SPText_Translator},
    {18,  {18,  20,  23},  15, 0, {BIT_ALWAYS_1, BIT_Sabre_Dino_Bag_S},         {BIT_ALWAYS_1, BIT_Krystal_Dino_Bag_S},        SPText_Sidekick_Foodbag_S},
    {35,  {38,  40,  45},  0,  0, {BIT_SP_Dino_Bag_M, BIT_Sabre_Dino_Bag_M},    {BIT_SP_Dino_Bag_M, BIT_Krystal_Dino_Bag_M},   SPText_Sidekick_Foodbag_M},
    {22,  {25,  28,  30},  20, 0, {BIT_SP_Magic_Chest_Medium, NONE},            {BIT_SP_Magic_Chest_Medium, NONE},             SPText_Magic_Chest_M},
    {50,  {50,  55,  60},  0,  0, {BIT_SP_Magic_Chest_Large, NONE},             {BIT_SP_Magic_Chest_Large, NONE},              SPText_Magic_Chest_L},
    {18,  {20,  23,  25},  0,  0, {BIT_SP_Foodbag_S, BIT_Sabre_Foodbag_S},      {BIT_SP_Foodbag_S, BIT_Krystal_Foodbag_S},     SPText_Player_Foodbag_Small},
    {35,  {38,  40,  45},  0,  0, {BIT_SP_Foodbag_M, BIT_Sabre_Foodbag_M},      {BIT_SP_Foodbag_M, BIT_Krystal_Foodbag_M},     SPText_Player_Foodbag_Medium},
    {57,  {60,  65,  70},  55, 0, {BIT_SP_Foodbag_L, BIT_Sabre_Foodbag_L},      {BIT_SP_Foodbag_L, BIT_Krystal_Foodbag_L},     SPText_Player_Foodbag_Large},

    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,

    {8,  {10, 12, 15}, 5,  0, {BIT_ALWAYS_1, BIT_Map_Dark_Ice_Mines},   {BIT_ALWAYS_0, BIT_Map_Dark_Ice_Mines},     SPText_Dark_Ice_Mines_Map},
    {30, {35, 38, 40}, 0,  0, {BIT_SP_Map_DR, NONE},                    {BIT_ALWAYS_0, NONE},                       SPText_Dragon_Rock_Map},
    {30, {35, 38, 40}, 0,  0, {BIT_ALWAYS_0, NONE},                     {BIT_ALWAYS_1, NONE},                       SPText_SwapStone_Circle_Map},
    {20, {20, 22, 25}, 0,  0, {BIT_ALWAYS_0, NONE},                     {BIT_SP_Map_MMP, NONE},                     SPText_Moon_Mountain_Map},
    {45, {50, 55, 60}, 40, 0, {BIT_ALWAYS_0, NONE},                     {BIT_SP_Replay_Disk_WM, NONE},              SPText_Warlock_Replay_Disk},
    {35, {38, 40, 43}, 0,  0, {BIT_SP_Replay_Disk_IM, NONE},            {BIT_ALWAYS_0, NONE},                       SPText_Ice_Mountain_Replay_Disk},
    {35, {38, 40, 43}, 0,  0, {BIT_ALWAYS_1, NONE},                     {BIT_ALWAYS_0, NONE},                       SPText_Ice_Mountain_Map},

    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM,
    EMPTY_ITEM
};

// offset: 0x0 | ctor
void SPShop_ctor(void *dll) { }

// offset: 0xC | dtor
void SPShop_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SPShop_setup(Object* self, ObjSetup* setup, s32 arg2) {
    obj_add_object_type(self, OBJTYPE_10);
    main_set_bits(BIT_SP_Exiting_Shop, 0);
    SPShop_set_random_prices();
}

// offset: 0x88 | func: 1 | export: 1
void SPShop_control(Object* self) {
    Object* player;

    player = get_player();
    if (self->unkDC == 0) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0, 1);
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 5, 1);

        if (player->id == OBJ_Krystal) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 6, 1);
        } else {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 7, 1);
        }

        func_80000860(self, self, 0x1C8, 0);
        func_80000860(self, self, 0x1CB, 0);
        func_80000450(self, self, 0x22F, 0, 0, 0);
        func_80000450(self, self, 0x231, 0, 0, 0);
        main_set_bits(BIT_SP_Entered_Shop, 1);
        gDLL_5_AMSEQ2->vtbl->set(NULL, 0xF3, 0, 0, 0);
        func_8001EBD0(1);
        self->unkDC = 1;
    }

    if (main_get_bits(BIT_SP_Exiting_Shop)) {
        if (player->id == OBJ_Sabre) {
            warpPlayer(WARP_SH_ROCKY_PODIUM, FALSE);
        } else {
            warpPlayer(WARP_SC_RUBBLE_PODIUM, FALSE);
        }
    }
}

// offset: 0x2D4 | func: 2 | export: 2
void SPShop_update(Object *self) { }

// offset: 0x2E0 | func: 3 | export: 3
void SPShop_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x334 | func: 4 | export: 4
void SPShop_free(Object* self, s32 arg1) {
    obj_free_object_type(self, 0xA);
}

// offset: 0x374 | func: 5 | export: 5
u32 SPShop_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x384 | func: 6 | export: 6
u32 SPShop_get_data_size(Object *self, u32 a1) {
    return sizeof(SPShop_Data);
}

// offset: 0x398 | func: 7 | export: 7
u8 SPShop_return_unk0(Object* self) {
    SPShop_Data* objData = self->data;
    return objData->unk0;
}

// offset: 0x3A8 | func: 8 | export: 8
void SPShop_play_sequence(Object* self, s32 playSequence, s32 sequenceIndex) {
    SPShop_Data* objData = self->data;
    
    objData->unk0 = playSequence;
    if (playSequence) {
        gDLL_3_Animation->vtbl->func17(sequenceIndex, self, -1);
    }
}

// offset: 0x40C | func: 9 | export: 9
/** 
    Checks if a particular shop item should appear for the current character.
    Uses gamebits to track whether or not to show it.

    Items will appear if a "show" gamebitID isn't specified (e.g. 0xFFFF),
    or by checking the value of the gamebit when a gamebitID is specified. 
*/
int SPShop_is_item_shown(Object* self, s32 itemIndex) {
    s16 gamebitID;
    ShopItem* item;

    item = &shopItemData[itemIndex];
    
    if (get_player()->id == OBJ_Sabre){
        gamebitID = item->sabre.show;
        return !(gamebitID + 1) || main_get_bits(gamebitID);
    } else {
        gamebitID = item->krystal.show;
        return !(gamebitID + 1) || main_get_bits(gamebitID);
    }
}

// offset: 0x4D8 | func: 10 | export: 10
/** 
    Checks if a particular normally-purchasable shop item should be hidden
    for the current character, e.g. after it's been purchased.

    Uses gamebits to track whether or not to hide it. Items will disappear
    if a "hide" gamebitID specified and the gamebit's value is currently nonzero. 
*/
int SPShop_is_item_hidden(Object* self, s32 itemIndex) {
    s16 gamebitID;
    ShopItem* item;

    item = &shopItemData[itemIndex];
    
    if (get_player()->id == OBJ_Sabre){
        gamebitID = item->sabre.hide;
        return (gamebitID + 1) && main_get_bits(gamebitID);
    } else {
        gamebitID = item->krystal.hide;
        return (gamebitID + 1) && main_get_bits(gamebitID);
    }
}

// offset: 0x5A4 | func: 11 | export: 11
/** Returns the ShopKeeper's minimum accepted price for a particular item */
u8 SPShop_get_minimum_price(Object* self, s32 itemIndex) {
    if (itemIndex >= 0 && itemIndex < TOTAL_ITEMS) {
        return shopItemData[itemIndex].price.min;
    }
    return 0;
}

// offset: 0x5E8 | func: 12 | export: 12
/** Returns the ShopKeeper's special offer price for a particular item (unused?) */
u8 SPShop_get_special_price(Object* self, s32 itemIndex) {
    if (itemIndex >= 0 && itemIndex < TOTAL_ITEMS) {
        return shopItemData[itemIndex].price.specialOffer;
    }
    return 0;
}

// offset: 0x62C | func: 13 | export: 13
/** Returns the ShopKeeper's (randomised) starting offer for a particular item */
u8 SPShop_get_initial_price(Object* self, s32 itemIndex) {
    if (itemIndex >= 0 && itemIndex < TOTAL_ITEMS) {
        return shopItemData[itemIndex].price.initial;
    }
    return 0;
}

// offset: 0x670 | func: 14 | export: 14
/** Returns the line index (in Gametext file #3) for a particular item's description */
s16 SPShop_get_item_gametext_index(Object* self, s32 itemIndex) {
    if (itemIndex >= 0 && itemIndex < TOTAL_ITEMS) {
        return shopItemData[itemIndex].gametextLine;
    }
    return 0;
}

// offset: 0x6B4 | func: 15 | export: 15
void SPShop_set_current_item_index(Object* self, s32 itemIndex) {
    SPShop_Data* objData = self->data;
    objData->itemIndex = itemIndex;
}

// offset: 0x6C4 | func: 16 | export: 16
u8 SPShop_get_current_item_index(Object* self) {
    SPShop_Data* objData = self->data;
    return objData->itemIndex;
}

// offset: 0x6D4 | func: 17 | export: 17
void SPShop_buy_item(Object* self, s32 cost) {
    Object* player = get_player();
    SPShop_Data* objData;
    s32 itemIndex;

    objData = self->data;

    //Subtract cost from player's scarabs
    ((DLL_210_Player*)player->dll)->vtbl->add_scarab(player, -cost);

    //Handle special cases (player energy/magic refills)
    switch (objData->itemIndex) {  
        case 2:
        case 3:
        case 6:
        case 7:
        case 27:
        case 45:
            break;
        case SPItem_Fruit:
            ((DLL_210_Player*)player->dll)->vtbl->add_health(player, 1);
            break;
        case SPItem_Energy_Egg:
            ((DLL_210_Player*)player->dll)->vtbl->add_health(player, 2);
            break;
        case SPItem_Green_Magic:
            ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 5);
            break;
        case SPItem_Blue_Magic:
            ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 10);
            break;
    }
    
    //Get the gamebitID to set upon purchase (if applicable)
    if (player->id == OBJ_Sabre) {
        itemIndex = shopItemData[objData->itemIndex].sabre.hide;
    } else {
        itemIndex = shopItemData[objData->itemIndex].krystal.hide;
    }
    
    //If specified, set the gamebit to hide the item after purchase
    if (itemIndex != -1) {
        main_set_bits(itemIndex, 1);
    }
}

// offset: 0x884 | func: 18 | export: 18
void SPShop_func_884(Object* self, s32 unk4) {
    SPShop_Data* objData = self->data;

    objData->unk2 = 0;
    objData->unk3 = 0;
    objData->unk4 = unk4;
}

// offset: 0x89C | func: 19 | export: 19
void SPShop_func_89C(Object* self, s32 dUnk3, s32 dUnk2) {
    SPShop_Data* objData = self->data;

    objData->unk2 += dUnk2;
    objData->unk3 += dUnk3;
}

// offset: 0x8C0 | func: 20 | export: 20
void SPShop_func_8C0(Object* self, s32* getUnk3, s32* getUnk2, s32* getUnk4) {
    SPShop_Data* objData = self->data;

    *getUnk2 = objData->unk2;
    *getUnk3 = objData->unk3;
    *getUnk4 = objData->unk4;
}

// offset: 0x8E4 | func: 21
/** Randomises the ShopKeeper's initial asking price for each item, picking one of 3 preset offers */
void SPShop_set_random_prices(void) {
    ShopItem* shopItem;
    s32 index;

    shopItem = shopItemData;

    for (index = 0; index < TOTAL_ITEMS; index++){
        shopItem[index].price.initial = shopItem[index].price.possibilities[rand_next(0, 2)];
    }
}
