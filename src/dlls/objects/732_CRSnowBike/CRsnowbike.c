#include "common.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/curves.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/math.h"
#include "sys/objhits.h"
#include "sys/objlib.h"
#include "sys/objtype.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/267_checkpoint4.h"
#include "unktypes.h"

typedef struct {
    s8 unk0[0xE];
    s8 xJoy;            //Joystick X (simulated for CPU racers)
    s8 yJoy;            //Joystick Y (simulated for CPU racers)
    s8 unk10;
} CRSnowBike_SteerData;

typedef struct {
    Vec3f vGravity; //Gravity acceleration vector (relative to bike's own coordinate space)
    Vec3f velocity; //Bike's objectSpace velocity (Z points out the back of the bike, so forward velocity unit vector is {0,0,-1}) 
    f32 accelerationFactorPerFrame; // Per sixtieth of a second
    f32 gravityFactor;              // Scaling factor for gravity
    f32 accelerationFactor;         // Per second
    f32 unk24;                      // Unused
    f32 gravity;                    // Base unit for gravity
    f32 friction;                   // Applied while in contact with the ground
    f32 airResistance;              // Applied in proportion with square of speed
} CRSnowBike_MotionData;

typedef enum {
    STATE_0_Parked,
    STATE_1,
    STATE_2_Driving
} CRSnowBike_States;

typedef enum {
    CRSnowBike_FLAG_0_None = 0,
    CRSnowBike_FLAG_1_Finished = 1,
    CRSnowBike_FLAG_2_Driving_In_Void = 2,
    CRSnowBike_FLAG_4_Grounded = 4,
    CRSnowBike_FLAG_8_Race_Started = 8,
    CRSnowBike_FLAG_10_Was_In_Sequence = 0x10,
    CRSnowBike_FLAG_20_SharpClaw_Driver = 0x20
} CRSnowBike_Flags;

typedef enum {
    CRSnowBike_SOUNDFLAG_None = 0,
    CRSnowBike_SOUNDFLAG_Engine = 1,
    CRSnowBike_SOUNDFLAG_Hiss = 2,
    CRSnowBike_SOUNDFLAG_Jets = 4
} CRSnowBike_SoundFlags;

typedef struct {
    s16 started;
    s16 ended;
    s16 unk;
} CRSnowBike_Gamebits;

typedef enum {
    RACETRACK_0_CloudRunner_Fortress,
    RACETRACK_1_Golden_Plains
} CRSnowBike_Racetracks;

#define PLAYER_NOT_NEARBY 0
typedef enum {
    SIDE_LEFT = 1,
    SIDE_RIGHT = 2
} CRSnowBike_Sides;

typedef enum {
    CRSnowBike_ANIMCMD_2_Lose_Race = 2,
    CRSnowBike_ANIMCMD_3_Free_Fuel_Gauge = 3
} CRSnowBike_AnimCommands;

typedef struct {
    ObjSetup base;
    u8 yaw;
    u8 isSharpClawBike;             // Whether the bike can only be driven by SharpClaw
    s16 gamebitUnlocked;            // The bike can only be mounted when this gamebit is set
    u8 racetrackIdx;                // Which race the bike is used in (CloudRunner Fortress vs. Golden Plains)
    u8 unk1D;
    s16 gamebitFinished;            // Bike disappears and stops updating when this gamebit is set
    u8 yJoySharpClaw;               // Strength factor for SharpClaws' simulated yJoy steering value
} CRSnowBike_Setup;

typedef struct {
    SRT srtCurves;                   //Stores CPU racers' current checkpoint/curve-interpolated position
    RaceStruct raceData;             //Race/checkpoints-related data
    s8 _unk3C[0x48 - 0x3C];
    u8 racetrackIdx;                 //See `CRSnowBike_Racetracks`
    u8 unk49;
    DLL27_Data collision;            //Terrain collider
    CRSnowBike_MotionData motion;    //Bike motion: objectSpace velocity (-Z forward), gravity, resistance, etc.
    CRSnowBike_SteerData steering;   //Driver controls
    DLL_IModgfx* modGfxDLLFlames;    //Effects DLL for bike's thruster flames
    DLL_IModgfx* modGfxDLLWaves;     //Effects DLL for bike's wave effects when turning sharply
    CRSnowBike_Gamebits* gamebitIDs; //Gamebits used when starting a race/etc.
    s8 _unk300[0x330 - 0x300];
    Vec3f wsCollisionCoords[5];      //worldSpace coords of dCollisionPoints' collisions
    s8 _unk36C[0x384 - 0x36C];
    f32 stallFactor;                 //Affects how quickly the bike loses speed after a damaging impact
    Vec3f attachPointCoords;
    f32 soundFactorRumble;
    f32 soundFactorJets;
    f32 forwardSpeed;
    Vec3f prevTranslate;        //The bike's worldSpace coordinates at the start of the tick
    Vec3f wsFrontOfBike;        //The front end of the bike's worldSpace coordinates
    u32 soundHandleEngine;
    u32 soundHandleJets;
    u32 soundHandleHiss;
    u32 soundHandleRumble;
    s32 fuelAmount;         //Fuel gauge level
    Vec3f maxVelocity;      //Velocity component limits, in bike's objectSpace 
    s32 _unk3D8;
    s16 yawOffset;
    s16 rollOffset;
    s16 yaw;
    s16 pitch;
    s16 roll;
    s16 pitchOffset;        //Tilt forward/back while airborne, based on joyY
    s16 _unk3E8;
    s16 fxTimer;
    s8 stallFrames;         //Slows the bike down for this many frames, after it's damaged
    u8 mountingFrom;        //Which side the player's approaching from when the bike's parked
    u8 numCollisionPoints;
    u8 flags;
    s8 state;
    u8 raceRanking;         //Race placement/ordinal ranking
    s8 framesInAir;         //How long the bike's been airborne, in frames
    s8 collisionFlags;      //Bitfield tracking collisions on different points on the bike
    u8 branchFlagCPU : 1;   //Randomised Boolean used for SharpClaw racers' checkpoint pathing
} CRSnowBike_Data;

/*0x0*/ static CRSnowBike_Gamebits dRaceGamebits[] = {
    { BIT_CF_Race_Started, BIT_498, BIT_49C },
    { BIT_GP_Sharpclaw_Jetbike_Cutscene2, BIT_77E, BIT_77F }
};
/*0xC*/ static u32 dCheckpointUIDs[][3] = {
    {0x00030c60, 0x00030c60, 0x00030c60}, 
    {0x00034dd3, 0x00034dc7, 0x00034dc9}
};
/*0x24*/ static Vec3f dTerrainTestPoints[] = {
    VEC3F(-6.5, 0, -7),
    VEC3F(6.5, 0, -7),
    VEC3F(6.5, 0, 7),
    VEC3F(-6.5, 0, 7)
};
/*0x54*/ static f32 dTerrainRadii[] = {
    0.0f, 0.0f, 0.0f, 0.0f
};
/*0x64*/ static Vec3f dCollisionPoints[] = {
    VEC3F(0, 1, -18), 
    VEC3F(-6.5, 1, -7), 
    VEC3F(6.5, 1, -7), 
    VEC3F(-6, 1, 11), 
    VEC3F(6, 1, 11)
};
/*0xA0*/ static f32 dCollisionRadii[] = {
    5, 
    4, 
    4, 
    4.5, 
    4.5
};
/*0xDC*/ static Vec3f data_DC[] = { //unused?
    VEC3F(-5, 0, 0), 
    VEC3F(5, 0, 0), 
    VEC3F(0, 2.0001223, 0), 
    VEC3F(0, 0, 2.0157478), 
    VEC3F(0, 0, 0)
};
/*0xF0*/ static Vec3f dBikeMountSidePoints[] = {
    VEC3F(14.5, 0, 9), 
    VEC3F(-14.5, 0, 9)
};

//Textures: not referenced in the DLL itself, but possibly intended to keep the modGfx's textures loaded
/*0x0*/ static Texture* sTexVerticalWave;       //(TEX0_486) 1 vertical wave 
/*0x4*/ static Texture* sTexThrusterFlames;     //(TEX0_253) thruster flames 
/*0x8*/ static Texture* sTexDriftWaves;         //(TEX0_136) 3 horizontal waves, used for effect when turning

/*0x10*/ static SRT sRotationTransform;

#include "prevent_bss_reordering.h"

static s32 CRSnowBike_sharpclaw_update_race_pathing(Object* self, CRSnowBike_Data* objData, f32 arg2);
void CRSnowBike_sharpclaw_start_race(Object* self);
static void CRSnowBike_handle_partfx_and_impact_sounds(Object* self, CRSnowBike_Data* objData, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols);
static void CRSnowBike_update_motion(Object* self, CRSnowBike_Data* objData, CRSnowBike_MotionData* motion, f32 updateRate, s32 applyTerrainCollision);
static void CRSnowBike_init_motion_data(Object* self, CRSnowBike_MotionData* motion);
static void CRSnowBike_sharpclaw_update_motion(Object* self, CRSnowBike_Data* objData, CRSnowBike_MotionData* motion, f32 updateRate, s32 applyTerrainCollision);
static void CRSnowBike_read_joy_stick(Object* self, CRSnowBike_SteerData* steering, u8 controllerPort, s32 buffer);
static void CRSnowBike_get_bike_matrix(Object* self, CRSnowBike_Data* objData, MtxF* oMtx, s32 useYawOffset, s32 useRoll, s32 usePitch);
static void CRSnowBike_handle_damage_or_fuel_collisions(Object* self, CRSnowBike_Data* objData);
static int CRSnowBike_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void CRSnowBike_handle_collisions(Object* self, CRSnowBike_Data* objData, DLL27_Data* collision);
static s32 CRSnowBike_sharpclaw_handle_steering(Object* self, CRSnowBike_Data* arg1, CRSnowBike_Data* objData, CRSnowBike_SteerData* steering);
static void CRSnowBike_update_collision_points(Object* self, CRSnowBike_Data* objData);
static void CRSnowBike_handle_engine_sfx_and_modgfx(Object* self, CRSnowBike_Data* objData, f32 forwardSpeed, s32 yJoy, s8* arg6, u8 soundFlags);

