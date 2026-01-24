#include "common.h"
#include "dlls/objects/325_trigger.h"
#include "libnaudio/n_libaudio.h"
#include "sys/audio.h"

extern s32 D_800967A0[127];
extern f32 D_8009699C[100];

// size:0x24C
typedef struct {
    u8 unk0;
    N_ALCSPlayer seqp;
    ALCSeq unk90;
    void *unk188;
    u8 unk18C;
    s16 unk18E;
    s16 unk190;
    u16 unk192;
    u16 unk194;
    u16 unk196;
    u16 unk198;
    u8 unk19A;
    u8 unk19B;
    s16 unk19C[16];
    u8 _unk1BC[0x1FC - 0x1BC];
    u8 unk1FC;
    s16 unk1FE;
    u16 unk200;
    s16 unk202;
    u16 unk204;
    u16 unk206;
    u16 unk208;
    u8 unk20A;
    u8 unk20B;
    u8 _unk20C[0x24C - 0x20C];
} AMSEQBss0;

typedef struct {
    s32 unk0;
    u8 _unk4[4];
} AMSEQBss18; 

typedef struct MusicAction {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u16 unk4;
    u16 unk6;
    u32 unk8;
    u32 unkC;
    u8 unk10; // amseq channel? 0-3?
    u8 unk11; // music/ambient ID
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u16 unk16;
    u16 unk18;
    u16 unk1A;
    u32 unk1C;
} MusicAction;

typedef struct {
    Object *unk0;
    MusicAction unk4;
    s16 unk24;
    s16 unk26;
    u16 unk28;
} AMSEQBss20; 

/*0x0*/ static u8 _data_0 = 0;
/*0x4*/ static u8 _data_4 = 0;
/*0x8*/ static Object *_data_8 = NULL;
/*0xC*/ static MusicAction* _data_C = NULL;

/*0x0*/ static AMSEQBss0 **_bss_0; // pointer to array of 4 pointers
/*0x4*/ static ALBankFile **_bss_4;
/*0x8*/ static ALSeqFile *_bss_8[2];
/*0x10*/ static struct oscstate *_bss_10;
/*0x14*/ static struct oscstate *_bss_14;
/*0x18*/ static AMSEQBss18 *_bss_18;
/*0x1C*/ static s32 _bss_1C;
/*0x20*/ static AMSEQBss20 *_bss_20;
/*0x24*/ static s32 _bss_24;
/*0x28*/ static u16 _bss_28;
/*0x2A*/ static u16 _bss_2A;
/*0x2C*/ static u16 _bss_2C;

void dll_5_func_CC8(u8 arg0, u8 arg1, s16 arg2, s16 arg3, u16 arg4);
/*static*/ void dll_5_func_26D0(void);
/*static*/ void dll_5_func_1514(u8 arg0);
/*static*/ void dll_5_func_1924(u8 arg0);
static void dll_5_func_2AE0(u8 arg0, u8 arg1, u16 arg2, u16 arg3, u16 arg4, s8* arg5);
static void dll_5_func_14A0(AMSEQBss0 *arg0);
/*static*/ AMSEQBss0* dll_5_func_135C(s32 arg0, s32 maxChannels, s32 maxVoices, s32 arg3, s32 maxEvents);
/*static*/ ALMicroTime dll_5_func_2C44(void **oscState,f32 *initVal, u8 oscType,
                                   u8 oscRate, u8 oscDepth, u8 oscDelay, u8 unk07);
/*static*/ ALMicroTime dll_5_func_3038(void *oscState, f32 *updateVal);
/*static*/ void dll_5_func_3434(void *oscState);
/*static*/ s32 dll_5_func_1E8C(MusicAction *a0, s8 a1, s16 a2);

