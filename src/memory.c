#include "PR/libaudio.h"
#include "PR/os.h"
#include "libultra/audio/synthInternals.h"
#include "sys/memory.h"
#include "sys/interrupt_util.h"
#include "sys/print.h"
#include "macros.h"

extern s32 get_stack_();

#define ALIGN16(a) (((u32)(a) & 0xF) ? (((u32)(a) & ~0xF) + 0x10) : (u32)(a))
#define S32_MAX 0x7FFFFFFF

// An extra 8 bytes appended to every slot's data to, presumably, detect memory corruption
#define SLOT_CANARY_SIZE 8
#define SLOT_CANARY_VALUE -1

// Note: Most of the function names here come from the Jet Force Gemini kiosk symbols,
// however some were made up for the sake of completeness

/* -------- .data start -------- */
s32 memMonVal0 = 0;
s32 memMonVal1 = 0;
s32 memMonVal2 = 0;
/* -------- .data end -------- */

// TODO: bss size for this file is correct but IDO reorders it
/* -------- .bss start 800b09d0 -------- */
MemoryPool gMemoryPools[8];
u8 gNumMemoryPools;
MemoryFreeQueueElement gMemoryFreeQueue[420];
s16 gMemoryFreeQueueLength;
s32 gMemoryFreeDelay;
/* -------- .bss end 800b17a0 -------- */

void mmInit(void) {
    // Start pool memory at the end of .bss
    u32 startAddr = (u32)&bss_end;

    // Clear to 0xFF
    s32 *mem = (s32*)startAddr;
    while ((u32)mem < osMemSize)
        *mem++ = -1;

    // Initialize memory pools
    gNumMemoryPools = 0;

    if (osMemSize != EXPANSION_SIZE)
    {
        mmInitPool((void *)startAddr, MEM_POOL_AREA_NO_EXPANSION - startAddr, 1200);
    }
    else
    {
        mmInitPool((void *)MEM_POOL_AREA_00, RAM_END - MEM_POOL_AREA_00,          400);
        mmInitPool((void *)MEM_POOL_AREA_01, MEM_POOL_AREA_00 - MEM_POOL_AREA_01, 800);
        mmInitPool((void *)startAddr,        MEM_POOL_AREA_02 - startAddr,        1200);
    }

    mmSetDelay(2);

    gMemoryFreeQueueLength = 0;
}

MemoryPoolSlot *mmInitPool(MemoryPoolSlot *slots, s32 size, s32 maxSlots) {
    MemoryPoolSlot *slot;
    MemoryPool *pool;
    s32 i;
    s32 len = gNumMemoryPools++;

    pool = &gMemoryPools[len];
    len = maxSlots * sizeof(MemoryPoolSlot);

    pool->maxSlots = maxSlots;
    pool->numSlots = 0;
    pool->slots = slots;
    pool->memAllocated = size; // @bug ? shouldn't this exclude the size taken up by the slot headers?
    pool->memUsed = 0;
    
    slot = slots;
    
    for (i = 0; i < pool->maxSlots; i++) {
        slot->index = i;
        slot++;
    }

    slot = &pool->slots[0];
    slots += maxSlots;
    slot->data = (void*)ALIGN16(slots);

    slot->size = size - len;
    slot->flags = SLOT_FREE;
    slot->prevIndex = MEMSLOT_NONE;
    slot->nextIndex = MEMSLOT_NONE;

    ++pool->numSlots;
    
    return pool->slots;
}

void *mmAlloc(s32 size, s32 tag, const char *name) {
    void *ptr;

    if (size == 0) {
        get_stack_();
        ptr = NULL;
        return ptr;
    }
    if ((size >= 4500) || (osMemSize != EXPANSION_SIZE)) {
        // >= 4500 bytes -> pool 0
        ptr = mmAllocR(0, size, tag, name);
        if (ptr == NULL) {
            get_stack_();
            // @bug ? If no expansion pack is in, memory pool 1 won't exist
            ptr = mmAllocR(1, size, tag, name);
        }
    } else if (size >= 1024) {
        // >= 1024 bytes -> pool 1
        ptr = mmAllocR(1, size, tag, name);
        if (ptr == NULL) {
            get_stack_();
            ptr = mmAllocR(2, size, tag, name);
        }
    } else {
        // < 1024 bytes -> pool 2
        ptr = mmAllocR(2, size, tag, name);
    }
    if (ptr == NULL) {
        get_stack_();
    }
    return ptr;
}

