#include "dll.h"
#include "game/objects/interaction_arrow.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/rand.h"
#include "sys/joypad.h"
#include "sys/segment_1460.h"
#include "sys/objprint.h"
#include "sys/gfx/modgfx.h"
#include "game/gamebits.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/modgfx/129.h"
#include "dlls/modgfx/130.h"
#include "dlls/modgfx/140.h"

typedef struct {
/*00*/ DLL_IModgfx *dll129;           //DLL 129
/*04*/ DLL_IModgfx *dll130;           //DLL 130
/*08*/ DLL_IModgfx *mGfxKrazoaPoints; //DLL 140, handles cylindrical glows randomly emanating from Krazoa symbol's 6 points
/*0C*/ f32 unkC;
/*10*/ s16 intervalDLL129;      //Waiting period between each time DLL 129 fires
/*12*/ s16 intervalDLL130;      //Waiting period between each time DLL 130 fires
/*14*/ s16 particleCount;       //[OUTBOUND] The number of blue glow particles to create per frame
/*16*/ s16 timerDLL129;         //Countdown until DLL 129 fires
/*18*/ s16 timerDLL130;         //Countdown until DLL 130 fires
/*1A*/ s16 unk1A;
/*1C*/ s16 durationPoweringUp;  //Length of Transporter's power-up sequence / frames until the Transporter fires
/*1E*/ s16 unk1E;
/*20*/ s16 timerWarpPlayer;     //[OUTBOUND] Countdown to warping player away (after powering up finished)
/*22*/ s16 timerEnvFxAction;    //[OUTBOUND] Countdown to applying envFX  (after powering up finished)
/*24*/ u8 isPoweringUp;         //The Transporter power-up sequence is active (ends when a blue magic explosion effect ripples out from base)
/*25*/ u8 isOutbound;           //Whether the player is warping into/away from the Transporter
/*26*/ u8 runTimersPostPowerUp; //[OUTBOUND] Enables countdown to applying enxFX / warping player away (after powering up finished)
} Transporter_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 unk19;
/*1A*/ s8 warpID;
/*1B*/ u8 unk1B[0x20 - 0x1B];
/*20*/ s16 gamebitEnabled;    //When specified, the Transporter will only be powered on when this gamebit is set
} Transporter_Setup;

static int Transporter_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void Transporter_ctor(void *dll) { }

// offset: 0xC | dtor
void Transporter_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Transporter_setup(Object *self, Transporter_Setup *setup, s32 reset) {
    Transporter_Data *objdata;

    objdata = self->data;
    self->srt.yaw = 0;
    objdata->dll129 = NULL;
    objdata->dll130 = NULL;
    objdata->mGfxKrazoaPoints = NULL;
    objdata->intervalDLL129 = 600;
    objdata->intervalDLL130 = 600;
    objdata->particleCount = 10;
    objdata->timerDLL129 = 0;
    objdata->timerDLL130 = 0;
    objdata->unk1A = 10;
    objdata->durationPoweringUp = 400;
    objdata->timerWarpPlayer = 0;
    objdata->unk1E = 2;
    objdata->unkC = 0.01f;
    self->srt.yaw = (f32)(setup->yaw << 8); // ?
    self->unkDC = 0;
    self->unkE0 = 150;
    self->animCallback = Transporter_anim_callback;
}

