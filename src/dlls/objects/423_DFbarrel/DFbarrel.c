#include "common.h"
#include "dlls/objects/357_ExplodeAnimator.h"
#include "dlls/objects/358_debris.h"
#include "dlls/objects/418_DFriverflow.h"
#include "dlls/objects/423_DFbarrel.h"
#include "dlls/objects/541_DIMexplosion.h"
#include "game/objects/interaction_arrow.h"
#include "sys/objhits.h"
#include "sys/objtype.h"

typedef struct {
    Pickup pickup;
    u8 damage;                  //Damage accumulated by the barrel (explodes if it's damaged at all, though!)
    u8 framesSinceDetonation;   //Seems intended to count up to deleting the barrel after it explodes, but it's deleted immediately anyway
    s32 _unusedC;
    f32 accelerationX;          //Acceleration from DFriverflow currents
    f32 accelerationZ;          //Acceleration from DFriverflow currents
    f32 velocityX;              //Copy of barrel's velocity, for flow/bouyancy calcs
    f32 velocityY;              //Copy of barrel's velocity, for flow/bouyancy calcs
    f32 velocityZ;              //Copy of barrel's velocity, for flow/bouyancy calcs
} DFBarrel_Data;

static void DFbarrel_handle_movement(Object* self);
static void DFbarrel_handle_damage(Object* self);

// offset: 0x0 | ctor
void DFbarrel_ctor(void *dll) { }

// offset: 0xC | dtor
void DFbarrel_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFbarrel_setup(Object* self, DFBarrel_Setup* objSetup, s32 reset) {
    obj_add_object_type(self, OBJTYPE_Barrel);
    self->srt.yaw = objSetup->yaw << 8;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    gDLL_54_pickup->vtbl->setup(self, (Pickup*)self->data, 33);
}

// offset: 0xA4 | func: 1 | export: 1
void DFbarrel_control(Object* self) {
    DFBarrel_Data* objData = self->data;
    
    //Do nothing if not on a map?
    if (map_world_coords_to_block_index(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z) == -1) {
        return;
    }
    
    switch (objData->framesSinceDetonation) {
    case 0:
        if (gDLL_54_pickup->vtbl->control(self) == 0) {
            DFbarrel_handle_movement(self);
            DFbarrel_handle_damage(self);
        }
        break;
    case 1:
        func_800267A4(self);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        objData->framesSinceDetonation = 20;
        return;
    default:
        objData->framesSinceDetonation++;
        /* fallthrough */
    case 20:
        obj_destroy_object(self);
        break;
    }
}

// offset: 0x1D0 | func: 2 | export: 2
void DFbarrel_update(Object *self) { }

// offset: 0x1DC | func: 3 | export: 3
void DFbarrel_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DFBarrel_Data* objData = self->data;
    
    if ((objData->framesSinceDetonation == 0) && gDLL_54_pickup->vtbl->should_print(self, visibility)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x278 | func: 4 | export: 4
void DFbarrel_free(Object* self, s32 onlySelf) {
    obj_free_object_type(self, OBJTYPE_Barrel);
    gDLL_54_pickup->vtbl->free(self);
}

// offset: 0x2E0 | func: 5 | export: 5
u32 DFbarrel_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2F0 | func: 6 | export: 6
u32 DFbarrel_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DFBarrel_Data);
}

