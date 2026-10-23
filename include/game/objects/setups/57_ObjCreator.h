#ifndef _SYS_GAME_OBJECTS_SETUPS_OBJCREATOR_H
#define _SYS_GAME_OBJECTS_SETUPS_OBJCREATOR_H

#include "game/objects/object.h"

// ctrl: 57 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebit;
/*1A*/ s16 type;
/*1C*/ u8 _unk1C[0x1E - 0x1C];
/*1E*/ s8 yaw8;
/*1F*/ s8 creatorIndex;
/*20*/ u8 _unk20[0x24 - 0x20];
} ObjCreator_Setup;

#endif
