#include "PR/ultratypes.h"
#include "dlls/engine/29_gplay.h"
#include "game/objects/object_id.h"
#include "sys/camera.h"
#include "sys/pi.h"
#include "sys/gfx/model.h"
#include "sys/asset.h"
#include "sys/dll.h"
#include "sys/exception.h"
#include "sys/linked_list.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/objanim.h"
#include "sys/objtype.h"
#include "sys/objhits.h"
#include "sys/newshadows.h"
#include "sys/objlib.h"
#include "macros.h"
#include "dll.h"

#define MAX_OBJECTS 180

typedef struct {
/*00*/  u8 _unk0[0x8 - 0x0];
/*08*/  f32 unk8;
/*0C*/  f32 unkC;
/*10*/  f32 unk10;
/*14*/  u8 _unk14[0x20 - 0x14];
} UnkStruct_80091668;

enum ObjFreeMode {
    OBJFREEMODE_FREE_ALL = 0,
    OBJFREEMODE_IMMEDIATE = 1, // unused
    OBJFREEMODE_DEFERRED = 2
};

// -------- .data start 80091660 -------- //
s32 sObjFreeMode = OBJFREEMODE_DEFERRED;
s16 sObjPlayerObjIDs[2] = {OBJ_Sabre, OBJ_Krystal};
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
        /*loadFlags=*/OBJSETUP_LOAD_LEVEL,
        /*fadeFlags=*/{ OBJSETUP_FADE_CAMERA },
        /*loadDistance=*/{ 0xFF },
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
s16 sObjListVisibleStartIdx;
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
Object **sObjLockList;
s32 sObjLockListCount;
Object **gObjList; //global object list
s32 gNumObjs;
LinkedList gObjUpdateList;
s8 gEffectBoxCount;
Object *gEffectBoxes[20];
s32 D_800B1988;
// -------- .bss end 800b1990 -------- //

void objClearAll(void);
void objInitObject(Object *obj, ObjSetup *setup, s32 reset);
void objUpdateObjModels(void);
void objControlObject(Object *obj);
void objUpdateObject(Object *obj);
ModLine *objLoadObjdefModlines(s32 modLineNo, s16 *modLineCount);
ObjDef *objLoadObjdef(s32 tabIdx);
u32 objGetModelFlags(Object *obj);
u32 objCalcMemSize(Object *obj, ObjDef *def, u32 flags);
void objFreeObjdef(s32 tabIdx);
void obj_func_80021E74(f32 scale, ModelInstance *modelInst);
void objModelLoadFailed(Object *obj, s32 param2, s32 param3);
u32 objAllocDLLData(Object *obj, u32 addr);
u32 objInitEventData(s32 param1, Object *obj, u32 addr);
u32 obj_func_8002298C(s32 param1, ModelInstance *param2, Object *obj, u32 addr);
f32 objCalcVisRadius(Object *obj);
void objFreeObjectInternal(Object *obj, s32 onlySelf);

void objInit(void) {
    int i;

    //allocate some buffers
    gObjDeferredFreeList = mmAlloc(sizeof(Object*) * MAX_OBJECTS, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("obj:dellist"));
    sObjLockList = mmAlloc(sizeof(Object*) * 24, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("obj:locklist"));
    D_800B18E4 = mmAlloc(0x10, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("obj:contnobuf"));

    //load OBJINDEX.BIN and count number of entries
    assetRomLoad((void **) (&gFile_OBJINDEX), OBJINDEX_BIN);
    gObjIndexCount = (piRomGetFileSize(OBJINDEX_BIN) >> 1) - 1;
    while(!gFile_OBJINDEX[gObjIndexCount]) gObjIndexCount--;

    //load OBJECTS.TAB and count number of entries
    assetRomLoad((void **)&gFile_OBJECTS_TAB, OBJECTS_TAB);
    gNumObjectsTabEntries = 0;
    while(gFile_OBJECTS_TAB[gNumObjectsTabEntries] != -1) gNumObjectsTabEntries++;
    gNumObjectsTabEntries--;

    //init ref count and pointers
    gLoadedObjDefs = mmAlloc(gNumObjectsTabEntries * 4, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("obj:deflist"));
    gObjDefRefCount = mmAlloc(gNumObjectsTabEntries, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("obj:defno"));
    for(i = 0; i < gNumObjectsTabEntries; i++) gObjDefRefCount[i] = 0; //why not memset?

    //load TABLES.BIN and TABLES.TAB and count number of entries
    assetRomLoad((void **) (&gFile_TABLES_BIN), TABLES_BIN);
    assetRomLoad((void **) (&gFile_TABLES_TAB), TABLES_TAB);
    gNumTablesTabEntries = 0;
    while(gFile_TABLES_TAB[gNumTablesTabEntries] != -1) gNumTablesTabEntries++;

    //allocate global object list and some other buffers
    gObjList = mmAlloc(sizeof(Object*) * MAX_OBJECTS, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("obj:ObjList"));
    objHitInit();
    objClearAll();
}

void objTick(void) {
    void *node;
    Object *obj;
    Object *player;
    s32 nextFieldOffset;

    nextFieldOffset = gObjUpdateList.nextFieldOffset; // == 0x38, &obj->next

    func_80058FE8();

    objUpdateObjModels();
    objHitUpdateHitModels(gNumObjs);

    node = gObjUpdateList.head;

    for (obj = (Object*)node; node != NULL && obj->updatePriority == OBJPRIORITY_ANIM; obj = (Object*)node) {
        objControlObject(obj);
        node = *((void**)(nextFieldOffset + (u32)node));

        if (obj->objhitInfo->unk58){} // fake match
    }

    for (obj = (Object*)node; node != NULL && (obj->def->flags & OBJDEF_IS_MOBILE_MAP); obj = (Object*)node) {
        objControlObject(obj);
        obj->matrixIdx = camAllocObjectMatrix(obj);
        node = *((void**)(nextFieldOffset + (u32)node));
    }

    func_80025E58();

    while (node != NULL) {
        obj = (Object*)node;

        if (obj->objhitInfo != NULL) {
            if (obj->objhitInfo->unk5A != 8 || (obj->objhitInfo->unk58 & 1) == 0) {
                objControlObject(obj);
            }
        } else {
            objControlObject(obj);
        }

        node = *((void**)(nextFieldOffset + (u32)node));
    }

    player = objGetPlayer();
    if (player != NULL && player->linkedObject != NULL) {
        player->linkedObject->parent = player->parent;
        objControlObject(player->linkedObject);
    }

    objHitDoHitDetection(gNumObjs);

    node = gObjUpdateList.head;
    while (node != NULL) {
        obj = (Object*)node;
        objUpdateObject(obj);
        node = *((void**)(nextFieldOffset + (u32)node));
    }

    player = objGetPlayer();
    if (player != NULL && player->linkedObject != NULL) {
        player->linkedObject->parent = player->parent;
        objUpdateObject(player->linkedObject);
    }

    gDLL_24_Waterfx->vtbl->tick(gUpdateRate);
    gDLL_15_Projgfx->vtbl->func2(gUpdateRate, 0);
    gDLL_14_Modgfx->vtbl->func2(0, 0, 0);
    gDLL_13_Expgfx->vtbl->func2(0, gUpdateRate, 0, 0);

    func_8002B6EC();

    gDLL_3_Animation->vtbl->tick();
    gDLL_3_Animation->vtbl->update_camera();
    gDLL_2_Camera->vtbl->tick(gUpdateRate);

    write_c_file_label_pointers("objects/objects.c", 361);
}

