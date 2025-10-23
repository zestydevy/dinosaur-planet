#include "common.h"
#include "dlls/engine/33.h"
#include "dlls/objects/214_animobj.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/objmsg.h"
#include "sys/objhits.h"
#include "segment_334F0.h"

typedef struct {
    DLL33_Data unk0;
    u8 _unk400[0x574 - 0x400];
} DLL702_Data;

typedef struct {
    DLL33_ObjSetup base;
    u8 _unk36[0x38 - 0x36];
    f32 unk38[4]; // unknown length
    u8 _unk48[0x52 - 0x48];
    u8 unk52[4];
    u8 unk56[4];
} KTrex_ObjSetup;

typedef struct {
    GenericStack *stack;
    u8 _unk4[0x8 - 0x4];
    f32 unk8;
    u8 _unkC[0x10 - 0xC];
    f32 unk10[4];
    f32 unk20[4];
    f32 unk30[4];
    f32 unk40[4];
    f32 unk50[4];
    f32 unk60[4];
    f32 unk70[4];
    f32 unk80[4];
    f32 unk90[4];
    f32 unkA0[4];
    f32 unkB0[4];
    f32 unkC0[4];
    f32 *unkD0;
    f32 *unkD4;
    f32 *unkD8;
    f32 *unkDC;
    f32 *unkE0;
    f32 *unkE4;
    u8 _unkE8[0xEC - 0xE8];
    f32 unkEC;
    u8 _unkF0[0xF4 - 0xF0];
    f32 unkF4;
    u8 _unkF8[0xFA - 0xF8];
    u16 unkFA;
    u8 unkFC;
    u8 unkFD;
    u8 unkFE;
    u8 unkFF;
    u8 unk100;
    u8 unk101;
    u8 unk102;
    u8 unk103;
} KTrex_ActualData;

/*0x0*/ static u32 _data_0[] = {
    0x00000002, 0x00000002
};
/*0x8*/ static u32 _data_8[] = {
    0x00090012, 0x00120000
};
/*0x10*/ static u32 _data_10[] = {
    0x00010002, 0x00030000
};
/*0x18*/ static u32 _data_18[] = {
    0x00040006, 0x00060000
};
/*0x20*/ static u32 _data_20[] = {
    0x0008000e, 0x00100011, 0x00100011
};
/*0x2C*/ static u32 _data_2C[] = {
    0x00010004, 0x00100000
};
/*0x34*/ static u32 _data_34[] = {
    0x00020008, 0x00200000
};
/*0x3C*/ static u32 _data_3C[] = {
    0x40004000, 0x40000000
};
/*0x44*/ static u32 _data_44[] = {
    0x80008000, 0x80000000
};
/*0x4C*/ static u32 _data_4C[] = {
    0x00400080, 0x01000000
};
/*0x54*/ static f32 _data_54[] = {
    0.006, 0.003, 0.003
};
/*0x60*/ static f32 _data_60[] = {
    0.0055, 0.012, 0.012
};
/*0x6C*/ static f32 _data_6C[] = {
    0, 0.025, 0.025
};
/*0x78*/ static f32 _data_78[] = {
    1, 0.975, 0.975
};
/*0x84*/ static u16 _data_84[2] = {0x06fa, 0x06fb};
/*0x88*/ static u16 _data_88[2] = {0x0691, 0x06fd};
/*0x8C*/ static u16 _data_8C[2] = {0x06fe, 0x0000};
/*0x90*/ static u16 _data_90[2] = {0x025b, 0x025c};
/*0x94*/ static u32 _data_94[] = { //uIDs?
    0x00032136, 0x0003213d, 0x00032128, 0x0003212f
};
/*0xA4*/ static u32 _data_A4[] = {
    0x00032130, 0x00032137, 0x00032123, 0x00032129
};
/*0xB4*/ static u32 _data_B4[] = {
    0x00032367, 0x00032369, 0x0003236a, 0x00032365
};
/*0xC4*/ static u32 _data_C4[] = {
    0x00032132, 0x00032139, 0x00032124, 0x0003212b
};
/*0xD4*/ static u16 _data_D4[] = {
    0x0566, 0x0567, 0x0568, 0x0569
};
/*0xDC*/ static u16 _data_DC[] = {
    0x0560, 0x0561, 0x0562, 0x0563
};
/*0xE4*/ static DLL_IModgfx *_data_E4 = NULL;
/*0xE8*/ static u32 _data_E8 = 0x02080104;
/*0xEC*/ static u32 _data_EC[] = {
    0x00000006, 0x00000069, 0x00000069, 0x000000ff, 0x00000000
};

