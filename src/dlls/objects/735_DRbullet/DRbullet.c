#include "common.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "macros.h"
#include "sys/gfx/model.h"

typedef struct {
    ObjSetup base;
    u8 speedX;
    u8 speedY;
    u8 speedZ;
} DRbullet_Setup;

typedef struct {
    Object* lfxEmitter;
    u32 whooshSoundHandle;  //Controls whooshing sound as bullet whistles past player
    u8 state;
    u8 flags;
    s32 timer;
} DRbullet_Data;

typedef enum {
    BULLET_STATE_INACTIVE = 0,
    BULLET_STATE_IMPACT = 1,
    BULLET_STATE_FIRED = 2
} DRbullet_States;

/*0x0*/ static u32 _data_0 = 0; //unused

static s32 DRbullet_tick_impact(Object* self);
static Object* DRbullet_create_lfxEmitter(Object* self, s32 arg1);

// offset: 0x0 | ctor
void DRbullet_ctor(void *dll) { }

// offset: 0xC | dtor
void DRbullet_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DRbullet_setup(Object* self, DRbullet_Setup* objSetup, s32 arg2) {
    ObjectStruct64* shadowData;
    DRbullet_Data* objData;

    objData = self->data;

    self->objhitInfo->unk5F = 0x12;
    self->objhitInfo->unk60 = 1;
    self->objhitInfo->unk58 &= 0xFFFE;

    self->srt.transl.x = objSetup->base.x;
    self->srt.transl.y = objSetup->base.y;
    self->srt.transl.z = objSetup->base.z;
    self->speed.x = objSetup->speedX;   
    self->speed.y = objSetup->speedY;
    self->speed.z = objSetup->speedZ;

    shadowData = self->unk64;
    if (shadowData != NULL) {
        shadowData->flags |= 0xA10;
    }

    objData->state = BULLET_STATE_INACTIVE;
    objData->flags = 0;
    objData->timer = 0;
}

// offset: 0x104 | func: 1 | export: 1
void DRbullet_control(Object* self) {
    DRbullet_Data* objData;

    objData = self->data;

    switch (objData->state){
        case BULLET_STATE_FIRED:
            obj_integrate_speed(self, self->speed.x * gUpdateRateF, self->speed.y * gUpdateRateF, self->speed.z * gUpdateRateF);
            objData->timer -= gUpdateRate;
            //Check if the bullet hit something or its timer expired
            if ((objData->timer < 0) || (self->objhitInfo->unk58 & 8)) {
                objData->state = BULLET_STATE_IMPACT;
                objData->timer = 0;
                if (self->objhitInfo->unk58 & 8) {
                    STUBBED_PRINTF(" hit sometime ");
                    gDLL_6_AMSFX->vtbl->play_sound(self, 0x851, 0x7F, NULL, NULL, 0, NULL);
                }
                if (0 && objData->timer == 0){
                    STUBBED_PRINTF(" long time ");
                }
            }
            self->objhitInfo->unk44 = 0x10;
            self->objhitInfo->unk40 = 0x10;
            break;
        case BULLET_STATE_IMPACT:
            //Wait for impact behaviour to play out, then await recycling
            if (DRbullet_tick_impact(self)) {
                func_800267A4(self);
                objData->state = BULLET_STATE_INACTIVE;
            }
            break;
        case BULLET_STATE_INACTIVE:
            //Bullets can be recycled while in this state (via exports)
            STUBBED_PRINTF(" Kill Me ");
            break;
    }

    //Control whooshing sound volume as bullet passes player
    if (objData->whooshSoundHandle) {
        gDLL_6_AMSFX->vtbl->func_1504(objData->whooshSoundHandle, self, get_player(), 150.0f);
    }
}

// offset: 0x2E0 | func: 2 | export: 2
void DRbullet_update(Object *self) { }