// offset: 0x0 | func: 0
static s32 CRSnowBike_sharpclaw_update_race_pathing(Object* self, CRSnowBike_Data* data, f32 arg2) {
    RaceCheckpointSetup* checkpointSetup;
    s32 sp30;
    CRSnowBike_Data* objData;
    
    objData = self->data;

    checkpointSetup = gDLL_4_Race->vtbl->func8(data->raceData.unk10, &sp30);
    if (checkpointSetup->unk20[1] == -1) {
        objData->branchFlagCPU = rand_next(0, 1);
    }
    
    return gDLL_4_Race->vtbl->func5(&data->srtCurves, &data->raceData, arg2, 1, 0, objData->branchFlagCPU);
}

// offset: 0xE4 | ctor
void CRSnowBike_ctor(void* dll) { }

// offset: 0xF0 | dtor
void CRSnowBike_dtor(void* dll) { }

// offset: 0xFC | func: 1 | export: 0
void CRSnowBike_setup(Object* self, CRSnowBike_Setup* setup, s32 reset) {
    s32 pad;
    s32 pad2;
    s32 flagsValue;
    CRSnowBike_Setup* objSetup;
    u8 dTerrainColliderArgs[] = {5, 5, 5, 5};
    CRSnowBike_Data* objData;

    objSetup = (CRSnowBike_Setup*)self->setup;
    objData = self->data;
    bzero(objData, sizeof(CRSnowBike_Data));
    
    objData->yaw = (setup->yaw & 0xFF) << 8;
    objData->modGfxDLLFlames = NULL;
    objData->modGfxDLLWaves = NULL;
    self->srt.yaw = objData->yaw;

    self->animCallback = CRSnowBike_anim_callback;
    
    gDLL_27->vtbl->init(&objData->collision, 0, 0x01040007, 1);
    gDLL_27->vtbl->setup_terrain_collider(&objData->collision, ARRAYCOUNT(dTerrainTestPoints), dTerrainTestPoints, dTerrainRadii, dTerrainColliderArgs);
    
    if (setup->isSharpClawBike) {
        objData->numCollisionPoints = 3;
    } else {
        objData->numCollisionPoints = 5;
    }

    CRSnowBike_update_collision_points(self, objData);
    CRSnowBike_init_motion_data(self, &objData->motion);

    func_80023D30(self, 0, 0.0f, 0);

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_4000 | OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }

    if (setup->isSharpClawBike) {
        objData->flags |= CRSnowBike_FLAG_20_SharpClaw_Driver;
    }

    objData->raceData.unk10 = -1;
    objData->raceData.unk14 = -1;
    objData->raceData.unk18 = -1;
    objData->racetrackIdx = setup->racetrackIdx;
    objData->unk49 = setup->unk1D;
    
    objData->srtCurves.transl.x = self->srt.transl.x;
    objData->srtCurves.transl.y = self->srt.transl.y;
    objData->srtCurves.transl.z = self->srt.transl.z;
    
    obj_add_object_type(self, OBJTYPE_Vehicle);

    //Load textures (not used by DLL itself, possibly just intended to cache the modgfx textures)
    sTexVerticalWave = tex_load_deferred(TEXTABLE_186);     //(TEX0_486) 1 vertical wave? 
    sTexThrusterFlames = tex_load_deferred(TEXTABLE_89);    //(TEX0_253) thruster flames 
    sTexDriftWaves = tex_load_deferred(TEXTABLE_3C);        //(TEX0_136) 3 horizontal waves, for drift effect

    //Hide the bike if a gamebit is set
    if (main_get_bits(objSetup->gamebitFinished)) {
        STUBBED_PRINTF(" FInished Is SEt for Some Reason \n");
        flagsValue = CRSnowBike_FLAG_1_Finished;
    } else {
        flagsValue = CRSnowBike_FLAG_0_None;
    }
    objData->flags |= flagsValue;
    
    objData->gamebitIDs = &dRaceGamebits[setup->racetrackIdx];
}

/*0x24*/ static const char str_24[] = " FInished Is SEt for Some Reason \n";

