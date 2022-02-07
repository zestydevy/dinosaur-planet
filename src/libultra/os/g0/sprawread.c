#include <PR/os_internal.h>
#include <PR/rcp.h>
#include "libultra/io/siint.h"

s32 __osSpRawReadIo(u32 devAddr, u32 *data)
{
    if (__osSiDeviceBusy()) // originally __osSpDeviceBusy
        return -1;
    *data = IO_READ(devAddr);
    return 0;
}
