#ifndef _DLLS_304_H
#define _DLLS_304_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s8 varianceY;
    s8 effectType;      //Decides what kind of partFX are created for the firefly, and whether to create LFXEmitters
    s16 lifetime;       //How long the firefly will stay around before being unloaded
    s16 varianceZ;
    s16 unused1E;
    s16 unused20;
} LanternFireFly_Setup;

DLL_INTERFACE(DLL_304_LanternFireFly) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*set_next_spline_coord_by_player_position)(Object* self, Vec3f* coordsPlayer);
    /*8*/ void (*send)(Object *self);
    /*9*/ void (*set_home)(Object* self, f32 x, f32 y, f32 z);
};

#endif // _DLLS_304_H

