#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/rand.h"
#include "sys/objects.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 flagPlay;
/*1A*/ s16 soundID; // Should use a SoundID enum value from dll 6
/*1C*/ u8 mode;
/*1D*/ u8 radius;
} SfxPlayer_Setup;

typedef struct {
/*00*/ f32 distanceSqInner;
/*04*/ f32 distanceSqOuter;
/*08*/ u8 hasPlayed;
/*09*/ u8 unk9;
/*0A*/ u16 unkA;
/*0C*/ u32 sound;
} SfxPlayer_Data;

// offset: 0x0 | ctor
void sfxplayer_ctor(void* dll){
}

// offset: 0xC | dtor
void sfxplayer_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void sfxplayer_setup(Object* self, SfxPlayer_Setup* setup, s32 arg2) {
    SfxPlayer_Data* objdata;
    SfxPlayer_Setup* setup2;

    objdata = self->data;
    setup2 = (SfxPlayer_Setup*)self->setup;
    objdata->hasPlayed = main_get_bits(setup->flagPlay);

    if ((setup2->mode & 1) && (!objdata->hasPlayed) && (setup2->mode & 4)) {
        objdata->hasPlayed = 1;
    }

    self->unk0xb0 |= 0x2000;
    objdata->distanceSqInner = setup->radius * 2;
    objdata->distanceSqOuter = objdata->distanceSqInner + 10.0f;
    objdata->distanceSqInner = objdata->distanceSqInner * objdata->distanceSqInner;
    objdata->distanceSqOuter = objdata->distanceSqOuter * objdata->distanceSqOuter;
}

// offset: 0xFC | func: 1 | export: 1
void sfxplayer_control(Object* self) {
    SfxPlayer_Data *objdata;
    SfxPlayer_Setup *setup;
    u8 flagValue;
    f32 playerDistanceSquared;
    u8 mode;

    setup = (SfxPlayer_Setup *) self->setup;
    objdata = self->data;
    flagValue = 0;
    if (setup->flagPlay){
        flagValue = main_get_bits(setup->flagPlay);
    }

    mode = setup->mode;
    if (mode & 1){
        if (setup->flagPlay != -1){
            if ((flagValue != objdata->hasPlayed) && (mode & 6)){
                if ((flagValue != ((mode & 4) == 4)) != 0){
                    objdata->sound = gDLL_6_AMSFX->vtbl->play_sound(self, setup->soundID, MAX_VOLUME, NULL, 0, 0, 0);
                } else if (objdata->sound){
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->sound);
                    objdata->sound = NULL;
                }
            }
        } else if (mode & 6){
            //Start sound when inside inner radius, stop sound when leaving outer radius
            playerDistanceSquared = vec3_distance_squared(&self->positionMirror, &get_player()->positionMirror);
            if (!objdata->sound && (playerDistanceSquared < objdata->distanceSqInner)){
                gDLL_6_AMSFX->vtbl->play_sound(self, setup->soundID, MAX_VOLUME, (u32*)&objdata->sound, 0, 0, 0);
            } else if ((objdata->distanceSqOuter < playerDistanceSquared) && objdata->sound){
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->sound);
                objdata->sound = NULL;
            }
        }
    } else {
        if (mode & 8) {
            //Random chance of playing sound if player is inside inner radius
            playerDistanceSquared = vec3_distance_squared(&self->positionMirror, &get_player()->positionMirror);
            if (!rand_next(0, 0x12C) && (playerDistanceSquared < objdata->distanceSqInner)){
                gDLL_6_AMSFX->vtbl->play_sound(self, setup->soundID, MAX_VOLUME, NULL, 0, 0, 0);
            }
        } else if (flagValue != objdata->hasPlayed){
            if ((flagValue == 1) && (mode & 2)){
                //Play sound if flag is set
                gDLL_6_AMSFX->vtbl->play_sound(self, setup->soundID, MAX_VOLUME, NULL, 0, 0, 0);
            } else if ((flagValue == 0) && (mode & 4)){
                //Play sound if flag isn't set
                gDLL_6_AMSFX->vtbl->play_sound(self, setup->soundID, MAX_VOLUME, NULL, 0, 0, 0);
            }
        }
    }

    objdata->hasPlayed = flagValue;
}

// offset: 0x458 | func: 2 | export: 2
void sfxplayer_update(Object* arg0) {
}

// offset: 0x464 | func: 3 | export: 3
void sfxplayer_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility){
    Object** obj;
    if (visibility) {
        obj = &self;
        draw_object(*obj, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4C0 | func: 4 | export: 4
void sfxplayer_free(Object* arg0, s32 arg1) {
    SfxPlayer_Data* objdata = arg0->data;
    if (objdata->sound) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->sound);
    }
}

// offset: 0x51C | func: 5 | export: 5
u32 sfxplayer_get_model_flags(Object* arg0) {
    return MODFLAGS_NONE;
}

// offset: 0x52C | func: 6 | export: 6
u32 sfxplayer_get_data_size(Object* arg0, s32 arg1) {
    return sizeof(SfxPlayer_Data);
}