void objDoNothing_80020A40(void) {}

void objUpdateObjModels(void) {
    int i;
    int j;
    int k;
    Object *object;
    ModelInstance *modelInst;
    AnimObj_Data *unk1;

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
                        unk1 = object->animObj != NULL ? (AnimObj_Data*)object->animObj->data : NULL;

                        if (object->animObj == NULL || (unk1 != NULL && unk1->unk62 == 0)) {
                            mod_func_8001B084(modelInst, gUpdateRateF);
                        }
                    }
                }
            }
        }
    }
}

void objDoDeferredFree(void) {
    int i;
    for(i = 0; i < gObjDeferredFreeListCount; i++) {
        objFreeObjectInternal(gObjDeferredFreeList[i], FALSE);
        gObjDeferredFreeList[i] = 0;
    }
    gObjDeferredFreeListCount = 0;
}

void objFreeAll(void) {
    s32 i;
    
    objDoDeferredFree();
    sObjFreeMode = OBJFREEMODE_FREE_ALL;

    i = 0;
    while (gNumObjs != 0) {
        objFreeObject(gObjList[i]);
        i++;
        if (i >= gNumObjs) {
            i = 0;
        }
    }

    objDoDeferredFree();
    sObjFreeMode = OBJFREEMODE_DEFERRED;

    gObjDeferredFreeListCount = 0;
    // @bug: This resets the locklist without actually freeing anything on it!
    sObjLockListCount = 0;
    gNumObjs = 0;

    linkedListInit(&gObjUpdateList, OFFSETOF(Object, next));

    objClearAll();

    gDLL_2_Camera->vtbl->store_player(NULL, 0);
}

void objClearAll(void) {
    gObjDeferredFreeListCount = 0;
    sObjLockListCount = 0;
    D_800B1988 = 0;
    gNumObjs = 0;

    linkedListInit(&gObjUpdateList, OFFSETOF(Object, next));

    sObjListVisibleStartIdx = 0;
    objTypeInit();
    func_80025DF0();
}

void objMarkVisibilitySortDirty(void) {
    sObjListVisibleStartIdx = 0;
}

/** 
 * Sorts the object list such that all invisible objects are before visible objects. 
 * Does not affect sort beyond that. 
 */
