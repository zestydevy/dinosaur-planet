#ifndef _DLLS_319_H
#define _DLLS_319_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "gbi_extra.h"

DLL_INTERFACE(DLL_319_fishingnet) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*set_visibility)(Object* self, u8 enable);
    /*8*/ void (*draw)(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, Vec3f* heldPosition, Vec3f* endPosition);
    /*9*/ void (*get_transform)(Object* self, SRT* oSrt);
    /*10*/ s32 (*catch)(Object* self, s32 flags, Object* caught);
    /*11*/ s32 (*func11)(Object* self, s32 arg1);
};

#endif // _DLLS_319_H

