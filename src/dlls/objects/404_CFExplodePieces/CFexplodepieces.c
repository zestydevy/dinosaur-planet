#include "common.h"
#include "dlls/objects/404_CFExplodePieces.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/gfx/model.h"

typedef struct {
    Vec3f centrepoint;
    Vec3f positionOffset;
    f32 yawSpeed;
    f32 pitchSpeed;
    f32 rollSpeed;
    f32 yawAcceleration;
    f32 pitchAcceleration;
    f32 rollAcceleration;
    Vec3f acceleration;
    SRT _unk3C;
    f32 heightFromGround;
    s32 fadeTimer;
    s32 fadeEndTime;
    u8 _unk60[0x66 - 0x60];
    u8 flags;
    u8 unk67; //Unused in practice, but possibly meant as opacity?
    u8 _unk68;
    u8 state;
} CFExplodePieces_Data;

static s32 CFExplodePieces_fadeOut(Object* self, CFExplodePieces_Data* objData);
static s32 CFExplodePieces_move(Object* self, CFExplodePieces_Data* objData);
static void CFExplodePieces_setupModelAndPhysics(Object* self, CFExplodePieces_Setup* objSetup, s32 reset, CFExplodePieces_Data* objData);

// offset: 0x0 | ctor
void CFExplodePieces_ctor(void* dll) { }

// offset: 0xC | dtor
void CFExplodePieces_dtor(void* dll) { }

/*0x0*/ static const char str_0[] = "phys %i ";

// offset: 0x18 | func: 0 | export: 0
void CFExplodePieces_obj_Setup(Object* self, CFExplodePieces_Setup* objSetup, s32 reset) {
    CFExplodePieces_Data* objData;

    objData = self->data;
    self->modelInstIdx = objSetup->modelIdx;

    CFExplodePieces_setupModelAndPhysics(self, objSetup, reset, objData);
    
    if (objSetup->velocity.x || objSetup->velocity.y || objSetup->velocity.z || 
        objSetup->acceleration.x || objSetup->acceleration.y || objSetup->acceleration.z
    ) {
        objData->state = CFExplodePieces_STATE_1_Moving;
    } else {
        objData->state = CFExplodePieces_STATE_0_Stopped;
    }
}

// offset: 0xC4 | func: 1 | export: 1
void CFExplodePieces_obj_Control(Object* self) {
    CFExplodePieces_Data* objData = self->data;
    
    switch (objData->state) {
    case CFExplodePieces_STATE_0_Stopped:
    case CFExplodePieces_STATE_2_Finished:
        break;
    case CFExplodePieces_STATE_1_Moving:
        if (CFExplodePieces_move(self, objData)) {
            objData->state = CFExplodePieces_STATE_0_Stopped;
        }
        break;
    }
    
    if (CFExplodePieces_fadeOut(self, objData)) {
        objData->state = CFExplodePieces_STATE_2_Finished;
    }
}

// offset: 0x16C | func: 2
s32 CFExplodePieces_fadeOut(Object* self, CFExplodePieces_Data* objData) {
    s32 opacity;

    if (objData->fadeEndTime != -1) {
        objData->fadeTimer += gUpdateRate;
        if (objData->fadeTimer >= objData->fadeEndTime) {
            objData->fadeEndTime = -1;
            self->opacity = 0;
            self->srt.flags |= OBJFLAG_INVISIBLE;
            return 1;
        } else {
            opacity = objData->fadeEndTime - objData->fadeTimer;
            if (opacity < OBJECT_OPACITY_MAX) {
                //@bug: opacity could be given a negative value here at the end
                self->opacity = opacity;
            }
        }
    }
    
    return 0;
}

// offset: 0x1E0 | func: 3
/**
  * The piece spins through the air, bounces off the ground, and eventually slides to a halt on the ground.
  *
  * Returns TRUE when the motion is finished.
  */