s32 objVisibilitySortObjects(s32 *numObjs) {
    s32 objsEnd;
    s32 idx1;
    s32 idx2;
    s32 endLoop;

    *numObjs = gNumObjs;

    if (0) {}

    if (sObjListVisibleStartIdx != 0) {
        return sObjListVisibleStartIdx;
    }

    idx1 = 0;
    idx2 = gNumObjs - 1;
    objsEnd = idx2;

    while (idx1 <= idx2) {
        endLoop = 0;
        while (idx1 <= objsEnd && !endLoop) {
            if ((gObjList[idx1]->def->flags & OBJDEF_INVISIBLE) != 0) {
                idx1++;
            } else {
                endLoop = -1;
            }
        }

        endLoop = 0;
        while (idx2 >= 0 && !endLoop) {
            if ((gObjList[idx2]->def->flags & OBJDEF_INVISIBLE) == 0) {
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

    sObjListVisibleStartIdx = idx1;
    return idx1;
}

void objDepthSortObjects(s32 start, s32 end) {
    s32 i;
    Object *obj;
    s32 endLoop;

    if (start <= end) {
        for (i = start; i <= end; i++) {
            obj = gObjList[i];

            if (obj->def->flags & OBJDEF_STATIC_DEPTH_SORT) {
                obj->depthSortVal = obj->def->staticDepthSortVal * 1000;
            } else {
                // dot product based depth sorting
                obj->depthSortVal = -camGetAngleToPoint(obj->globalPosition.x, obj->globalPosition.y, obj->globalPosition.z);
            }
        }

        do {
            endLoop = TRUE;

            for (i = start; i < end; i++) {
                if (gObjList[i]->depthSortVal < gObjList[i + 1]->depthSortVal) {
                    obj = gObjList[i];
                    gObjList[i] = gObjList[i + 1];
                    gObjList[i + 1] = obj;
                    
                    endLoop = FALSE;
                }
            }
        } while (!endLoop);
    }
}

void objHandleAnimseqActors(void) {
    s32 i;
    Object *obj;
    Object *var;

    for (i = 0; i < gNumObjs; i++) {
        obj = gObjList[i];

        if (obj->animObj != NULL) {
            var = obj->animObj;

            if (obj->parent == NULL) {
                if (var->parent != NULL) {
                    obj->parent = var->parent;
                }
            }
            
            obj->animObj = NULL;
        }
    }
}

Object **objGetObjects(s32 *param1, s32 *numObjs) {
    if (param1 != NULL) {
        *param1 = 0;
    }
    if (numObjs != NULL) {
        *numObjs = gNumObjs;
    }

    return gObjList;
}

Object *objGetObject(s32 idx) {
    if (idx < 0 || idx >= gNumObjs) {
        return NULL;
    }

    return gObjList[idx];
}

Object *objGetObjectByUID(s32 uID) {
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

s32 objGetNumObjects(void) {
    return gNumObjs;
}

s32 objRet0_800212E8(void) { return 0; }

// official name: objGetSequence
s32 objGetSequence(s32 objId, s32 idx) {
    ObjDef *objDef;
    s32 seq;
    
    seq = -1;

    if (objId > gObjIndexCount) {
        STUBBED_PRINTF("objGetSequence objtype out of range %d/%d\n", objId, gObjIndexCount);
        return -1;
    } else {
        objId = gFile_OBJINDEX[objId]; // turns objId into tabIdx
        objDef = objLoadObjdef(objId);

        if (objDef != NULL) {
            seq = -1;

            if (objDef->pSeq != NULL && idx >= 0 && idx < objDef->numSequences) {
                seq = objDef->pSeq[idx];
            }

            objFreeObjdef(objId);
        }
    }

    return seq;
}

void *objGetTable(s32 idx) {
    if (idx < 0 || idx >= gNumTablesTabEntries) {
        return gFile_TABLES_BIN;
    }

    return (void*)((u32)gFile_TABLES_BIN + gFile_TABLES_TAB[idx] * 4);
}

Object *objSetupObject(ObjSetup *setup, u32 initFlags, s32 mapID, s32 param4, Object *parent) {
    Object *obj;

    obj = NULL;
    assetLoadObject(&obj, setup, initFlags, mapID, param4, parent, 0);
    objAddObject(obj, initFlags);

    return obj;
}

// official name: objSetupObjectActual
Object *objSetupObjectActual(ObjSetup *setup, u32 initFlags, s32 mapID, s32 param4, Object *parent, s32 param6) {
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

    if (initFlags & OBJINIT_BY_TABIDX) {
        tabIdx = objId;
    } else {
        if (objId > gObjIndexCount) {
            STUBBED_PRINTF("objSetupObjectActual objtype out of range %d/%d\n", objId, gObjIndexCount);
            update_pi_manager_array(0, -1);
            return NULL;
        }

        tabIdx = gFile_OBJINDEX[objId];
    }

    bzero(&objHeader, sizeof(Object));

    objHeader.def = objLoadObjdef(tabIdx);
    def = objHeader.def;

    if (def == NULL || (u32)def == 0xFFFFFFFF) {
        STUBBED_PRINTF("Warning: Unknown object type '%d/%d romdefno %d', using DummyObject (128)\n",
            tabIdx, setup->objId, objHeader.id);
        /* default.dol
        if ((u32)def == 0xFFFFFFFF) {
            STUBBED_PRINTF("Warning: Object romdefno is -1, check the object is in objects.spec");
        }
        */
        return NULL;
    } 
    
    objHeader.srt.flags = OBJFLAG_UNK_2;

    if (def->flags & OBJDEF_FORCE_TRANSPARENT_DRAW_ORDER) {
        objHeader.srt.flags |= OBJFLAG_FORCE_TRANSPARENT_DRAW_ORDER;
    }

    if (def->flags & OBJDEF_FLAG40000) {
        objHeader.stateFlags |= OBJSTATE_UNK80;
    }

    if (initFlags & OBJINIT_FLAG4) {
        objHeader.srt.flags |= OBJFLAG_OWNS_SETUP;
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
    objHeader.seqSlot = SEQSLOT_NONE;
    objHeader.srt.scale = def->scale;
    objHeader.opacity = 255;
    objHeader.mesgQueue = NULL;
    objHeader.loadDistance = setup->loadDistance * 8;
    objHeader.fadeDistance = setup->fadeDistance * 8;
    objHeader.dll = NULL;

    if (def->dllID != 0) {
        objHeader.dll = (DLL_IObject*)dllLoadActual(def->dllID, 6, 1);
        // "OBJECTS: warning DLL load failed\n" (default.dol)
    }

    modflags = objGetModelFlags(&objHeader);

    if (def->flags & OBJDEF_FLAG20) {
        modflags &= ~MODFLAGS_1;
    } else {
        modflags |= MODFLAGS_1;
    }

    if (def->shadowType != OBJ_SHADOW_NONE) {
        modflags |= MODFLAGS_SHADOW;
    } else {
        modflags &= ~MODFLAGS_SHADOW;
    }

    if (def->flags & OBJDEF_INVISIBLE) {
        modflags |= MODFLAGS_DONT_LOAD_MODEL;
    }

    var = objCalcMemSize(&objHeader, def, modflags);

    obj = (Object*)mmAlloc(var, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("obj"));

    if (obj == NULL) {
        STUBBED_PRINTF("ObjSetupObject(3) Memory fail!!\n");
        objFreeObjdef(tabIdx);
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
                obj->modelInsts[var] = modLoadModelActual(-def->pModelList[var], modflags);

                if (obj->modelInsts[var] == NULL) {
                    modelLoadFailed = TRUE;
                    goto modelLoadFailedLabel;
                } else {
                    tempModel = obj->modelInsts[var];
                    obj_func_80021E74(obj->srt.scale, tempModel);
                }
            }
        } else {
            for (; var < modelCount; var++) {
                obj->modelInsts[var] = modLoadModelActual(-def->pModelList[var], modflags);
                if (obj->modelInsts[var] == NULL) {
                    modelLoadFailed = TRUE;
                } else {
                    tempModel = obj->modelInsts[var];
                    obj_func_80021E74(obj->srt.scale, tempModel);
                }
            }
        }
    }

    modelLoadFailedLabel:
    if (modelLoadFailed) {
        objModelLoadFailed(obj, modelCount, objId);
        objFreeObjdef(tabIdx);
        return NULL;
    }
     
    addr = objAllocDLLData(obj, (u32)&obj->modelInsts[def->numModels]);

    if (modflags & MODFLAGS_EVENTS) {
        addr = objInitEventData(obj->id, obj, addr);
    }

    if (modflags & MODFLAGS_100) {
        addr = obj_func_8002298C(obj->id, obj->modelInsts[0], obj, addr);
    }

    if ((modflags & MODFLAGS_SHADOW) && (def->shadowType != OBJ_SHADOW_NONE)) {
        addr = shadowsInitObjShadow(obj, addr, 0);
    }

    obj->visRadius = objCalcVisRadius(obj) * obj->srt.scale;

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

    if (def->numLockdata != 0) {
        obj->unk74 = (ObjectStruct74*)mmAlign4(addr);
        addr = (u32)obj->unk74 + (def->numLockdata * sizeof(ObjectStruct74));
    }

    if (def->unk8F != 0 && def->unk74 != 0) {
        addr = mmAlign4(addr);
        addr = func_80026A20(obj->id, obj->modelInsts[0], obj->objhitInfo, addr, obj);
    }

    if (def->numLockdata != 0) {
        obj->unk78 = (ObjectStruct78*)mmAlign4(addr);

        for (j = 0; j < def->numLockdata; j++) {
            obj->unk78[j].flags = def->lockdata[j].flags;
            obj->unk78[j].interactRadius = def->lockdata[j].interactRadius;
            obj->unk78[j].hlAngularRange = def->lockdata[j].hlAngularRange;
            obj->unk78[j].lockExitRadius = def->lockdata[j].lockExitRadius;
            obj->unk78[j].hlRadius = def->lockdata[j].hlRadius;
        }

        // addr = (u32)obj->unk78 + (def->unk9b * sizeof(ObjectStruct78)); // default.dol
    }

    // default.dol (size is mmAlloc size)
    // if (size != (addr - (s32)obj)) {
    //     // "objects.c: objSetupObject: sizes do not match\n"
    // }

    obj->parent = parent;
    
    return obj;
}

static const char str_800995c4[] = "ObjSetupObject(2) Memory fail!!\n";
static const char str_800995e8[] = "ObjList Overflow %d!!!\n";

void objAddObject(Object *obj, u32 initFlags) {
    if (obj->parent != NULL) {
        camTransformPointByObject(
            obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z,
            &obj->globalPosition.x, &obj->globalPosition.y, &obj->globalPosition.z,
            obj->parent
        );
    } else {
        obj->globalPosition.x = obj->srt.transl.x;
        obj->globalPosition.y = obj->srt.transl.y;
        obj->globalPosition.z = obj->srt.transl.z;
    }

    obj->prevLocalPosition.x = obj->srt.transl.x;
    obj->prevLocalPosition.y = obj->srt.transl.y;
    obj->prevLocalPosition.z = obj->srt.transl.z;

    obj->prevGlobalPosition.x = obj->globalPosition.x;
    obj->prevGlobalPosition.y = obj->globalPosition.y;
    obj->prevGlobalPosition.z = obj->globalPosition.z;

    objInitObject(obj, obj->setup, FALSE);

    if (obj->objhitInfo != NULL) {
        obj->objhitInfo->unk10.x = obj->srt.transl.x;
        obj->objhitInfo->unk10.y = obj->srt.transl.y;
        obj->objhitInfo->unk10.z = obj->srt.transl.z;

        obj->objhitInfo->unk20.x = obj->srt.transl.x;
        obj->objhitInfo->unk20.y = obj->srt.transl.y;
        obj->objhitInfo->unk20.z = obj->srt.transl.z;
    }

    if (obj->def->mobileMapID > -1) {
        mapLoadMobileMap(obj->def->mobileMapID, obj);
    }

    update_pi_manager_array(0, -1);

    if (obj->def->flags & OBJDEF_IS_MOBILE_MAP) {
        objAddObjectType(obj, OBJTYPE_MobileMap);

        if (obj->updatePriority != OBJPRIORITY_MOBILE_MAP) {
            objSetPriority(obj, OBJPRIORITY_MOBILE_MAP);
        }
    } else {
        if (obj->updatePriority == 0) {
            objSetPriority(obj, OBJPRIORITY_DEFAULT);
        }
    }

    if (initFlags & OBJINIT_STANDALONE) {
        obj->stateFlags |= OBJSTATE_STANDALONE;
        gObjList[gNumObjs] = obj;
        gNumObjs += 1;

        /* default.dol
        if (gNumObjs >= MAX_OBJECTS) {
            // "Failed assertion ObjListSize<MAX_OBJECTS"
        }
        */

        objEnable(obj);
    }

    if (obj->def->unk5e >= 1) {
        objAddObjectType(obj, OBJTYPE_LookAt);
    }

    // Resorting by visibility isn't necessary if the object is visible since the object
    // was added to the end of the list where the visible objects are.
    if (obj->def->flags & OBJDEF_INVISIBLE) {
        objMarkVisibilitySortDirty();
    }

    if (obj->def->flags & OBJDEF_FLAG10) {
        objAddObjectType(obj, OBJTYPE_56);
    }

    write_c_file_label_pointers("objects/objects.c", 1143);
}

u32 objCalcMemSize(Object *obj, ObjDef *def, u32 modflags) {
    u32 size;

    size = sizeof(Object);

    size += def->numModels * sizeof(u32);

    if (obj->dll != NULL) {
        size += obj->dll->vtbl->GetDataSize(obj, size);
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

    if ((modflags & MODFLAGS_SHADOW) && (def->shadowType != OBJ_SHADOW_NONE)) {
        size = mmAlign4(size);
        size += sizeof(ObjectShadow);
    }

    if (def->unk8F != 0) {
        size = mmAlign4(size);
        size += sizeof(ObjectHitInfo);

        if (def->unk93 & 8) {
            size += sizeof(ObjectPolyhits);
        }
    }

    if (def->numSequenceBones != 0) {
        size = mmAlign4(size);
        size += def->numSequenceBones * (sizeof(s16) * 9);
    }

    if (def->numAnimatedFrames != 0) {
        size = mmAlign4(size);
        size += def->numAnimatedFrames * sizeof(Vtx);
    }

    if (def->numLockdata != 0) {
        size = mmAlign4(size);
        size += def->numLockdata * sizeof(ObjectStruct74);
    }

    if (def->unk8F != 0 && def->unk74 != 0) {
        size = mmAlign8(size);
        size += 300;
    }

    if (def->numLockdata != 0) {
        size = mmAlign4(size);
        size += def->numLockdata * sizeof(ObjectStruct78);
    }

    return size;
}

static const char str_80099614[] = " No Points ";

void obj_func_80021E74(f32 scale, ModelInstance *modelInst) {
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
                temp_v0_2->unkC[i] = VECTOR_MAGNITUDE(sp2EC) * scale;
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

f32 objCalcVisRadius(Object *obj) {
    s32 i;
    f32 visRadius;

    visRadius = 10.0f; // minimum radius

    for (i = 0; i < obj->def->numModels; i++) {
        if (obj->modelInsts[i] != NULL) {
            f32 vertRadius = obj->modelInsts[i]->model->maxAnimatedVertDistance;

            if (vertRadius > visRadius) {
                visRadius = vertRadius;
            }
        }
    }

    if (visRadius < obj->def->minVisRadiusSixteenth) {
        visRadius = obj->def->minVisRadiusSixteenth * 16.0f;
    }

    return visRadius;
}

void objModelLoadFailed(Object *obj, s32 modelCount, s32 objID) {
    s32 i;

    for (i = 0; i < modelCount; i++) {
        if (obj->modelInsts[i] != NULL) {
            ModelInstance *modelInst = obj->modelInsts[i];
            modFreeModel(modelInst);
        }
    }
}

void objDisable(Object *obj) {
    if (obj->stateFlags & OBJSTATE_STANDALONE) {
        linkedListRemove(&gObjUpdateList, obj);
    }
}

void objEnable(Object *obj) {
    void *insertAfter;
    void *node;
    Object *objNode;
    s32 nextFieldOffset;
    
    if (obj->stateFlags & OBJSTATE_STANDALONE) {
        nextFieldOffset = gObjUpdateList.nextFieldOffset;
        insertAfter = NULL;

        node = gObjUpdateList.head;

        // Insert maintaining sort by update priority
        for (objNode = (Object*)node; node != NULL && obj->updatePriority < ((Object*)node)->updatePriority; objNode = (Object*)node) {
            insertAfter = objNode;
            node = *((void**)(nextFieldOffset + (u32)node));
        }

        linkedListInsert(&gObjUpdateList, insertAfter, (void*)obj);
    }
}

void objFreeObject(Object *obj) {
    s32 i;
    s32 k;

    if (obj == NULL) {
        // "Failed assertion obj" (default.dol)
        *((volatile s8*)NULL) = 0;
        return;
    }

    if (!(obj->stateFlags & OBJSTATE_DESTROYED)) {
        if (obj->unkD9 != 0) {
            objRemoveTouchCallbacksForObj(obj);
        }

        if (obj->stateFlags & OBJSTATE_STANDALONE) {
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

            objDisable(obj);
            objMarkVisibilitySortDirty();
        }

        obj->stateFlags |= OBJSTATE_DESTROYED;

        if (obj->freeLock != 0) {
            for (i = 0; i < sObjLockListCount; i++) {
                if (obj == sObjLockList[i]) {
                    break;
                }
            }

            if (i == sObjLockListCount) {
                sObjLockList[sObjLockListCount] = obj;
                sObjLockListCount += 1;
            } else {
                STUBBED_PRINTF("objFreeTick %08x locked %d,already on list\n", obj, obj->freeLock);
            }
        } else if (sObjFreeMode == OBJFREEMODE_DEFERRED) {
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
                
                if (gObjDeferredFreeListCount == MAX_OBJECTS) {
                    STUBBED_PRINTF("objFreeObject: delete list size overrun\n");
                    gObjDeferredFreeListCount--;
                }
            }
        } else {
            objFreeObjectInternal(obj, /*onlySelf*/sObjFreeMode == OBJFREEMODE_FREE_ALL);
        }
    }
}

void objInitObject(Object *obj, ObjSetup *setup, s32 reset) {
    DLL_IObject *dll;
    obj->controlNo = obj->def->controlNo;
    dll = obj->dll;
    if (1) {
        if (dll != NULL) {
            obj->dll->vtbl->Setup(obj, setup, reset);
        }
    }

    obj->prevLocalPosition.x = obj->srt.transl.x;
    obj->prevGlobalPosition.x = obj->srt.transl.x;
    obj->prevLocalPosition.y = obj->srt.transl.y;
    obj->prevGlobalPosition.y = obj->srt.transl.y;
    obj->prevLocalPosition.z = obj->srt.transl.z;
    obj->prevGlobalPosition.z = obj->srt.transl.z;
}

void objControlObject(Object *obj) {
    if (obj->stateFlags & OBJSTATE_DESTROYED) {
        return;
    }

    if (obj->animObj != NULL) {
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

        if (!(obj->srt.flags & OBJFLAG_MANUAL_PREV_POSITIONS)) {
            obj->prevLocalPosition.x = obj->srt.transl.x;
            obj->prevLocalPosition.y = obj->srt.transl.y;
            obj->prevLocalPosition.z = obj->srt.transl.z;

            obj->prevGlobalPosition.x = obj->globalPosition.x;
            obj->prevGlobalPosition.y = obj->globalPosition.y;
            obj->prevGlobalPosition.z = obj->globalPosition.z;
        }

        if (obj->dll != NULL && !(obj->stateFlags & OBJSTATE_CONTROL_DISABLED)) {
            obj->dll->vtbl->Control(obj);

            camGetObjectChildPosition(obj,
                &obj->globalPosition.x, &obj->globalPosition.y, &obj->globalPosition.z);
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

        if (obj->polyhits != NULL) {
            obj->polyhits->unk10F = 0;
        }

        obj->unkAF &= ~(0x1 | 0x2 | 0x4);

        update_pi_manager_array(1, -1);
    }
}

void objUpdateObject(Object *obj) {
    update_pi_manager_array(3, obj->id);

    if (obj->dll != NULL && !(obj->stateFlags & OBJSTATE_UPDATE_DISABLED)) {
        obj->dll->vtbl->Update(obj);

        camGetObjectChildPosition(obj,
            &obj->globalPosition.x, &obj->globalPosition.y, &obj->globalPosition.z);
    }

    update_pi_manager_array(3, -1);
}

u32 objAllocDLLData(Object *obj, u32 addr) {
    u32 dataSize = 0;
    
    addr = mmAlign4(addr);

    if (obj->dll != NULL) {
        dataSize = obj->dll->vtbl->GetDataSize(obj, addr);
    }

    if (dataSize != 0) {
        obj->data = (void*)addr;
        addr += dataSize;
    } else {
        obj->data = NULL;
    }

    return addr;
}

u32 objGetModelFlags(Object *obj) {
    if (obj->dll != NULL) {
        return obj->dll->vtbl->GetModelFlags(obj);
    } else {
        return MODFLAGS_NONE;
    }
}

u32 objInitEventData(s32 objId, Object *obj, u32 addr) {
    obj->curEvent = (ObjectEvent*)mmAlign4(addr);

    addr = mmAlign8((u32)obj->curEvent + sizeof(ObjectEvent));
    obj->curEvent->data = (UNK_PTR*)addr;

    addr += 0x50;

    objLoadEvent(obj, objId, obj->curEvent, 0, /*directLoad=*/TRUE);

    return addr;
}

void objLoadEvent(Object *obj, s32 objId, ObjectEvent *outEvent, s32 id, u8 directLoad) {
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
                STUBBED_PRINTF("objects.c: event data size overflow\n");
                outEvent->size = 80;
            }

            if (eventList) {}

            if (!directLoad) {
                assetRomLoadSection((void**)outEvent->data, OBJEVENT_BIN, offset, outEvent->size);
            } else {
                piRomLoadSection(OBJEVENT_BIN, outEvent->data, offset, outEvent->size);
            }

            break;
        }
    }
}

u32 obj_func_8002298C(s32 objId, ModelInstance *param2, Object *obj, u32 addr) {
    if (param2 == 0) {
        return addr;
    }

    obj->unk5C = (ObjectEvent*)mmAlign4(addr);

    addr = mmAlign8((u32)obj->unk5C + sizeof(ObjectEvent));
    obj->unk5C->data = (UNK_PTR*)addr;

    return addr + 0x400;
}

void objLoadWeapondata(Object *obj, s32 param2, BinFileEntry *outParam, s32 id, u8 yieldLoad) {
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
                // "Weapon data array size overflow\n" (default.dol)
                outParam->size = 1024;
            }

            if (weaponDataList) {}

            if (yieldLoad) {
                assetRomLoadSection((void**)outParam->data, WEAPONDATA_BIN, offset, outParam->size);
            } else {
                piRomLoadSection(WEAPONDATA_BIN, outParam->data, offset, outParam->size);
            }

            break;
        }
    }
}

