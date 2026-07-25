#include "sys/intersect.h"

#include "dlls/objects/210_player.h"
#include "sys/camera.h"
#include "sys/memory.h"
#include "sys/newshadows.h"
#include "sys/objtype.h"
#include "dll.h"
#include "macros.h"

/** @file official filename: track/intersect.c */

typedef struct {
    s16 unk0;
    u8 _unk2[14];
} D_800BB26C_Struct;

typedef struct {
    Object *unk0;
    Object *unk4;
    Vec3f unk8;
    u8 unk14;
    u8 unk15;
} Unk8005B17C;

s32 func_80055458(Object* arg0, UnkFunc80051D68Arg3* arg1, UnkFunc80051D68Arg3* arg2, f32* arg3, f32* arg4, s32 arg5, TrackIntersectResult* arg6, u8 checkingObj);
s32 func_800564C8(UnkFunc80051D68Arg3 *, UnkFunc80051D68Arg3 *, Vec3f *, s32, TrackIntersectResult*, u8);
void func_80058F8C(void);
s32 func_8005A3F8(Vec3f* arg0, Vec3f* arg1, f32 arg2, s32 arg3, TrackLineIntersectResult* arg4, Object* arg5, s8 arg6, s8 arg7, s8 arg8, Object* arg9);
Unk8005B17C* func_8005B17C(Object*, Object*, u8);
Unk8005B17C* func_8005B204(Object*, Object*, u8);
s32 func_80057A30(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, Vec3f* arg4, Vec4f* arg5, f32 arg6, u8 arg7);
s32 func_800573D8(f32 arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, Vec4f* arg4, f32 arg5, u8 arg6);
s32 func_80056E50(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, Vec4f* arg4, f32 arg5, u8 arg6);
s32 func_800567F4(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, f32 arg3, Vec3f* arg4, Vec4f* arg5);
s32 func_8005A2BC(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s16* arg4);
UnkFunc80051D68Arg3* func_80053B24(UnkFunc80051D68Arg3*, s32, s32, s32, s32, s32, s32, u8);
UnkFunc80051D68Arg3* func_8005471C(UnkFunc80051D68Arg3*, Unk8005341C*, ModelInstance*, f32, f32, f32, f32, f32, f32, u8);
void func_80054DF8(UnkFunc80051D68Arg3* pl, UnkFunc80051D68Arg3* plEnd, u8 flags);
void func_80058144(UnkFunc80051D68Arg3* plStart, UnkFunc80051D68Arg3* plEnd, Unk8005341C* plIdx, f32 x, f32 z, s32 arg5);
void func_80058D54(Vec4f* vX, Vec4f* vY, Vec4f* vZ, s32 p0, s32 p1, s32 p2, f32* outNx, f32* outNy, f32* outNz, f32* arg9);
s32 func_8005B274(f32* arg0, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s8 arg5);

// .bss 800bb200-800bb540
Vec3s32 gBgBlockList[8];
UnkFunc80051D68Arg3* D_800BB260;
s16 gPLlistCount;
Unk8005341C gPLindex[20];
u8 gPLindexCount; // count for gPLindex
u8 _bss_800BB3AC[0x800BB3B0-0x800BB3AC];
TrackHeightResult D_800BB3B0[10];
TrackHeightResult* D_800BB4A0;
u32 _bss_800BB4A4;
TrackHeightResult* D_800BB4A8;
u8 _bss_800BB4B0[0x800BB4D0-0x800BB4B0]; // 0x24
TrackHeightResult** D_800BB4D0;
s8 D_800BB4D4;
u8 _bss_800BB4D5;
s16 gLineListCount;
s16 gPointListCount;
// nothing here
u16 D_800BB4E0[38];
f32 D_800BB52C;
f32 D_800BB530;
f32 D_800BB534;
s8 D_800BB538;
s8 D_800BB539;
u8 D_800BB53A;

// .data 80092E70-80092E90
UnkFunc80051D68Arg3 *gPLlist = NULL; // 250 length
ModLineReencoded* gLineList = NULL; // 400 length
Vec3f *gPointList = NULL; // 400 length
s16 *gLineIndex = NULL;
s8 D_80092E80 = 0;
Unk8005B17C* gLastLines = NULL;

#define SOME_MIN 100000
// Idk where this random 34 is coming from, hex value is 0xFFFE7960
#define SOME_MAX (2 ^ 32) - SOME_MIN - 34

void trackIntersectInit(void) {
    if (gPLlist == NULL) {
        gPLlist = mmAlloc(250*sizeof(UnkFunc80051D68Arg3), COLOUR_TAG_YELLOW, ALLOC_NAME("int:pllist"));
        gLineList = mmAlloc(400*sizeof(ModLineReencoded), COLOUR_TAG_YELLOW, ALLOC_NAME("int:linelist"));
        gPointList = mmAlloc(4800, COLOUR_TAG_YELLOW, ALLOC_NAME("int:pointlist"));
        gLineIndex = mmAlloc(800, COLOUR_TAG_YELLOW, ALLOC_NAME("int:lineindex"));
        gLastLines = mmAlloc(20*sizeof(Unk8005B17C), COLOUR_TAG_YELLOW, ALLOC_NAME("int:lastlines"));
    }
    func_80058F8C();
    gLineListCount = 0;
    gPointListCount = 0;
    D_800BB539 = 0;
    D_800BB538 = 0;
}

void trackIntersect_func_800533D8(s32* arg0, UnkFunc80051D68Arg3** arg1) {
    *arg0 = gPLindex[gPLindexCount].unk4;
    *arg1 = gPLlist;
}

void trackIntersectGetBlockList(Vec3s32** blockList) {
    *blockList = gBgBlockList;
}

Unk8005341C* trackIntersectGetPLIndices(s32* count) {
    *count = gPLindexCount;
    return gPLindex;
}

void trackIntersectBuildAABB(AABBs32 *aabb, Vec3f *posArray1, Vec3f *posArray2, f32 *cubeRadiusArray, s32 arrayLength) {
    aabb->min.x = 1000000;
    aabb->max.x = -1000000;
    aabb->min.y = 1000000;
    aabb->max.y = -1000000;
    aabb->min.z = 1000000;
    aabb->max.z = -1000000;

    while (arrayLength--) {
        f32 var;

        // vec1
        var = posArray1->x - *cubeRadiusArray;
        if (var < aabb->min.x) {
            aabb->min.x = var;
        }

        var = posArray1->x + *cubeRadiusArray;
        if (var > aabb->max.x) {
            aabb->max.x = var;
        }

        var = posArray1->y - *cubeRadiusArray;
        if (var < aabb->min.y) {
            aabb->min.y = var;
        }

        var = posArray1->y + *cubeRadiusArray;
        if (var > aabb->max.y) {
            aabb->max.y = var;
        }

        var = posArray1->z - *cubeRadiusArray;
        if (var < aabb->min.z) {
            aabb->min.z = var;
        }

        var = posArray1->z + *cubeRadiusArray;
        if (var > aabb->max.z) {
            aabb->max.z = var;
        }

        // vec2
        var = posArray2->x - *cubeRadiusArray;
        if (var < aabb->min.x) {
            aabb->min.x = var;
        }

        var = posArray2->x + *cubeRadiusArray;
        if (var > aabb->max.x) {
            aabb->max.x = var;
        }

        var = posArray2->y - *cubeRadiusArray;
        if (var < aabb->min.y) {
            aabb->min.y = var;
        }

        var = posArray2->y + *cubeRadiusArray;
        if (var > aabb->max.y) {
            aabb->max.y = var;
        }

        var = posArray2->z - *cubeRadiusArray;
        if (var < aabb->min.z) {
            aabb->min.z = var;
        }

        var = posArray2->z + *cubeRadiusArray;
        if (var > aabb->max.z) {
            aabb->max.z = var;
        }

        posArray1++;
        posArray2++;
        
        cubeRadiusArray++;
    }
}

/** 
 * Do broadphase intersect checks.
 *
 * Determines which block shape and mobile map model vertices are within the given AABB. 
 */
void trackIntersectBroadphase(Object* obj, AABBs32* aabb, u8 flags) {
    UnkFunc80051D68Arg3* s2;
    Model* temp_a0_2;
    ModelInstance* temp_a2;
    Object* curObj;
    Object** objects;
    ObjectHitInfo* temp_a0;
    s32 _pad;
    Unk8005341C* var_s0;
    f32 temp_fa0;
    f32 temp_fv1;
    f32 temp_fs0;
    UnkFunc80051D68Arg3* spB8;
    f32 temp_fv1_2;
    s32 objCount;
    f32 temp_fs1;
    f32 temp_fs2;
    f32 temp_fs3;
    f32 temp_fs4;
    f32 temp_fs5;
    f32 temp_fv0;
    f32 temp_fv1_3;
    s16 i;

    temp_fs0 = aabb->min.x - 5;
    temp_fs1 = aabb->max.x + 5;
    temp_fs2 = aabb->min.y - 5;
    temp_fs3 = aabb->max.y + 5;
    temp_fs4 = aabb->min.z - 5;
    temp_fs5 = aabb->max.z + 5;
    var_s0 = &gPLindex[1]; // mobile maps start at index 1, index 0 is for blocks
    gPLindex->unk0 = 0;
    gPLindex->unk4 = 0;
    D_800BB260 = gPLlist + 250;
    // Build PLlist
    if (!(flags & 0x10)) {
        s2 = func_80053B24(gPLlist, temp_fs0, temp_fs2, temp_fs4, temp_fs1, temp_fs3, temp_fs5, flags);
    } else {
        s2 = gPLlist;
    }
    // Check mobile maps(?)
    if ((flags & 1) && obj != NULL) {
        spB8 = s2;
        objects = func_80025DD4(&objCount);
        for (i = 0; i < objCount; i++) {
            curObj = objects[i];
            if (curObj->objhitInfo != NULL && curObj->polyhits != NULL && curObj->polyhits->unk10D == 0 && curObj->polyhits->unk10E == 0) {
                temp_a2 = curObj->modelInsts[curObj->objhitInfo->unkA0];
                if (temp_a2 != NULL && temp_a2->model->facebatchBounds != NULL) {
                    temp_fv0 = temp_a2->model->maxAnimatedVertDistance;
                    if (
                        !((temp_fv1 = curObj->srt.transl.x - temp_fv0) > temp_fs1) && !((temp_fv1 = curObj->srt.transl.x + temp_fv0) < temp_fs0)
                        && !((temp_fv1_2 = curObj->srt.transl.y - temp_fv0) > temp_fs3) && !((temp_fv1_2 = curObj->srt.transl.y + temp_fv0) < temp_fs2)
                        && !((temp_fv1_3 = curObj->srt.transl.z - temp_fv0) > temp_fs5) && !((temp_fv1_3 = curObj->srt.transl.z + temp_fv0) < temp_fs4)
                    ) {
                            var_s0->unkC = (MtxF* ) &((f32*)curObj->polyhits)[(curObj->polyhits->unk10C + 2) << 4];
                            var_s0->unk8 = (MtxF* ) &((f32*)curObj->polyhits)[curObj->polyhits->unk10C << 4];
                            var_s0->unk4 = ((s32)s2 - (s32)gPLlist) / 52;
                            var_s0->unk0 = curObj;
                            /* default.dol
                            if (var_s0 >= &gPLindex[ARRAYCOUNT(gPLindex)]) {
                                STUBBED_PRINTF("PLindex overflow!!\n");
                            }
                            */
                            s2 = func_8005471C(s2, var_s0, temp_a2, temp_fs0, temp_fs2, temp_fs4, temp_fs1, temp_fs3, temp_fs5, flags);
                            var_s0 += 1;
                    }
                }
            }
        }
        func_80054DF8(spB8, s2, flags);
    }
    gPLlistCount = ((s32)s2 - (s32)gPLlist) / 52;
    /* default.dol
    if (gPLlistCount >= 250) { // length of gPLlist alloc, corrected for dp
        STUBBED_PRINTF("PLlist overflow!! - %d\n", gPLlist);
    }
    */
    gPLindexCount = var_s0 - gPLindex;
    /* default.dol
    if (gPLindexCount >= ARRAYCOUNT(gPLindex)) {
        STUBBED_PRINTF("PLindex overflow!! - %d\n", gPLindexCount);
    }
    */
    var_s0->unk4 = gPLlistCount;
}

/** Finds block triangles within the given bounds. */
#ifndef NON_MATCHING
static const char str_8009aa70[] = "Sorry Background Block list has been exceeded\n";
UnkFunc80051D68Arg3* func_80053B24(UnkFunc80051D68Arg3* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7);
#pragma GLOBAL_ASM("asm/nonmatchings/intersect/func_80053B24.s")
#else
// N64: https://decomp.me/scratch/SMq1i
// default.dol: https://decomp.me/scratch/pyduQ


