#ifndef _DLLS_1_H
#define _DLLS_1_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "game/objects/object.h"
#include "types.h"

typedef struct {
/*0*/ s16 gamebitObtained;  //Gamebit that adds item to inventory
/*2*/ s16 gamebitHide;      //Gamebit that removes item from inventory
/*4*/ s16 textureID;        //Mapped through TEXTABLE.bin
/*6*/ s16 sidekickCommand;  //Usually -1, except on sidekick commands
/*8*/ s16 textID;           //Gametext lineID (in either file #3 or 568, depending on upper byte)
/*A*/ s8 openPage;          //Command menu page to open after use (-1 to close)
/*B*/ u8 sound;             //The sound to play when selecting this item (see `CmdMenuItemSounds`)
} InventoryItem;

typedef struct {
/*0*/ s16 gamebitObtained;  //Gamebit that adds item to inventory
/*2*/ s16 gamebitHide;      //Gamebit that removes item from inventory
/*4*/ s16 textureID;        //Mapped through TEXTABLE.bin
/*6*/ s16 sidekickCommand;  //Usually -1, except on sidekick commands
/*8*/ s16 textID;           //Gametext lineID (in either file #3 or 568, depending on upper byte)
/*A*/ s8 openPage;          //Command menu page to open after use (-1 to close)
/*B*/ u8 sound;             //The sound to play when selecting this item (see `CmdMenuItemSounds`)
} InventoryCommand;

/** For use with dll_1_func_F24 */
typedef enum {
    CMDMENU_CATEGORY_0 = 0, //Closed
    CMDMENU_CATEGORY_1 = 1,
    CMDMENU_CATEGORY_2_Sidekick = 2,
    CMDMENU_CATEGORY_3_Items = 3,
    CMDMENU_CATEGORY_4_Spells = 4,
    CMDMENU_CATEGORY_5 = 5,
    CMDMENU_CATEGORY_6_Food = 6,
    CMDMENU_CATEGORY_7 = 7
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
/*0*/ s32 (*control)(void);
/*1*/ void (*func_35C)(void);
/*2*/ void (*print)(Gfx **gdl, Mtx **mtxs, Vertex **vtxs);
/*3*/ void (*func_1290)(void);
/*4*/ void (*request_new_player_stats_snapshot)(void); //Flags that a snapshot of the player's current tracked stats should be taken. (Used to show UI when player health changes, etc.)
/*5*/ s32 (*get_target_objects)(Object **targetObjects, s32 maxObjects, u8 lockFlag, s32 arg3, f32 range); //Gets an array (`targetObjects`) of nearby Objects that can be targetted (i.e. have lock-on data and are fully opaque), sorted by their address. Returns the number of Objects in the array.
/*6*/ int (*was_any_item_used)(void); //Checks if any item was used in the inventory (returns Boolean)
/*7*/ int (*was_this_item_used)(s32 itemGamebitID); //Checks if a particular item was used in the inventory, queried via its inventory gamebitID (returns Boolean)
/*8*/ s32 (*func_E2C)(s32 *arg0, s32 arg1);
/*9*/ s8 (*get_page_category)(void); //Returns the categoryID of the current inventory menu page (see `CmdMenuPageCategories`)
/*10*/ s16 (*func_F40)(void); //get gamebitID of active submenu?
/*11*/ void (*func_70A0)(u8 arg0);
/*12*/ void (*func_130C)(u32 arg0, u32 arg1, u32 arg2);
/*13*/ void (*func_1338)(s32 gametextID, s32 arg1, s32 arg2);
/*14*/ void (*func_13F4)(void);
/*15*/ void (*disable_buttons)(u16 mask);
/*16*/ void (*set_buttons_override)(s32 buttonsOverride); //Simulate controller button presses (only for the cmdmenu), used by tutorial sequences like Tricky's sidekick command explanation.
/*17*/ void (*info_show)(s16 itemGamebit, s32 displayDuration, s32 itemCount); //Show an item info pop-up in the bottom-left of screen (used after collecting certain items, e.g. Kyte's grubs) - the item is specified using its inventory gamebitID.
/*18*/ void (*info_show_tex)(s16 textureID, s32 displayDuration, s32 itemCount); //Show an item info pop-up in the bottom-left of screen (used after collecting certain items, e.g. Kyte's grubs) - the item is specified using its textable textureID.
/*19*/ void (*toggle_forced_stats_display)(u8 force);
/*20*/ void (*energy_bar_create)(s32 minEnergy, s32 maxEnergy, s32 arg2, s32 arg3, s32 arg4);
/*21*/ void (*energy_bar_set)(s32 energy);
/*22*/ void (*energy_bar_free)(void);
/*23*/ void (*energy_bar_fadeout)(void);
};

#endif //_DLLS_1_H
