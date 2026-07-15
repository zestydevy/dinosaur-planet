//NOTE: WL_Column and WL_Column_Top seem to have used this DLL earlier in development.
//      They both use DLL 313 (PickObj) in December 2000.

#include "common.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/objmsg.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
} WL_Column_Setup;

typedef struct {
    s16 messageArgLo;
    s16 messageArgHi;
    s8 unk4;
    s8 state;
    s8 sendPlayerMessage;
} WL_Column_Data;

typedef enum {
    WL_Column_STATE_0_Resting,
    WL_Column_STATE_1_Lifted,
    WL_Column_STATE_2_Dropped
} WL_Column_States;

static int WL_Column_handle_player_interaction(Object* self, Object* player, WL_Column_Data* objData);

// offset: 0x0 | ctor
void WL_Column_ctor(void* dll) { }

// offset: 0xC | dtor
void WL_Column_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_Column_setup(Object* self, WL_Column_Setup* objSetup, s32 reset) {
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.pitch = -M_180_DEGREES;
}

// offset: 0x38 | func: 1 | export: 1
void WL_Column_control(Object* self) {
    Object* player;
    WL_Column_Data* objData;
    f32 y;
    f32 floorY;
    s32 count;
    Func_80057F1C_Struct** collisionInfo;
    Func_80057F1C_Struct* collision;
    Object* obj;
    s32 i;

    objData = self->data;
    player = get_player();
    
    if (objData->state == WL_Column_STATE_0_Resting) {
        //Advance state when player interacts
        objData->state = WL_Column_handle_player_interaction(self, player, objData);
        if (objData->state != WL_Column_STATE_0_Resting) {
            objData->sendPlayerMessage = TRUE;
        }
        
        if (self->unkE0 == 0) {
            func_8002674C(self);
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            
            //Fall
            self->velocity.y -= 0.1f * gUpdateRateF;
            self->srt.transl.y += self->velocity.y * gUpdateRateF;
            
            //Rest on surfaces
            count = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &collisionInfo, 0, 1);
            obj = NULL;
            for (i = 0; i < count; i++) {
                collision = collisionInfo[i];
                if (collision->unk14 != 14) { //Check terrain isn't water
                    y = self->srt.transl.y;
                    floorY = collision->unk0[0];
                    if ((y < floorY) && (((floorY - 40.0f) < y) || (i == 0))) {
                        obj = collision->unk10;
                        self->srt.transl.y = floorY;
                        self->velocity.y = 0.0f;
                    }
                }
            }
            
            //Add self to other object's polyhits
            if (obj != NULL) {
                obj->polyhits->unk100[obj->polyhits->unk10F] = self;
                obj->polyhits->unk10F++;
            }
            
            //Save position
            map_save_object(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y + 10.0f, self->srt.transl.z);
        }
    } else {
        func_800267A4(self);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        
        //Let go when pressing A
        {
            if (joy_get_pressed(0) & A_BUTTON) {
                objData->sendPlayerMessage = FALSE;
                joy_disable_buttons(0, A_BUTTON);
            }
            
            if (self->unkE0 == 1) {
                objData->state = WL_Column_STATE_2_Dropped;
            }
        }
        
        //Return to initial state after being dropped
        if ((objData->state == WL_Column_STATE_2_Dropped) && (self->unkE0 == 0)) {
            objData->state = WL_Column_STATE_0_Resting;
            objData->sendPlayerMessage = FALSE;
        }
        
        //Send a message to the player while carried
        if (objData->sendPlayerMessage) {
            obj_send_mesg(player, 0x100008, self, (void*)((objData->messageArgHi << 0x10) | (objData->messageArgLo & 0xFFFF)));
        }
    }
}

// offset: 0x314 | func: 2 | export: 2
void WL_Column_update(Object* self) { }

// offset: 0x320 | func: 3 | export: 3
void WL_Column_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (self->unkE0) {
        if (visibility != -1) {
            return;
        }
    } else if (visibility == 0) {
        return;
    }
    
    if (self->def->shadowType == 2) {
        if (self->seqSlot == -1) {
            self->shadow->flags &= ~OBJ_SHADOW_FLAG_FADE_OUT;
        } else {
            self->shadow->flags |= OBJ_SHADOW_FLAG_FADE_OUT;
        }
    }
    
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0x3E0 | func: 4 | export: 4
void WL_Column_free(Object* self, s32 onlySelf) { }

// offset: 0x3F0 | func: 5 | export: 5
s32 WL_Column_get_model_flags(Object* self) {
    //This suggests this DLL was once used by `WL_Column` and `WL_Column_Top` (they use DLL 313 now instead)
    if (self->id == OBJ_WL_Column_Top) {
        return MODFLAGS_SHADOW;
    } else {
        //OBJ_WL_Column?
        return MODFLAGS_NONE;
    }
}


// offset: 0x414 | func: 6 | export: 6
u32 WL_Column_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(WL_Column_Data);
}

// offset: 0x428 | func: 7
int WL_Column_handle_player_interaction(Object* self, Object* player, WL_Column_Data* objData) {
    s32 nextState;

    nextState = WL_Column_STATE_0_Resting;
    if ((self->unkAF & ARROW_FLAG_1_Interacted) && (self->unkE0 == 0)) {
        objData->messageArgLo = 0;
        objData->messageArgHi = 40;
        joy_disable_buttons(0, A_BUTTON);
        nextState = WL_Column_STATE_1_Lifted;
    }
    
    return nextState;
}
