#include "dlls/engine/6_amsfx.h"
#include "game/objects/object.h"
#include "sys/controller.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/211_tricky.h"
#include "game/gamebits.h"
#include "dll.h"
#include "sys/main.h"
#include "sys/objects.h"

typedef struct {
    u8 state;
    u8 doneDemo;
    u8 demoState;
    f32 timer;
    GplayStruct11 *gplaystruct;
} NWtricky_Data;

typedef struct {
    ObjSetup base;
    u8 unk18[0x23-0x18];
    u8 unk23;
} GroundAnimator_Setup;

typedef enum {
    STATE_0,
    STATE_1,
    STATE_2
} NWtricky_State;

typedef enum {
    STATE2_0,
    STATE2_1,
    STATE2_2
} NWtricky_State2;

// time for tricky to call for help
#define NWTRICKY_MAX_TIMER_1 600.0f
#define NWTRICKY_MAX_TIMER_2 2000.0f

static int NWtricky_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void NWtricky_ctor(void *dll) { }

// offset: 0xC | dtor
void NWtricky_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void NWtricky_setup(Object *self, ObjSetup *setup, s32 arg2) {
    NWtricky_Data *objdata;
    Object *tricky;

    objdata = self->data;
    objdata->gplaystruct = gDLL_29_Gplay->vtbl->func_F30();
    objdata->doneDemo = FALSE;
    objdata->demoState = STATE2_0;
    if (!main_get_bits(BIT_SnowHorn_Tutorial_Defeated_SharpClaw)) {
        main_set_bits(BIT_4E3, 0xFF);
        objdata->state = STATE_0;
    } else {
        objdata->state = STATE_2;
    }
    if (!main_get_bits(BIT_4D4)) {
        tricky = get_sidekick();
        if (tricky) {
            ((DLL_211_Tricky*)tricky->dll)->vtbl->base.func22(tricky, self);
            main_set_bits(BIT_4D4, 1);
        }
    }
    self->animCallback = (AnimationCallback)NWtricky_anim_callback;
}

