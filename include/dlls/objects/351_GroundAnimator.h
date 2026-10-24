#ifndef _DLLS_351_H
#define _DLLS_351_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

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
    /*7*/ f32 (*TickDig)(Object* self, Object* sidekick);
    /*8*/ u8 (*IsDigFinished)(Object* self);
    /*9*/ u8 (*GetMagicCaveIndex)(Object* self);
};

#define dll_GroundAnimator(obj) (((DLL_351_GroundAnimator*)obj->dll)->vtbl)

#endif // _DLLS_351_H

