#include "PR/libaudio.h"
#include "PR/os.h"
#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dlls/engine/6_amsfx.h"
#include "functions.h"
#include "libnaudio/n_libaudio.h"
#include "libnaudio/n_sndp.h"
#include "libnaudio/n_sndplayer.h"
#include "mp3/mp3.h"
#include "macros.h"
#include "sys/acache.h"
#include "sys/asset_thread.h"
#include "sys/camera.h"
#include "sys/fs.h"
#include "sys/map.h"
#include "sys/mpeg.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "unktypes.h"
#include "sys/objects.h"
#include "prevent_bss_reordering.h"

/*0x0*/ static u32 _data_0[4] = { 0 };

#define IS_MP3 0x8000
#define PITCH_DEFAULT 100

typedef struct UnkBss4 {
    SoundDef def;
    s16 soundID;
    u16 pad10;
    u8 unk12;
    u8 unk13;
    s8 unk14;
    s8 unk15;
    s8 unk16;
    s8 unk17; //volume?
    Object *source; //object playing the sound
    sndstate *unk1C;
} UnkBss4;

typedef struct UnkBss28 {
    Vec3f pos;
    u16 unkC;
    u16 unkE;
} UnkBss28;

/*0x0*/ static ALBankFile *_bss_0; //SFX.bin buffer
/*0x4*/ static UnkBss4 *_bss_4; //active sounds?
/*0x8*/ static s32 _bss_8; //active sound count?
/*0xC*/ static u32 _bss_C;
/*0x10*/ static u32 _bss_10;
/*0x14*/ static u32 _bss_14;
/*0x18*/ static u32 _bss_18;
/*0x1C*/ static u8 _bss_1C; //mapID?
/*0x1D*/ static u8 _bss_1D;
/*0x1E*/ static u8 _bss_1E;
/*0x1F*/ static u8 _bss_1F; // sound volume
/*0x20*/ static u8 _bss_20[0x8]; // sound volume(s?)
/*0x28*/ static UnkBss28 _bss_28[0x10];
/*0x128*/ static ACache *_bss_128;

void dll_6_func_860(u32 arg0, u8 arg1);
void dll_6_func_A1C(u32 arg0);
s32 dll_6_func_DE8(u16 arg0, SoundDef* arg1);
static s32 dll_6_func_1C38(void);
static s32 dll_6_func_1D58(s32, char *, s32);
static s32 dll_6_func_1E64(u32);
static void dll_6_func_1F78(void);
static void dll_6_func_2240(Object* obj, f32* xo, f32* yo, f32* zo, u16* yawOut);
static void dll_6_func_22FC(f32 arg0, f32 arg1, f32 arg2, SoundDef* arg3, s8* outVolume);
static void dll_6_func_2438(f32 arg0, f32 arg1, s32 arg2, s8* outPan, s8* outFx);

// offset: 0x0 | ctor
void dll_6_ctor(s32 arg0) {
    u32* tab;
    s32 start;
    s32 length;
    ALSndpConfig sp30;

    _bss_14 = 0;
    _bss_18 = 0;
    _bss_1C = 0;
    _bss_1D = 0;
    tab = NULL;
    sp30.maxEvents = 0x96;
    sp30.maxStates = 0x20;
    sp30.maxSounds = 0x20;
    sp30.unk10 = 1;
    _bss_1E = 0;
    n_alSndpNew(&sp30);

    //SFX.bin
    queue_alloc_load_file((void*)&tab, SFX_TAB);
    start = tab[0];
    length = tab[1] - start;
    if (length != 0) {
        _bss_0 = mmAlloc(length, ALLOC_TAG_SFX_COL, NULL);
        queue_load_file_region_to_ptr((void*)_bss_0, SFX_BIN, start, length);
        alBnkfNew(_bss_0, (u8*)file_get_romaddr(SFX_BIN, tab[1]));
    } else {
        STUBBED_PRINTF("AMSFX: Error sound effects bank missing!\n");
    }
    mmFree(tab);

    //AUDIO.bin
    _bss_128 = acache_init(AUDIO_BIN, 0x40U, 0xE, 0x40U, 0);
    queue_alloc_load_file((void*)&tab, AUDIO_TAB);
    _bss_C = tab[0];
    _bss_10 = tab[1] - _bss_C;
    _bss_10 = _bss_10 / 14;
    mmFree(tab);

    _bss_8 = 65;
    length = _bss_8 << 5;
    _bss_4 = mmAlloc(length, ALLOC_TAG_SFX_COL, NULL);
    bzero(_bss_4, length);
    _bss_8 = 0;
}

