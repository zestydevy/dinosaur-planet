#include "common.h"

// voxmap2.c ?
// unlike the other voxmap.c file, this one is used by kyte and tricky

static const char str_80099db0[] = "****** GOAL FOUND Iterations=%d ******\n";
static const char str_80099dd8[] = " VOXMAPS : Overflow in Route Points for Net Route Finding \n";
static const char str_80099e14[] = " Curve Node %i \n";
static const char str_80099e28[] = " Curve Node %i \n";
static const char str_80099e3c[] = "net_route: No support for this type of node %d x=%f y=%f z=%f\n";
static const char str_80099e7c[] = "**** HEAP INSERT ****\n";
static const char str_80099e94[] = "**** NODE FIND ****\n";

typedef struct {
/*0*/ u16 unk0;
/*2*/ u16 unk2;
/*4*/ CurveSetup *unk4;
} UnkVoxmap2Struct;

// size: 0x10
typedef struct {
/*0*/ CurveSetup *curve;
/*4*/ u32 distSq;
/*8*/ u32 unk8;
/*C*/ u8 unkC;
/*D*/ u8 unkD;
/*E*/ u8 unkE;
} Voxmap2Struct_0;

// element of a binary max-heap/priority queue
// size: 0x8
typedef struct {
/*0*/ u32 priority;
/*4*/ u16 curveIdx;
} Voxmap2Struct_4;

// size: 0x30
typedef struct {
/*00*/ Voxmap2Struct_0 *unk0; // len: 254
/*04*/ Voxmap2Struct_4 *unk4; // len: 254
/*08*/ CurveSetup **unk8; // len: 100
/*0C*/ Vec3f *unkC; // target/goal?
/*10*/ void *unk10;
/*14*/ u8 _unk14[0x18 - 0x14];
/*18*/ CurveSetup *unk18;
/*1C*/ s32 unk1C;
/*20*/ s16 unk20; // unk0 count
/*22*/ s16 unk22; // unk4 count
/*24*/ u32 unk24;
/*28*/ u8 unk28;
/*29*/ u8 _unk29[0x2A - 0x29];
/*2A*/ s16 unk2A;
/*2C*/ s16 unk2C;
/*2E*/ u8 _unk2E[0x30 - 0x2E];
} Voxmap2Struct;

extern CurveSetup* D_800B4920;

void func_8003997C(Voxmap2Struct*, Voxmap2Struct_0*, s32);
void func_80039C6C(Voxmap2Struct*, Voxmap2Struct_0*, s32, u32, CurveSetup*);
void func_80039DD0(Voxmap2Struct* arg0);
void func_80039FA4(Voxmap2Struct_4* arg0, s16* arg1, u16 arg2, u32 arg4);
void func_8003A010(Voxmap2Struct_4* arg0, s32 arg1, u16 arg2, u32 arg3);
s32 func_8003A0B8(Voxmap2Struct_4* heap, s16* heapLengthPtr);
Voxmap2Struct_0* func_8003A130(Voxmap2Struct* arg0, CurveSetup *arg1, u32 arg2, u16 arg3);
s32 func_8003A240(Voxmap2Struct*, CurveSetup*, s32*);
s32 func_8003A298(Voxmap2Struct*, Voxmap2Struct_0*);

// public
void func_80039590(Voxmap2Struct *arg0) {
    arg0->unk0 = (Voxmap2Struct_0*)mmAlloc(
        (sizeof(Voxmap2Struct_0) * 254) + (sizeof(Voxmap2Struct_4) * 254) + (sizeof(CurveSetup*) * 100), 
        ALLOC_TAG_VOX_COL, 
        NULL);
    arg0->unk4 = (Voxmap2Struct_4*)(arg0->unk0 + 254);
    arg0->unk8 = (CurveSetup**)(arg0->unk4 + 254);
}

// public
void func_800395D8(Voxmap2Struct *arg0) {
    if (arg0->unk0 != NULL) {
        mmFree(arg0->unk0);
        arg0->unk0 = NULL;
    }
}

// public
s32 func_80039610(Voxmap2Struct* arg0, CurveSetup* arg1, Vec3f* arg2, void *arg3, s32 arg5) {
    Voxmap2Struct_0* temp_v0;

    func_80039DD0(arg0); // reset
    arg0->unk18 = arg1;
    arg0->unkC = arg2;
    arg0->unk10 = arg3;
    arg0->unk28 = arg5 & 1;
    arg0->unk24 = 10000;
    temp_v0 = func_8003A130(arg0, arg1, 0, 0xFF); // insert
    func_80039FA4(arg0->unk4, &arg0->unk22, (arg0->unk20 - 1), temp_v0->distSq + temp_v0->unk8); // another insert
    return 0;
}

