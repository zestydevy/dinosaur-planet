//Seems to have been named "frontend/control.c", based on default.dol

#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/75_frontend.h"
#include "macros.h"
#include "sys/fonts.h"
#include "sys/menu.h"

typedef enum {
    FRONTEND_TEXTURE_0_Slider_Track,
    FRONTEND_TEXTURE_1_Slider_Handle_HL,
    FRONTEND_TEXTURE_2_Slider_Handle,
    FRONTEND_TEXTURE_3_Checkbox_Ticked_HL,
    FRONTEND_TEXTURE_4_Checkbox_Ticked,
    FRONTEND_TEXTURE_5_Checkbox_Empty_HL,
    FRONTEND_TEXTURE_6_Checkbox_Empty
} FrontEndTextureIDs;

/*0x0*/ static s16 dTextureIDs[] = {
    TEXTABLE_305_FrontEnd_Slider_Track, 
    TEXTABLE_307_FrontEnd_Slider_Handle_Highlighted, 
    TEXTABLE_308_FrontEnd_Slider_Handle, 
    TEXTABLE_309_FrontEnd_Checkbox_Ticked_Highlighted, 
    TEXTABLE_30A_FrontEnd_Checkbox_Ticked, 
    TEXTABLE_30B_FrontEnd_Checkbox_Empty_Highlighted, 
    TEXTABLE_30C_FrontEnd_Checkbox_Empty
};

/*0x10*/ static char dLeftArrowChar = 0x3c;
/*0x14*/ static char dRightArrowChar = 0x3e;

/*0x0*/ static Texture* sTextures[7];

static void frontend_slider_draw(FrontEndSlider* ctrl, Gfx** gdl);
static void frontend_checkbox_draw(FrontEndCheckbox* ctrl, Gfx** gdl);
static void frontend_list_draw(FrontEndList* ctrl, Gfx** gdl);

// offset: 0x0 | ctor
void frontend_ctor(void *dll) {
    u32 i;

    for (i = 0; i < ARRAYCOUNT(sTextures); i++) {
        sTextures[i] = tex_load_deferred(dTextureIDs[i]);
    }
}

// offset: 0x7C | dtor
void frontend_dtor(void *dll) {
    u32 i;
    for (i = 0; i < ARRAYCOUNT(sTextures); i++) {
        tex_free(sTextures[i]);
    }
}

// offset: 0xE0 | func: 0 | export: 0
FrontEndSlider* frontend_create_slider(s16 x, s16 y, s16 min, s16 max, s16 initialValue) {
    FrontEndSlider* ctrl;
    Texture* tex = sTextures[FRONTEND_TEXTURE_0_Slider_Track];
    s32 width;
    
    if (initialValue < min) {
        initialValue = min;
    }
    if (initialValue > max) {
        initialValue = max;
    }

    ctrl = mmAlloc(sizeof(FrontEndSlider), ALLOC_TAG_TRACK_COL, NULL);
    ctrl->type = FRONTEND_CONTROL_Slider;
    ctrl->value = initialValue;
    ctrl->min = min;
    ctrl->max = max;
    ctrl->x = x;
    ctrl->y = y;
    ctrl->flags = FRONTEND_FLAG_0_None;
    ctrl->redrawFrames = 4;

    width = tex->width | ((tex->widthHeightHi & 0xF0) << 4);
    ctrl->width = width - 8;

    return ctrl;
}

// offset: 0x1C4 | func: 1 | export: 1
FrontEndCheckbox* frontend_create_checkbox(s16 x, s16 y, s16 min, s16 max, s16 initialValue) {
    FrontEndCheckbox* ctrl;

    if (initialValue < min) {
        initialValue = min;
    }
    if (initialValue > max) {
        initialValue = max;
    }
    
    ctrl = mmAlloc(sizeof(FrontEndCheckbox), ALLOC_TAG_TRACK_COL, NULL);
    ctrl->type = FRONTEND_CONTROL_Checkbox;
    ctrl->value = initialValue;
    ctrl->min = min;
    ctrl->max = max;
    ctrl->x = x;
    ctrl->y = y;
    ctrl->flags = FRONTEND_FLAG_0_None;
    ctrl->redrawFrames = 4;

    return ctrl;
}

// offset: 0x284 | func: 2 | export: 2
FrontEndList* frontend_create_list(s16 x, s16 y, s16 min, s16 max, s16 initialValue, char** strings, u8 halfWidth) {
    FrontEndList *ctrl;
    
    if (initialValue < min) {
        initialValue = min;
    } if (max < initialValue) {
        initialValue = max;
    }
    
    ctrl = mmAlloc(sizeof(FrontEndList), ALLOC_TAG_TRACK_COL, NULL);
    ctrl->type = FRONTEND_CONTROL_List;
    ctrl->value = initialValue;
    ctrl->min = min;
    ctrl->max = max;
    ctrl->strings = strings;
    ctrl->x = x;
    ctrl->y = y;
    ctrl->flags = FRONTEND_FLAG_2_Wrap_Value;
    ctrl->redrawFrames = 4;
    ctrl->halfWidth = halfWidth;

    return ctrl;
}