UnkFunc80051D68Arg3* func_80053B24(UnkFunc80051D68Arg3* arg0, s32 arg1, s32 upperY, s32 arg3, s32 arg4, s32 lowerY, s32 arg6, u8 arg7) {
    Block* temp_s0;
    Block* temp_v0;
    Block *blocks[8];
    f32 temp_fs1;
    Vtx_t* sp128[3];
    Vtx_t *sp124;
    s16 sp122;
    s32 sp11C;
    s32 sp118;
    f32 temp_fs0;
    f32 temp_fs2;
    s32 sp10C;
    s32 numBlocks;
    s32 temp_s4; // sp104
    s32 temp_s5; // sp100
    s32 temp_s7; // spFC
    s32 temp_s3; // spF8
    s32 lowerZ; // pad
    s32 upperZ; // pad
    BlockShape* temp_t9;
    BlockShape* var_s3;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 var_a0_3;
    s32 var_a1_3;
    s32 maxZ;
    s32 minX;
    s32 maxX;
    s32 minY;
    s32 maxY;
    s32 minZ;
    s16 var_v0;
    s32 var_v1_2;
    s32 zDiff;
    s32 xDiff;
    s32 temp_t9_4;
    s32 var_t0;
    u8 var_v0_3;
    u8 spA6;
    u8 lowestYIndex;
    u8 highestYIndex;
    s32 temp;
    BlockShape* sp9C;

    arg1 -= D_80092A60;
    arg3 -= D_80092A64;
    arg4 -= D_80092A60;
    arg6 -= D_80092A64;
    if (arg4 < arg1) {
        arg1 ^= arg4;
        arg4 ^= arg1;
        arg1 ^= arg4;
    }
    if (arg6 < arg3) {
        arg3 ^= arg6;
        arg6 ^= arg3;
        arg3 ^= arg6;
    }
    temp_s4 = floorf(arg1 / BLOCKS_GRID_UNIT_F);
    temp_s7 = floorf(arg3 / BLOCKS_GRID_UNIT_F);
    temp_s5 = floorf(arg4 / BLOCKS_GRID_UNIT_F);
    temp_s3 = floorf(arg6 / BLOCKS_GRID_UNIT_F);
    numBlocks = 0;
    for (sp118 = 0; sp118 < 5; sp118++) {
        if ((temp_v0 = mapGetBlockFromGrid(temp_s4, temp_s7, sp118)) != NULL) {
            blocks[numBlocks] = temp_v0;
            gBgBlockList[numBlocks].x = temp_s4 * BLOCKS_GRID_UNIT;
            gBgBlockList[numBlocks].z = temp_s7 * BLOCKS_GRID_UNIT;
            numBlocks++;
        }
        if (temp_s5 != temp_s4) {
            if ((temp_v0 = mapGetBlockFromGrid(temp_s5, temp_s7, sp118)) != NULL) {
                blocks[numBlocks] = temp_v0;
                gBgBlockList[numBlocks].x = temp_s5 * BLOCKS_GRID_UNIT;
                gBgBlockList[numBlocks].z = temp_s7 * BLOCKS_GRID_UNIT;
                numBlocks++;
            }
        }
        if (temp_s3 != temp_s7) {
            if ((temp_v0 = mapGetBlockFromGrid(temp_s4, temp_s3, sp118)) != NULL) {
                blocks[numBlocks] = temp_v0;
                gBgBlockList[numBlocks].x = temp_s4 * BLOCKS_GRID_UNIT;
                gBgBlockList[numBlocks].z = temp_s3 * BLOCKS_GRID_UNIT;
                numBlocks++;
            }
            if (temp_s5 != temp_s4) {
                if ((temp_v0 = mapGetBlockFromGrid(temp_s5, temp_s3, sp118)) != NULL) {
                    blocks[numBlocks] = temp_v0;
                    gBgBlockList[numBlocks].x = temp_s5 * BLOCKS_GRID_UNIT;
                    gBgBlockList[numBlocks].z = temp_s3 * BLOCKS_GRID_UNIT;
                    numBlocks++;
                }
            }
        }
    }
    if (numBlocks > 8) {
        STUBBED_PRINTF("Sorry Background Block list has been exceeded\n");
    }
    for (sp118 = 0; sp118 < numBlocks; sp118++) {
        temp_s4 = arg1 - gBgBlockList[sp118].x;
        temp_s5 = arg4 - gBgBlockList[sp118].x;
        temp_s7 = arg3 - gBgBlockList[sp118].z;
        temp_s3 = arg6 - gBgBlockList[sp118].z;
        gBgBlockList[sp118].x += D_80092A60;
        gBgBlockList[sp118].z += D_80092A64;
        if (temp_s4 < 0) {
            temp_s4 = 0;
        }
        if (temp_s5 > BLOCKS_GRID_UNIT) {
            temp_s5 = BLOCKS_GRID_UNIT;
        }
        if (temp_s7 < 0) {
            temp_s7 = 0;
        }
        if (temp_s3 > BLOCKS_GRID_UNIT) {
            temp_s3 = BLOCKS_GRID_UNIT;
        }
        xDiff = gBgBlockList[sp118].x - gBgBlockList->x;
        zDiff = gBgBlockList[sp118].z - gBgBlockList->z;
        temp_s0 = blocks[sp118];
        var_v0 = 1;
        sp122 = 0;
        for (var_t0 = 0; var_t0 < BLOCKS_GRID_UNIT; var_t0 += 80) {
            if ((var_t0 + 80) >= temp_s4 && temp_s5 >= var_t0) {
                sp122 |= var_v0;
            }
            var_v0 <<= 1;
        }
        for (var_t0 = 0; var_t0 < BLOCKS_GRID_UNIT; var_t0 += 80) {
            if ((var_t0 + 80) >= temp_s7 && temp_s3 >= var_t0) {
                sp122 |= var_v0;
            }
            var_v0 <<= 1;
        }
        /* default.dol
        if (sp122 == 0 || !(sp122 & 0xFF00)) { // condition from default.dol, may be wrong
            STUBBED_PRINTF("Mask Error - %d (%d,%d)-(%d,%d)\n\n",
                sp118, temp_s4, temp_s7, temp_s5, temp_s3);
        }
        */
        sp9C = &temp_s0->shapes[temp_s0->shapeCount];
        for (var_s3 = temp_s0->shapes; var_s3 < sp9C; var_s3++) {
            if (var_s3->flags & 0x2000) {
                if ((var_s3->flags & 0x300000)) {
                    continue;
                }
                spA6 = 4;
                if (!(arg7 & 0x20)) {
                    spA6 |= 0x10;
                }
            } else if (!(var_s3->flags & 0x800) || (arg7 & 0x20)) {
                spA6 = 2;
            } else  {
                continue;
            }

            if (
                lowerY >= var_s3->Ymin &&
                var_s3->Ymax >= upperY &&
                (temp_s5 >> 2) >= var_s3->Xmin &&
                var_s3->Xmax >= (temp_s4 >> 2) &&
                (temp_s3 >> 2) >= var_s3->Zmin &&
                var_s3->Zmax >= (temp_s7 >> 2)
            ) {
                if (var_s3->flags & 0x1000) {
                    spA6 |= 8;
                }
                sp124 = &temp_s0->vertices2[temp_s0->vtxFlags & 1][var_s3->vtxBase];
                sp11C = var_s3->triBase;
                sp10C = var_s3[1].triBase;
                for (;sp11C < sp10C; sp11C++) {
                    var_t0 = sp122 & temp_s0->xzBitmap[sp11C];
                    if ((var_t0 & 0xFF) && (var_t0 & 0xFF00)) {
                        minX = SOME_MIN;
                        maxX = SOME_MAX;
                        minZ = minY = minX;
                        maxZ = maxY = maxX;
                        sp128[0] = &sp124[(temp_s0->encodedTris[sp11C].d0 >> 13) & 0x1F];
                        sp128[1] = &sp124[(temp_s0->encodedTris[sp11C].d0 >> 7) & 0x1F];
                        sp128[2] = &sp124[(temp_s0->encodedTris[sp11C].d0 >> 1) & 0x1F];
                        // how to slti here?
                        for (var_t0 = 0; var_t0 != 3; var_t0++) {
                            var_a0_3 = sp128[var_t0]->ob[0];
                            var_v1_2 = sp128[var_t0]->ob[1];
                            var_a1_3 = sp128[var_t0]->ob[2];
                            if (var_s3->flags & 0x20000000) {
                                var_a0_3 *= 1.0f; // not necessary, forces a float cast tho
                                var_v1_2 *= 0.05f;
                                var_v1_2 += temp_s0->minY;
                                var_a1_3 *= 1.0f; // not necessary, forces a float cast tho
                            }
                            if (var_a0_3 > maxX) {
                                maxX = var_a0_3;
                            }
                            if (var_a0_3 < minX) {
                                minX = var_a0_3;
                            }
                            if (var_v1_2 > maxY) {
                                maxY = var_v1_2;
                                highestYIndex = var_t0;
                            }
                            if (var_v1_2 < minY) {
                                minY = var_v1_2;
                                lowestYIndex = var_t0;
                            }
                            if (var_a1_3 > maxZ) {
                                maxZ = var_a1_3;
                            }
                            if (var_a1_3 < minZ) {
                                minZ = var_a1_3;
                            }
                            arg0->vX[var_t0] = var_a0_3 + xDiff;
                            arg0->vY[var_t0] = var_v1_2;
                            arg0->vZ[var_t0] = var_a1_3 + zDiff;
                        }
                        if (
                            lowerY >= minY &&
                            maxY >= upperY &&
                            temp_s5 >= minX &&
                            maxX >= temp_s4 &&
                            temp_s3 >= minZ &&
                            maxZ >= temp_s7
                        ) {
                            if (var_s3->animatorID != 0) {
                                temp_fs0 = (arg0->vY[0] * (arg0->vZ[1] - arg0->vZ[2])) + (arg0->vY[1] * (arg0->vZ[2] - arg0->vZ[0])) + (arg0->vY[2] * (arg0->vZ[0] - arg0->vZ[1]));
                                temp_fs1 = (arg0->vZ[0] * (arg0->vX[1] - arg0->vX[2])) + (arg0->vZ[1] * (arg0->vX[2] - arg0->vX[0])) + (arg0->vZ[2] * (arg0->vX[0] - arg0->vX[1]));
                                temp_fs2 = (arg0->vX[0] * (arg0->vY[1] - arg0->vY[2])) + (arg0->vX[1] * (arg0->vY[2] - arg0->vY[0])) + (arg0->vX[2] * (arg0->vY[0] - arg0->vY[1]));
                                temp_fv0 = sqrtf(SQ(temp_fs0) + SQ(temp_fs1) + SQ(temp_fs2));
                                if (temp_fv0 > 0.0f) {
                                    temp_fv1 = 8191.0f / temp_fv0;
                                    arg0->nX = temp_fs0 * temp_fv1;
                                    arg0->nY = temp_fs1 * temp_fv1;
                                    arg0->nZ = temp_fs2 * temp_fv1;
                                }
                            } else {
                                arg0->nX = temp_s0->encodedTris[sp11C].d0 >> 0x12;
                                arg0->nY = (temp_s0->encodedTris[sp11C].d1 << 0xE) >> 0x12;
                                arg0->nZ = temp_s0->encodedTris[sp11C].d1 >> 0x12;
                            }
                            if ((!(arg7 & 8) || !(arg0->nY >= 5791.037f)) && ((arg7 & 4) == 0 || !(arg0->nY < 5791.037f))) {
                                arg0->unk0 = -((arg0->nY * arg0->vY[0]) + ((arg0->nX * arg0->vX[0]) + (arg0->vZ[0] * arg0->nZ))) * (1.0f / 8191.0f);
                                temp = (sp11C) * 9;
                                for (temp_t9_4 = temp; temp_t9_4 < (temp + 9); temp_t9_4++) {
                                    arg0->unk1C[temp_t9_4 - temp] = temp_s0->ptr_faceEdgeVectors[temp_t9_4];
                                }
                                if (var_s3->flags & 0x2000) {
                                    var_v0_3 = 0xE;
                                } else {
                                    if (var_s3->materialIndex == 0xFF) {
                                        var_v0_3 = 0;
                                    } else {
                                        var_v0_3 = temp_s0->materials[var_s3->materialIndex].terrain_type;
                                    }
                                }
                                arg0->unk2E = var_v0_3;
                                arg0->unk30 = (highestYIndex << 4) | lowestYIndex;
                                arg0->unk2F = (temp_s0->encodedTris[sp11C].d1 & 1) | spA6;
                                arg0++;
                                /* default.dol
                                if (arg0 >= ?) {
                                    STUBBED_PRINTF("PLlist overflow!!\n");
                                    return ?;
                                }
                                */
                            }
                        }
                    }
                }
            }
        }
    }
    return arg0;
}


#endif

UnkFunc80051D68Arg3* func_8005471C(UnkFunc80051D68Arg3* arg0, Unk8005341C* arg1, ModelInstance* arg2, f32 upperX, f32 upperY, f32 upperZ, f32 lowerX, f32 lowerY, f32 lowerZ, u8 arg9) {
    s32 var_s1;
    s16 spDC[3];
    u32* var_s5;
    Vtx* currVtx;
    s32 minY;
    s32 minX;
    s32 maxX;
    s32 maxY;
    s32 minZ;
    s32 maxZ;
    s32 var_s0;
    s32 spB4;
    s32 spB0;
    s32 i;
    u8 lowestYIndex; // s4
    u8 highestYIndex; // s3
    s16 pad;
    f32 xBounds[4]; // sp98
    f32 zBounds[4]; // sp88
    f32 sp84;
    FacebatchBound *bound;
    Model* sp7C;

    sp7C = arg2->model;

    mathMtxXFMF(arg1->unk8, upperX, upperY, upperZ, &xBounds[0], &sp84, &zBounds[0]);
    mathMtxXFMF(arg1->unk8, upperX, upperY, lowerZ, &xBounds[1], &upperY, &zBounds[1]);
    mathMtxXFMF(arg1->unk8, lowerX, lowerY, upperZ, &xBounds[2], &sp84, &zBounds[2]);
    mathMtxXFMF(arg1->unk8, lowerX, lowerY, lowerZ, &xBounds[3], &lowerY, &zBounds[3]);

    upperX = lowerX = xBounds[0];
    upperZ = lowerZ = zBounds[0];
    for (i = 1; i < 4; i++) {
        if (xBounds[i] < upperX) { upperX = xBounds[i]; }
        if (lowerX < xBounds[i]) { lowerX = xBounds[i]; }
        if (zBounds[i] < upperZ) { upperZ = zBounds[i]; }
        if (lowerZ < zBounds[i]) { lowerZ = zBounds[i]; }
    }

    spB0 = sp7C->unk70;
    for (spB4 = 0; spB4 < spB0; spB4++) {
        if (
            (sp7C->faces[spB4].tagC & 0x100000) == 0 &&
            (!(sp7C->faces[spB4].tagC & 0x800) || (arg9 & 0x20))
       ) {
            bound = (FacebatchBound *)((u8*)sp7C->facebatchBounds + ((spB4 * 3) << 2));
            if (
                !(bound->minX < upperX) &&
                !(lowerX < bound->maxX) &&
                !(bound->minY < upperY) &&
                !(lowerY < bound->maxY) &&
                !(bound->minZ < upperZ) &&
                !(lowerZ < bound->maxZ)
           ) {
                var_s0 = (sp7C->faces[spB4].tagC & 0x7FF);
                var_s0 *= 9;
                var_s1 = sp7C->faces[spB4].baseVertexID;
                pad = sp7C->faces[spB4].baseF3DCommandIndex;
                var_s5 = (u32*)&sp7C->displayList[pad];
                for (;; var_s0 += 9) {
                    minX = SOME_MIN;
                    maxX = SOME_MAX;
                    minY = SOME_MIN;
                    maxY = SOME_MAX;
                    minZ = SOME_MIN;
                    maxZ = SOME_MAX;
                    if ((var_s5[0] >> 24) != 6 && (var_s5[0] >> 24) != 5) {
                        break;
                    }
                    spDC[0] = (var_s5[0] >> 17) & 0x1F;
                    spDC[1] = (var_s5[0] >> 9) & 0x1F;
                    spDC[2] = (var_s5[0] >> 1) & 0x1F;
                    var_s5 += 1;
                    for (i = 0; i < 3; i++) {
                        currVtx = &sp7C->vertices[spDC[i] + var_s1];
                        if (maxX < currVtx->v.ob[0]) {
                            maxX = currVtx->v.ob[0];
                        }
                        if (currVtx->v.ob[0] < minX) {
                            minX = currVtx->v.ob[0];
                        }
                        if (maxY < currVtx->v.ob[1]) {
                            maxY = currVtx->v.ob[1];
                            highestYIndex = i;
                        }
                        if (currVtx->v.ob[1] < minY) {
                            minY = currVtx->v.ob[1];
                            lowestYIndex = i;
                        }
                        if (maxZ < currVtx->v.ob[2]) {
                            if (1) maxZ = currVtx->v.ob[2];
                        }
                        if (currVtx->v.ob[2] < minZ) {
                            minZ = currVtx->v.ob[2];
                        }
                        arg0->vX[i] = currVtx->v.ob[0];
                        arg0->vY[i] = currVtx->v.ob[1];
                        arg0->vZ[i] = currVtx->v.ob[2];
                    }
                    if (!(lowerY < minY) && !(maxY < upperY) && !(lowerX < minX) && !(maxX < upperX) && !(lowerZ < minZ) && !(maxZ < upperZ)) {
                        if (sp7C->edgeVectors != NULL) {
                            for (i = var_s0; i < (var_s0 + 9); i++) {
                                arg0->unk1C[i-var_s0] = sp7C->edgeVectors[i];
                            }
                        } else {
                            for (i = var_s0; i < (var_s0 + 9); i++) {
                                arg0->unk1C[i-var_s0] = 0;
                            }
                            // @fake
                            do {
                                arg0->unk1C[1] = 0x7FFF;
                                arg0->unk1C[4] = 0x7FFF;
                                arg0->unk1C[7] = 0x7FFF;
                            } while (0);
                        }
                        if (sp7C->faces[spB4].materialID == 0xFF) {
                            arg0->unk2E = 0;
                        } else {
                            arg0->unk2E = sp7C->materials[sp7C->faces[spB4].materialID].unk7;
                        }
                        arg0->unk30 = (highestYIndex * 0x10) | lowestYIndex;
                        arg0->unk2F = 1;
                        arg0 += 1;
                    }
                }
            }
        }
    }
    return arg0;
}

