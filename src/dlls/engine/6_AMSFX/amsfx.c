#include "PR/libaudio.h"
#include "PR/os.h"
#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/345_WaterFallSpray.h"
#include "game/objects/object_id.h"
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

#define MAX_SOUND_SLOTS 64

enum SoundSlotFlags {
    SOUNDSLOT_IN_USE = 0x40,
    SOUNDSLOT_PLAYING = 0x80
};

typedef struct SoundSlot {
    SoundDef def;
    s16 soundID;
    u16 pad10;
    u8 flags; // SoundSlotFlags
    u8 unk13;
    s8 unk14;
    s8 unk15;
    s8 volume; // effective volume
    s8 distVolume; // volume with only distance-based falloff considered
    Object *source; // object playing the sound
    sndstate *sndpHandle;
} SoundSlot;

typedef struct WaterFallSpray {
    Vec3f pos;
    u16 unkC;
    u16 unkE;
} WaterFallSpray;

#define MAX_WATER_FALL_SPRAY 16

enum AMSFXWaterFallsFlags {
    // The follow flags cut the volume of the high or low waterfall sfx in half for each flag set.
    AMSFX_WATERFALLS_LOWER_HIGH = 0x1,
    AMSFX_WATERFALLS_LOWER_LOW = 0x2,
    AMSFX_WATERFALLS_LOWER_HIGH2 = 0x4,
    AMSFX_WATERFALLS_LOWER_LOW2 = 0x8,
    // Clear the list of waterfall sprays and re-search the map for an updated list.
    AMSFX_WATERFALLS_REFRESH = 0x10
};

/*0x0*/ static ALBankFile *_bss_0; //SFX.bin buffer
/*0x4*/ static SoundSlot *sSndSlots; //active sounds
/*0x8*/ static s32 sSndSlotsLen; //active sound count?
/*0xC*/ static u32 _bss_C;
/*0x10*/ static u32 _bss_10;
/*0x14*/ static u32 sWaterfallHighHandle;
/*0x18*/ static u32 sWaterfallLowHandle;
/*0x1C*/ static u8 sWaterfallsLastMap; //mapID?
/*0x1D*/ static u8 sWaterFallSprayCount;
/*0x1E*/ static u8 sWaterfallFlags;
/*0x1F*/ static u8 sWaterfallLowVolume; // sound volume
/*0x20*/ static u8 sWaterfallHighVolume; // sound volume
/*0x28*/ static WaterFallSpray sWaterFallSprays[MAX_WATER_FALL_SPRAY];
/*0x128*/ static ACache *sSoundDefCache;

void amsfx_set_vol(u32 soundHandle, u8 volume);
void amsfx_stop(u32 arg0);
s32 amsfx_get_default(u16 arg0, SoundDef* arg1);
static s32 amsfx_water_falls_find_sprays(void);
static s32 amsfx_make_handle(s32 handle, char *filename, s32 lineNo);
static s32 amsfx_free_handle(u32);
static void amsfx_func_1F78(void);
static void amsfx_func_2240(Object* obj, f32* xo, f32* yo, f32* zo, u16* yawOut);
static void amsfx_func_22FC(f32 arg0, f32 arg1, f32 arg2, SoundDef* arg3, s8* outVolume);
static void amsfx_func_2438(f32 arg0, f32 arg1, s32 arg2, s8* outPan, s8* outFx);

// offset: 0x0 | ctor
void amsfx_ctor(void *dll) {
    u32* tab;
    s32 start;
    s32 length;
    ALSndpConfig sp30;

    sWaterfallHighHandle = 0;
    sWaterfallLowHandle = 0;
    sWaterfallsLastMap = 0;
    sWaterFallSprayCount = 0;
    tab = NULL;
    sp30.maxEvents = 0x96;
    sp30.maxStates = 0x20;
    sp30.maxSounds = 0x20;
    sp30.unk10 = 1;
    sWaterfallFlags = 0;
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
    sSoundDefCache = acache_init(AUDIO_BIN, 0x40, sizeof(SoundDef), 0x40, 0);
    queue_alloc_load_file((void*)&tab, AUDIO_TAB);
    _bss_C = tab[0];
    _bss_10 = tab[1] - _bss_C;
    _bss_10 = _bss_10 / sizeof(SoundDef);
    mmFree(tab);

    sSndSlotsLen = MAX_SOUND_SLOTS + 1;
    length = sSndSlotsLen << 5;
    sSndSlots = mmAlloc(length, ALLOC_TAG_SFX_COL, NULL);
    bzero(sSndSlots, length);
    sSndSlotsLen = 0;
}

