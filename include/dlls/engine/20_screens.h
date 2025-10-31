#ifndef _DLLS_20_H
#define _DLLS_20_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_20_screens) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*show_screen)(s32 screenNo);
    /*1*/ void (*hide_screen)(void);
    /*2*/ void (*draw)(Gfx **gdl);
};

#endif //_DLLS_20_H