void *mmRealloc(void *address, s32 newSize, const char *name) {
    MemoryPoolSlot *slots;
    s32 _pad;
    s32 _pad2;
    s32 poolIndex;
    s32 nextIndex;
    s32 slotIndex;
    s32 size;
    s32 lastSize;
    u32 *canary;

    if (newSize <= 0) {
        get_stack_();
        return NULL;
    }
    newSize = (s32)ALIGN16(newSize);
    newSize = newSize + SLOT_CANARY_SIZE;
    // find pool that the address is in
    poolIndex = mmFindPool(address);
    if (poolIndex == -1) {
        return NULL;
    }
    slots = gMemoryPools[poolIndex].slots;
    // find the address's existing slot, if any
    slotIndex = 0;
    while (slotIndex != MEMSLOT_NONE) {
        if (address == (slots + slotIndex)->data) {
            break;
        }
        slotIndex = (slots + slotIndex)->nextIndex;
    }
    if (slotIndex != MEMSLOT_NONE) {
        gMemoryPools[poolIndex].memUsed -= (slots + slotIndex)->size;
        gMemoryPools[poolIndex].memUsed += newSize;
    }
    // WARN: undefined behavior from this point on if the slot wasn't found!
    if (newSize < (slots + slotIndex)->size) {
        if ((gMemoryPools[poolIndex].numSlots + 1) == gMemoryPools[poolIndex].maxSlots) {
            return NULL;
        }
        mmAllocSlot2(poolIndex, slotIndex, newSize, SLOT_USED, SLOT_FREE, (slots + slotIndex)->tag, name);
    } else if ((slots + slotIndex)->size < newSize) {
        nextIndex = (slots + slotIndex)->nextIndex;
        size = (slots + slotIndex)->size;
        lastSize = 0;
        while (nextIndex != MEMSLOT_NONE) {
            if ((slots + nextIndex)->flags != SLOT_FREE) {
                break;
            }
            
            lastSize = size;
            size += (slots + nextIndex)->size;
            if (size >= newSize) {
                break;
            }
            nextIndex = (slots + nextIndex)->nextIndex;
        }
        if (size < newSize) {
            return NULL;
        }
        if (newSize < size) {
            mmAllocSlot2(poolIndex, nextIndex, newSize - lastSize, SLOT_USED, SLOT_FREE, (slots + slotIndex)->tag, name);
        }
        nextIndex = (slots + slotIndex)->nextIndex;
        size = (slots + slotIndex)->size;
        while (size < newSize) {
            lastSize = slots[nextIndex].nextIndex;
            size += slots[nextIndex].size;
            (slots + slotIndex)->nextIndex = lastSize;
            if (lastSize != MEMSLOT_NONE) {
                slots[lastSize].prevIndex = slotIndex;
            }
            gMemoryPools[poolIndex].numSlots = gMemoryPools[poolIndex].numSlots - 1;
            slots[gMemoryPools[poolIndex].numSlots].index = nextIndex;
            nextIndex = lastSize;
        }
        (slots + slotIndex)->size = newSize;
    }
    canary = (u32*)((slots + slotIndex)->data);
    canary += ((s32) (newSize - SLOT_CANARY_SIZE) >> 2);
    canary[0] = SLOT_CANARY_VALUE;
    canary[1] = SLOT_CANARY_VALUE;
    return address;
}

s32 mmGetSlotSize(void *address) {
    s32 poolIndex;
    s32 nextIndex;
    MemoryPoolSlot *slots;
    MemoryPoolSlot *slot;

    poolIndex = mmFindPool(address);

    nextIndex = 0;
    slots = gMemoryPools[poolIndex].slots;

    while (nextIndex != MEMSLOT_NONE) {
        slot = (slots + nextIndex);
        nextIndex = slot->nextIndex;

        if (address == slot->data)
            return slot->size;

        nextIndex = slot->nextIndex;
    }

    return -1;
}

