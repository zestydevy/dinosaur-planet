#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/engine/1_cmdmenu.h"
#include "dll.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/common/sidekick.h"
#include "game/gamebits.h"
#include "game/gametexts.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/textable.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/rcp.h"
#include "sys/vi.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/fonts.h"
#include "sys/rcp.h"
#include "sys/joypad.h"
#include "macros.h"
#include "gbi_extra.h"
#include "prevent_bss_reordering2.h"

// official filename: 9slcommandmenu.c (default.dol)

#define MAX_LOADED_ITEMS 64
#define MAX_OPACITY 0xFF
#define MAX_OPACITY_F 255.0f

#define NO_GAMETEXT -1
#define NO_TEXTURE -1
#define NO_PAGE -1

/* UI COORD MACROS */

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

//Change these to move the UI elements in the top-left of screen (character icon, health, magic)
#define UI_TOP_LEFT_X 0
#define UI_TOP_LEFT_Y 0

//Change these to move the UI elements in the top-right of screen (C-buttons, inventory scroll)
#define UI_TOP_RIGHT_X (SCREEN_WIDTH)
#define UI_TOP_RIGHT_Y (0)

//Change these to move the UI elements in the bottom-left of screen (item info pop-up, minimap)
#define UI_BOTTOM_LEFT_X (0)
#define UI_BOTTOM_LEFT_Y (SCREEN_HEIGHT)

//Change these to move the UI elements in the bottom-right of screen (Scarab counter, active Spell/Command)
#define UI_BOTTOM_RIGHT_X (SCREEN_WIDTH)
#define UI_BOTTOM_RIGHT_Y (SCREEN_HEIGHT)

//UI TOP-LEFT
#define POS_HEALTH_X 60
#define POS_HEALTH_Y 20

#define APPLES_ROW_1 7
#define APPLES_ROW_2 6
#define APPLES_ROW_2_IDX (APPLES_ROW_1)
#define APPLES_ROW_3_IDX (APPLES_ROW_1 + APPLES_ROW_2)
#define APPLES_SPACING_X 10
#define APPLES_SPACING_Y 10

//UI TOP-RIGHT
#define POS_PAGE_ICON_X 261
#define POS_PAGE_ICON_Y 10

#define POS_ITEM_HL_X1 266
#define POS_ITEM_HL_Y1 82
#define POS_ITEM_HL_X2 282
#define POS_ITEM_HL_Y2 102

//UI BOTTOM-LEFT

//UI BOTTOM-RIGHT
#define POS_SCARABS_X (UI_BOTTOM_RIGHT_X - 68)
#define POS_SCARABS_Y (UI_BOTTOM_RIGHT_Y - 42)
#define POS_SCARABS_NUMBER_X (POS_SCARABS_X + 18)
#define POS_SCARABS_NUMBER_Y (POS_SCARABS_Y + 4)


typedef struct {
/*0*/ Texture* texture;
/*4*/ s32 timer;
/*8*/ f32 opacity;
/*C*/ s16 count;
} CmdmenuInfoShowData;

typedef struct {
/*0*/ InventoryItem* items;
/*4*/ s16 unk4;     // the loaded item index of this page's selected item (at runtime)
/*8*/ u32 mesgID;   // objmesg ID sent to player
/*C*/ s32 btnMask;  // mask of joypad buttons that can be used to move to this item
} CmdmenuPage;

typedef union { 
    struct {
    /*0 00*/ s32 playerHealth;
    /*1 04*/ s32 sidekickBlueFood;
    /*2 08*/ s32 playerMagic;
    /*3 0C*/ s32 playerScarabCount;
    /*4 10*/ s32 sidekickRedFood;
    /*5 14*/ s32 unk14;
    /*6 18*/ s32 unk18;
    /*7 1C*/ s32 playerHealthMax;
    /*8 20*/ s32 playerMagicMax;
    /*9 24*/ s32 sidekickMaxFood;
    };
    s32 items[10];
} CmdmenuPlayerSidekickData;

typedef struct {
/*00*/ s32 min;
/*04*/ s32 max;
/*08*/ u32 width;
/*0C*/ u32 height;
/*10*/ f32 filledWidth;
/*14*/ u8 alpha;
/*18*/ TextureTile fullbarTex[2]; // official name: Fullbartex (default.dol)
/*30*/ TextureTile emptybarTex[2]; // official name: Emptybartex (default.dol)
/*48*/ s32 scale;
/*4C_0*/ u32 fadeout : 1;
} EnergyBar;

typedef union {
    struct {
    /*0 00*/ f32 playerHealth;
    /*1 04*/ f32 sidekickBlueFood;
    /*2 08*/ f32 playerMagic;
    /*3 0C*/ f32 playerScarabCount;
    /*4 10*/ f32 sidekickRedFood;
    /*5 14*/ f32 unk14;
    /*6 18*/ f32 unk18;
    /*7 1C*/ f32 playerHealthMax;
    /*8 20*/ f32 playerMagicMax;
    /*9 24*/ f32 sidekickMaxFood;
    };
    f32 items[10];
} CmdmenuPlayerSidekickDataChangeTimers;

enum CmdMenuTextures {
    CMDMENU_TEX_00_Scroll_BG = 0,
    CMDMENU_TEX_01_Scroll_Bottom = 1,
    CMDMENU_TEX_02_Scroll_Top = 2,
    CMDMENU_TEX_03_InfoScroll_Roll_End = 3,
    CMDMENU_TEX_04_InfoScroll_Roll = 4,
    CMDMENU_TEX_05_InfoScroll_Side = 5,
    CMDMENU_TEX_06_InfoScroll_BG = 6,
    CMDMENU_TEX_07_InfoScroll_SelfShadow = 7,
    CMDMENU_TEX_08_Apple_0_Pct = 8,
    CMDMENU_TEX_09_Apple_25_Pct = 9,
    CMDMENU_TEX_10_Apple_50_Pct = 10,
    CMDMENU_TEX_11_Apple_75_Pct = 11,
    CMDMENU_TEX_12_Mushroom_Blue_Full = 12,
    CMDMENU_TEX_13_Grub_Blue_Full = 13,
    CMDMENU_TEX_14 = 14,
    CMDMENU_TEX_15 = 15,
    CMDMENU_TEX_16_Grub_Blue_Half = 16,
    CMDMENU_TEX_17_Apple_100_Pct = 17,
    CMDMENU_TEX_18_Scarab = 18,
    CMDMENU_TEX_19_Scarab_Flutter_Frame1 = 19,
    CMDMENU_TEX_20_Scarab_Flutter_Frame2 = 20,
    CMDMENU_TEX_21_Scarab_Flutter_Frame3 = 21,
    CMDMENU_TEX_22_Scarab_Spin_Frame1 = 22,
    CMDMENU_TEX_23_Scarab_Spin_Frame2 = 23,
    CMDMENU_TEX_24_Scarab_Spin_Frame3 = 24,
    CMDMENU_TEX_25_Scarab_Spin_Frame4 = 25,
    CMDMENU_TEX_26_Scarab_Spin_Frame5 = 26,
    CMDMENU_TEX_27_Scarab_Spin_Frame6 = 27,
    CMDMENU_TEX_28_Scarab_Spin_Frame7 = 28,
    CMDMENU_TEX_29_Page_Torn_Left = 29,
    CMDMENU_TEX_30_Page_Torn_Right = 30,
    CMDMENU_TEX_31_Highlight_Corner_Top_Left = 31,
    CMDMENU_TEX_32_Highlight_Corner_Top_Right = 32,
    CMDMENU_TEX_33_Highlight_Corner_Bottom_Left = 33,
    CMDMENU_TEX_34_Highlight_Corner_Bottom_Right = 34,
    CMDMENU_TEX_35_MagicBar_Empty = 35,
    CMDMENU_TEX_36_MagicBar_Full = 36,
    CMDMENU_TEX_37_C_Down = 37,
    CMDMENU_TEX_38_LeftDownButtons_With_Kyte = 38,
    CMDMENU_TEX_39_C_Left = 39,
    CMDMENU_TEX_40_Sabre = 40,
    CMDMENU_TEX_41_C_Right = 41,
    CMDMENU_TEX_42_Tricky = 42,
    CMDMENU_TEX_43_LeftDownButtons_With_Tricky = 43,
    CMDMENU_TEX_44_Mushroom_Empty = 44,
    CMDMENU_TEX_45_Mushroom_Red_Full = 45,
    CMDMENU_TEX_46_Mushroom_Red_Half = 46,
    CMDMENU_TEX_47_RightButton_With_Bag = 47,
    CMDMENU_TEX_48_LeftDownButtons_NoSidekick = 48,
    CMDMENU_TEX_49_MagicBook = 49,
    CMDMENU_TEX_50_Bag = 50,
    CMDMENU_TEX_51_Mushroom_Blue_Half = 51,
    CMDMENU_TEX_52_Page_Torn_Shadow = 52,
    CMDMENU_TEX_53_Krystal = 53,
    CMDMENU_TEX_54_Kyte = 54,
    CMDMENU_TEX_55_Grub_Empty = 55,
    CMDMENU_TEX_56_Grub_Red_Full = 56,
    CMDMENU_TEX_57_Grub_Red_Half = 57
};

/*0x0*/ static s8 dInventoryShow = FALSE;
/*0x4*/ static s8 sInventoryScrollOffset = 0;   //Y offset for the scroll's inner strip, used to smoothly animate moving between inventory items
/*0x8*/ static s8 dInventoryMoveSpeed = 1;      //Scroll speed when moving between inventory items (how many times to shift gUpdateRate)
/*0xC*/ static s16 dInventoryUnrollMax = 72;    //The height of the inventory scroll when fully open
/*0x10*/ static s16 dOpacityInventoryScroll = 0;
/*0x14*/ static s16 _data_14 = 0;
/*0x18*/ static s16 _data_18 = -1;
/*0x1C*/ static s8 _data_1C = 0;
/*0x20*/ static s16 _data_20 = 60;
/*0x24*/ static s16 _data_24 = 50;
/*0x28*/ static s16 _data_28 = 20;
/*0x2C*/ static s16 _data_2C = 160;
/*0x30*/ static s16 dOpacityInfoScroll = 0;

/** 
  * Lines of text for the info scroll 
  */
/*0x34*/ static char* dTextStrings[] = {
    NULL, NULL, NULL, NULL
};

/** 
  * StringID for the info scroll's gametext
  *
  * (First 255 lines are from `gametext_3`, higher IDs use `gametext_568`) 
  */
/*0x44*/ static s16 dGametextStringID = NO_GAMETEXT;

