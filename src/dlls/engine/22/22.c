#include "dlls/engine/22.h"

/*0x0*/ static const char subtitlesDelayError[] = "subtitles error: sentence has a delay less than %d ms\n";
/*0x38*/ static const char subtitlesNewLineCountError[] = "Can't have more than one new line in a subtitle sentence, not enough space on screen\n";

/*0x0*/ static u32 _data_0[] = {
    0xfefe0274, 0xfefd0275, 0xfefc0276, 0xfefb0277, 0xfefa0278, 0xfef90279, 0xffffffff
};

/*0x1C*/ static StructData1C _data_1C[] = {
    {0x00000438, 0x00cc},
    {0x00000416, 0x00ce},
    {0x00000459, 0x00d1}
};

/*0x34*/ static u32 _data_34 = 0;
/*0x38*/ static u32 _data_38 = 1;
/*0x3C*/ static u32 _data_3C = 0;
/*0x40*/ static s32 _data_40 = 0;
/*0x44*/ static s32 _data_44 = 0;
/*0x48*/ static f32 _data_48 = 0.0f;
/*0x4C*/ static f32 _data_4C = 0.0f;
/*0x50*/ static u8 _data_50 = 0;
/*0x54*/ static u8 _data_54 = 0;
/*0x58*/ static u32 _data_58 = 0;
/*0x5C*/ static u32 _data_5C = 0x01000000;
/*0x60*/ static f32 _data_60 = 0.0f;

/*0x0*/ static s32 sFontID;
/*0x4*/ static s32 sWindowID;
/*0x8*/ static s32 _bss_8;
/*0xC*/ static s32 _bss_C;
/*0x10*/ static StructBss10 _bss_10;
/*0x38*/ static StructBss38 _bss_38[3];
/*0x77C*/ static u8 _bss_77C[0x4];
/*0x780*/ static StructBss38 *_bss_780[3];
/*0x78C*/ static u8 _bss_78C[0x4]; // This and _bss_7A4 may be related.. in a struct?
/*0x790*/ static s32 _bss_790;
/*0x794*/ static s32 _bss_794;
/*0x798*/ static s32 _bss_798;
/*0x79C*/ static s32 _bss_79C;
/*0x7A0*/ static u16 _bss_7A0;
/*0x7A2*/ static u8 _bss_7A2;
/*0x7A3*/ static u8 _bss_7A3;
/*0x7A4*/ static u8 _bss_7A4;
/*0x7A8*/ static s32 _bss_7A8;
/*0x7AC*/ static GameTextChunk *_bss_7AC;
/*0x7B0*/ static u16 _bss_7B0;
/*0x7B2*/ static u8 _bss_7B2[0x2];
/*0x7B4*/ static s32 _bss_7B4;
/*0x7B8*/ static u8 _bss_7B8[0x8];

// offset: 0x0 | ctor
void dll_22_ctor(s32 arg0) {
    StructBss38* var_a1;
    StructBss38* var_v0;
    Texture* temp_v0;
    s32 var_v1;
    s32 i;
    s32 j;

    sFontID = 1;
    sWindowID = 6;
    font_load(1);
    _bss_780[0] = &_bss_38[0];
    _bss_780[1] = &_bss_38[1];
    _bss_780[2] = &_bss_38[2];
    _data_38 = 1;
    dll_22_func_32C(12, 28);
    _data_40 = 0;
    var_a1 = _bss_38;
    var_v1 = 0;
    for (i = 0; i < 3; i++) {
        var_v0 = &_bss_38[i];
        for (j = 0; j < 8; j += 1) {
            var_v0->unkC8[j].unk0 = NULL;
            var_v0->unk188[j].unk0 = NULL;
            var_v0->unkC8[j].unkC = NULL;
            var_v0->unk188[j].unkC = NULL;
            var_v0->unkC8[j].unk8 = NULL;
            var_v0->unk188[j].unk8 = NULL;
            var_v0->unkC8[j].unkA = NULL;
            var_v0->unk188[j].unkA = NULL;
        }
    }
    _bss_10.unk0 = queue_load_texture_proxy(0x27B);
    _bss_10.unkC = queue_load_texture_proxy(0x27C);
    _bss_10.unk18 = 0;
    _bss_10.unk8 = 0;
    _bss_10.unk14 = _bss_10.unk0->width;
    _bss_7A3 = _bss_10.unkC->width + _bss_10.unk0->width;
    _bss_10.unkA = -2;
    _bss_10.unk16 = -2;
    _bss_10.pad4 = 0;
    _bss_10.unk10 = 0;
}

