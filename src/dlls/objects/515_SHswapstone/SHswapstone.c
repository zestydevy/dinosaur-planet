#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "sys/controller.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/map_enums.h"
#include "sys/objects.h"
#include "sys/objanim.h"
#include "sys/menu.h"
#include "dll.h"
#include "functions.h"
#include "types.h"

typedef enum {
    SWAPSTONE_PLAYER_HAS_SPIRIT = 0x1,
    SWAPSTONE_PLAYER_HAS_SPELLSTONE = 0x2,
    SWAPSTONE_IS_RUBBLE = 0x4
} SwapstoneFlags;

typedef struct {
    ObjSetup base;
    u8 _unk18[2];
    u8 rotation;
} SHswapstone_Setup;

typedef struct {
    u8 attachIdx;
    u8 bShowScreen;
    u8 flags;
    u8 unk3;
    u8 unk4;
    s16 bitSwapStoneSpokenTo; // game bits ID
    s16 bitIntroSeq; // game bits ID
    s16 bitSwappedToSeq; // game bits ID
} SHswapstone_Data;

// warlock mountain warps (sabre, krystal)
/*0x0*/ static u16 sWarlockMountainWarps[] = { WARP_WM_SABRE_SIDE, WARP_WM_KRYSTAL_SIDE };
// swapstone warps (sabre, krystal). this is for swapping characters
/*0x4*/ static u16 sSwapStoneWarps[] = { WARP_SH_ROCKY_PODIUM, WARP_SC_RUBBLE_PODIUM };

static int SHswapstone_func_448(Object* self, Object* a1, AnimObj_Data* a2, s8 a3);
static s32 SHswapstone_get_held_spirit(void);
static s32 SHswapstone_has_spellstone(void);
static void SHswapstone_func_A8C(Object* self, s32 arg1, s32 arg2);
static s32 SHswapstone_func_AD4(Object* self, s32 arg1, s32 arg2);

// offset: 0x0 | ctor
void SHswapstone_ctor(void *dll) { }

// offset: 0xC | dtor
void SHswapstone_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHswapstone_setup(Object* self, SHswapstone_Setup* setup, s32 arg2) {
    SHswapstone_Data* objdata;

    objdata = self->data;
    self->srt.yaw = setup->rotation << 8;
    self->animCallback = (AnimationCallback)SHswapstone_func_448;

    // @bug: can't tell mapID correctly if local BLOCKS cell is unloaded upon
    // approaching SwapStone (happens if camera lags behind in SwapStone Circe)
    if (map_get_map_id_from_xz_ws(self->srt.transl.x, self->srt.transl.z) == MAP_SWAPSTONE_CIRCLE) {
        // We are Rubble
        objdata->bitSwapStoneSpokenTo = BIT_883;
        objdata->bitIntroSeq = BIT_Play_Seq_0107_Rocky_Intro_Unused;
        objdata->bitSwappedToSeq = BIT_Play_Seq_01FB_SwapStone_Back_In_SH;
        objdata->flags |= SWAPSTONE_IS_RUBBLE;
        self->modelInstIdx = 1;
    } else {
        // We are Rocky
        objdata->bitSwapStoneSpokenTo = BIT_Talked_to_Rocky;
        objdata->bitIntroSeq = BIT_Play_Seq_035F_Rocky_Intro;
        objdata->bitSwappedToSeq = BIT_Play_Seq_00D7_Swapped_to_Krystal;
        self->modelInstIdx = 0;
    }
    if ((main_get_bits(BIT_Talking_to_Rocky) != 0) && (main_get_bits(BIT_Talked_to_Rocky) != 0)) {
        objdata->unk4 = 1;
    } else {
        objdata->unk4 = 0;
    }
    main_set_bits(objdata->bitIntroSeq, 0);
}

// offset: 0x140 | func: 1 | export: 1
void SHswapstone_control(Object* self) {
    SHswapstone_Data* objdata;

    objdata = self->data;
    if (objdata->unk4 == 0) {
        if (self->curModAnimId != 0xC) {
            func_80023D30(self, 0xC, 0.0f, 0);
        }
        func_80024108(self, 0.008f, delayFloat, NULL);
        if ((main_get_bits(BIT_Talking_to_Rocky) != 0) && (main_get_bits(BIT_Talked_to_Rocky) != 0)) {
            objdata->unk4 = 1;
        }
    } else {
        if (self->curModAnimId != 0) {
            func_80023D30(self, 0, 0.0f, 0);
        }
        func_80024108(self, 0.008f, delayFloat, NULL);
        if (main_get_bits(BIT_Talking_to_Rocky) == 0) {
            objdata->unk4 = 0;
        }
    }
}

