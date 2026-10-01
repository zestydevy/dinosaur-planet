#ifndef _DLLS_347_H
#define _DLLS_347_H

#include "dll.h"

DLL_INTERFACE(DLL_347_texscroll2) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*changeScrollSpeed)(Object* self, s8 vSpeedA);
};

#define dll_TexScroll2(obj) (((DLL_347_texscroll2*)obj->dll)->vtbl)

#endif // _DLLS_324_H
