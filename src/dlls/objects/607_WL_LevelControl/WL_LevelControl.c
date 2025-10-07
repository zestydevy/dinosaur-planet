#include "PR/ultratypes.h"
#include "dlls/objects/210_player.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "functions.h"
#include "dll.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
} WL_LevelControl_State;

static void dll_607_func_7A4(Object* self);
static void dll_607_func_940(Object* self);
static void dll_607_func_C50(Object* self);
static void dll_607_func_E34(Object* self);
static void dll_607_func_1118(Object* self);
static void dll_607_func_14FC(Object* self);
static void dll_607_func_1788(Object* self);

// offset: 0x0 | ctor
void dll_607_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_607_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_607_func_18(Object* self, ObjCreateInfo* createInfo, s32 arg2) {
    WL_LevelControl_State* state;

    obj_add_object_type(self, 0xA);
    state = self->state;
    state->unk7 = 0;
    state->unk2 = 0x1E;
    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 0:
        break;
    case 1:
        gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 1);
        gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_CIRCLE, 0, 1);
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        func_80000860(self, self, 0xA5, 0);
        func_80000860(self, self, 0xA6, 0);
        state->unk0 = -1;
        break;
    case 5:
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        break;
    case 6:
        func_80000860(self, self, 0xA5, 0);
        func_80000860(self, self, 0xA6, 0);
        set_gplay_bitstring(0x164, 1);
        break;
    case 7:
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        state->unk4 = 0x2BC;
        state->unk6 = 0x1E;
        state->unk2 = state->unk6;
        break;
    default:
        break;
    }
}

// offset: 0x25C | func: 1 | export: 1
void dll_607_func_25C(Object* self) {
    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 0:
        break;
    case 1:
        dll_607_func_7A4(self);
        break;
    case 2:
        dll_607_func_940(self);
        break;
    case 3:
        dll_607_func_C50(self);
        break;
    case 4:
        dll_607_func_E34(self);
        break;
    case 5:
        dll_607_func_1118(self);
        break;
    case 6:
        dll_607_func_14FC(self);
        break;
    case 7:
        dll_607_func_1788(self);
        break;
    }
}

// offset: 0x394 | func: 2 | export: 2
void dll_607_func_394(Object *self) { }

// offset: 0x3A0 | func: 3 | export: 3
void dll_607_func_3A0(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3F4 | func: 4 | export: 4
void dll_607_func_3F4(Object *self, s32 a1) {
    obj_free_object_type(self, 0xA);
    if (self){} // @fake
}

// offset: 0x43C | func: 5 | export: 5
s32 dll_607_func_43C(Object* self) {
    return 0;
}

// offset: 0x44C | func: 6 | export: 6
u32 dll_607_func_44C(Object *self, u32 a1) {
    return sizeof(WL_LevelControl_State);
}

// offset: 0x460 | func: 7
static void dll_607_func_460(Object* self, WL_LevelControl_State* state) {
    Object** temp_v0;
    s32 sp38;
    s32 var_a1;
    s32 var_t0;

    if (get_gplay_bitstring(0x78) == 0) {
        temp_v0 = obj_get_all_of_type(7, &sp38);
        var_t0 = 1;
        
        for (var_a1 = 0; var_a1 < sp38; var_a1++) {
            if ((temp_v0[var_a1]->id == OBJ_WL_Galleon) || (temp_v0[var_a1]->id == OBJ_SB_Galleon)) {
                var_t0 = 0;
            }
        }

        if (var_t0 != 0) {
            if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 1) == 0) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 1, 1);
            }
            set_gplay_bitstring(0xD1, 1);
            state->unk7 = 1;
            state->unk8 |= 1;
        }
    }
}

// offset: 0x648 | func: 8
static void dll_607_func_648(Object* self, WL_LevelControl_State* state) {
    u32 sp2C;
    s32 sp28;
    Object** temp_v0;
    s32 i;

    sp2C = get_gplay_bitstring(0x355);
    if ((state->unk8 & 1) && (get_gplay_bitstring(0xD1) == 0)) {
        if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 1) != 0) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 1, 0);
        }
        state->unk8 &= ~1;
    }
    if (!(state->unk8 & 2) && (sp2C != 0)) {
        temp_v0 = obj_get_all_of_type(7, &sp28);
        for (i = 0; i < sp28; i++) {
            if (temp_v0[i]->id == OBJ_WL_Galleon) {
                i = sp28; // break out of loop
            }
        }
        state->unk8 |= 2;
    }
}

