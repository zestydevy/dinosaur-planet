#ifndef _DLLS_358_H
#define _DLLS_358_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s16 unk1A; //yaw
    s16 unk1C; //pitch
    s16 unk1E; //roll
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    u16 unk38;
    s16 unk3A;
    s8 unk3C;
    s16 unk3E; //gamebit?
    s16 unk40; //gamebit?
    s16 unk42; //x
    s16 unk44; //y
    s16 unk46; //z
} Debris_Setup; //0x48

#endif // _DLLS_358_H

