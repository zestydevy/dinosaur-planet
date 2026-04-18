#ifndef _DLLS_502_H
#define _DLLS_502_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "sys/curves.h"
#include "types.h"

typedef struct {
	ObjSetup base;
	u8 index;               //Values 4/5 have curve-following behaviour. Possibly meant as an index for the 10 White Mushrooms? Only those in SwapStone Hollow (Well) have unique values though: the surface ones are all set to 1.
	u8 fleeRange;           //Mushroom flees when player/sidekick inside this radius
	s16 gamebitCollected;   //Gamebit to set when collected, preventing mushroom from reappearing (different from the inventory/count gamebitID: see `objData->gamebitInventory`)
	u8 hopSpeed;            //Coefficient for hop speed(?) (0 to 255, mapping from 0.0 to 0.2)
	u8 jumpSpeed;           //Coefficient for jump speed   (0 to 255, mapping from 0.0 to 1.0)
	u8 unused1E;
	u8 alertRange;          //Mushroom enters alert state when player/sidekick inside this radius (also enables sidekick's Find command when sidekick inside radius)
	u8 modelIdx;            //Mushroom type (blue/white)
} SHmushroom_Setup;

typedef struct {
	UnkCurvesStruct curves;		//Curve-following data (e.g. for the lily pond White Mushroom in SwapStone Hollow's well)
	f32 pursuerDistance;        //Distance to player/sidekick
	f32 prevPursuerDistance;    //Distance to player/sidekick on previous tick
	f32 jumpSpeedMultiplier;    //Factor for speed while jumping
	f32 hopSpeedMultiplier;     //Factor for speed while hopping(?) Set during setup but otherwise unused.
	f32 jumpSpeed;              //Jump speed, extracted from jump animation's root motion data
	f32 hopSpeed;               //Hop speed, extracted from hop animation's root motion data
	f32 curvesDelta;            //Curve-following parameter (speed?)
	f32 stunnedTimer;           //Countdown for exiting stunned state
	f32 stunFxTimer;            //Countdown for periodically emitting particles while stunned
	u32 soundHandleStun;        //Controls bird tweeting sound loop while stunned
	s16 fleeAngle;              //Yaw to flee towards (different from mushroom's visual yaw, since that needs to be offset by 90 degrees during jump animation)
	s16 _unk132;
	s16 gamebitInventory;       //Gamebit to increment when collected
	u8 state;                   //State Machine value
	u8 flags;                   //Tracks various conditions for the State Machine (animation finished, moving, hurt, etc.)
	s32 _unk138;
} SHmushroom_Data;

typedef enum {
	SHmushroom_MODEL_0_Blue_Mushroom = 0,
	SHmushroom_MODEL_1_White_Mushroom = 1
} SHmushroom_ModelIndices;

typedef enum {
	SHmushroom_FLAG_None = 0x0,
	SHmushroom_FLAG_Animation_Finished = 0x1,
	SHmushroom_FLAG_Follow_Curve = 0x2,
	SHmushroom_FLAG_No_Fleeing = 0x4,
	SHmushroom_FLAG_Moving = 0x8,
	SHmushroom_FLAG_Hurt = 0x10
} SHmushroom_Flags;

typedef enum {
	SHmushroom_STATE_0_Idle = 0,            //Hanging around
	SHmushroom_STATE_1_Jump = 1,            //Fleeing from player/sidekick
	SHmushroom_STATE_2_Collected = 2,       //NOTE: plays unseen dancing animation
	SHmushroom_STATE_3_Alert_Intro = 3,     //Alert: spotting threat
	SHmushroom_STATE_4_Alert = 4,           //Alert: watching threat
	SHmushroom_STATE_5_Surprised_Hop = 5,   //Alert: hopping in surprise (when player/sidekick sneak up during alert state)
	SHmushroom_STATE_6_Trapped = 6,         //Can't flee, can be collected immediately (possibly intended for trapping White Mushroom with cave stalectites?)
	SHmushroom_STATE_7_Alert_Outro = 7,   	//Alert: "sighing" with relief when player/sidekick back off
	SHmushroom_STATE_8_Hidden = 8,          //Mushroom invisible after being collected/eaten (NOTE: plays unseen dancing animation)
	SHmushroom_STATE_9_Stunned = 9          //Stunned after attack
} SHmushroom_States;

typedef enum {
	SHmushroom_MODANIM_0_Idle_LOOP = 0,      //Spinning head in small circles
	SHmushroom_MODANIM_1_Jump = 1,           //90 degree root turn to right, anticipation squash, big jump (including root motion in Y), and bounced landing (one-shot)
	SHmushroom_MODANIM_2_Look_Intro = 2,     //Tilting "neck" back as though trying to get a better look (one-shot)
	SHmushroom_MODANIM_3_Look_Idle_LOOP = 3, //Continuing from [mAnim2], head tilted back, staying very still.
	SHmushroom_MODANIM_4_Look_Hop_LOOP = 4,  //Continuing from [mAnim3], hop in place as though surprised.
	SHmushroom_MODANIM_5_Sigh = 5,           //"Sigh" of relief after player/sidekick back away: "neck" squashes down slightly, then back up (one-shot)
	SHmushroom_MODANIM_6_Dancing_LOOP = 6,   //Dancing/spraying spores? Root leaves ground during anim, so this is different from Red Mushrooms' spore attack anim. (Plays while collected, but can't be seen!)
	SHmushroom_MODANIM_7_Stunned_LOOP = 7    //Spinning head in wide dizzy circles
} SHmushroom_ModAnim;

#endif // _DLLS_502_H
