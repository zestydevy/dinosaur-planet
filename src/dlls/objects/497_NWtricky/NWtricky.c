#include "macros.h"
#include "game/gamebits.h"
#include "game/gametexts.h"
#include "game/objects/object.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/214_animobj.h"

typedef struct {
    u8 state;
    u8 doneDemo;
    u8 demoState;
    f32 timer;
    SidekickStats *sidekickStats;
} NWtricky_Data;

typedef struct {
    ObjSetup base;
    u8 unk18[0x23 - 0x18];
    u8 unk23; //Find command range?
} GroundAnimator_Setup;

typedef enum {
    STATE_0_Initial,
    STATE_1_Chased_by_SharpClaw,
    STATE_2_Learning_Sidekick_Commands
} NWtricky_State;

typedef enum {
    NWtricky_DEMO_STATE_Initial,
    NWtricky_DEMO_STATE_Show_Inventory,
    NWtricky_DEMO_STATE_Close_Inventory
} NWtricky_State2;

// Time for Tricky to call out while being chased by a SharpClaw
#define NWTRICKY_INVERVAL_CALL_FOR_HELP 600.0f

// Time for Tricky to offer hint
#define NWTRICKY_INVERVAL_OFFER_HINT 2000.0f

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
    objdata->sidekickStats = gDLL_29_Gplay->vtbl->get_sidekick_stats();
    objdata->doneDemo = FALSE;
    objdata->demoState = NWtricky_DEMO_STATE_Initial;

    if (!main_get_bits(BIT_SnowHorn_Tutorial_Defeated_SharpClaw)) {
        main_set_bits(BIT_4E3, 0xFF);
        objdata->state = STATE_0_Initial;
    } else {
        objdata->state = STATE_2_Learning_Sidekick_Commands;
    }

    if (!main_get_bits(BIT_4D4)) {
        tricky = get_sidekick();
        if (tricky) {
            ((DLL_ISidekick*)tricky->dll)->vtbl->func22(tricky, self);
            main_set_bits(BIT_4D4, 1);
        }
    }

    self->animCallback = NWtricky_anim_callback;
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
    if (tricky == NULL) {
        return;
    }

    switch (objdata->state) {
    case STATE_0_Initial:
        if (main_get_bits(BIT_SnowHorn_Tutorial_Defeated_SharpClaw)) {
            main_set_bits(BIT_8, 1);
            main_set_bits(BIT_Tricky_Unlocked_Sidekick_Commands, 1);
            objdata->state = STATE_2_Learning_Sidekick_Commands;
        } else if (((DLL_ISidekick*)tricky->dll)->vtbl->func24(tricky) != 0) {
            objdata->state = STATE_1_Chased_by_SharpClaw;
            objdata->timer = 0.0f;
        }
        break;

    case STATE_1_Chased_by_SharpClaw:
        if (main_get_bits(BIT_SnowHorn_Tutorial_Defeated_SharpClaw)) {
            ((DLL_ISidekick*)tricky->dll)->vtbl->func21(tricky, 0, 0);
            gDLL_6_AMSFX->vtbl->stop_object(tricky);
            main_set_bits(BIT_4E3, 0);
            objdata->state = STATE_2_Learning_Sidekick_Commands;
        } else {
            //Call out while being chased
            objdata->timer += gUpdateRateF;
            if (objdata->timer >= NWTRICKY_INVERVAL_CALL_FOR_HELP) {
                objdata->timer -= NWTRICKY_INVERVAL_CALL_FOR_HELP;
                gDLL_6_AMSFX->vtbl->play(tricky, SOUND_222_NW_Tricky_Sharpclaw_Help, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        }
        break;

    case STATE_2_Learning_Sidekick_Commands:
        objdata->timer += gUpdateRateF;
        if (objdata->timer >= NWTRICKY_INVERVAL_OFFER_HINT) {
            if (main_get_bits(BIT_4E3) == 0xFF) {
                objdata->timer = 0.0f;
                if (objdata->sidekickStats->blueFood < 4) {
                    main_set_bits(BIT_4E3, 1);
                } else if (!main_get_bits(BIT_SW_Tricky_Toy_Unearthed) && main_get_bits(BIT_Tricky_Unlocked_Sidekick_Commands)) {
                    player = get_player();

                    //Get GroundAnimator object for the hole containing Tricky's ball
                    trickyballGroundAnimator = func_800211B4(0x1785); //search by uID

                    //@bug: missing null check
                    gaSetup = (GroundAnimator_Setup*)trickyballGroundAnimator->setup;

                    //Offer a hint if Tricky and the player stay at the toy's dig spot for a while
                    if ((vec3_distance_squared(
                            &trickyballGroundAnimator->globalPosition,
                            &player->globalPosition) <= SQ(gaSetup->unk23)) && 
                        (vec3_distance_squared(
                            &player->globalPosition, 
                            &tricky->globalPosition) <= 10000.0f)
                    ) {
                        gDLL_6_AMSFX->vtbl->play(tricky, SOUND_4BC_Tricky_Dig_EMPTY, MAX_VOLUME, NULL, NULL, 0, NULL);
                        gDLL_22_Subtitles->vtbl->func_368(GAMETEXT_0BE_SW_Tricky_Tutorial_Hint);
                    }
                }
            }
        }

        if (main_get_bits(BIT_SW_Tricky_Ball_Collected)) {
            main_set_bits(BIT_Tricky_Ball_Unlocked, 1);
        }
        break;
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
        main_set_bits(BIT_Tricky_Unlocked_Sidekick_Commands, 1);
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
        ((DLL_ISidekick*)tricky->dll)->vtbl->func14(tricky, Sidekick_Command_INDEX_1_Find);
        ((DLL_ISidekick*)tricky->dll)->vtbl->func14(tricky, Sidekick_Command_INDEX_2_Distract);
        ((DLL_ISidekick*)tricky->dll)->vtbl->func14(tricky, Sidekick_Command_INDEX_3_Guard);
        ((DLL_ISidekick*)tricky->dll)->vtbl->func14(tricky, Sidekick_Command_INDEX_4_Flame);
        ((DLL_ISidekick*)tricky->dll)->vtbl->func14(tricky, Sidekick_Command_INDEX_5_Play);
        ((DLL_ISidekick*)tricky->dll)->vtbl->func14(tricky, Sidekick_Command_INDEX_0_Heel);

        switch (objdata->demoState) {
        case NWtricky_DEMO_STATE_Initial:
            STUBBED_PRINTF("menu start\n");
            for (i = 0; i < animObjData->unk98; i++) {
                if (animObjData->unk8E[i] == 3)
                    objdata->demoState = NWtricky_DEMO_STATE_Show_Inventory;
            }
            break;

        case NWtricky_DEMO_STATE_Show_Inventory:
            STUBBED_PRINTF("menu cbuttons %d\n", joy_get_pressed_raw(0));
            for (i = 0; i < animObjData->unk98; i++) {
                if (animObjData->unk8E[i] == 4) {
                    objdata->demoState = NWtricky_DEMO_STATE_Close_Inventory;
                    break;
                } else if (animObjData->unk8E[i] == 1)
                    buttonMask = D_CBUTTONS; // simulate C-Down press
            }

            //Get player's C-button presses as well
            buttonMask |= joy_get_pressed_raw(0) & D_CBUTTONS;
            break;

        case NWtricky_DEMO_STATE_Close_Inventory:
            STUBBED_PRINTF("menu a button\n");
            for (i = 0; i < animObjData->unk98; i++) {
                if (animObjData->unk8E[i] == 2)
                    buttonMask = A_BUTTON; // simulate A press
            }

            //Get player's A presses as well
            buttonMask |= joy_get_pressed_raw(0) & A_BUTTON;

            if (buttonMask & A_BUTTON) {
                objdata->doneDemo = TRUE;
            }
            break;
        }

        gDLL_1_cmdmenu->vtbl->set_buttons_override(buttonMask);
    } else {
        gDLL_1_cmdmenu->vtbl->set_buttons_override(CMDMENU_CLEAR_BUTTONS_OVERRIDE);
    }

    return 0;
}
