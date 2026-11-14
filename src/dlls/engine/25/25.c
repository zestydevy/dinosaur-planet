#include "dlls/engine/25.h"

#include "PR/os.h"
#include "PR/ultratypes.h"
#include "libc/string.h"
#include "dlls/engine/26_curves.h"
#include "game/objects/object.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "constants.h"
#include "dll.h"
#include "macros.h"

#define MAX_WALKGROUPS 217
#define MAX_PATCHGROUPS 256

typedef struct {
/*00*/ s8 dirs[8];
/*08*/ f32 dots[4];
/*18*/ s16 yMax;
/*1A*/ s16 yMin;
/*1C*/ u8 linkPatchIndices[4];
} WalkGroup;

typedef struct {
/*00*/ s8 dirs[8];
/*08*/ f32 dots[4];
/*18*/ s16 yMax;
/*1A*/ s16 yMin;
/*1C*/ u16 id;
/*1E*/ s16 exit0X;
/*20*/ s16 exit0Z;
/*22*/ s16 exit1X;
/*24*/ s16 exit1Z;
} PatchGroup;

/* 0x00 */ static u8 sLoadedWalkGroups[MAX_WALKGROUPS];
/* 0xDC */ static WalkGroup* sWalkGroups;
/* 0xE0 */ static PatchGroup* sPatchGroups;
/* 0xE4 */ static s32 sNextPatchIdx;
/* 0xE8 */ static u8 _bss_E8[0x8];

static s32 dll_25_isPointWithinWalkGroupXZ(WalkGroup* arg0, f32 arg1, f32 arg2);
static void dll_25_calcPlane(Vec3f* p1, Vec3f* p2, s8* dir, f32* dot);

// offset: 0x0 | ctor
void dll_25_ctor(s32 arg0) {
    bzero(sLoadedWalkGroups, MAX_WALKGROUPS);
    sWalkGroups = mmAlloc(sizeof(WalkGroup) * MAX_WALKGROUPS, ALLOC_TAG_TRACK_COL, ALLOC_NAME("walkGroup:planes"));
    sPatchGroups = mmAlloc(sizeof(PatchGroup) * MAX_PATCHGROUPS, ALLOC_TAG_TRACK_COL, ALLOC_NAME("patchGroup:planes"));
}

// offset: 0x84 | dtor
void dll_25_dtor(s32 arg0) {
    mmFree(sWalkGroups);
    mmFree(sPatchGroups);
}

