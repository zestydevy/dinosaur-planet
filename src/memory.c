#include "common.h"

#define ALIGN16(a) (((u32) (a) & ~0xF) + 0x10)

s32  increment_heap_block(s32, s32, s32, s32);
s32  find_heap_block(void *ptr);
void set_heap_block(Heap * heap, s32 size, s32 max);

void init_memory(void)
{
    u32 addr = (u32)&bss_end;

    int *mem = (int *)addr;
    while ((u32)mem < osMemSize)
        *mem++ = -1;

    gHeapBlkListSize = 0;

    if (osMemSize != EXPANSION_SIZE)
    {
        set_heap_block((void *)addr, HEAP_AREA_NO_EXPANSION - addr, 1200);
    }
    else
    {
        set_heap_block((void *)HEAP_AREA_00, RAM_END - HEAP_AREA_00, 400);
        set_heap_block((void *)HEAP_AREA_01, HEAP_AREA_00 - HEAP_AREA_01, 800);
        set_heap_block((void *)addr, HEAP_AREA_02 - addr, 1200);
    }

    func_80017254(2);

    pointerIntArrayCounter = 0;
}

// - Needs to use a multiplication to address the heapblock array
// - This function implies the existence of an `index` field in the 
//      HeapBlock struct, among several other s16 fields,
//      while find_heap_block deconfirms them.
#if 0
void _set_heap_block(Heap * heap, s32 size, s32 max)
{
    s32 i;
    struct HeapBlock * block = &gHeapBlkList[gHeapBlkListSize++];

    block->maxItems = max;
    block->itemCount = 0;
    block->ptr = heap;
    block->memAllocated = size;
    block->memUsed = 0;
    
    if (max > 0) {
        heap->index = i;
        for (i = 0; i < block->maxItems; ++i) {
            heap[i].index = i;
        }
    }

    /* 17648 80016A48 00CB0019 */  multu      $a2, $t3
    max * 0x14
    /* 1764C 80016A4C 8CE80008 */  lw         $t0, 8($a3)
    t0 = gHeapBlkList[gHeapBlkListSize].ptr
    /* 17650 80016A50 2401FFF0 */  addiu      $at, $zero, -0x10
    at = -0x10
    /* 17654 80016A54 00001812 */  mflo       $v1
    v1 = max * 0x14
    /* 17658 80016A58 00832021 */  addu       $a0, $a0, $v1
    a0 = heap + (max * 0x14)
    /* 1765C 80016A5C 308C000F */  andi       $t4, $a0, 0xf
    t4 = (heap + (max * 0x14)) & 0xF
    /* 17664 80016A64 00A37823 */   subu      $t7, $a1, $v1
    t7 = size - (max * 0x14)
    /* 17660 80016A60 11800005 */  beqz       $t4, .L80016A78
    if (((heap + (max * 0x14)) & 0xF) != 0) {
        /* 17668 80016A68 00816824 */  and        $t5, $a0, $at
        t5 = (heap + (max * 0x14)) & -0x10
        /* 1766C 80016A6C 25AE0010 */  addiu      $t6, $t5, 0x10
        t6 = ((heap + (max * 0x14)) & -0x10) + 0x10
        /* 17674 80016A74 AD0E0000 */   sw        $t6, ($t0)
        gHeapBlkList[gHeapBlkListSize].ptr->tail = ((heap + (max * 0x14)) & -0x10) + 0x10
        /* 17670 80016A70 10000002 */  b          .L80016A7C

        .L80016A78:
    } else {
        /* 17678 80016A78 AD040000 */  sw         $a0, ($t0)
        gHeapBlkList[gHeapBlkListSize].ptr->tail = heap + (max * 0x14)
    }
    .L80016A7C:
    /* 1767C 80016A7C 2404FFFF */  addiu      $a0, $zero, -1
    a0 = -1
    /* 17680 80016A80 AD0F0004 */  sw         $t7, 4($t0)
    HeapBlkList[gHeapBlkListSize].ptr->maxSize = size - (max * 0x14)
    /* 17684 80016A84 A5000008 */  sh         $zero, 8($t0)
    HeapBlkList[gHeapBlkListSize].ptr->m0008 = 0
    /* 17688 80016A88 A504000A */  sh         $a0, 0xa($t0)
    HeapBlkList[gHeapBlkListSize].ptr->m000A = -1
    /* 1768C 80016A8C A504000C */  sh         $a0, 0xc($t0)
    HeapBlkList[gHeapBlkListSize].ptr->m000C = -1
    /* 17690 80016A90 8CF80004 */  lw         $t8, 4($a3)
    t8 = gHeapBlkList[gHeapBlkListSize].itemCount
    /* 17694 80016A94 8CE20008 */  lw         $v0, 8($a3)
    v0 = gHeapBlkList[gHeapBlkListSize].ptr
    /* 17698 80016A98 27190001 */  addiu      $t9, $t8, 1
    /* 1769C 80016A9C ACF90004 */  sw         $t9, 4($a3)
    ++gHeapBlkList[gHeapBlkListSize].itemCount
    /* 176A0 80016AA0 03E00008 */  jr         $ra
    /* 176A4 80016AA4 00000000 */   nop
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/memory/set_heap_block.s")
#endif

void *malloc(s32 arg0, s32 arg1, s32 arg2) {
    void *v1;

    if (arg0 == 0) {
        get_stack_();
        v1 = NULL;
        return v1;
    }
    if ((arg0 >= 0x1194) || (osMemSize != 0x800000)) {
        v1 = increment_heap_block(0, arg0, arg1, arg2);
        if (v1 == NULL) {
            get_stack_();
            v1 = increment_heap_block(1, arg0, arg1, arg2);
        }
    } else if (arg0 >= 0x400) {
        v1 = increment_heap_block(1, arg0, arg1, arg2);
        if (v1 == NULL) {
            get_stack_();
            v1 = increment_heap_block(2, arg0, arg1, arg2);
        }
    } else {
        v1 = increment_heap_block(2, arg0, arg1, arg2);
    }
    if (v1 == NULL) {
        get_stack_();
    }
    return v1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory/reduce_heap_block.s")

// a mess, but it matches
int func_80016E68(void *a0)
{
    s32 i;
    s32 tmp;
    s32 *ptr;

    tmp = find_heap_block(a0);

    i = 0;
    tmp = (u32)gHeapBlkList[tmp].ptr;

    while (i != -1)
    {
        ptr = (s32 *)(tmp + (i * 0x14));
        i = *((s16 *)ptr + 6);

        if (a0 == (s32 *)ptr[0])
            return ptr[1];

        // repeated line
        i = *((s16 *)ptr + 6);
    }
    return -1;
}


#pragma GLOBAL_ASM("asm/nonmatchings/memory/increment_heap_block.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_8001707C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017254.s")

void free(void* p) {
    s32 sp1C = func_with_status_reg();
    if (D_800B179C == 0) {
        func_8001753C(p);
    } else {
        func_800175D4(p);
    }
    set_status_reg(sp1C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory/update_mem_mon_values.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_8001753C.s")

void func_800175D4(s32 a0)
{
    s16 *ptr1 = &pointerIntArrayCounter;
    pointerIntArray0[*ptr1].a = a0;
    pointerIntArray0[*ptr1].b[0] = D_800B179C;
    (*ptr1)++;
}

s32 find_heap_block(void *ptr)
{
    s32 i;

    for (i = 0; i < gHeapBlkListSize; i++)
    {
        if (((u32)ptr > (u32)gHeapBlkList[i].ptr))
            if ((u32)ptr < ((u32)gHeapBlkList[i].ptr + gHeapBlkList[i].memAllocated))
                return i;
    }

    return -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017674.s")

struct HeapBlock *func_80017790(s32 a0, s32 a1)
{
    return gHeapBlkList[a0].ptr;
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_800177B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_800178CC.s")

u32 align_16(u32 a0)
{
    s32 tmp = a0 & 0xF;

    if (tmp > 0)
        a0 += 0x10 - tmp;

    return a0;
}

u32 align_8(u32 a0) {
    s32 tmp;

    tmp = a0 & 7;

    if (tmp > 0)
        a0 += 8 - tmp;

    return a0;
}

u32 align_4(u32 a0) {
    s32 tmp;

    tmp = a0 & 3;

    if (tmp > 0)
        a0 += 4 - tmp;

    return a0;
}

u32 align_2(u32 a0) {
    s32 tmp;

    tmp = a0 & 1;

    if (tmp > 0)
        a0 += 2 - tmp;

    return a0;
}


s32 dbg_heap_print(s32 arg0)
{
    dummied_print_func(
        &D_800991E0, 
        memMonVal0 / 0x400,
        gHeapBlkList[0].memAllocated / 0x400, 
        memMonVal1 / 0x400, 
        gHeapBlkList[1].memAllocated / 0x400, 
        memMonVal2 / 0x400, 
        gHeapBlkList[2].memAllocated / 0x400, 
        gHeapBlkList[0].itemCount, 
        gHeapBlkList[0].maxItems, 
        gHeapBlkList[1].itemCount, 
        gHeapBlkList[1].maxItems, 
        gHeapBlkList[2].itemCount, 
        gHeapBlkList[2].maxItems
    );

    return memMonVal0 + memMonVal1 + memMonVal2;
}


#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017B3C.s")

extern u32 D_80099228;
void *_alHeapAlloc(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    void *ptr;

    arg4 = ALIGN16((arg4 * arg3) + 0xF);

    // ??
    if (arg4);
    if (arg4);

    ptr = malloc(arg4, 0xB, &D_80099228);
    bzero(ptr, arg4);
    return align_16(ptr);
}

