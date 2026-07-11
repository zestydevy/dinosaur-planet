#include "common.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 timerThreshold;
    s16 scale;
    s16 gamebitRestoreState;
    s16 gamebitDamaged;
} DBWaterPlant_Setup;

typedef struct {
    u8 state;
    s16 animSpeedPhase;
    s32 timer;
} DBWaterPlant_Data;

typedef enum {
    DBWaterPlant_STATE_0,
    DBWaterPlant_STATE_1,
    DBWaterPlant_STATE_2,
    DBWaterPlant_STATE_3,
    DBWaterPlant_STATE_4
} DBWaterPlant_States;

// offset: 0x0 | ctor
void DBWaterPlant_ctor(void* dll) { }

// offset: 0xC | dtor
void DBWaterPlant_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBWaterPlant_setup(Object* self, DBWaterPlant_Setup* objSetup, s32 reset) {
    DBWaterPlant_Data* objData;
    TextureAnimator* texAnim;

    objData = self->data;
    
    texAnim = func_800348A0(self, 0, 0);
    if (texAnim != NULL) {
        texAnim->frame = 0;
    }

    if (main_get_bits(objSetup->gamebitRestoreState)) {
        objData->state = DBWaterPlant_STATE_3;
        func_80023D30(self, 0, 0.0f, 0);
    } else {
        objData->state = DBWaterPlant_STATE_0;
        func_80023D30(self, 0, 1.0f, 0);
    }
    
    objData->timer = 0;
    self->srt.scale = objSetup->scale * ONE_SIXTY_FOURTH_F * self->def->scale;
}

// offset: 0x120 | func: 1 | export: 1
void DBWaterPlant_control(Object* self) {
    DBWaterPlant_Data* objData;
    TextureAnimator* texAnim;
    DBWaterPlant_Setup* objSetup;
    f32 animSpeed;

    objData = self->data;
    objSetup = (DBWaterPlant_Setup*)self->setup;
    
    texAnim = func_800348A0(self, 0, 0);
    if (texAnim != NULL) {
        texAnim->positionU += 0x20;
        if (texAnim->positionU > 0x3E0) {
            texAnim->positionU = 0;
        }

        texAnim->positionV += 0x20;
        if (texAnim->positionV > 0x3E0) {
            texAnim->positionV = 0;
        }
    }

    switch (objData->state) {
        case DBWaterPlant_STATE_3:
            objData->timer += gUpdateRate;
            if (objData->timer >= objSetup->timerThreshold) {
                objData->state = DBWaterPlant_STATE_1;
            }
            /* fallthrough */
        case DBWaterPlant_STATE_0:
            //Vary animSpeed sinusoidally
            animSpeed = fsin16_precise(objData->animSpeedPhase) * 0.005f;
            break;
        case DBWaterPlant_STATE_2:
            animSpeed = -(self->animProgress * 0.01f);
            if (self->animProgress < 0.25f) {
                objData->state = DBWaterPlant_STATE_3;
            }
            break;
        case DBWaterPlant_STATE_1:
            animSpeed = (1.0f - self->animProgress) * 0.005f;
            if (self->animProgress > 0.95f) {
                objData->state = DBWaterPlant_STATE_0;
            }
            break;
    }
    
    objData->animSpeedPhase += gUpdateRate << 9;
    
    func_80024108(self, animSpeed, gUpdateRate, NULL);
}

// offset: 0x310 | func: 2 | export: 2
void DBWaterPlant_update(Object* self) {
    s32 damageType;
    DBWaterPlant_Setup* objSetup;
    DBWaterPlant_Data* objData;

    objSetup = (DBWaterPlant_Setup*)self->setup;
    objData = self->data;

    damageType = func_80025F40(self, NULL, NULL, NULL);
    
    if (damageType && (objData->state < DBWaterPlant_STATE_2)) {
        main_set_bits(objSetup->gamebitRestoreState, 1);
        main_set_bits(objSetup->gamebitDamaged, 1);
        objData->timer = 0;
        objData->state = DBWaterPlant_STATE_2;
    }
}

// offset: 0x3BC | func: 3 | export: 3
void DBWaterPlant_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x410 | func: 4 | export: 4
void DBWaterPlant_free(Object* self, s32 onlySelf) { }

// offset: 0x420 | func: 5 | export: 5
u32 DBWaterPlant_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x430 | func: 6 | export: 6
u32 DBWaterPlant_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DBWaterPlant_Data);
}
