#include <libultra/io/controller.h>

s32 __osPfsSelectBank(OSPfs *pfs, u8 bank) {
    u8 data[BLOCKSIZE];
    int i;
    s32 ret;

    for (i = 0; i < BLOCKSIZE; i++) {
        data[i] = bank;
    }

    ret = __osContRamWrite(pfs->queue, pfs->channel, 1024, &data, FALSE);
    
    if (ret == 0) {
        pfs->activebank = bank;
    }

    return ret;
}
