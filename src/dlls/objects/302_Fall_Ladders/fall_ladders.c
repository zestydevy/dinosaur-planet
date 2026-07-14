#include "common.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 modelIdx;        //Which ladder model to use
    s16 raisedOffsetY;  //When raised, the ladder starts off this far above its base position
    s16 unused1C;
    s16 gamebitRaise;   //Ladder rises up by sequence when this gamebit is set (exclusive to `VFP_Ladders2`)
    s16 gamebitFall;    //Ladder starts falling when this gamebit is set
} Fall_Ladders_Setup;

typedef struct {
    f32 raisedOffsetY;  //When raised, the ladder starts off this far above its base position
    s16 gamebitRaise;   //Ladder rises up by sequence when this gamebit is set (exclusive to `VFP_Ladders2`)
    s16 gamebitFall;    //Ladder starts falling when this gamebit is set
    s16 state;          //Raised/Falling/Lowered
    s16 dropTimer;      //Counts down to ladder falling, after gamebitFall set
} Fall_Ladders_Data;

typedef enum {
    Fall_Ladders_STATE_0_Raised,
    Fall_Ladders_STATE_1_Falling,
    Fall_Ladders_STATE_2_Lowered
} Fall_Ladders_States;

static void Fall_Ladders_fall_or_rise_by_sequence(Object* self);
static int Fall_Ladders_anim_callback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3);

// offset: 0x0 | ctor
void Fall_Ladders_ctor(void *dll) { }

// offset: 0xC | dtor
void Fall_Ladders_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Fall_Ladders_setup(Object* self, Fall_Ladders_Setup* objSetup, s32 reset) {
    Fall_Ladders_Data* objData = self->data;
    
    self->srt.yaw = objSetup->yaw << 8;
    
    objData->gamebitFall = objSetup->gamebitFall;
    objData->gamebitRaise = objSetup->gamebitRaise;
    objData->raisedOffsetY = objSetup->raisedOffsetY;
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
    self->animCallback = Fall_Ladders_anim_callback;
    self->srt.transl.y = objSetup->base.y + objData->raisedOffsetY;
    
    objSetModel(self, objSetup->modelIdx);
    
    objData->state = Fall_Ladders_STATE_0_Raised;
}

// offset: 0xC4 | func: 1 | export: 1
void Fall_Ladders_control(Object* self) {
    Fall_Ladders_Setup* objSetup;
    Fall_Ladders_Data* objData;
    f32 magnitude;

    objSetup = (Fall_Ladders_Setup*)self->setup;
    objData = self->data;
    
    //For the special Fall Ladder in VFPT Act 2, use sequences to handle lowering (and uniquely: raising!) instead
    if (self->id == OBJ_VFP_Ladders2) {
        Fall_Ladders_fall_or_rise_by_sequence(self);
        return;
    }
    
    //Start falling after a short delay (when gamebit is set)
    if (objData->dropTimer != 0) {
        objData->dropTimer -= (s16)gUpdateRateF;
        if (objData->dropTimer <= 0) {
            objData->state = Fall_Ladders_STATE_1_Falling;
            gDLL_6_AMSFX->vtbl->play(self, SOUND_6E9_Wooden_Ratcheting, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->dropTimer = 0;
        }
        return;
    }
        
    //Check if the ladder should start falling
    if (objData->state == Fall_Ladders_STATE_0_Raised) {
        if (mainGetBits(objData->gamebitFall)) {
            objData->dropTimer = 10;
        }
    }
    
    //Handle falling
    if (objData->state == Fall_Ladders_STATE_1_Falling) {
        if (objSetup->base.y <= self->srt.transl.y) {
            self->velocity.y -= 0.9f;
            self->srt.transl.y += self->velocity.y * gUpdateRateF;
            
            //Bounce off the ground
            if (self->srt.transl.y <= objSetup->base.y) {
                self->srt.transl.y = objSetup->base.y;
                self->velocity.y = -self->velocity.y * 0.3f; //lose a lot of momentum
                
                if (self->velocity.y >= 0.0f) {
                    magnitude = self->velocity.y;
                } else {
                    magnitude = -self->velocity.y;
                }
                
                //Advance to lowered state when mostly stopped
                if (magnitude < 0.01f) {
                    objData->state = Fall_Ladders_STATE_2_Lowered;
                }
            }
        }
    }
}

// offset: 0x2B4 | func: 2 | export: 2
void Fall_Ladders_update(Object *self) { }

// offset: 0x2C0 | func: 3 | export: 3
void Fall_Ladders_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x2D8 | func: 4 | export: 4
void Fall_Ladders_free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x320 | func: 5 | export: 5
u32 Fall_Ladders_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x330 | func: 6 | export: 6
u32 Fall_Ladders_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(Fall_Ladders_Data);
}

// offset: 0x344 | func: 7
void Fall_Ladders_fall_or_rise_by_sequence(Object* self) {
    Fall_Ladders_Data* objData = self->data;
    
    //Fall
    if (mainGetBits(objData->gamebitFall) && (mainGetBits(objData->gamebitRaise) == FALSE)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
    }
    
    //Rise
    if ((mainGetBits(objData->gamebitFall) == FALSE) && mainGetBits(objData->gamebitRaise)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
    }
}

// offset: 0x42C | func: 8
int Fall_Ladders_anim_callback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    return 0;
}