// offset: 0x1D4 | dtor
void dll_22_dtor(s32 arg0) {
    Texture *temp_a0;
    StructBss38 *temp;
    s32 i;
    s32 j;

    font_unload(1);
    for (j = 0; j < 3; j++) {
        temp = &_bss_38[j];
        for (i = 0; i < 8; i ++) {
            temp_a0 = temp->unkC8[i].unk0;
            if (temp_a0 != 0) {
                texture_destroy(temp_a0);
            }
            temp_a0 = temp->unk188[i].unk0;
            if (temp_a0 != 0) {
                texture_destroy(temp_a0);
            }
        }
    }
    texture_destroy(_bss_10.unk0);
    texture_destroy(_bss_10.unkC);
}

// offset: 0x2D0 | func: 0 | export: 0
u32 dll_22_func_2D0(u32 arg0) {
    u32 temp_v0;

    temp_v0 = _data_38;
    _data_38 = arg0;
    return temp_v0;
}


// offset: 0x2F4 | func: 1 | export: 1
int dll_22_func_2F4(void) {
    return _data_34 != 0 && _data_40 != 0;
}

// offset: 0x32C | func: 2 | export: 2
void dll_22_func_32C(u8 arg0, u8 arg1) {
    _bss_790 = arg0;
    _bss_7A2 = arg1;
}

// offset: 0x368 | func: 3 | export: 3
void dll_22_func_368(u16 arg0) {
    if (_data_34 != 0) {
        if (arg0 == _bss_7B0) {
            _data_48 = _bss_780[0]->unk268;
            return;
        }
        dll_22_func_448();
    }
    _data_34 = 1;
    _bss_7A8 = -255;
    _bss_7AC = gDLL_21_gametext->exports->get_chunk(arg0);
    _bss_7B0 = arg0;
}

// offset: 0x448 | func: 4 | export: 4
void dll_22_func_448(void) {
    _data_44 = 0;
    _data_48 = 0;
    _data_4C = 0;
    _data_34 = 0;
    if (_bss_7AC != 0) {
        free(_bss_7AC);
        _bss_7AC = 0;
    }
    _bss_780[0]->unk0 = 0;
}

// offset: 0x4C0 | func: 5 | export: 5
void dll_22_func_4C0(void) {
    if (_data_54 == 0) {
        if (get_masked_buttons(0) & 0x10) {
            *_bss_7B2 = dll_22_func_2D0(1U);
            _data_54 = 1;
        }
    } else if (!(get_masked_buttons(0) & 0x10)) {
        dll_22_func_2D0((u32) *_bss_7B2);
        _data_54 = 0;
    }
}

