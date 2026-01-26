#include "PR/ultratypes.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/objects/325_trigger.h"
#include "game/gamebits.h"
#include "game/objects/object_id.h"
#include "libnaudio/n_libaudio.h"
#include "sys/asset_thread.h"
#include "sys/audio.h"
#include "sys/fs.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "dll.h"
#include "functions.h"

extern s32 D_800967A0[127];
extern f32 D_8009699C[100];

// size:0x24C
typedef struct {
/*000*/ u8 unk0;
/*004*/ N_ALCSPlayer seqp;
/*090*/ ALCSeq seq;
/*118*/ void *midiData;
/*18C*/ u8 currentSeqID;
/*18E*/ s16 bpm;
/*190*/ s16 volume;
/*192*/ u16 unk192;
/*194*/ u16 unk194; // enabled channels?
/*196*/ u16 unk196; // dirty channels?
/*198*/ u16 unk198; // ignore channels?
/*19A*/ u8 volUpRate; // volume per tick
/*19B*/ u8 volDownRate; // volume per tick
/*19C*/ s16 channelVolumes[16];
/*1BC*/ u8 _unk1BC[0x1FC - 0x1BC];
/*1FC*/ u8 nextSeqID; // current music/ambient id?
/*1FE*/ s16 nextBPM;
/*200*/ u16 unk200;
/*202*/ s16 targetVolume;
/*204*/ u16 unk204;
/*206*/ u16 unk206;
/*208*/ u16 unk208;
/*20A*/ u8 nextVolUpRate; // volume per tick
/*20B*/ u8 nextVolDownRate; // volume per tick
/*20C*/ u8 _unk20C[0x24C - 0x20C];
} AMSEQPlayer;

typedef struct {
/*0*/ s32 unk0;
/*4*/ u8 _unk4[4];
} AMSEQBss18; 

typedef struct {
/*00*/ Object *obj;
/*04*/ MusicAction action;
/*24*/ s16 actionNo;
/*26*/ s16 unk26;
/*28*/ s16 unk28;
} AMSEQHandle;

/*0x0*/ static u8 _data_0 = 0;
/*0x4*/ static u8 _data_4 = 0;
/*0x8*/ static Object *sFocusObj = NULL;
/*0xC*/ static MusicAction* sMusicAction = NULL;

/*0x0*/ static AMSEQPlayer **sSeqPlayers; // pointer to array of 4 pointers
/*0x4*/ static ALBankFile **sBankFiles;
/*0x8*/ static ALSeqFile *sSeqFiles[2];
/*0x10*/ static struct oscstate *sOscStates;
/*0x14*/ static struct oscstate *sFreeOscStateList;
/*0x18*/ static AMSEQBss18 *_bss_18;
/*0x1C*/ static s32 _bss_1C;
/*0x20*/ static AMSEQHandle *sSeqHandles;
/*0x24*/ static s32 sNumSeqHandles; // current number of sequence handles
/*0x28*/ static u16 sVolumeGameOption; // set by game options, scales amseq volume
/*0x2A*/ static u16 sUnkVolume; // set to 256 and never changed, scales amseq volume
/*0x2C*/ static u16 sCurrGlobalVolume;

void amseq_play_ex(u8 arg0, u8 arg1, s16 arg2, s16 arg3, u16 arg4);
/*static*/ void amseq_update_dist_falloff(void);
/*static*/ void amseq_start_next_sequence(u8 arg0);
/*static*/ void amseq_update_fade(u8 arg0);
static void amseq_determine_vols(u8 arg0, u8 arg1, u16 arg2, u16 arg3, u16 arg4, s8* arg5);
static void amseq_func_14A0(AMSEQPlayer *arg0);
/*static*/ AMSEQPlayer* amseq_func_135C(s32 arg0, s32 maxChannels, s32 maxVoices, s32 arg3, s32 maxEvents);
/*static*/ ALMicroTime amseq_initOscMain(void **oscState,f32 *initVal, u8 oscType,
                                   u8 oscRate, u8 oscDepth, u8 oscDelay, u8 unk07);
/*static*/ ALMicroTime amseq_updateOscMain(void *oscState, f32 *updateVal);
/*static*/ void amseq_stopOscMain(void *oscState);
static s32 amseq_func_1E8C(MusicAction *action, s8 a1, s16 actionNo);

