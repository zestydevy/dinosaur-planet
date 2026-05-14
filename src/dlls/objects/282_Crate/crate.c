#include "common.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "sys/objtype.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "dlls/objects/common/collectable.h"

#define REAPPEAR_TIME 1200.0f //20 seconds
#define FLASH_MAX_OPACITY 120.0f
#define FLASH_SPEED 12.0f
#define CRATE_HIT_POINTS 2

static DLL_IModgfx* dModGfxDLLDestroyed = NULL; //Creates particles when damaged
static DLL_IModgfx* dModGfxDLLDamaged = NULL;   //Creates debris when destroyed

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 useNearbyCollectable;    //When set: a nearby collectable is repositioned when the crate is destroyed (no Gplay timer).
                                //When not set: crate drops an Energy Egg, and it reappears after a Gplay timer expires.
    s16 unused1A;
    s16 unused1C;
    s16 gamebitDestroyed;       //Gamebit to set when destroyed. Doesn't affect crate reappearing!
} Crate_Setup;

typedef struct {
    f32 flash;                  //Opacity for red flash effect after being damaged
    f32 flashSpeed;             //Rate of change of flash opacity
    s8 isRespawned;             //Has reappeared via a Gplay timer
    s8 isDestroyed;             //Has been destroyed
    s8 health;                  //Durability of the crate
    s8 useNearbyCollectable;    //Reposition nearby collectable when destroyed 
                                //   (When this is set, the crate won't create an 
                                //    Energy Egg or respawn via a Gplay timer)
} Crate_Data;

// offset: 0x0 | ctor
void crate_ctor(void *dll) { }

// offset: 0xC | dtor
void crate_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void crate_setup(Object* self, Crate_Setup* objSetup, s32 arg2) {
    Crate_Data* objData;

    objData = self->data;
    
    obj_add_object_type(self, OBJTYPE_63);
    self->srt.yaw = objSetup->yaw << 8;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    
    dModGfxDLLDestroyed = dll_load(DLL_ID_107, 1, 0);
    dModGfxDLLDamaged = dll_load(DLL_ID_106, 1, 0);
    
    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        self->shadow->maxDistScale = self->shadow->scale * 0.5f;
    }
    
    objData->health = CRATE_HIT_POINTS;
    objData->useNearbyCollectable = objSetup->useNearbyCollectable;

    //Start off destroyed if the Gplay destruct timer is still active for this crate
    //(Provided the crate is the kind that stores Energy Egg)
    if ((objData->useNearbyCollectable == FALSE) && 
        (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID) == FALSE)
    ) {
        self->objhitInfo->unk58 &= ~1;
        objData->isDestroyed = TRUE;
        objData->isRespawned = FALSE;
    }
}

