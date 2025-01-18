// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include <PR/rdb.h>
#include <PR/R4300.h>
#include <PR/ultratypes.h>
#include <libultra/os/rdbint.h>

s32 gWriteHostInitialized = 0;
OSMesgQueue gWriteHostQueue;
OSMesg gWriteHostQueueBuffer[1];

void osWriteHost(void *dramAddr, u32 nbytes) {
    u8 *dramPtr;
    u32 sent;
    u8 sizePacket[3];
    u32 size;

    dramPtr = (u8 *)dramAddr;

    if (!gWriteHostInitialized) {
        osCreateMesgQueue(&gWriteHostQueue, gWriteHostQueueBuffer, 1);
        osSetEventMesg(OS_EVENT_RDB_DATA_DONE, &gWriteHostQueue, NULL);
        gWriteHostInitialized = TRUE;
    }

    while (nbytes != 0) {
        if (nbytes < RDB_DATA_MAX_BLOCK_SIZE) {
            size = nbytes;
        } else {
            size = RDB_DATA_MAX_BLOCK_SIZE;
        }

        sizePacket[0] = (u8)((size & 0xFF0000) >> 16);
        sizePacket[1] = (u8)((size & 0xFF00) >> 8);
        sizePacket[2] = (u8)(size & 0xFF);
        
        sent = 0;

        while (sent < 3) {
            sent += __osRdbSend(&sizePacket[sent], 3 - sent, RDB_TYPE_GtoH_DATA_CT);
        }

        sent = 0;

        while (sent < size) {
            sent += __osRdbSend(&dramPtr[sent], size - sent, RDB_TYPE_GtoH_DATA);
        }

        nbytes -= size;
        dramPtr += size;

        osRecvMesg(&gWriteHostQueue, NULL, OS_MESG_BLOCK);
    }
}