// offset: 0x578 | func: 6 | export: 6
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22/dll_22_func_578.s")
#else
// Requires dll_22_func_2118 to be static to match
void dll_22_func_578(Gfx **arg0) {
    f32 temp;
    s32 temp_fv1;
    s32 temp_ft5;
    s32 temp_v1;
    s32 temp_v0;

    temp_ft5 = (delayFloat * 100.0f) / 30.0f;
    if (_data_38 != 0) {
        _data_40 += temp_ft5;
        if (_data_40 >= 0x65) {
            _data_40 = 0x64;
        }
    } else {
        _data_40 -= temp_ft5;
        if (_data_40 < 0) {
            _data_40 = 0;
        }
    }
    dll_22_func_8F4();
    if (_data_34 == 0) {
        return;
    }

    temp_v0 = get_some_resolution_encoded();
    _bss_7A0 = temp_v0 & 0xFFFF;
    _bss_79C = _bss_7A0 - (_bss_7A2 * 2);
    _bss_798 = (_bss_7A0 - _bss_79C) / 2;
    if (_data_4C <= 0.0f) {
        temp_fv1 = dll_22_func_16A0();
        if (temp_fv1 == 0.0f) {
            dll_22_func_448();
            return;
        }

        temp_fv1 -= 600.0f;
        _data_4C += 600.0f;
        if (_bss_780[0]->unk26C & 1) {
            _data_44 = 0x258;
            temp_fv1 -= 600.0f;
        }
        _data_48 += temp_fv1;
    }

    if (_data_44 > 0.0f) {
        _data_44 -= dll_22_func_2118();
        dll_22_func_94C();
    } else if (_data_48 > 0.0f) {
        _data_48 -= dll_22_func_2118();
        _data_3C = 0;
        _bss_78C[0] = 0xFF;
        _bss_78C[1] = 0x17;
        _bss_78C[2] = 0;
        _bss_7A4 = 0xC7;
    } else if (_data_4C > 0.0f) {
        _data_4C -= dll_22_func_2118();
        dll_22_func_AA8();
    }
    dll_22_func_C64();
    dll_22_func_D9C(arg0);
}
#endif

// offset: 0x8F4 | func: 7
/* static */ void dll_22_func_8F4(void) {
    _bss_794 = get_some_resolution_encoded() >> 16;
    _bss_794 = (_bss_794 - _bss_790) - 0x18;
}

// offset: 0x94C | func: 8
/* static */ void dll_22_func_94C(void) {
    f32 temp_fv1;

    _data_3C = 0;
    if (_data_44 > 0.0f) {
        temp_fv1 = 600.0f - _data_44;
        _bss_78C[0] = (s32) ((255.0f * temp_fv1) / 600.0f);
        _bss_78C[1] = (s32) ((23.0f * temp_fv1) / 600.0f);
        _bss_78C[2] = 0;
        _bss_7A4 = ((199.0f * temp_fv1) / 600.0f);
        return;
    }
    _bss_78C[0] = 0xFF;
    _bss_78C[1] = 0x17;
    _bss_78C[2] = 0;
    _bss_7A4 = 0xC7;
}

// offset: 0xAA8 | func: 9
/* static */ void dll_22_func_AA8(void) {
    f32 temp_fv1;

    _data_3C = 0;
    if (_data_4C > 0.0f) {
        if (_bss_780[1]->unk0 == 0) {
            _bss_78C[0] = (s32) ((255.0f * _data_4C) / 600.0f);
            _bss_7A4 = (s32) ((199.0f * _data_4C) / 600.0f);
            return;
        }
        temp_fv1 = _data_4C / 600.0f;
        _bss_78C[2] = 0x17 - (s32) (23.0f * temp_fv1);
        if (temp_fv1 > 0.6f) {
            _bss_78C[1] = 0x17;
        } else {
            _bss_78C[1] = (s32) ((((0.6f - temp_fv1) * 232.0f) / 0.6f) + 23.0f);
        }
        if (temp_fv1 > 0.4f) {
            _bss_78C[0] = (s32) (((temp_fv1 - 0.4f) * 255.0f) / 0.6f);
        } else {
            _bss_78C[0] = 0;
        }
        _bss_7A4 = 0xC7;
        return;
    }
    _bss_78C[0] = 0;
    if (_bss_780[1]->unk0 == 0) {
        _bss_7A4 = 0;
        return;
    }
    _bss_7A4 = 0xC7;
    _bss_78C[1] = 0xFF;
    _bss_78C[2] = 0x17;
}

