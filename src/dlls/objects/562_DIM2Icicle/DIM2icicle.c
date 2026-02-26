#include "PR/ultratypes.h"
#include "dll.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "functions.h"
#include "sys/rand.h"

typedef struct {
    f32 groundHeight;
    u32 unused4;
    s16 pitch;
    u8 state;
    u8 groundFound;
    s16 soundTimer;
} DIM2Icicle_Data;

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 used19[5];
    s16 gamebitFell;
} DIM2Icicle_Setup;

typedef enum {
    DIM2Icicle_STATE_Dangling = 0,
    DIM2Icicle_STATE_Falling = 1,
    DIM2Icicle_STATE_Fall_Finished = 2,
    DIM2Icicle_STATE_Damaged = 3
} DIM2Icicle_States;

// offset: 0x0 | ctor
void DIM2Icicle_ctor(void *dll) { }

// offset: 0xC | dtor
void DIM2Icicle_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIM2Icicle_setup(Object* self, DIM2Icicle_Setup* setup, s32 arg2) {
    DIM2Icicle_Data* objdata = self->data;
    
    if (main_get_bits(setup->gamebitFell)) {
        objdata->state = DIM2Icicle_STATE_Fall_Finished;
        self->opacity = 0;
    } else {
        objdata->state = DIM2Icicle_STATE_Dangling;
        self->opacity = OBJECT_OPACITY_MAX;
    }
    
    self->srt.yaw = setup->yaw << 8;
    self->unkB0 |= 0x2000;
    self->speed.y = 0.0f;
}

// offset: 0xBC | func: 1 | export: 1
void DIM2Icicle_control(Object* self) {
    s32 pad;
    s32 opacity;
    DIM2Icicle_Data* objData;
    DIM2Icicle_Setup* objSetup;
    s32 count;
    s32 i;
    Func_80057F1C_Struct** collisionInfo;

    objSetup = (DIM2Icicle_Setup*)self->setup;
    objData = self->data;
    
    switch (objData->state) {
    case DIM2Icicle_STATE_Dangling:
        //React to Projectile Spell collisions
        if (func_80025F40(self, NULL, NULL, NULL) == 15) {
            objData->pitch = rand_next(800, 1200);
            objData->state = DIM2Icicle_STATE_Damaged;
            self->objhitInfo->unk58 &= ~1;
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_3D6, MAX_VOLUME, NULL, NULL, 0, NULL);
            return;
        }
        return;
    case DIM2Icicle_STATE_Damaged:
        //Shake
        self->srt.pitch = objData->pitch;
        objData->pitch *= 0.333f;
        if (self->srt.pitch < 10) {
            self->srt.pitch = 0;
            objData->state = DIM2Icicle_STATE_Falling;
            objData->soundTimer = 60;
            return;
        }
        break;
    case DIM2Icicle_STATE_Falling:
        //Find ground
        if (objData->groundFound == FALSE) {
            count = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &collisionInfo, 0, 0);
            for (i = 0, objData->groundHeight = -100000.0f; i < count; i++){
                if (collisionInfo[i]->unk14 == 14) {
                    objData->groundHeight = collisionInfo[i]->unk0[0];

                    //Index break
                    i = count;
                }
            }
            
            if (objData->groundHeight != -100000.0f) {
                objData->groundFound = TRUE;
            }
        }

        //Play whoosh sound after delay
        if (objData->soundTimer > 0) {
            objData->soundTimer -= gUpdateRate;
            if (objData->soundTimer <= 0) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_3D7_Whoosh, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        }

        //Fall
        self->speed.y -= 0.1f * gUpdateRateF;
        if (self->speed.y < -10.0f) {
            self->speed.y = -10.0f;
        }
        self->srt.transl.y += self->speed.y * gUpdateRateF;

        //Check for when icicle reaches ground
        if (self->srt.transl.y < objData->groundHeight) {
            main_set_bits(objSetup->gamebitFell, 1);
            objData->state = DIM2Icicle_STATE_Fall_Finished;
            gDLL_24_Waterfx->vtbl->func_174C(self->srt.transl.x, objData->groundHeight, self->srt.transl.z, 10.0f);
            gDLL_24_Waterfx->vtbl->func_1CC8(self->srt.transl.x, objData->groundHeight, self->srt.transl.z, 0, 0.0f, 2);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_3D8_Water_Splash, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->soundTimer = 150;
            return;
        }
        break;
    default:
        //Fall finished

        //Play freezing sound
        if (objData->soundTimer > 0) {
            objData->soundTimer -= gUpdateRate;
            if (objData->soundTimer <= 0) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_3E3_Ice_Cracking, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        }
        
        //Fade out
        opacity = self->opacity - (gUpdateRate * 8);
        if (opacity < 0) {
            opacity = 0;
            self->srt.transl.y = objSetup->base.y;
            self->speed.y = 0.0f;
        }
        self->opacity = opacity;

        //Continue fall during fadeout
        self->srt.transl.y += self->speed.y * gUpdateRateF;
        break;
    }
}

// offset: 0x538 | func: 2 | export: 2
void DIM2Icicle_update(Object *self) { }

// offset: 0x544 | func: 3 | export: 3
void DIM2Icicle_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x598 | func: 4 | export: 4
void DIM2Icicle_free(Object *self, s32 a1) { }

// offset: 0x5A8 | func: 5 | export: 5
u32 DIM2Icicle_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x5B8 | func: 6 | export: 6
u32 DIM2Icicle_get_data_size(Object* self, u32 a1) {
    return sizeof(DIM2Icicle_Data);
}
