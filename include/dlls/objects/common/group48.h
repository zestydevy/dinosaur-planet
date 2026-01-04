#ifndef _DLLS_COMMON_GROUP_48_H
#define _DLLS_COMMON_GROUP_48_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "PR/gbi.h"

typedef struct {
    u8 pad0[0x18];
    f32 unk18[2];
    f32 unk20[2];
    f32 unk28[2];
    f32 unk30[2];
    f32 unk38[2];
    f32 unk40[2];
    u8 pad48[0x8A-0x48];
    s16 unk8A;
    s16 unk8C;
} ObjGroup48_Data;

DLL_INTERFACE(DLL_IGROUP_48) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
	/*7*/ void (*func7)(Object *, f32);
	/*8*/ void (*func8)(Object *);
	/*9*/ void (*func9)(Object *, Object *, Vec3f *);
	/*10*/ void (*func10)(Object *, Gfx **, Mtx **, Vertex **, Triangle **);
	/*11*/ void (*func11)(Object *);
	/*12*/ void (*func12)(Object *, s32);
	/*13*/ void (*func13)(Object *, u8); // second arg might be a different type
	/*14*/ UnknownDLLFunc func14;
	/*15*/ UnknownDLLFunc func15;
	/*16*/ UnknownDLLFunc func16;
	/*17*/ UnknownDLLFunc func17;
	/*14*/ void (*func18)(Object *, f32, f32);
};

#endif //_DLLS_COMMON_GROUP_48_H
