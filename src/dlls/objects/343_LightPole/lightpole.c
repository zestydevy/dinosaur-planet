#include "common.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "macros.h"

typedef struct {
    ObjSetup base;
    s8 endFrameNoGamebit;           //When no gamebitFinished is specified, the TextureAnimator will hold on this frame at the end of its animation
    s8 halfYaw;                     //Yaw (shifted by 9 instead of the usual 8)
    s16 endFrame;                   //The TextureAnimator will finish animating once it reaches this frame
    s16 animSpeed;                  //Animation speed for the TextureAnimator
    s16 gamebitFinished;            //GamebitID set when the TextureAnimator's animation is finished
    s16 gamebitStart;               //The TextureAnimator's animation begins when this gamebit is set
} LightPole_Setup;

typedef struct {
    s32 animatorID;                 //Which model texture to animate
    u8 animSpeed;                   //Animation speed for the TextureAnimator
    s32 endFrameValue;              //The TextureAnimator will finish animating once it reaches this frame value threshold
    s32 endFrameValueNoGamebit;     //When no gamebitFinished is specified, the TextureAnimator will hold on this frame value at the end of its animation
    s32 frameValue;                 //The current frame value for the TextureAnimator
    u8 flagUnused : 1;
    u8 flagAnimFinished : 1;        //Whether the TextureAnimator has reached its end value
    u8 flagAnimStarted : 1;         //Whether the TextureAnimator has started animating (via a gamebit)
} LightPole_Data;

// offset: 0x0 | ctor
void LightPole_ctor(void* dll) { }

// offset: 0xC | dtor
void LightPole_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void LightPole_setup(Object* self, LightPole_Setup* objSetup, s32 reset) {
    LightPole_Data* objData = self->data;

    objData->animatorID = 0;
    
    self->srt.yaw = objSetup->halfYaw << 9;
    
    objData->endFrameValue = objSetup->endFrame << 8;
    objData->animSpeed = objSetup->animSpeed;
    objData->endFrameValueNoGamebit = objSetup->endFrameNoGamebit << 8;

    if (!objData->animSpeed) {
        STUBBED_PRINTF(" TFRAME ANIMATOR: no speed give \n");
    }

    //Restore the texture animation's state
    if ((objData->flagAnimFinished = mainGetBits(objSetup->gamebitFinished))) {
        objData->frameValue = objData->endFrameValue;
        objData->flagAnimStarted = TRUE;
    }
    
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    self->stateFlags |= OBJSTATE_PRINT_DISABLED;
}

// offset: 0xE4 | func: 1 | export: 1
void LightPole_control(Object* self) {
    LightPole_Data* objData;
    TextureAnimator* texAnim;
    LightPole_Setup* objSetup;

    objData = self->data;
    objSetup = (LightPole_Setup*)self->setup;

    //Check if the texture animation should start
    if (objData->flagAnimStarted == FALSE) {
        if ((mainGetBits(objSetup->gamebitStart)) && (objData->flagAnimFinished == FALSE)) {
            objData->frameValue = 0;
            objData->flagAnimStarted = TRUE;
        }
    }
    
    //Advance the texture animation when started
    if (objData->flagAnimStarted) {
        texAnim = objExprGetTexAnimator(self, objData->animatorID, 0);
        if (texAnim == NULL) {
            return;
        }

        objData->frameValue += objData->animSpeed;

        if (objData->frameValue < 0) {
            objData->frameValue = 0;
        } else if (objData->frameValue > objData->endFrameValue) {
            //Handle ending the texture animation and setting a gamebit
            if (objSetup->gamebitFinished != NO_GAMEBIT) {
                STUBBED_PRINTF(" Set bit %i ", objSetup->gamebitFinished);
                mainSetBits(objSetup->gamebitFinished, TRUE);

                objData->flagAnimStarted = FALSE;
                objData->flagAnimFinished = TRUE;
                objData->frameValue = objData->endFrameValue;
            } else {
                //Set a different end frame value if a gamebit wasn't specified
                objData->frameValue = objData->endFrameValueNoGamebit;
            }
        }

        texAnim->frame = objData->frameValue;
    }
}

// offset: 0x240 | func: 2 | export: 2
void LightPole_update(Object* self) { }

// offset: 0x24C | func: 3 | export: 3
void LightPole_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x264 | func: 4 | export: 4
void LightPole_free(Object* self, s32 onlySelf) { }

// offset: 0x274 | func: 5 | export: 5
u32 LightPole_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x284 | func: 6 | export: 6
u32 LightPole_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(LightPole_Data);
}

/*0x34*/ static const char str_34[] = "";
/*0x38*/ static const char str_38[] = "";
/*0x3C*/ static const char str_3C[] = "";
