#ifndef _SYS_INTERSECT_H
#define _SYS_INTERSECT_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/math.h"

// size: 0x18
typedef struct {
/*00*/ f32 y; // y-coordinate
/*04*/ f32 norm[3]; // surface normal
/*10*/ struct Object *obj; // if for a mobile map, the map's object
/*14*/ s8 unk14; // material?
} TrackHeightResult;

// size: 0x54
typedef struct TrackLineIntersectResult {
    struct Object *unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    Vec4f unk1C;
    Vec3f unk2C;
    Vec3f unk38;
    f32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s8 unk50;
    s8 unk51;
    s8 unk52;
} TrackLineIntersectResult;

// size: 0x10
typedef struct Unk8005341C {
    Object *unk0;
    s16 unk4; // index into gPLlist
    MtxF *unk8;
    MtxF *unkC;
} Unk8005341C;

typedef struct UnkFunc80051D68Arg3 {
/*00*/ f32 unk0; // dot product of v0 and (n / 8191)
/*04*/ s16 nX; // normalX * 8191
/*06*/ s16 nY; // normalY * 8191
/*08*/ s16 nZ; // normalZ * 8191
/*0A*/ s16 vX[3]; // vertexX
/*10*/ s16 vY[3]; // vertexY
/*16*/ s16 vZ[3]; // vertexZ
/*1C*/ s16 unk1C[8];
/*2C*/ UNK_TYPE_8 pad2C;
/*2D*/ UNK_TYPE_8 pad2D;
/*2E*/ s8 unk2E;
/*2F*/ s8 unk2F;
/*30*/ u8 unk30;
/*31*/ s8 unk31;
/*32*/ u8 unk32;
/*33*/ u8 pad33;
} UnkFunc80051D68Arg3;

// size: 0x6C
typedef struct TrackIntersectResult {
    Vec4f unk0[4];
    f32 unk40[4];
    s8 unk50[4];
    s8 unk54[4];
    Object* unk58[4];
    s16 unk68;
} TrackIntersectResult;

void trackIntersectInit(void);
void trackIntersect_func_800533D8(s32* arg0, UnkFunc80051D68Arg3** arg1);
void trackIntersectGetBlockList(Vec3s32** blockList);
Unk8005341C* trackIntersectGetPLIndices(s32* count);
/** Fits an AABB around two cubes. */
void trackIntersectBuildAABB(AABBs32 *aabb, Vec3f *posArray1, Vec3f *posArray2, f32 *cubeRadiusArray, s32 arrayLength);
void trackIntersectBroadphase(Object* obj, AABBs32* aabb, u8 flags);
s32 trackGetIntersect(Object *obj, f32* prevPos, f32* currPos, s32 posCount, TrackIntersectResult* result, u8 flags);
s32 trackIntersect_func_80056BCC(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec4f* arg3, Vec3f* arg4, f32 arg5);
s32 trackGetHeight(Object* obj, f32 x, f32 y, f32 z, TrackHeightResult*** result, s32 arg5, s32 flags);
s32 trackGetHeightNearest(Object* obj, f32 x, f32 y, f32 z, f32* height, u8 flags);
s32 trackGetHeightFloor(Object* obj, f32 x, f32 y, f32 z, f32* height, u8 flags);
s32 trackGetHeightCeiling(Object* obj, f32 x, f32 y, f32 z, f32* height, u8 flags);
void trackIntersectMarkBlocksDirty(void);
int trackIntersectNeedsUpdate(void);
s8 trackIntersectInitialized(void);
void trackIntersectLastLineTick(void);
void trackToggleHitLine(s32 animatorID, Object* parentObject, s32 enableLines);
void trackIntersectTick(void);
void trackIntersectModLineBuild(ObjDef* objdef);
s32 trackGetLineIntersect(Vec3f* pos1, Vec3f* pos2, f32 arg2, s32 arg3, TrackLineIntersectResult* result, Object* obj, s8 arg6, s8 arg7, u8 arg8, s8 arg9);
void trackIntersect_func_8005B5B8(Object*, Object*, s32);

#endif
