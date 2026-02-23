#include "common.h"

#include "dlls/objects/210_player.h"

typedef struct {
    ObjSetup base;
} ColdWaterControl_Setup;

typedef struct {
    f32 timer;
    Object* player;
    s32 hurtCounter;
} ColdWaterControl_Data;

#define COLDWATER_INITIAL -30.0f
#define COLDWATER_THRESHOLD 240.0f

// offset: 0x0 | ctor
void ColdWaterControl_ctor(void *dll) { }

// offset: 0xC | dtor
void ColdWaterControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void ColdWaterControl_setup(Object* self, ColdWaterControl_Setup* arg1, s32 arg2) {
    ColdWaterControl_Data* objData = self->data;
    objData->timer = COLDWATER_INITIAL;
    self->unkB0 |= 0x2000;
}

// offset: 0x54 | func: 1 | export: 1
void ColdWaterControl_control(Object* self) {
    ColdWaterControl_Data* objData;
    Object* player;

    objData = self->data;
    player = objData->player;
    
    if (player != NULL) {
        if (((DLL_210_Player*)player->dll)->vtbl->func43(player)) {
            //Player is swimming

            //Reset objHits interaction when timer is at its initial value
            if (objData->timer == COLDWATER_INITIAL) {
                func_8002635C(objData->player, self, 0x1C, 0, 1);
            }
            
            //Increment timer, and hurt the player after passing timer threshold
            objData->timer += gUpdateRateF;
            if (objData->timer > COLDWATER_THRESHOLD) {
                func_8002635C(objData->player, self, 0x1C, 1, 1);
                objData->hurtCounter++;
                objData->timer -= COLDWATER_THRESHOLD;
            }
        } else if (objData->timer != COLDWATER_INITIAL) {
            //Player out of water and timer not at initial value

            //Reset objHits interaction and hurt timer/counter
            if (objData->hurtCounter) {
                func_8002635C(objData->player, self, 0x1C, 0x7F, 1);
                objData->hurtCounter = 0;
            }
            objData->timer = COLDWATER_INITIAL;
        }
    } else {
        objData->player = get_player();
    }
}

// offset: 0x1E0 | func: 2 | export: 2
void ColdWaterControl_update(Object *self) { }

// offset: 0x1EC | func: 3 | export: 3
void ColdWaterControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x204 | func: 4 | export: 4
void ColdWaterControl_free(Object *self, s32 a1) { }

// offset: 0x214 | func: 5 | export: 5
u32 ColdWaterControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x224 | func: 6 | export: 6
u32 ColdWaterControl_get_data_size(Object *self, u32 a1){
    return sizeof(ColdWaterControl_Data);
}