// offset: 0xD8 | func: 0 | export: 0
void dll_25_LoadWalkGroups(void) {
    WalkGroup* walkGroup;
    UNUSED u32 sp274[256] = {0};
    s32 i;
    WalkGroup* g1;
    WalkGroup* g2;
    s32 curveListCount;
    CurveNode* curveList;
    CurveSetup* curve;
    s32 walkGroupNo;
    Vec3f p1;
    Vec3f p2;
    f32 y1;
    f32 y2;
    s32 k;
    CurveSetup* linkCurve;
    // @bug? the size of this array should probably be (MAX_PATCHGROUPS * 2)
    u8 walkGroupConnections[216 * 2]; // TODO: exact size is unclear
    s32 patchGroupID;
    s32 linkno;
    f32 dir[2];

    curveList = gDLL_26_Curves->vtbl->GetCurves(&curveListCount);
    bzero(sLoadedWalkGroups, MAX_WALKGROUPS);
    for (i = 0; i < MAX_PATCHGROUPS; i++) {
        sPatchGroups[i].id = 0;
    }
    sNextPatchIdx = 1;
    for (i = 0; i < curveListCount; i++) {
        curve = curveList[i].setup;
        if (curve->curveType == 0x26) {
            /* default.dol
            if (curve->unk3 >= MAX_WALKGROUPS) {
                STUBBED_PRINTF("Maximum walk group error (increase MAX_WALKGROUPS)\n");
                *((volatile int*)NULL) = 0; // kaboom
            }
            if (sLoadedWalkGroups[curve->unk3] != 0) {
                STUBBED_PRINTF("Walk group number %d enterd twice at %f %f %f\n",
                    curve->unk3, curve->pos.x, curve->pos.y, curve->pos.z);
                return;
            }
            */
            walkGroupNo = curve->unk3;
            walkGroup = &sWalkGroups[walkGroupNo];
            sLoadedWalkGroups[walkGroupNo] = 1;
            // Precalculate plane info for the walk group's walls (for later bounds checks)
            p1.f[0] = (curve->base_type26.unk4 * 5.7f) + curve->pos.x;
            p1.f[2] = (curve->base_type26.unk5 * 5.7f) + curve->pos.z;
            p2.f[0] = (curve->base_type26.unk6 * 5.7f) + curve->pos.x;
            p2.f[2] = (curve->base_type26.unk7 * 5.7f) + curve->pos.z;
            dll_25_calcPlane(&p1, &p2, &walkGroup->dirs[0], &walkGroup->dots[0]);
            p1.f[0] = (curve->type26.unk30 * 5.7f) + curve->pos.x;
            p1.f[2] = (curve->type26.unk31 * 5.7f) + curve->pos.z;
            dll_25_calcPlane(&p2, &p1, &walkGroup->dirs[2], &walkGroup->dots[1]);
            p2.f[0] = (curve->type26.unk32 * 5.7f) + curve->pos.x;
            p2.f[2] = (curve->type26.unk33 * 5.7f) + curve->pos.z;
            dll_25_calcPlane(&p1, &p2, &walkGroup->dirs[4], &walkGroup->dots[2]);
            p1.f[0] = (curve->base_type26.unk4 * 5.7f) + curve->pos.x;
            p1.f[2] = (curve->base_type26.unk5 * 5.7f) + curve->pos.z;
            dll_25_calcPlane(&p2, &p1, &walkGroup->dirs[6], &walkGroup->dots[3]);

            walkGroup->yMax = curve->pos.y + (2.0f * curve->unk18);
            walkGroup->yMin = curve->pos.y - (2.0f * curve->unk1A);

            for (linkno = 0; linkno < 4; linkno++) {
                walkGroup->linkPatchIndices[linkno] = 0;
                if (curve->links[linkno] >= 0) {
                    linkCurve = gDLL_26_Curves->vtbl->FindByUID(curve->links[linkno]);
                    patchGroupID = curve->unk3 * linkCurve->unk3;
                    for (k = 1; k < sNextPatchIdx; k++) {
                        if (patchGroupID == sPatchGroups[k].id) {
                            walkGroup->linkPatchIndices[linkno] = k;
                            break;
                        }
                    }
                    if (walkGroup->linkPatchIndices[linkno] == 0) {
                        for (k = 0; k < 4; k++) {
                            if ((s32)curve->uID == linkCurve->links[k]) {
                                break;
                            }
                        }
                        walkGroup->linkPatchIndices[linkno] = sNextPatchIdx;
                        sPatchGroups[sNextPatchIdx].id = patchGroupID;
                        walkGroupConnections[sNextPatchIdx * 2 + 0] = curve->unk3;
                        walkGroupConnections[sNextPatchIdx * 2 + 1] = linkCurve->unk3;
                        // Precalculate plane info for the patch group's walls (for later bounds checks)
                        p1.f[0] = (curve->type26.unk34[linkno][0] * 5.7f) + curve->pos.x;
                        p1.f[2] = (curve->type26.unk34[linkno][1] * 5.7f) + curve->pos.z;
                        p2.f[0] = (curve->type26.unk34[linkno][2] * 5.7f) + curve->pos.x;
                        p2.f[2] = (curve->type26.unk34[linkno][3] * 5.7f) + curve->pos.z;
                        sPatchGroups[sNextPatchIdx].exit0X = (p1.f[0] + p2.f[0]) / 2;
                        sPatchGroups[sNextPatchIdx].exit0Z = (p1.f[2] + p2.f[2]) / 2;
                        dll_25_calcPlane(&p1, &p2, &sPatchGroups[sNextPatchIdx].dirs[0], &sPatchGroups[sNextPatchIdx].dots[0]);
                        p1.f[0] = (linkCurve->type26.unk34[k][0] * 5.7f) + linkCurve->pos.x;
                        p1.f[2] = (linkCurve->type26.unk34[k][1] * 5.7f) + linkCurve->pos.z;
                        dll_25_calcPlane(&p2, &p1, &sPatchGroups[sNextPatchIdx].dirs[2], &sPatchGroups[sNextPatchIdx].dots[1]);
                        p2.f[0] = (linkCurve->type26.unk34[k][2] * 5.7f) + linkCurve->pos.x;
                        p2.f[2] = (linkCurve->type26.unk34[k][3] * 5.7f) + linkCurve->pos.z;
                        sPatchGroups[sNextPatchIdx].exit1X = (p1.f[0] + p2.f[0]) / 2;
                        sPatchGroups[sNextPatchIdx].exit1Z = (p1.f[2] + p2.f[2]) / 2;
                        dll_25_calcPlane(&p1, &p2, &sPatchGroups[sNextPatchIdx].dirs[4], &sPatchGroups[sNextPatchIdx].dots[2]);
                        p1.f[0] = (curve->type26.unk34[linkno][0] * 5.7f) + curve->pos.x;
                        p1.f[2] = (curve->type26.unk34[linkno][1] * 5.7f) + curve->pos.z;
                        dll_25_calcPlane(&p2, &p1, &sPatchGroups[sNextPatchIdx].dirs[6], &sPatchGroups[sNextPatchIdx].dots[3]);
                        y1 = curve->pos.y + (2.0f * curve->unk18);
                        y2 = linkCurve->pos.y + (2.0f * linkCurve->unk18);
                        if (y2 < y1) {
                            sPatchGroups[sNextPatchIdx].yMax = y1;
                        } else {
                            sPatchGroups[sNextPatchIdx].yMax = y2;
                        }
                        y1 = curve->pos.y - (2.0f * curve->unk1A);
                        y2 = linkCurve->pos.y - (2.0f * linkCurve->unk1A);
                        if (y1 < y2) {
                            sPatchGroups[sNextPatchIdx].yMin = y1;
                        } else {
                            sPatchGroups[sNextPatchIdx].yMin = y2;
                        }
                        sNextPatchIdx += 1;
                    }
                }
            }
        }
    }
    for (i = 1; i < sNextPatchIdx; i++) {
        g1 = &sWalkGroups[walkGroupConnections[i * 2]];
        g2 = &sWalkGroups[walkGroupConnections[i * 2 + 1]];
        dir[0] = sPatchGroups[i].exit1X - sPatchGroups[i].exit0X;
        dir[1] = sPatchGroups[i].exit1Z - sPatchGroups[i].exit0Z;
        while (dll_25_isPointWithinWalkGroupXZ(g1, sPatchGroups[i].exit0X, sPatchGroups[i].exit0Z) == 0 && 
               dll_25_isPointWithinWalkGroupXZ(g2, sPatchGroups[i].exit0X, sPatchGroups[i].exit0Z) == 0) {
            sPatchGroups[i].exit0X += dir[0] / 20.0f;
            sPatchGroups[i].exit0Z += dir[1] / 20.0f;
        }
        while (dll_25_isPointWithinWalkGroupXZ(g1, sPatchGroups[i].exit1X, sPatchGroups[i].exit1Z) == 0 && 
               dll_25_isPointWithinWalkGroupXZ(g2, sPatchGroups[i].exit1X, sPatchGroups[i].exit1Z) == 0) {
            sPatchGroups[i].exit1X -= dir[0] / 20.0f;
            sPatchGroups[i].exit1Z -= dir[1] / 20.0f;
        }
    }
}

