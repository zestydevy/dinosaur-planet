#include "common.h"

#include "common.h"
#include "variables.h"
#include "sys/gfx/model.h"
#include "sys/linked_list.h"

extern void **gLoadedObjDefs;
extern void *D_800B1918;
extern void *D_800B18E4;
extern int gObjIndexCount; //count of OBJINDEX.BIN entries
extern int gNumObjectsTabEntries;
extern ObjData **gLoadedObjData;
extern u8 *gObjRefCount; //pObjectRefCount
extern int gNumTablesTabEntries;
extern TActor **gObjList; //global object list
extern LinkedList gObjUpdateList;

enum FILE_ID {
    FILE_TABLES_BIN   = 0x16,
    FILE_TABLES_TAB   = 0x17,
    FILE_BITTABLE     = 0x37,
    FILE_OBJECTS_TAB  = 0x41,
    FILE_OBJINDEX_BIN = 0x43
};

extern void *gFile_TABLES_BIN;
extern s32  *gFile_TABLES_TAB;
extern s32  *gFile_OBJECTS_TAB;
extern s16  *gFile_OBJINDEX;

int get_file_size(int file);
void queue_alloc_load_file(void **dest, s32 fileId);
void queue_load_file_to_ptr(void **dest, s32 fileId);
void alloc_some_object_arrays(void); //related to objects
void func_80020D34(void);

void copy_obj_position_mirrors(TActor *obj, ObjCreateInfo *param2, s32 param3);

void func_80046320(s16 param1, TActor *actor);
void func_80023A00(TActor *actor, s8 param2); // sets actor->updatePriority = param2
void func_800222AC(TActor *actor);

extern char D_80099600[]; // "objects/objects.c"
extern s32 gNumObjs;

extern void func_80058FE8();
extern void update_obj_hitboxes(s32);
extern s32 func_80004258(TActor *actor);
extern void func_80025E58();
extern void obj_do_hit_detection(s32);
extern void func_8002B6EC();

extern char D_800994E0[]; // "objects/objects.c"

extern s32 D_800B1914;

void update_obj_models();
void update_object(TActor *obj);
void func_8002272C(TActor *obj);

extern s32 D_80091660;
extern s32 D_800B191C;

void func_80022338(TActor *);

extern s16 D_800B18E0;
extern s32 D_800B1988;

extern void func_80025DF0();

void func_80021A84(TActor *actor, u32 someFlags);

void func_800228D0(TActor *obj, s32 param2, Actor60 *outParam, s32 id, u8 dontQueueLoad);

ModLine *func_80022D00(s32 modLineNo, s16 *modLineCount);

extern void func_800596BC(ObjData*);

ObjData *func_80022AA4(s32 tabIdx);
u32 func_80022828(TActor *obj);
u32 func_80021CC0(TActor *obj, ObjData *def, u32 flags);
void func_80022C68(s32 tabIdx);

void func_80021E74(f32, ModelInstance*);
void func_80022200(TActor *obj, s32 param2, s32 param3);
u32 func_800227AC(TActor *obj, u32 addr);
u32 func_80022868(s32 param1, TActor *obj, u32 addr);
u32 func_8002298C(s32 param1, ModelInstance *param2, TActor *obj, u32 addr);

f32 func_80022150(TActor *obj);

void init_objects(void) {
    int i;

    //allocate some buffers
    gLoadedObjDefs = malloc(0x2D0, ALLOC_TAG_OBJECTS_COL, NULL);
    D_800B1918     = malloc(0x60, ALLOC_TAG_OBJECTS_COL,  NULL);
    D_800B18E4     = malloc(0x10, ALLOC_TAG_OBJECTS_COL,  NULL);

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
    gLoadedObjData = malloc(gNumObjectsTabEntries * 4, ALLOC_TAG_OBJECTS_COL, NULL);
    gObjRefCount   = malloc(gNumObjectsTabEntries,     ALLOC_TAG_OBJECTS_COL, NULL);
    for(i = 0; i < gNumObjectsTabEntries; i++) gObjRefCount[i] = 0; //why not memset?

    //load TABLES.BIN and TABLES.TAB and count number of entries
    queue_alloc_load_file((void **) (&gFile_TABLES_BIN), FILE_TABLES_BIN);
    queue_alloc_load_file((void **) (&gFile_TABLES_TAB), FILE_TABLES_TAB);
    gNumTablesTabEntries = 0;
    while(gFile_TABLES_TAB[gNumTablesTabEntries] != -1) gNumTablesTabEntries++;

    //allocate global object list and some other buffers
    gObjList = malloc(0x2D0, ALLOC_TAG_OBJECTS_COL, NULL);
    alloc_some_object_arrays();
    func_80020D34();
}

