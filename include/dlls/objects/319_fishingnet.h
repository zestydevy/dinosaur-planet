#ifndef _DLLS_319_H
#define _DLLS_319_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "gbi_extra.h"

typedef enum {
    FISHINGNET_FLAG_Unk1 = 0x1,
    FISHINGNET_FLAG_Unk2 = 0x2,
    FISHINGNET_FLAG_Unk4 = 0x4,
    FISHINGNET_FLAG_Unk8 = 0x8,
    FISHINGNET_GetFlags = 0x10,
    FISHINGNET_FLAG_Unk20 = 0x20,
    FISHINGNET_FLAG_Unk40 = 0x40
} fishingnet_Flags;

DLL_INTERFACE(DLL_319_fishingnet) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*SetVisibility)(Object* self, u8 visible);
    /*8*/ void (*Draw)(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, Vec3f* heldPosition, Vec3f* lookatPosition);
    /*9*/ void (*GetTransform)(Object* self, SRT* oSrt);
    /*10*/ s32 (*State)(Object* self, s32 flags, Object* caught);
    /*11*/ s32 (*Func4D4)(Object* self, s32 arg1);
};

#endif // _DLLS_319_H

