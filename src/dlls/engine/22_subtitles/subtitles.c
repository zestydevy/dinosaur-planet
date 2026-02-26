#include "PR/ultratypes.h"
#include "dlls/engine/21_gametext.h"
#include "PR/gbi.h"

#include "sys/camera.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/textable.h"
#include "sys/joypad.h"
#include "sys/fonts.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "dll.h"
#include "functions.h"
#include "sys/rcp.h"
#include "types.h"

#include "prevent_bss_reordering.h"

// Size: 0x18
typedef struct InnerBss38 {
    Texture *unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 pad10;
    s32 pad14;
} InnerBss38;

// Size: 0x26C
typedef struct StructBss38 {
/*0x00*/ char *unk0[2][8];
/*0x40*/ s32 unk40[2][8];
/*0x80*/ u16 unk80[2][8];
/*0xA0*/ u16 unkA0[2][8];
/*0xC0*/ s16 unkC0[4];
/*0xC8*/ InnerBss38 unkC8[2][8];
/*0x248*/ u16 unk248;
/*0x24C*/ u8 pad24C[0x268 - 0x24A];
/*0x268*/ u16 unk268;
/*0x26A*/ u16 unk26A;
} StructBss38;

typedef struct StructData1C {
    s32 objectID;
    s16 gametextID;
} StructData1C; //Mind Read text for particular Objects?

/*0x0*/ static const char subtitlesDelayError[] = "subtitles error: sentence has a delay less than %d ms\n";
/*0x38*/ static const char subtitlesNewLineCountError[] = "Can't have more than one new line in a subtitle sentence, not enough space on screen\n";

/*0x0*/ static u32 _data_0[] = {
    0xfefe0274, 0xfefd0275, 0xfefc0276, 0xfefb0277, 0xfefa0278, 0xfef90279, 0xffffffff
};

/*0x1C*/ static StructData1C _data_1C[] = { {0x00000438, 0x00cc}, {0x00000416, 0x00ce}, {0x00000459, 0x00d1}
};

/*0x34*/ static u32 _data_34 = 0;
/*0x38*/ static u32 _data_38 = 1;
/*0x3C*/ static u32 _data_3C = 0;
/*0x40*/ static s32 _data_40 = 0; // something for opacity of printed text
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
/*0x10*/ static Func_80037F9C_Struct _bss_10[3];
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
/*0x7A8*/ static s32 currentColourCommand;
/*0x7AC*/ static GameTextChunk *_bss_7AC;
/*0x7B0*/ static u16 _bss_7B0;
/*0x7B2*/ static u8 _bss_7B2[0x2];
/*0x7B4*/ static s32 _bss_7B4;
/*0x7B8*/ static u8 _bss_7B8[0x8];

void dll_22_func_448(void);
f32 dll_22_func_16A0(void);
void dll_22_func_8F4();
void dll_22_func_32C(u8 arg0, u8 arg1);
void dll_22_func_94C(void);
void dll_22_func_AA8(void);
void dll_22_func_C64(void);
void dll_22_func_D9C(Gfx **gdl);
void dll_22_func_10C8(s32 *arg0, u8 arg1, Gfx **arg2);
s32 dll_22_func_1164(s32, u8, s32, Gfx **gdl);
void dll_22_func_1798(StructBss38 *arg0, s32 *arg1);
static void dll_22_func_2000(u8 *arg0, s32 arg1);
static u8 dll_22_func_20CC(u8 arg0);
f32 dll_22_func_2118(void);

