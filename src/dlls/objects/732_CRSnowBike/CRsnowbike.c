#include "common.h"
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
    Vec3f unkC;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} DLL732_Data2AC;

typedef enum {
    STATE_0,
    STATE_1,
    STATE_2,
    STATE_3
} CRSnowBike_States;

typedef enum {
    CRSnowBike_FLAG_0 = 0,
    CRSnowBike_FLAG_1 = 1,
    CRSnowBike_FLAG_2 = 2,
    CRSnowBike_FLAG_4_Grounded = 4,
    CRSnowBike_FLAG_8_Race_Started = 8,
    CRSnowBike_FLAG_10_Was_In_Sequence = 0x10,
    CRSnowBike_FLAG_20_SharpClaw_Bike = 0x20
} CRSnowBike_Flags;

typedef enum {
    RACETRACK_0_CloudRunner_Fortress,
    RACETRACK_1_Golden_Plains
} CRSnowBike_Racetracks;

typedef struct {
    ObjSetup base;
    u8 yaw;
    u8 unk19;
    s16 gamebitUnlocked;
    u8 racetrackIdx;
    u8 unk1D;
    s16 gamebitA;
    u8 unk20;
} CRSnowBike_Setup;

typedef struct {
    SRT unk0;
    RaceStruct raceData;
    s8 _unk3C[0x48 - 0x3C];
    u8 racetrackIdx;                //See `DLL732_Racetracks`
    u8 unk49;
    DLL27_Data collision;
    DLL732_Data2AC unk2AC;
    DLL732_Unk_2E0 unk2E0;
    DLL_IModgfx* unk2F4;
    DLL_IModgfx* unk2F8;
    s16* gamebitIDs;
    s8 _unk300[0x330 - 0x300];
    Vec3f unk330[6];
    s8 _unk378[0x384 - 0x378];
    f32 unk384;
    Vec3f unk388;
    f32 unk394;
    f32 unk398;
    f32 unk39C;
    Vec3f unk3A0; //previous position?
    Vec3f unk3AC;
    u32 soundHandle1;
    u32 soundHandle2;
    u32 soundHandle3;
    u32 soundHandle4;
    s32 fuelAmount;
    Vec3f unk3CC;
    s8 _unk3D8[0x3DC - 0x3D8];
    s16 yawOffset;
    s16 rollOffset;
    s16 yaw;
    s16 pitch;
    s16 roll;
    s16 unk3E6;
    s8 _unk3E8[0x3EA - 0x3E8];
    s16 unk3EA;
    s8 unk3EC;
    u8 unk3ED;
    u8 unk3EE;
    u8 flags;
    s8 state;
    u8 unk3F1;
    s8 unk3F2;
    s8 unk3F3;
    u8 unk3F4_0 : 1;
} CRSnowBike_Data;

/*0x0*/ static s16 dRaceGamebits[][3] = {
    { BIT_CF_Race_Started, BIT_498, BIT_49C },
    { BIT_GP_Sharpclaw_Jetbike_Cutscene2, BIT_77E, BIT_77F }
};
/*0xC*/ static u32 data_C[][3] = {
    {0x00030c60, 0x00030c60, 0x00030c60}, 
    {0x00034dd3, 0x00034dc7, 0x00034dc9}
};
/*0x24*/ static Vec3f data_24[] = {
    VEC3F(-6.5, 0, -7),
    VEC3F(6.5, 0, -7),
    VEC3F(6.5, 0, 7),
    VEC3F(-6.5, 0, 7)
};
/*0x54*/ static f32 data_54[] = {
    0.0f, 0.0f, 0.0f, 0.0f
};
/*0x64*/ static Vec3f data_64[] = {
    VEC3F(0, 1, -18), 
    VEC3F(-6.5, 1, -7), 
    VEC3F(6.5, 1, -7), 
    VEC3F(-6, 1, 11), 
    VEC3F(6, 1, 11)
};
/*0xA0*/ static f32 data_A0[] = {
    5, 4, 4, 4.5, 4.5, -5, 0, 0, 
    5, 0, 0, 0, 2.0001223, 0, 0, 0, 
    2.0157478, 0, 0, 0
};
/*0xF0*/ static Vec3f data_F0[] = {
    VEC3F(14.5, 0, 9), 
    VEC3F(-14.5, 0, 9)
};

/*0x0*/ static Texture* sTexVerticalWave;       //(TEX0_486) 1 vertical wave? 
/*0x4*/ static Texture* sTexFlames;             //(TEX0_253) thruster flames 
/*0x8*/ static Texture* sTexHorizontalWaves;    //(TEX0_136) 3 horizontal waves?
/*0x10*/ static SRT bss_10;

#include "prevent_bss_reordering.h"

static s32 CRSnowBike_func_0(Object* self, CRSnowBike_Data* objData, f32 arg2);
void CRSnowBike_func_1C38(Object* self);
static void CRSnowBike_func_1DC8(Object* self, CRSnowBike_Data* objData, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols);
static void CRSnowBike_func_2340(Object* self, CRSnowBike_Data* objData, DLL732_Data2AC* arg2, f32 updateRate, s32 arg4);
static void CRSnowBike_func_22BC(Object* self, DLL732_Data2AC* arg1);
static void CRSnowBike_func_2E64(Object* self, CRSnowBike_Data* objData, DLL732_Data2AC* arg2, f32 updateRate, s32 arg4);
static void CRSnowBike_func_3618(Object* self, DLL732_Unk_2E0* arg1, u8 controllerPort, s32 buffer);
static void CRSnowBike_get_bike_matrix(Object* self, CRSnowBike_Data* objData, MtxF* oMtx, s32 useYawOffset, s32 useRoll, s32 usePitch);
static void CRSnowBike_func_3748(Object* self, CRSnowBike_Data* objData);
static int CRSnowBike_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void CRSnowBike_func_3AF8(Object* self, CRSnowBike_Data* objData, DLL27_Data* collision);
static s32 CRSnowBike_func_3DAC(Object* self, CRSnowBike_Data* arg1, CRSnowBike_Data* objData, DLL732_Unk_2E0* arg3);
static void CRSnowBike_func_3FE0(Object* self, CRSnowBike_Data* objData);
static void CRSnowBike_func_40FC(Object* self, CRSnowBike_Data* objData, f32 arg2, s32 arg3, s8* arg6, u8 arg5);

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
    u8 data_108[] = {5, 5, 5, 5};
    CRSnowBike_Data* objData;

    objSetup = (CRSnowBike_Setup*)self->setup;
    objData = self->data;
    bzero(objData, sizeof(CRSnowBike_Data));
    
    objData->yaw = (setup->yaw & 0xFF) << 8;
    objData->unk2F4 = 0;
    objData->unk2F8 = 0;
    self->srt.yaw = objData->yaw;

    self->animCallback = CRSnowBike_anim_callback;
    
    gDLL_27->vtbl->init(&objData->collision, 0, 0x01040007, 1);
    gDLL_27->vtbl->setup_terrain_collider(&objData->collision, 4, data_24, data_54, data_108);
    
    if (setup->unk19 != 0) {
        objData->unk3EE = 3;
    } else {
        objData->unk3EE = 5;
    }

    CRSnowBike_func_3FE0(self, objData);
    CRSnowBike_func_22BC(self, &objData->unk2AC);

    func_80023D30(self, 0, 0.0f, 0);

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_4000 | OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }

    if (setup->unk19 != 0) {
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

    sTexVerticalWave = tex_load_deferred(TEXTABLE_186);     //(TEX0_486) 1 vertical wave? 
    sTexFlames = tex_load_deferred(TEXTABLE_89);            //(TEX0_253) thruster flames 
    sTexHorizontalWaves = tex_load_deferred(TEXTABLE_3C);   //(TEX0_136) 3 horizontal waves?

    if (main_get_bits(objSetup->gamebitA)) {
        flagsValue = CRSnowBike_FLAG_1;
    } else {
        flagsValue = CRSnowBike_FLAG_0;
    }
    objData->flags |= flagsValue;
    
    objData->gamebitIDs = &dRaceGamebits[setup->racetrackIdx][0];
}

