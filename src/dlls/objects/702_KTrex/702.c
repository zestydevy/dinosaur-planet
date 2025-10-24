#include "common.h"
#include "dlls/engine/33.h"
#include "dlls/objects/214_animobj.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "segment_334F0.h"

typedef struct {
    GenericStack *stack;
    f32 unk4;
    f32 unk8;
    s32 unkC;
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
    f32 unkE8;
    f32 unkEC;
    f32 unkF0;
    f32 unkF4;
    s16 unkF8;
    u16 unkFA;
    u8 unkFC;
    u8 unkFD;
    u8 unkFE;
    u8 unkFF;
    u8 unk100;
    u8 unk101;
    u8 unk102;
    s8 unk103;
    s32 unk104;
    u8 unk108;
    u8 _unk109[0x10c - 0x109];
    SRT unk10C;
    SRT unk124;
    SRT unk13C;
    SRT unk154;
    Vec3f unk16C;
} KTrex_ActualData;

typedef struct {
    DLL33_ObjSetup base;
    f32 unk38[3];
    u16 unk44[3];
    u16 unk4A[4];
    u8 unk52[4];
    u8 unk56[4];
} KTrex_ObjSetup;

/*0x0*/ static u32 _data_0[] = {
    0x00000002, 0x00000002
};
/*0x8*/ static s16 _data_8[] = {
    0x0009, 0x0012, 0x0012, 0x0000
};
/*0x10*/ static s16 _data_10[] = {
    0x0001, 0x0002, 0x0003, 0x0000
};
/*0x18*/ static s16 _data_18[] = {
    0x0004, 0x0006, 0x0006, 0x0000
};
/*0x20*/ static s16 _data_20[3][2] = {
    { 0x0008, 0x000e }, 
    { 0x0010, 0x0011 }, 
    { 0x0010, 0x0011 }
};
/*0x2C*/ static u16 _data_2C[] = {
    0x0001, 0x0004, 0x0010, 0x0000
};
/*0x34*/ static u16 _data_34[] = {
    0x0002, 0x0008, 0x0020, 0x0000
};
/*0x3C*/ static u16 _data_3C[] = {
    0x4000, 0x4000, 0x4000, 0x0000
};
/*0x44*/ static u16 _data_44[] = {
    0x8000, 0x8000, 0x8000, 0x0000
};
/*0x4C*/ static u16 _data_4C[] = {
    0x0040, 0x0080, 0x0100, 0x0000
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
/*0xD4*/ static s16 _data_D4[] = {
    0x0566, 0x0567, 0x0568, 0x0569
};
/*0xDC*/ static s16 _data_DC[] = {
    0x0560, 0x0561, 0x0562, 0x0563
};
/*0xE4*/ static DLL_IModgfx *_data_E4 = NULL;
/*0xE8*/ static u32 _data_E8 = 0x02080104;
/*0xEC*/ static u32 _data_EC[] = {
    0x00000006, 0x00000069, 0x00000069, 0x000000ff, 0x00000000
};

/*0x0*/ static dll18_callback _bss_0[9];
/*0x28*/ static dll18_callback _bss_28[12];
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

static s32 dll_702_func_23EC(Object* arg1, DLL18_Data* arg2, f32 arg3);
static s32 dll_702_func_2454(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2644(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_28BC(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_29D0(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2AF0(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2BA4(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2C54(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2CF8(Object* arg1, DLL18_Data* arg2, f32 arg3);

static u32 dll_702_func_2D80(Object *self, DLL18_Data* arg1, f32 arg2);
static s16 dll_702_func_2D98(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s16 dll_702_func_2E64(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3160(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3208(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3330(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3490(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s16 dll_702_func_3518(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s16 dll_702_func_35A0(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s16 dll_702_func_3720(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s16 dll_702_func_3828(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s16 dll_702_func_3AA0(Object* arg0, DLL18_Data* arg1, f32 arg2);

/*static*/ s32 dll_702_func_B98(void);
/*static*/ u8 dll_702_func_C2C(u16 arg0);
static f32 dll_702_func_C74(Object* arg0, KTrex_ActualData* arg1);
/*static*/ s32 dll_702_func_D5C(u8 arg0);
/*static*/ int dll_702_func_119C(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3);
/*static*/ void dll_702_func_12DC(Object* arg0);

// offset: 0x0 | func: 0
static void dll_702_func_0(void) {
    _bss_0[0] = (dll18_callback)dll_702_func_23EC;
    _bss_0[1] = (dll18_callback)dll_702_func_2454;
    _bss_0[2] = (dll18_callback)dll_702_func_2644;
    _bss_0[3] = (dll18_callback)dll_702_func_28BC;
    _bss_0[4] = (dll18_callback)dll_702_func_29D0;
    _bss_0[5] = (dll18_callback)dll_702_func_2AF0;
    _bss_0[6] = (dll18_callback)dll_702_func_2BA4;
    _bss_0[7] = (dll18_callback)dll_702_func_2C54;
    _bss_0[8] = (dll18_callback)dll_702_func_2CF8;
    
    _bss_28[0] = (dll18_callback)dll_702_func_2D80;
    _bss_28[1] = (dll18_callback)dll_702_func_2D98;
    _bss_28[2] = (dll18_callback)dll_702_func_2E64;
    _bss_28[3] = (dll18_callback)dll_702_func_3160;
    _bss_28[4] = (dll18_callback)dll_702_func_3208;
    _bss_28[5] = (dll18_callback)dll_702_func_3330;
    _bss_28[6] = (dll18_callback)dll_702_func_3490;
    _bss_28[7] = (dll18_callback)dll_702_func_3518;
    _bss_28[8] = (dll18_callback)dll_702_func_35A0;
    _bss_28[9] = (dll18_callback)dll_702_func_3720;
    _bss_28[10] = (dll18_callback)dll_702_func_3828;
    _bss_28[11] = (dll18_callback)dll_702_func_3AA0;
}

// offset: 0x120 | ctor
void dll_702_ctor(void *dll) {
    dll_702_func_0();
}

// offset: 0x160 | dtor
void dll_702_dtor(void *dll) { }

// offset: 0x16C | func: 1 | export: 0
#ifndef NON_MATCHING
void dll_702_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_setup.s")
#else
// missing static refs
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
/*static*/ void dll_702_func_1EF0(Object* a0, void* a1);

// needs static ref
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
        gDLL_18->vtbl->func1(self, &temp_s1->unk0, delayFloat, delayFloat, _bss_0, _bss_28);
        self->srt.transl.y = _bss_5C->unkEC;
    }
}
#endif

// offset: 0x72C | func: 3 | export: 2
void dll_702_update(Object *self) { }

// offset: 0x738 | func: 4 | export: 3
void dll_702_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    MtxF sp48;
    s32 _pad;
 
    _bss_58 = (DLL33_Data* ) self->data;
    if ((visibility != 0) && (self->unk0xdc == 0)) {
        if (_bss_58->unk3E8 != 0.0f) {
            func_80036FBC(0xC8, 0, 0, _bss_58->unk3E8);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 1, &_bss_5C->unk124.transl.x, &_bss_5C->unk124.transl.y, &_bss_5C->unk124.transl.z, 0);
        func_80031F6C(self, 2, &_bss_5C->unk13C.transl.x, &_bss_5C->unk13C.transl.y, &_bss_5C->unk13C.transl.z, 0);
        func_80031F6C(self, 3, &_bss_5C->unk154.transl.x, &_bss_5C->unk154.transl.y, &_bss_5C->unk154.transl.z, 0);
        func_80031F6C(self, 0, &_bss_5C->unk10C.transl.x, &_bss_5C->unk10C.transl.y, &_bss_5C->unk10C.transl.z, 0);
        memcpy(&sp48, func_80032170(self, 4), 0x40);
        sp48.m[3][1] = 0.0f;
        sp48.m[3][0] = 0.0f;
        sp48.m[3][2] = 0.0f;
        _bss_5C->unk16C.x = (f32) ((f32) rand_next(-0x32, 0x32) * 0.1f);
        _bss_5C->unk16C.y = (f32) ((f32) rand_next(0x3C, 0x78) * 0.1f);
        _bss_5C->unk16C.z = (f32) ((f32) rand_next(0x64, 0x96) * -0.25f);
        vec3_transform(&sp48, _bss_5C->unk16C.x, _bss_5C->unk16C.y, _bss_5C->unk16C.z, &_bss_5C->unk16C.x, &_bss_5C->unk16C.y, &_bss_5C->unk16C.z);
        _bss_5C->unk104 |= 0x100000;
    }
}

// offset: 0xA78 | func: 5 | export: 4
void dll_702_free(Object* self, s32 a1) {
    _bss_58 = (DLL33_Data* ) self->data;
    _bss_5C = _bss_58->unk3F4;
    obj_free_object_type(self, 4);
    gDLL_33->vtbl->func15(self, _bss_58, 0);
    generic_stack_free(_bss_5C->stack);
    if (_data_E4 != NULL) {
        dll_unload(_data_E4);
    }
    _data_E4 = NULL;
}

// offset: 0xB3C | func: 6 | export: 5
u32 dll_702_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0xB4C | func: 7 | export: 6
u32 dll_702_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL33_Data) + sizeof(KTrex_ActualData);
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
/*static*/ s32 dll_702_func_B98(void) {
    s32 i;
    u8 var_s2;

    var_s2 = 0;
    for (i = 0;  i < 4; i++) {
        if (main_get_bits(_data_D4[i]) != 0) {
            var_s2 |= (1 << i);
        }
    }
    return var_s2;
}

// offset: 0xC2C | func: 11
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_C2C.s")
#else
/*static*/ u8 dll_702_func_C2C(u16 arg0) {
  s8 data_E8_local[4] = {0x02, 0x08, 0x01, 0x04};

  return data_E8_local[(arg0 >> 1) & 3];
}
#endif

// offset: 0xC74 | func: 12
static f32 dll_702_func_C74(Object* arg0, KTrex_ActualData* arg1) {
    s32 _pad;
    s32 temp_t7;
    f32 sp2C;
    f32 zDiff;
    f32 xDiff;

    temp_t7 = ((s32) arg1->unkFA >> 1) & 3;
    xDiff = arg1->unkDC[temp_t7] - arg1->unkD0[temp_t7];
    zDiff = arg1->unkE4[temp_t7] - arg1->unkD8[temp_t7];
    sp2C = sqrtf(SQ(xDiff) + SQ(zDiff));
    xDiff = arg0->srt.transl.x - arg1->unkD0[temp_t7];
    zDiff = arg0->srt.transl.z - arg1->unkD8[temp_t7];
    return sqrtf(SQ(xDiff) + SQ(zDiff)) / sp2C;
}

// offset: 0xD5C | func: 13
/*static*/ s32 dll_702_func_D5C(u8 arg0) {
    s32 temp_s1;
    s32 i;
    u8 var_s2;

    var_s2 = 0;
    
    for (i = 0; i < 4; i++) {
        temp_s1 = 1 << i;
        if ((arg0 & temp_s1) && (main_get_bits(_data_DC[i]) != 0)) {
            var_s2 |= temp_s1;
        }
    }
    return var_s2;
}

// offset: 0xE24 | func: 14
static void dll_702_func_E24(s32 arg0) {
    if (!generic_stack_is_full(_bss_5C->stack)) {
        generic_stack_push(_bss_5C->stack, &arg0);
    }
}

// offset: 0xE88 | func: 15
static s32 dll_702_func_E88(void) {
    s32 sp24;

    sp24 = 0;
    if (!generic_stack_is_empty(_bss_5C->stack)) {
        generic_stack_pop(_bss_5C->stack, &sp24);
    }
    return sp24;
}

// offset: 0xEF0 | func: 16
static s32 dll_702_func_EF0(Object* arg0) {
    f32 temp_fv1;
    f32 temp_fv1_2;

    if (_bss_5C->unk100 == 0) {
        return 0;
    }
    switch (_bss_5C->unk100) {
    case 1:
    case 2:
        temp_fv1 = arg0->srt.transl.z - 150.0f;
        temp_fv1_2 = arg0->srt.transl.z + 150.0f;
        if ((temp_fv1 > -8960.0f) || (temp_fv1_2 < -8960.0f)) {
            return 0;
        }
        return 1;
    case 4:
    case 8:
        temp_fv1 = arg0->srt.transl.x - 150.0f;
        temp_fv1_2 = arg0->srt.transl.x + 150.0f;
        if ((temp_fv1 > -13470.0f) || (temp_fv1_2 < -13470.0f)) {
            return 0;
        }
        return 1;
    default:
        return 0;
    }
}

// offset: 0xFE4 | func: 17
static s32 dll_702_func_FE4(DLL18_Data* a0, KTrex_ActualData* a1) {
    f32 var_fv0;
    s32 temp_v0;
    s32 var_a3;
    s32 var_v1;

    var_v1 = 0;
    temp_v0 = a1->unkFA & 1;
    var_a3 = ((s32) a1->unkFA >> 1) & 3;
    if (temp_v0 != 0) {
        var_fv0 = -a0->unk28C;
    } else {
        var_fv0 = a0->unk28C;
    }

    a1->unk8 += var_fv0 * delayFloat;

    if ((_data_78[a1->unkFC] < a1->unk8) || (a1->unk8 < _data_6C[a1->unkFC])) {
        var_v1 = 1;
        if (temp_v0 != 0) {
            var_a3 -= 1;
            if (var_a3 < 0) {
                var_a3 = 3;
            }
        } else {
            var_a3 += 1;
            if (var_a3 >= 4) {
                var_a3 = 0;
            }
        }
        a1->unkFA &= ~0x6;
        a1->unkFA |= (var_a3 * 2);
        if (a1->unk8 > _data_78[a1->unkFC]) {
            a1->unk8 = _data_78[a1->unkFC];
        } else if (a1->unk8 < _data_6C[a1->unkFC]) {
            a1->unk8 = _data_6C[a1->unkFC];
        }
        if ((a1 && a1) && a1){} // @fake
    }
    
    a1->unkE8 = a1->unkD0[var_a3] + ((a1->unkDC[var_a3] - a1->unkD0[var_a3]) * a1->unk8);
    a1->unkEC = a1->unkD4[var_a3] + ((a1->unkE0[var_a3] - a1->unkD4[var_a3]) * a1->unk8);
    a1->unkF0 = a1->unkD8[var_a3] + ((a1->unkE4[var_a3] - a1->unkD8[var_a3]) * a1->unk8);
    return var_v1;
}

// offset: 0x119C | func: 18
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_119C.s")
#else
/*static*/ int dll_702_func_119C(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3) {
    s32 i;

    a2->unk62 = 0;

    for (i = 0; i < a2->unk98; i++) {
        switch (a2->unk8E[i]) {
        case 1:
            _bss_5C->unk104 |= 4;
            break;
        case 2:
            _bss_5C->unk104 |= 8;
            break;
        case 3:
            _bss_5C->unk104 |= 0x800;
            break;
        case 4:
            _bss_5C->unk104 |= 0x1000;
            break;
        case 5:
            _bss_5C->unk104 |= 0x20000;
            break;
        default:
            break;
        }
    }
    dll_702_func_12DC(a0);
    return 0;
}
#endif

// offset: 0x12DC | func: 19
/*static*/ void dll_702_func_12DC(Object* arg0) {
    s32 i;
    f32 sp48;

    sp48 = 1.0f - (_bss_58->unk0.unk2B8 / 2000.0f);
    if (sp48 < 0.0f) {
        sp48 = 0.0f;
    } else if (sp48 > 1.0f) {
        sp48 = 1.0f;
    }
    if (_bss_5C->unk104 & 0x40) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x68E, 0x7F, NULL, NULL, 0, NULL);
    }
    if (_bss_5C->unk104 & 0x80) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x68F, 0x7F, NULL, NULL, 0, NULL);
    }
    if (_bss_5C->unk104 & 0x100) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x690, 0x7F, NULL, NULL, 0, NULL);
    }
    if (_bss_5C->unk104 & 0x200) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x68D, 0x7F, NULL, NULL, 0, NULL);
    }
    if (_bss_5C->unk104 & 0x10000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6FC, 0x7F, NULL, NULL, 0, NULL);
    }
    if (_bss_5C->unk104 & 0x40000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_84[rand_next(0, 1)], 0x7F, NULL, NULL, 0, NULL);
    }
    if (_bss_5C->unk104 & 0x80000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, *_data_8C, 0x7F, NULL, NULL, 0, NULL);
    }
    if (_bss_5C->unk104 & 0x2000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_88[rand_next(0, 2)], 0x7F, NULL, NULL, 0, NULL);
    }
    if (_bss_5C->unk104 & 0x1000) {
        _bss_5C->unk104 = _bss_5C->unk104 & ~0x1800;
    }
    if (_bss_5C->unk104 & 0x20000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6F9, 0x7F, NULL, NULL, 0, NULL);
        func_800013BC();
        func_80003B70(2.0f * sp48);
    }
    if (_bss_5C->unk104 & 0x4000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x68B, 0x7F, NULL, NULL, 0, NULL);
        _bss_5C->unk108 ^= 1;
    }
    if (_bss_5C->unk104 & 0x8000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x68C, 0x7F, NULL, NULL, 0, NULL);
        _bss_5C->unk108 ^= 1;
    }
    if (_bss_5C->unk104 & 3) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x688, 0x7F, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(sp48);
            main_set_bits(0x554, 1U);
        }
    }
    if (_bss_5C->unk104 & 0xC) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x689, 0x7F, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(2.0f * sp48);
            main_set_bits(0x554, 1U);
        }
    }
    if (_bss_5C->unk104 & 0x30) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x68A, 0x7F, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(3.0f * sp48);
            main_set_bits(0x554, 1U);
        }
    }
    if (!(_bss_5C->unk104 & 0x100000)) {
        _bss_5C->unk104 = _bss_5C->unk104 & 0x1800;
        return;
    }
    if (_bss_5C->unk104 & 1) {
        _bss_5C->unk124.scale = 1.0f;
        for (i = 0; i != 0xA; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &_bss_5C->unk124, 0x200001, -1, NULL);
        }
    }
    if (_bss_5C->unk104 & 2) {
        _bss_5C->unk13C.scale = 1.0f;
        for (i = 0; i != 0xA; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &_bss_5C->unk13C, 0x200001, -1, NULL);
        }
    }
    if (_bss_5C->unk104 & 4) {
        _bss_5C->unk124.scale = 1.5f;
        for (i = 0; i != 0xD; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &_bss_5C->unk124, 0x200001, -1, NULL);
        }
    }
    if (_bss_5C->unk104 & 8) {
        _bss_5C->unk13C.scale = 1.5f;
        for (i = 0; i != 0xD; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &_bss_5C->unk13C, 0x200001, -1, NULL);
        }
    }
    if (_bss_5C->unk104 & 0x10) {
        _bss_5C->unk124.scale = 2.0f;
        for (i = 0; i != 0x10; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &_bss_5C->unk124, 0x200001, -1, NULL);
        }
    }
    if (_bss_5C->unk104 & 0x20) {
        _bss_5C->unk13C.scale = 2.0f;
        for (i = 0; i != 0x10; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &_bss_5C->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &_bss_5C->unk13C, 0x200001, -1, NULL);
        } 
    }
    if (_bss_5C->unk104 & 0x800) {
        gDLL_17->vtbl->func1(arg0, 0x487, &_bss_5C->unk10C, 0x200001, -1, (void*)&_bss_5C->unk16C);
    }
    _bss_5C->unk104 = _bss_5C->unk104 & 0x1800;
}

