#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "sys/controller.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/menu.h"
#include "dll.h"
#include "functions.h"

typedef struct {
    ObjCreateInfo base;
    u8 _unk18[2];
    u8 rotation;
} SHswapstone_CreateInfo;

typedef struct {
    u8 attachIdx;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    s16 unk6; // game bits ID
    s16 unk8; // game bits ID
    s16 unkA; // game bits ID
} SHswapstone_State;

// warlock mountain warps (sabre, krystal)
/*0x0*/ static u16 sWarlockMountainWarps[] = { WARP_WM_SABRE_SIDE, WARP_WM_KRYSTAL_SIDE };
// swapstone warps (sabre, krystal). this is for swapping characters
/*0x4*/ static u16 sSwapStoneWarps[] = { WARP_SH_ROCKY_PODIUM, WARP_SC_RUBBLE_PODIUM };

static s32 SHswapstone_func_448(Object* self, Object* a1, AnimObjState* a2, void* a3);
static s32 SHswapstone_func_C04(void);
static s32 SHswapstone_func_D0C(void);
static void SHswapstone_func_A8C(Object* self, s32 arg1, s32 arg2);
static s32 SHswapstone_func_AD4(Object* self, s32 arg1, s32 arg2);

// offset: 0x0 | ctor
void SHswapstone_ctor(void *dll) { }

// offset: 0xC | dtor
void SHswapstone_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHswapstone_create(Object* self, SHswapstone_CreateInfo* createInfo, s32 arg2) {
    SHswapstone_State* state;

    state = self->state;
    self->srt.yaw = createInfo->rotation << 8;
    self->unk0xbc = (ObjectCallback)SHswapstone_func_448;

    if (map_get_map_id_from_xz_ws(self->srt.transl.x, self->srt.transl.z) == MAP_SWAPSTONE_CIRCLE) {
        // We are Rubble
        state->unk6 = 0x883;
        state->unk8 = 0x3EB;
        state->unkA = 0x884;
        state->unk2 |= 4;
        self->modelInstIdx = 1;
    } else {
        // We are Rocky
        state->unk6 = 0x15A;
        state->unk8 = 0x886;
        state->unkA = 0x3EA;
        self->modelInstIdx = 0;
    }
    if ((get_gplay_bitstring(0x887) != 0) && (get_gplay_bitstring(0x15A) != 0)) {
        state->unk4 = 1;
    } else {
        state->unk4 = 0;
    }
    set_gplay_bitstring(state->unk8, 0);
}

// offset: 0x140 | func: 1 | export: 1
void SHswapstone_update(Object* self) {
    SHswapstone_State* state;

    state = self->state;
    if (state->unk4 == 0) {
        if (self->curModAnimId != 0xC) {
            func_80023D30(self, 0xC, 0.0f, 0);
        }
        func_80024108(self, 0.008f, delayFloat, 0);
        if ((get_gplay_bitstring(0x887) != 0) && (get_gplay_bitstring(0x15A) != 0)) {
            state->unk4 = 1;
        }
    } else {
        if (self->curModAnimId != 0) {
            func_80023D30(self, 0, 0.0f, 0);
        }
        func_80024108(self, 0.008f, delayFloat, 0);
        if (get_gplay_bitstring(0x887) == 0) {
            state->unk4 = 0;
        }
    }
}

// offset: 0x278 | func: 2 | export: 2
void SHswapstone_func_278(Object *self) { }

// offset: 0x284 | func: 3 | export: 3
void SHswapstone_draw(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Object* player;
    f32 x;
    f32 y;
    f32 z;
    SHswapstone_State* state;

    state = self->state;
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        player = get_player();
        if ((player != NULL) && (((DLL_210_Player*)player->dll)->vtbl->func34(player) != 0)) {
            func_80031F6C(self, state->attachIdx, &x, &y, &z, 0);
            ((DLL_210_Player*)player->dll)->vtbl->func65(player, x, y, z);
            ((DLL_210_Player*)player->dll)->vtbl->base.draw(player, gdl, mtxs, vtxs, pols, -1);
        }
    }
}

// offset: 0x3C4 | func: 4 | export: 4
void SHswapstone_destroy(Object *self, s32 a1) { }

// offset: 0x3D4 | func: 5 | export: 5
s32 SHswapstone_func_3D4(Object* self) {
    s32 var_v1;

    if (map_get_map_id_from_xz_ws(self->srt.transl.x, self->srt.transl.z) == MAP_SWAPSTONE_CIRCLE) {
        // We are Rubble
        var_v1 = 1;
    } else {
        // We are Rocky
        var_v1 = 0;
    }
    return (var_v1 << 0xB) | 0x400;
}

// offset: 0x434 | func: 6 | export: 6
u32 SHswapstone_get_state_size(Object *self, u32 a1) {
    return sizeof(SHswapstone_State);
}

