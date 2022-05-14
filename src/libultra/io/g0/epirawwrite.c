#include <libultra/io/piint.h>

s32 osEPiRawWriteIo(OSPiHandle *pihandle, u32 devAddr, u32 data) {
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
    
    *((u32 *)PHYS_TO_K1(pihandle->baseAddress | devAddr)) = data;

    return 0;
}
