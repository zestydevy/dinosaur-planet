#ifndef _SYS_GAME_OBJECTS_SETUPS_SEQPROP_H
#define _SYS_GAME_OBJECTS_SETUPS_SEQPROP_H

#include "game/objects/object.h"

// ctrl: 87 | size: 0x2c
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitOpenA;           //animCallback advances door to "Opening" state when this gamebit (and gamebitOpenB if specified) is set, or to "Closing" state when unset (obj must be in a sequence!)
/*1A*/ s16 gamebitRestoreState;    //Restores state when the door loads (either "Open" or "Closed" state) - objSeq will use a preemptTime to skip the door to its open position when restoring "Open" state.
/*1C*/ s16 preemptTime;            //The sequence time to jump to when the object loads in its "Open" state (state restored via `gamebitRestoreState`).
/*1E*/ s8 objSeqIdx;               //The door opening cutscene's objSeqIdx
/*1F*/ u8 yaw;
/*20*/ u8 preemptEnabledActors;    //Configures which actors to include when the door's sequence is played using a preemptTime
/*21*/ u8 scale;                   //Scale factor for the door (0x40 is 1x scale)
/*22*/ s16 gamebitOpenB;           //When specified, animCallback advances door to "Opening" state when this gamebit and gamebitOpenA are set (obj must be in a sequence!)
                                   //DFSH_Door1Special: A point on the door's Krazoa symbol lights up when this gamebit is set (through pressing ancient switches around Discovery Falls)
/*24*/ s16 gamebitCameraBack;      //When specified, animCallback flips part (`flipBitsCameraBack`) of this gamebit's value if the camera is behind the door when the door finishes opening/closing (obj must be in a sequence!)
/*26*/ s16 gamebitCameraFront;     //When specified, animCallback flips part (`flipBitsCameraFront`) of this gamebit's value if the camera is in front of the door when the door finishes opening/closing (obj must be in a sequence!)
/*28*/ u8 flipBitsCameraBack;      //The section of `gamebitCameraBack`'s value to flip when `gamebitCameraBack`'s value is updated
/*29*/ u8 flipBitsCameraFront;     //The section of `gamebitCameraFront`'s value to flip when `gamebitCameraFront`'s value is updated
} SeqDoor_Setup;

#endif
