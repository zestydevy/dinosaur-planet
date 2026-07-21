#ifndef _SYS_LIGHTING_ASM_H
#define _SYS_LIGHTING_ASM_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "sys/math.h"

typedef struct { u8 unk0[0x3300]; } Unk800B1848;

void func_8001F890(Vtx* vertices, void* vertexGroups, s32 jointCount, MtxF* matrix, s32 , s32, Unk800B1848*, void *, f32 visRadius);
void func_800202E8(Vtx*, Vtx*, void *, s16 *, u8*, u8*, s32, s32, s32, s32);

#endif
