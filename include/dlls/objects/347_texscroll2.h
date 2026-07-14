#ifndef _DLLS_347_H
#define _DLLS_347_H

#include "dll.h"

DLL_INTERFACE(DLL_347_texscroll2) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*change_scroll_speed)(Object* self, s8 vSpeedA);
};

#endif // _DLLS_324_H