// offset: 0x7A4 | func: 9
static void dll_607_func_7A4(Object* self) {
    /*0x0*/ static u8 _data_0 = 0;

    WL_LevelControl_State* state;
    u8 temp_t8;

    state = self->state;
    temp_t8 = 0;
    temp_t8 += get_gplay_bitstring(0xB1);
    temp_t8 += get_gplay_bitstring(0xB2);
    temp_t8 += get_gplay_bitstring(0xB3);
    if ((temp_t8 == 3) && (_data_0 != 3)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, 0xB89, 0x7F, NULL, NULL, 0, NULL);
    }
    _data_0 = temp_t8;
    switch (state->unk7) {
        case 1:
            if (state->unk9 == 1) {
                dll_607_func_648(self, state);
            }
            break;
        case 0:
            state->unk9 = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
            switch (state->unk9) {
            case 1:
                dll_607_func_460(self, state);
                break;
            case 2:
                break;
            }
            break;
    }
}

// offset: 0x940 | func: 10
static void dll_607_func_940(Object* self) {
    static u8 _data_4 = 1;
    
    Object* player;
    Object* temp_v0;

    player = get_player();
    if ((_data_4 != 0) && (get_gplay_bitstring(0x277) == 0)) {
        set_gplay_bitstring(0x2D, 1);
        set_gplay_bitstring(0x1D7, 1);
        player = get_player();
        set_gplay_bitstring(0xFC, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, 0x14);
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 1, 1);
        func_80000860(self, self, 0x204, 0);
        func_80000860(self, self, 0x205, 0);
        func_80000860(self, self, 0x206, 0);
        _data_4 = 0;
    }
    if (get_gplay_bitstring(0x164) != 0) {
        temp_v0 = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
        if ((temp_v0 != NULL) && (get_gplay_bitstring(0x166) == 0)) {
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        }
    }
}

// offset: 0xC50 | func: 11
static void dll_607_func_C50(Object* self) {
    static u8 _data_8 = 1;
    
    Object* player;

    if ((get_gplay_bitstring(0x316) == 0) & _data_8) {
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        func_80000450(self, self, 0x275, 0, 0, 0);
        func_80000450(self, self, 0x278, 0, 0, 0);
        set_gplay_bitstring(0x2D, 1);
        set_gplay_bitstring(0x1D7, 1);
        player = get_player();
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 2, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, 0x14);
        _data_8 = 0;
    } else if (_data_8 != 0) {
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        func_80000450(self, self, 0x275, 0, 0, 0);
        _data_8 = 0;
    }
}

// offset: 0xE34 | func: 12
static void dll_607_func_E34(Object* self) {
    /*0xC*/ static u8 _data_C = 1;
    /*0x10*/ static s32 _data_10 = -1;

    WL_LevelControl_State* state;
    Object* player;
    s16 temp_v0_3;
    s16 var_s2;
    u8 i;

    var_s2 = 0;
    state = self->state;
    if ((_data_C != 0) && (get_gplay_bitstring(0x317) == 0)) {
        set_gplay_bitstring(0x2D, 1);
        set_gplay_bitstring(0x1D7, 1);
        set_gplay_bitstring(0x21B, 1);
        player = get_player();
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 4, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, 0x14);
        _data_C = 0;
    }
    for (i = 0; i < 6; i++) {
        var_s2 +=  get_gplay_bitstring(i + 0x2AA);
    }
    temp_v0_3 = get_gplay_bitstring(0x2C4);
    if ((temp_v0_3 != 0) && (var_s2 != state->unk0)) {
        state->unk0 = var_s2;
        func_80000860(self, self, (state->unk0 + 0xF4), 0);
        func_80000450(self, self, ((state->unk0 / 2) + 0x13C), 0, 0, 0);
    } else if (temp_v0_3 != _data_10) {
        if (temp_v0_3 != 0) {
            func_80000860(self, self, (state->unk0 + 0xF4), 0);
            func_80000450(self, self, ((state->unk0 / 2) + 0x13C), 0, 0, 0);
        } else {
            func_80000860(self, self, 0xFBU, 0);
            func_80000450(self, self, 0x144U, 0, 0, 0);
        }
    }
    _data_10 = temp_v0_3;
}

