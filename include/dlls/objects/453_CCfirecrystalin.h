#ifndef _DLLS_453_H
#define _DLLS_453_H

#include "common.h"

// This is *technically* part of controlNo 103 but has completely different field
// types and cannot be a pre-placed object due to the pointer field.
typedef struct {
    ObjSetup base;
    Object* fireCrystal;
    s8 rotateSpeed;
    s8 yaw;
    u8 scrollSpeed;
} CCfirecrystalin_Setup; //0x20

#endif //_DLLS_453_H
