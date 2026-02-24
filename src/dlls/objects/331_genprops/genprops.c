#include "common.h"
#include "game/gamebits.h"
#include "sys/gfx/model.h"
#include "sys/objmsg.h"

/*0x0*/ static Object* _data_0 = NULL;
/*0x4*/ static u16 _data_4[] = {
    0x0157, 0x0158, 0x015a, 0x015b, 0x0000, 0x0000
};

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 modelInstanceIdx; //modelInstanceIdx (OBJ_DFP_blockwall)
    s16 pitch;           //usually pitch, but some objects (vines etc.) use as debug print distance, or to toggle sounds (DIM2IceFloe2)
    s16 roll;            //usually roll, but used for minCooldown on DIM2IceFloe2
    s16 gamebitA;        //gamebitID (NOTE: can be stored on opposite gamebit slot in GenProps_Data)
    s16 gamebitB;        //gamebitID (NOTE: can be stored on opposite gamebit slot in GenProps_Data)
} GenProps_Setup;

typedef struct {
    s32 unk0;               //unused
    f32 pStartX;            //initial position for WMplatform
    f32 pStartY;            //initial position for WMplatform
    f32 pStartZ;            //initial position for WMplatform
    f32 tValue;             //used by WMPlatform for lerping position
    f32 pitch;              //used by WMrock (pitch), SB_Galleon (?)
    f32 roll;               //used by WMrock (roll), SB_Galleon (?)
    f32 unk1C;              //used by SB_Galleon, Krazcol, SB_Lamp
    f32 unk20;              //used by SB_Galleon, Krazcol
    f32 unk24;              //used by SB_Galleon, Krazcol, SB_Lamp
    f32 unk28;              //used by SB_Galleon, Krazcol
    f32 debugPrintDistance; //usually radius for debug prints, but WMrock uses it differently 
    s16 unk30;              //unused
    s16 lampZero;           //used by SB_Lamp
    s16 lampRandom;         //used by SB_Lamp (randomised value)
    s16 speed;              //usually speed, but WMrock seems to use it as a timer
    s16 gamebitA;           //used by many objects
    s16 gamebitB;           //used by many objects
    s16 timer;              //used by WM_Platform, DIM2IceFloe2
    u8 unk3E;               //used by WMrock, SB_Galleon, Krazcol (sound/debug related?)
    u8 lampBool;            //used by SB_Lamp
    u8 lastSoundIndex;      //used by DIM2IceFloe2, last index into soundID data (_data_4)
    s8 vineHealth;          //used by NoPassVines
    s16 unk42;              //unused
    u16* soundIDs;          //used by DIM2IceFloe2
    u16 minCooldown;        //used by DIM2IceFloe2, affecting time between sounds
    s16 unk4A;              //unused
} GenProps_Data;

static s32 dll_331_func_1D34(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3);

