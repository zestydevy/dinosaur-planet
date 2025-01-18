// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>

extern OSThread *__osActiveQueue;

OSThread *__osGetActiveQueue(void)
{
    return __osActiveQueue;
}