void update_objects() {
    void *node;
    TActor *obj;
    TActor *player;
    s32 nextFieldOffset;

    nextFieldOffset = gObjUpdateList.nextFieldOffset; // == 0x38, &obj->next

    func_80058FE8();

    update_obj_models();
    update_obj_hitboxes(gNumObjs);

    node = gObjUpdateList.head;

    for (obj = (TActor*)node; node != NULL && obj->updatePriority == 100; obj = (TActor*)node) {
        update_object(obj);
        node = *((void**)(nextFieldOffset + (u32)node));

        if (obj->objhitInfo->unk_0x58){} // fake match
    }

    for (obj = (TActor*)node; node != NULL && obj->data->flags & 0x40; obj = (TActor*)node) {
        update_object(obj);
        obj->matrixIdx = func_80004258(obj);
        node = *((void**)(nextFieldOffset + (u32)node));
    }

    func_80025E58();

    while (node != NULL) {
        obj = (TActor*)node;

        if (obj->objhitInfo != NULL) {
            if (obj->objhitInfo->unk_0x5a != 8 || (obj->objhitInfo->unk_0x58 & 1) == 0) {
                update_object(obj);
            }
        } else {
            update_object(obj);
        }

        node = *((void**)(nextFieldOffset + (u32)node));
    }

    player = get_player();
    if (player != NULL && player->linkedActor2 != NULL) {
        player->linkedActor2->linkedActor = player->linkedActor;
        update_object(player->linkedActor2);
    }

    obj_do_hit_detection(gNumObjs);

    node = gObjUpdateList.head;
    while (node != NULL) {
        obj = (TActor*)node;
        func_8002272C(obj);
        node = *((void**)(nextFieldOffset + (u32)node));
    }

    player = get_player();
    if (player != NULL && player->linkedActor2 != NULL) {
        player->linkedActor2->linkedActor = player->linkedActor;
        func_8002272C(player->linkedActor2);
    }

    gDLL_waterfx->exports->func[0].withOneArg(delayByte);
    gDLL_projgfx->exports->func[2].withTwoArgs(delayByte, 0);
    gDLL_modgfx->exports->func[2].withThreeArgs(0, 0, 0);
    gDLL_expgfx->exports->func[2].withFourArgs(0, delayByte, 0, 0);

    func_8002B6EC();

    gDLL_ANIM->exports->func[9].asVoid();
    gDLL_ANIM->exports->func[5].asVoid();
    gDLL_Camera->exports->func[1].withOneArg(delayByte);

    write_c_file_label_pointers(D_800994E0, 0x169);
}

void doNothing_80020A40(void) {}

void update_obj_models() {
    int i;
    int j;
    int k;
    TActor *actor;
    ModelInstance *modelInst;
    ActorUnk0xc0_0xb8 *unk1;

    for (i = 0; i < gNumObjs; i++) {
        actor = gObjList[i];

        for (j = 0; j < 2; j++) {
            if (j != 0) {
                actor = actor->linkedActor2;
            }

            if (actor == NULL) {
                continue;
            }

            for (k = 0; k < actor->data->modLinesIdx; k++) {
                modelInst = actor->modelInsts[k];

                if (modelInst != NULL) {
                    modelInst->unk_0x34 &= ~0x8;

                    if (modelInst->model->unk_0x1c != NULL) {
                        unk1 = actor->unk0xc0 != NULL ? actor->unk0xc0->unk_0xb8 : NULL;

                        if (actor->unk0xc0 == NULL || (unk1 != NULL && unk1->unk_0x62 == 0)) {
                            func_8001B084(modelInst, delayFloat);
                        }
                    }
                }
            }
        }
    }
}

void func_80020BB8() {
    int i;
    for(i = 0; i < D_800B1914; i++) {
        func_80022F94(gLoadedObjDefs[i], 0); //possibly some type of free?
        gLoadedObjDefs[i] = 0;
    }
    D_800B1914 = 0;
}

void func_80020C48() {
    s32 i;
    
    func_80020BB8();
    D_80091660 = 0;

    i = 0;
    while (gNumObjs != 0) {
        func_80022338(gObjList[i]);
        i++;
        if (i >= gNumObjs) {
            i = 0;
        }
    }

    func_80020BB8();
    D_80091660 = 2;

    D_800B1914 = 0;
    D_800B191C = 0;
    gNumObjs = 0;

    linked_list_init(&gObjUpdateList, OFFSETOF(TActor, next));

    func_80020D34();

    gDLL_Camera->exports->func[9].withTwoArgs(0, 0);
}

