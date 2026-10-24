#include "dll.h"
#include "dlls/objects/210_player.h"
#include "game/objects/object_id.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/objects.h"
#include "sys/objprint.h"

typedef struct {
    f32 cooldownTimer;
    u8 state;
} WCTempleLift_Data;

typedef enum {
    Lift_STATE_Down,
    Lift_STATE_Up
} WCTempleLift_States;

typedef enum {
    WCTempleLift_OBJSEQ0_Up,
    WCTempleLift_OBJSEQ1_Down
} WCTempleLift_ObjSeqs;

// offset: 0x0 | ctor
void WCTempleLift_ctor(void* dll) { }

// offset: 0xC | dtor
void WCTempleLift_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCTempleLift_obj_Setup(Object* self, Generic_Setup* setup, s32 reset) {
    self->srt.yaw = setup->yaw << 8;
}

// offset: 0x30 | func: 1 | export: 1
void WCTempleLift_obj_Control(Object* self) {
    #define LIFT_MOVE_COOLDOWN 300
    #define LIFT_TERRAIN_TYPE 0x21
    WCTempleLift_Data* objdata;
    Object* player;
    s32 i;
    Object* obj;

    objdata = self->data;

    //Wait at least 5 seconds between lift movements
    objdata->cooldownTimer -= gUpdateRateF;
    if (objdata->cooldownTimer < 0.0f) {
        objdata->cooldownTimer = 0.0f;
    }

    //Move up/down when the player stands on the lift
    if (objdata->state == Lift_STATE_Down) {
        if (self->polyhits->unk10F > 0) {
            for (i = 0; i < self->polyhits->unk10F; i++) {
                obj = self->polyhits->unk100[i];
                if (obj->id == OBJ_Sabre) {
                    player = objGetPlayer();
                    if ((objdata->cooldownTimer <= 0.0f) && (((DLL_210_Player*)player->dll)->vtbl->func70(player) == LIFT_TERRAIN_TYPE)) {
                        gDLL_3_Animation->vtbl->start_obj_sequence(WCTempleLift_OBJSEQ0_Up, self, -1);
                        objdata->state = Lift_STATE_Up;
                        objdata->cooldownTimer = LIFT_MOVE_COOLDOWN;
                    }
                }
            }
        }
    } else {
        if (self->polyhits->unk10F > 0) {
            for (i = 0; i < self->polyhits->unk10F; i++) {
                obj = self->polyhits->unk100[i];
                if (obj->id == OBJ_Sabre) {
                    player = objGetPlayer();
                    if ((objdata->cooldownTimer <= 0.0f) && (((DLL_210_Player*)player->dll)->vtbl->func70(player) == LIFT_TERRAIN_TYPE)) {
                        gDLL_3_Animation->vtbl->start_obj_sequence(WCTempleLift_OBJSEQ1_Down, self, -1);
                        objdata->state = Lift_STATE_Down;
                        objdata->cooldownTimer = LIFT_MOVE_COOLDOWN;
                    }
                }
            }
        }
    }
}

// offset: 0x284 | func: 2 | export: 2
void WCTempleLift_obj_Update(Object* self) { }

// offset: 0x290 | func: 3 | export: 3
void WCTempleLift_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2E4 | func: 4 | export: 4
void WCTempleLift_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x2F4 | func: 5 | export: 5
u32 WCTempleLift_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x304 | func: 6 | export: 6
u32 WCTempleLift_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCTempleLift_Data);
}
