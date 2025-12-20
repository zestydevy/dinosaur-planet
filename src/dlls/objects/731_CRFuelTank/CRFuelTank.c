#include "dll.h"
#include "functions.h"
#include "game/gamebits.h"
#include "sys/main.h"
#include "game/objects/object_id.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/hitbox.h"
#include "sys/objhits.h"

void func_80026160(Object* obj);

typedef struct {
/*00*/ u8 unk0[0x4];
} CRFuelTank_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 _unk18;
/*19*/ u8 _unk19;
/*1A*/ s16 unk1A;
/*1C*/ u8 _unk1C;
/*1D*/ u8 _unk1D;
/*1E*/ s16 gamebit;
} CRFuelTank_Setup;

// offset: 0x0 | ctor
void CRFuelTank_ctor(void *dll) { }

// offset: 0xC | dtor
void CRFuelTank_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CRFuelTank_setup(Object *self, CRFuelTank_Setup *setup, s32 arg2) {
    self->unkB0 |= 0x4000;
    func_8002674C(self);
    func_80026128(self, 0x1D, setup->unk1A / 10, 0);
    if ((setup->gamebit != NO_GAMEBIT) && (main_get_bits(setup->gamebit))) {
        func_80026160(self);
        func_800267A4(self);
        self->srt.flags |= 0x4000;
    }
}

// offset: 0xFC | func: 1 | export: 1
void CRFuelTank_control(Object *self) {
    if (self->opacity < OBJECT_OPACITY_MAX) {
        if (gUpdateRate >= self->opacity) {
            self->srt.flags |= 0x4000;
            return;
        }
        self->opacity -= 2 * gUpdateRate;
        self->srt.scale *= 1.1f;
        self->unkE0 = 0;
    }
}

// offset: 0x164 | func: 2 | export: 2
void CRFuelTank_update(Object *self) {
    ObjectHitInfo *hitinfo;
    CRFuelTank_Setup *setup;

    hitinfo = self->objhitInfo;
    setup = (CRFuelTank_Setup*)self->setup;
    if (hitinfo->unk48 && hitinfo->unk48->id == OBJ_CRSnowBike) {
        func_80026160(self);
        func_800267A4(self);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B3C_Machinery_Charge_Up, MAX_VOLUME, NULL, NULL, 0, NULL);
        self->opacity = 250;
        self->unkE0 = 1;
        if (setup->gamebit != NO_GAMEBIT) {
            main_set_bits(setup->gamebit, 1);
        }
    }
}

// offset: 0x258 | func: 3 | export: 3
void CRFuelTank_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x270 | func: 4 | export: 4
void CRFuelTank_free(Object *self, s32 a1) { }

// offset: 0x280 | func: 5 | export: 5
u32 CRFuelTank_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x290 | func: 6 | export: 6
u32 CRFuelTank_get_data_size(Object *self, u32 a1) {
    return sizeof(CRFuelTank_Data);
}
