#include <PR/os_internal.h>
#include <libultra/io/controller.h>

OSPifRam gMotorData[MAXCONTROLLERS];

s32 __osMotorAccess(OSPfs *pfs, s32 param2) {
    s32 i;
    s32 ret;
    __OSContRamReadFormat *ptr;

    ptr = (__OSContRamReadFormat *)&gMotorData[pfs->channel];

    if ((pfs->status & 8) == 0) {
        return PFS_ERR_INVALID;
    }

    __osSiGetAccess();

    gMotorData[pfs->channel].pifstatus = CONT_FORMAT;
    
    ptr = (__OSContRamReadFormat *)((u8 *)ptr + pfs->channel);

    for (i = 0; i < BLOCKSIZE; i++) {
        ptr->data[i] = (u8)param2;
    }
    
    __osContLastCmd = CONT_ETC;
    
    __osSiRawStartDma(OS_WRITE, &gMotorData[pfs->channel]);
    osRecvMesg(pfs->queue, (OSMesg *)NULL, OS_MESG_BLOCK);

    __osSiRawStartDma(OS_READ, &gMotorData[pfs->channel]);
    osRecvMesg(pfs->queue, (OSMesg *)NULL, OS_MESG_BLOCK);
    
    ret = (s32)(ptr->rxsize & CON_ERR_MASK);
    
    if (ret == 0) {
        if (param2 == 0) {
            if (ptr->datacrc != 0) {
                ret = PFS_ERR_CONTRFAIL;
            }
        } else if (ptr->datacrc != 0xEB) {
            ret = PFS_ERR_CONTRFAIL;
        }
    }
    
    __osSiRelAccess();
    
    return ret;
}

void __osMakeMotorData(s32 channel, OSPifRam *mdata) {
    u8 *ptr;
    __OSContRamReadFormat ramreadformat;
    int i;

    ptr = (u8 *)mdata->ramarray;
    
    ramreadformat.dummy = CONT_CMD_NOP;
    ramreadformat.txsize = CONT_CMD_WRITE_MEMPACK_TX;
    ramreadformat.rxsize = CONT_CMD_WRITE_MEMPACK_RX;
    ramreadformat.cmd = CONT_CMD_WRITE_MEMPACK;
    ramreadformat.address_hi = 0xC0;
    ramreadformat.address_lo = 0xC000 | __osContAddressCrc(0x600);

    if (channel != 0)
    {
        for (i = 0; i < channel; i++)
        {
            *ptr++ = 0;
        }
    }

    *(__OSContRamReadFormat *)ptr = ramreadformat;
    
    ptr += sizeof(__OSContRamReadFormat);
    ptr[0] = CONT_CMD_END;
}

s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel) {
    s32 ret;
    u8 data[BLOCKSIZE];

    pfs->queue = mq;
    pfs->channel = channel;
    pfs->activebank = 0xFF;
    pfs->status = 0;

    ret = __osPfsSelectBank(pfs, 0xfe);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = __osPfsSelectBank(pfs, 0x80);
    }
    if (ret != 0) {
        return ret;
    }

    ret = __osContRamRead(mq, channel, 1024, data);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }
    if (ret != 0) {
        return ret;
    }
    if (data[BLOCKSIZE - 1] == 0xFE) {
        return PFS_ERR_DEVICE;
    }

    ret = __osPfsSelectBank(pfs, 0x80);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }
    if (ret != 0) {
        return ret;
    }

    ret = __osContRamRead(mq, channel, 1024, data);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }
    if (ret != 0) {
        return ret;
    }
    if (data[BLOCKSIZE - 1] != 0x80) {
        return PFS_ERR_DEVICE;
    }

    if ((pfs->status & 8) == 0) {
        __osMakeMotorData(channel, &gMotorData[channel]);
    }

    pfs->status = 8;

    return 0;
}
