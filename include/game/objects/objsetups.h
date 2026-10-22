#ifndef _SYS_GAME_OBJECTS_OBJSETUPS_H
#define _SYS_GAME_OBJECTS_OBJSETUPS_H

#include "game/objects/object.h"

// ctrl: 4 | size: 0x20
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 mapAct;
/*19*/ u8 setupID; //assigned to Object->unkE0
/*1A*/ u8 yaw;
/*1C*/ u32 objGroupStatusBits;
} SetupPoint_Setup;

// ctrl: 5 | size: 0x40
typedef struct {
    ObjSetup base;
    u8 unk18[0x28 - 0x18];
    s8 unk28;
    u8 yaw;
    u8 scale;
    u8 unk2B[0x40 - 0x2B];
} checkpoint4_Setup;

// ctrl: 6 | size: 0x30
typedef struct {
/*00*/    ObjSetup base;
/*18*/    s8 unused18;
/*19*/    s8 messageArgBase;      //Base arg value used for sidekick-related collectables
/*1A*/    u8 scale;        //[DLL 272 "collectable": stored to Collectable_Data but unused]
                                    //[DLL 523 "SCcollectables": value for objHitInfo->unk52]
/*1B*/    u8 yaw8;
/*1C*/    s16 gamebitCollected;   //Set when collected: collectable vanishes when set
/*1E*/    s16 animMessage;
/*20*/    s8 unused20;
/*21*/    s8 unused21;    
/*22*/    u8 pitch8;
/*23*/    u8 roll8;
/*24*/    s16 gamebitSecondary;   //(Optional) [DLL 272 "collectable": show when set]
                                    //           [DLL 523 "SCcollectables": fall from tree]
/*26*/    u8 modelIdx;
/*27*/    u8 applyColourMultiplier;
/*28*/    u8 multiplyR;           //Colour multiplier for model
/*29*/    u8 multiplyG;           //Colour multiplier for model
/*2A*/    u8 multiplyB;           //Colour multiplier for model
/*2B*/    u8 unused2B;
/*2C*/    s16 gamebitCount;
/*2E*/    s16 mode;
} Collectable_Setup;

// ctrl: 8 | size: 0x20
typedef struct {
    ObjSetup base;
    s16 gametextID;                      //Gametext to print
    s8 unused1A;
    u8 unk1B;                            //Assigned to `objData->unk10` in setup, but otherwise unused
    u8 yaw;
} InfoPoint_Setup;

// ctrl: 9 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 roll;
/*1A*/ u8 halfWidth;  // x dimension (stored halved), centred around origin
/*1B*/ u8 halfHeight; // y dimension (stored halved), base of box at origin
/*1C*/ u8 halfLength; // z dimension (stored halved), centred around origin
/*1D*/ u8 effect;
/*1E*/ u8 _unk1E;     // Unused, but rarely set to 1 in ROM?
/*1F*/ u8 gamebitDisableValue;      // disabled if the target gamebit is this value
/*20*/ s16 gamebitEnable;           // -1 if this effect box is always enabled
/*22*/ u8 target;                   // see `EffectBox_Targets`
} EffectBox_Setup;

// ctrl: 18 | size: 0x24
typedef struct {
    ObjSetup base;
    u8 cameraID; //An identifier for selecting a specific camera (Trigger Objects' CameraAction params need to match this)
    u8 unk19;
    u8 fov;
    u8 flags;   //1: aim yaw at player, 2: aim pitch at player, 4: use player roll 
    s16 yaw;
    s16 pitch;
    s16 roll;
} StaticCamera_Setup;

// ctrl: 30 | size: 0x24
typedef struct {
    ObjSetup base;
    u8 modelIndex;          //Which model to use
    u8 modAnimIndex;        //Which model animation to use
    u8 animProgress;        //Initial tValue for the animation (expressed from 0-256 instead of 0-1)
    u8 animSpeed;           //Animation playback speed (stored at 10000x actual value)
    u8 scaleMultiplier;     //Multiplies the size of the object and its shadow (64 = 1x, 128 = 2x, 32 = 0.5x, etc.)
    u8 yaw;
    s16 unused1E;
    s16 gamebitVisible;     //Toggles object visibility
} Ctrl30_Setup;

