#ifndef _DLLS_16_H
#define _DLLS_16_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_16) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*func0)();
    /*1*/ void (*func1)(void*);
    /*2*/ void (*func2)(u8);
};

#endif //_DLLS_16_H
