#include "common.h"

#define ALIGN16(a) (((u32) (a) & ~0xF) + 0x10)

s32  increment_heap_block(s32, s32, s32, s32);
s32  find_heap_block(void *ptr);
void set_heap_block(struct HeapBlock *heap, s32 size, s32 max);

void initMemory(void)
{
    u32 addr = (u32)&bss_end;

    int *mem = (int *)addr;
    while ((u32)mem < osMemSize)
        *mem++ = -1;

    heap_block_array_size = 0;

    if (osMemSize != EXPANSION_SIZE)
    {
        set_heap_block((void *)addr, 0x802D4000 - addr, 1200);
    }
    else
    {
        set_heap_block((void *)0x8042C000, 0x3D4000, 400);
        set_heap_block((void *)0x80245000, 0x1E7000, 800);
        set_heap_block((void *)addr, 0x80119000 - addr, 1200);
    }

    func_80017254(2);

    D_800B1798 = 0;
}

// - Needs to use a multiplication to address the heapblock array
// - This function implies the existence of an `index` field in the 
//      HeapBlock struct, among several other s16 fields,
//      while find_heap_block deconfirms them.
#if 0
void _set_heap_block(struct HeapBlock *heap, s32 size, s32 max) {
    struct HeapBlock *temp_a0;
    struct HeapBlock *temp_t0;
    struct HeapBlock *temp_a3;
    s32  phi_t1;
    struct HeapBlock *phi_t0;

    temp_a3 = &heap_block_array[heap_block_array_size++];
    temp_a3->items_max = max;
    temp_a3->items_count = 0;
    temp_a3->ptr = heap;
    temp_a3->mem_allocated = size;
    temp_a3->mem_used = 0;
    if (max > 0) {
        for (phi_t1 = 0; phi_t1 < temp_a3->items_max; phi_t1++) {
            heap[phi_t1].index = phi_t1;
        }
    }
    temp_t0 = temp_a3->ptr;
    temp_a0 = &heap[max];
    if (((u32)temp_a0 & 0xF) != 0) {
        temp_t0->items_max = ALIGN16(temp_a0);
    } else {
        temp_t0->items_max = temp_a0;
    }
    temp_t0->items_count = size - (max * 0x14);
    temp_t0->ptr = NULL;
    temp_t0->index = -1;
    temp_t0->mem_allocated = -1;
    temp_a3->items_count++;
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
    tmp = (u32)heap_block_array[tmp].ptr;

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

void free(s32 arg0) {
    s32 sp1C = func_with_status_reg();
    if (D_800B179C == 0) {
        func_8001753C(arg0);
    } else {
        func_800175D4(arg0);
    }
    set_status_reg(sp1C);
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory/update_mem_mon_values.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_8001753C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_800175D4.s")

s32 find_heap_block(void *ptr)
{
    s32 i;

    for (i = 0; i < heap_block_array_size; i++)
    {
        if (((u32)ptr > (u32)heap_block_array[i].ptr))
            if ((u32)ptr < ((u32)heap_block_array[i].ptr + heap_block_array[i].mem_allocated))
                return i;
    }

    return -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017674.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017790.s")

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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/memory/some_memory_monitor.s")
#else
/*
 * debug memory monitor (ala SM64?)
 * uses a seemingly dummied out print function (could it possibly be restored?)
 *
*/
s32 some_memory_monitor(s32 arg0) {
    dummied_print_func(&D_800991E0, mem_mon_val_0 / 0x400,
                        heap_block_array.unkC / 0x400, 
                        mem_mon_val_1 / 0x400, 
                        heap_block_array.unk20 / 0x400, 
                        mem_mon_val_2 / 0x400, 
                        heap_block_array.unk34 / 0x400, 
                        heap_block_array.unk4, 
                        heap_block_array.unk0, 
                        heap_block_array.unk18, 
                        heap_block_array.unk14, 
                        heap_block_array.unk2C, 
                        heap_block_array.unk28);
    return mem_mon_val_0 + mem_mon_val_1 + mem_mon_val_2;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017B3C.s")

// those empty statements get everything into place, except stack
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/memory/alHeapAlloc.s")
#else
extern u32 D_80099228;
void *func_80017BA8(s32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4) {
    s32 temp_s0;
    void *temp_v0;

    temp_s0 = ALIGN16((arg4 * arg3) + 0xF);

    if (!temp_s0) {
        if (arg4) {

        }
    }

    if (temp_s0) {
        
    }

    temp_v0 = malloc(temp_s0, 0xB, &D_80099228);
    _bzero(temp_v0, temp_s0);
    return align_16(temp_v0);
}
#endif
