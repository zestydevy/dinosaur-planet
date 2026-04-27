//NOTE: official name unknown, but used for SwapStone / ShopKeeper's selection screens

#include "common.h"
#include "game/gametexts.h"
#include "sys/fonts.h"
#include "sys/gfx/textable.h"
#include "sys/menu.h"

/** COORD MACROS */

#define SHADOW_X -1
#define SHADOW_Y -1

// Confirm/Cancel (left-aligned)

#define CONFIRM_LEFT_A_X 30
#define CONFIRM_LEFT_A_Y 168

#define CONFIRM_LEFT_B_X 30
#define CONFIRM_LEFT_B_Y 142

#define CONFIRM_LEFT_TEXT_CANCEL_X 56
#define CONFIRM_LEFT_TEXT_CANCEL_Y 148

#define CONFIRM_LEFT_TEXT_CONFIRM_X 56
#define CONFIRM_LEFT_TEXT_CONFIRM_Y 174

// Confirm/Cancel (right-aligned)

#define CONFIRM_RIGHT_A_X 218
#define CONFIRM_RIGHT_A_Y 168

#define CONFIRM_RIGHT_B_X 218
#define CONFIRM_RIGHT_B_Y 142

#define CONFIRM_RIGHT_TEXT_CANCEL_X 244
#define CONFIRM_RIGHT_TEXT_CANCEL_Y 148

#define CONFIRM_RIGHT_TEXT_CONFIRM_X 244
#define CONFIRM_RIGHT_TEXT_CONFIRM_Y 174

// SwapStone Choices

#define SWAPSTONE_STICK_X 157
#define SWAPSTONE_STICK_Y 78

#define SWAPSTONE_TEXT_SWAP_X 149
#define SWAPSTONE_TEXT_SWAP_Y 84

#define SWAPSTONE_TEXT_SHOP_X 153
#define SWAPSTONE_TEXT_SHOP_Y 66

#define SWAPSTONE_TEXT_WARP_X 187
#define SWAPSTONE_TEXT_WARP_Y 84

// Confirm/Cancel (right-aligned)

#define HAGGLE_A_X 218
#define HAGGLE_A_Y 94

#define HAGGLE_B_X 218
#define HAGGLE_B_Y 68

#define HAGGLE_STICK_X 218
#define HAGGLE_STICK_Y 42

#define HAGGLE_TEXT_CHANGE_X 244
#define HAGGLE_TEXT_CHANGE_Y 48

#define HAGGLE_TEXT_CANCEL_X 244
#define HAGGLE_TEXT_CANCEL_Y 74

#define HAGGLE_TEXT_CONFIRM_X 244
#define HAGGLE_TEXT_CONFIRM_Y 100

/*0x0*/ static u8 sMenuState;
/*0x4*/ static Texture* sTexButtonA;
/*0x8*/ static Texture* sTexButtonB;
/*0xC*/ static Texture* sTexControlStick;
/*0x10*/ static s32 bss_10; //Set to 0x40000 but otherwise unused
/*0x14*/ static s32 bss_14; //Set to 0x40000 but otherwise unused
/*0x18*/ static s32 bss_18; //Set to 0x40000 but otherwise unused
/*0x1C*/ static GameTextChunk* sGametext;
/*0x20*/ static f32 sOpacity;

typedef enum {
    SelectionMenu_STRING_WARP,
    SelectionMenu_STRING_SWAP,
    SelectionMenu_STRING_SHOP,
    SelectionMenu_STRING_CONFIRM,
    SelectionMenu_STRING_CANCEL,
    SelectionMenu_STRING_CHANGE
} SelectionMenu_Strings;

// offset: 0x0 | ctor
void selection_ctor(void* dll) {
    sTexButtonA = tex_load_deferred(TEXTABLE_274_AButton_Anim);
    sTexButtonA->animSpeed = 40;
    
    bss_10 = 0x40000;
    
    sTexButtonB = tex_load_deferred(TEXTABLE_275_BButton_Anim);
    sTexButtonB->animSpeed = 40;
    
    bss_14 = 0x40000;
    
    sTexControlStick = tex_load_deferred(TEXTABLE_4FA_ControlStick_Anim);
    sTexControlStick->animSpeed = 40;
    
    bss_18 = 0x40000;

    sGametext = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_1FF_SwapStone_Menu);

    sOpacity = 0.0f;
}

// offset: 0xF8 | dtor
void selection_dtor(void* dll) {
    tex_free(sTexButtonA);
    tex_free(sTexButtonB);
    tex_free(sTexControlStick);
    mmFree(sGametext);
}

