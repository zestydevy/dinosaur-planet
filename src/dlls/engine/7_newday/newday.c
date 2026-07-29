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
#include "gbi_extra.h"

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
/*0x4*/ static s32 _data_4 = 0;
/*0x8*/ static f32 _data_8 = 0.0;
/*0xC*/ static f32 _data_C = 0.0;
/*0x10*/ static s32 _data_10 = 0;
/*0x14*/ static f32 _data_14 = 0.1f;
/*0x18*/ static Vec3f _data_18 = VEC3F(0.0f, 1.0f, 0.0f);
/*0x24*/ static Vec3f _data_24 = VEC3F(0.0f, 1.0f, 0.0f);
/*0x30*/ static s32 _data_30 = -1;
/*0x34*/ static s32 _data_34 = 255; //clouds R
/*0x38*/ static s32 _data_38 = 255; //clouds G
/*0x3C*/ static s32 _data_3C = 255; //clouds B
/*0x40*/ static s32 _data_40 = 0;
/*0x44*/ static s32 _data_44 = 0;
/*0x48*/ static s32 _data_48 = 0;
/*0x4C*/ static s32 _data_4C = 0;
/*0x50*/ static s32 _data_50 = 0;
/*0x54*/ static s32 _data_54 = 0;
/*0x58*/ static u8 _data_58 = 0;
/*0x5C*/ static f32 _data_5C = NOON; //time of day
/*0x60*/ static s32 _data_60 = 0;
/*0x64*/ static s16 _data_64 = 0;
/*0x68*/ static s16 _data_68 = 0;
         // splines
/*0x6C*/ static f32 _data_6C[][7] = {
    {55, 55, 100, 210, 125, 55, 55}, 
    {55, 55, 60,  210, 120, 55, 55}, 
    {55, 55, 60,  220, 240, 55, 55}
};
/*0xC0*/ static Object* _data_C0 = NULL;
/*0xC4*/ static f32 _data_C4 = 1.0;
/*0xC8*/ static u8 _data_C8 = 0;
/*0xCC*/ static u8 _data_CC[][6] = {
    {0x00, 0x1d, 0xa4, 0x00, 0x00, 0x48}, 
    {0x9b, 0x44, 0x1d, 0x0c, 0x35, 0x1c}, 
    {0xff, 0x8f, 0xbf, 0xff, 0x74, 0xba}, 
    {0xff, 0xdb, 0xff, 0xff, 0xb0, 0xff}, 
    {0xff, 0xff, 0xff, 0xe8, 0xd3, 0xff}, 
    {0x82, 0xff, 0xff, 0x4f, 0xa3, 0xff}, 
    {0xb4, 0xff, 0xff, 0x6f, 0xa7, 0xff}, 
    {0xff, 0xff, 0xa5, 0xf5, 0xb7, 0x8c}, 
    {0xff, 0xcd, 0x00, 0xff, 0x98, 0x00}, 
    {0xff, 0x81, 0x24, 0xf2, 0x60, 0x21}, 
    {0x99, 0x35, 0x6b, 0x68, 0x26, 0x66}, 
    {0x0b, 0x00, 0x45, 0x07, 0x00, 0x41}, 
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, 
    {0xca, 0x00, 0xfe, 0x4d, 0x00, 0x61}, 
    {0xff, 0x74, 0xc8, 0x62, 0x00, 0x3b}, 
    {0x65, 0xe0, 0x7f, 0x00, 0x53, 0x2c}, 
    {0xfe, 0xfe, 0x13, 0x00, 0x69, 0x26}, 
    {0xff, 0xfe, 0x26, 0xcd, 0x2d, 0x3d}, 
    {0xff, 0xfd, 0x00, 0xa9, 0x13, 0x39}, 
    {0xfe, 0xfe, 0xfe, 0x08, 0x79, 0xd0}, 
    {0xce, 0x00, 0x00, 0xff, 0xa1, 0x00}, 
    {0xff, 0xfe, 0xe2, 0x5c, 0x83, 0x3f}, 
    {0xff, 0xff, 0x93, 0xb4, 0x5b, 0x43}, 
    {0xff, 0xfe, 0xfe, 0xd2, 0x38, 0x82}, 
    {0xff, 0x00, 0x00, 0x7a, 0x11, 0x01}, 
    {0x98, 0x00, 0x95, 0x24, 0x00, 0x57}, 
    {0xff, 0x48, 0xff, 0x65, 0x06, 0x65}, 
    {0xff, 0xe6, 0x83, 0xff, 0xb0, 0x2f}, 
    {0xfe, 0x00, 0x00, 0x3f, 0x00, 0x00}, 
    {0x5c, 0x8e, 0xff, 0x00, 0x00, 0x5c}, 
    {0x99, 0xc6, 0xff, 0x00, 0x25, 0xac}, 
    {0xff, 0xff, 0xff, 0x35, 0x35, 0xff}, 
    {0x80, 0x80, 0x53, 0x7a, 0x60, 0x46}, 
    {0xc9, 0x00, 0xfe, 0x4d, 0x00, 0x61}, 
    {0x1e, 0x41, 0x55, 0xc3, 0xdb, 0xf4}, 
    {0x1e, 0x41, 0x55, 0xcb, 0xdb, 0x85}, 
    {0x1e, 0x41, 0x55, 0x6f, 0x0c, 0x86}, 
    {0x1e, 0x41, 0x55, 0x31, 0x8a, 0xd8}, 
    {0x1e, 0x41, 0x55, 0xff, 0xd5, 0x51}, 
    {0x1e, 0x41, 0x55, 0xff, 0x0c, 0x00}
};

