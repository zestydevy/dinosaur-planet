#include "PR/ultratypes.h"
#include "libnaudio/n_libaudio.h"
#include "libnaudio/n_sndplayer.h"
#include "libnaudio/mp3/segment_67F50.h"
#include "sys/acache.h"
#include "sys/fs.h"
#include "sys/map.h"
#include "mpeg_fs.h"
#include "common.h"

/*0x0*/ static const char str_0[] = "AMSFX: Error sound effects bank missing!\n";
/*0x2C*/ static const char str_2C[] = "amSfxPlayEx failed\n";
/*0x40*/ static const char str_40[] = "amSfxSetVol: Warning,sound handle '%d' out of range.\n";
/*0x78*/ static const char str_78[] = "amSfxSetVol: Warning,invalid handle '%d'.\n";
/*0xA4*/ static const char str_A4[] = "amSfxSetPitch: Warning,sound handle '%d' out of range.\n";
/*0xDC*/ static const char str_DC[] = "amSfxSetPitch: Warning,invalid handle '%d'.\n";
/*0x10C*/ static const char str_10C[] = "amSfxStop: Warning,sound handle '%d' out of range.\n";
/*0x140*/ static const char str_140[] = "amSfxIsPlaying: Warning,sound handle '%d' out of range.\n";
/*0x17C*/ static const char str_17C[] = "amSfxGetDefault: Warning,sound effect value '%d' out of range.\n";
/*0x1BC*/ static const char str_1BC[] = "amSndPlayEx: Warning,sound '%d' out of range.\n";
/*0x1EC*/ static const char str_1EC[] = "amSndPlayEx: Warning,sound handle '%d' out of range.\n";

/*0x0*/ static u32 _data_0[4] = { 0 };

typedef struct UnkBss4 {
    u16 pad0;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 pad6;
    u8 unk7;
    u8 pad8[0xE - 0x8];
    s16 unkE;
    u16 pad10;
    u8 unk12;
    u8 unk13;
    s8 unk14;
    s8 unk15;
    s8 unk16;
    s8 unk17;
    Object *unk18;
    sndstate *unk1C;
} UnkBss4;

typedef struct UnkBss28 {
    Vec3f pos;
    u16 unkC;
    u16 unkE;
} UnkBss28;

/*0x0*/ static ALBankFile *_bss_0;
/*0x4*/ static UnkBss4 *_bss_4;
/*0x8*/ static s32 _bss_8;
/*0xC*/ static u32 _bss_C;
/*0x10*/ static u32 _bss_10;
/*0x14*/ static u32 _bss_14;
/*0x18*/ static u32 _bss_18;
/*0x1C*/ static u8 _bss_1C;
/*0x1D*/ static u8 _bss_1D;
/*0x1E*/ static u8 _bss_1E;
/*0x1F*/ static u8 _bss_1F; // sound volume
/*0x20*/ static u8 _bss_20[0x8]; // sound volume(s?)
/*0x28*/ static UnkBss28 _bss_28[0x10];
/*0x128*/ static ACache *_bss_128;

void dll_6_func_860(u32, u8);
void dll_6_func_A1C(u32);
s32 dll_6_func_DE8(u16, UnkDE8 *);
static s32 dll_6_func_1C38(void);
static s32 dll_6_func_1D58(s32, char *, s32);
static s32 dll_6_func_1E64(u32);
static void dll_6_func_1F78(void);
static void dll_6_func_2240(Object*, f32*, f32*, f32*, u16*);
static void dll_6_func_22FC(f32, f32, f32, UnkDE8*, s8*);
/* static */ void dll_6_func_2438(f32 arg0, f32 arg1, s32 arg2, u8* arg3, u8* arg4);