// offset: 0x22C | dtor
void amsfx_dtor(void *dll) {
    s32 i;
    s32 var_v0;

    i = 1;
    sWaterfallHighHandle = 0;
    sWaterfallLowHandle = 0;
    sWaterfallsLastMap = 0;
    sWaterFallSprayCount = 0;
    sWaterfallFlags = 0;
    for (i = 1; i < (sSndSlotsLen + 1); i++) {
        if (sSndSlots[i].sndpHandle) {
            amsfx_free_handle(i);
        }
    }
    mmFree(sSndSlots);
    sSndSlotsLen = 0;
    mmFree(_bss_0);
}

// offset: 0x338 | func: 0 | export: 0
void amsfx_func_338(void) {
    sndstate *temp_a0;
    s32 i;

    amsfx_func_1F78();
    for (i = 1; i < (sSndSlotsLen + 1); i++) {
        temp_a0 = sSndSlots[i].sndpHandle;
        // @fake
        if (1) {}
        if (temp_a0 != 0) {
            if (temp_a0 == (sndstate*)-1) {
                if (sSndSlots[i].flags & SOUNDSLOT_PLAYING) {
                    amsfx_free_handle(i);
                }
            } else if (temp_a0 == (sndstate*)-2) {
                if (mp3_is_busy() == 0) {
                    sSndSlots[i].sndpHandle = (sndstate*)-1;
                }
            } else if (sndGetState(temp_a0) == 0) {
                sSndSlots[i].sndpHandle = (sndstate*)-1;
            }
        }
    }
}

// offset: 0x480 | func: 1 | export: 1
void amsfx_func_480(Object *obj) { }

/*0x2C*/ static const char str_2C[] = "amSfxPlayEx failed\n";

// offset: 0x48C | func: 2 | export: 2
u32 amsfx_play(Object* obj, u16 soundID, u8 volume, u32* soundHandle, char *filename, s32 lineNo, char *code) {
    SoundDef soundDef;
    f32 x;
    f32 y;
    f32 z;
    u32 handle;
    u16 yaw;
    s8 volumeCalc;

    sSndSlots->flags = 0;
    sSndSlots->sndpHandle = NULL;
    
    //Bail if soundID is 0
    if (!soundID) {
        return 0;
    }

    //Get sound definition from AUDIO.bin subfile 0
    amsfx_get_default(soundID, &soundDef);

    //Bail if sound's clipID is 0
    if (!(soundDef.bankAndClipID & 0x7FFF)) {
        return 0;
    }

    if (soundHandle != NULL) {
        handle = *soundHandle;
    } else {
        handle = 0;
    }

    handle = amsfx_make_handle(handle, filename, lineNo);
    sSndSlots[handle].soundID = soundID;
    sSndSlots[handle].source = obj;
    sSndSlots[handle].distVolume = MAX_VOLUME;

    if ((obj != NULL) && (soundDef.volumeFalloff & 3)) {
        amsfx_func_2240(obj, &x, &y, &z, &yaw);
        amsfx_func_22FC(x, y, z, &soundDef, &volumeCalc);
        sSndSlots[handle].distVolume = volumeCalc;
    }

    sSndSlots[handle].volume = (volume * soundDef.volume) >> 7;
    volumeCalc = (sSndSlots[handle].volume * sSndSlots[handle].distVolume) >> 7;
    sSndSlots[handle].unk13 = volumeCalc;

    if (soundDef.bankAndClipID & IS_MP3) {
        sSndSlots[handle].sndpHandle = (sndstate* )-2;
        // @fake
        if (sSndSlots[handle].def.bankAndClipID) {}
        mpeg_play((soundDef.bankAndClipID & 0x7FFF) - 1);
        mp3_set_volume(volumeCalc << 8, 0);
        // @fake
        if (sSndSlots) {}
    } else {
        some_sound_func(
            _bss_0->bankArray[0], 
            soundDef.bankAndClipID, 
            (volumeCalc << 8), 
            PAN_CENTRE, 
            soundDef.pitch / 100.0f, 
            (s32)(f32)soundDef.unk6, 
            1, 
            &sSndSlots[handle].sndpHandle);
    }

    bcopy(&soundDef, &sSndSlots[handle], sizeof(SoundDef));
    bcopy(&sSndSlots[handle], sSndSlots, sizeof(SoundSlot));

    if (soundHandle != NULL) {
        *soundHandle = handle;
    } else {
        sSndSlots[handle].flags |= SOUNDSLOT_PLAYING;
    }
    return handle;
}

