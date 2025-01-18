// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include "libultra/os/osint.h"

void osYieldThread(void){
    register u32 saveMask = __osDisableInt();
    __osRunningThread->state = OS_STATE_RUNNABLE;
    __osEnqueueAndYield(&__osRunQueue);
    __osRestoreInt(saveMask);
}