/*0x0*/ static const char str_0[] = " FInished Is SEt for Some Reason \n";
/*0x24*/ static const char str_24[] = " FInished Is SEt for Some Reason \n";

// offset: 0x398 | func: 2 | export: 1
void CRSnowBike_control(Object* self) {
    CRSnowBike_Setup* objSetup;
    Object* player;
    s32 pad;
    f32 yOffset;
    MtxF bikeMtx;
    Vec3f wsPoint;
    DLL732_Data2AC* innerData;
    CRSnowBike_Data* objData;
    DLL732_Unk_2E0* new_var;
    f32 dx;
    f32 dz;
    s32 sp5C;
    f32 var_fv1;
    s32 updateIdx;
    s32 flagValue;
    DLL732_Data2AC* sp44;

    objData = self->data;
    objSetup = (CRSnowBike_Setup*)self->setup;
    
    innerData = &objData->unk2AC;
    new_var = &objData->unk2E0;
    
    yOffset = 0.0f;
    
    if ((objData->flags & CRSnowBike_FLAG_1)) {
        return;
    }
        
    if (main_get_bits(objSetup->gamebitA)) {
        objData->flags |= CRSnowBike_FLAG_1;
        return;
    }
    
    player = get_player();

    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    objData->unk3A0.f[0] = self->srt.transl.f[0];
    objData->unk3A0.f[1] = self->srt.transl.f[1];
    objData->unk3A0.f[2] = self->srt.transl.f[2];
    
    if (objData->flags & CRSnowBike_FLAG_10_Was_In_Sequence) {
        objData->flags &= ~CRSnowBike_FLAG_10_Was_In_Sequence;

        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) == FALSE) {
            innerData->unkC.f[0] = 0.0f;
            innerData->unkC.f[1] = 0.0f;
            innerData->unkC.f[2] = -2.0f;
            objData->yawOffset = 0;
            objData->rollOffset = 0;
            objData->unk3F2 = 0;
            objData->unk3E6 = 0;
            objData->yaw = self->srt.yaw;
            objData->pitch = self->srt.pitch;
            objData->roll = self->srt.roll;
            CRSnowBike_func_3FE0(self, objData);
        }
    }

    switch (objData->state) {
    case STATE_0:
        if ((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) == FALSE) {
            self->objhitInfo->unk5B = 0;
            self->objhitInfo->unk5C = 0;
            objData->unk3ED = 0;
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

            if ((objSetup->gamebitUnlocked == NO_GAMEBIT) || main_get_bits(objSetup->gamebitUnlocked)) {
                self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
            } else {
                self->unkAF |= ARROW_FLAG_10_Greyed_Out;
            }
            
            //Check if the player is near the bike
            if ((player != NULL) && (vec3_distance(&player->globalPosition, &self->globalPosition) < 100.0f)) {
                objData->unk3ED = 1;
                CRSnowBike_get_bike_matrix(self, objData, &bikeMtx, 0, 1, 1);
                vec3_transform(&bikeMtx, data_F0[0].x, data_F0[0].y, data_F0[0].z, &wsPoint.z, &wsPoint.y, &wsPoint.x);
                dx = player->srt.transl.x - wsPoint.z;
                dz = player->srt.transl.z - wsPoint.x;

                if ((SQ(dx) + SQ(dz)) < SQ(10)) {
                    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                    objData->unk3ED = 2;
                } else {
                    vec3_transform(&bikeMtx, data_F0[1].x, data_F0[1].y, data_F0[1].z, &wsPoint.z, &wsPoint.y, &wsPoint.x);
                    dx = player->srt.transl.x - wsPoint.z;
                    dz = player->srt.transl.z - wsPoint.x;
                    if ((SQ(dx) + SQ(dz)) < SQ(10)) {
                        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                        objData->unk3ED = 1;
                    }
                }
            }
        }

        if (objData->soundHandle2) {
            gDLL_6_AMSFX->vtbl->stop(objData->soundHandle2);
            objData->soundHandle2 = 0;
        }

        if (objData->soundHandle3) {
            gDLL_6_AMSFX->vtbl->stop(objData->soundHandle3);
            objData->soundHandle3 = 0;
        }

        if (objData->soundHandle4) {
            gDLL_6_AMSFX->vtbl->stop(objData->soundHandle4);
            objData->soundHandle4 = 0;
        }

        if (objData->unk2F4 != NULL) {
            dll_unload(objData->unk2F4);
            objData->unk2F4 = NULL;
        }

        if (objData->unk2F8 != NULL) {
            dll_unload(objData->unk2F8);
            objData->unk2F8 = NULL;
        }

        objData->flags &= ~CRSnowBike_FLAG_8_Race_Started;
        break;
    case STATE_2:
        if (!(objData->flags & CRSnowBike_FLAG_8_Race_Started)) {
            objData->raceData.unk10 = -1;
            objData->raceData.unk14 = -1;
            objData->raceData.unk18 = -1;
            objData->raceData.unk1C = 0;

            if (main_get_bits(objData->gamebitIDs[0])) {
                flagValue = CRSnowBike_FLAG_8_Race_Started;
            } else {
                flagValue = CRSnowBike_FLAG_0;
            }
            objData->flags |= flagValue;

            if (objData->flags & CRSnowBike_FLAG_8_Race_Started) {
                if (objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) {
                    CRSnowBike_func_1C38(self);
                } else {
                    gDLL_4_Race->vtbl->func3(self, &objData->raceData, objSetup->racetrackIdx);
                }
                gDLL_4_Race->vtbl->func9(&objData->raceData);
            }
        } else if (main_get_bits(objData->gamebitIDs[1])) {
            objData->flags &= ~CRSnowBike_FLAG_8_Race_Started;
        }

        CRSnowBike_func_3748(self, objData);

        if (objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) {
            if (!(objData->flags & CRSnowBike_FLAG_8_Race_Started)) {
                return;
            }
            
            if (map_world_coords_to_block_index(self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2]) >= 0) {
                if (objData->flags & CRSnowBike_FLAG_2) {
                    sp5C = CRSnowBike_func_0(self, objData, 2.8f * gUpdateRateF);
                    gDLL_4_Race->vtbl->func4(self, &objData->raceData);
                    gDLL_4_Race->vtbl->func10(&objData->raceData);
                    if (sp5C == 0) {
                        sp44 = &objData->unk2AC;
                        self->srt.yaw = arctan2_f(self->srt.transl.x - objData->unk0.transl.x, self->srt.transl.f[2] - objData->unk0.transl.f[2]);
                        self->srt.transl.x = objData->unk0.transl.x;
                        self->srt.transl.y = objData->unk0.transl.y;
                        self->srt.transl.z = objData->unk0.transl.z;
                        sp44->unkC.f[0] = 0.0f;
                        sp44->unkC.f[1] = 0.0f;
                        sp44->unkC.f[2] = -2.0f;
                        objData->yawOffset = 0;
                        objData->rollOffset = 0;
                        objData->unk3F2 = 0;
                        objData->unk3E6 = 0;
                        objData->yaw = self->srt.yaw;
                        objData->pitch = self->srt.pitch;
                        objData->roll = self->srt.roll;
                        CRSnowBike_func_3FE0(self, objData);
                        func_80058680(self, self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2], &yOffset, 0);
                        self->srt.transl.y -= yOffset;
                        objData->flags &= ~CRSnowBike_FLAG_2;
                    }
                    return;
                } else if (CRSnowBike_func_3DAC(self, objData, objData, &objData->unk2E0)) {
                    return;
                }
            } else {
                sp5C = CRSnowBike_func_0(self, objData, 2.8f * gUpdateRateF);
                gDLL_4_Race->vtbl->func4(self, &objData->raceData);
                gDLL_4_Race->vtbl->func10(&objData->raceData);
                if (sp5C == 0) {
                    self->srt.yaw = arctan2_f(self->srt.transl.x - objData->unk0.transl.x, self->srt.transl.f[2] - objData->unk0.transl.f[2]);
                    self->srt.transl.x = objData->unk0.transl.x;
                    self->srt.transl.y = objData->unk0.transl.y;
                    self->srt.transl.z = objData->unk0.transl.z;
                    objData->flags |= CRSnowBike_FLAG_2;
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
                CRSnowBike_func_3618(self, &objData->unk2E0, 0, updateIdx);
                CRSnowBike_func_2340(self, objData, &objData->unk2AC, gUpdateRateF, (updateIdx + 1) == gUpdateRate);
                objData->yawOffset += (s16) ((-objData->unk2E0.xJoy * 60.0f) - objData->yawOffset) >> 4;
                objData->rollOffset += (s16) ((-objData->unk2E0.xJoy * 105.0f) - objData->rollOffset) >> 4;
                self->srt.yaw = objData->yaw + objData->yawOffset;
                self->srt.roll = objData->roll + objData->rollOffset;
            }
            
            sp44 = &objData->unk2AC;
            if (objData->fuelAmount >= 0) {
                objData->fuelAmount = objData->fuelAmount - (s32) (VECTOR_MAGNITUDE(sp44->unkC) * gUpdateRateF * 1.5f) - gUpdateRate;
                diPrintf(" FUEL AMT %i \n", objData->fuelAmount / 10);
                gDLL_1_cmdmenu->vtbl->energy_bar_set(objData->fuelAmount);
            } else if (objData->unk3CC.f[0] > 0.1f) {
                diPrintf(" \tRAN OUT OF FUEL \t");
                
                if (rand_next(0, 10) == 0) {
                    gDLL_6_AMSFX->vtbl->play(self, SOUND_B38, MAX_VOLUME, NULL, NULL, 0, NULL);
                }

                objData->unk3CC.f[0] *= 0.95f;
                objData->unk3CC.f[1] *= 0.95f;
                objData->unk3CC.f[2] *= 0.95f;

                if (objData->unk3CC.f[0] < 0.1f) {
                    gDLL_1_cmdmenu->vtbl->energy_bar_free();
                    gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                    objData->unk3CC.f[0] = 0.01f;
                    objData->unk3CC.f[1] = 0.01f;
                    objData->unk3CC.f[2] = 0.01f;
                }
            }
        } else {
            for (updateIdx = 0; updateIdx < gUpdateRate; updateIdx++) {
                CRSnowBike_func_2E64(self, objData, &objData->unk2AC, gUpdateRateF, (updateIdx + 1) == gUpdateRate);
                objData->yawOffset += (s16) ((-objData->unk2E0.xJoy * 60.0f) - objData->yawOffset) >> 4;
                objData->rollOffset += (s16) ((-objData->unk2E0.xJoy * 105.0f) - objData->rollOffset) >> 4;
                self->srt.yaw = objData->yaw + objData->yawOffset;
                self->srt.roll = objData->roll + objData->rollOffset;
            }
        }
        
        if (!(objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike)) {
            CRSnowBike_func_40FC(self, objData, objData->unk2AC.unkC.f[2], new_var->yJoy, &new_var->unk10, 7);
        } else {
            if (objData->soundHandle1) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandle1);
                objData->soundHandle1 = 0;
            }
            if (objData->soundHandle2) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandle2);
                objData->soundHandle2 = 0;
            }
            if (objData->soundHandle3) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandle3);
                objData->soundHandle3 = 0;
            }
            if (objData->soundHandle4) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandle4);
                objData->soundHandle4 = 0;
            }
        }
        break;
    case STATE_1:
        break;
    }

    if (((objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike) == 0) && (objData->state == STATE_2)) {
        if (objData->unk39C >= 0.0f) {
            var_fv1 = objData->unk39C;
        } else {
            var_fv1 = -objData->unk39C;
        }
        if (var_fv1 > 2.0f) {
            self->objhitInfo->unk5F = 0x14;
            self->objhitInfo->unk60 = 1;
            self->objhitInfo->unk58 |= 1;
            self->objhitInfo->unk40 = 0x10;
            self->objhitInfo->unk58 |= 4;
            self->srt.flags &= 0xBFFF;
        }
    }
}

