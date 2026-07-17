#include "game/objects/object.h"
#include "sys/asset.h"
#include "sys/dll.h"
#include "sys/pi.h"
#include "sys/interrupt_util.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/voxmap.h"
#include "macros.h"

/* -------- .bss start 800ab970 -------- */
u64 gAssetThreadStack[STACKSIZE(0xFA0) + 1];
OSThread gAssetThread;
OSMesgQueue gAssetLoadThreadSendQueue; //send load requests to asset thread
OSMesg D_800ACB60[1];
OSMesgQueue D_800ACB68; // signals a completed async load
OSMesg D_800ACB80[5];
OSMesgQueue gAssetLoadThreadRecvQueue; //receive acknowledgement from asset thread
OSMesg D_800ACBB0[1];
GenericQueue gAssetThreadQueueInternal;
GenericQueue *gAssetThreadQueue;
UnkStructAssetThreadSingle gAssetThreadQueueData[100];
GenericQueue D_800AD6C0;
UnkStructAssetThreadSingle D_800AD6D0[100];
GenericStack gAssetThreadStackInternal;
GenericStack *gAssetThreadGenericStack;
AssetThreadStackElement gAssetThreadStackData[5];
struct AssetLoadThreadMsg D_800AE240;
struct AssetLoadThreadMsg assetLoadMsg;
u8 gDisableObjectStreamingFlag;
u8 D_800AE29D;
u8 D_800AE29E;
/* -------- .bss end 800ae2a0 -------- */

const char load_error[] = "UNKNOWN load request\n";

void assetQueueProcessCompleted(void);
void assetThreadMain(void *arg);
void assetThreadLoadNextFromQueue(void);
void assetThreadLoad(struct AssetLoadThreadMsg *load);

void assetInit(void) {
    gDisableObjectStreamingFlag = 0;

    gAssetThreadQueue = genericQueueInit(
        &gAssetThreadQueueInternal, 
        (void*)&gAssetThreadQueueData, 
        100, 
        sizeof(UnkStructAssetThreadSingle));
    
    gAssetThreadGenericStack = genericStackInit(
        &gAssetThreadStackInternal, 
        (void*)&gAssetThreadStackData, 
        5, 
        sizeof(AssetThreadStackElement));
    
    osCreateThread(&gAssetThread, ASSET_THREAD_ID, &assetThreadMain, 0,
        &gAssetThreadStack[STACKSIZE(0xFA0)], ASSET_THREAD_PRIORITY);
    osStartThread(&gAssetThread);
}

void assetEnqueueLoad(
        s32 param1, 
        u8 param2, 
        u32 *param3, 
        ObjSetup *param4,
        s32 param5, 
        s32 param6,
        Object* param7,
        s32 param8
) {
    UnkStructAssetThreadSingle element;
    s32 prevIE;

    prevIE = disableInterrupts();

    if (!genericQueueIsFull(gAssetThreadQueue)) {
        element.unk0 = param2;
        element.unk4 = param3;
        element.unk8ObjSetup = param4;
        element.unkC = param5;
        element.unk10 = param6;
        element.unk14 = param7;
        element.unk18 = param8;

        genericQueueEnqueue(gAssetThreadQueue, &element);
    }

    enableInterrupts(prevIE);
}

void assetRomLoad(void **dest, s32 fileId) {
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_FILE;
    assetLoadMsg.p.file.id      = fileId;
    assetLoadMsg.p.file.dest    = dest;
    osSendMesg(&gAssetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&gAssetLoadThreadRecvQueue, 0, 1); //wait for acknowledge
}

void assetRomLoadToDest(void **dest, s32 fileId) {
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_ALLOCATED_FILE;
    assetLoadMsg.p.file.id      = fileId;
    assetLoadMsg.p.file.dest    = dest;
    osSendMesg(&gAssetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&gAssetLoadThreadRecvQueue, 0, 1);
}