// offset: 0x0 | ctor
void dll_6_ctor(s32 arg0) {
    u32* sp4C;
    s32 temp_v0;
    s32 temp_a3;
    ALSndpConfig sp30;

    _bss_14 = 0;
    _bss_18 = 0;
    _bss_1C = 0;
    _bss_1D = 0;
    sp4C = NULL;
    sp30.maxEvents = 0x96;
    sp30.maxStates = 0x20;
    sp30.maxSounds = 0x20;
    sp30.unk10 = 1;
    _bss_1E = 0;
    n_alSndpNew(&sp30);
    queue_alloc_load_file((void*)&sp4C, SFX_TAB);
    temp_v0 = sp4C[0];
    temp_a3 = sp4C[1] - temp_v0;
    if (temp_a3 != 0) {
        _bss_0 = mmAlloc(temp_a3, ALLOC_TAG_SFX_COL, NULL);
        queue_load_file_region_to_ptr((void*)_bss_0, SFX_BIN, temp_v0, temp_a3);
        alBnkfNew(_bss_0, (u8*)file_get_romaddr(SFX_BIN, sp4C[1]));
    }
    mmFree(sp4C);
    _bss_128 = acache_init(AUDIO_BIN, 0x40U, 0xE, 0x40U, 0);
    queue_alloc_load_file((void*)&sp4C, AUDIO_TAB);
    _bss_C = sp4C[0];
    _bss_10 = sp4C[1] - _bss_C;
    _bss_10 = _bss_10 / 14;
    mmFree(sp4C);

    _bss_8 = 0x41;
    temp_a3 = _bss_8 << 5;
    _bss_4 = mmAlloc(temp_a3, ALLOC_TAG_SFX_COL, NULL);
    bzero(_bss_4, temp_a3);
    _bss_8 = 0;
}

// offset: 0x22C | dtor
void dll_6_dtor(s32 arg0) {
    s32 i;
    s32 var_v0;

    i = 1;
    _bss_14 = 0;
    _bss_18 = 0;
    _bss_1C = 0;
    _bss_1D = 0;
    _bss_1E = 0;
    for (i = 1; i < (_bss_8 + 1); i++) {
        if (_bss_4[i].unk1C) {
            dll_6_func_1E64(i);
        }
    }
    mmFree(_bss_4);
    _bss_8 = 0;
    mmFree(_bss_0);
}


// offset: 0x338 | func: 0 | export: 0
#ifndef NON_MATCHING
// Matches requires dll_6_func_1F78 as static
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_338.s")
#else
void dll_6_func_338(void) {
    sndstate *temp_a0;
    s32 i;

    dll_6_func_1F78();
    for (i = 1; i < (_bss_8 + 1); i++) {
        temp_a0 = _bss_4[i].unk1C;
        // @fake
        if (1) {}
        if (temp_a0 != 0) {
            if (temp_a0 == (sndstate*)-1) {
                if (_bss_4[i].unk12 & 0x80) {
                    dll_6_func_1E64(i);
                }
            } else if (temp_a0 == (sndstate*)-2) {
                if (func_800675EC(temp_a0) == 0) {
                    _bss_4[i].unk1C = -1;
                }
            } else if (sndGetState(temp_a0) == 0) {
                _bss_4[i].unk1C = -1;
            }
        }
    }
}
#endif

// offset: 0x480 | func: 1 | export: 1
void dll_6_func_480(s32 arg0) {
}

// offset: 0x48C | func: 2 | export: 2
u32 dll_6_play_sound(Object* obj, u16 soundID, u8 volume, u32* arg3, char *arg4, s32 arg5, char *arg6) {
    UnkDE8 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    u32 temp_v0;
    u16 sp46;
    s8 sp45;

    _bss_4->unk12 = 0;
    _bss_4->unk1C = NULL;
    if (!soundID) {
        return 0;
    }
    dll_6_func_DE8(soundID, &sp58);
    if (!(sp58.unk0 & 0x7FFF)) {
        return 0;
    }
    if (arg3 != NULL) {
        temp_v0 = arg3[0];
    } else {
        temp_v0 = 0;
    }
    temp_v0 = dll_6_func_1D58(temp_v0, arg4, arg5);
    _bss_4[temp_v0].unkE = soundID;
    _bss_4[temp_v0].unk18 = obj;
    _bss_4[temp_v0].unk17 = 0x7F;
    if ((obj != NULL) && (sp58.unk7 & 3)) {
        dll_6_func_2240(obj, &sp54, &sp50, &sp4C, &sp46);
        dll_6_func_22FC(sp54, sp50, sp4C, &sp58, &sp45);
        _bss_4[temp_v0].unk17 = sp45;
    }
    _bss_4[temp_v0].unk16 = (volume * sp58.unk2) >> 7;
    sp45 = (_bss_4[temp_v0].unk16 * _bss_4[temp_v0].unk17) >> 7;
    _bss_4[temp_v0].unk13 = sp45;
    if (sp58.unk0 & 0x8000) {
        _bss_4[temp_v0].unk1C = (sndstate* )-2;
        // @fake
        if (_bss_4[temp_v0].pad0) {}
        mpeg_fs_play((sp58.unk0 & 0x7FFF) - 1);
        func_80067650(sp45 << 8, 0);
        // @fake
        if (_bss_4) {}
    } else {
        some_sound_func(_bss_0->bankArray[0], sp58.unk0, (sp45 << 8), 0x40, sp58.unk4 / 100.0f, (s32)(f32)sp58.unk6, 1U, &_bss_4[temp_v0].unk1C);
    }
    bcopy(&sp58, &_bss_4[temp_v0], 0xE);
    bcopy(&_bss_4[temp_v0], _bss_4, sizeof(UnkBss4));
    if (arg3 != NULL) {
        arg3[0] = temp_v0;
    } else {
        _bss_4[temp_v0].unk12 |= 0x80;
    }
    return temp_v0;
}