/*0x48*/ static s8 _data_48 = 0;
/*0x4C*/ static s16 _data_4C = 120;
/*0x50*/ static s16 _data_50 = 80;
/*0x54*/ static s16 _data_54 = 20;
/*0x58*/ static s16 _data_58 = 160;
/*0x5C*/ static s16 _data_5C = 0;
/*0x60*/ static s16 _data_60 = 0;
/*0x64*/ static s16 _data_64 = -1;
/*0x68*/ static Texture* dInventoryPageIcon = NULL; //Icon in the top-right corner of screen: Bag/SpellBook/Kyte/Tricky
/*0x6C*/ static u32 _data_6C[] = {
    0x3f800000, 0xff000000
};
/*0x74*/ static s8 sJoyButtonMask = 0;
/*0x78*/ static s16 _data_78 = 0;
/*0x7C*/ static u8 _data_7C = 0;
/*0x80*/ static u8 sForceStatsDisplay = FALSE;
/*0x84*/ static s16 _data_84 = 0;
/*0x88*/ static s8 _data_88 = 1;
/*0x8C*/ static s16 dSpellGamebits[] = {
    BIT_Spell_Projectile, 
    BIT_Spell_Illusion, 
    BIT_Spell_Forcefield, 
    BIT_Spell_Portal, 
    BIT_Spell_Ice_Blast, 
    BIT_Spell_Mind_Read, 
    BIT_Spell_Grenade
};
/*0x9C*/ static s16 dSpellTextableIDs[] = {
    TEXTABLE_578_CMDMENU_ProjectileSpell_NoBG, 
    TEXTABLE_57A_CMDMENU_IllusionSpell_NoBG, 
    TEXTABLE_57D_CMDMENU_ShieldSpell_NoBG, 
    TEXTABLE_57C_CMDMENU_PortalSpell_NoBG, 
    TEXTABLE_57B_CMDMENU_IceSpell_NoBG, 
    TEXTABLE_57E_CMDMENU_UnkSpell_NoBG, 
    TEXTABLE_579_CMDMENU_GrenadeSpell_NoBG
};
/*0xAC*/ static s16 dCommandTextableIDs[] = {
    TEXTABLE_57F_CMDMENU_FindCommand_NoBG, 
    TEXTABLE_57F_CMDMENU_FindCommand_NoBG, 
    TEXTABLE_581_CMDMENU_DistractCommand_NoBG, 
    TEXTABLE_580_CMDMENU_GuardCommand_NoBG, 
    TEXTABLE_582_CMDMENU_FlameCommand_NoBG, 
    TEXTABLE_583_CMDMENU_TrickyBall_NoBG
};
/*0xB8*/ static s16 _data_B8[10][2] = {
    {NO_SOUND, NO_SOUND}, 
    {NO_SOUND, NO_SOUND}, 
    {NO_SOUND, NO_SOUND}, 
    {NO_SOUND, NO_SOUND}, 
    {NO_SOUND, NO_SOUND}, 
    {NO_SOUND, NO_SOUND}, 
    {NO_SOUND, NO_SOUND}, 
    {NO_SOUND, NO_SOUND}, 
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
/*0x118*/ static s8 _data_118 = 0;
/*0x11C*/ static s8 _data_11C = 0;
/*0x120*/ static s8 _data_120 = 0;
/*0x124*/ static s8 _data_124 = 0;

#define NONE 0xFFFF
#define EXIT -1
#define END {NONE, NONE, NONE, NONE, 0x0000, 0x00, 0x00}

/* Krystal items
 * 0x128*/ static InventoryItem dPage0ItemsKrystal[] = {
    {BIT_Krystal_Warp_Crystal,   BIT_Krystal_Used_Warp_Crystal,    TEXTABLE_245, NONE,  0x010, EXIT,                                CMDMENU_SOUND_ITEM}, //Warp Crystal
    {BIT_CRF_Prison_Key_1,       NONE,                             TEXTABLE_175, NONE,  0x007, EXIT,                                CMDMENU_SOUND_ITEM}, //Prison Key (CloudRunner Fortress)
    {BIT_CRF_Power_Room_Key,     NONE,                             TEXTABLE_176, NONE,  0x008, EXIT,                                CMDMENU_SOUND_ITEM}, //Power Room Key (CloudRunner Fortress)
    {BIT_CRF_Red_Power_Crystal,  NONE,                             TEXTABLE_180, NONE,  0x009, EXIT,                                CMDMENU_SOUND_ITEM}, //Red Power Crystal (CloudRunner Fortress)
    {BIT_CRF_Green_Power_Crystal,NONE,                             TEXTABLE_181, NONE,  0x009, EXIT,                                CMDMENU_SOUND_ITEM}, //Green Power Crystal (CloudRunner Fortress)
    {BIT_CRF_Blue_Power_Crystal, NONE,                             TEXTABLE_182, NONE,  0x009, EXIT,                                CMDMENU_SOUND_ITEM}, //Blue Power Crystal (CloudRunner Fortress)
    {BIT_IM_Snowbike_Key,        NONE,                             TEXTABLE_175, NONE,  0x011, EXIT,                                CMDMENU_SOUND_ITEM}, //Snowbike Key (Removed)
    {BIT_DIM_Mine_Key,           BIT_DIM_Used_Mine_Key,            TEXTABLE_175, NONE,  0x013, EXIT,                                CMDMENU_SOUND_ITEM}, //Mine Key (unused?)
    {BIT_DIM_Alpine_Roots,       NONE,                             TEXTABLE_1A3, NONE,  0x00b, EXIT,                                CMDMENU_SOUND_ITEM}, //Alpine Root (unused?)
    {BIT_DIM_Gear_1,             BIT_DIM_Used_Gear_1,              TEXTABLE_479, NONE,  0x00b, EXIT,                                CMDMENU_SOUND_ITEM}, //DIM Gear 1
    {BIT_DIM_Gear_2,            BIT_DIM_Used_Gear_2,              TEXTABLE_479,  NONE,  0x00b, EXIT,                                CMDMENU_SOUND_ITEM}, //DIM Gear 2
    {BIT_DIM_Gear_3,            BIT_DIM_Used_Gear_3,              TEXTABLE_479,  NONE,  0x00b, EXIT,                                CMDMENU_SOUND_ITEM}, //DIM Gear 3
    {BIT_DIM_Gear_4,            BIT_DIM_Used_Gear_4,              TEXTABLE_479,  NONE,  0x00b, EXIT,                                CMDMENU_SOUND_ITEM}, //DIM Gear 4
    {BIT_Krystal_Foodbag_S,     BIT_Krystal_Foodbag_M,            TEXTABLE_27A,  NONE,  0x016, CMDMENU_PAGE_2_Food_Actions_Krystal, CMDMENU_SOUND_PAGE}, //Small Food Bag
    {BIT_Krystal_Foodbag_M,     BIT_Krystal_Foodbag_L,            TEXTABLE_244,  NONE,  0x017, CMDMENU_PAGE_2_Food_Actions_Krystal, CMDMENU_SOUND_PAGE}, //Medium Food Bag
    {BIT_Krystal_Foodbag_L,     NONE,                             TEXTABLE_2BF,  NONE,  0x018, CMDMENU_PAGE_2_Food_Actions_Krystal, CMDMENU_SOUND_PAGE}, //Large Food Bag
    {BIT_Krystal_Dino_Bag_S,    BIT_Krystal_Dino_Bag_M,           TEXTABLE_471,  NONE,  0x02f, CMDMENU_PAGE_9_Food_Actions_Kyte,    CMDMENU_SOUND_PAGE}, //Small Grub Bag
    {BIT_Krystal_Dino_Bag_M,    BIT_Krystal_Dino_Bag_L,           TEXTABLE_560,  NONE,  0x030, CMDMENU_PAGE_9_Food_Actions_Kyte,    CMDMENU_SOUND_PAGE}, //Medium Grub Bag
    {BIT_Krystal_Dino_Bag_L,    NONE,                             TEXTABLE_560,  NONE,  0x031, CMDMENU_PAGE_9_Food_Actions_Kyte,    CMDMENU_SOUND_PAGE}, //Large Grub Bag
    {BIT_Gold_Nugget_GP,        BIT_CC_Bribed_GuardClaw,          TEXTABLE_257,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}, //Gold Nugget #1 (Near Golden Plains)
    {BIT_Gold_Nugget_LFV,       BIT_CC_Rescued_Kyte,              TEXTABLE_257,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}, //Gold Nugget #2 (Near LightFoot Village)
    {BIT_CC_Fire_Crystal,       NONE,                             TEXTABLE_4F7,  NONE,  0x102, EXIT,                                CMDMENU_SOUND_ITEM}, //Fire Crystal
    {BIT_CC_Cell_Door_Key,      BIT_CC_Rescued_Kyte,              TEXTABLE_175,  NONE,  0x01e, EXIT,                                CMDMENU_SOUND_ITEM}, //Cell Door Key (Cape Claw?)
    {BIT_SpellStone_CRF,        NONE,                             TEXTABLE_562,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}, //SpellStone - CloudRunner Fortress (Inactive)
    {BIT_SpellStone_BWC,        NONE,                             TEXTABLE_562,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}, //SpellStone - BlackWater Canyon (Inactive)
    {BIT_SpellStone_KP,         NONE,                             TEXTABLE_562,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}, //SpellStone - Krazoa Palace (Inactive)
    {BIT_Krazoa_Translator,     NONE,                             TEXTABLE_496,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}, //Krazoa Translator
    {BIT_Horn_of_Truth,         NONE,                             TEXTABLE_258,  NONE,  0x01d, EXIT,                                CMDMENU_SOUND_ITEM}, //Horn of Truth
    {BIT_CRF_Treasure_Chest_Key,NONE,                             TEXTABLE_568,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}, //Treasure Chest Key (CloudRunner Fortress)
    {BIT_CloudRunner_Grubs,     NONE,                             TEXTABLE_1A5,  NONE,  0x02a, EXIT,                                CMDMENU_SOUND_ITEM}, //Blue Mushroom (CloudRunner Grubs?)
    {BIT_Krystal_Fireflies,     NONE,                             TEXTABLE_46D,  NONE,  0x03d, EXIT,                                CMDMENU_SOUND_ITEM}, //Firefly Lantern
    {BIT_CRF_Prison_Key_2,      NONE,                             TEXTABLE_175,  NONE,  0x007, EXIT,                                CMDMENU_SOUND_ITEM}, //Prison Key (CloudRunner Fortress)
    {BIT_CC_Krazoa_Tablets,     NONE,                             TEXTABLE_496,  NONE,  0x05b, EXIT,                                CMDMENU_SOUND_ITEM}, //Krazoa Tablets
    {BIT_Inventory_MoonSeeds,   NONE,                             TEXTABLE_40D,  NONE,  0x05e, EXIT,                                CMDMENU_SOUND_ITEM}, //MoonSeeds
    {BIT_CC_Engineers_Key,      NONE,                             TEXTABLE_175,  NONE,  0x064, EXIT,                                CMDMENU_SOUND_ITEM}, //Construction Engineer's Key (CloudRunner Fortress)
    {BIT_Gold_Nugget_CC,        BIT_CC_Bribed_GuardClaw,          TEXTABLE_257,  NONE,  0x103, EXIT,                                CMDMENU_SOUND_ITEM}, //Gold Nugget #3 (Cape Claw)
    END
};

/* Sabre items
 * 0x2E4*/ static InventoryItem dPage1ItemsSabre[] = {
    {BIT_SP_Krazoa_Translator,       NONE,                         TEXTABLE_175, NONE,  0x0a,  EXIT,                                CMDMENU_SOUND_ITEM}, //Gate Key (Northern Wastes) (Unused?)
    {BIT_SW_Alpine_Roots,            NONE,                         TEXTABLE_1A3, NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}, //Alpine Root (Geyser area)
    {BIT_Inventory_Blue_Mushrooms,   NONE,                         TEXTABLE_1A5, NONE,  0x0c,  EXIT,                                CMDMENU_SOUND_ITEM}, //Blue Mushroom
    {BIT_Inventory_Purple_Mushrooms, NONE,                         TEXTABLE_498, NONE,  0x57,  EXIT,                                CMDMENU_SOUND_ITEM}, //Purple Mushroom
    {BIT_Inventory_White_Mushrooms,  NONE,                         TEXTABLE_497, NONE,  0x56,  EXIT,                                CMDMENU_SOUND_ITEM}, //White Mushroom
    {BIT_DIM_Mine_Key,               BIT_DIM_Used_Mine_Key,        TEXTABLE_175, NONE,  0x13,  EXIT,                                CMDMENU_SOUND_ITEM}, //Mine Key
    {BIT_DIM_Alpine_Roots,           NONE,                         TEXTABLE_1A3, NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}, //Alpine Root (DIM)
    {BIT_DIM_Gear_1,                 BIT_DIM_Used_Gear_1,          TEXTABLE_479, NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}, //DIM Gear 1 
    {BIT_DIM_Gear_2,                 BIT_DIM_Used_Gear_2,          TEXTABLE_479, NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}, //DIM Gear 2
    {BIT_DIM_Gear_3,                 BIT_DIM_Used_Gear_3,          TEXTABLE_479, NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}, //DIM Gear 3
    {BIT_DIM_Gear_4,                BIT_DIM_Used_Gear_4,          TEXTABLE_479,  NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}, //DIM Gear 4
    {BIT_Belina_Te_Cell_Key,        BIT_Used_Belina_Te_Cell_Key,  TEXTABLE_175,  NONE,  0x19,  EXIT,                                CMDMENU_SOUND_ITEM}, //Belina Te Cell Key (DarkIce Mines)
    {BIT_Tricky_Cell_Key,           BIT_Used_Tricky_Cell_Key,     TEXTABLE_175,  NONE,  0x19,  EXIT,                                CMDMENU_SOUND_ITEM}, //Tricky Cell Key (DarkIce Mines)
    {BIT_Sabre_Warp_Crystal,        BIT_Sabre_Used_Warp_Crystal,  TEXTABLE_245,  NONE,  0x10,  EXIT,                                CMDMENU_SOUND_ITEM}, //Warp Crystal
    {BIT_DIM_Door_Key_1,            BIT_DIM_Used_Door_Key_1,      TEXTABLE_175,  NONE,  0x19,  EXIT,                                CMDMENU_SOUND_ITEM}, //Door Key 1 (DarkIce Mines)
    {BIT_DIM_Door_Key_2,            BIT_DIM_Used_Door_Key_2,      TEXTABLE_175,  NONE,  0x19,  EXIT,                                CMDMENU_SOUND_ITEM}, //Door Key 2 (DarkIce Mines)
    {BIT_Sabre_Foodbag_S,           BIT_Sabre_Foodbag_M,          TEXTABLE_27A,  NONE,  0x16,  CMDMENU_PAGE_3_Food_Actions_Sabre,   CMDMENU_SOUND_PAGE}, //Small Food Bag
    {BIT_Sabre_Foodbag_M,           BIT_Sabre_Foodbag_L,          TEXTABLE_244,  NONE,  0x17,  CMDMENU_PAGE_3_Food_Actions_Sabre,   CMDMENU_SOUND_PAGE}, //Medium Food Bag
    {BIT_Sabre_Foodbag_L,           NONE,                         TEXTABLE_2BF,  NONE,  0x18,  CMDMENU_PAGE_3_Food_Actions_Sabre,   CMDMENU_SOUND_PAGE}, //Large Food Bag
    {BIT_Sabre_Dino_Bag_S,          BIT_Sabre_Dino_Bag_M,         TEXTABLE_471,  NONE,  0x32,  CMDMENU_PAGE_10_Food_Actions_Tricky, CMDMENU_SOUND_PAGE}, //Small Dinosaur Food Bag
    {BIT_Sabre_Dino_Bag_M,          BIT_Sabre_Dino_Bag_L,         TEXTABLE_560,  NONE,  0x33,  CMDMENU_PAGE_10_Food_Actions_Tricky, CMDMENU_SOUND_PAGE}, //Medium Dinosaur Food Bag
    {BIT_Sabre_Dino_Bag_L,          NONE,                         TEXTABLE_560,  NONE,  0x34,  CMDMENU_PAGE_10_Food_Actions_Tricky, CMDMENU_SOUND_PAGE}, //Large Dinosaur Food Bag
    {BIT_Inventory_Corrupt_Item,    NONE,                         TEXTABLE_175,  NONE,  0x1d,  EXIT,                                CMDMENU_SOUND_ITEM}, //Corrupted? A key with "ForceField Spell (Cape Claw)" as its description
    {BIT_Horn_of_Truth,             NONE,                         TEXTABLE_258,  NONE,  0x1d,  EXIT,                                CMDMENU_SOUND_ITEM}, //Horn of Truth
    {BIT_Sabre_Fireflies,           NONE,                         TEXTABLE_46D,  NONE,  0x3d,  EXIT,                                CMDMENU_SOUND_ITEM}, //Firefly Lantern
    {BIT_WC_Silver_Tooth,           BIT_WC_Used_Silver_Tooth,     TEXTABLE_52B,  NONE,  0x00,  EXIT,                                CMDMENU_SOUND_ITEM}, //Silver Tooth (Walled City)
    {BIT_WC_Gold_Tooth,             BIT_WC_Used_Gold_Tooth,       TEXTABLE_52A,  NONE,  0x00,  EXIT,                                CMDMENU_SOUND_ITEM}, //Gold Tooth (Walled City)
    {BIT_WC_Sun_Stone,              BIT_WC_Used_Sun_Stone,        TEXTABLE_3A8,  NONE,  0x1b,  EXIT,                                CMDMENU_SOUND_ITEM}, //Sun Stone (Walled City)
    {BIT_WC_Moon_Stone,             BIT_WC_Used_Moon_Stone,       TEXTABLE_3D6,  NONE,  0x1c,  EXIT,                                CMDMENU_SOUND_ITEM}, //Moon Stone (Walled City)
    {BIT_Inventory_MoonSeeds,       NONE,                         TEXTABLE_40D,  NONE,  0x62,  EXIT,                                CMDMENU_SOUND_ITEM}, //MoonSeeds
    {BIT_SpellStone_DIM,            BIT_SpellStone_DIM_Activated, TEXTABLE_562,  NONE,  0x60,  EXIT,                                CMDMENU_SOUND_ITEM}, //SpellStone - DarkIce Mines (Inactive)
    {BIT_SpellStone_DIM_Activated,  NONE,                         TEXTABLE_563,  NONE,  0x61,  EXIT,                                CMDMENU_SOUND_ITEM}, //SpellStone - DarkIce Mines (Activated) 
    {BIT_SpellStone_WC,             NONE,                         TEXTABLE_180,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}, //Spellstone - Walled City (Inactive) (Incorrect icon: Red Power Crystal, CRF)
    {BIT_SpellStone_DR,             NONE,                         TEXTABLE_180,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}, //Spellstone - Dragon Rock (Inactive) (Incorrect icon: Red Power Crystal, CRF)
    {BIT_DB_PointBack_Egg,          NONE,                         TEXTABLE_55E,  NONE,  0x5e,  EXIT,                                CMDMENU_SOUND_ITEM}, //PointBack Egg (Diamond Bay)
    {BIT_DB_Bay_Diamond,            NONE,                         TEXTABLE_55F,  NONE,  0x5f,  EXIT,                                CMDMENU_SOUND_ITEM}, //Bay Diamond
    END
};

/* Food Bag actions (Krystal)
 * 0x4A0*/ static InventoryItem dPage2FoodActionsKrystal[] = {
    {BIT_Foodbag_Eat,               NONE, TEXTABLE_2C7, NONE, 0x1f, CMDMENU_PAGE_4_Food_Krystal,            CMDMENU_SOUND_PAGE}, //Eat food in foodbag
    {BIT_Foodbag_Place,             NONE, TEXTABLE_2C8, NONE, 0x20, CMDMENU_PAGE_4_Food_Krystal,            CMDMENU_SOUND_PAGE}, //Place food down from foodbag
    {BIT_Foodbag_Give,              NONE, TEXTABLE_2EF, NONE, 0x21, CMDMENU_PAGE_4_Food_Krystal,            CMDMENU_SOUND_PAGE}, //Give food from foodbag
    {BIT_Foodbag_Setting_Eat_First, NONE, TEXTABLE_265, NONE, 0x39, CMDMENU_PAGE_2_Food_Actions_Krystal,    CMDMENU_SOUND_PAGE}, //Eat later (NOTE: option to switch shown when "Eat First" enabled)
    {BIT_Foodbag_Setting_Eat_Later, NONE, TEXTABLE_529, NONE, 0x3a, CMDMENU_PAGE_2_Food_Actions_Krystal,    CMDMENU_SOUND_PAGE}, //Eat first (NOTE: option to switch shown when "Eat Later" enabled)
    END
};

/* Food Bag actions (Sabre)
 * 0x4E8*/ static InventoryItem dPage3FoodActionsSabre[] = {
    {BIT_Foodbag_Eat,               NONE, TEXTABLE_2C7, NONE, 0x1f, CMDMENU_PAGE_5_Food_Sabre,              CMDMENU_SOUND_PAGE}, //Eat food in foodbag
    {BIT_Foodbag_Place,             NONE, TEXTABLE_2C8, NONE, 0x20, CMDMENU_PAGE_5_Food_Sabre,              CMDMENU_SOUND_PAGE}, //Place food down from foodbag
    {BIT_Foodbag_Give,              NONE, TEXTABLE_2EF, NONE, 0x21, CMDMENU_PAGE_5_Food_Sabre,              CMDMENU_SOUND_PAGE}, //Give food from foodbag
    {BIT_Foodbag_Setting_Eat_First, NONE, TEXTABLE_265, NONE, 0x39, CMDMENU_PAGE_3_Food_Actions_Sabre,      CMDMENU_SOUND_PAGE}, //Eat later (NOTE: option to switch shown when "Eat First" enabled)
    {BIT_Foodbag_Setting_Eat_Later, NONE, TEXTABLE_529, NONE, 0x3a, CMDMENU_PAGE_3_Food_Actions_Sabre,      CMDMENU_SOUND_PAGE}, //Eat first (NOTE: option to switch shown when "Eat Later" enabled)
    END
};

/* Food Bag items (Krystal)
 * 0x530*/ static InventoryItem dPage4FoodItemsKrystal[] = {
    {BIT_Green_Apple_Count,  NONE,                 TEXTABLE_26E,  NONE, 0x22, EXIT,                                 CMDMENU_SOUND_ITEM}, //Green apple
    {BIT_Red_Apple_Count,    NONE,                 TEXTABLE_26F,  NONE, 0x23, EXIT,                                 CMDMENU_SOUND_ITEM}, //Red apple
    {BIT_Brown_Apple_Count,  NONE,                 TEXTABLE_270,  NONE, 0x24, EXIT,                                 CMDMENU_SOUND_ITEM}, //Brown Apple
    {BIT_Fish_Count,         NONE,                 TEXTABLE_272,  NONE, 0x25, EXIT,                                 CMDMENU_SOUND_ITEM}, //Fish
    {BIT_Smoked_Fish_Count,  NONE,                 TEXTABLE_2C6,  NONE, 0x26, EXIT,                                 CMDMENU_SOUND_ITEM}, //Smoked Fish
    {BIT_Dino_Egg_Count,     NONE,                 TEXTABLE_2C4,  NONE, 0x27, EXIT,                                 CMDMENU_SOUND_ITEM}, //Dino Egg
    {BIT_Moldy_Meat_Count,   NONE,                 TEXTABLE_2C5,  NONE, 0x28, EXIT,                                 CMDMENU_SOUND_ITEM}, //Moldy Meat
    {BIT_Green_Bean_Count,   NONE,                 TEXTABLE_270,  NONE, 0x35, EXIT,                                 CMDMENU_SOUND_ITEM}, //Green Bean
    {BIT_Red_Bean_Count,     NONE,                 TEXTABLE_270,  NONE, 0x36, EXIT,                                 CMDMENU_SOUND_ITEM}, //Red Bean
    {BIT_Brown_Bean_Count,   NONE,                 TEXTABLE_270,  NONE, 0x37, EXIT,                                 CMDMENU_SOUND_ITEM}, //Brown Bean
    {BIT_Blue_Bean_Count,   NONE,                  TEXTABLE_270,  NONE, 0x38, EXIT,                                 CMDMENU_SOUND_ITEM}, //Blue Bean
    {BIT_Krystal_Foodbag_S, BIT_Krystal_Foodbag_M, TEXTABLE_27A,  NONE, 0x16, CMDMENU_PAGE_2_Food_Actions_Krystal,  CMDMENU_SOUND_PAGE}, //Small Food Bag
    {BIT_Krystal_Foodbag_M, BIT_Krystal_Foodbag_L, TEXTABLE_244,  NONE, 0x17, CMDMENU_PAGE_2_Food_Actions_Krystal,  CMDMENU_SOUND_PAGE}, //Medium Food Bag
    {BIT_Krystal_Foodbag_L, NONE,                  TEXTABLE_2BF,  NONE, 0x18, CMDMENU_PAGE_2_Food_Actions_Krystal,  CMDMENU_SOUND_PAGE}, //Large Food Bag
    END
};

/* Food Bag items (Sabre)
 * 0x5E4*/ static InventoryItem dPage5FoodItemsSabre[] = {
    {BIT_Green_Apple_Count,  NONE,                 TEXTABLE_26E,  NONE, 0x22, EXIT,                                 CMDMENU_SOUND_ITEM}, //Green apple
    {BIT_Red_Apple_Count,    NONE,                 TEXTABLE_26F,  NONE, 0x23, EXIT,                                 CMDMENU_SOUND_ITEM}, //Red apple
    {BIT_Brown_Apple_Count,  NONE,                 TEXTABLE_270,  NONE, 0x24, EXIT,                                 CMDMENU_SOUND_ITEM}, //Brown Apple
    {BIT_Fish_Count,         NONE,                 TEXTABLE_272,  NONE, 0x25, EXIT,                                 CMDMENU_SOUND_ITEM}, //Fish
    {BIT_Smoked_Fish_Count,  NONE,                 TEXTABLE_2C6,  NONE, 0x26, EXIT,                                 CMDMENU_SOUND_ITEM}, //Smoked Fish
    {BIT_Dino_Egg_Count,     NONE,                 TEXTABLE_2C4,  NONE, 0x27, EXIT,                                 CMDMENU_SOUND_ITEM}, //Dino Egg
    {BIT_Moldy_Meat_Count,   NONE,                 TEXTABLE_2C5,  NONE, 0x28, EXIT,                                 CMDMENU_SOUND_ITEM}, //Moldy Meat
    {BIT_Green_Bean_Count,   NONE,                 TEXTABLE_270,  NONE, 0x35, EXIT,                                 CMDMENU_SOUND_ITEM}, //Green Bean 
    {BIT_Red_Bean_Count,     NONE,                 TEXTABLE_270,  NONE, 0x36, EXIT,                                 CMDMENU_SOUND_ITEM}, //Red Bean
    {BIT_Brown_Bean_Count,   NONE,                 TEXTABLE_270,  NONE, 0x37, EXIT,                                 CMDMENU_SOUND_ITEM}, //Brown Bean
    {BIT_Blue_Bean_Count,   NONE,                  TEXTABLE_270,  NONE, 0x38, EXIT,                                 CMDMENU_SOUND_ITEM}, //Blue Bean
    {BIT_Sabre_Foodbag_S,   BIT_Sabre_Foodbag_M,   TEXTABLE_27A,  NONE, 0x16, CMDMENU_PAGE_3_Food_Actions_Sabre,    CMDMENU_SOUND_PAGE}, //Small Food Bag
    {BIT_Sabre_Foodbag_M,   BIT_Sabre_Foodbag_L,   TEXTABLE_244,  NONE, 0x17, CMDMENU_PAGE_3_Food_Actions_Sabre,    CMDMENU_SOUND_PAGE}, //Medium Food Bag
    {BIT_Sabre_Foodbag_L,   NONE,                  TEXTABLE_2BF,  NONE, 0x18, CMDMENU_PAGE_3_Food_Actions_Sabre,    CMDMENU_SOUND_PAGE}, //Large Food Bag
    END
};

/* Magic Spells
 * 0x698*/ static InventoryItem dPage6MagicSpells[] = {
    {BIT_Spell_Projectile, NONE,    TEXTABLE_183, NONE, 0x0d, EXIT, CMDMENU_SOUND_NONE}, //Projectile Spell
    {BIT_Spell_Ice_Blast,  NONE,    TEXTABLE_468, NONE, 0x3c, EXIT, CMDMENU_SOUND_NONE}, //Ice Blast Spell
    {BIT_Spell_Grenade,    NONE,    TEXTABLE_4F8, NONE, 0x0f, EXIT, CMDMENU_SOUND_NONE}, //Grenade Spell (Incorrectly labelled as Randorn)
    {BIT_Spell_Illusion,   NONE,    TEXTABLE_177, NONE, 0x0e, EXIT, CMDMENU_SOUND_NONE}, //Illusion Spell
    {BIT_Spell_Forcefield, NONE,    TEXTABLE_265, NONE, 0x14, EXIT, CMDMENU_SOUND_NONE}, //ForceField Spell
    {BIT_Spell_Glitched_1, NONE,    TEXTABLE_183, NONE, 0x0d, EXIT, CMDMENU_SOUND_NONE}, //Glitched Spell
    {BIT_Spell_Glitched_2, BIT_348, TEXTABLE_529, NONE, 0x0d, EXIT, CMDMENU_SOUND_NONE}, //Glitched Spell (Maybe "Quake", mentioned elsewhere?)
    {BIT_Spell_Portal,     NONE,    TEXTABLE_466, NONE, 0x3b, EXIT, CMDMENU_SOUND_NONE}, //Portal Spell
    {BIT_Spell_Mind_Read,  NONE,    TEXTABLE_470, NONE, 0x3e, EXIT, CMDMENU_SOUND_NONE}, //Mind Read
    END
};

//TODO: should sidekick pages use an InventoryItem struct variant with slightly different member names?
/* Sidekick Commands (Kyte)
 * 0x710*/ static InventoryItem dPage7CommandsKyte[] = {
    {Sidekick_COMMAND_01_Heel,      0, TEXTABLE_1CD, TEXTABLE_1CD, 0, 0, 0}, //Heel
    {Sidekick_COMMAND_20_Play,      5, TEXTABLE_1D1, TEXTABLE_1D1, 6, 0, 0}, //Play
    {Sidekick_COMMAND_02_Find,      1, TEXTABLE_1CE, TEXTABLE_1CE, 1, 0, 0}, //Find
    {Sidekick_COMMAND_10_Flame,     4, TEXTABLE_1CF, TEXTABLE_1CF, 2, 0, 0}, //Flame
    {Sidekick_COMMAND_04_Distract,  2, TEXTABLE_1D0, TEXTABLE_1D0, 3, 0, 0}, //Distract
    {Sidekick_COMMAND_08_Guard,     3, TEXTABLE_1D2, TEXTABLE_1D2, 4, 0, 0}, //Guard
    END
};

/* Sidekick Commands (Tricky)
 * 0x764*/ static InventoryItem sPage8CommandsTricky[] = {
    {Sidekick_COMMAND_01_Heel,      0, TEXTABLE_1CD, TEXTABLE_1CD, 0, 0, 0}, //Heel
    {Sidekick_COMMAND_20_Play,      5, TEXTABLE_1D1, TEXTABLE_1D1, 6, 0, 0}, //Play
    {Sidekick_COMMAND_02_Find,      1, TEXTABLE_1CE, TEXTABLE_1CE, 1, 0, 0}, //Find
    {Sidekick_COMMAND_10_Flame,     4, TEXTABLE_1CF, TEXTABLE_1CF, 2, 0, 0}, //Flame
    {Sidekick_COMMAND_04_Distract,  2, TEXTABLE_1D0, TEXTABLE_1D0, 3, 0, 0}, //Distract
    {Sidekick_COMMAND_08_Guard,     3, TEXTABLE_1D2, TEXTABLE_1D2, 4, 0, 0}, //Guard
    END
};

/* Dinosaur Food Bag Actions (Krystal)
 * 0x7B8*/ static InventoryItem dPage9FoodActionsKyte[] = {
    {BIT_Dino_Foodbag_Place, NONE, TEXTABLE_2C8, NONE, 0x20, CMDMENU_PAGE_11_Food_Kyte, CMDMENU_SOUND_PAGE}, //Place food down from foodbag
    {BIT_Dino_Foodbag_Give,  NONE, TEXTABLE_561, NONE, 0x21, CMDMENU_PAGE_11_Food_Kyte, CMDMENU_SOUND_PAGE}, //Give food from foodbag
    END
};

/* Dinosaur Food Bag Actions (Sabre)
 * 0x7DC*/ static InventoryItem dPage10FoodActionsTricky[] = {
    {BIT_Dino_Foodbag_Place, NONE, TEXTABLE_2C8, NONE, 0x20, CMDMENU_PAGE_12_Food_Tricky, CMDMENU_SOUND_PAGE}, //Place food down from foodbag
    {BIT_Dino_Foodbag_Give,  NONE, TEXTABLE_561, NONE, 0x21, CMDMENU_PAGE_12_Food_Tricky, CMDMENU_SOUND_PAGE}, //Give food from foodbag
    END
};

/* Dinosaur Food Bag Items (Kyte)
 * 0x800*/ static InventoryItem dPage11FoodItemsKyte[] = {
    {BIT_Krystal_Dino_Bag_S,      BIT_Krystal_Dino_Bag_M, TEXTABLE_27A, NONE, 0x2f, CMDMENU_PAGE_9_Food_Actions_Kyte,       CMDMENU_SOUND_PAGE}, //Small Mushroom Bag
    {BIT_Krystal_Dino_Bag_M,      BIT_Krystal_Dino_Bag_L, TEXTABLE_244, NONE, 0x30, CMDMENU_PAGE_9_Food_Actions_Kyte,       CMDMENU_SOUND_PAGE}, //Medium Mushroom Bag
    {BIT_Krystal_Dino_Bag_L,      NONE,                   TEXTABLE_2BF, NONE, 0x31, CMDMENU_PAGE_9_Food_Actions_Kyte,       CMDMENU_SOUND_PAGE}, //Large Mushroom Bag
    {BIT_Dino_Bag_Blue_Mushrooms, NONE,                   TEXTABLE_270, NONE, 0x0c, EXIT,                                   CMDMENU_SOUND_ITEM}, //Blue Mushroom
    {BIT_Dino_Bag_Red_Mushrooms,  NONE,                   TEXTABLE_270, NONE, 0x2d, EXIT,                                   CMDMENU_SOUND_ITEM}, //Red Mushroom
    {BIT_Dino_Bag_Old_Mushrooms,  NONE,                   TEXTABLE_270, NONE, 0x2e, EXIT,                                   CMDMENU_SOUND_ITEM}, //Old Mushroom
    {BIT_Dino_Bag_Blue_Grubs,     NONE,                   TEXTABLE_270, NONE, 0x2a, EXIT,                                   CMDMENU_SOUND_ITEM}, //Blue Grub
    {BIT_Dino_Bag_Red_Grubs,      NONE,                   TEXTABLE_270, NONE, 0x2b, EXIT,                                   CMDMENU_SOUND_ITEM}, //Red Grub
    {BIT_Dino_Bag_Old_Grubs,      NONE,                   TEXTABLE_270, NONE, 0x2c, EXIT,                                   CMDMENU_SOUND_ITEM}, //Old Grub
    END
};

/* Dinosaur Food Bag Items (Tricky)
 * 0x878*/ static InventoryItem dPage12FoodItemsTricky[] = {
    {BIT_Sabre_Dino_Bag_S,        BIT_Sabre_Dino_Bag_M,   TEXTABLE_27A, NONE, 0x32, CMDMENU_PAGE_10_Food_Actions_Tricky,    CMDMENU_SOUND_PAGE}, //Small Mushroom Bag
    {BIT_Sabre_Dino_Bag_M,        BIT_Sabre_Dino_Bag_L,   TEXTABLE_244, NONE, 0x33, CMDMENU_PAGE_10_Food_Actions_Tricky,    CMDMENU_SOUND_PAGE}, //Medium Mushroom Bag
    {BIT_Sabre_Dino_Bag_L,        NONE,                   TEXTABLE_2BF, NONE, 0x34, CMDMENU_PAGE_10_Food_Actions_Tricky,    CMDMENU_SOUND_PAGE}, //Large Mushroom Bag
    {BIT_Dino_Bag_Blue_Mushrooms, NONE,                   TEXTABLE_270, NONE, 0x0c, EXIT,                                   CMDMENU_SOUND_ITEM}, //Blue Mushroom
    {BIT_Dino_Bag_Red_Mushrooms,  NONE,                   TEXTABLE_270, NONE, 0x2d, EXIT,                                   CMDMENU_SOUND_ITEM}, //Red Mushroom
    {BIT_Dino_Bag_Old_Mushrooms,  NONE,                   TEXTABLE_270, NONE, 0x2e, EXIT,                                   CMDMENU_SOUND_ITEM}, //Old Mushroom
    {BIT_Dino_Bag_Blue_Grubs,     NONE,                   TEXTABLE_270, NONE, 0x2a, EXIT,                                   CMDMENU_SOUND_ITEM}, //Blue Grub
    {BIT_Dino_Bag_Red_Grubs,      NONE,                   TEXTABLE_270, NONE, 0x2b, EXIT,                                   CMDMENU_SOUND_ITEM}, //Red Grub
    {BIT_Dino_Bag_Old_Grubs,      NONE,                   TEXTABLE_270, NONE, 0x2c, EXIT,                                   CMDMENU_SOUND_ITEM}, //Old Grub
    END
};

/*0x8F0*/ static CmdmenuPage dCmdmenuPages[] = {
    /*0*/  { dPage0ItemsKrystal,         0, 0x80001, R_CBUTTONS },  // Krystal items
    /*1*/  { dPage1ItemsSabre,           0, 0x80001, R_CBUTTONS },  // Sabre items
    /*2*/  { dPage2FoodActionsKrystal,   0, 0x80001, R_CBUTTONS },  // Foodbag actions (Krystal)
    /*3*/  { dPage3FoodActionsSabre,     0, 0x80001, R_CBUTTONS },  // Foodbag actions (Sabre)
    /*4*/  { dPage4FoodItemsKrystal,     0, 0x80001, R_CBUTTONS },  // Foodbag items (Krystal)
    /*5*/  { dPage5FoodItemsSabre,       0, 0x80001, R_CBUTTONS },  // Foodbag items (Sabre)
    /*6*/  { dPage6MagicSpells,          0, 0x80002, L_CBUTTONS },  // Magic spells
    /*7*/  { dPage7CommandsKyte,         0, 0x80009, D_CBUTTONS },  // Sidekick commands (Kyte)
    /*8*/  { sPage8CommandsTricky,       0, 0x80009, D_CBUTTONS },  // Sidekick commands (Tricky)
    /*9*/  { dPage9FoodActionsKyte,      0, 0x80001, R_CBUTTONS },  // Dinosaur foodbag actions (Krystal)
    /*10*/ { dPage10FoodActionsTricky,  0, 0x80001, R_CBUTTONS },  // Dinosaur foodbag actions (Sabre)
    /*11*/ { dPage11FoodItemsKyte,      0, 0x80001, R_CBUTTONS },  // Dinosaur foodbag items (Krystal)
    /*12*/ { dPage12FoodItemsTricky,    0, 0x80001, R_CBUTTONS },  // Dinosaur foodbag items (Sabre)
    /*13*/ { NULL,                      0, 0,       0 }
};

/*0x9D0*/ static s8 dPageCategory = 0; //Category of cmdmenu page currently open (see `CmdMenuPageCategories`)
/*0x9D4*/ static s8 dNextPageCategory = 0; //Assigned when changing between menus in the inventory
/*0x9D8*/ static s16 dTextableIDs[] = {
    /*00*/ TEXTABLE_1BE_CMDMENU_Scroll_BG,
    /*01*/ TEXTABLE_1BF_CMDMENU_Scroll_Bottom,
    /*02*/ TEXTABLE_1C0_CMDMENU_Scroll_Top,
    /*03*/ TEXTABLE_1C2_CMDMENU_InfoScroll_Roll_End,
    /*04*/ TEXTABLE_1C3_CMDMENU_InfoScroll_Roll,
    /*05*/ TEXTABLE_1C4_CMDMENU_InfoScroll_Side,
    /*06*/ TEXTABLE_1C5_CMDMENU_InfoScroll_BG,
    /*07*/ TEXTABLE_1C6_CMDMENU_InfoScroll_SelfShadow,
    /*08*/ TEXTABLE_3E2_CMDMENU_Apple_0_Pct,
    /*09*/ TEXTABLE_3E3_CMDMENU_Apple_25_Pct,
    /*10*/ TEXTABLE_3E4_CMDMENU_Apple_50_Pct,
    /*11*/ TEXTABLE_3E5_CMDMENU_Apple_75_Pct,
    /*12*/ TEXTABLE_421_CMDMENU_Mushroom_Blue_Full,
    /*13*/ TEXTABLE_1C9_CMDMENU_Grub_Blue_Full,
    /*14*/ TEXTABLE_3EA,
    /*15*/ TEXTABLE_3EB,
    /*16*/ TEXTABLE_1CB_CMDMENU_Grub_Blue_Half,
    /*17*/ TEXTABLE_3E6_CMDMENU_Apple_100_Pct,
    /*18*/ TEXTABLE_3E7_CMDMENU_Scarab,
    /*19*/ TEXTABLE_3F5_CMDMENU_Scarab_Flutter_Frame1,
    /*20*/ TEXTABLE_3F6_CMDMENU_Scarab_Flutter_Frame2,
    /*21*/ TEXTABLE_3F7_CMDMENU_Scarab_Flutter_Frame3,
    /*22*/ TEXTABLE_3F8_CMDMENU_Scarab_Spin_Frame1,
    /*23*/ TEXTABLE_3F9_CMDMENU_Scarab_Spin_Frame2,
    /*24*/ TEXTABLE_3FA_CMDMENU_Scarab_Spin_Frame3,
    /*25*/ TEXTABLE_3FB_CMDMENU_Scarab_Spin_Frame4,
    /*26*/ TEXTABLE_3FC_CMDMENU_Scarab_Spin_Frame5,
    /*27*/ TEXTABLE_3FD_CMDMENU_Scarab_Spin_Frame6,
    /*28*/ TEXTABLE_3FE_CMDMENU_Scarab_Spin_Frame7,
    /*29*/ TEXTABLE_422_CMDMENU_Page_Torn_Left,
    /*30*/ TEXTABLE_423_CMDMENU_Page_Torn_Right,
    /*31*/ TEXTABLE_429_CMDMENU_Highlight_Corner_Top_Left,
    /*32*/ TEXTABLE_42A_CMDMENU_Highlight_Corner_Top_Right,
    /*33*/ TEXTABLE_42B_CMDMENU_Highlight_Corner_Bottom_Left,
    /*34*/ TEXTABLE_42C_CMDMENU_Highlight_Corner_Bottom_Right,
    /*35*/ TEXTABLE_424_CMDMENU_MagicBar_Empty,
    /*36*/ TEXTABLE_42D_CMDMENU_MagicBar_Full,
    /*37*/ TEXTABLE_431_CMDMENU_C_Down,
    /*38*/ TEXTABLE_1D4_CMDMENU_LeftDownButtons_With_Kyte,
    /*39*/ TEXTABLE_42F_CMDMENU_C_Left,
    /*40*/ TEXTABLE_432_CMDMENU_Sabre,
    /*41*/ TEXTABLE_430_CMDMENU_C_Right,
    /*42*/ TEXTABLE_433_CMDMENU_Tricky,
    /*43*/ TEXTABLE_42E_CMDMENU_LeftDownButtons_With_Tricky,
    /*44*/ TEXTABLE_435_CMDMENU_Mushroom_Empty,
    /*45*/ TEXTABLE_436_CMDMENU_Mushroom_Red_Full,
    /*46*/ TEXTABLE_438_CMDMENU_Mushroom_Red_Half,
    /*47*/ TEXTABLE_43A_CMDMENU_RightButton_With_Bag,
    /*48*/ TEXTABLE_43B_CMDMENU_LeftDownButtons_NoSidekick,
    /*49*/ TEXTABLE_43D_CMDMENU_MagicBook,
    /*50*/ TEXTABLE_43E_CMDMENU_Bag,
    /*51*/ TEXTABLE_434_CMDMENU_Mushroom_Blue_Half,
    /*52*/ TEXTABLE_437_CMDMENU_Page_Torn_Shadow,
    /*53*/ TEXTABLE_267_CMDMENU_Krystal,
    /*54*/ TEXTABLE_268_CMDMENU_Kyte,
    /*55*/ TEXTABLE_476_CMDMENU_Grub_Empty,
    /*56*/ TEXTABLE_477_CMDMENU_Grub_Red_Full,
    /*57*/ TEXTABLE_478_CMDMENU_Grub_Red_Half
};

/*0x0*/ static f32 sOpacityHealth;  //Opacity of player health UI
/*0x4*/ static f32 sOpacityScarabs; //Opacity of Scarab counter UI
/*0x8*/ static f32 sOpacityMagic;   //Opacity of player magic bar UI
/*0xC*/ static s32 _bss_C;
/*0x10*/ static CmdmenuPlayerSidekickData sStats;
/*0x38*/ static CmdmenuPlayerSidekickData sPrevStats;
/*0x60*/ static CmdmenuPlayerSidekickDataChangeTimers sStatsChangeTimers;
/*0x88*/ static u8 _bss_88;
/*0x89*/ static u8 sAnimFrameScarab;
/*0x8A*/ static u8 sAnimScarabFlutterTimer;
/*0x8B*/ static u8 _bss_8B;
/*0x8C*/ static f32 sOpacityR; //Opacity of icons on right side of screen (C-buttons, menu page image, etc.)
/*0x90*/ static EnergyBar* sEnergyBar; 
/*0x98*/ static Texture* sMenuItemTextures[MAX_LOADED_ITEMS]; //Inventory icon texture pointers for the current menu page's loaded items
/*0x198*/ static Texture* sMenuSidekickCommandTextures[MAX_LOADED_ITEMS]; //Sidekick command inventory icon textures for the current menu page's loaded items
/*0x298*/ static s16 sMenuItemTextureIDs[MAX_LOADED_ITEMS]; //TextableIDs for the current menu page's loaded items
/*0x318*/ static s32 sMenuItemGamebits[MAX_LOADED_ITEMS]; //GamebitIDs for the current menu page's loaded items
/*0x418*/ static s16 sMenuItemTextIDs[MAX_LOADED_ITEMS]; //Gametext lineIDs for the current menu page's loaded items
/*0x498*/ static s8 _bss_498[MAX_LOADED_ITEMS]; //array of unkAs (from InventoryItem)
/*0x4D8*/ static u8 _bss_4D8[MAX_LOADED_ITEMS]; //array of unkBs (from InventoryItem)
/*0x518*/ static u8 sMenuItemVisibilities[MAX_LOADED_ITEMS];
/*0x558*/ static u8 sMenuItemQuantities[MAX_LOADED_ITEMS];
/*0x598*/ static Texture* sActiveSpellIcon; //Icon in bottom-right of screen
/*0x59C*/ static Texture* sActiveSpellRing; //Icon in bottom-right of screen
/*0x5A0*/ static Texture* sAButtonAnimTex;
/*0x5A4*/ static s16 sPrevActiveSpellGamebit;
/*0x5A8*/ static Texture* sActiveSidekickCommandIcon;
/*0x5AC*/ static Texture* sActiveSidekickCommandRing;
/*0x5B0*/ static s16 sPrevSidekickCommandIndex;
/*0x5B4*/ static s32 _bss_5B4;
/*0x5B8*/ static s32 sCrosshairAnimRenderFlags;
/*0x5BC*/ static s32 _bss_5BC;
/*0x5C0*/ static s32 sCrosshairAnimProgress;
/*0x5C8*/ static Texture* sTextures[ARRAYCOUNT(dTextableIDs)];
/*0x6B0*/ static Texture* sInventoryStackNumbersTex;
/*0x6B8*/ static TextureTile sTextureTiles[ARRAYCOUNT(dTextableIDs)][2];
/*0xC28*/ static s16 sInventoryUnrollY; //How far the inventory scroll has opened (0 when fully closed)
/*0xC2A*/ static s16 _bss_C2A;
/*0xC2C*/ static s16 _bss_C2C;
/*0xC2E*/ static s16 _bss_C2E;
/*0xC30*/ static GameTextChunk* _bss_C30;
/*0xC34*/ static Texture* sCrosshairTex;
/*0xC38*/ static s16 sUsedItemGamebitID;
/*0xC3A*/ static s16 _bss_C3A;
/*0xC3C*/ static s8 sUsedItemSound; //Set to 0 when item selection successful (item given to character, etc.)
/*0xC3D*/ static s8 _bss_C3D;
/*0xC3E*/ static s8 sInventoryPageID;
/*0xC3F*/ static u8 _bss_C3F;
/*0xC40*/ static s16 usedLoadedIdx;
/*0xC44*/ static s32 _bss_C44;
/*0xC48*/ static s8 sShouldOverrideJoypadButtons;
/*0xC4C*/ static s32 _bss_C4C;
/*0xC50*/ static s32 sJoyPressedButtons; // joypad button bitfield
/*0xC54*/ static s32 sJoyPressedButtonsOverride; //controllerButtons
/*0xC58*/ static s32 sJoyHeldButtons; // joypad button bitfield
/*0xC60*/ static TextureTile _bss_C60[2];
/*0xC78*/ static s8 _bss_C78;
/*0xC7A*/ static s16 _bss_C7A;
/*0xC7C*/ static s16 _bss_C7C;
/*0xC7E*/ static s16 _bss_C7E;
/*0xC80*/ static s16 _bss_C80;
/*0xC88*/ static CmdmenuInfoShowData sInfoPopup; //item info pop-up that appears after collecting certain items (e.g. Kyte's grubs)

static void cmdmenu_func_1410(void);
static void cmdmenu_func_1614(Gfx** gdl, Mtx** mtxs, Vertex** vtxs);
static void cmdmenu_func_1FEC(void);
static void cmdmenu_func_27D8(Gfx** gdl, Mtx** mtxs, Vertex** vtxs);
static s32 cmdmenu_func_325C(InventoryItem* arg0, s8 arg1);
static s32 cmdmenu_page_count_shown_items(InventoryItem* arg0, s8 arg1);
static void cmdmenu_func_3880(InventoryItem* items, s32 loadedItemIndex, s32 itemIndex);
static void cmdmenu_func_38E4(Gfx** gdl, Texture* tex, s32 frame);
static s32 cmdmenu_is_inventory_open(void);
static s32 cmdmenu_func_3A4C(void);
static void cmdmenu_close_inventory(void);
static void cmdmenu_open_inventory(void);
static void cmdmenu_func_3AD0(void);
static void cmdmenu_func_3D28(Gfx** gdl, Mtx** mtxs, Vertex** vtxs);
static void cmdmenu_func_4630(Gfx **gdl);
static void cmdmenu_gfx_set_screen_scissor(Gfx** gfx);
static s32 cmdmenu_func_47E8(void);
static void cmdmenu_func_4830(void);
static void cmdmenu_func_484C(void);
static void cmdmenu_func_486C(void);
static void cmdmenu_print_info_scroll(Gfx** gdl, Mtx** mtxs, Vertex** vtxs);
static void cmdmenu_update_stats(void);
static void cmdmenu_draw_player_stats(Gfx** gdl, Mtx** mtxs, Vertex** vtxs);
static void cmdmenu_info_hide(CmdmenuInfoShowData* arg0);
static void cmdmenu_info_draw(Gfx** gdl, CmdmenuInfoShowData* arg1);
static void cmdmenu_draw_energy_bar(Gfx** gdl);
void cmdmenu_energy_bar_free(void);

// offset: 0x0 | ctor
void cmdmenu_ctor(void* dll) {
    u32 i;

    _bss_C78 = -1;
    _bss_C7A = -1;
    sUsedItemGamebitID = NO_GAMEBIT;
    sUsedItemSound = CMDMENU_SOUND_NONE;
    _bss_C3D = -1;
    sPrevActiveSpellGamebit = NO_GAMEBIT;
    sPrevSidekickCommandIndex = NO_SIDEKICK_COMMAND;

    //Load all the cmdmenu textures
    for (i = 0; i < ARRAYCOUNT(dTextableIDs); i++) {
        sTextures[i] = tex_load_deferred(dTextableIDs[i]);

        sTextureTiles[i][0].tex = tex_load_deferred(dTextableIDs[i]);
        sTextureTiles[i][0].animProgress = 0;
        sTextureTiles[i][0].x = 0;
        sTextureTiles[i][0].y = 0;
        sTextureTiles[i][1].tex = NULL;
    }

    sCrosshairTex = tex_load_deferred(TEXTABLE_500_SpellCrosshair);
    sCrosshairTex->animSpeed = 40;
    sCrosshairAnimRenderFlags = RENDER_TEXANIM_REVERSE;
    sCrosshairAnimProgress = 0;
    sInventoryStackNumbersTex = tex_load_deferred(TEXTABLE_3A7_InventoryStackNumbers);
    sAButtonAnimTex = tex_load_deferred(TEXTABLE_274_AButton_Anim);
    sAButtonAnimTex->animSpeed = 40;
    
    cmdmenu_info_hide(&sInfoPopup);
}

// offset: 0x180 | dtor
void cmdmenu_dtor(void* dll) {
    s32 i;

    for (i = 0; i < (s32)ARRAYCOUNT(sTextures); i++) {
        if (sTextures[i] != NULL) {
            tex_free(sTextures[i]);
        }
    }

    tex_free(sInventoryStackNumbersTex);
    tex_free(sCrosshairTex);

    if (sActiveSpellIcon != NULL) {
        tex_free(sActiveSpellIcon);
    }
    if (sActiveSpellRing != NULL) {
        tex_free(sActiveSpellRing);
    }
    if (sActiveSidekickCommandIcon != NULL) {
        tex_free(sActiveSidekickCommandIcon);
    }
    if (sActiveSidekickCommandRing != NULL) {
        tex_free(sActiveSidekickCommandRing);
    }
    if (sAButtonAnimTex != NULL) {
        tex_free(sAButtonAnimTex);
    }
}

// offset: 0x2B8 | func: 0 | export: 15
void cmdmenu_disable_buttons(u16 mask) {
    sJoyButtonMask = mask;
}

// offset: 0x2E0 | func: 1 | export: 19
void cmdmenu_toggle_forced_stats_display(u8 force) {
    sForceStatsDisplay = force;
}

// offset: 0x308 | func: 2 | export: 0
s32 cmdmenu_func_308(void) {
    cmdmenu_func_1FEC();
    cmdmenu_func_1410();
    return 0;
}

// offset: 0x35C | func: 3 | export: 1
void cmdmenu_func_35C(void) {
    Object* player;
    Object* sidekick;
    s16 _pad;
    s8 sp31;
    s16 newStringID;
    s8 var_a0;
    s8 newPageIndex;

    player = get_player();
    sidekick = get_sidekick();

    if (player != NULL) {
        sJoyPressedButtons = joy_get_pressed(0);
        sJoyHeldButtons = joy_get_buttons(0);

        if (player->unkB0 & 0x1000) {
            joy_set_button_mask(0, L_CBUTTONS | R_CBUTTONS | D_CBUTTONS);
            sJoyPressedButtons &= ~(R_TRIG | L_CBUTTONS | R_CBUTTONS | D_CBUTTONS);
            sJoyHeldButtons &= ~(R_TRIG | L_CBUTTONS | R_CBUTTONS | D_CBUTTONS);
        } else if (sJoyButtonMask != 0) {
            joy_set_button_mask(0, sJoyButtonMask);
            sJoyPressedButtons &= ~sJoyButtonMask;
            sJoyHeldButtons &= ~sJoyButtonMask;
        }

        if (sShouldOverrideJoypadButtons) {
            sJoyPressedButtons = sJoyPressedButtonsOverride;
        } else {
            sJoyPressedButtons |= sJoyPressedButtonsOverride;
            if ((player->unkB0 & 0x1000) || (sJoyButtonMask != 0)) {
                sJoyPressedButtons |= B_BUTTON;
            }
        }

        //Using C buttons (left/down/right) to open or change inventory pages
        if ((sJoyPressedButtons & D_CBUTTONS) && (sidekick != NULL) && (dPageCategory != CMDMENU_CATEGORY_2_Sidekick)) {
            newPageIndex = sidekick->id == OBJ_Kyte ? CMDMENU_PAGE_8_Sidekick_Tricky : CMDMENU_PAGE_7_Sidekick_Kyte;
            if (cmdmenu_page_count_shown_items(dCmdmenuPages[newPageIndex].items, 1)) {
                joy_set_button_mask(0, D_CBUTTONS);
                dNextPageCategory = CMDMENU_CATEGORY_2_Sidekick;
                sInventoryPageID = newPageIndex;
            }
        } else if ((sJoyPressedButtons & R_CBUTTONS) && (dPageCategory != CMDMENU_CATEGORY_3_Items) && (dPageCategory != CMDMENU_CATEGORY_6_Food)) {
            newPageIndex = player->id == OBJ_Krystal ? CMDMENU_PAGE_0_Items_Krystal : CMDMENU_PAGE_1_Items_Sabre;
            if (cmdmenu_page_count_shown_items(dCmdmenuPages[newPageIndex].items, 0)) {
                joy_set_button_mask(0, R_CBUTTONS);
                dNextPageCategory = CMDMENU_CATEGORY_3_Items;
                sInventoryPageID = newPageIndex;
            }
        } else if ((sJoyPressedButtons & L_CBUTTONS) && (dPageCategory != CMDMENU_CATEGORY_4_Spells)) {
            if (cmdmenu_page_count_shown_items(dCmdmenuPages[CMDMENU_PAGE_6_Spells].items, 0)) {
                joy_set_button_mask(0, L_CBUTTONS);
                dNextPageCategory = CMDMENU_CATEGORY_4_Spells;
                sInventoryPageID = CMDMENU_PAGE_6_Spells;
            }
        } else if (_bss_C3D != -1) {
            _bss_C3A = sUsedItemGamebitID;
            sInventoryPageID = _bss_C3D;
            if ((dPageCategory == CMDMENU_CATEGORY_2_Sidekick) || (dPageCategory == CMDMENU_CATEGORY_5)) {
                dNextPageCategory = 7;
            } else if ((dPageCategory == CMDMENU_CATEGORY_3_Items) || (dPageCategory == CMDMENU_CATEGORY_6_Food)) {
                dNextPageCategory = 6;
            } else if ((dPageCategory == CMDMENU_CATEGORY_4_Spells) || (dPageCategory == CMDMENU_CATEGORY_7)) {
                dNextPageCategory = 7;
            }
        }

        if (gDLL_2_Camera->vtbl->get_dll_ID() == DLL_ID_CAMSHIPBATTLE2) {
            cmdmenu_close_inventory();
        } else if (dNextPageCategory != 0) {
            if (cmdmenu_func_3A4C() != 0) {
                switch (dNextPageCategory) {
                case CMDMENU_CATEGORY_3_Items:
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_5EC_Cmdmenu_OpenBag, MAX_VOLUME, NULL, NULL, 0, NULL);
                    break;
                case CMDMENU_CATEGORY_2_Sidekick:
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_5F0_Cmdmenu_OpenSidekickMenu, MAX_VOLUME, NULL, NULL, 0, NULL);
                    break;
                case CMDMENU_CATEGORY_4_Spells:
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_5ED_Cmdmenu_OpenSpellBook, MAX_VOLUME, NULL, NULL, 0, NULL);
                    break;
                default:
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_28D_Cmdmenu_OpenBag_HighPitch, MAX_VOLUME, NULL, NULL, 0, NULL);
                    break;
                }
                cmdmenu_open_inventory();
                dCmdmenuPages[7].unk4 = 0;
                dCmdmenuPages[8].unk4 = 0;
                dPageCategory = dNextPageCategory;
                sJoyPressedButtons = 0;
                _data_78 = 0;
                dNextPageCategory = 0;
            } else {
                cmdmenu_close_inventory();
            }
        }

        cmdmenu_update_stats();
        cmdmenu_func_3AD0();
        cmdmenu_func_486C();

        _bss_C4C = _bss_C4C + 1;
        if (_bss_C4C >= 3) {
            _bss_C4C = 2;
        }

        if ((dInventoryShow != 0) || (dNextPageCategory != 0)) {
            newStringID = _data_18;
        } else {
            newStringID = gDLL_2_Camera->vtbl->get_target_gametextID();
        }

        _data_18 = -1;
        var_a0 = 0;
        if (_bss_C7C >= 0) {
            newStringID = _bss_C7C;
            _data_28 = _bss_C80;
            _data_2C = _bss_C7E;
            var_a0 = 1;
        } else {
            if (cmdmenu_func_47E8() != 0) {
                _data_2C = 160;
                _data_28 = 30;
            }
        }
        _bss_C7C = -1;

        if (((sJoyHeldButtons & R_TRIG) || (var_a0 != 0)) && (newStringID >= 0)) {
            if (newStringID != dGametextStringID) {
                dGametextStringID = newStringID;
                if (*dTextStrings != NULL) {
                    mmFree(*dTextStrings);
                    *dTextStrings = NULL;
                }
            }
            cmdmenu_func_484C();
        } else {
            cmdmenu_func_4830();
            if ((cmdmenu_func_47E8() != 0) && (*dTextStrings != NULL)) {
                mmFree(*dTextStrings);
                *dTextStrings = NULL;
                dGametextStringID = NO_GAMETEXT;
            }
        }

        joy_set_button_mask(0, L_CBUTTONS | R_CBUTTONS | D_CBUTTONS);
        sJoyButtonMask = 0;
    }
}

