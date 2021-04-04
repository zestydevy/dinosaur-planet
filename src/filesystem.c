#include "common.h"

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/init_filesystem.s")
#else
extern void * romcopy_dat;
void _init_filesystem(void)
{
    s32 a3;

    osCreateMesgQueue(D_800B2EA8, D_800B2E68, 0x10);
    osCreateMesgQueue(romcopy_mesgq, D_800B2E48, 1);

    osCreatePiManager(0x96, D_800B2EA8, D_800B2E68, 0x10);

    a3 = 0xA4AA0 - 0xA4970;

    romcopy_dat = malloc(a3, 0x7F7F7FFF, NULL);
    possible_romcopy(0xA4970, romcopy_dat, a3);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/read_alloc_file.s")

#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/read_file.s")

#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/read_file_region.s")

#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/func_800372C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/get_file_size.s")

#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/possible_romcopy.s")
