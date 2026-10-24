#ifndef _SYS_GAME_OBJECTS_SETUPS_VISANIMATOR_H
#define _SYS_GAME_OBJECTS_SETUPS_VISANIMATOR_H

#include "game/objects/object.h"

// ctrl: 82 | size: 0x20
typedef struct {
    ObjSetup base;
    s16 gamebitID;          //gamebit controlling visibility (usually multi-bit, used as a bitfield)
    u8 animatorID1;         //the animatorID tag of the Block Shapes that will be shown/hidden
    u8 initialVisibility;   //Boolean
    u8 bitIndex;            //the index to check on the gamebit bitfield
    u8 animatorID2;         //up to 3 different animatorIDs can be specified
    u8 animatorID3;         //up to 3 different animatorIDs can be specified
} VisAnimator_Setup;

#endif
