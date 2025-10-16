#ifndef _GAME_GAMETEXTS_UI_H
#define _GAME_GAMETEXTS_UI_H

/** Main descriptive text, contained in Gametext_3*/
typedef enum {
    GAMETEXT_UI_0_Heel,             // Returns your Sidekick back to you
    GAMETEXT_UI_1_Find,             // Uncover the things only Sidekicks can
    GAMETEXT_UI_2_Flame,            // Set fire to stuff
    GAMETEXT_UI_3_Distract,         // GuardClaws cannot resist this
    GAMETEXT_UI_4_Guard,            // Stops baddies nicking stuff
    GAMETEXT_UI_5_Ride,
    GAMETEXT_UI_6_Play,             // Throw the ball and watch him go
    GAMETEXT_UI_7_Prison_Key,       // CloudRunner Fortress
    GAMETEXT_UI_8_Power_Room_Key,   // CloudRunner Fortress
    GAMETEXT_UI_9_Power_Crystal,    // CloudRunner Fortress
    GAMETEXT_UI_A_Gate_Key,         // Northern Wastes
    GAMETEXT_UI_B_Alpine_Root,      // SnowHorn Food
    GAMETEXT_UI_C_Blue_Mushroom,    // Earthwalker Food
    GAMETEXT_UI_D_Projectile_Spell, // Shoot stuff
    GAMETEXT_UI_E_Illusion_Spell,   // Makes sharpclaws think you are one too
    GAMETEXT_UI_F_Wizard_Randorn,   // Sabre's father
    GAMETEXT_UI_10_Warp_Activator,
    GAMETEXT_UI_11_Snowbike_Key,    // Ice Mountain (unused)
    GAMETEXT_UI_12_Deck_Door,       // The door is locked (unused)
    GAMETEXT_UI_13_Mine_Key,        // Northern wastes
    GAMETEXT_UI_14_Shield,          // Protect yourself
    GAMETEXT_UI_15_Fire_Spell,      // (unused?)
    GAMETEXT_UI_16_Small_Food_Bag,
    GAMETEXT_UI_17_Medium_Food_Bag,
    GAMETEXT_UI_18_Large_Food_Bag,
    GAMETEXT_UI_19_Cell_Key,         // Dark Ice Mines 2
    GAMETEXT_UI_1A_Horn_Of_Truth,    // Summon The SnowHorn
    GAMETEXT_UI_1B_Sun_Stone,        // Walled City
    GAMETEXT_UI_1C_Moon_Stone,       // Walled City
    GAMETEXT_UI_1D_Forcefield_Spell, // Cape Claw
    GAMETEXT_UI_1E_Cell_Door_Key,    // Cape Claw
    GAMETEXT_UI_1F_Eat,              // Eat food in foodbag
    GAMETEXT_UI_20_Place,            // Place food down from foodbag
    GAMETEXT_UI_21_Give,             // Give food from foodbag
    GAMETEXT_UI_22_Green_Apple,
    GAMETEXT_UI_23_Red_Apple,
    GAMETEXT_UI_24_Brown_Apple,
    GAMETEXT_UI_25_Fish,
    GAMETEXT_UI_26_Smoked_Fish,
    GAMETEXT_UI_27_Dino_Egg,     // Restores your energy
    GAMETEXT_UI_28_Moldy_Meat,
    GAMETEXT_UI_29_Ball,         // A Toy For Tricky
    GAMETEXT_UI_2A_Blue_Grub,    // Cloudrunner Food
    GAMETEXT_UI_2B_Red_Grub,     // Cloudrunner Food
    GAMETEXT_UI_2C_Old_Grub,     // Cloudrunner Food
    GAMETEXT_UI_2D_Red_Mushroom, // Flame Fuel
    GAMETEXT_UI_2E_Old_Mushroom, // Earthwalker Food
    GAMETEXT_UI_2F_Small_Grub_Bag,
    GAMETEXT_UI_30_Medium_Grub_Bag,
    GAMETEXT_UI_31_Large_Grub_Bag,
    GAMETEXT_UI_32_Small_Mushroom_Bag,
    GAMETEXT_UI_33_Medium_Mushroom_Bag,
    GAMETEXT_UI_34_Large_Mushroom_Bag,
    GAMETEXT_UI_35_Green_Bean,
    GAMETEXT_UI_36_Red_Bean,
    GAMETEXT_UI_37_Brown_Bean,
    GAMETEXT_UI_38_Blue_Bean,
    GAMETEXT_UI_39_Take_All_Food,
    GAMETEXT_UI_3A_Eat_First,
    GAMETEXT_UI_3B_Portal_Spell,    // Opens Spatial Walkways
    GAMETEXT_UI_3C_Ice_Blast_Spell, // Fires a Column of Ice
    GAMETEXT_UI_3D_Firefly_Lantern, // Uses Fire Flys to Light the Way
    GAMETEXT_UI_3E_Mindread_Spell,  // Reads Only Small Minds
    GAMETEXT_UI_3F_Fruit,
    GAMETEXT_UI_40_Meat,
    GAMETEXT_UI_41_Red_Mushroom,
    GAMETEXT_UI_42_Red_Maggot,
    GAMETEXT_UI_43_Green_Magic,
    GAMETEXT_UI_44_Blue_Magic,
    GAMETEXT_UI_45_Fishing_Net,
    GAMETEXT_UI_46_Firefly_Lantern,
    GAMETEXT_UI_47_Translator,
    GAMETEXT_UI_48_Small_Sidekick_Foodbag,
    GAMETEXT_UI_49_Medium_Sidekick_Foodbag,
    GAMETEXT_UI_4A_Medium_Magic_Chest,
    GAMETEXT_UI_4B_Large_Magic_Chest,
    GAMETEXT_UI_4C_Small_Player_Foodbag,
    GAMETEXT_UI_4D_Medium_Player_Foodbag,
    GAMETEXT_UI_4E_Large_Player_Foodbag,
    GAMETEXT_UI_4F_Dark_Ice_Mines_Map,
    GAMETEXT_UI_50_Dragon_Rock_Map,
    GAMETEXT_UI_51_Swapstone_Circle_Map,
    GAMETEXT_UI_52_Moon_Mountain_Map,
    GAMETEXT_UI_53_Warlock_Replay_Disk,
    GAMETEXT_UI_54_Ice_Mountain_Replay_Disk,
    GAMETEXT_UI_55_Ice_Mountain_Map,
    GAMETEXT_UI_56_White_Mushroom,       // Earthwalker Medicine
    GAMETEXT_UI_57_Purple_Mushroom,      // ?
    GAMETEXT_UI_58_Thorntail,            // Inhabitants Of SwapHollow
    GAMETEXT_UI_59_Magic_Plant,          // Grows Magic Energy
    GAMETEXT_UI_5A_Soil,                 // Plant Something Here
    GAMETEXT_UI_5B_Krazoa_Tablet,        // Cape Claw
    GAMETEXT_UI_5C_Silver_Trex_Tooth,    // Walled City
    GAMETEXT_UI_5D_Gold_Trex_Tooth,      // Walled City
    GAMETEXT_UI_5E_Pointback_Egg,        // A dinosaur treasure
    GAMETEXT_UI_5F_Bay_Diamond,          // The famous diamond of diamond bay
    GAMETEXT_UI_60_Spellstone,           // DarkIce Mines
    GAMETEXT_UI_61_Spellstone_Activated, // DarkIce Mines
    GAMETEXT_UI_62_Moon_Seeds,           // Grow Moon Seeds
    GAMETEXT_UI_63_Treasure_Chest_Key,
    GAMETEXT_UI_64_Construction_Engineers_Key,
    GAMETEXT_UI_65_Large_Basket,         // Smash to open
    GAMETEXT_UI_66_Large_Crate,          // Smash to open
    GAMETEXT_UI_67_Small_Basket,         // Throw or smash to open
    GAMETEXT_UI_68_Small_Crate,          // Throw or smash to open
    GAMETEXT_UI_69_Earthwalker,          // Soldier of Earthwalker tribe
    GAMETEXT_UI_6A_Stone_Block,
    GAMETEXT_UI_6B_Warp_Podium
} GameTexts_UI_A;

/** More descriptive text, contained in Gametext_568 */
typedef enum {
    GAMETEXT_UI_B_00_Scarabs,      // Dinosaur Planet Currency (unused)
    GAMETEXT_UI_B_01_Toggle_Map,   // (unused)
    GAMETEXT_UI_B_02_Fire_Crystal, // Cape Claw
    GAMETEXT_UI_B_03_Shiney_Nugget // Cape Claw
} GameTexts_UI_B;

#endif // _GAME_GAMETEXTS_UI_H
