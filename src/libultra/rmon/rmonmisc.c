// @DECOMP_OPT_FLAGS=-O1
#include "libultra/rmon/dbgproto.h"
#include "libultra/rmon/rmonint.h"
#include "PR/os_internal.h"
#include "PR/sptask.h"
#include "PRinternal/macros.h"

int __rmonSetFault(KKHeader* req) {
    KKFaultRequest* request = (KKFaultRequest*)req;
    KKObjectEvent reply;

    STUBBED_PRINTF(("SetFault\n"));

    reply.header.code = request->header.code;
    reply.header.error = TV_ERROR_NO_ERROR;
    reply.object = request->tid;

    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);
    return TV_ERROR_NO_ERROR;
}

OSMesgQueue __rmonMQ ALIGNED(0x8);
// @bug? It appears that OSThread has an extra 128 bytes in the rest of the codebase but here it seems
// like rmon was compiled before this change... doesn't seem like it will necessarily break anything tho
#ifndef AVOID_UB
static OSThread_Old rmonIOThread ALIGNED(0x8);
#else
static OSThread rmonIOThread ALIGNED(0x8);
#endif
static OSMesg rmonMsgs[8] ALIGNED(0x8);
static STACK(rmonIOStack, 0x4000) ALIGNED(0x10);
static OSMesg rmonPiMsgs[8] ALIGNED(0x8);
static OSMesgQueue rmonPiMQ ALIGNED(0x8);

void __rmonInit(void) {
    osCreateMesgQueue(&__rmonMQ, rmonMsgs, ARRLEN(rmonMsgs));
    osSetEventMesg(OS_EVENT_CPU_BREAK, &__rmonMQ, (OSMesg)RMON_MESG_CPU_BREAK);
    osSetEventMesg(OS_EVENT_SP_BREAK, &__rmonMQ, (OSMesg)RMON_MESG_SP_BREAK);
    osSetEventMesg(OS_EVENT_FAULT, &__rmonMQ, (OSMesg)RMON_MESG_FAULT);
    osSetEventMesg(OS_EVENT_THREADSTATUS, &__rmonMQ, NULL);
    osCreateThread((OSThread*)&rmonIOThread, 0, (void (*)(void*))__rmonIOhandler, NULL, STACK_START(rmonIOStack),
                   OS_PRIORITY_MAX);
    osCreatePiManager(OS_PRIORITY_PIMGR, &rmonPiMQ, rmonPiMsgs, ARRLEN(rmonPiMsgs));
    osStartThread((OSThread*)&rmonIOThread);
}

void __rmonPanic(void) {
    STUBBED_PRINTF(("PANIC!!\n"));

    for (;;) {
        ;
    }
}

int __rmonSetComm(KKHeader* req) {
    KKObjectEvent reply;

    STUBBED_PRINTF(("SetComm\n"));

    reply.header.code = req->code;
    reply.object = 0;
    reply.header.error = TV_ERROR_NO_ERROR;

    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);

    return TV_ERROR_NO_ERROR;
}
