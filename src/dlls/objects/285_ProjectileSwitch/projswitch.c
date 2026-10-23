#include "common.h"
#include "game/objects/setups/85_ProjectileTarget.h"
#include "sys/gfx/model.h"

typedef struct {
    u8 switchPressed;
    f32 resetTimer;
} ProjectileSwitch_Data;

typedef enum {
    Switch_FLAG_Can_Be_Toggled_Via_Attacks = 1, //Repeated Projectile Spell hits will toggle the switch's state
    Switch_FLAG_Resets_After_Delay = 2          //Switch resets resets to its unpressed state after a number of seconds (specified by `objData->resetDelay`)
} ProjectileSwitch_Flags;

static void ProjectileSwitch_changeState(Object* self, int switchPressed, int playSound);

// offset: 0x0 | ctor
void ProjectileSwitch_ctor(void* dll) { }

// offset: 0xC | dtor
void ProjectileSwitch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void ProjectileSwitch_obj_Setup(Object* self, ProjectileTarget_Setup* objSetup, s32 reset) {
    ProjectileSwitch_Data* objData = self->data;
    
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.pitch = objSetup->pitch << 8;
    
    //Set scale
    if (objSetup->scale == 0) {
        self->srt.scale = self->def->scale;
    } else {
        self->srt.scale = objSetup->scale * self->def->scale * (1.0f / 64.0f);
    }
    self->objhitInfo->unk52 = (objSetup->scale * self->def->hitbox_flagsB6) / 64;
    
    //Set model instance
    self->modelInstIdx = objSetup->settings >> 2;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    
    //Check if the switch is already pressed
    objData->switchPressed = mainGetBits(objSetup->gamebit);
    if (objData->switchPressed) {
        ProjectileSwitch_changeState(self, TRUE, FALSE);
    } else {
        ProjectileSwitch_changeState(self, FALSE, FALSE);
    }
    
    if (objSetup->enableTint == FALSE) {
        self->stateFlags |= OBJSTATE_PRINT_DISABLED;
    }
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x1B0 | func: 1 | export: 1
void ProjectileSwitch_obj_Control(Object* self) {
    ProjectileTarget_Setup* objSetup;
    ProjectileSwitch_Data* objData;

    objData = self->data;
    objSetup = (ProjectileTarget_Setup*)self->setup;
    
    if (objData->switchPressed && (mainGetBits(objSetup->gamebit) == FALSE)) {
        ProjectileSwitch_changeState(self, FALSE, TRUE);
    }
    
    //If the switch has an active delay timer, wait for it to expire
    if (objData->resetTimer > 0.0f) {
        objData->resetTimer -= (f32)gUpdateRate;
        if (objData->resetTimer <= 0.0f) {
            objData->resetTimer = 0.0f;
            mainSetBits(objSetup->gamebit, 0);
        } else {
            return;
        }
    }

    //React to Projectile Switch attacks
    if (func_80025F40(self, NULL, NULL, NULL) == Damage_Type_Projectile) {
        if (objData->switchPressed) {
            if ((objSetup->settings & 3) == Switch_FLAG_Can_Be_Toggled_Via_Attacks) {
                ProjectileSwitch_changeState(self, FALSE, TRUE);
                mainSetBits(objSetup->gamebit, 0);
            }
        } else {
            ProjectileSwitch_changeState(self, TRUE, TRUE);
            mainSetBits(objSetup->gamebit, 1);
            if ((objSetup->settings & 3) == Switch_FLAG_Resets_After_Delay) {
                objData->resetTimer = objSetup->resetDelay * 0.1f * 60.0f;
            }
        }
    }
}

// offset: 0x398 | func: 2 | export: 2
void ProjectileSwitch_obj_Update(Object* self) { }

// offset: 0x3A4 | func: 3 | export: 3
void ProjectileSwitch_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    ProjectileTarget_Setup* objSetup = (ProjectileTarget_Setup*)self->setup;
    
    if (visibility) {
        if (objSetup->enableTint) {
            objprintSetMultiplierColor(objSetup->tintR, objSetup->tintG, objSetup->tintB);
        }
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x440 | func: 4 | export: 4
void ProjectileSwitch_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x450 | func: 5 | export: 5
u32 ProjectileSwitch_obj_GetModelFlags(Object* self) {
    ProjectileTarget_Setup* objSetup;
    s32 modelIndex;

    objSetup = (ProjectileTarget_Setup*) self->setup;
               
    modelIndex = objSetup->settings >> 2;
    if (modelIndex >= self->def->numModels) {
        modelIndex = 0;
    }
    
    return MODFLAGS_MODEL_INDEX(modelIndex) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x488 | func: 6 | export: 6
u32 ProjectileSwitch_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(ProjectileSwitch_Data);
}

// offset: 0x49C | func: 7
void ProjectileSwitch_changeState(Object* self, int switchPressed, int playSound) {
    ProjectileSwitch_Data* objData;
    TextureAnimator* texAnim;
    s32 soundID;

    objData = self->data;
    
    //Optionally play a state change sound
    if (playSound) {
        soundID = switchPressed ? SOUND_7AE_Switch_Hit_Blast : SOUND_7AF_Switch_Reset_Swoosh;
        dll_amSfx->Play(self, soundID, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    
    //Set texture frame
    texAnim = objExprGetTexAnimator(self, 0, 0);
    if (texAnim != NULL) {
        texAnim->frame = switchPressed << 8;
    }
    
    //Set state
    objData->switchPressed = switchPressed;
}
