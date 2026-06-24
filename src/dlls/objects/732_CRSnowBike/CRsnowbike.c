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
#include "sys/objlib.h"
#include "sys/objtype.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/267_checkpoint4.h"

typedef struct {
    s8 unk0[0xE];
    s8 xJoy;
    s8 yJoy;
    s8 unk10;
} DLL732_Unk_2E0; //Controller/joystick-related

typedef struct {
    Vec3f unk0;
    Vec3f velocity; //Bike's objectSpace velocity (Z points out the back of the bike, so forward velocity unit vector is {0,0,-1}) 
    f32 maxAccelerationPerFrame; //Per sixtieth of a second
    f32 unk1C;
    f32 maxAcceleration;         //Per second
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} CRSnowBike_Motion;

typedef enum {
    STATE_0_Parked,
    STATE_1,
    STATE_2,
    STATE_3
} CRSnowBike_States;

typedef enum {
    CRSnowBike_FLAG_0 = 0,
    CRSnowBike_FLAG_1_Finished = 1,
    CRSnowBike_FLAG_2_Driving_In_Void = 2,
    CRSnowBike_FLAG_4_Grounded = 4,
    CRSnowBike_FLAG_8_Race_Started = 8,
    CRSnowBike_FLAG_10_Was_In_Sequence = 0x10,
    CRSnowBike_FLAG_20_SharpClaw_Bike = 0x20
} CRSnowBike_Flags;

typedef enum {
    CRSnowBike_SOUNDFLAG_0 = 0,
    CRSnowBike_SOUNDFLAG_Engine = 1,
    CRSnowBike_SOUNDFLAG_Hiss = 2,
    CRSnowBike_SOUNDFLAG_Jets = 4,
    CRSnowBike_SOUNDFLAG_8 = 8
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
    u8 isSharpClawBike;
    s16 gamebitUnlocked;
    u8 racetrackIdx;
    u8 unk1D;
    s16 gamebitFinished;
    u8 yJoySharpClaw;
} CRSnowBike_Setup;