void func_80054DF8(UnkFunc80051D68Arg3* pl, UnkFunc80051D68Arg3* plEnd, u8 flags) {
    f32 magnitude;
    f32 nx;
    f32 ny;
    f32 nz;
    s16* vX;
    s16* vY;
    s16* vZ;

    while (pl < plEnd) {
        // Calculate surface normal of triangle (alternate form of equation)
        // nx = (y0 * (z1 - z2)) + (y1 * (z2 - z0)) + (y2 * (z0 - z1))
        // ny = (z0 * (x1 - x2)) + (z1 * (x2 - x0)) + (z2 * (x0 - x1))
        // nz = (x0 * (y1 - y2)) + (x1 * (y2 - y0)) + (x2 * (y0 - y1))
        nx = (pl->vY[0] * (pl->vZ[1] - pl->vZ[2])) + (pl->vY[1] * (pl->vZ[2] - pl->vZ[0])) + (pl->vY[2] * (pl->vZ[0] - pl->vZ[1]));
        ny = (pl->vZ[0] * (pl->vX[1] - pl->vX[2])) + (pl->vZ[1] * (pl->vX[2] - pl->vX[0])) + (pl->vZ[2] * (pl->vX[0] - pl->vX[1]));
        nz = (pl->vX[0] * (pl->vY[1] - pl->vY[2])) + (pl->vX[1] * (pl->vY[2] - pl->vY[0])) + (pl->vX[2] * (pl->vY[0] - pl->vY[1]));
        vX = pl->vX;
        vY = pl->vY;
        vZ = pl->vZ;
        // Normalize
        magnitude = sqrtf(SQ(nx) + SQ(ny) + SQ(nz));
        if (magnitude > 0.0f) {
            nx *= (1.0f / magnitude);
            ny *= (1.0f / magnitude);
            nz *= (1.0f / magnitude);
        }
        if ((flags & 8) && (ny >= 0.707f)) {
            // Floor
            pl->unk2F |= 0x10;
        }
        if ((flags & 4) && (ny < 0.707f)) {
            // Ceiling
            pl->unk2F |= 0x10;
        }
        pl->nX = nx * 8191.0f;
        pl->nY = ny * 8191.0f;
        pl->nZ = nz * 8191.0f;
        // Negated dot product
        pl->unk0 = -((*vX * nx) + (*vY * ny) + (*vZ * nz));
        pl++;
    }
}

s32 trackGetIntersect(Object *obj, f32* prevPos, f32* currPos, s32 posCount, TrackIntersectResult* result, u8 flags) {
    Unk8005341C* var_s3;
    Unk8005341C* target;
    f32 spA0[4 * 3];
    f32 sp70[4 * 3];
    u8 sp6F;
    s16 var_s1;
    s16 var_s2;
    u8 temp_v0_3;
    u8 var_s5;
    Object* worldObj;

    sp6F = 0;
    result->unk68 = 0;
    var_s3 = gPLindex;
    target = &gPLindex[gPLindexCount];
    if ((u32) target >= (u32) ((u8*)&gPLindex + 1)) {
        do {
            worldObj = var_s3->unk0;
            if (worldObj != NULL) {
                // mobile map
                for (var_s1 = 0, var_s2 = 0; var_s2 < posCount;) {
                    mathMtxXFMF(
                        var_s3->unk8,
                        prevPos[var_s1 + 0], prevPos[var_s1 + 1], prevPos[var_s1 + 2],
                        &sp70[var_s1 + 0], &sp70[var_s1 + 1], &sp70[var_s1 + 2]
                    );
                    mathMtxXFMF(
                        var_s3->unk8,
                        currPos[var_s1 + 0], currPos[var_s1 + 1], currPos[var_s1 + 2],
                        &spA0[var_s1 + 0], &spA0[var_s1 + 1], &spA0[var_s1 + 2]
                    );
                    var_s1 += 3;\
                    var_s2++;
                }
                if (flags & 2) {
                    sp6F |= func_800564C8(&gPLlist[var_s3->unk4], &gPLlist[var_s3[1].unk4], (Vec3f*)&spA0[0], posCount, result, 1);
                } else {
                    temp_v0_3 = func_80055458(worldObj, &gPLlist[var_s3->unk4], &gPLlist[var_s3[1].unk4], &sp70[0], &spA0[0], posCount, result, 1);
                    if (temp_v0_3) {
                        func_80026184(worldObj, obj);
                        sp6F |= temp_v0_3;
                    }
                }
                for (var_s1 = 0, var_s2 = 0, var_s5 = 1; var_s2 < posCount; var_s2++) {
                    if (sp6F & var_s5) {
                        mathMtxXFMF(
                            var_s3->unkC,
                            spA0[var_s1 + 0], spA0[var_s1 + 1], spA0[var_s1 + 2],
                            &currPos[var_s1 + 0], &currPos[var_s1 + 1], &currPos[var_s1 + 2]
                        );
                    }
                    var_s5 <<= 1;
                    var_s1 += 3;
                }
            } else if (flags & 2) {
                sp6F |= func_800564C8(&gPLlist[var_s3->unk4], &gPLlist[var_s3[1].unk4], (Vec3f*)currPos, posCount, result, 0);
            } else {
                sp6F |= func_80055458(worldObj, &gPLlist[var_s3->unk4], &gPLlist[var_s3[1].unk4], prevPos, currPos, posCount, result, 0);
            }
            var_s3 += 1;
        } while ((u32) var_s3 < (u32) target);
    }
    return sp6F;
}

