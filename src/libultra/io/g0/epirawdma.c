#include <PR/os_internal.h>
#include <PR/rcp.h>
#include <libultra/io/piint.h>

s32 osEPiRawStartDma(OSPiHandle *pihandle, s32 direction, u32 devAddr, void *dramAddr, u32 size)
{
    u32 stat;
    u8 domain;
    OSPiHandle *cHandle;
    
    WAIT_ON_IOBUSY(stat)

    domain = pihandle->domain;

    if (__osCurrentHandle[domain]->type != pihandle->type) {
        cHandle = __osCurrentHandle[domain];
        
        if (domain == PI_DOMAIN1) {
            UPDATE_REG(PI_BSD_DOM1_LAT_REG, latency)
            UPDATE_REG(PI_BSD_DOM1_PGS_REG, pageSize)
            UPDATE_REG(PI_BSD_DOM1_RLS_REG, relDuration)
            UPDATE_REG(PI_BSD_DOM1_PWD_REG, pulse)
        } else {
            UPDATE_REG(PI_BSD_DOM2_LAT_REG, latency)
            UPDATE_REG(PI_BSD_DOM2_PGS_REG, pageSize)
            UPDATE_REG(PI_BSD_DOM2_RLS_REG, relDuration)
            UPDATE_REG(PI_BSD_DOM2_PWD_REG, pulse)
        }

        cHandle->type = pihandle->type;
        cHandle->latency = pihandle->latency;
        cHandle->pageSize = pihandle->pageSize;
        cHandle->relDuration = pihandle->relDuration;
        cHandle->pulse = pihandle->pulse;
    }
    
    IO_WRITE(PI_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));
    IO_WRITE(PI_CART_ADDR_REG, K1_TO_PHYS(pihandle->baseAddress | devAddr));

    switch (direction)
    {
        case OS_READ:
            IO_WRITE(PI_WR_LEN_REG, size - 1);
            break;
        case OS_WRITE:
            IO_WRITE(PI_RD_LEN_REG, size - 1);
            break;
        default:
            return -1;
    }

    return 0;
}