// offset: 0xBC | func: 1 | export: 1
void Transporter_control(Object *self) {
    Camera *camera;
    s16 i;
    Object *player;
    Vec3f delta;
    f32 distToPlayer;
    f32 mag1;
    SRT transform;
    Transporter_Setup *setup;
    Transporter_Data *objdata;

    objdata = self->data;
    setup = (Transporter_Setup*)self->setup;

    //Show a tutorial when approaching a Transporter for the first time
    if ((self->unkAF & ARROW_FLAG_4_Highlighted) && !mainGetBits(BIT_Shown_ZLock_Interact_Message)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
        mainSetBits(BIT_Shown_ZLock_Interact_Message, TRUE);
        return;
    }

    camUseShake();

    player = objGetPlayer();
    if (player == NULL) {
        return;
    }

    //Handle the player being nearby, while the warp has yet to begin
    distToPlayer = vec3DistanceXZ(&player->globalPosition, &self->globalPosition);
    if ((objdata->isOutbound == FALSE) && (objdata->isPoweringUp == FALSE) && (distToPlayer < 40.0f)) {
        if (!objdata->mGfxKrazoaPoints) {
            objdata->mGfxKrazoaPoints = dllLoad(DLL_ID_140, 1);
        }

        if (D_800B4A5E >= 0) {
            //Set up inbound warp
            objdata->isOutbound = FALSE;
            player->opacity = 0;
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            self->unkDC = objdata->durationPoweringUp;
            if (!objdata->dll129) {
                objdata->dll129 = dllLoad(DLL_ID_129, 1);
            }
            if (!objdata->dll130) {
                objdata->dll130 = dllLoad(DLL_ID_130, 1);
            }
            objdata->isPoweringUp = TRUE;
            D_80092A78 = 2;
            objdata->runTimersPostPowerUp = FALSE;
        } else {
            //Set up outbound warp when the player interacts with the Transporter's LockIcon
            if (((setup->gamebitEnabled == NO_GAMEBIT) || mainGetBits(setup->gamebitEnabled)) && 
                (self->unkAF & ARROW_FLAG_1_Interacted)
            ) {
                joyDisableButtons(0, A_BUTTON);
                func_80000860(self, self, 187, 0);
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                objdata->isOutbound = TRUE;
                self->unkDC = objdata->durationPoweringUp;
                if (!objdata->dll129) {
                    objdata->dll129 = dllLoad(DLL_ID_129, 1);
                }
                if (!objdata->dll130) {
                    objdata->dll130 = dllLoad(DLL_ID_130, 1);
                }
                objdata->isPoweringUp = TRUE;
                objdata->runTimersPostPowerUp = TRUE;
            }
        }

        //Emanate a beam of light from a randomly-selected point on the Krazoa symbol (2.5% chance) 
        if (((setup->gamebitEnabled == NO_GAMEBIT) || mainGetBits(setup->gamebitEnabled)) && 
            (mathRnd(0, 40) == 0)) {
            objdata->mGfxKrazoaPoints->vtbl->func0(self, mathRnd(0, 5), NULL, 1, -1, NULL);
        }
    }

    //Handle effects while Transporter is powering up
    if (objdata->isPoweringUp && (self->unkDC > 0)) {
        //Handle modGfx and their timers
        {
            objdata->timerDLL129 -= gUpdateRate;
            objdata->timerDLL130 -= gUpdateRate;

            if (objdata->timerDLL129 <= 0) {
                objdata->dll129->vtbl->func0(self, 0, NULL, 1, -1, NULL);
                objdata->timerDLL129 = objdata->intervalDLL129;
            }

            if (objdata->isOutbound && (objdata->timerDLL130 <= 0)) {
                objdata->dll130->vtbl->func0(self, 0, NULL, 1, -1, NULL);
                objdata->timerDLL130 = objdata->intervalDLL130;
            }
        }

        //Get camera-to-Transporter vector
        {
            camera = camGet();
            delta.f[0] = self->globalPosition.x - camera->tx;
            delta.f[1] = self->globalPosition.y - camera->ty;
            delta.f[2] = self->globalPosition.z - camera->tz;
            mag1 = VECTOR_MAGNITUDE(delta);
            if (mag1 != 0.0f) {
                delta.f[0] /= mag1; delta.f[0] *= 20.0f;
                delta.f[1] /= mag1; delta.f[1] *= 20.0f;
                delta.f[2] /= mag1; delta.f[2] *= 20.0f;
            }
        }
        
        //Create particles after 2.5 seconds
        {
            transform.transl.x = player->globalPosition.x;
            transform.transl.y = player->globalPosition.y;
            transform.transl.z = player->globalPosition.z;
            transform.yaw = 0;
            transform.roll = 0;
            transform.pitch = 0;
            transform.scale = 1.0f;

            //Run continuously after reaching timer threshold
            if (self->unkE0 <= 0) {
                if (objdata->isOutbound) {
                    //Create small blue glows, positioned randomly along the vertical warp beam
                    for (i = 0; i < objdata->particleCount; i++) {
                        gDLL_17_partfx->vtbl->spawn(player, PARTICLE_79, NULL, PARTFXFLAG_4, -1, NULL);
                    }
                    camSetShakeOffset(mathRnd(0, 10) * 0.1f);
                } else if ((objdata->isOutbound == FALSE) && (self->unkDC < 200)) {
                    camSetShakeOffset(mathRnd(0, 10) * 0.05f);
                }

                //Create wide ripples periodically, with a randomised vertical offset
                transform.transl.x = delta.f[0] + self->globalPosition.x;
                transform.transl.y = delta.f[1] + self->globalPosition.y;
                transform.transl.z = delta.f[2] + self->globalPosition.z;
                if ((self->unkDC % 10) < (s16)gUpdateRateF) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_78, &transform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                }
            } else {
                self->unkE0 -= gUpdateRate;

                //Run once, upon reaching timer threshold
                if ((self->unkE0 <= 0) && (objdata->isOutbound == FALSE)) {
                    transform.transl.x = player->globalPosition.x;
                    transform.transl.y = player->globalPosition.y;
                    transform.transl.z = player->globalPosition.z;
                    transform.scale = 1.0f;
                    transform.yaw = 0;
                    transform.roll = 0;
                    transform.pitch = 0;
                    camSetShakeOffset(8.0f);
                    gDLL_6_AMSFX->vtbl->play(NULL, SOUND_43D_Transporter_Fire, MAX_VOLUME, NULL, NULL, 0, NULL);
                    
                    //Create horizontal blast particle spreading out from base of Transporter
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_75, NULL, PARTFXFLAG_2, -1, NULL);
                }
            }
        }

        self->unkDC -= gUpdateRate;
    } else if (objdata->isPoweringUp && (self->unkDC <= 0)) {
        //Transporter finished powering up

        transform.transl.x = player->globalPosition.x;
        transform.transl.y = player->globalPosition.y;
        transform.transl.z = player->globalPosition.z;
        transform.yaw = 0;
        transform.roll = 0;
        transform.pitch = 0;
        transform.scale = 1.0f;

        if (objdata->isOutbound) {
            //Fire the transporter
            camSetShakeOffset(8.0f);
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_43D_Transporter_Fire, MAX_VOLUME, NULL, NULL, 0, NULL);
            
            //Create horizontal blast particle spreading out from base of Transporter
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_75, NULL, PARTFXFLAG_2, -1, NULL);
            
            //Create lots of tiny sparkles that wander outwards
            for (i = 0; i < 80; i++) {
                gDLL_17_partfx->vtbl->spawn(player, PARTICLE_77, NULL, PARTFXFLAG_NONE, -1, NULL);
            }
        } else {
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_9B_Transporter_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
            
            //Create lots of tiny sparkles that wander outwards before gravitating back inwards
            for (i = 0; i < 100; i++) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_7A, NULL, PARTFXFLAG_NONE, -1, NULL);
            }
        }

        //Unload modGfxDLLs
        dllFree(objdata->dll129);
        objdata->dll129 = NULL;
        dllFree(objdata->dll130);
        objdata->dll130 = NULL;

        self->unkDC = 0;
        self->unkE0 = 150;

        objdata->timerDLL129 = 0;
        objdata->timerDLL130 = 0;
        objdata->unk1A = 10;
        objdata->unk1E = 2;
        objdata->unkC = 0.01f;
        objdata->isPoweringUp = FALSE;
        objdata->timerWarpPlayer = 550;
        objdata->timerEnvFxAction = 550;
    }

    //Count down to applying an envFxAction (outbound only)
    if (objdata->timerEnvFxAction && objdata->runTimersPostPowerUp) {
        objdata->timerEnvFxAction -= gUpdateRate;
        if (objdata->timerEnvFxAction <= 0) {
            func_80000860(self, self, D_80092A7C[0], 0);
            objdata->timerEnvFxAction = 0;
        }
    }

    //Count down to warping the player away (outbound only)
    if (objdata->timerWarpPlayer && objdata->runTimersPostPowerUp) {
        objdata->timerWarpPlayer -= gUpdateRate;
        if (objdata->timerWarpPlayer <= 0) {
            mapWarpPlayer(setup->warpID, FALSE);
            objdata->runTimersPostPowerUp = FALSE;
            objdata->timerWarpPlayer = 0;
        }
    }

    //Set opacity of player's weapon
    if (player && player->linkedObject && player->opacity < OBJECT_OPACITY_MAX) {
        player->linkedObject->opacity = 0;
    } else if (player && player->linkedObject) {
        player->linkedObject->opacity = OBJECT_OPACITY_MAX;
    }

    self->srt.yaw += 1000;
}