// official name: trackGetIntersect2 ?
s32 func_80055458(Object* arg0, UnkFunc80051D68Arg3* arg1, UnkFunc80051D68Arg3* arg2, f32* arg3, f32* arg4, s32 arg5, TrackIntersectResult* arg6, u8 checkingObj) {
    u8 sp1DF;
    u8 sp1DE;
    u8 var_a1;
    u8 var_s0;
    u8 var_s3;
    u8 sp1DA;
    s16 sp1D8;
    s16 sp1D6;
    s16 pad_sp1D4;
    s16 sp1D2;
    f32 temp_fa0;
    f32 temp_fs0;
    f32 temp_fs2;
    f32 var_fs4;
    f32 var_fv0;
    f32 var_fv1;
    f32 sp1B4;
    Vec3f sp1A8;
    Vec3f sp19C;
    Vec3f sp190;
    Vec3f sp184;
    Vec3f sp178;
    Vec4f sp168;
    Vec4f sp158;
    Vec4f sp148;
    Vec4f sp138;
    Vec4f sp128;
    f32 sp124;
    f32 sp120;
    s16 var_s4;
    s32 temp_v0_2;
    Vec3f sp10C;
    Vec3f sp100;
    Vec3f spF4;
    Vec3f spE8; // size 0x30?
    s32 pad_spE4;
    UnkFunc80051D68Arg3* var_s2;
    Vec3f *spDC;
    Vec3f spD0; // unused
    Vec3f spC4;
    Vec3f spB8; // size 0x30?
    u8 spB7;
    u8 spB6;
    u8 spB5;
    s8 spB4;

    if (checkingObj) {
        sp124 = 0.0f;
        sp120 = 0.0f;
    } else {
        sp124 = gBgBlockList->x;
        sp120 = gBgBlockList->z;
    }
    sp1D8 = 0;
    sp1D6 = 0;
    spB7 = 0;
    spB5 = 0;
    spB6 = 1;
    do {
        sp178.f[0] = arg4[sp1D8 + 0] - sp124;
        sp178.f[1] = arg4[sp1D8 + 1];
        sp178.f[2] = arg4[sp1D8 + 2] - sp120;
        sp1A8.f[0] = arg3[sp1D8 + 0] - sp124;
        sp1A8.f[1] = arg3[sp1D8 + 1];
        sp1A8.f[2] = arg3[sp1D8 + 2] - sp120;
        temp_fs2 = arg6->unk40[sp1D6];
        sp1DF = 0;
        var_fs4 = (temp_fs2 + 0.1f) * 32767.0f;
        if (var_fs4 < 32767.0f) {
            var_fs4 = 32767.0f;
        }
        sp10C.f[0] = temp_fs2;
        sp10C.f[1] = SQ(temp_fs2);
        arg6->unk0[sp1D6].f[0] = 0/*.0f*/;
        arg6->unk0[sp1D6].f[1] = 1.0f;
        arg6->unk0[sp1D6].f[2] = 0/*.0f*/;
        arg6->unk0[sp1D6].f[3] = 0/*.0f*/;
        arg6->unk58[sp1D6] = NULL;
        do {
            sp1DE = 0;
            sp19C.f[0] = sp178.f[0];
            sp19C.f[1] = sp178.f[1];
            sp19C.f[2] = sp178.f[2];
            var_s4 = 0;
            sp1DA = arg6->unk54[sp1D6] == 6;
            var_s2 = arg1;
            for (;(u32) var_s2 < (u32) arg2; var_s2++) {
                if (!(var_s2->unk2F & 0x10)) {
                    sp168.f[0] = var_s2->nX * (1.0f / 8191.0f);
                    sp168.f[1] = var_s2->nY * (1.0f / 8191.0f);
                    sp168.f[2] = var_s2->nZ * (1.0f / 8191.0f);
                    sp168.f[3] = var_s2->unk0;
                    /* default.dol
                    if (sp168.f[0] == 0.0f && sp168.f[1] == 0.0f && sp168.f[2] == 0.0f) {
                        if (? == 0) {
                            STUBBED_PRINTF("trackGetIntersect2: Bad ABC (not reporting any more)\n");
                        }
                        ? = 1;
                        break;
                    }
                    */
                    temp_fa0 = (sp168.f[3] + DOT_PRODUCT(sp168, sp19C)) - temp_fs2;
                    if (sp1DA || (temp_fa0 <= 0.0f)) {
                        var_fv0 = sp168.f[3] + DOT_PRODUCT(sp168, sp1A8);
                        temp_v0_2 = arg6->unk54[sp1D6];
                        if (temp_v0_2 != 4) {
                            var_fv0 -= temp_fs2;
                        }
                        if (((var_fv0 <= 0.0f) && (temp_fa0 >= 0.0f)) || ((var_fv0 >= 0.0f) && (temp_fa0 <= 0.0f))) {
                            VECTOR_SUBTRACT(sp19C, sp1A8, sp190);
                            if (temp_v0_2 == 4) {
                                var_fv0 -= temp_fs2;
                            }
                            if (var_fv0 != temp_fa0) {
                                var_fv1 = var_fv0 / (var_fv0 - temp_fa0);
                            } else {
                                var_fv1 = 0.0f;
                            }
                            sp184.f[0] = sp190.f[0] * var_fv1;
                            sp184.f[1] = sp190.f[1] * var_fv1;
                            sp184.f[2] = sp190.f[2] * var_fv1;
                            sp184.f[0] = sp184.f[0] + sp1A8.f[0];\
                            sp184.f[1] = sp184.f[1] + sp1A8.f[1];\
                            sp184.f[2] = sp184.f[2] + sp1A8.f[2];\
                            if (sp184.f[1] < (var_s2->vY[var_s2->unk30 & 0xF] - (temp_fs2 + 0.1f))) {
                                continue;
                            }
                            if ((var_s2->vY[var_s2->unk30 >> 4] + (temp_fs2 + 0.1f)) < sp184.f[1]) {
                                continue;
                            }
                            sp138.f[0] = var_s2->unk1C[0];
                            sp138.f[1] = var_s2->unk1C[1];
                            sp138.f[2] = var_s2->unk1C[2];
                            sp138.f[3] = ((sp184.f[2] * sp138.f[2]) + ((sp138.f[0] * sp184.f[0]) + (sp138.f[1] * sp184.f[1]))) - ((var_s2->vZ[0] * sp138.f[2]) + ((var_s2->vY[0] * sp138.f[1]) + (sp138.f[0] * var_s2->vX[0])));
                            if (var_fs4 < sp138.f[3]) {
                                continue;
                            }
                            sp148.f[0] = var_s2->unk1C[3];
                            sp148.f[1] = var_s2->unk1C[4];
                            sp148.f[2] = var_s2->unk1C[5];
                            sp148.f[3] = ((sp184.f[2] * sp148.f[2]) + ((sp148.f[0] * sp184.f[0]) + (sp148.f[1] * sp184.f[1]))) - ((var_s2->vZ[1] * sp148.f[2]) + ((var_s2->vY[1] * sp148.f[1]) + (sp148.f[0] * var_s2->vX[1])));
                            if (var_fs4 < sp148.f[3]) {
                                continue;
                            }
                            sp158.f[0] = var_s2->unk1C[6];
                            sp158.f[1] = var_s2->unk1C[7];
                            sp158.f[2] = var_s2->unk1C[8];
                            sp158.f[3] = ((sp184.f[2] * sp158.f[2]) + ((sp158.f[0] * sp184.f[0]) + (sp158.f[1] * sp184.f[1]))) - ((var_s2->vZ[2] * sp158.f[2]) + ((var_s2->vY[2] * sp158.f[1]) + (sp158.f[0] * var_s2->vX[2])));
                            if (var_fs4 < sp158.f[3]) {
                                continue;
                            }
                            var_s3 = 0;
                            if (temp_fs2 > 0.0f) {
                                if (sp138.f[3] > 0.0f) {
                                    var_s3 |= 1;
                                }
                                if (sp148.f[3] > 0.0f) {
                                    var_s3 |= 2;
                                }
                                if (sp158.f[3] > 0.0f) {
                                    var_s3 |= 4;
                                }
                            }
                            if (var_s3) {
                                temp_fs0 = vec3Normalize(&sp190);
                                for (var_s0 = 0; var_s0 < 3; var_s0++) {
                                    // @fake
                                    dummy_label_911687: ;
                                    if (var_s3 & (1 << var_s0)) {
                                        var_a1 = var_s0 + 1;
                                        if (var_a1 >= 3) {
                                            var_a1 = 0;
                                        }
                                        spE8.f[0] = var_s2->vX[var_s0];
                                        spE8.f[1] = var_s2->vY[var_s0];
                                        spE8.f[2] = var_s2->vZ[var_s0];
                                        spF4.f[0] = var_s2->vX[var_a1];
                                        spF4.f[1] = var_s2->vY[var_a1];
                                        spF4.f[2] = var_s2->vZ[var_a1];
                                        VECTOR_SUBTRACT(spF4, spE8, sp100);
                                        sp10C.f[2] = vec3Normalize(&sp100);
                                        var_s4 = func_800567F4(&spE8, &sp1A8, &sp190, temp_fs0, &sp184, &sp168);
                                    }
                                }
                            } else {
                                var_s4 = 4;
                            }
                        } else {
                            if ((-temp_fs2 <= var_fv0) && (temp_fs2 > 0.0f)) {
                                var_s3 = 0;
                                sp138.f[0] = var_s2->unk1C[0];
                                sp138.f[1] = var_s2->unk1C[1];
                                sp138.f[2] = var_s2->unk1C[2];
                                sp138.f[3] = ((sp1A8.f[2] * sp138.f[2]) + ((sp138.f[0] * sp1A8.f[0]) + (sp138.f[1] * sp1A8.f[1]))) - ((var_s2->vZ[0] * sp138.f[2]) + ((var_s2->vY[0] * sp138.f[1]) + (sp138.f[0] * var_s2->vX[0])));
                                sp148.f[0] = var_s2->unk1C[3];
                                sp148.f[1] = var_s2->unk1C[4];
                                sp148.f[2] = var_s2->unk1C[5];
                                sp148.f[3] = ((sp1A8.f[2] * sp148.f[2]) + ((sp148.f[0] * sp1A8.f[0]) + (sp148.f[1] * sp1A8.f[1]))) - ((var_s2->vZ[1] * sp148.f[2]) + ((var_s2->vY[1] * sp148.f[1]) + (sp148.f[0] * var_s2->vX[1])));
                                sp158.f[0] = var_s2->unk1C[6];
                                sp158.f[1] = var_s2->unk1C[7];
                                sp158.f[2] = var_s2->unk1C[8];
                                sp158.f[3] = ((sp1A8.f[2] * sp158.f[2]) + ((sp158.f[0] * sp1A8.f[0]) + (sp158.f[1] * sp1A8.f[1]))) - ((var_s2->vZ[2] * sp158.f[2]) + ((var_s2->vY[2] * sp158.f[1]) + (sp158.f[0] * var_s2->vX[2])));
                                if (sp138.f[3] > 0.0f) {
                                    var_s3 |= 1;
                                }
                                if (sp148.f[3] > 0.0f) {
                                    var_s3 |= 2;
                                }
                                if (sp158.f[3] > 0.0f) {
                                    var_s3 |= 4;
                                }
                                if (var_s3) {
                                    sp190.f[0] = sp19C.f[0] - sp1A8.f[0];
                                    sp190.f[1] = sp19C.f[1] - sp1A8.f[1];
                                    sp190.f[2] = sp19C.f[2] - sp1A8.f[2];
                                    temp_fs0 = vec3Normalize(&sp190);
                                    for (var_s0 = 0; var_s0 < 3; var_s0++) {
                                        if (var_s3 & (1 << var_s0)) {
                                            var_a1 = var_s0 + 1;
                                            if (var_a1 >= 3) {
                                                var_a1 = 0;
                                            }
                                            spE8.f[0] = var_s2->vX[var_s0];
                                            spE8.f[1] = var_s2->vY[var_s0];
                                            spE8.f[2] = var_s2->vZ[var_s0];
                                            spF4.f[0] = var_s2->vX[var_a1];
                                            spF4.f[1] = var_s2->vY[var_a1];
                                            spF4.f[2] = var_s2->vZ[var_a1];
                                            VECTOR_SUBTRACT(spF4, spE8, sp100);
                                            sp10C.f[2] = vec3Normalize(&sp100);
                                            var_s4 = func_800567F4(&spE8, &sp1A8, &sp190, temp_fs0, &sp184, &sp168);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (var_s4) {
                        sp19C.f[0] = sp184.f[0];
                        sp19C.f[1] = sp184.f[1];
                        sp19C.f[2] = sp184.f[2];
                        sp1DE = 1;
                        if (var_s4 == 4) {
                            sp1B4 = temp_fa0;
                        } else {
                            bcopy(&spE8, &spB8, 0x30);
                        }
                        sp128.f[0] = sp168.f[0];
                        sp128.f[1] = sp168.f[1];
                        sp128.f[2] = sp168.f[2];
                        sp128.f[3] = sp168.f[3];
                        sp1D2 = var_s4;
                        var_s4 = 0;
                        spB4 = var_s2->unk2E;
                    }
                }
            }
            if (sp1DE) {
                sp1DF++;
                if (sp1DF > 20) {
                    sp1DE = 0;
                    sp178.f[0] = sp1A8.f[0];
                    sp178.f[1] = sp1A8.f[1];
                    sp178.f[2] = sp1A8.f[2];
                } else {
                    if ((arg6->unk54[sp1D6] == 2U) && (sp128.f[1] >= 0.707f)) {
                        sp1DE = 0;
                    } else {
                        switch (sp1D2) {
                        case 1:
                            func_80057A30(&spB8, spDC, &sp1A8, &sp178, &sp19C, &sp128, temp_fs2 + 0.1f, arg6->unk54[sp1D6]);
                            break;
                        case 2:
                            func_80057A30(&spC4, spDC, &sp1A8, &sp178, &sp19C, &sp128, temp_fs2 + 0.1f, arg6->unk54[sp1D6]);
                            break;
                        case 3:
                            func_80056E50(&spB8, &sp1A8, &sp178, &sp19C, &sp128, temp_fs2 + 0.1f, arg6->unk54[sp1D6]);
                            break;
                        case 4:
                            func_800573D8(sp1B4, &sp1A8, &sp178, &sp19C, &sp128, temp_fs2 + 0.1f, arg6->unk54[sp1D6]);
                            break;
                        }
                        arg6->unk0[sp1D6].f[0] = sp128.f[0];
                        arg6->unk0[sp1D6].f[1] = sp128.f[1];
                        arg6->unk0[sp1D6].f[2] = sp128.f[2];
                        arg6->unk0[sp1D6].f[3] = sp128.f[3];
                        arg6->unk50[sp1D6] = spB4;
                        arg6->unk58[sp1D6] = arg0;
                    }
                }
            }
        } while (sp1DE && (arg6->unk54[sp1D6] != 3) && (arg6->unk54[sp1D6] != 6));
        if (sp1DF) {
            if (arg6->unk54[sp1D6] != 2) {
                if (sp128.f[1] >= 0.707f) {
                    spB5 |= spB6;
                }
                arg4[sp1D8 + 0] = sp178.f[0] + sp124;
                arg4[sp1D8 + 1] = sp178.f[1];
                arg4[sp1D8 + 2] = (f32) (sp178.f[2] + sp120);
            } else if (sp128.f[1] < 0.707f) {
                arg4[sp1D8 + 0] = sp178.f[0] + sp124;
                arg4[sp1D8 + 1] = sp178.f[1];
                arg4[sp1D8 + 2] = (f32) (sp178.f[2] + sp120);
            } else {
                spB5 |= spB6;
            }
            arg6->unk68 += 1;
            spB7 |= spB6;
        }
        spB6 *= 2;
        sp1D6++;
        sp1D8 += 3;
    } while (sp1D6 < arg5);
    return spB7 | (spB5 * 0x10);
}

s32 func_800564C8(UnkFunc80051D68Arg3* arg0, UnkFunc80051D68Arg3* arg1, Vec3f* arg2, s32 arg3, TrackIntersectResult* arg4, u8 arg5) {
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fs4;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv1;
    s32 var_v0;
    f32 var_fs1;
    f32 sp7C;
    f32 sp78;
    f32 var_fs2;
    s32 var_a0;
    s32 var_a1;
    s32 var_v1;
    f32 sp64;
    f32 sp60;
    s32 var_v1_2;
    s32 i;
    s32 temp2;
    UnkFunc80051D68Arg3 *temp;

    if (arg5 != 0) {
        sp64 = 0;
        sp60 = 0;
    } else {
        sp64 = gBgBlockList->x;
        sp60 = gBgBlockList->z;
    }
    i = 0;
    temp2 = 0;
    var_a0 = 0;
    var_a1 = 1;
    do {
        var_fs1 = arg2[temp2].f[0];
        temp_fs4 = arg2[temp2].f[1];
        var_fs2 = arg2[temp2].f[2];
        sp7C = arg4->unk40[i];
        sp78 = sp7C + 0.1f;
        var_fs1 -= sp64;
        var_fs2 -= sp60;
        var_v0 = 0;
        do {
            var_v1 = FALSE;
            temp = arg0;
            for (; (u32) temp < (u32) arg1; temp++) {
                temp_fa1 = temp->nX * 0.00012208521f;
                temp_fa0 = temp->nY * 0.00012208521f;
                temp_ft4 = temp->nZ * 0.00012208521f;
                temp_ft5 = temp->unk0;
                if (temp_fa0 >= 0.707f) {
                    continue;
                }

                temp_fv1 = ((temp_fa1 * var_fs1) + (temp_fa0 * temp_fs4) + (temp_ft4 * var_fs2) + temp_ft5) - sp7C;
                if ((2.0f * -sp7C) <= temp_fv1 && temp_fv1 <= 0.0f) {
                    if (temp_fs4 < (temp->vY[temp->unk30 & 0xF] - sp78)) {
                        continue;
                    }

                    if ((temp->vY[temp->unk30 >> 4] + sp78) < temp_fs4) {
                        continue;
                    }

                    var_v1_2 = 0;
                    // @fake to fix floats
                    do { } while (0);
                    var_fs1 -= temp_fv1 * temp_fa1;
                    var_fs2 -= temp_fv1 * temp_ft4;
                    while (var_v1_2 < 0x65 && ((var_fs1 * temp_fa1) + (temp_fa0 * temp_fs4) + (var_fs2 * temp_ft4) + temp_ft5) < sp78) {
                        var_v1_2++;
                        var_fs1 += temp_fa1 * 0.1f;
                        var_fs2 += temp_ft4 * 0.1f;
                    }
                    var_v0++;
                    var_v1 = TRUE;
                    if (var_v0 > 20) {
                        var_v1 = FALSE;
                    }
                    arg2[temp2].f[0] = var_fs1 + sp64;
                    arg2[temp2].f[2] = var_fs2 + sp60;
                    break;
                }
            }
        } while (var_v1 != FALSE);
        if (var_v0 != 0) {
            var_a0 |= var_a1;
            arg4->unk68++;
        }
        var_a1 <<= 1;
        i++;
        temp2++;
    } while (i < arg3);

    return var_a0;
}

s32 func_800567F4(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, f32 arg3, Vec3f* arg4, Vec4f* arg5) {
    Vec3f sp64;
    Vec3f sp58;
    Vec3f sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;

    vec3CrossProduct(arg2, &arg0[2], &sp64);
    sp44 = vec3Normalize(&sp64);
    if (sp44 == 0.0f) {
        return 0;
    }
    VECTOR_SUBTRACT((*arg1), (*arg0), sp58);
    sp48 = DOT_PRODUCT(sp64, sp58);
    sp48 = SQ(sp48);
    if (sp48 <= arg0[3].f[1]) {
        vec3CrossProduct(&sp58, &arg0[2], &sp4C);
        sp40 = -DOT_PRODUCT(sp4C, sp64) / sp44;
        vec3CrossProduct(&sp64, &arg0[2], &sp4C);
        vec3Normalize(&sp4C);
        sp48 = sqrtf(arg0[3].f[1] - sp48) / DOT_PRODUCT((*arg2), sp4C);
        if (sp48 < 0.0f) {
            sp48 = -sp48;
        }
        sp40 -= sp48;
        if ((sp40 >= 0.0f) && (sp40 <= arg3)) {
            arg4->x = arg2->x * sp40;
            arg4->y = arg2->y * sp40;
            arg4->z = arg2->z * sp40;
            arg4->x += arg1->x;
            arg4->y += arg1->y;
            arg4->z += arg1->z;
            sp40 = (DOT_PRODUCT((*arg4), arg0[2])) - DOT_PRODUCT(arg0[2], (*arg0));
            if ((sp40 >= 0.0f) && (sp40 <= arg0[3].f[2])) {
                sp4C.f[0] = arg0[2].f[0] * sp40;
                sp4C.f[1] = arg0[2].f[1] * sp40;
                sp4C.f[2] = arg0[2].f[2] * sp40;
                sp4C.f[0] += arg0->x;
                sp4C.f[1] += arg0->y;
                sp4C.f[2] += arg0->z;
                VECTOR_SUBTRACT(*arg4, sp4C, (*arg5));
                vec3Normalize((Vec3f*)arg5);
                arg5->f[3] = -DOT_PRODUCT((*arg4), (*arg5));
                return 3;
            }
            if (sp40 < 0.0f) {
                if (trackIntersect_func_80056BCC(arg1, arg2, arg4, arg5, arg0, arg0[3].f[1]) != 0) { return 1; }
            } else if (trackIntersect_func_80056BCC(arg1, arg2, arg4, arg5, &arg0[1], arg0[3].f[1]) != 0) {
                return 2;
            }
        }
    }
    return 0;
}

s32 trackIntersect_func_80056BCC(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec4f* arg3, Vec3f* arg4, f32 arg5) {
    f32 sp3C;
    f32 sp38;
    f32 temp_ft5;
    f32 sp1C;
    f32 sp18;
    Vec3f sp20;

    if (arg5 == 0.0f) {
        return 0;
    }

    VECTOR_SUBTRACT((*arg0), (*arg4), sp20);
    temp_ft5 = (SQ(sp20.f[0]) + SQ(sp20.f[1]) + SQ(sp20.f[2])) - arg5;
    if (temp_ft5 < 0.0f) {
        return 0;
    }
    sp38 = DOT_PRODUCT(sp20, (*arg1));
    sp3C = SQ(arg1->f[0]) + SQ(arg1->f[1]) + SQ(arg1->f[2]);
    sp1C = SQ(sp38);
    sp18 = 4.0f * sp3C * temp_ft5;
    if (sp18 < sp1C) {
        sp1C = (-sp38 - sqrtf(sp1C - sp18)) / (2.0f * sp3C);
        if ((sp1C >= 0.0f) && (sp1C <= 1.0f)) {
            arg2->f[0] = arg1->f[0] * sp1C;
            arg2->f[1] = arg1->f[1] * sp1C;
            arg2->f[2] = arg1->f[2] * sp1C;
            arg2->f[0] += arg0->f[0];
            arg2->f[1] += arg0->f[1];
            arg2->f[2] += arg0->f[2];
            VECTOR_SUBTRACT((*arg2), (*arg4), (*arg3));
            vec3Normalize((Vec3f* ) arg3);
            arg3->f[3] = -DOT_PRODUCT((*arg2), (*arg3));
            return 1;
        }
    }

    return 0;
}

s32 func_80056E50(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, Vec4f* arg4, f32 arg5, u8 arg6) {
    Vec3f sp64;
    Vec3f sp58;
    f32 sp54;
    f32 sp50;
    s16 i;

    if ((arg6 == 3) || (arg6 == 6)) {
        arg2->f[0] = arg3->f[0];
        arg2->f[1] = arg3->f[1];
        arg2->f[2] = arg3->f[2];
        VECTOR_SUBTRACT((*arg2), (*arg1), sp58);
        vec3Normalize(&sp58);
        i = 0;
        while ((arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4))) < arg5) {
            arg2->f[0] -= sp58.f[0] * 0.1f;
            arg2->f[1] -= sp58.f[1] * 0.1f;
            arg2->f[2] -= sp58.f[2] * 0.1f;
            i++;
            if (i >= 0x65) {
                break;
            } else { } // @fake
        }
        return 1;
    }
    sp54 = arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4));
    if (arg4->f[1] < 0.707f) {
        if (arg6 == 4) {
            arg2->f[0] = arg3->f[0];
            arg2->f[1] = arg3->f[1];
            arg2->f[2] = arg3->f[2];
            VECTOR_SUBTRACT((*arg2), (*arg1), sp58);
            sp50 = sqrtf(SQ(sp58.f[2]) + SQ(sp58.f[0]));
            if (sp50 > 0.0f) {
                sp58.f[0] /= sp50;
                sp58.f[2] /= sp50;
            } else {
                sp58.f[2] = 0.0f;
                sp58.f[0] = 1.0f;
            }
            i = 0;
            while ((arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4))) < arg5) {
                arg2->f[0] -= sp58.f[0] * 0.1f;
                arg2->f[2] -= sp58.f[2] * 0.1f;
                i++;
                if (i >= 0x65) {
                    STUBBED_PRINTF("1: track/intersect.c: OVERFLOW error\n");
                    break;   
                }
            }
        } else {
            sp54 += sp54; // used to be temp_ft5
            arg2->f[0] -= (sp54 * arg4->f[0]);
            arg2->f[1] -= (sp54 * arg4->f[1]);
            arg2->f[2] -= (sp54 * arg4->f[2]);
            sp50 = DOT_PRODUCT((*arg2), arg0[2]) - DOT_PRODUCT(arg0[2], (*arg0));
            sp64.f[0] = arg0[2].f[0] * sp50;
            sp64.f[1] = arg0[2].f[1] * sp50;
            sp64.f[2] = arg0[2].f[2] * sp50;
            sp64.f[0] += arg0->f[0];
            sp64.f[1] += arg0->f[1];
            sp64.f[2] += arg0->f[2];
            VECTOR_SUBTRACT((*arg2), sp64, sp58);
            vec3Normalize(&sp58);
            VECTOR_SCALE(sp58, arg5);
            VECTOR_ADD(sp64, sp58, (*arg2));
        }
    } else if (arg6 != 2) {
        arg2->f[1] = arg3->f[1];
        sp54 = arg5 - (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4)));
        if (sp54 > 0/*.0f*/) {
            sp54 = sp54 / mathSinfInterp(mathAtan2f(arg4->f[1], sqrtf(SQ(arg4->f[2]) + SQ(arg4->f[0]))));
            arg2->f[1] += sp54 * arg4->f[1];
        }
    }
    return 1;
}