// offset: 0x7E4 | func: 3 | export: 3
void amsfx_func_7E4(u8 volume) {
    s32 i;

    for (i = 0; i < 1; i++) {
        func_80066978(i, volume << 8);
    }
}

// offset: 0x860 | func: 4 | export: 4
void amsfx_set_vol(u32 soundHandle, u8 volume) {
    sndstate* temp_a0;
    u8 vol;

    if (1) {
        if ((u32)sSndSlotsLen < soundHandle) {
            STUBBED_PRINTF("amSfxSetVol: Warning,sound handle '%d' out of range.\n", soundHandle);
            return;
        }
        
        temp_a0 = sSndSlots[soundHandle].sndpHandle;
        if (temp_a0 == NULL) {
            STUBBED_PRINTF("amSfxSetVol: Warning,invalid handle '%d'.\n", soundHandle);
            return;
        }
        
        sSndSlots[soundHandle].volume = (sSndSlots[soundHandle].def.volume * volume) >> 7;
    }

    vol = (sSndSlots[soundHandle].volume * sSndSlots[soundHandle].distVolume) >> 7;
    if (vol != (s8) sSndSlots[soundHandle].unk13) {
        sSndSlots[soundHandle].unk13 = vol;
        if (temp_a0 == (sndstate* )-2) {
            mp3_set_volume(vol << 8, soundHandle * 0);
        } else if (temp_a0 != (sndstate* )-1) {
            audioPostEvent(temp_a0, AL_SNDP_VOL_EVT, (void *)(vol << 8));
        }
    }
}

// offset: 0x954 | func: 5 | export: 5
void amsfx_set_pitch(u32 soundHandle, f32 pitch) {
    sndstate *temp_a0;

    if ((u32) sSndSlotsLen < soundHandle) {
        STUBBED_PRINTF("amSfxSetPitch: Warning,sound handle '%d' out of range.\n", soundHandle);
        return;
    }

    temp_a0 = sSndSlots[soundHandle].sndpHandle;
    if (temp_a0 == NULL) {
        STUBBED_PRINTF("amSfxSetPitch: Warning,invalid handle '%d'.\n", soundHandle);
        return;
    }

    pitch *= sSndSlots[soundHandle].def.pitch / 100.0f;
    if ((temp_a0 != (sndstate *)-2) && (temp_a0 != (sndstate *)-1)) {
        audioPostEvent(temp_a0, AL_SNDP_PITCH_EVT, ((void **)&pitch)[0]);
    }
}

// offset: 0xA1C | func: 6 | export: 6
void amsfx_stop(u32 soundHandle) {
    if ((u32) sSndSlotsLen >= soundHandle) {
        amsfx_free_handle(soundHandle);
    } else {
        STUBBED_PRINTF("amSfxStop: Warning,sound handle '%d' out of range.\n", soundHandle);
    }
}

// offset: 0xA6C | func: 7 | export: 7
void amsfx_stop_object(Object *obj) {
    s32 i;

    if (obj == NULL) {
        return;
    }

    // @fake
    if (1) {}
    for (i = 1; i < (sSndSlotsLen+1); i++) {
        if ((sSndSlots[i].flags & SOUNDSLOT_IN_USE) != 0 && obj == sSndSlots[i].source && sSndSlots[i].sndpHandle != NULL) {
            amsfx_free_handle(i);
        }
    }
}