// offset: 0x123C | func: 3 | export: 2
void CRSnowBike_update(Object* self) {
    CRSnowBike_Data* objData;
    ObjectHitInfo* objHitInfo;
    Vec3f spDC;
    MtxF sp9C;
    MtxF sp5C;
    DLL732_Data2AC* temp_v0;
    Object* obj;
    s32 objID;
    s32 i;

    objHitInfo = self->objhitInfo;
    objData = self->data;
    temp_v0 = &objData->unk2AC;

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
            
            objData->unk3EC = 5;
            objData->unk384 = 0.2f;
            break;
        }
    }
    
    if (!(objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike)) {
        bss_10.yaw = -objData->yaw;
        bss_10.pitch = -objData->pitch;
        bss_10.roll = -objData->roll;
        matrix_from_srt_reversed(&sp9C, &bss_10);
        self->velocity.f[0] = (self->srt.transl.x - self->prevLocalPosition.x) * gUpdateRateInverseF;
        self->velocity.f[1] = (self->srt.transl.y - self->prevLocalPosition.y) * gUpdateRateInverseF;
        self->velocity.f[2] = (self->srt.transl.z - self->prevLocalPosition.z) * gUpdateRateInverseF;
        spDC.x = self->velocity.f[0] * 0.93749994f;
        spDC.y = self->velocity.f[1] * 0.93749994f;
        spDC.z = self->velocity.f[2] * 0.93749994f;
        
        vec3_transform(&sp9C, spDC.f[0], spDC.f[1], spDC.f[2], 
                       &temp_v0->unkC.f[0], &temp_v0->unkC.f[1], &temp_v0->unkC.f[2]);
        CRSnowBike_get_bike_matrix(self, objData, &sp5C, 0, 0, 0);
        vec3_transform(&sp5C, 0.0f, 0.0f, -10.0f, &objData->unk3AC.x, &objData->unk3AC.y, &objData->unk3AC.z);
    }
}

