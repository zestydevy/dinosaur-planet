#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "sys/gfx/modgfx.h"
#include "sys/math.h"
#include "sys/menu.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

typedef struct {
    s8 unk0;
    u8 maxTurnAcceleration;
    u8 turnAcceleration;
    f32 unk4;
    s16 unk8;
    u8 unkA;
    u16 unkC;
    s16 unkE;
    u16 unk10;
    f32 freezeTimer;
} SharpClaw_DataActual;

typedef struct {
    Baddie baddie;
    SharpClaw_DataActual objData;
} SharpClaw_Data;

typedef enum {
    SharpClaw_ASTATE_0_Idle, //Idle/standing?
    SharpClaw_ASTATE_1_Walk, //Walk?
    SharpClaw_ASTATE_2,
    SharpClaw_ASTATE_3, //Run?
    SharpClaw_ASTATE_4, //Dodge right?
    SharpClaw_ASTATE_5, //Dodge back?
    SharpClaw_ASTATE_6,
    SharpClaw_ASTATE_7,
    SharpClaw_ASTATE_8,
    SharpClaw_ASTATE_9,
    SharpClaw_ASTATE_10,
    SharpClaw_ASTATE_11, //Dodge left?
    SharpClaw_ASTATE_12_Taunt1, //Taunting?
    SharpClaw_ASTATE_13_Taunt2,
    SharpClaw_ASTATE_14_Taunt3,
    SharpClaw_ASTATE_15,
    SharpClaw_ASTATE_16_Attack_Anticlockwise, //Swing attack anticlockwise
    SharpClaw_ASTATE_17_Attack_Clockwise, //Swing attack clockwise
    SharpClaw_ASTATE_18_Attack_Overhead, //Overhead attack? 
    SharpClaw_ASTATE_19, //Hurt/hit
    SharpClaw_ASTATE_20,
    SharpClaw_ASTATE_21, //Knocked down (via Projectile etc.)
    SharpClaw_ASTATE_22,
    SharpClaw_ASTATE_23_Dying, //Dying
    SharpClaw_ASTATE_24
} SharpClaw_AnimStates;

typedef enum {
    SharpClaw_LSTATE_0,
    SharpClaw_LSTATE_1,
    SharpClaw_LSTATE_2, //Idle? Standing? Looking?
    SharpClaw_LSTATE_3,
    SharpClaw_LSTATE_4,
    SharpClaw_LSTATE_5,
    SharpClaw_LSTATE_6,
    SharpClaw_LSTATE_7, //Knocked down? Hit?
    SharpClaw_LSTATE_8, //Dodging/hit/backstepping?
    SharpClaw_LSTATE_9_Dying, //Dying
    SharpClaw_LSTATE_10_Dead, //Dead?
    SharpClaw_LSTATE_11, //Engage/approach?
    SharpClaw_LSTATE_12, //Attack
    SharpClaw_LSTATE_13
} SharpClaw_LogicStates;

