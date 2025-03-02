// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include "libultra/os/osint.h"

extern s32 __osShutdown;
extern s32 __osPreNMI;

__OSEventState __osEventStateTab[OS_NUM_EVENTS];

void osSetEventMesg(OSEvent event, OSMesgQueue *mq, OSMesg msg)
{
	register u32 saveMask = __osDisableInt();
	__OSEventState *es;

	es = &__osEventStateTab[event];
	es->messageQueue = mq;
	es->message = msg;

	if (event == OS_EVENT_PRENMI) {
		if (__osShutdown && !__osPreNMI) {
			osSendMesg(mq, msg, OS_MESG_NOBLOCK);
		}

		__osPreNMI = TRUE;
	}

	__osRestoreInt(saveMask);
}
