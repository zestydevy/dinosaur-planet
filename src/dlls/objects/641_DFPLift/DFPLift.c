#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "dll.h"
#include "functions.h"
#include "types.h"

// Note: This file appears to be called "lift1.c" in the original game.

typedef struct {
/*0x00*/ ObjSetup base;
/*0x18*/ s8 rotation;
/*0x1A*/ s16 unk1A;
/*0x1C*/ s16 unk1C;
/*0x1E*/ s16 unk1E;
/*0x20*/ s16 unk20;
} DFPLift_Setup;

typedef struct {
/*0x00*/ f32 unk0;
/*0x04*/ u32 soundHandle;
/*0x08*/ s16 state;
/*0x0A*/ s16 unkA;
/*0x0C*/ s16 unkC;
/*0x0E*/ s16 cooldown;
/*0x10*/ u8 unk10;
/*0x11*/ u8 unk11;
} DFPLift_Data;

typedef enum {
    LIFT_STATE_INIT = 0,
    LIFT_STATE_INIT_DONE = 1,
    LIFT_STATE_STOPPED = 2,
    LIFT_STATE_GO_DOWN = 3,
    LIFT_STATE_GO_UP = 4
} LiftStateType;

#define LIFT_UP 60.0f // how far up from spawn the lift goes
#define LIFT_DOWN 1228.0f // how far down from spawn the lift goes
#define LIFT_COOLDOWN 100
#define PLAYER_ACTIVATE_RANGE 50.0f
#define PLAYER_INIT_ACTIVATE_RANGE 230.0f

static int DFPLift_func_91C(Object *a0, Object *a1, AnimObj_Data *a2, s8 a3);

// offset: 0x0 | ctor
void DFPLift_ctor(void *dll) { }

// offset: 0xC | dtor
void DFPLift_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFPLift_setup(Object *self, DFPLift_Setup *setup, s32 a2) {
    DFPLift_Data *objdata;

    objdata = (DFPLift_Data*)self->data;

    self->animCallback = DFPLift_func_91C;
    self->srt.yaw = setup->rotation * 256;

    objdata->state = LIFT_STATE_INIT;
    objdata->unkA = setup->unk20;
    objdata->unkC = setup->unk1E;
    objdata->unk0 = setup->unk1A;
    objdata->unk10 = setup->unk1C;

    self->srt.transl.y -= LIFT_DOWN;
    self->unkB0 |= 0x2000;
}