// offset: 0x0 | ctor
#ifndef NON_MATCHING
void amseq_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/amseq_ctor.s")
#else
// https://decomp.me/scratch/aRlW8
void amseq_ctor(void* dll) {
    s32* tab;
    s32 sp48;
    s32 sp44;
    u32 sp40;

    tab = NULL;
    sSeqPlayers = mmAlloc(sizeof(AMSEQPlayer*) * 4, ALLOC_TAG_SEQ_COL, NULL);
    bzero(sSeqPlayers, sizeof(AMSEQPlayer*) * 4);
    sBankFiles = mmAlloc(sizeof(ALBankFile*) * 2, ALLOC_TAG_SEQ_COL, NULL);
    bzero(sBankFiles, sizeof(ALBankFile*) * 2);
    queue_alloc_load_file((void**)&tab, AMBIENT_TAB);
    sp44 = tab[0];
    sp48 = tab[1] - sp44;
    if (sp48 != 0) {
        sBankFiles[0] = mmAlloc(sp48, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void**)sBankFiles[0], AMBIENT_BIN, sp44, sp48);
        alBnkfNew(sBankFiles[0], (u8*)file_get_romaddr(AMBIENT_BIN, tab[1]));
    }
    mmFree(tab);
    queue_alloc_load_file((void**)&tab, MUSIC_TAB);
    sp44 = tab[0];
    sp48 = tab[1] - sp44;
    if (sp48 != 0) {
        sBankFiles[1] = mmAlloc(sp48, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void**)sBankFiles[1], MUSIC_BIN, sp44, sp48);
        alBnkfNew(sBankFiles[1], (u8*)file_get_romaddr(MUSIC_BIN, tab[1]));
    }
    mmFree(tab);
    queue_alloc_load_file((void**)&tab, AUDIO_TAB);
    sp44 = tab[2];
    if (tab[3] != sp44) {
        sSeqFiles[0] = mmAlloc(4, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void** ) sSeqFiles[0], AUDIO_BIN, sp44, 4);
        sp48 = (sSeqFiles[0]->seqCount * 8) + 4;
        mmFree(sSeqFiles[0]);
        sSeqFiles[0] = mmAlloc(sp48, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void** ) sSeqFiles[0], AUDIO_BIN, sp44, sp48);
        alSeqFileNew(sSeqFiles[0], (u8*)file_get_romaddr(AUDIO_BIN, sp44));
        for (sp40 = 0; sp40 < sSeqFiles[0]->seqCount; sp40++) {
            if (sSeqFiles[0]->seqArray[sp40].len & 1) {
                sSeqFiles[0]->seqArray[sp40].len++;
            }
        }
    }
    sp44 = tab[1];
    if (tab[2] != sp44) {
        sSeqFiles[1] = mmAlloc(4, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void** ) sSeqFiles[1], AUDIO_BIN, sp44, 4);
        sp48 = (sSeqFiles[1]->seqCount * 8) + 4;
        mmFree(sSeqFiles[1]);
        sSeqFiles[1] = mmAlloc(sp48, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void**)sSeqFiles[1], AUDIO_BIN, sp44, sp48);
        alSeqFileNew(sSeqFiles[1], (u8*)file_get_romaddr(AUDIO_BIN, sp44));
        for (sp40 = 0; sp40 < sSeqFiles[1]->seqCount; sp40++) {
            if (sSeqFiles[1]->seqArray[sp40].len & 1) {
                sSeqFiles[1]->seqArray[sp40].len++;
            }
        }
    }
    mmFree(tab);
    sOscStates = sFreeOscStateList = mmAlloc(sizeof(struct oscstate) * 32, ALLOC_TAG_SEQ_COL, NULL);
    for (sp40 = 0; sp40 < 31; sp40++) {
        sOscStates[sp40].next = &sOscStates[sp40 + 1];
    }
    sOscStates[31].next = NULL;
    sSeqPlayers[0] = amseq_func_135C(0, 8, 8, 8, 100);
    sSeqPlayers[1] = amseq_func_135C(0, 8, 8, 8, 100);
    sSeqPlayers[2] = amseq_func_135C(1, 16, 32, 32, 120);
    sSeqPlayers[3] = amseq_func_135C(1, 16, 32, 32, 120);
    sMusicAction = mmAlloc(sizeof(MusicAction), ALLOC_TAG_SEQ_COL, NULL);
    _bss_1C = 16;
    _bss_18 = mmAlloc(_bss_1C * 8, ALLOC_TAG_SEQ_COL, NULL);
    while (_bss_1C != 0) {
        _bss_18[_bss_1C - 1].unk0 = -1;
        _bss_1C--;
    }
    sp40 = 0x2C; // @fake
    sNumSeqHandles = 16;
    sSeqHandles = mmAlloc(sNumSeqHandles * sp40, ALLOC_TAG_SEQ_COL, NULL);
    while (sNumSeqHandles > 0) {
        sSeqHandles[sNumSeqHandles - 1].obj = NULL;
        sNumSeqHandles--;
    }
    sVolumeGameOption = sUnkVolume = 256;
    sCurrGlobalVolume = (sVolumeGameOption * sUnkVolume) >> 8;
}
#endif

// offset: 0x7E8 | dtor
void amseq_dtor(void* dll) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (sBankFiles[i] != NULL) {
            mmFree(sBankFiles[i]);
        }
    }
    mmFree(sBankFiles);
    mmFree(sSeqFiles[0]);
    mmFree(sSeqFiles[1]);
    mmFree(sOscStates);
    for (i = 0; i < 4; i++) {
        amseq_func_14A0(sSeqPlayers[i]);
    }
    mmFree(sSeqPlayers);
    mmFree(_bss_18);
    mmFree(sSeqHandles);
}

// offset: 0x930 | func: 0 | export: 0
s32 amseq_set(Object *obj, u16 actionNo, const char *filename, s32 lineNo, const char *debugStr) {
    s32 i;
    s32 temp_v1;

    if (!actionNo) {
        return -1;
    }
    // "music %08x,%d\n"
    queue_load_file_region_to_ptr((void** ) sMusicAction, MUSICACTIONS_BIN, (actionNo - 1) * sizeof(MusicAction), sizeof(MusicAction));
    if (obj != NULL && (sMusicAction->unk18 & sMusicAction->unk1A) != 0) {
        // "object+fade\n"
        temp_v1 = sMusicAction->unk18 & sMusicAction->unk1A;
        if (temp_v1 != 0) { // lolwat
            for (i = 0; i < sNumSeqHandles; i++) {
                if (sSeqHandles[i].obj == obj) {
                    break;
                }
            }
            if (i == sNumSeqHandles) {
                sNumSeqHandles++;
                if (sNumSeqHandles == 16) {
                    // "AUDIO: Maximum sequence handles reached.\n"
                }
                sSeqHandles[i].actionNo = actionNo;
                sSeqHandles[i].obj = obj;
                sSeqHandles[i].unk28 = -1;
                sSeqHandles[i].unk26 = sSeqHandles[i].unk28;
                bcopy(sMusicAction, &sSeqHandles[i].action, sizeof(MusicAction));
                // "registered %d,%08x\n"
            } else {
                // "already registered %d,%08x\n";
            }
            if (temp_v1 != 0xFFFF) {
                // "starting non-fade channels\n"
                return amseq_func_1E8C(sMusicAction, 0, actionNo);
            }
        }
        goto bail;
    }
    // "starting static sequence\n"
    return amseq_func_1E8C(sMusicAction, 0, actionNo);

    bail:
    return -1;
}

// offset: 0xB0C | func: 1 | export: 1
void amseq_free(Object *obj, u16 actionNo, const char *filename, s32 lineNo, const char *debugStr) {
    s32 i;

    if (sNumSeqHandles > 0) {
        for (i = 0; i < sNumSeqHandles; i++) {
            if ((obj == sSeqHandles[i].obj) && ((actionNo == 0xFFFF) || (actionNo == sSeqHandles[i].actionNo))) {
                break;
            }
        }
        if (i != sNumSeqHandles) {
            if (sSeqHandles[i].obj != NULL) {
                sNumSeqHandles--;
                bcopy(&sSeqHandles[sNumSeqHandles], &sSeqHandles[i], sizeof(AMSEQHandle));
            }
        }
    }
}

