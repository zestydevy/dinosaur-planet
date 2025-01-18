// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include "libultra/io/viint.h"

void osViSetMode(OSViMode *modep)
{
    register u32 saveMask;
    saveMask = __osDisableInt();
    __osViNext->modep = modep;
    __osViNext->state = VI_STATE_01;
    __osViNext->control = __osViNext->modep->comRegs.ctrl;
    __osRestoreInt(saveMask);
}
