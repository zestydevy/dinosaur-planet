#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "functions.h"
#include "dll.h"
#include "sys/main.h"
#include "game/gamebits.h"

/*0x0*/ static u32 _data_0[] = { 0x00000002, 0x00000003, 0x00000000, 0x00000000 };

/*0x0*/ static s32 _bss_0;

typedef struct {
/*00*/ f32 unk0;
} KT_RexLevel_Data;

// offset: 0x0 | ctor
void KT_RexLevel_ctor(void *dll) { }

// offset: 0xC | dtor
void KT_RexLevel_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void KT_RexLevel_setup(Object *self, ObjSetup *setup, s32 arg2) {
    KT_RexLevel_Data *objdata;

    objdata = self->data;
    func_80000860(self, self, 0x18E, 0);
    func_80000860(self, self, 0x18F, 0);
    func_80000450(self, self, 0x1FD, 0, 0, 0);
    func_80000450(self, self, 0x1FE, 0, 0, 0);
    gDLL_5_AMSEQ2->vtbl->func0(self, 0xD5, 0, 0, 0);
    main_set_bits(BIT_572_KT_FightProgress, 0);
    main_set_bits(BIT_56E, 1);
    main_set_bits(BIT_KT_Player_In_Segment_2, 1);
    main_set_bits(BIT_KT_Player_In_Segment_1, 1);
    objdata->unk0 = 600.0f;
    main_set_bits(BIT_55A, 1);
    main_set_bits(BIT_54A, 2);
    main_set_bits(BIT_54E, 2);
    main_set_bits(BIT_552, 1);
    main_set_bits(BIT_556, 1);
    self->unkDC = 0;
}

// offset: 0x1FC | func: 1 | export: 1
void KT_RexLevel_control(Object *self) {
    s32 temp_v0;

    if (self->unkDC == 0) {
        main_set_bits(BIT_55E, 1);
        self->unkDC = 1;
    }
    temp_v0 = main_get_bits(BIT_572_KT_FightProgress);
    if (_bss_0 != (temp_v0 ^ 0)) {
        if (temp_v0 & 1) {
            main_set_bits(BIT_54A, 0);
            main_set_bits(BIT_54E, 0);
            main_set_bits(BIT_552, 0);
            main_set_bits(BIT_556, 0);
        } else {
            main_set_bits(BIT_55C, 1);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_699, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    }
    if (main_get_bits(BIT_55C)) {
        if (main_get_bits(BIT_55A)) {
            main_set_bits(BIT_54A, 2);
            main_set_bits(BIT_54E, 2);
            main_set_bits(BIT_552, 1);
            main_set_bits(BIT_556, 1);
        } else if (main_get_bits(BIT_55B)) {
            main_set_bits(BIT_54A, 1);
            main_set_bits(BIT_54E, 1);
            main_set_bits(BIT_552, 2);
            main_set_bits(BIT_556, 2);
        }
        main_set_bits(BIT_55C, 0);
    }
    _bss_0 = temp_v0;
}

// offset: 0x470 | func: 2 | export: 2
void KT_RexLevel_update(Object *self) { }

// offset: 0x47C | func: 3 | export: 3
void KT_RexLevel_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4D0 | func: 4 | export: 4
void KT_RexLevel_free(Object *self, s32 arg1) {
    gDLL_5_AMSEQ2->vtbl->func0(self, 0xD6, 0, 0, 0);
    gDLL_5_AMSEQ2->vtbl->func1(self, 0xD5, 0, 0, 0);
}

// offset: 0x560 | func: 5 | export: 5
u32 KT_RexLevel_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x570 | func: 6 | export: 6
u32 KT_RexLevel_get_data_size(Object *self, u32 arg1) {
    return sizeof(KT_RexLevel_Data);
}