// offset: 0xC00 | func: 2 | export: 2
s32 amseq_is_set(Object *obj, u16 actionNo) {
    s32 i;

    for (i = 0; i < sNumSeqHandles; i++) {
        if ((obj == sSeqHandles[i].obj) && (actionNo == sSeqHandles[i].actionNo)) {
            return 1;
        }
    }

    return 0;
}

// offset: 0xC6C | func: 3 | export: 4
void amseq_play(u8 playerNo, u8 seqID) {
    amseq_play_ex(playerNo, seqID, 0x64, 0x7F, 0xFFFF);
}

// offset: 0xCC8 | func: 4 | export: 5
void amseq_play_ex(u8 playerNo, u8 seqID, s16 bpm, s16 volume, u16 arg4) {
    if (seqID == 0) {
        return;
    }
    if (playerNo >= 4) {
        // "amSeqPlayEx: Warning, player value '%d' out of range.\n"
        return;
    }
    if (sSeqFiles[playerNo >> 1]->seqCount < seqID) {
        // "amSeqPlayEx: Warning, sequence value '%d' out of range.\n"
        *((volatile u8*)NULL) = 0;
        return;
    }
    if (!(sSeqPlayers[playerNo]->unk0 & 2) && (sCurrGlobalVolume != 0)) {
        sSeqPlayers[playerNo]->nextSeqID = seqID;
        sSeqPlayers[playerNo]->nextBPM = bpm;
        sSeqPlayers[playerNo]->targetVolume = volume << 4;
        sSeqPlayers[playerNo]->unk200 = arg4;
        sSeqPlayers[playerNo]->unk204 = 0;
        sSeqPlayers[playerNo]->unk206 = 0;
        sSeqPlayers[playerNo]->unk208 = 0;
        sSeqPlayers[playerNo]->nextVolUpRate = 0;
        sSeqPlayers[playerNo]->nextVolDownRate = 0;
    }
}

// offset: 0xE14 | func: 5 | export: 6
void amseq_stop(u8 playerNo) {
    if (playerNo < 4) {
        if (!(sSeqPlayers[playerNo]->unk0 & 2) && (sSeqPlayers[playerNo]->unk0 & 1)) {
            n_alCSPStop(&sSeqPlayers[playerNo]->seqp);
            sSeqPlayers[playerNo]->nextSeqID = 0;
            sSeqPlayers[playerNo]->currentSeqID = 0;
            sSeqPlayers[playerNo]->unk0 &= ~1;
        }
    } else {
        // "amSeqStop: Warning, player value '%d' out of range.\n"
    }
}

// offset: 0xEE4 | func: 6 | export: 7
u8 amseq_get_no(u8 playerNo) {
    AMSEQPlayer* player;

    if (playerNo >= 4) {
        // "amSeqGetNo: Warning, player value '%d' out of range.\n"
        return 0;
    }
    player = sSeqPlayers[playerNo];
    if (player->nextSeqID != 0) {
        return player->nextSeqID;
    }
    if (player->currentSeqID != 0) {
        return player->currentSeqID;
    }
    return 0;
}

// offset: 0xF58 | func: 7 | export: 8
u16 amseq_get_volume_option(void) {
    return sVolumeGameOption;
}

// offset: 0xF74 | func: 8 | export: 9
void amseq_set_volume_option(u32 volume) {
    AMSEQPlayer* player;

    if (volume > 256) {
        volume = 256;
    }
    sVolumeGameOption = volume;
    sCurrGlobalVolume = (sVolumeGameOption * sUnkVolume) >> 8;
    player = sSeqPlayers[0];
    n_alCSPSetVol(&player->seqp, (s16) ((player->volume >> 4) * sCurrGlobalVolume));
    player = sSeqPlayers[1];
    n_alCSPSetVol(&player->seqp, (s16) ((player->volume >> 4) * sCurrGlobalVolume));
    player = sSeqPlayers[2];
    n_alCSPSetVol(&player->seqp, (s16) ((player->volume >> 4) * sCurrGlobalVolume));
    // @bug: Doesn't affect the 4th sequence player
}

// offset: 0x1090 | func: 9 | export: 10
UNK_TYPE_32 amseq_func_1090(void) {
    return 0;
}

// offset: 0x109C | func: 10 | export: 11
void amseq_func_109C(UNK_TYPE_32 arg0) {

}

// offset: 0x10A8 | func: 11 | export: 12
u8 amseq_get_volume(u8 playerNo) {
    if (playerNo >= 4) {
        // "amSeqGetVolume: Warning, player value '%d' out of range.\n"
        return 0;
    }
    return sSeqPlayers[playerNo]->volume >> 4;
}

// offset: 0x10FC | func: 12 | export: 13
void amseq_set_volume(u8 playerNo, u8 volume) {
    if (playerNo < 4) {
        sSeqPlayers[playerNo]->targetVolume = volume << 4;
    } else {
        // "amSeqSetVolume: Warning, player value '%d' out of range.\n"
    }
}

// offset: 0x1150 | func: 13 | export: 14
UNK_TYPE_32 amseq_func_1150(UNK_TYPE_32 arg0) {
    return 0;
}

