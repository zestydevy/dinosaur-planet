#include "common.h"
#include "sys/newshadows.h"

static const char str_8009aa70[] = "Sorry Background Block list has been exceeded\n";
static const char str_8009aaa0[] = "1: track/intersect.c: OVERFLOW error\n";
static const char str_8009aac8[] = "2: track/intersect.c: OVERFLOW error\n";
static const char str_8009aaf0[] = "3: track/intersect.c: OVERFLOW error\n";
static const char str_8009ab18[] = "TrackGetHeight()-Overflow!!!\n";
static const char str_8009ab38[] = "trackIntersect: FUNC OVERFLOW %d\n";
static const char str_8009ab5c[] = "intersectModLineBuild point list overflow, %d/%d\n";
static const char str_8009ab90[] = "trackIntersect: FUNC OVERFLOW %d\n";
static const char str_8009abb4[] = "insertPoint array overrun %d/%d\n";
static const char str_8009abd8[] = "NO FREE LAST LINE\n";

typedef struct {
/*00*/ s16 unk0;    
/*02*/ s16 unk2;    
/*04*/ s16 unk4;    
/*06*/ s16 unk6;    
/*08*/ s16 unk8;    
/*0A*/ s16 unkA;    
/*0C*/ s16 unkC;    
/*0E*/ s16 unkE;    
/*10*/ s16 unk10;    
/*12*/ s16 unk12;    
/*14*/ u8 unk14;    
/*15*/ s8 unk15;    
/*16*/ s16 unk16;    
} HitsUnk;

/** HitsLines seem to end up reencoded into this format
  * The point data seems to be missing and index values are introduced,
  * so maybe they're combining coincident points across different lines,
  * and referencing those verts by index?
*/
typedef struct{
/*00*/    s8 heightA;
/*01*/    s8 heightB;
/*02*/    s8 settingsA;
/*03*/    s8 settingsB;  
/*04*/    s16 indexA; 
/*06*/    s16 indexB; 
/*08*/    s16 indexC; 
/*0a*/    s16 indexD;
/*0c*/    s16 animatorID;
/*0e*/    s16 unkE;
} HitsLineReencoded; //0x10

typedef struct {
    s16 unk0;
    u8 _unk2[14];
} D_800BB26C_Struct;

s32 func_80055458(Object*, UnkFunc80051D68Arg3 *, UnkFunc80051D68Arg3 *, f32*, f32 *, s32, s8*, s32);
s32 func_800564C8(UnkFunc80051D68Arg3 *, UnkFunc80051D68Arg3 *, Vec3f *, s32, Unk80027934*, u8);
void func_80058F8C(void);
s32 func_80056BCC(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec4f* arg3, Vec3f* arg4, f32 arg5);

UnkFunc80051D68Arg3* func_80053B24(UnkFunc80051D68Arg3*, s32, s32, s32, s32, s32, s32, u8);
UnkFunc80051D68Arg3* func_8005471C(UnkFunc80051D68Arg3*, Unk8005341C*, ModelInstance*, f32, f32, f32, f32, f32, f32, u8);
void func_80054DF8(UnkFunc80051D68Arg3*, UnkFunc80051D68Arg3*, u8);

// move to objhits.h
Object **func_80025DD4(s32 *arg0);

// .bss 800bb200-800bb540
Vec3s32 D_800BB200[8];
UnkFunc80051D68Arg3* D_800BB260;
s16 D_800BB264;
Unk8005341C D_800BB268[2]; // unknown size
u8 _bss_800BB288[0x800BB3A8 - 0x800BB288];
u8 D_800BB3A8; // count for D_800BB268
u8 _bss_800BB3A9[0x800BB4D6 - 0x800BB3A9];
s16 D_800BB4D6;
s16 D_800BB4D8;
u8 _bss_800BB4DA[0x800BB538 - 0x800BB4DA];
s8 D_800BB538;
s8 D_800BB539;

// .data 80092E70-80092E90
UnkFunc80051D68Arg3 *D_80092E70 = NULL; // 250 length
HitsLineReencoded* D_80092E74 = NULL;
void *D_80092E78 = NULL;
void *D_80092E7C = NULL;
s8 D_80092E80 = 0;
HitsUnk* D_80092E84 = NULL;

#define SOME_MIN 100000
// Idk where this random 34 is coming from, hex value is 0xFFFE7960
#define SOME_MAX (2 ^ 32) - SOME_MIN - 34

void func_80053300(void) {
    if (D_80092E70 == NULL) {
        D_80092E70 = mmAlloc(250*sizeof(UnkFunc80051D68Arg3), COLOUR_TAG_YELLOW, 0);
        D_80092E74 = mmAlloc(400*sizeof(HitsLineReencoded), COLOUR_TAG_YELLOW, 0);
        D_80092E78 = mmAlloc(4800, COLOUR_TAG_YELLOW, 0);
        D_80092E7C = mmAlloc(800, COLOUR_TAG_YELLOW, 0);
        D_80092E84 = mmAlloc(20*sizeof(HitsUnk), COLOUR_TAG_YELLOW, 0);
    }
    func_80058F8C();
    D_800BB4D6 = 0;
    D_800BB4D8 = 0;
    D_800BB539 = 0;
    D_800BB538 = 0;
}

