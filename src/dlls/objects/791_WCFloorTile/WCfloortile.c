#include "common.h"
#include "game/objects/object.h"
#include "game/objects/setups/51_Generic.h"

typedef struct {
    f32 timer;
    s16 maxAngle;
    u8 state;
    u8 flags;
} WCFloorTile_Data;

typedef enum {
    WCFloorTile_STATE_0_Waiting_to_Fall,
    WCFloorTile_STATE_1_Falling,
    WCFloorTile_STATE_2_Gone,
    WCFloorTile_STATE_3_Finished
} WCFloorTile_States;

typedef enum {
    WCFloorTile_FLAG_Hits_Line_Enabled = 1,
    WCFloorTile_FLAG_Hits_Update_Needed = 2,
    WCFloorTile_FLAG_Can_Fall = 4
} WCFloorTile_Flags;

static void WCFloorTile_manageHitLines(Object* self, WCFloorTile_Data* objData);

// offset: 0x0 | ctor
void WCFloorTile_ctor(void* dll) { }

// offset: 0xC | dtor
void WCFloorTile_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCFloorTile_obj_Setup(Object* self, WCFloorTile_Setup* objSetup, s32 reset) {
    WCFloorTile_Data* objData = self->data;

    self->srt.yaw = -M_90_DEGREES;

    self->objhitInfo->unk58 |= ObjHitInfo_FLAG_1000 | ObjHitInfo_FLAG_800;

    objData->flags |= WCFloorTile_FLAG_Hits_Update_Needed;
}

// offset: 0x50 | func: 1 | export: 1
void WCFloorTile_obj_Control(Object* self) {
    f32 dy;
    s32 i;
    f32 opacity;
    Object* obj;
    WCFloorTile_Data* objData;
    WCFloorTile_Setup* objSetup;

    objData = self->data;
    objSetup = (WCFloorTile_Setup*)self->setup;

    //Reappear and don't fall when the player backtracks through the corridor
    if (mainGetBits(BIT_WC_Moon_Temple_Hazards_Deactivated)) {
        self->srt.transl.y = objSetup->base.y;
        objData->state = WCFloorTile_STATE_3_Finished;
    }

    switch (objData->state) {
    default:
    case WCFloorTile_STATE_0_Waiting_to_Fall:
        //Wait until the player has pressed the wall switch, and then fall when Sabre stands on the tile
        if (objData->flags & WCFloorTile_FLAG_Can_Fall) {
            //Check if Sabre's standing on the tile
            if (self->polyhits->unk10F > 0) {
                for (i = 0; i < self->polyhits->unk10F; i++) {
                    obj = self->polyhits->unk100[i];
                    if (obj->id == OBJ_Sabre) {
                        dll_amSfx->Play(self, SOUND_A11_Stone_Crumbling, 0x28, NULL, NULL, 0, NULL);
                        objData->state = WCFloorTile_STATE_1_Falling;
                        objData->timer = 0.0f;
                        self->velocity.y = 0.0f;
                    }
                }
            }
        } else if (mainGetBits(BIT_WC_Moon_Temple_Illusory_Wall_Switch_Pressed)) {
            objData->flags |= WCFloorTile_FLAG_Can_Fall;
        }
        break;
    case WCFloorTile_STATE_1_Falling:
        objData->timer += gUpdateRateF;

        //Start falling after 2 seconds
        if (objData->timer > 120.0f) {
            objData->timer = 120.0f;
            objData->flags |= WCFloorTile_FLAG_Hits_Line_Enabled | WCFloorTile_FLAG_Hits_Update_Needed;
            self->velocity.y += gUpdateRateF * -0.1f;
        }

        //Rattle around to indicate the tile's going to fall
        objData->maxAngle = (s16) ((objData->timer / 120.0f) * 256.0f);
        self->srt.pitch = mathRnd(-objData->maxAngle, objData->maxAngle);
        self->srt.roll = mathRnd(-objData->maxAngle, objData->maxAngle);

        //Drop
        self->srt.transl.y += self->velocity.y * gUpdateRateF;

        //Correlate opacity with fall distance
        dy = objSetup->base.y - self->srt.transl.y;
        if (dy < 50.0f) {
            opacity = OBJECT_OPACITY_MAX;
        } else if (dy > 150.0f) {
            opacity = 0.0f;
        } else {
            opacity = 1.0f - ((dy - 50.0f) / 100.0f);
            if (opacity > 1.0f) {
                opacity = 1.0f;
            } else if (opacity < 0.0f) {
                opacity = 0.0f;
            }
            opacity *= OBJECT_OPACITY_MAX;
        }
        self->opacity = (s32)opacity;

        //Advance state when fully faded out
        if (self->opacity == 0) {
            objData->state = WCFloorTile_STATE_2_Gone;
        }
        break;
    case WCFloorTile_STATE_2_Gone:
        self->opacity = 0;
        func_800267A4(self);
        objData->flags |= WCFloorTile_FLAG_Hits_Line_Enabled | WCFloorTile_FLAG_Hits_Update_Needed;
        break;
    case WCFloorTile_STATE_3_Finished:
        //Fade back in, and don't fall from now on
        opacity = self->opacity;
        opacity += (gUpdateRateF * 8.0f);
        if (opacity > OBJECT_OPACITY_MAX) {
            opacity = OBJECT_OPACITY_MAX;
        }
        self->opacity = opacity;

        //@bug: hits lines can still be enabled while in this state

        func_8002674C(self);
        break;
    }

    WCFloorTile_manageHitLines(self, objData);
}

// offset: 0x4B0 | func: 2 | export: 2
void WCFloorTile_obj_Update(Object* self) { }

// offset: 0x4BC | func: 3 | export: 3
void WCFloorTile_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x510 | func: 4 | export: 4
void WCFloorTile_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x520 | func: 5 | export: 5
u32 WCFloorTile_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x530 | func: 6 | export: 6
u32 WCFloorTile_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCFloorTile_Data);
}

// offset: 0x544 | func: 7
void WCFloorTile_manageHitLines(Object* self, WCFloorTile_Data* objData) {
    WCFloorTile_Setup* objSetup = (WCFloorTile_Setup*)self->setup;

    if (trackIntersectNeedsUpdate()) {
        objData->flags |= WCFloorTile_FLAG_Hits_Update_Needed;
    }

    //Update the jump/grab HITS line(s) associated with the tile
    if ((objData->flags & WCFloorTile_FLAG_Hits_Update_Needed) && (trackIntersectNeedsUpdate() == FALSE)) {
        trackToggleHitLine(objSetup->hitsAnimatorID, self->parent, objData->flags & WCFloorTile_FLAG_Hits_Line_Enabled);
        objData->flags &= ~WCFloorTile_FLAG_Hits_Update_Needed;
    }
}
