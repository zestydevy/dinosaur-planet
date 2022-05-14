#include "common.h"

s32 osEPiWriteIo(OSPiHandle *pihandle, u32 devAddr, u32 data) {
    s32 ret;

    __osPiGetAccess();
    ret = osEPiRawWriteIo(pihandle, devAddr, data);
    __osPiRelAccess();
    
    return ret;
}