// offset: 0xB48 | func: 8 | export: 8
s32 amsfx_is_playing(u32 soundHandle) {
    if (soundHandle == 0) {
        return 0;
    }

    if ((u32) sSndSlotsLen < soundHandle) {
        STUBBED_PRINTF("amSfxIsPlaying: Warning,sound handle '%d' out of range.\n", soundHandle);
        return 0;
    }

    if (sSndSlots[soundHandle].sndpHandle == NULL || sSndSlots[soundHandle].sndpHandle == (sndstate* )-1) {
        return 0;
    }

    return 1;
}

// offset: 0xBBC | func: 9 | export: 9
s32 amsfx_find_emitters_in_range(Object* listener, s32 distance, s32 minVolume, Object **foundObjs, s32 foundObjsMax) {
    Object* soundSource;
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

    if (distance != -1) {
        distance *= distance;
    }

    outCount = 0;
    for (i = 1; i < (sSndSlotsLen + 1); i++) {
        if ((sSndSlots[i].sndpHandle == 0) || (sSndSlots[i].sndpHandle == (sndstate*)-1)) {
            continue;
        }
        soundSource = sSndSlots[i].source;
        if (soundSource == NULL) {
            continue;
        }
        if ((minVolume != -1) && (sSndSlots[i].volume < minVolume)) {
            continue;
        }
        if (distance != -1) {
            get_object_child_position(listener, &sp74, &sp70, &sp6C);
            get_object_child_position(soundSource, &sp68, &sp64, &sp60);
            temp_fv1 = sp74 - sp68;
            temp_fv0 = sp70 - sp64;
            if (distance < (SQ(temp_fv1) + SQ(temp_fv0) + ((sp6C - sp60) * temp_fv0))) {
                continue;
            }
        }
        foundObjs[outCount++] = sSndSlots[i].source;
        if (outCount == foundObjsMax) {
            break;
        }
    }

    return outCount;
}

// offset: 0xDCC | func: 10 | export: 10
s32 amsfx_func_DCC(s32 arg0, UNK_TYPE_32 arg1, s32* arg2, s32* arg3) {
    *arg2 = 0;
    *arg3 = 0;
    return 0;
}