// public
s32 func_800396B0(Voxmap2Struct* arg0, s32 arg1) {
    Voxmap2Struct_0* temp_s1;
    s32 curveIdx;
    s32 var_s3;
    s32 var_s5;

    var_s3 = 0;
    var_s5 = 0;
    while ((var_s3 == 0) && (arg1 != 0)) {
        curveIdx = func_8003A0B8(arg0->unk4, &arg0->unk22); // get next curve from priority queue
        if (curveIdx >= 0) {
            arg0->unk1C = curveIdx;
            temp_s1 = &arg0->unk0[curveIdx];
            if (func_8003A298(arg0, temp_s1) != 0) {
                // goal found?
                var_s3 = 1;
                var_s5 = 1;
            } else {
                temp_s1->unkE = 1;
                func_8003997C(arg0, temp_s1, curveIdx);
            }
        } else {
            var_s3 = 1;
            var_s5 = -1;
        }
        arg1 -= 1;
    }
    return var_s5;
}

// public
s32 func_80039798(Voxmap2Struct* arg0) {
    CurveSetup** temp_t6;
    Voxmap2Struct_0* var_v1;
    Voxmap2Struct_0* var_v1_2;
    s32 var_v0;
    s32 var_a1;
    s32 var_a2;

    var_v0 = arg0->unk1C;
    var_v1 = &arg0->unk0[var_v0];
    var_v1->unkD = 0xFF;
    while (var_v1->unkC != 0xFF) {
        var_a2 = var_v1->unkC;
        var_v1 = &arg0->unk0[var_v1->unkC];
        var_v1->unkD = var_v0;
        var_v0 = var_a2;
        var_a2 = var_v1->unkC;
    }
    if (var_v1->unkD == 0xFF) {
        var_v1_2 = NULL;
    } else {
        var_v1_2 = &arg0->unk0[var_v1->unkD];
    }
    var_a1 = 0;
    while (var_v1_2 != NULL) {
        arg0->unk8[var_a1] = var_v1_2->curve;
        var_a1 += 1;
        if (var_a1 >= 100) {
            var_v1_2 = NULL;
        } else if (var_v1_2->unkD == 0xFF) {
            var_v1_2 = NULL;
        } else {
            var_v1_2 = &arg0->unk0[var_v1_2->unkD];
        }
    }
    arg0->unk2A = var_a1;
    arg0->unk2C = 0;
    return var_a1;
}

// public
CurveSetup* func_80039870(Voxmap2Struct* arg0) {
    if (arg0->unk2C < arg0->unk2A) {
        return arg0->unk8[arg0->unk2C++];
    }
    return NULL;
}

s32 func_800398AC(CurveSetup* arg0, UnkVoxmap2Struct *arg1) {
    if ((arg0->type22.unk30 == -1 || main_get_bits(arg0->type22.unk30) != 0) && 
        (arg0->type22.usedBit == -1 || main_get_bits(arg0->type22.usedBit) == 0)) {
        if (arg0->base_type22.unk4 == arg1->unk0) {
            return 1;
        }
        if (arg0->unk1A < 3) {
            return 1;
        }
        if (arg1->unk4 != NULL) {
            if (arg0 == arg1->unk4) {
                return 1;
            }
        } else {
            if (arg1->unk2 == arg0->base_type22.unk4) {
                return 1;
            }
        }
    }
    return 0;
}

