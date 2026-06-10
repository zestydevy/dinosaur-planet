#ifndef _DLLS_355_H
#define _DLLS_355_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitDug;          //Gamebit to set when Tricky digs through the wall
/*1A*/ s16 sidekickExperience;  //Not used by the WallAnimator itself, but Tricky must have this much experience to dig the spot ("It's too deep, I need more practice!") 
/*1C*/ s16 sidekickCost;        //Tricky must have this much energy to dig the spot ("I'm too tired to do this right now!")
/*1E*/ s8 _unk1E;               //Unused - all existing WallAnimator objSetups have this set to 10
/*1F*/ s8 _unk1F;               //Unused - all existing WallAnimator objSetups have this set to 0
/*20*/ u8 findCommandRadius;    //Range for Find command to show up in inventory
/*21*/ u8 falloffRadius;        //Tagged shapes' vertices will be influenced if they're inside this radius (relative to the vertex nearest the WallAnimator)
/*22*/ s8 _unk22;               //Unused - all existing WallAnimator objSetups have this set to 60
/*23*/ u8 animatorID;           //Block shapes with this tag will be animated
/*24*/ s16 yaw;                 //NOTE: 16-bit precision, instead of setups' usual 8-bit yaw.
/*26*/ u8 unk26;                //Stored to `objData->unk28` during setup, but otherwise unused (all existing WallAnimator objSetups have this set to 40)
} WallAnimator_Setup;

typedef struct {
    Vec3f digVector;            //The direction vertices will be extruded along while digging (2D only)
    f32 zero;                   //Set to 0 during dig vector setup, but otherwise unused
    f32* vtxWeights;            //Influence values (from 0 to 1) for each vertex being animated, falling off from a central vertex. Vertices with nonzero weighting are all extruded to the same extent however, since these values aren't multipled in. (NOTE: stores a float for each vertex in the animatorID tagged shapes, not just the few with nonzero influence!)
    Vec3f closestVtxPosition;   //The position of the animatorID-tagged vertex closest to the WallAnimator, relative to the Block's origin. Used as a falloff centrepoint for influencing neighbouring vertices.
    s32 digDepth;               //Current dig progress (starts at 0, increases while digging)
    f32 falloffRadius;          //Tagged shapes' vertices will be influenced if they're inside this radius (relative to the vertex nearest the WallAnimator)
    f32 unk28;                  //Stores `objSetup->unk26` during setup, but otherwise unused
    s8 previousDigDepth;        //Previous dig progress value (vertex updates are queued when this differs from current dig value)
    u8 animatorID;              //Block shapes with this tag will be animated
    u8 sidekickCost;            //Tricky must have this much energy to dig the spot ("I'm too tired to do this right now!")
    u8 animUpdates;             //Used to queue vertex animation updates
    u8 digFinished;             //Boolean set when Tricky's finished digging through the wall
    s16 gamebitDug;             //Gamebit to set when Tricky digs through the wall
} WallAnimator_Data;

DLL_INTERFACE(DLL_355_WallAnimator) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ f32 (*tick_dig)(Object* self, Object* sidekick);
    /*8*/ u8 (*is_dig_finished)(Object* self);
    /*9*/ u8 (*get_energy_cost)(Object* self);
};

#endif // _DLLS_355_H

