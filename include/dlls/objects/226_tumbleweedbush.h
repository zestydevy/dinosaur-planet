#ifndef _DLLS_226_H
#define _DLLS_226_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    u8 roll;
    u8 pitch;
    u8 yaw;
    u8 growWeedsRadius;             //Tumbleweeds grow on tree when player inside this radius (stored halved)
    f32 scale;
    u8 multiplyColourR;             //Colour multiplier for the tree
    u8 multiplyColourG;             //Colour multiplier for the tree
    u8 multiplyColourB;             //Colour multiplier for the tree
    u8 carryingGold;                //Bitfield flags: carrying a Shiny Nugget when bit0 is set
} TumbleweedBush_Setup;

typedef struct {
    f32 unk0;                       //Set to 0.0f in setup, but otherwise unused
    f32 unused4;
    u16 growWeedsRadius;            //Tumbleweeds grow on tree when player inside this radius
    Object* heldWeeds[4];           //Array of Tumbleweed Objects growing in the tree
    Vec3f heldWeedCoords[4];        //Positions for Tumbleweeds to grow at (relative to the tree)
    u8 carryingGold;                //Bitfield flags: carrying a Shiny Nugget when bit0 is set
    u8 pad4D;
    u16 tumbleweedsGrown;           //The total number of Tumbleweeds grown by this tree
    u8 heldWeedCount;               //The number of Tumbleweed objects referenced in the heldWeeds array
} TumbleweedBush_Data;

DLL_INTERFACE(DLL_226_TumbleweedBush) {
/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
/*07*/ void (*remove_tumbleweed)(Object* self, Object* tumbleweed); //Removes a specific Tumbleweed Object from the tree's list of held Tumbleweeds.
};

#endif //_DLLS_226_H
