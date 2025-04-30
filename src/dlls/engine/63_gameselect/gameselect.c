#include "dlls/engine/63_gameselect.h"

#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/28_screen_fade.h"
#include "dlls/engine/74_picmenu.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/texture.h"
#include "sys/fonts.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/memory.h"
#include "dll.h"
#include "types.h"
#include "functions.h"
#include "macros.h"
#include "prevent_bss_reordering.h"

static PicMenuItem data_0[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 158, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 146, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ 1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 212, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 200, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 266, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 254, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ 1, /*downLink*/ 3, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 324, /*innerWidth*/ 0, 
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_AUTO_WIDTH, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ 2, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ 4, /*overrideWith*/ -1),
    /*4*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 324, /*innerWidth*/ 0, 
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_INLINE | PICMENU_AUTO_WIDTH, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ 2, /*downLink*/ -1, /*leftLink*/ 3, /*rightLink*/ -1,  /*overrideWith*/ -1
    )
};
static s8 data_12C[] = { -1, -2, -3, 5, 6 };

static PicMenuItem data_134[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 158, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 146, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ 1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 212, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 200, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 266, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 254, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ 1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    )
};
static s8 data_1E8[] = { -1, -2, -3 };

static PicMenuItem data_1EC[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 284, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 272, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ 1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 338, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 326, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ 0, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    )
};
static s8 data_264[] = { -1, -2 };

static PicMenuItem data_268[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0, 
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_AUTO_WIDTH, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ 1, /*overrideWith*/ -1
    ),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0, 
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_INLINE | PICMENU_AUTO_WIDTH, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 0, /*rightLink*/ -1, /*overrideWith*/ -1
    )
};
static s8 data_2E0[] = { 1, 2 };

static PicMenuItem data_2E4[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 158, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 146, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ 1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 212, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 200, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 266, /*innerWidth*/ 320, 
        /*itemX*/ 58, /*itemY*/ 254, /*textureID*/ -1, /*outerWidth*/ 320, 
        /*flags*/ PICMENU_HAS_BACKGROUND, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ 1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    )
};
static s8 data_398[] = { -1, -2, -3 };

static PicMenuItem data_39C[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0, 
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_AUTO_WIDTH, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ 1, /*overrideWith*/ -1
    ),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0, 
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_INLINE | PICMENU_AUTO_WIDTH, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 0, /*rightLink*/ -1, /*overrideWith*/ -1
    )
};
static s8 data_414[] = { 1, 2 };

static PicMenuItem data_418[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0, 
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_AUTO_WIDTH, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ 1, /*overrideWith*/ -1
    ),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0, 
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_INLINE | PICMENU_AUTO_WIDTH, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 0, /*rightLink*/ -1, /*overrideWith*/ -1
    )
};
static s8 data_490[] = { 1, 2 };

static PicMenuItem data_494[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 320, /*textY*/ 382, /*innerWidth*/ 0, 
        /*itemX*/ 320, /*itemY*/ 370, /*textureID*/ -1, /*outerWidth*/ 0, 
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_AUTO_WIDTH | PICMENU_ALIGN_ITEM_CENTER, 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1,
        /*overrideWith*/ -1
    )
};
static s8 data_4D0[] = { 14 };

static DLL63Struct2 data_4D4[] = {
    { data_0,   data_12C, /*count*/5, 0x03, 0x00, 0x02 }, 
    { data_418, data_490, /*count*/2, 0x07, 0x00, 0x02 }, 
    { data_134, data_1E8, /*count*/3, 0x08, 0x00, 0x02 }, 
    { data_1EC, data_264, /*count*/2, 0x09, 0x00, 0x02 }, 
    { data_268, data_2E0, /*count*/2, 0x0a, 0x00, 0x02 }, 
    { data_2E4, data_398, /*count*/3, 0x0b, 0x00, 0x02 }, 
    { data_39C, data_414, /*count*/2, 0x0c, 0x00, 0x02 }, 
    { data_494, data_4D0, /*count*/1, 0x0d, 0xff, 0x02 }
};

