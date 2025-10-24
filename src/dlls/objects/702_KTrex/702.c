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
#include "prevent_bss_reordering.h"

enum {
    KTSTATE_0 = 0,
};

// Note: This is not the only object data. DLL 33 data is prepended to this in memory.
typedef struct {
/*000*/ GenericStack *stateStack;
/*004*/ f32 unk4;
/*008*/ f32 unk8; // how far along a straight (0-1 progress to next corner)
/*00C*/ s32 unkC;
/*010*/ f32 unk10[4];
/*020*/ f32 unk20[4];
/*030*/ f32 unk30[4];
/*040*/ f32 unk40[4];
/*050*/ f32 unk50[4];
/*060*/ f32 unk60[4];
/*070*/ f32 unk70[4];
/*080*/ f32 unk80[4];
/*090*/ f32 unk90[4];
/*0A0*/ f32 unkA0[4];
/*0B0*/ f32 unkB0[4];
/*0C0*/ f32 unkC0[4];
/*0D0*/ f32 *unkD0;
/*0D4*/ f32 *unkD4;
/*0D8*/ f32 *unkD8;
/*0DC*/ f32 *unkDC;
/*0E0*/ f32 *unkE0;
/*0E4*/ f32 *unkE4;
/*0E8*/ f32 unkE8;
/*0EC*/ f32 unkEC;
/*0F0*/ f32 unkF0;
/*0F4*/ f32 unkF4;
/*0F8*/ s16 unkF8;
/*0FA*/ u16 unkFA;
/*0FC*/ u8 unkFC;
/*0FD*/ u8 unkFD;
/*0FE*/ u8 unkFE;
/*0FF*/ u8 unkFF;
/*100*/ u8 unk100;
/*101*/ u8 unk101;
/*102*/ u8 unk102;
/*103*/ s8 unk103;
/*104*/ s32 unk104;
/*108*/ u8 unk108;
/*10C*/ SRT unk10C;
/*124*/ SRT unk124;
/*13C*/ SRT unk13C;
/*154*/ SRT unk154;
/*16C*/ Vec3f unk16C;
} KTrex_Data;

typedef struct {
/*00*/ DLL33_ObjSetup base;
/*38*/ f32 unk38[3];
/*44*/ u16 unk44[3];
/*4A*/ u16 unk4A[4];
/*52*/ u8 unk52[4];
/*56*/ u8 unk56[4];
} KTrex_ObjSetup;

