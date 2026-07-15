#ifndef _SYS_OBJLIB_H
#define _SYS_OBJLIB_H

#include "PR/ultratypes.h"

#include "game/objects/object.h"

typedef void (*ObjectTouchCallbackFunc)(Object*, Object*);

u16 objGetAreaValueAtPoint(f32 x, f32 y, f32 z);
s16 objAngleToObjectXZ(Object* objA, Object* objB, f32* distance);
s32 objGetAttachPointWorldSpace(Object* obj, s32 attachIdx, f32* ox, f32* oy, f32* oz, s32 arg5);
MtxF* objGetAttachPointBoneMatrix(Object* obj, s32 attachIdx);
void objGetAttachPointLocalSpace(Object* obj, s32 attachIdx, f32* ox, f32* oy, f32* oz);
void objGetAttachPointBoneWorldPositions(Object* obj, s32 attachBaseIdx, s32 count, Vec3f* positions);
s32 objIsObjectStandalone(Object *obj);
Object* objFindClosestObject(Object* obj, s32 objID, f32* distance);
int objCheckPlayerInteract(Object* obj);
s32 objRegisterTouchCallback(Object* obj, Object* otherObj, ObjectTouchCallbackFunc callback);
void objRemoveTouchCallback(Object* obj, Object* otherObj);
void objRemoveTouchCallbacksForObj(Object *obj);
void objInvokeTouchCallbacks(Object* obj, Object* otherObj);

#endif