// offset: 0x0 | ctor
void dll_331_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_331_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_331_setup(Object* self, GenProps_Setup* objSetup, s32 arg2) {
    s16 id;
    s16 temp;
    GenProps_Data* objData;

    id = objSetup->base.objId;
    objData = self->data;
    objData->debugPrintDistance = 1.0f;
    
    switch (id) {
    case OBJ_NWbigrock:
        self->srt.pitch = objSetup->pitch;
        self->srt.roll = objSetup->roll;
        self->srt.scale = 1.0f;
        self->animCallback = (void*)&dll_331_func_1D34;
        return;
    case OBJ_WMfallencol:
        self->srt.yaw = objSetup->yaw << 8;
        self->srt.pitch = objSetup->pitch;
        self->srt.roll = objSetup->roll;
        self->srt.scale = 1.0f;
        return;
    case OBJ_DFSHcol:
    case OBJ_MMSHcol:
    case OBJ_ECSHcol:
    case OBJ_GPSHcol:
    case OBJ_DBSHcol:
    case OBJ_WGSHcol:
    case OBJ_DFP_dish:
        self->srt.yaw = objSetup->yaw << 8;
        return;
    case OBJ_WM_Walkway1:
    case OBJ_WM_Walkway2:
    case OBJ_WM_Platform:
        self->srt.yaw = objSetup->yaw << 8;
        self->srt.pitch = objSetup->pitch;
        objData->gamebitA = objSetup->gamebitA;
        objData->gamebitB = objSetup->gamebitB;
        objData->tValue = 0.0f;
        objData->pStartX = objSetup->base.x;
        objData->pStartY = objSetup->base.y;
        objData->pStartZ = objSetup->base.z;
        objData->timer = 500;
        objData->speed = 1;
        if ((id == OBJ_WM_Platform) && main_get_bits(BIT_322)) {
            self->srt.transl.x = 34022.0f;
            self->srt.transl.y = 457.0f;
            self->srt.transl.z = 22523.0f;
        } else if ((id == OBJ_WM_Platform) && main_get_bits(BIT_369)) {
            self->srt.transl.x = 34022.0f;
            self->srt.transl.y = 457.0f;
            self->srt.transl.z = 22523.0f;
        }
        self->animCallback = (void*)&dll_331_func_1D34;
        return;
    case OBJ_WM_MoonSeedMoun:
        self->srt.yaw = objSetup->yaw << 8;
        objData->debugPrintDistance = objSetup->pitch;
        return;
    case OBJ_WM_NoPassVine:
        self->srt.yaw = objSetup->yaw << 8;
        objData->debugPrintDistance = objSetup->pitch;
        objData->vineHealth = 2;
        return;
    case OBJ_WM_NoPassHorzVi:
        self->srt.yaw = objSetup->yaw << 8;
        objData->debugPrintDistance = objSetup->pitch;
        objData->vineHealth = 2;
        objData->gamebitA = objSetup->gamebitA;
        return;
    case OBJ_NWSH_col:
        self->srt.yaw = objSetup->yaw << 8;
        self->srt.pitch = objSetup->pitch;
        return;
    case OBJ_NWSH_rock:
    case OBJ_WMrock:
        objData->roll = rand_next(100, 400);
        objData->pitch = rand_next(100, 400);
        self->speed.x = rand_next(0, 100) * 0.04f;
        self->speed.z = rand_next(0, 100) * 0.04f;
        self->srt.scale *= 0.3f + (0.01f * rand_next(0, 10));
        objData->speed = 200;
        objData->debugPrintDistance = 0;
        objData->unk3E = 2;
        self->modelInstIdx = rand_next(0, 1);
        return;
    case OBJ_DFP_blockwall:
        self->srt.yaw = objSetup->yaw << 8;
        self->modelInstIdx = objSetup->modelInstanceIdx;
        objData->gamebitA = objSetup->gamebitB;
        if (main_get_bits(objData->gamebitA)) {
            self->srt.transl.y = objSetup->base.y + 30.0f;
            return;
        }
        return;
    case OBJ_WMlargerock:
        self->srt.yaw = objSetup->yaw << 8;
        self->srt.pitch = objSetup->pitch;
        self->srt.roll = objSetup->roll;
        self->srt.scale = 1.0f;
        self->animCallback = (void*)&dll_331_func_1D34;
        objData->gamebitB = objSetup->gamebitB;
        objData->gamebitA = objSetup->gamebitA;
        return;
    case 133: //unknown deleted object
        self->srt.yaw = objSetup->yaw << 8;
        self->srt.pitch = objSetup->pitch;
        self->srt.roll = objSetup->roll;
        self->srt.scale = 0.05f;
        self->unkDC = 100;
        self->unkE0 = 0;
        self->animCallback = (void*)&dll_331_func_1D34;
        return;
    case 134: //unknown deleted object
        self->srt.yaw = objSetup->yaw << 8;
        self->srt.pitch = objSetup->pitch;
        self->srt.roll = objSetup->roll;
        self->srt.scale = 0.5f;
        self->unkDC = 0;
        self->unkE0 = 0;
        self->animCallback = (void*)&dll_331_func_1D34;
        return;
    case OBJ_SB_Galleon:
        self->srt.yaw = 0;
        self->srt.pitch = 0;
        if (objSetup->roll >= 1000) {
            self->srt.scale = 1.0f / (objSetup->roll / 1000.0f);
        } else {
            self->srt.scale = 0.2f;
        }
        objData->unk3E = 0;
        objData->pStartX = objSetup->base.x;
        objData->pStartY = objSetup->base.y;
        objData->pStartZ = objSetup->base.z;
        objData->roll = 0.0f;
        objData->pitch = 0.0f;
        objData->unk24 = 0.5f;
        objData->unk1C = 0.5f;
        objData->unk28 = 1000.0f;
        objData->unk20 = 400.0f;
        self->srt.roll = 0;
        self->animCallback = (void*)&dll_331_func_1D34;
        return;
    case OBJ_Krazcol:
        self->srt.yaw = objSetup->yaw << 8;
        self->srt.scale = 1.0f;
        objData->unk3E = 0;
        objData->pStartX = objSetup->base.x;
        objData->pStartY = objSetup->base.y;
        objData->pStartZ = objSetup->base.z;
        objData->roll = 0.0f;
        objData->pitch = 0.0f;
        objData->unk28 = 0.0f;
        objData->unk20 = 0.0f;
        objData->unk24 = 0.0f;
        objData->unk1C = 0.0f;
        self->animCallback = (void*)&dll_331_func_1D34;
        return;
    case OBJ_SB_Lamp:
        self->srt.yaw = 0;
        self->srt.pitch = 0;
        self->srt.roll = 0;
        self->srt.scale = 1.0f;
        self->unkDC = 0;
        self->unkE0 = 0;
        objData->lampZero = 0;
        objData->unk1C = 0.5f;
        objData->unk24 = 600.0f;
        objData->lampRandom = rand_next(1000, 5000);
        objData->lampBool = TRUE;
        self->animCallback = (void*)&dll_331_func_1D34;
        return;
    case OBJ_DIM2IceFloe2:
        self->objhitInfo = 0;
        if (objSetup->pitch == 0) {
            objData->soundIDs = _data_4;
            objData->lastSoundIndex = 4;
        }
        temp = objSetup->roll ^ 0;
        objData->minCooldown = temp;
        objData->timer = temp;
        return;
    case OBJ_DFdebris1:
        objData->roll = 30.0f;
        return;
    case OBJ_FireFly:
        self->srt.scale = self->def->scale * 40.0f;
        break;
    }
}