/*0x0*/ static u8 _bss_0[0x28];
/*0x28*/ static u8 _bss_28[0x8];
/*0x30*/ static u8 _bss_30[0x28];
/*0x58*/ static DLL33_Data* _bss_58;
/*0x5C*/ static KTrex_ActualData *_bss_5C;
/*0x60*/ static u8 _bss_60[0x2];
/*0x62*/ static u8 _bss_62[0x2];
/*0x64*/ static u8 _bss_64[0x4];
/*0x68*/ static u8 _bss_68[0x4];
/*0x6C*/ static u8 _bss_6C[0x4];
/*0x70*/ static u8 _bss_70[0x4];
/*0x74*/ static u8 _bss_74[0x4];
/*0x78*/ static u8 _bss_78[0x8];

/*static*/ int dll_702_func_119C(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3);

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_0.s")

// offset: 0x120 | ctor
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_ctor.s")

// offset: 0x160 | dtor
void dll_702_dtor(void *dll) { }

// offset: 0x16C | func: 1 | export: 0
#ifndef NON_MATCHING
void dll_702_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_setup.s")
#else
void dll_702_setup(Object* self, DLL33_ObjSetup* setup, s32 arg2) {
    CurveSetup* curve;
    s32 i;
    s32 var_v0;
    DLL33_Data* objdata;
    KTrex_ActualData* ktdata;

    objdata = (DLL33_Data*)self->data;
    var_v0 = 0x10;
    if (arg2 != 0) {
        var_v0 = 0x11;
    }
    gDLL_33->vtbl->func21(self, setup, objdata, 9, 0xC, 0x100, (u8) var_v0, 20.0f);
    self->unk0xbc = (ObjectCallback)dll_702_func_119C;
    gDLL_18->vtbl->func4(self, &objdata->unk0, 0);
    objdata->unk0.unk268 = 2;
    objdata->unk0.unk2C8 = 0;
    objdata->unk0.unk4.mode = 0;
    objdata->unk0.unk33D = 0;
    self->unk0xaf |= 0x88;
    func_8002674C(self);
    if (self->ptr0x64 != NULL) {
        self->ptr0x64->flags |= 0x810;
    }
    ktdata = objdata->unk3F4;
    ktdata->stack = generic_stack_new(4, 4);

    for (i = 0; i < 4; i++) {
        curve = gDLL_26_Curves->vtbl->curves_func_39c(_data_A4[i]);
        if (curve != NULL) {
            ktdata->unk10[i] = curve->base.x;
            ktdata->unk20[i] = curve->base.y;
            ktdata->unk30[i] = curve->base.z;
            curve = gDLL_26_Curves->vtbl->curves_func_39c(_data_94[i]);
            ktdata->unk40[i] = curve->base.x;
            ktdata->unk50[i] = curve->base.y;
            ktdata->unk60[i] = curve->base.z;
            curve = gDLL_26_Curves->vtbl->curves_func_39c(_data_C4[i]);
            ktdata->unk70[i] = curve->base.x;
            ktdata->unk80[i] = curve->base.y;
            ktdata->unk90[i] = curve->base.z;
            curve = gDLL_26_Curves->vtbl->curves_func_39c(_data_B4[i]);
            ktdata->unkA0[i] = curve->base.x;
            ktdata->unkB0[i] = curve->base.y;
            ktdata->unkC0[i] = curve->base.z;
        }
    }
    
    ktdata->unkD0 = ktdata->unk10;
    ktdata->unkD4 = ktdata->unk20;
    ktdata->unkD8 = ktdata->unk30;
    ktdata->unkDC = ktdata->unk40;
    ktdata->unkE0 = ktdata->unk50;
    ktdata->unkE4 = ktdata->unk60;
    ktdata->unk102 = 4;
    _data_E4 = (DLL_IModgfx*)dll_load_deferred(DLL_ID_106, 1);
}
#endif