void assetRomLoadSection(void **dest, s32 fileId, s32 offset, s32 length) {
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_FILE_REGION;
    assetLoadMsg.p.file.length  = length;
    assetLoadMsg.p.file.id      = fileId;
    assetLoadMsg.p.file.dest    = dest;
    assetLoadMsg.p.file.offset  = offset;
    osSendMesg(&gAssetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&gAssetLoadThreadRecvQueue, 0, 1);
}

void assetLoadObject(Object **dest, ObjSetup *setup, u32 initFlags, s32 mapID, s32 arg4, Object *parent, s32 arg6) {
    assetLoadMsg.loadCategory  = 1;
    assetLoadMsg.loadType      = ASSET_TYPE_OBJECT;
    assetLoadMsg.p.object.mapID = mapID;
    assetLoadMsg.p.object.setup = setup;
    assetLoadMsg.p.object.initFlags = initFlags;
    assetLoadMsg.p.object.arg4 = arg4;
    assetLoadMsg.p.object.parent = parent;
    assetLoadMsg.p.object.arg6 = arg6;
    assetLoadMsg.p.object.dest = dest;
    osSendMesg(&gAssetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&gAssetLoadThreadRecvQueue, 0, 1);
}

void assetLoadTexture(Texture **dest, s32 id) {
    //XXX verify types
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_TEXTURE;
    assetLoadMsg.p.texture.id   = id;
    assetLoadMsg.p.texture.dest = dest;
    osSendMesg(&gAssetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&gAssetLoadThreadRecvQueue, 0, 1);
}

void assetLoadDLL(void **dest, s32 idOrIdx, s32 exportCount) {
    //XXX verify types
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_DLL;
    assetLoadMsg.p.dll.idOrIdx  = idOrIdx;
    assetLoadMsg.p.dll.dest     = dest;
    assetLoadMsg.p.dll.exportCount = exportCount;
    osSendMesg(&gAssetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&gAssetLoadThreadRecvQueue, 0, 1);
}

void assetLoadModel(void **dest, s32 id, s32 arg2) {
    //XXX verify types
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_MODEL;
    assetLoadMsg.p.model.id     = id;
    assetLoadMsg.p.model.dest   = dest;
    assetLoadMsg.p.model.unkC   = arg2;
    osSendMesg(&gAssetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&gAssetLoadThreadRecvQueue, 0, 1);
}

void assetLoadAnim(void **dest, s16 id, s16 modAnimID, AmapPlusAnimation* amap, Model* model) {
    //XXX dest type
    assetLoadMsg.loadCategory     = 1;
    assetLoadMsg.loadType         = ASSET_TYPE_ANIMATION;
    assetLoadMsg.p.animation.arg3 = amap;
    assetLoadMsg.p.animation.id   = id;
    assetLoadMsg.p.animation.dest = dest;
    assetLoadMsg.p.animation.arg2 = modAnimID;
    assetLoadMsg.p.animation.arg4 = model;
    osSendMesg(&gAssetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&gAssetLoadThreadRecvQueue, 0, 1);
}

u8 assetIsObjQueueEnabled(void) {
    return gDisableObjectStreamingFlag;
}

void assetToggleObjQueue(u32 enabled) {
    gDisableObjectStreamingFlag = enabled;
}

void assetQueueTick(void) {
    s32 temp;
    s32 prevIE;

    prevIE = disableInterrupts();

    assetQueueProcessCompleted();

    if (gAssetThreadQueue->count != 0 && gAssetLoadThreadSendQueue.validCount == 0) {
        if (gAssetThreadQueue->count) {} // @fake match

        D_800AE240.loadCategory = 0;
        osSendMesg(&gAssetLoadThreadSendQueue, &D_800AE240, OS_MESG_NOBLOCK);
    }

    enableInterrupts(prevIE);
}

