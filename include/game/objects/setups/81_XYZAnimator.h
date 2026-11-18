#ifndef _SYS_GAME_OBJECTS_SETUPS_XYZANIMATOR_H
#define _SYS_GAME_OBJECTS_SETUPS_XYZANIMATOR_H

#include "game/objects/object.h"

typedef enum {
    XYZAnimator_MODE_0_Oneshot_and_Set_Gamebit,     //Interpolate to the goal, and set gamebitFinished
    XYZAnimator_MODE_1_Oneshot,                     //Interpolate to the goal
    XYZAnimator_MODE_2_Reversible_Set_Gamebit,      //Interpolate to/from the goal, and set gamebitFinished
    XYZAnimator_MODE_3_Radial_Falloff_Set_Gamebit,  //Animate vertices within an expanding radius, and set gamebitFinished
    XYZAnimator_MODE_4_Oneshot_and_Set_Gamebit_No_Initial_Update //Interpolate to the goal, and set gamebitFinished (without updating the vertices during initialisation)
} XYZAnimator_Modes;

typedef enum {
    XYZAnimator_POSITION_Initial,
    XYZAnimator_POSITION_Goal
} XYZAnimator_Positions;

// ctrl: 81 | size: 0x30
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebitActivate;    //Gamebit to begin the animation
/*1A*/ s16 gamebitFinished;    //Gamebit to set once the animation's finished
/*1C*/ s16 initDisplacementX;  //Initial position
/*1E*/ s16 initDisplacementY;  //Initial position
/*20*/ s16 initDisplacementZ;  //Initial position
/*22*/ s16 endDisplacementX;   //Goal position
/*24*/ s16 endDisplacementY;   //Goal position
/*26*/ s16 endDisplacementZ;   //Goal position
/*28*/ s8 animatorID;          //Shape animator tag
/*29*/ s8 speedX;              //Interpolation speed (in tenths of a worldSpace unit)
/*2A*/ s8 speedY;              //Interpolation speed (in tenths of a worldSpace unit)
/*2B*/ s8 speedZ;              //Interpolation speed (in tenths of a worldSpace unit)
/*2C*/ u8 mode;                //`XYZAnimator_Modes`
/*2E*/ u16 goalRadius;         //For `XYZAnimator_MODE_3_Radial_Falloff_Set_Gamebit`
} XYZAnimator_Setup;

#endif
