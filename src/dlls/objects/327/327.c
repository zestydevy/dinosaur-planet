// Official DLL name unknown, but used for many basic stationary animated props/characters

#include "common.h"
#include "sys/objanim.h"

typedef struct {
    ObjSetup base;
    u8 modelIndex;          //Which model to use
    u8 modAnimIndex;        //Which model animation to use
    u8 animProgress;        //Initial tValue for the animation (expressed from 0-256 instead of 0-1)
    u8 animSpeed;           //Animation playback speed (stored at 10000x actual value)
    u8 scaleMultiplier;     //Multiplies the size of the object and its shadow (64 = 1x, 128 = 2x, 32 = 0.5x, etc.)
    u8 yaw;
    s16 unused1E;
    s16 gamebitVisible;     //Toggles object visibility
} DLL327_Setup;

// offset: 0x0 | ctor
void dll_327_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_327_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_327_setup(Object* self, DLL327_Setup* objSetup, s32 arg2) {
    f32 factor;
    
    self->srt.flags |= OBJFLAG_UNK_2;
    
    //Apply a scale multiplier to the object and its shadow
    factor = objSetup->scaleMultiplier & 0xFF;
    if (factor < 10) {
        factor = 10;
    }
    factor /= 64.0f;
    
    self->srt.scale = self->def->scale * factor;
    self->srt.yaw = (objSetup->yaw << 8) << 2;
    
    if (self->shadow != NULL) {
        self->shadow->scale = self->def->shadowScale * factor;
    }
    
    //Choose model index
    self->modelInstIdx = objSetup->modelIndex;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    
    //Set up animation
    objAnimSet(self, objSetup->modAnimIndex, objSetup->animProgress / 256.0f, 0);
    
    //Set visibility based on a gamebit
    if (objSetup->gamebitVisible != NO_GAMEBIT) {
        if (mainGetBits(objSetup->gamebitVisible)) {
            self->opacity = OBJECT_OPACITY_MAX;
        } else {
            self->opacity = 0;
        }
    }
}

// offset: 0x170 | func: 1 | export: 1
void dll_327_control(Object* self) {
    DLL327_Setup *objSetup;
    f32 animSpeed;
    f32 animSpeedTemp;
    
    objSetup = (DLL327_Setup*)self->setup;
    
    //Advance animation
    animSpeedTemp = objSetup->animSpeed;
    animSpeedTemp /= 10000.0f;
    animSpeed = animSpeedTemp;
    objAnimAdvance(self, animSpeed, gUpdateRateF, 0);

    //Set visibility based on a gamebit
    if (objSetup->gamebitVisible != NO_GAMEBIT) {
        if (mainGetBits(objSetup->gamebitVisible)) {
            self->opacity = OBJECT_OPACITY_MAX;
        } else {
            self->opacity = 0;
        }
    }
}

// offset: 0x230 | func: 2 | export: 2
void dll_327_update(Object *self) { }

// offset: 0x23C | func: 3 | export: 3
void dll_327_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x290 | func: 4 | export: 4
void dll_327_free(Object *self, s32 a1) { }

// offset: 0x2A0 | func: 5 | export: 5
u32 dll_327_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2B0 | func: 6 | export: 6
u32 dll_327_get_data_size(Object *self, u32 a1){
    return 0;
}