/*0x0*/ static u32 _data_0[] = {
    0x00000002, 0x00000002
};
/*0x8*/ static s16 _data_8[] = { // mod anim indices
    0x0009, 0x0012, 0x0012, 0x0000
};
/*0x10*/ static s16 _data_10[] = { // mod anim indices
    0x0001, 0x0002, 0x0003, 0x0000
};
/*0x18*/ static s16 _data_18[] = { // mod anim indices
    0x0004, 0x0006, 0x0006, 0x0000
};
/*0x20*/ static s16 _data_20[3][2] = { // mod anim indices
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
/*0x84*/ static u16 _data_84[2] = {SOUND_6FA, SOUND_6FB};
/*0x88*/ static u16 _data_88[2] = {SOUND_691_KT_Rex_Roar, SOUND_6FD_KT_Rex_Roar};
/*0x8C*/ static u16 _data_8C[2] = {SOUND_6FE_KT_Rex_Roar, 0};
/*0x90*/ static u16 _data_90[2] = {SOUND_25B_Magic_Attack_Deflected, SOUND_25C_Melee_Attack_Deflected};
/*0x94*/ static u32 _data_94[] = { // curve uIDs
    0x32136, 0x3213d, 0x32128, 0x3212f
};
/*0xA4*/ static u32 _data_A4[] = { // curve uIDs
    0x32130, 0x32137, 0x32123, 0x32129
};
/*0xB4*/ static u32 _data_B4[] = { // curve uIDs
    0x32367, 0x32369, 0x3236a, 0x32365
};
/*0xC4*/ static u32 _data_C4[] = { // curve uIDs
    0x32132, 0x32139, 0x32124, 0x3212b
};
/*0xD4*/ static s16 _data_D4[] = { // game bits
    BIT_566, BIT_567, BIT_568, BIT_569
};
/*0xDC*/ static s16 _data_DC[] = { // game bits
    BIT_560, BIT_561, BIT_562, BIT_563
};
/*0xE4*/ static DLL_IModgfx *_data_E4 = NULL;

/*0x0*/ static dll18_callback _bss_0[9];
/*0x28*/ static dll18_callback _bss_28[12];
/*0x58*/ static DLL33_Data* sDLL33Data;
/*0x5C*/ static KTrex_Data *sKTData;

static s32 dll_702_func_23EC(Object* arg1, DLL18_Data* arg2, f32 arg3);
static s32 dll_702_func_2454(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2644(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_28BC(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_29D0(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2AF0(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2BA4(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2C54(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2CF8(Object* arg1, DLL18_Data* arg2, f32 arg3);

static s32 dll_702_func_2D80(Object *self, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2D98(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_2E64(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3160(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3208(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3330(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3490(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3518(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_35A0(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3720(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3828(Object* arg0, DLL18_Data* arg1, f32 arg2);
static s32 dll_702_func_3AA0(Object* arg0, DLL18_Data* arg1, f32 arg2);

static s32 dll_702_func_B98(void);
static u8 dll_702_func_C2C(u16 arg0);
static f32 dll_702_func_C74(Object* arg0, KTrex_Data* arg1);
static s32 dll_702_func_D5C(u8 arg0);
/*static*/ int dll_702_func_119C(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3);
static void dll_702_func_12DC(Object* arg0);
static void dll_702_func_1EF0(Object* arg0, DLL33_Data* arg1);

// offset: 0x0 | func: 0
static void dll_702_func_0(void) {
    _bss_0[0] = dll_702_func_23EC;
    _bss_0[1] = dll_702_func_2454;
    _bss_0[2] = dll_702_func_2644;
    _bss_0[3] = dll_702_func_28BC;
    _bss_0[4] = dll_702_func_29D0;
    _bss_0[5] = dll_702_func_2AF0;
    _bss_0[6] = dll_702_func_2BA4;
    _bss_0[7] = dll_702_func_2C54;
    _bss_0[8] = dll_702_func_2CF8;
    
    _bss_28[0] = dll_702_func_2D80;
    _bss_28[1] = dll_702_func_2D98;
    _bss_28[2] = dll_702_func_2E64;
    _bss_28[3] = dll_702_func_3160;
    _bss_28[4] = dll_702_func_3208;
    _bss_28[5] = dll_702_func_3330;
    _bss_28[6] = dll_702_func_3490;
    _bss_28[7] = dll_702_func_3518;
    _bss_28[8] = dll_702_func_35A0;
    _bss_28[9] = dll_702_func_3720;
    _bss_28[10] = dll_702_func_3828;
    _bss_28[11] = dll_702_func_3AA0;
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
    KTrex_Data* ktdata;

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
    ktdata->stateStack = generic_stack_new(4, 4);

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
void dll_702_control(Object* self) {
    DLL33_Data* temp_s1;
    f32 sp38[3];

    if (self->unk0xdc == 0) {
        temp_s1 = (DLL33_Data*)self->data;
        sDLL33Data = temp_s1;
        sKTData = sDLL33Data->unk3F4;
        func_80028D2C(self);
        temp_s1->unk0.unk2C8 = get_player();
        if (temp_s1->unk0.unk2C8 != NULL) {
            sp38[0] = temp_s1->unk0.unk2C8->positionMirror.x - self->positionMirror.x;
            sp38[1] = temp_s1->unk0.unk2C8->positionMirror.y - self->positionMirror.y;
            sp38[2] = temp_s1->unk0.unk2C8->positionMirror.z - self->positionMirror.z;
            temp_s1->unk0.unk2B8 = sqrtf(SQ(sp38[0]) + SQ(sp38[1]) + SQ(sp38[2]));
        }
        func_80032A08(self, &sDLL33Data->unk3BC);
        sKTData->unkFF = dll_702_func_B98();
        sKTData->unkF4 = dll_702_func_C74(temp_s1->unk0.unk2C8, sKTData);
        sKTData->unkFE = dll_702_func_C2C(sKTData->unkFA);
        sKTData->unk100 = dll_702_func_D5C(sKTData->unkFE);
        gDLL_33->vtbl->func20(self, &temp_s1->unk0, &sDLL33Data->unk34C, sDLL33Data->unk39E, &sDLL33Data->unk3B4, 2, 2, 0);
        if (sKTData->unkFA & 0x10) {
            temp_s1->unk0.unk348 = 2;
        } else {
            temp_s1->unk0.unk348 = 0;
        }
        dll_702_func_1EF0(self, temp_s1);
        dll_702_func_12DC(self);
        gDLL_33->vtbl->func10(self, temp_s1, 0.0f, 0);
        func_80026128(self, 0x17, 1, -1);
        gDLL_18->vtbl->func1(self, &temp_s1->unk0, delayFloat, delayFloat, _bss_0, _bss_28);
        self->srt.transl.y = sKTData->unkEC;
    }
}

// offset: 0x72C | func: 3 | export: 2
void dll_702_update(Object *self) { }

// offset: 0x738 | func: 4 | export: 3
void dll_702_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    MtxF sp48;
    s32 _pad;
 
    sDLL33Data = (DLL33_Data* ) self->data;
    if ((visibility != 0) && (self->unk0xdc == 0)) {
        if (sDLL33Data->unk3E8 != 0.0f) {
            func_80036FBC(0xC8, 0, 0, sDLL33Data->unk3E8);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 1, &sKTData->unk124.transl.x, &sKTData->unk124.transl.y, &sKTData->unk124.transl.z, 0);
        func_80031F6C(self, 2, &sKTData->unk13C.transl.x, &sKTData->unk13C.transl.y, &sKTData->unk13C.transl.z, 0);
        func_80031F6C(self, 3, &sKTData->unk154.transl.x, &sKTData->unk154.transl.y, &sKTData->unk154.transl.z, 0);
        func_80031F6C(self, 0, &sKTData->unk10C.transl.x, &sKTData->unk10C.transl.y, &sKTData->unk10C.transl.z, 0);
        memcpy(&sp48, func_80032170(self, 4), 0x40);
        sp48.m[3][1] = 0.0f;
        sp48.m[3][0] = 0.0f;
        sp48.m[3][2] = 0.0f;
        sKTData->unk16C.x = (f32) ((f32) rand_next(-0x32, 0x32) * 0.1f);
        sKTData->unk16C.y = (f32) ((f32) rand_next(0x3C, 0x78) * 0.1f);
        sKTData->unk16C.z = (f32) ((f32) rand_next(0x64, 0x96) * -0.25f);
        vec3_transform(&sp48, sKTData->unk16C.x, sKTData->unk16C.y, sKTData->unk16C.z, &sKTData->unk16C.x, &sKTData->unk16C.y, &sKTData->unk16C.z);
        sKTData->unk104 |= 0x100000;
    }
}

// offset: 0xA78 | func: 5 | export: 4
void dll_702_free(Object* self, s32 a1) {
    sDLL33Data = (DLL33_Data* ) self->data;
    sKTData = sDLL33Data->unk3F4;
    obj_free_object_type(self, 4);
    gDLL_33->vtbl->func15(self, sDLL33Data, 0);
    generic_stack_free(sKTData->stateStack);
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
    return sizeof(DLL33_Data) + sizeof(KTrex_Data);
}

// offset: 0xB60 | func: 8 | export: 7
s16 dll_702_func_B60(Object* self) {
    DLL33_Data *dll33data;
    
    dll33data = (sDLL33Data = (DLL33_Data*)self->data);
    return dll33data->unk0.unk26C;
}

// offset: 0xB88 | func: 9 | export: 8
void dll_702_func_B88(Object *self, s32 arg1) { 

}

// offset: 0xB98 | func: 10
static s32 dll_702_func_B98(void) {
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
static u8 dll_702_func_C2C(u16 arg0) {
  s8 data_E8_local[4] = {0x02, 0x08, 0x01, 0x04};

  return data_E8_local[(arg0 >> 1) & 3];
}

// offset: 0xC74 | func: 12
static f32 dll_702_func_C74(Object* arg0, KTrex_Data* arg1) {
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
static s32 dll_702_func_D5C(u8 arg0) {
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
static void dll_702_push_state(s32 state) {
    if (!generic_stack_is_full(sKTData->stateStack)) {
        generic_stack_push(sKTData->stateStack, &state);
    }
}

// offset: 0xE88 | func: 15
static s32 dll_702_pop_state(void) {
    s32 state;

    state = 0;
    if (!generic_stack_is_empty(sKTData->stateStack)) {
        generic_stack_pop(sKTData->stateStack, &state);
    }
    return state;
}

// offset: 0xEF0 | func: 16
static s32 dll_702_func_EF0(Object* arg0) {
    f32 temp_fv1;
    f32 temp_fv1_2;

    if (sKTData->unk100 == 0) {
        return 0;
    }
    switch (sKTData->unk100) {
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
static s32 dll_702_func_FE4(DLL18_Data* a0, KTrex_Data* a1) {
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
        a1->unkFA |= (var_a3 << 1);
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
            sKTData->unk104 |= 4;
            break;
        case 2:
            sKTData->unk104 |= 8;
            break;
        case 3:
            sKTData->unk104 |= 0x800;
            break;
        case 4:
            sKTData->unk104 |= 0x1000;
            break;
        case 5:
            sKTData->unk104 |= 0x20000;
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
static void dll_702_func_12DC(Object* arg0) {
    s32 i;
    f32 sp48;

    sp48 = 1.0f - (sDLL33Data->unk0.unk2B8 / 2000.0f);
    if (sp48 < 0.0f) {
        sp48 = 0.0f;
    } else if (sp48 > 1.0f) {
        sp48 = 1.0f;
    }
    if (sKTData->unk104 & 0x40) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68E_KT_Rex_Noise, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->unk104 & 0x80) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68F_KT_Rex_Roar, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->unk104 & 0x100) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_690_KT_Rex_Roar, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->unk104 & 0x200) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68D_KT_Rex_Roar_Kinda, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->unk104 & 0x10000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_6FC_KT_Rex_Slam, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->unk104 & 0x40000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_84[rand_next(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->unk104 & 0x80000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, *_data_8C, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->unk104 & 0x2000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_88[rand_next(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->unk104 & 0x1000) {
        sKTData->unk104 = sKTData->unk104 & ~0x1800;
    }
    if (sKTData->unk104 & 0x20000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_6F9_Explosion, MAX_VOLUME, NULL, NULL, 0, NULL);
        func_800013BC();
        func_80003B70(2.0f * sp48);
    }
    if (sKTData->unk104 & 0x4000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68B_KT_Rex_Groan, MAX_VOLUME, NULL, NULL, 0, NULL);
        sKTData->unk108 ^= 1;
    }
    if (sKTData->unk104 & 0x8000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68C_KT_Rex_Groan, MAX_VOLUME, NULL, NULL, 0, NULL);
        sKTData->unk108 ^= 1;
    }
    if (sKTData->unk104 & 3) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_688_KT_Rex_Stomp, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(sp48);
            main_set_bits(BIT_554, 1);
        }
    }
    if (sKTData->unk104 & 0xC) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_689_KT_Rex_Stomp, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(2.0f * sp48);
            main_set_bits(BIT_554, 1);
        }
    }
    if (sKTData->unk104 & 0x30) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68A_KT_Rex_Stomp, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(3.0f * sp48);
            main_set_bits(BIT_554, 1);
        }
    }
    if (!(sKTData->unk104 & 0x100000)) {
        sKTData->unk104 = sKTData->unk104 & 0x1800;
        return;
    }
    if (sKTData->unk104 & 1) {
        sKTData->unk124.scale = 1.0f;
        for (i = 0; i != 0xA; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk124, 0x200001, -1, NULL);
        }
    }
    if (sKTData->unk104 & 2) {
        sKTData->unk13C.scale = 1.0f;
        for (i = 0; i != 0xA; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk13C, 0x200001, -1, NULL);
        }
    }
    if (sKTData->unk104 & 4) {
        sKTData->unk124.scale = 1.5f;
        for (i = 0; i != 0xD; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk124, 0x200001, -1, NULL);
        }
    }
    if (sKTData->unk104 & 8) {
        sKTData->unk13C.scale = 1.5f;
        for (i = 0; i != 0xD; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk13C, 0x200001, -1, NULL);
        }
    }
    if (sKTData->unk104 & 0x10) {
        sKTData->unk124.scale = 2.0f;
        for (i = 0; i != 0x10; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk124, 0x200001, -1, NULL);
        }
    }
    if (sKTData->unk104 & 0x20) {
        sKTData->unk13C.scale = 2.0f;
        for (i = 0; i != 0x10; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk13C, 0x200001, -1, NULL);
        } 
    }
    if (sKTData->unk104 & 0x800) {
        gDLL_17->vtbl->func1(arg0, 0x487, &sKTData->unk10C, 0x200001, -1, (void*)&sKTData->unk16C);
    }
    sKTData->unk104 = sKTData->unk104 & 0x1800;
}

// offset: 0x1E9C | func: 20
static void dll_702_func_1E9C(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = 1 << arg0;
    if (sDLL33Data->unk0.unk308 & temp_v0) {
        sDLL33Data->unk0.unk308 = sDLL33Data->unk0.unk308 & ~temp_v0;
        sKTData->unk104 |= arg1;
    }
}

// offset: 0x1EF0 | func: 21
static void dll_702_func_1EF0(Object* arg0, DLL33_Data* arg1) {
    static SRT _bss_60;
    DLL33_Data* temp_v0 = (DLL33_Data*)arg0->data;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    Object* sp54;
    MtxF* temp_v1;
    ModelInstance* modelInst;
    u32 sp3C[] = {0x00000006, 0x00000069, 0x00000069, 0x000000ff};

    if (temp_v0->unk3E8 > 0.0f) {
        temp_v0->unk3E8 += (delayFloat * temp_v0->unk3EC);
        if (temp_v0->unk3E8 < 0.0f) {
            temp_v0->unk3E8 = 0.0f;
        } else if (temp_v0->unk3E8 > 120.0f) {
            temp_v0->unk3E8 = (f32) (120.0f - (temp_v0->unk3E8 - 120.0f));
            temp_v0->unk3EC = (f32) -temp_v0->unk3EC;
        }
    }
    sp60 = func_80025F40(arg0, &sp54, &sp5C, &sp58);
    if (sp60 != 0) {
        modelInst = arg0->modelInsts[arg0->modelInstIdx];
        temp_v1 = modelInst->unk_0x24;
        if ((arg1->unk0.unk348 != 0) && ((sp5C == 3) || (sp5C == 2))) {
            _bss_60.transl.x = temp_v1->m[sp5C][1] + gWorldX;
            _bss_60.transl.y = temp_v1->m[sp5C][2];
            _bss_60.transl.z = temp_v1->m[sp5C][3] + gWorldZ;
            gDLL_6_AMSFX->vtbl->play_sound(arg0, *_data_8C, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_693_Explosion, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_2_Camera->vtbl->func8(2, 0);
            gDLL_17->vtbl->func1(arg0, 0x4B2, &_bss_60, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x4B3, &_bss_60, 0x200001, -1, NULL);
            sKTData->unkFA &= ~0x10;
            sKTData->unkFA |= 8;
            arg1->unk0.unk343 = (s8) sp60;
            arg1->unk0.unk348 -= 1;
        } else {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_90[rand_next(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
            modelInst = arg0->modelInsts[arg0->modelInstIdx];
            temp_v1 = modelInst->unk_0x24;
            _bss_60.transl.x = temp_v1->m[sp5C][1] + gWorldX;
            _bss_60.transl.y = temp_v1->m[sp5C][2];
            _bss_60.transl.z = temp_v1->m[sp5C][3] + gWorldZ;
            gDLL_17->vtbl->func1(arg0, 0x328, &_bss_60, 0x200001, -1, NULL);
            _bss_60.transl.x -= arg0->positionMirror.x;
            _bss_60.transl.y -= arg0->positionMirror.y;
            _bss_60.transl.z -= arg0->positionMirror.z;
            _bss_60.scale = 1.0f;
            _bss_60.yaw = 0;
            _bss_60.pitch = 0;
            _bss_60.roll = 0;
            sp3C[1] += rand_next(0, 0x9B);
            sp3C[2] += rand_next(0, 0x9B);
            _data_E4->vtbl->func0(arg0, 0, &_bss_60, 1, -1, &sp3C);
        }
        if (arg1->unk0.unk348 <= 0) {
            arg1->unk0.unk348 = 0;
        }
        obj_send_mesg(sp54, 0xE0001, arg0, NULL);
    }
}

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
        func_80023D30(arg0, (s32) _data_10[sKTData->unkFC], 0.0f, 0U);
        arg1->unk278 = 0.0f;
        arg1->unk27C = 0.0f;
    }
    dll_702_func_1E9C(2, (s32) _data_2C[sKTData->unkFC]);
    dll_702_func_1E9C(1, (s32) _data_34[sKTData->unkFC]);
    if (sKTData->unk108 != 0) {
        var_a1 = _data_3C[sKTData->unkFC];
    } else {
        var_a1 = _data_44[sKTData->unkFC];
    }
    dll_702_func_1E9C(0, var_a1);
    temp_fv0 = (sKTData->unkE8 - arg0->srt.transl.x) * inverseDelay;
    temp_fa1 = (sKTData->unkF0 - arg0->srt.transl.z) * inverseDelay;
    func_8002493C(arg0, sqrtf(SQ(temp_fv0) + SQ(temp_fa1)), &arg1->unk298);
    arg0->srt.transl.x = sKTData->unkE8;
    arg0->srt.transl.z = sKTData->unkF0;
    return 0;
}

// offset: 0x2644 | func: 24
static s32 dll_702_func_2644(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    MtxF sp60;
    f32 temp_y;
    SRT sp44;
    u16 temp_t7;

    temp_t7 = sKTData->unkFA & 1;
    if (arg1->unk272 != 0) {
        func_80023D30(arg0, _data_20[sKTData->unkFC & 0xFFFF][temp_t7], 0.0f, 0);
        arg1->unk298 = _data_60[sKTData->unkFC];
        sKTData->unkF8 = arg0->srt.yaw;
    }
    dll_702_func_1E9C(2, 1);
    dll_702_func_1E9C(1, 2);
    dll_702_func_1E9C(0, 0x40);
    dll_702_func_1E9C(7, 0x10000);
    arg1->unk340 |= 1;
    gDLL_18->vtbl->func7(arg0, arg1, delayFloat, 3);
    sp44.yaw = sKTData->unkF8;
    sp44.pitch = 0;
    sp44.roll = 0;
    sp44.transl.x = 0.0f;
    sp44.transl.y = 0.0f;
    sp44.transl.z = 0.0f;
    sp44.scale = 1.0f;
    matrix_from_srt(&sp60, &sp44);
    vec3_transform(&sp60, arg1->unk27C, 0.0f, -arg1->unk278, &arg0->speed.x, &temp_y, &arg0->speed.z);
    if (temp_t7 != 0) {
        arg0->srt.yaw = (s16) (s32) ((f32) sKTData->unkF8 + (16384.0f * arg0->animProgress));
    } else {
        arg0->srt.yaw = (s16) (s32) ((f32) sKTData->unkF8 - (16384.0f * arg0->animProgress));
    }
    return 0;
}

// offset: 0x28BC | func: 25
static s32 dll_702_func_28BC(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    u16 var_t0;

    var_t0 = sKTData->unkFA & 1;
    if (arg1->unk272 != 0) {
        func_80023D30(arg0, 0xF, 0.0f, 0);
        arg1->unk298 = 0.005f;
        arg1->unk278 = 0.0f;
        arg1->unk27C = 0.0f;
        sKTData->unkF8 = arg0->srt.yaw;
    }
    if (var_t0 != 0) {
        arg0->srt.yaw = (s16) (s32) ((f32) sKTData->unkF8 + (32768.0f * arg0->animProgress));
    } else {
        arg0->srt.yaw = (s16) (s32) ((f32) sKTData->unkF8 - (32768.0f * arg0->animProgress));
    }
    return 0;
}

// offset: 0x29D0 | func: 26
static s32 dll_702_func_29D0(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk272 != 0) {
        func_80023D30(arg0, _data_18[sKTData->unkFD], 0.0f, 0);
        arg1->unk298 = _data_54[sKTData->unkFD];
        arg1->unk278 = 0.0f;
        arg1->unk27C = 0.0f;
    }
    dll_702_func_1E9C(0, _data_4C[sKTData->unkFD]);
    dll_702_func_1E9C(9, 0x800);
    dll_702_func_1E9C(0xA, 0x1000);
    return 0;
}
// offset: 0x2AF0 | func: 27
static s32 dll_702_func_2AF0(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk272 != 0) {
        func_80023D30(arg0, _data_8[sKTData->unkFC], 0.0f, 0);
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
        func_80023D30(arg0, 0xC, 0.0f, 0);
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
static s32 dll_702_func_2D80(Object *self, DLL18_Data* arg1, f32 arg2) {
    return 0;
}

// offset: 0x2D98 | func: 32
static s32 dll_702_func_2D98(Object* arg0, DLL18_Data* arg1, f32 arg2) {
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
static s32 dll_702_func_2E64(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    KTrex_ObjSetup* objsetup;
    s32 sp30;
    s32 temp_a3;

    objsetup = (KTrex_ObjSetup*)arg0->setup;
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 1);
        sKTData->unkFC = 0;
        sKTData->unkFA &= ~0x20;
        arg1->unk28C = objsetup->unk38[sKTData->unkFC] / 1000.0f;
    }
    if (dll_702_func_FE4(arg1, sKTData) != 0) {
        dll_702_push_state(2);
        return 4;
    }

    temp_a3 = sKTData->unkFA & 1;
    if (sKTData->unkFC == 0) {
        if (((s32) sKTData->unk101 >= 2) && !(sKTData->unkFA & 0x20) && (((temp_a3 == 0) && (sKTData->unk8 >= 0.7f)) || ((temp_a3 != 0) && (sKTData->unk8 <= 0.3f)))) {
            sp30 = (s32) sKTData->unk101 >> 1;
            if (rand_next(0, 0x64) <= (s32) objsetup->unk56[sp30]) {
                sKTData->unk103 = 2;
                dll_702_push_state(5);
                sKTData->unkFD = 1;
                return 5;
            }
            if (rand_next(0, 0x64) <= (s32) objsetup->unk52[sp30]) {
                sKTData->unkFD = 0;
                dll_702_push_state(0xB);
                return 5;
            }
            sKTData->unkFA |= 0x20;
        }
    }
    if ((sKTData->unkFE & sKTData->unkFF) && 
            (((temp_a3 == 0) && (sKTData->unk8 <= sKTData->unkF4)) || ((temp_a3 != 0) && (sKTData->unkF4 <= sKTData->unk8)))) {
        sKTData->unk103 = 1;
        dll_702_push_state(5);
        sKTData->unkFD = 1;
        return 5;
    }
    return 0;
}

// offset: 0x3160 | func: 34
static s32 dll_702_func_3160(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 2);
    } else if (arg1->unk33A != 0) {
        sKTData->unk8 = dll_702_func_C74(arg0, sKTData);
        return dll_702_pop_state() + 1;
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
        sKTData->unk4 = (f32) objsetup->unk44[sKTData->unkFD];
    } else {
        sKTData->unk4 -= delayFloat;
        if (sKTData->unk4 < 0.0f) {
            sKTData->unk4 = 0.0f;
        }
        if ((arg1->unk33A != 0) && (sKTData->unk4 <= 0.0f)) {
            return dll_702_pop_state() + 1;
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
        sKTData->unkFC = 1;
        arg1->unk28C = objsetup->unk38[sKTData->unkFC] / 1000.0f;
    }
    if (dll_702_func_FE4(arg1, sKTData) != 0) {
        sKTData->unk103 -= 1;
    }
    if (sKTData->unk103 <= 0) {
        dll_702_push_state(2);
        dll_702_push_state(6);
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
        return dll_702_pop_state() + 1;
    }

    return 0;
}

// offset: 0x3518 | func: 38
static s32 dll_702_func_3518(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 6);
        arg0->unk0xaf &= ~0x8;
    } else if (arg1->unk33A != 0) {
        return 9;
    }

    return 0;
}

// offset: 0x35A0 | func: 39
static s32 dll_702_func_35A0(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    KTrex_ObjSetup* objsetup;

    objsetup = (KTrex_ObjSetup*)arg0->setup;
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 7);
        sKTData->unk4 = (f32) objsetup->unk4A[(s32) sKTData->unk101 >> 1];
        sKTData->unkFA |= 0x10;
        sKTData->unkFA &= ~8;
        arg0->unk0xaf &= ~8;
    } else {
        if ((sKTData->unkFA & 8) || (sKTData->unk4 -= delayFloat) <= 0.0f) {
            if (sKTData->unkFA & 8) {
                sKTData->unk102 -= 1;
            }
            if ((s32) sKTData->unk102 <= 0) {
                return 2;
            }
            arg0->unk0xaf |= 8;
            return 0xA;
        }
    }

    return 0;
}

// offset: 0x3720 | func: 40
static s32 dll_702_func_3720(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    u16 temp;
    
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 8);
    } else if (arg1->unk33A != 0) {
        if (sKTData->unkFA & 8) {
            sKTData->unk101 += 1;
            main_set_bits(BIT_572, sKTData->unk101);
        }
        temp = sKTData->unkFA;
        sKTData->unkC = (temp >> 1) & 3;
        sKTData->unk4 = 300.0f;
        gDLL_2_Camera->vtbl->func8(2, 0);
        return 11;
    }
    return 0;
}

// offset: 0x3828 | func: 41
static s32 dll_702_func_3828(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    KTrex_ObjSetup* objsetup;
    s32 sp28;
    s32 temp_t0;

    objsetup = (KTrex_ObjSetup*)arg0->setup;
    sp28 = ((s32) sKTData->unkFA >> 1) & 3;
    temp_t0 = sKTData->unkFA & 1;
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 1);
        sKTData->unkFC = 2;
        arg1->unk28C = objsetup->unk38[sKTData->unkFC] / 1000.0f;
    }
    if (dll_702_func_FE4(arg1, sKTData) != 0) {
        dll_702_push_state(10);
        return 4;
    }
    sKTData->unk4 = (f32) (sKTData->unk4 - delayFloat);
    if (sKTData->unk4 <= 0.0f) {
        sKTData->unk4 = 0.0f;
    }
    if ((sKTData->unk4 <= 0.0f) && (sp28 == sKTData->unkC) && (((temp_t0 == 0) && (sKTData->unk8 >= 0.75f)) || ((temp_t0 != 0) && (sKTData->unk8 <= 0.25f)))) {
        if (sKTData->unkFA & 8) {
            sKTData->unk101 += 1;
            sKTData->unkFD = 0;
            dll_702_push_state(11);
            dll_702_push_state(4);
        } else {
            dll_702_push_state(2);
        }
        gDLL_2_Camera->vtbl->func8(3, 0);
        main_set_bits(BIT_572, (u32) sKTData->unk101);
        return 7;
    }
    return 0;
}

