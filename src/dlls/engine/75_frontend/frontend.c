#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "macros.h"
#include "sys/fonts.h"
#include "sys/menu.h"

typedef struct {
/*0*/    s16 unk0;  //width?
/*2*/    s16 unk2;
/*4*/    u8 flags;
/*5*/    u8 type;
/*6*/    s8 unk6;
/*8*/    s16 min;
/*A*/    s16 max;
/*C*/    s16 value;
/*E*/    s16 unkE;
} FrontEndOther2; //Actually a slider?

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
/*0x1C*/ static s32 bss_1C;

static void frontend_func_820(FrontEndOther2* arg0, Gfx** arg1);
static void frontend_func_938(FrontEndSlider* arg0, Gfx** arg1);
static void frontend_func_A00(FrontEndOther* arg0, Gfx** arg1);

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
FrontEndOther2* frontend_func_E0(s16 arg0, s16 arg1, s16 min, s16 max, s16 initialValue) {
    FrontEndOther2* ctrl;
    Texture* tex = sTextures[0];
    s32 width;
    
    if (initialValue < min) {
        initialValue = min;
    }
    if (max < initialValue) {
        initialValue = max;
    }

    ctrl = mmAlloc(sizeof(FrontEndOther2), ALLOC_TAG_TRACK_COL, NULL);
    ctrl->type = 0;
    ctrl->value = initialValue;
    ctrl->min = min;
    ctrl->max = max;
    ctrl->unk0 = arg0;
    ctrl->unk2 = arg1;
    ctrl->flags = 0;
    ctrl->unk6 = 4;

    width = tex->width | ((tex->widthHeightHi & 0xF0) << 4);
    ctrl->unkE = width - 8;

    return ctrl;
}

// offset: 0x1C4 | func: 1 | export: 1
FrontEndSlider* frontend_func_1C4(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    FrontEndSlider* ctrl;

    if (arg4 < arg2) {
        arg4 = arg2;
    }
    if (arg3 < arg4) {
        arg4 = arg3;
    }
    
    ctrl = mmAlloc(sizeof(FrontEndSlider), 5, NULL);
    ctrl->type = 1;
    ctrl->value = arg4;
    ctrl->min = arg2;
    ctrl->max = arg3;
    ctrl->unk0 = arg0;
    ctrl->unk2 = arg1;
    ctrl->flags = 0;
    ctrl->unk6 = 4;

    return ctrl;
}

// offset: 0x284 | func: 2 | export: 2
FrontEndOther* frontend_func_284(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, char** arg5, u8 arg6) {
    FrontEndOther *ctrl;
    int new_var;
    
    if (arg4 < arg2) {
        arg4 = arg2;
    } if (arg3 < arg4) {
        arg4 = arg3;
    }
    
    new_var = 2;
    ctrl = mmAlloc(sizeof(FrontEndOther), 5, NULL);
    ctrl->type = new_var;
    ctrl->value = arg4;
    ctrl->min = arg2;
    ctrl->max = arg3;
    ctrl->unk10 = arg5;
    ctrl->unk0 = arg0;
    ctrl->unk2 = arg1;
    ctrl->flags = new_var;
    ctrl->unk6 = 4;
    ctrl->unk14 = arg6;

    return ctrl;
}

// offset: 0x354 | func: 3 | export: 3
void frontend_func_354(FrontEndSlider* arg0) {
    mmFree(arg0);
}

// offset: 0x38C | func: 4 | export: 4
void frontend_func_38C(FrontEndOther2* ctrl) {
    s8 sp57;
    s8 sp56;
    s16 previousValue;
    s32 sp50;
    s32 screenWidth;
    s32 sp48;
    s32 screenHeight;
    s32 temp_ft0;

    if (ctrl->unk6 != 0) {
        func_80010158(&sp50, &screenWidth, &sp48, &screenHeight);
        if (ctrl->type == 0) {
            screenHeight += 4;
        } else if (ctrl->type == 1) {
            screenHeight += 4;
        }
        func_800100D4(sp50, screenWidth, sp48, screenHeight);
    }
    
    if (ctrl->flags & 1) {
        ctrl->flags &= ~(0x10 | 8 | 4);
        previousValue = ctrl->value;
        ctrl->unk6 = 4;

        switch (ctrl->type) {
        case 2:
            joy_get_stick_menu_xy_sign(0, &sp57, &sp56);
            if (sp57 < 0) {
                gDLL_6_AMSFX->vtbl->play(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);
                ctrl->value--;
                ctrl->flags |= 4;
            } else if (sp57 > 0) {
                gDLL_6_AMSFX->vtbl->play(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);
                ctrl->value++;
                ctrl->flags |= 8;
            }
            break;
        case 0:
            sp57 = ((joy_get_stick_x(0) * gUpdateRateF) / 40.0f);
            ctrl->value += sp57;
            break;
        default:
        case 1:
            if (!(ctrl->flags & 0x20) && (joy_get_pressed(0) & A_BUTTON)) {
                gDLL_6_AMSFX->vtbl->play(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);
                ctrl->value ^= 1;
            }
            break;
        }
        
        if (ctrl->max < ctrl->value) {
            if (ctrl->flags & 2) {
                ctrl->value = 0;
            } else {
                ctrl->value = ctrl->max;
            }
        } else if (ctrl->value < ctrl->min) {
            if (ctrl->flags & 2) {
                ctrl->value = ctrl->max;
            } else {
                ctrl->value = ctrl->min;
            }
        }
        
        if (previousValue != ctrl->value) {
            ctrl->flags |= 0x10;
        }
    }
}

