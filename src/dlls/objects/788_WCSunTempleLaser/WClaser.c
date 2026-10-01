#include "common.h"
#include "game/gamebits.h"
#include "sys/gfx/modgfx.h"
#include "sys/objmsg.h"
#include "dlls/objects/210_player.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 mode;
    s16 offIntervalDuration;
    s16 firingDuration;
    s16 gamebitEnabled;
} WCSunTempleLaser_Setup;

typedef struct {
    Texture* texLaser;
    f32 timerDelay;
    f32 timerFiring;
    f32 timerOffInterval;
    u32 soundHandle;
    u8 _unk14[0x2C - 0x14];
    f32 whirVolume;
    f32 _unk30;
    u8 laserIsActive;
    u8 skipStateCheck;
    s8 hurtRange;
    s8 playerZapCooldown;
    s16 timerStateCheckDelay;
    s16 timerDelayThreshold;
    SRT playerKnockbackDir;
    u8 state;
    u8 unkPlayerMessage;
    u8 beamStartHissPlayed;
    u8 flags;
} WCSunTempleLaser_Data;

typedef enum {
    WCSunTempleLaser_Flag_Firing = 1
} WCSunTempleLaser_Flags;

typedef enum {
    WCSunTempleLaser_STATE_0_Off,
    WCSunTempleLaser_STATE_1_Started,
    WCSunTempleLaser_STATE_2_Stopping
} WCSunTempleLaser_States;

typedef enum {
    WCSunTempleLaser_MODE_Always_On = 0,    //Sun-themed, doesn't cycle on/off
    WCSunTempleLaser_MODE_Timed_Blue = 1,   //Moon-themed
    WCSunTempleLaser_MODE_Timed_Yellow = 2  //Sun-themed
} WCSunTempleLaser_Modes;

/*0x0*/ static DLL_IModgfx* dModGfxDLL = NULL;

// offset: 0x0 | ctor
void WCSunTempleLaser_ctor(void* dll) { }

// offset: 0xC | dtor
void WCSunTempleLaser_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCSunTempleLaser_obj_Setup(Object* self, WCSunTempleLaser_Setup* objSetup, s32 reset) {
    WCSunTempleLaser_Data* objData = self->data;
    
    objInitMesgQueue(self, 2);
    
    self->srt.yaw = objSetup->yaw << 8;
    
    objData->timerDelay = mathRnd(-80, 80) + 400;
    
    dModGfxDLL = dllLoad(DLL_ID_145, 1);
    
    objData->timerDelayThreshold = 280;
    
    if (objData->texLaser == NULL) {
        objData->texLaser = texLoadTexture(TEXTABLE_2E);
    }
    
    objData->timerFiring = mathRnd(0, objSetup->firingDuration);
    objData->timerOffInterval = mathRnd(0, objSetup->offIntervalDuration);
}

