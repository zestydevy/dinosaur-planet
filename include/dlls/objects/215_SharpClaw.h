#ifndef _DLLS_215_H
#define _DLLS_215_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

//NOTE: SharpClaw (Orange) and SnowClaw (Blue) have slightly different MODANIM lists!
typedef enum {
    //Bank 0 (Noncombat?)
    SharpClaw_MODANIM0_0_Idle_LOOP = 0,         //Standing with arms loose and mouth agape
    SharpClaw_MODANIM0_1_Patrol_LOOP,           //A patrolling walk cycle, with weapon held up in right hand and slung over shoulder
    SharpClaw_MODANIM0_2_Walk_LOOP,             //A more casual walk cycle, with arms loose
    SharpClaw_MODANIM0_3_Run_LOOP,              //Bounding on all fours
    SharpClaw_MODANIM0_4_Fast_Walk_LOOP,        //Similar to [mAnim0_2], but with softer timing on the contacts
    SharpClaw_MODANIM0_5_Jump,                  //Winding up into big jump, with a landing and settle      
    SharpClaw_MODANIM0_6_Prone_Peering,         //Lying prone, with weight resting on arms/elbows on a slightly elevated surface. Peering out as though over a step/into water? Flicks weapon slightly.
    SharpClaw_MODANIM0_7_Prone_Bop_L,           //From [mAnim0_6], looks to the left, then raises weapon overhead and bops it to the left
    SharpClaw_MODANIM0_8_Prone_Bop_R,           //From [mAnim0_6], looks to the right, then raises weapon overhead and bops it to the right
    SharpClaw_MODANIM0_9_Prone_Fidget,          //From [mAnim0_6], looks up briefly while shifting weapon in right hand
    SharpClaw_MODANIM0_10_Die_Tree_of_Life,     //The old death animation seen in the One Hour Footage (and still used by CRF's prison guard) - lifted limply into the air, then spins and dissipates
    SharpClaw_MODANIM0_11_Knocked_onto_Back,    //Sent flying backwards through the air, then flops to the floor on landing. Maybe an alternate death animation?
    SharpClaw_MODANIM0_12_Hips_Idle_LOOP,       //Standing square, with hands on hips and subtle breathing motion.
    SharpClaw_MODANIM0_13_Hips_Walk_LOOP,       //From [mAnim0_12], taking small steps forward, leading with right foot.
    SharpClaw_MODANIM0_14_Hips_Backstep_LOOP,   //From [mAnim0_12], taking small steps backwards, leading with left foot.
    SharpClaw_MODANIM0_15_Hips_Sidestep_R_LOOP, //From [mAnim0_12], taking small steps to the right, leading with right foot.
    SharpClaw_MODANIM0_16_Hips_Sidestep_L_LOOP, //From [mAnim0_12], taking small steps to the left, leading with left foot.
    SharpClaw_MODANIM0_17_Hips_Arm_Raise_Intro, //From [mAnim0_12], raising right hand and holding it up with palm out. Presenting something, or interacting with a mechanism?
    SharpClaw_MODANIM0_18_Hips_Arm_Raise_HOLD,  //From [mAnim0_17], no animation, just holding on end-pose with right hand held up.
    SharpClaw_MODANIM0_19_Search_LOOP,          //Shielding eyes with left hand, looks around in search of a target
    SharpClaw_MODANIM0_20_Patrol_U_Turn,        //From [mAnim0_1], looks over left shoulder and does an 180 degree turn
    SharpClaw_MODANIM0_21_Club_Idle_Fidget,     //From [mAnim0_22], with club slung over right shoulder, lifts it up slightly and uses it to scratch back
    SharpClaw_MODANIM0_22_Club_Idle_LOOP,       //Standing with club slung over right shoulder, breathing heavily
    SharpClaw_MODANIM0_23_Club_Idle_Search_L,   //From [mAnim0_22], drops club, then peers up and to the left while shielding eyes with left hand. Settles back to neutral at end.
    SharpClaw_MODANIM0_24_Club_Idle_Search_R,   //From [mAnim0_22], drops club, then peers up and to the right while shielding eyes with club. Settles back to neutral at end.
    SharpClaw_MODANIM0_25_Sneak_LOOP,           //Sneaking forward clumsily, with arms raised
    SharpClaw_MODANIM0_26_Bike_Idle_LOOP,       //Jetbike: hunched over the steering in a neutral pose  [ROOT Y SHIFTED]
    SharpClaw_MODANIM0_27_Bike_Turn_R_LOOP,     //Jetbike: leaning heavily into a right turn            [ROOT Y SHIFTED]
    SharpClaw_MODANIM0_28_Bike_Turn_L_LOOP,     //Jetbike: leaning heavily into a left turn             [ROOT Y SHIFTED]
    SharpClaw_MODANIM0_29_Club_Look_R_Intro,    //Starting from a hunched standing pose, rests end of weapon in left off-hand, then looks/swivels to the right
    SharpClaw_MODANIM0_30_Club_Look_L_Intro,    //Starting from a hunched standing pose, rests end of weapon in left off-hand, then looks/swivels to the left
    SharpClaw_MODANIM0_31_Club_Look_R_LOOP,     //From [mAnim0_29], continues looking right while breathing
    SharpClaw_MODANIM0_32_Club_Look_L_LOOP,     //From [mAnim0_30], continues looking left while breathing
    //NOTE: the rest of the animations in this bank differ from the blue SnowClaws'
    SharpClaw_MODANIM0_33_Swagger_Walk_R_LOOP,  //[SHARPCLAW ONLY] Walking upright, leading with right foot, swinging left arm confidently with elbow leading, and bringing weapon up to right shoulder at the top of the right arm's swing
    SharpClaw_MODANIM0_34_Swagger_Walk_L_LOOP,  //[SHARPCLAW ONLY] Walking upright, leading with left foot, swinging both arms confidently with fist leading. Seems to be the left equivalent of [mAnim0_33], but it's a subtly different animation where the arms mostly swing low.
    SharpClaw_MODANIM0_35_Carry_Forward_LOOP,   //[SHARPCLAW ONLY] From [mAnim0_36], standing with arms braced wide, rolling head curiously [ROOT Z SHIFTED FORWARD]
    SharpClaw_MODANIM0_36_Carry_Forward_Intro,  //[SHARPCLAW ONLY] Takes two steps forward leading with right foot, stands with arms braced wide (carrying Warp Crystal under left arm) [ROOT Z SHIFTS FORWARD]
    SharpClaw_MODANIM0_37_Carry_Forward_Flinch, //[SHARPCLAW ONLY] From [mAnim0_35], flinches back as though hit lightly, then settles back to initial pose [ROOT Z SHIFTED FORWARD]
    SharpClaw_MODANIM0_38_Carry_Forward_Guard,  //[SHARPCLAW ONLY] From [mAnim0_35], raises right elbow up and ahead into a defensive pose [ROOT Z SHIFTED FORWARD]
    SharpClaw_MODANIM0_39_Carry_Forward_Check,  //[SHARPCLAW ONLY] From [mAnim0_35], looks down twice under left arm, as though checking what they're carrying [ROOT Z SHIFTED FORWARD]
    SharpClaw_MODANIM0_40_Detect_Intruder,      //[SHARPCLAW ONLY] From a neutral standing pose, glances left suddenly "Grah! Intruder!" (Used during Sabre's first WM visit)
    SharpClaw_MODANIM0_41_Carry_LOOP,           //[SHARPCLAW ONLY] A walk cycle, carrying something heavy (barrels etc.) at hip level
    SharpClaw_MODANIM0_42_Carry_Drop,           //[SHARPCLAW ONLY] From [mAnim0_41], takes two more steps and then lowers cargo to the floor [ROOT Z SHIFTS FORWARD]
    SharpClaw_MODANIM0_43_Carry_Lift,           //[SHARPCLAW ONLY] From [mAnim0_42], squats down and then slowly lifts cargo up to hip level [ROOT Z SHIFTED FORWARD]
    SharpClaw_MODANIM0_44_Startled,             //[SHARPCLAW ONLY] Starts off looking down, then pops up to look around (DF SharpClaw reacting to barrel explosion)
    SharpClaw_MODANIM0_45_Look_Behind,          //[SHARPCLAW ONLY] Starts off wiping forehead with arm, then turns to look back over left shoulder (DF SharpClaw reacting to barrel explosion)
    SharpClaw_MODANIM0_46_Look_Behind_Outro,    //[SHARPCLAW ONLY] From [mAnim0_45], turns away from left shoulder back to facing forward, then contorts face funnily (DF SharpClaw reacting to barrel explosion)
    SharpClaw_MODANIM0_47_Startled_Outro,       //[SHARPCLAW ONLY] From [mAnim0_44], returns to looking forward, flopping head down mournfully (DF SharpClaw reacting to barrel explosion) 
    SharpClaw_MODANIM0_48_Laugh,                //[SHARPCLAW ONLY] Holding belly while chortling away (DF SharpClaw reacting to explosion)
    SharpClaw_MODANIM0_49_Punch,                //[SHARPCLAW ONLY] Swinging right fist anticlockwise (used when rousing MMP's sleeping SharpClaw)
    SharpClaw_MODANIM0_50_Laugh_Lolling,        //[SHARPCLAW ONLY] Lolling head back and forth while laughing and clubbing the ground, jaw slightly askew (DF SharpClaw reacting to explosion?)
    SharpClaw_MODANIM0_51_Ordering_Intro,       //[SHARPCLAW ONLY] Stomping forward then turning around snappily
    SharpClaw_MODANIM0_52_Ordering_Middle,      //[SHARPCLAW ONLY] From [mAnim0_51], waves arm at other SharpClaw: "Grah! Step. On. Switch!"
    SharpClaw_MODANIM0_53_Ordering_Outro,       //[SHARPCLAW ONLY] From [mAnim0_52], slowly swivels back around the way they were facing

    //Bank 1 (Combat?)
    SharpClaw_MODANIM1_0_Hunched_LOOP = 0x100,    //Hunched idle cycle, with end of weapon resting in off-hand
    SharpClaw_MODANIM1_1_Die,                     //Holding left hand to chest, then keeling over backwards
    SharpClaw_MODANIM1_2_Hop_Forward,             //Leading with left foot, gains on foe with a sudden hop
    SharpClaw_MODANIM1_3_Hop_Backward,            //Leading with right foot, distances self with a sudden hop
    SharpClaw_MODANIM1_4_Hop_L,                   //Leading with left foot, sidesteps/strafes to the left with a sudden hop
    SharpClaw_MODANIM1_5_Hop_R,                   //Leading with right foot, sidesteps/strafes to the right with a sudden hop
    SharpClaw_MODANIM1_6_Attack_Anticlockwise,    //Standing in place, swings weapon in an anticlockwise arc
    SharpClaw_MODANIM1_7_Attack_Clockwise,        //Standing in place, swings weapon in a clockwise arc
    SharpClaw_MODANIM1_8_Attack_Overhead,         //Standing in place, lifts weapon up above head before smashing it down with both hands
    SharpClaw_MODANIM1_9_Attack_Forward,          //Standing in place, lifts weapon slightly and then bops it forward single-handedly (unused?)
    SharpClaw_MODANIM1_10_Flinch_L,               //Reeling to the left after being hit
    SharpClaw_MODANIM1_11_Flinch_R,               //Reeling to the right after being hit
    SharpClaw_MODANIM1_12_Flinch_Back,            //Reeling backwards after being hit
    SharpClaw_MODANIM1_13_Flinch_Neutral,         //Reeling head after being hit
    SharpClaw_MODANIM1_14_Spin_Attack_Intro,      //Winds up into a big anticipation pose, then does a full 360 anticlockwise weapon swing (Unused!)
    SharpClaw_MODANIM1_15_Club_Raise_Intro,       //In standing pose, lifts club overhead with both hands and holds it there (without running)
    SharpClaw_MODANIM1_16_Club_Chase_LOOP,        //From [mAnim1_17], a run cycle while holding the club overhead in both hands (Unused!)
    SharpClaw_MODANIM1_17_Club_Chase_Intro,       //From a parrying pose(?), lifts the club overhead in both hands and begins a run cycle (Unused!)
    SharpClaw_MODANIM1_18_Club_Chase_Outro,       //From [mAnim1_16], leaps out of the club-wielding run cycle and smashes the club to the ground. Impact vibration, then settles back to neutral pose. (Unused!)
    SharpClaw_MODANIM1_19_Spin_Attack_LOOP,       //From [mAnim1_14], continues spinning in a wide circle (Unused!)
    SharpClaw_MODANIM1_20_Club_Raise_LOOP,        //Similar to [mAnim1_15]'s end-pose, continues holding the club straight up with mouth agape (Unused!)
    SharpClaw_MODANIM1_21_Guard,                  //Holds weapon ahead defensively, and gradually dips head down
    SharpClaw_MODANIM1_22_Crash_Landing_Intro,    //Flying through the air for a while, then curling up when colliding with a vertical surface (Unused?)
    SharpClaw_MODANIM1_23_Crash_Landing_Outro,    //From [mAnim1_22], crashes backwards, bounces off the ground, then lies supine
    SharpClaw_MODANIM1_24_Fight_Walk_LOOP,        //Hunched forward walk cycle, leading with right foot, with weapon at the ready
    SharpClaw_MODANIM1_25_Hop_and_Overhead_Swing_LOOP, //Raises weapon overhead, then hops and swings it down in an anticlockwise swing
    SharpClaw_MODANIM1_26_Club_Attack_Overhead,   //Similar to [mAnim1_18], but a single animation of raising the club, crashing it down, impact vibrating, settling back to neutral
    SharpClaw_MODANIM1_27_Carry_Throw_LOOP,       //From a similar pose to [mAnim0_41], 
    SharpClaw_MODANIM1_28_Jump_Forward_Attack,    //Anticipates backwards slightly, then launches into a big forward jump, swinging the weapon in a wild rising anticlockwise arc on landing
    SharpClaw_MODANIM1_29_Jump_Forward,           //Similar to [mAnim1_28], but without swinging the weapon
    SharpClaw_MODANIM1_30_Wall_Spring_Intro,      //From [mAnim1_0], dips hips down slightly and begins springing up
    SharpClaw_MODANIM1_31_Wall_Spring_Middle,     //From [mAnim1_30], leans body to the left while springing through the air. Brings right arm up and plants feet as though against a wall on the right? (Slightly jerky/unfinished-looking)
    SharpClaw_MODANIM1_32_Wall_Spring_Outro,      //From [mAnim1_31], lands from jump and settles back into neutral
    SharpClaw_MODANIM1_33_Back_Scratch_LOOP,      //Similar to [mAnim0_21], but a loop of the middle section where the SharpClaw scratches their back with the club
    SharpClaw_MODANIM1_34_Standing_LOOP,          //Standing slack-jawed
    SharpClaw_MODANIM1_35_Taunt_Intro,            //In hunched standing pose, talks and pats the weapon's end menacingly against their off-hand palm
    SharpClaw_MODANIM1_36_Hunched_Ready_LOOP,     //Similar to [mAnim1_0], but in a slightly deeper hunch
    SharpClaw_MODANIM1_37_Hunched_Turn_90_R,      //From [mAnim1_0], turns on the spot to face 90 degrees to the right [ROOT YAW SHIFTED 90 ANTICLOCKWISE AT START]
    SharpClaw_MODANIM1_38_Hunched_Turn_90_L,      //From [mAnim1_0], turns on the spot to face 90 degrees to the left [ROOT YAW SHIFTED 90 CLOCKWISE AT START]
    SharpClaw_MODANIM1_39_Hunched_Strafe_L_LOOP,  //From [mAnim1_0], leading with right foot, strafes to the left while facing target
    SharpClaw_MODANIM1_40_Hunched_Strafe_L_Guard_LOOP,  //Similar to [mAnim1_39], but holds weapon in front of face [ROOT SHIFTS LEFT]
    SharpClaw_MODANIM1_41_Hunched_Guard_LOOP,     //Similar to [mAnim1_0], but guarding face with weapon 
    SharpClaw_MODANIM1_42_Knocked_Over_Intro,     //Flung backwards, landing with a bounce onto rear (after being shot by Projectile Spell, etc.)
    SharpClaw_MODANIM1_43_Knocked_Over_Outro,     //From [mAnim1_43], picks self back up into neutral hunch pose
    SharpClaw_MODANIM1_44_Hunched_Turn_Left,      //Leading with left foot, a slight turn/strafe left
    SharpClaw_MODANIM1_45_Taunt,                  //From [mAnim1_35], talking while punching the air and shaking fist
    SharpClaw_MODANIM1_46_Taunt,                  //From [mAnim1_35], talking while patting weapon
    SharpClaw_MODANIM1_47_Hunched_Strafe_R,       //Similar to [mAnim1_39], leading with right foot, strafing to the right
    
    //Bank 2 (Gameplay Sequences?)
    SharpClaw_MODANIM2_0_Exercise_LOOP = 0x200,     //Starting with arms huddled to chest, swings arms to warm self
    SharpClaw_MODANIM2_1_Campfire_Idle_LOOP,        //Huddled with hands close to chest
    SharpClaw_MODANIM2_2_Campfire_Fidget_LOOP,      //Stepping on the spot
    SharpClaw_MODANIM2_3_Prison_Guard_Sleep_LOOP,   //CRF Prison Guard asleep with arms and head on desk
    SharpClaw_MODANIM2_4_Prison_Guard_Awake_Intro,  //Guard waking and resting head in right hand
    SharpClaw_MODANIM2_5_Prison_Guard_Awake_LOOP,   //Guard resting head in right head dozily
    //NOTE: the rest of the animations in this bank differ from the blue SnowClaws'
    SharpClaw_MODANIM2_6_Campfire_Cold_LOOP,        //[SHARPCLAW ONLY] Huddling arms to chest to keep self warm
    SharpClaw_MODANIM2_7_Campfire_Warm_Hands_HOLD,  //[SHARPCLAW ONLY] From [mAnim2_8], no animation, holding hands out to warm them
    SharpClaw_MODANIM2_8_Campfire_Warm_Hands_Intro, //[SHARPCLAW ONLY] From [mAnim2_1], holds out hands to warm them
    SharpClaw_MODANIM2_9_Campfire_Warm_Hands_Outro, //[SHARPCLAW ONLY] From [mAnim2_7], brings hands back to chest
    SharpClaw_MODANIM2_10_Campfire_Warm_Arms,       //[SHARPCLAW ONLY] From [mAnim2_1], rubbing arms to warm them
    SharpClaw_MODANIM2_11_Campfire_Search,          //[SHARPCLAW ONLY] From [mAnim2_1], stops to look over right shoulder (no outro?)
    SharpClaw_MODANIM2_12_Scoop_LOOP,               //[SHARPCLAW ONLY] Holding weapon in both hands (off-hand supporting end), scoops it ahead as though through water (fishing, or shovelling snow?)
    SharpClaw_MODANIM2_13_Scoop_Idle_LOOP,          //[SHARPCLAW ONLY] Similar to [mAnim2_12], an idle cycle where the SharpClaw peers down while holding their weapon (fishing, or shovelling snow?)
    
    //Bank 3 (Cutscene Sequences?) (NOTE: the animations in this bank differ from the blue SnowClaws')
    SharpClaw_MODANIM3_0_Lifting_Intro = 0x300, //[SHARPCLAW ONLY] Picking up a heavy object, then turning anticlockwise while lifting the object above their head (unused?)
    SharpClaw_MODANIM3_1_Lifting_Outro,         //[SHARPCLAW ONLY] From [mAnim3_0], squats and settles into place
    SharpClaw_MODANIM3_2_Drop_Object,           //[SHARPCLAW ONLY] Taking a step forward onto right foot while carrying something heavy at waist level, then tips it forward and lets it go (DF SharpClaw dropping food bag?)
    SharpClaw_MODANIM3_3_Telescope_LOOP,        //[SHARPCLAW ONLY] From [mAnim3_4], MMP SharpClaw peering through telescope
    SharpClaw_MODANIM3_4_Telescope_Intro,       //[SHARPCLAW ONLY] MMP SharpClaw starts peering through telescope
    SharpClaw_MODANIM3_5_Telescope_Intro_HOLD,  //[SHARPCLAW ONLY] No animation, initial pose of [mAnim3_4]
    SharpClaw_MODANIM3_6_Asleep_Sitting_LOOP,   //[SHARPCLAW ONLY] MMP SharpClaw sleeping at their post
    SharpClaw_MODANIM3_7_Telescope_Outro,       //[SHARPCLAW ONLY] From [mAnim3_3], MMP SharpClaw lowers telescope and faces the other sleeping SharpClaw
    SharpClaw_MODANIM3_8_Asleep_Rouse,          //[SHARPCLAW ONLY] From [mAnim3_6], waking up
    SharpClaw_MODANIM3_9_Gaze_Up_R,             //[SHARPCLAW ONLY] DF Shrine patrolling SharpClaw looking up
    SharpClaw_MODANIM3_10_Head_Scratch,         //[SHARPCLAW ONLY] DF Shrine patrolling SharpClaw placing hands on hips and scratching their head
    SharpClaw_MODANIM3_11_Hammering_Club,       //Raising the club and pounding it against a nearby surface (yaw interpolation issues midway) (unused?) [ROOT ROTATED 180]
    SharpClaw_MODANIM3_12_Strut_Forward_LOOP,   //[SHARPCLAW ONLY] Strutting forward quickly in upright posture
    SharpClaw_MODANIM3_13_Bike_Road_Rage_Intro, //[SHARPCLAW ONLY] Jetbike: reels from an impact, then stands up with left arm/foot still steering and shakes fist angrily to the right (Euler lerp problems on right hand during reel) [ROOT Y SHIFTED]
    SharpClaw_MODANIM3_14_Bike_Road_Rage_LOOP,  //[SHARPCLAW ONLY] Jetbike: from [mAnim3_13], moving hold of the end-pose, with right hand still waving subtly in the air [ROOT Y SHIFTED]
    SharpClaw_MODANIM3_15_Meddle_Intro,         //[SHARPCLAW ONLY] Sidestepping left, with hands held up as though pressed up to a wall, then leaning and reaching out left to meddle with something (unused?)
    SharpClaw_MODANIM3_16_Meddle_Outro,         //[SHARPCLAW ONLY] From [mAnim3_15], continues meddling with something, then suddenly gets hit under chin and falls back in the direction they approached from, flat onto their back (unused?)
    SharpClaw_MODANIM3_17_Bike_Mount_Leap,      //[SHARPCLAW ONLY] Jetbike: vaults into riding pose from behind the jetbike
    SharpClaw_MODANIM3_18_Bike_Mount_Step,      //[SHARPCLAW ONLY] Jetbike: stepping forward into riding pose [ROOT Z SHIFTS FORWARD]
    SharpClaw_MODANIM3_19_Cower_LOOP,           //[SHARPCLAW ONLY] From [mAnim3_21], head slung low, with hands raised in posture of surrender (DF food bag SharpClaw)
    SharpClaw_MODANIM3_20_Clower_Outro,         //[SHARPCLAW ONLY] From [mAnim3_20], slumps head and arms down further
    SharpClaw_MODANIM3_21_Cower_Intro,          //[SHARPCLAW ONLY] Slumps head down, then raises hands in surrender
    SharpClaw_MODANIM3_22_Jump_Reach_LOOP,      //[SHARPCLAW ONLY] Jumping on the spot, trying to reach something above them (unused?)
    SharpClaw_MODANIM3_23_Eat,                  //[SHARPCLAW ONLY] DR SharpClaw: grabbing Energy Egg from Sabre, then stuffing it in mouth and burping
    SharpClaw_MODANIM3_24_Flip_Switch_R,        //[SHARPCLAW ONLY] DR SharpClaw: resetting the switch to their right
    SharpClaw_MODANIM3_25_Beckon_LOOP           //Looking way up, beckoning and signalling to a spaceship
} SharpClaw_ModAnims;

