// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include "libultra/io/viint.h"

void *osViGetCurrentFramebuffer(void)
{
    register u32 saveMask;
    void *framep;
    saveMask = __osDisableInt();
    framep = __osViCurr->framep;
    __osRestoreInt(saveMask);
    return framep;
}