void *mmAllocR(s32 poolIndex, s32 size, s32 tag, const char *name) {
    MemoryPool* pool;
    MemoryPoolSlot* curSlot;
    s32 nextIndex;
    s32 currIndex;
    MemoryPoolSlot* slots;
    s32 intFlags;
    s32 var_t0;
    s32 slotSize;
    u32* canary;

    var_t0 = 0;
    intFlags = interrupts_disable();
    currIndex = MEMSLOT_NONE;
    pool = &gMemoryPools[poolIndex];
    pool->memUsed += size;
    if (pool->maxSlots == (pool->numSlots + 1)) {
        interrupts_enable(intFlags);
        return 0;
    }
    size = (s32)ALIGN16(size);
    size = size + 8;
    slots = pool->slots;
    slotSize = S32_MAX;
    nextIndex = 0;
    while (nextIndex != MEMSLOT_NONE) {
        curSlot = &slots[nextIndex];
        if (curSlot->flags == SLOT_FREE) {
            if (curSlot->size >= size) {
                // can fit in slot
                if (curSlot->size < slotSize) {
                    // better (smaller) slot found
                    slotSize = curSlot->size;
                    currIndex = nextIndex;
                }
            } else if (var_t0 < curSlot->size) {
                // record the closest to fitting slot size found that's still to small
                var_t0 = curSlot->size;
            }
        }
        nextIndex = curSlot->nextIndex;
    }
    if (currIndex != MEMSLOT_NONE) {
        mmAllocSlot2(poolIndex, currIndex, size, SLOT_USED, SLOT_FREE, tag, name);
        interrupts_enable(intFlags);
        
        canary = (u32*)((slots + currIndex)->data);
        canary += ((s32) (size - SLOT_CANARY_SIZE) >> 2);
        canary[0] = SLOT_CANARY_VALUE;
        canary[1] = SLOT_CANARY_VALUE;
        
        return (void*)(slots + currIndex)->data;
    }
    // no space left on desired pool
    if (poolIndex == 2) {
        if (var_t0 && var_t0 && var_t0){} // fake match
    }
    return NULL;
}

void *mmAllocAtAddr(s32 size, void *address, s32 tag, const char *name) {
    s32 idx;
    MemoryPoolSlot *currSlot;
    MemoryPoolSlot *slots;
    s32 intFlags;
    u32 *canary;

    intFlags = interrupts_disable();

    if (size == 0) {}
    
    if ((gMemoryPools[0].numSlots + 1) == gMemoryPools[0].maxSlots) {
        interrupts_enable(intFlags);
    } else {
        size = (s32)ALIGN16(size);
        size = size + SLOT_CANARY_SIZE;
        
        slots = gMemoryPools[0].slots;
        for (idx = 0; idx != MEMSLOT_NONE; idx = currSlot->nextIndex) {
            currSlot = &slots[idx];
            if (currSlot->flags == SLOT_FREE) {
                if ((u32) address >= (u32) currSlot->data && 
                    (u32) address + size <= (u32) currSlot->data + currSlot->size) {
                    if (address == currSlot->data) {
                        mmAllocSlot2(0, idx, size, SLOT_USED, SLOT_FREE, tag, name);
                        interrupts_enable(intFlags);
                        return currSlot->data;
                    } else {
                        idx = mmAllocSlot2(0, idx, (u32)address - (u32)currSlot->data, SLOT_FREE, SLOT_USED, tag, name);
                        mmAllocSlot2(0, idx, size, SLOT_USED, SLOT_FREE, tag, name);
                        interrupts_enable(intFlags);

                        canary = (u32*)(slots + idx)->data;
                        canary += ((s32) (size - SLOT_CANARY_SIZE) >> 2);
                        canary[0] = SLOT_CANARY_VALUE;
                        canary[1] = SLOT_CANARY_VALUE;
                        
                        return (slots + idx)->data;
                    }
                }
            }
        }
        
        interrupts_enable(intFlags);
    }

    return NULL;
}

void mmSetDelay(s32 delay) {
    s32 intFlags;

    intFlags = interrupts_disable();
    gMemoryFreeDelay = delay;
    if (delay == 0) {
        while (gMemoryFreeQueueLength > 0) {
            mmFreeNow(gMemoryFreeQueue[--gMemoryFreeQueueLength].address);
        }
    }
    interrupts_enable(intFlags);
}

