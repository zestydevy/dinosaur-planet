#ifndef _DLLS_5_H
#define _DLLS_5_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct MusicAction {
/*00*/ u8 unk0; // unused, always 0
/*01*/ u8 unk1; // unused, always 0
/*02*/ u8 distFalloffExp; // if 2, distance fade uses a squared falloff. does nothing with any other value
/*03*/ u8 unk3; // does something if 1 or 2, always 0 in this build EXCEPT for action 6 (which is 2). falloff related
/*04*/ u16 distFalloffStart; // full volume less than this dist
/*06*/ u16 distFalloffEnd; // zero volume more than this dist
/*08*/ u32 unk8; // unused, always 0
/*0C*/ u32 unkC; // unused, always 0
/*10*/ u8 playerNo; // amseq player 0-3
/*11*/ u8 seqID; // music/ambient ID
/*12*/ u8 volume; // 0-127
/*13*/ u8 bpm;
/*14*/ u8 unk14; // unused, by set by some actions
/*15*/ u8 fadeTimeDs; // the time it takes to fade volume in/out in tenths of a second (deciseconds)
/*16*/ u16 unk16;
/*18*/ u16 unk18;
/*1A*/ u16 unk1A;
/*1C*/ u32 unk1C; // unused, always 0
} MusicAction;

DLL_INTERFACE(DLL_5_AMSEQ) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ s32 (*set)(Object *arg0, u16 actionNo, s32 arg2, s32 arg3, s32 arg4);
    /*1*/ void (*free)(Object *arg0, u16 actionNo, s32 arg2, s32 arg3, s32 arg4);
    /*2*/ s32 (*is_set)(Object *arg0, u16 arg1);
    /*3*/ void (*set_focus_obj)(Object *arg0);
    /*4*/ void (*play)(u8 arg0, u8 arg1);
    /*5*/ void (*play_ex)(u8 arg0, u8 arg1, s16 arg2, s16 arg3, u16 arg4);
    /*6*/ void (*stop)(u8 arg0);
    /*7*/ u8 (*get_no)(u8 arg0);
    /*8*/ u16 (*get_volume_option)(void);
    /*9*/ void (*set_volume_option)(u32 volume);
    /*10*/ UNK_TYPE_32 (*func10)(void);
    /*11*/ void (*func11)(UNK_TYPE_32 arg0);
    /*12*/ u8 (*get_volume)(u8 arg0);
    /*13*/ void (*set_volume)(u8 arg0, u8 arg1);
    /*14*/ UNK_TYPE_32 (*func14)(UNK_TYPE_32 arg0);
    /*15*/ void (*func15)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*16*/ void (*func16)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*17*/ void (*func17)(Object *obj);
    /*18*/ void (*tick)(void);
    /*19*/ void (*func19)(UNK_TYPE_32 arg0);
    /*20*/ void (*func20)(UNK_TYPE_32 arg0);
    /*21*/ void (*func21)(UNK_TYPE_32 arg0);
    /*22*/ void (*func22)(UNK_TYPE_32 arg0);
    /*23*/ void (*func23)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*24*/ void (*func24)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*25*/ void (*func25)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*26*/ UNK_TYPE_32 (*func26)(UNK_TYPE_32 arg0);
    /*27*/ void (*func27)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*28*/ void (*func28)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*29*/ void (*func29)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*30*/ UNK_TYPE_32 (*func30)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*31*/ UNK_TYPE_32 (*func31)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*32*/ void (*func32)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2);
    /*33*/ UNK_TYPE_32 (*func33)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*34*/ void (*func34)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2);
    /*35*/ void (*func35)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2);
};

#endif //_DLLS_5_H