// offset: 0x398 | func: 2 | export: 1
void CRSnowBike_control(Object* self) {
    CRSnowBike_Setup* objSetup;
    Object* player;
    s32 pad;
    f32 yOffset;
    MtxF bikeMtx;
    Vec3f wsPoint;
    CRSnowBike_MotionData* bikeMotion;
    CRSnowBike_Data* objData;
    CRSnowBike_SteerData* driverSteering;
    f32 dx;
    f32 dz;
    s32 sp5C;
    f32 forwardSpeed;
    s32 updateIdx;
    s32 flagValue;
    CRSnowBike_MotionData* motion;

    objData = self->data;
    objSetup = (CRSnowBike_Setup*)self->setup;
    
    bikeMotion = &objData->motion;
    driverSteering = &objData->steering;
    
    yOffset = 0.0f;
    
    if ((objData->flags & CRSnowBike_FLAG_1_Finished)) {
        return;
    }
        
    if (main_get_bits(objSetup->gamebitFinished)) {
        objData->flags |= CRSnowBike_FLAG_1_Finished;
        return;
    }
    
    player = get_player();

    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    objData->prevTranslate.x = self->srt.transl.x;
    objData->prevTranslate.y = self->srt.transl.y;
    objData->prevTranslate.z = self->srt.transl.z;
    
    if (objData->flags & CRSnowBike_FLAG_10_Was_In_Sequence) {
        objData->flags &= ~CRSnowBike_FLAG_10_Was_In_Sequence;

        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver) == FALSE) {
            bikeMotion->velocity.x = 0.0f;
            bikeMotion->velocity.y = 0.0f;
            bikeMotion->velocity.z = -2.0f;
            objData->yawOffset = 0;
            objData->rollOffset = 0;
            objData->framesInAir = 0;
            objData->pitchOffset = 0;
            objData->yaw = self->srt.yaw;
            objData->pitch = self->srt.pitch;
            objData->roll = self->srt.roll;
            CRSnowBike_update_collision_points(self, objData);
        }
    }

    switch (objData->state) {
    case STATE_0_Parked:
        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver) == FALSE) {
            self->objhitInfo->unk5B = 0;
            self->objhitInfo->unk5C = 0;
            objData->mountingFrom = PLAYER_NOT_NEARBY;
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

            if ((objSetup->gamebitUnlocked == NO_GAMEBIT) || main_get_bits(objSetup->gamebitUnlocked)) {
                self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
            } else {
                self->unkAF |= ARROW_FLAG_10_Greyed_Out;
            }
            
            //Check if the player is near the bike, and which side of the bike they're on
            if ((player != NULL) && (vec3_distance(&player->globalPosition, &self->globalPosition) < 100.0f)) {
                objData->mountingFrom = SIDE_LEFT;

                CRSnowBike_get_bike_matrix(self, objData, &bikeMtx, 0, 1, 1);
                
                //Show LockIcon if the player is close to the left side of the bike
                vec3_transform(&bikeMtx, dBikeMountSidePoints[0].x, dBikeMountSidePoints[0].y, dBikeMountSidePoints[0].z, &wsPoint.z, &wsPoint.y, &wsPoint.x);
                dx = player->srt.transl.x - wsPoint.z;
                dz = player->srt.transl.z - wsPoint.x;
                if ((SQ(dx) + SQ(dz)) < SQ(10)) {
                    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                    objData->mountingFrom = SIDE_RIGHT;
                } else {
                    //Show LockIcon if the player is close to the right side of the bike
                    vec3_transform(&bikeMtx, dBikeMountSidePoints[1].x, dBikeMountSidePoints[1].y, dBikeMountSidePoints[1].z, &wsPoint.z, &wsPoint.y, &wsPoint.x);
                    dx = player->srt.transl.x - wsPoint.z;
                    dz = player->srt.transl.z - wsPoint.x;
                    if ((SQ(dx) + SQ(dz)) < SQ(10)) {
                        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                        objData->mountingFrom = SIDE_LEFT;
                    }
                }
            }
        }

        //Stop sound loops
        {
            if (objData->soundHandleJets) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandleJets);
                objData->soundHandleJets = 0;
            }

            if (objData->soundHandleHiss) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandleHiss);
                objData->soundHandleHiss = 0;
            }

            if (objData->soundHandleRumble) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandleRumble);
                objData->soundHandleRumble = 0;
            }
        }

        //Remove modGfx
        {
            if (objData->modGfxDLLFlames != NULL) {
                dll_unload(objData->modGfxDLLFlames);
                objData->modGfxDLLFlames = NULL;
            }

            if (objData->modGfxDLLWaves != NULL) {
                dll_unload(objData->modGfxDLLWaves);
                objData->modGfxDLLWaves = NULL;
            }
        }

        objData->flags &= ~CRSnowBike_FLAG_8_Race_Started;
        break;
    case STATE_2_Driving:
        if ((objData->flags & CRSnowBike_FLAG_8_Race_Started) == FALSE) {
            objData->raceData.unk10 = -1;
            objData->raceData.unk14 = -1;
            objData->raceData.unk18 = -1;
            objData->raceData.unk1C = 0;

            //Check if the race has started
            if (main_get_bits(objData->gamebitIDs->started)) {
                flagValue = CRSnowBike_FLAG_8_Race_Started;
            } else {
                flagValue = CRSnowBike_FLAG_0_None;
            }
            objData->flags |= flagValue;

            if (objData->flags & CRSnowBike_FLAG_8_Race_Started) {
                if (objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver) {
                    CRSnowBike_sharpclaw_start_race(self);
                } else {
                    gDLL_4_Race->vtbl->func3(self, &objData->raceData, objSetup->racetrackIdx);
                }
                gDLL_4_Race->vtbl->func9(&objData->raceData);
            }
        } else if (main_get_bits(objData->gamebitIDs->ended)) {
            //Check if the race ended
            objData->flags &= ~CRSnowBike_FLAG_8_Race_Started;
        }

        //Handle slowdown after being damaged, and handle fuel replenishment
        CRSnowBike_handle_damage_or_fuel_collisions(self, objData);

        //Handle SharpClaw driving
        if (objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver) {
            if ((objData->flags & CRSnowBike_FLAG_8_Race_Started) == FALSE) {
                return;
            }
            
            if (map_world_coords_to_block_index(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z) >= 0) {
                if (objData->flags & CRSnowBike_FLAG_2_Driving_In_Void) {
                    sp5C = CRSnowBike_sharpclaw_update_race_pathing(self, objData, 2.8f * gUpdateRateF);
                    gDLL_4_Race->vtbl->func4(self, &objData->raceData);
                    gDLL_4_Race->vtbl->func10(&objData->raceData);
                    
                    if (sp5C == 0) {
                        motion = &objData->motion;
                        self->srt.yaw = arctan2_f(self->srt.transl.x - objData->srtCurves.transl.x, self->srt.transl.z - objData->srtCurves.transl.z);
                        self->srt.transl.x = objData->srtCurves.transl.x;
                        self->srt.transl.y = objData->srtCurves.transl.y;
                        self->srt.transl.z = objData->srtCurves.transl.z;
                        motion->velocity.x = 0.0f;
                        motion->velocity.y = 0.0f;
                        motion->velocity.z = -2.0f;
                        objData->yawOffset = 0;
                        objData->rollOffset = 0;
                        objData->framesInAir = 0;
                        objData->pitchOffset = 0;
                        objData->yaw = self->srt.yaw;
                        objData->pitch = self->srt.pitch;
                        objData->roll = self->srt.roll;
                        CRSnowBike_update_collision_points(self, objData);
                        func_80058680(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &yOffset, 0);
                        self->srt.transl.y -= yOffset;
                        objData->flags &= ~CRSnowBike_FLAG_2_Driving_In_Void;
                    }
                    return;
                } else if (CRSnowBike_sharpclaw_handle_steering(self, objData, objData, &objData->steering)) {
                    return;
                }
            } else {
                sp5C = CRSnowBike_sharpclaw_update_race_pathing(self, objData, 2.8f * gUpdateRateF);
                gDLL_4_Race->vtbl->func4(self, &objData->raceData);
                gDLL_4_Race->vtbl->func10(&objData->raceData);
                if (sp5C == 0) {
                    self->srt.yaw = arctan2_f(self->srt.transl.x - objData->srtCurves.transl.x, self->srt.transl.z - objData->srtCurves.transl.z);
                    self->srt.transl.x = objData->srtCurves.transl.x;
                    self->srt.transl.y = objData->srtCurves.transl.y;
                    self->srt.transl.z = objData->srtCurves.transl.z;
                    objData->flags |= CRSnowBike_FLAG_2_Driving_In_Void;
                }
                return;
            }
        } else {
            //Otherwise: configure player driving objHits
            self->objhitInfo->unk5B = 10;
            self->objhitInfo->unk5C = 10;
        }

        //Handle player driving
        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver) == FALSE) {
            //Update race-related data
            if (objData->flags & CRSnowBike_FLAG_8_Race_Started) {
                gDLL_4_Race->vtbl->func4(self, &objData->raceData);
                gDLL_4_Race->vtbl->func10(&objData->raceData);
                objData->raceRanking = gDLL_4_Race->vtbl->func12(&objData->raceData);
            }

            //Run control logic, iterating for each frame (including any skipped)
            for (updateIdx = 0; updateIdx < gUpdateRate; updateIdx++){
                CRSnowBike_read_joy_stick(self, &objData->steering, 0, updateIdx);
                CRSnowBike_update_motion(self, objData, &objData->motion, gUpdateRateF, (updateIdx + 1) == gUpdateRate);
                objData->yawOffset += (s16) ((-objData->steering.xJoy * 60.0f) - objData->yawOffset) >> 4;
                objData->rollOffset += (s16) ((-objData->steering.xJoy * 105.0f) - objData->rollOffset) >> 4;
                self->srt.yaw = objData->yaw + objData->yawOffset;
                self->srt.roll = objData->roll + objData->rollOffset;
            }
            
            motion = &objData->motion;

            //Handle fuel depletion
            if (objData->fuelAmount >= 0) {
                //Deplete fuel gradually and based on bike's speed
                objData->fuelAmount = objData->fuelAmount - (s32) (VECTOR_MAGNITUDE(motion->velocity) * gUpdateRateF * 1.5f) - gUpdateRate;
                diPrintf(" FUEL AMT %i \n", objData->fuelAmount / 10);
                gDLL_1_cmdmenu->vtbl->energy_bar_set(objData->fuelAmount);
            } else if (objData->maxVelocity.x > 0.1f) {
                diPrintf(" \tRAN OUT OF FUEL \t");
                
                //Play bump sound randomly while out of fuel
                if (rand_next(0, 10) == 0) {
                    gDLL_6_AMSFX->vtbl->play(self, SOUND_B38, MAX_VOLUME, NULL, NULL, 0, NULL);
                }

                //Max velocity dwindles
                objData->maxVelocity.x *= 0.95f;
                objData->maxVelocity.y *= 0.95f;
                objData->maxVelocity.z *= 0.95f;

                //Free the fuel gauge and play the defeat cutscene when max speed is approximately 0
                if (objData->maxVelocity.x < 0.1f) {
                    gDLL_1_cmdmenu->vtbl->energy_bar_free();
                    gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                    objData->maxVelocity.x = 0.01f;
                    objData->maxVelocity.y = 0.01f;
                    objData->maxVelocity.z = 0.01f;
                }
            }
        } else {
            //SharpClaw driver: run control logic, iterating for each frame (including any skipped)
            for (updateIdx = 0; updateIdx < gUpdateRate; updateIdx++) {
                CRSnowBike_sharpclaw_update_motion(self, objData, &objData->motion, gUpdateRateF, (updateIdx + 1) == gUpdateRate);
                objData->yawOffset += (s16) ((-objData->steering.xJoy * 60.0f) - objData->yawOffset) >> 4;
                objData->rollOffset += (s16) ((-objData->steering.xJoy * 105.0f) - objData->rollOffset) >> 4;
                self->srt.yaw = objData->yaw + objData->yawOffset;
                self->srt.roll = objData->roll + objData->rollOffset;
            }
        }
        
        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver) == FALSE) {
            CRSnowBike_handle_engine_sfx_and_modgfx(self, objData, 
                objData->motion.velocity.z, driverSteering->yJoy, 
                &driverSteering->unk10, 
                (CRSnowBike_SOUNDFLAG_Engine | CRSnowBike_SOUNDFLAG_Hiss | CRSnowBike_SOUNDFLAG_Jets)
            );
        } else {
            if (objData->soundHandleEngine) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandleEngine);
                objData->soundHandleEngine = 0;
            }
            if (objData->soundHandleJets) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandleJets);
                objData->soundHandleJets = 0;
            }
            if (objData->soundHandleHiss) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandleHiss);
                objData->soundHandleHiss = 0;
            }
            if (objData->soundHandleRumble) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandleRumble);
                objData->soundHandleRumble = 0;
            }
        }
        break;
    case STATE_1:
        break;
    }

    //Set up player objHits to cause damage
    if (((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver) == FALSE) && (objData->state == STATE_2_Driving)) {
        if (objData->forwardSpeed >= 0.0f) {
            forwardSpeed = objData->forwardSpeed;
        } else {
            forwardSpeed = -objData->forwardSpeed;
        }

        if (forwardSpeed > 2.0f) {
            self->objhitInfo->unk5F = Damage_Type_Speeding_Vehicle;
            self->objhitInfo->unk60 = 1;
            self->objhitInfo->unk58 |= 1;
            self->objhitInfo->unk40 = 16;
            self->objhitInfo->unk58 |= 4;
            self->srt.flags &= ~OBJFLAG_INVISIBLE;
        }
    }
}

// offset: 0x123C | func: 3 | export: 2
void CRSnowBike_update(Object* self) {
    CRSnowBike_Data* objData;
    ObjectHitInfo* objHitInfo;
    Vec3f wsVelocity;
    MtxF invRotMtx;
    MtxF bikeMtx;
    CRSnowBike_MotionData* bikeMotion;
    Object* obj;
    s32 objID;
    s32 i;

    objHitInfo = self->objhitInfo;
    objData = self->data;
    bikeMotion = &objData->motion;

    if (objHitInfo->unk48) {
        obj = objHitInfo->unk48;
        objID = obj->id;

        STUBBED_PRINTF(" HIT OBJECT %i \n"), objID;
        
        switch (objID){
        case OBJ_CRSnowClaw:
        case OBJ_CRSnowClaw2:
            camera_enable_y_offset();
            camera_set_shake_offset(1.0f);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_551, NULL, 4, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_552, NULL, 4, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_554, NULL, 4, -1, NULL);

            i = 10;
            while (i--) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_553, NULL, 2, -1, NULL);
            }
            
            objData->stallFrames = 5;
            objData->stallFactor = 0.2f;
            break;
        }
    }
    
    if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver) == FALSE) {
        //Update velocity based on position/time delta
        {
            //Set worldSpace velocity
            sRotationTransform.yaw = -objData->yaw;
            sRotationTransform.pitch = -objData->pitch;
            sRotationTransform.roll = -objData->roll;
            matrix_from_srt_reversed(&invRotMtx, &sRotationTransform);

            self->velocity.f[0] = (self->srt.transl.x - self->prevLocalPosition.x) * gUpdateRateInverseF;
            self->velocity.f[1] = (self->srt.transl.y - self->prevLocalPosition.y) * gUpdateRateInverseF;
            self->velocity.f[2] = (self->srt.transl.z - self->prevLocalPosition.z) * gUpdateRateInverseF;

            //Store objectSpace velocity too
            wsVelocity.x = self->velocity.f[0] * 0.93749994f;
            wsVelocity.y = self->velocity.f[1] * 0.93749994f;
            wsVelocity.z = self->velocity.f[2] * 0.93749994f;
            
            vec3_transform(&invRotMtx, 
                wsVelocity.f[0], wsVelocity.f[1], wsVelocity.f[2], 
                &bikeMotion->velocity.x, &bikeMotion->velocity.y, &bikeMotion->velocity.z
            );
        }
        
        //Store the worldSpace coordinates of a point on the bike
        CRSnowBike_get_bike_matrix(self, objData, &bikeMtx, FALSE, FALSE, FALSE);
        vec3_transform(&bikeMtx, 
            0.0f, 0.0f, -10.0f, 
            &objData->wsFrontOfBike.x, &objData->wsFrontOfBike.y, &objData->wsFrontOfBike.z
        );
    }
}

