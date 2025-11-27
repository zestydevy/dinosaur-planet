#ifndef _DLLS_314_H
#define _DLLS_314_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s8 unk1A; //yaw
    s16 unk1C;
    s16 unk1E;
} Food_Setup;

typedef struct {
    s16 expiry;
    u8 healthRestored;
    u8 unk3;
    u16 unk4;
    s16 gamebitID; //inventory item
    s16 objectID;  //Object to create when placed
} FoodbagItem;

typedef struct {
    u8 nextIndex; //total placed objects?
    Object* placedObjects[20];
    u16 foodType[20]; //foodType of each placed object?
} FoodbagStructUnk; //food objects placed in world?

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
    /*10*/ void (*Foodbag_func_83C)(Object* self);
    /*11*/ void (*Foodbag_func_708)(Object* self, s32 arg1);
    /*12*/ void (*Foodbag_func_7E0)(Object* self, s16 arg1);
    /*13*/ s32 (*Foodbag_func_650)(Object* self, s32 arg1);
    /*14*/ u16 (*Foodbag_func_A0C)(Object* self);
    /*15*/ s32 (*Foodbag_func_A1C)(Object* self);
    /*16*/ s16 (*Foodbag_get_anim_objectID_from_food_gamebit)(s16 foodGamebit);
};

#endif // _DLLS_314_H