// offset: 0x1534 | func: 4 | export: 3
void CRSnowBike_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CRSnowBike_Data* objData;
    s32 sp38;

    objData = self->data;

    if (objData->flags & CRSnowBike_FLAG_1) {
        return;
    }
    
    if (visibility == -1) {
        sp38 = gDLL_13_Expgfx->vtbl->func10(self);
        if (!(objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike)) {
            CRSnowBike_func_1DC8(self, objData, gdl, mtxs, vtxs, pols);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 0, &objData->unk388.x, &objData->unk388.y, &objData->unk388.z, 0);
        if (sp38 != 0) {
            gDLL_13_Expgfx->vtbl->func6(self, gdl, mtxs, NULL, 0, 0, 0);
        }
        return;
    } else if (objData->state == STATE_2) {
        objData->unk388.x = self->srt.transl.x;
        objData->unk388.y = self->srt.transl.y;
        objData->unk388.z = self->srt.transl.z;
        return;
    }
    
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 0, &objData->unk388.x, &objData->unk388.y, &objData->unk388.z, 0);
    }
}

// offset: 0x1724 | func: 5 | export: 4
void CRSnowBike_free(Object* self, s32 onlySelf) {
    CRSnowBike_Data* objData;

    objData = self->data;

    if (objData->soundHandle1) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandle1);
        objData->soundHandle1 = 0;
    }

    if (objData->soundHandle2) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandle2);
        objData->soundHandle2 = 0;
    }

    if (objData->soundHandle3) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandle3);
        objData->soundHandle3 = 0;
    }

    if (objData->soundHandle4) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandle4);
        objData->soundHandle4 = 0;
    }

    if (objData->unk2F4 != NULL) {
        dll_unload(objData->unk2F4);
    }
    objData->unk2F4 = NULL;
    
    if (objData->unk2F8 != NULL) {
        dll_unload(objData->unk2F8);
    }
    objData->unk2F8 = NULL;
    
    obj_free_object_type(self, OBJTYPE_Vehicle);
    
    if (!(objData->flags & CRSnowBike_FLAG_20_SharpClaw_Bike)) {
        gDLL_1_cmdmenu->vtbl->energy_bar_free();
    }
    
    tex_free(sTexVerticalWave);
    tex_free(sTexFlames);
    tex_free(sTexHorizontalWaves);
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
u8 CRSnowBike_func_18FC(Object* self, s32 arg1) {
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

    STUBBED_PRINTF(" Bike Can Mount %i ", objData->unk3ED);
    
    return objData->unk3ED;
}

// offset: 0x1980 | func: 9 | export: 8
u8 CRSnowBike_func_1980(Object* self) {
    CRSnowBike_Data* objData = self->data;
    return objData->unk3ED;
}

static const char str_extra2[] = "tracks %d\n";
static const char str_extra3[] = "ident %d\n";

// offset: 0x1990 | func: 10 | export: 9
void CRSnowBike_func_1990(Object* self, f32* ox, f32* oy, f32* oz) {
    CRSnowBike_Data* objData = self->data;
    
    *ox = objData->unk388.x;
    *oy = objData->unk388.y;
    *oz = objData->unk388.z;
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
        objData->unk3CC.f[0] = 2.0f;
        objData->unk3CC.f[1] = 4.0f;
        objData->unk3CC.f[2] = 4.6f;
        gDLL_1_cmdmenu->vtbl->energy_bar_create(0, 13000, TEXTABLE_569, TEXTABLE_56A, 1);
    }
}

// offset: 0x1B10 | func: 16 | export: 15
void CRSnowBike_func_1B10(Object* self, f32* arg1, s32* arg2) {
    CRSnowBike_Data* objData = self->data;
    
    *arg1 = objData->yawOffset / 2500.0f;
    
    if (*arg1 > 1.0f) {
        *arg1 = 1.0f;
    } else if (*arg1 < -1.0f) {
        *arg1 = -1.0f;
    }
    
    *arg2 = objData->yawOffset < 0;
}

// offset: 0x1B9C | func: 17 | export: 16
f32 CRSnowBike_func_1B9C(Object* self, f32* arg1) {
    CRSnowBike_Data* objData;
    f32 magnitude;

    objData = self->data;
    
    *arg1 = 5.0f;

    magnitude = VECTOR_MAGNITUDE(objData->unk2AC.unkC) * 0.2f;
    if (magnitude > 1.0f) {
        magnitude = 1.0f;
    }
    return magnitude;
}

// offset: 0x1C28 | func: 18 | export: 17
s8 CRSnowBike_func_1C28(Object* self) {
    CRSnowBike_Data* objData = self->data;
    return objData->unk3F1;
}