//NOTE: SharpClaw (Orange) and SnowClaw (Blue) have slightly different MODANIM lists!
typedef enum {
    //Bank 0 (Noncombat?)
    SharpClaw_MODANIM0_0_Idle_LOOP = 0,         //Standing with arms loose and mouth agape
    SharpClaw_MODANIM0_1_Patrol_LOOP,           //A patrolling walk cycle, with weapon held up in right hand and slung over shoulder
    SharpClaw_MODANIM0_2_Walk_LOOP,             //A more casual walk cycle, with arms loose
    SharpClaw_MODANIM0_3_Run_LOOP,              //Bounding on all fours
    SharpClaw_MODANIM0_4_Walk_LOOP,             //Similar to [mAnim0_2], but with softer timing on the contacts
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

/*0x0*/ static s32 dHitAnimStateMap[] = {
    SharpClaw_ASTATE_19, SharpClaw_ASTATE_19, SharpClaw_ASTATE_21, SharpClaw_ASTATE_21, 
    SharpClaw_ASTATE_21, SharpClaw_ASTATE_19, SharpClaw_ASTATE_19, SharpClaw_ASTATE_19, 
    SharpClaw_ASTATE_19, SharpClaw_ASTATE_19, SharpClaw_ASTATE_19, SharpClaw_ASTATE_21, 
    SharpClaw_ASTATE_21, SharpClaw_ASTATE_21, SharpClaw_ASTATE_21, SharpClaw_ASTATE_19, 
    SharpClaw_ASTATE_21, SharpClaw_ASTATE_19, SharpClaw_ASTATE_19, SharpClaw_ASTATE_19, 
    SharpClaw_ASTATE_19, SharpClaw_ASTATE_21, SharpClaw_ASTATE_19, SharpClaw_ASTATE_20, 
    SharpClaw_ASTATE_19, SharpClaw_ASTATE_19, SharpClaw_ASTATE_19, SharpClaw_ASTATE_21
};
/*0x70*/ static u32 dHitReactAnims[] = {
    SharpClaw_MODANIM1_13_Flinch_Neutral, 
    SharpClaw_MODANIM1_13_Flinch_Neutral, 
    SharpClaw_MODANIM1_12_Flinch_Back, 
    SharpClaw_MODANIM1_10_Flinch_L, 
    SharpClaw_MODANIM1_10_Flinch_L, 
    SharpClaw_MODANIM1_11_Flinch_R, 
    SharpClaw_MODANIM1_11_Flinch_R, 
    SharpClaw_MODANIM1_11_Flinch_R, 
    SharpClaw_MODANIM1_10_Flinch_L
};

#define ONE_HP 1 //Used to help highlight non-default hitDamageMap entries
/*0x94*/ static s8 dHitDamageMap[] = {
    -1,       -1,      -1,    ONE_HP,
    -1,       -1,      -1,    -1, 
    -1,       -1,      -1,   -1, 
    ONE_HP,  ONE_HP, -1,   -1, 
    -1,      -1,     -1,   -1, 
    -1,      -1,     -1,   -1, 
    ONE_HP,  -1,     -1,   -1
};

typedef enum {
    SharpClaw_FOOTSTEP_Soft,
    SharpClaw_FOOTSTEP_Wood,
    SharpClaw_FOOTSTEP_Snow,
    SharpClaw_FOOTSTEP_Water,
    SharpClaw_FOOTSTEP_Stone
} SharpClaw_Footsteps;

/*0xB0*/ static s16 dFootstepSounds[] = {
    SOUND_61_Footstep_Soft,         SOUND_62_Footstep_Soft, 
    SOUND_69_Footstep_Wood_Creaky,  SOUND_6A_Footstep_Wood_Creaky, 
    SOUND_65_Footstep_Snow,         SOUND_66_Footstep_Snow, 
    SOUND_6D_Footstep_Water_Wade,   SOUND_6E_Footstep_Water_Wade, 
    SOUND_280_Footstep_Stone,       SOUND_281_Footstep_Stone
};
/*0xC4*/ static s32 dTauntSounds[] = {
    SOUND_169_SharpClaw_Taunt_1, 
    SOUND_16A_SharpClaw_Taunt_2, 
    SOUND_16B_SharpClaw_Taunt_3
};
/*0xD0*/ static u32 dHurtSounds[] = {
    SOUND_236_SharpClaw_Argh, 
    SOUND_237_SharpClaw_Arghhh, 
    SOUND_238_SharpClaw_Snort, 
    SOUND_239_SharpClaw_Rah_Snort, 
    SOUND_23A_SharpClaw_Ugh_Snort
};
/*0xE4*/ static u32 dAttackSounds[] = {
    SOUND_23B_SharpClaw_Hyah, 
    SOUND_23F_SharpClaw_Nyeh_Nyah, 
    SOUND_23D_SharpClaw_Hhhhah, 
    SOUND_23E_SharpClaw_Rrrgh, 
    SOUND_240_SharpClaw_Snort, 
    SOUND_23C_SharpClaw_Ryah
};
/*0xFC*/ static u32 dWeaponSounds[] = {
    SOUND_B6_Weapon_Whoosh, 
    SOUND_99_Punchy_Smack, 
    SOUND_90_Heavier_Whoosh, 
    SOUND_91_Heavy_Whoosh
};
/*0x10C*/ static u32 dWeaponHitSoundsSabre[] = {
    SOUND_374_Whack, 
    SOUND_375_Smack1, 
    SOUND_376_Smack2, 
    SOUND_25B_Magic_Attack_Deflected, 
    SOUND_25C_Melee_Attack_Deflected
};
/*0x120*/ static u32 dWeaponHitSoundsKrystal[] = {
    SOUND_377_Metal_Smack, 
    SOUND_378_Metal_Whack, 
    SOUND_379_Metal_Slice, 
    SOUND_25B_Magic_Attack_Deflected, 
    SOUND_25C_Melee_Attack_Deflected
};
/*0x134*/ static s16 dWalkAnimsPatrol[] = {
    SharpClaw_MODANIM0_0_Idle_LOOP, 
    SharpClaw_MODANIM0_1_Patrol_LOOP, 
    SharpClaw_MODANIM0_2_Walk_LOOP 
};
/*0x13C*/ static s16 dWalkAnims[] = {
    SharpClaw_MODANIM0_0_Idle_LOOP, 
    SharpClaw_MODANIM0_4_Walk_LOOP, 
    SharpClaw_MODANIM0_2_Walk_LOOP 
};
/*0x144*/ static s16 dIdleAnims[] = {
    SharpClaw_MODANIM0_21_Club_Idle_Fidget, 
    SharpClaw_MODANIM0_23_Club_Idle_Search_L, 
    SharpClaw_MODANIM0_24_Club_Idle_Search_R, 
    SharpClaw_MODANIM0_19_Search_LOOP
};
/*0x14C*/ static f32 dWalkSpeedThresholds[] = {
    0, 0.05, 
    0.03, 0.7, 
    0.68, 1.05
};
/*0x164*/ static s32 dCurveTypes[] = {
    2, 3
};

// NOTE: interestingly nearly all the objIDs here are deleted objects!
// (They seem to be meant as other kinds of SharpClaw, or at least objects with similar logic states)
/*0x16C*/ static u32 dAllyObjectIDs[] = {
    OBJ_ClubSharpClaw, 271, 277, OBJ_SnowClaw, 366, 415, 567, 580
};

/*0x18C*/ static u8 dTerrainTypeSoundMap[] = {
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Snow, 
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft, 
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Snow,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft, 
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Snow,
   SharpClaw_FOOTSTEP_Water,
   SharpClaw_FOOTSTEP_Soft, 
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Water, 
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Snow,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft, 
   SharpClaw_FOOTSTEP_Wood,
   SharpClaw_FOOTSTEP_Stone,
   SharpClaw_FOOTSTEP_Water,
   SharpClaw_FOOTSTEP_Snow, 
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft, 
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft,
   SharpClaw_FOOTSTEP_Soft
};

//Local data for func_2044 (troublesomely unmatched!)
/*0x1B0*/ static u32 data_1B0[] = {
    0x00000006, 0x00000069, 0x00000069, 0x000000ff
};
/*0x1C0*/ static u16 data_1C0[] = {
    0x0206, 0x0167, 0x0165, 0x0206
};
/*0x1C8*/ static u16 data_1C8[] = {
    0x0206, 0x0167, 0x0165, 0x0206
};

/*0x0*/ static SRT sFXTransform;
/*0x1C*/ static s32 bss_1C;
/*0x20*/ static ObjFSA_StateCallback sAnimStateCallbacks[26];
/*0x88*/ static ObjFSA_StateCallback sLogicStateCallbacks[14];

static s32 SharpClaw_animState0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState1Walk(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState5(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState6(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState7(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState9(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState10(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState11(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState12Taunt1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState13Taunt2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState14Taunt3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState15(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState16AttackAnticlockwise(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState17AttackClockwise(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState18AttackOverhead(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState19(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState20(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState21(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState22(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState23(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState24(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 SharpClaw_logicState0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState5(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState6(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState7(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState9Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState10Dead(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState11(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState12(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState13(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void SharpClaw_func_0(void) {
    sAnimStateCallbacks[SharpClaw_ASTATE_0_Idle] = SharpClaw_animState0;
    sAnimStateCallbacks[SharpClaw_ASTATE_1_Walk] = SharpClaw_animState1Walk;
    sAnimStateCallbacks[SharpClaw_ASTATE_2] = SharpClaw_animState2;
    sAnimStateCallbacks[SharpClaw_ASTATE_3] = SharpClaw_animState3;
    sAnimStateCallbacks[SharpClaw_ASTATE_4] = SharpClaw_animState4;
    sAnimStateCallbacks[SharpClaw_ASTATE_5] = SharpClaw_animState5;
    sAnimStateCallbacks[SharpClaw_ASTATE_6] = SharpClaw_animState6;
    sAnimStateCallbacks[SharpClaw_ASTATE_7] = SharpClaw_animState7;
    sAnimStateCallbacks[SharpClaw_ASTATE_8] = SharpClaw_animState8;
    sAnimStateCallbacks[SharpClaw_ASTATE_9] = SharpClaw_animState9;
    sAnimStateCallbacks[SharpClaw_ASTATE_10] = SharpClaw_animState10;
    sAnimStateCallbacks[SharpClaw_ASTATE_11] = SharpClaw_animState11;
    sAnimStateCallbacks[SharpClaw_ASTATE_12_Taunt1] = SharpClaw_animState12Taunt1;
    sAnimStateCallbacks[SharpClaw_ASTATE_13_Taunt2] = SharpClaw_animState13Taunt2;
    sAnimStateCallbacks[SharpClaw_ASTATE_14_Taunt3] = SharpClaw_animState14Taunt3;
    sAnimStateCallbacks[SharpClaw_ASTATE_15] = SharpClaw_animState15;
    sAnimStateCallbacks[SharpClaw_ASTATE_16_Attack_Anticlockwise] = SharpClaw_animState16AttackAnticlockwise;
    sAnimStateCallbacks[SharpClaw_ASTATE_17_Attack_Clockwise] = SharpClaw_animState17AttackClockwise;
    sAnimStateCallbacks[SharpClaw_ASTATE_18_Attack_Overhead] = SharpClaw_animState18AttackOverhead;
    sAnimStateCallbacks[SharpClaw_ASTATE_19] = SharpClaw_animState19;
    sAnimStateCallbacks[SharpClaw_ASTATE_20] = SharpClaw_animState20;
    sAnimStateCallbacks[SharpClaw_ASTATE_21] = SharpClaw_animState21;
    sAnimStateCallbacks[SharpClaw_ASTATE_22] = SharpClaw_animState22;
    sAnimStateCallbacks[SharpClaw_ASTATE_23_Dying] = SharpClaw_animState23;
    sAnimStateCallbacks[SharpClaw_ASTATE_24] = SharpClaw_animState24;
    
    sLogicStateCallbacks[SharpClaw_LSTATE_0] = SharpClaw_logicState0;
    sLogicStateCallbacks[SharpClaw_LSTATE_1] = SharpClaw_logicState1;
    sLogicStateCallbacks[SharpClaw_LSTATE_2] = SharpClaw_logicState2;
    sLogicStateCallbacks[SharpClaw_LSTATE_3] = SharpClaw_logicState3;
    sLogicStateCallbacks[SharpClaw_LSTATE_4] = SharpClaw_logicState4;
    sLogicStateCallbacks[SharpClaw_LSTATE_5] = SharpClaw_logicState5;
    sLogicStateCallbacks[SharpClaw_LSTATE_6] = SharpClaw_logicState6;
    sLogicStateCallbacks[SharpClaw_LSTATE_7] = SharpClaw_logicState7;
    sLogicStateCallbacks[SharpClaw_LSTATE_8] = SharpClaw_logicState8;
    sLogicStateCallbacks[SharpClaw_LSTATE_9_Dying] = SharpClaw_logicState9Dying;
    sLogicStateCallbacks[SharpClaw_LSTATE_10_Dead] = SharpClaw_logicState10Dead;
    sLogicStateCallbacks[SharpClaw_LSTATE_11] = SharpClaw_logicState11;
    sLogicStateCallbacks[SharpClaw_LSTATE_12] = SharpClaw_logicState12;
    sLogicStateCallbacks[SharpClaw_LSTATE_13] = SharpClaw_logicState13;
}

// offset: 0x1F8 | ctor
void SharpClaw_ctor(void* dll) {
    SharpClaw_func_0();
}

// offset: 0x238 | dtor
void SharpClaw_dtor(void* dll) { }

// offset: 0x244 | func: 1 | export: 0
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_obj_Setup.s")
#else

static int SharpClaw_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

//Matches, but needs SharpClaw_func_2044 to be static
void SharpClaw_obj_Setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    Baddie* baddie;
    u8 flags;
    SharpClaw_DataActual* objData;

    baddie = self->data;
    
    flags = 0x10 | 4 | 2;
    if (reset) {
        flags = 0x10 | 4 | 2 | 1;
    }
    if ((objSetup->unk2B & 0x20) == FALSE) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0x19, 0xE, 0x10E, flags, 20.0f);
    
    self->animCallback = SharpClaw_animCallback;
    
    objData = baddie->objdata;
    baddie->fsa.animState = SharpClaw_ASTATE_0_Idle;
    baddie->fsa.logicState = SharpClaw_LSTATE_0;
    baddie->unk3B6 = 150;
    objData->unk0 = 0;
    objData->maxTurnAcceleration = objSetup->unk2F;
    objAddObjectType(self, OBJTYPE_63);
}
#endif

// offset: 0x32C | func: 2 | export: 1
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_obj_Control.s")
#else

static void SharpClaw_func_14C0(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa);
static void SharpClaw_func_18EC(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SharpClaw_func_E88(Object* self, Baddie* baddie, ObjFSA_Data* fsa);

//Matches, but needs SharpClaw_func_2044 to be static
void SharpClaw_obj_Control(Object* self) {
    Baddie* baddie;
    Baddie_Setup* objSetup;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    
    if (menuGetCurrent() == MENU_TITLE_SCREEN) {
        return;
    }
    
    if (self->unkDC != 0) {
        if (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID)) {
            gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0x19, 0xE, 0x10E, 0x36, 20.0f);
            baddie->fsa.logicState = SharpClaw_LSTATE_1;
            baddie->fsa.enteredLogicState = TRUE;
            self->opacity = 0;
            baddie->unk3B6 = 150;
        }
        return;
    } 

    if (!(baddie->unk3B0 & 4) && (self->unkE0 == 0)) {
        if (objSetup && !(baddie->unk3B0 & 8)) {
            self->srt.transl.x = objSetup->base.x;
            self->srt.transl.y = objSetup->base.y;
            self->srt.transl.z = objSetup->base.z;
        }
        
        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->unk2E, self, -1);
        self->unkE0 = 1;
        return;
    }
    
    if (baddie->unk3B2 & 2) {
        if (!(baddie->unk3B0 & 4)) {
            gDLL_33_BaddieControl->vtbl->func9(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, (s8*)&baddie->unk3B4, 0, 0, 0, 1);
            if (baddie->unk3B2 & 4) {
                baddie->fsa.logicState = SharpClaw_LSTATE_13;
            }
        }
        
        baddie->unk3B0 &= ~4;
        baddie->unk3B2 &= ~2;
        
        if (baddie->fsa.hitpoints > 0) {
            func_8002674C(self);
        }
    }
    
    if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1)) {
        SharpClaw_func_E88(self, baddie, &baddie->fsa);
        if (baddie->unk3B4 == 2) {
            SharpClaw_func_14C0(self, 0, baddie, &baddie->fsa);
        } else {
            SharpClaw_func_18EC(self, baddie, &baddie->fsa);
        }
    }
}
#endif

// offset: 0x5E0 | func: 3 | export: 2
void SharpClaw_obj_Update(Object* self) {
    Baddie* baddie;
    ObjFSA_Data* fsa;
    ObjectHitInfo* objhitInfo;

    baddie = self->data;
    fsa = &((Baddie*)self->data)->fsa;
    
    gDLL_18_objfsa->vtbl->func2(self, fsa, sAnimStateCallbacks);
    if (fsa->unk341 == 1) {
        if (self->linkedObject != NULL) {
            objhitInfo = self->linkedObject->objhitInfo;
        } else {
            objhitInfo = self->objhitInfo;
        }
        
        if ((self->objhitInfo->unk48 != NULL) || (objhitInfo->unk48 != NULL)) {
            self->objhitInfo->unk61 = 1;
        }
    }
}

// offset: 0x68C | func: 4 | export: 3
void SharpClaw_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (!visibility || self->unkDC) {
        return;
    }
    
    //Tint blue/red while frozen or hurt
    if (objData->freezeTimer > 0.0f) {
        objprintSetBlendColor(0x64, 0xFF, 0xFF, 0x9B);
    } else {
        if (baddie->unk3E8 != 0.0f) {
            objprintSetBlendColor(0xC8, 0, 0, baddie->unk3E8);
        }
    }
    
    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    
    //Create particles while frozen
    if (objData->freezeTimer > 0.0f) {
        gDLL_32->vtbl->func2(self, PARTICLE_52A, NULL);
    }
    
    if (baddie->unk3B2 & (0x40 | 0x20)) {
        if (baddie->unk3B2 & 0x20) {
            gDLL_32->vtbl->func2(self, PARTICLE_330, &baddie->unk3E8);
            gDLL_32->vtbl->func2(self, PARTICLE_330, &baddie->unk3E8);
        }
        gDLL_32->vtbl->func2(self, PARTICLE_32F, &baddie->unk3E8);
    }
    
    if (baddie->unk3B2 & 0x100) {
        gDLL_32->vtbl->func2(self, PARTICLE_333, &baddie->unk3E8);
        gDLL_32->vtbl->func2(self, PARTICLE_334, &baddie->unk3E8);
        baddie->unk3B2 &= ~0x100;
    }
}

// offset: 0x958 | func: 5 | export: 4
void SharpClaw_obj_Free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    objFreeObjectType(self, OBJTYPE_Baddie);
    objFreeObjectType(self, OBJTYPE_63);

    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0xA0C | func: 6 | export: 5
u32 SharpClaw_obj_GetModelFlags(Object* self) {
    return MODFLAGS_100 | MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0xA1C | func: 7 | export: 6
u32 SharpClaw_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(SharpClaw_Data);
}

// offset: 0xA30 | func: 8 | export: 7
s16 SharpClaw_GetLogicState(Object* self) {
    Baddie* baddie = self->data;
    return baddie->fsa.logicState;
}

// offset: 0xA40 | func: 9 | export: 8
void SharpClaw_ReceiveMessage(Object* self, u8 message) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;
 
    baddie = self->data;
    objData = baddie->objdata;
    
    switch (message) {
    case 1:
        objData->unkA = 1;
        break;
    case 2:
        objData->unkA = 2;
        break;
    }
}

// offset: 0xA88 | func: 10
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_animCallback.s")
#else

static void SharpClaw_func_14C0(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa);
static void SharpClaw_func_17A0(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SharpClaw_func_E88(Object* self, Baddie* baddie, ObjFSA_Data* fsa);

//Matches, but needs SharpClaw_func_2044 to be static
int SharpClaw_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    Baddie* baddie;
    Baddie_Setup* objSetup;
    Object* sidekick;
    s32 i;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    
    if (self->unkDC != 0) {
        return 0;
    }

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case 1:
            sidekick = objGetSidekick();
            if (sidekick != NULL) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->func21(sidekick, 1, self);
                baddie->unk3B2 |= 4;
                baddie->unk3B4 = 2;
                baddie->fsa.logicState = SharpClaw_LSTATE_13;
            }
            break;
        case 2:
            baddie->nextWeaponID = 3;
            SharpClaw_func_E88(self, baddie, &baddie->fsa);
            break;
        case 3:
            gDLL_3_Animation->vtbl->set_camera_module(0x5B, 4, (s32)self, 0x3C);
            break;
        }
    }
    
    if (self->seqSlot != -1) {
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1) == 0) {
            func_800267A4(self);
            return 1;
        }
        if (baddie->fsa.hitpoints > 0) {
            func_8002674C(self);
        }
        SharpClaw_func_E88(self, baddie, &baddie->fsa);
        if ((baddie->unk3A0 != NO_GAMEBIT) && mainGetBits(baddie->unk3A0)) {
            gDLL_3_Animation->vtbl->func21(animData, objSetup->unk2C);
            baddie->unk3A0 = -1;
        }

        switch (baddie->unk3B4) {
        case 2:
            animData->unk7A = 0;
            SharpClaw_func_14C0(self, animData, baddie, &baddie->fsa);
            if (baddie->unk3B4 == 1) {
                baddie->fsa.logicState = SharpClaw_LSTATE_11;
                gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, 1.0f, 1.0f, sAnimStateCallbacks, sLogicStateCallbacks);
                animData->unk62 = 0;
            }
            break;
        case 1:
            if (gDLL_33_BaddieControl->vtbl->func12(self, animData, baddie, sAnimStateCallbacks, sLogicStateCallbacks, 0)) {
                gDLL_33_BaddieControl->vtbl->func10(self, &baddie->fsa, 0.17f, 1);
            }
            SharpClaw_func_17A0(self, baddie, &baddie->fsa);
            break;
        case 0:
        default:
            animData->unk7A = -1;
            animData->unk7A &= ~0x40;
            SharpClaw_func_17A0(self, baddie, &baddie->fsa);
            break;
        }
    }

    if (self->seqSlot == -1) {
        baddie->unk3B2 |= 2;
        func_800267A4(self);
        return 0;
    }
    
    return baddie->unk3B4 != 0;
}
#endif

// offset: 0xE88 | func: 11
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_E88.s")
#else

static void SharpClaw_func_1E2C(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SharpClaw_func_2044(Object* self, SRT* fxTransform, s32 useModGfx);
static s32 SharpClaw_func_267C(u8 message, Object* baddie);

//Matches, but needs SharpClaw_func_2044 to be static
void SharpClaw_func_E88(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    SharpClaw_DataActual* objData;
    s32 hit;
    s32 result;

    objData = baddie->objdata;

    if (self->linkedObject != NULL) {
        self->linkedObject->parent = self->parent;
    }
    
    gDLL_33_BaddieControl->vtbl->func4(self, objGetPlayer(), 0x10, &objData->unkC, &objData->unkE, &objData->unk10);
    fsa->targetDist = objData->unk10;
    if (!(baddie->unk3B0 & 0x20)) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 9, 0xA, baddie->unk3A6, baddie->unk3A4);
    }
    gDLL_33_BaddieControl->vtbl->change_weapon(self, baddie);
    
    SharpClaw_func_1E2C(self, baddie, fsa);
    objExprEyeIdle(self, &baddie->unk3BC);

    result = gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0);
    if (result == 1) {
        baddie->unk3B2 |= 4;
    } else if (result == 2) {
        baddie->unk3B2 &= ~4;
    }
    
    if ((fsa->animState == SharpClaw_ASTATE_16_Attack_Anticlockwise) || 
        (fsa->animState == SharpClaw_ASTATE_17_Attack_Clockwise) || 
        (fsa->animState == SharpClaw_ASTATE_18_Attack_Overhead)
    ) {
        func_80028D2C(self);
    } else {
        func_80026160(self);
    }
    
    objData->unk4 += gUpdateRateF;
    
    if (baddie->unk3B0 & 0x80) {
        if ((fsa->animState == SharpClaw_ASTATE_16_Attack_Anticlockwise) || 
            (fsa->animState == SharpClaw_ASTATE_17_Attack_Clockwise) || 
            (fsa->animState == SharpClaw_ASTATE_18_Attack_Overhead) || 
            (baddie->unk3B2 & 0x10)
        ) {
            hit = gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, dHitAnimStateMap, dHitDamageMap, SharpClaw_LSTATE_7, &baddie->unk3A8, &sFXTransform);
            if (hit) {
                SharpClaw_func_2044(self, &sFXTransform, FALSE);
            }
        } else {
            hit = gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, NULL, SharpClaw_LSTATE_7, &baddie->unk3A8, &sFXTransform);
            if (hit) {
                SharpClaw_func_2044(self, &sFXTransform, TRUE);
            }
        }
    } else {
        if (((fsa->animState == SharpClaw_ASTATE_15) || (fsa->animState == SharpClaw_ASTATE_5)) 
            && !(baddie->unk3B2 & 0x10)
        ) {
            hit = gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, NULL, SharpClaw_LSTATE_7, &baddie->unk3A8, &sFXTransform);
            if (hit) {
                SharpClaw_func_2044(self, &sFXTransform, TRUE);
            }
        } else {
            hit = gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, dHitAnimStateMap, dHitDamageMap, SharpClaw_LSTATE_7, &baddie->unk3A8, &sFXTransform);
            if (hit) {
                SharpClaw_func_2044(self, &sFXTransform, FALSE);
            }
        }
    }
    
    //React to explosions/projectiles
    if ((hit == Damage_Type_Explosion) || 
        (hit == Damage_Type_E) || 
        (hit == Damage_Type_Projectile)
    ) {
        baddie->unk3B4 = 2;
        fsa->target = objGetPlayer();
        return;
    }
    
    //Become frozen by the Ice Blast Spell
    if (hit == Damage_Type_Ice_Blast) {
        baddie->unk3B4 = 2;
        objData->freezeTimer = 400.0f;
        baddie->unk3B2 |= 0x80;
        return;
    }
    
    //Handle other kinds of damage
    if (hit) {
        baddie->unk3B4 = 2;
        if (objData->unk4 < 240.0f) {
            objData->unk8++;
        } else {
            objData->unk8 = 0;
        }
        
        if (fsa->hitpoints > 0) {
            objData->unk4 = 0.0f;
            if (objData->unk8 >= 2) {
                objData->unk8 = 0;
                fsa->logicState = SharpClaw_LSTATE_8;
                fsa->enteredLogicState = TRUE;
                if (SharpClaw_func_267C(2, self)) {
                    objData->unkA = 1;
                }
            } else if (SharpClaw_func_267C(2, self)) {
                objData->unkA = 1;
            }
        }
    }
}

#endif

// offset: 0x14C0 | func: 12
void SharpClaw_func_14C0(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa) {
    SharpClaw_DataActual* objData;
    Baddie_Setup* objSetup;
    UnkCurvesStruct* curves;
    Object* sidekick;
    Object* target;

    objData = baddie->objdata;
    objSetup = (Baddie_Setup*)self->setup;
    target = fsa->target;
    if (target != NULL) {
        objExpr_func_80032C0C(self, target, &baddie->unk3BC, 0x19);
    }
    
    if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2 * 1.39f, !(baddie->unk3B0 & 0x10))) {
        fsa->target = baddie->unk3AC;
        fsa->unk33D = 0;
        if (objSetup->unk2E != -1) {
            baddie->unk3B4 = 1;
            if (animData != NULL) {
                gDLL_3_Animation->vtbl->func21(animData, objSetup->unk24);
            }
        } else {
            fsa->speed = 0.0f;
            fsa->unk278 = 0.0f;
            objData->turnAcceleration = 0;
            curves = baddie->unk3F8;
            baddie->unk3B4 = 0;
            if (curves != NULL) {
                if (gDLL_26_Curves->vtbl->func_4288(curves, self, 700.0f, dCurveTypes, -1) != 0) {
                    baddie->unk3B2 &= ~8;
                } else {
                    baddie->unk3B2 |= 8;
                }
            }
        }

        if (fsa->hitpoints != 0) {
            if (baddie->unk3B2 & 4) {
                sidekick = objGetSidekick();
                if (sidekick != NULL) {
                    ((DLL_ISidekick*)sidekick->dll)->vtbl->func21(sidekick, 1, self);
                }
                fsa->logicState = SharpClaw_LSTATE_13;
            } else {
                fsa->logicState = SharpClaw_LSTATE_11;
            }
        }
    }
    
    if (fsa->hitpoints > 0) {
        func_8002674C(self);
    }
    
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, 1);
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0x17A0 | func: 13
void SharpClaw_func_17A0(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    SharpClaw_DataActual* objData;
    Object* target;
    Object* player;
    u32 temp = 2;

    objData = baddie->objdata;
    player = objGetPlayer();

    if (!(baddie->unk3B0 & 0x40) && (((DLL_210_Player*)player->dll)->vtbl->func50(player) != 0x40)) {
        target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, M_180_DEGREES);
        if (target != NULL) {
            gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, (s8*)&baddie->unk3B4, 0, 0, 0, 1);
            fsa->unk33D = 0;
            fsa->target = target;
            baddie->unk3B0 &= ~0x10;
            baddie->unk3B4 = temp;
            objData->unkA = temp;
        }
    }
}

#define WRAP_F(x,low,high) { if ((x) > (high)) (x) = -(((high) * 2) - 1) + (x); if ((x) < (low)) (x) = (((high) * 2) - 1) + (x); }
#define CIRCLE_WRAP_F(x) WRAP_F(x, -M_180_DEGREES_F, M_180_DEGREES_F)

// offset: 0x18EC | func: 14
void SharpClaw_func_18EC(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Object* target;
    s32 pad[3];
    UnkCurvesStruct* curves;
    SharpClaw_DataActual* objData;
    Object* player;
    f32 dx;
    f32 dz;
    f32 yawDiff;
    f32 absYawDiff;

    curves = baddie->unk3F8;
    objData = baddie->objdata;
    player = objGetPlayer();
    
    if (fsa->hitpoints == 0) {
        return;
    }
    
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, 1);
    
    if (((baddie->unk3B2 & 4) == FALSE) && (fsa->logicState != SharpClaw_LSTATE_1)) {
        fsa->logicState = SharpClaw_LSTATE_2;
        if (baddie->unk3B2 & 8) {
            dx = curves->unk0.unk68.x - self->srt.transl.x;
            dz = curves->unk0.unk68.z - self->srt.transl.z;
            dx = sqrtf(SQ(dx) + SQ(dz));
            if ((curves_func_800053B0(&curves->unk0, 10.0f / dx) || curves->unk0.unk10) && 
                gDLL_26_Curves->vtbl->func_4704(curves) && 
                gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, self, 700.0f, dCurveTypes, -1)
            ) {
                baddie->unk3B2 &= ~8;
            }

            if (objData->turnAcceleration < objData->maxTurnAcceleration) {
                objData->turnAcceleration += gUpdateRate;
            }

            yawDiff = (((u16)mathAtan2f(curves->unk0.unk74, curves->unk0.unk7C)) - ((u16)self->srt.yaw & 0xFFFF)) + M_180_DEGREES;
            //Similar to CIRCLE_WRAP, but applied to a float?
            CIRCLE_WRAP_F(yawDiff);

            absYawDiff = (yawDiff < 0) ? -yawDiff : yawDiff;
            
            fsa->unk278 = 1.0f - (absYawDiff / (M_180_DEGREES_F - 1.0f));
            if (fsa->unk278 < 0.01f) {
                fsa->unk278 = 0.01f;
            }

            fsa->unk278 *= objData->turnAcceleration / 100.0f;
            fsa->speed = fsa->unk278;
            gDLL_18_objfsa->vtbl->func6(self, fsa, curves->unk0.unk68.x, curves->unk0.unk68.f[2], 0.0f, 0.0f, 60.0f);
            if ((fsa->enteredAnimState) || (fsa->unk33A)) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_1_Walk);
            }
        } else if ((fsa->enteredAnimState) || (fsa->unk33A)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_0_Idle);
        }
        
        if ((fsa->enteredAnimState || fsa->unk33A) && !(baddie->unk3B0 & 0x40) && (((DLL_210_Player*)player->dll)->vtbl->func50(player) != 0x40)) {           
            target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, M_180_DEGREES);
            if (target != NULL) {
                gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, (s8*)&baddie->unk3B4, 0, 0, 0, 1);
                fsa->target = target;
                fsa->unk33D = 0;
                baddie->unk3B0 &= ~0x10;
                baddie->unk3B4 = 2;
                objData->unkA = 2;
            }
        }
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0x1E2C | func: 15
void SharpClaw_func_1E2C(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    s32 flags;
    s32 soundIdx;
    u32 soundHandle;
    u32 terrainType;
    s16 soundID;

    soundIdx = 0;
    if (baddie->fsa.unk4.mode != 0) {
        terrainType = baddie->fsa.unk4.unk68.unk50[0];
        if (terrainType < 34) {
            soundIdx = dTerrainTypeSoundMap[terrainType];
        }
        if (soundIdx >= 5) {
            soundIdx = 0;
        }
    }
    
    flags = 0;    
    if (fsa->unk308 & 2) {
        flags = 1;
    }
    if (fsa->unk308 & 4) {
        flags |= 2;
    }
    
    if (flags & 1) {
        soundHandle = dll_amSfx->Play(self, dFootstepSounds[soundIdx*2], MAX_VOLUME, NULL, NULL, 0, NULL);
        if (soundHandle != 0) {
            dll_amSfx->SetPitch(soundHandle, (mathRnd(1, 250) * 0.001f) + 0.875f);
        }
    }
    
    if (flags & 2) {
        soundHandle = dll_amSfx->Play(self, dFootstepSounds[(soundIdx*2) + 1], MAX_VOLUME, NULL, NULL, 0, NULL);
        if (soundHandle != 0) {
            dll_amSfx->SetPitch(soundHandle, (mathRnd(1, 250) * 0.001f) + 0.875f);
        }
    }
}