// offset: 0x0 | ctor
void dll_22_ctor(s32 arg0) {
    StructBss38 *var_a1;
    StructBss38 *var_v0;
    Texture *temp_v0;
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
            var_v0->unkC8[0][j].unk0 = NULL;
            var_v0->unkC8[1][j].unk0 = NULL;
            var_v0->unkC8[0][j].unkC = NULL;
            var_v0->unkC8[1][j].unkC = NULL;
            var_v0->unkC8[0][j].unk8 = NULL;
            var_v0->unkC8[1][j].unk8 = NULL;
            var_v0->unkC8[0][j].unkA = NULL;
            var_v0->unkC8[1][j].unkA = NULL;
        }
    }
    _bss_10[0].unk0 = tex_load_deferred(TEXTABLE_27B_SubtitleScaleyBackground1);
    _bss_10[1].unk0 = tex_load_deferred(TEXTABLE_27C_SubtitleScaleyBackground2);
    _bss_10[2].unk0 = NULL;
    _bss_10[0].unk8 = 0;
    _bss_10[1].unk8 = _bss_10[0].unk0->width;
    _bss_7A3 = _bss_10[0].unk0->width + _bss_10[1].unk0->width;
    _bss_10[0].unkA = -2;
    _bss_10[1].unkA = -2;
    _bss_10[0].unk4 = 0;
    _bss_10[1].unk4 = 0;
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
        for (i = 0; i < 8; i++) {
            temp_a0 = temp->unkC8[0][i].unk0;
            if (temp_a0 != 0) {
                tex_free(temp_a0);
            }
            temp_a0 = temp->unkC8[1][i].unk0;
            if (temp_a0 != 0) {
                tex_free(temp_a0);
            }
        }
    }
    tex_free(_bss_10[0].unk0);
    tex_free(_bss_10[1].unk0);
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
    currentColourCommand = -255;
    _bss_7AC = gDLL_21_Gametext->vtbl->get_chunk(arg0);
    _bss_7B0 = arg0;
}

// offset: 0x448 | func: 4 | export: 4
void dll_22_func_448(void) {
    _data_44 = 0;
    _data_48 = 0;
    _data_4C = 0;
    _data_34 = 0;
    if (_bss_7AC != 0) {
        mmFree(_bss_7AC);
        _bss_7AC = 0;
    }
    _bss_780[0]->unk0[0][0] = 0;
}

// offset: 0x4C0 | func: 5 | export: 5
void dll_22_func_4C0(void) {
    if (_data_54 == 0) {
        if (joy_get_buttons(0) & R_TRIG) {
            *_bss_7B2 = dll_22_func_2D0(1U);
            _data_54 = 1;
        }
    } else if (!(joy_get_buttons(0) & R_TRIG)) {
        dll_22_func_2D0((u32)*_bss_7B2);
        _data_54 = 0;
    }
}

