#ifndef _COMMON_STRUCTS_H
#define _COMMON_STRUCTS_H

#include "ultra64.h"

// typedef f32 MtxF[4][4];
// For clarity:
typedef struct {
    f32 m[4][4];
} MtxF;

typedef f32 quartic[5];

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x000 */ s32 regs[50];
    /* 0x0C8 */ char unk_0C8[4];
    /* 0x0CC */ s32 gp;
    /* 0x0D0 */ char unk_0D0[4];
    /* 0x0D4 */ s32 sp_upper;
    /* 0x0D8 */ void* sp;
    /* 0x0DC */ s32 s8_up;
    /* 0x0E0 */ s32 s8;
    /* 0x0E4 */ u32 ra_up;
    /* 0x0E8 */ void* ra;
    /* 0x0EC */ s32 lo_up;
    /* 0x0F0 */ s32 lo;
    /* 0x0F4 */ s32 hi_up;
    /* 0x0F8 */ u32 hi;
    /* 0x0FC */ u32 sr;
    /* 0x100 */ void* pc;
    /* 0x104 */ u32 cause;
    /* 0x108 */ u32 badvaddr;
    /* 0x10C */ u32 rcp;
    /* 0x110 */ u32 fpcsr;
    /* 0x114 */ f32 fregs[32];
} OSThreadContext; // size = 0x194

struct OSThread {
    /* 0x000 */ OSThread* next;
    /* 0x004 */ s32 priority;
    /* 0x008 */ OSThread** queue;
    /* 0x00C */ OSThread* tlnext;
    /* 0x010 */ u16 state;
    /* 0x012 */ u16 flags;
    /* 0x014 */ s32 id;
    /* 0x018 */ s32 fp;
    /* 0x01C */ OSThreadContext context;
    /* 0x1B0 */ char unk_1B0[0x8];
    /* 0x1B8 */ void* stackInfo;
};

typedef union {
    /* 0x04 */ void (*asVoid)(void);
    /* 0x04 */ s32 (*asVoidS32)(void); //HACK
    /* 0x04 */ void (*withOneArg)(s32);
    /* 0x04 */ void (*withTwoArgs)(s32, s32);
    /* 0x04 */ void (*withThreeArgs)(s32, s32, s32);
    /* 0x04 */ void (*withFourArgs)(s32, s32, s32, s32);
    /* 0x04 */ void (*withFiveArgs)(s32, s32, s32, s32, u16);
} DLLFuncs;

typedef struct DLLInstance {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ DLLFuncs func[1]; //set to 1 to shut compiler up
} DLLInstance;

struct UnkStruct_800175D4 {
    s32 a;
    u8  b[4];
};

#endif
