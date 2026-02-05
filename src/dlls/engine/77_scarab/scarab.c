#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "functions.h"
#include "game/objects/object.h"
#include "sys/fonts.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/texture.h"
#include "sys/menu.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "dlls/objects/768_SPshop.h"
#include "sys/rcp.h"

/*0x0*/ static Texture* scarabTexture;

// offset: 0x0 | ctor
void dll_77_ctor(void *dll) {
    scarabTexture = tex_load_deferred(1146);
}

// offset: 0x44 | dtor
void dll_77_dtor(void *dll) {
    tex_free(scarabTexture);
}

// offset: 0x84 | func: 0 | export: 0
s32 dll_77_update1(void) {
    return 0;
}

// offset: 0x90 | func: 1 | export: 1
void dll_77_update2(void) { }

// offset: 0x98 | func: 2 | export: 2
/** Possibly unused? The UI doesn't call this for the scarab counter, and the Scarab objects don't seem to either */
void dll_77_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
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
        func_8000FB2C(gdl);
    }
    
    object = obj_get_nearest_type_to(OBJTYPE_10, get_player(), &distance);
    if (object) {
        ((DLL_768_SPShop*)object->dll)->vtbl->func20(object, &sp40, &scarabCount, &sp38);
    }
    
    func_8003825C(gdl, scarabTexture, 0xFC, 0xC6, 0, 0, 0xFF, 0);
    
    scarabCount = (sp38 - scarabCount) + sp40;
    if (scarabCount < 0) {
        scarabCount = 0;
    }
    sprintf(scarabCountString, "%2d", (int)scarabCount);
    
    font_window_set_coords(1, 0, 0, GET_VIDEO_WIDTH(vi_get_current_size()), GET_VIDEO_HEIGHT(vi_get_current_size()));
    font_window_flush_strings(1);
    font_window_use_font(1, FONT_DINO_SUBTITLE_FONT_1);
    font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
    font_window_add_string_xy(1, 270, 202, scarabCountString, 1, ALIGN_TOP_LEFT);
    font_window_draw(gdl, mtxs, vtxs, 1);
}
