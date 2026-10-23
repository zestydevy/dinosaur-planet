#include "common.h"
#include "game/objects/setups/86_PressurePad.h"
#include "sys/objtype.h"

typedef struct {
/*00*/ u32 soundHandle;
/*04*/ s8 pressedTimer;
/*05*/ s8 state;
/*08*/ Object* objectsOnSwitch[10];
/*30*/ Vec2f objCoords[10];
} PressureSwitch_Data;

enum WCPressureSwitchStates {
    STATE_0_UP,
    STATE_1_MOVING_UP,
    STATE_2_DOWN,
    STATE_3_MOVING_DOWN
};

static void WCPressureSwitch_addObject(Object* self, Object* objectOnSwitch);
static s32 WCPressureSwitch_isObjectOnSwitch(Object* self);
static int WCPressureSwitch_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void WCPressureSwitch_ctor(void* dll) { }

// offset: 0xC | dtor
void WCPressureSwitch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCPressureSwitch_obj_Setup(Object* self, PressurePad_Setup* setup, s32 reset) {
    PressureSwitch_Data* objData;
    s32 index;

    self->srt.yaw = setup->yaw << 8;
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);

    objData = self->data;
    self->modelInstIdx = setup->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
        STUBBED_PRINTF("PRESSURESWITCH.c: modelno out of range romdefno=%d\n", setup->base.objId);
    }

    if (mainGetBits(setup->gameBitPressed)) {
        self->srt.transl.y = setup->base.y - setup->yOffsetAnimation;
        objData->pressedTimer = 30;
        objData->state = STATE_2_DOWN;
    }

    objAddObjectType(self, OBJTYPE_TrickyTarget);

    for (index = 0; index < 10; index++) { objData->objectsOnSwitch[index] = 0; }

    self->animCallback = WCPressureSwitch_animCallback;
}

// offset: 0x150 | func: 1 | export: 1
void WCPressureSwitch_obj_Control(Object* self) {
    PressurePad_Setup* setup;
    f32 deltaY;
    Object* listedObject;
    TextureAnimator* animTexture;
    s32 index;
    PressureSwitch_Data* objdata;

    setup = (PressurePad_Setup*)self->setup;
    objdata = self->data;

    //Bail if switch deactivated
    if (setup->gamebitActivated > 0 && !mainGetBits(setup->gamebitActivated)) {
        diPrintf(" Avitvate %i ", setup->gamebitActivated);
        return;
    }

    //Decrement timer until not considered pressed (fps-dependent)
    objdata->pressedTimer--;
    if (objdata->pressedTimer < 0) {
        objdata->pressedTimer = 0;
    }

    //Handle adding objects to switch
    if (self->polyhits->unk10F > 0) {
        for (index = 0; index < self->polyhits->unk10F; index++) {
            listedObject = (Object*)self->polyhits->unk100[index];
            deltaY = listedObject->srt.transl.y - self->srt.transl.y;
            if (deltaY > setup->yThreshold) {
                WCPressureSwitch_addObject(self, listedObject);
            }
        }
    }

    //Check if object on switch
    if (WCPressureSwitch_isObjectOnSwitch(self)) {
        objdata->pressedTimer = 5;
    }

    //Main state machine
    deltaY = setup->base.y - setup->yOffsetAnimation;
    switch (objdata->state) {
        case STATE_0_UP:
            if ((objdata->pressedTimer != 0) && (deltaY <= self->srt.transl.y)) {
                dll_amSfx->Play(self, SOUND_99A_Mechanical_Ratcheting, MAX_VOLUME, NULL, 0, 0, 0);
                objdata->state = STATE_3_MOVING_DOWN;
            }
            break;
        case STATE_3_MOVING_DOWN:
            self->srt.transl.y -= 0.05f * gUpdateRateF;
            if (self->srt.transl.y < deltaY) {
                mainSetBits(setup->gameBitPressed, 1);
                objdata->state = STATE_2_DOWN;
                self->srt.transl.y = deltaY;
            }
            break;
        case STATE_2_DOWN:
            /* Subtly different behaviour to other pressure switches,
             * waits for flag to unset before depressing the switch (for WC's timed challenges) */
            if (!mainGetBits(setup->gameBitPressed)) {
                dll_amSfx->Play(self, SOUND_99A_Mechanical_Ratcheting, MAX_VOLUME, NULL, 0, 0, 0);
                objdata->state = STATE_1_MOVING_UP;
            }
            break;
        case STATE_1_MOVING_UP:
            self->srt.transl.y += 0.05f * gUpdateRateF;
            if (self->srt.transl.y > setup->base.y) {
                self->srt.transl.y = setup->base.y;
                objdata->state = STATE_0_UP;
            }
            break;
    }

    //Change texture frame (sun/moon glowing)
    animTexture = objExprGetTexAnimator(self, 0, 0);
    if (animTexture != NULL) {
        if (objdata->state == STATE_2_DOWN) {
            animTexture->frame = 1;
        } else {
            animTexture->frame = 0;
        }
        animTexture->frame <<= 8;
    }
}

