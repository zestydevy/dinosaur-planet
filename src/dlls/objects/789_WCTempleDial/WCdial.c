#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/gfx/model.h"

typedef struct {
    f32 rotateSpeed;
    f32 rotateSpeedGoal;
    u8 switchFlags;
    u8 dialFlags;
    f32* rotateSpeedGoals;
    s16* switchGamebits;
} WCTempleDial_Data;

typedef enum {
    WCTempleDial_MODEL_Sun,
    WCTempleDial_MODEL_Moon
} WCTempleDial_Models;

typedef enum {
    WCTempleDial_FLAG_Stopped = 1 //Puzzle solved
} WCTempleDial_Flags;

typedef enum {
    WCTempleDial_SWITCH_1 = 1,
    WCTempleDial_SWITCH_2 = 2,
    WCTempleDial_SWITCH_3 = 4
} WCTempleDial_SwitchFlags;

#define ALL_THREE_SWITCHES (\
    WCTempleDial_SWITCH_1 |\
    WCTempleDial_SWITCH_2 |\
    WCTempleDial_SWITCH_3\
)

/*0x0*/ static s16 dSunGamebits[] = {
    BIT_WC_Sun_Temple_Dial_Hit_Sunrise_Switch, 
    BIT_WC_Sun_Temple_Dial_Hit_Midday_Switch, 
    BIT_WC_Sun_Temple_Dial_Hit_Sunset_Switch
};
/*0x8*/ static s16 dMoonGamebits[] = {
    BIT_WC_Moon_Temple_Dial_Hit_Crescent_Moon_Switch,
    BIT_WC_Moon_Temple_Dial_Hit_Half_Moon_Switch,
    BIT_WC_Moon_Temple_Dial_Hit_Full_Moon_Switch
};
/*0x10*/ static f32 dSunRotateSpeedGoals[] = {
    32, 64, 128
};
/*0x1C*/ static f32 dMoonRotateSpeedGoals[] = {
    -32, -64, -128
};

static void WCTempleDial_setBlockIconFrames(Object* self, u8 iconStates);

// offset: 0x0 | ctor
void WCTempleDial_ctor(void* dll) { }

// offset: 0xC | dtor
void WCTempleDial_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCTempleDial_obj_Setup(Object* self, WCDial_Setup* objSetup, s32 reset) {
    WCTempleDial_Data* objData;
    s32 i;

    objData = self->data;
    self->srt.yaw = objSetup->yaw << 8;
    
    //Set model
    self->modelInstIdx = objSetup->type;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = WCTempleDial_MODEL_Sun;
    }
    
    //Set up gamebits and rotation speed arrays, based on model index
    if (self->modelInstIdx == WCTempleDial_MODEL_Sun) {
        objData->switchGamebits = dSunGamebits;
        objData->rotateSpeedGoals = dSunRotateSpeedGoals;
    } else {
        objData->switchGamebits = dMoonGamebits;
        objData->rotateSpeedGoals = dMoonRotateSpeedGoals;
    }
    
    //Restore state via the 3 switch gamebits
    for (i = 0; i < 3; i++) {
        if (mainGetBits(objData->switchGamebits[i])) {
            objData->switchFlags |= 1 << i;
        }
    }
    
    //Check if the dial puzzle is already finished
    if (mainGetBits(objSetup->gamebitFinished)) {
        objData->switchFlags = ALL_THREE_SWITCHES;
        objData->dialFlags |= WCTempleDial_FLAG_Stopped;
    }
    
    //Restore rotation speed based on how many switches have been lit
    if (objData->switchFlags & WCTempleDial_SWITCH_2) {
        objData->rotateSpeed = objData->rotateSpeedGoals[2];
    } else if (objData->switchFlags & WCTempleDial_SWITCH_1) {
        objData->rotateSpeed = objData->rotateSpeedGoals[1];
    } else {
        objData->rotateSpeed = objData->rotateSpeedGoals[0];
    }
    objData->rotateSpeedGoal = objData->rotateSpeed;
    
    //Update block's sun/moon icons
    WCTempleDial_setBlockIconFrames(self, objData->switchFlags);
}

