//NOTE: seems to be the DLL that some older menus used for text printing, which was once located at `dll_throw_fault`'s address

#include "PR/os.h"
#include "common.h"
#include "sys/fonts.h"
#include "dlls/engine/73.h"

/*0x0*/ static s8 dButtonsEnabled = FALSE;  //Whether to check button presses (for backing out/advancing)

/*0x0*/ static s8 sValueEnter; //Value returned when selecting a menu item
/*0x1*/ static s8 sValueExit;  //Value returned when backing out from the menu
/*0x2*/ static s16 sTextY;
/*0x4*/ static s8 sTotalItems;
/*0x8*/ static f32 sTimer;

void dll_73_set_font_and_colour(s32 dimmed);

// offset: 0x0 | ctor
void dll_73_ctor(void* dll) {
    sTotalItems = 0;
    sTextY = sTotalItems;
    sValueExit = sTextY;
    sValueEnter = sValueExit;
    dButtonsEnabled = sValueEnter;
}

// offset: 0x60 | dtor
void dll_73_dtor(void *dll) { }

// offset: 0x6C | func: 0 | export: 0
void dll_73_init_text_window(s32 y) {
    font_window_set_coords(1, 0, 0, 
        GET_VIDEO_WIDTH(vi_get_current_size()), 
        GET_VIDEO_HEIGHT(vi_get_current_size())
    );
    font_window_flush_strings(1);
    sTextY = y;
    sTotalItems = 0;
    sValueExit = DLL73_ACTION_None;
}

// offset: 0x118 | func: 1 | export: 1
void dll_73_init_text_window_with_margin(s32 marginX, s32 y) {
    u32 halfWidth;

    halfWidth = (GET_VIDEO_WIDTH(vi_get_current_size()) - marginX) / 2;
    
    font_window_set_coords(1, halfWidth, 0, 
        GET_VIDEO_WIDTH(vi_get_current_size()) - halfWidth, 
        GET_VIDEO_HEIGHT(vi_get_current_size())
    );
    
    font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);
    font_window_flush_strings(1);
    sTextY = y;
    sTotalItems = 0;
    sValueExit = DLL73_ACTION_None;
}

// offset: 0x204 | func: 2 | export: 2
/**
  * Adds a string to the font window, centred horizontally.
  *
  * The selected line has an animation, fading between the Dino Medium In/Out fonts.
  */
void dll_73_add_string(s32 enterValue, char* text, s32 lineHeight, s32 selectedIndex) {
    f32 tValue;
    f32 opacity;
    f32 opacityRemainder;

    if (selectedIndex == sTotalItems) {
        sValueEnter = enterValue;
        if (sTimer <= 100.0f) {
            tValue = sTimer / 100.0f;
            opacity = (s32)(tValue * 255.0f);
            opacityRemainder = (s32) ((1.0f - tValue) * 255.0f);
        } else {
            tValue = (sTimer - 100.0f) / 100.0f;
            opacityRemainder = (s32) (tValue * 255.0f);
            opacity = (s32) ((1.0f - tValue) * 255.0f);
        }
        
        font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, (s32)opacity);
        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);
        font_window_add_string_xy(1, -0x8000, sTextY, text, 1, ALIGN_TOP_CENTER);
        font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, (s32)opacityRemainder);
        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_OUT);
    } else {
        font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_OUT);
    }
    
    font_window_add_string_xy(1, -0x8000, sTextY, text, 1, ALIGN_TOP_CENTER);
    sTextY += lineHeight;
    sTotalItems++;
}

// offset: 0x46C | func: 3 | export: 3
/**
  * Adds a string to the font window, using a specific horizontal position.
  */
void dll_73_add_string_x(s32 valueEnter, char* text, s32 x, s32 lineHeight, s32 selectedIndex) {
    dll_73_set_font_and_colour(selectedIndex == sTotalItems);
    
    if (selectedIndex == sTotalItems) {
        sValueEnter = valueEnter;
    }
    
    font_window_add_string_xy(1, x, sTextY, text, 1, ALIGN_TOP_CENTER);
    sTextY += lineHeight;
    sTotalItems++;
}

// offset: 0x540 | func: 4 | export: 4
/* Sets a value for `dll_73_handle_joystick_and_buttons` to return when backing out with the B button */
void dll_73_set_exit_value(s32 value) {
    sValueExit = value;
}

// offset: 0x55C | func: 5 | export: 5
s16 dll_73_handle_joystick_and_buttons(s32* idx) {
    s32 joyPressed;
    s8 joyX;
    s8 joyY;

    sTimer += gUpdateRateF;
    if (sTimer > 200.0f) {
        sTimer -= 200.0f;
    }
    
    //Handle moving up/down menu with joystick
    {
        joy_get_stick_menu_xy_sign(0, &joyX, &joyY);
        if (joyY < 0) {
            *idx = *idx + 1;
        } else if (joyY > 0) {
            *idx = *idx - 1;
        }

        //Handle wrapping back to top/bottom of list
        if (*idx < 0) {
            *idx = sTotalItems - 1;
        }
        
        if (*idx >= sTotalItems) {
            *idx = 0;
        }
    }
    
    //Check for button presses
    if (dButtonsEnabled) {
        joyPressed = joy_get_pressed(0);
        
        if (joyPressed & (A_BUTTON | START_BUTTON)) {
            if (main_get_bits(BIT_Menus_Selection_Blocked) == FALSE) {
                return sValueEnter;
            }
        }
        
        if (joyPressed & B_BUTTON) {
            return sValueExit;
        }
    }
    dButtonsEnabled = TRUE;
    
    return DLL73_ACTION_None;
}

// offset: 0x6D0 | func: 6 | export: 6
void dll_73_set_font_and_colour(s32 dimmed) {
    font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
    if (dimmed == FALSE) {
        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);
    } else {
        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_OUT);
    }
}

// offset: 0x764 | func: 7 | export: 7
/**
  * Enable/disable button interaction.
  */
void dll_73_enable_joy_buttons(s32 enabled) {
    dButtonsEnabled = enabled;
}

// offset: 0x780 | func: 8 | export: 8
s8 dll_73_get_total_items(void) {
    return sTotalItems;
}
