#ifndef _SYS_GAME_OBJECTS_SETUPS_ROCKETMUSHROOM_H
#define _SYS_GAME_OBJECTS_SETUPS_ROCKETMUSHROOM_H

#include "game/objects/object.h"

// ctrl: 45 | size: 0x20
typedef struct {
    ObjSetup base;
    s16 regrowthDelay;
    s16 sporeLaunchInterval;    //Frames between spore launches (actual interval has some randomised variance added to this value)
    s16 gamebitGrow;            //When specified, plant only grows when gamebit set (and gamebit unset upon exploding)
    s8 sporeAngularRange;       //Variance in launched SHspores' flight yaw
    s8 yaw;
} RocketMushroom_Setup;

#endif
