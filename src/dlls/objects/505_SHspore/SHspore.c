#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/rand.h"
#include "types.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18;                       //unused
/*1A*/ s16 angularRange;                //maximum angular distance from desired travel angle
/*1C*/ s16 windAngle;                   //desired direction the spore should travel in
} SHSpore_Setup;

typedef struct {
/*000*/ DLL27_Data terrainCollider;
/*260*/ f32 lifetime;
/*264*/ f32 lateralSpeed;
/*268*/ f32 lateralAcceleration;
/*26C*/ f32 lateralSpeedGoal;
/*270*/ f32 angularJitterTimer;         //waiting time between slightly randomising flight angle
/*274*/ f32 velocityX;
/*278*/ f32 velocityZ;
/*27C*/ f32 coefficientX;               //multiplier for lateral acceleration
/*280*/ f32 coefficientZ;               //multiplier for lateral acceleration
/*284*/ f32 lateralDecelerationTimer;   //waiting time until lateral acceleration dwindles
/*288*/ f32 lateralAccelerationGoal;
/*28C*/ f32 angleChangeTimer;           //waiting time between large randomisation of flight angle
/*290*/ f32 deletionTimer;      
/*294*/ s16 angle;                      //lateral direction of travel
/*296*/ s16 angleJitter;                //lateral direction of travel (plus randomised angular noise)
/*298*/ s16 angleGoal;
} SHSpore_Data;

static void SHspore_change_flight_direction(Object* self, SHSpore_Data* objData);
static void SHspore_jitter_flight_direction(Object* self, SHSpore_Data* objData);

/*0x0*/ static Vec3f data_collision_test_point = { 0, 0, 0 };
/*0xC*/ static f32 data_collision_radius = 5.0f;

// offset: 0x0 | ctor
void SHspore_ctor(void *dll) { }

// offset: 0xC | dtor
void SHspore_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHspore_setup(Object* self, s32 arg1, s32 arg2) {
    SHSpore_Data* objData;
    s8 pad[4];
    u8 sp37;

    objData = self->data;
    sp37 = 5;
    objData->lifetime = 1500.0f; //25s
    self->unkB0 |= 0x6000;
    self->speed.y = 3.0f;

    func_800267A4(self);
    objData->angleGoal = rand_next(0, 0xFFFF);
    objData->lateralSpeedGoal = rand_next(0, 1000) / 1000.0f;

    gDLL_27->vtbl->init(&objData->terrainCollider, DLL27FLAG_NONE, DLL27FLAG_2 | DLL27FLAG_40000, DLL27MODE_1);
    gDLL_27->vtbl->setup_terrain_collider(&objData->terrainCollider, 1, &data_collision_test_point, &data_collision_radius, &sp37);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3F1, NULL, PARTFXFLAG_4, -1, NULL);
}

// offset: 0x18C | func: 1 | export: 1
void SHspore_control(Object* self) {
    SHSpore_Data* objData;
    s8 pad[10];
    s32 particleCount;
    f32 temp;
    f32 lateralJolt;
    s32 index;
    Object* collidedObject;

    objData = (SHSpore_Data*)self->data;

    if (objData->deletionTimer != 0.0f) {
        self->srt.yaw += gUpdateRate << 6;
        objData->deletionTimer -= gUpdateRateF;
        if (objData->deletionTimer <= 0.0f) {
            obj_destroy_object(self);
        }
    } else {
        //Update motion
        objData->angularJitterTimer -= gUpdateRateF;
        if (objData->angularJitterTimer < 0.0f) {
            objData->angularJitterTimer = 0.0f;
        }
        objData->angleChangeTimer -= gUpdateRateF;
        if (objData->angleChangeTimer < 0.0f) {
            objData->angleChangeTimer = 0.0f;
        }

        self->speed.y += -0.009f * gUpdateRateF;
        if (self->speed.y < -0.2f) {
            self->speed.y = -0.2f;
        }
        if (self->speed.y > 0) {
            self->speed.y *= 0.97f; //framerate dependent? Ascent would slow more rapidly at 60fps
        }
        if (self->speed.y < 0.0f) {
            func_8002674C(self);
        }

        SHspore_change_flight_direction(self, objData);
        if ((rand_next(0, 100) < 5) && (objData->angularJitterTimer <= 0.0f)) {
            SHspore_jitter_flight_direction(self, objData);
        }

        objData->lateralDecelerationTimer -= gUpdateRateF;
        if (objData->lateralDecelerationTimer <= 0.0f) {
            objData->coefficientX *= 0.97f;
            objData->coefficientZ *= 0.97f;
            objData->lateralDecelerationTimer = 0.0f;
        } else {
            lateralJolt = objData->lateralAccelerationGoal - objData->lateralAcceleration;
            objData->lateralAcceleration += lateralJolt * 0.01f * gUpdateRateF;
        }
        self->speed.x = objData->velocityX + (objData->coefficientX * objData->lateralAcceleration);
        self->speed.z = objData->velocityZ + (objData->coefficientZ * objData->lateralAcceleration);
        obj_integrate_speed(self, self->speed.x * gUpdateRateF, self->speed.y * gUpdateRateF, self->speed.z * gUpdateRateF);
        gDLL_27->vtbl->func_1E8(self, &objData->terrainCollider, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, &objData->terrainCollider);
        gDLL_27->vtbl->func_624(self, &objData->terrainCollider, gUpdateRateF);
        func_80026128(self, 0xA, 0, 0);

        //Handle object collisions
        collidedObject = self->objhitInfo->unk48;
        if (collidedObject) {
            particleCount = 20;
            if (get_player() == collidedObject) {
                //Player collecting purple mushroom
                main_increment_bits(BIT_Inventory_Purple_Mushrooms);
                particleCount = 0;
            }
            if (collidedObject->id != OBJ_SHrocketmushroo) {
                //Other objects (ignoring SHrocketmushroom since the spores emerge out of it)
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B31_Item_Collection_Chime, MAX_VOLUME, NULL, 0, 0, 0);
                gDLL_13_Expgfx->vtbl->func4(self);

                //Create collision particles
                for (index = 0; index < particleCount; index++){
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3F3, NULL, PARTFXFLAG_4, -1, NULL);
                }

                objData->deletionTimer = 200.0f;
                self->srt.flags |= 0x4000;
                func_800267A4(self);
            }
        } else {
            objData->lifetime -= gUpdateRateF;
            //Destroy the spore if its lifetime runs out or it collides with terrain
            if (objData->lifetime <= 0.0f || objData->terrainCollider.unk25C & 0x11) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_8A2_Spore_Disintegrate, MAX_VOLUME, NULL, 0, 0, 0);
                gDLL_13_Expgfx->vtbl->func4(self);

                //Create collision particles
                for (index = 0; index < 20; index++){
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3F3, NULL, PARTFXFLAG_4, -1, NULL);
                }

                objData->deletionTimer = 200.0f;
                self->srt.flags |= 0x4000;
                func_800267A4(self);
            }
        }
    }
}