// offset: 0x7E4 | func: 3 | export: 3
void dll_6_func_7E4(u8 arg0) {
    s32 i;

    for (i = 0; i < 1; i++) {
        func_80066978(i, arg0 << 8);
    }
}

// offset: 0x860 | func: 4 | export: 4
void dll_6_func_860(u32 arg0, u8 arg1) {
    sndstate* temp_a0;
    u8 temp_t6;

    if (1) {
        if ((u32)_bss_8 < arg0) {
            return;
        }
        
        temp_a0 = _bss_4[arg0].unk1C;
        if (temp_a0 == NULL) {
            return;
        }
        
        _bss_4[arg0].unk16 = (_bss_4[arg0].unk2 * arg1) >> 7;
    }

    temp_t6 = (_bss_4[arg0].unk16 * _bss_4[arg0].unk17) >> 7;
    if (temp_t6 != (s8) _bss_4[arg0].unk13) {
        _bss_4[arg0].unk13 = temp_t6;
        if (temp_a0 == (sndstate* )-2) {
            func_80067650(temp_t6 << 8, arg0 * 0);
        } else if (temp_a0 != (sndstate* )-1) {
            audioPostEvent(temp_a0, 8, temp_t6 << 8);
        }
    }
}

// offset: 0x954 | func: 5 | export: 5
void dll_6_func_954(u32 arg0, s32 arg1) {
    UnkBss4* temp_v0;
    sndstate* temp_a0;

    if ((u32)_bss_8 < arg0) {
        return;
    }

    temp_a0 = _bss_4[arg0].unk1C;
    if (temp_a0 == NULL) {
        return;
    }

    (f32)arg1 *= _bss_4[arg0].unk4 / 100.0f;
    if ((temp_a0 != (sndstate* )-2) && (temp_a0 != (sndstate* )-1)) {
        audioPostEvent(temp_a0, 0x10, arg1);
    }
}


// offset: 0xA1C | func: 6 | export: 6
void dll_6_func_A1C(u32 arg0) {
    if ((u32) _bss_8 >= arg0) {
        dll_6_func_1E64(arg0);
    }
}

// offset: 0xA6C | func: 7 | export: 7
void dll_6_func_A6C(Object *arg0) {
    s32 i;

    if (arg0 == 0) {
        return;
    }

    // @fake
    if (1) {}
    for (i = 1; i < (_bss_8+1); i++) {
        if ((_bss_4[i].unk12 & 0x40) != 0 && arg0 == _bss_4[i].unk18 && _bss_4[i].unk1C != NULL) {
            dll_6_func_1E64(i);
        }
    }
}

// offset: 0xB48 | func: 8 | export: 8
s32 dll_6_func_B48(u32 arg0) {
    if (arg0 == 0) {
        return 0;
    }

    if ((u32) _bss_8 < arg0) {
        return 0;
    }

    if (_bss_4[arg0].unk1C == NULL || _bss_4[arg0].unk1C == (sndstate* )-1) {
        return 0;
    }

    return 1;
}

