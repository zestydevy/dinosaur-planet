#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/214_animobj.h"
#include "functions.h"
#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/gfx/model.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/main.h"
#include "functions.h"
#include "types.h"

typedef struct {
f32 x;
f32 z;
} CoordXZ;

typedef struct {
/*00*/ u32 soundHandle;
/*04*/ s8 pressed;
/*05*/ s8 state;
/*08*/ Object* objectsOnSwitch[10];
/*30*/ CoordXZ objCoords[10];
} PressureSwitch_Data;

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

static void pressureswitch_add_object(Object* self, Object* objectOnSwitch);
static s32 pressureswitch_is_object_on_switch(Object* self);
static s32 pressureswitch_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3);

// offset: 0x0 | ctor
void dll_287_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_287_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void pressureswitch_setup(Object* self, PressureSwitch_Setup* setup, s32 arg2) {
    PressureSwitch_Data* objdata;
    s32 index;

    self->srt.yaw = setup->yaw << 8;
    self->unk0xb0 |= 0x6000;

    objdata = self->data;
    self->modelInstIdx = setup->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        // diPrintf("PRESSURESWITCH.c: modelno out of range romdefno=%d\n", self->modelInstIdx);
        self->modelInstIdx = 0;
    }

    if (main_get_bits(setup->gameBitPressed)) {
        self->srt.transl.y = setup->base.y - setup->yOffsetAnimation;
        objdata->pressed = 30;
    }

    obj_add_object_type(self, 0x33);

    for (index = 0; index < 10; index++) { objdata->objectsOnSwitch[index] = 0; }

    self->unk0xbc = (void*)&pressureswitch_anim_callback;
}