typedef enum {
    //Bank 0 (Noncombat?)
    SnowClaw_MODANIM0_0_Idle_LOOP = 0,         //Standing with arms loose and mouth agape
    SnowClaw_MODANIM0_1_Patrol_LOOP,           //A patrolling walk cycle, with weapon held up in right hand and slung over shoulder
    SnowClaw_MODANIM0_2_Walk_LOOP,             //A more casual walk cycle, with arms loose
    SnowClaw_MODANIM0_3_Run_LOOP,              //Bounding on all fours
    SnowClaw_MODANIM0_4_Walk_LOOP,             //Similar to [mAnim0_2], but with softer timing on the contacts (maybe an earlier draft?)
    SnowClaw_MODANIM0_5_Jump,                  //Winding up into big jump, with a landing and settle      
    SnowClaw_MODANIM0_6_Prone_Peering,         //Lying prone, with weight resting on arms/elbows on a slightly elevated surface. Peering out as though over a step/into water? Flicks weapon slightly.
    SnowClaw_MODANIM0_7_Prone_Bop_L,           //From [mAnim0_6], looks to the left, then raises weapon overhead and bops it to the left
    SnowClaw_MODANIM0_8_Prone_Bop_R,           //From [mAnim0_6], looks to the right, then raises weapon overhead and bops it to the right
    SnowClaw_MODANIM0_9_Prone_Fidget,          //From [mAnim0_6], looks up briefly while shifting weapon in right hand
    SnowClaw_MODANIM0_10_Die_Tree_of_Life,     //The old death animation seen in the One Hour Footage (and still used by CRF's prison guard) - lifted limply into the air, then spins and dissipates
    SnowClaw_MODANIM0_11_Knocked_onto_Back,    //Sent flying backwards through the air, then flops to the floor on landing. Maybe an alternate death animation?
    SnowClaw_MODANIM0_12_Hips_Idle_LOOP,       //Standing square, with hands on hips and subtle breathing motion.
    SnowClaw_MODANIM0_13_Hips_Walk_LOOP,       //From [mAnim0_12], taking small steps forward, leading with right foot.
    SnowClaw_MODANIM0_14_Hips_Backstep_LOOP,   //From [mAnim0_12], taking small steps backwards, leading with left foot.
    SnowClaw_MODANIM0_15_Hips_Sidestep_R_LOOP, //From [mAnim0_12], taking small steps to the right, leading with right foot.
    SnowClaw_MODANIM0_16_Hips_Sidestep_L_LOOP, //From [mAnim0_12], taking small steps to the left, leading with left foot.
    SnowClaw_MODANIM0_17_Hips_Arm_Raise_Intro, //From [mAnim0_12], raising right hand and holding it up with palm out. Presenting something, or interacting with a mechanism?
    SnowClaw_MODANIM0_18_Hips_Arm_Raise_HOLD,  //From [mAnim0_17], no animation, just holding on end-pose with right hand held up.
    SnowClaw_MODANIM0_19_Search_LOOP,          //Shielding eyes with left hand, looks around in search of a target
    SnowClaw_MODANIM0_20_Patrol_U_Turn,        //From [mAnim0_1], looks over left shoulder and does an 180 degree turn
    SnowClaw_MODANIM0_21_Club_Idle_Fidget,     //From [mAnim0_22], with club slung over right shoulder, lifts it up slightly and uses it to scratch back
    SnowClaw_MODANIM0_22_Club_Idle_LOOP,       //Standing with club slung over right shoulder, breathing heavily
    SnowClaw_MODANIM0_23_Club_Idle_Search_L,   //From [mAnim0_22], drops club, then peers up and to the left while shielding eyes with left hand. Settles back to neutral at end.
    SnowClaw_MODANIM0_24_Club_Idle_Search_R,   //From [mAnim0_22], drops club, then peers up and to the right while shielding eyes with club. Settles back to neutral at end.
    SnowClaw_MODANIM0_25_Sneak_LOOP,           //Sneaking forward clumsily, with arms raised
    SnowClaw_MODANIM0_26_Bike_Idle_LOOP,       //Jetbike: hunched over the steering in a neutral pose  [ROOT Y SHIFTED]
    SnowClaw_MODANIM0_27_Bike_Turn_R_LOOP,     //Jetbike: leaning heavily into a right turn            [ROOT Y SHIFTED]
    SnowClaw_MODANIM0_28_Bike_Turn_L_LOOP,     //Jetbike: leaning heavily into a left turn             [ROOT Y SHIFTED]
    SnowClaw_MODANIM0_29_Club_Look_R_Intro,    //Starting from a hunched standing pose, rests end of weapon in left off-hand, then looks/swivels to the right
    SnowClaw_MODANIM0_30_Club_Look_L_Intro,    //Starting from a hunched standing pose, rests end of weapon in left off-hand, then looks/swivels to the left
    SnowClaw_MODANIM0_31_Club_Look_R_LOOP,     //From [mAnim0_29], continues looking right while breathing
    SnowClaw_MODANIM0_32_Club_Look_L_LOOP,     //From [mAnim0_30], continues looking left while breathing
    //NOTE: the orange SharpClaws have additional animations in this bank

    //Bank 1 (Combat?)
    SnowClaw_MODANIM1_0_Hunched_LOOP = 0x100,        //Hunched idle cycle, with end of weapon resting in off-hand
    SnowClaw_MODANIM1_1_Die,                         //Holding left hand to chest, then keeling over backwards
    SnowClaw_MODANIM1_2_Hop_Forward,                 //Leading with left foot, gains on foe with a sudden hop
    SnowClaw_MODANIM1_3_Hop_Backward,                //Leading with right foot, distances self with a sudden hop
    SnowClaw_MODANIM1_4_Hop_L,                       //Leading with left foot, sidesteps/strafes to the left with a sudden hop
    SnowClaw_MODANIM1_5_Hop_R,                       //Leading with right foot, sidesteps/strafes to the right with a sudden hop
    SnowClaw_MODANIM1_6_Attack_Anticlockwise,        //Standing in place, swings weapon in an anticlockwise arc
    SnowClaw_MODANIM1_7_Attack_Clockwise,            //Standing in place, swings weapon in a clockwise arc
    SnowClaw_MODANIM1_8_Attack_Overhead,             //Standing in place, lifts weapon up above head before smashing it down with both hands
    SnowClaw_MODANIM1_9_Attack_Forward,              //Standing in place, lifts weapon slightly and then bops it forward single-handedly (unused?)
    SnowClaw_MODANIM1_10_Flinch_L,                   //Reeling to the left after being hit
    SnowClaw_MODANIM1_11_Flinch_R,                   //Reeling to the right after being hit
    SnowClaw_MODANIM1_12_Flinch_Back,                //Reeling backwards after being hit
    SnowClaw_MODANIM1_13_Flinch_Neutral,             //Reeling head after being hit
    SnowClaw_MODANIM1_14_Spin_Attack_Intro,          //Winds up into a big anticipation pose, then does a full 360 anticlockwise weapon swing (Unused!)
    SnowClaw_MODANIM1_15_Club_Raise_Intro,           //In standing pose, lifts club overhead with both hands and holds it there (without running)
    SnowClaw_MODANIM1_16_Club_Chase_LOOP,            //From [mAnim1_17], a run cycle while holding the club overhead in both hands (Unused!)
    SnowClaw_MODANIM1_17_Club_Chase_Intro,           //From a parrying pose(?), lifts the club overhead in both hands and begins a run cycle (Unused!)
    SnowClaw_MODANIM1_18_Club_Chase_Outro,           //From [mAnim1_16], leaps out of the club-wielding run cycle and smashes the club to the ground. Impact vibration, then settles back to neutral pose. (Unused!)
    SnowClaw_MODANIM1_19_Spin_Attack_LOOP,           //From [mAnim1_14], continues spinning in a wide circle (Unused!)
    SnowClaw_MODANIM1_20_Club_Raise_LOOP,            //Similar to [mAnim1_15]'s end-pose, continues holding the club straight up with mouth agape (Unused!)
    SnowClaw_MODANIM1_21_Guard,                      //Holds weapon ahead defensively, and gradually dips head down
    SnowClaw_MODANIM1_22_Crash_Landing_Intro,        //Flying through the air for a while, then curling up when colliding with a vertical surface (Unused?)
    SnowClaw_MODANIM1_23_Crash_Landing_Outro,        //From [mAnim1_22], crashes backwards, bounces off the ground, then lies supine
    SnowClaw_MODANIM1_24_Fight_Walk_LOOP,            //Hunched forward walk cycle, leading with right foot, with weapon at the ready
    SnowClaw_MODANIM1_25_Hop_and_Overhead_Swing_LOOP, //Raises weapon overhead, then hops and swings it down in an anticlockwise swing
    SnowClaw_MODANIM1_26_Club_Attack_Overhead,       //Similar to [mAnim1_18], but a single animation of raising the club, crashing it down, impact vibrating, settling back to neutral
    SnowClaw_MODANIM1_27_Carry_Throw_LOOP,           //From a similar pose to [mAnim0_41], 
    SnowClaw_MODANIM1_28_Jump_Forward_Attack,        //Anticipates backwards slightly, then launches into a big forward jump, swinging the weapon in a wild rising anticlockwise arc on landing
    SnowClaw_MODANIM1_29_Jump_Forward,               //Similar to [mAnim1_28], but without swinging the weapon
    SnowClaw_MODANIM1_30_Wall_Spring_Intro,          //From [mAnim1_0], dips hips down slightly and begins springing up
    SnowClaw_MODANIM1_31_Wall_Spring_Middle,         //From [mAnim1_30], leans body to the left while springing through the air. Brings right arm up and plants feet as though against a wall on the right? (Slightly jerky/unfinished-looking)
    SnowClaw_MODANIM1_32_Wall_Spring_Outro,          //From [mAnim1_31], lands from jump and settles back into neutral
    SnowClaw_MODANIM1_33_Back_Scratch_LOOP,          //Similar to [mAnim0_21], but a loop of the middle section where the SharpClaw scratches their back with the club
    SnowClaw_MODANIM1_34_Standing_LOOP,              //Standing slack-jawed
    SnowClaw_MODANIM1_35_Taunt_Intro,                //In hunched standing pose, talks and pats the weapon's end menacingly against their off-hand palm
    SnowClaw_MODANIM1_36_Hunched_Ready_LOOP,         //Similar to [mAnim1_0], but in a slightly deeper hunch
    SnowClaw_MODANIM1_37_Hunched_Turn_90_R,          //From [mAnim1_0], turns on the spot to face 90 degrees to the right [ROOT YAW SHIFTED 90 ANTICLOCKWISE AT START]
    SnowClaw_MODANIM1_38_Hunched_Turn_90_L,          //From [mAnim1_0], turns on the spot to face 90 degrees to the left [ROOT YAW SHIFTED 90 CLOCKWISE AT START]
    SnowClaw_MODANIM1_39_Hunched_Strafe_L_LOOP,      //From [mAnim1_0], leading with right foot, strafes to the left while facing target
    SnowClaw_MODANIM1_40_Hunched_Strafe_L_Guard_LOOP,  //Similar to [mAnim1_39], but holds weapon in front of face [ROOT SHIFTS LEFT]
    SnowClaw_MODANIM1_41_Hunched_Guard_LOOP,         //Similar to [mAnim1_0], but guarding face with weapon 
    SnowClaw_MODANIM1_42_Knocked_Over_Intro,         //Flung backwards, landing with a bounce onto rear (after being shot by Projectile Spell, etc.)
    SnowClaw_MODANIM1_43_Knocked_Over_Outro,         //From [mAnim1_43], picks self back up into neutral hunch pose
    SnowClaw_MODANIM1_44_Hunched_Turn_Left,          //Leading with left foot, a slight turn/strafe left
    SnowClaw_MODANIM1_45_Taunt,                      //From [mAnim1_35], talking while punching the air and shaking fist
    SnowClaw_MODANIM1_46_Taunt,                      //From [mAnim1_35], talking while patting weapon
    SnowClaw_MODANIM1_47_Hunched_Strafe_R,           //Similar to [mAnim1_39], leading with right foot, strafing to the right
    
    //Bank 2 (Gameplay Sequences?)
    SnowClaw_MODANIM2_0_Exercise_LOOP = 0x200,       //Starting with arms huddled to chest, swings arms to warm self
    SnowClaw_MODANIM2_1_Campfire_Idle_LOOP,          //Huddled with hands close to chest
    SnowClaw_MODANIM2_2_Campfire_Fidget_LOOP,        //Stepping on the spot
    SnowClaw_MODANIM2_3_Prison_Guard_Sleep_LOOP,     //CRF Prison Guard asleep with arms and head on desk
    SnowClaw_MODANIM2_4_Prison_Guard_Awake_Intro,    //Guard waking and resting head in right hand
    SnowClaw_MODANIM2_5_Prison_Guard_Awake_LOOP,     //Guard resting head in right head dozily
    //NOTE: the rest of the animations in this bank differ from the orange SharpClaws'
    SnowClaw_MODANIM2_6_Club_Prod,                   //[SNOWCLAW ONLY] Hurting Tricky >:( 
    SnowClaw_MODANIM2_7_Bike_Mount_Step,             //Jetbike: stepping forward into riding pose [ROOT Z SHIFTS FORWARD]
    SnowClaw_MODANIM2_8_CannonClaw_Idle_LOOP,        //[SNOWCLAW ONLY] Operating a cannon turret
    SnowClaw_MODANIM2_9_CannonClaw_Fire_Recoil,      //[SNOWCLAW ONLY] Firing a cannon turret (unused)

    //Bank 3 (Cutscene Sequences?) (NOTE: the animations in this bank differ from the orange SharpClaws')
    SnowClaw_MODANIM3_0_Walk_LOOP = 0x300,           //[SNOWCLAW ONLY] From [mAnim3_2], a walk cycle starting in passing pose, with left foot leading towards contact [ROOT Z SHIFTS FORWARD]
    SnowClaw_MODANIM3_1_Walk_Outro,                  //[SNOWCLAW ONLY] From [mAnim3_0], leading with left foot, stops walking and settles into a standing pose. 
    SnowClaw_MODANIM3_2_Walk_Intro,                  //[SNOWCLAW ONLY] From a standing pose, starts to walk ahead, leading with right foot [ROOT Z SHIFTS FORWARD]
    SnowClaw_MODANIM3_3_Battle_Ready_LOOP,           //[SNOWCLAW ONLY] In a hunched pose, with weapon at the ready and tail raised.
    SnowClaw_MODANIM3_4_Huddled_Look_R_Intro,        //[SNOWCLAW ONLY] From huddled pose, drops arms and turns to look right
    SnowClaw_MODANIM3_5_Huddled_Look_R_Hold,         //[SNOWCLAW ONLY] From [mAnim3_4], holding end-pose with weapon dropped slightly
    SnowClaw_MODANIM3_6_Huddled_Look_R_Swat,         //[SNOWCLAW ONLY] From [mAnim3_5], still looking right, swats club slightly
    SnowClaw_MODANIM3_7_Huddled_Look_R_Turn,         //[SNOWCLAW ONLY] From [mAnim3_5], turns further to look fully behind, then returns to roughly initial pose
    SnowClaw_MODANIM3_8_Battle_Sway_LOOP,            //[SNOWCLAW ONLY] Similar to [mAnim3_3], but swaying body from side to side slightly with club at the ready.
    SnowClaw_MODANIM3_9_Club_Sway_Clockwise,         //[SNOWCLAW ONLY] From [mAnim3_8], swings the club lightly through the air before returning to original pose
    SnowClaw_MODANIM3_10_Huddled_Look_R_Pushed,      //[SNOWCLAW ONLY] Similar to [mAnim3_4], but with a more exaggerated/polished turn
    SnowClaw_MODANIM3_11_Bike_Look_LOOP,             //[SNOWCLAW ONLY] Jetbike: looking around while in idle pose
    SnowClaw_MODANIM3_12_Bike_Road_Rage_Intro,       //Jetbike: reels from an impact, then stands up with left arm/foot still steering and shakes fist angrily to the right (Euler lerp problems on right hand during reel) [ROOT Y SHIFTED]
    SnowClaw_MODANIM3_13_Bike_Road_Rage_LOOP,        //Jetbike: from [mAnim3_13], moving hold of the end-pose, with right hand still waving subtly in the air [ROOT Y SHIFTED]
    SnowClaw_MODANIM3_14_Bike_Flinch_R,              //[SNOWCLAW ONLY] Jetbike: swaying to the right after a collision, shaking head disapprovingly, then settling back and reopening mouth
    SnowClaw_MODANIM3_15_Bike_Flinch_L,              //[SNOWCLAW ONLY] Jetbike: swaying to the left after a collision, then settling back and reopening mouth
    SnowClaw_MODANIM3_16_Jump_Reach_LOOP,            //Jumping on the spot, trying to reach something above them (unused?)
    SnowClaw_MODANIM3_17_Beckon_LOOP,                //Looking way up, beckoning and signalling to a spaceship
    SnowClaw_MODANIM3_18_Club_Prop_LOOP,             //[SNOWCLAW ONLY] A double-prod forward with the club
    SnowClaw_MODANIM3_19_Bike_SpellStone_Carry_LOOP, //[SNOWCLAW ONLY] Jetbike: carrying SpellStone under right arm, steering with left arm [ROOT Y SHIFTED]
    SnowClaw_MODANIM3_20_Bike_Look_Behind_R_Intro,   //[SNOWCLAW ONLY] Jetbike: swivelling to look behind over right shoulder [ROOT Y SHIFTED]
    SnowClaw_MODANIM3_21_Bike_Look_Behind_L_Intro,   //[SNOWCLAW ONLY] Jetbike: swivelling to look behind over left shoulder  [ROOT Y SHIFTED]
    SnowClaw_MODANIM3_22_Bike_Look_Behind_R_LOOP,    //[SNOWCLAW ONLY] From [mAnim3_20], looking behind over right shoulder
    SnowClaw_MODANIM3_23_Bike_Look_Behind_L_LOOP,    //[SNOWCLAW ONLY] From [mAnim3_20], looking behind over left shoulder
    SnowClaw_MODANIM3_24_Gaze_Up_R,                  //DF Shrine patrolling SharpClaw looking up
    SnowClaw_MODANIM3_25_Hammering_Club              //Raising the club and pounding it against a nearby surface (yaw interpolation issues midway) (unused?) [ROOT ROTATED 180]
} SnowClaw_ModAnims;

typedef enum {
    SharpClaw_MESSAGE_1_Others_Attacking = 1,
    SharpClaw_MESSAGE_2_Had_Priority_Over_Allies = 2
} SharpClaw_Messages;

typedef enum {
    SharpClaw_FOOTSTEP_Soft,
    SharpClaw_FOOTSTEP_Wood,
    SharpClaw_FOOTSTEP_Snow,
    SharpClaw_FOOTSTEP_Water,
    SharpClaw_FOOTSTEP_Stone
} SharpClaw_FootstepSounds;

DLL_INTERFACE(DLL_214_SharpClaw) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ s32 (*GetLogicState)(Object* self);
    /*08*/ u8 (*ReceiveMessage)(Object* self, u8 message);
};

#define dll_sharpClaw(obj) (((DLL_214_SharpClaw*)obj->dll)->vtbl)

#endif //_DLLS_215_H