// offset: 0x2044 | func: 16
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_2044.s")
#else
//https://decomp.me/scratch/lQrpb
void SharpClaw_func_2044(Object* self, SRT* fxTransform, s32 useModGfx) {
    DLL_IModgfx* modGfxDLL;
    s32 i;
/*0x1B0*/ s32 data_1B0[] = { 0x00000006, 0x00000069, 0x00000069, 0x000000ff }; //60
/*0x1C0*/ s16 data_1C0[] = { 0x0206, 0x0167, 0x0165, 0x0206 }; //58
/*0x1C0*/ s16 data_1C8[] = { 0x0206, 0x0167, 0x0165, 0x0206 }; //50
    f32 fxParam; //4C
    Object* player; //48    
    
    player = objGetPlayer();

    
    if (useModGfx == FALSE) {
        fxParam = 0.014f;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_325, fxTransform, 0x200001, -1, &fxParam);
        
        fxTransform->scale = 92.0f;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, fxTransform, 0x200001, -1, NULL);
        
        fxParam = 0.015f;
        fxTransform->scale = 231.0f;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, fxTransform, 0x200001, -1, &fxParam);
        
        fxTransform->transl.x -= self->globalPosition.x;
        fxTransform->transl.y -= self->globalPosition.y;
        fxTransform->transl.z -= self->globalPosition.z;
        fxTransform->scale = 123.0f;

        for (i = 0; i < 15; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, fxTransform, 2, -1, NULL);
        }
        
        if (player->id != OBJ_Sabre) {
            dll_amSfx->Play(self, dWeaponHitSoundsSabre[mathRnd(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        } else {
            dll_amSfx->Play(self, dWeaponHitSoundsKrystal[mathRnd(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        dll_amSfx->Play(self, dHurtSounds[mathRnd(0, 4)], MAX_VOLUME, NULL, NULL, 0, NULL);
    } else {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_328, fxTransform, 0x200001, -1, NULL);
        
        fxTransform->transl.x -= self->globalPosition.x;
        fxTransform->transl.y -= self->globalPosition.y;
        fxTransform->transl.z -= self->globalPosition.z;
        dll_amSfx->Play(self, dWeaponHitSoundsSabre[mathRnd(3, 4)], MAX_VOLUME, NULL, NULL, 0, NULL);
        
        modGfxDLL = dllLoad(DLL_ID_106, 1);
        
        data_1B0[1] += mathRnd(0, 155);
        data_1B0[2] += mathRnd(0, 155);
        
        fxTransform->yaw = 0;
        fxTransform->pitch = 0;
        fxTransform->roll = 0;
        fxTransform->scale = 1.0f;
        
        modGfxDLL->vtbl->func0(self, 0, fxTransform, 1, -1, data_1B0);
        if (modGfxDLL != NULL) {
            dllFree(modGfxDLL);
        }
        
        if (useModGfx) {}
    }
}
#endif

// offset: 0x2534 | func: 17
static void SharpClaw_func_2534(u8 message, Object* self) {
    s32 index;
    s32 count;
    u32 j;
    Object** objects;
    
    for (objects = objGetObjects(&index, &count); index < count; index++) {
        if (self != objects[index]) {
            for (j = 0; j < ARRAYCOUNT(dAllyObjectIDs); j++) {
                if (objects[index]->id == (s32)dAllyObjectIDs[j]){
                    if (((DLL_IBaddie*)objects[index]->dll)->vtbl->get_fsa_state(objects[index]) == SharpClaw_LSTATE_12) {
                        ((DLL_IBaddie*)objects[index]->dll)->vtbl->send_message(objects[index], message);
                    }
                }
            }
        }        
    }
}

// offset: 0x267C | func: 18
s32 SharpClaw_func_267C(u8 message, Object* arg1) {
    s32 index;
    s32 count;
    u32 j;
    Object** objects;
    
    for (objects = objGetObjects(&index, &count); index < count; index++) {
        if (arg1 != objects[index]) {
            for (j = 0; j < ARRAYCOUNT(dAllyObjectIDs); j++) {
                if (objects[index]->id == (s32)dAllyObjectIDs[j]){
                    if (((DLL_IBaddie*)objects[index]->dll)->vtbl->get_fsa_state(objects[index]) == 0xC) {
                        ((DLL_IBaddie*)objects[index]->dll)->vtbl->send_message(objects[index], message);
                        return 1;
                    }
                }
            }
        }        
    }

    return 0;
}

// offset: 0x27C4 | func: 19
s32 SharpClaw_animState0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s16 modAnimIdx;

    if ((fsa->enteredAnimState) || (fsa->unk33A)) {
        fsa->enteredAnimState = TRUE;
        if (mathRnd(0, 2) == 0) {
            modAnimIdx = dIdleAnims[mathRnd(0, 3)];
            if (fsa->enteredAnimState) {
                objAnimSet(self, modAnimIdx, 0.0f, 0);
                fsa->unk33A = FALSE;
            }
        } else {
            if (fsa->enteredAnimState) {
                objAnimSet(self, SharpClaw_MODANIM0_22_Club_Idle_LOOP, 0.0f, 0);
                fsa->unk33A = FALSE;
            } 
            fsa->unk33A = FALSE;
        }
        
        fsa->unk33A = FALSE;
        fsa->enteredAnimState = FALSE;
        fsa->enteredLogicState = FALSE;
        fsa->speed = 0.0f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        fsa->unk2B0 = 8.0f;
        fsa->animTickDelta = 0.005f;
        self->velocity.x = 0.0f;
        self->velocity.y = 0.0f;
        self->velocity.z = 0.0f;
    }
    
    if (fsa->analogInputPower < 0.05f) {
        fsa->unk328 = 0;
        fsa->unk32A = 0;
        fsa->analogInputPower = 0.0f;
    }
    
    if (fsa->analogInputPower >= 0.2f) {
        return FSA_NEXTSTATE_ASYNC(SharpClaw_ASTATE_1_Walk);
    }
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x298C | func: 20
s32 SharpClaw_animState1Walk(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    f32 animProgress;
    s32 animChanged;
    f32* thresholds;
    s32 idx;
    s16* modAnims;

    baddie = self->data;
    fsa->animTickDelta = 0.025f;
    
    if (fsa->logicState == SharpClaw_LSTATE_2) {
        modAnims = dWalkAnimsPatrol;
    } else {
        modAnims = dWalkAnims;
    }
    
    if (fsa->analogInputPower < 0.005f) {
        fsa->analogInputPower = 0.005f;
    }
    
    gDLL_33_BaddieControl->vtbl->func3(self, fsa, baddie, 1.0f, 12.0f);
    
    animChanged = FALSE;
    animProgress = self->animProgress;

    //Find walk anim index
    idx = 0;
    while (self->curModAnimId != modAnims[idx] && idx != 3) {idx++;}
    if (idx >= 3) {
        idx = 0;
    }
    
    //Compare walk speed with walk anims' min/max speed thresholds, to determine if the walk anim should change
    thresholds = &dWalkSpeedThresholds[idx << 1];
    if (fsa->speed < thresholds[0]) {
        animChanged = TRUE;
        if (idx == 1) {
            return FSA_NEXTSTATE_SYNC(SharpClaw_ASTATE_0_Idle);
        }
        idx--;
    } else if (thresholds[1] <= fsa->speed) {
        animChanged = TRUE;
        if (idx == 0) {
            animProgress = 0.0f;
        }
        idx++;
    }
    
    //Change/advance walk animation
    if (animChanged) {
        objAnimSet(self, modAnims[idx], animProgress, 0);
    }
    objGetAnimChange(self, fsa->unk278, &fsa->animTickDelta);
    
    return 0;
}

// offset: 0x2B54 | func: 21
s32 SharpClaw_animState2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState || fsa->unk33A) {
        if (fsa->targetDist < 80.0f) {
            if (mathRnd(0, 4) == 0) {
                SharpClaw_func_2534(1, self);
                objData = baddie->objdata;
                objData->unkA = 2;
            }
            
            if (fsa->enteredAnimState) {
                objAnimSet(self, SharpClaw_MODANIM1_3_Hop_Backward, 0.0f, 0);
                fsa->unk33A = FALSE;
            }
            
            fsa->unk33A = FALSE;
        } else if (fsa->targetDist > 110.0f) {
            if (fsa->enteredAnimState) {
                objAnimSet(self, SharpClaw_MODANIM0_4_Walk_LOOP, 0.0f, 0);
                fsa->unk33A = FALSE;
            }
            
            fsa->unk33A = FALSE;
        } else {
            if (fsa->enteredAnimState) {
                objAnimSet(self, SharpClaw_MODANIM1_36_Hunched_Ready_LOOP, 0.0f, 0);
                fsa->unk33A = FALSE;
            } 
            
            fsa->unk33A = FALSE;
        }
        
        fsa->unk33A = FALSE;
        
        fsa->enteredAnimState = FALSE;
        fsa->enteredLogicState = FALSE;
        fsa->speed = 0.0f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        fsa->unk2B0 = 8.0f;
        fsa->animTickDelta = 0.035f;
        self->velocity.x = 0.0f;
        self->velocity.y = 0.0f;
        self->velocity.z = 0.0f;
    }
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 2);
    
    return 0;
}

// offset: 0x2D54 | func: 22
s32 SharpClaw_animState3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM0_4_Walk_LOOP, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.035f;
    fsa->unk278 = 0.0f;
    
    if (fsa->target != NULL) {
        objData = baddie->objdata;
        if (objData->unk10 <= 25) {
            fsa->unk33A = TRUE;
            gDLL_18_objfsa->vtbl->tick(self, fsa, 1.0f, 1.0f, sAnimStateCallbacks, sLogicStateCallbacks);
        }
    } 
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    return 0;
}

