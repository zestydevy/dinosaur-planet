#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/74_picmenu.h"
#include "functions.h"
#include "sys/fonts.h"
#include "sys/gfx/gx.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/memory.h"

#include "prevent_bss_reordering.h"

/*0x0*/ static PicMenuItem _data_0[] = {
    /*0*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 165, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 175, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    )
};
/*0x3C*/ static PicMenuItem _data_3C[] = {
    /*0*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 4, /*rightLink*/ 1, /*overrideWith*/ -1
    ),
    /*1*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 0, /*rightLink*/ 2, /*overrideWith*/ -1
    ),
    /*2*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 1, /*rightLink*/ 3, /*overrideWith*/ -1
    ),
    /*3*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 2, /*rightLink*/ 4, /*overrideWith*/ -1
    ),
    /*4*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_FLAG_INVISIBLE | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 3, /*rightLink*/ 0, /*overrideWith*/ -1
    ),
    /*5*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 150, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 180, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ -1, /*downLink*/ 6, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*6*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 165, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ 5, /*downLink*/ 7, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1
    ),
    /*7*/ NEW_PICMENU_ITEM(
    /*textX*/ 160, /*textY*/ 180, /*innerWidth*/ 0, 
    /*itemX*/ 160, /*itemY*/ 187, /*textureID*/ -1, /*outerWidth*/ 0, 
    /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_ALWAYS_REDRAW | PICMENU_USE_TEXT_COLOR | PICMENU_RAISED_EFFECT, 
    /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
    /*upLink*/ 6, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ 0
    ),
};
/*0x21C*/ static s16 _data_21C[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0007, 0x0008,
};

/*0x0*/ static u8 _bss_0;
/*0x4*/ static GameTextChunk* _bss_4;
/*0x8*/ static GplayStruct4* _bss_8;
/*0xC*/ static s8 _bss_C;
/*0xD*/ static s8 _bss_D;
/*0xE*/ static u8 _bss_E[0x2];
/*0x10*/ static Texture* _bss_10;

// offset: 0x0 | ctor
void dll_62_ctor(s32 arg0) {
    int total_strings;
    s32 index;

    total_strings = 8;
    
    _bss_10 = queue_load_texture_proxy(0xC5);
    func_800379D0(0, 0, 0);

    //Set language and get text
    _bss_8 = gDLL_29_Gplay->vtbl->func_930();
    gDLL_21_Gametext->vtbl->set_bank(_bss_8->unk0x0);
    _bss_4 = gDLL_21_Gametext->vtbl->get_chunk(238);

    //Set "Press Start" text?
    _data_0->text = _bss_4->strings[6];

    for (index = 0; index < total_strings; index++){
        _data_3C[index].text = _bss_4->strings[_data_21C[index]];
    }
    
    if (menu_get_previous() == 3) {
        gDLL_74_Picmenu->vtbl->set_items(_data_0, 1, 0, 0, 0, 0, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
        _bss_0 = 0;
    } else {
        _data_3C[7].overrideWith = _bss_8->unk0x0; //Set language index?
        gDLL_74_Picmenu->vtbl->set_items(_data_3C, 8, 5, 0, 0, 0, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
        _bss_0 = 1;
    }
    
    _bss_D = 0;
    _bss_C = 0;
}

// offset: 0x310 | dtor
void dll_62_dtor(void *dll){

}

// offset: 0x31C | func: 0 | export: 0
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/62_mainmenu/dll_62_func_31C.s")
#else
void dll_62_func_8AC(void);

s32 dll_62_func_31C(void) {
    // PicMenuItem* var_a0;
    // s16 temp_t8;
    s32 items;
    s32 var_v1_2;
    s32 temp_v0;
    // s32 temp_v0_2;
    // s8 var_v0;
    // u8 temp_v1;
    s32 var_v1;


    
    var_v1 = delayByte;
    if (var_v1 >= 4) {
        var_v1 = 3;
    }
    
    if (_bss_C > 0) {
        _bss_C -= var_v1;
    }
    
    if (_bss_D != 0) {
        if (_bss_C >= 0xD && _bss_C < 0xD) {
            func_8005D410(0xE, get_ossched(), 0);
            dll_62_func_8AC();
            func_80041D20(0);
            func_80041C6C(0);
            if (_bss_D == 5) {
                gDLL_29_Gplay->vtbl->func_8D8();
            }
        }
        if (_bss_C <= 0) {
            func_800141A4(1, 0, 1, (s32) _bss_D);
        }
        if (_bss_C < 0xD) {
            return 1;
        }
        // goto block_29;
        return 0;
    }
    
    temp_v0 = gDLL_74_Picmenu->vtbl->update(_bss_C, &_bss_C, _bss_D);
    
    if (_bss_0 == 0) {
        if (temp_v0 == PICMENU_ACTION_SELECT) {
            gDLL_74_Picmenu->vtbl->clear_items();
            _data_3C[7].overrideWith = (u8) _bss_8->unk0x0;
            gDLL_74_Picmenu->vtbl->set_items(_data_3C, 8, 5, NULL, 0, 0, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
            _bss_0 = 1;
        }
        // goto block_29;
        return 0;
    }
    
    if (temp_v0 == PICMENU_ACTION_SELECT) {
        // temp_v0_2 = gDLL_74_Picmenu->vtbl->get_selected_item();
        switch (gDLL_74_Picmenu->vtbl->get_selected_item()) {
        case 5:
            gDLL_28_ScreenFade->vtbl->fade(0x14, 1);
            _bss_D = 5;
            _bss_C = 0x23;
            return 0;
        case 6:
            gDLL_28_ScreenFade->vtbl->fade(0x14, 1);
            _bss_D = 7;
            _bss_C = 0x23;
            return 0;
        }
    } else {
        _bss_8->unk0x0 = gDLL_74_Picmenu->vtbl->get_item_override(7);
        // temp_v1 = _bss_8->unk0x0;
        if (gDLL_21_Gametext->vtbl->curr_bank() != _bss_8->unk0x0) {
            gDLL_21_Gametext->vtbl->set_bank((s8) _bss_8->unk0x0);
            mmFree(_bss_4);
            _bss_4 = gDLL_21_Gametext->vtbl->get_chunk(0xEE);

            items = 8;
            
            for (var_v1_2 = 0; var_v1_2 < items; var_v1_2++){
                _data_3C[var_v1_2].text = _bss_4->strings[_data_21C[var_v1_2]];
            }
            
            gDLL_74_Picmenu->vtbl->update_text(_data_3C);
        }
// block_29:
        return 0;
    }
}
#endif

// offset: 0x764 | func: 1 | export: 1
void dll_62_func_764(void){

}

// offset: 0x76C | func: 2 | export: 2
void dll_62_func_76C(Gfx** gfx, Mtx** mtx, Vertex** vtx) {
    if (!_bss_D || _bss_C >= 0xB) {
        font_window_set_coords(1, 0, 0, get_some_resolution_encoded() & 0xFFFF, (get_some_resolution_encoded() >> 0x10));
        font_window_flush_strings(1);
        gDLL_74_Picmenu->vtbl->draw(gfx);
        if (func_80014C60()) {
            func_8003825C(gfx, _bss_10, 0x32, 0x32, 0, 0, 0xFF, 0);
        }
        font_window_draw(gfx, NULL, NULL, 1);
    }
}

// offset: 0x8AC | func: 3
void dll_62_func_8AC(void) {
    texture_destroy(_bss_10);
    gDLL_74_Picmenu->vtbl->clear_items();
    mmFree(_bss_4);
}


