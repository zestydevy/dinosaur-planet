// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include <PR/rcp.h>

void osDpSetStatus(u32 data)
{
    IO_WRITE(DPC_STATUS_REG, data);
}