// offset: 0x1534 | func: 4 | export: 3
void CRSnowBike_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CRSnowBike_Data* objData;
    s32 sp38;

    objData = self->data;

    if (objData->flags & CRSnowBike_FLAG_1_Finished) {
        return;
    }
    
    if (visibility == -1) {
        sp38 = gDLL_13_Expgfx->vtbl->func10(self);
        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver) == FALSE) {
            CRSnowBike_handle_partfx_and_impact_sounds(self, objData, gdl, mtxs, vtxs, pols);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);

        func_80031F6C(self, 0, 
            &objData->attachPointCoords.x, 
            &objData->attachPointCoords.y, 
            &objData->attachPointCoords.z, 
            0
        );
        if (sp38 != 0) {
            gDLL_13_Expgfx->vtbl->func6(self, gdl, mtxs, NULL, 0, 0, 0);
        }
        return;
    } 
    
    if (objData->state == STATE_2_Driving) {
        objData->attachPointCoords.x = self->srt.transl.x;
        objData->attachPointCoords.y = self->srt.transl.y;
        objData->attachPointCoords.z = self->srt.transl.z;
        return;
    }
    
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 0, 
            &objData->attachPointCoords.x,
            &objData->attachPointCoords.y,
            &objData->attachPointCoords.z, 
            0
        );
    }
}

// offset: 0x1724 | func: 5 | export: 4
void CRSnowBike_free(Object* self, s32 onlySelf) {
    CRSnowBike_Data* objData;

    objData = self->data;

    if (objData->soundHandleEngine) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandleEngine);
        objData->soundHandleEngine = 0;
    }

    if (objData->soundHandleJets) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandleJets);
        objData->soundHandleJets = 0;
    }

    if (objData->soundHandleHiss) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandleHiss);
        objData->soundHandleHiss = 0;
    }

    if (objData->soundHandleRumble) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandleRumble);
        objData->soundHandleRumble = 0;
    }

    if (objData->modGfxDLLFlames != NULL) {
        dll_unload(objData->modGfxDLLFlames);
    }
    objData->modGfxDLLFlames = NULL;
    
    if (objData->modGfxDLLWaves != NULL) {
        dll_unload(objData->modGfxDLLWaves);
    }
    objData->modGfxDLLWaves = NULL;
    
    obj_free_object_type(self, OBJTYPE_Vehicle);
    
    if (!(objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver)) {
        gDLL_1_cmdmenu->vtbl->energy_bar_free();
    }
    
    tex_free(sTexVerticalWave);
    tex_free(sTexThrusterFlames);
    tex_free(sTexDriftWaves);
}

// offset: 0x18D8 | func: 6 | export: 5
u32 CRSnowBike_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW;
}

// offset: 0x18E8 | func: 7 | export: 6
u32 CRSnowBike_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(CRSnowBike_Data);
}

// offset: 0x18FC | func: 8 | export: 7
u8 CRSnowBike_can_bike_be_mounted(Object* self, s32 arg1) {
    CRSnowBike_Data* objData;
    CRSnowBike_Setup* objSetup;

    objData = self->data;
    objSetup = (CRSnowBike_Setup*)self->setup;
    
    if (objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver) {
        return 0;
    }

    if ((objSetup->gamebitUnlocked != NO_GAMEBIT) && (main_get_bits(objSetup->gamebitUnlocked) == FALSE)) {
        return 0;
    }

    STUBBED_PRINTF(" Bike Can Mount %i ", objData->mountingFrom);
    
    return objData->mountingFrom;
}

// offset: 0x1980 | func: 9 | export: 8
u8 CRSnowBike_get_mount_side(Object* self) {
    CRSnowBike_Data* objData = self->data;
    return objData->mountingFrom;
}

static const char str_extra2[] = "tracks %d\n";
static const char str_extra3[] = "ident %d\n";

// offset: 0x1990 | func: 10 | export: 9
void CRSnowBike_get_attach_position(Object* self, f32* ox, f32* oy, f32* oz) {
    CRSnowBike_Data* objData = self->data;
    
    *ox = objData->attachPointCoords.x;
    *oy = objData->attachPointCoords.y;
    *oz = objData->attachPointCoords.z;
}

// offset: 0x19B4 | func: 11 | export: 10
s32 CRSnowBike_func_19B4(Object* self, s32 arg1) {
    CRSnowBike_Data* objData = self->data;
    
    if (objData->flags & CRSnowBike_FLAG_8_Race_Started) {
        return 0;
    }
    
    if ((joy_get_pressed(0) & B_BUTTON) == FALSE) {
        return 0;
    }
    
    return 2;
}

// offset: 0x1A24 | func: 12 | export: 11
s32 CRSnowBike_func_1A24(s32 arg0) {
    return 2;
}

// offset: 0x1A34 | func: 13 | export: 12
/**
  * Retrieves worldSpace coords of a point on the bike.
  */
void CRSnowBike_get_front_of_bike_coords(Object* self, f32* ox, f32* oy, f32* oz) {
    CRSnowBike_Data* objData = self->data;
    
    *ox = objData->wsFrontOfBike.x;
    *oy = objData->wsFrontOfBike.y;
    *oz = objData->wsFrontOfBike.z;
}

// offset: 0x1A58 | func: 14 | export: 13
s8 CRSnowBike_get_state(Object* self) {
    CRSnowBike_Data* objData = self->data;
    return objData->state;
}

// offset: 0x1A68 | func: 15 | export: 14
void CRSnowBike_set_state(Object* self, s32 state) {
    CRSnowBike_Data* objData = self->data;
    
    objData->state = state;

    if ((state == STATE_2_Driving) && !(objData->flags & CRSnowBike_FLAG_20_SharpClaw_Driver)) {
        objData->fuelAmount = 10000;
        objData->maxVelocity.x = 2.0f;
        objData->maxVelocity.y = 4.0f;
        objData->maxVelocity.z = 4.6f;
        gDLL_1_cmdmenu->vtbl->energy_bar_create(0, 13000, TEXTABLE_569, TEXTABLE_56A, 1);
    }
}

// offset: 0x1B10 | func: 16 | export: 15
void CRSnowBike_get_turn_tvalue_and_direction(Object* self, f32* tValueTurn, s32* turnDirection) {
    CRSnowBike_Data* objData = self->data;
    
    *tValueTurn = objData->yawOffset / 2500.0f;
    
    if (*tValueTurn > 1.0f) {
        *tValueTurn = 1.0f;
    } else if (*tValueTurn < -1.0f) {
        *tValueTurn = -1.0f;
    }
    
    *turnDirection = objData->yawOffset < 0;
}

// offset: 0x1B9C | func: 17 | export: 16
/**
  * Returns the bike's current speed divided by its max speed.
  * Also retrieves the max speed value.
  */
f32 CRSnowBike_get_speed_tvalue_and_max(Object* self, f32* oMaxSpeed) {
    CRSnowBike_Data* objData;
    f32 tValueSpeed;

    objData = self->data;
    
    *oMaxSpeed = 5;

    tValueSpeed = VECTOR_MAGNITUDE(objData->motion.velocity) * 0.2f;
    if (tValueSpeed > 1.0f) {
        tValueSpeed = 1.0f;
    }
    return tValueSpeed;
}

// offset: 0x1C28 | func: 18 | export: 17
/**
  * Returns the racer's ranking (1st, 2nd, 3rd, etc.)
  */
s8 CRSnowBike_get_ranking(Object* self) {
    CRSnowBike_Data* objData = self->data;
    return objData->raceRanking;
}

// offset: 0x1C38 | func: 19 | export: 18
void CRSnowBike_sharpclaw_start_race(Object* self) {
    s32 checkpointUID;
    checkpoint4_Setup* checkpointSetup;
    CRSnowBike_Data* objData;
    CRSnowBike_Setup* objSetup;

    objSetup = (CRSnowBike_Setup*)self->setup;
    objData = self->data;
    
    switch (self->id) {
    default:
    case OBJ_CRSnowClawBike:
        checkpointUID = dCheckpointUIDs[objSetup->racetrackIdx][0];
        break;
    case OBJ_CRSnowClawBike2:
        checkpointUID = dCheckpointUIDs[objSetup->racetrackIdx][1];
        break;
    case OBJ_CRSnowClawBike3:
        checkpointUID = dCheckpointUIDs[objSetup->racetrackIdx][2];
        break;
    }
    
    checkpointSetup = (checkpoint4_Setup*)map_find_obj_setup(checkpointUID, NULL, NULL, NULL, NULL);
    if (checkpointSetup == NULL) {
        return;
    }
    
    self->srt.transl.x = checkpointSetup->base.x;
    self->srt.transl.y = checkpointSetup->base.y;
    self->srt.transl.z = checkpointSetup->base.z;
    self->srt.yaw = checkpointSetup->yaw << 8;

    CRSnowBike_update_collision_points(self, objData);
    gDLL_4_Race->vtbl->func3(self, &objData->raceData, objSetup->racetrackIdx);
    
    objData->srtCurves.transl.x = self->srt.transl.x;
    objData->srtCurves.transl.y = self->srt.transl.y;
    objData->srtCurves.transl.z = self->srt.transl.z;
    objData->srtCurves.yaw = self->srt.yaw;
}

// offset: 0x1DB8 | func: 20 | export: 19
void CRSnowBike_func_1DB8(Object* self, UNK_TYPE_32 arg1) {

}


