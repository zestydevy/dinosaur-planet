// @DECOMP_OPT_FLAGS=-O2 -g0
#include <PR/os_internal.h>
#include <PR/rcp.h>
#include <libultra/io/controller.h>
#include <libultra/io/siint.h>

#define READFORMAT(ptr) ((__OSContRamReadFormat*)(ptr))

extern int __osPfsLastChannel;

s32 __osContRamWrite(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force) {
    s32 ret;
    int i;
    u8 *ptr;
    s32 retry;
    u8 crc;

    // make 3 attempts at a write
    retry = 2;

    // skip if not forcing and address is [1,6] (protected region?)
    if ((force != TRUE) && (address < PFS_LABEL_AREA) && (address != 0)) {
        return 0;
    }

    // acquire lock for SI access
    __osSiGetAccess();

    do {
        // get pointer into PIF ram
        ptr = (u8 *)&__osPfsPifRam.ramarray;

        // skip format update if the previous command was a write on the same channel
        if (__osContLastCmd != CONT_CMD_WRITE_PAK || (u32)__osPfsLastChannel != channel) {
            __osContLastCmd = CONT_CMD_WRITE_PAK;
            __osPfsLastChannel = channel;

            for (i = 0; i < channel; i++) { *ptr++ = CONT_CMD_REQUEST_STATUS; }

            // set up read format
            __osPfsPifRam.pifstatus = CONT_CMD_EXE;

            READFORMAT(ptr)->dummy = CONT_CMD_NOP;
            READFORMAT(ptr)->txsize = CONT_CMD_WRITE_PAK_TX;
            READFORMAT(ptr)->rxsize = CONT_CMD_WRITE_PAK_RX;
            READFORMAT(ptr)->cmd = CONT_CMD_WRITE_PAK;
            READFORMAT(ptr)->datacrc = 0xFF;

            ptr[sizeof(__OSContRamReadFormat)] = CONT_CMD_END;
        } else {
            ptr += channel;
        }
        
        READFORMAT(ptr)->addrh = address >> 3;
        READFORMAT(ptr)->addrl = ((address << 5) | __osContAddressCrc(address));

        // copy given data to PIF data
        bcopy(buffer, READFORMAT(ptr)->data, BLOCKSIZE);
        
        // DMA PIF data
        __osSiRawStartDma(OS_WRITE, &__osPfsPifRam);
        crc = __osContDataCrc(buffer); // calculate CRC of sent data
        osRecvMesg(mq, (OSMesg *)NULL, OS_MESG_BLOCK);

        __osSiRawStartDma(OS_READ, &__osPfsPifRam);
        osRecvMesg(mq, (OSMesg *)NULL, OS_MESG_BLOCK);
        
        // check for error
        ret = CHNL_ERR(*READFORMAT(ptr));
        
        if (ret == 0) {
            // validate CRC
            if (crc != READFORMAT(ptr)->datacrc) {
                // CRC failed, retry if status is 0
                ret = __osPfsGetStatus(mq, channel);
                if (ret != 0) {
                    break;
                } else {
                    ret = PFS_ERR_CONTRFAIL;
                }
            }
        } else {
            // failed
            ret = PFS_ERR_NOPACK;
        }
    } while (ret == PFS_ERR_CONTRFAIL && retry-- >= 0);
    
    // release SI access lock
    __osSiRelAccess();

    return ret;
}
