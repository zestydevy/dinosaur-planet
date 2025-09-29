#include "PR/os.h"
#include "PR/os_internal.h"
#include "PR/ultratypes.h"
#include "libultra/io/controller.h"
#include "libultra/io/siint.h"

s32 NOTosContSetch(OSMesgQueue *mesgQ, void *arg1) {
    s32 ret;

    __osSiGetAccess();
    __osSiRawStartDma(OS_WRITE, arg1);
    osRecvMesg(mesgQ, NULL, OS_MESG_BLOCK);
    ret = __osSiRawStartDma(OS_READ, arg1);
    osRecvMesg(mesgQ, NULL, OS_MESG_BLOCK);
    __osContLastCmd = CONT_CMD_END;
    __osSiRelAccess();
    return ret;
}
