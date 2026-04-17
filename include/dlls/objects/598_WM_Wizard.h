#ifndef _DLLS_598_H
#define _DLLS_598_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/objhits.h"
#include "dlls/engine/6_amsfx.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
} WMWizard_Setup;

typedef struct {
    Vec3f home;             //Initial position
    f32 animSpeed;          //Animation delta when Randorn is walking around the hall at random
    f32 objHitsValue;       //ObjHits-related
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    s16 walkWaitTimer;      //Pause before moving to next random walk destination
    s16 talkTimer;          //Randomised delay when calling out to Krystal
    u8 walkIndexFlags;      //Random walk destination index, and objHits flag (upper half-byte)
    u8 prevWalkIndex;       //Previous random walk destination index
    u8 hasMetKrystal;       //Boolean: whether you've seen Krystal's 1st cutscene with Randorn
    u8 activeSeqIndex;      //Used by animCallback funcs to tell which sequence is playing
    u8 objectID;
    u8 timesFed;            //Incremented when offering food from the inventory
} WMWizard_Data;

typedef enum {
    WMWizard_FLAG_None = 0,
    WMWizard_FLAG_80 = 0x80
} WMWizard_Flags;

typedef enum {
    WMWizard_OBJSEQ_0_Unk = 0,
    WMWizard_OBJSEQ_1_Offering_Magic_Refill = 1,
    WMWizard_OBJSEQ_2_Discovery_Falls_Reminder = 2,
    WMWizard_OBJSEQ_3_Walk = 3
} WMWizard_ObjSeqs; //Sequences invoked by Randorn's own object

#define WALK_STOPPING 12
#define WALK_STOPPED 13

typedef struct {
    union {
        struct {
            f32 x;          //Walk goal x (relative to home position)
            f32 z;          //Walk goal z (relative to home position)
            f32 modAnimIDIdleIntro; //Initial intro anim after stopping (typed as a float, strangely)
            f32 modAnimIDIdleLoop;  //Looped anim after idle intro      (typed as a float, strangely)
            f32 animSpeed;
        };
        f32 f[5];
    };
} RandomWalkData;

