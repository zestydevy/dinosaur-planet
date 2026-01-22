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
s32 func_800564C8(UnkFunc80051D68Arg3 *, UnkFunc80051D68Arg3 *, f32 *, s32, s8*, s32);
void func_80058F8C(void);

UnkFunc80051D68Arg3* func_80053B24(UnkFunc80051D68Arg3*, s32, s32, s32, s32, s32, s32, u8);
UnkFunc80051D68Arg3* func_8005471C(UnkFunc80051D68Arg3*, Unk8005341C*, ModelInstance*, f32, f32, f32, f32, f32, f32, u8);
void func_80054DF8(UnkFunc80051D68Arg3*, UnkFunc80051D68Arg3*, u8);

// move to objhits.h
Object **func_80025DD4(s32 *arg0);

// .bss 800bb200-800bb540
Vec3s32 D_800BB200[7];
// extra 0xC bytes?
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
// https://decomp.me/scratch/yoG09
UnkFunc80051D68Arg3* func_80053B24(UnkFunc80051D68Arg3* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7) {
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
    s32 sp104;
    s32 sp100;
    s32 spFC;
    s32 spF8;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 var_a0_3;
    s32 var_a1_3;
    s32 var_ra_2;
    s32 var_t1;
    s32 var_t2;
    s32 var_t3;
    s32 var_t4_2;
    s32 var_t5;
    s16 var_v0;
    s32 var_v1_2;
    s32 temp_a3_3;
    s32 temp_s3;
    s32 temp_s4;
    s32 temp_s5;
    s32 temp_s6;
    s32 temp_s7;
    s32 temp_t9_4;
    s32 var_t0;
    u8 var_v0_3;
    u8 spA6;
    u8 spA5;
    u8 spA4;
    s32 var_v0_2;
    FaceBatch* sp9C;
    s16* var_v1_3;

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
    temp_s4 = floor_f(arg1 / 640.0f);
    temp_s7 = floor_f(arg3 / 640.0f);
    temp_s5 = floor_f(arg4 / 640.0f);
    temp_s3 = floor_f(arg6 / 640.0f);
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
        sp104 = arg1 - D_800BB200[sp118].x;
        sp100 = arg4 - D_800BB200[sp118].x;
        spFC = arg3 - D_800BB200[sp118].z;
        spF8 = arg6 - D_800BB200[sp118].z;
        D_800BB200[sp118].x += D_80092A60;
        D_800BB200[sp118].z += D_80092A64;
        // sp104 = MAX(0, sp104);
        if (sp104 < 0) {
            sp104 = 0;
        }
        // sp100 = MIN(BLOCKS_GRID_UNIT, sp100);
        if (sp100 > BLOCKS_GRID_UNIT) {
            sp100 = BLOCKS_GRID_UNIT;
        }
        // spFC = MAX(0, spFC);
        if (spFC < 0) {
            spFC = 0;
        }
        // spF8 = MIN(BLOCKS_GRID_UNIT, spF8);
        if (spF8 > BLOCKS_GRID_UNIT) {
            spF8 = BLOCKS_GRID_UNIT;
        }
        temp_s6 = D_800BB200[sp118].x - D_800BB200->x;
        temp_s7 = D_800BB200[sp118].z - D_800BB200->z;
        temp_s0 = sp138[sp118];
        var_v0 = 1;
        sp122 = 0;
        for (var_t0 = 0; var_t0 < BLOCKS_GRID_UNIT; var_t0 += 80) {
            if ((var_t0 + 80) >= sp104 && sp100 >= var_t0) {
                sp122 |= var_v0;
            }
            var_v0 <<= 1;
        }
        for (var_t0 = 0; var_t0 < BLOCKS_GRID_UNIT; var_t0 += 80) {
            if (var_t0 >= (spFC - 80) && spF8 >= var_t0) {
                sp122 |= var_v0;
            }
            var_v0 <<= 1;
        }
        var_s3 = temp_s0->ptr_faceBatches;
        temp_t9 = &var_s3[temp_s0->faceBatch_count];
        sp9C = temp_t9;
        while ((u32) var_s3 < (u32) sp9C) {
            if (var_s3->renderSettingBitfield & 0x2000) {
                if (!(var_s3->renderSettingBitfield & 0x300000)) {
                    spA6 = 4;
                    if (!(arg7 & 0x20)) {
                        spA6 = 0x14;
                    }
                    goto block_55;
                }
            } else if (!(var_s3->renderSettingBitfield & 0x800) || (arg7 & 0x20)) {
                spA6 = 2;
block_55:
                if ((arg5 >= var_s3->Ymin) && (var_s3->Ymax >= arg2) && ((sp100 >> 2) >= (u8) var_s3->Xmin) && ((u8) var_s3->Xmax >= (sp104 >> 2)) && ((spF8 >> 2) >= (u8) var_s3->Zmin) && ((u8) var_s3->Zmax >= (spFC >> 2))) {
                    if (var_s3->renderSettingBitfield & 0x1000) {
                        spA6 |= 8;
                    }
                    sp124 = &temp_s0->unk20[temp_s0->flags & 1][var_s3->baseVertexID];
                    sp11C = var_s3->baseFaceID;
                    sp10C = var_s3[1].baseFaceID;
                    for (; sp11C < sp10C; sp11C++) {
                        var_t0 = temp_s0->unk14[sp11C] & sp122;
                        if ((var_t0 & 0xFF) && (var_t0 & 0xFF00)) {
                            var_t1 = SOME_MIN;
                            var_t2 = SOME_MAX;
                            var_t3 = SOME_MIN;
                            var_t4_2 = SOME_MAX;
                            var_t5 = SOME_MIN;
                            var_ra_2 = SOME_MAX;
                            sp128[0] = &sp124[(temp_s0->ptr_faces[sp11C].unk0 >> 0xD) & 0x1F];
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
                                if (var_t2 < var_a0_3) {
                                    var_t2 = var_a0_3;
                                }
                                if (var_a0_3 < var_t1) {
                                    var_t1 = var_a0_3;
                                }
                                if (var_t4_2 < var_v1_2) {
                                    var_t4_2 = var_v1_2;
                                    spA4 = var_t0;
                                }
                                if (var_v1_2 < var_t3) {
                                    var_t3 = var_v1_2;
                                    spA5 = var_t0;
                                }
                                if (var_ra_2 < var_a1_3) {
                                    var_ra_2 = var_a1_3;
                                }
                                if (var_a1_3 < var_t5) {
                                    var_t5 = var_a1_3;
                                }
                                arg0->unkA[var_t0] = var_a0_3 + temp_s6;
                                arg0->unk10[var_t0] = var_v1_2;
                                arg0->unk16[var_t0] = var_a1_3 + temp_s7;
                            }
                            if ((arg5 >= var_t3) && (var_t4_2 >= arg2) && (sp100 >= var_t1) && (var_t2 >= sp104) && (spF8 >= var_t5) && (var_ra_2 >= spFC)) {
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

                                    // This loop should write arg0->unk1C but I have no idea how it's actually indexed
                                    temp_t9_4 = sp11C * 9;
                                    temp_a3_3 = temp_t9_4 + 9;
                                    if (temp_t9_4 < temp_a3_3) {
                                        var_v0_2 = sp11C * 18;
                                        var_v1_3 = arg0 + (sp11C * 18) + -(sp11C * 18);
                                        do {
                                            var_v1_3[14] = *((s16*)temp_s0->ptr_faceEdgeVectors + var_v0_2);
                                            var_v1_3 += 1 ;
                                            var_v0_2 += 1 ;
                                        } while (var_v0_2 < (temp_a3_3 * 2));
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
                                    arg0->unk30 = (spA4 * 0x10) | spA5;
                                    arg0->unk2F = (temp_s0->ptr_faces[sp11C].unk4 & 1) | spA6;
                                    arg0++;
                                }
                            }
                        }
                    }
                }
            }
            var_s3++;
        }
    }
    return arg0;
}
#endif