// size: 0x118
typedef struct {
/*000*/ DLTri *unk0;
/*004*/ Vtx *unk4;
/*008*/ Texture *unk8;
/*00C*/ Texture *unkC;
/*010*/ Texture *unk10;
/*014*/ Texture *unk14;
/*018*/ Texture *unk18;
/*01C*/ Texture *unk1C;
/*020*/ s32 unk20;
/*024*/ u8 _unk20[8];
/*02C*/ f32 unk2C;
/*030*/ f32 unk30;
/*034*/ f32 unk34;
/*038*/ u8 _unk38[8];
/*040*/ f32 unk40;
/*044*/ f32 unk44[3][7]; // splines
/*098*/ u8 _unk98[0xc0 - 0x98];
/*0C0*/ f32 timeSeconds; //time of day (seconds)
/*0C4*/ f32 unkC4;
/*0C8*/ f32 unkC8;
/*0CC*/ s32 unkCC;
/*0D0*/ s32 unkD0;
/*0D4*/ s32 unkD4;
/*0D8*/ s32 unkD8;
/*0DC*/ s32 unkDC;
/*0E0*/ s32 unkE0;
/*0E4*/ s32 unkE4;
/*0E8*/ s32 unkE8;
/*0EC*/ s32 unkEC[8];
/*10C*/ s32 unk10C;
/*110*/ u8 unk110;
/*111*/ u8 unk111;
/*112*/ u8 unk112;
/*113*/ u8 unk113;
/*114*/ u8 unk114;
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
/*0x28*/ static f32 _bss_28;
/*0x2C*/ static f32 _bss_2C;
/*0x30*/ static NewDayStruct *_bss_30;
/*0x34*/ static u8 _bss_34[0x28];

s32 dll_7_func_DF4(f32* outTime);
static void dll_7_func_21CC(void);
void dll_7_func_2464(EnvFxAction*, f32, f32, f32);
static void dll_7_func_30FC(void);
static void dll_7_func_3294(Gfx** gdl, Mtx** mtxs);
static void dll_7_func_4484(void);
static void dll_7_func_5124(f32 x, f32 y, f32 z);
static s32 dll_7_func_56F8(Gfx** gdl);
static void dll_7_func_57C0(void);
static void dll_7_func_5818(void);

// offset: 0x0 | ctor
void dll_7_ctor(void *self) { }

// offset: 0xC | dtor
void dll_7_dtor(void *self) { }

// offset: 0x18 | func: 0 | export: 0
// official name: doDayAction
#ifndef NON_MATCHING
void dll_7_func_18(Object *arg0, Object *arg1, EnvFxAction *arg2, s32 arg3);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_18.s")
#else
void dll_7_func_18(Object* arg0, Object* arg1, EnvFxAction* arg2, s32 arg3) {
    PlayerEnvActions* temp_v0;
    s32 i;

    temp_v0 = gDLL_29_Gplay->vtbl->get_current_player_envactions();
    if ((arg2 != NULL) && (arg2->unk26 < 2)) {
        if ((&_bss_30)[arg2->unk26] == NULL) {
            if (!(arg2->unk58 & 0x20) && (arg2->unk58 & 2)) {
                arg2->unk58 |= 0x40;
                if (arg0 != NULL) {
                    dll_7_func_2464(arg2, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z);
                    return;
                }
                dll_7_func_2464(arg2, 0.0f, 0.0f, 0.0f);
            }
        } else if ((&_bss_30)[arg2->unk26] != NULL) {
            if (arg2->unk58 & 2) {
                (&_bss_30)[arg2->unk26]->unk112 |= 0x40;
                _data_4 = 1;
                if (arg2->unk58 & 1) {
                    _data_8 = (&_bss_30)[arg2->unk26]->timeSeconds;
                    _bss_30->unk112 |= 1;
                } else {
                    _data_8 = (f32) ((arg2->unk50 * 0x3C) + (arg2->unk4E * 0xE10));
                }
                if (arg2->unk5D == 0) {
                    _bss_30->unkC8 = 0.9f;
                } else if (arg2->unk5D == 1) {
                    _bss_30->unkC8 = 1.0f;
                } else if (arg2->unk5D == 2) {
                    _bss_30->unkC8 = 0.9f;
                } else if (arg2->unk5D == 3) {
                    _bss_30->unkC8 = 0.75f;
                }
                if (arg2->unk54 == 0) {
                    _bss_30->unk40 = _bss_30->unkC8 * 2000.0f;
                } else if (arg2->unk54 == 1) {
                    _bss_30->unk40 = _bss_30->unkC8 * 10000.0f;
                } else if (arg2->unk54 == 2) {
                    _bss_30->unk40 = _bss_30->unkC8 * 12000.0f;
                } else if (arg2->unk54 == 3) {
                    _bss_30->unk40 = _bss_30->unkC8 * 14000.0f;;
                }
                _bss_30->unkE8 = (s32) arg2->unk56;
                _data_C = (&_bss_30)[arg2->unk26]->unk40;
                (&_bss_30)[arg2->unk26]->unkDC = (s32) arg2->unk52;
                (&_bss_30)[arg2->unk26]->unkDC = 0x168;
                (&_bss_30)[arg2->unk26]->unkC4 = (f32) (&_bss_30)[arg2->unk26]->unkDC / 60.0f;
                (&_bss_30)[arg2->unk26]->unkCC = (s32) arg2->unk2A;
                (&_bss_30)[arg2->unk26]->unkD0 = (s32) arg2->unk2C;
                (&_bss_30)[arg2->unk26]->unkD8 = 0;
                (&_bss_30)[arg2->unk26]->unkD4 = 1;
                temp_v0->unk4 = (s16) arg2->unk24 - 1;
                _data_10 = 0;
                _data_60 = 1;
                for (i = 0; i < (3 * 7); i++) {
                    ((f32*)_data_6C)[i] = ((f32*)_bss_30->unk44)[i];
                }
                _bss_30->unk44[0][0] = (f32) arg2->unkC;
                _bss_30->unk44[0][1] = (f32) arg2->unkC;
                _bss_30->unk44[0][2] = (f32) arg2->unkD;
                _bss_30->unk44[0][3] = (f32) arg2->unkE;
                _bss_30->unk44[0][4] = (f32) arg2->unkF;
                _bss_30->unk44[0][5] = (f32) arg2->unkC;
                _bss_30->unk44[0][6] = (f32) arg2->unkC;
                _bss_30->unk44[1][0] = (f32) arg2->unk14;
                _bss_30->unk44[1][1] = (f32) arg2->unk14;
                _bss_30->unk44[1][2] = (f32) arg2->unk15;
                _bss_30->unk44[1][3] = (f32) arg2->unk16;
                _bss_30->unk44[1][4] = arg2->unk17;
                _bss_30->unk44[1][5] = arg2->unk14;
                _bss_30->unk44[1][6] = arg2->unk14;
                _bss_30->unk44[2][0] = arg2->unk1C;
                _bss_30->unk44[2][1] = arg2->unk1C;
                _bss_30->unk44[2][2] = arg2->unk1D;
                _bss_30->unk44[2][3] = arg2->unk1E;
                _bss_30->unk44[2][4] = arg2->unk1F;
                _bss_30->unk44[2][5] = arg2->unk1C;
                _bss_30->unk44[2][6] = arg2->unk1C;
                _bss_30->unkEC[0] = arg2->unk2E + 0x210;
                _bss_30->unkEC[1] = arg2->unk30 + 0x210;
                _bss_30->unkEC[2] = arg2->unk32 + 0x210;
                _bss_30->unkEC[3] = arg2->unk34 + 0x210;
                _bss_30->unkEC[4] = arg2->unk3E + 0x210;
                _bss_30->unkEC[5] = arg2->unk40 + 0x210;
                _bss_30->unkEC[6] = arg2->unk42 + 0x210;
                _bss_30->unkEC[7] = arg2->unk44 + 0x210;
                _bss_30->unk114 = 1;
            } else if (arg2->unk58 & 0x20) {
                (&_bss_30)[arg2->unk26]->unk112 |= 0x40;
                _data_4 = 1;
                _bss_30->unkC8 = 1.0f;
                _bss_30->unk40 = _bss_30->unkC8 * 10000.0f;
                _data_C = (&_bss_30)[arg2->unk26]->unk40;
                (&_bss_30)[arg2->unk26]->unkDC = (s32) arg2->unk52;
                (&_bss_30)[arg2->unk26]->unkDC = 0x168;
                (&_bss_30)[arg2->unk26]->unkC4 = (f32) (&_bss_30)[arg2->unk26]->unkDC / 60.0f;
                (&_bss_30)[arg2->unk26]->unkD8 = 1;
                (&_bss_30)[arg2->unk26]->unkD4 = 0;
                (&_bss_30)[arg2->unk26]->unkCC = (s32) arg2->unk2A;
                (&_bss_30)[arg2->unk26]->unkD0 = (s32) arg2->unk2C;
                _bss_30->unk114 = 0;
            }
        }
    }
}
#endif

// offset: 0x910 | func: 1 | export: 1
void dll_7_func_910(void) {
    if (_bss_30 != NULL) {
        dll_7_func_30FC();
    }
    envfxAction(NULL, NULL, 4, 0);
    lightSetInside(0);
    _data_30 = -1;
    dll_7_func_4484();
    _data_18.x = 0.0f;
    _data_18.y = 1.0f;
    _data_18.z = 0.0f;
    _data_24.x = 0.0f;
    _data_24.y = 1.0f;
    _data_24.z = 0.0f;
    light_func_8001CDE4(0xFF, 0xFF, 0xFF, 1, 0);
    dll_7_func_57C0();
}

// offset: 0xA10 | func: 2 | export: 2
void dll_7_func_A10(s32 arg0, s32 arg1) {
    /*0x1BC*/ static s32 _data_1BC = 0x00000000;
    f32 sp34;
    s32 _pad;
    f32 sp2C;
    PlayerEnvActions* sp28;

    sp2C = 0.0f;
    sp28 = gDLL_29_Gplay->vtbl->get_current_player_envactions();
    _data_C8 = 0;
    if (_bss_30 != NULL) {
        if (_bss_30->unkDC != 0) {
            _bss_30->timeSeconds += _bss_30->unkC4 * gUpdateRateF;
        }
        sp34 = _bss_30->timeSeconds;
        if (sp34 >= 86400.0f) {
            sp34 -= 86400.0f;
            _bss_30->timeSeconds -= 86400.0f;
        } else if (sp34 < 0.0f) {
            sp34 += 86400.0f;
            _bss_30->timeSeconds += 86400.0f;
        }
        if (dll_7_func_DF4(&sp2C) != 0) {
            if (_data_1BC == 0) {
                _data_1BC = 1;
            }
        } else if (_data_1BC) {
            _bss_30->unkE4 += 1;
            if (_bss_30->unkE4 >= 0x1F) {
                _bss_30->unkE4 = 0;
            }
            _data_1BC = 0;
        }
        if (0) { } // @fake
        if (_bss_30->unk112 & 0x40) {
            dll_7_func_21CC();
        }
        if (_bss_30 != NULL) {
            _data_5C = sp34;
            if (objGetPlayer() != NULL) {
                sp28->unk0 = _bss_30->timeSeconds;
            }
        } else {
            _data_5C = 43200.0f;
        }
        if ((objGetPlayer() != NULL) && (_bss_30 != NULL)) {
            sp28->unk0 = _bss_30->timeSeconds;
        }
    }
    _data_58 = 0;
    dll_7_func_5818();
}

// offset: 0xC58 | func: 3 | export: 3
void dll_7_func_C58(Gfx** gdl, Mtx** mtxs, u8 arg2) {
    if ((lightGetInside() == 0) && (arg2 != 0)) {
        dll_7_func_3294(gdl, mtxs);
    }
    dll_7_func_4484();
}

// offset: 0xCDC | func: 4 | export: 4
void dll_7_func_CDC(f32* timeSeconds) {
    if (_bss_30 != NULL) {
        *timeSeconds = _bss_30->timeSeconds;
    }
}

// offset: 0xD08 | func: 5 | export: 5
void dll_7_func_D08(f32* arg0) {
    if (_bss_30 != NULL) {
        *arg0 = (f32) _bss_30->unkDC;
    } else {
        *arg0 = 0.0f;
    }
}

// offset: 0xD50 | func: 6 | export: 6
void dll_7_func_D50(f32 arg0) {
    if (_bss_30 != NULL) {
        _bss_30->unkDC = (s32) arg0;
        _bss_30->unkC4 = (f32) _bss_30->unkDC / 60.0f;
    }
}

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
s32 dll_7_func_FFC(void) {
    if (_bss_30 != NULL) {
        return _bss_30->unkE8;
    } else {
        return 0;
    }
}

// offset: 0x102C | func: 13 | export: 13
void dll_7_func_102C(Gfx** gdl, Mtx** arg1) {
    /*0x1C0*/ static s16 _data_1C0 = 0;
    /*0x1C4*/ static s16 _data_1C4 = 0;
    f32 sp64;
    s32 temp_v0_8;
    f32 sp5C;
    f32 sp58;
    f32 var_fv0;
    f32 sp50;
    s32 temp_t8;
    s32 temp_t6;
    s32 sp44;
    Camera* sp40;
    Texture* sp3C;
    Texture* sp38;
    f32 fov;
    f32 sp30;
    f32 fa1;

    sp44 = 0;
    texRenderReset();
    var_fv0 = _data_5C / 86400.0f;
    if (var_fv0 < 0.0f) {
        var_fv0 = 0.0f;
    }
    if (var_fv0 > 1.0f) {
        var_fv0 = 1.0f;
    }
    sp30 = 0.0f;
    if ((var_fv0 >= 0.0f) && (var_fv0 < (1.0f/8.0f))) {
        sp30 = var_fv0 / (1.0f/8.0f);
    } else if ((var_fv0 >= (1.0f/8.0f)) && (var_fv0 < (2.0f/8.0f))) {
        sp44 = 1;
        sp30 = (var_fv0 - (1.0f/8.0f)) / (1.0f/8.0f);
    } else if ((var_fv0 >= (2.0f/8.0f)) && (var_fv0 < (3.0f/8.0f))) {
        sp44 = 2;
        sp30 = (var_fv0 - (2.0f/8.0f)) / (1.0f/8.0f);
    } else if ((var_fv0 >= (3.0f/8.0f)) && (var_fv0 < (4.0f/8.0f))) {
        sp44 = 3;
        sp30 = (var_fv0 - (3.0f/8.0f)) / (1.0f/8.0f);
    } else if ((var_fv0 >= (4.0f/8.0f)) && (var_fv0 < (5.0f/8.0f))) {
        sp44 = 4;
        sp30 = (var_fv0 - (4.0f/8.0f)) / (1.0f/8.0f);
    } else if ((var_fv0 >= (5.0f/8.0f)) && (var_fv0 < (6.0f/8.0f))) {
        sp44 = 5;
        sp30 = (var_fv0 - (5.0f/8.0f)) / (1.0f/8.0f);
    } else if ((var_fv0 >= (6.0f/8.0f)) && (var_fv0 < (7.0f/8.0f))) {
        sp44 = 6;
        sp30 = (var_fv0 - (6.0f/8.0f)) / (1.0f/8.0f);
    } else if ((var_fv0 >= (7.0f/8.0f)) && (var_fv0 <= (8.0f/8.0f))) {
        sp44 = 7;
        sp30 = (var_fv0 - (7.0f/8.0f)) / (1.0f/8.0f);
    }
    if ((_bss_30 != NULL) && (sp44 != _data_30) && (_data_C8 == 0)) {
        _bss_30->unk10C = sp44;
        if (_bss_30->unk114 != 0) {
            _bss_30->unk18 = _bss_30->unk1C;
            _bss_30->unk18->refCount += 1;
            _data_1C0 = _data_1C4;
        } else {
            if (_bss_30->unk18 != NULL) {
                texFreeTexture(_bss_30->unk18);
            }
            _bss_30->unk18 = NULL;
        }
        if (_bss_30->unk1C != NULL) {
            texFreeTexture(_bss_30->unk1C);
        }
        _bss_30->unk1C = NULL;
        if (_bss_30->unk10C == 0) {
            if (_bss_30->unk114 != 1) {
                _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[0]);
                _data_1C0 = _bss_30->unkEC[0] - 0x210;
            }
            _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[1]);
            _data_1C4 = _bss_30->unkEC[1] - 0x210;
        } else if (_bss_30->unk10C == 1) {
            if (_bss_30->unk114 != 1) {
                _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[1]);
                _data_1C0 = _bss_30->unkEC[1] - 0x210;
            }
            _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[2]);
            _data_1C4 = _bss_30->unkEC[2] - 0x210;
        } else if (_bss_30->unk10C == 2) {
            if (_bss_30->unk114 != 1) {
                _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[2]);
                _data_1C0 = _bss_30->unkEC[2] - 0x210;
            }
            _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[3]);
            _data_1C4 = _bss_30->unkEC[3] - 0x210;
        } else if (_bss_30->unk10C == 3) {
            if (_bss_30->unk114 != 1) {
                _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[3]);
                _data_1C0 = _bss_30->unkEC[3] - 0x210;
            }
            _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[4]);
            _data_1C4 = _bss_30->unkEC[4] - 0x210;
        } else if (_bss_30->unk10C == 4) {
            if (_bss_30->unk114 != 1) {
                _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[4]);
                _data_1C0 = _bss_30->unkEC[4] - 0x210;
            }
            _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[5]);
            _data_1C4 = _bss_30->unkEC[5] - 0x210;
        } else if (_bss_30->unk10C == 5) {
            if (_bss_30->unk114 != 1) {
                _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[5]);
                _data_1C0 = _bss_30->unkEC[5] - 0x210;
            }
            _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[6]);
            _data_1C4 = _bss_30->unkEC[6] - 0x210;
        } else if (_bss_30->unk10C == 6) {
            if (_bss_30->unk114 != 1) {
                _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[6]);
                _data_1C0 = _bss_30->unkEC[6] - 0x210;
            }
            _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[7]);
            _data_1C4 = _bss_30->unkEC[7] - 0x210;
        } else {
            if (_bss_30->unk114 != 1) {
                _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[7]);
                _data_1C0 = _bss_30->unkEC[7] - 0x210;
            }
            _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[0]);
            _data_1C4 = _bss_30->unkEC[0] - 0x210;
        }
        if (_bss_30->unk114 != 0) {
            _bss_30->unk114--;
        }
    } else if (_data_C8 != 0) {
        if (_bss_30->unk1C != NULL) {
            texFreeTexture(_bss_30->unk1C);
        }
        if (_bss_30->unk18 != NULL) {
            texFreeTexture(_bss_30->unk18);
        }
        if (_bss_30->unk10C < 7) {
            _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[_bss_30->unk10C - 1]);
            _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[_bss_30->unk10C]);
            _data_1C0 = _bss_30->unkEC[_bss_30->unk10C - 1] - 0x210;
            _data_1C4 = _bss_30->unkEC[_bss_30->unk10C] - 0x210;
        } else {
            _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[0]);
            _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[7]);
            _data_1C0 = _bss_30->unkEC[7] - 0x210;
            _data_1C4 = _bss_30->unkEC[0] - 0x210;
        }
    }
    _data_30 = sp44;
    if (sp30 > 1.0f) {
        sp30 = 1.0f;
    }
    if (sp30 < 0.0f) {
        sp30 = 0.0f;
    }
    _data_4C = _data_CC[_data_1C0][0] + ((f32) (_data_CC[_data_1C4][0] - _data_CC[_data_1C0][0]) * sp30);
    _data_50 = _data_CC[_data_1C0][1] + ((f32) (_data_CC[_data_1C4][1] - _data_CC[_data_1C0][1]) * sp30);
    _data_54 = _data_CC[_data_1C0][2] + ((f32) (_data_CC[_data_1C4][2] - _data_CC[_data_1C0][2]) * sp30);
    _data_40 = _data_CC[_data_1C0][3] + ((f32) (_data_CC[_data_1C4][3] - _data_CC[_data_1C0][3]) * sp30);
    _data_44 = _data_CC[_data_1C0][4] + ((f32) (_data_CC[_data_1C4][4] - _data_CC[_data_1C0][4]) * sp30);
    _data_48 = _data_CC[_data_1C0][5] + ((f32) (_data_CC[_data_1C4][5] - _data_CC[_data_1C0][5]) * sp30);
    if (_bss_30 == NULL) {
        sp3C = texLoadTexture(0x210);
        sp38 = texLoadTexture(0x210);
    } else {
        sp3C = _bss_30->unk18;
        sp38 = _bss_30->unk1C;
    }
    sp40 = camGet();
    fov = camGetFOV();

    sp5C = (((sp50 = (f32) (sp3C->height | ((sp3C->widthHeightHi & 0xF) << 8))) * (fov * 0.5f)) / 180.0f) * 3.0f;
    sp58 = mathCosfInterp((s16) (-sp40->roll)) * sp5C;
    fa1 = (((sp50 * 0.5f) - 6.0f) - ((3.0f * (sp50 * (f32) sp40->pitch)) / 32768.0f));
    sp64 = (fa1 + sp58) * 32.0f;
    gSPLoadGeometryMode(*gdl, 0);
    dlApplyGeometryMode(gdl);
    gDPLoadTextureBlockS((*gdl)++, 
        sp3C + 1, 
        G_IM_FMT_RGBA, 
        G_IM_SIZ_16b, 
        4, 
        256, 
        0, 
        G_TX_NOMIRROR | G_TX_CLAMP, 
        G_TX_NOMIRROR | G_TX_CLAMP, 
        G_TX_NOMASK, G_TX_NOMASK, 
        G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadMultiBlockS((*gdl)++, 
        sp38 + 1, 
        (s16) sp38->sizeBytes >> 3, 
        1, 
        G_IM_FMT_RGBA, 
        G_IM_SIZ_16b, 
        4, 
        256, 
        0, 
        G_TX_NOMIRROR | G_TX_CLAMP, 
        G_TX_NOMIRROR | G_TX_CLAMP, 
        G_TX_NOMASK, G_TX_NOMASK, 
        G_TX_NOLOD, G_TX_NOLOD);
    if (_data_C0 != NULL) {
        if (dll_7_func_56F8(gdl) == 0) {
            gDLL_8_newfog->vtbl->func5(gdl);
        }
    } else {
        gDLL_8_newfog->vtbl->func5(gdl);
    }
    dlSetEnvColor(gdl, 0xFF, 0xFF, 0xFF, (u8) (s16) (255.0f * sp30));
    gDPSetCombineLERP(*gdl, 
        TEXEL1, TEXEL0, ENV_ALPHA, TEXEL0, TEXEL1, TEXEL0, ENVIRONMENT, TEXEL0, 
        PRIMITIVE, COMBINED, PRIMITIVE_ALPHA, COMBINED, COMBINED, 0, PRIMITIVE, 0);
    dlApplyCombine(gdl);
    gDPSetOtherMode(*gdl, 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_2CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2);
    dlApplyOtherMode(gdl);
    temp_v0_8 = viGetCurrentSize();
    temp_t6 = (temp_v0_8 >> 0x10) & 0xFFFF;
    temp_t8 = (temp_v0_8 & 0xFFFF);
    sp58 /= (temp_t6 >> 1);
    sp58 *= 1024.0f;
    gSPTextureRectangle((*gdl)++, 
        0, 
        0, 
        temp_t8 << 2, 
        temp_t6 << 2, 
        G_TX_RENDERTILE, 
        0, 
        (s32)sp64, 
        qs510(1), 
        -(s32)sp58);
    gDLBuilder->needsPipeSync = 1;
    if (_bss_30 == NULL) {
        if (sp3C != NULL) {
            texFreeTexture(sp3C);
        }
        if (sp38 != NULL) {
            texFreeTexture(sp38);
        }
    }
    texRenderReset();
}