// offset: 0x134 | func: 1 | export: 1
void NWtricky_control(Object *self) {
    NWtricky_Data *objdata;
    Object *tricky;
    Object *player;
    Object *trickyballGroundAnimator;
    GroundAnimator_Setup *gaSetup;

    objdata = self->data;
    tricky = get_sidekick();
    if (tricky) {
        switch (objdata->state) {
        case STATE_0:
            if (main_get_bits(BIT_SnowHorn_Tutorial_Defeated_SharpClaw)) {
                main_set_bits(BIT_8, 1);
                main_set_bits(BIT_4E4, 1);
                objdata->state = STATE_2;
            } else if (((DLL_211_Tricky*)tricky->dll)->vtbl->func24(tricky) != 0) {
                objdata->state = STATE_1;
                objdata->timer = 0.0f;
            }
            break;
        case STATE_1:
            if (main_get_bits(BIT_SnowHorn_Tutorial_Defeated_SharpClaw)) {
                ((DLL_211_Tricky*)tricky->dll)->vtbl->base.func21(tricky, 0, 0);
                gDLL_6_AMSFX->vtbl->func_A6C(tricky);
                main_set_bits(BIT_4E3, 0);
                objdata->state = STATE_2;
            } else {
                objdata->timer += delayFloat;
                if (objdata->timer >= NWTRICKY_MAX_TIMER_1) {
                    objdata->timer -= NWTRICKY_MAX_TIMER_1;
                    gDLL_6_AMSFX->vtbl->play_sound(tricky, SOUND_222_NW_Tricky_Sharpclaw_Help, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
            }
            break;
        case STATE_2:
            objdata->timer += delayFloat;
            if (objdata->timer >= NWTRICKY_MAX_TIMER_2) {
                if (main_get_bits(BIT_4E3) == 0xFF) {
                    objdata->timer = 0.0f;
                    if (objdata->gplaystruct->unk0x0 < 4) {
                        main_set_bits(BIT_4E3, 1);
                    } else if (!main_get_bits(BIT_111) && main_get_bits(BIT_4E4)) {
                        player = get_player();
                        // get GroundAnimator object for the hole containing tricky's ball
                        trickyballGroundAnimator = func_800211B4(6021);
                        gaSetup = (GroundAnimator_Setup*)trickyballGroundAnimator->setup;
                        if ((vec3_distance_squared(&trickyballGroundAnimator->positionMirror, &player->positionMirror) <= SQ(gaSetup->unk23)) && (vec3_distance_squared(&player->positionMirror, &tricky->positionMirror) <= 10000.0f)) {
                            gDLL_6_AMSFX->vtbl->play_sound(tricky, SOUND_4BC, MAX_VOLUME, NULL, NULL, 0, NULL);
                            gDLL_22_Subtitles->vtbl->func_368(0xBE);
                        }
                    }
                }
            }
            if (main_get_bits(BIT_25)) {
                main_set_bits(BIT_3F8, 1);
            }
            break;
        }
    }
}

// offset: 0x538 | func: 2 | export: 2
void NWtricky_update(Object *self) { }

// offset: 0x544 | func: 3 | export: 3
void NWtricky_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x55C | func: 4 | export: 4
void NWtricky_free(Object *self, s32 a1) {
    if (main_get_bits(BIT_SnowHorn_Tutorial_Defeated_SharpClaw) && !main_get_bits(BIT_4E3)) {
        main_set_bits(BIT_4E3, 0xFF);
        main_set_bits(BIT_4E4, 1);
    }
}

// offset: 0x5EC | func: 5 | export: 5
u32 NWtricky_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x5FC | func: 6 | export: 6
u32 NWtricky_get_data_size(Object *self, u32 a1) {
    return sizeof(NWtricky_Data);
}

// offset: 0x610 | func: 7
int NWtricky_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    NWtricky_Data *objdata;
    Object *tricky;
    s32 i;
    s32 buttonMask;

    objdata = self->data;
    buttonMask = 0;

    if (!objdata->doneDemo) {
        tricky = get_sidekick();
        ((DLL_211_Tricky*)tricky->dll)->vtbl->base.func14(tricky, 1);
        ((DLL_211_Tricky*)tricky->dll)->vtbl->base.func14(tricky, 2);
        ((DLL_211_Tricky*)tricky->dll)->vtbl->base.func14(tricky, 3);
        ((DLL_211_Tricky*)tricky->dll)->vtbl->base.func14(tricky, 4);
        ((DLL_211_Tricky*)tricky->dll)->vtbl->base.func14(tricky, 5);
        ((DLL_211_Tricky*)tricky->dll)->vtbl->base.func14(tricky, 0);
        switch (objdata->demoState) {
        case STATE2_0:
            for (i = 0; i < animObjData->unk98; i++) {
                if (animObjData->unk8E[i] == 3)
                    objdata->demoState = STATE2_1;
            }
            break;
        case STATE2_1:
            get_button_presses(0);
            for (i = 0; i < animObjData->unk98; i++) {
                if (animObjData->unk8E[i] == 4) {
                    objdata->demoState = STATE2_2;
                    break;
                } else if (animObjData->unk8E[i] == 1)
                    buttonMask = 0x4; // simulate C-Down press?
            }
            buttonMask |= get_button_presses(0) & 0x4;
            break;
        case STATE2_2:
            for (i = 0; i < animObjData->unk98; i++) {
                if (animObjData->unk8E[i] == 2)
                    buttonMask = 0x8000; // simulate A press?
            }
            buttonMask |= get_button_presses(0) & 0x8000;
            if (buttonMask & 0x8000) {
                objdata->doneDemo = TRUE;
            }
            break;
        }
        gDLL_1_UI->vtbl->ui_func_6984(buttonMask);
    } else {
        gDLL_1_UI->vtbl->ui_func_6984(-1);
    }

    return 0;
}

/*0x0*/ static const char str_0[] = "menu start\n";
/*0xC*/ static const char str_C[] = "menu cbuttons %d\n";
/*0x20*/ static const char str_20[] = "menu a button\n";