// offset: 0x1E9C | func: 20
static void dll_702_func_1E9C(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = 1 << arg0;
    if (_bss_58->unk0.unk308 & temp_v0) {
        _bss_58->unk0.unk308 = _bss_58->unk0.unk308 & ~temp_v0;
        _bss_5C->unk104 |= arg1;
    }
}

// offset: 0x1EF0 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_1EF0.s")

// offset: 0x23EC | func: 22
static s32 dll_702_func_23EC(Object* arg1, DLL18_Data* arg2, f32 arg3) {
    if (arg2->unk272 != 0) {
        func_80023D30(arg1, 0, 0.0f, 0);
    }
    arg2->unk298 = 0.01f;
    return 0;
}

// offset: 0x2454 | func: 23
static s32 dll_702_func_2454(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    f32 temp_fa1;
    f32 temp_fv0;
    s32 var_a1;

    if (arg1->unk272 != 0) {
        func_80023D30(arg0, (s32) _data_10[_bss_5C->unkFC], 0.0f, 0U);
        arg1->unk278 = 0.0f;
        arg1->unk27C = 0.0f;
    }
    dll_702_func_1E9C(2, (s32) _data_2C[_bss_5C->unkFC]);
    dll_702_func_1E9C(1, (s32) _data_34[_bss_5C->unkFC]);
    if (_bss_5C->unk108 != 0) {
        var_a1 = _data_3C[_bss_5C->unkFC];
    } else {
        var_a1 = _data_44[_bss_5C->unkFC];
    }
    dll_702_func_1E9C(0, var_a1);
    temp_fv0 = (_bss_5C->unkE8 - arg0->srt.transl.x) * inverseDelay;
    temp_fa1 = (_bss_5C->unkF0 - arg0->srt.transl.z) * inverseDelay;
    func_8002493C(arg0, sqrtf(SQ(temp_fv0) + SQ(temp_fa1)), &arg1->unk298);
    arg0->srt.transl.x = _bss_5C->unkE8;
    arg0->srt.transl.z = _bss_5C->unkF0;
    return 0;
}