s32 func_800573D8(f32 arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, Vec4f* arg4, f32 arg5, u8 arg6) {
    Vec3f sp3C;
    f32 temp_fv0;
    f32 sp34;
    s16 i;

    if ((arg6 == 3) || (arg6 == 6)) {
        arg2->f[0] = arg3->f[0];
        arg2->f[1] = arg3->f[1];
        arg2->f[2] = arg3->f[2];
        VECTOR_SUBTRACT((*arg2), (*arg1), sp3C);
        vec3Normalize(&sp3C);
        // @fake
        if (arg4) {}
        for (i = 0; i < 0x65 && (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4))) < arg5; i++) {
            arg2->f[0] -= sp3C.f[0] * 0.1f;
            arg2->f[1] -= sp3C.f[1] * 0.1f;
            arg2->f[2] -= sp3C.f[2] * 0.1f;
        }
        return 1;
    }

    if (arg4->f[1] < 0.707f) {
        switch (arg6) {
            case 4:
                arg2->f[0] = arg3->f[0];
                arg2->f[1] = arg3->f[1];
                arg2->f[2] = arg3->f[2];
                VECTOR_SUBTRACT((*arg2), (*arg1), sp3C);
                temp_fv0 = sqrtf(SQ(sp3C.x) + SQ(sp3C.z));
                if (temp_fv0 > 0.0f) {
                    sp3C.f[0] /= temp_fv0;
                    sp3C.f[2] /= temp_fv0;
                } else {
                    sp3C.f[2] = 0.0f;
                    sp3C.f[0] = 1.0f;
                }
                i = 0;
                while ((arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4))) < arg5) {
                    arg2->f[0] -= sp3C.f[0] * 0.1f;
                    arg2->f[2] -= sp3C.f[2] * 0.1f;
                    i++;
                    if (i >= 0x65) {
                        STUBBED_PRINTF("2: track/intersect.c: OVERFLOW error\n");
                        break;
                    }
                }
                break;
            case 1:
                arg2->f[0] -= arg0 * arg4->f[0];
                arg2->f[2] -= arg0 * arg4->f[2];
                sp34 = arg5 - (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4)));
                if (sp34 > 0.0f) {
                    i = mathAtan2f(arg4->f[1], sqrtf(SQ(arg4->x) + SQ(arg4->z)));
                    temp_fv0 = mathCosfInterp(i);
                    if (temp_fv0 > 0/*.0f*/) {
                        sp34 /= temp_fv0;
                    }
                    arg2->f[0] += sp34 * arg4->f[0];
                    arg2->f[2] += sp34 * arg4->f[2];
                }
                break;
            default:
                arg2->f[0] -= arg0 * arg4->f[0];
                arg2->f[1] -= arg0 * arg4->f[1];
                arg2->f[2] -= arg0 * arg4->f[2];
                sp34 = arg5 - (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4)));
                arg2->f[0] += sp34 * arg4->f[0];
                arg2->f[1] += sp34 * arg4->f[1];
                arg2->f[2] += sp34 * arg4->f[2];
                break;
        }
    } else if (arg6 != 2) {
        if (arg6 == 5) {
            arg2->f[0] -= arg0 * arg4->f[0];
            arg2->f[1] -= arg0 * arg4->f[1];
            arg2->f[2] -= arg0 * arg4->f[2];
            sp34 = arg5 - (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4)));
            arg2->f[0] += sp34 * arg4->f[0];
            arg2->f[1] += sp34 * arg4->f[1];
            arg2->f[2] += sp34 * arg4->f[2];
        } else {
            arg2->f[1] = arg3->f[1];
            sp34 = arg5 - (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4)));
            if (sp34 > 0.0f) {
                i = mathAtan2f(arg4->f[1], sqrtf(SQ(arg4->x) + SQ(arg4->z)));
                sp34 = sp34 / mathSinfInterp(i);
                arg2->f[1] += sp34 * arg4->f[1];
            }
        }
    }
    return 1;
}

s32 func_80057A30(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, Vec3f* arg4, Vec4f* arg5, f32 arg6, u8 arg7) {
    Vec3f sp3C;
    f32 sp38;
    s16 i;

    if ((arg7 == 3) || (arg7 == 6)) {
        arg3->f[0] = arg4->f[0];
        arg3->f[1] = arg4->f[1];
        arg3->f[2] = arg4->f[2];
        VECTOR_SUBTRACT((*arg3), (*arg2), sp3C);
        vec3Normalize(&sp3C);
        // @fake
        if (arg5) {}
        for (i = 0; i < 0x65 && (arg5->f[3] + DOT_PRODUCT((*arg3), (*arg5))) < arg6; i++) {
            arg3->f[0] -= sp3C.f[0] * 0.1f;
            arg3->f[1] -= sp3C.f[1] * 0.1f;
            arg3->f[2] -= sp3C.f[2] * 0.1f;
        }
        return 1;
    }

    sp38 = (arg5->f[3] + DOT_PRODUCT((*arg3), (*arg5)));
    if (arg5->f[1] < 0.707f) {
        if (arg7 == 4) {
            arg3->f[0] = arg4->f[0];
            arg3->f[1] = arg4->f[1];
            arg3->f[2] = arg4->f[2];
            VECTOR_SUBTRACT((*arg3), (*arg2), sp3C);
            sp38 = sqrtf(SQ(sp3C.x) + SQ(sp3C.z));
            if (sp38 > 0.0f) {
                sp3C.f[0] /= sp38;
                sp3C.f[2] /= sp38;
            } else {
                sp3C.f[2] = 0.0f;
                sp3C.f[0] = 1.0f;
            }
            i = 0;
            while ((arg5->f[3] + DOT_PRODUCT((*arg3), (*arg5))) < arg6) {
                arg3->f[0] -= sp3C.f[0] * 0.1f;
                arg3->f[2] -= sp3C.f[2] * 0.1f;
                i++;
                if (i >= 0x65) {
                    STUBBED_PRINTF("3: track/intersect.c: OVERFLOW error\n");
                    break;
                }
            }
        } else {
            sp38 += sp38;
            arg3->f[0] -= (sp38 * arg5->f[0]);
            arg3->f[1] -= (sp38 * arg5->f[1]);
            arg3->f[2] -= (sp38 * arg5->f[2]);
            VECTOR_SUBTRACT((*arg3), (*arg0), sp3C);
            vec3Normalize(&sp3C);
            VECTOR_SCALE(sp3C, arg6);
            VECTOR_ADD((*arg0), sp3C, (*arg3));
        }
    } else if (arg7 != 2) {
        arg3->f[1] = arg4->f[1];
        sp38 = arg6 - (arg5->f[3] + DOT_PRODUCT((*arg3), (*arg5)));
        if (sp38 > 0.0f) {
            i = mathAtan2f(arg5->f[1], sqrtf(SQ(arg5->x) + SQ(arg5->z)));
            sp38 = sp38 / mathSinfInterp(i);
            arg3->f[1] += sp38 * arg5->f[1];
        }
    }
    return 1;
}

// official name: trackGetHeight
s32 trackGetHeight(Object* obj, f32 x, f32 y, f32 z, TrackHeightResult*** result, s32 arg5, s32 flags) {
    AABBs32 aabb;
    f32 lx;
    f32 ly;
    f32 lz;
    Unk8005341C* plIdx;
    Unk8005341C* plIdxEnd;

    plIdx = gPLindex;
    if (arg5 >= 0) {
        aabb.min.x = x;
        aabb.max.x = x;
        aabb.min.y = y - 10000.0f;
        aabb.max.y = y + 10000.0f;
        aabb.min.z = z;
        aabb.max.z = z;
        trackIntersectBroadphase(obj, &aabb, flags);
    } else {
        arg5 = 0;
    }
    D_800BB4A0 = D_800BB3B0;
    D_800BB4D0 = &D_800BB4A8;
    D_800BB4D4 = 0;
    plIdxEnd = &gPLindex[gPLindexCount];
    while (plIdx < plIdxEnd) {
        if (plIdx->unk0 != NULL) {
            // mobile map
            mathMtxXFMF(plIdx->unk8, x, 0.0f, z, &lx, &ly, &lz);
            func_80058144(&gPLlist[plIdx->unk4], &gPLlist[plIdx[1].unk4], plIdx, lx, lz, arg5);
        } else {
            // block
            func_80058144(&gPLlist[plIdx->unk4], &gPLlist[plIdx[1].unk4], plIdx, x, z, arg5);
        }
        plIdx += 1;
    }
    *result = &D_800BB4A8;
    return D_800BB4D4;
}

// local helper for trackGetHeight
void func_80058144(UnkFunc80051D68Arg3* plStart, UnkFunc80051D68Arg3* plEnd, Unk8005341C* plIdx, f32 x, f32 z, s32 anyAngle) {
    s32 valid;
    u8 pad_sp118[0x134 - 0x118];
    Vec4f vx;
    u8 pad_spFC[0x108 - 0xFC];
    Vec4f vy;
    UnkFunc80051D68Arg3* pl;
    TrackHeightResult *temp_a0;
    TrackHeightResult* temp_a1;
    Vec4f vz;
    f32 spCC;
    f32 y;
    f32 spC4;
    s32 isSorted;
    Vec3f normal;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 spA4;
    s32 i2;
    s32 i;

    x = x;
    if (plIdx->unk0 == NULL) {
        x -= (f32) gBgBlockList->x;
        z -= (f32) gBgBlockList->z;
    }
    for (pl = plStart; (u32) pl < (u32) plEnd; pl++) {
        if ((pl->unk2F & 0x10) && !(pl->unk2F & 4)) {
            continue;
        }
        normal.x = pl->nX * (1.0f / 8191.0f);
        normal.y = pl->nY * (1.0f / 8191.0f);
        normal.z = pl->nZ * (1.0f / 8191.0f);
        if ((normal.y > 0.0f) || !((anyAngle == FALSE) || (normal.y == 0.0f))) { // isFloor || (anyAngle && !isWall)
            // Project XZ onto triangle to get Y
            y = -((normal.x * x) + (normal.z * z) + pl->unk0) / normal.y;
            // Check if XZ is actually within the triangle?
            valid = TRUE;
            for (i = 0; i < 3; i++) {
                vx.f[i] = pl->vX[i];
                vy.f[i] = pl->vY[i];
                vz.f[i] = pl->vZ[i];
            }
            for (i = 0; i < 3; i++) {
                i2 = i + 1;
                if (i2 >= 3) {
                    i2 = 0;
                }
                vx.f[3] = (normal.x * 10.0f) + vx.f[i];
                vy.f[3] = (normal.y * 10.0f) + vy.f[i];
                vz.f[3] = (normal.z * 10.0f) + vz.f[i];
                func_80058D54(&vx, &vy, &vz, i, i2, 3, &nx, &ny, &nz, &spA4);
                if (((nx * x) + (ny * y) + (nz * z) + spA4) > 0.2f) {
                    valid = FALSE;
                    break;
                }
            }
            if (valid) {
                if (plIdx->unk0 != NULL) {
                    mathMtxXFMF(plIdx->unkC, x, y, z, &spCC, &y, &spC4);
                    mathMtxFastXFMF(plIdx->unkC, &normal, &normal);
                }
                D_800BB4A0->y = y;
                D_800BB4A0->unk14 = pl->unk2E;
                D_800BB4A0->norm[0] = normal.x;
                D_800BB4A0->norm[1] = normal.y;
                D_800BB4A0->norm[2] = normal.z;
                D_800BB4A0->obj = plIdx->unk0;
                D_800BB4A0++;
                D_800BB4D4++;
                if (D_800BB4D4 > 9) {
                    STUBBED_PRINTF("TrackGetHeight()-Overflow!!!\n");
                    // break
                    pl = plEnd;
                }
            }
        }
    }

    for (i = 0; i < D_800BB4D4; i++) {
        D_800BB4D0[i] = &D_800BB3B0[i];
    }

    // Sort by Y-coordinate, descending
    do {
        isSorted = TRUE;
        for (i = 0; i < D_800BB4D4 - 1; i++) {
            temp_a0 = D_800BB4D0[i];
            temp_a1 = D_800BB4D0[i + 1];
            if (temp_a0->y < temp_a1->y) {
                D_800BB4D0[i] = D_800BB4D0[i + 1];
                D_800BB4D0[i + 1] = temp_a0;
                isSorted = FALSE;
            }
        }
    } while (isSorted == FALSE);
}

s32 trackGetHeightNearest(Object* obj, f32 x, f32 y, f32 z, f32* height, u8 flags) {
    f32 closestYDist;
    f32 yDist;
    s32 count;
    s32 closest;
    s32 i;
    TrackHeightResult** heights;

    count = trackGetHeight(obj, x, y, z, &heights, 0, flags);
    if (count != 0) {
        closest = 0;
        if (heights[0]->y <= y) {
            closestYDist = y - heights[0]->y;
        } else {
            closestYDist = -(y - heights[0]->y);
        }
        for (i = 1; i < count; i++) {
            if (heights[i]->y <= y) {
                yDist = y - heights[i]->y;
            } else {
                yDist = -(y - heights[i]->y);
            }
            if (yDist < closestYDist) {
                closestYDist = yDist;
                closest = i;
            }
        }
        *height = y - heights[closest]->y;
        return 0;
    }

    *height = 0.0f;
    return 1;
}

