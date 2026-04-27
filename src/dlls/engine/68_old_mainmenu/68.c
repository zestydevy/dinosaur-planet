#include "dlls/engine/20_screens.h"
#include "dlls/engine/21_gametext.h"
#include "game/gametexts.h"
#include "sys/fonts.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/menu.h"

extern DLL_20_screens *gDLL_20_Screens;
extern DLL_21_gametext *gDLL_21_Gametext;
extern DLL_Unknown* dll_throw_fault; //NOTE: BROKEN! This is a function now, not a DLL.

extern u8 D_8008C8B4;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} OldMainMenuUnk;

/*0x0*/ static OldMainMenuUnk data_0[] = {
    {0,   0,  0,   0,   -96, -32}, 
    {0,   0,  0,   0,   -64, -32}, 
    {0,   0,  0,   0,   -32, -32}, 
    {0,   0,  0,   0,   0,   -32}, 
    {0,   0,  0,   0,   32,  -32}, 
    {0,   0,  0,   0,   64,  -32}, 
    {0,   0,  0,   0,   -96, 0}, 
    {0,   0,  0,   0,   -64, 0}, 
    {0,   0,  0,   0,   -32, 0},
    {0,   0,  0,   0,   0,   0}, 
    {0,  0,  0,   0,   32,  0}, 
    {0,  0,  0,   0,   64,  0}, 
    {0,  0,  0,   0,   -96, 32}, 
    {0,  0,  0,   0,   -64, 32}, 
    {0,  0,  0,   0,   -32, 32}, 
    {0,  0,  0,   0,   0,   32}, 
    {0,  0,  0,   0,   32,  32}, 
    {0,  0,  0,   0,   64,  32}, 
    {0,  0,  -1,  -1,  0,   0}, 
    {0,  0,  0,   0,   0,   0}
};

/*0x0*/ static GameTextChunk* sGametext;
/*0x4*/ static s32 bss_4;
/*0x8*/ static s32 bss_8;
/*0xC*/ static s32 bss_C;
/*0x10*/ static s32 bss_10;

// offset: 0x0 | ctor
void old_mainmenu_ctor(void* dll) {
    sGametext = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_005_Title_Menu);
    bss_4 = 0;
    bss_8 = 0;
    bss_C = 0;
    bss_10 = 0;
    func_80010018(2);
}

// offset: 0x84 | dtor
void old_mainmenu_dtor(s32 arg0) {
    mmFree(sGametext);
}

// offset: 0xC4 | func: 0 | export: 0
s32 old_mainmenu_func_C4(void) {
    return 0;
}

// offset: 0xD0 | func: 1 | export: 1
void old_mainmenu_func_D0(void) {

}

// offset: 0xD8 | func: 2 | export: 2
void old_mainmenu_func_D8(Gfx** gdl, Mtx** mtx, Vertex** vtx) {   
    D_8008C8B4 = 1;
    
    bss_10 += gUpdateRate;    
    if (bss_10 >= 41) {
        bss_10 = 41;
    }

    // There were more than 2 SCREENS.bin images at one stage!
    // Maybe the static Rare logo from the One Hour Footage?
    gDLL_20_Screens->vtbl->show_screen(2);
    
    func_80014508(20);

    // NOTE: this section is very broken!
    // Maybe it used to call a DLL for the old title screen text seen in the One Hour Footage?
    dll_throw_fault->vtbl->func[7].withOneArg(bss_C);
    dll_throw_fault->vtbl->func[0].withOneArg(180);
    dll_throw_fault->vtbl->func[2].withFourArgs(0, (s32)sGametext->strings[0], 20, bss_4);
    if (dll_throw_fault->vtbl->func[5].withOneArgS32((s32)&bss_4) == FALSE) {
        menu_set(MENU_13);
    }
    
    // Maybe this was the blinking "PRESS START" text?
    {
        if (bss_8 > 20) {
            font_window_draw(gdl, NULL, NULL, 1);
        }
        bss_8 = (gUpdateRate + bss_8) & 0x3F;
    }

    bss_C = 1;
}
