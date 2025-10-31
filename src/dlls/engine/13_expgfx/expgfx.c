#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    u8 unkE;
    u8 unkF;
} UnkBss0Struct_Unk0;

typedef struct {
    UnkBss0Struct_Unk0 unk0[4];
    SRT unk40;
    Vec3f unk58;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    union {
        u32 unk7C;
        struct {
            u32 unk7C_31: 1;
            u32 unk7C_30: 1;
            u32 unk7C_29: 1;
            u32 unk7C_28: 1;
            u32 unk7C_27: 1;
            u32 unk7C_26: 1;
            u32 unk7C_25: 1;
            u32 unk7C_24: 1;
            u32 unk7C_23: 1;
            u32 unk7C_22: 1;
            u32 unk7C_21: 1;
            u32 unk7C_20: 1; // & 0x100000
            u32 unk7C_19: 1;
            u32 unk7C_18: 1;
            u32 unk7C_17: 1;
            u32 unk7C_16: 1;
            u32 unk7C_15: 1;
            u32 unk7C_14: 1;
            u32 unk7C_13: 1;
            u32 unk7C_12: 1;
            u32 unk7C_11: 1;
            u32 unk7C_10: 1;
            u32 unk7C_9: 1;
            u32 unk7C_8: 1;
            u32 unk7C_7: 1;
            u32 unk7C_6: 1;
            u32 unk7C_5: 1;
            u32 unk7C_4: 1;
            u32 unk7C_3: 1;
            u32 unk7C_2: 1;
            u32 unk7C_1: 1;
            u32 unk7C_0: 1;
        } bits;
    };
    u32 unk80;
    u16 unk84;
    u16 unk86;
    u16 unk88;
    u16 unk8A : 7;
    u16 unk8A_2 : 1;
    u16 unk8A_3 : 4;
    u16 unk8A_4 : 2;
    u16 unk8A_5 : 1;
    u16 unk8A_6 : 1;
    u8 unk8C;
    u8 unk8D;
    u8 unk8E;
} UnkBss0Struct;

typedef struct {
    Object *unk0;
    Object *unk4;
    Texture *unk8;
    u16 unkC;
} UnkBss190Struct;

typedef struct {
    Texture *unk0;
    s32 unk4; // lifetime?
    s32 unk8; // texture ID
    s32 unkC;
} UnkBss370Struct;

