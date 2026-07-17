#include "common.h"
#include "dlls/objects/210_player.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
} DIMGate_Setup;

static int DIMGate_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void DIMGate_ctor(void* dll) { }

// offset: 0xC | dtor
void DIMGate_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMGate_setup(Object* self, DIMGate_Setup* objSetup, s32 reset) {
    self->animCallback = DIMGate_anim_callback;
    self->srt.yaw = objSetup->yaw << 8;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED;
}

// offset: 0x54 | func: 1 | export: 1
void DIMGate_control(Object* self) {
    f32 distance;
    Object* player;

    player = objGetPlayer();

    //Open automatically if the player is riding a vehicle nearby (i.e. on a SnowHorn)
    if (((DLL_210_Player*)player->dll)->vtbl->get_vehicle(player)) {
        //Switch to door-opening animation
        if (self->curModAnimId != 2) {
            objAnimSet(self, 2, 0.0f, 0);
        }

        //Play animation forwards or in reverse, depending on player distance
        distance = vec3Distance(&self->globalPosition, &player->globalPosition);
        if (distance < 250.0f) {
            objAnimAdvance(self, 0.005f, gUpdateRateF, NULL);
        } else if (distance > 250.0f) {
            objAnimAdvance(self, -0.005f, gUpdateRateF, NULL);
        }

        //Update objHits based around the midpoint of the door-opening animation
        if (self->animProgress > 0.5f) {
            if (self->objhitInfo->unkA0 != 2) {
                func_800269CC(self, self->objhitInfo, 2);
            }
        } else {
            if (self->objhitInfo->unkA0 != 1) {
                func_800269CC(self, self->objhitInfo, 1);
            }
        }
    }
}

// offset: 0x1E4 | func: 2 | export: 2
void DIMGate_update(Object* self) { }

// offset: 0x1F0 | func: 3 | export: 3
void DIMGate_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x244 | func: 4 | export: 4
void DIMGate_free(Object* self, s32 onlySelf) { }

// offset: 0x254 | func: 5 | export: 5
u32 DIMGate_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x264 | func: 6 | export: 6
u32 DIMGate_get_data_size(Object* self, u32 offsetAddr) {
    return 0;
}

// offset: 0x278 | func: 7
int DIMGate_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    // NOTE: suggests DIMgate was intended to be used in a sequence! 
    // Possibly related to the unused animations for the sliding hatch built into the gate.
    return 0;
}
