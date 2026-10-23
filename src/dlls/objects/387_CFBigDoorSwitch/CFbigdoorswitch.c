#include "common.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/setups/51.h"
#include "macros.h"

typedef struct {
    u8 state;            //The switch's current state (up/down)
    u8 flags;            //Flags whether an objSeq preempt is needed (to quickly move the switch to its up/down position)
    u8 unlocked;         //Stores whether `gamebitUnlocked` is set during setup, but otherwise unused
} CFBigDoorSwitch_Data;

typedef enum {
    CFBigDoorSwitch_FLAG_Preempt_Needed = 1
} CFBigDoorSwitch_Flags;

// offset: 0x0 | ctor
void CFBigDoorSwitch_ctor(void* dll) { }

// offset: 0xC | dtor
void CFBigDoorSwitch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFBigDoorSwitch_obj_Setup(Object* self, Ctrl51_Setup* objSetup, s32 reset) {
    CFBigDoorSwitch_Data* objData;

    self->srt.yaw = objSetup->cfBigDoorSwitch.yaw << 8;
    
    if (self->objhitInfo != NULL) {
        self->objhitInfo->unk58 |= 1;
    }
    
    objData = self->data;
    objData->unlocked = mainGetBits(objSetup->cfBigDoorSwitch.unlockedBit);
    objData->state = mainGetBits(objSetup->cfBigDoorSwitch.stateBit);
    objData->flags = CFBigDoorSwitch_FLAG_Preempt_Needed;
}

// offset: 0xB8 | func: 1 | export: 1
void CFBigDoorSwitch_obj_Control(Object* self) {
    Ctrl51_Setup* objSetup;
    CFBigDoorSwitch_Data* objData;
    s32 damageType;
    Object* hitBy;

    objData = self->data;
    objSetup = (Ctrl51_Setup*)self->setup;
    
    // Check for attacks
    // (unused, maybe a debug leftover similar to being able to shoot DFPT's perch switch?)
    damageType = func_80025F40(self, &hitBy, NULL, NULL);
    
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        // Let the player interact with the switch, if its unlock gamebit's set.
        // This is a bit surprising, since it's barely within reach for the player and
        // the perch object placed on the switch suggests Kyte was supposed to activate it!
        if (mainGetBits(objSetup->cfBigDoorSwitch.unlockedBit)) {
            //Toggle state and play a sequence
            gDLL_3_Animation->vtbl->start_obj_sequence(objData->state ^ 1, self, -1);
            objData->state ^= 1;
            mainSetBits(objSetup->cfBigDoorSwitch.stateBit, objData->state);
            objData->flags &= ~CFBigDoorSwitch_FLAG_Preempt_Needed;
        }
    } else if (objData->flags & CFBigDoorSwitch_FLAG_Preempt_Needed) {
        //Handle quickly flipping up/down using an objSeq preempt, when flagged
        STUBBED_PRINTF(" Preempt %i ", objSetup->cfBigDoorSwitch.preemptTime);
        gDLL_3_Animation->vtbl->preempt_sequence_time(self, objSetup->cfBigDoorSwitch.preemptTime);
        gDLL_3_Animation->vtbl->start_obj_sequence(objData->state, self, -1);
        objData->flags &= ~CFBigDoorSwitch_FLAG_Preempt_Needed;
    }
    
    //Check if the state-restoring gamebit's value differs from the switch's current state
    if (objData->state != mainGetBits(objSetup->cfBigDoorSwitch.stateBit)) {
        //Sync state with the gamebit, and queue an objSeq preempt to quickly raise/lower the switch
        objData->state = mainGetBits(objSetup->cfBigDoorSwitch.stateBit);
        objData->flags |= CFBigDoorSwitch_FLAG_Preempt_Needed;
    }
}

// offset: 0x254 | func: 2 | export: 2
void CFBigDoorSwitch_obj_Update(Object* self) { }

// offset: 0x260 | func: 3 | export: 3
void CFBigDoorSwitch_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2B4 | func: 4 | export: 4
void CFBigDoorSwitch_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x2C4 | func: 5 | export: 5
u32 CFBigDoorSwitch_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x2D4 | func: 6 | export: 6
u32 CFBigDoorSwitch_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFBigDoorSwitch_Data);
}
