#include "common.h"

#define MAX_LOADED_DLLS 128
#define DLL_NONE -1

#define DLL_FILE_EXPORTS(dllFile) ((u32*)((u32)dllFile + sizeof(DLLFile)))
#define DLL_EXPORTS_TO_FILE(exports) ((DLLFile*)((u32)exports - sizeof(DLLFile)))

#define DLL_INST_EXPORTS_FIELD_OFFSET 0x8
#define DLL_INST_EXPORTS_TO_INST(instExports) ((DLLInst*)((u32)instExports - DLL_INST_EXPORTS_FIELD_OFFSET))

extern u32 *gFile_DLLSIMPORTTAB;
extern DLLTab *gFile_DLLS_TAB;
extern s32 gDLLCount;

void dll_relocate(DLLFile *dll);
DLLFile *dll_load_from_tab(u16 id, s32 *sizeOut);

void init_dll_system() {
    s32 noId = DLL_NONE;

    queue_alloc_load_file((void**)&gFile_DLLS_TAB, 0x47);
    queue_alloc_load_file((void**)&gFile_DLLSIMPORTTAB, 0x48);

    gDLLCount = 2; 
    while (((DLLTabEntry*)((u8*)gFile_DLLS_TAB + gDLLCount * 2 * 4u))->offset != -1) {
        ++gDLLCount;
    }

    gLoadedDLLList = (DLLInst*)malloc(0x800, 4, NULL);

    gLoadedDLLCount = MAX_LOADED_DLLS;
    while (gLoadedDLLCount != 0)
    {
        --gLoadedDLLCount;
        gLoadedDLLList[gLoadedDLLCount].id = noId;
    }
}

u32 find_executing_dll(u32 pc, void **start, void **end) {
    s32 i;

    for (i = 0; i < gLoadedDLLCount; i++) {
        if (gLoadedDLLList[i].id == DLL_NONE) {
            continue;
        }
        
        // Get start and end of the full DLL
        *start = (void*)DLL_EXPORTS_TO_FILE(gLoadedDLLList[i].exports);
        *end = gLoadedDLLList[i].end;

        if ((s32)*start > (s32)pc || (s32)pc > (s32)*end) {
            // PC is not within the DLL
        } else {
            // PC is within the DLL

            // Set start to the beginning of executable DLL code
            *start = ((DLLFile*)*start)->code + (u32)*start;

            // Return ID of found DLL
            return gLoadedDLLList[i].id;
        }
    }

    // PC is not executing within any loaded DLL
    return DLL_NONE;
}

void replace_loaded_dll_list(DLLInst arg0[], s32 arg1) {
    gLoadedDLLCount = arg1;
    gLoadedDLLList = arg0;
}

DLLInst *get_loaded_dlls(u32 *outLoadedDLLCount) {
    *outLoadedDLLCount = gLoadedDLLCount;
    return gLoadedDLLList;
}

u32 **dll_load_deferred(u16 id, u16 exportCount) {
    DLLFile *dll;
    DLLInst *dllInst;
    u32 **dllInstExports;

    dllInstExports = NULL;
    
    if (!id) {
        return NULL;
    }

    queue_load_dll(&dllInstExports, id, exportCount);

    dllInst = DLL_INST_EXPORTS_TO_INST(dllInstExports);

    if (dllInst->refCount == 1) {
        // convert addr of dll exports array to addr of dll start
        dll = DLL_EXPORTS_TO_FILE(*dllInstExports);
        dll->ctor(dll);
    }

    return dllInstExports;
}

// Returns pointer to DLLInst exports field
u32 **dll_load(u16 id, u16 exportCount, s32 runConstructor) {
    DLLFile *dll;
    u32 i;
    u32 totalSize;
    u32 **result;

    if (id >= 0x8000) {
        id -= 0x8000;
        // bank2
        id += gFile_DLLS_TAB->header.bank2;
    } else if (id >= 0x2000) {
        id -= 0x2000;
        // bank1
        id += gFile_DLLS_TAB->header.bank1 + 1;
    } else if (id >= 0x1000) {
        id -= 0x1000;
        // bank0
        id += gFile_DLLS_TAB->header.bank0 + 1;
    }

    // Check if DLL is already loaded, and if so, increment the reference count
    for (i = 0; i < gLoadedDLLCount; i++) {
        if (id == gLoadedDLLList[i].id) {
            ++gLoadedDLLList[i].refCount;
            return &gLoadedDLLList[i].exports;
        }
    }

    dll = dll_load_from_tab(id, &totalSize);
    if (!dll) {
        return NULL;
    }

    if (dll->exportCount < exportCount) {
        free(dll);
        return NULL;
    }

    // Find an open slot in the DLL list
    for (i = 0; i < gLoadedDLLCount; i++) {
        if (gLoadedDLLList[i].id == DLL_NONE) {
            break;
        }
    }
    
    // If no open slots were available, try to add a new slot
    if (i == gLoadedDLLCount) {
        if (gLoadedDLLCount == MAX_LOADED_DLLS) {
            free(dll);
            return NULL;
        }

        ++gLoadedDLLCount;
    }

    gLoadedDLLList[i].id = id;
    gLoadedDLLList[i].exports = DLL_FILE_EXPORTS(dll);
    gLoadedDLLList[i].end = (void*)((u32)dll + totalSize);
    gLoadedDLLList[i].refCount = 1;
    result = &gLoadedDLLList[i].exports;

    if (runConstructor) {
        dll->ctor(dll);
    }

    return result;
}

