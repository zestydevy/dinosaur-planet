#ifndef _DLLS_437_H
#define _DLLS_437_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

// size: 0x1C
typedef struct {
    ObjSetup base;
    u8 _unk18[0x1A - 0x18];
    u8 unk1A;
} EWTrobotpatrol_Setup;

typedef void (*EWTrobotpatrolCallback)(Object* base, Object* robot, s32 arg2, u32 uID);

DLL_INTERFACE(DLL_437_EWTrobotpatrol) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ void (*Func_3AE4)(Object* self, Object* base, EWTrobotpatrolCallback callback);
    /*08*/ void (*Func_3AF8)(Object* self, s32 arg1);
};

#endif //_DLLS_216_H
