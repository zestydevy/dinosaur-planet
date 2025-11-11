#include "game/objects/object.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/210_player.h"
#include "dll.h"
#include "sys/objects.h"
#include "functions.h"
#include "game/gamebits.h"
#include "sys/gfx/gx.h"
#include "dlls/objects/common/vehicle.h"

typedef struct {
    u8 state;
    u8 flags;
    s8 warpCounter;
} IMIceMountain_Data;

typedef enum {
    STATE_0,
    STATE_Rescue_Tricky,
    STATE_Race_Start_Sequence,
    STATE_Race,
    STATE_Race_Won,
    STATE_Race_Lost,
    STATE_Intro_Sequence
} IMIceMountain_State;

typedef enum {
    IMICEMOUNTAIN_FLAG_1 = 0x1
} IMIceMountain_Flag;

static int IMIceMountain_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);
static void IMIceMountain_do_act1(Object *self);
static void IMIceMountain_do_race(Object *self, IMIceMountain_Data *objdata);
static void IMIceMountain_do_act2(Object *self);
static void IMIceMountain_do_act3(Object *self);
static void IMIceMountain_do_act4(Object *self);

// offset: 0x0 | ctor
void IMIceMountain_ctor(void *dll) { }

// offset: 0xC | dtor
void IMIceMountain_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void IMIceMountain_setup(Object *self, ObjSetup *setup, s32 arg2) {
    IMIceMountain_Data *objdata;
    Object *player;

    objdata = self->data;
    self->animCallback = IMIceMountain_anim_callback;
    gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 1, 0);
    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 1:
        player = get_player();
        if (player && (vec3_distance(&player->positionMirror, &self->positionMirror) < 2.5e7f)) {
            objdata->state = STATE_0;
            break;
        }
        if (main_get_bits(BIT_IM_MultiSeq_3)) {
            if (main_get_bits(BIT_Play_Seq_00EA_IM_Sabre_Falls_Into_Hot_Spring)) {
                objdata->state = STATE_Race_Won;
            } else {
                main_set_bits(BIT_IM_MultiSeq_3, 0);
                main_set_bits(BIT_IM_Race_Ended, 0);
                main_set_bits(BIT_IM_Race_Started, 0);
                main_set_bits(BIT_Play_Seq_00EB_IM_Lose_Race, 0);
                main_set_bits(BIT_Play_Seq_00EA_IM_Sabre_Falls_Into_Hot_Spring, 0);
                objdata->state = STATE_Race_Start_Sequence;
            }
        } else {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0, 1);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 5, 1);
            if (main_get_bits(BIT_Played_Seq_0063_IM_Sabre_Intro)) {
                objdata->state = STATE_Rescue_Tricky;
            } else {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 2, 1);
                objdata->state = STATE_Intro_Sequence;
                main_set_bits(BIT_Play_Seq_0063_IM_Sabre_Intro, 1);
            }
        }
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 3, 1);
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 4, 1);
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 7, 1);
    case 2:
    case 3:
    case 4:
        break;
    }
}

// offset: 0x330 | func: 1 | export: 1
void IMIceMountain_control(Object *self) {
    IMIceMountain_Data *objdata;

    objdata = self->data;
    vi_set_update_rate_target(1); // 60 FPS
    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 1:
        IMIceMountain_do_act1(self);
        break;
    case 2:
        IMIceMountain_do_act2(self);
        break;
    case 3:
        IMIceMountain_do_act3(self);
        break;
    case 4:
        IMIceMountain_do_act4(self);
        break;
    }
    objdata->flags &= ~IMICEMOUNTAIN_FLAG_1;
}

// offset: 0x444 | func: 2 | export: 2
void IMIceMountain_update(Object *self) { }

// offset: 0x450 | func: 3 | export: 3
void IMIceMountain_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4A4 | func: 4 | export: 4
void IMIceMountain_free(Object *self, s32 a1) { }

// offset: 0x4B4 | func: 5 | export: 5
u32 IMIceMountain_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x4C4 | func: 6 | export: 6
u32 IMIceMountain_get_data_size(Object *self, u32 a1) {
    return sizeof(IMIceMountain_Data);
}