// offset: 0xC00 | func: 2 | export: 2
void Transporter_update(Object *self) {
    Transporter_Setup *setup;

    setup = (Transporter_Setup*)self->setup;
    if ((setup->gamebitEnabled != NO_GAMEBIT) && (mainGetBits(setup->gamebitEnabled) == FALSE)) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    } else {
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    }

    if (self->unk74) {
        objprintUpdateLockIconCoords(self);
    }
}

// offset: 0xCA0 | func: 3 | export: 3
void Transporter_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xCB8 | func: 4 | export: 4
void Transporter_free(Object *self, s32 a1) {
    Transporter_Data *objdata;

    objdata = self->data;
    gDLL_13_Expgfx->vtbl->func5(self);
    if (objdata->dll129) {
        dllFree(objdata->dll129);
    }
    if (objdata->dll130) {
        dllFree(objdata->dll130);
    }
    if (objdata->mGfxKrazoaPoints) {
        dllFree(objdata->mGfxKrazoaPoints);
    }
}

// offset: 0xD68 | func: 5 | export: 5
u32 Transporter_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xD78 | func: 6 | export: 6
u32 Transporter_get_data_size(Object *self, u32 a1) {
    return sizeof(Transporter_Data);
}

// offset: 0xD8C | func: 7
int Transporter_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    return 0;
}