s32 trackGetHeightFloor(Object* obj, f32 x, f32 y, f32 z, f32* height, u8 flags) {
    f32 yDiff;
    f32 best;
    s32 count;
    s32 i;
    TrackHeightResult** heights;

    count = trackGetHeight(obj, x, y, z, &heights, 0, flags);
    if (count != 0) {
        best = y - heights[0]->y;
        for (i = 1; i < count; i++) {
            yDiff = y - heights[i]->y;
            if ((yDiff >= 0.0f) && ((best < 0.0f) || (yDiff < best))) { // isBelow && (bestIsAbove || isCloser)
                best = yDiff;
            }
        }

        if (best >= 0.0f) {
            *height = best;
            return 1;
        }

        *height = 0.0f;
        return 0;
    }

    *height = 0.0f;
    return 0;
}

s32 trackGetHeightCeiling(Object* obj, f32 x, f32 y, f32 z, f32* height, u8 flags) {
    f32 yDiff;
    f32 best;
    s32 count;
    s32 i;
    TrackHeightResult** heights;

    count = trackGetHeight(obj, x, y, z, &heights, 0, flags);
    if (count != 0) {
        best = heights[0]->y - y;
        for (i = 1; i < count; i++) {
            yDiff = heights[i]->y - y;
            if ((yDiff >= 0.0f) && ((best < 0.0f) || (yDiff < best))) {
                best = yDiff;
            }
        }
        if (best >= 0.0f) {
            *height = best;
            return 1;
        }

        return 0;
    }

    return 0;
}

void func_80058D54(Vec4f* vX, Vec4f* vY, Vec4f* vZ, s32 p0, s32 p1, s32 p2, f32* outNx, f32* outNy, f32* outNz, f32* arg9) {
    f32 nx;
    f32 ny;
    f32 nz;
    f32 mag;

    // Surface normal calculation (alternate form)
    nx = (vY->f[p0] * (vZ->f[p1] - vZ->f[p2])) + (vY->f[p1] * (vZ->f[p2] - vZ->f[p0])) + (vY->f[p2] * (vZ->f[p0] - vZ->f[p1]));
    ny = (vZ->f[p0] * (vX->f[p1] - vX->f[p2])) + (vZ->f[p1] * (vX->f[p2] - vX->f[p0])) + (vZ->f[p2] * (vX->f[p0] - vX->f[p1]));
    nz = (vX->f[p0] * (vY->f[p1] - vY->f[p2])) + (vX->f[p1] * (vY->f[p2] - vY->f[p0])) + (vX->f[p2] * (vY->f[p0] - vY->f[p1]));
    mag = sqrtf(SQ(nx) + SQ(ny) + SQ(nz));
    if (mag > 0.0f) {
        mag = 1.0f / mag;
        nx *= mag;
        ny *= mag;
        nz *= mag;
    }
    *arg9 = -((vX->f[p0] * nx) + (vY->f[p0] * ny) + (vZ->f[p0] * nz));
    *outNx = nx;
    *outNy = ny;
    *outNz = nz;
}

void trackIntersectMarkBlocksDirty(void) {
    D_800BB539 = 1;
}

int trackIntersectNeedsUpdate(void) {
    return (D_800BB539 != 0) || (D_800BB538 != 0);
}

s8 trackIntersectInitialized(void) {
    return D_80092E80;
}

void func_80058F8C(void) {
    s32 index;
    Unk8005B17C* temp;

    for (index = 0; index < 20; index++){ 
        temp = &gLastLines[index];
        temp->unk14 = 0;
    }
}

void trackIntersectLastLineTick(void) {
    s16 index;
    Unk8005B17C *temp;

    index = 0;
    while (index < 20){
        temp = &gLastLines[index];
        if (temp->unk14){
            temp->unk14--;
        }
        index++;
    }
}

/** Used by HitAnimators to toggle HITS lines */
void trackToggleHitLine(s32 animatorID, Object* parentObject, s32 enableLines) {
    s16 lineCount;
    s32 index;
    ObjDef *objDef;
    ModLineReencoded*hitsLines;

    hitsLines = gLineList;

    if (parentObject != NULL){
        objDef = parentObject->def;
        hitsLines = objDef->pIntersectPoints; //pointer to mobile map's encoded HITS lines? (MODLINES.bin)
        lineCount = objDef->modLineCount;
    } else {    
        hitsLines = gLineList;
        lineCount = gLineListCount;
    }

    if (enableLines){
        for (index = 0; index < lineCount; hitsLines++, index++){
            if (hitsLines->animatorID == animatorID){
                hitsLines->settingsB &= ~0x40;
            }
        }
    } else {
        for (index = 0; index < lineCount; hitsLines++, index++){
            if (hitsLines->animatorID == animatorID){
                hitsLines->settingsB |= 0x40;
            }
        }    
    }
}

// official name: trackIntersect ?
// Collects and pre-processes hit lines from currently loaded blocks. 
void trackIntersectTick(void) {
    s32 a3;
    f32 f0;
    Block* temp_v0_2;
    s32 sp700;
    s32 sp6FC;
    s32 sp6F8;
    ModLineReencoded* var_s2;
    f32 temp_fs0;
    f32 temp_fs1;
    s16 temp_a2;
    s16 var_a1_2;
    s32 var_a1;
    s32 gridIndex;
    s32 var_a0;
    s32 var_a2;
    s32 var_s1;
    s8* sp6D0;
    s32 var_s6;
    s32 isSorted;
    ModLine* var_s0;
    s16 sp84[800];

    D_80092E80 = 0;
    if (D_800BB539 == 1) {
        D_800BB538 = 1;
        D_800BB539 = 0;
        return;
    }

    if (D_800BB538 == 0) {
        return;
    }

    D_800BB538 = 0;
    gLineListCount = 0;
    gPointListCount = 0;
    for (sp6F8 = 0; sp6F8 < 5; sp6F8++) {
        sp6D0 = mapGetBlockGridLayer(sp6F8);
        for (sp6FC = 0; sp6FC < BLOCKS_GRID_SPAN; sp6FC++) {
            for (sp700 = 0; sp700 < BLOCKS_GRID_SPAN; sp700++) {
                gridIndex = (sp6FC << 4)  + sp700; // can't use the GRID_INDEX macro here, bitshift required
                if (sp6D0[gridIndex] >= 0) {
                    temp_v0_2 = mapGetBlockByIndex(sp6D0[gridIndex]);
                    for (var_s6 = 0; var_s6 < temp_v0_2->hits_line_count; var_s6++) {
                        var_s0 = &temp_v0_2->ptr_hits_lines[var_s6];
                        var_s2 = &gLineList[gLineListCount];
                        var_s2->heightA = var_s0->heightA;
                        var_s2->heightB = var_s0->heightB;
                        var_s2->settingsB = var_s0->settingsB;
                        var_s2->settingsA = var_s0->settingsA;
                        var_s2->animatorID = var_s0->animatorID;
                        temp_fs0 = (sp700 * BLOCKS_GRID_UNIT_F) + gWorldX;
                        temp_fs1 = (sp6FC * BLOCKS_GRID_UNIT_F) + gWorldZ;
                        for (var_s1 = 0; var_s1 < 2; var_s1++) {
                            var_s2->indexes[var_s1] = func_8005A2BC(var_s0->somePos[var_s1] + temp_fs0, var_s0->somePos[var_s1 + 2], f0 = var_s0->somePos[var_s1 + 4] + temp_fs1, gLineListCount, sp84);
                        }
                        gLineListCount += 1;
                    }
                }
            }
        }
    }
    for (var_a0 = 0; var_a0 < gLineListCount; var_a0++) {
        var_s2 = &gLineList[var_a0];
        for (var_a2 = 0; var_a2 < 2; var_a2++) {
            if (sp84[var_s2->indexes[var_a2] * 2 + 0] >= 0 && var_a0 != sp84[var_s2->indexes[var_a2] * 2 + 0]) {
                var_s2->indexes[var_a2 + 2] = sp84[var_s2->indexes[var_a2] * 2 + 0];
            } else if (sp84[var_s2->indexes[var_a2] * 2 + 1] >= 0 && var_a0 != sp84[var_s2->indexes[var_a2] * 2 + 1]) {
                var_s2->indexes[var_a2 + 2] = sp84[var_s2->indexes[var_a2] * 2 + 1];
            } else {
                var_s2->indexes[var_a2 + 2] = -1;
            }
        }
    }
    for (var_a0 = 0; var_a0 < gLineListCount; var_a0++) {
        gLineIndex[var_a0] = var_a0;
    }
    do {
        isSorted = TRUE;
        for (var_a0 = 0; var_a0 < gLineListCount - 1; var_a0++) {
            if ((gLineList[ gLineIndex[var_a0]].settingsB & 0x3F) < (gLineList[gLineIndex[var_a0 + 1]].settingsB & 0x3F)) {
                var_a1 = gLineIndex[var_a0];
                gLineIndex[var_a0] = gLineIndex[var_a0 + 1] & 0xFFFF;
                gLineIndex[var_a0 + 1] = var_a1;
                isSorted = FALSE;
            }
        }
    } while (isSorted == FALSE);
    for (var_a0 = 0; var_a0 < 38; var_a0++) {
        D_800BB4E0[var_a0] = 0xFFFF;
    }
    temp_a2 = -1;
    for (var_a0 = 0; var_a0 < gLineListCount; var_a0++) {
        var_a1_2 = gLineList[gLineIndex[var_a0]].settingsB & 0x3F;
        if (var_a1_2 > 18) {
            var_a1_2 = 1;
            STUBBED_PRINTF("trackIntersect: FUNC OVERFLOW %d\n", var_a1_2); // @bug: should be before the = 1
        }
        if (temp_a2 != var_a1_2) {
            D_800BB4E0[var_a1_2 * 2] = var_a0;
            if (temp_a2 != -1) {
                D_800BB4E0[temp_a2 * 2 + 1] = var_a0;
            }
            temp_a2 = var_a1_2;
        }
    }
    if (temp_a2 != -1) {
        D_800BB4E0[temp_a2 * 2 + 1] = gLineListCount;
    }
    /* default.dol
    if (gLineListCount < 0) {
        STUBBED_PRINTF("trackIntersect: linefunctable overflow!!!\n");
    }
    if (gLineListCount > 400) { // comparison updated for dp
        STUBBED_PRINTF("trackIntersect() MAX_LINEPOINTS overflow!!!\n");
    }
    if (gPointListCount > 400) { // comparison updated for dp
        STUBBED_PRINTF("trackIntersect() MAX_LINEPOINTS overflow!!!\n");
    }
    */
    D_80092E80 = 1;
}

// official name: intersectModLineBuild ?
void trackIntersectModLineBuild(ObjDef* objdef) {
    ModLineReencoded* var_s2;
    ModLine* var_s0;
    ModLine* var_s7;
    s32 sp6C8;
    s16 var_s2_3;
    s16 var_t0;
    s16 var_v0_2;
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_s1;
    s32 var_s3;
    s16 sp68[800];
    s16* temp_v1_2;
    s16* s2;
    f32 f0;

    D_800BB539 = 1;
    gLineListCount = 0;
    gPointListCount = 0;
    var_s0 = objdef->pModLines;        
    sp6C8 = objdef->modLineCount;
    for (var_s3 = 0; var_s3 < sp6C8; var_s3++, gLineListCount++) {
        var_s7 = &var_s0[var_s3];
        var_s2 = &gLineList[gLineListCount];
        var_s2->heightA = var_s7->heightA;
        var_s2->heightB = var_s7->heightB;
        var_s2->settingsB = var_s7->settingsB;
        var_s2->settingsA = var_s7->settingsA;
        var_s2->animatorID = var_s7->animatorID;
        for (var_s1 = 0; var_s1 < 2; var_s1++) {
            var_s2->indexes[var_s1] = func_8005A2BC(var_s7->somePos[var_s1], var_s7->somePos[var_s1 + 2], f0 = var_s7->somePos[var_s1 + 4], gLineListCount, sp68);
        }

        if (gPointListCount >= 400) {
            STUBBED_PRINTF("intersectModLineBuild point list overflow, %d/%d\n");
            break;
        }
    }
    for (var_s3 = 0; var_s3 < gLineListCount; var_s3++) {
        var_s2 = &gLineList[var_s3];
        for (var_s1 = 0; var_s1 < 2; var_s1++) {
            temp_v1_2 = &sp68[var_s2->indexes[var_s1] * 2];
            if (temp_v1_2[0] >= 0 && var_s3 != temp_v1_2[0]) {
                var_s2->indexes[var_s1 + 2] = temp_v1_2[0];
            } else {
                if (temp_v1_2[1] >= 0 && var_s3 != temp_v1_2[1]) {
                    var_s2->indexes[var_s1 + 2] = temp_v1_2[1];
                } else {
                    var_s2->indexes[var_s1 + 2] = -1;
                }
            }
        }
    }
    temp_a0 = (gLineListCount * sizeof(ModLineReencoded)) + (gPointListCount * sizeof(Vec3f)) + (38 * sizeof(u8));
    if (temp_a0 == 0) {
        return;
    }
    objdef->pIntersectPoints = mmAlloc(temp_a0, COLOUR_TAG_YELLOW, ALLOC_NAME("int:wobjlist"));
    objdef->nextIntersectLine = (Vec3f*)(objdef->pIntersectPoints + gLineListCount);
    objdef->nextIntersectPoint = (u8*)((u8*)objdef->nextIntersectLine + (gPointListCount * 0xC)); // this should use sizeof(Vec3f) instead of 0xC
    for (var_s3 = 0; var_s3 < 38; var_s3++) {
        objdef->nextIntersectPoint[var_s3] = 0xFF;
    }
    var_s2_3 = -1;
    for (var_s3 = 0; var_s3 < gLineListCount; var_s3++) {
        var_t0 = 0;
        for (var_s1 = 0; var_s1 < gLineListCount; var_s1++) {
            if ((gLineList[var_s1].settingsB & 0x3F) < (gLineList[var_t0].settingsB & 0x3F)) {
                var_t0 = var_s1;
            }
        }
        var_v0_2 = gLineList[var_t0].settingsB & 0x3F;
        if (var_v0_2 > 18) {
            var_v0_2 = 1;
            STUBBED_PRINTF("trackIntersect: FUNC OVERFLOW %d\n", var_v0_2); // @bug: should be before the = 1
        }
        if (var_v0_2 != var_s2_3) {
            objdef->nextIntersectPoint[var_v0_2 * 2 + 0] = var_s3;
            if (var_s2_3 != -1) {
                objdef->nextIntersectPoint[var_s2_3 * 2 + 1] = var_s3;
            }
            var_s2_3 = var_v0_2;
        }
        for (var_a0_2 = 0; var_a0_2 < var_s3; var_a0_2++) {
            if (var_t0 == objdef->pIntersectPoints[var_a0_2].indexC) {
                objdef->pIntersectPoints[var_a0_2].indexC = var_s3;
            }
            if (var_t0 == objdef->pIntersectPoints[var_a0_2].indexD) {
                objdef->pIntersectPoints[var_a0_2].indexD = var_s3;
            }
        }
        for (var_a0_2 = 0; var_a0_2 < gLineListCount; var_a0_2++) {
            if (gLineList[var_a0_2].settingsB != 19) {
                if (var_t0 == gLineList[var_a0_2].indexC) {
                    gLineList[var_a0_2].indexC = var_s3;
                }
                if (var_t0 == gLineList[var_a0_2].indexD) {
                    gLineList[var_a0_2].indexD = var_s3;
                }
            }
        }
        bcopy(&gLineList[var_t0], objdef->pIntersectPoints + (var_s3), sizeof(ModLineReencoded));
        gLineList[var_t0].settingsB = 0x13;
    }
    if (var_s2_3 != -1) {
        objdef->nextIntersectPoint[var_s2_3 * 2 + 1] = (s8) gLineListCount;
    }
    bcopy(gPointList, objdef->nextIntersectLine, gPointListCount * sizeof(Vec3f));
    gLineListCount = 0;
    gPointListCount = 0;
}

