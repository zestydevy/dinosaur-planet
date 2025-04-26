#include "sys/menu.h"

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/dll.h"
#include "sys/fonts.h"
#include "sys/gfx/gx.h"
#include "sys/main.h"
#include "dll.h"
#include "dll_def.h"
#include "functions.h"
#include "bss.h"

s32 gMenuDLLIDs[] = {
    -1,
    19,
    60,
    61,
    62,
    63,
    64,
    65,
    66,
    67,
    71,
    68,
    69,
    70,
    77,
    78,
    79,
    80
};

s32 D_8008C888 = -1;
s8 D_8008C88C = 0;
s32 D_8008C890 = 0;

BSS_GLOBAL s32 D_800A7D50;
BSS_GLOBAL DLLInst_Menu *gActiveMenuDLL;
BSS_GLOBAL s32 D_800A7D58;
BSS_GLOBAL s32 D_800A7D5C;
BSS_GLOBAL s32 D_800A7D60;
BSS_GLOBAL s32 D_800A7D64;
BSS_GLOBAL u8 __osBbCardChange;
BSS_GLOBAL u8 D_800A7D69;
BSS_GLOBAL s32 D_800A7D6C;
BSS_GLOBAL f32 D_800A7D70;
BSS_GLOBAL f32 D_800A7D74;
BSS_GLOBAL s8 D_800A7D78;
BSS_GLOBAL s8 D_800A7D79;
BSS_GLOBAL s32 D_800A7D7C;
BSS_GLOBAL s32 gNextMenuID;
BSS_GLOBAL s32 gCurrentMenuID;
BSS_GLOBAL s32 gPreviousMenuID;
BSS_GLOBAL s16 D_800A7D8C;
BSS_GLOBAL s16 D_800A7D8E;
BSS_GLOBAL s16 D_800A7D90;
BSS_GLOBAL s16 D_800A7D92;
BSS_GLOBAL s8 D_800A7D94;
BSS_GLOBAL char D_800A7D98[8];
BSS_GLOBAL char D_800A7DA0[8];

void menu_do_menu_swap();
void func_80010088();

void menu_init() {
    gActiveMenuDLL = NULL;
    D_800A7D58 = 0;
    D_800A7D5C = 0;
    D_800A7D60 = -1;
    D_800A7D64 = 0;
    gNextMenuID = 0;
    gPreviousMenuID = 0;
    gCurrentMenuID = 0;
    D_800A7D94 = 0;
    D_800A7D78 = 1;
    D_800A7D79 = 0;
    D_800A7D70 = 0.0f;
    D_800A7D74 = 0.0f;
    D_800A7D7C = 0;
}

void menu_set(s32 id) {
    if (id != gCurrentMenuID) {
        gNextMenuID = id + 1;

        if (gActiveMenuDLL == NULL) {
            menu_do_menu_swap();
        }
    }
}

s32 menu_get_current() {
    return gCurrentMenuID;
}

DLLInst_Menu *menu_get_active_dll() {
    return gActiveMenuDLL;
}

s32 menu_get_previous() {
    return gPreviousMenuID;
}

void menu_do_menu_swap() {
    if (gNextMenuID != 0) {
        gNextMenuID -= 1;

        if (gNextMenuID == MENU_2) {
            D_800A7D50 = 0;
        }

        if (gCurrentMenuID == MENU_NONE) {
            font_window_flush_strings(1);
        }

        gPreviousMenuID = gCurrentMenuID;

        if (gActiveMenuDLL != NULL) {
            dll_unload(gActiveMenuDLL);
            gActiveMenuDLL = NULL;
        }

        if (gMenuDLLIDs[gNextMenuID] != -1) {
            gActiveMenuDLL = (DLLInst_Menu*)dll_load_deferred(gMenuDLLIDs[gNextMenuID], 1);
        } else {
            gActiveMenuDLL = NULL;
            gNextMenuID = 0;
        }

        gCurrentMenuID = gNextMenuID;
        gNextMenuID = 0;
    }
}

