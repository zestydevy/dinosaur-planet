// @DECOMP_OPT_FLAGS=-O2 -g0
#include "PR/os_internal.h"
#include "PR/rdb.h"
#include "PRinternal/macros.h"
#include "libultra/io/piint.h"

// @bug: This file was not recompiled with the modified OSThread definition!
#ifndef AVOID_UB
static OSThread_Old piThread ALIGNED(0x8);
#else
static OSThread piThread ALIGNED(0x8);
#endif
static STACK(piThreadStack, OS_PIM_STACKSIZE) ALIGNED(0x10);

#ifndef AVOID_UB
static OSThread_Old piRamRomThread ALIGNED(0x8);
#else
static OSThread piRamRomThread ALIGNED(0x8);
#endif
static STACK(piRamRomThreadStack, 0x400) ALIGNED(0x10);
static OSMesgQueue gPiReqRamRomQueue ALIGNED(0x8);
static OSMesg gPiReqRamRomQueueBuffer[1];
static OSMesgQueue gPiFreeRamRomQueue ALIGNED(0x8);
static OSMesg gPiFreeRamRomQueueBuffer[1];
static void pi_ramrom_thread_main(void *arg);

static OSMesgQueue piEventQueue;
static OSMesg piEventBuf[1];

OSDevMgr __osPiDevMgr = {0};
OSPiHandle *__osPiTable = NULL;
OSPiHandle __Dom1SpeedParam ALIGNED(0x8);
OSPiHandle __Dom2SpeedParam ALIGNED(0x8);
OSPiHandle *__osCurrentHandle[2] ALIGNED(0x8) = {&__Dom1SpeedParam, &__Dom2SpeedParam};

void osCreatePiManager(OSPri pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt)
{
	u32 savedMask;
	OSPri oldPri;
	OSPri myPri;
	if (!__osPiDevMgr.active)
	{
		osCreateMesgQueue(cmdQ, cmdBuf, cmdMsgCnt);
		osCreateMesgQueue(&piEventQueue, (OSMesg*)&piEventBuf, 1);
		if (!__osPiAccessQueueEnabled)
			__osPiCreateAccessQueue();
		osSetEventMesg(OS_EVENT_PI, &piEventQueue, (OSMesg)0x22222222);
		oldPri = -1;
		myPri = osGetThreadPri(NULL);
		if (myPri < pri)
		{
			oldPri = myPri;
			osSetThreadPri(NULL, pri);
		}
		savedMask = __osDisableInt();
		__osPiDevMgr.active = 1;
		__osPiDevMgr.thread = &piThread;
		__osPiDevMgr.cmdQueue = cmdQ;
		__osPiDevMgr.evtQueue = &piEventQueue;
		__osPiDevMgr.acsQueue = &__osPiAccessQueue;
		__osPiDevMgr.dma = osPiRawStartDma;
		__osPiDevMgr.edma = osEPiRawStartDma;
		osCreateThread(&piThread, 0, __osDevMgrMain, &__osPiDevMgr, STACK_START(piThreadStack), pri);
		osStartThread(&piThread);
        osCreateThread(&piRamRomThread, 0, pi_ramrom_thread_main, NULL, STACK_START(piRamRomThreadStack), pri - 1);
		osStartThread(&piRamRomThread);
		__osRestoreInt(savedMask);
		if (oldPri != -1)
		{
			osSetThreadPri(NULL, oldPri);
		}
	}
}

static void pi_ramrom_thread_main(void *arg) {
    s32 sent;
    u8 buf[1];

    osCreateMesgQueue(&gPiReqRamRomQueue, gPiReqRamRomQueueBuffer, 1);
    osCreateMesgQueue(&gPiFreeRamRomQueue, gPiFreeRamRomQueueBuffer, 1);
    osSetEventMesg(OS_EVENT_RDB_REQ_RAMROM, &gPiReqRamRomQueue, NULL);
    osSetEventMesg(OS_EVENT_RDB_FREE_RAMROM, &gPiFreeRamRomQueue, NULL);

	while (1) {
		osRecvMesg(&gPiReqRamRomQueue, NULL, OS_MESG_BLOCK);
		__osPiGetAccess();

		sent = 0;
		while (sent == 0) {
			sent += __osRdbSend(buf, 1, RDB_TYPE_GtoH_RAMROM);
		}

		osRecvMesg(&gPiFreeRamRomQueue, NULL, OS_MESG_BLOCK);
		__osPiRelAccess();
    }
}
