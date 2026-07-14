#include "common.h"
#include "sys/objhits.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 gamebitA;           //GamebitID for defeating a nearby Pollen Cannon
    s16 gamebitB;           //GamebitID for defeating a nearby Pollen Cannon
    s16 gamebitHurtPlayer;  //Vine bounces and hurts the player when this is set (by TriggerPlane)
} WGBouncyVine_Setup;

typedef struct {
    u8 state;
    u8 soundPlayed;
} WGBouncyVine_Data;

typedef enum {
    WGBouncyVine_STATE_0_Idle,
    WGBouncyVine_STATE_1_Bounce,
    WGBouncyVine_STATE_2_Collapsing,
    WGBouncyVine_STATE_3_Gone
} WGBouncyVine_States;

typedef enum {
    WGBouncyVine_MODANIM_0_Idle,
    WGBouncyVine_MODANIM_1_Collapsing,
    WGBouncyVine_MODANIM_2_Bounce
} WGBouncyVine_ModAnim_Indices;

// offset: 0x0 | ctor
void WGBouncyVine_ctor(void *dll) { }

// offset: 0xC | dtor
void WGBouncyVine_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WGBouncyVine_setup(Object* self, WGBouncyVine_Setup* objSetup, s32 reset) {
    WGBouncyVine_Data* objData;
    s8 gamebitValueA;
    s8 gamebitValueB;

    objData = self->data;
    self->srt.yaw = objSetup->yaw << 8;

    //Check whether the pollen cannons' gamebits are set
    gamebitValueA = -1;
    gamebitValueB = -1;
    if (objSetup->gamebitA != NO_GAMEBIT) {
        gamebitValueA = mainGetBits(objSetup->gamebitA);
    }
    if (objSetup->gamebitB != NO_GAMEBIT) {
        gamebitValueB = mainGetBits(objSetup->gamebitA); //@bug: shouldn't this be checking gamebitB?
    }
    
    //Remove the vine when both gamebits' values are set
    if ((gamebitValueA * gamebitValueB) && ((gamebitValueA + gamebitValueB) >= 0)) {
        objData->state = WGBouncyVine_STATE_3_Gone;
        self->objhitInfo->unk58 &= ~1;
    }
}

// offset: 0x114 | func: 1 | export: 1
void WGBouncyVine_control(Object* self) {
    WGBouncyVine_Setup* objSetup;
    WGBouncyVine_Data* objData;
    s32 pad;
    UnkFunc_80024108Struct animInfo;
    s8 gamebitValueA;
    s8 gamebitValueB;

    objSetup = (WGBouncyVine_Setup*)self->setup;
    objData = self->data;

    gamebitValueA = -1;
    gamebitValueB = -1;
    animInfo.unk1B = 0;

    switch (objData->state) {
    case WGBouncyVine_STATE_0_Idle:
        //Bounce and hurt the player when a gamebit is set
        if (mainGetBits(objSetup->gamebitHurtPlayer)) {
            objAnimSet(self, WGBouncyVine_MODANIM_2_Bounce, 0.0f, 0);
            objData->state = WGBouncyVine_STATE_1_Bounce;
            func_8002635C(objGetPlayer(), NULL, Damage_Type_8, 1, 0);
        }

        //Check whether the pollen cannons' gamebits are set
        if (objSetup->gamebitA != NO_GAMEBIT) {
            gamebitValueA = mainGetBits(objSetup->gamebitA);
        }
        if (objSetup->gamebitB != NO_GAMEBIT) {
            gamebitValueB = mainGetBits(objSetup->gamebitA); //@bug: shouldn't this be checking gamebitB?
        }
        
        //Remove the vine when both gamebits' values are set
        if ((gamebitValueA * gamebitValueB) && ((gamebitValueA + gamebitValueB) >= 0)) {
            self->objhitInfo->unk58 &= ~1;
            gDLL_6_AMSFX->vtbl->play(self, SOUND_A3E_Vines_Rustle, MAX_VOLUME, NULL, NULL, 0, NULL);
            objAnimSet(self, WGBouncyVine_MODANIM_1_Collapsing, 0.0f, 0);
            objData->state = WGBouncyVine_STATE_2_Collapsing;
        }
        break;
    case WGBouncyVine_STATE_3_Gone:
        break;
    case WGBouncyVine_STATE_1_Bounce:
        //Play a sound partway through animation
        if ((objData->soundPlayed == FALSE) && (self->animProgress > 0.3f)) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_A3F_Boing, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->soundPlayed = TRUE;
        }
        
        //Return to idle when animation finished
        if (objAnimAdvance(self, 0.02f, gUpdateRateF, &animInfo)) {
            mainSetBits(objSetup->gamebitHurtPlayer, FALSE);
            objData->state = WGBouncyVine_STATE_0_Idle;
            objData->soundPlayed = FALSE;
        }
        break;
    case WGBouncyVine_STATE_2_Collapsing:
        //Disappear when animation finished
        if (objAnimAdvance(self, 0.01f, gUpdateRateF, &animInfo)) {
            objData->state = WGBouncyVine_STATE_3_Gone;
        }
        break;
    }
}

// offset: 0x3F0 | func: 2 | export: 2
void WGBouncyVine_update(Object *self) { }

// offset: 0x3FC | func: 3 | export: 3
void WGBouncyVine_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    WGBouncyVine_Data* objData = self->data;
    
    if ((objData->state != WGBouncyVine_STATE_3_Gone) && visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x460 | func: 4 | export: 4
void WGBouncyVine_free(Object *self, s32 onlySelf) { }

// offset: 0x470 | func: 5 | export: 5
u32 WGBouncyVine_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x480 | func: 6 | export: 6
u32 WGBouncyVine_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(WGBouncyVine_Data);
}
