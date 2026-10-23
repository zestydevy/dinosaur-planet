#ifndef _SYS_GAME_OBJECTS_SETUPS_CURVEFISH_H
#define _SYS_GAME_OBJECTS_SETUPS_CURVEFISH_H

#include "game/objects/object.h"

// ctrl: 116 | size: 0x24
typedef struct {
/*00*/    ObjSetup base;
/*18*/    u8 scale;
/*19*/    u8 maxSpeed;
/*1A*/    u8 ascendSpeed;
/*1B*/    u8 diveSpeed; 
/*1C*/    u16 swimOnBedDuration;
/*1E*/    u16 swimOnSurfaceDuration;
/*20*/    u16 respawnDuration;
/*22*/    u8 curveOffsetY;
/*23*/    u8 surfaceOffsetY; 
} CurveFish_Setup;

#endif