// offset: 0x1C38 | func: 19 | export: 18
void CRSnowBike_func_1C38(Object* self) {
    s32 checkpointUID;
    checkpoint4_Setup* checkpointSetup;
    CRSnowBike_Data* objData;
    CRSnowBike_Setup* objSetup;

    objSetup = (CRSnowBike_Setup*)self->setup;
    objData = self->data;
    
    switch (self->id) {
    default:
    case OBJ_CRSnowClawBike:
        checkpointUID = data_C[objSetup->racetrackIdx][0];
        break;
    case OBJ_CRSnowClawBike2:
        checkpointUID = data_C[objSetup->racetrackIdx][1];
        break;
    case OBJ_CRSnowClawBike3:
        checkpointUID = data_C[objSetup->racetrackIdx][2];
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

    CRSnowBike_func_3FE0(self, objData);
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
    DLL732_Data2AC* temp_v1;
    u32 dColourRGBA[] = { 6, 105, 105, 255}; //Unused RGBA colour, maybe for randomised spark colours?
    s32 volume;
    s32 pad3;

    temp_v1 = &objData->unk2AC;

    gfx = *gdl;
    vertex = *vtxs;
    tri = *pols;
    
    translateX = 0.0f;
    if (temp_v1->unkC.z < 0.0f) {
        translateX = temp_v1->unkC.z;
    }
    fxTransform.transl.z = translateX;

    if (temp_v1->unkC.z < 0.0f) {
        translateX = temp_v1->unkC.x;
    }
    fxTransform.transl.x = translateX;
    
    dl_set_prim_color(&gfx, 0xFF, 0xFF, 0xFF, 0xFF);
    
    if (temp_v1->unkC.z < -0.5f) {
        if (1) { }
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_12E, &fxTransform, 4, -1, NULL);
    }
    if (temp_v1->unkC.z < -1.5f) {
        if (1) { }
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_12F, &fxTransform, 4, -1, NULL);
    }
    if (temp_v1->unkC.z < -2.1f) {
        if (1) { }
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_130, &fxTransform, 4, -1, NULL);
    }
    
    i = 0;
    if (objData->unk2E0.yJoy > 0) {
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
    if (objData->unk39C < -1.2f) {
        dColourRGBA[1] += rand_next(0, 155);
        dColourRGBA[2] += rand_next(0, 155);
        volume = ((0.0f - objData->unk39C) * 21.0f);
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
void CRSnowBike_func_22BC(Object* self, DLL732_Data2AC* arg1) {
    arg1->unk1C = 90;
    arg1->unk20 = 1/arg1->unk1C;
    arg1->unk28 = -11.0f;
    arg1->unk2C = 0.1f;
    arg1->unk30 = 13.0f;
    arg1->unk18 = (1/arg1->unk1C) * 0.01666666f;
    
    bss_10.transl.x = 0;
    bss_10.transl.y = 0;
    bss_10.transl.z = 0;
    bss_10.scale = 1;
}

// offset: 0x2340 | func: 23
void CRSnowBike_func_2340(Object* self, CRSnowBike_Data* objData, DLL732_Data2AC* arg2, f32 updateRate, s32 arg4) {
    MtxF sp130;
    MtxF spF0;
    MtxF spB0;
    s32 pad1;
    f32 temp3;
    f32 temp2;
    f32 temp_fv0;
    f32 temp_fv1_7;
    f32 temp_fa0_4;
    f32 sp94;
    f32 var_fv1;
    Vec3f sp84;
    f32 var_fv0_4;
    f32 var_fa0;
    s32 var_a0;
    Vec3f sp6C;
    Vec3f sp60;
    s32 pad2;
    DLL27_Data* collision;
    s32 angle1;
    s32 angle2;
    s32 i;

    collision = &objData->collision;
    
    if (objData->unk3EC != 0) {
        VECTOR_SCALE(arg2->unkC, objData->unk384);
        objData->unk3EC--;
        if (objData->unk3EC < 0) {
            objData->unk3EC = 0;
        }
    }

    temp3 = -objData->unk3CC.f[0];
    arg2->unkC.f[0] = (arg2->unkC.x < temp3) ? temp3 : (
                      (objData->unk3CC.f[0] < arg2->unkC.x) ? objData->unk3CC.f[0] : arg2->unkC.x);
    
    temp3 = -objData->unk3CC.f[1];
    arg2->unkC.f[1] = (arg2->unkC.f[1] < temp3) ? temp3 : (
                      (objData->unk3CC.f[1] < arg2->unkC.f[1]) ? objData->unk3CC.f[1] : arg2->unkC.f[1]);

    temp3 = -objData->unk3CC.f[2];
    arg2->unkC.f[2] = (arg2->unkC.f[2] < temp3) ? temp3 : (
                      (objData->unk3CC.f[2] < arg2->unkC.f[2]) ? objData->unk3CC.f[2] : arg2->unkC.f[2]);
    
    bss_10.yaw = objData->yaw;
    bss_10.pitch = objData->pitch;
    bss_10.roll = objData->roll;
    matrix_from_srt(&sp130, &bss_10);
    bss_10.yaw = -objData->yaw;
    bss_10.pitch = -objData->pitch;
    bss_10.roll = -objData->roll;
    matrix_from_srt_reversed(&spF0, &bss_10);
    
    if ((objData->flags & CRSnowBike_FLAG_4_Grounded) == FALSE) {
        var_fv0_4 = (f32) -objData->unk2E0.yJoy / 60.0f;
        if (var_fv0_4 > 1.0f) {
            var_fv0_4 = 1.0f;
        } else if (var_fv0_4 < -1.0f) {
            var_fv0_4 = -1.0f;
        }
        var_fv0_4 *= 6144.0f;
        objData->unk3E6 += ((s32) (var_fv0_4) - objData->unk3E6) >> 5;
    } else if (objData->unk3E6 != 0) {
        objData->unk3E6 -= (-objData->unk3E6 >> 5);
    }
    
    vec3_transform(&spF0, 0.0f, arg2->unk28 * arg2->unk1C, 0.0f, &sp84.f[0], &sp84.f[1], &sp84.f[2]);

    if (objData->unk2E0.yJoy < 0) {
        var_fv1 = -(f32) objData->unk2E0.yJoy * 4.0f;
    } else {
        var_fv1 = -(f32) objData->unk2E0.yJoy * 10.0f;
    }
    
    temp_fv0 = arg2->unk20 * var_fv1;
    if (temp_fv0 < 0.0f) {
        arg2->unkC.f[2] += (temp_fv0 * 0.01666666f);
    } else {
        if (arg2->unkC.f[2] <= 0.0f) {
            arg2->unkC.f[2] += (temp_fv0 * 0.01666666f);
            if (arg2->unkC.f[2] > 0.0f) {
                arg2->unkC.f[2] = 0.0f;
            }
        }
    }
    
    arg2->unk0.x = sp84.f[0] * arg2->unk18;
    arg2->unk0.y = sp84.f[1] * arg2->unk18;
    arg2->unk0.z = sp84.f[2] * arg2->unk18;
    arg2->unkC.x = arg2->unk0.x + arg2->unkC.x;
    arg2->unkC.y = arg2->unk0.y + arg2->unkC.y;
    arg2->unkC.z = arg2->unk0.z + arg2->unkC.z;
    
    if (collision->unk25D != 0) {
        temp_fv0 = arg2->unk2C * sp84.y;
        if (arg2->unkC.f[2] < 0.0f) {
            if (temp_fv0 < 0.0f) {
                temp_fv0 = -temp_fv0;
            }
        } else if (temp_fv0 > 0.0f) {
            temp_fv0 = -temp_fv0;
        }
        temp_fv0 *= arg2->unk18;
        var_fa0 = arg2->unkC.f[2] + temp_fv0;
        if (arg2->unkC.f[2] < 0.0f) {
            if (var_fa0 > 0.0f) {
                arg2->unkC.f[2] = 0.0f;
            } else {
                arg2->unkC.f[2] = var_fa0;
            }
        } else if (var_fa0 < 0.0f) {
            arg2->unkC.f[2] = 0.0f;
        } else {
            arg2->unkC.f[2] = var_fa0;
        }
        
        if (arg2->unkC.f[2] < 0.0f) {
            var_fa0 = -arg2->unkC.f[2];
        } else {
            var_fa0 = arg2->unkC.f[2];
        }
        
        temp_fv0 = arg2->unk2C * sp84.y * (4.0f + SQ(var_fa0));
        if (arg2->unkC.f[0] < 0.0f) {
            if (temp_fv0 < 0.0f) {
                temp_fv0 = -temp_fv0;
            }
        } else if (temp_fv0 > 0.0f) {
            temp_fv0 = -temp_fv0;
        }
        temp_fv0 *= arg2->unk18;
        
        temp_fv1_7 = arg2->unkC.f[0] + temp_fv0;
        if (arg2->unkC.f[0] < 0.0f) {
            if (temp_fv1_7 > 0.0f) {
                arg2->unkC.f[0] = 0.0f;
            } else {
                arg2->unkC.f[0] = temp_fv1_7;
            }
        } else if (temp_fv1_7 < 0.0f) {
            arg2->unkC.f[0] = 0.0f;
        } else {
            arg2->unkC.f[0] = temp_fv1_7;
        }
        
        objData->unk3F2 = 0;
        objData->unk3E6 = 0;
    } else {
        objData->unk3F2++;
        if (objData->unk3F2 > 0x64) {
            objData->unk3F2 = 0x64;
        }
    }

    temp2 = SQ(arg2->unkC.f[2]);
    temp_fv0 = arg2->unk30 * temp2;
    if (arg2->unkC.f[2] > 0.0f) {
        temp_fv0 = -temp_fv0;
    }
    temp_fv0 *= arg2->unk18;
    temp_fa0_4 = arg2->unkC.f[2] + temp_fv0;
    if (arg2->unkC.f[2] < 0.0f) {
        if (temp_fa0_4 > 0.0f) {
            arg2->unkC.f[2] = 0.0f;
        } else {
            arg2->unkC.f[2] = temp_fa0_4;
        }
    } else if (temp_fa0_4 < 0.0f) {
        arg2->unkC.f[2] = 0.0f;
    } else {
        arg2->unkC.f[2] = temp_fa0_4;
    }
    
    vec3_transform(&sp130, arg2->unkC.x, arg2->unkC.y, arg2->unkC.z, 
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
        vec3_transform(&spF0, sp60.f[0], sp60.f[1], sp60.f[2], 
                       &arg2->unkC.x, &arg2->unkC.y, &arg2->unkC.z);
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
            temp_fv0 = 1.0f / var_a0;
            VECTOR_SCALE(sp6C, temp_fv0);
        } else {
            sp6C.f[0] = 0.0f;
            sp6C.f[1] = 1.0f;
            sp6C.f[2] = 0.0f;
        }
        
        bss_10.yaw = -objData->yaw;
        bss_10.pitch = 0;
        bss_10.roll = 0;
        matrix_from_srt_reversed(&spB0, &bss_10);
        vec3_transform(&spB0, sp6C.f[0], sp6C.f[1], sp6C.f[2], &sp6C.f[0], &sp6C.f[1], &sp6C.f[2]);
        
        angle1 = M_90_DEGREES - arctan2_f(sp6C.f[1], sp6C.f[2]);
        angle2 = -(M_90_DEGREES - arctan2_f(sp6C.f[1], sp6C.f[0]));
        angle1 -= (objData->pitch & 0xFFFF);
        CIRCLE_WRAP(angle1);
        objData->pitch += (((angle1 >> 2) / 3) * (s32) updateRate);
        self->srt.pitch = objData->pitch + objData->unk3E6;
        
        angle2 -= (objData->roll & 0xFFFF);
        CIRCLE_WRAP(angle2);
        objData->roll += (((angle2 >> 2) / 3) * (s32) updateRate);
    }
    
    objData->yaw -= (s16) (objData->unk2E0.xJoy * (70.0f - (objData->unk2E0.yJoy * 0.05f)) * 0.0666f);
}

// offset: 0x2E64 | func: 24
void CRSnowBike_func_2E64(Object* self, CRSnowBike_Data* objData, DLL732_Data2AC* arg2, f32 updateRate, s32 arg4) {
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
    
    if (arg2->unkC.f[0] > 4.0f) {
        arg2->unkC.f[0] = 4.0f;
    }
    if (arg2->unkC.f[0] < -4.0f) {
        arg2->unkC.f[0] = -4.0f;
    }
    if (arg2->unkC.f[1] > 4.0f) {
        arg2->unkC.f[1] = 4.0f;
    }
    if (arg2->unkC.f[1] < -4.0f) {
        arg2->unkC.f[1] = -4.0f;
    }
    if (arg2->unkC.f[2] > 5.0f) {
        arg2->unkC.f[2] = 5.0f;
    }
    if (arg2->unkC.f[2] < -5.8f) {
        arg2->unkC.f[2] = -5.8f;
    }
    
    bss_10.yaw = objData->yaw;
    bss_10.pitch = objData->pitch;
    bss_10.roll = objData->roll;
    matrix_from_srt(&sp120, &bss_10);
    
    bss_10.yaw = -objData->yaw;
    bss_10.pitch = -objData->pitch;
    bss_10.roll = -objData->roll;
    matrix_from_srt_reversed(&spE0, &bss_10);
    
    vec3_transform(&spE0, 0.0f, arg2->unk28 * arg2->unk1C, 0.0f, &sp80.f[0], &sp80.f[1], &sp80.f[2]);

    temp_fv0 = -(f32) objData->unk2E0.yJoy * 50.0f;
    temp_fv0 *= arg2->unk18;
    arg2->unkC.f[2] += temp_fv0;
    arg2->unk0.f[0] = sp80.f[0] * arg2->unk18;
    arg2->unk0.f[1] = sp80.f[1] * arg2->unk18;
    arg2->unk0.f[2] = sp80.f[2] * arg2->unk18;
    arg2->unkC.f[0] = arg2->unkC.f[0] + arg2->unk0.f[0];
    arg2->unkC.f[1] = arg2->unkC.f[1] + arg2->unk0.f[1];
    arg2->unkC.f[2] = arg2->unkC.f[2] + arg2->unk0.f[2];
    
    if (collision->unk25D != 0) {
        arg2->unkC.f[0] = 0.0f;
        var_fv1 = arg2->unk2C * sp80.y;
        
        if (arg2->unkC.f[2] < 0.0f) {
            if (var_fv1 < 0.0f) {
                var_fv1 = -var_fv1;
            }
        } else if (var_fv1 > 0.0f) {
            var_fv1 = -var_fv1;
        }

        var_fv1 *= arg2->unk18;
        temp_fv0 = arg2->unkC.f[2] + var_fv1;
        if (temp_fv0 > 0.0f) {
            arg2->unkC.f[2] = 0.0f;
        } else {
            arg2->unkC.f[2] = temp_fv0;
        }
    }

    sq = SQ(arg2->unkC.f[2]);
    var_fv1 = arg2->unk30 * sq;
    var_fv1 *= arg2->unk18;
    
    temp_fv0 = arg2->unkC.f[2] + var_fv1;
    if (temp_fv0 > 0.0f) {
        arg2->unkC.f[2] = 0.0f;
    } else {
        arg2->unkC.f[2] = temp_fv0;
    }
    
    vec3_transform(&sp120, arg2->unkC.f[0], arg2->unkC.f[1], arg2->unkC.f[2], &self->velocity.f[0], &self->velocity.f[1], &self->velocity.f[2]);
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
        vec3_transform(&spE0, sp68.f[0], sp68.f[1], sp68.f[2], &arg2->unkC.f[0], &arg2->unkC.f[1], &arg2->unkC.f[2]);
        
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
        
        bss_10.yaw = -objData->yaw;
        bss_10.pitch = 0;
        bss_10.roll = 0;
        matrix_from_srt_reversed(&spA0, &bss_10);
        vec3_transform(&spA0, sp74.f[0], sp74.f[1], sp74.f[2], &sp74.f[0], &sp74.f[1], &sp74.f[2]);
        
        var_s0 = M_90_DEGREES - arctan2_f(sp74.f[1], sp74.f[2]);
        temp_a1 = -(M_90_DEGREES - arctan2_f(sp74.f[1], sp74.f[0]));
        var_s0 -= (objData->pitch & 0xFFFF);
        CIRCLE_WRAP(var_s0);
        objData->pitch += ((var_s0 >> 2) / 3) * (s32)updateRate;
        self->srt.pitch = objData->pitch + objData->unk3E6;
        temp_a1 -= (objData->roll & 0xFFFF);
        CIRCLE_WRAP(temp_a1);
        objData->roll += ((temp_a1 >> 2) / 3) * (s32)updateRate;
    }
    
    objData->yaw -= (s16) (objData->unk2E0.xJoy * (70.0f - (objData->unk2E0.yJoy * 0.05f)) * 0.0666f);
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
        objData->unk3EC = 20;
        objData->unk384 = 0.8f;
        return;
    case Damage_Type_14:
        objData->unk3EC = 5;
        objData->unk384 = 0.75f;
        return;
    case Damage_Type_1D:
        objData->fuelAmount += (hitDamage * 100);
        objData->unk3CC.f[0] = 2.0f;
        objData->unk3CC.f[1] = 4.0f;
        objData->unk3CC.f[2] = 4.6f;
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
    DLL732_Data2AC* v;
    Vec3f spB8;
    SRT transform;
    s32 i;
    MtxF mtx;

    objData = self->data;
    unkSubstruct = &objData->unk2E0;

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case 2:
            main_set_bits(BIT_Play_Seq_02AA_CF_Lose_Race, TRUE);
            break;
        case 3:
            gDLL_1_cmdmenu->vtbl->energy_bar_free();
            break;
        }
    }
    
    if (objData->state == STATE_2) {
        spB8.f[0] = (self->srt.transl.x - objData->unk3A0.f[0]) * gUpdateRateInverseF;
        spB8.f[1] = (self->srt.transl.y - objData->unk3A0.f[1]) * gUpdateRateInverseF;
        spB8.f[2] = (self->srt.transl.z - objData->unk3A0.f[2]) * gUpdateRateInverseF;

        spB8.f[0] *= 0.93749994f;
        spB8.f[1] *= 0.93749994f;
        spB8.f[2] *= 0.93749994f;
        
        transform.transl.x = 0.0f;
        transform.transl.y = 0.0f;
        transform.transl.z = 0.0f;
        transform.scale = 1.0f;
        transform.yaw = -self->srt.yaw;
        transform.pitch = -self->srt.pitch;
        transform.roll = -self->srt.roll;
        v = &objData->unk2AC;
        matrix_from_srt_reversed(&mtx, &transform);
        
        vec3_transform(&mtx, spB8.f[0], spB8.f[1], spB8.f[2], &v->unkC.f[0], &v->unkC.f[1], &v->unkC.f[2]);

        unkSubstruct->yJoy += gUpdateRate * 8;
        if (unkSubstruct->yJoy > 70) {
            unkSubstruct->yJoy = 70;
        }
        CRSnowBike_func_40FC(self, objData, v->unkC.f[2], unkSubstruct->yJoy, &unkSubstruct->unk10, 4);
    }
    
    objData->unk3A0.x = self->srt.transl.x;
    objData->unk3A0.y = self->srt.transl.y;
    objData->unk3A0.z = self->srt.transl.z;
    objData->flags |= CRSnowBike_FLAG_10_Was_In_Sequence;
    objData->flags &= ~CRSnowBike_FLAG_8_Race_Started;
    return 0;
}

