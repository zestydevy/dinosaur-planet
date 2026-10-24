#ifndef _SYS_GAME_OBJECTS_SETUPS_GENERIC_H
#define _SYS_GAME_OBJECTS_SETUPS_GENERIC_H

#include "game/objects/object.h"
#include "macros.h"

/**
 * A lot of objects are defined with this controlNo but attribute different meaning to most fields. 
 * The base Generic_Setup struct makes sense for many objects but for the rest an alias struct can be
 * defined instead using the following macro. It's expected that aliases *only* rename fields and do
 * not change their datatypes.
 */
#define GENERIC_SETUP_STRINGIFY(name) #name
#define GENERIC_SETUP(name, fields) \
    /*ctrl: 51*/ \
    typedef struct { ObjSetup base; fields } name; \
    STATIC_ASSERT(sizeof(name) == sizeof(Generic_Setup), GENERIC_SETUP_STRINGIFY(name)" size is incorrect.")

// ctrl: 51 | size: 0x24
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 genericA;
/*1C*/ s16 genericB;
/*1E*/ s16 bitA;
/*20*/ s16 bitB;
} Generic_Setup;

GENERIC_SETUP(BalloonBaddie_Setup, 
/*18*/ s8 unk18;
/*19*/ s8 attackRange;
/*1A*/ s16 unk1A;
/*1C*/ s16 respawnTimer;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(SwarmBaddie_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 unused2;
/*1E*/ s16 unused3;
/*20*/ s16 unused4;
)

GENERIC_SETUP(PirahnaBaddie_Setup, 
/*18*/ s8 unk18;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 respawnTime;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(TurtleBaddie_Setup, 
/*18*/ s8 unk18;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(SHvines_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused1;
/*1A*/ s16 unused2;
/*1C*/ s16 flameDistance;
/*1E*/ s16 gamebitBurnt;
/*20*/ s16 unused3;
)

GENERIC_SETUP(Iceblast_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unk19; //collision-related
/*1A*/ s16 timer;
/*1C*/ u16 unk1C; // wrong type
/*1E*/ s16 unused2;
/*20*/ s16 unused3;
)

GENERIC_SETUP(FlameBlast_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unk19;
/*1A*/ s16 timer;
/*1C*/ u16 unk1C; // wrong type
/*1E*/ s16 unused2;
/*20*/ s16 unused3;
)

GENERIC_SETUP(Crate_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 useNearbyCollectable; //When set: a nearby collectable is repositioned when the crate is destroyed (no Gplay timer).
                                //When not set: crate drops an Energy Egg, and it reappears after a Gplay timer expires.
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitDestroyed; //Gamebit to set when destroyed. Doesn't affect crate reappearing!
/*20*/ s16 unused3;
)

GENERIC_SETUP(KrazoaText_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 stringID;
/*1C*/ s16 unused3;
/*1E*/ s16 unused4;
/*20*/ s16 unused5;
)

GENERIC_SETUP(SmallBasket_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 storedItemID;
/*1A*/ s16 storedItemQuantity;
/*1C*/ s16 unk1C;                   //some time (in seconds)
/*1E*/ s16 gamebit;
/*20*/ s16 autoThrowRadius;
)

GENERIC_SETUP(MediumCrate_Setup, 
/*18*/ s8 unk18;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 gamebit;
/*20*/ s16 unused;
)

GENERIC_SETUP(Scarab_Setup, 
/*18*/ s8 yaw; // not used
/*19*/ s8 unused1;
/*1A*/ s16 lifetime;
/*1C*/ s16 unused2;
/*1E*/ s16 unused3;
/*20*/ s16 unused4;
)

GENERIC_SETUP(Scorpion_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 unused2;
/*1E*/ s16 unused3;
/*20*/ s16 unused4;
)

GENERIC_SETUP(FallLadders_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 modelIdx;        //Which ladder model to use
/*1A*/ s16 raisedOffsetY;  //When raised, the ladder starts off this far above its base position
/*1C*/ s16 unused;
/*1E*/ s16 gamebitRaise;   //Ladder rises up by sequence when this gamebit is set (exclusive to `VFP_Ladders2`)
/*20*/ s16 gamebitFall;    //Ladder starts falling when this gamebit is set
)

GENERIC_SETUP(LanternFireFly_Setup, 
/*18*/ s8 varianceY;
/*19*/ s8 effectType;      //Decides what kind of partFX are created for the firefly, and whether to create LFXEmitters
/*1A*/ s16 lifetime;       //How long the firefly will stay around before being unloaded
/*1C*/ s16 varianceZ;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(PortalSpellDoor_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 hitsAnimatorID;      //animatorID of the HITS line(s) that should be removed when door is open
/*1A*/ s16 scale;
/*1C*/ s16 pitch;
/*1E*/ s16 gamebitActivated;   //gamebitID set when door is open
/*20*/ s16 unused;
)

GENERIC_SETUP(MMP_Bridge_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 modelIdx;                    //Which bridge model to use
/*1A*/ s16 duration;                   //How long the bridge should stay visible for (in seconds), excluding the fade-in/fade-out times
/*1C*/ s16 useTravellingFade;          //If this is TRUE, the bridge's fade in/out vertex animation will travel along the bridge in progressive/overlapping way
/*1E*/ s16 gamebitVisible;             //Stores the bridge's visibility state
/*20*/ s16 stayVisible;                //If this is FALSE, the bridge will disappear after its timer expires
)

GENERIC_SETUP(EnvEmitter_Setup, 
/*18*/ s8 activateRange;
/*19*/ s8 unk19;
/*1A*/ s16 actionIdxBase;
/*1C*/ s16 gamebitActivate;
/*1E*/ s16 gamebitActionIdxOffset;
/*20*/ s16 unused;
)

GENERIC_SETUP(Firepole_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 scale; //0x2000 is 1x scale
/*1C*/ s16 roll;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(DoorOpen_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 pitch;       //Stored to objData but otherwise unused. Possibly pitch, based on surrounding fields.
/*1A*/ s16 roll;
/*1C*/ s16 scale;      //Stored to objData, but otherwise unused. Likely scale based on the `*0.01f` (typical of objSetup scale).
/*1E*/ s16 unused;
/*20*/ s16 gamebit;    //PartFX and modGfx are created when this gamebit is set. 
)

GENERIC_SETUP(LightPole_Setup, 
/*18*/ s8 endFrameNoGamebit;           //When no gamebitFinished is specified, the TextureAnimator will hold on this frame at the end of its animation
/*19*/ s8 halfYaw;                     //Yaw (shifted by 9 instead of the usual 8)
/*1A*/ s16 endFrame;                   //The TextureAnimator will finish animating once it reaches this frame
/*1C*/ s16 animSpeed;                  //Animation speed for the TextureAnimator
/*1E*/ s16 gamebitFinished;            //GamebitID set when the TextureAnimator's animation is finished
/*20*/ s16 gamebitStart;               //The TextureAnimator's animation begins when this gamebit is set
)

GENERIC_SETUP(TexFrameAnimator_Setup, 
/*18*/ s8 loopStartFrame;   //Frame to loop back to after reaching endFrame (only when looping)
/*19*/ s8 materialIndex;    //Index within object's local Blocks model's materials (the frame of the material's texture animation will be manipulated)
/*1A*/ s16 endFrame;        //End frame of texture animation
/*1C*/ s16 speed;           //Playback rate when advancing through the texture animation
/*1E*/ s16 gamebitFinished; //GamebitID to set when end of animation reached (also prevents looping)
/*20*/ s16 gamebitPlay;     //GamebitID to query, starting animation
)

GENERIC_SETUP(WindLift_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 unk1A;
/*1C*/ s16 reverseBit;
/*1E*/ s16 playerInsideBit;
/*20*/ s16 unused;
)

GENERIC_SETUP(LaserBeam_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused1;
/*1A*/ s16 beamLength;
/*1C*/ s16 mode;
/*1E*/ s16 gamebit;
/*20*/ s16 unused2;
)

GENERIC_SETUP(CFPrisonGuard_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 sightRange;
/*1C*/ s16 unused1;
/*1E*/ s16 unused2;
/*20*/ s16 unused3;
)

GENERIC_SETUP(RobotAnimPatrol_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 unk1A;
/*1C*/ s16 unused;
/*1E*/ s16 destroyedBit;
/*20*/ s16 crashBit;
)

GENERIC_SETUP(ToggleSwitch_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 preemptTime;
/*1C*/ s16 unused;
/*1E*/ s16 enabledGamebit;
/*20*/ s16 stateGamebit;
)

GENERIC_SETUP(CFBigDoorSwitch_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 preemptTime;
/*1C*/ s16 unused;
/*1E*/ s16 unlockedBit;
/*20*/ s16 stateBit;
)

GENERIC_SETUP(BlastedObject_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 extraDamageStages; // aka health - 1. if zero, no model is drawn and it's assumed the block shapes are the visible surface
/*1C*/ s16 shapeAnimatorID; // animator ID of block shapes to hide/remove collision of when blasted
/*1E*/ s16 blastedGamebit; // bit to set when blasted
/*20*/ s16 hitCountGamebit; // for persisting the hit count
)

GENERIC_SETUP(CFTreasSharpy_Setup, 
/*18*/ s8 yaw8;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
)

GENERIC_SETUP(CFLamp_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 unk1A;
/*1C*/ s16 unused3;
/*1E*/ s16 unused4;
/*20*/ s16 unused5;
)

GENERIC_SETUP(CFCheapGalleon_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 fullGalleonObjGroup;      //The map objectGroup for the full-detail Galleon and its objects
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitForceLowDetail;   //The cheap Galleon stays at max opacity and exits control early when this gamebit is set
/*20*/ s16 unused3;
)

GENERIC_SETUP(IMSpaceRing_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused1;
/*1A*/ s16 spinSpeed;
/*1C*/ s16 rollSpeed;
/*1E*/ s16 unused2;
/*20*/ s16 unused3;
)

GENERIC_SETUP(DFDockpoint_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 range;
/*1C*/ s16 unused1;
/*1E*/ s16 unused2;
/*20*/ s16 unused3;
)

// DFSHshrine, MMshrine, etc
GENERIC_SETUP(Shrine_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 testStartRange; //Test begins when player inside this radius (stored multiplied by 256)
/*1C*/ s16 unused3;
/*1E*/ s16 unused4;
/*20*/ s16 unused5;
)

GENERIC_SETUP(MMP_GeyserVent_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 unused3;
/*1C*/ s16 unused4;
/*1E*/ s16 gamebitOff;
/*20*/ s16 unused5;
)

GENERIC_SETUP(CCkrazoaTablet_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 objectSeqIndex;
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebit;
/*20*/ s16 unused3;
)

GENERIC_SETUP(CCDockDoor_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused1;
/*1A*/ s16 unused2;
/*1C*/ s16 unused3;
/*1E*/ s16 gamebitDestroy;
/*20*/ s16 unused4;
)

GENERIC_SETUP(ECSHCup_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 cupIndex;
/*1C*/ s16 unused3;
/*1E*/ s16 unused4;
/*20*/ s16 unused5;
)

GENERIC_SETUP(GPSH_Scene_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 index;
/*1A*/ s16 dropoffRange;
/*1C*/ s16 unused1; //Gamebit field? Always -1
/*1E*/ s16 gamebit; //Unused, but this the same gamebit that the DLL sets upon placing this item
/*20*/ s16 unused2;
)

// GPSH_flybaddie, WGSH_flybaddie
GENERIC_SETUP(Flybaddie_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 unk1A;
/*1C*/ s16 unused3;
/*1E*/ s16 unused4;
/*20*/ s16 unused5;
)

GENERIC_SETUP(NWSH_Sacrifice_Setup, 
/*18*/ s8 unk18;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 unk1E;
/*20*/ s16 unused;
)

GENERIC_SETUP(SHSpore_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 angularRange;                //maximum angular distance from desired travel angle
/*1C*/ s16 windAngle;                   //desired direction the spore should travel in
/*1E*/ s16 unused3;
/*20*/ s16 unused4;
)

GENERIC_SETUP(SCMeterBlock_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 weedsNeeded;            //The total Tumbleweeds that must be damaged to fill the meter
/*1C*/ s16 gamebitDeactivated;     //Meter deactivated while this Gamebit is set
/*1E*/ s16 gamebitActivated;       //Meter deactivated while this Gamebit is unset
/*20*/ s16 gamebitMeterProgress;   //Gamebit tracking how many Tumbleweeds have been offered to the meter
)

GENERIC_SETUP(DIMIceWall_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 health;
/*1C*/ s16 flameRange;
/*1E*/ s16 gamebitMelted;
/*20*/ s16 unused3;
)

GENERIC_SETUP(DIMWoodDoor_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused1;
/*1A*/ s16 unused2;
/*1C*/ s16 unused3;
/*1E*/ s16 gamebitDestroyed;
/*20*/ s16 unused4;
)

GENERIC_SETUP(DIMMagicBridge_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 modelIdx;                    //Which bridge model to use
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitVisible;             //Stores the bridge's visibility state
/*20*/ s16 unused3;
)

GENERIC_SETUP(DIMTent_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused1;
/*1A*/ s16 tentIndex;     //An index (0-11 inclusive) for deciding which tent holds the bridge cog
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitBurnt;  //Set when the tent's been destroyed, prevents it reappearing
/*20*/ s16 unused3;
)

GENERIC_SETUP(DIM2Conveyor_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused1;
/*1A*/ s16 speed;
/*1C*/ s16 unused2;
/*1E*/ s16 unused3;
/*20*/ s16 unused4;
)

GENERIC_SETUP(DIM2icicle_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused1;
/*1A*/ s16 unused2;
/*1C*/ s16 unused3;
/*1E*/ s16 gamebitFell;
/*20*/ s16 unused4;
)

GENERIC_SETUP(SB_ShipGunBroke_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 unused3;
/*1C*/ s16 unused4;
/*1E*/ s16 gunDestroyedGamebit;
/*20*/ s16 unused5;
)

GENERIC_SETUP(DeadDino_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 survived;        //The EarthWalker animates and can be talked to when this is set
/*1A*/ s16 deadModAnimIdx; //This modAnimIdx is used when `objSetup->survived == FALSE`
/*1C*/ s16 pitch;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(WLWorm_Setup, 
/*18*/ s8 unk18;
/*19*/ s8 unused1;
/*1A*/ s16 particleID;
/*1C*/ s16 particleCount;
/*1E*/ s16 unused2;
/*20*/ s16 unused3;
)

GENERIC_SETUP(WMwallpower_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 fxRadius;        //2D player range for creating magic wave effects (stored halved)
/*1A*/ s16 roll;
/*1C*/ s16 pitch;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(WMFireFly_Setup, 
/*18*/ s8 varianceY;
/*19*/ s8 effectType;
/*1A*/ s16 fxRange;
/*1C*/ s16 varianceZ;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(WL_spiritplace_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 mode;
/*1A*/ s16 pitch;
/*1C*/ s16 unk1C;
/*1E*/ s16 bit1;
/*20*/ s16 bit2;
)

GENERIC_SETUP(WL_seqpoint_Setup, 
/*18*/ s8 rotation; // yaw >> 8
/*19*/ s8 triggerCondition;
/*1A*/ s16 triggerRange;
/*1C*/ s16 seqno; // object seq index
/*1E*/ s16 conditionBit; // if not -1, bit that determines if the seqpoint is enabled. required value depends on the trigger condition
/*20*/ s16 triggeredBit; // if not -1, this bit is set to 1 once seqpoint is triggered. prevents re-trigger if bit is already 1
)

GENERIC_SETUP(WL_Crystal_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 modelIdx;
/*1A*/ s16 unused1;
/*1C*/ s16 scale;
/*1E*/ s16 unused2;
/*20*/ s16 unused3;
)

GENERIC_SETUP(WMPlanets_Setup, 
/*18*/ s8 scaleMultiplier;
/*19*/ s8 orbitalRadius; //divided by 16
/*1A*/ s16 modelIdx;
/*1C*/ s16 unused1;
/*1E*/ s16 unused2;
/*20*/ s16 unused3;
)

GENERIC_SETUP(WM_MovePlatform_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused1;
/*1A*/ s16 playerRange;
/*1C*/ s16 unused2;
/*1E*/ s16 unused3;
/*20*/ s16 unused4;
)

GENERIC_SETUP(VFP_DoorSwitch_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 roll;
/*1A*/ s16 unk1A;
/*1C*/ s16 pitch;
/*1E*/ s16 bit;
/*20*/ s16 unused;
)

GENERIC_SETUP(VFP_FlamePoint_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 health;
/*1C*/ s16 flameCommandRange;
/*1E*/ s16 gamebitFlamed;
/*20*/ s16 unused3;
)

GENERIC_SETUP(DFPLift_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused2;
/*1A*/ s16 unk1A;           //Sets value for objdata->unused0, but otherwise unused
/*1C*/ s16 alreadyEnabled;  //Lift ignores activation gamebit and silently ascends into position
/*1E*/ s16 unk1E;           //Sets value for objdata->unusedC, but otherwise unused
/*20*/ s16 gamebitActivated;
)

GENERIC_SETUP(DFPfloorbar_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 rowIndex;
/*1A*/ s16 unused;
/*1C*/ s16 inverseScale;
/*1E*/ s16 unk1E;
/*20*/ s16 gamebitLowered;
)

GENERIC_SETUP(DFP_ForceAway_Setup, 
/*18*/ s8 yRadiusQuarter;
/*19*/ s8 type;
/*1A*/ s16 xRadius;
/*1C*/ s16 zRadius;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(DFP_RotatePuzzle_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unk19;               //Stored to objData->unkF during setup, but otherwise unused
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitUnused;
/*20*/ s16 gamebitSpin;        //Puzzle wheel rotates when this gamebit is set
)

GENERIC_SETUP(DFP_Statue1_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 statueIdx;       //Which of the 4 statues this is (see note below)
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitUnused;
/*20*/ s16 gamebitMove;    //The statue moves when this gamebit is set (by hitting the Projectile Switch underneath)
)

GENERIC_SETUP(DFP_PerchSwitch_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unused1;
/*1A*/ s16 unused2;
/*1C*/ s16 unused3;
/*1E*/ s16 gamebitLocked;
/*20*/ s16 gamebitPulled;
)

GENERIC_SETUP(Textblock_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 seqIndex;                //sequence index (in objDef) to play when translated
/*1A*/ s16 gametextID;             //gametext fileID of subtitles to display when translated
/*1C*/ s16 unused;
/*1E*/ s16 gamebitTranslated;      //gamebit set when Krazoa Translator is used on textblock
/*20*/ s16 gamebitInteractable;    //gamebit queried to tell if textblock should be powered on
)

GENERIC_SETUP(DFP_Platform1_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unk19;
/*1A*/ s16 roll;
/*1C*/ s16 unused;
/*1E*/ s16 gamebitMoveZ;
/*20*/ s16 gamebitMoveY;
)

GENERIC_SETUP(DBSpike_Setup, 
/*18*/ s8 roll;
/*19*/ s8 unused1;
/*1A*/ s16 yaw;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebit;
/*20*/ s16 unused3;
)

GENERIC_SETUP(DBMagicBridge_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 modelIdx;                    //Which bridge model to use
/*1A*/ s16 gamebit1A;
/*1C*/ s16 unused;
/*1E*/ s16 gamebitVisible;             //Stores the bridge's visibility state
/*20*/ s16 gamebit20;
)

GENERIC_SETUP(DBExpBit_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 modelIdx;
/*1C*/ s16 doDraw;
/*1E*/ s16 unused3;
/*20*/ s16 unused4;
)

GENERIC_SETUP(DBWaterPlant_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 timerThreshold;
/*1C*/ s16 scale;
/*1E*/ s16 gamebitRestoreState;
/*20*/ s16 gamebitDamaged;
)

GENERIC_SETUP(DBPlatform_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 yOffset;
/*1A*/ s16 quarterVisRadius;
/*1C*/ s16 transitionDuration;
/*1E*/ s16 gamebitStandingOn;
/*20*/ s16 gamebitStateTransition;
)

GENERIC_SETUP(DBDustGeezer_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 randomnessRange;     //Controls the random variance in launched gems' start position/speed
/*1A*/ s16 cooldownDuration;   //Cooldown after launching all gems
/*1C*/ s16 launchedGemCount;   //How many gems to launch after being dug up (max of 8)
/*1E*/ s16 unused2;
/*20*/ s16 gamebitHidden;      //Hides geyser during setup if it's already dug up
)

GENERIC_SETUP(DBFakeHorizon_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 divisor;
/*1C*/ s16 unused3;
/*1E*/ s16 unused4;
/*20*/ s16 unused5;
)

GENERIC_SETUP(CRSpellStone_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 gamebitIndexWin;
/*1A*/ s16 unused2;
/*1C*/ s16 unused3;
/*1E*/ s16 gamebitVehicleHit;
/*20*/ s16 gamebitFreeTick;
)

GENERIC_SETUP(DRLavaControl_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 dataIndex;
/*1A*/ s16 freezeDuration;   //how long it takes to freeze with Ice Blast spell
/*1C*/ s16 unused2;
/*1E*/ s16 gameBitFrozen;    //gamebit to set when fully cooled with Ice Blast
/*20*/ s16 unused3;
)

GENERIC_SETUP(DR_NPC_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 characterType;    //0: SharpClaw, 1: GuardClaw
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitFinished; //Gamebit set when the character's interactions are finished (SharpClaw fed, GuardClaw tricked)
/*20*/ s16 unused3;
)

GENERIC_SETUP(DRExplodeDoor_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 mode;                //Sets object transform flags when nonzero, and enables control function when set to 2
/*1A*/ s16 explosionScale;     //Scale for modGfxDLL function call transform, and for object's objHits
/*1C*/ s16 modGfxArg1;         //Arg1 for the modGfxDLL's function call (type of model to create?)
/*1E*/ s16 gamebitExplode;     //Explodes when this gamebit is set, or sets this gamebit when damaged by barrel explosion
/*20*/ s16 unused;
)

GENERIC_SETUP(SPItemBeam_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 unused2;
/*1A*/ s16 itemIndex; //The kind of shop item being highlighted (see `ShopItemIndices`)
/*1C*/ s16 unused3;
/*1E*/ s16 unused4;
/*20*/ s16 unused5;
)

GENERIC_SETUP(WCBeacon_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 modelIndex;
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitLit;
/*20*/ s16 gamebitRise;
)

GENERIC_SETUP(WCPushBlock_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 modelIndex;
/*1A*/ s16 puzzlePieceID;
/*1C*/ s16 unused2;
/*1E*/ s16 unused3;
/*20*/ s16 unused4;
)

GENERIC_SETUP(WCTile_Setup, 
/*18*/ s8 unused1;
/*19*/ s8 modelIdx;
/*1A*/ s16 puzzlePieceID;
/*1C*/ s16 unused2;
/*1E*/ s16 unused3;
/*20*/ s16 unused4;
)

GENERIC_SETUP(WCTrexStatue_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 modelIdx;
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitActivated;
/*20*/ s16 unused3;
)

GENERIC_SETUP(WCApertureSymbol_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 opacityThreshold;
/*1C*/ s16 unused;
/*1E*/ s16 bitViewed;
/*20*/ s16 bitEnabled;
)

GENERIC_SETUP(WCLaser_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 offIntervalDuration;
/*1C*/ s16 firingDuration;
/*1E*/ s16 bitEnabled;
/*20*/ s16 unused;
)

GENERIC_SETUP(WCDial_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitFinished;
/*20*/ s16 unused3;
)

GENERIC_SETUP(WCMagicBridge_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 unused1;
/*1C*/ s16 unused2;
/*1E*/ s16 gamebitVisible;
/*20*/ s16 unused3;
)

GENERIC_SETUP(WCFloorTile_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 type;
/*1A*/ s16 hitsAnimatorID;
/*1C*/ s16 unused1;
/*1E*/ s16 unused2;
/*20*/ s16 unused3;
)

GENERIC_SETUP(WLWallTorch_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 mode;
/*1A*/ s16 scale;
/*1C*/ s16 flags;
/*1E*/ s16 gamebit;
/*20*/ s16 unused;
)

GENERIC_SETUP(WMvein_Setup, 
/*18*/ s8 fxRadius;        //Player radius for creating effects (stored quartered)
/*19*/ s8 modGfxFuncArg1;
/*1A*/ s16 modGfxIndex;    //DLL index in the modgfx bank
/*1C*/ s16 animateRoll;    //Whether to advance object roll while player inside fxRadius 
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(DIMCannonBall_Setup, 
/*18*/ s8 yaw;
/*19*/ s8 unk19;
/*1A*/ s16 velocityY;
/*1C*/ s16 velocityX;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

GENERIC_SETUP(DIMLavaBall_Setup, 
/*18*/ s8 unk18;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 unused1;
/*20*/ s16 unused2;
)

#undef GENERIC_SETUP_STRINGIFY
#undef GENERIC_SETUP

#endif
