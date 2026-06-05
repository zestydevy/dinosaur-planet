//NOTE: seems to be the DLL that some older menus used for text printing, which was once located at `dll_throw_fault`'s address

#include "PR/os.h"
#include "common.h"
#include "sys/fonts.h"

/*0x0*/ static s8 data_0 = 0;

/*0x0*/ static s8 bss_0;
/*0x1*/ static s8 bss_1;
/*0x2*/ static s16 bss_2;
/*0x4*/ static s8 bss_4;
/*0x8*/ static f32 bss_8;

void dll_73_func_6D0(s32 arg0);

// offset: 0x0 | ctor
void dll_73_ctor(void* dll) {
    bss_4 = 0;
    bss_2 = bss_4;
    bss_1 = bss_2;
    bss_0 = bss_1;
    data_0 = bss_0;
}

// offset: 0x60 | dtor
void dll_73_dtor(void *dll) { }

// offset: 0x6C | func: 0 | export: 0
void dll_73_func_6C(s32 arg0) {
    font_window_set_coords(1, 0, 0, 
        GET_VIDEO_WIDTH(vi_get_current_size()), 
        GET_VIDEO_HEIGHT(vi_get_current_size())
    );
    font_window_flush_strings(1);
    bss_2 = arg0;
    bss_4 = 0;
    bss_1 = -1;
}

// offset: 0x118 | func: 1 | export: 1
void dll_73_func_118(s32 x, s32 y) {
    u32 halfWidth;

    halfWidth = (GET_VIDEO_WIDTH(vi_get_current_size()) - x) / 2;
    
    font_window_set_coords(1, halfWidth, 0, 
        GET_VIDEO_WIDTH(vi_get_current_size()) - halfWidth, 
        GET_VIDEO_HEIGHT(vi_get_current_size())
    );
    
    font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);
    font_window_flush_strings(1);
    bss_2 = y;
    bss_4 = 0;
    bss_1 = -1;
}

// offset: 0x204 | func: 2 | export: 2
void dll_73_func_204(s32 arg0, char* text, s32 arg2, s32 arg3) {
    f32 tValue;
    f32 opacity;
    f32 opacityRemainder;

    if (arg3 == bss_4) {
        bss_0 = arg0;
        if (bss_8 <= 100.0f) {
            tValue = bss_8 / 100.0f;
            opacity = (s32)(tValue * 255.0f);
            opacityRemainder = (s32) ((1.0f - tValue) * 255.0f);
        } else {
            tValue = (bss_8 - 100.0f) / 100.0f;
            opacityRemainder = (s32) (tValue * 255.0f);
            opacity = (s32) ((1.0f - tValue) * 255.0f);
        }
        
        font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, (s32)opacity);
        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);
        font_window_add_string_xy(1, -0x8000, bss_2, text, 1, 4);
        font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, (s32)opacityRemainder);
        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_OUT);
    } else {
        font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_OUT);
    }
    
    font_window_add_string_xy(1, -0x8000, bss_2, text, 1, 4);
    bss_2 += arg2;
    bss_4++;
}

// offset: 0x46C | func: 3 | export: 3
void dll_73_func_46C(s32 arg0, char* text, s32 x, s32 lineHeight, s32 fontIndex) {
    dll_73_func_6D0(fontIndex == bss_4);
    
    if (fontIndex == bss_4) {
        bss_0 = arg0;
    }
    
    font_window_add_string_xy(1, x, bss_2, text, 1, 4);
    bss_2 += lineHeight;
    bss_4 += 1;
}

// offset: 0x540 | func: 4 | export: 4
void dll_73_func_540(s32 arg0) {
    bss_1 = arg0;
}

// offset: 0x55C | func: 5 | export: 5
s16 dll_73_func_55C(s32* arg0) {
    s32 joyPressed;
    s8 joyX;
    s8 joyY;

    bss_8 += gUpdateRateF;
    if (bss_8 > 200.0f) {
        bss_8 -= 200.0f;
    }
    
    joy_get_stick_menu_xy_sign(0, &joyX, &joyY);
    if (joyY < 0) {
        *arg0 = *arg0 + 1;
    } else if (joyY > 0) {
        *arg0 = *arg0 - 1;
    }

    if (*arg0 < 0) {
        *arg0 = bss_4 - 1;
    }
    
    if (*arg0 >= bss_4) {
        *arg0 = 0;
    }
    
    if (data_0 != 0) {
        joyPressed = joy_get_pressed(0);
        
        if (joyPressed & (A_BUTTON | START_BUTTON)) {
            if (main_get_bits(BIT_44F) == 0) {
                return bss_0;
            }
        }
        
        if (joyPressed & B_BUTTON) {
            return bss_1;
        }
    }
    
    data_0 = 1;
    
    return -1;
}

// offset: 0x6D0 | func: 6 | export: 6
void dll_73_func_6D0(s32 arg0) {
    font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
    if (arg0 == 0) {
        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);
    } else {
        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_OUT);
    }
}

// offset: 0x764 | func: 7 | export: 7
void dll_73_func_764(s32 arg0) {
    data_0 = arg0;
}

// offset: 0x780 | func: 8 | export: 8
s8 dll_73_func_780(void) {
    return bss_4;
}
