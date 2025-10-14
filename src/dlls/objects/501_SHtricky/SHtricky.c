#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/objects/common/sidekick.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "dll.h"
#include "types.h"

typedef struct {
    u8 unk0;
} SHtricky_Data;

// offset: 0x0 | ctor
void SHtricky_ctor(void *dll) { }

// offset: 0xC | dtor
void SHtricky_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHtricky_setup(Object* self, ObjSetup* setup, s32 arg2) {
    SHtricky_Data *objdata;

    objdata = (SHtricky_Data*)self->data;
    
    if (main_get_bits(BIT_SH_Queen_EW_Quest_Progress) != 0) {
        objdata->unk0 = 3;
    } else {
        main_set_bits(BIT_Play_Seq_00D1, 0);
        objdata->unk0 = 0;
    }

    self->unk0xb0 |= 0x2000;
}

// offset: 0xA8 | func: 1 | export: 1
void SHtricky_control(Object *self) {
    SHtricky_Data *objdata;
    Object *sidekick;

    objdata = (SHtricky_Data*)self->data;
    sidekick = get_sidekick();

    switch (objdata->unk0) {
    case 0:
        if (main_get_bits(BIT_Play_Seq_00D1) != 0) {
            gDLL_3_Animation->vtbl->func17(3, sidekick, -1);
            objdata->unk0 = 1;
        } 
        break;
    case 1:
        objdata->unk0 = 2;
        break;
    case 2:
        ((DLL_ISidekick*)sidekick->dll)->vtbl->func22(sidekick, self);
        objdata->unk0 = 3;
        break;
    case 3:
        break;
    }
}

// offset: 0x1A8 | func: 2 | export: 2)
void SHtricky_update(Object *self) { }

// offset: 0x1B4 | func: 3 | export: 3
void SHtricky_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x1CC | func: 4 | export: 4
void SHtricky_free(Object *self, s32 a1) { }

// offset: 0x1DC | func: 5 | export: 5
u32 SHtricky_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1EC | func: 6 | export: 6
u32 SHtricky_get_data_size(Object *self, u32 a1) {
    return sizeof(SHtricky_Data);
}