// offset: 0x750 | func: 2
static void SHspore_change_flight_direction(Object* self, SHSpore_Data* objData) {
    s8 pad[4];
    SHSpore_Setup* setup;
    f32 lateralAcceleration;
    s32 windAngle;
    s32 deltaTheta;

    setup = (SHSpore_Setup*)self->setup;

    windAngle = setup->windAngle;

    //10% chance of shifting away from current angle (approximately 11 to 22 degree shift)
    if (rand_next(0, 100) < 10 && objData->angleChangeTimer <= 0.0f) {
        objData->angleGoal = rand_next(2000, 4000);
        //Either clockwise or anticlockwise shift
        if (rand_next(0, 1)) {
            objData->angleGoal = -objData->angleGoal;
        }
        objData->angleGoal += objData->angle;

        //Constrain goal angle based on setup parameters
        deltaTheta = objData->angleGoal - (windAngle & 0xFFFF);
        CIRCLE_WRAP(deltaTheta)
        if (deltaTheta > setup->angularRange) {
            objData->angleGoal = windAngle + setup->angularRange;
        }
        if (deltaTheta < -setup->angularRange) {
            objData->angleGoal = windAngle - setup->angularRange;
        }

        objData->angleChangeTimer = 150.0f;
    }

    //Randomise lateral speed
    if (rand_next(0, 100) < 10 && objData->angleChangeTimer <= 0.0f) {
        objData->lateralSpeedGoal = (rand_next(-200, 200) / 1000.0f) + objData->lateralSpeed;
        if (objData->lateralSpeedGoal < 0.5f) {
            objData->lateralSpeedGoal = 0.5f;
        } else if (objData->lateralSpeedGoal > 1.0f) {
            objData->lateralSpeedGoal = 1.0f;
        }
    }

    deltaTheta = objData->angleGoal - (objData->angle & 0xFFFF);
    CIRCLE_WRAP(deltaTheta)

    objData->angle += (deltaTheta * gUpdateRate) >> 4;

    lateralAcceleration = objData->lateralSpeedGoal - objData->lateralSpeed;
    objData->lateralSpeed += lateralAcceleration * 0.006f * gUpdateRateF;
    objData->velocityX = fsin16_precise(objData->angle) * objData->lateralSpeed;
    objData->velocityZ = fcos16_precise(objData->angle) * objData->lateralSpeed;
}

// offset: 0xA00 | func: 3
static void SHspore_jitter_flight_direction(Object* self, SHSpore_Data* objData) {
    SHSpore_Setup* setup;
    s32 windAngle;
    s32 deltaTheta;

    setup = (SHSpore_Setup*)self->setup;

    windAngle = setup->windAngle;
    objData->lateralDecelerationTimer = rand_next(30, 45);
    objData->angularJitterTimer = rand_next(120, 180) + objData->lateralDecelerationTimer;
    objData->angleJitter = rand_next(-2000, 2000) + objData->angle; //up to ~11 degrees away from current angle

    deltaTheta = objData->angleJitter - (windAngle & 0xFFFF);
    CIRCLE_WRAP(deltaTheta)

    //Clamp direction of travel within acceptable range
    if (deltaTheta > setup->angularRange) {
        objData->angleJitter = windAngle + setup->angularRange;
    }
    if (deltaTheta < -setup->angularRange) {
        objData->angleJitter = windAngle - setup->angularRange;
    }

    objData->lateralAccelerationGoal = rand_next(900, 1300) / 1000.0f;
    objData->lateralAcceleration = 0.0f;
    objData->coefficientX = fsin16_precise(objData->angleJitter);
    objData->coefficientZ = fcos16_precise(objData->angleJitter);
}

// offset: 0xB88 | func: 4 | export: 2
void SHspore_update(Object *self) { }

// offset: 0xB94 | func: 5 | export: 3
void SHspore_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xBAC | func: 6 | export: 4
void SHspore_free(Object* self, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func4(self);
}

// offset: 0xBF4 | func: 7 | export: 5
u32 SHspore_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xC04 | func: 8 | export: 6
u32 SHspore_get_data_size(Object *self, s32 arg1) {
    return sizeof(SHSpore_Data);
}