static PicMenuSounds data_534 = {
    /*move*/   0x5D5, 
    /*select*/ 0x5D6, 
    /*back*/   0x5D4
};
static GameTextChunk *data_540 = NULL;
static s8 data_544 = -1;
static s8 data_548 = -1;
static s8 data_54C = -1;
static s16 data_550[] = {
    0x031d, 0x031c, 
    0x031f, 0x031e
};
static s16 data_558[] = {
    0x0320, 0x02f5, 
    0x02f6, 0x02f7, 
    0x02f8, 0x02f9, 
    0x02fa, 0x02fb, 
    0x02fc, 0x0313, 
    0x02fd, 0x02fe, 
    0x02ff, 0x0300, 
    0x0301, 0x0302, 
    0x0303, 0x0304
};
static s16 data_57C[] = {
    0x0004, 0x0011, 
    0x0010, 0x0011, 
    0x0005, 0xffff, 
    0x0006, 0x000a, 
    0x0009, 0x000b, 
    0x000d, 0xffff, 
    0x0007, 0x000b, 
    0x000c, 0x0009, 
    0x000e, 0xffff, 
    0x0002, 0x0001, 
    0x0000, 0x0001, 
    0x0003, 0xffff
};

static DLL63Struct bss_0[3];
static s8 bss_36[2];
static s8 bss_38;
static s8 bss_39;
static s16 bss_3A;
static s16 bss_3C;
static char bss_40[10];
static char bss_4A[2];
static char bss_4C[2];
static u8 bss_4E;
static u8 bss_4F;
static Texture *bss_50;
static Texture *bss_54;
static Texture *bss_58;
static char bss_60[4][4];
static Texture *bss_70[4];
static Texture *bss_80[18];

static void dll_63_func_E28(s32 param1);
static void dll_63_func_FD4(s32 param1);
static void dll_63_func_10F4();
static void dll_63_func_12C4();
static void dll_63_func_1B34(DLL63Struct2 *param1);

static void dll_63_func_1C9C(s32 param1, s32 param2);
static void dll_63_func_1DC0(s32 param1, s32 param2);
static void dll_63_func_1F4C(s32 param1, s32 param2);
static void dll_63_func_2074(s32 param1, s32 param2);
static void dll_63_func_20F0(s32 param1, s32 param2);
static void dll_63_func_2160(s32 param1, s32 param2);
static void dll_63_func_21F4(s32 param1, s32 param2);
static void dll_63_func_2270(s32 param1, s32 param2);

static void dll_63_func_22FC(Gfx **gdl, s32 x, s32 y, DLL63Struct *param4);

void dll_63_ctor(void *self) {
    s32 i;

    bss_50 = queue_load_texture_proxy(0x2DD);
    bss_54 = queue_load_texture_proxy(0xC5);
    bss_58 = queue_load_texture_proxy(0x2E1);

    if (data_540 == NULL) {
        data_540 = gDLL_21_gametext->exports->get_chunk(0xEC);
    }

    for (i = 0; i < 18; i++) {
        bss_80[i] = queue_load_texture_proxy(data_558[i]);
    }

    for (i = 0; i < 4; i++) {
        bss_70[i] = queue_load_texture_proxy(data_550[i]);
    }

    if (menu_get_previous() != MENU_6) {
        gDLL_28_screen_fade->exports->fade_reversed(20, SCREEN_FADE_BLACK);
        dll_63_func_FD4(1);
    } else {
        data_548 = osBbCardChange();
        bss_3A = 56;
        bss_3C = 179;
        dll_63_func_10F4();
        dll_63_func_12C4();
    }

    bss_4E = bss_4F = 0;
    bss_38 = 0;
    bss_39 = 4;
}

void dll_63_dtor(void *self) { }

