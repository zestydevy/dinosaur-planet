#ifndef _SYS_GAME_OBJECTS_SETUPS_PROJECTILETARGET_H
#define _SYS_GAME_OBJECTS_SETUPS_PROJECTILETARGET_H

#include "game/objects/object.h"

// ctrl: 85 | size: 0x24
typedef struct {
    ObjSetup base;
    s16 gamebit;            //GamebitID to set when switch is hit
    s16 resetDelay;         //Number of deciseconds until the switch resets to its unpressed state (if `Switch_FLAG_Resets_After_Delay` is set)
    u8 pitch;               //Rotation
    u8 scale;               //Scale multiplier (0x64 = 100%)
    u8 settings;            //projswitch: Flags on lowest two bits, modelIndex on upper bits
                            //blownuptarget: Mode
    u8 yaw;                 //Rotation
    u8 tintR;
    u8 tintG;
    u8 tintB;
    u8 enableTint;          //Enable tinting the switch to a different colour
} ProjectileTarget_Setup;

#endif