// offset: 0xBBC | func: 9 | export: 9
s32 dll_6_func_BBC(Object* arg0, s32 arg1, s32 arg2, Object **arg3, s32 arg4) {
    Object* temp_s0;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 i;
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    f32 sp60;
    s32 outCount;

    if (arg1 != -1) {
        arg1 *= arg1;
    }

    outCount = 0;
    for (i = 1; i < (_bss_8 + 1); i++) {
        if ((_bss_4[i].unk1C == 0) || (_bss_4[i].unk1C == (sndstate*)-1)) {
            continue;
        }
        temp_s0 = _bss_4[i].unk18;
        if (temp_s0 == NULL) {
            continue;
        }
        if ((arg2 != -1) && (_bss_4[i].unk16 < arg2)) {
            continue;
        }
        if (arg1 != -1) {
            get_object_child_position(arg0, &sp74, &sp70, &sp6C);
            get_object_child_position(temp_s0, &sp68, &sp64, &sp60);
            temp_fv1 = sp74 - sp68;
            temp_fv0 = sp70 - sp64;
            if (arg1 < (SQ(temp_fv1) + SQ(temp_fv0) + ((sp6C - sp60) * temp_fv0))) {
                continue;
            }
        }
        arg3[outCount++] = _bss_4[i].unk18;
        if (outCount == arg4) {
            break;
        }
    }

    return outCount;
}

// offset: 0xDCC | func: 10 | export: 10
s32 dll_6_func_DCC(s32 arg0, UNK_TYPE_32 arg1, s32* arg2, s32* arg3) {
    *arg2 = 0;
    *arg3 = 0;
    return 0;
}

// offset: 0xDE8 | func: 11 | export: 11
s32 dll_6_func_DE8(u16 arg0, UnkDE8* arg1) {
    void* temp_v0;

    if (arg0 <= 0 || (s32)_bss_10 < arg0) {
        bzero(arg1, 0xE);
        return 0;
    }

    arg0--;
    temp_v0 = acache_get(_bss_128, arg0);
    if (temp_v0 != NULL) {
        bcopy(temp_v0, arg1, 0xE);
    } else {
        return 0;
    }

    if (arg1->unk2 == 0) {
        arg1->unk2 = 0x3F;
    }
    if (arg1->unk4 == 0) {
        arg1->unk4 = 0x64;
    }
    if (arg1->unk5 == 0) {
        arg1->unk5 = 0x40;
    }

    return 1;
}

// offset: 0xEE4 | func: 12 | export: 12
s32 dll_6_func_EE4(s32 arg0, UNK_TYPE_32 arg1) {
    acache_func_80000D50(_bss_128);
    return 1;
}

// offset: 0xF2C | func: 13 | export: 13
void dll_6_func_F2C(Object* arg0, u16 arg1, u32* arg2, char *arg3, s32 arg4) {
    u32 temp_v0_2;

    if (!arg1) {
        return;
    }

    if (_bss_0->bankArray[0]->instArray[0]->soundCount < arg1) {
        return;
    }

    if (arg2 == NULL) {
        temp_v0_2 = 0;
    } else if ((u32) _bss_8 >= *arg2) {
        temp_v0_2 = *arg2;
    } else {
        return;
    }
    
    temp_v0_2 = dll_6_func_1D58(temp_v0_2, arg3, arg4);
    some_sound_func(_bss_0->bankArray[0], arg1, 0x4000U, 0x40U, 1.0f, 0U, 1U, &_bss_4[temp_v0_2].unk1C);
    _bss_4[temp_v0_2].unkE = -1 - arg1;
    _bss_4[temp_v0_2].unk18 = arg0;
    bcopy(&_bss_4[temp_v0_2], _bss_4, sizeof(UnkBss4));
    if (arg2 != NULL) {
        *arg2 = temp_v0_2;
        return;
    }
    _bss_4[temp_v0_2].unk12 |= 0x80;
}

void dll_6_func_10D0(Object* obj, u16 soundID, u32* arg2) {
    if (arg2 != NULL) {
        dll_6_play_sound(obj, soundID, MAX_VOLUME, arg2, "game/amsfx.c", 0x2BD, "");
        return;
    }

    dll_6_play_sound(obj, soundID, MAX_VOLUME, NULL, "game/amsfx.c", 0x2BE, "");
}

