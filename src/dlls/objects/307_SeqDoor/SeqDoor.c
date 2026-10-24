// NOTE: this DLL's official name is unknown. The DLL is shared by many sequence-driven door objects 
// (e.g. WCCageDoor, CF_MovDeckDoor, DIM2PuzzleDoor), so the name "SeqDoor" has been used here!

#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/setups/87_SeqDoor.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "dll.h"
#include "dlls/objects/307_SeqDoor.h"

typedef struct {
    f32 sinYaw;                  //Used to streamline calculating whether the camera is in front of/behind the door
    f32 cosYaw;                  //Used to streamline calculating whether the camera is in front of/behind the door
    f32 worldOriginObjectSpaceZ; //Used to streamline calculating whether the camera is in front of/behind the door
    u8 state;                    //The door's state machine value (handled in animCallback only)
    u8 startSequence;            //Whether the control function should start playing the door's objSeq (always activates the first time control runs)
    u8 flags;                    //Tracks whether gamebitOpenA and gamebitOpenB (if specified) are set - used to apply textureAnimators to the door, etc.
} SeqDoor_Data;

typedef enum {
    SeqDoor_STATE_0_Closed,
    SeqDoor_STATE_1_Open,
    SeqDoor_STATE_2_Opening,
    SeqDoor_STATE_3_Closing
} SeqDoor_States;

typedef enum {
    SeqDoor_FLAG_1_Activated_A = 1,
    SeqDoor_FLAG_2_Activated_B = 2
} SeqDoor_Flags;

static int SeqDoor_animCallback(Object *self, Object *animObj, AnimObj_Data *animData, s8 prevCallbackValue);
static void SeqDoor_setCameraPositionGamebits(SeqDoor_Data *objData, SeqDoor_Setup *objSetup);

// offset: 0x0 | ctor
void SeqDoor_ctor(void* dll) { }

// offset: 0xC | dtor
void SeqDoor_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void SeqDoor_obj_Setup(Object* self, SeqDoor_Setup* objSetup, s32 reset) {
    SeqDoor_Data* objData = self->data;
    
    objData->startSequence = TRUE;
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = SeqDoor_animCallback;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;

    //Set scale
    {
        self->srt.scale = objSetup->scale * (1.0f / 64.0f);
        if (self->srt.scale == 0.0f) {
            self->srt.scale = 1.0f;
        }
        self->srt.scale *= self->def->scale;
    }

    //Restore state via gamebit
    if (objSetup->gamebitRestoreState != NO_GAMEBIT) {
        objData->state = mainGetBits(objSetup->gamebitRestoreState);
    } else {
        objData->state = SeqDoor_STATE_0_Closed;
    }

    //Project the world origin onto the door's objectSpace Z-axis
    //(Helps with later calcs determining whether the camera is in front of or behind the door)
    objData->sinYaw = mathSinfInterp(self->srt.yaw);
    objData->cosYaw = mathCosfInterp(self->srt.yaw);
    objData->worldOriginObjectSpaceZ = -((objData->sinYaw * self->srt.transl.x) + (objData->cosYaw * self->srt.transl.z));

    //Set flags based on gamebits (for WCDoorSlab lighting up its Moon/Sun icons, etc.)
    objData->flags = 0;
    if (mainGetBits(objSetup->gamebitOpenA)) {
        objData->flags |= SeqDoor_FLAG_1_Activated_A;
    }
    if (mainGetBits(objSetup->gamebitOpenB)) {
        objData->flags |= SeqDoor_FLAG_2_Activated_B;
    }
}

// offset: 0x1C8 | func: 1 | export: 1
void SeqDoor_obj_Control(Object* self) {
    SeqDoor_Data* objData;
    SeqDoor_Setup* objSetup;
    s32 enabledActors;

    objSetup = (SeqDoor_Setup*)self->setup;
    objData = self->data;

    if (objData->startSequence) {
        //Skip the door's objSeq to its preemptTime, if the door's not in its initial state
        if (objSetup->preemptTime && (objData->state != SeqDoor_STATE_0_Closed)) {
            enabledActors = objSetup->preemptEnabledActors;
            gDLL_3_Animation->vtbl->preempt_sequence_time(self, objSetup->preemptTime);
        } else {
            enabledActors = -1;
        }
        
        //Play the door's sequence
        if (objSetup->objSeqIdx != -1) {
            gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->objSeqIdx, self, enabledActors);
        }

        objData->startSequence = FALSE;
    }
}

// offset: 0x2A8 | func: 2 | export: 2
void SeqDoor_obj_Update(Object *self) { }

// offset: 0x2B4 | func: 3 | export: 3
void SeqDoor_obj_Print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x308 | func: 4 | export: 4
void SeqDoor_obj_Free(Object *self, s32 a1) { }

// offset: 0x318 | func: 5 | export: 5
u32 SeqDoor_obj_GetModelFlags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x328 | func: 6 | export: 6
u32 SeqDoor_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(SeqDoor_Data);
}

