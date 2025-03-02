// @DECOMP_OPT_FLAGS=-O2 -g0
#include <PR/os_internal.h>
#include <libultra/io/controller.h>

OSPifRam gMotorData[MAXCONTROLLERS];

#define READFORMAT(ptr) ((__OSContRamReadFormat*)(ptr))

s32 __osMotorAccess(OSPfs *pfs, s32 flag) {
    s32 i;
    s32 ret;
    u8 *ptr;

    ptr = (u8 *)&gMotorData[pfs->channel];

    if (!(pfs->status & PFS_MOTOR_INITIALIZED)) {
        return 5;
    }

    __osSiGetAccess();

    gMotorData[pfs->channel].pifstatus = CONT_CMD_EXE;
    ptr += pfs->channel;

    for (i = 0; i < BLOCKSIZE; i++) {
        READFORMAT(ptr)->data[i] = flag;
    }
    
    __osContLastCmd = CONT_CMD_END;
    
    __osSiRawStartDma(OS_WRITE, &gMotorData[pfs->channel]);
    osRecvMesg(pfs->queue, (OSMesg *)NULL, OS_MESG_BLOCK);

    __osSiRawStartDma(OS_READ, &gMotorData[pfs->channel]);
    osRecvMesg(pfs->queue, (OSMesg *)NULL, OS_MESG_BLOCK);
    
    ret = READFORMAT(ptr)->rxsize & CHNL_ERR_MASK;
    
    if (ret == 0) {
        if (flag == 0) {
            if (READFORMAT(ptr)->datacrc != 0) {
                ret = PFS_ERR_CONTRFAIL;
            }
        } else if (READFORMAT(ptr)->datacrc != 0xEB) {
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
    ramreadformat.txsize = CONT_CMD_WRITE_PAK_TX;
    ramreadformat.rxsize = CONT_CMD_WRITE_PAK_RX;
    ramreadformat.cmd = CONT_CMD_WRITE_PAK;
    ramreadformat.addrh = CONT_BLOCK_RUMBLE >> 3;
    ramreadformat.addrl = (u8)(__osContAddressCrc(CONT_BLOCK_RUMBLE) | (CONT_BLOCK_RUMBLE << 5));

    if (channel != 0)
    {
        for (i = 0; i < channel; i++)
        {
            *ptr++ = CONT_CMD_REQUEST_STATUS;
        }
    }

    *READFORMAT(ptr) = ramreadformat;
    
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
