#ifndef _SYS_MAIN_H
#define _SYS_MAIN_H

#include "PR/ultratypes.h"
#include "PR/sched.h"

typedef struct {
    // Start bit index
    u16 start;
    // bits 0-4: length in bits (minus 1)
    // bit 5: has task
    // bits 6-7: which bitstring
    u8 field_0x2;
    u8 task;
} BitTableEntry;

extern const char *gGameBuildVersion;
extern const char *gGameBuildDateTime;
extern const char *gGameBuildName;
extern char gDBVersion[];

/**
 * The update rate multiplier for the current game tick.
 * This is equal to the number of vi frames that passed since the last tick.
 */
extern u8 gUpdateRate;
extern u8 gUpdateRateMirror;
extern f32 gUpdateRateF;
extern f32 gUpdateRateMirrorF;
extern f32 gUpdateRateInverseF; // 1/gUpdateRate
extern f32 gUpdateRateInverseMirrorF;

extern u8 alSynFlag;

// TODO: static DLLs

extern void *gTempDLLInsts[3];


void mainproc(void *arg);

void func_80013FB4(void);

void func_800141A4(s32, s32, s32 playerno, s32);
void func_800142A0(f32 arg0, f32 arg1, f32 arg2);
void func_800142F0(f32 x, f32 y, f32 z, s32 playerno);
void func_800143A4(void);

s8 get_pause_state(void);
void unpause();
void set_pause_state(s32 state);

void func_80014508(s8 arg0);

s32 ret1_8001454c(void);

OSSched *get_ossched(void);

void main_set_bits(s32 entry, u32 value);
u32 main_get_bits(s32 entry);
s32 main_increment_bits(s32 entry);
s32 main_decrement_bits(s32 entry);
s32 create_temp_dll(s32 id);
s32 remove_temp_dll(s32 id);
void func_80014B1C(void);
void func_80014B6C(void);
void func_80014BBC(void);
void func_80014BD4(f32 x, f32 y, f32 z, s32 playerno);
s32 func_80014C28(void);
u8 func_80014C50(void);
u8 func_80014C60(void);
void func_80014D34(f32 param1, f32 *outX, f32 *outY, f32 *outZ);

#endif