// offset: 0x0 | ctor
#ifndef NON_MATCHING
void dll_5_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_ctor.s")
#else
// https://decomp.me/scratch/aRlW8
void dll_5_ctor(void* dll) {
    s32* sp4C;
    s32 sp48;
    s32 sp44;
    u32 sp40;

    sp4C = NULL;
    _bss_0 = mmAlloc(sizeof(AMSEQBss0*) * 4, ALLOC_TAG_SEQ_COL, NULL);
    bzero(_bss_0, sizeof(AMSEQBss0*) * 4);
    _bss_4 = mmAlloc(sizeof(ALBankFile*) * 2, ALLOC_TAG_SEQ_COL, NULL);
    bzero(_bss_4, sizeof(ALBankFile*) * 2);
    queue_alloc_load_file((void**)&sp4C, AMBIENT_TAB);
    sp44 = sp4C[0];
    sp48 = sp4C[1] - sp44;
    if (sp48 != 0) {
        _bss_4[0] = mmAlloc(sp48, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void**)_bss_4[0], AMBIENT_BIN, sp44, sp48);
        alBnkfNew(_bss_4[0], (u8*)file_get_romaddr(AMBIENT_BIN, sp4C[1]));
    }
    mmFree(sp4C);
    queue_alloc_load_file((void**)&sp4C, MUSIC_TAB);
    sp44 = sp4C[0];
    sp48 = sp4C[1] - sp44;
    if (sp48 != 0) {
        _bss_4[1] = mmAlloc(sp48, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void**)_bss_4[1], MUSIC_BIN, sp44, sp48);
        alBnkfNew(_bss_4[1], (u8*)file_get_romaddr(MUSIC_BIN, sp4C[1]));
    }
    mmFree(sp4C);
    queue_alloc_load_file((void**)&sp4C, AUDIO_TAB);
    sp44 = sp4C[2];
    if (sp4C[3] != sp44) {
        _bss_8[0] = mmAlloc(4, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void** ) _bss_8[0], AUDIO_BIN, sp44, 4);
        sp48 = (_bss_8[0]->seqCount * 8) + 4;
        mmFree(_bss_8[0]);
        _bss_8[0] = mmAlloc(sp48, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void** ) _bss_8[0], AUDIO_BIN, sp44, sp48);
        alSeqFileNew(_bss_8[0], (u8*)file_get_romaddr(AUDIO_BIN, sp44));
        for (sp40 = 0; sp40 < _bss_8[0]->seqCount; sp40++) {
            if (_bss_8[0]->seqArray[sp40].len & 1) {
                _bss_8[0]->seqArray[sp40].len++;
            }
        }
    }
    sp44 = sp4C[1];
    if (sp4C[2] != sp44) {
        _bss_8[1] = mmAlloc(4, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void** ) _bss_8[1], AUDIO_BIN, sp44, 4);
        sp48 = (_bss_8[1]->seqCount * 8) + 4;
        mmFree(_bss_8[1]);
        _bss_8[1] = mmAlloc(sp48, ALLOC_TAG_SEQ_COL, NULL);
        queue_load_file_region_to_ptr((void**)_bss_8[1], AUDIO_BIN, sp44, sp48);
        alSeqFileNew(_bss_8[1], (u8*)file_get_romaddr(AUDIO_BIN, sp44));
        for (sp40 = 0; sp40 < _bss_8[1]->seqCount; sp40++) {
            if (_bss_8[1]->seqArray[sp40].len & 1) {
                _bss_8[1]->seqArray[sp40].len++;
            }
        }
    }
    mmFree(sp4C);
    _bss_10 = _bss_14 = mmAlloc(sizeof(struct oscstate) * 32, ALLOC_TAG_SEQ_COL, NULL);
    for (sp40 = 0; sp40 < 31; sp40++) {
        _bss_10[sp40].unk00 = &_bss_10[sp40 + 1];
    }
    _bss_10[31].unk00 = NULL;
    _bss_0[0] = dll_5_func_135C(0, 8, 8, 8, 100);
    _bss_0[1] = dll_5_func_135C(0, 8, 8, 8, 100);
    _bss_0[2] = dll_5_func_135C(1, 16, 32, 32, 120);
    _bss_0[3] = dll_5_func_135C(1, 16, 32, 32, 120);
    _data_C = mmAlloc(0x20, ALLOC_TAG_SEQ_COL, NULL);
    _bss_1C = 0x10;
    _bss_18 = mmAlloc(_bss_1C * 8, ALLOC_TAG_SEQ_COL, NULL);
    while (_bss_1C != 0) {
        _bss_18[_bss_1C - 1].unk0 = -1;
        _bss_1C--;
    }
    sp40 = 0x2C; // @fake
    _bss_24 = 0x10;
    _bss_20 = mmAlloc(_bss_24 * sp40, ALLOC_TAG_SEQ_COL, NULL);
    while (_bss_24 > 0) {
        _bss_20[_bss_24 - 1].unk0 = 0;
        _bss_24--;
    }
    _bss_28 = _bss_2A = 0x100;
    _bss_2C = (_bss_28 * _bss_2A) >> 8;
}
#endif

// offset: 0x7E8 | dtor
void dll_5_dtor(void* dll) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (_bss_4[i] != NULL) {
            mmFree(_bss_4[i]);
        }
    }
    mmFree(_bss_4);
    mmFree(_bss_8[0]);
    mmFree(_bss_8[1]);
    mmFree(_bss_10);
    for (i = 0; i < 4; i++) {
        dll_5_func_14A0(_bss_0[i]);
    }
    mmFree(_bss_0);
    mmFree(_bss_18);
    mmFree(_bss_20);
}