void func_800533D8(s32* arg0, UnkFunc80051D68Arg3** arg1) {
    *arg0 = D_800BB268[D_800BB3A8].unk4;
    *arg1 = D_80092E70;
}

void func_80053408(Vec3s32** arg0) {
    *arg0 = D_800BB200;
}

Unk8005341C* func_8005341C(s32* arg0) {
    *arg0 = D_800BB3A8;
    return D_800BB268;
}

void fit_aabb_around_cubes(AABBs32 *aabb, Vec3f *posArray1, Vec3f *posArray2, f32 *cubeRadiusArray, s32 arrayLength) {
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

void func_80053750(Object* arg0, AABBs32* arg1, u8 arg2) {
    UnkFunc80051D68Arg3* s2;
    Model* temp_a0_2;
    ModelInstance* temp_a2;
    Object* curObj;
    Object** objects;
    ObjectHitInfo* temp_a0;
    ObjectStruct58* temp_v1_2;
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

    temp_fs0 = arg1->min.x - 5;
    temp_fs1 = arg1->max.x + 5;
    temp_fs2 = arg1->min.y - 5;
    temp_fs3 = arg1->max.y + 5;
    temp_fs4 = arg1->min.z - 5;
    temp_fs5 = arg1->max.z + 5;
    var_s0 = &D_800BB268[1];
    D_800BB268->unk0 = 0;
    D_800BB268->unk4 = 0;
    D_800BB260 = D_80092E70 + 250;
    if (!(arg2 & 0x10)) {
        s2 = func_80053B24(D_80092E70, temp_fs0, temp_fs2, temp_fs4, temp_fs1, temp_fs3, temp_fs5, arg2);
    } else {
        s2 = D_80092E70;
    }
    if ((arg2 & 1) && arg0 != NULL) {
        spB8 = s2;
        objects = func_80025DD4(&objCount);
        for (i = 0; i < objCount; i++) {
            curObj = objects[i];
            if (curObj->objhitInfo != NULL && curObj->unk58 != NULL && curObj->unk58->unk10D == 0 && curObj->unk58->unk10E == 0) {
                temp_a2 = curObj->modelInsts[curObj->objhitInfo->unkA0];
                if (temp_a2 != NULL && temp_a2->model->facebatchBounds != NULL) {
                    temp_fv0 = temp_a2->model->maxAnimatedVertDistance;
                    if (
                        !((temp_fv1 = curObj->srt.transl.x - temp_fv0) > temp_fs1) && !((temp_fv1 = curObj->srt.transl.x + temp_fv0) < temp_fs0)
                        && !((temp_fv1_2 = curObj->srt.transl.y - temp_fv0) > temp_fs3) && !((temp_fv1_2 = curObj->srt.transl.y + temp_fv0) < temp_fs2)
                        && !((temp_fv1_3 = curObj->srt.transl.z - temp_fv0) > temp_fs5) && !((temp_fv1_3 = curObj->srt.transl.z + temp_fv0) < temp_fs4)
                    ) {
                            var_s0->unkC = (MtxF* ) &((f32*)curObj->unk58)[(curObj->unk58->unk10C + 2) << 4];
                            var_s0->unk8 = (MtxF* ) &((f32*)curObj->unk58)[curObj->unk58->unk10C << 4];
                            var_s0->unk4 = ((s32)s2 - (s32)D_80092E70) / 52;
                            var_s0->unk0 = curObj;
                            s2 = func_8005471C(s2, var_s0, temp_a2, temp_fs0, temp_fs2, temp_fs4, temp_fs1, temp_fs3, temp_fs5, arg2);
                            var_s0 += 1;
                    }
                }
            }
        }
        func_80054DF8(spB8, s2, arg2);
    }
    D_800BB264 = ((s32)s2 - (s32)D_80092E70) / 52;
    D_800BB3A8 = var_s0 - D_800BB268;
    var_s0->unk4 = D_800BB264;
}

#ifndef NON_EQUIVALENT
UnkFunc80051D68Arg3* func_80053B24(UnkFunc80051D68Arg3* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80053B24.s")
#else
// https://decomp.me/scratch/s5P5f
UnkFunc80051D68Arg3* func_80053B24(UnkFunc80051D68Arg3* arg0, s32 arg1, s32 upperY, s32 arg3, s32 arg4, s32 lowerY, s32 arg6, u8 arg7) {
    BlocksModel* temp_s0;
    BlocksModel* temp_v0;
    FaceBatch* temp_t9;
    FaceBatch* var_s3;
    BlocksModel *sp138[6]; // unknown size, max 10
    f32 temp_fs1;
    Vtx_t* sp128[3];
    Vtx_t *sp124;
    s16 sp122;
    s32 sp11C;
    s32 sp118;
    f32 temp_fs0;
    f32 temp_fs2;
    s32 sp10C;
    s32 sp108;
    s32 lowerX;
    s32 upperX;
    s32 lowerZ;
    s32 upperZ;
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
    s32 pad;
    u32 temp_s3;
    u32 temp_s4;
    u32 temp_s5;
    u32 xDiff;
    u32 temp_s7;
    s32 temp_t9_4;
    s32 var_t0;
    u8 var_v0_3;
    u8 spA6;
    u8 lowestYIndex;
    u8 highestYIndex;
    s32 temp;
    FaceBatch* sp9C;

    arg1 -= D_80092A60;
    arg4 -= D_80092A60;
    arg3 -= D_80092A64;
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
    temp_s4 = floor_f(arg1 / BLOCKS_GRID_UNIT_F);
    temp_s7 = floor_f(arg3 / BLOCKS_GRID_UNIT_F);
    temp_s5 = floor_f(arg4 / BLOCKS_GRID_UNIT_F);
    temp_s3 = floor_f(arg6 / BLOCKS_GRID_UNIT_F);
    sp108 = 0;
    for (sp118 = 0; sp118 < 5; sp118++) {
        temp_v0 = func_80044B18(temp_s4, temp_s7, sp118);
        if (temp_v0 != NULL) {
            sp138[sp108] = temp_v0;
            D_800BB200[sp108].x = temp_s4 * BLOCKS_GRID_UNIT;
            D_800BB200[sp108].z = temp_s7 * BLOCKS_GRID_UNIT;
            sp108++;
        }
        if (temp_s5 != temp_s4) {
            temp_v0 = func_80044B18(temp_s5, temp_s7, sp118);
            if (temp_v0 != NULL) {
                sp138[sp108] = temp_v0;
                D_800BB200[sp108].x = temp_s5 * BLOCKS_GRID_UNIT;
                D_800BB200[sp108].z = temp_s7 * BLOCKS_GRID_UNIT;
                sp108++;
            }
        }
        if (temp_s3 != temp_s7) {
            temp_v0 = func_80044B18(temp_s4, temp_s3, sp118);
            if (temp_v0 != NULL) {
                sp138[sp108] = temp_v0;
                D_800BB200[sp108].x = temp_s4 * BLOCKS_GRID_UNIT;
                D_800BB200[sp108].z = temp_s3 * BLOCKS_GRID_UNIT;
                sp108++;
            }
            if (temp_s5 != temp_s4) {
                temp_v0 = func_80044B18(temp_s5, temp_s3, sp118);
                if (temp_v0 != NULL) {
                    sp138[sp108] = temp_v0;
                    D_800BB200[sp108].x = temp_s5 * BLOCKS_GRID_UNIT;
                    D_800BB200[sp108].z = temp_s3 * BLOCKS_GRID_UNIT;
                    sp108++;
                }
            }
        }
    }

    for (sp118 = 0; sp118 < sp108; sp118++) {
        lowerX = arg1 - D_800BB200[sp118].x;
        upperX = arg4 - D_800BB200[sp118].x;
        lowerZ = arg3 - D_800BB200[sp118].z;
        upperZ = arg6 - D_800BB200[sp118].z;
        D_800BB200[sp118].x += D_80092A60;
        D_800BB200[sp118].z += D_80092A64;
        // lowerX = MAX(0, lowerX);
        if (lowerX < 0) {
            lowerX = 0;
        }
        // upperX = MIN(BLOCKS_GRID_UNIT, upperX);
        if (upperX > BLOCKS_GRID_UNIT) {
            upperX = BLOCKS_GRID_UNIT;
        }
        // lowerZ = MAX(0, lowerZ);
        if (lowerZ < 0) {
            lowerZ = 0;
        }
        // upperZ = MIN(BLOCKS_GRID_UNIT, upperZ);
        if (upperZ > BLOCKS_GRID_UNIT) {
            upperZ = BLOCKS_GRID_UNIT;
        }
        xDiff = D_800BB200->x - D_800BB200[sp118].x;
        temp_s7 = D_800BB200->z - D_800BB200[sp118].z;
        temp_s0 = sp138[sp118];
        var_v0 = 1;
        sp122 = 0;
        for (var_t0 = 0; var_t0 < BLOCKS_GRID_UNIT; var_t0 += 80) {
            if ((var_t0 + 80) >= lowerX && upperX >= var_t0) {
                sp122 |= var_v0;
            }
            var_v0 <<= 1;
        }
        for (var_t0 = 0; var_t0 < BLOCKS_GRID_UNIT; var_t0 += 80) {
            if (var_t0 >= (lowerZ - 80) && upperZ >= var_t0) {
                sp122 |= var_v0;
            }
            var_v0 <<= 1;
        }
        sp9C = &temp_s0->ptr_faceBatches[temp_s0->faceBatch_count];
        for (var_s3 = temp_s0->ptr_faceBatches; var_s3 < sp9C; var_s3++) {
            if (var_s3->renderSettingBitfield & 0x2000) {
                if (!(var_s3->renderSettingBitfield & 0x300000)) {
                    spA6 = 4;
                    if (!(arg7 & 0x20)) {
                        spA6 |= 0x10;
                    }
                    goto block_55;
                }
            } else if (!(var_s3->renderSettingBitfield & 0x800) || (arg7 & 0x20)) {
                spA6 = 2;
block_55:
                if (
                    lowerY >= var_s3->Ymin &&
                    var_s3->Ymax >= upperY &&
                    (upperX >> 2) >= var_s3->Xmin &&
                    var_s3->Xmax >= (lowerX >> 2) &&
                    (upperZ >> 2) >= var_s3->Zmin &&
                    var_s3->Zmax >= (lowerZ >> 2)
                ) {
                    if (var_s3->renderSettingBitfield & 0x1000) {
                        spA6 |= 8;
                    }
                    sp124 = &temp_s0->unk20[temp_s0->flags & 1][var_s3->baseVertexID];
                    sp11C = var_s3->baseFaceID;
                    sp10C = var_s3[1].baseFaceID;
                    for (; sp11C < sp10C; sp11C++) {
                        if ((!temp_s0) && (!temp_s0)) {}
                        var_t0 = temp_s0->unk14[sp11C] & sp122;
                        if ((var_t0 & 0xFF) && (var_t0 & 0xFF00)) {
                            minX = SOME_MIN;
                            maxX = SOME_MAX;
                            minY = SOME_MIN;
                            maxY = SOME_MAX;
                            minZ = SOME_MIN;
                            maxZ = SOME_MAX;
                            sp128[0] = &sp124[(temp_s0->ptr_faces[sp11C].unk0 >> 13) & 0x1F];
                            sp128[1] = &sp124[(temp_s0->ptr_faces[sp11C].unk0 >> 7) & 0x1F];
                            sp128[2] = &sp124[(temp_s0->ptr_faces[sp11C].unk0 >> 1) & 0x1F];
                            for (var_t0 = 0; var_t0 < 3; var_t0++) {
                                var_a0_3 = sp128[var_t0]->ob[0];
                                var_v1_2 = sp128[var_t0]->ob[1];
                                var_a1_3 = sp128[var_t0]->ob[2];
                                if (var_s3->renderSettingBitfield & 0x20000000) {
                                    var_a0_3 *= 1.0f; // not necessary, forces a float cast tho
                                    var_v1_2 *= 0.05f;
                                    var_a1_3 *= 1.0f; // not necessary, forces a float cast tho
                                    var_v1_2 += temp_s0->minY;
                                }
                                if (maxX < var_a0_3) {
                                    maxX = var_a0_3;
                                }
                                if (var_a0_3 < minX) {
                                    minX = var_a0_3;
                                }
                                if (maxY < var_v1_2) {
                                    maxY = var_v1_2;
                                    highestYIndex = var_t0;
                                }
                                if (var_v1_2 < minY) {
                                    minY = var_v1_2;
                                    lowestYIndex = var_t0;
                                }
                                if (maxZ < var_a1_3) {
                                    maxZ = var_a1_3;
                                }
                                if (var_a1_3 < minZ) {
                                    minZ = var_a1_3;
                                }
                                // @fake
                                if ((!temp_s7) && (!temp_s7)) {}
                                arg0->unkA[var_t0] = var_a0_3 + xDiff;
                                arg0->unk10[var_t0] = var_v1_2;
                                arg0->unk16[var_t0] = var_a1_3 + temp_s7;
                            }
                            if (
                                lowerY >= minY &&
                                maxY >= upperY &&
                                upperX >= minX &&
                                maxX >= lowerX &&
                                upperZ >= minZ &&
                                maxZ >= lowerZ
                            ) {
                                if (var_s3->animatorID != 0) {
                                    temp_fs0 = (arg0->unk10[0] * (arg0->unk16[1] - arg0->unk16[2])) + (arg0->unk10[1] * (arg0->unk16[2] - arg0->unk16[0])) + (arg0->unk10[2] * (arg0->unk16[0] - arg0->unk16[1]));
                                    temp_fs1 = (arg0->unk16[0] * (arg0->unkA[1] - arg0->unkA[2])) + (arg0->unk16[1] * (arg0->unkA[2] - arg0->unkA[0])) + (arg0->unk16[2] * (arg0->unkA[0] - arg0->unkA[1]));
                                    temp_fs2 = (arg0->unkA[0] * (arg0->unk10[1] - arg0->unk10[2])) + (arg0->unkA[1] * (arg0->unk10[2] - arg0->unk10[0])) + (arg0->unkA[2] * (arg0->unk10[0] - arg0->unk10[1]));
                                    temp_fv0 = sqrtf(SQ(temp_fs0) + SQ(temp_fs1) + SQ(temp_fs2));
                                    if (temp_fv0 > 0.0f) {
                                        temp_fv1 = 8191.0f / temp_fv0;
                                        arg0->unk4 = temp_fs0 * temp_fv1;
                                        arg0->unk6 = temp_fs1 * temp_fv1;
                                        arg0->unk8 = temp_fs2 * temp_fv1;
                                    }
                                } else {
                                    arg0->unk4 = temp_s0->ptr_faces[sp11C].unk0 >> 0x12;
                                    arg0->unk6 = (temp_s0->ptr_faces[sp11C].unk4 << 0xE) >> 0x12;
                                    arg0->unk8 = temp_s0->ptr_faces[sp11C].unk4 >> 0x12;
                                }
                                if (((arg7 & 8) == 0 || !(arg0->unk6 >= 5791.037f)) && ((arg7 & 4) == 0 || !(arg0->unk6 < 5791.037f))) {
                                    arg0->unk0 = (f32) -((arg0->unk6 * arg0->unk10[0]) + ((arg0->unk4 * arg0->unkA[0]) + (arg0->unk16[0] * arg0->unk8))) * 0.00012208521f;
                                    temp = (sp11C) * 9;
                                    for (temp_t9_4 = temp; temp_t9_4 < (temp + 9); temp_t9_4++) {
                                        arg0->unk1C[temp_t9_4 - temp] = temp_s0->ptr_faceEdgeVectors[temp_t9_4];
                                    }
                                    if (var_s3->renderSettingBitfield & 0x2000) {
                                        var_v0_3 = 0xE;
                                    } else {
                                        if (var_s3->materialID == 0xFF) {
                                            var_v0_3 = 0;
                                        } else {
                                            var_v0_3 = temp_s0->ptr_materials[var_s3->materialID].terrain_type;
                                        }
                                    }
                                    arg0->unk2E = var_v0_3;
                                    arg0->unk30 = (highestYIndex * 0x10) | lowestYIndex;
                                    arg0->unk2F = (temp_s0->ptr_faces[sp11C].unk4 & 1) | spA6;
                                    arg0++;
                                }
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

#ifndef NON_MATCHING
UnkFunc80051D68Arg3* func_8005471C(UnkFunc80051D68Arg3* arg0, Unk8005341C* arg1, ModelInstance* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, u8 arg9);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005471C.s")
#else
// https://decomp.me/scratch/Nabui
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
    f32 xBounds[4]; // sp98
    f32 zBounds[4]; // sp88
    f32 sp84;
    s32 pad;
    Model* sp7C;

    sp7C = arg2->model;

    vec3_transform(arg1->unk8, upperX, upperY, upperZ, &xBounds[0], &sp84, &zBounds[0]);
    vec3_transform(arg1->unk8, upperX, upperY, lowerZ, &xBounds[1], &upperY, &zBounds[1]);
    vec3_transform(arg1->unk8, lowerX, lowerY, upperZ, &xBounds[2], &sp84, &zBounds[2]);
    vec3_transform(arg1->unk8, lowerX, lowerY, lowerZ, &xBounds[3], &lowerY, &zBounds[3]);

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
            (!(sp7C->faces[spB4].tagC & 0x800) || (arg9 & 0x20)) &&
            !(sp7C->facebatchBounds[(0, spB4)].minX < upperX) &&
            !(lowerX < sp7C->facebatchBounds[(0, spB4)].maxX) &&
            !(sp7C->facebatchBounds[(0, spB4)].minY < upperY) &&
            !(lowerY < sp7C->facebatchBounds[(0, spB4)].maxY) &&
            !(sp7C->facebatchBounds[(0, spB4)].minZ < upperZ) &&
            !(lowerZ < sp7C->facebatchBounds[(0, spB4)].maxZ)
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
                    arg0->unkA[i] = currVtx->v.ob[0];
                    arg0->unk10[i] = currVtx->v.ob[1];
                    arg0->unk16[i] = currVtx->v.ob[2];
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
                        arg0->unk30 = 0;
                    } else {
                        arg0->unk30 = sp7C->materials[sp7C->faces[spB4].materialID].unk7;
                    }
                    arg0->unk32 = (highestYIndex * 0x10) | lowestYIndex;
                    arg0->unk31 = 1;
                    arg0 += 1;
                }
            }
        }
    }
    return arg0;
}
#endif

void func_80054DF8(UnkFunc80051D68Arg3* arg0, UnkFunc80051D68Arg3* arg1, u8 arg2) {
    f32 temp_fv0;
    f32 temp_fv1;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fs2;
    s16* temp_s1;
    s16* temp_s2;
    s16* temp_s3;

    while (arg0 < arg1) {
        temp_fs1 = (arg0->unk10[0] * (arg0->unk16[1] - arg0->unk16[2])) + (arg0->unk10[1] * (arg0->unk16[2] - arg0->unk16[0])) + (arg0->unk10[2] * (arg0->unk16[0] - arg0->unk16[1]));
        temp_fs0 = (arg0->unk16[0] * (arg0->unkA[1] - arg0->unkA[2])) + (arg0->unk16[1] * (arg0->unkA[2] - arg0->unkA[0])) + (arg0->unk16[2] * (arg0->unkA[0] - arg0->unkA[1]));
        temp_fs2 = (arg0->unkA[0] * (arg0->unk10[1] - arg0->unk10[2])) + (arg0->unkA[1] * (arg0->unk10[2] - arg0->unk10[0])) + (arg0->unkA[2] * (arg0->unk10[0] - arg0->unk10[1]));
        temp_s1 = arg0->unkA;
        temp_s2 = arg0->unk10;
        temp_s3 = arg0->unk16;
        temp_fv0 = sqrtf(SQ(temp_fs1) + SQ(temp_fs0) + SQ(temp_fs2));
        if (temp_fv0 > 0.0f) {
            temp_fv1 = 1.0f / temp_fv0;
            temp_fs1 *= temp_fv1;
            temp_fs0 *= temp_fv1;
            temp_fs2 *= temp_fv1;
        }
        if ((arg2 & 8) && (temp_fs0 >= 0.707f)) {
            arg0->unk31 |= 0x10;
        }
        if ((arg2 & 4) && (temp_fs0 < 0.707f)) {
            arg0->unk31 |= 0x10;
        }
        arg0->unk4 = temp_fs1 * 8191.0f;
        arg0->unk6 = temp_fs0 * 8191.0f;
        arg0->unk8 = temp_fs2 * 8191.0f;
        arg0->unk0 = -((*temp_s1 * temp_fs1) + (*temp_s2 * temp_fs0) + (*temp_s3 * temp_fs2));
        arg0++;
    }
}

u8 func_8005509C(Object *arg0, f32* arg1, f32* arg2, s32 arg3, Unk80027934* arg4, u8 arg5) {
    Unk8005341C* var_s3;
    Unk8005341C* target;
    f32 spA0[4 * 3];
    f32 sp70[4 * 3];
    u8 sp6F;
    s16 var_s1;
    s16 var_s2;
    u8 temp_v0_3;
    u8 var_s5;
    Object* sp64;

    sp6F = 0;
    arg4->unk68 = 0;
    var_s3 = D_800BB268;
    target = &D_800BB268[D_800BB3A8];
    if ((u32) target >= (u32) ((u8*)&D_800BB268 + 1)) {
        do {
            sp64 = var_s3->unk0;
            if (sp64 != NULL) {
                for (var_s1 = 0, var_s2 = 0; var_s2 < arg3;) {
                    vec3_transform(
                        var_s3->unk8,
                        arg1[var_s1 + 0], arg1[var_s1 + 1], arg1[var_s1 + 2],
                        &sp70[var_s1 + 0], &sp70[var_s1 + 1], &sp70[var_s1 + 2]
                    );
                    vec3_transform(
                        var_s3->unk8,
                        arg2[var_s1 + 0], arg2[var_s1 + 1], arg2[var_s1 + 2],
                        &spA0[var_s1 + 0], &spA0[var_s1 + 1], &spA0[var_s1 + 2]
                    );
                    var_s1 += 3;\
                    var_s2++;
                }
                if (arg5 & 2) {
                    sp6F |= func_800564C8(&D_80092E70[var_s3->unk4], &D_80092E70[var_s3[1].unk4], &spA0[0], arg3, arg4, 1);
                } else {
                    temp_v0_3 = func_80055458(sp64, &D_80092E70[var_s3->unk4], &D_80092E70[var_s3[1].unk4], &sp70[0], &spA0[0], arg3, (s8* ) arg4, 1);
                    if (temp_v0_3) {
                        func_80026184(sp64, arg0);
                        sp6F |= temp_v0_3;
                    }
                }
                for (var_s1 = 0, var_s2 = 0, var_s5 = 1; var_s2 < arg3; var_s2++) {
                    if (sp6F & var_s5) {
                        vec3_transform(
                            var_s3->unkC,
                            spA0[var_s1 + 0], spA0[var_s1 + 1], spA0[var_s1 + 2],
                            &arg2[var_s1 + 0], &arg2[var_s1 + 1], &arg2[var_s1 + 2]
                        );
                    }
                    var_s5 <<= 1;
                    var_s1 += 3;
                }
            } else if (arg5 & 2) {
                sp6F |= func_800564C8(&D_80092E70[var_s3->unk4], &D_80092E70[var_s3[1].unk4], arg2, arg3, arg4, 0);
            } else {
                sp6F |= func_80055458(sp64, &D_80092E70[var_s3->unk4], &D_80092E70[var_s3[1].unk4], arg1, arg2, arg3, (s8* ) arg4, 0);
            }
            var_s3 += 1;
        } while ((u32) var_s3 < (u32) target);
    }
    return sp6F;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80055458.s")

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800564C8.s")
#else
// https://decomp.me/scratch/iUTQF
s32 func_800564C8(UnkFunc80051D68Arg3* arg0, UnkFunc80051D68Arg3* arg1, Vec3f* arg2, s32 arg3, Unk80027934* arg4, u8 arg5) {
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fs4;
    f32 temp_ft3;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv1;
    s32 var_v0;
    f32 sp7C;
    f32 sp78;
    f32 var_fs1;
    f32 var_fs2;
    s32 var_a0;
    s32 var_a1;
    f32 sp64;
    f32 sp60;
    s32 var_v1;
    s32 var_v1_2;
    s32 i;
    UnkFunc80051D68Arg3 *temp;

    var_a0 = 0;
    var_a1 = 1;
    if (arg5 != 0) {
        sp64 = 0;
        sp60 = 0;
    } else {
        sp64 = D_800BB200->x;
        sp60 = D_800BB200->z;
    }
    i = 0;
    do {
        temp_fs4 = arg2[i].f[1];
        var_fs1 = arg2[i].f[0];
        var_fs2 = arg2[i].f[2];
        sp7C = arg4->unk40[i];
        sp78 = sp7C + 0.1f;
        var_fs1 -= sp64;
        var_fs2 -= sp60;
        var_v0 = 0;
        do {
            var_v1 = FALSE;
            temp = arg0;
            for (; (u32) temp < (u32) arg1; temp++) {
                temp_fa1 = temp->unk4 * 0.00012208521f;
                temp_fa0 = temp->unk6 * 0.00012208521f;
                temp_ft4 = temp->unk8 * 0.00012208521f;
                temp_ft5 = temp->unk0;
                if (temp_fa0 >= 0.707f) {
                    continue;
                }

                temp_fv1 = ((temp_fa1 * var_fs1) + (temp_fa0 * temp_fs4) + (temp_ft4 * var_fs2) + temp_ft5) - sp7C;
                if ((2.0f * -sp7C) <= temp_fv1 && temp_fv1 <= 0.0f) {
                    if (temp_fs4 < (temp->unk10[temp->unk32 & 0xF] - sp78)) {
                        continue;
                    }

                    if ((temp->unk10[temp->unk32 >> 4] + sp78) < temp_fs4) {
                        continue;
                    }

                    var_v1_2 = 0;
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
                    arg2[i].f[0] = var_fs1 + sp64;
                    arg2[i].f[2] = var_fs2 + sp60;
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
    } while (i < arg3);

    return var_a0;
}
#endif

s32 func_800567F4(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, f32 arg3, Vec3f* arg4, Vec4f* arg5) {
    Vec3f sp64;
    Vec3f sp58;
    Vec3f sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;

    vec3_cross_product(arg2, &arg0[2], &sp64);
    sp44 = vec3_normalize(&sp64);
    if (sp44 == 0.0f) {
        return 0;
    }
    VECTOR_SUBTRACT((*arg1), (*arg0), sp58);
    sp48 = DOT_PRODUCT(sp64, sp58);
    sp48 = SQ(sp48);
    if (sp48 <= arg0[3].f[1]) {
        vec3_cross_product(&sp58, &arg0[2], &sp4C);
        sp40 = -DOT_PRODUCT(sp4C, sp64) / sp44;
        vec3_cross_product(&sp64, &arg0[2], &sp4C);
        vec3_normalize(&sp4C);
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
                vec3_normalize((Vec3f*)arg5);
                arg5->f[3] = -DOT_PRODUCT((*arg4), (*arg5));
                return 3;
            }
            if (sp40 < 0.0f) {
                if (func_80056BCC(arg1, arg2, arg4, arg5, arg0, arg0[3].f[1]) != 0) { return 1; }
            } else if (func_80056BCC(arg1, arg2, arg4, arg5, &arg0[1], arg0[3].f[1]) != 0) {
                return 2;
            }
        }
    }
    return 0;
}

s32 func_80056BCC(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec4f* arg3, Vec3f* arg4, f32 arg5) {
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
            vec3_normalize((Vec3f* ) arg3);
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
        vec3_normalize(&sp58);
        // @fake
        if (arg4) {}
        for (i = 0; i < 0x65 && (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4))) < arg5; i++) {
            arg2->f[0] -= sp58.f[0] * 0.1f;
            arg2->f[1] -= sp58.f[1] * 0.1f;
            arg2->f[2] -= sp58.f[2] * 0.1f;
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
            for (i = 0; i < 0x65 && (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4))) < arg5; i++) {
                arg2->f[0] -= sp58.f[0] * 0.1f;
                arg2->f[2] -= sp58.f[2] * 0.1f;
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
            vec3_normalize(&sp58);
            sp58.f[0] *= arg5;
            sp58.f[1] *= arg5;
            sp58.f[2] *= arg5;
            VECTOR_ADD(sp64, sp58, (*arg2));
        }
    } else if (arg6 != 2) {
        arg2->f[1] = arg3->f[1];
        sp54 = arg5 - (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4)));
        if (sp54 > 0/*.0f*/) {
            sp54 = sp54 / fsin16_precise(arctan2_f(arg4->f[1], sqrtf(SQ(arg4->f[2]) + SQ(arg4->f[0]))));
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
        vec3_normalize(&sp3C);
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
                for (i = 0; i < 0x65 && (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4))) < arg5; i++) {
                    arg2->f[0] -= sp3C.f[0] * 0.1f;
                    arg2->f[2] -= sp3C.f[2] * 0.1f;
                }
                break;
            case 1:
                arg2->f[0] -= arg0 * arg4->f[0];
                arg2->f[2] -= arg0 * arg4->f[2];
                sp34 = arg5 - (arg4->f[3] + DOT_PRODUCT((*arg2), (*arg4)));
                if (sp34 > 0.0f) {
                    i = arctan2_f(arg4->f[1], sqrtf(SQ(arg4->x) + SQ(arg4->z)));
                    temp_fv0 = fcos16_precise(i);
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
                i = arctan2_f(arg4->f[1], sqrtf(SQ(arg4->x) + SQ(arg4->z)));
                sp34 = sp34 / fsin16_precise(i);
                arg2->f[1] += sp34 * arg4->f[1];
            }
        }
    }
    return 1;
}