// offset: 0x1DC8 | func: 21
void CRSnowBike_handle_partfx_and_impact_sounds(Object* self, CRSnowBike_Data* objData, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols) {
/*0x28*/ static u32 sImpactSoundHandle;
    Vertex* vertex;
    Gfx* gfx;
    Triangle* tri;
    s32 pad1[6];
    SRT fxTransform;
    s32 i;
    f32 translateX;
    s32 pad2;
    CRSnowBike_MotionData* bikeMotion;
    u32 dColourRGBA[] = { 6, 105, 105, 255}; //Unused RGBA colour, maybe for randomised spark colours?
    s32 volume;
    s32 pad3;

    bikeMotion = &objData->motion;

    gfx = *gdl;
    vertex = *vtxs;
    tri = *pols;
    
    //Get transform for particles, based on bike's velocity (bike velocity uses -Z forward, so axes are swapped for fxTransform)
    {
        translateX = 0.0f;
        if (bikeMotion->velocity.z < 0.0f) {
            translateX = bikeMotion->velocity.z;
        }
        fxTransform.transl.z = translateX;

        if (bikeMotion->velocity.z < 0.0f) {
            translateX = bikeMotion->velocity.x;
        }
        fxTransform.transl.x = translateX;
    }

    //Set particle colour (just set to white, but maybe once intended to use dColourRGBA?)
    dl_set_prim_color(&gfx, 0xFF, 0xFF, 0xFF, 0xFF);
    
    //Create different smoke particles based on bike's forward speed
    {
        if (bikeMotion->velocity.z < -0.5f) {
            if (1) { }
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_12E, &fxTransform, 4, -1, NULL);
        }

        if (bikeMotion->velocity.z < -1.5f) {
            if (1) { }
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_12F, &fxTransform, 4, -1, NULL);
        }

        if (bikeMotion->velocity.z < -2.1f) {
            if (1) { }
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_130, &fxTransform, 4, -1, NULL);
        }
    }
    
    //Create starry particles when jets are firing
    {
        i = 0;
        if (objData->steering.yJoy > 0) {
            i = 2;
        }

        while (i) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_131, NULL, 4, -1, NULL);
            i--;
        }
    }
    
    fxTransform.yaw = 0;
    fxTransform.pitch = 0;
    fxTransform.roll = 0;
    fxTransform.scale = 1.0f;

    //Play impact sound effects while colliding (not debounced)
    if (objData->forwardSpeed < -1.2f) {
        dColourRGBA[1] += rand_next(0, 155);
        dColourRGBA[2] += rand_next(0, 155);
        volume = ((0.0f - objData->forwardSpeed) * 21.0f);
        if (objData->collisionFlags & 1) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_292_Impact, volume, &sImpactSoundHandle, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->set_pitch(sImpactSoundHandle, (volume / 127.0f) + 0.5f);
        } else if (objData->collisionFlags & 2) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_292_Impact, volume, &sImpactSoundHandle, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->set_pitch(sImpactSoundHandle, (volume / 127.0f) + 0.5f);
        } else if (objData->collisionFlags & 4) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_292_Impact, volume, &sImpactSoundHandle, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->set_pitch(sImpactSoundHandle, (volume / 127.0f) + 0.5f);
        }
    }
    
    *gdl = gfx;
    *vtxs = vertex;
    *pols = tri;
}

// offset: 0x22BC | func: 22
void CRSnowBike_init_motion_data(Object* self, CRSnowBike_MotionData* motion) {
    motion->gravityFactor = 90;
    motion->accelerationFactor = 1/motion->gravityFactor;  
    motion->gravity = -11.0f;
    motion->friction = 0.1f;
    motion->airResistance = 13.0f;
    motion->accelerationFactorPerFrame = (1/motion->gravityFactor) * ONE_OVER_SIXTY_F;
    
    sRotationTransform.transl.x = 0;
    sRotationTransform.transl.y = 0;
    sRotationTransform.transl.z = 0;
    sRotationTransform.scale = 1;
}

// offset: 0x2340 | func: 23
/**
  * Handles bike motion when the player is driving the bike.
  */
void CRSnowBike_update_motion(Object* self, CRSnowBike_Data* objData, CRSnowBike_MotionData* motion, f32 updateRate, s32 applyTerrainCollision) {
    MtxF mtx;
    MtxF invMtx;
    MtxF invMtxYaw;
    f32 pad1;
    f32 minusLimit;
    f32 sqForwardSpeed;
    f32 acceleration;
    f32 newVelocityX;
    f32 newVelocityZA;
    f32 oneOverUpdateRate;
    f32 accelerationAmount;
    Vec3f osGravity;
    f32 yValue;
    f32 newVelocityZ;
    s32 countCollision;
    Vec3f vNormal;
    Vec3f velocityReduced;
    s32 pad2;
    DLL27_Data* collision;
    s32 anglePitch;
    s32 angleRoll;
    s32 i;

    collision = &objData->collision;
    
    //Stall the bike's velocity for a few frames after being hit
    if (objData->stallFrames != 0) {
        VECTOR_SCALE(motion->velocity, objData->stallFactor);
        objData->stallFrames--;
        if (objData->stallFrames < 0) {
            objData->stallFrames = 0;
        }
    }

    //Limit velocity
    minusLimit = -objData->maxVelocity.x;
    motion->velocity.x = (motion->velocity.x < minusLimit) ? minusLimit : (
                      (objData->maxVelocity.f[0] < motion->velocity.x) ? objData->maxVelocity.f[0] : motion->velocity.x);
    
    minusLimit = -objData->maxVelocity.y;
    motion->velocity.y = (motion->velocity.f[1] < minusLimit) ? minusLimit : (
                      (objData->maxVelocity.f[1] < motion->velocity.f[1]) ? objData->maxVelocity.f[1] : motion->velocity.f[1]);

    minusLimit = -objData->maxVelocity.z;
    motion->velocity.z = (motion->velocity.f[2] < minusLimit) ? minusLimit : (
                      (objData->maxVelocity.f[2] < motion->velocity.f[2]) ? objData->maxVelocity.f[2] : motion->velocity.f[2]);
    
    //Get rotation matrices
    sRotationTransform.yaw = objData->yaw;
    sRotationTransform.pitch = objData->pitch;
    sRotationTransform.roll = objData->roll;
    matrix_from_srt(&mtx, &sRotationTransform);

    sRotationTransform.yaw = -objData->yaw;
    sRotationTransform.pitch = -objData->pitch;
    sRotationTransform.roll = -objData->roll;
    matrix_from_srt_reversed(&invMtx, &sRotationTransform);
    
    //Calculate pitch while airborne
    if ((objData->flags & CRSnowBike_FLAG_4_Grounded) == FALSE) {
        yValue = (f32) -objData->steering.yJoy / 60.0f;
        if (yValue > 1.0f) {
            yValue = 1.0f;
        } else if (yValue < -1.0f) {
            yValue = -1.0f;
        }
        yValue *= 0x1800;
        objData->pitchOffset += ((s32)yValue - objData->pitchOffset) >> 5;
    } else if (objData->pitchOffset != 0) {
        objData->pitchOffset -= (-objData->pitchOffset >> 5);
    }
    
    //Transform gravity vector into bike's objectSpace
    vec3_transform(&invMtx, 
        0.0f, motion->gravity * motion->gravityFactor, 0.0f, 
        &osGravity.f[0], &osGravity.f[1], &osGravity.f[2]);

    //Handle forward acceleration/deceleration
    {
        if (objData->steering.yJoy < 0) {
            accelerationAmount = -(f32) objData->steering.yJoy * 4.0f;
        } else {
            accelerationAmount = -(f32) objData->steering.yJoy * 10.0f;
        }
        
        acceleration = motion->accelerationFactor * accelerationAmount;

        //NOTE: motion->velocity Z points out the back of the bike, so negative values mean forward
        if (acceleration < 0.0f) {
            //Acceleration
            motion->velocity.z += acceleration * ONE_OVER_SIXTY_F;
        } else {
            //Deceleration
            if (motion->velocity.z <= 0.0f) {
                motion->velocity.z += acceleration * ONE_OVER_SIXTY_F;
                
                //Clamp velocity so bike can't move backwards
                if (motion->velocity.z > 0.0f) {
                    motion->velocity.z = 0.0f;
                }
            }
        }
    }
    
    //Apply gravity
    {
        motion->vGravity.x = osGravity.f[0] * motion->accelerationFactorPerFrame;
        motion->vGravity.y = osGravity.f[1] * motion->accelerationFactorPerFrame;
        motion->vGravity.z = osGravity.f[2] * motion->accelerationFactorPerFrame;

        motion->velocity.x = motion->vGravity.x + motion->velocity.x;
        motion->velocity.y = motion->vGravity.y + motion->velocity.y;
        motion->velocity.z = motion->vGravity.z + motion->velocity.z;
    }

    //Handle being on ground/in air
    if (collision->unk25D != 0) {        
        //Apply friction

        //Forward component (relative to bike's own coordinate space)
        {
            acceleration = motion->friction * osGravity.y;
            if (motion->velocity.z < 0.0f) {
                if (acceleration < 0.0f) {
                    acceleration = -acceleration;
                }
            } else {
                if (acceleration > 0.0f) {
                    acceleration = -acceleration;
                }
            }
            acceleration *= motion->accelerationFactorPerFrame;

            newVelocityZ = motion->velocity.z + acceleration;
            if (motion->velocity.z < 0.0f) {
                if (newVelocityZ > 0.0f) {
                    motion->velocity.z = 0.0f;
                } else {
                    motion->velocity.z = newVelocityZ;
                }
            } else if (newVelocityZ < 0.0f) {
                motion->velocity.z = 0.0f;
            } else {
                motion->velocity.z = newVelocityZ;
            }
        }
        
        //Lateral component (relative to bike's own coordinate space)
        {
            //Note: this value's squared afterwards anyway, so there's no need to check sign
            {
                if (motion->velocity.z < 0.0f) {
                    newVelocityZ = -motion->velocity.z;
                } else {
                    newVelocityZ = motion->velocity.z;
                }
            }
            
            acceleration = motion->friction * osGravity.y * (4.0f + SQ(newVelocityZ));
            if (motion->velocity.x < 0.0f) {
                if (acceleration < 0.0f) {
                    acceleration = -acceleration;
                }
            } else if (acceleration > 0.0f) {
                acceleration = -acceleration;
            }
            acceleration *= motion->accelerationFactorPerFrame;
            
            newVelocityX = motion->velocity.x + acceleration;
            if (motion->velocity.x < 0.0f) {
                if (newVelocityX > 0.0f) {
                    motion->velocity.x = 0.0f;
                } else {
                    motion->velocity.x = newVelocityX;
                }
            } else if (newVelocityX < 0.0f) {
                motion->velocity.x = 0.0f;
            } else {
                motion->velocity.x = newVelocityX;
            }
        }
        
        objData->framesInAir = 0;
        objData->pitchOffset = 0;
    } else {
        objData->framesInAir++;
        if (objData->framesInAir > 100) {
            objData->framesInAir = 100;
        }
    }

    //Apply air resistance (proportionate to square of speed)
    {
        sqForwardSpeed = SQ(motion->velocity.z);
        acceleration = motion->airResistance * sqForwardSpeed;
        if (motion->velocity.z > 0.0f) {
            acceleration = -acceleration;
        }
        acceleration *= motion->accelerationFactorPerFrame;

        newVelocityZA = motion->velocity.z + acceleration;
        if (motion->velocity.z < 0.0f) {
            if (newVelocityZA > 0.0f) {
                motion->velocity.z = 0.0f;
            } else {
                motion->velocity.z = newVelocityZA;
            }
        } else if (newVelocityZA < 0.0f) {
            motion->velocity.z = 0.0f;
        } else {
            motion->velocity.z = newVelocityZA;
        }
    }

    vec3_transform(&mtx, motion->velocity.x, motion->velocity.y, motion->velocity.z, 
                   &self->velocity.f[0], &self->velocity.f[1], &self->velocity.f[2]);
    VECTOR_SCALE(self->velocity, 1.0666667f);
    obj_move(self, self->velocity.x, self->velocity.f[1], self->velocity.f[2]);

    /* Run terrain calculations 
       (the bike iterates its motion logic for each skipped frame plus the current one, 
       but only runs collision for the last iteration - i.e. once per update) */
    if (applyTerrainCollision) {
        oneOverUpdateRate = 1.0f / updateRate;
        CRSnowBike_handle_collisions(self, objData, collision);
        gDLL_27->vtbl->func_1E8(self, collision, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, collision);
        gDLL_27->vtbl->func_624(self, collision, updateRate);
        
        //Get velocity based on position delta and time delta
        self->velocity.x = (self->srt.transl.x - self->prevLocalPosition.x) * oneOverUpdateRate;
        self->velocity.y = (self->srt.transl.y - self->prevLocalPosition.y) * oneOverUpdateRate;
        self->velocity.z = (self->srt.transl.z - self->prevLocalPosition.z) * oneOverUpdateRate;
        
        //Lose some momentum
        velocityReduced.f[0] = self->velocity.x * 0.93749994f;
        velocityReduced.f[1] = self->velocity.y * 0.93749994f;
        velocityReduced.f[2] = self->velocity.z * 0.93749994f;

        //Transform velocity into bike's local objectSpace
        vec3_transform(&invMtx, 
            velocityReduced.f[0], velocityReduced.f[1], velocityReduced.f[2], 
            &motion->velocity.x, &motion->velocity.y, &motion->velocity.z
        );

        //Get the surface normal vector, averaged out over the bike's test points
        {
            vNormal.f[0] = 0.0f;
            vNormal.f[1] = 1.0f;
            vNormal.f[2] = 0.0f;
            
            if (collision->unk25C & 0xF) {
                objData->flags |= CRSnowBike_FLAG_4_Grounded;
            } else {
                objData->flags &= ~CRSnowBike_FLAG_4_Grounded;
            }
            
            for (countCollision = 0, i = 0; i < 4; i++) {
                if (collision->unk25C & (1 << i)) {
                    vNormal.x = collision->unk68.unk0[i].x + vNormal.x;
                    vNormal.y = collision->unk68.unk0[i].y + vNormal.y;
                    vNormal.z = collision->unk68.unk0[i].z + vNormal.z;
                    countCollision++;
                }
            }
            
            VECTOR_SCALE(vNormal, 0.25f);
            if (countCollision != 0) {
                acceleration = 1.0f / countCollision;
                VECTOR_SCALE(vNormal, acceleration);
            } else {
                vNormal.f[0] = 0.0f;
                vNormal.f[1] = 1.0f;
                vNormal.f[2] = 0.0f;
            }
        }
        
        //Transform the normal vector around the bike's yaw
        sRotationTransform.yaw = -objData->yaw;
        sRotationTransform.pitch = 0;
        sRotationTransform.roll = 0;
        matrix_from_srt_reversed(&invMtxYaw, &sRotationTransform);
        vec3_transform(&invMtxYaw, 
            vNormal.f[0], vNormal.f[1], vNormal.f[2], 
            &vNormal.f[0], &vNormal.f[1], &vNormal.f[2]
        );
        
        //Get the bike's goal pitch and roll, based on the surface normal
        anglePitch = M_90_DEGREES - arctan2_f(vNormal.f[1], vNormal.f[2]);
        angleRoll = -(M_90_DEGREES - arctan2_f(vNormal.f[1], vNormal.f[0]));

        //Blend towards goal pitch
        anglePitch -= (objData->pitch & 0xFFFF);
        CIRCLE_WRAP(anglePitch);
        objData->pitch += (((anglePitch >> 2) / 3) * (s32) updateRate);
        self->srt.pitch = objData->pitch + objData->pitchOffset;
        
        //Blend towards goal roll
        angleRoll -= (objData->roll & 0xFFFF);
        CIRCLE_WRAP(angleRoll);
        objData->roll += (((angleRoll >> 2) / 3) * (s32) updateRate);
    }
    
    //Turn
    objData->yaw -= (s16) (objData->steering.xJoy * (70.0f - (objData->steering.yJoy * 0.05f)) * 0.0666f);
}

