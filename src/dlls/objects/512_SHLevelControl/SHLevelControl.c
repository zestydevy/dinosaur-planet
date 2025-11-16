#include "dll.h"

/*0x0*/ static u32 _data_0[] = {
    0x00610061, 0x00610061, 0x01b601b9, 0x01bc01bc, 0x01b60061, 0x00610061, 0x00610061, 0x01b901bf, 
    0x01bf01bf, 0x01b60061, 0x00610061, 0x00610061, 0x01b60061, 0x00610061
};
/*0x38*/ static u32 _data_38[] = {
    0x005f005f, 0x005f005f, 0x01b501b8, 0x01bb01bb, 0x01b5005f, 0x005f005f, 0x005f005f, 0x005f01be, 
    0x01be01be, 0x01b5005f, 0x005f005f, 0x005f005f, 0x005f005f, 0x005f005f
};
/*0x70*/ static u32 _data_70[] = {
    0x00600060, 0x00600060, 0x01b701ba, 0x01bd01bd, 0x01b70060, 0x00600060, 0x00600060, 0x01ba01c0, 
    0x01c001c0, 0x01b70060, 0x00600060, 0x00600060, 0x01b70060, 0x00600060
};
/*0xA8*/ static u32 _data_A8[] = {
    0x01c301c3, 0x01c301c3, 0x01c301c3, 0x01c301a8, 0x01c301c3, 0x01c301c3, 0x01c301c3, 0x01c301c3, 
    0x01a801a8, 0x01c301c3, 0x01c301c3, 0x01c301c3, 0x01c301c3, 0x01c301c3
};
/*0xE0*/ static u32 _data_E0[] = {
    0x01910191, 0x01910191, 0x01910191, 0x01910191, 0x01910191, 0x01910191, 0x01910191, 0x01910191, 
    0x01910191, 0x01910191, 0x01910191, 0x01910191, 0x01910191, 0x01910191
};
/*0x118*/ static u32 _data_118[] = {
    0x01920192, 0x01920192, 0x01920192, 0x01920192, 0x01920192, 0x01920192, 0x01920192, 0x01920192, 
    0x01920192, 0x01920192, 0x01920192, 0x01920192, 0x01920192, 0x01920192
};

// offset: 0x0 | ctor
void SHLevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void SHLevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHLevelControl_setup(Object *self, ObjSetup *setup, s32 arg2) {
    self->unkB0 |= 0x6000;
    gDLL_7_Newday->vtbl->func16(7);
    gDLL_7_Newday->vtbl->func18((s32)&_data_38, (s32)&_data_0, (s32)&_data_70, (s32)&_data_A8);
    gDLL_7_Newday->vtbl->func19((s32)&_data_E0, (s32)&_data_118, 0, 0);
    gDLL_7_Newday->vtbl->func20(1, 0xF4, 0, 0);
}

// offset: 0x118 | func: 1 | export: 1
void SHLevelControl_control(Object *self) { }

// offset: 0x124 | func: 2 | export: 2
void SHLevelControl_update(Object *self) { }

// offset: 0x130 | func: 3 | export: 3
void SHLevelControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x148 | func: 4 | export: 4
void SHLevelControl_free(Object *self, s32 a1) {
    gDLL_7_Newday->vtbl->func16(0);
}

// offset: 0x198 | func: 5 | export: 5
u32 SHLevelControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1A8 | func: 6 | export: 6
u32 SHLevelControl_get_data_size(Object *self, u32 a1) {
    return 0;
}