s32 func_80057A30(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, Vec4f* arg4, Vec4f* arg5, f32 arg6, u8 arg7) {
    Vec3f sp3C;
    f32 sp38;
    s16 i;

    if ((arg7 == 3) || (arg7 == 6)) {
        arg3->f[0] = arg4->f[0];
        arg3->f[1] = arg4->f[1];
        arg3->f[2] = arg4->f[2];
        VECTOR_SUBTRACT((*arg3), (*arg2), sp3C);
        vec3_normalize(&sp3C);
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
            for (i = 0; i < 0x65 && (arg5->f[3] + DOT_PRODUCT((*arg3), (*arg5))) < arg6; i++) {
                arg3->f[0] -= sp3C.f[0] * 0.1f;
                arg3->f[2] -= sp3C.f[2] * 0.1f;
            }
        } else {
            sp38 += sp38;
            arg3->f[0] -= (sp38 * arg5->f[0]);
            arg3->f[1] -= (sp38 * arg5->f[1]);
            arg3->f[2] -= (sp38 * arg5->f[2]);
            VECTOR_SUBTRACT((*arg3), (*arg0), sp3C);
            vec3_normalize(&sp3C);
            sp3C.f[0] *= arg6;
            sp3C.f[1] *= arg6;
            sp3C.f[2] *= arg6;
            VECTOR_ADD((*arg0), sp3C, (*arg3));
        }
    } else if (arg7 != 2) {
        arg3->f[1] = arg4->f[1];
        sp38 = arg6 - (arg5->f[3] + DOT_PRODUCT((*arg3), (*arg5)));
        if (sp38 > 0.0f) {
            i = arctan2_f(arg5->f[1], sqrtf(SQ(arg5->x) + SQ(arg5->z)));
            sp38 = sp38 / fsin16_precise(i);
            arg3->f[1] += sp38 * arg5->f[1];
        }
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80057F1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058144.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058680.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800588D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058B1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058D54.s")

void func_80058F3C(void) {
    D_800BB539 = 1;
}

int func_80058F50(void) {
    return (D_800BB539 != 0) || (D_800BB538 != 0);
}

s8 func_80058F7C(void) {
    return D_80092E80;
}

void func_80058F8C(void) {
    s32 index;
    HitsUnk* temp;

    for (index = 0; index < 20; index++){ 
        temp = &D_80092E84[index];
        temp->unk14 = 0;
    }
}

void func_80058FE8(void) {
    s16 index;
    HitsUnk *temp;

    index = 0;
    while (index < 20){
        temp = &D_80092E84[index];
        if (temp->unk14){
            temp->unk14--;
        }
        index++;
    }
}

/** Used by HitAnimators to toggle HITS lines */
void func_80059038(s32 animatorID, Object* parentObject, s32 enableLines) {
    s16 lineCount;
    s32 index;
    ObjDef *objDef;
    HitsLineReencoded *hitsLines;

    hitsLines = D_80092E74;

    if (parentObject != NULL){
        objDef = parentObject->def;
        hitsLines = objDef->pIntersectPoints; //pointer to mobile map's encoded HITS lines? (MODLINES.bin)
        lineCount = objDef->modLineCount;
    } else {    
        hitsLines = D_80092E74;
        lineCount = D_800BB4D6;
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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800591EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800596BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80059C40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005A2BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005A3F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005B17C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005B204.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005B274.s")

//parent_object_to_mobile_map_object?
void func_8005B5B8(Object* arg0, Object* arg1, s32 arg2) {
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
        func_800042A8(parent);
    }
    if (arg1 != NULL) {
        func_800042A8(arg1);
    }

    arg0->parent = arg1;
    hitInfo = arg0->objhitInfo;

    if (parent != NULL) {
        transform_point_by_object(arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, &arg0->positionMirror.x, &arg0->positionMirror.y, &arg0->positionMirror.z, parent);
        transform_point_by_object(arg0->positionMirror2.x, arg0->positionMirror2.y, arg0->positionMirror2.z, &arg0->positionMirror3.x, &arg0->positionMirror3.y, &arg0->positionMirror3.z, parent);
        rotate_point_by_object(arg0->speed.x, 0, arg0->speed.z, &speedX, &speedY, &speedZ, parent);
        dYaw = parent->srt.yaw + arg0->srt.yaw;
    } else {
        speedX = arg0->speed.x;
        speedZ = arg0->speed.z;
        dYaw = arg0->srt.yaw;
    }

    if (arg2 != 0) {
        parent = arg0->parent;
        if (arg0->parent != NULL) {
            inverse_transform_point_by_object(arg0->positionMirror.x, arg0->positionMirror.y, arg0->positionMirror.z, &arg0->srt.transl.x, &arg0->srt.transl.y, &arg0->srt.transl.z, arg0->parent);
            inverse_transform_point_by_object(arg0->positionMirror3.x, arg0->positionMirror3.y, arg0->positionMirror3.z, &arg0->positionMirror2.x, &arg0->positionMirror2.y, &arg0->positionMirror2.z, arg0->parent);
            inverse_rotate_point_by_object(speedX, 0, speedZ, &arg0->speed.x, &speedY, &arg0->speed.z, arg0->parent);
            dYaw -= arg0->parent->srt.yaw;
            CIRCLE_WRAP(dYaw)
            arg0->srt.yaw = dYaw;
        } else {
            arg0->srt.transl.x = arg0->positionMirror.x;
            arg0->srt.transl.y = arg0->positionMirror.y;
            arg0->srt.transl.z = arg0->positionMirror.z;
            arg0->positionMirror2.x = arg0->positionMirror3.x;
            arg0->positionMirror2.y = arg0->positionMirror3.y;
            arg0->positionMirror2.z = arg0->positionMirror3.z;
            arg0->speed.x = speedX;
            arg0->speed.z = speedZ;
            arg0->srt.yaw = dYaw;
        }
    }

    if (hitInfo != NULL) {
        hitInfo->unk10.x = arg0->srt.transl.x;
        hitInfo->unk10.y = arg0->srt.transl.y;
        hitInfo->unk10.z = arg0->srt.transl.z;
        hitInfo->unk20.x = arg0->positionMirror.x;
        hitInfo->unk20.y = arg0->positionMirror.y;
        hitInfo->unk20.z = arg0->positionMirror.z;
    }

    if (arg0->group == 1) {
        ((DLL_Unknown*)gDLL_27)->vtbl->func[7].withTwoArgs((s32)arg0, 
            ((DLL_Unknown*)arg0->dll)->vtbl->func[57].withOneArgS32((s32)arg0)
        );
    }
}