// offset: 0x2E64 | func: 24
/**
  * Handles bike motion when a SharpClaw is driving the bike.
  */
void CRSnowBike_sharpclaw_update_motion(Object* self, CRSnowBike_Data* objData, CRSnowBike_MotionData* motion, f32 updateRate, s32 applyTerrainCollision) {
    MtxF rotMtx;
    MtxF invRotMtx;
    MtxF invMtxYaw;
    DLL27_Data* collision;
    f32 updateVelocityZ;
    f32 acceleration;
    f32 oneOverUpdateRate;
    s32 angleRoll;
    Vec3f osGravity;
    Vec3f vNormal;
    Vec3f velocityReduced;
    s32 pad;
    s32 countCollision;
    s32 anglePitch;
    f32 temp;
    s32 i;
    f32 sq;

    collision = &objData->collision;
    
    //Limit velocity
    if (motion->velocity.f[0] > 4.0f) {
        motion->velocity.f[0] = 4.0f;
    }
    if (motion->velocity.f[0] < -4.0f) {
        motion->velocity.f[0] = -4.0f;
    }
    if (motion->velocity.f[1] > 4.0f) {
        motion->velocity.f[1] = 4.0f;
    }
    if (motion->velocity.f[1] < -4.0f) {
        motion->velocity.f[1] = -4.0f;
    }
    if (motion->velocity.f[2] > 5.0f) {
        motion->velocity.f[2] = 5.0f;
    }
    if (motion->velocity.f[2] < -5.8f) {
        motion->velocity.f[2] = -5.8f;
    }
    
    //Get rotation matrices
    sRotationTransform.yaw = objData->yaw;
    sRotationTransform.pitch = objData->pitch;
    sRotationTransform.roll = objData->roll;
    matrix_from_srt(&rotMtx, &sRotationTransform);
    
    sRotationTransform.yaw = -objData->yaw;
    sRotationTransform.pitch = -objData->pitch;
    sRotationTransform.roll = -objData->roll;
    matrix_from_srt_reversed(&invRotMtx, &sRotationTransform);
    
    //Transform gravity vector into bike's objectSpace
    vec3_transform(&invRotMtx, 
        0.0f, motion->gravity * motion->gravityFactor, 0.0f, 
        &osGravity.f[0], &osGravity.f[1], &osGravity.f[2]
    );

    //Handle forward acceleration
    updateVelocityZ = -(f32) objData->steering.yJoy * 50.0f;
    updateVelocityZ *= motion->accelerationFactorPerFrame;
    motion->velocity.z += updateVelocityZ;

    //Apply gravity
    {
        motion->vGravity.x = osGravity.f[0] * motion->accelerationFactorPerFrame;
        motion->vGravity.y = osGravity.f[1] * motion->accelerationFactorPerFrame;
        motion->vGravity.z = osGravity.f[2] * motion->accelerationFactorPerFrame;

        motion->velocity.x = motion->velocity.x + motion->vGravity.f[0];
        motion->velocity.y = motion->velocity.y + motion->vGravity.f[1];
        motion->velocity.z = motion->velocity.z + motion->vGravity.f[2];
    }

    //Apply friction while grounded
    if (collision->unk25D != 0) {
        motion->velocity.f[0] = 0.0f;
        acceleration = motion->friction * osGravity.y;
        
        if (motion->velocity.f[2] < 0.0f) {
            if (acceleration < 0.0f) {
                acceleration = -acceleration;
            }
        } else if (acceleration > 0.0f) {
            acceleration = -acceleration;
        }

        acceleration *= motion->accelerationFactorPerFrame;

        updateVelocityZ = motion->velocity.f[2] + acceleration;
        if (updateVelocityZ > 0.0f) {
            motion->velocity.f[2] = 0.0f;
        } else {
            motion->velocity.f[2] = updateVelocityZ;
        }
    }

    //Apply air resistance (proportionate to square of speed)
    {
        sq = SQ(motion->velocity.f[2]);
        acceleration = motion->airResistance * sq;
        acceleration *= motion->accelerationFactorPerFrame;
        
        updateVelocityZ = motion->velocity.f[2] + acceleration;
        if (updateVelocityZ > 0.0f) {
            motion->velocity.f[2] = 0.0f;
        } else {
            motion->velocity.f[2] = updateVelocityZ;
        }
    }
    
    vec3_transform(&rotMtx, 
        motion->velocity.f[0], motion->velocity.f[1], motion->velocity.f[2], 
        &self->velocity.f[0], &self->velocity.f[1], &self->velocity.f[2]
    );
    VECTOR_SCALE(self->velocity, 1.0666667f);
    obj_move(self, self->velocity.x, self->velocity.y, self->velocity.z);

    /* Run terrain calculations 
       (the bike iterates its motion logic for each skipped frame plus the current one, 
       but only runs collision for the last iteration - i.e. once per update) */

    if (applyTerrainCollision) {
        oneOverUpdateRate = 1.0f / updateRate;
        gDLL_27->vtbl->func_1E8(self, collision, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, collision);
        gDLL_27->vtbl->func_624(self, collision, updateRate);

        //Get velocity based on position delta and time delta
        self->velocity.x = (self->srt.transl.x - self->prevLocalPosition.x) * oneOverUpdateRate;
        self->velocity.y = (self->srt.transl.y - self->prevLocalPosition.y) * oneOverUpdateRate;
        self->velocity.z = (self->srt.transl.z - self->prevLocalPosition.z) * oneOverUpdateRate;

        //Lose some momentum
        velocityReduced.f[0] = self->velocity.x * 0.93749994f;
        velocityReduced.f[1] = self->velocity.y * 0.93749994f;
        velocityReduced.f[2] = self->velocity.z * 0.93749994f;
        vec3_transform(&invRotMtx, 
            velocityReduced.f[0], velocityReduced.f[1], velocityReduced.f[2], 
            &motion->velocity.f[0], &motion->velocity.f[1], &motion->velocity.f[2]
        );
        
        //Get the surface normal vector, averaged out over the bike's test points
        {
            vNormal.f[0] = 0.0f;
            vNormal.f[1] = 1.0f;
            vNormal.f[2] = 0.0f;
            
            if (collision->unk25C & 0xF) {
                objData->flags |= CRSnowBike_FLAG_4_Grounded;
            } else {
                objData->flags &= ~CRSnowBike_FLAG_4_Grounded;
            }

            for (countCollision = 0, i = 0; i < 4; i++) {
                if (collision->unk25C & (1 << i)) {
                    vNormal.f[0] = vNormal.f[0] + collision->unk68.unk0[i].f[0];
                    vNormal.f[1] = vNormal.f[1] + collision->unk68.unk0[i].f[1];
                    vNormal.f[2] = vNormal.f[2] + collision->unk68.unk0[i].f[2];
                    countCollision++;
                }
            }
            
            if (countCollision != 0) {
                VECTOR_SCALE(vNormal, 1.0f / countCollision);
            } else {
                vNormal.f[0] = 0.0f;
                vNormal.f[1] = 1.0f;
                vNormal.f[2] = 0.0f;
            }
        }
        
        //Transform the normal vector around the bike's yaw
        sRotationTransform.yaw = -objData->yaw;
        sRotationTransform.pitch = 0;
        sRotationTransform.roll = 0;
        matrix_from_srt_reversed(&invMtxYaw, &sRotationTransform);
        vec3_transform(&invMtxYaw, 
            vNormal.f[0], vNormal.f[1], vNormal.f[2], 
            &vNormal.f[0], &vNormal.f[1], &vNormal.f[2]
        );
        
        //Get the bike's goal pitch and roll, based on the surface normal
        anglePitch = M_90_DEGREES - arctan2_f(vNormal.f[1], vNormal.f[2]);
        angleRoll = -(M_90_DEGREES - arctan2_f(vNormal.f[1], vNormal.f[0]));

        //Blend towards goal pitch
        anglePitch -= (objData->pitch & 0xFFFF);
        CIRCLE_WRAP(anglePitch);
        objData->pitch += ((anglePitch >> 2) / 3) * (s32)updateRate;
        self->srt.pitch = objData->pitch + objData->pitchOffset;

        //Blend towards goal roll
        angleRoll -= (objData->roll & 0xFFFF);
        CIRCLE_WRAP(angleRoll);
        objData->roll += ((angleRoll >> 2) / 3) * (s32)updateRate;
    }
    
    //Turn
    objData->yaw -= (s16) (objData->steering.xJoy * (70.0f - (objData->steering.yJoy * 0.05f)) * 0.0666f);
}