// func 0 export 0
s32 dll_63_func_220() {
    s32 var2;
    s32 selected;
    s8 prev_bss_38;
    s32 delay;

    prev_bss_38 = bss_38;

    delay = delayByte;
    if (delay > 3) {
        delay = 3;
    }

    if (bss_38 > 0) {
        bss_38 -= delay;
    }

    if (gDLL_28_screen_fade->exports->is_complete() == 0) {
        gDLL_picmenu->exports->redraw_all();
        bss_39 = 4;
    }

    if (bss_4E != 0 || bss_4F != 0) {
        if (prev_bss_38 >= 13 && bss_38 <= 12) {
            func_8005D410(1, get_ossched(), 0);
            if (bss_4E != 0) {
                dll_63_func_E28(1);
            } else {
                dll_63_func_E28(0);
            }
            func_80041D20(1);
            func_80041C6C(1);
        } else if (bss_38 < 1) {
            if (bss_4E != 0) {
                gDLL_29_gplay->exports->func_AE0();
            } else {
                func_80014BBC();
                func_800142F0(12457.1f, -1474.875f, -6690.398f, 1);
                menu_set(MENU_4);
            }
        }

        if (bss_38 <= 12) {
            return 1;
        } else {
            return 0;
        }
    } else {
        var2 = gDLL_picmenu->exports->update();
        if (var2 != PICMENU_UPDATE_NONE) {
            selected =  gDLL_picmenu->exports->get_selected_item();

            switch (data_544) {
                case 0:
                    dll_63_func_1C9C(var2, selected);
                    break;
                case 1:
                    dll_63_func_1DC0(var2, selected);
                    break;
                case 2:
                    dll_63_func_2074(var2, selected);
                    break;
                case 3:
                    dll_63_func_20F0(var2, selected);
                    break;
                case 4:
                    dll_63_func_2160(var2, selected);
                    break;
                case 5:
                    dll_63_func_21F4(var2, selected);
                    break;
                case 6:
                    dll_63_func_2270(var2, selected);
                    break;
                case 7:
                    dll_63_func_1F4C(var2, selected);
                    break;
            }
        }

        return 0;
    }
}

// func 1 export 1
void dll_63_func_5A0() { }

// func 2 export 2
void dll_63_func_5A8(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    s32 tasks;
    s32 uly;
    s32 lry;
    s32 ulx;
    s32 lrx;
    s32 var9;
    s32 i;
    DLL63Struct2 *var5;

    var5 = &data_4D4[data_544];

    if ((bss_4E == 0 && bss_4F == 0) || bss_38 > 10) {
        font_window_set_coords(1, 0, 0, 
            RESOLUTION_WIDTH(get_some_resolution_encoded()) - 100, 
            RESOLUTION_HEIGHT(get_some_resolution_encoded()));
        font_window_flush_strings(1);

        font_window_set_coords(3, 105, 0, 
            RESOLUTION_WIDTH(get_some_resolution_encoded()) - 200, 
            RESOLUTION_HEIGHT(get_some_resolution_encoded()));
        font_window_flush_strings(3);

        if (bss_39 != 0) {
            func_8003825C(gdl, bss_50, 0, 0, 0, 0, 0xFF, 2);

            if (data_544 == 7) {
                func_8003825C(gdl, bss_58, 0x77, 0x5c, 0, 0, 0xFF, 0);
                func_8003825C(gdl, bss_54, 0x81, 100, 0, 0, 0xFF, 0);

                tasks = gDLL_30_task->exports->get_num_recently_completed();
                if (tasks > 3) {
                    tasks = 3;
                }

                font_window_enable_wordwrap(3);
                font_window_use_font(1, FONT_FUN_FONT);
                font_window_use_font(3, FONT_FUN_FONT);
                font_window_set_text_colour(1, 183, 139, 97, 255, 255);
                font_window_set_text_colour(3, 183, 139, 97, 255, 255);

                var9 = 232;
                for (i = 0; i < tasks; i++) {
                    sprintf(bss_60[i], "%1d.", (int)(i + 1));
                    font_window_add_string_xy(1, 0x4b, var9, bss_60[i], 1, ALIGN_TOP_LEFT);
                    font_window_add_string_xy(3, 2, var9, gDLL_30_task->exports->get_recently_completed_task_text(i), 1, ALIGN_TOP_LEFT);
                    var9 += 40;
                }

                var9 = 232;
                font_window_set_text_colour(1, 0, 0, 0, 255, 255);
                font_window_set_text_colour(3, 0, 0, 0, 255, 255);
                for (i = 0; i < tasks; i++) {
                    sprintf(bss_60[i], "%1d.", (int)(i + 1));
                    font_window_add_string_xy(1, 0x49, var9 - 2, bss_60[i], 1, ALIGN_TOP_LEFT);
                    font_window_add_string_xy(3, 0, var9 - 2, gDLL_30_task->exports->get_recently_completed_task_text(i), 1, ALIGN_TOP_LEFT);
                    var9 += 40;
                }
            } else {
                if (data_548 != -1) {
                    dll_63_func_22FC(gdl, bss_3A, bss_3C, &bss_0[data_548]);
                }

                font_window_use_font(1, FONT_FUN_FONT);
                font_window_set_text_colour(1, 183, 139, 97, 255, 255);

                font_window_add_string_xy(1, 0x140, 0x195, data_540->strings[var5->unkA], 1, ALIGN_TOP_CENTER);
                font_window_set_text_colour(1, 0, 0, 0, 255, 255);
                font_window_add_string_xy(1, 0x13e, 0x193, data_540->strings[var5->unkA], 2, ALIGN_TOP_CENTER);
            }

            font_window_set_coords(2, 0, 0, 
                RESOLUTION_WIDTH(get_some_resolution_encoded()) - 100, 
                RESOLUTION_HEIGHT(get_some_resolution_encoded()));
            font_window_flush_strings(2);
            font_window_use_font(2, FONT_DINO_MEDIUM_FONT_IN);
            font_window_enable_wordwrap(2);
            font_window_set_text_colour(2, 255, 255, 255, 0, 255);

            font_window_add_string_xy(2, 0x45, 0x3d, data_540->strings[var5->unk9], 1, ALIGN_TOP_LEFT);
            font_window_set_text_colour(2, 0, 0, 0, 255, 255);
            font_window_add_string_xy(2, 0x40, 0x38, data_540->strings[var5->unk9], 2, ALIGN_TOP_LEFT);

            font_window_draw(gdl, NULL, NULL, 2);
        } else {
            func_80010158(&ulx, &lrx, &uly, &lry);
            func_800382AC(gdl, bss_50, 0, 0, uly, lry, 0xFF, 2);
        }

        gDLL_picmenu->exports->draw(gdl);

        font_window_draw(gdl, NULL, NULL, 1);
        font_window_draw(gdl, NULL, NULL, 3);

        bss_39 -= 1;
        if (bss_39 < 0) {
            bss_39 = 0;
        }
    }
}

