#ifndef _DLLS_217_H
#define _DLLS_217_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef void (*GuardClawItemPrintCallback)(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, Object* item, Object* guardClaw);

DLL_INTERFACE(DLL_217_GuardClaw) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ s16 (*GetFSAState)(Object* self);
    /*08*/ void (*SendMessage)(Object* self, u8 message);
    /*09*/ void (*SetItem)(Object* self, Object* item, GuardClawItemPrintCallback callback);
    /*10*/ void (*Func10)(Object* self, s32 arg1);
};

#endif
