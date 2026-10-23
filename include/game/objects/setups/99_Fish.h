#ifndef _SYS_GAME_OBJECTS_SETUPS_FISH_H
#define _SYS_GAME_OBJECTS_SETUPS_FISH_H

#include "game/objects/object.h"

// ctrl: 99 | size: 0x2c
typedef struct {
/*00*/    ObjSetup base;
/*18*/    u8 yaw;
/*19*/    u8 scale;
/*1A*/    u8 peekDuration;
/*1B*/    u8 bubbleRate;
/*1C*/    u16 swimDurationBeforePeek;     //How long to stay in State 2, before transitioning into peek state (#3)
/*1E*/    u16 respawnDuration;
/*20*/    u8 maxLateralSpeed;             //Stored at 100x actual value
/*21*/    u8 jumpDistanceLateral;
/*22*/    u8 jumpDuration;
/*23*/    u8 jumpAnticipateDuration;
/*24*/    u8 jumpRecoveryDuration;
/*26*/    u16 swimDurationAfterPeek;      //How long to stay in State 4, before transitioning into jump state
/*28*/    u8 surfaceOffsetY;
} Fish_Setup;

#endif