// func 3
static void dll_63_func_E28(s32 param1) {
    s32 i;

    if (data_540 != NULL) {
        free(data_540);
        data_540 = NULL;
    }

    if (data_544 != -1) {
        gDLL_picmenu->exports->clear_items();
        data_544 = -1;
    }

    for (i = 0; i < 18; i++) {
        if (bss_80[i] != NULL) {
            texture_destroy(bss_80[i]);
            bss_80[i] = NULL;
        }
    }

    for (i = 0; i < 4; i++) {
        if (bss_70[i] != NULL) {
            texture_destroy(bss_70[i]);
            bss_70[i] = NULL;
        }
    }

    texture_destroy(bss_50);
    texture_destroy(bss_54);
    texture_destroy(bss_58);

    if (param1 != 0) {
        font_unload(FONT_DINO_MEDIUM_FONT_IN);
        font_unload(FONT_DINO_MEDIUM_FONT_OUT);
        font_unload(FONT_FUN_FONT);
        func_80014B6C();
    }
}

// func 4
static void dll_63_func_FD4(s32 param1) {
    DLL63Struct2 *submenu;

    if (data_544 != -1) {
        gDLL_picmenu->exports->clear_items();
    }

    data_544 = 0;
    data_548 = -1;
    submenu = &data_4D4[data_544];

    dll_63_func_10F4();

    dll_63_func_1B34(submenu);
    gDLL_picmenu->exports->set_items(submenu->menuItems, submenu->count,
        /*defaultItem*/ 0,
        /*sounds*/ NULL,
        /*param5*/ 5,
        /*param6*/ 4,
        /*textColor*/ 0, 0, 0,
        /*textHighlight*/ 0, 0, 0);
    
    bss_39 = 2;
}

// func 5
static void dll_63_func_10F4() {
    s32 i;
    GplayStruct7 *gplayStruct;
    char *ptr;

    for (i = 0; i < 3; i++) {
        if ((u8)gDLL_29_gplay->exports->func_3E4(i, 0) == 0) {
            gDLL_29_gplay->exports->func_110(i);
            bzero(&bss_0[i], sizeof(DLL63Struct));
            bss_0[i].unk0 = 1;
        } else {
            gplayStruct = gDLL_29_gplay->exports->func_E74();

            if (gplayStruct->unk0.unk0.unk0.unk0x2f6 == 0) {
                bss_0[i].character = gplayStruct->unk0.unk0.unk0.character;
                bss_0[i].unk2 = get_gplay_bitstring(0x489);
                bss_0[i].unk3 = 0;

                ptr = bss_0[i].unkC;

                gDLL_7_newday->exports->func11(
                    gplayStruct->unk0.unk0.unk0.unk0x2fc,
                    &bss_0[i].unk4, &bss_0[i].unk6, &bss_0[i].unk8);

                bss_0[i].unkA = 0;
                bss_0[i].unk0 = 0;

                bcopy(gplayStruct->unk0.unk0.unk0.unk0x2F0, ptr, 5);
            } else {
                bzero(&bss_0[i], sizeof(DLL63Struct));
                bss_0[i].unk0 = 1;
            }
        }
    }
}