// offset: 0x1BC | func: 1 | export: 1
void WCTempleDial_obj_Control(Object* self) {
    s32 switchIdx;
    s32 j;
    s32 wrongSwitchPressed;
    WCTempleDial_Data* objData;
    WCDial_Setup* objSetup;

    objData = self->data;
    objSetup = (WCDial_Setup*)self->setup;
    
    //When the puzzle's finished, update the sun/moon icons and return early
    if (objData->dialFlags & WCTempleDial_FLAG_Stopped) {
        WCTempleDial_setBlockIconFrames(self, objData->switchFlags);
        return;
    }
    
    //Accelerate/decelerate to meet goal rotation speed
    objData->rotateSpeed += (objData->rotateSpeedGoal - objData->rotateSpeed) * 0.01f * gUpdateRateF;

    //Rotate
    self->srt.roll += gUpdateRateF * objData->rotateSpeed;

    //Check if the 3 switches have been pressed
    for (switchIdx = 0; switchIdx < 3; switchIdx++) {
        //Check if the gamebit for each of the inactive switches becomes set
        if (!(objData->switchFlags & (1 << switchIdx)) && mainGetBits(objData->switchGamebits[switchIdx])) {
            //Check if the switch was pressed in the wrong order
            wrongSwitchPressed = FALSE;
            for (j = 0; j < switchIdx; j++) {
                if ((objData->switchFlags & (1 << j)) == FALSE) {
                    wrongSwitchPressed = TRUE;
                    break;
                }
            }
            
            if (wrongSwitchPressed) {
                //Reset all the switches
                for (j = 0; j < 3; j++) {
                    mainSetBits(objData->switchGamebits[j], 0);
                }
                
                dll_amSfx->Play(self, SOUND_912_Object_Refused, MAX_VOLUME, NULL, NULL, 0, NULL);
                
                //Reset back to initial state and rotation speed
                objData->switchFlags = 0;
                objData->rotateSpeedGoal = objData->rotateSpeedGoals[0];
                break;
            } else {
                dll_amSfx->Play(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);

                //Update switch flags and rotation speed
                objData->switchFlags |= (1 << switchIdx);
                if (switchIdx == 0) {
                    objData->rotateSpeedGoal = objData->rotateSpeedGoals[1];
                } else if (switchIdx == 1) {
                    objData->rotateSpeedGoal = objData->rotateSpeedGoals[2];
                }
            }
        }
    }
    
    //Update the three sun/moon icons to match the dial's state
    WCTempleDial_setBlockIconFrames(self, objData->switchFlags);
    
    //Check if the puzzle is finished
    if (objData->switchFlags == ALL_THREE_SWITCHES) {
        mainSetBits(objSetup->gamebitFinished, TRUE);
        objData->dialFlags |= WCTempleDial_FLAG_Stopped;
    }
}

// offset: 0x464 | func: 2 | export: 2
void WCTempleDial_obj_Update(Object* self) { }

// offset: 0x470 | func: 3 | export: 3
void WCTempleDial_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4C4 | func: 4 | export: 4
void WCTempleDial_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x4D4 | func: 5 | export: 5
u32 WCTempleDial_obj_GetModelFlags(Object* self) {
    WCDial_Setup* objSetup;
    s8 modelIdx;

    objSetup = (WCDial_Setup*)self->setup;
    
    modelIdx = objSetup->type;
    if (modelIdx >= self->def->numModels) {
        modelIdx = 0;
    }
    
    return MODFLAGS_MODEL_INDEX(modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x508 | func: 6 | export: 6
u32 WCTempleDial_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCTempleDial_Data);
}

// offset: 0x51C | func: 7
void WCTempleDial_setBlockIconFrames(Object* self, u8 iconStates) {
    Block* block;
    BlockTextureAnim* bTexAnim;
    BlockTextureAnimInstance* bTexAnimInstance;
    s32 i;

    block = mapGetBlockByIndex(mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z));
    if (block == NULL) {
        return;
    }
    
    for (i = 1; i < 4; i++) {
        bTexAnimInstance = blockTexanimGetInstance(block, i);
        if (bTexAnimInstance != NULL) {
            bTexAnim = blockTexanimGet(bTexAnimInstance->texanimID);
            if (iconStates & (1 << (i - 1))) {
                bTexAnim->unk4 = 0x100; //Use frame 1 (glowing symbol)
            } else {
                bTexAnim->unk4 = 0; //Use frame 0 (unlit symbol)
            }
        }
    }
}
