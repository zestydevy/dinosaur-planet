#ifndef _DLLS_607_H
#define _DLLS_607_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    s16 enemiesDefeated;            //Only used during setup4 (Krystal's MMP spirit), seems intended to count how many Skeetlas she's defeated
    s16 timer;                      //A timer used during various setups
    s16 intervalBehaviourTimer;     //Only used during setup7 (last Spirits), causes gamebit to be set at intervals for a short while
    u8 interval;                    //Only used during setup7 (last Spirits), stores a decreasing interval for `objData->timer` to set a gamebit
    u8 galleonIsLoaded;             //Only Used during setup1 (Krystal meeting Randorn)
    u8 flags;                       //Galleon-related flags
    u8 setup;                       //Current level setup number
} WL_LevelControl_Data;

typedef enum {
    WL_LevelControl_FLAG_None = 0,
    WL_LevelControl_FLAG_Galleon_Docked = 1,
    WL_LevelControl_FLAG_Galleon_Leaving_Search = 2
} WL_LevelControl_Flags;

typedef enum {
    WM_Setup1_Krystal_Meeting_Randorn = 1,
    WM_Setup2_Spirit1_Krystal_DF = 2,
    WM_Setup3_Spirit2_Sabre_DB = 3,
    WM_Setup4_Spirit3_Krystal_MMP = 4,
    WM_Setup5_Spirit4_Sabre_WC = 5,
    WM_Setup6_Spirit5_6_Krystal_CC_Sabre_WG = 6,
    WM_Setup7_Spirit7_8_Krystal_GP_Sabre_SW = 7
} WM_Setups;

typedef enum {
    WM_ObjGroup0_EMPTY = 0,
    WM_ObjGroup1_Galleon = 1,
    WM_ObjGroup2_EMPTY = 2, //Empty, but loaded when leaving through Krystal's entrance cave
    WM_ObjGroup3_Krystal_Cave_FX_Galleon_Reloader = 3, //magic vein effects, and a TriggerPlane reloading the Galleon for the sequence where Scales escapes with Kyte
    WM_ObjGroup4_Krystal_Laser_Corridor = 4, //switches, laser emitters, wall torch
    WM_ObjGroup5_Krystal_Randorn_Hall_and_Antechamber = 5, //Randorn, door, pressure switch, column, etc.
    WM_ObjGroup6_Krystal_Antechamber_SharpClaw = 6, //SharpClaw near pressure switch
    WM_ObjGroup7_Krystal_Warp_Crystal_Hall = 7, //podium area for the Warp Crystal
    WM_ObjGroup8_Exposed_Mantle_FX = 8, //effects for exposed planetary core at base of main chamber
    WM_ObjGroup9_EMPTY = 9,
    WM_ObjGroup10_EMPTY = 10, //Empty, but loaded around Krystal antechamber (specifically in setup4, so may be intended for ceiling Skeetlas)
    WM_ObjGroup11_Krystal_Orrery = 11, //planets, console
    WM_ObjGroup13_EMPTY = 13 //Empty, but loaded when entering Krystal antechamber (specifically in setup6)
} WM_ObjectGroups;

#define SPIRIT_INDEX(value) (1 << (value - 1))

#endif // _DLLS_607_H
