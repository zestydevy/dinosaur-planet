// @DECOMP_OPT_FLAGS=-O1
#include "common.h"
#include <PR/os_internal.h>
#include "libultra/rmon/rmonint.h"

struct UnkRmonClearBreakStruct {
    u32 *unk0;
    u32 unk4;
};

extern struct UnkRmonClearBreakStruct rmonbrk_bss_0000[16];
extern struct UnkRmonClearBreakStruct rmonbrk_bss_0080;
extern struct UnkRmonClearBreakStruct rmonbrk_bss_0088[16];

static const char str_800a2bf0[] = "Set temp BP at %08x";
static const char str_800a2c04[] = " and %08x";
static const char str_800a2c10[] = "\n";
static const char str_800a2c14[] = "ClearTempBreak @ %08x\n";
static const char str_800a2c2c[] = "ClearTempBreak @ %08x\n";
static const char str_800a2c44[] = "SetBreak at %08x, method %d\n";
static const char str_800a2c64[] = "* (%08x) = %08x (was %08x)\n";
static const char str_800a2c80[] = "ListBreak\n";
static const char str_800a2c8c[] = "ClearBreak\n";
static const char str_800a2c98[] = "SingleStep\n";
static const char str_800a2ca4[] = "Break %d in thread %d\n";
static const char str_800a2cbc[] = "HitBreak\n";
static const char str_800a2cc8[] = "Hit SP Break\n";
static const char str_800a2cd8[] = "HitCpuFault\n";
static const char str_800a2ce8[] = "Brk in thread %d @ %08x, inst %08x\r\n";

void rmonFindFaultedThreads();

void SetTempBreakpoint(u32* arg0, u32* arg1) {
    rmonbrk_bss_0000[0].unk4 = (s32) *arg0;
    *arg0 = 0x40D;
    osWritebackDCache((void*)arg0, 4);
    osInvalICache((void*)arg0, 4);
    rmonbrk_bss_0000[0].unk0 = arg0;
    if (arg1 != NULL) {
        rmonbrk_bss_0080.unk4 = *arg1;
        *arg1 = 0x40D;
        osWritebackDCache(arg1, 4);
        osInvalICache(arg1, 4);
        rmonbrk_bss_0080.unk0 = arg1;
    }
}

void ClearTempBreakpoint(void) {
    s32 sp1C;

    if (rmonbrk_bss_0000[0].unk0 != NULL) {
        sp1C = *rmonbrk_bss_0000[0].unk0;
        if ((sp1C & 0xFC00003F) == 0xD) {
            *rmonbrk_bss_0000[0].unk0 = rmonbrk_bss_0000[0].unk4;
            osWritebackDCache((void*)rmonbrk_bss_0000[0].unk0, 4);
            osInvalICache((void*)rmonbrk_bss_0000[0].unk0, 4);
        }
        rmonbrk_bss_0000[0].unk0 = NULL;
    }
    if (rmonbrk_bss_0080.unk0 != NULL) {
        sp1C = *rmonbrk_bss_0080.unk0;
        if ((sp1C & 0xFC00003F) == 0xD) {
            *rmonbrk_bss_0080.unk0 = rmonbrk_bss_0080.unk4;
            osWritebackDCache((void*)rmonbrk_bss_0080.unk0, 4);
            osInvalICache((void*)rmonbrk_bss_0080.unk0, 4);
        }
        rmonbrk_bss_0080.unk0 = NULL;
    }
}

