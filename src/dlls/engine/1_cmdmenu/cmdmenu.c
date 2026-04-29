#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/1_cmdmenu.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "game/gametexts.h"
#include "game/gametexts_ui.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/textable.h"
#include "sys/gfx/texture.h"
#include "sys/fonts.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/rcp.h"
#include "sys/vi.h"
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
#define NO_ITEM -1

#define SLOT_OCCUPIED 0
#define SLOT_PADDED 1

/* UI COORD MACROS (TO-DO: move to separate header?) */

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

//Change these to move the UI elements in the top-left of screen (character icon, health, magic)
#define UI_TOP_LEFT_X 0
#define UI_TOP_LEFT_Y 0

//Change these to move the UI elements in the top-right of screen (C-buttons, inventory scroll)
#define UI_TOP_RIGHT_X 0
#define UI_TOP_RIGHT_Y 0

//Change these to move the UI elements in the bottom-left of screen (item info pop-up, minimap)
#define UI_BOTTOM_LEFT_X 0
#define UI_BOTTOM_LEFT_Y 0

//Change these to move the UI elements in the bottom-right of screen (Scarab counter, active Spell/Command)
#define UI_BOTTOM_RIGHT_X 0
#define UI_BOTTOM_RIGHT_Y 0

/* UI TOP-LEFT */

//Character icon
#define CHARACTER_ICON_X 20
#define CHARACTER_ICON_Y 10

//Health
#define HEALTH_ICONS_X 60
#define HEALTH_ICONS_Y 20
#define APPLES_SPACING_X 10
#define APPLES_SPACING_Y 10
#define APPLES_ROW_1 7
#define APPLES_ROW_2 6
#define APPLES_ROW_1_OFFSET_X 0
#define APPLES_ROW_2_OFFSET_X 5
#define APPLES_ROW_3_OFFSET_X 0

#define APPLES_ROW_2_IDX (APPLES_ROW_1)
#define APPLES_ROW_3_IDX (APPLES_ROW_1 + APPLES_ROW_2)

//Magic
#define MAGIC_UNITS_PER_BAR 25
#define MAGIC_BARS_WIDTH 66
#define MAGIC_BARS_HEIGHT 14
#define MAGIC_BARS_X 23
#define MAGIC_BARS_Y 60
#define MAGIC_BARS_SPACING_Y 12
#define MAGIC_BARS_ZERO_POINT_X 13

/* UI TOP-RIGHT */

//Inventory page icon
#define PAGE_ICON_X 261
#define PAGE_ICON_Y 10

//Inventory item icons
#define MENU_ITEM_X 262
#define MENU_ITEM_Y 59
#define MENU_ITEM_WIDTH 32
#define MENU_ITEM_HEIGHT 24
#define MENU_ITEM_QUANTITY_OFFSET_X 14
#define MENU_ITEM_QUANTITY_OFFSET_Y 9

#define MENU_ITEM_QUANTITY_X (MENU_ITEM_X + MENU_ITEM_QUANTITY_OFFSET_X)

//Inventory scroll
#define MENU_HEIGHT_OPEN 72

#define MENU_SCROLL_WIDTH 40
#define MENU_SCROLL_HEIGHT 8

#define MENU_SCROLL_X (MENU_ITEM_X - (MENU_SCROLL_WIDTH - MENU_ITEM_WIDTH)/2)
#define MENU_SCROLL_TOP_Y (MENU_ITEM_Y - MENU_SCROLL_HEIGHT)
#define MENU_SCROLL_BOTTOM_Y (MENU_ITEM_Y)

#define MENU_SCROLL_CENTRE_Y (MENU_ITEM_Y + (MENU_ITEM_HEIGHT + MENU_ITEM_HEIGHT/2)) //Screen Y-coord in the middle of the inventory's 3 tiles

//Inventory item selection highlight
#define ITEM_HL_WIDTH 8
#define ITEM_HL_HEIGHT 6
#define ITEM_HL_MARGIN 4

#define ITEM_HL_X1 (MENU_ITEM_X + ITEM_HL_MARGIN)
#define ITEM_HL_Y1 (MENU_ITEM_Y + (MENU_HEIGHT_OPEN - MENU_ITEM_HEIGHT)/2 - 1)
#define ITEM_HL_X2 (MENU_ITEM_X + MENU_ITEM_WIDTH - ITEM_HL_WIDTH - ITEM_HL_MARGIN)
#define ITEM_HL_Y2 (ITEM_HL_Y1 + MENU_ITEM_HEIGHT - ITEM_HL_MARGIN)

//Sidekick meter
#define SIDEKICK_METER_X 250
#define SIDEKICK_METER_Y 21
#define SIDEKICK_METER_SPACING_X 9
#define SIDEKICK_METER_SPACING_Y 8
#define SIDEKICK_METER_ICONS_PER_COLUMN 4

//C buttons
#define C_BUTTONS_X 245
#define C_BUTTONS_Y 17

#define C_BUTTONS_LEFT_EMPTY_X (C_BUTTONS_X + 1)
#define C_BUTTONS_LEFT_EMPTY_Y (C_BUTTONS_Y + 9)

#define C_BUTTONS_DOWN_EMPTY_X (C_BUTTONS_X + 7)
#define C_BUTTONS_DOWN_EMPTY_Y (C_BUTTONS_Y + 26)

#define C_BUTTONS_RIGHT_EMPTY_X (C_BUTTONS_X + 29)
#define C_BUTTONS_RIGHT_EMPTY_Y (C_BUTTONS_Y + 17)

#define C_BUTTONS_LEFT_DOWN_BOOK_SIDEKICK_X (C_BUTTONS_X + 0)
#define C_BUTTONS_LEFT_DOWN_BOOK_SIDEKICK_Y (C_BUTTONS_Y + 0)

#define C_BUTTONS_RIGHT_BAG_X (C_BUTTONS_X + 30)
#define C_BUTTONS_RIGHT_BAG_Y (C_BUTTONS_Y + 8)

/* UI BOTTOM-LEFT */

//Item info pop-up
#define INFO_POPUP_X 20
#define INFO_POPUP_Y 175
#define INFO_POPUP_EDGE_WIDTH 16

#define INFO_POPUP_L_X (INFO_POPUP_X + 0)
#define INFO_POPUP_M_X (INFO_POPUP_L_X + INFO_POPUP_EDGE_WIDTH)
#define INFO_POPUP_R_X (INFO_POPUP_M_X + MENU_ITEM_WIDTH)
#define INFO_POPUP_SHADOW_X (INFO_POPUP_X + 2)
#define INFO_POPUP_SHADOW_Y (INFO_POPUP_Y + 1)
#define INFO_POPUP_QUANTITY_X (INFO_POPUP_M_X + 16)
#define INFO_POPUP_QUANTITY_Y (INFO_POPUP_Y + 16)

/* UI BOTTOM-RIGHT */

//Scarabs counter
#define SCARABS_ICON_X 252
#define SCARABS_ICON_Y 198
#define SCARABS_ICON_WIDTH 16
#define SCARABS_ICON_HEIGTH 16
#define SCARABS_NUMBER_X (SCARABS_ICON_X + 18)
#define SCARABS_NUMBER_Y (SCARABS_ICON_Y + 4)

//Active Spell
#define ACTIVE_SPELL_X 253
#define ACTIVE_SPELL_Y 169
#define ACTIVE_SPELL_ICON_OFFSET_X 9
#define ACTIVE_SPELL_ICON_OFFSET_Y 11

#define ACTIVE_SPELL_ICON_X (ACTIVE_SPELL_X + ACTIVE_SPELL_ICON_OFFSET_X)
#define ACTIVE_SPELL_ICON_Y (ACTIVE_SPELL_Y + ACTIVE_SPELL_ICON_OFFSET_Y)

//Active Sidekick Command
#define ACTIVE_SIDECOMMAND_X 253
#define ACTIVE_SIDECOMMAND_Y 121
#define ACTIVE_SIDECOMMAND_ICON_OFFSET_X 9
#define ACTIVE_SIDECOMMAND_ICON_OFFSET_Y 11

#define ACTIVE_SIDECOMMAND_ICON_X (ACTIVE_SIDECOMMAND_X + ACTIVE_SIDECOMMAND_ICON_OFFSET_X)
#define ACTIVE_SIDECOMMAND_ICON_Y (ACTIVE_SIDECOMMAND_Y + ACTIVE_SIDECOMMAND_ICON_OFFSET_Y)

/* CENTRED UI */

//Info scroll
#define INFO_SCROLL_WIDTH 120
#define INFO_SCROLL_HEIGHT 50 //When open
#define INFO_SCROLL_TEXT_Y 3 //Top margin for text printed inside the tutorial box
#define INFO_SCROLL_LINE_HEIGHT 16 //Text lines' vertical spacing
#define INFO_SCROLL_X (SCREEN_WIDTH/2)
#define INFO_SCROLL_Y 30
#define INFO_SCROLL_Y_INITIAL (INFO_SCROLL_Y - 10)
#define INFO_SCROLL_OPACITY_MAX 160
#define INFO_SCROLL_OPACITY_SPEED 32

//Info scroll texture dimensions
#define INFO_SCROLL_PAGE_EDGE_WIDTH 16
#define INFO_SCROLL_PAGE_SHADOW_HEIGHT 8
#define INFO_SCROLL_ROLL_HEIGHT 16
#define INFO_SCROLL_ROLL_TOP_Y_OFFSET 11
#define INFO_SCROLL_ROLL_BOTTOM_Y_OFFSET 4 //NOTE: causes 1px gap, may be intentional as dark shadow
#define INFO_SCROLL_HANDLE_WIDTH 16
#define INFO_SCROLL_HANDLE_HEIGHT 16

//Tutorial textbox
#define TUTORIAL_BOX_WIDTH 240
#define TUTORIAL_BOX_HEIGHT 80
#define TUTORIAL_BOX_TEXT_Y 3 //Top margin for text printed inside the tutorial box
#define TUTORIAL_BOX_LINE_HEIGHT 16 //Text lines' vertical spacing
#define TUTORIAL_BOX_X (SCREEN_WIDTH/2)
#define TUTORIAL_BOX_Y 20
#define TUTORIAL_BOX_OPACITY_MAX 160
#define TUTORIAL_BOX_OPACITY_SPEED 8

//Tutorial textbox texture dimensions
#define TUTORIAL_BOX_PAGE_EDGE_WIDTH 16
#define TUTORIAL_BOX_PAGE_SHADOW_HEIGHT 8
#define TUTORIAL_BOX_ROLL_HEIGHT 16
#define TUTORIAL_BOX_ROLL_TOP_Y_OFFSET 11
#define TUTORIAL_BOX_ROLL_BOTTOM_Y_OFFSET 4 //NOTE: causes 1px gap, may be intentional as dark shadow
#define TUTORIAL_BOX_HANDLE_WIDTH 16
#define TUTORIAL_BOX_HANDLE_HEIGHT 16

#define TUTORIAL_BOX_A_BUTTON_WIDTH 24
#define TUTORIAL_BOX_A_BUTTON_HEIGHT 24
#define TUTORIAL_BOX_A_BUTTON_OFFSET_X 8
#define TUTORIAL_BOX_A_BUTTON_OFFSET_Y 0

//Energy bar
#define ENERGY_BAR_X (SCREEN_WIDTH / 2)
#define ENERGY_BAR_Y (SCREEN_HEIGHT - 10)

//Aiming reticle
#define AIMING_RETICLE_WIDTH 32
#define AIMING_RETICLE_HEIGHT 32
#define AIMING_RETICLE_OPACITY 150

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
    CMDMENU_TEX_14_Unk_Circle_Glow = 14,
    CMDMENU_TEX_15_Unk_Circle_Blue = 15,
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
    CMDMENU_TEX_38_LeftDownButtons_SpellBook_With_Kyte = 38,
    CMDMENU_TEX_39_C_Left = 39,
    CMDMENU_TEX_40_Sabre = 40,
    CMDMENU_TEX_41_C_Right = 41,
    CMDMENU_TEX_42_Tricky = 42,
    CMDMENU_TEX_43_LeftDownButtons_SpellBook_With_Tricky = 43,
    CMDMENU_TEX_44_Mushroom_Empty = 44,
    CMDMENU_TEX_45_Mushroom_Red_Full = 45,
    CMDMENU_TEX_46_Mushroom_Red_Half = 46,
    CMDMENU_TEX_47_RightButton_With_Bag = 47,
    CMDMENU_TEX_48_LeftDownButtons_SpellBook_NoSidekick = 48,
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

typedef enum {
    CIcon_FLAG_None = 0,
    CIcon_FLAG_Have_Spells = 1,
    CIcon_FLAG_Have_Items = 2,
    CIcon_FLAG_Have_Sidekick_Commands = 4
} CmdMenuCIconFlags;

typedef enum {
    PlayerStats_FLAG_None = 0,
    PlayerStats_FLAG_Update_Snapshot = 1
} CmdMenuPlayerStatsFlags;

/*0x0*/ static s8 dInventoryShow = FALSE;
/*0x4*/ static s8 sInventoryScrollOffset = 0; //Y offset for the scroll's inner strip, used to smoothly animate moving between inventory items
/*0x8*/ static s8 dInventoryMoveSpeed = 1; //Scroll speed when moving between inventory items (how many times to shift gUpdateRate)
/*0xC*/ static s16 dInventoryUnrollMax = MENU_HEIGHT_OPEN; //The height of the inventory scroll when fully open
/*0x10*/ static s16 dInventoryOpacity = 0;
/*0x14*/ static s16 dOpacitySidekickMeter = 0;
/*0x18*/ static s16 dSelectedItemTextID = NO_GAMETEXT;

/*0x1C*/ static s8 dInfoScrollShow = FALSE;
/*0x20*/ static s16 dInfoScrollWidthHalf = INFO_SCROLL_WIDTH >> 1; //Half-width of the info scroll's page
/*0x24*/ static s16 dInfoScrollUnrollMax = INFO_SCROLL_HEIGHT; //Height of the info scroll's page (when fully open)
/*0x28*/ static s16 dInfoScrollY = INFO_SCROLL_Y_INITIAL; //Screen coord for the info scroll (initialised at slightly higher Y)
/*0x2C*/ static s16 dInfoScrollX = INFO_SCROLL_X; //Screen coord for the centre of the info scroll
/*0x30*/ static s16 dInfoScrollOpacity = 0;

/** 
  * Lines of text for the info scroll 
  */
/*0x34*/ static char* dInfoScrollStrings[] = {
    NULL, NULL, NULL, NULL
};

/** 
  * StringID for the info scroll's gametext
  *
  * (First 255 lines are from `gametext_3`, higher IDs use `gametext_568`) 
  */
/*0x44*/ static s16 dInfoScrollTextID = NO_GAMETEXT;
/*0x48*/ static s8 dTutorialBoxShow = FALSE;
/*0x4C*/ static s16 dTutorialBoxHalfWidth = TUTORIAL_BOX_WIDTH / 2;
/*0x50*/ static s16 dTutorialBoxHeight = TUTORIAL_BOX_HEIGHT;
/*0x54*/ static s16 dTutorialBoxY = TUTORIAL_BOX_Y;
/*0x58*/ static s16 dTutorialBoxX = TUTORIAL_BOX_X;
/*0x5C*/ static s16 dTutorialBoxOpacity = 0; //Controls the tutorial box's opacity
/*0x60*/ static s16 dTutorialBoxTextOpacity = 0; //Controls the tutorial box's text opacity, and the opacity of the A button icon.
/*0x64*/ static s16 _data_64 = -1; //unused
/*0x68*/ static Texture* dInventoryPageIcon = NULL; //Icon in the top-right corner of screen: Bag/SpellBook/Kyte/Tricky (also used for C-right button)

typedef struct {
    f32 unk0;
    s8 unk4;
} CmdMenuUnusedData6C;

/*0x6C*/ static CmdMenuUnusedData6C _data_6C = {
    1.0f, -1
};
/*0x74*/ static s8 sJoyButtonMask = 0;
/*0x78*/ static s16 dInventoryMovesQueued = 0;
/*0x7C*/ static u8 dInventoryIsScrolling = FALSE;
/*0x80*/ static u8 sForceStatsDisplay = FALSE;
/*0x84*/ static s16 dInfoScrollDisabled = FALSE; //Stops the info scroll from appearing (unused)
/*0x88*/ static s8 _data_88 = 1; //Unknown purpose. Foodbag-related? `Foodbag_control` uses `func_70A0` to set this data's value to 0 or 1, but it's otherwise unused.

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

/**
  * Sound effects to play when tracked player stats increase/decrease
  * (unused in practice, all set to NO_SOUND or 0)
  */
/*0xB8*/ static CmdmenuPlayerStatsChangeSounds dStatChangeSounds = {{
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
}};

/** Unused data */
/*0xE0*/ static u16 _data_E0[] = {
    0x0d05, 0x1310, 0x040a, 0x0c16, 0x0416, 0x0c20, 
    0x0d16, 0x1328, 0x140a, 0x1c16, 0x1416, 0x1c20, 
    0x0b00, 0x150a, 0x0204, 0x0c10, 0x0417, 0x0c28, 
    0x0b20, 0x152c, 0x1417, 0x1c28, 0x1404, 0x1e10, 
    0x080e, 0x1812, 0x0812, 0x1818
};

/* 
 * This value ends up unused, but its purpose can be inferred from the info scroll's equivalent variables.
 *
 * Frame counter, intended to animate the top of the inventory scroll rotating (3 frame animation)
 * The texture only has 1 frame though, so this may be a leftover from an earlier UI design.
 *
 * Value increases while info scroll expanding (decreases while collapsing), or while scrolling between items.
 */
/*0x118*/ static s8 dInventoryFrameTop = 0; 

/* 
 * This value ends up unused, but its purpose can be inferred from the info scroll's equivalent variables.
 *
 * Frame counter, intended to animate the bottom of the inventory scroll rotating (3 frame animation)
 * The texture only has 1 frame though, so this may be a leftover from an earlier UI design.
 *
 * Value increases while info scroll collapsing (decreases while expanding), or while scrolling between items.
 */
/*0x11C*/ static s8 dInventoryFrameBottom = 0;

/* 
 * Frame counter, intended to animate the top of the info scroll rotating (3 frame animation)
 * The texture only has 1 frame though, so this may be a leftover from an earlier UI design.
 *
 * Value increases while info scroll expanding (decreases while collapsing), but gets reset to 0.
 *
 * Top rolls in opposite direction to bottom, suggesting info scroll used to expand from the middle 
 * (in the Dec 2000 design it expands from the top down, with the top fixed in place.)
 */
/*0x120*/ static s8 dInfoScrollFrameTop = 0;

/* 
 * Frame counter, intended to animate the bottom of the info scroll rotating (3 frame animation)
 * The texture only has 1 frame though, so this may be a leftover from an earlier UI design.
 *
 * Value increases while info scroll collapsing (decreases while expanding), but gets reset to 0.
 *
 * Bottom rolls in opposite direction to top, suggesting info scroll used to expand from the middle 
 * (in the Dec 2000 design it expands from the top down, with the top fixed in place.)
 */
/*0x124*/ static s8 dInfoScrollFrameBottom = 0;

#define NONE 0xFFFF
#define EXIT -1
#define END {NONE, NONE, NONE, NONE, 0x0000, 0x00, 0x00}

