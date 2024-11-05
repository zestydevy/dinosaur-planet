#include "common.h"

extern GenericStack gAssetThreadStackInternal;
extern UnkStructFunc80012A4C gAssetThreadStackData[5];

extern GenericQueue *gAssetThreadQueueInternal;
extern UnkStructAssetThreadSingle gAssetThreadQueueData[100];

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
        sizeof(UnkStructFunc80012A4C));
    
    osCreateThread(&assetThread, ASSET_THREAD_ID, &asset_thread_main, 0,
        &assetThreadStackEnd, ASSET_THREAD_PRIORITY);
    osStartThread(&assetThread);
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/func_80012584.s")
#else
void _func_80012584(s32 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 sp18;
    // TODO: is it really?
    struct AssetLoadThreadMsg sp1C;

    sp18 = func_with_status_reg();
    if (generic_queue_is_full(gAssetThreadQueue) == 0) {
        sp1C.loadCategory = arg1;
        sp1C.unk4 = arg2;
        sp1C.unk8 = arg3;
        sp1C.unkC = arg4;
        sp1C.unk10 = arg5;
        sp1C.unk14 = arg6;
        sp1C.unk18 = arg7;
        generic_queue_enqueue(gAssetThreadQueue, &sp1C);
    }
    set_status_reg(sp18);
}
#endif

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

void queue_load_file_region_to_ptr(void **dest, s32 fileId, s32 length, s32 offset) {
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_FILE_REGION;
    assetLoadMsg.p.file.offset  = offset;
    assetLoadMsg.p.file.id      = fileId;
    assetLoadMsg.p.file.dest    = dest;
    assetLoadMsg.p.file.length  = length;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1);
}

void queue_load_map_object(void **dest, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    //XXX dest type
    assetLoadMsg.loadCategory  = 1;
    assetLoadMsg.loadType      = ASSET_TYPE_OBJECT;
    assetLoadMsg.p.object.arg3 = arg3;
    assetLoadMsg.p.object.arg1 = arg1;
    assetLoadMsg.p.object.arg2 = arg2;
    assetLoadMsg.p.object.arg4 = arg4;
    assetLoadMsg.p.object.arg5 = arg5;
    assetLoadMsg.p.object.arg6 = arg6;
    assetLoadMsg.p.object.dest = dest;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1);
}

void queue_load_texture(void **dest, s32 id) {
    //XXX verify types
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_TEXTURE;
    assetLoadMsg.p.texture.id   = id;
    assetLoadMsg.p.texture.dest = dest;
    osSendMesg(&assetLoadThreadSendQueue, &assetLoadMsg, 0);
    osRecvMesg(&assetLoadThreadRecvQueue, 0, 1);
}