// offset: 0x3618 | func: 25
/**
  * Used to read the controller joystick when the player is driving the bike.
  *
  * Interesting to note arg2 being `controllerPort`! It may suggest Rare's were at least considering the idea of multiplayer bike races.
  */
void CRSnowBike_read_joy_stick(Object* self, CRSnowBike_SteerData* steering, u8 controllerPort, s32 buffer) {
    steering->xJoy = joy_get_stick_x_buffered(controllerPort, buffer);
    steering->yJoy = joy_get_stick_y_buffered(controllerPort, buffer);
}

// offset: 0x3694 | func: 26
/**
  * Retrieves the bike's current transformation matrix. Pitch/roll can optionally be excluded, as can the bike's yaw offset.
  */
void CRSnowBike_get_bike_matrix(Object* self, CRSnowBike_Data* objData, MtxF* oMtx, s32 useYawOffset, s32 useRoll, s32 usePitch) {
    SRT transform;

    transform.yaw = objData->yaw;
    transform.pitch = usePitch ? self->srt.pitch : 0;
    transform.roll = useRoll ? self->srt.roll : 0;
    transform.scale = 1.0f;
    transform.transl.x = self->srt.transl.x;
    transform.transl.y = self->srt.transl.y;
    transform.transl.z = self->srt.transl.z;
    
    if (useYawOffset) {
        transform.yaw += objData->yawOffset;
    }
    
    matrix_from_srt(oMtx, &transform);
}

// offset: 0x3748 | func: 27
void CRSnowBike_handle_damage_or_fuel_collisions(Object* self, CRSnowBike_Data* objData) {
    s32 damageType;
    s32 hitSphereID;
    s32 hitDamage;
    Object* hitBy;

    damageType = func_80025F40(self, &hitBy, &hitSphereID, &hitDamage);
    if (!(self->objhitInfo->unk58 & 1) || (damageType == 0)) {
        return;
    }
    
    switch (damageType) {
    case Damage_Type_D:
        objData->stallFrames = 20;
        objData->stallFactor = 0.8f;
        return;
    case Damage_Type_Speeding_Vehicle:
        objData->stallFrames = 5;
        objData->stallFactor = 0.75f;
        return;
    case Damage_Type_Bike_Fuel:
        objData->fuelAmount += (hitDamage * 100);
        objData->maxVelocity.x = 2.0f;
        objData->maxVelocity.y = 4.0f;
        objData->maxVelocity.z = 4.6f;
        if (objData->fuelAmount > 13000) {
            objData->fuelAmount = 13000;
        }
        break;
    }
}

// offset: 0x3860 | func: 28
int CRSnowBike_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    CRSnowBike_Data* objData;
    CRSnowBike_SteerData* steering;
    CRSnowBike_MotionData* bikeMotion;
    Vec3f wsVelocity;
    SRT transform;
    s32 i;
    MtxF mtx;

    objData = self->data;
    steering = &objData->steering;

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case CRSnowBike_ANIMCMD_2_Lose_Race:
            main_set_bits(BIT_Play_Seq_02AA_CF_Lose_Race, TRUE);
            break;
        case CRSnowBike_ANIMCMD_3_Free_Fuel_Gauge:
            gDLL_1_cmdmenu->vtbl->energy_bar_free();
            break;
        }
    }
    
    if (objData->state == STATE_2_Driving) {
        //Get the bike's objectSpace velocity vector
        {
            //Get bike's worldSpace velocity vector (from position delta)
            wsVelocity.x = (self->srt.transl.x - objData->prevTranslate.x) * gUpdateRateInverseF;
            wsVelocity.y = (self->srt.transl.y - objData->prevTranslate.y) * gUpdateRateInverseF;
            wsVelocity.z = (self->srt.transl.z - objData->prevTranslate.z) * gUpdateRateInverseF;

            //Apply drag
            wsVelocity.f[0] *= 0.93749994f;
            wsVelocity.f[1] *= 0.93749994f;
            wsVelocity.f[2] *= 0.93749994f;
            
            //Get bike's rotation matrix
            transform.transl.x = 0.0f;
            transform.transl.y = 0.0f;
            transform.transl.z = 0.0f;
            transform.scale = 1.0f;
            transform.yaw = -self->srt.yaw;
            transform.pitch = -self->srt.pitch;
            transform.roll = -self->srt.roll;
            bikeMotion = &objData->motion;
            matrix_from_srt_reversed(&mtx, &transform);
            
            //Transform velocity from worldSpace to the bike's own objectSpace
            vec3_transform(&mtx, 
                wsVelocity.x, wsVelocity.y, wsVelocity.z, 
                &bikeMotion->velocity.x, &bikeMotion->velocity.y, &bikeMotion->velocity.z
            );
        }

        steering->yJoy += gUpdateRate * 8;
        if (steering->yJoy > 70) {
            steering->yJoy = 70;
        }

        CRSnowBike_handle_engine_sfx_and_modgfx(self, objData, 
            bikeMotion->velocity.z, steering->yJoy, &steering->unk10, 
            CRSnowBike_SOUNDFLAG_Jets
        );
    }
    
    objData->prevTranslate.x = self->srt.transl.x;
    objData->prevTranslate.y = self->srt.transl.y;
    objData->prevTranslate.z = self->srt.transl.z;

    objData->flags |= CRSnowBike_FLAG_10_Was_In_Sequence;
    objData->flags &= ~CRSnowBike_FLAG_8_Race_Started;
    
    return 0;
}