int __rmonSetBreak(KKHeader* req) {
    register KKSetBkptRequest* copy_arg0 = (KKSetBkptRequest*)req;
    // Dont know if using register keyword is right here, but it matches
    register struct UnkRmonClearBreakStruct *var_s1; // Seems to be a refernece to array address
    register struct UnkRmonClearBreakStruct *var_s2; // Maybe reference to element one or 2
    register struct UnkRmonClearBreakStruct *var_s3; // Maybe reference to the last element, or size
    KKBkptEvent event;
    
    if (req->method == 1) {
        var_s1 = &rmonbrk_bss_0088[0];
        var_s2 = &rmonbrk_bss_0088[1];
        var_s3 = &rmonbrk_bss_0088[16];
    } else {
        var_s1 = &rmonbrk_bss_0000[0];
        var_s2 = &rmonbrk_bss_0000[1];
        var_s3 = &rmonbrk_bss_0000[16];
    }

    // My gut tells me this is a for loop, but cant get it to match as a for loop. 
    // Maybe uses one of the five seemingly unused stack variables
    // The loop looks for the array element that is equal to 0 and equal to arg0->addr
    while ((u32) var_s2 < (u32) var_s3) {// Would make sense if this means: if current element is not the last, or within bounds of array
        // Use commented code below if addr is a non pointer
        // if ((*var_s2 != 0) && (*(u32*)var_s2 != copy_arg0->addr)) {

        // Use code below if addr is a pointer
        if ((var_s2->unk0 == 0) || ((u32)var_s2->unk0 == copy_arg0->addr)) { 
            break;
        }
        var_s2 += 1;
    }
    
    if (var_s2 == var_s3) {
        return -0xA;
    }
    
    if (var_s2->unk0 == NULL) {
        if (req->method == 1) {
            var_s2->unk4 = __rmonReadWordAt((u32*)copy_arg0->addr);
            __rmonWriteWordTo((u32*)copy_arg0->addr, (((((s32) ((u32)var_s2 - (u32)var_s1) >> 3) + 0x10) & 0xFFFFF) << 6) | 0xD);
        } else {
            var_s2->unk4 = *(s32*)(copy_arg0->addr);
            *(s32*)copy_arg0->addr = ((((((s32) ((u32)var_s2 - (u32)var_s1) >> 3) + 0x10) & 0xFFFFF) << 6) | 0xD);
            
            osWritebackDCache((void*)copy_arg0->addr, 4);
            osInvalICache((void*)copy_arg0->addr, 4);
        }
        var_s2->unk0 = (u32*)copy_arg0->addr;
    }
    
    event.header.code = copy_arg0->header.code;
    event.header.error = 0;
    event.object = copy_arg0->object;
    event.bp = (s32)((u32)var_s2 - (u32)var_s1) >> 0x3;
    event.instruction = var_s2->unk4;
    __rmonSendReply((KKHeader * const)&event, 0x18, 1);
    return 0;
}

int __rmonListBreak(KKHeader* req) {
    return -1;
}

int __rmonClearBreak(KKHeader* req) {
    register KKClrBkptRequest* copy_arg0;
    register struct UnkRmonClearBreakStruct* var_s1;
    KKBkptEvent event;
    s32 sp24;

    copy_arg0 = (KKClrBkptRequest*)req;

    if ((s32)copy_arg0->bp >= 16) {
        return -2;
    }
    if (req->method == 1) {
        var_s1 = &rmonbrk_bss_0088[copy_arg0->bp];

        if (var_s1->unk0 == NULL) {
            return -2;
        }
        sp24 = __rmonReadWordAt(var_s1->unk0);
        if ((sp24 & 0xFC00003F) == 0xD) {
            __rmonWriteWordTo(var_s1->unk0, var_s1->unk4);
        }

    }else{
        var_s1 = &rmonbrk_bss_0000[copy_arg0->bp];
        if (var_s1->unk0 == NULL) {
            return -2;
        }
    
        if (((sp24 = *var_s1->unk0) & 0xFC00003F) == 0xD) {
            *var_s1->unk0 = var_s1->unk4;
            osWritebackDCache((void*)var_s1->unk0, 4);
            osInvalICache((void*)var_s1->unk0, 4);
        }
    }
    

    var_s1->unk0 = NULL;
    
    event.header.code = copy_arg0->header.code;
    event.header.error = 0;
    event.object = copy_arg0->object;
    event.bp = copy_arg0->bp;
    __rmonSendReply((KKHeader * const)&event, 0x18, 1);
    return 0;
}

u32 __rmonGetBranchTarget(int method, int thread, char* addr) {
    s32 inst;

    if (method == 1) {
        inst = __rmonReadWordAt((u32*)addr);
    } else {
        inst = *(s32*)addr;
    }

    switch ((inst >> 26) & 0x3f) {
        case 0:
            if ( ((inst >> 5) & 0x7fff) == 0 && (inst & 0x3f) == 8 ) {
                return __rmonGetRegisterContents(method, thread, (inst >> 21) & 0x1f);
            }
            if ( ((inst >> 16) & 0x1f) == 0 && (inst & 0x7ff) == 9 ) {
                return __rmonGetRegisterContents(method, thread, (inst >> 21) & 0x1f);
            }
            break;
        case 1:
            switch ((inst >> 16) & 0x1f) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 0x10:
                case 0x11:
                case 0x12:
                case 0x13:
                    return (u32)(((inst << 16) >> 14) + addr + 4);
            }
            break;
        case 2:
        case 3:
            return (((u32)(inst << 6)) >> 4) + ((((s32)addr + 4) >> 28) << 28);
        case 4:
        case 5:
        case 0x14:
        case 0x15:
            return (u32)(((inst << 16) >> 14) + addr + 4);
        case 6:
        case 7:
        case 0x16:
        case 0x17:
            if (((inst >> 16) & 0x1f) == 0) {
                return (u32)(((inst << 16) >> 14) + addr + 4);
            }
            break;
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            if (((inst >> 21) & 0x1f) == 8) {
                switch ((inst >> 16) & 0x1f) {
                    case 0:
                    case 2:
                    case 1:
                    case 3:
                        return (u32)(((inst << 16) >> 14) + addr + 4);
                }
            }
            break;
    }

    return (u32)-1;
}

