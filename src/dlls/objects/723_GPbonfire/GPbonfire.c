#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/26_curves.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/214_animobj.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/menu.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "dlls/objects/210_player.h"
#include "dll.h"
#include "types.h"

typedef struct {
/*00*/ u8 stateIndex;
/*01*/ u8 currentState;
/*02*/ u8 previousState;
/*04*/ s32 sequenceIndexKindling;   //the cutscene for placing the kindling
/*08*/ s32 sequenceIndexBurning;    //the cutscene for Kyte starting the fire
/*0C*/ CurveSetup* curves;
/*10*/ s16 gameBitKindlingPlaced;   //flag to set when kindling is ready to burn
/*12*/ s16 gameBitBurning;          //flag to set when the bonfire's burning
/*14*/ s32 unused14;
/*18*/ u32 soundHandles[2];         //burning sfx
/*20*/ s16 timer;                   //counts down until the bonfire dwindles by one stage
/*22*/ s8 weedsDeposited;
/*23*/ u8 updateFireEffect;         //replaces the modgfx fire effect when its size changes
/*24*/ u8 callbackBool;             //Boolean associated with the cutscene callback function
} GPBonfire_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 interactionDistance;
/*1A*/ u16 kyteCurveID;
/*1C*/ s16 unused1C;
/*1E*/ s8 unused1E;
/*1F*/ u8 yaw;
} GPBonfire_Setup;

enum GPBonfireStates{
    STATE_0_INITIALISE = 0,
    STATE_1_WAIT_FOR_PLAYER_INTERACTION = 1,
    STATE_2_WAIT_FOR_KYTE = 2,
    STATE_3_START_BURNING = 3,
    STATE_4_ADDING_TUMBLEWEEDS = 4
};

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
static int GPbonfire_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3);

// offset: 0x0 | ctor
void GPbonfire_ctor(void* dll){ }

// offset: 0xC | dtor
void GPbonfire_dtor(void* dll){ }

// offset: 0x18 | func: 0 | export: 0
void GPbonfire_setup(Object* self, GPBonfire_Setup* setup, s32 arg2) {
    GPBonfire_Data* objdata;

    objdata = self->data;
    self->animCallback = GPbonfire_anim_callback;
    self->srt.yaw = setup->yaw << 8;
    objdata->stateIndex = STATE_0_INITIALISE;
    objdata->currentState = 0;
    objdata->soundHandles[0] = 0;
    objdata->soundHandles[1] = 0;
    obj_add_object_type(self, OBJTYPE_48);
    objdata->gameBitKindlingPlaced = BIT_GP_Bonfire_Kindling_Placed;
    objdata->gameBitBurning = BIT_GP_Bonfire_Burning;
    objdata->sequenceIndexKindling = 0;
    objdata->sequenceIndexBurning = 1;
    objdata->unused14 = 0;
    objdata->weedsDeposited = 0;
}

