#ifndef _DLLS_211_H
#define _DLLS_211_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dlls/objects/common/sidekick.h"

DLL_INTERFACE(DLL_211_Tricky) {
    /*:*/ DLL_INTERFACE_BASE(DLL_ISidekick);
    /*25*/ s32 (*func25)(Object* sidekick);
    /*26*/ UnknownDLLFunc func26;
};

#endif // _DLLS_211_H
