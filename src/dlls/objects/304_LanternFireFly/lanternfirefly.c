#include "common.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "macros.h"
#include "sys/math.h"
#include "sys/objtype.h"
#include "sys/curves.h"

typedef struct {
    ObjSetup base;
    s8 varianceY;
    s8 effectType;
    s16 lifetime;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} LanternFireFly_Setup;

typedef struct {
    Object* lfxEmitter;
    f32 splineX[4];
    f32 splineY[4];
    f32 splineZ[4];
    Vec3f homeRelativeToPlayer;
    f32 tValueSpline; //tValue
    f32 tValueSpeed;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    Vec3f home;         //Firefly's initial coordinates in worldSpace 
    s32 lifetime;
    u32 soundHandle;    //Handle for the firefly's twinkling sound loop
    s16 randomYaw;
    s16 unk6A;
    s16 varianceY;
    u8 effectType;      //Decides what kind of partFX are created for the firefly, and whether to create LFXEmitters
    s8 unk6F;
    u8 unk70;
    s8 unk71;
    u8 lightCreated;
    s8 unk73;
    u32 flagLightPerFirefly : 2;
} LanternFireFly_Data; //0x78

/*0x0*/ static s8 data_0 = 0;
/*0x4*/ static u8 dLightCreated = 0;

static void LanternFireFly_set_spline_point(Object* self);
static void LanternFireFly_func_AB0(Object* self);
void LanternFireFly_func_BE0(Object* self, Vec3f* arg1);
void LanternFireFly_set_home(Object* self, f32 x, f32 y, f32 z);

// offset: 0x0 | ctor
void LanternFireFly_ctor(void *dll) { }

// offset: 0xC | dtor
void LanternFireFly_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void LanternFireFly_setup(Object* self, LanternFireFly_Setup* objSetup, s32 reset) {
    LanternFireFly_Data* objData = self->data;
    Vec3f vZero;

    obj_add_object_type(self, OBJTYPE_FireFly);
    
    //Initialise spline
    {
        vZero.x = 0.0f;
        vZero.y = 0.0f; 
        vZero.z = 0.0f;
        
        objData->splineX[0] = vZero.f[0];
        objData->splineY[0] = vZero.f[1];
        objData->splineZ[0] = vZero.f[2];
        
        objData->splineX[1] = vZero.f[0];
        objData->splineY[1] = vZero.f[1];
        objData->splineZ[1] = vZero.f[2];
        
        objData->splineX[2] = vZero.f[0];
        objData->splineY[2] = vZero.f[1];
        objData->splineZ[2] = vZero.f[2];
        
        objData->splineX[3] = vZero.f[0];
        objData->splineY[3] = vZero.f[1];
        objData->splineZ[3] = vZero.f[2];
    }

    objData->lfxEmitter = NULL;
    objData->lightCreated = FALSE;

    objData->tValueSpeed = 0.08f;
    objData->unk48 = 0.0275f;
    objData->tValueSpline = 1.0f;
    objData->unk70 = 0;
    objData->unk6F = 0;
    objData->unk6A = rand_next(500, 1500);
    objData->randomYaw = rand_next(0, 65000);
    objData->varianceY = 4;
    objData->effectType = objSetup->effectType;
    objData->unk4C = 0.0f;
    objData->unk50 = 5;

    objData->home.x = objSetup->base.x;
    objData->home.y = objSetup->base.y;
    objData->home.z = objSetup->base.z;

    objData->unk73 = 0;
    objData->flagLightPerFirefly = 0;
}