void func_8003997C(Voxmap2Struct* arg0, Voxmap2Struct_0* arg1, s32 curveIdx) {
    CurveSetup* temp_s5;
    CurveSetup* temp_v0;
    s32 temp_a0;
    s32 var_s1;
    u8 var_fp;

    temp_s5 = arg1->curve;
    if (arg0->unk28 != 0) {
        var_fp = temp_s5->unk1B;
    } else {
        var_fp = ~temp_s5->unk1B;
    }
    
    for (var_s1 = 0; var_s1 < 4; var_s1++) {
        temp_a0 = temp_s5->unk1C[var_s1];
        if ((temp_a0 >= 0) && (var_fp & (1 << var_s1))) {
            temp_v0 = gDLL_26_Curves->vtbl->func_39C(temp_a0);
            if (temp_v0 != NULL) {
                switch (temp_v0->unk19) {
                case 0x22: // Kyte
                    if (func_800398AC(temp_v0, (UnkVoxmap2Struct*)arg0->unk10) != 0) {
                        func_80039C6C(arg0, arg1, curveIdx, 
                            (u32) (vec3_distance_squared(&temp_s5->pos, &temp_v0->pos) + (f32) arg1->unk8), 
                            temp_v0);
                    }
                    break;
                case 0x24: // Tricky
                    main_get_bits(BIT_4E2);
                    if ((temp_v0->type22.unk30 == -1 || main_get_bits(temp_v0->type22.unk30) != 0) &&
                           (temp_v0->type22.usedBit == -1 || main_get_bits(temp_v0->type22.usedBit) == 0)) {
                        func_80039C6C(arg0, arg1, curveIdx, 
                            (u32) (vec3_distance_squared(&temp_s5->pos, &temp_v0->pos) + (f32) arg1->unk8), 
                            temp_v0);
                    }
                    break;
                default:
                    D_800B4920 = temp_v0;
                }
            }
        }
    }
}

void func_80039C6C(Voxmap2Struct* arg0, Voxmap2Struct_0* arg1, s32 arg2, u32 arg3, CurveSetup* arg4) {
    s32 sp3C;
    s32 temp_v0;
    Voxmap2Struct_0* temp_v0_2;
    Voxmap2Struct_0* temp_v1;

    if (func_8003A298(arg0, arg1) != 0) {
        temp_v0 = arg0->unk20;
        func_8003A130(arg0, arg4, arg3, (u16) arg2);
        func_80039FA4(arg0->unk4, &arg0->unk22, temp_v0, 1);
    }
    temp_v0 = func_8003A240(arg0, arg4, &sp3C);
    if ((temp_v0 >= 0) && (sp3C == 0)) {
        temp_v1 = &arg0->unk0[temp_v0];
        if (arg3 < temp_v1->unk8) {
            temp_v1->unkC = (u8) arg2;
            temp_v1->unk8 = arg3;
            func_8003A010(arg0->unk4, arg0->unk22, temp_v0, temp_v1->distSq + arg3);
        }
    } else if (temp_v0 < 0) {
        temp_v0 = arg0->unk20;
        temp_v1 = func_8003A130(arg0, arg4, arg3, (u16) arg2);
        if (temp_v1 != NULL) {
            if (arg0->unk24 < temp_v1->distSq) {
                func_80039FA4(arg0->unk4, &arg0->unk22, temp_v0, temp_v1->distSq + temp_v1->unk8);
            } else {
                if (temp_v1->distSq < arg0->unk24) {
                    arg0->unk24 = temp_v1->distSq;
                }
                func_80039FA4(arg0->unk4, &arg0->unk22, temp_v0, temp_v1->distSq + temp_v1->unk8);
            }
        }
    }
}

// reset
void func_80039DD0(Voxmap2Struct* arg0) {
    s32 i;

    arg0->unk22 = 0;
    arg0->unk20 = 0;
    for (i = 0; i < 254; i++) {
        arg0->unk4[i].priority = 0;
        arg0->unk0[i].unkE = 0;
    }
}

// up-heap
void func_80039E78(Voxmap2Struct_4* heap, s32 idx) {
    u32 priority;
    s32 parent;
    s32 curveIdx;

    priority = heap[idx].priority;
    curveIdx = heap[idx].curveIdx;
    heap[0].priority = -1;
    parent = idx >> 1;
    while (heap[parent].priority < priority) {
        heap[idx].curveIdx = heap[parent].curveIdx;
        heap[idx].priority = heap[parent].priority;
        idx = parent;
        parent = parent >> 1;
    }
    heap[idx].priority = priority;
    heap[idx].curveIdx = curveIdx;
}

// down-heap
void func_80039F08(Voxmap2Struct_4* heap, s32 heapLength, s32 idx) {
    s32 var_t1;
    s32 temp_a3;
    u32 temp_v0;

    temp_v0 = heap[idx].priority;
    temp_a3 = heap[idx].curveIdx;
    while ((heapLength >> 1) >= idx) {
        // find smaller child
        var_t1 = idx + idx;
        if (var_t1 < heapLength) {
            if (heap[var_t1].priority < heap[var_t1 + 1].priority) {
                var_t1 = var_t1 + 1;
            }
        }

        if (temp_v0 >= heap[var_t1].priority) {
            break;
        }

        heap[idx].priority = heap[var_t1].priority;
        heap[idx].curveIdx = heap[var_t1].curveIdx;
        idx = var_t1;
    }
    heap[idx].priority = temp_v0;
    heap[idx].curveIdx = temp_a3;
}

