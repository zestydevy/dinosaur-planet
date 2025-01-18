// @DECOMP_OPT_FLAGS=-O2 -g0
#include <PR/os_internal.h>
#include <PR/rcp.h>
#include "libultra/os/osint.h"

s32 __osSpRawReadIo(u32 devAddr, u32 *data)
{
    if (__osSpDeviceBusy())
        return -1;
    *data = IO_READ(devAddr);
    return 0;
}
