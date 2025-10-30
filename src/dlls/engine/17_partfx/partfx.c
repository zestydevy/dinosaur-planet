#include "PR/ultratypes.h"
#include "common.h"

/*0x0*/ static s8 _data_0 = 0;
/*0x4*/ static DLL_Unknown *_data_4 = NULL;
/*0x8*/ static DLL_Unknown *_data_8 = NULL;
/*0xC*/ static DLL_Unknown *_data_C = NULL;
/*0x10*/ static DLL_Unknown *_data_10 = NULL;
/*0x14*/ static DLL_Unknown *_data_14 = NULL;
/*0x18*/ static DLL_Unknown *_data_18 = NULL;
/*0x1C*/ static DLL_Unknown *_data_1C = NULL;
/*0x20*/ static DLL_Unknown *_data_20 = NULL;
/*0x24*/ static DLL_Unknown *_data_24 = NULL;
/*0x28*/ static DLL_Unknown *_data_28 = NULL;
/*0x2C*/ static DLL_Unknown *_data_2C = NULL;
/*0x30*/ static DLL_Unknown *_data_30 = NULL;
/*0x34*/ static DLL_Unknown *_data_34 = NULL;
/*0x38*/ static DLL_Unknown *_data_38 = NULL;
/*0x3C*/ static DLL_Unknown *_data_3C = NULL;
/*0x40*/ static DLL_Unknown *_data_40 = NULL;
/*0x44*/ static DLL_Unknown *_data_44 = NULL;
/*0x48*/ static DLL_Unknown *_data_48 = NULL;
/*0x4C*/ static DLL_Unknown *_data_4C = NULL;
/*0x50*/ static u32 _data_50[] = {
    0x000000df, 0x000001fc, 0x00000200, 0x000001fb, 0x42b40000, 0x42960000, 0xc2b40000, 
    0x42960000, 0x42c80000, 0xc2aa0000, 0xc2c80000, 0xc2aa0000, 0x00000000, 0x43200000, 0x00000000, 
    0xc2e60000
};
/*0x90*/ static f32 _data_90 = 0.10000000149011612f;
/*0x94*/ static f32 _data_94 = 0.30000001192092896f;
/*0x98*/ static f32 _data_98 = 0.10000000149011612f;
/*0x9C*/ static f32 _data_9C = 0.3f;
/*0xA0*/ static s32 _data_A0 = 0;
/*0xA4*/ static s32 _data_A4 = 0;
/*0xA8*/ static f32 _data_A8 = 0.0f;
/*0xAC*/ static f32 _data_AC = 0.0f;

/*0x0*/ static SRT _bss_0;
/*0x18*/ static s16 _bss_18[19];

// offset: 0x0 | ctor
void dll_17_ctor(void* dll) {
    s8 var_v0 = 19;
    while (var_v0 != 0) {
        var_v0--;
        _bss_18[var_v0] = 0;
    }

    _data_0 = 0;
}

// offset: 0x48 | dtor
void dll_17_dtor(void* dll) {
    s8 var_v0;

    var_v0 = 19;
    while (var_v0 != 0) {
        var_v0 -= 1;
        _bss_18[var_v0] = 0;
    }
    
    if (_data_4 != NULL) {
        dll_unload(_data_4);
    }
    _data_4 = NULL;
    if (_data_8 != NULL) {
        dll_unload(_data_8);
    }
    _data_8 = NULL;
    if (_data_C != NULL) {
        dll_unload(_data_C);
    }
    _data_C = NULL;
    if (_data_10 != NULL) {
        dll_unload(_data_10);
    }
    _data_10 = NULL;
    if (_data_14 != NULL) {
        dll_unload(_data_14);
    }
    _data_14 = NULL;
    if (_data_18 != NULL) {
        dll_unload(_data_18);
    }
    _data_18 = NULL;
    if (_data_1C != NULL) {
        dll_unload(_data_1C);
    }
    _data_1C = NULL;
    if (_data_20 != NULL) {
        dll_unload(_data_20);
    }
    _data_20 = NULL;
    if (_data_24 != NULL) {
        dll_unload(_data_24);
    }
    _data_24 = NULL;
    if (_data_28 != NULL) {
        dll_unload(_data_28);
    }
    _data_28 = NULL;
    if (_data_2C != NULL) {
        dll_unload(_data_2C);
    }
    _data_2C = NULL;
    if (_data_30 != NULL) {
        dll_unload(_data_30);
    }
    _data_30 = NULL;
    if (_data_34 != NULL) {
        dll_unload(_data_34);
    }
    _data_34 = NULL;
    if (_data_38 != NULL) {
        dll_unload(_data_38);
    }
    _data_38 = NULL;
    if (_data_3C != NULL) {
        dll_unload(_data_3C);
    }
    _data_3C = NULL;
    if (_data_40 != NULL) {
        dll_unload(_data_40);
    }
    _data_40 = NULL;
    if (_data_44 != NULL) {
        dll_unload(_data_44);
    }
    _data_44 = NULL;
    if (_data_48 != NULL) {
        dll_unload(_data_48);
    }
    _data_48 = NULL;
    if (_data_4C != NULL) {
        dll_unload(_data_4C);
    }
    _data_4C = NULL;
    
    _data_0 = 0;
}

// offset: 0x398 | func: 0 | export: 0
void dll_17_func_398(void) { }

