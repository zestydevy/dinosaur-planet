#include "common.h"

#define END {NULL, -1, 0, 0}

/*0x4*/ static TextureTile dTexTiles[] = {
    {NULL, 0, -93,  -32},
    {NULL, 0, -62,  -32}, 
    {NULL, 0, -31,  -32}, 
    {NULL, 0, 0,    -32}, 
    {NULL, 0, 31,   -32},
    {NULL, 0, 62,   -32}, 
    END, 
};

// offset: 0x0 | ctor
void dll_72_ctor(void* dll) {
    s32 i;

    i = 0;
    while (dTexTiles[i].animProgress != -1) {
        dTexTiles[i].tex = tex_load_deferred(i + TEXTABLE_1EE);
        i++;
    }
}

// offset: 0x88 | dtor
void dll_72_dtor(void* dll) {
    s32 i;
    
    i = 0;
    while (dTexTiles[i].animProgress != -1) {
        tex_free(dTexTiles[i].tex);
        i++;
    }
}

// offset: 0x100 | func: 0 | export: 0
s32 dll_72_update1(void) {
    return 0;
}

// offset: 0x10C | func: 1 | export: 1
void dll_72_update2(void) {

}

// offset: 0x114 | func: 2 | export: 2
void dll_72_draw(Gfx** gdl, Mtx** mtx, Vertex** vtx) {
    rcp_tile_write(gdl, dTexTiles, 160, 160, 0xFF, 0xFF, 0xFF, 0xFF);
}
