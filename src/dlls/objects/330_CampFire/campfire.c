#include "common.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/objhits.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/338_LFXEmitter.h"

typedef struct {
    ObjSetup base;
    s16 gamebitID;          //Checked during setup, but does nothing (maybe for tracking lighting fire/putting it out?)
    u8 scale;               //Scale multiplier (100 = 1.0)
    u8 unk1B;               //Unknown - stored in setup but otherwise unused
    u8 alwaysIlluminates;   //When nonzero, fire emits light even when the player isn't nearby
    u8 modGfxArg;           //Boolean affecting the fire mesh modGfx calls, unknown purpose
    u16 lfxUnk1E;           //Param for the light emitter
} CampFire_Setup;

typedef struct {
    Object* light;          //LFXEmitter
    s16 gamebitID;          //Checked during setup, but does nothing (maybe for tracking lighting fire/putting it out?)
    s16 unk6;               //Unused
    u8 unk8;                //Unknown - stores `objSetup->unk1B`, but otherwise unused
    u8 flags;               //Bits set based on gamebits, no effect/unfinished
} CampFire_Data;

typedef enum {
    CampFire_FLAG_0_None = 0,
    CampFire_FLAG_1 = 1, //Does nothing, but applied in setup when BIT_8C is set
    CampFire_FLAG_2 = 2, //Unused
    CampFire_FLAG_4 = 4, //Does nothing, but applied in setup when instance-specific gamebit is set
} CampFire_Flags;

// offset: 0x0 | ctor
void campfire_ctor(void *dll) { }

// offset: 0xC | dtor
void campfire_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void campfire_setup(Object* self, CampFire_Setup* objSetup, s32 arg2) {
    f32 scaleMultiplier;
    CampFire_Data* objData;

    objData = self->data;
    
    //Set scale
    if (objSetup->scale != 0) {
        self->srt.scale = objSetup->scale * 0.01f;
    }
    
    //Track that the fire mesh hasn't been created yet
    self->unkE0 = FALSE;
    
    //Set a flag based on a common gamebit
    //Does nothing, but maybe intended to put out fire under certain global conditions like rain?
    if (main_get_bits(BIT_8C)) {
        objData->flags |= CampFire_FLAG_1;
    }
    
    //Set another flag based on an instance-specific gamebit
    //Does nothing, but maybe intended to track whether fire is lit/extinguished?
    objData->gamebitID = objSetup->gamebitID;
    if (objData->gamebitID != NO_GAMEBIT) {
        if (main_get_bits(objData->gamebitID)) {
            objData->flags |= CampFire_FLAG_4;
        }
    }
    
    objData->unk8 = objSetup->unk1B;
    objData->light = NULL;

    //Scale the fire's objHits wrt. the object's scale
    scaleMultiplier = self->srt.scale / self->def->scale;
    if (self->objhitInfo != NULL) {
        self->objhitInfo->unk52 *= scaleMultiplier;
        self->objhitInfo->unk56 *= scaleMultiplier;
        self->objhitInfo->unk54 *= scaleMultiplier;
    }
    
    //Start fire sound loops
    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_50a_Fire_Burning_Low_Loop,  MAX_VOLUME, NULL, NULL, 0, NULL);
    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_50b_Fire_Burning_High_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
}

// offset: 0x210 | func: 1 | export: 1
void campfire_control(Object* self) {
    CampFire_Data* objData;
    LFXEmitter_Setup* lfxSetup;
    Object* player;
    ObjSetup* campSetup;
    f32 distance;
    CampFire_Setup* objSetup;
    u8 modGfxArg;
    s8 createLight;
    DLL_IModgfx* modGfxDLL;

    objData = self->data;
    player = get_player();
    objSetup = (CampFire_Setup*)self->setup;

    //Set up the fire so it hurts the player on contact
    func_80026128(self, Damage_Type_Sword_Staff_Strike2, 1, 0);
    
    //Create fire mesh
    if (self->unkE0 == FALSE) {
        gDLL_14_Modgfx->vtbl->func10(self);
        
        modGfxDLL = dll_load_deferred(DLL_ID_116, 1);
        
        if (objSetup->modGfxArg) {
            modGfxArg = 1;
        } else {
            modGfxArg = 0;
        }
        
        modGfxDLL->vtbl->func0(self, modGfxArg, 0, 0x10004, -1, 0);
        dll_unload(modGfxDLL);
        
        modGfxDLL = dll_load_deferred(DLL_ID_115, 1);
        modGfxDLL->vtbl->func0(self, modGfxArg, 0, 0x10004, -1, 0);
        dll_unload(modGfxDLL);
        
        modGfxDLL = dll_load_deferred(DLL_ID_114, 1);
        modGfxDLL->vtbl->func0(self, modGfxArg, 0, 0x10004, -1, 0);
        dll_unload(modGfxDLL);
        
        self->unkE0 = TRUE;
    }
    
    //Get player distance
    distance = vec3_distance(&player->globalPosition, &self->globalPosition);

    //Handle creating/unloading the fire's LFXEmitter when needed
    if (objData->light != NULL) {
        //Unload the light if the player's too far away
        if ((objSetup->alwaysIlluminates == FALSE) && (distance > 110.0f)) {
            obj_destroy_object(objData->light);
            objData->light = NULL;
        }
    } else {
        //When there's no LFXEmitter, check if one should be created
        createLight = FALSE;
        if (objSetup->alwaysIlluminates == FALSE) {
            if (distance < 100.0f) {
                createLight = TRUE;
            }
        } else {
            createLight = TRUE;
        }

        //Create the LFXEmitter when needed
        if (createLight) {
            lfxSetup = (LFXEmitter_Setup*)obj_alloc_setup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
            campSetup = self->setup;
            lfxSetup->base.loadFlags = campSetup->loadFlags;
            lfxSetup->base.byte6 = campSetup->byte6;
            lfxSetup->base.x = self->srt.transl.x;
            lfxSetup->base.y = self->srt.transl.y + rand_next(0, 30);
            lfxSetup->base.z = self->srt.transl.z;
            lfxSetup->unk1E = objSetup->lfxUnk1E;
            if (lfxSetup->unk1E == 0) {
                lfxSetup->unk1E = 0x59;
            }
            lfxSetup->unk22 = -1;
            lfxSetup->unk18 = 2400;
            lfxSetup->unk1A = 2400;
            lfxSetup->unk1C = 200;
            objData->light = obj_create((ObjSetup*)lfxSetup, 5, self->mapID, -1, self->parent);
        }
    }
}

// offset: 0x580 | func: 2 | export: 2
void campfire_update(Object *self) { }

// offset: 0x58C | func: 3 | export: 3
void campfire_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0x5E0 | func: 4 | export: 4
void campfire_free(Object* self, s32 onlySelf) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x650 | func: 5 | export: 5
u32 campfire_get_model_flags(Object *self){
    return MODFLAGS_1;
}

// offset: 0x660 | func: 6 | export: 6
u32 campfire_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(CampFire_Data);
}
