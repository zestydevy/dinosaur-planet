#include "common.h"

extern u32* gFile_DLLSIMPORTTAB;

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/dll/init_dll_system.s")
#else
extern u32 UINT_800a7d1c;
void queue_alloc_load_file(s32 arg0, u32 id);
void init_dll_system()
{
    queue_alloc_load_file(&gFile_DLLS_TAB, 0x47);
    queue_alloc_load_file(&gFile_DLLSIMPORTTAB, 0x48);

    // Count DLLs
    UINT_800a7d1c = 2;
    while (gFile_DLLS_TAB->entries[UINT_800a7d1c].offset != -1)
    {
        UINT_800a7d1c++;
    }

    gLoadedDLLList = malloc(0x800, 4, 0);
    for (gLoadedDLLCount = 128; gLoadedDLLCount != 0;)
    {
        gLoadedDLLCount--;
        gLoadedDLLList[gLoadedDLLCount - 1].id = 0xFFFFFFFF;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000BD1C.s")

void some_crash_setter(DLLInst arg0[], s32 arg1) {
    gLoadedDLLCount = arg1;
    gLoadedDLLList = arg0;
}

DLLInst * func_8000BDE8(u32 * arg0);
DLLInst * func_8000BDE8(u32 * arg0) {
    *arg0 = gLoadedDLLCount;
    return gLoadedDLLList;
}

#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_load_deferred.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_load.s")
#else
#define MAX_LOADED_DLLS 128
// Returns pointer to DLLInst exports field
u32* _dll_load(u16 id, u16 exportCount, s32 arg2);
u32* _dll_load(u16 id, u16 exportCount, s32 arg2)
{
    u32 totalSize;
    DLLFile * dll;
    u32 i;
    u32* result;

    if (id >= 0x8000) {
        id = ((u16)(id - 0x8000) + gFile_DLLS_TAB->bank2);
    } else if (id >= 0x2000) {
        id = (((u16)(id - 0x2000) + gFile_DLLS_TAB->bank1) + 1);
    } else if (id >= 0x1000) {
        id = (((u16)(id - 0x1000) + gFile_DLLS_TAB->bank0) + 1);
    }

    // Check if DLL is already loaded, and if so, increment the reference count
    for (i = 0; i != gLoadedDLLCount; i++)
    {
        if (id == gLoadedDLLList[i].id) {
            ++gLoadedDLLList[i].refCount;
            return gLoadedDLLList[i].exports;
        }
    }

    dll = dll_load_from_tab(id, &totalSize);
    if (!dll) {
        return 0;
    }

    if (dll->exportCount < exportCount) {
        free(dll);
        return 0;
    }

    // Find an open slot in the DLL list
    for (i = 0; i < (u32)gLoadedDLLCount; i++)
    {
        if (0xFFFFFFFF == gLoadedDLLList[i].id) {
            break;
        }
    }
    
    // If no open slots were available, try to add a new slot
    if (i == gLoadedDLLCount) {
        if (gLoadedDLLCount == MAX_LOADED_DLLS) {
            free(dll);
            return 0;
        }

        ++gLoadedDLLCount;
    }

    gLoadedDLLList[i].id = id;
    gLoadedDLLList[i].exports = dll->exports;
    gLoadedDLLList[i].end = (u32 *)((u32)dll + totalSize);
    gLoadedDLLList[i].refCount = 1;
    result = gLoadedDLLList[i].exports;

    if (arg2 != 0) {
        dll->ctor((u32)dll);
    }

    return result;
}
#endif

// close
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C0B8.s")
#else
void _func_8000C0B8(u16 id, s32 arg1, s32 arg2, s32 arg3);
void _func_8000C0B8(u16 id, s32 arg1, s32 arg2, s32 arg3)
{
    DLLFile * dll;
    s32 i;

    for (i = 0; i != gLoadedDLLCount; i++) {
        if (id == (*gLoadedDLLList)[i].id) {
            return;
        }
    }

    dll = (DLLFile *)malloc(arg2 + arg3, 4, 0);
    _bcopy((void *)arg1, (void *)dll, arg2);

    if (arg3 != 0) {
        bzero((void *)((u32)dll + arg2), arg3);
    }
    
    dll_relocate(dll);
    i = 0;
    osInvalICache(dll, 0x4000);
    osInvalDCache(dll, 0x4000);

    for (; i < gLoadedDLLCount; i++) {
        if ((-1) == (*gLoadedDLLList)[i].id) {
            break;
        }
    }
    
    if (i == gLoadedDLLCount)
    {
        if (gLoadedDLLCount == 128) {
            return;
        }
        ++gLoadedDLLCount;
    }

    (*gLoadedDLLList)[i].id = id;
    (*gLoadedDLLList)[i].exports = (u32 *)((u32)dll + 0x18);
    (*gLoadedDLLList)[i].end = (u32 *)(((u32)dll + arg2) + arg3);
    (*gLoadedDLLList)[i].refCount = 2;

    dll->ctor((u32)dll);

}
#endif

// close
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C258.s")
#else
u32 _func_8000C258(u32 arg0);
u32 _func_8000C258(u32 arg0)
{
    u16 v1;
    u16 t8;
    DLLFile * sp0034;

    v1 = ((arg0 - (u32)gLoadedDLLList) - 8);

    if ((v1 & 0xF) != 0) {
        return 0;
    }

    t8 = (v1 >> 4);

    if (t8 >= gLoadedDLLCount) {
        return 0;
    }

    --(*gLoadedDLLList)[t8].refCount;

    if ((*gLoadedDLLList)[t8].refCount < 0) {
        return 0;
    }

    sp0034 = (DLLFile *)((u32)((*gLoadedDLLList)[t8].exports) - 0x18);

    sp0034->dtor((s32)sp0034);

    if ((*gLoadedDLLList)[t8].exports < (*gLoadedDLLList)[t8].end) {
        u32 * exports = (*gLoadedDLLList)[t8].exports;
        while (exports < (*gLoadedDLLList)[t8].end)) {
            exports += 4;
            *(exports - 0x4) = (u32)0x7000D;
        }
    }

    free(sp0034);

    while (gLoadedDLLCount != 0) {
        if (-1 == (*gLoadedDLLList)[gLoadedDLLCount-1].id) {
            return 1;
        }
        --gLoadedDLLCount;
    }

    return gLoadedDLLCount;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_throw_fault.s")

// close
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_load_from_tab.s")
#else
DLLFile * _dll_load_from_tab(u16 idx, s32 *totalSize)
{
    const DLLTabEntry* entry = &gFile_DLLS_TAB->entries[(u16)(idx + 1)];

    s32 offset = entry->offset;
    s32 bssSize = entry->bssSize;
    u32 dataSize = entry[1].offset - offset;

    DLLFile* dll = malloc(dataSize + bssSize, 4, 0);

    if (dll)
    {
        read_file_region(0x46, dll, offset, dataSize);

        if (bssSize != 0)
        {
            bzero((u8*)dll + dataSize, bssSize);
        }

        dll_relocate(dll);

        osInvalICache(dll, 0x4000);
        osInvalDCache(dll, 0x4000);
        
        *totalSize = dataSize + bssSize;
    }

    return dll;
}
#endif

void dll_relocate(DLLFile* dll);
void dll_relocate(DLLFile* dll)
{
    u32 *tmp_target;
    u32* exports;
    u32* target;
    u32 exportCount;
    s32* relocations;
    s32* currRelocation;

    target = (u32*)((u8*)dll + dll->code);

    *(u32*)&dll->ctor += (u32)target;
    *(u32*)&dll->dtor += (u32)target;

    exports = (u32*)((u8*)dll + sizeof(DLLFile));
    for (exportCount = dll->exportCount; exportCount != 0; exportCount--)
    {
        *exports++ += (u32)target;
    }

    if (dll->rodata != -1)
    {
        relocations = (s32*)((u8*)dll + dll->rodata);
        currRelocation = relocations;

        while (*currRelocation != -2)
        {
            if (*currRelocation & 0x80000000)
            {
                *currRelocation = gFile_DLLSIMPORTTAB[(*currRelocation & 0x7fffffff) - 1];
            }
            else
            {
                *currRelocation += (s32)target;
            }
            currRelocation++;
        }

        currRelocation++;

        while (*currRelocation != -3)
        {
            u32* fn = ((u32)*currRelocation / 4) + (tmp_target = target);

            fn[0] |= (u32)relocations >> 16;
            fn[1] |= (u32)relocations & 0xffff;
            currRelocation++;
        }

        currRelocation++;

        exports = &((u8 *) dll)[dll->data];
        target = (u32 *) exports;
        
        while (*currRelocation != -1)
        {
            exports = &((u8 *) dll)[dll->data];
            target[(u32)*currRelocation / 4] += (u32)target;
            currRelocation++;
        }
    }
}

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C648.s")
#else
void _func_8000C648(void);
void _func_8000C648(void)
{
    s32 v1 = gLoadedDLLCount;
    s32 v0 = 0;
    s32 a0 = gLoadedDLLList;

    if (v1 > 0) {
        s32 a1 = a0 * 16;
        do {
            v0 += 16;
        } while (v0 < a1);
    }
}
#endif