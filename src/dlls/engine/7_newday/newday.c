#include "sys/gfx/texture.h"
#include "dlls/engine/7_newday.h"

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
/*0x10*/ static f32 _data_10[] = {
    0.0, 0.1
};
/*0x18*/ static f32 _data_18 = 0.0;
/*0x1C*/ static f32 _data_1C = 1.0f;
/*0x20*/ static u32 _data_20 = 0x00000000;
/*0x24*/ static f32 _data_24 = 0.0;
/*0x28*/ static f32 _data_28 = 1;
/*0x2C*/ static u32 _data_2C = 0x00000000;
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
/*0x68*/ static u32 _data_68 = 0x00000000;
/*0x6C*/ static f32 _data_6C = 55.0;
/*0x70*/ static f32 _data_70[] = {
    55, 100, 210, 125, 55, 55, 55, 55, 
    60, 210, 120, 55, 55, 55, 55, 60, 
    220, 240, 55, 55
};
/*0xC0*/ static u32 _data_C0 = 0x00000000;
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
/*0x294*/ static f32 _data_294[] = {
    0, 0, 200, 0
};
/*0x2A4*/ static u32 _data_2A4 = 0x00000000;
/*0x2A8*/ static u32 _data_2A8[] = {
    0x00000000, 0x00000000
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
/*044*/ f32 unk44;
/*048*/ f32 unk48;
/*04C*/ f32 unk4C;
/*050*/ f32 unk50;
/*054*/ f32 unk54;
/*058*/ f32 unk58;
/*05C*/ f32 unk5C;
/*060*/ f32 unk60;
/*064*/ f32 unk64;
/*068*/ f32 unk68;
/*06C*/ f32 unk6C;
/*070*/ f32 unk70;
/*074*/ f32 unk74;
/*078*/ f32 unk78;
/*07C*/ f32 unk7C;
/*080*/ f32 unk80;
/*084*/ f32 unk84;
/*088*/ f32 unk88;
/*08C*/ f32 unk8C;
/*090*/ f32 unk90;
/*094*/ f32 unk94;
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

/*0x0*/ static u8 _bss_0[0x26]; // DAT_810296a0
/*0x26*/ static u8 _bss_26; // DAT_810296c6
/*0x27*/ static u8 _bss_27; // DAT_810296c7
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_30FC.s")

// offset: 0x3294 | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_3294.s")

// offset: 0x4484 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_4484.s")

// offset: 0x5124 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_5124.s")

// offset: 0x56F8 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_56F8.s")

// offset: 0x57C0 | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_57C0.s")

// offset: 0x5818 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_5818.s")

// offset: 0x5D20 | func: 25 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_5D20.s")

// offset: 0x5D6C | func: 26 | export: 17
s32 dll_7_func_5D6C(void) {
    return _bss_27 & 0xF;
}

// offset: 0x5D90 | func: 27 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_5D90.s")

// offset: 0x5DBC | func: 28 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_5DBC.s")

// offset: 0x5E00 | func: 29 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_5E00.s")

// offset: 0x5E5C | func: 30 | export: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_5E5C.s")

// offset: 0x5EB4 | func: 31 | export: 22
u8 dll_7_func_5EB4(void) {
    return _bss_26;
}

// offset: 0x5ED0 | func: 32 | export: 23
s32 dll_7_func_5ED0(Gfx **gdl);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_5ED0.s")
