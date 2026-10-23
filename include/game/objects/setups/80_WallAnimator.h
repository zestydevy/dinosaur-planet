#ifndef _SYS_GAME_OBJECTS_SETUPS_WALLANIMATOR_H
#define _SYS_GAME_OBJECTS_SETUPS_WALLANIMATOR_H

#include "game/objects/object.h"

// ctrl: 80 | size: 0x28
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitDug;          //Gamebit to set when Tricky digs through the wall
/*1A*/ s16 sidekickExperience;  //Not used by the WallAnimator itself, but Tricky must have this much experience to dig the spot ("It's too deep, I need more practice!") 
/*1C*/ s16 sidekickCost;        //Tricky must have this much energy to dig the spot ("I'm too tired to do this right now!")
/*1E*/ s8 _unk1E;               //Unused - all existing WallAnimator objSetups have this set to 10
/*1F*/ s8 _unk1F;               //Unused - all existing WallAnimator objSetups have this set to 0
/*20*/ u8 findCommandRadius;    //Range for Find command to show up in inventory
/*21*/ u8 falloffRadius;        //Tagged shapes' vertices will be influenced if they're inside this radius (relative to the vertex nearest the WallAnimator)
/*22*/ s8 _unk22;               //Unused - all existing WallAnimator objSetups have this set to 60
/*23*/ u8 animatorID;           //Block shapes with this tag will be animated
/*24*/ s16 yaw;                 //NOTE: 16-bit precision, instead of setups' usual 8-bit yaw.
/*26*/ u8 unk26;                //Stored to `objData->unk28` during setup, but otherwise unused (all existing WallAnimator objSetups have this set to 40)
} WallAnimator_Setup;

#endif
