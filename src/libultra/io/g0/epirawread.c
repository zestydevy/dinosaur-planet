#include <libultra/io/piint.h>

s32 osEPiRawReadIo(OSPiHandle *pihandle, u32 devAddr, u32 *data) {
    u32 stat;
    u8 domain;
    
    EPI_SYNC(pihandle, stat, domain)
    
    *data = *((u32 *)PHYS_TO_K1(pihandle->baseAddress | devAddr));

    return 0;
}
