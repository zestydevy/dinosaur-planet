#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "functions.h"
#include "dll.h"
#include "sys/main.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "sys/objects.h"
#include "sys/map_enums.h"
#include "sys/menu.h"
#include "sys/map.h"

#define SOMESIZE 28

// tree order
/*0x0*/ static u16 _data_0[3] = {
    BIT_SC_Hit_Village_Tree_One,
    BIT_SC_Hit_Village_Tree_Two,
    BIT_SC_Hit_Village_Tree_Three
};
// passed to func_80000860
/*0x8*/ static u16 _data_8[SOMESIZE] = {
    0x0050, 0x01b3, 0x0050, 0x01b3, 0x01b6, 0x01b9, 0x01bc,
    0x01bc, 0x01b6, 0x01b3, 0x0050, 0x0050, 0x0050, 0x01b3,
    0x01b9, 0x01bf, 0x01bf, 0x01bf, 0x01b6, 0x01b3, 0x0050,
    0x0050, 0x0050, 0x01b3, 0x01b6, 0x01b3, 0x0050, 0x0050
};
// ditto
/*0x40*/ static u16 _data_40[SOMESIZE] = {
    0x004f, 0x01b2, 0x004f, 0x01b2, 0x01b5, 0x01b8, 0x01bb,
    0x01bb, 0x01b5, 0x01b2, 0x004f, 0x004f, 0x004f, 0x01b2,
    0x01b2, 0x01be, 0x01be, 0x01be, 0x01b5, 0x01b2, 0x004f,
    0x004f, 0x004f, 0x01b2, 0x004f, 0x004f, 0x004f, 0x004f
};
// ditto
/*0x78*/ static u16 _data_78[SOMESIZE] = {
    0x0051, 0x01b4, 0x0051, 0x01b4, 0x01b7, 0x01ba, 0x01bd,
    0x01bd, 0x01b7, 0x01b4, 0x0051, 0x0051, 0x0051, 0x01b4,
    0x01ba, 0x01c0, 0x01c0, 0x01c0, 0x01b7, 0x01b4, 0x0051,
    0x0051, 0x0051, 0x01b4, 0x01b7, 0x01b4, 0x0051, 0x0051
};
// ditto
/*0xB0*/ static u16 _data_B0[SOMESIZE] = {
    0x01c3, 0x01c3, 0x01c3, 0x01c3, 0x01c3, 0x01c3, 0x01c3,
    0x01a8, 0x01c3, 0x01c3, 0x01c3, 0x01c3, 0x01c3, 0x01c3,
    0x01c3, 0x01c3, 0x01a8, 0x01a8, 0x01c3, 0x01c3, 0x01c3,
    0x01c3, 0x01c3, 0x01c3, 0x01c3, 0x01c3, 0x01c3, 0x01c3
};
// passed to func_80000450
/*0xE8*/ static u16 _data_E8[SOMESIZE] = {
    0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5,
    0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5,
    0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5,
    0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5, 0x01e5
};
// ditto
/*0x120*/ static u16 _data_120[SOMESIZE] = {
    0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6,
    0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6,
    0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6,
    0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6, 0x01e6
};
// ?
/*0x158*/ static u8 _data_158[3][5] = {
    { 0x01, 0x02, 0x03, 0x04, 0x20 },
    { 0x02, 0x01, 0x03, 0x04, 0x20 },
    { 0x03, 0x01, 0x02, 0x04, 0x20 }
};

typedef struct {
/*00*/ u8 isNighttime;
/*01*/ u8 treesHit; // counts how many correct trees have been hit
/*02*/ u8 unk2; // act/seq related
/*03*/ u8 unk3; // a counter - seconds/minutes?
/*04*/ u8 mapID;
/*08*/ s32 _unk8; // unused?
/*0C*/ u8 index1; // index into SOMESIZE arrays
/*0D*/ u8 index2; // index into _data_158
} SC_levelcontrol_State;

static void SC_levelcontrol_func_8B4(Object *self);
static void SC_levelcontrol_func_BBC(Object *self, u8 arg1);
static void SC_levelcontrol_func_11FC(Object *self);
static s32 SC_levelcontrol_func_12D8(Object *self, Object *arg1, AnimObjState *arg2, void *arg3);

// offset: 0x0 | ctor
void SC_levelcontrol_ctor(void *dll) { }