// offset: 0x1174 | func: 15 | export: 15
void dll_6_func_1174(Object* obj, u16 soundID, u8 volume, u32* arg3) {
    if (arg3 != NULL) {
        dll_6_play_sound(obj, soundID, volume, arg3, "game/amsfx.c", 0x2C5, "");
        return;
    }

    dll_6_play_sound(obj, soundID, volume, NULL, "game/amsfx.c", 0x2C6, "\0\0\0\0AMSFX: Software bug in object (controlfunc %d)\n\0       amSfxStop is required to release effect %d.\n\0AMSFX: Software bug in object (controlfunc %d)\n\0       amSfxStop is required to stop effect %d.\n");
}

// offset: 0x1218 | func: 16 | export: 16
void dll_6_func_1218(Object *obj) {
    s32 i;
    s32 temp = (!obj) && (!obj);

    if (!obj) {
        return;
    }

    for (i = 1; i < (_bss_8 + 1); i++) { 
        if (!(_bss_4[i].unk12 & 0x40)) { continue; }
        // @fake
        if (((0, obj)) != _bss_4[i].unk18) { continue; }
        if (!(_bss_4[i].unk12 & 0x80)) {
            // @fake
            if (_bss_4[i].unk1C) {}
            if (_bss_4[i].unk1C != 0) {
                dll_6_func_1E64(i);
            }
        }
        dll_6_func_1E64(i);
        // @fake
        if (temp && (!obj)) {}
    }
}


#ifndef NON_EQUIVALENT
// offset: 0x1320 | func: 17 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1320.s")
#else
// https://decomp.me/scratch/eQMa4
void dll_6_func_1320(Object* arg0, u16 arg1, s32 arg2, f32 arg3, f32 arg4, u32* arg5) {
    f32 temp3;
    f32 temp2;
    f32 temp;
    f32 zDiff;
    f32 yDiff;
    f32 xDiff;
    s32 soundVolume;
    Camera* sp30;

    sp30 = get_camera_array();
    update_camera_for_object(sp30);
    if (sp30) {}
    xDiff = arg0->positionMirror.x - sp30->tx;
    yDiff = arg0->positionMirror.y - sp30->ty;
    zDiff = arg0->positionMirror.z - sp30->tz;
    temp = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
    if (arg4 < temp) {
        soundVolume = 0;
    } else if (temp <= arg3) {
        soundVolume = MAX_VOLUME;
    } else {
        temp2 = arg4 - arg3;
        temp3 = temp2 - (temp - arg3);
        temp3 = ((temp3 * 127.0f) / temp2);
        soundVolume = temp3;
        if (soundVolume > MAX_VOLUME) {
            soundVolume = MAX_VOLUME;
        }
        if (soundVolume < 0) {
            soundVolume = 0;
        }
    }
    if ((soundVolume > 0) && (*arg5 == 0)) {
        dll_6_play_sound(arg0, arg1, soundVolume, arg5, "game/amsfx.c", 0x37B, "\0\0\0\0amSfxSetPitch: Warning,sound handle '%d' out of range.\n\0amSfxSetPitch: Warning,invalid handle '%d'.\n\0");
        dll_6_func_954(*arg5, arg2);
    }
    if (*arg5 != 0) {
        if (soundVolume == 0) {
            dll_6_func_A1C(*arg5);
            *arg5 = 0;
            return;
        }
        dll_6_func_860(*arg5, soundVolume);
    }
}
#endif