// func 6
static void dll_63_func_12C4() {
    DLL63Struct2 *submenu;

    if (data_544 != -1) {
        gDLL_picmenu->exports->clear_items();
    }

    data_544 = 1;
    submenu = &data_4D4[data_544];

    dll_63_func_1B34(submenu);
    gDLL_picmenu->exports->set_items(submenu->menuItems, submenu->count,
        /*defaultItem*/ 0,
        /*sounds*/ NULL,
        /*param5*/ 5,
        /*param6*/ 4,
        /*textColor*/ 0, 0, 0,
        /*textHighlight*/ 0, 0, 0);
    
    bss_39 = 2;
}

// func 7
static void dll_63_func_13C0() {
    DLL63Struct2 *submenu;
    s32 i;

    if (data_544 != -1) {
        gDLL_picmenu->exports->clear_items();
    }

    data_544 = 2;
    data_548 = -1;
    submenu = &data_4D4[data_544];

    for (i = 0; i < 3; i++) {
        if (bss_0[i].unk0 != 0) {
            submenu->menuItems[i].flags |= (PICMENU_TRANSPARENT | PICMENU_DISABLED);
        } else {
            submenu->menuItems[i].flags &= ~(PICMENU_TRANSPARENT | PICMENU_DISABLED);
        }
    }

    dll_63_func_1B34(submenu);
    gDLL_picmenu->exports->set_items(submenu->menuItems, submenu->count,
        /*defaultItem*/ 0,
        /*sounds*/ NULL,
        /*param5*/ 5,
        /*param6*/ 4,
        /*textColor*/ 0, 0, 0,
        /*textHighlight*/ 0, 0, 0);
    
    bss_39 = 2;
}

// func 8
static void dll_63_func_1528() {
    DLL63Struct2 *submenu;
    s32 i;
    s32 k;
    s32 temp;

    if (data_544 != -1) {
        gDLL_picmenu->exports->clear_items();
    }

    data_544 = 3;
    submenu = &data_4D4[data_544];
    k = 0;
    for (i = 0; i < 3; i++) {
        if (i != data_548) {
            submenu->textIDs[k] = -i - 1;

            if (bss_0[i].unk0 == 0) {
                submenu->menuItems[k].flags |= (PICMENU_TRANSPARENT | PICMENU_DISABLED);
            } else {
                submenu->menuItems[k].flags &= ~(PICMENU_TRANSPARENT | PICMENU_DISABLED);
            }

            bss_36[k] = i;
            k++;
        }
    }

    dll_63_func_1B34(submenu);
    gDLL_picmenu->exports->set_items(submenu->menuItems, submenu->count,
        /*defaultItem*/ 0,
        /*sounds*/ NULL,
        /*param5*/ 5,
        /*param6*/ 4,
        /*textColor*/ 0, 0, 0,
        /*textHighlight*/ 0, 0, 0);
    
    bss_39 = 2;
}

// func 9
static void dll_63_func_16D8() {
    DLL63Struct2 *submenu;

    if (data_544 != -1) {
        gDLL_picmenu->exports->clear_items();
    }

    data_544 = 4;
    submenu = &data_4D4[data_544];

    dll_63_func_1B34(submenu);
    gDLL_picmenu->exports->set_items(submenu->menuItems, submenu->count,
        /*defaultItem*/ 0,
        /*sounds*/ NULL,
        /*param5*/ 5,
        /*param6*/ 4,
        /*textColor*/ 0, 0, 0,
        /*textHighlight*/ 0, 0, 0);
    
    bss_39 = 2;
}

