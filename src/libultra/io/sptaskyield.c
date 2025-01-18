// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include <PR/rcp.h>

void osSpTaskYield(void)
{
    __osSpSetStatus(SP_SET_YIELD);
}
