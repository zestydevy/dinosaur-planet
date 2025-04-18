#ifndef _DLLS_20_H
#define _DLLS_20_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll_def.h"

DLL_INTERFACE_BEGIN(20_screens)
    /*0*/ void (*show_screen)(s32 screenNo);
    /*1*/ void (*hide_screen)();
    /*2*/ void (*draw)(Gfx **gdl);
DLL_INTERFACE_END()

#endif //_DLLS_20_H
