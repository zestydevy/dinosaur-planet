#ifndef _DLLS_27_H
#define _DLLS_27_H

#include "PR/ultratypes.h"
#include "sys/objhits.h"
#include "sys/math.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

// size:0x260
typedef struct {
/*0x000*/ u32 unk0; // flags
/*0x004*/ Vec3f *unk4;
/*0x008*/ Vec3f unk8[4];
/*0x038*/ Vec3f unk38[4];
/*0x068*/ Unk80027934 unk68;
/*0x0D4*/ Object *unkD4; // parent object (only when touching?)
/*0x0D8*/ Vec3f *unkD8;
/*0x0DC*/ f32 *unkDC;
/*0x0E0*/ Vec3f unkE0[4]; // relative to unkD4 (if any, otherwise world)
/*0x110*/ Vec3f unk110[4]; // relative to unkD4 (if any, otherwise world)
/*0x140*/ Func_80059C40_Struct unk140;
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
/*0x1CC*/ f32 unk1CC[4];; // ceiling Y pos? (never set for player?)
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
/*0x257*/ s8 unk257; // mode?
/*0x258*/ u8 unk258; // upper and lower 4 bits contain array lengths
/*0x259*/ s8 unk259;
/*0x25A*/ u8 unk25A; // 1 when walking into hit line
/*0x25B*/ u8 unk25B; // always 0?
          // bit 0 & 4 when on ground
          // bit 1 when waking into terrain (map hits dont count)
/*0x25C*/ s8 unk25C;
          // bit 0 when walking into terrain (map hits dont count)
          // bit 1 when on ground
/*0x25D*/ s8 unk25D;
} DLL27_Data;

//Just a placeholder name - seems to have something to do with applying procedural head turn animations
DLL_INTERFACE(DLL_27_head_turn) {
    /*:*/ DLL_INTERFACE_BASE(DLL); 
    /*0*/ void (*head_turn_func_18)(DLL27_Data *data, s32 a1, s32 a2, s32 a3);
    /*1*/ void (*head_turn_func_84)(DLL27_Data *data, s32 a1, Vec3f *a2, f32 *a3, s32 a4);
    /*2*/ void (*head_turn_func_c0)(DLL27_Data *data, s32 arg1, Vec3f *arg2, f32 *arg3, u8 *arg4);
    /*3*/ void (*head_turn_func_1e8)(Object *object, DLL27_Data *data, f32 delayFloat);
    /*4*/ void (*head_turn_func_5a8)(Object *object, DLL27_Data *data);
    /*5*/ void (*head_turn_func_624)(Object *object, DLL27_Data *data, f32 delayFloat);
    /*6*/ Func_80057F1C_Struct* (*head_turn_func_c7c)(Object *object, f32 arg1, f32 arg2, s32 *arg3, s32 arg4);
    /*7*/ void (*head_turn_func_fb8)(Object *object, DLL27_Data *data);
    /*8*/ f32 (*head_turn_func_df4)(Object *object, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
};

#endif //_DLLS_27_H
