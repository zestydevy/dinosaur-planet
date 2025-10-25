#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/214_animobj.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "types.h"

typedef struct {
/*00*/ u32 soundHandle;
/*04*/ s8 pressed;
/*05*/ s8 state;
} WLPressureSwitch_Data;

typedef struct {
ObjSetup base;
s8 yaw;
s16 unused1A;
s16 gameBitPressed;             //flag to set when switch is pressed down
} WLPressureSwitch_Setup;

enum WLPressureSwitchStates {
    STATE_0_UP = 0,
    STATE_1_DOWN = 1
};

enum WarlockMountainActs {
    WM_ACT_1_KRYSTAL_MEETS_WITH_RANDORN = 1,
    WM_ACT_2_SPIRIT_1_KRYSTAL_DF = 2,
    WM_ACT_3_SPIRIT_2_SABRE_DB = 3,
    WM_ACT_4_SPIRIT_3_KRYSTAL_MMP = 4,
    WM_ACT_5_SPIRIT_4_SABRE_WC = 5,
    WM_ACT_6_SPIRIT_5_KRYSTAL_CC = 6,
    WM_ACT_7_SPIRIT_6_SABRE_WG = 7,
    WM_ACT_8_SPIRIT_7_KRYSTAL_GP = 8,
    WM_ACT_9_SPIRIT_8_SABRE_SW = 9
};

static int WLpressureswitch_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3);

// offset: 0x0 | ctor
void WLpressureswitch_ctor(void* dll){
}

// offset: 0xC | dtor
void WLpressureswitch_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void WLpressureswitch_setup(Object* self, WLPressureSwitch_Setup* setup, s32 arg2) {
    WLPressureSwitch_Data* objdata;

    objdata = self->data;
    self->animCallback = (AnimationCallback)WLpressureswitch_anim_callback;
    self->srt.yaw = setup->yaw << 8;
    if (main_get_bits(setup->gameBitPressed)) {
        self->srt.transl.y = setup->base.y - 25.0f;
        objdata->pressed = 30;
    }
    objdata->state = STATE_0_UP;
}

// offset: 0xB4 | func: 1 | export: 1
void WLpressureswitch_control(Object* self) {
    Object* sidekick;
    Object* player;
    f32 deltaY;
    Object* listedObject;
    WLPressureSwitch_Setup* setup;
    WLPressureSwitch_Data* objdata;
    s8 playSound;
    s8 playerIsFarAway;
    s32 index;

    player = get_player();
    setup = (WLPressureSwitch_Setup*)self->setup;
    objdata = self->data;

    playerIsFarAway = FALSE;
    if (vec3_distance(&self->positionMirror, &player->positionMirror) > 100.0f) {
        playerIsFarAway = TRUE;
    }

    //Decrement timer until not considered pressed (fps-dependent)
    objdata->pressed--;
    if (objdata->pressed < 0) {
        objdata->pressed = 0;
        objdata->state = STATE_0_UP;
    }

    //Handle objects on/near the switch
    if (self->unk58->unk10f > 0) {
        for (index = 0; index < self->unk58->unk10f; index++){
            listedObject = (Object*)self->unk58->unk100[index];
            deltaY = listedObject->srt.transl.y - self->srt.transl.y;
            if (deltaY > 8.9f) {
                objdata->pressed = 5; //considered pressed for next 5 updates
            }
            if (objdata->state == STATE_0_UP && 
                listedObject && listedObject->id == OBJ_WL_Column_Top) {
                if (!playerIsFarAway) {
                    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B89_Puzzle_Solved, 0x7F, NULL, 0, 0, 0);
                }
                objdata->state = STATE_1_DOWN;
            }
        }
    //Handle Tricky's behaviour during Sabre's first visit
    } else if (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID) == WM_ACT_3_SPIRIT_2_SABRE_DB) {
        sidekick = get_sidekick();
        if (sidekick && vec3_distance(&self->positionMirror, &sidekick->positionMirror) < 50.0f) {
            objdata->pressed = 5;
        }
    }

    //Handle the column piece puzzle during Krystal's first visit
    if (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID) == WM_ACT_1_KRYSTAL_MEETS_WITH_RANDORN
             && !playerIsFarAway) {
        if (objdata->pressed) {
            deltaY = setup->base.y - self->srt.transl.y;
            if (2.5f < deltaY && deltaY < 5.0f) {
                main_set_bits(BIT_WM_Randorn_Hall_Opened, TRUE);
            } else if (main_get_bits(BIT_WM_Randorn_Hall_Opened)) {
                main_set_bits(BIT_WM_Randorn_Hall_Opened, FALSE);
            }
        } else if (main_get_bits(BIT_WM_Randorn_Hall_Opened)) {
            main_set_bits(BIT_WM_Randorn_Hall_Opened, FALSE);
        }
    }

    //Animate the switch's y coordinate
    playSound = FALSE;
    if (objdata->pressed) {
        deltaY = setup->base.y - 5.0f;
        if (self->srt.transl.y < deltaY) {
            self->srt.transl.y += 0.25f * delayFloat;
            if (deltaY < self->srt.transl.y) {
                self->srt.transl.y = deltaY;
            }
            playSound = FALSE;
            main_set_bits(setup->gameBitPressed, TRUE);
        } else {
            self->srt.transl.y -= 0.125f * delayFloat;
            if (self->srt.transl.y < deltaY) {
                self->srt.transl.y = deltaY;
                main_set_bits(setup->gameBitPressed, TRUE);
                playSound = FALSE;
            } else {
                playSound = TRUE;
            }
        }
    } else {
        self->srt.transl.y += 0.125f * delayFloat;
        deltaY = setup->base.y;
        if (deltaY < self->srt.transl.y) {
            self->srt.transl.y = deltaY;
        } else {
            playSound = TRUE;
        }
        main_set_bits(setup->gameBitPressed, FALSE);
    }

    //Play stone rumbling sound when moving
    if (playSound) {
        if (!objdata->soundHandle) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_1e1_Stone_Moving, 0x7F, (u32*)&objdata->soundHandle, 0, 0, 0);
        }
    } else {
        if (objdata->soundHandle) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
            objdata->soundHandle = 0;
        }
    }
}

// offset: 0x598 | func: 2 | export: 2
void WLpressureswitch_update(Object* self){
}

// offset: 0x5A4 | func: 3 | export: 3
void WLpressureswitch_print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x5F8 | func: 4 | export: 4
void WLpressureswitch_free(Object* self, s32 arg1) {
    WLPressureSwitch_Data* objdata = self->data;
    u32 soundHandle;

    soundHandle = objdata->soundHandle;
    if (soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(soundHandle);
    }
}

// offset: 0x654 | func: 5 | export: 5
u32 WLpressureswitch_get_model_flags(Object* self){
    return MODFLAGS_NONE;
}

// offset: 0x664 | func: 6 | export: 6
u32 WLpressureswitch_get_data_size(Object* self, s32 arg1){
    return sizeof(WLPressureSwitch_Data);
}

// offset: 0x678 | func: 7
static int WLpressureswitch_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    animObjData->unk7A = -1;
    animObjData->unk62 = 0;
    return 0;
}