// func 10
static void dll_63_func_17D4() {
    DLL63Struct2 *submenu;
    s32 i;

    if (data_544 != -1) {
        gDLL_picmenu->exports->clear_items();
    }

    data_544 = 5;
    data_548 = -1;
    submenu = &data_4D4[data_544];

    dll_63_func_1B34(submenu);

    for (i = 0; i < 3; i++) {
        if (bss_0[i].unk0 != 0) {
            submenu->menuItems[i].flags |= (PICMENU_TRANSPARENT | PICMENU_DISABLED);
        } else {
            submenu->menuItems[i].flags &= ~(PICMENU_TRANSPARENT | PICMENU_DISABLED);
        }
    }

    gDLL_picmenu->exports->set_items(submenu->menuItems, submenu->count,
        /*defaultItem*/ 0,
        /*sounds*/ NULL,
        /*param5*/ 5,
        /*param6*/ 4,
        /*textColor*/ 0, 0, 0,
        /*textHighlight*/ 0, 0, 0);
    
    bss_39 = 2;
}

// func 11
static void dll_63_func_1938() {
    DLL63Struct2 *submenu;

    if (data_544 != -1) {
        gDLL_picmenu->exports->clear_items();
    }

    data_544 = 6;
    submenu = &data_4D4[data_544];

    dll_63_func_1B34(submenu);
    gDLL_picmenu->exports->set_items(submenu->menuItems, submenu->count,
        /*defaultItem*/ 1,
        /*sounds*/ NULL,
        /*param5*/ 5,
        /*param6*/ 4,
        /*textColor*/ 0, 0, 0,
        /*textHighlight*/ 0, 0, 0);
    
    bss_39 = 2;
}

// func 12
static void dll_63_func_1A34() {
    DLL63Struct2 *submenu;

    if (data_544 != -1) {
        gDLL_picmenu->exports->clear_items();
    }

    data_544 = 7;
    submenu = &data_4D4[data_544];

    dll_63_func_1B34(submenu);
    gDLL_picmenu->exports->set_items(submenu->menuItems, submenu->count,
        /*defaultItem*/ 0,
        /*sounds*/ &data_534,
        /*param5*/ 5,
        /*param6*/ 4,
        /*textColor*/ 0, 0, 0,
        /*textHighlight*/ 0, 0, 0);
    
    bss_39 = 2;
}

// func 13
static void dll_63_func_1B34(DLL63Struct2 *param1) {
    s32 i;
    s32 textID;
    DLL63Struct *thing;

    for (i = 0; i < param1->count; i++) {
        textID = param1->textIDs[i];

        if (textID >= 0) {
            param1->menuItems[i].text = data_540->strings[textID];
        } else {
            thing = &bss_0[-textID - 1];

            if (thing->unk0 != 0) {
                param1->menuItems[i].text = data_540->strings[4];
                param1->menuItems[i].flags &= ~1;
                param1->menuItems[i].flags |= PICMENU_ALIGN_TEXT_CENTER;
                param1->menuItems[i].texture.asID = -1;
            } else {
                param1->menuItems[i].text = thing->unkC;
                param1->menuItems[i].flags &= ~PICMENU_ALIGN_TEXT_CENTER;
                param1->menuItems[i].flags |= 1;
                param1->menuItems[i].texture.asID = data_550[thing->character];
            }
        }
    }
}

// func 14
static void dll_63_func_1C9C(s32 param1, s32 param2) {
    switch (param2) {
        case 4:
            if (param1 == 1) {
                dll_63_func_17D4();
            }
            break;
        case 3:
            if (param1 == 1) {
                dll_63_func_13C0();
            }
            break;
        default:
            if (param1 == 1) {
                if (bss_0[param2].unk0 != 0) {
                    dll_63_func_E28(0);
                    func_80010068(param2);
                    menu_set(MENU_6);
                } else {
                    data_548 = param2;
                    bss_3A = 56;
                    bss_3C = 179;
                    dll_63_func_12C4();
                }
            }
            break;
    }
}

// func 15
static void dll_63_func_1DC0(s32 param1, s32 param2) {
    if (param1 == 0 || param2 == 1) {
        dll_63_func_FD4(0);
    } else {
        gDLL_29_gplay->exports->func_3E4(data_548, 0);
        
        gDLL_30_task->exports->load_recently_completed();

        if (gDLL_30_task->exports->get_num_recently_completed() != 0) {
            dll_63_func_1A34();
        } else {
            bss_4E = 1;
            gDLL_28_screen_fade->exports->fade(20, SCREEN_FADE_BLACK);
            gDLL_AMSEQ->exports->func[6].withOneArg(0);
            gDLL_AMSEQ->exports->func[6].withOneArg(1);
            gDLL_AMSEQ->exports->func[6].withOneArg(2);
            gDLL_AMSEQ->exports->func[6].withOneArg(3);
            bss_38 = 35;
        }
    }
}