// offset: 0x2EA8 | func: 23
s32 SharpClaw_animState4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_2_Hop_Forward, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 5, dAttackSounds);
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.035f;
    fsa->unk278 = 1.0f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    return 0;
}

// offset: 0x2F98 | func: 24
s32 SharpClaw_animState5(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_3_Hop_Backward, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 2, dAttackSounds);
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.035f;
    fsa->unk278 = -1.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    return 0;
}

// offset: 0x3090 | func: 25
s32 SharpClaw_animState6(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_4_Hop_L, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.0336f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 3);
    
    return 0;
}

// offset: 0x3178 | func: 26
s32 SharpClaw_animState7(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_5_Hop_R, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.0336f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 3);
    
    return 0;
}

// offset: 0x3260 | func: 27
s32 SharpClaw_animState8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        fsa->unk341 = 1;
        fsa->animTickDelta = 0.027999999f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        objAnimSet(self, SharpClaw_MODANIM1_37_Hunched_Turn_90_R, 0.0f, 4);
        self->srt.yaw -= M_90_DEGREES;
    }
    
    objGetAnimChange(self, fsa->unk278, &fsa->animTickDelta);
    
    return 0;
}

// offset: 0x330C | func: 28
s32 SharpClaw_animState9(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        fsa->unk341 = 1;
        fsa->animTickDelta = 0.027999999f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        objAnimSet(self, SharpClaw_MODANIM1_38_Hunched_Turn_90_L, 0.0f, 4);
        self->srt.yaw += M_90_DEGREES;
    }
    
    objGetAnimChange(self, fsa->unk278, &fsa->animTickDelta);
    
    return 0;
}