// unused
void dll_load_from_bytes(u16 id, void *dllBytes, s32 dllBytesSize, s32 bssSize) {
    DLLFile *dll;
    u32 i;

    for (i = 0; i < gLoadedDLLCount; i++) {
        if (gLoadedDLLList[i].id == id) {
            return;
        }
    }

    dll = (DLLFile*)malloc(dllBytesSize + bssSize, 4, NULL);
    bcopy(dllBytes, (void*)dll, dllBytesSize);

    if (bssSize) {
        bzero((void *)((u32)dll + dllBytesSize), bssSize);
    }
    
    dll_relocate(dll);
    osInvalICache(dll, 0x4000);
    osInvalDCache(dll, 0x4000);

    for (i = 0; i < gLoadedDLLCount; i++) {
        if (gLoadedDLLList[i].id == DLL_NONE) {
            break;
        }
    }

    if (gLoadedDLLCount == i) {
        if (gLoadedDLLCount == MAX_LOADED_DLLS) {
            return;
        }

        gLoadedDLLCount++;
    }

    gLoadedDLLList[i].id       = id;
    gLoadedDLLList[i].exports  = DLL_FILE_EXPORTS(dll);
    gLoadedDLLList[i].end      = (void*)(((u32)dll + dllBytesSize) + bssSize);
    gLoadedDLLList[i].refCount = 2;

    dll->ctor(dll);
}

u32 dll_unload(u32 **dllInstExports) {
    DLLFile *dll;
    u16 idx;
    u32 *dllClearAddr;
    u32 *dllTextEnd;

    idx = (u32)DLL_INST_EXPORTS_TO_INST(dllInstExports) - (u32)gLoadedDLLList;

    if ((idx % 16) != 0) {
        return FALSE;
    }

    idx >>= 4;

    if (idx >= gLoadedDLLCount) {
        return FALSE;
    }

    gLoadedDLLList[idx].refCount--;

    if (gLoadedDLLList[idx].refCount == 0) {
        dll = DLL_EXPORTS_TO_FILE(gLoadedDLLList[idx].exports);
        
        dll->dtor(dll);

        dllClearAddr = gLoadedDLLList[idx].exports;
        dllTextEnd = gLoadedDLLList[idx].end;

        while (dllClearAddr < dllTextEnd) {
            *(dllClearAddr++) = 0x0007000D; // MIPS break instruction
        }

        free(dll);

        gLoadedDLLList[idx].id = DLL_NONE;

        while (gLoadedDLLCount != 0) {
            if (gLoadedDLLList[gLoadedDLLCount - 1].id != DLL_NONE) {
                break;
            }

            gLoadedDLLCount--;
        }

        return TRUE;
    }

    return FALSE;
}

s32 dll_throw_fault() {
    u8 *nullPtr = NULL;
    *nullPtr = 0;

    return 0;
}

DLLFile *dll_load_from_tab(u16 id, s32 *sizeOut) {
    DLLFile * dll;
    s32 offset;
    s32 dllSize; // t0
    s32 bssSize;

    // ID is already 1-based, adding one again gets us passed the header
    id++;
    offset = ((DLLTabEntry*)((u8*)gFile_DLLS_TAB + id * 2 * 4u))->offset;
    dllSize = (((DLLTabEntry*)((u8*)gFile_DLLS_TAB + id * 2 * 4u)) + 1)->offset - offset;
    bssSize = ((DLLTabEntry*)((u8*)gFile_DLLS_TAB + id * 2 * 4u))->bssSize;

    dll = malloc((u32)(dllSize + bssSize), ALLOC_TAG_DLL_COL, NULL);
    if (dll != NULL) {
        read_file_region(DLLS_BIN, dll, offset, dllSize);
    }

    if (dll != NULL) {
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

void dll_relocate(DLLFile *dll) {
    u32 *tmp_target;
    u32 *exports;
    u32 *target;
    u32 exportCount;
    s32 *globalOffsetTable;
    s32 *currRelocation;

    target = (u32*)((u8*)dll + dll->code);

    // Relocate constructor and destructor to absolute addresses
    *(u32*)&dll->ctor += (u32)target;
    *(u32*)&dll->dtor += (u32)target;

    // Relocate exports to absolute addresses (skip first one)
    exports = DLL_FILE_EXPORTS(dll) + 1;
    for (exportCount = dll->exportCount; exportCount != 0; exportCount--) {
        *exports++ += (u32)target;
    }

    if (dll->rodata != -1) {
        // Relocate global offset table (GOT)
        globalOffsetTable = (s32*)((u8*)dll + dll->rodata);
        currRelocation = globalOffsetTable;

        while (*currRelocation != -2) {
            if (*currRelocation & 0x80000000) {
                *currRelocation = gFile_DLLSIMPORTTAB[(*currRelocation & 0x7fffffff) - 1];
            } else {
                *currRelocation += (s32)target;
            }
            currRelocation++;
        }

        currRelocation++;

        // Relocate $gp initializers
        while (*currRelocation != -3) {
            u32* fn = ((u32)*currRelocation / 4) + (tmp_target = target);

            fn[0] |= (u32)globalOffsetTable >> 16;
            fn[1] |= (u32)globalOffsetTable & 0xffff;
            currRelocation++;
        }

        currRelocation++;

        // Relocate .data
        exports = &((u8 *) dll)[dll->data];
        target = (u32 *) exports;
        
        while (*currRelocation != -1) {
            exports = &((u8 *) dll)[dll->data];
            target[(u32)*currRelocation / 4] += (u32)target;
            currRelocation++;
        }
    }
}

void dll_unused_8000C648() {
    s32 v0 = 0;
    s32 count = gLoadedDLLCount;

    if (count > 0) {
        DLLInst *dll = gLoadedDLLList;
        do {
            v0 += sizeof(DLLInst);
            if (dll++)
                ;

        } while (v0 < count * 16);
    }
}
