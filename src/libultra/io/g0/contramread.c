#include <PR/os_internal.h>
#include <PR/rcp.h>
#include <libultra/io/controller.h>
#include <libultra/io/siint.h>

int __osPfsLastChannel = -1;

s32 __osContRamRead(OSMesgQueue *mq, int channel, u16 address, u8 *buffer) {
    s32 ret;
    int i;
    __OSContRamReadFormat *pifPtr;
    s32 retry;

    // make 3 attempts at a read
    retry = 2;

    // acquire lock for SI access
    __osSiGetAccess();

    do {
        // get pointer into PIF ram
        pifPtr = (__OSContRamReadFormat *)&__osPfsPifRam;

        // skip format update if the previous command was a read on the same channel
        if ((__osContLastCmd != CONT_RAM_READ) || (channel != __osPfsLastChannel)) {
            __osContLastCmd = CONT_RAM_READ;
            __osPfsLastChannel = channel;

            // ???
            for (i = 0; i < channel; i++) (*((u8*)pifPtr) = 0, pifPtr = (__OSContRamReadFormat*)((u8*)pifPtr + 1));

            // set up read format
            __osPfsPifRam.pifstatus = CONT_FORMAT;

            pifPtr->dummy = 0xFF;
            pifPtr->txsize = 3;
            pifPtr->rxsize = 0x21;
            pifPtr->cmd = CONT_RAM_READ;
            pifPtr->datacrc = 0xFF;

            ((u8*)pifPtr)[sizeof(__OSContRamReadFormat)] = FORMAT_END;
        } else {
            // ???
            pifPtr = (__OSContRamReadFormat *)(&((u8 *)&__osPfsPifRam)[channel]);
        }
        
        // ???
        pifPtr->address_hi = (u8)(address >> 3);
        pifPtr->address_lo = (u8)(__osContAddressCrc(address) | (address << 5));
        
        // DMA PIF data
        __osSiRawStartDma(OS_WRITE, &__osPfsPifRam);
        osRecvMesg(mq, (OSMesg *)NULL, OS_MESG_BLOCK);

        __osSiRawStartDma(OS_READ, &__osPfsPifRam);
        osRecvMesg(mq, (OSMesg *)NULL, OS_MESG_BLOCK);
        
        // check for error
        ret = (s32)(pifPtr->rxsize & CON_ERR_MASK) >> 4;
        
        if (ret == 0) {
            // validate CRC
            if (pifPtr->datacrc != __osContDataCrc(pifPtr->data)) {
                // CRC failed, retry if status is 0
                ret = __osPfsGetStatus(mq, channel);
                if (ret != 0) {
                    break;
                } else {
                    ret = PFS_ERR_CONTRFAIL;
                }
            } else {
                // success, copy data to given buffer
                bcopy(pifPtr->data, buffer, BLOCKSIZE);
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
