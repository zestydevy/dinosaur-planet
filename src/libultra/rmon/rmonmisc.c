// @DECOMP_OPT_FLAGS=-O1
#include "libultra/rmon/rmonint.h"

typedef struct OSThread_Original_s {
	struct OSThread_s	*next;		/* run/mesg queue link */
	OSPri			priority;	/* run/mesg queue priority */
	struct OSThread_s	**queue;	/* queue thread is on */
	struct OSThread_s	*tlnext;	/* all threads queue link */
	u16			state;		/* OS_STATE_* */
	u16			flags;		/* flags for rmon */
	OSId			id;		/* id for debugging */
	int			fp;		/* thread has used fp unit */
	__OSThreadContext	context;	/* register/interrupt mask */
} OSThread_Original;

// bug? It appears that OSThread has an extra 128 bytes in the rest of the codebase but here it seems
// like rmon was compiled before this change... doesn't seem like it will necessarily break anything tho
extern OSThread_Original rmonmisc_bss_0018;
extern OSMesg rmonmisc_bss_01C8[8];
extern u8 rmonmisc_bss_01E8[0x4000];
extern OSMesg rmonmisc_bss_41E8[8];
extern OSMesgQueue rmonmisc_bss_4208;

static const char str_800a2b40[] = "SetFault\n";
static const char str_800a2b4c[] = "PANIC!!\n";
static const char str_800a2b58[] = "SetComm\n";

int __rmonSetFault(KKHeader* req) {
    KKFaultRequest *request = (KKFaultRequest*)req;
    KKObjectEvent event;

    event.header.code = request->header.code;
    event.header.error = 0;
    event.object = request->tid;
    __rmonSendReply((KKHeader *const)&event, 0x10, 1);

    return 0;
}

void __rmonInit() {
    osCreateMesgQueue(&__rmonMQ, &rmonmisc_bss_01C8[0], 8);
    osSetEventMesg(OS_EVENT_CPU_BREAK, &__rmonMQ, (OSMesg)RMON_MESG_CPU_BREAK);
    osSetEventMesg(OS_EVENT_SP_BREAK, &__rmonMQ, (OSMesg)RMON_MESG_SP_BREAK);
    osSetEventMesg(OS_EVENT_FAULT, &__rmonMQ, (OSMesg)RMON_MESG_FAULT);
    osSetEventMesg(OS_EVENT_THREADSTATUS, &__rmonMQ, (OSMesg)NULL);
    osCreateThread((OSThread*)&rmonmisc_bss_0018, 0, (void (*)(void *))__rmonIOhandler, NULL, &rmonmisc_bss_01E8[0x4000], OS_PRIORITY_MAX);
    osCreatePiManager(OS_PRIORITY_PIMGR, &rmonmisc_bss_4208, &rmonmisc_bss_41E8[0], 8);
    osStartThread((OSThread*)&rmonmisc_bss_0018);
}

void __rmonPanic() {
    while (1) { }
}

int __rmonSetComm(KKHeader* req) {
    KKObjectEvent event;

    event.header.code = req->code;
    event.object = 0;
    event.header.error = 0;

    __rmonSendReply((KKHeader *const)&event, 0x10, 1);

    return 0;
}