// offset: 0x448 | func: 7
static s32 SHswapstone_func_448(Object* self, Object* a1, AnimObjState* a2, void* a3) {
    SHswapstone_State* state;
    s32 playerno;
    s32 i;

    state = self->state;
    if (menu_get_current() != MENU_16) {
        menu_set(MENU_16);
    }
    a2->unkF8 = SHswapstone_func_AD4;
    a2->unkF4 = SHswapstone_func_A8C;
    if (a2->unk62 != 0) {
        state->unk2 &= ~3;
        if (SHswapstone_func_C04() != 0) {
            state->unk2 |= 1;
        }
        if (SHswapstone_func_D0C() != 0) {
            state->unk2 |= 2;
        }
        a2->unk62 = 0;
        if (get_gplay_bitstring(state->unk6) != 0) {
            a2->unk9D |= 4;
        }
    }
    for (i = 0; i < a2->unk98; i++) {
        switch (a2->unk8E[i]) {
        case 3:
            state->attachIdx = 0;
            break;
        case 4:
            state->attachIdx = 1;
            break;
        case 6:
            playerno = 1 - gDLL_29_Gplay->vtbl->get_playerno();
            gDLL_29_Gplay->vtbl->set_playerno(playerno);
            if (state->unk2 & 4) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_HOLLOW, 0, 1);
                gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_HOLLOW, 7, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_HOLLOW, 1);
                if ((get_gplay_bitstring(0x277) != 0) && (get_gplay_bitstring(0x178) == 0)) {
                    set_gplay_bitstring(0x885, 1);
                } else {
                    set_gplay_bitstring(state->unkA, 1);
                }
            } else {
                gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_CIRCLE, 0, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 2);
                set_gplay_bitstring(state->unkA, 1);
            }
            warpPlayer(sSwapStoneWarps[playerno], /*fadeToBlack=*/FALSE);
            break;
        case 7:
            playerno = gDLL_29_Gplay->vtbl->get_playerno();
            switch (SHswapstone_func_C04()) {
            case 0x2:
            case 0x8:
            case 0x10:
            case 0x40:
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_ICE_MOUNTAIN_1, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_SNOWHORN_WASTES, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_WARLOCK_MOUNTAIN, 3);
                break;
            case 0x1:
            case 0x4:
            case 0x20:
            case 0x80:
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_ICE_MOUNTAIN_1, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_SNOWHORN_WASTES, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_WARLOCK_MOUNTAIN, 2);
                break;
            }
            warpPlayer(sWarlockMountainWarps[playerno], /*fadeToBlack=*/FALSE);
            break;
        case 10:
            state->unk1 ^= 1;
            break;
        case 9:
            playerno = gDLL_29_Gplay->vtbl->get_playerno();
            gDLL_29_Gplay->vtbl->set_playerno(1 - playerno);
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_ICE_MOUNTAIN_1, 1);
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 2);
            menu_set(MENU_GAMEPLAY);
            warpPlayer(WARP_ICE_MOUNTAIN_CAMPSITE, /*fadeToBlack=*/FALSE);
            break;
        case 12:
            warpPlayer(WARP_SWAPSTONE_SHOP_ENTRANCE, /*fadeToBlack=*/FALSE);
            break;
        case 13:
        case 14:
        case 15:
        case 16:
            if (menu_get_current() == MENU_16) {
                ((DLL_Menu16*)menu_get_active_dll())->vtbl->func3(a2->unk8E[i] - 0xD);
            }
            break;
        default:
            break;
        }
    }
    if (state->unk1 != 0) {
        gDLL_20_Screens->vtbl->show_screen(0);
    }
    return 0;
}

// offset: 0xA8C | func: 8
static void SHswapstone_func_A8C(Object* self, s32 arg1, s32 arg2) {
    menu_set(MENU_GAMEPLAY);
}

// offset: 0xAD4 | func: 9
static s32 SHswapstone_func_AD4(Object* self, s32 arg1, s32 arg2) {
    SHswapstone_State* state;
    s8 joyXSign;
    s8 joyYSign;

    state = self->state;
    get_joystick_menu_xy_sign(0, &joyXSign, &joyYSign);
    switch (arg2) {
    case 20:
        if ((joyXSign < 0) && !(state->unk2 & 3)) {
            return 1;
        }
    default:
        break;
    case 21:
        if ((joyXSign < 0) && (state->unk2 & 3)) {
            return 1;
        }
        break;
    case 22:
        if ((joyXSign > 0) && (state->unk2 & 1)) {
            return 1;
        }
        break;
    case 23:
        if ((joyXSign > 0) && !(state->unk2 & 1)) {
            return 1;
        }
        break;
    case 24:
        if (joyYSign > 0) {
            return 1;
        }
        break;
    }
    return 0;
}

// offset: 0xC04 | func: 10
static s32 SHswapstone_func_C04(void) {
    s32 playerno;
    Object* player;
    s32 temp_v0_2;

    playerno = gDLL_29_Gplay->vtbl->get_playerno();
    player = get_player();
    temp_v0_2 = ((DLL_210_Player*)player->dll)->vtbl->func38(player, 0xFF);
    if (playerno == 0) {
        if (temp_v0_2 & 2) {
            return 2;
        }
        if (temp_v0_2 & 8) {
            return 8;
        }
        if (temp_v0_2 & 0x40) {
            return 0x40;
        }
        if (temp_v0_2 & 0x10) {
            return 0x10;
        }
    } else {
        if (temp_v0_2 & 1) {
            return 1;
        }
        if (temp_v0_2 & 4) {
            return 4;
        }
        if (temp_v0_2 & 0x20) {
            return 0x20;
        }
        if (temp_v0_2 & 0x80) {
            return 0x80;
        }
    }
    
    return 0;
}

// offset: 0xD0C | func: 11
static s32 SHswapstone_func_D0C(void) {
    if (get_gplay_bitstring(0x2E8) != 0) {
        return 1;
    }
    if (get_gplay_bitstring(0x123) != 0) {
        return 1;
    }
    return 0;
}
