#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/541_DIMexplosion.h"
#include "game/objects/interaction_arrow.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    u8 yaw;
} Barrel_Setup;

typedef struct {
    Pickup pickup;
    u8 state;
    f32 timer;
} Barrel_Data;

typedef enum {
    Barrel_STATE_0_Main,
    Barrel_STATE_1_Destroyed,
    Barrel_STATE_2_Waiting_to_Reappear
} Barrel_States;

// offset: 0x0 | ctor
void Barrel_ctor(void *dll) { }

// offset: 0xC | dtor
void Barrel_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Barrel_setup(Object* self, Barrel_Setup* objSetup, s32 reset) {
    self->srt.yaw = objSetup->yaw << 8;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    gDLL_54_pickup->vtbl->setup(self, (Pickup*)self->data, 33);
    gDLL_54_pickup->vtbl->set_dont_save((Pickup*)self->data, TRUE);
}

// offset: 0xBC | func: 1 | export: 1
void Barrel_control(Object* self) {
    Barrel_Data* objData;
    ObjSetup* objSetup;
    DIMExplosion_Setup* explosionSetup;
    s32 hitDamage;
    Pickup* pickup;

    objData = self->data;
    objSetup = self->setup;
    pickup = &objData->pickup;
 
    switch (objData->state) {
    case Barrel_STATE_0_Main:
        gDLL_54_pickup->vtbl->control(self, pickup);
        
        //Explode when damaged
        if (func_80025F40(self, NULL, NULL, &hitDamage) != 0) {
            gDLL_54_pickup->vtbl->drop(self, pickup);
            dll_amSfx->Play(self, SOUND_372_Crate_Struck, MAX_VOLUME, NULL, NULL, 0, NULL);
            func_80026940(self, 40);
            func_80026128(self, Damage_Type_Explosion, 4, 0);
            
            //Create an explosion
            explosionSetup = objAllocSetup(sizeof(DIMExplosion_Setup), OBJ_DIMExplosion);
            explosionSetup->base.x = self->srt.transl.x;
            explosionSetup->base.y = self->srt.transl.y;
            explosionSetup->base.z = self->srt.transl.z;
            objSetupObject((ObjSetup*)explosionSetup, 5, self->mapID, -1, self->parent);
            
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_355, NULL, 0, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_352, NULL, 0, -1, NULL);
            
            objData->state = Barrel_STATE_1_Destroyed;
            return;
        }
        return;
    case Barrel_STATE_1_Destroyed:
        //Warp back to setup position, disable objHits, and await recycling
        func_80026160(self);
        func_800267A4(self);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        objData->state = Barrel_STATE_2_Waiting_to_Reappear;
        objData->timer = 0.0f;
        self->srt.transl.x = objSetup->x;
        self->srt.transl.y = objSetup->y;
        self->srt.transl.z = objSetup->z;
        return;
    case Barrel_STATE_2_Waiting_to_Reappear:
        objData->timer += gUpdateRateF;
        
        //Reappear when timer has reached threshold and barrel is off-screen
        if ((objData->timer > 300.0f) && (trackIsSphereInFrustum(&self->srt.transl, self->visRadius) == FALSE)) {
            func_8002674C(self);
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            objData->state = Barrel_STATE_0_Main;
        }
        break;
    }
}

// offset: 0x3A8 | func: 2 | export: 2
void Barrel_update(Object *self) { }

// offset: 0x3B4 | func: 3 | export: 3
void Barrel_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Barrel_Data* objData = self->data;
    
    if ((objData->state == Barrel_STATE_0_Main) && (gDLL_54_pickup->vtbl->should_print(self, visibility))) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x450 | func: 4 | export: 4
void Barrel_free(Object* self, s32 onlySelf) {
    gDLL_54_pickup->vtbl->free(self);
}

// offset: 0x498 | func: 5 | export: 5
u32 Barrel_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x4A8 | func: 6 | export: 6
u32 Barrel_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(Barrel_Data);
}
