#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/engine/28_screen_fade.h"
#include "game/gamebits.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "functions.h"
#include "dll.h"
#include "types.h"

static s32 data_0 = 0;
static s8 data_4 = 0;
static u32 data_8 = 0xe49c44ff;

static s8 bss_0;
static s8 bss_1;
static s8 bss_2;
static f32 bss_4;
static f32 bss_8;
static Texture *bss_C;
static Texture *bss_10;
static Texture *bss_14;
static Texture *bss_18;

void dll_61_ctor(void *self) {
    u8 temp[4];
    *((u32*)temp) = data_8; // wat

    func_800379D0(0, 0, 0);
    func_80010018(0);
    data_0 = 0;
    data_4 = 0;
    bss_0 = 0;
    bss_1 = 0;
    bss_2 = 0;
    bss_C = queue_load_texture_proxy(0x2d6);
    bss_10 = queue_load_texture_proxy(0x2d7);
    bss_14 = queue_load_texture_proxy(0x2d4);
    bss_18 = queue_load_texture_proxy(0x2d5);
}

void dll_61_dtor(void *self) {
    texture_destroy(bss_C);
    texture_destroy(bss_10);
    texture_destroy(bss_14);
    texture_destroy(bss_18);
}

s32 dll_61_update1() {
    s32 delay;

    delay = delayByte;
    if (delay > 3) {
        delay = 3;
    }

    if (bss_1 > 0) {
        bss_1 -= delay;
    }

    if (bss_2 != 0) {
        main_set_bits(BIT_44F, 0);
        //@bug? Skips directly to Game Select screen, likely for developer convenience
        menu_set(MENU_GAME_SELECT);
    }

    data_0 += delayByte;
    if (data_0 > 620) {
        bss_0 = 1;
    }

    if (bss_0 != 0) {
        gDLL_28_ScreenFade->vtbl->fade(30, SCREEN_FADE_BLACK);
        bss_1 = 45;
        bss_2 = 1;
    }

    if (data_4 > 0) {
        bss_4 -= delayFloat;
    }
    if (data_4 > 2) {
        bss_8 -= delayFloat;
    }

    return 0;
}

void dll_61_update2() { }

void dll_61_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    f32 var1;
    u8 _stackPad[4];

    if (bss_2 != 0 && bss_1 < 11) {
        return;
    }

    func_80037A14(gdl, mtxs, 1);
    gDLL_76->vtbl->func2(gdl, mtxs);

    if (data_0 > 40 && data_4 == 0) {
        data_4 = 1;
        bss_4 = 500.0f;
    }

    if (data_0 > 50 && data_4 == 1) {
        data_4 = 2;
    }

    if (data_0 > 285 && data_4 == 2) {
        data_4 = 3;
        bss_8 = 145.0f;
    }

    if (data_4 > 2) {
        if (bss_8 <= 72.0f) {
            var1 = 1.0f - ((72.0f - bss_8) / 72.0f);
        } else {
            var1 = 1.0f - ((bss_8 - 72.0f) / 72.0f);
        }

        if (var1 < 0.0f) {
            var1 = 0.0f;
        } else if (var1 > 1.0f) {
            var1 = 1.0f;
        }

        func_8003825C(gdl, bss_10, 0xa5, 0x94, 0, 0, (s16)(255.0f * var1), 0);
        func_8003825C(gdl, bss_18, 0x66, 0xfa, 0, 0, (s16)(255.0f * var1), 0);
    }

    if (data_4 >= 1) {
        if (bss_4 <= 125.0f) {
            var1 = 1.0f - ((125.0f - bss_4) / 125.0f);
        } else {
            var1 = 1.0f - ((bss_4 - 375.0f) / 125.0f);
        }

        if (var1 < 0.0f) {
            var1 = 0.0f;
        } else if (var1 > 1.0f) {
            var1 = 1.0f;
        }

        func_8003825C(gdl, bss_C, 0xa5, 0x94, 0, 0, (u32)(255.0f * var1) & 0xFF, 0);
        func_8003825C(gdl, bss_14, 0x66, 0xfa, 0, 0, (u32)(255.0f * var1) & 0xFF, 0);
    }
}
