#include "common.h"
#include "macros.h"

/*0x0*/ static s16 dSoundIDs[] = {
    SOUND_157_Bird_Call_Warbling, 
    SOUND_158_Bird_Call_Whistle_Descending, 
    SOUND_15A_Bird_Call_Whistle_Ascending, 
    SOUND_15B_Bird_Call_Squawk
};

typedef struct {
    ObjSetup base;
    s16 gamebitDisable;      //Sounds stop playing when this gamebit is set
    u8 soundListIdx;         //The random sounds list will only be used when this is set to 0. Possibly intended to pick between different (unimplemented) arrays of soundIDs?
    u8 intervalInSeconds;    //The delay between sounds (actual delay will be randomised: somewhere between 1x and 2x this value)
} NWsfx_Setup;

typedef struct {
    s16 gamebitDisable;      //Sounds stop playing when this gamebit is set
    s16* sounds;             //An array of soundIDs, to pick from at random
    s16 interval;            //The delay between sounds, in frames (actual delay will be randomised: somewhere between 1x and 2x this value)
    s16 intervalTimer;       //Countdown until the next sound plays
    u8 totalSounds;          //The number of soundIDs in the `objData->sounds` array
} NWsfx_Data;

// offset: 0x0 | ctor
void NWsfx_ctor(void* dll) { }

// offset: 0xC | dtor
void NWsfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void NWsfx_setup(Object* self, NWsfx_Setup* objSetup, s32 reset) {
    NWsfx_Data* objData;
    s16 interval;

    objData = self->data;
    
    objData->gamebitDisable = objSetup->gamebitDisable;

    //Set up interval between sounds
    interval = objSetup->intervalInSeconds * 60;
    objData->intervalTimer = interval;
    objData->interval = interval;
    
    //Set up the list of soundIDs
    if (objSetup->soundListIdx == 0) {
        objData->sounds = dSoundIDs;
        objData->totalSounds = ARRAYCOUNT(dSoundIDs);
    } else {
        objData->sounds = NULL;
        objData->totalSounds = 0;
    }
}

// offset: 0x84 | func: 1 | export: 1
void NWsfx_control(Object* self) {
    NWsfx_Data* objData = self->data;
    
    if (objData->sounds == NULL) {
        return;
    }
    
    //Wait out the interval between sounds
    objData->intervalTimer -= gUpdateRate;
    if (objData->intervalTimer >= 0) {
        return;
    }

    //Set up the next interval between sounds (randomised: somewhere between 1x and 2x the objSetup interval value)
    objData->intervalTimer = objData->interval; 
    objData->intervalTimer += mathRnd(0, objData->interval);

    //Don't play sounds if a specified gamebit is set
    if ((objData->gamebitDisable != NO_GAMEBIT) && mainGetBits(objData->gamebitDisable)) {
        return;
    }
    
    //Play a sound
    if (objData->totalSounds == 0) {
        //Play the first sound (NOTE: unreachable, since `objData->sounds` will be NULL when `objData->totalSounds` is 0?)
        gDLL_6_AMSFX->vtbl->play(self, objData->sounds[0], MAX_VOLUME, NULL, NULL, 0, NULL);
    } else {
        //Pick a random sound to play
        gDLL_6_AMSFX->vtbl->play(self, objData->sounds[mathRnd(0, objData->totalSounds - 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}

// offset: 0x1E8 | func: 2 | export: 2
void NWsfx_update(Object* self) { }

// offset: 0x1F4 | func: 3 | export: 3
void NWsfx_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x248 | func: 4 | export: 4
void NWsfx_free(Object* self, s32 onlySelf) { }

// offset: 0x258 | func: 5 | export: 5
u32 NWsfx_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x268 | func: 6 | export: 6
u32 NWsfx_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(NWsfx_Data);
}
