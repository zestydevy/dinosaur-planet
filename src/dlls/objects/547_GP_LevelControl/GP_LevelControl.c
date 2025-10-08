#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "dll.h"
#include "dlls/objects/210_player.h"
#include "sys/objects.h"
#include "functions.h"
#include "sys/map_enums.h"

typedef struct {
/*00*/ f32 heatCutsceneTimer;
/*04*/ u8 unk4; // ?
/*05*/ u8 mapID;
} GP_LevelControl_State;

static void GP_LevelControl_func_4C8(Object *self);
static void GP_LevelControl_func_58C(Object *self, s32 arg1);

// offset: 0x0 | ctor
void GP_LevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void GP_LevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void GP_LevelControl_create(Object *self, ObjCreateInfo *createInfo, s32 arg2) {
    GP_LevelControl_State *state;

    state = self->state;
    state->unk4 = 0;

    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 1:
        main_set_bits(BIT_GP_Sharpclaw_Jetbike_Cutscene2, 1);
        break;
    case 3:
        main_set_bits(BIT_GP_Sharpclaw_Jetbike_Cutscene2, 1);
        break;
    }

    main_set_bits(BIT_DFP_Place_Spellstone_One, 1);
    state->mapID = 0xFF;
}

// offset: 0xF0 | func: 1 | export: 1
void GP_LevelControl_update(Object *self) {
    GP_LevelControl_State *state;
    f32 time;
    f32 timerInc;
    Object *obj;
    Object *obj2;
    Object *player;

    state = self->state;
    player = get_player();

    if (state->mapID != MAP_GOLDEN_PLAINS) {
        if (map_get_map_id_from_xz_ws(player->srt.transl.x, player->srt.transl.z) == MAP_GOLDEN_PLAINS) {
            GP_LevelControl_func_4C8(self);
        } else {
            return;
        }
    }

    state->mapID = map_get_map_id_from_xz_ws(player->srt.transl.x, player->srt.transl.z);

    // heat reducing energy cutscene
    if (!gDLL_7_Newday->vtbl->func8(&time) && !main_get_bits(BIT_643) && ((DLL_210_Player*)player->dll)->vtbl->func43(player) == 0) {
        if (((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xA) != 0) {
            timerInc = 10.0f;
        } else {
            switch ((s32)((DLL_210_Player*)player->dll)->vtbl->func66(player, 2)) {
            default:
            case 0:
                timerInc = 1.0f;
                break;
            case 1:
            case 2:
            case 3:
                timerInc = 2.0f;
                break;
            case 4:
                timerInc = 3.3333f;
                break;
            }
        }

        state->heatCutsceneTimer += timerInc * delayFloat;
        if (state->heatCutsceneTimer > 600.0f) {
            state->heatCutsceneTimer -= 600.0f;
            if (!main_get_bits(BIT_GP_Shown_Heat_Cutscene)) {
                main_set_bits(BIT_GP_Shown_Heat_Cutscene, 1);
                gDLL_3_Animation->vtbl->func17(9, player, -1);
            }
        }
    }

    // trigger the jetbike cutscene?
    if (gDLL_29_Gplay->vtbl->get_map_setup(MAP_GOLDEN_PLAINS) == 2) {
        obj = func_800211B4(0x35138);
        obj2 = func_800211B4(0x3043B);
        if (obj && obj2 && vec3_distance_squared(&obj->positionMirror, &obj2->positionMirror) < 300.0f) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 3, 1);
            main_set_bits(BIT_GP_Sharpclaw_Jetbike_Cutscene1, 1);
            main_set_bits(BIT_GP_Sharpclaw_Jetbike_Cutscene2, 1);
        }
    }
}

// offset: 0x434 | func: 2 | export: 2
void GP_LevelControl_func_434(Object *self) { }

// offset: 0x440 | func: 3 | export: 3
void GP_LevelControl_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x494 | func: 4 | export: 4
void GP_LevelControl_destroy(Object *self, s32 arg1) { }

// offset: 0x4A4 | func: 5 | export: 5
s32 GP_LevelControl_get_setup_flags(Object *self) {
    return 0;
}

// offset: 0x4B4 | func: 6 | export: 6
u32 GP_LevelControl_get_state_size(Object *self, u32 arg1) {
    return sizeof(GP_LevelControl_State);
}

// offset: 0x4C8 | func: 7
void GP_LevelControl_func_4C8(Object *self) {
    s32 mapSetup;

    mapSetup = gDLL_29_Gplay->vtbl->get_map_setup(MAP_GOLDEN_PLAINS);
    gDLL_5_AMSEQ2->vtbl->func0(self, 0xEE, 0, 0, 0);
    GP_LevelControl_func_58C(self, 1);
    if (mapSetup == 3) {
        main_set_bits(BIT_GP_Sharpclaw_Taken_Spellstone, 1);
    }
}

// offset: 0x58C | func: 8
void GP_LevelControl_func_58C(Object *self, s32 arg1) {
    Object *player;

    player = get_player();
    func_80000860(self, player, 0x155, 0);
    func_80000860(self, player, 0x1E5, 0);
    func_80000860(self, player, 0x1E4, 0);
}
