// @DECOMP_OPT_FLAGS=-O1
#include "libultra/rmon/rmonint.h"

extern u32 D_800D3DC0;
extern u32 D_800D3DC4;
extern u32 D_800D3DC8[4];

static const char str_800a2b70[] = "GetGRegisters\n";
static const char str_800a2b80[] = "SetGRegisters\n";
static const char str_800a2b90[] = "GetFRegisters\n";
static const char str_800a2ba0[] = "SetFRegisters\n";
static const char str_800a2bb0[] = "GetSRegisters\n";
static const char str_800a2bc0[] = "SetSRegisters\n";
static const char str_800a2bd0[] = "GetVRegisters\n";
static const char str_800a2be0[] = "SetVRegs\n";

union LoadStoreSUStruct {
    u32 as_u32;
    struct {
        s32 unk0 : 6;
        s32 unk6 : 5;
        s32 unkB : 5;
    } bits;
};

void LoadStoreSU(s32 a0, s32 a1) {
    union LoadStoreSUStruct data;
    data.as_u32 = 0;
    data.bits.unk0 = a0;
    data.bits.unkB = a1;

    __rmonWriteWordTo((u32*)0x4001000, data.as_u32);
    __rmonWriteWordTo((u32*)0x4080000, 0);
}

union LoadStoreVUStruct {
    u32 as_u32;
    struct {
        s32 unk0 : 6;
        s32 unk6 : 5;
        s32 unkB : 5;
        s32 unk10 : 5;
    } bits;
};

void LoadStoreVU(s32 a0, s32 a1) {
    union LoadStoreVUStruct data;
    data.as_u32 = 0;
    data.bits.unk0 = a0;
    data.bits.unkB = a1;
    data.bits.unk10 = 4;

    __rmonWriteWordTo((u32*)0x4001000, data.as_u32);
    __rmonWriteWordTo((u32*)0x4080000, 0);
    
}

void SetUpForRCPop(s32 a0) {
    D_800D3DC0 = __rmonReadWordAt((u32*)0x4080000);
    D_800D3DC4 = __rmonReadWordAt((u32*)0x4001000);

    D_800D3DC8[0] = __rmonReadWordAt((u32*)0x4000000);

    if (a0 != 0) {
        D_800D3DC8[1] = __rmonReadWordAt((u32*)0x4000004);
        D_800D3DC8[2] = __rmonReadWordAt((u32*)0x4000008);
        D_800D3DC8[3] = __rmonReadWordAt((u32*)0x400000c);
    }
}

void CleanupFromRCPop(s32 a0) {
    __rmonWriteWordTo((u32*)0x4000000, D_800D3DC8[0]);

    if (a0 != 0) {
        __rmonWriteWordTo((u32*)0x4000004, D_800D3DC8[1]);
        __rmonWriteWordTo((u32*)0x4000008, D_800D3DC8[2]);
        __rmonWriteWordTo((u32*)0x400000c, D_800D3DC8[2]); // bug? shouldn't this be [3] to be consistent with SetUpForRCPop?
    }

    __rmonWriteWordTo((u32*)0x4001000, D_800D3DC4);
    __rmonWriteWordTo((u32*)0x4080000, D_800D3DC0);
}

int __rmonGetGRegisters(KKHeader* req) {
    register KKObjectRequest *request = (KKObjectRequest*)req;
    KKGregEvent event;
    OSThread *thread;
    u64 *var;
    register s32 i;

    event.tid = request->object;
    event.header.code = request->header.code;
    event.header.error = 0;

    if (request->header.method == 0) {
        thread = __rmonGetTCB(request->object);
        if (thread == NULL) {
            return -2;
        }

        for (i = 1, var = &thread->context.at; i < 26; i++, var++) {
            event.registers.gregs[i] = (u32)*var;
        }
        for (i = 28, var = &thread->context.gp; i < 34; i++, var++) {
            event.registers.gregs[i] = (u32)*var;
        }

        event.registers.gregs[34] = thread->context.cause;
        event.registers.gregs[35] = thread->context.pc;
        event.registers.gregs[36] = thread->context.sr;
        event.registers.gregs[0] = 0;
    } else {
        return -2;
    }

    __rmonSendReply((KKHeader *const)&event, 0xa4, 1);

    return 0;
}