s32 menu_update1() {
    s32 ret;

    ret = 0;

    func_80010088();

    if (gActiveMenuDLL != NULL) {
        ret = gActiveMenuDLL->exports->update1();
    }

    menu_do_menu_swap();

    return ret;
}

void menu_update2() {
    if (gActiveMenuDLL != NULL) {
        gActiveMenuDLL->exports->update2();
    }

    menu_do_menu_swap();
}

void menu_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols) {
    if (gActiveMenuDLL != NULL) {
        gActiveMenuDLL->exports->draw(gdl, mtxs, vtxs);
    }
}

void func_8000F640(s32 param1) { }

void func_8000F64C(s8 param1, s32 param2) {
    param2 *= 60;

    D_800A7D79 = param1;

    if (param1 & 1) {
        D_800A7D70 = param2;
    } else {
        D_800A7D70 = 0.0f;
    }

    D_800A7D74 = param2;
    D_800A7D6C = 1;
    D_800A7D78 = 0;
    D_800A7D94 = param1 & 3;
}

void func_8000F6CC() {
    if (D_800A7D6C == 0) {
        return;
    }

    D_800A7D6C = 0;

    if ((D_800A7D79 & 4)) {
        if (D_800A7D7C != 0) {
            gDLL_AMSFX->exports->func6(D_800A7D7C);
        }

        D_800A7D7C = gDLL_AMSFX->exports->func2(0, 0x775, 0x7F, 0, 0, 0, 0);
    } else {
        D_800A7D7C = 0;
    }

    if ((D_800A7D79 & 1)) {
        gDLL_AMSFX->exports->func5(D_800A7D7C, 1.3f - (D_800A7D70 / D_800A7D74) * 0.6f);
        gDLL_AMSFX->exports->func4(D_800A7D7C, (127 - (u8)((D_800A7D70 / D_800A7D74) * 80.0f)));
    } else {
        gDLL_AMSFX->exports->func5(D_800A7D7C, (D_800A7D70 / D_800A7D74) * 0.6f + 0.69999999f);
        gDLL_AMSFX->exports->func4(D_800A7D7C, ((u8)((D_800A7D70 / D_800A7D74) * 80.0f)) + 47);
    }
}

void func_8000F9DC() {
    D_800A7D6C = 1;

    if (D_800A7D7C != 0) {
        gDLL_AMSFX->exports->func6(D_800A7D7C);
        D_800A7D7C = 0;
    }
}

void func_8000FA2C() {
    D_800A7D94 = 0;

    if (D_800A7D79 & 8) {
        gDLL_AMSFX->exports->func2(0, 0x242, 0x7f, 0, 0, 0, 0);
    }

    if (D_800A7D7C != 0) {
        gDLL_AMSFX->exports->func6(D_800A7D7C);
        D_800A7D7C = 0;
    }

    D_800A7D79 = 0;
}

void func_8000FAC8() {
    D_800A7D94 = 0;
    
    if (D_800A7D7C != 0) {
        gDLL_AMSFX->exports->func6(D_800A7D7C);
        D_800A7D7C = 0;
    }

    D_800A7D79 = 0;
}

s32 func_8000FB1C() {
    return D_800A7D78;
}

