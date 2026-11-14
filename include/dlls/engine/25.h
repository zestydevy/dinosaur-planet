#ifndef _DLLS_25_H
#define _DLLS_25_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "dlls/engine/26_curves.h"
#include "game/objects/object.h"
#include "sys/curves.h"
#include "sys/math.h"

typedef struct {
    // The ID of the walk group.
    u8 id;
    // A bitfield that specifies which patch groups contain the point used in the walk group search.
    //
    // Bit 0 = patchIDs[0], bit 1 = patchIDs[1], etc.
    u8 patchFlags;
    // The IDs of the patch groups associated with each walk group link.
    u16 patchIDs[4];
} WalkGroupInfo;

DLL_INTERFACE(DLL_25) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*LoadWalkGroups)(void);
/*1*/ s32 (*FindWalkGroup)(Vec3f* pos, WalkGroupInfo* info);
/*2*/ s32 (*IsPointWithinAnyGroup)(Vec3f* pos);
/*3*/ s32 (*GetPatchGroupContainingPoint)(Vec3f* pos, s32 walkGroupID);
/*4*/ s32 (*IsPointWithinPatchGroup)(Vec3f* pos, s32 walkGroupID, s32 patchGroupID);
/*5*/ CurveSetup* (*FindNearestEnabledTrickyCurve)(Vec3f* pos, s32 arg1, s32 arg2);
/*6*/ CurveSetup* (*FindNearestTrickyCurve)(Vec3f* pos, s32 arg1, s32 arg2);
/*7*/ s32 (*GetClosestExitOfPatch)(Vec3f* pos, Vec3f* exitPos, s32 patchGroupID);
/*8*/ s32 (*func_1D30)(UnkCurvesStruct *arg0, CurveSetup *arg1, CurveSetup *arg2, CurveSetup *arg3);
/*9*/ s32 (*func_21F4)(UnkCurvesStruct *arg0, CurveSetup *arg1);
/*10*/ s32 (*func_27A8)(UnkCurvesStruct *arg0, CurveSetup *arg1);
/*11*/ void (*func_29FC)(UnkCurvesStruct* arg0);
// returns a CurveSetup of type 0x22
/*12*/ CurveSetup* (*func_2A50)(Object *obj, s32 arg1);
// returns a CurveSetup of type 0x22
/*13*/ CurveSetup* (*func_2BC4)(Object *obj, s32 arg1);
// returns a CurveSetup of type 0x22
/*14*/ CurveSetup* (*func_2CF8)(Object *obj, s32 arg1);
};

#endif //_DLLS_25_H


