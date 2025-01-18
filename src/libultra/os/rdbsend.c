// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include <PR/rdb.h>
#include <PR/R4300.h>
#include <PR/ultratypes.h>
#include <libultra/os/rdbint.h>

u32 __osRdb_IP6_Empty = 1;

u32 __osRdbSend(u8 *buf, u32 size, u32 type) {
    rdbPacket *pktPtr;
    rdbPacket pkt;
    u32 pktLength;
    s32 i;
    s32 k;
    s32 wasEmpty;
    u32 savedMask;
    u32 sent;

    k = 0;
    wasEmpty = 0;
    sent = 0;

    savedMask = __osDisableInt();

    if (__osRdb_IP6_Empty) {
        __osRdb_IP6_Empty = FALSE;

        pkt.type = type;
        
        pktLength = size < 3 ? size : 3;

        pkt.length = pktLength;
        i = 0;
        
        while (i < pktLength) {
            pkt.buf[i++] = buf[k++];
        }

        size -= pktLength;
        sent = pktLength;
        wasEmpty = 1;
    }

    while (size != 0 && __osRdb_IP6_Ct < __osRdb_IP6_Size) {
        pktLength = size < 3 ? size : 3;

        pktPtr = &__osRdb_IP6_Data[__osRdb_IP6_CurWrite];
        pktPtr->type = type;
        pktPtr->length = pktLength;
        
        i = 0;
        while (i < pktLength) {
            pktPtr->buf[i++] = buf[k++];
        }

        __osRdb_IP6_CurWrite += 1;
        
        if (__osRdb_IP6_CurWrite >= __osRdb_IP6_Size) {
            __osRdb_IP6_CurWrite = 0;
        }

        __osRdb_IP6_Ct += 1;
        size -= pktLength;
        sent += pktLength;
    }

    if (wasEmpty != 0) {
        *((u32 *)RDB_BASE_REG) = *((u32 *)&pkt);
    }

    __osRestoreInt(savedMask);

    return sent;
}
