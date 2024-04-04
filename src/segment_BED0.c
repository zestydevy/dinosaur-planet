#include "common.h"

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B2D0.s")
#else
/*O2 -g3*/
UnkStruct_8000B2D0* func_8000B2D0(UnkStruct_8000B2D0* arg0, s8* arg1, s32 arg2, s32 arg3, s32 arg4) {
    register UnkStruct_8000B2D0* arg0_copy;
    arg0_copy = arg0;
    
    
    arg0_copy->var8 = arg1;
    arg0_copy->varC = arg2;
    arg0_copy->var0 = 0;
    arg0_copy->var2 = (s16) arg3;
    arg0_copy->var4 = (s16) arg4;
    
    bzero(arg1, (arg3 * 2) + 2);
    *arg0_copy->var8 = 0x7F;
    return arg0_copy;
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B334.s")
#else

#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B3C4.s")
#else
void func_8000B3C4(void* arg0) {
    free(arg0);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B3E4.s")
#else
/* Might use arrays, but matches as is*/
void func_8000B3E4(struct UnkStruct_8000B3E4* arg0, u8 arg1, void* arg2) {
    s32 var_v0;
    s32 sp18;
    
    // Probably a for loop, couldn't get it to match
    var_v0 = 0;
    if (arg0->var2 > 0) {
        do {
            if (!(*(u8*)(arg0->var8 + (var_v0 * 2)) & 0x80)) {
                sp18 = var_v0;
                var_v0 = (s32) arg0->var2;
            }
            var_v0 += 1;
        } while (var_v0 < arg0->var2);
    }
    

    bcopy(arg2, arg0->varC + (sp18 * arg0->var4), (s32) arg0->var4);
    
    /* I think i kept this commented code because the way to get it to mathc was equally as scuffed*/

    // temp_v1_2 = arg0->var8 + (sp18 * 2);
    // *temp_v1_2 |= 0x80;
    *(u8*)(arg0->var8 + (sp18 * 2)) |= 0x80;
    
    arg0->var0 = (s16) (arg0->var0 + 1);
    
    //(arg0->var8 + (arg0->var0 * 2))->var1 = (s8) sp18;
    //(arg0->var8[arg0->var0 * 2])->var1 = (s8) sp18;
    *(u8*)(arg0->var8 + (arg0->var0 * 2) + 1) = (s8) sp18; /*scuffed*/
    
    // temp_v0 = arg0->var8 + (arg0->var0 * 2);
    // *temp_v0 &= 0x80;
    *(u8*)(arg0->var8 + (arg0->var0 * 2)) &= 0x80;
    
    // temp_v0_2 = arg0->var8 + (arg0->var0 * 2);
    // *temp_v0_2 |= arg1;
    *(u8*)(arg0->var8 + (arg0->var0 * 2)) |= arg1;
    
    func_8000B6D0(arg0->var8, arg0->var0, sp18, arg0);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B4FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B5E0.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B6A4.s")
#else
s32 func_8000B6A4(UnkStruct_8000B6A4* arg0) {
    return arg0->var0 == arg0->var2;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B6BC.s")
#else
s32 func_8000B6BC(s16* arg0) {
    return *arg0 == 0;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B6D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B798.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/segment_BED0/func_8000B89C.s")
#else
void func_8000B89C(s32 arg0) {
}
#endif