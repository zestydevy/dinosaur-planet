#ifndef _DLLS_1_H
#define _DLLS_1_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "game/objects/object.h"
#include "sys/rcp.h"
#include "types.h"

/** Data for a particular inventory page */
typedef struct {
/*0*/ void* items; //all the different items that can appear on this page
/*4*/ s16 selectedIndex; // the loaded item index of this page's selected item (at runtime)
/*8*/ u32 mesgID;    // objmesg ID sent to player
/*C*/ s32 btnMask;   // mask of joypad buttons that can be used to move to this item
} CmdmenuPage;

/** Data for the item info pop-up that appears after collecting certain items (e.g. Kyte's grubs) */
typedef struct {
/*0*/ Texture* texture;
/*4*/ s32 timer;
/*8*/ f32 opacity;
/*C*/ s16 count;
} CmdmenuInfoPopup;

/** Sidekick Commands use this slightly different version of the InventoryItem struct */
typedef struct {
/*0*/ s16 flag; //For commands: the sidekick bitfield value for the sidekick command (not a gamebit)
/*2*/ s16 index; //For commands: the index of the sidekick command (not a gamebit)
/*4*/ s16 textureID; //Mapped through TEXTABLE.bin
/*6*/ s16 sidekickTextureID; //Always just stores the same textureID as the previous member
                                //Maybe intended to store the different textureID for the transparent active sidekick command bubble?
/*8*/ s16 textID; //Gametext lineID (in either file #3 or 568, depending on upper byte)
/*A*/ s8 openPage; //Command menu page to open after use (-1 to close)
/*B*/ u8 sound; //The sound to play when selecting this item (see `CmdMenuItemSounds`)
} InventoryCommand; 

typedef union {
    //Items/Spells
    struct {
    /*0*/ s16 gamebitObtained; //Gamebit that adds item to inventory
    /*2*/ s16 gamebitHide; //Gamebit that removes item from inventory
    /*4*/ s16 textureID; //Mapped through TEXTABLE.bin
    /*6*/ s16 unused; //Only used by sidekick commands
    /*8*/ s16 textID; //Gametext lineID (in either file #3 or 568, depending on upper byte)
    /*A*/ s8 openPage; //Command menu page to open after use (-1 to close)
    /*B*/ u8 sound; //The sound to play when selecting this item (see `CmdMenuItemSounds`)
    }; 

    //Commands
    InventoryCommand command;
} InventoryItem;

#define CMDMENU_TRACKED_PLAYER_STATS_COUNT 10

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

typedef struct {
    s16 increased;
    s16 decreased;
} StatChangeSounds;

typedef union {
    struct {
        /*0 00*/ StatChangeSounds playerHealth;
        /*1 04*/ StatChangeSounds sidekickBlueFood;
        /*2 08*/ StatChangeSounds playerMagic;
        /*3 0C*/ StatChangeSounds playerScarabCount;
        /*4 10*/ StatChangeSounds sidekickRedFood;
        /*5 14*/ StatChangeSounds unk14;
        /*6 18*/ StatChangeSounds unk18;
        /*7 1C*/ StatChangeSounds playerHealthMax;
        /*8 20*/ StatChangeSounds playerMagicMax;
        /*9 24*/ StatChangeSounds sidekickMaxFood;
    };
    StatChangeSounds items[10];
} CmdmenuPlayerStatsChangeSounds;

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

/** For use with cmdmenu_get_page_category */
typedef enum {
    CMDMENU_CATEGORY_0 = 0, //Closed/no change
    CMDMENU_CATEGORY_1 = 1, //Unused
    CMDMENU_CATEGORY_2_Sidekick = 2,
    CMDMENU_CATEGORY_3_Items = 3,
    CMDMENU_CATEGORY_4_Spells = 4,
    CMDMENU_CATEGORY_5 = 5,                 //Seems intended for Sidekick subpages
    CMDMENU_CATEGORY_6_Food = 6,            //Item subpages (player foodbag actions/items)
    CMDMENU_CATEGORY_7_Sidekick_Food = 7    //Seems intended for Spells subpages (not used); used for Sidekick subpages instead
} CmdMenuPageCategories;

typedef enum {
    CMDMENU_PAGE_0_Items_Krystal = 0,
    CMDMENU_PAGE_1_Items_Sabre = 1,
    CMDMENU_PAGE_2_Food_Actions_Krystal = 2,
    CMDMENU_PAGE_3_Food_Actions_Sabre = 3,
    CMDMENU_PAGE_4_Food_Krystal = 4,
    CMDMENU_PAGE_5_Food_Sabre = 5,
    CMDMENU_PAGE_6_Spells = 6,
    CMDMENU_PAGE_7_Sidekick_Kyte = 7,
    CMDMENU_PAGE_8_Sidekick_Tricky = 8,
    CMDMENU_PAGE_9_Food_Actions_Kyte = 9,
    CMDMENU_PAGE_10_Food_Actions_Tricky = 10,
    CMDMENU_PAGE_11_Food_Kyte = 11,
    CMDMENU_PAGE_12_Food_Tricky = 12
} CmdMenuPages;

