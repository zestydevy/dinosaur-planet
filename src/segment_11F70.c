#include "common.h"
#include "bss.h"

// size: 0x38
typedef struct {
    s32 unk0;
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
    s32 unk30;
    s32 unk34;
} ContQueryStruct;

typedef struct {
    u8 tx;
    u8 rx;
    u8 cmd;
    u8 arg0;
    u8 arg1;
    u8 ret[8];
} UnkPifRequest;

typedef struct
{
    /* 0x0 */ UnkPifRequest unk0[4];
    /* 0x36 */ u8 pad[11];
    /* 0x3f */ s8 pifstatus;
} UnkOSPifRam;

extern s32 NOTosContSetch(OSMesgQueue *, void*);

BSS_GLOBAL UnkOSPifRam* D_800A8640;
BSS_GLOBAL UnkOSPifRam D_800A8648;

void controller_querey(OSMesgQueue* arg0, ContQueryStruct* arg1) {
    s32 i;
    OSContStatus sp24[4];

    D_800A8640 = mmAlloc(sizeof(UnkOSPifRam), 5, NULL);
    osContStartQuery(arg0);
    osRecvMesg(arg0, NULL, 1);
    osContGetQuery(sp24);

    for (i = 0; i < 4; i++) {
        arg1->unk4 = sp24[i].status;
        arg1->unkC = 0;
        arg1->unk8 = 0;
        
        if (sp24[i].errno == 0) {
            if (sp24[i].type == 9) {
                arg1->unk0 = 2;
            } else if (sp24[i].type == 5) {
                arg1->unk0 = 1;
            } else {
                arg1->unk0 = 0;
            }
        } else {
            arg1->unk0 = 0;
        }

        arg1++;
    }
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/segment_11F70/func_80011490.s")
s32 func_80011490(OSMesgQueue *arg0, UnkOSPifRam* arg1, ContQueryStruct* arg2);
#else
s32 func_80011490(OSMesgQueue *arg0, UnkOSPifRam* arg1, ContQueryStruct* arg2) {
    s32 i;
    s32 temp_v0;
    u8* sp1C;
    UnkPifRequest *req;

    sp1C = (u8*)&D_800A8640->unk0;
    bzero(D_800A8640, sizeof(UnkOSPifRam));
    for (i = 0; i < 4; i++) {
        req = (UnkPifRequest*)sp1C;
        req->tx = 3;
        req->rx = 8;
        req->cmd = 0x40;
        req->arg0 = 0;
        req->arg1 = arg2->unk8;

        sp1C += sizeof(UnkPifRequest);
        arg2++;
    }
    sp1C[0] = 0xfe;
    D_800A8640->pifstatus = 1;
    temp_v0 = NOTosContSetch(arg0, D_800A8640);
    bcopy(D_800A8640, arg1, sizeof(UnkOSPifRam));
    return temp_v0;
}
#endif

s32 func_80011588(OSMesgQueue* arg0, ContQueryStruct* arg1) {
    s32 i;

    func_80011490(arg0, &D_800A8648, arg1);
    
    for (i = 0; i < 4; i++) {
        if (!(D_800A8648.unk0[i].tx & 0x80)) {
            arg1->unk20 = D_800A8648.unk0[i].ret[2];
            arg1->unk24 = D_800A8648.unk0[i].ret[3];
            arg1->unk10 = arg1->unkC;
            arg1->unkC =  (D_800A8648.unk0[i].ret[0] << 8) + D_800A8648.unk0[i].ret[1];
            if (arg1->unk20 >= 0xD3) {
                arg1->unkC |= 4;
            }
            if (arg1->unk24 >= 0xD3) {
                arg1->unkC |= 0x40;
            }
            arg1->unk14 = (s32) (arg1->unkC & ~arg1->unk10);
            arg1->unk28 = D_800A8648.unk0[i].ret[5];
            arg1->unk2C = D_800A8648.unk0[i].ret[4];
            arg1->unk30 = D_800A8648.unk0[i].ret[7];
            arg1->unk34 = D_800A8648.unk0[i].ret[6];
            arg1->unk18 = arg1->unkC & arg1->unk10;
            if (arg1->unk18 != 0) {
                arg1->unk1C++;
                if (arg1->unk1C < 0x20) {
                    arg1->unk18 = 0;
                } else if ((arg1->unk1C < 0x50) && (arg1->unk1C & 3)) {
                    arg1->unk18 = 0;
                }
            } else {
                arg1->unk1C = 0;
            }
            arg1->unk18 |= arg1->unk14;
        } else {
            arg1->unk0 = 0;
        }
        arg1++;
    }
    
    return 0;
}

s32 func_800116D4(ContQueryStruct *param_1, s32 param_2) {
    param_1 += param_2;
    if (param_1->unk8) {}
    param_1->unk8 = 1;
    return 0;
}

s32 func_800116F8(ContQueryStruct *param_1, s32 param_2) {
    param_1 += param_2;
    if (param_1->unk8) {}
    param_1->unk8 = 0;
    return 0;
}
