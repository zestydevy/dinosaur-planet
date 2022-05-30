#include <PR/os_internal.h>
#include <PR/rdb.h>
#include <PR/ultratypes.h>
#include <libultra/os/rdbint.h>

rdbPacket *__osRdb_IP6_Data;
u32 __osRdb_IP6_Size;
u32 __osRdb_IP6_Ct;
u32 __osRdb_IP6_CurWrite;
u32 __osRdb_IP6_CurSend;

void osInitRdb(u8 *sendBuf, u32 sendSize) {
    s32 savedMask;
    
    sendSize = sendSize / 4;
    
    // 4-byte align sendBuf address
    if (((u32)sendBuf & 3) != 0) {
        sendBuf = (u8 *)((u32)sendBuf & 3) + 4;
        sendSize = sendSize - 1;
    }

    savedMask = __osDisableInt();

    __osRdb_IP6_Data = (rdbPacket *)sendBuf;
    __osRdb_IP6_Size = sendSize;
    __osRdb_IP6_Ct = 0;
    __osRdb_IP6_CurWrite = 0;
    __osRdb_IP6_CurSend = 0;

    __osRestoreInt(savedMask);
}