// offset: 0x4D8 | func: 7
int IMIceMountain_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    s32 i;
    IMIceMountain_Data *objdata;

    objdata = self->data;
    objdata->flags |= IMICEMOUNTAIN_FLAG_1;
    for (i = 0; i < animObjData->unk98; i++) {
        if (animObjData->unk8E[i] == 1) {
            func_80000860(self, self, 0xA3, 0);
            func_80000860(self, self, 0x9E, 0);
            func_80000860(self, self, 0x104, 0);
            func_80000450(self, self, 0x15B, 0, 0, 0);
            func_80000450(self, self, 0x15A, 0, 0, 0);
            func_80000450(self, self, 0x17C, 0, 0, 0);
            func_80000450(self, self, 0x17B, 0, 0, 0);
            gDLL_12_Minic->vtbl->func6(1);
        }
    }

    return 0;
}

// offset: 0x694 | func: 8
void IMIceMountain_do_act1(Object *self) {
    IMIceMountain_Data *objdata;

    objdata = self->data;
    switch (objdata->state) {
    case STATE_Intro_Sequence:
        if (main_get_bits(BIT_Played_Seq_0063_IM_Sabre_Intro)) {
            objdata->state = STATE_Rescue_Tricky;
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 2, 0);
        }
        break;
    case STATE_Rescue_Tricky:
        if (main_get_bits(BIT_IM_MultiSeq_2)) {
            objdata->state = STATE_Race_Start_Sequence;
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 6, 1);
        }
        break;
    case STATE_Race_Start_Sequence:
        if (main_get_bits(BIT_IM_MultiSeq_3)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 0, 0);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 5, 0);
        }
        if (main_get_bits(BIT_IM_Race_Started)) {
            objdata->state = STATE_Race;
        }
        if (self->unkDC == 0) {
            func_80000860(self, self, 0xA3, 0);
            func_80000860(self, self, 0x9E, 0);
            func_80000860(self, self, 0x119, 0);
            func_80000450(self, self, 0x15B, 0, 0, 0);
            func_80000450(self, self, 0x15C, 0, 0, 0);
            func_80000450(self, self, 0x17C, 0, 0, 0);
            func_80000450(self, self, 0x17B, 0, 0, 0);
            gDLL_12_Minic->vtbl->func6(1);
            self->unkDC = 1;
        }
        break;
    case STATE_Race:
        IMIceMountain_do_race(self, objdata);
        break;
    case STATE_Race_Won:
        if (objdata->flags & IMICEMOUNTAIN_FLAG_1) {
            gDLL_29_Gplay->vtbl->set_map_setup(self->mapID, 2);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 3, 0);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 4, 0);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 6, 0);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 7, 0);
            objdata->state = STATE_0;
        }
        break;
    case STATE_Race_Lost:
        if (objdata->flags & IMICEMOUNTAIN_FLAG_1) {
            objdata->warpCounter = 2;
        }
        if (objdata->warpCounter > 0) {
            objdata->warpCounter -= 1;
            if (objdata->warpCounter == 0) {
                // race will restart after warp
                warpPlayer(WARP_IM_RACE_START, FALSE);
            }
        }
        break;
    case STATE_0:
        break;
    }
}

// offset: 0xAB4 | func: 9
void IMIceMountain_do_race(Object *self, IMIceMountain_Data *objdata) {
    s32 racePosition;
    Object *player;
    Object *snowbike;

    gDLL_1_UI->vtbl->func_2B8(7);
    vi_set_update_rate_target(3); // 20 FPS
    if (main_get_bits(BIT_IM_Race_Ended)) {
        main_set_bits(BIT_IM_Race_Ended, 0);
        main_set_bits(BIT_IM_Race_Started, 0);
        player = get_player();
        snowbike = ((DLL_210_Player*)player->dll)->vtbl->get_vehicle(player);
        if (snowbike) {
            racePosition = ((DLL_IVehicle*)snowbike->dll)->vtbl->func17(snowbike);
        } else {
            racePosition = 0;
        }
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 1, 1);
        if (racePosition == 1) {
            gDLL_1_UI->vtbl->func_2B8(1);
            objdata->state = STATE_Race_Won;
            main_set_bits(BIT_Play_Seq_00EA_IM_Sabre_Falls_Into_Hot_Spring, 1);
        } else {
            objdata->state = STATE_Race_Lost;
            main_set_bits(BIT_Play_Seq_00EB_IM_Lose_Race, 1);
        }
    }
}

// offset: 0xC60 | func: 10
void IMIceMountain_do_act2(Object *self) { }

// offset: 0xC6C | func: 11
void IMIceMountain_do_act3(Object *self) { }

// offset: 0xC78 | func: 12
void IMIceMountain_do_act4(Object *self) { }
