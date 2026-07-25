#ifndef _DLLS_351_H
#define _DLLS_351_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitDug;         //Gamebit to set when Tricky digs up the spot
/*1A*/ s16 sidekickExperience; //Not used by the GroundAnimator itself, but Tricky must have this much experience to dig the spot ("It's too deep, I need more practice!") 
/*1C*/ s16 sidekickCost;       //Not used by the GroundAnimator itself, but Tricky must have this much energy to dig the spot ("I'm too tired to do this right now!")
/*1E*/ s16 magicCaveID;        //Values 3 onwards cause the dig spot to become be a Magic Cave entrance
/*20*/ u8 digDepthMax;         //Dig distance (stored at 100x scale)
/*21*/ u8 soundIndex;          //Index of the sound to play when digging has finished (0: small secret, 1: big secret)
/*22*/ u8 unused22;            //Not used by the GroundAnimator itself, most (but not all) GroundAnimator objSetups have this set to 0
/*23*/ u8 findCommandRadius;   //Range for Find command to show up in inventory
/*24*/ u8 unused24;            //Unused - all existing GroundAnimator objSetups have this set to 0
/*25*/ u8 animatorID;          //Block shapes with this tag will be animated
/*26*/ u8 falloffRadius;       //Vertex influence tapers off with this radius, and it also decides how far Tricky scoots backwards while digging
/*27*/ u8 collectableDepth;    //How far down the collectable is buried beneath the dig spot
} GroundAnimator_Setup;

typedef struct {
    f32* vtxWeights;          //Displacement strengths (from 0 to 1) for each vertex being animated (influence falls off from centre)
    Object* collectable;      //A nearby collectable buried under the GroundAnimator, if found
    s32 digDepth;             //Current dig progress (starts at 0, increases while digging)
    f32 falloffRadius;        //Vertex influence tapers off with this radius, and it also decides how far Tricky scoots backwards while digging
    f32 collectableDepth;     //Affects how far down the collectable is buried
    s16 animatedShapeIDs[5];
    s16 misconfiguredShapeID; //Highest-indexed animatorID-tagged shapeID that doesn't have `RENDER_SUBSURFACE` enabled - unused, but maybe to assist with debugging?
    s16 animatedVtxCount;     //The total number of vertices being animated
    u8 animatedShapesCount;   //Total shapes animated (i.e. number of items in `animatedShapeIDs`)
    s8 previousDigDepth;      //Previous dig progress value (vertex updates are queued when this differs from current dig value)
    u8 magicCaveID;           //Seems intended as an index for a specific Magic Cave instance (can be queried with `get_magic_cave_index` export)
    u8 animUpdates;           //Used to queue vertex animation updates
    u8 flags;                 //Various state flags: Block search, dig finished, Magic Cave entrance/glow
} GroundAnimator_Data;

typedef enum {
    GroundAnimator_FLAG_0_None = 0,
    GroundAnimator_FLAG_1_Block_Found = 1,
    GroundAnimator_FLAG_2_Dig_Finished = 2,
    GroundAnimator_FLAG_4_Unused = 4,
    GroundAnimator_FLAG_8_Magic_Cave_Entrance = 8,
    GroundAnimator_FLAG_10_Glow_Created = 0x10,
    GroundAnimator_FLAG_20_Glow_Required = 0x20
} GroundAnimator_Flags;

DLL_INTERFACE(DLL_351_GroundAnimator) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ f32 (*tick_dig)(Object* self, Object* sidekick);
    /*8*/ u8 (*is_dig_finished)(Object* self);
    /*9*/ u8 (*get_magic_cave_index)(Object* self);
};

#endif // _DLLS_351_H