// offset: 0x448 | func: 2 | export: 1
#ifndef NON_MATCHING
void dll_702_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_control.s")
#else
/*static*/ void dll_702_func_12DC(Object* a0);
/*static*/ void dll_702_func_1EF0(Object* a0, void* a1);
/*static*/ u8 dll_702_func_B98();
/*static*/ u8 dll_702_func_C2C(u16 arg0);
/*static*/ f32 dll_702_func_C74(Object* a0, KTrex_ActualData* a1);
/*static*/ u8 dll_702_func_D5C(u8 a0);

void dll_702_control(Object* self) {
    DLL33_Data* temp_s1;
    f32 sp38[3];

    if (self->unk0xdc == 0) {
        temp_s1 = (DLL33_Data*)self->data;
        _bss_58 = temp_s1;
        _bss_5C = _bss_58->unk3F4;
        func_80028D2C(self);
        temp_s1->unk0.unk2C8 = get_player();
        if (temp_s1->unk0.unk2C8 != NULL) {
            sp38[0] = temp_s1->unk0.unk2C8->positionMirror.x - self->positionMirror.x;
            sp38[1] = temp_s1->unk0.unk2C8->positionMirror.y - self->positionMirror.y;
            sp38[2] = temp_s1->unk0.unk2C8->positionMirror.z - self->positionMirror.z;
            temp_s1->unk0.unk2B8 = sqrtf(SQ(sp38[0]) + SQ(sp38[1]) + SQ(sp38[2]));
        }
        func_80032A08(self, &_bss_58->unk3BC);
        _bss_5C->unkFF = dll_702_func_B98();
        _bss_5C->unkF4 = dll_702_func_C74(temp_s1->unk0.unk2C8, _bss_5C);
        _bss_5C->unkFE = dll_702_func_C2C(_bss_5C->unkFA);
        _bss_5C->unk100 = dll_702_func_D5C(_bss_5C->unkFE);
        gDLL_33->vtbl->func20(self, &temp_s1->unk0, &_bss_58->unk34C, _bss_58->unk39E, &_bss_58->unk3B4, 2, 2, 0);
        if (_bss_5C->unkFA & 0x10) {
            temp_s1->unk0.unk348 = 2;
        } else {
            temp_s1->unk0.unk348 = 0;
        }
        dll_702_func_1EF0(self, temp_s1);
        dll_702_func_12DC(self);
        gDLL_33->vtbl->func10(self, temp_s1, 0.0f, 0);
        func_80026128(self, 0x17, 1, -1);
        gDLL_18->vtbl->func1(self, &temp_s1->unk0, delayFloat, delayFloat, (s16 (**)(Object*, DLL18_Data*, f32)) _bss_0, (s16 (**)(Object*, DLL18_Data*, f32)) _bss_28);
        self->srt.transl.y = _bss_5C->unkEC;
    }
}
#endif

// offset: 0x72C | func: 3 | export: 2
void dll_702_update(Object *self) { }

// offset: 0x738 | func: 4 | export: 3
void dll_702_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_print.s")

// offset: 0xA78 | func: 5 | export: 4
void dll_702_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_free.s")

// offset: 0xB3C | func: 6 | export: 5
u32 dll_702_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0xB4C | func: 7 | export: 6
u32 dll_702_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL702_Data);
}

// offset: 0xB60 | func: 8 | export: 7
s16 dll_702_func_B60(Object* self) {
    DLL33_Data *dll33data;
    
    dll33data = (_bss_58 = (DLL33_Data*)self->data);
    return dll33data->unk0.unk26C;
}

// offset: 0xB88 | func: 9 | export: 8
void dll_702_func_B88(Object *self, s32 arg1) { 

}

// offset: 0xB98 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_B98.s")

// offset: 0xC2C | func: 11
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_C2C.s")
#else
u8 dll_702_func_C2C(u16 arg0) {
  s8 data_E8_local[4] = {0x02, 0x08, 0x01, 0x04};

  return data_E8_local[(arg0 >> 1) & 3];
}
#endif

// offset: 0xC74 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_C74.s")

// offset: 0xD5C | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_D5C.s")

// offset: 0xE24 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_E24.s")

// offset: 0xE88 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_E88.s")

