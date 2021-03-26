#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/dll/init_dll_system.s")

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

// close but needs work in areas
#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_load.s")
/*
s32 dll_load(u16 arg0, u16 arg1, s32 arg2);
s32 dll_load(u16 arg0, u16 arg1, s32 arg2)
{
    u16 a3 = (s16)arg0;     // likely a temp var
    s32 v0 = a3;            // likely a temp var
    s32 s0028;
    s32 s002C;
    DLLFile * s0034;
    s16 s003A;
    s32 i;

    if (a3 >= 0x8000) {
        a3 = ((v0 - 0x8000) + gFile_DLLS_TAB->bank2);
    } else if (v0 <= 0x2000) {
        a3 = (((v0 - 0x2000) + gFile_DLLS_TAB->bank1) + 1);
    } else if (v0 >= 0x1000) {
        a3 = (((v0 - 0x1000) + gFile_DLLS_TAB->bank0) + 1);
    }

    v0 = a3;

    if (gLoadedDLLCount != 0)
    {
        for (i = 0; i < gLoadedDLLCount; i++)
        {
            if (a3 == gLoadedDLLList[i]->id) {
                continue;
            }

            ++gLoadedDLLList[i]->refCount;
            return gLoadedDLLList[i]->id + 8;
        }
    }
    
    i = a3;
    s003A = a3;
    a3 = s003A;

    s0034 = dll_load_from_tab(i, &s002C);
    if (s0034 == NULL) {
        return 0;
    }

    if (s0034->exportCount >= arg1) {
        free((u32)s0034);
        return 0;
    }

    if (gLoadedDLLCount != 0)
    {
        for (i = 0; i < gLoadedDLLCount; i++)
        {
            if (0xFFFFFFFF != gLoadedDLLList[i]->id) {
                break;
            }
        }
    } 
    else if (0 == gLoadedDLLCount) {
        if (gLoadedDLLCount == 128) {
            free((u32)s0034);
            return 0;
        }
        ++gLoadedDLLCount;
    }


    gLoadedDLLList[i]->id = s003A;
    gLoadedDLLList[i]->exports = s0034->reserved;
    gLoadedDLLList[i]->end = (u32 *)((u32)s0034 + s002C);
    gLoadedDLLList[i]->refCount = 1;

    s0028 = gLoadedDLLList[i]->id + 8;

    if (arg2 != 0) {
        s0034->ctor((u32)s0034);
    }

    return s0028;
}
*/

#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C0B8.s")
/*
void func_8000C0B8(u16 id, s32 arg1, s32 arg2, s32 arg3);
void func_8000C0B8(u16 id, s32 arg1, s32 arg2, s32 arg3)
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
*/

#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C258.s")


#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C3BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_load_from_tab.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_relocate.s")

// regalloc
#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C648.s")
/*
void func_8000C648(void);
void func_8000C648(void)
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
*/