// offset: 0x930 | func: 0 | export: 0
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_930.s")
#else
s32 dll_5_func_930(Object* arg0, u16 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_v1;
    s32 var_v1;

    if (arg1 == 0) {
        return -1;
    }
    queue_load_file_region_to_ptr((void** ) _data_C, MUSICACTIONS_BIN, (arg1 - 1) * 0x20, 0x20);
    if (arg0 != NULL) {
        temp_v1 = _data_C->unk18 & _data_C->unk1A;
        if (temp_v1 != 0) {
            if (temp_v1 != 0) {
                for (var_v1 = 0; var_v1 < _bss_24; var_v1++) {
                    if (_bss_20[var_v1].unk0 == arg0) {
                        break;
                    }
                }
                if (var_v1 == _bss_24) {
                    _bss_24 += 1;
                    _bss_20[var_v1].unk24 = arg1;
                    _bss_20[var_v1].unk0 = arg0;
                    _bss_20[var_v1].unk28 = -1;
                    _bss_20[var_v1].unk26 = _bss_20[var_v1].unk28;
                    bcopy(_data_C, &_bss_20[var_v1].unk4, 0x20);
                }
                if (temp_v1 != 0xFFFF) {
                    return dll_5_func_1E8C(_data_C, 0, arg1);
                }
                //goto block_14;
            }
//block_14:
            return -1;
        }
    }
    return dll_5_func_1E8C(_data_C, 0, arg1);
}
#endif

// offset: 0xB0C | func: 1 | export: 1
void dll_5_func_B0C(Object *arg0, u16 arg1, s32 arg2, s32 arg3) {
    s32 i;

    if (_bss_24 > 0) {
        for (i = 0; i < _bss_24; i++) {
            if ((arg0 == _bss_20[i].unk0) && ((arg1 == 0xFFFF) || (arg1 == _bss_20[i].unk24))) {
                break;
            }
        }
        if (i != _bss_24) {
            if (_bss_20[i].unk0 != NULL) {
                _bss_24--;
                bcopy(&_bss_20[_bss_24], &_bss_20[i], 0x2C);
            }
        }
    }
}

// offset: 0xC00 | func: 2 | export: 2
s32 dll_5_func_C00(Object *arg0, u16 arg1) {
    s32 i;

    for (i = 0; i < _bss_24; i++) {
        if ((arg0 == _bss_20[i].unk0) && (arg1 == _bss_20[i].unk24)) {
            return 1;
        }
    }

    return 0;
}

// offset: 0xC6C | func: 3 | export: 4
void dll_5_func_C6C(u8 arg0, u8 arg1) {
    dll_5_func_CC8(arg0, arg1, 0x64, 0x7F, 0xFFFF);
}

// offset: 0xCC8 | func: 4 | export: 5
void dll_5_func_CC8(u8 arg0, u8 arg1, s16 arg2, s16 arg3, u16 arg4) {
    if ((arg1 != 0) && (arg0 < 4)) {
        if (_bss_8[arg0 >> 1]->seqCount < arg1) {
            *((volatile u8*)NULL) = 0;
            return;
        }
        if (!(_bss_0[arg0]->unk0 & 2) && (_bss_2C != 0)) {
            _bss_0[arg0]->unk1FC = arg1;
            _bss_0[arg0]->unk1FE = arg2;
            _bss_0[arg0]->unk202 = arg3 * 0x10;
            _bss_0[arg0]->unk200 = arg4;
            _bss_0[arg0]->unk204 = 0;
            _bss_0[arg0]->unk206 = 0;
            _bss_0[arg0]->unk208 = 0;
            _bss_0[arg0]->unk20A = 0;
            _bss_0[arg0]->unk20B = 0;
        }
    }
}

// offset: 0xE14 | func: 5 | export: 6
void dll_5_func_E14(u8 arg0) {
    if (arg0 < 4) {
        if (!(_bss_0[arg0]->unk0 & 2) && (_bss_0[arg0]->unk0 & 1)) {
            n_alCSPStop(&_bss_0[arg0]->seqp);
            _bss_0[arg0]->unk1FC = 0;
            _bss_0[arg0]->unk18C = 0;
            _bss_0[arg0]->unk0 &= ~1;
        }
    }
}

// offset: 0xEE4 | func: 6 | export: 7
u8 dll_5_func_EE4(u8 arg0) {
    AMSEQBss0* temp_v0;

    if (arg0 >= 4) {
        return 0;
    }
    temp_v0 = _bss_0[arg0];
    if (temp_v0->unk1FC != 0) {
        return temp_v0->unk1FC;
    }
    if (temp_v0->unk18C != 0) {
        return temp_v0->unk18C;
    }
    return 0;
}

// offset: 0xF58 | func: 7 | export: 8
u16 dll_5_func_F58(void) {
    return _bss_28;
}

// offset: 0xF74 | func: 8 | export: 9
void dll_5_func_F74(u32 volume) {
    AMSEQBss0* temp_v0;

    if (volume > 256) {
        volume = 256;
    }
    _bss_28 = volume;
    _bss_2C = (_bss_28 * _bss_2A) >> 8;
    temp_v0 = _bss_0[0];
    n_alCSPSetVol(&temp_v0->seqp, (s16) ((temp_v0->unk190 >> 4) * _bss_2C));
    temp_v0 = _bss_0[1];
    n_alCSPSetVol(&temp_v0->seqp, (s16) ((temp_v0->unk190 >> 4) * _bss_2C));
    temp_v0 = _bss_0[2];
    n_alCSPSetVol(&temp_v0->seqp, (s16) ((temp_v0->unk190 >> 4) * _bss_2C));
    // @bug: Doesn't affect the 4th music player
}