/* Krystal items
 * 0x128*/ static InventoryItem dPage0ItemsKrystal[] = {
    {{BIT_Krystal_Warp_Crystal,   BIT_Krystal_Used_Warp_Crystal,    TEXTABLE_245, NONE,  0x010, EXIT,                                CMDMENU_SOUND_ITEM}}, //Warp Crystal
    {{BIT_CRF_Prison_Key_1,       NONE,                             TEXTABLE_175, NONE,  0x007, EXIT,                                CMDMENU_SOUND_ITEM}}, //Prison Key (CloudRunner Fortress)
    {{BIT_CRF_Power_Room_Key,     NONE,                             TEXTABLE_176, NONE,  0x008, EXIT,                                CMDMENU_SOUND_ITEM}}, //Power Room Key (CloudRunner Fortress)
    {{BIT_CRF_Red_Power_Crystal,  NONE,                             TEXTABLE_180, NONE,  0x009, EXIT,                                CMDMENU_SOUND_ITEM}}, //Red Power Crystal (CloudRunner Fortress)
    {{BIT_CRF_Green_Power_Crystal,NONE,                             TEXTABLE_181, NONE,  0x009, EXIT,                                CMDMENU_SOUND_ITEM}}, //Green Power Crystal (CloudRunner Fortress)
    {{BIT_CRF_Blue_Power_Crystal, NONE,                             TEXTABLE_182, NONE,  0x009, EXIT,                                CMDMENU_SOUND_ITEM}}, //Blue Power Crystal (CloudRunner Fortress)
    {{BIT_IM_Snowbike_Key,        NONE,                             TEXTABLE_175, NONE,  0x011, EXIT,                                CMDMENU_SOUND_ITEM}}, //Snowbike Key (Removed)
    {{BIT_DIM_Mine_Key,           BIT_DIM_Used_Mine_Key,            TEXTABLE_175, NONE,  0x013, EXIT,                                CMDMENU_SOUND_ITEM}}, //Mine Key (unused?)
    {{BIT_DIM_Alpine_Roots,       NONE,                             TEXTABLE_1A3, NONE,  0x00b, EXIT,                                CMDMENU_SOUND_ITEM}}, //Alpine Root (unused?)
    {{BIT_DIM_Gear_1,             BIT_DIM_Used_Gear_1,              TEXTABLE_479, NONE,  0x00b, EXIT,                                CMDMENU_SOUND_ITEM}}, //DIM Gear 1
    {{BIT_DIM_Gear_2,            BIT_DIM_Used_Gear_2,              TEXTABLE_479,  NONE,  0x00b, EXIT,                                CMDMENU_SOUND_ITEM}}, //DIM Gear 2
    {{BIT_DIM_Gear_3,            BIT_DIM_Used_Gear_3,              TEXTABLE_479,  NONE,  0x00b, EXIT,                                CMDMENU_SOUND_ITEM}}, //DIM Gear 3
    {{BIT_DIM_Gear_4,            BIT_DIM_Used_Gear_4,              TEXTABLE_479,  NONE,  0x00b, EXIT,                                CMDMENU_SOUND_ITEM}}, //DIM Gear 4
    {{BIT_Krystal_Foodbag_S,     BIT_Krystal_Foodbag_M,            TEXTABLE_27A,  NONE,  0x016, CMDMENU_PAGE_2_Food_Actions_Krystal, CMDMENU_SOUND_PAGE}}, //Small Food Bag
    {{BIT_Krystal_Foodbag_M,     BIT_Krystal_Foodbag_L,            TEXTABLE_244,  NONE,  0x017, CMDMENU_PAGE_2_Food_Actions_Krystal, CMDMENU_SOUND_PAGE}}, //Medium Food Bag
    {{BIT_Krystal_Foodbag_L,     NONE,                             TEXTABLE_2BF,  NONE,  0x018, CMDMENU_PAGE_2_Food_Actions_Krystal, CMDMENU_SOUND_PAGE}}, //Large Food Bag
    {{BIT_Krystal_Dino_Bag_S,    BIT_Krystal_Dino_Bag_M,           TEXTABLE_471,  NONE,  0x02f, CMDMENU_PAGE_9_Food_Actions_Kyte,    CMDMENU_SOUND_PAGE}}, //Small Grub Bag
    {{BIT_Krystal_Dino_Bag_M,    BIT_Krystal_Dino_Bag_L,           TEXTABLE_560,  NONE,  0x030, CMDMENU_PAGE_9_Food_Actions_Kyte,    CMDMENU_SOUND_PAGE}}, //Medium Grub Bag
    {{BIT_Krystal_Dino_Bag_L,    NONE,                             TEXTABLE_560,  NONE,  0x031, CMDMENU_PAGE_9_Food_Actions_Kyte,    CMDMENU_SOUND_PAGE}}, //Large Grub Bag
    {{BIT_Gold_Nugget_GP,        BIT_CC_Bribed_GuardClaw,          TEXTABLE_257,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Gold Nugget #1 (Near Golden Plains)
    {{BIT_Gold_Nugget_LFV,       BIT_CC_Rescued_Kyte,              TEXTABLE_257,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Gold Nugget #2 (Near LightFoot Village)
    {{BIT_CC_Fire_Crystal,       NONE,                             TEXTABLE_4F7,  NONE,  0x102, EXIT,                                CMDMENU_SOUND_ITEM}}, //Fire Crystal
    {{BIT_CC_Cell_Door_Key,      BIT_CC_Rescued_Kyte,              TEXTABLE_175,  NONE,  0x01e, EXIT,                                CMDMENU_SOUND_ITEM}}, //Cell Door Key (Cape Claw?)
    {{BIT_SpellStone_CRF,        NONE,                             TEXTABLE_562,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}}, //SpellStone - CloudRunner Fortress (Inactive)
    {{BIT_SpellStone_BWC,        NONE,                             TEXTABLE_562,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}}, //SpellStone - BlackWater Canyon (Inactive)
    {{BIT_SpellStone_KP,         NONE,                             TEXTABLE_562,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}}, //SpellStone - Krazoa Palace (Inactive)
    {{BIT_Krazoa_Translator,     NONE,                             TEXTABLE_496,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Krazoa Translator
    {{BIT_Horn_of_Truth,         NONE,                             TEXTABLE_258,  NONE,  0x01d, EXIT,                                CMDMENU_SOUND_ITEM}}, //Horn of Truth
    {{BIT_CRF_Treasure_Chest_Key,NONE,                             TEXTABLE_568,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Treasure Chest Key (CloudRunner Fortress)
    {{BIT_CloudRunner_Grubs,     NONE,                             TEXTABLE_1A5,  NONE,  0x02a, EXIT,                                CMDMENU_SOUND_ITEM}}, //Blue Mushroom (CloudRunner Grubs?)
    {{BIT_Krystal_Fireflies,     NONE,                             TEXTABLE_46D,  NONE,  0x03d, EXIT,                                CMDMENU_SOUND_ITEM}}, //Firefly Lantern
    {{BIT_CRF_Prison_Key_2,      NONE,                             TEXTABLE_175,  NONE,  0x007, EXIT,                                CMDMENU_SOUND_ITEM}}, //Prison Key (CloudRunner Fortress)
    {{BIT_CC_Krazoa_Tablets,     NONE,                             TEXTABLE_496,  NONE,  0x05b, EXIT,                                CMDMENU_SOUND_ITEM}}, //Krazoa Tablets
    {{BIT_Inventory_MoonSeeds,   NONE,                             TEXTABLE_40D,  NONE,  0x05e, EXIT,                                CMDMENU_SOUND_ITEM}}, //MoonSeeds
    {{BIT_CC_Engineers_Key,      NONE,                             TEXTABLE_175,  NONE,  0x064, EXIT,                                CMDMENU_SOUND_ITEM}}, //Construction Engineer's Key (CloudRunner Fortress)
    {{BIT_Gold_Nugget_CC,        BIT_CC_Bribed_GuardClaw,          TEXTABLE_257,  NONE,  0x103, EXIT,                                CMDMENU_SOUND_ITEM}}, //Gold Nugget #3 (Cape Claw)
    {END}
};

/* Sabre items
 * 0x2E4*/ static InventoryItem dPage1ItemsSabre[] = {
    {{BIT_SP_Krazoa_Translator,       NONE,                         TEXTABLE_175, NONE,  0x0a,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Gate Key (Northern Wastes) (Unused?)
    {{BIT_SW_Alpine_Roots,            NONE,                         TEXTABLE_1A3, NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Alpine Root (Geyser area)
    {{BIT_Inventory_Blue_Mushrooms,   NONE,                         TEXTABLE_1A5, NONE,  0x0c,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Blue Mushroom
    {{BIT_Inventory_Purple_Mushrooms, NONE,                         TEXTABLE_498, NONE,  0x57,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Purple Mushroom
    {{BIT_Inventory_White_Mushrooms,  NONE,                         TEXTABLE_497, NONE,  0x56,  EXIT,                                CMDMENU_SOUND_ITEM}}, //White Mushroom
    {{BIT_DIM_Mine_Key,               BIT_DIM_Used_Mine_Key,        TEXTABLE_175, NONE,  0x13,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Mine Key
    {{BIT_DIM_Alpine_Roots,           NONE,                         TEXTABLE_1A3, NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Alpine Root (DIM)
    {{BIT_DIM_Gear_1,                 BIT_DIM_Used_Gear_1,          TEXTABLE_479, NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}}, //DIM Gear 1 
    {{BIT_DIM_Gear_2,                 BIT_DIM_Used_Gear_2,          TEXTABLE_479, NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}}, //DIM Gear 2
    {{BIT_DIM_Gear_3,                 BIT_DIM_Used_Gear_3,          TEXTABLE_479, NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}}, //DIM Gear 3
    {{BIT_DIM_Gear_4,                BIT_DIM_Used_Gear_4,          TEXTABLE_479,  NONE,  0x0b,  EXIT,                                CMDMENU_SOUND_ITEM}}, //DIM Gear 4
    {{BIT_Belina_Te_Cell_Key,        BIT_Used_Belina_Te_Cell_Key,  TEXTABLE_175,  NONE,  0x19,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Belina Te Cell Key (DarkIce Mines)
    {{BIT_Tricky_Cell_Key,           BIT_Used_Tricky_Cell_Key,     TEXTABLE_175,  NONE,  0x19,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Tricky Cell Key (DarkIce Mines)
    {{BIT_Sabre_Warp_Crystal,        BIT_Sabre_Used_Warp_Crystal,  TEXTABLE_245,  NONE,  0x10,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Warp Crystal
    {{BIT_DIM_Door_Key_1,            BIT_DIM_Used_Door_Key_1,      TEXTABLE_175,  NONE,  0x19,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Door Key 1 (DarkIce Mines)
    {{BIT_DIM_Door_Key_2,            BIT_DIM_Used_Door_Key_2,      TEXTABLE_175,  NONE,  0x19,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Door Key 2 (DarkIce Mines)
    {{BIT_Sabre_Foodbag_S,           BIT_Sabre_Foodbag_M,          TEXTABLE_27A,  NONE,  0x16,  CMDMENU_PAGE_3_Food_Actions_Sabre,   CMDMENU_SOUND_PAGE}}, //Small Food Bag
    {{BIT_Sabre_Foodbag_M,           BIT_Sabre_Foodbag_L,          TEXTABLE_244,  NONE,  0x17,  CMDMENU_PAGE_3_Food_Actions_Sabre,   CMDMENU_SOUND_PAGE}}, //Medium Food Bag
    {{BIT_Sabre_Foodbag_L,           NONE,                         TEXTABLE_2BF,  NONE,  0x18,  CMDMENU_PAGE_3_Food_Actions_Sabre,   CMDMENU_SOUND_PAGE}}, //Large Food Bag
    {{BIT_Sabre_Dino_Bag_S,          BIT_Sabre_Dino_Bag_M,         TEXTABLE_471,  NONE,  0x32,  CMDMENU_PAGE_10_Food_Actions_Tricky, CMDMENU_SOUND_PAGE}}, //Small Dinosaur Food Bag
    {{BIT_Sabre_Dino_Bag_M,          BIT_Sabre_Dino_Bag_L,         TEXTABLE_560,  NONE,  0x33,  CMDMENU_PAGE_10_Food_Actions_Tricky, CMDMENU_SOUND_PAGE}}, //Medium Dinosaur Food Bag
    {{BIT_Sabre_Dino_Bag_L,          NONE,                         TEXTABLE_560,  NONE,  0x34,  CMDMENU_PAGE_10_Food_Actions_Tricky, CMDMENU_SOUND_PAGE}}, //Large Dinosaur Food Bag
    {{BIT_Inventory_Corrupt_Item,    NONE,                         TEXTABLE_175,  NONE,  0x1d,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Corrupted? A key with "ForceField Spell (Cape Claw)" as its description
    {{BIT_Horn_of_Truth,             NONE,                         TEXTABLE_258,  NONE,  0x1d,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Horn of Truth
    {{BIT_Sabre_Fireflies,           NONE,                         TEXTABLE_46D,  NONE,  0x3d,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Firefly Lantern
    {{BIT_WC_Silver_Tooth,           BIT_WC_Used_Silver_Tooth,     TEXTABLE_52B,  NONE,  0x00,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Silver Tooth (Walled City)
    {{BIT_WC_Gold_Tooth,             BIT_WC_Used_Gold_Tooth,       TEXTABLE_52A,  NONE,  0x00,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Gold Tooth (Walled City)
    {{BIT_WC_Sun_Stone,              BIT_WC_Used_Sun_Stone,        TEXTABLE_3A8,  NONE,  0x1b,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Sun Stone (Walled City)
    {{BIT_WC_Moon_Stone,             BIT_WC_Used_Moon_Stone,       TEXTABLE_3D6,  NONE,  0x1c,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Moon Stone (Walled City)
    {{BIT_Inventory_MoonSeeds,       NONE,                         TEXTABLE_40D,  NONE,  0x62,  EXIT,                                CMDMENU_SOUND_ITEM}}, //MoonSeeds
    {{BIT_SpellStone_DIM,            BIT_SpellStone_DIM_Activated, TEXTABLE_562,  NONE,  0x60,  EXIT,                                CMDMENU_SOUND_ITEM}}, //SpellStone - DarkIce Mines (Inactive)
    {{BIT_SpellStone_DIM_Activated,  NONE,                         TEXTABLE_563,  NONE,  0x61,  EXIT,                                CMDMENU_SOUND_ITEM}}, //SpellStone - DarkIce Mines (Activated) 
    {{BIT_SpellStone_WC,             NONE,                         TEXTABLE_180,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Spellstone - Walled City (Inactive) (Incorrect icon: Red Power Crystal, CRF)
    {{BIT_SpellStone_DR,             NONE,                         TEXTABLE_180,  NONE,  NONE,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Spellstone - Dragon Rock (Inactive) (Incorrect icon: Red Power Crystal, CRF)
    {{BIT_DB_PointBack_Egg,          NONE,                         TEXTABLE_55E,  NONE,  0x5e,  EXIT,                                CMDMENU_SOUND_ITEM}}, //PointBack Egg (Diamond Bay)
    {{BIT_DB_Bay_Diamond,            NONE,                         TEXTABLE_55F,  NONE,  0x5f,  EXIT,                                CMDMENU_SOUND_ITEM}}, //Bay Diamond
    {END}
};

/* Food Bag actions (Krystal)
 * 0x4A0*/ static InventoryItem dPage2FoodActionsKrystal[] = {
    {{BIT_Foodbag_Eat,               NONE, TEXTABLE_2C7, NONE, 0x1f, CMDMENU_PAGE_4_Food_Krystal,            CMDMENU_SOUND_PAGE}}, //Eat food in foodbag
    {{BIT_Foodbag_Place,             NONE, TEXTABLE_2C8, NONE, 0x20, CMDMENU_PAGE_4_Food_Krystal,            CMDMENU_SOUND_PAGE}}, //Place food down from foodbag
    {{BIT_Foodbag_Give,              NONE, TEXTABLE_2EF, NONE, 0x21, CMDMENU_PAGE_4_Food_Krystal,            CMDMENU_SOUND_PAGE}}, //Give food from foodbag
    {{BIT_Foodbag_Setting_Eat_First, NONE, TEXTABLE_265, NONE, 0x39, CMDMENU_PAGE_2_Food_Actions_Krystal,    CMDMENU_SOUND_PAGE}}, //Eat later (NOTE: option to switch shown when "Eat First" enabled)
    {{BIT_Foodbag_Setting_Eat_Later, NONE, TEXTABLE_529, NONE, 0x3a, CMDMENU_PAGE_2_Food_Actions_Krystal,    CMDMENU_SOUND_PAGE}}, //Eat first (NOTE: option to switch shown when "Eat Later" enabled)
    {END}
};

/* Food Bag actions (Sabre)
 * 0x4E8*/ static InventoryItem dPage3FoodActionsSabre[] = {
    {{BIT_Foodbag_Eat,               NONE, TEXTABLE_2C7, NONE, 0x1f, CMDMENU_PAGE_5_Food_Sabre,              CMDMENU_SOUND_PAGE}}, //Eat food in foodbag
    {{BIT_Foodbag_Place,             NONE, TEXTABLE_2C8, NONE, 0x20, CMDMENU_PAGE_5_Food_Sabre,              CMDMENU_SOUND_PAGE}}, //Place food down from foodbag
    {{BIT_Foodbag_Give,              NONE, TEXTABLE_2EF, NONE, 0x21, CMDMENU_PAGE_5_Food_Sabre,              CMDMENU_SOUND_PAGE}}, //Give food from foodbag
    {{BIT_Foodbag_Setting_Eat_First, NONE, TEXTABLE_265, NONE, 0x39, CMDMENU_PAGE_3_Food_Actions_Sabre,      CMDMENU_SOUND_PAGE}}, //Eat later (NOTE: option to switch shown when "Eat First" enabled)
    {{BIT_Foodbag_Setting_Eat_Later, NONE, TEXTABLE_529, NONE, 0x3a, CMDMENU_PAGE_3_Food_Actions_Sabre,      CMDMENU_SOUND_PAGE}}, //Eat first (NOTE: option to switch shown when "Eat Later" enabled)
    {END}
};

/* Food Bag items (Krystal)
 * 0x530*/ static InventoryItem dPage4FoodItemsKrystal[] = {
    {{BIT_Green_Apple_Count,  NONE,                 TEXTABLE_26E,  NONE, GAMETEXT_UI_22_Green_Apple,     EXIT,                                 CMDMENU_SOUND_ITEM}}, //Green apple
    {{BIT_Red_Apple_Count,    NONE,                 TEXTABLE_26F,  NONE, GAMETEXT_UI_23_Red_Apple,       EXIT,                                 CMDMENU_SOUND_ITEM}}, //Red apple
    {{BIT_Brown_Apple_Count,  NONE,                 TEXTABLE_270,  NONE, GAMETEXT_UI_24_Brown_Apple,     EXIT,                                 CMDMENU_SOUND_ITEM}}, //Brown Apple
    {{BIT_Fish_Count,         NONE,                 TEXTABLE_272,  NONE, GAMETEXT_UI_25_Fish,            EXIT,                                 CMDMENU_SOUND_ITEM}}, //Fish
    {{BIT_Smoked_Fish_Count,  NONE,                 TEXTABLE_2C6,  NONE, GAMETEXT_UI_26_Smoked_Fish,     EXIT,                                 CMDMENU_SOUND_ITEM}}, //Smoked Fish
    {{BIT_Dino_Egg_Count,     NONE,                 TEXTABLE_2C4,  NONE, GAMETEXT_UI_27_Dino_Egg,        EXIT,                                 CMDMENU_SOUND_ITEM}}, //Dino Egg
    {{BIT_Moldy_Meat_Count,   NONE,                 TEXTABLE_2C5,  NONE, GAMETEXT_UI_28_Moldy_Meat,      EXIT,                                 CMDMENU_SOUND_ITEM}}, //Moldy Meat
    {{BIT_Green_Bean_Count,   NONE,                 TEXTABLE_270,  NONE, GAMETEXT_UI_35_Green_Bean,      EXIT,                                 CMDMENU_SOUND_ITEM}}, //Green Bean
    {{BIT_Red_Bean_Count,     NONE,                 TEXTABLE_270,  NONE, GAMETEXT_UI_36_Red_Bean,        EXIT,                                 CMDMENU_SOUND_ITEM}}, //Red Bean
    {{BIT_Brown_Bean_Count,   NONE,                 TEXTABLE_270,  NONE, GAMETEXT_UI_37_Brown_Bean,      EXIT,                                 CMDMENU_SOUND_ITEM}}, //Brown Bean
    {{BIT_Blue_Bean_Count,   NONE,                  TEXTABLE_270,  NONE, GAMETEXT_UI_38_Blue_Bean,       EXIT,                                 CMDMENU_SOUND_ITEM}}, //Blue Bean
    {{BIT_Krystal_Foodbag_S, BIT_Krystal_Foodbag_M, TEXTABLE_27A,  NONE, GAMETEXT_UI_16_Small_Food_Bag,  CMDMENU_PAGE_2_Food_Actions_Krystal,  CMDMENU_SOUND_PAGE}}, //Small Food Bag
    {{BIT_Krystal_Foodbag_M, BIT_Krystal_Foodbag_L, TEXTABLE_244,  NONE, GAMETEXT_UI_17_Medium_Food_Bag, CMDMENU_PAGE_2_Food_Actions_Krystal,  CMDMENU_SOUND_PAGE}}, //Medium Food Bag
    {{BIT_Krystal_Foodbag_L, NONE,                  TEXTABLE_2BF,  NONE, GAMETEXT_UI_18_Large_Food_Bag,  CMDMENU_PAGE_2_Food_Actions_Krystal,  CMDMENU_SOUND_PAGE}}, //Large Food Bag
    {END}
};

/* Food Bag items (Sabre)
 * 0x5E4*/ static InventoryItem dPage5FoodItemsSabre[] = {
    {{BIT_Green_Apple_Count,  NONE,                 TEXTABLE_26E,  NONE, GAMETEXT_UI_22_Green_Apple,     EXIT,                                 CMDMENU_SOUND_ITEM}}, //Green apple
    {{BIT_Red_Apple_Count,    NONE,                 TEXTABLE_26F,  NONE, GAMETEXT_UI_23_Red_Apple,       EXIT,                                 CMDMENU_SOUND_ITEM}}, //Red apple
    {{BIT_Brown_Apple_Count,  NONE,                 TEXTABLE_270,  NONE, GAMETEXT_UI_24_Brown_Apple,     EXIT,                                 CMDMENU_SOUND_ITEM}}, //Brown Apple
    {{BIT_Fish_Count,         NONE,                 TEXTABLE_272,  NONE, GAMETEXT_UI_25_Fish,            EXIT,                                 CMDMENU_SOUND_ITEM}}, //Fish
    {{BIT_Smoked_Fish_Count,  NONE,                 TEXTABLE_2C6,  NONE, GAMETEXT_UI_26_Smoked_Fish,     EXIT,                                 CMDMENU_SOUND_ITEM}}, //Smoked Fish
    {{BIT_Dino_Egg_Count,     NONE,                 TEXTABLE_2C4,  NONE, GAMETEXT_UI_27_Dino_Egg,        EXIT,                                 CMDMENU_SOUND_ITEM}}, //Dino Egg
    {{BIT_Moldy_Meat_Count,   NONE,                 TEXTABLE_2C5,  NONE, GAMETEXT_UI_28_Moldy_Meat,      EXIT,                                 CMDMENU_SOUND_ITEM}}, //Moldy Meat
    {{BIT_Green_Bean_Count,   NONE,                 TEXTABLE_270,  NONE, GAMETEXT_UI_35_Green_Bean,      EXIT,                                 CMDMENU_SOUND_ITEM}}, //Green Bean 
    {{BIT_Red_Bean_Count,     NONE,                 TEXTABLE_270,  NONE, GAMETEXT_UI_36_Red_Bean,        EXIT,                                 CMDMENU_SOUND_ITEM}}, //Red Bean
    {{BIT_Brown_Bean_Count,   NONE,                 TEXTABLE_270,  NONE, GAMETEXT_UI_37_Brown_Bean,      EXIT,                                 CMDMENU_SOUND_ITEM}}, //Brown Bean
    {{BIT_Blue_Bean_Count,   NONE,                  TEXTABLE_270,  NONE, GAMETEXT_UI_38_Blue_Bean,       EXIT,                                 CMDMENU_SOUND_ITEM}}, //Blue Bean
    {{BIT_Sabre_Foodbag_S,   BIT_Sabre_Foodbag_M,   TEXTABLE_27A,  NONE, GAMETEXT_UI_16_Small_Food_Bag,  CMDMENU_PAGE_3_Food_Actions_Sabre,    CMDMENU_SOUND_PAGE}}, //Small Food Bag
    {{BIT_Sabre_Foodbag_M,   BIT_Sabre_Foodbag_L,   TEXTABLE_244,  NONE, GAMETEXT_UI_17_Medium_Food_Bag, CMDMENU_PAGE_3_Food_Actions_Sabre,    CMDMENU_SOUND_PAGE}}, //Medium Food Bag
    {{BIT_Sabre_Foodbag_L,   NONE,                  TEXTABLE_2BF,  NONE, GAMETEXT_UI_18_Large_Food_Bag,  CMDMENU_PAGE_3_Food_Actions_Sabre,    CMDMENU_SOUND_PAGE}}, //Large Food Bag
    {END}
};

/* Magic Spells
 * 0x698*/ static InventoryItem dPage6MagicSpells[] = {
    {{BIT_Spell_Projectile, NONE,    TEXTABLE_183, NONE, GAMETEXT_UI_D_Projectile_Spell, EXIT, CMDMENU_SOUND_NONE}}, //Projectile Spell
    {{BIT_Spell_Ice_Blast,  NONE,    TEXTABLE_468, NONE, GAMETEXT_UI_3C_Ice_Blast_Spell, EXIT, CMDMENU_SOUND_NONE}}, //Ice Blast Spell
    {{BIT_Spell_Grenade,    NONE,    TEXTABLE_4F8, NONE, GAMETEXT_UI_F_Wizard_Randorn,   EXIT, CMDMENU_SOUND_NONE}}, //Grenade Spell (Incorrectly labelled as Randorn)
    {{BIT_Spell_Illusion,   NONE,    TEXTABLE_177, NONE, GAMETEXT_UI_E_Illusion_Spell,   EXIT, CMDMENU_SOUND_NONE}}, //Illusion Spell
    {{BIT_Spell_Forcefield, NONE,    TEXTABLE_265, NONE, GAMETEXT_UI_14_Shield,          EXIT, CMDMENU_SOUND_NONE}}, //ForceField Spell
    {{BIT_Spell_Glitched_1, NONE,    TEXTABLE_183, NONE, GAMETEXT_UI_D_Projectile_Spell, EXIT, CMDMENU_SOUND_NONE}}, //Glitched Spell
    {{BIT_Spell_Glitched_2, BIT_348, TEXTABLE_529, NONE, GAMETEXT_UI_D_Projectile_Spell, EXIT, CMDMENU_SOUND_NONE}}, //Glitched Spell (Maybe "Quake", mentioned elsewhere?)
    {{BIT_Spell_Portal,     NONE,    TEXTABLE_466, NONE, GAMETEXT_UI_3B_Portal_Spell,    EXIT, CMDMENU_SOUND_NONE}}, //Portal Spell
    {{BIT_Spell_Mind_Read,  NONE,    TEXTABLE_470, NONE, GAMETEXT_UI_3E_Mindread_Spell,  EXIT, CMDMENU_SOUND_NONE}}, //Mind Read
    {END}
};

/* Sidekick Commands (Tricky) (NOTE: uses a slightly different struct)
 * 0x710*/ static InventoryCommand dPage7CommandsTricky[] = {
    {Sidekick_Command_FLAG_01_Heel,      Sidekick_Command_INDEX_0_Heel,     TEXTABLE_1CD, TEXTABLE_1CD, GAMETEXT_UI_0_Heel,      0, 0}, //Heel
    {Sidekick_Command_FLAG_20_Play,      Sidekick_Command_INDEX_5_Play,     TEXTABLE_1D1, TEXTABLE_1D1, GAMETEXT_UI_6_Play,      0, 0}, //Play
    {Sidekick_Command_FLAG_02_Find,      Sidekick_Command_INDEX_1_Find,     TEXTABLE_1CE, TEXTABLE_1CE, GAMETEXT_UI_1_Find,      0, 0}, //Find
    {Sidekick_Command_FLAG_10_Flame,     Sidekick_Command_INDEX_4_Flame,    TEXTABLE_1CF, TEXTABLE_1CF, GAMETEXT_UI_2_Flame,     0, 0}, //Flame
    {Sidekick_Command_FLAG_04_Distract,  Sidekick_Command_INDEX_2_Distract, TEXTABLE_1D0, TEXTABLE_1D0, GAMETEXT_UI_3_Distract,  0, 0}, //Distract
    {Sidekick_Command_FLAG_08_Guard,     Sidekick_Command_INDEX_3_Guard,    TEXTABLE_1D2, TEXTABLE_1D2, GAMETEXT_UI_4_Guard,     0, 0}, //Guard
    END
};

/* Sidekick Commands (Kyte) (NOTE: uses a slightly different struct)
 * 0x764*/ static InventoryCommand sPage8CommandsKyte[] = {
    {Sidekick_Command_FLAG_01_Heel,      Sidekick_Command_INDEX_0_Heel,     TEXTABLE_1CD, TEXTABLE_1CD, GAMETEXT_UI_0_Heel,      0, 0}, //Heel
    {Sidekick_Command_FLAG_20_Play,      Sidekick_Command_INDEX_5_Play,     TEXTABLE_1D1, TEXTABLE_1D1, GAMETEXT_UI_6_Play,      0, 0}, //Play
    {Sidekick_Command_FLAG_02_Find,      Sidekick_Command_INDEX_1_Find,     TEXTABLE_1CE, TEXTABLE_1CE, GAMETEXT_UI_1_Find,      0, 0}, //Find
    {Sidekick_Command_FLAG_10_Flame,     Sidekick_Command_INDEX_4_Flame,    TEXTABLE_1CF, TEXTABLE_1CF, GAMETEXT_UI_2_Flame,     0, 0}, //Flame
    {Sidekick_Command_FLAG_04_Distract,  Sidekick_Command_INDEX_2_Distract, TEXTABLE_1D0, TEXTABLE_1D0, GAMETEXT_UI_3_Distract,  0, 0}, //Distract
    {Sidekick_Command_FLAG_08_Guard,     Sidekick_Command_INDEX_3_Guard,    TEXTABLE_1D2, TEXTABLE_1D2, GAMETEXT_UI_4_Guard,     0, 0}, //Guard
    END
};

/* Dinosaur Food Bag Actions (Krystal)
 * 0x7B8*/ static InventoryItem dPage9FoodActionsKyte[] = {
    {{BIT_Dino_Foodbag_Place, NONE, TEXTABLE_2C8, NONE, GAMETEXT_UI_20_Place, CMDMENU_PAGE_11_Food_Kyte,   CMDMENU_SOUND_PAGE}}, //Place food down from foodbag
    {{BIT_Dino_Foodbag_Give,  NONE, TEXTABLE_561, NONE, GAMETEXT_UI_21_Give,  CMDMENU_PAGE_11_Food_Kyte,   CMDMENU_SOUND_PAGE}}, //Give food from foodbag
    {END}
};

/* Dinosaur Food Bag Actions (Sabre)
 * 0x7DC*/ static InventoryItem dPage10FoodActionsTricky[] = {
    {{BIT_Dino_Foodbag_Place, NONE, TEXTABLE_2C8, NONE, GAMETEXT_UI_20_Place, CMDMENU_PAGE_12_Food_Tricky, CMDMENU_SOUND_PAGE}}, //Place food down from foodbag
    {{BIT_Dino_Foodbag_Give,  NONE, TEXTABLE_561, NONE, GAMETEXT_UI_21_Give,  CMDMENU_PAGE_12_Food_Tricky, CMDMENU_SOUND_PAGE}}, //Give food from foodbag
    {END}
};

/* Dinosaur Food Bag Items (Kyte)
 * 0x800*/ static InventoryItem dPage11FoodItemsKyte[] = {
    {{BIT_Krystal_Dino_Bag_S,      BIT_Krystal_Dino_Bag_M, TEXTABLE_27A, NONE, GAMETEXT_UI_2F_Small_Grub_Bag,       CMDMENU_PAGE_9_Food_Actions_Kyte,       CMDMENU_SOUND_PAGE}}, //Small Mushroom Bag
    {{BIT_Krystal_Dino_Bag_M,      BIT_Krystal_Dino_Bag_L, TEXTABLE_244, NONE, GAMETEXT_UI_30_Medium_Grub_Bag,      CMDMENU_PAGE_9_Food_Actions_Kyte,       CMDMENU_SOUND_PAGE}}, //Medium Mushroom Bag
    {{BIT_Krystal_Dino_Bag_L,      NONE,                   TEXTABLE_2BF, NONE, GAMETEXT_UI_31_Large_Grub_Bag,       CMDMENU_PAGE_9_Food_Actions_Kyte,       CMDMENU_SOUND_PAGE}}, //Large Mushroom Bag
    {{BIT_Dino_Bag_Blue_Mushrooms, NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_C_Blue_Mushroom,         EXIT,                                   CMDMENU_SOUND_ITEM}}, //Blue Mushroom
    {{BIT_Dino_Bag_Red_Mushrooms,  NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_2D_Red_Mushroom,         EXIT,                                   CMDMENU_SOUND_ITEM}}, //Red Mushroom
    {{BIT_Dino_Bag_Old_Mushrooms,  NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_2E_Old_Mushroom,         EXIT,                                   CMDMENU_SOUND_ITEM}}, //Old Mushroom
    {{BIT_Dino_Bag_Blue_Grubs,     NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_2A_Blue_Grub,            EXIT,                                   CMDMENU_SOUND_ITEM}}, //Blue Grub
    {{BIT_Dino_Bag_Red_Grubs,      NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_2B_Red_Grub,             EXIT,                                   CMDMENU_SOUND_ITEM}}, //Red Grub
    {{BIT_Dino_Bag_Old_Grubs,      NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_2C_Old_Grub,             EXIT,                                   CMDMENU_SOUND_ITEM}}, //Old Grub
    {END}
};

/* Dinosaur Food Bag Items (Tricky)
 * 0x878*/ static InventoryItem dPage12FoodItemsTricky[] = {
    {{BIT_Sabre_Dino_Bag_S,        BIT_Sabre_Dino_Bag_M,   TEXTABLE_27A, NONE, GAMETEXT_UI_32_Small_Mushroom_Bag,   CMDMENU_PAGE_10_Food_Actions_Tricky,    CMDMENU_SOUND_PAGE}}, //Small Mushroom Bag
    {{BIT_Sabre_Dino_Bag_M,        BIT_Sabre_Dino_Bag_L,   TEXTABLE_244, NONE, GAMETEXT_UI_33_Medium_Mushroom_Bag,  CMDMENU_PAGE_10_Food_Actions_Tricky,    CMDMENU_SOUND_PAGE}}, //Medium Mushroom Bag
    {{BIT_Sabre_Dino_Bag_L,        NONE,                   TEXTABLE_2BF, NONE, GAMETEXT_UI_34_Large_Mushroom_Bag,   CMDMENU_PAGE_10_Food_Actions_Tricky,    CMDMENU_SOUND_PAGE}}, //Large Mushroom Bag
    {{BIT_Dino_Bag_Blue_Mushrooms, NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_C_Blue_Mushroom,         EXIT,                                   CMDMENU_SOUND_ITEM}}, //Blue Mushroom
    {{BIT_Dino_Bag_Red_Mushrooms,  NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_2D_Red_Mushroom,         EXIT,                                   CMDMENU_SOUND_ITEM}}, //Red Mushroom
    {{BIT_Dino_Bag_Old_Mushrooms,  NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_2E_Old_Mushroom,         EXIT,                                   CMDMENU_SOUND_ITEM}}, //Old Mushroom
    {{BIT_Dino_Bag_Blue_Grubs,     NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_2A_Blue_Grub,            EXIT,                                   CMDMENU_SOUND_ITEM}}, //Blue Grub
    {{BIT_Dino_Bag_Red_Grubs,      NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_2B_Red_Grub,             EXIT,                                   CMDMENU_SOUND_ITEM}}, //Red Grub
    {{BIT_Dino_Bag_Old_Grubs,      NONE,                   TEXTABLE_270, NONE, GAMETEXT_UI_2C_Old_Grub,             EXIT,                                   CMDMENU_SOUND_ITEM}}, //Old Grub
    {END}
};

/*0x8F0*/ static CmdmenuPage dCmdmenuPages[] = {
    /*0*/  { dPage0ItemsKrystal,         0, 0x80001, R_CBUTTONS },  // Krystal items
    /*1*/  { dPage1ItemsSabre,           0, 0x80001, R_CBUTTONS },  // Sabre items
    /*2*/  { dPage2FoodActionsKrystal,   0, 0x80001, R_CBUTTONS },  // Foodbag actions (Krystal)
    /*3*/  { dPage3FoodActionsSabre,     0, 0x80001, R_CBUTTONS },  // Foodbag actions (Sabre)
    /*4*/  { dPage4FoodItemsKrystal,     0, 0x80001, R_CBUTTONS },  // Foodbag items (Krystal)
    /*5*/  { dPage5FoodItemsSabre,       0, 0x80001, R_CBUTTONS },  // Foodbag items (Sabre)
    /*6*/  { dPage6MagicSpells,          0, 0x80002, L_CBUTTONS },  // Magic spells
    /*7*/  { dPage7CommandsTricky,       0, 0x80009, D_CBUTTONS },  // Sidekick commands (Tricky)
    /*8*/  { sPage8CommandsKyte,         0, 0x80009, D_CBUTTONS },  // Sidekick commands (Kyte)
    /*9*/  { dPage9FoodActionsKyte,      0, 0x80001, R_CBUTTONS },  // Dinosaur foodbag actions (Krystal)
    /*10*/ { dPage10FoodActionsTricky,  0, 0x80001, R_CBUTTONS },  // Dinosaur foodbag actions (Sabre)
    /*11*/ { dPage11FoodItemsKyte,      0, 0x80001, R_CBUTTONS },  // Dinosaur foodbag items (Krystal)
    /*12*/ { dPage12FoodItemsTricky,    0, 0x80001, R_CBUTTONS },  // Dinosaur foodbag items (Sabre)
    /*13*/ { NULL,                      0, 0,       0 }
};

/*0x9D0*/ static s8 dPageCategory = 0; //Category of cmdmenu page currently open (see `CmdMenuPageCategories`)
/*0x9D4*/ static s8 dNextPageCategory = 0; //Assigned a non-zero value when switching between sections in the inventory
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
    /*14*/ TEXTABLE_3EA_CMDMENU_Unk_Circle_Glow, //Small glowing purple circle, unknown purpose
    /*15*/ TEXTABLE_3EB_CMDMENU_Unk_Circle_Blue, //Small blue circle, unknown purpose
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
    /*38*/ TEXTABLE_1D4_CMDMENU_LeftDownButtons_SpellBook_With_Kyte,
    /*39*/ TEXTABLE_42F_CMDMENU_C_Left,
    /*40*/ TEXTABLE_432_CMDMENU_Sabre,
    /*41*/ TEXTABLE_430_CMDMENU_C_Right,
    /*42*/ TEXTABLE_433_CMDMENU_Tricky,
    /*43*/ TEXTABLE_42E_CMDMENU_LeftDownButtons_SpellBook_With_Tricky,
    /*44*/ TEXTABLE_435_CMDMENU_Mushroom_Empty,
    /*45*/ TEXTABLE_436_CMDMENU_Mushroom_Red_Full,
    /*46*/ TEXTABLE_438_CMDMENU_Mushroom_Red_Half,
    /*47*/ TEXTABLE_43A_CMDMENU_RightButton_With_Bag,
    /*48*/ TEXTABLE_43B_CMDMENU_LeftDownButtons_SpellBook_NoSidekick,
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
/*0xC*/ static f32 _bss_C;          //Unused
/*0x10*/ static CmdmenuPlayerSidekickData sStats;
/*0x38*/ static CmdmenuPlayerSidekickData sPrevStats;
/*0x60*/ static CmdmenuPlayerSidekickDataChangeTimers sStatsChangeTimers;
/*0x88*/ static u8 sPlayerStatsFlags;
/*0x89*/ static u8 sAnimFrameScarab;        //Frame offset for the Scarab (an ID offset in practice, since Scarab's animation frames are stored as separate textures)
/*0x8A*/ static u8 sAnimScarabFlutterTimer; //Plays Scarab flutter animation during last ticks of countdown
/*0x8B*/ static u8 sAnimScarabSpin;         //Plays Scarab spin animation when nonzero (used as frame offset)
/*0x8C*/ static f32 sOpacityR;              //Opacity of icons on right side of screen (C-buttons, menu page image, etc.)
/*0x90*/ static EnergyBar* sEnergyBar; 
/*0x98*/ static Texture* sMenuItemTextures[MAX_LOADED_ITEMS];           //Inventory icon texture pointers for the current menu page's loaded items
/*0x198*/ static Texture* sMenuItemTexturesSidekick[MAX_LOADED_ITEMS];  //Sidekick command inventory icon textures for the current menu page's loaded items (unused aside from loading the textures)
/*0x298*/ static s16 sMenuItemTextureIDs[MAX_LOADED_ITEMS];             //TextableIDs for the current menu page's loaded items
/*0x318*/ static s32 sMenuItemGamebits[MAX_LOADED_ITEMS];               //GamebitIDs for the current menu page's loaded items (or for sidekick commands: the command's index)
/*0x418*/ static s16 sMenuItemTextIDs[MAX_LOADED_ITEMS];                //Gametext lineIDs for the current menu page's loaded items
/*0x498*/ static s8 sMenuItemOpenPageIDs[MAX_LOADED_ITEMS];             //The menu page ID opened by each of the current menu page's loaded items (or -1 if it closes the inventory on use)
/*0x4D8*/ static u8 sMenuItemUseSounds[MAX_LOADED_ITEMS];               //The sound types (see `CmdMenuItemSounds`) used by each of the current menu page's loaded items
/*0x518*/ static u8 sMenuItemVisibilities[MAX_LOADED_ITEMS];            //Visibility Booleans for each of the current menu page's loaded items (Spells are the only kind of inventory item that still load while their gamebitHidden is set, however they're drawn as an empty tile when hidden)
/*0x558*/ static u8 sMenuItemQuantities[MAX_LOADED_ITEMS];              //Item quantities for each of the current menu page's loaded items
/*0x598*/ static Texture* sActiveSpellIcon;             //Icon in bottom-right of screen, showing the Spell currently in use
/*0x59C*/ static Texture* sActiveSpellRing;             //Icon in bottom-right of screen, circling the Spell currently in use
/*0x5A0*/ static Texture* sAButtonAnimTex;              //Animated A button icon, shown on the tutorial textbox
/*0x5A4*/ static s16 sPrevActiveSpellGamebit;           //The gamebitID of the mostly recently-used Spell (used to check if the active Spell changed)
/*0x5A8*/ static Texture* sActiveSidekickCommandIcon;   //Icon in bottom-right of screen, showing the Sidekick Command currently in use
/*0x5AC*/ static Texture* sActiveSidekickCommandRing;   //Icon in bottom-right of screen, circling the Sidekick Command currently in use
/*0x5B0*/ static s16 sPrevSidekickCommandIndex;         //The index of the mostly recently-used Sidekick Command (used to check if the active Sidekick Command changed)
/*0x5B4*/ static s32 sAButtonAnimRenderFlags;
/*0x5B8*/ static s32 sCrosshairAnimRenderFlags;
/*0x5BC*/ static s32 sAButtonAnimProgress;
/*0x5C0*/ static s32 sCrosshairAnimProgress;
/*0x5C8*/ static Texture* sTextures[ARRAYCOUNT(dTextableIDs)];
/*0x6B0*/ static Texture* sInventoryStackNumbersTex;
/*0x6B8*/ static TextureTile sTextureTiles[ARRAYCOUNT(dTextableIDs)][2];
/*0xC28*/ static s16 sInventoryUnrollY;  //How far the inventory scroll has opened (0 when fully closed)
/*0xC2A*/ static s16 sInfoScrollUnrollY; //How far the R-button info scroll has opened (0 when fully closed)
/*0xC2C*/ static s16 sTutorialBoxHeight;
/*0xC2E*/ static s16 sTutorialBoxStringIndex;
/*0xC30*/ static GameTextChunk* sTutorialBoxGametext;
/*0xC34*/ static Texture* sCrosshairTex;
/*0xC38*/ static s16 sUsedItemGamebitID;  //The gamebitID associated with the used item
/*0xC3A*/ static s16 sSubmenuGamebitID;   //The gamebitID associated with the item that opened a menu subpage (e.g. a foodbag's gamebit)
/*0xC3C*/ static s8 sUsedItemSoundType;   //Set to 0 when item selection successful (item given to character, etc.)
/*0xC3D*/ static s8 sUsedItemPageID;      //The pageID associated with the used item (see `CmdMenuPages`)
/*0xC3E*/ static s8 sInventoryPageID;     //The pageID currently open (see `CmdMenuPages`)
/*0xC40*/ static s16 sMenuSelectedItemIdx; //Display index of the item currently selected in the menu page 
/*0xC44*/ static s32 sDisplayedItemCount;  //The number of items displayed on the current page (while drawing the inventory icons, this number is updated to be at least the number of slots in the tile strip)
/*0xC48*/ static s8 sShouldOverrideJoypadButtons;   //Whether to fully override the player's UI control with simulated button presses
/*0xC4C*/ static s32 sInventoryFrameCounter;        //Counts how many times `cmdmenu_update2` has run (clamped from 0-2, and resets to 0 upon closing the inventory)
/*0xC50*/ static s32 sJoyPressedButtons;            //Joypad button bitfield
/*0xC54*/ static s32 sJoyPressedButtonsOverride;    //Joypad button bitfield (for simulated presses, used during inventory tutorials)
/*0xC58*/ static s32 sJoyHeldButtons;               //Joypad button bitfield
/*0xC60*/ static TextureTile sTempIcon[2];
/*0xC78*/ static s8 sAutoSelectItemGamebit;     //Always -1, but seems intended to auto-select a specific item when opening the inventory
/*0xC7A*/ static s16 sAutoSelectItemIdx;        //Index of the auto-selected item (unused in practice)
/*0xC7C*/ static s16 sInfoScrollOverrideTextID; //Causes the info scroll to open automatically (@bug: initialises at 0 instead of NO_GAMETEXT. This causes the info box to display for the first few frames of gameplay, leaving a beige smear in the top-left of the framebuffer.)
/*0xC7E*/ static s16 sInfoScrollOverrideX;      //Custom screen position for the info scroll when auto-shown
/*0xC80*/ static s16 sInfoScrollOverrideY;      //Custom screen position for the info scroll when auto-shown
/*0xC88*/ static CmdmenuInfoPopup sInfoPopup;   //Item info pop-up that appears after collecting certain items (e.g. Kyte's grubs)

static void cmdmenu_tick_tutorial_textbox(void);
static void cmdmenu_draw_tutorial_textbox(Gfx** gdl, Mtx** mtxs, Vertex** vtxs);
static void cmdmenu_tick_inventory_page(void);
static void cmdmenu_draw_main(Gfx** gdl, Mtx** mtxs, Vertex** vtxs);
static s32 cmdmenu_page_load_items(InventoryItem* items, s8 isSidekickMenu);
static s32 cmdmenu_page_count_shown_items(InventoryItem* menuItems, s8 isSidekickMenu);
static void cmdmenu_store_loaded_item_metadata(InventoryItem* items, s32 loadedItemIndex, s32 itemIndex);
static void cmdmenu_gfx_set_texture(Gfx** gdl, Texture* tex, s32 frame);
static int cmdmenu_is_inventory_open(void);
static int cmdmenu_is_inventory_closed(void);
static void cmdmenu_close_inventory(void);
static void cmdmenu_open_inventory(void);
static void cmdmenu_inventory_animate(void);
static void cmdmenu_draw_c_buttons_and_sidekick_meter(Gfx** gdl, Mtx** mtxs, Vertex** vtxs);
static void cmdmenu_gfx_set_scroll_scissor(Gfx** gdl);
static void cmdmenu_gfx_set_screen_scissor(Gfx** gdl);
static int cmdmenu_is_info_scroll_closed(void);
static void cmdmenu_close_info_scroll(void);
static void cmdmenu_open_info_scroll(void);
static void cmdmenu_info_scroll_animate(void);
static void cmdmenu_draw_info_scroll(Gfx** gdl, Mtx** mtxs, Vertex** vtxs);
static void cmdmenu_update_stats(void);
static void cmdmenu_draw_player_stats(Gfx** gdl, Mtx** mtxs, Vertex** vtxs);
static void cmdmenu_info_hide(CmdmenuInfoPopup* info);
static void cmdmenu_info_draw(Gfx** gdl, CmdmenuInfoPopup* box);
static void cmdmenu_draw_energy_bar(Gfx** gdl);
void cmdmenu_energy_bar_free(void);

// offset: 0x0 | ctor
void cmdmenu_ctor(void* dll) {
    u32 i;

    sAutoSelectItemGamebit = NO_GAMEBIT;
    sAutoSelectItemIdx = NO_ITEM;
    sUsedItemGamebitID = NO_GAMEBIT;
    sUsedItemSoundType = CMDMENU_SOUND_NONE;
    sUsedItemPageID = NO_PAGE;
    sPrevActiveSpellGamebit = NO_GAMEBIT;
    sPrevSidekickCommandIndex = NO_SIDEKICK_COMMAND;

    //Initialise all the cmdmenu TextureTiles and load their Textures
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
    u32 i;

    //Free all UI textures
    for (i = 0; i < ARRAYCOUNT(sTextures); i++) {
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
/**
  * Sets a controller button mask, affecting player control of the inventory.
  */
void cmdmenu_disable_buttons(u16 mask) {
    sJoyButtonMask = mask;
}

// offset: 0x2E0 | func: 1 | export: 19
/**
  * A nonzero argument causes the player's stats HUD to appear automatically. An argument of 0 will remove this effect.
  */
void cmdmenu_toggle_forced_stats_display(u8 force) {
    sForceStatsDisplay = force;
}

// offset: 0x308 | func: 2 | export: 0
/**
  * Main tick function for the cmdmenu (called by `menu_gameplay.c`)
  */
s32 cmdmenu_update1(void) {
    cmdmenu_tick_inventory_page();
    cmdmenu_tick_tutorial_textbox();
    return 0;
}

// offset: 0x35C | func: 3 | export: 1
/**
  * Handles opening/closing player interactions for the inventory and info scroll:
  * - Inventory: opening/closing inventory pages with the C buttons (or when entering sub-pages like the foodbag)
  * - Info scroll: opening when holding R (or automatically), and sets the displayed textID
  *
  * Also calls the following functions:
  * - Calls `cmdmenu_update_stats` (to track changes in player stats, and show C-buttons/health when holding R)
  * - Calls `cmdmenu_inventory_animate` (inventory fading/expanding/collapsing)
  * - Calls `cmdmenu_info_scroll_animate` (info scroll fading/expanding/collapsing)
  */
void cmdmenu_update2(void) {
    Object* player;
    Object* sidekick;
    s16 pad1;
    s8 pad2;
    s16 newStringID;
    s8 autoShowInfoScroll;
    s8 newPageIndex;

    player = get_player();
    sidekick = get_sidekick();

    if (player == NULL) {
        return;
    }

    //Get controller button presses/holds
    sJoyPressedButtons = joy_get_pressed(0);
    sJoyHeldButtons = joy_get_buttons(0);

    if (player->stateFlags & OBJSTATE_IN_SEQ) {
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
        if ((player->stateFlags & OBJSTATE_IN_SEQ) || (sJoyButtonMask != 0)) {
            sJoyPressedButtons |= B_BUTTON;
        }
    }

    //Use C buttons (left/down/right) to open/change inventory pages
    if ((sJoyPressedButtons & D_CBUTTONS) && (sidekick != NULL) && (dPageCategory != CMDMENU_CATEGORY_2_Sidekick)) {
        //C-down: Sidekick Commands
        newPageIndex = sidekick->id == OBJ_Kyte ? CMDMENU_PAGE_8_Sidekick_Kyte : CMDMENU_PAGE_7_Sidekick_Tricky;
        if (cmdmenu_page_count_shown_items(dCmdmenuPages[newPageIndex].items, TRUE)) {
            joy_set_button_mask(0, D_CBUTTONS);
            dNextPageCategory = CMDMENU_CATEGORY_2_Sidekick;
            sInventoryPageID = newPageIndex;
        }
    } else if ((sJoyPressedButtons & R_CBUTTONS) && (dPageCategory != CMDMENU_CATEGORY_3_Items) && (dPageCategory != CMDMENU_CATEGORY_6_Food)) {
        //C-right: Items
        newPageIndex = player->id == OBJ_Krystal ? CMDMENU_PAGE_0_Items_Krystal : CMDMENU_PAGE_1_Items_Sabre;
        if (cmdmenu_page_count_shown_items(dCmdmenuPages[newPageIndex].items, FALSE)) {
            joy_set_button_mask(0, R_CBUTTONS);
            dNextPageCategory = CMDMENU_CATEGORY_3_Items;
            sInventoryPageID = newPageIndex;
        }
    } else if ((sJoyPressedButtons & L_CBUTTONS) && (dPageCategory != CMDMENU_CATEGORY_4_Spells)) {
        //C-left: Magic Spells
        if (cmdmenu_page_count_shown_items(dCmdmenuPages[CMDMENU_PAGE_6_Spells].items, FALSE)) {
            joy_set_button_mask(0, L_CBUTTONS);
            dNextPageCategory = CMDMENU_CATEGORY_4_Spells;
            sInventoryPageID = CMDMENU_PAGE_6_Spells;
        }
    } else if (sUsedItemPageID != EXIT) {
        //No C-buttons were pressed, but the selected item opened an inventory page (e.g. foodbags)
        sSubmenuGamebitID = sUsedItemGamebitID;
        sInventoryPageID = sUsedItemPageID;

        //Sidekick Commands -> Sidekick Foodbag
        if ((dPageCategory == CMDMENU_CATEGORY_2_Sidekick) || (dPageCategory == CMDMENU_CATEGORY_5)) {
            dNextPageCategory = CMDMENU_CATEGORY_7_Sidekick_Food; //@bug? Maybe Category 5 is intended for Sidekick Food (i.e. 2->5, 3->6, 4->7; instead of 2->7, 3->6, 4->7)
        //Items -> Player Foodbag
        } else if ((dPageCategory == CMDMENU_CATEGORY_3_Items) || (dPageCategory == CMDMENU_CATEGORY_6_Food)) {
            dNextPageCategory = CMDMENU_CATEGORY_6_Food;
        //Spells -> Spell subpage (unused)
        } else if ((dPageCategory == CMDMENU_CATEGORY_4_Spells) || (dPageCategory == CMDMENU_CATEGORY_7_Sidekick_Food)) {
            dNextPageCategory = CMDMENU_CATEGORY_7_Sidekick_Food;
        }
    }

    //Keep the inventory closed during the Galleon battle
    if (gDLL_2_Camera->vtbl->get_dll_ID() == DLL_ID_CAMSHIPBATTLE2) {
        cmdmenu_close_inventory();
    } else if (dNextPageCategory != 0) {
        //Handle opening the inventory (or opening a different page category)
        if (cmdmenu_is_inventory_closed()) {
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

            //Sidekick command pages always open on index 0 (unlike other pages which remember last selection)
            dCmdmenuPages[CMDMENU_PAGE_7_Sidekick_Tricky].selectedIndex = 0;
            dCmdmenuPages[CMDMENU_PAGE_8_Sidekick_Kyte].selectedIndex = 0;

            dPageCategory = dNextPageCategory;
            sJoyPressedButtons = 0;
            dInventoryMovesQueued = 0;
            dNextPageCategory = 0;
        } else {
            /* When changing page category (e.g. from Items to Spells),
               first close up the scroll's current page, before reopening with the new page */
            cmdmenu_close_inventory();
        }
    }

    cmdmenu_update_stats();
    cmdmenu_inventory_animate();
    cmdmenu_info_scroll_animate();

    //Count how many times this function has run (clamped from 0-2)
    sInventoryFrameCounter++;
    if (sInventoryFrameCounter > 2) {
        sInventoryFrameCounter = 2;
    }

    /* If the inventory is visible, use the selected item's gametext,
       otherwise use the target Object's description gametext */
    if ((dInventoryShow != FALSE) || (dNextPageCategory != 0)) {
        newStringID = dSelectedItemTextID;
    } else {
        newStringID = gDLL_2_Camera->vtbl->get_target_gametextID();
    }
    dSelectedItemTextID = NO_GAMETEXT;

    //Check if the info scroll should be auto-shown
    autoShowInfoScroll = FALSE;
    if (sInfoScrollOverrideTextID > NO_GAMETEXT) {
        newStringID = sInfoScrollOverrideTextID;

        //When auto-shown, scroll's screen position can temporarily be changed
        dInfoScrollY = sInfoScrollOverrideY; 
        dInfoScrollX = sInfoScrollOverrideX;

        autoShowInfoScroll = TRUE;
    } else {
        //Restore the scroll's default position when closed
        if (cmdmenu_is_info_scroll_closed()) {
            dInfoScrollX = INFO_SCROLL_X;
            dInfoScrollY = INFO_SCROLL_Y;
        }
    }
    sInfoScrollOverrideTextID = NO_GAMETEXT;

    //Open the scroll when R is held (or when auto-shown) and a textID is set
    if (((sJoyHeldButtons & R_TRIG) || autoShowInfoScroll) && (newStringID > NO_GAMETEXT)) {
        //Free the scroll's current strings when the textID changes
        if (newStringID != dInfoScrollTextID) {
            dInfoScrollTextID = newStringID;
            if (*dInfoScrollStrings != NULL) {
                mmFree(*dInfoScrollStrings);
                *dInfoScrollStrings = NULL;
            }
        }
        cmdmenu_open_info_scroll();
    } else {
        cmdmenu_close_info_scroll();
        //Free the scroll's current strings when fully closed
        if (cmdmenu_is_info_scroll_closed() && (*dInfoScrollStrings != NULL)) {
            mmFree(*dInfoScrollStrings);
            *dInfoScrollStrings = NULL;
            dInfoScrollTextID = NO_GAMETEXT;
        }
    }

    joy_set_button_mask(0, L_CBUTTONS | R_CBUTTONS | D_CBUTTONS);
    sJoyButtonMask = 0;
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

    //Draw Spell reticle when aiming (@bug: x coord not adjusted in widescreen)
    if (((DLL_210_Player*)player->dll)->vtbl->func77(player, &screenX, &screenY)) {
        tex_animate(sCrosshairTex, &sCrosshairAnimRenderFlags, &sCrosshairAnimProgress);
        rcp_screen_full_write(
            gdl, 
            sCrosshairTex, 
            screenX - (AIMING_RETICLE_WIDTH/2), 
            screenY - (AIMING_RETICLE_HEIGHT/2), 
            0, 
            sCrosshairAnimProgress >> 8, 
            AIMING_RETICLE_OPACITY, 
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

    cmdmenu_draw_c_buttons_and_sidekick_meter(gdl, mtxs, vtxs);
    cmdmenu_draw_info_scroll(gdl, mtxs, vtxs);
    cmdmenu_draw_tutorial_textbox(gdl, mtxs, vtxs);
    cmdmenu_draw_main(gdl, mtxs, vtxs);
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
        //Don't play the "item refused" sound when an item was used successfully
        sUsedItemSoundType = CMDMENU_SOUND_NONE;
        return TRUE;
    }
    return FALSE;
}

// offset: 0xE2C | func: 7 | export: 8
/**
  * Checks if the used inventory item's gamebitID matches any of those in a given array of gamebitIDs.
  *
  * If a match is found, the used item's gamebitID is returned. 
  *
  * If no match was found, `NO_GAMEBIT` (-1) is returned.
  *
  * The array's address should be provided (`gamebitArray`), as well as its item count (`arraySize`).
  */
s32 cmdmenu_was_used_item_in_gamebit_array(s32* gamebitArray, s32 arraySize) {
    s32 i;

    for (i = 0; i < arraySize; i++) {
        if (sUsedItemGamebitID == gamebitArray[i]) {
            sUsedItemSoundType = CMDMENU_SOUND_NONE;
            return gamebitArray[i];
        }
    }

    return NO_GAMEBIT;
}

// offset: 0xF24 | func: 8 | export: 9
/**
  * Returns the categoryID of the current inventory menu page (see `CmdMenuPageCategories`)
  */
s8 cmdmenu_get_page_category(void) {
    return dPageCategory;
}

// offset: 0xF40 | func: 9 | export: 10
/**
  * Returns the gamebitID associated with the current menu subpage.
  * (e.g. if Krystal has opened her Food Bag Actions page, the returned gamebitID would be: 
            `BIT_Krystal_Foodbag_S` or `BIT_Krystal_Foodbag_M` or
            `BIT_Krystal_Foodbag_L`, depending on which bag she has.)
  */
s16 cmdmenu_get_subpage_gamebit(void) {
    return sSubmenuGamebitID;
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

        if ((obj->def->lockdata != NULL) && 
            (obj->opacity == OBJECT_OPACITY_MAX) && 
            ((obj->unkAF & ARROW_FLAG_8_No_Targetting) == FALSE) && 
            (obj->def->lockdata->flags & lockFlag) && 
            (targetCount < maxObjects) && 
            (arg3 & 1)
        ) {
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
/**
  * Zeroes the selected item index on all the inventory pages (i.e. forgets user's last selection).
  */
void cmdmenu_pages_clear_last_selected_index(void) {
    s32 i;
    CmdmenuPage *page;

    page = dCmdmenuPages;

    for (i = 0; page[i].items; i++) { page[i].selectedIndex = 0; }

    sUsedItemGamebitID = NO_GAMEBIT;
    sUsedItemSoundType = CMDMENU_SOUND_NONE;
    sUsedItemPageID = NO_PAGE;
}

// offset: 0x12EC | func: 12 | export: 4
/** 
  * Flags that a snapshot of the player's current tracked stats should be taken.
  *
  * (Used to show UI when player health changes, etc.)
  */
void cmdmenu_request_new_player_stats_snapshot(void) {
    sPlayerStatsFlags = PlayerStats_FLAG_Update_Snapshot;
}

// offset: 0x130C | func: 13 | export: 12
/**
  * Overrides the info scroll, displaying a specific textID at a custom screen position (can differ from usual top-centre).
  *
  * See `GameTexts_UI_A` and `GameTexts_UI_B` for textIDs.
  */
void cmdmenu_auto_show_info_scroll(u32 uiTextID, u32 screenX, u32 screenY) {
    sInfoScrollOverrideTextID = uiTextID;
    sInfoScrollOverrideX = screenX;
    sInfoScrollOverrideY = screenY;
}

// offset: 0x1338 | func: 14 | export: 13
/**
  * Opens the tutorial textbox at a specified screen position, displaying a specific gametext file's tutorial text.
  */
void cmdmenu_open_tutorial_textbox(s32 gametextID, s32 screenX, s32 screenY) {
    if (sTutorialBoxGametext == NULL) {
        sAButtonAnimRenderFlags = 0x40000;
        sAButtonAnimProgress = 0;
        sTutorialBoxGametext = gDLL_21_Gametext->vtbl->get_chunk(gametextID);
        sTutorialBoxStringIndex = 0;
        dTutorialBoxShow = TRUE;
        dTutorialBoxX = screenX;
        dTutorialBoxY = screenY;
        sTutorialBoxHeight = dTutorialBoxHeight;
    }
}

// offset: 0x13F4 | func: 15 | export: 14
/**
  * Causes the tutorial textbox to close.
  */
void cmdmenu_close_tutorial_textbox(void) {
    dTutorialBoxShow = FALSE;
}

// offset: 0x1410 | func: 16
/**
  * Handles the tutorial textbox's player interactions and animation:
  * - Fading the box in/out
  * - Playing the A button icon's animation
  * - Advancing the text (or closing textbox) when the player presses A. 
  */
static void cmdmenu_tick_tutorial_textbox(void) {
    //Update the tutorial textbox's opacity
    if (dTutorialBoxShow) {
        //Fade in the box background
        dTutorialBoxOpacity += gUpdateRate * TUTORIAL_BOX_OPACITY_SPEED;
        if (dTutorialBoxOpacity > TUTORIAL_BOX_OPACITY_MAX) {
            dTutorialBoxOpacity = TUTORIAL_BOX_OPACITY_MAX;
        }

        //Fade in the box's text (and button icon)
        dTutorialBoxTextOpacity += gUpdateRate * TUTORIAL_BOX_OPACITY_SPEED;
        if (dTutorialBoxTextOpacity > MAX_OPACITY) {
            dTutorialBoxTextOpacity = MAX_OPACITY;
        }
    } else {
        //Fade out the box's text first
        dTutorialBoxTextOpacity -= gUpdateRate * TUTORIAL_BOX_OPACITY_SPEED;
        if (dTutorialBoxTextOpacity < 0) {
            dTutorialBoxTextOpacity = 0;
        }

        //Then fade out the background and text together
        if (dTutorialBoxOpacity > dTutorialBoxTextOpacity) {
            dTutorialBoxOpacity = dTutorialBoxTextOpacity;
        }
    }

    //Free the box's gametext if the box is fully faded out
    if (dTutorialBoxOpacity == 0) {
        if (sTutorialBoxGametext != NULL) {
            mmFree(sTutorialBoxGametext);
            sTutorialBoxGametext = NULL;
        }
        return;
    }

    //Handle player interaction
    {
        //Animate the A button icon
        tex_animate(sAButtonAnimTex, &sAButtonAnimRenderFlags, &sAButtonAnimProgress);

        //Advance to the next paragraph upon pressing A
        if (sJoyPressedButtons & A_BUTTON) {
            sTutorialBoxStringIndex += 3; //3 strings per screen

            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);

            //Check if the end of the gametext has been reached
            if (sTutorialBoxStringIndex < sTutorialBoxGametext->count) {
                joy_set_button_mask(0, A_BUTTON);
            } else {
                //If so, close the tutorial box
                sTutorialBoxStringIndex -= 3;
                dTutorialBoxShow = FALSE;
            }
        }
    }
}

// offset: 0x1614 | func: 17
/** 
  * Draws the tutorial textbox that appears upon seeing/collecting items for the first time.
  */
static void cmdmenu_draw_tutorial_textbox(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    s32 x;
    s32 y;
    s32 halfWidth;
    s32 height;
    s32 tempY;
    s32 i;
    Gfx* dl;

    if (dTutorialBoxOpacity == 0) {
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
    dl_set_prim_color(&dl, 255, 255, 255, dTutorialBoxOpacity);

    //Get dimensions (note: coords multiplied by 4 for gSPTextureRectangle)
    x = dTutorialBoxX;
    x <<= 2;
    halfWidth = dTutorialBoxHalfWidth << 2;
    halfWidth -= (TUTORIAL_BOX_PAGE_EDGE_WIDTH << 2); //Subtract the page's edge margin
    y = dTutorialBoxY << 2;
    height = sTutorialBoxHeight << 2;
    
    //Draw main paper background 
    {
        //Middle section (excluding the paper's left/right edges)
        cmdmenu_gfx_set_texture(&dl, sTextures[CMDMENU_TEX_06_InfoScroll_BG], 0);
        gSPTextureRectangle(dl++, 
            /*ulx*/ x - halfWidth,
            /*uly*/ y,
            /*lrx*/ x + halfWidth,
            /*lry*/ y + height,
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(0), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
        );
        gDLBuilder->needsPipeSync = TRUE;
        
        //Left edge of paper
        cmdmenu_gfx_set_texture(&dl, sTextures[CMDMENU_TEX_05_InfoScroll_Side], 0);
        gSPTextureRectangle(dl++, 
            /*lrx*/ x - halfWidth - (TUTORIAL_BOX_PAGE_EDGE_WIDTH << 2),
            /*lry*/ y,
            /*ulx*/ x - halfWidth,
            /*uly*/ y + height,
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(0), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
        );
        gDLBuilder->needsPipeSync = TRUE;
        
        //Right edge of paper
        gSPTextureRectangle(dl++, 
            /*ulx*/ x + halfWidth,
            /*uly*/ y,
            /*lrx*/ x + halfWidth + (TUTORIAL_BOX_PAGE_EDGE_WIDTH << 2),
            /*lry*/ y + height,
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(15), /*t*/ qs105(0),
            /*dsdx*/ qs510(-1), /*dtdy*/ qs510(1)
        );
        gDLBuilder->needsPipeSync = TRUE;

        halfWidth += (TUTORIAL_BOX_PAGE_EDGE_WIDTH << 2); //restore half-width
    }
    
    //Draw the top/bottom rolls' page shadows
    {
        cmdmenu_gfx_set_texture(&dl, sTextures[CMDMENU_TEX_07_InfoScroll_SelfShadow], 0);
        dl_set_prim_color(&dl, 255, 128, 128, 128);
        
        //Top shadow
        tempY = y;
        gSPTextureRectangle(dl++,
            /*ulx*/ x - halfWidth,
            /*uly*/ tempY,
            /*lrx*/ x + halfWidth,
            /*lry*/ tempY + (TUTORIAL_BOX_PAGE_SHADOW_HEIGHT << 2),
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(7), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
        );
        gDLBuilder->needsPipeSync = TRUE;
        
        //Bottom shadow
        tempY = y + height - ((TUTORIAL_BOX_PAGE_SHADOW_HEIGHT - 2) << 2);
        gSPTextureRectangle(dl++,
            /*ulx*/ x - halfWidth,
            /*uly*/ tempY,
            /*lrx*/ x + halfWidth,
            /*lry*/ tempY + (TUTORIAL_BOX_PAGE_SHADOW_HEIGHT << 2),
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(0), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
        );
        gDLBuilder->needsPipeSync = TRUE;
        
        //Restore prim colour
        dl_set_prim_color(&dl, 255, 255, 255, dTutorialBoxOpacity);
    }

    //Draw the top/bottom rolls
    {
        //Top roll (middle span)
        {
            tempY = y - (TUTORIAL_BOX_ROLL_TOP_Y_OFFSET << 2);
            cmdmenu_gfx_set_texture(
                &dl, 
                sTextures[CMDMENU_TEX_04_InfoScroll_Roll],
                0
            );
            gSPTextureRectangle(dl++,
                /*ulx*/ x - halfWidth,
                /*uly*/ tempY,
                /*lrx*/ x + halfWidth,
                /*lry*/ tempY + (TUTORIAL_BOX_ROLL_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(0), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;
        }
        
        //Bottom roll (middle span)
        {
            tempY = y + height - (TUTORIAL_BOX_ROLL_BOTTOM_Y_OFFSET << 2);
            cmdmenu_gfx_set_texture(
                &dl, 
                sTextures[CMDMENU_TEX_04_InfoScroll_Roll], 
                0
            );
            gSPTextureRectangle(dl++,
                /*ulx*/ x - halfWidth,
                /*uly*/ tempY,
                /*lrx*/ x + halfWidth,
                /*lry*/ tempY + (TUTORIAL_BOX_ROLL_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(0), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;
        }
        
        //Roll handles
        {
            cmdmenu_gfx_set_texture(
                &dl, 
                sTextures[CMDMENU_TEX_03_InfoScroll_Roll_End], 
                0
            );
            
            //Top roll handle (left)
            tempY = y - (TUTORIAL_BOX_ROLL_TOP_Y_OFFSET << 2);
            gSPTextureRectangle(dl++,
                /*ulx*/ x - halfWidth - (TUTORIAL_BOX_HANDLE_WIDTH << 2),
                /*uly*/ tempY,
                /*lrx*/ x - halfWidth,
                /*lry*/ tempY + (TUTORIAL_BOX_HANDLE_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(0), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;
            
            //Top roll handle (right)
            gSPTextureRectangle(dl++,
                /*ulx*/ x + halfWidth,
                /*uly*/ tempY,
                /*lrx*/ x + halfWidth + (TUTORIAL_BOX_HANDLE_WIDTH << 2),
                /*lry*/ tempY + (TUTORIAL_BOX_HANDLE_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(15), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(-1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;

            //Bottom roll handle (left)
            tempY = y + height - (TUTORIAL_BOX_ROLL_BOTTOM_Y_OFFSET << 2);
            gSPTextureRectangle(dl++, 
                /*ulx*/ x - halfWidth - (TUTORIAL_BOX_HANDLE_WIDTH << 2),
                /*uly*/ tempY,
                /*lrx*/ x - halfWidth,
                /*lry*/ tempY + (TUTORIAL_BOX_HANDLE_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(0), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;
            
            //Bottom roll handle (right)
            gSPTextureRectangle(dl++, 
                /*ulx*/ x + halfWidth,
                /*uly*/ tempY,
                /*lrx*/ x + halfWidth + (TUTORIAL_BOX_HANDLE_WIDTH << 2),
                /*lry*/ tempY + (TUTORIAL_BOX_HANDLE_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(15), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(-1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;
        }
    }
    
    //Restore prim colour
    dl_set_prim_color(&dl, 255, 255, 255, 255);

    //Draw A button icon (animated) at bottom-centre of box
    rcp_screen_full_write(
        &dl, 
        sAButtonAnimTex, 
        dTutorialBoxX - (TUTORIAL_BOX_A_BUTTON_WIDTH/2) + 4, 
        (dTutorialBoxY + dTutorialBoxHeight) - TUTORIAL_BOX_A_BUTTON_HEIGHT, 
        0, 
        sAButtonAnimProgress >> 8, 
        dTutorialBoxTextOpacity, 
        SCREEN_WRITE_TRANSLUCENT
    );

    //Set up the text window
    font_window_set_coords(3, 
        /*x1*/ dTutorialBoxX - dTutorialBoxHalfWidth, 
        /*y1*/ dTutorialBoxY, 
        /*x2*/ dTutorialBoxX + dTutorialBoxHalfWidth, 
        /*y2*/ dTutorialBoxY + sTutorialBoxHeight);
    font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
    font_window_set_bg_colour(3, 0, 0, 0, 0);
    font_window_flush_strings(3);
    font_window_set_text_colour(3, 0, 0, 255, 255, dTutorialBoxTextOpacity);

    //Print 3 lines of text at a time
    for (i = sTutorialBoxStringIndex, y = TUTORIAL_BOX_TEXT_Y; 
        i < sTutorialBoxGametext->count && i < (sTutorialBoxStringIndex + 3); 
        i++, y += TUTORIAL_BOX_LINE_HEIGHT
    ) {
        font_window_add_string_xy(
            3, 
            -0x8000, 
            y, 
            sTutorialBoxGametext->strings[i], 
            1, 
            ALIGN_TOP_CENTER
        );
    }

    font_window_draw(&dl, mtxs, vtxs, 3);

    *gdl = dl;
}

// offset: 0x1FEC | func: 18
/**
  * Handles player interaction on the current page of the inventory scroll menu:
  * - Moving through the current page's items with its C button.
  * - Using an item with the A button.
  * - Closing the inventory with the B button.
  * - Playing a sound when using items (unsuccessfully) or entering subpages.
  * - Auto-selecting an item when opening the inventory (unused?)
  */
static void cmdmenu_tick_inventory_page(void) {
    s16* pageSelectionIndex;
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
    if (player->stateFlags & OBJSTATE_IN_SEQ) {
        joy_set_button_mask(0, L_CBUTTONS | R_CBUTTONS | D_CBUTTONS);
    } else if (sJoyButtonMask != 0) {
        joy_set_button_mask(0, sJoyButtonMask);
    }

    //Get button presses (or simulated ones, for tutorial sequences)
    if (sShouldOverrideJoypadButtons) {
        sJoyPressedButtons = sJoyPressedButtonsOverride;
    } else {
        sJoyPressedButtons = joy_get_pressed(0);
        if ((player->stateFlags & OBJSTATE_IN_SEQ) || (sJoyButtonMask != 0)) {
            sJoyPressedButtons |= B_BUTTON;
        }
    }

    //Play item use sound if needed
    switch (sUsedItemSoundType) {
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
    sUsedItemSoundType = CMDMENU_SOUND_NONE;
    sUsedItemPageID = NO_PAGE;

    pageItems = dCmdmenuPages[sInventoryPageID].items;
    pageSelectionIndex = &dCmdmenuPages[sInventoryPageID].selectedIndex;
    pageMsg = dCmdmenuPages[sInventoryPageID].mesgID;
    pageBtnMask = dCmdmenuPages[sInventoryPageID].btnMask;

    //Check if the current page is a sidekick command page
    if ((sInventoryPageID == CMDMENU_PAGE_7_Sidekick_Tricky) || 
        (sInventoryPageID == CMDMENU_PAGE_8_Sidekick_Kyte)
    ) {
        isSidekickMenu = TRUE;
    }

    sMenuSelectedItemIdx = *pageSelectionIndex;
    sDisplayedItemCount = cmdmenu_page_load_items(pageItems, isSidekickMenu);

    //If the current page has no visible items, close the inventory
    if (sDisplayedItemCount == 0) {
        dPageCategory = 0;
        cmdmenu_close_inventory();
        return;
    }

    if (sMenuSelectedItemIdx >= sDisplayedItemCount) {
        sMenuSelectedItemIdx = 0;
    }

    dSelectedItemTextID = sMenuItemTextIDs[sMenuSelectedItemIdx];

    //Handle scrolling through the inventory
    if (cmdmenu_is_inventory_open()) {
        //If the page's scroll button was pressed
        if ((sJoyPressedButtons & pageBtnMask) && (sInventoryScrollOffset < 8) && (dInventoryIsScrolling == FALSE)) {
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_28A_Cmdmenu_MoveSelection, MAX_VOLUME, NULL, NULL, 0, NULL);
            dInventoryMovesQueued++;

            if (sInventoryScrollOffset != 0) {
                dInventoryIsScrolling = TRUE;
            }
        }

        //Limit the number of moves
        if (dInventoryMovesQueued > 255) {
            dInventoryMovesQueued = 255;
        }

        //Auto-select a specific item (unused feature)
        if (sAutoSelectItemIdx != NO_ITEM) {
            sMenuSelectedItemIdx = sAutoSelectItemIdx;
        }

        //Move between items
        if ((dInventoryMovesQueued > 0) && (sInventoryScrollOffset == 0)) {
            dInventoryMovesQueued--;

            if (sDisplayedItemCount > 1) {
                //When wrapping to top, skip over a gap in the inventory in specific cases (2 or 4 items)
                if (((sDisplayedItemCount == 2) && (sMenuSelectedItemIdx == 1)) || 
                    ((sDisplayedItemCount == 4) && (sMenuSelectedItemIdx == 3))
                ) {
                    //@bug: causes visual pop: should be MENU_ITEM_HEIGHT
                    sInventoryScrollOffset = 2 * MENU_ITEM_WIDTH;
                    dInventoryMoveSpeed = 2;
                    dInventoryIsScrolling = FALSE;
                } else {
                    //@bug: causes visual pop: should be MENU_ITEM_HEIGHT
                    sInventoryScrollOffset = MENU_ITEM_WIDTH;
                    dInventoryMoveSpeed = 2;
                    dInventoryIsScrolling = FALSE;
                }

                //Select next item
                sMenuSelectedItemIdx++;

                //Wrap selection back to top of item list
                if (sMenuSelectedItemIdx >= sDisplayedItemCount) {
                    sMenuSelectedItemIdx = 0;
                }
            }

        //Close the inventory with the B button
        } else if (sJoyPressedButtons & B_BUTTON) {
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_28C_Cmdmenu_Close, MAX_VOLUME, NULL, NULL, 0, NULL);
            cmdmenu_close_inventory();

        //Use an inventory item with the A button
        } else if ((sJoyPressedButtons & A_BUTTON) && cmdmenu_is_inventory_open()) {
            usedGamebit = sMenuItemGamebits[sMenuSelectedItemIdx];
            
            //Items/Spells
            if (isSidekickMenu == FALSE) {
                obj_send_mesg(player, pageMsg, NULL, (void*)usedGamebit);
                
                sUsedItemGamebitID = usedGamebit;
                sUsedItemSoundType = sMenuItemUseSounds[sMenuSelectedItemIdx];
                sUsedItemPageID = sMenuItemOpenPageIDs[sMenuSelectedItemIdx];
                gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_28B_Cmdmenu_Use, MAX_VOLUME, NULL, NULL, 0, NULL);
                cmdmenu_close_inventory();
                
            //Sidekick commands
            } else {
                if (sMenuItemVisibilities[sMenuSelectedItemIdx]) {
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_28B_Cmdmenu_Use, MAX_VOLUME, NULL, NULL, 0, NULL);
                    cmdmenu_close_inventory();
                    sUsedItemGamebitID = usedGamebit;
                    sUsedItemSoundType = CMDMENU_SOUND_NONE;
                } else {
                    /*  If the item's gamebitHidden is set, play a refusal sound and keep the menu open.
                        
                        This feature is unused in practice, because loaded Sidekick Commands 
                        always have their sMenuItemVisibilities set to TRUE.
                        
                        Possibly intended for Spells instead, since they're the only kind of 
                        inventory item that can have their sMenuItemVisibilities set to FALSE?
                    */
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_A0_Cmdmenu_Item_Locked, MAX_VOLUME, NULL, NULL, 0, NULL);
                    sUsedItemGamebitID = NO_GAMEBIT;
                    sUsedItemSoundType = CMDMENU_SOUND_NONE;
                }
            }
        }
    }

    if (cmdmenu_is_inventory_closed()) {
        dPageCategory = 0;
        sInventoryFrameCounter = 0;
        dInventoryMovesQueued = 0;
    } else {
        joy_set_button_mask(0, A_BUTTON | B_BUTTON);
    }

    *pageSelectionIndex = sMenuSelectedItemIdx;
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
/**
  * Draws the main inventory scroll and its page icon. 
  *
  * Also draws the active spell and active sidekick command icons.
  * 
  * Calls drawing the info pop-up (cmdmenu_info_draw) and the energy bar (cmdmenu_draw_energy_bar).
  *
  * (NOTE: The top/bottom parts of the scroll aren't drawn here, 
  *        for some reason they're in `cmdmenu_draw_c_buttons_and_sidekick_meter`!)
  */
static void cmdmenu_draw_main(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    s16 commandTexTableID;
    Object* player;
    s32 activeSpellGamebit;
    s32 stripY;
    s32 spellTexTableID;
    s32 numSlotsAboveSelected;
    s8 slot[MAX_LOADED_ITEMS];
    s32 itemIdx;
    s32 i;
    s32 sideCommandIndex;
    s32 tileCount;
    u8 iconOpacity;
    u8 pageIcon;
    s8 offsetX;
    s8 offsetY;
    Object* sidekick;

    player = get_player();
    offsetX = 0;
    offsetY = 0;
    sidekick = get_sidekick();

    //Call the item info pop-up's draw
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
            rcp_screen_full_write(gdl, sActiveSpellRing, ACTIVE_SPELL_X,      ACTIVE_SPELL_Y,      0, 0, 0xFF, SCREEN_WRITE_TRANSLUCENT);
            rcp_screen_full_write(gdl, sActiveSpellIcon, ACTIVE_SPELL_ICON_X, ACTIVE_SPELL_ICON_Y, 0, 0, 0xFF, SCREEN_WRITE_TRANSLUCENT);
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
            rcp_screen_full_write(gdl, sActiveSidekickCommandRing, ACTIVE_SIDECOMMAND_X,      ACTIVE_SIDECOMMAND_Y,      0, 0, 0xFF, SCREEN_WRITE_TRANSLUCENT);
            rcp_screen_full_write(gdl, sActiveSidekickCommandIcon, ACTIVE_SIDECOMMAND_ICON_X, ACTIVE_SIDECOMMAND_ICON_Y, 0, 0, 0xFF, SCREEN_WRITE_TRANSLUCENT);
        }
    }

    //Call the energy bar's draw
    cmdmenu_draw_energy_bar(gdl);

    //Draw the inventory's vertical icon strip
    //(i.e. every part of the scroll except its top/bottom rolls)
    if (sDisplayedItemCount != 0) {
        if (dInventoryOpacity != 0) {
            /* 
                Figure out the number of tiles to draw (minimum of 3).

                Empty tiles are inserted to pad out the strip when 
                there're only 1/2/4 items shown on the page.
            */
            tileCount = 3;
            if (sDisplayedItemCount <= 3) {
                numSlotsAboveSelected = 1;
            } else {
                tileCount = 5;
                numSlotsAboveSelected = 2;
            }

            //Set a scissor mask for the inner strip of the inventory scroll
            cmdmenu_gfx_set_scroll_scissor(gdl);

            sTempIcon->y = 0;

            //Calculate the screen Y-coord of the top of the tile strip
            stripY = MENU_SCROLL_CENTRE_Y - (tileCount * (MENU_ITEM_HEIGHT/2));

            sTempIcon[1].tex = NULL;

            //Set the slots that show icons
            for (i = 0; i < sDisplayedItemCount; i++) {
                slot[i] = SLOT_OCCUPIED;
            }
            //Set the padded slots (empty BG tiles)
            for (i = sDisplayedItemCount; i < tileCount; i++) {
                slot[i] = SLOT_PADDED;
            }

            //Change sDisplayedItemCount so it's at least the size of the tile strip (including empty slots)
            if (sDisplayedItemCount < tileCount) {
                sDisplayedItemCount = tileCount;
            }

            //While animating moving between items
            {
                //Shift the top of the strip up by 1 item
                if (sInventoryScrollOffset > 0) {
                    numSlotsAboveSelected++;
                    stripY -= MENU_ITEM_HEIGHT;
                    tileCount++;
                }
                //Or shift it up by 2 during large offsets (when wrapping from bottom-to-top)
                if (sInventoryScrollOffset > MENU_ITEM_HEIGHT) {
                    numSlotsAboveSelected++;
                    stripY -= MENU_ITEM_HEIGHT;
                    tileCount++;
                }
            }

            //Have the strip move with the bottom of the scroll (during its expanding/collapsing animation)
            stripY += sInventoryUnrollY - dInventoryUnrollMax;

            //Calculate the item index of the uppermost slot in the strip
            itemIdx = sMenuSelectedItemIdx - numSlotsAboveSelected;
            if (itemIdx < 0) {
                itemIdx += sDisplayedItemCount;
            }

            //Iterate down the strip, drawing the slots' tiles
            for (i = 0; i < tileCount; i++) {
                if (slot[itemIdx] == SLOT_OCCUPIED) {
                    sTempIcon->animProgress = 0;
                    iconOpacity = dInventoryOpacity;

                    //Shift the selected item's tile when scrolling's finished (does nothing: cases identical)
                    //May suggest Rare considered lifting/popping out the selected item slightly
                    if ((itemIdx == sMenuSelectedItemIdx) && (sInventoryScrollOffset == 0)) {
                        sTempIcon->x = 0;
                        sTempIcon->y = 0;
                    } else {
                        sTempIcon->x = 0;
                        sTempIcon->y = 0;
                    }

                    if (sMenuItemVisibilities[itemIdx] != FALSE) {
                        sTempIcon->tex = sMenuItemTextures[itemIdx];

                        //Draw icon
                        if (func_80041E08()) {
                            //Widescreen aspect
                            rcp_tile_write(
                                gdl, 
                                sTempIcon, 
                                MENU_ITEM_X - 1, 
                                stripY + (i * MENU_ITEM_HEIGHT) + sInventoryScrollOffset, 
                                0xFF, 0xFF, 0xFF, iconOpacity
                            );
                        } else {
                            //Standard aspect
                            rcp_tile_write(
                                gdl, 
                                sTempIcon, 
                                MENU_ITEM_X, 
                                stripY + (i * MENU_ITEM_HEIGHT) + sInventoryScrollOffset, 
                                0xFF, 0xFF, 0xFF, iconOpacity
                            );
                        }

                        //Draw quantity text (for stackable items)
                        if (sMenuItemQuantities[itemIdx] > 1) {
                            sTempIcon->tex = sInventoryStackNumbersTex;
                            sTempIcon->animProgress = (sMenuItemQuantities[itemIdx] - 2) << 8; //Numbers only shown from 2 onwards (up to 10)
                            rcp_tile_write(
                                gdl, 
                                sTempIcon, 
                                MENU_ITEM_QUANTITY_X, 
                                stripY + (i * MENU_ITEM_HEIGHT) + sInventoryScrollOffset + MENU_ITEM_QUANTITY_OFFSET_Y, 
                                0xFF, 0xFF, 0xFF, 0xFF
                            );
                        }
                    }
                } else {
                    //Draw empty tile
                    if (func_80041E08()) {
                        //Widescreen aspect
                        rcp_tile_write(
                            gdl, 
                            sTextureTiles[CMDMENU_TEX_00_Scroll_BG], 
                            MENU_ITEM_X - 1, 
                            stripY + (i * MENU_ITEM_HEIGHT) + sInventoryScrollOffset, 
                            0xFF, 0xFF, 0xFF, 0xFF
                        );
                    } else {
                        //Standard aspect
                        rcp_tile_write(
                            gdl, 
                            sTextureTiles[CMDMENU_TEX_00_Scroll_BG], 
                            MENU_ITEM_X, 
                            stripY + (i * MENU_ITEM_HEIGHT) + sInventoryScrollOffset, 
                            0xFF, 0xFF, 0xFF, 0xFF
                        );
                    }
                }

                //Increment/wrap the item index
                itemIdx++;
                if (itemIdx >= sDisplayedItemCount) {
                    itemIdx -= sDisplayedItemCount;
                }
            }

            //Draw a selection square around the currently highlighted item
            rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_31_Highlight_Corner_Top_Left],     ITEM_HL_X1, (sInventoryUnrollY - dInventoryUnrollMax) + ITEM_HL_Y1, 255, 255, 255, dInventoryOpacity);
            rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_32_Highlight_Corner_Top_Right],    ITEM_HL_X2, (sInventoryUnrollY - dInventoryUnrollMax) + ITEM_HL_Y1, 255, 255, 255, dInventoryOpacity);
            rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_33_Highlight_Corner_Bottom_Left],  ITEM_HL_X1, (sInventoryUnrollY - dInventoryUnrollMax) + ITEM_HL_Y2, 255, 255, 255, dInventoryOpacity);
            rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_34_Highlight_Corner_Bottom_Right], ITEM_HL_X2, (sInventoryUnrollY - dInventoryUnrollMax) + ITEM_HL_Y2, 255, 255, 255, dInventoryOpacity);
            
            //Restore full-screen scissor
            cmdmenu_gfx_set_screen_scissor(gdl);
        }

        //Get page icon (Bag/SpellBook/Kyte/Tricky)
        if (dInventoryShow || 
            dInventoryOpacity == MAX_OPACITY || 
            (dInventoryOpacity != 0 && dOpacitySidekickMeter == 0)
        ) {
            switch (sInventoryPageID) {
            case CMDMENU_PAGE_7_Sidekick_Tricky:
                pageIcon = CMDMENU_TEX_42_Tricky;
                offsetY = 3;
                break;
            case CMDMENU_PAGE_8_Sidekick_Kyte:
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

            if (dOpacitySidekickMeter < dInventoryOpacity) {
                iconOpacity = dInventoryOpacity;
            } else {
                iconOpacity = dOpacitySidekickMeter;
            }
        } else {
            //Show sidekick's icon when the sidekick meter should be visible
            if (dOpacitySidekickMeter != 0) {
                pageIcon = CMDMENU_TEX_42_Tricky;
                if (sidekick != NULL && sidekick->id == OBJ_Kyte) {
                    pageIcon = CMDMENU_TEX_54_Kyte;
                    iconOpacity = dOpacitySidekickMeter;
                } else {
                    offsetY = 3;
                    iconOpacity = dOpacitySidekickMeter;
                }
            } else {
                iconOpacity = 0;
            }
        }

        //Draw page icon
        if (iconOpacity) {
            dInventoryPageIcon = tex_load_deferred(dTextableIDs[pageIcon]);
            rcp_screen_full_write(
                gdl, 
                dInventoryPageIcon, 
                PAGE_ICON_X + offsetX,
                PAGE_ICON_Y + offsetY,
                0, 
                0, 
                iconOpacity, 
                SCREEN_WRITE_TRANSLUCENT
            );
            tex_free(dInventoryPageIcon);
        }
    }
}

// offset: 0x325C | func: 21
/**
  * Sets up a given menu page's visible items, loading their textures and storing their quantities, textureIDs, gamebits.
  *
  * Returns how many items will be shown on the page.
  *
  * For sidekick command pages: an item will only be shown if the sidekick is loaded, and
  * the command is unhidden (e.g. near a Find target)
  */
static s32 cmdmenu_page_load_items(InventoryItem* items, s8 isSidekickMenu) {
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
        prevMenuSidekickCommandTextures[i] = sMenuItemTexturesSidekick[i];

        //Reset arrays
        sMenuItemTextures[i] = NULL;
        sMenuItemTextureIDs[i] = NO_TEXTURE;
        sMenuItemQuantities[i] = 1;
        sMenuItemTexturesSidekick[i] = NULL;
    }

    loadIdx = 0;
    i = 0;

    //Items/Spells
    if (isSidekickMenu == FALSE) {
        sAutoSelectItemIdx = NO_ITEM;
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

                    cmdmenu_store_loaded_item_metadata(items, loadIdx, i);

                    //Store item's visibility state (Spells never get taken away, though)
                    if ((items[i].gamebitHide <= NO_GAMEBIT) || (main_get_bits(items[i].gamebitHide) == FALSE)) {
                        sMenuItemVisibilities[loadIdx] = TRUE;
                    } else {
                        sMenuItemVisibilities[loadIdx] = FALSE;
                    }

                    loadIdx++;

                //Other items
                } else {
                    //Only load non-magic items if they aren't used/hidden
                    if ((items[i].gamebitHide <= NO_GAMEBIT) || (main_get_bits(items[i].gamebitHide) == FALSE)) {
                        
                        /* Auto-select a specified item?
                          (Never runs, since `sAutoSelectItemGamebit` is always -1 (NO_GAMEBIT) and 
                           no inventory items have `NO_GAMEBIT` as their `gamebitObtained`)
                        */
                        if ((sAutoSelectItemGamebit != 0) && (sAutoSelectItemGamebit == items[i].gamebitObtained)) {
                            sAutoSelectItemIdx = loadIdx;
                        }

                        sMenuItemTextures[loadIdx] = tex_load_deferred(items[i].textureID);
                        sMenuItemTextureIDs[loadIdx] = items[i].textureID;
                        sMenuItemGamebits[loadIdx] = items[i].gamebitObtained;

                        //Cap the displayed quantity at 10
                        if (obtained > 10) {
                            obtained = 10;
                        }
                        sMenuItemQuantities[loadIdx] = obtained;

                        cmdmenu_store_loaded_item_metadata(items, loadIdx, i);

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
            while (items[i].command.flag >= 0) {
                //Only load a sidekick command item if it's currently available
                if (items[i].command.flag & availableCommands) {
                    sMenuItemTextures[loadIdx] = tex_load_deferred(items[i].textureID);
                    sMenuItemQuantities[loadIdx] = 1;

                    /* Load secondary sidekick texture
                       NOTE: not used/drawn, and always the same as the command's `.textureID`.
                       Maybe it's supposed to hold the alternate transparent texture for the active sidekick command bubble?
                    */
                    if (items[i].command.sidekickTextureID != NO_TEXTURE) {
                        sMenuItemTexturesSidekick[loadIdx] = tex_load_deferred(items[i].command.sidekickTextureID);
                    } else {
                        sMenuItemTexturesSidekick[loadIdx] = NULL;
                    }

                    sMenuItemGamebits[loadIdx] = items[i].command.index;
                    cmdmenu_store_loaded_item_metadata(items, loadIdx, i);
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
/**
  * Stores interaction-related data for a given loaded inventory item:
  *
  * - textID (to show when R is held)
  * - pageID (to open when used)
  * - sound type (to play when used)
  */
static void cmdmenu_store_loaded_item_metadata(InventoryItem* items, s32 loadedItemIndex, s32 itemIndex) {
    sMenuItemTextIDs[loadedItemIndex] = items[itemIndex].textID;
    sMenuItemOpenPageIDs[loadedItemIndex] = items[itemIndex].openPage;
    sMenuItemUseSounds[loadedItemIndex] = items[itemIndex].sound;
}

// offset: 0x38E4 | func: 24
static void cmdmenu_gfx_set_texture(Gfx** gdl, Texture* tex, s32 frame) {
    Gfx* dl;
    s32 i;

    dl = *gdl;

    //Get the animation frame
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
static int cmdmenu_is_inventory_open(void) {
    //Check if the inventory shouldn't be shown
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
/**
  * Checks if the inventory scroll menu is fully closed.
  */
static int cmdmenu_is_inventory_closed(void) {
    //Check if the inventory should be shown
    if (dInventoryShow) {
        return FALSE;
    }

    //Check if the scroll isn't fully closed
    if (sInventoryUnrollY != 0) {
        return FALSE;
    }

    return TRUE;
}

// offset: 0x3A94 | func: 27
/**
  * Causes the inventory scroll to close.
  */
static void cmdmenu_close_inventory(void) {
    dInventoryShow = FALSE;
}

// offset: 0x3AB0 | func: 28
/**
  * Causes the inventory scroll to open.
  */
static void cmdmenu_open_inventory(void) {
    dInventoryShow = TRUE;
}

// offset: 0x3AD0 | func: 29
/**
  * Updates the inventory's animation:
  * - Fading in/out
  * - Expanding/collapsing page
  * - Scrolling between items
  *
  * Also manages unused inventory frame counters.
  */
static void cmdmenu_inventory_animate(void) {
    //Animate moving to the next inventory item
    sInventoryScrollOffset -= gUpdateRate << dInventoryMoveSpeed;
    if (sInventoryScrollOffset < 0) {
        sInventoryScrollOffset = 0;
    }

    //Manage the inventory scroll's opacity
    if (dInventoryShow) {
        //Fade in when opened
        dInventoryOpacity += gUpdateRate * 8;
        if (dInventoryOpacity > MAX_OPACITY) {
            dInventoryOpacity = MAX_OPACITY;
        }
    } else if (sInventoryUnrollY == 0) {
        //Fade out when fully closed
        dInventoryOpacity -= gUpdateRate * 8;
        if (dInventoryOpacity < 0) {
            dInventoryOpacity = 0;
        }
    }

    //Animate the inventory scroll expanding/collapsing
    if (dInventoryShow && (dInventoryOpacity > 64)) {
        //Expand/unfurl scroll when inventory opacity partially faded in
        sInventoryUnrollY += gUpdateRate * 4;
        if (sInventoryUnrollY > dInventoryUnrollMax) {
            sInventoryUnrollY = dInventoryUnrollMax;
        } else {
            //Increment/decrement roll frames while inventory is expanding
            dInventoryFrameTop++;
            dInventoryFrameBottom--;
        }
    } else {
        sInventoryUnrollY -= gUpdateRate * 4;
        if (sInventoryUnrollY < 0) {
            sInventoryUnrollY = 0;
        } else {
            //Decrement/increment roll frames while inventory is collapsing
            dInventoryFrameTop--;
            dInventoryFrameBottom++;
        }
    }

    if (dInventoryOpacity) {
        //Increment roll frames while the inventory is scrolling
        if (sInventoryScrollOffset > 0) {
            dInventoryFrameTop++;
            dInventoryFrameBottom++;
        }
        
        //Update frame counters, keeping them in 0-2 range (effectively `dInventoryFrame %= 3`)
        while (dInventoryFrameTop >= 3) {
            dInventoryFrameTop -= 3;
        }
        while (dInventoryFrameTop < 0) {
            dInventoryFrameTop += 3;
        }
        while (dInventoryFrameBottom >= 3) {
            dInventoryFrameBottom -= 3;
        }
        while (dInventoryFrameBottom < 0) {
            dInventoryFrameBottom += 3;
        }
    }
}

// offset: 0x3D28 | func: 30
/**
  * Draws the C button icons, and the sidekick food meter.
  *
  * The top/bottom of the inventory scroll is also drawn here for some reason!
  */
static void cmdmenu_draw_c_buttons_and_sidekick_meter(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    Gfx* dl;
    Object* sidekick;
    Object* player;
    u8 texIdx;
    u8 pad;
    u8 isKyte;
    u8 pageIdx;
    u8 hasHalfRed;
    u8 fullBlueEnd;
    u8 hasHalfBlue;
    u8 cIconFlags;
    u8 fullRedEnd;
    u8 i;
    u8 iconIndex;

    sidekick = get_sidekick();
    player = get_player();
    isKyte = FALSE;
    cIconFlags = CIcon_FLAG_None;

    if (sidekick != NULL) {
        isKyte = (sidekick->id == OBJ_Kyte);
    }

    //Draw C buttons (only while the inventory scroll is hidden)
    if (dInventoryOpacity == 0) {
        if (sOpacityR != 0.0f) {
            //Check whether each C button has items to show
            {
                //Check if player has inventory items
                if (player != NULL) {
                    pageIdx = player->id == OBJ_Krystal ? CMDMENU_PAGE_0_Items_Krystal : CMDMENU_PAGE_1_Items_Sabre;
                    cIconFlags = CIcon_FLAG_None;
                    if (cmdmenu_page_count_shown_items(dCmdmenuPages[pageIdx].items, FALSE) != 0) {
                        cIconFlags = CIcon_FLAG_Have_Items;
                    }
                }

                //Check if Sidekick Commands are available
                if (sidekick != NULL) {
                    pageIdx = sidekick->id == OBJ_Kyte ? CMDMENU_PAGE_8_Sidekick_Kyte : CMDMENU_PAGE_7_Sidekick_Tricky;
                    if (cmdmenu_page_count_shown_items(dCmdmenuPages[pageIdx].items, TRUE) != 0) {
                        cIconFlags |= CIcon_FLAG_Have_Sidekick_Commands;
                    }
                }

                //Check if Spells are available
                if (cmdmenu_page_count_shown_items(dCmdmenuPages[CMDMENU_PAGE_6_Spells].items, FALSE) != 0) {
                    cIconFlags |= CIcon_FLAG_Have_Spells;
                }
            }

            //Draw C-right button
            {
                if (cIconFlags & CIcon_FLAG_Have_Items) {
                    //With inventory bag
                    texIdx = CMDMENU_TEX_47_RightButton_With_Bag;
                    dInventoryPageIcon = tex_load_deferred(dTextableIDs[texIdx]);
                    rcp_screen_full_write(gdl, 
                        dInventoryPageIcon, 
                        C_BUTTONS_RIGHT_BAG_X, 
                        C_BUTTONS_RIGHT_BAG_Y, 
                        0, 
                        0, 
                        sOpacityR, 
                        SCREEN_WRITE_TRANSLUCENT
                    );
                    tex_free(dInventoryPageIcon);
                } else {
                    //Empty C-right button
                    texIdx = CMDMENU_TEX_41_C_Right;
                    rcp_tile_write(
                        gdl,
                        sTextureTiles[texIdx], 
                        C_BUTTONS_RIGHT_EMPTY_X, 
                        C_BUTTONS_RIGHT_EMPTY_Y, 
                        255, 255, 255, sOpacityR
                    );
                }
            }

            //Draw C-left and C-down buttons
            {
                if (((cIconFlags & CIcon_FLAG_Have_Sidekick_Commands) && sidekick) || 
                    (cIconFlags & CIcon_FLAG_Have_Spells)
                ) {
                    //If Sidekick Commands AND Spells are available
                    if ((cIconFlags & CIcon_FLAG_Have_Sidekick_Commands) && sidekick && (cIconFlags & CIcon_FLAG_Have_Spells)) {
                        //Show the SpellBook on C-left and Kyte/Tricky on C-down
                        if (isKyte) {
                            texIdx = CMDMENU_TEX_38_LeftDownButtons_SpellBook_With_Kyte;
                        } else {
                            texIdx = CMDMENU_TEX_43_LeftDownButtons_SpellBook_With_Tricky;
                        }
                    } else if (cIconFlags & CIcon_FLAG_Have_Items) { //@bug: should be checking `CIcon_FLAG_Have_Spells`?
                        //Show the SpellBook on C-left and nothing on C-down
                        texIdx = CMDMENU_TEX_48_LeftDownButtons_SpellBook_NoSidekick;
                    }
                    /* @bug: texIdx could end up reusing the C-right section's value here if neither condition is met
                       (i.e. have commands but not spells and items, or have spells but not commands and items) 
                    */

                    dInventoryPageIcon = tex_load_deferred(dTextableIDs[texIdx]);
                    rcp_screen_full_write(
                        gdl, 
                        dInventoryPageIcon, 
                        C_BUTTONS_LEFT_DOWN_BOOK_SIDEKICK_X, 
                        C_BUTTONS_LEFT_DOWN_BOOK_SIDEKICK_Y, 
                        0, 
                        0, 
                        sOpacityR, 
                        SCREEN_WRITE_TRANSLUCENT
                    );
                    tex_free(dInventoryPageIcon);
                } else {
                    //Draw empty C-down and C-left buttons
                    rcp_tile_write(
                        gdl,
                        sTextureTiles[CMDMENU_TEX_37_C_Down], 
                        C_BUTTONS_DOWN_EMPTY_X, 
                        C_BUTTONS_DOWN_EMPTY_Y, 
                        255, 255, 255, sOpacityR
                    );
                    rcp_tile_write(
                        gdl, 
                        sTextureTiles[CMDMENU_TEX_39_C_Left], 
                        C_BUTTONS_LEFT_EMPTY_X, 
                        C_BUTTONS_LEFT_EMPTY_Y, 
                        255, 255, 255, sOpacityR
                    );
                }
            }
        }
    } else {
        sOpacityR = 0.0f;
    }

    dl = *gdl;

    //Draw the top/bottom of the inventory scroll
    /* @bug: why is this handled in the C-button/sidekick-meter function?
             Should likely be in `cmdmenu_draw_main` instead, and maybe ended up here by mistake!
     */
    if (dInventoryOpacity != 0) {
        rcp_tile_write(&dl, sTextureTiles[CMDMENU_TEX_02_Scroll_Top],    MENU_SCROLL_X, MENU_SCROLL_TOP_Y,                        255, 255, 255, dInventoryOpacity);
        rcp_tile_write(&dl, sTextureTiles[CMDMENU_TEX_01_Scroll_Bottom], MENU_SCROLL_X, MENU_SCROLL_BOTTOM_Y + sInventoryUnrollY, 255, 255, 255, dInventoryOpacity);
    }

    //Draw the sidekick food meter
    if (sidekick != NULL) {
        //Handle the meter's opacity
        if (((sInventoryPageID == CMDMENU_PAGE_7_Sidekick_Tricky || sInventoryPageID == CMDMENU_PAGE_8_Sidekick_Kyte) && dInventoryOpacity) || 
            (sStatsChangeTimers.sidekickBlueFood >= 0.0f)
        ) {
            /* Fade in while the sidekick's inventory page is open, 
               or when the sidekick's blue food count has recently changed */
            dOpacitySidekickMeter += gUpdateRate * 8;
            if (dOpacitySidekickMeter > MAX_OPACITY) {
                dOpacitySidekickMeter = MAX_OPACITY;
            }
        } else {
            //Otherwise fade out
            dOpacitySidekickMeter -= gUpdateRate * 8;
            if (dOpacitySidekickMeter < 0) {
                dOpacitySidekickMeter = 0;
            }
        }

        if (dOpacitySidekickMeter != 0) {
            //Get sidekick's red/blue food counts (@unfinished: hard-coded quantities)
            sStats.sidekickRedFood = 3;
            sStats.sidekickBlueFood = 3;

            //Determine the values to show on the meter (2 food units per icon)
            fullRedEnd = sStats.sidekickRedFood >> 1;
            hasHalfRed = sStats.sidekickRedFood & 1;
            fullBlueEnd = (sStats.sidekickBlueFood >> 1) + fullRedEnd + hasHalfRed;
            hasHalfBlue = sStats.sidekickBlueFood & 1;

            //Draw the sidekick's food icons
            for (i = 0; i < sStats.sidekickMaxFood; i++) {
                //Draw red food first (full icons)
                if (i < fullRedEnd) {
                    if (isKyte) {
                        iconIndex = CMDMENU_TEX_56_Grub_Red_Full;
                    } else {
                        iconIndex = CMDMENU_TEX_45_Mushroom_Red_Full;
                    }
                //Draw red food's remainder second (half icon)
                } else if (i == fullRedEnd && hasHalfRed) {
                    if (isKyte) {
                        iconIndex = CMDMENU_TEX_57_Grub_Red_Half;
                    } else {
                        iconIndex = CMDMENU_TEX_46_Mushroom_Red_Half;
                    }
                //Draw blue food third (full icons)
                } else if (i < fullBlueEnd) {
                    if (isKyte) {
                        iconIndex = CMDMENU_TEX_13_Grub_Blue_Full;
                    } else {
                        iconIndex = CMDMENU_TEX_12_Mushroom_Blue_Full;
                    }
                //Draw blue food's remainder last (half icon)
                } else if (i == fullBlueEnd && hasHalfBlue) {
                    if (isKyte) {
                        iconIndex = CMDMENU_TEX_16_Grub_Blue_Half;
                    } else {
                        iconIndex = CMDMENU_TEX_51_Mushroom_Blue_Half;
                    }
                //Pad out with empty icons
                } else {
                    if (isKyte) {
                        iconIndex = CMDMENU_TEX_55_Grub_Empty;
                    } else {
                        iconIndex = CMDMENU_TEX_44_Mushroom_Empty;
                    }
                }
                
                //Draw icons (in 2x4 grid: starting top-right to bottom-right, then top-left to bottom-left)
                rcp_tile_write(&dl, sTextureTiles[iconIndex], 
                    SIDEKICK_METER_X - ((i / SIDEKICK_METER_ICONS_PER_COLUMN) * SIDEKICK_METER_SPACING_X), 
                    SIDEKICK_METER_Y + ((i % SIDEKICK_METER_ICONS_PER_COLUMN) * SIDEKICK_METER_SPACING_Y), 
                    255, 255, 255, dOpacitySidekickMeter
                );
            }
        }
    }

    dl_set_prim_color(&dl, 255, 255, 255, 255);

    *gdl = dl;
}

// offset: 0x4630 | func: 31
/**
  * Sets a scissor mask for the inner strip of the inventory scroll.
  */
static void cmdmenu_gfx_set_scroll_scissor(Gfx **gdl) {
    if (func_80041E08()) {
        //Widescreen aspect
        gDPSetScissorFrac((*gdl)++, G_SC_NON_INTERLACE, 
            qu102(314.75), 
            qu102(48.25), 
            qu102(366.75), 
            ((sInventoryUnrollY * 0.82f) + 48.38f + 2.0f) * 4.0f);
    } else {
        //Standard aspect
        gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 
            MENU_ITEM_X, 
            MENU_ITEM_Y, 
            MENU_ITEM_X + MENU_ITEM_WIDTH, 
            sInventoryUnrollY + MENU_ITEM_Y + 2);
    }
}

// offset: 0x474C | func: 32
/**
  * Sets a full-screen scissor mask.
  */
static void cmdmenu_gfx_set_screen_scissor(Gfx** gdl) {
    s32 resolution;

    resolution = vi_get_current_size();
    gDPSetScissor((*gdl)++, 0 , 0, 0, GET_VIDEO_WIDTH(resolution), GET_VIDEO_HEIGHT(resolution));
}

// offset: 0x47E8 | func: 33
/**
  * Checks if the info scroll is fully closed.
  */
static int cmdmenu_is_info_scroll_closed(void) {
    //Check if the info scroll should be shown
    if (dInfoScrollShow) {
        return FALSE;
    }

    //Check if the scroll isn't fully closed
    if (sInfoScrollUnrollY != 0) {
        return FALSE;
    }

    return TRUE;
}

// offset: 0x4830 | func: 34
/**
  * Causes the inventory/target info scroll to close.
  */
static void cmdmenu_close_info_scroll(void) {
    dInfoScrollShow = FALSE;
}

// offset: 0x484C | func: 35
/**
  * Causes the inventory/target info scroll to open.
  */
static void cmdmenu_open_info_scroll(void) {
    dInfoScrollShow = TRUE;
}

// offset: 0x486C | func: 36
/**
  * Updates the info scroll's animation:
  * - Fading in/out
  * - Expanding/collapsing
  *
  * Also manages unused info scroll frame counters.
  */
static void cmdmenu_info_scroll_animate(void) {
    //Update the info scroll's opacity
    if (dInfoScrollShow && (dInfoScrollDisabled == FALSE)) {
        //Fade in
        dInfoScrollOpacity += gUpdateRate * INFO_SCROLL_OPACITY_SPEED;
        if (dInfoScrollOpacity > INFO_SCROLL_OPACITY_MAX) {
            dInfoScrollOpacity = INFO_SCROLL_OPACITY_MAX;
        }
    } else if (sInfoScrollUnrollY == 0) {
        //Fade out once the scroll's closed
        dInfoScrollOpacity -= gUpdateRate * INFO_SCROLL_OPACITY_SPEED;
        if (dInfoScrollOpacity < 0) {
            dInfoScrollOpacity = 0;
        }
    }

    //Animate the scroll expanding/collapsing
    if (dInfoScrollShow && (dInfoScrollOpacity == INFO_SCROLL_OPACITY_MAX)) {
        //Expand scroll once fully faded in
        sInfoScrollUnrollY += gUpdateRate * 4;
        if (sInfoScrollUnrollY > dInfoScrollUnrollMax) {
            sInfoScrollUnrollY = dInfoScrollUnrollMax;
        } else {
            //Increment/decrement roll frames while info scroll is expanding
            dInfoScrollFrameTop++;
            dInfoScrollFrameBottom--;
        }
    } else {
        //Collapse scroll before fading out
        sInfoScrollUnrollY -= gUpdateRate * 4;
        if (sInfoScrollUnrollY < 0) {
            sInfoScrollUnrollY = 0;
        } else {
            //Decrement/increment roll frames while info scroll is collapsing
            dInfoScrollFrameTop--;
            dInfoScrollFrameBottom++;
        }
    }

    if (dInfoScrollOpacity == 0) {
        //Free the info scroll's strings when fully faded out
        if (*dInfoScrollStrings != NULL) {
            mmFree(*dInfoScrollStrings);
            *dInfoScrollStrings = NULL;
        }
    } else {
        //Update roll frame counters, keeping them in 0-2 range (effectively `dInfoScrollFrame %= 3`)
        while (dInfoScrollFrameTop >= 3) {
            dInfoScrollFrameTop -= 3;
        }
        while (dInfoScrollFrameTop < 0) {
            dInfoScrollFrameTop += 3;
        }
        while (dInfoScrollFrameBottom >= 3) {
            dInfoScrollFrameBottom -= 3;
        }
        while (dInfoScrollFrameBottom < 0) {
            dInfoScrollFrameBottom += 3;
        }

        //Set the frame counters to 0 anyway (since the texture only has 1 frame)
        dInfoScrollFrameTop = 0;
        dInfoScrollFrameBottom = 0;
    }
}

// offset: 0x4AD4 | func: 37
/**
  * Draws the info box at the top of the screen describing the highlighted/z-locked item.
  *
  * The info scroll can also be shown automatically (e.g. during Forcefield Spell 
  * collection cutscene) at custom screen coordinates.
  */
static void cmdmenu_draw_info_scroll(Gfx** gdl, Mtx** mtxs, Vertex** vtxs) {
    s32 x;
    s32 y;
    s32 halfWidth;
    s32 height;
    s32 tempY;
    s32 lineIdx;
    Gfx* dl;

    if (dInfoScrollOpacity == 0) {
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
    dl_set_prim_color(&dl, 255, 255, 255, dInfoScrollOpacity);

    //Get dimensions (note: coords multiplied by 4 for gSPTextureRectangle)
    x = dInfoScrollX << 2;
    halfWidth = dInfoScrollWidthHalf << 2;
    halfWidth -= INFO_SCROLL_PAGE_EDGE_WIDTH << 2; //Subtract the page's edge margin
    y = dInfoScrollY << 2;
    height = sInfoScrollUnrollY << 2;

    //Draw main paper background
    {
        //Middle section (excluding the paper's left/right edges)
        cmdmenu_gfx_set_texture(&dl, sTextures[CMDMENU_TEX_06_InfoScroll_BG], 0);
        gSPTextureRectangle(dl++, 
            /*ulx*/ x - halfWidth,
            /*uly*/ y,
            /*lrx*/ x + halfWidth,
            /*lry*/ y + height,
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(0), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
        );
        gDLBuilder->needsPipeSync = TRUE;

        //Left edge of paper
        cmdmenu_gfx_set_texture(&dl, sTextures[CMDMENU_TEX_05_InfoScroll_Side], 0);
        gSPTextureRectangle(dl++, 
            /*lrx*/ x - halfWidth - (INFO_SCROLL_PAGE_EDGE_WIDTH << 2),
            /*lry*/ y,
            /*ulx*/ x - halfWidth,
            /*uly*/ y + height,
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(0), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
        );
        gDLBuilder->needsPipeSync = TRUE;

        //Right edge of paper
        gSPTextureRectangle(dl++, 
            /*ulx*/ x + halfWidth,
            /*uly*/ y,
            /*lrx*/ x + halfWidth + (INFO_SCROLL_PAGE_EDGE_WIDTH << 2),
            /*lry*/ y + height,
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(15), /*t*/ qs105(0),
            /*dsdx*/ qs510(-1), /*dtdy*/ qs510(1)
        );
        gDLBuilder->needsPipeSync = TRUE;

        halfWidth += (INFO_SCROLL_PAGE_EDGE_WIDTH << 2); //restore half-width
    }

    //Draw the top/bottom rolls' page shadows
    if (sInfoScrollUnrollY > INFO_SCROLL_PAGE_SHADOW_HEIGHT) {
        cmdmenu_gfx_set_texture(&dl, sTextures[CMDMENU_TEX_07_InfoScroll_SelfShadow], 0);
        dl_set_prim_color(&dl, 255, 128, 128, 128);
        
        //Top shadow
        tempY = y;
        gSPTextureRectangle(dl++, 
            /*ulx*/ x - halfWidth,
            /*uly*/ tempY,
            /*lrx*/ x + halfWidth,
            /*lry*/ tempY + (INFO_SCROLL_PAGE_SHADOW_HEIGHT << 2),
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(7), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
        );
        gDLBuilder->needsPipeSync = TRUE;

        //Bottom shadow
        tempY = y + height - ((INFO_SCROLL_PAGE_SHADOW_HEIGHT - 2) << 2);
        gSPTextureRectangle(dl++, 
            /*ulx*/ x - halfWidth,
            /*uly*/ tempY,
            /*lrx*/ x + halfWidth,
            /*lry*/ tempY + (INFO_SCROLL_PAGE_SHADOW_HEIGHT << 2),
            /*tile*/ G_TX_RENDERTILE,
            /*s*/ qs105(0), /*t*/ qs105(0), 
            /*dsdx*/ qs510(1), /*dtdy*/ qs510(1)
        );
        gDLBuilder->needsPipeSync = TRUE;

        //Restore prim colour
        dl_set_prim_color(&dl, 255, 255, 255, dInfoScrollOpacity);
    }

    //Draw the top/bottom rolls
    {
        //Top roll (middle span)
        {
            tempY = y - (INFO_SCROLL_ROLL_TOP_Y_OFFSET << 2);
            cmdmenu_gfx_set_texture(
                &dl,
                sTextures[CMDMENU_TEX_04_InfoScroll_Roll], 
                dInfoScrollFrameTop //Unused/scrapped rolling animation (always set to frame 0)
            );
            gSPTextureRectangle(dl++, 
                /*ulx*/ x - halfWidth,
                /*uly*/ tempY,
                /*lrx*/ x + halfWidth,
                /*lry*/ tempY + (INFO_SCROLL_ROLL_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(0), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;
        }

        //Bottom roll (middle span)
        {
            tempY = y + height - (INFO_SCROLL_ROLL_BOTTOM_Y_OFFSET << 2);
            cmdmenu_gfx_set_texture(&dl, sTextures[CMDMENU_TEX_04_InfoScroll_Roll], dInfoScrollFrameBottom);
            gSPTextureRectangle(dl++, 
                /*ulx*/ x - halfWidth,
                /*uly*/ tempY,
                /*lrx*/ x + halfWidth,
                /*lry*/ tempY + (INFO_SCROLL_ROLL_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(0), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;
        }

        //Roll handles
        {
            cmdmenu_gfx_set_texture(
                &dl, 
                sTextures[CMDMENU_TEX_03_InfoScroll_Roll_End], 
                0
            );
            
            //Top roll handle (left)
            tempY = y - (INFO_SCROLL_ROLL_TOP_Y_OFFSET << 2);
            gSPTextureRectangle(dl++, 
                /*ulx*/ x - halfWidth - (INFO_SCROLL_HANDLE_WIDTH << 2),
                /*uly*/ tempY,
                /*lrx*/ x - halfWidth,
                /*lry*/ tempY + (INFO_SCROLL_HANDLE_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(0), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;

            //Top roll handle (right)
            gSPTextureRectangle(dl++, 
                /*ulx*/ x + halfWidth,
                /*uly*/ tempY,
                /*lrx*/ x + halfWidth + (INFO_SCROLL_HANDLE_WIDTH << 2),
                /*lry*/ tempY + (INFO_SCROLL_HANDLE_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(15), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(-1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;

            //Bottom roll handle (left)
            tempY = y + height - (INFO_SCROLL_ROLL_BOTTOM_Y_OFFSET << 2);
            gSPTextureRectangle(dl++, 
                /*ulx*/ x - halfWidth - (INFO_SCROLL_HANDLE_WIDTH << 2),
                /*uly*/ tempY,
                /*lrx*/ x - halfWidth,
                /*lry*/ tempY + (INFO_SCROLL_HANDLE_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(0), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;

            //Bottom roll handle (right)
            gSPTextureRectangle(dl++, 
                /*ulx*/ x + halfWidth,
                /*uly*/ tempY,
                /*lrx*/ x + halfWidth + (INFO_SCROLL_HANDLE_WIDTH << 2),
                /*lry*/ tempY + (INFO_SCROLL_HANDLE_HEIGHT << 2),
                /*tile*/ G_TX_RENDERTILE,
                /*s*/ qs105(15), /*t*/ qs105(15.9688), 
                /*dsdx*/ qs510(-1), /*dtdy*/ qs510(-1)
            );
            gDLBuilder->needsPipeSync = TRUE;
        }
    }

    //Restore prim colour
    dl_set_prim_color(&dl, 255, 255, 255, 255);

    //Return early if there's no gametext
    if (dInfoScrollTextID <= NO_GAMETEXT) {
        // @bug: This skips updating the gdl pointer, which desyncs the DL builder state with
        //       what's actually on the display list.
        return;
    }

    //Get the gametext lines
    if (dInfoScrollStrings[0] == NULL) {
        //Get the item's string (use a different gametext file for textIDs beyond 255) 
        dInfoScrollStrings[0] = dInfoScrollTextID >= 256 
            ? gDLL_21_Gametext->vtbl->get_text(GAMETEXT_238_UI_Text_2, dInfoScrollTextID - 256) 
            : gDLL_21_Gametext->vtbl->get_text(GAMETEXT_003_UI_Text_1, dInfoScrollTextID);
        dInfoScrollStrings[1] = NULL;
        dInfoScrollStrings[2] = NULL;
        dInfoScrollStrings[3] = NULL;

        //Check for bar delimiter in text (separates lines)
        for (x = 0, lineIdx = 1; dInfoScrollStrings[0][x] != '\0'; x++) {
            if (dInfoScrollStrings[0][x] == '|') {
                dInfoScrollStrings[0][x] = '\0';
                x += 2; //ROM's text should have space after bar ("| "), so skipping over it
                
                dInfoScrollStrings[lineIdx] = &dInfoScrollStrings[0][x]; //store address of new line
                lineIdx++;
            }            
        }
    }

    //Draw text if the scroll's open
    if (sInfoScrollUnrollY > 0) {
        font_window_set_coords(3, 
            /*x1*/ dInfoScrollX - dInfoScrollWidthHalf, 
            /*y1*/ dInfoScrollY, 
            /*x2*/ dInfoScrollX + dInfoScrollWidthHalf, 
            /*y2*/ dInfoScrollY + sInfoScrollUnrollY);
        font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
        font_window_set_bg_colour(3, 0, 0, 0, 0);
        font_window_flush_strings(3);
        font_window_set_text_colour(3, 0, 0, 255, 255, 255);

        //Print the text lines
        for (y = INFO_SCROLL_TEXT_Y, lineIdx = 0; lineIdx <= 3; lineIdx++) {
            if (dInfoScrollStrings[lineIdx] == NULL) {
                break;
            }
            font_window_add_string_xy(3, -0x8000, y, dInfoScrollStrings[lineIdx], 1, ALIGN_TOP_CENTER);
            font_window_set_text_colour(3, 20, 20, 20, 255, 255);
            font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
            y += INFO_SCROLL_LINE_HEIGHT;
        }

        font_window_draw(&dl, mtxs, vtxs, 3);
    }

    *gdl = dl;
}

// offset: 0x5608 | func: 38
/**
  * Tracks changes in the player/sidekick's stats, and handles the following:
  *
  * - Fading in the C buttons while holding R
  * - Fading in the health/magic UI while holding R / locked on / auto-shown
  * - Spinning the Scarab counter icon when a Scarab is collected
  */
static void cmdmenu_update_stats(void) {
    Object* player;
    Object* sidekick;
    CmdmenuPlayerSidekickData stats;
    f32 timer;
    u8 scarabFrameOffset;
    u8 i;

    player = get_player();
    sidekick = get_sidekick();
    scarabFrameOffset = 0;

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

    //Check if the player collected a Scarab
    if (((DLL_210_Player*)player->dll)->vtbl->get_scarabs_largest_recently_collected(player) != 0) {
        scarabFrameOffset = 7;
    }
    if (sAnimScarabSpin < scarabFrameOffset) {
        sAnimScarabSpin = scarabFrameOffset;
    }

    //Play sound when pressing R to show HUD
    if (sJoyPressedButtons & R_TRIG) {
        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_5EA_Cmdmenu_ShowHUD, MAX_VOLUME, NULL, NULL, 0, NULL);
    }

    //Increment stat.unk14 when holding R, or when there's a target Object, or when stats are auto-shown
    //(Causes health/magic to fade in and stay on-screen a little while longer than C buttons)
    if ((sJoyHeldButtons & R_TRIG) || 
        (gDLL_2_Camera->vtbl->get_target_object() != NULL) || 
        (sForceStatsDisplay && (camera_get_letterbox() == 0))
    ) {
        stats.unk14 = sPrevStats.unk14 + 1;
    } else {
        stats.unk14 = sPrevStats.unk14;
    }

    //Fade in C buttons when holding R
    if (sJoyHeldButtons & R_TRIG) {
        sOpacityR += 8.5f * gUpdateRateF;
        if (sOpacityR > MAX_OPACITY_F) {
            sOpacityR = MAX_OPACITY_F;
        }
    } else {
        sOpacityR -= 8.5f * gUpdateRateF;
        if (sOpacityR < 0.0f) {
            sOpacityR = 0.0f;
        }
    }

    //Right side's opacity shouldn't exceed health UI's opacity
    sOpacityR = (sOpacityR < sOpacityHealth) ? sOpacityR : sOpacityHealth;

    stats.unk18 = 0;

    //Update the player stats snapshot when requested
    if (sPlayerStatsFlags & PlayerStats_FLAG_Update_Snapshot) {
        sPlayerStatsFlags &= ~PlayerStats_FLAG_Update_Snapshot;
        for (i = 0; i < CMDMENU_TRACKED_PLAYER_STATS_COUNT; i++) {
            sStats.items[i] = stats.items[i];
            sPrevStats.items[i] = stats.items[i];
            sStatsChangeTimers.items[i] = -30.0f;
        }
        sOpacityHealth = 0.0f;
        return;
    }

    //Compare the current player stats and the stored player stats
    for (i = 0; i < CMDMENU_TRACKED_PLAYER_STATS_COUNT; i++) {
        //Decrease the stat's timer
        timer = sStatsChangeTimers.items[i];
        sStatsChangeTimers.items[i] = timer - gUpdateRateF;

        if (timer > 60.0f) {
            if (sStatsChangeTimers.items[i] <= 60.0f) {
                //Optionally play a sound when the stat increases/decreases (unused)
                if (sStats.items[i] < stats.items[i]) {
                    if (dStatChangeSounds.items[i].increased != NO_SOUND) {
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, dStatChangeSounds.items[i].increased, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                } else {
                    if (dStatChangeSounds.items[i].decreased != NO_SOUND) {
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, dStatChangeSounds.items[i].decreased, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                }

                sStats.items[i] = stats.items[i];
            }
        }

        //Check if the stat changed
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
    gDPSetScissor(dl++, G_SC_NON_INTERLACE, 0, 0, (u16)GET_VIDEO_WIDTH(temp) - 1, SCREEN_HEIGHT - 1);
    
    //Draw player health
    {
        if ((sStatsChangeTimers.playerHealth >= 0.0f) || 
            (sStatsChangeTimers.playerHealthMax >= 0.0f) || 
            (sStatsChangeTimers.unk14 >= 0.0f) //Shows health when pressing R
        ) {
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
                    offsetX = (i * APPLES_SPACING_X) + (APPLES_ROW_3_OFFSET_X - (APPLES_ROW_3_IDX * APPLES_SPACING_X));
                    offsetY = 2 * APPLES_SPACING_Y;
                } else if (i >= APPLES_ROW_2_IDX) {
                    //Row 2
                    offsetX = (i * APPLES_SPACING_X) + (APPLES_ROW_2_OFFSET_X - (APPLES_ROW_2_IDX * APPLES_SPACING_Y));
                    offsetY = APPLES_SPACING_Y;
                } else {
                    //Row 1
                    offsetX = (i * APPLES_SPACING_X) + (APPLES_ROW_1_OFFSET_X);
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
                    HEALTH_ICONS_X + offsetX, 
                    HEALTH_ICONS_Y + offsetY,
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
            //Draw a magic bar for every 25 units of the player's max magic (just 1 bar initially)
            for (i = 0; i < (sStats.playerMagicMax / MAGIC_UNITS_PER_BAR); i++) {
                if (i < (sStats.playerMagic / MAGIC_UNITS_PER_BAR)) {
                    //Full bar
                    temp = MAGIC_BARS_WIDTH;
                } else if ((sStats.playerMagic / MAGIC_UNITS_PER_BAR) < i) {
                    //Empty bar
                    temp = 0;
                } else {
                    //Partial bar
                    temp = MAGIC_BARS_ZERO_POINT_X + ((sStats.playerMagic % MAGIC_UNITS_PER_BAR) * 2);
                }

                //Draw the filled part of the bar
                rcp_tile_write_x(
                    &dl,
                    sTextureTiles[CMDMENU_TEX_36_MagicBar_Full],
                    MAGIC_BARS_X,
                    MAGIC_BARS_Y + (i * MAGIC_BARS_SPACING_Y),
                    temp,
                    MAGIC_BARS_HEIGHT,
                    0, 0,
                    1.0f, 1.0f,
                    statsOpacity | ~0xFF,
                    TILE_WRITE_TRANSLUCENT | TILE_WRITE_POINT_FILT
                );

                //Draw the empty part of the bar
                rcp_tile_write_x(
                    &dl, sTextureTiles[CMDMENU_TEX_35_MagicBar_Empty],
                    MAGIC_BARS_X + temp,
                    MAGIC_BARS_Y + (i * MAGIC_BARS_SPACING_Y),
                    (MAGIC_BARS_WIDTH - temp), 
                    MAGIC_BARS_HEIGHT,
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
                CHARACTER_ICON_X + offsetX,
                CHARACTER_ICON_Y + offsetY,
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
        if (statsOpacity && main_get_bits(BIT_UI_Scarab_Counter_Enabled)) {
            sAnimFrameScarab = 0;
            if (statsOpacity == MAX_OPACITY) {
                if ((i = sAnimScarabSpin)) { //@fake assignment of i
                    //Animate Scarab spinning (upon collecting Scarabs)
                    sAnimFrameScarab = 11 - sAnimScarabSpin;
                    sAnimScarabSpin--; //@framerate-dependent
                    if (sAnimScarabSpin == 0) {
                        sAnimScarabFlutterTimer = 80;
                    }
                } else {
                    //Animate Scarab fluttering wings
                    if (sAnimScarabFlutterTimer) {
                        sAnimScarabFlutterTimer--; //@framerate-dependent

                        //Change the Scarab's frame during last few ticks of the timer 
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
                SCARABS_ICON_X, 
                SCARABS_ICON_Y, 
                SCARABS_ICON_WIDTH, 
                SCARABS_ICON_HEIGTH, 
                0, 0, 
                1.0f, 1.0f, 
                statsOpacity | ~0xFF, 
                TILE_WRITE_TRANSLUCENT | TILE_WRITE_POINT_FILT
            );

            sprintf(playerScarabCountText, "%d", (int)sStats.playerScarabCount);
            font_window_set_coords(3, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            font_window_use_font(3, FONT_DINO_SUBTITLE_FONT_1);
            font_window_set_bg_colour(3, 0, 0, 0, 0);
            font_window_flush_strings(3);
            font_window_set_text_colour(3, 0xFF, 0xFF, 0xFF, 0xFF, statsOpacity);
            font_window_add_string_xy(3, SCARABS_NUMBER_X, SCARABS_NUMBER_Y, playerScarabCountText, 1, ALIGN_TOP_LEFT);
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
static void cmdmenu_info_hide(CmdmenuInfoPopup* info) {
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
static void cmdmenu_info_draw(Gfx** gdl, CmdmenuInfoPopup* box) {
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
        box->opacity = (box->timer * MAX_OPACITY_F) / 30.0f;
    } else {
        //Otherwise, fade the box in if it's not fully visible
        if (box->opacity != MAX_OPACITY_F) {
            box->opacity += 8.5f * (f32) gUpdateRate;
            if (box->opacity > MAX_OPACITY_F) {
                box->opacity = MAX_OPACITY_F;
            }
        }
    }

    //Draw the box's shadow
    rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_52_Page_Torn_Shadow], 
        INFO_POPUP_SHADOW_X, 
        INFO_POPUP_SHADOW_Y, 
        0xFF, 
        0xFF, 
        0xFF, 
        box->opacity
    );

    //Draw the item's inventory icon (including embedded page background)
    bzero(sTempIcon, sizeof(TextureTile));
    sTempIcon->tex = box->texture;
    sTempIcon[1].tex = NULL;
    rcp_tile_write(gdl, sTempIcon, 
        INFO_POPUP_M_X, 
        INFO_POPUP_Y, 
        0xFF, 
        0xFF, 
        0xFF, 
        box->opacity
    );

    //Draw tattered left edge of box (@unfinished: outdated page design, mismatched with icons' newer BG)
    rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_29_Page_Torn_Left], 
        INFO_POPUP_L_X, 
        INFO_POPUP_Y, 
        0xFF, 
        0xFF, 
        0xFF, 
        box->opacity
    );

    //Draw tattered right edge of box (@unfinished: outdated page design, mismatched with icons' newer BG)
    rcp_tile_write(gdl, sTextureTiles[CMDMENU_TEX_30_Page_Torn_Right], 
        INFO_POPUP_R_X,
        INFO_POPUP_Y, 
        0xFF, 
        0xFF, 
        0xFF, 
        box->opacity
    );

    //Draw item count (NOTE: can't draw values under 2 or over 10, because no icons provided)
    if (box->count > 1) {
        sTempIcon->tex = sInventoryStackNumbersTex;

        //@bug: crashes if count is above 10, since there's no texture frame
        sTempIcon->animProgress = (box->count - 2) << 8;
        rcp_tile_write(gdl, sTempIcon, 
            INFO_POPUP_QUANTITY_X, 
            INFO_POPUP_QUANTITY_Y, 
            0xFF, 
            0xFF, 
            0xFF, 
            box->opacity
        );
    }
}

// offset: 0x70A0 | func: 45 | export: 11
/**
  * Foodbag-related? 
  *
  * `Foodbag_control` uses this function to set `_data_88` to 0 or 1, but the data seems to otherwise be unused.
  */
void cmdmenu_func_70A0(u8 unkBool) {
    _data_88 = unkBool;
}

// offset: 0x70C8 | func: 46 | export: 20
/**
  * Creates an energy bar (e.g. the fuel gauge at CloudRunner Fortress' Racetrack)
  */
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
/*
 * Sets the value of the active energy bar.
 */
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
/*
 * Draws the energy bar, if one is created.
 */
static void cmdmenu_draw_energy_bar(Gfx** gdl) {
    EnergyBar* enbar;

    enbar = sEnergyBar;
    if (enbar == NULL) {
        return;
    }

    if (enbar->fadeout) {
        if (gUpdateRate >= enbar->alpha) {
            enbar->fadeout = FALSE;
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
        (ENERGY_BAR_X - (enbar->width >> 1)), 
        (ENERGY_BAR_Y - enbar->height), 
        enbar->filledWidth, 
        enbar->height, 
        0, 
        0,
        1.0f, 
        1.0f, 
        enbar->alpha | ~0xFF, 
        TILE_WRITE_TRANSLUCENT | TILE_WRITE_POINT_FILT);
    rcp_tile_write_x(gdl, enbar->emptybarTex, 
        (ENERGY_BAR_X - (enbar->width >> 1)) + enbar->filledWidth, 
        (ENERGY_BAR_Y - enbar->height), 
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
  * Frees the energy bar immediately.
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
/**
  * Causes the energy bar to fade out and free itself when invisible.
  */
void cmdmenu_energy_bar_fadeout(void) {
    if (sEnergyBar != NULL) {
        sEnergyBar->fadeout = TRUE;
    }
    /* default.dol
    else {
        STUBBED_PRINTF(" WARNING : cmdmenu Energy bar alreadby freed \n");
    }
    */
}
