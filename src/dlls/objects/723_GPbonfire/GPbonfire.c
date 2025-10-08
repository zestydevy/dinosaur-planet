#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "dlls/objects/210_player.h"
#include "dll.h"

typedef struct {
s8 unk0[0x62 - 0];
u8 unk62;
s8 unk63[0x7A - 0x63];
s16 unk7A;
s8 unk7C[0x8D - 0x7C];
u8 unk8D;
u8 unk8E[0x98 - 0x8E];
u8 unk98;
} CallbackBCUnkArg2;

typedef struct {
/*00*/ u8 stateIndex;
/*01*/ u8 currentState;
/*02*/ u8 previousState;
/*04*/ s32 sequenceIndexKindling;   //the cutscene for placing the kindling
/*08*/ s32 sequenceIndexBurning;    //the cutscene for Kyte starting the fire
/*0C*/ CurvesFunc1BCReturnInner* curves;
/*10*/ s16 gameBitKindlingPlaced;   //flag to set when kindling is ready to burn
/*12*/ s16 gameBitBurning;          //flag to set when the bonfire's burning
/*14*/ s32 unused14;
/*18*/ u32 soundHandles[2];         //burning sfx
/*20*/ s16 timer;                   //counts down until the bonfire dwindles by one stage
/*22*/ s8 weedsDeposited;
/*23*/ u8 updateFireEffect;         //replaces the modgfx fire effect when its size changes
/*24*/ u8 callbackBool;             //Boolean associated with the cutscene callback function
} GPBonfireState;

typedef struct {
/*00*/ ObjCreateInfo base;
/*18*/ u8 interactionDistance;
/*1A*/ u16 kyteCurveID;
/*1C*/ s16 unused1C;
/*1E*/ s8 unused1E;
/*1F*/ u8 yaw;
} GPBonfireCreateInfo;

enum GPBonfireStates{
    STATE_0_INITIALISE = 0,
    STATE_1_WAIT_FOR_PLAYER_INTERACTION = 1,
    STATE_2_WAIT_FOR_KYTE = 2,
    STATE_3_START_BURNING = 3,
    STATE_4_ADDING_TUMBLEWEEDS = 4
};

// enum AUDIO {
//     SOUND_1e1_Stone_Moving = 0x1e1,
//     SOUND_50a_Fire_Burning_Low_Loop = 0x50a,
//     SOUND_50b_Fire_Burning_High_Loop = 0x50b,
//     SOUND_99a_Mechanical_Ratcheting = 0x99a
// };

/** Scale values for the GPbonfire object itself as the fire is built up */
/*0x0*/ static f32 bonfireScaleData[] = {
    0.1, 0.14, 0.196, 0.2744, 0.38415999
};
/** Scale values for the modgfx fire effect as the fire is built up */
/*0x14*/ static f32 modgfxScaleData[] = {
    6, 12, 18, 24, 30
};

/** How long (in 60ths of a second) until the bonfire dwindles by one stage */
#define BONFIRE_DWINDLING_TIMER 2000

static void GPbonfire_func_A44(Object* self);
static s32 GPbonfire_callbackBC(Object* self, s32 arg1, CallbackBCUnkArg2* arg2, s32 arg3);

// offset: 0x0 | ctor
void GPbonfire_ctor(void* dll){
}

// offset: 0xC | dtor
void GPbonfire_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void GPbonfire_create(Object* self, GPBonfireCreateInfo* createInfo, s32 arg2) {
    GPBonfireState* state;

    state = self->state;
    self->unk0xbc = (void*)&GPbonfire_callbackBC;
    self->srt.yaw = createInfo->yaw << 8;
    state->stateIndex = STATE_0_INITIALISE;
    state->currentState = 0;
    state->soundHandles[0] = 0;
    state->soundHandles[1] = 0;
    obj_add_object_type(self, 0x30);
    state->gameBitKindlingPlaced = BIT_GP_Bonfire_Kindling_Placed;
    state->gameBitBurning = BIT_GP_Bonfire_Burning;
    state->sequenceIndexKindling = 0;
    state->sequenceIndexBurning = 1;
    state->unused14 = 0;
    state->weedsDeposited = 0;
}

