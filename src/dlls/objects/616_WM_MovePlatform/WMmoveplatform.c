//NOTE: this is an unused object, distinct from WM_Platform

#include "common.h"
#include "sys/intersect.h"
#include "sys/objects.h"
#include "sys/objprint.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 playerRange;
} WM_MovePlatform_Setup;

typedef struct {
    f32 playerRange;
    f32 accelerationY;
    u8 playerOnPlatform;
} WM_MovePlatform_Data;

// offset: 0x0 | ctor
void WM_MovePlatform_ctor(void* dll) { }

// offset: 0xC | dtor
void WM_MovePlatform_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WM_MovePlatform_obj_Setup(Object* self, WM_MovePlatform_Setup* objSetup, s32 reset) {
    WM_MovePlatform_Data* objData = self->data;
    
    self->srt.scale *= 0.25f;
    self->srt.yaw = objSetup->yaw << 8;
    
    objData->playerRange = objSetup->playerRange;
    objData->playerOnPlatform = 0;
    objData->accelerationY = 0.0f;
}

// offset: 0x74 | func: 1 | export: 1
void WM_MovePlatform_obj_Control(Object* self) {
    WM_MovePlatform_Data* objData;
    Object* player;
    f32 distance;
    s32 pad;
    f32 speed;
    TrackHeightResult** collisionInfo;
    f32 dSpeed;
    f32 dy;
    f32 minDistance;
    s32 count;
    f32 collisionY;
    SRT fxTransform;
    s32 i;

    objData = self->data;
    player = objGetPlayer();
    collisionInfo = NULL;
    speed = 1.0f;
    collisionY = 0.0f;
    
    //Check if the player's on/near the platform
    if ((vec3DistanceXZ(&player->globalPosition, &self->globalPosition) < objData->playerRange) && (objData->playerOnPlatform == FALSE)) {
        objData->playerOnPlatform = TRUE;
    }
    
    //Bail if the player isn't on the platform
    if ((objData->playerOnPlatform == FALSE) || (objData->playerOnPlatform != TRUE)) {
        return;
    }

    //Lock player to platform's position
    player->srt.transl.x = self->srt.transl.x;
    player->srt.transl.y = self->srt.transl.y;
    player->srt.transl.z = self->srt.transl.z;

    //Control platform using joystick(!) (not transformed with respect to camera, though)
    self->velocity.y += -0.04f;
    self->velocity.x += joyGetStickY(0) * 0.001f;
    self->velocity.z += joyGetStickX(0) * 0.001f;
    
    //Use D-pad left to boost upwards, creating a flurry of flame particles!
    if (joyGetButtons(0) & L_JPAD) {
        objData->accelerationY += 0.0015f; //@framerate-dependent
        fxTransform.scale = objData->accelerationY * 4.0f;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A5, &fxTransform, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A5, &fxTransform, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A5, &fxTransform, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A5, &fxTransform, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A5, &fxTransform, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A5, &fxTransform, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A5, &fxTransform, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A5, &fxTransform, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A5, &fxTransform, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A5, &fxTransform, 2, -1, NULL);
    } else {
        //Fall
        if (objData->accelerationY >= 0.0f) {
            objData->accelerationY -= 0.005f; //@framerate-dependent
        }
    }
    self->velocity.y += objData->accelerationY; //@framerate-dependent
    
    //Handle terrain collisions
    {
        minDistance = 10000.0f;
        count = trackGetHeight(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &collisionInfo, 0, 0);
        
        for (i = 0; i < count; i++) {
            dy = collisionInfo[i]->y - self->srt.transl.f[1];
            distance = dy;
            if (dy < 0) {
                dy = collisionInfo[i]->y - self->srt.transl.f[1];
                distance *= -1.0f;
            }
            if (distance < minDistance) {
                minDistance = distance;
                collisionY = dy;
            }
        }
        
        if (1) {} //fake
        
        if (count != 0) {
            if (collisionY > -10.0f) {
                if (self->velocity.y < 0.0f) {
                    self->velocity.y = 0.0f;
                }
                speed = 20.0f;
            } else if (collisionY > -30.0f) {
                speed = 10.0f;
            }
        }
    }
    
    //Move
    {
        dSpeed = SQ(self->velocity.x) * 0.04576f * speed;
        if (self->velocity.x > 0.0f) {
            dSpeed *= -1.0f;
        }
        self->velocity.x += dSpeed;
        
        dSpeed = SQ(self->velocity.z) * 0.04576f * speed;
        if (self->velocity.z > 0.0f) {
            dSpeed *= -1.0f;
        }
        self->velocity.z += dSpeed;
        
        if (joyGetButtons(0) & L_JPAD) {
            speed = 1.0f;
        }
        
        dSpeed = SQ(self->velocity.y) * 0.051999997f * speed;
        if (self->velocity.y > 0.0f) {
            dSpeed *= -1.0f;
        }
        self->velocity.y += dSpeed;
        
        self->srt.transl.x += self->velocity.x * gUpdateRateF;
        self->srt.transl.y += self->velocity.y * gUpdateRateF;
        self->srt.transl.z += self->velocity.z * gUpdateRateF;
    }
}

// offset: 0x6E4 | func: 2 | export: 2
void WM_MovePlatform_obj_Update(Object* self) { }

// offset: 0x6F0 | func: 3 | export: 3
void WM_MovePlatform_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x744 | func: 4 | export: 4
void WM_MovePlatform_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x754 | func: 5 | export: 5
u32 WM_MovePlatform_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x764 | func: 6 | export: 6
u32 WM_MovePlatform_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WM_MovePlatform_Data);
}
