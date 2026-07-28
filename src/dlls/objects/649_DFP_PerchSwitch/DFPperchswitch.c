#include "common.h"
#include "game/objects/object.h"
#include "sys/objhits.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 unk1A;
    s16 unk1C;
    s16 gamebitLocked;
    s16 gamebitPulled;
} DFP_PerchSwitch_Setup;

typedef struct {
    u8 alreadyPulled;
    u8 startedLocked;
} DFP_PerchSwitch_Data;

static int DFP_PerchSwitch_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void DFP_PerchSwitch_ctor(void* dll) { }

// offset: 0xC | dtor
void DFP_PerchSwitch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFP_PerchSwitch_obj_Setup(Object* self, DFP_PerchSwitch_Setup* objSetup, s32 reset) {
    DFP_PerchSwitch_Data* objData;

    self->srt.yaw = objSetup->yaw << 8;
    if (self->objhitInfo != NULL) {
        self->objhitInfo->unk58 |= 1;
    }
    
    objData = self->data;
    objData->startedLocked = mainGetBits(objSetup->gamebitLocked);
    objData->alreadyPulled = mainGetBits(objSetup->gamebitPulled);
    
    self->animCallback = DFP_PerchSwitch_animCallback;
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
}

// offset: 0xCC | func: 1 | export: 1
void DFP_PerchSwitch_obj_Control(Object* self) {
    DFP_PerchSwitch_Setup* objSetup;
    s32 damageType;
    Object* hitBy;

    objSetup = (DFP_PerchSwitch_Setup*)self->setup;
    damageType = func_80025F40(self, &hitBy, NULL, NULL);

    //Pull the switch when hit by the Projectile Spell, or via a gamebit
    if (((damageType == Damage_Type_Projectile) || mainGetBits(BIT_DFP_Kyte_Activated_Perch)) && 
        (mainGetBits(objSetup->gamebitLocked) == FALSE)
    ) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        gDLL_6_AMSFX->vtbl->Play(self, SOUND_99F_Metal_Hinge_Squeak, MAX_VOLUME, NULL, NULL, 0, NULL);
        mainSetBits(objSetup->gamebitPulled, TRUE);
        mainSetBits(objSetup->gamebitLocked, TRUE);
    }
}

// offset: 0x1E8 | func: 2 | export: 2
void DFP_PerchSwitch_obj_Update(Object* self) { }

// offset: 0x1F4 | func: 3 | export: 3
void DFP_PerchSwitch_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x20C | func: 4 | export: 4
void DFP_PerchSwitch_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x21C | func: 5 | export: 5
u32 DFP_PerchSwitch_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x22C | func: 6 | export: 6
u32 DFP_PerchSwitch_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFP_PerchSwitch_Data);
}

// offset: 0x240 | func: 7
int DFP_PerchSwitch_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    return 0;
}
