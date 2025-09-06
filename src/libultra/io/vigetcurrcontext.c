// @DECOMP_OPT_FLAGS=-O1
#include <PR/os_internal.h>
#include "libultra/io/viint.h"

__OSViContext *__osViGetCurrentContext(void)
{
    return __osViCurr;
}
