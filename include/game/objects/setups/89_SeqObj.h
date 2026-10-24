#ifndef _SYS_GAME_OBJECTS_SETUPS_SEQOBJ_H
#define _SYS_GAME_OBJECTS_SETUPS_SEQOBJ_H

#include "game/objects/object.h"

typedef enum {
    SEQOBJ_OPTIONS_ManualHasPlayedBit = 1, // don't auto update the hasPlayed gamebit
    SEQOBJ_OPTIONS_ManualHasPlayed_Set_By_SeqCmd = 2, //gamebitFinished intended to be set via a seqCommand?
    SEQOBJ_OPTIONS_ManualHasPlayed_Dont_Unset_Play_Gamebit = 4, //Avoids setting gamebitPlay back to 0 after sequence finishes, under some circumstances?
    SEQOBJ_OPTIONS_AutoHasPlayed_Set_After_Sequence = 8, //Set gamebitFinished after sequences finishes, instead of immediately?
    SEQOBJ_OPTIONS_HasReplayActorMask = 16 // if set, more than just the seqobj will be controlled by a replay
} SeqObj_PlaybackOptions;

// ctrl: 89 | size: 0x28
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitHasPlayed; // when gamebit is set, this objseq has played before
/*1A*/ s16 gamebitPlay;    // when gamebit is set, triggers this seqobj to play
/*1C*/ u8 yaw;
/*1D*/ u8 playbackOptions; // SeqObj_PlaybackOptions
/*1E*/ s8 seqIndex;            //The index of the sequence in the Object.bin entry's sequence list
/*1F*/ s8 type;        //Choose between 3D models, visible when debugging (usually a clapperboard)
                       //DBPlaceHolder: whether the egg is stealable
/*20*/ s16 replayStartTime;
/*22*/ u16 replayActorMask; // bitfield of which actors are enabled if this seq is replaying
/*24*/ u8 warpID;              //Optionally warp the player
} SeqObj_Setup;

#endif
