#ifndef _DLLS_685_H
#define _DLLS_685_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef enum {
    DBBoneDust_STATE_Hidden = 0,
    DBBoneDust_STATE_Launched = 1,
    DBBoneDust_STATE_Hovering = 2,
    DBBoneDust_STATE_Fly_Towards_Player = 3
} DBBoneDust_States;

typedef struct {
    ObjSetup base;
    u16 _unk18;
    u16 _unk1A;
    u16 _unk1C;
    s16 unk1E;
    s16 unk20;
} DBBoneDust_Setup;

typedef struct {
    f32 baseY;
    // s32 rotation; //union of s32 and two s16s? TO-DO: see if this can work without union
    union {
        s32 asWord;
        s16 asHalfwords[2];
    } rotation;
    u8 state;
} DBBoneDust_Data;

DLL_INTERFACE(DLL_685_DBBoneDust) {
/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
/*7*/ int (*is_hidden)(Object *self);
/*8*/ void (*launch)(Object* self, Vec3f* position, Vec3f* speed, f32 arg3);
};

#endif // _DLLS_685_H
