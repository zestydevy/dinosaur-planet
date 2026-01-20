#include "common.h"

static const char str_8009b670[] = "diRcpTrace: Buffer not allocated!\n";

#define MAX_DL_DEBUG_INFO_LENGTH 128

typedef struct {
/*0000*/    Gfx *gdl;
/*0004*/    u32 unk4;
/*0008*/    const char *file;
/*000C*/    u32 unkC;
/*0010*/    s32 unk10;
} DLDebugInfo;

/* -------- .data start 80093810 -------- */
DLDebugInfo *gDLDebugInfos[2] = {NULL, NULL}; // There are two sets of 128 infos each.
u32 gDLDebugInfoIdx = 0;
/* -------- .data end 80093820 -------- */

/* -------- .bss start 800bfe70 -------- */
s32 gDLDebugInfoLengths[2];
/* -------- .bss end 800bfe80 -------- */

void dl_init_debug_infos()
{
    gDLDebugInfos[0] = mmAlloc(MAX_DL_DEBUG_INFO_LENGTH * sizeof(DLDebugInfo), COLOUR_TAG_CYAN, NULL);
    gDLDebugInfos[1] = mmAlloc(MAX_DL_DEBUG_INFO_LENGTH * sizeof(DLDebugInfo), COLOUR_TAG_CYAN, NULL);
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
        gDLDebugInfos[gDLDebugInfoIdx][gDLDebugInfoLengths[gDLDebugInfoIdx]].unk4 = param_2;
        gDLDebugInfos[gDLDebugInfoIdx][gDLDebugInfoLengths[gDLDebugInfoIdx]].file = file;
        gDLDebugInfos[gDLDebugInfoIdx][gDLDebugInfoLengths[gDLDebugInfoIdx]].unkC = param_4;
        gDLDebugInfos[gDLDebugInfoIdx][gDLDebugInfoLengths[gDLDebugInfoIdx]].unk10 = -1;
        gDLDebugInfoLengths[gDLDebugInfoIdx]++;
    }
}

void dl_get_debug_info_for_gdl(Gfx *gdl, const char** file, s32* arg2, s32* arg3) {
    Gfx* curr;
    Gfx* prev;
    s32 i;

    *file = NULL;
    for (i = 0; i < gDLDebugInfoLengths[gDLDebugInfoIdx]; i++) {
        curr = gDLDebugInfos[gDLDebugInfoIdx][i].gdl;
        prev = gDLDebugInfos[1 - gDLDebugInfoIdx][i].gdl;
        if (gdl == curr) {
            *file = gDLDebugInfos[gDLDebugInfoIdx][i].file;
            *arg2 = gDLDebugInfos[gDLDebugInfoIdx][i].unkC;
            *arg3 = gDLDebugInfos[gDLDebugInfoIdx][i].unk4;
            break;
        }

        if (gdl == prev) {
            *file = gDLDebugInfos[1 - gDLDebugInfoIdx][i].file;
            *arg2 = gDLDebugInfos[1 - gDLDebugInfoIdx][i].unkC;
            *arg3 = gDLDebugInfos[1 - gDLDebugInfoIdx][i].unk4;
            break;
        }

        if (gdl < curr && gdl < prev) {
            break;
        }
    }
}

void dl_get_debug_info2(Gfx* arg0, u32* arg1, const char** arg2, u32* arg3, Gfx** arg4, u32* arg5, const char** arg6, u32* arg7, Gfx** arg8) {
    s32 i;
    Gfx *temp_a0;
    Gfx *var_t0;
    Gfx *var_v1;
    DLDebugInfo* var_t1;
    DLDebugInfo* var_v0;

    var_v1 = (Gfx *)-1U;
    var_t0 = NULL;
    var_v0 = NULL;
    var_t1 = NULL;
    for (i = 0; i < gDLDebugInfoLengths[1 - gDLDebugInfoIdx]; i++) {
        temp_a0 = gDLDebugInfos[1 - gDLDebugInfoIdx][i].gdl;
        if ((arg0 >= temp_a0) && ((arg0 - temp_a0) < (arg0 - var_t0))) {
            var_t0 = temp_a0;
            var_t1 = &gDLDebugInfos[1 - gDLDebugInfoIdx][i];
        }
        if ((temp_a0 >= arg0) && ((temp_a0 - arg0) < (var_v1 - arg0))) {
            var_v1 = temp_a0;
            var_v0 = &gDLDebugInfos[1 - gDLDebugInfoIdx][i];
        }
    }
    if (var_t1 != NULL) {
        *arg2 = var_t1->file;
        *arg3 = var_t1->unkC;
        *arg4 = var_t1->gdl;
        *arg1 = var_t1->unk4;
    } else {
        *arg2 = NULL;
    }

    if (var_v0 != NULL) {
        *arg6 = var_v0->file;
        *arg7 = var_v0->unkC;
        *arg8 = var_v0->gdl;
        *arg5 = var_v0->unk4;
    } else {
        *arg6 = NULL;
    }
}
