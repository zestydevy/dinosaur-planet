/**
 * Internal libultra remote debug (RDB) code.
 */

#ifndef _LIBULTRA_OS_RDBINT_H
#define _LIBULTRA_OS_RDBINT_H

#include <PR/ultratypes.h>
#include <PR/rdb.h>

extern rdbPacket *__osRdb_IP6_Data;
extern u32 __osRdb_IP6_Size;
extern u32 __osRdb_IP6_Ct;
extern u32 __osRdb_IP6_CurWrite;
extern u32 __osRdb_IP6_CurSend;
extern u32 __osRdb_IP6_Empty;

#endif //_LIBULTRA_OS_RDBINT_H