// insert curve into priority queue
void func_80039FA4(Voxmap2Struct_4* heap, s16* heapLengthPtr, u16 curveIdx, u32 curveDist) {
    *heapLengthPtr += 1;
    heap[*heapLengthPtr].curveIdx = curveIdx;
    heap[*heapLengthPtr].priority = -1 - curveDist; // invert distance?
    func_80039E78(heap, *heapLengthPtr); // up-heap
}

// change heap node priority
void func_8003A010(Voxmap2Struct_4* heap, s32 heapLength, u16 curveIdx, u32 newPriority) {
    s32 idx;
    s32 i;
    u32 prevPriority;

    // find curve in heap
    for (i = 0; heapLength >= i; i++) {
        if (curveIdx == heap[i].curveIdx) {
            idx = i;
            i = heapLength + 1; // break
        }
    }
    // change priority
    prevPriority = heap[idx].priority;
    heap[idx].priority = newPriority;
    // fix heap
    if (newPriority < prevPriority) {
        func_80039F08(heap, heapLength, idx);
    } else if (prevPriority < newPriority) {
        func_80039E78(heap, idx); // up-heap
    }
}

// binary heap extract
s32 func_8003A0B8(Voxmap2Struct_4* heap, s16* heapLengthPtr) {
    u16 curveIdx;
    
    if (*heapLengthPtr == 0) {
        return -1;
    }

    curveIdx = heap[1].curveIdx;
    heap[1].priority = heap[*heapLengthPtr].priority;
    heap[1].curveIdx = heap[*heapLengthPtr].curveIdx;
    *heapLengthPtr -= 1;
    func_80039F08(heap, *heapLengthPtr, 1);
    return (s32) curveIdx;
}

// add curve
Voxmap2Struct_0* func_8003A130(Voxmap2Struct* arg0, CurveSetup *curve, u32 arg2, u16 arg3) {
    Voxmap2Struct_0* temp_v1;

    if (arg0->unk20 == 254) {
        return NULL;
    }
    temp_v1 = &arg0->unk0[arg0->unk20++];
    temp_v1->curve = curve;
    temp_v1->unk8 = arg2;
    temp_v1->unkC = arg3;
    temp_v1->distSq = (u32) vec3_distance_squared(&curve->pos, arg0->unkC);
    return temp_v1;
}

s32 func_8003A240(Voxmap2Struct* arg0, CurveSetup* arg1, s32* arg2) {
    Voxmap2Struct_0* temp_a0;
    s32 i;

    for (i = 0; i < arg0->unk20; i++) {
        temp_a0 = &arg0->unk0[i];
        if (arg1 == temp_a0->curve) {
            *arg2 = temp_a0->unkE;
            return i;
        }
    }
    return -1;
}

s32 func_8003A298(Voxmap2Struct* arg0, Voxmap2Struct_0* arg1) {
    UnkVoxmap2Struct *new_var;
    CurveSetup* temp_a1;
    void *temp_v0;
    s32 temp3;

    temp_v0 = arg0->unk10;
    switch (arg1->curve->unk19) {
    case 0x22: // Kyte
        new_var = (UnkVoxmap2Struct*)arg0->unk10;
        if (new_var->unk4 != NULL) {
            return new_var->unk4 == arg1->curve;
        }
        return new_var->unk2 == arg1->curve->base_type22.unk4;
    case 0x24: // Tricky
        if (!(arg1->unkC & 0x80)) {
            temp3 = arg1->curve->unk3;
            if (temp3 != 0) {
                return (s32)temp_v0 == arg1->curve->unk3;
            }

            if (arg0->unk0[arg1->unkC].curve) {}

            temp_a1 = arg0->unk0[arg1->unkC].curve;
            if (arg1->curve->uID == temp_a1->unk1C[0]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk4;
            }
            if (arg1->curve->uID == temp_a1->unk1C[1]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk5;
            }
            if (arg1->curve->uID == temp_a1->unk1C[2]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk6;
            }
            if (arg1->curve->uID == temp_a1->unk1C[3]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk7;
            }
        }
        return 0;
    default:
      return ((CurveSetup*)temp_v0) == (arg1->curve);
    }
}