// offset: 0x174 | func: 0 | export: 0
s32 selection_update1(void) {
    //Fade in/out
    if (sMenuState == SelectionMenu_STATE_0_Fade_Out) {
        sOpacity -= gUpdateRateF * 8.0f;
    } else {
        sOpacity += gUpdateRateF * 8.0f;
    }
    
    //Clamp opacity
    if (sOpacity > 255.0f) {
        sOpacity = 255.0f;
    } else if (sOpacity < 0.0f) {
        sOpacity = 0.0f;
    }
    
    return 0;
}

// offset: 0x230 | func: 1 | export: 1
void selection_update2(void) {

}

// offset: 0x238 | func: 2 | export: 2
void selection_draw(Gfx** gdl, Mtx** mtx, Vertex** vtx) {
    /*0x0*/ static s32 dFrameButtonA = 0;
    /*0x4*/ static s32 dFrameButtonB = 0;
    /*0x8*/ static s32 dFrameControlStick = 0;

    font_window_set_coords(1, 0, 0, GET_VIDEO_WIDTH(vi_get_current_size()), GET_VIDEO_HEIGHT(vi_get_current_size()));
    font_window_flush_strings(1);
    font_window_use_font(1, 1);
    
    switch (sMenuState) {
    case SelectionMenu_STATE_3_Confirm_Left:
        //Draw B button & A button
        rcp_screen_full_write(gdl, sTexButtonB, CONFIRM_LEFT_B_X, CONFIRM_LEFT_B_Y, 0, dFrameButtonB >> 8, (u8)sOpacity, 0);
        rcp_screen_full_write(gdl, sTexButtonA, CONFIRM_LEFT_A_X, CONFIRM_LEFT_A_Y, 0, dFrameButtonA >> 8, (u8)sOpacity, 0);
        
        //"Confirm" / "Cancel" text (drop-shadow)
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, (u8)sOpacity);
        font_window_add_string_xy(1, CONFIRM_LEFT_TEXT_CANCEL_X,  CONFIRM_LEFT_TEXT_CANCEL_Y,  sGametext->strings[SelectionMenu_STRING_CANCEL],  1, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, CONFIRM_LEFT_TEXT_CONFIRM_X, CONFIRM_LEFT_TEXT_CONFIRM_Y, sGametext->strings[SelectionMenu_STRING_CONFIRM], 1, ALIGN_TOP_LEFT);
        
        //"Confirm" / "Cancel" text (main)
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, (((u8)sOpacity) * 0x96) >> 8);
        font_window_add_string_xy(1, (CONFIRM_LEFT_TEXT_CANCEL_X  + SHADOW_X), (CONFIRM_LEFT_TEXT_CANCEL_Y  + SHADOW_Y), sGametext->strings[SelectionMenu_STRING_CANCEL],  2, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, (CONFIRM_LEFT_TEXT_CONFIRM_X + SHADOW_X), (CONFIRM_LEFT_TEXT_CONFIRM_Y + SHADOW_Y), sGametext->strings[SelectionMenu_STRING_CONFIRM], 2, ALIGN_TOP_LEFT);
        break;

    case SelectionMenu_STATE_2_Confirm_Right:
        //Draw B button & A button
        rcp_screen_full_write(gdl, sTexButtonB, CONFIRM_RIGHT_B_X, CONFIRM_RIGHT_B_Y, 0, dFrameButtonB >> 8, (u8)sOpacity, 0);
        rcp_screen_full_write(gdl, sTexButtonA, CONFIRM_RIGHT_A_X, CONFIRM_RIGHT_A_Y, 0, dFrameButtonA >> 8, (u8)sOpacity, 0);
        
        //"Confirm" / "Cancel" text (drop-shadow)
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, (u8)sOpacity);
        font_window_add_string_xy(1, CONFIRM_RIGHT_TEXT_CANCEL_X,  CONFIRM_RIGHT_TEXT_CANCEL_Y,  sGametext->strings[SelectionMenu_STRING_CANCEL],  1, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, CONFIRM_RIGHT_TEXT_CONFIRM_X, CONFIRM_RIGHT_TEXT_CONFIRM_Y, sGametext->strings[SelectionMenu_STRING_CONFIRM], 1, ALIGN_TOP_LEFT);
        
        //"Confirm" / "Cancel" text (main)
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, (((u8)sOpacity) * 0x96) >> 8);
        font_window_add_string_xy(1, (CONFIRM_RIGHT_TEXT_CANCEL_X  + SHADOW_X), (CONFIRM_RIGHT_TEXT_CANCEL_Y  + SHADOW_Y), sGametext->strings[SelectionMenu_STRING_CANCEL],  2, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, (CONFIRM_RIGHT_TEXT_CONFIRM_X + SHADOW_X), (CONFIRM_RIGHT_TEXT_CONFIRM_Y + SHADOW_Y), sGametext->strings[SelectionMenu_STRING_CONFIRM], 2, ALIGN_TOP_LEFT);
        break;

    case SelectionMenu_STATE_1_SwapStone_Choices:
        //Draw control stick
        rcp_screen_full_write(gdl, sTexControlStick, SWAPSTONE_STICK_X, SWAPSTONE_STICK_Y, 0, dFrameControlStick >> 8, (u8)sOpacity, 0);
        
        //"Warp" / "Swap" / "Shop" text (drop-shadow)
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, (u8)sOpacity);
        font_window_add_string_xy(1, SWAPSTONE_TEXT_WARP_X, SWAPSTONE_TEXT_WARP_Y,  sGametext->strings[SelectionMenu_STRING_WARP], 1, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, SWAPSTONE_TEXT_SWAP_X, SWAPSTONE_TEXT_SWAP_Y,  sGametext->strings[SelectionMenu_STRING_SWAP], 1, ALIGN_TOP_RIGHT);
        font_window_add_string_xy(1, SWAPSTONE_TEXT_SHOP_X, SWAPSTONE_TEXT_SHOP_Y,  sGametext->strings[SelectionMenu_STRING_SHOP], 1, -0x8000);
        
        //"Warp" / "Swap" / "Shop" text (main)
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, (((u8)sOpacity) * 0x96) >> 8);
        font_window_add_string_xy(1, (SWAPSTONE_TEXT_WARP_X + SHADOW_X), (SWAPSTONE_TEXT_WARP_Y + SHADOW_Y), sGametext->strings[SelectionMenu_STRING_WARP], 2, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, (SWAPSTONE_TEXT_SWAP_X + SHADOW_X), (SWAPSTONE_TEXT_SWAP_Y + SHADOW_Y), sGametext->strings[SelectionMenu_STRING_SWAP], 2, ALIGN_TOP_RIGHT);
        font_window_add_string_xy(1, (SWAPSTONE_TEXT_SHOP_X + SHADOW_X), (SWAPSTONE_TEXT_SHOP_Y + SHADOW_Y), sGametext->strings[SelectionMenu_STRING_SHOP], 2, -0x8000);
        break;

    case SelectionMenu_STATE_4_Confirm_ShopKeeper:
        //Draw control stick, B button, & A button
        rcp_screen_full_write(gdl, sTexControlStick, HAGGLE_STICK_X, HAGGLE_STICK_Y, 0, dFrameControlStick >> 8, (u8)sOpacity, 0);
        rcp_screen_full_write(gdl, sTexButtonB,      HAGGLE_B_X,     HAGGLE_B_Y,     0, dFrameButtonB >> 8, (u8)sOpacity, 0);
        rcp_screen_full_write(gdl, sTexButtonA,      HAGGLE_A_X,     HAGGLE_A_Y,     0, dFrameButtonA >> 8, (u8)sOpacity, 0);
        
        //"Change" / "Cancel" / "Confirm" text (drop-shadow)
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, (u8)sOpacity);
        font_window_add_string_xy(1, HAGGLE_TEXT_CHANGE_X,  HAGGLE_TEXT_CHANGE_Y,  sGametext->strings[SelectionMenu_STRING_CHANGE],  1, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, HAGGLE_TEXT_CANCEL_X,  HAGGLE_TEXT_CANCEL_Y,  sGametext->strings[SelectionMenu_STRING_CANCEL],  1, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, HAGGLE_TEXT_CONFIRM_X, HAGGLE_TEXT_CONFIRM_Y, sGametext->strings[SelectionMenu_STRING_CONFIRM], 1, ALIGN_TOP_LEFT);
        
        //"Change" / "Cancel" / "Confirm" text (main)
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, (((u8)sOpacity) * 0x96) >> 8);
        font_window_add_string_xy(1, (HAGGLE_TEXT_CHANGE_X  + SHADOW_X), (HAGGLE_TEXT_CHANGE_Y  + SHADOW_Y), sGametext->strings[SelectionMenu_STRING_CHANGE],  1, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, (HAGGLE_TEXT_CANCEL_X  + SHADOW_X), (HAGGLE_TEXT_CANCEL_Y  + SHADOW_Y), sGametext->strings[SelectionMenu_STRING_CANCEL],  2, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, (HAGGLE_TEXT_CONFIRM_X + SHADOW_X), (HAGGLE_TEXT_CONFIRM_Y + SHADOW_Y), sGametext->strings[SelectionMenu_STRING_CONFIRM], 2, ALIGN_TOP_LEFT);
        break;
    }
    
    font_window_draw(gdl, 0, 0, 1);
}

// offset: 0x1340 | func: 3 | export: 3
/** 
  * Sets which selection screen to show, or whether to fade out the current selection screen. 
  * (See `SelectionMenu_States`) 
  */
void selection_set_selection_state(SelectionMenu_States state) {
    sMenuState = state;
}