// offset: 0x2644 | func: 24
static s32 dll_702_func_2644(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    MtxF sp60;
    f32 temp_y;
    SRT sp44;
    u16 temp_t7;

    temp_t7 = _bss_5C->unkFA & 1;
    if (arg1->unk272 != 0) {
        func_80023D30(arg0, _data_20[_bss_5C->unkFC & 0xFFFF][temp_t7], 0.0f, 0);
        arg1->unk298 = _data_60[_bss_5C->unkFC];
        _bss_5C->unkF8 = arg0->srt.yaw;
    }
    dll_702_func_1E9C(2, 1);
    dll_702_func_1E9C(1, 2);
    dll_702_func_1E9C(0, 0x40);
    dll_702_func_1E9C(7, 0x10000);
    arg1->unk340 |= 1;
    gDLL_18->vtbl->func7(arg0, arg1, delayFloat, 3);
    sp44.yaw = _bss_5C->unkF8;
    sp44.pitch = 0;
    sp44.roll = 0;
    sp44.transl.x = 0.0f;
    sp44.transl.y = 0.0f;
    sp44.transl.z = 0.0f;
    sp44.scale = 1.0f;
    matrix_from_srt(&sp60, &sp44);
    vec3_transform(&sp60, arg1->unk27C, 0.0f, -arg1->unk278, &arg0->speed.x, &temp_y, &arg0->speed.z);
    if (temp_t7 != 0) {
        arg0->srt.yaw = (s16) (s32) ((f32) _bss_5C->unkF8 + (16384.0f * arg0->animProgress));
    } else {
        arg0->srt.yaw = (s16) (s32) ((f32) _bss_5C->unkF8 - (16384.0f * arg0->animProgress));
    }
    return 0;
}

