#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "sys/gfx/modgfx.h"
#include "sys/map.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 _unk5;
} DLL629_Data;

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} DLL629_Setup;

static void dll_629_func_23C(Object* self);
static void dll_629_func_3D4(Object* self);

// offset: 0x0 | ctor
void dll_629_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_629_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_629_setup(Object* self, DLL629_Setup* setup, s32 reset) {
    DLL629_Data* temp_v1;

    temp_v1 = self->data;
    self->srt.yaw = setup->unk18 << 8;
    self->srt.roll = setup->unk19 << 8;
    self->srt.pitch = setup->unk1C;
    temp_v1->unk0 = (s16) setup->unk1E;
    temp_v1->unk2 = (s16) setup->unk1A;
    if (mainGetBits((s32) temp_v1->unk0) != 0) {
        temp_v1->unk4 = 1U;
    }
    if ((self->id == 0x3E7) && (temp_v1->unk4 != 0)) {
        self->modelInstIdx = 1;
    }
    self->stateFlags |= 0x2000;
}

// offset: 0xDC | func: 1 | export: 1

void dll_629_control(Object* self) {
    if (self->id != 0x3E7) {
        dll_629_func_23C(self);
    } else {
        dll_629_func_3D4(self);
    }
}

// offset: 0x140 | func: 2 | export: 2
void dll_629_update(Object* self) { }

// offset: 0x14C | func: 3 | export: 3
void dll_629_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL629_Data* objdata = self->data;

    if (self->id != 0x3E7) {
        if (visibility != 0) {
            if (objdata->unk4 != 0) {
                return;
            }
            goto block_5;
        }
    } else if (visibility != 0) {
block_5:
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1D0 | func: 4 | export: 4
void dll_629_free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x218 | func: 5 | export: 5
u32 dll_629_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x228 | func: 6 | export: 6
u32 dll_629_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL629_Data);
}

// offset: 0x23C | func: 7
static void dll_629_func_23C(Object* self) {
    static DLL_IModgfx* data_0 = NULL;
    
    DLL629_Data* objdata;
    int i;
    DLL_IModgfx* temp_v0;

    objdata = self->data;
    self->srt.roll += objdata->unk2 * (s16) gUpdateRateF;
    if (objdata->unk4 != 0 || mainGetBits(objdata->unk0) == 0) {
        return;
    } 
    gDLL_6_AMSFX->vtbl->Play(self, 0xB55U, MAX_VOLUME, NULL, NULL, 0, NULL);
    i =  0x28;
    // FAKE
    do {} while (0);
    while (i--) {
        gDLL_17_partfx->vtbl->spawn(self, 0x551, NULL, 2, -1, NULL);
    }
    data_0 = dllLoad(DLL_ID_107, 1U);
    data_0->vtbl->func0(self, 0x11, NULL, 2, -1, NULL);
    dllFree(data_0);

    objdata->unk4 = 1U;
}

// offset: 0x3D4 | func: 8
static void dll_629_func_3D4(Object* self) {
    DLL629_Data* sp34;

    sp34 = self->data;
    if (sp34->unk4 == 0) {
        if (mainGetBits((s32) sp34->unk0) != 0) {
            gDLL_6_AMSFX->vtbl->Play(self, 0xB55U, 0x7FU, NULL, NULL, 0, NULL);
            objSetModel(self, 1);
            sp34->unk4 = 1;
        }
    }
}