// offset: 0x33B8 | func: 29
s32 SharpClaw_animState10(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_44_Hunched_Turn_Left, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.049f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 8);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x34A0 | func: 30
s32 SharpClaw_animState11(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_47_Hunched_Strafe_R, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.049f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 8);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3588 | func: 31
s32 SharpClaw_animState12Taunt1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_45_Taunt, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    if (fsa->enteredAnimState) {
        dll_amSfx->Play(self, dTauntSounds[0], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
    }
    
    fsa->unk341 = 8;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.0039f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x36C4 | func: 32
s32 SharpClaw_animState13Taunt2(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    Baddie* baddie = self->data;

    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_35_Taunt_Intro, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    if (fsa->enteredAnimState) {
        dll_amSfx->Play(self, dTauntSounds[1], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
    }
    
    fsa->unk341 = 8;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.0045f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, arg2, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, arg2, 0);
    
    return 0;
}

// offset: 0x3800 | func: 33
s32 SharpClaw_animState14Taunt3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_46_Taunt, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    if (fsa->enteredAnimState) {
        dll_amSfx->Play(self, dTauntSounds[2], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
    }
    
    fsa->unk341 = 8;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.006f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x393C | func: 34
s32 SharpClaw_animState15(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_36_Hunched_Ready_LOOP, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->unk341 = 1;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.021f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 8);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3A24 | func: 35
s32 SharpClaw_animState16AttackAnticlockwise(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    func_80026128(self, 0xB, 1, -1);
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_6_Attack_Anticlockwise, 0.24f, 0);
        fsa->unk33A = FALSE;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 2), dAttackSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 6, 0, dWeaponSounds);
    
    fsa->unk341 = 1;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.0112000005f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3B84 | func: 36
