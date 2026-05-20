#ifndef _SYS_OBJTYPE_H
#define _SYS_OBJTYPE_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

enum ObjType {
    OBJTYPE_PLAYER = 0, // Player
    OBJTYPE_SIDEKICK = 1, // Tricky, Kyte
    OBJTYPE_2 = 2, // Trigger (DLL 325)
    OBJTYPE_3 = 3, // Tesla (DLL 257)
    // Baddie (DLL 33), SabreBaddie, Tumbleweed (DLL 227), CFCloudBaby, RobotPatrol, RobotAnimPatrol, 
    // EWTrobotpatrol, CCsandwormBoss, DIMCannon (DLL 537), SB_Galleon, SB_ShipHead, Crawler (DLL 615),
    // DBstealerworm, DR_Cannon (DLL 733), DR_SharpClaw (DLL 739 (conditional)), WCTrex
    OBJTYPE_4 = 4,
    OBJTYPE_5 = 5, // Collectable (DLL 272), CCspellpage, SC_golden_nugge
    OBJTYPE_6 = 6, // PushBlock (DLL 274)
    OBJTYPE_MOBILE_MAP = 7, // Anything with ObjDef flag OBJDEF_IS_MOBILE_MAP
    OBJTYPE_8 = 8, // StaticCamera
    OBJTYPE_9 = 9, // Anything with ObjDef.unk5e >= 1
    // WL_LevelControl, VFP_LevelControl, DFP_LevelControl, SPShop, WCLevelControl, BWLevelControl
    OBJTYPE_10 = 10,
    // DFlog, IMSnowBike (DLL 711), DIMSnowHorn1, DR_EarthWarrior, DR_CloudRunner, CRSnowBike (DLL 732),
    // BWLog
    OBJTYPE_11 = 11,
    OBJTYPE_12 = 12, // SnowHorn (DLL 496)
    OBJTYPE_13 = 13, // ECSH_Shrine
    OBJTYPE_14 = 14, // DIMIceWall
    OBJTYPE_16 = 16, // Door (DLL 279)
    OBJTYPE_17 = 17, // UseObj (DLL 308), SeqObj (DLL 309, 310, 311, 312), DIMBridgeCogMai, DB_SuperSymbol
    OBJTYPE_18 = 18, // Pickup (DLL 54), SmallCrate (DLL 295), ZBomb
    OBJTYPE_19 = 19, // CrackAnimator
    OBJTYPE_21 = 21, // DIMDismountPoin, DR_CloudPerch
    OBJTYPE_22 = 22, // DFriverflow (conditional)
    OBJTYPE_23 = 23, // DFdockpoint
    OBJTYPE_24 = 24, // CFGuardian (conditional), RobotAnimPatrol (conditional), CFbarrel (conditional), DIM2Conveyor
    OBJTYPE_25 = 25, // DFropenode
    OBJTYPE_26 = 26, // DFpulley, DR_pulley
    OBJTYPE_27 = 27, // CFbarrel, DFbarrel
    OBJTYPE_28 = 28, // ExplodeAnimator
    OBJTYPE_29 = 29, // WaveAnimator
    OBJTYPE_30 = 30, // LFXEmitter
    OBJTYPE_31 = 31, // DLL 359
    OBJTYPE_32 = 32, // CFAttractor, CFMagicWall, CFTreasureChest, CRSpellstone
    OBJTYPE_33 = 33, // CFSupTreasureChest
    OBJTYPE_34 = 34, // CFCloudBaby
    OBJTYPE_35 = 35, // ExplodeObj (DLL 390), DBExpWall
    OBJTYPE_36 = 36, // CFChuckObj (DLL 402)
    OBJTYPE_37 = 37, // WallAnimator
    OBJTYPE_38 = 38, // DB_egg (conditional), ? (added by DBstealerworm)
    OBJTYPE_39 = 39, // Player, DBPlaceHolder (conditional), DBDiamond (conditional)
    OBJTYPE_40 = 40, // CClightfoot, SHthorntail
    OBJTYPE_41 = 41, // DBExpbit
    OBJTYPE_42 = 42, // DBlgtbeam
    OBJTYPE_43 = 43, // DBshooterball
    OBJTYPE_44 = 44, // DBDustGeezer
    OBJTYPE_45 = 45, // DBkillboulder
    OBJTYPE_47 = 47, // CF_WallTorch
    // kyteFireFlys, FlameObject, PerchObject, FlameWall (DLL 719), FindKyteObject, SC_beacon, 
    // MMP_mseedrecept, GPbonfire, CCfireflyfind, SC_flamegamefla, RestPerch
    OBJTYPE_48 = 48,
    OBJTYPE_49 = 49, // MMP_barrel (conditional)
    OBJTYPE_50 = 50, // LanternFireFly, WLFireFly
    // Tumbleweed, SandGrabber, SHvines, PressurePad (DLL 287), TrickyWarp, TrickyGuard, GroundAnimator, 
    // WallAnimator, SHbluemushroom, DIMLogFire (DLL 531), DIMIceWall, VFP_flamepoint, DB_FlameSwitch,
    // WCPressureSwitc, DBDustGeezer, DR_Chimmey, DR_Vines
    OBJTYPE_51 = 51,
    OBJTYPE_52 = 52, // DBlevelcontrol
    OBJTYPE_53 = 53, // CFTreasRobo
    OBJTYPE_54 = 54, // MagicPlant
    OBJTYPE_55 = 55, // WallTorch, FirePole
    OBJTYPE_56 = 56, // Anything with ObjDef flag 0x10
    OBJTYPE_57 = 57, // DR_Shackle (DLL 747)
    OBJTYPE_58 = 58, // DR_Platform (DLL 749)
    OBJTYPE_DINOCALLER = 59, // DR_CloudPerch, DR_EarthCallPad
    OBJTYPE_60 = 60, // DR_BarrelPositi
    OBJTYPE_61 = 61, // NWice (DLL 499)
    OBJTYPE_62 = 62, // NWanimice (DLL 498)
    OBJTYPE_63 = 63, // SharpClaw, Crate (DLL 282), MagicPlant
    OBJTYPE_64 = 64 // CCgasvent
};

void obj_object_type_init();
void obj_add_object_type(Object *obj, s32 type);
void obj_free_object_type(Object *obj, s32 type);
Object **obj_get_all_of_type(s32 type, s32 *count);
Object *obj_get_nearest_type_to(s32 type, Object *object, float *distance);
Object *obj_get_nearest_type_to_excluding_self(s32 type, Object *object, float *distance);
Object *obj_get_nearest_type(s32 type, Vec3f *position, float *distance);
s32 obj_is_object_type(Object *obj, s32 type);

#endif
