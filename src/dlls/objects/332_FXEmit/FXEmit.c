#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/332_FXEmit.h"
#include "sys/dll.h"
#include "game/gamebits.h"
#include "sys/gfx/modgfx.h"
#include "sys/gfx/projgfx.h"
#include "sys/main.h"
#include "sys/rand.h"
#include "dll.h"

#define USE_DEFAULT_SPEED 0x7F

typedef enum {
    BANK_ParticleFX = 0,
    BANK_ModelFX = 1,
    BANK_ProjectileFX = 2
} FXEmit_Banks;

typedef enum {
    Preset_0 = 0,   //PARTFXFLAG_2
    Preset_1 = 1,   //PARTFXFLAG_4
    Preset_2 = 2,   //PARTFXFLAG_1 (plus PARTFXFLAG_200000 if the effect is from the partFX bank)
    Preset_3 = 3    //PARTFXFLAG_NONE
} FXEmit_Flag_Presets;

typedef struct {
/*00*/ f32 activationRange;
/*04*/ f32 translateX;
/*08*/ s16 bank;                //where the effect is stored 
                                //0: partfx DLL (#17)
                                //1) modgfx DLLs
                                //2) projgfx DLLs
/*0A*/ s16 indexInBank;         //index of the effect within its bank
/*0C*/ s16 defaultFXIndex;      //only used if fxCount <= 0, always set to 0
/*0E*/ s16 fxRate;              //when positive: how many times the emitter runs per tick
                                //when negative: number of frames to waits between single emit
/*10*/ u16 _unk10;              //unused
/*12*/ s16 randomDelay;
/*14*/ s16 toggleGamebit;
/*16*/ s16 disableGamebit;
/*18*/ s16 disabled;
/*1A*/ s16 intervalTimer;
/*1C*/ u8 animCallbackRotate;   //switches particle rotation on/off when used in sequences
} FXEmit_Data;

static int FXEmit_anim_callback(Object *, Object *, AnimObj_Data *, s8);
static void FXEmit_emit(Object *self);

// offset: 0x0 | ctor
void FXEmit_ctor(void *dll) { }

// offset: 0xC | dtor
void FXEmit_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void FXEmit_setup(Object *self, FXEmit_Setup *setup, s32 arg2) {
    FXEmit_Data *objdata = (FXEmit_Data*)self->data;

    self->srt.yaw = 0;
    self->animCallback = FXEmit_anim_callback;

    objdata->activationRange = setup->activationRange * 4;
    objdata->bank = setup->bank;
    objdata->indexInBank = setup->indexInBank;
    objdata->fxRate = setup->fxRate;
    self->srt.scale = 0.1f;
    objdata->toggleGamebit = setup->toggleGamebit;
    objdata->disableGamebit = setup->disableGamebit;
    objdata->disabled = FALSE;

    if (objdata->fxRate <= 0) {
        self->unkDC = objdata->fxRate;
    } else {
        self->unkDC = 0;
    }

    if (objdata->disableGamebit != NO_GAMEBIT && main_get_bits(objdata->disableGamebit)) {
        objdata->disabled = TRUE;
    }

    self->srt.yaw = setup->yaw << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.roll = setup->roll << 8;
    objdata->intervalTimer = setup->interval * 100;
    objdata->translateX = self->srt.transl.x;
    objdata->randomDelay = rand_next(0, 10);
    objdata->defaultFXIndex = 0;
}