ObjDef *objLoadObjdef(s32 tabIdx) {
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

    def = (ObjDef*)mmAlloc(fileSize, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("obj:def"));
    if (def != NULL) {
        piRomLoadSection(OBJECTS_BIN, (void*)def, fileOffset, fileSize);

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

        if (def->collectableDef != 0) {
            def->collectableDef = (CollectableDef*)((u32)def + (u32)def->collectableDef);
        }

        if (def->lockdata != 0) {
            def->lockdata = (ObjDefLockData*)((u32)def + (u32)def->lockdata);
        }

        if (def->pSeq != 0) {
            def->pSeq = (s16*)((u32)def + (u32)def->pSeq);
        }

        def->pModLines = NULL;

        def->pAttachPoints = (AttachPoint*)((u32)def + (u32)def->pAttachPoints);

        def->pIntersectPoints = NULL;

        if (def->modLineNo > -1) {
            STUBBED_PRINTF("ob %d fileno %d\n", tabIdx, def->modLineNo);
            def->pModLines = objLoadObjdefModlines(def->modLineNo, &def->modLineCount);
            func_800596BC(def);
        }

        gLoadedObjDefs[tabIdx] = def;
        gObjDefRefCount[tabIdx] = 1;

    } else {
        STUBBED_PRINTF("Objects out of ram(1) !!\n");
        return NULL;
    }

    return def;
}