// offset: 0x7CC | func: 1 | export: 1
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/331_genprops/dll_331_control.s")
#else
//https://decomp.me/scratch/d5noN
void dll_331_control(Object* self) {
    s32 index;
    Object* player;
    ObjectPolyhits* temp_v0;
    s16 id;
    Object** new_var;
    GenProps_Setup *objSetup;
    s32 temp_t3;
    GenProps_Data* objData;
    DLL_Unknown* tempDLL2;
    DLL_Unknown* tempDLL;
    u8 temp_t8;
    u8 var_v1;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distance;
    GenProps_Data* objData2;
    Camera* camera;

    player = get_player();
    objData = self->data;
    camera = get_camera();
    objSetup = (GenProps_Setup*)self->setup;
    
    id = self->id;
    
    switch (id) {
    case OBJ_NWbigrock: //0x81
        break;
    case OBJ_DFP_PowerBolt: //0x4fa
        self->unkE0 -= (s16)gUpdateRateF;
        self->srt.transl.f[0] += self->speed.f[0] * gUpdateRateF; 
        self->srt.transl.f[1] += self->speed.f[1] * gUpdateRateF; 
        self->srt.transl.f[2] += self->speed.f[2] * gUpdateRateF;
        gDLL_17_partfx->vtbl->spawn(self, 0x5F3, NULL, 0x10001, -1, NULL);
        if (vec3_distance(&self->positionMirror, &player->positionMirror) < 30.0f) {
            diPrintf("\tHit Krystal\n");
            obj_send_mesg(player, 0x60004, self, (void*)1);
            obj_destroy_object(self);
        }
        if (self->unkE0 <= 0) {
            obj_destroy_object(self);
        }
        return;
    case OBJ_VFP_PowerBolt: //0x549
        self->unkE0 -= (s16)gUpdateRateF;
        self->srt.transl.f[0] += self->speed.f[0] * gUpdateRateF;
        self->srt.transl.f[1] += self->speed.f[1] * gUpdateRateF;
        self->srt.transl.f[2] += self->speed.f[2] * gUpdateRateF;
        gDLL_17_partfx->vtbl->spawn(self, 0x39D, NULL, 0x10001, -1, NULL);
        if (vec3_distance(&self->positionMirror, &player->positionMirror) < 30.0f) {
            diPrintf("\tHit Krystal\n");
            obj_send_mesg(player, 0x60004, self, (void*)1);
            obj_destroy_object(self);
        }
        if ((self->unkE0 <= 0) != 0) {
            obj_destroy_object(self);
        }
        break;
    case OBJ_DFP_blockwall: //0x4bf
        if ((self->srt.transl.f[1] < (objSetup->base.y + 30.0f)) && (main_get_bits(objData->gamebitA))) {
            self->srt.transl.f[1] += gUpdateRateF;
        }
        break;
    case OBJ_GPSHswapstone: //0x409
        gDLL_3_Animation->vtbl->func17(0, self, -1);
        break;
    case OBJ_DFturbinelever: //0xae
        
        new_var = &self->polyhits->unk100[0];
        if ((self->unkDC == 0) && (new_var != NULL) && (*(s16*)(((s32)self->polyhits) + 0x146) == 0x2B)) {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
            self->unkDC = 1;
        }
        break;
        
    case OBJ_DFdebris1: //0xab
        objData->roll -= gUpdateRate;
        if (objData->roll < 0) {
            tempDLL2 = dll_load_deferred(0x1003, 1);
            tempDLL2->vtbl->func[0].withSixArgs((s32)self, 0, 0, 1, -1, 0);
            dll_unload(tempDLL2);
            objData->roll = 30.0f;
            return;
        }
        break;
    case OBJ_WMlargerock: //0x2b7
        if (main_get_bits(objData->gamebitA) == 0) {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
        }
        break;
    case OBJ_WM_MoonSeedMoun: //0x271
        if (vec3_distance(&self->positionMirror, &player->positionMirror) < objData->debugPrintDistance) {
            diPrintf("\tMoonSeed Mound\n");
            diPrintf("\tThe Player Guesses that a Seed goes here!\n");
        }
        break;
    case OBJ_WM_Walkway1: //0x293
    case OBJ_WM_Walkway2: //0x294
        if ((objData->gamebitA != -1) && (main_get_bits(objData->gamebitA) != 0)) {
            if (self->srt.pitch <= 0) {
                self->srt.pitch += 50;
            } else {
                self->srt.pitch = 0;
            }
        }
        break;
    case OBJ_WM_Platform: //0x295
        if (objData->gamebitA != -1) {
            objData2 = objData;
            if ((main_get_bits(objData2->gamebitA)) && (objData2->tValue <= 1.0f) && (objData2->timer == 0)) {
                if (objData2->gamebitB != -1) {
                    if (main_get_bits(objData2->gamebitB)) {
                        objData2->timer = 400;
                        main_set_bits(objData2->gamebitB, 0);
                    }
                }
                objData2->tValue += 0.001f * objData2->speed * gUpdateRateF;
                if (objData2->tValue > 1.0f) {
                    objData2->tValue = 1.0f;
                    objData2->speed *= -1;
                    objData2->timer = 400;
                    main_set_bits(BIT_322, 1);
                } else if (objData2->tValue < 0.0f) {
                    objData2->tValue = 0.0f;
                    objData2->speed *= -1;
                    objData2->timer = 400;
                    main_set_bits(BIT_322, 0);
                }

                //Lerping for the platform, with different paths based on gamebitID
                if (objData2->gamebitA == BIT_319) {
                    self->srt.transl.f[0] = objData2->pStartX + (objData2->tValue * (14807.0f - objData2->pStartX));
                    self->srt.transl.f[1] = objData2->pStartY + (objData2->tValue * (457.0f - objData2->pStartY));
                    self->srt.transl.f[2] = objData2->pStartZ + (objData2->tValue * (3305.0f - objData2->pStartZ));
                    self->srt.yaw = (1.0f - objData->tValue) * 8192.0f;
                } else if (objData2->gamebitA == BIT_363) {
                    self->srt.transl.f[0] = objData2->pStartX + (objData2->tValue * (14621.0f - objData2->pStartX));
                    self->srt.transl.f[1] = objData2->pStartY + (objData2->tValue * (457.0f - objData2->pStartY));
                    self->srt.transl.f[2] = objData2->pStartZ + (objData2->tValue * (2453.0f - objData2->pStartZ));
                    self->srt.yaw = (1.0f - objData2->tValue) * 8192.0f;
                }

                //Tying player position directly to platform
                player->srt.transl.f[0] = self->srt.transl.f[0];
                player->srt.transl.f[1] = self->srt.transl.f[1];
                player->srt.transl.f[2] = self->srt.transl.f[2];
                return;
            }
        }

        //Handle cooldown timer
        if (objData->timer > 0) {
            objData->timer -= gUpdateRate;
            if (objData->timer <= 0) {
                objData->timer = 0;
                if (main_get_bits(BIT_322) != 0) {
                    main_set_bits(BIT_322, 0);
                } else if (main_get_bits(BIT_369) != 0) {
                    main_set_bits(BIT_369, 0);
                }
            }
        }
        break;
    case OBJ_WM_NoPassVine: //0x273
        //Handle collisions damaging the vines
        if (func_80025F40(self, NULL, NULL, NULL) != 0) {
            objData->vineHealth--;
            if (objData->vineHealth < 0) {
                obj_destroy_object(self);
            }
        }
        //Print debug info about object's purpose
        if (vec3_distance(&self->positionMirror, &player->positionMirror) < objData->debugPrintDistance) {
            diPrintf("\tNoPass Vine\n");
            diPrintf("\tThe Player Burns it away!\n");
        }
        break;
    case OBJ_WM_NoPassHorzVi: //0x28c
        //Handle collisions damaging the vines
        if (func_80025F40(self, NULL, NULL, NULL) != 0) {
            objData->vineHealth--;
            if (objData->vineHealth < 0) {
                main_set_bits(objData->gamebitA, 1);
                obj_destroy_object(self);
            }
        }
        //Print debug info about object's purpose
        if (vec3_distance(&self->positionMirror, &player->positionMirror) < objData->debugPrintDistance) {
            diPrintf("\tNoPass Vine\n");
            diPrintf("\tThe Player Burns it away!\n");
        }
        break;
    case OBJ_NWSH_rock: //0x221
    case OBJ_WMrock: //0x2bc
        self->objhitInfo->unk5D = 13;   //width?
        self->objhitInfo->unk5E = 2;    //height?
        self->objhitInfo->unk52 = 10;   //depth?
        self->objhitInfo->unkC = 10.0f;
        self->objhitInfo->unk50 = 30;
        self->objhitInfo->unk58 |= 1;
        if (main_get_bits(BIT_Player_Immune_to_Rainbow_Scarabs) != 0) {
            self->objhitInfo->unk58 &= ~1;
        }
        if (objData->unk3E == 2) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_35A_Low_Whoosh, 0x43, NULL, NULL, 0, NULL);
            objData->unk3E--;
        }
        if ((objData->unk3E != 0) && ((self->srt.transl.f[1] + self->speed.f[1]) <= player->srt.transl.f[1])) {
            objData->unk3E = 0;
            self->speed.f[1] *= -0.4f;
            self->speed.f[0] *= 2.0f;
            self->speed.f[2] *= 2.0f;
            self->srt.scale *= 0.5f;
            objData->roll *= 2;
            objData->pitch *= 2;
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_35B_Rock_Slide, 0x43, NULL, NULL, 0, NULL);
            camera_set_shake_offset(0.5f);
        }
        if (objData->unk3E == 0) {
            if (objData->debugPrintDistance <= 40000.0f) {
                objData->debugPrintDistance += 2.0f * gUpdateRateF;
                func_80026940(self, (s16)(objData->debugPrintDistance / 10.0f) + 20);
            }
        }
        self->speed.f[1] += -0.15f * gUpdateRateF;
        self->srt.roll += objData->roll;
        self->srt.pitch += objData->pitch;
        self->srt.transl.y += self->speed.y;
        self->srt.transl.x += self->speed.x;
        self->srt.transl.z += self->speed.z;
        self->positionMirror.y = self->srt.transl.y;
        self->positionMirror.x = self->srt.transl.x;
        self->positionMirror.z = self->srt.transl.z;
        objData->speed -= gUpdateRate;
        if (player->srt.transl.f[1] < self->srt.transl.f[1]) {
            if (rand_next(0, 2) == 0) {
                gDLL_17_partfx->vtbl->spawn(self, 0x27F, NULL, 0x10001, -1, NULL);
            }
        }
        if (objData->speed <= 0) {
            obj_destroy_object(self);
        }
        break;
    case OBJ_SB_Galleon: //0x322
        objData->pitch += objData->unk1C * 3.0f;
        if ((objData->pitch > 180.0f) || (objData->pitch < -180.0f)) {
            objData->unk1C = 0.0f - objData->unk1C;
        }
        if ((objData->roll > 90.0f) || (objData->roll < -90.0f)) {
            objData->unk24 = 0.0f - objData->unk24;
        }
        objData->roll += objData->unk24 * 3.0f;
        break;
    case 133: //unknown deleted object (0x85)
        player = get_player();
        if (player != NULL) {
            dx = player->positionMirror.f[0] - self->positionMirror.f[0];
            dz = player->positionMirror.f[2] - self->positionMirror.f[2];
            distance = sqrtf(SQ(dx) + SQ(dz));
            if ((distance < 400.0f) && (self->unkDC <= 0)) {

                index = 1;
                tempDLL2 = dll_load_deferred(0x1008, 1);

                while (index != 0){ 
                    tempDLL2->vtbl->func[0].withSixArgs((s32)self, 0, 0, 1, -1, 0);
                    index--;
                }
                
                dll_unload(tempDLL2);
                self->unkDC = rand_next(100, 200);
            } else if (distance < 200.0f) {
                self->unkDC -= 1;
            }
            if ((_data_0 != NULL) && (self->unkE0 == 0)) {
                self->unkE0 = 1;
                tempDLL = dll_load_deferred(0x200A, 1);
                tempDLL->vtbl->func[0].withSevenArgs((s32)self, 0, 0, 1, -1, 0xA, 0);
                if (tempDLL != NULL) {
                    dll_unload(tempDLL);
                }
            }
            if (_data_0 == NULL) {
                self->unkE0 = 0;
            }
        }
        break;
    case 134: //unknown deleted object (0x86)
        player = get_player();
        if (player != NULL) {
            dx = player->positionMirror.f[0] - self->positionMirror.f[0];
            dz = player->positionMirror.f[2] - self->positionMirror.f[2];
            distance = sqrtf(SQ(dx) + SQ(dz));
            if (self->unkDC != 0) {
                self->unkDC -= gUpdateRate;
                if (self->unkDC <= 0) {
                    tempDLL = dll_load_deferred(0x2009, 1);
                    tempDLL->vtbl->func[0].withSevenArgs((s32)_data_0, 0, 0, 1, -1, 9, 0);
                    if (tempDLL != NULL) {
                        dll_unload(tempDLL);
                    }
                    _data_0 = NULL;
                    self->unkDC = 0;
                    self->unkE0 = 100;
                }
            } else if ((distance <= 10.0f) && (_data_0 == NULL) && (self->unkE0 == 0)) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_1D2_Roar, 0x7F, NULL, NULL, 0, NULL);
                _data_0 = self;
                self->unkDC = 0x46;
            } else if ((distance < 40.0f) && (self->unkE0 == 0) && (self->unkDC == 0)) {
                gDLL_3_Animation->vtbl->func17(0, self, -1);
            }
            if (--self->unkE0 <= 0) {
                self->unkE0 = 0;
            }
        }
        break;
    case OBJ_DIM2IceFloe2: //10d
        objData->timer -= gUpdateRate;
        if (objData->timer < 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, objData->soundIDs[rand_next(0, objData->lastSoundIndex)], 0x7F, NULL, NULL, 0, NULL);
            objData->timer = objData->minCooldown;
            objData->timer += rand_next(0, objData->minCooldown);
        }
        break;
    case OBJ_SB_Lamp: //0x125
        self->srt.roll = -camera->srt.roll * 1.5;
        player = get_player();
        dx = player->positionMirror.x - self->positionMirror.x;
        dz = player->positionMirror.z - self->positionMirror.z;
        dy = player->positionMirror.y - self->positionMirror.y;
        distance = sqrtf(SQ(dx) + SQ(dz) + SQ(dy));
        if ((distance < 75.0f) && (objData->lampBool == TRUE)) {
            objData->lampBool = FALSE;
            func_80000450(self, self, 0x5C, 0, 0, 0);
        } else if ((distance > 75.0f) && (objData->lampBool == FALSE)) {
            objData->lampBool = TRUE;
            func_80000450(self, self, 0x5D, 0, 0, 0);
        }
        break;
    }
}
#endif