// func 16
static void dll_63_func_1F4C(s32 param1, s32 param2) {
    if (param1 == 0) {
        data_548 = param2;
        bss_3A = 56;
        bss_3C = 179;
        dll_63_func_12C4();
    } else if (param1 == 1) {
        bss_4E = 1;
        gDLL_28_screen_fade->exports->fade(20, SCREEN_FADE_BLACK);
        gDLL_AMSEQ->exports->func[6].withOneArg(0);
        gDLL_AMSEQ->exports->func[6].withOneArg(1);
        gDLL_AMSEQ->exports->func[6].withOneArg(2);
        gDLL_AMSEQ->exports->func[6].withOneArg(3);
        bss_38 = 35;
    }
}

// func 17
static void dll_63_func_2074(s32 param1, s32 param2) {
    if (param1 == 0) {
        dll_63_func_FD4(0);
    } else {
        data_548 = param2;
        bss_3A = 58;
        bss_3C = 146;
        dll_63_func_1528();
    }
}

// func 18
static void dll_63_func_20F0(s32 param1, s32 param2) {
    if (param1 == 0) {
        dll_63_func_13C0();
    } else {
        data_54C = bss_36[param2];
        dll_63_func_16D8();
    }
}

// func 19
static void dll_63_func_2160(s32 param1, s32 param2) {
    if (param1 == 0 || param2 == 1) {
        dll_63_func_1528();
    } else {
        gDLL_29_gplay->exports->func_638(data_548, data_54C);
        dll_63_func_FD4(0);
    }
}

// func 20
static void dll_63_func_21F4(s32 param1, s32 param2) {
    if (param1 == 0) {
        dll_63_func_FD4(0);
    } else {
        data_548 = param2;
        bss_3A = 56;
        bss_3C = 179;
        dll_63_func_1938();
    }
}

// func 21
static void dll_63_func_2270(s32 param1, s32 param2) {
    if (param1 == 0 || param2 == 1) {
        dll_63_func_17D4();
    } else {
        gDLL_29_gplay->exports->func_110(data_548);
        dll_63_func_FD4(0);
    }
}

// func 22
static void dll_63_func_22FC(Gfx **gdl, s32 x, s32 y, DLL63Struct *param4) {
    s32 i;
    s32 x2;
    s32 y2;
    s32 len;

    x2 = x;
    y2 = y;
    len = ARRAYCOUNT(data_57C);

    for (i = 0; i < len; i++) {
        if (data_57C[i] == -1) {
            x2 = x;
            y2 += 32;
        } else {
            func_8003825C(gdl, bss_80[data_57C[i]], x2, y2, 0, 0, 0xFF, 0);
            x2 += 64;
        }
    }

    func_8003825C(gdl, bss_70[param4->character], x + 0xe, y + 8, 0, 0, 0xFF, 0);
    func_8003825C(gdl, bss_70[2], x + 0xf1, y + 0x47, 0, 0, 0xFF, 0);
    func_8003825C(gdl, bss_70[3], x2 + 0xe, y + 0x47, 0, 0, 0xFF, 0);

    font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);
    font_window_set_text_colour(1, 255, 255, 255, 0, 255);
    
    font_window_add_string_xy(1, x + 0x40, y + 0x12, param4->unkC, 1, ALIGN_TOP_LEFT);

    sprintf(bss_40, "%3d:%02d:%02d", param4->unk4, param4->unk6, param4->unk8);
    font_window_add_string_xy(1, x + 0x9c, y + 0x31, bss_40, 1, ALIGN_TOP_CENTER);

    sprintf(bss_4A, "%1d", 0);
    font_window_add_string_xy(1, x + 0xea, y + 0x51, bss_4A, 1, ALIGN_TOP_CENTER);

    sprintf(bss_4C, "%1d", 0);
    font_window_add_string_xy(1, x + 0x54, y + 0x51, bss_4C, 1, ALIGN_TOP_CENTER);
}