// offset: 0x28BC | func: 25
static s32 dll_702_func_28BC(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    u16 var_t0;

    var_t0 = _bss_5C->unkFA & 1;
    if (arg1->unk272 != 0) {
        func_80023D30(arg0, 0xF, 0.0f, 0U);
        arg1->unk298 = 0.005f;
        arg1->unk278 = 0.0f;
        arg1->unk27C = 0.0f;
        _bss_5C->unkF8 = arg0->srt.yaw;
    }
    if (var_t0 != 0) {
        arg0->srt.yaw = (s16) (s32) ((f32) _bss_5C->unkF8 + (32768.0f * arg0->animProgress));
    } else {
        arg0->srt.yaw = (s16) (s32) ((f32) _bss_5C->unkF8 - (32768.0f * arg0->animProgress));
    }
    return 0;
}

// offset: 0x29D0 | func: 26
static s32 dll_702_func_29D0(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk272 != 0) {
        func_80023D30(arg0, _data_18[_bss_5C->unkFD], 0.0f, 0);
        arg1->unk298 = _data_54[_bss_5C->unkFD];
        arg1->unk278 = 0.0f;
        arg1->unk27C = 0.0f;
    }
    dll_702_func_1E9C(0, _data_4C[_bss_5C->unkFD]);
    dll_702_func_1E9C(9, 0x800);
    dll_702_func_1E9C(0xA, 0x1000);
    return 0;
}
// offset: 0x2AF0 | func: 27
static s32 dll_702_func_2AF0(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk272 != 0) {
        func_80023D30(arg0, _data_8[_bss_5C->unkFC], 0.0f, 0);
        arg1->unk298 = 0.005f;
        arg1->unk278 = 0.0f;
        arg1->unk27C = 0.0f;
    }
    dll_702_func_1E9C(0, 0x200);
    return 0;
}

