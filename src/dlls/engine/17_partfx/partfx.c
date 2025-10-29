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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/17_partfx/dll_17_func_D74.s")
