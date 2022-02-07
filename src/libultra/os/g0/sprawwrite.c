#include <PR/os_internal.h>
#include <PR/rcp.h>
#include "libultra/os/os_int.h"

s32 __osSpRawWriteIo(u32 devAddr, u32 data)
{
    if (__osSpDeviceBusy())
        return -1;
    IO_WRITE(devAddr, data);
    return 0;
}
