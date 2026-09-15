#include "PR/ultratypes.h"
#include "common.h"
#include "sys/math.h"
#include "sys/objtype.h"
#include "dlls/objects/common/sidekick.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 modelIdx;
/*1A*/ s16 gameBitPressed;             //Gamebit to set when the switch is pressed down
/*1C*/ u8 yOffsetAnimation;            //How far down the switch should move when pressed
/*1D*/ u8 yThreshold;                  //Threshold for other objects pressing switch
/*1E*/ u8 distanceGuardCommand;        //Player distance at which Guard sidekick command is selectable (only used by DLL 287)
/*20*/ s16 gamebitActivated;           //Gamebit to check if switch is deactivated
} PressureSwitch_Setup;

typedef struct {
/*00*/ u32 soundHandle;
/*04*/ s8 pressedTimer;
/*05*/ s8 state;
/*08*/ Object* objectsOnSwitch[10];
/*30*/ Vec2f objCoords[10];
} PressureSwitch_Data;

static void PressureSwitch_addObject(Object* self, Object* objectOnSwitch);
static s32 PressureSwitch_isObjectOnSwitch(Object* self);
static int PressureSwitch_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void PressureSwitch_ctor(void* dll) { }

// offset: 0xC | dtor
void PressureSwitch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void PressureSwitch_obj_Setup(Object* self, PressureSwitch_Setup* setup, s32 reset) {
    PressureSwitch_Data* objdata;
    s32 index;
    
    objdata = self->data;

    self->srt.yaw = setup->yaw << 8;
    self->stateFlags |= (OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED);

    self->modelInstIdx = setup->modelIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
        STUBBED_PRINTF("PRESSURESWITCH.c: modelno out of range romdefno=%d\n", setup->base.objId);
    }

    if (mainGetBits(setup->gameBitPressed)) {
        self->srt.transl.y = setup->base.y - setup->yOffsetAnimation;
        objdata->pressedTimer = 30;
    }

    objAddObjectType(self, OBJTYPE_TrickyTarget);

    for (index = 0; index < 10; index++) { objdata->objectsOnSwitch[index] = 0; }

    self->animCallback = PressureSwitch_animCallback;
}

// offset: 0x148 | func: 1 | export: 1
void PressureSwitch_obj_Control(Object* self) {
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
    if (setup->gamebitActivated > 0 && !mainGetBits(setup->gamebitActivated)) {
        return;
    }

    //Decrement timer until not considered pressed (@framerate-dependent)
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
                PressureSwitch_addObject(self, listedObject);
            }
        }
    }

    //Check if object on switch
    if (PressureSwitch_isObjectOnSwitch(self)) {
        objdata->pressedTimer = 5;
    }

    //Animate the switch's y coordinate
    playSound = FALSE;
    if (objdata->pressedTimer != 0) {
        deltaY = setup->base.y - setup->yOffsetAnimation;
        if (self->srt.transl.y < deltaY) {
            self->srt.transl.y += 0.25f * gUpdateRateF;
            if (self->srt.transl.y > deltaY) {
                self->srt.transl.y = deltaY;
            }
            mainSetBits(setup->gameBitPressed, 1);
        } else {
            self->srt.transl.y -= 0.125f * gUpdateRateF;
            if (self->srt.transl.y < deltaY) {
                self->srt.transl.y = deltaY;
                mainSetBits(setup->gameBitPressed, 1);
            } else {
                playSound = TRUE;
            }
        }
    } else {
        self->srt.transl.y += 0.125f * gUpdateRateF;

        if (self->srt.transl.y > setup->base.y) {
            self->srt.transl.y = setup->base.y;
            mainSetBits(setup->gameBitPressed, 0);
        } else {
            playSound = TRUE;
        }
    }

    //Play stone rumbling sound when moving
    if (playSound) {
        if (!objdata->soundHandle) {
            dll_amSfx->Play(self, SOUND_1e1_Stone_Moving_Loop, MAX_VOLUME, &objdata->soundHandle, 0, 0, 0);
        }
    } else {
        if (objdata->soundHandle) {
            dll_amSfx->Stop(objdata->soundHandle);
            objdata->soundHandle = 0;
        }
    }

    //Show Guard command option when player close to switch
    if (!mainGetBits(setup->gameBitPressed) && setup->distanceGuardCommand) {
        player = objGetPlayer();
        sidekick = objGetSidekick();
        if (sidekick) {
            if (vec3DistanceSquared(&self->globalPosition, &player->globalPosition) <= SQ(setup->distanceGuardCommand)) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->EnableCommand(sidekick, Sidekick_Command_INDEX_3_Guard);
            }
        }
    }
}

// offset: 0x524 | func: 2 | export: 2
void PressureSwitch_obj_Update(Object* dll) { }

// offset: 0x530 | func: 3 | export: 3
void PressureSwitch_obj_Print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gfx, mtx, vtx, pols, 1.0f);
    }
}

// offset: 0x584 | func: 4 | export: 4
void PressureSwitch_obj_Free(Object* self, s32 onlySelf) {
    PressureSwitch_Data* objdata = self->data;

    if (objdata->soundHandle) {
        dll_amSfx->Stop(objdata->soundHandle);
    }

    objFreeObjectType(self, OBJTYPE_TrickyTarget);
}

// offset: 0x604 | func: 5 | export: 5
u32 PressureSwitch_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x614 | func: 6 | export: 6
u32 PressureSwitch_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(PressureSwitch_Data);
}

// offset: 0x628 | func: 7
void PressureSwitch_addObject(Object* self, Object* objectOnSwitch) {
    PressureSwitch_Data* objdata = self->data;
    u8 objectIndex;

    //@bug: should be && and "objectIndex != 9" (crashes game once objectsOnSwitch array overflows)
    for (objectIndex = 0; objdata->objectsOnSwitch[objectIndex] || objectIndex == 9; objectIndex++);

    objdata->objectsOnSwitch[objectIndex] = objectOnSwitch;    
    objdata->objCoords[objectIndex].x = objectOnSwitch->srt.transl.x;
    objdata->objCoords[objectIndex].y = objectOnSwitch->srt.transl.z;
}

// offset: 0x694 | func: 8
s32 PressureSwitch_isObjectOnSwitch(Object* self) {
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

// offset: 0x718 | func: 9
static int PressureSwitch_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 prevCallbackValue) {
    PressureSwitch_Data* objdata;
    PressureSwitch_Setup* setup;
    u8 index;

    objdata = self->data;
    setup = (PressureSwitch_Setup*)self->setup;

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

/*0x34*/ static const char str_34[] = "";
/*0x38*/ static const char str_38[] = "";
/*0x3C*/ static const char str_3C[] = "";