// offset: 0xB8 | func: 1 | export: 1
void GPbonfire_control(Object* self) {
    GPBonfire_Data* objdata;
    GPBonfire_Setup* setup;
    Object* sidekick;
    Object* player;
    f32 distanceToTumbleweed;
    s32 weedIndex;
    s32 count;
    void** dll;
    u8 index;
    u8 playerIsNearby;
    Object** tumbleweeds;

    objdata = self->data;
    setup = (GPBonfire_Setup*)self->setup;
    player = get_player();

    playerIsNearby = vec3_distance_xz_squared(&player->positionMirror, &self->positionMirror) <= setup->interactionDistance * setup->interactionDistance;    

    objdata->currentState &= 0xFFFD;
    if (objdata->currentState & 1) {
        main_set_bits(objdata->gameBitKindlingPlaced, TRUE);
        objdata->stateIndex = STATE_2_WAIT_FOR_KYTE;
        objdata->currentState &= 0xFFFE;
        self->modelInstIdx = 1;
    }

    if (objdata->updateFireEffect) {
        self->srt.scale = bonfireScaleData[objdata->weedsDeposited];
        gDLL_14_Modgfx->vtbl->func10(self);
        dll = dll_load_deferred(0x104B, 1);
        ((DLL_Unknown*)dll)->vtbl->func[0].withSixArgs((s32)self, modgfxScaleData[objdata->weedsDeposited], 0, 0x10004, -1, 0);
        dll_unload(dll);
        objdata->updateFireEffect = FALSE;
    }

    switch (objdata->stateIndex) {
        case STATE_0_INITIALISE:
            self->modelInstIdx = 0;
            objdata->stateIndex = STATE_1_WAIT_FOR_PLAYER_INTERACTION;
            if (main_get_bits(objdata->gameBitKindlingPlaced)) {
                self->modelInstIdx = 1;
                objdata->stateIndex = STATE_2_WAIT_FOR_KYTE;
            }
            if (main_get_bits(objdata->gameBitBurning)) {
                objdata->timer = BONFIRE_DWINDLING_TIMER;
                GPbonfire_func_A44(self);
            }
            break;
        case STATE_1_WAIT_FOR_PLAYER_INTERACTION:
            if (playerIsNearby && 
                ((DLL_210_Player*)player->dll)->vtbl->func48(player) == 0x4BB) {
                gDLL_3_Animation->vtbl->func17(objdata->sequenceIndexKindling, self, -1);
            }
            break;
        case STATE_2_WAIT_FOR_KYTE:
            sidekick = get_sidekick();
            if (sidekick && playerIsNearby) {
                ((DLL_Unknown*)sidekick->dll)->vtbl->func[14].withTwoArgs((s32)sidekick, 4);
                if (gDLL_1_UI->vtbl->func_DF4(4)) {
                    main_set_bits(BIT_Kyte_Flight_Curve, setup->kyteCurveID);
                }
            }
            break;
        case STATE_3_START_BURNING:
            if (vec3_distance_xz_squared(&get_sidekick()->positionMirror, &self->positionMirror) <= 2500.0f) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_425, NULL, PARTFXFLAG_2, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_426, NULL, PARTFXFLAG_2, -1, NULL);
            }
            break;
        case STATE_4_ADDING_TUMBLEWEEDS:
            if (playerIsNearby) {
                objdata->currentState |= 2;
            }
            if ((objdata->currentState & 2) && !(objdata->previousState & 2)) {
                func_80000450(self, self, 0x59, 0, 0, 0);
            } else if (!(objdata->currentState & 2) && (objdata->previousState & 2)) {
                func_80000450(self, self, 0x5A, 0, 0, 0);
            }
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_425, NULL, PARTFXFLAG_2, -1, NULL);

            //Handle tumbleweeds (only whole ones, twigs don't count)
            tumbleweeds = obj_get_all_of_type(4, &count);
            for (weedIndex = 0; weedIndex < count; weedIndex++){
                if (tumbleweeds[weedIndex]->id == OBJ_Tumbleweed3) {
                    distanceToTumbleweed = vec3_distance(&self->positionMirror, &tumbleweeds[weedIndex]->positionMirror);

                    if (distanceToTumbleweed < 40.0f) {
                        if (tumbleweeds[weedIndex]) {} // @fake?
                        obj_destroy_object(tumbleweeds[weedIndex]);

                        //Deposit tumbleweed into bonfire
                        if (objdata->weedsDeposited < 4) {
                            objdata->weedsDeposited++;
                            gDLL_14_Modgfx->vtbl->func5(self);
                            gDLL_13_Expgfx->vtbl->func5(self);
                            objdata->updateFireEffect = TRUE;

                            //Lift up the ChimneySwipe once the fire's roaring
                            if (objdata->weedsDeposited == 4) {
                                main_set_bits(BIT_GP_ChimneySwipe_Lifted, TRUE);
                                //Start a 60 second challenge timer
                                func_8000F64C(0x15, 60);
                                func_8000F6CC();
                            }
                            objdata->timer = BONFIRE_DWINDLING_TIMER;
                        }
                    } else if (distanceToTumbleweed < 100.0f) {
                        ((DLL_Unknown*)tumbleweeds[weedIndex]->dll)->vtbl->func[12].withTwoArgs((s32)tumbleweeds[weedIndex], (s32)self);
                    }
                }
            }

            //Handle bonfire dwindling
            objdata->timer -= gUpdateRate;
            if (objdata->timer < 0) {
                objdata->weedsDeposited--;
                gDLL_14_Modgfx->vtbl->func5(self);
                gDLL_13_Expgfx->vtbl->func5(self);

                if (objdata->weedsDeposited >= 0) {
                    objdata->updateFireEffect = TRUE;
                    objdata->timer = BONFIRE_DWINDLING_TIMER;
                } else {
                    for (index = 0; index < 2; index++){
                        if (objdata->soundHandles[index]) {
                            gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandles[index]);
                             objdata->soundHandles[index] = 0;
                        }
                    }

                    objdata->weedsDeposited = 0;
                    self->modelInstIdx = 0;
                    objdata->stateIndex = STATE_1_WAIT_FOR_PLAYER_INTERACTION;
                    self->srt.scale = bonfireScaleData[0];
                }
            }
            break;
    }

    objdata->previousState = objdata->currentState;
}

