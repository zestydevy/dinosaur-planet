#include "common.h"
#include "queue.h"

struct UnkStruct8000ADF0 {
    s16 unk0;
};
extern struct UnkStruct8000ADF0 *D_800ACBC8;
struct UnkStruct8000ADF0 *func_8000ADF0(s32 *, s32 *, s32, s32);

struct UnkStruct8000B010 {
    s16 unk0;
};
extern struct UnkStruct8000B010 *D_800AE1D0;
struct UnkStruct8000B010 *func_8000B010(s32 *, s32 *, s32, s32);

extern u8 gDisableObjectStreamingFlag;

extern u8 D_800AE29D, D_800AE29E;

void asset_thread_main(void);

extern s32 *D_800ACBB8, *D_800ACBD0;

extern s32 *D_800AE1C0, *D_800AE1D8;

extern u64 *D_800AC910; // end of stack
extern OSThread *D_800AC918;

void create_asset_thread(void) {
    gDisableObjectStreamingFlag = 0;
    D_800ACBC8 = func_8000ADF0(&D_800ACBB8, &D_800ACBD0, 0x64, 0x1C);
    D_800AE1D0 = func_8000B010(&D_800AE1C0, &D_800AE1D8, 5, 0x14);
    osCreateThread(&D_800AC918, 0x63, &asset_thread_main, 0, &D_800AC910, 0xB);
    osStartThread(&D_800AC918);
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/func_80012584.s")
#else
void _func_80012584(s32 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 sp18;
    // TODO: is it really?
    struct UnkMesg800AE270 sp1C;

    sp18 = func_with_status_reg();
    if (func_8000AFDC(D_800ACBC8) == 0) {
        sp1C.loadCategory = arg1;
        sp1C.unk4 = arg2;
        sp1C.unk8 = arg3;
        sp1C.unkC = arg4;
        sp1C.unk10 = arg5;
        sp1C.unk14 = arg6;
        sp1C.unk18 = arg7;
        func_8000AE98(D_800ACBC8, &sp1C);
    }
    set_status_reg(sp18);
}
#endif

void queue_alloc_load_file(s32 arg0, s32 arg1) {
    D_800AE270.loadCategory = 1;
    D_800AE270.loadType = QUEUE_FILE;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_file_to_ptr(s32 arg0, s32 arg1) {
    D_800AE270.loadCategory = 1;
    D_800AE270.loadType = QUEUE_ALLOCATED_FILE;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_file_region_to_ptr(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_800AE270.loadCategory = 1;
    D_800AE270.loadType = QUEUE_FILE_REGION;
    D_800AE270.unkC = arg3;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    D_800AE270.unk10 = arg2;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_map_object(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    D_800AE270.loadCategory = 1;
    D_800AE270.loadType = QUEUE_OBJECT;
    D_800AE270.unk24 = arg3;
    D_800AE270.unk18 = arg1;
    D_800AE270.unk1C = arg2;
    D_800AE270.unk20 = arg4;
    D_800AE270.unk14 = arg5;
    D_800AE270.unk28 = arg6;
    D_800AE270.unk8 = arg0;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_texture(s32 arg0, s32 arg1) {
    D_800AE270.loadCategory = 1;
    D_800AE270.loadType = QUEUE_TEXTURE;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_dll(s32 arg0, s32 arg1, s32 arg2) {
    D_800AE270.loadCategory = 1;
    D_800AE270.loadType = QUEUE_DLL;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    D_800AE270.unkC = arg2;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_model(s32 arg0, s32 arg1, s32 arg2) {
    D_800AE270.loadCategory = 1;
    D_800AE270.loadType = QUEUE_MODEL;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    D_800AE270.unkC = arg2;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

const char load_error[] = "UNKNOWN load request\n";

void queue_load_anim(s32 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4) {
    D_800AE270.loadCategory = 1;
    D_800AE270.loadType = QUEUE_ANIMATION;
    D_800AE270.unk20 = arg3;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    D_800AE270.unkC = arg2;
    D_800AE270.unk24 = arg4;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

u8 map_get_is_object_streaming_disabled(void) {
    return gDisableObjectStreamingFlag;
}

void func_800129D4(u32 arg0) {
    gDisableObjectStreamingFlag = arg0;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/func_800129E4.s")
#else
void _func_800129E4(void) {
    s32 sp18;

    sp18 = func_with_status_reg();
    func_80012A4C();
    if ((*D_800ACBC8 != 0) && (D_800ACB48.unk8 == 0)) {
        D_800AE240 = 0;
        osSendMesg(&D_800ACB48, &D_800AE240, 0);
    }
    set_status_reg(sp18);
}
#endif

struct UnkStructFunc80012A4C {
    u8 unk0;
    u32 *unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
};
void func_80012A4C(void) {
    struct UnkStructFunc80012A4C sp24;

    while (osRecvMesg(&D_800ACB68, NULL, 0) != -1);

    while (D_800AE1D0->unk0 != 0) {
        func_8000B124(D_800AE1D0, &sp24);

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
                func_80049024(sp24.unk4, sp24.unk8, sp24.unkC, sp24.unk10);
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
    func_8000ADF0(&D_800AD6C0, &D_800AD6D0, 0x64, 0x1C);
    while (func_8000AFF8(D_800ACBC8) == 0) {
        func_8000AF0C(D_800ACBC8, &sp3C);
        if (arg0 != sp3C) {
            func_8000AE98(&D_800AD6C0, &sp3C);
        } else if ((arg0 == 4) && (arg1 != sp44->unk14)) {
            func_8000AE98(&D_800AD6C0, &sp3C);
        }
    }
    D_800ACBC8->unk0 = D_800AD6C0;
    D_800ACBC8->unk8 = D_800AD6C8;
    D_800ACBC8->unkA = D_800AD6CA;
    D_800ACBC8->unk6 = D_800AD6C6;
    _bcopy(*0x800AD6CC, D_800ACBC8->unkC, *0x800AD6C2 * 0x1C);
    if ((D_800AE29D != 0) && (arg0 == D_800AE29E)) {
        set_status_reg(sp38);
        osRecvMesg(&D_800ACB68, 0, 1);
        sp38 = func_with_status_reg();
        func_80012A4C();
    }
    set_status_reg(sp38);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/func_80012D04.s")
#else
void _func_80012D04(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
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
    temp_a0 = D_800AE1D0;
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
        func_8000B0B0(temp_a0, temp_a1, temp_a2);
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
void asset_thread_load_asset(struct UnkMesg800AE270 *);
extern struct UnkMesg800AE270 D_800ACB60, D_800ACB80, D_800ACBB0;

void _asset_thread_main(s32 arg0) {
    struct UnkMesg800AE270 *sp34;
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
    struct UnkStructAssetThreadSingle sp2C;
    s32 sp28;
    s32 tmp;

    sp28 = func_with_status_reg();
    if (func_8000AFF8(D_800ACBC8) == 0) {
        func_8000AF0C(D_800ACBC8, &sp2C);
        D_800AE29D = 1;
        D_800AE29E = sp2C.unk0;
        set_status_reg(sp28);
        switch (sp2C.unk0) {
            case 5:
                func_80012D04(5, objSetupObjectActual(sp2C.unk8, 1, sp2C.unkC, sp2C.unk10, sp2C.unk14, sp2C.unk18), 1, 0, 0);
                break;
            case 3:
                tmp = texture_load(sp2C.unk8, 0);
                if (sp2C.unk4 != 0) {
                    func_80012D04(3, sp2C.unk4, tmp, 0, 0);
                }
                break;
            case 1:
                func_80048724(sp2C.unk8, sp2C.unkC, sp2C.unk10, 1);
                break;
            case 0:
                func_80012D04(0, sp2C.unk4, func_80007468(sp2C.unk8, sp2C.unkC, sp2C.unk10, sp2C.unk14), 0, 0);
                break;
            case 6:
                func_80012D04(6, sp2C.unk4, func_80007620(sp2C.unk8, sp2C.unkC), 0, 0);
                break;
            default:
                break;
        }
        osSendMesg(&D_800ACB68, NULL, 0);
        return;
    }
    set_status_reg(sp28);
} 

void asset_thread_load_asset(struct UnkMesg800AE270 *arg0) {
    switch (arg0->loadType) {
        case QUEUE_FILE:
            *arg0->unk8 = read_alloc_file(arg0->unk4, 0);
            break;
        case QUEUE_ALLOCATED_FILE:
            read_file(arg0->unk4, arg0->unk8);
            break;
        case QUEUE_FILE_REGION:
            read_file_region(arg0->unk4, arg0->unk8, arg0->unk10, arg0->unkC);
            break;
        case QUEUE_OBJECT:
            *arg0->unk8 = objSetupObjectActual(arg0->unk18, arg0->unk1C, arg0->unk24, arg0->unk20, arg0->unk14, arg0->unk28);
            break;
        case QUEUE_TEXTURE:
            *arg0->unk8 = texture_load(arg0->unk4, 0);
            break;
        case QUEUE_DLL:
            *arg0->unk8 = dll_load((u16) arg0->unk4, (u16) arg0->unkC, 0);
            break;
        case QUEUE_MODEL:
            *arg0->unk8 = model_load_create_instance(arg0->unk4, arg0->unkC);
            break;
        case QUEUE_ANIMATION:
            *arg0->unk8 = anim_load((s16) arg0->unk4, (s16) arg0->unkC, arg0->unk20, arg0->unk24);
    }
    osSendMesg(&D_800ACB98, NULL, 0);
}