void func_8000FB2C(Gfx **gdl) {
    s32 _stackPad[2];
    f32 delay;
    s32 bvar;

    delay = delayFloat;
    if (D_800A7D6C != 0 || get_pause_state() != 0) {
        delay = 0.0f;
    }

    bvar = FALSE;
    if (D_800A7D94 == 1) {
        D_800A7D70 -= delay;

        if (D_800A7D70 <= 0.0f) {
            bvar = TRUE;
            D_800A7D70 = 0.0f;
        }
    } else {
        if (0) {}

        D_800A7D70 += delay;

        if (D_800A7D70 > D_800A7D74) {
            bvar = TRUE;
            D_800A7D70 = D_800A7D74;
        }
    }

    if (bvar) {
        if (D_800A7D79 & 8) {
            gDLL_AMSFX->exports->func2(0, 0x242, 0x7f, 0, 0, 0, 0);
        }

        if (D_800A7D7C != 0) {
            gDLL_AMSFX->exports->func6(D_800A7D7C);
            D_800A7D7C = 0;
        }

        D_800A7D94 = 0;
        D_800A7D78 = 1;
        D_800A7D79 = 0;
    }

    if (D_800A7D7C != 0) {
        if ((D_800A7D79 & 1)) {
            gDLL_AMSFX->exports->func5(D_800A7D7C, 1.3f - (D_800A7D70 / D_800A7D74) * 0.6f);
            gDLL_AMSFX->exports->func4(D_800A7D7C, (127 - (u8)((D_800A7D70 / D_800A7D74) * 80.0f)));
        } else {
            gDLL_AMSFX->exports->func5(D_800A7D7C, (D_800A7D70 / D_800A7D74) * 0.6f + 0.69999999f);
            gDLL_AMSFX->exports->func4(D_800A7D7C, ((u8)((D_800A7D70 / D_800A7D74) * 80.0f)) + 47);
        }
    }

    if (D_800A7D79 & 0x10) {
        font_window_set_coords(2, 130, 25, 190, 50);
        font_window_use_font(2, FONT_DINO_SUBTITLE_FONT_1);
        font_window_set_bg_colour(2, 0, 0, 0, 128);
        font_window_flush_strings(2);
        sprintf(D_800A7D98, "%d", (int)D_800A7D70 / 60);
        font_window_add_string_xy(2, 20, 10, D_800A7D98, 1, ALIGN_TOP_RIGHT);
        sprintf(D_800A7DA0, ".%02d", (int)D_800A7D70 % 60);
        font_window_add_string_xy(2, 45, 10, D_800A7DA0, 1, ALIGN_TOP_RIGHT);
        font_window_draw(gdl, NULL, NULL, 2);
    }
}

s32 func_80010008() {
    return D_8008C888;
}

void func_80010018(s32 param1) {
    D_8008C888 = param1;
}

s32 func_80010028() {
    return D_8008C88C;
}

void func_80010038(s32 param1) {
    D_8008C88C = param1;
}

s32 func_80010048() {
    return D_8008C890;
}

void func_80010058(s32 param1) {
    D_8008C890 = param1;
}

void func_80010068(s32 param1) {
    __osBbCardChange = param1;
}

s32  osBbCardChange() {
    return __osBbCardChange;
}

void func_80010088() {
    u32 res;

    res = get_some_resolution_encoded();
    
    D_800A7D8C = RESOLUTION_WIDTH(res);
    D_800A7D8E = 0;
    D_800A7D90 = RESOLUTION_HEIGHT(res);
    D_800A7D92 = 0;
    D_800A7D69 = 0;
}

void func_800100D4(s32 param1, s32 param2, s32 param3, s32 param4) {
    if (param1 < D_800A7D8C) {
        D_800A7D8C = param1;
    }

    if (param2 > D_800A7D8E) {
        D_800A7D8E = param2;
    }

    if (param3 < D_800A7D90) {
        D_800A7D90 = param3;
    }

    if (param4 > D_800A7D92) {
        D_800A7D92 = param4;
    }

    D_800A7D69 = 1;
}

void func_80010158(s32 *param1, s32 *param2, s32 *param3, s32 *param4) {
    if (D_800A7D69 != 0) {
        *param1 = D_800A7D8C;
        *param2 = D_800A7D8E;
        *param3 = D_800A7D90;
        *param4 = D_800A7D92;
    } else {
        u32 res = get_some_resolution_encoded();

        *param1 = 0;
        *param2 = RESOLUTION_WIDTH(res);
        *param3 = 0;
        *param4 = RESOLUTION_HEIGHT(res);
    }
}