// offset: 0x7F4 | func: 2 | export: 2
void GPbonfire_update(Object* self){ }

// offset: 0x800 | func: 3 | export: 3
void GPbonfire_print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility && self->modelInstIdx == 1) {
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x864 | func: 4 | export: 4
void GPbonfire_free(Object* self, s32 arg1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
    obj_free_object_type(self, 0x30);
    //@bug? doesn't stop soundHandles like other object DLLs do
}

// offset: 0x8EC | func: 5 | export: 5
u32 GPbonfire_get_model_flags(Object* self){
    return MODFLAGS_1;
}

// offset: 0x8FC | func: 6 | export: 6
u32 GPbonfire_get_data_size(Object* self, s32 arg1){
    return sizeof(GPBonfire_Data);
}

// offset: 0x910 | func: 7 | export: 7
s32 GPbonfire_start_burning(Object* self, s32 skipSequence) {
    GPBonfire_Data* objdata;
    s32 returnVal;

    objdata = self->data;

    returnVal = 0;
    if (!skipSequence) {
        gDLL_3_Animation->vtbl->func17(objdata->sequenceIndexBurning, self, -1);
        returnVal = 1;
        objdata->stateIndex = STATE_3_START_BURNING;
    } else if (skipSequence == TRUE) {
        if (objdata->stateIndex == STATE_3_START_BURNING) {
            returnVal = 1;
        }

        if (main_get_bits(objdata->gameBitKindlingPlaced)) {
            objdata->timer = BONFIRE_DWINDLING_TIMER;
            GPbonfire_func_A44(self);
        } else {
            objdata->stateIndex = STATE_1_WAIT_FOR_PLAYER_INTERACTION;
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
    GPBonfire_Data* objdata;
    GPBonfire_Setup* setup;
    void** dll;
    CurveSetup* curves;

    objdata = self->data;
    setup = (GPBonfire_Setup*)self->setup;

    if (!main_get_bits(objdata->gameBitKindlingPlaced)) {
        return;
    }

    //Loop burning sounds
    objdata->soundHandles[0] = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_50a_Fire_Burning_Low_Loop, 0x7F, NULL, 0, 0, 0);
    objdata->soundHandles[1] = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_50b_Fire_Burning_High_Loop, 0x7F, NULL, 0, 0, 0);

    //Create fire effect
    gDLL_14_Modgfx->vtbl->func10(self);
    dll = dll_load_deferred(0x104B, 1); //modgfx #75?
    ((DLL_Unknown*)dll)->vtbl->func[0].withSixArgs((s32)self, objdata->weedsDeposited, 0, 0x10004, -1, 0);
    dll_unload(dll);

    //Add damaging collision?
    self->objhitInfo->unk58 = 0x100;
    self->unkAF |= 8;

    //Advance state
    main_set_bits(objdata->gameBitBurning, TRUE);
    objdata->stateIndex = STATE_4_ADDING_TUMBLEWEEDS;

    //Set flag based on Kyte's curve motion
    curves = gDLL_25->vtbl->func_2BC4(self, setup->kyteCurveID);
    objdata->curves = curves;
    if (curves->type22.usedBit != -1) {
        main_set_bits(curves->type22.usedBit, 1);
    }
}

// offset: 0xC24 | func: 12
static int GPbonfire_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    GPBonfire_Data* objdata;
    s32 index;

    objdata = self->data;

    if (!objdata->callbackBool) {
        for (index = 0; index < animObjData->unk98; index++){
            if (animObjData->unk8E[index] == 1) {
                objdata->callbackBool = TRUE;
                self->modelInstIdx = 1;
            }
        }
    }
    objdata->currentState |= 1;

    return 0;
}
