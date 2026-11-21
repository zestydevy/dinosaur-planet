#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/fonts.h"
#include "sys/gfx/textable.h"
#include "sys/gfx/texture.h"
#include "sys/menu.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/vi.h"
#include "dlls/objects/768_SPshop.h"
#include "sys/rcp.h"

/*0x0*/ static Texture* scarabTexture;

// offset: 0x0 | ctor
void dll_77_ctor(void *dll) {
    scarabTexture = texLoadTexture(TEXTABLE_47A);
}

// offset: 0x44 | dtor
void dll_77_dtor(void *dll) {
    texFreeTexture(scarabTexture);
}

// offset: 0x84 | func: 0 | export: 0
s32 dll_77_update1(void) {
    return 0;
}

// offset: 0x90 | func: 1 | export: 1
void dll_77_update2(void) { }

// offset: 0x98 | func: 2 | export: 2
void dll_77_draw(Gfx **gdl, Mtx **mtxs, Vtx **vtxs) {
    char scarabCountString[3];
    f32 distance;
    Object* shop;
    s32 sp40;
    s32 scarabCount;
    s32 total;

    scarabCount = 0;
    sp40 = 0;
    total = 0;
    distance = 10000.0f;
    if (D_800A7D94 != 0) {
        menu_func_8000FB2C(gdl);
    }
    
    shop = objGetNearestTypeTo(OBJTYPE_LevelControl, objGetPlayer(), &distance);
    if (shop) {
        dll_SPShop(shop)->GetMinigameStats(shop, &sp40, &scarabCount, &total);
    }
    
    rcpScreenFullWrite(gdl, scarabTexture, 252, 198, 0, 0, 0xFF, SCREEN_WRITE_TRANSLUCENT);
    
    scarabCount = (total - scarabCount) + sp40;
    if (scarabCount < 0) {
        scarabCount = 0;
    }
    sprintf(scarabCountString, "%2d", (int)scarabCount);
    
    fontWindowSetCoords(1, 0, 0, GET_VIDEO_WIDTH(viGetCurrentSize()), GET_VIDEO_HEIGHT(viGetCurrentSize()));
    fontWindowFlushStrings(1);
    fontWindowUseFont(1, FONT_DINO_SUBTITLE_FONT_1);
    fontWindowSetTextColour(1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
    fontWindowAddStringXY(1, 270, 202, scarabCountString, 1, ALIGN_TOP_LEFT);
    fontWindowDraw(gdl, mtxs, vtxs, 1);
}
