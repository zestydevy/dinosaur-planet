// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include <PR/R4300.h>

void __osSetGlobalIntMask(OSHWIntr mask)
{
    register u32 saveMask = __osDisableInt();
    __OSGlobalIntMask |= mask;
    __osRestoreInt(saveMask);
}
