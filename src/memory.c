#include "common.h"

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/memory/wipe_mem_past_bss.s")
#else
void wipe_mem_past_bss()
{
    if ((u32)bss_end < osMemSize) {
        int * mem = (int *)bss_end;
        while ((u32)mem < osMemSize) {
            *mem = -1;
            ++mem;
        }
    }
    
    heap_block_array_size = 0;

    if (osMemSize != EXPANSION_SIZE) {
        set_heap_block((void *)bss_end, 0x802D4000 - (u32)bss_end, 1200);
    } else {
        set_heap_block((void *)0x8042C000, 0x3D4000, 400);
        set_heap_block((void *)0x80245000, 0x1E7000, 800);
        set_heap_block((void *)bss_end, 0x80119000 - (u32)bss_end, 1200);
    }
    
    func_80017254(2);
    
    D_800B1798 = 0;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/memory/set_heap_block.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/malloc.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_800179A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_800179C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_800179E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017A04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/some_memory_monitor.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017B3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/memory/func_80017BA8.s")
