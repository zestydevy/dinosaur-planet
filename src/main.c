#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainproc.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/some_init_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/has_exppak_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/init_hasnt_expPak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80013D80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80013FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014074.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800141A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800142A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800142F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800143A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800143D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800143FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8001440C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8001441C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8001442C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8001443C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/four_funcs_checking_expPak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014508.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014524.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_8001454C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_initing_rumblepak.s")

void testWrite(void) {
    HW_REG2(0x1C000C02, u16) = 0x4040;
}

void checkDongle(void)
{
    u32 head = ACCESS(0x1C000000, s16);

    head <<= 16;
    head |= ACCESS(0x1C000002, s16);

    if((head == 0x4C534653)  // 'LSFS'
    || (head == 0x4D504653)) // 'MPFS'
        return; // if either of these, dont do the EXP ram write below I guess.
    else {
        int *write = (int *)(u32)EXPANSION_RAM_START;
        while((u32)write != 0x80000000) {
            *write = 0;
            write -= 2; // hmm...
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/get_ossched.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014614.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014670.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800147EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014930.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800149AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800149F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014A80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014B1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014B6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014BBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014BD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014C28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014C50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014C60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014C70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014CA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014D34.s")