void func_80020D34() {
    D_800B1914 = 0;
    D_800B191C = 0;
    D_800B1988 = 0;
    gNumObjs = 0;

    linked_list_init(&gObjUpdateList, OFFSETOF(TActor, next));

    D_800B18E0 = 0;
    func_80030EC0();
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

    while (idx1 <= idx2) {
        endLoop = 0;
        while (idx1 <= objsEnd && !endLoop) {
            if ((gObjList[idx1]->data->flags & 1) != 0) {
                idx1++;
            } else {
                endLoop = -1;
            }
        }

        endLoop = 0;
        while (idx2 >= 0 && !endLoop) {
            if ((gObjList[idx2]->data->flags & 1) == 0) {
                idx2--;
            } else {
                endLoop = -1;
            }
        }

        if (idx1 < idx2) {
            TActor *temp;

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

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80020EE4.s")

void func_800210DC() {
    s32 i;
    TActor *obj;
    ActorUnk0xc0 *var;

    for (i = 0; i < gNumObjs; i++) {
        obj = gObjList[i];

        if (obj->unk0xc0 != NULL) {
            var = obj->unk0xc0;

            if (obj->linkedActor == NULL) {
                if (var->unk_0x30 != NULL) {
                    obj->linkedActor = var->unk_0x30;
                }
            }
            
            obj->unk0xc0 = NULL;
        }
    }
}

TActor **get_world_actors(s32 *param1, s32 *numObjs) {
    if (param1 != NULL) {
        *param1 = 0;
    }
    if (numObjs != NULL) {
        *numObjs = gNumObjs;
    }

    return gObjList;
}

TActor *func_80021178(s32 idx) {
    if (idx < 0 || idx >= gNumObjs) {
        return NULL;
    }

    return gObjList[idx];
}

TActor *func_800211B4(s32 param1) {
    s32 i;
    s32 len;
    TActor *obj;

    i = 0;
    len = gNumObjs;

    while (i < len) {
        obj = gObjList[i];
        if (obj->createInfo != NULL && obj->createInfo->unk14 == param1) {
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

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800212F4.s")

void *func_800213A0(s32 idx) {
    if (idx < 0 || idx >= gNumTablesTabEntries) {
        return gFile_TABLES_BIN;
    }

    return (void*)((u32)gFile_TABLES_BIN + gFile_TABLES_TAB[idx] * 4);
}

TActor *func_800213EC(s32 param1, s32 param2, s32 param3, s32 param4, s32 param5) {
    TActor *obj;

    obj = NULL;
    queue_load_map_object((void**)&obj, param1, param2, param3, param4, param5, 0);
    func_80021A84(obj, param2);

    return obj;
}

TActor *objSetupObject(ObjCreateInfo *createInfo, u32 param2, s32 mapID, s32 param4, TActor *param5) {
    ObjData *def;
    s32 modelCount;
    s32 var;
    s32 flags;
    ModelInstance *tempModel;
    TActor *obj;
    s32 tabIdx;
    s32 objId;
    s32 j;
    TActor objHeader;
    s32 addr;
    s8 modelLoadFailed;

    objId = createInfo->unk0;

    update_pi_manager_array(0, objId);

    if (param2 & 2) {
        tabIdx = objId;
    } else {
        if (objId > gObjIndexCount) {
            update_pi_manager_array(0, -1);
            return NULL;
        }

        tabIdx = gFile_OBJINDEX[objId];
    }

    bzero(&objHeader, sizeof(TActor));

    objHeader.data = func_80022AA4(tabIdx);
    def = objHeader.data;

    if (def == NULL || (u32)def == 0xFFFFFFFF) {
        return NULL;
    } 
    
    objHeader.srt.flags = 2;

    if (def->flags & 0x80) {
        objHeader.srt.flags = 0x82;
    }

    if (def->flags & 0x40000) {
        objHeader.unk0xb0 |= 0x80;
    }

    if (param2 & 4) {
        objHeader.srt.flags |= 0x2000;
    }

    objHeader.srt.transl.x = createInfo->x;
    objHeader.srt.transl.y = createInfo->y;
    objHeader.srt.transl.z = createInfo->z;
    objHeader.createInfo = createInfo;
    objHeader.unk0x48 = tabIdx;
    objHeader.unk0x46 = objId;
    objHeader.unk0xb2 = param4;
    objHeader.mapID = mapID;
    objHeader.unk_0xa2 = -1;
    objHeader.unk0xb4 = -1;
    objHeader.srt.scale = def->scale;
    objHeader.unk_0x36 = 0xFF;
    objHeader.ptr0xcc = NULL;
    objHeader.unk0x3c = createInfo->unk6 << 3;
    objHeader.unk0x40 = createInfo->unk7 << 3;
    objHeader.dll = NULL;

    if (def->dllID != 0) {
        objHeader.dll = (DLLInst_Object*)dll_load(def->dllID, 6, 1);
    }

    flags = func_80022828(&objHeader);

    if (def->flags & 0x20) {
        flags &= ~1;
    } else {
        flags |= 1;
    }

    if (def->shadowType != 0) {
        flags |= 2;
    } else {
        flags &= ~2;
    }

    if (def->flags & 1) {
        flags |= 0x200;
    }

    var = func_80021CC0(&objHeader, def, flags);

    obj = (TActor*)malloc(var, ALLOC_TAG_OBJECTS_COL, NULL);

    if (obj == NULL) {
        func_80022C68(tabIdx);
        return NULL;
    }

    bcopy(&objHeader, obj, sizeof(TActor));
    bzero((void*)((u32)obj + sizeof(TActor)), var - sizeof(TActor));

    modelCount = def->modLinesIdx;

    obj->modelInsts = (ModelInstance**)((u32)obj + sizeof(TActor));

    modelLoadFailed = FALSE;
    var = 0;
    
    if (!(flags & 0x200)) {
        if (flags & 0x400) {
            var = (flags >> 11) & 0xf;

            if (var < modelCount) {
                obj->modelInsts[var] = model_load_create_instance(-def->pModelList[var], flags);

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
                obj->modelInsts[var] = model_load_create_instance(-def->pModelList[var], flags);
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
        func_80022C68(tabIdx);
        return NULL;
    }
     
    addr = func_800227AC(obj, (u32)&obj->modelInsts[def->modLinesIdx]);

    if (flags & 0x40) {
        addr = func_80022868(obj->unk0x46, obj, addr);
    }

    if (flags & 0x100) {
        addr = func_8002298C(obj->unk0x46, obj->modelInsts[0], obj, addr);
    }

    if ((flags & 0x2) && (def->shadowType != 0)) {
        addr = func_8004D8A4(obj, addr, 0);
    }

    obj->unk_0xa8 = func_80022150(obj) * obj->srt.scale;

    if (def->unk8F != 0) {
        addr = func_8002667C(obj, addr);

        if (def->unk93 & 0x8) {
            addr = func_80026BD8(obj, addr);
        }
    }

    if (def->numAButtonInteractions != 0) {
        obj->ptr0x6c = (u16*)align_4(addr);
        addr = (u32)obj->ptr0x6c + (def->numAButtonInteractions * 0x12);
    }

    if (def->flags_0x71 != 0) {
        obj->ptr0x70 = (void*)align_4(addr);
        addr = (u32)obj->ptr0x70 + (def->flags_0x71 * 0x10);
    }

    if (def->unk9b != 0) {
        obj->unk0x74 = align_4(addr);
        addr = (u32)obj->unk0x74 + (def->unk9b * 0x18);
    }

    if (def->unk8F != 0 && def->unk74 != 0) {
        addr = align_4(addr);
        addr = func_80026A20(obj->unk0x46, obj->modelInsts[0], obj->objhitInfo, addr, obj);
    }

    if (def->unk9b != 0) {
        obj->unk_0x78 = (Actor78*)align_4(addr);

        for (j = 0; j < def->unk9b; j++) {
            obj->unk_0x78[j].unk4 = def->aButtonInteraction[j].unk10;
            obj->unk_0x78[j].unk0 = def->aButtonInteraction[j].unk0c;
            obj->unk_0x78[j].unk3 = def->aButtonInteraction[j].unk0f;
            obj->unk_0x78[j].unk1 = def->aButtonInteraction[j].unk0d;
            obj->unk_0x78[j].unk2 = def->aButtonInteraction[j].unk0e;
        }
    }

    obj->linkedActor = param5;
    
    return obj;
}

// add_actor
void func_80021A84(TActor *actor, u32 someFlags) {
    if (actor->linkedActor != NULL) {
        transform_point_by_actor(
            actor->srt.transl.x, actor->srt.transl.y, actor->srt.transl.z,
            &actor->positionMirror.x, &actor->positionMirror.y, &actor->positionMirror.z,
            actor->linkedActor
        );
    } else {
        actor->positionMirror.x = actor->srt.transl.x;
        actor->positionMirror.y = actor->srt.transl.y;
        actor->positionMirror.z = actor->srt.transl.z;
    }

    actor->positionMirror2.x = actor->srt.transl.x;
    actor->positionMirror2.y = actor->srt.transl.y;
    actor->positionMirror2.z = actor->srt.transl.z;

    actor->positionMirror3.x = actor->positionMirror.x;
    actor->positionMirror3.y = actor->positionMirror.y;
    actor->positionMirror3.z = actor->positionMirror.z;

    copy_obj_position_mirrors(actor, actor->createInfo, 0);

    if (actor->objhitInfo != NULL) {
        actor->objhitInfo->unk_0x10.x = actor->srt.transl.x;
        actor->objhitInfo->unk_0x10.y = actor->srt.transl.y;
        actor->objhitInfo->unk_0x10.z = actor->srt.transl.z;

        actor->objhitInfo->unk_0x20.x = actor->srt.transl.x;
        actor->objhitInfo->unk_0x20.y = actor->srt.transl.y;
        actor->objhitInfo->unk_0x20.z = actor->srt.transl.z;
    }

    if (actor->data->unka0 > -1) {
        func_80046320(actor->data->unka0, actor);
    }

    update_pi_manager_array(0, -1);

    if (actor->data->flags & OBJDATA_FLAG44_HasChildren) {
        add_object_to_array(actor, 7);

        if (actor->updatePriority != 90) {
            func_80023A00(actor, 90);
        }
    } else {
        if (actor->updatePriority == 0) {
            func_80023A00(actor, 80);
        }
    }

    if (someFlags & 1) {
        actor->unk0xb0 |= 0x10;
        gObjList[gNumObjs] = actor;
        gNumObjs += 1;

        func_800222AC(actor);
    }

    if (actor->data->numSeqs >= 1) {
        add_object_to_array(actor, 9);
    }

    if (actor->data->flags & OBJDATA_FLAG44_HaveModels) {
        func_80020D90();
    }

    if (actor->data->flags & OBJDATA_FLAG44_DifferentLightColor) {
        add_object_to_array(actor, 56);
    }

    write_c_file_label_pointers(D_80099600, 0x477);
}

u32 func_80021CC0(TActor *obj, ObjData *def, u32 flags) {
    u32 size;

    size = sizeof(TActor);

    size += def->modLinesIdx * 4;

    if (obj->dll != NULL) {
        size += obj->dll->exports->get_state_size(obj, size);
    }

    if (flags & 0x40) {
        size = align_4(size);
        size = align_8(size + 8);
        size += 0x50;
    }

    if (flags & 0x100) {
        size = align_4(size);
        size = align_8(size + 8);
        size += 0x400;
    }

    if ((flags & 2) && (def->shadowType != 0)) {
        size = align_4(size);
        size += 0x44;
    }

    if (def->unk8F != 0) {
        size = align_4(size);
        size += 0xa4;

        if (def->unk93 & 8) {
            size += 0x110;
        }
    }

    if (def->numAButtonInteractions != 0) {
        size = align_4(size);
        size += def->numAButtonInteractions * 0x12;
    }

    if (def->flags_0x71 != 0) {
        size = align_4(size);
        size += def->flags_0x71 * 0x10;
    }

    if (def->unk9b != 0) {
        size = align_4(size);
        size += def->unk9b * 0x18;
    }

    if (def->unk8F != 0 && def->unk74 != 0) {
        size = align_8(size);
        size += 300;
    }

    if (def->unk9b != 0) {
        size = align_4(size);
        size += def->unk9b * 5;
    }

    return size;
}

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80021E74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022150.s")

void func_80022200(TActor *obj, s32 param2, s32 param3) {
    s32 i;

    for (i = 0; i < param2; i++) {
        if (obj->modelInsts[i] != NULL) {
            ModelInstance *modelInst = obj->modelInsts[i];
            destroy_model_instance(modelInst);
        }
    }
}

// objFreeTick?
void func_80022274(TActor *obj) {
    if (obj->unk0xb0 & 0x10) {
        linked_list_remove(&gObjUpdateList, obj);
    }
}

void func_800222AC(TActor *obj) {
    void *insertAfter;
    void *node;
    TActor *objNode;
    s32 nextFieldOffset;
    
    if (obj->unk0xb0 & 0x10) {
        nextFieldOffset = gObjUpdateList.nextFieldOffset;
        insertAfter = NULL;

        node = gObjUpdateList.head;

        // Insert maintaining sort by update priority
        for (objNode = (TActor*)node; node != NULL && obj->updatePriority < ((TActor*)node)->updatePriority; objNode = (TActor*)node) {
            insertAfter = objNode;
            node = *((void**)(nextFieldOffset + (u32)node));
        }

        linked_list_insert(&gObjUpdateList, insertAfter, (void*)obj);
    }
}

// objFreeObject?
#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022338.s")

void copy_obj_position_mirrors(TActor *obj, ObjCreateInfo *param2, s32 param3) {
    DLLInst_Object *dll;
    obj->objGroup = obj->data->objGroup;
    dll = obj->dll;
    if(1) {
        if(dll != NULL) {
            obj->dll->exports->func1(obj, param2, param3);
        }
    }

    obj->positionMirror2.x = obj->srt.transl.x;
    obj->positionMirror3.x = obj->srt.transl.x;
    obj->positionMirror2.y = obj->srt.transl.y;
    obj->positionMirror3.y = obj->srt.transl.y;
    obj->positionMirror2.z = obj->srt.transl.z;
    obj->positionMirror3.z = obj->srt.transl.z;
}

void update_object(TActor *obj) {
    if (obj->unk0xb0 & 0x40) {
        return;
    }

    if (obj->unk0xc0 != NULL) {
        if (obj->linkedActor2 != NULL) {
            obj->linkedActor2->objhitInfo->unk_0x48 = 0;
            obj->linkedActor2->objhitInfo->unk_0x62 = 0;
        }

        if (obj->objhitInfo != NULL) {
            obj->objhitInfo->unk_0x48 = 0;
            obj->objhitInfo->unk_0x62 = 0;
        }
    } else {
        update_pi_manager_array(1, obj->unk0x46);

        if (!(obj->srt.flags & 8)) {
            obj->positionMirror2.x = obj->srt.transl.x;
            obj->positionMirror2.y = obj->srt.transl.y;
            obj->positionMirror2.z = obj->srt.transl.z;

            obj->positionMirror3.x = obj->positionMirror.x;
            obj->positionMirror3.y = obj->positionMirror.y;
            obj->positionMirror3.z = obj->positionMirror.z;
        }

        if (obj->dll != NULL && !(obj->unk0xb0 & 0x8000)) {
            obj->dll->exports->update(obj);

            get_actor_child_position(obj,
                &obj->positionMirror.x, &obj->positionMirror.y, &obj->positionMirror.z);
        }

        if (obj->objhitInfo != NULL) {
            if (obj->linkedActor2 != NULL) {
                obj->linkedActor2->objhitInfo->unk_0x48 = 0;
                obj->linkedActor2->objhitInfo->unk_0x62 = 0;
            }

            obj->objhitInfo->unk_0x4c = 0;
            obj->objhitInfo->unk_0x48 = 0;
            obj->objhitInfo->unk_0x62 = 0;
        }

        if (obj->unk0x58 != NULL) {
            obj->unk0x58->unk10f = 0;
        }

        obj->unk0xaf &= 0xFFF8;

        update_pi_manager_array(1, -1);
    }
}

void func_8002272C(TActor *obj) {
    update_pi_manager_array(3, obj->unk0x46);

    if (obj->dll != NULL && !(obj->unk0xb0 & 0x2000)) {
        obj->dll->exports->func3(obj);

        get_actor_child_position(obj,
            &obj->positionMirror.x, &obj->positionMirror.y, &obj->positionMirror.z);
    }

    update_pi_manager_array(3, -1);
}

u32 func_800227AC(TActor *obj, u32 addr) {
    u32 stateSize = 0;
    
    addr = align_4(addr);

    if (obj->dll != NULL) {
        stateSize = obj->dll->exports->get_state_size(obj, addr);
    }

    if (stateSize != 0) {
        obj->state = (void*)addr;
        addr += stateSize;
    } else {
        obj->state = NULL;
    }

    return addr;
}

u32 func_80022828(TActor *obj) {
    if (obj->dll != NULL) {
        return obj->dll->exports->func6(obj);
    } else {
        return 0;
    }
}

u32 func_80022868(s32 param1, TActor *obj, u32 addr) {
    obj->ptr0x60 = (Actor60*)align_4(addr);

    addr = align_8((u32)obj->ptr0x60 + sizeof(Actor60));
    obj->ptr0x60->unk4 = (UNK_PTR*)addr;

    addr += 80;

    func_800228D0(obj, param1, obj->ptr0x60, 0, 1);

    return addr;
}

void func_800228D0(TActor *obj, s32 param2, Actor60 *outParam, s32 id, u8 dontQueueLoad) {
    ObjDataEvent *eventList;
    ObjDataEvent *event;
    
    eventList = obj->data->pEvent;

    outParam->unk0 = 0;
    
    if (eventList == NULL) {
        return;
    }

    for (event = eventList; event->id != -1; event++) {
        if (event->id == id) {
            s32 offset;

            offset = event->fileOffsetInBytes;

            outParam->unk0 = event->fileSizeInBytes;

            if (outParam->unk0 > 80) {
                outParam->unk0 = 80;
            }

            if (eventList) {}

            if (!dontQueueLoad) {
                queue_load_file_region_to_ptr((void**)outParam->unk4, OBJEVENT_BIN, offset, outParam->unk0);
            } else {
                read_file_region(OBJEVENT_BIN, outParam->unk4, offset, outParam->unk0);
            }

            break;
        }
    }
}

u32 func_8002298C(s32 param1, ModelInstance *param2, TActor *obj, u32 addr) {
    if (param2 == 0) {
        return addr;
    } else {
        obj->ptr0x5c = (Actor5c*)align_4(addr);

        addr = align_8((u32)obj->ptr0x5c + sizeof(Actor5c));
        obj->ptr0x5c->unk4 = (UNK_PTR*)addr;

        return addr + 1024;
    }
}

void func_800229E8(TActor *obj, s32 param2, WeaponDataPtr *outParam, s32 id, u8 queueLoad) {
    ObjDataWeaponData *weaponDataList;
    ObjDataWeaponData *weaponData;
    
    weaponDataList = obj->data->pWeaponDa;

    outParam->sizeInBytes = 0;
    
    if (weaponDataList == NULL) {
        return;
    }

    for (weaponData = weaponDataList; weaponData->id != -1; weaponData++) {
        if (weaponData->id == id) {
            s32 offset;

            offset = weaponData->fileOffsetInBytes;

            outParam->sizeInBytes = weaponData->fileSizeInBytes;

            if (outParam->sizeInBytes > 1024) {
                outParam->sizeInBytes = 1024;
            }

            if (weaponDataList) {}

            if (queueLoad) {
                queue_load_file_region_to_ptr((void**)outParam->ptr, WEAPONDATA_BIN, offset, outParam->sizeInBytes);
            } else {
                read_file_region(WEAPONDATA_BIN, outParam->ptr, offset, outParam->sizeInBytes);
            }

            break;
        }
    }
}

ObjData *func_80022AA4(s32 tabIdx) {
    ObjData *data;
    s32 fileOffset;
    s32 fileSize;

    if (tabIdx >= gNumObjectsTabEntries) {
        return NULL;
    }
    
    if (gObjRefCount[tabIdx] != 0) {
        gObjRefCount[tabIdx]++;
        data = gLoadedObjData[tabIdx];
        return data;
    }
    
    fileOffset = gFile_OBJECTS_TAB[tabIdx];
    fileSize = gFile_OBJECTS_TAB[tabIdx + 1] - fileOffset;

    data = (ObjData*)malloc(fileSize, ALLOC_TAG_OBJECTS_COL, NULL);
    if (data != NULL) {
        read_file_region(OBJECTS_BIN, (void*)data, fileOffset, fileSize);

        if (data->pEvent != 0) {
            data->pEvent = (ObjDataEvent*)((u32)data + (u32)data->pEvent);
        }

        if (data->pHits != 0) {
            data->pHits = (UNK_TYPE_8*)((u32)data + (u32)data->pHits);
        }

        if (data->pWeaponDa != 0) {
            data->pWeaponDa = (ObjDataWeaponData*)((u32)data + (u32)data->pWeaponDa);
        }

        data->pModelList = (u32*)((u32)data + (u32)data->pModelList);
        data->textures = (UNK_PTR*)((u32)data + (u32)data->textures);
        data->unk10 = (UNK_PTR*)((u32)data + (u32)data->unk10);

        if (data->offset_0x18 != 0) {
            data->offset_0x18 = (u32*)((u32)data + (u32)data->offset_0x18);
        }

        if (data->aButtonInteraction != 0) {
            data->aButtonInteraction = (AButtonInteraction*)((u32)data + (u32)data->aButtonInteraction);
        }

        if (data->pSeq != 0) {
            data->pSeq = (u16*)((u32)data + (u32)data->pSeq);
        }

        data->pModLines = NULL;

        data->pAttachPoints = (AttachPoint*)((u32)data + (u32)data->pAttachPoints);

        data->pIntersectPoints = NULL;

        if (data->modLineNo > -1) {
            data->pModLines = func_80022D00(data->modLineNo, &data->modLineCount);
            func_800596BC(data);
        }

        gLoadedObjData[tabIdx] = data;
        gObjRefCount[tabIdx] = 1;

    } else {
        return NULL;
    }

    return data;
}

// objFreeObjdef?
void func_80022C68(s32 tabIdx) {
    if (gObjRefCount[tabIdx] != 0) {
        gObjRefCount[tabIdx]--;

        if (gObjRefCount[tabIdx] == 0) {
            ObjData *data;

            data = gLoadedObjData[tabIdx];
            
            if (data->pModLines != NULL) {
                free(data->pModLines);
            }

            if (data->pIntersectPoints != NULL) {
                free(data->pIntersectPoints);
            }

            free(data);
        }
    }
}

ModLine *func_80022D00(s32 modLineNo, s16 *modLineCount) {
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

    tabEntry = (s32*)malloc(16, ALLOC_TAG_TEST_COL, NULL);
    read_file_region(MODLINES_TAB, (void*)tabEntry, modLineNo << 2, 8);

    offset = tabEntry[0];
    size = tabEntry[1] - tabEntry[0];

    if (size > 0) {
        modLines = (ModLine*)malloc(size, ALLOC_TAG_TRACK_COL, NULL);
        read_file_region(MODLINES_BIN, (void*)modLines, offset, size);
    }

    free(tabEntry);

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

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022E3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022EC0.s")

// free_object
#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80022F94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800233F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023464.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023628.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023894.s")

TActor *get_player(void) {
    TActor **obj;
    s32 idx;
    obj = TActor_getter(0, &idx);
    if(idx) {} else {}; //wat
    if(idx) return *obj;
    else return NULL;
}

// get_sidekick?
TActor *func_8002394C() {
    TActor **actorList;
    s32 count;

    actorList = TActor_getter(1, &count);

    if (count) {}

    if (count != 0) {
        return actorList[0];
    } else {
        return NULL;
    }
}

void func_80023984(TActor *obj) {
    obj->mapID = -1;
}

void func_80023994(TActor *obj) {
    obj->mapID = map_get_map_id_from_xz_ws(obj->srt.transl.x, obj->srt.transl.z);
}

// obj_integrate_speed
s32 func_800239C0(TActor *obj, f32 dx, f32 dy, f32 dz) {
    obj->srt.transl.x += dx;
    obj->srt.transl.y += dy;
    obj->srt.transl.z += dz;

    return 0;
}

// obj_set_update_priority
void func_80023A00(TActor *obj, s8 priority) {
    obj->updatePriority = priority;
}

void func_80023A18(TActor *obj, s32 param2) {
    obj->unk0xb0 &= 0xF0FF;

    if (param2 == obj->modelInstIdx) {
        return;
    }

    if (param2 < 0) {
        param2 = 0;
    } else if (param2 >= obj->data->modLinesIdx) {
        param2 = obj->data->modLinesIdx - 1;
    }

    obj->unk0xb0 |= 0x800;
    obj->unk0xb0 |= (param2 << 8) & 0x700;
}

void func_80023A78(TActor *obj, ModelInstance *modelInst, Model *model) {
    s32 modelInstIdx;
    ModelInstance *modelInst2;
    s32 prevAnimId;

    modelInstIdx = (obj->unk0xb0 & 0x700) >> 8;
    modelInst2 = obj->modelInsts[modelInstIdx];

    modelInst2->unk_0x34 &= 0xFFF0;
    modelInst2->unk_0x34 |= modelInst->unk_0x34 & 7;

    prevAnimId = obj->curAnimId;

    obj->modelInstIdx = modelInstIdx;
    obj->curAnimId = -1;
    obj->unk_0xa2 = -1;

    func_80023D30(obj, prevAnimId, obj->animTimer, 0);

    if (obj->objhitInfo != NULL) {
        if ((obj->objhitInfo->unk_0x5a & 0x10)) {
            obj->objhitInfo->unk_0x9f = 2;
        }

        obj->objhitInfo->unk_0x9e = 1;
    }

    obj->unk0xb0 &= 0xF0FF;
}

extern s8 D_800B1930;
extern TActor *D_800B1938[20];

// objAddEffectBox?
void func_80023B34(TActor *obj) {
    D_800B1938[D_800B1930] = obj;
    D_800B1930 += 1;
}

// objFreeEffectBox?
void func_80023B60(TActor *obj) {
    s32 i;
    s32 newCount;

    i = 0;

    while (i < D_800B1930 && obj != D_800B1938[i]) {
        i++;
    }

    newCount = D_800B1930 - 1;
    
    if (i != D_800B1930) {
        while (i < newCount) {
            D_800B1938[i] = D_800B1938[i + 1];
            i++;
        }

        D_800B1930 = newCount;
    }
}

void func_80023BF8(TActor *obj, s32 param2, s32 param3, s32 param4, u8 param5, u8 param6) {
    Actor78 *dst;

    if (obj != NULL) {
        dst = obj->unk_0x78;

        if (dst != NULL) {
            dst += obj->unk_0xd4;

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

            if (param6 != 0) {
                dst->unk4 = param6;
            }
        }
    }
}

void func_80023C6C(TActor *obj) {
    AButtonInteraction *src;
    Actor78 *dst;

    if (obj != NULL) {
        dst = obj->unk_0x78;

        if (dst != NULL) {
            src = &obj->data->aButtonInteraction[obj->unk_0xd4];
            dst += obj->unk_0xd4;

            dst->unk0 = src->unk0c;
            dst->unk1 = src->unk0d;
            dst->unk2 = src->unk0e;
            dst->unk3 = src->unk0f;
            dst->unk4 = src->unk10;
        }
    }
}

void func_80023CD8(TActor *obj, u16 param2) {
    if (param2 > obj->data->unk9b) {
        param2 = 0;
    }

    obj->unk_0xd4 = param2;
}

void func_80023D08(TActor *obj, u16 param2) {
    if (param2 > 4) {
        param2 = 0;
    }

    obj->unk_0xd8 = param2;
}

// start of objanim.c or something like that?
#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80023D30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800240BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80024108.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_8002493C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80024D74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80024DD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80024E2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80024E50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800250F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80025140.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80025540.s")
#else
//in retail SFA 1.0 I believe this is at 8002ed18
//but it's different because SFA doesn't use Model Instances
//or, this might be a completely different function...
void func_80025540(TActor *obj, s32 a1, s32 a2)
{
    Model *model = obj->models[obj->curModel];
    void **anims; //this is a struct of some sort.
    if(model->unk66) {
        anims = model->animations;
        if (!anims) { }
        func_800255F8(obj, model, anims, a1, (short) a2); //probably ObjSetBlendMove
        //not certain about the order/count of params here.
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_8002559C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_800255F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80025780.s")

#pragma GLOBAL_ASM("asm/nonmatchings/object/func_80025CD4.s")