// official name: objFreeObjdef
void objFreeObjdef(s32 tabIdx) {
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
    } else {
        STUBBED_PRINTF("objFreeObjdef: Error!! (%d)\n", tabIdx);
    }
}

ModLine *objLoadObjdefModlines(s32 modLineNo, s16 *modLineCount) {
    ModLine *modLines;
    s32 fileSize;
    s32 totalEntries;
    s32 *tabEntry;
    s32 size;
    s32 offset;

    modLines = NULL;

    fileSize = piRomGetFileSize(MODLINES_TAB);
    totalEntries = (fileSize - 4) >> 2;

    if (modLineNo > totalEntries) {
        return NULL;
    }

    tabEntry = (s32*)mmAlloc(16, ALLOC_TAG_TEST_COL, ALLOC_NAME("obj:tempindex"));
    piRomLoadSection(MODLINES_TAB, (void*)tabEntry, modLineNo << 2, 8);

    offset = tabEntry[0];
    size = tabEntry[1] - tabEntry[0];

    if (size > 0) {
        modLines = (ModLine*)mmAlloc(size, ALLOC_TAG_TRACK_COL, ALLOC_NAME("obj:templine"));
        piRomLoadSection(MODLINES_BIN, (void*)modLines, offset, size);
    }

    mmFree(tabEntry);

    *modLineCount = (size / sizeof(ModLine));

    return modLines;
}