// offset: 0x1D28 | func: 2 | export: 2
void dll_331_update(Object *self) { }

// offset: 0x1D34 | func: 3
s32 dll_331_func_1D34(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3) {
    GenProps_Data* objData;
    s32 index;

    objData = self->data;

    animObjData->unk62 = 0;
    animObjData->unk7A = -1;
    
    switch (self->id) {
        case 133: //unknown deleted object
        case 134: //unknown deleted object
        case OBJ_SB_Galleon:
        case OBJ_DFdebris1:
        case OBJ_DFturbinelever:
        case OBJ_DIM2IceFloe2:
        case OBJ_GPSHswapstone:
            break;
        case OBJ_NWbigrock:
            for (index = 0; index < animObjData->unk98; index++){
                if (animObjData->unk8E[index] == 1) {
                    gDLL_17_partfx->vtbl->spawn(self, 0x44, NULL, 2, -1, NULL);
                }
                animObjData->unk8E[index] = 0;
            }        
            break;
        case OBJ_WMlargerock:
            if (main_get_bits(objData->gamebitB)) {
                animObjData->unk9D |= 4;
            }
            
            for (index = 0; index < animObjData->unk98; index++){
                if (animObjData->unk8E[index] == 1) {
                    gDLL_17_partfx->vtbl->spawn(self, 0x44, NULL, 2, -1, NULL);
                }
                animObjData->unk8E[index] = 0;
            }
            
            break;
    }
    return 0;
}

// offset: 0x1F2C | func: 4 | export: 3
void dll_331_print(Object* self, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle** arg4, s8 visibility) {
    if (!visibility || self->id == OBJ_DFP_PowerBolt){
        return;
    }
    if (!visibility || self->id == OBJ_VFP_PowerBolt){
        return;
    }
    if (!visibility || self->id == OBJ_scalessword){
        return;
    }
    
    draw_object(self, arg1, arg2, arg3, arg4, 1.0f);
}

// offset: 0x1FB8 | func: 5 | export: 4
void dll_331_free(Object* self, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x2000 | func: 6 | export: 5
u32 dll_331_get_model_flags(Object *self){
    return MODFLAGS_1;
}

// offset: 0x2010 | func: 7 | export: 6
u32 dll_331_get_data_size(Object *self, u32 a1){
    return sizeof(GenProps_Data);
}
