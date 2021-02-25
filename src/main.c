#include "common.h"

void mainproc(void * arg)
{
    testWrite(); // ROM write check
    some_init_func();

    while(TRUE) {
        checkDongle();  // copy protection check
        
        if (osMemSize != EXPANSION_SIZE) {
            main_no_expPak();
        } else {
            main_expPak();
        }

        threadTimerTick();
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/some_init_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/main_expPak.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/main_no_expPak.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/main/four_mallocs.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014508.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014524.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/ret1_8001454c.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_initing_rumblepak.s")

/* 
 * 
*/
void testWrite(void) {
    HW_REG2(0x1C000C02, u16) = 0x4040;
}

/*
 * Probe the copy protection dongle for the correct magic string and
 * if failed, wipe a majority of RAM to prevent RAM viewing.
 */
void checkDongle(void) {
    // attempt to get the first magic short from the dongle. if it is
    // connected, this will retrieve correctly.
    u32 head = ACCESS_1;
    
    // append the other part.
    head <<= 16;
    head |= ACCESS_2;

    /* 
     * Perform the check against the 2 known codes:
     *
     * 'LSFS' (0x4C534653)
     * 'MPFS' (0x4D504653)
     *
     * It is not known which one the original dongle for this ROM
     * was intended to use.
     */
    if((head == 'LSFS') || (head == 'MPFS')) {
        return;
    }
    else {
        int *write = (int *)(u32)EXPANSION_RAM_START;
        // copy protection failed. Wipe every 2nd word to prevent RAM analysis.
        // probably hackers trying to view our precious data.
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
