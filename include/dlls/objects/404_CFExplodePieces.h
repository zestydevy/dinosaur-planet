#ifndef _DLLS_404_H
#define _DLLS_404_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef enum {
    CFExplodePieces_STATE_0_Stopped,
    CFExplodePieces_STATE_1_Moving,
    CFExplodePieces_STATE_2_Finished
} CFExplodePieces_States;

typedef enum {
    CFExplodePieces_FLAGS_4_Touching_Ground = 4
} CFExplodePieces_Flags;

DLL_INTERFACE(DLL_404_CFExplodePieces) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ CFExplodePieces_States (*GetState)(Object* self);
};

#define dll_CFExplodePieces(obj) (((DLL_404_CFExplodePieces*)obj->dll)->vtbl)

#endif // _DLLS_404_H