s32 SharpClaw_animState17AttackClockwise(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    func_80026128(self, 0xA, 1, -1);
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_7_Attack_Clockwise, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 2), dAttackSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 6, 2, dWeaponSounds);
    
    fsa->unk341 = 1;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.0182f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3CE8 | func: 37
s32 SharpClaw_animState18AttackOverhead(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    func_80026128(self, 0xA, 1, -1);
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_8_Attack_Overhead, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 2), dAttackSounds);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 6, 3, dWeaponSounds);
    
    fsa->unk341 = 1;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.0182f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3E4C | func: 38
s32 SharpClaw_animState19(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, dHitReactAnims[baddie->unk3F0], 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.021f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3F1C | func: 39
s32 SharpClaw_animState20(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->animTickDelta = 0.0f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    return 0;
}

// offset: 0x3F4C | func: 40
s32 SharpClaw_animState21(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_42_Knocked_Over_Intro, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.007f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 3);
    
    return 0;
}

// offset: 0x4008 | func: 41
s32 SharpClaw_animState22(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_43_Knocked_Over_Outro, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 3);
    
    return 0;
}

// offset: 0x40C4 | func: 42
s32 SharpClaw_animState23(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    fsa->unk341 = 3;
    fsa->animTickDelta = 0.005f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, SharpClaw_MODANIM1_1_Die, 0.0f, 0);
        fsa->unk33A = FALSE;
    }
    
    if (!(fsa->unk34A & 1)) {
        dll_amSfx->Play(self, SOUND_B21_Dissipating_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 1;
    }
    
    if (!(fsa->unk34A & 2) && (self->animProgress > 0.3f)) {
        baddie = self->data;
        dll_amSfx->Play(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 2;
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0);
    }
    
    return 0;
}

