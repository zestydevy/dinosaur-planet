#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/engine/1_ui.h"
#include "dll.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/common/sidekick.h"
#include "game/gamebits.h"
#include "sys/gfx/textable.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/rcp.h"
#include "sys/vi.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/fonts.h"
#include "sys/rcp.h"
#include "macros.h"
#include "gbi_extra.h"
#include "prevent_bss_reordering2.h"

// 9slcommandmenu.c (default.dol)

typedef struct {
/*0*/ Texture* unk0;
/*4*/ s32 unk4;
/*8*/ f32 unk8;
/*C*/ s16 unkC;
} CmdmenuItemUnkBSS;

typedef struct {
/*0*/ InventoryItem* characterItems;
/*4*/ s16 unk4;
/*6*/ s16 unk6;
/*8*/ s16 unk8;
/*A*/ s16 unkA;
/*C*/ s16 unkC;
/*E*/ s16 unkE;
} UIUnknownCharacterStruct;

typedef struct {
    f32 unk0; //scarabUIOpacity
    f32 unk4; //playerIconMagicBarOpacity
    s32 unk8;
    s32 unkC; //healthValue
} CmdmenuBSS0;

// size: 0x18
typedef struct {
/*0*/ Texture* unk0;
/*4*/ s32 unk4;
/*8*/ s16 unk8;
/*8*/ s16 unkA;
/*C*/ s32 unkC;
/*10*/ s16 unk10;
/*12*/ s16 unk12;
/*14*/ s32 unk14;
} CmdmenuItemUnkBSS6B8;

typedef union { 
    struct {
        s32 unk0;
        s32 unk4; //magic
        s32 unk8; //scarabCount
        s32 unkC;
        s32 unk10;
        s32 unk14;
        s32 unk18; //maxHealth
        s32 unk1C; //maxMagic
        s32 unk20;
        s32 unk24;
    };
    s32 items[10];
} CmdmenuPlayerSidekickData;

typedef struct {
/*00*/ s32 unk0;
/*04*/ s32 unk4;
/*08*/ u32 unk8;
/*0C*/ u32 unkC;
/*10*/ f32 unk10;
/*14*/ u8 unk14;
/*15*/ s8 unk15;
/*16*/ s16 unk16;
/*18*/ Func_80037F9C_Struct Fullbartex[2]; // official name: Fullbartex (default.dol)
/*30*/ Func_80037F9C_Struct Emptybartex[2]; // official name: Emptybartex (default.dol)
/*48*/ s32 unk48;
/*4C*/ u32 unk4C_0 : 1;
} EnergyBar;

typedef union {
    struct {
        f32 unk0;
        u8 _unk4[0x8 - 0x4];
        f32 unk8;
        f32 unkC;
        u8 _unk10[0x14 - 0x10];
        f32 unk14;
        u8 _unk18[0x1C - 0x18];
        f32 unk1C;
    };
    f32 items[10];
} BSS60;

