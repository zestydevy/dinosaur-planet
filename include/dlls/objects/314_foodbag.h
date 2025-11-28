#ifndef _DLLS_314_H
#define _DLLS_314_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

#define MAX_PLACED_FOOD 20

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s8 yaw; //yaw
    s16 unk1C;
    s16 unk1E;
} Food_Setup;

/** Defines various parameters for a type of food that can be stored in a food bag */
typedef struct {
    s16 expiry;         //frames until food spoils
    u8 healthRestored;  //when eaten by player
    u8 unk3;
    u16 unk4;
    s16 gamebitID; //inventory item gamebit
    s16 objectID;  //Object to create when placed
} FoodbagItem;

/** Handles food objects placed in the world */
typedef struct {
    u8 nextIndex; //index to use when next placing food (eventually wraps around to overwrite oldest)
    Object* objects[MAX_PLACED_FOOD]; //the actual food objects in the world
    u16 foodType[MAX_PLACED_FOOD]; //foodType of each placed object?
} FoodbagPlaced;

typedef struct {
    u16 small;
    u16 medium;
    u16 large;
} FoodbagGamebits;

#define BLANK_FOODBAG_ITEM {0, 0, 0, 0, 0, 0}
#define NO_FOOD_OBJECT_ID -1

DLL_INTERFACE(DLL_314_Foodbag) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ int (*Foodbag_func_590)(Object* self);
    /*8*/ void (*Foodbag_func_5B4)(Object* self, s32 arg1, s32 arg2);
    /*9*/ void (*Foodbag_func_5FC)(Object* self, s32 arg1);
    /*10*/ void (*Foodbag_set_capacity)(Object* self);
    /*11*/ void (*Foodbag_func_708)(Object* self, s32 arg1);
    /*12*/ void (*Foodbag_func_7E0)(Object* self, s16 arg1);
    /*13*/ s32 (*Foodbag_set_eat_config)(Object* self, s32 arg1);
    /*14*/ u16 (*Foodbag_func_A0C)(Object* self);
    /*15*/ s32 (*Foodbag_func_A1C)(Object* self);
    /*16*/ s16 (*Foodbag_get_anim_objectID_from_food_gamebit)(s16 foodGamebit);
};

#endif // _DLLS_314_H