typedef struct {
    SRT unk0;
    RaceStruct raceData;
    s8 _unk3C[0x48 - 0x3C];
    u8 racetrackIdx;                //See `DLL732_Racetracks`
    u8 unk49;
    DLL27_Data collision;
    CRSnowBike_Motion unk2AC;
    DLL732_Unk_2E0 steering;
    DLL_IModgfx* modGfxDLLFlames;
    DLL_IModgfx* modGfxDLLWaves;
    CRSnowBike_Gamebits* gamebitIDs;
    s8 _unk300[0x330 - 0x300];
    Vec3f wsCollisionCoords[5]; //worldSpace coords of dCollisionPoints' collisions
    s8 _unk36C[0x384 - 0x36C];
    f32 stallFactor;
    Vec3f attachPointCoords;
    f32 soundFactorRumble;
    f32 soundFactorJets;
    f32 forwardSpeed;
    Vec3f prevTranslate;        //worldSpace coordinates at the start of the tick
    Vec3f unk3AC;
    u32 soundHandleEngine;
    u32 soundHandleJets;
    u32 soundHandleHiss;
    u32 soundHandleRumble;
    s32 fuelAmount;
    Vec3f maxVelocity;
    s8 _unk3D8[0x3DC - 0x3D8];
    s16 yawOffset;
    s16 rollOffset;
    s16 yaw;
    s16 pitch;
    s16 roll;
    s16 pitchOffset;    //Tilt forward/back while airborne, based on joyY
    s8 _unk3E8[0x3EA - 0x3E8];
    s16 fxTimer;
    s8 stallFrames;
    u8 mountingFrom;
    u8 numCollisionPoints;
    u8 flags;
    s8 state;
    u8 unk3F1;
    s8 framesInAir;
    s8 unk3F3;
    u8 unk3F4_0 : 1;
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
/*0xF0*/ static Vec3f dBikeSidePoints[] = {
    VEC3F(14.5, 0, 9), 
    VEC3F(-14.5, 0, 9)
};

//Textures: not referenced in the DLL itself, but possibly intended to keep the modGfx's textures loaded
/*0x0*/ static Texture* sTexVerticalWave;       //(TEX0_486) 1 vertical wave 
/*0x4*/ static Texture* sTexThrusterFlames;     //(TEX0_253) thruster flames 
/*0x8*/ static Texture* sTexDriftWaves;         //(TEX0_136) 3 horizontal waves, used for effect when turning

/*0x10*/ static SRT sRotationTransform;

#include "prevent_bss_reordering.h"

static s32 CRSnowBike_func_0(Object* self, CRSnowBike_Data* objData, f32 arg2);
void CRSnowBike_sharpclaw_start_race(Object* self);
static void CRSnowBike_func_1DC8(Object* self, CRSnowBike_Data* objData, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols);
static void CRSnowBike_func_2340(Object* self, CRSnowBike_Data* objData, CRSnowBike_Motion* arg2, f32 updateRate, s32 arg4);
static void CRSnowBike_func_22BC(Object* self, CRSnowBike_Motion* arg1);
static void CRSnowBike_func_2E64(Object* self, CRSnowBike_Data* objData, CRSnowBike_Motion* arg2, f32 updateRate, s32 arg4);
static void CRSnowBike_func_3618(Object* self, DLL732_Unk_2E0* arg1, u8 controllerPort, s32 buffer);
static void CRSnowBike_get_bike_matrix(Object* self, CRSnowBike_Data* objData, MtxF* oMtx, s32 useYawOffset, s32 useRoll, s32 usePitch);
static void CRSnowBike_func_3748(Object* self, CRSnowBike_Data* objData);
static int CRSnowBike_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void CRSnowBike_func_3AF8(Object* self, CRSnowBike_Data* objData, DLL27_Data* collision);
static s32 CRSnowBike_sharpclaw_do_steering(Object* self, CRSnowBike_Data* arg1, CRSnowBike_Data* objData, DLL732_Unk_2E0* arg3);
static void CRSnowBike_update_collision_points(Object* self, CRSnowBike_Data* objData);
static void CRSnowBike_handle_engine_sfx_and_modgfx(Object* self, CRSnowBike_Data* objData, f32 forwardSpeed, s32 yJoy, s8* arg6, u8 soundFlags);

// offset: 0x0 | func: 0
static s32 CRSnowBike_func_0(Object* self, CRSnowBike_Data* data, f32 arg2) {
    RaceCheckpointSetup* checkpointSetup;
    s32 sp30;
    CRSnowBike_Data* objData;
    
    objData = self->data;

    checkpointSetup = gDLL_4_Race->vtbl->func8(data->raceData.unk10, &sp30);
    if (checkpointSetup->unk20[1] == -1) {
        objData->unk3F4_0 = rand_next(0, 1);
    }
    
    return gDLL_4_Race->vtbl->func5(&data->unk0, &data->raceData, arg2, 1, 0, objData->unk3F4_0);
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
    CRSnowBike_func_22BC(self, &objData->unk2AC);

    func_80023D30(self, 0, 0.0f, 0);

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_4000 | OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }

    if (setup->isSharpClawBike) {
        objData->flags |= CRSnowBike_FLAG_20_SharpClaw_Bike;
    }

    objData->raceData.unk10 = -1;
    objData->raceData.unk14 = -1;
    objData->raceData.unk18 = -1;
    objData->racetrackIdx = setup->racetrackIdx;
    objData->unk49 = setup->unk1D;
    
    objData->unk0.transl.x = self->srt.transl.x;
    objData->unk0.transl.y = self->srt.transl.y;
    objData->unk0.transl.z = self->srt.transl.z;
    
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
        flagsValue = CRSnowBike_FLAG_0;
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
    CRSnowBike_Motion* bikeMotion;
    CRSnowBike_Data* objData;
    DLL732_Unk_2E0* driverSteering;
    f32 dx;
    f32 dz;
    s32 sp5C;
    f32 var_fv1;
    s32 updateIdx;
    s32 flagValue;
    CRSnowBike_Motion* motion;

    objData = self->data;
    objSetup = (CRSnowBike_Setup*)self->setup;
    
    bikeMotion = &objData->unk2AC;
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

        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) == FALSE) {
            bikeMotion->velocity.x = 0.0f;
            bikeMotion->velocity.y = 0.0f;
            bikeMotion->velocity.z = -2.0f; //flings bike forward after sequence
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
        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) == FALSE) {
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
                vec3_transform(&bikeMtx, dBikeSidePoints[0].x, dBikeSidePoints[0].y, dBikeSidePoints[0].z, &wsPoint.z, &wsPoint.y, &wsPoint.x);
                dx = player->srt.transl.x - wsPoint.z;
                dz = player->srt.transl.z - wsPoint.x;
                if ((SQ(dx) + SQ(dz)) < SQ(10)) {
                    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                    objData->mountingFrom = SIDE_RIGHT;
                } else {
                    //Show LockIcon if the player is close to the right side of the bike
                    vec3_transform(&bikeMtx, dBikeSidePoints[1].x, dBikeSidePoints[1].y, dBikeSidePoints[1].z, &wsPoint.z, &wsPoint.y, &wsPoint.x);
                    dx = player->srt.transl.x - wsPoint.z;
                    dz = player->srt.transl.z - wsPoint.x;
                    if ((SQ(dx) + SQ(dz)) < SQ(10)) {
                        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                        objData->mountingFrom = SIDE_LEFT;
                    }
                }
            }
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
            objData->modGfxDLLFlames = NULL;
        }

        if (objData->modGfxDLLWaves != NULL) {
            dll_unload(objData->modGfxDLLWaves);
            objData->modGfxDLLWaves = NULL;
        }

        objData->flags &= ~CRSnowBike_FLAG_8_Race_Started;
        break;
    case STATE_2:
        if ((objData->flags & CRSnowBike_FLAG_8_Race_Started) == FALSE) {
            objData->raceData.unk10 = -1;
            objData->raceData.unk14 = -1;
            objData->raceData.unk18 = -1;
            objData->raceData.unk1C = 0;

            //Check if the race has started
            if (main_get_bits(objData->gamebitIDs->started)) {
                flagValue = CRSnowBike_FLAG_8_Race_Started;
            } else {
                flagValue = CRSnowBike_FLAG_0;
            }
            objData->flags |= flagValue;

            if (objData->flags & CRSnowBike_FLAG_8_Race_Started) {
                if (objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) {
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

        CRSnowBike_func_3748(self, objData);

        if (objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) {
            if ((objData->flags & CRSnowBike_FLAG_8_Race_Started) == FALSE) {
                return;
            }
            
            if (map_world_coords_to_block_index(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z) >= 0) {
                if (objData->flags & CRSnowBike_FLAG_2_Driving_In_Void) {
                    sp5C = CRSnowBike_func_0(self, objData, 2.8f * gUpdateRateF);
                    gDLL_4_Race->vtbl->func4(self, &objData->raceData);
                    gDLL_4_Race->vtbl->func10(&objData->raceData);
                    
                    if (sp5C == 0) {
                        motion = &objData->unk2AC;
                        self->srt.yaw = arctan2_f(self->srt.transl.x - objData->unk0.transl.x, self->srt.transl.z - objData->unk0.transl.z);
                        self->srt.transl.x = objData->unk0.transl.x;
                        self->srt.transl.y = objData->unk0.transl.y;
                        self->srt.transl.z = objData->unk0.transl.z;
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
                } else if (CRSnowBike_sharpclaw_do_steering(self, objData, objData, &objData->steering)) {
                    return;
                }
            } else {
                sp5C = CRSnowBike_func_0(self, objData, 2.8f * gUpdateRateF);
                gDLL_4_Race->vtbl->func4(self, &objData->raceData);
                gDLL_4_Race->vtbl->func10(&objData->raceData);
                if (sp5C == 0) {
                    self->srt.yaw = arctan2_f(self->srt.transl.x - objData->unk0.transl.x, self->srt.transl.z - objData->unk0.transl.z);
                    self->srt.transl.x = objData->unk0.transl.x;
                    self->srt.transl.y = objData->unk0.transl.y;
                    self->srt.transl.z = objData->unk0.transl.z;
                    objData->flags |= CRSnowBike_FLAG_2_Driving_In_Void;
                }
                return;
            }
        } else {
            self->objhitInfo->unk5B = 0xA;
            self->objhitInfo->unk5C = 0xA;
        }

        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) == FALSE) {
            if (objData->flags & CRSnowBike_FLAG_8_Race_Started) {
                gDLL_4_Race->vtbl->func4(self, &objData->raceData);
                gDLL_4_Race->vtbl->func10(&objData->raceData);
                objData->unk3F1 = gDLL_4_Race->vtbl->func12(&objData->raceData);
            }

            for (updateIdx = 0; updateIdx < gUpdateRate; updateIdx++){
                CRSnowBike_func_3618(self, &objData->steering, 0, updateIdx);
                CRSnowBike_func_2340(self, objData, &objData->unk2AC, gUpdateRateF, (updateIdx + 1) == gUpdateRate);
                objData->yawOffset += (s16) ((-objData->steering.xJoy * 60.0f) - objData->yawOffset) >> 4;
                objData->rollOffset += (s16) ((-objData->steering.xJoy * 105.0f) - objData->rollOffset) >> 4;
                self->srt.yaw = objData->yaw + objData->yawOffset;
                self->srt.roll = objData->roll + objData->rollOffset;
            }
            
            motion = &objData->unk2AC;

            //Handle fuel depletion
            if (objData->fuelAmount >= 0) {
                //Deplete fuel based on bike's speed
                objData->fuelAmount = objData->fuelAmount - (s32) (VECTOR_MAGNITUDE(motion->velocity) * gUpdateRateF * 1.5f) - gUpdateRate;
                diPrintf(" FUEL AMT %i \n", objData->fuelAmount / 10);
                gDLL_1_cmdmenu->vtbl->energy_bar_set(objData->fuelAmount);
            } else if (objData->maxVelocity.x > 0.1f) {
                diPrintf(" \tRAN OUT OF FUEL \t");
                
                //Play sound randomly while out of fuel
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
            for (updateIdx = 0; updateIdx < gUpdateRate; updateIdx++) {
                CRSnowBike_func_2E64(self, objData, &objData->unk2AC, gUpdateRateF, (updateIdx + 1) == gUpdateRate);
                objData->yawOffset += (s16) ((-objData->steering.xJoy * 60.0f) - objData->yawOffset) >> 4;
                objData->rollOffset += (s16) ((-objData->steering.xJoy * 105.0f) - objData->rollOffset) >> 4;
                self->srt.yaw = objData->yaw + objData->yawOffset;
                self->srt.roll = objData->roll + objData->rollOffset;
            }
        }
        
        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) == FALSE) {
            CRSnowBike_handle_engine_sfx_and_modgfx(self, objData, 
                objData->unk2AC.velocity.z, driverSteering->yJoy, 
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

    if (((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) == FALSE) && (objData->state == STATE_2)) {
        if (objData->forwardSpeed >= 0.0f) {
            var_fv1 = objData->forwardSpeed;
        } else {
            var_fv1 = -objData->forwardSpeed;
        }
        if (var_fv1 > 2.0f) {
            self->objhitInfo->unk5F = 0x14;
            self->objhitInfo->unk60 = 1;
            self->objhitInfo->unk58 |= 1;
            self->objhitInfo->unk40 = 0x10;
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
    MtxF sp9C;
    MtxF bikeMtx;
    CRSnowBike_Motion* bikeMotion;
    Object* obj;
    s32 objID;
    s32 i;

    objHitInfo = self->objhitInfo;
    objData = self->data;
    bikeMotion = &objData->unk2AC;

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
    
    if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) == FALSE) {
        sRotationTransform.yaw = -objData->yaw;
        sRotationTransform.pitch = -objData->pitch;
        sRotationTransform.roll = -objData->roll;
        matrix_from_srt_reversed(&sp9C, &sRotationTransform);
        self->velocity.f[0] = (self->srt.transl.x - self->prevLocalPosition.x) * gUpdateRateInverseF;
        self->velocity.f[1] = (self->srt.transl.y - self->prevLocalPosition.y) * gUpdateRateInverseF;
        self->velocity.f[2] = (self->srt.transl.z - self->prevLocalPosition.z) * gUpdateRateInverseF;
        wsVelocity.x = self->velocity.f[0] * 0.93749994f;
        wsVelocity.y = self->velocity.f[1] * 0.93749994f;
        wsVelocity.z = self->velocity.f[2] * 0.93749994f;
        
        vec3_transform(&sp9C, wsVelocity.f[0], wsVelocity.f[1], wsVelocity.f[2], 
                       &bikeMotion->velocity.x, &bikeMotion->velocity.y, &bikeMotion->velocity.z);
        CRSnowBike_get_bike_matrix(self, objData, &bikeMtx, FALSE, FALSE, FALSE);
        vec3_transform(&bikeMtx, 0.0f, 0.0f, -10.0f, &objData->unk3AC.x, &objData->unk3AC.y, &objData->unk3AC.z);
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
        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) == FALSE) {
            CRSnowBike_func_1DC8(self, objData, gdl, mtxs, vtxs, pols);
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
    
    if (objData->state == STATE_2) {
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
    
    if (!(objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike)) {
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
    
    if (objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) {
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
void CRSnowBike_func_1A34(Object* self, f32* ox, f32* oy, f32* oz) {
    CRSnowBike_Data* objData = self->data;
    
    *ox = objData->unk3AC.x;
    *oy = objData->unk3AC.y;
    *oz = objData->unk3AC.z;
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

    if ((state == STATE_2) && !(objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike)) {
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
f32 CRSnowBike_get_speed_tvalue_and_max(Object* self, f32* maxSpeed) {
    CRSnowBike_Data* objData;
    f32 tValueSpeed;

    objData = self->data;
    
    *maxSpeed = 5;

    tValueSpeed = VECTOR_MAGNITUDE(objData->unk2AC.velocity) * 0.2f;
    if (tValueSpeed > 1.0f) {
        tValueSpeed = 1.0f;
    }
    return tValueSpeed;
}

// offset: 0x1C28 | func: 18 | export: 17
s8 CRSnowBike_func_1C28(Object* self) {
    CRSnowBike_Data* objData = self->data;
    return objData->unk3F1;
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
    
    objData->unk0.transl.x = self->srt.transl.x;
    objData->unk0.transl.y = self->srt.transl.y;
    objData->unk0.transl.z = self->srt.transl.z;
    objData->unk0.yaw = self->srt.yaw;
}

// offset: 0x1DB8 | func: 20 | export: 19
void CRSnowBike_func_1DB8(s32 arg0, s32 arg1) {

}


// offset: 0x1DC8 | func: 21
void CRSnowBike_func_1DC8(Object* self, CRSnowBike_Data* objData, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols) {
/*0x28*/ static u32 sImpactSoundHandle;
    Vertex* vertex;
    Gfx* gfx;
    Triangle* tri;
    s32 pad1[6];
    SRT fxTransform;
    s32 i;
    f32 translateX;
    s32 pad2;
    CRSnowBike_Motion* bikeMotion;
    u32 dColourRGBA[] = { 6, 105, 105, 255}; //Unused RGBA colour, maybe for randomised spark colours?
    s32 volume;
    s32 pad3;

    bikeMotion = &objData->unk2AC;

    gfx = *gdl;
    vertex = *vtxs;
    tri = *pols;
    
    translateX = 0.0f;
    if (bikeMotion->velocity.z < 0.0f) {
        translateX = bikeMotion->velocity.z;
    }
    fxTransform.transl.z = translateX;

    if (bikeMotion->velocity.z < 0.0f) {
        translateX = bikeMotion->velocity.x;
    }
    fxTransform.transl.x = translateX;
    
    //Set particle colour (just set to white, but maybe once intended to use dColourRGBA?)
    dl_set_prim_color(&gfx, 0xFF, 0xFF, 0xFF, 0xFF);
    
    //Create different particles based on bike's forward speed
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
    
    i = 0;
    if (objData->steering.yJoy > 0) {
        i = 2;
    }

    while (i) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_131, NULL, 4, -1, NULL);
        i--;
    }
    
    fxTransform.yaw = 0;
    fxTransform.pitch = 0;
    fxTransform.roll = 0;
    fxTransform.scale = 1.0f;

    if (objData->forwardSpeed < -1.2f) {
        dColourRGBA[1] += rand_next(0, 155);
        dColourRGBA[2] += rand_next(0, 155);
        volume = ((0.0f - objData->forwardSpeed) * 21.0f);
        if (objData->unk3F3 & 1) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_292_Impact, volume, &sImpactSoundHandle, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->set_pitch(sImpactSoundHandle, (volume / 127.0f) + 0.5f);
        } else if (objData->unk3F3 & 2) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_292_Impact, volume, &sImpactSoundHandle, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->set_pitch(sImpactSoundHandle, (volume / 127.0f) + 0.5f);
        } else if (objData->unk3F3 & 4) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_292_Impact, volume, &sImpactSoundHandle, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->set_pitch(sImpactSoundHandle, (volume / 127.0f) + 0.5f);
        }
    }
    
    *gdl = gfx;
    *vtxs = vertex;
    *pols = tri;
}

// offset: 0x22BC | func: 22
void CRSnowBike_func_22BC(Object* self, CRSnowBike_Motion* motion) {
    motion->unk1C = 90;
    motion->maxAcceleration = 1/motion->unk1C;  
    motion->unk28 = -11.0f;
    motion->unk2C = 0.1f;
    motion->unk30 = 13.0f;
    motion->maxAccelerationPerFrame = (1/motion->unk1C) * ONE_OVER_SIXTY_F;
    
    sRotationTransform.transl.x = 0;
    sRotationTransform.transl.y = 0;
    sRotationTransform.transl.z = 0;
    sRotationTransform.scale = 1;
}

// offset: 0x2340 | func: 23
void CRSnowBike_func_2340(Object* self, CRSnowBike_Data* objData, CRSnowBike_Motion* motion, f32 updateRate, s32 arg4) {
    MtxF mtx;
    MtxF invMtx;
    MtxF spB0;
    f32 pad1;
    f32 minusLimit;
    f32 sqForwardSpeed;
    f32 acceleration;
    f32 newVelocityX;
    f32 newVelocityZ2;
    f32 sp94;
    f32 accelerationAmount;
    Vec3f sp84;
    f32 yValue;
    f32 newVelocityZ;
    s32 var_a0;
    Vec3f sp6C;
    Vec3f sp60;
    s32 pad2;
    DLL27_Data* collision;
    s32 angle1;
    s32 angle2;
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
    
    vec3_transform(&invMtx, 
        0.0f, motion->unk28 * motion->unk1C, 0.0f, 
        &sp84.f[0], &sp84.f[1], &sp84.f[2]);

    //Handle forward acceleration/deceleration
    {
        if (objData->steering.yJoy < 0) {
            accelerationAmount = -(f32) objData->steering.yJoy * 4.0f;
        } else {
            accelerationAmount = -(f32) objData->steering.yJoy * 10.0f;
        }
        
        acceleration = motion->maxAcceleration * accelerationAmount;

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
    
    motion->unk0.x = sp84.f[0] * motion->maxAccelerationPerFrame;
    motion->unk0.y = sp84.f[1] * motion->maxAccelerationPerFrame;
    motion->unk0.z = sp84.f[2] * motion->maxAccelerationPerFrame;
    motion->velocity.x = motion->unk0.x + motion->velocity.x;
    motion->velocity.y = motion->unk0.y + motion->velocity.y;
    motion->velocity.z = motion->unk0.z + motion->velocity.z;
    
    if (collision->unk25D != 0) {
        acceleration = motion->unk2C * sp84.y;
        if (motion->velocity.z < 0.0f) {
            if (acceleration < 0.0f) {
                acceleration = -acceleration;
            }
        } else if (acceleration > 0.0f) {
            acceleration = -acceleration;
        }
        acceleration *= motion->maxAccelerationPerFrame;

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
        
        if (motion->velocity.z < 0.0f) {
            newVelocityZ = -motion->velocity.z;
        } else {
            newVelocityZ = motion->velocity.z;
        }
        
        acceleration = motion->unk2C * sp84.y * (4.0f + SQ(newVelocityZ));
        if (motion->velocity.x < 0.0f) {
            if (acceleration < 0.0f) {
                acceleration = -acceleration;
            }
        } else if (acceleration > 0.0f) {
            acceleration = -acceleration;
        }
        acceleration *= motion->maxAccelerationPerFrame;
        
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
        
        objData->framesInAir = 0;
        objData->pitchOffset = 0;
    } else {
        objData->framesInAir++;
        if (objData->framesInAir > 100) {
            objData->framesInAir = 100;
        }
    }

    sqForwardSpeed = SQ(motion->velocity.z);
    acceleration = motion->unk30 * sqForwardSpeed;
    if (motion->velocity.z > 0.0f) {
        acceleration = -acceleration;
    }
    acceleration *= motion->maxAccelerationPerFrame;

    newVelocityZ2 = motion->velocity.z + acceleration;
    if (motion->velocity.z < 0.0f) {
        if (newVelocityZ2 > 0.0f) {
            motion->velocity.z = 0.0f;
        } else {
            motion->velocity.z = newVelocityZ2;
        }
    } else if (newVelocityZ2 < 0.0f) {
        motion->velocity.z = 0.0f;
    } else {
        motion->velocity.z = newVelocityZ2;
    }
    
    vec3_transform(&mtx, motion->velocity.x, motion->velocity.y, motion->velocity.z, 
                   &self->velocity.f[0], &self->velocity.f[1], &self->velocity.f[2]);
    VECTOR_SCALE(self->velocity, 1.0666667f);
    obj_move(self, self->velocity.x, self->velocity.f[1], self->velocity.f[2]);

    if (arg4 != 0) {
        sp94 = 1.0f / updateRate;
        CRSnowBike_func_3AF8(self, objData, collision);
        gDLL_27->vtbl->func_1E8(self, collision, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, collision);
        gDLL_27->vtbl->func_624(self, collision, updateRate);
        
        self->velocity.x = (self->srt.transl.x - self->prevLocalPosition.x) * sp94;
        self->velocity.y = (self->srt.transl.y - self->prevLocalPosition.y) * sp94;
        self->velocity.z = (self->srt.transl.z - self->prevLocalPosition.z) * sp94;
        
        sp60.f[0] = self->velocity.x * 0.93749994f;
        sp60.f[1] = self->velocity.y * 0.93749994f;
        sp60.f[2] = self->velocity.z * 0.93749994f;
        vec3_transform(&invMtx, sp60.f[0], sp60.f[1], sp60.f[2], 
                       &motion->velocity.x, &motion->velocity.y, &motion->velocity.z);
        sp6C.f[0] = 0.0f;
        sp6C.f[1] = 1.0f;
        sp6C.f[2] = 0.0f;
        
        if (collision->unk25C & 0xF) {
            objData->flags |= CRSnowBike_FLAG_4_Grounded;
        } else {
            objData->flags &= ~CRSnowBike_FLAG_4_Grounded;
        }
        
        for (var_a0 = 0, i = 0; i < 4; i++) {
            if (collision->unk25C & (1 << i)) {
                sp6C.x = collision->unk68.unk0[i].x + sp6C.x;
                sp6C.y = collision->unk68.unk0[i].y + sp6C.y;
                sp6C.z = collision->unk68.unk0[i].z + sp6C.z;
                var_a0++;
            }
        }
        
        VECTOR_SCALE(sp6C, 0.25f);
        if (var_a0 != 0) {
            acceleration = 1.0f / var_a0;
            VECTOR_SCALE(sp6C, acceleration);
        } else {
            sp6C.f[0] = 0.0f;
            sp6C.f[1] = 1.0f;
            sp6C.f[2] = 0.0f;
        }
        
        sRotationTransform.yaw = -objData->yaw;
        sRotationTransform.pitch = 0;
        sRotationTransform.roll = 0;
        matrix_from_srt_reversed(&spB0, &sRotationTransform);
        vec3_transform(&spB0, sp6C.f[0], sp6C.f[1], sp6C.f[2], &sp6C.f[0], &sp6C.f[1], &sp6C.f[2]);
        
        angle1 = M_90_DEGREES - arctan2_f(sp6C.f[1], sp6C.f[2]);
        angle2 = -(M_90_DEGREES - arctan2_f(sp6C.f[1], sp6C.f[0]));
        angle1 -= (objData->pitch & 0xFFFF);
        CIRCLE_WRAP(angle1);
        objData->pitch += (((angle1 >> 2) / 3) * (s32) updateRate);
        self->srt.pitch = objData->pitch + objData->pitchOffset;
        
        angle2 -= (objData->roll & 0xFFFF);
        CIRCLE_WRAP(angle2);
        objData->roll += (((angle2 >> 2) / 3) * (s32) updateRate);
    }
    
    objData->yaw -= (s16) (objData->steering.xJoy * (70.0f - (objData->steering.yJoy * 0.05f)) * 0.0666f);
}

// offset: 0x2E64 | func: 24
void CRSnowBike_func_2E64(Object* self, CRSnowBike_Data* objData, CRSnowBike_Motion* arg2, f32 updateRate, s32 arg4) {
    MtxF sp120;
    MtxF spE0;
    MtxF spA0;
    DLL27_Data* collision;
    f32 temp_fv0;
    f32 var_fv1;
    f32 sp90;
    s32 temp_a1;
    Vec3f sp80;
    Vec3f sp74;
    Vec3f sp68;
    s32 pad;
    s32 var_a0;
    s32 var_s0;
    s32 var_v1;
    s32 i;
    f32 sq;

    collision = &objData->collision;
    
    if (arg2->velocity.f[0] > 4.0f) {
        arg2->velocity.f[0] = 4.0f;
    }
    if (arg2->velocity.f[0] < -4.0f) {
        arg2->velocity.f[0] = -4.0f;
    }
    if (arg2->velocity.f[1] > 4.0f) {
        arg2->velocity.f[1] = 4.0f;
    }
    if (arg2->velocity.f[1] < -4.0f) {
        arg2->velocity.f[1] = -4.0f;
    }
    if (arg2->velocity.f[2] > 5.0f) {
        arg2->velocity.f[2] = 5.0f;
    }
    if (arg2->velocity.f[2] < -5.8f) {
        arg2->velocity.f[2] = -5.8f;
    }
    
    sRotationTransform.yaw = objData->yaw;
    sRotationTransform.pitch = objData->pitch;
    sRotationTransform.roll = objData->roll;
    matrix_from_srt(&sp120, &sRotationTransform);
    
    sRotationTransform.yaw = -objData->yaw;
    sRotationTransform.pitch = -objData->pitch;
    sRotationTransform.roll = -objData->roll;
    matrix_from_srt_reversed(&spE0, &sRotationTransform);
    
    vec3_transform(&spE0, 
        0.0f, arg2->unk28 * arg2->unk1C, 0.0f, 
        &sp80.f[0], &sp80.f[1], &sp80.f[2]
    );

    temp_fv0 = -(f32) objData->steering.yJoy * 50.0f;
    temp_fv0 *= arg2->maxAccelerationPerFrame;
    arg2->velocity.f[2] += temp_fv0;
    arg2->unk0.f[0] = sp80.f[0] * arg2->maxAccelerationPerFrame;
    arg2->unk0.f[1] = sp80.f[1] * arg2->maxAccelerationPerFrame;
    arg2->unk0.f[2] = sp80.f[2] * arg2->maxAccelerationPerFrame;
    arg2->velocity.f[0] = arg2->velocity.f[0] + arg2->unk0.f[0];
    arg2->velocity.f[1] = arg2->velocity.f[1] + arg2->unk0.f[1];
    arg2->velocity.f[2] = arg2->velocity.f[2] + arg2->unk0.f[2];
    
    if (collision->unk25D != 0) {
        arg2->velocity.f[0] = 0.0f;
        var_fv1 = arg2->unk2C * sp80.y;
        
        if (arg2->velocity.f[2] < 0.0f) {
            if (var_fv1 < 0.0f) {
                var_fv1 = -var_fv1;
            }
        } else if (var_fv1 > 0.0f) {
            var_fv1 = -var_fv1;
        }

        var_fv1 *= arg2->maxAccelerationPerFrame;
        temp_fv0 = arg2->velocity.f[2] + var_fv1;
        if (temp_fv0 > 0.0f) {
            arg2->velocity.f[2] = 0.0f;
        } else {
            arg2->velocity.f[2] = temp_fv0;
        }
    }

    sq = SQ(arg2->velocity.f[2]);
    var_fv1 = arg2->unk30 * sq;
    var_fv1 *= arg2->maxAccelerationPerFrame;
    
    temp_fv0 = arg2->velocity.f[2] + var_fv1;
    if (temp_fv0 > 0.0f) {
        arg2->velocity.f[2] = 0.0f;
    } else {
        arg2->velocity.f[2] = temp_fv0;
    }
    
    vec3_transform(&sp120, arg2->velocity.f[0], arg2->velocity.f[1], arg2->velocity.f[2], &self->velocity.f[0], &self->velocity.f[1], &self->velocity.f[2]);
    VECTOR_SCALE(self->velocity, 1.0666667f);
    obj_move(self, self->velocity.x, self->velocity.f[1], self->velocity.f[2]);
    
    if (arg4 != 0) {
        sp90 = 1.0f / updateRate;
        gDLL_27->vtbl->func_1E8(self, collision, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, collision);
        gDLL_27->vtbl->func_624(self, collision, updateRate);
        self->velocity.x = (self->srt.transl.x - self->prevLocalPosition.x) * sp90;
        self->velocity.y = (self->srt.transl.y - self->prevLocalPosition.y) * sp90;
        self->velocity.z = (self->srt.transl.z - self->prevLocalPosition.z) * sp90;
        sp68.f[0] = self->velocity.x * 0.93749994f;
        sp68.f[1] = self->velocity.y * 0.93749994f;
        sp68.f[2] = self->velocity.z * 0.93749994f;
        vec3_transform(&spE0, sp68.f[0], sp68.f[1], sp68.f[2], &arg2->velocity.f[0], &arg2->velocity.f[1], &arg2->velocity.f[2]);
        
        sp74.f[0] = 0.0f;
        sp74.f[1] = 1.0f;
        sp74.f[2] = 0.0f;
        
        if (collision->unk25C & 0xF) {
            objData->flags |= CRSnowBike_FLAG_4_Grounded;
        } else {
            objData->flags &= ~CRSnowBike_FLAG_4_Grounded;
        }

        for (var_a0 = 0, i = 0; i < 4; i++) {
            if (collision->unk25C & (1 << i)) {
                sp74.f[0] = sp74.f[0] + collision->unk68.unk0[i].f[0];
                sp74.f[1] = sp74.f[1] + collision->unk68.unk0[i].f[1];
                sp74.f[2] = sp74.f[2] + collision->unk68.unk0[i].f[2];
                var_a0++;
            }
        }
        
        if (var_a0 != 0) {
            VECTOR_SCALE(sp74, 1.0f / var_a0);
        } else {
            sp74.f[0] = 0.0f;
            sp74.f[1] = 1.0f;
            sp74.f[2] = 0.0f;
        }
        
        sRotationTransform.yaw = -objData->yaw;
        sRotationTransform.pitch = 0;
        sRotationTransform.roll = 0;
        matrix_from_srt_reversed(&spA0, &sRotationTransform);
        vec3_transform(&spA0, sp74.f[0], sp74.f[1], sp74.f[2], &sp74.f[0], &sp74.f[1], &sp74.f[2]);
        
        var_s0 = M_90_DEGREES - arctan2_f(sp74.f[1], sp74.f[2]);
        temp_a1 = -(M_90_DEGREES - arctan2_f(sp74.f[1], sp74.f[0]));
        var_s0 -= (objData->pitch & 0xFFFF);
        CIRCLE_WRAP(var_s0);
        objData->pitch += ((var_s0 >> 2) / 3) * (s32)updateRate;
        self->srt.pitch = objData->pitch + objData->pitchOffset;
        temp_a1 -= (objData->roll & 0xFFFF);
        CIRCLE_WRAP(temp_a1);
        objData->roll += ((temp_a1 >> 2) / 3) * (s32)updateRate;
    }
    
    objData->yaw -= (s16) (objData->steering.xJoy * (70.0f - (objData->steering.yJoy * 0.05f)) * 0.0666f);
}

// offset: 0x3618 | func: 25
void CRSnowBike_func_3618(Object* self, DLL732_Unk_2E0* arg1, u8 controllerPort, s32 buffer) {
    arg1->xJoy = joy_get_stick_x_buffered(controllerPort, buffer);
    arg1->yJoy = joy_get_stick_y_buffered(controllerPort, buffer);
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
void CRSnowBike_func_3748(Object* self, CRSnowBike_Data* objData) {
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
    case Damage_Type_14:
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
    DLL732_Unk_2E0* unkSubstruct;
    CRSnowBike_Motion* bikeMotion;
    Vec3f wsVelocity;
    SRT transform;
    s32 i;
    MtxF mtx;

    objData = self->data;
    unkSubstruct = &objData->steering;

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
    
    if (objData->state == STATE_2) {
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
            bikeMotion = &objData->unk2AC;
            matrix_from_srt_reversed(&mtx, &transform);
            
            //Transform velocity from worldSpace to the bike's own objectSpace
            vec3_transform(&mtx, 
                wsVelocity.x, wsVelocity.y, wsVelocity.z, 
                &bikeMotion->velocity.x, &bikeMotion->velocity.y, &bikeMotion->velocity.z
            );
        }

        unkSubstruct->yJoy += gUpdateRate * 8;
        if (unkSubstruct->yJoy > 70) {
            unkSubstruct->yJoy = 70;
        }

        CRSnowBike_handle_engine_sfx_and_modgfx(self, objData, 
            bikeMotion->velocity.z, unkSubstruct->yJoy, &unkSubstruct->unk10, 
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
void CRSnowBike_func_3AF8(Object* self, CRSnowBike_Data* objData, DLL27_Data* collision) {
    s32 pad;
    MtxF bikeMtx;
    Vec3f wsPoint[7];
    s8 flags;
    s8 sp86;
    s32 i;

    sp86 = -0x80 >> (7 - objData->numCollisionPoints);
    sp86 = ~sp86;
    objData->unk3F3 = -1;
    objData->forwardSpeed = objData->unk2AC.velocity.z;
    
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
            
            if (objData->unk3F3 == -1) {
                objData->unk3F3 = flags;
            }
        } while (flags);
    }
    
    bcopy(&wsPoint, objData->wsCollisionCoords, objData->numCollisionPoints * sizeof(Vec3f));
}

// offset: 0x3DAC | func: 30
s32 CRSnowBike_sharpclaw_do_steering(Object* self, CRSnowBike_Data* arg1, CRSnowBike_Data* objData, DLL732_Unk_2E0* steering) {
    f32 dx;
    f32 dz;
    s32 checkpointsReturn;
    f32 speed;
    s32 dYaw;
    s32 value;
    CRSnowBike_Setup* objSetup;

    objSetup = (CRSnowBike_Setup*)self->setup;
    
    dx = self->srt.transl.x - objData->unk0.transl.x;
    dz = self->srt.transl.z - objData->unk0.transl.z;
    dx = sqrtf(SQ(dx) + SQ(dz));
    
    speed = 100.0f - dx;
    if (dx > 100.0f) {
        speed = 0.0f;
    }
    
    checkpointsReturn = CRSnowBike_func_0(self, objData, speed);
    
    gDLL_4_Race->vtbl->func4(self, &objData->raceData);
    gDLL_4_Race->vtbl->func10(&objData->raceData);

    if (checkpointsReturn != 0) {
        steering->xJoy = 0;
        steering->yJoy = 0;
        return 1;
    }

    dx = self->srt.transl.x - objData->unk0.transl.x;
    dz = self->srt.transl.z - objData->unk0.transl.z;
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
void CRSnowBike_func_4B30(s32 arg0, s32 arg1, s32 arg2) {

}
