#include "common.h"
#include "sys/gfx/model.h"
#include "sys/math.h"
#include "sys/objhits.h"

/*0x0*/ static Vec3f dBounceVelocities[] = {
    VEC3F(-0.5, 0.6, -0.2), 
    VEC3F(-0.5, 1.2, -0.2), 
    VEC3F(-1, 2, -2)
};

typedef struct {
    ObjSetup base;
    s8 yaw;
} DIMBoulder_Setup;

typedef struct {
    f32 scale;
    s16 soundTimer;
    u8 state;
    u8 timesHit;
} DIMBoulder_Data;

typedef enum {
    DIMBoulder_STATE_0_Stopped,
    DIMBoulder_STATE_1_Moving,
    DIMBoulder_STATE_2_Finished
} DIMBoulder_States;

// offset: 0x0 | ctor
void DIMBoulder_ctor(void *dll) { }

// offset: 0xC | dtor
void DIMBoulder_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMBoulder_setup(Object* self, DIMBoulder_Setup* objSetup, s32 arg2) {
    DIMBoulder_Data* objData;

    objData = self->data;
    self->srt.yaw = objSetup->yaw << 8;
    if (self->objhitInfo != NULL) {
        self->objhitInfo->unk5B = 0;
    }
    
    objData->scale = 1.0f;
    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
    
    objData->timesHit = main_get_bits(BIT_4CE);
    if ((objData->timesHit) == 3) {
        objData->state = DIMBoulder_STATE_2_Finished;
    }
}

// offset: 0xD0 | func: 1 | export: 1
void DIMBoulder_control(Object* self) {
    f32 twoTimesDot;
    f32 speed;
    f32 speedX;
    f32 speedZ;
    f32 pad2;
    f32 temp;
    Vec3f pA;
    Vec3f pB;
    Vec3f unitV;
    Vec3f normalV;
    DIMBoulder_Data* objData;
    f32 pad4;
    s32 updateRate;
    s32 volume;
    Unk80027934 sp4C;
    AABBs32 bounds;

    objData = self->data;
    updateRate = gUpdateRate;
    temp = gUpdateRateF;

    if (objData->state == DIMBoulder_STATE_0_Stopped) {
        if ((func_80025F40(self, NULL, NULL, NULL) == Damage_Type_Explosion) || 
            ((joyGetButtons(1) & A_BUTTON)) //Move the snowball with A button presses on Controller 2!
        ) {
            objData->state = DIMBoulder_STATE_1_Moving;
            self->velocity.x = dBounceVelocities[objData->timesHit].x;
            self->velocity.y = dBounceVelocities[objData->timesHit].y;
            self->velocity.z = dBounceVelocities[objData->timesHit].z;
            objData->timesHit++;
            main_set_bits(BIT_4CE, objData->timesHit);
        }
        return;
    } 
    
    if (objData->state != DIMBoulder_STATE_2_Finished) {
        self->velocity.f[1] -= 0.09f * temp;
        obj_move(self, 
            self->velocity.f[0] * temp, 
            self->velocity.f[1] * temp, 
            self->velocity.f[2] * temp
        );
        
        pA.x = self->prevLocalPosition.x - 80.0f;
        pA.y = self->prevLocalPosition.y - 80.0f;
        pA.z = self->prevLocalPosition.z - 80.0f;
        pB.x = self->srt.transl.x + 80.0f;
        pB.y = self->srt.transl.y + 80.0f;
        pB.z = self->srt.transl.z + 80.0f;
        sp4C.unk50[0] = -1;
        sp4C.unk54[0] = 5;
        sp4C.unk40[0] = 55.0f;
        fit_aabb_around_cubes(&bounds, &pA, &pB, sp4C.unk40, 1);
        func_80053750(self, &bounds, 0);

        //Bounce off ground (reflect velocity using surface normal)
        if (func_8005509C(self, &self->prevLocalPosition.x, &self->srt.transl.x, 1, &sp4C, 0) != 0) {
            unitV.f[0] = -self->velocity.f[0];
            unitV.f[1] = -self->velocity.f[1];
            unitV.f[2] = -self->velocity.f[2];
            speed = sqrtf(SQ(unitV.f[0]) + SQ(unitV.f[1]) + SQ(unitV.f[2]));
            
            //Stop when speed low
            if (speed < 0.3f) {
                speed = 0.0f;
                unitV.f[0] = 0.0f;
                unitV.f[1] = 0.0f;
                unitV.f[2] = 0.0f;

                if (objData->timesHit == 3) {
                    objData->state = DIMBoulder_STATE_2_Finished;
                } else {
                    objData->state = DIMBoulder_STATE_0_Stopped;
                }
            }

            //Play impact sound (debounced)
            if (objData->soundTimer <= 0) {
                volume = (speed / 2.0f) * 127.0f;
                if (volume > MAX_VOLUME) {
                    volume = MAX_VOLUME;
                }
                
                gDLL_6_AMSFX->vtbl->play(self, SOUND_111_Heavy_Collision, volume, NULL, NULL, 0, NULL);
                objData->soundTimer = 30;
            }

            //Get unit velocity vector (negated)
            if (speed) {
                unitV.f[0] *= 1.0f / speed;
                unitV.f[1] *= 1.0f / speed;
                unitV.f[2] *= 1.0f / speed;
            }
            
            //Get reflected velocity vector
            //R = V - 2*(V ⋅ N)*N

            normalV.f[0] = sp4C.unk0[0].f[0];
            normalV.f[1] = sp4C.unk0[0].f[1];
            normalV.f[2] = sp4C.unk0[0].f[2];

            twoTimesDot = 2.0f * DOT_PRODUCT(unitV, normalV);
            
            self->velocity.f[0] = twoTimesDot * normalV.f[0];
            self->velocity.f[1] = twoTimesDot * normalV.f[1];
            self->velocity.f[2] = twoTimesDot * normalV.f[2];
            
            self->velocity.f[0] -= unitV.f[0];
            self->velocity.f[1] -= unitV.f[1];
            self->velocity.f[2] -= unitV.f[2];

            //Lose some momentum
            self->velocity.f[0] *= speed * 0.7f;
            self->velocity.f[1] *= speed * 0.5f;
            self->velocity.f[2] *= speed * 0.7f;
        }

        //Decrement sound debounce timer
        if (objData->soundTimer > 0) {
            objData->soundTimer -= updateRate;
        }

        //Rotate with lateral velocity
        speedX = -self->velocity.f[2];
        self->srt.pitch -= speedX * 1000.0f;
        
        temp = self->velocity.f[0];
        self->srt.roll -= temp * 1000.0f;
        
        //Set scale
        self->srt.scale = self->def->scale * objData->scale;
        
        //Save position
        map_save_object(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
    }
}

// offset: 0x634 | func: 2 | export: 2
void DIMBoulder_update(Object *self) { }

// offset: 0x640 | func: 3 | export: 3
void DIMBoulder_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x694 | func: 4 | export: 4
void DIMBoulder_free(Object *self, s32 onlySelf) { }

// offset: 0x6A4 | func: 5 | export: 5
u32 DIMBoulder_get_model_flags(Object *self) {
    return MODFLAGS_SHADOW;
}

// offset: 0x6B4 | func: 6 | export: 6
u32 DIMBoulder_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DIMBoulder_Data);
}