void mmFree(void *address) {
    s32 prevIE = interrupts_disable();
    if (gMemoryFreeDelay == 0) {
        mmFreeNow(address);
    } else {
        mmFreeEnqueue(address);
    }
    interrupts_enable(prevIE);
}

void mmFreeTick(void) {
    s32 _pad;
    s32 _pad2;
    s32 i;
    s32 intFlags;
    MemoryPoolSlot* slot;
    s32 nextIndex;

    intFlags = interrupts_disable();
    
    for (i = 0; i < gMemoryFreeQueueLength;) {
        gMemoryFreeQueue[i].ticksLeft--;
        if (gMemoryFreeQueue[i].ticksLeft == 0) {
            mmFreeNow(gMemoryFreeQueue[i].address);
            // Replace with last element
            gMemoryFreeQueue[i].address = gMemoryFreeQueue[gMemoryFreeQueueLength - 1].address;
            gMemoryFreeQueue[i].ticksLeft = gMemoryFreeQueue[gMemoryFreeQueueLength - 1].ticksLeft;
            gMemoryFreeQueueLength--;
        } else {
            i++;
        }
    }
    
    interrupts_enable(intFlags);
    
    // Update memory monitors
    memMonVal0 = 0;
    memMonVal2 = 0;
    memMonVal1 = 0;
    
    slot = gMemoryPools[0].slots;
    do {
        if (slot->flags != SLOT_FREE) {
            memMonVal0 += slot->size;
        }
        nextIndex = slot->nextIndex;
        if (nextIndex != MEMSLOT_NONE) {
            slot = &gMemoryPools->slots[nextIndex];
        }
    } while (nextIndex != MEMSLOT_NONE);
    
    if ((s32) gNumMemoryPools >= 2) {
        slot = gMemoryPools[1].slots;
        do {
            if (slot->flags != SLOT_FREE) {
                memMonVal1 += slot->size;
            }
            nextIndex = slot->nextIndex;
            if (nextIndex != MEMSLOT_NONE) {
                slot = &gMemoryPools[1].slots[nextIndex];
            }
        } while (nextIndex != MEMSLOT_NONE);
        
        slot = gMemoryPools[2].slots;
        do {
            if (slot->flags != SLOT_FREE) {
                memMonVal2 += slot->size;
            }
            nextIndex = slot->nextIndex;
            if (nextIndex != MEMSLOT_NONE) {
                slot = &gMemoryPools[2].slots[nextIndex];
            }
        } while (nextIndex != MEMSLOT_NONE);
    }
}

void mmFreeNow(void *address) {
    s32 slotIndex;
    s32 poolIndex;
    MemoryPool *pool;
    MemoryPoolSlot *slot;
    MemoryPoolSlot *slots;

    poolIndex = mmFindPool(address);
    if (poolIndex != -1) {
        pool = &gMemoryPools[poolIndex];
        slots = pool->slots;
        for (slotIndex = 0; slotIndex != MEMSLOT_NONE; slotIndex = slot->nextIndex) {
            slot = &slots[slotIndex];
            if (address == slot->data) {
                if (slot->flags == SLOT_USED || slot->flags == SLOT_SAFEGUARD) {
                    mmFreeSlot(poolIndex, slotIndex);
                }
                break;
            }
            slot = &slots[slotIndex];
        }
    }
}

void mmFreeEnqueue(void *address) {
    gMemoryFreeQueue[gMemoryFreeQueueLength].address = address;
    gMemoryFreeQueue[gMemoryFreeQueueLength].ticksLeft = gMemoryFreeDelay;
    gMemoryFreeQueueLength++;
}

s32 mmFindPool(void *address) {
    s32 i;

    for (i = 0; i < gNumMemoryPools; i++) {
        if (((u32)address > (u32)gMemoryPools[i].slots))
            if ((u32)address < ((u32)gMemoryPools[i].slots + gMemoryPools[i].memAllocated))
                return i;
    }

    return -1;
}

