#include "common.h"

typedef struct UnkStruct_8000B2D0 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 *unk8;
    s8 *unkC; // could also be a u8 *
} UnkStruct_8000B2D0;

void func_8000B798(u8 *, s32, s32);
void func_8000B6D0(u8 *, s32);

// create / init func?
UnkStruct_8000B2D0* func_8000B2D0(UnkStruct_8000B2D0* arg0, u8* arg1, s32 arg2, s32 arg3, s32 arg4) {
    arg0->unk8 = arg1;
    arg0->unkC = arg2;
    arg0->unk0 = 0;
    arg0->unk2 = arg3;
    arg0->unk4 = arg4;
    
    bzero(arg1, (arg3 * 2) + 2);
    arg0->unk8[0] = 0x7F;
    return arg0;
}

UnkStruct_8000B2D0 *func_8000B334(s32 arg0, s32 arg1) {
    UnkStruct_8000B2D0* sp1C;

    sp1C = (UnkStruct_8000B2D0 *) mmAlloc((arg0 * 2) + (arg1 * arg0) + 0x12, 0x1A, NULL);
    sp1C->unk8 = sp1C + 1;
    sp1C->unkC = (sp1C->unk8 + 2 + (arg0 * 2));
    sp1C->unk0 = 0;
    sp1C->unk2 = arg0;
    sp1C->unk4 = arg1;
    bzero(sp1C->unk8, (arg0 * 2) + 2);
    sp1C->unk8[0] = 0x7F;
    return sp1C;
}

// free func?
void func_8000B3C4(UnkStruct_8000B2D0* arg0) {
    mmFree(arg0);
}

void func_8000B3E4(UnkStruct_8000B2D0* arg0, u8 arg1, UnkStruct_8000B2D0* arg2) {
    s32 i;
    s32 sp18;
    
    for (i = 0; i < arg0->unk2; i++) {
        if (!(arg0->unk8[i * 2] & 0x80)) {
            sp18 = i;
            // break
            i = arg0->unk2;
        }
    }

    bcopy(arg2, arg0->unkC + (sp18 * arg0->unk4), (s32) arg0->unk4);
    
    arg0->unk8[sp18 * 2] |= 0x80;
    arg0->unk0++;
    arg0->unk8[arg0->unk0 * 2 + 1] = sp18;
    arg0->unk8[arg0->unk0 * 2] &= 0x80;
    arg0->unk8[arg0->unk0 * 2] |= arg1;
    
    func_8000B6D0(arg0->unk8, arg0->unk0);
}

s32 func_8000B4FC(UnkStruct_8000B2D0* arg0, UnkStruct_8000B2D0* arg1) {
    s16 mask;
    s32 sp20;
    s32 sp1C;

    sp20 = arg0->unk8[3];
    sp1C = arg0->unk8[2] & ~0x80;
    bcopy(&arg0->unkC[sp20 * arg0->unk4], arg1, arg0->unk4);
    mask = 0xFF7F;
    arg0->unk8[sp20 * 2] &= mask;
    arg0->unk8[2] &= 0x80;
    arg0->unk8[2] |= arg0->unk8[arg0->unk0 * 2] & mask;
    arg0->unk8[3] = arg0->unk8[arg0->unk0 * 2 + 1];
    arg0->unk0 -= 1;
    func_8000B798(arg0->unk8, arg0->unk0, 1);
    return sp1C;
}

void func_8000B5E0(UnkStruct_8000B2D0* arg0, s32 arg1) {
    s32 idx = arg0->unk8[(arg1 * 2) + 1] * 2;

    arg0->unk8[idx] &= ~0x80;
    arg0->unk8[(arg1 * 2)] &= 0x80;
    arg0->unk8[(arg1 * 2)] |= arg0->unk8[arg0->unk0 * 2] & ~0x80;
    arg0->unk8[(arg1 * 2) + 1] = arg0->unk8[arg0->unk0 * 2 + 1];
    arg0->unk0 -= 1;
    func_8000B798(arg0->unk8, arg0->unk0, arg1);
}

s32 func_8000B6A4(UnkStruct_8000B2D0* arg0) {
    return arg0->unk0 == arg0->unk2;
}

s32 func_8000B6BC(UnkStruct_8000B2D0* arg0) {
    return arg0->unk0 == 0;
}

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B6D0.s")
#else
void func_8000B6D0(u8 *arg0, s32 arg1) {
    u16 temp_t2;
    u16 var_t0;
    s32 temp_a3;

    var_t0 = arg1 >> 1;
    temp_t2 = arg0[arg1 * 2] & ~0x80;
    temp_a3 = arg0[arg1 * 2 + 1];
    while ((arg0[var_t0 * 2] & ~0x80) < temp_t2) {
        arg0[arg1 * 2 + 1] = arg0[var_t0 * 2 + 1];
        arg0[arg1 * 2] &= 0x80;
        arg0[arg1 * 2] |= (arg0[var_t0 * 2] & ~0x80);
        arg1 = var_t0;
        var_t0 >>= 1;
    }
    arg0[arg1 * 2] &= 0x80;
    arg0[arg1 * 2] |= temp_t2;
    arg0[arg1 * 2 + 1] = temp_a3;
}
#endif

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B798.s")
#else
void func_8000B798(u8* arg0, s32 arg1, s32 arg2) {
    s32 temp_t1;
    u16 temp_t3;
    u16 temp_t7;
    s32 var_a2;
    u16 var_t2;
    u8 temp_t0;
    u8 temp_t7_2;
    u8 var_a3;
    u8* var_v1;
    u8* temp_t6;

    var_a2 = arg2;
    var_v1 = &arg0[var_a2 * 2];
    var_a3 = arg0[var_a2 * 2 + 0];
    temp_t1 = arg1 >> 1;
    temp_t7 = var_a3 & ~0x80;
    temp_t0 = arg0[var_a2 * 2 + 1];
    if (temp_t1 >= var_a2) {
loop_2:
        temp_t3 = var_a2 + var_a2;
        var_v1 = &arg0[var_a2 * 2];
        var_t2 = var_a2 + var_a2;
        var_a3 = var_v1[0];
        if (temp_t3 < arg1) {
            temp_t6 = &arg0[temp_t3 * 2];
            if ((temp_t6[0] & ~0x80) < (arg0[var_t2 * 2 + 2] & ~0x80)) {
                var_t2 = temp_t3 + 1;
            }
        }
        if (temp_t7 < (arg0[var_t2 * 2] & ~0x80)) {
            var_v1[0] = var_a3 & 0x80;
            var_v1[0] |= arg0[var_t2 * 2] & ~0x80;
            var_v1[1] = arg0[var_t2 * 2 + 1];
            var_a2 = var_t2;
            if (temp_t1 < var_t2) {
                var_v1 = arg0 + (var_a2 * 2);
                var_a3 = var_v1[0];
            } else {
                goto loop_2;
            }
        }
    }
    arg0[var_a2 * 2 + 0] &= 0x80;
    arg0[var_a2 * 2 + 0] |= temp_t7;
    arg0[var_a2 * 2 + 1] = temp_t0;
}
#endif

void func_8000B89C(UnkStruct_8000B2D0* arg0) {
}