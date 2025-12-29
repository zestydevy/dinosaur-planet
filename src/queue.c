#include "game/objects/object.h"
#include "sys/asset_thread.h"
#include "sys/dll.h"
#include "sys/fs.h"
#include "sys/interrupt_util.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/voxmap.h"
#include "functions.h"
#include "macros.h"

extern GenericStack gAssetThreadStackInternal;
extern AssetThreadStackElement gAssetThreadStackData[5];

extern GenericQueue gAssetThreadQueueInternal;
extern UnkStructAssetThreadSingle gAssetThreadQueueData[100];

extern u8 D_800AE240;

extern OSMesg D_800ACB60[1];
extern OSMesg D_800ACB80[5];
extern OSMesg D_800ACBB0[1];

// pad to 0x800ad6c0
static u8 _bss_pad[0x5180]; // TODO: remove bss padding

void func_80012A4C();
void asset_thread_load_single(void);
void asset_thread_load_asset(struct AssetLoadThreadMsg *load);

void create_asset_thread(void) {
    gDisableObjectStreamingFlag = 0;

    gAssetThreadQueue = generic_queue_init(
        &gAssetThreadQueueInternal, 
        (void*)&gAssetThreadQueueData, 
        100, 
        sizeof(UnkStructAssetThreadSingle));
    
    gAssetThreadStack = generic_stack_init(
        &gAssetThreadStackInternal, 
        (void*)&gAssetThreadStackData, 
        5, 
        sizeof(AssetThreadStackElement));
    
    osCreateThread(&assetThread, ASSET_THREAD_ID, &asset_thread_main, 0,
        &assetThreadStackEnd, ASSET_THREAD_PRIORITY);
    osStartThread(&assetThread);
}

void func_80012584(
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

    prevIE = interrupts_disable();

    if (!generic_queue_is_full(gAssetThreadQueue)) {
        element.unk0 = param2;
        element.unk4 = param3;
        element.unk8 = param4;
        element.unkC = param5;
        element.unk10 = param6;
        element.unk14 = param7;
        element.unk18 = param8;

        generic_queue_enqueue(gAssetThreadQueue, &element);
    }

    interrupts_enable(prevIE);
}

void queue_alloc_load_file(void **dest, s32 fileId) {
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_FILE;
    assetLoadMsg.p.file.id      = fileId;
    assetLoadMsg.p.file.dest    = dest;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1); //wait for acknowledge
}

void queue_load_file_to_ptr(void **dest, s32 fileId) {
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_ALLOCATED_FILE;
    assetLoadMsg.p.file.id      = fileId;
    assetLoadMsg.p.file.dest    = dest;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1);
}

void queue_load_file_region_to_ptr(void **dest, s32 fileId, s32 offset, s32 length) {
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_FILE_REGION;
    assetLoadMsg.p.file.length  = length;
    assetLoadMsg.p.file.id      = fileId;
    assetLoadMsg.p.file.dest    = dest;
    assetLoadMsg.p.file.offset  = offset;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1);
}

void queue_load_map_object(Object **dest, ObjSetup *setup, u32 initFlags, s32 mapID, s32 arg4, Object *parent, s32 arg6) {
    assetLoadMsg.loadCategory  = 1;
    assetLoadMsg.loadType      = ASSET_TYPE_OBJECT;
    assetLoadMsg.p.object.mapID = mapID;
    assetLoadMsg.p.object.setup = setup;
    assetLoadMsg.p.object.initFlags = initFlags;
    assetLoadMsg.p.object.arg4 = arg4;
    assetLoadMsg.p.object.parent = parent;
    assetLoadMsg.p.object.arg6 = arg6;
    assetLoadMsg.p.object.dest = dest;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1);
}

void queue_load_texture(Texture **dest, s32 id) {
    //XXX verify types
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_TEXTURE;
    assetLoadMsg.p.texture.id   = id;
    assetLoadMsg.p.texture.dest = dest;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1);
}

void queue_load_dll(void **dest, s32 idOrIdx, s32 exportCount) {
    //XXX verify types
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_DLL;
    assetLoadMsg.p.dll.idOrIdx  = idOrIdx;
    assetLoadMsg.p.dll.dest     = dest;
    assetLoadMsg.p.dll.exportCount = exportCount;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1);
}

void queue_load_model(void **dest, s32 id, s32 arg2) {
    //XXX verify types
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_MODEL;
    assetLoadMsg.p.model.id     = id;
    assetLoadMsg.p.model.dest   = dest;
    assetLoadMsg.p.model.unkC   = arg2;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1);
}

