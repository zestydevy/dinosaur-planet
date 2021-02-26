#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "ultra64.h"
#include <PR/sched.h>

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
extern struct UnkStruct80014614
            **D_8008C970, **D_8008C974, **D_8008C978, **D_8008C97C,
            **D_8008C980, **D_8008C984, **D_8008C988, **D_8008C98C,
            **D_8008C990, **D_8008C994, **D_8008C998, **D_8008C99C,
            **D_8008C9A0, **D_8008C9A4, **D_8008C9A8, **D_8008C9AC,
            **D_8008C9B0, **D_8008C9B4, **D_8008C9B8, **D_8008C9BC,
            **D_8008C9C0, **D_8008C9C4, **D_8008C9C8, **D_8008C9CC,
            **D_8008C9D0, **D_8008C9D4, **D_8008C9D8, **D_8008C9DC,
            **D_8008C9E0, **D_8008C9E4, **D_8008C9E8, **D_8008C9EC,
            **D_8008C9F0, **D_8008C9F4, **D_8008C9F8, **D_8008C9FC,
            **D_8008CA00, **D_8008CA04, **D_8008CA08, **D_8008CA0C,
            **D_8008CA10, **D_8008CA14;
extern u8 alSyn_flag;

extern OSSched *osscheduler_;
extern s32 D_800AE2A0;
extern s32 D_800AE2A4;
extern s16 D_800AE2A8;
extern s32 D_800AE678[], D_800AE688[], D_800AE698[], D_800AE6A8[];
extern Gfx *D_800AE680;

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