s32 trackGetLineIntersect(Vec3f* pos1, Vec3f* pos2, f32 arg2, s32 arg3, TrackLineIntersectResult* result, Object* obj, s8 arg6, s8 arg7, u8 arg8, s8 arg9) {
    f32 spE0[2];
    s32 i;
    f32 distance;
    s32 isInRange;
    Object** objects;
    Unk8005B17C* temp_v0_2;
    Vec3f spC0;
    Vec3f spB4;
    Vec3f spA8;
    Vec3f sp9C;
    f32 dz;
    s32 objectCount;
    f32 dx;
    f32 dy;
    Object* parentObj;
    Object* currentObj;
    ModelInstance *modelInst;
    Model *model;

    D_800BB53A = 0;
    if (result != NULL) {
        result->unk50 = -1;
        result->unk51 = -1;
    }
    if (obj != NULL) {
        parentObj = obj->parent;
    } else {
        parentObj = NULL;
    }

    if (parentObj != NULL) {
        camTransformPointByObject(pos1->f[0], pos1->f[1], pos1->f[2], &spC0.f[0], &spC0.f[1], &spC0.f[2], parentObj);
        camTransformPointByObject(pos2->f[0], pos2->f[1], pos2->f[2], &spB4.f[0], &spB4.f[1], &spB4.f[2], parentObj);
    } else {
        bcopy(pos1, &spC0, sizeof(Vec3f));
        bcopy(pos2, &spB4, sizeof(Vec3f));
    }

    objects = objGetAllOfType(OBJTYPE_MobileMap, &objectCount);
    for (i = 0; i < objectCount; i++) {
        currentObj = objects[i];
        if (currentObj != obj && currentObj->matrixIdx >= 0 && currentObj->def->pIntersectPoints != NULL) {
            if (currentObj->objhitInfo != NULL && !(currentObj->objhitInfo->unk58 & 1)) {
                continue;
            }
            dx = currentObj->srt.transl.f[0] - spC0.f[0];
            dy = currentObj->srt.transl.f[1] - spC0.f[1];
            dz = currentObj->srt.transl.f[2] - spC0.f[2];
            modelInst = currentObj->modelInsts[currentObj->objhitInfo->unkA0];
            model = modelInst->model;
            distance = model->maxAnimatedVertDistance + 0x32;
            distance = SQ(distance);

            isInRange = FALSE;
            if ((SQ(dx) + SQ(dy) + SQ(dz)) < distance) {
                isInRange = TRUE;
            }

            if (isInRange == FALSE) {
                dx = currentObj->srt.transl.f[0] - spB4.f[0];
                dy = currentObj->srt.transl.f[1] - spB4.f[1];
                dz = currentObj->srt.transl.f[2] - spB4.f[2];
                if ((SQ(dx) + SQ(dy) + SQ(dz)) < distance) {
                    isInRange = TRUE;
                }
            }

            if (isInRange != FALSE) {
                if (arg8 != 0xFF && (temp_v0_2 = func_8005B17C(obj, currentObj, arg8), (temp_v0_2 != NULL))) {
                    spA8.f[0] = temp_v0_2->unk8.f[0];
                    spA8.f[1] = temp_v0_2->unk8.f[1];
                    spA8.f[2] = temp_v0_2->unk8.f[2];
                } else {
                    camInverseTransformPointByObject(spC0.f[0], spC0.f[1], spC0.f[2], &spA8.f[0], &spA8.f[1], &spA8.f[2], currentObj);
                }

                camInverseTransformPointByObject(spB4.f[0], spB4.f[1], spB4.f[2], &sp9C.f[0], &sp9C.f[1], &sp9C.f[2], currentObj);
                
                if (func_8005A3F8(&spA8, &sp9C, arg2, arg3, result, currentObj, arg6, arg7, arg9, obj) != 0) {
                    camTransformPointByObject(sp9C.f[0], sp9C.f[1], sp9C.f[2], &spB4.f[0], &spB4.f[1], &spB4.f[2], currentObj);
                }

                if (arg8 != 0xFF) {
                    temp_v0_2 = func_8005B204(obj, currentObj, arg8);
                    if (temp_v0_2 != NULL) {
                        temp_v0_2->unk8.f[0] = sp9C.f[0];
                        temp_v0_2->unk8.f[1] = sp9C.f[1];
                        temp_v0_2->unk8.f[2] = sp9C.f[2];
                    }
                }
            }
        }
    }

    func_8005A3F8(&spC0, &spB4, arg2, arg3, result, NULL, arg6, arg7, arg9, obj);
    if ((D_800BB53A != 0) && (result != NULL)) {
        spE0[0] = result->unk38.f[1] - result->unkC;
        spE0[1] = result->unk38.f[2] - result->unk10;
        result->unk2C.f[0] = result->unk18 - result->unk14;
        result->unk2C.f[1] = 0.0f;
        result->unk2C.f[2] = result->unk4 - result->unk8;
        dy = 1.0f / sqrtf(SQ(result->unk2C.x) + SQ(result->unk2C.z));
        result->unk2C.f[0] *= dy;
        result->unk2C.f[2] *= dy;
        result->unk38.f[0] = -((result->unk14 * result->unk2C.z) + (result->unk2C.x * result->unk4));
        if (result->unk0 != NULL) {
            camTransformPointByObject(result->unk4, result->unkC, result->unk14, &result->unk4, &result->unkC, &result->unk14, result->unk0);
            camTransformPointByObject(result->unk8, result->unk10, result->unk18, &result->unk8, &result->unk10, &result->unk18, result->unk0);
        }
        if (parentObj != NULL) {
            camInverseTransformPointByObject(result->unk4, result->unkC, result->unk14, &result->unk4, &result->unkC, &result->unk14, parentObj);
            camInverseTransformPointByObject(result->unk8, result->unk10, result->unk18, &result->unk8, &result->unk10, &result->unk18, parentObj);
        }
        result->unk1C.f[0] = result->unk18 - result->unk14;
        result->unk1C.f[1] = 0.0f;
        result->unk1C.f[2] = result->unk4 - result->unk8;
        dy = 1.0f / sqrtf(SQ(result->unk1C.x) + SQ(result->unk1C.z));
        result->unk1C.f[0] *= dy;
        result->unk1C.f[2] *= dy;
        result->unk38.f[1] = spE0[0] + result->unkC;
        result->unk38.f[2] = spE0[1] + result->unk10;
        result->unk1C.f[3] = -((result->unk14 * result->unk1C.z) + (result->unk1C.x * result->unk4));
    }

    if (D_800BB53A != 0) {
        if (parentObj != NULL) {
            camInverseTransformPointByObject(spB4.f[0], spB4.f[1], spB4.f[2], pos2->f, &pos2->f[1], &pos2->f[2], parentObj);
        } else {
            bcopy(&spB4, pos2, sizeof(Vec3f));
        }
    }

    return D_800BB53A;
}

// official name: insertPoint
s32 func_8005A2BC(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s16* arg4) {
    s32 i;

    for (i = 0; i < gPointListCount; i++) {
        if (arg0 == gPointList[i].f[0] && arg1 == gPointList[i].f[1] && arg2 == gPointList[i].f[2]) {
            arg4[i * 2 + 1] = arg3;
            return i;
        }
    }
    gPointList[gPointListCount].f[0] = arg0;
    gPointList[gPointListCount].f[1] = arg1;
    gPointList[gPointListCount].f[2] = arg2;
    arg4[gPointListCount * 2 + 0] = arg3;
    arg4[gPointListCount * 2 + 1] = -1;
    gPointListCount++;
    if (gPointListCount == 400) {
        STUBBED_PRINTF("insertPoint array overrun %d/%d\n", gPointListCount, 400);
        gPointListCount--;
    }
    return gPointListCount - 1;
}