/*0x2C*/ static const char str_2C[] = "amSfxPlayEx failed\n";

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
                if (mp3_is_busy() == 0) {
                    _bss_4[i].unk1C = (sndstate*)-1;
                }
            } else if (sndGetState(temp_a0) == 0) {
                _bss_4[i].unk1C = (sndstate*)-1;
            }
        }
    }
}

// offset: 0x480 | func: 1 | export: 1
void dll_6_func_480(Object *obj) { }

// offset: 0x48C | func: 2 | export: 2
u32 dll_6_play_sound(Object* obj, u16 soundID, u8 volume, u32* soundHandle, char *arg4, s32 arg5, char *arg6) {
    SoundDef soundDef;
    f32 x;
    f32 y;
    f32 z;
    u32 activeSoundIndex;
    u16 yaw;
    s8 volumeCalc;

    _bss_4->unk12 = 0;
    _bss_4->unk1C = NULL;
    
    //Bail if soundID is 0
    if (!soundID) {
        return 0;
    }

    //Get sound definition from AUDIO.bin subfile 0
    dll_6_func_DE8(soundID, &soundDef);

    //Bail if sound's clipID is 0
    if (!(soundDef.bankAndClipID & 0x7FFF)) {
        return 0;
    }

    if (soundHandle != NULL) {
        activeSoundIndex = soundHandle[0];
    } else {
        activeSoundIndex = 0;
    }

    activeSoundIndex = dll_6_func_1D58(activeSoundIndex, arg4, arg5);
    _bss_4[activeSoundIndex].soundID = soundID;
    _bss_4[activeSoundIndex].source = obj;
    _bss_4[activeSoundIndex].unk17 = MAX_VOLUME;

    if ((obj != NULL) && (soundDef.volumeFalloff & 3)) {
        dll_6_func_2240(obj, &x, &y, &z, &yaw);
        dll_6_func_22FC(x, y, z, &soundDef, &volumeCalc);
        _bss_4[activeSoundIndex].unk17 = volumeCalc;
    }

    _bss_4[activeSoundIndex].unk16 = (volume * soundDef.volume) >> 7;
    volumeCalc = (_bss_4[activeSoundIndex].unk16 * _bss_4[activeSoundIndex].unk17) >> 7;
    _bss_4[activeSoundIndex].unk13 = volumeCalc;

    if (soundDef.bankAndClipID & IS_MP3) {
        _bss_4[activeSoundIndex].unk1C = (sndstate* )-2;
        // @fake
        if (_bss_4[activeSoundIndex].def.bankAndClipID) {}
        mpeg_play((soundDef.bankAndClipID & 0x7FFF) - 1);
        mp3_set_volume(volumeCalc << 8, 0);
        // @fake
        if (_bss_4) {}
    } else {
        some_sound_func(_bss_0->bankArray[0], soundDef.bankAndClipID, (volumeCalc << 8), PAN_CENTRE, soundDef.pitch / 100.0f, (s32)(f32)soundDef.unk6, 1U, &_bss_4[activeSoundIndex].unk1C);
    }

    bcopy(&soundDef, &_bss_4[activeSoundIndex], sizeof(SoundDef));
    bcopy(&_bss_4[activeSoundIndex], _bss_4, sizeof(UnkBss4));

    if (soundHandle != NULL) {
        soundHandle[0] = activeSoundIndex;
    } else {
        _bss_4[activeSoundIndex].unk12 |= 0x80;
    }
    return activeSoundIndex;
}

// offset: 0x7E4 | func: 3 | export: 3
void dll_6_func_7E4(u8 arg0) {
    s32 i;

    for (i = 0; i < 1; i++) {
        func_80066978(i, arg0 << 8);
    }
}