// offset: 0xDE8 | func: 11 | export: 11
s32 amsfx_get_default(u16 soundID, SoundDef* soundDef) {
    void* temp_v0;

    if (soundID <= 0 || (s32)_bss_10 < soundID) {
        STUBBED_PRINTF("amSfxGetDefault: Warning,sound effect value '%d' out of range.\n", soundID);
        bzero(soundDef, sizeof(SoundDef));
        return 0;
    }

    soundID--;
    temp_v0 = acache_get(sSoundDefCache, soundID);
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

// offset: 0xEE4 | func: 12 | export: 12
s32 amsfx_flush_cache(s32 arg0, UNK_TYPE_32 arg1) {
    acache_flush(sSoundDefCache);
    return 1;
}

// offset: 0xF2C | func: 13 | export: 13
void amsfx_snd_play_ex(Object* obj, u16 soundID, u32* soundHandle, char *filename, s32 lineNo) {
    u32 handle;

    if (!soundID || _bss_0->bankArray[0]->instArray[0]->soundCount < soundID) {
        STUBBED_PRINTF("amSndPlayEx: Warning,sound '%d' out of range.\n", soundID);
        return;
    }

    if (soundHandle == NULL) {
        handle = 0;
    } else if ((u32) sSndSlotsLen >= *soundHandle) {
        handle = *soundHandle;
    } else {
        STUBBED_PRINTF("amSndPlayEx: Warning,sound handle '%d' out of range.\n", *soundHandle);
        return;
    }
    
    handle = amsfx_make_handle(handle, filename, lineNo);
    some_sound_func(_bss_0->bankArray[0], soundID, 0x4000, 0x40, 1.0f, 0, 1, &sSndSlots[handle].sndpHandle);
    sSndSlots[handle].soundID = -1 - soundID;
    sSndSlots[handle].source = obj;
    bcopy(&sSndSlots[handle], sSndSlots, sizeof(SoundSlot));
    if (soundHandle != NULL) {
        *soundHandle = handle;
        return;
    }
    sSndSlots[handle].flags |= SOUNDSLOT_PLAYING;
}

// offset: 0x10D0 | func: 14 | export: 14
void amsfx_play2(Object* obj, u16 soundID, u32* soundHandle) {
    if (soundHandle != NULL) {
        amsfx_play(obj, soundID, MAX_VOLUME, soundHandle, "game/amsfx.c", 701, "");
    } else {
        amsfx_play(obj, soundID, MAX_VOLUME, NULL, "game/amsfx.c", 702, "");
    }
}

// offset: 0x1174 | func: 15 | export: 15
void amsfx_play3(Object* obj, u16 soundID, u8 volume, u32* soundHandle) {
    if (soundHandle != NULL) {
        amsfx_play(obj, soundID, volume, soundHandle, "game/amsfx.c", 709, "");
    } else {
        amsfx_play(obj, soundID, volume, NULL, "game/amsfx.c", 710, "");
    }
}

static const char str_4[] = "AMSFX: Software bug in object (controlfunc %d)\n";
static const char str_5[] = "       amSfxStop is required to release effect %d.\n";
static const char str_6[] = "AMSFX: Software bug in object (controlfunc %d)\n";
static const char str_7[] = "       amSfxStop is required to stop effect %d.\n";

// offset: 0x1218 | func: 16 | export: 16
void amsfx_free_object(Object *obj) {
    s32 i;
    s32 temp = (!obj) && (!obj);

    if (!obj) {
        return;
    }

    for (i = 1; i < (sSndSlotsLen + 1); i++) { 
        if (!(sSndSlots[i].flags & SOUNDSLOT_IN_USE)) { continue; }
        // @fake
        if (((0, obj)) != sSndSlots[i].source) { continue; }
        if (!(sSndSlots[i].flags & SOUNDSLOT_PLAYING)) {
            // @fake
            if (sSndSlots[i].sndpHandle) {}
            if (sSndSlots[i].sndpHandle != 0) {
                amsfx_free_handle(i);
            }
        }
        amsfx_free_handle(i);
        // @fake
        if (temp && (!obj)) {}
    }
}


// offset: 0x1320 | func: 17 | export: 20
void amsfx_play_custom(Object* obj, u16 soundID, f32 pitch, f32 minDist, f32 maxDist, u32* soundHandle) {
    f32 pad;
    f32 temp2;
    f32 temp;
    f32 camX;
    f32 camY;
    f32 camZ;
    s32 volume;
    Camera* camera;

    camera = get_camera_array();
    update_camera_for_object(camera);
    camX = camera->tx;
    camY = camera->ty;
    camZ = camera->tz;
    pad  = obj->globalPosition.y - camY;
    temp = obj->globalPosition.y - camY;
    temp = sqrtf(SQ(obj->globalPosition.x - camX) + SQ(temp) + SQ(obj->globalPosition.z - camZ));
    if (maxDist < temp) {
        volume = 0;
    } else if (temp <= minDist) {
        volume = MAX_VOLUME;
    } else {
        temp2 = (temp - minDist);
        temp = maxDist - minDist;
        temp2 = temp - (temp2);
        temp2 = ((temp2 * MAX_VOLUME_F) / temp);
        volume = temp2;
        if (volume > MAX_VOLUME) {
            volume = MAX_VOLUME;
        }
        if (volume < 0) {
            volume = 0;
        }
    }
    if ((volume > 0) && (*soundHandle == 0)) {
        amsfx_play(obj, soundID, volume, soundHandle, "game/amsfx.c", 891, "");
        amsfx_set_pitch(*soundHandle, pitch);
    }
    if (*soundHandle != 0) {
        if (volume == 0) {
            amsfx_stop(*soundHandle);
            *soundHandle = 0;
            return;
        }
        amsfx_set_vol(*soundHandle, volume);
    }
}

static const char str_11[] = "amSfxSetPitch: Warning,sound handle '%d' out of range.\n";
static const char str_12[] = "amSfxSetPitch: Warning,invalid handle '%d'.\n";

// offset: 0x1504 | func: 18 | export: 17
void amsfx_update_doppler(s32 soundHandle, Object* emitter, Object* listener, f32 arg3) {
    sndstate* sndpHandle;
    f32 pitch;
    f32 dist;
    f32 prevDist;
    f32 diffX;
    f32 diffY;
    f32 diffZ;
    f32 prevDiffX;
    f32 prevDiffY;
    f32 prevDiffZ;

    if (soundHandle == 0) {
        return;
    }

    diffX = listener->srt.transl.x - emitter->srt.transl.x;
    diffY = listener->srt.transl.y - emitter->srt.transl.y;
    diffZ = listener->srt.transl.z - emitter->srt.transl.z;
    prevDiffX = listener->prevLocalPosition.x - emitter->prevLocalPosition.x;
    prevDiffY = listener->prevLocalPosition.y - emitter->prevLocalPosition.y;
    prevDiffZ = listener->prevLocalPosition.z - emitter->prevLocalPosition.z;
    dist = sqrtf(SQ(diffX) + SQ(diffY) + SQ(diffZ));
    prevDist = sqrtf(SQ(prevDiffX) + SQ(prevDiffY) + SQ(prevDiffZ));
    pitch = ((prevDist - dist) / arg3) + 1.0f;
    if (pitch < 0.1f) {
        pitch = 0.1f;
    } else if (pitch > 1.9f) {
        pitch = 1.9f;
    }
    if (sSndSlotsLen >= soundHandle) {
        sndpHandle = sSndSlots[soundHandle].sndpHandle;
        if (sndpHandle != NULL) {
            pitch *= sSndSlots[soundHandle].def.pitch / 100.0f;
            if ((sndpHandle != (sndstate* )-2) && (sndpHandle != (sndstate* )-1)) {
                audioPostEvent(sndpHandle, AL_SNDP_PITCH_EVT, ((void **)&pitch)[0]);
            }
        }
    }
}

// offset: 0x16E0 | func: 19 | export: 18
void amsfx_water_falls_control(void) {
    Object* player;
    s32 i;
    f32 distance;
    u32 lowVolume;
    u32 highVolume;
    u8 mapID;
    Vec3f* camera;

    player = get_player();
    mapID = map_world_xz_to_map_id(player->srt.transl.x, player->srt.transl.z);
    if ((mapID != sWaterfallsLastMap) || (sWaterfallFlags & AMSFX_WATERFALLS_REFRESH)) {
        amsfx_water_falls_find_sprays();
        sWaterfallsLastMap = mapID;
        sWaterfallFlags &= ~AMSFX_WATERFALLS_REFRESH;
    }
    if (sWaterFallSprayCount == 0) {
        return;
    }

    camera = (Vec3f*)get_camera();
    highVolume = 0;
    lowVolume = 0;
    for (i = 0; i < sWaterFallSprayCount; i++) {
        distance = vec3_distance(camera + 1, &sWaterFallSprays[i].pos);
        if (distance < sWaterFallSprays[i].unkC) {
            lowVolume += MAX_VOLUME - (u8)((u32)((distance / sWaterFallSprays[i].unkC) * MAX_VOLUME_F));
        }
        if (distance < sWaterFallSprays[i].unkE) {
            highVolume += MAX_VOLUME - (u8)((u32)((distance / sWaterFallSprays[i].unkE) * MAX_VOLUME_F));
        }
    }
    if (sWaterfallFlags & AMSFX_WATERFALLS_LOWER_HIGH) {
        highVolume >>= 1;
    }
    if (sWaterfallFlags & AMSFX_WATERFALLS_LOWER_LOW) {
        lowVolume >>= 1;
    }
    if (sWaterfallFlags & AMSFX_WATERFALLS_LOWER_HIGH2) {
        highVolume >>= 1;
    }
    if (sWaterfallFlags & AMSFX_WATERFALLS_LOWER_LOW2) {
        lowVolume >>= 1;
    }
    if (highVolume > MAX_VOLUME) {
        highVolume = MAX_VOLUME;
    }
    if (lowVolume > MAX_VOLUME) {
        lowVolume = MAX_VOLUME;
    }
    if ((lowVolume == 0) && (sWaterfallLowHandle != 0)) {
        amsfx_stop(sWaterfallLowHandle);
        sWaterfallLowHandle = 0;
    } else if (lowVolume != 0) {
        if (sWaterfallLowHandle == 0) {
            sWaterfallLowVolume = 1;
            amsfx_play(NULL, SOUND_986_Waterfall_Low_Loop, sWaterfallLowVolume, &sWaterfallLowHandle, "game/amsfx.c", 1016, "");
        }
        if (lowVolume < sWaterfallLowVolume) {
            sWaterfallLowVolume -= 1;
        } else {
            sWaterfallLowVolume += 1;
        }
        amsfx_set_vol(sWaterfallLowHandle, sWaterfallLowVolume);
    }
    if ((highVolume == 0) && (sWaterfallHighHandle != 0)) {
        amsfx_stop(sWaterfallHighHandle);
        sWaterfallHighHandle = 0;
        return;
    }
    if (highVolume != 0) {
        if (sWaterfallHighHandle == 0) {
            sWaterfallHighVolume = 1;
            amsfx_play(NULL, SOUND_987_Waterfall_High_Loop, sWaterfallHighVolume, &sWaterfallHighHandle, "game/amsfx.c", 1036, "");
        }
        if (highVolume < sWaterfallHighVolume) {
            sWaterfallHighVolume -= 1;
        } else {
            sWaterfallHighVolume += 1;
        }
        amsfx_set_vol(sWaterfallHighHandle, sWaterfallHighVolume);
    }
}

static const char str_13[] = "AMSFX: Maximum sfx handles allocated,%d (from %s:%d)\n";
static const char str_14[] = "AMSFX: allocSfxHandle handle out of range, %d\n";
static const char str_15[] = "AMSFX: freeSfxHandle invalid handle, %d\n";
static const char str_16[] = "AMSFX: freeSfxHandle not used, %d\n";
static const char str_17[] = "AMSFX: Warning min/max range identical\n";

// offset: 0x1C10 | func: 20 | export: 19
void amsfx_water_falls_set_flags(u8 flags) {
    sWaterfallFlags = flags;
}

// offset: 0x1C38 | func: 21
static s32 amsfx_water_falls_find_sprays(void) {
    Object* player;
    s32 offset;
    ObjSetup* setup;
    s32 setupListLength;

    player = get_player();
    if (player == NULL) {
        return TRUE;
    }
    setup = map_world_xz_to_map_obj_setup_list(player->srt.transl.x, player->srt.transl.z, &setupListLength);
    if (setup == NULL) {
        return TRUE;
    }
    offset = 0;
    sWaterFallSprayCount = 0;
    while (setupListLength > offset && sWaterFallSprayCount < MAX_WATER_FALL_SPRAY) {
        if (setup->objId == OBJ_WaterFallSpray) {
            sWaterFallSprays[sWaterFallSprayCount].pos.x = setup->x;
            sWaterFallSprays[sWaterFallSprayCount].pos.y = setup->y;
            sWaterFallSprays[sWaterFallSprayCount].pos.z = setup->z;
            sWaterFallSprays[sWaterFallSprayCount].unkC = ((WaterFallSpray_Setup*)setup)->unk21 * 16;
            sWaterFallSprays[sWaterFallSprayCount].unkE = ((WaterFallSpray_Setup*)setup)->unk22 * 16;
            sWaterFallSprayCount++;
        }
        offset += setup->quarterSize << 2;
        setup = (ObjSetup*)((u8*)setup + (setup->quarterSize << 2));
    }
    return FALSE;
}

// offset: 0x1D58 | func: 22
static s32 amsfx_make_handle(s32 handle, char *filename, s32 lineNo) {
    SoundSlot* var_v0;
    s32 temp_a0;
    s32 i;

    if (handle == 0) {
        for (i = 1; i < (sSndSlotsLen + 1); i++) {
            if (!(sSndSlots[i].flags & SOUNDSLOT_IN_USE)) {
                break;
            }
        }
        if (i == (sSndSlotsLen + 1)) {
            sSndSlotsLen++;
            if (sSndSlotsLen == MAX_SOUND_SLOTS) {
                sSndSlotsLen--;
                i = 1;
            }
        }
        handle = i;
    } else if ((u32) sSndSlotsLen < (u32) handle) {
        handle = 0;
    }
    bzero(&sSndSlots[handle], sizeof(SoundSlot));
    sSndSlots[handle].flags |= SOUNDSLOT_IN_USE;
    return handle;
}

// offset: 0x1E64 | func: 23
static s32 amsfx_free_handle(u32 handle) {
    sndstate* sndpHandle;

    if (handle == 0) {
        return 0;
    }
    if (!(sSndSlots[handle].flags & SOUNDSLOT_IN_USE)) {
        return 0;
    }

    sndpHandle = sSndSlots[handle].sndpHandle;
    if ((sndpHandle != NULL) && (sndpHandle != (sndstate* )-1)) {
        if (sndpHandle == sSndSlots->sndpHandle) {
            sSndSlots->flags = 0;
            sSndSlots->sndpHandle = 0;
        }
        if (sndpHandle == (sndstate* )-2) {
            mp3_stop();
        } else {
            audioStop(sndpHandle);
        }
    }
    sSndSlots[handle].sndpHandle = NULL;
    sSndSlots[handle].source = NULL;
    sSndSlots[handle].flags = 0;
    return 0;
}

// offset: 0x1F78 | func: 24
static void amsfx_func_1F78(void) {
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

    for (i = 1; i < (sSndSlotsLen + 1); i++) {
        temp_a0 = sSndSlots[i].source;
        if (temp_a0 == NULL) { continue; }

        temp_s0 = sSndSlots[i].sndpHandle;
        if (temp_s0 == NULL) { continue; }
        if (temp_s0 == (sndstate* )-1) { continue; } 
        if (!(sSndSlots[i].def.volumeFalloff & 3)) { continue; }
        
        amsfx_func_2240(temp_a0, &sp6C, &sp68, &sp64, &sp5A);
        amsfx_func_22FC(sp6C, sp68, sp64, &sSndSlots[i].def, &vol);
        amsfx_func_2438(sp6C, sp64, sp5A, &pan, &fx);
        sSndSlots[i].distVolume = vol;
        if (pan != (s8) sSndSlots[i].unk14) {
            sSndSlots[i].unk14 = (u8) pan;
            if (temp_s0 == (sndstate* )-2) {
                mp3_set_pan(pan, FALSE);
            } else {
                audioPostEvent(temp_s0, AL_SNDP_PAN_EVT, (void *)(s32)pan);
            }
        }
        vol = (s32) (sSndSlots[i].volume * sSndSlots[i].distVolume) >> 7;
        if (vol < sSndSlots[i].def.unk3) {
            vol = sSndSlots[i].def.unk3;
        }
        if (vol != (s8)sSndSlots[i].unk13) {
            sSndSlots[i].unk13 = vol;
            if (temp_s0 == (sndstate* )-2) {
                mp3_set_volume(vol << 8, 0);
            } else {
                audioPostEvent(temp_s0, AL_SNDP_VOL_EVT, (void*)(vol << 8));
            }
        }
        fx = fx > 0 ? 0 : -0x80;
        if (fx != sSndSlots[i].unk15) {
            sSndSlots[i].unk15 = fx;
            if (temp_s0 != (sndstate* )-2) {
                audioPostEvent(temp_s0, AL_SNDP_FX_EVT, (void*)(s32)(fx));
            }
        }
    }
}

// offset: 0x2240 | func: 25
static void amsfx_func_2240(Object* obj, f32* xo, f32* yo, f32* zo, u16* yawOut) {
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
static void amsfx_func_22FC(f32 x, f32 y, f32 z, SoundDef* soundDef, s8* outVolume) {
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
static void amsfx_func_2438(f32 x, f32 z, s32 arg2, s8* outPan, s8* outFx) {
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