// offset: 0xBB8 | func: 4 | export: 2
void cmdmenu_print(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    Object* player;
    s32 viSize;
    s32 screenX;
    s32 screenY;

    player = get_player();
    if (player == NULL) {
        return;
    }

    //Draw Spell reticle when aiming
    if (((DLL_210_Player*)player->dll)->vtbl->func77(player, &screenX, &screenY)) {
        tex_animate(sCrosshairTex, &sCrosshairAnimRenderFlags, &sCrosshairAnimProgress);
        rcp_screen_full_write(
            gdl, 
            sCrosshairTex, 
            screenX - 16, 
            screenY - 16, 
            0, 
            sCrosshairAnimProgress >> 8, 
            150, 
            SCREEN_WRITE_TRANSLUCENT
        );
    }

    cmdmenu_draw_player_stats(gdl, mtxs, vtxs);
    viSize = vi_get_current_size();
    gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 
        0, 
        0, 
        GET_VIDEO_WIDTH(viSize), 
        GET_VIDEO_HEIGHT(viSize));
    cmdmenu_func_3D28(gdl, mtxs, vtxs);
    cmdmenu_print_info_scroll(gdl, mtxs, vtxs);
    cmdmenu_func_1614(gdl, mtxs, vtxs);
    cmdmenu_func_27D8(gdl, mtxs, vtxs);
    camera_apply_scissor(gdl);
}