// offset: 0x148 | func: 1 | export: 1
void pressureswitch_control(Object* self) {
    f32 deltaY;
    Object* sidekick;
    Object* player;
    Object* listedObject;
    PressureSwitch_Setup* setup;
    s32 pad;
    PressureSwitch_Data* objdata;
    s8 playSound;
    s32 index;

    setup = (PressureSwitch_Setup*)self->setup;
    objdata = self->data;

    //Bail if switch deactivated
    if (setup->gameBitActivated > 0 && !main_get_bits(setup->gameBitActivated)){
        return;
    }

    //Decrement timer until not considered pressed (fps-dependent)
    objdata->pressed--;
    if (objdata->pressed < 0) {
        objdata->pressed = 0;
    }

    //Handle adding objects to switch
    if (self->unk0x58->unk10f > 0) {
        for (index = 0; index < self->unk0x58->unk10f; index++){
            listedObject = (Object*)self->unk0x58->unk100[index];
            deltaY = listedObject->srt.transl.y - self->srt.transl.y;
            if (deltaY > setup->yThreshold) {
                pressureswitch_add_object(self, listedObject);
            }
        }
    }

    //Check if object on switch
    if (pressureswitch_is_object_on_switch(self)) {
        objdata->pressed = 5;
    }

    //Animate the switch's y coordinate
    playSound = 0;
    if (objdata->pressed) {
        deltaY = setup->base.y - setup->yOffsetAnimation;
        if (self->srt.transl.y < deltaY) {
            self->srt.transl.y += 0.25f * delayFloat;
            if (deltaY < self->srt.transl.y) {
                self->srt.transl.y = deltaY;
            }
            main_set_bits(setup->gameBitPressed, 1);
        } else {
            self->srt.transl.y -= 0.125f * delayFloat;
            if (self->srt.transl.y < deltaY) {
                self->srt.transl.y = deltaY;
                main_set_bits(setup->gameBitPressed, 1);
            } else {
                playSound = 1;
            }
        }
    } else {
        self->srt.transl.y += 0.125f * delayFloat;

        if (setup->base.y < self->srt.transl.y) {
            self->srt.transl.y = setup->base.y;
            main_set_bits(setup->gameBitPressed, 0);
        } else {
            playSound = 1;
        }
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

    //Activate sidekick behaviour when player close to switch
    if (!main_get_bits(setup->gameBitPressed) && setup->distanceSidekickBehaviour) {
        player = get_player();
        sidekick = get_sidekick();
        if (sidekick) {
            if (vec3_distance_squared(&self->positionMirror, &player->positionMirror) <= 
                (setup->distanceSidekickBehaviour * setup->distanceSidekickBehaviour)) {
                //TO-DO: use proper sidekick DLL interface
                ((DLL_Unknown*)sidekick->dll)->vtbl->func[14].withTwoArgs((s32)sidekick, 3);
            }
        }
    }
}

// offset: 0x524 | func: 2 | export: 2
void pressureswitch_update(Object* dll){
}

// offset: 0x530 | func: 3 | export: 3
void pressureswitch_print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x584 | func: 4 | export: 4
void pressureswitch_free(Object* self, s32 arg1) {
    PressureSwitch_Data* objdata = self->data;

    if (objdata->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
    }
    obj_free_object_type(self, 0x33);
}

// offset: 0x604 | func: 5 | export: 5
u32 pressureswitch_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x614 | func: 6 | export: 6
u32 pressureswitch_get_data_size(Object *self, u32 a1) {
    return sizeof(PressureSwitch_Data);
}

// offset: 0x628 | func: 7
void pressureswitch_add_object(Object* self, Object* objectOnSwitch) {
    PressureSwitch_Data *objdata = self->data;
    u8 objectIndex;

    //@bug: should be && and "objectIndex != 9" (crashes game once objectsOnSwitch array overflows)
    for (objectIndex = 0; objdata->objectsOnSwitch[objectIndex] || objectIndex == 9; objectIndex++);

    objdata->objectsOnSwitch[objectIndex] = objectOnSwitch;    
    objdata->objCoords[objectIndex].x = objectOnSwitch->srt.transl.x;
    objdata->objCoords[objectIndex].z = objectOnSwitch->srt.transl.z;
}

// offset: 0x694 | func: 8
s32 pressureswitch_is_object_on_switch(Object* self) {
    PressureSwitch_Data* objdata;
    CoordXZ* coord;
    u8 index;
    u8 returnVal;

    objdata = self->data;

    for (returnVal = FALSE, index = 0; index < 10; index++){
        if (!objdata->objectsOnSwitch[index])
            continue;

        coord = &objdata->objCoords[index];
        if (objdata->objectsOnSwitch[index]->srt.transl.x == coord->x && 
            objdata->objectsOnSwitch[index]->srt.transl.z == coord->z) {
            returnVal = TRUE;
        } else {
            objdata->objectsOnSwitch[index] = NULL;
        }
    }

    return returnVal;
}

// offset: 0x718 | func: 9
s32 pressureswitch_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3) {
    PressureSwitch_Data* objdata;
    PressureSwitch_Setup* setup;
    u8 index;

    objdata = self->data;
    setup = (PressureSwitch_Setup*)self->setup;

    if (animObjData->unk8D == 1) {
        for (index = 0; index < 10; index++){
            if (objdata->objectsOnSwitch[index]) {
                objdata->objCoords[index].x = objdata->objectsOnSwitch[index]->srt.transl.x;
                objdata->objCoords[index].z = objdata->objectsOnSwitch[index]->srt.transl.z;
            }
        }
        animObjData->unk8D = 0;

    } else if (animObjData->unk8D == 2) {
        for (index = 0; index < 10; index++);

        self->srt.transl.z = setup->base.x; //@bug? should be x component?
        self->srt.transl.y = setup->base.y;
        self->srt.transl.z = setup->base.z;
        main_set_bits(setup->gameBitPressed, 0);
        animObjData->unk8D = 0;
    }

    return 0;
}

/*0x0*/ static const char str_0[] = "PRESSURESWITCH.c: modelno out of range romdefno=%d\n";
/*0x34*/ static const char str_34[] = "";
/*0x38*/ static const char str_38[] = "";
/*0x3C*/ static const char str_3C[] = "";
