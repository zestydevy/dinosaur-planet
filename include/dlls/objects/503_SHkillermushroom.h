#ifndef _DLLS_503_H
#define _DLLS_503_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

#define INHALE_RADIUS_SPEED 2.5f
#define INHALE_RADIUS_MAX 40.0f
#define SPORE_ATTACK_DURATION 120.0f

typedef struct {
    ObjSetup base;
    u16 attackCooldown;     //How long (in frames) until the mushroom leaves its attack outro state
    u16 regrowWaitDuration; //How long (in frames) until the mushroom starts regrowing after being collected/killed
    s16 gamebitAttacked;    //Optional gamebitID to set when the mushroom is damaged
    u8 aggroRadius;         //While in idle state, attacks the player with spores if they enter this radius
    u8 modelIdx;            //Mushroom design (only has 1 model, though)
} SHkillermushroom_Setup;

typedef struct {
    f32 timer;              //Cooldown timer for various behaviour (growing, attacking, etc.)
    f32 scaleMax;           //Maximum scale once grown (randomly varied by ±10%)
    f32 growDuration;       //How long it'll take for the mushroom to finish regrowing (after starting to scale up)
    f32 baseScale;          //Baseline scale, stored during setup
    f32 scaleSpeed;         //Rate of change of scale when growing
    s32 _unused14;
    s32 _unused18;
    s32 _unused1C;
    f32 attachX;            //Attach point coords for spore particles
    f32 attachY;            //Attach point coords for spore particles
    f32 attachZ;            //Attach point coords for spore particles
    f32 sporeInhaleRange;   //Player can be harmed by spores when inside this radius (grows during attack)
    f32 stunFxTimer;        //Countdown for periodically emitting particles while stunned
    u32 soundHandle;        //Controls the spore sound loop, and the stun sound loop
    s16 regrowWaitDuration; //How long (in frames) until the mushroom starts regrowing after being collected/killed
    u8 state;               //State Machine value
    u8 flags;               //Tracks various conditions for the State Machine (animation finished, vulnerable, can't hurt player)
    s32 _unused3C;
} SHkillermushroom_Data;

typedef enum {
	SHmushroom_FLAG_None = 0x0,
	SHmushroom_FLAG_Disable_Spore_Damage = 0x1,
	SHmushroom_FLAG_Animation_Finished = 0x2,
	SHmushroom_FLAG_Vulnerable = 0x4
} SHkillermushroom_Flags;

typedef enum {
	SHmushroom_STATE_0_Idle = 0,                //Attacks with spores when the player to approaches
	SHmushroom_STATE_1_Regrow = 1,              //Regrowing after being collected/killed
	SHmushroom_STATE_2_Dying_Outro = 2,         //(INACCESSIBLE) Unused state! Spores dissipate, mushroom topples over and fades out, before regrowing
	SHmushroom_STATE_3_Spore_Attack_Intro = 3,  //Spore attack: wind-up
	SHmushroom_STATE_4_Spore_Attack = 4,        //Spore attack: released harmful spores
	SHmushroom_STATE_5_Spore_Attack_Outro = 5,  //Spore attack: back to idle
	SHmushroom_STATE_6_Dying_Intro = 6,         //(INACCESSIBLE) Unused state! Mushroom releases spores and then starts toppling over
	SHmushroom_STATE_7_Angry_Fast = 7,          //(UNUSED) No State Machine code, but has an animation of recoiling and hopping angrily (faster playback)
	SHmushroom_STATE_8_Angry_Slow = 8,          //(UNUSED) No State Machine code, but has an animation of recoiling and hopping angrily (slower playback)
	SHmushroom_STATE_9_Stunned = 9,             //Stunned after attack
	SHmushroom_STATE_10_Hidden = 10             //Mushroom hidden after being collected, starts regrowing after a while
} SHkillermushroom_States;

typedef enum {
	SHmushroom_MODANIM_0_Idle_LOOP = 0,     //Spinning head in small circles
	SHmushroom_MODANIM_1_Spores_Intro = 1,  //Anticipates upwards with cap, then squashes down (one-shot)
	SHmushroom_MODANIM_2_Spores_LOOP = 2,   //Continuing from [mAnim1], spinning head in circles while spraying spores
	SHmushroom_MODANIM_3_Spores_Outro = 3,  //Continuing from [mAnim2], unsquashing and returning to idle (one-shot)
	SHmushroom_MODANIM_4_Topple_End = 4,    //Continuing from [mAnim5], topples forward onto its head, flopping its stem behind it (one-shot)
	SHmushroom_MODANIM_5_Topple_Start = 5,  //Squashes down while spinning head, then springs off the ground into curved pose (one-shot)
	SHmushroom_MODANIM_6_Angry = 6,         //Recoils its "neck" as through from an attack, then hops in place angrily (one-shot)
	SHmushroom_MODANIM_7_Spring_Jump = 7,   //(UNUSED) Progressively squashes its stem down like winding up a spring, until only its cap is visible flat on the ground, then jumps up massively (one-shot)
	SHmushroom_MODANIM_8_Spring_Fly = 8,    //(UNUSED) Continuing from [mAnim7], mushroom rockets away into sky! Maybe these two anims were for an older design of `SHrocketmushroom`? (one-shot)
	SHmushroom_MODANIM_9_Stunned_LOOP = 9   //Spinning head in wide dizzy circles
} SHkillermushroom_ModAnim;

#endif // _DLLS_503_H
