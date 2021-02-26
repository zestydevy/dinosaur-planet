#include "common.h"

void func_8001440C(s32 arg0);
void func_80014C70(void);
void func_800483BC(f32, f32, s32);

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

void func_800142A0(f32 arg0, f32 arg1, s32 arg2) {
    func_8001440C(0);
    func_800483BC(arg0, arg1, arg2);
    func_80014C70();
    ossceduler_stack = 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800142F0.s")

void func_800143A4(void) {
    func_80048034();
    ossceduler_stack = 1;
}

s32 func_800143D0(s32 *arg0) {
    *arg0 = D_800AE678[D_800B09C1];
    return D_800AE680;
}

s8 func_800143FC(void) {
    return D_8008C940;
}

void func_8001440C(s32 arg0) {
    D_8008C940 = arg0;
}

s8 func_8001441C(void) {
    return D_800B09C2;
}

void func_8001442C(void) {
    D_800B09C2 = 0;
}

void func_8001443C(s32 arg0) {
    D_800B09C2 = arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/four_mallocs.s")

void func_80014508(s8 arg0) {
    D_8008C94C = arg0;
}

s32 func_80014524(void) {
    if (some_controller_init_val == -1) {
        return 1;
    } else {
        return 0;
    }
}

s32 ret1_8001454c(void) {
    return 1;
}

void func_initing_rumblepak(void) {
    osContStartQuery(0);
    osContGetQuery(0);
    osMotorInit(0, 0, 0);
    func_800683E0(0);
}

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

extern u8* osscheduler_;

void *get_ossched(void) {
    return &osscheduler_;
}

void func_80014614(void) {
    func_80012618(&D_800AE2A4, 0x37);
    D_800AE2A8 = func_80037310(0x37) >> 1;
    D_800AE2A0 = (*D_8008C9F4)->unk_88();
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014670.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800147EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014930.s")

s32 func_800149AC(s32 arg0) {
    s32 temp_v0 = func_800147EC(arg0);

    if (temp_v0 != 0) {
        func_80014670(arg0, --temp_v0);
        return temp_v0;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800149F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014A80.s")

void func_80014B1C(void) {
    if (D_8008CA04 == 0) {
        D_8008C9EC = make_function_struct(0x4B, 0xA);
        D_8008CA04 = make_function_struct(0x4C, 3);
    }
}

void func_80014B6C(void) {
    if (D_8008CA04 != 0) {
        func_8000C258(D_8008C9EC);
        D_8008C9EC = 0;
        func_8000C258(D_8008CA04);
        D_8008CA04 = 0;
    }
}

void func_80014BBC(void) {
    D_800B09C3 = 0;
    D_800B09C4 = 0;
}

void func_800142F0(s32, s32, s32);

void func_80014BD4(s32 arg0, s32 arg1, s32 arg2) {
    D_800B09C3++;

    if (D_800B09C3 >= 5) {
        D_800B09C3 = 0;
        D_800B09C4 = 1;
    }
    func_800142F0(arg0, arg1, arg2);
    if (arg2) {}; // fake match
}

s32 func_80014C28(void)
{
    s32 _v1 = D_800B09C3 + 1;
    if (_v1 >= 5)
        _v1 = 0;
    return _v1;
}

u8 func_80014C50(void)
{
    return D_800B09C3;
}

u8 func_80014C60(void)
{
    return D_800B09C4;
}

void func_80014C70(void)
{
    _bzero(&D_800AE2B0, 0x3C0);
    D_800AE670 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014CA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014D34.s")