// offset: 0xD00 | func: 1
static void dll_25_calcPlane(Vec3f* p1, Vec3f* p2, s8* dir, f32* dot) {
    f32 magnitude;
    f32 z;
    f32 x;

    z = p2->z - p1->z;
    x = p1->x - p2->x;
    magnitude = sqrtf((z * z) + (x * x));
    if (magnitude != 0.0f) {
        z /= magnitude;
        x /= magnitude;
    }
    dir[0] = z * 127.0f;
    dir[1] = x * 127.0f;
    dot[0] = -((dir[0] * p1->f[0]) + (dir[1] * p1->f[2])); // negated dot product
}

// offset: 0xDF8 | func: 2
static s32 dll_25_isPointWithinWalkGroupXZ(WalkGroup* group, f32 x, f32 z) {
    u8 j;
    u8 k;

    for (j = 0, k = 0; j < 4; j++, k += 2) {
        if (((group->dirs[k] * x) + (group->dirs[k + 1] * z) + group->dots[j]) > 0.0f) {
            break;
        }
    }
    if (j == 4) {
        return 1;
    }
    return 0;
}

// offset: 0xEAC | func: 3
static s32 dll_25_findWalkGroupFast(Vec3f* pos) {
    /*0x400*/ static u32 sLastFoundWalkGroup = 0;
    s16 g2;
    s16 g1;
    WalkGroup* group;

    /*
        I think the idea behind this function is to bias the search toward walk groups near the previously
        found group so that repeated searches don't take as long (assuming that nearby groups have IDs that
        are numerically close to each other, if the curves are sorted that way in the map object list).
    */

    g1 = sLastFoundWalkGroup;
    if (sLastFoundWalkGroup == (MAX_WALKGROUPS - 1)) {
        g2 = 0;
    } else {
        g2 = sLastFoundWalkGroup + 1;
    }
    while (g1 != g2) {
        if (sLoadedWalkGroups[g1] != 0) {
            group = &sWalkGroups[g1];
            if ((pos->y < group->yMax) && (group->yMin < pos->y) && (dll_25_isPointWithinWalkGroupXZ(group, pos->x, pos->z) != 0)) {
                sLastFoundWalkGroup = g1;
                return g1;
            }
        }
        if (sLoadedWalkGroups[g2] != 0) {
            group = &sWalkGroups[g2];
            if ((pos->y < group->yMax) && (group->yMin < pos->y) && (dll_25_isPointWithinWalkGroupXZ(group, pos->x, pos->z) != 0)) {
                sLastFoundWalkGroup = g2;
                return g2;
            }
        }
        g1--;
        if (g1 == -1) {
            g1 = MAX_WALKGROUPS - 1;
        }
        g2++;
        if (g2 == MAX_WALKGROUPS) {
            g2 = 0;
        }
    }

    if (sLoadedWalkGroups[g1] != 0) {
        group = &sWalkGroups[g1];
        if ((pos->y < group->yMax) && (group->yMin < pos->y) && (dll_25_isPointWithinWalkGroupXZ(group, pos->x, pos->z) != 0)) {
            sLastFoundWalkGroup = g1;
            return g1;
        }
    }
    return 0;
}

