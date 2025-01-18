// @DECOMP_OPT_FLAGS=-O2 -g0
#include <PR/os_internal.h>
#include <PR/rcp.h>
#include "libultra/io/siint.h"

s32 __osSiRawReadIo(u32 devAddr, u32 *data)
{
    if (__osSiDeviceBusy())
        return -1;
    *data = IO_READ(devAddr);
    return 0;
}
