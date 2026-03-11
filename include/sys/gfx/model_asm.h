#ifndef _SYS_GFX_MODEL_ASM_H
#define _SYS_GFX_MODEL_ASM_H

#include "PR/ultratypes.h"
#include "sys/gfx/animation.h"
#include "sys/gfx/model.h"
#include "sys/camera.h"
#include "sys/math.h"

void func_8001B4F0(MtxF**, MtxF*, AnimState*, ModelJoint*, s32, s16*, s32, s32);
void convert_mtxf_to_mtx_in_pool(MatrixSlot *);
void func_8001CAA4(AnimState*, s16*, s16*);

#endif
