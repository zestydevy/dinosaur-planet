#include "common.h"

#define MAX_DL_DEBUG_INFO_LENGTH 128

typedef struct {
/*0000*/    Gfx *gdl;
/*0004*/    u32 unk_0x4;
/*0008*/    const char *file;
/*000C*/    u32 unk_0xc;
/*0010*/    s32 unk_0x10;
} DLDebugInfo;

extern DLDebugInfo *gDLDebugInfos[2]; // There are two sets of 128 infos each.
extern u32 gDLDebugInfoIdx;
extern s32 gDLDebugInfoLengths[2];

void dl_init_debug_infos()
{
    gDLDebugInfos[0] = mmAlloc(MAX_DL_DEBUG_INFO_LENGTH * sizeof(DLDebugInfo), 0xffffff, NULL);
    gDLDebugInfos[1] = mmAlloc(MAX_DL_DEBUG_INFO_LENGTH * sizeof(DLDebugInfo), 0xffffff, NULL);
}

void dl_next_debug_info_set()
{
    gDLDebugInfoIdx = 1 - gDLDebugInfoIdx;
    gDLDebugInfoLengths[gDLDebugInfoIdx] = 0;
}

void dl_add_debug_info(Gfx *gdl, u32 param_2, const char *file, u32 param_4)
{
    if (gDLDebugInfos[gDLDebugInfoIdx] != NULL && gDLDebugInfoLengths[gDLDebugInfoIdx] < MAX_DL_DEBUG_INFO_LENGTH)
    {
        gDLDebugInfos[gDLDebugInfoIdx][gDLDebugInfoLengths[gDLDebugInfoIdx]].gdl = gdl;
        gDLDebugInfos[gDLDebugInfoIdx][gDLDebugInfoLengths[gDLDebugInfoIdx]].unk_0x4 = param_2;
        gDLDebugInfos[gDLDebugInfoIdx][gDLDebugInfoLengths[gDLDebugInfoIdx]].file = file;
        gDLDebugInfos[gDLDebugInfoIdx][gDLDebugInfoLengths[gDLDebugInfoIdx]].unk_0xc = param_4;
        gDLDebugInfos[gDLDebugInfoIdx][gDLDebugInfoLengths[gDLDebugInfoIdx]].unk_0x10 = -1;
        gDLDebugInfoLengths[gDLDebugInfoIdx]++;
    }
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dl_debug/dl_get_debug_info_for_gdl.s")
#else
void dl_get_debug_info_for_gdl(Gfx *gdl, char **file, u32 *param_3, u32 *param_4)
{
    s32 i;
    DLDebugInfo *curr;
    DLDebugInfo *prev;

    *file = NULL;

    for (i = 0; i < gDLDebugInfoLengths[gDLDebugInfoIdx]; i++)
    {
        curr = &gDLDebugInfos[gDLDebugInfoIdx][i];
        prev = &gDLDebugInfos[1 - gDLDebugInfoIdx][i];

        if (gdl == curr->gdl) {
            *file = curr->file;
            *param_3 = gDLDebugInfos[gDLDebugInfoIdx][i].unk_0xc;
            *param_4 = gDLDebugInfos[gDLDebugInfoIdx][i].unk_0x4;
            return;
        }

        if (gdl == prev->gdl) {
            *file = prev->file;
            *param_3 = gDLDebugInfos[1 - gDLDebugInfoIdx][i].unk_0xc;
            *param_4 = gDLDebugInfos[1 - gDLDebugInfoIdx][i].unk_0x4;
            return;
        }

        if (gdl < curr->gdl && gdl < prev->gdl) {
            return;
        }
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/dl_debug/dl_get_debug_info2.s")
