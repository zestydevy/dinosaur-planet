#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "sys/objtype.h"
#include "sys/main.h"
#include "dll.h"
#include "sys/joypad.h"
#include "functions.h"

typedef struct {
    u8 unk0;
} DLL308_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 pitch;
/*1A*/ u8 roll;
/*1B*/ u8 flags;
/*1C*/ s16 gamebit;
/*1E*/ s16 gamebit2;
/*20*/ s8 objectSeqIndex;
/*21*/ s8 modelInstIdx;
/*22*/ s16 gamebit3;
/*24*/ s16 unk24;
} UseObj_Setup;

static int UseObj_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void UseObj_ctor(void *dll) { }

// offset: 0xC | dtor
void UseObj_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void UseObj_setup(Object *self, UseObj_Setup *setup, s32 arg2) {
    DLL308_Data *objdata;

    self->srt.yaw = setup->yaw << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.roll = setup->roll << 8;
    self->animCallback = UseObj_anim_callback;
    self->modelInstIdx = setup->modelInstIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        // diPrintf("USEOBJ.c: modelno out of range romdefno=%d\n"), self->modelInstIdx);
        self->modelInstIdx = 0;
    }
    objdata = self->data;
    objdata->unk0 = main_get_bits(setup->gamebit);
    obj_add_object_type(self, OBJTYPE_17);
    if (setup->flags & 1 && objdata->unk0 != 0) {
        self->opacity = 0;
    }
}

// offset: 0x110 | func: 1 | export: 1
void UseObj_control(Object *self) {
    DLL308_Data *objdata;
    UseObj_Setup *setup;
    s32 var_a2;

    objdata = self->data;
    setup = (UseObj_Setup*)self->setup;
    objdata->unk0 = main_get_bits(setup->gamebit);
    if (objdata->unk0 == 0) {
        self->unkAF &= ~8;
        if (setup->gamebit3 != -1) {
            if (main_get_bits(setup->gamebit3)) {
                self->unkAF &= ~0x10;
            } else {
                self->unkAF |= 0x10;
                if (setup->flags & 0x10) {
                    self->unkAF |= 8;
                }
            }
        } else {
            self->unkAF &= ~0x10;
        }
        if (self->unkAF & 1 && (setup->gamebit2 == -1 || gDLL_1_UI->vtbl->func_DF4(setup->gamebit2))) {
            if (setup->objectSeqIndex != -1) {
                gDLL_3_Animation->vtbl->func17(setup->objectSeqIndex, self, -1);
            }
            if (!(setup->flags & 4)) {
                main_set_bits(setup->gamebit, 1);
            }
            if (setup->flags & 8) {
                main_set_bits(setup->gamebit3, 0);
            } else {
                objdata->unk0 = 1;
                self->unkDC = 1;
            }
            joy_set_button_mask(0, A_BUTTON);
        }
    } else {
        if (self->unkDC == 0) {
            if (setup->objectSeqIndex != -1) {
                if (setup->unk24 != 0) {
                    gDLL_3_Animation->vtbl->func20(self, setup->unk24);
                    var_a2 = 1;
                    if (setup->flags & 0x20) {
                        var_a2 = 3;
                    }
                    if (setup->flags & 0x40) {
                        var_a2 |= 4;
                    }
                    if (setup->flags & 0x80) {
                        var_a2 |= 8;
                    }
                    gDLL_3_Animation->vtbl->func17(setup->objectSeqIndex, self, var_a2);
                }
            }
            self->unkDC = 1;
        }
        self->unkAF |= 8;
    }
}

// offset: 0x3B8 | func: 2 | export: 2
void UseObj_update(Object *self) {
    if (self->def->flags & 1 && self->unk74) {
        func_80036438(self);
    }
}

// offset: 0x410 | func: 3 | export: 3
void UseObj_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x464 | func: 4 | export: 4
void UseObj_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_17);
}

// offset: 0x4A4 | func: 5 | export: 5
u32 UseObj_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x4B4 | func: 6 | export: 6
u32 UseObj_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL308_Data);
}

// offset: 0x4C8 | func: 7
static int UseObj_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    UseObj_Setup *setup;

    setup = (UseObj_Setup*)self->setup;
    self->unkAF |= 8;
    if (animObjData->unk8D != 0) {
        if (setup->flags & 4 && animObjData->unk8D == 1) {
            main_set_bits(setup->gamebit, 1);
        }
        if (animObjData->unk8D == 2) {
            if (setup->unk24 != 0) {
                gDLL_3_Animation->vtbl->func21(animObjData, setup->unk24);
            }
        }
        animObjData->unk8D = 0;
    }
    return 0;
}

/*0x0*/ static const char str_0[] = "USEOBJ.c: modelno out of range romdefno=%d\n";
/*0x2C*/ static const char str_2C[] = "";
