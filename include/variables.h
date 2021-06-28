#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "ultra64.h"
#include <PR/sched.h>

typedef u8 UNK_TYPE_8;
typedef u16 UNK_TYPE_16;
typedef u32 UNK_TYPE_32;

#include "constants.h"
#include "common_structs.h"
#include "math.h"
#include "sys/dlls/dlls.h"
#include "sys/fs/files.h"
#include "sys/memory/heap.h"
#include "sys/menu/menu.h"
#include "sys/pad.h"
#include "sys/thread.h"
#include "sys/video/video.h"
#include "sys/fault.h"
#include "game/actors/actor.h"
#include "game/actors/player/player.h"
#include "game/state/gamebits.h"

//unknown What this is for.
struct PointersInts{
	void *prts[2];
	u32 valA;
	u32 valB;
};

extern u8 D_8008C940;
extern u8 alSynFlag;
extern s32 D_800AE678[], D_800AE688[], D_800AE698[], D_800AE6A8[]; //likely pointers
extern char * D_800991E0;
extern u8 D_800B09C1;
extern u8 D_800B09C3;
extern u8 D_800B09C2;
extern u8 D_800B09C4;
extern u8 D_8008C94C;
extern s32 D_800B179C;
extern s32 D_800A7D14;
extern s32 D_800A7D10;
extern int func_printing_null_nil ( char * str, const char * format, ... );
extern u8 delayByte; //60/int(x) FPS x<=6
extern u8 delayByteMirror;
extern float delayFloat;
extern float delayFloatMirror;
extern float inverseDelay; // 1/delayByte
extern float inverseDelayMirror; // why the mirrors, if they aren't used?

extern struct Vec3_Int Vec3_Int_array[];
/* not variables
extern struct ErrString errStringArray_cause[];
extern struct ErrString errStringArray_fpsr[];
*/
extern struct PointersInts pointersIntsArray[];
extern struct UnkStruct_800175D4 pointerIntArray0[]; // possible 420 length. counter for array fits this size.
extern u16 pointerIntArrayCounter; //yeah, this needs a better name.

#endif
