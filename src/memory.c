#include "common.h"
#include "sys/interrupt_util.h"

#define ALIGN16(a) (((u32) (a) & ~0xF) + 0x10)

s32  increment_heap_block(s32 heap, s32 size, s32 tag, const char *name);
s32  find_heap_block(void *ptr);

void init_memory(void)
{
    u32 addr = (u32)&bss_end;

    int *mem = (int *)addr;
    while ((u32)mem < osMemSize)
        *mem++ = -1;

    gHeapListSize = 0;

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

HeapBlock * set_heap_block(HeapBlock * blocks, s32 size, s32 maxBlocks)
{
    HeapBlock* entryBlock;
    Heap * heap;
    s32 i;
    s32 len = gHeapListSize++;


    heap = &gHeapList[len];
    len = maxBlocks * sizeof(HeapBlock);

    heap->maxBlocks = maxBlocks;
    heap->blockCount = 0;
    heap->blocks = blocks;
    heap->memAllocated = size;
    heap->memUsed = 0;
    
    entryBlock = blocks;
    
    for (i = 0; i < heap->maxBlocks; i++) {
        entryBlock->index = i;
        entryBlock++;
    }

    entryBlock = &heap->blocks[0];
    blocks += maxBlocks;
    if (((s32) blocks & 0xF) != 0) {
        entryBlock->data = ALIGN16(blocks);
    } else {
        entryBlock->data = blocks;
    }

    entryBlock->maxSize = size - len;
    entryBlock->flags = 0;
    entryBlock->prevIdx = -1;
    entryBlock->nextIdx = -1;

    ++heap->blockCount;
    
    return heap->blocks;
}

void *malloc(s32 size, s32 tag, const char *name) {
    void *v1;

    if (size == 0) {
        get_stack_();
        v1 = NULL;
        return v1;
    }
    if ((size >= 0x1194) || (osMemSize != 0x800000)) {
        v1 = increment_heap_block(0, size, tag, name);
        if (v1 == NULL) {
            get_stack_();
            v1 = increment_heap_block(1, size, tag, name);
        }
    } else if (size >= 0x400) {
        v1 = increment_heap_block(1, size, tag, name);
        if (v1 == NULL) {
            get_stack_();
            v1 = increment_heap_block(2, size, tag, name);
        }
    } else {
        v1 = increment_heap_block(2, size, tag, name);
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
    tmp = (u32)gHeapList[tmp].blocks;

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
    s32 prevIE = interrupts_disable();
    if (D_800B179C == 0) {
        func_8001753C(p);
    } else {
        func_800175D4(p);
    }
    interrupts_enable(prevIE);
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

    for (i = 0; i < gHeapListSize; i++)
    {
        if (((u32)ptr > (u32)gHeapList[i].blocks))
            if ((u32)ptr < ((u32)gHeapList[i].blocks + gHeapList[i].memAllocated))
                return i;
    }

    return -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017674.s")

struct HeapBlock *func_80017790(s32 a0, s32 a1)
{
    return gHeapList[a0].blocks;
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
    diPrintf(
        &D_800991E0, 
        memMonVal0 / 0x400,
        gHeapList[0].memAllocated / 0x400, 
        memMonVal1 / 0x400, 
        gHeapList[1].memAllocated / 0x400, 
        memMonVal2 / 0x400, 
        gHeapList[2].memAllocated / 0x400, 
        gHeapList[0].blockCount, 
        gHeapList[0].maxBlocks, 
        gHeapList[1].blockCount, 
        gHeapList[1].maxBlocks, 
        gHeapList[2].blockCount, 
        gHeapList[2].maxBlocks
    );

    return memMonVal0 + memMonVal1 + memMonVal2;
}


#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017B3C.s")

const char s_mm_audioheap[] = "mm:audioheap";
void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size) {
    void *ptr;

    size = ALIGN16((size * num) + 0xF);

    // ??
    if (size);
    if (size);

    ptr = malloc(size, ALLOC_TAG_AUDIO_COL, s_mm_audioheap);
    bzero(ptr, size);
    return align_16(ptr);
}

