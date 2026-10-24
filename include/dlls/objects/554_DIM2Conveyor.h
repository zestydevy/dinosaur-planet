#ifndef _DLLS_554_H
#define _DLLS_554_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_554_DIM2Conveyor) {
/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
/*7*/ void (*Move)(Object* self, Object* objConveyed, f32 updateRate);
};

#endif // _DLLS_554_H
