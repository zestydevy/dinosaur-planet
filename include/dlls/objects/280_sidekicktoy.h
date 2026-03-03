#ifndef _DLLS_280_H
#define _DLLS_280_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/engine/27.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
} SidekickToy_Setup;

typedef struct {
    DLL27_Data collision;
    f32 collisionRadius;    //DLL27 parameter
    f32 timer;              //Toy will disappear 100s after being thrown
    f32 interactionTimer;   //Toy has no collision and can't be collected until a half-second after being thrown
    u8 state;
} SidekickToy_Data;

typedef enum {
    TOY_STATE_0_Carried = 0, //by player or sidekick
    TOY_STATE_1_At_Rest = 1,
    TOY_STATE_2_In_Flight = 2,
    TOY_STATE_3_Collected = 3,
    TOY_STATE_4_Vanish = 4
} SidekickToy_States;

DLL_INTERFACE(DLL_280_SidekickToy) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*throw)(Object* self, Object* thrownBy, f32 speedX, f32 speedY, f32 speedZ);
    /*8*/ void (*carry)(Object* self, SidekickToy_Data* objdata);
    /*9*/ s32 (*is_stopped)(Object* self);
};

#endif // _DLLS_280_H