// offset: 0xC64 | func: 10
// Needs to be static for matching
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22/dll_22_func_C64.s")
#else
// WIP
void dll_22_func_C64(void) {
    StructBss38 *var_a0;
    StructBss38 *var_v1;
    StructBss38 *temp;
    s32 j;
    s32 i;
    f32 temp_fv0;
    s32 temp_t6;
    s32 var_v0;
    s8 temp_t2;
    s8 temp_t8;

    temp_fv0 = *_data_60 + delayFloat;
    if (_data_5C != 0) {
        *_data_60 = temp_fv0;
        if (temp_fv0 > 10.0f) {
            _data_58++;
            *_data_60 = (temp_fv0 - 10.0f);
            if (temp_t8 == 3) {
                _data_5C = 0;
            }
        }
    } else {
        *_data_60 = temp_fv0;
        if (temp_fv0 > 10.0f) {
            _data_58--;
            *_data_60 = (temp_fv0 - 10.0f);
            if (temp_t2 == 0) {
                _data_5C = 1;
            }
        }
    }
    var_v1 = _bss_38;
    temp_t6 = _data_58 << 8;
    for (i = 0; i < 3; i++) {
        temp = &_bss_38[i];
        var_a0->unkC8[7] = (Texture *) temp_t6;
        var_a0->unkC8[14] = (Texture *) temp_t6;
        var_a0->unkC8[19] = (Texture *) temp_t6;
        var_a0->unkC8[43] = (Texture *) temp_t6;
        var_a0->unk188[7] = (Texture *) temp_t6;
        var_a0->unk188[14] = (Texture *) temp_t6;
        var_a0->unk188[19] = (Texture *) temp_t6;
        var_a0->unk188[43] = (Texture *) temp_t6;
    }
}
#endif

// offset: 0xD9C | func: 11
// Needs to be static for matching
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22/dll_22_func_D9C.s")
#else
// Requires dll_22_func_10C8 to be static to match
void dll_22_func_D9C(Gfx** gdl) {
    s32 sp6C;
    StructBss38* temp_v0;
    StructBss38** var_v1;
    s32 temp_a2;
    s32 i;

    sp6C = 0;
    if (_data_40 == 0) {
        return;
    }
    
    font_window_use_font(sWindowID, sFontID);
    temp_a2 = _bss_794 + _data_3C;
    _bss_8 = temp_a2;
    _bss_C = _bss_8;
    var_v1 = _bss_780;
    do {
        temp_v0 = *var_v1;
        if (temp_v0->unk0 != 0) {
            _bss_C += 0xC;
        }
        if (temp_v0->unk20 != 0) {
            _bss_C += 0xC;
        }
        var_v1 += 1;
    } while ((u32) var_v1 < (u32) _bss_78C);
    if (temp_a2 < _bss_794) {
        _bss_8 = _bss_794;
    }
    if ((_bss_794 + 0x18) < _bss_C) {
        _bss_C = (_bss_794 + 0x18);
    }
    if (func_80004A4C() == 0) {
        gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 0, (_bss_8 - 2), (_bss_7A0 - 1), (_bss_C + 2));
        for (i = 0; i < _bss_7A0; i += _bss_7A3) {
            func_80037F9C(gdl, &_bss_10, i, _bss_8, 0x7F, 0x7F, 0x7F, (_bss_7A4 * _data_40) / 100);
        }
    }
    font_window_set_coords(sWindowID, _bss_798, _bss_8, _bss_798 + _bss_79C, _bss_C);
    for (i = 0; i < 3; i++) {
        dll_22_func_10C8(&sp6C, i, gdl);
    }
}
#endif

// offset: 0x10C8 | func: 12
// Needs to be static for matching
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22/dll_22_func_10C8.s")
#else
// Requires dll_22_func_1164 to be static to match
void dll_22_func_10C8(s32 *arg0, u8 arg1, Gfx **arg2) {
    *arg0 = 0;
    if (dll_22_func_1164(0, arg1, 0, arg2) != 0) {
        *arg0 = *arg0 + 1;
        if (dll_22_func_1164(*arg0,  arg1, 1, arg2) != 0) {
            *arg0 += 1;
        }
    }
}

#endif

// offset: 0x1164 | func: 13
// Needs to be static for matching
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22/dll_22_func_1164.s")
#else
#endif