s32 CFExplodePieces_move(Object* self, CFExplodePieces_Data* objData) {
    f32 lateralSpeed;

    self->velocity.x += gUpdateRateF * objData->acceleration.x;
    self->velocity.y += gUpdateRateF * objData->acceleration.y;
    self->velocity.z += gUpdateRateF * objData->acceleration.z;
    
    objData->yawSpeed   += gUpdateRateF * objData->yawAcceleration;
    objData->pitchSpeed += gUpdateRateF * objData->pitchAcceleration;
    objData->rollSpeed  += gUpdateRateF * objData->rollAcceleration;

    self->srt.transl.x += self->velocity.x * gUpdateRateF;
    self->srt.transl.y += self->velocity.y * gUpdateRateF;
    self->srt.transl.z += self->velocity.z * gUpdateRateF;
    
    //Bounce
    if (self->srt.transl.y < objData->heightFromGround) {
        self->velocity.y = -self->velocity.y * 0.5f;
        objData->acceleration.y = -0.07f;
        objData->rollAcceleration = -objData->rollAcceleration;

        //Stop vertical motion, slide to a halt
        if (((self->velocity.y > 0) && (objData->flags & CFExplodePieces_FLAGS_4_Touching_Ground)) || (self->velocity.y == 0)) {
            objData->acceleration.y = 0;

            objData->rollAcceleration = 0;
            objData->rollSpeed = 0;
            objData->pitchAcceleration = 0;
            objData->pitchSpeed = 0;
            objData->yawAcceleration = 0;
            objData->yawSpeed = 0;
            
            self->velocity.y = 0;
            
            objData->acceleration.x *= 0.6f;
            self->velocity.x *= 0.6f;
            objData->acceleration.z *= 0.6f;
            self->velocity.z *= 0.6f;
            
            if (self->velocity.x >= 0.0f) {
                lateralSpeed = self->velocity.x;
            } else {
                lateralSpeed = -self->velocity.x;
            }
            
            if (lateralSpeed < 0.15f) {
                if (self->velocity.z >= 0.0f) {
                    lateralSpeed = self->velocity.z;
                } else {
                    lateralSpeed = -self->velocity.z;
                }
                
                if (lateralSpeed < 0.15f) {
                    return TRUE;
                }
            }
        }

        objData->flags |= CFExplodePieces_FLAGS_4_Touching_Ground;
    } else {
        objData->flags &= ~CFExplodePieces_FLAGS_4_Touching_Ground;
    }
    
    self->srt.yaw   += objData->yawSpeed   * gUpdateRateF;
    self->srt.pitch += objData->pitchSpeed * gUpdateRateF;
    self->srt.roll  += objData->rollSpeed  * gUpdateRateF;
    
    return FALSE;
}

// offset: 0x4B8 | func: 4 | export: 2
void CFExplodePieces_obj_Update(Object* self) { }

// offset: 0x4C4 | func: 5 | export: 3
void CFExplodePieces_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

/*0xC*/ static const char str_C[] = " Freeing Physic Obj ";

// offset: 0x518 | func: 6 | export: 4
void CFExplodePieces_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x528 | func: 7 | export: 5
u32 CFExplodePieces_obj_GetModelFlags(Object* self) {
    CFExplodePieces_Setup* setup = (CFExplodePieces_Setup*)self->setup;
    return MODFLAGS_MODEL_INDEX(setup->modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x540 | func: 8 | export: 6
u32 CFExplodePieces_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFExplodePieces_Data);
}

// offset: 0x554 | func: 9 | export: 7
u8 CFExplodePieces_GetState(Object* self) {
    CFExplodePieces_Data* objData = self->data;
    return objData->state;
}

/*0x24*/ static const char str_24[] = " Hieght From Ground %i ";