void mmFreeSlot(s32 poolIndex, s32 slotIndex) {
    MemoryPoolSlot* nextSlot;
    MemoryPoolSlot* prevSlot;
    MemoryPoolSlot* currSlot;
    MemoryPoolSlot* slots;
    MemoryPool *pool;
    s32 nextIndex;
    s32 prevIndex;
    s32 tempNextIndex;
    s32 *canary;

    slots = gMemoryPools[poolIndex].slots;
    pool = &gMemoryPools[poolIndex];
    currSlot = &slots[slotIndex];
    nextIndex = currSlot->nextIndex;
    nextSlot = &slots[nextIndex];
    prevIndex = currSlot->prevIndex;
    prevSlot = &slots[prevIndex];
    currSlot = &slots[slotIndex]; // @fake

    // set as free
    currSlot->flags = SLOT_FREE;

    if (1){}
    
    // check canary
    canary = (s32*)(currSlot->data);
    canary += ((s32) ( currSlot->size - SLOT_CANARY_SIZE) >> 2);
    if (canary[0] != SLOT_CANARY_VALUE || canary[1] != SLOT_CANARY_VALUE) {

    }
    
    tempNextIndex = currSlot->size; // probably a fakematch

    pool->memUsed -= tempNextIndex;
    if (nextIndex != MEMSLOT_NONE && nextSlot->flags == SLOT_FREE) {
        // merge with free slot to the right
        currSlot->size += nextSlot->size;
        tempNextIndex = nextSlot->nextIndex;
        currSlot->nextIndex = tempNextIndex;
        if (tempNextIndex != MEMSLOT_NONE) {
            slots[tempNextIndex].prevIndex = slotIndex;
        }
        pool->numSlots -= 1;
        slots[pool->numSlots].index = nextIndex;
    }
    
    if (prevIndex != MEMSLOT_NONE && prevSlot->flags == SLOT_FREE) {
        // merge with free slot to the left
        prevSlot->size += currSlot->size;
        tempNextIndex = currSlot->nextIndex;
        prevSlot->nextIndex = tempNextIndex;
        if (tempNextIndex != MEMSLOT_NONE) {
            slots[tempNextIndex].prevIndex = prevIndex;
        }
        pool->numSlots--;
        slots[pool->numSlots].index = slotIndex;
    }
}

MemoryPoolSlot *mmGetSlotPtr(s32 poolIndex, s32 a1) {
    return gMemoryPools[poolIndex].slots;
}

// new version (compared to DKR, JFG). old mmAllocSlot is unused in DP
// calling with a size bigger than the requested slot is undefined behavior
s32 mmAllocSlot2(s32 poolIndex, s32 slotIndex, s32 size, s32 flags, s32 flags2, s32 tag, const char* name) {
    MemoryPool* pool;
    MemoryPoolSlot *slot;
    MemoryPoolSlot *slots;
    s32 temp_a0;
    s32 temp;
    s32 temp_t2;

    slots = gMemoryPools[poolIndex].slots;
    pool = &gMemoryPools[poolIndex];
    slot = slots;
    slot += slotIndex; 
    temp = slot->size;
    slot->flags = flags;
    slot->size = size;
    slot->tag = tag;
    if (size < temp) {
        // requested size is smaller than the slot's current size, fill in gap
        temp_a0 = slot->nextIndex;
        if (temp_a0 != MEMSLOT_NONE) {
            if (((slots + temp_a0)->flags == SLOT_FREE) && (flags2 == SLOT_FREE)) {
                // grow empty next slot to fill in the gap
                (slots + temp_a0)->data = slot->data + size;
                (slots + temp_a0)->size = ((slots + temp_a0)->size + temp) - size;
                // return gap fill slot
                return temp_a0;
            }
        }
        // next slot is in use, insert a new slot in between to fill in gap
        temp_t2 = pool->numSlots;
        temp_a0 = (slots + temp_t2)->index;
        pool->numSlots = temp_t2 + 1;
        (slots + temp_a0)->data = slot->data + size;
        (slots + temp_a0)->size = temp - size;
        (slots + temp_a0)->flags =  flags2;
        temp = slot->nextIndex; // why is this not a separate var?
        (slots + temp_a0)->prevIndex = slotIndex;
        (slots + temp_a0)->nextIndex = temp;
        slot->nextIndex = temp_a0;
        if (temp != MEMSLOT_NONE) {
            slots[temp].prevIndex = temp_a0;
        }
        // return gap fill slot
        return temp_a0;
    }

    // requested size either fits the slot exactly or is too big
    return slotIndex;
}

