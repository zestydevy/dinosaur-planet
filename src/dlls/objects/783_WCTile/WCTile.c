#include "dlls/objects/779_WCLevelControl.h"
#include "game/gamebits.h"
#include "sys/main.h"
#include "sys/objtype.h"
#include "dll.h"
#include "functions.h"

typedef struct {
    Object *levelCtrl;
    u8 _unk4[0x8 - 0x4];
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
} WCTile_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 _unk18;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
} WCTile_Setup;

// offset: 0x0 | ctor
void dll_783_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_783_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_783_setup(Object *self, WCTile_Setup *setup, s32 arg2) {
    WCTile_Data *objdata = (WCTile_Data*)self->data;

    self->srt.transl.y = setup->base.y + 25.0f;
    self->modelInstIdx = setup->unk19;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    objdata->unkC = (s16) setup->unk1A;
    self->shadow->flags |= (OBJ_SHADOW_FLAG_800 | OBJ_SHADOW_FLAG_10);
    self->opacity = 0;
}

// offset: 0x88 | func: 1 | export: 1
void dll_783_control(Object *self) {
    f32 sp44;
    f32 sp40;
    WCTile_Data *objdata;
    u8 sp3B;
    s32 opacity;

    sp44 = 100000.0f;
    objdata = self->data;
    if (objdata->levelCtrl == NULL) {
        objdata->levelCtrl = obj_get_nearest_type_to(OBJTYPE_10, self, &sp44);
        self->opacity = 0;
        return;
    }
    self->srt.yaw += (s16) (s32) (gUpdateRateF * 180.0f);
    if (objdata->unkE != 5) {
        sp3B = gDLL_7_Newday->vtbl->func8(&sp40);
        if (self->modelInstIdx == 1) {
            if (main_get_bits(BIT_812) != 0) {
                objdata->unkE = 5;
            } else if (sp3B) {
                objdata->unkE = 3;
            }
        } else {
            if (main_get_bits(BIT_813) != 0) { // if moon block puzzle complete
                objdata->unkE = 5;
            } else if (!sp3B) {
                objdata->unkE = 3;
            }
        }
    }
    switch (objdata->unkE) {
    case 0:
        if (self->modelInstIdx == 1) {
            ((DLL_779_WCLevelControl*)objdata->levelCtrl->dll)->vtbl->func11(objdata->unkC, &objdata->unk8, &objdata->unkA);
            ((DLL_779_WCLevelControl*)objdata->levelCtrl->dll)->vtbl->func7(&self->srt, objdata->unk8, objdata->unkA, &self->srt.transl.x, &self->srt.transl.z);
        } else {
            ((DLL_779_WCLevelControl*)objdata->levelCtrl->dll)->vtbl->func18(objdata->unkC, &objdata->unk8, &objdata->unkA);
            ((DLL_779_WCLevelControl*)objdata->levelCtrl->dll)->vtbl->func14(&self->srt, objdata->unk8, objdata->unkA, &self->srt.transl.x, &self->srt.transl.z);
        }
        objdata->unkE = 1;
        break;
    case 2:
        self->opacity = 0;
        break;
    case 5:
        self->opacity = 0;
        break;
    case 3:
        opacity = self->opacity - (gUpdateRate * 8);
        if (opacity < 0) {
            opacity = 0;
        }
        self->opacity = (u8) opacity;
        if (self->opacity == 0) {
            if (self->modelInstIdx == 1) {
                if (!sp3B) {
                    ((DLL_779_WCLevelControl*)objdata->levelCtrl->dll)->vtbl->func11(objdata->unkC, &objdata->unk8, &objdata->unkA);
                    ((DLL_779_WCLevelControl*)objdata->levelCtrl->dll)->vtbl->func7(&self->srt, objdata->unk8, objdata->unkA, &self->srt.transl.x, &self->srt.transl.z);
                    objdata->unkE = 4;
                }
            } else {
                if (sp3B) {
                    ((DLL_779_WCLevelControl*)objdata->levelCtrl->dll)->vtbl->func18(objdata->unkC, &objdata->unk8, &objdata->unkA);
                    ((DLL_779_WCLevelControl*)objdata->levelCtrl->dll)->vtbl->func14(&self->srt, objdata->unk8, objdata->unkA, &self->srt.transl.x, &self->srt.transl.z);
                    objdata->unkE = 4;
                }
            }
        }
        break;
    case 4:
        opacity = self->opacity + (gUpdateRate * 8);
        if (opacity > 0xC8) {
            opacity = 0xC8;
        }
        self->opacity = (u8) opacity;
        if (self->opacity >= 0xC8) {
            objdata->unkE = 1;
        }
        break;
    case 1:
    default:
        opacity = self->opacity + (gUpdateRate * 8);
        if (opacity > 0xC8) {
            opacity = 0xC8;
        }
        self->opacity = (u8) opacity;
        if (self->modelInstIdx == 1) {
            if (objdata->unkC != ((DLL_779_WCLevelControl*)objdata->levelCtrl->dll)->vtbl->func10(objdata->unk8, objdata->unkA)) {
                objdata->unkE = 2;
            }
        } else {
            if (objdata->unkC != ((DLL_779_WCLevelControl*)objdata->levelCtrl->dll)->vtbl->func17(objdata->unk8, objdata->unkA)) {
                objdata->unkE = 2;
            }
        }
        break;
    }
}

// offset: 0x510 | func: 2 | export: 2
void dll_783_update(Object *self) { }

// offset: 0x51C | func: 3 | export: 3
void dll_783_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x570 | func: 4 | export: 4
void dll_783_free(Object *self, s32 a1) { }

// offset: 0x580 | func: 5 | export: 5
u32 dll_783_get_model_flags(Object *self) {
    WCTile_Setup *setup = (WCTile_Setup*)self->setup;
    s8 modelIdx = setup->unk19;

    if (modelIdx >= self->def->numModels) {
        modelIdx = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x5B4 | func: 6 | export: 6
u32 dll_783_get_data_size(Object *self, u32 a1) {
    return sizeof(WCTile_Data);
}