// offset: 0x564 | func: 10
static void CFExplodePieces_setupPhysics(Object* self, CFExplodePieces_Data* objData, CFExplodePieces_Setup* objSetup) {
    f32 floorHeight = 0.0f;
    
    self->srt.transl.x = objSetup->base.x + (objData->positionOffset.x * self->srt.scale);
    self->srt.transl.y = objSetup->base.y + (objData->positionOffset.y * self->srt.scale);
    self->srt.transl.z = objSetup->base.z + (objData->positionOffset.z * self->srt.scale);
    self->srt.yaw   = objSetup->yaw;
    self->srt.pitch = objSetup->pitch;
    self->srt.roll  = objSetup->roll;
    self->velocity.x = objSetup->velocity.x / 100.0f;
    self->velocity.y = objSetup->velocity.y / 100.0f;
    self->velocity.z = objSetup->velocity.z / 100.0f;
    objData->yawSpeed   = objSetup->yawSpeed;
    objData->pitchSpeed = objSetup->pitchSpeed;
    objData->rollSpeed  = objSetup->rollSpeed;

    if (objSetup->floorOffset == 0) {
        trackGetHeightFloor(self, self->srt.transl.x, self->srt.transl.y - 10.0f, self->srt.transl.z, &floorHeight, 0);
        objData->heightFromGround = self->srt.transl.y - floorHeight;
    } else {
        objData->heightFromGround = self->srt.transl.y + (s16) objSetup->floorOffset;
    }
    
    objData->yawAcceleration   = objSetup->yawAcceleration   / 10.0f;
    objData->pitchAcceleration = objSetup->pitchAcceleration / 10.0f;
    objData->rollAcceleration  = objSetup->rollAcceleration  / 10.0f;
    objData->acceleration.x = objSetup->acceleration.x / 1000.0f;
    objData->acceleration.y = objSetup->acceleration.y / 1000.0f;
    objData->acceleration.z = objSetup->acceleration.z / 1000.0f;
    objData->fadeTimer = 0;
    
    //Set fade end time (randomised, anywhere from 50% to 100% of lifetimeMax)
    if (objSetup->lifetimeMax) {
        objData->fadeEndTime = ((mathRnd(0, 100) + 100) * objSetup->lifetimeMax) / 200;
    } else {
        objData->fadeEndTime = -1;
    }
}

// offset: 0x804 | func: 11
void CFExplodePieces_setupModelAndPhysics(Object* self, CFExplodePieces_Setup* objSetup, s32 reset, CFExplodePieces_Data* objData) {
    Vtx* vtxs0;
    Vtx* vtxs1;
    s32 i;
    s32 averageX;
    s32 averageY;
    s32 averageZ;
    ModelInstance* modelInstance;
    Model* model;
    
    if (reset == FALSE) {
        //Calculate model centrepoint
        objData->centrepoint.z = objData->centrepoint.y = objData->centrepoint.x = 0.0f;
        
        averageX = 0;
        averageY = 0;
        averageZ = 0;
        
        modelInstance = self->modelInsts[objSetup->modelIdx];
        model = modelInstance->model;
        
        for (i = 0; i < model->vertexCount; i++) {
            averageX += model->vertices[i].v.ob[0];
            averageY += model->vertices[i].v.ob[1];
            averageZ += model->vertices[i].v.ob[2];
        }
        averageX /= model->vertexCount;
        averageY /= model->vertexCount;
        averageZ /= model->vertexCount;
        
        //Move model vertices so the centrepoint's at the origin
        vtxs0 = modelInstance->vertices[0];
        vtxs1 = modelInstance->vertices[1];
        
        for (i = 0; i < model->vertexCount; i++) {
            vtxs0[i].v.ob[0] -= averageX;
            vtxs0[i].v.ob[1] -= averageY;
            vtxs0[i].v.ob[2] -= averageZ;
            if (vtxs1 != vtxs0) {
                vtxs1[i].v.ob[0] -= averageX;
                vtxs1[i].v.ob[1] -= averageY; 
                vtxs1[i].v.ob[2] -= averageZ; 
            } 
        } 
        
        //Store centrepoint
        objData->centrepoint.x = averageX; 
        objData->centrepoint.y = averageY; 
        objData->centrepoint.z = averageZ; 
    }
    
    //Set the original model centrepoint as the Object's initial position offset
    objData->positionOffset.x = objData->centrepoint.x;
    objData->positionOffset.y = objData->centrepoint.y;
    objData->positionOffset.z = objData->centrepoint.z;
    
    CFExplodePieces_setupPhysics(self, objData, objSetup);
    
    objData->unk67 = 0xFF;
}