// offset: 0x3AF8 | func: 29
void CRSnowBike_func_3AF8(Object* self, CRSnowBike_Data* objData, DLL27_Data* collision) {
    s32 pad;
    MtxF spDC;
    Vec3f sp88[7];
    s8 sp87;
    s8 sp86;
    s32 i;

    sp86 = -0x80 >> (7 - objData->unk3EE);
    sp86 = ~sp86;
    objData->unk3F3 = -1;
    objData->unk39C = objData->unk2AC.unkC.f[2];
    
    if (sp86) {
        do {
            sp87 = sp86;
            for (i = 0; i < objData->unk3EE; i++){
                CRSnowBike_get_bike_matrix(self, objData, &spDC, 1, 1, 1);
                vec3_transform(&spDC, data_64[i].x, data_64[i].y, data_64[i].z, 
                               &sp88[i].x, &sp88[i].y, &sp88[i].z);
                if (func_80059C40(&objData->unk330[i], 
                        &sp88[i], data_A0[i], 0, NULL, self, 8, 1, 0xFF, 0) == 0) {
                    sp87 &= ~(1 << i);
                } else {
                    bcopy(&sp88[i], &objData->unk330[i], sizeof(Vec3f));
                    spDC.m[3][0] = sp88[i].x;
                    spDC.m[3][1] = sp88[i].y;
                    spDC.m[3][2] = sp88[i].z;
                    vec3_transform(&spDC, -data_64[i].x, -data_64[i].y, -data_64[i].z, 
                        &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z
                    );
                }
            }
            
            if (objData->unk3F3 == -1) {
                objData->unk3F3 = sp87;
            }
        } while (sp87);
    }
    
    bcopy(&sp88, objData->unk330, objData->unk3EE * sizeof(Vec3f));
}

