#ifndef _DLL_10_H
#define _DLL_10_H

#include "dll_def.h"
#include "PR/gbi.h"
#include "types.h"

DLL_INTERFACE(DLL_10_newstars) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*Init)(void);
/*1*/ void (*Print)(Gfx** gdl);
/*2*/ void (*Free)(void);
};

#endif // _DLL_10_H