void assetQueueProcessCompleted(void) {
    AssetThreadStackElement sp24;

    // Flush queue
    while (osRecvMesg(&D_800ACB68, NULL, OS_MESG_NOBLOCK) != -1);

    while (gAssetThreadGenericStack->count != 0) {
        genericStackPop(gAssetThreadGenericStack, &sp24);

        switch (sp24.unk0) {
            case 5:
                objAddObject(sp24.unk4Object, sp24.unk8);
                break;
            case 3:
                if (sp24.unk4 != NULL) {
                    *sp24.unk4 = sp24.unk8;
                }
                break;
            case 1:
                blockEmplace(sp24.unk4Ptr, sp24.unk8, sp24.unkC, sp24.unk10);
                break;
            case 6:
                if (1) {};
            case 0:
                *sp24.unk4 = sp24.unk8;
                break;
            default:
            case 2:
            case 4:
                break;
        }
    }
}

// remove load messages of a given type (param1) from the async queue. 
// param2 further filters for type 4 (objects).
void assetQueueClearMesgType(s32 param1, s32 param2) {
    UnkStructAssetThreadSingle elementTemp;
    s32 prevIE;
    GenericQueue *qptr;
    ObjSetup *new_var;
    
    prevIE = disableInterrupts();

    assetQueueProcessCompleted();

    // Note: does not allocate, effectively just resets the temp queue
    genericQueueInit(&D_800AD6C0, (void*)&D_800AD6D0, 100, sizeof(UnkStructAssetThreadSingle));
    qptr = &D_800AD6C0;

    while (!genericQueueIsEmpty(gAssetThreadQueue)) {
        genericQueueDequeue(gAssetThreadQueue, &elementTemp);
        
        if (param1 != elementTemp.unk0) {
            genericQueueEnqueue(qptr, &elementTemp);
        } else {
            if ((param1 == 4) && ((new_var = ((0, elementTemp)).unk8ObjSetup)->uID != param2)) {
                genericQueueEnqueue(qptr, &elementTemp);
            }
        }
    }

    gAssetThreadQueue->count = qptr->count;
    gAssetThreadQueue->top = qptr->top;
    gAssetThreadQueue->bottom = qptr->bottom;
    gAssetThreadQueue->unk6 = qptr->unk6;

    bcopy(qptr->data, gAssetThreadQueue->data, qptr->capacity * sizeof(UnkStructAssetThreadSingle));

    // If in the middle of an queued load for the message type we are clearing, wait for it to finish and complete it.
    // TODO: If interrupts are disabled leading up to this, is this condition really even possible?
    if (D_800AE29D != 0 && param1 == D_800AE29E) {
        enableInterrupts(prevIE);

        osRecvMesg(&D_800ACB68, NULL, OS_MESG_BLOCK);

        prevIE = disableInterrupts();

        assetQueueProcessCompleted();
    }

    enableInterrupts(prevIE);
}

void assetQueueCompletedLoad(s32 param1, u32 *param2, u8 *param3, s32 param4, s32 param5) {
    s32 prevIE;
    AssetThreadStackElement element;

    prevIE = disableInterrupts();

    if ((gAssetThreadGenericStack->count + 1) != gAssetThreadGenericStack->capacity) { // !is_stack_empty
        element.unk0 = (u8)param1;
        element.unk4 = param2;
        element.unk8ptr = param3;
        element.unkC = param4;
        element.unk10 = param5;

        // Copy element into stack
        genericStackPush(gAssetThreadGenericStack, &element);
    }

    enableInterrupts(prevIE);
}

void assetThreadMain(void *arg) {
    OSMesg msg;
    s32 prevIE;
    struct AssetLoadThreadMsg *msg2;

    D_800AE29D = 0;
    D_800AE29E = 0;

    osCreateMesgQueue(&gAssetLoadThreadSendQueue, D_800ACB60, ARRAYCOUNT(D_800ACB60));
    osCreateMesgQueue(&D_800ACB68, D_800ACB80, ARRAYCOUNT(D_800ACB80));
    osCreateMesgQueue(&gAssetLoadThreadRecvQueue, D_800ACBB0, ARRAYCOUNT(D_800ACBB0));

    while (1) {
        osRecvMesg(&gAssetLoadThreadSendQueue, &msg, OS_MESG_BLOCK);

        msg2 = (struct AssetLoadThreadMsg *)msg;

        switch (msg2->loadCategory) {
            case 0:
                assetThreadLoadNextFromQueue();
                break;
            case 1:
            default:
                assetThreadLoad(msg2);
                break;
        }

        prevIE = disableInterrupts();
        D_800AE29D = 0;
        enableInterrupts(prevIE);
    }
}