// offset: 0x354 | func: 3 | export: 3
void frontend_free(FrontEndControl* ctrl) {
    mmFree(ctrl);
}

// offset: 0x38C | func: 4 | export: 4
void frontend_update(FrontEndControl* ctrl) {
    s8 joyX;
    s8 joyY;
    s16 previousValue;
    s32 ulx;
    s32 lrx;
    s32 uly;
    s32 lry;

    if (ctrl->redrawFrames != 0) {
        func_80010158(&ulx, &lrx, &uly, &lry);
        if (ctrl->type == FRONTEND_CONTROL_Slider) {
            lry += 4;
        } else if (ctrl->type == FRONTEND_CONTROL_Checkbox) {
            lry += 4;
        }
        func_800100D4(ulx, lrx, uly, lry);
    }
    
    if ((ctrl->flags & FRONTEND_FLAG_1_Highlighted) == FALSE) {
        return;
    }

    ctrl->flags &= ~(FRONTEND_FLAG_10_Value_Changed | FRONTEND_FLAG_8_Moved_Right | FRONTEND_FLAG_4_Moved_Left);
    previousValue = ctrl->value;
    ctrl->redrawFrames = 4;

    switch (ctrl->type) {
    case FRONTEND_CONTROL_List:
        joy_get_stick_menu_xy_sign(0, &joyX, &joyY);
        if (joyX < 0) {
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);
            ctrl->value--;
            ctrl->flags |= FRONTEND_FLAG_4_Moved_Left;
        } else if (joyX > 0) {
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);
            ctrl->value++;
            ctrl->flags |= FRONTEND_FLAG_8_Moved_Right;
        }
        break;
    case FRONTEND_CONTROL_Slider:
        joyX = (s8)((joy_get_stick_x(0) * gUpdateRateF) / 40.0f);
        ctrl->value += joyX;
        break;
    default:
    case FRONTEND_CONTROL_Checkbox:
        if (!(ctrl->flags & FRONTEND_FLAG_20_Locked) && (joy_get_pressed(0) & A_BUTTON)) {
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);
            ctrl->value ^= 1;
        }
        break;
    }
    
    //Handle wrapping/clamping value when moving past min/max
    if (ctrl->value > ctrl->max) {
        if (ctrl->flags & FRONTEND_FLAG_2_Wrap_Value) {
            ctrl->value = 0;
        } else {
            ctrl->value = ctrl->max;
        }
    } else if (ctrl->value < ctrl->min) {
        if (ctrl->flags & FRONTEND_FLAG_2_Wrap_Value) {
            ctrl->value = ctrl->max;
        } else {
            ctrl->value = ctrl->min;
        }
    }
    
    //Flag when the control's value changes
    if (previousValue != ctrl->value) {
        ctrl->flags |= FRONTEND_FLAG_10_Value_Changed;
    }
}

// offset: 0x6A0 | func: 5 | export: 5
void frontend_draw(FrontEndControl* ctrl, Gfx** gdl) {
    if (ctrl->redrawFrames != 0) {
        switch (ctrl->type) {
        case FRONTEND_CONTROL_Slider:
            frontend_slider_draw((FrontEndSlider*)ctrl, gdl);
            break;
        case FRONTEND_CONTROL_Checkbox:
            frontend_checkbox_draw((FrontEndCheckbox*)ctrl, gdl);
            break;
        case FRONTEND_CONTROL_List:
            frontend_list_draw((FrontEndList*)ctrl, gdl);
            break;
        }
    }
    
    ctrl->redrawFrames--;
    if (ctrl->redrawFrames < 0) {
        ctrl->redrawFrames = 0;
    }
}

// offset: 0x77C | func: 6 | export: 6
s32 frontend_get_highlight_state(FrontEndControl* ctrl) {
    return ctrl->flags & FRONTEND_FLAG_1_Highlighted;
}

// offset: 0x790 | func: 7 | export: 7
void frontend_set_highlight_state(FrontEndControl* slider, s32 enable) {
    if (enable) {
        slider->flags |= FRONTEND_FLAG_1_Highlighted;
    } else {
        slider->flags &= ~FRONTEND_FLAG_1_Highlighted;
    }
}

// offset: 0x7C0 | func: 8 | export: 8
s16 frontend_get_value(FrontEndControl* ctrl) {
    return ctrl->value;
}