/*0x0*/ static s16 _data_0[] = {
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
/*0x3C*/ static u8 _data_3C[30] = {0};
/*0x5C*/ static u32 _data_5C = 0x00000000;
/*0x60*/ static u32 _data_60 = 0x00000000;
/*0x64*/ static u8 _data_64 = 0;
/*0x68*/ static u32 _data_68[] = {
    0x00000000, 0x00000000
};
/*0x70*/ static u32 _data_70[] = {
    0x40000102, 0x00000000, 0x00000000, 0x00000000, 0x40000203, 0x00000000, 0x00000000, 0x00000000
};
/*0x90*/ static u32 _data_90 = 0x00000000;
/*0x94*/ static u32 _data_94 = 0x00000000;
/*0x98*/ static u32 _data_98 = 0x00000000;
/*0x9C*/ static u32 _data_9C[] = {
    0xfc180000, 0x03e803e8, 0x000003e8, 0x03e80000, 0xfc18fc18, 0x0000fc18
};
/*0xB4*/ static u32 _data_B4[] = {
    0xfc18fc18, 0x000003e8, 0xfc180000, 0x03e803e8, 0x0000fc18, 0x03e80000, 0xfc18fc18, 0x000003e8, 
    0xfc180000, 0x03e803e8, 0x0000fc18, 0x03e80000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static UnkBss0Struct *_bss_0[30];
/*0x78*/ static u32 _bss_78[30];
/*0xF0*/ static s8 _bss_F0[30];
/*0x110*/ static u32 _bss_110;
/*0x118*/ static Object *_bss_118[30];
/*0x190*/ static UnkBss190Struct _bss_190[30];
/*0x370*/ static UnkBss370Struct _bss_370[8];

/*static*/ s16 dll_13_func_2060(Texture* arg0, Object* arg1, Object* arg2);
static void dll_13_func_2254(u8 arg0);
/*static*/ void dll_13_func_2298(u8 a0, s32 a1, s32 a2);
/*static*/ void dll_13_func_4684(UnkBss0Struct* a0);
/*static*/ s32 dll_13_func_4A18(s16* arg0, s16* arg1, s16 arg2, s32 arg3, Object *arg4);
static void dll_13_func_4DCC(void);
static void dll_13_func_4F2C(s32 arg0, s32 arg1, s32 arg2);
/*static*/ s16 dll_13_func_5068(s32 textureID);

// offset: 0x0 | ctor
void dll_13_ctor(s32 arg0) {
    s32 i;

    for (i = 0; i < 30; i++) {
        _bss_78[i] = 0;
        _bss_F0[i] = 0;
        _data_0[i] = -1;
    }

    for (i = 0; i < 30; i++) {
        _bss_0[i] = mmAlloc(sizeof(UnkBss0Struct) * 30, ALLOC_TAG_EXPGFX_COL, NULL);
        bzero(_bss_0[i], sizeof(UnkBss0Struct) * 30);
    }
    
    bzero(_bss_190, sizeof(UnkBss190Struct) * 30);
}

// offset: 0xE0 | dtor
void dll_13_dtor(s32 arg0) {
    s32 i;

    dll_13_func_4DCC();

    for (i = 0; i < 30; i++) {
        mmFree(_bss_0[i]);
        
    }
}

// offset: 0x158 | func: 0 | export: 0
void dll_13_func_158(void) {
    s32 i;

    dll_13_func_4DCC();

    for (i = 0; i < 30; i++) {
        _bss_78[i] = 0;
        _bss_F0[i] = 0;
        _data_0[i] = -1;
    }

    for (i = 0; i < 8; i++) {
        if (_bss_370[i].unk0 != NULL) {
            texture_destroy(_bss_370[i].unk0);
        }

        _bss_370[i].unk0 = NULL;
        _bss_370[i].unk8 = 0;
        _bss_370[i].unk4 = 0;
        _bss_370[i].unkC = 0;
    }
}

// offset: 0x228 | func: 1 | export: 1
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_228.s")
#else
// needs dll_13_func_4684 to be static
s16 dll_13_func_228(ExpgfxStruct* arg0, s32 arg1) {
    UnkBss0Struct* var_a3; // sp7C
    f32 fv1;
    s16 sp76;
    s16 sp74;
    Object* var_a2;
    Texture* temp_a0;
    f32 temp_fv0;
    s16 temp_v0_3;
    Object* temp_v0_5; // sp60
    s16 sp5A[4];
    u32 sp54;
    s32 sp50;
    f32 sp44[3];
    s32 temp_t9;
    u32 temp_v0_2;

    sp74 = 0;
    sp5A[3] = 0;
    sp5A[1] = 0;
    sp76 = 0;
    sp54 = osGetCount();
    if (dll_13_func_4A18(&sp76, &sp74, arg0->unk5E, arg1, arg0->unk0) == -1) {
        return -1;
    }
    if (sp76 < 30) {
        _bss_118[sp76] = arg0->unk0;
    }
    if ((sp76 < 30) && (arg0->unk44 & 0x40000)) {
        _bss_110 |= 1 << sp76;
    } else {
        _bss_110 &= ~(1 << sp76);
    }
    var_a3 = _bss_0[sp76];
    var_a3 += sp74;
    _data_5C += 1;
    if (_data_5C > 30000) {
        _data_5C = 0;
    }
    var_a3->unk0[2].unk6 = _data_5C;
    var_a3->unk7C = arg0->unk44;
    var_a3->unk80 = arg0->unk48;
    var_a3->unk8A_4 = 0;
    sp50 = dll_13_func_5068(arg0->unk42);
    if (sp50 < 0) {
        dll_13_func_4F2C(sp76, sp74, 1);
        return -1;
    }
    temp_a0 = _bss_370[sp50].unk0;
    if (temp_a0 != NULL) {
        temp_a0->unk5 += 1;
        temp_a0->unkE = (u16) (u8) arg0->unk61;
    }
    temp_v0_2 = var_a3->unk7C;
    var_a2 = NULL;
    temp_v0_5 = arg0->unk0;
    if (temp_v0_5 == NULL) {
        var_a3->unk40.transl.x = arg0->transform.transl.x;
        var_a3->unk40.transl.y = arg0->transform.transl.y;
        var_a3->unk40.transl.z = arg0->transform.transl.z;
        var_a3->unk40.scale = arg0->transform.scale;
        var_a3->unk40.roll = arg0->transform.roll;
        var_a3->unk40.pitch = arg0->transform.pitch;
        var_a3->unk40.yaw = arg0->transform.yaw;
    } else if (temp_v0_2 & 0x200000) {
        var_a3->unk40.transl.x = temp_v0_5->positionMirror.x;
        var_a3->unk40.transl.y = temp_v0_5->positionMirror.y;
        var_a3->unk40.transl.z = temp_v0_5->positionMirror.z;
        var_a3->unk40.scale = temp_v0_5->srt.scale;
        var_a3->unk40.roll = temp_v0_5->srt.roll;
        var_a3->unk40.pitch = temp_v0_5->srt.pitch;
        var_a3->unk40.yaw = temp_v0_5->srt.yaw;
        if ((temp_v0_2 & 2) || (temp_v0_2 & 4)) {
            arg0->unk24.x += temp_v0_5->speed.x;
            arg0->unk24.y += temp_v0_5->speed.y;
            arg0->unk24.z += temp_v0_5->speed.z;
        }
        if (temp_v0_5 != NULL) {
            var_a2 = temp_v0_5->parent;
        }
        temp_v0_5 = NULL;
    }
    temp_v0_3 = dll_13_func_2060(temp_a0, temp_v0_5, var_a2);
    if (temp_v0_3 == -1) {
        return -1;
    }
    var_a3->unk8A = temp_v0_3;
    var_a3->unk58.x = var_a3->unk64 = arg0->unk30.x;
    var_a3->unk58.y = var_a3->unk68 = arg0->unk30.y;
    var_a3->unk58.z = var_a3->unk6C = arg0->unk30.z;
    var_a3->unk70 = arg0->unk24.x;
    var_a3->unk74 = arg0->unk24.y;
    var_a3->unk78 = arg0->unk24.z;
    var_a3->unk0[0].unkF = arg0->unk60;
    var_a3->unk0[3].unk6 = arg0->unk4;
    var_a3->unk0[0].unk6 = arg0->unk8;
    var_a3->unk0[1].unk6 = arg0->unk8;
    // @fake
    if (arg0->unk3C) {}
    temp_fv0 = arg0->unk3C * 65535.0f;
    if (var_a3->unk7C & 0x100000) {
        var_a3->unk84 = 0;
        var_a3->unk88 = temp_fv0 / var_a3->unk0[1].unk6;
        var_a3->unk86 = temp_fv0;
    } else {
        var_a3->unk84 = temp_fv0;
        var_a3->unk86 = var_a3->unk84;
        var_a3->unk88 = 0;
    }
    if ((var_a3->unk7C & 0x20000) || (var_a3->unk7C & 0x04000000)) {
        var_a3->unk40.transl.x = arg0->transform.transl.x;
        var_a3->unk40.transl.y = arg0->transform.transl.y;
        var_a3->unk40.transl.z = arg0->transform.transl.z;
        var_a3->unk40.scale = arg0->transform.scale;
        var_a3->unk40.roll = arg0->transform.roll ;
        var_a3->unk40.pitch = arg0->transform.pitch ;
        var_a3->unk40.yaw = arg0->transform.yaw ;
    }
    var_a3->unk8A_6 = _data_60;
    if (var_a3->unk80 & 8) {
        var_a3->unk80 = var_a3->unk80 ^ 8;
        temp_fv0 = var_a3->unk0[0].unk6 * 1.5f;
        var_a3->unk58.x += var_a3->unk70 * temp_fv0;
        var_a3->unk58.y += var_a3->unk74 * temp_fv0;
        var_a3->unk58.z += var_a3->unk78 * temp_fv0;
        var_a3->unk70 *= -1.0f;
        var_a3->unk74 *= -1.0f;
        var_a3->unk78 *= -1.0f;
    }
    if (var_a3->unk80 & 0x10) {
        var_a2 = get_player();
        var_a3->unk80 ^= 0x10;
        if (var_a3->unk7C & 1) {
            sp44[0] = var_a2->positionMirror.x - var_a3->unk64;
            sp44[2] = var_a2->positionMirror.z - var_a3->unk6C;
            temp_fv0 = SQ(sp44[0]) + SQ(sp44[2]);
            if (temp_fv0 < 3600.0f && var_a2->speed.x != 0.0f && var_a2->speed.z != 0.0f) {
                temp_fv0 = var_a3->unk0[0].unk6 * 2;
                var_a3->unk70 += (var_a2->positionMirror.x - var_a3->unk64) / temp_fv0;
                var_a3->unk74 += ((var_a2->positionMirror.y + 30.0f) - var_a3->unk68) / temp_fv0;
                var_a3->unk78 += (var_a2->positionMirror.z - var_a3->unk6C) / temp_fv0;
            }
        } else {
            sp44[0] = var_a2->positionMirror.x - (var_a3->unk64 + temp_v0_5->srt.transl.x);
            sp44[2] = var_a2->positionMirror.z - (var_a3->unk6C + temp_v0_5->srt.transl.z);
            temp_fv0 = SQ(sp44[0]) + SQ(sp44[2]);
            if (temp_fv0 < 3600.0f && var_a2->speed.x != 0.0f && var_a2->speed.z != 0.0f) {
                temp_fv0 = var_a3->unk0[0].unk6 * 2;
                var_a3->unk70 -= (var_a2->positionMirror.x - (var_a3->unk64 + temp_v0_5->srt.transl.x)) / temp_fv0;
                var_a3->unk74 -= ((var_a2->positionMirror.y + 30.0f) - (var_a3->unk68 + temp_v0_5->srt.transl.y)) / temp_fv0;
                var_a3->unk78 -= (var_a2->positionMirror.z - (var_a3->unk6C + temp_v0_5->srt.transl.z)) / temp_fv0;
            }
        }
    }
    if (sp50 == 1) {
        _data_90 += 1;
        _data_94 += osGetCount() - sp54;
        _data_98 = ((s32) _data_94 / (s32) _data_90);
    }
    if (arg0->unk48 & 0x20) {
        var_a3->unk8C = arg0->unk58[0] >> 8;
        var_a3->unk8D = arg0->unk58[1] >> 8;
        var_a3->unk8E = arg0->unk58[2] >> 8;
        var_a3->unk0[1].unkF = arg0->unk4C[0] >> 8;
        var_a3->unk0[2].unkF = arg0->unk4C[1] >> 8;
        var_a3->unk0[3].unkF = arg0->unk4C[2] >> 8;
    }
    temp_v0_2 = sp5A[1] << 5;
    var_a3->unk0[0].unk8 = temp_v0_2;
    temp_t9 = sp5A[3] << 5;
    var_a3->unk0[0].unkA = temp_t9;
    var_a3->unk0[1].unk8 = 0;
    var_a3->unk0[1].unkA = temp_t9;
    var_a3->unk0[2].unk8 = 0;
    var_a3->unk0[2].unkA = 0;
    var_a3->unk0[3].unk8 = temp_v0_2;
    var_a3->unk0[3].unkA = 0;
    if (var_a3->unk80 & 2) {
        dll_13_func_4684(var_a3);
    }
    return var_a3->unk0[2].unk6;
}
#endif

// offset: 0xC18 | func: 2 | export: 2
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_C18.s")
#else
// needs dll_13_func_2298 to be static
void dll_13_func_C18(u8 arg0, s32 updateRate, s32 arg2, s32 arg3) {
    u8 i;

    if (func_80000824(-1) != 1) {
        D_8008C504 = 1;
        dll_13_func_2298(arg0, arg1, 0);
        D_8008C504 = 0;
        i = 30;
        do {
            i--;
            _data_3C[i] = 0;
        } while (i != 0);
        gDLL_17_partfx->vtbl->func2(0);
        _data_68[0] = 1;
    }
}
#endif

// offset: 0xCEC | func: 3 | export: 3
void dll_13_func_CEC(void) {
    UnkBss0Struct* var_s0;
    s32 i;
    s32 k;

    for (i = 0; i < 30; i++) {
        var_s0 = _bss_0[i];

        for (k = 0; k < 30; k++) {
            if (_bss_78[i] & (1 << k)) {
                if (_bss_190[var_s0->unk8A].unk8 != NULL) {
                    _bss_190[var_s0->unk8A].unk8->unk5 -= 1;
                }
                dll_13_func_2254(var_s0->unk8A);
                var_s0->unk0[2].unk6 = -1;
                _bss_78[i] &= ~(1 << k);
            }

            var_s0++;
        }

        _bss_F0[i] = 0;
        _data_0[i] = -1;
        _bss_118[i] = NULL;
        _data_3C[i] = 0;
    }

    for (i = 0; i < 8; i++) {
        if (_bss_370[i].unk0 != NULL) {
            texture_destroy(_bss_370[i].unk0);
        }

        _bss_370[i].unk0 = NULL;
        _bss_370[i].unk8 = 0;
        _bss_370[i].unk4 = 0;
        _bss_370[i].unkC = 0;
    }
}

// offset: 0xED4 | func: 4 | export: 4
void dll_13_func_ED4(Object* obj) {
    UnkBss0Struct* var_s1;
    s32 var_s0;
    s32 var_s2;

    var_s0 = 0;
    
    if (obj != NULL) {
        for (var_s2 = 0; var_s2 < 30; var_s2++) {
            var_s1 = _bss_0[var_s2];
            if (obj == _bss_118[var_s2]) {
                for (var_s0 = 0; var_s0 < 30; var_s0++) {
                    if ((var_s1 != NULL) && (obj == _bss_190[var_s1->unk8A].unk0)) {
                        dll_13_func_4F2C(var_s2, var_s0, 0);
                    }
                    var_s1 += 1;
                    if (_bss_F0[var_s2] == 0) {
                        _data_0[var_s2] = -1;
                    }
                }
                _bss_118[var_s2] = NULL;
                _data_3C[var_s2] = 0;
            }
        }
    }
}

// offset: 0x1048 | func: 5 | export: 5
void dll_13_func_1048(Object *obj) {
    dll_13_func_ED4(obj);
}

// offset: 0x1080 | func: 6 | export: 6
void dll_13_func_1080(Object *arg0, Gfx **gdl, Mtx **mtxs, Vertex **vertices, s32 arg4, s32 arg5, s32 arg6);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1080.s")

// offset: 0x1EEC | func: 7 | export: 7
void dll_13_func_1EEC(void) { }

// offset: 0x1EF4 | func: 8 | export: 8
void dll_13_func_1EF4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3) { }

// offset: 0x1F0C | func: 9 | export: 9
void dll_13_func_1F0C(Object *obj) {
    dll_13_func_ED4(obj);
}

// offset: 0x1F44 | func: 10 | export: 10
s8 dll_13_func_1F44(Object* obj) {
    s16 i;
    s8 var_v1;

    var_v1 = 0;
    _data_64 = 0;
    for (i = 0; i < 30; i++) {
        if ((obj->id == OBJ_FXEmit) || (obj == _bss_118[i])) {
            if ((1 << i) & _bss_110) {
                _data_3C[i] = 2;
                if (var_v1 == 1) {
                    var_v1 = 3;
                } else {
                    var_v1 = 2;
                }
            } else {
                _data_3C[i] = 1;
                if (var_v1 == 2) {
                    var_v1 = 3;
                } else {
                    var_v1 = 1;
                }
            }
        } else {
            _data_3C[i] = 0;
        }
    }
    if ((obj->id == OBJ_WL_WallTorch) && (_data_0[29] != -1)) { // ???
        _data_64 = 1;
    }
    return var_v1;
}

// offset: 0x2060 | func: 11
s16 dll_13_func_2060(Texture* arg0, Object* arg1, Object* arg2) {
    s32 i;

    i = 0;
    while (TRUE) {
        if ((_bss_190[i].unkC != 0) && (arg0 == _bss_190[i].unk8) && 
                (arg1 == _bss_190[i].unk0) && (arg2 == _bss_190[i].unk4)) {
            if (_bss_190[i].unkC >= 0xFFFF) {
                return -1;
            }
            _bss_190[i].unkC++;
            return i;
        }
        i += 1;
        if (i >= 30) {
            for (i = 0; i < 30; i++) {
                if (_bss_190[i].unkC == 0) {
                    _bss_190[i].unkC = 1;
                    _bss_190[i].unk8 = arg0;
                    _bss_190[i].unk0 = arg1;
                    _bss_190[i].unk4 = arg2;
                    return i;
                }
            }
            return -1;
        }
    }
}

// offset: 0x2254 | func: 12
static void dll_13_func_2254(u8 arg0) {
    if (_bss_190[arg0].unkC != 0) {
        _bss_190[arg0].unkC -= 1;
    }
}

// offset: 0x2298 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_2298.s")

// offset: 0x4684 | func: 14
// https://decomp.me/scratch/mORFk
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_4684.s")

// offset: 0x4A18 | func: 15
s32 dll_13_func_4A18(s16* arg0, s16* arg1, s16 arg2, s32 arg3, Object *arg4) {
    s32 var_t0;
    s32 var_t1_2;
    s16 var_v1;
    s32 var_t1;

    var_v1 = -1;
    var_t1 = 0;
    var_t0 = 0;
    for (var_t0 = 0; var_t0 < 30; var_t0++) {
        if ((arg4 == _bss_118[var_t0]) && (arg2 == _data_0[var_t0]) && (_bss_F0[var_t0] < 30)) {
            var_v1 = var_t0;
            var_t1 = 1;
            break;
        }
    }
    
    if (var_t1 != 0) {
        for (var_t1_2 = 0; var_t1_2 < 30; var_t1_2++) {
            if (!(_bss_78[var_v1] & (1 << var_t1_2))) {
                *arg1 = var_t1_2;
                *arg0 = var_v1;
                _bss_78[var_v1] |= (1 << var_t1_2);
                _bss_F0[var_v1]++;
                return 1;
            }
        }
    }
    
    var_t1 = 0;
    if (arg3 == -1) {
        for (var_t0 = 0; var_t0 < 29; var_t0++) {
            if (_bss_F0[var_t0] <= 0) {
                var_v1 = var_t0;
                var_t1 = 1;
                _bss_F0[var_t0] = 0;
                break;
            }
        }
    } else if (arg3 != -1) {
        var_t0 = arg3;
        if (_bss_F0[arg3] < 30) {
            var_v1 = arg3;
            var_t1 = 1;
        }
    }
    
    if (var_t1 != 0) {
        for (var_t1_2 = 0; var_t1_2 < 30; var_t1_2++) {
            if (arg2){} // fake

            if (!(_bss_78[var_v1] & (1 << var_t1_2))) {
                *arg1 = var_t1_2;
                *arg0 = var_v1;
                _bss_78[var_v1] |= (1 << var_t1_2);
                _data_0[var_t0] = arg2;
                _bss_F0[var_v1]++;
                return 1;
            }
        }

        return -1;
    }
    
    return -1;
}

// offset: 0x4DCC | func: 16
static void dll_13_func_4DCC(void) {
    UnkBss0Struct* var_s0;
    s32 i;
    s32 k;

    for (i = 0; i < 30; i++) {
        var_s0 = _bss_0[i];
        for (k = 0; k < 30; k++) {
            if (_bss_78[i] & (1 << k)) {
                if (_bss_190[var_s0->unk8A].unk8 != NULL) {
                    _bss_190[var_s0->unk8A].unk8->unk5 -= 1;
                }
                dll_13_func_2254(var_s0->unk8A);
                var_s0->unk0[2].unk6 = -1;
                _bss_78[i] &= ~(1 << k);
            }
            var_s0 += 1;
        }
        _bss_F0[i] = 0;
        _data_0[i] = -1;
    }
}

// offset: 0x4F2C | func: 17
static void dll_13_func_4F2C(s32 arg0, s32 arg1, s32 arg2) {
    UnkBss0Struct* temp_v1;

    if (_bss_78[arg0] & (1 << arg1)) {
        temp_v1 = _bss_0[arg0];
        temp_v1 += arg1;
        temp_v1->unk7C = 0;
        if (arg2 == 0) {
            if (_bss_190[temp_v1->unk8A].unk8 != NULL) {
                _bss_190[temp_v1->unk8A].unk8->unk5 -= 1;
                dll_13_func_2254(temp_v1->unk8A);
            }
        }
        temp_v1->unk0[2].unk6 = -1;
        _bss_78[arg0] &= ~(1 << arg1);
        _bss_F0[arg0] -= 1;
        if (_bss_F0[arg0] == 0) {
            _data_0[arg0] = -1;
        }
    }
}

// offset: 0x5068 | func: 18
s16 dll_13_func_5068(s32 textureID) {
    s32 i;
    s32 var_a3;
    s32 var_a0;
    
    i = 0;
    while (TRUE) {
        if (textureID == _bss_370[i].unk8) {
            // Existing found
            if ((_bss_370[i].unk0 != NULL) && (_bss_370[i].unk0->unk5 >= 0xFE)) {
                return -1;
            }
            _bss_370[i].unk4 = 1000;
            return i;
        }
        i += 1;
        if (i >= 8) {
            i = 0;
            var_a3 = 0;
            while (TRUE) {
                if (_bss_370[i].unk0 == NULL) {
                    // Empty slot found
                    _bss_370[i].unk0 = queue_load_texture_proxy(textureID);
                    if ((_bss_370[i].unk0 != NULL) && (_bss_370[i].unk0->unk5 >= 0xFE)) {
                        if (_bss_370[i].unk0 != NULL) {
                            texture_destroy(_bss_370[i].unk0);
                        }
                        _bss_370[i].unk0 = NULL;
                        return -1;
                    }
                    if (_bss_370[i].unk0 != NULL) {
                        _bss_370[i].unk4 = 1000;
                        _bss_370[i].unk8 = textureID;
                        _bss_370[i].unkC = (s32) (_bss_370[i].unk0->levels * _bss_370[i].unk0->unk18) >> 3;
                        return (s16) i;
                    }
                    return -2;
                }
                i += 1;
                if (i >= 8) {
                    // No room, replace the oldest
                    var_a0 = 64000;
                    for (i = 0; i < 8; i++) {
                        if (_bss_370[i].unk4 < var_a0) {
                            var_a0 = _bss_370[i].unk4;
                            var_a3 = i;
                        }
                    }
                    if (_bss_370[var_a3].unk0 != NULL) {
                        texture_destroy(_bss_370[var_a3].unk0);
                    }
                    _bss_370[var_a3].unk0 = NULL;
                    _bss_370[var_a3].unk0 = queue_load_texture_proxy(textureID);
                    if (_bss_370[var_a3].unk0 != NULL) {
                        _bss_370[var_a3].unk4 = 1000;
                        _bss_370[var_a3].unk8 = textureID;
                        _bss_370[var_a3].unkC = (s32) (_bss_370[var_a3].unk0->levels * _bss_370[var_a3].unk0->unk18) >> 3;
                        return (s16) var_a3;
                    }
                    return -3;
                }
            }
        }
    }
}

// offset: 0x52B4 | func: 19
void dll_13_func_52B4(s32 arg0) {
    UnkBss370Struct* var_s0;
    s32 i;

    for (i = 0; i < 8; i++) {
        var_s0 = &_bss_370[i];
        if (var_s0->unk8 != 0) {
            var_s0->unk4 -= gUpdateRate;
            if (var_s0->unk4 <= 0) {
                var_s0->unk8 = 0;
                var_s0->unk4 = 0;
                var_s0->unkC = 0;
                texture_destroy(var_s0->unk0);
                var_s0->unk0 = NULL;
            }
        }
    }
}

// offset: 0x5358 | func: 20
void dll_13_func_5358(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3) {
    Vec3f spC;
    Vec3f sp0;

    spC.x = arg2->x - arg0->x;
    spC.y = arg2->y - arg0->y;
    spC.z = arg2->z - arg0->z;
    sp0.x = arg1->x - arg0->x;
    sp0.y = arg1->y - arg0->y;
    sp0.z = arg1->z - arg0->z;
    arg3->x = (sp0.y * spC.z) - (sp0.z * spC.y);
    arg3->y = -((sp0.x * spC.z) - (sp0.z * spC.x));
    arg3->z = (sp0.x * spC.y) - (sp0.y * spC.x);
}

/*0x0*/ static const char str_0[] = "WARN no free particle\n";
/*0x18*/ static const char str_18[] = "error no texture\n";
/*0x2C*/ static const char str_2C[] = "expgfx.c: invalid tabindex\n";
/*0x48*/ static const char str_48[] = "expgfx.c: scale overflow\n";
/*0x64*/ static const char str_64[] = "expgfx.c: Error on owner free\n";
/*0x84*/ static const char str_84[] = "\t exp x %f\n";
/*0x90*/ static const char str_90[] = "\t exp z %f\n";
/*0x9C*/ static const char str_9C[] = "expgfx.c: addToTable usage overflow\n";
/*0xC4*/ static const char str_C4[] = "expgfx.c: exptab is FULL\n";
/*0xE0*/ static const char str_E0[] = "expgfx.c: mismatch in add/remove in exptab\n";
/*0x10C*/ static const char str_10C[] = "notexture \n";
/*0x118*/ static const char str_118[] = "notexture \n";