// offset: 0x190 | func: 1 | export: 1
void LanternFireFly_control(Object* self) {
    LanternFireFly_Data* objData;
    Object* player;
    LFXEmitter_Setup* lfxSetup;
    s32 i;
    Vec3f delta;
    s32 volume;
    LFXEmitter_Data* lfxData;
    s32 numPart;
    SRT fxTransform;
    int True = TRUE;

    objData = self->data;
    player = get_player();
    
    self->prevLocalPosition.x = self->srt.transl.x;\
    self->prevLocalPosition.y = self->srt.transl.y;\
    self->prevLocalPosition.z = self->srt.transl.z;
    
    if (objData->tValueSpline > 1.0f) {
        objData->tValueSpline--;
        if (objData->unk70 >= 4) {
            if (objData->unk70 != 7) {
                objData->unk70++;
            } else {
                objData->unk70 = 0;
            }
        } else {
            LanternFireFly_func_AB0(self);
        }
        LanternFireFly_set_spline_point(self);
    }
    
    self->srt.transl.x = objData->home.x + curves_b_spline(objData->splineX, objData->tValueSpline, NULL);
    self->srt.transl.y = objData->home.y + curves_b_spline(objData->splineY, objData->tValueSpline, NULL);
    self->srt.transl.z = objData->home.z + curves_b_spline(objData->splineZ, objData->tValueSpline, NULL);
    
    if ((objData->flagLightPerFirefly) == True) {
        objData->tValueSpeed = (vec3_distance(&self->globalPosition, &get_player()->globalPosition) * 0.0015f) + 0.0001f;
    }
    objData->tValueSpline += (objData->tValueSpeed * gUpdateRateF);

    if ((objData->lfxEmitter) && (objData->lfxEmitter->stateFlags & OBJSTATE_DESTROYED)) {
        objData->lightCreated = FALSE;
        objData->lfxEmitter = NULL;
    }
    
    if ((objData->effectType == 1) || (objData->effectType == 4)) {
        if ((((objData->flagLightPerFirefly) == True) && (objData->lightCreated == FALSE)) ||  //Create LFXEmitter per firefly
            (dLightCreated == FALSE)                                                           //Or create just one LFXEmitter for the DLL, tracked via a static var
        ) {
            objData->lightCreated = TRUE;

            STUBBED_PRINTF(" Creating Light ");

            lfxSetup = obj_alloc_setup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
            lfxSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
            lfxSetup->base.x = self->srt.transl.x;
            lfxSetup->base.y = self->srt.transl.y;
            lfxSetup->base.z = self->srt.transl.z;
            lfxSetup->unk1E = rand_next(0, 1) + 0x1AA;
            lfxSetup->unk22 = -1;
            objData->lfxEmitter = obj_create((ObjSetup*)lfxSetup, 5, self->mapID, -1, self->parent);
            
            if ((objData->flagLightPerFirefly) != True) {
                dLightCreated = TRUE;
            }
        }
        
        if (objData->lfxEmitter != NULL) {
            objData->lfxEmitter->srt.transl.x = self->srt.transl.x;
            objData->lfxEmitter->srt.transl.y = self->srt.transl.y;
            objData->lfxEmitter->srt.transl.z = self->srt.transl.z;
        }
    }
    
    delta.x = self->srt.transl.x - self->prevLocalPosition.x;
    delta.y = self->srt.transl.y - self->prevLocalPosition.y;
    delta.z = self->srt.transl.z - self->prevLocalPosition.z;

    numPart = (s32) (VECTOR_MAGNITUDE(delta) / 2.5f) + 1;
    delta.x *= 1.0f / numPart;
    delta.y *= 1.0f / numPart;
    delta.z *= 1.0f / numPart;
    
    fxTransform.yaw = 0;
    fxTransform.pitch = 0;
    fxTransform.roll = 0;
    fxTransform.scale = 1.0f;
    fxTransform.transl.x = self->prevLocalPosition.x;
    fxTransform.transl.y = self->prevLocalPosition.y;
    fxTransform.transl.z = self->prevLocalPosition.z;

    if (objData->soundHandle != 0) {
        volume = numPart * 30;
        volume = (volume < 20) ? 20 : 
                ((volume > MAX_VOLUME) ? MAX_VOLUME : volume);
        
        gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandle, volume);
    }
    
    diPrintf(" Num Part %i ", numPart);   

    if ((objData->flagLightPerFirefly) == True) {
        if ((objData->effectType == 1) || (objData->effectType == 4)) {
            for (i = 0; i < numPart; i++) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1A0, &fxTransform, 0x200001, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_19E, &fxTransform, 0x200001, -1, NULL);
                fxTransform.transl.x = delta.x + fxTransform.transl.x;
                fxTransform.transl.y = delta.y + fxTransform.transl.y;
                fxTransform.transl.z = delta.z + fxTransform.transl.z;
            }
        } else {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1BB, NULL, 1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_1BD, NULL, 1, -1, NULL);
        }
        
        objData->lifetime -= gUpdateRate;
        if (objData->lifetime < 0) {
            main_decrement_bits(BIT_698);
            obj_destroy_object(self);
        }
        
        LanternFireFly_set_home(self, player->globalPosition.x, player->globalPosition.y + 34.0f, player->globalPosition.z);

        if (objData->lfxEmitter != NULL) {
            if (objData->lifetime < 180) {
                lfxData = objData->lfxEmitter->data;
                gDLL_11_Newlfx->vtbl->func6(lfxData->unk108->unk10, 0, fsin16_precise((objData->lifetime << 11)) * objData->lifetime);
            }
        }
    }
}

const char rodata_str[] = " Sending FireFly ";

// offset: 0x84C | func: 2 | export: 2
void LanternFireFly_update(Object *self) { }

// offset: 0x858 | func: 3 | export: 3
void LanternFireFly_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x8AC | func: 4 | export: 4
void LanternFireFly_free(Object* self, s32 onlySelf) {
    LanternFireFly_Data* objData = self->data;
    
    if (onlySelf == FALSE) {
        if (objData->lfxEmitter != NULL) {
            obj_destroy_object(objData->lfxEmitter);
            objData->lfxEmitter = NULL;

            if ((objData->flagLightPerFirefly) != TRUE) {
                dLightCreated = FALSE;
            }
        }
    }
    
    obj_free_object_type(self, OBJTYPE_FireFly);
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x960 | func: 5 | export: 5
u32 LanternFireFly_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x970 | func: 6 | export: 6
u32 LanternFireFly_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(LanternFireFly_Data);
}

