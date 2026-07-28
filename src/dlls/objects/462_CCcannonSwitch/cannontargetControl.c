#include "common.h"
#include "game/objects/interaction_arrow.h"
#include "macros.h"
#include "sys/objlib.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 yaw;
} CCCannonSwitch_Setup;

typedef struct {
    u8 state;
} CCCannonSwitch_Data;

typedef enum {
    CCCannonSwitch_STATE_0_Initial,
    CCCannonSwitch_STATE_1,
    CCCannonSwitch_STATE_2,
    CCCannonSwitch_STATE_3_Lever_Unlocked,
    CCCannonSwitch_STATE_4_Finished
} CCCannonSwitch_States;

// offset: 0x0 | ctor
void CCCannonSwitch_ctor(void* dll) { }

// offset: 0xC | dtor
void CCCannonSwitch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCCannonSwitch_obj_Setup(Object* self, CCCannonSwitch_Setup* objSetup, s32 reset) {
    CCCannonSwitch_Data* objData;

    objData = self->data;
    
    self->srt.yaw = objSetup->yaw << 8;
    
    mainSetBits(BIT_22, 0);
    mainSetBits(BIT_621, 0);
    
    if (mainGetBits(BIT_61F)) {
        objData->state = CCCannonSwitch_STATE_4_Finished;
    }
    
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
}


// offset: 0xC8 | func: 1 | export: 1
void CCCannonSwitch_obj_Control(Object* self) {
    CCCannonSwitch_Data* objData = self->data;

    STUBBED_PRINTF("obj mapno is %d\n", self->mapID);

    switch (objData->state) {
    case CCCannonSwitch_STATE_0_Initial:
        if (mainGetBits(BIT_22)) {
            //Unload CannonClaw group
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 17, 0);
            objData->state = CCCannonSwitch_STATE_1;
        }
        break;
    case CCCannonSwitch_STATE_1:
        if (mainGetBits(BIT_621)) {
            //Load CannonClaw group and set gamebits
            mainSetBits(BIT_61E, 1);
            mainSetBits(BIT_622, 0);
            mainSetBits(BIT_623, 0);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 17, 1);
            objData->state = CCCannonSwitch_STATE_2;
        }
        break;
    case CCCannonSwitch_STATE_2:
        if (mainGetBits(BIT_622) && mainGetBits(BIT_623)) {
            //Unlock lever
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            objData->state = CCCannonSwitch_STATE_3_Lever_Unlocked;
        }
        break;
    case CCCannonSwitch_STATE_3_Lever_Unlocked:
        //Play the level-pulling sequence when the player interacts (raising up the cannon)
        if (objCheckPlayerInteract(self)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            objData->state = CCCannonSwitch_STATE_4_Finished;
            mainSetBits(BIT_61F, 1);
        }
        break;
    case CCCannonSwitch_STATE_4_Finished:
        break;
    }
}

// offset: 0x2D4 | func: 2 | export: 2
void CCCannonSwitch_obj_Update(Object* self) { }

// offset: 0x2E0 | func: 3 | export: 3
void CCCannonSwitch_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0x334 | func: 4 | export: 4
void CCCannonSwitch_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x344 | func: 5 | export: 5
u32 CCCannonSwitch_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x354 | func: 6 | export: 6
u32 CCCannonSwitch_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CCCannonSwitch_Data);
}