// offset: 0x304 | func: 7
void DFbarrel_handle_movement(Object* self) {
    DFBarrel_Data* objData;
    Object* riverFlow;
    Object** objects;
    s32 count;
    s32 i;
    f32 delta;
    f32 dy;
    f32 dz;
    f32 range;
    f32 diffMagnitude;
    Func_80057F1C_Struct** collisionInfo;
    Vec3f position;
    f32 minDiffMagnitude;
    f32 yDiff;

    objData = self->data;

    //Get swept away by DFriverflow objects
    {
        objData->accelerationX = objData->accelerationZ = 0.0f;

        for (objects = obj_get_all_of_type(OBJTYPE_Riverflow, &count), i = 0; i < count; i++) {
            riverFlow = objects[i];
            dy = riverFlow->srt.transl.y - self->srt.transl.y;
            if ((dy <= 200.0f) && (dy >= -200.0f)) {
                delta = riverFlow->srt.transl.x - self->srt.transl.x;
                dz = riverFlow->srt.transl.z - self->srt.transl.z;
                delta = sqrtf(SQ(delta) + SQ(dz));
                
                range = ((DFriverflow_Setup*)riverFlow->setup)->range * 1.5f;
                if (delta < range) {
                    delta = (range - delta) / range;
                    delta *= riverFlow->srt.scale * 10.0f;
                    
                    objData->accelerationX += fsin16_precise(riverFlow->srt.yaw) * delta;
                    objData->accelerationZ += fcos16_precise(riverFlow->srt.yaw) * delta;
                }
            }
        }
        
        if (count != 0) {
            objData->accelerationX /= count;
            objData->accelerationZ /= count;
            objData->velocityX -= (objData->accelerationX * 0.05f);
            objData->velocityZ -= (objData->accelerationZ * 0.05f);
            objData->velocityX *= 0.99f;
            objData->velocityZ *= 0.99f;
            
            delta = sqrtf(SQ(objData->velocityX) + SQ(objData->velocityZ));
            if (delta > 0.85f) {
                objData->velocityX *= 0.85f / delta;
                objData->velocityZ *= 0.85f / delta;
            }
            self->velocity.x = objData->velocityX * gUpdateRateF;
            self->velocity.z = objData->velocityZ * gUpdateRateF;
        }
    }

    //Float on water
    {
        objData->velocityY = self->velocity.y;
        
        //Find the nearest water plane
        count = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &collisionInfo, 0, 0);
        if (count) {
            delta = 10000.0f;
            
            for (i = 0; i < count; i++){
                if (collisionInfo[i]->unk14 == 14) { //water only?            
                    yDiff = self->srt.transl.y - collisionInfo[i]->unk0[0];
                    if (yDiff >= 0) {
                        diffMagnitude = yDiff;
                    } else {
                        diffMagnitude = -yDiff;
                    }
                    
                    if (delta >= 0) {
                        minDiffMagnitude = delta;
                    } else {
                        minDiffMagnitude = -delta;
                    }
                    
                    if (diffMagnitude < minDiffMagnitude) {
                        delta = yDiff;
                    }
                }
            }

            //Correlate velocityY with barrel's depth underwater
            if ((delta != 10000.0f) && (delta < 0)) {
                objData->velocityY += (-delta * 0.1f);
                objData->velocityY *= 0.8f;
            }
        }

        self->velocity.y = objData->velocityY;
    }

    position.x = self->srt.transl.x;
    position.y = self->srt.transl.y;
    position.z = self->srt.transl.z;

    obj_move(self, self->velocity.x, self->velocity.y, self->velocity.z);

    func_80059C40(&position, &self->srt.transl, 10.0f, 0, NULL, self, 8, -1, 0xFF, 0);
}

// offset: 0x960 | func: 8
void DFbarrel_handle_damage(Object* self) {
    DFBarrel_Data* objData;
    DIMExplosion_Setup* explosion;
    s32 hitDamage;
    Object* obj;
    f32 distance;
    ExplodeAnimator_Setup* explodeAnimSetup;
    Debris_Setup* debrisSetup;

    objData = self->data;

    //Check for damage, return early if nothing damaged the barrel
    if (func_80025F40(self, NULL, NULL, &hitDamage) == 0) {
        return;
    }

    //When damaged, play an impact sound and increase damage counter
    gDLL_6_AMSFX->vtbl->play(self, SOUND_372_Crate_Struck, MAX_VOLUME, NULL, NULL, 0, NULL);
    objData->damage += hitDamage;

    //Return early if the barrel hasn't reached the damage threshold
    if (objData->damage <= 0) { //passes threshold as soon as it takes any damage
        return;
    }

    //Explode!
    {
        func_80026940(self, 40);
        func_80026128(self, Damage_Type_Explosion, 4, 0);
        
        explosion = obj_alloc_setup(sizeof(DIMExplosion_Setup), OBJ_DIMExplosion);
        explosion->base.x = self->srt.transl.x;
        explosion->base.y = self->srt.transl.y;
        explosion->base.z = self->srt.transl.z;
        obj_create((ObjSetup*)explosion, 5, self->mapID, -1, self->parent);
        
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_355, NULL, 0, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_352, NULL, 0, -1, NULL);
    }

    //Check for any nearby debris/explodeAnimator objects, and set their gamebits
    {
        distance = 110.0f;

        //Debris
        if ((obj = obj_get_nearest_type_to(OBJTYPE_ExplodeObj, self, &distance))) {
            debrisSetup = (Debris_Setup*)obj->setup;
            if (debrisSetup->unk40 != NO_GAMEBIT) {
                main_set_bits(debrisSetup->unk40, 1);
            }
        }

        //ExplodeAnimator
        if ((obj = obj_get_nearest_type_to(OBJTYPE_ExplodeAnimator, self, &distance))) {
            explodeAnimSetup = (ExplodeAnimator_Setup*)obj->setup;
            if (explodeAnimSetup->gamebitExplodeTrigger != NO_GAMEBIT) {
                main_set_bits(explodeAnimSetup->gamebitExplodeTrigger, 1);
            }
        }
    }
    
    objData->framesSinceDetonation = 1;
}