// offset: 0x33C | func: 7
static int SeqDoor_animCallback(Object *self, Object *animObj, AnimObj_Data *animData, s8 prevCallbackValue) {
    SeqDoor_Data *objData;
    SeqDoor_Setup *objSetup;
    TextureAnimator *animator;
    u32 activatedA;
    s32 activatedB;
    s32 i;

    objData = (SeqDoor_Data*)self->data;
    objSetup = (SeqDoor_Setup*)self->setup;

    //Change animated texture frames using a TextureAnimator (used by WCSlabDoor to light up the Moon/Sun icons on the door)
    if (self->def->numAnimatedFrames != 0) {
        //Light up moon icon (textureAnimator0)
        if (objData->flags & SeqDoor_FLAG_1_Activated_A) {
            animator = objExprGetTexAnimator(self, 0, 0);
            if (animator != NULL) {
                animator->frame = 0x100;
            }
        }

        //Light up sun icon (textureAnimator1)
        if (objData->flags & SeqDoor_FLAG_2_Activated_B) {
            animator = objExprGetTexAnimator(self, 1, 0);
            if (animator != NULL) {
                animator->frame = 0x100;
            }
        }
    }

    //State Machine (NOTE: only runs while the door is in a sequence!)
    {
        if (objData->state == SeqDoor_STATE_0_Closed) {
            activatedA = mainGetBits(objSetup->gamebitOpenA);
            activatedB = FALSE;

            //Check if gamebitOpenB is set too (if gamebitOpenB wasn't specified, just ignore it and assume it's set)
            if ((objSetup->gamebitOpenB == NO_GAMEBIT) || mainGetBits(objSetup->gamebitOpenB)) {
                activatedB = TRUE;
            }

            //Flag that gamebitOpenA is set, and play a sound if this will queue textureAnimator0
            if (activatedA && ((objData->flags & SeqDoor_FLAG_1_Activated_A) == FALSE)) {
                if (self->def->numAnimatedFrames != 0) {
                    dll_amSfx->Play(self, SOUND_9A3_Magic_Reverse_Cymbal, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
                objData->flags |= SeqDoor_FLAG_1_Activated_A;
            }

            //Flag that gamebitOpenB is set, and play a sound if this will queue textureAnimator1
            if (activatedB && ((objData->flags & SeqDoor_FLAG_2_Activated_B) == FALSE)) {
                if (self->def->numAnimatedFrames != 0) {
                    dll_amSfx->Play(self, SOUND_9A3_Magic_Reverse_Cymbal, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
                objData->flags |= SeqDoor_FLAG_2_Activated_B;
            }

            //Advance state when gamebitOpenA and gamebitOpenB are set (or when just gamebitOpenA is set, if gamebitOpenB isn't specified)
            if (objData->flags == (SeqDoor_FLAG_1_Activated_A | SeqDoor_FLAG_2_Activated_B)) {
                SeqDoor_setCameraPositionGamebits(objData, objSetup);
                objData->state = SeqDoor_STATE_2_Opening;
            }
        } else if ((objData->state == SeqDoor_STATE_1_Open) && (mainGetBits(objSetup->gamebitOpenA) == FALSE)) {
            //Start closing if gamebitOpenA unsets while the door is open
            objData->state = SeqDoor_STATE_3_Closing;
        }

        //Advance states from opening->open and closing->closed, based on objSeq messages
        if (objData->state == SeqDoor_STATE_2_Opening) {
            for (i = 0; i < animData->messageCount; i++) {
                if (animData->messages[i] == SeqDoor_SEQCMD_2_Finished_Opening) {
                    objData->state = SeqDoor_STATE_1_Open;
                    if (objSetup->gamebitRestoreState != NO_GAMEBIT) {
                        mainSetBits(objSetup->gamebitRestoreState, SeqDoor_STATE_1_Open);
                    }
                }
            }
        } else if (objData->state == SeqDoor_STATE_3_Closing) {
            for (i = 0; i < animData->messageCount; i++) {
                if (animData->messages[i] == SeqDoor_SEQCMD_1_Finished_Closing) {
                    SeqDoor_setCameraPositionGamebits(objData, objSetup);
                    objData->state = SeqDoor_STATE_0_Closed;
                    objData->flags = 0;
                    if (objSetup->gamebitRestoreState != NO_GAMEBIT) {
                        mainSetBits(objSetup->gamebitRestoreState, SeqDoor_STATE_0_Closed);
                    }
                }
            }
        }
    }

    return !(objData->state == SeqDoor_STATE_2_Opening) && !(objData->state == SeqDoor_STATE_3_Closing);
}

// offset: 0x6E4 | func: 8
static void SeqDoor_setCameraPositionGamebits(SeqDoor_Data *objData, SeqDoor_Setup *objSetup) {
    Camera *camera;
    f32 distance;
    u8 value;

    camera = camGetMain();

    //Get the camera's distance from the door along the door's local objectSpace Z-axis
    //(projects the camera's XZ coords onto the door's own Z-axis, then subtracts the world origin's projected distance on door's own Z-axis)
    distance = (objData->sinYaw * camera->srt.transl.x) + (objData->cosYaw * camera->srt.transl.z) + objData->worldOriginObjectSpaceZ;

    //Check whether the camera is in front of/behind the door
    if (distance < 0.0f) {
        //Camera is behind the door
        if (objSetup->gamebitCameraBack != NO_GAMEBIT) {
            //Flip a section of the behind gamebit's value
            value = mainGetBits(objSetup->gamebitCameraBack);
            value ^= objSetup->flipBitsCameraBack;
            mainSetBits(objSetup->gamebitCameraBack, value);
        }
    } else {
        //Camera is in front of the door
        if (objSetup->gamebitCameraFront != NO_GAMEBIT) {
            //Flip a section of the front gamebit's value
            value = mainGetBits(objSetup->gamebitCameraFront);
            value ^= objSetup->flipBitsCameraFront;
            mainSetBits(objSetup->gamebitCameraFront, value);
        }
    }
}
