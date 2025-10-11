#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "segment_334F0.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/main.h"
#include "functions.h"
#include "macros.h"
#include "sys/print.h"

typedef struct {
f32 x;
f32 z;
} CoordXZ;

typedef struct {
/*00*/ u32 soundHandle;
/*04*/ s8 pressed;
/*05*/ s8 stateIndex;
/*08*/ Object* objectsOnSwitch[10];
/*30*/ CoordXZ objCoords[10];
} PressureSwitchState;

typedef struct {
ObjSetup base;
u8 yaw;
u8 modelIdx;
s16 gameBitPressed;             //flag to set when switch is pressed down
u8 yOffsetAnimation;            //how far down the switch should move when pressed
u8 yThreshold;                  //threshold for other objects pressing switch
u8 distanceSidekickBehaviour;   //player distance at which special sidekick behaviour is activated
s16 gameBitActivated;            //flag to check if switch is deactivated
} PressureSwitch_Setup;

typedef struct {
s8 unk0[0x62 - 0];
u8 unk62;
s8 unk63[0x7A - 0x63];
s16 unk7A;
s8 unk7C[0x8D - 0x7C];
u8 unk8D;
} CallbackBCUnkArg2;

enum WCPressureSwitchStates {
    STATE_0_UP = 0,
    STATE_1_MOVING_UP = 1,
    STATE_2_DOWN = 2,
    STATE_3_MOVING_DOWN = 3
};

static void WCpressureswitch_add_object(Object* self, Object* objectOnSwitch);
static s32 WCpressureswitch_is_object_on_switch(Object* self);
static s32 WCpressureswitch_callbackBC(Object* self, s32 arg1, CallbackBCUnkArg2* arg2, s32 arg3);

// offset: 0x0 | ctor
void WCpressureswitch_ctor(void* dll){
}

// offset: 0xC | dtor
void WCpressureswitch_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void WCpressureswitch_create(Object* self, PressureSwitch_Setup* setup, s32 arg2) {
    PressureSwitchState* state;
    s32 index;

    self->srt.yaw = setup->yaw << 8;
    self->unk0xb0 |= 0x6000;

    state = self->state;
    self->modelInstIdx = setup->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        // diPrintf("PRESSURESWITCH.c: modelno out of range romdefno=%d\n", self->modelInstIdx);
        self->modelInstIdx = 0;
    }

    if (main_get_bits(setup->gameBitPressed)) {
        self->srt.transl.y = setup->base.y - setup->yOffsetAnimation;
        state->pressed = 30;
        state->stateIndex = 2;
    }

    obj_add_object_type(self, 0x33);

    for (index = 0; index < 10; index++) { state->objectsOnSwitch[index] = 0; }

    self->unk0xbc = (void*)&WCpressureswitch_callbackBC;
}

/*0x0*/ static const char str_0[] = "PRESSURESWITCH.c: modelno out of range romdefno=%d\n";

// offset: 0x150 | func: 1 | export: 1
void WCpressureswitch_update(Object* self) {
    PressureSwitch_Setup* setup;
    f32 deltaY;
    Object* listedObject;
    s32* textureFrame;
    s32 index;
    PressureSwitchState* state;

    setup = (PressureSwitch_Setup*)self->setup;
    state = self->state;

    //Bail if switch deactivated
    if (setup->gameBitActivated > 0 && !main_get_bits(setup->gameBitActivated)) {
        diPrintf(" Avitvate %i ", setup->gameBitActivated);
        return;
    }

    //Decrement timer until not considered pressed (fps-dependent)
    state->pressed--;
    if (state->pressed < 0) {
        state->pressed = 0;
    }

    //Handle adding objects to switch
    if (self->unk0x58->unk10f > 0) {
        for (index = 0; index < self->unk0x58->unk10f; index++){
            listedObject = (Object*)self->unk0x58->unk100[index];
            deltaY = listedObject->srt.transl.y - self->srt.transl.y;
            if (deltaY > setup->yThreshold) {
                WCpressureswitch_add_object(self, listedObject);
            }
        }
    }

    //Check if object on switch
    if (WCpressureswitch_is_object_on_switch(self)) {
        state->pressed = 5;
    }

    //Main state machine
    deltaY = setup->base.y - setup->yOffsetAnimation;
    switch (state->stateIndex) {
        case STATE_0_UP:
            if (state->pressed && deltaY <= self->srt.transl.y) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_99a_Mechanical_Ratcheting, 0x7F, NULL, 0, 0, 0);
                state->stateIndex = STATE_3_MOVING_DOWN;
            }
            break;
        case STATE_3_MOVING_DOWN:
            self->srt.transl.y -= 0.05f * delayFloat;
            if (self->srt.transl.y < deltaY) {
                main_set_bits(setup->gameBitPressed, 1);
                state->stateIndex = STATE_2_DOWN;
                self->srt.transl.y = deltaY;
            }
            break;
        case STATE_2_DOWN:
            /* Subtly different behaviour to other pressure switches,
             * waits for flag to unset before depressing the switch (for WC's timed challenges) */
            if (!main_get_bits(setup->gameBitPressed)) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_99a_Mechanical_Ratcheting, 0x7F, NULL, 0, 0, 0);
                state->stateIndex = STATE_1_MOVING_UP;
            }
            break;
        case STATE_1_MOVING_UP:
            self->srt.transl.y += 0.05f * delayFloat;
            if (setup->base.y < self->srt.transl.y) {
                self->srt.transl.y = setup->base.y;
                state->stateIndex = STATE_0_UP;
            }
            break;
    }

    //Change texture frame (sun/moon glowing)
    textureFrame = func_800348A0(self, 0, 0);
    if (textureFrame != NULL) {
        if (state->stateIndex == 2) {
            *textureFrame = 1;
        } else {
            *textureFrame = 0;
        }
        *textureFrame <<= 8;
    }
}

