#ifndef _SYS_GAME_OBJECTS_SETUPS_INFOPOINT_H
#define _SYS_GAME_OBJECTS_SETUPS_INFOPOINT_H

#include "game/objects/object.h"

// ctrl: 8 | size: 0x20
typedef struct {
    ObjSetup base;
    s16 gametextID;                      //Gametext to print
    s8 unused1A;
    u8 unk1B;                            //Assigned to `objData->unk10` in setup, but otherwise unused
    u8 yaw;
} InfoPoint_Setup;

#endif