// offset: 0xC | dtor
void SC_levelcontrol_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SC_levelcontrol_create(Object *self, ObjSetup *setup, s32 arg2) {
    SC_levelcontrol_State *state;

    state = self->state;
    state->mapID = -1;
    state->isNighttime = FALSE;
    state->unk2 = 0;
    state->index1 = main_get_bits(BIT_SC_UNKNOWN_2BA);
    self->unk0xbc = (ObjectCallback)SC_levelcontrol_func_12D8;
}

// offset: 0x90 | func: 1 | export: 1
void SC_levelcontrol_update(Object *self) {
    SC_levelcontrol_State *state;
    Object *player;
    u8 sp2F;

    state = self->state;
    player = get_player();
    sp2F = 0;
    if (main_get_bits(BIT_4BD)) {
        main_set_bits(BIT_4BD, 0);
        sp2F = 1;
        state->index1 = 15;
    }

    if (state->mapID != MAP_SWAPSTONE_CIRCLE) {
        if (map_get_map_id_from_xz_ws(player->srt.transl.x, player->srt.transl.z) == MAP_SWAPSTONE_CIRCLE) {
            SC_levelcontrol_func_8B4(self);
            sp2F = 0;
        } else {
            return;
        }
    }

    state->mapID = map_get_map_id_from_xz_ws(player->srt.transl.x, player->srt.transl.z);

    // trees must be hit in the correct order
    if (main_get_bits(BIT_SC_Hit_Village_Tree_One)) {
        main_set_bits(BIT_SC_Hit_Village_Tree_One, 0);
        if (_data_0[state->treesHit] == BIT_SC_Hit_Village_Tree_One) {
            state->treesHit++;
        } else {
            state->treesHit = 0;
        }
    } else if (main_get_bits(BIT_SC_Hit_Village_Tree_Two)) {
        main_set_bits(BIT_SC_Hit_Village_Tree_Two, 0);
        if (_data_0[state->treesHit] == BIT_SC_Hit_Village_Tree_Two) {
            state->treesHit++;
        } else {
            state->treesHit = 0;
        }
    } else if (main_get_bits(BIT_SC_Hit_Village_Tree_Three)) {
        main_set_bits(BIT_SC_Hit_Village_Tree_Three, 0);
        if (_data_0[state->treesHit] == BIT_SC_Hit_Village_Tree_Three) {
            state->treesHit++;
        } else {
            state->treesHit = 0;
        }
    }

    if (state->treesHit >= 3) {
        main_set_bits(BIT_SC_Village_Opened, 1);
        state->treesHit = 0;
    }

    if (state->unk2 != 0) {
        if (state->unk2 == 5) {
            if (func_8000FB1C() != 0) {
                main_set_bits(BIT_7CF, 1);
            }
        } else if ((main_get_bits(BIT_Dont_Repeat_Swapstone_Intro)) && (func_8000FB1C() != 0)) {
            main_set_bits(BIT_SC_Pond_Platform_Raised, 0);
            if (state->isNighttime) {
                gDLL_5_AMSEQ2->vtbl->func0(self, 0xEC, 0, 0, 0);
            } else {
                gDLL_5_AMSEQ2->vtbl->func0(self, 0x12, 0, 0, 0);
            }
            state->unk2 = 0;
        }
    }

    SC_levelcontrol_func_BBC(self, sp2F);
    SC_levelcontrol_func_11FC(self);
}

// offset: 0x420 | func: 2 | export: 2
void SC_levelcontrol_func_420(Object *self) { }

// offset: 0x42C | func: 3 | export: 3
void SC_levelcontrol_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x480 | func: 4 | export: 4
void SC_levelcontrol_destroy(Object *self, s32 arg1) {
    SC_levelcontrol_State *state;

    state = self->state;
    if (arg1 == 0) {
        func_80000860(self, get_player(), 0x1C3, 0);
    }
    main_set_bits(BIT_SC_UNKNOWN_2BA, state->index1);
    func_8000FA2C();
    gDLL_5_AMSEQ2->vtbl->func0(self, 0xBB, 0, 0, 0);
    gDLL_5_AMSEQ2->vtbl->func0(self, 0xDA, 0, 0, 0);
    if (state->isNighttime) {
        gDLL_5_AMSEQ2->vtbl->func1(self, 0xEC, 0, 0, 0);
        gDLL_5_AMSEQ2->vtbl->func1(self, 0xBA, 0, 0, 0);
    } else {
        gDLL_5_AMSEQ2->vtbl->func1(self, 0x12, 0, 0, 0);
        gDLL_5_AMSEQ2->vtbl->func1(self, 0xB9, 0, 0, 0);
    }
}

