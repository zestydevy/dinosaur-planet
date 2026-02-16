#include "dlls/engine/1_ui.h"
#include "sys/menu.h"
#include "dll.h"

// offset: 0x0 | ctor
void dll_19_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_19_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_19_func_18(void) {
    gDLL_1_UI->vtbl->func_308();
}

// offset: 0x5C | func: 1 | export: 1
void dll_19_func_5C(void) {
    gDLL_1_UI->vtbl->func_35C();
}

// offset: 0xA0 | func: 2 | export: 2
void dll_19_func_A0(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    if (D_800A7D94 != 0) {
        func_8000FB2C(gdl);
    }
    gDLL_1_UI->vtbl->func_BB8(gdl, mtxs, vtxs);
}
