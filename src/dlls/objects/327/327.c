// Official DLL name unknown, but used for many basic stationary animated props/characters

#include "game/gamebits.h"
#include "game/objects/setups/30.h"
#include "sys/main.h"
#include "sys/objanim.h"
#include "sys/objprint.h"

// offset: 0x0 | ctor
void DLL327_ctor(void *dll) { }

// offset: 0xC | dtor
void DLL327_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DLL327_obj_Setup(Object* self, Ctrl30_Setup* objSetup, s32 reset) {
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
void DLL327_obj_Control(Object* self) {
    Ctrl30_Setup *objSetup;
    f32 animSpeed;
    f32 animSpeedTemp;
    
    objSetup = (Ctrl30_Setup*)self->setup;
    
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
void DLL327_obj_Update(Object *self) { }

// offset: 0x23C | func: 3 | export: 3
void DLL327_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x290 | func: 4 | export: 4
void DLL327_obj_Free(Object *self, s32 onlySelf) { }

// offset: 0x2A0 | func: 5 | export: 5
u32 DLL327_obj_GetModelFlags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2B0 | func: 6 | export: 6
u32 DLL327_obj_GetDataSize(Object *self, u32 offsetAddress){
    return 0;
}