// offset: 0x424C | func: 43
s32 SharpClaw_animState24(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    fsa->unk341 = 0;
    fsa->animTickDelta = 0.01f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState) {
        dll_amSfx->Play(self, SOUND_B20_Low_Grunt, MAX_VOLUME, NULL, NULL, 0, NULL);
        objAnimSet(self, SharpClaw_MODANIM1_43_Knocked_Over_Outro, 0.0f, 0x10);
        fsa->unk33A = FALSE;
        self->opacity = OBJECT_OPACITY_MAX;
        baddie->unk3B2 |= 0x100;
    }
    
    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(SharpClaw_ASTATE_0_Idle);
    } else {
        return 0;
    }
}

// offset: 0x433C | func: 44
s32 SharpClaw_logicState0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->target != NULL) {
        if (fsa->logicStateTime > (4.0f * gUpdateRateF)) {
            return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_3);
        }

        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_1_Walk);
        gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);

    }

    return 0;
}

// offset: 0x4408 | func: 45
s32 SharpClaw_logicState1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_24);
    }

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_0);
    } else {
        return 0;
    }
}

// offset: 0x447C | func: 46
s32 SharpClaw_logicState2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}

// offset: 0x4494 | func: 47
s32 SharpClaw_logicState3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    if (fsa->enteredLogicState) {
        if (baddie->unk3B0 & 1) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_12_Taunt1);
        } else if (baddie->unk3B0 & 2) {
            dll_amSfx->Play(self, dTauntSounds[0], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
            return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_4);
        } else {
            return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_4);
        }
    }

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_4);
    } else {
        return 0;
    }
}

// offset: 0x4574 | func: 48
s32 SharpClaw_logicState4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;

    if (fsa->targetDist < (s32) (baddie->unk3E2 * 0.55f)) {
        baddie->unk3B0 &= ~0x10;
        return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_5);
    }
    
    return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_11);
}

// offset: 0x4600 | func: 49
s32 SharpClaw_logicState5(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;
    Object* target;

    baddie = self->data;

    target = gDLL_2_Camera->vtbl->get_target_object();

    if (fsa->enteredLogicState) {
        
        objData = baddie->objdata;
        objData->unk0 = 1;
        if ((target != self) && (baddie->unk3B0 & 1)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_13_Taunt2);
        } else {
            return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_12);
        }
    } else {
        return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_12);
    }
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);

    return 0;
}

