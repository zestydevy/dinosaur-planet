// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include "libultra/io/piint.h"

OSMesgQueue *osPiGetCmdQueue(void)
{
    if (!__osPiDevMgr.active)
        return NULL;
    return __osPiDevMgr.cmdQueue;
}
