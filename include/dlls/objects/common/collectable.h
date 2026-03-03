#ifndef _DLLS_COMMON_COLLECTABLE_H
#define _DLLS_COMMON_COLLECTABLE_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

/** Used by DLL 272 "collectable" and DLL 523 "SCcollectables" */
typedef struct {
/*00*/    ObjSetup base;
/*18*/    s8 unused18;
/*19*/    s8 messageArgBase;      //Base arg value used for sidekick-related collectables
/*1A*/    u8 objHitsValue;        //[DLL 272 "collectable": stored to Collectable_Data but unused]
                                  //[DLL 523 "SCcollectables": value for objHitInfo->unk52]
/*1B*/    u8 yaw;
/*1C*/    s16 gamebitCollected;   //Set when collected: collectable vanishes when set
/*1E*/    s16 animMessage;
/*20*/    s8 unused20;
/*21*/    s8 unused21;    
/*22*/    u8 pitch;
/*23*/    u8 roll;
/*24*/    s16 gamebitSecondary;   //(Optional) [DLL 272 "collectable": show when set]
                                  //           [DLL 523 "SCcollectables": fall from tree]
/*26*/    u8 modelIdx;
/*27*/    u8 applyColourMultiplier;
/*28*/    u8 multiplyR;           //Colour multiplier for model
/*29*/    u8 multiplyG;           //Colour multiplier for model
/*2A*/    u8 multiplyB;           //Colour multiplier for model
/*2B*/    u8 unused2B;
/*2C*/    s16 gamebitCount;
} Collectable_Setup;

typedef enum {
    Collectable_Type_Inventory = 0x1,       //Used by [CFPrisonKey, CFPowerCrystal1, CFPowerCrystal2, CFPowerCrystal3, CFPowerRoomKey, CFTreasureChest, CFExplosiveKey, WCSunStone, WCMoonStone, WCTrexTooth, DIMBridgeCogCol, DIMShackleKey, DIMAlpineRoot, DIMAlpineRoot2, DIMTruthHorn, DIM2CellKey, DIM2SilverKey, DIM2GoldKey, DIM2PuzzleKey, CCCellKey, CCgoldnuggetPic, NWtrickyball, NWalpineroot, NWreplaymedal, SC_golden_nugge, SC_MMP_Key, WMconsolestone, DB_egg, DBmap, DBDiamond, PadlockKey, GuardPass, Dynamite, Spellstone, Duster, EnergyPlant1, EnergyPlant2, foodbagSmall, foodbagMedium, foodbagLarge, FireNut, fishingnetColle]
    Collectable_Type_Spells = 0x2,          //Used by [EarthQuakSpell, FireSpell, IllusionSpell, PortalSpell, ProjectileSpell]
    Collectable_Type_Food = 0x4,            //Used by [meatPickup, applePickup, beanPickup]
    Collectable_Type_SidekickA = 0x8,       //Unused? Sidekick-related (Tricky?)
    Collectable_Type_SidekickB = 0x10,      //Unused? Sidekick-related (Kyte?)
    Collectable_Type_20 = 0x20,             //Unused?
    Collectable_Type_Magic = 0x40,          //Used by [WM_PureMagic, BoneDust, MagicDustSmall, MagicDustMid, MagicDustLarge, MagicDustHuge]
    Collectable_Type_Upgrade = 0x80         //Used by [OBJ_WeaponUp]
} Collectable_Types;

#endif //_DLLS_COMMON_COLLECTABLE_H
