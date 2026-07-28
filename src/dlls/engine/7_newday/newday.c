#include "dlls/engine/17_partfx.h"
#include "dlls/engine/7_newday.h"
#include "dlls/objects/332_FXEmit.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/texture.h"
#include "sys/curves.h"
#include "sys/envfx.h"
#include "sys/lighting.h"
#include "sys/newshadows.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/rand.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/vi.h"
#include "dll.h"

#define DAYTIME 18000.0f //5am
#define NIGHTTIME 75600.0f //9pm
#define NOON 12.0 * 60.0 * 60.0
#define SECONDS_IN_DAY 86400.0f
#define SECONDS_IN_HOUR (60 * 60)
#define SECONDS_IN_MINUTE (60)
#define TICKS_IN_HOUR (SECONDS_IN_HOUR * 60)
#define TICKS_IN_MINUTE (SECONDS_IN_MINUTE * 60)
#define TICKS_IN_SECOND (60)

/*0x0*/ static f32 _data_0 = 0.0;
/*0x4*/ static u32 _data_4 = 0x00000000;
/*0x8*/ static f32 _data_8 = 0.0;
/*0xC*/ static f32 _data_C = 0.0;
/*0x10*/ static s32 _data_10 = 0;
/*0x14*/ static f32 _data_14 = 0.1f;
/*0x18*/ static Vec3f _data_18 = VEC3F(0.0f, 1.0f, 0.0f);
/*0x24*/ static Vec3f _data_24 = VEC3F(0.0f, 1.0f, 0.0f);
/*0x30*/ static u32 _data_30 = 0xffffffff;
/*0x34*/ static u32 _data_34 = 255; //clouds R
/*0x38*/ static u32 _data_38 = 255; //clouds G
/*0x3C*/ static u32 _data_3C = 255; //clouds B
/*0x40*/ static u32 _data_40 = 0x00000000;
/*0x44*/ static u32 _data_44 = 0x00000000;
/*0x48*/ static u32 _data_48 = 0x00000000;
/*0x4C*/ static u32 _data_4C = 0x00000000;
/*0x50*/ static u32 _data_50 = 0x00000000;
/*0x54*/ static u32 _data_54 = 0x00000000;
/*0x58*/ static u8 _data_58 = 0;
/*0x5C*/ static f32 _data_5C = NOON; //time of day
/*0x60*/ static u32 _data_60 = 0x00000000;
/*0x64*/ static u32 _data_64 = 0x00000000;
/*0x68*/ static s16 _data_68 = 0;
         // splines
/*0x6C*/ static f32 _data_6C[][7] = {
    {55, 55, 100, 210, 125, 55, 55}, 
    {55, 55, 60,  210, 120, 55, 55}, 
    {55, 55, 60,  220, 240, 55, 55}
};
/*0xC0*/ static Object* _data_C0 = NULL;
/*0xC4*/ static f32 _data_C4 = 1.0;
/*0xC8*/ static u32 _data_C8 = 0x00000000;
/*0xCC*/ static u32 _data_CC[] = {
    0x001da400, 0x00489b44, 0x1d0c351c, 0xff8fbfff, 0x74baffdb, 0xffffb0ff, 0xffffffe8, 0xd3ff82ff, 
    0xff4fa3ff, 0xb4ffff6f, 0xa7ffffff, 0xa5f5b78c, 0xffcd00ff, 0x9800ff81, 0x24f26021, 0x99356b68, 
    0x26660b00, 0x45070041, 0xffffffff, 0xffffca00, 0xfe4d0061, 0xff74c862, 0x003b65e0, 0x7f00532c, 
    0xfefe1300, 0x6926fffe, 0x26cd2d3d, 0xfffd00a9, 0x1339fefe, 0xfe0879d0, 0xce0000ff, 0xa100fffe, 
    0xe25c833f, 0xffff93b4, 0x5b43fffe, 0xfed23882, 0xff00007a, 0x11019800, 0x95240057, 0xff48ff65, 
    0x0665ffe6, 0x83ffb02f, 0xfe00003f, 0x00005c8e, 0xff00005c, 0x99c6ff00, 0x25acffff, 0xff3535ff, 
    0x8080537a, 0x6046c900, 0xfe4d0061, 0x1e4155c3, 0xdbf41e41, 0x55cbdb85, 0x1e41556f, 0x0c861e41, 
    0x55318ad8, 0x1e4155ff, 0xd5511e41, 0x55ff0c00
};
/*0x1BC*/ static u32 _data_1BC = 0x00000000;
/*0x1C0*/ static u32 _data_1C0 = 0x00000000;
/*0x1C4*/ static u32 _data_1C4 = 0x00000000;
/*0x1C8*/ static f32 _data_1C8 = 0.0;
/*0x1CC*/ static u32 _data_1CC[] = {
    0x00000003, 0x00020000, 0x001f0000, 0x0000000f, 0x000f0003, 0x00040002, 0x00000000, 0x001f0000, 
    0x000f000f, 0x00040001, 0x0002001f, 0x0000001f, 0x001f000f, 0x000f0001, 0x00000002, 0x001f001f, 
    0x0000001f, 0x000f000f
};
/*0x214*/ static u32 _data_214[] = {
    0xffec0014, 0x00000000, 0x03e00014, 0x00140000, 0x03e003e0, 0x00000000, 0x000001e0, 0x01e0ffec, 
    0xffec0000, 0x00000000, 0x0014ffec, 0x000003e0, 0x00000000
};
/*0x248*/ static u32 _data_248[] = {
    0xffec0014, 0x00000000, 0x07e00014, 0x00140000, 0x07e007e0, 0x00000000, 0x000003e0, 0x03e0ffec, 
    0xffec0000, 0x00000000, 0x0014ffec, 0x000007e0, 0x00000000
};
/*0x27C*/ static f32 _data_27C[] = {
    0, 0, 4600
};
/*0x288*/ static f32 _data_288[] = {
    0, 0, 4600
};