// offset: 0x63C | func: 5 | export: 5
u32 SC_levelcontrol_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x64C | func: 6 | export: 6
u32 SC_levelcontrol_get_state_size(Object *self, u32 arg1) {
    return sizeof(SC_levelcontrol_State);
}

// offset: 0x660 | func: 7 | export: 7
void SC_levelcontrol_func_660(Object *self, u8 arg1) {
    SC_levelcontrol_State *state;
    f32 time;

    state = self->state;
    state->unk2 = arg1;

    if (state->unk2 == 1) {
        main_set_bits(BIT_SC_Pond_Platform_Raised, 1);
        main_set_bits(BIT_SC_Platform_Rises_Totem_Challenge_Begins, 1);
        gDLL_5_AMSEQ2->vtbl->func0(self, 0xEC, 0, 0, 0);
        func_8000F64C(0x1D, 0x2D);
        func_8000F6CC();
    } else if (state->unk2 == 3) {
        if (gDLL_7_Newday->vtbl->func8(&time)) {
            gDLL_5_AMSEQ2->vtbl->func0(self, 0xEC, 0, 0, 0);
            gDLL_5_AMSEQ2->vtbl->func0(self, 0xBA, 0, 0, 0);
        } else {
            gDLL_5_AMSEQ2->vtbl->func0(self, 0x12, 0, 0, 0);
            gDLL_5_AMSEQ2->vtbl->func0(self, 0xB9, 0, 0, 0);
        }
        state->unk2 = 0;
    } else if (state->unk2 == 5) {
        state->index2 = rand_next(0, 2);
        func_8000F64C(0x1D, _data_158[state->index2][4]);
        func_8000F6CC();
    }
}

// offset: 0x8A4 | func: 8 | export: 8
u8 SC_levelcontrol_func_8A4(Object *self) {
    SC_levelcontrol_State *state;
    state = self->state;
    return state->unk2;
}

// offset: 0x8B4 | func: 9
void SC_levelcontrol_func_8B4(Object *self) {
    SC_levelcontrol_State *state;
    s32 mapSetup;
    Object *player;

    state = self->state;
    mapSetup = gDLL_29_Gplay->vtbl->get_map_setup(MAP_SWAPSTONE_CIRCLE);
    player = get_player();

    if (mapSetup < 3) {
        if (main_get_bits(BIT_CC_Used_Cell_Door_Key)) {
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 3);
        }
    } else switch (mapSetup) {
    case 3:
        if (((DLL_210_Player*)player->dll)->vtbl->func38(player, 1)) {
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 8);
        }
        break;
    case 8:
        if (main_get_bits(BIT_Spirit_2_Release_Sabre)) {
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 9);
        }
        break;
    case 9:
        if (main_get_bits(BIT_CF_Spellstone_Activated)) {
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 5);
        }
        break;
    case 5:
        if (main_get_bits(BIT_317)) {
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 6);
        }
        break;
    case 6:
        if (((DLL_210_Player*)player->dll)->vtbl->func38(player, 0x80)) {
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 7);
        }
        break;
    }

    // check map setup of previous map
    mapSetup = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
    SC_levelcontrol_func_BBC(self, 1);
    switch (mapSetup) {
    case 1:
        gDLL_5_AMSEQ2->vtbl->func0(self, 0xCB, 0, 0, 0);
        state->unk2 = 7;
        break;
    case 6:
        main_set_bits(BIT_SC_Play_Krystal_Sees_Tribe_Dino, 0);
        main_set_bits(BIT_SC_Played_Krystal_Sees_Tribe_Dino, 0);
        break;
    }
}

