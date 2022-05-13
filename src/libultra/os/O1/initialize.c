#include <PR/os.h>
#include <PR/os_internal.h>
#include <PR/R4300.h>
#include <PR/rcp.h>

#define PIF_COMMAND_BYTE (PIF_RAM_START+0x3c)

typedef struct
{
   /* 0x0 */ unsigned int inst1;
   /* 0x4 */ unsigned int inst2;
   /* 0x8 */ unsigned int inst3;
   /* 0xC */ unsigned int inst4;
} __osExceptionVector;

typedef int (*some_p64_func)(int, int);

extern __osExceptionVector __osExceptionPreamble;
extern u32 __ptExceptionPreamble[];

OSTime osClockRate = OS_CLOCK_RATE;
s32 osViClock = VI_NTSC_CLOCK;
u32 __osShutdown = 0;
u32 __OSGlobalIntMask = OS_IM_ALL;
s32 __kmc_pt_mode;

void __createSpeedParam();

void osInitialize() {
    u32 pifdata;
    u32 clock; // unused
    some_p64_func p64Func;
    u32 *ptPreamblePtr;
    u32 *eVecPtr;
    u32 p64Struct_Dst;
    u32 *p64Struct;
    u32 *p64Status;
    u32 *p64Src;
    u32 *p64Dst;
    u32 p64WordsLeft;

    clock = 0;

    // enable fpu
    __osSetSR(__osGetSR() | SR_CU1);
    // flush denorm to zero, enable invalid operation
    __osSetFpcCsr(FPCSR_FS | FPCSR_EV);

    // terminate boot process
    while (__osSiRawReadIo(PIF_COMMAND_BYTE, &pifdata));
    while (__osSiRawWriteIo(PIF_COMMAND_BYTE, pifdata | 8));

    // copy exception preamble to exception vectors
    *(__osExceptionVector *)UT_VEC = __osExceptionPreamble;
    *(__osExceptionVector *)XUT_VEC = __osExceptionPreamble;
    *(__osExceptionVector *)ECC_VEC = __osExceptionPreamble;
    *(__osExceptionVector *)E_VEC = __osExceptionPreamble;

    // flush exception vectors from cache
    osWritebackDCache((void *)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    osInvalICache((void *)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));

    // TODO:
    __createSpeedParam();

    // map TLB for remote debug port
    osUnmapTLBAll();
    osMapTLBRdb();

    // bus clock -> cpu counter speed
    osClockRate = osClockRate * 3 / 4;

    if (osResetType == 0) {
        // cold reset, clear post-NMI buffer
        bzero(osAppNMIBuffer, OS_APP_NMI_BUFSIZE);
    }

    // set up correct video clock
    if (osTvType == OS_TV_PAL) {
        osViClock = VI_PAL_CLOCK;
    } else if (osTvType == OS_TV_MPAL) {
        osViClock = VI_MPAL_CLOCK;
    } else {
        osViClock = VI_NTSC_CLOCK;
    }

    // halt if pre-NMI (IP5, aka. INT2, maps to pre-NMI)
    if (__osGetCause() & CAUSE_IP5) {
        while (1) {}
    }

    // enable audio DMA
    IO_WRITE(AI_CONTROL_REG, AI_CONTROL_DMA_ON);
    // set up audio default rates
	IO_WRITE(AI_DACRATE_REG, AI_MAX_DAC_RATE - 1);
	IO_WRITE(AI_BITRATE_REG, AI_MAX_BIT_RATE - 1);

    // partner-64 stuff
    // note: not 100% sure what the below is doing, some comments may be wrong
    if (__kmc_pt_mode == 0) {
        p64Status = (u32 *)0xBFF08004;
        p64Struct = (u32 *)0xBFF00000;
        
        if (*p64Struct != 0x4B4D4300) {
            // already initialized/no partner-64 hooked up?
            return;
        }

        // copy partner exception preamble to exception vector
        ptPreamblePtr = (u32 *)__ptExceptionPreamble;
        eVecPtr = (u32 *)E_VEC;
        
        *(eVecPtr++) = *(ptPreamblePtr++);
        *(eVecPtr++) = *(ptPreamblePtr++);
        *(eVecPtr++) = *(ptPreamblePtr++);
        ptPreamblePtr += 2; // exclude two instructions (these are nops in the partner preamble)
        eVecPtr += 2;
        *(eVecPtr++) = *(ptPreamblePtr++);
        *(eVecPtr++) = *(ptPreamblePtr++);
        *(eVecPtr++) = *(ptPreamblePtr++);

        // flush exception vector from cache
        osWritebackDCache((void *)E_VEC, 0x24);
        osInvalICache((void *)E_VEC, 0x24);

        // mark partner mode as enabled
        __kmc_pt_mode = 1;

        // check some status bit
        if ((*p64Status & 0x10) == 0) {
            p64Struct_Dst = *(p64Struct + 1);

            if (p64Struct_Dst != 0xBFF00000) {
                // copy 8192 bytes of something
                p64Dst = (u32 *)(p64Struct_Dst | 0x20000000);
                p64WordsLeft = 2048;
                p64Src = (u32 *)0xBFF00000;

                while (p64WordsLeft != 0) {
                    *(p64Dst++) = *(p64Src++);
                    p64WordsLeft--;
                }
            }

            // call some func with the same value used above for skipping all this
            p64Func = (some_p64_func)(p64Struct_Dst + 8);
            (*p64Func)(0x4B4D4300, 0);
        }
    }
}

void func_8007CC9C() {}

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/os/O1/initialize/__createSpeedParam.s")