// offset: 0x1090 | func: 9 | export: 10
UNK_TYPE_32 dll_5_func_1090(void) {
    return 0;
}

// offset: 0x109C | func: 10 | export: 11
void dll_5_func_109C(UNK_TYPE_32 arg0) {

}

// offset: 0x10A8 | func: 11 | export: 12
u8 dll_5_func_10A8(u8 arg0) {
    if (arg0 >= 4) {
        return 0;
    }
    return _bss_0[arg0]->unk190 >> 4;
}

// offset: 0x10FC | func: 12 | export: 13
void dll_5_func_10FC(u8 arg0, u8 arg1) {
    if (arg0 < 4) {
        _bss_0[arg0]->unk202 = arg1 * 16;
    }
}

// offset: 0x1150 | func: 13 | export: 14
UNK_TYPE_32 dll_5_func_1150(UNK_TYPE_32 arg0) {
    return 0;
}

// offset: 0x1160 | func: 14 | export: 15
void dll_5_func_1160(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x1170 | func: 15 | export: 16
void dll_5_func_1170(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x1180 | func: 16 | export: 18
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1180.s")
#else
// needs dll_5_func_1924, dll_5_func_1514, dll_5_func_26D0 to be static
void dll_5_func_1180(void) {
    s32 i;

    if (audio_func_80012348() != 0) {
        for (i = 0; i < 4; i++) {
            dll_5_func_1514(i);
            dll_5_func_1924(i);
        }
        dll_5_func_26D0();
    }
}
#endif

// offset: 0x1228 | func: 17 | export: 3
void dll_5_func_1228(Object *arg0) {
    _data_8 = arg0;
}

// offset: 0x1244 | func: 18 | export: 19
void dll_5_func_1244(UNK_TYPE_32 arg0) {

}

// offset: 0x1250 | func: 19 | export: 20
void dll_5_func_1250(UNK_TYPE_32 arg0) {

}

// offset: 0x125C | func: 20 | export: 21
void dll_5_func_125C(UNK_TYPE_32 arg0) {

}

// offset: 0x1268 | func: 21 | export: 22
void dll_5_func_1268(UNK_TYPE_32 arg0) {

}

// offset: 0x1274 | func: 22 | export: 23
void dll_5_func_1274(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x1284 | func: 23 | export: 24
void dll_5_func_1284(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x1294 | func: 24 | export: 25
void dll_5_func_1294(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x12A4 | func: 25 | export: 26
UNK_TYPE_32 dll_5_func_12A4(UNK_TYPE_32 arg0) {
    return 0;
}

// offset: 0x12B4 | func: 26 | export: 27
void dll_5_func_12B4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x12C4 | func: 27 | export: 28
void dll_5_func_12C4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x12D4 | func: 28 | export: 29
void dll_5_func_12D4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {

}

// offset: 0x12E4 | func: 29 | export: 30
UNK_TYPE_32 dll_5_func_12E4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {
    return 0;
}

// offset: 0x12F8 | func: 30 | export: 31
UNK_TYPE_32 dll_5_func_12F8(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {
    return 0x7F;
}

// offset: 0x130C | func: 31 | export: 32
void dll_5_func_130C(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {

}

// offset: 0x1320 | func: 32 | export: 33
UNK_TYPE_32 dll_5_func_1320(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {
    return 0;
}

// offset: 0x1334 | func: 33 | export: 34
void dll_5_func_1334(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {

}

// offset: 0x1348 | func: 34 | export: 35
void dll_5_func_1348(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {

}

// offset: 0x135C | func: 35
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_135C.s")
#else
// needs dll_5_func_2C44, dll_5_func_3038, dll_5_func_3434 to be static
AMSEQBss0* dll_5_func_135C(s32 arg0, s32 maxChannels, s32 maxVoices, s32 arg3, s32 maxEvents) {
    AMSEQBss0* temp_v0;
    ALSeqpConfig config;
    s32 i;

    temp_v0 = mmAlloc(sizeof(AMSEQBss0), ALLOC_TAG_SEQ_COL, NULL);
    bzero(temp_v0, sizeof(AMSEQBss0));
    config.heap = NULL;
    config.maxVoices = maxVoices;
    config.maxEvents = maxEvents;
    config.unk1C = arg3;
    config.maxChannels = maxChannels;
    config.initOsc = &dll_5_func_2C44;
    config.updateOsc = &dll_5_func_3038;
    config.stopOsc = &dll_5_func_3434;
    n_alCSPNew(&temp_v0->seqp, &config);
    n_alCSPSetBank(&temp_v0->seqp, (ALBank*)_bss_4[arg0]->bankArray);
    temp_v0->unk188 = 0;
    temp_v0->unk18C = 0;
    temp_v0->seqp.chanMask = 0xFFFF;
    for (i = 0; i < temp_v0->seqp.maxChannels; i++) {
        temp_v0->unk19C[i] = 0x7F;
    }
    return temp_v0;
}
#endif

// offset: 0x14A0 | func: 36
static void dll_5_func_14A0(AMSEQBss0 *arg0) {
    n_alCSPDelete(&arg0->seqp);
    if (arg0->unk188 != NULL) {
        mmFree(arg0->unk188);
    }
    mmFree(arg0);
}

// offset: 0x1514 | func: 37
void dll_5_func_1514(u8 arg0) {
    AMSEQBss0* temp_s4;
    N_ALCSPlayer* seqp;
    s32 var_s2;
    s32 temp_s5;
    s32 var_s0;
    s8 sp64[16]; // has to be at least 16
    s32 temp;

    temp_s4 = _bss_0[arg0];
    if (temp_s4->unk1FC == 0) {
        return;
    }
    seqp = &temp_s4->seqp;
    if (n_alCSPGetState(seqp) != 0) {
        n_alCSPStop(seqp);
        temp_s4->unk18C = 0;
        temp_s4->unk0 &= ~0x1;
        return;
    }
    if (((arg0 == 2) || (arg0 == 3)) && (main_get_bits(BIT_549) != 0)) {
        temp_s4->unk18C = 0;
        temp_s4->unk0 &= ~0x1;
        return;
    }
    temp_s4->unk18C = temp_s4->unk1FC;
    temp_s4->unk1FC = 0;
    if (arg0 >= 2) {
        var_s0 = 1;
    } else {
        var_s0 = 0;
    }
    var_s2 = temp_s4->unk18C - 1;
    temp_s5 = _bss_8[var_s0]->seqArray[var_s2].offset - (u8*)file_get_romaddr(AUDIO_BIN, 0);
    if (temp_s4->unk188 != NULL) {
        mmFree(temp_s4->unk188);
    }
    temp_s4->unk188 = mmAlloc(_bss_8[var_s0]->seqArray[var_s2].len, ALLOC_TAG_SEQ_COL, NULL);
    if (temp_s4->unk188 != NULL) {
        queue_load_file_region_to_ptr(temp_s4->unk188, AUDIO_BIN, temp_s5, _bss_8[var_s0]->seqArray[var_s2].len);
        n_alCSeqNew(&temp_s4->unk90, (u8* ) temp_s4->unk188);
        n_alCSPSetSeq(seqp, &temp_s4->unk90);
        n_alCSPPlay(seqp);
        temp_s4->unk18E = temp_s4->unk1FE;
        if (temp_s4->unk18E != 0) {
            n_alCSPSetTempo(seqp, (s32) ((1.0f / (f32) temp_s4->unk18E) * 6e7f));
        }
        temp_s4->unk19A = temp_s4->unk20A;
        temp_s4->unk19B = temp_s4->unk20B;
        if (temp_s4->unk19A) {
            temp_s4->unk190 = 0;
            temp_s4->unk202 = temp_s4->unk202;
        } else {
            temp_s4->unk190 = temp_s4->unk202 = temp_s4->unk202;
        }
        n_alCSPSetVol(seqp, (temp_s4->unk190 >> 4) * _bss_2C);
        temp_s4->unk192 = temp_s4->unk200;
        temp_s4->seqp.chanMask = temp_s4->unk192;
        temp_s4->unk194 = temp_s4->unk204;
        temp_s4->unk196 = temp_s4->unk206;
        temp_s4->unk198 = temp_s4->unk208;
        dll_5_func_2AE0(0, temp_s4->seqp.maxChannels, temp_s4->unk196, temp_s4->unk198, temp_s4->unk194, sp64);
        for (var_s2 = 0, temp = 1; var_s2 < temp_s4->seqp.maxChannels; var_s2++, temp <<= 1) {
            switch (sp64[var_s2]) {
                case -1:
                    func_80075E38(seqp, var_s2);
                    func_80075DCC(seqp, var_s2, 0);
                    temp_s4->unk19C[var_s2] = 0;
                    break;
                case -2:
                    break;
                case -3:
                    sp64[var_s2] = (s8) temp_s4->unk19C[var_s2];
                    /* fallthrough */
                default:
                    func_80075E9C(seqp, var_s2);
                    func_80075DCC(seqp, var_s2, sp64[var_s2]);
                    temp_s4->unk19C[var_s2] = sp64[var_s2] * 16;
                    break;
            }
        }
        temp_s4->unk0 |= 1;
    }
}

// offset: 0x1924 | func: 38
void dll_5_func_1924(u8 arg0) {
    AMSEQBss0* temp_s1;
    N_ALCSPlayer* seqp;
    s32 var_s0;
    s32 temp_s7;
    s32 var_s2;
    s32 var_s3;

    temp_s1 = _bss_0[arg0];
    seqp = &temp_s1->seqp;
    if (n_alCSPGetState(seqp) == 1) {
        if (temp_s1->unk1FC == 0) {
            if (temp_s1->unk202 != temp_s1->unk190) {
                if (temp_s1->unk19A == 0) {
                    temp_s1->unk190 = temp_s1->unk202;
                    if (temp_s1->unk202 == 0) {
                        n_alCSPStop(seqp);
                    }
                } else if (temp_s1->unk190 < temp_s1->unk202) {
                    temp_s1->unk190 += (temp_s1->unk19A * gUpdateRate);
                    if (temp_s1->unk202 < temp_s1->unk190) {
                        temp_s1->unk190 = temp_s1->unk202;
                    }
                } else {
                    temp_s1->unk190 -= (temp_s1->unk19B * gUpdateRate);
                    if (temp_s1->unk202 >= temp_s1->unk190) {
                        temp_s1->unk190 = temp_s1->unk202;
                        if (temp_s1->unk202 == 0) {
                            n_alCSPStop(seqp);
                        }
                    }
                }
                n_alCSPSetVol(seqp, (temp_s1->unk190 >> 4) * _bss_2C);
            }
        }
        temp_s7 = temp_s1->unk196 & ~temp_s1->unk198;
        if (temp_s7 != 0) {
            var_s3 = 0;
            var_s2 = 1;
            while (var_s3 < temp_s1->seqp.maxChannels) {
                if (temp_s7 & var_s2) {
                    var_s0 = temp_s1->unk19C[var_s3];
                    if (temp_s1->unk194 & var_s2) {
                        func_80075E9C(seqp, var_s3);
                        temp_s1->unk192 |= var_s2;
                        temp_s1->seqp.chanMask = temp_s1->unk192;
                        var_s0 += (temp_s1->unk19A * gUpdateRate);
                        if (var_s0 >= 0x7F0) {
                            var_s0 = 0x7F0;
                            temp_s1->unk196 &= ~var_s2;
                        }
                    } else {
                        var_s0 -= (temp_s1->unk19B * gUpdateRate);
                        if (var_s0 <= 0) {
                            var_s0 = 0;
                            temp_s1->unk196 &= ~var_s2;
                            func_80075E38(seqp, var_s3);
                            temp_s1->unk192 |= var_s2;
                            temp_s1->seqp.chanMask = temp_s1->unk192;
                        }
                    }
                    temp_s1->unk19C[var_s3] = var_s0;
                    func_80075DCC(seqp, var_s3, (var_s0 >> 4));
                }
                var_s3 += 1;
                var_s2 *= 2;
            }
        }
    }
}

// offset: 0x1C08 | func: 39
s32 dll_5_func_1C08(u8 arg0, s32 arg1, s8* arg2, s8* arg3) {
    switch (arg0) {
    case 0:
    case 1:
        if (arg1 == dll_5_func_EE4(0)) {
            *arg2 = 0;
            *arg3 = 1;
            return 1;
        }
        if (arg1 == dll_5_func_EE4(1)) {
            *arg2 = 1;
            *arg3 = -1;
            return 1;
        }
        break;
    case 2:
    case 3:
        if (arg1 == dll_5_func_EE4(2)) {
            *arg2 = 2;
            *arg3 = 3;
            return 1;
        }
        if (arg1 == dll_5_func_EE4(3)) {
            *arg2 = 3;
            *arg3 = -1;
            return 1;
        }
        break;
    }

    return 0;
}

// offset: 0x1D60 | func: 40
void dll_5_func_1D60(u8 arg0, s8* arg1, s8* arg2, u8 arg3) {
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
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1E8C.s")
#else
// https://decomp.me/scratch/TbDs7
s32 dll_5_func_1E8C(MusicAction* a0, s8 a1, s16 a2) {
    s8 sp83;
    s8 sp82;
    s8 sp64[16];
    AMSEQBss0* temp_s1;
    GplayStruct13* temp_v0;
    s32 temp_s6;
    s32 temp_v0_2;
    s32 var_s4;
    s32 temp_v1;
    s32 temp;

    temp_v0 = gDLL_29_Gplay->vtbl->func_FE8();
    sp83 = -1;
    sp82 = -1;
    if (a0->unk11 != 0) {
        temp_v0->unk0[a0->unk10] = a2;
        temp_v1 = a0->unk1A;
        temp_s6 = (temp_v1 & a0->unk16);
        if ((dll_5_func_1C08(a0->unk10, (s32) a0->unk11, &sp83, &sp82) == 0) || (temp_s6 != 0)) {
            if (temp_s6 == 0) {
                dll_5_func_1D60(a0->unk10, &sp83, &sp82, temp_v1);
            } else {
                if (sp83 == -1) {
                    dll_5_func_1D60(a0->unk10, &sp83, &sp82, temp_v1);
                }
                sp82 = -1;
            }
            temp_s1 = _bss_0[sp83];
            temp_s1->unk1FC = a0->unk11;
            temp_s1->unk202 = a0->unk12 * 0x10;
            temp_s1->unk1FE = (s16) a0->unk13;
            temp_v0_2 = (a0->unk18 | a0->unk1A);
            temp_s1->unk204 &= ~temp_v0_2;
            temp_s1->unk206 &= ~temp_v0_2;
            temp_s1->unk208 &= ~temp_v0_2;
            temp_s1->unk204 |= a0->unk16;
            temp_s1->unk206 |= a0->unk18;
            temp_s1->unk208 |= a0->unk1A;
            temp_s1->unk200 = 0xFFFF;
            temp_s1->unk200 &= a0->unk1A & ~a0->unk16;
            if (a0->unk15 != 0) {
                temp_s1->unk20A = (u8) (u32) ((f32) (a0->unk12 * 0x10) / (f32) (a0->unk15 * 6));
                temp_s1->unk20B = (u8) (u32) ((f32) (a0->unk12 * 0x10) / (f32) (a0->unk15 * 6));
            } else {
                temp_s1->unk20A = 0;
                temp_s1->unk20B = 0;
            }
            if (sp82 != -1) {
                _bss_0[sp82]->unk18C = 0;
                _bss_0[sp82]->unk202 = 0;
                if (_bss_0[sp82]->unk19B == 0) {
                    _bss_0[sp82]->unk19B = (u8) (u32) ((f32) _bss_0[sp82]->unk190 / 6.0f);
                }
            }
        } else {
            temp_s1 = _bss_0[sp83];
            temp_s1->unk202 = a0->unk12 * 0x10;
            temp_s1->unk1FE = (s16) a0->unk13;
            temp_v0_2 = (a0->unk18 | a0->unk1A);
            temp_s1->unk194 &= ~temp_v0_2;
            temp_s1->unk196 &= ~temp_v0_2;
            temp_s1->unk198 &= ~temp_v0_2;
            temp_s1->unk194 |= a0->unk16;
            temp_s1->unk196 |= a0->unk18;
            temp_s1->unk198 |= a0->unk1A;
            dll_5_func_2AE0(0, temp_s1->seqp.maxChannels, a0->unk18, a0->unk1A, a0->unk16, sp64);
            for (var_s4 = 0, temp = 1; var_s4 < (s32) temp_s1->seqp.maxChannels; var_s4++, temp <<= 1) {
                switch (sp64[var_s4]) {
                    case -1:
                        func_80075E38(&temp_s1->seqp, var_s4);
                        temp_s1->unk19C[var_s4] = 0;
                        break;
                    case -2:
                        break;
                    case -3:
                        sp64[var_s4] = a1;
                        /* fallthrough */
                    default:
                        func_80075E9C(&temp_s1->seqp, var_s4);
                        func_80075DCC(&temp_s1->seqp, var_s4, (u8) sp64[var_s4]);
                        temp_s1->unk19C[var_s4] = (s16) (sp64[var_s4] * 0x10);
                        break;
                }
            }
            if (a0->unk15 != 0) {
                temp_s1->unk19A = (u8) (u32) ((f32) temp_s1->unk190 / (f32) (a0->unk15 * 6));
                temp_s1->unk19B = (u8) (u32) ((f32) temp_s1->unk190 / (f32) (a0->unk15 * 6));
            } else {
                temp_s1->unk19A = 0;
                temp_s1->unk19B = 0;
            }
        }
    } else {
        switch (a0->unk10) {                        /* irregular */
        case 0:
            sp83 = _data_0;
            break;
        case 1:
            sp83 = 1 - _data_0;
            break;
        case 2:
            sp83 = _data_4 + 2;
            break;
        case 3:
            sp83 = 3 - _data_4;
            break;
        }
        temp_s1 = _bss_0[sp83];
        temp_s1->unk18C = 0;
        temp_s1->unk0 &= 0xFFFE;
        n_alCSPStop(&temp_s1->seqp);
    }

    return sp83;
}
#endif

// offset: 0x26D0 | func: 42
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_26D0.s")
#else
// needs dll_5_func_1E8C to be static
void dll_5_func_26D0(void) {
    Camera* temp_v0;
    MusicAction* musicAction;
    Object* obj;
    f32 var_fa1;
    f32 var_fv0;
    f32 var_fv1;
    s32 temp_ft0;
    s32 i;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    s8 temp_v1_4;
    Object* temp_v0_2;
    s8 sp78[4];
    Trigger_Data* temp_v1_3;
    s8 sp70[3];
    s32 sp64[3];

    obj = _data_8;
    if (obj != NULL) {
        if (obj->unkB0 & 0x40) {
            _data_8 = NULL;
            obj = NULL;
        } else {
            targetX = obj->positionMirror.f[0];
            targetY = obj->positionMirror.f[1];
            targetZ = obj->positionMirror.f[2];
        }
    }

    if (obj == NULL) {
        temp_v0 = get_camera_array();
        update_camera_for_object(temp_v0);
        targetX = temp_v0->tx;
        targetY = temp_v0->ty;
        targetZ = temp_v0->tz;
    }

    for (i = 0; i < 4; i++) {
        sp78[i] = -1;
    }

    for (i = 0; i < 3; i++) {
        sp70[i] = -1;
        sp64[i] = 0xFFFF;
    }

    for (i = 0; i < _bss_24; i++) {
        temp_v0_2 = _bss_20[i].unk0;
        if (temp_v0_2->unkB0 & 0x40) {
            _bss_20[i].unk0 = NULL;
            _bss_24--;
            bcopy(&_bss_20[_bss_24], &_bss_20[i], 0x2C);
            i = 0;
        }
    }
    
    for (i = 0; i < _bss_24; i++) {
        _bss_20[i].unk28 = _bss_20[i].unk26;
        temp_v0_2 = _bss_20[i].unk0;
        if ((temp_v0_2->setup != NULL) && (temp_v0_2->setup->objId == OBJ_TriggerPoint)) {
            temp_v1_3 = temp_v0_2->data;
            var_fv0 = temp_v1_3->activatorCurrPos.x - targetX;
            var_fv1 = temp_v1_3->activatorCurrPos.y - targetY;
            var_fa1 = temp_v1_3->activatorCurrPos.z - targetZ;
        } else {
            var_fv0 = temp_v0_2->srt.transl.x - targetX;
            var_fv1 = temp_v0_2->srt.transl.y - targetY;
            var_fa1 = temp_v0_2->srt.transl.z - targetZ;
        }
        if (_bss_20[i].unk4.unk3 == 1) {
            var_fv1 = 0.0f;
        } else if (_bss_20[i].unk4.unk3 == 2) {
            var_fa1 = 0.0f;
            var_fv0 = 0.0f;
        }
        temp_ft0 = sqrtf(SQ(var_fv0) + SQ(var_fv1) + SQ(var_fa1));
        temp_v1_4 = _bss_20[i].unk4.unk10;
        if (temp_ft0 < sp64[temp_v1_4]) {
            sp70[temp_v1_4] = i;
            sp64[temp_v1_4] = temp_ft0;
        }
        _bss_20[i].unk26 = temp_ft0;
    }

    for (i = 0; i < 3; i++) {
        if (sp70[i] == -1) {
            continue;
        }

        musicAction = &_bss_20[sp70[i]].unk4;
        if (sp64[i] < musicAction->unk4) {
            var_fv1 = 127.0f;
        } else {
            if (musicAction->unk6 >= sp64[i]) {
                var_fv0 = (sp64[i] - musicAction->unk4);
                var_fv0 /= (musicAction->unk6 - musicAction->unk4);
                if (musicAction->unk2 == 2) {
                    var_fv0 *= var_fv0;
                }
                var_fv0 = 1.0f - var_fv0;
                var_fv1 = 127.0f * var_fv0;
            } else {
                continue;
            }
        }
        dll_5_func_1E8C(musicAction, var_fv1, -1);
    }
}
#endif

// offset: 0x2AE0 | func: 43
static void dll_5_func_2AE0(u8 arg0, u8 arg1, u16 arg2, u16 arg3, u16 arg4, s8* arg5) {
    u16 var_v0;
    u16 var_v1;
    s8 new_var;

    var_v0 = 0;
    var_v1 = 1;
    while (var_v0 < arg1) {
        if (!((arg2 | arg3) & var_v1)) {
            if (arg0 != 0) {
                arg5[var_v0] = 127;
            } else {
                arg5[var_v0] = -2;
            }
        } else if ((arg2 & var_v1) && (arg3 & var_v1)) {
            if (arg0 != 0) {
                arg5[var_v0] = 0;
            } else {
                arg5[var_v0] = -3;
            }
        } else if (arg3 & var_v1) {
            if (arg4 & var_v1) {
                arg5[var_v0] = 127;
            } else {
                arg5[var_v0] = -1;
            }
        } else if (arg2 & var_v1) {
            if ((arg4 & var_v1) && (arg0 != 0)) {
                arg5[var_v0] = 0;
            } else {
                arg5[var_v0] = -2;
            }
        } else {
            arg5[var_v0] = -2;
        }
        
        // @fake ---
        new_var = arg5[var_v0];
        if (new_var != -3) {
            if ((new_var && new_var) && new_var){}
        }
        // @fake ---
        
        var_v0++;
        var_v1 <<= 1;
    }
}

// offset: 0x2C44 | func: 44
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_2C44.s")
#else
// https://decomp.me/scratch/Kmu7i
ALMicroTime dll_5_func_2C44(void** oscState, f32* initVal, u8 oscType, u8 oscRate, u8 oscDepth, u8 oscDelay, u8 oscRiseTime) {
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
    if (_bss_14 == NULL) {
        //STUBBED_PRINTF("amAudio: WARNING no free oscstates.\n");
        return 0;
    }
    state = _bss_14;
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
    _bss_14 = _bss_14->unk00;
    if (oscDelay) {
        return D_800967A0[oscDelay];
    }
    
    return AL_USEC_PER_FRAME;
}
#endif

// offset: 0x3038 | func: 45
ALMicroTime dll_5_func_3038(void *oscState_, f32* updateVal) {
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
void dll_5_func_3434(void *oscState) {
    ((struct oscstate *)oscState)->unk00 = (struct oscstate *)_bss_14;
    _bss_14 = (struct oscstate *)oscState;
}

// offset: 0x345C | func: 47 | export: 17
void dll_5_func_345C(Object *obj) {

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