// offset: 0x3DAC | func: 30
s32 CRSnowBike_func_3DAC(Object* self, CRSnowBike_Data* arg1, CRSnowBike_Data* objData, DLL732_Unk_2E0* arg3) {
    f32 dx;
    f32 dz;
    s32 sp3C;
    f32 var_fv1;
    s32 dYaw;
    s32 angle;
    CRSnowBike_Setup* objSetup;

    objSetup = (CRSnowBike_Setup*)self->setup;
    
    dx = self->srt.transl.x - objData->unk0.transl.x;
    dz = self->srt.transl.z - objData->unk0.transl.z;
    dx = sqrtf(SQ(dx) + SQ(dz));
    
    var_fv1 = 100.0f - dx;
    if (dx > 100.0f) {
        var_fv1 = 0.0f;
    }
    
    sp3C = CRSnowBike_func_0(self, objData, var_fv1);
    
    gDLL_4_Race->vtbl->func4(self, &objData->raceData);
    gDLL_4_Race->vtbl->func10(&objData->raceData);
    if (sp3C != 0) {
        arg3->xJoy = 0;
        arg3->yJoy = 0;
        return 1;
    }

    dx = self->srt.transl.x - objData->unk0.transl.x;
    dz = self->srt.transl.z - objData->unk0.transl.z;
    dYaw = arctan2_f(dx, dz) - (self->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(dYaw);
    
    angle = dYaw >> 5;
    if (angle > 65) {
        angle = 65;
    } else if (angle < -65) {
        angle = -65;
    }
    
    arg3->xJoy = -angle;
    if (angle < 0) {
        angle = -angle;
    }

    arg3->yJoy = (objSetup->unk20 - 8.0f) - (angle * (objSetup->unk20 * 0.02f));
    diPrintf(" YJOY %i ", arg3->yJoy);
    return 0;
}

// offset: 0x3FE0 | func: 31
void CRSnowBike_func_3FE0(Object* self, CRSnowBike_Data* objData) {
    MtxF sp60;
    s32 i;

    gDLL_27->vtbl->reset(self, &objData->collision);
    CRSnowBike_get_bike_matrix(self, objData, &sp60, 0, 0, 0);
    
    for (i = 0; i < objData->unk3EE; i++) {
        vec3_transform(&sp60, 
            data_64[i].x, data_64[i].y, data_64[i].z, 
            &objData->unk330[i].x, &objData->unk330[i].y, &objData->unk330[i].z
        );
    }
}

// offset: 0x40FC | func: 32
void CRSnowBike_func_40FC(Object* self, CRSnowBike_Data* objData, f32 arg2, s32 arg3, s8* arg6, u8 arg5) {
    /*0x2C*/ static f32 sEngineAudioTweak;
    f32 sp54 = 1.0f;
    SRT sp3C;
    s32 volume;
    
    if (arg5 & 1) {
        if (objData->soundHandle1 == 0) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_289_Engine_Loop, MAX_VOLUME, &objData->soundHandle1, NULL, 0, NULL);
        }

        if (objData->soundHandle1 != 0) {
            sEngineAudioTweak = arg2 * 11.6f;
            if (sEngineAudioTweak < 0.0f) {
                sEngineAudioTweak = -sEngineAudioTweak;
            }
            if (sEngineAudioTweak < 40.0f) {
                sEngineAudioTweak = 40.0f;
            }
            if (sEngineAudioTweak > 200.0f) {
                sEngineAudioTweak = 200.0f;
            }
            gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandle1, (sEngineAudioTweak / 70.0f) + 0.1f);

            if (objData->unk3F2 < 0x12) {
                volume = arg2 * 30.0f;
                if (volume < 0) {
                    volume = -volume;
                }
                if (volume > MAX_VOLUME) {
                    volume = MAX_VOLUME;
                }
                gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandle1, volume);
            } else {
                gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandle1, 0);
            }
        }
    }
    
    if (arg5 & 2) {
        if (objData->soundHandle3 == 0) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_28F, MAX_VOLUME, &objData->soundHandle3, NULL, 0, NULL);
        }
        if (objData->soundHandle3 != 0) {
            sEngineAudioTweak = arg2 ? ((self->srt.roll * arg2) / 30000.0f) : 0;
            if (sEngineAudioTweak < 0) {
                sEngineAudioTweak = -sEngineAudioTweak;
            } else if (sEngineAudioTweak > 1.0f) {
                sEngineAudioTweak = 1.0f;
            }
            gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandle3, 0.1f + sEngineAudioTweak);

            if (objData->unk3F2 < 0x12) {
                sEngineAudioTweak *= MAX_VOLUME_F;
                if (sEngineAudioTweak > MAX_VOLUME_F) {
                    sEngineAudioTweak = MAX_VOLUME_F;
                } else if (sEngineAudioTweak < 0.0f) {
                    sEngineAudioTweak = 0.0f;
                }

                gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandle3, sEngineAudioTweak);
            } else {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandle3);
                objData->soundHandle3 = 0;
            }
        }
    }
    
    if (arg5 & 4) {
        if (objData->unk3EA <= 0) {
            if (objData->soundHandle2 == 0) {
                gDLL_6_AMSFX->vtbl->play(self, SOUND_50D_Jet_Loop, MAX_VOLUME, &objData->soundHandle2, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->play(self, SOUND_28E, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            if (objData->soundHandle4 == 0) {
                gDLL_6_AMSFX->vtbl->play(self, SOUND_50C_Low_Rumble_Loop, MAX_VOLUME, &objData->soundHandle4, NULL, 0, NULL);
            }
        }
        
        if (objData->soundHandle2 != 0) {
            gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandle2, (objData->unk398 * 0.00048828125f) + 0.5f);
            if (arg3 >= 6) {
                objData->unk398 += gUpdateRateF;
            } else {
                if (objData->unk398 > 55.0f) {
                    objData->unk398 -= (0.5f * gUpdateRateF);
                } else {
                    objData->unk398 += (0.2f * gUpdateRateF);
                }
            }
            if (objData->unk398 > 90.0f) {
                objData->unk398 = 90.0f;
            }
            gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandle2, objData->unk398);
        }

        if (objData->soundHandle4 != 0) {
            gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandle4, (objData->unk394 / 75.0f) + 0.2f);
            if (arg3 >= 6) {
                objData->unk394 = (arg3 * 0.6f) + 15.0f;
            } else {
                if (0.5f * gUpdateRateF < objData->unk394) {
                    objData->unk394 -= 0.5f * gUpdateRateF;
                } else {
                    objData->unk394 = 0.0f;
                }
            }
            if (objData->unk394 > MAX_VOLUME_F) {
                objData->unk394 = MAX_VOLUME_F;
            }
            gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandle4, objData->unk394);
        }
    }
    
    if (objData->unk2F4 == NULL) {
        objData->unk2F4 = dll_load_deferred(0x1025, 1);
    }
    
    if (objData->unk2F8 == NULL) {
        objData->unk2F8 = dll_load_deferred(0x1026, 1);
    }
    
    if (objData->unk3EA <= 0) {
        sp54 = 0.7f;
        if (arg3 > 10) {
            sp54 = arg3;
            sp54 = (0.01f * sp54) + 0.1f;
            objData->unk2F4->vtbl->func0(self, 0, 0, 0x404, -1, &sp54);
        }

        objData->unk3EA = 30;

        if ((self->srt.roll > 1000) && (arg2 < -1.0f)) {
            sp3C.scale = self->srt.roll / 8000.0f;
            sp3C.scale *= (-arg2 / 3.8f);
            objData->unk2F8->vtbl->func0(self, 0, &sp3C, 0x404, -1, 0);
        } else if ((self->srt.roll < -0x3E8) && (arg2 < -1.0f)) {
            sp3C.scale = self->srt.roll / -8000.0f;
            sp3C.scale *= (-arg2 / 3.8f);
            objData->unk2F8->vtbl->func0(self, 1, &sp3C, 0x404, -1, 0);
        }
    }
    objData->unk3EA -= gUpdateRate;
}

// offset: 0x4B30 | func: 33 | export: 20
void CRSnowBike_func_4B30(s32 arg0, s32 arg1, s32 arg2) {

}
