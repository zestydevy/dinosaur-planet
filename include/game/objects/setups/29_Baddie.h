#ifndef _SYS_GAME_OBJECTS_SETUPS_BADDIE_H
#define _SYS_GAME_OBJECTS_SETUPS_BADDIE_H

#include "game/objects/object.h"

// ctrl: 29 | size:0x38
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18; //gamebit set when defeated
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
/*22*/ s16 unk22; //droppedItemIdx
/*24*/ s16 unk24; //objSeqID
/*26*/ s8 unk26;
/*27*/ s8 initialWeaponID; // chuka playerAimOffsetY
/*28*/ s8 unk28; // chuka angularRange
/*29*/ u8 unk29; // chuka range2D
/*2A*/ s8 unk2A; //yaw (when respawned)?
/*2B*/ u8 unk2B; //flags?
/*2C*/ s16 unk2C; //respawn time (in seconds)?
/*2E*/ s8 unk2E; //objSeqID, played when lifetime expires?
/*2F*/ u8 unk2F; // chuka chuckProbability
/*30*/ s16 unk30;
/*32*/ u8 quarterHitpoints; // HP / 4
/*33*/ u8 pad33;
/*34*/ u16 unk34;
} Baddie_Setup;

// size: 0x24
// Some baddies fire projectile objects that are also in the baddie controlNo group but do not
// use the main Baddie_Setup. These only exist at runtime and cannot be pre-placed in the map.
typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E; // bit?
    s16 unk20; // bit?
    s16 unk22;
} BaddieProjectile_Setup;

#endif