// offset: 0x7CC | func: 9 | export: 9
void frontend_set_value(FrontEndControl* ctrl, s32 value) {
    ctrl->value = value;
    ctrl->redrawFrames = 2;
}

// offset: 0x7E0 | func: 10 | export: 10
s32 frontend_did_value_change(FrontEndControl* ctrl) {
    return ctrl->flags & FRONTEND_FLAG_10_Value_Changed;
}

// offset: 0x7F4 | func: 11 | export: 11
void frontend_set_unlock_state(FrontEndControl* ctrl, s32 unlock) {
    if (unlock) {
        ctrl->flags &= ~FRONTEND_FLAG_20_Locked;
    } else {
        ctrl->flags |= FRONTEND_FLAG_20_Locked;
    }
}

// offset: 0x820 | func: 12
void frontend_slider_draw(FrontEndSlider* ctrl, Gfx** gdl) {
    s32 screenX;
    f32 tValue;
    
    rcp_screen_full_write(gdl, 
        sTextures[FRONTEND_TEXTURE_0_Slider_Track], 
        ctrl->x, ctrl->y, 
        0, 0, 
        0xFF, 0
    );
    
    tValue = (f32)(ctrl->value - ctrl->min) / (ctrl->max - ctrl->min);
    screenX = ctrl->x + (ctrl->width) * tValue;
    
    rcp_screen_full_write(gdl, 
        sTextures[(ctrl->flags & 1) ? FRONTEND_TEXTURE_1_Slider_Handle_HL : FRONTEND_TEXTURE_2_Slider_Handle], 
        screenX, ctrl->y  - 6, 
        0, 0, 0xFF, 0);
}

// offset: 0x938 | func: 13
void frontend_checkbox_draw(FrontEndCheckbox* ctrl, Gfx** gdl) {
    s32 textureIdx;
    s32 opacity;

    if (ctrl->flags & FRONTEND_FLAG_1_Highlighted) {
        if (ctrl->value) {
            textureIdx = FRONTEND_TEXTURE_3_Checkbox_Ticked_HL;
        } else {
            textureIdx = FRONTEND_TEXTURE_5_Checkbox_Empty_HL;
        }
    } else {
        if (ctrl->value) {
            textureIdx = FRONTEND_TEXTURE_4_Checkbox_Ticked;
        } else {
            textureIdx = FRONTEND_TEXTURE_6_Checkbox_Empty;
        }
    }
    
    if (ctrl->flags & FRONTEND_FLAG_20_Locked) {
        opacity = 0x80;
    } else {
        opacity = 0xFF;
    }
    
    rcp_screen_full_write(gdl, sTextures[textureIdx], 
        ctrl->x, ctrl->y, 
        0, 0, 
        opacity, 0);
}

// offset: 0xA00 | func: 14
void frontend_list_draw(FrontEndList* ctrl, Gfx** gdl) {
    font_window_use_font(1, FONT_FUN_FONT);
    font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);

    //Print currently-displayed list item
    {
        //Different colour when selected
        if (ctrl->flags & FRONTEND_FLAG_1_Highlighted) {
            font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, 0xFF);
        } else {
            font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, 0xFF);
        }
        font_window_add_string_xy(1, ctrl->x, ctrl->y, ctrl->strings[ctrl->value], 1, ALIGN_TOP_CENTER);

        //Print drop-shadow
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0x96);
        font_window_add_string_xy(1, ctrl->x - 1, ctrl->y - 1, ctrl->strings[ctrl->value], 2, ALIGN_TOP_CENTER);
        
        font_get_text_width(1, ctrl->strings[ctrl->value], 0, FONT_FUN_FONT);
    }

    //Left/right navigation arrows
    {
        //Print left navigation arrow
        if (ctrl->flags & FRONTEND_FLAG_4_Moved_Left) {
            font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, 0xFF);
        } else {
            font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, 0xFF);
        }
        font_window_add_string_xy(1, (ctrl->x - ctrl->halfWidth) - 2, ctrl->y, &dLeftArrowChar, 1, ALIGN_TOP_CENTER);
        
        //Print right navigation arrow
        if (ctrl->flags & FRONTEND_FLAG_8_Moved_Right) {
            font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, 0xFF);
        } else {
            font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, 0xFF);
        }
        font_window_add_string_xy(1, ctrl->x + ctrl->halfWidth, ctrl->y, &dRightArrowChar, 1, ALIGN_TOP_CENTER);

        //Print drop-shadows for navigation arrows
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0x96);
        font_window_add_string_xy(1, (ctrl->x - ctrl->halfWidth) - 3, ctrl->y, &dLeftArrowChar, 2, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, (ctrl->x + ctrl->halfWidth) - 1, ctrl->y, &dRightArrowChar, 2, ALIGN_TOP_CENTER);
    }
}