int __rmonSetGRegisters(KKHeader* req) {
    register KKGRegsetRequest *request = (KKGRegsetRequest*)req;
    KKObjectEvent event;
    OSThread *thread;
    u64 *var;
    register s32 i;

    if (request->header.method == 0) {
        thread = __rmonGetTCB(request->tid);
        if (thread == NULL) {
            return -2;
        }

        for (i = 1, var = &thread->context.at; i < 26; i++, var++) {
            *var = (u64)(s32)request->registers.gregs[i];
        }
        for (i = 28, var = &thread->context.gp; i < 34; i++, var++) {
            *var = (u64)(s32)request->registers.gregs[i];
        }

        thread->context.cause = request->registers.gregs[34];
        thread->context.pc = request->registers.gregs[35];
        thread->context.sr = request->registers.gregs[36];
    } else {
        return -2;
    }

    event.object = request->tid;
    event.header.code = request->header.code;
    event.header.error = 0;
    __rmonSendReply((KKHeader *const)&event, 0x10, 1);

    return 0;
}

int __rmonGetFRegisters(KKHeader* req) {
    register KKObjectRequest *request = (KKObjectRequest*)req;
    KKFPregEvent event;
    OSThread *thread;
    volatile f32 var;

    if (req->method != 0) {
        return -2;
    }

    var = 0.0f;

    thread = __rmonGetTCB(request->object);
    if (thread == NULL) {
        return -2;
    }

    __rmonCopyWords((u32*)&event.registers, (u32*)&thread->context.fp0, 32);

    event.registers.fpcsr = thread->context.fpcsr;
    event.header.code = request->header.code;
    event.header.error = 0;
    event.tid = request->object;
    __rmonSendReply((KKHeader *const)&event, 0x98, 1);

    return 0;
}

int __rmonSetFRegisters(KKHeader* req) {
    register KKFPRegsetRequest *request = (KKFPRegsetRequest*)req;
    KKObjectEvent event;
    OSThread *thread;
    volatile f32 var;

    if (req->method != 0) {
        return -2;
    }

    var = 0.0f;

    thread = __rmonGetTCB(request->tid);
    if (thread == NULL) {
        return -2;
    }

    __rmonCopyWords((u32*)&thread->context.fp0, (u32*)request->registers.fpregs.fregs, 32);

    thread->context.fpcsr = request->registers.fpcsr;

    event.object = request->tid;
    event.header.code = request->header.code;
    event.header.error = 0;
    __rmonSendReply((KKHeader *const)&event, 0x10, 1);

    return 0;
}

u32 rmonGetRcpRegister(int regNumber) {
    u32 word;

    if (__rmonRCPrunning()) {
        return 0;
    }

    SetUpForRCPop(0);
    LoadStoreSU(0x2b, regNumber);
    __rmonStepRCP();
    word = __rmonReadWordAt((u32*)0x4000000);
    CleanupFromRCPop(0);

    return word;
}

int __rmonGetSRegs(KKHeader* req) {
    register KKObjectRequest *request = (KKObjectRequest*)req;
    KKCpSregEvent event;
    register s32 i;

    if (__rmonRCPrunning()) {
        return -4;
    }

    event.tid = request->object;
    event.header.code = request->header.code;
    event.header.error = 0;

    SetUpForRCPop(0);

    for (i = 0; i < 32; i++) {
        LoadStoreSU(0x2b, i);
        __rmonStepRCP();
        event.registers.sregs[i] = __rmonReadWordAt((u32*)0x4000000);
    }

    CleanupFromRCPop(0);

    event.registers.sregs[32] = __rmonReadWordAt((u32*)0x4040004);
    event.registers.sregs[33] = __rmonReadWordAt((u32*)0x4040000);
    event.registers.sregs[34] = __rmonReadWordAt((u32*)0x4040008);
    event.registers.sregs[35] = __rmonReadWordAt((u32*)0x4080000) + 0x4001000;
    event.registers.sregs[36] = __rmonReadWordAt((u32*)0x404000c);
    event.registers.sregs[37] = __rmonReadWordAt((u32*)0x4040010);
    event.registers.sregs[38] = __rmonReadWordAt((u32*)0x4040014);
    event.registers.sregs[39] = __rmonReadWordAt((u32*)0x4040018);

    __rmonSendReply((KKHeader *const)&event, 0xb0, 1);

    return 0;
}

