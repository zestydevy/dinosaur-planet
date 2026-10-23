#ifndef _SYS_GAME_OBJECTS_SETUPS_SFXPLAYER_H
#define _SYS_GAME_OBJECTS_SETUPS_SFXPLAYER_H

#include "game/objects/object.h"

// ctrl: 96 | size: 0x20
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebit;   // LOOPING SOUNDS: start/stop when value changes, or use distance check if gamebit unspecified.
                      // ONE-SHOT SOUNDS: play sound when value changes (if "Play At Random Inside Radius" mode isn't being used).
/*1A*/ s16 soundID;
/*1C*/ u8 flags;      //See `SfxPlayer_Flags`.
/*1D*/ u8 halfRadius; //Stored halved, the sound plays when the player is inside this radius. Looping sounds stop when the player is outside of (radius + 10).
} SfxPlayer_Setup;

#endif
