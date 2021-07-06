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
extern u8 D_800B09C2;
extern u8 D_800B09C4;
extern u8 D_8008C94C;
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

#endif
