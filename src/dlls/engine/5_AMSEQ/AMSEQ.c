#include "common.h"
#include "libnaudio/n_libaudio.h"

// size:0x24C
typedef struct {
    u8 unk0;
    u8 _unk1[0x4 - 0x1];
    N_ALCSPlayer unk4;
    ALCSeq unk90;
    void *unk188;
    u8 _unk18C[0x190 - 0x18C];
    s16 unk190;
    u8 _unk192[0x24C - 0x192];
} AMSEQBss0;

/*0x0*/ static u32 _data_0 = 0x00000000;
/*0x4*/ static u32 _data_4 = 0x00000000;
/*0x8*/ static u32 _data_8 = 0x00000000;
/*0xC*/ static u32 _data_C = 0x00000000;

/*0x0*/ static AMSEQBss0 **_bss_0; // pointer to array of 4 pointers
/*0x4*/ static u8 _bss_4[0x4];
/*0x8*/ static u8 _bss_8[0x4];
/*0xC*/ static u8 _bss_C[0x4];
/*0x10*/ static u8 _bss_10[0x4];
/*0x14*/ static u8 _bss_14[0x4];
/*0x18*/ static u8 _bss_18[0x4];
/*0x1C*/ static u8 _bss_1C[0x4];
/*0x20*/ static u8 _bss_20[0x4];
/*0x24*/ static u8 _bss_24[0x4];
/*0x28*/ static u16 _bss_28;
/*0x2A*/ static u16 _bss_2A;
/*0x2C*/ static u16 _bss_2C;

// offset: 0x0 | ctor
void dll_5_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_ctor.s")

// offset: 0x7E8 | dtor
void dll_5_dtor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_dtor.s")

// offset: 0x930 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_930.s")

// offset: 0xB0C | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_B0C.s")

// offset: 0xC00 | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_C00.s")

// offset: 0xC6C | func: 3 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_C6C.s")

// offset: 0xCC8 | func: 4 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_CC8.s")

// offset: 0xE14 | func: 5 | export: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_E14.s")

// offset: 0xEE4 | func: 6 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_EE4.s")

// offset: 0xF58 | func: 7 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_F58.s")

// offset: 0xF74 | func: 8 | export: 9
void dll_5_func_F74(u32 volume) {
    AMSEQBss0* temp_v0;

    if (volume > 256) {
        volume = 256;
    }
    _bss_28 = volume;
    _bss_2C = (_bss_28 * _bss_2A) >> 8;
    temp_v0 = _bss_0[0];
    n_alCSPSetVol(&temp_v0->unk4, (s16) ((temp_v0->unk190 >> 4) * _bss_2C));
    temp_v0 = _bss_0[1];
    n_alCSPSetVol(&temp_v0->unk4, (s16) ((temp_v0->unk190 >> 4) * _bss_2C));
    temp_v0 = _bss_0[2];
    n_alCSPSetVol(&temp_v0->unk4, (s16) ((temp_v0->unk190 >> 4) * _bss_2C));
    // @bug: Doesn't affect the 4th music player
}

// offset: 0x1090 | func: 9 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1090.s")

// offset: 0x109C | func: 10 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_109C.s")

// offset: 0x10A8 | func: 11 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_10A8.s")

// offset: 0x10FC | func: 12 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_10FC.s")

// offset: 0x1150 | func: 13 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1150.s")

// offset: 0x1160 | func: 14 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1160.s")

// offset: 0x1170 | func: 15 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1170.s")

// offset: 0x1180 | func: 16 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1180.s")

// offset: 0x1228 | func: 17 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1228.s")

// offset: 0x1244 | func: 18 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1244.s")

// offset: 0x1250 | func: 19 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1250.s")

// offset: 0x125C | func: 20 | export: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_125C.s")

// offset: 0x1268 | func: 21 | export: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1268.s")

// offset: 0x1274 | func: 22 | export: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1274.s")

// offset: 0x1284 | func: 23 | export: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1284.s")

// offset: 0x1294 | func: 24 | export: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1294.s")

// offset: 0x12A4 | func: 25 | export: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_12A4.s")

// offset: 0x12B4 | func: 26 | export: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_12B4.s")

// offset: 0x12C4 | func: 27 | export: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_12C4.s")

// offset: 0x12D4 | func: 28 | export: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_12D4.s")

// offset: 0x12E4 | func: 29 | export: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_12E4.s")

// offset: 0x12F8 | func: 30 | export: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_12F8.s")

// offset: 0x130C | func: 31 | export: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_130C.s")

// offset: 0x1320 | func: 32 | export: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1320.s")

// offset: 0x1334 | func: 33 | export: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1334.s")

// offset: 0x1348 | func: 34 | export: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1348.s")

// offset: 0x135C | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_135C.s")

// offset: 0x14A0 | func: 36
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_14A0.s")

// offset: 0x1514 | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1514.s")

// offset: 0x1924 | func: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1924.s")

// offset: 0x1C08 | func: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1C08.s")

// offset: 0x1D60 | func: 40
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1D60.s")

// offset: 0x1E8C | func: 41
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_1E8C.s")

// offset: 0x26D0 | func: 42
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_26D0.s")

// offset: 0x2AE0 | func: 43
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_2AE0.s")

// offset: 0x2C44 | func: 44
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_2C44.s")

// offset: 0x3038 | func: 45
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_3038.s")

// offset: 0x3434 | func: 46
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_3434.s")

// offset: 0x345C | func: 47 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/5_AMSEQ/dll_5_func_345C.s")

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
