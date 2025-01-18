// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include <PR/rcp.h>

u32 __osSpGetStatus()
{
    return IO_READ(SP_STATUS_REG);
}