// offset: 0x3A0 | func: 1 | export: 2
void dll_17_func_3A0(s32 arg0) {
    _data_98 += gUpdateRateF * 0.001f;
    if (_data_98 > 1.0f) {
        _data_98 = 0.1f;
    }
    _data_9C += gUpdateRateF * 0.001f;
    if (_data_9C > 1.0f) {
        _data_9C = 0.3f;
    }
    _data_A0 += (gUpdateRate * 100);
    if (_data_A0 >= 0x8000) {
        _data_A0 = 0;
    }
    _data_AC = fsin16_precise(_data_A0);
    _data_A4 += (gUpdateRate * 50);
    if (_data_A4 >= 0x8000) {
        _data_A4 = 0;
    }
    _data_A8 = fsin16_precise(_data_A4);
    if (_bss_18[0] != 0) {
        _bss_18[0] -= gUpdateRate;
        if (_bss_18[0] <= 0) {
            if (_data_4 != NULL) {
                dll_unload(_data_4);
            }
            _data_4 = NULL;
            _bss_18[0] = 0;
            _data_0 = (s8) _data_0 - 1;
        }
    }
    if (_bss_18[1] != 0) {
        _bss_18[1] -= gUpdateRate;
        if (_bss_18[1] <= 0) {
            if (_data_8 != NULL) {
                dll_unload(_data_8);
            }
            _data_8 = NULL;
            _bss_18[1] = 0;
            _data_0 = (s8) _data_0 - 1;
        }
    }
    if (_bss_18[2] != 0) {
        _bss_18[2] -= gUpdateRate;
        if (_bss_18[2] <= 0) {
            if (_data_C != NULL) {
                dll_unload(_data_C);
            }
            _data_C = NULL;
            _bss_18[2] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[3] != 0) {
        _bss_18[3] -= gUpdateRate;
        if (_bss_18[3] <= 0) {
            if (_data_10 != NULL) {
                dll_unload(_data_10);
            }
            _data_10 = NULL;
            _bss_18[3] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[4] != 0) {
        _bss_18[4] -= gUpdateRate;
        if (_bss_18[4] <= 0) {
            if (_data_14 != NULL) {
                dll_unload(_data_14);
            }
            _data_14 = NULL;
            _bss_18[4] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[5] != 0) {
        _bss_18[5] -= gUpdateRate;
        if (_bss_18[5] <= 0) {
            if (_data_18 != NULL) {
                dll_unload(_data_18);
            }
            _data_18 = NULL;
            _bss_18[5] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[16] != 0) {
        _bss_18[16] -= gUpdateRate;
        if (_bss_18[16] <= 0) {
            if (_data_1C != NULL) {
                dll_unload(_data_1C);
            }
            _data_1C = NULL;
            _bss_18[16] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[6] != 0) {
        _bss_18[6] -= gUpdateRate;
        if (_bss_18[6] <= 0) {
            if (_data_20 != NULL) {
                dll_unload(_data_20);
            }
            _data_20 = NULL;
            _bss_18[6] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[7] != 0) {
        _bss_18[7] -= gUpdateRate;
        if (_bss_18[7] <= 0) {
            if (_data_24 != NULL) {
                dll_unload(_data_24);
            }
            _data_24 = NULL;
            _bss_18[7] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[8] != 0) {
        _bss_18[8] -= gUpdateRate;
        if (_bss_18[8] <= 0) {
            if (_data_28 != NULL) {
                dll_unload(_data_28);
            }
            _data_28 = NULL;
            _bss_18[8] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[9] != 0) {
        _bss_18[9] -= gUpdateRate;
        if (_bss_18[9] <= 0) {
            if (_data_2C != NULL) {
                dll_unload(_data_2C);
            }
            _data_2C = NULL;
            _bss_18[9] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[10] != 0) {
        _bss_18[10] -= gUpdateRate;
        if (_bss_18[10] <= 0) {
            if (_data_30 != NULL) {
                dll_unload(_data_30);
            }
            _data_30 = NULL;
            _bss_18[10] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[11] != 0) {
        _bss_18[11] -= gUpdateRate;
        if (_bss_18[11] <= 0) {
            if (_data_34 != NULL) {
                dll_unload(_data_34);
            }
            _data_34 = NULL;
            _bss_18[11] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[12] != 0) {
        _bss_18[12] -= gUpdateRate;
        if (_bss_18[12] <= 0) {
            if (_data_38 != NULL) {
                dll_unload(_data_38);
            }
            _data_38 = NULL;
            _bss_18[12] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[13] != 0) {
        _bss_18[13] -= gUpdateRate;
        if (_bss_18[13] <= 0) {
            if (_data_3C != NULL) {
                dll_unload(_data_3C);
            }
            _data_3C = NULL;
            _bss_18[13] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[14] != 0) {
        _bss_18[14] -= gUpdateRate;
        if (_bss_18[14] <= 0) {
            if (_data_40 != NULL) {
                dll_unload(_data_40);
            }
            _data_40 = NULL;
            _bss_18[14] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[15] != 0) {
        _bss_18[15] -= gUpdateRate;
        if (_bss_18[15] <= 0) {
            if (_data_44 != NULL) {
                dll_unload(_data_44);
            }
            _data_44 = NULL;
            _bss_18[15] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[17] != 0) {
        _bss_18[17] -= gUpdateRate;
        if (_bss_18[17] <= 0) {
            if (_data_48 != NULL) {
                dll_unload(_data_48);
            }
            _data_48 = NULL;
            _bss_18[17] = 0;
            _data_0 -= 1;
        }
    }
    if (_bss_18[18] != 0) {
        _bss_18[18] = (s16) (_bss_18[18] - gUpdateRate);
        if (_bss_18[18] <= 0) {
            if (_data_4C != NULL) {
                dll_unload(_data_4C);
            }
            _data_4C = NULL;
            _bss_18[18] = 0;
            _data_0 -= 1;
        }
    }
}

// offset: 0xD74 | func: 2 | export: 1
typedef struct {
    Object *unk0; // sp6C
    s32 unk4; // sp70;
    s32 unk8; // sp74;
    SRT transform; // sp78;
    Vec3f unk24; // sp90;
    Vec4f unk30; // sp9C;
    s16 unk40; // spAC;
    s16 unk42; // spAE;
    s32 unk44; // spB0;
    s32 unk48; // spB4;
    s32 unk4C[3]; // spB8 - C0;
    u16 unk58[3]; // spC4 - C8;
    s8 unk5E; // spCA;
    u8 unk5F; // spCB;
    u8 unk60; // spCC;
    s8 unk61; // spCD;
    s8 unk62; // spCE;
} D74Stack;

#define RESET_SRT(srt) \
(srt).transl.x = 0.0f;\
(srt).transl.y = 0.0f;\
(srt).transl.z = 0.0f;\
(srt).scale = 1.0f;\
(srt).yaw = 0;\
(srt).pitch = 0;\
(srt).roll = 0;\
(srt).flags = 0;\

s32 dll_17_func_D74(Object* arg0, s32 arg1, SRT* arg2, s32 arg3, s8 arg4, f32* arg5) {
    D74Stack sp6C;
    f32* sp68;
    s32 sp64;
    s16 sp62;
    s16 sp60;
    Vec3f sp54;
    SRT sp3C;
    f32 sp38;
    f32* sp34;
    s32 sp30;

    if (((arg1 >= 0x384) && (arg1 < 0x3B5)) || ((arg1 >= 0x5DD) && (arg1 < 0x641))) {
        _bss_18[0] = 0x7D0;
        if (_data_4 == NULL) {
            _data_0 += 1;
            _data_4 = dll_load_deferred(0x22U, 2U);
        }
        return _data_4->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x258) && (arg1 < 0x2BC)) {
        _bss_18[1] = 0x7D0;
        if (_data_8 == NULL) {
            _data_0 += 1;
            _data_8 = dll_load_deferred(0x23U, 2U);
        }
        return _data_8->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x1F4) && (arg1 < 0x258)) {
        _bss_18[2] = 0x7D0;
        if (_data_C == NULL) {
            _data_0 += 1;
            _data_C = dll_load_deferred(0x24U, 2U);
        }
        return _data_C->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x190) && (arg1 < 0x1F4)) {
        _bss_18[3] = 0x7D0;
        if (_data_10 == NULL) {
            _data_0 += 1;
            _data_10 = dll_load_deferred(0x25U, 2U);
        }
        return _data_10->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0xC8) && (arg1 < 0x12C)) {
        _bss_18[4] = 0x7D0;
        if (_data_14 == NULL) {
            _data_0 += 1;
            _data_14 = dll_load_deferred(0x26U, 2U);
        }
        return _data_14->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x41A) && (arg1 < 0x44C)) {
        _bss_18[5] = 0x7D0;
        if (_data_18 == NULL) {
            _data_0 += 1;
            _data_18 = dll_load_deferred(0x27U, 2U);
        }
        return _data_18->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x73A) && (arg1 < 0x76C)) {
        _bss_18[0x10] = 0x7D0;
        if (_data_1C == NULL) {
            _data_0 += 1;
            _data_1C = dll_load_deferred(0x32U, 2U);
        }
        return _data_1C->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 == 0x84) || (arg1 == 0x85) || ((arg1 >= 0x8A) && (arg1 < 0xC8))) {
        _bss_18[6] = 0x7D0;
        if (_data_20 == NULL) {
            _data_0 += 1;
            _data_20 = dll_load_deferred(0x28U, 2U);
        }
        return _data_20->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x3B6) && (arg1 < 0x3DE)) {
        _bss_18[8] = 0x7D0;
        if (_data_28 == NULL) {
            _data_0 += 1;
            _data_28 = dll_load_deferred(0x2AU, 2U);
        }
        return _data_28->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x352) && (arg1 < 0x384)) {
        _bss_18[7] = 0x7D0;
        if (_data_24 == NULL) {
            _data_0 += 1;
            _data_24 = dll_load_deferred(0x29U, 2U);
        }
        return _data_24->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x32A) && (arg1 < 0x351)) {
        _bss_18[9] = 0x7D0;
        if (_data_2C == NULL) {
            _data_0 += 1;
            _data_2C = dll_load_deferred(0x2BU, 2U);
        }
        return _data_2C->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x12C) && (arg1 < 0x190)) {
        _bss_18[0xA] = 0x7D0;
        if (_data_30 == NULL) {
            _data_0 += 1;
            _data_30 = dll_load_deferred(0x2CU, 2U);
        }
        return _data_30->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x47E) && (arg1 < 0x4B0)) {
        _bss_18[0xB] = 0x7D0;
        if (_data_34 == NULL) {
            _data_0 += 1;
            _data_34 = dll_load_deferred(0x2DU, 2U);
        }
        return _data_34->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x4B0) && (arg1 < 0x4E2)) {
        _bss_18[0xC] = 0x7D0;
        if (_data_38 == NULL) {
            _data_0 += 1;
            _data_38 = dll_load_deferred(0x2FU, 2U);
        }
        return _data_38->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x3E8) && (arg1 < 0x41A)) {
        _bss_18[0xD] = 0x7D0;
        if (_data_3C == NULL) {
            _data_0 += 1;
            _data_3C = dll_load_deferred(0x30U, 2U);
        }
        return _data_3C->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x44C) && (arg1 < 0x47E)) {
        _bss_18[0xE] = 0x7D0;
        if (_data_40 == NULL) {
            _data_0 += 1;
            _data_40 = dll_load_deferred(0x2EU, 2U);
        }
        return _data_40->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x6D7) && (arg1 < 0x708)) {
        _bss_18[0xF] = 0x7D0;
        if (_data_44 == NULL) {
            _data_0 += 1;
            _data_44 = dll_load_deferred(0x31U, 2U);
        }
        return _data_44->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x708) && (arg1 < 0x73A)) {
        _bss_18[0x11] = 0x7D0;
        if (_data_48 == NULL) {
            _data_0 += 1;
            _data_48 = dll_load_deferred(0x33U, 2U);
        }
        return _data_48->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    if ((arg1 >= 0x76C) && (arg1 < 0x79E)) {
        _bss_18[0x12] = 0x7D0;
        if (_data_4C == NULL) {
            _data_0 += 1;
            _data_4C = dll_load_deferred(0x34U, 2U);
        }
        return _data_4C->vtbl->func[1].withSixArgsS32(arg0, arg1, arg2, arg3, (s32) arg4, arg5);
    }
    sp54.z = 0.0f;
    sp54.y = 0.0f;
    sp54.x = 0.0f;
    sp38 = 1.0f;
    _data_90 += 0.001f;
    sp62 = -1;
    sp34 = arg5;
    sp30 = -1;
    if (_data_90 > 1.0f) {
        _data_90 = 0.1f;
    }
    _data_94 += 0.0003f;
    if (_data_94 > 1.0f) {
        _data_94 = 0.3f;
    }
    if (arg0 == NULL) {
        return -1;
    }
    if (arg3 & 0x200000) {
        if (arg2 == NULL) {
            return -1;
        }
        sp6C.transform.transl.x = arg2->transl.f[0];
        sp6C.transform.transl.y = arg2->transl.f[1];
        sp6C.transform.transl.z = arg2->transl.f[2];
        sp6C.transform.scale = arg2->scale;
        sp6C.transform.roll = arg2->roll;
        sp6C.transform.pitch = arg2->pitch;
        sp6C.transform.yaw = arg2->yaw;
        sp6C.unk62 = arg4;
    }
    sp6C.unk5E = arg1;
    sp6C.unk0 = arg0;
    sp6C.unk4 = -1;
    sp6C.unk60 = 0xFF;
    sp6C.unk44 = 0;
    sp6C.unk48 = 0;
    sp6C.unk8 = 0;
    sp6C.unk61 = 0;
    sp6C.unk42 = 0;
    sp6C.unk30.x = 0.0f;
    sp6C.unk30.y = 0.0f;
    sp6C.unk30.z = 0.0f;
    sp6C.unk24.x = 0.0f;
    sp6C.unk24.y = 0.0f;
    sp6C.unk24.z = 0.0f;
    sp6C.unk30.w = 0.0f;
    sp6C.unk58[0] = 0xFFFFU;
    sp6C.unk58[1] = 0xFFFFU;
    sp6C.unk58[2] = 0xFFFFU;
    sp6C.unk4C[0] = 0xFFFF;
    sp6C.unk4C[1] = 0xFFFF;
    sp6C.unk4C[2] = 0xFFFF;
    sp6C.unk40 = 0;

    switch (arg1) {
        case 0x555:
            sp6C.unk24.x = rand_next(-0x1E, 0x1E) * 0.04f;
            sp6C.unk24.y = rand_next(6, 0x16) * 0.12f;
            sp6C.unk24.z = rand_next(-0x1E, 0x1E) * 0.04f;
            sp6C.unk8 = 0x91;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x03000030;
            sp6C.unk48 = 0x600000;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.z = 16.0f;
            sp6C.unk30.w = 0.0062f;
            break;
        case 0x556:
            sp6C.unk8 = 0xAF;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x500010;
            sp6C.unk48 = 0x400200;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.y = 30.0f;
            sp6C.unk30.w = 0.0092f;
            gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            /* fallthrough */
        case 0x55C:
            sp6C.unk8 = 0xAF;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x500010;
            sp6C.unk48 = 0x400100;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.y = 30.0f;
            sp6C.unk30.w = 0.01f;
            gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            /* fallthrough */
        case 0x55D:
            sp6C.unk8 = 0x2D;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x100210;
            sp6C.unk48 = 0x200;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.y = 30.0f;
            sp6C.unk30.w = 0.015f;
            gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            /* fallthrough */
        case 0x557:
            sp6C.unk30.y = 30.0f;
            if (arg5 != NULL) {
                sp6C.unk24.y = 0.2f;
            } else {
                sp6C.unk24.y = -0.2f;
            }
            sp6C.unk8 = 0xAF;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x500010;
            sp6C.unk48 = 0x400200;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.w = 0.0042f;
            gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            /* fallthrough */
        case 0x558:
            sp6C.unk30.y = 30.0f;
            if (arg5 != NULL) {
                sp6C.unk24.y = -0.2f;
            } else {
                sp6C.unk24.y = 0.2f;
            }
            sp6C.unk8 = 0xAF;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x500010;
            sp6C.unk48 = 0x400200;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.w = 0.0042f;
            gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            /* fallthrough */
        case 0x559:
            sp6C.unk30.y = 30.0f;
            if (arg5 != NULL) {
                sp6C.unk24.y = 0.2f;
            } else {
                sp6C.unk24.y = -0.2f;
            }
            sp6C.unk8 = 0xAF;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x500010;
            sp6C.unk48 = 0x400100;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.w = 0.005f;
            gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            /* fallthrough */
        case 0x55B:
            sp6C.unk30.y = 30.0f;
            if (arg5 != NULL) {
                sp6C.unk24.y = -0.2f;
            } else {
                sp6C.unk24.y = 0.2f;
            }
            sp6C.unk8 = 0xAF;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x500010;
            sp6C.unk48 = 0x400100;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.w = 0.005f;
            break;
        case 0x55E:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk30.y = rand_next(-6, 6) + arg2->transl.y;
            sp6C.unk24.x = rand_next(-0x64, 0x64) * 0.01f;
            sp6C.unk24.z = rand_next(-0x64, 0x64) * 0.01f;
            sp6C.unk8 = 0x12;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x400010;
            sp6C.unk48 = 0x400008;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.w = 0.0015f;
            break;
        case 0x551:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk8 = 0x23;
            sp6C.unk60 = 0x9B;
            sp6C.unk44 = 0x100210;
            sp6C.unk42 = 0x91;
            sp6C.unk30.z = -26.0f;
            sp6C.unk30.w = 0.06f;
            break;
        case 0x552:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk8 = 0x23;
            sp6C.unk60 = 0x9B;
            sp6C.unk44 = 0x0A100210;
            sp6C.unk42 = 0x91;
            sp6C.unk30.z = -26.0f;
            sp6C.unk30.w = 0.06f;
            break;
        case 0x554:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk8 = 0x37;
            sp6C.unk60 = 0x9B;
            sp6C.unk44 = 0x0A100210;
            sp6C.unk42 = 0x73;
            sp6C.unk30.z = -26.0f;
            sp6C.unk30.w = 0.16f;
            break;
        case 0x553:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk24.x = rand_next(-0x1E, 0x1E) * 0.02f;
            sp6C.unk24.y = rand_next(0x14, 0x1E) * 0.06f;
            sp6C.unk24.z = rand_next(-0x1E, 0x1E) * 0.02f;
            sp3C.roll = 0;
            sp3C.pitch = 0;
            sp6C.unk30.z = -26.0f;
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 1.0f;
            sp3C.yaw = arg0->srt.yaw;
            rotate_vec3(&sp3C, sp6C.unk30.f);
            sp6C.unk8 = 0x91;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x03000010;
            sp6C.unk48 = 0x02600000;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.w = 0.0032f;
            break;
        case 0x549:
            sp6C.unk30.x = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.y = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.z = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0001f;
            sp6C.unk8 = rand_next(0x64, 0x96);
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x80480110;
            if (arg5 != NULL) {
                sp6C.unk44 = 0xC0480110;
            }
            sp6C.unk42 = 0x85;
            break;
        case 0x54A:
            sp6C.unk30.x = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.y = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.z = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0001f;
            sp6C.unk8 = rand_next(0x64, 0x96);
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x80480110;
            if (arg5 != NULL) {
                sp6C.unk44 = 0xC0480110;
            }
            sp6C.unk42 = 0x84;
            break;
        case 0x54B:
            sp6C.unk30.x = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.y = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.z = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0001f;
            sp6C.unk8 = rand_next(0x64, 0x96);
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x80480110;
            if (arg5 != NULL) {
                sp6C.unk44 = 0xC0480110;
            }
            sp6C.unk42 = 0x32;
            break;
        case 0x54C:
            sp6C.unk30.x = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.y = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.z = rand_next(-0xA, 0xA) * 0.2f;
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0001f;
            sp6C.unk8 = rand_next(0x64, 0x96);
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x80480110;
            if (arg5 != NULL) {
                sp6C.unk44 = 0xC0480110;
            }
            sp6C.unk42 = 0x157;
            break;
        case 0x54D:
            if (arg5 != NULL) {
                sp64 = ((u8*) arg5)[0];
            } else {
                sp64 = 0;
            }
            if (sp64 == 1) {
                sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0001f;
                sp6C.unk44 = 0x4C0800;
                sp6C.unk48 = 0x202;
            } else if (sp64 == 2) {
                sp6C.unk30.w = rand_next(0xA, 0x14) * 0.00015f;
                sp6C.unk44 = 0x4C0800;
                sp6C.unk48 = 0x102;
            } else {
                sp6C.unk30.w = rand_next(0x12, 0x14) * 0.0006f;
                sp6C.unk44 = 0xC0800;
                sp6C.unk48 = 2;
            }
            sp6C.unk8 = 1;
            sp6C.unk60 = 0x60;
            sp6C.unk42 = 0x85;
            break;
        case 0x54E:
            if (arg5 != NULL) {
                sp64 = ((u8*) arg5)[0];
            } else {
                sp64 = 0;
            }
            if (sp64 == 1) {
                sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0001f;
                sp6C.unk44 = 0x4C0800;
                sp6C.unk48 = 0x202;
            } else if (sp64 == 2) {
                sp6C.unk30.w = rand_next(0xA, 0x14) * 0.00015f;
                sp6C.unk44 = 0x4C0800;
                sp6C.unk48 = 0x102;
            } else {
                sp6C.unk30.w = rand_next(0x12, 0x14) * 0.0006f;
                sp6C.unk44 = 0xC0800;
                sp6C.unk48 = 2;
            }
            sp6C.unk8 = 1;
            sp6C.unk60 = 0x60;
            sp6C.unk42 = 0x84;
            break;
        case 0x54F:
            if (arg5 != NULL) {
                sp64 = ((u8*) arg5)[0];
            } else {
                sp64 = 0;
            }
            if (sp64 == 1) {
                sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0001f;
                sp6C.unk44 = 0x4C0800;
                sp6C.unk48 = 0x202;
            } else if (sp64 == 2) {
                sp6C.unk30.w = rand_next(0xA, 0x14) * 0.00015f;
                sp6C.unk44 = 0x4C0800;
                sp6C.unk48 = 0x102;
            } else {
                sp6C.unk30.w = rand_next(0x12, 0x14) * 0.0006f;
                sp6C.unk44 = 0xC0800;
                sp6C.unk48 = 2;
            }
            sp6C.unk8 = 1;
            sp6C.unk60 = 0x60;
            sp6C.unk42 = 0x32;
            break;
        case 0x550:
            if (arg5 != NULL) {
                sp64 = ((u8*) arg5)[0];
            } else {
                sp64 = 0;
            }
            if (sp64 == 1) {
                sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0001f;
                sp6C.unk44 = 0x4C0800;
                sp6C.unk48 = 0x202;
            } else if (sp64 == 2) {
                sp6C.unk30.w = rand_next(0xA, 0x14) * 0.00015f;
                sp6C.unk44 = 0x4C0800;
                sp6C.unk48 = 0x102;
            } else {
                sp6C.unk30.w = rand_next(0x12, 0x14) * 0.0006f;
                sp6C.unk44 = 0xC0800;
                sp6C.unk48 = 2;
            }
            sp6C.unk8 = 1;
            sp6C.unk60 = 0x60;
            sp6C.unk42 = 0x157;
            break;
        case 0x545:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk30.w = arg2->scale * 0.5f;
            sp6C.unk8 = 4;
            sp6C.unk44 = 0x480000;
            sp6C.unk48 = 2;
            sp6C.unk42 = 0x527;
            sp6C.unk60 = 0x69;
            break;
        case 0x546:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk30.w = arg2->scale * 0.65f;
            sp6C.unk8 = 4;
            sp6C.unk44 = 0x480000;
            sp6C.unk48 = 0x02000002;
            sp6C.unk42 = 0x158;
            sp6C.unk60 = 0x73;
            break;
        case 0x547:
            sp6C.unk30.x = 60.0f;
            sp6C.unk30.y = rand_next(-0x50, 0x50);
            sp6C.unk24.y = rand_next(-0x64, 0x64) * 0.01f;
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk30.w = 0.0065f;
            sp6C.unk8 = 0x12C;
            sp6C.unk44 = 0x480000;
            sp6C.unk48 = 0x02000000;
            sp6C.unk42 = 0x158;
            sp6C.unk60 = 0xFF;
            sp6C.unk4 = 0x548;
            sp6C.transform.yaw = 0xC8;
            sp6C.transform.pitch = 0;
            sp6C.transform.yaw = 0;
            sp6C.transform.transl.x = 100.0f;
            sp6C.transform.transl.y = 0.0f;
            sp6C.transform.transl.z = 0.0f;
            sp6C.transform.scale = 1.0f;
            sp6C.unk8 = rand_next(0, 0x14) + 0x28;
            sp6C.unk61 = 0x10;
            sp6C.unk44 |= 0x20000;
            break;
        case 0x548:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk8 = 0x50;
            sp6C.unk44 = 0x80201;
            sp6C.unk48 = 0x02000000;
            sp6C.unk42 = 0x158;
            sp6C.unk60 = 0xFF;
            sp6C.unk30.w = 0.0055f;
            break;
        case 0x52B:
        case 0x52C:
        case 0x52D:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 != NULL) {
                sp6C.unk30.x = arg2->transl.f[0];
                sp6C.unk30.y = arg2->transl.f[1];
                sp6C.unk30.z = arg2->transl.f[2];
                sp6C.unk30.x -= sp6C.unk0->positionMirror.x;
                sp6C.unk30.y -= sp6C.unk0->positionMirror.y;
                sp6C.unk30.z -= sp6C.unk0->positionMirror.z;
            }
            if (rand_next(0, 0x28) == 0) {
                sp6C.unk30.w = 0.0003f;
            } else {
                sp6C.unk30.w = 0.0015f;
            }
            sp6C.unk8 = 0x14;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x80210;
            sp6C.unk42 = arg1 - 0x3D5;
            break;
        case 0x52F:
        case 0x530:
        case 0x531:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 != NULL) {
                sp6C.unk30.x = arg2->transl.f[0];
                sp6C.unk30.y = arg2->transl.f[1];
                sp6C.unk30.z = arg2->transl.f[2];
                sp6C.unk30.x -= sp6C.unk0->positionMirror.x;
                sp6C.unk30.y -= sp6C.unk0->positionMirror.y;
                sp6C.unk30.z -= sp6C.unk0->positionMirror.z;
                sp6C.unk24.z = 0.3f;
            }
            sp6C.unk8 = 0x64;
            sp6C.unk30.w = 0.0015f;
            break;
        case 0x53C:
            if (arg5 != NULL) {
                sp6C.unk60 = (s16) ((1.0f - arg5[0]) * 255.0f);
                diPrintf("alpha %d\n", sp6C.unk60);
            }
            sp6C.unk44 = 0x80000;
            sp6C.unk48 = 0x02000002;
            sp6C.unk8 = 0;
            sp6C.unk42 = 0xE4;
            sp6C.unk30.w = 4.0f;
            break;
        case 0x53D:
            sp6C.unk60 = 0x69;
            sp6C.unk44 = 0x80014;
            sp6C.unk48 = 0x22;
            sp6C.unk8 = 0;
            sp6C.unk42 = 0x4FE;
            sp6C.unk30.z = 0.0f;
            sp6C.unk30.w = 0.03f;
            sp6C.unk58[0] = 0xB1DFU;
            sp6C.unk58[1] = 0xB1DFU;
            sp6C.unk58[2] = 0xFFFFU;
            sp6C.unk4C[0] = sp6C.unk58[0];
            sp6C.unk4C[1] = sp6C.unk58[1];
            sp6C.unk4C[2] = sp6C.unk58[2];
            sp62 = gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            sp6C.unk60 = 0x69;
            sp6C.unk44 = 0x80014;
            sp6C.unk48 = 0x22;
            sp6C.unk30.z = -200.0f;
            sp6C.unk30.w = 0.036f;
            sp6C.unk58[0] = 0xFFFFU;
            sp6C.unk58[1] = 0xB1DFU;
            sp6C.unk58[2] = 0xFFFFU;
            sp6C.unk4C[0] = sp6C.unk58[0];
            sp6C.unk4C[1] = sp6C.unk58[1];
            sp6C.unk4C[2] = sp6C.unk58[2];
            sp6C.unk8 = 0;
            sp6C.unk42 = 0x4FF;
            sp62 = gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            sp6C.unk60 = 0x69;
            sp6C.unk44 = 0x80014;
            sp6C.unk48 = 0x22;
            sp6C.unk30.z = -400.0f;
            sp6C.unk30.w = 0.042f;
            sp6C.unk58[0] = 0xB1DFU;
            sp6C.unk58[1] = 0xFFFFU;
            sp6C.unk58[2] = 0xFFFFU;
            sp6C.unk4C[0] = sp6C.unk58[0];
            sp6C.unk4C[1] = sp6C.unk58[1];
            sp6C.unk4C[2] = sp6C.unk58[2];
            sp6C.unk8 = 0;
            sp6C.unk42 = 0x4FE;
            break;
        case 0x53E:
            sp6C.unk44 = 0x80010;
            sp6C.unk48 = 2;
            sp6C.unk8 = 1;
            sp6C.unk42 = 0x64;
            sp6C.unk30.x = -20.0f;
            sp6C.unk30.w = 0.2f;
            break;
        case 0x53F:
            sp6C.unk60 = 0x37;
            sp6C.unk44 = 0x80010;
            sp6C.unk48 = 2;
            sp6C.unk8 = 1;
            sp6C.unk42 = 0x156;
            sp6C.unk30.w = 0.1f;
            break;
        case 0x532:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk24.x = rand_next(-0x1E, 0x1E) * 0.002f;
            sp6C.unk24.y = rand_next(-0x1E, 0x1E) * 0.002f;
            sp6C.unk24.z = rand_next(0x14, 0x1E) * -0.026f;
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 1.0f;
            sp3C.roll = arg0->srt.roll;
            sp3C.pitch = arg0->srt.pitch;
            sp3C.yaw = arg0->srt.yaw;
            rotate_vec3(&sp3C, sp6C.unk24.f);
            sp6C.unk60 = 0xCD;
            sp6C.unk44 = 0x100110;
            sp6C.unk30.w = rand_next(0x96, 0xC8) * 0.00003f;
            sp6C.unk8 = 0x28;
            sp6C.unk42 = 0x89;
            break;
        case 0x533:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk24.x = rand_next(-0x1E, 0x1E) * 0.002f;
            sp6C.unk24.y = rand_next(8, 0xA) * 0.01f;
            sp6C.unk24.z = rand_next(0xA, 0x1E) * -0.005f;
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 1.0f;
            sp3C.roll = arg0->srt.roll;
            sp3C.pitch = arg0->srt.pitch;
            sp3C.yaw = arg0->srt.yaw;
            rotate_vec3(&sp3C, sp6C.unk24.f);
            sp6C.unk30.w = rand_next(8, 0x14) * 0.0003f;
            sp6C.unk8 = rand_next(0x3C, 0x78);
            sp6C.unk44 = 0x80180000;
            sp6C.unk48 = 0x01400020;
            sp6C.unk42 = 0x162;
            sp6C.unk60 = 0x7F;
            sp6C.unk58[0] = 0xFFFFU;
            sp6C.unk58[1] = 0xFFFFU;
            sp6C.unk58[2] = 0xFFFFU;
            sp6C.unk4C[0] = 0x3CAF;
            sp6C.unk4C[1] = 0x3CAF;
            sp6C.unk4C[2] = 0x3CAF;
            break;
        case 0x535:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk24.x = rand_next(-0x1E, 0x1E) * 0.01f;
            sp6C.unk24.y = rand_next(-0x1E, 0x1E) * 0.01f;
            sp6C.unk24.z = rand_next(0x14, 0x1E) * -0.02f;
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 1.0f;
            sp3C.roll = arg0->srt.roll;
            sp3C.pitch = arg0->srt.pitch;
            sp3C.yaw = arg0->srt.yaw;
            rotate_vec3(&sp3C, sp6C.unk24.f);
            sp6C.unk60 = 0xFF;
            sp6C.unk30.w = rand_next(0x96, 0xC8) * 0.000006f;
            sp6C.unk44 = 0x02000110;
            sp6C.unk48 = 0x02200000;
            sp6C.unk8 = 0x19;
            sp6C.unk42 = 0x24;
            break;
        case 0x534:
            sp6C.unk30.y = 10.0f;
            sp6C.unk24.x = rand_next(-0xF, 0xF) * 0.02f;
            sp6C.unk24.y = rand_next(-0xF, 0xF) * 0.02f;
            sp6C.unk24.z = -1.0f;
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 1.0f;
            sp3C.roll = arg0->srt.roll;
            sp3C.pitch = arg0->srt.pitch;
            sp3C.yaw = arg0->srt.yaw;
            rotate_vec3(&sp3C, sp6C.unk24.f);
            sp6C.unk60 = 0xFF;
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.00004f;
            sp6C.unk44 = 0x02000110;
            sp6C.unk48 = 0x200000;
            sp6C.unk8 = 0x19;
            sp6C.unk42 = 0x156;
            break;
        case 0x52A:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk8 = 0xA;
            sp6C.unk60 = 0xFF;
            sp6C.unk61 = 0x10;
            sp6C.unk44 = 0x80440202;
            sp6C.unk42 = 0x4F9;
            sp6C.unk48 = 0x02000000;
            sp6C.unk30.w = 0.01465f;
            break;
        case 0x86:
            sp6C.unk8 = 0xA;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x100111;
            sp6C.unk42 = 0xDC;
            sp6C.unk61 = 0x20;
            sp6C.unk30.w = 0.015f;
            break;
        case 0x87:
            sp6C.unk30.x = rand_next(-0x28, 0x28) * 0.1f;
            sp6C.unk30.y = rand_next(-0x28, 0x28) * 0.1f;
            sp6C.unk30.z = rand_next(-0x28, 0x28) * 0.1f;
            sp6C.unk30.w = rand_next(1, 3) * 0.005f;
            sp6C.unk8 = rand_next(0, 0x28) + 0x28;
            sp6C.unk44 = 0x100111;
            sp6C.unk42 = 0x16E;
            sp6C.unk60 = 0xFF;
            sp6C.unk61 = 0x20;
            if (sp6C.unk0 != NULL) {
                sp6C.unk30.x += sp6C.unk0->srt.transl.f[0];
                sp6C.unk30.y += sp6C.unk0->srt.transl.f[1];
                sp6C.unk30.z += sp6C.unk0->srt.transl.f[2];
            } else {
                sp6C.unk30.x += sp6C.transform.transl.x;
                sp6C.unk30.y += sp6C.transform.transl.y;
                sp6C.unk30.z += sp6C.transform.transl.z;
            }
            sp62 = gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            return sp62;
        case 0x88:
            sp6C.unk8 = 0x1F4;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x100211;
            sp6C.unk42 = 0xDC;
            sp6C.unk60 = 0xFF;
            sp6C.unk61 = 0x20;
            sp6C.unk30.w = 0.03f;
            break;
        case 0x528:
            sp6C.unk60 = 0xFF;
            sp6C.unk8 = 0x32;
            sp6C.unk44 = 0x180200;
            sp6C.unk42 = 0x73;
            sp6C.unk30.w = 0.1f;
            break;
        case 0x527:
            sp6C.unk60 = 0xFF;
            sp6C.unk30.w = 0.015f;
            sp6C.unk8 = rand_next(0, 0xA) + 0x14;
            sp6C.unk44 = 0x180200;
            sp6C.unk42 = 0x73;
            break;
        case 0x526:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk24.x = rand_next(-0x1E, 0x1E) * 0.01f;
            sp6C.unk24.x = rand_next(-0x1E, 0x1E) * 0.01f;
            sp6C.unk24.y = rand_next(0x14, 0x1E) * -0.2f;
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 1.0f;
            sp3C.roll = arg0->srt.roll;
            if (arg2 != NULL) {
                sp3C.pitch = arg2->pitch + arg0->srt.pitch;
            } else {
                sp3C.pitch = arg0->srt.pitch;
            }
            sp3C.yaw = arg0->srt.yaw;
            rotate_vec3(&sp3C, sp6C.unk24.f);
            sp6C.unk60 = 0x9B;
            sp6C.unk44 = 0x100;
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0011f;
            sp6C.unk8 = 0x1E;
            sp6C.unk42 = 0x60;
            break;
        case 0x529:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk24.x = rand_next(-0x1E, 0x1E) * 0.01f;
            sp6C.unk24.x = rand_next(-0x1E, 0x1E) * 0.01f;
            sp6C.unk24.y = rand_next(0x14, 0x1E) * -0.2f;
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 1.0f;
            sp3C.roll = arg0->srt.roll;
            if (arg2 != NULL) {
                sp3C.pitch = arg2->pitch + arg0->srt.pitch;
            } else {
                sp3C.pitch = arg0->srt.pitch;
            }
            sp3C.yaw = arg0->srt.yaw;
            rotate_vec3(&sp3C, sp6C.unk24.f);
            sp6C.unk60 = 0x9B;
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0011f;
            sp6C.unk44 = 0x100;
            sp6C.unk8 = 0x1E;
            sp6C.unk42 = 0x158;
            break;
        case 0x525:
            sp6C.unk24.x = rand_next(-0xF, 0xF) * 0.1f;
            sp6C.unk24.z = rand_next(-0xF, 0xF) * 0.1f;
            sp6C.unk24.y = -5.0f;
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 1.0f;
            sp3C.roll = arg0->srt.roll;
            sp3C.pitch = arg0->srt.pitch;
            sp3C.yaw = arg0->srt.yaw;
            rotate_vec3(&sp3C, sp6C.unk24.f);
            sp6C.unk60 = 0xFF;
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.000288f;
            sp6C.unk44 = 0x02000100;
            sp6C.unk48 = 0x200000;
            sp6C.unk8 = 0x19;
            sp6C.unk42 = 0x156;
            break;
        case 0x524:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk24.x = rand_next(-0x1E, 0x1E) * 0.3f;
            sp6C.unk24.z = rand_next(-0x1E, 0x1E) * 0.3f;
            sp6C.unk24.z = rand_next(-0x1E, 0x1E) * 0.3f;
            sp6C.unk60 = 0xFF;
            if (arg2 != NULL) {
                sp6C.unk30.x += arg2->transl.f[0];
                sp6C.unk30.y += arg2->transl.f[1];
                sp6C.unk30.z += arg2->transl.f[2];
            }
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.00088f;
            sp6C.unk8 = 0xFF;
            sp6C.unk58[0] = 0xFFFFU;
            sp6C.unk58[1] = (u16) (rand_next(0, 0x2710) + 0xB1DF);
            sp6C.unk58[2] = (u16) (rand_next(0, 0x2710) + 0x63BF);
            sp6C.unk4C[0] = 0xC350;
            sp6C.unk4C[1] = rand_next(0, 0x7530);
            sp6C.unk4C[2] = rand_next(0, 0x7530);
            sp6C.unk48 = 0x200020;
            sp6C.unk42 = 0x60;
            break;
        case 0x523:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 != NULL) {
                sp6C.unk30.x += arg2->transl.f[0];
                sp6C.unk30.y += arg2->transl.f[1];
                sp6C.unk30.z += arg2->transl.f[2];
            }
            sp6C.unk24.x = rand_next(-0xA, 0xA) * 0.03f;
            sp6C.unk24.z = rand_next(-0xA, 0xA) * 0.03f;
            sp6C.unk24.y = rand_next(-0xA, 0xA) * 0.03f;
            sp6C.unk8 = 0x14;
            sp6C.unk44 = 0x480000;
            sp6C.unk42 = 0x167;
            sp6C.unk60 = 0xFF;
            sp6C.unk30.w = 0.00028f;
            break;
        case 0x522:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk30.x = rand_next(-0xA, -0xA) * 0.1f;
            sp6C.unk30.y = rand_next(-0x14, -0xA) * 0.1f;
            sp6C.unk30.z = rand_next(-0xA, 0xA) * 0.1f;
            sp6C.unk24.x = rand_next(-0xA, 0xA) * 0.003f;
            sp6C.unk24.z = rand_next(-0xA, 0xA) * 0.003f;
            sp6C.unk60 = 0xFF;
            if (arg2 != NULL) {
                sp6C.unk30.x += arg2->transl.f[0];
                sp6C.unk30.y += arg2->transl.f[1];
                sp6C.unk30.z += arg2->transl.f[2];
            }
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.00088f;
            sp6C.unk8 = 0x55;
            sp6C.unk44 = 0x100200;
            sp6C.unk42 = 0x60;
            sp6C.unk58[0] = 0x7FFFU;
            sp6C.unk58[1] = 0x7FFFU;
            sp6C.unk58[2] = 0x7FFFU;
            sp6C.unk4C[0] = rand_next(0, 0xA) * 0xACF;
            sp6C.unk4C[1] = sp6C.unk4C[0];
            sp6C.unk4C[2] = sp6C.unk4C[0];
            sp6C.unk48 = 0xA0;
            break;
        case 0x521:
            if (arg2 != NULL) {
                sp6C.unk30.x += arg2->transl.f[0];
                sp6C.unk30.y += arg2->transl.f[1];
                sp6C.unk30.z += arg2->transl.f[2];
            }
            sp6C.unk8 = 0x14;
            sp6C.unk44 = 0x80200;
            sp6C.unk42 = 0x167;
            sp6C.unk60 = 0xFF;
            sp6C.unk30.w = 0.0028f;
            break;
        case 0x520:
            sp6C.unk30.x = rand_next(-0x1E, 0x1E) * 0.1f;
            sp6C.unk30.z = rand_next(-0x1E, 0x1E) * 0.1f;
            sp6C.unk24.y = rand_next(0x19, 0x23) * 0.005f;
            sp6C.unk30.w = rand_next(0x64, 0x96) * 0.00005f;
            sp6C.unk60 = 0x55;
            sp6C.unk8 = rand_next(0x28, 0x50);
            sp6C.unk44 = 0x80100100;
            sp6C.unk42 = 0x62;
            break;
        case 0x51F:
            sp6C.unk8 = 0x1E;
            sp6C.unk60 = 0xFF;
            sp6C.unk61 = 0x10;
            sp6C.unk44 = 0x88140210;
            sp6C.unk42 = 0x159;
            sp6C.unk30.y = 3.0f;
            sp6C.unk30.w = 0.015565f;
            break;
        case 0x51E:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk8 = 0xA;
            sp6C.unk60 = 0xFF;
            sp6C.unk61 = 0x10;
            sp6C.unk44 = 0x80440202;
            sp6C.unk42 = 0x156;
            sp6C.unk30.w = 0.005565f;
            break;
        case 0x53A:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk8 = rand_next(0xA, 0x3C);
            sp6C.unk24.x = rand_next(-0x14, 0x14) * 0.03f;
            sp6C.unk24.y = sp6C.unk8 * 0.02f;
            sp6C.unk24.z = rand_next(-0x14, 0x14) * 0.03f;
            sp6C.unk30.w = 0.0015f;
            if (arg2 != NULL) {
                sp6C.unk30.w = arg2->scale * 0.002f;
            }
            sp6C.unk44 = 0x03000000;
            sp6C.unk48 = 0x600020;
            sp6C.unk42 = 0x33;
            sp6C.unk60 = 0xFF;
            sp6C.unk58[0] = 0xFFFFU;
            sp6C.unk58[1] = 0xFFFFU;
            sp6C.unk58[2] = 0xFFFFU;
            sp6C.unk4C[0] = 0xFFFF;
            sp6C.unk4C[1] = sp6C.unk4C[2] = rand_next(0, 0x8000);
            break;
        case 0x51C:
            sp6C.unk30.x = rand_next(-0x1E, 0x1E) * 0.1f;
            sp6C.unk30.y = 7.0f;
            sp6C.unk30.z = rand_next(-0x1E, 0x1E) * 0.1f;
            sp6C.unk24.y = rand_next(0x19, 0x23) * 0.005f;
            sp6C.unk30.w = rand_next(0x64, 0x96) * 0.00005f;
            sp6C.unk8 = rand_next(0x5A, 0x78);
            sp6C.unk44 = 0x80100100;
            sp6C.unk42 = 0x60;
            sp6C.unk58[0] = 0x7FFFU;
            sp6C.unk58[1] = 0x7FFFU;
            sp6C.unk58[2] = 0x7FFFU;
            sp6C.unk4C[0] = rand_next(0, 0xA) * 0xACF;
            sp6C.unk4C[1] = sp6C.unk4C[0];
            sp6C.unk4C[2] = sp6C.unk4C[0];
            sp6C.unk48 = 0x20;
            break;
        case 0x51B:
            sp6C.unk30.w = (rand_next(0, 0xF) * 0.002f) + 0.03f;
            sp6C.unk30.x = rand_next(-0x32, 0x32) * 0.1f;
            sp6C.unk30.y = (rand_next(-0x32, 0x32) * 0.1f) + 10.0f;
            sp6C.unk30.z = rand_next(-0x32, 0x32) * 0.1f;
            sp6C.unk24.x = sp6C.unk30.x / 25.0f;
            sp6C.unk24.y = sp6C.unk30.y / 25.0f;
            sp6C.unk24.z = sp6C.unk30.z / 25.0f;
            sp6C.unk8 = rand_next(0, 0x14) + 0x14;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x100110;
            sp6C.unk42 = 0xE4;
            break;
        case 0x51A:
            sp6C.unk30.w = (rand_next(0, 0xF) * 0.001f) + 0.005f;
            sp6C.unk8 = 0x3C;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x08100110;
            if (arg2 != NULL) {
                sp6C.unk42 = arg2->scale;
            } else {
                sp6C.unk42 = 0x26C;
            }
            break;
        case 0x519:
            sp6C.unk24.x = rand_next(-0x64, 0x64) * 0.0025f;
            sp6C.unk24.z = rand_next(-0x64, 0x64) * 0.0025f;
            sp6C.unk8 = 0x3C;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x80480110;
            sp6C.unk42 = 0x33;
            sp6C.unk30.w = 0.001f;
            break;
        case 0x514:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.transform.yaw = 0x3E8;
            sp6C.transform.pitch = 0x3E8;
            sp6C.transform.roll = 0x3E8;
            sp6C.transform.transl.x = 0.0f;
            sp6C.transform.transl.y = 0.0f;
            sp6C.transform.transl.z = 0.0f;
            if (arg2 != NULL) {
                sp6C.unk30.x = arg2->transl.f[0];
                sp6C.unk30.y = arg2->transl.f[1];
                sp6C.unk30.z = arg2->transl.f[2];
            }
            sp6C.unk8 = 6;
            sp6C.unk60 = 0xE1;
            sp6C.unk44 = 0x4A0010;
            if (rand_next(0, 1) != 0) {
                sp6C.unk48 = 0x202;
            } else {
                sp6C.unk48 = 0x102;
            }
            if ((arg2 != NULL) && (arg2->scale == 0.0f)) {
                sp6C.unk30.w = (rand_next(0, 3) * 0.001f) + 0.003f;
                sp6C.unk42 = 0x33;
            } else if ((arg2 != NULL) && (arg2->scale == 2.0f)) {
                sp6C.unk30.w = (rand_next(0, 3) * 0.001f) + 0.003f;
                sp6C.unk42 = 0x4F9;
            } else {
                sp6C.unk30.w = (rand_next(0, 3) * 0.001f) + 0.004f;
                sp6C.unk42 = 0x3F4;
            }
            break;
        case 0x515:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 != NULL) {
                sp6C.unk30.x = arg2->transl.f[0];
                sp6C.unk30.y = arg2->transl.f[1];
                sp6C.unk30.z = arg2->transl.f[2];
            }
            sp6C.unk8 = 0x3C;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x480010;
            if (arg2->scale == 0.0f) {
                sp6C.unk42 = 0x88;
                sp6C.unk30.w = 0.009f;
            } else {
                sp6C.unk42 = 0x88;
                sp6C.unk30.w = 0.012f;
            }
            sp6C.unk48 = 2;
            break;
        case 0x516:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 != NULL) {
                sp6C.unk30.x = arg2->transl.f[0];
                sp6C.unk30.y = arg2->transl.f[1];
                sp6C.unk30.z = arg2->transl.f[2];
            }
            sp6C.unk8 = 0x3C;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x480010;
            sp6C.unk48 = 2;
            if (arg2->scale == 0.0f) {
                sp6C.unk42 = 0x5C;
                sp6C.unk30.w = 0.027f;
            } else {
                sp6C.unk42 = 0x5C;
                sp6C.unk30.w = 0.036f;
            }
            break;
        case 0x517:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk8 = 0x1E;
            sp6C.unk61 = 0;
            sp6C.unk60 = 0x37;
            sp6C.unk44 = 0x180010;
            if ((arg2 != NULL) && (arg2->scale == 0.0f)) {
                sp6C.unk30.w = rand_next(0x14, 0x32) * 0.0004f;
                sp6C.unk42 = 0x73;
            } else {
                sp6C.unk30.w = rand_next(0x14, 0x32) * 0.00053f;
                sp6C.unk42 = 0x72;
            }
            break;
        case 0x518:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk8 = 0x3C;
            sp6C.unk61 = 0;
            sp6C.unk60 = 0x37;
            sp6C.unk44 = 0x480010;
            sp6C.unk48 = 2;
            if (arg2->scale == 0.0f) {
                sp6C.unk30.w = rand_next(0x46, 0x50) * 0.0003f;
                sp6C.unk42 = 0x73;
            } else if (arg2->scale == 2.0f) {
                sp6C.unk30.w = rand_next(0x46, 0x50) * 0.0003f;
                sp6C.unk42 = 0x77;
            } else {
                sp6C.unk30.w = rand_next(0x46, 0x50) * 0.0004f;
                sp6C.unk42 = 0x72;
            }
            break;
        case 0x2BC:
        case 0x2BD:
        case 0x2BE:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 != NULL) {
                sp6C.unk30.x = arg2->transl.x;
                sp6C.unk30.y = arg2->transl.y;
                sp6C.unk30.z = arg2->transl.z;
                sp6C.unk30.x -= sp6C.unk0->positionMirror.x;
                sp6C.unk30.y -= sp6C.unk0->positionMirror.y;
                sp6C.unk30.z -= sp6C.unk0->positionMirror.z;
            }
            sp6C.unk8 = 0x14;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x80210;
            sp6C.unk48 = 0x100;
            sp6C.unk42 = arg1 - 0x28C;
            sp6C.unk30.w = 0.0003148f;
            break;
        case 0x4B:
            sp6C.unk8 = 0x14;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x80100;
            sp6C.unk42 = 0xDF;
            sp6C.unk24.x = 0.0f;
            sp6C.unk24.y = 0.0f;
            sp6C.unk24.z = 0.0f;
            sp6C.unk30.w = 0.0022f;
            break;
        case 0x3C:
            sp6C.unk30.y = 22.0f;
            sp6C.unk30.w = (rand_next(1, 0xA) * 0.003f) + 0.03f;
            sp6C.unk60 = 0xFF;
            sp6C.transform.yaw = rand_next(0, 0xFFFF);
            sp6C.transform.pitch = rand_next(0, 0xFFFF);
            sp6C.transform.yaw = rand_next(0, 0xFFFF);
            sp6C.transform.transl.x = 0.0f;
            sp6C.transform.transl.y = 0.0f;
            sp6C.transform.transl.z = 0.0f;
            sp6C.unk8 = rand_next(0, 0x14) + 0x28;
            sp6C.unk61 = 0x10;
            sp6C.unk44 = 0x06100214;
            sp6C.unk42 = 0x206;
            break;
        case 0x329:
            sp6C.unk30.x = rand_next(-0x64, 0x64) * 0.1f;
            sp6C.unk30.y = 50.0f;
            sp6C.unk30.z = rand_next(-0x64, 0x64) * 0.1f;
            sp6C.unk24.x = rand_next(0x64, 0xC8) * 0.001f;
            sp6C.unk24.y = rand_next(0x64, 0xC8) * 0.001f;
            sp6C.unk24.z = rand_next(-0x64, 0x64) * 0.001f;
            sp6C.unk44 = 0x01081010;
            if (rand_next(0, 3) == 0) {
                sp6C.unk30.w = rand_next(0x28, 0x50) * 0.000065f;
                sp6C.unk60 = 0x8C;
            } else {
                sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00135f;
                sp6C.unk60 = 0xA;
                sp6C.unk44 |= 0x100000;
            }
            if (rand_next(0, 0xA) == 0) {
                arg3 ^= 4;
                arg3 |= 1;
            }
            sp6C.unk8 = 0xDC;
            sp6C.unk58[0] = 0xB1DFU;
            sp6C.unk58[1] = 0x8ACFU;
            sp6C.unk58[2] = 0x63BFU;
            sp6C.unk4C[0] = 0x3CAF;
            sp6C.unk4C[1] = 0x30F7;
            sp6C.unk4C[2] = 0x2710;
            sp6C.unk48 = 0x20;
            sp6C.unk48 = 0x100020;
            sp6C.unk42 = 0x60;
            break;
        case 0x3B9:
            sp6C.unk24.x = rand_next(-0x14, 0x14) * 0.01f;
            sp6C.unk24.z = rand_next(-0x14, 0x14) * 0.01f;
            sp6C.unk30.x = rand_next(-0x32, 0x32);
            sp6C.unk30.z = rand_next(-0x32, 0x32);
            sp6C.unk30.y = rand_next(0x1E, 0x64);
            sp6C.unk8 = 0x4B0;
            sp6C.unk60 = 0xC8;
            sp6C.unk44 = 0x180100;
            sp6C.unk42 = 0x62;
            sp6C.unk30.w = 0.1f;
            break;
        case 0x3B8:
            sp6C.unk30.x = (0x3C - rand_next(0, 0x78)) * 0.25f;
            sp6C.unk30.y = 10.0f;
            sp6C.unk30.z = (0x3C - rand_next(0, 0x78)) * 0.25f;
            sp6C.unk24.x = (0x28 - rand_next(0, 0x50)) * 0.005f;
            sp6C.unk24.z = (0x28 - rand_next(0, 0x50)) * 0.005f;
            sp6C.unk24.y = rand_next(0x28, 0x50) * 0.005f;
            sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00005f;
            sp6C.unk8 = 0xB4;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x80400201;
            sp6C.unk42 = 0x47;
            break;
        case 0x1:
            sp6C.unk30.y = 80.0f;
            sp6C.unk24.x = rand_next(-0xF, 0xF) * (_data_98 * 0.002f);
            sp6C.unk24.y = rand_next(5, 0x14) * 0.003f;
            sp6C.unk24.z = rand_next(-0xF, 0xF) * (_data_98 * 0.002f);
            sp6C.unk30.w = (rand_next(0, 0xA) * 0.0001f) + 0.003f;
            sp6C.unk60 = 0xFF;
            sp6C.unk61 = 0xF;
            sp6C.unk44 = 0x588008;
            sp6C.unk48 = 0x10000;
            sp6C.unk42 = 0x23B;
            sp6C.unk4 = 4;
            break;
        case 0x4:
            sp6C.unk24.y = rand_next(0xA, 0x14) * 0.004f;
            sp6C.unk30.w = (rand_next(0, 0xA) * 0.0001f) + 0.006f;
            sp6C.unk8 = 0x3C;
            sp6C.unk60 = 0xCD;
            sp6C.unk61 = 6;
            sp6C.unk44 = 0x0A100200;
            sp6C.unk42 = 0x47;
            break;
        case 0x3:
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.y = rand_next(0x14, 0x3C) * 0.3f;
            sp6C.unk8 = 0x23;
            sp6C.unk60 = 0x96;
            sp6C.unk61 = 0x14;
            sp6C.unk44 = 0x09100110;
            sp6C.unk30.w = 0.15f;
            sp6C.unk42 = arg2->roll;
            break;
        case 0x5:
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.x = rand_next(-0x1E, 0x1E) * 0.18f;
            sp6C.unk30.y = rand_next(-0x1E, 0x1E) * 0.18f;
            sp6C.unk30.z = rand_next(-0x1E, 0x1E) * 0.18f;
            sp6C.unk24.y = rand_next(0xF, 0x23) * 0.025f;
            sp6C.unk30.w = rand_next(0x64, 0x96) * 0.00035f;
            sp6C.unk8 = rand_next(0x32, 0x50);
            sp6C.unk61 = rand_next(0xA, 0x1E);
            sp6C.unk44 = 0x100218;
            sp6C.unk42 = arg2->roll;
            if (arg2->roll == 0x4C) {
                sp6C.unk58[0] = 0x6400U;
                sp6C.unk58[1] = 0x3200U;
                sp6C.unk58[2] = 0xA000U;
                sp6C.unk4C[0] = 0x1F4;
                sp6C.unk4C[1] = 0;
                sp6C.unk4C[2] = 0x3E8;
                sp6C.unk48 = 0x20;
            }
            break;
        case 0x7:
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.x = rand_next(-0x1E, 0x1E) * 0.18f;
            sp6C.unk30.y = rand_next(-0x1E, 0x1E) * 0.18f;
            sp6C.unk30.z = rand_next(-0x1E, 0x1E) * 0.18f;
            sp6C.unk24.x = rand_next(-0x28, 0x28) * 0.035f;
            sp6C.unk24.y = rand_next(0xA, 0x28) * 0.035f;
            sp6C.unk24.z = rand_next(-0x28, 0x28) * 0.035f;
            sp6C.unk30.w = 0.002f;
            sp6C.unk8 = rand_next(0x14, 0x32);
            sp6C.unk61 = 0x1E;
            sp6C.unk44 = 0x511;
            sp6C.unk42 = arg2->roll;
            break;
        case 0x7B:
            sp6C.unk30.y = rand_next(0, 0xA) + 120.0f;
            sp6C.unk8 = 0x50;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x08100208;
            sp6C.unk42 = 0x91;
            sp6C.unk24.y = -0.67f;
            sp6C.unk30.w = 0.2f;
            break;
        case 0x7F:
            sp6C.unk8 = 0x64;
            sp6C.unk60 = 0x37;
            sp6C.unk44 = 0x400100;
            sp6C.unk30.w = 0.032f;
            switch (sp6C.transform.roll) {
            case 0:
                sp6C.unk42 = 0x15E;
                break;
            case 1:
                sp6C.unk42 = 0x15F;
                break;
            case 2:
                sp6C.unk42 = 0x15D;
                break;
            default:
                sp6C.unk42 = 0x15E;
                break;
            }
            sp6C.transform.roll = 0;
            break;
        case 0x7C:
            sp6C.unk24.x = (f32)rand_next(-0x1E, 0x1E) * 0.03;
            sp6C.unk24.z = (f32)rand_next(-0x1E, 0x1E) * 0.03;
            sp6C.unk30.w = 0.0016f;
            sp6C.unk8 = 0x12C;
            sp6C.unk61 = 0;
            sp6C.unk4 = 0x7D;
            sp6C.unk44 = 0x41001C;
            sp6C.unk42 = 0x65;
            break;
        case 0x7D:
            sp6C.unk8 = 0x14;
            sp6C.unk61 = 0;
            sp6C.unk60 = 0x32;
            sp6C.unk44 = 0x400100;
            sp6C.unk42 = 0x65;
            sp6C.unk30.y = 0.0f;
            sp6C.unk30.w = 0.002f;
            break;
        case 0x7E:
            sp6C.unk8 = 0x32;
            sp6C.unk44 = 0x400100;
            sp6C.unk24.x = rand_next(-4, 4) * 0.06f;
            sp6C.unk24.z = rand_next(-4, 4) * 0.06f;
            sp6C.unk24.y = rand_next(0x28, 0x50) * 0.006f;
            sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00013f;
            switch (sp6C.transform.roll) {
            case 0:
                sp6C.unk42 = 0xDD;
                break;
            case 1:
                sp6C.unk42 = 0x160;
                break;
            case 2:
                sp6C.unk42 = 0xDF;
                break;
            default:
                sp6C.unk42 = 0xDF;
                break;
            }
            sp6C.transform.roll = 0;
            break;
        case 0x3E7:
            sp6C.unk8 = 0x12C;
            sp6C.unk44 = 0x80400500;
            sp6C.unk24.x = rand_next(-4, 4) * 0.02f;
            sp6C.unk24.z = rand_next(-4, 4) * 0.03f;
            sp6C.unk24.y = rand_next(0x28, 0x50) * 0.002f;
            sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00013f;
            switch (sp6C.transform.roll) {
            case 0:
                sp6C.unk42 = 0xDD;
                break;
            case 1:
                sp6C.unk42 = 0x160;
                break;
            case 2:
                sp6C.unk42 = 0xDF;
                break;
            default:
                sp6C.unk42 = 0xDF;
                break;
            }
            sp6C.transform.roll = 0;
            break;
        case 0x80:
            sp6C.unk8 = 2;
            sp6C.unk61 = 0;
            sp6C.unk60 = 0x32;
            sp6C.unk44 = 0x400110;
            sp6C.unk42 = 0xDF;
            sp6C.unk30.w = 0.004f;
            break;
        case 0x81:
            sp6C.unk30.x = rand_next(-0xA0, 0xA0);
            sp6C.unk30.y = rand_next(-0x32, 0xFA);
            sp6C.unk30.x = rand_next(-0xA0, 0xA0);
            sp6C.unk8 = 0xC8;
            sp6C.unk61 = 0x10;
            sp6C.unk44 = 0x80000108;
            sp6C.unk42 = 0x165;
            sp6C.unk4 = 0x7D;
            sp6C.unk30.w = 0.0025f;
            break;
        case 0x82:
            sp6C.unk30.x = rand_next(-0xA0, 0xA0);
            sp6C.unk30.y = rand_next(-0x32, 0xFA);
            sp6C.unk30.x = rand_next(-0xA0, 0xA0);
            sp6C.unk8 = 0xC8;
            sp6C.unk61 = 0x10;
            sp6C.unk44 = 0x80000108;
            sp6C.unk42 = 0x166;
            sp6C.unk4 = 0x7D;
            sp6C.unk30.w = 0.0025f;
            break;
        case 0x83:
            sp6C.unk30.x = rand_next(-0xA0, 0xA0);
            sp6C.unk30.y = rand_next(-0x32, 0xFA);
            sp6C.unk30.x = rand_next(-0xA0, 0xA0);
            sp6C.unk8 = 0xC8;
            sp6C.unk61 = 0x10;
            sp6C.unk44 = 0x80000108;
            sp6C.unk42 = 0x167;
            sp6C.unk4 = 0x7D;
            sp6C.unk30.w = 0.0025f;
            break;
        case 0x71:
            sp6C.unk30.x = rand_next(-2, 2);
            sp6C.unk30.y = 20.0f;
            sp6C.unk30.z = rand_next(-0x10, 0x10);
            sp6C.unk24.y = rand_next(-3, -1) * 0.05f;
            sp6C.unk30.w = rand_next(1, 3) * 0.00025f;
            sp6C.unk8 = 0x64;
            sp6C.unk60 = 0x7D;
            sp6C.unk61 = 0x10;
            sp6C.unk44 = 0x80000100;
            sp6C.unk42 = 0x2C;
            break;
        case 0x6D:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk30.w = arg2->scale;
            sp6C.unk8 = 1;
            sp6C.unk61 = 0;
            sp6C.unk60 = 0x19;
            if (arg2->roll != 0) {
                sp6C.unk60 = 0x7D;
            }
            sp6C.unk44 = 0xC0012;
            sp6C.unk42 = 0x77;
            break;
        case 0x6A:
            sp6C.unk30.x = rand_next(-0xA, 0xA) * sp38;
            sp6C.unk30.y = 0.0f;
            sp6C.unk30.z = rand_next(-0xA, 0xA) * sp38;
            sp6C.unk24.x = 0.0f;
            sp6C.unk24.y = rand_next(1, 3) * 0.15f;
            sp6C.unk8 = 0x78;
            sp6C.unk60 = 0xFF;
            sp6C.unk61 = 0x10;
            sp6C.unk44 = 0x100200;
            sp6C.unk42 = 0x5F;
            sp6C.unk24.z = 0.0f;
            sp6C.unk30.w = 0.02f;
            break;
        case 0x66:
            sp6C.unk61 = 0x20;
            sp6C.unk8 = 0x50;
            sp6C.unk4 = 0x67;
            sp6C.unk44 = 0x400000;
            sp6C.unk42 = 0x156;
            sp6C.unk30.w = 0.016f;
            break;
        case 0x67:
            sp6C.unk8 = 0x1E;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x200;
            sp6C.unk30.w = 0.016f;
            sp6C.unk42 = rand_next(0, 2) + 0x156;
            break;
        case 0x68:
            sp6C.unk24.x = rand_next(-0xA, 0xA) * 0.032f;
            sp6C.unk24.y = rand_next(-0xA, 0xA) * 0.032f;
            sp6C.unk24.z = rand_next(-0xA, 0xA) * 0.032f;
            sp6C.unk8 = 0x69;
            sp6C.unk44 = 0x480200;
            sp6C.unk42 = 0x156;
            sp6C.unk30.w = 0.00188f;
            break;
        case 0x65:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk4C[0] = 0xFFFF;
            sp6C.unk4C[1] = 0xFFFF;
            sp6C.unk4C[2] = 0xFFFF;
            sp6C.unk58[0] = 0U;
            sp6C.unk58[1] = 0U;
            sp6C.unk58[2] = 0U;
            sp6C.unk8 = 0x64;
            sp6C.unk60 = 0xFF;
            sp6C.unk48 = 0x20;
            sp6C.unk42 = 0x30;
            sp6C.unk30.w = 0.005565f;
            break;
        case 0x72:
            sp6C.unk30.w = rand_next(1, 5) * 0.00037f;
            sp6C.unk8 = rand_next(0x1E, 0x28);
            sp6C.unk44 = 0;
            sp6C.unk48 = 2;
            sp6C.unk61 = 0;
            sp6C.unk42 = 0xDE;
            break;
        case 0x73:
            sp6C.unk30.w = rand_next(4, 5) * 0.00097f;
            sp6C.unk30.w *= 0.5f;
            sp6C.unk8 = rand_next(0x1E, 0x28);
            sp6C.unk44 = 0;
            sp6C.unk48 = 2;
            sp6C.unk61 = 0x10;
            sp6C.unk42 = 0xDF;
            break;
        case 0x55:
            sp6C.unk8 = 0x78;
            sp6C.unk60 = 0xFF;
            sp6C.unk61 = 0x20;
            sp6C.unk44 = 0x0A100201;
            sp6C.unk42 = 0x56;
            sp6C.unk30.w = 0.01f;
            break;
        case 0x59:
            sp6C.unk24.x = 2.0f - rand_next(0, 4);
            sp6C.unk24.y = 2.0f - rand_next(0, 4);
            sp6C.unk24.z = 2.0f - rand_next(0, 4);
            sp6C.unk30.w = rand_next(1, 0x28) * 0.0001f;
            sp6C.unk8 = 0x28;
            sp6C.unk44 = 0x200;
            sp6C.unk42 = 0x2B;
            break;
        case 0x51:
            sp6C.unk8 = 0xA;
            sp6C.unk44 = 0x200;
            sp6C.unk42 = 0x2B;
            sp6C.unk30.w = 0.001f;
            break;
        case 0x50:
            sp6C.unk8 = 0xA;
            sp6C.unk44 = 0x200;
            sp6C.unk42 = 0x2B;
            sp6C.unk30.w = 0.004f;
            break;
        case 0x4D:
            sp6C.unk8 = 0x190;
            sp6C.unk4 = 0x4E;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x20100;
            sp6C.unk42 = 0xDF;
            sp6C.unk30.y = 2000.0f;
            sp6C.unk30.w = 0.04f;
            sp6C.transform.transl.x = 0.0f;
            sp6C.transform.transl.y = 0.0f;
            sp6C.transform.transl.z = 0.0f;
            sp6C.transform.scale = 1.0f;
            sp6C.transform.roll = 0x64 - rand_next(0, 0xC8);
            sp6C.transform.pitch = 0x64 - rand_next(0, 0xC8);
            sp6C.transform.yaw = 0x64 - rand_next(0, 0xC8);
            break;
        case 0x4E:
            sp6C.unk24.x = (1 - rand_next(0, 2)) * 0.6f;
            sp6C.unk24.z = (1 - rand_next(0, 2)) * 0.6f;
            sp6C.unk8 = 0x4B;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x200;
            sp6C.unk42 = 0x7B;
            sp6C.unk30.w = 0.008f;
            break;
        case 0x4A:
            sp6C.unk8 = 0x78;
            sp6C.unk61 = 0;
            sp6C.unk4 = 0x4B;
            sp6C.unk44 = 0x70000;
            sp6C.unk30.y = 40.0f;
            sp6C.unk30.w = 0.012f;
            sp64 = rand_next(0, 3);
            sp6C.unk42 = sp64 + 0xDD;
            sp6C.transform.roll = 0;
            sp6C.transform.transl.x = 0.0f;
            sp6C.transform.transl.y = 30.0f;
            sp6C.transform.transl.z = 0.0f;
            sp6C.transform.scale = 1.0f;
            sp6C.transform.pitch = 0x1F4 - rand_next(0, 0x3E8);
            sp6C.transform.yaw = 0x1F4 - rand_next(0, 0x3E8);
            break;
        case 0x49:
            sp6C.unk8 = 0xE;
            sp6C.unk60 = 0;
            sp6C.unk44 = 0x110210;
            sp6C.unk42 = 0x31;
            sp6C.unk30.y = 20.0f;
            sp6C.unk30.w = 0.5f;
            break;
        case 0x47:
            sp6C.unk30.x = 2.0f - rand_next(0, 4);
            sp6C.unk30.y = 2.0f - rand_next(0, 4);
            sp6C.unk30.z = 2.0f - rand_next(0, 4);
            sp6C.unk30.w = 0.015f;
            sp6C.unk8 = rand_next(4, 0xE);
            sp6C.unk44 = 0x110100;
            sp6C.unk42 = 0xDC;
            break;
        case 0x42:
            sp6C.unk30.x = 2.0f - rand_next(0, 4);
            sp6C.unk30.y = 2.0f - rand_next(0, 4);
            sp6C.unk30.z = 2.0f - rand_next(0, 4);
            sp6C.unk30.w = 0.002f;
            sp6C.unk8 = 1;
            sp6C.unk44 = 0x70800;
            sp6C.unk42 = rand_next(0, 1) + 0xDD;
            sp6C.transform.transl.x = 0.0f;
            sp6C.transform.transl.y = 0.0f;
            sp6C.transform.transl.z = 0.0f;
            sp6C.transform.scale = 1.0f;
            sp6C.transform.roll = 0x1F4 - rand_next(0, 0x3E8);
            sp6C.transform.pitch = 0x1F4 - rand_next(0, 0x3E8);
            sp6C.transform.yaw = 0x1F4 - rand_next(0, 0x3E8);
            break;
        case 0x40:
            sp6C.unk30.y = rand_next(0, 0x28);
            sp6C.unk24.x = (1 - rand_next(0, 2)) * 0.4f;
            sp6C.unk24.y = rand_next(1, 3) * 0.4f;
            sp6C.unk24.z = (1 - rand_next(0, 2)) * 0.4f;
            sp6C.unk8 = 0x96;
            sp6C.unk44 = 0x108;
            sp6C.unk42 = 0x5C;
            sp6C.unk30.w = 0.004f;
            break;
        case 0x41:
            for (sp60 = 0; sp60 < 30; sp60++) {
                sp6C.unk30.y = -10.0f;
                sp6C.unk24.x = (2 - rand_next(0, 4)) * 1.6f;
                sp6C.unk24.y = rand_next(1, 2) * 0.4f;
                sp6C.unk24.z = (2 - rand_next(0, 4)) * 1.6f;
                sp6C.unk8 = 0x3C;
                sp6C.unk44 = 0x108;
                sp6C.unk42 = 0x5C;
                sp6C.unk30.w = 0.003f;
                sp62 = gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            }
            break;
        case 0x3D:
        case 0x3E:
        case 0x3F:
        case 0x43:
        case 0x44:
        case 0x4F:
            sp6C.unk44 = 0x100100;
            sp6C.unk8 = 0x190;
            if (arg1 == 0x3D) {
                sp6C.unk30.x = 10.0f - rand_next(0, 0x14);
                sp6C.unk30.y = 35.0f;
                sp6C.unk30.z = 10.0f - rand_next(0, 0x14);
                sp6C.unk30.w = rand_next(1, 3) * 0.06f;
            } else if (arg1 == 0x3E) {
                sp6C.unk30.x = 10.0f - rand_next(0, 0x14);
                sp6C.unk30.y = 220.0f;
                sp6C.unk30.z = 10.0f - rand_next(0, 0x14);
                sp6C.unk30.w = rand_next(1, 3) * 0.04f;
            } else if (arg1 == 0x3F) {
                sp6C.unk8 = 0x64;
                sp6C.unk30.x = 0.0f;
                sp6C.unk30.y = -18.0f;
                sp6C.unk30.z = 0.0f;
                sp6C.unk30.w = rand_next(1, 3) * 0.04f;
            } else if (arg1 == 0x43) {
                sp6C.unk30.x = 110.0f;
                sp6C.unk30.y = 60.0f;
                sp6C.unk30.z = rand_next(0, 0x78) + -20.0f;
                sp6C.unk30.w = rand_next(1, 8) * 0.01f;
                sp6C.unk44 |= 8;
            } else if (arg1 == 0x44) {
                sp6C.unk30.x = 110.0f;
                sp6C.unk30.y = 85.0f;
                sp6C.unk30.z = rand_next(0, 0x78);
                sp6C.unk24.y = -0.26f;
                sp6C.unk30.w = rand_next(1, 8) * 0.01f;
            }
            sp6C.unk61 = 0x20;
            sp6C.unk42 = 0x5F;
            sp6C.unk44 |= arg3;
            if (sp6C.unk44 & 1) {
                if (sp6C.unk0 != NULL) {
                    sp6C.unk30.x += sp6C.unk0->positionMirror.x;
                    sp6C.unk30.y += sp6C.unk0->positionMirror.y;
                    sp6C.unk30.z += sp6C.unk0->positionMirror.z;
                } else {
                    sp6C.unk30.x += sp6C.transform.transl.x;
                    sp6C.unk30.y += sp6C.transform.transl.y;
                    sp6C.unk30.z += sp6C.transform.transl.z;
                }
            }
            if ((arg1 == 0x3E) || (arg1 == 0x3F)) {
                sp6C.unk44 |= 0x08000000;
            }
            break;
        case 0x48:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk30.y = 0.0f;
            sp6C.unk24.y = rand_next(1, 0xA) * 0.2f;
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 0.5f;
            sp3C.roll = 0x7D0 - rand_next(0, 0xFA0);
            sp3C.pitch = 0x7D0 - rand_next(0, 0xFA0);
            sp3C.yaw = 0x7D0 - rand_next(0, 0xFA0);
            rotate_vec3(&sp3C, sp6C.unk24.f);
            sp6C.unk8 = 0x50;
            sp6C.unk61 = 8;
            sp6C.unk44 = 0x100;
            sp6C.unk42 = 0xDD;
            sp6C.unk30.w = 0.0036f;
            break;
        case 0x38:
            rand_set_seed(0x4233D);
            for (sp60 = 0; sp60 < 40; sp60++) {
                sp6C.unk30.y = 35.0f;
                sp6C.unk24.x = (0x50 - rand_next(0, 0xA0)) * 0.01f;
                sp6C.unk24.z = (0x50 - rand_next(0, 0xA0)) * 0.01f;
                sp6C.unk30.w = 0.0025f;
                sp6C.unk8 = rand_next(1, 4) * 33.0f;
                sp6C.unk44 = 0x100011;
                sp6C.unk42 = 0x30;
                if (sp6C.unk0 != NULL) {
                    sp6C.unk30.x += sp6C.unk0->srt.transl.f[0];
                    sp6C.unk30.y += sp6C.unk0->srt.transl.f[1];
                    sp6C.unk30.z += sp6C.unk0->srt.transl.f[2];
                } else {
                    sp6C.unk30.x += sp6C.transform.transl.x;
                    sp6C.unk30.y += sp6C.transform.transl.y;
                    sp6C.unk30.z += sp6C.transform.transl.z;
                }
                sp62 = gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, 0);
            }
            break;
        case 0x35:
            sp6C.unk30.x = (0x1E - rand_next(0, 0x3C)) * 0.14f;
            sp6C.unk30.y = 44.0f;
            sp6C.unk30.z = (0x1E - rand_next(0, 0x3C)) * 0.14f;
            sp6C.unk24.y = rand_next(0x28, 0x50) * 0.0045f;
            sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00002f;
            sp6C.unk8 = rand_next(0x28, 0x50);
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x80400001;
            sp6C.unk42 = 0x47;
            break;
        case 0x3A:
            sp6C.unk30.x = (0x1E - rand_next(0, 0x3C)) * 0.14f;
            sp6C.unk30.y = 10.0f;
            sp6C.unk30.z = (0x1E - rand_next(0, 0x3C)) * 0.14f;
            sp6C.unk24.y = rand_next(0x28, 0x50) * 0.0045f;
            sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00002f;
            sp6C.unk8 = 0xB4;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x80400200;
            sp6C.unk42 = 0x47;
            break;
        case 0x3B:
            sp6C.unk30.x = (0x1E - rand_next(0, 0x3C)) * 0.04f;
            sp6C.unk30.y = 20.0f;
            sp6C.unk30.z = (0x1E - rand_next(0, 0x3C)) * 0.04f;
            sp6C.unk24.y = rand_next(0x28, 0x50) * 0.0045f;
            sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00002f;
            sp6C.unk8 = 0x78;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x80400201;
            sp6C.unk42 = 0x47;
            break;
        case 0x53:
            sp6C.unk30.x = (0x1E - rand_next(0, 0x3C)) * 0.14f;
            sp6C.unk30.z = (0x1E - rand_next(0, 0x3C)) * 0.14f;
            sp6C.unk24.y = rand_next(0x28, 0x50) * 0.0015f;
            sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00002f;
            sp6C.unk8 = 0xD2;
            sp6C.unk44 = 0x80000201;
            sp6C.unk42 = rand_next(0, 3) + 0xDD;
            break;
        case 0x2E:
            sp6C.unk8 = 0x30;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x08100210;
            sp6C.unk42 = 0x5E;
            sp6C.unk30.w = 0.1f;
            break;
        case 0x78:
            sp6C.unk30.y = rand_next(0, 0x64);
            sp6C.unk8 = 0x30;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x08100210;
            sp6C.unk42 = 0x5E;
            sp6C.unk30.w = 0.1f;
            break;
        case 0x3E6:
            sp6C.unk30.x = rand_next(-4, 4);
            sp6C.unk30.z = rand_next(-4, 4);
            sp6C.unk24.y = rand_next(4, 0xA) * 0.07f;
            sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00005f;
            sp6C.unk8 = 0x15E;
            sp6C.unk4 = 0x85;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x80400201;
            sp6C.unk42 = 0xDF;
            break;
        case 0x77:
            sp6C.unk30.x = rand_next(-4, 4);
            sp6C.unk30.y = rand_next(0, 0x28);
            sp6C.unk30.z = rand_next(-4, 4);
            sp6C.unk24.x = rand_next(-0x28, 0x28) * 0.0025f;
            sp6C.unk24.y = rand_next(0, 0x50) * 0.0045f;
            sp6C.unk24.z = rand_next(-0x28, 0x28) * 0.0025f;
            sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00005f;
            sp6C.unk8 = rand_next(0, 0x118) + 0x96;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x400101;
            sp6C.unk42 = 0xDF;
            break;
        case 0x7A:
            sp6C.unk30.x = rand_next(-4, 4);
            sp6C.unk30.y = rand_next(0, 0x23);
            sp6C.unk30.z = rand_next(-4, 4);
            sp6C.unk24.x = rand_next(-0x28, 0x28) * 0.0025f;
            sp6C.unk24.z = rand_next(-0x28, 0x28) * 0.0025f;
            sp6C.unk24.y = rand_next(0, 0x50) * 0.0025f;
            sp6C.unk30.w = rand_next(0x28, 0x50) * 0.00005f;
            sp6C.unk8 = rand_next(0, 0x118) + 0xB4;
            sp6C.unk60 = 0;
            sp6C.unk44 = 0xC80404;
            sp6C.unk42 = 0xDF;
            break;
        case 0x76:
            sp6C.unk30.w = rand_next(1, 8) * 0.0125f;
            sp6C.unk8 = rand_next(0, 0x32) + 0x26;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x06100110;
            sp6C.unk42 = 0x159;
            sp6C.transform.transl.x = 0.0f;
            sp6C.transform.transl.y = 0.0f;
            sp6C.transform.transl.z = 0.0f;
            break;
        case 0x2F:
            sp6C.unk8 = 0x32;
            sp6C.unk61 = 0x20;
            sp6C.unk44 = 0x400010;
            sp6C.unk42 = 0x71;
            sp6C.unk30.w = 0.05f;
            break;
        case 0x34:
            sp6C.unk8 = 0x1E;
            sp6C.unk61 = 0x20;
            sp6C.unk44 = 0x400210;
            sp6C.unk42 = 0x71;
            sp6C.unk30.w = 0.05f;
            break;
        case 0x30:
            sp6C.unk8 = 0x14;
            sp6C.unk44 = 0x400010;
            sp6C.unk42 = 0x7C;
            sp6C.unk30.w = 1.0f;
            break;
        case 0x39:
            if (rand_next(0, 1) != 0) {
                sp6C.unk30.z = -400.0f;
            } else {
                sp6C.unk30.z = 200.0f;
            }
            sp6C.unk30.w = rand_next(1, 4) * 0.55f;
            sp6C.unk8 = rand_next(0, 0x18) + 0x18;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x100;
            sp6C.unk42 = 0x33;
            break;
        case 0x79:
            if (rand_next(0, 1) != 0) {
                sp6C.unk30.x = -18.0f;
            } else {
                sp6C.unk30.x = 18.0f;
            }
            sp6C.unk30.y = rand_next(0xA, 0x3C);
            sp6C.unk30.z = rand_next(-3, 3);
            sp6C.unk24.y = rand_next(1, 0x14) * 0.01f;
            sp6C.unk30.w = rand_next(1, 7) * 0.006f;
            sp6C.unk8 = rand_next(0, 0xF) + 0xF;
            sp6C.unk60 = 0x9B;
            sp6C.unk44 = 0x100100;
            sp6C.unk42 = 0x156;
            break;
        case 0x75:
            sp6C.unk8 = 0x62;
            sp6C.unk60 = 0xFF;
            sp6C.unk40 = 0xA9;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x08100210;
            sp6C.unk42 = 0x159;
            sp6C.unk30.w = 0.4f;
            break;
        case 0x32:
            sp6C.unk8 = 0x96;
            sp6C.unk44 = 0x400012;
            sp6C.unk42 = 0x7C;
            sp6C.unk30.w = 0.035f;
            break;
        case 0x33:
            sp6C.unk8 = 0x55;
            sp6C.unk44 = 0x400012;
            sp6C.unk42 = 0x7C;
            sp6C.unk30.y = 35.0f;
            sp6C.unk30.w = 0.008f;
            break;
        case 0x69:
            sp6C.unk8 = 0x44;
            sp6C.unk44 = 0x100201;
            sp6C.unk42 = 0x60;
            sp6C.unk30.w = 0.12f;
            break;
        case 0x2:
            sp6C.unk30.x = rand_next(-0x14, 0x14) * 0.4f;
            sp6C.unk30.y = rand_next(-0x14, 0x14) * 0.4f;
            sp6C.unk30.z = rand_next(-0x14, 0x14) * 0.4f;
            sp6C.unk30.w = (rand_next(0, 0x1E) * 0.001f) + 0.08f;
            sp6C.unk8 = rand_next(0, 8) + 8;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x100100;
            sp6C.unk42 = 0x33;
            break;
        case 0x2A:
            sp6C.unk30.x = rand_next(-0x1E, 0x1E) * 0.1f;
            sp6C.unk30.y = rand_next(-0x1E, 0x1E) * 0.1f;
            sp6C.unk30.z = rand_next(-0x1E, 0x1E) * 0.1f;
            sp6C.unk30.w = (rand_next(0, 0xA) * 0.0003f) + 0.008f;
            sp6C.unk8 = rand_next(0x14, 0x32);
            sp6C.unk60 = 0x9B;
            sp6C.unk61 = 0xE;
            sp6C.unk44 = 0x100110;
            if (arg5 != NULL) {
                sp68 = arg5;
                sp64 = ((s16) ((s16*)arg5)[0x39] >> 1) - 1;
                if (sp64 < 0) {
                    sp64 = 0;
                }
                sp6C.unk42 = 0x78;
                if (sp64 == 1) {
                    sp6C.unk42 = 0x87;
                } else if (sp64 == 2) {
                    sp6C.unk42 = 0x88;
                }
            } else {
                sp6C.unk42 = 0x88;
            }
            break;
        case 0x37:
            sp6C.unk8 = 0x14;
            sp6C.unk40 = 0x9A;
            sp6C.unk44 = 0x20100210;
            sp6C.unk42 = 0x87;
            sp6C.unk30.w = 0.025f;
            break;
        case 0x2B:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk24.x = 0.05f;
            sp54.z = rand_next(0, 0xFFFE);
            sp54.y = rand_next(0, 0xFFFE);
            sp54.x = rand_next(0, 0xFFFE);
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 1.0f;
            sp3C.roll = sp54.x;
            sp3C.pitch = sp54.y;
            sp3C.yaw = sp54.z;
            rotate_vec3(&sp3C, sp6C.unk24.f);
            sp6C.unk8 = 0x32;
            sp6C.unk40 = 0;
            sp6C.unk44 = 0x100;
            sp6C.unk42 = 0x30;
            sp6C.unk30.w = 0.0005f;
            break;
        case 0x2C:
            sp6C.unk8 = 0xA;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x80211;
            sp6C.unk42 = 0x3FF;
            sp6C.unk30.w = 0.01f;
            break;
        case 0x28:
            sp6C.unk8 = 0x46;
            sp6C.unk44 = 0x0B100200;
            sp6C.unk42 = 0x74;
            sp6C.unk30.w = 0.1f;
            break;
        case 0x31:
            sp6C.unk8 = 0x46;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x0B100200;
            sp6C.unk42 = 0x74;
            sp6C.unk30.w = 0.033f;
            break;
        case 0x2D:
            sp6C.unk30.y = 35.0f;
            sp6C.unk24.x = (0x50 - rand_next(0, 0xA0)) * 0.01f;
            sp6C.unk24.z = (0x50 - rand_next(0, 0xA0)) * 0.01f;
            sp6C.unk30.w = 0.0025f;
            sp6C.unk8 = rand_next(1, 4) * 33.0f;
            sp6C.unk44 = 0x100000;
            sp6C.unk42 = 0x30;
            break;
        case 0x25:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 == NULL) {
                return -1;
            }
            sp6C.unk30.x = rand_next(0, 6) + arg2->transl.x;
            sp6C.unk30.y = arg2->transl.y;
            sp6C.unk30.z = rand_next(0, 6) + arg2->transl.z;
            sp6C.unk24.y = rand_next(0, 0xA) * 0.012f;
            sp6C.unk30.w = rand_next(4, 8) * 0.003f;
            sp6C.unk8 = 0x24;
            sp6C.unk60 = 0x41;
            sp6C.unk44 = 0x100112;
            sp6C.unk42 = 0x61;
            break;
        case 0x36:
            if (arg5 == NULL) {
                return -1;
            }
            sp68 = arg5;
            sp6C.unk8 = 0x20;
            sp6C.unk60 = 0xFF;
            sp6C.unk61 = 0x20;
            sp6C.unk44 = 0x01100201;
            sp6C.unk42 = 0x249;
            sp6C.unk30.x = sp6C.unk30.y = sp6C.unk30.z = 0.0f;
            sp6C.unk30.w = 0.002f;
            break;
        case 0x26:
            sp6C.unk30.x = rand_next(-1, 1);
            if (arg5 != NULL) {
                sp6C.unk30.x += arg5[1];
            }
            sp6C.unk30.y = 0.0f;
            sp6C.unk30.z = rand_next(-1, 1);
            sp6C.unk24.y = 0.05f;
            sp6C.unk30.w = 0.005f;
            if (arg5 != NULL) {
                sp6C.unk8 = arg5[0];
            } else {
                sp6C.unk8 = 0x78;
            }
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x80100201;
            sp6C.unk44 = 0x100201;
            sp6C.unk42 = 0x63;
            sp3C.roll = 0;
            sp3C.pitch = 0;
            sp3C.transl.x = 0.0f;
            sp3C.transl.y = 0.0f;
            sp3C.transl.z = 0.0f;
            sp3C.scale = 1.0f;
            sp3C.yaw = arg0->srt.yaw;
            rotate_vec3(&sp3C, sp6C.unk30.f);
            break;
        case 0xC:
            sp6C.unk8 = 0x8A;
            sp6C.unk44 = 0x10000;
            sp6C.unk42 = 0x30;
            sp6C.unk30.w = 0.02f;
            break;
        case 0xD:
            sp6C.unk8 = 0x8A;
            sp6C.unk44 = 0x10000;
            sp6C.unk42 = 0x30;
            sp6C.unk30.w = 0.02f;
            break;
        case 0xE:
            sp6C.unk8 = 0x8A;
            sp6C.unk44 = 0x10002;
            sp6C.unk42 = 0x30;
            sp6C.unk30.y = 20.0f;
            sp6C.unk30.w = 0.02f;
            break;
        case 0x0:
            sp6C.unk8 = 6;
            sp6C.unk40 = 0;
            sp6C.unk44 = 0x10;
            sp6C.unk42 = 0x87;
            sp6C.unk30.w = 0.01f;
            break;
        case 0xF:
            sp6C.unk30.x = 8.0f;
            sp6C.unk30.y = 40.0f;
            sp6C.unk30.z = 5.0f;
            sp6C.unk24.x = (0x50 - rand_next(0, 0xA0)) * 0.025f;
            sp6C.unk24.z = (0x50 - rand_next(0, 0xA0)) * 0.025f;
            sp6C.unk30.w = 0.005f;
            sp6C.unk8 = (rand_next(0, 3) + 1) * 33.0f;
            sp6C.unk44 = 0x110214;
            sp6C.unk42 = 0x30;
            break;
        case 0x11:
            sp6C.unk24.x = (0x50 - rand_next(0, 0xA0)) * 0.025f;
            sp6C.unk24.y = rand_next(0, 0x50) * 0.05f;
            sp6C.unk24.z = (0x50 - rand_next(0, 0xA0)) * 0.025f;
            sp6C.unk30.w = 0.005f;
            sp6C.unk8 = (rand_next(0, 3) + 1) * 33.0f;
            sp6C.unk44 = 0x01110214;
            sp6C.unk42 = 0x33;
            break;
        case 0x19:
            sp6C.unk24.x = rand_next(-0xA, 0xA) * 0.01f;
            sp6C.unk24.y = rand_next(-0xA, 0xA) * 0.01f;
            sp6C.unk24.z = rand_next(-0xA, 0xA) * 0.01f;
            sp6C.unk30.w = 0.001f;
            sp6C.unk8 = 0x32;
            sp6C.unk44 = 0x211;
            sp6C.unk42 = 0x30;
            break;
        case 0x1A:
            sp6C.unk24.x = (0xA - rand_next(0, 0x14)) * 0.02f;
            sp6C.unk24.y = rand_next(0, 0x3C) * 0.02f;
            sp6C.unk24.z = (0xA - rand_next(0, 0x14)) * 0.02f;
            sp6C.unk30.w = rand_next(0, 4) * 0.0005f;
            sp6C.unk8 = (rand_next(0, 3) + 1) * 14.0f;
            sp6C.unk44 = 0x01000211;
            sp6C.unk42 = 0x30;
            break;
        case 0x1B:
            sp6C.unk24.y = rand_next(0, 0x3C) * 0.02f;
            sp6C.unk30.w = rand_next(0, 4) * 0.0005f;
            sp6C.unk8 =  (rand_next(0, 3) + 1) * 78.0f;
            sp6C.unk61 = 5;
            sp6C.unk44 = 0x01000211;
            sp6C.unk42 = 0x30;
            break;
        case 0x20:
            sp6C.unk8 = 0xC8;
            sp6C.unk60 = 0x9B;
            sp6C.unk44 = 0x12;
            sp6C.unk42 = 0x22D;
            sp6C.unk30.y = 50.0f;
            sp6C.unk30.w = 0.008f;
            break;
        case 0x21:
            sp6C.unk30.x = (0xA - rand_next(0, 0x14));
            sp6C.unk30.z = (0xA - rand_next(0, 0x14));
            sp6C.unk8 = 0x32;
            sp6C.unk44 = 0x201;
            sp6C.unk42 = 0x321;
            sp6C.unk24.x = 0.6f;
            sp6C.unk24.y = 3.0f;
            sp6C.unk24.z = 0.6f;
            sp6C.unk30.w = 0.008f;
            break;
        case 0x22:
            sp6C.unk8 = 0x178E;
            sp6C.unk60 = 0xFF;
            sp6C.unk61 = 0x10;
            sp6C.unk44 = 0x14;
            sp6C.unk42 = 0x30;
            sp6C.unk30.z = 30.0f;
            sp6C.unk30.w = 0.001f;
            break;
        case 0x23:
            sp6C.unk8 = 0x69;
            sp6C.unk44 = 0x400010;
            sp6C.unk42 = 0x4B;
            sp6C.unk30.y = 10.0f;
            sp6C.unk30.w = 0.019f;
            break;
        case 0x24:
            sp6C.unk8 = 0x5F;
            sp6C.unk44 = 0x400212;
            sp6C.unk42 = 0x4B;
            sp6C.unk30.w = 0.019f;
            break;
        case 0x1C:
            sp6C.unk30.x = rand_next(-0xC8, 0xC8);
            sp6C.unk30.y = 300.0f;
            sp6C.unk30.z = rand_next(-0xC8, 0xC8);
            sp6C.unk24.x = (0xA - rand_next(0, 0x14)) * 0.06f;
            sp6C.unk24.z = (0xA - rand_next(0, 0x14)) * 0.06f;
            sp6C.unk24.y = (0xA - rand_next(0, 0x14)) * 0.08f;
            sp6C.unk30.w = 0.0011f;
            sp6C.unk30.x = 0.0f;
            sp6C.unk30.y = 100.0f;
            sp6C.unk30.z = 0.0f;
            sp6C.unk8 = 0x104;
            sp6C.unk44 = 0x01000202;
            sp6C.unk4 = 0x1E;
            sp6C.unk24.z = (0xA - rand_next(0, 0x14)) * 0.06f;
            sp6C.unk8 = 0xA0;
            sp6C.unk44 = 0x01000204;
            sp6C.unk44 = 0x11000204;
            sp6C.unk42 = 0x151;
            sp6C.unk30.w = 0.0025f;
            break;
        case 0x74:
            sp6C.unk30.x = rand_next(-0x50, 0x50);
            sp6C.unk30.y = 0.0f;
            sp6C.unk30.z = rand_next(-0x50, 0x50);
            sp6C.unk24.y = rand_next(1, 4) * 0.1f;
            sp6C.unk30.w = 0.0022f;
            sp6C.unk8 = 0x140;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x01000204;
            sp6C.unk42 = 0x151;
            break;
        case 0x1D:
            sp6C.unk30.y = 48.0f;
            sp6C.unk30.z = -110.0f;
            sp6C.unk24.x = (0xA - rand_next(0, 0x14)) * 0.08f;
            sp6C.unk24.y = (0xA - rand_next(0, 0x14)) * 0.08f;
            sp6C.unk8 = 0x78;
            sp6C.unk44 = 0x204;
            sp6C.unk42 = 0x1F0;
            sp6C.unk30.w = 0.0088f;
            break;
        case 0x1E:
            sp6C.unk30.w = rand_next(1, 4) * 0.003f;
            sp6C.unk8 = 0x5A;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x0A100100;
            sp6C.unk42 = 0x56;
            sp6C.unk61 = 0;
            break;
        case 0x1F:
            sp6C.unk30.w = rand_next(2, 4) * 0.02f;
            sp6C.unk8 = 0xC8;
            sp6C.unk44 = 0x0A100201;
            sp6C.unk42 = 0x56;
            break;
        case 0x54:
            sp6C.unk30.x = (5 - rand_next(0, 0xA));
            sp6C.unk30.z = (5 - rand_next(0, 0xA));
            sp6C.unk30.w = rand_next(2, 0xC) * 0.004f;
            sp6C.unk8 = 0x78;
            sp6C.unk44 = 0x0A100201;
            sp6C.unk42 = 0x56;
            break;
        case 0x27:
            sp6C.unk30.y = 10.0f;
            sp6C.unk30.w = rand_next(1, 2) * 0.04f;
            sp6C.unk8 = 0xC8;
            sp6C.unk44 = 0x0A100201;
            sp6C.unk42 = 0x6B;
            break;
        case 0x13:
            sp6C.unk8 = 0xD05;
            sp6C.unk60 = 0;
            sp6C.unk44 = 0x11;
            sp6C.unk42 = 0x30;
            sp6C.unk30.w = 2.5f;
            break;
        case 0x14:
            sp6C.unk8 = 0xD;
            sp6C.unk44 = 0x110212;
            sp6C.unk42 = 0x33;
            sp6C.unk30.w = 0.5f;
            break;
        case 0x12:
            sp6C.unk8 = 0x14D;
            sp6C.unk44 = 0x10012;
            sp6C.unk42 = 0x33;
            sp6C.unk30.y = 40.0f;
            sp6C.unk30.w = 0.005f;
            break;
        case 0x10:
            sp6C.unk30.y = (0x14 - rand_next(0, 0x28));
            sp6C.unk24.x = (0x50 - rand_next(0, 0xA0)) * 0.025f;
            sp6C.unk24.z = (0x50 - rand_next(0, 0xA0)) * 0.025f;
            sp6C.unk30.w = 0.005f;
            sp6C.unk8 = (rand_next(0, 3) + 1) * 6.0f;
            sp6C.unk44 = 0x110204;
            sp6C.unk42 = 0x30;
            break;
        case 0x6:
            sp6C.unk8 = 0x12;
            sp6C.unk44 = 0x300200;
            sp6C.unk42 = 0x33;
            sp6C.unk30.w = 0.04f;
            break;
        case 0x8:
            sp6C.unk8 = 0x30;
            sp6C.unk60 = 0xC8;
            sp6C.unk44 = 0x300002;
            sp6C.unk42 = 0x2C;
            sp6C.unk30.y = 35.0f;
            sp6C.unk30.w = 0.06f;
            break;
        case 0x9:
            sp6C.unk8 = 0x3C;
            sp6C.unk60 = 0xC8;
            sp6C.unk44 = 0x300000;
            sp6C.unk42 = 0x2C;
            sp6C.unk30.y = 35.0f;
            sp6C.unk30.z = 50.0f;
            sp6C.unk30.w = 0.06f;
            break;
        case 0xA:
            sp6C.unk8 = 0x3C;
            sp6C.unk60 = 0xC8;
            sp6C.unk44 = 0x300000;
            sp6C.unk42 = 0x2C;
            sp6C.unk30.w = 0.06f;
            break;
        case 0x6B:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg5 == NULL) {
                return -1;
            }
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk24.x = arg5[0];
            sp6C.unk24.y = arg5[1];
            sp6C.unk24.z = arg5[2];
            sp6C.unk8 = 0x28;
            sp6C.unk30.w = 0.001f;
            sp6C.unk60 = arg2->scale;
            sp6C.unk61 = 0xA;
            sp6C.unk44 = 0x200;
            sp6C.unk42 = 0x65;
            sp6C.transform.roll = 0;
            sp6C.transform.pitch = 0;
            sp6C.transform.transl.x = 0.0f;
            sp6C.transform.transl.y = 0.0f;
            sp6C.transform.transl.z = 0.0f;
            sp6C.transform.scale = 1.0f;
            sp6C.transform.yaw = arg2->yaw;
            break;
        case 0x6C:
            sp6C.unk8 = 1;
            sp6C.unk61 = 0;
            sp6C.unk44 = 0x11;
            sp6C.unk48 = 2;
            sp6C.unk42 = 0xDD;
            sp6C.unk30.w = 0.002f;
            break;
        case 0x56:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk30.x = rand_next(-6, 6);
            sp6C.unk30.z = rand_next(-6, 6);
            sp6C.unk24.x = rand_next(-2, 2) * 0.2f * arg2->scale;
            sp6C.unk24.y = rand_next(0, 4) * 0.1f * arg2->scale;
            sp6C.unk24.z = rand_next(-2, 2) * 0.2f * arg2->scale;
            sp6C.unk30.w = arg2->scale * 0.012f;
            sp6C.unk8 = 0x18;
            sp6C.unk44 = 0x01080000;
            sp6C.unk48 = 0x01000000;
            sp6C.unk60 = 0xA5;
            if (arg5 != NULL) {
                sp6C.unk58[0] = sp6C.unk4C[0] = ((u8*)arg5)[0] << 8;
                sp6C.unk58[1] = sp6C.unk4C[1] = ((u8*)arg5)[1] << 8;
                sp6C.unk58[2] = sp6C.unk4C[2] = ((u8*)arg5)[2] << 8;
                sp6C.unk48 |= 0x20;
            }
            sp6C.unk42 = 0x60;
            break;
        case 0x57:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk30.y = rand_next(0, 0xA);
            sp6C.unk24.x = rand_next(-0x64, 0x64) * 0.003f * arg2->scale;
            sp6C.unk24.y = rand_next(0xC8, 0x190) * 0.003f * arg2->scale;
            sp6C.unk24.z = rand_next(-0x64, 0x64) * 0.003f * arg2->scale;
            sp6C.unk30.w = rand_next(8, 0xB) * 0.0001f * arg2->scale;
            sp6C.unk60 = 0xBE;
            sp6C.unk8 = arg2->scale * 75.0f;
            sp6C.unk44 = 0x01200000;
            sp6C.unk48 = 0x01000000;
            sp6C.unk42 = 0x77;
            if (arg5 != NULL) {
                sp6C.unk58[0] = sp6C.unk4C[0] = ((u8*)arg5)[0] << 8;
                sp6C.unk58[1] = sp6C.unk4C[1] = ((u8*)arg5)[1] << 8;
                sp6C.unk58[2] = sp6C.unk4C[2] = ((u8*)arg5)[2] << 8;
                sp6C.unk48 |= 0x20;
            }
            break;
        case 0x58:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk24.x = rand_next(-0x64, 0x64) * 0.02f * arg2->scale;
            sp6C.unk24.y = rand_next(0xA, 0xC8) * 0.02f * arg2->scale;
            sp6C.unk24.z = rand_next(-0x64, 0x64) * 0.02f * arg2->scale;
            sp6C.unk30.w = (rand_next(8, 0xB) * 0.0001f) * arg2->scale;
            sp6C.unk8 = 0x4B;
            sp6C.unk44 = 0x01080000;
            sp6C.unk48 = 0x01000000;
            sp6C.unk42 = 0x77;
            if (arg5 != NULL) {
                sp6C.unk58[0] = sp6C.unk4C[0] = ((u8*)arg5)[0] << 8;
                sp6C.unk58[1] = sp6C.unk4C[1] = ((u8*)arg5)[1] << 8;
                sp6C.unk58[2] = sp6C.unk4C[2] = ((u8*)arg5)[2] << 8;
                sp6C.unk48 |= 0x20;
            }
            break;
        case 0x323:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk8 = 0x1E;
            sp6C.unk44 = 0x80210;
            sp6C.unk48 |= 0x02000000;
            sp6C.unk30.w = 0.025f;
            if (arg2 != NULL) {
                sp6C.unk42 = arg2->scale;
            } else {
                sp6C.unk42 = 0x206;
            }
            if (sp34 != NULL) {
                sp6C.unk30.w = *sp34;
            }
            break;
        case 0x324:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk24.x = rand_next(-0x64, 0x64) * 0.0035f;
            sp6C.unk24.y = rand_next(0, 0x64) * 0.0075f;
            sp6C.unk24.z = rand_next(-0x64, 0x64) * 0.0035f;
            sp6C.unk30.w = rand_next(0x1F4, 0x320) * 0.000002f;
            sp6C.unk8 = 0x50;
            sp6C.unk44 = 0x01088200;
            sp6C.unk48 = 0x100000;
            sp6C.unk42 = arg2->scale;
            break;
        case 0x325:
            sp6C.unk8 = 0x1E;
            sp6C.unk44 = 0x180210;
            sp6C.unk42 = 0x159;
            sp6C.unk30.w = 0.015f;
            if (sp34 != NULL) {
                sp6C.unk30.w = *sp34;
            }
            break;
        case 0x328:
            sp6C.unk8 = 0x28;
            sp6C.unk44 = 0x180210;
            sp6C.unk30.w = rand_next(8, 0xC) * 0.002f;
            sp6C.unk42 = 0x5C;
            break;
        case 0x3DE:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            if (arg2 != NULL) {
                sp6C.unk30.x = arg2->transl.f[0];
                sp6C.unk30.y = arg2->transl.f[1];
                sp6C.unk30.z = arg2->transl.f[2];
            } else {
                sp6C.unk30.x = rand_next(-0xA, 0xA) * 0.1f;
                sp6C.unk30.y = rand_next(-0xA, 0xA) * 0.1f;
                sp6C.unk30.z = rand_next(-0xA, 0xA) * 0.1f;
            }
            sp6C.unk8 = 0x96;
            sp6C.unk61 = 0x1E;
            sp6C.unk60 = 0xFF;
            sp6C.unk44 = 0x80080209;
            sp6C.unk48 = 0x01000020;
            sp6C.unk42 = 0x5F;
            sp6C.unk24.x = 0.0f;
            sp6C.unk24.y = 0.2f;
            sp6C.unk24.z = 0.0f;
            sp6C.unk30.w = 0.015f;
            sp6C.unk58[0] = 0xFFFFU;
            sp6C.unk58[1] = 0xFFFFU;
            sp6C.unk58[2] = 0xA000U;
            sp6C.unk4C[0] = 0xFFFF;
            sp6C.unk4C[1] = 0xFFFF;
            sp6C.unk4C[2] = 0xC000;
            break;
        case 0x3DF:
            sp6C.unk30.x = rand_next(-0x64, 0x64) * 0.1f;
            sp6C.unk30.y = rand_next(-0x64, 0x64) * 0.1f;
            sp6C.unk30.z = rand_next(-0x64, 0x64) * 0.1f;
            sp6C.unk24.y = rand_next(8, 0xA) * 0.05f;
            if (rand_next(0, 0x28) != 0) {
                sp6C.unk30.w = rand_next(8, 0x14) * 0.001f;
                sp6C.unk8 = rand_next(0x5A, 0x78);
            } else {
                sp6C.unk30.w = rand_next(0x15, 0x29) * 0.001f;
                sp6C.unk8 = 0x1CC;
            }
            sp6C.unk44 = 0x80380201;
            sp6C.unk48 = 0x01000020;
            sp6C.unk42 = 0x162;
            sp6C.unk60 = 0x7F;
            sp6C.unk4C[0] = 0xFF00;
            sp6C.unk4C[1] = 0xAC00;
            sp6C.unk4C[2] = 0x2800;
            sp6C.unk58[0] = 0xE100U;
            sp6C.unk58[1] = 0xAC00U;
            sp6C.unk58[2] = 0xB00U;
            break;
        case 0x320:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk24.x = rand_next(-2, 2) * 0.05f;
            sp6C.unk24.y = rand_next(2, 5) * 0.07f;
            sp6C.unk24.z = rand_next(1, 3) * -0.1f;
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk8 = 0x28;
            sp6C.unk48 = 0x01000000;
            sp6C.unk44 = 0x1C0208;
            sp6C.unk42 = 0x5F;
            sp6C.unk30.w = 0.03f;
            break;
        case 0x321:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk24.y = rand_next(0, 4) * 0.1f;
            sp6C.unk24.z = rand_next(2, 4) * -0.15f;
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk30.w = 0.01f;
            sp6C.unk8 = 0x64;
            sp6C.unk44 = 0x011C0200;
            sp6C.unk48 = 0x01000000;
            sp6C.unk42 = 0x60;
            break;
        case 0x322:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk30.w = 0.015f;
            sp6C.unk8 = 0x50;
            sp6C.unk44 = 0x1C0200;
            sp6C.unk48 = 0x01000000;
            sp6C.unk42 = 0x60;
            sp6C.unk60 = 0xA5;
            break;
        case 0x351:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk24.z = -0.5f;
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.z = arg2->transl.f[2];
            sp6C.unk30.w = rand_next(0x32, 0x64) * 0.0002f;
            sp6C.unk8 = rand_next(0x28, 0x50);
            sp6C.unk44 = 0x08140200;
            sp6C.unk48 = 0x01000000;
            sp6C.unk42 = 0x5F;
            break;
        case 0x51D:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.transform.yaw = 0x2BC;
            sp6C.unk42 = 0x462;
            sp6C.unk30.x = arg2->transl.f[0];
            sp6C.unk30.y = arg2->transl.f[1];
            sp6C.unk30.w = rand_next(0xA, 0x14) * 0.0001f;
            sp6C.unk8 = 0xAA;
            sp6C.unk44 = 0xA0104;
            sp6C.transform.pitch = 0;
            sp6C.transform.roll = 0;
            sp6C.transform.transl.x = 0.0f;
            sp6C.transform.transl.y = 0.0f;
            sp6C.transform.transl.z = 0.0f;
            sp6C.transform.scale = 1.0f;
            break;
        case 0x55A:
            if (arg2 == NULL) {
                RESET_SRT(_bss_0)
                arg2 = &_bss_0;
            }
            sp6C.unk24.x = rand_next(-0x28, 0x28) * 0.004f;
            sp6C.unk24.y = rand_next(0xA, 0x50) * 0.002f;
            sp6C.unk24.z = rand_next(-0x28, 0x28) * 0.004f;
            sp6C.unk30.w = rand_next(5, 0x19) * 0.00015f;
            sp6C.unk8 = rand_next(0x122, 0x15E);
            sp6C.unk60 = 0xFF;
            sp6C.transform.yaw = rand_next(0, 0xFFFF);
            sp6C.transform.pitch = rand_next(0, 0xFFFF);
            sp6C.transform.yaw = rand_next(0, 0xFFFF);
            sp6C.transform.transl.x = rand_next(0xE6, 0x320);
            sp6C.transform.transl.y = rand_next(0xE6, 0x320);
            sp6C.transform.transl.z = rand_next(0xE6, 0x320);
            sp6C.unk48 = 0x01000020;
            sp6C.unk44 = 0x86000008;
            sp6C.unk58[0] = sp6C.unk4C[0] = rand_next(0, 0xFFF) + 0xF000;
            sp6C.unk58[1] = sp6C.unk4C[1] = 0xE000;
            sp6C.unk58[2] = sp6C.unk4C[2] = 0xE000;
            sp6C.unk42 = 0x567;
            break;
        default:
            return -1;
    }

    sp6C.unk44 |= arg3;
    if ((sp6C.unk44 & 1) && (sp6C.unk44 & 2)) {
        sp6C.unk44 ^= 2;
    }
    if (sp6C.unk44 & 1) {
        if (arg3 & 0x200000) {
            sp6C.unk30.x += sp6C.transform.transl.x;
            sp6C.unk30.y += sp6C.transform.transl.y;
            sp6C.unk30.z += sp6C.transform.transl.z;
        } else if (sp6C.unk0 != NULL) {
            sp6C.unk30.x += sp6C.unk0->positionMirror.x;
            sp6C.unk30.y += sp6C.unk0->positionMirror.y;
            sp6C.unk30.z += sp6C.unk0->positionMirror.z;
        }
    }
    sp62 = gDLL_13_Expgfx->vtbl->func1.withTwoArgsS32(&sp6C, sp30);
    return sp62;
}