typedef enum {
    CMDMENU_SOUND_NONE = 0,
    CMDMENU_SOUND_ITEM = 1, //Sound only plays if the item couldn't be used (e.g. Alpine Root wasn't given to SnowHorn)
    CMDMENU_SOUND_PAGE = 2  //Sound for entering inventory sub-pages (e.g. entering into foodbag's actions page)
} CmdMenuItemSounds;

#define CMDMENU_CLEAR_BUTTONS_OVERRIDE -1

DLL_INTERFACE(DLL_1_cmdmenu) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ s32 (*update1)(void);
/*1*/ void (*update2)(void);
/*2*/ void (*print)(Gfx **gdl, Mtx **mtxs, Vertex **vtxs);
/*3*/ void (*pages_clear_last_selected_index)(void); //Zeroes the selected item index on all the inventory pages (i.e. forgets user's last selection).
/*4*/ void (*request_new_player_stats_snapshot)(void); //Flags that a snapshot of the player's current tracked stats should be taken. (Used to show UI when player health changes, etc.)
/*5*/ s32 (*get_target_objects)(Object **targetObjects, s32 maxObjects, u8 lockFlag, s32 arg3, f32 range); //Gets an array (`targetObjects`) of nearby Objects that can be targetted (i.e. have lock-on data and are fully opaque), sorted by their address. Returns the number of Objects in the array.
/*6*/ int (*was_any_item_used)(void); //Checks if any item was used in the inventory (returns Boolean)
/*7*/ int (*was_this_item_used)(s32 itemGamebitID); //Checks if a particular item was used in the inventory, queried via its inventory gamebitID (or in the case of Sidekick Commands queried by the command's index; see `SidekickCommandIndices`)
/*8*/ s32 (*was_used_item_in_gamebit_array)(s32* gamebitArray, s32 arraySize); //Checks if the used inventory item's gamebitID matches any of those in a given array of gamebitIDs. If a match is found, the used item's gamebitID is returned. If no match was found, `NO_GAMEBIT` (-1) is returned. The array's address should be provided (`gamebitArray`), as well as its item count (`arraySize`).
/*9*/ s8 (*get_page_category)(void); //Returns the categoryID of the current inventory menu page (see `CmdMenuPageCategories`)
/*10*/ s16 (*get_subpage_gamebit)(void); //Returns the gamebitID associated with the current menu subpage (e.g. if Krystal has opened her Food Bag Actions page, the returned gamebitID would be: `BIT_Krystal_Foodbag_S` or `BIT_Krystal_Foodbag_M` or `BIT_Krystal_Foodbag_L`, depending on which bag she has).
/*11*/ void (*func_70A0)(u8 unkBool); //Foodbag-related?
/*12*/ void (*auto_show_info_scroll)(u32 uiTextID, u32 screenX, u32 screenY); //Overrides the info scroll, displaying a specific textID at a custom screen position (can differ from usual top-centre). See `GameTexts_UI_A` and `GameTexts_UI_B` for UI textIDs.
/*13*/ void (*open_tutorial_textbox)(s32 gametextID, s32 screenX, s32 screenY); //Opens the tutorial textbox at a specified screen position, displaying a specific gametext file's tutorial text.
/*14*/ void (*close_tutorial_textbox)(void); //Causes the tutorial textbox to close.
/*15*/ void (*disable_buttons)(u16 mask); //Sets a controller button mask, affecting player control of the inventory.
/*16*/ void (*set_buttons_override)(s32 buttonsOverride); //Simulate controller button presses (only for the cmdmenu), used by tutorial sequences like Tricky's sidekick command explanation. An argument of `CMDMENU_CLEAR_BUTTONS_OVERRIDE` (-1) clears the override.
/*17*/ void (*info_show)(s16 itemGamebit, s32 displayDuration, s32 itemCount); //Show an item info pop-up in the bottom-left of screen (used after collecting certain items, e.g. Kyte's grubs) - the item is specified using its inventory gamebitID.
/*18*/ void (*info_show_tex)(s16 textureID, s32 displayDuration, s32 itemCount); //Show an item info pop-up in the bottom-left of screen (used after collecting certain items, e.g. Kyte's grubs) - the item is specified using its textable textureID.
/*19*/ void (*toggle_forced_stats_display)(u8 force); //A nonzero argument causes the player's stats HUD to appear automatically. An argument of 0 will remove this effect.
/*20*/ void (*energy_bar_create)(s32 minEnergy, s32 maxEnergy, s32 fullTexID, s32 emptyTexID, s32 scale); //Creates an energy bar (e.g. the fuel gauge at CloudRunner Fortress' Racetrack)
/*21*/ void (*energy_bar_set)(s32 energy); //Sets the value of the active energy bar.
/*22*/ void (*energy_bar_free)(void); //Frees the energy bar immediately.
/*23*/ void (*energy_bar_fadeout)(void); //Causes the energy bar to fade out and free itself when invisible.
};

#endif //_DLLS_1_H