typedef struct
{
/*000*/ void *unk0;
/*004*/ void *unk4;
/*008*/ Texture *unk8;
/*00C*/ Texture *unkC;
/*010*/ Texture *unk10;
/*014*/ Texture *unk14;
/*018*/ Texture *unk18;
/*01C*/ Texture *unk1C;
/*020*/ UNK_TYPE_32 unk20;
/*024*/ u8 _unk20[8];
/*02C*/ f32 unk2C;
/*030*/ f32 unk30;
/*034*/ f32 unk34;
/*038*/ u8 _unk38[8];
/*040*/ f32 unk40;
/*044*/ f32 unk44[7]; // spline
/*060*/ f32 unk60[7]; // spline
/*07C*/ f32 unk7C[7]; // spline
/*098*/ u8 _unk98[0xc0 - 0x98];
/*0C0*/ f32 timeSeconds; //time of day (seconds)
/*0C4*/ f32 unkC4;
/*0C8*/ f32 unkC8;
/*0CC*/ UNK_TYPE_32 unkCC;
/*0D0*/ UNK_TYPE_32 unkD0;
/*0D4*/ u8 _unkD4[8];
/*0DC*/ UNK_TYPE_32 unkDC;
/*0E0*/ UNK_TYPE_32 unkE0;
/*0E4*/ s32 unkE4;
/*0E8*/ UNK_TYPE_32 unkE8;
/*0EC*/ UNK_TYPE_32 unkEC;
/*0F0*/ UNK_TYPE_32 unkF0;
/*0F4*/ UNK_TYPE_32 unkF4;
/*0F8*/ UNK_TYPE_32 unkF8;
/*0FC*/ UNK_TYPE_32 unkFC;
/*100*/ UNK_TYPE_32 unk100;
/*104*/ UNK_TYPE_32 unk104;
/*108*/ UNK_TYPE_32 unk108;
/*10C*/ UNK_TYPE_32 unk10C;
/*110*/ u8 unk110;
/*111*/ u8 unk111;
/*112*/ u8 _unk112[2];
/*114*/ UNK_TYPE_8 unk114;
/*115*/ u8 _unk115[3];
} NewDayStruct;

// size: 0x28
typedef struct {
    u16* unk0;
    u16* unk4;
    u16* unk8;
    u16* unkC;
    u16* unk10;
    u16* unk14;
    u16 unk18;
    u16 unk1A;
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u16 unk22;
    s8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
} BSS0;

/*0x0*/ static BSS0 _bss_0;
/*0x28*/ static u8 _bss_28[0x4]; // DAT_810296c8
/*0x2C*/ static u8 _bss_2C[0x4]; // DAT_810296cc
/*0x30*/ static NewDayStruct *_bss_30; // PTR_810296d0
/*0x34*/ static u8 _bss_34[0x28];

