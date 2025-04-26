#ifndef _SYS_MAIN_H
#define _SYS_MAIN_H

#include "PR/ultratypes.h"
#include "PR/sched.h"

extern u8 delayByte; //60/int(x) FPS x<=6
extern u8 delayByteMirror;
extern f32 delayFloat;
extern f32 delayFloatMirror;
extern f32 inverseDelay; // 1/delayByte
extern f32 inverseDelayMirror;

extern u8 alSynFlag;

// TODO: static DLLs

extern void *gTempDLLInsts[3];


void mainproc(void *arg);

void func_80013FB4(void);

void func_800141A4(s32, s32, s32, s32);
void func_800142A0(f32 arg0, f32 arg1, f32 arg2);
void func_800142F0(f32 x, f32 y, f32 z, s32 arg3);
void func_800143A4(void);

s8 get_pause_state(void);
void unpause();
void set_pause_state(s32 state);

void func_80014508(s8 arg0);

s32 ret1_8001454c(void);

OSSched *get_ossched(void);

void set_gplay_bitstring(s32 entry, u32 value);
u32 get_gplay_bitstring(s32 entry);
s32 increment_gplay_bitstring(s32 entry);
s32 decrement_gplay_bitstring(s32 entry);
s32 create_temp_dll(s32 id);
s32 remove_temp_dll(s32 id);
void func_80014B1C(void);
void func_80014B6C(void);
void func_80014BBC(void);
void func_80014BD4(f32 arg0, f32 arg1, f32 arg2, s32 arg3);
s32 func_80014C28(void);
u8 func_80014C50(void);
u8 func_80014C60(void);
void func_80014D34(f32 param1, f32 *outX, f32 *outY, f32 *outZ);

#endif
