#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "game/gamebits.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/gfx/gx.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/730_CRSpellstone.h"

typedef struct {
/*00*/ u8 _unk0[0x4];
/*04*/ u8 state;
/*05*/ u8 flags;
/*06*/ u8 _unk6;
/*07*/ u8 _unk7;
} CRCloudRace_Data;

typedef enum {
    STATE_0,
    STATE_1,
    STATE_Pre_Race,
    STATE_Race,
    STATE_Race_Won,
    STATE_Restart_Race
} CRCloudRace_State;

static int CRCloudRace_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);
static void CRCloudRace_do(Object *self);
static void CRCloudRace_do_race(Object *self, CRCloudRace_Data *objdata);

// offset: 0x0 | ctor
void CRCloudRace_ctor(void *dll) { }

// offset: 0xC | dtor
void CRCloudRace_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CRCloudRace_setup(Object *self, ObjSetup *setup, s32 arg2) {
    CRCloudRace_Data *objdata;

    self->animCallback = CRCloudRace_anim_callback;
    objdata = self->data;
    objdata->state = STATE_Pre_Race;
}

// offset: 0x4C | func: 1 | export: 1
void CRCloudRace_control(Object *self) {
    CRCloudRace_Data *objdata;

    objdata = self->data;
    vi_set_update_rate_target(1);
    CRCloudRace_do(self);
    objdata->flags &= ~1;
}

// offset: 0xBC | func: 2 | export: 2
void CRCloudRace_update(Object *self) { }

// offset: 0xC8 | func: 3 | export: 3
void CRCloudRace_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x11C | func: 4 | export: 4
void CRCloudRace_free(Object *self, s32 a1) { }

// offset: 0x12C | func: 5 | export: 5
u32 CRCloudRace_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x13C | func: 6 | export: 6
u32 CRCloudRace_get_data_size(Object *self, u32 a1) {
    return sizeof(CRCloudRace_Data);
}

// offset: 0x150 | func: 7
int CRCloudRace_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    CRCloudRace_Data *objdata;

    objdata = self->data;
    objdata->flags |= 1;
    return 0;
}

// offset: 0x178 | func: 8
void CRCloudRace_do(Object *self) {
    CRCloudRace_Data *objdata;
    Object *player;

    objdata = self->data;
    player = get_player();
    switch (objdata->state) {
    case STATE_Pre_Race:
        if (main_get_bits(BIT_4A0)) {
            main_set_bits(BIT_CF_Killed_Sharpclaw_In_Treasure_Room, 1);
        }
        if (((DLL_210_Player*)player->dll)->vtbl->get_vehicle(player)) {
            main_set_bits(BIT_Play_Seq_0206_CF_Race_Intro, 1);
            main_set_bits(BIT_CF_Race_Started, 1);
            objdata->state = STATE_Race;
        }
        break;
    case STATE_Race:
        CRCloudRace_do_race(self, objdata);
        break;
    case STATE_Race_Won:
        main_set_bits(BIT_CF_Killed_Sharpclaw_In_Treasure_Room, 0);
        if (main_get_bits(BIT_Played_Seq_02A9_CF_Race_End)) {
            func_80000450(self, player, 464, 0, 0, 0);
        }
        break;
    case STATE_Restart_Race:
        objdata->state = STATE_Pre_Race;
        break;
    case STATE_0:
        break;
    default:
        objdata->state = STATE_Pre_Race;
        break;
    }
}

// offset: 0x308 | func: 9
void CRCloudRace_do_race(Object *self, CRCloudRace_Data *objdata) {
    f32 distance;
    Object *spellstone;
    Object *player;

    distance = 5e7f;
    player = get_player();
    if (!main_get_bits(BIT_Play_Seq_02AA_CF_Lose_Race)) {
        if (main_get_bits(BIT_SpellStone_CRF)) {
            objdata->state = STATE_Race_Won;
            main_set_bits(BIT_CF_Race_Started, 0);
            main_set_bits(BIT_Play_Seq_0206_CF_Race_Intro, 0);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 2, 0);
        }
    } else {
        main_set_bits(BIT_Play_Seq_02AA_CF_Lose_Race, 1);
        if (main_get_bits(BIT_Played_Seq_02AA_CF_Lose_Race) && !((DLL_210_Player*)player->dll)->vtbl->get_vehicle(player)) {
            main_set_bits(BIT_CF_Race_Started, 0);
            // reset sequences
            main_set_bits(BIT_Play_Seq_0206_CF_Race_Intro, 0);
            main_set_bits(BIT_Played_Seq_0206_CF_Race_Intro, 0);
            main_set_bits(BIT_Play_Seq_02AA_CF_Lose_Race, 0);
            main_set_bits(BIT_Played_Seq_02AA_CF_Lose_Race, 0);
            // replay sharpclaw sequence
            main_set_bits(BIT_Played_Seq_02AC_CF_Sharpclaw_With_Spellstone, 0);
            main_set_bits(BIT_Play_Seq_02AC_CF_Sharpclaw_With_Spellstone, 1);
            spellstone = obj_get_nearest_type_to(OBJTYPE_32, self, &distance);
            if (spellstone) {
                ((DLL_730_CRSpellstone*)spellstone->dll)->vtbl->func7(spellstone, 1);
            }
            objdata->state = STATE_Restart_Race;
        }
    }
}
