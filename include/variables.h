#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "ultra64.h"
#include "PR/sched.h"
#include "constants.h"
#include "sys/crash.h"

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

extern s32 D_800B179C;
extern s32 D_800A7D14;
extern s32 D_800A7D10;


// not variables?
extern struct ErrString errStringArray_cause[];
extern struct ErrString errStringArray_fpsr[];

extern struct PointersInts pointersIntsArray[];
extern struct UnkStruct_800175D4 pointerIntArray0[]; // possible 420 length. counter for array fits this size.
extern u16 pointerIntArrayCounter; //yeah, this needs a better name.

extern f32 gWorldX;
extern f32 gWorldZ;

struct UnkStruct_80064514 {
    s32 var0;
    s32 var4;
    s16 var8;
    s16 varA;
    f32 varC;
    s16 var10;
    u8 var12;
    u8 var13;
    u8 var14;
    u8 var15;
    f32 var18;
    s32 var1C;
    s32 var20;
};

struct UnkStruct_n_sym_var34{
    s32 padding[9];
    s32 var24;
};

struct UnkStruct_n_syn {
    /* Padding memory address, as these are currently unknown*/
    s32 padding0[7];
    s32 var1C;
    s32 padding1[3];
    struct UnkStruct_80064514* var2C;
    s32 padding30;
    struct UnkStruct_n_sym_var34* var34;
    s32 var40;
};

typedef struct UnkStruct_8006C220 {
    s32 padding0[2]; /*Placeholder*/
    struct UnkStruct_8006C220_arg0_unk8* unk8; /*This seems to be an address to another struct, that has variable unk88*/
    s32 padding2[3]; /*Placeholder*/
    s16 padding; /*Placeholder*/
    s16 unk1A;

}UnkStruct_8006C220;

struct UnkStruct_8006C220_arg0_unk8 {
    /*Currently, only unk88 is known to be used, so this array will pad out memory addresses until more is known*/
    s32 padding[34];
    s32 unk88;
};

typedef struct UnkStruct_n_alEnvmixerParam {
    s32 padding[18];
    s32 var48;
    f32 var4C;
    s32 var50;
    s32 padding54;
    s16 padding58;
    s16 var5A;
    s32 padding5C;
    s32 padding60;
    s32 padding64;
    s32 padding68;
    s32 padding6C;
    s32 padding70;
    s32 var74;
    s32 var78;
    s32 var7C;
    s32* var80;
    s32 var84;
}UnkStruct_n_alEnvmixerParam;

typedef struct UnkStruct_80065D7C {
    s32 padding[17];
    u8 var44;

}UnkStruct_80065D7C;

struct UnkStruct_80065E6C {
    u16 var0;
    s16 padding2;
    s32 var4;
};

/* struct for arg0 for func_80065DEC*/
typedef struct UnkStruct_80065DEC {
    s32 padding[2];
    struct UnkStruct_80065DEC_VAR8* var8;
    s32 padding2[8];
    f32 var2C;

}UnkStruct_80065DEC;

/* struct for arg0->var8 in func_80065DEC*/
typedef struct  UnkStruct_80065DEC_VAR8 {
    s32 padding0;
    struct UnkStruct_80065DEC_VAR8_VAR5* var4;
} UnkStruct_80065DEC_VAR8;

/* struct for arg0->var8->var5 in func_80065DEC*/
typedef struct  UnkStruct_80065DEC_VAR8_VAR5 {
    s32 padding0;
    s8 padding4;
    s8 var5;
} UnkStruct_80065DEC_VAR8_VAR5;

typedef struct UnkStruct_800663B8 {
    s32 padding[16];
    s8 var40;
}UnkStruct_800663B8;

/* I think this is a linked list*/
struct UnkStruct_D_800938C0 {
    struct UnkStruct_D_800938C0* var0; /* must be address of next in linked list */
    /* Padding memory address, as these are currently unknown*/
    s32 padding0[16];
    u8 var44;
};


/* segment_BED0*/

typedef struct UnkStruct_8000B2D0 {
    s16 var0;
    s16 var2;
    s16 var4;
    //s16 padding6; /* cant remembr if this was needed at some point or not*/
    s8* var8;
    s32 varC;
}UnkStruct_8000B2D0;

typedef struct UnkStruct_8000B3E4 {
    s16 var0;
    s16 var2;
    s16 var4;
    //s16 padding6;                     /* cant remembr if this was needed at some point or not*/
    //struct UnkStruct_test* var8[1];   /* cant remembr if this was needed at some point or not*/
    s32 var8;
    s32 varC;
}UnkStruct_8000B3E4;

typedef struct UnkStruct_8000B6A4{
    s16 var0;
    s16 var2;
}UnkStruct_8000B6A4;


#endif //_VARIABLES_H_
