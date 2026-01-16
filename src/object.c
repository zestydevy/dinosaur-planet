#include "common.h"
#include "variables.h"
#include "sys/gfx/model.h"
#include "sys/linked_list.h"
#include "sys/objects.h"
#include "sys/objanim.h"
#include "sys/objtype.h"
#include "sys/newshadows.h"

typedef struct {
/*00*/  u8 _unk0[0x8 - 0x0];
/*08*/  f32 unk8;
/*0C*/  f32 unkC;
/*10*/  f32 unk10;
/*14*/  u8 _unk14[0x20 - 0x14];
} UnkStruct_80091668;

// -------- .data start 80091660 -------- //
s32 D_80091660 = 2;
s16 D_80091664[2] = {0x00, 0x1F}; // gCharacterObjIds
UnkStruct_80091668 D_80091668 = {
    { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
    0.0f,
    0.0f,
    0.0f,
    { 0x00,0x00,0x00,0x00,0xFF,0x3C,0x00,0x5C,0x00,0x5A,0x1E,0x14 }
};
SidekickSetup D_80091688 = {
    {
        /*objId=*/-1,
        /*quarterSize=*/7,
        /*setupExclusions1=*/0,
        /*loadFlags=*/OBJSETUP_LOAD_FLAG1,
        /*fadeFlags=*/OBJSETUP_FADE_FLAG4,
        /*loadDistance=*/0xFF,
        /*fadeDistance=*/0xFF,
        /*x=*/0.0f,
        /*y=*/0.0f,
        /*z=*/0.0f,
        /*uID=*/-1
    },
    /*unk18=*/0,
    /*unk19=*/0
};
// -------- .data end 800916b0 -------- //

// -------- .bss start 800b18e0 -------- //
s16 D_800B18E0;
s16* D_800B18E4;
u32 D_800B18E8;
s32  *gFile_OBJECTS_TAB;
int gNumObjectsTabEntries;
void *gFile_TABLES_BIN;
s32  *gFile_TABLES_TAB;
int gNumTablesTabEntries;
ObjDef **gLoadedObjDefs;
u8 *gObjDefRefCount;
s16  *gFile_OBJINDEX;
int gObjIndexCount; //count of OBJINDEX.BIN entries
Object **gObjDeferredFreeList;
s32 gObjDeferredFreeListCount;
Object **D_800B1918;
s32 D_800B191C;
Object **gObjList; //global object list
s32 gNumObjs;
LinkedList gObjUpdateList;
s8 gEffectBoxCount;
Object *gEffectBoxes[20];
s32 D_800B1988;
// -------- .bss end 800b1990 -------- //

enum FILE_ID {
    FILE_TABLES_BIN   = 0x16,
    FILE_TABLES_TAB   = 0x17,
    FILE_BITTABLE     = 0x37,
    FILE_OBJECTS_TAB  = 0x41,
    FILE_OBJINDEX_BIN = 0x43
};

void queue_alloc_load_file(void **dest, s32 fileId);
void queue_load_file_to_ptr(void **dest, s32 fileId);
void alloc_some_object_arrays(void); //related to objects
void obj_clear_all(void);

void copy_obj_position_mirrors(Object *obj, ObjSetup *setup, s32 param3);

void func_80046320(s16 param1, Object *object);

extern void func_80058FE8();
extern void update_obj_hitboxes(s32);
extern void func_80025E58();
extern void obj_do_hit_detection(s32);
extern void func_8002B6EC();

void update_obj_models();
void update_object(Object *obj);
void func_8002272C(Object *obj);

extern void func_80025DF0();

ModLine *obj_load_objdef_modlines(s32 modLineNo, s16 *modLineCount);

extern void func_800596BC(ObjDef*);

ObjDef *obj_load_objdef(s32 tabIdx);
u32 obj_get_model_flags(Object *obj);
u32 obj_calc_mem_size(Object *obj, ObjDef *def, u32 flags);
void obj_free_objdef(s32 tabIdx);

void func_80021E74(f32 scale, ModelInstance *modelInst);
void func_80022200(Object *obj, s32 param2, s32 param3);
u32 obj_alloc_object_state(Object *obj, u32 addr);
u32 obj_init_event_data(s32 param1, Object *obj, u32 addr);
u32 func_8002298C(s32 param1, ModelInstance *param2, Object *obj, u32 addr);

f32 func_80022150(Object *obj);

void obj_free_object(Object *obj, s32 param2);

void init_objects(void) {
    int i;

    //allocate some buffers
    gObjDeferredFreeList = mmAlloc(0x2D0, ALLOC_TAG_OBJECTS_COL, NULL);
    D_800B1918     = mmAlloc(0x60, ALLOC_TAG_OBJECTS_COL,  NULL);
    D_800B18E4     = mmAlloc(0x10, ALLOC_TAG_OBJECTS_COL,  NULL);

    //load OBJINDEX.BIN and count number of entries
    queue_alloc_load_file((void **) (&gFile_OBJINDEX), FILE_OBJINDEX_BIN);
    gObjIndexCount = (get_file_size(FILE_OBJINDEX_BIN) >> 1) - 1;
    while(!gFile_OBJINDEX[gObjIndexCount]) gObjIndexCount--;

    //load OBJECTS.TAB and count number of entries
    queue_alloc_load_file((void **)&gFile_OBJECTS_TAB, FILE_OBJECTS_TAB);
    gNumObjectsTabEntries = 0;
    while(gFile_OBJECTS_TAB[gNumObjectsTabEntries] != -1) gNumObjectsTabEntries++;
    gNumObjectsTabEntries--;

    //init ref count and pointers
    gLoadedObjDefs = mmAlloc(gNumObjectsTabEntries * 4, ALLOC_TAG_OBJECTS_COL, NULL);
    gObjDefRefCount   = mmAlloc(gNumObjectsTabEntries,     ALLOC_TAG_OBJECTS_COL, NULL);
    for(i = 0; i < gNumObjectsTabEntries; i++) gObjDefRefCount[i] = 0; //why not memset?

    //load TABLES.BIN and TABLES.TAB and count number of entries
    queue_alloc_load_file((void **) (&gFile_TABLES_BIN), FILE_TABLES_BIN);
    queue_alloc_load_file((void **) (&gFile_TABLES_TAB), FILE_TABLES_TAB);
    gNumTablesTabEntries = 0;
    while(gFile_TABLES_TAB[gNumTablesTabEntries] != -1) gNumTablesTabEntries++;

    //allocate global object list and some other buffers
    gObjList = mmAlloc(0x2D0, ALLOC_TAG_OBJECTS_COL, NULL);
    alloc_some_object_arrays();
    obj_clear_all();
}

void update_objects() {
    void *node;
    Object *obj;
    Object *player;
    s32 nextFieldOffset;

    nextFieldOffset = gObjUpdateList.nextFieldOffset; // == 0x38, &obj->next

    func_80058FE8();

    update_obj_models();
    update_obj_hitboxes(gNumObjs);

    node = gObjUpdateList.head;

    for (obj = (Object*)node; node != NULL && obj->updatePriority == 100; obj = (Object*)node) {
        update_object(obj);
        node = *((void**)(nextFieldOffset + (u32)node));

        if (obj->objhitInfo->unk58){} // fake match
    }

    for (obj = (Object*)node; node != NULL && obj->def->flags & 0x40; obj = (Object*)node) {
        update_object(obj);
        obj->matrixIdx = func_80004258(obj);
        node = *((void**)(nextFieldOffset + (u32)node));
    }

    func_80025E58();

    while (node != NULL) {
        obj = (Object*)node;

        if (obj->objhitInfo != NULL) {
            if (obj->objhitInfo->unk5A != 8 || (obj->objhitInfo->unk58 & 1) == 0) {
                update_object(obj);
            }
        } else {
            update_object(obj);
        }

        node = *((void**)(nextFieldOffset + (u32)node));
    }

    player = get_player();
    if (player != NULL && player->linkedObject != NULL) {
        player->linkedObject->parent = player->parent;
        update_object(player->linkedObject);
    }

    obj_do_hit_detection(gNumObjs);

    node = gObjUpdateList.head;
    while (node != NULL) {
        obj = (Object*)node;
        func_8002272C(obj);
        node = *((void**)(nextFieldOffset + (u32)node));
    }

    player = get_player();
    if (player != NULL && player->linkedObject != NULL) {
        player->linkedObject->parent = player->parent;
        func_8002272C(player->linkedObject);
    }

    gDLL_24_Waterfx->vtbl->func_6E8(gUpdateRate);
    gDLL_15_Projgfx->vtbl->func2(gUpdateRate, 0);
    gDLL_14_Modgfx->vtbl->func2(0, 0, 0);
    gDLL_13_Expgfx->vtbl->func2(0, gUpdateRate, 0, 0);

    func_8002B6EC();

    gDLL_3_Animation->vtbl->func9();
    gDLL_3_Animation->vtbl->func5();
    gDLL_2_Camera->vtbl->func1(gUpdateRate);

    write_c_file_label_pointers("objects/objects.c", 0x169);
}

static const char str_800994f4[] = "objGetSequence objtype out of range %d/%d\n";
static const char str_80099520[] = "objSetupObjectActual objtype out of range %d/%d\n";
static const char str_80099554[] = "Warning: Unknown object type '%d/%d romdefno %d', using DummyObject (128)\n";
static const char str_800995a0[] = "ObjSetupObject(3) Memory fail!!\n";
static const char str_800995c4[] = "ObjSetupObject(2) Memory fail!!\n";
static const char str_800995e8[] = "ObjList Overflow %d!!!\n";

void doNothing_80020A40(void) {}

void update_obj_models() {
    int i;
    int j;
    int k;
    Object *object;
    ModelInstance *modelInst;
    ObjectC0_Data *unk1;

    for (i = 0; i < gNumObjs; i++) {
        object = gObjList[i];

        for (j = 0; j < 2; j++) {
            if (j != 0) {
                object = object->linkedObject;
            }

            if (object == NULL) {
                continue;
            }

            for (k = 0; k < object->def->numModels; k++) {
                modelInst = object->modelInsts[k];

                if (modelInst != NULL) {
                    modelInst->unk34 &= ~0x8;

                    if (modelInst->model->blendshapes != NULL) {
                        unk1 = object->unkC0 != NULL ? (ObjectC0_Data*)object->unkC0->data : NULL;

                        if (object->unkC0 == NULL || (unk1 != NULL && unk1->unk62 == 0)) {
                            func_8001B084(modelInst, gUpdateRateF);
                        }
                    }
                }
            }
        }
    }
}

void obj_do_deferred_free() {
    int i;
    for(i = 0; i < gObjDeferredFreeListCount; i++) {
        obj_free_object(gObjDeferredFreeList[i], 0);
        gObjDeferredFreeList[i] = 0;
    }
    gObjDeferredFreeListCount = 0;
}

void obj_free_all() {
    s32 i;
    
    obj_do_deferred_free();
    D_80091660 = 0;

    i = 0;
    while (gNumObjs != 0) {
        obj_destroy_object(gObjList[i]);
        i++;
        if (i >= gNumObjs) {
            i = 0;
        }
    }

    obj_do_deferred_free();
    D_80091660 = 2;

    gObjDeferredFreeListCount = 0;
    D_800B191C = 0;
    gNumObjs = 0;

    linked_list_init(&gObjUpdateList, OFFSETOF(Object, next));

    obj_clear_all();

    gDLL_2_Camera->vtbl->func9(0, 0);
}

void obj_clear_all() {
    gObjDeferredFreeListCount = 0;
    D_800B191C = 0;
    D_800B1988 = 0;
    gNumObjs = 0;

    linked_list_init(&gObjUpdateList, OFFSETOF(Object, next));

    D_800B18E0 = 0;
    obj_object_type_init();
    func_80025DF0();
}

void func_80020D90(void) {
    D_800B18E0 = 0;
}

s32 func_80020DA0(s32 *numObjs) {
    s32 objsEnd;
    s32 idx1;
    s32 idx2;
    s32 endLoop;

    *numObjs = gNumObjs;

    if (0) {}

    if (D_800B18E0 != 0) {
        return D_800B18E0;
    }

    idx1 = 0;
    idx2 = gNumObjs - 1;
    objsEnd = idx2;

    // some kind of sort?
    while (idx1 <= idx2) {
        endLoop = 0;
        while (idx1 <= objsEnd && !endLoop) {
            if ((gObjList[idx1]->def->flags & 1) != 0) {
                idx1++;
            } else {
                endLoop = -1;
            }
        }

        endLoop = 0;
        while (idx2 >= 0 && !endLoop) {
            if ((gObjList[idx2]->def->flags & 1) == 0) {
                idx2--;
            } else {
                endLoop = -1;
            }
        }

        if (idx1 < idx2) {
            Object *temp;

            temp = gObjList[idx1];
            gObjList[idx1] = gObjList[idx2];
            gObjList[idx2] = temp;

            idx1++;
            idx2--;
        }
    }

    D_800B18E0 = idx1;
    return idx1;
}

void func_80020EE4(s32 param1, s32 param2) {
    s32 i;
    Object *obj;
    s32 endLoop;

    if (param1 <= param2) {
        for (i = param1; i <= param2; i++) {
            obj = gObjList[i];

            if (obj->def->flags & 0x80000) {
                obj->unkA4 = obj->def->unk9d * 1000;
            } else {
                obj->unkA4 = -camera_get_angle_to_point(obj->positionMirror.x, obj->positionMirror.y, obj->positionMirror.z);
            }
        }

        do {
            endLoop = TRUE;

            for (i = param1; i < param2; i++) {
                if (gObjList[i]->unkA4 < gObjList[i + 1]->unkA4) {
                    obj = gObjList[i];
                    gObjList[i] = gObjList[i + 1];
                    gObjList[i + 1] = obj;
                    
                    endLoop = FALSE;
                }
            }
        } while (!endLoop);
    }
}

void func_800210DC(void) {
    s32 i;
    Object *obj;
    Object *var;

    for (i = 0; i < gNumObjs; i++) {
        obj = gObjList[i];

        if (obj->unkC0 != NULL) {
            var = obj->unkC0;

            if (obj->parent == NULL) {
                if (var->parent != NULL) {
                    obj->parent = var->parent;
                }
            }
            
            obj->unkC0 = NULL;
        }
    }
}

Object **get_world_objects(s32 *param1, s32 *numObjs) {
    if (param1 != NULL) {
        *param1 = 0;
    }
    if (numObjs != NULL) {
        *numObjs = gNumObjs;
    }

    return gObjList;
}

Object *get_world_object(s32 idx) {
    if (idx < 0 || idx >= gNumObjs) {
        return NULL;
    }

    return gObjList[idx];
}

/** get_world_object_by_uid? */
Object *func_800211B4(s32 uID) {
    s32 i;
    s32 len;
    Object *obj;

    i = 0;
    len = gNumObjs;

    while (i < len) {
        obj = gObjList[i];
        if (obj->setup != NULL && obj->setup->uID == uID) {
            return obj;
        }

        i++;
    }

    return NULL;
}

s32 get_num_objects(void) {
    return gNumObjs;
}

s32 ret0_800212E8(void) { return 0; }

s32 obj_get_seq(s32 objId, s32 idx) {
    ObjDef *objDef;
    s32 seq;
    
    seq = -1;

    if (objId > gObjIndexCount) {
        return -1;
    } else {
        objId = gFile_OBJINDEX[objId]; // turns objId into tabIdx
        objDef = obj_load_objdef(objId);

        if (objDef != NULL) {
            seq = -1;

            if (objDef->pSeq != NULL && idx >= 0 && idx < objDef->numSequences) {
                seq = objDef->pSeq[idx];
            }

            obj_free_objdef(objId);
        }
    }

    return seq;
}

void *func_800213A0(s32 idx) {
    if (idx < 0 || idx >= gNumTablesTabEntries) {
        return gFile_TABLES_BIN;
    }

    return (void*)((u32)gFile_TABLES_BIN + gFile_TABLES_TAB[idx] * 4);
}

Object *obj_create(ObjSetup *setup, u32 initFlags, s32 mapID, s32 param4, Object *parent) {
    Object *obj;

    obj = NULL;
    queue_load_map_object(&obj, setup, initFlags, mapID, param4, parent, 0);
    obj_add_object(obj, initFlags);

    return obj;
}

// actual name: objSetupObjectActual ?
Object *obj_setup_object(ObjSetup *setup, u32 initFlags, s32 mapID, s32 param4, Object *parent, s32 param6) {
    ObjDef *def;
    s32 modelCount;
    s32 var;
    u32 modflags;
    ModelInstance *tempModel;
    Object *obj;
    s32 tabIdx;
    s32 objId;
    s32 j;
    Object objHeader;
    s32 addr;
    s8 modelLoadFailed;

    objId = setup->objId;

    update_pi_manager_array(0, objId);

    if (initFlags & OBJ_INIT_FLAG2) {
        tabIdx = objId;
    } else {
        if (objId > gObjIndexCount) {
            update_pi_manager_array(0, -1);
            return NULL;
        }

        tabIdx = gFile_OBJINDEX[objId];
    }

    bzero(&objHeader, sizeof(Object));

    objHeader.def = obj_load_objdef(tabIdx);
    def = objHeader.def;

    if (def == NULL || (u32)def == 0xFFFFFFFF) {
        return NULL;
    } 
    
    objHeader.srt.flags = 2;

    if (def->flags & 0x80) {
        objHeader.srt.flags = 0x82;
    }

    if (def->flags & 0x40000) {
        objHeader.unkB0 |= 0x80;
    }

    if (initFlags & OBJ_INIT_FLAG4) {
        objHeader.srt.flags |= 0x2000;
    }

    objHeader.srt.transl.x = setup->x;
    objHeader.srt.transl.y = setup->y;
    objHeader.srt.transl.z = setup->z;
    objHeader.setup = setup;
    objHeader.tabIdx = tabIdx;
    objHeader.id = objId;
    objHeader.unkB2 = param4;
    objHeader.mapID = mapID;
    objHeader.curModAnimIdLayered = -1;
    objHeader.unkB4 = -1;
    objHeader.srt.scale = def->scale;
    objHeader.opacity = 255;
    objHeader.mesgQueue = NULL;
    objHeader.loadDistance = setup->loadDistance * 8;
    objHeader.fadeDistance = setup->fadeDistance * 8;
    objHeader.dll = NULL;

    if (def->dllID != 0) {
        objHeader.dll = (DLL_IObject*)dll_load(def->dllID, 6, 1);
    }

    modflags = obj_get_model_flags(&objHeader);

    if (def->flags & 0x20) {
        modflags &= ~MODFLAGS_1;
    } else {
        modflags |= MODFLAGS_1;
    }

    if (def->shadowType != 0) {
        modflags |= MODFLAGS_SHADOW;
    } else {
        modflags &= ~MODFLAGS_SHADOW;
    }

    if (def->flags & 1) {
        modflags |= MODFLAGS_DONT_LOAD_MODEL;
    }

    var = obj_calc_mem_size(&objHeader, def, modflags);

    obj = (Object*)mmAlloc(var, ALLOC_TAG_OBJECTS_COL, NULL);

    if (obj == NULL) {
        obj_free_objdef(tabIdx);
        return NULL;
    }

    bcopy(&objHeader, obj, sizeof(Object));
    bzero((void*)((u32)obj + sizeof(Object)), var - sizeof(Object));

    modelCount = def->numModels;

    obj->modelInsts = (ModelInstance**)((u32)obj + sizeof(Object));

    modelLoadFailed = FALSE;
    var = 0;
    
    if (!(modflags & MODFLAGS_DONT_LOAD_MODEL)) {
        if (modflags & MODFLAGS_LOAD_SINGLE_MODEL) {
            var = MODFLAGS_GET_MODEL_INDEX(modflags);

            if (var < modelCount) {
                obj->modelInsts[var] = model_load_create_instance(-def->pModelList[var], modflags);

                if (obj->modelInsts[var] == NULL) {
                    modelLoadFailed = TRUE;
                    goto modelLoadFailedLabel;
                } else {
                    tempModel = obj->modelInsts[var];
                    func_80021E74(obj->srt.scale, tempModel);
                }
            }
        } else {
            for (; var < modelCount; var++) {
                obj->modelInsts[var] = model_load_create_instance(-def->pModelList[var], modflags);
                if (obj->modelInsts[var] == NULL) {
                    modelLoadFailed = TRUE;
                } else {
                    tempModel = obj->modelInsts[var];
                    func_80021E74(obj->srt.scale, tempModel);
                }
            }
        }
    }

    modelLoadFailedLabel:
    if (modelLoadFailed) {
        func_80022200(obj, modelCount, objId);
        obj_free_objdef(tabIdx);
        return NULL;
    }
     
    addr = obj_alloc_object_state(obj, (u32)&obj->modelInsts[def->numModels]);

    if (modflags & MODFLAGS_EVENTS) {
        addr = obj_init_event_data(obj->id, obj, addr);
    }

    if (modflags & MODFLAGS_100) {
        addr = func_8002298C(obj->id, obj->modelInsts[0], obj, addr);
    }

    if ((modflags & MODFLAGS_SHADOW) && (def->shadowType != 0)) {
        addr = shadow_init_obj_shadow(obj, addr, 0);
    }

    obj->unkA8 = func_80022150(obj) * obj->srt.scale;

    if (def->unk8F != 0) {
        addr = func_8002667C(obj, addr);

        if (def->unk93 & 0x8) {
            addr = func_80026BD8(obj, addr);
        }
    }

    if (def->numSequenceBones != 0) {
        obj->unk6C = (void*)mmAlign4(addr);
        addr = (u32)obj->unk6C + (def->numSequenceBones * sizeof(s16) * 9);
    }

    if (def->numAnimatedFrames != 0) {
        obj->unk70 = (Vtx*)mmAlign4(addr);
        addr = (u32)obj->unk70 + (def->numAnimatedFrames * sizeof(Vtx));
    }

    if (def->unk9b != 0) {
        obj->unk74 = (ObjectStruct74*)mmAlign4(addr);
        addr = (u32)obj->unk74 + (def->unk9b * sizeof(ObjectStruct74));
    }

    if (def->unk8F != 0 && def->unk74 != 0) {
        addr = mmAlign4(addr);
        addr = func_80026A20(obj->id, obj->modelInsts[0], obj->objhitInfo, addr, obj);
    }

    if (def->unk9b != 0) {
        obj->unk78 = (ObjectStruct78*)mmAlign4(addr);

        for (j = 0; j < def->unk9b; j++) {
            obj->unk78[j].colourIndex = def->unk40[j].unk10;
            obj->unk78[j].unk0 = def->unk40[j].unk0c;
            obj->unk78[j].unk3 = def->unk40[j].unk0f;
            obj->unk78[j].unk1 = def->unk40[j].unk0d;
            obj->unk78[j].unk2 = def->unk40[j].unk0e;
        }
    }

    obj->parent = parent;
    
    return obj;
}

void obj_add_object(Object *obj, u32 initFlags) {
    if (obj->parent != NULL) {
        transform_point_by_object(
            obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z,
            &obj->positionMirror.x, &obj->positionMirror.y, &obj->positionMirror.z,
            obj->parent
        );
    } else {
        obj->positionMirror.x = obj->srt.transl.x;
        obj->positionMirror.y = obj->srt.transl.y;
        obj->positionMirror.z = obj->srt.transl.z;
    }

    obj->positionMirror2.x = obj->srt.transl.x;
    obj->positionMirror2.y = obj->srt.transl.y;
    obj->positionMirror2.z = obj->srt.transl.z;

    obj->positionMirror3.x = obj->positionMirror.x;
    obj->positionMirror3.y = obj->positionMirror.y;
    obj->positionMirror3.z = obj->positionMirror.z;

    copy_obj_position_mirrors(obj, obj->setup, 0);

    if (obj->objhitInfo != NULL) {
        obj->objhitInfo->unk10.x = obj->srt.transl.x;
        obj->objhitInfo->unk10.y = obj->srt.transl.y;
        obj->objhitInfo->unk10.z = obj->srt.transl.z;

        obj->objhitInfo->unk20.x = obj->srt.transl.x;
        obj->objhitInfo->unk20.y = obj->srt.transl.y;
        obj->objhitInfo->unk20.z = obj->srt.transl.z;
    }

    if (obj->def->unka0 > -1) {
        func_80046320(obj->def->unka0, obj);
    }

    update_pi_manager_array(0, -1);

    if (obj->def->flags & OBJDATA_FLAG44_HasChildren) {
        obj_add_object_type(obj, OBJTYPE_7);

        if (obj->updatePriority != 90) {
            obj_set_update_priority(obj, 90);
        }
    } else {
        if (obj->updatePriority == 0) {
            obj_set_update_priority(obj, 80);
        }
    }

    if (initFlags & OBJ_INIT_FLAG1) {
        obj->unkB0 |= 0x10;
        gObjList[gNumObjs] = obj;
        gNumObjs += 1;

        obj_add_tick(obj);
    }

    if (obj->def->unk5e >= 1) {
        obj_add_object_type(obj, OBJTYPE_9);
    }

    if (obj->def->flags & OBJDATA_FLAG44_HaveModels) {
        func_80020D90();
    }

    if (obj->def->flags & OBJDATA_FLAG44_DifferentLightColor) {
        obj_add_object_type(obj, OBJTYPE_56);
    }

    write_c_file_label_pointers("objects/objects.c", 0x477);
}

u32 obj_calc_mem_size(Object *obj, ObjDef *def, u32 modflags) {
    u32 size;

    size = sizeof(Object);

    size += def->numModels * sizeof(u32);

    if (obj->dll != NULL) {
        size += obj->dll->vtbl->get_data_size(obj, size);
    }

    if (modflags & MODFLAGS_EVENTS) {
        size = mmAlign4(size);
        size = mmAlign8(size + sizeof(ObjectEvent));
        size += 0x50;
    }

    if (modflags & MODFLAGS_100) {
        size = mmAlign4(size);
        size = mmAlign8(size + sizeof(BinFileEntry));
        size += 0x400;
    }

    if ((modflags & MODFLAGS_SHADOW) && (def->shadowType != 0)) {
        size = mmAlign4(size);
        size += sizeof(ObjectShadow);
    }

    if (def->unk8F != 0) {
        size = mmAlign4(size);
        size += sizeof(ObjectHitInfo);

        if (def->unk93 & 8) {
            size += sizeof(ObjectStruct58);
        }
    }

    if (def->numSequenceBones != 0) {
        size = mmAlign4(size);
        size += def->numSequenceBones * 0x12;
    }

    if (def->numAnimatedFrames != 0) {
        size = mmAlign4(size);
        size += def->numAnimatedFrames * 0x10;
    }

    if (def->unk9b != 0) {
        size = mmAlign4(size);
        size += def->unk9b * 0x18;
    }

    if (def->unk8F != 0 && def->unk74 != 0) {
        size = mmAlign8(size);
        size += 300;
    }

    if (def->unk9b != 0) {
        size = mmAlign4(size);
        size += def->unk9b * 5;
    }

    return size;
}

static const char str_80099614[] = " No Points ";

void func_80021E74(f32 scale, ModelInstance *modelInst) {
    Model *model;
    ModelInstance_0x14 *temp_v0_2;
    Vec3f sp2EC;
    f32 sp94[150];
    f32 *temp_v1;
    s32 i;
    s8 parentJointID;

    model = modelInst->model;
    if (modelInst->model->jointCount != 0) {
        temp_v1 = modelInst->model->collisionA;
        if (temp_v1 != NULL && modelInst->unk14 != NULL) {
            temp_v0_2 = modelInst->unk14;
            temp_v0_2->unk4[0] = temp_v1[0] * scale;
            if (temp_v0_2->unk4[0] == 0.0f) {
                temp_v0_2->unk4[0] = temp_v1[1] * scale;
            }
            temp_v0_2->unk8[0] = SQ(temp_v0_2->unk4[0]);
            temp_v0_2->unkC[0] = 0.1f;
            temp_v0_2->unk10[0] = temp_v0_2->unk4[0];
            sp94[0] = 0;
            for (i = 1; i < modelInst->model->jointCount; i++) {
                temp_v0_2->unk4[i] = temp_v1[i] * scale;
                temp_v0_2->unk8[i] = SQ(temp_v0_2->unk4[i]);
                parentJointID = model->joints[i].parentJointID;
                sp2EC.f[0] = model->joints[i].x;
                sp2EC.f[1] = model->joints[i].y;
                sp2EC.f[2] = model->joints[i].z;
                temp_v0_2->unkC[i] = sqrtf(SQ(sp2EC.f[0]) + SQ(sp2EC.f[1]) + SQ(sp2EC.f[2])) * scale;
                if (temp_v0_2->unkC[i] == 0.0f) {
                    temp_v0_2->unkC[i] = 0.1f;
                }
                if (model->collisionB[i] >= 1.0f) {
                    temp_v0_2->unkC[i] *= model->collisionB[i];
                }
                sp94[i] = sp94[parentJointID] + temp_v0_2->unkC[i];
                if (temp_v1[i] == 0.0f) {
                    temp_v0_2->unk10[i] = -100.0f;
                } else {
                    temp_v0_2->unk10[i] = sp94[i] + temp_v0_2->unk4[i];
                }
            }
        }
    }
}

f32 func_80022150(Object *obj) {
    s32 i;
    f32 largestSomething;

    largestSomething = 10.0f;

    for (i = 0; i < obj->def->numModels; i++) {
        if (obj->modelInsts[i] != NULL) {
            f32 var = obj->modelInsts[i]->model->maxAnimatedVertDistance;

            if (var > largestSomething) {
                largestSomething = var;
            }
        }
    }

    if (largestSomething < obj->def->unk9c) {
        largestSomething = obj->def->unk9c * 16.0f;
    }

    return largestSomething;
}

void func_80022200(Object *obj, s32 param2, s32 param3) {
    s32 i;

    for (i = 0; i < param2; i++) {
        if (obj->modelInsts[i] != NULL) {
            ModelInstance *modelInst = obj->modelInsts[i];
            destroy_model_instance(modelInst);
        }
    }
}

static const char str_80099620[] = "objFreeTick %08x locked %d,already on list\n";

// name guessed from leftover strings
void obj_free_tick(Object *obj) {
    if (obj->unkB0 & 0x10) {
        linked_list_remove(&gObjUpdateList, obj);
    }
}

void obj_add_tick(Object *obj) {
    void *insertAfter;
    void *node;
    Object *objNode;
    s32 nextFieldOffset;
    
    if (obj->unkB0 & 0x10) {
        nextFieldOffset = gObjUpdateList.nextFieldOffset;
        insertAfter = NULL;

        node = gObjUpdateList.head;

        // Insert maintaining sort by update priority
        for (objNode = (Object*)node; node != NULL && obj->updatePriority < ((Object*)node)->updatePriority; objNode = (Object*)node) {
            insertAfter = objNode;
            node = *((void**)(nextFieldOffset + (u32)node));
        }

        linked_list_insert(&gObjUpdateList, insertAfter, (void*)obj);
    }
}

static const char str_8009964c[] = "objFreeObject: delete list size overrun\n";

void obj_destroy_object(Object *obj) {
    s32 i;
    s32 k;

    if (obj == NULL) {
        *((volatile s8*)NULL) = 0;
        return;
    }

    if (!(obj->unkB0 & 0x40)) {
        if (obj->unkD9 != 0) {
            func_8003273C(obj);
        }

        if (obj->unkB0 & 0x10) {
            for (i = 0; i < gNumObjs; i++) {
                if (obj == gObjList[i]) {
                    break;
                }
            }

            if (i < gNumObjs) {
                gNumObjs--;

                for (k = i; k < gNumObjs; k++) {
                    gObjList[k] = gObjList[k + 1];
                }
            }

            obj_free_tick(obj);
            func_80020D90();
        }

        obj->unkB0 |= 0x40;

        if (obj->unkDA != 0) {
            for (i = 0; i < D_800B191C; i++) {
                if (obj == D_800B1918[i]) {
                    break;
                }
            }

            if (i == D_800B191C) {
                D_800B1918[D_800B191C] = obj;
                D_800B191C += 1;
            } else {
                
            }
        } else if (D_80091660 == 2) {
            i = gObjDeferredFreeListCount;

            if (gObjDeferredFreeListCount != 0) {
                for (i = 0; i < gObjDeferredFreeListCount; i++) {
                    if (obj == gObjDeferredFreeList[i]) {
                        break;
                    }
                }
            }

            if (i == gObjDeferredFreeListCount) {
                gObjDeferredFreeList[gObjDeferredFreeListCount] = obj;
                gObjDeferredFreeListCount++;
                
                if (gObjDeferredFreeListCount == 180) {
                    gObjDeferredFreeListCount--;
                }
            }
        } else {
            obj_free_object(obj, D_80091660 == 0);
        }
    }
}

void copy_obj_position_mirrors(Object *obj, ObjSetup *setup, s32 param3) {
    DLL_IObject *dll;
    obj->group = obj->def->group;
    dll = obj->dll;
    if(1) {
        if(dll != NULL) {
            obj->dll->vtbl->setup(obj, setup, param3);
        }
    }

    obj->positionMirror2.x = obj->srt.transl.x;
    obj->positionMirror3.x = obj->srt.transl.x;
    obj->positionMirror2.y = obj->srt.transl.y;
    obj->positionMirror3.y = obj->srt.transl.y;
    obj->positionMirror2.z = obj->srt.transl.z;
    obj->positionMirror3.z = obj->srt.transl.z;
}

void update_object(Object *obj) {
    if (obj->unkB0 & 0x40) {
        return;
    }

    if (obj->unkC0 != NULL) {
        if (obj->linkedObject != NULL) {
            obj->linkedObject->objhitInfo->unk48 = 0;
            obj->linkedObject->objhitInfo->unk62 = 0;
        }

        if (obj->objhitInfo != NULL) {
            obj->objhitInfo->unk48 = 0;
            obj->objhitInfo->unk62 = 0;
        }
    } else {
        update_pi_manager_array(1, obj->id);

        if (!(obj->srt.flags & 8)) {
            obj->positionMirror2.x = obj->srt.transl.x;
            obj->positionMirror2.y = obj->srt.transl.y;
            obj->positionMirror2.z = obj->srt.transl.z;

            obj->positionMirror3.x = obj->positionMirror.x;
            obj->positionMirror3.y = obj->positionMirror.y;
            obj->positionMirror3.z = obj->positionMirror.z;
        }

        if (obj->dll != NULL && !(obj->unkB0 & 0x8000)) {
            obj->dll->vtbl->control(obj);

            get_object_child_position(obj,
                &obj->positionMirror.x, &obj->positionMirror.y, &obj->positionMirror.z);
        }

        if (obj->objhitInfo != NULL) {
            if (obj->linkedObject != NULL) {
                obj->linkedObject->objhitInfo->unk48 = 0;
                obj->linkedObject->objhitInfo->unk62 = 0;
            }

            obj->objhitInfo->unk4C = 0;
            obj->objhitInfo->unk48 = 0;
            obj->objhitInfo->unk62 = 0;
        }

        if (obj->unk58 != NULL) {
            obj->unk58->unk10f = 0;
        }

        obj->unkAF &= ~(0x1 | 0x2 | 0x4);

        update_pi_manager_array(1, -1);
    }
}

void func_8002272C(Object *obj) {
    update_pi_manager_array(3, obj->id);

    if (obj->dll != NULL && !(obj->unkB0 & 0x2000)) {
        obj->dll->vtbl->update(obj);

        get_object_child_position(obj,
            &obj->positionMirror.x, &obj->positionMirror.y, &obj->positionMirror.z);
    }

    update_pi_manager_array(3, -1);
}

u32 obj_alloc_object_state(Object *obj, u32 addr) {
    u32 dataSize = 0;
    
    addr = mmAlign4(addr);

    if (obj->dll != NULL) {
        dataSize = obj->dll->vtbl->get_data_size(obj, addr);
    }

    if (dataSize != 0) {
        obj->data = (void*)addr;
        addr += dataSize;
    } else {
        obj->data = NULL;
    }

    return addr;
}

u32 obj_get_model_flags(Object *obj) {
    if (obj->dll != NULL) {
        return obj->dll->vtbl->get_model_flags(obj);
    } else {
        return MODFLAGS_NONE;
    }
}

u32 obj_init_event_data(s32 objId, Object *obj, u32 addr) {
    obj->curEvent = (ObjectEvent*)mmAlign4(addr);

    addr = mmAlign8((u32)obj->curEvent + sizeof(ObjectEvent));
    obj->curEvent->data = (UNK_PTR*)addr;

    addr += 0x50;

    obj_load_event(obj, objId, obj->curEvent, 0, /*dontQueueLoad=*/TRUE);

    return addr;
}

static const char str_80099678[] = "objects.c: event data size overflow\n";

void obj_load_event(Object *obj, s32 objId, ObjectEvent *outEvent, s32 id, u8 dontQueueLoad) {
    ObjDefEvent *eventList;
    ObjDefEvent *event;
    
    eventList = obj->def->pEvent;

    outEvent->size = 0;
    
    if (eventList == NULL) {
        return;
    }

    for (event = eventList; event->id != -1; event++) {
        if (event->id == id) {
            s32 offset;

            offset = event->fileOffsetInBytes;

            outEvent->size = event->fileSizeInBytes;

            if (outEvent->size > 80) {
                outEvent->size = 80;
            }

            if (eventList) {}

            if (!dontQueueLoad) {
                queue_load_file_region_to_ptr((void**)outEvent->data, OBJEVENT_BIN, offset, outEvent->size);
            } else {
                read_file_region(OBJEVENT_BIN, outEvent->data, offset, outEvent->size);
            }

            break;
        }
    }
}

u32 func_8002298C(s32 objId, ModelInstance *param2, Object *obj, u32 addr) {
    if (param2 == 0) {
        return addr;
    }

    obj->unk5C = (ObjectEvent*)mmAlign4(addr);

    addr = mmAlign8((u32)obj->unk5C + sizeof(ObjectEvent));
    obj->unk5C->data = (UNK_PTR*)addr;

    return addr + 0x400;
}

void obj_load_weapondata(Object *obj, s32 param2, BinFileEntry *outParam, s32 id, u8 queueLoad) {
    ObjDefWeaponData *weaponDataList;
    ObjDefWeaponData *weaponData;
    
    weaponDataList = obj->def->pWeaponData;

    outParam->size = 0;
    
    if (weaponDataList == NULL) {
        return;
    }

    for (weaponData = weaponDataList; weaponData->id != -1; weaponData++) {
        if (weaponData->id == id) {
            s32 offset;

            offset = weaponData->fileOffsetInBytes;

            outParam->size = weaponData->fileSizeInBytes;

            if (outParam->size > 1024) {
                outParam->size = 1024;
            }

            if (weaponDataList) {}

            if (queueLoad) {
                queue_load_file_region_to_ptr((void**)outParam->data, WEAPONDATA_BIN, offset, outParam->size);
            } else {
                read_file_region(WEAPONDATA_BIN, outParam->data, offset, outParam->size);
            }

            break;
        }
    }
}

static const char str_800996a0[] = "ob %d fileno %d\n";
static const char str_800996b4[] = "Objects out of ram(1) !!\n";

ObjDef *obj_load_objdef(s32 tabIdx) {
    ObjDef *def;
    s32 fileOffset;
    s32 fileSize;

    if (tabIdx >= gNumObjectsTabEntries) {
        return NULL;
    }
    
    if (gObjDefRefCount[tabIdx] != 0) {
        gObjDefRefCount[tabIdx]++;
        def = gLoadedObjDefs[tabIdx];
        return def;
    }
    
    fileOffset = gFile_OBJECTS_TAB[tabIdx];
    fileSize = gFile_OBJECTS_TAB[tabIdx + 1] - fileOffset;

    def = (ObjDef*)mmAlloc(fileSize, ALLOC_TAG_OBJECTS_COL, NULL);
    if (def != NULL) {
        read_file_region(OBJECTS_BIN, (void*)def, fileOffset, fileSize);

        if (def->pEvent != 0) {
            def->pEvent = (ObjDefEvent*)((u32)def + (u32)def->pEvent);
        }

        if (def->pHits != 0) {
            def->pHits = (ObjDefHit*)((u32)def + (u32)def->pHits);
        }

        if (def->pWeaponData != 0) {
            def->pWeaponData = (ObjDefWeaponData*)((u32)def + (u32)def->pWeaponData);
        }

        def->pModelList = (u32*)((u32)def + (u32)def->pModelList);
        def->pTextures = (UNK_PTR*)((u32)def + (u32)def->pTextures);
        def->pSequenceBones = (UNK_PTR*)((u32)def + (u32)def->pSequenceBones);

        if (def->unk18 != 0) {
            def->unk18 = (u32*)((u32)def + (u32)def->unk18);
        }

        if (def->unk40 != 0) {
            def->unk40 = (ObjDefStruct40*)((u32)def + (u32)def->unk40);
        }

        if (def->pSeq != 0) {
            def->pSeq = (s16*)((u32)def + (u32)def->pSeq);
        }

        def->pModLines = NULL;

        def->pAttachPoints = (AttachPoint*)((u32)def + (u32)def->pAttachPoints);

        def->pIntersectPoints = NULL;

        if (def->modLineNo > -1) {
            def->pModLines = obj_load_objdef_modlines(def->modLineNo, &def->modLineCount);
            func_800596BC(def);
        }

        gLoadedObjDefs[tabIdx] = def;
        gObjDefRefCount[tabIdx] = 1;

    } else {
        return NULL;
    }

    return def;
}

static const char str_800996d0[] = "objFreeObjdef: Error!! (%d)\n";

void obj_free_objdef(s32 tabIdx) {
    if (gObjDefRefCount[tabIdx] != 0) {
        gObjDefRefCount[tabIdx]--;

        if (gObjDefRefCount[tabIdx] == 0) {
            ObjDef *def;

            def = gLoadedObjDefs[tabIdx];
            
            if (def->pModLines != NULL) {
                mmFree(def->pModLines);
            }

            if (def->pIntersectPoints != NULL) {
                mmFree(def->pIntersectPoints);
            }

            mmFree(def);
        }
    }
}

ModLine *obj_load_objdef_modlines(s32 modLineNo, s16 *modLineCount) {
    ModLine *modLines;
    s32 fileSize;
    s32 totalEntries;
    s32 *tabEntry;
    s32 size;
    s32 offset;

    modLines = NULL;

    fileSize = get_file_size(MODLINES_TAB);
    totalEntries = (fileSize - 4) >> 2;

    if (modLineNo > totalEntries) {
        return NULL;
    }

    tabEntry = (s32*)mmAlloc(16, ALLOC_TAG_TEST_COL, NULL);
    read_file_region(MODLINES_TAB, (void*)tabEntry, modLineNo << 2, 8);

    offset = tabEntry[0];
    size = tabEntry[1] - tabEntry[0];

    if (size > 0) {
        modLines = (ModLine*)mmAlloc(size, ALLOC_TAG_TRACK_COL, NULL);
        read_file_region(MODLINES_BIN, (void*)modLines, offset, size);
    }

    mmFree(tabEntry);

    *modLineCount = (size / sizeof(ModLine));

    return modLines;
}

void doNothing_80022DD8(s32 a0, s32 a1, s32 a2) { }

s32 func_80022DEC(void) {
    return gObjIndexCount;
}

s32 func_80022DFC(s32 idx) {
    if (idx > gObjIndexCount) {
        return FALSE;
    }

    return gFile_OBJINDEX[idx] != -1;
}

static const char str_800996f0[] = "objGetControlNo objtype out of range %d/%d\n";

// unused
s16 func_80022E3C(s32 param1) {
    ObjDef *def;

    if (param1 > gObjIndexCount) {
        return 0;
    }

    param1 = gFile_OBJINDEX[param1];

    if (param1 == -1) {
        return 0;
    }

    def = (ObjDef*)(gFile_OBJECTS_TAB[param1] + D_800B18E8); // ???

    if (def->flags & 0x40) {
        return def->unka0;
    }

    return -1;
}

//https://decomp.me/scratch/CPQVO
s16 func_80022EC0(s32 arg0) {
    s8 pad_sp8C[76];
    s8 *var_v0;
    s8 pad_sp87;
    s8 sp86;
    s32 pad_sp80;
    s8 sp2C[84];
    s16 pad_sp28;
    s32 pad_sp24;
    s32 var_v1;

    if (gObjIndexCount < arg0) {
        return 0;
    }    

    arg0 = gFile_OBJINDEX[arg0];
    if (arg0 == -1) {
        return 0;
    }
    var_v0 = (s8*)(&sp86 - sp2C);

    var_v1 = 0;
    while ((u32)var_v0 & 1){
        var_v0 -= 1;
        var_v1 += 1;        
    }

    queue_load_file_region_to_ptr((void*)D_800B18E4, OBJECTS_BIN, (s32)(gFile_OBJECTS_TAB[arg0] + var_v0), 8);
    return D_800B18E4[var_v1];
}

void obj_free_object(Object *obj, s32 param2) {
    Object *obj2;
    /*sp+0xE4*/ LightAction lAction;
    ObjectAnim_Data *animObjdata;
    ModelInstance *modelInst;
    /*sp+0x40*/ Object *stackObjs[39]; // unknown exact length
    /*sp+0x3c*/ s32 k;
    /*sp+0x38*/ s32 numModels;
    /*sp+0x34*/ s32 i;
    /*sp+0x30*/ s32 numStackObjs;

    if (obj->dll != NULL) {
        update_pi_manager_array(4, obj->id);
        obj->dll->vtbl->free(obj, param2);
        update_pi_manager_array(4, -1);
        dll_unload(obj->dll);
    }

    gDLL_6_AMSFX->vtbl->func_1218(obj);
    gDLL_5_AMSEQ->vtbl->func17(obj);
    gDLL_13_Expgfx->vtbl->func9(obj);

    if (obj->def != NULL && obj->def->flags & 0x10) {
        obj_free_object_type(obj, 56);
    }

    if (obj->def->flags & 0x40) {
        obj_free_object_type(obj, 7);

        if (!param2) {
            numStackObjs = 0;

            for (i = 0; i < gNumObjs; i++) {
                obj2 = gObjList[i];

                if (obj == obj2->parent) {
                    obj2->parent = NULL;

                    if (obj2->setup != NULL) {
                        stackObjs[numStackObjs] = obj2;
                        numStackObjs++;
                    }
                }
            }

            for (i = 0; i < numStackObjs; i++) {
                obj_destroy_object(stackObjs[i]);
            }

            func_80045F48(obj->unk34);
        }
    }

    if (!param2 && obj->group == GROUP_UNK16) {
        for (i = 0; i < gNumObjs; i++) {
            obj2 = gObjList[i];
            if (obj == obj2->unkC0) {
                obj2->unkC0 = NULL;
            }
        }
    }

    for (k = 0; k < gNumObjs; k++) {
        obj2 = gObjList[k];
        if (obj2->group == GROUP_UNK16) {
            animObjdata = (ObjectAnim_Data*)obj2->data;
            if (obj == animObjdata->unk0) {
                animObjdata->unk0 = NULL;
                animObjdata->unk9C = 1;
            }
        }
    }
    

    if (obj->def->unk5e >= 1) {
        obj_free_object_type(obj, 9);
    }

    if (obj->def->unk87 & 0x10) {
        lAction.unk12.asByte = 2;
        lAction.unke = 0;
        lAction.unk10 = obj->unkD6;
        lAction.unk1b = 0;

        gDLL_11_Newlfx->vtbl->func0(obj, obj, &lAction, 0, 0, 0);
    }

    if (obj->shadow != NULL) {
        if (obj->def->shadowType == 1) {
            func_8004D974(1);
        }

        if (obj->shadow->texture != NULL) {
            texture_destroy(obj->shadow->texture);
        }

        if (obj->shadow->unk8 != NULL) {
            texture_destroy(obj->shadow->unk8);
        }
    }

    if (obj->mesgQueue != NULL) {
        mmFree(obj->mesgQueue);
        obj->mesgQueue = NULL;
    }

    numModels = obj->def->numModels;
    for (k = 0; k < numModels; k++) {
        if (obj->modelInsts[k] != NULL) {
            modelInst = obj->modelInsts[k];
            destroy_model_instance(modelInst);
        }
    }

    obj_free_objdef(obj->tabIdx);

    if (obj->unkB4 >= 0) {
        if (!param2) {
            gDLL_3_Animation->vtbl->func18((s32)obj->unkB4);
            obj->unkB4 = -1;
        }
    }

    if (obj->srt.flags & 0x2000 && obj->setup != NULL) {
        mmFree(obj->setup);
    }

    mmFree(obj);
}

void *obj_alloc_create_info(s32 size, s32 objId) {
    ObjSetup *setup;

    setup = (ObjSetup*)mmAlloc(size, ALLOC_TAG_OBJECTS_COL, NULL);
    bzero(setup, size);

    setup->uID = -1;
    setup->loadDistance = 100;
    setup->fadeDistance = 50;
    setup->loadFlags = OBJSETUP_LOAD_FLAG8;
    setup->fadeFlags = OBJSETUP_FADE_FLAG4;
    setup->objId = objId;

    return (void*)setup;
}

void func_80023464(s32 playerno) {
    Object *player;
    s32 activePlayerno;
    ObjSetup playerSetup;
    f32 x, y, z;
    Object *newPlayer;

    player = get_player();
    activePlayerno = gDLL_29_Gplay->vtbl->get_playerno();

    if (playerno != activePlayerno) {
        gDLL_29_Gplay->vtbl->set_playerno(playerno);

        if (player != NULL) {
            obj_destroy_object(player);
            x = player->srt.transl.x;
            y = player->srt.transl.y;
            z = player->srt.transl.z;
        } else {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }

        newPlayer = NULL;

        if (playerno > PLAYER_NONE) {
            bzero(&playerSetup, sizeof(playerSetup));
            playerSetup.uID = -1;
            playerSetup.setupExclusions1 = 0;
            playerSetup.loadFlags = OBJSETUP_LOAD_FLAG1;
            playerSetup.fadeFlags = OBJSETUP_FADE_FLAG4;
            playerSetup.loadDistance = 255;
            playerSetup.fadeDistance = 100;
            playerSetup.objId = D_80091664[playerno];
            playerSetup.quarterSize = 24;
            playerSetup.x = x;
            playerSetup.y = y;
            playerSetup.z = z;

            newPlayer = obj_create(&playerSetup, OBJ_INIT_FLAG1, -1, -1, NULL);
        }

        gDLL_2_Camera->vtbl->func0(newPlayer, x - 50.0f, y, z - 50.0f);
        gDLL_6_AMSFX->vtbl->func_480(newPlayer);
        gDLL_5_AMSEQ->vtbl->func3(newPlayer);
    }
}

void func_80023628() {
    Object *player;
    s32 mapType;
    ObjSetup playerSetup;
    PlayerLocation *savedPlayerLocation;
    f32 x, y, z;
    s32 playerno;

    mapType = map_get_type();
    if (mapType == MAPTYPE_2 || mapType == MAPTYPE_3) {
        obj_free_all();
        return;
    }

    playerno = gDLL_29_Gplay->vtbl->get_playerno();
    savedPlayerLocation = gDLL_29_Gplay->vtbl->get_player_saved_location();

    x = savedPlayerLocation->vec.x;
    y = savedPlayerLocation->vec.y;
    z = savedPlayerLocation->vec.z;

    player = NULL;

    if (playerno > PLAYER_NONE) {
        bzero(&playerSetup, sizeof(playerSetup));
        playerSetup.uID = -1;
        playerSetup.setupExclusions1 = 0;
        playerSetup.loadFlags = OBJSETUP_LOAD_FLAG1;
        playerSetup.fadeFlags = OBJSETUP_FADE_FLAG4;
        playerSetup.loadDistance = 255;
        playerSetup.fadeDistance = 100;
        playerSetup.objId = D_80091664[playerno];
        playerSetup.quarterSize = 24;
        playerSetup.x = x;
        playerSetup.y = y;
        playerSetup.z = z;

        player = obj_create(&playerSetup, OBJ_INIT_FLAG1, -1, -1, NULL);
    }

    D_80091668.unk8 = fsin16_precise(savedPlayerLocation->rotationY << 8) * 60.0f + x;
    D_80091668.unkC = y + 40.0f;
    D_80091668.unk10 = fcos16_precise(savedPlayerLocation->rotationY << 8) * 60.0f + z;

    gDLL_2_Camera->vtbl->func0(player, D_80091668.unk8, D_80091668.unkC, D_80091668.unk10);
    gDLL_2_Camera->vtbl->func6(0x54, 0, 0, 0x20, &D_80091668, 0, 0xFF);
    gDLL_2_Camera->vtbl->func1(1);
    gDLL_6_AMSFX->vtbl->func_480(player);
    gDLL_5_AMSEQ->vtbl->func3(player);

    D_800B1988 = 0;
    func_8004A67C();
}

void func_80023894(Object* object, s32 objectId) {
    SidekickSetup* sidekickSetup;

    sidekickSetup = (SidekickSetup*)object->setup;
    D_80091688.base.objId = objectId;
    D_80091688.base.x = object->positionMirror.x;
    D_80091688.base.y = object->positionMirror.y;
    D_80091688.base.z = object->positionMirror.z;
    D_80091688.unk18 = sidekickSetup->unk18;
    D_80091688.unk19 = sidekickSetup->unk19;

    obj_create((ObjSetup*)&D_80091688, OBJ_INIT_FLAG1, -1, -1, NULL);
}

Object *get_player(void) {
    Object **objectList;
    s32 count;

    objectList = obj_get_all_of_type(0, &count);

    if(count > 1) {
        STUBBED_PRINTF(" ERROR : Error in Get Main More Players Loaded ");
    }

    if (count) {
        return objectList[0];
    } else {
        return NULL;
    }
}

Object *get_sidekick() {
    Object **objectList;
    s32 count;

    objectList = obj_get_all_of_type(1, &count);

    if (count > 1) {
        STUBBED_PRINTF(" ERROR : ERROR : Error in Get Sidekicks Loaded  ");
    }

    if (count) {
        return objectList[0];
    } else {
        return NULL;
    }
}

void obj_clear_map_id(Object *obj) {
    obj->mapID = -1;
}

void obj_infer_map_id(Object *obj) {
    obj->mapID = map_get_map_id_from_xz_ws(obj->srt.transl.x, obj->srt.transl.z);
}

s32 obj_integrate_speed(Object *obj, f32 dx, f32 dy, f32 dz) {
    obj->srt.transl.x += dx;
    obj->srt.transl.y += dy;
    obj->srt.transl.z += dz;

    return 0;
}

void obj_set_update_priority(Object *obj, s8 priority) {
    obj->updatePriority = priority;
}

void func_80023A18(Object *obj, s32 modelIdx) {
    obj->unkB0 &= 0xF0FF;

    if (modelIdx == obj->modelInstIdx) {
        return;
    }

    if (modelIdx < 0) {
        modelIdx = 0;
    } else if (modelIdx >= obj->def->numModels) {
        modelIdx = obj->def->numModels - 1;
    }

    obj->unkB0 |= 0x800;
    obj->unkB0 |= (modelIdx << 8) & 0x700;
}

void func_80023A78(Object *obj, ModelInstance *modelInst, Model *model) {
    s32 modelInstIdx;
    ModelInstance *modelInst2;
    s32 prevAnimId;

    modelInstIdx = (obj->unkB0 & 0x700) >> 8;
    modelInst2 = obj->modelInsts[modelInstIdx];

    modelInst2->unk34 &= 0xFFF0;
    modelInst2->unk34 |= modelInst->unk34 & 7;

    prevAnimId = obj->curModAnimId;

    obj->modelInstIdx = modelInstIdx;
    obj->curModAnimId = -1;
    obj->curModAnimIdLayered = -1;

    func_80023D30(obj, prevAnimId, obj->animProgress, 0);

    if (obj->objhitInfo != NULL) {
        if ((obj->objhitInfo->unk5A & 0x10)) {
            obj->objhitInfo->unk9F = 2;
        }

        obj->objhitInfo->unk9E = 1;
    }

    obj->unkB0 &= 0xF0FF;
}

static const char str_80099780[] = "warning: objAddEffectBox max effect boxes\n";
static const char str_800997ac[] = "objFreeEffectBox: Not found\n";

void obj_add_effect_box(Object *obj) {
    gEffectBoxes[gEffectBoxCount] = obj;
    gEffectBoxCount += 1;
}

void obj_free_effect_box(Object *obj) {
    s32 i;
    s32 newCount;

    i = 0;

    while (i < gEffectBoxCount && obj != gEffectBoxes[i]) {
        i++;
    }

    newCount = gEffectBoxCount - 1;
    
    if (i != gEffectBoxCount) {
        while (i < newCount) {
            gEffectBoxes[i] = gEffectBoxes[i + 1];
            i++;
        }

        gEffectBoxCount = newCount;
    }
}

/** Set interaction arrow params? */
void func_80023BF8(Object *obj, s32 param2, s32 param3, s32 param4, u8 param5, u8 colourIndex) {
    ObjectStruct78 *dst;

    if (obj != NULL) {
        dst = obj->unk78;

        if (dst != NULL) {
            dst += obj->unkD4;

            if (param2 != 0) {
                dst->unk0 = param2 >> 2;
            }

            if (param4 != 0) {
                dst->unk1 = param4 >> 2;
            }

            if (param3 != 0) {
                dst->unk2 = param3 >> 2;
            }

            if (param5 != 0) {
                dst->unk3 = param5;
            }

            if (colourIndex != 0) {
                dst->colourIndex = colourIndex ;
            }
        }
    }
}

void func_80023C6C(Object *obj) {
    ObjDefStruct40 *src;
    ObjectStruct78 *dst;

    if (obj != NULL) {
        dst = obj->unk78;

        if (dst != NULL) {
            src = &obj->def->unk40[obj->unkD4];
            dst += obj->unkD4;

            dst->unk0 = src->unk0c;
            dst->unk1 = src->unk0d;
            dst->unk2 = src->unk0e;
            dst->unk3 = src->unk0f;
            dst->colourIndex = src->unk10;
        }
    }
}

static const char str_800997cc[] = "locknum out of range\n";
static const char str_800997e4[] = "infonum out of range\n";

void func_80023CD8(Object *obj, u16 param2) {
    if (param2 > obj->def->unk9b) {
        param2 = 0;
    }

    obj->unkD4 = param2;
}

void func_80023D08(Object *obj, u16 param2) {
    if (param2 > 4) {
        param2 = 0;
    }

    obj->unkD8 = param2;
}