void objDoNothing_80022DD8(s32 a0, s32 a1, s32 a2) { }

s32 objGetObjIndexCount(void) {
    return gObjIndexCount;
}

s32 objIsObjIndexDefined(s32 idx) {
    if (idx > gObjIndexCount) {
        return FALSE;
    }

    return gFile_OBJINDEX[idx] != -1;
}

// unused
s16 objGetMobileMapID(s32 id) {
    ObjDef *def;

    if (id > gObjIndexCount) {
        return 0;
    }

    id = gFile_OBJINDEX[id];

    if (id == -1) {
        return 0;
    }

    def = (ObjDef*)(gFile_OBJECTS_TAB[id] + D_800B18E8); // ???

    if (def->flags & OBJDEF_IS_MOBILE_MAP) {
        return def->mobileMapID;
    }

    return -1;
}

// official name: objGetControlNo
s16 objGetControlNo(s32 id) {
    ObjDef def;
    s32 objtype;
    u32 var_v0;
    u32 var_v1;

    if (gObjIndexCount < id) {
        return 0;
    }    

    objtype = id;
    id = gFile_OBJINDEX[id];
    if (id == -1) {
        STUBBED_PRINTF("objGetControlNo objtype out of range %d/%d\n", objtype, gObjIndexCount);
        return 0;
    }

    var_v0 = (u32)&def.controlNo - (u32)&def;
    var_v1 = 0;
    while (var_v0 & 1) {
        var_v0 -= 1;
        var_v1 += 1;        
    }

    assetRomLoadSection((void*)D_800B18E4, OBJECTS_BIN, (s32)(gFile_OBJECTS_TAB[id] + var_v0), 8);
    return D_800B18E4[var_v1];
}