// offset: 0x140 | func: 1 | export: 1
void WCSunTempleLaser_obj_Control(Object* self) {
    WCSunTempleLaser_Setup* objSetup;
    WCSunTempleLaser_Data* objData;
    s32 pad1[3];
    f32 sin;
    f32 cos;
    f32 worldOriginInObjectSpaceZ;
    f32 minusCos;
    f32 distance;
    f32 worldOriginInObjectSpaceX;
    s32 effectIdx;
    s32 pad2;
    Object* player;
    s32 i;
    f32 range;
    f32 radius;
    f32 dy;
    s32 pad3[2];
    f32 sp34;

    objSetup = (WCSunTempleLaser_Setup*)self->setup;
    objData = self->data;
    objData->timerDelay -= gUpdateRateF;
    
    if ((objSetup->mode != WCSunTempleLaser_MODE_Always_On) && (mainGetBits(BIT_WC_Moon_Temple_Hazards_Deactivated))) {
        mainSetBits(objSetup->gamebitEnabled, FALSE);
    }
    
    //Laser's State Machine (starting/stopping/waiting)
    if (mainGetBits(objSetup->gamebitEnabled)) {
        if (objData->flags & WCSunTempleLaser_Flag_Firing) {
            if (objSetup->mode != WCSunTempleLaser_MODE_Always_On) {
                objData->timerFiring -= gUpdateRateF;
            }
            
            if (objData->timerFiring < 0.0f) {
                //Switch off
                objData->timerOffInterval = objSetup->offIntervalDuration;
                objData->flags &= ~WCSunTempleLaser_Flag_Firing;
                
                if (objData->soundHandle != 0) {
                    dll_amSfx->Stop(objData->soundHandle);
                    objData->soundHandle = 0;
                }
                
                dll_modgfx->Func5(self);
                dll_amSfx->Play(self, SOUND_2BB_Laser_Stop_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
            } else {
                //Firing
                if (objData->timerDelay < 0.0f) {
                    objData->timerDelay = 275.0f;
                    objData->state = WCSunTempleLaser_STATE_0_Off;
                } else if (objData->timerDelay < objData->timerDelayThreshold) {
                    if (objData->state == WCSunTempleLaser_STATE_0_Off) {
                        //Start laser whirring sound loop
                        if (objData->soundHandle == 0) {
                            dll_amSfx->Play(self, SOUND_9FA_Laser_Whir_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
                        }
                        
                        //Play a hiss sound as the beam starts firing
                        if (objData->beamStartHissPlayed == FALSE) {
                            objData->beamStartHissPlayed = TRUE;
                            dll_amSfx->Play(self, SOUND_9F9_Laser_Startup_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
                        }
                        
                        objData->state = WCSunTempleLaser_STATE_1_Started;
                        
                        //Create modGfx
                        if (dModGfxDLL != NULL) {
                            effectIdx = (objSetup->mode == WCSunTempleLaser_MODE_Timed_Blue) ? 13 : 15;
                            dModGfxDLL->vtbl->Spawn(self, effectIdx, NULL, 0x10004, -1, NULL);
                        }
                    }
                    
                    if (objData->timerDelay < 140.0f) {
                        if (objData->state == WCSunTempleLaser_STATE_1_Started) {
                            objData->state = WCSunTempleLaser_STATE_2_Stopping;
                            if (dModGfxDLL != NULL) {
                                effectIdx = (objSetup->mode == WCSunTempleLaser_MODE_Timed_Blue) ? 13 : 16;
                                dModGfxDLL->vtbl->Spawn(self, effectIdx, NULL, 0x10004, -1, NULL);
                            }
                        }
                    } else if (objData->whirVolume <= 1.0f) {
                        objData->whirVolume += 0.052f * gUpdateRateF;
                    }
                }
            }
        } else if (objSetup->mode != WCSunTempleLaser_MODE_Always_On) {
            //Laser off, wait out the interval between blasts
            objData->timerOffInterval -= gUpdateRateF;
            if (objData->timerOffInterval <= 0.0f) {
                objData->timerFiring = objSetup->firingDuration;
                objData->flags |= WCSunTempleLaser_Flag_Firing;
                objData->state = WCSunTempleLaser_STATE_0_Off;
                objData->beamStartHissPlayed = FALSE;
                objData->timerDelay = 0.0f;
                objData->whirVolume = 0.0f;
            }
        } else {
            objData->flags |= WCSunTempleLaser_Flag_Firing;
        }
    } else {
        objData->state = WCSunTempleLaser_STATE_0_Off;
        objData->beamStartHissPlayed = FALSE;
        objData->timerOffInterval = 0.0f;
        objData->timerDelay = 0.0f;
        objData->whirVolume = 0.0f;
    }
    
    //Handle laser whir volume
    if (objData->laserIsActive) {
        if (objData->soundHandle != 0) {
            dll_amSfx->SetVol(objData->soundHandle, (s8) (objData->whirVolume * 127.0f));
        }
    }
    
    //Determining if the laser should hurt the player
    {
        sin = mathSinfInterp(self->srt.yaw);
        cos = mathCosfInterp(self->srt.yaw);
        worldOriginInObjectSpaceZ = -((self->srt.transl.x * sin) + (self->srt.transl.z * cos));
        
        player = objGetPlayer();
        
        objData->playerZapCooldown -= gUpdateRate;
        if (objData->playerZapCooldown < 0) {
            objData->playerZapCooldown = 0;
        }
        
        //Check the state var to determine if the laser is active, but only during the first second of the laser being active?
        {
            if (objData->laserIsActive) {
                objData->timerStateCheckDelay += gUpdateRate;
                if (objData->timerStateCheckDelay > 60) {
                    objData->timerStateCheckDelay = 60;
                    objData->skipStateCheck = TRUE;
                }
            }
            
            if (objData->skipStateCheck == FALSE) {
                objData->laserIsActive = objData->state & (WCSunTempleLaser_STATE_1_Started | WCSunTempleLaser_STATE_2_Stopping);
            } else {
                objData->laserIsActive = TRUE;
            }
        }
        
        //Check the laser's gamebit and its firing flag
        if ((mainGetBits(objSetup->gamebitEnabled) == FALSE) || (objData->flags & WCSunTempleLaser_Flag_Firing) == FALSE) {
            objData->laserIsActive = FALSE;
        }

        //Don't hurt the player if they were zapped recently, or if the laser's off
        if (player == NULL || objData->playerZapCooldown != 0 || objData->laserIsActive == FALSE) {
            return;
        }

        //Check if the player is close enough to be zapped by the laser
        range = objData->hurtRange + 5.0f;
        dy = player->srt.transl.y - self->srt.transl.y;
        if ((dy < range) && (-(range + 25.0f) < dy)) {

            //Check the player's position along the laser's objectSpace X axis (i.e. distance perpendicular to the beam)
            minusCos = -cos; 
            worldOriginInObjectSpaceX = -((self->srt.transl.x * minusCos) + (self->srt.transl.z * sin)); 
            distance = (player->srt.transl.x * minusCos) + (sin * player->srt.transl.z) + worldOriginInObjectSpaceX;

            if ((range > distance) && (distance > -range)) {

                //Check the player's position along the laser's objectSpace Z axis (i.e. distance along the beam)
                distance = (player->srt.transl.x * sin) + (cos * player->srt.transl.z) + worldOriginInObjectSpaceZ;
                if ((0.0f < distance) && (distance < 170.0f)) {
                    //Zap the player if they're not using the Forcefield Spell
                    if (((DLL_210_Player*)player->dll)->vtbl->func50(player) != BIT_Spell_Forcefield) {
                        dll_amSfx->Play(self, SOUND_228_Laser_Zap, MAX_VOLUME, NULL, NULL, 0, NULL);
                        
                        for (i = 0; i < 4; i++) {
                            gDLL_17_partfx->vtbl->spawn(objGetPlayer(), PARTICLE_28B, NULL, 4, -1, NULL);
                        }
                        
                        distance = ((player->prevLocalPosition.x * minusCos) + (sin * player->prevLocalPosition.z)) + worldOriginInObjectSpaceX;
                        if (distance < 0.0f) {
                            radius = -20.0f;
                        } else {
                            radius = 20.0f;
                        }

                        objData->playerKnockbackDir.transl.x = player->srt.transl.x + (minusCos * radius);
                        objData->playerKnockbackDir.transl.z = player->srt.transl.z + (sin * radius);
                        
                        if ((objData->unkPlayerMessage == 0) || (objData->unkPlayerMessage == 1)) {
                            //NOTE: casting an SRT* to an Object*, but the player code only reads the SRT portion when handling this message, so it's safe!
                            objSendMesg(player, 0x60003, (Object*)&objData->playerKnockbackDir, NULL);
                        }
                        
                        objData->playerZapCooldown = 20;
                    } else {
                        //Or if they are using the Forcefield Spell, have it change colour to show it's shielding damage
                        mainSetBits(BIT_468_Forcefield_Spell_Taking_Damage, TRUE);
                    }     
                }
            }
        }
    }
}

// offset: 0x9D4 | func: 2 | export: 2
void WCSunTempleLaser_obj_Update(Object* self) { }

// offset: 0x9E0 | func: 3 | export: 3
void WCSunTempleLaser_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x9F8 | func: 4 | export: 4
void WCSunTempleLaser_obj_Free(Object* self, s32 onlySelf) {
    WCSunTempleLaser_Data* objData = self->data;
    
    if (objData->soundHandle != 0) {
        dll_amSfx->Stop(objData->soundHandle);
    }
    
    dll_modgfx->Func5(self);
    
    dllFree(dModGfxDLL);

    if (objData->texLaser != NULL) {
        texFreeTexture(objData->texLaser);
    }
    objData->texLaser = NULL;
}

// offset: 0xABC | func: 5 | export: 5
u32 WCSunTempleLaser_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xACC | func: 6 | export: 6
u32 WCSunTempleLaser_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCSunTempleLaser_Data);
}