// offset: 0x6A0 | func: 5 | export: 5
void frontend_func_6A0(FrontEndSlider* arg0, Gfx** arg1) {
    if (arg0->unk6 != 0) {
        switch (arg0->type) {
        case 0:
            frontend_func_820(arg0, arg1);
            break;
        case 1:
            frontend_func_938(arg0, arg1);
            break;
        case 2:
            frontend_func_A00(arg0, arg1);
            break;
        }
    }
    
    arg0->unk6--;
    if (arg0->unk6 < 0) {
        arg0->unk6 = 0;
    }
}

// offset: 0x77C | func: 6 | export: 6
s32 frontend_func_77C(FrontEndSlider* arg0) {
    return arg0->flags & 1;
}

// offset: 0x790 | func: 7 | export: 7
void frontend_func_790(FrontEndSlider* slider, s32 enable) {
    if (enable) {
        slider->flags |= 1;
    } else {
        slider->flags &= ~1;
    }
}

// offset: 0x7C0 | func: 8 | export: 8
s16 frontend_func_7C0(FrontEndSlider* slider) {
    return slider->value;
}

// offset: 0x7CC | func: 9 | export: 9
void frontend_func_7CC(FrontEndSlider* arg0, s32 arg1) {
    arg0->value = arg1;
    arg0->unk6 = 2;
}

// offset: 0x7E0 | func: 10 | export: 10
s32 frontend_func_7E0(FrontEndSlider* arg0) {
    return arg0->flags & 0x10;
}

// offset: 0x7F4 | func: 11 | export: 11
void frontend_func_7F4(FrontEndSlider* arg0, s32 arg1) {
    if (arg1) {
        arg0->flags &= ~0x20;
    } else {
        arg0->flags |= 0x20;
    }
}

// offset: 0x820 | func: 12
void frontend_func_820(FrontEndOther2* arg0, Gfx** arg1) {
    s32 var_v0;
    f32 diff;
    
    rcp_screen_full_write(arg1, sTextures[0], arg0->unk0, arg0->unk2, 0, 0, 0xFF, 0);
    
    diff = ((f32)(arg0->value - arg0->min)) / (arg0->max - arg0->min);
    var_v0 = arg0->unk0 + (arg0->unkE) * diff;
    
    rcp_screen_full_write(arg1, sTextures[(arg0->flags & 1) ? 1 : 2], var_v0, arg0->unk2  - 6, 0, 0, 0xFF, 0);
}

// offset: 0x938 | func: 13
void frontend_func_938(FrontEndSlider* arg0, Gfx** arg1) {
    s32 var_v0;
    s32 var_v1;

    if (arg0->flags & 1) {
        if (arg0->value != 0) {
            var_v0 = 3;
        } else {
            var_v0 = 5;
        }
    } else {
        if (arg0->value != 0) {
            var_v0 = 4;
        } else {
            var_v0 = 6;
        }
    }
    
    if (arg0->flags & 0x20) {
        var_v1 = 0x80;
    } else {
        var_v1 = 0xFF;
    }
    
    rcp_screen_full_write(arg1, sTextures[var_v0], arg0->unk0, arg0->unk2, 0, 0, var_v1, 0);
}

// offset: 0xA00 | func: 14
void frontend_func_A00(FrontEndOther* arg0, Gfx** arg1) {
    font_window_use_font(1, 0);
    font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
    if (arg0->flags & 1) {
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, 0xFF);
    } else {
        font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, 0xFF);
    }
    
    font_window_add_string_xy(1, arg0->unk0, arg0->unk2, arg0->unk10[arg0->value], 1, ALIGN_TOP_CENTER);
    font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0x96);
    font_window_add_string_xy(1, arg0->unk0 - 1, arg0->unk2 - 1, arg0->unk10[arg0->value], 2, ALIGN_TOP_CENTER);
    font_get_text_width(1, arg0->unk10[arg0->value], 0, 0);
    
    if (arg0->flags & 4) {
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, 0xFF);
    } else {
        font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, 0xFF);
    }
    
    font_window_add_string_xy(1, (arg0->unk0 - arg0->unk14) - 2, (s32) arg0->unk2, &dLeftArrowChar, 1, ALIGN_TOP_CENTER);
    
    if (arg0->flags & 8) {
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, 0xFF);
    } else {
        font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, 0xFF);
    }
    
    font_window_add_string_xy(1, arg0->unk0 + arg0->unk14, arg0->unk2, &dRightArrowChar, 1, ALIGN_TOP_CENTER);
    font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0x96);
    font_window_add_string_xy(1, (arg0->unk0 - arg0->unk14) - 3, arg0->unk2, &dLeftArrowChar, 2, ALIGN_TOP_CENTER);
    font_window_add_string_xy(1, (arg0->unk0 + arg0->unk14) - 1, arg0->unk2, &dRightArrowChar, 2, ALIGN_TOP_CENTER);
}