void objFreeObjectInternal(Object *obj, s32 onlySelf) {
    Object *obj2;
    /*sp+0xE4*/ LightAction lAction;
    AnimObj_Data *animObjdata;
    ModelInstance *modelInst;
    /*sp+0x40*/ Object *stackObjs[39]; // unknown exact length
    /*sp+0x3c*/ s32 k;
    /*sp+0x38*/ s32 numModels;
    /*sp+0x34*/ s32 i;
    /*sp+0x30*/ s32 numStackObjs;

    if (obj->dll != NULL) {
        update_pi_manager_array(4, obj->id);
        obj->dll->vtbl->Free(obj, onlySelf);
        update_pi_manager_array(4, -1);
        dllFree(obj->dll);
    }

    dll_amSfx->FreeObject(obj);
    gDLL_5_AMSEQ->vtbl->func17(obj);
    gDLL_13_Expgfx->vtbl->func9(obj);

    if (obj->def != NULL && (obj->def->flags & OBJDEF_FLAG10)) {
        objFreeObjectType(obj, OBJTYPE_56);
    }

    if (obj->def->flags & OBJDEF_IS_MOBILE_MAP) {
        objFreeObjectType(obj, OBJTYPE_MobileMap);

        if (!onlySelf) {
            numStackObjs = 0;

            for (i = 0; i < gNumObjs; i++) {
                obj2 = gObjList[i];

                if (obj == obj2->parent) {
                    obj2->parent = NULL;

                    if (obj2->setup != NULL) {
                        stackObjs[numStackObjs] = obj2;
                        numStackObjs++;
                        /* default.dol
                        if (39 < numStackObjs) {
                            // "world free obj list overflow\n"
                        }
                        */
                    }
                }
            }

            for (i = 0; i < numStackObjs; i++) {
                objFreeObject(stackObjs[i]);
            }

            mapFree(obj->mobileMapID);
        }
    }

    if (!onlySelf && obj->controlNo == OBJCONTROL_AnimObj) {
        for (i = 0; i < gNumObjs; i++) {
            obj2 = gObjList[i];
            if (obj == obj2->animObj) {
                obj2->animObj = NULL;
            }
        }
    }

    for (k = 0; k < gNumObjs; k++) {
        obj2 = gObjList[k];
        if (obj2->controlNo == OBJCONTROL_AnimObj) {
            animObjdata = (AnimObj_Data*)obj2->data;
            if (obj == animObjdata->actor) {
                animObjdata->actor = NULL;
                animObjdata->unk9C = 1;
            }
        }
    }
    

    if (obj->def->unk5e >= 1) {
        objFreeObjectType(obj, OBJTYPE_LookAt);
    }

    if (obj->def->unk87 & 0x10) {
        lAction.unk12.asByte = 2;
        lAction.unke = 0;
        lAction.unk10 = obj->unkD6;
        lAction.unk1b = 0;

        gDLL_11_Newlfx->vtbl->func0(obj, obj, &lAction, 0, 0, 0);
    }

    if (obj->shadow != NULL) {
        if (obj->def->shadowType == OBJ_SHADOW_BOX) {
            shadows_func_8004D974(1);
        }

        if (obj->shadow->texture != NULL) {
            texFreeTexture(obj->shadow->texture);
        }

        if (obj->shadow->unk8 != NULL) {
            texFreeTexture(obj->shadow->unk8);
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
            modFreeModel(modelInst);
        }
    }

    objFreeObjdef(obj->tabIdx);

    if (obj->seqSlot >= 0) {
        if (!onlySelf) {
            gDLL_3_Animation->vtbl->end_obj_sequence(obj->seqSlot);
            obj->seqSlot = SEQSLOT_NONE;
        }
    }

    if (obj->srt.flags & OBJFLAG_OWNS_SETUP && obj->setup != NULL) {
        mmFree(obj->setup);
    }

    mmFree(obj);
}

void *objAllocSetup(s32 size, s32 objId) {
    ObjSetup *setup;

    setup = (ObjSetup*)mmAlloc(size, ALLOC_TAG_OBJECTS_COL, ALLOC_NAME("romdef"));
    bzero(setup, size);

    setup->uID = -1;
    setup->loadDistance = 100;
    setup->fadeDistance = 50;
    setup->loadFlags = OBJSETUP_LOAD_CAMERA;
    setup->fadeFlags = OBJSETUP_FADE_CAMERA;
    setup->objId = objId;

    return (void*)setup;
}

// Change player, preserving their current position. Probably for debugging.
void objReplacePlayer(s32 playerno) {
    Object *player;
    s32 activePlayerno;
    ObjSetup playerSetup;
    f32 x, y, z;
    Object *newPlayer;

    player = objGetPlayer();
    activePlayerno = gDLL_29_Gplay->vtbl->get_playerno();

    if (playerno != activePlayerno) {
        gDLL_29_Gplay->vtbl->set_playerno(playerno);

        if (player != NULL) {
            objFreeObject(player);
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
            playerSetup.actExclusions1 = 0;
            playerSetup.loadFlags = OBJSETUP_LOAD_LEVEL;
            playerSetup.fadeFlags = OBJSETUP_FADE_CAMERA;
            playerSetup.loadDistance = 255;
            playerSetup.fadeDistance = 100;
            playerSetup.objId = sObjPlayerObjIDs[playerno];
            playerSetup.quarterSize = 24;
            playerSetup.x = x;
            playerSetup.y = y;
            playerSetup.z = z;

            newPlayer = objSetupObject(&playerSetup, OBJINIT_STANDALONE, -1, -1, NULL);
        }

        gDLL_2_Camera->vtbl->init_data(newPlayer, x - 50.0f, y, z - 50.0f);
        dll_amSfx->Func480(newPlayer);
        gDLL_5_AMSEQ->vtbl->set_focus_obj(newPlayer);
    }
}

void objLoadPlayer(void) {
    Object *player;
    s32 mapType;
    ObjSetup playerSetup;
    PlayerLocation *savedPlayerLocation;
    f32 x, y, z;
    s32 playerno;

    mapType = mapGetType();
    if (mapType == MAPTYPE_2 || mapType == MAPTYPE_3) {
        objFreeAll();
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
        playerSetup.actExclusions1 = 0;
        playerSetup.loadFlags = OBJSETUP_LOAD_LEVEL;
        playerSetup.fadeFlags = OBJSETUP_FADE_CAMERA;
        playerSetup.loadDistance = 255;
        playerSetup.fadeDistance = 100;
        playerSetup.objId = sObjPlayerObjIDs[playerno];
        playerSetup.quarterSize = 24;
        playerSetup.x = x;
        playerSetup.y = y;
        playerSetup.z = z;

        player = objSetupObject(&playerSetup, OBJINIT_STANDALONE, -1, -1, NULL);
    }

    D_80091668.unk8 = mathSinfInterp(savedPlayerLocation->rotationY << 8) * 60.0f + x;
    D_80091668.unkC = y + 40.0f;
    D_80091668.unk10 = mathCosfInterp(savedPlayerLocation->rotationY << 8) * 60.0f + z;

    gDLL_2_Camera->vtbl->init_data(player, D_80091668.unk8, D_80091668.unkC, D_80091668.unk10);
    gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 0, sizeof(D_80091668), &D_80091668, 0, Cam_Ease_All);
    gDLL_2_Camera->vtbl->tick(1);
    dll_amSfx->Func480(player);
    gDLL_5_AMSEQ->vtbl->set_focus_obj(player);

    D_800B1988 = 0;
    map_func_8004A67C();
}