// offset: 0x860 | func: 4 | export: 4
//amSfxSetVol?
void dll_6_func_860(u32 soundHandle, u8 volume) {
    sndstate* temp_a0;
    u8 vol;

    if (1) {
        if (_bss_8 < soundHandle) {
            STUBBED_PRINTF("amSfxSetVol: Warning,sound handle '%d' out of range.\n");
            return;
        }
        
        temp_a0 = _bss_4[soundHandle].unk1C;
        if (temp_a0 == NULL) {
            STUBBED_PRINTF("amSfxSetVol: Warning,invalid handle '%d'.\n");
            return;
        }
        
        _bss_4[soundHandle].unk16 = (_bss_4[soundHandle].def.volume * volume) >> 7;
    }

    vol = (_bss_4[soundHandle].unk16 * _bss_4[soundHandle].unk17) >> 7;
    if (vol != (s8) _bss_4[soundHandle].unk13) {
        _bss_4[soundHandle].unk13 = vol;
        if (temp_a0 == (sndstate* )-2) {
            mp3_set_volume(vol << 8, soundHandle * 0);
        } else if (temp_a0 != (sndstate* )-1) {
            audioPostEvent(temp_a0, AL_SNDP_VOL_EVT, (void *)(vol << 8));
        }
    }
}

// offset: 0x954 | func: 5 | export: 5
//amSfxSetPitch?
void dll_6_func_954(u32 soundHandle, f32 pitch) {
    sndstate *temp_a0;

    if ((u32) _bss_8 < soundHandle) {
        STUBBED_PRINTF("amSfxSetPitch: Warning,sound handle '%d' out of range.\n");
        return;
    }

    temp_a0 = _bss_4[soundHandle].unk1C;
    if (temp_a0 == NULL) {
        STUBBED_PRINTF("amSfxSetPitch: Warning,invalid handle '%d'.\n");
        return;
    }

    pitch *= _bss_4[soundHandle].def.pitch / 100.0f;
    if ((temp_a0 != (sndstate *)-2) && (temp_a0 != (sndstate *)-1)) {
        audioPostEvent(temp_a0, AL_SNDP_PITCH_EVT, ((void **)&pitch)[0]);
    }
}

// offset: 0xA1C | func: 6 | export: 6
//amSfxStop?
void dll_6_func_A1C(u32 arg0) {
    if ((u32) _bss_8 >= arg0) {
        dll_6_func_1E64(arg0);
    } else {
        STUBBED_PRINTF("amSfxStop: Warning,sound handle '%d' out of range.\n");
    }
}

// offset: 0xA6C | func: 7 | export: 7
void dll_6_func_A6C(Object *obj) {
    s32 i;

    if (obj == 0) {
        return;
    }

    // @fake
    if (1) {}
    for (i = 1; i < (_bss_8+1); i++) {
        if ((_bss_4[i].unk12 & 0x40) != 0 && obj == _bss_4[i].source && _bss_4[i].unk1C != NULL) {
            dll_6_func_1E64(i);
        }
    }
}

