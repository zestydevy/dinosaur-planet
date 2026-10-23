#ifndef _SYS_GAME_OBJECTS_SETUPS_SWITCH_H
#define _SYS_GAME_OBJECTS_SETUPS_SWITCH_H

#include "game/objects/object.h"

// ctrl: 53 | size: 0x20
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
/*1A*/ s16 _unused1A;
/*1C*/ s16 gameBitPressed;  //Gamebit to set when the switch is pressed down
} Switch_Setup;

#endif
