#include <PR/os_internal.h>
#include "libultra/io/viint.h"

void osViSetEvent(OSMesgQueue *mq, OSMesg m, u32 retraceCount)
{
    register u32 saveMask;
    saveMask = __osDisableInt();
    __osViNext->msgq = mq;
    __osViNext->msg = m;
    __osViNext->retraceCount = retraceCount;
    __osRestoreInt(saveMask);
}
