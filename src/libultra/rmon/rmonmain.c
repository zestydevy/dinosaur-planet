// @DECOMP_OPT_FLAGS=-O1
#include "libultra/rmon/rmonint.h"

int __rmonActive = 0;

extern volatile u32 D_800D3EF0;
extern s32 rmonmain_bss_0008[280];
extern u8 rmonmain_bss_0468;
extern u8 rmonmain_bss_0469;
extern s32 rmonmain_bss_046C;
extern s32 *rmonmain_bss_0470;

const char str_800a2dc0[] = "rmon: Thread %d created\n";
const char str_800a2ddc[] = "rmon: Thread %d destroyed\n";

void __rmonSendHeader(KKHeader* const block, u32 blockSize, u32 type) {
    u32 local_4;
    u8 *blockPtr;
    
    blockPtr = (u8*)block;

    block->rev = 2;
    block->type = (char)type;


    local_4 = 0;
    while (local_4 < blockSize) {
        local_4 += __osRdbSend(&blockPtr[local_4], blockSize - local_4, 8);
    }
}

void __rmonSendReply(KKHeader* const block, u32 blockSize, u32 replyType) {
    u8 *blockSizeAddr;
    s32 local_8;

    local_8 = 0;

    block->length = blockSize;
    blockSizeAddr = (u8*)&blockSize;

    while (local_8 < 4) {
        local_8 += __osRdbSend(&blockSizeAddr[local_8], 4 - local_8, 8);
    }

    __rmonSendHeader(block, blockSize, replyType);
    __rmonIOflush();
}

void __rmonSendData(char* const block, unsigned int blockSize) {
    u32 *blockPtr;
    u32 local_8;
    u32 local_c;
    u32 local_10;

    blockPtr = (u32*)block;
    local_8 = blockSize + 3 >> 2;

    if (((u32)block & 3) == 0) {
        while (local_8--) {
            if ((u32)blockPtr >= 0x4000000 && (u32)blockPtr <= 0x4ffffff) {
                __osSpRawReadIo((u32)blockPtr++, &local_c);
                __rmonIOputw(local_c);
            } else {
                __rmonIOputw(*blockPtr++);
            }
        }
    } else {
        while (local_8--) {
            __rmonMemcpy((u8*)&local_10, (u8*)blockPtr, 4);
            __rmonIOputw(local_10);
            blockPtr += 1;
        }
    }

    __rmonIOflush();
}

// This is declared differently than in rmon.h...
void rmonMain() {
    register s32 s0;
    OSMesg local_8;

    D_800D3EF0 = 0;
    rmonmain_bss_0469 = 0;
    rmonmain_bss_0468 = 0;

    __rmonInit();
    __rmonActive = 1;

    rmonmain_bss_046C = 0;
    rmonmain_bss_0470 = rmonmain_bss_0008;
    s0 = 0;

    while (1) {
        osRecvMesg(&__rmonMQ, &local_8, OS_MESG_BLOCK);
        D_800D3EF0 |= (u32)local_8;

        if (D_800D3EF0 & 2) {
            D_800D3EF0 &= ~2;
            __rmonHitBreak();
        }

        if (D_800D3EF0 & 4) {
            D_800D3EF0 &= ~4;
            __rmonHitSpBreak();
        }

        if (D_800D3EF0 & 8) {
            D_800D3EF0 &= ~8;
            __rmonHitCpuFault();
        }

        if (D_800D3EF0 & 0x10) {
	        (D_800D3EF0);
            D_800D3EF0 &= 0xef;
        }

        if (D_800D3EF0 & 0x20) {
            (D_800D3EF0);
            D_800D3EF0 &= 0xdf;
        }
    }
}