// offset: 0x578 | func: 6 | export: 6
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22_subtitles/dll_22_func_578.s")
#else
// Requires dll_22_func_2118 to be static to match
void dll_22_func_578(Gfx **gdl) {
    f32 temp;
    s32 temp_fv1;
    s32 temp_ft5;
    s32 temp_v1;
    s32 temp_v0;

    temp_ft5 = (gUpdateRateF * 100.0f) / 30.0f;
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

    temp_v0 = vi_get_current_size();
    _bss_7A0 = GET_VIDEO_WIDTH(temp_v0);
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
        if (_bss_780[0]->unk26A & 1) {
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
    dll_22_func_D9C(gdl);
}
#endif

// offset: 0x8F4 | func: 7
/* static */ void dll_22_func_8F4(void) {
    _bss_794 = GET_VIDEO_HEIGHT(vi_get_current_size());
    _bss_794 = (_bss_794 - _bss_790) - 0x18;
}

// offset: 0x94C | func: 8
/* static */ void dll_22_func_94C(void) {
    f32 temp_fv1;

    _data_3C = 0;
    if (_data_44 > 0.0f) {
        temp_fv1 = 600.0f - _data_44;
        _bss_78C[0] = (s32)((255.0f * temp_fv1) / 600.0f);
        _bss_78C[1] = (s32)((23.0f * temp_fv1) / 600.0f);
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
        if (_bss_780[1]->unk0[0][0] == 0) {
            _bss_78C[0] = (s32)((255.0f * _data_4C) / 600.0f);
            _bss_7A4 = (s32)((199.0f * _data_4C) / 600.0f);
            return;
        }
        temp_fv1 = _data_4C / 600.0f;
        _bss_78C[2] = 0x17 - (s32)(23.0f * temp_fv1);
        if (temp_fv1 > 0.6f) {
            _bss_78C[1] = 0x17;
        } else {
            _bss_78C[1] = (s32)((((0.6f - temp_fv1) * 232.0f) / 0.6f) + 23.0f);
        }
        if (temp_fv1 > 0.4f) {
            _bss_78C[0] = (s32)(((temp_fv1 - 0.4f) * 255.0f) / 0.6f);
        } else {
            _bss_78C[0] = 0;
        }
        _bss_7A4 = 0xC7;
        return;
    }
    _bss_78C[0] = 0;
    if (_bss_780[1]->unk0[0][0] == 0) {
        _bss_7A4 = 0;
        return;
    }
    _bss_7A4 = 0xC7;
    _bss_78C[1] = 0xFF;
    _bss_78C[2] = 0x17;
}

// offset: 0xC64 | func: 10
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22_subtitles/dll_22_func_C64.s")
#else
// WIP
/* static */ void dll_22_func_C64(void) {
    StructBss38 *temp;
    s32 j;
    s32 i;
    f32 temp_fv0;
    s32 temp_t2;

    temp_fv0 = _data_60;
    temp_fv0 += gUpdateRateF;
    if (_data_5C != 0) {
        _data_60 = temp_fv0;
        if (temp_fv0 > 10.0f) {
            temp_t2 = (s8)(_data_58 + 1);
            _data_58 = temp_t2;
            temp_fv0 -= 10.0f;
            _data_60 = temp_fv0;
            if ((s8)temp_t2 == 3) {
                _data_5C = 0;
            }
        }
    } else {
        _data_60 = temp_fv0;
        if (temp_fv0 > 10.0f) {
            temp_t2 = (s8)(_data_58 - 1);
            _data_58 = temp_t2;
            temp_fv0 -= 10.0f;
            _data_60 = temp_fv0;
            if ((s8)temp_t2 == 0) {
                _data_5C = 1;
            }
        }
    }
    temp_t2 = _data_58 << 8;
    for (i = 0; i < 3; i++) {
        temp = &_bss_38[i];
        for (j = 0; j < 8; j += 1) {
            temp->unkC8[0][j].unk4 = temp_t2;
        }
    }
}
#endif

// offset: 0xD9C | func: 11
// Needs to be static for matching
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22_subtitles/dll_22_func_D9C.s")
#else
// Requires dll_22_func_10C8 to be static to match
void dll_22_func_D9C(Gfx **gdl) {
    s32 sp6C;
    StructBss38 *temp_v0;
    StructBss38 **var_v1;
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
        if (temp_v0->unk0[0][0] != 0) {
            _bss_C += 0xC;
        }
        if (temp_v0->unk0[1][0] != 0) {
            _bss_C += 0xC;
        }
        var_v1 += 1;
    } while ((u32)var_v1 < (u32)_bss_78C);
    if (temp_a2 < _bss_794) {
        _bss_8 = _bss_794;
    }
    if ((_bss_794 + 0x18) < _bss_C) {
        _bss_C = (_bss_794 + 0x18);
    }
    if (camera_get_letterbox() == 0) {
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
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22_subtitles/dll_22_func_10C8.s")
#else
// Requires dll_22_func_1164 to be static to match
void dll_22_func_10C8(s32 *arg0, u8 arg1, Gfx **arg2) {
    *arg0 = 0;
    if (dll_22_func_1164(0, arg1, 0, arg2) != 0) {
        *arg0 = *arg0 + 1;
        if (dll_22_func_1164(*arg0, arg1, 1, arg2) != 0) {
            *arg0 += 1;
        }
    }
}

#endif

// offset: 0x1164 | func: 13
// Needs to be static for matching
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22_subtitles/dll_22_func_1164.s")
#else
#endif

// offset: 0x16A0 | func: 14
// Needs to be static for matching
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22_subtitles/dll_22_func_16A0.s")
#else
// Requires dll_22_func_1798 to be static to match
f32 dll_22_func_16A0(void) {
    StructBss38 *temp_a2;
    int new_var;
    new_var = 0;
    if (_bss_780[0]->unk0[0][0] == 0) {
        _bss_7B4 = new_var;
        dll_22_func_1798(_bss_780[0], &_bss_7B4);
        dll_22_func_1798(_bss_780[1], &_bss_7B4);
        dll_22_func_1798(_bss_780[2], &_bss_7B4);
    } else {
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
// Needs to be static for matching + more
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/22_subtitles/dll_22_func_1798.s")
#else
#define SPACE_CHAR 0x20
static void dll_22_func_1F44(StructBss38 *arg0, char *text, s32 arg2, u8 arg3, u8 arg4);
static void dll_22_func_2000(u8 *arg0, s32 arg1);
void dll_22_func_1798(StructBss38 *arg0, s32 *arg1) {
    s16 var_s0_2;
    u8 sp5D;
    s16 var_s1;
    s16 var_t0; // sp58
    Texture *tex;
    s16 var_v1;
    u8 var_s0;
    u8 var_s4;
    s32 *var_v0;
    u16 temp_v1_3;
    StructData0 *temp_v0_2;
    s16 var_a2;
    StructBss38 *var_a1;
    StructBss38 *sp4C;
    StructBss38 *sp48;

    sp5D = 1;
    var_s4 = 0;
    for (var_s1 = 0; var_s1 < 8; var_s1++) {
        // bad C code, fix later
        ((s32 *)&arg0->unk0[0])[var_s1] = 0;
        ((s32 *)&arg0->unk0[1])[var_s1] = 0;
        arg0->unk80[0].unk0[var_s1] = 0;
        arg0->unk80[1].unk0[var_s1] = 0;
        tex = arg0->unkC8[var_s1].unk0;
        if (tex != NULL) {
            tex_free(tex);
            arg0->unkC8[var_s1].unk0 = NULL;
        }
        tex = arg0->unk188[var_s1].unk0;
        if (tex != NULL) {
            tex_free(tex);
            arg0->unk188[var_s1].unk0 = NULL;
        }
    }
    arg0->unkC0[0] = 0;
    arg0->unkC0[1] = 0;
    arg0->unkC0[2] = 0;
    arg0->unkC0[3] = 0;
    if (_bss_7AC->count == *arg1) {
        arg0->unk268 = 0;
        arg0->unk26A = 0;
        return;
    }
    arg0->unk268 = _bss_7AC->unk4[*arg1];
    if (*arg1 == 0) {
        arg0->unk26A = 1;
    } else {
        arg0->unk26A = 0;
    }
    dll_22_func_1F44(arg0, _bss_7AC->strings[(*arg1)++], _bss_7A8, 0, 0);
    var_s0 = 1;
    while ((_bss_7AC->unk4[*arg1] <= 0) && (_bss_7AC->count != *arg1)) {
        if (_bss_7AC->unk4[*arg1] == 0) {
            var_s0 = 1;
            var_s4++;
            dll_22_func_1F44(arg0, _bss_7AC->strings[(*arg1)++], _bss_7A8, var_s4, 0);
        } else if (_bss_7AC->unk4[*arg1] >= -255) {
            dll_22_func_1F44(arg0, _bss_7AC->strings[*arg1], _bss_7AC->unk4[(*arg1)++], var_s4, var_s0++);
        } else if (_bss_7AC->unk4[*arg1] == -256) {
            // ??????????????
            sp48 = (StructBss38 *)((s16 *)arg0 + var_s4);
            dll_22_func_1F44(arg0, _bss_7AC->strings[(*arg1)++], _bss_7A8, var_s4, var_s0++);
            sp48->unkC0[2] = 6;
        } else if (_bss_7AC->unk4[*arg1] == -257) {
            sp5D = 0;
            dll_22_func_1F44(arg0, _bss_7AC->strings[(*arg1)++], _bss_7A8, var_s4, var_s0++);
        } else {
            arg0->unk80[var_s4][var_s0] = 0x18;
            var_s1 = 0;
            do {
                temp_v0_2 = &_data_0[var_s1++];
            } while (temp_v0_2->unk0 != (_bss_7AC->unk4[*arg1]));
            tex = tex_load_deferred(temp_v0_2->unk2);
            arg0->unkC8[var_s0][0].unk0 = tex;
            dll_22_func_1F44(arg0, _bss_7AC->strings[(*arg1)++], _bss_7A8, var_s4, var_s0++);
        }
    }
    if (var_s4 == 0) {
        var_t0 = 0;
        for (var_s1 = 0; var_s1 < 8;) {
            if (arg0->chars[var_s1] == NULL) {
                var_s1--;
                break;
            }
            else {
                // removing the else might cause s register swap
                var_t0 += arg0->unk80[0].unk0[var_s1];
                var_s1++;
            }
        }
        if (_bss_79C < var_t0) {
            var_s0_2 = strlen(arg0->chars[var_s1]) + 1;
            while (arg0->chars[var_s1][var_s0_2] != SPACE_CHAR) {
                var_s0_2--;
            }
            arg0->chars[var_s1][var_s0_2] = 0;
            var_v1 = arg0->unk80[var_s1].unk0[0] - font_get_text_width(0, arg0->chars[var_s1], 0, sFontID);
            while (_bss_79C < (var_t0 - var_v1)) {
                arg0->chars[var_s1][var_s0_2] = SPACE_CHAR;

                var_s0_2 -= 1;
                while (arg0->chars[var_s1][var_s0_2] != SPACE_CHAR)
                {
                    var_s0_2--;
                }
                arg0->chars[var_s1][var_s0_2] = 0;

                var_v1 = arg0->unk80[var_s1].unk0[0] - font_get_text_width(0, arg0->chars[var_s1], 0, sFontID);
            }
            arg0->unk80[var_s1].unk0[0] -= var_v1;
            arg0->unk0[1].unk0 = arg0->chars[var_s1] + var_s0_2 + 1;
            arg0->unk80[1][0] = var_v1;
            dll_22_func_2000(arg0->unk60, _bss_7A8);
        }
    }
    for (var_s4 = 0; var_s4 < 2; var_s4++) {
        var_t0 = 0;
        var_s0 = 0;
        if (sp5D != 0) {
            for (; var_s0 < 8; var_s0++) {
                var_t0 += arg0->unk80[var_s4].unk0[var_s0];
            }
            arg0->unkC0[var_s4] = (_bss_79C - var_t0) / 2;
        }
        arg0->unkA0[var_s4].unk0[0] = 0;
        for (var_s0 = 1; var_s0 < 8; var_s0++) {
            // @diff, this should load from arg0?
            arg0->unkA0[var_s4].unk0[var_s0] = arg0->unk80[var_s0].unk0[-1] + arg0->unk80[var_s0].unk0[15];
        }
        var_a1 = (StructBss38 *)(((s32 *)arg0) + (var_s4 * 0x30));
        for (var_s0 = 0; var_s0 < 8; var_s0++) {
            if (var_a1->unkC8[var_s0].unk0 != 0) {
                temp_v1_3 = arg0->unkA0[var_s4].unk0[var_s0];
                arg0->unkA0[var_s4].unk0[var_s0] += 0x18;
                arg0->unk248 = temp_v1_3;
            }
        }
    }
}
#endif

// offset: 0x1F44 | func: 16
void dll_22_func_1F44(StructBss38 *arg0, char *text, s32 arg2, u8 arg3, u8 arg4) {
    arg0->unk0[arg3][arg4] = text;
    arg0->unk80[arg3][arg4] += font_get_text_width(0, text, 0, sFontID);
    dll_22_func_2000((u8 *)&arg0->unk40[arg3][arg4], arg2);
}

// offset: 0x2000 | func: 17
/** Converts a Gametext RGBA8 colour command to an RGBA32 colour */
static void dll_22_func_2000(u8 *colour_rgba32, s32 rgba8) {
    s32 componentIndex;

    currentColourCommand = rgba8;
    rgba8 = -rgba8;

    switch (rgba8 & 3) {
        case 0:
            colour_rgba32[3] = 63;
            break;
        case 1:
            colour_rgba32[3] = 127;
            break;
        case 2:
            colour_rgba32[3] = 191;
            break;
        default:
            colour_rgba32[3] = 255;
    }
    for (componentIndex = 2; componentIndex >= 0; componentIndex--) {
        rgba8 >>= 2;
        colour_rgba32[componentIndex] = dll_22_func_20CC(rgba8 & 3);
    }
}

// offset: 0x20CC | func: 18
/** Converts a 2-bit RGB colour component value to an 8bit value */
static u8 dll_22_func_20CC(u8 colourComponent) {
    switch (colourComponent) {
        case 0:
            return 0;
        case 1:
            return 63;
        case 2:
            return 127;
        default:
            return 255;
    }
}

// offset: 0x2118 | func: 19
f32 dll_22_func_2118(void) {
    return (1000.0f * gUpdateRateF) / 60.0f;
}

// offset: 0x214C | func: 20 | export: 7
s16 dll_22_func_214C(s32 arg0) {
    u32 var_v1;

    for (var_v1 = 0; var_v1 < 3 && _data_1C[var_v1].objectID != arg0; var_v1++) {
    }
    if (var_v1 == 3) {
        return 0xCD;
    }
    return _data_1C[var_v1].gametextID;
}

// offset: 0x21C0 | func: 21 | export: 8
// Set Mind Read text for object?
s32 dll_22_func_21C0(s32 objectID, s16 gametextID) {
    u32 index;

    for (index = 0; index < 3 && _data_1C[index].objectID != objectID; index++) {
    }
    if (index == 3) {
        return 0;
    }

    _data_1C[index].gametextID = gametextID;
    return 1;
}

// offset: 0x2248 | func: 22 | export: 9
s32 dll_22_func_2248(u8 arg0) {
    _data_50 = arg0 & 0xFF;
    return 1;
}