#ifndef NON_EQUIVALENT
UnkFunc80051D68Arg3* func_8005471C(UnkFunc80051D68Arg3* arg0, Unk8005341C* arg1, ModelInstance* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, u8 arg9);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005471C.s")
#else
UnkFunc80051D68Arg3* func_8005471C(UnkFunc80051D68Arg3* arg0, Unk8005341C* arg1, ModelInstance* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, u8 arg9) {
    s16 spDC[3];
    s32 spB4;
    s32 spB0;
    u8 spAB; // s4
    u8 spAA; // s3
    f32 sp9C[3];
    f32 sp98;
    f32 sp8C[3];
    f32 sp88;
    f32 sp84;
    Model* sp7C;
    u32* var_s5;
    UnkFunc80051D68Arg3* var_a2;
    Vtx* temp_v1_3;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 var_fa0;
    f32 var_fa1;
    f32 var_fs0;
    f32 var_fv1;
    s16 temp_a1;
    s16 temp_t6_2;
    s16 temp_t7_2;
    s16 temp_t9;
    s16 temp_v0_3;
    s16 temp_v0_4;
    s32 var_ra;
    s32 var_t0;
    s32 var_t1;
    s32 var_t2;
    s32 var_t3;
    s32 var_t4;
    s16* var_a3;
    s32 temp_a0;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_t0;
    s32 temp_t0_2;
    s32 temp_t6_3;
    s32 temp_t7;
    s32 temp_t9_2;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_s0;
    s32 var_s7;
    s32 var_v0_2;
    s32 var_v1_2;
    s32 var_v1_3;
    u32 temp_t6;
    u32 temp_v0_2;
    u32 temp_v1_2;
    u8 temp_t8;
    u8 temp_v0_5;
    FacebatchBound* temp_v0;
    ModelFacebatch* temp_v1;
    s16* var_v0_3;
    s16* var_v0_5;
    s32 i;

    sp7C = arg2->model;
    vec3_transform(arg1->unk8, arg3, arg4, arg5, &sp98, &sp84, &sp88);
    vec3_transform(arg1->unk8, arg3, arg4, arg8, &sp9C[0], &arg4, &sp8C[0]);
    vec3_transform(arg1->unk8, arg6, arg7, arg5, &sp9C[1], &sp84, &sp8C[1]);
    vec3_transform(arg1->unk8, arg6, arg7, arg8, &sp9C[2], &arg7, &sp8C[2]);
    var_fv1 = sp98;
    var_fa1 = sp88;
    var_fa0 = var_fv1;
    var_fs0 = var_fa1;
    for (i = 0; i < 3; i++) {
        if (sp9C[i] < var_fa0) {
            var_fa0 = sp9C[i];
        }
        if (var_fv1 < sp9C[i]) {
            var_fv1 = sp9C[i];
        }
        if (sp8C[i] < var_fs0) {
            var_fs0 = sp8C[i];
        }
        if (var_fa1 < sp8C[i]) {
            var_fa1 = sp8C[i];
        }
    }
    spB0 = sp7C->unk70;
    var_s7 = 0;
    for (spB4 = 0; spB4 < spB0; spB4++, var_s7++) {
        temp_v1 = &sp7C->faces[var_s7];
        temp_a0 = temp_v1->tagC;
        // early returns might be incorrect, shifts some t regs by 1
        if ((temp_a0 & 0x100000)) {
            continue;
        }
        if ((temp_a0 & 0x800) && (arg9 & 0x20)) {
            continue;
        }
        temp_v0 = &sp7C->facebatchBounds[spB4];
        if ((temp_v0->unk0 < var_fa0) || (var_fv1 < temp_v0->unk2) || (temp_v0->unk4 < arg4) || (arg7 < temp_v0->unk6) || (temp_v0->unk8 < var_fs0)) {
            continue;
        }

        var_t0 = SOME_MIN;
        var_t1 = SOME_MAX;
        var_t3 = SOME_MIN;
        var_t2 = SOME_MAX;
        var_ra = SOME_MIN;
        var_t4 = SOME_MAX;
        var_s0 = (temp_a0 & 0x7FF) * 9;
        if (var_fa1 < temp_v0->unkA) {
            continue;
        }

        var_s5 = (u32*)&sp7C->displayList[temp_v1->baseF3DCommandIndex];
        while ((var_s5[0] >> 24) == 6 || (var_s5[0] >> 24) == 5) {
            spDC[0] = (var_s5[0] >> 17) & 0x1F;
            spDC[1] = (var_s5[0] >> 9) & 0x1F;
            spDC[2] = (var_s5[0] >> 1) & 0x1F;
            var_s5 += 1;
            var_a2 = arg0;
            for (var_a0 = 0; var_a0 < 3; var_a0++) {
                temp_v1_3 = &sp7C->vertices[spDC[var_a0] + temp_v1->baseVertexID];
                if (var_t1 < temp_v1_3->v.ob[0]) {
                    var_t1 = temp_v1_3->v.ob[0];
                }
                if (temp_v1_3->v.ob[0] < var_t0) {
                    var_t0 = temp_v1_3->v.ob[0];
                }
                if (var_t2 < temp_v1_3->v.ob[1]) {
                    var_t2 = temp_v1_3->v.ob[1];
                    spAA = var_a0;
                }
                if (temp_v1_3->v.ob[1] < var_ra) {
                    var_ra = temp_v1_3->v.ob[1];
                    spAB = var_a0;
                }
                if (var_t4 < temp_v1_3->v.ob[2]) {
                    var_t4 = temp_v1_3->v.ob[2];
                }
                if (temp_v1_3->v.ob[2] < var_t3) {
                    var_t3 = temp_v1_3->v.ob[2];
                }
                var_a2->unkA[var_a0] = temp_v1_3->v.ob[0];
                var_a2->unk10[var_a0] = temp_v1_3->v.ob[1];
                var_a2->unk16[var_a0] = temp_v1_3->v.ob[2];
            }
            if (!(arg7 < var_ra)) {
                if (!(var_t2 < arg4) && !(var_fv1 < var_t0) && !(var_t1 < var_fa0) && !(var_fa1 < var_t3) && !(var_t4 < var_fs0)) {
                    temp_a2 = var_s0 + 9;
                    if (sp7C->edgeVectors != NULL) {
                        temp_a2_2 = var_s0 + 9;
                        var_a0_2 = var_s0;
                        if (var_s0 < temp_a2_2) {
                            temp_t9_2 = ((var_s0 * 0) + 9) & 3;
                            temp_t0 = var_s0 * 2;
                            if (temp_t9_2 != 0) {
                                var_v0_3 = (s16*)arg0 + (var_a0_2 * 2) + -temp_t0;
                                var_v1_2 = var_a0_2 * 2;
                                do {
                                    var_a0_2 += 1;
                                    var_v0_3 += 2;
                                    var_v0_3[9] = sp7C->edgeVectors[var_v1_2];
                                    var_v1_2 += 2;
                                } while ((temp_t9_2 + var_s0) != var_a0_2);
                                // if (var_a0_2 != temp_a2_2) {
                                //     goto block_52;
                                // }
//                                         } else {
// block_52:
//                                             var_v0_4 = arg0 + (var_a0_2 * 2) + -temp_t0;
//                                             var_v1_3 = var_a0_2 * 2;
//                                             do {
//                                                 var_v0_4 += 8;
//                                                 var_v0_4->unk14 = (s16) *(sp7C->edgeVectors + var_v1_3);
//                                                 var_v0_4->unk16 = (s16) (sp7C->edgeVectors + var_v1_3)->unk2;
//                                                 var_v0_4->unk18 = (s16) (sp7C->edgeVectors + var_v1_3)->unk4;
//                                                 temp_t7_2 = (sp7C->edgeVectors + var_v1_3)->unk6;
//                                                 var_v1_3 += 8;
//                                                 var_v0_4->unk1A = temp_t7_2;
//                                             } while (var_v1_3 != (temp_a2_2 * 2));
                            }
                        }
                    } else {
                        var_a0_3 = var_s0;
                        if (var_s0 < temp_a2) {
                            temp_t6_3 = ((var_s0 * 0) + 9) & 3;
                            temp_t0_2 = var_s0 * 2;
                            if (temp_t6_3 != 0) {
                                var_v0_5 = (s16*)arg0 + (var_a0_3 * 2) + -temp_t0_2;
                                do {
                                    var_a0_3 += 1;
                                    var_v0_5[9] = 0;
                                    var_v0_5 += 2;
                                } while ((temp_t6_3 + var_s0) != var_a0_3);
                                // if (var_a0_3 != temp_a2) {
                                //     goto block_60;
                                // }
//                                         } else {
// block_60:
//                                             var_v0_6 = arg0 + (var_a0_3 * 2) + -temp_t0_2;
//                                             do {
//                                                 var_a0_3 += 4;
//                                                 var_v0_6->unk1E = 0;
//                                                 var_v0_6->unk20 = 0;
//                                                 var_v0_6->unk22 = 0;
//                                                 var_v0_6 += 8;
//                                                 var_v0_6->unk14 = 0;
//                                             } while (var_a0_3 != temp_a2);
                            }
                        }
                        arg0->unk1C[1] = 0x7FFF;
                        arg0->unk1C[4] = 0x7FFF;
                        arg0->unk1C[7] = 0x7FFF;
                    }
                    temp_v0_5 = sp7C->faces[var_s7].materialID;
                    if (temp_v0_5 == 0xFF) {
                        arg0->unk30 = 0;
                    } else {
                        arg0->unk30 = (s8) sp7C->materials[temp_v0_5].unk4;
                    }
                    arg0->pad31[1] = (spAA * 0x10) | spAB;
                    arg0->pad31[0] = 1;
                    arg0 += 1;
                }
            }
            var_t0 = SOME_MIN;
            var_t1 = SOME_MAX;
            var_t3 = SOME_MIN;
            var_t2 = SOME_MAX;
            var_ra = SOME_MIN;
            var_t4 = SOME_MAX;
            var_s0 += 9;
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
                    sp6F |= func_800564C8(&D_80092E70[var_s3->unk4], &D_80092E70[var_s3[1].unk4], &spA0[0], arg3, (s8* ) arg4, 1);
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
                sp6F |= func_800564C8(&D_80092E70[var_s3->unk4], &D_80092E70[var_s3[1].unk4], arg2, arg3, (s8* ) arg4, 0);
            } else {
                sp6F |= func_80055458(sp64, &D_80092E70[var_s3->unk4], &D_80092E70[var_s3[1].unk4], arg1, arg2, arg3, (s8* ) arg4, 0);
            }
            var_s3 += 1;
        } while ((u32) var_s3 < (u32) target);
    }
    return sp6F;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80055458.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800564C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800567F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80056BCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80056E50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800573D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80057A30.s")

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
