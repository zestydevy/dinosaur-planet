#ifndef _SYS_GAME_OBJECTS_SETUPS_EFFECTBOX_H
#define _SYS_GAME_OBJECTS_SETUPS_EFFECTBOX_H

#include "game/objects/object.h"

// ctrl: 9 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 roll;
/*1A*/ u8 halfWidth;  // x dimension (stored halved), centred around origin
/*1B*/ u8 halfHeight; // y dimension (stored halved), base of box at origin
/*1C*/ u8 halfLength; // z dimension (stored halved), centred around origin
/*1D*/ u8 effect;
/*1E*/ u8 _unk1E;     // Unused, but rarely set to 1 in ROM?
/*1F*/ u8 gamebitDisableValue;      // disabled if the target gamebit is this value
/*20*/ s16 gamebitEnable;           // -1 if this effect box is always enabled
/*22*/ u8 target;                   // see `EffectBox_Targets`
} EffectBox_Setup;

#endif