// offset: 0x3AF8 | func: 29
void CRSnowBike_handle_collisions(Object* self, CRSnowBike_Data* objData, DLL27_Data* collision) {
    s32 pad;
    MtxF bikeMtx;
    Vec3f wsPoint[7];
    s8 flags;
    s8 sp86;
    s32 i;

    sp86 = -0x80 >> (7 - objData->numCollisionPoints);
    sp86 = ~sp86;
    objData->collisionFlags = -1;
    objData->forwardSpeed = objData->motion.velocity.z;
    
    if (sp86) {
        do {
            flags = sp86;
            for (i = 0; i < objData->numCollisionPoints; i++){
                //Get bike's transformation matrix
                CRSnowBike_get_bike_matrix(self, objData, &bikeMtx, TRUE, TRUE, TRUE);
                
                //Transform the bike's objectSpace collision point into worldSpace
                vec3_transform(&bikeMtx, 
                        dCollisionPoints[i].x, dCollisionPoints[i].y, dCollisionPoints[i].z, 
                        &wsPoint[i].x, &wsPoint[i].y, &wsPoint[i].z);

                //Check for collisions
                if (func_80059C40(&objData->wsCollisionCoords[i], &wsPoint[i], dCollisionRadii[i], 
                    0, NULL, self, 8, 1, 0xFF, 0) == 0
                ) {
                    //Unset bit if there's no collision
                    flags &= ~(1 << i);
                } else {
                    //Otherwise, store collision's worldSpace point, and move self away from point
                    bcopy(&wsPoint[i], &objData->wsCollisionCoords[i], sizeof(Vec3f));
                    bikeMtx.m[3][0] = wsPoint[i].x;
                    bikeMtx.m[3][1] = wsPoint[i].y;
                    bikeMtx.m[3][2] = wsPoint[i].z;
                    vec3_transform(&bikeMtx, 
                        -dCollisionPoints[i].x, -dCollisionPoints[i].y, -dCollisionPoints[i].z, 
                        &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z
                    );
                }
            }
            
            if (objData->collisionFlags == -1) {
                objData->collisionFlags = flags;
            }
        } while (flags);
    }
    
    bcopy(&wsPoint, objData->wsCollisionCoords, objData->numCollisionPoints * sizeof(Vec3f));
}

// offset: 0x3DAC | func: 30
s32 CRSnowBike_sharpclaw_handle_steering(Object* self, CRSnowBike_Data* arg1, CRSnowBike_Data* objData, CRSnowBike_SteerData* steering) {
    f32 dx;
    f32 dz;
    s32 checkpointsReturn;
    f32 speed;
    s32 dYaw;
    s32 value;
    CRSnowBike_Setup* objSetup;

    objSetup = (CRSnowBike_Setup*)self->setup;
    
    dx = self->srt.transl.x - objData->srtCurves.transl.x;
    dz = self->srt.transl.z - objData->srtCurves.transl.z;
    dx = sqrtf(SQ(dx) + SQ(dz));
    
    speed = 100.0f - dx;
    if (dx > 100.0f) {
        speed = 0.0f;
    }
    
    checkpointsReturn = CRSnowBike_sharpclaw_update_race_pathing(self, objData, speed);
    
    gDLL_4_Race->vtbl->func4(self, &objData->raceData);
    gDLL_4_Race->vtbl->func10(&objData->raceData);

    if (checkpointsReturn != 0) {
        steering->xJoy = 0;
        steering->yJoy = 0;
        return 1;
    }

    dx = self->srt.transl.x - objData->srtCurves.transl.x;
    dz = self->srt.transl.z - objData->srtCurves.transl.z;
    dYaw = arctan2_f(dx, dz) - (self->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(dYaw);
    
    value = dYaw >> 5;
    if (value > 65) {
        value = 65;
    } else if (value < -65) {
        value = -65;
    }
    
    steering->xJoy = -value;
    if (value < 0) {
        value = -value;
    }

    steering->yJoy = (objSetup->yJoySharpClaw - 8.0f) - (value * (objSetup->yJoySharpClaw * 0.02f));
    diPrintf(" YJOY %i ", steering->yJoy);
    return 0;
}

// offset: 0x3FE0 | func: 31
void CRSnowBike_update_collision_points(Object* self, CRSnowBike_Data* objData) {
    MtxF bikeMtx;
    s32 i;

    gDLL_27->vtbl->reset(self, &objData->collision);
    CRSnowBike_get_bike_matrix(self, objData, &bikeMtx, 0, 0, 0);
    
    for (i = 0; i < objData->numCollisionPoints; i++) {
        vec3_transform(&bikeMtx, 
            dCollisionPoints[i].x, dCollisionPoints[i].y, dCollisionPoints[i].z, 
            &objData->wsCollisionCoords[i].x, &objData->wsCollisionCoords[i].y, &objData->wsCollisionCoords[i].z
        );
    }
}

// offset: 0x40FC | func: 32
void CRSnowBike_handle_engine_sfx_and_modgfx(Object* self, CRSnowBike_Data* objData, f32 forwardSpeed, s32 yJoy, s8* arg6, u8 soundFlags) {
    /*0x2C*/ static f32 sEngineAudioTweak;
    f32 flamesArg = 1.0f;
    SRT fxTransform;
    s32 volume;
    
    if (soundFlags & CRSnowBike_SOUNDFLAG_Engine) {
        if (objData->soundHandleEngine == 0) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_289_Engine_Loop, MAX_VOLUME, &objData->soundHandleEngine, NULL, 0, NULL);
        }

        if (objData->soundHandleEngine != 0) {
            sEngineAudioTweak = forwardSpeed * 11.6f;
            if (sEngineAudioTweak < 0.0f) {
                sEngineAudioTweak = -sEngineAudioTweak;
            }
            if (sEngineAudioTweak < 40.0f) {
                sEngineAudioTweak = 40.0f;
            }
            if (sEngineAudioTweak > 200.0f) {
                sEngineAudioTweak = 200.0f;
            }
            gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandleEngine, (sEngineAudioTweak / 70.0f) + 0.1f);

            if (objData->framesInAir < 18) {
                volume = forwardSpeed * 30.0f;
                if (volume < 0) {
                    volume = -volume;
                }
                if (volume > MAX_VOLUME) {
                    volume = MAX_VOLUME;
                }
                gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandleEngine, volume);
            } else {
                gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandleEngine, 0);
            }
        }
    }
    
    if (soundFlags & CRSnowBike_SOUNDFLAG_Hiss) {
        if (objData->soundHandleHiss == 0) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_28F_Hiss_Loop, MAX_VOLUME, &objData->soundHandleHiss, NULL, 0, NULL);
        }
        if (objData->soundHandleHiss != 0) {
            sEngineAudioTweak = forwardSpeed ? ((self->srt.roll * forwardSpeed) / 30000.0f) : 0;
            if (sEngineAudioTweak < 0) {
                sEngineAudioTweak = -sEngineAudioTweak;
            } else if (sEngineAudioTweak > 1.0f) {
                sEngineAudioTweak = 1.0f;
            }
            gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandleHiss, 0.1f + sEngineAudioTweak);

            if (objData->framesInAir < 18) {
                sEngineAudioTweak *= MAX_VOLUME_F;
                if (sEngineAudioTweak > MAX_VOLUME_F) {
                    sEngineAudioTweak = MAX_VOLUME_F;
                } else if (sEngineAudioTweak < 0.0f) {
                    sEngineAudioTweak = 0.0f;
                }

                gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandleHiss, sEngineAudioTweak);
            } else {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandleHiss);
                objData->soundHandleHiss = 0;
            }
        }
    }
    
    if (soundFlags & CRSnowBike_SOUNDFLAG_Jets) {
        if (objData->fxTimer <= 0) {
            if (objData->soundHandleJets == 0) {
                gDLL_6_AMSFX->vtbl->play(self, SOUND_50D_Jet_Loop, MAX_VOLUME, &objData->soundHandleJets, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->play(self, SOUND_28E, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            if (objData->soundHandleRumble == 0) {
                gDLL_6_AMSFX->vtbl->play(self, SOUND_50C_Low_Rumble_Loop, MAX_VOLUME, &objData->soundHandleRumble, NULL, 0, NULL);
            }
        }
        
        if (objData->soundHandleJets != 0) {
            gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandleJets, (objData->soundFactorJets * ONE_OVER_2048_F) + 0.5f);
            if (yJoy >= 6) {
                objData->soundFactorJets += gUpdateRateF;
            } else {
                if (objData->soundFactorJets > 55.0f) {
                    objData->soundFactorJets -= (0.5f * gUpdateRateF);
                } else {
                    objData->soundFactorJets += (0.2f * gUpdateRateF);
                }
            }
            if (objData->soundFactorJets > 90.0f) {
                objData->soundFactorJets = 90.0f;
            }
            gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandleJets, objData->soundFactorJets);
        }

        if (objData->soundHandleRumble != 0) {
            gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandleRumble, (objData->soundFactorRumble / 75.0f) + 0.2f);
            if (yJoy >= 6) {
                objData->soundFactorRumble = (yJoy * 0.6f) + 15.0f;
            } else {
                if (0.5f * gUpdateRateF < objData->soundFactorRumble) {
                    objData->soundFactorRumble -= 0.5f * gUpdateRateF;
                } else {
                    objData->soundFactorRumble = 0.0f;
                }
            }
            if (objData->soundFactorRumble > MAX_VOLUME_F) {
                objData->soundFactorRumble = MAX_VOLUME_F;
            }
            gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandleRumble, objData->soundFactorRumble);
        }
    }
    
    //Handle modGfx
    {
        if (objData->modGfxDLLFlames == NULL) {
            objData->modGfxDLLFlames = dll_load_deferred(DLL_ID_141, 1);
        }
        
        if (objData->modGfxDLLWaves == NULL) {
            objData->modGfxDLLWaves = dll_load_deferred(DLL_ID_142, 1);
        }
        
        if (objData->fxTimer <= 0) {
            //Create thruster flames when moving forward
            flamesArg = 0.7f;
            if (yJoy > 10) {
                flamesArg = yJoy;
                flamesArg = (0.01f * flamesArg) + 0.1f;
                objData->modGfxDLLFlames->vtbl->func0(self, 0, 0, 0x404, -1, &flamesArg);
            }

            objData->fxTimer = 30;

            //Create wave modGfx when turning sharply
            if ((self->srt.roll > 1000) && (forwardSpeed < -1.0f)) {
                fxTransform.scale = self->srt.roll / 8000.0f;
                fxTransform.scale *= (-forwardSpeed / 3.8f);
                objData->modGfxDLLWaves->vtbl->func0(self, 0, &fxTransform, 0x404, -1, 0);
            } else if ((self->srt.roll < -1000) && (forwardSpeed < -1.0f)) {
                fxTransform.scale = self->srt.roll / -8000.0f;
                fxTransform.scale *= (-forwardSpeed / 3.8f);
                objData->modGfxDLLWaves->vtbl->func0(self, 1, &fxTransform, 0x404, -1, 0);
            }
        }
    }

    objData->fxTimer -= gUpdateRate;
}

// offset: 0x4B30 | func: 33 | export: 20
void CRSnowBike_func_4B30(Object* self, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {

}
