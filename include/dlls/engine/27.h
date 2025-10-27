#ifndef _DLLS_27_H
#define _DLLS_27_H

#include "PR/ultratypes.h"
#include "sys/objhits.h"
#include "sys/math.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef enum {
    DLL27FLAG_NONE = 0x0,
    DLL27FLAG_1 = 0x1,
    DLL27FLAG_2 = 0x2,
    DLL27FLAG_4 = 0x4, // whether it collides with mobile maps?
    DLL27FLAG_HAS_HITS_COLLIDER = 0x8,
    DLL27FLAG_20 = 0x20,
    DLL27FLAG_80 = 0x80,
    DLL27FLAG_100 = 0x100,
    DLL27FLAG_200 = 0x200,
    DLL27FLAG_400 = 0x400,
    DLL27FLAG_800 = 0x800,
    DLL27FLAG_1000 = 0x1000,
    DLL27FLAG_HAS_TERRAIN_COLLIDER = 0x2000,
    DLL27FLAG_8000 = 0x8000,
    DLL27FLAG_40000 = 0x40000,
    DLL27FLAG_1000000 = 0x1000000,
    DLL27FLAG_2000000 = 0x2000000,
    DLL27FLAG_4000000 = 0x4000000,
    DLL27FLAG_8000000 = 0x8000000
} DLL27Flags;

typedef enum {
    DLL27MODE_DISABLED = 0,
    DLL27MODE_1 = 1,
    DLL27MODE_2 = 2
} DLL27Mode;

// size:0x260
typedef struct {
/*0x000*/ u32 flags; // collider configuration
/*0x004*/ Vec3f *unk4; // points used for terrain detection
/*0x008*/ Vec3f unk8[4]; // for terrain detection, derived from unk4
/*0x038*/ Vec3f unk38[4]; // for terrain detection, derived from unk8 (prev tick?)
/*0x068*/ Unk80027934 unk68; // terrain collision result
/*0x0D4*/ Object *unkD4; // parent object (only when touching?)
/*0x0D8*/ Vec3f *localHitsTestPoints; // points used for hit line detection, in local space relative to the owner
/*0x0DC*/ f32 *hitsTestRadii; // radii of hit line detection (smaller values means object can get closer to hit)
/*0x0E0*/ Vec3f unkE0[4]; // for hit line detection, relative to unkD4 (if a mobile map, otherwise world)
/*0x110*/ Vec3f unk110[4]; // for hit line detection, relative to unkD4 (if a mobile map, otherwise world) (prev tick?)
/*0x140*/ Func_80059C40_Struct unk140; // hit line collision result
          // Pitch/roll of floor normal, relative to object yaw
/*0x194*/ s16 relativeFloorPitchSmooth;
/*0x196*/ s16 relativeFloorRollSmooth;
/*0x198*/ s16 relativeFloorPitch;
/*0x19A*/ s16 relativeFloorRoll;
          // Surface normal of floor (updated only when actually touching the floor)
/*0x19C*/ f32 floorNormalX;
/*0x1A0*/ f32 floorNormalY;
/*0x1A4*/ f32 floorNormalZ;
/*0x1A8*/ f32 floorDist; // height above floor (usually caps out at 100)
/*0x1AC*/ f32 unk1AC; // ceiling Y pos? (never set for player?)
/*0x1B0*/ f32 underwaterDist; // how far under water
/*0x1B4*/ f32 floorY; // y pos of floor when a floor is detected (not necessarily touching)
/*0x1B8*/ f32 waterY; // y pos of detected water
/*0x1BC*/ f32 floorDistList[4]; // height above floor (usually caps out at 100)
/*0x1CC*/ f32 unk1CC[4]; // ceiling Y pos? (never set for player?)
/*0x1DC*/ f32 underwaterDistList[4]; // how far under water the checked point is
/*0x1EC*/ f32 floorYList[4]; // y pos of floor when a floor is detected (not necessarily touching)
/*0x1FC*/ f32 waterYList[4]; // y pos of detected water
          // Normal of water surface the object is in
/*0x20C*/ f32 waterNormalXList[4];
/*0x21C*/ f32 waterNormalYList[4];
/*0x22C*/ f32 waterNormalZList[4];
/*0x23C*/ AABBs32 aabb; // AABB for broadphase collision detection?
/*0x254*/ u8 boundsYExtension; // extends AABB y in both directions by this much
/*0x255*/ u8 _unk255[0x257 - 0x255];
/*0x257*/ s8 mode;
          // Upper 4-bits = number of terrain test points
          // Lower 4-bits = number of hits test points
/*0x258*/ u8 numTestPoints;
/*0x259*/ s8 unk259; // some config for hits detection
          // For each hits test point, a bit that is set if that test point collided with a hits line.
          // i.e. test point 0 is bit 0
/*0x25A*/ u8 hitsTouchBits;
/*0x25B*/ u8 unk25B; // always 0?
          // bit 0 & 4 when on ground
          // bit 1 when waking into terrain (map hits dont count)
/*0x25C*/ s8 unk25C;
          // bit 0 when walking into terrain (map hits dont count)
          // bit 1 when on ground
/*0x25D*/ s8 unk25D;
} DLL27_Data;

DLL_INTERFACE(DLL_27) {
    /*:*/ DLL_INTERFACE_BASE(DLL); 
    /*0*/ void (*init)(DLL27_Data *data, s32 flags1, s32 flags2, s32 mode);
    /*1*/ void (*setup_hits_collider)(DLL27_Data *data, s32 testCount, Vec3f *testPoints, f32 *testRadii, s32 a4);
    /*2*/ void (*setup_terrain_collider)(DLL27_Data *data, s32 testCount, Vec3f *testPoints, f32 *arg3, u8 *arg4);
    /*3*/ void (*func_1e8)(Object *object, DLL27_Data *data, f32 updateRate);
    /*4*/ void (*func_5a8)(Object *object, DLL27_Data *data);
    /*5*/ void (*func_624)(Object *object, DLL27_Data *data, f32 updateRate);
    /*6*/ Func_80057F1C_Struct *(*func_c7c)(Object *object, f32 arg1, f32 arg2, s32 *arg3, s32 arg4);
    /*7*/ void (*reset)(Object *object, DLL27_Data *data);
    /*8*/ f32 (*func_df4)(Object *object, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
};

#endif //_DLLS_27_H
