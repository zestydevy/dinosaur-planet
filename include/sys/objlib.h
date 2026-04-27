#ifndef _SYS_OBJLIB_H
#define _SYS_OBJLIB_H

#include "PR/ultratypes.h"

#include "game/objects/object.h"

u16 func_80031BBC(f32 x, f32 y, f32 z);
/** check if object talked to by player? */
int func_80032538(Object* objInteracted);
void func_8003273C(Object *obj);
s32 func_80031F6C(Object* obj, s32 attachIdx, f32* ox, f32* oy, f32* oz, s32 arg5);
MtxF* func_80032170(Object*, s32);
void func_80032238(Object* player, s32 arg1, s32 arg2, Vec3f* arg3);
s16 func_80031DD8(Object* objA, Object* objB, f32* distance);
void func_800321E4(Object* obj, s32 arg1, f32* ox, f32* oy, f32* oz);

#endif
