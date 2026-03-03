#ifndef _DLLS_638_H
#define _DLLS_638_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef enum {
    DFPT_ObjGroup0_Lower_LevControl_and_Vertical_Magic_Waves = 0,
    DFPT_ObjGroup1_EMPTY = 1,
    DFPT_ObjGroup2_Bottom_BigDoor = 2,
    DFPT_ObjGroup3_EMPTY = 3,
    DFPT_ObjGroup4_Under_SpellStone_Holder_1 = 4,
    DFPT_ObjGroup5_Bottom_Upper_Walkway_Electric_Barriers = 5,
    DFPT_ObjGroup6_Bottom_Block_Slide_Puzzle = 6, 
    DFPT_ObjGroup7_SpellStone_Holder_1_and_Dish = 7,
    DFPT_ObjGroup8_Bottom_Upper_Walkway_FX = 8,
    DFPT_ObjGroup9_Bottom_Upper_Walkway_FX = 9,
    DFPT_ObjGroup10_Puzzle_Pad = 10,
    DFPT_ObjGroup11_Electrified_Tiles_and_Lift_Torches = 11
} DFPT_ObjectGroups;

typedef struct {
    ObjSetup base;
} DFPTLevelControl_Setup;

typedef struct {
    s16 zappedTimer;     //Duration for player electrocution effect (when zapped by tiles)
    u16 unused2;
    u8 unused4;
    u8 previousPadState; //Used to track when the player steps onto/off the DFP_PuzzlePad switch
} DFPTLevelControl_Data;

DLL_INTERFACE(DLL_638_DFPLevcontrol) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*export_safe_floor_tiles)(Object* self, u8* floorTileIndices);
};

#endif // _DLLS_638_H