// offset: 0x2BA4 | func: 28
static s32 dll_702_func_2BA4(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk272 != 0) {
        func_80023D30(arg0, 0xB, 0.0f, 0);
        arg1->unk298 = 0.006f;
        arg1->unk278 = 0.0f;
        arg1->unk27C = 0.0f;
    }
    dll_702_func_1E9C(0, 0x80000);
    dll_702_func_1E9C(7, 0x20000);
    return 0;
}

// offset: 0x2C54 | func: 29
static s32 dll_702_func_2C54(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk272 != 0) {
        func_80023D30(arg0, 0xC, 0.0f, 0U);
        arg1->unk298 = 0.01f;
    }
    dll_702_func_1E9C(0, 0x2000);
    dll_702_func_1E9C(7, 0x40000);
    return 0;
}

// offset: 0x2CF8 | func: 30
static s32 dll_702_func_2CF8(Object* arg1, DLL18_Data* arg2, f32 arg3) {
    if (arg2->unk272 != 0) {
        func_80023D30(arg1, 0xD, 0.0f, 0);
        arg2->unk298 = 0.0017f;
    }
    dll_702_func_1E9C(0, 0x2000);
    return 0;
}

// offset: 0x2D80 | func: 31
static u32 dll_702_func_2D80(Object *self, DLL18_Data* arg1, f32 arg2) {
    return 0;
}