// offset: 0x20D4 | func: 14 | export: 14
void dll_7_func_20D4(u8* arg0, u8* arg1, u8* arg2) {
    if (_bss_30 != NULL) {
        *arg0 =  _data_34;
        *arg1 =  _data_38;
        *arg2 =  _data_3C;
    } else {
        *arg0 = 0xFF;
        *arg1 = 0xFF;
        *arg2 = 0xFF;
    }
}

// offset: 0x2130 | func: 15 | export: 15
void dll_7_func_2130(u8* arg0, u8* arg1, u8* arg2, u8* arg3, u8* arg4, u8* arg5) {
    if (_bss_30 != NULL) {
        *arg0 = _data_40;
        *arg1 = _data_44;
        *arg2 = _data_48;
        *arg3 = _data_4C;
        *arg4 = _data_50;
        *arg5 = _data_54;
    } else {
        *arg0 = 0xFF;
        *arg1 = 0xFF;
        *arg2 = 0xFF;
        *arg3 = 0xFF;
        *arg4 = 0xFF;
        *arg5 = 0xFF;
    }
}

/*0x1C8*/ static f32 _data_1C8 = 0.0;

// offset: 0x21CC | func: 16
static void dll_7_func_21CC(void) {
    f32 temp_fv1;
    f32 var_fv0;
    s32 i;

    if (_data_4 != 0) {
        temp_fv1 = (f32) _bss_30->unkCC * 10.0f;
        if (temp_fv1 < 100.0f) {
            _data_C8 = 1;
            _bss_30->timeSeconds = _data_8;
            _data_0 = 1.0f;
        } else {
            _data_1C8 = 1.0f / temp_fv1;
            var_fv0 = (f32) ((s32) _data_8 - (s32) _bss_30->timeSeconds);
            if (_bss_30->unk112 & 1) {
                _bss_30->unk112 ^= 1;
                var_fv0 = 0.0f;
            }
            if (var_fv0 < 0.0f) {
                var_fv0 = (86400.0f - _bss_30->timeSeconds) + _data_8;
            }
            _bss_28 = var_fv0 / temp_fv1;
            _bss_2C = (_data_C - _bss_30->unk40) / temp_fv1;
        }
        _data_4 = 0;
    }
    _bss_30->timeSeconds += _bss_28 * gUpdateRateF;
    _bss_30->unk40 += _bss_2C * gUpdateRateF;
    _data_0 += _data_1C8 * gUpdateRateF;
    if (_data_0 >= 1.0f) {
        _bss_30->unk112 ^= 0x40;
        _data_1C8 = 0.0f;
        _data_0 = 0.0f;
        for (i = 0; i < (3 * 7); i++) {
            ((f32*)_data_6C)[i] = ((f32*)_bss_30->unk44)[i];
        }
        _data_60 = (u32) _data_10;
        if (_bss_30->unkD8 != 0) {
            dll_7_func_30FC();
        }
    }
}

