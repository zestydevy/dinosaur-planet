#include "common.h"

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/create_asset_thread.s")
#else
extern u64 *D_800AC910; // end of stack
extern OSThread *D_800AC918;
void _create_asset_thread(void) {
    gDisableObjectStreamingFlag = 0;
    // both structs
    D_800ACBC8 = func_8000ADF0(&D_800ACBB8, &D_800ACBD0, 0x64, 0x1C);
    D_800AE1D0 = func_8000B010(&D_800AE1C0, &D_800AE1D8, 5, 0x14);
    osCreateThread(&D_800AC918, 0x63, &asset_thread_main, 0, &D_800AC910, 0xB);
    osStartThread(&D_800AC918);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/func_80012584.s")
#else
void _func_80012584(s32 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    u8 sp1C;
    s32 sp18;

    sp18 = func_with_status_reg();
    if (func_8000AFDC(D_800ACBC8) == 0) {
        sp1C = arg1;
        sp20 = arg2;
        sp24 = arg3;
        sp28 = arg4;
        sp2C = arg5;
        sp30 = arg6;
        sp34 = arg7;
        func_8000AE98(D_800ACBC8, &sp1C);
    }
    set_status_reg(sp18);
}
#endif

struct UnkMesg800AE270 {
	u8 unk0; // loading single or asset?
	u8 unk1; // type of asset?
	s32 unk4;
	s32 unk8;
	s32 unkC;
	s32 unk10;
	s32 unk14;
	s32 unk18;
	s32 unk1C;
	s32 unk20;
	s32 unk24;
	s32 unk28;
	s32 unk2C;
};
extern struct UnkMesg800AE270 D_800AE270;

extern OSMesgQueue D_800ACB48, D_800ACB68, D_800ACB98;

void queue_alloc_load_file(s32 arg0, s32 arg1) {
    D_800AE270.unk0 = 1;
    D_800AE270.unk1 = 0;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_file_to_ptr(s32 arg0, s32 arg1) {
    D_800AE270.unk0 = 1;
    D_800AE270.unk1 = 1;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_file_region_to_ptr(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_800AE270.unk0 = 1;
    D_800AE270.unk1 = 2;
    D_800AE270.unkC = arg3;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    D_800AE270.unk10 = arg2;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_map_object(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    D_800AE270.unk0 = 1;
    D_800AE270.unk1 = 4;
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
    D_800AE270.unk0 = 1;
    D_800AE270.unk1 = 3;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_dll(s32 arg0, s32 arg1, s32 arg2) {
    D_800AE270.unk0 = 1;
    D_800AE270.unk1 = 5;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    D_800AE270.unkC = arg2;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_model(s32 arg0, s32 arg1, s32 arg2) {
    D_800AE270.unk0 = 1;
    D_800AE270.unk1 = 6;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    D_800AE270.unkC = arg2;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

void queue_load_anim(s32 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4) {
    D_800AE270.unk0 = 1;
    D_800AE270.unk1 = 7;
    D_800AE270.unk20 = arg3;
    D_800AE270.unk4 = arg1;
    D_800AE270.unk8 = arg0;
    D_800AE270.unkC = arg2;
    D_800AE270.unk24 = arg4;
    osSendMesg(&D_800ACB48, &D_800AE270, 0);
    osRecvMesg(&D_800ACB98, 0, 1);
}

extern u8 gDisableObjectStreamingFlag;

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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/func_80012A4C.s")
#else
Failed to decompile function func_80012A4C:

Unable to determine jump table for jr instruction at /home/faris/Documents/dinosaur-planet/asm/nonmatchings/queue/func_80012A4C.s line 39.

There must be a read of a variable in the same block as
the instruction, which has a name starting with "jtbl"/"jpt_".
#endif

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
s32 func_with_status_reg(void);
extern u8 D_800AE29D, D_800AE29E;
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
	    switch (sp34->unk0) {
			case 0:
				asset_thread_load_single(sp34);
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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/asset_thread_load_single.s")
#else
Failed to decompile function asset_thread_load_single:

Unable to determine jump table for jr instruction at /home/faris/Documents/dinosaur-planet/asm/nonmatchings/queue/asset_thread_load_single.s line 30.

There must be a read of a variable in the same block as
the instruction, which has a name starting with "jtbl"/"jpt_".
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/queue/asset_thread_load_asset.s")
#else
Failed to decompile function asset_thread_load_asset:

Unable to determine jump table for jr instruction at /home/faris/Documents/dinosaur-planet/asm/nonmatchings/queue/asset_thread_load_asset.s line 13.

There must be a read of a variable in the same block as
the instruction, which has a name starting with "jtbl"/"jpt_".
#endif
