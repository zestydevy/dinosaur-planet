#ifndef _SYS_GAME_OBJECTS_SETUPS_KTREX_H
#define _SYS_GAME_OBJECTS_SETUPS_KTREX_H

#include "dlls/engine/33_BaddieControl.h"
#include "game/objects/object.h"

// ctrl: 120 | size: 0x5c
typedef struct {
/*00*/ Baddie_Setup base;
/*38*/ f32 speeds[3]; // Movement speed, per "anger" level.
/*44*/ u16 roarTime[3]; // How long a roar lasts.
/*4A*/ u16 vulnerableTime[4]; // How long the boss is vulnerable for.
/*52*/ u8 reverseChance[4]; // Chance to reverse direction.
/*56*/ u8 chargeChance[4]; // Chance to charge without seeing Sabre.
} KTrex_Setup;

#endif
