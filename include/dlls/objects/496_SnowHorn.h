#ifndef _DLLS_496_H
#define _DLLS_496_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct{
/*0x10*/ ObjSetup base;
/*0x18*/ s16 playerNearbyRange; //`SnowHorn_FLAG_80_Player_Nearby` is set when the player is inside a multiple of this range (not used for anything, though)
/*0x1A*/ s16 squirtInverval;    //For the Blue SnowHorn: the time (in seconds) between squirting water at the player
/*0x1C*/ s8 yaw;
/*0x1D*/ s8 characterIdx;       //Which SnowHorn this is (see `SnowHorn_CharacterIndices`)
} SnowHorn_Setup;

typedef enum {
    //Bank 0
    SnowHorn_MODANIM0_0_Idle_LOOP = 0,               //Bobbling head merrily
    SnowHorn_MODANIM0_1_Squirt,                      //Sucking up water, then squirting
    SnowHorn_MODANIM0_2_Talk_LOOP,                   //Flapping trunk around
    SnowHorn_MODANIM0_3_Walk_LOOP,                   //Walk cycle
    SnowHorn_MODANIM0_4_Sleep_Intro,                 //Curling up for a snooze
    SnowHorn_MODANIM0_5_Sleep_LOOP,                  //Snoring gently
    SnowHorn_MODANIM0_6_Sleep_Outro,                 //Waking up and stretching
    SnowHorn_MODANIM0_7_Sleep_Outro_Alt,             //Waking up without stretching (unused?)
    SnowHorn_MODANIM0_8_Resting_Intro,               //Curling up, but looking up instead of sleeping (unused?)
    SnowHorn_MODANIM0_9_Resting_LOOP,                //Similar to the sleep pose, but sitting up attentively
    SnowHorn_MODANIM0_10_Resting_LOOP_FLIPPED,       //Mirrored version of the resting loop
    SnowHorn_MODANIM0_11_Shaking_Off,                //Spinning torso to shake off snow
    SnowHorn_MODANIM0_12_Turn_Walk_LOOP,             //Turn loop, maybe?
    SnowHorn_MODANIM0_13_Trunk_Grab,                 //Swiping Alpine Root from Sabre's hands
    SnowHorn_MODANIM0_14_Trunk_Sweep,                //Reaching behind and to the left with trunk as though grabbing something, then sweeping to the right
    SnowHorn_MODANIM0_15_Sneeze,                     //Bracing on all 4s, building up to a sneeze (unused?)
    SnowHorn_MODANIM0_16_Look_Right,                 //Turning to the right, with trunk settle
    SnowHorn_MODANIM0_17_Stretch_Up_Intro,           //Stretching foreleg and trunk widely (Garunda Te escaping ice?)
    SnowHorn_MODANIM0_18_Stretch_Up_Outro,           //End of stretch, with trunk settle
    SnowHorn_MODANIM0_19_Fallen_Idle_LOOP,           //Collapsed onto side (DarkIce Mines' famished SnowHorn)
    SnowHorn_MODANIM0_20_Fallen_Talk_Intro,          //Looking up to chat
    SnowHorn_MODANIM0_21_Fallen_Talk_LOOP,           //Chatting and gesturing with trunk
    SnowHorn_MODANIM0_22_Fallen_Talk_Idle_LOOP,      //Listening
    SnowHorn_MODANIM0_23_Fallen_Eat,                 //Eating an Alpine Root
    SnowHorn_MODANIM0_24_Fallen_Stand_Intro,         //Getting up
    SnowHorn_MODANIM0_25_Fallen_Stand_Outro,         //Getting up, into neutral pose
    SnowHorn_MODANIM0_26_Sleep_Talk_LOOP,            //Talking in their sleep (unused?)
    SnowHorn_MODANIM0_27_Sit_Talk_LOOP,              //Talking while sitting up (unused?)
    SnowHorn_MODANIM0_28_Backing_Away_Scared,        //Walking backwards while trunk twitches nervously (unused?)
    SnowHorn_MODANIM0_29_Trunk_Flick_Right,          //Craning head to the right and flicking/grabbing with their trunk?
    SnowHorn_MODANIM0_30_Falling_and_Landing,        //Seems to be dropping off a ledge and landing below?
    SnowHorn_MODANIM0_31_Bow,                        //Craning trunk up, and then dipping into a deep bow (unused?)
    SnowHorn_MODANIM0_32_Garunda_Smash_Ice_Intro,    //On sloped ground, winding up in anticipation of smashing through the ice
    SnowHorn_MODANIM0_33_Garunda_Smash_Ice_Outro,    //Smashing through the ice with his tusks
    SnowHorn_MODANIM0_34_Garunda_Awkward_Pose,       //No animation, on sloped ground, contorted as though frozen in ice? Trunk is slightly off fitting through the crack in Garunda's icy prison (maybe an early draft of mAnim0_35?)
    SnowHorn_MODANIM0_35_Garunda_Trapped_Idle_LOOP,  //On sloped ground, waiting with trunk sticking out through a crack in the ice
    SnowHorn_MODANIM0_36_Garunda_Trapped_Drop_Trunk,                //(Root shifted vertically!) From [mAnim0_35], withdraws trunk back down under the ice (unused?)
    SnowHorn_MODANIM0_37_Garunda_Trapped_Drop_Trunk_Idle_LOOP,      //(Root shifted vertically!) From [mAnim0_36], swings trunk in idle cycle (unused?)
    SnowHorn_MODANIM0_38_Garunda_Awkward_Idle_LOOP,  //Same pose as [mAnim0_34], but with subtle trunk movements (maybe an early draft of mAnim0_35?)
    SnowHorn_MODANIM0_39_Garunda_Awkward_Trunk_Nod,  //From [mAnim0_38], bobs trunk back and forward as though talking
    SnowHorn_MODANIM0_40_Dejected_Sigh,              //In standing pose, flops head sadly to the left
    SnowHorn_MODANIM0_41_Garunda_Trapped_Eat,        //From [mAnim0_35], brings trunk back through the ice and to his mouth, then returns to initial pose
    //NOTE: Garunda Te (OBJ_NWmammothguardi)'s modAnim bank0 ends here and doesn't include the next 7 animations
    SnowHorn_MODANIM0_42_Flinch_Up,                  //Reels head backwards, then returns to neutral standing pose
    SnowHorn_MODANIM0_43_Flinch_Left,                //Reels head to the left, then returns to neutral standing pose
    SnowHorn_MODANIM0_44_Flinch_Right,               //Reels head to the right, then returns to neutral standing pose
    SnowHorn_MODANIM0_45_Flinch_Down_L,              //Reels head downwards and lifts left foreleg in pain, then returns to neutral standing pose
    SnowHorn_MODANIM0_46_Flinch_Down_R,              //Reels head downwards and lifts right foreleg in pain, then returns to neutral standing pose
    SnowHorn_MODANIM0_47_Flinch_Neutral,             //Reels head dizzily, then returns to neutral standing pose
    SnowHorn_MODANIM0_48_Counterattack,              //Swings tusks wildly to the right, then left, then returns to neutral standing pose (unused, but referenced in unused field of hitSphere data - maybe intended as a Baddie attack reaction, or after the player attacks too many times?)

    //Bank 1
    SnowHorn_MODANIM1_0_Talk_LOOP = 0x100,           //Flapping trunk around in standing pose (similar to [mAnim0_2])
    SnowHorn_MODANIM1_1_Trunk_Toot,                  //Raising trunk into the air, in standing pose
    SnowHorn_MODANIM1_2_Walk_LOOP,                   //Walk cycle (similar to [mAnim0_3])
    SnowHorn_MODANIM1_3_Idle_Fidget,                 //Shuffles feet, then reaches behind with trunk to scratch back (used by DIMSnowHorn1)
    SnowHorn_MODANIM1_4_Peer_Right_Intro,            //Leaning down slightly and peering to the right
    SnowHorn_MODANIM1_5_Peer_Right_Idle_LOOP,        //From [mAnim1_4], idle cycle
    SnowHorn_MODANIM1_6_Peer_Right_Outro,            //From [mAnim1_5], returning to neutral standing pose

    //Bank 2
    SnowHorn_MODANIM2_0_Turn_Right_LOOP = 0x200, //Turning on the spot
    SnowHorn_MODANIM2_1_Turn_Left_LOOP,          //Turning on the spot
    SnowHorn_MODANIM2_2_Sidestep_Left_LOOP,      //Scooting over towards mount platform
    SnowHorn_MODANIM2_3_Sidestep_Right_LOOP,     //Scooting over towards mount platform
    SnowHorn_MODANIM2_4_Tusk_Attack,             //A powerful tusk swing
    SnowHorn_MODANIM2_5_Sit_Idle_LOOP,           //Sitting in a cute loaf pose, with legs tucked underneath body
    SnowHorn_MODANIM2_6_Sit_Intro,               //Wiggling hind legs and settling into a loaf pose [mAnim2_5]
    SnowHorn_MODANIM2_7_Sit_Outro,               //From [mAnim2_5], Getting back up into neutral standing pose
    SnowHorn_MODANIM2_8_Walk_Intro,              //Stepping forward onto left foreleg
    SnowHorn_MODANIM2_9_Walk_Outro_L,            //Starting off with left foreleg in contact pose, settles into a neutral standing pose
    SnowHorn_MODANIM2_10_Walk_Outro_R,           //Starting off with right foreleg in contact pose, settles into a neutral standing pose
    SnowHorn_MODANIM2_11_Gallop_and_Stop,        //Starting in mid-gallop, takes a few bounds and then comes to a sudden skidding halt (called by Horn of Truth)
    SnowHorn_MODANIM2_12_Wheel_Walk_LOOP,        //A poor SnowHorn stuck operating DarkIce Mines' wheels
    SnowHorn_MODANIM2_13_Offering_Item,          //Reaching behind and to the left with their trunk, and then offering an item
    SnowHorn_MODANIM2_14_Lifting_Trunk_Intro,    //Crouching down on forelegs, and laying trunk flat out ahead
    SnowHorn_MODANIM2_15_Lifting_Trunk_Outro,    //From [mAnim2_14], pushing down on forelegs and lifting their trunk high up (lifting an object, maybe?)
    SnowHorn_MODANIM2_16_Ground_Slam             //Rearing up on hind legs, and slamming back heavily to the ground (Grumpy SnowHorn stopping geyser)
} SnowHorn_ModAnims;

#endif // _DLLS_496_H