// offset: 0xEF0 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_EF0.s")

// offset: 0xFE4 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_FE4.s")

// offset: 0x119C | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_119C.s")

// offset: 0x12DC | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_12DC.s")

// offset: 0x1E9C | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_1E9C.s")

// offset: 0x1EF0 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_1EF0.s")

// offset: 0x23EC | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_23EC.s")

// offset: 0x2454 | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2454.s")

// offset: 0x2644 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2644.s")

// offset: 0x28BC | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_28BC.s")

// offset: 0x29D0 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_29D0.s")

// offset: 0x2AF0 | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2AF0.s")

// offset: 0x2BA4 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2BA4.s")

// offset: 0x2C54 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2C54.s")

// offset: 0x2CF8 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2CF8.s")

// offset: 0x2D80 | func: 31
u32 dll_702_func_2D80(Object *self, u32 a1, u32 a2) {
    return 0;
}

// offset: 0x2D98 | func: 32
s16 dll_702_func_2D98(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk273 != 0) {
        gDLL_28_ScreenFade->vtbl->fade(30, SCREEN_FADE_BLACK);
        func_80013FB4();
        arg0->unk0xaf |= 8;
        arg1->unk33D = 0;
        arg1->unk4.mode = 0;
        obj_send_mesg(get_player(), 0xE0000, arg0, NULL);
    }
    return 0;
}

// offset: 0x2E64 | func: 33
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_2E64.s")
#else
/*static*/ void dll_702_func_E24(s32 a0);
/*static*/ s32 dll_702_func_FE4(DLL18_Data* a0, KTrex_ActualData* a1);

s16 dll_702_func_2E64(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    KTrex_ObjSetup* objsetup;
    s32 sp30;
    s32 temp_a3;

    objsetup = (KTrex_ObjSetup*)arg0->setup;
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 1);
        _bss_5C->unkFC = 0;
        _bss_5C->unkFA &= ~0x20;
        arg1->unk28C = objsetup->unk38[_bss_5C->unkFC] / 1000.0f;
    }
    if (dll_702_func_FE4(arg1, _bss_5C) != 0) {
        dll_702_func_E24(2);
        return 4;
    }

    temp_a3 = _bss_5C->unkFA & 1;
    if (_bss_5C->unkFC == 0) {
        if (((s32) _bss_5C->unk101 >= 2) && !(_bss_5C->unkFA & 0x20) && (((temp_a3 == 0) && (_bss_5C->unk8 >= 0.7f)) || ((temp_a3 != 0) && (_bss_5C->unk8 <= 0.3f)))) {
            sp30 = (s32) _bss_5C->unk101 >> 1;
            if (rand_next(0, 0x64) <= (s32) objsetup->unk56[sp30]) {
                _bss_5C->unk103 = 2;
                dll_702_func_E24(5);
                _bss_5C->unkFD = 1;
                return 5;
            }
            if (rand_next(0, 0x64) <= (s32) objsetup->unk52[sp30]) {
                _bss_5C->unkFD = 0;
                dll_702_func_E24(0xB);
                return 5;
            }
            _bss_5C->unkFA |= 0x20;
        }
    }
    if ((_bss_5C->unkFE & _bss_5C->unkFF) && (((temp_a3 == 0) && (_bss_5C->unk8 <= _bss_5C->unkF4)) || ((temp_a3 != 0) && (_bss_5C->unkF4 <= _bss_5C->unk8)))) {
        _bss_5C->unk103 = 1;
        dll_702_func_E24(5);
        _bss_5C->unkFD = 1;
        return 5;
    }
    return 0;
}
#endif

// offset: 0x3160 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3160.s")

// offset: 0x3208 | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3208.s")

// offset: 0x3330 | func: 36
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3330.s")

// offset: 0x3490 | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3490.s")

// offset: 0x3518 | func: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3518.s")

// offset: 0x35A0 | func: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_35A0.s")

// offset: 0x3720 | func: 40
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3720.s")

// offset: 0x3828 | func: 41
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3828.s")

// offset: 0x3AA0 | func: 42
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_3AA0.s")

/*0x0*/ static const char str_0[] = "TREX: state stack overflow\n";
/*0x1C*/ static const char str_1C[] = "State Error!!!\n";