// offset: 0x278 | func: 2 | export: 2
void SHswapstone_update(Object *self) { }

// offset: 0x284 | func: 3 | export: 3
void SHswapstone_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Object* player;
    f32 x;
    f32 y;
    f32 z;
    SHswapstone_Data* objdata;

    objdata = self->data;
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        player = get_player();
        if ((player != NULL) && (((DLL_210_Player*)player->dll)->vtbl->func34(player) != 0)) {
            func_80031F6C(self, objdata->attachIdx, &x, &y, &z, 0);
            ((DLL_210_Player*)player->dll)->vtbl->func65(player, x, y, z);
            ((DLL_210_Player*)player->dll)->vtbl->base.print(player, gdl, mtxs, vtxs, pols, -1);
        }
    }
}

// offset: 0x3C4 | func: 4 | export: 4
void SHswapstone_free(Object *self, s32 a1) { }

// offset: 0x3D4 | func: 5 | export: 5
u32 SHswapstone_get_model_flags(Object* self) {
    s32 modelno;

    if (map_get_map_id_from_xz_ws(self->srt.transl.x, self->srt.transl.z) == MAP_SWAPSTONE_CIRCLE) {
        // We are Rubble
        modelno = 1;
    } else {
        // We are Rocky
        modelno = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelno) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x434 | func: 6 | export: 6
u32 SHswapstone_get_data_size(Object *self, u32 a1) {
    return sizeof(SHswapstone_Data);
}

// offset: 0x448 | func: 7
static int SHswapstone_func_448(Object* self, Object* a1, AnimObj_Data* a2, s8 a3) {
    SHswapstone_Data* objdata;
    s32 playerno;
    s32 i;

    objdata = self->data;
    if (menu_get_current() != MENU_16) {
        menu_set(MENU_16);
    }
    a2->unkF8 = SHswapstone_func_AD4;
    a2->unkF4 = SHswapstone_func_A8C;
    if (a2->unk62 != 0) {
        objdata->flags &= ~(SWAPSTONE_PLAYER_HAS_SPIRIT | SWAPSTONE_PLAYER_HAS_SPELLSTONE);
        if (SHswapstone_get_held_spirit() != PLAYER_NO_SPIRIT) {
            objdata->flags |= SWAPSTONE_PLAYER_HAS_SPIRIT;
        }
        if (SHswapstone_has_spellstone() != 0) {
            objdata->flags |= SWAPSTONE_PLAYER_HAS_SPELLSTONE;
        }
        a2->unk62 = 0;
        if (main_get_bits(objdata->bitSwapStoneSpokenTo) != 0) {
            a2->unk9D |= 4;
        }
    }
    for (i = 0; i < a2->unk98; i++) {
        switch (a2->unk8E[i]) {
        case 3:
            objdata->attachIdx = 0;
            break;
        case 4:
            objdata->attachIdx = 1;
            break;
        case 6:
            // Swap players
            playerno = 1 - gDLL_29_Gplay->vtbl->get_playerno();
            gDLL_29_Gplay->vtbl->set_playerno(playerno);
            if (objdata->flags & SWAPSTONE_IS_RUBBLE) {
                // Going to SwapStone Hollow
                gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_HOLLOW, 0, 1);
                gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_HOLLOW, 7, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_HOLLOW, 1);
                if ((main_get_bits(BIT_Play_Seq_0180_Release_Spirit_1) != 0) && (main_get_bits(BIT_Played_Seq_01FD_Rocky_Teaches_Distract) == 0)) {
                    // Set Rocky to give Tricky the distract command
                    main_set_bits(BIT_Play_Seq_01FD_Rocky_Teaches_Distract, 1);
                } else {
                    main_set_bits(objdata->bitSwappedToSeq, 1);
                }
            } else {
                // Going to SwapStone Circle
                gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_CIRCLE, 0, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 2);
                main_set_bits(objdata->bitSwappedToSeq, 1);
            }
            warpPlayer(sSwapStoneWarps[playerno], /*fadeToBlack=*/FALSE);
            break;
        case 7:
            // Warp to Warlock Mountain
            playerno = gDLL_29_Gplay->vtbl->get_playerno();
            switch (SHswapstone_get_held_spirit()) {
            case PLAYER_SPIRIT_2:
            case PLAYER_SPIRIT_4:
            case PLAYER_SPIRIT_5:
            case PLAYER_SPIRIT_7:
                // Sabre has a spirit
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_ICE_MOUNTAIN_1, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_SNOWHORN_WASTES, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_WARLOCK_MOUNTAIN, 3);
                break;
            case PLAYER_SPIRIT_1:
            case PLAYER_SPIRIT_3:
            case PLAYER_SPIRIT_6:
            case PLAYER_SPIRIT_8:
                // Krystal has a spirit
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_ICE_MOUNTAIN_1, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_SNOWHORN_WASTES, 1);
                gDLL_29_Gplay->vtbl->set_map_setup(MAP_WARLOCK_MOUNTAIN, 2);
                break;
            }
            warpPlayer(sWarlockMountainWarps[playerno], /*fadeToBlack=*/FALSE);
            break;
        case 10:
            objdata->bShowScreen ^= 1;
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
    if (objdata->bShowScreen != 0) {
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
    SHswapstone_Data* objdata;
    s8 joyXSign;
    s8 joyYSign;

    objdata = self->data;
    get_joystick_menu_xy_sign(0, &joyXSign, &joyYSign);
    switch (arg2) {
    case 20:
        if ((joyXSign < 0) && !(objdata->flags & (SWAPSTONE_PLAYER_HAS_SPIRIT | SWAPSTONE_PLAYER_HAS_SPELLSTONE))) {
            return 1;
        }
    default:
        break;
    case 21:
        if ((joyXSign < 0) && (objdata->flags & (SWAPSTONE_PLAYER_HAS_SPIRIT | SWAPSTONE_PLAYER_HAS_SPELLSTONE))) {
            return 1;
        }
        break;
    case 22:
        if ((joyXSign > 0) && (objdata->flags & SWAPSTONE_PLAYER_HAS_SPIRIT)) {
            return 1;
        }
        break;
    case 23:
        if ((joyXSign > 0) && !(objdata->flags & SWAPSTONE_PLAYER_HAS_SPIRIT)) {
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
static s32 SHswapstone_get_held_spirit(void) {
    s32 playerno;
    Object* player;
    s32 spiritBits;

    playerno = gDLL_29_Gplay->vtbl->get_playerno();
    player = get_player();
    spiritBits = ((DLL_210_Player*)player->dll)->vtbl->func38(player, PLAYER_SPIRIT_ANY);
    if (playerno == PLAYER_SABRE) {
        if (spiritBits & PLAYER_SPIRIT_2) {
            return PLAYER_SPIRIT_2;
        }
        if (spiritBits & PLAYER_SPIRIT_4) {
            return PLAYER_SPIRIT_4;
        }
        if (spiritBits & PLAYER_SPIRIT_7) {
            return PLAYER_SPIRIT_7;
        }
        if (spiritBits & PLAYER_SPIRIT_5) {
            return PLAYER_SPIRIT_5;
        }
    } else {
        if (spiritBits & PLAYER_SPIRIT_1) {
            return PLAYER_SPIRIT_1;
        }
        if (spiritBits & PLAYER_SPIRIT_3) {
            return PLAYER_SPIRIT_3;
        }
        if (spiritBits & PLAYER_SPIRIT_6) {
            return PLAYER_SPIRIT_6;
        }
        if (spiritBits & PLAYER_SPIRIT_8) {
            return PLAYER_SPIRIT_8;
        }
    }
    
    return PLAYER_NO_SPIRIT;
}

// offset: 0xD0C | func: 11
// Whether the player has a SpellStone.
// Only checks for the first two!
static s32 SHswapstone_has_spellstone(void) {
    if (main_get_bits(BIT_SpellStone_CRF) != 0) {
        return 1;
    }
    if (main_get_bits(BIT_SpellStone_DIM) != 0) {
        return 1;
    }
    return 0;
}