void dll_6_func_1504(s32 arg0, Object* arg1, Object* arg2, f32 arg3) {
    s32 pad2;
    s32 sp40;
    f32 sp3C;
    f32 temp_fa1;
    s32 pad[2];
    f32 temp_fv0;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 temp_fv1;
    sndstate* temp_a0;

    if (arg0 == 0) {
        return;
    }

    temp_fv0 = arg2->srt.transl.x - arg1->srt.transl.x;
    temp_fv1 = arg2->srt.transl.y - arg1->srt.transl.y;
    temp_fa1 = arg2->srt.transl.z - arg1->srt.transl.z;
    sp28 = arg2->positionMirror2.x - arg1->positionMirror2.x;
    sp24 = arg2->positionMirror2.y - arg1->positionMirror2.y;
    sp20 = arg2->positionMirror2.z - arg1->positionMirror2.z;
    sp3C = sqrtf(SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa1));
    temp_fv1 = ((sqrtf(SQ(sp28) + SQ(sp24) + SQ(sp20)) - sp3C) / arg3) + 1.0f;
    if (temp_fv1 < 0.1f) {
        (f32)sp40 = 0.1f;
    } else {
        (f32)sp40 = temp_fv1;
        if (temp_fv1 > 1.9f) {
            (f32)sp40 = 1.9f;
        }
    }
    if (_bss_8 >= arg0) {
        temp_a0 = _bss_4[arg0].unk1C;
        if (temp_a0 != NULL) {
            (f32)sp40 *= _bss_4[arg0].unk4 / 100.0f;
            if ((temp_a0 != (sndstate* )-2) && (temp_a0 != (sndstate* )-1)) {
                audioPostEvent(temp_a0, 0x10, sp40);
            }
        }
    }
}

// offset: 0x16E0 | func: 19 | export: 18
void dll_6_func_16E0(void) {
    Object* temp_v0;
    s32 i;
    f32 temp_fv0;
    u32 sp50;
    u32 sp4C;
    u8 temp_v0_2;
    Vec3f* sp40;

    temp_v0 = get_player();
    temp_v0_2 = map_get_map_id_from_xz_ws(temp_v0->srt.transl.x, temp_v0->srt.transl.z);
    if ((temp_v0_2 != _bss_1C) || (_bss_1E & 0x10)) {
        dll_6_func_1C38();
        _bss_1C = temp_v0_2;
        _bss_1E &= ~0x10;
    }
    if (_bss_1D == 0) {
        return;
    }

    sp40 = (Vec3f*)get_camera();
    sp4C = 0;
    sp50 = 0;
    for (i = 0; i < _bss_1D; i++) {
        temp_fv0 = vec3_distance(sp40 + 1, (Vec3f* ) &_bss_28[i]);
        if (temp_fv0 < _bss_28[i].unkC) {
            sp50 += 0x7F - (u8)((u32)((temp_fv0 / _bss_28[i].unkC) * 127.0f));
        }
        if (temp_fv0 < _bss_28[i].unkE) {
            sp4C += 0x7F - (u8)((u32)((temp_fv0 / _bss_28[i].unkE) * 127.0f));
        }
    }
    if (_bss_1E & 1) {
        sp4C >>= 1;
    }
    if (_bss_1E & 2) {
        sp50 >>= 1;
    }
    if (_bss_1E & 4) {
        sp4C >>= 1;
    }
    if (_bss_1E & 8) {
        sp50 >>= 1;
    }
    if (sp4C >= 0x80U) {
        sp4C = 0x7F;
    }
    if (sp50 >= 0x80U) {
        sp50 = 0x7F;
    }
    if ((sp50 == 0) && (_bss_18 != 0)) {
        dll_6_func_A1C(_bss_18);
        _bss_18 = 0;
    } else if (sp50 != 0) {
        if (_bss_18 == 0) {
            _bss_1F = 1;
            dll_6_play_sound(NULL, SOUND_986, _bss_1F, &_bss_18, "game/amsfx.c", 0x3F8, "");
        }
        if (sp50 < (u8) _bss_1F) {
            _bss_1F -= 1;
        } else {
            _bss_1F += 1;
        }
        dll_6_func_860(_bss_18, _bss_1F);
    }
    if ((sp4C == 0) && (_bss_14 != 0)) {
        dll_6_func_A1C(_bss_14);
        _bss_14 = 0;
        return;
    }
    if (sp4C != 0) {
        if (_bss_14 == 0) {
            *_bss_20 = 1;
            dll_6_play_sound(NULL, SOUND_987, *_bss_20, &_bss_14, "game/amsfx.c", 0x40C, "\0\0\0\0AMSFX: Maximum sfx handles allocated,%d (from %s:%d)\n\0\0\0AMSFX: allocSfxHandle handle out of range, %d\n\0\0AMSFX: freeSfxHandle invalid handle, %d\n\0\0\0\0AMSFX: freeSfxHandle not used, %d\n\0\0AMSFX: Warning min/max range identical\n");
        }
        if (sp4C < (u8) *_bss_20) {
            *_bss_20 -= 1;
        } else {
            *_bss_20 += 1;
        }
        dll_6_func_860(_bss_14, *_bss_20);
    }
}