void objLoadSidekick(Object* object, s32 objectId) {
    SidekickSetup* sidekickSetup;

    sidekickSetup = (SidekickSetup*)object->setup;
    D_80091688.base.objId = objectId;
    D_80091688.base.x = object->globalPosition.x;
    D_80091688.base.y = object->globalPosition.y;
    D_80091688.base.z = object->globalPosition.z;
    D_80091688.unk18 = sidekickSetup->unk18;
    D_80091688.unk19 = sidekickSetup->unk19;

    objSetupObject((ObjSetup*)&D_80091688, OBJINIT_STANDALONE, -1, -1, NULL);
}

Object *objGetPlayer(void) {
    Object **objectList;
    s32 count;

    objectList = objGetAllOfType(OBJTYPE_Player, &count);

    if (count > 1) {
        STUBBED_PRINTF(" ERROR : Error in Get Main More Players Loaded ");
    }

    if (count) {
        return objectList[0];
    } else {
        return NULL;
    }
}

Object *objGetSidekick(void) {
    Object **objectList;
    s32 count;

    objectList = objGetAllOfType(OBJTYPE_Sidekick, &count);

    if (count > 1) {
        STUBBED_PRINTF(" ERROR : ERROR : Error in Get Sidekicks Loaded  ");
    }

    if (count) {
        return objectList[0];
    } else {
        return NULL;
    }
}

void objClearMapID(Object *obj) {
    obj->mapID = -1;
}

void objInferMapID(Object *obj) {
    obj->mapID = mapWorldXZToMapID(obj->srt.transl.x, obj->srt.transl.z);
}

s32 objMove(Object *obj, f32 dx, f32 dy, f32 dz) {
    obj->srt.transl.x += dx;
    obj->srt.transl.y += dy;
    obj->srt.transl.z += dz;

    return 0;
}

void objSetPriority(Object *obj, s8 priority) {
    /* default.dol
    if (priority == 90 && !(obj->def->flags & 0x40)) {
        // "WARNING Cannot set priority Level to WORLD if not world object  \n"
    }
    */
    obj->updatePriority = priority;
}

void objSetModel(Object *obj, s32 modelIdx) {
    obj->stateFlags &= ~(OBJSTATE_NEXT_MODEL_INDEX_MASK | OBJSTATE_PENDING_MODEL_SWITCH);

    if (modelIdx == obj->modelInstIdx) {
        return;
    }

    if (modelIdx < 0) {
        modelIdx = 0;
    } else if (modelIdx >= obj->def->numModels) {
        modelIdx = obj->def->numModels - 1;
    }

    obj->stateFlags |= OBJSTATE_PENDING_MODEL_SWITCH;
    obj->stateFlags |= (modelIdx << OBJSTATE_NEXT_MODEL_INDEX_OFFSET) & OBJSTATE_NEXT_MODEL_INDEX_MASK;
}

void objHandleModelSwitch(Object *obj, ModelInstance *modelInst, Model *model) {
    s32 modelInstIdx;
    ModelInstance *modelInst2;
    s32 prevAnimId;

    modelInstIdx = (obj->stateFlags & OBJSTATE_NEXT_MODEL_INDEX_MASK) >> OBJSTATE_NEXT_MODEL_INDEX_OFFSET;
    modelInst2 = obj->modelInsts[modelInstIdx];

    modelInst2->unk34 &= ~0xF;
    modelInst2->unk34 |= modelInst->unk34 & 7;

    prevAnimId = obj->curModAnimId;

    obj->modelInstIdx = modelInstIdx;
    obj->curModAnimId = -1;
    obj->curModAnimIdLayered = -1;

    objAnimSet(obj, prevAnimId, obj->animProgress, 0);

    if (obj->objhitInfo != NULL) {
        if ((obj->objhitInfo->unk5A & 0x10)) {
            obj->objhitInfo->unk9F = 2;
        }

        obj->objhitInfo->unk9E = 1;
    }

    obj->stateFlags &= ~(OBJSTATE_NEXT_MODEL_INDEX_MASK | OBJSTATE_PENDING_MODEL_SWITCH);
}

// official name: objAddEffectBox
void objAddEffectBox(Object *obj) {
    gEffectBoxes[gEffectBoxCount] = obj;
    gEffectBoxCount += 1;

    if (gEffectBoxCount > ARRAYCOUNT_S(gEffectBoxes)) {
        STUBBED_PRINTF("warning: objAddEffectBox max effect boxes\n");
    }
}

static const char str_800997ac[] = "objFreeEffectBox: Not found\n";
// official name: objFreeEffectBox
void objFreeEffectBox(Object *obj) {
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
void obj_func_80023BF8(Object *obj, s32 param2, s32 param3, s32 param4, u8 param5, u8 colourIndex) {
    ObjectStruct78 *dst;

    if (obj != NULL) {
        dst = obj->unk78;

        if (dst != NULL) {
            dst += obj->unkD4;

            if (param2 != 0) {
                dst->interactRadius = param2 >> 2;
            }

            if (param4 != 0) {
                dst->lockExitRadius = param4 >> 2;
            }

            if (param3 != 0) {
                dst->hlRadius = param3 >> 2;
            }

            if (param5 != 0) {
                dst->hlAngularRange = param5;
            }

            if (colourIndex != 0) {
                dst->flags = colourIndex ;
            }
        }
    }
}

void obj_func_80023C6C(Object *obj) {
    ObjDefLockData *src;
    ObjectStruct78 *dst;

    if (obj != NULL) {
        dst = obj->unk78;

        if (dst != NULL) {
            src = &obj->def->lockdata[obj->unkD4];
            dst += obj->unkD4;

            dst->interactRadius = src->interactRadius;
            dst->lockExitRadius = src->lockExitRadius;
            dst->hlRadius = src->hlRadius;
            dst->hlAngularRange = src->hlAngularRange;
            dst->flags = src->flags;
        }
    }
}

void objSetLockNum(Object *obj, u16 param2) {
    if (param2 > obj->def->numLockdata) {
        STUBBED_PRINTF("locknum out of range\n");
        param2 = 0;
    }

    obj->unkD4 = param2;
}

void objSetInfoNum(Object *obj, u16 param2) {
    if (param2 > 4) {
        STUBBED_PRINTF("infonum out of range\n");
        param2 = 0;
    }

    obj->unkD8 = param2;
}