// offset: 0xDC4 | func: 5 | export: 6
int cmdmenu_was_any_item_used(void) {
    if (sUsedItemGamebitID > NO_GAMEBIT) {
        return TRUE;
    }
    return FALSE;
}

// offset: 0xDF4 | func: 6 | export: 7
int cmdmenu_was_this_item_used(s32 itemGamebitID) {
    if (itemGamebitID == sUsedItemGamebitID) {
        sUsedItemSound = 0;
        return TRUE;
    }
    return FALSE;
}

// offset: 0xE2C | func: 7 | export: 8
s32 cmdmenu_func_E2C(s32* arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < arg1; i++) {
        if (sUsedItemGamebitID == arg0[i]) {
            sUsedItemSound = 0;
            return arg0[i];
        }
    }

    return -1;
}

// offset: 0xF24 | func: 8 | export: 9
s8 cmdmenu_get_page_category(void) {
    return dPageCategory;
}

// offset: 0xF40 | func: 9 | export: 10
s16 cmdmenu_func_F40(void) {
    return _bss_C3A;
}

// offset: 0xF5C | func: 10 | export: 5
/** 
  * Gets an array (`targetObjects`) of nearby Objects that can be targetted 
  * (i.e. have lock-on data and are fully opaque), sorted by their address.
  *
  * Returns the number of Objects in the array.
  */
