#ifndef _SYS_GAME_OBJECTS_SETUPS_HITANIMATOR_H
#define _SYS_GAME_OBJECTS_SETUPS_HITANIMATOR_H

#include "game/objects/object.h"

// ctrl: 79 | size: 0x20
typedef struct {
    ObjSetup base;
    s16 gamebitActivate; //gamebitID to check before animating
    u8 enableFlag1;      //when enabled, `HitAnimator_Flag_1` is stored whenever the value of the HitAnimator's gamebit changes, provided its local Block is loaded
    u8 blocksAnimatorID; //when animating Blocks model Shapes
    u8 mode;             //decides which behaviour to use: removing/adding HITS lines, removing/adding Blocks Shapes, fading in/out Blocks Shapes
    u8 hitsAnimatorID;   //when animating HITS lines
} HitAnimator_Setup;

#endif