// offset: 0x2D98 | func: 32
static s16 dll_702_func_2D98(Object* arg0, DLL18_Data* arg1, f32 arg2) {
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
static s16 dll_702_func_2E64(Object* arg0, DLL18_Data* arg1, f32 arg2) {
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

// offset: 0x3160 | func: 34
static s32 dll_702_func_3160(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 2);
    } else if (arg1->unk33A != 0) {
        _bss_5C->unk8 = dll_702_func_C74(arg0, _bss_5C);
        return dll_702_func_E88() + 1;
    }

    return 0;
}

// offset: 0x3208 | func: 35
static s32 dll_702_func_3208(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    KTrex_ObjSetup* objsetup;
    f32 var_ft1;
    u16 temp_t3;

    objsetup = (KTrex_ObjSetup*)arg0->setup;
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 4);
        _bss_5C->unk4 = (f32) objsetup->unk44[_bss_5C->unkFD];
    } else {
        _bss_5C->unk4 -= delayFloat;
        if (_bss_5C->unk4 < 0.0f) {
            _bss_5C->unk4 = 0.0f;
        }
        if ((arg1->unk33A != 0) && (_bss_5C->unk4 <= 0.0f)) {
            return dll_702_func_E88() + 1;
        }
    }
    return 0;
}

// offset: 0x3330 | func: 36
static s32 dll_702_func_3330(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    KTrex_ObjSetup* objsetup;

    objsetup = (KTrex_ObjSetup*)arg0->setup;
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 1);
        _bss_5C->unkFC = 1;
        arg1->unk28C = objsetup->unk38[_bss_5C->unkFC] / 1000.0f;
    }
    if (dll_702_func_FE4(arg1, _bss_5C) != 0) {
        _bss_5C->unk103 -= 1;
    }
    if (_bss_5C->unk103 <= 0) {
        dll_702_func_E24(2);
        dll_702_func_E24(6);
        return 4;
    }
    if (dll_702_func_EF0(arg0) != 0) {
        return 8;
    }
    return 0;
}

