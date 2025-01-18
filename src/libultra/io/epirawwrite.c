// @DECOMP_OPT_FLAGS=-O2 -g0
#include <libultra/io/piint.h>

s32 osEPiRawWriteIo(OSPiHandle *pihandle, u32 devAddr, u32 data) {
    u32 stat;
    u8 domain;
    
    EPI_SYNC(pihandle, stat, domain)
    
    *((u32 *)PHYS_TO_K1(pihandle->baseAddress | devAddr)) = data;

    return 0;
}
