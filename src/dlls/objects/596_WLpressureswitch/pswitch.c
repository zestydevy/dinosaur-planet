#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/gamebits.h"

typedef struct {
/*00*/ u32 soundHandle;
/*04*/ s8 pressed;
/*05*/ s8 stateIndex;
} WLPressureSwitchState;

typedef struct {
ObjCreateInfo base;
s8 yaw;
s16 unused1A;
s16 gameBitPressed;             //flag to set when switch is pressed down
} WLPressureSwitchCreateInfo;

typedef struct {
s8 unk0[0x62 - 0];
u8 unk62;
s8 unk63[0x7A - 0x63];
s16 unk7A;
s8 unk7C[0x8D - 0x7C];
u8 unk8D;
} CallbackBCUnkArg2;

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

static s32 WLpressureswitch_callbackBC(s32 self, s32 createInfo, CallbackBCUnkArg2* arg2, s32 arg3);

// offset: 0x0 | ctor
void WLpressureswitch_ctor(void* dll){
}

// offset: 0xC | dtor
void WLpressureswitch_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void WLpressureswitch_create(Object* self, WLPressureSwitchCreateInfo* createInfo, s32 arg2) {
    WLPressureSwitchState* state;

    state = self->state;
    self->unk0xbc = (void*)&WLpressureswitch_callbackBC;
    self->srt.yaw = createInfo->yaw << 8;
    if (get_gplay_bitstring(createInfo->gameBitPressed)) {
        self->srt.transl.y = createInfo->base.y - 25.0f;
        state->pressed = 30;
    }
    state->stateIndex = STATE_0_UP;
}

// offset: 0xB4 | func: 1 | export: 1
void WLpressureswitch_update(Object* self) {
    Object* sidekick;
    Object* player;
    f32 deltaY;
    Object* listedObject;
    WLPressureSwitchCreateInfo* createInfo;
    WLPressureSwitchState* state;
    s8 playSound;
    s8 playerIsFarAway;
    s32 index;

    player = get_player();
    createInfo = (WLPressureSwitchCreateInfo*)self->createInfo;
    state = self->state;

    playerIsFarAway = FALSE;
    if (vec3_distance(&self->positionMirror, &player->positionMirror) > 100.0f) {
        playerIsFarAway = TRUE;
    }

    //Decrement timer until not considered pressed (fps-dependent)
    state->pressed--;
    if (state->pressed < 0) {
        state->pressed = 0;
        state->stateIndex = STATE_0_UP;
    }

    //Handle objects on/near the switch
    if (self->unk0x58->unk10f > 0) {
        for (index = 0; index < self->unk0x58->unk10f; index++){
            listedObject = (Object*)self->unk0x58->unk100[index];
            deltaY = listedObject->srt.transl.y - self->srt.transl.y;
            if (deltaY > 8.9f) {
                state->pressed = 5; //considered pressed for next 5 updates
            }
            if (state->stateIndex == STATE_0_UP && 
                listedObject && listedObject->id == OBJ_WL_Column_Top) {
                if (!playerIsFarAway) {
                    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B89_Puzzle_Solved, 0x7F, NULL, 0, 0, 0);
                }
                state->stateIndex = STATE_1_DOWN;
            }
        }
    //Handle Tricky's behaviour during Sabre's first visit
    } else if (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID) == WM_ACT_3_SPIRIT_2_SABRE_DB) {
        sidekick = get_sidekick();
        if (sidekick && vec3_distance(&self->positionMirror, &sidekick->positionMirror) < 50.0f) {
            state->pressed = 5;
        }
    }

    //Handle the column piece puzzle during Krystal's first visit
    if (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID) == WM_ACT_1_KRYSTAL_MEETS_WITH_RANDORN
             && !playerIsFarAway) {
        if (state->pressed) {
            deltaY = createInfo->base.y - self->srt.transl.y;
            if (2.5f < deltaY && deltaY < 5.0f) {
                set_gplay_bitstring(BIT_905_WM_Randorn_Hall_Opened, TRUE);
            } else if (get_gplay_bitstring(BIT_905_WM_Randorn_Hall_Opened)) {
                set_gplay_bitstring(BIT_905_WM_Randorn_Hall_Opened, FALSE);
            }
        } else if (get_gplay_bitstring(BIT_905_WM_Randorn_Hall_Opened)) {
            set_gplay_bitstring(BIT_905_WM_Randorn_Hall_Opened, FALSE);
        }
    }

    //Animate the switch's y coordinate
    playSound = FALSE;
    if (state->pressed) {
        deltaY = createInfo->base.y - 5.0f;
        if (self->srt.transl.y < deltaY) {
            self->srt.transl.y += 0.25f * delayFloat;
            if (deltaY < self->srt.transl.y) {
                self->srt.transl.y = deltaY;
            }
            playSound = FALSE;
            set_gplay_bitstring(createInfo->gameBitPressed, TRUE);
        } else {
            self->srt.transl.y -= 0.125f * delayFloat;
            if (self->srt.transl.y < deltaY) {
                self->srt.transl.y = deltaY;
                set_gplay_bitstring(createInfo->gameBitPressed, TRUE);
                playSound = FALSE;
            } else {
                playSound = TRUE;
            }
        }
    } else {
        self->srt.transl.y += 0.125f * delayFloat;
        deltaY = createInfo->base.y;
        if (deltaY < self->srt.transl.y) {
            self->srt.transl.y = deltaY;
        } else {
            playSound = TRUE;
        }
        set_gplay_bitstring(createInfo->gameBitPressed, FALSE);
    }

    //Play stone rumbling sound when moving
    if (playSound) {
        if (!state->soundHandle) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_1e1_Stone_Moving, 0x7F, (u32*)&state->soundHandle, 0, 0, 0);
        }
    } else {
        if (state->soundHandle) {
            gDLL_6_AMSFX->vtbl->func_A1C(state->soundHandle);
            state->soundHandle = 0;
        }
    }
}

// offset: 0x598 | func: 2 | export: 2
void WLpressureswitch_func_598(Object* self){
}

// offset: 0x5A4 | func: 3 | export: 3
void WLpressureswitch_draw(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x5F8 | func: 4 | export: 4
void WLpressureswitch_destroy(Object* self, s32 arg1) {
    WLPressureSwitchState* state = self->state;
    u32 soundHandle;

    soundHandle = state->soundHandle;
    if (soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(soundHandle);
    }
}

// offset: 0x654 | func: 5 | export: 5
s32 WLpressureswitch_get_setup_flags(Object* self){
    return 0;
}

// offset: 0x664 | func: 6 | export: 6
s32 WLpressureswitch_get_state_size(Object* self, s32 arg1){
    return sizeof(WLPressureSwitchState);
}

// offset: 0x678 | func: 7
s32 WLpressureswitch_callbackBC(s32 arg0, s32 arg1, CallbackBCUnkArg2* arg2, s32 arg3) {
    arg2->unk7A = -1;
    arg2->unk62 = 0;
    return 0;
}
