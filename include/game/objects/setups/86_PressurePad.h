#ifndef _SYS_GAME_OBJECTS_SETUPS_PRESSUREPAD_H
#define _SYS_GAME_OBJECTS_SETUPS_PRESSUREPAD_H

#include "game/objects/object.h"

// ctrl: 86 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 modelIdx;
/*1A*/ s16 gameBitPressed;             //Gamebit to set when the switch is pressed down
/*1C*/ u8 yOffsetAnimation;            //How far down the switch should move when pressed
/*1D*/ u8 yThreshold;                  //Threshold for other objects pressing switch
/*1E*/ u8 distanceGuardCommand;        //Player distance at which Guard sidekick command is selectable (only used by DLL 287)
/*20*/ s16 gamebitActivated;           //Gamebit to check if switch is deactivated
} PressurePad_Setup;

#endif
