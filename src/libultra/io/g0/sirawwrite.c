#include <PR/os_internal.h>
#include <PR/rcp.h>
#include "libultra/io/siint.h"

s32 __osSiRawWriteIo(u32 devAddr, u32 data)
{
    if (__osSiDeviceBusy())
        return -1;
    IO_WRITE(devAddr, data);
    return 0;
}