// offset: 0x3490 | func: 37
static s32 dll_702_func_3490(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 5);
    } else if (arg1->unk33A != 0) {
        return dll_702_func_E88() + 1;
    }

    return 0;
}

// offset: 0x3518 | func: 38
static s16 dll_702_func_3518(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 6);
        arg0->unk0xaf &= ~0x8;
    } else if (arg1->unk33A != 0) {
        return 9;
    }

    return 0;
}

// offset: 0x35A0 | func: 39
static s16 dll_702_func_35A0(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    KTrex_ObjSetup* objsetup;

    objsetup = (KTrex_ObjSetup*)arg0->setup;
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 7);
        _bss_5C->unk4 = (f32) objsetup->unk4A[(s32) _bss_5C->unk101 >> 1];
        _bss_5C->unkFA |= 0x10;
        _bss_5C->unkFA &= ~8;
        arg0->unk0xaf &= ~8;
    } else {
        if ((_bss_5C->unkFA & 8) || (_bss_5C->unk4 -= delayFloat) <= 0.0f) {
            if (_bss_5C->unkFA & 8) {
                _bss_5C->unk102 -= 1;
            }
            if ((s32) _bss_5C->unk102 <= 0) {
                return 2;
            }
            arg0->unk0xaf |= 8;
            return 0xA;
        }
    }

    return 0;
}