void queue_load_dll(void **dest, s32 id, s32 param) {
    //XXX verify types
    assetLoadMsg.loadCategory   = 1;
    assetLoadMsg.loadType       = ASSET_TYPE_DLL;
    assetLoadMsg.p.dll.id       = id;
    assetLoadMsg.p.dll.dest     = dest;
    assetLoadMsg.p.dll.param    = param;
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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/func_800129E4.s")
#else
void _func_800129E4(void) {
    s32 sp18;

    sp18 = func_with_status_reg();
    func_80012A4C();
    if ((*gAssetThreadQueue != 0) && (D_800ACB48.unk8 == 0)) {
        D_800AE240 = 0;
        osSendMesg(&D_800ACB48, &D_800AE240, 0);
    }
    set_status_reg(sp18);
}
#endif

void func_80012A4C(void) {
    UnkStructFunc80012A4C sp24;

    while (osRecvMesg(&D_800ACB68, NULL, 0) != -1);

    while (gAssetThreadStack->count != 0) {
        generic_stack_pop(gAssetThreadStack, &sp24);

        switch (sp24.unk0) {
            case 5:
                func_80021A84(sp24.unk4, sp24.unk8);
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

// TODO: struct
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/func_80012B54.s")
#else
void _func_80012B54(s32 arg0, s32 arg1) {
    u8 sp3C;
    s32 sp38;

    sp38 = func_with_status_reg();
    func_80012A4C();
    generic_queue_init(&D_800AD6C0, &D_800AD6D0, 0x64, 0x1C);
    while (generic_queue_is_empty(gAssetThreadQueue) == 0) {
        generic_queue_dequeue(gAssetThreadQueue, &sp3C);
        if (arg0 != sp3C) {
            generic_queue_enqueue(&D_800AD6C0, &sp3C);
        } else if ((arg0 == 4) && (arg1 != sp44->unk14)) {
            generic_queue_enqueue(&D_800AD6C0, &sp3C);
        }
    }
    gAssetThreadQueue->unk0 = D_800AD6C0;
    gAssetThreadQueue->unk8 = D_800AD6C8;
    gAssetThreadQueue->unkA = D_800AD6CA;
    gAssetThreadQueue->unk6 = D_800AD6C6;
    bcopy(*0x800AD6CC, gAssetThreadQueue->unkC, *0x800AD6C2 * 0x1C);
    if ((D_800AE29D != 0) && (arg0 == D_800AE29E)) {
        set_status_reg(sp38);
        osRecvMesg(&D_800ACB68, 0, 1);
        sp38 = func_with_status_reg();
        func_80012A4C();
    }
    set_status_reg(sp38);
}
#endif

// FIXME: should be a different name?
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/queue_block_emplace.s")
#else
void _queue_block_emplace(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s8 sp20;
    s32 temp_a2;
    s32 temp_v0;
    s8 *temp_a1;
    void *temp_a0;
    s32 phi_a2;

    temp_v0 = func_with_status_reg();
    temp_a0 = gAssetThreadStack;
    temp_a2 = temp_v0;
    temp_a1 = &sp20;
    phi_a2 = temp_a2;
    if (temp_a0->unk2 != (temp_a0->unk0 + 1)) {
        sp20 = arg0;
        sp34 = temp_v0;
        sp24 = arg1;
        sp28 = arg2;
        sp2C = arg3;
        sp30 = arg4;
        generic_stack_push(temp_a0, temp_a1, temp_a2);
        phi_a2 = sp34;
    }
    set_status_reg(phi_a2);
}
#endif


// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/asset_thread_main.s")
#else
extern s32 func_with_status_reg(void);
void asset_thread_load_single(void);
void asset_thread_load_asset(struct AssetLoadThreadMsg *);
extern struct AssetLoadThreadMsg D_800ACB60, D_800ACB80, D_800ACBB0;

void _asset_thread_main(s32 arg0) {
    struct AssetLoadThreadMsg *sp34;
    s32 temp_v0;

    D_800AE29D = 0;
    D_800AE29E = 0;
    osCreateMesgQueue(&D_800ACB48, &D_800ACB60, 1);
    osCreateMesgQueue(&D_800ACB68, &D_800ACB80, 5);
    osCreateMesgQueue(&D_800ACB98, &D_800ACBB0, 1);
    while (1) {
        osRecvMesg(&D_800ACB48, &sp34, 1);
        switch (sp34->loadCategory) {
            case 0:
                asset_thread_load_single();
                break;
            case 1:
            default:
                asset_thread_load_asset(sp34);
                break;
        }
        temp_v0 = func_with_status_reg();
        D_800AE29D = 0;
        set_status_reg(temp_v0);
    }
}
#endif

void asset_thread_load_single(void) {
    UnkStructAssetThreadSingle sp2C;
    s32 sp28;
    s32 tmp;

    sp28 = func_with_status_reg();
    if (generic_queue_is_empty(gAssetThreadQueue) == 0) {
        generic_queue_dequeue(gAssetThreadQueue, &sp2C);
        D_800AE29D = 1;
        D_800AE29E = sp2C.unk0;
        set_status_reg(sp28);
        switch (sp2C.unk0) {
            case 5:
                queue_block_emplace(5, objSetupObjectActual(sp2C.unk8, 1, sp2C.unkC, sp2C.unk10, sp2C.unk14, sp2C.unk18), 1, 0, 0);
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
                queue_block_emplace(0, sp2C.unk4, func_80007468(sp2C.unk8, sp2C.unkC, sp2C.unk10, sp2C.unk14), 0, 0);
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
    set_status_reg(sp28);
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
                load->p.file.length, load->p.file.offset);
            break;
        case ASSET_TYPE_OBJECT:
            *load->p.object.dest = objSetupObjectActual(load->p.object.arg1,
                load->p.object.arg2, load->p.object.arg3, load->p.object.arg4,
                load->p.object.arg5, load->p.object.arg6);
            break;
        case ASSET_TYPE_TEXTURE:
            *load->p.texture.dest = texture_load(load->p.texture.id, 0);
            break;
        case ASSET_TYPE_DLL:
            *load->p.dll.dest = dll_load((u16)load->p.dll.id, (u16)load->p.dll.param, 0);
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