/*0x0*/ static s8 _data_0 = 0;
/*0x4*/ static s8 _data_4 = 0x00;
/*0x8*/ static s8 _data_8 = 0x01;
/*0xC*/ static s16 _data_C = 0x0048;
/*0x10*/ static s16 _data_10 = 0x0000;
/*0x14*/ static u32 _data_14 = 0x00000000;
/*0x18*/ static s16 _data_18 = 0xffff;
/*0x1C*/ static s8 _data_1C = 0;
/*0x20*/ static s16 _data_20 = 60;
/*0x24*/ static s16 _data_24 = 50;
/*0x28*/ static s16 _data_28 = 20;
/*0x2C*/ static s16 _data_2C = 160;
/*0x30*/ static s16 _data_30 = 0;
/*0x34*/ static char* _data_34[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x44*/ static s16 _data_44 = 0xffff;
/*0x48*/ static s8 _data_48 = 0;
/*0x4C*/ static s16 _data_4C = 0x0078;
/*0x50*/ static s16 _data_50 = 0x0050;
/*0x54*/ static s16 _data_54 = 0x0014;
/*0x58*/ static s16 _data_58 = 0x00a0;
/*0x5C*/ static s16 _data_5C = 0x0000;
/*0x60*/ static u32 _data_60[] = {
    0x00000000, 0xffff0000
};
/*0x68*/ static Texture* _data_68 = 0;
/*0x6C*/ static u32 _data_6C[] = {
    0x3f800000, 0xff000000
};
/*0x74*/ static u8 _data_74 = 0;
/*0x78*/ static u32 _data_78 = 0x00000000;
/*0x7C*/ static u32 _data_7C = 0x00000000;
/*0x80*/ static u8 _data_80 = 0;
/*0x84*/ static s16 _data_84 = 0x0000;
/*0x88*/ static s8 _data_88 = 1;
/*0x8C*/ static u16 _data_8C[] = {0x002d, 0x0040};
/*0x90*/ static u16 _data_90[] = {
    0x01d7, 0x05bd, 0x05ce, 0x05fc, 0x0777, 0x0000
};
/*0x9C*/ static u16 _data_9C[] = {0x0578, 0x057a};
/*0xA0*/ static u16 _data_A0[] = {
    0x057d, 0x057c, 0x057b, 0x057e, 0x0579, 0x0000
};
/*0xAC*/ static u16 _data_AC[] = {
    0x057f, 0x057f, 0x0581, 0x0580, 0x0582, 0x0583
};
/*0xB8*/ static s16 _data_B8[10][2] = {
    {-1, -1}, 
    {-1, -1}, 
    {-1, -1}, 
    {-1, -1}, 
    {-1, -1}, 
    {-1, -1}, 
    {-1, -1}, 
    {-1, -1}, 
    {0, 0}, 
    {0, 0}
};
/*0xE0*/ static u16 _data_E0[] = {
    0x0d05, 0x1310, 0x040a, 0x0c16, 
    0x0416, 0x0c20, 0x0d16, 0x1328, 
    0x140a, 0x1c16, 0x1416, 0x1c20, 
    0x0b00, 0x150a, 0x0204, 0x0c10, 
    0x0417, 0x0c28, 0x0b20, 0x152c, 
    0x1417, 0x1c28, 0x1404, 0x1e10, 
    0x080e, 0x1812, 0x0812, 0x1818
};
/*0x118*/ static s8 _data_118 = 0x00;
/*0x11C*/ static s8 _data_11C = 0x00;
/*0x120*/ static s8 _data_120 = 0x00;
/*0x124*/ static s8 _data_124 = 0x00;

#define NONE 0xFFFF
#define END {NONE, NONE, NONE, NONE, 0x0000, 0x00, 0x00}

/* Krystal items
 * 0x128*/ static InventoryItem _data_128[] = {
    {BIT_Krystal_Warp_Crystal,   BIT_Krystal_Used_Warp_Crystal,    0x245, NONE,  0x010, -1, 1}, //Warp Crystal
    {BIT_CRF_Prison_Key_1,       NONE,                             0x175, NONE,  0x007, -1, 1}, //Prison Key (CloudRunner Fortress)
    {BIT_CRF_Power_Room_Key,     NONE,                             0x176, NONE,  0x008, -1, 1}, //Power Room Key (CloudRunner Fortress)
    {BIT_CRF_Red_Power_Crystal,  NONE,                             0x180, NONE,  0x009, -1, 1}, //Red Power Crystal (CloudRunner Fortress)
    {BIT_CRF_Green_Power_Crystal,NONE,                             0x181, NONE,  0x009, -1, 1}, //Green Power Crystal (CloudRunner Fortress)
    {BIT_CRF_Blue_Power_Crystal, NONE,                             0x182, NONE,  0x009, -1, 1}, //Blue Power Crystal (CloudRunner Fortress)
    {BIT_IM_Snowbike_Key,        NONE,                             0x175, NONE,  0x011, -1, 1}, //Snowbike Key (Removed)
    {BIT_DIM_Mine_Key,           BIT_DIM_Used_Mine_Key,            0x175, NONE,  0x013, -1, 1}, //Mine Key (unused?)
    {BIT_DIM_Alpine_Roots,       NONE,                             0x1a3, NONE,  0x00b, -1, 1}, //Alpine Root (unused?)
    {BIT_DIM_Gear_1,             BIT_DIM_Used_Gear_1,              0x479, NONE,  0x00b, -1, 1}, //DIM Gear 1
    {BIT_DIM_Gear_2,            BIT_DIM_Used_Gear_2,              0x479,  NONE,  0x00b, -1, 1}, //DIM Gear 2
    {BIT_DIM_Gear_3,            BIT_DIM_Used_Gear_3,              0x479,  NONE,  0x00b, -1, 1}, //DIM Gear 3
    {BIT_DIM_Gear_4,            BIT_DIM_Used_Gear_4,              0x479,  NONE,  0x00b, -1, 1}, //DIM Gear 4
    {BIT_Krystal_Foodbag_S,     BIT_Krystal_Foodbag_M,            0x27a,  NONE,  0x016,  2, 2}, //Small Food Bag
    {BIT_Krystal_Foodbag_M,     BIT_Krystal_Foodbag_L,            0x244,  NONE,  0x017,  2, 2}, //Medium Food Bag
    {BIT_Krystal_Foodbag_L,     NONE,                             0x2bf,  NONE,  0x018,  2, 2}, //Large Food Bag
    {BIT_Krystal_Dino_Bag_S,    BIT_Krystal_Dino_Bag_M,           0x471,  NONE,  0x02f,  9, 2}, //Small Grub Bag
    {BIT_Krystal_Dino_Bag_M,    BIT_Krystal_Dino_Bag_L,           0x560,  NONE,  0x030,  9, 2}, //Medium Grub Bag
    {BIT_Krystal_Dino_Bag_L,    NONE,                             0x560,  NONE,  0x031,  9, 2}, //Large Grub Bag
    {BIT_Gold_Nugget_GP,        BIT_CC_Bribed_GuardClaw,          0x257,  NONE,  NONE,  -1, 1}, //Gold Nugget #1 (Near Golden Plains)
    {BIT_Gold_Nugget_LFV,       BIT_CC_Used_Cell_Door_Key,        0x257,  NONE,  NONE,  -1, 1}, //Gold Nugget #2 (Near LightFoot Village)
    {BIT_CC_Fire_Crystal,       NONE,                             0x4f7,  NONE,  0x102, -1, 1}, //Fire Crystal
    {BIT_CC_Cell_Door_Key,      BIT_CC_Used_Cell_Door_Key,        0x175,  NONE,  0x01e, -1, 1}, //Cell Door Key (Cape Claw?)
    {BIT_SpellStone_CRF,        NONE,                             0x562,  NONE,  NONE,  -1, 1}, //SpellStone - CloudRunner Fortress (Inactive)
    {BIT_SpellStone_BWC,        NONE,                             0x562,  NONE,  NONE,  -1, 1}, //SpellStone - BlackWater Canyon (Inactive)
    {BIT_SpellStone_KP,         NONE,                             0x562,  NONE,  NONE,  -1, 1}, //SpellStone - Krazoa Palace (Inactive)
    {BIT_Krazoa_Translator,     NONE,                             0x496,  NONE,  NONE,  -1, 1}, //Krazoa Translator
    {BIT_Horn_of_Truth,         NONE,                             0x258,  NONE,  0x01d, -1, 1}, //Horn of Truth
    {BIT_CRF_Treasure_Chest_Key,NONE,                             0x568,  NONE,  NONE,  -1, 1}, //Treasure Chest Key (CloudRunner Fortress)
    {BIT_CloudRunner_Grubs,     NONE,                             0x1a5,  NONE,  0x02a, -1, 1}, //Blue Mushroom (CloudRunner Grubs?)
    {BIT_Krystal_Fireflies,     NONE,                             0x46d,  NONE,  0x03d, -1, 1}, //Firefly Lantern
    {BIT_CRF_Prison_Key_2,      NONE,                             0x175,  NONE,  0x007, -1, 1}, //Prison Key (CloudRunner Fortress)
    {BIT_CC_Krazoa_Tablets,     NONE,                             0x496,  NONE,  0x05b, -1, 1}, //Krazoa Tablets
    {BIT_Inventory_MoonSeeds,   NONE,                             0x40d,  NONE,  0x05e, -1, 1}, //MoonSeeds
    {BIT_CC_Engineers_Key,      NONE,                             0x175,  NONE,  0x064, -1, 1}, //Construction Engineer's Key (CloudRunner Fortress)
    {BIT_Gold_Nugget_CC,        BIT_CC_Bribed_GuardClaw,          0x257,  NONE,  0x103, -1, 1}, //Gold Nugget #3 (Cape Claw)
    END
};

/* Sabre items
 * 0x2E4*/ static InventoryItem _data_2E4[] = {
    {BIT_SP_Krazoa_Translator,       NONE,                         0x175, NONE,  0x0a,  -1, 1}, //Gate Key (Northern Wastes) (Unused?)
    {BIT_SW_Alpine_Roots,            NONE,                         0x1a3, NONE,  0x0b,  -1, 1}, //Alpine Root (Geyser area)
    {BIT_Inventory_Blue_Mushrooms,   NONE,                         0x1a5, NONE,  0x0c,  -1, 1}, //Blue Mushroom
    {BIT_Inventory_Purple_Mushrooms, NONE,                         0x498, NONE,  0x57,  -1, 1}, //Purple Mushroom
    {BIT_Inventory_White_Mushrooms,  NONE,                         0x497, NONE,  0x56,  -1, 1}, //White Mushroom
    {BIT_DIM_Mine_Key,               BIT_DIM_Used_Mine_Key,        0x175, NONE,  0x13,  -1, 1}, //Mine Key
    {BIT_DIM_Alpine_Roots,           NONE,                         0x1a3, NONE,  0x0b,  -1, 1}, //Alpine Root (DIM)
    {BIT_DIM_Gear_1,                 BIT_DIM_Used_Gear_1,          0x479, NONE,  0x0b,  -1, 1}, //DIM Gear 1 
    {BIT_DIM_Gear_2,                 BIT_DIM_Used_Gear_2,          0x479, NONE,  0x0b,  -1, 1}, //DIM Gear 2
    {BIT_DIM_Gear_3,                 BIT_DIM_Used_Gear_3,          0x479, NONE,  0x0b,  -1, 1}, //DIM Gear 3
    {BIT_DIM_Gear_4,                BIT_DIM_Used_Gear_4,          0x479,  NONE,  0x0b,  -1, 1}, //DIM Gear 4
    {BIT_Belina_Te_Cell_Key,        BIT_Used_Belina_Te_Cell_Key,  0x175,  NONE,  0x19,  -1, 1}, //Belina Te Cell Key (DarkIce Mines)
    {BIT_Tricky_Cell_Key,           BIT_Used_Tricky_Cell_Key,     0x175,  NONE,  0x19,  -1, 1}, //Tricky Cell Key (DarkIce Mines)
    {BIT_Sabre_Warp_Crystal,        BIT_Sabre_Used_Warp_Crystal,  0x245,  NONE,  0x10,  -1, 1}, //Warp Crystal
    {BIT_DIM_Door_Key_1,            BIT_DIM_Used_Door_Key_1,      0x175,  NONE,  0x19,  -1, 1}, //Door Key 1 (DarkIce Mines)
    {BIT_DIM_Door_Key_2,            BIT_DIM_Used_Door_Key_2,      0x175,  NONE,  0x19,  -1, 1}, //Door Key 2 (DarkIce Mines)
    {BIT_Sabre_Foodbag_S,           BIT_Sabre_Foodbag_M,          0x27a,  NONE,  0x16,   3, 2}, //Small Food Bag
    {BIT_Sabre_Foodbag_M,           BIT_Sabre_Foodbag_L,          0x244,  NONE,  0x17,   3, 2}, //Medium Food Bag
    {BIT_Sabre_Foodbag_L,           NONE,                         0x2bf,  NONE,  0x18,   3, 2}, //Large Food Bag
    {BIT_Sabre_Dino_Bag_S,          BIT_Sabre_Dino_Bag_M,         0x471,  NONE,  0x32,  10, 2}, //Small Dinosaur Food Bag
    {BIT_Sabre_Dino_Bag_M,          BIT_Sabre_Dino_Bag_L,         0x560,  NONE,  0x33,  10, 2}, //Medium Dinosaur Food Bag
    {BIT_Sabre_Dino_Bag_L,          NONE,                         0x560,  NONE,  0x34,  10, 2}, //Large Dinosaur Food Bag
    {BIT_Inventory_Corrupt_Item,    NONE,                         0x175,  NONE,  0x1d,  -1, 1}, //Corrupted? A key with "ForceField Spell (Cape Claw)" as its description
    {BIT_Horn_of_Truth,             NONE,                         0x258,  NONE,  0x1d,  -1, 1}, //Horn of Truth
    {BIT_Sabre_Fireflies,           NONE,                         0x46d,  NONE,  0x3d,  -1, 1}, //Firefly Lantern
    {BIT_WC_Silver_Tooth,           BIT_WC_Used_Silver_Tooth,     0x52b,  NONE,  0x00,  -1, 1}, //Silver Tooth (Walled City)
    {BIT_WC_Gold_Tooth,             BIT_WC_Used_Gold_Tooth,       0x52a,  NONE,  0x00,  -1, 1}, //Gold Tooth (Walled City)
    {BIT_WC_Sun_Stone,              BIT_WC_Used_Sun_Stone,        0x3a8,  NONE,  0x1b,  -1, 1}, //Sun Stone (Walled City)
    {BIT_WC_Moon_Stone,             BIT_WC_Used_Moon_Stone,       0x3d6,  NONE,  0x1c,  -1, 1}, //Moon Stone (Walled City)
    {BIT_Inventory_MoonSeeds,       NONE,                         0x40d,  NONE,  0x62,  -1, 1}, //MoonSeeds
    {BIT_SpellStone_DIM,            BIT_SpellStone_DIM_Activated, 0x562,  NONE,  0x60,  -1, 1}, //SpellStone - DarkIce Mines (Inactive)
    {BIT_SpellStone_DIM_Activated,  NONE,                         0x563,  NONE,  0x61,  -1, 1}, //SpellStone - DarkIce Mines (Activated) 
    {BIT_SpellStone_WC,             NONE,                         0x180,  NONE,  NONE,  -1, 1}, //Spellstone - Walled City (Inactive) (Incorrect icon: Red Power Crystal, CRF)
    {BIT_SpellStone_DR,             NONE,                         0x180,  NONE,  NONE,  -1, 1}, //Spellstone - Dragon Rock (Inactive) (Incorrect icon: Red Power Crystal, CRF)
    {BIT_DB_PointBack_Egg,          NONE,                         0x55e,  NONE,  0x5e,  -1, 1}, //PointBack Egg (Diamond Bay)
    {BIT_DB_Bay_Diamond,            NONE,                         0x55f,  NONE,  0x5f,  -1, 1}, //Bay Diamond
    END
};

/* Food Bag actions (Krystal)
 * 0x4A0*/ static InventoryItem _data_4A0[] = {
    {BIT_Foodbag_Eat,               NONE, 0x2c7, NONE, 0x1f, 4, 2}, //Eat food in foodbag
    {BIT_Foodbag_Place,             NONE, 0x2c8, NONE, 0x20, 4, 2}, //Place food down from foodbag
    {BIT_Foodbag_Give,              NONE, 0x2ef, NONE, 0x21, 4, 2}, //Give food from foodbag
    {BIT_Foodbag_Setting_Eat_First, NONE, 0x265, NONE, 0x39, 2, 2}, //Eat later (NOTE: option to switch shown when "Eat First" enabled)
    {BIT_Foodbag_Setting_Eat_Later, NONE, 0x529, NONE, 0x3a, 2, 2}, //Eat first (NOTE: option to switch shown when "Eat Later" enabled)
    END
};

/* Food Bag actions (Sabre)
 * 0x4E8*/ static InventoryItem _data_4E8[] = {
    {BIT_Foodbag_Eat,               NONE, 0x2c7, NONE, 0x1f, 5, 2}, //Eat food in foodbag
    {BIT_Foodbag_Place,             NONE, 0x2c8, NONE, 0x20, 5, 2}, //Place food down from foodbag
    {BIT_Foodbag_Give,              NONE, 0x2ef, NONE, 0x21, 5, 2}, //Give food from foodbag
    {BIT_Foodbag_Setting_Eat_First, NONE, 0x265, NONE, 0x39, 3, 2}, //Eat later (NOTE: option to switch shown when "Eat First" enabled)
    {BIT_Foodbag_Setting_Eat_Later, NONE, 0x529, NONE, 0x3a, 3, 2}, //Eat first (NOTE: option to switch shown when "Eat Later" enabled)
    END
};

/* Food Bag items (Krystal)
 * 0x530*/ static InventoryItem _data_530[] = {
    {BIT_Green_Apple_Count,  NONE,                  0x26e, NONE, 0x22, -1, 1}, //Green apple
    {BIT_Red_Apple_Count,    NONE,                  0x26f, NONE, 0x23, -1, 1}, //Red apple
    {BIT_Brown_Apple_Count,  NONE,                  0x270, NONE, 0x24, -1, 1}, //Brown Apple
    {BIT_Fish_Count,         NONE,                  0x272, NONE, 0x25, -1, 1}, //Fish
    {BIT_Smoked_Fish_Count,  NONE,                  0x2c6, NONE, 0x26, -1, 1}, //Smoked Fish
    {BIT_Dino_Egg_Count,     NONE,                  0x2c4, NONE, 0x27, -1, 1}, //Dino Egg
    {BIT_Moldy_Meat_Count,   NONE,                  0x2c5, NONE, 0x28, -1, 1}, //Moldy Meat
    {BIT_Green_Bean_Count,   NONE,                  0x270, NONE, 0x35, -1, 1}, //Green Bean
    {BIT_Red_Bean_Count,     NONE,                  0x270, NONE, 0x36, -1, 1}, //Red Bean
    {BIT_Brown_Bean_Count,   NONE,                  0x270, NONE, 0x37, -1, 1}, //Brown Bean
    {BIT_Blue_Bean_Count,   NONE,                  0x270,  NONE, 0x38, -1, 1}, //Blue Bean
    {BIT_Krystal_Foodbag_S, BIT_Krystal_Foodbag_M, 0x27a,  NONE, 0x16,  2, 2}, //Small Food Bag
    {BIT_Krystal_Foodbag_M, BIT_Krystal_Foodbag_L, 0x244,  NONE, 0x17,  2, 2}, //Medium Food Bag
    {BIT_Krystal_Foodbag_L, NONE,                  0x2bf,  NONE, 0x18,  2, 2}, //Large Food Bag
    END
};

/* Food Bag items (Sabre)
 * 0x5E4*/ static InventoryItem _data_5E4[] = {
    {BIT_Green_Apple_Count,  NONE,                  0x26e, NONE, 0x22, -1, 1}, //Green apple
    {BIT_Red_Apple_Count,    NONE,                  0x26f, NONE, 0x23, -1, 1}, //Red apple
    {BIT_Brown_Apple_Count,  NONE,                  0x270, NONE, 0x24, -1, 1}, //Brown Apple
    {BIT_Fish_Count,         NONE,                  0x272, NONE, 0x25, -1, 1}, //Fish
    {BIT_Smoked_Fish_Count,  NONE,                  0x2c6, NONE, 0x26, -1, 1}, //Smoked Fish
    {BIT_Dino_Egg_Count,     NONE,                  0x2c4, NONE, 0x27, -1, 1}, //Dino Egg
    {BIT_Moldy_Meat_Count,   NONE,                  0x2c5, NONE, 0x28, -1, 1}, //Moldy Meat
    {BIT_Green_Bean_Count,   NONE,                  0x270, NONE, 0x35, -1, 1}, //Green Bean 
    {BIT_Red_Bean_Count,     NONE,                  0x270, NONE, 0x36, -1, 1}, //Red Bean
    {BIT_Brown_Bean_Count,   NONE,                  0x270, NONE, 0x37, -1, 1}, //Brown Bean
    {BIT_Blue_Bean_Count,   NONE,                  0x270,  NONE, 0x38, -1, 1}, //Blue Bean
    {BIT_Sabre_Foodbag_S,   BIT_Sabre_Foodbag_M,   0x27a,  NONE, 0x16,  3, 2}, //Small Food Bag
    {BIT_Sabre_Foodbag_M,   BIT_Sabre_Foodbag_L,   0x244,  NONE, 0x17,  3, 2}, //Medium Food Bag
    {BIT_Sabre_Foodbag_L,   NONE,                  0x2bf,  NONE, 0x18,  3, 2}, //Large Food Bag
    END
};

/* Magic Spells
 * 0x698*/ static InventoryItem _data_698[] = {
    {BIT_Spell_Projectile, NONE,    0x0183, NONE, 0x0d, -1, 0}, //Projectile Spell
    {BIT_Spell_Ice_Blast,  NONE,    0x0468, NONE, 0x3c, -1, 0}, //Ice Blast Spell
    {BIT_Spell_Grenade,    NONE,    0x04f8, NONE, 0x0f, -1, 0}, //Grenade Spell (Incorrectly labelled as Randorn)
    {BIT_Spell_Illusion,   NONE,    0x0177, NONE, 0x0e, -1, 0}, //Illusion Spell
    {BIT_Spell_Forcefield, NONE,    0x0265, NONE, 0x14, -1, 0}, //ForceField Spell
    {BIT_Spell_Glitched_1, NONE,    0x0183, NONE, 0x0d, -1, 0}, //Glitched Spell
    {BIT_Spell_Glitched_2, BIT_348, 0x0529, NONE, 0x0d, -1, 0}, //Glitched Spell (Maybe "Quake", mentioned elsewhere?)
    {BIT_Spell_Portal,     NONE,    0x0466, NONE, 0x3b, -1, 0}, //Portal Spell
    {BIT_Spell_Mind_Read,  NONE,    0x0470, NONE, 0x3e, -1, 0}, //Mind Read
    END
};

/* Sidekick Commands (Kyte)
 * 0x710*/ static InventoryItem _data_710[] = {
    {0x01, 0, 0x1cd, 0x1cd, 0, 0, 0}, //Heel
    {0x20, 5, 0x1d1, 0x1d1, 6, 0, 0}, //Play
    {0x02, 1, 0x1ce, 0x1ce, 1, 0, 0}, //Find
    {0x10, 4, 0x1cf, 0x1cf, 2, 0, 0}, //Flame
    {0x04, 2, 0x1d0, 0x1d0, 3, 0, 0}, //Distract
    {0x08, 3, 0x1d2, 0x1d2, 4, 0, 0}, //Guard
    END
};

/* Sidekick Commands (Tricky)
 * 0x764*/ static InventoryItem _data_764[] = {
    {0x01, 0, 0x1cd, 0x1cd, 0, 0, 0}, //Heel
    {0x20, 5, 0x1d1, 0x1d1, 6, 0, 0}, //Play
    {0x02, 1, 0x1ce, 0x1ce, 1, 0, 0}, //Find
    {0x10, 4, 0x1cf, 0x1cf, 2, 0, 0}, //Flame
    {0x04, 2, 0x1d0, 0x1d0, 3, 0, 0}, //Distract
    {0x08, 3, 0x1d2, 0x1d2, 4, 0, 0}, //Guard
    END
};

/* Dinosaur Food Bag Actions (Krystal)
 * 0x7B8*/ static InventoryItem _data_7B8[] = {
    {BIT_Dino_Foodbag_Place, NONE, 0x2c8, NONE, 0x20, 11, 2}, //Place food down from foodbag
    {BIT_Dino_Foodbag_Give,  NONE, 0x561, NONE, 0x21, 11, 2}, //Give food from foodbag
    END
};

/* Dinosaur Food Bag Actions (Sabre)
 * 0x7DC*/ static InventoryItem _data_7DC[] = {
    {BIT_Dino_Foodbag_Place, NONE, 0x2c8, NONE, 0x20, 12, 2}, //Place food down from foodbag
    {BIT_Dino_Foodbag_Give,  NONE, 0x561, NONE, 0x21, 12, 2}, //Give food from foodbag
    END
};

/* Dinosaur Food Bag Items (Kyte)
 * 0x800*/ static InventoryItem _data_800[] = {
    {BIT_Krystal_Dino_Bag_S,      BIT_Krystal_Dino_Bag_M, 0x27a, NONE, 0x2f,  9, 2}, //Small Mushroom Bag
    {BIT_Krystal_Dino_Bag_M,      BIT_Krystal_Dino_Bag_L, 0x244, NONE, 0x30,  9, 2}, //Medium Mushroom Bag
    {BIT_Krystal_Dino_Bag_L,      NONE,                   0x2bf, NONE, 0x31,  9, 2}, //Large Mushroom Bag
    {BIT_Dino_Bag_Blue_Mushrooms, NONE,                   0x270, NONE, 0x0c, -1, 1}, //Blue Mushroom
    {BIT_Dino_Bag_Red_Mushrooms,  NONE,                   0x270, NONE, 0x2d, -1, 1}, //Red Mushroom
    {BIT_Dino_Bag_Old_Mushrooms,  NONE,                   0x270, NONE, 0x2e, -1, 1}, //Old Mushroom
    {BIT_Dino_Bag_Blue_Grubs,     NONE,                   0x270, NONE, 0x2a, -1, 1}, //Blue Grub
    {BIT_Dino_Bag_Red_Grubs,      NONE,                   0x270, NONE, 0x2b, -1, 1}, //Red Grub
    {BIT_Dino_Bag_Old_Grubs,      NONE,                   0x270, NONE, 0x2c, -1, 1}, //Old Grub
    END
};

/* Dinosaur Food Bag Items (Tricky)
 * 0x878*/ static InventoryItem _data_878[] = {
    {BIT_Sabre_Dino_Bag_S,        BIT_Sabre_Dino_Bag_M,   0x27a, NONE, 0x32, 10, 2}, //Small Mushroom Bag
    {BIT_Sabre_Dino_Bag_M,        BIT_Sabre_Dino_Bag_L,   0x244, NONE, 0x33, 10, 2}, //Medium Mushroom Bag
    {BIT_Sabre_Dino_Bag_L,        NONE,                   0x2bf, NONE, 0x34, 10, 2}, //Large Mushroom Bag
    {BIT_Dino_Bag_Blue_Mushrooms, NONE,                   0x270, NONE, 0x0c, -1, 1}, //Blue Mushroom
    {BIT_Dino_Bag_Red_Mushrooms,  NONE,                   0x270, NONE, 0x2d, -1, 1}, //Red Mushroom
    {BIT_Dino_Bag_Old_Mushrooms,  NONE,                   0x270, NONE, 0x2e, -1, 1}, //Old Mushroom
    {BIT_Dino_Bag_Blue_Grubs,     NONE,                   0x270, NONE, 0x2a, -1, 1}, //Blue Grub
    {BIT_Dino_Bag_Red_Grubs,      NONE,                   0x270, NONE, 0x2b, -1, 1}, //Red Grub
    {BIT_Dino_Bag_Old_Grubs,      NONE,                   0x270, NONE, 0x2c, -1, 1}, //Old Grub
    END
};

//Krystal
/*0x8F0*/ static UIUnknownCharacterStruct _data_8F0 = {
    _data_128, 0x0000, 0x0000, 0x0008, 0x0001, 0x0000, 0x0001 };

//Sabre
/*0x900*/ static UIUnknownCharacterStruct _data_900[] = {{
    _data_2E4, 0x0000, 0x0000, 0x0008, 0x0001, 0x0000, 0x0001 }};

/*0x910*/ static UIUnknownCharacterStruct _data_910[] = {{
    _data_4A0, 0x0000, 0x0000, 0x0008, 0x0001, 0x0000, 0x0001 }};
/*0x920*/ static UIUnknownCharacterStruct _data_920[] = {{
    _data_4E8, 0x0000, 0x0000, 0x0008, 0x0001, 0x0000, 0x0001 }};
/*0x930*/ static UIUnknownCharacterStruct _data_930[] = {{
    _data_530, 0x0000, 0x0000, 0x0008, 0x0001, 0x0000, 0x0001 }};
/*0x940*/ static UIUnknownCharacterStruct _data_940[] = {{
    _data_5E4, 0x0000, 0x0000, 0x0008, 0x0001, 0x0000, 0x0001 }};
/*0x950*/ static UIUnknownCharacterStruct _data_950[] = {{
    _data_698, 0x0000, 0x0000, 0x0008, 0x0002, 0x0000, 0x0002 }};

/*0x960*/ static u32 _data_960 = (u32)&_data_710;
/*0x964*/ static u32 _data_964[] = {
    0x00000000, 0x00080009, 0x00000004
};
/*0x970*/ static u32 _data_970 = (u32)&_data_764;
/*0x974*/ static u32 _data_974[] = {
    0x00000000, 0x00080009, 0x00000004
};
/*0x980*/ static u32 _data_980[] = {
    (u32)&_data_7B8, 0x00000000, 0x00080001, 0x00000001
};
/*0x990*/ static u32 _data_990[] = {
    (u32)&_data_7DC, 0x00000000, 0x00080001, 0x00000001
};
/*0x9A0*/ static u32 _data_9A0[] = {
    (u32)&_data_800, 0x00000000, 0x00080001, 0x00000001
};
/*0x9B0*/ static u32 _data_9B0[] = {
    (u32)&_data_878, 0x00000000, 0x00080001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/*0x9D0*/ static s8 _data_9D0 = 0; //index of cmdmenu page currently open
/*0x9D4*/ static u32 _data_9D4 = 0x00000000;
/*0x9D8*/ static s16 _data_9D8[] = {
    0x01be, 0x01bf, 0x1c0, 0x01c2, 0x01c3, 0x01c4, 0x01c5, 0x01c6, 
    0x03e2, 0x03e3, 0x03e4, 0x03e5, 0x0421, 0x01c9, 0x03ea, 0x03eb, 
    0x01cb, 0x03e6, 0x03e7, 0x03f5, 0x03f6, 0x03f7, 0x03f8, 0x03f9,
    0x03fa, 0x03fb, 0x03fc, 0x03fd, 0x03fe, 0x0422, 0x0423, 0x0429, 
    0x042a, 0x042b, 0x042c, 0x0424, 0x042d, 0x0431, 0x01d4, 0x042f,
    0x0432, 0x0430, 0x0433, 0x042e, 0x0435, 0x0436, 0x0438, 0x043a, 
    0x043b, 0x043d, 0x043e, 0x0434, 0x0437, 0x0267, 0x0268, 0x0476,
    0x0477, 0x0478, 0x0000, 0x0000
};

/*0x0*/ static f32 _bss_0; //scarabUIOpacity
/*0x4*/ static f32 _bss_4; //playerIconMagicBarOpacity
/*0x8*/ static f32 _bss_8;
/*0xC*/ static s32 _bss_C;
/*0x10*/ static CmdmenuPlayerSidekickData _bss_10; //Tricky food level is stored in here! Maybe a struct?
/*0x38*/ static CmdmenuPlayerSidekickData _bss_38;
/*0x60*/ static BSS60 _bss_60;
/*0x88*/ static u8 _bss_88;
/*0x89*/ static u8 _bss_89;
/*0x8A*/ static u8 _bss_8A;
/*0x8B*/ static u8 _bss_8B;
/*0x8C*/ static f32 _bss_8C;
/*0x90*/ static EnergyBar* _bss_90; 
/*0x98*/ static u8 _bss_98[0x8]; //Pointers to inventory icon textures
/*0xA0*/ static u8 _bss_A0[0xf8];
/*0x198*/ static u8 _bss_198[0x8];
/*0x1A0*/ static u8 _bss_1A0[0xf8];
/*0x298*/ static u8 _bss_298[0x8];
/*0x2A0*/ static u8 _bss_2A0[0x78];
/*0x318*/ static u8 _bss_318[0x8];
/*0x320*/ static u8 _bss_320[0xf8];
/*0x418*/ static s16 _bss_418[4]; //array of textIDs
/*0x420*/ static u8 _bss_420[0x78];
/*0x498*/ static s8 _bss_498[8]; //array of unkAs (from InventoryItem)
/*0x4A0*/ static u8 _bss_4A0[0x38];
/*0x4D8*/ static s8 _bss_4D8[8]; //array of unkBs (from InventoryItem)
/*0x4E0*/ static u8 _bss_4E0[0x38];
/*0x518*/ static u8 _bss_518[0x8];
/*0x520*/ static u8 _bss_520[0x38];
/*0x558*/ static u8 _bss_558[0x8];
/*0x560*/ static u8 _bss_560[0x38];
/*0x598*/ static Texture* _bss_598;
/*0x59C*/ static Texture* _bss_59C;
/*0x5A0*/ static Texture* _bss_5A0;
/*0x5A4*/ static s16 _bss_5A4;
/*0x5A8*/ static Texture* _bss_5A8;
/*0x5AC*/ static Texture* _bss_5AC;
/*0x5B0*/ static s16 _bss_5B0;
/*0x5B4*/ static s32 _bss_5B4;
/*0x5B8*/ static u32 _bss_5B8;
/*0x5BC*/ static s32 _bss_5BC;
/*0x5C0*/ static s32 _bss_5C0[2];
/*0x5C8*/ static Texture* _bss_5C8[58];
/*0x6B0*/ static Texture* _bss_6B0[2]; // TODO: not an array?
/*0x6B8*/ static Func_80037F9C_Struct _bss_6B8[58][2];
/*0xC28*/ static s16 _bss_C28;
/*0xC2A*/ static s16 _bss_C2A;
/*0xC2C*/ static s16 _bss_C2C;
/*0xC2E*/ static s16 _bss_C2E;
/*0xC30*/ static GameTextChunk* _bss_C30;
/*0xC34*/ static Texture* _bss_C34;
/*0xC38*/ static s16 usedItemGamebitID;
/*0xC3A*/ static s16 _bss_C3A;
/*0xC3C*/ static s8 _bss_C3C; //set to 0 when item selection successful
/*0xC3D*/ static s8 _bss_C3D;
/*0xC3E*/ static u8 _bss_C3E[0x2];
/*0xC40*/ static u8 _bss_C40[0x4];
/*0xC44*/ static u8 _bss_C44[0x4];
/*0xC48*/ static s8 _bss_C48;
/*0xC4C*/ static u8 _bss_C4C[0x4];
/*0xC50*/ static s32 _bss_C50;
/*0xC54*/ static s32 _bss_C54; //controllerButtons
/*0xC58*/ static s32 _bss_C58;
/*0xC5C*/ static u8 _bss_C5C[0x4];
/*0xC60*/ static Func_80037F9C_Struct _bss_C60[2];
/*0xC78*/ static s8 _bss_C78;
/*0xC7A*/ static s16 _bss_C7A;
/*0xC7C*/ static u16 _bss_C7C;
/*0xC7E*/ static u16 _bss_C7E;
/*0xC80*/ static u16 _bss_C80;
/*0xC88*/ static CmdmenuItemUnkBSS _bss_C88;
// /*0xC90*/ static u8 _bss_C90[0x10];

static void dll_1_func_69CC(CmdmenuItemUnkBSS* arg0);
void dll_1_func_7550(void);

// offset: 0x0 | ctor
void dll_1_ctor(s32 arg0) {
    s32 i;

    _bss_C78 = -1;
    _bss_C7A = -1;
    usedItemGamebitID = -1;
    _bss_C3C = 0;
    _bss_C3D = -1;
    _bss_5A4 = -1;
    _bss_5B0 = -1;

    for (i = 0; i < 58; i++) {
        _bss_5C8[i] = tex_load_deferred(_data_9D8[i]);
        _bss_6B8[i][0].unk0 = tex_load_deferred(_data_9D8[i]);
        _bss_6B8[i][0].unk4 = 0;
        _bss_6B8[i][0].unk8 = 0;
        _bss_6B8[i][0].unkA = 0;
        _bss_6B8[i][1].unk0 = 0;
    }

    _bss_C34 = tex_load_deferred(TEXTABLE_500_SpellCrosshair);
    _bss_C34->animSpeed = 0x28;
    _bss_5B8 = 0x80000;
    _bss_5C0[0] = 0;
    _bss_6B0[0] = tex_load_deferred(TEXTABLE_3A7_InventoryStackNumbers);
    _bss_5A0 = tex_load_deferred(TEXTABLE_274_AButton_Anim);
    _bss_5A0->animSpeed = 0x28;
    
    dll_1_func_69CC(&_bss_C88);
}

// offset: 0x180 | dtor
void dll_1_dtor(s32 arg0) {
    s32 i;

    for (i = 0; i < (s32)ARRAYCOUNT(_bss_5C8); i++) {
        if (_bss_5C8[i] != NULL) {
            tex_free(_bss_5C8[i]);
        }
    }
    tex_free(_bss_6B0[0]);
    tex_free(_bss_C34);
    if (_bss_598 != NULL) {
        tex_free(_bss_598);
    }
    if (_bss_59C != NULL) {
        tex_free(_bss_59C);
    }
    if (_bss_5A8 != NULL) {
        tex_free(_bss_5A8);
    }
    if (_bss_5AC != NULL) {
        tex_free(_bss_5AC);
    }
    if (_bss_5A0 != NULL) {
        tex_free(_bss_5A0);
    }
}

// offset: 0x2B8 | func: 0 | export: 15
void dll_1_func_2B8(u16 arg0) {
    _data_74 = arg0;
}

// offset: 0x2E0 | func: 1 | export: 19
void dll_1_func_2E0(u8 arg0) {
    _data_80 = arg0;
}

// offset: 0x308 | func: 2 | export: 0
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_308.s")
#else
/* static */ void dll_1_func_1410(void);
/* static */ void dll_1_func_1FEC(void);

s32 dll_1_func_308(void) {
    dll_1_func_1FEC();
    dll_1_func_1410();
    return 0;
}
#endif

// offset: 0x35C | func: 3 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_35C.s")

// offset: 0xBB8 | func: 4 | export: 2
void dll_1_func_BB8(Gfx **gdl, Mtx **mtxs, Vertex **vtxs);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_BB8.s")

// offset: 0xDC4 | func: 5 | export: 6
s32 dll_1_func_DC4(void) {
    if (usedItemGamebitID >= 0) {
        return 1;
    }
    return 0;
}

// offset: 0xDF4 | func: 6 | export: 7
//cmdmenu_was_item_selected
int dll_1_func_DF4(s32 itemGamebitID) {
    if (itemGamebitID == usedItemGamebitID) {
        _bss_C3C = 0;
        return TRUE;
    }
    return FALSE;
}

// offset: 0xE2C | func: 7 | export: 8
s32 dll_1_func_E2C(s32 *arg0, s32 arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_E2C.s")

// offset: 0xF24 | func: 8 | export: 9
// cmdmenu_get_pageID?
s8 dll_1_func_F24(void) {
    return _data_9D0;
}

// offset: 0xF40 | func: 9 | export: 10
s16 dll_1_func_F40(void) {
    return _bss_C3A;
}

// offset: 0xF5C | func: 10 | export: 5
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_F5C.s")
#else
// https://decomp.me/scratch/fX0HX
s32 dll_1_func_F5C(Object **arg0, s32 arg1, u8 arg2, s32 arg3, f32 arg4) {
    s32 _pad[2];
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fv0;
    Camera* temp_s2;
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    s32 _sp8C_pad[2];
    s32 sp88;
    s32 sp84;
    Object* temp_s0;
    Object** temp_v0;
    Object* temp_a0;
    Object* temp_v1;
    s32 var_a3;
    s32 var_s1;
    s32 var_s4;
    s32 var_v1;

    set_camera_selector(0);
    temp_s2 = get_main_camera();
    temp_v0 = get_world_objects(&sp84, &sp88);
    var_s4 = 0;
    for (var_s1 = sp84; var_s1 < sp88; var_s1++) {
        temp_s0 = temp_v0[var_s1];
        if ((temp_s0->def->unk40 != NULL) && (temp_s0->opacity == 0xFF) && !(temp_s0->unkAF & 8) && 
                (temp_s0->def->unk40->flags & arg2) && (var_s4 < arg1) && (arg3 & 1)) {
            get_object_child_position(temp_s0, &sp9C, &sp98, &sp94);
            temp_fa0 = sp9C - temp_s2->srt.transl.x;
            temp_fv0 = sp98 - temp_s2->srt.transl.y;
            temp_fa1 = sp94 - temp_s2->srt.transl.z;
            if ((SQ(temp_fa0) + SQ(temp_fv0) + SQ(temp_fa1)) < SQ(arg4)) {
                var_v1 = temp_s2->srt.yaw - ((0x4000 - arctan2_f(temp_fa0, temp_fa1)) & 0xFFFF);
                CIRCLE_WRAP(var_v1);
                if ((var_v1 < -0x2710) && (var_v1 > -0x55F0)) {
                    arg0[var_s4] = temp_s0;
                    var_s4 += 1;
                }
            }
        }
    }

    if (var_s4 > 0) {
        do {
            var_a3 = 1;
            for (var_s1 = 0; var_s1 < (var_s4 - 1); var_s1++) {
                temp_v1 = arg0[var_s1];
                temp_a0 = arg0[var_s1 + 1];
                if ((s32)temp_v1 < (s32)temp_a0) {
                    arg0[var_s1] = temp_a0;
                    arg0[var_s1 + 1] = temp_v1;
                    var_a3 = 0;
                }
            }
        } while (var_a3 == 0);
    }
    
    return var_s4;
}
#endif

// offset: 0x1290 | func: 11 | export: 3
void dll_1_func_1290(void) {
    s32 i;
    UIUnknownCharacterStruct *temp;

    temp = &_data_8F0;

    for (i = 0; temp[i].characterItems; i++) { temp[i].unk4 = 0; }
    usedItemGamebitID = -1;
    _bss_C3C = 0;
    _bss_C3D = -1;
}

// offset: 0x12EC | func: 12 | export: 4
void dll_1_func_12EC(void) {
    _bss_88 = 1;
}

// offset: 0x130C | func: 13 | export: 12
void dll_1_func_130C(u32 arg0, u32 arg1, u32 arg2) {
    _bss_C7C = arg0;
    _bss_C7E = arg1;
    _bss_C80 = arg2;
}

// offset: 0x1338 | func: 14 | export: 13
void dll_1_func_1338(s32 gametextID, s32 arg1, s32 arg2) {
    if (_bss_C30 == NULL) {
        _bss_5B4 = 0x40000;
        _bss_5BC = 0;
        _bss_C30 = gDLL_21_Gametext->vtbl->get_chunk(gametextID);
        _bss_C2E = 0;
        _data_48 = 1;
        _data_58 = arg1;
        _data_54 = arg2;
        _bss_C2C = _data_50;
    }
}

// offset: 0x13F4 | func: 15 | export: 14
void dll_1_func_13F4(void) {
    _data_48 = 0;
}

// offset: 0x1410 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_1410.s")

// offset: 0x1614 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_1614.s")

// offset: 0x1FEC | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_1FEC.s")

// offset: 0x26D8 | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_26D8.s")

// offset: 0x27D8 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_27D8.s")

// offset: 0x325C | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_325C.s")

// offset: 0x3718 | func: 22
s32 dll_1_func_3718(InventoryItem* arg0, s8 arg1) {
    Object* sidekick;
    s32 var_s1;
    s32 var_v1;
    s32 i;

    var_s1 = 0;
    i = 0;
    if (arg1 == 0) {
        while (arg0[i].flagObtained >= 0) {
            if (main_get_bits(arg0[i].flagObtained) != 0) {
                if (arg0 == _data_698) {
                    var_s1 += 1;
                } else if ((arg0[i].flagHide < 0) || (main_get_bits(arg0[i].flagHide) == 0)) {
                    var_s1 += 1;
                }
            }
            i++;
        }
    } else {
        sidekick = get_sidekick();
        if (sidekick != NULL) {
            var_v1 = ((DLL_ISidekick*)sidekick->dll)->vtbl->func13(sidekick);
        } else {
            var_v1 = 0;
        }
        if (var_v1 > 0) {
            while (arg0[i].flagObtained >= 0) {
                if (arg0[i].flagObtained != 0) {
                    var_s1 += 1;
                }
                i++;
            }
        }
    }
    return var_s1;
}

// offset: 0x3880 | func: 23
void dll_1_func_3880(InventoryItem* items, s32 loadedItemIndex, s32 itemIndex) {
    _bss_418[loadedItemIndex] = items[itemIndex].textID;
    _bss_498[loadedItemIndex] = items[itemIndex].openPage;
    _bss_4D8[loadedItemIndex] = items[itemIndex].unkB;
}

// offset: 0x38E4 | func: 24
static void dll_1_func_38E4(Gfx** gdl, Texture* tex, s32 frame) {
    Gfx* dl;
    s32 i;

    dl = *gdl;
    for (i = 0; i < frame; i++) {
        if (tex->next != NULL) {
            tex = tex->next;
        }
    }
    dl->words.w0 = tex->gdl->words.w0;
    dl->words.w1 = (u32) (OS_PHYSICAL_TO_K0(tex + 1));
    dl++;
    gSPDisplayList(dl++, OS_PHYSICAL_TO_K0(tex->gdl + 1));
    tex_render_reset();
    *gdl = dl;
}

// offset: 0x39FC | func: 25
s32 dll_1_func_39FC(void) {
    if (_data_0 == 0) {
        return 0;
    }
    if (_bss_C28 != _data_C) {
        return 0;
    }
    return 1;
}

// offset: 0x3A4C | func: 26
s32 dll_1_func_3A4C(void) {
    if (_data_0 != 0) {
        return 0;
    }
    if (_bss_C28 != 0) {
        return 0;
    }
    return 1;
}

// offset: 0x3A94 | func: 27
void dll_1_func_3A94(void) {
    _data_0 = 0;
}

// offset: 0x3AB0 | func: 28
void dll_1_func_3AB0(void) {
    _data_0 = 1;
}

// offset: 0x3AD0 | func: 29
void dll_1_func_3AD0(void) {
    _data_4 -= gUpdateRate << _data_8;
    if (_data_4 < 0) {
        _data_4 = 0;
    }
    if (_data_0 != 0) {
        _data_10 += gUpdateRate * 8;
        if (_data_10 > 255) {
            _data_10 = 255;
        }
    } else if (_bss_C28 == 0) {
        _data_10 -= gUpdateRate * 8;
        if (_data_10 < 0) {
            _data_10 = 0;
        }
    }
    if ((_data_0 != 0) && (_data_10 > 64)) {
        _bss_C28 += gUpdateRate * 4;
        if (_data_C < _bss_C28) {
            _bss_C28 = _data_C;
        } else {
            _data_118 += 1;
            _data_11C -= 1;
        }
    } else {
        _bss_C28 -= gUpdateRate * 4;
        if (_bss_C28 < 0) {
            _bss_C28 = 0;
        } else {
            _data_118 -= 1;
            _data_11C += 1;
        }
    }
    if (_data_10 != 0) {
        if (_data_4 > 0) {
            _data_118 += 1;
            _data_11C += 1;
        }
        while (_data_118 >= 3) {
            _data_118 -= 3;
        }
        while (_data_118 < 0) {
            _data_118 += 3;
        }
        while (_data_11C >= 3) {
            _data_11C -= 3;
        }
        while (_data_11C < 0) {
            _data_11C += 3;
        }
    }
}

// offset: 0x3D28 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/1_cmdmenu/dll_1_func_3D28.s")

// offset: 0x4630 | func: 31
void dll_1_func_4630(Gfx **gdl) {
    if (func_80041E08() != 0) {
        gDPSetScissorFrac((*gdl)++, G_SC_NON_INTERLACE, 
            qu102(314.75), 
            qu102(48.25), 
            qu102(366.75), 
            (((f32) _bss_C28 * 0.82f) + 48.38f + 2.0f) * 4.0f);
    } else {
        gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 
            262, 
            59, 
            294, 
            _bss_C28 + 0x3D);
    }
}

// offset: 0x474C | func: 32
void dll_1_func_474C(Gfx** gfx) {
    s32 resolution;

    resolution = vi_get_current_size();
    gDPSetScissor((*gfx)++, 0 , 0, 0, GET_VIDEO_WIDTH(resolution), GET_VIDEO_HEIGHT(resolution));
}

// offset: 0x47E8 | func: 33
s32 dll_1_func_47E8(void) {
    if (_data_1C != 0) {
        return 0;
    }
    if (_bss_C2A != 0) {
        return 0;
    }
    return 1;
}

// offset: 0x4830 | func: 34
void dll_1_func_4830(void) {
    _data_1C = 0;
}

// offset: 0x484C | func: 35
void dll_1_func_484C(void) {
    _data_1C = 1;
}

// offset: 0x486C | func: 36
void dll_1_func_486C(void) {
    if ((_data_1C != 0) && (_data_84 == 0)) {
        _data_30 += gUpdateRate * 32;
        if (_data_30 > 160) {
            _data_30 = 160;
        }
    } else if (_bss_C2A == 0) {
        _data_30 -= gUpdateRate * 32;
        if (_data_30 < 0) {
            _data_30 = 0;
        }
    }
    if ((_data_1C != 0) && (_data_30 == 160)) {
        _bss_C2A += gUpdateRate * 4;
        if (_data_24 < _bss_C2A) {
            _bss_C2A = _data_24;
        } else {
            _data_120 += 1;
            _data_124 -= 1;
        }
    } else {
        _bss_C2A -= gUpdateRate * 4;
        if (_bss_C2A < 0) {
            _bss_C2A = 0;
        } else {
            _data_120 -= 1;
            _data_124 += 1;
        }
    }
    if (_data_30 == 0) {
        if (*_data_34 != NULL) {
            mmFree(*_data_34);
            *_data_34 = NULL;
        }
    } else {
        while (_data_120 >= 3) {
            _data_120 -= 3;
        }
        while (_data_120 < 0) {
            _data_120 += 3;
        }
        while (_data_124 >= 3) {
            _data_124 -= 3;
        }
        while (_data_124 < 0) {
            _data_124 += 3;
        }
        _data_120 = 0;
        _data_124 = 0;
    }
}

// offset: 0x4AD4 | func: 37
// Draws the info box at the top of the screen describing the highlighted/z-locked item.
void dll_1_func_4AD4(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    s32 sp10C;
    s32 y;
    s32 sp104;
    s32 height;
    s32 tempY;
    s32 i;
    Gfx* dl;

    if (_data_30 == 0) {
        return;
    }
    dl = *gdl;
    gDPSetCombineMode(dl, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    dl_apply_combine(&dl);
    gDPSetOtherMode(dl,
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
    dl_apply_other_mode(&dl);
    gSPLoadGeometryMode(dl, G_SHADE | G_SHADING_SMOOTH);
    dl_apply_geometry_mode(&dl);
    dl_set_prim_color(&dl, 255, 255, 255, _data_30);
    sp10C = _data_2C << 2;
    sp104 = _data_20 << 2;
    sp104 -= (16 << 2);
    y = _data_28 << 2;
    height = _bss_C2A << 2;

    dll_1_func_38E4(&dl, _bss_5C8[6], 0);
    gSPTextureRectangle(dl++, 
        /*ulx*/ sp10C - sp104,
        /*uly*/ y,
        /*lrx*/ sp10C + sp104,
        /*lry*/ y + height,
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(0), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
    );
    gDLBuilder->needsPipeSync = 1;

    dll_1_func_38E4(&dl, _bss_5C8[5], 0);
    gSPTextureRectangle(dl++, 
        /*lrx*/ sp10C - sp104 - (16 << 2),
        /*lry*/ y,
        /*ulx*/ sp10C - sp104,
        /*uly*/ y + height,
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(0), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
    );
    gDLBuilder->needsPipeSync = 1;
    gSPTextureRectangle(dl++, 
        /*ulx*/ sp10C + sp104,
        /*uly*/ y,
        /*lrx*/ sp10C + sp104 + (16 << 2),
        /*lry*/ y + height,
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(15), /*t*/ qs105(0),
        /*dsdx*/ qs510(-1), /*dtdy*/ qs510(1)
    );
    gDLBuilder->needsPipeSync = 1;

    sp104 += (16 << 2);

    if (_bss_C2A >= 9) {
        dll_1_func_38E4(&dl, _bss_5C8[7], 0);
        dl_set_prim_color(&dl, 255, 128, 128, 128);
        
        tempY = y;
        gSPTextureRectangle(dl++, 
            /*ulx*/ sp10C - sp104,
            /*uly*/ tempY,
            /*lrx*/ sp10C + sp104,
            /*lry*/ tempY + (8 << 2),
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(7), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
        );
        gDLBuilder->needsPipeSync = 1;

        tempY = (y + height) - (6 << 2);
        gSPTextureRectangle(dl++, 
            /*ulx*/ sp10C - sp104,
            /*uly*/ tempY,
            /*lrx*/ sp10C + sp104,
            /*lry*/ tempY + (8 << 2),
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(0), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
        );
        gDLBuilder->needsPipeSync = 1;

        dl_set_prim_color(&dl, 255, 255, 255, _data_30);
    }

    tempY = y - (11 << 2);
    dll_1_func_38E4(&dl, _bss_5C8[4], _data_120);
    gSPTextureRectangle(dl++, 
        /*ulx*/ sp10C - sp104,
        /*uly*/ tempY,
        /*lrx*/ sp10C + sp104,
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = 1;

    tempY = (y + height) - (4 << 2);
    dll_1_func_38E4(&dl, _bss_5C8[4], _data_124);
    gSPTextureRectangle(dl++, 
        /*ulx*/ sp10C - sp104,
        /*uly*/ tempY,
        /*lrx*/ sp10C + sp104,
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = 1;

    dll_1_func_38E4(&dl, _bss_5C8[3], 0);
    tempY = y - (11 << 2);
    gSPTextureRectangle(dl++, 
        /*ulx*/ sp10C - sp104 - (16 << 2),
        /*uly*/ tempY,
        /*lrx*/ sp10C - sp104,
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = 1;
    gSPTextureRectangle(dl++, 
        /*ulx*/ sp10C + sp104,
        /*uly*/ tempY,
        /*lrx*/ sp10C + sp104 + (16 << 2),
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(15), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(-1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = 1;
    tempY = (y + height) - (4 << 2);
    gSPTextureRectangle(dl++, 
        /*ulx*/ sp10C - sp104 - (16 << 2),
        /*uly*/ tempY,
        /*lrx*/ sp10C - sp104,
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = 1;
    gSPTextureRectangle(dl++, 
        /*ulx*/ sp10C + sp104,
        /*uly*/ tempY,
        /*lrx*/ sp10C + sp104 + (16 << 2),
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(15), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(-1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = 1;

    dl_set_prim_color(&dl, 255, 255, 255, 255);

    if (_data_44 < 0) {
        return;
    }
    if (_data_34[0] == NULL) {
        _data_34[0] = _data_44 >= 256 
            ? gDLL_21_Gametext->vtbl->get_text(568, _data_44 - 256) 
            : gDLL_21_Gametext->vtbl->get_text(3, _data_44);
        _data_34[1] = NULL;
        _data_34[2] = NULL;
        _data_34[3] = NULL;
        sp10C = 0;
        i = 1;
        while (_data_34[0][sp10C] != '\0') {
            if (_data_34[0][sp10C] == '|') {
                _data_34[0][sp10C] = '\0';
                sp10C += 2;
                _data_34[i] = &_data_34[0][sp10C];
                i += 1;
            }
            sp10C += 1;
        }
    }
    if (_bss_C2A > 0) {
        font_window_set_coords(3, 
            /*x1*/ _data_2C - _data_20, 
            /*y1*/ _data_28, 
            /*x2*/ _data_2C + _data_20, 
            /*y2*/ _data_28 + _bss_C2A);
        font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
        font_window_set_bg_colour(3, 0, 0, 0, 0);
        font_window_flush_strings(3);
        font_window_set_text_colour(3, 0, 0, 255, 255, 255);
        y = 3;
        for (i = 0; i < 4; i++) {
            if (_data_34[i] == NULL) {
                break;
            }
            font_window_add_string_xy(3, -0x8000, y, _data_34[i], 1, ALIGN_TOP_CENTER);
            font_window_set_text_colour(3, 20, 20, 20, 255, 255);
            font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
            y += 16;
        }
        font_window_draw(&dl, mtxs, vtxs, 3);
    }
    *gdl = dl;
}

// offset: 0x5608 | func: 38
void dll_1_func_5608(void) {
    Object* player;
    Object* sidekick;
    CmdmenuPlayerSidekickData sp80;
    f32 temp_fv0;
    s16 temp_v0;
    u8 var_s2;
    u8 i;

    player = get_player();
    sidekick = get_sidekick();
    var_s2 = 0;
    sp80.items[0] = ((DLL_210_Player*)player->dll)->vtbl->get_health(player);
    sp80.items[7] = ((DLL_210_Player*)player->dll)->vtbl->get_health_max(player);
    if (sidekick != NULL) {
        sp80.items[1] = ((DLL_ISidekick*)sidekick->dll)->vtbl->func15(sidekick);
        sp80.items[4] = ((DLL_ISidekick*)sidekick->dll)->vtbl->func16(sidekick);
        sp80.items[9] = 8;
    } else {
        sp80.items[1] = 0;
        sp80.items[4] = 0;
        sp80.items[9] = 0;
    }
    sp80.items[2] = ((DLL_210_Player*)player->dll)->vtbl->get_magic(player);
    sp80.items[8] = ((DLL_210_Player*)player->dll)->vtbl->get_magic_max(player);
    sp80.items[3] = ((DLL_210_Player*)player->dll)->vtbl->get_scarabs(player);
    if (((DLL_210_Player*)player->dll)->vtbl->func21(player) != 0) {
        var_s2 = 7;
    }
    if (_bss_8B < var_s2) {
        _bss_8B = var_s2;
    }
    if (_bss_C50 & 0x10) {
        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_5EA, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if ((_bss_C58 & 0x10) || (gDLL_2_Camera->vtbl->get_target_object() != NULL) || ((_data_80 != 0) && (camera_get_letterbox() == 0))) {
        sp80.items[5] = _bss_38.unk14 + 1;
    } else {
        sp80.items[5] = _bss_38.unk14;
    }
    if (_bss_C58 & 0x10) {
        _bss_8C += 8.5f * gUpdateRateF;
        if (_bss_8C > 255.0f) {
            _bss_8C = 255.0f;
        }
    } else {
        _bss_8C -= 8.5f * gUpdateRateF;
        if (_bss_8C < 0.0f) {
            _bss_8C = 0.0f;
        }
    }
    _bss_8C = _bss_8C < _bss_0 ? _bss_8C : _bss_0;
    sp80.items[6] = 0;
    if (_bss_88 & 1) {
        _bss_88 &= ~1;
        for (i = 0; i < 10; i++) {
            _bss_10.items[i] = sp80.items[i];
            _bss_38.items[i] = sp80.items[i];
            _bss_60.items[i] = -30.0f;
        }
        _bss_0 = 0.0f;
        return;
    }

    for (i = 0; i < 10; i++) {
        temp_fv0 = _bss_60.items[i];
        _bss_60.items[i] = temp_fv0 - gUpdateRateF;
        if (temp_fv0 > 60.0f) {
            if (_bss_60.items[i] <= 60.0f) {
                if (_bss_10.items[i] < sp80.items[i]) {
                    if (_data_B8[i][0] != -1) {
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, _data_B8[i][0], MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                } else {
                    if (_data_B8[i][1] != -1) {
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, _data_B8[i][1], MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                }
                _bss_10.items[i] = sp80.items[i];
            }
        }
        if (sp80.items[i] != _bss_38.items[i]) {
            _bss_38.items[i] = sp80.items[i];
            if (_bss_60.items[i] <= 60.0f) {
                _bss_60.items[i] = 90.0f - gUpdateRateF;
            }
        }
        if (_bss_60.items[i] < -30.0f) {
            _bss_60.items[i] = -30.0f;
        }
    }
}

// offset: 0x5BBC | func: 39
void dll_1_func_5BBC(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    f32 var_fv1;
    u32 pad;
    u8 var_s0;
    u8 sp7E;
    s8 sp7C;
    s8 sp7D;
    s32 viSize;
    Gfx* sp74;
    Object* sp70 = get_player();
    u8 var_v0;
    char sp68[4] = "   ";

    sp74 = *gdl;
    viSize = vi_get_current_size();
    gDPSetScissor(sp74++, G_SC_NON_INTERLACE, 0, 0, (GET_VIDEO_WIDTH(viSize) & 0xFFFF) - 1, 239);
    if ((_bss_60.unk0 >= 0.0f) || (_bss_60.unk1C >= 0.0f) || (_bss_60.unk14 >= 0.0f)) {
        var_fv1 = 255.0f;
    } else {
        var_fv1 = 0.0f;
    }
    if (_bss_0 < var_fv1) {
        _bss_0 += 8.5f * gUpdateRateF;
        if (_bss_0 > 255.0f) {
            _bss_0 = 255.0f;
        }
    } else if (var_fv1 < _bss_0) {
        _bss_0 -= 8.5f * gUpdateRateF;
        if (_bss_0 < 0.0f) {
            _bss_0 = 0.0f;
        }
    }
    sp7E = _bss_0;
    if (sp7E) {
        for (var_s0 = 0; var_s0 < (_bss_10.unk1C >> 2); var_s0++) {
            s32 pad;
            if (var_s0 >= 0xD) {
                sp7C = (var_s0 * 0xA) - 0x82;
                sp7D = 0x14;
            } else if (var_s0 >= 7) {
                sp7C = (var_s0 * 0xA) - 0x41;
                sp7D = 0xA;
            } else {
                sp7C = var_s0 * 0xA;
                sp7D = 0;
            }
            viSize = _bss_10.unk0 >> 2;
            if (var_s0 < viSize) {
                var_v0 = 0x11;
            } else if (viSize < var_s0) {
                var_v0 = 8;
            } else {
                var_v0 = ((_bss_10.unk0 & 3) + 8);
            }
            func_80037F9C(&sp74, _bss_6B8[var_v0], sp7C + 0x3C, sp7D + 0x14, 0xFF, 0xFF, 0xFF, sp7E);
        }
    }
    if ((_bss_60.unk8 >= 0.0f) || (_bss_60.unk14 >= 0.0f) || (((DLL_210_Player*)sp70->dll)->vtbl->func50(sp70) != -1)) {
        var_fv1 = 255.0f;
    } else {
        var_fv1 = 0.0f;
    }
    if (_bss_8 < var_fv1) {
        _bss_8 += 8.5f * gUpdateRateF;
        if (_bss_8 > 255.0f) {
            _bss_8 = 255.0f;
        }
    } else if (var_fv1 < _bss_8) {
        _bss_8 -= 8.5f * gUpdateRateF;
        if (_bss_8 < 0.0f) {
            _bss_8 = 0.0f;
        }
    }
    sp7E = _bss_8;
    if (sp7E) {
        for (var_s0 = 0; var_s0 < (_bss_10.unk20 / 25); var_s0++) {
            if (var_s0 < (_bss_10.unk8 / 25)) {
                viSize = 0x42;
            } else if ((_bss_10.unk8 / 25) < var_s0) {
                viSize = 0;
            } else {
                viSize = ((_bss_10.unk8 % 25) * 2) + 0xD;
            }
            func_800390A4(&sp74, _bss_6B8[36], 23.0f, ((var_s0 * 0xC) + 0x3C), (f32) viSize, 14.0f, 0, 0, 1.0f, 1.0f, sp7E | ~0xFF, 0x4002);
            func_800390A4(&sp74, _bss_6B8[35], (f32) (viSize + 0x17), ((var_s0 * 0xC) + 0x3C), (f32) (0x42 - viSize), 14.0f, viSize << 5, 0, 1.0f, 1.0f, sp7E | ~0xFF, 0x4002);
        }
    }
    sp7E = (u8)_bss_0 < (u8)_bss_8 ?  (u8)_bss_8 : (u8)_bss_0;
    if (sp7E) {
        if (sp70->id == 0x1F) {
            sp7C = 0;
            sp7D = 0;
            var_v0 = 0x35;
        } else {
            sp7C = 2;
            sp7D = -1;
            var_v0 = 0x28;
        }
        _data_68 = tex_load_deferred(_data_9D8[var_v0]);
        func_8003825C(&sp74, _data_68, sp7C + 0x14, sp7D + 0xA, 0, 0, sp7E, 0);
        tex_free(_data_68);
    }
    if ((_bss_60.unkC >= 0.0f) || (_bss_60.unk14 >= 0.0f)) {
        var_fv1 = 255.0f;
    } else {
        var_fv1 = 0.0f;
    }
    if (_bss_4 < var_fv1) {
        _bss_4 += 8.5f * gUpdateRateF;
        if (_bss_4 > 255.0f) {
            _bss_4 = 255.0f;
        }
    } else if (var_fv1 < _bss_4) {
        _bss_4 -= 8.5f * gUpdateRateF;
        if (_bss_4 < 0.0f) {
            _bss_4 = 0.0f;
        }
    }
    sp7E = _bss_4;
    if (sp7E && (main_get_bits(BIT_UI_Scarab_Counter_Enabled) != 0)) {
        _bss_89 = 0;
        if (sp7E == 0xFF) {
            // @fake assignment of var_s0
            if ((var_s0 = _bss_8B) != 0) {
                _bss_89 = 0xB - _bss_8B;
                _bss_8B--;
                if (!_bss_8B) {
                    _bss_8A = 0x50;
                }
            } else {
                if (_bss_8A != 0) {
                    _bss_8A--;
                    if (_bss_8A < 6) {
                        _bss_89 = 3 - (_bss_8A >> 1);
                    }
                } else {
                    _bss_8A = rand_next(0x14, 0xFF);
                }
            }
        }
        func_800390A4(&sp74, _bss_6B8[_bss_89+18], 252.0f, 198.0f, 16.0f, 16.0f, 0, 0, 1.0f, 1.0f, sp7E | ~0xFF, 0x4002);
        sprintf(sp68, "%d", (int)_bss_10.unkC);
        font_window_set_coords(3, 0, 0, 0x140, 0xF0);
        font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
        font_window_set_bg_colour(3, 0, 0, 0, 0);
        font_window_flush_strings(3);
        font_window_set_text_colour(3, 0xFF, 0xFF, 0xFF, 0xFF, sp7E);
        font_window_add_string_xy(3, 0x10E, 0xCA, sp68, 1, ALIGN_TOP_LEFT);
        font_window_set_text_colour(3, 0x14, 0x14, 0x14, 0xFF, 0xFF);
        font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
        font_window_draw(&sp74, mtxs, vtxs, 3);
    }
    *gdl = sp74;
}

// offset: 0x6984 | func: 40 | export: 16
void dll_1_func_6984(s32 arg0) {
    if (arg0 == -1) {
        _bss_C54 = 0;
        _bss_C48 = 0;
        return;
    }
    _bss_C54 = arg0;
    _bss_C48 = 1;
}

// offset: 0x69CC | func: 41
static void dll_1_func_69CC(CmdmenuItemUnkBSS* arg0) {
    arg0->unk4 = -1;
    arg0->unkC = 0;
    arg0->unk0 = 0;
    arg0->unk8 = 0.0f;
}

// offset: 0x69F8 | func: 42 | export: 17
void dll_1_func_69F8(s16 arg0, s32 arg1, s32 arg2) {
    InventoryItem *var_v0;
    UIUnknownCharacterStruct *var_s2;

    var_s2 = &_data_8F0;
    STUBBED_PRINTF("qInfoShow\n");
    _bss_C88.unk0 = NULL;
    while (var_s2->characterItems != NULL) {
        var_v0 = var_s2->characterItems;
        while (var_v0->flagObtained != -1) {
            if (arg0 == var_v0->flagObtained) {
                _bss_C88.unk0 = tex_load_deferred(var_v0->textureID);
                break;
            }
            var_v0++;
        }
        var_s2++;
    }
    if (_bss_C88.unk0 != NULL) {
        _bss_C88.unk4 = arg1;
        _bss_C88.unkC = arg2;
        _bss_C88.unk8 = 0.0f;
    }
}

// offset: 0x6B00 | func: 43 | export: 18
void dll_1_func_6B00(s16 textureID, s32 arg1, s32 arg2) {   
    STUBBED_PRINTF("qInfoShowTex\n");
    if ((_bss_C88.unk0 = tex_load_deferred(textureID))){
        _bss_C88.unk4 = arg1;
        _bss_C88.unkC = arg2;
        _bss_C88.unk8 = 0.0f;
    }
}

// offset: 0x6B74 | func: 44
void dll_1_func_6B74(Gfx** gdl, CmdmenuItemUnkBSS* arg1) {
    if (arg1->unk4 >= 0) {
        arg1->unk4 -= gUpdateRate;
        if (arg1->unk4 < 0) {
            tex_free(arg1->unk0);
            arg1->unk0 = NULL;
            return;
        }
        if ((f32) arg1->unk4 < 30.0f) {
            arg1->unk8 = ((f32) arg1->unk4 * 255.0f) / 30.0f;
        } else {
            if (arg1->unk8 != 255.0f) {
                arg1->unk8 += (8.5f * (f32) gUpdateRate);
                if (arg1->unk8 > 255.0f) {
                    arg1->unk8 = 255.0f;
                }
            }
        }
        func_80037F9C(gdl, _bss_6B8[52], 
            0x16, 
            0xB0, 
            0xFF, 
            0xFF, 
            0xFF, 
            (u8) arg1->unk8);
        bzero(_bss_C60, sizeof(Func_80037F9C_Struct));
        _bss_C60->unk0 = arg1->unk0;
        _bss_C60[1].unk0 = NULL;
        func_80037F9C(gdl, _bss_C60, 
            0x24, 
            0xAF, 
            0xFF, 
            0xFF, 
            0xFF, 
            (u8) arg1->unk8);
        func_80037F9C(gdl, _bss_6B8[29], 
            0x14, 
            0xAF, 
            0xFF, 
            0xFF, 
            0xFF, 
            (u8) arg1->unk8);
        func_80037F9C(gdl, _bss_6B8[30], 
            0x44, 
            0xAF, 
            0xFF, 
            0xFF, 
            0xFF, 
            (u8) arg1->unk8);
        if (arg1->unkC >= 2) {
            _bss_C60->unk0 = *_bss_6B0;
            _bss_C60->unk4 = (arg1->unkC - 2) << 8;
            func_80037F9C(gdl, _bss_C60, 
                0x34, 
                0xBF, 
                0xFF, 
                0xFF, 
                0xFF, 
                (u8) arg1->unk8);
        }
    }
}

// offset: 0x70A0 | func: 45 | export: 11
void dll_1_func_70A0(u8 arg0) {
    _data_88 = arg0;
}

// offset: 0x70C8 | func: 46 | export: 20
void dll_1_func_70C8(s32 minEnergy, s32 maxEnergy, s32 arg2, s32 arg3, s32 arg4) {
    EnergyBar *enbar;
    Func_80037F9C_Struct *temp;

    /* default.dol
    if (_bss_90 != NULL) {
        STUBBED_PRINTF("WARNING: ENERGY BAR ALREADY CREATED and need killed \n");
    }
    */

    enbar = mmAlloc(sizeof(EnergyBar), ALLOC_TAG_GAME_COL, ALLOC_NAME("cmdmenu : Bar"));
    bzero(enbar, sizeof(EnergyBar));
    enbar->unk0 = minEnergy;
    enbar->unk4 = maxEnergy;
    temp = &enbar->Fullbartex[0];
    temp->unk0 = tex_load_deferred(arg2);
    temp->unk4 = 0;
    temp->unk8 = 0;
    temp->unkA = 0;
    enbar->Fullbartex[1].unk0 = 0;
    temp = &enbar->Emptybartex[0];
    temp->unk0 = tex_load_deferred(arg3);
    temp->unk4 = 0;
    temp->unk8 = 0;
    temp->unkA = 0;
    enbar->Emptybartex[1].unk0 = 0;
    enbar->unk8 = enbar->Fullbartex[0].unk0->width;
    enbar->unkC = enbar->Fullbartex[0].unk0->height;
    if (arg4 == 0) {
        arg4 = 1;
    }
    enbar->unk48 = arg4;
    _bss_90 = enbar;
    STUBBED_PRINTF(" Width %i Length %i ", enbar->unk8, enbar->unkC);
    enbar->unk14 = 0;
    enbar->unk8 = enbar->unk8 * enbar->unk48;
    enbar->unk10 = (f32) enbar->unk8;
}

// offset: 0x7208 | func: 47 | export: 21
void dll_1_func_7208(s32 arg0) {
    u32 temp_ft1;
    f32 f0;

    /* default.dol
    if (_bss_90 == NULL) {
        STUBBED_PRINTF(" WARNING: Energy bar not created ");
    }
    */

    f0 = ((f32)(arg0 - _bss_90->unk0)) / ((f32)(_bss_90->unk4 - _bss_90->unk0));
    temp_ft1 = (s32) (_bss_90->unk8 * f0);
    _bss_90->unk10 = temp_ft1;
}

// offset: 0x7298 | func: 48
void dll_1_func_7298(Gfx** gdl) {
    EnergyBar* temp_v0;

    temp_v0 = _bss_90;
    if (temp_v0 == NULL) {
        return;
    }
    if (temp_v0->unk4C_0) {
        if (gUpdateRate >= temp_v0->unk14) {
            temp_v0->unk4C_0 = 0;
            dll_1_func_7550();
            return;
        } else {
            temp_v0->unk14 -= gUpdateRate;
        }
    } else {
        if (temp_v0->unk14 < (0xFF - (gUpdateRate * 2))) {
            temp_v0->unk14 += (gUpdateRate * 2);
        } else if (temp_v0->unk10 == 0.0f) {
            temp_v0->unk14 = 0;
        } else {
            temp_v0->unk14 = -1;
        }
    }
    
    func_800390A4(gdl, temp_v0->Fullbartex, 
        (f32) (0xA0 - (temp_v0->unk8 >> 1)), 
        (f32) (0xE6 - temp_v0->unkC), 
        temp_v0->unk10, 
        (f32) temp_v0->unkC, 
        0, 
        0,
        1.0f, 
        1.0f, 
        temp_v0->unk14 | ~0xFF, 
        0x4002);
    func_800390A4(gdl, temp_v0->Emptybartex, 
        (f32) (0xA0 - (temp_v0->unk8 >> 1)) + temp_v0->unk10, 
        (f32) (0xE6 - temp_v0->unkC), 
        temp_v0->unk8 - temp_v0->unk10, 
        (f32) temp_v0->unkC, 
        (s32) (temp_v0->unk10 * 32.0f), 
        0, 
        1.0f, 
        1.0f, 
        temp_v0->unk14 | ~0xFF, 
        0x4002);
}

// offset: 0x7550 | func: 49 | export: 22
/* Related to the fuel gauge at CloudRunner Fortress' Racetrack */
void dll_1_func_7550(void) {
    EnergyBar* enbar;

    /* default.dol
    if (_bss_90 == NULL) {
        STUBBED_PRINTF(" WARNING : cmdmenu Energy bar alreadby freed \n");
    }
    */

    STUBBED_PRINTF(" Killing Bar ");
    enbar = _bss_90;
    enbar->unk14 = 0;
    tex_free(enbar->Fullbartex[0].unk0);
    tex_free(enbar->Emptybartex[0].unk0);
    mmFree(_bss_90);
    _bss_90 = NULL;
}

// offset: 0x75CC | func: 50 | export: 23
void dll_1_func_75CC(void) {
    if (_bss_90 != NULL) {
        _bss_90->unk4C_0 = 1;
    }
    /* default.dol
    else {
        STUBBED_PRINTF(" WARNING : cmdmenu Energy bar alreadby freed \n");
    }
    */
}