// offset: 0x4B4 | func: 2 | export: 2
void WCPressureSwitch_obj_Update(Object* self) { }

// offset: 0x4C0 | func: 3 | export: 3
void WCPressureSwitch_obj_Print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x514 | func: 4 | export: 4
void WCPressureSwitch_obj_Free(Object* self, s32 onlySelf) {
    PressureSwitch_Data* objdata = self->data;

    if (objdata->soundHandle) {
        dll_amSfx->Stop(objdata->soundHandle);
    }
    objFreeObjectType(self, OBJTYPE_TrickyTarget);
}

// offset: 0x594 | func: 5 | export: 5
u32 WCPressureSwitch_obj_GetModelFlags(Object* self) {
    PressurePad_Setup* setup = (PressurePad_Setup*)self->setup;
    s32 modelIndex;

    modelIndex = setup->modelIdx;
    if (modelIndex >= self->def->numModels) {
        modelIndex = 0;
    }
    
    return MODFLAGS_MODEL_INDEX(modelIndex) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x5C8 | func: 6 | export: 6
u32 WCPressureSwitch_obj_GetDataSize(Object* self, s32 offsetAddr) {
    return sizeof(PressureSwitch_Data);
}

// offset: 0x5DC | func: 7
void WCPressureSwitch_addObject(Object* self, Object* objectOnSwitch) {
    PressureSwitch_Data* objdata = self->data;
    u8 objectIndex;

    //@bug: should be && and "objectIndex != 9" (crashes game once objectsOnSwitch array overflows)
    for (objectIndex = 0; objdata->objectsOnSwitch[objectIndex] || objectIndex == 9; objectIndex++);

    objdata->objectsOnSwitch[objectIndex] = objectOnSwitch;    
    objdata->objCoords[objectIndex].x = objectOnSwitch->srt.transl.x;
    objdata->objCoords[objectIndex].y = objectOnSwitch->srt.transl.z;
}

// offset: 0x648 | func: 8
s32 WCPressureSwitch_isObjectOnSwitch(Object* self) {
    PressureSwitch_Data* objdata;
    Vec2f* coord;
    u8 index;
    u8 returnVal;

    objdata = self->data;

    returnVal = FALSE;

    for (index = 0; index < 10; index++) {
        if (!objdata->objectsOnSwitch[index])
            continue;

        coord = &objdata->objCoords[index];
        if (objdata->objectsOnSwitch[index]->srt.transl.x == coord->x && 
            objdata->objectsOnSwitch[index]->srt.transl.z == coord->y) {
            returnVal = TRUE;
        } else {
            objdata->objectsOnSwitch[index] = NULL;
        }
    }

    return returnVal;
}

// offset: 0x6CC | func: 9
static int WCPressureSwitch_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 prevCallbackValue) {
    PressureSwitch_Data* objdata;
    PressurePad_Setup* setup;
    u8 index;

    objdata = self->data;
    setup = (PressurePad_Setup*)self->setup;

    if (animObjData->lastMessage == 1) {
        for (index = 0; index < 10; index++) {
            if (objdata->objectsOnSwitch[index]) {
                objdata->objCoords[index].x = objdata->objectsOnSwitch[index]->srt.transl.x;
                objdata->objCoords[index].y = objdata->objectsOnSwitch[index]->srt.transl.z;
            }
        }
        animObjData->lastMessage = 0;

    } else if (animObjData->lastMessage == 2) {
        for (index = 0; index < 10; index++);

        self->srt.transl.z = setup->base.x; //@bug? should be x component?
        self->srt.transl.y = setup->base.y;
        self->srt.transl.z = setup->base.z;
        mainSetBits(setup->gameBitPressed, 0);
        animObjData->lastMessage = 0;
    }

    return 0;
}