// offset: 0x1158 | func: 4 | export: 1
s32 dll_25_FindWalkGroup(Vec3f* pos, WalkGroupInfo* info) {
    u8 walkGroup;
    u8 j;
    u8 linkno;
    u8 flag;
    u8 patchIdx;
    u8 k;
    PatchGroup* patch;

    walkGroup = dll_25_findWalkGroupFast(pos);
    if (info != NULL && walkGroup != 0) {
        info->id = walkGroup;
        info->patchFlags = 0;
        for (linkno = 0, flag = 1; linkno < 4; linkno++, flag <<= 1) {
            patchIdx = sWalkGroups[walkGroup].linkPatchIndices[linkno];
            if (patchIdx != 0) {
                patch = &sPatchGroups[patchIdx];
                info->patchIDs[linkno] = patch->id;
                for (k = 0, j = 0; k < 4; k++, j += 2) {
                    if (((patch->dirs[j] * pos->x) + (patch->dirs[j + 1] * pos->z) + patch->dots[k]) > 0.0f) {
                        break;
                    }
                }
                if (k == 4) {
                    info->patchFlags |= flag;
                }
            } else {
                info->patchIDs[linkno] = 0;
            }
        }
    }
    return walkGroup;
}

// offset: 0x12FC | func: 5 | export: 2
s32 dll_25_IsPointWithinAnyGroup(Vec3f* pos) {
    PatchGroup* patch;
    s16 k;
    s16 patchIdx;
    s16 j;

    if (dll_25_findWalkGroupFast(pos) != 0) {
        return 1;
    }
    patchIdx = 1;
    while (patchIdx < sNextPatchIdx) {
        patch = &sPatchGroups[patchIdx];
        if (pos->y < patch->yMax && patch->yMin < pos->y) {
            for (k = 0, j = 0; k < 4; k++, j += 2) {
                if (((patch->dirs[j] * pos->x) + (patch->dirs[j + 1] * pos->z) + patch->dots[k]) > 0.0f) {
                    break;
                }
            }
            if (k == 4) {
                return 1;
            }
        }
        patchIdx++;
    }
    return 0;
}

