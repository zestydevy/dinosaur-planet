#include "common.h"

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
        set_heap_block((void *)addr, (void *)(0x80119000 - addr), 1200);
    }
    
    func_80017254(2);
    
    D_800B1798 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/memory/set_heap_block.s")


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

#pragma GLOBAL_ASM("asm/nonmatchings/memory/reduce_heap_block_.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80016E68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/increment_heap_block.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_8001707C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017254.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_800172E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/update_mem_mon_values.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_8001753C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_800175D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/find_heap_block.s")

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
        a0 = (a0 - tmp) + 4;

    return a0;
}

u32 align_2(u32 a0) {
    s32 tmp;

    tmp = a0 & 1;

    if (tmp > 0)
        a0 = (a0 - tmp) + 2;

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

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017BA8.s")
