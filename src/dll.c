#include "common.h"

#define MAX_LOADED_DLLS 128

extern u32* gFile_DLLSIMPORTTAB;
extern s32 gDLLCount;

void init_dll_system(void)
{
    s32 noId = -1;

    queue_alloc_load_file(&gFile_DLLS_TAB, 0x47);
    queue_alloc_load_file(&gFile_DLLSIMPORTTAB, 0x48);

    for (gDLLCount = 2; ((DLLTabEntry*)((u8*)gFile_DLLS_TAB->entries + gDLLCount * 2 * 4u))->offset != -1; ++gDLLCount);
    gLoadedDLLList = malloc(0x800, 4, 0);

    gLoadedDLLCount = 128;
    while (gLoadedDLLCount != 0)
    {
        --gLoadedDLLCount;
        gLoadedDLLList[gLoadedDLLCount].id = noId;
    }
}

u32 find_executing_dll(u32 pc, u32 **start, u32 **end) {
    s32 i;

    for (i = 0; i < gLoadedDLLCount; i++) {
        if (gLoadedDLLList[i].id == -1) {
            continue;
        }
        
        // Get start and end of the full DLL
        *start = gLoadedDLLList[i].exports - 6;
        *end = gLoadedDLLList[i].end;

        if ((s32)*start > (s32)pc || (s32)pc > (s32)*end) {
            // PC is not within the DLL
        } else {
            // PC is within the DLL

            // Set start to the beginning of executable DLL code
            // *start = headerSize + startOfFullDLL
            *start = **start + (u32)*start;

            // Return ID of found DLL
            return gLoadedDLLList[i].id;
        }
    }

    // PC is not executing within any loaded DLL
    return -1;
}

void replace_loaded_dll_list(DLLInst arg0[], s32 arg1) {
    gLoadedDLLCount = arg1;
    gLoadedDLLList = arg0;
}

