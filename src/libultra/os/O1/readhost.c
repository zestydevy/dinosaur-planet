#include <PR/os_internal.h>
#include <PR/rdb.h>
#include <PR/R4300.h>
#include <PR/ultratypes.h>
#include <libultra/os/rdbint.h>

s32 gReadHostInitialized = 0;
OSMesgQueue gReadHostQueue;
OSMesg gReadHostQueueBuffer[1];

u8 *__osRdb_Read_Data_Buf;
u32 __osRdb_Read_Data_Ct;

void osReadHost(void *dramAddr, u32 nbytes) {
    u8 buf[1];
    s32 sent;

    sent = 0;
    
    if (!gReadHostInitialized) {
        osCreateMesgQueue(&gReadHostQueue, gReadHostQueueBuffer, 1);
        osSetEventMesg(OS_EVENT_RDB_READ_DONE, &gReadHostQueue, NULL);
        gReadHostInitialized = TRUE;
    }
    
    __osRdb_Read_Data_Buf = dramAddr;
    __osRdb_Read_Data_Ct = nbytes;
    
    while (sent == 0) {
        sent += __osRdbSend(buf, 1, RDB_TYPE_GtoH_READY_FOR_DATA);
    }

    osRecvMesg(&gReadHostQueue, NULL, OS_MESG_BLOCK);
}