s32 cmdmenu_get_target_objects(Object **targetObjects, s32 maxObjects, u8 lockFlag, s32 arg3, f32 range) {
    s32 _pad1[2];
    f32 dx;
    f32 dz;
    f32 dy;
    Camera* camera;
    f32 objX;
    f32 objY;
    f32 objZ;
    Object* obj;
    Object** objects;
    s32 count;
    s32 index;
    s32 _pad2[2];
    s32 isSorted;
    s32 i;
    s32 targetCount;
    s32 yaw;

    set_camera_selector(0);
    camera = get_main_camera();
    objects = get_world_objects(&index, &count);

    //Get the subset of Objects that can be targetted
    for (targetCount = 0, i = index; i < count; i++) {
        obj = objects[i];
        if ((obj->def->lockdata != NULL) && (obj->opacity == OBJECT_OPACITY_MAX) && !(obj->unkAF & ARROW_FLAG_8_No_Targetting) && 
                (obj->def->lockdata->flags & lockFlag) && (targetCount < maxObjects) && (arg3 & 1)) {
            get_object_child_position(obj, &objX, &objY, &objZ);
            dx = objX - camera->srt.transl.x;
            dy = objY - camera->srt.transl.y;
            dz = objZ - camera->srt.transl.z;
            if ((SQ(dx) + SQ(dy) + SQ(dz)) < SQ(range)) {
                yaw = camera->srt.yaw - (u16)(M_90_DEGREES - arctan2_f(dx, dz));
                CIRCLE_WRAP(yaw);
                if (yaw < -10000 && yaw > -22000) {
                    targetObjects[targetCount++] = obj;
                }
            }
        }
    }

    //Sort the targettable Objects by address
    if (targetCount > 0) {
        do {
            isSorted = TRUE;
            for (i = 0; i < (targetCount - 1); i++) {
                if ((s32)targetObjects[i] < (s32)targetObjects[i + 1]) {
                    obj = targetObjects[i];
                    targetObjects[i] = targetObjects[i + 1];
                    targetObjects[i + 1] = obj;
                    isSorted = FALSE;
                }
            }
        } while (isSorted == FALSE);
    }
    
    return targetCount;
}

// offset: 0x1290 | func: 11 | export: 3
void cmdmenu_func_1290(void) {
    s32 i;
    CmdmenuPage *temp;

    temp = dCmdmenuPages;

    for (i = 0; temp[i].items; i++) { temp[i].unk4 = 0; }
    sUsedItemGamebitID = NO_GAMEBIT;
    sUsedItemSound = 0;
    _bss_C3D = -1;
}

// offset: 0x12EC | func: 12 | export: 4
/** Update player stats */
void cmdmenu_func_12EC(void) {
    _bss_88 = 1;
}

// offset: 0x130C | func: 13 | export: 12
void cmdmenu_func_130C(u32 arg0, u32 arg1, u32 arg2) {
    _bss_C7C = arg0;
    _bss_C7E = arg1;
    _bss_C80 = arg2;
}

// offset: 0x1338 | func: 14 | export: 13
void cmdmenu_func_1338(s32 gametextID, s32 arg1, s32 arg2) {
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
void cmdmenu_func_13F4(void) {
    _data_48 = 0;
}

// offset: 0x1410 | func: 16
static void cmdmenu_func_1410(void) {
    if (_data_48 != 0) {
        _data_5C += gUpdateRate * 8;
        if (_data_5C > 160) {
            _data_5C = 160;
        }
        _data_60 += gUpdateRate * 8;
        if (_data_60 > 255) {
            _data_60 = 255;
        }
    } else {
        _data_60 -= gUpdateRate * 8;
        if (_data_60 < 0) {
            _data_60 = 0;
        }
        if (_data_60 < _data_5C) {
            _data_5C = _data_60;
        }
    }

    if (_data_5C == 0) {
        if (_bss_C30 != NULL) {
            mmFree(_bss_C30);
            _bss_C30 = NULL;
        }
    } else {
        tex_animate(sAButtonAnimTex, &_bss_5B4, &_bss_5BC);

        if (sJoyPressedButtons & A_BUTTON) {
            _bss_C2E += 3;
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);

            if (_bss_C2E < _bss_C30->count) {
                joy_set_button_mask(0, A_BUTTON);
                return;
            }

            _bss_C2E -= 3;
            _data_48 = 0;
        }
    }
}

// offset: 0x1614 | func: 17
static void cmdmenu_func_1614(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    s32 sp10C;
    s32 y;
    s32 sp104;
    s32 height;
    s32 tempY;
    s32 i;
    Gfx* dl;

    if (_data_5C == 0) {
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
    dl_set_prim_color(&dl, 255, 255, 255, (u8) _data_5C);
    sp10C = _data_58;
    sp10C <<= 2;
    sp104 = _data_4C << 2;
    sp104 -= (16 << 2);
    y = _data_54 << 2;
    height = _bss_C2C << 2;
    
    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_06_InfoScroll_BG], 0);
    gSPTextureRectangle(dl++, 
        sp10C - sp104, 
        y, 
        sp10C + sp104, 
        y + height, 
        G_TX_RENDERTILE, 
        qs105(0), qs105(0), 
        qs510(1), qs510(1));
    gDLBuilder->needsPipeSync = TRUE;
    
    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_05_InfoScroll_Side], 0);
    gSPTextureRectangle(dl++, 
        sp10C - sp104 - (16 << 2), 
        y, 
        sp10C - sp104, 
        y + height, 
        G_TX_RENDERTILE, 
        qs105(0), qs105(0), 
        qs510(1), qs510(1));
    gDLBuilder->needsPipeSync = TRUE;
    
    gSPTextureRectangle(dl++, 
        sp10C + sp104, 
        y, 
        sp10C + sp104 + (16 << 2), 
        y + height, 
        G_TX_RENDERTILE, 
        qs105(15), qs105(0), 
        qs510(-1), qs510(1));
    gDLBuilder->needsPipeSync = TRUE;

    sp104 += (16 << 2);
    
    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_07_InfoScroll_SelfShadow], 0);
    dl_set_prim_color(&dl, 255, 128, 128, 128);
    
    tempY = y;
    gSPTextureRectangle(dl++,
        sp10C - sp104, 
        tempY, 
        sp10C + sp104, 
        tempY + (8 << 2), 
        G_TX_RENDERTILE, 
        qs105(0), qs105(7), 
        qs510(1), qs510(-1));
    gDLBuilder->needsPipeSync = TRUE;
    
    tempY = (y + height) - (6 << 2);
    gSPTextureRectangle(dl++,
        sp10C - sp104,
        tempY, 
        sp10C + sp104, 
        tempY + (8 << 2), 
        G_TX_RENDERTILE, 
        qs105(0), qs105(0), 
        qs510(1), qs510(1));
    gDLBuilder->needsPipeSync = TRUE;
    
    dl_set_prim_color(&dl, 255, 255, 255, (u8) _data_5C);

    tempY = y - (11 << 2);
    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_04_InfoScroll_Roll], 0);
    gSPTextureRectangle(dl++,
        sp10C - sp104, 
        tempY, 
        sp10C + sp104, 
        tempY + (16 << 2), 
        G_TX_RENDERTILE, 
        qs105(0), qs105(15.9688), 
        qs510(1), qs510(-1));
    gDLBuilder->needsPipeSync = TRUE;
    
    tempY = (y + height) - (4 << 2);
    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_04_InfoScroll_Roll], 0);
    gSPTextureRectangle(dl++,
        sp10C - sp104, 
        tempY, 
        sp10C + sp104, 
        tempY + (16 << 2), 
        G_TX_RENDERTILE, 
        qs105(0), qs105(15.9688), 
        qs510(1), qs510(-1));
    gDLBuilder->needsPipeSync = TRUE;
    
    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_03_InfoScroll_Roll_End], 0);
    tempY = y - (11 << 2);
    gSPTextureRectangle(dl++,
        sp10C - sp104 - (16 << 2),
        tempY, 
        sp10C - sp104, 
        tempY + (16 << 2), 
        G_TX_RENDERTILE, 
        qs105(0), qs105(15.9688), 
        qs510(1), qs510(-1));
    gDLBuilder->needsPipeSync = TRUE;
    
    gSPTextureRectangle(dl++,
        sp10C + sp104, 
        tempY, 
        sp10C + sp104 + (16 << 2), 
        tempY + (16 << 2), 
        G_TX_RENDERTILE, 
        qs105(15), qs105(15.9688), 
        qs510(-1), qs510(-1));
    gDLBuilder->needsPipeSync = TRUE;

    tempY = (y + height) - (4 << 2);
    gSPTextureRectangle(dl++,
        sp10C - sp104 - (16 << 2), 
        tempY, 
        sp10C - sp104, 
        tempY + (16 << 2), 
        G_TX_RENDERTILE, 
        qs105(0), qs105(15.9688), 
        qs510(1), qs510(-1));
    gDLBuilder->needsPipeSync = TRUE;
    
    gSPTextureRectangle(dl++,
        sp10C + sp104, 
        tempY, 
        sp10C + sp104 + (16 << 2), 
        tempY + (16 << 2), 
        G_TX_RENDERTILE, 
        qs105(15), qs105(15.9688), 
        qs510(-1), qs510(-1));
    gDLBuilder->needsPipeSync = TRUE;
    
    dl_set_prim_color(&dl, 255, 255, 255, 255);
    rcp_screen_full_write(&dl, sAButtonAnimTex, _data_58 - 8, (_data_54 + _data_50) - 24, 0, _bss_5BC >> 8, _data_60, SCREEN_WRITE_TRANSLUCENT);

    font_window_set_coords(3, 
        /*x1*/ _data_58 - _data_4C, 
        /*y1*/ _data_54, 
        /*x2*/ _data_58 + _data_4C, 
        /*y2*/ _data_54 + _bss_C2C);
    font_window_use_font(3, 1);
    font_window_set_bg_colour(3, 0, 0, 0, 0);
    font_window_flush_strings(3);
    font_window_set_text_colour(3, 0, 0, 255, 255, _data_60);
    for (i = _bss_C2E, y = 3; i < _bss_C30->count && i < (_bss_C2E + 3); i++, y += 16) {
        font_window_add_string_xy(3, -0x8000, y, _bss_C30->strings[i], 1, ALIGN_TOP_CENTER);
    }
    font_window_draw(&dl, mtxs, vtxs, 3);
    *gdl = dl;
}

// offset: 0x1FEC | func: 18
static void cmdmenu_func_1FEC(void) {
    s16* pageUnk4;
    Object* player;
    InventoryItem *pageItems;
    u32 pageMsg;
    s32 pageBtnMask;
    s32 usedGamebit;
    s8 isSidekickMenu;

    player = get_player();

    isSidekickMenu = FALSE;

    //Do nothing if the player's unloaded
    if (player == NULL) {
        return;
    }

    //Lock/unlock accessing the C-button scroll menu
    if (player->unkB0 & 0x1000) {
        joy_set_button_mask(0, L_CBUTTONS | R_CBUTTONS | D_CBUTTONS);
    } else if (sJoyButtonMask != 0) {
        joy_set_button_mask(0, sJoyButtonMask);
    }

    //Get button presses (or simulated ones, for tutorial sequences)
    if (sShouldOverrideJoypadButtons) {
        sJoyPressedButtons = sJoyPressedButtonsOverride;
    } else {
        sJoyPressedButtons = joy_get_pressed(0);
        if ((player->unkB0 & 0x1000) || (sJoyButtonMask != 0)) {
            sJoyPressedButtons |= B_BUTTON;
        }
    }

    //Play item use sound if needed
    switch (sUsedItemSound) {
    case CMDMENU_SOUND_NONE:
        break;
    case CMDMENU_SOUND_ITEM:
        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_79C_Cmdmenu_CantUse, MAX_VOLUME, NULL, NULL, 0, NULL);
        break;
    case CMDMENU_SOUND_PAGE:
        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_814_Cmdmenu_OpenSubMenu, MAX_VOLUME, NULL, NULL, 0, NULL);
        break;
    }

    sUsedItemGamebitID = NO_GAMEBIT;
    sUsedItemSound = CMDMENU_SOUND_NONE;
    _bss_C3D = -1;
    pageItems = dCmdmenuPages[sInventoryPageID].items;
    pageUnk4 = &dCmdmenuPages[sInventoryPageID].unk4;
    pageMsg = dCmdmenuPages[sInventoryPageID].mesgID;
    pageBtnMask = dCmdmenuPages[sInventoryPageID].btnMask;

    if ((sInventoryPageID == CMDMENU_PAGE_7_Sidekick_Kyte) || (sInventoryPageID == CMDMENU_PAGE_8_Sidekick_Tricky)) {
        isSidekickMenu = TRUE;
    }

    usedLoadedIdx = *pageUnk4;
    _bss_C44 = cmdmenu_func_325C(pageItems, isSidekickMenu);

    if (_bss_C44 == 0) {
        dPageCategory = 0;
        cmdmenu_close_inventory();
        return;
    }

    if (usedLoadedIdx >= _bss_C44) {
        usedLoadedIdx = 0;
    }

    _data_18 = sMenuItemTextIDs[usedLoadedIdx];

    if (cmdmenu_is_inventory_open()) {
        if ((sJoyPressedButtons & pageBtnMask) && (sInventoryScrollOffset < 8) && (_data_7C == 0)) {
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_28A_Cmdmenu_MoveSelection, MAX_VOLUME, NULL, NULL, 0, NULL);
            _data_78 += 1;
            if (sInventoryScrollOffset != 0) {
                _data_7C = 1;
            }
        }
        if (_data_78 > 255) {
            _data_78 = 255;
        }
        if (_bss_C7A != -1) {
            usedLoadedIdx = _bss_C7A;
        }

        if ((_data_78 > 0) && (sInventoryScrollOffset == 0)) {
            _data_78 -= 1;
            if (_bss_C44 >= 2) {
                if (((_bss_C44 == 2) && (usedLoadedIdx == 1)) || ((_bss_C44 == 4) && (usedLoadedIdx == 3))) {
                    sInventoryScrollOffset = 64;
                    dInventoryMoveSpeed = 2;
                    _data_7C = 0;
                } else {
                    sInventoryScrollOffset = 32;
                    dInventoryMoveSpeed = 2;
                    _data_7C = 0;
                }
                usedLoadedIdx++;
                if (usedLoadedIdx >= _bss_C44) {
                    usedLoadedIdx = 0;
                }
            }
        } else if (sJoyPressedButtons & B_BUTTON) {
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_28C_Cmdmenu_Close, MAX_VOLUME, NULL, NULL, 0, NULL);
            cmdmenu_close_inventory();
        } else if ((sJoyPressedButtons & A_BUTTON) && cmdmenu_is_inventory_open()) {
            usedGamebit = sMenuItemGamebits[usedLoadedIdx];
            if (isSidekickMenu == FALSE) {
                obj_send_mesg(player, pageMsg, NULL, (void* ) usedGamebit);
                sUsedItemGamebitID = usedGamebit;
                sUsedItemSound = _bss_4D8[usedLoadedIdx];
                _bss_C3D = _bss_498[usedLoadedIdx];
                gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_28B_Cmdmenu_Use, MAX_VOLUME, NULL, NULL, 0, NULL);
                cmdmenu_close_inventory();
            } else {
                if (sMenuItemVisibilities[usedLoadedIdx] != 0) {
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_28B_Cmdmenu_Use, MAX_VOLUME, NULL, NULL, 0, NULL);
                    cmdmenu_close_inventory();
                    sUsedItemGamebitID = usedGamebit;
                    sUsedItemSound = 0;
                } else {
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_A0, MAX_VOLUME, NULL, NULL, 0, NULL);
                    sUsedItemGamebitID = NO_GAMEBIT;
                    sUsedItemSound = 0;
                }
            }
        }
    }

    if (cmdmenu_func_3A4C() != 0) {
        dPageCategory = 0;
        _bss_C4C = 0;
        _data_78 = 0;
    } else {
        joy_set_button_mask(0, A_BUTTON | B_BUTTON);
    }

    *pageUnk4 = usedLoadedIdx;
}

// offset: 0x26D8 | func: 19
/** 
  * Returns the textableID of a Magic Spell's transparent UI texture (or -1 if unsuccessful).
  *
  * The Spell is queried using its gamebit.
  */
static s16 cmdmenu_get_spell_textable(s32 spellGamebit) {
    u32 i;
    for (i = 0; i < ARRAYCOUNT(dSpellGamebits); i++) {
        if (spellGamebit == dSpellGamebits[i]) {
            return dSpellTextableIDs[i];
        }    
    }
    return NO_TEXTURE;
}