// offset: 0x174 | func: 1 | export: 1
void FXEmit_control(Object* self) {
    FXEmit_Data* objdata;
    Vec3f vectorToPlayer;
    s32 _pad;
    FXEmit_Setup* setup;
    Object* player;

    objdata = (FXEmit_Data*)self->data;
    setup = (FXEmit_Setup*)self->setup;
    
    if (objdata->randomDelay) {
        objdata->randomDelay -= (s16)gUpdateRateF;
        if (objdata->randomDelay < 0) {
            objdata->randomDelay = 0;
        }
    } else {
        self->srt.transl.x += self->speed.x * gUpdateRateF;
        self->srt.transl.y += self->speed.y * gUpdateRateF;
        self->srt.transl.z += self->speed.z * gUpdateRateF;
        self->positionMirror.x = self->srt.transl.x;
        self->positionMirror.y = self->srt.transl.y;
        self->positionMirror.z = self->srt.transl.z;

        player = get_player();
        if (!player || !setup) {
            return;
        }

        //(Optional) Activate at intervals, playing sound effect
        if (setup->interval && (setup->interval != 0xFF)) {
            if (objdata->intervalTimer <= 0) {
                objdata->disabled = FALSE;
                objdata->intervalTimer = setup->interval * 100;
                if (setup->intervalSoundID) {
                    gDLL_6_AMSFX->vtbl->play_sound(self, setup->intervalSoundID, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
            } else {
                objdata->disabled = TRUE;
            }
            objdata->intervalTimer -= gUpdateRate;
        }

        //Handle rotations
        if (setup->yawSpeed == USE_DEFAULT_SPEED) {
            self->srt.yaw += gUpdateRate * 10;
        } else {
            self->srt.yaw += setup->yawSpeed * gUpdateRate * 100;
        }

        if (setup->pitchSpeed == USE_DEFAULT_SPEED) {
            self->srt.pitch += gUpdateRate * 10;
        } else {
            self->srt.pitch += setup->pitchSpeed * gUpdateRate * 100;
        }

        if (setup->rollSpeed == USE_DEFAULT_SPEED) {
            self->srt.roll += gUpdateRate * 10;
        } else {
            self->srt.roll += setup->rollSpeed * gUpdateRate * 100;
        }

        //Bail if not enabled
        if (((objdata->toggleGamebit != NO_GAMEBIT) && main_get_bits(objdata->toggleGamebit) == FALSE) 
            || objdata->disabled) {
          return;
        }

        //Check if should be disabled (@bug?: continues with this tick when disabled, causing 1 frame flash)
        if (objdata->disableGamebit != NO_GAMEBIT && main_get_bits(objdata->disableGamebit)) {
            objdata->disabled = TRUE;
        }
        if (setup->interval == 0xFF) {
            objdata->disabled = TRUE;
        }

        if ((objdata->fxRate >= 0) || ((objdata->fxRate < 0) && (self->unkDC <= 0))) {
            vectorToPlayer.f[0] = self->positionMirror.x - player->positionMirror.x;
            vectorToPlayer.f[1] = self->positionMirror.y - player->positionMirror.y;
            vectorToPlayer.f[2] = self->positionMirror.z - player->positionMirror.z;
            if (objdata->fxRate == 0) {
                objdata->disabled = TRUE;
            }

            if (VECTOR_MAGNITUDE(vectorToPlayer) <= objdata->activationRange || objdata->activationRange == 0.0f) {
                FXEmit_emit(self);
            }

            self->unkDC = -objdata->fxRate;
            return;
        }
        if (objdata->fxRate < 0) {
            if (self->unkDC > 0) {
                self->unkDC -= gUpdateRate;
            }
        }
    }
}

// offset: 0x608 | func: 2 | export: 2
void FXEmit_update(Object *self) { }

// offset: 0x614 | func: 3 | export: 3
void FXEmit_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x62C | func: 4 | export: 4
void FXEmit_free(Object *self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
    gDLL_14_Modgfx->vtbl->func4(self);
}

// offset: 0x69C | func: 5 | export: 5
u32 FXEmit_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x6AC | func: 6 | export: 6
u32 FXEmit_get_data_size(Object *self, u32 a1) {
    return sizeof(FXEmit_Data);
}

// offset: 0x6C0 | func: 7
static int FXEmit_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    FXEmit_Data *objdata;
    FXEmit_Setup *setup;
    s32 i;

    objdata = (FXEmit_Data*)self->data;
    setup = (FXEmit_Setup*)self->setup;
    
    for (i = 0; i < animObjData->unk98; i++) {
        if (animObjData->unk8E[i] == 1) {
            FXEmit_emit(self);
        }
        if (animObjData->unk8E[i] == 2) {
            objdata->animCallbackRotate = 1 - objdata->animCallbackRotate;
        }
        animObjData->unk8E[i] = 0;
    }
    
    if (objdata->animCallbackRotate) {
        if (setup->yawSpeed == USE_DEFAULT_SPEED) {
            self->srt.yaw += gUpdateRate * 10;
        } else {
            self->srt.yaw += setup->yawSpeed * gUpdateRate * 100;
        }

        if (setup->pitchSpeed == USE_DEFAULT_SPEED) {
            self->srt.pitch += gUpdateRate * 10;
        } else {
            self->srt.pitch += setup->pitchSpeed * gUpdateRate * 100;
        }

        if (setup->rollSpeed == USE_DEFAULT_SPEED) {
            self->srt.roll += gUpdateRate * 10;
        } else {
            self->srt.roll += setup->rollSpeed * gUpdateRate * 100;
        }

        FXEmit_emit(self);
    }
    return 0;
}

// offset: 0x8E8 | func: 8
/** Creates the effect(s) */
static void FXEmit_emit(Object *self) {
    FXEmit_Data *objdata;
    FXEmit_Setup *setup;
    s32 flags;
    DLL_IModgfx *modfxDLL;
    DLL_IProjgfx *projfxDLL;
    s16 i;
    SRT srt;

    objdata = self->data;
    setup = (FXEmit_Setup*)self->setup;

    flags = PARTFXFLAG_NONE;
    switch (setup->flagConfig) {
    case Preset_3:
        break;
    default:
        flags = PARTFXFLAG_2;
        break;
    case Preset_0:
        if (objdata->bank == BANK_ParticleFX) {
            flags = PARTFXFLAG_2;
        }
        if (objdata->bank == BANK_ModelFX) {
            flags = PARTFXFLAG_2;
        }
        if (objdata->bank == BANK_ProjectileFX) {
            flags = PARTFXFLAG_2;
        }
        break;
    case Preset_1:
        if (objdata->bank == BANK_ParticleFX) {
            flags = PARTFXFLAG_4;
        }
        if (objdata->bank == BANK_ModelFX) {
            flags = PARTFXFLAG_4;
        }
        if (objdata->bank == BANK_ProjectileFX) {
            flags = PARTFXFLAG_4;
        }
        break;
    case Preset_2:
        if (objdata->bank == BANK_ParticleFX) {
            flags = PARTFXFLAG_200000 + PARTFXFLAG_1;
        }
        if (objdata->bank == BANK_ModelFX) {
            flags = PARTFXFLAG_1;
        }
        if (objdata->bank == BANK_ProjectileFX) {
            flags = PARTFXFLAG_1;
        }
        break;
    }

    if (flags & PARTFXFLAG_1) {
        srt.transl.x = self->srt.transl.x;
        srt.transl.y = self->srt.transl.y;
        srt.transl.z = self->srt.transl.z;
        srt.yaw = self->srt.yaw;
        srt.roll = self->srt.roll;
        srt.pitch = self->srt.pitch;
        srt.scale = 1.0f;
        if (objdata->fxRate > 0) {
            for (i = 0; i < objdata->fxRate; i++) {
                gDLL_17_partfx->vtbl->spawn(self, objdata->indexInBank, &srt, flags, -1, NULL);
            }
        } else {
            gDLL_17_partfx->vtbl->spawn(self, objdata->defaultFXIndex, &srt, flags, -1, NULL);
        }
    } else {
        if (objdata->bank == BANK_ParticleFX) {
            if (objdata->fxRate > 0) {
                for (i = 0; i < objdata->fxRate; i++) {
                    gDLL_17_partfx->vtbl->spawn(self, objdata->indexInBank, NULL, flags, -1, NULL);
                }
            } else {
                gDLL_17_partfx->vtbl->spawn(self, objdata->indexInBank, NULL, flags, -1, NULL);
            }
        } else if (objdata->bank == BANK_ModelFX) {
            modfxDLL = dll_load_deferred((objdata->indexInBank + 0x1000), 1);
            if (objdata->fxRate > 0) {
                for (i = 0; i < objdata->fxRate; i++) {
                    modfxDLL->vtbl->func0(self, 0, 0, flags, -1, 0);
                }
            } else {
                modfxDLL->vtbl->func0(self, 0, 0, flags, -1, 0);
            }
            dll_unload(modfxDLL);
        } else if (objdata->bank == BANK_ProjectileFX) {
            projfxDLL = dll_load_deferred((objdata->indexInBank + 0x2000), 1);
            if (objdata->fxRate > 0) {
                for (i = 0; i < objdata->fxRate; i++) {
                    projfxDLL->vtbl->func0(self, 0, 0, flags, -1, objdata->indexInBank, 0);
                }
            } else {
                projfxDLL->vtbl->func0(self, 0, 0, flags, -1, objdata->indexInBank, 0);
            }
            dll_unload(projfxDLL);
        }
    }
}
