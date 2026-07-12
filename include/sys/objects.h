#ifndef _SYS_OBJECTS_H
#define _SYS_OBJECTS_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "game/objects/object_def.h"

void objInit(void);
void objUpdateObjModels(void);
void objTick(void); 

void *objGetTable(s32 idx);

void objDoDeferredFree(void);
void objFreeAll(void);

s32 objVisibilitySortObjects(s32 *numObjs);
void objDepthSortObjects(s32 param1, s32 param2);

Object **objGetObjects(s32 *param1, s32 *numObjs);

Object *objSetupObject(ObjSetup *setup, u32 initFlags, s32 mapID, s32 param4, Object *parent);
Object *objSetupObjectActual(ObjSetup *setup, u32 initFlags, s32 mapID, s32 param4, Object *parent, s32 param6);
void objAddObject(Object *obj, u32 initFlags);
void objDisable(Object *obj);
void objEnable(Object *obj);
void objFreeObject(Object *obj);

void objLoadEvent(Object *obj, s32 objId, ObjectEvent *outEvent, s32 id, u8 dontQueueLoad);
void objLoadWeapondata(Object *obj, s32 param2, BinFileEntry *outParam, s32 id, u8 queueLoad);

Object *objGetPlayer(void);
Object *objGetSidekick(void);

void objClearMapID(Object *obj);
void objInferMapID(Object *obj);
void objSetPriority(Object *object, s8 param2);

void objSetModel(Object *obj, s32 param2);
void objHandleModelSwitch(Object *obj, ModelInstance *modelInst, Model *model);

void objAddEffectBox(Object *obj);
void objFreeEffectBox(Object *obj);

void *objAllocSetup(s32 size, s32 objId);

void objHandleAnimseqActors(void);
void obj_func_80023BF8(Object *obj, s32 param2, s32 param3, s32 param4, u8 param5, u8 param6);

s32 objMove(Object *obj, f32 dx, f32 dy, f32 dz);
Object *objGetObjectByUID(s32 param1);

void obj_func_80023C6C(Object *obj);

void objSetInfoNum(Object *obj, u16 param2);
void objLoadPlayer(void);
void objLoadSidekick(Object* playerect, s32 objectId);

#endif