// offset: 0x27D8 | func: 20
static void cmdmenu_func_27D8(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    s16 commandTexTableID;
    Object* player;
    s32 activeSpellGamebit;
    s32 var_a2;
    s32 spellTexTableID;
    s32 var_s0;
    s8 sp70[64];
    s32 var_s1;
    s32 i;
    s32 sideCommandIndex;
    s32 var_s5;
    u8 iconOpacity;
    u8 pageIcon;
    s8 offsetX;
    s8 offsetY;
    Object* sidekick;

    player = get_player();
    offsetX = 0;
    offsetY = 0;
    sidekick = get_sidekick();

    //Draw item info pop-up
    cmdmenu_info_draw(gdl, &sInfoPopup);

    //Draw active spell icon
    {
        activeSpellGamebit = ((DLL_210_Player*)player->dll)->vtbl->func50(player);

        //Clear the icon's data before any change
        if ((sActiveSpellIcon != NULL) && (activeSpellGamebit != sPrevActiveSpellGamebit)) {
            tex_free(sActiveSpellRing);
            tex_free(sActiveSpellIcon);
            sPrevActiveSpellGamebit = NO_GAMEBIT;
            sActiveSpellIcon = NULL;
        }

        //Load the icon's textures when needed
        if ((sActiveSpellIcon == NULL) && (activeSpellGamebit != NO_GAMEBIT)) {
            spellTexTableID = cmdmenu_get_spell_textable(activeSpellGamebit);
            if (spellTexTableID != NO_TEXTURE) {
                sActiveSpellIcon = tex_load_deferred(spellTexTableID);
                sActiveSpellRing = tex_load_deferred(TEXTABLE_574_CMDMENU_Active_Spell_Ring);
            }
        }

        sPrevActiveSpellGamebit = activeSpellGamebit;

        if (sActiveSpellIcon != NULL) {
            rcp_screen_full_write(gdl, sActiveSpellRing, 253, 169, 0, 0, 0xFF, SCREEN_WRITE_TRANSLUCENT);
            rcp_screen_full_write(gdl, sActiveSpellIcon, 262, 180, 0, 0, 0xFF, SCREEN_WRITE_TRANSLUCENT);
        }
    }

    //Draw active sidekick command icon
    if (sidekick != NULL) {
        ((DLL_ISidekick*)sidekick->dll)->vtbl->func26(sidekick, &sideCommandIndex);

        //Clear the icon's data before any change
        if ((sActiveSidekickCommandIcon != NULL) && (sideCommandIndex != sPrevSidekickCommandIndex)) {
            tex_free(sActiveSidekickCommandRing);
            tex_free(sActiveSidekickCommandIcon);
            sPrevSidekickCommandIndex = NO_SIDEKICK_COMMAND;
            sActiveSidekickCommandIcon = NULL;
        }

        //Load the icon's textures when needed
        if (sActiveSidekickCommandIcon == NULL && sideCommandIndex > 0) {
            commandTexTableID = dCommandTextableIDs[sideCommandIndex];
            if (commandTexTableID != NO_TEXTURE) {
                sActiveSidekickCommandIcon = tex_load_deferred(commandTexTableID);
                sActiveSidekickCommandRing = tex_load_deferred(TEXTABLE_584_CMDMENU_Active_Sidekick_Command_Ring);
            }
        }

        sPrevSidekickCommandIndex = sideCommandIndex;

        if (sActiveSidekickCommandIcon != NULL) {
            rcp_screen_full_write(gdl, sActiveSidekickCommandRing, 253, 121, 0, 0, 0xFF, SCREEN_WRITE_TRANSLUCENT);
            rcp_screen_full_write(gdl, sActiveSidekickCommandIcon, 262, 132, 0, 0, 0xFF, SCREEN_WRITE_TRANSLUCENT);
        }
    }

    cmdmenu_draw_energy_bar(gdl);

    if (_bss_C44 != 0) {
        if (dOpacityInventoryScroll != 0) {
            var_s5 = 3;
            if (_bss_C44 < 4) {
                var_s0 = 1;
            } else {
                var_s5 = 5;
                var_s0 = 2;
            }

            cmdmenu_func_4630(gdl);

            _bss_C60->y = 0;
            var_a2 = 95 - (var_s5 * 12);
            _bss_C60[1].tex = NULL;

            for (i = 0; i < _bss_C44; i++) {
                sp70[i] = 0;
            }

            for (i = _bss_C44; i < var_s5; i++) {
                sp70[i] = 1;
            }

            if (_bss_C44 < var_s5) {
                _bss_C44 = var_s5;
            }

            if (sInventoryScrollOffset > 0) {
                var_s0 += 1;
                var_a2 -= 24;
                var_s5 += 1;
            }

            if (sInventoryScrollOffset > 24) {
                var_s0 += 1;
                var_a2 -= 24;
                var_s5 += 1;
            }

            var_a2 += sInventoryUnrollY - dInventoryUnrollMax;
            var_s1 = usedLoadedIdx - var_s0;

            if (var_s1 < 0) {
                var_s1 += _bss_C44;
            }

            for (i = 0; i < var_s5; i++) {
                if (sp70[var_s1] == 0) {
                    _bss_C60->animProgress = 0;
                    iconOpacity = dOpacityInventoryScroll;

                    if ((var_s1 == usedLoadedIdx) && (sInventoryScrollOffset == 0)) {
                        _bss_C60->x = 0;
                        _bss_C60->y = 0;
                    } else {
                        _bss_C60->x = 0;
                        _bss_C60->y = 0;
                    }

                    if (sMenuItemVisibilities[var_s1] != 0) {
                        _bss_C60->tex = sMenuItemTextures[var_s1];

                        if (func_80041E08() != 0) {
                            rcp_tile_write(
                                gdl, 
                                _bss_C60, 
                                261, 
                                ((i * 24) + var_a2) + sInventoryScrollOffset, 
                                0xFF, 0xFF, 0xFF, iconOpacity
                            );
                        } else {
                            rcp_tile_write(
                                gdl, 
                                _bss_C60, 
                                262, 
                                ((i * 24) + var_a2) + sInventoryScrollOffset, 
                                0xFF, 0xFF, 0xFF, iconOpacity
                            );
                        }

                        if (sMenuItemQuantities[var_s1] >= 2) {
                            _bss_C60->tex = sInventoryStackNumbersTex;
                            _bss_C60->animProgress = (sMenuItemQuantities[var_s1] - 2) << 8;
                            rcp_tile_write(
                                gdl, 
                                _bss_C60, 
                                276, 
                                ((i * 24) + var_a2) + sInventoryScrollOffset + 9, 
                                0xFF, 0xFF, 0xFF, 0xFF
                            );
                        }
                    }
                } else {
                    if (func_80041E08() != 0) {
                        rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_00_Scroll_BG], 261, ((i * 24) + var_a2) + sInventoryScrollOffset, 0xFF, 0xFF, 0xFF, 0xFF);
                    } else {
                        rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_00_Scroll_BG], 262, ((i * 24) + var_a2) + sInventoryScrollOffset, 0xFF, 0xFF, 0xFF, 0xFF);
                    }
                }

                var_s1 += 1;
                if (var_s1 >= _bss_C44) {
                    var_s1 -= _bss_C44;
                }
            }

            //Draw a square around the currently highlighted item
            rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_31_Highlight_Corner_Top_Left],     POS_ITEM_HL_X1, (sInventoryUnrollY - dInventoryUnrollMax) + POS_ITEM_HL_Y1,  255, 255, 255, dOpacityInventoryScroll);
            rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_32_Highlight_Corner_Top_Right],    POS_ITEM_HL_X2, (sInventoryUnrollY - dInventoryUnrollMax) + POS_ITEM_HL_Y1,  255, 255, 255, dOpacityInventoryScroll);
            rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_33_Highlight_Corner_Bottom_Left],  POS_ITEM_HL_X1, (sInventoryUnrollY - dInventoryUnrollMax) + POS_ITEM_HL_Y2, 255, 255, 255, dOpacityInventoryScroll);
            rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_34_Highlight_Corner_Bottom_Right], POS_ITEM_HL_X2, (sInventoryUnrollY - dInventoryUnrollMax) + POS_ITEM_HL_Y2, 255, 255, 255, dOpacityInventoryScroll);
            
            cmdmenu_gfx_set_screen_scissor(gdl);
        }

        //Draw page icon (Bag/SpellBook/Kyte/Tricky)
        {
            if (dInventoryShow != 0 || dOpacityInventoryScroll == 0xFF || (dOpacityInventoryScroll != 0 && _data_14 == 0)) {
                switch (sInventoryPageID) {
                case CMDMENU_PAGE_7_Sidekick_Kyte:
                    pageIcon = CMDMENU_TEX_42_Tricky;
                    offsetY = 3;
                    break;
                case CMDMENU_PAGE_8_Sidekick_Tricky:
                    pageIcon = CMDMENU_TEX_54_Kyte;
                    break;
                case CMDMENU_PAGE_6_Spells:
                    offsetX = -2;
                    offsetY = 9;
                    pageIcon = CMDMENU_TEX_49_MagicBook;
                    break;
                default:
                case CMDMENU_PAGE_0_Items_Krystal:
                case CMDMENU_PAGE_1_Items_Sabre:
                case CMDMENU_PAGE_2_Food_Actions_Krystal:
                case CMDMENU_PAGE_3_Food_Actions_Sabre:
                case CMDMENU_PAGE_4_Food_Krystal:
                case CMDMENU_PAGE_5_Food_Sabre:
                    offsetX = 1;
                    offsetY = 9;
                    pageIcon = CMDMENU_TEX_50_Bag;
                    break;
                }
                if (_data_14 < dOpacityInventoryScroll) {
                    iconOpacity = dOpacityInventoryScroll;
                } else {
                    iconOpacity = _data_14;
                }
            } else {
                if (_data_14 != 0) {
                    pageIcon = CMDMENU_TEX_42_Tricky;
                    if (sidekick != NULL && sidekick->id == OBJ_Kyte) {
                        pageIcon = CMDMENU_TEX_54_Kyte;
                        iconOpacity = _data_14;
                    } else {
                        offsetY = 3;
                        iconOpacity = _data_14;
                    }
                } else {
                    iconOpacity = 0;
                }
            }

            if (iconOpacity) {
                dInventoryPageIcon = tex_load_deferred(dTextableIDs[pageIcon]);
                rcp_screen_full_write(
                    gdl, 
                    dInventoryPageIcon, 
                    POS_PAGE_ICON_X + offsetX,
                    POS_PAGE_ICON_Y + offsetY,
                    0, 
                    0, 
                    iconOpacity, 
                    SCREEN_WRITE_TRANSLUCENT
                );
                tex_free(dInventoryPageIcon);
            }
        }
    }
}

// offset: 0x325C | func: 21
static s32 cmdmenu_func_325C(InventoryItem* items, s8 isSidekickMenu) {
    s32 i;
    s32 loadIdx;
    s32 obtained;
    s32 availableCommands;
    Object* sidekick;
    Texture* prevMenuItemTextures[MAX_LOADED_ITEMS];
    Texture* prevMenuSidekickCommandTextures[MAX_LOADED_ITEMS];

    //Set up loaded item arrays
    for (i = 0; i < MAX_LOADED_ITEMS; i++) {
        //Store previous menu's textures so they can be freed
        prevMenuItemTextures[i] = sMenuItemTextures[i];
        prevMenuSidekickCommandTextures[i] = sMenuSidekickCommandTextures[i];

        //Reset arrays
        sMenuItemTextures[i] = NULL;
        sMenuItemTextureIDs[i] = NO_TEXTURE;
        sMenuItemQuantities[i] = 1;
        sMenuSidekickCommandTextures[i] = NULL;
    }

    loadIdx = 0;
    i = 0;

    //Items/Spells
    if (isSidekickMenu == FALSE) {
        _bss_C7A = -1;
        while (items[i].gamebitObtained > NO_GAMEBIT) {
            obtained = main_get_bits(items[i].gamebitObtained);
            if (obtained) {
                //Magic Spells
                if (items == dPage6MagicSpells) {
                    sMenuItemTextures[loadIdx] = tex_load_deferred(items[i].textureID);
                    sMenuItemTextureIDs[loadIdx] = items[i].textureID;
                    sMenuItemGamebits[loadIdx] = items[i].gamebitObtained;

                    //Cap the displayed quantity at 10 (should always just be 1 for Spells)
                    if (obtained > 10) {
                        obtained = 10;
                    }
                    sMenuItemQuantities[loadIdx] = obtained;

                    cmdmenu_func_3880(items, loadIdx, i);

                    //Store item's visibility state (Spells never get taken away, though)
                    if ((items[i].gamebitHide < 0) || (main_get_bits(items[i].gamebitHide) == 0)) {
                        sMenuItemVisibilities[loadIdx] = TRUE;
                    } else {
                        sMenuItemVisibilities[loadIdx] = FALSE;
                    }

                    loadIdx++;

                //Other items
                } else {
                    //Only load non-magic items if they aren't used/hidden
                    if ((items[i].gamebitHide <= NO_GAMEBIT) || (main_get_bits(items[i].gamebitHide) == FALSE)) {
                        
                        if ((_bss_C78 != 0) && (_bss_C78 == items[i].gamebitObtained)) {
                            _bss_C7A = loadIdx;
                        }

                        sMenuItemTextures[loadIdx] = tex_load_deferred(items[i].textureID);
                        sMenuItemTextureIDs[loadIdx] = items[i].textureID;
                        sMenuItemGamebits[loadIdx] = items[i].gamebitObtained;

                        //Cap the displayed quantity at 10
                        if (obtained > 10) {
                            obtained = 10;
                        }
                        sMenuItemQuantities[loadIdx] = obtained;

                        cmdmenu_func_3880(items, loadIdx, i);

                        sMenuItemVisibilities[loadIdx] = TRUE;

                        loadIdx++;
                    }
                }
            }
            i++;
        }

    //Sidekick Commands
    } else {
        sidekick = get_sidekick();

        //Get a bitfield of the sidekick's available commands
        if (sidekick != NULL) {
            availableCommands = ((DLL_ISidekick*)sidekick->dll)->vtbl->func13(sidekick);
        } else {
            availableCommands = 0;
        }

        if (availableCommands != NO_SIDEKICK_COMMAND) {

            while (items[i].gamebitObtained >= 0) {
                //Only load a sidekick command item if it's currently available
                if (items[i].gamebitObtained & availableCommands) {
                    sMenuItemTextures[loadIdx] = tex_load_deferred(items[i].textureID);
                    sMenuItemQuantities[loadIdx] = 1;

                    if (items[i].sidekickCommand != NO_TEXTURE) {
                        sMenuSidekickCommandTextures[loadIdx] = tex_load_deferred(items[i].sidekickCommand);
                    } else {
                        sMenuSidekickCommandTextures[loadIdx] = NULL;
                    }

                    sMenuItemGamebits[loadIdx] = items[i].gamebitHide;
                    cmdmenu_func_3880(items, loadIdx, i);
                    sMenuItemVisibilities[loadIdx] = TRUE;

                    loadIdx++;
                }
                i++;
            }
        }
    }

    //Free the previous menu's textures
    for (i = 0; i < MAX_LOADED_ITEMS; i++) {
        if (prevMenuItemTextures[i] != NULL) {
            tex_free(prevMenuItemTextures[i]);
        }
        if (prevMenuSidekickCommandTextures[i] != NULL) {
            tex_free(prevMenuSidekickCommandTextures[i]);
        }
    }

    return loadIdx;
}

// offset: 0x3718 | func: 22
/**
  * Returns how many items will be shown in a given menu page.
  *
  * For sidekick command pages: an item will only be shown if the sidekick is loaded, and
  * the command is unhidden (e.g. near a Find target)
  */
static s32 cmdmenu_page_count_shown_items(InventoryItem* menuItems, s8 isSidekickMenu) {
    Object* sidekick;
    s32 shownCount;
    s32 availableCommands;
    s32 itemIdx;

    shownCount = 0;
    itemIdx = 0;

    //Items/Spells
    if (isSidekickMenu == FALSE) {
        while (menuItems[itemIdx].gamebitObtained > NO_GAMEBIT) {
            if (main_get_bits(menuItems[itemIdx].gamebitObtained)) {
                if (menuItems == dPage6MagicSpells) {
                    //Spells shown once obtained
                    shownCount++;
                } else if ((menuItems[itemIdx].gamebitHide <= NO_GAMEBIT) || (main_get_bits(menuItems[itemIdx].gamebitHide) == FALSE)) {
                    //Items shown if obtained and not yet used/hidden
                    shownCount++;
                }
            }
            itemIdx++;
        }

    //Sidekick Commands
    } else {
        sidekick = get_sidekick();
        if (sidekick != NULL) {
            availableCommands = ((DLL_ISidekick*)sidekick->dll)->vtbl->func13(sidekick);
        } else {
            availableCommands = 0;
        }

        if (availableCommands > 0) {
            while (menuItems[itemIdx].gamebitObtained >= 0) {
                if (menuItems[itemIdx].gamebitObtained) {
                    shownCount++;
                }
                itemIdx++;
            }
        }
    }

    return shownCount;
}

// offset: 0x3880 | func: 23
static void cmdmenu_func_3880(InventoryItem* items, s32 loadedItemIndex, s32 itemIndex) {
    sMenuItemTextIDs[loadedItemIndex] = items[itemIndex].textID;
    _bss_498[loadedItemIndex] = items[itemIndex].openPage;
    _bss_4D8[loadedItemIndex] = items[itemIndex].sound;
}