// offset: 0x9C | func: 1 | export: 1
void DFPLift_control(Object* self) {
    DFPLift_Setup* setup;
    DFPLift_Data* objdata;
    Object* player;

    setup = (DFPLift_Setup*)self->setup;
    objdata = (DFPLift_Data*)self->data;
    
    player = get_player();
    if (player == NULL) {
        return;
    }
    
    switch (objdata->state) {
    case LIFT_STATE_INIT:
        if ((main_get_bits(objdata->unkA) != 0) && (objdata->unk10 != 1) && 
                (vec3_distance_xz(&self->positionMirror, &player->positionMirror) < PLAYER_INIT_ACTIVATE_RANGE)) {
            // go up with sound
            if (self->srt.transl.y < (setup->base.y + LIFT_UP)) {
                if (objdata->soundHandle == 0) {
                    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_6EC_Mechanical_Hum_Loop, 0x75, &objdata->soundHandle, NULL, 0, NULL);
                    objdata->unk11 = 1;
                }
                self->srt.transl.y += gUpdateRateF;
                if ((setup->base.y + LIFT_UP) <= self->srt.transl.y) {
                    self->srt.transl.y = (setup->base.y + LIFT_UP);
                    objdata->state = LIFT_STATE_INIT_DONE;
                    if (objdata->soundHandle != 0) {
                        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
                        objdata->soundHandle = 0;
                    }
                }
                return;
            }
        } else if (objdata->unk10 == 1) {
            if (vec3_distance_xz(&self->positionMirror, &player->positionMirror) < PLAYER_INIT_ACTIVATE_RANGE) {
                // go up without sound
                if (self->srt.transl.y < (setup->base.y + LIFT_UP)) {
                    self->srt.transl.y += gUpdateRateF;
                    if ((setup->base.y + LIFT_UP) <= self->srt.transl.y) {
                        self->srt.transl.y = (setup->base.y + LIFT_UP);
                        objdata->state = LIFT_STATE_INIT_DONE;
                    }
                }
            }
        }
        break;
    case LIFT_STATE_INIT_DONE:
        objdata->state = LIFT_STATE_STOPPED;
        objdata->cooldown = LIFT_COOLDOWN;
        break;
    case LIFT_STATE_STOPPED:
        // stopped
        if (objdata->cooldown != 0) {
            objdata->cooldown -= (s16)gUpdateRateF;
            if (objdata->cooldown <= 0) {
                objdata->cooldown = 0;
            }
        } else {
            if (vec3_distance_xz(&self->positionMirror, &player->positionMirror) < PLAYER_ACTIVATE_RANGE) {
                // player is on lift, start moving
                if (self->srt.transl.y == (setup->base.y + LIFT_UP)) {
                    // at top, start going down
                    objdata->state = LIFT_STATE_GO_DOWN;
                    if (objdata->soundHandle == 0) {
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_6EC_Mechanical_Hum_Loop, 0x6B, &objdata->soundHandle, NULL, 0, NULL);
                        objdata->unk11 = 1;
                    }
                } else if (self->srt.transl.y == (setup->base.y - LIFT_DOWN)) {
                    // at bottom, start going up
                    objdata->state = LIFT_STATE_GO_UP;
                    if (objdata->soundHandle == 0) {
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_6EC_Mechanical_Hum_Loop, 0x43, &objdata->soundHandle, NULL, 0, NULL);
                        objdata->unk11 = 1;
                    }
                }
            } else {
                // player is not on the lift, move to where they are
                if (player->srt.transl.y < setup->base.y) {
                    objdata->state = LIFT_STATE_GO_DOWN;
                    if (objdata->unk11 == 1) {
                        objdata->unk11 = 0;
                    }
                } else if (setup->base.y < player->srt.transl.y) {
                    objdata->state = LIFT_STATE_GO_UP;
                    if (objdata->unk11 == 1) {
                        objdata->unk11 = 0;
                    }
                }
            }
        }
        break;
    case LIFT_STATE_GO_DOWN:
        // going down
        if ((setup->base.y - LIFT_DOWN) < self->srt.transl.y) {
            self->srt.transl.y -= gUpdateRateF;
            if (self->srt.transl.y <= (setup->base.y - LIFT_DOWN)) {
                // reached bottom
                self->srt.transl.y = (setup->base.y - LIFT_DOWN);
                objdata->state = LIFT_STATE_STOPPED;
                if (objdata->soundHandle != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
                    objdata->soundHandle = 0;
                }
                objdata->cooldown = LIFT_COOLDOWN;
            }
            if ((vec3_distance_xz(&self->positionMirror, &player->positionMirror) < PLAYER_ACTIVATE_RANGE) && (objdata->unk11 == 1)) {
                
            }
        } else {
            // already at bottom
            if (objdata->soundHandle != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
                objdata->soundHandle = 0;
            }
            vec3_distance_xz(&self->positionMirror, &player->positionMirror);
            objdata->state = LIFT_STATE_STOPPED;
            objdata->cooldown = LIFT_COOLDOWN;
        }
        break;
    case LIFT_STATE_GO_UP:
        // going up
        if (self->srt.transl.y < (setup->base.y + LIFT_UP)) {
            self->srt.transl.y += gUpdateRateF;
            if ((setup->base.y + LIFT_UP) <= self->srt.transl.y) {
                // reached top
                self->srt.transl.y = (setup->base.y + LIFT_UP);
                objdata->state = LIFT_STATE_STOPPED;
                objdata->cooldown = LIFT_COOLDOWN;
                if (objdata->soundHandle != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
                    objdata->soundHandle = 0;
                }
            }
            if ((vec3_distance_xz(&self->positionMirror, &player->positionMirror) < PLAYER_ACTIVATE_RANGE) && (objdata->unk11 == 1)) {
                
            }
        } else {
            // already at top
            objdata->state = LIFT_STATE_STOPPED;
            objdata->cooldown = LIFT_COOLDOWN;
            if (objdata->soundHandle != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
                objdata->soundHandle = 0;
            }
            if ((vec3_distance_xz(&self->positionMirror, &player->positionMirror) < PLAYER_ACTIVATE_RANGE) && (objdata->unk11 == 1)) {

            }
        }
        break;
    default:
        break;
    }
}

// offset: 0x7EC | func: 2 | export: 2
void DFPLift_update(Object *self) { }

// offset: 0x7F8 | func: 3 | export: 3
void DFPLift_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x84C | func: 4 | export: 4
void DFPLift_free(Object *self, s32 a1) {
    DFPLift_Data *objdata;

    objdata = (DFPLift_Data*)self->data;

    gDLL_13_Expgfx->vtbl->func5(self);

    if (objdata->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
        objdata->soundHandle = 0;
    }

    func_80000450(self, self, 0, 0, 0, 0);
}

// offset: 0x8F8 | func: 5 | export: 5
u32 DFPLift_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x908 | func: 6 | export: 6
u32 DFPLift_get_data_size(Object *self, u32 a1) {
    return sizeof(DFPLift_Data);
}

// offset: 0x91C | func: 7
static int DFPLift_func_91C(Object *a0, Object *a1, AnimObj_Data *a2, s8 a3) {
    return 0;
}