// offset: 0x1474 | func: 6 | export: 3
s32 dll_25_GetPatchGroupContainingPoint(Vec3f* pos, s32 walkGroupID) {
    PatchGroup* patch;
    u8 k;
    u8 j;
    u8 linkno;
    u8 patchIdx;

    for (linkno = 0; linkno < 4; linkno++) {
        if (sLoadedWalkGroups[walkGroupID] != 0) {
            patchIdx = sWalkGroups[walkGroupID].linkPatchIndices[linkno];
            if (patchIdx != 0) {
                patch = &sPatchGroups[patchIdx];
                for (k = 0, j = 0; k < 4; k++, j += 2) {
                    if (((patch->dirs[j] * pos->x) + (patch->dirs[j + 1] * pos->z) + patch->dots[k]) > 0.0f) {
                        break;
                    }
                }
                if (k == 4) {
                    return patch->id;
                }
            }
        }
    }

    return 0;
}

// offset: 0x15B0 | func: 7 | export: 4
s32 dll_25_IsPointWithinPatchGroup(Vec3f* pos, s32 walkGroupID, s32 patchGroupID) {
    PatchGroup* patch;
    u8 linkno;
    u8 k;
    u8 j;
    u8 patchIdx;

    for (linkno = 0; linkno < 4; linkno++) {
        patchIdx = sWalkGroups[walkGroupID].linkPatchIndices[linkno];
        if (patchIdx != 0) {
            patch = &sPatchGroups[patchIdx];
            if (patchGroupID == patch->id) {
                for (k = 0, j = 0; k < 4; k++, j += 2) {
                    if (((patch->dirs[j] * pos->x) + (patch->dirs[j + 1] * pos->z) + patch->dots[k]) > 0.0f) {
                        break;
                    }
                }
                if (k == 4) {
                    return 1;
                }
                return 0;
            }
        }
    }

    STUBBED_PRINTF("Error in isPointWithinPatchGroup\n");
    return 0;
}

// offset: 0x16D4 | func: 8 | export: 7
s32 dll_25_GetClosestExitOfPatch(Vec3f* pos, Vec3f* exitPos, s32 patchGroupID) {
#ifdef AVOID_UB
    s32 i;
#else
    u8 i;
#endif
    f32 tempDistance;
    s32 pad;
    PRAGMA_IGNORE_PUSH("-Wtype-limits");
    for (i = 0; i < MAX_PATCHGROUPS; i++) { // @bug: This condition is always true!
        if (patchGroupID == sPatchGroups[i].id) {
            break;
        }
    }
    PRAGMA_IGNORE_POP()
#ifdef AVOID_UB
    if (i == MAX_PATCHGROUPS) {
        return 0;
    }
#endif
    exitPos->x = sPatchGroups[i].exit0X;
    exitPos->y = pos->y;
    exitPos->z = sPatchGroups[i].exit0Z;
    tempDistance = vec3DistanceSquared(pos, exitPos);
    exitPos->x = sPatchGroups[i].exit1X;
    exitPos->z = sPatchGroups[i].exit1Z;
    if (vec3DistanceSquared(pos, exitPos) < tempDistance) {
        return 1;
    }

    exitPos->x = sPatchGroups[i].exit0X;
    exitPos->z = sPatchGroups[i].exit0Z;
    return 1;
}

// offset: 0x1864 | func: 9 | export: 5
CurveSetup* dll_25_FindNearestEnabledTrickyCurve(Vec3f* pos, s32 arg1, s32 arg2) {
    f32 zDiff;
    f32 dist;
    f32 xDiff;
    f32 yDiff;
    f32 bestDist;
    s32 i;
    s32 curveListCount;
    CurveSetup* curve;
    CurveNode* curveList;
    CurveSetup* out;

    curveList = gDLL_26_Curves->vtbl->GetCurves(&curveListCount);
    out = NULL;
    bestDist = F32_MAX;
    for (i = 0; i < curveListCount; i++) {
        curve = curveList[i].setup;
        if ((curve != NULL) && 
                (curve->curveType == 0x24) && 
                ((arg1 == -1) || (arg1 == curve->unk3)) && 
                ((arg2 == -1) || (arg2 == curve->unk1A)) && 
                ((curve->links[0] != -1) || (curve->unk2F == -1)) && 
                ((curve->type24.activeBit == -1) || (mainGetBits(curve->type24.activeBit) != 0)) && 
                ((curve->type24.usedBit == -1) || (mainGetBits(curve->type24.usedBit) == 0))) {
            xDiff = pos->f[0] - curve->pos.x;
            yDiff = pos->f[1] - curve->pos.y;
            zDiff = pos->f[2] - curve->pos.z;
            dist = (xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff);
            if (dist < bestDist) {
                bestDist = dist;
                out = curve;
            }
        }
    }
    return out;
}

