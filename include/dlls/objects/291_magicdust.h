#ifndef _DLLS_291_H
#define _DLLS_291_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/engine/27.h"
#include "game/objects/object.h"

typedef struct {
    DLL27_Data collision;
    f32 collisionRadius;    //Radius for collider and collection (+8 units for collection)
    f32 timer;              //Timer (in frames) for different events: delayed fall, vanish, delete
    u16 fxIDSparkle;        //ParticleID for the small sparkles to create when vanishing
    u16 fxIDGlow;           //ParticleID for the large glow that appears over the crystal
    s16 soundID;            //SoundID to play when collected
    s16 animObjectID;       //ObjectID to use for the item collect sequence's VariableObject
    s16 soundTimer;         //Random delay between ringing sounds
    u8 flags;
    u8 bounces;             //How many times the gem has bounced (stops after 6)
} MagicDust_Data;

typedef enum {
    MagicDust_FLAG_None = 0,
    MagicDust_FLAG_Delay_Fall = 1,              //Gem falls after 1 second
    MagicDust_FLAG_On_Ground = 2,               //Gem stopped, spins in place
    MagicDust_FLAG_Fell = 4,                    //Gem falls, bounces, dissipates into sparkles once lifetime's up
    MagicDust_FLAG_Vanish = 8,                  //Gem should unload self in 3 seconds
    MagicDust_FLAG_Tutorial_Sequence = 0x10     //Gem activated the item collection tutorial sequence
} MagicDust_Flags;

typedef enum {
    MagicDust_MODE_Default = 0,         //Gem falls immediately, with randomised angle/speed
    MagicDust_MODE_Delay_Fall = 1,      //Gem falls after 1 second
    MagicDust_MODE_Towards_Player = 2   //Gem's initial velocity points towards the player
} MagicDust_Modes;

#endif // _DLLS_291_H