// ctrl: 31 | size: 0x1c
typedef struct {
/*00*/	ObjSetup base;
/*18*/	union Unk18{
        u8 byte[2];
        s16 halfword;
    } unk18;
/*1A*/	u8 unk1A;
} SideSetupObj_Setup;

// ctrl: 32 | size: 0x1c
typedef struct {
    ObjSetup base;
    s16 gamebitUnlocked;    //GamebitID determining whether the sidekick is unlocked (i.e. Kyte rescued)
    u8 sidekickIndex;       //Whether this should load Tricky/Kyte (see `SideLoad_Indices`)
} SideLoad_Setup;

// ctrl: 35 | size: 0x24
typedef struct {
    ObjSetup base;
    s16 textureIndex;       //Primary material: index in TABLES.bin subfile #14 ("scroll table")
    s16 gamebitActivate;    //gamebitID that activates animation (or -1 to skip bit check)
    s8 uSpeedB;             //U scroll speed for secondary blend material 
    s8 vSpeedB;             //V scroll speed for secondary blend material 
    s8 uSpeedA;             //U scroll speed for primary material              
    s8 vSpeedA;             //V scroll speed for primary material              
    s16 blendTextureIndex;  //Secondary blend material: index in TABLES.bin subfile #14 (optional, -1 if unused)
                            //Blend material is used for multitextured scrolling water, etc.
} TexScroll_Setup;

// ctrl: 37 | size: ?
typedef struct {
    ObjSetup base;
    u8 yaw;
    u8 unk19;       //Unknown purpose. Maybe meant as a lifetime for the cannon or its cannonballs?
} Cannon_Setup;

// ctrl: 38 | size: 0x24
typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} Projectile_Setup;

// ctrl: 39 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 unk19;
/*1A*/ s8 warpID;
/*1B*/ s8 objectSeqIndex;
/*1C*/ s8 isInboundWarp;
/*1D*/ s8 mode;
/*1E*/ s8 quarterRange;
/*20*/ s16 gamebit;
} Warp_Setup;

// ctrl: 40 | size: 0x20
typedef struct {
/*0x10*/ ObjSetup base;
/*0x18*/ s16 playerNearbyRange; //`SnowHorn_FLAG_80_Player_Nearby` is set when the player is inside a multiple of this range (not used for anything, though)
/*0x1A*/ s16 squirtInverval;    //For the Blue SnowHorn: the time (in seconds) between squirting water at the player
/*0x1C*/ s8 yaw;
/*0x1D*/ s8 characterIdx;       //Which SnowHorn this is (see `SnowHorn_CharacterIndices`)
} SnowHorn_Setup;

// ctrl: 41 | size: 0x20
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 pad1B[0x20 - 0x1B];    
} InvHit_Setup;

// ctrl: 42 | size: 0x20
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 fallPlayerRange; //The player must be inside this lateral distance from the icicle, for it to fall
    u8 scale;
    s16 gamebitEnabled; //The icicle's state machine won't run until this gamebit is set (when specified)
    s16 fallDelayTimer; //The icicle will start falling after this delay, after the fall conditions are activated
} IMIcicle_Setup;

// ctrl: 43 | size: 0x24
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

// ctrl: 44 | size: 0x24
typedef struct {
    ObjSetup base;
    u16 attackCooldown;     //How long (in frames) until the mushroom leaves its attack outro state
    u16 regrowWaitDuration; //How long (in frames) until the mushroom starts regrowing after being collected/killed
    s16 gamebitAttacked;    //Optional gamebitID to set when the mushroom is damaged
    u8 aggroRadius;         //While in idle state, attacks the player with spores if they enter this radius
    u8 modelIdx;            //Mushroom design (only has 1 model, though)
    u8 _pad20[0x24 - 0x20];
} SHkillermushroom_Setup;

// ctrl: 45 | size: 0x20
typedef struct {
    ObjSetup base;
    s16 regrowthDelay;
    s16 sporeLaunchInterval;    //Frames between spore launches (actual interval has some randomised variance added to this value)
    s16 gamebitGrow;            //When specified, plant only grows when gamebit set (and gamebit unset upon exploding)
    s8 sporeAngularRange;       //Variance in launched SHspores' flight yaw
    s8 yaw;
} SHrocketmushroom_Setup;