// offset: 0x1C10 | func: 20 | export: 19
void water_falls_set_flags(u8 arg0) {
    _bss_1E = arg0;
}

// offset: 0x1C38 | func: 21
static s32 dll_6_func_1C38(void) {
    Object* temp_v0;
    s32 var_a1;
    u8* var_v1;
    s32 sp28;

    temp_v0 = get_player();
    if (temp_v0 == NULL) {
        return TRUE;
    }
    var_v1 = func_80044A20(temp_v0->srt.transl.x, temp_v0->srt.transl.z, &sp28);
    if (var_v1 == NULL) {
        return TRUE;
    }
    var_a1 = 0;
    _bss_1D = 0;
    while (sp28 > var_a1 && _bss_1D < 0x10) {
        if (((s16*)var_v1)[0] == 0x377) {
            _bss_28[_bss_1D].pos.x = ((f32*)var_v1)[2];
            _bss_28[_bss_1D].pos.y = ((f32*)var_v1)[3];
            _bss_28[_bss_1D].pos.z = ((f32*)var_v1)[4];
            _bss_28[_bss_1D].unkC = ((u8*)var_v1)[0x21] * 0x10;
            _bss_28[_bss_1D].unkE = ((u8*)var_v1)[0x22] * 0x10;
            _bss_1D++;
        }
        var_a1 += ((u8*)var_v1)[2] << 2;
        var_v1 += ((u8*)var_v1)[2] << 2;
    }
    return FALSE;
}

// offset: 0x1D58 | func: 22
static s32 dll_6_func_1D58(s32 arg0, char *arg1, s32 arg2) {
    UnkBss4* var_v0;
    s32 temp_a0;
    s32 i;

    if (arg0 == 0) {
        for (i = 1; i < (_bss_8 + 1); i++) {
            if (!(_bss_4[i].unk12 & 0x40)) {
                break;
            }
        }
        if (i == (_bss_8 + 1)) {
            _bss_8++;
            if (_bss_8 == 0x40) {
                _bss_8--;
                i = 1;
            }
        }
        arg0 = i;
    } else if ((u32) _bss_8 < (u32) arg0) {
        arg0 = 0;
    }
    bzero(&_bss_4[arg0], 0x20);
    _bss_4[arg0].unk12 |= 0x40;
    return arg0;
}

// offset: 0x1E64 | func: 23
static s32 dll_6_func_1E64(u32 arg0) {
    sndstate* temp_a0;

    if (arg0 == 0) {
        return 0;
    }
    if (!(_bss_4[arg0].unk12 & 0x40)) {
        return 0;
    }

    temp_a0 = _bss_4[arg0].unk1C;
    if ((temp_a0 != NULL) && (temp_a0 != (sndstate* )-1)) {
        if (temp_a0 == _bss_4->unk1C) {
            _bss_4->unk12 = 0;
            _bss_4->unk1C = 0;
        }
        if (temp_a0 == (sndstate* )-2) {
            func_8006758C(temp_a0);
        } else {
            audioStop(temp_a0);
        }
    }
    _bss_4[arg0].unk1C = NULL;
    _bss_4[arg0].unk18 = NULL;
    _bss_4[arg0].unk12 = 0;
    return 0;
}

