#include "PR/ultratypes.h"
#include "dlls/engine/6_amsfx.h"
#include "sys/main.h"
#include "game/objects/object.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "dll.h"
#include "dlls/objects/210_player.h"
#include "functions.h"
#include "game/gamebits.h"

typedef struct {
/*00*/ u8 state;
/*04*/ Object *player;
/*08*/ f32 timer;
} CCgasventControl_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 _unk18[0x1A-0x18];
/*1A*/ u8 yaw;
} CCgasventControl_Setup;

typedef enum {
    STATE_Loading,
    STATE_Unstarted,
    STATE_Puzzle,
    STATE_Puzzle_Completed,
    STATE_Puzzle_Complete
} CCgasventControl_State;

#define GASVENT_COUNT 4
#define DURATION 900.0f
#define DISTANCE 100.0f

// offset: 0x0 | ctor
void CCgasventControl_ctor(void *dll) { }

// offset: 0xC | dtor
void CCgasventControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCgasventControl_setup(Object *self, CCgasventControl_Setup *setup, s32 arg2) {
    CCgasventControl_Data *objdata = self->data;
    self->srt.yaw = setup->yaw << 8;
    if (main_get_bits(BIT_CC_Completed_Gas_Chamber_Puzzle)) {
        objdata->state = STATE_Puzzle_Completed;
    }
}

// offset: 0x84 | func: 1 | export: 1
void CCgasventControl_control(Object *self) {
    s32 i;
    Object **objects;
    CCgasventControl_Data *objdata;
    s32 count;

    objdata = self->data;

    switch (objdata->state) {
    case STATE_Loading:
        obj_get_all_of_type(OBJTYPE_64, &count);
        objdata->player = get_player();
        if (count == GASVENT_COUNT && objdata->player)
            objdata->state = STATE_Unstarted;
        break;
    case STATE_Unstarted:
        if (main_get_bits(BIT_3EC)) {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
            objdata->state = STATE_Puzzle;
        }
        break;
    case STATE_Puzzle:
        // check if the vents are covered
        objects = obj_get_all_of_type(OBJTYPE_64, &count);
        count = 0;
        for (i = 0; i < GASVENT_COUNT; i++) {
            if (vec3_distance_xz_squared(&objects[i]->positionMirror,
                &obj_get_nearest_type_to(OBJTYPE_6, objects[i], NULL)->positionMirror) > DISTANCE) {
                count++;
                objdata->timer += gUpdateRateF;
            }
        }
        if (count == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_3_Animation->vtbl->func17(1, self, -1);
            objdata->state = STATE_Puzzle_Complete;
            break;
        }
        // hurt player?
        if (objdata->timer > DURATION) {
            objdata->timer -= DURATION;
            if (((DLL_210_Player*)objdata->player->dll)->vtbl->func50(objdata->player) != BIT_Spell_Forcefield)
                func_8002635C(objdata->player, self, 0x15, 1, 0);
        }
        break;
    case STATE_Puzzle_Completed:
        gDLL_3_Animation->vtbl->func20(self, 0);
        gDLL_3_Animation->vtbl->func17(1, self, 0xC);
        objdata->state = STATE_Puzzle_Complete;
        break;
    }
}

// offset: 0x37C | func: 2 | export: 2
void CCgasventControl_update(Object *self) { }

// offset: 0x388 | func: 3 | export: 3
void CCgasventControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3DC | func: 4 | export: 4
void CCgasventControl_free(Object *self, s32 arg1) { }

// offset: 0x3EC | func: 5 | export: 5
u32 CCgasventControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x3FC | func: 6 | export: 6
u32 CCgasventControl_get_data_size(Object *self, u32 arg1) {
    return sizeof(CCgasventControl_Data);
}