int __rmonSetSRegs(KKHeader* req) {
    register KKCpScalarRegsetRequest *request = (KKCpScalarRegsetRequest*)req;
    KKObjectEvent event;
    register s32 i;

    if (__rmonRCPrunning()) {
        return -4;
    }

    SetUpForRCPop(0);

    for (i = 0; i < 32; i++) {
        __rmonWriteWordTo((u32*)0x4000000, request->registers.sregs[i]);
        LoadStoreSU(0x23, i);
        __rmonStepRCP();
    }

    CleanupFromRCPop(0);

    __rmonWriteWordTo((u32*)0x4040004, request->registers.sregs[32]);
    __rmonWriteWordTo((u32*)0x4040000, request->registers.sregs[33]);
    __rmonWriteWordTo((u32*)0x4080000, request->registers.sregs[35] & 0xfff);
    __rmonWriteWordTo((u32*)0x404000c, request->registers.sregs[36]);
    __rmonWriteWordTo((u32*)0x4040010, request->registers.sregs[37]);

    event.object = request->tid;
    event.header.code = request->header.code;
    event.header.error = 0;
    __rmonSendReply((KKHeader *const)&event, 0x10, 1);

    return 0;
}

int __rmonGetVRegs(KKHeader* req) {
    u8 *local_4;
    s32 local_8;
    s32 local_c;
    register KKObjectRequest *request = (KKObjectRequest*)req;
    KKCpVregEvent event;
    register s32 i;

    if (__rmonRCPrunning()) {
        return -4;
    } else {
        event.tid = request->object;
        event.header.code = request->header.code;
        event.header.error = 0;
        event.header.length = 0x210;

        local_c = 0x210;
        local_4 = (u8*)&local_c;

        local_8 = 0;
        while (local_8 < 4) {
            local_8 += __osRdbSend(&local_4[local_8], 4 - local_8, 8);
        }

        __rmonSendHeader((KKHeader *const)&event, 0x10, 1);

        SetUpForRCPop(1);

        for (i = 0; i < 32; i++) {
            LoadStoreVU(0x3a, i);
            __rmonStepRCP();
            __rmonSendData((char * const)0x4000000, 0x10);
        }

        CleanupFromRCPop(1);

        return 0;
    }
}

int __rmonSetVRegs(KKHeader* req) {
    register KKCpVectorRegsetRequest *request = (KKCpVectorRegsetRequest*)req;
    KKObjectEvent event;
    register s32 i;

    if (__rmonRCPrunning() != 0) {
        return -4;
    } else {
        SetUpForRCPop(1);

        for (i = 0; i < 32; i++) {
            __rmonCopyWords((u32*)0x4000000, (u32*)&request->registers.vregs[i], 4);
            LoadStoreVU(50, i);
            __rmonStepRCP();
        }

        CleanupFromRCPop(1);
        
        event.object = request->tid;
        event.header.code = request->header.code;
        event.header.error = 0;
        __rmonSendReply((KKHeader *const)&event, sizeof(KKObjectEvent), 1);

        return 0;
    }
}

u32 __rmonGetRegisterContents(int method, int threadNumber, int regNumber) {
    u32 *regPtr;
    OSThread *thread;

    if (method == 0) {
        if (regNumber >= 1 && regNumber <= 25) {
            regNumber -= 1;
        } else if (regNumber >= 28 && regNumber <= 31) {
            regNumber -= 3;
        } else {
            return 0;
        }

        thread = __rmonGetTCB(threadNumber);
        if (thread == NULL) {
            return 0;
        } else {
            regPtr = (u32*)&thread->context;
            regPtr += regNumber;
            return *regPtr;
        }
    } else {
        return rmonGetRcpRegister(regNumber);
    }
}
