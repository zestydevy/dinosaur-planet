// @DECOMP_OPT_FLAGS=-O1
#include "libultra/rmon/dbgproto.h"
#include "libultra/rmon/rmonint.h"
#include "PR/os_internal.h"
#include "PR/ramrom.h"
#include "PR/sptask.h"
#include "PR/rdb.h"
#include "PR/rmon.h"
#include "PRinternal/macros.h"
#include "bss.h"

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

BSS_STATIC OSMesgQueue IOmq;
BSS_STATIC OSMesg IOmsgs[1];

BSS_GLOBAL void* __osRdb_DbgRead_Buf;
BSS_GLOBAL u8 rmonRdbReadBuf[RMON_DBG_BUF_SIZE] ALIGNED(0x10);

void __rmonSendFault(OSThread* thread) {
    volatile float f UNUSED;
    u8* tPtr;
    u32 sent = 0;

    /* touch fpu to ensure registers are saved to the context structure */
    f = 0.0f;

    tPtr = (u8*)thread;
    while (sent < sizeof(OSThread_Original)) {
        sent += __osRdbSend(tPtr + sent, sizeof(OSThread_Original) - sent, RDB_TYPE_GtoH_FAULT);
    }
}

void __rmonIOflush(void) {
    int sent = 0;
    char tstr[4];

    while (sent <= 0) {
        sent += __osRdbSend(tstr, 1, RDB_TYPE_GtoH_DEBUG_DONE);
    }
}

void __rmonIOputw(u32 word) {
    int sent = 0;
    char* cPtr = (char*)&word;

    while (sent < 4) {
        sent += __osRdbSend(cPtr + sent, sizeof(word) - sent, RDB_TYPE_GtoH_DEBUG);
    }
}

void __rmonIOhandler(void) {
    int sent;
    char tstr[4];

    osCreateMesgQueue(&IOmq, &IOmsgs[0], 1);
    osSetEventMesg(OS_EVENT_RDB_DBG_DONE, &IOmq, NULL);
    __osRdb_DbgRead_Buf = rmonRdbReadBuf;

    while (TRUE) {
        osRecvMesg(&IOmq, NULL, OS_MESG_BLOCK);

        __rmonExecute((KKHeader*)&rmonRdbReadBuf);
        __osRdb_DbgRead_Buf = rmonRdbReadBuf;

        sent = 0;
        while (sent <= 0) {
            sent += __osRdbSend(tstr, 1, RDB_TYPE_GtoH_DEBUG_READY);
        }
    }
}
