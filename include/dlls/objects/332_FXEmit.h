#ifndef _DLLS_332_H
#define _DLLS_332_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 activationRange;
/*19*/ s8 bank;
/*1A*/ s16 indexInBank;
/*1C*/ s16 fxRate;
/*1E*/ s16 toggleGamebit;
/*20*/ s16 disableGamebit;
/*22*/ s8 roll;
/*23*/ s8 pitch;
/*24*/ s8 yaw;
/*25*/ s8 rollSpeed;        //animation speed for particle's roll   (0x7F for a slower default speed)
/*26*/ s8 pitchSpeed;       //animation speed for particle's pitch  (0x7F for a slower default speed)
/*27*/ s8 yawSpeed;         //animation speed for particle's yaw    (0x7F for a slower default speed)
/*28*/ u8 flagConfig;       //chooses between 4 partfx flag presets
/*29*/ u8 interval;         //FXEmit activates repeatedly, at multiples of 100 frames 
                            //interval = 0 skips interval behaviour, 0xFF disables FXEmit
/*2A*/ s16 intervalSoundID; //soundID to play during interval activation
} FXEmit_Setup;

#endif // _DLLS_332_H
