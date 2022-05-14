#include <PR/os_internal.h>
#include <PR/rcp.h>
#include <libultra/io/controller.h>
#include <libultra/io/siint.h>

extern int __osPfsLastChannel;

s32 __osContRamWrite(OSMesgQueue *mq, int channel, u16 address, u8 *buffer, int force) {
    s32 ret;
    int i;
    __OSContRamReadFormat *pifPtr;
    s32 retry;
    u8 crc;

    // make 3 attempts at a write
    retry = 2;

    // skip if not forcing and address is [1,6] (protected region?)
    if ((force != 1) && (address < 7) && (address != 0)) {
        return 0;
    }

    // acquire lock for SI access
    __osSiGetAccess();

    do {
        // get pointer into PIF ram
        pifPtr = (__OSContRamReadFormat *)&__osPfsPifRam;

        // skip format update if the previous command was a write on the same channel
        if ((__osContLastCmd != CONT_RAM_WRITE) || (channel != __osPfsLastChannel)) {
            __osContLastCmd = CONT_RAM_WRITE;
            __osPfsLastChannel = channel;

            // ???
            for (i = 0; i < channel; i++) (*((u8*)pifPtr) = 0, pifPtr = (__OSContRamReadFormat*)((u8*)pifPtr + 1));

            // set up read format
            __osPfsPifRam.pifstatus = CONT_FORMAT;

            pifPtr->dummy = 0xFF;
            pifPtr->txsize = 0x23;
            pifPtr->rxsize = 1;
            pifPtr->cmd = CONT_RAM_WRITE;
            pifPtr->datacrc = 0xFF;

            ((u8*)pifPtr)[sizeof(__OSContRamReadFormat)] = FORMAT_END;
        } else {
            // ???
            pifPtr = (__OSContRamReadFormat *)(&((u8 *)&__osPfsPifRam)[channel]);
        }
        
        // ???
        pifPtr->address_hi = (u8)(address >> 3);
        pifPtr->address_lo = (u8)(__osContAddressCrc(address) | (address << 5));

        // copy given data to PIF data
        bcopy(buffer, pifPtr->data, BLOCKSIZE);
        
        // DMA PIF data
        __osSiRawStartDma(OS_WRITE, &__osPfsPifRam);
        crc = __osContDataCrc(buffer); // calculate CRC of sent data
        osRecvMesg(mq, (OSMesg *)NULL, OS_MESG_BLOCK);

        __osSiRawStartDma(OS_READ, &__osPfsPifRam);
        osRecvMesg(mq, (OSMesg *)NULL, OS_MESG_BLOCK);
        
        // check for error
        ret = (s32)(pifPtr->rxsize & CON_ERR_MASK) >> 4;
        
        if (ret == 0) {
            // validate CRC
            if (crc != pifPtr->datacrc) {
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
