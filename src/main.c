#include "common.h"
#include <PR/sched.h>

void func_8001440C(s32 arg0);
void func_80014C70(void);
void func_800483BC(f32, f32, s32);
void some_init_func(void);
void func_80014614(void);
struct UnkStruct80014614 **make_function_struct(s32, s32);

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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/main/some_init_func.s")
#else
void osCreateScheduler(OSSched *s, void *stack, OSPri priority, u8 mode, u8 retreceCount);
void _some_init_func(void) {
    struct UnkStruct80014614 *temp_v0;
    u8 phi_v0;
    struct UnkStruct80014614 **tmp3;

    initMemory();
    three_more_mallocs();
    create_some_thread();

    // this comparison is wrong
    if (osTvType == 0) {
        phi_v0 = 0x10;
    } else if (osTvType == 2) {
        phi_v0 = 0x1E;
    } else {
        phi_v0 = 2;
    }
    osCreateScheduler(&osscheduler_, 0x800B09C0, 0xD, phi_v0, 1);
    PiManager_thread_func();
    piMan_creator();
    create_3_megs_quues(&osscheduler_);
    four_mallocs();
    D_800B09C1 = 0;
    D_800AE680 = D_800AE678[D_800B09C1];
    some_controller_init_val = init_controller_data();
    controller_thread_func(&osscheduler_);
    crash_thread_func(&osscheduler_);
    func_8003CC90();
    func_80041E84();
    func_8001CD00();
    func_80017C20();
    func_8000BC40();
    func_80020550();
    func_80060A80();
    func_80053300();
    func_8004D470();
    func_8005C780();
    func_8000C680();
    clear_several_values();
    al_thread_init(&osscheduler_, 0xE);
    func_8004676C();
    if (osMemSize != 0x800000) {
        temp_v0 = make_function_struct(5, 0x24);
        D_8008C9A0 = D_8008C99C = temp_v0;
        D_8008C9A4 = make_function_struct(6, 0x12);
        D_8008C9D0 = make_function_struct(0x15, 5);
        D_8008C9F4 = make_function_struct(0x1D, 0x2F);
        D_8008CA00 = make_function_struct(0x1F, 2);
        D_8008C974 = make_function_struct(0x1C, 4);
    } else {
        D_8008C994 = make_function_struct(1, 0xF);
        D_8008C978 = make_function_struct(2, 0x17);
        D_8008C9D8 = make_function_struct(0x17, 8);
        D_8008C9C8 = make_function_struct(0x12, 0x16);
        D_8008C97C = make_function_struct(3, 0x1D);
        D_8008C974 = make_function_struct(0x1C, 4);
        D_8008C9E0 = make_function_struct(0x19, 0xE);
        D_8008C980 = make_function_struct(7, 0xF);
        D_8008C984 = make_function_struct(8, 0xC);
        D_8008C988 = make_function_struct(9, 8);
        D_8008C98C = make_function_struct(0xA, 3);
        D_8008C990 = make_function_struct(0xC, 0xA);
        D_8008C998 = make_function_struct(4, 0xD);
        temp_v0 = make_function_struct(5, 0x24);
        D_8008C9A0 = D_8008C99C = temp_v0;
        D_8008C9A4 = make_function_struct(6, 0x12);
        D_8008C9A8 = make_function_struct(0xB, 7);
        D_8008C9B4 = make_function_struct(0xD, 0xA);
        D_8008C9B8 = make_function_struct(0xE, 0xC);
        D_8008C9BC = make_function_struct(0xF, 8);
        D_8008C9C0 = make_function_struct(0x10, 3);
        D_8008C9C4 = make_function_struct(0x11, 2);
        D_8008C9CC = make_function_struct(0x14, 3);
        D_8008C9D0 = make_function_struct(0x15, 5);
        D_8008C9D4 = make_function_struct(0x16, 7);
        D_8008C9DC = make_function_struct(0x18, 7);
        D_8008C9E4 = make_function_struct(0x1A, 0x26);
        D_8008C9E8 = make_function_struct(0x4A, 7);
        D_8008C9F0 = make_function_struct(0x1B, 9);
        D_8008C9F4 = make_function_struct(0x1D, 0x24);
        D_8008C9F8 = make_function_struct(0x38, 0xA);
        D_8008C9FC = make_function_struct(0x1E, 6);
        D_8008CA00 = make_function_struct(0x1F, 2);
        D_8008CA08 = make_function_struct(0x20, 6);
        D_8008CA0C = make_function_struct(0x21, 0x16);
        D_8008CA10 = make_function_struct(0x3B, 2);
        D_8008CA14 = make_function_struct(0x36, 0xC);
        D_8008C9AC = make_function_struct(0x39, 4);
        D_8008C9B0 = make_function_struct(0x3A, 2);
        (*D_8008C9FC)->unk4.asVoid();
    }
    func_80014614();
    alSyn_flag = 1;
    start_alSyn_thread();
    func_80012224(0);

    gDPFullSync(D_800AE680++);
    gSPEndDisplayList(D_800AE680++);
    func_800632B0();
    func_8000F404(2);
    if (osMemSize == EXPANSION_SIZE) {
        func_80014074();
    }
    func_80041D20(0);
    func_80041C6C(0);
}
#endif

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

// These smell like structs
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/main/four_mallocs.s")
#else
void _four_mallocs(void) {
    u8 *temp_v0;
    s32 tmp2;

    temp_v0 = (u8 *) malloc(0x11940, 1, 0);
    D_800AE678[0] = temp_v0;
    D_800AE678[1] = temp_v0 + 0x8CA0;
    temp_v0 = (u8 *) malloc(0x22600, 1, 0);
    D_800AE688[0] = temp_v0;
    D_800AE688[1] = temp_v0 + 0x11300;
    temp_v0 = (u8 *) malloc(0x640, 1, 0);
    D_800AE6A8[0] = temp_v0;
    D_800AE6A8[1] = temp_v0 + 0x320;
    temp_v0 = (u8 *) malloc(0x2580, 1, 0);
    D_800AE698[0] = temp_v0;
    D_800AE698[1] = temp_v0 + 0x12C0;
}
#endif


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

OSSched *get_ossched(void) {
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