// offset: 0xBBC | func: 10
void SC_levelcontrol_func_BBC(Object *self, u8 arg1) {
    SC_levelcontrol_State *state;
    f32 time;
    u8 isNighttime;
    Object *player;

    state = self->state;
    player = get_player();
    isNighttime = gDLL_7_Newday->vtbl->func8(&time);

    if (arg1 != 0) {
        if (state->index1 >= SOMESIZE) {
            state->index1 = 0;
        }
        func_80000860(self, player, _data_40[state->index1], 0);
        func_80000860(self, player, _data_8[state->index1], 0);
        func_80000860(self, player, _data_78[state->index1], 0);
        if (_data_B0[state->index1] != 0) {
            func_80000860(self, player, _data_B0[state->index1], 0);
        }

        func_80000450(self, player, _data_E8[state->index1], 0, 0, 0);
        func_80000450(self, player, _data_120[state->index1], 0, 0, 0);
        if (isNighttime) {
            gDLL_5_AMSEQ2->vtbl->func0(self, 0xEC, 0, 0, 0);
            gDLL_5_AMSEQ2->vtbl->func0(self, 0xBA, 0, 0, 0);
            func_80000450(self, player, 0x1E3, 0, 0, 0);
        } else {
            gDLL_5_AMSEQ2->vtbl->func0(self, 0x12, 0, 0, 0);
            gDLL_5_AMSEQ2->vtbl->func0(self, 0xB9, 0, 0, 0);
            func_80000450(self, player, 0x1E4, 0, 0, 0);
        }
        main_set_bits(BIT_SC_UNKNOWN_2BA, state->index1);
        return;
    }

    if (isNighttime != state->isNighttime) {
        state->unk3 = 60;
    }

    if (state->unk3 > 0) {
        state->unk3--;
        if (!isNighttime) {
            if (state->unk3 == 59) {
                state->index1++;
                if (state->index1 >= SOMESIZE) {
                    state->index1 = 0;
                }
                func_80000860(self, player, _data_40[state->index1], 0);
                main_set_bits(BIT_SC_UNKNOWN_2BA, state->index1);
            }
            if (state->unk3 == 49) {
                if (_data_B0[state->index1] != 0) {
                    func_80000860(self, player, _data_B0[state->index1], 0);
                }
            }
            if (state->unk3 == 39) {
                func_80000860(self, player, _data_8[state->index1], 0);
            }
            if (state->unk3 == 29) {
                func_80000860(self, player, _data_78[state->index1], 0);
            }
            if (state->unk3 == 19) {
                func_80000450(self, player, _data_E8[state->index1], 0, 0, 0);
            }
            if (state->unk3 == 9) {
                func_80000450(self, player, _data_120[state->index1], 0, 0, 0);
            }
        }
        if (state->unk3 <= 0) {
            if (isNighttime) {
                if (state->unk2 == 0) {
                    gDLL_5_AMSEQ2->vtbl->func0(self, 0xEC, 0, 0, 0);
                    gDLL_5_AMSEQ2->vtbl->func0(self, 0xBA, 0, 0, 0);
                }
                func_80000450(self, player, 0x1E3, 0, 0, 0);
            } else {
                if (state->unk2 == 0) {
                    gDLL_5_AMSEQ2->vtbl->func0(self, 0x12, 0, 0, 0);
                    gDLL_5_AMSEQ2->vtbl->func0(self, 0xB9, 0, 0, 0);
                }
                func_80000450(self, player, 0x1E4, 0, 0, 0);
            }
            state->unk3 = 0;
        }
    }
    state->isNighttime = isNighttime;
}

// offset: 0x11FC | func: 11
void SC_levelcontrol_func_11FC(Object *self) {
    if (!main_get_bits(BIT_SC_Ambushed) &&
        main_get_bits(BIT_SC_Encountered_Baby_Lightfoot)) {
        main_set_bits(BIT_SC_Ambushed, 1);
        gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_CIRCLE, 2, 1);
        warpPlayer(WARP_SC_TOTEM_POLE, FALSE);
        gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_CIRCLE, 1, 0);
    }
}

// offset: 0x12D8 | func: 12
s32 SC_levelcontrol_func_12D8(Object *self, Object *arg1, AnimObjState *arg2, void *arg3) {
    s32 i;

    arg2->unk62 = 0;
    for (i = 0; i < arg2->unk98; i++) {
        switch (arg2->unk8E[i]) {
        case 1:
            SC_levelcontrol_func_660(self, 6);
            break;
        default:
            break;
        }
    }
    SC_levelcontrol_update(self);
    return 0;
}