// offset: 0x2464 | func: 17
#ifndef NON_MATCHING
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
void dll_7_func_2464(EnvFxAction*, f32, f32, f32);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/7_newday/dll_7_func_2464.s")
#else
struct thing {
    s16 ob[3];
    s16 tc[2];
};
struct thing2 {
    s16 v[3];
    s16 _unk6[6];
};
void dll_7_func_2464(EnvFxAction* arg0, f32 arg1, f32 arg2, f32 arg3) {
    s32 var_v1;
    s32 i;
    s32 j;
    struct thing2 sp94[] = {
        {{0x0000, 0x0003, 0x0002}, {0x0000, 0x001f, 0x0000, 0x0000, 0x000f, 0x000f}}, 
        {{0x0003, 0x0004, 0x0002}, {0x0000, 0x0000, 0x001f, 0x0000, 0x000f, 0x000f}}, 
        {{0x0004, 0x0001, 0x0002}, {0x001f, 0x0000, 0x001f, 0x001f, 0x000f, 0x000f}}, 
        {{0x0001, 0x0000, 0x0002}, {0x001f, 0x001f, 0x0000, 0x001f, 0x000f, 0x000f}}
    };
    struct thing sp60[] = {
        {{0xffec, 0x0014, 0x0000}, {0x0000, 0x03e0}}, 
        {{0x0014, 0x0014, 0x0000}, {0x03e0, 0x03e0}}, 
        {{0x0000, 0x0000, 0x0000}, {0x01e0, 0x01e0}}, 
        {{0xffec, 0xffec, 0x0000}, {0x0000, 0x0000}}, 
        {{0x0014, 0xffec, 0x0000}, {0x03e0, 0x0000}}
    };
    struct thing sp2C[] = {
        {{0xffec, 0x0014, 0x0000}, {0x0000, 0x07e0}}, 
        {{0x0014, 0x0014, 0x0000}, {0x07e0, 0x07e0}}, 
        {{0x0000, 0x0000, 0x0000}, {0x03e0, 0x03e0}}, 
        {{0xffec, 0xffec, 0x0000}, {0x0000, 0x0000}}, 
        {{0x0014, 0xffec, 0x0000}, {0x07e0, 0x0000}}
    };

    _bss_30 = NULL;
    _bss_30 = mmAlloc(sizeof(NewDayStruct), ALLOC_TAG_SKY_COL, ALLOC_NAME("nday:tzones2"));
    if (_bss_30 != NULL) {
        _bss_30->unk20 = 0;
        _bss_30->unk2C = arg1;
        _bss_30->unk30 = arg2;
        _bss_30->unk34 = arg3;
        _bss_30->unkE0 = arg0->unk5D;
        _bss_30->unk110 = (u8) arg0->unk4E;
        _bss_30->unk111 = (u8) arg0->unk50;
        _bss_30->unkDC = arg0->unk52;
        _bss_30->unkDC = 0x168;
        _bss_30->timeSeconds = (f32) ((_bss_30->unk111 * 0x3C) + (_bss_30->unk110 * 0xE10));
        if (arg0->unk5D == 0) {
            _bss_30->unkC8 = 0.8f;
        } else if (arg0->unk5D == 1) {
            _bss_30->unkC8 = 1.0f;
        } else if (arg0->unk5D == 2) {
            _bss_30->unkC8 = 0.8f;
        } else if (arg0->unk5D == 3) {
            _bss_30->unkC8 = 0.55f;
        }
        if (arg0->unk54 == 0) {
            _bss_30->unk40 = _bss_30->unkC8 * 2000.0f;
        } else if (arg0->unk54 == 1) {
            _bss_30->unk40 = _bss_30->unkC8 * 10000.0f;
        } else if (arg0->unk54 == 2) {
            _bss_30->unk40 = _bss_30->unkC8 * 12000.0f;
        } else if (arg0->unk54 == 3) {
            _bss_30->unk40 = _bss_30->unkC8 * 14000.0f;
        }
        _bss_30->unkC4 = (f32) _bss_30->unkDC / 60.0f;
        _bss_30->unk112 = arg0->unk58;
        _bss_30->unkCC = 0;
        _bss_30->unkD0 = 0;
        _bss_30->unkE4 = mathRnd(0, 0x1C);
        _bss_30->unkE8 = (s32) arg0->unk56;
        _bss_30->unk0 = mmAlloc(0x100, ALLOC_TAG_GFX_COL, ALLOC_NAME("newday:NTri"));
        _bss_30->unk4 = mmAlloc(0x140, ALLOC_TAG_GFX_COL, ALLOC_NAME("newday:Vtx"));
        var_v1 = 0;
        for (i = 0; i < 5; i++) {
            _bss_30->unk4[var_v1].v.ob[0] = sp60[i].ob[0] * 1000;
            _bss_30->unk4[var_v1].v.ob[1] = sp60[i].ob[1] * 1000;
            _bss_30->unk4[var_v1].v.ob[2] = sp60[i].ob[2];
            _bss_30->unk4[var_v1].v.tc[0] = sp60[i].tc[0];
            _bss_30->unk4[var_v1].v.tc[1] = sp60[i].tc[1];
            _bss_30->unk4[var_v1].v.cn[0] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[1] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[2] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[3] = 0xFF;
            var_v1 += 1;
        }
        var_v1 = 5;
        for (i = 0; i < 5; i++) {
            _bss_30->unk4[var_v1].v.ob[0] = sp2C[i].ob[0] * 1000;
            _bss_30->unk4[var_v1].v.ob[1] = sp2C[i].ob[1] * 1000;
            _bss_30->unk4[var_v1].v.ob[2] = sp2C[i].ob[2];
            _bss_30->unk4[var_v1].v.tc[0] = sp2C[i].tc[0];
            _bss_30->unk4[var_v1].v.tc[1] = sp2C[i].tc[1];
            _bss_30->unk4[var_v1].v.cn[0] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[1] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[2] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[3] = 0xFF;
            var_v1 += 1;
        }
        var_v1 = 10;
        for (i = 0; i < 5; i++) {
            _bss_30->unk4[var_v1].v.ob[0] = sp60[i].ob[0] * 1000;
            _bss_30->unk4[var_v1].v.ob[1] = sp60[i].ob[1] * 1000;
            _bss_30->unk4[var_v1].v.ob[2] = sp60[i].ob[2];
            _bss_30->unk4[var_v1].v.tc[0] = sp60[i].tc[0] << 1;
            _bss_30->unk4[var_v1].v.tc[1] = sp60[i].tc[1] << 1;
            _bss_30->unk4[var_v1].v.cn[0] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[1] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[2] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[3] = 0xFF;
            var_v1 += 1;
        }
        var_v1 = 15;
        for (i = 0; i < 5; i++) {
            _bss_30->unk4[var_v1].v.ob[0] = sp2C[i].ob[0] * 1000;
            _bss_30->unk4[var_v1].v.ob[1] = sp2C[i].ob[1] * 1000;
            _bss_30->unk4[var_v1].v.ob[2] = sp2C[i].ob[2];
            _bss_30->unk4[var_v1].v.tc[0] = sp2C[i].tc[0];
            _bss_30->unk4[var_v1].v.tc[1] = sp2C[i].tc[1];
            _bss_30->unk4[var_v1].v.cn[0] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[1] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[2] = 0xFF;
            _bss_30->unk4[var_v1].v.cn[3] = 0xFF;
            var_v1 += 1;
        }
        _bss_30->unk114 = 0;
        var_v1 = 0;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                _bss_30->unk0[var_v1].v0 = sp94[j].v[0];
                _bss_30->unk0[var_v1].v1 = sp94[j].v[1];
                _bss_30->unk0[var_v1].v2 = sp94[j].v[2];
                var_v1++;
                arg0++; arg0--; // @fake
            }
        }
        _bss_30->unk44[0][0] = (f32) arg0->unkC;
        _bss_30->unk44[0][1] = (f32) arg0->unkC;
        _bss_30->unk44[0][2] = (f32) arg0->unkD;
        _bss_30->unk44[0][3] = (f32) arg0->unkE;
        _bss_30->unk44[0][4] = (f32) arg0->unkF;
        _bss_30->unk44[0][5] = (f32) arg0->unkC;
        _bss_30->unk44[0][6] = (f32) arg0->unkC;
        
        _bss_30->unk44[1][0] = (f32) arg0->unk14;
        _bss_30->unk44[1][1] = (f32) arg0->unk14;
        _bss_30->unk44[1][2] = (f32) arg0->unk15;
        _bss_30->unk44[1][3] = (f32) arg0->unk16;
        _bss_30->unk44[1][4] = (f32) arg0->unk17;
        _bss_30->unk44[1][5] = (f32) arg0->unk14;
        _bss_30->unk44[1][6] = (f32) arg0->unk14;
        
        _bss_30->unk44[2][0] = (f32) arg0->unk1C;
        _bss_30->unk44[2][1] = (f32) arg0->unk1C;
        _bss_30->unk44[2][2] = (f32) arg0->unk1D;
        _bss_30->unk44[2][3] = (f32) arg0->unk1E;
        _bss_30->unk44[2][4] = (f32) arg0->unk1F;
        _bss_30->unk44[2][5] = (f32) arg0->unk1C;
        _bss_30->unk44[2][6] = (f32) arg0->unk1C;
        _bss_30->unk8 = texLoadTexture(0x20D);
        _bss_30->unkC = texLoadTexture(0x20C);
        _bss_30->unk10 = texLoadTexture(0x20E);
        _bss_30->unk14 = texLoadTexture(0x20F);
        _bss_30->unkEC[0] = arg0->unk2E + 0x210;
        _bss_30->unkEC[1] = arg0->unk30 + 0x210;
        _bss_30->unkEC[2] = arg0->unk32 + 0x210;
        _bss_30->unkEC[3] = arg0->unk34 + 0x210;
        _bss_30->unkEC[4] = arg0->unk3E + 0x210;
        _bss_30->unkEC[5] = arg0->unk40 + 0x210;
        _bss_30->unkEC[6] = arg0->unk42 + 0x210;
        _bss_30->unkEC[7] = arg0->unk44 + 0x210;
        _bss_30->unk18 = texLoadTexture(_bss_30->unkEC[0]);
        _bss_30->unk1C = texLoadTexture(_bss_30->unkEC[1]);
        _bss_30->unk10C = 0;
        _data_30 = -1;
    }
}
#endif