// offset: 0x3720 | func: 40
static s16 dll_702_func_3720(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    u16 temp;
    
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 8);
    } else if (arg1->unk33A != 0) {
        if (_bss_5C->unkFA & 8) {
            _bss_5C->unk101 += 1;
            main_set_bits(0x572, _bss_5C->unk101);
        }
        temp = _bss_5C->unkFA;
        _bss_5C->unkC = (temp >> 1) & 3;
        _bss_5C->unk4 = 300.0f;
        gDLL_2_Camera->vtbl->func8(2, 0);
        return 0xB;
    }
    return 0;
}

// offset: 0x3828 | func: 41
static s16 dll_702_func_3828(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    KTrex_ObjSetup* objsetup;
    s32 sp28;
    s32 temp_t0;

    objsetup = (KTrex_ObjSetup*)arg0->setup;
    sp28 = ((s32) _bss_5C->unkFA >> 1) & 3;
    temp_t0 = _bss_5C->unkFA & 1;
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 1);
        _bss_5C->unkFC = 2;
        arg1->unk28C = objsetup->unk38[_bss_5C->unkFC] / 1000.0f;
    }
    if (dll_702_func_FE4(arg1, _bss_5C) != 0) {
        dll_702_func_E24(0xA);
        return 4;
    }
    _bss_5C->unk4 = (f32) (_bss_5C->unk4 - delayFloat);
    if (_bss_5C->unk4 <= 0.0f) {
        _bss_5C->unk4 = 0.0f;
    }
    if ((_bss_5C->unk4 <= 0.0f) && (sp28 == _bss_5C->unkC) && (((temp_t0 == 0) && (_bss_5C->unk8 >= 0.75f)) || ((temp_t0 != 0) && (_bss_5C->unk8 <= 0.25f)))) {
        if (_bss_5C->unkFA & 8) {
            _bss_5C->unk101 += 1;
            _bss_5C->unkFD = 0;
            dll_702_func_E24(0xB);
            dll_702_func_E24(4);
        } else {
            dll_702_func_E24(2);
        }
        gDLL_2_Camera->vtbl->func8(3, 0);
        main_set_bits(0x572, (u32) _bss_5C->unk101);
        return 7;
    }
    return 0;
}

// offset: 0x3AA0 | func: 42
static s16 dll_702_func_3AA0(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 3);
    } else if (arg1->unk33A != 0) {
        _bss_5C->unkFA ^= 1;
        if (_bss_5C->unkFA & 1) {
            _bss_5C->unkD0 = _bss_5C->unk70;
            _bss_5C->unkD4 = _bss_5C->unk80;
            _bss_5C->unkD8 = _bss_5C->unk90;
            _bss_5C->unkDC = _bss_5C->unkA0;
            _bss_5C->unkE0 = _bss_5C->unkB0;
            _bss_5C->unkE4 = _bss_5C->unkC0;
        } else {
            _bss_5C->unkD0 = _bss_5C->unk10;
            _bss_5C->unkD4 = _bss_5C->unk20;
            _bss_5C->unkD8 = _bss_5C->unk30;
            _bss_5C->unkDC = _bss_5C->unk40;
            _bss_5C->unkE0 = _bss_5C->unk50;
            _bss_5C->unkE4 = _bss_5C->unk60;
        }
        _bss_5C->unk8 = dll_702_func_C74(arg0, _bss_5C);
        return 3;
    }

    return 0;
}

/*0x0*/ static const char str_0[] = "TREX: state stack overflow\n";
/*0x1C*/ static const char str_1C[] = "State Error!!!\n";