// ctrl: 49 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 isSharpClawBike;             // Whether the bike can only be driven by SharpClaw
/*1A*/ s16 gamebitUnlocked;            // The bike can only be mounted when this gamebit is set
/*1C*/ u8 racetrackIdx;                // Which race the bike is used in (CloudRunner Fortress vs. Golden Plains)
/*1D*/ u8 unk1D;
/*1E*/ s16 gamebitFinished;            // Bike disappears and stops updating when this gamebit is set
/*20*/ u8 yJoySharpClaw;               // Strength factor for SharpClaws' simulated yJoy steering value
} SnowBike_Setup;

// TODO: many more DLLs to add this to
// ctrl: 51 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
    // Union provided for code readability, all fields must be s16 to conform with the actual objcontrol definition.
    union {
        // Base definition:
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 genericA;
            /*1C*/ s16 genericB;
            /*1E*/ s16 bitA;
            /*20*/ s16 bitB;
        } generic;
        // Object-specific definitions:
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 range;
            /*1C*/ s16 unused1;
            /*1E*/ s16 unused2;
            /*20*/ s16 unused3;
        } dockpoint;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 preemptTime;
            /*1C*/ s16 unused;
            /*1E*/ s16 unlockedBit;
            /*20*/ s16 stateBit;
        } cfBigDoorSwitch;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 extraDamageStages; // aka health - 1. if zero, no model is drawn and it's assumed the block shapes are the visible surface
            /*1C*/ s16 shapeAnimatorID; // animator ID of block shapes to hide/remove collision of when blasted
            /*1E*/ s16 blastedGamebit; // bit to set when blasted
            /*20*/ s16 hitCountGamebit; // for persisting the hit count
        } blastedObj;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 opacityThreshold;
            /*1C*/ s16 unused;
            /*1E*/ s16 bitViewed;
            /*20*/ s16 bitEnabled;
        } apertureSymbol;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 offIntervalDuration;
            /*1C*/ s16 firingDuration;
            /*1E*/ s16 bitEnabled;
            /*20*/ s16 unused;
        } wcLaser;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 hitsAnimatorID;
            /*1C*/ s16 unused1;
            /*1E*/ s16 unused2;
            /*20*/ s16 unused3;
        } wcFloorTile;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 scale; //0x2000 is 1x scale
            /*1C*/ s16 roll;
            /*1E*/ s16 unused1;
            /*20*/ s16 unused2;
        } firepole;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 unused1;
            /*1C*/ s16 unused2;
            /*1E*/ s16 gamebitFinished;
            /*20*/ s16 unused3;
        } wcDial;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 unused1;
            /*1C*/ s16 unused2;
            /*1E*/ s16 gamebitVisible;
            /*20*/ s16 unused3;
        } wcMagicBridge;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 unk1A;
            /*1C*/ s16 unused;
            /*1E*/ s16 destroyedBit;
            /*20*/ s16 crashBit;
        } robotAnimPatrol;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 sightRange;
            /*1C*/ s16 unused1;
            /*1E*/ s16 unused2;
            /*20*/ s16 unused3;
        } cfPrisonGuard;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 unk1A;
            /*1C*/ s16 reverseBit;
            /*1E*/ s16 playerInsideBit;
            /*20*/ s16 unused;
        } windLift;
        struct {
            /*18*/ s8 yaw;
            /*19*/ s8 type;
            /*1A*/ s16 preemptTime;
            /*1C*/ s16 unused;
            /*1E*/ s16 enabledGamebit;
            /*20*/ s16 stateGamebit;
        } toggleSwitch;
        struct {
            /*18*/ s8 activateRange;
            /*19*/ s8 unk19;
            /*1A*/ s16 actionIdxBase;
            /*1C*/ s16 gamebitActivate;
            /*1E*/ s16 gamebitActionIdxOffset;
            /*20*/ s16 unused2;
        } envEmitter;
    };
} Ctrl51_Setup;

// ctrl: 52 | size: 0x20
typedef struct {
    ObjSetup base;
    s16 gamebitID;          //Checked during setup, but does nothing (maybe for tracking lighting fire/putting it out?)
    u8 scale;               //Scale multiplier (100 = 1.0)
    u8 unk1B;               //Unknown - stored in setup but otherwise unused
    u8 alwaysIlluminates;   //When nonzero, fire emits light even when the player isn't nearby
    u8 modGfxArg;           //Boolean affecting the fire mesh modGfx calls, unknown purpose
    u16 lfxIdx;             //Param for the light emitter
} CampFire_Setup;

#endif
