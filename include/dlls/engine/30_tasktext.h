#ifndef _DLLS_30_H
#define _DLLS_30_H

#include <PR/ultratypes.h>
#include "dll_def.h"

DLL_INTERFACE_BEGIN(30_tasktext)
    void (*func_18)();
    void (*func_DC)(u8 param1);
    u8 (*func_368)();
    void (*func_390)(u8 param1);
    void (*func_3F8)();
    s16 (*func_454)();
DLL_INTERFACE_END()

#endif //_DLLS_30_H
