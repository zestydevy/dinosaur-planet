#ifndef _DLLS_272_H
#define _DLLS_272_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_272_Collectable) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ int (*is_collected)(Object *self);
    /*8*/ void (*set_pause_state)(Object* self, s32 state);
    /*9*/ s32 (*get_area_value)(Object *self);
    /*10*/ void (*set_speed)(Object* self, f32 speedX, f32 speedY, f32 speedZ);
    /*11*/ void (*set_visibility)(Object* self, s32 visibility);
    /*12*/ u8 (*get_visibility)(Object* self);
    /*13*/ void (*save_position)(Object* self, f32 x, f32 y, f32 z);
};

#endif // _DLLS_272_H