// offset: 0x30FC | func: 18
static void dll_7_func_30FC(void) {
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
static void dll_7_func_3294(Gfx** gdl, Mtx** mtxs) {
    DLTri* spEC;
    Vtx* spE8;
    SRT spD0;
    SRT spB8;
    Camera* camera = camGet();
    f32 var_fv1_2;
    f32 spA4[3];
    f32 prevFarPlane;
    s16 var_v1;
    f32 sp90[3] = {0, 0, 4600.0f};
    f32 sp84[3] = {0, 0, 4600.0f};
    f32 sp80;
    s32 var_v0;
    f32 var_fv1;
    f32 sp74;
    f32 sp70;
    f32 var_fa0;
    s32 sp68;

    sp68 = 0;
    spD0.roll = 0;
    spD0.pitch = 0;
    spD0.yaw = 0;
    spD0.scale = 1.0f;
    spD0.transl.x = 0/*.0f*/;
    spD0.transl.y = 0/*.0f*/;
    spD0.transl.z = 0/*.0f*/;
    spB8.roll = 0;
    spB8.pitch = 0;
    spB8.yaw = 0;
    spB8.scale = 1.0f;
    spB8.transl.x = 0/*.0f*/;
    spB8.transl.y = 0/*.0f*/;
    spB8.transl.z = 0/*.0f*/;
    gDLL_7_Newday->vtbl->func7(&sp68);
    if (camera == NULL || _bss_30 == NULL) {
        return;
    }

    prevFarPlane = camGetFarPlane();
    camSetFarPlaneLerp(15000.0f, 0);
    var_fv1 = (_data_5C - 18000.0f) / 57600.0f;
    if (var_fv1 < 0/*.0f*/) {
        var_fv1 = 0/*.0f*/;
    } else if (var_fv1 > 1.0f) {
        var_fv1 = 1.0f;
    }
    if (var_fv1 < 0.1f) {
        if (var_fv1 < 0/*.0f*/) {
            _data_64 = 0;
        } else {
            _data_64 = var_fv1 * 2550.0f;
        }
    } else if (var_fv1 > 0.9f) {
        if (var_fv1 > 1.0f) {
            _data_64 = 0;
        } else {
            _data_64 = (0.1f - (var_fv1 - 0.9f)) * 2550.0f;
        }
    } else {
        _data_64 = 0xFF;
    }
    var_fv1 *= 32676.0f;
    sp74 = (_data_5C - 18000.0f) / 28800.0f;
    if (sp74 < 0/*.0f*/) {
        sp74 = 0/*.0f*/;
    }
    if (sp74 > 1.0f) {
        sp74 = 1.0f - (sp74 - 1.0f);
    }
    sp74 = 1.0f - (0.55f * sp74);
    spA4[0] = 2.0f * sp90[0];
    spA4[1] = 2.0f * sp90[1];
    spA4[2] = 2.0f * sp90[2];
    sp80 = _bss_30->unk40;
    spD0.yaw = var_fv1;
    mathRotateRPY(&spD0, spA4);
    spD0.pitch = 0;
    spD0.yaw = 0;
    spD0.scale = 1.0f;
    spD0.roll = (s16) sp80;
    mathRotateRPY(&spD0, spA4);
    _data_18.x = spA4[0];
    _data_18.y = spA4[1];
    _data_18.z = spA4[2];
    spD0.transl.x = (s16)spA4[0] + camera->tx;
    spD0.transl.y = (s16)spA4[1] + camera->ty;
    spD0.transl.z = (s16)spA4[2] + camera->tz;
    spD0.scale = 0.05f * sp74;
    spD0.roll = 0;
    spD0.yaw = 0x10000 - camera->srt.yaw;
    spD0.pitch = camera->srt.pitch;
    if (trackGetSunGlareOn() != 0) {
        dll_7_func_5124(spD0.transl.x, spD0.transl.y, spD0.transl.z);
    } else if (_data_C0 != NULL) {
        objFreeObject(_data_C0);
        _data_C0 = NULL;
    }
    var_fa0 = _data_5C;
    if (var_fa0 >= 75600.0f) {
        var_fa0 -= 75600;
    } else {
        var_fa0 += 10800.0f;
    }
    var_fv1_2 = var_fa0 / 28800.0f;
    if (var_fv1_2 < 0/*.0f*/) {
        var_fv1_2 = 0/*.0f*/;
    } else if (var_fv1_2 > 1.0f) {
        var_fv1_2 = 1.0f;
    }
    if (var_fv1_2 < 0.1f) {
        if (var_fv1_2 < 0/*.0f*/) {
            _data_68 = 0;
        } else {
            _data_68 = var_fv1_2 * 2550.0f;
        }
    } else if (var_fv1_2 > 0.9f) {
        if (var_fv1_2 > 1.0f) {
            _data_68 = 0;
        } else {
            _data_68 = (0.1f - (var_fv1_2 - 0.9f)) * 2550.0f;
        }
    } else {
        _data_68 = 0xFF;
    }
    
    var_fv1_2 *= 32676.0f;
    sp70 = var_fa0 / 14400.0f;
    if (sp70 < 0/*.0f*/) {
        sp70 = 0/*.0f*/;
    }
    if (sp70 > 1.0f) {
        sp70 = 1.0f - (sp70 - 1.0f);
    }
    sp70 = 1.0f - (0.55f * sp70);
    spA4[0] = 2.0f * sp84[0];
    spA4[1] = 2.0f * sp84[1];
    spA4[2] = 2.0f * sp84[2];
    spB8.yaw = var_fv1_2;
    mathRotateRPY(&spB8, spA4);
    spB8.pitch = 0;
    spB8.yaw = 0;
    spB8.scale = 1.0f;
    spB8.roll = (s16) sp80;
    mathRotateRPY(&spB8, spA4);
    _data_24.x = spA4[0];
    _data_24.y = spA4[1];
    _data_24.z = spA4[2];
    spB8.transl.x = (s16)spA4[0] + camera->tx;
    spB8.transl.y = (s16)spA4[1] + camera->ty;
    spB8.transl.z = (s16)spA4[2] + camera->tz;
    spB8.scale = 0.05f * sp70;
    spB8.roll = 0;
    spB8.yaw = 0x10000 - camera->srt.yaw;
    spB8.pitch = camera->srt.pitch;
    for (var_v0 = 0; var_v0 < 5; var_v0++) {
        _bss_30->unk4[var_v0].v.cn[0] = 0xFF;
        _bss_30->unk4[var_v0].v.cn[1] = 0xFF;
        _bss_30->unk4[var_v0].v.cn[2] = 0xFF;
        _bss_30->unk4[var_v0].v.cn[3] = 0xFF;
    }
    
    for (var_v0 = 5; var_v0 < 10; var_v0++) {
        _bss_30->unk4[var_v0].v.cn[0] = 0xFF;
        _bss_30->unk4[var_v0].v.cn[1] = 0xFF;
        _bss_30->unk4[var_v0].v.cn[2] = 0xFF;
        _bss_30->unk4[var_v0].v.cn[3] = 0xFF;
    }
    
    var_fa0 = 1.0f - sp70;
    for (var_v0 = 10; var_v0 < 15; var_v0++) {
        if (var_v0 == 0) {
            _bss_30->unk4[var_v0].v.cn[0] = 0xFF;
            _bss_30->unk4[var_v0].v.cn[1] = 0xFF;
            _bss_30->unk4[var_v0].v.cn[2] = 0xFF;
        } else {
            _bss_30->unk4[var_v0].v.cn[0] = 0xFF;
            _bss_30->unk4[var_v0].v.cn[1] = (155/*.0f*/) + (var_fa0 * 100/*.0f*/);
            _bss_30->unk4[var_v0].v.cn[2] = (105/*.0f*/) + (var_fa0 * 150/*.0f*/);
        }
        _bss_30->unk4[var_v0].v.cn[3] = 0xFF;
    }

    for (var_v0 = 15; var_v0 < 20; var_v0++) {
        _bss_30->unk4[var_v0].v.cn[0] = 0xFF;
        _bss_30->unk4[var_v0].v.cn[1] = (205/*.0f*/) + (var_fa0 * 50/*.0f*/);
        _bss_30->unk4[var_v0].v.cn[2] = (155/*.0f*/) + (var_fa0 * 100/*.0f*/);
        _bss_30->unk4[var_v0].v.cn[3] = 0xFF;
    }
    
    gDLL_8_newfog->vtbl->func5(gdl);
    gSPLoadGeometryMode(*gdl, G_SHADE | G_SHADING_SMOOTH);
    dlApplyGeometryMode(gdl);
    gDPSetCombineLERP(*gdl, TEXEL0, 0, SHADE, 0, TEXEL0, 0, SHADE, 0, PRIMITIVE, COMBINED, PRIMITIVE_ALPHA, COMBINED, COMBINED, 0, ENVIRONMENT, 0);
    dlApplyCombine(gdl);
    gDPSetOtherMode(*gdl,
        G_AD_PATTERN | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
    dlApplyOtherMode(gdl);
    if (_data_64 > 0) {
        spD0.scale = 0.05f * sp74;
        spEC = _bss_30->unk0;
        spE8 = _bss_30->unk4;
        camSetupObjectSRTMatrix(gdl, mtxs, &spD0, 1.0f, 0.0f, NULL);
        texDPTextures(gdl, _bss_30->unk8, NULL, RENDER_Z_COMPARE | RENDER_ANTI_ALIASING, 0, FALSE, FALSE);
        dlSetEnvColor(gdl, 0xFF, 0xFF, 0xFF, (u8) _data_64);
        gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(spE8), 5, 0);
        dlTriangles(gdl, spEC, 4);
        spD0.scale = 0.1f;
        spEC = _bss_30->unk0;
        spE8 = _bss_30->unk4 + 5;
        camSetupObjectSRTMatrix(gdl, mtxs, &spD0, 1.0f, 0.0f, NULL);
        texDPTextures(gdl, _bss_30->unk14, NULL, RENDER_Z_COMPARE | RENDER_ANTI_ALIASING, 0, FALSE, FALSE);
        var_v1 = (s16)var_fv1 >> 6;
        if (var_v1 > 0xFF) {
            var_v1 = 0xFF;
        }
        dlSetPrimColor(gdl, 0xFF, (u8) (((f32) var_v1 / 2.5f) + 150.0f), var_v1, (u8) _data_64);
        gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(spE8), 5, 0);
        dlTriangles(gdl, spEC, 4);
    }
    gDLL_8_newfog->vtbl->func5(gdl);
    if (_data_68 > 0) {
        spB8.scale = 0.04f * sp70;
        spEC = _bss_30->unk0;
        spE8 = _bss_30->unk4 + 10;
        camSetupObjectSRTMatrix(gdl, mtxs, &spB8, 1.0f, 0.0f, NULL);
        texDPTextures(gdl, _bss_30->unk10, NULL, RENDER_Z_COMPARE | RENDER_ANTI_ALIASING, 0, FALSE, FALSE);
        dlSetEnvColor(gdl, 0xFF, 0xFF, 0xFF, (u8) _data_68);
        gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(spE8), 5, 0);
        dlTriangles(gdl, spEC, 4);
        spB8.scale = 0.1f;
        camSetupObjectSRTMatrix(gdl, mtxs, &spB8, 1.0f, 0.0f, NULL);
        var_v1 = (s16)var_fv1_2 >> 6;
        if (var_v1 > 0xFF) {
            var_v1 = 0xFF;
        }
        texDPTextures(gdl, _bss_30->unk14, NULL, RENDER_Z_COMPARE | RENDER_ANTI_ALIASING, 0, FALSE, FALSE);
        dlSetPrimColor(gdl, 0xFF, ((var_v1 / 5) + 0xC8), (u8) (((f32) var_v1 / 2.5f) + 150.0f), (u8) (_data_68 / 2));
    }
    camSetFarPlaneLerp(prevFarPlane, 0);
}

// offset: 0x4484 | func: 20
static void dll_7_func_4484(void) {
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
    sp70 = (s32) curvesCatmullRom(&_bss_30->unk44[0][sp4C], sp88, NULL);
    sp6C = (s32) curvesCatmullRom(&_bss_30->unk44[1][sp4C], sp88, NULL);
    sp68 = (s32) curvesCatmullRom(&_bss_30->unk44[2][sp4C], sp88, NULL);
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
    _data_34 = sp70;
    _data_38 = sp6C;
    _data_3C = sp68;
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
static void dll_7_func_5124(f32 x, f32 y, f32 z) {
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
static s32 dll_7_func_56F8(Gfx** gdl) {
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
static void dll_7_func_57C0(void) {
    bzero(&_bss_0, sizeof(_bss_0));
    _bss_0.unk26 = mainGetBits(BIT_SC_UNKNOWN_2BA);
}

// offset: 0x5818 | func: 24
static void dll_7_func_5818(void) {
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