#ifndef NON_MATCHING
// Matches but requires dll_6_func_2438 as static
// offset: 0x1F78 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_1F78.s")
#else
static void dll_6_func_1F78(void) {
    Object* temp_a0;
    s32 i;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    s32 pad;
    s8 sp5F;
    u8 sp5E;
    u8 sp5D;
    u16 sp5A;
    sndstate* temp_s0;

    for (i = 1; i < (_bss_8 + 1); i++) {
        temp_a0 = _bss_4[i].unk18;
        if (temp_a0 == NULL) { continue; }

        temp_s0 = _bss_4[i].unk1C;
        if (temp_s0 == NULL) { continue; }
        if (temp_s0 == (sndstate* )-1) { continue; } 
        if (!(_bss_4[i].unk7 & 3)) { continue; }
        
        dll_6_func_2240(temp_a0, &sp6C, &sp68, &sp64, &sp5A);
        dll_6_func_22FC(sp6C, sp68, sp64, &_bss_4[i], &sp5F);
        dll_6_func_2438(sp6C, sp64, sp5A, &sp5E, &sp5D);
        _bss_4[i].unk17 = sp5F;
        if (sp5E != (s8) _bss_4[i].unk14) {
            _bss_4[i].unk14 = (u8) sp5E;
            if (temp_s0 == (sndstate* )-2) {
                func_800676A4(sp5E, 0);
            } else {
                audioPostEvent(temp_s0, 4, (s32) sp5E);
            }
        }
        sp5F = (s32) (_bss_4[i].unk16 * _bss_4[i].unk17) >> 7;
        if (sp5F < _bss_4[i].unk3) {
            sp5F = _bss_4[i].unk3;
        }
        if (sp5F != (s8)_bss_4[i].unk13) {
            _bss_4[i].unk13 = sp5F;
            if (temp_s0 == (sndstate* )-2) {
                func_80067650(sp5F << 8, 0);
            } else {
                audioPostEvent(temp_s0, 8, sp5F << 8);
            }
        }
        sp5D = sp5D > 0 ? 0 : -0x80;
        if (sp5D != _bss_4[i].unk15) {
            _bss_4[i].unk15 = sp5D;
            if (temp_s0 != (sndstate* )-2) {
                audioPostEvent(temp_s0, 0x100, sp5D);
            }
        }
    }
}
#endif

// offset: 0x2240 | func: 25
static void dll_6_func_2240(Object* obj, f32* xo, f32* yo, f32* zo, u16* yawOut) {
    Camera* sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;

    get_object_child_position(obj, &sp28, &sp24, &sp20);
    sp2C = get_camera_array();
    update_camera_for_object(sp2C);
    *xo = sp2C->tx - sp28;
    *yo = sp2C->ty - sp24;
    *zo = sp2C->tz - sp20;
    *yawOut = sp2C->srt.yaw;
}

// offset: 0x22FC | func: 26
static void dll_6_func_22FC(f32 arg0, f32 arg1, f32 arg2, UnkDE8* arg3, s8* arg4) {
    f32 temp_fa0;
    f32 temp_fv0;
    f32 a1;
    f32 var_ft4;
    f32 var_fv1;

    temp_fv0 = sqrtf(SQ(arg0) + SQ(arg1) + SQ(arg2));
    if (temp_fv0 < arg3->unkA) {
        *arg4 = 0x7F;
        return;
    }

    if (arg3->unkC < temp_fv0) {
        *arg4 = 0;
        return;
    }

    temp_fa0 = (arg3->unkC - arg3->unkA);
    arg1 = temp_fv0 - arg3->unkA;
    if (temp_fa0 != 0.0f) {
        arg1 /= temp_fa0;
        arg1 = 1.0f - arg1;
        *arg4 = (127.0f * arg1);
        return;
    }
    *arg4 = 0x7F;
}

#ifndef NON_MATCHING
// offset: 0x2438 | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/6_AMSFX/dll_6_func_2438.s")
#else
// https://decomp.me/scratch/TxBKM
void dll_6_func_2438(f32 arg0, f32 arg1, s32 arg2, u8* arg3, u8* arg4) {
    f32 sp34;
    s32 var_v1_2;
    s32 sp24;
    s8 sp2B;

    sp34 = sqrtf(SQ(arg0) + SQ(arg1));
    sp24 = (arg2 - -arctan2_f(arg0, arg1)) - 0x8000;
    if (sp24 >= 0x8001) { sp24 = sp24 + 0xFFFF0001; }
    if (sp24 < -0x8000) { sp24 += 0xFFFF; }
    if (sp34 <= 100.0f) {
        sp2B = -((sin16_precise(sp24) / 1024) * (sp34 * 0.01f));
        var_v1_2 = (s8)( -((cos16_precise(sp24) / 1024) * (sp34 * 0.01f)));
    } else {
        sp2B = -sin16(sp24) / 1024;
        var_v1_2 = (s8)( -cos16(sp24) / 1024);
    }
    *arg3 = sp2B + 0x40;
    *arg4 = var_v1_2 * -1;
}
#endif
