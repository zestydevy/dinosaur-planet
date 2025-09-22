#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "bss.h"
#include "functions.h"
#include "sys/fonts.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/texture.h"
#include "sys/menu.h"
#include "sys/objects.h"
#include "sys/objtype.h"

BSS_GLOBAL s8 D_800A7D94;

/*0x0*/ static Texture* scarabTexture;

// offset: 0x0 | ctor
void dll_77_ctor(s32 arg0) {
    scarabTexture = queue_load_texture_proxy(1146);
}

// offset: 0x44 | dtor
void dll_77_dtor(s32 arg0) {
    texture_destroy(scarabTexture);
}

// offset: 0x84 | func: 0 | export: 0
s32 dll_77_func_84(){
    return 0;
}

// offset: 0x90 | func: 1 | export: 1
void dll_77_func_90(){
}

// offset: 0x98 | func: 2 | export: 2
/** Possibly unused? The UI doesn't call this for the scarab counter, and the Scarab objects don't seem to either */
void dll_77_func_98(Gfx** gfx, Mtx** mtx, Vtx** vtx) {
    char scarabCountString[3];
    f32 distance;
    Object* object;
    s32 sp40;
    s32 scarabCount;
    s32 sp38;

    scarabCount = 0;
    sp40 = 0;
    sp38 = 0;
    distance = 10000.0f;
    if (D_800A7D94 != 0) {
        func_8000FB2C(gfx);
    }
    
    object = obj_get_nearest_type_to(0xA, get_player(), &distance);
    if (object) {
        ((DLL_Unknown*)object->dll)->vtbl->func[20].withFourArgs((s32)object, (s32)&sp40, (s32)&scarabCount, (s32)&sp38);
    }
    
    func_8003825C(gfx, scarabTexture, 0xFC, 0xC6, 0, 0, 0xFF, 0);
    
    scarabCount = (sp38 - scarabCount) + sp40;
    if (scarabCount < 0) {
        scarabCount = 0;
    }
    sprintf(scarabCountString, "%2d", (int)scarabCount);
    
    font_window_set_coords(1, 0, 0, get_some_resolution_encoded() & 0xFFFF, (get_some_resolution_encoded() >> 0x10));
    font_window_flush_strings(1);
    font_window_use_font(1, FONT_DINO_SUBTITLE_FONT_1);
    font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
    font_window_add_string_xy(1, 270, 202, scarabCountString, 1, ALIGN_TOP_LEFT);
    font_window_draw(gfx, (void*)mtx, (void*)vtx, 1);
}
