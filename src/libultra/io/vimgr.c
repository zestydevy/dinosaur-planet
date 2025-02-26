#include <PR/os_internal.h>
#include <PR/rcp.h>
#include "libultra/io/viint.h"
#include "libultra/os/osint.h"

s32 D_800930B0 = 0;
OSDevMgr __osViDevMgr = {0};
s32 __additional_scanline = 0;

// all these were static
OSThread viThread;
unsigned char viThreadStack[OS_VIM_STACKSIZE];
OSMesgQueue viEventQueue;
OSMesg viEventBuf[5];
OSIoMesg viRetraceMsg;
OSIoMesg viCounterMsg;

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


#pragma GLOBAL_ASM("asm/nonmatchings/libultra/io/vimgr/viMgrMain.s")