// offset: 0x38E4 | func: 24
static void cmdmenu_func_38E4(Gfx** gdl, Texture* tex, s32 frame) {
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
/**
  * Checks if the inventory scroll menu is fully open.
  */
static s32 cmdmenu_is_inventory_open(void) {
    //Check if the inventory is closed
    if (dInventoryShow == FALSE) {
        return FALSE;
    }

    //Check if the scroll isn't fully unrolled
    if (sInventoryUnrollY != dInventoryUnrollMax) {
        return FALSE;
    }

    return TRUE;
}

// offset: 0x3A4C | func: 26
static s32 cmdmenu_func_3A4C(void) {
    if (dInventoryShow != 0) {
        return 0;
    }

    if (sInventoryUnrollY != 0) {
        return 0;
    }

    return 1;
}

// offset: 0x3A94 | func: 27
static void cmdmenu_close_inventory(void) {
    dInventoryShow = FALSE;
}

// offset: 0x3AB0 | func: 28
static void cmdmenu_open_inventory(void) {
    dInventoryShow = TRUE;
}

// offset: 0x3AD0 | func: 29
static void cmdmenu_func_3AD0(void) {
    //Animate moving to the next inventory item
    sInventoryScrollOffset -= gUpdateRate << dInventoryMoveSpeed;
    if (sInventoryScrollOffset < 0) {
        sInventoryScrollOffset = 0;
    }

    //Manage the inventory scroll's opacity
    if (dInventoryShow) {
        //Fade in when opened
        dOpacityInventoryScroll += gUpdateRate * 8;
        if (dOpacityInventoryScroll > MAX_OPACITY) {
            dOpacityInventoryScroll = MAX_OPACITY;
        }
    } else if (sInventoryUnrollY == 0) {
        //Fade out when fully closed
        dOpacityInventoryScroll -= gUpdateRate * 8;
        if (dOpacityInventoryScroll < 0) {
            dOpacityInventoryScroll = 0;
        }
    }

    //Animate the inventory scroll expanding/collapsing
    if (dInventoryShow && (dOpacityInventoryScroll > 64)) {
        //Expand/unfurl scroll when inventory partially faded in
        sInventoryUnrollY += gUpdateRate * 4;
        if (sInventoryUnrollY > dInventoryUnrollMax) {
            sInventoryUnrollY = dInventoryUnrollMax;
        } else {
            _data_118++;
            _data_11C--;
        }
    } else {
        sInventoryUnrollY -= gUpdateRate * 4;
        if (sInventoryUnrollY < 0) {
            sInventoryUnrollY = 0;
        } else {
            _data_118--;
            _data_11C++;
        }
    }

    if (dOpacityInventoryScroll) {
        if (sInventoryScrollOffset > 0) {
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
static void cmdmenu_func_3D28(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    Gfx* dl;
    Object* sidekick;
    Object* player;
    u8 texIdx;
    u8 pad_sp4A;
    u8 isKyte;
    u8 pageIdx;
    u8 hasHalfRed;
    u8 fullBlueEnd;
    u8 hasHalfBlue;
    u8 sp44;
    u8 fullRedEnd;
    u8 i;
    u8 iconIndex;

    sidekick = get_sidekick();
    player = get_player();
    isKyte = FALSE;
    sp44 = 0;

    if (sidekick != NULL) {
        isKyte = (sidekick->id == OBJ_Kyte);
    }

    //Draw C buttons while the inventory scroll is hidden
    if (dOpacityInventoryScroll == 0) {
        if (sOpacityR != 0.0f) {
            if (player != NULL) {
                pageIdx = player->id == OBJ_Krystal ? CMDMENU_PAGE_0_Items_Krystal : CMDMENU_PAGE_1_Items_Sabre;
                sp44 = 0;
                if (cmdmenu_page_count_shown_items(dCmdmenuPages[pageIdx].items, FALSE) != 0) {
                    sp44 = 2;
                }
            }

            if (sidekick != NULL) {
                pageIdx = sidekick->id == OBJ_Kyte ? CMDMENU_PAGE_8_Sidekick_Tricky : CMDMENU_PAGE_7_Sidekick_Kyte;
                if (cmdmenu_page_count_shown_items(dCmdmenuPages[pageIdx].items, TRUE) != 0) {
                    sp44 |= 4;
                }
            }

            if (cmdmenu_page_count_shown_items(dCmdmenuPages[CMDMENU_PAGE_6_Spells].items, FALSE) != 0) {
                sp44 |= 1;
            }

            if (sp44 & 2) {
                texIdx = CMDMENU_TEX_47_RightButton_With_Bag;
                dInventoryPageIcon = tex_load_deferred(dTextableIDs[texIdx]);
                rcp_screen_full_write(gdl, dInventoryPageIcon, 275, 25, 0, 0, sOpacityR, SCREEN_WRITE_TRANSLUCENT);
                tex_free(dInventoryPageIcon);
            } else {
                texIdx = CMDMENU_TEX_41_C_Right;
                rcp_tile_write(gdl, sTextureTiles[texIdx], 274, 34, 255, 255, 255, sOpacityR);
            }

            if (((sp44 & 4) && (sidekick != NULL)) || (sp44 & 1)) {
                if ((sp44 & 4) && (sidekick != NULL) && (sp44 & 1)) {
                    if (isKyte) {
                        texIdx = CMDMENU_TEX_38_LeftDownButtons_With_Kyte;
                    } else {
                        texIdx = CMDMENU_TEX_43_LeftDownButtons_With_Tricky;
                    }
                } else if (sp44 & 2) {
                    texIdx = CMDMENU_TEX_48_LeftDownButtons_NoSidekick;
                }

                dInventoryPageIcon = tex_load_deferred(dTextableIDs[texIdx]);
                rcp_screen_full_write(gdl, dInventoryPageIcon, 245, 17, 0, 0, sOpacityR, SCREEN_WRITE_TRANSLUCENT);
                tex_free(dInventoryPageIcon);
            } else {
                rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_37_C_Down], 252, 43, 255, 255, 255, sOpacityR);
                rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_39_C_Left], 246, 26, 255, 255, 255, sOpacityR);
            }
        }
    } else {
        sOpacityR = 0.0f;
    }

    dl = *gdl;

    if (dOpacityInventoryScroll != 0) {
        rcp_tile_write(&dl, sTextureTiles[CMDMENU_TEX_02_Scroll_Top], 258, 51, 255, 255, 255, dOpacityInventoryScroll);
        rcp_tile_write(&dl, sTextureTiles[CMDMENU_TEX_01_Scroll_Bottom], 258, sInventoryUnrollY + 59, 255, 255, 255, dOpacityInventoryScroll);
    }

    if (sidekick != NULL) {
        if ((
            ((sInventoryPageID == CMDMENU_PAGE_7_Sidekick_Kyte) || 
             (sInventoryPageID == CMDMENU_PAGE_8_Sidekick_Tricky)
            ) && 
            (dOpacityInventoryScroll != 0)) || (sStatsChangeTimers.sidekickBlueFood >= 0.0f)
        ) {
            _data_14 += gUpdateRate * 8;
            if (_data_14 > MAX_OPACITY) {
                _data_14 = MAX_OPACITY;
            }
        } else {
            _data_14 -= gUpdateRate * 8;
            if (_data_14 < 0) {
                _data_14 = 0;
            }
        }

        if (_data_14 != 0) {
            //@unfinished: hard-coded food quantities
            sStats.sidekickRedFood = 3;
            sStats.sidekickBlueFood = 3;

            fullRedEnd = sStats.sidekickRedFood >> 1;
            hasHalfRed = sStats.sidekickRedFood & 1;
            fullBlueEnd = (sStats.sidekickBlueFood >> 1) + fullRedEnd + hasHalfRed;
            hasHalfBlue = sStats.sidekickBlueFood & 1;

            for (i = 0; i < sStats.sidekickMaxFood; i++) {
                if (i < fullRedEnd) {
                    if (isKyte) {
                        iconIndex = CMDMENU_TEX_56_Grub_Red_Full;
                    } else {
                        iconIndex = CMDMENU_TEX_45_Mushroom_Red_Full;
                    }
                } else if (i == fullRedEnd && hasHalfRed) {
                    if (isKyte) {
                        iconIndex = CMDMENU_TEX_57_Grub_Red_Half;
                    } else {
                        iconIndex = CMDMENU_TEX_46_Mushroom_Red_Half;
                    }
                } else if (i < fullBlueEnd) {
                    if (isKyte) {
                        iconIndex = CMDMENU_TEX_13_Grub_Blue_Full;
                    } else {
                        iconIndex = CMDMENU_TEX_12_Mushroom_Blue_Full;
                    }
                } else if (i == fullBlueEnd && hasHalfBlue) {
                    if (isKyte) {
                        iconIndex = CMDMENU_TEX_16_Grub_Blue_Half;
                    } else {
                        iconIndex = CMDMENU_TEX_51_Mushroom_Blue_Half;
                    }
                } else {
                    if (isKyte) {
                        iconIndex = CMDMENU_TEX_55_Grub_Empty;
                    } else {
                        iconIndex = CMDMENU_TEX_44_Mushroom_Empty;
                    }
                }
                
                rcp_tile_write(&dl, sTextureTiles[iconIndex], 
                    250 - ((i / 4) * 9), 
                    21  + ((i % 4) * 8), 
                    255, 255, 255, _data_14);
            }
        }
    }

    dl_set_prim_color(&dl, 255, 255, 255, 255);

    *gdl = dl;
}

// offset: 0x4630 | func: 31
static void cmdmenu_func_4630(Gfx **gdl) {
    if (func_80041E08() != 0) {
        gDPSetScissorFrac((*gdl)++, G_SC_NON_INTERLACE, 
            qu102(314.75), 
            qu102(48.25), 
            qu102(366.75), 
            (((f32) sInventoryUnrollY * 0.82f) + 48.38f + 2.0f) * 4.0f);
    } else {
        gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 
            262, 
            59, 
            294, 
            sInventoryUnrollY + 0x3D);
    }
}

// offset: 0x474C | func: 32
static void cmdmenu_gfx_set_screen_scissor(Gfx** gfx) {
    s32 resolution;

    resolution = vi_get_current_size();
    gDPSetScissor((*gfx)++, 0 , 0, 0, GET_VIDEO_WIDTH(resolution), GET_VIDEO_HEIGHT(resolution));
}

// offset: 0x47E8 | func: 33
static s32 cmdmenu_func_47E8(void) {
    if (_data_1C != 0) {
        return 0;
    }
    if (_bss_C2A != 0) {
        return 0;
    }
    return 1;
}

// offset: 0x4830 | func: 34
static void cmdmenu_func_4830(void) {
    _data_1C = 0;
}

// offset: 0x484C | func: 35
static void cmdmenu_func_484C(void) {
    _data_1C = 1;
}