// offset: 0x15C | func: 1 | export: 1
void crate_control(Object* self) {
    Crate_Data* objData = self->data;
    s32 pad;
    Object* obj;
    Collectable_Setup* foodSetup;
    Crate_Setup* objSetup;
    f32 distance;
    s32 hitDamage;
    s32 dModGfxParams[4] = {0x8, 0xff, 0xff, 0x78};
    SRT srt;
    
    //Return early if destroyed
    if (objData->isDestroyed) {
        objSetup = (Crate_Setup*)self->setup;

        //Crate resets when its Gplay timer runs out (if the crate was storing an Energy Egg)
        if ((objData->useNearbyCollectable == FALSE) && 
            gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID)
        ) {
            objData->isDestroyed = FALSE;
            objData->isRespawned = TRUE;
            objData->health = CRATE_HIT_POINTS;
            self->objhitInfo->unk58 |= 1;
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        } else {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
        }
        return;
    }
    
    //Take damage from attacks
    if (func_8002601C(self, NULL, NULL, &hitDamage, (f32*)&srt.transl.x, (f32*)&srt.transl.y, (f32*)&srt.transl.z) != 0) {
        objData->health -= hitDamage;
        if (objData->health > 0) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_372_Crate_Struck, MAX_VOLUME, NULL, NULL, 0, NULL);
            
            //Change model index (increase index to look more and more damaged)
            obj_set_model(self, CRATE_HIT_POINTS - objData->health);

            //Flash after being hit
            objData->flash = 1.0f;
            objData->flashSpeed = FLASH_SPEED;

            //Create particles
            srt.transl.x += gWorldX;
            srt.transl.z += gWorldZ;
            srt.scale = 1.0f;
            srt.yaw = 0;
            srt.pitch = 0;
            srt.roll = 0;
            dModGfxDLLDamaged->vtbl->func0(0, 1, &srt, 0x401, -1, &dModGfxParams);
        }
    }
    
    //Handle being destroyed
    if (objData->health <= 0) {
        objSetup = (Crate_Setup*)self->setup;

        //Set a Gplay timer to reappear 20 seconds later (if the crate was storing an Energy Egg)
        if (objData->useNearbyCollectable == FALSE) {
            gDLL_29_Gplay->vtbl->add_time(objSetup->base.uID, REAPPEAR_TIME);
        }
        objData->isDestroyed = TRUE;
        objData->isRespawned = FALSE;

        gDLL_6_AMSFX->vtbl->play(self, SOUND_371_Crate_Smash, MAX_VOLUME, NULL, NULL, 0, NULL);
        
        self->objhitInfo->unk58 &= ~1;

        //Set a gamebit (NOTE: not checked during setup, so crate still comes back)
        if (objSetup->gamebitDestroyed != NO_GAMEBIT) {
            main_set_bits(objSetup->gamebitDestroyed, 1);
        }
        
        //Create an Energy Egg
        if (objData->useNearbyCollectable == FALSE) {
            foodSetup = (Collectable_Setup*)obj_alloc_setup(
                sizeof(Collectable_Setup), 
                OBJ_meatPickup
            );
            foodSetup->gamebitCollected = NO_GAMEBIT;
            foodSetup->base.x = self->srt.transl.x;
            foodSetup->base.y = self->srt.transl.y + 10.0f;
            foodSetup->base.z = self->srt.transl.z;
            foodSetup->objHitsValue = 3;
            foodSetup->gamebitCount = NO_GAMEBIT;
            foodSetup->gamebitSecondary = NO_GAMEBIT;
            obj_create(
                (ObjSetup*)foodSetup, 
                5, 
                self->mapID, 
                -1, 
                self->parent
            );

        //Or reposition a nearby Collectable
        } else {
            distance = 50.0f;
            obj = obj_get_nearest_type_to(OBJTYPE_5, self, &distance);
            if (obj != NULL) {
                obj->srt.transl.x = obj->globalPosition.x = self->srt.transl.x;
                obj->srt.transl.y = obj->globalPosition.y = self->srt.transl.y + 10.0f;
                obj->srt.transl.z = obj->globalPosition.z = self->srt.transl.z;
                obj->srt.yaw = self->srt.yaw;
            }
        }

        //Create debris
        dModGfxDLLDestroyed->vtbl->func0(self, 1, 0, 2, -1, 0);
    }
    
    //Handle flash opacity (after being damaged, fade in/out red glow)
    if (objData->flash > 0.0f) {
        objData->flash += gUpdateRateF * objData->flashSpeed;
        if (objData->flash < 0.0f) {
            objData->flash = 0.0f;
        } else if (objData->flash > FLASH_MAX_OPACITY) {
            objData->flash = FLASH_MAX_OPACITY - (objData->flash - FLASH_MAX_OPACITY); //reverse progress past max value
            objData->flashSpeed = -objData->flashSpeed; //reverse to fade out
        }
    }
}

// offset: 0x5D8 | func: 2 | export: 2
void crate_update(Object *self) { }

// offset: 0x5E4 | func: 3 | export: 3
void crate_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    Crate_Data* objData = self->data;
    
    if ((objData->isDestroyed == 0) && visibility) {
        //Flash red after being struck
        if (objData->flash) {
            func_80036FBC(0xC8, 0, 0, objData->flash);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x728 | func: 4 | export: 4
void crate_free(Object* self, s32 arg1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    dll_unload(dModGfxDLLDestroyed);
    dll_unload(dModGfxDLLDamaged);
    obj_free_object_type(self, OBJTYPE_63);
}

// offset: 0x7B8 | func: 5 | export: 5
u32 crate_get_model_flags(Object *self){
    return MODFLAGS_SHADOW;
}

// offset: 0x7C8 | func: 6 | export: 6
u32 crate_get_data_size(Object *self, u32 a1){
    return sizeof(Crate_Data);
}