// offset: 0xB8 | func: 1 | export: 1
void GPbonfire_update(Object* self) {
    GPBonfireState* state;
    GPBonfireCreateInfo* createInfo;
    Object* sidekick;
    Object* player;
    f32 distanceToTumbleweed;
    s32 weedIndex;
    s32 count;
    void** dll;
    u8 index;
    u8 playerIsNearby;
    Object** tumbleweeds;

    state = self->state;
    createInfo = (GPBonfireCreateInfo*)self->createInfo;
    player = get_player();

    playerIsNearby = vec3_distance_xz_squared(&player->positionMirror, &self->positionMirror) <= createInfo->interactionDistance * createInfo->interactionDistance;    

    state->currentState &= 0xFFFD;
    if (state->currentState & 1) {
        set_gplay_bitstring(state->gameBitKindlingPlaced, TRUE);
        state->stateIndex = STATE_2_WAIT_FOR_KYTE;
        state->currentState &= 0xFFFE;
        self->modelInstIdx = 1;
    }

    if (state->updateFireEffect) {
        self->srt.scale = bonfireScaleData[state->weedsDeposited];
        gDLL_14_Modgfx->vtbl->func10.withOneArg((s32)self);
        dll = dll_load_deferred(0x104B, 1);
        ((DLL_Unknown*)dll)->vtbl->func[0].withSixArgs((s32)self, modgfxScaleData[state->weedsDeposited], 0, 0x10004, -1, 0);
        dll_unload(dll);
        state->updateFireEffect = FALSE;
    }

    switch (state->stateIndex) {
        case STATE_0_INITIALISE:
            self->modelInstIdx = 0;
            state->stateIndex = STATE_1_WAIT_FOR_PLAYER_INTERACTION;
            if (get_gplay_bitstring(state->gameBitKindlingPlaced)) {
                self->modelInstIdx = 1;
                state->stateIndex = STATE_2_WAIT_FOR_KYTE;
            }
            if (get_gplay_bitstring(state->gameBitBurning)) {
                state->timer = BONFIRE_DWINDLING_TIMER;
                GPbonfire_func_A44(self);
            }
            break;
        case STATE_1_WAIT_FOR_PLAYER_INTERACTION:
            if (playerIsNearby && 
                ((DLL_210_Player*)player->dll)->vtbl->func48(player) == 0x4BB) {
                gDLL_3_Animation->vtbl->func17(state->sequenceIndexKindling, self, -1);
            }
            break;
        case STATE_2_WAIT_FOR_KYTE:
            sidekick = get_sidekick();
            if (sidekick && playerIsNearby) {
                ((DLL_Unknown*)sidekick->dll)->vtbl->func[14].withTwoArgs((s32)sidekick, 4);
                if (gDLL_1_UI->vtbl->func7(4)) {
                    set_gplay_bitstring(BIT_Kyte_Flight_Curve, createInfo->kyteCurveID);
                }
            }
            break;
        case STATE_3_START_BURNING:
            if (vec3_distance_xz_squared(&get_sidekick()->positionMirror, &self->positionMirror) <= 2500.0f) {
                gDLL_17->vtbl->func1(self, 0x425, NULL, 2, -1, NULL);
                gDLL_17->vtbl->func1(self, 0x426, NULL, 2, -1, NULL);
            }
            break;
        case STATE_4_ADDING_TUMBLEWEEDS:
            if (playerIsNearby) {
                state->currentState |= 2;
            }
            if ((state->currentState & 2) && !(state->previousState & 2)) {
                func_80000450(self, self, 0x59, 0, 0, 0);
            } else if (!(state->currentState & 2) && (state->previousState & 2)) {
                func_80000450(self, self, 0x5A, 0, 0, 0);
            }
            gDLL_17->vtbl->func1(self, 0x425, NULL, 2, -1, NULL);

            //Handle tumbleweeds (only whole ones, twigs don't count)
            tumbleweeds = obj_get_all_of_type(4, &count);
            for (weedIndex = 0; weedIndex < count; weedIndex++){
                if (tumbleweeds[weedIndex]->id == OBJ_Tumbleweed3) {
                    distanceToTumbleweed = vec3_distance(&self->positionMirror, &tumbleweeds[weedIndex]->positionMirror);

                    if (distanceToTumbleweed < 40.0f) {
                        if (tumbleweeds[weedIndex]) {} // @fake?
                        obj_destroy_object(tumbleweeds[weedIndex]);

                        //Deposit tumbleweed into bonfire
                        if (state->weedsDeposited < 4) {
                            state->weedsDeposited++;
                            gDLL_14_Modgfx->vtbl->func5(self);
                            gDLL_13_Expgfx->vtbl->func5(self);
                            state->updateFireEffect = TRUE;

                            //Lift up the ChimneySweep once the fire's roaring
                            if (state->weedsDeposited == 4) {
                                set_gplay_bitstring(BIT_GP_ChimneySweep_Lifted, TRUE);
                                //Start a 60 second challenge timer
                                func_8000F64C(0x15, 60);
                                func_8000F6CC();
                            }
                            state->timer = BONFIRE_DWINDLING_TIMER;
                        }
                    } else if (distanceToTumbleweed < 100.0f) {
                        ((DLL_Unknown*)tumbleweeds[weedIndex]->dll)->vtbl->func[12].withTwoArgs((s32)tumbleweeds[weedIndex], (s32)self);
                    }
                }
            }

            //Handle bonfire dwindling
            state->timer -= delayByte;
            if (state->timer < 0) {
                state->weedsDeposited--;
                gDLL_14_Modgfx->vtbl->func5(self);
                gDLL_13_Expgfx->vtbl->func5(self);

                if (state->weedsDeposited >= 0) {
                    state->updateFireEffect = TRUE;
                    state->timer = BONFIRE_DWINDLING_TIMER;
                } else {
                    for (index = 0; index < 2; index++){
                        if (state->soundHandles[index]) {
                            gDLL_6_AMSFX->vtbl->func_A1C(state->soundHandles[index]);
                             state->soundHandles[index] = 0;
                        }
                    }

                    state->weedsDeposited = 0;
                    self->modelInstIdx = 0;
                    state->stateIndex = STATE_1_WAIT_FOR_PLAYER_INTERACTION;
                    self->srt.scale = bonfireScaleData[0];
                }
            }
            break;
    }

    state->previousState = state->currentState;
}