typedef enum {
    Randorn_MODANIM_0_Standing_Thoughtfully_LOOP = 0, //Pondering with left hand at mouth, right hand on hip
    Randorn_MODANIM_1_Sitting_Holding_Head_LOOP = 1, //Sitting at edge of podium, head down and left hand held to temple, right hand planted behind on the podium
    Randorn_MODANIM_2_Sitting_Hands_on_Knees_LOOP = 2, //Looking ahead, breathing slowly, upper body weight supported by hands on knees
    Randorn_MODANIM_3_Sitting_Exhausted_LOOP = 3, //Head down, hunched forward, elbows leaning heavily on knees (initial pose for 1st cutscene with Krystal)
    Randorn_MODANIM_4_Sitting_Exhausted_Head_Lift = 4, //Slowly looking up from previous pose [mAnim3] (one-shot)
    Randorn_MODANIM_5_Sitting_Exhausted_Placing_Hands = 5, //Slowly resting both hands at knee height onto something in front of Randorn, starting from previous pose [mAnim4]: as though he's taking the hands of someone helping him up? (one-shot)
    Randorn_MODANIM_6_Sitting_Exhausted_Placed_Hands = 6, //Starting roughly from previous pose [mAnim5], head slowly dips down/left before gradually looking up again (one-shot)
    Randorn_MODANIM_7_Sitting_Attempt_to_Stand_Collapse = 7, //Leaning back slightly and pressing down onto podium's edge with hands, attempts to stand up but collapses forward weakly; ending with head slumped over, right arm flopped on knee at elbow, left hand still grabbing podium. (one-shot)
    Randorn_MODANIM_8_Lying_Down_Looking_Up = 8, //Lying down to left, outstretched, pressing down with fingertips to crane head into looking up (one-shot) (root at podium height, rotated to left) 
    Randorn_MODANIM_9_Standing_Peer_Over_Left_Shoulder = 9, //From limping stand pose, slowly looks over left shoulder (one-shot) (root rotated to right)
    Randorn_MODANIM_10_Standing_Talking = 10, //Looking ahead, gesturing with open arms as though explaining something (one-shot)
    Randorn_MODANIM_11_Standing_Listening_LOOP = 11, //Head slightly down, arms slightly poised, listening while breathing slowly
    Randorn_MODANIM_12_Limp_Walk_LOOP = 12, //Limping forward with weight mostly on right foot, left leg injured. Left arm held near chest, right arm flopped down without swinging.
    Randorn_MODANIM_13_Standing_Greeting = 13, //From shoulder-peering pose [mAnim9], greets Krystal and turns to face forward (one-shot)
    Randorn_MODANIM_14_Shielding_Eyes_Peer = 14, //Starting from defensive pose [mAnim16], slowly pushes right hand out to peer forward (one-shot)
    Randorn_MODANIM_15_Shielding_Eyes_Start = 15, //Curls left into defensive pose with right hand shielding eyes (one-shot)
    Randorn_MODANIM_16_Shielding_Eyes_Stagger = 16, //Starting from defensive pose [mAnim15], staggers back a few steps and unshields face slightly (one-shot)
    Randorn_MODANIM_17_Shielding_Eyes_LOOP = 17, //With right hand held out [mAnim14] and left hand near hip, breathing slowly while looking forward.
    Randorn_MODANIM_18_Shielding_Eyes_Shoosh = 18, //From [mAnim17], raises left hand to mouth to shoosh Krystal. (one-shot)
    Randorn_MODANIM_19_Shielding_Eyes_End = 19, //From a pose similar to [mAnim16], slowly drops guard and looks left to watch Quan Ata Lachu float back into Krystal (one-shot)
    Randorn_MODANIM_20_Standing_Idle_L_LOOP = 20, //Standing with slightly slumped/exhausted posture, breathing slowly, weight slightly more on left leg.
    Randorn_MODANIM_21_Standing_Idle_Swap = 21, //From [mAnim20], slowly swivels upper body slightly to right while still looking ahead (into [mAnim22]) (one-shot)
    Randorn_MODANIM_22_Standing_Idle_R_LOOP = 22, //Standing with slightly slumped/exhausted posture, breathing slowly, weight slightly more on right leg.
    Randorn_MODANIM_23_Standing_Gesture = 23, //From [mAnim22], nods head and gestures out ahead with right arm (one-shot)
    Randorn_MODANIM_24_Standing_Reaching_Forward = 24, //Taking small steps as though to a precipice, leans body weight forward and reaches out beyond edge with right hand: reaching out to someone across divide? (one-shot)
    Randorn_MODANIM_25_Shielding_Eyes_Alt_Stagger = 25, //Similar to a combined [mAnim16 and mAnim14]: a single animation where he staggers back shielding his eyes, then quickly drops guard (without pushing his hand out as in [mAnim14]) into a hunched pose (one-shot)
    Randorn_MODANIM_26_Shielding_Eyes_Alt_Dismiss = 26, //From [mAnim25], waves right hand ahead dismissively (one-shot)
    Randorn_MODANIM_27_Shielding_Eyes_LOOP = 27, //From [mAnim25 or mAnim26], standing hunched while breathing slowly.
    Randorn_MODANIM_28_Limp_Standing_Talk = 28, //From a pose similar to [mAnim12], looks ahead and gestures with left hand as though talking (one-shot)
    Randorn_MODANIM_29_Standing_Slump = 29, //From a pose similar to [mAnim12], looks up and shrugs forward slightly with both palms, then slumps neck down while shaking head (one-shot)
    Randorn_MODANIM_30_Standing_Slump_Look = 30, //From [mAnim29], slowly lifts head and upper body to look up (one-shot)
    Randorn_MODANIM_31_Shielding_Eyes_Alt_Start = 31, //Similar to [mAnim15], but doesn't curl up as far defensively. Begins with arms up and ahead, before curling to left and shielding eyes with right hand (one-shot)
    Randorn_MODANIM_32_Sitting_Slumped_to_Left_Collapse = 32, //Body slumped fully onto left side while sitting on podium. Attempts to lift head and gesture with left fingers, but soon passes out (one-shot)
    Randorn_MODANIM_33_Collapsed_on_Floor = 33, //Lying prone with right hand reached out forward to grasp edge of podium, slowly lifts head to look up (one-shot) (root rotated 180)
    Randorn_MODANIM_34_Lying_Prone_to_Sitting = 34, //Lying prone from [mAnim33], pushes down on right hand and hoists self into sitting pose on podium (one-shot) (root ends rotated ~90, and seems like Object should rotate during anim, so that Randorn starts out facing towards the podium, and ends facing out/away from podium)
    Randorn_MODANIM_35_Sitting_Receive_Food_Middle = 35, //From [mAnim36], turns his upper body to look left and cups an item in both hands (one-shot)
    Randorn_MODANIM_36_Sitting_Receive_Food_Start = 36, //Sitting hunched at edge of podium similar to [mAnim2], slowly lifts head to look up/left (one-shot)
    Randorn_MODANIM_37_Sitting_Receive_Food_End = 37, //From [mAnim35], turns upper body back central and takes a bite from the food (one-shot)
    Randorn_MODANIM_38_Sitting_Slumped_LOOP = 38, //Similiar to [mAnim2], but heavier breathing
    Randorn_MODANIM_39_Sitting_Offering_SpellBook = 39, //From [mAnim40], turns left to pick up the SpellBook and offer it forward to Krystal (one-shot)
    Randorn_MODANIM_40_Sitting_Slumped_to_Lean = 40, //From a sitting pose like [mAnim2], plants left arm onto podium's edge and looks up (one-shot)
    Randorn_MODANIM_41_Sitting_Offering_Magic = 41, //From [mAnim40], turns left to pick up a Magic Crystal and offer it forward to Krystal (one-shot)
    Randorn_MODANIM_42_Sitting_Offering_to_Lean = 42, //From [mAnim39/mAnim41], returns to leaning with left hand planted on podium [mAnim43]
    Randorn_MODANIM_43_Sitting_Lean_LOOP = 43, //Sitting, leaning onto left arm planted on the podium's edge, breathing slowly.
    Randorn_MODANIM_44_Sitting_Lean_Refuse = 44, //From [mAnim43], leans slightly forward and grasps something in right hand, then returns it - refusing food when feeling unwell? (one-shot)
    Randorn_MODANIM_45_Sitting_Lean_to_Slump = 45, //From [mAnim43], head reels faintly and body slumps down onto left elbow (one-shot)
    Randorn_MODANIM_46_Sitting_Lean_Slump_LOOP = 46, //From [mAnim45], sitting in heavy lean onto left elbow, breathing slowly
    Randorn_MODANIM_47_Sitting_Shoo_LOOP = 47, //From a sitting pose like [mAnim2], leans onto left hand, then uses his right hand to shoo away a creature at his left ankle (likely Skeetlas)
    Randorn_MODANIM_48_Limp_Back_Ache_LOOP = 48, //From frail standing pose like [mAnim12], slumps forward then grasps near his lower back
    Randorn_MODANIM_49_Limp_into_Wall_Resting_L_LOOP = 49, //From frail standing pose like [mAnim12], goes into left wall lean [mAnim57]
    Randorn_MODANIM_50_Limp_into_Wall_Resting_R_LOOP = 50, //From frail standing pose like [mAnim12], goes into right wall lean [mAnim56]
    Randorn_MODANIM_51_Stand_Up_LOOP = 51, //From a sitting pose like [mAnim2], turns left to press down on podiums with hands, and lifts up into standing pose.
    Randorn_MODANIM_52_Sitting_to_Head_in_Hands_LOOP = 52, //From a sitting pose like [mAnim2], leans forward and places head in hands: into [mAnim58]
    Randorn_MODANIM_53_T_Pose = 53, //TO-DO: is this really empty, or just missing an AMAP?
    Randorn_MODANIM_54_Walk_LOOP = 54, //Healthier walk anim, not limping.
    Randorn_MODANIM_55_Turn_Right_LOOP = 55, //For walking, pivot to face right.
    Randorn_MODANIM_56_Wall_Resting_R_LOOP = 56, //Leaning with right hand pressed against wall, left hand high on hip, tired posture.
    Randorn_MODANIM_57_Wall_Resting_L_LOOP = 57, //Leaning with left hand pressed against wall, right hand high on hip, tired posture.
    Randorn_MODANIM_58_Sitting_Head_in_Hands_LOOP = 58, //Hunched forward with head in hands, elbows on knees, breathing slowly.
    Randorn_MODANIM_59_Walk_LOOP = 59, //Similar to [mAnim54]
    Randorn_MODANIM_60_Sit_Down = 60, //From standing, sitting back onto podium (one-shot)
    Randorn_MODANIM_61_Standing_Gesture_to_Left = 61, //Standing with left foot forward and upper body swivelled left, gestures with arms as though talking (one-shot)
    Randorn_MODANIM_62_Limp_Turn = 62, //Swivelling in place while limping (one-shot)
    Randorn_MODANIM_63_Sitting_Offer_Object = 63 //With left hand resting on podium, right hand reaches to pick up something small on left, then presents it to Krystal (different to offering SpellBook/Magic) (one-shot) 
} WMWizard_ModAnim;