// offset: 0x16A0 | func: 14
// Needs to be static for matching
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22/dll_22_func_16A0.s")
#else
// Requires dll_22_func_1798 to be static to match
f32 dll_22_func_16A0(void)
{
  StructBss38 *temp_a2;
  int new_var;
  new_var = 0;
  if (_bss_780[0]->unk0 == 0)
  {
    _bss_7B4 = new_var;
    dll_22_func_1798(_bss_780[0], &_bss_7B4);
    dll_22_func_1798(_bss_780[1], &_bss_7B4);
    dll_22_func_1798(_bss_780[2], &_bss_7B4);
  }
  else
  {
    temp_a2 = _bss_780[0];
    _bss_780[0] = _bss_780[1];
    _bss_780[1] = _bss_780[2];
    _bss_780[2] = temp_a2;
    dll_22_func_1798(temp_a2, &_bss_7B4);
  }
  return _bss_780[0]->unk268;
}

#endif

// offset: 0x1798 | func: 15
// Needs to be static for matching
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22/dll_22_func_1798.s")
#else
#endif

// offset: 0x1F44 | func: 16
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22/dll_22_func_1F44.s")
#else
// WIP
void dll_22_func_1F44(StructBss38 *arg0, const char *text, s32 arg2, u8 arg3, u8 arg4) {
    u8 *temp_t0;
    u16 *temp_v1;

    temp_t0 = &arg0->unk0[(arg3 << 5) + (arg4 * 4)];
    temp_t0 = text;
    temp_v1 = arg0->unk0[(arg3 * 0x10) + (arg4 * 2)];
    temp_v1[0x80] = (u16) (temp_v1[0x80] + font_get_text_width(0, text, 0, sFontID));
    dll_22_func_2000(&temp_t0[0x40], arg2);
}
#endif

// offset: 0x2000 | func: 17
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22/dll_22_func_2000.s")
#else
// Requires dll_22_func_20CC to be static to match
void dll_22_func_2000(u8 *arg0, s32 arg1) {
    s32 var_s0;

    _bss_7A8 = arg1;
    arg1 = -arg1;
    
    switch (arg1 & 3) {
    case 0:
        arg0[3] = 0x3F;
        break;
    case 1:
        arg0[3] = 0x7F;
        break;
    case 2:
        arg0[3] = 0xBF;
        break;
    default:
        // @fake
        if (1) {}
        arg0[3] = 0xFF;
        break;
    }
    for (var_s0 = 2; var_s0 >= 0; var_s0--) {
        arg1 >>= 2;
        arg0[var_s0] = dll_22_func_20CC((u8) arg1 & 3);
    }
}
#endif

// offset: 0x20CC | func: 18
u8 dll_22_func_20CC(u8 arg0) {
    switch (arg0) {
    case 0:
        return 0;
    case 1:
        return 0x3F;
    case 2:
        return 0x7F;
    default:
        return 0xFF;
    }
}

// offset: 0x2118 | func: 19
f32 dll_22_func_2118(void) {
    return (1000.0f * delayFloat) / 60.0f;
}

// offset: 0x214C | func: 20 | export: 7
s16 dll_22_func_214C(s32 arg0) {
    u32 var_v1;

    for (var_v1 = 0; var_v1 < 3 && _data_1C[var_v1].unk0 != arg0; var_v1++) {
    }
    if (var_v1 == 3) {
        return 0xCD;
    }
    return _data_1C[var_v1].unk4;
}

// offset: 0x21C0 | func: 21 | export: 8
s32 dll_22_func_21C0(s32 arg0, s16 arg1) {
    u32 var_v1;

    for (var_v1 = 0; var_v1 < 3 && _data_1C[var_v1].unk0 != arg0; var_v1++) {
    }
    if (var_v1 == 3) {
        return 0;
    }

    _data_1C[var_v1].unk4 = arg1;
    return 1;
}

// offset: 0x2248 | func: 22 | export: 9
s32 dll_22_func_2248(u8 arg0) {
    _data_50 = arg0 & 0xFF;
    return 1;
}
