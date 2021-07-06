#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037780.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800378E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800378FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037910.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037924.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037978.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003798C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800379D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037A14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037DB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037EC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/create_3_megs_quues.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037F8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80037F9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003825C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800382AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003833C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80038DC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800390A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039560.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039578.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039590.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800395D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039610.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800396B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039798.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039870.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_800398AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003997C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039C6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039DD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039E78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039F08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_80039FA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003A010.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003A0B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003A130.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003A240.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003A298.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/three_more_mallocs.s")

u32 read_le32(u8 *b)
{
    u32 result = *b++;
    result |= *b++ << 8;
    result |= *b++ << 16;
    result |= *b++ << 24;
    return result;
}

extern u32 * D_800918B4;
void func_8003A418(s32);
u32 read_file_8bytes(s32 arg0, s32 arg1, s32 arg2) {
    if (arg2 != 0) {
        read_file_region(arg0, D_800918B4, arg1, 8);
    } else {
        queue_load_file_region_to_ptr(D_800918B4, arg0, arg1, 8);
    }
    return read_le32(D_800918B4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/inflate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_38380/func_8003A534.s")
