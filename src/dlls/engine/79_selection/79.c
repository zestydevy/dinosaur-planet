//NOTE: official name unknown, but used for SwapStone / ShopKeeper's selection screens

#include "common.h"
#include "game/gametexts.h"
#include "sys/fonts.h"
#include "sys/gfx/textable.h"
#include "sys/menu.h"

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
        rcp_screen_full_write(gdl, sTexButtonB, 30, 142, 0, dFrameButtonB >> 8, (u8)sOpacity, 0);
        rcp_screen_full_write(gdl, sTexButtonA, 30, 168, 0, dFrameButtonA >> 8, (u8)sOpacity, 0);
        
        //"Confirm" / "Cancel" text (drop-shadow)
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, (u8)sOpacity);
        font_window_add_string_xy(1, 56, 148, sGametext->strings[SelectionMenu_STRING_CANCEL],  1, 0);
        font_window_add_string_xy(1, 56, 174, sGametext->strings[SelectionMenu_STRING_CONFIRM], 1, 0);
        
        //"Confirm" / "Cancel" text (main)
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, (((u8)sOpacity) * 0x96) >> 8);
        font_window_add_string_xy(1, 55, 147, sGametext->strings[SelectionMenu_STRING_CANCEL],  2, 0);
        font_window_add_string_xy(1, 55, 173, sGametext->strings[SelectionMenu_STRING_CONFIRM], 2, 0);
        break;

    case SelectionMenu_STATE_2_Confirm_Right:
        //Draw B button & A button
        rcp_screen_full_write(gdl, sTexButtonB, 218, 142, 0, dFrameButtonB >> 8, (u8)sOpacity, 0);
        rcp_screen_full_write(gdl, sTexButtonA, 218, 168, 0, dFrameButtonA >> 8, (u8)sOpacity, 0);
        
        //"Confirm" / "Cancel" text (drop-shadow)
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, (u8)sOpacity);
        font_window_add_string_xy(1, 244, 148, sGametext->strings[SelectionMenu_STRING_CANCEL],  1, 0);
        font_window_add_string_xy(1, 244, 174, sGametext->strings[SelectionMenu_STRING_CONFIRM], 1, 0);
        
        //"Confirm" / "Cancel" text (main)
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, (((u8)sOpacity) * 0x96) >> 8);
        font_window_add_string_xy(1, 243, 147, sGametext->strings[SelectionMenu_STRING_CANCEL],  2, 0);
        font_window_add_string_xy(1, 243, 173, sGametext->strings[SelectionMenu_STRING_CONFIRM], 2, 0);
        break;

    case SelectionMenu_STATE_1_SwapStone_Choices:
        //Draw control stick
        rcp_screen_full_write(gdl, sTexControlStick, 0x9D, 0x4E, 0, dFrameControlStick >> 8, (u8)sOpacity, 0);
        
        //"Warp" / "Swap" / "Shop" text (drop-shadow)
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, (u8)sOpacity);
        font_window_add_string_xy(1, 187, 84,  sGametext->strings[SelectionMenu_STRING_WARP], 1, 0);
        font_window_add_string_xy(1, 149, 84,  sGametext->strings[SelectionMenu_STRING_SWAP], 1, 1);
        font_window_add_string_xy(1, 153, 66,  sGametext->strings[SelectionMenu_STRING_SHOP], 1, -0x8000);
        
        //"Warp" / "Swap" / "Shop" text (main)
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, (((u8)sOpacity) * 0x96) >> 8);
        font_window_add_string_xy(1, 186, 83,  sGametext->strings[SelectionMenu_STRING_WARP], 2, 0);
        font_window_add_string_xy(1, 148, 83,  sGametext->strings[SelectionMenu_STRING_SWAP], 2, 1);
        font_window_add_string_xy(1, 152, 65,  sGametext->strings[SelectionMenu_STRING_SHOP], 2, -0x8000);
        break;

    case SelectionMenu_STATE_4_Confirm_ShopKeeper:
        //Draw control stick, B button, & A button
        rcp_screen_full_write(gdl, sTexControlStick, 218, 42, 0, dFrameControlStick >> 8, (u8)sOpacity, 0);
        rcp_screen_full_write(gdl, sTexButtonB,      218, 68, 0, dFrameButtonB >> 8, (u8)sOpacity, 0);
        rcp_screen_full_write(gdl, sTexButtonA,      218, 94, 0, dFrameButtonA >> 8, (u8)sOpacity, 0);
        
        //"Change" / "Cancel" / "Confirm" text (drop-shadow)
        font_window_set_text_colour(1, 0xFF, 0xD7, 0x3D, 0xFF, (u8)sOpacity);
        font_window_add_string_xy(1, 244, 48,  sGametext->strings[SelectionMenu_STRING_CHANGE],  1, 0);
        font_window_add_string_xy(1, 244, 74,  sGametext->strings[SelectionMenu_STRING_CANCEL],  1, 0);
        font_window_add_string_xy(1, 244, 100, sGametext->strings[SelectionMenu_STRING_CONFIRM], 1, 0);
        
        //"Change" / "Cancel" / "Confirm" text (main)
        font_window_set_text_colour(1, 0, 0, 0, 0xFF, (((u8)sOpacity) * 0x96) >> 8);
        font_window_add_string_xy(1, 243, 47,  sGametext->strings[SelectionMenu_STRING_CHANGE],  1, 0);
        font_window_add_string_xy(1, 243, 73,  sGametext->strings[SelectionMenu_STRING_CANCEL],  2, 0);
        font_window_add_string_xy(1, 243, 99,  sGametext->strings[SelectionMenu_STRING_CONFIRM], 2, 0);
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