// offset: 0x4B4 | func: 2 | export: 2
void WCpressureswitch_func_4B4(Object* self){
}

// offset: 0x4C0 | func: 3 | export: 3
void WCpressureswitch_draw(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x514 | func: 4 | export: 4
void WCpressureswitch_destroy(Object* self, s32 arg1) {
    PressureSwitchState* state = self->state;

    if (state->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(state->soundHandle);
    }
    obj_free_object_type(self, 0x33);
}

// offset: 0x594 | func: 5 | export: 5
u32 WCpressureswitch_get_model_flags(Object* self) {
    PressureSwitch_Setup* setup = (PressureSwitch_Setup*)self->setup;
    s32 modelIndex;

    modelIndex = setup->modelIdx;
    if (modelIndex >= self->def->numModels) {
        modelIndex = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelIndex) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x5C8 | func: 6 | export: 6
u32 WCpressureswitch_get_state_size(Object* self, s32 arg1){
    return sizeof(PressureSwitchState);
}

// offset: 0x5DC | func: 7
void WCpressureswitch_add_object(Object* self, Object* objectOnSwitch) {
    PressureSwitchState *state = self->state;
    u8 objectIndex;

    //@bug: should be && and "objectIndex != 9" (crashes game once objectsOnSwitch array overflows)
    for (objectIndex = 0; state->objectsOnSwitch[objectIndex] || objectIndex == 9; objectIndex++);

    state->objectsOnSwitch[objectIndex] = objectOnSwitch;    
    state->objCoords[objectIndex].x = objectOnSwitch->srt.transl.x;
    state->objCoords[objectIndex].z = objectOnSwitch->srt.transl.z;
}

// offset: 0x648 | func: 8
s32 WCpressureswitch_is_object_on_switch(Object* self) {
    PressureSwitchState* state;
    CoordXZ* coord;
    u8 index;
    u8 returnVal;

    state = self->state;

    for (returnVal = FALSE, index = 0; index < 10; index++){
        if (!state->objectsOnSwitch[index])
            continue;

        coord = &state->objCoords[index];
        if (state->objectsOnSwitch[index]->srt.transl.x == coord->x && 
            state->objectsOnSwitch[index]->srt.transl.z == coord->z) {
            returnVal = TRUE;
        } else {
            state->objectsOnSwitch[index] = NULL;
        }
    }

    return returnVal;
}

// offset: 0x6CC | func: 9
s32 WCpressureswitch_callbackBC(Object* self, s32 arg1, CallbackBCUnkArg2* arg2, s32 arg3) {
    PressureSwitchState* state;
    PressureSwitch_Setup* setup;
    u8 index;

    state = self->state;
    setup = (PressureSwitch_Setup*)self->setup;

    if (arg2->unk8D == 1) {
        for (index = 0; index < 10; index++){
            if (state->objectsOnSwitch[index]) {
                state->objCoords[index].x = state->objectsOnSwitch[index]->srt.transl.x;
                state->objCoords[index].z = state->objectsOnSwitch[index]->srt.transl.z;
            }
        }
        arg2->unk8D = 0;

    } else if (arg2->unk8D == 2) {
        for (index = 0; index < 10; index++);

        self->srt.transl.z = setup->base.x; //@bug? should be x component?
        self->srt.transl.y = setup->base.y;
        self->srt.transl.z = setup->base.z;
        main_set_bits(setup->gameBitPressed, 0);
        arg2->unk8D = 0;
    }

    return 0;
}