/*0x0*/ static Unk80026DF4 dObjHitsData[] = {
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0}, 
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0}, 
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0}, 
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0}, 
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0}, 
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0}, 
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0},
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0}, 
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0}, 
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0}, 
    {SOUND_377_Metal_Smack, NO_SOUND, -1, -1, 0, 0, 0}
};

/*0xDC*/ static f32 dRandomWalkData[] = {
    0,      0,    Randorn_MODANIM_0_Standing_Thoughtfully_LOOP,  Randorn_MODANIM_0_Standing_Thoughtfully_LOOP,    0.02,   //middle of Krazoa floor mural
    79,     152,  Randorn_MODANIM_20_Standing_Idle_L_LOOP,      Randorn_MODANIM_20_Standing_Idle_L_LOOP,        0.01,   //near top-right pillar (furthest from antechamber door, on right when facing podium)
    138,   -6,  Randorn_MODANIM_20_Standing_Idle_L_LOOP,     Randorn_MODANIM_20_Standing_Idle_L_LOOP,       0.02,  //near podium
    -73,   -48, Randorn_MODANIM_20_Standing_Idle_L_LOOP,     Randorn_MODANIM_20_Standing_Idle_L_LOOP,       0.02,  //near middle of Krazoa floor mural, closer to orrery door
    -248,  -7,  Randorn_MODANIM_0_Standing_Thoughtfully_LOOP, Randorn_MODANIM_0_Standing_Thoughtfully_LOOP,   0.02,  //beside orrery door
    0,     0,   Randorn_MODANIM_0_Standing_Thoughtfully_LOOP, Randorn_MODANIM_0_Standing_Thoughtfully_LOOP,   0.02
};

#endif // _DLLS_598_H
