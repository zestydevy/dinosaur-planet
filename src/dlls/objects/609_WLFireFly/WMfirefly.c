#include "common.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "dlls/objects/609_WMFireFly.h"
#include "sys/objtype.h"

typedef struct {
    Object* lfxEmitter;
    f32 splineX[4];
    f32 splineY[4];
    f32 splineZ[4];
    Vec3f nextSplineCoord;
    f32 tValueSpline;
    f32 tValueSpeed;
    f32 unk48;              //Unknown purpose: increases slowly while player/sidekick nearby, decreases slowly otherwise
    f32 fxRange;            //Particles appear when player/sidekick are within this range
    f32 varianceZ;
    Vec3f home;
    s16 randomYaw;
    s16 unk62;
    s16 varianceY;
    u8 effectType;
    s8 unk67;
    u8 splinePointNum;
    s8 unk69;
    u8 lightCreated;
    s8 unk6B;
} WMFireFly_Data;

static void WMFireFly_append_spline_point(Object* self);
static void WMFireFly_set_next_spline_coord_randomised(Object* self);

/*0x0*/ static u32 data_0 = 0;

// offset: 0x0 | ctor
void WMFireFly_ctor(void* dll) { }

// offset: 0xC | dtor
void WMFireFly_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WMFireFly_setup(Object* self, WMFireFly_Setup* objSetup, s32 reset) {
    WMFireFly_Data* objData;
    Vec3f v;
    
    objData = self->data;
    
    obj_add_object_type(self, OBJTYPE_FireFly);

    v.f[0] = self->srt.transl.x;
    v.f[1] = self->srt.transl.y;
    v.f[2] = self->srt.transl.z;
    
    objData->splineX[0] = v.f[0];
    objData->splineY[0] = v.f[1];
    objData->splineZ[0] = v.f[2];
    
    objData->splineX[1] = v.f[0];
    objData->splineY[1] = v.f[1];
    objData->splineZ[1] = v.f[2];
    
    objData->splineX[2] = v.f[0];
    objData->splineY[2] = v.f[1];
    objData->splineZ[2] = v.f[2];
    
    objData->splineX[3] = v.f[0];
    objData->splineY[3] = v.f[1];
    objData->splineZ[3] = v.f[2];

    objData->lfxEmitter = NULL;
    objData->lightCreated = FALSE;

    objData->tValueSpeed = 0.01f;
    objData->unk48 = 0.0275f;
    objData->tValueSpline = 1.0f;
    objData->splinePointNum = 0;
    objData->unk67 = 0;
    objData->unk62 = rand_next(500, 1500);
    objData->randomYaw = rand_next(0, 65000);
    objData->varianceY = objSetup->varianceY;
    objData->effectType = objSetup->effectType;
    objData->fxRange = objSetup->fxRange;
    objData->varianceZ = objSetup->varianceZ;
    
    objData->home.x = objSetup->base.x;
    objData->home.y = objSetup->base.y;
    objData->home.z = objSetup->base.z;
    
    objData->unk6B = 0;
}

// offset: 0x1B4 | func: 1 | export: 1
void WMFireFly_control(Object* self) {
    WMFireFly_Data* objData;
    Object* player;
    Object* sidekick;
    LFXEmitter_Setup* lfxSetup;

    objData = self->data;
    player = get_player();
    sidekick = get_sidekick();
    
    if (objData->tValueSpline > 1.0f) {
        objData->tValueSpline -= 1.0f;
        if (objData->splinePointNum >= 4) {
            objData->splinePointNum++;
        } else {
            WMFireFly_set_next_spline_coord_randomised(self);
        }
        
        WMFireFly_append_spline_point(self);
    }
    
    self->srt.transl.x = curves_b_spline(objData->splineX, objData->tValueSpline, NULL);
    self->srt.transl.y = curves_b_spline(objData->splineY, objData->tValueSpline, NULL);
    self->srt.transl.z = curves_b_spline(objData->splineZ, objData->tValueSpline, NULL);
    
    objData->tValueSpline += objData->tValueSpeed * gUpdateRateF;
    
    self->srt.yaw = arctan2_f(self->srt.transl.x - self->prevLocalPosition.x, self->srt.transl.f[2] - self->prevLocalPosition.f[2]);

    if (objData->lfxEmitter && (objData->lfxEmitter->stateFlags & OBJSTATE_DESTROYED)) {
        objData->lightCreated = FALSE;
        objData->lfxEmitter = NULL;
    }

    if ((objData->effectType == 1) || (objData->effectType == 4)) {
        if ((vec3_distance_xz(&player->globalPosition, (Vec3f* ) &self->setup->x) < 190.0f) || ((sidekick != NULL) && (vec3_distance_xz(&sidekick->globalPosition, (Vec3f* ) &self->setup->x) < 190.0f))) {
            if (objData->lightCreated == FALSE) {
                objData->lightCreated = TRUE;
                lfxSetup = obj_alloc_setup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
                lfxSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
                lfxSetup->base.x = self->srt.transl.x;
                lfxSetup->base.y = self->srt.transl.y;
                lfxSetup->base.z = self->srt.transl.z;
                lfxSetup->unk1E = rand_next(0, 1) + 0x1AA;
                lfxSetup->unk22 = -1;
                objData->lfxEmitter = obj_create((ObjSetup*)lfxSetup, 5, self->mapID, -1, self->parent);
            }

            objData->lfxEmitter->srt.transl.x = self->srt.transl.x;
            objData->lfxEmitter->srt.transl.y = self->srt.transl.y;
            objData->lfxEmitter->srt.transl.z = self->srt.transl.z;
        } else {
            if (((objData->effectType == 1) || (objData->effectType == 4)) && (objData->lightCreated == TRUE)) {
                if (objData->lfxEmitter != NULL) {
                    obj_destroy_object(objData->lfxEmitter);
                    objData->lightCreated = FALSE;
                    objData->lfxEmitter = NULL;
                }
            }
        }
    }

    if ((objData->effectType == 1) || (objData->effectType == 4)) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_19E, NULL, 1, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A0, NULL, 1, -1, NULL);
    } else {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1BB, NULL, 1, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1BD, NULL, 1, -1, NULL);
    }
    
    //Create more particles when the player or sidekick are nearby
    if ((vec3_distance_xz(&player->globalPosition, (Vec3f*)&self->setup->x) < objData->fxRange) || 
        (sidekick && (vec3_distance_xz(&sidekick->globalPosition, (Vec3f*)&self->setup->x) < objData->fxRange))
    ) {
        if (objData->effectType == 4) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_19F, NULL, 1, -1, NULL);
        } else if (objData->effectType == 3) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1BC, NULL, 1, -1, NULL);
        } else if (objData->effectType == 5) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1BC, NULL, 1, -1, NULL);
        }

        //Slowly increase value while in range (framerate dependent)
        if (objData->unk48 < 0.003f) {
            objData->unk48 += 0.00001f;
            if (objData->unk48 > 0.003f) {
                objData->unk48 = 0.003f;
            }
        }
    } else {
        //Slowly decrease value while in range (framerate dependent)
        if (objData->unk48 > 0.001f) {
            objData->unk48 -= 0.00001f;
            if (objData->unk48 < 0.001f) {
                objData->unk48 = 0.001f;
            }
        }
    }
}