// offset: 0xB48 | func: 8 | export: 8
//amSfxIsPlaying?
s32 dll_6_func_B48(u32 soundHandle) {
    if (soundHandle == 0) {
        return 0;
    }

    if ((u32) _bss_8 < soundHandle) {
        STUBBED_PRINTF("amSfxIsPlaying: Warning,sound handle '%d' out of range.\n");
        return 0;
    }

    if (_bss_4[soundHandle].unk1C == NULL || _bss_4[soundHandle].unk1C == (sndstate* )-1) {
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
        temp_s0 = _bss_4[i].source;
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
        arg3[outCount++] = _bss_4[i].source;
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
//amSfxGetDefault?
s32 dll_6_func_DE8(u16 soundID, SoundDef* soundDef) {
    void* temp_v0;

    if (soundID <= 0 || (s32)_bss_10 < soundID) {
        STUBBED_PRINTF("amSfxGetDefault: Warning,sound effect value '%d' out of range.\n");
        bzero(soundDef, sizeof(SoundDef));
        return 0;
    }

    soundID--;
    temp_v0 = acache_get(_bss_128, soundID);
    if (temp_v0 != NULL) {
        bcopy(temp_v0, soundDef, sizeof(SoundDef));
    } else {
        return 0;
    }

    if (soundDef->volume == 0) {
        soundDef->volume = HALF_VOLUME;
    }
    if (soundDef->pitch == 0) {
        soundDef->pitch = PITCH_DEFAULT;
    }
    if (soundDef->pan == 0) {
        soundDef->pan = PAN_CENTRE;
    }

    return 1;
}

/*0x1BC*/ static const char str_1BC[] = "amSndPlayEx: Warning,sound '%d' out of range.\n";
/*0x1EC*/ static const char str_1EC[] = "amSndPlayEx: Warning,sound handle '%d' out of range.\n";

// offset: 0xEE4 | func: 12 | export: 12
s32 dll_6_func_EE4(s32 arg0, UNK_TYPE_32 arg1) {
    acache_func_80000D50(_bss_128);
    return 1;
}

// offset: 0xF2C | func: 13 | export: 13
void dll_6_func_F2C(Object* arg0, u16 soundID, u32* arg2, char *arg3, s32 arg4) {
    u32 temp_v0_2;

    if (!soundID) {
        return;
    }

    if (_bss_0->bankArray[0]->instArray[0]->soundCount < soundID) {
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
    some_sound_func(_bss_0->bankArray[0], soundID, 0x4000U, 0x40U, 1.0f, 0U, 1U, &_bss_4[temp_v0_2].unk1C);
    _bss_4[temp_v0_2].soundID = -1 - soundID;
    _bss_4[temp_v0_2].source = arg0;
    bcopy(&_bss_4[temp_v0_2], _bss_4, sizeof(UnkBss4));
    if (arg2 != NULL) {
        *arg2 = temp_v0_2;
        return;
    }
    _bss_4[temp_v0_2].unk12 |= 0x80;
}

void dll_6_func_10D0(Object* obj, u16 soundID, u32* soundHandle) {
    if (soundHandle != NULL) {
        dll_6_play_sound(obj, soundID, MAX_VOLUME, soundHandle, "game/amsfx.c", 0x2BD, "");
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
        if (((0, obj)) != _bss_4[i].source) { continue; }
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


// offset: 0x1320 | func: 17 | export: 20
void dll_6_func_1320(Object* arg0, u16 arg1, f32 arg2, f32 arg3, f32 arg4, u32* arg5) {
    f32 pad;
    f32 temp2;
    f32 temp;
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;
    s32 var_v1;
    Camera* sp30;

    sp30 = get_camera_array();
    update_camera_for_object(sp30);
    xDiff = sp30->tx;
    yDiff = sp30->ty;
    zDiff = sp30->tz;
    pad  = arg0->positionMirror.y - yDiff;
    temp = arg0->positionMirror.y - yDiff;
    temp = sqrtf(SQ(arg0->positionMirror.x - xDiff) + SQ(temp) + SQ(arg0->positionMirror.z - zDiff));
    if (arg4 < temp) {
        var_v1 = 0;
    } else if (temp <= arg3) {
        var_v1 = 0x7F;
    } else {
        temp2 = (temp - arg3);
        temp = arg4 - arg3;
        temp2 = temp - (temp2);
        temp2 = ((temp2 * MAX_VOLUME_F) / temp);
        var_v1 = temp2;
        if (var_v1 >= 0x80) {
            var_v1 = 0x7F;
        }
        if (var_v1 < 0) {
            var_v1 = 0;
        }
    }
    if ((var_v1 > 0) && (*arg5 == 0)) {
        dll_6_play_sound(arg0, arg1, var_v1, arg5, "game/amsfx.c", 0x37B, "\0\0\0\0amSfxSetPitch: Warning,sound handle '%d' out of range.\n\0amSfxSetPitch: Warning,invalid handle '%d'.\n");
        dll_6_func_954(*arg5, arg2);
    }
    if (*arg5 != 0) {
        if (var_v1 == 0) {
            dll_6_func_A1C(*arg5);
            *arg5 = 0;
            return;
        }
        dll_6_func_860(*arg5, var_v1);
    }
}


void dll_6_func_1504(s32 arg0, Object* arg1, Object* arg2, f32 arg3) {
    s32 pad2;
    f32 pitch;
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
        pitch = 0.1f;
    } else {
        pitch = temp_fv1;
        if (temp_fv1 > 1.9f) {
            pitch = 1.9f;
        }
    }
    if (_bss_8 >= arg0) {
        temp_a0 = _bss_4[arg0].unk1C;
        if (temp_a0 != NULL) {
            pitch *= _bss_4[arg0].def.pitch / 100.0f;
            if ((temp_a0 != (sndstate* )-2) && (temp_a0 != (sndstate* )-1)) {
                audioPostEvent(temp_a0, AL_SNDP_PITCH_EVT, ((void **)&pitch)[0]);
            }
        }
    }
}

// offset: 0x16E0 | func: 19 | export: 18
void dll_6_func_16E0(void) {
    Object* player;
    s32 i;
    f32 distance;
    u32 volume2;
    u32 volume1;
    u8 mapID;
    Vec3f* camera;

    player = get_player();
    mapID = map_get_map_id_from_xz_ws(player->srt.transl.x, player->srt.transl.z);
    if ((mapID != _bss_1C) || (_bss_1E & 0x10)) {
        dll_6_func_1C38();
        _bss_1C = mapID;
        _bss_1E &= ~0x10;
    }
    if (_bss_1D == 0) {
        return;
    }

    camera = (Vec3f*)get_camera();
    volume1 = 0;
    volume2 = 0;
    for (i = 0; i < _bss_1D; i++) {
        distance = vec3_distance(camera + 1, (Vec3f* ) &_bss_28[i]);
        if (distance < _bss_28[i].unkC) {
            volume2 += MAX_VOLUME - (u8)((u32)((distance / _bss_28[i].unkC) * MAX_VOLUME_F));
        }
        if (distance < _bss_28[i].unkE) {
            volume1 += MAX_VOLUME - (u8)((u32)((distance / _bss_28[i].unkE) * MAX_VOLUME_F));
        }
    }
    if (_bss_1E & 1) {
        volume1 >>= 1;
    }
    if (_bss_1E & 2) {
        volume2 >>= 1;
    }
    if (_bss_1E & 4) {
        volume1 >>= 1;
    }
    if (_bss_1E & 8) {
        volume2 >>= 1;
    }
    if (volume1 > MAX_VOLUME) {
        volume1 = MAX_VOLUME;
    }
    if (volume2 > MAX_VOLUME) {
        volume2 = MAX_VOLUME;
    }
    if ((volume2 == 0) && (_bss_18 != 0)) {
        dll_6_func_A1C(_bss_18);
        _bss_18 = 0;
    } else if (volume2 != 0) {
        if (_bss_18 == 0) {
            _bss_1F = 1;
            dll_6_play_sound(NULL, SOUND_986_Waterfall_Low_Loop, _bss_1F, &_bss_18, "game/amsfx.c", 0x3F8, "");
        }
        if (volume2 < (u8) _bss_1F) {
            _bss_1F -= 1;
        } else {
            _bss_1F += 1;
        }
        dll_6_func_860(_bss_18, _bss_1F);
    }
    if ((volume1 == 0) && (_bss_14 != 0)) {
        dll_6_func_A1C(_bss_14);
        _bss_14 = 0;
        return;
    }
    if (volume1 != 0) {
        if (_bss_14 == 0) {
            *_bss_20 = 1;
            dll_6_play_sound(NULL, SOUND_987_Waterfall_High_Loop, *_bss_20, &_bss_14, "game/amsfx.c", 0x40C, "\0\0\0\0AMSFX: Maximum sfx handles allocated,%d (from %s:%d)\n\0\0\0AMSFX: allocSfxHandle handle out of range, %d\n\0\0AMSFX: freeSfxHandle invalid handle, %d\n\0\0\0\0AMSFX: freeSfxHandle not used, %d\n\0\0AMSFX: Warning min/max range identical\n");
        }
        if (volume1 < (u8) *_bss_20) {
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
    Object* player;
    s32 var_a1;
    u8* var_v1;
    s32 sp28;

    player = get_player();
    if (player == NULL) {
        return TRUE;
    }
    var_v1 = func_80044A20(player->srt.transl.x, player->srt.transl.z, &sp28);
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
            mp3_stop();
        } else {
            audioStop(temp_a0);
        }
    }
    _bss_4[arg0].unk1C = NULL;
    _bss_4[arg0].source = NULL;
    _bss_4[arg0].unk12 = 0;
    return 0;
}

// offset: 0x1F78 | func: 24
static void dll_6_func_1F78(void) {
    Object* temp_a0;
    s32 i;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    s32 pad;
    s8 vol;
    s8 pan;
    s8 fx;
    u16 sp5A;
    sndstate* temp_s0;

    for (i = 1; i < (_bss_8 + 1); i++) {
        temp_a0 = _bss_4[i].source;
        if (temp_a0 == NULL) { continue; }

        temp_s0 = _bss_4[i].unk1C;
        if (temp_s0 == NULL) { continue; }
        if (temp_s0 == (sndstate* )-1) { continue; } 
        if (!(_bss_4[i].def.volumeFalloff & 3)) { continue; }
        
        dll_6_func_2240(temp_a0, &sp6C, &sp68, &sp64, &sp5A);
        dll_6_func_22FC(sp6C, sp68, sp64, &_bss_4[i].def, &vol);
        dll_6_func_2438(sp6C, sp64, sp5A, &pan, &fx);
        _bss_4[i].unk17 = vol;
        if (pan != (s8) _bss_4[i].unk14) {
            _bss_4[i].unk14 = (u8) pan;
            if (temp_s0 == (sndstate* )-2) {
                mp3_set_pan(pan, FALSE);
            } else {
                audioPostEvent(temp_s0, AL_SNDP_PAN_EVT, (void *)pan);
            }
        }
        vol = (s32) (_bss_4[i].unk16 * _bss_4[i].unk17) >> 7;
        if (vol < _bss_4[i].def.unk3) {
            vol = _bss_4[i].def.unk3;
        }
        if (vol != (s8)_bss_4[i].unk13) {
            _bss_4[i].unk13 = vol;
            if (temp_s0 == (sndstate* )-2) {
                mp3_set_volume(vol << 8, 0);
            } else {
                audioPostEvent(temp_s0, AL_SNDP_VOL_EVT, (void*)(vol << 8));
            }
        }
        fx = fx > 0 ? 0 : -0x80;
        if (fx != _bss_4[i].unk15) {
            _bss_4[i].unk15 = fx;
            if (temp_s0 != (sndstate* )-2) {
                audioPostEvent(temp_s0, AL_SNDP_FX_EVT, (void*)(fx));
            }
        }
    }
}

// offset: 0x2240 | func: 25
static void dll_6_func_2240(Object* obj, f32* xo, f32* yo, f32* zo, u16* yawOut) {
    Camera* camera;
    f32 sp28;
    f32 sp24;
    f32 sp20;

    get_object_child_position(obj, &sp28, &sp24, &sp20);
    camera = get_camera_array();
    update_camera_for_object(camera);
    *xo = camera->tx - sp28;
    *yo = camera->ty - sp24;
    *zo = camera->tz - sp20;
    *yawOut = camera->srt.yaw;
}

// offset: 0x22FC | func: 26
//apply_volume_falloff?
static void dll_6_func_22FC(f32 x, f32 y, f32 z, SoundDef* soundDef, s8* outVolume) {
    f32 range;
    f32 distance;

    distance = sqrtf(SQ(x) + SQ(y) + SQ(z));

    //Use max volume if inside near radius
    if (distance < soundDef->falloffNear) {
        *outVolume = MAX_VOLUME;
        return;
    }

    //Use 0 volume if outside far radius
    if (soundDef->falloffFar < distance) {
        *outVolume = 0;
        return;
    }

    //Linearly interpolate volume when between radii
    range = soundDef->falloffFar - soundDef->falloffNear;
    y = distance - soundDef->falloffNear;
    if (range != 0.0f) {
        y /= range;
        y = 1.0f - y;
        *outVolume = (MAX_VOLUME_F * y);
        return;
    }
    *outVolume = MAX_VOLUME;
}

// offset: 0x2438 | func: 27
static void dll_6_func_2438(f32 x, f32 z, s32 arg2, s8* outPan, s8* outFx) {
    f32 magnitude;
    s32 theta;
    s32 pad;
    s8 panOffset;
    s8 sp2A;

    magnitude = sqrtf(SQ(x) + SQ(z));
    theta = -arctan2_f(x, z);
    theta = (arg2 - theta) - M_180_DEGREES;
    CIRCLE_WRAP(theta)
    if (magnitude <= 100.0f) {
        panOffset = -((sin16_precise(theta) / 1024) * (magnitude * 0.01f));
        sp2A = (s8) -((cos16_precise(theta) / 1024) * (magnitude * 0.01f));
    } else {
        panOffset = -sin16(theta) / 1024;
        sp2A = (s8)  (-cos16(theta) / 1024) ;
    }
    sp2A = -sp2A;
    *outPan = PAN_CENTRE + panOffset;
    *outFx = sp2A;
}
