#include "common.h"

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
    u8 _unk0[0x34];
} D_80092E70_Struct;

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

s32 func_80055458(Object*, D_80092E70_Struct *, D_80092E70_Struct *, f32*, f32 *, s32, s8*, s32);
s32 func_800564C8(D_80092E70_Struct *, D_80092E70_Struct *, f32 *, s32, s8*, s32);
void func_80058F8C(void);

typedef struct Unk800BB268 {
    Object *unk0;
    s16 unk4;
    MtxF *unk8;
    MtxF *unkC;
} Unk800BB268;

extern D_80092E70_Struct *D_80092E70; // 250 length
extern HitsLineReencoded* D_80092E74;
extern s32 D_80092E78;
extern s32 D_80092E7C;
extern s8 D_80092E80;
extern HitsUnk* D_80092E84;
extern Unk800BB268 D_800BB268[];
extern D_800BB26C_Struct D_800BB26C[];
extern u8 D_800BB3A8;
extern s16 D_800BB4D6;
extern s16 D_800BB4D8;
extern s8 D_800BB538;
extern s8 D_800BB539;

// .bss 800bb200-800bb540
u8 _bss_800bb200[0x800bb540 - 0x800bb200]; // size:0x340

void func_80053300(void) {
    if (D_80092E70 == NULL) {
        D_80092E70 = mmAlloc(250*sizeof(D_80092E70_Struct), 0xFFFF00FF, 0);
        D_80092E74 = mmAlloc(400*sizeof(HitsLineReencoded), 0xFFFF00FF, 0);
        D_80092E78 = mmAlloc(4800, 0xFFFF00FF, 0);
        D_80092E7C = mmAlloc(800, 0xFFFF00FF, 0);
        D_80092E84 = mmAlloc(20*sizeof(HitsUnk), 0xFFFF00FF, 0);
    }
    func_80058F8C();
    D_800BB4D6 = 0;
    D_800BB4D8 = 0;
    D_800BB539 = 0;
    D_800BB538 = 0;
}

void func_800533D8(s32* arg0, s32* arg1) {
    *arg0 = D_800BB26C[D_800BB3A8].unk0;
    *arg1 = D_80092E70;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80053408.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005341C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80053750.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80053B24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005471C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80054DF8.s")

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
