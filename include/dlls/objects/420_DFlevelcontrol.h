#ifndef _DLLS_420_H
#define _DLLS_420_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef enum {
    DF_ObjGroup0_Foodbag_SharpClaw_Cave, //The eastern cave in the lower falls, near the exit to MMP
    DF_ObjGroup1_Entrance_Magic_Plant_Basin, //The secluded area up a side pathway along the cascading route into Discovery Falls
    DF_ObjGroup2_Lower_Falls, //The main lower tier of Discovery Falls
    DF_ObjGroup3_Shrine_Exterior, //The area outside the shrine
    DF_ObjGroup4_Toxic_Cave, //The cave with noxious gas, in the north-west of the Falls' middle tier
    DF_ObjGroup5_Mole_Cave, //The two-tiered cave with the SharpClaw and the hungry mole
    DF_ObjGroup6_Middle_Falls, //The middle tier of Discovery Falls, with the pulley and cradle
    DF_ObjGroup7, //Empty, but seems to have been for Lower Falls (unloads/loads when entering/exit Foodbag Cave, Toxic Cave, Mole Cave)
    DF_ObjGroup8_Whirlpool_Cave, //The south-east cave, near the upper falls and shrine
    DF_ObjGroup9_Whirlpool_Cave_HitAnimator, //Objects for removing/animating the wall cracks in/before the Whirlpool Cave
    DF_ObjGroup10_Mole_Cave_Podium, //One of the shrine switches
    DF_ObjGroup11_Shrine_Door, //The shrine's door pieces, and related SeqObjs
    DF_ObjGroup12_Upper_Falls_Stalactite_Cave, //The upper cave leading down towards the whirlpool cave 
    DF_ObjGroup13_Upper_Falls_Demolition_Cave, //The cave network just outside the whirlpool cave, with SharpClaw and explosive barrels
    DF_ObjGroup14_Middle_and_Upper_Falls, //Various objects near the upper falls' turbine, including the middle falls' projectile switches and fish
    DF_ObjGroup15, //Empty
    DF_ObjGroup16, //Empty
    DF_ObjGroup17_Shrine_Exterior_Whirlpool_Cave_Waterfall //Texscroll for the Whirlpool Cave's exterior waterfall (once blown up) (never loaded, since there's already a waterfall texscroll2 in objGroup3?)
} DF_ObjectGroups;

#endif // _DLLS_420_H