// offset: 0x0 | ctor
void dll_7_ctor(void *self) { }

// offset: 0xC | dtor
void dll_7_dtor(void *self) { }

// offset: 0x18 | func: 0 | export: 0
void dll_7_func_18(Object *arg0, Object *arg1, void *arg2, s32 arg3);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_18.s")

// offset: 0x910 | func: 1 | export: 1
void dll_7_func_910(void);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_910.s")

// offset: 0xA10 | func: 2 | export: 2
void dll_7_func_A10(void);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_A10.s")

// offset: 0xC58 | func: 3 | export: 3
void dll_7_func_C58(Gfx **gdl, Mtx **arg1, u8 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_C58.s")

// offset: 0xCDC | func: 4 | export: 4
void dll_7_func_CDC(f32* timeSeconds) {
    if (_bss_30 != NULL) {
        *timeSeconds = _bss_30->timeSeconds;
    }
}

// offset: 0xD08 | func: 5 | export: 5
void dll_7_func_D08(f32* arg0);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_D08.s")

// offset: 0xD50 | func: 6 | export: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_D50.s")

// offset: 0xDAC | func: 7 | export: 7
void dll_7_func_DAC(s32 *param1) {
    if (_bss_30 != NULL) {
        *param1 = (f32)_bss_30->unkE4;
    } else {
        *param1 = 0;
    }
}

/** 
 * offset: 0xDF4 | func: 8 | export: 8
 * Checks if nighttime
 * Also sets the argument float to the seconds until nighttime/daytime
 */
s32 dll_7_func_DF4(f32* outTime) {
    if (_bss_30 == NULL) {
        *outTime = 0.0f;
        return FALSE;
    }

    //If night
    if (_data_5C >= NIGHTTIME || _data_5C < DAYTIME) {
        if (_data_5C >= NIGHTTIME) {
            *outTime = (_data_5C - NIGHTTIME) + DAYTIME; //seconds into nighttime (added to base daytime?)
        } else {
            *outTime = DAYTIME - _data_5C; //seconds until daytime (when time in 0-to-daytime range)
        }

        return TRUE;
    }

    //If day
    *outTime = NIGHTTIME - _data_5C; //seconds until nighttime
    return FALSE;
}

/**
 * offset: 0xEA0 | func: 9 | export: 9
 * Sets the current time (in seconds), wrapping the value into a 24h range
 */
void dll_7_func_EA0(f32 time) {
    f32 tWrap;

    tWrap = time;
    _data_58 = 1;
    if (tWrap >= SECONDS_IN_DAY) {
        tWrap = tWrap / SECONDS_IN_DAY;
        tWrap -= (s32) tWrap;
        tWrap *= SECONDS_IN_DAY;
    }
    if (_bss_30) {
        _bss_30->timeSeconds = tWrap;
    } else {
        _data_5C = tWrap;
    }
    _data_8 = tWrap;
}

// offset: 0xF24 | func: 10 | export: 10
s32 dll_7_func_F24(void) {
    return _data_60;
}

// offset: 0xF40 | func: 11 | export: 11
void dll_7_convert_ticks_to_real_time(f32 ticksF, s16 *hours, s16 *minutes, s16 *seconds) {
    s32 ticks = (s32)ticksF;
    
    *hours = ticks / TICKS_IN_HOUR;
    ticks -= *hours * TICKS_IN_HOUR;

    *minutes = ticks / TICKS_IN_MINUTE;
    ticks -= *minutes * TICKS_IN_MINUTE;

    *seconds = ticks / TICKS_IN_SECOND;
}

// offset: 0xFFC | func: 12 | export: 12
s32 dll_7_func_FFC(void);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_FFC.s")

// offset: 0x102C | func: 13 | export: 13
void dll_7_func_102C(Gfx **gdl, Mtx **arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_102C.s")

// offset: 0x20D4 | func: 14 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_20D4.s")

// offset: 0x2130 | func: 15 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_2130.s")

// offset: 0x21CC | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_21CC.s")

// offset: 0x2464 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_2464.s")

// offset: 0x30FC | func: 18
void dll_7_func_30FC(void) {
    if ((_bss_30 != NULL) && (_bss_30 != NULL)) {
        if (_bss_30->unk8 != NULL) {
            texFreeTexture(_bss_30->unk8);
        }
        if (_bss_30->unkC != NULL) {
            texFreeTexture(_bss_30->unkC);
        }
        if (_bss_30->unk10 != NULL) {
            texFreeTexture(_bss_30->unk10);
        }
        if (_bss_30->unk14 != NULL) {
            texFreeTexture(_bss_30->unk14);
        }
        if (_bss_30->unk18 != NULL) {
            texFreeTexture(_bss_30->unk18);
        }
        if (_bss_30->unk1C != NULL) {
            texFreeTexture(_bss_30->unk1C);
        }
        if (_bss_30->unk0 != NULL) {
            mmFree(_bss_30->unk0);
        }
        if (_bss_30->unk4 != NULL) {
            mmFree(_bss_30->unk4);
        }
        mmFree(_bss_30);
        _bss_30 = NULL;
    }
}

// offset: 0x3294 | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_3294.s")

// offset: 0x4484 | func: 20
void dll_7_func_4484(void) {
    f32 sp8C;
    f32 sp88;
    f32 var_fa0;
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    s32 temp_ft0;
    s32 sp70;
    s32 sp6C;
    s32 sp68;
    s32 sp64;
    s32 sp60;
    f32 temp_fv0_2;
    s32 sp4C;
    s32 sp54;

    if (_bss_30 == NULL) {
        return;
    }
    var_fa0 = SQ(_data_18.f[0]) + SQ(_data_18.f[1]) + SQ(_data_18.f[2]);
    if (var_fa0 != 0.0f) {
        var_fa0 = sqrtf(var_fa0);
    } else {
        var_fa0 = 1.0f;
    }
    _data_18.f[0] /= var_fa0;
    _data_18.f[1] /= var_fa0;
    _data_18.f[2] /= var_fa0;
    var_fa0 = SQ(_data_24.f[0]) + SQ(_data_24.f[1]) + SQ(_data_24.f[2]);
    if (var_fa0 != 0.0f) {
        var_fa0 = sqrtf(var_fa0);

    } else {
        var_fa0 = 1.0f;
    }
    _data_24.f[0] /= var_fa0;
    _data_24.f[1] /= var_fa0;
    _data_24.f[2] /= var_fa0;
    sp8C = _data_5C / 86400.0f;
    if (sp8C < 0.0f) {
        sp8C = 0.0f;
    }
    if (sp8C > 1.0f) {
        sp8C = 1.0f;
    }
    sp54 = gDLL_7_Newday->vtbl->func10();
    if (sp54 < 0) {
        sp54 = 0;
    } else if (sp54 >= 5) {
        sp54 = 4;
    }
    if (sp8C <= 0.25f) {
        sp4C = 0;
        sp88 = sp8C / 0.25f;
    } else if (sp8C <= 0.5f) {
        sp4C = 1;
        sp88 = (sp8C - 0.25f) / 0.25f;
    } else if (sp8C <= 0.75f) {
        sp4C = 2;
        sp88 = (sp8C - 0.5f) / 0.25f;
    } else {
        sp4C = 3;
        sp88 = (sp8C - 0.75f) / 0.25f;
    }
    sp70 = (s32) curvesCatmullRom(&_bss_30->unk44[sp4C], sp88, NULL);
    sp6C = (s32) curvesCatmullRom(&_bss_30->unk60[sp4C], sp88, NULL);
    sp68 = (s32) curvesCatmullRom(&_bss_30->unk7C[sp4C], sp88, NULL);
    if (sp54 != _data_10) {
        sp64 = (s32) curvesCatmullRom(&_data_6C[0][sp4C], sp88, NULL);
        sp60 = (s32) curvesCatmullRom(&_data_6C[1][sp4C], sp88, NULL);
        temp_ft0 = (s32) curvesCatmullRom(&_data_6C[2][sp4C], sp88, NULL);
        sp70 = (s32) ((f32) sp64 + (_data_0 * (f32) (sp70 - sp64)));
        sp6C = (s32) ((f32) sp60 + (_data_0 * (f32) (sp6C - sp60)));
        sp68 = (s32) ((f32) temp_ft0 + (_data_0 * (f32) (sp68 - temp_ft0)));
    }
    if (mainGetBits(BIT_77A) != 0) {
        sp70 += mathRnd(0, 0x4B) + 0xB4;
        sp6C += mathRnd(0, 0x4B) + 0x50;
        sp68 += mathRnd(0, 0x4B) + 0x50;
    } else if (mainGetBits(BIT_77B) != 0) {
        sp70 += mathRnd(0, 0x4B) + 0xB4;
        sp6C += mathRnd(0, 0x4B) + 0xB4;
        sp68 += mathRnd(0, 0x4B) + 0x50;
    }
    if (mainGetBits(BIT_77C) != 0) {
        sp70 += mathRnd(0, 0x4B) + 0x50;
        sp6C += mathRnd(0, 0x4B) + 0x50;
        sp68 += mathRnd(0, 0x4B) + 0xB4;
    }
    if (mainGetBits(BIT_77D) != 0) {
        sp70 += mathRnd(0, 0x4B) + 0x50;
        sp6C += mathRnd(0, 0x4B) + 0xB4;
        sp68 += mathRnd(0, 0x4B) + 0x50;
    }
    if (sp70 < 0) {
        sp70 = 0;
    }
    if (sp70 > 0xFF) {
        sp70 = 0xFF;
    }
    if (sp6C < 0) {
        sp6C = 0;
    }
    if (sp6C > 0xFF) {
        sp6C = 0xFF;
    }
    if (sp68 < 0) {
        sp68 = 0;
    }
    if (sp68 > 0xFF) {
        sp68 = 0xFF;
    }
    if (sp70 < 0) {
        sp70 = 0;
    }
    if (sp70 > 0xFF) {
        sp70 = 0xFF;
    }
    if (sp6C < 0) {
        sp6C = 0;
    }
    if (sp6C > 0xFF) {
        sp6C = 0xFF;
    }
    if (sp68 < 0) {
        sp68 = 0;
    }
    if (sp68 > 0xFF) {
        sp68 = 0xFF;
    }
    gDLL_8_newfog->vtbl->func6(&sp70, &sp6C, &sp68);
    _data_34 = (u32) sp70;
    _data_38 = (u32) sp6C;
    _data_3C = (u32) sp68;
    if ((_data_5C >= 18000.0f) && (_data_5C <= 75600.0f)) {
        lightUpdateSkyLight(_data_18.f[0], _data_18.f[1], _data_18.f[2], 
            (u8) (160.0f - (_data_18.f[1] * 30.0f)), 0xFF, 0x3C, -0x1E, 
            (u8) sp70, (u8) sp6C, (u8) sp68);
        shadows_func_8004D698(_data_18.f[0], _data_18.f[1], _data_18.f[2], 0x64);
        gDLL_12_Minic->vtbl->func5(_data_18.f[0], _data_18.f[1], _data_18.f[2], 1);
        D_80090CAC = 1.0f;
        return;
    }
    temp_fv0_2 = (f32) _data_68 / 255.0f;
    D_80090CAC = 1.0f - temp_fv0_2;
    if (temp_fv0_2 != 1.0f) {
        sp78 = _data_18.f[0] + ((_data_24.f[0] - _data_18.f[0]) * temp_fv0_2);
        sp7C = _data_18.f[1] + ((_data_24.f[1] - _data_18.f[1]) * temp_fv0_2);
        sp80 = _data_18.f[2] + ((_data_24.f[2] - _data_18.f[2]) * temp_fv0_2);
        lightUpdateSkyLight(sp78, sp7C, sp80, 
            (u8) (160.0f - (sp7C * 30.0f)), 0xFF, 0x3C, -0x1E, 
            (u8) sp70, (u8) sp6C, (u8) sp68);
        shadows_func_8004D698(sp78, sp7C, sp80, 0x64);
        gDLL_12_Minic->vtbl->func5(_data_18.f[0], _data_18.f[1], _data_18.f[2], 0);
    } else {
        lightUpdateSkyLight(_data_24.f[0], _data_24.f[1], _data_24.f[2], 
            (u8) (160.0f - (_data_24.f[1] * 30.0f)), 0xFF, 0x3C, -0x1E, 
            (u8) sp70, (u8) sp6C, (u8) sp68);
        shadows_func_8004D698(_data_24.f[0], _data_24.f[1], _data_24.f[2], 0x64);
        gDLL_12_Minic->vtbl->func5(_data_24.f[0], _data_24.f[1], _data_24.f[2], 0);
    }
    if (D_80090CAC < 0.0f) {
        D_80090CAC = 0.0f;
    } else if (D_80090CAC > 1.0f) {
        D_80090CAC = 1.0f;
    }
}

// offset: 0x5124 | func: 21
void dll_7_func_5124(f32 x, f32 y, f32 z) {
    SRT partSrt;
    SRT sp98;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 fbZ;
    Camera* camera;
    FXEmit_Setup* fxSetup;
    f32 cx;
    f32 cy;
    f32 cz;
    f32 camDirX;
    f32 camDirY;
    f32 camDirZ;
    f32 lx;
    f32 ly;
    f32 lz;
    f32 sp50[3] = {0.0f, 0.0f, 200.0f};
    u16 temp2;
    Object* player;
    s32 pad;
    f32 temp;
    s32 scrCenterX;
    s32 scrCenterY;
    f32 transp;

    camera = camGet();
    player = objGetPlayer();
    transp = 0.0f;
    if (_data_C0 == 0) {
        fxSetup = objAllocSetup(sizeof(FXEmit_Setup), OBJ_FXEmit);
        fxSetup->base.loadDistance = 0xFF;
        fxSetup->base.fadeDistance = 0xFF;
        fxSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
        fxSetup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
        fxSetup->base.x = camera->srt.transl.x;
        fxSetup->base.y = camera->srt.transl.y;
        fxSetup->base.z = camera->srt.transl.z;
        fxSetup->toggleGamebit = BIT_7DC;
        fxSetup->disableGamebit = NO_GAMEBIT;
        fxSetup->yaw = 0;
        fxSetup->flagConfig = 1;
        fxSetup->pitch = 0;
        fxSetup->roll = 0;
        fxSetup->rollSpeed = 0;
        fxSetup->pitchSpeed = 0;
        fxSetup->yawSpeed = 0;
        fxSetup->activationRange = 0;
        fxSetup->bank = 0;
        fxSetup->indexInBank = PARTICLE_53D;
        fxSetup->fxRate = 1;
        _data_C0 = objSetupObject((ObjSetup*)fxSetup, OBJINIT_FLAG4 | OBJINIT_STANDALONE, player->mapID, -1, player->parent);
        if (_data_C0 == 0) {
            return;
        }
    }
    if (_data_C0 == 0) {
        return;
    }

    lx = x - gWorldX;
    ly = y;
    lz = z - gWorldZ;
    partSrt.transl.x = x;
    partSrt.transl.y = y;
    partSrt.transl.z = z;
    partSrt.scale = 1.0f;
    partSrt.yaw = 0;
    partSrt.roll = 0;
    partSrt.pitch = 0;
    camProjectPoint(lx, ly, lz, &cx, &cy, &cz);
    camClipToScreen(cx, cy, cz, &sx, &sy, NULL);
    if ((sx >= 0) && (sx <= 320) && (sy >= 0) && (sx <= 240)) {
        scrCenterX = sx - 160;
        scrCenterY = sy - 120;
        temp = SQ(scrCenterX) + SQ(scrCenterY);
        if (temp != 0.0f) {
            _data_C4 = sqrtf(temp);
        } else {
            _data_C4 = 0.0f;
        }
    } else {
        _data_C4 = 200.0f;
    }
    fbZ = viObjDepth(sx, sy, (Object* )0x3039);
    camGetVec3ToCameraNormalized(lx, ly, lz, &camDirX, &camDirY, &camDirZ);
    lx += (camDirX * 20.0f);
    ly += (camDirY * 20.0f);
    lz += (camDirZ * 20.0f);
    camProjectPoint(lx, ly, lz, &cx, &cy, &cz);
    camClipToScreen(cx, cy, cz, NULL, NULL, &sz);
    mainSetBits(BIT_7DC, 0);
    if ((viContainsPoint(sx, sy) != 0) && (sz > 0) && (sz < fbZ)) {
        if (_data_C4 != 0.0f) {
            transp = _data_C4 / 200.0f;
        } else {
            transp = 0.0f;
        }
        player = objGetPlayer();
        gDLL_17_partfx->vtbl->spawn(player, PARTICLE_53C, &partSrt, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, &transp);
        sp50[0] = 0;
        sp50[1] = 0;
        sp50[2] = 1000.0f;
        sp98.transl.x = 0;
        sp98.transl.y = 0;
        sp98.transl.z = 0;
        sp98.scale = 1.0f;
        sp98.yaw = -camera->srt.yaw;
        sp98.roll = camera->srt.roll;
        sp98.pitch = camera->srt.pitch;
        mathRotateRPY(&sp98, sp50);
        sp50[0] += camera->srt.transl.x;
        sp50[1] += camera->srt.transl.y;
        sp50[2] += camera->srt.transl.z;
        _data_C0->srt.transl.x = sp50[0];
        _data_C0->srt.transl.y = sp50[1];
        _data_C0->srt.transl.z = sp50[2];
        x -= _data_C0->srt.transl.x;
        y -= _data_C0->srt.transl.y;
        z -= _data_C0->srt.transl.z;
        temp = mathAtan2f(x, z);
        temp2 = ((s32) (f32) mathAtan2f(x, y)) & 0xFFFF;
        _data_C0->srt.yaw = temp;
        _data_C0->srt.pitch = -temp2;
        _data_C0->srt.roll = camera->srt.roll;
        mainSetBits(BIT_7DC, 1);
    }
}

/*0x2A0*/ static u32 _data_2A0 = 0x00000000;

// offset: 0x56F8 | func: 22
s32 dll_7_func_56F8(Gfx** gdl) {
    /*0x2A4*/ static u8 _data_2A4 = 0;
    f32 transp;

    _data_2A4 += 1;
    transp = _data_C4 / 200.0f;
    if (transp < 0.0f) {
        transp = 0.0f;
    }
    if (transp == 0.0f) {
        return 0;
    }
    dlSetFogColor(gdl, 0xFF, 0xFF, 0xDC, (u8) (0xFF - (s16) (transp * 255.0f)));
    return 1;
}

// offset: 0x57C0 | func: 23
void dll_7_func_57C0(void) {
    bzero(&_bss_0, sizeof(_bss_0));
    _bss_0.unk26 = mainGetBits(BIT_SC_UNKNOWN_2BA);
}

// offset: 0x5818 | func: 24
void dll_7_func_5818(void) {
    u8 isNight;
    f32 time;
    Object* player;
    s32 _pad;

    if (lightGetInside() != 0) {
        _bss_0.unk27 |= 0x10;
    }
    isNight = gDLL_7_Newday->vtbl->func8(&time);
    if ((isNight != _bss_0.unk25) || (_bss_0.unk27 & 0x10)) {
        _bss_0.unk24 = 0x3C;
        _bss_0.unk27 &= ~0x10;
    }
    if (_bss_0.unk24 > 0) {
        _bss_0.unk24--;
        if (!isNight) {
            switch (_bss_0.unk24) {
            case 0x3B:
                _bss_0.unk26++;
                if (_bss_0.unk26 >= 28) {
                    _bss_0.unk26 = 0;
                }
                if ((_bss_0.unk27 & 1) && (lightGetInside() == 0)) {
                    envfxAction(NULL, NULL, _bss_0.unkC[_bss_0.unk26], 0);
                }
                mainSetBits(BIT_SC_UNKNOWN_2BA, _bss_0.unk26);
                break;
            case 0x31:
                if ((_bss_0.unk27 & 1) && (_bss_0.unk4[_bss_0.unk26] != 0) && (lightGetInside() == 0)) {
                    player = objGetPlayer();
                    envfxAction(player, player, _bss_0.unk4[_bss_0.unk26], 0);
                }
                break;
            case 0x27:
                if ((_bss_0.unk27 & 1) && (lightGetInside() == 0)) {
                    envfxAction(NULL, NULL, _bss_0.unk0[_bss_0.unk26], 0);
                }
                break;
            case 0x1D:
                if ((_bss_0.unk27 & 1) && (lightGetInside() == 0)) {
                    envfxAction(NULL, NULL, _bss_0.unk8[_bss_0.unk26], 0);
                }
                break;
            case 0x13:
                if ((_bss_0.unk27 & 2) && (lightGetInside() == 0)) {
                    lfxAction(NULL, NULL, _bss_0.unk10[_bss_0.unk26], 0, 0, 0);
                }
                break;
            case 0x9:
                if ((_bss_0.unk27 & 2) && (lightGetInside() == 0)) {
                    lfxAction(NULL, NULL, _bss_0.unk14[_bss_0.unk26], 0, 0, 0);
                }
                break;
            }
        }
        if (_bss_0.unk24 <= 0) {
            if (lightGetInside() == 0) {
                player = objGetPlayer();
                if (isNight) {
                    if (_bss_0.unk27 & 4) {
                        if (_bss_0.unk1E != 0) {
                            gDLL_5_AMSEQ2->vtbl->set(player, _bss_0.unk1E, NULL, 0, NULL);
                        }
                        if (_bss_0.unk22 != 0) {
                            gDLL_5_AMSEQ2->vtbl->set(player, _bss_0.unk22, NULL, 0, NULL);
                        }
                    }
                    if (_bss_0.unk27 & 8) {
                        lfxAction(NULL, NULL, _bss_0.unk1A, 0, 0, 0);
                    }
                } else {
                    if (_bss_0.unk27 & 4) {
                        if (_bss_0.unk1C != 0) {
                            gDLL_5_AMSEQ2->vtbl->set(player, _bss_0.unk1C, NULL, 0, NULL);
                        }
                        if (_bss_0.unk20 != 0) {
                            gDLL_5_AMSEQ2->vtbl->set(player, _bss_0.unk20, NULL, 0, NULL);
                        }
                    }
                    if (_bss_0.unk27 & 8) {
                        lfxAction(NULL, NULL, _bss_0.unk18, 0, 0, 0);
                    }
                }
            }
            _bss_0.unk24 = 0;
        }
    }
    _bss_0.unk25 = isNight;
}

// offset: 0x5D20 | func: 25 | export: 16
void dll_7_func_5D20(u8 arg0) {
    _bss_0.unk27 &= ~0xF;
    _bss_0.unk27 |= (arg0 & 0xF);
    _bss_0.unk27 |= 0x10;
}

// offset: 0x5D6C | func: 26 | export: 17
s32 dll_7_func_5D6C(void) {
    return _bss_0.unk27 & 0xF;
}

// offset: 0x5D90 | func: 27 | export: 18
void dll_7_func_5D90(u16* arg0, u16* arg1, u16* arg2, u16* arg3) {
    _bss_0.unkC = arg0;
    _bss_0.unk0 = arg1;
    _bss_0.unk8 = arg2;
    _bss_0.unk4 = arg3;
}

// offset: 0x5DBC | func: 28 | export: 19
void dll_7_func_5DBC(u16* arg0, u16* arg1, u16 arg2, u16 arg3) {
    _bss_0.unk10 = arg0;
    _bss_0.unk14 = arg1;
    _bss_0.unk18 = arg2;
    _bss_0.unk1A = arg3;
}

// offset: 0x5E00 | func: 29 | export: 20
void dll_7_func_5E00(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    _bss_0.unk1C = arg0;
    _bss_0.unk1E = arg1;
    _bss_0.unk20 = arg2;
    _bss_0.unk22 = arg3;
}

// offset: 0x5E5C | func: 30 | export: 21
void dll_7_func_5E5C(u8 arg0) {
    if (arg0 >= 28) {
        arg0 = 0;
    }
    _bss_0.unk26 = arg0;
    mainSetBits(BIT_SC_UNKNOWN_2BA, arg0);
}

// offset: 0x5EB4 | func: 31 | export: 22
u8 dll_7_func_5EB4(void) {
    return _bss_0.unk26;
}

// offset: 0x5ED0 | func: 32 | export: 23
s32 dll_7_func_5ED0(Gfx** gdl) {
    /*0x2A8*/ static u8 _data_2A8 = 0;

    s16 tmp;
    f32 var_fv0;

    _data_2A8 += 1;
    var_fv0 = _data_C4 / 200.0f;
    if (var_fv0 < 0.0f) {
        var_fv0 = 0.0f;
    }
    if (var_fv0 == 0.0f) {
        return 0;
    }
    dlSetFogColor(gdl, 0xFF, 0xFF, 0xDC, 0xFF);
    tmp = (s32) (var_fv0 * 355.0f);
    gSPFogFactor((*gdl)++, 
        128000 / (((tmp * 2) - tmp) + 150), 
        (-76800 - (tmp << 8)) / (((tmp * 2) - tmp) + 150));
    return 1;
}
