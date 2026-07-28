#include "common.h"
#include "sys/objlib.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 gamebitRestoreState;
    s16 unk1C;
    s8 unk1E;
    u8 yaw;
    s8 unk20;
    u8 scale;
} CFTreasureDoor_Setup;

typedef struct {
    u8 preemptState;
    u8 state;
    s32 texAnimTimer;
} CFTreasureDoor_Data;

typedef enum {
    CFTreasureDoor_STATE_0_Initial,
    CFTreasureDoor_STATE_1_Find_Other_Door,
    CFTreasureDoor_STATE_2_Finished
} CFTreasureDoor_States;

typedef enum {
    CFTreasureDoor_PREEMPT_STATE_1_Preempt_Needed = 1,
    CFTreasureDoor_PREEMPT_STATE_2_Preempt_Finished = 2
} CFTreasureDoor_ObjSeq_PreemptStates;

/*0x0*/ static s16 dBabyPerchedGamebits[] = {
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_One, 
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Two, 
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Three, 
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Four
};

// offset: 0x0 | ctor
void CFTreasureDoor_ctor(void* dll) { }

// offset: 0xC | dtor
void CFTreasureDoor_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFTreasureDoor_obj_Setup(Object* self, CFTreasureDoor_Setup* objSetup, s32 reset) {
    CFTreasureDoor_Data* objData;
    TextureAnimator* texAnim;

    objData = self->data;
    
    objData->state = mainGetBits(objSetup->gamebitRestoreState);
    self->srt.yaw = objSetup->yaw << 8;
    
    //Set scale
    {
        if (objSetup->scale == 0) {
            objSetup->scale = 64;
        }
        self->srt.scale = objSetup->scale / 64.0f;
        if (self->srt.scale == 0.0f) {
            self->srt.scale = 1.0f;
        }
        self->srt.scale *= self->def->scale;
    }

    objData->preemptState = mainGetBits(BIT_477_CRF_Treasure_Room_Doors_Opened);
    
    texAnim = objExprGetTexAnimator(self, 0, 0);
    if (texAnim != NULL) {
        texAnim->frame = 0;
    }
    
    objData->texAnimTimer = 0;

    self->unkE0 = 0;
    self->unkDC = 0;
}

// offset: 0x150 | func: 1 | export: 1
void CFTreasureDoor_obj_Control(Object* self) {
    CFTreasureDoor_Data* objData;
    TextureAnimator* texAnim;
    Object* otherDoor;
    s32 frameIdx;
    s32 gamebitIdx;
    f32 distance;
    u8 preemptState;

    objData = self->data;
    gamebitIdx = 0;
    distance = 6000.0f;

    //Preempt the door into its open position if the Treasure Room doors have already been opened
    {
        preemptState = objData->preemptState;
        if (preemptState == CFTreasureDoor_PREEMPT_STATE_2_Preempt_Finished) {
            return;
        }

        switch (preemptState) {
        case CFTreasureDoor_PREEMPT_STATE_1_Preempt_Needed:
            gDLL_3_Animation->vtbl->preempt_sequence_time(self, 85);
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, 1);
            objData->preemptState = CFTreasureDoor_PREEMPT_STATE_2_Preempt_Finished;
            return;   
        }
    }

    //When this door's sequence finishes: find the other door, and set the "both doors open" gamebit 
    {
        if (self->unkDC != 0) {
            self->unkDC = 0;
            self->unkE0 = 1;
        }
        
        if (self->unkE0 != 0) {
            distance = 80000.0f;
            otherDoor = objFindClosestObject(self, OBJ_CFTreasureDoor, &distance);
            if ((otherDoor != NULL) && (otherDoor->unkE0 != 0)) {
                mainSetBits(BIT_477_CRF_Treasure_Room_Doors_Opened, TRUE);
                self->unkE0 = 0;
            }
        }
    }
    
    //Decide which texAnimator frame to use
    {
        objData->texAnimTimer += gUpdateRate;
        frameIdx = 0;

        if (mainGetBits(BIT_CRF_Play_Seq_005D_Treasure_Room_Door_Opening_Sequences)) {
            //Blink all lights
            if ((objData->texAnimTimer % 79) / 20) {
                frameIdx = 5;
            }
        } else {
            //Cycle through the perch lights, and light up the ones that have a CloudRunner Baby on them
            gamebitIdx = (objData->texAnimTimer % 79) / 20;
            if (mainGetBits(dBabyPerchedGamebits[gamebitIdx])) {
                frameIdx = gamebitIdx + 1;
            }
        }
    }
    
    //@debug code: set the current CloudRunner Baby gamebit using B!
    if (joyGetPressed(0) & B_BUTTON) {
        mainSetBits(dBabyPerchedGamebits[gamebitIdx], TRUE);
    }
    
    if (objData->state != CFTreasureDoor_STATE_2_Finished) {
        //Set door's texture frame (lighting up green panels for each CloudRunner baby rescued)
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            texAnim->frame = frameIdx << 8;
        }
        
        //Check if Treasure Door opening sequence should play
        if (mainGetBits(BIT_CRF_Play_Seq_005D_Treasure_Room_Door_Opening_Sequences)) {
            objData->state = CFTreasureDoor_STATE_1_Find_Other_Door;
        }
        
        //Play the other door's opening sequence
        if (objData->state == CFTreasureDoor_STATE_1_Find_Other_Door) {
            distance = 80000.0f; 
            otherDoor = objFindClosestObject(self, OBJ_CFTreasureDoor, &distance);
            
            if ((otherDoor && (otherDoor->unkDC == 0)) || (otherDoor == NULL)) {
                self->unkDC = 1;
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                objData->state = CFTreasureDoor_STATE_2_Finished;
            }
        }
    }
}

// offset: 0x480 | func: 2 | export: 2
void CFTreasureDoor_obj_Update(Object* self) { }

// offset: 0x48C | func: 3 | export: 3
void CFTreasureDoor_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4E0 | func: 4 | export: 4
void CFTreasureDoor_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x4F0 | func: 5 | export: 5
u32 CFTreasureDoor_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x500 | func: 6 | export: 6
u32 CFTreasureDoor_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFTreasureDoor_Data);
}
