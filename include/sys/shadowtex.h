#ifndef _SYS_SHADOWTEX_H
#define _SYS_SHADOWTEX_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"

/** The number of shadow texture slots. Each slot is double buffered. The player uses slot 0. */
#define SHADOWTEX_NUM_SLOTS 2

void shadowtex_init(void);
s32 shadowtex_get_status(s32 *outTexWidth);
void shadowtex_draw(Object *obj, s16 arg1, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s32 arg6, f32 arg7, s32 slot);
void shadowtex_get_textures(s32 bufferIdx, void **arg1, void **arg2, s32 slot);

#endif