// offset: 0x46F8 | func: 50
s32 SharpClaw_logicState6(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_4);
}

// offset: 0x4710 | func: 51
s32 SharpClaw_logicState7(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    SharpClaw_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (baddie->unk3B2 & 0x80) {
        objData->freezeTimer -= gUpdateRateF;
        if (objData->freezeTimer <= 0.0f) {
            objData->freezeTimer = 0.0f;
            baddie->unk3B2 &= ~0x80;
            if (fsa->hitpoints > 0) {
                return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_12);
            } else {
                return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_9_Dying);
            }
        }
    } else if (fsa->hitpoints <= 0) {
        return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_9_Dying);
    } else if (fsa->unk33A) {
        if (fsa->animState == SharpClaw_ASTATE_21) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_22);
        } else if (fsa->hitpoints < mathRnd(2, 4)) {
            return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_8);
        } else {
            baddie->unk3B6 = 300;
            return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_12);
        }
    }

    return 0;
}

// offset: 0x4858 | func: 52
s32 SharpClaw_logicState8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_5);
    } else if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_12);
    }

    return 0;
}

// offset: 0x48CC | func: 53
s32 SharpClaw_logicState9Dying(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_23_Dying);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        baddie->unk3B4 = 2;
        func_800267A4(self);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }

    if (self->opacity == 0) {
        if (self->setup == NULL) {
            objFreeObject(self);
        }

        objSendMesg(objGetPlayer(), 0xE0000, self, 0);
        return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_10_Dead);
    } else {
        return 0;
    }
}

// offset: 0x49E4 | func: 54
s32 SharpClaw_logicState10Dead(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredLogicState) {
        baddie->unk3B4 = 0;
        mainSetBits(baddie->unk39E, 0);
        mainSetBits(baddie->unk39C, 1);
    }
    
    return 0;
}

// offset: 0x4A5C | func: 55
s32 SharpClaw_logicState11(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Unk80009024* vox;
    Baddie* baddie;

    baddie = self->data;
    fsa->unk278 = 1.0f;
    fsa->speed = 1.0f;

    if (fsa->target != NULL) {
        if (fsa->enteredLogicState) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_1_Walk);
        }
        
        vox = &baddie->unk34C;
        fsa->xAnalogInput = 0.0f;
        fsa->yAnalogInput = 0.0f;
        bcopy(&self->srt.transl, vox, sizeof(Vec3f));
        bcopy(&fsa->target->srt.transl, &baddie->unk34C.unkC, sizeof(Vec3f));
        vox_func_80009024(vox, &baddie->unk374);

        if ((fsa->targetDist < (baddie->unk3E2 * 0.55f)) && (baddie->unk3B4 == 2)) {
            baddie->unk3B0 &= ~0x10;
            return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_5);
        }
        
        if (vox->unk25 == 0) {
            gDLL_18_objfsa->vtbl->func6(self, fsa, vox->unk18.x, vox->unk18.f[2], 0.0f, 0.0f, 60.0f);
        } else {
            gDLL_18_objfsa->vtbl->func6(self, fsa, vox->unk18.x, vox->unk18.f[2], 15.0f, 30.0f, 60.0f);
        }
    } else if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_0_Idle);
    }
    
    return 0;
}

// offset: 0x4CA4 | func: 56
s32 SharpClaw_logicState12(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;
    s32 pad[2];
    u8 flags;
    s32 random;

    baddie = self->data;
    
    if (fsa->enteredLogicState) {
        fsa->animTickDelta *= 4.0f;
    }
    
    if ((fsa->unk33A) || (fsa->enteredLogicState)) {
        objData = baddie->objdata;
        if (objData->unkA == 1) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_2);
            return 0;
        }
        
        SharpClaw_func_2534(1, self);

        if (gDLL_2_Camera->vtbl->get_target_object() != self) {
            if (objData->unk0 >= 7) {
                if (baddie->unk3B0 & 1) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_14_Taunt3);
                }
                objData->unk0 = 0;
                
                return 0;
            } else if (objData->unk0 > 0) {
                objData->unk0++;
            }
        }

        flags = gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f);

        if (baddie->unk3B6 > 20) {
            baddie->unk3B6 -= 20;
        }

        if ((objData->unkC < 2) || (objData->unkC >= 0xE)) {
            if (flags & 1) {
                if (objData->unk10 >= 0x56) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_4);
                } else if (objData->unk10 >= 0x2C) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_3);
                } else if (objData->unk10 < 0x1A) {
                    if ((objData->unkC >= 0xE) && (flags & 8)) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_6);
                    } else if (flags & 2) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_7);
                    } else if (flags & 4) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_5);
                    } else {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_17_Attack_Clockwise);
                    }
                } else {
                    if ((objData->unkC == 1) || (objData->unkC == 0xE)) {
                        random = mathRnd(0, 1);
                    } else {
                        random = mathRnd(0, 2);
                    }
                    
                    switch (random) {
                    case 0:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_16_Attack_Anticlockwise);
                        break;
                    case 1:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_17_Attack_Clockwise);
                        break;
                    default:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_18_Attack_Overhead);
                        break;
                    }
                    baddie->unk3B6 = 400;
                }
            } else {
                if (mathRnd(0, 1)) {
                    if ((objData->unkC >= 0xE) && (flags & 8)) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_6);
                    } else if (flags & 2) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_7);
                    } else if (flags & 4) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_5);
                    } else {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_17_Attack_Clockwise);
                    }
                } else {
                    switch (mathRnd(0, 2)) {
                    case 0:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_16_Attack_Anticlockwise);
                        break;
                    case 1:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_17_Attack_Clockwise);
                        break;
                    default:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_18_Attack_Overhead);
                        break;
                    }
                    baddie->unk3B6 = 400;
                }
            }
        } else if ((objData->unkC >= 2) && (objData->unkC < 4)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_10);
        } else if ((objData->unkC >= 4) && (objData->unkC < 8)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_9);
        } else if ((objData->unkC >= 8) && (objData->unkC < 12)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_8);
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_11);
        }
    }
    
    return 0;
}

// offset: 0x5398 | func: 57
s32 SharpClaw_logicState13(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* player;
    Object* sidekick;
    Unk80009024* vox;
    Baddie* baddie;
    Player_Data* playerData;

    baddie = self->data;
    player = objGetPlayer();
    sidekick = objGetSidekick();
    fsa->target = sidekick;

    playerData = player->data;
    
    if (self == playerData->unk0.target) {
        if (fsa->target != NULL) {
            ((DLL_ISidekick*)fsa->target->dll)->vtbl->func21(fsa->target, 0, 0);
        }
        
        fsa->target = player;
        fsa->unk33D = 0;
        gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, (s8*)&baddie->unk3B4, 0, 0, 0, 1);
        baddie->unk3B0 &= ~0x10;
        return FSA_NEXTSTATE_SYNC(SharpClaw_LSTATE_3);
    }
    
    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_1_Walk);
    fsa->xAnalogInput = 0.0f;
    fsa->yAnalogInput = 0.0f;
    
    vox = &baddie->unk34C;
    bcopy(&self->srt.transl, vox, sizeof(Vec3f));
    bcopy(&fsa->target->srt.transl, &baddie->unk34C.unkC, sizeof(Vec3f));
    vox_func_80009024(&baddie->unk34C, &baddie->unk374);
    
    if (vox->unk25 == 0) {
        gDLL_18_objfsa->vtbl->func6(self, fsa, vox->unk18.x, vox->unk18.z, 0.0f, 0.0f, 60.0f);
    } else {
        gDLL_18_objfsa->vtbl->func6(self, fsa, vox->unk18.x, vox->unk18.z, 15.0f, 30.0f, 60.0f);
    }
    
    return 0;
}
