#include "common.h"


// regalloc
#if NON_MATCHING
TMap *_func_8000B8B0(TMap *map, s32 arg1, s32 arg2, s32 arg3) {

    s32 temp_t6 = arg2 >> 3;
    map->m0004 = temp_t6;
    if ((arg2 & 7) != 0) {
        map->m0004 = temp_t6 + 1;
    }
    map->m0008 = arg2;
    map->m000C = arg3;
    map->m0000 = arg1;
    map->m0010 = 0;

    return map;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/voxmaps/func_8000B8B0.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/voxmaps/func_8000B8E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmaps/func_8000B970.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmaps/func_8000BA20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmaps/func_8000BA50.s")
