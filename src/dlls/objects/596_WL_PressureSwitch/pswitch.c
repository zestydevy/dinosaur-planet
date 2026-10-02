#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/607_WL_LevelControl.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "types.h"

//NOTE: has a different field layout to other pressure switches' setup structs
typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;              //NOTE: Signed, unlike the other pressure switches' yaw setups
/*1A*/ s16 unused1A;
/*1C*/ s16 gameBitPressed;  //Gamebit to set when the switch is pressed down (NOTE: at 0x1C here, instead of 0x1A)
} WLPressureSwitch_Setup;

typedef struct {
/*00*/ u32 soundHandle;
/*04*/ s8 pressedTimer;
/*05*/ s8 state;
} WLPressureSwitch_Data;

typedef enum {
    STATE_0_UP,
    STATE_1_DOWN
} WLPressureSwitchStates;

static int WLPressureSwitch_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void WLPressureSwitch_ctor(void* dll) { }

// offset: 0xC | dtor
void WLPressureSwitch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WLPressureSwitch_obj_Setup(Object* self, WLPressureSwitch_Setup* setup, s32 reset) {
    WLPressureSwitch_Data* objdata = self->data;

    self->animCallback = WLPressureSwitch_animCallback;
    self->srt.yaw = setup->yaw << 8;

    if (mainGetBits(setup->gameBitPressed)) {
        self->srt.transl.y = setup->base.y - 25.0f;
        objdata->pressedTimer = 30;
    }

    objdata->state = STATE_0_UP;
}

// offset: 0xB4 | func: 1 | export: 1
void WLPressureSwitch_obj_Control(Object* self) {
    Object* sidekick;
    Object* player;
    f32 deltaY;
    Object* listedObject;
    WLPressureSwitch_Setup* setup;
    WLPressureSwitch_Data* objdata;
    s8 playSound;
    s8 playerIsFarAway;
    s32 index;

    player = objGetPlayer();
    setup = (WLPressureSwitch_Setup*)self->setup;
    objdata = self->data;

    playerIsFarAway = FALSE;
    if (vec3Distance(&self->globalPosition, &player->globalPosition) > 100.0f) {
        playerIsFarAway = TRUE;
    }

    //Decrement timer until not considered pressed (fps-dependent)
    objdata->pressedTimer--;
    if (objdata->pressedTimer < 0) {
        objdata->pressedTimer = 0;
        objdata->state = STATE_0_UP;
    }

    //Handle objects on/near the switch
    if (self->polyhits->unk10F > 0) {
        for (index = 0; index < self->polyhits->unk10F; index++) {
            listedObject = (Object*)self->polyhits->unk100[index];
            deltaY = listedObject->srt.transl.y - self->srt.transl.y;
            if (deltaY > 8.9f) {
                objdata->pressedTimer = 5; //considered pressed for next 5 updates
            }
            if (objdata->state == STATE_0_UP && 
                listedObject && listedObject->id == OBJ_WL_Column_Top) {
                if (!playerIsFarAway) {
                    dll_amSfx->Play(self, SOUND_B89_Puzzle_Solved, 0x7F, NULL, 0, 0, 0);
                }
                objdata->state = STATE_1_DOWN;
            }
        }
    //Handle Tricky's behaviour during Sabre's first visit
    } else if (dll_gplay->get_act(self->mapID) == WM_ACT3_Spirit2_Sabre_DB) {
        sidekick = objGetSidekick();
        if (sidekick && vec3Distance(&self->globalPosition, &sidekick->globalPosition) < 50.0f) {
            objdata->pressedTimer = 5;
        }
    }

    //Handle the column piece puzzle during Krystal's first visit
    if ((dll_gplay->get_act(self->mapID) == WM_ACT1_Krystal_Meeting_Randorn) && !playerIsFarAway) {
        if (objdata->pressedTimer) {
            deltaY = setup->base.y - self->srt.transl.y;
            // Set a gamebit partway through descending, playing a sequence of Randorn's hall door opening
            if (2.5f < deltaY && deltaY < 5.0f) {
                mainSetBits(BIT_WM_Seq_446_LookAt_Randorn_Hall_Door, TRUE);
            } else if (mainGetBits(BIT_WM_Seq_446_LookAt_Randorn_Hall_Door)) {
                mainSetBits(BIT_WM_Seq_446_LookAt_Randorn_Hall_Door, FALSE);
            }
        } else if (mainGetBits(BIT_WM_Seq_446_LookAt_Randorn_Hall_Door)) {
            mainSetBits(BIT_WM_Seq_446_LookAt_Randorn_Hall_Door, FALSE);
        }
    }

    //Animate the switch's y coordinate
    playSound = FALSE;
    if (objdata->pressedTimer) {
        deltaY = setup->base.y - 5.0f;
        if (self->srt.transl.y < deltaY) {
            self->srt.transl.y += 0.25f * gUpdateRateF;
            if (self->srt.transl.y > deltaY) {
                self->srt.transl.y = deltaY;
            }
            playSound = FALSE;
            mainSetBits(setup->gameBitPressed, TRUE);
        } else {
            self->srt.transl.y -= 0.125f * gUpdateRateF;
            if (self->srt.transl.y < deltaY) {
                self->srt.transl.y = deltaY;
                mainSetBits(setup->gameBitPressed, TRUE);
                playSound = FALSE;
            } else {
                playSound = TRUE;
            }
        }
    } else {
        self->srt.transl.y += 0.125f * gUpdateRateF;
        if (self->srt.transl.y > setup->base.y) {
            self->srt.transl.y = setup->base.y;
        } else {
            playSound = TRUE;
        }

        mainSetBits(setup->gameBitPressed, FALSE);
    }

    //Play stone rumbling sound when moving
    if (playSound) {
        if (!objdata->soundHandle) {
            dll_amSfx->Play(self, SOUND_1E1_Stone_Moving_Loop, MAX_VOLUME, &objdata->soundHandle, 0, 0, 0);
        }
    } else {
        if (objdata->soundHandle) {
            dll_amSfx->Stop(objdata->soundHandle);
            objdata->soundHandle = 0;
        }
    }
}

// offset: 0x598 | func: 2 | export: 2
void WLPressureSwitch_obj_Update(Object* self) { }

// offset: 0x5A4 | func: 3 | export: 3
void WLPressureSwitch_obj_Print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x5F8 | func: 4 | export: 4
void WLPressureSwitch_obj_Free(Object* self, s32 onlySelf) {
    WLPressureSwitch_Data* objdata = self->data;

    if (objdata->soundHandle) {
        dll_amSfx->Stop(objdata->soundHandle);
    }
}

// offset: 0x654 | func: 5 | export: 5
u32 WLPressureSwitch_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x664 | func: 6 | export: 6
u32 WLPressureSwitch_obj_GetDataSize(Object* self, s32 offsetAddr) {
    return sizeof(WLPressureSwitch_Data);
}

// offset: 0x678 | func: 7
static int WLPressureSwitch_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    animData->unk7A = -1;
    animData->unk62 = 0;

    return 0;
}
