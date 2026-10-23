#ifndef _SYS_GAME_OBJECTS_SETUPS_SNOWBIKE_H
#define _SYS_GAME_OBJECTS_SETUPS_SNOWBIKE_H

#include "game/objects/object.h"

// ctrl: 49 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 isSharpClawBike;             // Whether the bike can only be driven by SharpClaw
/*1A*/ s16 gamebitUnlocked;            // The bike can only be mounted when this gamebit is set
/*1C*/ u8 racetrackIdx;                // Which race the bike is used in (CloudRunner Fortress vs. Golden Plains)
/*1D*/ u8 unk1D;
/*1E*/ s16 gamebitFinished;            // Bike disappears and stops updating when this gamebit is set
/*20*/ u8 yJoySharpClaw;               // Strength factor for SharpClaws' simulated yJoy steering value
} SnowBike_Setup;

#endif