// offset: 0x7B8 | func: 2 | export: 2
void WMFireFly_update(Object* self) { }

// offset: 0x7C4 | func: 3 | export: 3
void WMFireFly_print(Object* self, Gfx** gfl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gfl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x818 | func: 4 | export: 4
void WMFireFly_free(Object* self, s32 onlySelf) {
    WMFireFly_Data* objData = self->data;

    if (onlySelf == FALSE) {
        if (objData->lfxEmitter != NULL) {
            obj_destroy_object(objData->lfxEmitter);
        }
    }
    
    obj_free_object_type(self, OBJTYPE_FireFly);
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x8A4 | func: 5 | export: 5
u32 WMFireFly_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x8B4 | func: 6 | export: 6
u32 WMFireFly_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(WMFireFly_Data);
}

// offset: 0x8C8 | func: 7
void WMFireFly_append_spline_point(Object* self) {
    WMFireFly_Data* objData = self->data;
    
    objData->splineX[0] = objData->splineX[1];
    objData->splineY[0] = objData->splineY[1];
    objData->splineZ[0] = objData->splineZ[1];
    
    objData->splineX[1] = objData->splineX[2];
    objData->splineY[1] = objData->splineY[2];
    objData->splineZ[1] = objData->splineZ[2];
    
    objData->splineX[2] = objData->splineX[3];
    objData->splineY[2] = objData->splineY[3];
    objData->splineZ[2] = objData->splineZ[3];
    
    objData->tValueSpeed = rand_next(60, 90) * 0.00015f;
    
    objData->splineX[3] = objData->nextSplineCoord.x;
    objData->splineY[3] = objData->nextSplineCoord.y;
    objData->splineZ[3] = objData->nextSplineCoord.z;
}

// offset: 0x98C | func: 8
void WMFireFly_set_next_spline_coord_randomised(Object* self) {
    WMFireFly_Data* objData;
    SRT transform;

    objData = self->data;

    //Randomise Y and Z components
    objData->nextSplineCoord.x = 0;
    objData->nextSplineCoord.y = rand_next(-objData->varianceY, objData->varianceY);
    if (objData->varianceZ < 21.0f) {
        objData->nextSplineCoord.z = 0.0f;
    } else {
        objData->nextSplineCoord.z = objData->varianceZ - rand_next(20, (s16)objData->varianceZ);
    }

    //Pick random yaw
    objData->randomYaw += (s16)rand_next(3000, 5000);
    
    //Rotate nextSplineCoord around random yaw
    transform.transl.x = 0.0f;
    transform.transl.y = 0.0f;
    transform.transl.z = 0.0f;
    transform.scale = 1.0f;
    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = objData->randomYaw;
    rotate_vec3(&transform, objData->nextSplineCoord.f);
    
    //Transform to worldSpace
    objData->nextSplineCoord.x = (objData->home.x + objData->nextSplineCoord.x);
    objData->nextSplineCoord.y = (objData->home.y + objData->nextSplineCoord.y);
    objData->nextSplineCoord.z = (objData->home.z + objData->nextSplineCoord.z);
}

// offset: 0xADC | func: 9 | export: 7
void WMFireFly_set_next_spline_coord(Object* self, Vec3f* point) {
    WMFireFly_Data* objData = self->data;
    
    objData->nextSplineCoord.x = point->x;
    objData->nextSplineCoord.y = point->y;
    objData->nextSplineCoord.z = point->z;
    
    objData->splinePointNum = 4;
}

/*0x0*/ static const char str_0[] = " Sending FireFly ";