// offset: 0x1118 | func: 13
static void dll_607_func_1118(Object* self) {
    /*0x14*/ static u8 _data_14 = 1;

    WL_LevelControl_State* state;
    f32 sp40;
    Object* temp_v0;
    Object** var_a2;
    s32 sp34;
    s16 i;
    s16 temp_v0_2;
    ObjCreateInfo *someCreateInfo;
    Object* player;
    
    sp34 = 0;
    sp40 = 10000.0f;
    player = get_player();
    state = self->state;
    if ((_data_14 != 0) && (get_gplay_bitstring(0x318) == 0)) {
        set_gplay_bitstring(0x2D, 1);
        set_gplay_bitstring(0x1D7, 1);
        set_gplay_bitstring(0x40, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 8, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, 0x14);
        set_gplay_bitstring(0x2DD, 0);
        _data_14 = 0;
    }
    if (get_gplay_bitstring(0x2DB) != 0) {
        func_80059038(0x18, 0, 0);
    }
    if (get_gplay_bitstring(0x2DD) != 0) {
        set_gplay_bitstring(0x32E, 1);
        set_gplay_bitstring(0x2DD, 0);
        temp_v0 = obj_get_nearest_type_to(4, self, &sp40);
        if (temp_v0 != NULL) {
            obj_destroy_object(temp_v0);
        }
        state->unk2 = 0x1E;
    }
    if (get_gplay_bitstring(0x2F7) != 0) {
        var_a2 = obj_get_all_of_type(4, &sp34);
        for (i = 0; i < sp34; i++) {
            someCreateInfo = var_a2[i]->createInfo;
            if ((someCreateInfo->uID == 0x296E) || (someCreateInfo->uID == 0x296F)) {
                obj_destroy_object(var_a2[i]);
            }
        }
        set_gplay_bitstring(0x2F7, 0);
    }
    if (get_gplay_bitstring(0x2EE) != 0) {
        temp_v0_2 = ((DLL_210_Player*)player->dll)->vtbl->func50(player);
        if ((temp_v0_2 != 0x40) && (temp_v0_2 != 0x1D7) && (get_gplay_bitstring(0x2F3) == 0)) {
            warpPlayer(0x39, 0);
        }
        set_gplay_bitstring(0x2EE, 0);
    }
    if (get_gplay_bitstring(0x2FA) != 0) {
        if (get_gplay_bitstring(0x2F7) == 0) {
            set_gplay_bitstring(0x2F7, 1);
        }
        state->unk2 -= (s16)delayByte;
        if (state->unk2 <= 0) {
            state->unk2 = 0;
            set_gplay_bitstring(0x2FA, 0);
            set_gplay_bitstring(0x2F3, 1);
            state->unk2 = 0x1E;
        }
    }
}

// offset: 0x14FC | func: 14
static void dll_607_func_14FC(Object* self) {
    /*0x18*/ static u8 _data_18 = 1;

    Object* player;
    Object* temp_v0;

    player = get_player();
    if ((_data_18 != 0) && (get_gplay_bitstring(0x366) == 0)) {
        temp_v0 = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[10].withOneArg((s32)temp_v0);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 2);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 4);
        set_gplay_bitstring(0x2D, 1);
        set_gplay_bitstring(0x1D7, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 0x20, 1);
        _data_18 = 0;
    }
}

// offset: 0x1788 | func: 15
static void dll_607_func_1788(Object* self) {
    /*0x1C*/ static u8 _data_1C = 1;

    WL_LevelControl_State* state;
    Object* player;

    get_player();
    state = (WL_LevelControl_State*)self->state;
    if ((_data_1C != 0) && (get_gplay_bitstring(0x366) == 0)) {
        set_gplay_bitstring(0x2D, 1);
        set_gplay_bitstring(0x1D7, 1);
        player = get_player();
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 0x40, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, 0x14);
        _data_1C = 0;
        state->unk2 = 1;
        func_80000860(self, self, 0x32, 0);
        func_80000860(self, self, 0x33, 0);
        set_gplay_bitstring(0x221, 1U);
    }
    if (get_gplay_bitstring(0x36C) != 0) {
        if (state->unk4 > 0) {
            state->unk4 -= (s16)delayByte;
            if (state->unk2 != 0) {
                state->unk2 -= (s16)delayByte;
                if (state->unk2 <= 0) {
                    set_gplay_bitstring(0x36D, 1);
                    if (state->unk6 >= 0xB) {
                        state->unk6 = state->unk6 - 1;
                    }
                    state->unk2 = state->unk6;
                }
            }
        }
    }
    if (rand_next(0, 0x1E) == 0) {
        set_gplay_bitstring(0xA7, 1);
    }
    if (rand_next(0, 0xA) == 0) {
        set_gplay_bitstring(0xA7, 0);
    }
}
