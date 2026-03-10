#ifndef _DLLS_519_H
#define _DLLS_519_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef enum {
    SC_ObjGroup0_Main_SwapStone_Area = 0,
    SC_ObjGroup1_Autumnal_Path = 1,
    SC_ObjGroup2_LightFoot_Village = 2,
    SC_ObjGroup3_Golden_Plains_Approach = 3,
    SC_ObjGroup4_Golden_Plains_Gateway = 4,
    SC_ObjGroup5_LightFoot_Village_Mound_Interior = 5,
    SC_ObjGroup6_LightFoot_Village_Rotating_Door = 6
} SC_ObjectGroups;

DLL_INTERFACE(DLL_519_SC_Levelcontrol) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*func7)(Object *self, u8 arg1);
    /*8*/ u8 (*func8)(Object *self);
};

#endif // _DLLS_519_H