DLLInst * get_loaded_dlls(u32 * arg0) {
    *arg0 = gLoadedDLLCount;
    return gLoadedDLLList;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_load_deferred.s")
#else
void *dll_load_deferred(u32 id, u16 param) {
    DLLFile *dll;
    DLLInst *dllInst;
    u32 *dllExports;

    dllExports = 0;
    
    if (((u16)id) == 0) {
        return NULL;
    }

    queue_load_dll((void**)&dllExports, (u16)id, param);

    dllInst = (DLLInst *)(dllExports - 2);

    if (dllInst->refCount == 1) {
        dll = (DLLFile*)(*dllExports - 0x18);
        dll->ctor((u32)dll);
    }

    return dllExports;
}
#endif

// Returns pointer to DLLInst exports field
u32* dll_load(u16 id, u16 exportCount, s32 arg2)
{
    DLLFile * dll;
    u32 i;
    u32 totalSize;
    u32* result;

    if (id >= 0x8000) {
        id -= 0x8000;
        // bank2
        id += gFile_DLLS_TAB->entries[1].bssSize;       // bank 2 end
    } else if (id >= 0x2000) {
        id -= 0x2000;
        // bank1
        id += gFile_DLLS_TAB->entries[0].bssSize + 1;   // bank 1 end + 1
    } else if (id >= 0x1000) {
        id -= 0x1000;
        // bank0
        id += gFile_DLLS_TAB->entries[0].offset + 1;    // bank 0 end + 1
    }

    // Check if DLL is already loaded, and if so, increment the reference count
    for (i = 0; i < gLoadedDLLCount; i++)
    {
        if (id == gLoadedDLLList[i].id) {
            ++gLoadedDLLList[i].refCount;
            return &gLoadedDLLList[i].exports;
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
    result = &gLoadedDLLList[i].exports;

    if (arg2 != 0) {
        dll->ctor((u32)dll);
    }

    return result;
}

void func_8000C0B8(u16 id, s32 arg1, s32 arg2, s32 arg3)
{
    DLLFile *dll;
    u32 i;

    for (i = 0; i < gLoadedDLLCount; i++)
        if (gLoadedDLLList[i].id == id)
            return;

    dll = (DLLFile *)malloc(arg2 + arg3, 4, 0);
    bcopy((void *)arg1, (void *)dll, arg2);

    if (arg3)
        bzero((void *)((u32)dll + arg2), arg3);
    
    dll_relocate(dll);
    osInvalICache(dll, 0x4000);
    osInvalDCache(dll, 0x4000);

    for (i = 0; i < gLoadedDLLCount; i++)
        if (gLoadedDLLList[i].id == -1)
            break;

    if (gLoadedDLLCount == i)
    {
        if (gLoadedDLLCount == 128)
            return;

        gLoadedDLLCount++;
    }

    gLoadedDLLList[i].id       = id;
    gLoadedDLLList[i].exports  = dll->exports;
    gLoadedDLLList[i].end      = (u32 *)(((u32)dll + arg2) + arg3);
    gLoadedDLLList[i].refCount = 2;

    dll->ctor((u32)dll);
}

// close
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C258.s")
#else
u32 func_8000C258(DLLInst *param1) {
    DLLFile *dll;
    u16 idx;
    u16 addr;
    u32 *dllTextEnd;
    u32 *dllTextAddr;

    addr = (((u8*)param1 - (u8*)gLoadedDLLList) - 8);

    if ((addr & 0xF) != 0) {
        return 0;
    }

    idx = ((u32)addr >> 4);

    if (idx >= gLoadedDLLCount) {
        return 0;
    }

    gLoadedDLLList[idx].refCount--;

    if (gLoadedDLLList[idx].refCount == 0) {
        dll = (DLLFile*)(gLoadedDLLList[idx].exports - (0x18 / 4));
        
        dll->dtor((u32)dll);

        dllTextAddr = gLoadedDLLList[idx].exports;
        dllTextEnd = gLoadedDLLList[idx].end;

        while (dllTextAddr < dllTextEnd) {
            *(dllTextAddr++) = 0x7000D;
        }

        free(dll);

        gLoadedDLLList[idx].id = 0xFFFFFFFF;

        while (gLoadedDLLCount != 0) {
            if (gLoadedDLLList[gLoadedDLLCount - 1].id != 0xFFFFFFFF) {
                break;
            }

            gLoadedDLLCount--;
        }

        return 1;
    }

    return 0;
}
#endif

s32 dll_throw_fault(void)
{
    u8 * nullPtr = NULL;
    *nullPtr = 0;

    return 0;
}

DLLFile * dll_load_from_tab(u16 id, s32 * sizeOut)
{
    DLLFile * dll;
    s32 offset;
    s32 dllSize; // t0
    s32 bssSize;

    id++;
    offset = ((DLLTab*)((u8*)gFile_DLLS_TAB + id * 2 * 4u))->entries[0].offset;
    dllSize = ((DLLTab*)((u8*)gFile_DLLS_TAB + id * 2 * 4u))->entries[1].offset - offset;
    bssSize = ((DLLTab*)((u8*)gFile_DLLS_TAB + id * 2 * 4u))->entries[0].bssSize;

    dll = malloc((u32)(dllSize + bssSize), ALLOC_TAG_DLL_COL, NULL);
    if (dll != NULL) {
        read_file_region(DLLS_BIN, dll, offset, dllSize);
    }

    if (dll != NULL)
    {
        if (bssSize != 0) {
            bzero((u32)dll + dllSize, bssSize);
        }

        dll_relocate(dll);
        osInvalICache(dll, 0x4000);
        osInvalDCache(dll, 0x4000);

        *sizeOut = dllSize + bssSize;
    }
    return dll;
}

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

    // Relocate constructor and destructor to absolute addresses
    *(u32*)&dll->ctor += (u32)target;
    *(u32*)&dll->dtor += (u32)target;

    // Relocate exports to absolute addresses
    exports = (u32*)((u8*)dll + sizeof(DLLFile));
    for (exportCount = dll->exportCount; exportCount != 0; exportCount--)
    {
        *exports++ += (u32)target;
    }

    if (dll->rodata != -1)
    {
        // Relocate global offset table (GOT)
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

        // Relocate $gp initializer
        while (*currRelocation != -3)
        {
            u32* fn = ((u32)*currRelocation / 4) + (tmp_target = target);

            fn[0] |= (u32)relocations >> 16;
            fn[1] |= (u32)relocations & 0xffff;
            currRelocation++;
        }

        currRelocation++;

        // Relocate .data
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

void func_8000C648(void)
{
    s32 v0 = 0;
    s32 count = gLoadedDLLCount;

    if (count > 0)
    {
        DLLInst *dll = gLoadedDLLList;
        do
        {
            v0 += sizeof(DLLInst);
            if (dll++)
                ;

        } while (v0 < count * 16);
    }
}