#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/21_gametext.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/model.h"
#include "sys/fonts.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "sys/objects.h"

typedef struct {
    ObjSetup base;
    s16 gamebitShown;
    s16 unk1A;
    s16 unk1C;
    s16 textID;
    u8 activationRadius;
} LevelName_Setup;

typedef struct {
/*00*/ GameTextChunk* gametext;
/*04*/ char* strings;
/*08*/ u32 displayDuration;
/*0C*/ u8 activationRadius;
/*0D*/ s8 unusedD;
/*0E*/ s16 gamebitShown;
/*10*/ s16 timer;
/*12*/ s16 opacity;
/*14*/ u8 state;
/*15*/ u8 pad[3];
} LevelName_Data;

enum LevelNameStates{
    LevelName_STATE_0_WAITING = 0,
    LevelName_STATE_1_FADING_IN = 1,
    LevelName_STATE_2_HOLDING = 2,
    LevelName_STATE_3_FADING_OUT = 3,
    LevelName_STATE_4_FINISHED = 4
};

//Unused textureIDs?
/*0x0*/ static u16 _data_0[] = {
    0x4449, 
    0x414d, 
    0x4f4e, 
    0x4420, 
    0x4241, 
    0x5900, 
    0x01ef, 
    0x01f0, 
    0x01f1,
    0x01f2,
    0x01f3, 
    0x01f4, 
    0x01f5, 
    0x01f6
};

static int LevelName_animCallback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void LevelName_ctor(void* dll) { }

// offset: 0xC | dtor
void LevelName_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void LevelName_obj_Setup(Object* self, LevelName_Setup* setup, s32 reset) {
    LevelName_Data* objdata;
    GameTextChunk* gametext;

    objdata = self->data;

    fontLoad(FONT_DINO_MEDIUM_FONT_IN);
    self->animCallback = LevelName_animCallback;

    gametext = gDLL_21_Gametext->vtbl->get_chunk(setup->textID);

    objdata->strings = gametext->strings[0];
    objdata->displayDuration = gametext->commands[0];
    objdata->gametext = gametext;
    objdata->activationRadius = setup->activationRadius;
    objdata->gamebitShown = setup->gamebitShown;
    objdata->opacity = 0;

    objdata->state = LevelName_STATE_0_WAITING;
    objdata->timer = objdata->opacity;

    if ((objdata->gamebitShown != NO_GAMEBIT) && mainGetBits(objdata->gamebitShown)) {
        objdata->state = LevelName_STATE_4_FINISHED;
    }
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x120 | func: 1 | export: 1
void LevelName_obj_Control(Object* self) {
    LevelName_Data* objdata;
    f32 distance;

    objdata = self->data;

    switch (objdata->state) {
        case LevelName_STATE_0_WAITING:
            distance = vec3Distance(&self->globalPosition, &objGetPlayer()->globalPosition);
            if (distance < objdata->activationRadius) {
                if (objdata->gamebitShown != NO_GAMEBIT) {
                    mainSetBits(objdata->gamebitShown, 1);
                }
                objdata->state = LevelName_STATE_1_FADING_IN;
            }
            break;
        case LevelName_STATE_1_FADING_IN:
            objdata->opacity += gUpdateRate * 4;
            if (objdata->opacity > 220) {
                objdata->opacity = 220;
                objdata->state = LevelName_STATE_2_HOLDING;
            }
            break;
        case LevelName_STATE_2_HOLDING:
            objdata->timer += gUpdateRate;
            if ((u32)objdata->timer > objdata->displayDuration) {
                objdata->state = LevelName_STATE_3_FADING_OUT;
            }    
            //Opacity pulses sinusoidally (51.2 frames for a full cycle)
            objdata->opacity = (s32) (mathSinfInterp(objdata->timer * 0x500) * 30.0f) + 220;
            break;
        case LevelName_STATE_3_FADING_OUT:
            objdata->opacity -= gUpdateRate * 4;
            if (objdata->opacity < 0) {
                objdata->opacity = 0;
                objdata->state = LevelName_STATE_4_FINISHED;
            }
            break;
        case LevelName_STATE_4_FINISHED:
            break;
    }
}

// offset: 0x304 | func: 2 | export: 2
void LevelName_obj_Update(Object* self) { }

// offset: 0x310 | func: 3 | export: 3
void LevelName_obj_Print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    s32 index;
    s32 yCoord;
    LevelName_Data* objdata;
    GameTextChunk* gametext;

    yCoord = 80;
    objdata = self->data;
    if (objdata->opacity == 0) {
        return;
    }

    gametext = objdata->gametext;
    fontWindowSetCoords(6, 0, 0, 320, 240);
    fontWindowUseFont(6, FONT_DINO_MEDIUM_FONT_IN);
    fontWindowFlushStrings(6);
    fontWindowSetTextColour(6, 0xFF, 0xFF, 0xFF, 0, objdata->opacity);

    for (index = 0; index < gametext->count; index++, yCoord += 30) {
        fontWindowAddStringXY(6, -0x8000, yCoord, 
            gametext->strings[index], 1, ALIGN_TOP_CENTER);
        fontWindowDraw(gfx, mtx, vtx, 6);
    }
}

// offset: 0x480 | func: 4 | export: 4
void LevelName_obj_Free(Object* self, s32 onlySelf) {
    LevelName_Data* objdata = self->data;

    fontUnload(FONT_DINO_MEDIUM_FONT_IN);
    mmFree(objdata->gametext);
}

// offset: 0x4E4 | func: 5 | export: 5
u32 LevelName_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x4F4 | func: 6 | export: 6
u32 LevelName_obj_GetDataSize(Object* self, s32 offsetAddr) {
    return sizeof(LevelName_Data);
}

// offset: 0x508 | func: 7
static int LevelName_animCallback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    LevelName_Data* objdata;
    s32 i;

    objdata = self->data;

    for (i = 0; i < animData->messageCount; i++) {
        if (animData->messages[i] == 1) {
            if (objdata->gamebitShown != NO_GAMEBIT) {
                mainSetBits(objdata->gamebitShown, 1);
            }
            objdata->state = LevelName_STATE_1_FADING_IN;
            return 4;
        } 
    }

    return 0;
}