s32 mmAllocSlot(s32 poolIndex, s32 slotIndex, s32 size, s32 flags, s32 flags2, s32 tag) {
    MemoryPool *pool;
    MemoryPoolSlot *slots;
    s32 index;
    s32 temp;

    slots = gMemoryPools[poolIndex].slots;
    pool = &gMemoryPools[poolIndex];
    slots[slotIndex].flags = flags;
    temp = slots[slotIndex].size;
    slots[slotIndex].size = size;
    slots[slotIndex].tag = tag;
    index = slots[pool->numSlots].index;
    if (size < temp) {
        // requested size is smaller than the slot's current size,
        // insert a new slot to fill in the gap
        index = (slots + pool->numSlots)->index;
        pool->numSlots++;
        slots[slotIndex].flags = flags2;
        slots[slotIndex].size = temp - size;
        slots[index].flags = flags;
        slots[index].data = &slots[slotIndex].data[temp] - size;
        slots[index].size = size;
        slots[index].tag = tag;
        
        temp = slots[slotIndex].nextIndex; 
        slots[index].prevIndex = slotIndex;
        slots[index].nextIndex = temp;
        slots[slotIndex].nextIndex = index;
        if (temp != MEMSLOT_NONE) {
            slots[temp].prevIndex = index;
        }
        return index;
    }
    return slotIndex;
}

u32 mmAlign16(u32 a) {
    s32 misalignment = a & 0xF;

    if (misalignment > 0) {
        a += 0x10 - misalignment;
    }

    return a;
}

u32 mmAlign8(u32 a) {
    s32 misalignment = a & 7;

    if (misalignment > 0) {
        a += 8 - misalignment;
    }

    return a;
}

u32 mmAlign4(u32 a) {
    s32 misalignment = a & 3;

    if (misalignment > 0) {
        a += 4 - misalignment;
    }

    return a;
}

u32 mmAlign2(u32 a) {
    s32 misalignment = a & 1;

    if (misalignment > 0) {
        a += 2 - misalignment;
    }

    return a;
}

s32 mmSlotPrint(s32 arg0) {
    diPrintf(
        "mem %dk/%dk %dk/%dk %dk/%dk\n\tslot %d/%d %d/%d %d/%d\t",
        memMonVal0 / 1024,
        gMemoryPools[0].memAllocated / 1024, 
        memMonVal1 / 1024, 
        gMemoryPools[1].memAllocated / 1024, 
        memMonVal2 / 1024, 
        gMemoryPools[2].memAllocated / 1024, 
        gMemoryPools[0].numSlots, 
        gMemoryPools[0].maxSlots, 
        gMemoryPools[1].numSlots, 
        gMemoryPools[1].maxSlots, 
        gMemoryPools[2].numSlots, 
        gMemoryPools[2].maxSlots
    );

    return memMonVal0 + memMonVal1 + memMonVal2;
}

// maybe this is alHeapAlloc and below is alHeapDBAlloc? could have swapped the macro
// for a function when they changed this to use the normal heap stuff. this one is unused
void *alHeapDBAlloc2(ALHeap *hp, s32 num, s32 size) {
    void *ptr;

    size = (size * num + AL_CACHE_ALIGN) & ~AL_CACHE_ALIGN;
    size += 0x10;

    ptr = mmAlloc(size, ALLOC_TAG_AUDIO_COL, "mm:audioheap");
    bzero(ptr, size);
    
    return (void*)mmAlign16((u32)ptr);
}

void *alHeapDBAlloc(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size) {
    void *ptr;

    size = (size * num + AL_CACHE_ALIGN) & ~AL_CACHE_ALIGN;
    size += 0x10;

    ptr = mmAlloc(size, ALLOC_TAG_AUDIO_COL, "mm:audioheap");
    bzero(ptr, size);
    
    return (void*)mmAlign16((u32)ptr);
}
