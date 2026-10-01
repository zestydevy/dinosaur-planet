#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "types.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebit;   // LOOPING SOUNDS: start/stop when value changes, or use distance check if gamebit unspecified.
                      // ONE-SHOT SOUNDS: play sound when value changes (if "Play At Random Inside Radius" mode isn't being used).
/*1A*/ s16 soundID;
/*1C*/ u8 flags;      //See `SfxPlayer_Flags`.
/*1D*/ u8 halfRadius; //Stored halved, the sound plays when the player is inside this radius. Looping sounds stop when the player is outside of (radius + 10).
} SfxPlayer_Setup;

typedef struct {
/*00*/ f32 innerDistanceSq;
/*04*/ f32 outerDistanceSq;
/*08*/ u8 prevGamebitValue;
/*0C*/ u32 soundHandle;
} SfxPlayer_Data;

typedef enum {
    SfxPlayer_FLAG_1_Looping_Sound = 1,               //Use this if the sound loops (plays with a soundHandle, and stops it when sound finished) [Has priority over "play at random inside radius" mode]
    SfxPlayer_FLAG_2_Play_if_Gamebit_Set = 2,
    SfxPlayer_FLAG_4_Play_if_Gamebit_Unset = 4,
    SfxPlayer_FLAG_8_Play_At_Random_Inside_Radius = 8 //Sound plays at random intervals when player is inside radius [Has priority over one-shot sound "play by gamebit" modes]
} SfxPlayer_Flags;

// offset: 0x0 | ctor
void sfxPlayer_ctor(void* dll) { }

// offset: 0xC | dtor
void sfxPlayer_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void sfxPlayer_obj_Setup(Object* self, SfxPlayer_Setup* objSetup, s32 reset) {
    SfxPlayer_Data* objData;
    SfxPlayer_Setup* setup;

    objData = self->data;
    setup = (SfxPlayer_Setup*)self->setup;

    objData->prevGamebitValue = mainGetBits(objSetup->gamebit);

    if ((setup->flags & SfxPlayer_FLAG_1_Looping_Sound) && 
        (objData->prevGamebitValue == FALSE) && 
        (setup->flags & SfxPlayer_FLAG_4_Play_if_Gamebit_Unset)
    ) {
        objData->prevGamebitValue = TRUE;
    }

    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;

    objData->innerDistanceSq = objSetup->halfRadius * 2;
    objData->outerDistanceSq = objData->innerDistanceSq + 10.0f;

    objData->innerDistanceSq = SQ(objData->innerDistanceSq);
    objData->outerDistanceSq = SQ(objData->outerDistanceSq);
}

// offset: 0xFC | func: 1 | export: 1
void sfxPlayer_obj_Control(Object* self) {
    #define PLAY_BY_GAMEBIT (SfxPlayer_FLAG_2_Play_if_Gamebit_Set | SfxPlayer_FLAG_4_Play_if_Gamebit_Unset)
    #define PLAY_IF_UNSET ((flags & SfxPlayer_FLAG_4_Play_if_Gamebit_Unset) == SfxPlayer_FLAG_4_Play_if_Gamebit_Unset)
    SfxPlayer_Data* objData;
    SfxPlayer_Setup* setup;
    u8 gamebitValue;
    f32 playerDistanceSq;
    u8 flags;

    setup = (SfxPlayer_Setup*)self->setup;
    objData = self->data;

    gamebitValue = FALSE;
    if (setup->gamebit) {
        gamebitValue = mainGetBits(setup->gamebit);
    }

    flags = setup->flags;

    if (flags & SfxPlayer_FLAG_1_Looping_Sound) {
        //LOOPING SOUND

        if (setup->gamebit != NO_GAMEBIT) {
            //Play by gamebit
            if ((gamebitValue != objData->prevGamebitValue) && (flags & PLAY_BY_GAMEBIT)) {
                if ((gamebitValue != PLAY_IF_UNSET) != 0) {
                    //Start the looping sound when the gamebit is set (or when it's unset, if the opposite mode is being used)
                    objData->soundHandle = dll_amSfx->Play(self, setup->soundID, MAX_VOLUME, NULL, 0, 0, 0);
                } else if (objData->soundHandle) {
                    //Stop the looping sound when the gamebit is unset (or when it's unset, if the opposite mode is being used)
                    dll_amSfx->Stop(objData->soundHandle);
                    objData->soundHandle = 0;
                }
            }
        } else if (flags & PLAY_BY_GAMEBIT) { //NOTE: one of these flags must be set, even though it doesn't check gamebits in this mode
            //Play by distance
            playerDistanceSq = vec3DistanceSquared(&self->globalPosition, &objGetPlayer()->globalPosition);

            //Start sound when inside inner radius, stop sound when leaving outer radius
            if (!objData->soundHandle && (playerDistanceSq < objData->innerDistanceSq)) {
                dll_amSfx->Play(self, setup->soundID, MAX_VOLUME, &objData->soundHandle, 0, 0, 0);
            } else if ((objData->outerDistanceSq < playerDistanceSq) && objData->soundHandle) {
                dll_amSfx->Stop(objData->soundHandle);
                objData->soundHandle = 0;
            }
        }
    } else {
        //ONE-SHOT SOUND

        if (flags & SfxPlayer_FLAG_8_Play_At_Random_Inside_Radius) {
            //Play by distance (randomly)
            playerDistanceSq = vec3DistanceSquared(&self->globalPosition, &objGetPlayer()->globalPosition);

            //Random chance of playing sound if the player is inside inner radius
            if (!mathRnd(0, 300) && (playerDistanceSq < objData->innerDistanceSq)) {
                dll_amSfx->Play(self, setup->soundID, MAX_VOLUME, NULL, 0, 0, 0);
            }
        } else if (gamebitValue != objData->prevGamebitValue) {
            //Play by gamebit

            if ((gamebitValue == TRUE) && (flags & SfxPlayer_FLAG_2_Play_if_Gamebit_Set)) {
                //Play sound if gamebit is set
                dll_amSfx->Play(self, setup->soundID, MAX_VOLUME, NULL, 0, 0, 0);
            } else if ((gamebitValue == FALSE) && (flags & SfxPlayer_FLAG_4_Play_if_Gamebit_Unset)) {
                //Play sound if gamebit isn't set
                dll_amSfx->Play(self, setup->soundID, MAX_VOLUME, NULL, 0, 0, 0);
            }
        }
    }

    objData->prevGamebitValue = gamebitValue;
}

// offset: 0x458 | func: 2 | export: 2
void sfxPlayer_obj_Update(Object* self) { }

// offset: 0x464 | func: 3 | export: 3
void sfxPlayer_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Object** obj = &self; //fake?
    
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4C0 | func: 4 | export: 4
void sfxPlayer_obj_Free(Object* self, s32 onlySelf) {
    SfxPlayer_Data* objdata = self->data;
    if (objdata->soundHandle) {
        dll_amSfx->Stop(objdata->soundHandle);
    }
}

// offset: 0x51C | func: 5 | export: 5
u32 sfxPlayer_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x52C | func: 6 | export: 6
u32 sfxPlayer_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(SfxPlayer_Data);
}