void assetThreadLoadNextFromQueue(void) {
    UnkStructAssetThreadSingle sp2C;
    s32 prevIE;
    Texture *tmp;

    prevIE = disableInterrupts();

    if (!genericQueueIsEmpty(gAssetThreadQueue)) {
        genericQueueDequeue(gAssetThreadQueue, &sp2C);

        D_800AE29D = 1;
        D_800AE29E = sp2C.unk0;

        enableInterrupts(prevIE);

        switch (sp2C.unk0) {
            case 5:
                assetQueueCompletedLoad(5, (u32*)objSetupObjectActual(sp2C.unk8ObjSetup, 1, sp2C.unkC, sp2C.unk10, sp2C.unk14, sp2C.unk18), (u8*)1, 0, 0);
                break;
            case 3:
                tmp = texLoadTextureActual(sp2C.blockId, 0);
                if (sp2C.unk4 != 0) {
                    assetQueueCompletedLoad(3, sp2C.unk4, (u8*)tmp, 0, 0);
                }
                break;
            case 1:
                blockLoad(sp2C.blockId, sp2C.unkC, sp2C.unk10, /*fromAssetThread=*/TRUE);
                break;
            case 0:
                assetQueueCompletedLoad(0, sp2C.unk4, voxLoadSlot(sp2C.blockId, sp2C.unkC, sp2C.unk10, sp2C.unk14int), 0, 0);
                break;
            case 6:
                assetQueueCompletedLoad(6, sp2C.unk4, vox_func_80007620(sp2C.blockId, sp2C.unkC), 0, 0);
                break;
            default:
                break;
        }

        osSendMesg(&D_800ACB68, NULL, OS_MESG_NOBLOCK); // signal async load complete
        return;
    }

    enableInterrupts(prevIE);
}

void assetThreadLoad(struct AssetLoadThreadMsg *load) {
    switch (load->loadType) {
        case ASSET_TYPE_FILE:
            *load->p.file.dest = piRomLoad(load->p.file.id, 0);
            break;
        case ASSET_TYPE_ALLOCATED_FILE:
            piRomLoadToDest(load->p.file.id, load->p.file.dest);
            break;
        case ASSET_TYPE_FILE_REGION:
            piRomLoadSection(load->p.file.id, load->p.file.dest,
                load->p.file.offset, load->p.file.length);
            break;
        case ASSET_TYPE_OBJECT:
            *load->p.object.dest = objSetupObjectActual(load->p.object.setup,
                load->p.object.initFlags, load->p.object.mapID, load->p.object.arg4,
                load->p.object.parent, load->p.object.arg6);
            break;
        case ASSET_TYPE_TEXTURE:
            *load->p.texture.dest = texLoadTextureActual(load->p.texture.id, 0);
            break;
        case ASSET_TYPE_DLL:
            *load->p.dll.dest = dllLoadActual(load->p.dll.idOrIdx, load->p.dll.exportCount, FALSE);
            break;
        case ASSET_TYPE_MODEL:
            *load->p.model.dest = modLoadModelActual(load->p.model.id,
                load->p.model.unkC);
            break;
        case ASSET_TYPE_ANIMATION:
            *load->p.animation.dest = modLoadAnimActual((s16)load->p.animation.id,
                (s16)load->p.animation.arg2, load->p.animation.arg3, load->p.animation.arg4);
    }
    osSendMesg(&gAssetLoadThreadRecvQueue, NULL, 0);
}