// offset: 0x1A2C | func: 10 | export: 6
CurveSetup* dll_25_FindNearestTrickyCurve(Vec3f* pos, s32 arg1, s32 arg2) {
    CurveNode* curveList;
    CurveSetup* curve;
    CurveSetup* out;
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;
    s32 curveListCount;
    f32 bestDist;
    s32 i;

    curveList = gDLL_26_Curves->vtbl->GetCurves(&curveListCount);
    out = NULL;
    bestDist = F32_MAX;
    for (i = 0; i < curveListCount; i++) {
        curve = curveList[i].setup;
        if ((curve != NULL) && 
                (curve->curveType == 0x24) && 
                ((arg1 == -1) || (arg1 == curve->unk3)) && 
                ((arg2 == -1) || (arg2 == curve->unk1A)) && 
                ((curve->links[0] != -1) || (curve->unk2F == -1))) {
            xDiff = pos->f[0] - curve->pos.x;
            yDiff = pos->f[1] - curve->pos.y;
            zDiff = pos->f[2] - curve->pos.z;
            if (((xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff)) < bestDist) {
                out = curve;
                bestDist = (xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff);
            }
        }
    }
    return out;
}

// offset: 0x1D30 | func: 11 | export: 8
s32 dll_25_func_1D30(UnkCurvesStruct* arg0, CurveSetup* arg1, CurveSetup* arg2, CurveSetup* arg3) {
    if (arg0->unk0.unk80 != 0) {
        arg0->unkA0 = arg1;
        arg0->unkA4 = arg2;
        arg0->unkA8[0] = arg2->pos.x;
        arg0->unkA8[1] = arg1->pos.x;
        arg0->unkA8[2] = 2.0f * (mathSinfInterp(arg2->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkA8[3] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkC8[0] = arg0->unkA4->pos.y;
        arg0->unkC8[1] = arg0->unkA0->pos.y;
        arg0->unkC8[2] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2D << 8) * arg0->unkA4->unk2E);
        arg0->unkC8[3] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2D << 8) * arg0->unkA0->unk2E);
        arg0->unkE8[0] = arg0->unkA4->pos.z;
        arg0->unkE8[1] = arg0->unkA0->pos.z;
        arg0->unkE8[2] = 2.0f * (mathCosfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkE8[3] = 2.0f * (mathCosfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
    } else {
        arg0->unkA0 = arg1;
        arg0->unkA4 = arg2;
        arg0->unkB8[0] = arg1->pos.x;
        arg0->unkB8[1] = arg2->pos.x;
        arg0->unkB8[2] = 2.0f * (mathSinfInterp(arg1->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkB8[3] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkD8[0] = arg0->unkA0->pos.y;
        arg0->unkD8[1] = arg0->unkA4->pos.y;
        arg0->unkD8[2] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2D << 8) * arg0->unkA0->unk2E);
        arg0->unkD8[3] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2D << 8) * arg0->unkA4->unk2E);
        arg0->unkF8[0] = arg0->unkA0->pos.z;
        arg0->unkF8[1] = arg0->unkA4->pos.z;
        arg0->unkF8[2] = 2.0f * (mathCosfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkF8[3] = 2.0f * (mathCosfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
    }
    if (gDLL_25->vtbl->func_21F4(arg0, arg3) != 0) {
        return 1;
    }
    arg0->unk0.splineFunc = curvesHermite;
    arg0->unk0.splineConverterFunc = curvesHermiteConverter;
    arg0->unk0.unk84 = arg0->unkA8;
    arg0->unk0.unk88 = arg0->unkC8;
    arg0->unk0.unk8C = arg0->unkE8;
    arg0->unk0.numControlPoints = 8;
    curvesMove(&arg0->unk0);
    return 0;
}

