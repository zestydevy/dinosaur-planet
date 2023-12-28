#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "ultra64.h"
#include <PR/sched.h>
#include "constants.h"

//unknown What this is for.
struct PointersInts{
	void *prts[2];
	u32 valA;
	u32 valB;
};

struct UnkStruct_800175D4 {
    s32 a;
    u8  b[4];
};

extern u8 D_8008C940;
extern s32 D_800AE674;
extern u8 D_800B09C3;
extern s8 D_800B09C2;
extern s8 D_800B09C4;
extern s8 D_8008C94C;
extern s32 D_800B179C;
extern s32 D_800A7D14;
extern s32 D_800A7D10;


extern int func_printing_null_nil ( char * str, const char * format, ... );

/* not variables
extern struct ErrString errStringArray_cause[];
extern struct ErrString errStringArray_fpsr[];
*/
extern struct PointersInts pointersIntsArray[];
extern struct UnkStruct_800175D4 pointerIntArray0[]; // possible 420 length. counter for array fits this size.
extern u16 pointerIntArrayCounter; //yeah, this needs a better name.

/* No idea where to put these*/
struct UnkStruct_80064514 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s16 unkA;
    f32 unkC;
    s16 unk10;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    f32 unk18;
    s32 unk1C;
    s32 unk20;
};


struct UnkStruct_n_syn {
    /* These are all copied from another struct, so are likely wrong*/
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s16 unkA;
    f32 unkC;
    s16 unk10;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    f32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk28;
    /*This seems to be right*/
    struct UnkStruct_80064514* unk2C;
};

#endif //_VARIABLES_H_
