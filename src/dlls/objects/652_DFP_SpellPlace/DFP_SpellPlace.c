#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/gfx/model.h"
#include "sys/objects.h"
#include "types.h"
#include "game/gamebits.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} DFP_SpellPlace_Setup;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
} DLL652_Data;

static void dll_652_func_254(Object* self);
static void dll_652_func_364(Object* self);
static int dll_652_func_45C(Object* a0, Object* a1, AnimObj_Data* a2, void* a3);

// offset: 0x0 | ctor
void dll_652_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_652_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_652_setup(Object* self, DFP_SpellPlace_Setup* setup, s32 arg2) {
    DLL652_Data* objdata;

    objdata = self->data;
    self->srt.yaw = setup->yaw << 8;
    self->unk0xbc = (ObjectCallback)dll_652_func_45C;
    objdata->unk0 = setup->unk1E;
    objdata->unk2 = setup->unk20;
    if (main_get_bits(objdata->unk2) != 0 && main_get_bits(objdata->unk0) != 0) {
        objdata->unk4 = 1;
    } else {
        self->unk0xaf |= 8;
    }
    self->unk0xb0 |= 0x6000;
}

// offset: 0xE0 | func: 1 | export: 1
void dll_652_control(Object* self) {
    Object* player;
    u8 mapSetupID;

    player = get_player();
    if ((player != NULL) && !(vec3_distance(&self->positionMirror, &player->positionMirror) > 100.0f)) {
        mapSetupID = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
        switch (mapSetupID) {
        case 1:
            dll_652_func_254(self);
            break;
        case 2:
            dll_652_func_364(self);
            break;
        }
    }
}

// offset: 0x1C4 | func: 2 | export: 2
void dll_652_update(Object *self) { }

// offset: 0x1D0 | func: 3 | export: 3
void dll_652_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1E8 | func: 4 | export: 4
void dll_652_free(Object* self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x230 | func: 5 | export: 5
u32 dll_652_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x240 | func: 6 | export: 6
u32 dll_652_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL652_Data);
}

// offset: 0x254 | func: 7
static void dll_652_func_254(Object* self) {
    DLL652_Data* objdata;
    s16 bit2Val;
    s16 bit1Val;

    objdata = self->data;
    bit2Val = main_get_bits(objdata->unk2);
    bit1Val = main_get_bits(objdata->unk0);
    if ((bit1Val == 0) && (bit2Val != 0) && (objdata->unk4 == 0)) {
        self->unk0xaf &= ~8;
        if ((bit2Val != 0) && (gDLL_1_UI->vtbl->func7(BIT_SpellStone_CRF) != 0)) {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
            objdata->unk4 = 1;
            self->unk0xaf |= 8;
            main_set_bits(objdata->unk0, 1);
        }
    }
}

// offset: 0x364 | func: 8
static void dll_652_func_364(Object* self) {
    DLL652_Data* objdata;
    s16 bit2Val;
    s16 bit1Val;

    objdata = (DLL652_Data*)self->data;
    bit2Val = main_get_bits(objdata->unk2);
    bit1Val = main_get_bits(objdata->unk0);
    if ((bit1Val == 0) && (bit2Val != 0) && (objdata->unk4 == 0)) {
        self->unk0xaf &= ~8;
        if ((bit2Val != 0) && (gDLL_1_UI->vtbl->func7(BIT_SpellStone_DIM) != 0)) {
            gDLL_3_Animation->vtbl->func17(1, self, -1);
            objdata->unk4 = 1;
            self->unk0xaf |= 8;
        }
    }
}

// offset: 0x45C | func: 9
static int dll_652_func_45C(Object* a0, Object* a1, AnimObj_Data* a2, void* a3) {
    return 0;
}