// offset: 0x1160 | func: 14 | export: 15
void amseq_func_1160(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x1170 | func: 15 | export: 16
void amseq_func_1170(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x1180 | func: 16 | export: 18
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/amseq_tick.s")
#else
// needs amseq_update_fade, amseq_start_next_sequence, amseq_update_dist_falloff to be static
void amseq_tick(void) {
    s32 i;

    if (audio_func_80012348() != 0) {
        for (i = 0; i < 4; i++) {
            amseq_start_next_sequence(i);
            amseq_update_fade(i);
        }
        amseq_update_dist_falloff();
    }
}
#endif

// offset: 0x1228 | func: 17 | export: 3
void amseq_set_focus_obj(Object *arg0) {
    sFocusObj = arg0;
}

// offset: 0x1244 | func: 18 | export: 19
void amseq_func_1244(UNK_TYPE_32 arg0) {

}

// offset: 0x1250 | func: 19 | export: 20
void amseq_func_1250(UNK_TYPE_32 arg0) {

}

// offset: 0x125C | func: 20 | export: 21
void amseq_func_125C(UNK_TYPE_32 arg0) {

}

// offset: 0x1268 | func: 21 | export: 22
void amseq_func_1268(UNK_TYPE_32 arg0) {

}

// offset: 0x1274 | func: 22 | export: 23
void amseq_func_1274(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x1284 | func: 23 | export: 24
void amseq_func_1284(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x1294 | func: 24 | export: 25
void amseq_func_1294(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x12A4 | func: 25 | export: 26
UNK_TYPE_32 amseq_func_12A4(UNK_TYPE_32 arg0) {
    return 0;
}

// offset: 0x12B4 | func: 26 | export: 27
void amseq_func_12B4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x12C4 | func: 27 | export: 28
void amseq_func_12C4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x12D4 | func: 28 | export: 29
void amseq_func_12D4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x12E4 | func: 29 | export: 30
UNK_TYPE_32 amseq_func_12E4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {
    return 0;
}

// offset: 0x12F8 | func: 30 | export: 31
UNK_TYPE_32 amseq_func_12F8(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {
    return 0x7F;
}

// offset: 0x130C | func: 31 | export: 32
void amseq_func_130C(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {

}

// offset: 0x1320 | func: 32 | export: 33
UNK_TYPE_32 amseq_func_1320(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {
    return 0;
}

// offset: 0x1334 | func: 33 | export: 34
void amseq_func_1334(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {

}

// offset: 0x1348 | func: 34 | export: 35
void amseq_func_1348(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {

}

// offset: 0x135C | func: 35
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/amseq_func_135C.s")
#else
// needs amseq_initOscMain, amseq_updateOscMain, amseq_stopOscMain to be static
AMSEQPlayer* amseq_func_135C(s32 bankFileIdx, s32 maxChannels, s32 maxVoices, s32 arg3, s32 maxEvents) {
    AMSEQPlayer* player;
    ALSeqpConfig config;
    s32 i;

    player = mmAlloc(sizeof(AMSEQPlayer), ALLOC_TAG_SEQ_COL, NULL);
    bzero(player, sizeof(AMSEQPlayer));
    config.heap = NULL;
    config.maxVoices = maxVoices;
    config.maxEvents = maxEvents;
    config.unk1C = arg3;
    config.maxChannels = maxChannels;
    config.initOsc = &amseq_initOscMain;
    config.updateOsc = &amseq_updateOscMain;
    config.stopOsc = &amseq_stopOscMain;
    n_alCSPNew(&player->seqp, &config);
    n_alCSPSetBank(&player->seqp, (ALBank*)sBankFiles[bankFileIdx]->bankArray);
    player->midiData = 0;
    player->currentSeqID = 0;
    player->seqp.chanMask = 0xFFFF;
    for (i = 0; i < player->seqp.maxChannels; i++) {
        player->channelVolumes[i] = 0x7F;
    }
    return player;
}
#endif

// offset: 0x14A0 | func: 36
static void amseq_func_14A0(AMSEQPlayer *player) {
    n_alCSPDelete(&player->seqp);
    if (player->midiData != NULL) {
        mmFree(player->midiData);
    }
    mmFree(player);
}

// offset: 0x1514 | func: 37
void amseq_start_next_sequence(u8 playerNo) {
    AMSEQPlayer* player;
    N_ALCSPlayer* seqp;
    s32 idx;
    s32 offset;
    s32 seqFileIdx;
    s8 channelVolumes[16]; // has to be at least 16
    s32 temp;

    player = sSeqPlayers[playerNo];
    if (player->nextSeqID == 0) {
        // Next sequence not set, nothing to start, let current sequence continue (if any)
        return;
    }
    seqp = &player->seqp;
    if (n_alCSPGetState(seqp) != AL_STOPPED) {
        n_alCSPStop(seqp);
        player->currentSeqID = 0;
        player->unk0 &= ~0x1;
        return;
    }
    if (((playerNo == 2) || (playerNo == 3)) && (main_get_bits(BIT_DisableMusic) != 0)) {
        player->currentSeqID = 0;
        player->unk0 &= ~0x1;
        return;
    }
    // "startSequencer %d\n"
    player->currentSeqID = player->nextSeqID;
    player->nextSeqID = 0;
    if (playerNo >= 2) {
        seqFileIdx = 1;
    } else {
        seqFileIdx = 0;
    }
    idx = player->currentSeqID - 1;
    offset = sSeqFiles[seqFileIdx]->seqArray[idx].offset - (u8*)file_get_romaddr(AUDIO_BIN, 0);
    if (player->midiData != NULL) {
        mmFree(player->midiData);
    }
    player->midiData = mmAlloc(sSeqFiles[seqFileIdx]->seqArray[idx].len, ALLOC_TAG_SEQ_COL, NULL);
    if (player->midiData != NULL) {
        // "download sequence %d:%d\n"
        queue_load_file_region_to_ptr(player->midiData, AUDIO_BIN, offset, sSeqFiles[seqFileIdx]->seqArray[idx].len);
        n_alCSeqNew(&player->seq, (u8* ) player->midiData);
        n_alCSPSetSeq(seqp, &player->seq);
        n_alCSPPlay(seqp);
        player->bpm = player->nextBPM;
        if (player->bpm != 0) {
            n_alCSPSetTempo(seqp, (s32) ((1.0f / (f32) player->bpm) * 6e7f));
        }
        player->volUpRate = player->nextVolUpRate;
        player->volDownRate = player->nextVolDownRate;
        if (player->volUpRate) {
            player->volume = 0;
            player->targetVolume = player->targetVolume;
        } else {
            player->volume = player->targetVolume = player->targetVolume;
        }
        n_alCSPSetVol(seqp, (player->volume >> 4) * sCurrGlobalVolume);
        player->unk192 = player->unk200;
        player->seqp.chanMask = player->unk192;
        player->unk194 = player->unk204;
        player->unk196 = player->unk206;
        player->unk198 = player->unk208;
        amseq_determine_vols(0, player->seqp.maxChannels, player->unk196, player->unk198, player->unk194, channelVolumes);
        for (idx = 0, temp = 1; idx < player->seqp.maxChannels; idx++, temp <<= 1) {
            switch (channelVolumes[idx]) {
                case -1:
                    func_80075E38(seqp, idx);
                    n_alCSPSetAmSeqChlVol(seqp, idx, 0);
                    player->channelVolumes[idx] = 0;
                    break;
                case -2:
                    break;
                case -3:
                    channelVolumes[idx] = (s8) player->channelVolumes[idx];
                    /* fallthrough */
                default:
                    func_80075E9C(seqp, idx);
                    n_alCSPSetAmSeqChlVol(seqp, idx, channelVolumes[idx]);
                    player->channelVolumes[idx] = channelVolumes[idx] << 4;
                    break;
            }
        }
        player->unk0 |= 1;
    } else {
        // "AUDIO: MIDI sequence buffer failed.\n"
    }
}

// offset: 0x1924 | func: 38
void amseq_update_fade(u8 playerNo) {
    AMSEQPlayer* player;
    N_ALCSPlayer* seqp;
    s32 volume;
    s32 temp_s7;
    s32 chanMask;
    s32 chan;

    player = sSeqPlayers[playerNo];
    seqp = &player->seqp;
    if (n_alCSPGetState(seqp) == AL_PLAYING) {
        if (player->nextSeqID == 0) {
            if (player->targetVolume != player->volume) {
                if (player->volUpRate == 0) {
                    player->volume = player->targetVolume;
                    if (player->targetVolume == 0) {
                        n_alCSPStop(seqp);
                    }
                } else if (player->volume < player->targetVolume) {
                    player->volume += (player->volUpRate * gUpdateRate);
                    if (player->targetVolume < player->volume) {
                        player->volume = player->targetVolume;
                    }
                } else {
                    player->volume -= (player->volDownRate * gUpdateRate);
                    if (player->targetVolume >= player->volume) {
                        player->volume = player->targetVolume;
                        if (player->targetVolume == 0) {
                            n_alCSPStop(seqp);
                        }
                    }
                }
                n_alCSPSetVol(seqp, (player->volume >> 4) * sCurrGlobalVolume);
            }
        }
        temp_s7 = player->unk196 & ~player->unk198;
        if (temp_s7 != 0) {
            chan = 0;
            chanMask = 1;
            while (chan < player->seqp.maxChannels) {
                if (temp_s7 & chanMask) {
                    volume = player->channelVolumes[chan];
                    if (player->unk194 & chanMask) {
                        // fade in channel
                        func_80075E9C(seqp, chan);
                        player->unk192 |= chanMask;
                        player->seqp.chanMask = player->unk192;
                        volume += (player->volUpRate * gUpdateRate);
                        if (volume >= (127 << 4)) {
                            volume = (127 << 4);
                            player->unk196 &= ~chanMask;
                        }
                    } else {
                        // fade out channel
                        volume -= (player->volDownRate * gUpdateRate);
                        if (volume <= 0) {
                            volume = 0;
                            player->unk196 &= ~chanMask;
                            func_80075E38(seqp, chan);
                            player->unk192 |= chanMask;
                            player->seqp.chanMask = player->unk192;
                        }
                    }
                    player->channelVolumes[chan] = volume;
                    n_alCSPSetAmSeqChlVol(seqp, chan, (volume >> 4));
                }
                chan += 1;
                chanMask <<= 1;
            }
        }
    }
}

// offset: 0x1C08 | func: 39
static s32 amseq_func_1C08(u8 playerNo, s32 seqID, s8* arg2, s8* arg3) {
    switch (playerNo) {
    case 0:
    case 1:
        if (seqID == amseq_get_no(0)) {
            *arg2 = 0;
            *arg3 = 1;
            return 1;
        }
        if (seqID == amseq_get_no(1)) {
            *arg2 = 1;
            *arg3 = -1;
            return 1;
        }
        break;
    case 2:
    case 3:
        if (seqID == amseq_get_no(2)) {
            *arg2 = 2;
            *arg3 = 3;
            return 1;
        }
        if (seqID == amseq_get_no(3)) {
            *arg2 = 3;
            *arg3 = -1;
            return 1;
        }
        break;
    }

    return 0;
}

// offset: 0x1D60 | func: 40
static void amseq_func_1D60(u8 arg0, s8* arg1, s8* arg2, u8 arg3) {
    switch (arg0) {
    case 0:
        if (arg3 == 0) {
            _data_0 = 1 - _data_0;
        }
        *arg1 = _data_0;
        *arg2 = 1 - _data_0;
        return;
    case 1:
        *arg1 = 1 - _data_0;
        *arg2 = -1;
        return;
    case 2:
        if (arg3 == 0) {
            _data_4 = 1 - _data_4;
        }
        *arg1 = _data_4 + 2;
        *arg2 = 3 - _data_4;
        return;
    case 3:
        *arg1 = 3 - _data_4;
        *arg2 = -1;
        return;
    default:
        *arg1 = -1;
        *arg2 = 0;
        return;
    }
}

// offset: 0x1E8C | func: 41
static s32 amseq_func_1E8C(MusicAction *action, s8 a1, s16 actionNo) {
    AMSEQPlayer* player;
    s8 playerNo;
    s8 sp82;
    PlayerMusicAction* playerMusicAction;
    s32 temp_s6;
    s32 chan;
    s8 channelVolumes[16];
    u16 temp_v1;
    s32 temp;

    playerMusicAction = gDLL_29_Gplay->vtbl->func_FE8();
    playerNo = -1;
    sp82 = -1;
    if (action->seqID != 0) {
        // @fake
        if (action->playerNo) {}
        // Update savegame music action
        playerMusicAction->actionNums[action->playerNo] = actionNo;
        temp = amseq_func_1C08(action->playerNo, action->seqID, &playerNo, &sp82) == 0;
        temp_v1 = action->unk1A;
        temp_s6 = (temp_v1 & action->unk16);
        if (temp || (temp_s6 != 0)) {
            if (temp_s6 == 0) {
                amseq_func_1D60(action->playerNo, &playerNo, &sp82, temp_v1);
            } else {
                if (playerNo == -1) {
                    amseq_func_1D60(action->playerNo, &playerNo, &sp82, temp_v1);
                }
                sp82 = -1;
            }
            player = sSeqPlayers[playerNo];
            player->nextSeqID = action->seqID;
            player->targetVolume = action->volume << 4;
            player->nextBPM = (s16) action->bpm;
            temp_s6 = (action->unk18 | action->unk1A);
            player->unk204 &= ~temp_s6;
            player->unk206 &= ~temp_s6;
            player->unk208 &= ~temp_s6;
            player->unk204 |= action->unk16;
            player->unk206 |= action->unk18;
            player->unk208 |= action->unk1A;
            player->unk200 = 0xFFFF;
            player->unk200 &= action->unk1A & ~action->unk16;
            if (action->fadeTimeDs != 0) {
                player->nextVolUpRate = (f32) (action->volume << 4) / (f32) (action->fadeTimeDs * 6);
                player->nextVolDownRate = (f32) (action->volume << 4) / (f32) (action->fadeTimeDs * 6);
            } else {
                player->nextVolUpRate = 0;
                player->nextVolDownRate = 0;
            }
            if (sp82 != -1) {
                sSeqPlayers[sp82]->currentSeqID = 0;
                sSeqPlayers[sp82]->targetVolume = 0;
                if (sSeqPlayers[sp82]->volDownRate == 0) {
                    sSeqPlayers[sp82]->volDownRate = sSeqPlayers[sp82]->volume / 6.0f;
                }
            }
        } else {
            player = sSeqPlayers[playerNo];
            player->targetVolume = action->volume << 4;
            player->nextBPM = action->bpm;
            temp_s6 = (action->unk18 | action->unk1A);
            player->unk194 &= ~temp_s6;
            player->unk196 &= ~temp_s6;
            player->unk198 &= ~temp_s6;
            player->unk194 |= action->unk16;
            player->unk196 |= action->unk18;
            player->unk198 |= action->unk1A;
            amseq_determine_vols(0, player->seqp.maxChannels, action->unk18, action->unk1A, action->unk16, channelVolumes);
            for (chan = 0, temp_s6 = 1; chan < player->seqp.maxChannels; chan++, temp_s6 <<= 1) {
                switch (channelVolumes[chan]) {
                    case -1: // mute?
                        func_80075E38(&player->seqp, chan);
                        player->channelVolumes[chan] = 0;
                        break;
                    case -2: // leave?
                        break;
                    case -3: // same?
                        channelVolumes[chan] = a1;
                        /* fallthrough */
                    default: // change?
                        func_80075E9C(&player->seqp, chan);
                        n_alCSPSetAmSeqChlVol(&player->seqp, chan, (u8) channelVolumes[chan]);
                        player->channelVolumes[chan] = (s16) (channelVolumes[chan] << 4);
                        break;
                }
            }
            if (action->fadeTimeDs != 0) {
                player->volUpRate = (u8) ((f32) player->volume / (f32) (action->fadeTimeDs * 6));
                player->volDownRate = (u8) ((f32) player->volume / (f32) (action->fadeTimeDs * 6));
            } else {
                player->volUpRate = 0;
                player->volDownRate = 0;
            }
        }
    } else {
        // Stop player
        switch (action->playerNo) {
        case 0:
            playerNo = _data_0;
            break;
        case 1:
            playerNo = 1 - _data_0;
            break;
        case 2:
            playerNo = _data_4 + 2;
            break;
        case 3:
            playerNo = 3 - _data_4;
            break;
        }
        player = sSeqPlayers[playerNo];
        player->currentSeqID = 0;
        player->unk0 &= ~0x1;
        n_alCSPStop(&player->seqp);
    }

    return playerNo;
}

// offset: 0x26D0 | func: 42
void amseq_update_dist_falloff(void) {
    Camera* camera;
    MusicAction* musicAction;
    Object* focus;
    f32 fvar3;
    f32 fvar1;
    f32 fvar2;
    s32 dist;
    s32 i;
    f32 focusX;
    f32 focusY;
    f32 focusZ;
    s8 playerNo;
    Object* obj;
    s8 sp78[4];
    Trigger_Data* triggerData;
    s8 handleIndices[3];
    s32 closestDists[3];

    focus = sFocusObj;
    if (focus != NULL) {
        if (focus->unkB0 & 0x40) {
            // "amSeq: focus object deleted %08x,%d\n"
            sFocusObj = NULL;
            focus = NULL;
        } else {
            focusX = focus->positionMirror.f[0];
            focusY = focus->positionMirror.f[1];
            focusZ = focus->positionMirror.f[2];
        }
    }

    if (focus == NULL) {
        camera = get_camera_array();
        update_camera_for_object(camera);
        focusX = camera->tx;
        focusY = camera->ty;
        focusZ = camera->tz;
    }

    for (i = 0; i < 4; i++) {
        sp78[i] = -1;
    }

    for (i = 0; i < 3; i++) {
        handleIndices[i] = -1;
        closestDists[i] = 0xFFFF;
    }

    for (i = 0; i < sNumSeqHandles; i++) {
        obj = sSeqHandles[i].obj;
        if (obj->unkB0 & 0x40) {
            // "amSeq: seq object deleted\n"
            sSeqHandles[i].obj = NULL;
            sNumSeqHandles--;
            bcopy(&sSeqHandles[sNumSeqHandles], &sSeqHandles[i], sizeof(AMSEQHandle));
            i = 0;
        }
    }
    
    // Find the closest handle for each player
    for (i = 0; i < sNumSeqHandles; i++) {
        sSeqHandles[i].unk28 = sSeqHandles[i].unk26;
        obj = sSeqHandles[i].obj;
        if ((obj->setup != NULL) && (obj->setup->objId == OBJ_TriggerPoint)) {
            triggerData = obj->data;
            fvar1 = triggerData->activatorCurrPos.x - focusX;
            fvar2 = triggerData->activatorCurrPos.y - focusY;
            fvar3 = triggerData->activatorCurrPos.z - focusZ;
        } else {
            fvar1 = obj->srt.transl.x - focusX;
            fvar2 = obj->srt.transl.y - focusY;
            fvar3 = obj->srt.transl.z - focusZ;
        }
        if (sSeqHandles[i].action.unk3 == 1) {
            fvar2 = 0.0f; // yDiff = 0
        } else if (sSeqHandles[i].action.unk3 == 2) {
            fvar3 = 0.0f; // zDiff = 0
            fvar1 = 0.0f; // xDiff = 0
        }
        dist = sqrtf(SQ(fvar1) + SQ(fvar2) + SQ(fvar3));
        playerNo = sSeqHandles[i].action.playerNo;
        if (dist < closestDists[playerNo]) {
            handleIndices[playerNo] = i;
            closestDists[playerNo] = dist;
        }
        sSeqHandles[i].unk26 = dist;
    }

    for (i = 0; i < 3; i++) {
        if (handleIndices[i] == -1) {
            continue;
        }

        musicAction = &sSeqHandles[handleIndices[i]].action;
        if (closestDists[i] < musicAction->distFalloffStart) {
            fvar2 = 127.0f; // full volume
        } else if (musicAction->distFalloffEnd >= closestDists[i]) {
            fvar1 = (closestDists[i] - musicAction->distFalloffStart);
            fvar1 /= (musicAction->distFalloffEnd - musicAction->distFalloffStart);
            if (musicAction->distFalloffExp == 2) {
                fvar1 *= fvar1;
            }
            fvar1 = 1.0f - fvar1;
            fvar2 = 127.0f * fvar1;
        } else {
            continue;
        }
        amseq_func_1E8C(musicAction, fvar2, -1);
    }
}

// TODO: "c:%04x f:%04x d:%04x mask:%04x\n"
//       cut?    fade?  ??     combination of those?

// offset: 0x2AE0 | func: 43                             dirty     nofade?   enabled
static void amseq_determine_vols(u8 arg0, u8 maxChannels, u16 arg2, u16 arg3, u16 arg4, s8* volumes) {
    u16 chan;
    u16 chanMask;
    s8 new_var;

    // "determineVols %d %04x %04x %04x\n"

    // where do these go?
    // "leave %d\n"
    // "mute %d [0]\n"
    // "same %d\n"
    // "change %d [%d]\n"

    chan = 0;
    chanMask = 1;
    while (chan < maxChannels) {
        if (!((arg2 | arg3) & chanMask)) {
            if (arg0 != 0) {
                volumes[chan] = 127;
            } else {
                volumes[chan] = -2;
            }
        } else if ((arg2 & chanMask) && (arg3 & chanMask)) {
            if (arg0 != 0) {
                volumes[chan] = 0;
            } else {
                volumes[chan] = -3;
            }
        } else if (arg3 & chanMask) {
            if (arg4 & chanMask) {
                volumes[chan] = 127;
            } else {
                volumes[chan] = -1;
            }
        } else if (arg2 & chanMask) {
            if ((arg4 & chanMask) && (arg0 != 0)) {
                volumes[chan] = 0;
            } else {
                volumes[chan] = -2;
            }
        } else {
            volumes[chan] = -2;
        }
        
        // @fake ---
        new_var = volumes[chan];
        if (new_var != -3) {
            if ((new_var && new_var) && new_var){}
        }
        // @fake ---
        
        chan++;
        chanMask <<= 1;
    }
}

// offset: 0x2C44 | func: 44
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/amseq_initOscMain.s")
#else
// https://decomp.me/scratch/Kmu7i
ALMicroTime amseq_initOscMain(void** oscState, f32* initVal, u8 oscType, u8 oscRate, u8 oscDepth, u8 oscDelay, u8 oscRiseTime) {
    struct oscstate* state;
    f32 oscDepthf;

    if (oscRate > 99) {
        //STUBBED_PRINTF("amAudio: WARNING invalid osc rate, using default.\n");
        oscRate = 99;
    }
    if (oscDelay > 127) {
        //STUBBED_PRINTF("amAudio: WARNING invalid osc delay, using default.\n");
        oscDelay = 127;
    }
    if (oscRiseTime > 127) {
        //STUBBED_PRINTF("amAudio: WARNING invalid osc rise time, using default.\n");
        oscRiseTime = 127;
    }
    if (sFreeOscStateList == NULL) {
        //STUBBED_PRINTF("amAudio: WARNING no free oscstates.\n");
        return 0;
    }
    state = sFreeOscStateList;
    if (oscRiseTime == 0) {
        state->unk18 = 1.0f;
        state->unk1C = 0;
    } else {
        state->unk18 = 0;
        state->unk1C = 1.0f / (D_800967A0[oscRiseTime] / AL_USEC_PER_FRAME);
    }
    state->unk04 = oscType;
    state->unk14 = 0;
    state->unk16 = 1000000.0f / (f32)D_8009699C[oscRate] / AL_USEC_PER_FRAME;
    state->unk08 = AL_USEC_PER_FRAME;
    oscDepthf = (f32) oscDepth;
    if (oscType & 0x80) {
        oscDepthf = _depth2Cents(oscDepthf);
    }
    oscType &= ~0x80;
    switch (oscType & ~0x80) {
    case 2:
    case 3:
    case 4:
    case 5:
        state->unk0c = oscDepthf;
        if ((oscType) == 2) {
            state->unk10 = -oscDepthf;
        } else {
            state->unk10 = 0;
        }
        state->unk08 = 500000.0f / (f32)D_8009699C[oscRate];
        break;
    case 6:
    case 8:
    case 11:
    case 12:
        state->unk10 = 0;
        state->unk0c = oscDepthf;
        break;
    case 7:
    case 9:
    case 13:
        state->unk10 = oscDepthf * 0.5f;
        state->unk0c = oscDepthf * 0.5f;
        break;
    case 10:
        state->unk10 = -oscDepthf;
        state->unk0c = oscDepthf * 2;
        break;
    default:
        //STUBBED_PRINTF(("amAudio: unsupported vibrato/tremolo type,%d\n", var_v0_2->unk04));
        return 0;
    }

    if (state->unk04 & 0x80) {
        *initVal = alCents2Ratio(state->unk10);
    } else {
        *initVal = state->unk10 + 127.0f;
    }
    *oscState = state;
    sFreeOscStateList = sFreeOscStateList->next;
    if (oscDelay) {
        return D_800967A0[oscDelay];
    }
    
    return AL_USEC_PER_FRAME;
}
#endif

// offset: 0x3038 | func: 45
ALMicroTime amseq_updateOscMain(void *oscState_, f32* updateVal) {
    f32 var_fa0;
    f32 sp28;
    f32 sp24;
    struct oscstate *oscState = oscState_;

    if ((oscState->unk04 & ~0x80) >= 6) {
        oscState->unk14++;
        if (oscState->unk14 >= oscState->unk16) {
            oscState->unk14 = 0;
        }
        sp28 = (f32)oscState->unk14 / (f32)oscState->unk16;
    }
    if (oscState->unk1C != 0.0f) {
        oscState->unk18 += oscState->unk1C;
        if (oscState->unk18 >= 1.0f) {
            oscState->unk18 = 1.0f;
            oscState->unk1C = 0;
        }
    }
    var_fa0 = oscState->unk0c;
    if (oscState->unk18 != 1.0f) {
        var_fa0 *= oscState->unk18;
    }
    switch (oscState->unk04 & ~0x80) {
    case 2:
    case 3:
    case 4:
    case 5:
        if (oscState->unk14) {
            sp28 = var_fa0;
        } else {
            sp28 = oscState->unk10;
        }
        oscState->unk14 ^= 1;
        break;
    case 6:
    case 7:
        if (sp28 < 0.25f) {
           sp28 *= (4.0f * var_fa0);
        } else if (sp28 >= 0.75f) {
            sp28 -= 0.75f;
            sp28 *= 4.0f * var_fa0;
            sp28 -= var_fa0;
        } else {
            sp28 -= 0.25f;
            sp28 *= 4.0f * var_fa0;
            sp28 = var_fa0 - sp28;
        }
        sp28 += oscState->unk10;
        break;
    case 8:
    case 9:
        sp28 = (fsin16_precise(sp28 * 65536.0f) * var_fa0) + oscState->unk10;
        break;
    case 10:
    case 11:
        sp28 *= var_fa0;
        sp28 += oscState->unk10;
        break;
    case 12:
    case 13:
        if (sp28 < 0.25f) {
            sp28 *= (4.0f * var_fa0);
        } else if (sp28 >= 0.75f) {
            sp28 -= 0.75f;
            sp28 *= 4.0f * var_fa0;
            sp28 -= var_fa0;
        } else {
            sp28 -= 0.25f;
            sp28 *= 4.0f * var_fa0;
            sp28 = var_fa0 - sp28;
        }
        sp24 = oscState->unk10 + sp28;
        sp28 = (f32)oscState->unk14 / (f32)oscState->unk16;
        sp28 = fsin16_precise(sp28 * 65536.0f) * var_fa0 + oscState->unk10;
        sp28 += sp24;
        sp28 /= 2.0f;
        break;
    }
    if (oscState->unk04 & 0x80) {
        *updateVal = alCents2Ratio((s32) sp28);
    } else {
        *updateVal = sp28 + 127.0f;
    }
    return oscState->unk08;
}

// offset: 0x3434 | func: 46
void amseq_stopOscMain(void *oscState) {
    ((struct oscstate *)oscState)->next = (struct oscstate *)sFreeOscStateList;
    sFreeOscStateList = (struct oscstate *)oscState;
}

// offset: 0x345C | func: 47 | export: 17
void amseq_func_345C(Object *obj) {

}

/*0x0*/ static const char str_0[] = "music %08x,%d\n";
/*0x10*/ static const char str_10[] = "object+fade\n";
/*0x20*/ static const char str_20[] = "AUDIO: Maximum sequence handles reached.\n";
/*0x4C*/ static const char str_4C[] = "registered %d,%08x\n";
/*0x60*/ static const char str_60[] = "already registered %d,%08x\n";
/*0x7C*/ static const char str_7C[] = "starting non-fade channels\n";
/*0x98*/ static const char str_98[] = "starting static sequence\n";
/*0xB4*/ static const char str_B4[] = "amSeqPlayEx: Warning, player value '%d' out of range.\n";
/*0xEC*/ static const char str_EC[] = "amSeqPlayEx: Warning, sequence value '%d' out of range.\n";
/*0x128*/ static const char str_128[] = "amSeqStop: Warning, player value '%d' out of range.\n";
/*0x160*/ static const char str_160[] = "amSeqGetNo: Warning, player value '%d' out of range.\n";
/*0x198*/ static const char str_198[] = "amSeqGetVolume: Warning, player value '%d' out of range.\n";
/*0x1D4*/ static const char str_1D4[] = "amSeqSetVolume: Warning, player value '%d' out of range.\n";
/*0x210*/ static const char str_210[] = "startSequencer %d\n";
/*0x224*/ static const char str_224[] = "download sequence %d:%d\n";
/*0x240*/ static const char str_240[] = "AUDIO: MIDI sequence buffer failed.\n";
/*0x268*/ static const char str_268[] = "volumes %d,%d %d\n";
/*0x27C*/ static const char str_27C[] = "start c:%04x f:%04x d:%04x mask:%04x\n";
/*0x2A4*/ static const char str_2A4[] = "doaction %08x,%d\n";
/*0x2B8*/ static const char str_2B8[] = "play %d on player %d\n";
/*0x2D0*/ static const char str_2D0[] = "AMSEQ: player out of range\n";
/*0x2EC*/ static const char str_2EC[] = "not already playing/cut set\n";
/*0x30C*/ static const char str_30C[] = "cut %04x player %d otherplayer %d\n";
/*0x330*/ static const char str_330[] = "c:%04x f:%04x d:%04x mask:%04x\n";
/*0x350*/ static const char str_350[] = "fade out %d\n";
/*0x360*/ static const char str_360[] = "already on player %d\n";
/*0x378*/ static const char str_378[] = "already c:%04x f:%04x d:%04x mask:%04x\n";
/*0x3A0*/ static const char str_3A0[] = "stop playing player %d\n";
/*0x3B8*/ static const char str_3B8[] = "amSeq: focus object deleted %08x,%d\n";
/*0x3E0*/ static const char str_3E0[] = "amSeq: seq object deleted\n";
/*0x3FC*/ static const char str_3FC[] = "determineVols %d %04x %04x %04x\n";
/*0x420*/ static const char str_420[] = "leave %d\n";
/*0x42C*/ static const char str_42C[] = "mute %d [0]\n";
/*0x43C*/ static const char str_43C[] = "same %d\n";
/*0x448*/ static const char str_448[] = "change %d [%d]\n";
/*0x458*/ static const char str_458[] = "amAudio: WARNING invalid osc rate, using default.\n";
/*0x48C*/ static const char str_48C[] = "amAudio: WARNING invalid osc delay, using default.\n";
/*0x4C0*/ static const char str_4C0[] = "amAudio: WARNING invalid osc rise time, using default.\n";
/*0x4F8*/ static const char str_4F8[] = "amAudio: WARNING no free oscstates.\n";
/*0x520*/ static const char str_520[] = "amAudio: unsupported vibrato/tremolo type, %d\n";
