#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "ultra64.h"

#include "constants.h"
#include "common_structs.h"

typedef u8 UNK_TYPE_8;

struct HeapBlock {
    s32 items_max;
    s32 items_count;
    struct HeapBlock *ptr;
    s32 mem_allocated;
    // s16 index;
    s32 mem_used;
};

extern OSThread* __osRunningThread;
extern OSThread* __osRunQueue;
// this needs double checking. its address is within gMainThreadStack....
extern u8 gIdleThreadStack[IDLE_THREAD_SIZE];
extern OSThread gIdleThread;
extern OSThread gMainThread;
extern u64 gMainThreadStack[];        // some sort of data

extern u8 D_8008C940;
extern s32 D_8008C9EC;
extern UnkStruct80014614** D_8008C9F4;
extern s32 D_8008CA04;

extern s32 D_800AE2A0;
extern s32 D_800AE2A4;
extern s16 D_800AE2A8;
extern s32 D_800AE678[];
extern s32 D_800AE680;

extern u32 * bss_end;
extern u16 D_800B1798;
extern u8 heap_block_array_size;
extern struct HeapBlock heap_block_array[];
extern s8 ossceduler_stack;
extern u8 D_800B09C1;
extern u8 D_800B09C3;
extern u8 D_800B09C2;
extern u8 D_800B09C4;
extern u8 D_8008C94C;
extern s32 D_800B179C;
extern s8 some_controller_init_val;
extern u32 D_800AE670;
extern UNK_TYPE_8 D_800AE2B0;




#endif
