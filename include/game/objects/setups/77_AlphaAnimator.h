#ifndef _SYS_GAME_OBJECTS_SETUPS_ALPHAANIMATOR_H
#define _SYS_GAME_OBJECTS_SETUPS_ALPHAANIMATOR_H

#include "game/objects/object.h"

// ctrl: 77 | size: 0x28
typedef struct {
    ObjSetup base;
    // TODO: these two could probably be merged
    union {
        struct {
            /*18*/ s16 gamebitActivate;            //Gamebit controlling when the fade is activated/deactivated
            /*1A*/ s16 gamebitActivated;           //(For modes 0, 2, 3) Optionally gamebit that's set when the fade animation is complete - also used to restore fully-faded state
            /*1C*/ u8 initialOpacity;              //Opacity at the beginning of the fade animation (can be higher than the goal opacity)
            /*1D*/ u8 goalOpacity;                 //Opacity at the end of the fade animation (can be lower than the initial opacity)
            /*1E*/ s8 animatorID;                  //Vertices will only be affected if they have this shape animatorID
            /*1F*/ s8 fadeSpeed;                   //Fade in rate. In radial mode, the sign inverts the behaviour (fade inside/outside radius)!
            /*20*/ u8 flags;                       //Mode stored on bits 0, 1 (see `GET_MODE` macro) - higher bits determine whether to play a sound
            /*21*/ u8 removeCollisionWhenHidden;   //Boolean, fade affects whether the affected shapes are tangible
            /*22*/ u16 fadeRadiusGoal;             //(For radial mode) End radius of the fade animation
            /*24*/ u16 soundID;                    //Which sound to play (requires sound playing enabled on `objData->flags`)
        } alphaAnim;
        struct {
            /*18*/ s16 gamebitEnable;
            /*1A*/ s16 unused1;
            /*1C*/ u8 maxOpacity;
            /*1D*/ u8 minOpacity;
            /*1E*/ s8 animatorID;
            /*1F*/ s8 unused2;
            /*20*/ u8 unused3;
            /*21*/ u8 removeCollision; //When enabled, shape is tangible while visible
            /*22*/ u16 minDistance;
        } portalTexAnim;
    };
} AlphaAnimator_Setup;

#endif