s32 func_8005A3F8(Vec3f* arg0, Vec3f* arg1, f32 arg2, s32 arg3, TrackLineIntersectResult* arg4, Object* arg5, s8 arg6, s8 arg7, s8 arg8, Object* arg9) {
    f32 sp1C0[2];
    f32 sp1B8[2];
    ModLineReencoded* var_a0_2;
    f32 temp_fa0;
    f32 sp1A8[2];
    f32 sp1A0[2];
    f32 sp198[2];
    f32 sp194;
    f32 sp190;
    f32 temp_fa1;
    f32 temp_fs0;
    f32 sp178[4];
    f32 sp168[4];
    f32 sp158[4];
    f32 temp_fs1;
    f32 temp_fs3;
    f32 temp_ft0;
    f32 var_ft4;
    f32 temp_fs1_2;
    f32 var_ft1;
    f32 sp13C;
    f32 sp138;
    f32 sp134;
    f32 sp130;
    f32 sp128[2];
    s32 sp124;
    f32 temp_fs0_2;
    f32 var_fv0;
    f32 temp_fa0_2;
    s16 var_v1_2;
    s16 var_v1_4;
    s32 sp110;
    s32 var_s1;
    s32 var_s3;
    s32 sp104;
    s32 sp100;
    s32 j;
    s16 spF8[2];
    s32 a1;
    s32 a2;
    s16* spEC;
    ModLineReencoded* spE8;
    s32 i;
    Vec3f* spE0;
    s8 var_r25;
    s8 var_s0;
    s8 spDF;
    s8 spDE;
    s16 spD0[5];
    f32 spBC[5];
    f32 spA8[5];

    if (arg5 != NULL) {
        if (arg7 != -1) {
            sp104 = arg5->def->nextIntersectPoint[arg7 * 2 + 0];
            sp100 = arg5->def->nextIntersectPoint[arg7 * 2 + 1];
        } else {
            sp104 = 0;
            sp100 = arg5->def->modLineCount;
        }
        spEC = NULL;
        spE8 = arg5->def->pIntersectPoints;
        spE0 = arg5->def->nextIntersectLine;
        if (arg5->stateFlags & OBJSTATE_DISABLE_MODLINES) {
            sp100 = 0;
        }
    } else {
        if (arg7 != -1) {
            sp104 = D_800BB4E0[arg7 * 2 + 0];
            sp100 = D_800BB4E0[arg7 * 2 + 1];
        } else {
            sp104 = 0;
            sp100 = gLineListCount;
        }
        spEC = gLineIndex;
        spE8 = gLineList;
        spE0 = gPointList;
    }
    spDF = !(arg3 & 1);
    spDE = arg3 & 2;
    sp1C0[0] = arg0->f[0];
    sp1B8[0] = arg0->f[2];
    sp1C0[1] = arg1->f[0];
    sp1B8[1] = arg1->f[2];
    if (sp1C0[0] < sp1C0[1]) {
        sp13C = sp1C0[0];
        sp138 = sp1C0[1];
    } else {
        sp138 = sp1C0[0];
        sp13C = sp1C0[1];
    }
    if (sp1B8[0] < sp1B8[1]) {
        sp134 = sp1B8[0];
        sp130 = sp1B8[1];
    } else {
        sp134 = sp1B8[1];
        sp130 = sp1B8[0];
    }
    sp13C -= arg2;
    sp138 += arg2;
    sp134 -= arg2;
    sp130 += arg2;
    sp13C -= 200.0f;
    sp138 += 200.0f;
    sp134 -= 200.0f;
    sp130 += 200.0f;
    sp110 = 0;
    var_s3 = 1;
    while (var_s3 != 0) {
        var_s3 = 0;
        for (sp124 = sp104; sp124 < sp100; sp124++) {
            sp190 = -1.0f;
            if (spEC != NULL) {
                var_a0_2 = &spE8[spEC[sp124]];
            } else {
                var_a0_2 = &spE8[sp124];
            }

            if (!(~var_a0_2->settingsA & arg6)) {
                continue;
            }

            if (var_a0_2->settingsB & 0x40) {
                continue;
            }
            a1 = var_a0_2->indexA;
            a2 = var_a0_2->indexB;
            var_r25 = !(var_a0_2->settingsB & 0x80);
            if (spDE) {
                var_r25 = 1;
            }
            sp1A8[0] = spE0[a1].f[0];
            sp1A0[0] = spE0[a1].f[1];
            sp198[0] = spE0[a1].f[2];
            sp1A8[1] = spE0[a2].f[0];
            sp1A0[1] = spE0[a2].f[1];
            sp198[1] = spE0[a2].f[2];
            if (
                (sp1A8[0] < sp13C && sp1A8[1] < sp13C) ||
                (sp138 < sp1A8[0] && sp138 < sp1A8[1]) ||
                (sp198[0] < sp134 && sp198[1] < sp134) ||
                (sp130 < sp198[0] && sp130 < sp198[1])
            ) {
                continue;
            }
            temp_fa0_2 = sp1A0[0];
            if (sp1A0[1] < temp_fa0_2) {
                temp_fa0_2 = sp1A0[1];
            }
            temp_fa0_2 -= arg8;
            if (var_a0_2->settingsA & 0x80) {
                sp128[0] = ((s16*)var_a0_2)[0]; // should be heightA
                sp128[1] = sp128[0];
            } else {
                sp128[0] = var_a0_2->heightA;
                sp128[1] = var_a0_2->heightB;
            }
            var_fv0 = sp1A0[0] + sp128[0];
            temp_fa1 = sp1A0[1] + sp128[1];
            if (var_fv0 < temp_fa1) {
                var_fv0 = temp_fa1;
            }
            var_fv0 += arg8;
            if (arg0->f[1] < temp_fa0_2 || var_fv0 < arg0->f[1]) {
                continue;
            }
            temp_fs0 = sp1A8[1] - sp1A8[0];
            temp_fs1 = sp198[1] - sp198[0];
            sp194 = SQ(temp_fs0) + SQ(temp_fs1);
            if (sp194 == 0.0f) {
                continue;
            }
            sp194 = sqrtf(sp194);
            temp_fs0 = temp_fs0 / sp194;
            temp_fs1 = temp_fs1 / sp194;
            sp178[0] = temp_fs0;
            sp168[0] = temp_fs1;
            sp158[0] = -((sp178[0] * sp1A8[0]) + (sp168[0] * sp198[0]));
            sp178[1] = -temp_fs0;
            sp168[1] = -temp_fs1;
            sp158[1] = -((sp178[1] * sp1A8[1]) + (sp168[1] * sp198[1]));
            sp178[2] = -temp_fs1;
            sp168[2] = temp_fs0;
            sp158[2] = -((sp178[2] * (sp1A8[0] + (2.0f * sp178[2]))) + (sp168[2] * ((sp198[0] + (2.0f * sp168[2])))));
            sp178[3] = temp_fs1;
            sp168[3] = -temp_fs0;
            sp158[3] = -((sp178[3] * (sp1A8[0] + (arg2 * sp178[3]))) + (sp168[3] * (sp198[0] + (sp168[3] * arg2))));
            D_800BB530 = 0.5f * (sp178[3] * arg2);
            D_800BB534 = 0.5f * (sp168[3] * arg2);
            for (i = 0; i < 2; i++) {
                var_v1_2 = 1;
                spF8[i] = 0;
                for (j = 0; j < 4; j++) {
                    temp_fa1 = (sp1C0[i] * sp178[j]) + (sp1B8[i] * sp168[j]) + sp158[j];
                    if (temp_fa1 < 0.0f) {
                        spF8[i] |= var_v1_2;
                    }
                    var_v1_2 <<= 1;
                }
            }
            var_v1_2 = spF8[0] ^ spF8[1];
            var_v1_4 = spF8[0] & spF8[1];
            sp190 = 1.0f;
            if ((spF8[0] & 0xC) == 0xC) {
                if (spF8[0] & 1) {
                    var_s3 = func_8005B274(sp1C0, sp1B8, sp1A8[0], sp198[0], arg2, var_r25);
                    sp190 = 0.0f;
                } else if (spF8[0] & 2) {
                    var_s3 = func_8005B274(sp1C0, sp1B8, sp1A8[1], sp198[1], arg2, var_r25);
                    sp190 = 1.0f;
                } else if (var_r25 != 0) {
                    sp1C0[1] += D_800BB530;
                    sp1B8[1] += D_800BB534;
                }
            } else {
                if (var_v1_2 & 0xC) {
                    if (var_v1_4 & 1) {
                        var_s3 = func_8005B274(sp1C0, sp1B8, sp1A8[0], sp198[0], arg2, var_r25);
                        sp190 = 0.0f;
                    } else if (var_v1_4 & 2) {
                        var_s3 = func_8005B274(sp1C0, sp1B8, sp1A8[1], sp198[1], arg2, var_r25);
                        sp190 = 1.0f;
                    } else if (spF8[0] & 4) {
                        temp_fs0 = (sp1C0[1] - sp1C0[0]);
                        temp_fs1 = (sp1B8[1] - sp1B8[0]);
                        temp_fa0 = (sp1C0[0] * sp178[3]) + (sp1B8[0] * sp168[3]) + sp158[3];
                        temp_fa1 = (sp1C0[1] * sp178[3]) + (sp1B8[1] * sp168[3]) + sp158[3];
                        if (temp_fa0 != temp_fa1) {
                            temp_fa1 = temp_fa0 / (temp_fa0 - temp_fa1);
                        } else {
                            temp_fa1 = 0.0f;
                        }
                        temp_fs0_2 = (temp_fs0 * temp_fa1) + sp1C0[0];
                        temp_fs1_2 = (temp_fs1 * temp_fa1) + sp1B8[0];
                        D_800BB52C = temp_fa1;
                        var_s0 = TRUE;
                        var_fv0 = (temp_fs0_2 * sp178[0]) + (temp_fs1_2 * sp168[0]) + sp158[0];
                        if (var_fv0 < 0.0f) {
                            var_s3 = func_8005B274(sp1C0, sp1B8, sp1A8[0], sp198[0], arg2, var_r25);
                            var_s0 = FALSE;
                            sp190 = 0.0f;
                        }
                        var_fv0 = (temp_fs0_2 * sp178[1]) + (temp_fs1_2 * sp168[1]) + sp158[1];
                        if (var_fv0 < 0.0f) {
                            var_s3 = func_8005B274(sp1C0, sp1B8, sp1A8[1], sp198[1], arg2, var_r25);
                            var_s0 = FALSE;
                            sp190 = 1.0f;
                        }
                        if (var_s0 != FALSE) {
                            var_s3 = 1;
                            if (var_r25 != 0) {
                                if (spDF) {
                                    temp_fa1 = (sp1C0[1] * sp178[3]) + (sp1B8[1] * sp168[3]) + sp158[3];
                                    sp1C0[1] -= temp_fa1 * sp178[3];
                                    sp1B8[1] -= temp_fa1 * sp168[3];
                                    while ((temp_fa1 = (sp1C0[1] * sp178[3]) + (sp1B8[1] * sp168[3]) + sp158[3]) < 0.1f) {
                                        sp1C0[1] += sp178[3] * 0.1f;
                                        sp1B8[1] += sp168[3] * 0.1f;
                                    }
                                } else {
                                    sp1C0[1] = temp_fs0_2;
                                    sp1B8[1] = temp_fs1_2;
                                    while ((temp_fa1 = (sp1C0[1] * sp178[3]) + (sp1B8[1] * sp168[3]) + sp158[3]) < 0.1f) {
                                        sp1C0[1] += sp178[3] * 0.1f;
                                        sp1B8[1] += sp168[3] * 0.1f;
                                    }
                                }
                                temp_fs0 = sp1C0[1] - sp1A8[0];
                                temp_fs1 = sp1B8[1] - sp198[0];
                                sp190 = sqrtf(SQ(temp_fs0) + SQ(temp_fs1));
                                sp190 /= sp194;
                            }
                        }
                    }
                }
            }
            if (var_s3 != 0) {
                break;
            }
        }
        if (var_s3 != 0) {
            spD0[sp110] = sp124;
            spBC[sp110] = D_800BB52C;
            spA8[sp110] = sp190;
            sp110++;
            if (sp110 > 4) {
                var_s3 = 0;
                if (var_r25 != 0) {
                    sp1C0[1] = sp1C0[0];
                    sp1B8[1] = sp1B8[0];
                }
            }
        }
    }
    if ((sp110 != 0) && (arg4 != NULL)) {
        var_s1 = sp110 - 1;
        if (spDF == 0) {
            var_s1 = 0;
        }
        temp_fs0 = arg1->f[0];
        temp_fs0 -= sp1C0[0];
        temp_fs1 = arg1->f[2];
        temp_fs1 -= sp1B8[0];
        arg4->unk44 = temp_fa1 = sqrtf(SQ(temp_fs0) + SQ(temp_fs1)) * spBC[0];
        arg4->unk48 = spA8[var_s1];
        sp124 = spD0[var_s1];
        if (spEC != NULL) {
            var_a0_2 = &spE8[spEC[sp124]];
        } else {
            var_a0_2 = &spE8[sp124];
        }
        a1 = var_a0_2->indexA;
        a2 = var_a0_2->indexB;
        if (var_a0_2->settingsA & 0x80) {
            sp128[0] = ((s16*)var_a0_2)[0]; // should be heightA
            sp128[1] = sp128[0];
        } else {
            sp128[0] = var_a0_2->heightA;
            sp128[1] = var_a0_2->heightB;
        }
        arg4->unk4 = spE0[a1].f[0];
        arg4->unkC = spE0[a1].f[1];
        arg4->unk38.f[1] = sp128[0] + arg4->unkC;
        arg4->unk14 = spE0[a1].f[2];
        arg4->unk8 = spE0[a2].f[0];
        arg4->unk10 = spE0[a2].f[1];
        arg4->unk38.f[2] = sp128[1] + arg4->unk10;
        arg4->unk18 = spE0[a2].f[2];
        arg4->unk50 = var_a0_2->settingsB & 0x3F;
        arg4->unk52 = var_a0_2->settingsA;
        arg4->unk51 = var_a0_2->animatorID;
        arg4->unk0 = arg5;
        arg4->unk4C = var_a0_2->indexC;
        arg4->unk4E = var_a0_2->indexD;
    }
    if (sp110 != 0) {
        D_800BB53A += 1;
        sp110 = 1;
        arg1->f[0] = sp1C0[1];
        arg1->f[2] = sp1B8[1];
    }
    return sp110;
}

Unk8005B17C* func_8005B17C(Object* arg0, Object* arg1, u8 arg2) {
    Unk8005B17C* temp_v1;
    s16 i = 0;

    for (;i < 20; i++) {
        temp_v1 = &gLastLines[i];
        if (temp_v1->unk14 == 0) {continue;}
        if (arg0 != temp_v1->unk0) {continue;}
        if (arg1 != temp_v1->unk4) {continue;}
        if (temp_v1->unk15 != arg2) {continue;}

        temp_v1->unk14 = 0U;
        return temp_v1;
    }

    return NULL;
}

Unk8005B17C* func_8005B204(Object* obj, Object* worldObj, u8 arg2) {
    Unk8005B17C* temp_v1;
    s16 i = 0;

    for (;i < 20; i++) {
        temp_v1 = &gLastLines[i];
        if (temp_v1->unk14 == 0) {
            temp_v1->unk0 = obj;
            temp_v1->unk4 = worldObj;
            temp_v1->unk15 = arg2;
            temp_v1->unk14 = 2;
            return temp_v1;
        }
    }
    STUBBED_PRINTF("NO FREE LAST LINE\n");
    return NULL;
}

s32 func_8005B274(f32* arg0, f32* arg1, f32 arg2, f32 arg3, f32 arg4, s8 arg5) {
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 var_ft4;
    f32 sp20;
    f32 sp38;
    f32 sp34;
    f32 sp30;

    if (arg4 == 0.0f) {
        return 0;
    }
    sp38 = sp20 = (SQ((arg0[0] - arg2)) + SQ((arg1[0] - arg3))) - SQ(arg4);
    if (sp38 < 0/*.0f*/) {
        if (arg5 != 0) {
            arg0[1] = arg0[0] + D_800BB530;
            arg1[1] = arg1[0] + D_800BB534;
        }
        return 0;
    }
    sp54 = arg0[1] - arg0[0];
    sp50 = arg1[1] - arg1[0];
    sp4C = SQ(sp54) + SQ(sp50);
    if (sp4C > 0.0f) {
        sp48 = sp4C;
        sp44 = 2.0f * ((sp54 * (arg0[0] - arg2)) + (sp50 * (arg1[0] - arg3)));
        sp4C = SQ(sp44) - (4.0f * sp4C * sp38);
        if (sp4C >= 0.0f) {
            sp20 = sqrtf(sp4C);
            var_ft4 = (-sp44 + sp20) / (2.0f * sp48);
            sp38 = (-sp44 - sp20) / (2.0f * sp48);
            if (var_ft4 < 0.0f) {
                var_ft4 = 10.0f;
            }
            if (sp38 < 0.0f) {
                sp38 = 10.0f;
            }
            if (sp38 < var_ft4) {
                var_ft4 = sp38;
            }
            if ((var_ft4 >= 0.0f) && (var_ft4 <= 1.0f)) {
                D_800BB52C = var_ft4;
                if (arg5 != 0) {
                    sp34 = arg0[0] + (var_ft4 * sp54);
                    sp30 = arg1[0] + (var_ft4 * sp50);
                    sp48 = (sp34 - arg2) / arg4;
                    sp44 = (sp30 - arg3) / arg4;
                    sp20 = -((sp34 * sp48) + (sp30 * sp44));
                    var_ft4 = ((sp48 * arg0[1]) + (arg1[1] * sp44) + sp20);
                    arg0[1] -= var_ft4 * sp48;
                    arg1[1] -= var_ft4 * sp44;
                    while (((arg0[1] * sp48) + (arg1[1] * sp44) + sp20) < 0.1f) {
                        arg0[1] += sp48 * 0.1f;
                        arg1[1] += sp44 * 0.1f;
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}

//parent_object_to_mobile_map_object?
void trackIntersect_func_8005B5B8(Object* arg0, Object* arg1, s32 arg2) {
    Object* parent;
    ObjectHitInfo* hitInfo;
    f32 speedY;
    f32 speedX;
    f32 speedZ;
    s32 dYaw;

    parent = arg0->parent;

    if (parent == arg1) {
        return;
    }

    //Camera-related
    if (parent != NULL) {
        camUpdateObjectMatrix(parent);
    }
    if (arg1 != NULL) {
        camUpdateObjectMatrix(arg1);
    }

    arg0->parent = arg1;
    hitInfo = arg0->objhitInfo;

    if (parent != NULL) {
        camTransformPointByObject(arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, &arg0->globalPosition.x, &arg0->globalPosition.y, &arg0->globalPosition.z, parent);
        camTransformPointByObject(arg0->prevLocalPosition.x, arg0->prevLocalPosition.y, arg0->prevLocalPosition.z, &arg0->prevGlobalPosition.x, &arg0->prevGlobalPosition.y, &arg0->prevGlobalPosition.z, parent);
        camRotatePointByObject(arg0->velocity.x, 0, arg0->velocity.z, &speedX, &speedY, &speedZ, parent);
        dYaw = parent->srt.yaw + arg0->srt.yaw;
    } else {
        speedX = arg0->velocity.x;
        speedZ = arg0->velocity.z;
        dYaw = arg0->srt.yaw;
    }

    if (arg2 != 0) {
        parent = arg0->parent;
        if (arg0->parent != NULL) {
            camInverseTransformPointByObject(arg0->globalPosition.x, arg0->globalPosition.y, arg0->globalPosition.z, &arg0->srt.transl.x, &arg0->srt.transl.y, &arg0->srt.transl.z, arg0->parent);
            camInverseTransformPointByObject(arg0->prevGlobalPosition.x, arg0->prevGlobalPosition.y, arg0->prevGlobalPosition.z, &arg0->prevLocalPosition.x, &arg0->prevLocalPosition.y, &arg0->prevLocalPosition.z, arg0->parent);
            camInverseRotatePointByObject(speedX, 0, speedZ, &arg0->velocity.x, &speedY, &arg0->velocity.z, arg0->parent);
            dYaw -= arg0->parent->srt.yaw;
            CIRCLE_WRAP(dYaw)
            arg0->srt.yaw = dYaw;
        } else {
            arg0->srt.transl.x = arg0->globalPosition.x;
            arg0->srt.transl.y = arg0->globalPosition.y;
            arg0->srt.transl.z = arg0->globalPosition.z;
            arg0->prevLocalPosition.x = arg0->prevGlobalPosition.x;
            arg0->prevLocalPosition.y = arg0->prevGlobalPosition.y;
            arg0->prevLocalPosition.z = arg0->prevGlobalPosition.z;
            arg0->velocity.x = speedX;
            arg0->velocity.z = speedZ;
            arg0->srt.yaw = dYaw;
        }
    }

    if (hitInfo != NULL) {
        hitInfo->unk10.x = arg0->srt.transl.x;
        hitInfo->unk10.y = arg0->srt.transl.y;
        hitInfo->unk10.z = arg0->srt.transl.z;
        hitInfo->unk20.x = arg0->globalPosition.x;
        hitInfo->unk20.y = arg0->globalPosition.y;
        hitInfo->unk20.z = arg0->globalPosition.z;
    }

    if (arg0->controlNo == OBJCONTROL_Player) {
        ((DLL_27*)gDLL_27)->vtbl->reset(arg0, ((DLL_210_Player*)arg0->dll)->vtbl->func57(arg0));
    }
}