int IsJump(u32 arg0) {
    switch ((arg0 >> 0x1A) & 0x3F) {                              /* irregular */
        case 0:
            if (!((arg0 >> 5) & 0x7FFF) && ((arg0 & 0x3F) == 8)) {
                return 1;
            }
            if (!((arg0 >> 0x10) & 0x1F) && ((arg0 & 0x7FF) == 9)) {
                return 1;
            }
        break;
        case 2:
        case 3:
            return 1;
        }
        
    return 0;
}

int __rmonSetSingleStep(int thread, u32* instptr) {
    u32 sp1C;

    sp1C = __rmonGetBranchTarget(0, thread, (char*)instptr);
    if (sp1C & 3) {
        /* Changed the + 4 that was originally here to + 1, compiler seems to convert appropratly (pretty sure it means 1 word or byte) */
        SetTempBreakpoint(instptr + 1, NULL);
        
    }else if ((instptr+0) == (void*)sp1C) { /* had to add the + 0 to arg1 to get function to match */
        return 0;
    }
    /* Changed the + 8 to + 2, compiler seems to convert appropratly (pretty sure it means 2 words or bytes) */
    else if ((IsJump(*instptr) != 0) || ((instptr + 2) == (void*)sp1C)) { 
        SetTempBreakpoint((u32*)sp1C, NULL);
    } else {
        /* Same here*/
        SetTempBreakpoint((u32*)sp1C, instptr + 2);
    }
    
    
    return 1;
}

void __rmonGetExceptionStatus(KKStatusEvent* reply) {
    reply->status.flags = 1;
    reply->status.why = 2;
    reply->status.what = 0;
    reply->status.rv = 0;
    reply->status.info.major = 2;
    reply->status.info.minor = 4;
    reply->header.code = 4;
    reply->header.error = 0;
    reply->header.length = 0x4C;
}

void rmonSendBreakMessage(s32 arg0, s32 arg1) {
    KKStatusEvent event;
    
    /* Hey future me, the inline conditional expression (ternary operator) was how you solved this problem with saved registers*/
    __rmonGetThreadStatus(0, (arg0 ? arg0 : 0x3EB), &event);
    __rmonGetExceptionStatus(&event);
    if (arg1 == 0xF) {
        event.status.info.major = 1;
        event.status.info.minor = 2;
    }
    if (arg1 < 0x10) {
        arg1 = 0;
    } else {
        arg1 -= 0x10;
    }
    if (arg1 != 0) {
        event.status.instr = 0xD;
    }
    __rmonSendReply((KKHeader *const)&event, 0x4C, 2);
}

void __rmonHitBreak(void) {
    ClearTempBreakpoint();
    __rmonStopUserThreads(0);
    rmonFindFaultedThreads();
}

void __rmonHitSpBreak(void) {
    KKStatusEvent reply;
    
    __rmonWriteWordTo((u32*)0x04080000, __rmonReadWordAt((u32*)0x04080000) - 4);
    __rmonGetThreadStatus(1, 0x3E8, &reply);
    __rmonGetExceptionStatus(&reply); /* This function uses a struct im sure*/
    __rmonSendReply((KKHeader *const)&reply, 0x4C, 2);
    __rmonRcpAtBreak = 1;
}

void __rmonHitCpuFault(void) {
    __rmonMaskIdleThreadInts();
    __rmonStopUserThreads(0);
    rmonFindFaultedThreads();
}

void rmonFindFaultedThreads() {
    register OSThread * var_s0 = __osGetActiveQueue();
    s32 sp20;

    while (var_s0->priority != -1){
        if ((var_s0->priority > 0) && (var_s0->priority < 0x80)) {
            if (var_s0->flags & 1) {
                sp20 = *(s32*)(var_s0->context.pc);
                if ((sp20 & 0xFC00003F) == 0xD) {
                    rmonSendBreakMessage(var_s0->id, sp20 >> 6);
                } else {
                    rmonSendBreakMessage(var_s0->id, 0);
                }
            }
            if (var_s0->flags & 2) {
                __rmonSendFault(var_s0);
                rmonSendBreakMessage(var_s0->id, 0xF);
            }
        }
        var_s0 = var_s0->tlnext;
    } 
}