// offset: 0x7F4 | func: 2 | export: 2
void GPbonfire_func_7F4(Object* self){
}

// offset: 0x800 | func: 3 | export: 3
void GPbonfire_draw(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility && self->modelInstIdx == 1) {
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x864 | func: 4 | export: 4
void GPbonfire_destroy(Object* self, s32 arg1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
    obj_free_object_type(self, 0x30);
    //@bug? doesn't stop soundHandles like other object DLLs do
}

// offset: 0x8EC | func: 5 | export: 5
s32 GPbonfire_get_setup_flags(Object* self){
    return 1;
}

// offset: 0x8FC | func: 6 | export: 6
s32 GPbonfire_get_state_size(Object* self, s32 arg1){
    return sizeof(GPBonfireState);
}

// offset: 0x910 | func: 7 | export: 7
s32 GPbonfire_start_burning(Object* self, s32 skipSequence) {
    GPBonfireState* state;
    s32 returnVal;

    state = self->state;

    returnVal = 0;
    if (!skipSequence) {
        gDLL_3_Animation->vtbl->func17(state->sequenceIndexBurning, self, -1);
        returnVal = 1;
        state->stateIndex = STATE_3_START_BURNING;
    } else if (skipSequence == TRUE) {
        if (state->stateIndex == STATE_3_START_BURNING) {
            returnVal = 1;
        }

        if (get_gplay_bitstring(state->gameBitKindlingPlaced)) {
            state->timer = BONFIRE_DWINDLING_TIMER;
            GPbonfire_func_A44(self);
        } else {
            state->stateIndex = STATE_1_WAIT_FOR_PLAYER_INTERACTION;
        }
    }

    return returnVal;
}

// offset: 0xA08 | func: 8 | export: 8
s32 GPbonfire_func_A08(Object* self, s32 arg1, s32 arg2){
    return 0;
}

// offset: 0xA20 | func: 9 | export: 9
s32 GPbonfire_func_A20(Object* self, s32 arg1, s32 arg2){
    return 0;
}

// offset: 0xA38 | func: 10 | export: 10
s32 GPbonfire_func_A38(void){
    return 2;
}

// offset: 0xA44 | func: 11
void GPbonfire_func_A44(Object* self) {
    GPBonfireState* state;
    GPBonfireCreateInfo* createInfo;
    void** dll;
    CurvesFunc1BCReturnInner* curves;

    state = self->state;
    createInfo = (GPBonfireCreateInfo*)self->createInfo;

    if (!get_gplay_bitstring(state->gameBitKindlingPlaced)) {
        return;
    }

    //Loop burning sounds
    state->soundHandles[0] = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_50a_Fire_Burning_Low_Loop, 0x7F, NULL, 0, 0, 0);
    state->soundHandles[1] = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_50b_Fire_Burning_High_Loop, 0x7F, NULL, 0, 0, 0);

    //Create fire effect
    gDLL_14_Modgfx->vtbl->func10.withOneArg((s32)self);
    dll = dll_load_deferred(0x104B, 1); //modgfx #75?
    ((DLL_Unknown*)dll)->vtbl->func[0].withSixArgs((s32)self, state->weedsDeposited, 0, 0x10004, -1, 0);
    dll_unload(dll);

    //Add damaging collision?
    self->objhitInfo->unk_0x58 = 0x100;
    self->unk0xaf |= 8;

    //Advance state
    set_gplay_bitstring(state->gameBitBurning, TRUE);
    state->stateIndex = STATE_4_ADDING_TUMBLEWEEDS;

    //Set flag based on Kyte's curve motion
    curves = gDLL_25->vtbl->dll_25_func_2BC4(self, createInfo->kyteCurveID);
    state->curves = curves;
    if (curves->unk30.words.unk32 != -1) {
        set_gplay_bitstring(curves->unk30.words.unk32, 1);
    }
}

// offset: 0xC24 | func: 12
s32 GPbonfire_callbackBC(Object* self, s32 arg1, CallbackBCUnkArg2* arg2, s32 arg3) {
    GPBonfireState* state;
    s32 index;

    state = self->state;

    if (!state->callbackBool) {
        for (index = 0; index < arg2->unk98; index++){
            if (arg2->unk8E[index] == 1) {
                state->callbackBool = TRUE;
                self->modelInstIdx = 1;
            }
        }
    }
    state->currentState |= 1;

    return 0;
}
