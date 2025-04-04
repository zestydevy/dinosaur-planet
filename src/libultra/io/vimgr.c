#include <PR/os_internal.h>
#include <PR/rcp.h>
#include "libultra/io/viint.h"
#include "libultra/os/osint.h"
#include "bss.h"

OSViMode *gNewViMode = NULL;
OSDevMgr __osViDevMgr = {0};
s32 __additional_scanline = 0;

BSS_STATIC OSThread viThread;
BSS_STATIC unsigned char viThreadStack[OS_VIM_STACKSIZE];
BSS_STATIC OSMesgQueue viEventQueue;
BSS_STATIC OSMesg viEventBuf[5];
BSS_STATIC OSIoMesg viRetraceMsg;
BSS_STATIC OSIoMesg viCounterMsg;

void viMgrMain(void *arg);

void osCreateViManager(OSPri pri)
{
	u32 savedMask;
	OSPri oldPri;
	OSPri myPri;
	if (__osViDevMgr.active == 0)
	{
		__osTimerServicesInit();
        __additional_scanline = 0;
		osCreateMesgQueue(&viEventQueue, viEventBuf, 5);
		viRetraceMsg.hdr.type = OS_MESG_TYPE_VRETRACE;
		viRetraceMsg.hdr.pri = OS_MESG_PRI_NORMAL;
		viRetraceMsg.hdr.retQueue = NULL;
		viCounterMsg.hdr.type = OS_MESG_TYPE_COUNTER;
		viCounterMsg.hdr.pri = OS_MESG_PRI_NORMAL;
		viCounterMsg.hdr.retQueue = NULL;
		osSetEventMesg(OS_EVENT_VI, &viEventQueue, &viRetraceMsg);
		osSetEventMesg(OS_EVENT_COUNTER, &viEventQueue, &viCounterMsg);
		oldPri = -1;
		myPri = osGetThreadPri(NULL);
		if (myPri < pri)
		{
			oldPri = myPri;
			osSetThreadPri(NULL, pri);
		}
		savedMask = __osDisableInt();
		__osViDevMgr.active = 1;
		__osViDevMgr.thread = &viThread;
		__osViDevMgr.cmdQueue = &viEventQueue;
		__osViDevMgr.evtQueue = &viEventQueue;
		__osViDevMgr.acsQueue = NULL;
		__osViDevMgr.dma = NULL;
		__osViDevMgr.edma = NULL;
		osCreateThread(&viThread, 0, viMgrMain, &__osViDevMgr, &viThreadStack[OS_VIM_STACKSIZE], pri);
		__osViInit();
		osStartThread(&viThread);
		__osRestoreInt(savedMask);
		if (oldPri != -1)
		{
			osSetThreadPri(0, oldPri);
		}
	}
}

// pad to 0x800bce70
static u8 _bss_pad[0xEC40]; // TODO: remove bss padding

void viMgrMain(void* arg) {
    __OSViContext* vc;
    OSDevMgr* dm;
    OSIoMesg* mb;
    static u16 retrace;
    s32 first;
    u32 count;

    mb = NULL;
    first = 0;
    vc = __osViGetCurrentContext();
    retrace = vc->retraceCount;
    if (retrace == 0) {
        retrace = 1;
    }
    dm = (OSDevMgr*)arg;

	while (TRUE) {
		osRecvMesg(dm->evtQueue, (OSMesg)&mb, OS_MESG_BLOCK);
		switch (mb->hdr.type) {
			case OS_MESG_TYPE_VRETRACE:
				if (gNewViMode != NULL) {
					__osViNext->modep = gNewViMode;
					gNewViMode = NULL;
				}
		
				__osViSwapContext();
				retrace--;
		
				if (retrace == 0) {
					vc = __osViGetCurrentContext();
					if (vc->msgq != NULL) {
						osSendMesg(vc->msgq, vc->msg, OS_MESG_NOBLOCK);
					}
					retrace = vc->retraceCount;
				}
		
				__osViIntrCount++;
		
				if (first) {
					count = osGetCount();
					__osCurrentTime = count;
					first = 0;
				}
		
				count = __osBaseCounter;
				__osBaseCounter = osGetCount();
				count = __osBaseCounter - count;
				__osCurrentTime = __osCurrentTime + count;
				break;
			case OS_MESG_TYPE_COUNTER:
				__osTimerInterrupt();
				break;
			case OS_MESG_TYPE_SET_VI_MODE:
				// RARE smuggles a VI mode pointer in the retQueue field for this message type.
				// Could make it a union field, but the way this is passed is hacky to begin with...
				gNewViMode = (OSViMode*)mb->hdr.retQueue;
				break;
		}
	}
}
