#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
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
ObjCreateInfo base;
u8 yaw;
u8 modelIdx;
s16 gameBitPressed;             //flag to set when switch is pressed down
u8 yOffsetAnimation;            //how far down the switch should move when pressed
u8 yThreshold;                  //threshold for other objects pressing switch
u8 distanceSidekickBehaviour;   //player distance at which special sidekick behaviour is activated
s16 gameBitActivated;            //flag to check if switch is deactivated
} PressureSwitchCreateInfo;

typedef struct {
s8 unk0[0x62 - 0];
u8 unk62;
s8 unk63[0x7A - 0x63];
s16 unk7A;
s8 unk7C[0x8D - 0x7C];
u8 unk8D;
} CallbackBCUnkArg2;

static void pressureswitch_add_object(Object* self, Object* objectOnSwitch);
static s32 pressureswitch_is_object_on_switch(Object* self);
static s32 pressureswitch_callbackBC(Object* self, s32 arg1, CallbackBCUnkArg2* arg2, s32 arg3);

// offset: 0x0 | ctor
void dll_287_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_287_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void pressureswitch_create(Object* self, PressureSwitchCreateInfo* createInfo, s32 arg2) {
    PressureSwitchState* state;
    s32 index;

    self->srt.yaw = createInfo->yaw << 8;
    self->unk0xb0 |= 0x6000;

    state = self->state;
    self->modelInstIdx = createInfo->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        // diPrintf("PRESSURESWITCH.c: modelno out of range romdefno=%d\n", self->modelInstIdx);
        self->modelInstIdx = 0;
    }

    if (get_gplay_bitstring(createInfo->gameBitPressed)) {
        self->srt.transl.y = createInfo->base.y - createInfo->yOffsetAnimation;
        state->pressed = 30;
    }

    obj_add_object_type(self, 0x33);

    for (index = 0; index < 10; index++) { state->objectsOnSwitch[index] = 0; }

    self->unk0xbc = (void*)&pressureswitch_callbackBC;
}

// offset: 0x148 | func: 1 | export: 1
void pressureswitch_update(Object* self) {
    f32 deltaY;
    Object* sidekick;
    Object* player;
    Object* listedObject;
    PressureSwitchCreateInfo* createInfo;
    s32 pad;
    PressureSwitchState* state;
    s8 playSound;
    s32 index;

    createInfo = (PressureSwitchCreateInfo*)self->createInfo;
    state = self->state;

    //Bail if switch deactivated
    if (createInfo->gameBitActivated > 0 && !get_gplay_bitstring(createInfo->gameBitActivated)){
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
            if (deltaY > createInfo->yThreshold) {
                pressureswitch_add_object(self, listedObject);
            }
        }
    }

    //Check if object on switch
    if (pressureswitch_is_object_on_switch(self)) {
        state->pressed = 5;
    }

    //Animate the switch's y coordinate
    playSound = 0;
    if (state->pressed) {
        deltaY = createInfo->base.y - createInfo->yOffsetAnimation;
        if (self->srt.transl.y < deltaY) {
            self->srt.transl.y += 0.25f * delayFloat;
            if (deltaY < self->srt.transl.y) {
                self->srt.transl.y = deltaY;
            }
            set_gplay_bitstring(createInfo->gameBitPressed, 1);
        } else {
            self->srt.transl.y -= 0.125f * delayFloat;
            if (self->srt.transl.y < deltaY) {
                self->srt.transl.y = deltaY;
                set_gplay_bitstring(createInfo->gameBitPressed, 1);
            } else {
                playSound = 1;
            }
        }
    } else {
        self->srt.transl.y += 0.125f * delayFloat;

        if (createInfo->base.y < self->srt.transl.y) {
            self->srt.transl.y = createInfo->base.y;
            set_gplay_bitstring(createInfo->gameBitPressed, 0);
        } else {
            playSound = 1;
        }
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

    //Activate sidekick behaviour when player close to switch
    if (!get_gplay_bitstring(createInfo->gameBitPressed) && createInfo->distanceSidekickBehaviour) {
        player = get_player();
        sidekick = get_sidekick();
        if (sidekick) {
            if (vec3_distance_squared(&self->positionMirror, &player->positionMirror) <= 
                (createInfo->distanceSidekickBehaviour * createInfo->distanceSidekickBehaviour)) {
                //TO-DO: use proper sidekick DLL interface
                ((DLL_Unknown*)sidekick->dll)->vtbl->func[14].withTwoArgs((s32)sidekick, 3);
            }
        }
    }
}

// offset: 0x524 | func: 2 | export: 2
void pressureswitch_func_524(Object* dll){
}

// offset: 0x530 | func: 3 | export: 3
void pressureswitch_draw(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x584 | func: 4 | export: 4
void pressureswitch_destroy(Object* self, s32 arg1) {
    PressureSwitchState* state = self->state;

    if (state->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(state->soundHandle);
    }
    obj_free_object_type(self, 0x33);
}

// offset: 0x604 | func: 5 | export: 5
s32 pressureswitch_get_setup_flags(Object* self) {
    return 0;
}

// offset: 0x614 | func: 6 | export: 6
u32 pressureswitch_get_state_size(Object *self, u32 a1) {
    return sizeof(PressureSwitchState);
}

// offset: 0x628 | func: 7
void pressureswitch_add_object(Object* self, Object* objectOnSwitch) {
    PressureSwitchState *state = self->state;
    u8 objectIndex;

    //@bug: should be && and "objectIndex != 9" (crashes game once objectsOnSwitch array overflows)
    for (objectIndex = 0; state->objectsOnSwitch[objectIndex] || objectIndex == 9; objectIndex++);

    state->objectsOnSwitch[objectIndex] = objectOnSwitch;    
    state->objCoords[objectIndex].x = objectOnSwitch->srt.transl.x;
    state->objCoords[objectIndex].z = objectOnSwitch->srt.transl.z;
}

// offset: 0x694 | func: 8
s32 pressureswitch_is_object_on_switch(Object* self) {
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

// offset: 0x718 | func: 9
s32 pressureswitch_callbackBC(Object* self, s32 arg1, CallbackBCUnkArg2* arg2, s32 arg3) {
    PressureSwitchState* state;
    PressureSwitchCreateInfo* createInfo;
    u8 index;

    state = self->state;
    createInfo = (PressureSwitchCreateInfo*)self->createInfo;

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

        self->srt.transl.z = createInfo->base.x; //@bug? should be x component?
        self->srt.transl.y = createInfo->base.y;
        self->srt.transl.z = createInfo->base.z;
        set_gplay_bitstring(createInfo->gameBitPressed, 0);
        arg2->unk8D = 0;
    }

    return 0;
}

/*0x0*/ static const char str_0[] = "PRESSURESWITCH.c: modelno out of range romdefno=%d\n";
/*0x34*/ static const char str_34[] = "";
/*0x38*/ static const char str_38[] = "";
/*0x3C*/ static const char str_3C[] = "";