// offset: 0x3AA0 | func: 42
static s32 dll_702_func_3AA0(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    if (arg1->unk273 != 0) {
        gDLL_18->vtbl->func4(arg0, arg1, 3);
    } else if (arg1->unk33A != 0) {
        sKTData->unkFA ^= 1;
        if (sKTData->unkFA & 1) {
            sKTData->unkD0 = sKTData->unk70;
            sKTData->unkD4 = sKTData->unk80;
            sKTData->unkD8 = sKTData->unk90;
            sKTData->unkDC = sKTData->unkA0;
            sKTData->unkE0 = sKTData->unkB0;
            sKTData->unkE4 = sKTData->unkC0;
        } else {
            sKTData->unkD0 = sKTData->unk10;
            sKTData->unkD4 = sKTData->unk20;
            sKTData->unkD8 = sKTData->unk30;
            sKTData->unkDC = sKTData->unk40;
            sKTData->unkE0 = sKTData->unk50;
            sKTData->unkE4 = sKTData->unk60;
        }
        sKTData->unk8 = dll_702_func_C74(arg0, sKTData);
        return 3;
    }

    return 0;
}

/*0x0*/ static const char str_0[] = "TREX: state stack overflow\n";
/*0x1C*/ static const char str_1C[] = "State Error!!!\n";