const char load_error[] = "UNKNOWN load request\n";

void queue_load_anim(void **dest, s16 id, s16 arg2, s32 arg3, s32 arg4) {
    //XXX dest type
    assetLoadMsg.loadCategory     = 1;
    assetLoadMsg.loadType         = ASSET_TYPE_ANIMATION;
    assetLoadMsg.p.animation.arg3 = arg3;
    assetLoadMsg.p.animation.id   = id;
    assetLoadMsg.p.animation.dest = dest;
    assetLoadMsg.p.animation.arg2 = arg2;
    assetLoadMsg.p.animation.arg4 = arg4;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1);
}

u8 map_get_is_object_streaming_disabled(void) {
    return gDisableObjectStreamingFlag;
}

void map_set_object_streaming_disabled(u32 arg0) {
    gDisableObjectStreamingFlag = arg0;
}

void func_800129E4() {
    s32 temp;
    s32 prevIE;

    prevIE = interrupts_disable();

    func_80012A4C();

    if (gAssetThreadQueue->count != 0 && assetLoadThreadSendQueue.validCount == 0) {
        if (gAssetThreadQueue->count) {} // fake match

        D_800AE240 = 0;
        osSendMesg(&assetLoadThreadSendQueue, &D_800AE240, OS_MESG_NOBLOCK);
    }

    interrupts_enable(prevIE);
}