// offset: 0x21F4 | func: 12 | export: 9
s32 dll_25_func_21F4(UnkCurvesStruct* arg0, CurveSetup* arg1) {
    if ((arg0->unkA0 == NULL) || (arg0->unkA4 == NULL) || (arg1 == NULL)) {
        return 1;
    }

    if (arg0->unk0.unk80 != 0) {
        arg0->unk9C = arg0->unkA0;
        arg0->unkA0 = arg0->unkA4;
        arg0->unkA4 = arg1;
        memcpy(arg0->unkB8, arg0->unkA8, sizeof(arg0->unkA8));
        memcpy(arg0->unkD8, arg0->unkC8, sizeof(arg0->unkC8));
        memcpy(arg0->unkF8, arg0->unkE8, sizeof(arg0->unkE8));
        arg0->unkA8[0] = arg0->unkA4->pos.x;
        arg0->unkA8[1] = arg0->unkA0->pos.x;
        arg0->unkA8[2] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkA8[3] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkC8[0] = arg0->unkA4->pos.y;
        arg0->unkC8[1] = arg0->unkA0->pos.y;
        arg0->unkC8[2] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2D << 8) * arg0->unkA4->unk2E);
        arg0->unkC8[3] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2D << 8) * arg0->unkA0->unk2E);
        arg0->unkE8[0] = arg0->unkA4->pos.z;
        arg0->unkE8[1] = arg0->unkA0->pos.z;
        arg0->unkE8[2] = 2.0f * (mathCosfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkE8[3] = 2.0f * (mathCosfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        if (arg0->unk0.numControlPoints != 0) {
            curvesSetupMoveNetworkCurve(&arg0->unk0);
            if (arg0->unk0.unk0 <= 0.0f) {
                arg0->unk0.unk0 = 0.01f;
            }
        }
    } else {
        arg0->unk9C = arg0->unkA0;
        arg0->unkA0 = arg0->unkA4;
        arg0->unkA4 = arg1;
        memcpy(arg0->unkA8, arg0->unkB8, sizeof(arg0->unkA8));
        memcpy(arg0->unkC8, arg0->unkD8, sizeof(arg0->unkC8));
        memcpy(arg0->unkE8, arg0->unkF8, sizeof(arg0->unkE8));
        arg0->unkB8[0] = arg0->unkA0->pos.x;
        arg0->unkB8[1] = arg0->unkA4->pos.x;
        arg0->unkB8[2] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkB8[3] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkD8[0] = arg0->unkA0->pos.y;
        arg0->unkD8[1] = arg0->unkA4->pos.y;
        arg0->unkD8[2] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2D << 8) * arg0->unkA0->unk2E);
        arg0->unkD8[3] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2D << 8) * arg0->unkA4->unk2E);
        arg0->unkF8[0] = arg0->unkA0->pos.z;
        arg0->unkF8[1] = arg0->unkA4->pos.z;
        arg0->unkF8[2] = 2.0f * (mathCosfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkF8[3] = 2.0f * (mathCosfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        if (arg0->unk0.numControlPoints != 0) {
            curvesSetupMoveNetworkCurve(&arg0->unk0);
            if (arg0->unk0.unk0 >= 1.0f) {
                arg0->unk0.unk0 = 0.99f;
            }
        }
    }
    return 0;
}

// offset: 0x27A8 | func: 13 | export: 10
s32 dll_25_func_27A8(UnkCurvesStruct* arg0, CurveSetup* arg1) {
    if ((arg0->unkA0 == 0) || (arg0->unkA4 == NULL) || (arg1 == NULL)) {
        return 1;
    }
    arg0->unkA4 = arg1;
    if (arg0->unk0.unk80 != 0) {
        arg0->unkA8[0] = arg1->pos.x;
        arg0->unkA8[2] = 2.0f * (mathSinfInterp(arg1->unk2C << 8) * arg1->unk2E);
        arg0->unkC8[0] = arg1->pos.y;
        arg0->unkC8[2] = 2.0f * (mathSinfInterp(arg1->unk2D << 8) * arg1->unk2E);
        arg0->unkE8[0] = arg1->pos.z;
        arg0->unkE8[2] = 2.0f * (mathCosfInterp(arg1->unk2C << 8) * arg1->unk2E);
    } else {
        arg0->unkB8[1] = arg1->pos.x;
        arg0->unkB8[3] = 2.0f * (mathSinfInterp(arg1->unk2C << 8) * arg1->unk2E);
        arg0->unkD8[1] = arg1->pos.y;
        arg0->unkD8[3] = 2.0f * (mathSinfInterp(arg1->unk2D << 8) * arg1->unk2E);
        arg0->unkF8[1] = arg1->pos.z;
        arg0->unkF8[3] = 2.0f * (mathCosfInterp(arg1->unk2C << 8) * arg1->unk2E);
    }
    return 0;
}