// offset: 0x984 | func: 7
void LanternFireFly_set_spline_point(Object* self) {
    LanternFireFly_Data* objData = self->data;
    
    objData->splineX[0] = objData->splineX[1];
    objData->splineY[0] = objData->splineY[1];
    objData->splineZ[0] = objData->splineZ[1];
    
    objData->splineX[1] = objData->splineX[2];
    objData->splineY[1] = objData->splineY[2];
    objData->splineZ[1] = objData->splineZ[2];
    
    objData->splineX[2] = objData->splineX[3];
    objData->splineY[2] = objData->splineY[3];
    objData->splineZ[2] = objData->splineZ[3];
    
    if ((objData->flagLightPerFirefly) == TRUE) {
        objData->tValueSpeed = (vec3_distance(&self->globalPosition, &get_player()->globalPosition) * 0.0015f) + 0.0001f;
    } else {
        objData->tValueSpeed = rand_next(60, 90) * 0.0015f;
    }
    
    objData->splineX[3] = objData->homeRelativeToPlayer.x;
    objData->splineY[3] = objData->homeRelativeToPlayer.y;
    objData->splineZ[3] = objData->homeRelativeToPlayer.z;
}

// offset: 0xAB0 | func: 8
void LanternFireFly_func_AB0(Object* self) {
    LanternFireFly_Data* objData;
    SRT transform;

    objData = self->data;
    
    objData->homeRelativeToPlayer.x = 0.0f;
    objData->homeRelativeToPlayer.y = rand_next(-objData->varianceY, objData->varianceY);
    if (objData->unk50 < 21.0f) {
        objData->homeRelativeToPlayer.z = 0.0f; //@bug? should this be 21, given `rand_next(/*min*/ 20, /*max*/ objData->unk50)`?
    } else {
        objData->homeRelativeToPlayer.z = objData->unk50 - rand_next(20, (s16)objData->unk50);
    }

    objData->randomYaw += (s16)rand_next(3000, 5000);
    
    transform.transl.x = 0;
    transform.transl.y = 0;
    transform.transl.z = 0;
    transform.scale = 1;
    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = objData->randomYaw;
    rotate_vec3(&transform, objData->homeRelativeToPlayer.f);
}

// offset: 0xBE0 | func: 9 | export: 7
void LanternFireFly_func_BE0(Object* self, Vec3f* coordsPlayer) {
    LanternFireFly_Data* objData = self->data;
    
    coordsPlayer->x -= objData->home.x;
    coordsPlayer->y -= objData->home.y;
    coordsPlayer->z -= objData->home.z;
    
    objData->homeRelativeToPlayer.x = coordsPlayer->x;
    objData->homeRelativeToPlayer.y = coordsPlayer->y;
    objData->homeRelativeToPlayer.z = coordsPlayer->z;

    objData->unk70 = 4;
}

// offset: 0xC3C | func: 10 | export: 8
void LanternFireFly_func_C3C(Object* self) {
    LanternFireFly_Data* objData;
    LanternFireFly_Setup* objSetup;
    Object* player;
    Vec3f coordsPlayer;

    objSetup = (LanternFireFly_Setup*)self->setup;
    objData = self->data;
    
    objData->varianceY = objSetup->varianceY;
    objData->effectType = objSetup->effectType;
    objData->unk4C = 1.0f;
    objData->unk50 = objSetup->unk1C;
    objData->unk73 = 0;
    func_8005B5B8(self, NULL, 1);
    
    player = get_player();
    coordsPlayer.x = player->globalPosition.x;
    coordsPlayer.y = player->globalPosition.y;
    coordsPlayer.z = player->globalPosition.z;
    coordsPlayer.y += 40.0f;

    LanternFireFly_set_home(self, player->globalPosition.x, player->globalPosition.y + 34.0f, player->globalPosition.z);
    
    LanternFireFly_func_BE0(self, &coordsPlayer);
    
    LanternFireFly_set_spline_point(self);
    LanternFireFly_set_spline_point(self);
    LanternFireFly_set_spline_point(self);
    LanternFireFly_set_spline_point(self);
    LanternFireFly_set_spline_point(self);
    LanternFireFly_set_spline_point(self);
    
    objData->flagLightPerFirefly = 1;

    if (objData->soundHandle == 0) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_7F6_Firefly_Twinkle_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
    }
    objData->lifetime = objSetup->lifetime;
    
    main_increment_bits(BIT_698);
}

// offset: 0xE54 | func: 11 | export: 9
void LanternFireFly_set_home(Object* self, f32 x, f32 y, f32 z) {
    LanternFireFly_Data* objData = self->data;

    objData->home.x = x;
    objData->home.y = y;
    objData->home.z = z;
}