// offset: 0x486C | func: 36
static void cmdmenu_func_486C(void) {
    if ((_data_1C != 0) && (_data_84 == 0)) {
        dOpacityInfoScroll += gUpdateRate * 32;
        if (dOpacityInfoScroll > 160) {
            dOpacityInfoScroll = 160;
        }
    } else if (_bss_C2A == 0) {
        dOpacityInfoScroll -= gUpdateRate * 32;
        if (dOpacityInfoScroll < 0) {
            dOpacityInfoScroll = 0;
        }
    }

    if ((_data_1C != 0) && (dOpacityInfoScroll == 160)) {
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

    if (dOpacityInfoScroll == 0) {
        if (*dTextStrings != NULL) {
            mmFree(*dTextStrings);
            *dTextStrings = NULL;
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
static void cmdmenu_print_info_scroll(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    s32 charIdx;
    s32 y;
    s32 sp104;
    s32 height;
    s32 tempY;
    s32 lineIdx;
    Gfx* dl;

    if (dOpacityInfoScroll == 0) {
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
    dl_set_prim_color(&dl, 255, 255, 255, dOpacityInfoScroll);
    charIdx = _data_2C << 2;
    sp104 = _data_20 << 2;
    sp104 -= (16 << 2);
    y = _data_28 << 2;
    height = _bss_C2A << 2;

    //Draw scroll's paper BG
    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_06_InfoScroll_BG], 0);
    gSPTextureRectangle(dl++, 
        /*ulx*/ charIdx - sp104,
        /*uly*/ y,
        /*lrx*/ charIdx + sp104,
        /*lry*/ y + height,
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(0), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
    );
    gDLBuilder->needsPipeSync = TRUE;

    //Draw scroll's left side
    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_05_InfoScroll_Side], 0);
    gSPTextureRectangle(dl++, 
        /*lrx*/ charIdx - sp104 - (16 << 2),
        /*lry*/ y,
        /*ulx*/ charIdx - sp104,
        /*uly*/ y + height,
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(0), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
    );
    gDLBuilder->needsPipeSync = TRUE;
    gSPTextureRectangle(dl++, 
        /*ulx*/ charIdx + sp104,
        /*uly*/ y,
        /*lrx*/ charIdx + sp104 + (16 << 2),
        /*lry*/ y + height,
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(15), /*t*/ qs105(0),
        /*dsdx*/ qs510(-1), /*dtdy*/ qs510(1)
    );
    gDLBuilder->needsPipeSync = TRUE;

    sp104 += (16 << 2);

    if (_bss_C2A >= 9) {
        cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_07_InfoScroll_SelfShadow], 0);
        dl_set_prim_color(&dl, 255, 128, 128, 128);
        
        tempY = y;
        gSPTextureRectangle(dl++, 
            /*ulx*/ charIdx - sp104,
            /*uly*/ tempY,
            /*lrx*/ charIdx + sp104,
            /*lry*/ tempY + (8 << 2),
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(7), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
        );
        gDLBuilder->needsPipeSync = TRUE;

        tempY = (y + height) - (6 << 2);
        gSPTextureRectangle(dl++, 
            /*ulx*/ charIdx - sp104,
            /*uly*/ tempY,
            /*lrx*/ charIdx + sp104,
            /*lry*/ tempY + (8 << 2),
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(0), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
        );
        gDLBuilder->needsPipeSync = TRUE;

        dl_set_prim_color(&dl, 255, 255, 255, dOpacityInfoScroll);
    }

    tempY = y - (11 << 2);
    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_04_InfoScroll_Roll], _data_120);
    gSPTextureRectangle(dl++, 
        /*ulx*/ charIdx - sp104,
        /*uly*/ tempY,
        /*lrx*/ charIdx + sp104,
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = TRUE;

    tempY = (y + height) - (4 << 2);
    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_04_InfoScroll_Roll], _data_124);
    gSPTextureRectangle(dl++, 
        /*ulx*/ charIdx - sp104,
        /*uly*/ tempY,
        /*lrx*/ charIdx + sp104,
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = TRUE;

    cmdmenu_func_38E4(&dl, sTextures[CMDMENU_TEX_03_InfoScroll_Roll_End], 0);
    tempY = y - (11 << 2);
    gSPTextureRectangle(dl++, 
        /*ulx*/ charIdx - sp104 - (16 << 2),
        /*uly*/ tempY,
        /*lrx*/ charIdx - sp104,
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = TRUE;
    gSPTextureRectangle(dl++, 
        /*ulx*/ charIdx + sp104,
        /*uly*/ tempY,
        /*lrx*/ charIdx + sp104 + (16 << 2),
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(15), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(-1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = TRUE;
    tempY = (y + height) - (4 << 2);
    gSPTextureRectangle(dl++, 
        /*ulx*/ charIdx - sp104 - (16 << 2),
        /*uly*/ tempY,
        /*lrx*/ charIdx - sp104,
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(0), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = TRUE;
    gSPTextureRectangle(dl++, 
        /*ulx*/ charIdx + sp104,
        /*uly*/ tempY,
        /*lrx*/ charIdx + sp104 + (16 << 2),
        /*lry*/ tempY + (16 << 2),
        /*tile*/ G_TX_RENDERTILE,
        /*s*/ qs105(15), /*t*/ qs105(15.9688), 
        /*dsdx*/ qs510(-1), /*dtdy*/ qs510(-1)
    );
    gDLBuilder->needsPipeSync = TRUE;

    dl_set_prim_color(&dl, 255, 255, 255, 255);

    //Return early if there's no gametext
    if (dGametextStringID < 0) {
        return;
    }

    //Get the gametext lines
    if (dTextStrings[0] == NULL) {
        //Get the item's string (use a different gametext file for lineIDs beyond 255) 
        dTextStrings[0] = dGametextStringID >= 256 
            ? gDLL_21_Gametext->vtbl->get_text(GAMETEXT_238_UI_Text_2, dGametextStringID - 256) 
            : gDLL_21_Gametext->vtbl->get_text(GAMETEXT_003_UI_Text_1, dGametextStringID);
        dTextStrings[1] = NULL;
        dTextStrings[2] = NULL;
        dTextStrings[3] = NULL;

        //Check for bar delimiter in text (separates lines)
        for (charIdx = 0, lineIdx = 1; dTextStrings[0][charIdx] != '\0'; charIdx++) {
            if (dTextStrings[0][charIdx] == '|') {
                dTextStrings[0][charIdx] = '\0';
                charIdx += 2; //ROM's text should have space after bar, so skipping it
                
                dTextStrings[lineIdx] = &dTextStrings[0][charIdx]; //store address of new line
                lineIdx++;
            }            
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

        //Print the text lines
        for (lineIdx = 0; lineIdx < 4; lineIdx++) {
            if (dTextStrings[lineIdx] == NULL) {
                break;
            }
            font_window_add_string_xy(3, -0x8000, y, dTextStrings[lineIdx], 1, ALIGN_TOP_CENTER);
            font_window_set_text_colour(3, 20, 20, 20, 255, 255);
            font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
            y += 16;
        }

        font_window_draw(&dl, mtxs, vtxs, 3);
    }

    *gdl = dl;
}

// offset: 0x5608 | func: 38
static void cmdmenu_update_stats(void) {
    Object* player;
    Object* sidekick;
    CmdmenuPlayerSidekickData stats;
    f32 temp_fv0;
    u8 var_s2;
    u8 i;

    player = get_player();
    sidekick = get_sidekick();
    var_s2 = 0;
    stats.playerHealth = ((DLL_210_Player*)player->dll)->vtbl->get_health(player);
    stats.playerHealthMax = ((DLL_210_Player*)player->dll)->vtbl->get_health_max(player);
    if (sidekick != NULL) {
        stats.sidekickBlueFood = ((DLL_ISidekick*)sidekick->dll)->vtbl->get_blue_food_count(sidekick);
        stats.sidekickRedFood = ((DLL_ISidekick*)sidekick->dll)->vtbl->get_red_food_count(sidekick);
        stats.sidekickMaxFood = 8;
    } else {
        stats.sidekickBlueFood = 0;
        stats.sidekickRedFood = 0;
        stats.sidekickMaxFood = 0;
    }
    stats.playerMagic = ((DLL_210_Player*)player->dll)->vtbl->get_magic(player);
    stats.playerMagicMax = ((DLL_210_Player*)player->dll)->vtbl->get_magic_max(player);
    stats.playerScarabCount = ((DLL_210_Player*)player->dll)->vtbl->get_scarabs(player);
    if (((DLL_210_Player*)player->dll)->vtbl->func21(player) != 0) {
        var_s2 = 7;
    }
    if (_bss_8B < var_s2) {
        _bss_8B = var_s2;
    }
    if (sJoyPressedButtons & R_TRIG) {
        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_5EA_Cmdmenu_ShowHUD, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if ((sJoyHeldButtons & R_TRIG) || (gDLL_2_Camera->vtbl->get_target_object() != NULL) || ((sForceStatsDisplay != 0) && (camera_get_letterbox() == 0))) {
        stats.unk14 = sPrevStats.unk14 + 1;
    } else {
        stats.unk14 = sPrevStats.unk14;
    }
    if (sJoyHeldButtons & R_TRIG) {
        sOpacityR += 8.5f * gUpdateRateF;
        if (sOpacityR > 255.0f) {
            sOpacityR = 255.0f;
        }
    } else {
        sOpacityR -= 8.5f * gUpdateRateF;
        if (sOpacityR < 0.0f) {
            sOpacityR = 0.0f;
        }
    }

    sOpacityR = sOpacityR < sOpacityHealth ? sOpacityR : sOpacityHealth;
    stats.unk18 = 0;
    if (_bss_88 & 1) {
        _bss_88 &= ~1;
        for (i = 0; i < 10; i++) {
            sStats.items[i] = stats.items[i];
            sPrevStats.items[i] = stats.items[i];
            sStatsChangeTimers.items[i] = -30.0f;
        }
        sOpacityHealth = 0.0f;
        return;
    }

    for (i = 0; i < 10; i++) {
        temp_fv0 = sStatsChangeTimers.items[i];
        sStatsChangeTimers.items[i] = temp_fv0 - gUpdateRateF;
        if (temp_fv0 > 60.0f) {
            if (sStatsChangeTimers.items[i] <= 60.0f) {
                if (sStats.items[i] < stats.items[i]) {
                    if (_data_B8[i][0] != -1) {
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, _data_B8[i][0], MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                } else {
                    if (_data_B8[i][1] != -1) {
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, _data_B8[i][1], MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                }
                sStats.items[i] = stats.items[i];
            }
        }
        if (stats.items[i] != sPrevStats.items[i]) {
            sPrevStats.items[i] = stats.items[i];
            if (sStatsChangeTimers.items[i] <= 60.0f) {
                sStatsChangeTimers.items[i] = 90.0f - gUpdateRateF;
            }
        }
        if (sStatsChangeTimers.items[i] < -30.0f) {
            sStatsChangeTimers.items[i] = -30.0f;
        }
    }
}

// offset: 0x5BBC | func: 39
/** 
  * Draws the player's character icon, health, magic, and Scarab counter
  */
static void cmdmenu_draw_player_stats(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    f32 goalOpacity;
    u32 pad;
    u8 i;
    u8 statsOpacity;
    s8 offsetX;
    s8 offsetY;
    s32 temp;
    Gfx* dl;
    Object* player = get_player();
    u8 texIdx;
    char playerScarabCountText[4] = "   ";

    dl = *gdl;
    temp = vi_get_current_size();
    gDPSetScissor(dl++, G_SC_NON_INTERLACE, 0, 0, (u16)GET_VIDEO_WIDTH(temp) - 1, 239);
    
    //Draw player health
    {
        if ((sStatsChangeTimers.playerHealth >= 0.0f) || (sStatsChangeTimers.playerHealthMax >= 0.0f) || (sStatsChangeTimers.unk14 >= 0.0f)) {
            goalOpacity = MAX_OPACITY_F;
        } else {
            goalOpacity = 0.0f;
        }

        if (sOpacityHealth < goalOpacity) {
            sOpacityHealth += 8.5f * gUpdateRateF;
            if (sOpacityHealth > MAX_OPACITY_F) {
                sOpacityHealth = MAX_OPACITY_F;
            }
        } else if (goalOpacity < sOpacityHealth) {
            sOpacityHealth -= 8.5f * gUpdateRateF;
            if (sOpacityHealth < 0.0f) {
                sOpacityHealth = 0.0f;
            }
        }
        
        statsOpacity = sOpacityHealth;
        if (statsOpacity) {
            for (i = 0; i < (sStats.playerHealthMax >> 2); i++) {
                s32 appleCount;

                //Get apple coords (3 rows)
                if (i >= APPLES_ROW_3_IDX) {
                    //Row 3
                    offsetX = (i * APPLES_SPACING_X) - (APPLES_ROW_3_IDX * APPLES_SPACING_X);
                    offsetY = 2 * APPLES_SPACING_Y;
                } else if (i >= APPLES_ROW_2_IDX) {
                    //Row 2
                    offsetX = (i * APPLES_SPACING_X) - (-5 + (APPLES_ROW_2_IDX * APPLES_SPACING_Y));
                    offsetY = APPLES_SPACING_Y;
                } else {
                    //Row 1
                    offsetX = i * APPLES_SPACING_X;
                    offsetY = 0;
                }

                //Pick which texture to use for this apple
                appleCount = sStats.playerHealth >> 2;
                if (i < appleCount) {
                    //Full apple
                    texIdx = CMDMENU_TEX_17_Apple_100_Pct;
                } else if (appleCount < i) {
                    //Empty apple
                    texIdx = CMDMENU_TEX_08_Apple_0_Pct;
                } else {
                    //Portion of an apple
                    texIdx = CMDMENU_TEX_08_Apple_0_Pct + (sStats.playerHealth & 3);
                }
                
                rcp_tile_write(
                    &dl,
                    sTextureTiles[texIdx], 
                    POS_HEALTH_X + offsetX, 
                    POS_HEALTH_Y + offsetY,
                    0xFF,0xFF, 0xFF, statsOpacity
                );
            }
        }
    }

    //Draw player magic
    {
        //Get opacity goal
        if ((sStatsChangeTimers.playerMagic >= 0.0f) || 
            (sStatsChangeTimers.unk14 >= 0.0f) || 
            (((DLL_210_Player*)player->dll)->vtbl->func50(player) != -1)
        ) {
            goalOpacity = MAX_OPACITY_F;
        } else {
            goalOpacity = 0.0f;
        }

        //Animate magic bar's opacity towards goal opacity
        if (sOpacityMagic < goalOpacity) {
            sOpacityMagic += 8.5f * gUpdateRateF;
            if (sOpacityMagic > MAX_OPACITY_F) {
                sOpacityMagic = MAX_OPACITY_F;
            }
        } else if (goalOpacity < sOpacityMagic) {
            sOpacityMagic -= 8.5f * gUpdateRateF;
            if (sOpacityMagic < 0.0f) {
                sOpacityMagic = 0.0f;
            }
        }

        //Draw magic bar(s)
        statsOpacity = sOpacityMagic;
        if (statsOpacity) {
            for (i = 0; i < (sStats.playerMagicMax / 25); i++) {
                if (i < (sStats.playerMagic / 25)) {
                    temp = 66;
                } else if ((sStats.playerMagic / 25) < i) {
                    temp = 0;
                } else {
                    temp = ((sStats.playerMagic % 25) * 2) + 13;
                }

                rcp_tile_write_x(
                    &dl,
                    sTextureTiles[CMDMENU_TEX_36_MagicBar_Full],
                    23.0f,
                    ((i * 12) + 60),
                    (f32) temp,
                    14.0f,
                    0, 0,
                    1.0f, 1.0f,
                    statsOpacity | ~0xFF,
                    TILE_WRITE_TRANSLUCENT | TILE_WRITE_POINT_FILT
                );

                rcp_tile_write_x(
                    &dl, sTextureTiles[CMDMENU_TEX_35_MagicBar_Empty],
                    (23 + temp),
                    ((i * 12) + 60),
                    (f32) (66 - temp), 
                    14.0f,
                    temp << 5, 0, 
                    1.0f, 1.0f, 
                    statsOpacity | ~0xFF, 
                    TILE_WRITE_TRANSLUCENT | TILE_WRITE_POINT_FILT
                );
            }
        }
    }

    //Draw character icon
    {
        statsOpacity = ((u8)sOpacityHealth < (u8)sOpacityMagic) ? (u8)sOpacityMagic : (u8)sOpacityHealth;
        if (statsOpacity) {
            if (player->id == OBJ_Krystal) {
                offsetX = 0;
                offsetY = 0;
                texIdx = CMDMENU_TEX_53_Krystal;
            } else {
                offsetX = 2;
                offsetY = -1;
                texIdx = CMDMENU_TEX_40_Sabre;
            }

            dInventoryPageIcon = tex_load_deferred(dTextableIDs[texIdx]);

            rcp_screen_full_write(
                &dl,
                dInventoryPageIcon,
                20 + offsetX,
                10 + offsetY,
                0,
                0,
                statsOpacity,
                SCREEN_WRITE_TRANSLUCENT
            );

            tex_free(dInventoryPageIcon);
        }
    }

    //Draw Scarab counter
    {
        if ((sStatsChangeTimers.playerScarabCount >= 0.0f) || (sStatsChangeTimers.unk14 >= 0.0f)) {
            goalOpacity = MAX_OPACITY_F;
        } else {
            goalOpacity = 0.0f;
        }

        if (sOpacityScarabs < goalOpacity) {
            sOpacityScarabs += 8.5f * gUpdateRateF;
            if (sOpacityScarabs > MAX_OPACITY_F) {
                sOpacityScarabs = MAX_OPACITY_F;
            }
        } else if (goalOpacity < sOpacityScarabs) {
            sOpacityScarabs -= 8.5f * gUpdateRateF;
            if (sOpacityScarabs < 0.0f) {
                sOpacityScarabs = 0.0f;
            }
        }

        statsOpacity = sOpacityScarabs;
        if (statsOpacity && (main_get_bits(BIT_UI_Scarab_Counter_Enabled) != 0)) {
            sAnimFrameScarab = 0;
            if (statsOpacity == MAX_OPACITY) {
                // @fake assignment of i
                if ((i = _bss_8B) != 0) {
                    sAnimFrameScarab = 11 - _bss_8B;
                    _bss_8B--;
                    if (!_bss_8B) {
                        sAnimScarabFlutterTimer = 80;
                    }
                } else {
                    if (sAnimScarabFlutterTimer) {
                        sAnimScarabFlutterTimer--; //@framerate-dependent

                        //Change the Scarab's frame during last few ticks of timer 
                        if (sAnimScarabFlutterTimer < 6) {
                            sAnimFrameScarab = 3 - (sAnimScarabFlutterTimer >> 1);
                        }
                    } else {
                        sAnimScarabFlutterTimer = rand_next(20, 255);
                    }
                }
            }

            rcp_tile_write_x(
                &dl, 
                sTextureTiles[CMDMENU_TEX_18_Scarab + sAnimFrameScarab], 
                POS_SCARABS_X, 
                POS_SCARABS_Y, 
                16, 
                16, 
                0, 0, 
                1.0f, 1.0f, 
                statsOpacity | ~0xFF, 
                TILE_WRITE_TRANSLUCENT | TILE_WRITE_POINT_FILT
            );

            sprintf(playerScarabCountText, "%d", (int)sStats.playerScarabCount);
            font_window_set_coords(3, 0, 0, 320, 240);
            font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
            font_window_set_bg_colour(3, 0, 0, 0, 0);
            font_window_flush_strings(3);
            font_window_set_text_colour(3, 0xFF, 0xFF, 0xFF, 0xFF, statsOpacity);
            font_window_add_string_xy(3, POS_SCARABS_NUMBER_X, POS_SCARABS_NUMBER_Y, playerScarabCountText, 1, ALIGN_TOP_LEFT);
            font_window_set_text_colour(3, 0x14, 0x14, 0x14, 0xFF, 0xFF);
            font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
            font_window_draw(&dl, mtxs, vtxs, 3);
        }
    }

    *gdl = dl;
}

// offset: 0x6984 | func: 40 | export: 16
/**
  * Simulate controller button presses (only for the cmdmenu),
  * used by tutorial sequences like Tricky's sidekick command explanation.
  *
  * An argument of `CMDMENU_CLEAR_BUTTONS_OVERRIDE` (-1) clears the override.
  */
void cmdmenu_set_buttons_override(s32 buttonsOverride) {
    if (buttonsOverride == CMDMENU_CLEAR_BUTTONS_OVERRIDE) {
        sJoyPressedButtonsOverride = 0;
        sShouldOverrideJoypadButtons = FALSE;
    } else {
        sJoyPressedButtonsOverride = buttonsOverride;
        sShouldOverrideJoypadButtons = TRUE;
    }
}

// offset: 0x69CC | func: 41
/**
  * Resets/zeroes out the item info pop-up that appears in the 
  * bottom-left of screen after collecting certain items (e.g. Kyte's grubs)
  */
static void cmdmenu_info_hide(CmdmenuInfoShowData* info) {
    info->timer = -1;
    info->count = 0;
    info->texture = NULL;
    info->opacity = 0.0f;
}

// offset: 0x69F8 | func: 42 | export: 17
/**
  * Show an item info pop-up in the bottom-left of screen
  * (used after collecting certain items, e.g. Kyte's grubs)
  *
  * The item is specified using its inventory gamebitID.
  */
void cmdmenu_info_show(s16 itemGamebit, s32 displayDuration, s32 itemCount) {
    InventoryItem *items;
    CmdmenuPage *inventoryPage;

    inventoryPage = dCmdmenuPages;
    STUBBED_PRINTF("qInfoShow\n");
    sInfoPopup.texture = NULL;

    //Find the item's TEXTABLE textureID, using the item's collection gamebitID
    while (inventoryPage->items != NULL) {
        items = inventoryPage->items;
        while (items->gamebitObtained != NO_GAMEBIT) {
            if (itemGamebit == items->gamebitObtained) {
                sInfoPopup.texture = tex_load_deferred(items->textureID);
                break;
            }
            items++;
        }
        inventoryPage++;
    }

    //Assign the rest of the info box's parameters, and set it up to fade in
    if (sInfoPopup.texture != NULL) {
        sInfoPopup.timer = displayDuration;
        sInfoPopup.count = itemCount;
        sInfoPopup.opacity = 0.0f;
    }
}

// offset: 0x6B00 | func: 43 | export: 18
/**
  * Show an item info pop-up in the bottom-left of screen
  * (used after collecting certain items, e.g. Kyte's grubs)
  *
  * The item is specified using its textable textureID.
  */
void cmdmenu_info_show_tex(s16 textureID, s32 displayDuration, s32 itemCount) {   
    STUBBED_PRINTF("qInfoShowTex\n");
    if ((sInfoPopup.texture = tex_load_deferred(textureID))){
        sInfoPopup.timer = displayDuration;
        sInfoPopup.count = itemCount;
        sInfoPopup.opacity = 0.0f;
    }
}

// offset: 0x6B74 | func: 44
/**
  * Displays an item collection pop-up in the bottom-left corner of the screen.
  * 
  * NOTE: The box is styled after an older UI design, and it's only used by certain items (notably Kyte's grubs).
  */
static void cmdmenu_info_draw(Gfx** gdl, CmdmenuInfoShowData* box) {
    //Do nothing after item's timer finished
    if (box->timer < 0) {
        return;
    }

    //Decrement box's timer
    box->timer -= gUpdateRate;
    if (box->timer < 0) {
        tex_free(box->texture);
        box->texture = NULL;
        return;
    }

    //Update box's opacity
    if (box->timer < 30.0f) {
        //Fade out box during last half-second on-screen
        box->opacity = (box->timer * 255.0f) / 30.0f;
    } else {
        //Otherwise, fade the box in if it's not fully visible
        if (box->opacity != 255.0f) {
            box->opacity += 8.5f * (f32) gUpdateRate;
            if (box->opacity > 255.0f) {
                box->opacity = 255.0f;
            }
        }
    }

    //Draw the box's shadow
    rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_52_Page_Torn_Shadow], 
        22, 
        176, 
        0xFF, 
        0xFF, 
        0xFF, 
        (u8) box->opacity);

    //Draw the item's inventory icon (including embedded page background)
    bzero(_bss_C60, sizeof(TextureTile));
    _bss_C60->tex = box->texture;
    _bss_C60[1].tex = NULL;
    rcp_tile_write(gdl, _bss_C60, 
        36, 
        175, 
        0xFF, 
        0xFF, 
        0xFF, 
        (u8) box->opacity);

    //Draw tattered left edge of box (@bug: outdated page design, mismatched with icons' newer BG)
    rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_29_Page_Torn_Left], 
        20, 
        175, 
        0xFF, 
        0xFF, 
        0xFF, 
        (u8) box->opacity);

    //Draw tattered right edge of box (@bug: outdated page design, mismatched with icons' newer BG)
    rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_30_Page_Torn_Right], 
        68, 
        175, 
        0xFF, 
        0xFF, 
        0xFF, 
        (u8) box->opacity);

    //Draw item count
    if (box->count > 1) {
        _bss_C60->tex = sInventoryStackNumbersTex;
        _bss_C60->animProgress = (box->count - 2) << 8;
        rcp_tile_write(gdl, _bss_C60, 
            52, 
            191, 
            0xFF, 
            0xFF, 
            0xFF, 
            (u8) box->opacity);
    }
}

// offset: 0x70A0 | func: 45 | export: 11
void cmdmenu_func_70A0(u8 arg0) {
    _data_88 = arg0;
}

// offset: 0x70C8 | func: 46 | export: 20
void cmdmenu_energy_bar_create(s32 minEnergy, s32 maxEnergy, s32 fullTexID, s32 emptyTexID, s32 scale) {
    EnergyBar *enbar;
    TextureTile *drawtex;

    /* default.dol
    if (sEnergyBar != NULL) {
        STUBBED_PRINTF("WARNING: ENERGY BAR ALREADY CREATED and need killed \n");
    }
    */

    enbar = mmAlloc(sizeof(EnergyBar), ALLOC_TAG_GAME_COL, ALLOC_NAME("cmdmenu : Bar"));
    bzero(enbar, sizeof(EnergyBar));
    enbar->min = minEnergy;
    enbar->max = maxEnergy;
    
    drawtex = &enbar->fullbarTex[0];
    drawtex->tex = tex_load_deferred(fullTexID);
    drawtex->animProgress = 0;
    drawtex->x = 0;
    drawtex->y = 0;
    enbar->fullbarTex[1].tex = NULL;

    drawtex = &enbar->emptybarTex[0];
    drawtex->tex = tex_load_deferred(emptyTexID);
    drawtex->animProgress = 0;
    drawtex->x = 0;
    drawtex->y = 0;
    enbar->emptybarTex[1].tex = NULL;

    enbar->width = enbar->fullbarTex[0].tex->width;
    enbar->height = enbar->fullbarTex[0].tex->height;

    if (scale == 0) {
        scale = 1;
    }
    enbar->scale = scale;

    sEnergyBar = enbar;
    STUBBED_PRINTF(" Width %i Length %i ", enbar->width, enbar->height);
    enbar->alpha = 0;
    enbar->width = enbar->width * enbar->scale;
    enbar->filledWidth = enbar->width;
}

// offset: 0x7208 | func: 47 | export: 21
void cmdmenu_energy_bar_set(s32 energy) {
    u32 filledWidth;
    f32 filledRatio;

    /* default.dol
    if (sEnergyBar == NULL) {
        STUBBED_PRINTF(" WARNING: Energy bar not created ");
    }
    */

    filledRatio = ((f32)(energy - sEnergyBar->min)) / ((f32)(sEnergyBar->max - sEnergyBar->min));
    filledWidth = (s32) (sEnergyBar->width * filledRatio);
    sEnergyBar->filledWidth = filledWidth;
}

// offset: 0x7298 | func: 48
static void cmdmenu_draw_energy_bar(Gfx** gdl) {
    EnergyBar* enbar;

    enbar = sEnergyBar;
    if (enbar == NULL) {
        return;
    }

    if (enbar->fadeout) {
        if (gUpdateRate >= enbar->alpha) {
            enbar->fadeout = 0;
            cmdmenu_energy_bar_free();
            return;
        } else {
            enbar->alpha -= gUpdateRate;
        }
    } else {
        if (enbar->alpha < (MAX_OPACITY - (gUpdateRate * 2))) {
            enbar->alpha += (gUpdateRate * 2);
        } else if (enbar->filledWidth == 0.0f) {
            enbar->alpha = 0;
        } else {
            enbar->alpha = -1;
        }
    }
    
    rcp_tile_write_x(gdl, enbar->fullbarTex, 
        (160 - (enbar->width >> 1)), 
        (230 - enbar->height), 
        enbar->filledWidth, 
        enbar->height, 
        0, 
        0,
        1.0f, 
        1.0f, 
        enbar->alpha | ~0xFF, 
        TILE_WRITE_TRANSLUCENT | TILE_WRITE_POINT_FILT);
    rcp_tile_write_x(gdl, enbar->emptybarTex, 
        (160 - (enbar->width >> 1)) + enbar->filledWidth, 
        (230 - enbar->height), 
        enbar->width - enbar->filledWidth, 
        enbar->height, 
        (enbar->filledWidth * 32.0f), 
        0, 
        1.0f, 
        1.0f, 
        enbar->alpha | ~0xFF, 
        TILE_WRITE_TRANSLUCENT | TILE_WRITE_POINT_FILT);
}

// offset: 0x7550 | func: 49 | export: 22
/**
  * Frees the energy bar (e.g. the fuel gauge at CloudRunner Fortress' Racetrack)
  */
void cmdmenu_energy_bar_free(void) {
    EnergyBar* enbar;

    /* default.dol
    if (sEnergyBar == NULL) {
        STUBBED_PRINTF(" WARNING : cmdmenu Energy bar alreadby freed \n");
    }
    */

    STUBBED_PRINTF(" Killing Bar ");
    enbar = sEnergyBar;
    enbar->alpha = 0;
    tex_free(enbar->fullbarTex[0].tex);
    tex_free(enbar->emptybarTex[0].tex);
    mmFree(sEnergyBar);
    sEnergyBar = NULL;
}

// offset: 0x75CC | func: 50 | export: 23
void cmdmenu_energy_bar_fadeout(void) {
    if (sEnergyBar != NULL) {
        sEnergyBar->fadeout = 1;
    }
    /* default.dol
    else {
        STUBBED_PRINTF(" WARNING : cmdmenu Energy bar alreadby freed \n");
    }
    */
}
