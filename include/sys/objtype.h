#ifndef _SYS_OBJTYPE_H
#define _SYS_OBJTYPE_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

enum ObjType {
    OBJTYPE_PLAYER = 0,
    OBJTYPE_SIDEKICK = 1
};

void obj_object_type_init();
void obj_add_object_type(Object *obj, s32 type);
void obj_free_object_type(Object *obj, s32 type);
Object **obj_get_all_of_type(s32 type, s32 *count);
Object *obj_get_nearest_type_to(s32 type, Object *object, float *distance);
Object *obj_get_nearest_type_to_excluding_self(s32 type, Object *object, float *distance);
Object *obj_get_nearest_type(s32 type, Vec3f *position, float *distance);
s32 obj_is_object_type(Object *obj, s32 type);

#endif
