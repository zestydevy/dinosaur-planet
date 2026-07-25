#ifndef _SYS_OBJPRINT_H
#define _SYS_OBJPRINT_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"

void objprintDrawShadowModel(Object *obj, Gfx **gdl, Mtx **rspMtxs, Vertex **vtxs, Triangle **pols, u32 param_6);
void objprintSetModelMatrixOverride(MtxF* arg0);
void objprintDrawObject(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** tris, Object* obj, s8 visibility);
void objprintDrawModel(Object *obj, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, f32 yPrescale);
/** Sets a multiplier colour for models (generally lowers brightness) */
void objprintSetMultiplierColor(s16 r, s16 g, s16 b);
/** Sets a blend colour for models (can increase brightness) */
void objprintSetBlendColor(s16 r, s16 g, s16 b, u8 a);
void objprintUpdateLockIconCoords(Object*);
ModelInstance *objprintDrawChildModel(Gfx** arg0, Mtx** arg1, Vertex** arg2, Triangle** arg3, Object* arg4, ModelInstance* arg5, MtxF* arg6, MtxF* arg7, Object* arg8, s32 arg9, s32 arg10);

#endif
