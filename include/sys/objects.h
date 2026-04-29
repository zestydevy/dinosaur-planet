#ifndef _SYS_OBJECTS_H
#define _SYS_OBJECTS_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "game/objects/object_def.h"

void init_objects(void);
void update_obj_models(void);
void update_objects(void); 

void *func_800213A0(s32 idx);

void obj_do_deferred_free(void);
void obj_free_all(void);

s32 obj_visibility_sort_objects(s32 *numObjs);
void obj_depth_sort_objects(s32 param1, s32 param2);

Object **get_world_objects(s32 *param1, s32 *numObjs);

Object *obj_create(ObjSetup *setup, u32 initFlags, s32 mapID, s32 param4, Object *parent);
Object *obj_setup_object(ObjSetup *setup, u32 initFlags, s32 mapID, s32 param4, Object *parent, s32 param6);
void obj_add_object(Object *obj, u32 initFlags);
void obj_free_tick(Object *obj);
void obj_add_tick(Object *obj);
void obj_destroy_object(Object *obj);

void obj_load_event(Object *obj, s32 objId, ObjectEvent *outEvent, s32 id, u8 dontQueueLoad);
void obj_load_weapondata(Object *obj, s32 param2, BinFileEntry *outParam, s32 id, u8 queueLoad);

Object *get_player(void);
Object *get_sidekick(void);

void obj_clear_map_id(Object *obj);
void obj_infer_map_id(Object *obj);
void obj_set_update_priority(Object *object, s8 param2);

void obj_set_model(Object *obj, s32 param2);
void obj_handle_model_switch(Object *obj, ModelInstance *modelInst, Model *model);

void obj_add_effect_box(Object *obj);
void obj_free_effect_box(Object *obj);

void *obj_alloc_setup(s32 size, s32 objId);

void func_800210DC(void);
void func_80023BF8(Object *obj, s32 param2, s32 param3, s32 param4, u8 param5, u8 param6);

s32 obj_move(Object *obj, f32 dx, f32 dy, f32 dz);
Object *func_800211B4(s32 param1);

void func_80023C6C(Object *obj);

void func_80023D08(Object *obj, u16 param2);
void func_80023628(void);
void func_80023894(Object* playerect, s32 objectId);

#endif