// offset: 0x2EC | func: 3 | export: 3
void DRbullet_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DRbullet_Data* objData = self->data;
    if (visibility && (objData->state != BULLET_STATE_INACTIVE)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x350 | func: 4 | export: 4
void DRbullet_free(Object* self, s32 arg1) {
    DRbullet_Data* objData = self->data;
    Object* lfxEmitter;

    lfxEmitter = objData->lfxEmitter;
    if (lfxEmitter) {
        obj_destroy_object(lfxEmitter);
    }
}

// offset: 0x3A0 | func: 5 | export: 5
u32 DRbullet_get_model_flags(s32 arg0) {
    return MODFLAGS_SHADOW;
}

// offset: 0x3B0 | func: 6 | export: 6
u32 DRbullet_get_data_size(Object *self, u32 a1) {
    return sizeof(DRbullet_Data);
}

// offset: 0x3C4 | func: 7 | export: 7
s32 DRbullet_check_if_recycleable(Object* self) {
    DRbullet_Data* objData = self->data;
    return objData->state == BULLET_STATE_INACTIVE;
}

// offset: 0x3DC | func: 8 | export: 8
void DRbullet_recycle(Object* self, SRT* pFired, SRT* pTarget, f32 speed) {
    DRbullet_Data *objData;
    Object *lfxEmitter;
    f32 lateralSpeed;
    f32 pad;
    Vec3f velocity;
    Vec3f displacement;
    Vec3f futurePosition;
    Vec3s16 sCurrentPosition;
    Vec3s16 sFuturePosition;
    Vec3s16 sp44;
    f32 magnitudeOverSpeed;

    objData = self->data;

    //Get unit vector from point of origin to target point (and then multiply it by speed)
    velocity.x = pTarget->transl.x - pFired->transl.x;
    velocity.y = pTarget->transl.y - pFired->transl.y;
    velocity.z = pTarget->transl.z - pFired->transl.z;
    magnitudeOverSpeed = sqrtf(((velocity.f[0] * velocity.f[0]) + (velocity.f[1] * velocity.f[1])) + (velocity.f[2] * velocity.f[2])) / speed;
    if (magnitudeOverSpeed != 0.0f){
        velocity.f[0] /= magnitudeOverSpeed;
        velocity.f[1] /= magnitudeOverSpeed;
        velocity.f[2] /= magnitudeOverSpeed;
    }

    //Set bullet's transform
    self->srt.transl.x = pFired->transl.x;
    self->srt.transl.y = pFired->transl.y;
    self->srt.transl.z = pFired->transl.z;
    self->speed.x = velocity.f[0];
    self->speed.y = velocity.f[1];
    self->speed.z = velocity.f[2];    
    lateralSpeed = sqrtf((self->speed.x * self->speed.x) + (self->speed.z * self->speed.z));
    self->srt.yaw = arctan2_f(self->speed.x, self->speed.z);
    self->srt.pitch = -arctan2_f(self->speed.y, lateralSpeed);
    self->srt.roll = 0;
    func_8002674C(self);

    //Set bullet to "fired" state and play sound
    objData->state = BULLET_STATE_FIRED;
    gDLL_6_AMSFX->vtbl->play_sound(self, 0x927, 0x7F, &objData->whooshSoundHandle, NULL, 0, NULL);

    //Set bullet's expiry timer based on 10 second trajectory after being fired
    futurePosition.x = self->speed.x * 600.0f;
    futurePosition.y = self->speed.y * 600.0f;
    futurePosition.z = self->speed.z * 600.0f;
    futurePosition.x += self->srt.transl.x;
    futurePosition.y += self->srt.transl.y;
    futurePosition.z += self->srt.transl.z;
    func_80007EE0(&self->srt.transl, &sCurrentPosition);
    func_80007EE0(&futurePosition, &sFuturePosition);

    if (func_80008048(&sCurrentPosition, &sFuturePosition, &sp44, NULL, 0) == 0){
        func_80007E2C(&futurePosition, &sp44);
        displacement.f[0] = futurePosition.f[0] - self->srt.transl.f[0];
        displacement.f[1] = futurePosition.f[1] - self->srt.transl.f[1];
        displacement.f[2] = futurePosition.f[2] - self->srt.transl.f[2];
        objData->timer = sqrtf((displacement.f[0] * displacement.f[0]) + (displacement.f[1] * displacement.f[1]) + (displacement.f[2] * displacement.f[2])) / speed;
    } else {
        objData->timer = 600;
    }

    //Create new lfxEmitter
    lfxEmitter = objData->lfxEmitter;
    if (lfxEmitter != NULL){
        obj_destroy_object(lfxEmitter);
    }    
    lfxEmitter = DRbullet_create_lfxEmitter(self, 0x24B);
    objData->lfxEmitter = lfxEmitter;
    if (lfxEmitter != NULL){
        objData->lfxEmitter->speed.x = self->speed.x;
        objData->lfxEmitter->speed.y = self->speed.y;
        objData->lfxEmitter->speed.z = self->speed.z;
    }

    //Set bullet's opacity and scale
    self->opacity = 0xFF;
    self->srt.scale = self->def->scale;
}

// offset: 0x750 | func: 9 | export: 9
void DRbullet_free_if_inactive(Object* self) {
    DRbullet_Data* objData = self->data;

    objData->flags |= 1;
    if (objData->state == BULLET_STATE_INACTIVE) {
        obj_destroy_object(self);
    }
}

// offset: 0x7A0 | func: 10 | export: 10
void DRbullet_set_explode_state(Object* self) {
    DRbullet_Data* objData;

    objData = self->data;
    if (objData->state == BULLET_STATE_FIRED) {
        objData->state = BULLET_STATE_IMPACT;
    }
}

// offset: 0x7C4 | func: 11
s32 DRbullet_tick_impact(Object* self) {
    Object* lfxEmitter;
    s32 opacity;
    s32 index;
    DRbullet_Data* objData;

    objData = self->data;

    //Explode scale
    self->srt.scale += 0.1f;

    //Create particles and play sounds when timer is 0
    if (objData->timer == 0) {
        for (index = 0; index != 0x12; index++){
            gDLL_17_partfx->vtbl->spawn(self, 0x340, NULL, 1, -1, NULL);
        }
        gDLL_17_partfx->vtbl->spawn(self, 0x685, NULL, 1, -1, NULL);

        //Stop whoosh sound
        if (objData->whooshSoundHandle) {
            gDLL_6_AMSFX->vtbl->func_A1C(objData->whooshSoundHandle);
            objData->whooshSoundHandle = 0;
        }

        func_800267A4(self);
        gDLL_6_AMSFX->vtbl->play_sound(self, 0x380, 0x7F, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play_sound(self, 0x386, 0x7F, NULL, NULL, 0, NULL);
    }

    //Increment timer, and return True if bullet has been in impact state for a while
    objData->timer += gUpdateRate;
    if (objData->timer > 128) {
        return TRUE;
    }

    //Handle opacity fade-out and expiry
    opacity = 0xFF - (s32) (((f32) objData->timer / 64) * 255.0f);
    if (opacity >= 0) {
        self->opacity = opacity;

        lfxEmitter = objData->lfxEmitter;
        if (lfxEmitter != NULL) {
            //TO-DO: use proper lfxEmitter DLL interface
            ((DLL_Unknown*)lfxEmitter->dll)->vtbl->func[8].withOneS32OneF32((s32)lfxEmitter, opacity);
        }
    } else {
        if (objData->lfxEmitter != NULL) {
            obj_destroy_object(objData->lfxEmitter);
            objData->lfxEmitter = NULL;
        }
        self->opacity = 0;
    }

    return FALSE;
}

// offset: 0xA1C | func: 12
Object* DRbullet_create_lfxEmitter(Object* self, s32 arg1) {
    LFXEmitter_Setup* lfxSetup;

    lfxSetup = obj_alloc_create_info(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
    lfxSetup->base.loadFlags = 2;
    lfxSetup->base.x = self->srt.transl.x;
    lfxSetup->base.y = self->srt.transl.y;
    lfxSetup->base.z = self->srt.transl.z;
    lfxSetup->unk1E = arg1;
    lfxSetup->unk22 = -1;
    return obj_create((ObjSetup*)lfxSetup, 5, self->mapID, -1, self->parent);
}