void func_80012A4C(void) {
    AssetThreadStackElement sp24;

    while (osRecvMesg(&D_800ACB68, NULL, 0) != -1);

    while (gAssetThreadStack->count != 0) {
        generic_stack_pop(gAssetThreadStack, &sp24);

        switch (sp24.unk0) {
            case 5:
                obj_add_object(sp24.unk4, sp24.unk8);
                break;
            case 3:
                if (sp24.unk4 != NULL) {
                    *sp24.unk4 = sp24.unk8;
                }
                break;
            case 1:
                block_emplace(sp24.unk4, sp24.unk8, sp24.unkC, sp24.unk10);
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

void func_80012B54(s32 param1, s32 param2) {
    static GenericQueue D_800AD6C0;
    static UnkStructAssetThreadSingle D_800AD6D0[100];

    UnkStructAssetThreadSingle elementTemp;
    s32 prevIE;
    UnkStructAssetThreadSingle *ptr;
    ObjSetup *ptr_unk8;
    
    prevIE = interrupts_disable();

    func_80012A4C();

    // Note: does not allocate, effectively just resets the temp queue
    generic_queue_init(&D_800AD6C0, (void*)&D_800AD6D0, 100, sizeof(UnkStructAssetThreadSingle));

    while (!generic_queue_is_empty(gAssetThreadQueue)) {
        ptr = &elementTemp;
        generic_queue_dequeue(gAssetThreadQueue, ptr);
        
        if (param1 != ptr->unk0) {
            generic_queue_enqueue(&D_800AD6C0, ptr);
        } else {
            ptr_unk8 = ptr->unk8;
            if (param1 == 4 && ptr_unk8->uID != param2) {
                generic_queue_enqueue(&D_800AD6C0, ptr);
            }
        }
    }

    gAssetThreadQueue->count = D_800AD6C0.count;
    gAssetThreadQueue->top = D_800AD6C0.top;
    gAssetThreadQueue->bottom = D_800AD6C0.bottom;
    gAssetThreadQueue->unk6 = D_800AD6C0.unk6;

    bcopy(D_800AD6C0.data, gAssetThreadQueue->data, D_800AD6C0.capacity * sizeof(UnkStructAssetThreadSingle));

    if (D_800AE29D != 0 && param1 == D_800AE29E) {
        interrupts_enable(prevIE);

        osRecvMesg(&D_800ACB68, NULL, OS_MESG_BLOCK);

        prevIE = interrupts_disable();

        func_80012A4C();
    }

    interrupts_enable(prevIE);
}

// FIXME: should be a different name?
void queue_block_emplace(s32 param1, u32 *param2, s32 param3, s32 param4, s32 param5) {
    s32 prevIE;
    AssetThreadStackElement element;

    prevIE = interrupts_disable();

    if ((gAssetThreadStack->count + 1) != gAssetThreadStack->capacity) { // !is_stack_empty
        element.unk0 = (u8)param1;
        element.unk4 = param2;
        element.unk8 = param3;
        element.unkC = param4;
        element.unk10 = param5;

        // Copy element into stack
        generic_stack_push(gAssetThreadStack, &element);
    }

    interrupts_enable(prevIE);
}

void asset_thread_main(void *arg) {
    OSMesg msg;
    s32 prevIE;
    struct AssetLoadThreadMsg *msg2;

    D_800AE29D = 0;
    D_800AE29E = 0;

    osCreateMesgQueue(&assetLoadThreadSendQueue, D_800ACB60, ARRAYCOUNT(D_800ACB60));
    osCreateMesgQueue(&D_800ACB68, D_800ACB80, ARRAYCOUNT(D_800ACB80));
    osCreateMesgQueue(&assetLoadThreadRecvQueue, D_800ACBB0, ARRAYCOUNT(D_800ACBB0));

    while (1) {
        osRecvMesg(&assetLoadThreadSendQueue, &msg, OS_MESG_BLOCK);

        msg2 = (struct AssetLoadThreadMsg *)msg;

        switch (msg2->loadCategory) {
            case 0:
                asset_thread_load_single();
                break;
            case 1:
            default:
                asset_thread_load_asset(msg2);
                break;
        }

        prevIE = interrupts_disable();
        D_800AE29D = 0;
        interrupts_enable(prevIE);
    }
}

void asset_thread_load_single(void) {
    UnkStructAssetThreadSingle sp2C;
    s32 prevIE;
    Texture *tmp;

    prevIE = interrupts_disable();

    if (!generic_queue_is_empty(gAssetThreadQueue)) {
        generic_queue_dequeue(gAssetThreadQueue, &sp2C);

        D_800AE29D = 1;
        D_800AE29E = sp2C.unk0;

        interrupts_enable(prevIE);

        switch (sp2C.unk0) {
            case 5:
                queue_block_emplace(5, obj_setup_object(sp2C.unk8, 1, sp2C.unkC, sp2C.unk10, sp2C.unk14, sp2C.unk18), 1, 0, 0);
                break;
            case 3:
                tmp = texture_load(sp2C.unk8, 0);
                if (sp2C.unk4 != 0) {
                    queue_block_emplace(3, sp2C.unk4, tmp, 0, 0);
                }
                break;
            case 1:
                block_load(sp2C.unk8, sp2C.unkC, sp2C.unk10, 1);
                break;
            case 0:
                queue_block_emplace(0, sp2C.unk4, voxmap_load_slot(sp2C.unk8, sp2C.unkC, sp2C.unk10, sp2C.unk14), 0, 0);
                break;
            case 6:
                queue_block_emplace(6, sp2C.unk4, func_80007620(sp2C.unk8, sp2C.unkC), 0, 0);
                break;
            default:
                break;
        }

        osSendMesg(&D_800ACB68, NULL, 0);
        return;
    }

    interrupts_enable(prevIE);
}

void asset_thread_load_asset(struct AssetLoadThreadMsg *load) {
    switch (load->loadType) {
        case ASSET_TYPE_FILE:
            *load->p.file.dest = read_alloc_file(load->p.file.id, 0);
            break;
        case ASSET_TYPE_ALLOCATED_FILE:
            read_file(load->p.file.id, load->p.file.dest);
            break;
        case ASSET_TYPE_FILE_REGION:
            read_file_region(load->p.file.id, load->p.file.dest,
                load->p.file.offset, load->p.file.length);
            break;
        case ASSET_TYPE_OBJECT:
            *load->p.object.dest = obj_setup_object(load->p.object.setup,
                load->p.object.initFlags, load->p.object.mapID, load->p.object.arg4,
                load->p.object.parent, load->p.object.arg6);
            break;
        case ASSET_TYPE_TEXTURE:
            *load->p.texture.dest = texture_load(load->p.texture.id, 0);
            break;
        case ASSET_TYPE_DLL:
            *load->p.dll.dest = dll_load(load->p.dll.idOrIdx, load->p.dll.exportCount, FALSE);
            break;
        case ASSET_TYPE_MODEL:
            *load->p.model.dest = model_load_create_instance(load->p.model.id,
                load->p.model.unkC);
            break;
        case ASSET_TYPE_ANIMATION:
            *load->p.animation.dest = anim_load((s16)load->p.animation.id,
                (s16)load->p.animation.arg2, load->p.animation.arg3, load->p.animation.arg4);
    }
    osSendMesg(&assetLoadThreadRecvQueue, NULL, 0);
}


