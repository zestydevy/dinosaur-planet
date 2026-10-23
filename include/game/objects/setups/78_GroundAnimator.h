#ifndef _SYS_GAME_OBJECTS_SETUPS_GROUNDANIMATOR_H
#define _SYS_GAME_OBJECTS_SETUPS_GROUNDANIMATOR_H

#include "game/objects/object.h"

// ctrl: 78 | size: 0x28
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitDug;         //Gamebit to set when Tricky digs up the spot
/*1A*/ s16 sidekickExperience; //Not used by the GroundAnimator itself, but Tricky must have this much experience to dig the spot ("It's too deep, I need more practice!") 
/*1C*/ s16 sidekickCost;       //Not used by the GroundAnimator itself, but Tricky must have this much energy to dig the spot ("I'm too tired to do this right now!")
/*1E*/ s16 magicCaveID;        //Values 3 onwards cause the dig spot to become be a Magic Cave entrance
/*20*/ u8 digDepthMax;         //Dig distance (stored at 100x scale)
/*21*/ u8 soundIndex;          //Index of the sound to play when digging has finished (0: small secret, 1: big secret)
/*22*/ u8 unused22;            //Not used by the GroundAnimator itself, most (but not all) GroundAnimator objSetups have this set to 0
/*23*/ u8 findCommandRadius;   //Range for Find command to show up in inventory
/*24*/ u8 unused24;            //Unused - all existing GroundAnimator objSetups have this set to 0
/*25*/ u8 animatorID;          //Block shapes with this tag will be animated
/*26*/ u8 falloffRadius;       //Vertex influence tapers off with this radius, and it also decides how far Tricky scoots backwards while digging
/*27*/ u8 collectableDepth;    //How far down the collectable is buried beneath the dig spot
} GroundAnimator_Setup;

#endif