// offset: 0x29FC | func: 14 | export: 11
void dll_25_func_29FC(UnkCurvesStruct* arg0) {
    arg0->unk9C = (CurveSetup*)((s32)arg0->unk9C ^ (s32)arg0->unkA4);\
    arg0->unkA4 = (CurveSetup*)((s32)arg0->unkA4 ^ (s32)arg0->unk9C);\
    arg0->unk9C = (CurveSetup*)((s32)arg0->unk9C ^ (s32)arg0->unkA4);
    if (arg0->unk0.unk0 >= 1.0f) {
        arg0->unk0.unk0 = 0.99f;
    }
}

// offset: 0x2A50 | func: 15 | export: 12
// returns a CurveSetup of type 0x22
CurveSetup* dll_25_func_2A50(Object* obj, s32 flightGroup) {
    CurveNode* result;
    CurveSetup* curve;
    CurveSetup* out;
    s32 count;
    f32 dist;
    f32 bestDist;
    s32 i;

    result = gDLL_26_Curves->vtbl->GetCurves(&count);
    out = NULL;
    bestDist = 1e12f;
    for (i = 0; i < count; i++) {
        curve = result[i].setup;
        if ((curve->curveType == 0x22) && 
                ((flightGroup == curve->base_type22.unk4) || ((curve->unk1A < 3) && (flightGroup == -1))) && 
                ((curve->type22.activeBit == -1) || (mainGetBits(curve->type22.activeBit) != 0)) && 
                ((curve->type22.usedBit == -1) || (mainGetBits(curve->type22.usedBit) == 0))) {
            dist = vec3DistanceSquared(&obj->globalPosition, &curve->pos);
            if (dist < bestDist) {
                bestDist = dist;
                out = curve;
            }
        }
    }
    return out;
}

// offset: 0x2BC4 | func: 16 | export: 13
// returns a CurveSetup of type 0x22
CurveSetup* dll_25_func_2BC4(Object* obj, s32 flightGroup) {
    CurveNode* result;
    CurveSetup* curve;
    CurveSetup* out;
    s32 count;
    f32 dist;
    f32 bestDist;
    s32 i;

    result = gDLL_26_Curves->vtbl->GetCurves(&count);
    out = NULL;
    i = 0;
    bestDist = 1e12f;
    for (i = 0; i < count; i++) {
        curve = result[i].setup;
        if ((curve->curveType == 0x22) && ((flightGroup == curve->base_type22.unk4) || ((curve->unk1A < 3) && (flightGroup == -1)))) {
            dist = vec3DistanceSquared(&obj->globalPosition, &curve->pos);
            if (dist < bestDist) {
                bestDist = dist;
                out = curve;
            }
        }
    }
    return out;
}

// offset: 0x2CF8 | func: 17 | export: 14
CurveSetup* dll_25_func_2CF8(Object* obj, s32 arg1) {
    CurveNode* result;
    s32 _pad;
    CurveSetup* curve;
    s32 count;
    CurveSetup* out;
    f32 dist;
    f32 bestDist;
    s32 i;

    result = gDLL_26_Curves->vtbl->GetCurves(&count);
    out = NULL;
    bestDist = 1e12f;
    for (i = 0; i < count; i++) {
        curve = result[i].setup;
        if ((curve->curveType == 0x22) && 
                (arg1 == curve->unk1A) && 
                ((curve->type22.activeBit == -1) || (mainGetBits(curve->type22.activeBit) != 0)) && 
                ((curve->type22.usedBit == -1) || (mainGetBits(curve->type22.usedBit) == 0))) {
            dist = vec3DistanceSquared(&obj->globalPosition, &curve->pos);
            if (dist < bestDist) {
                bestDist = dist;
                out = curve;
            }
        }
    }
    return out;
}
