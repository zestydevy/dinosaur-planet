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

// 0 -> DLL -1 (no DLL)
// 1 -> DLL 53
// 2 -> DLL 55
extern void *gTempDLLInsts[3];


void mainproc(void *arg);

void func_80013FB4(void);

void func_800141A4(s32, s32, s32 playerno, s32);
void func_800142A0(f32 arg0, f32 arg1, f32 arg2);
void main_start_game(f32 x, f32 y, f32 z, s32 playerno);
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
void main_load_frontend(void);
void main_unload_frontend(void);
void main_demo_reset(void);
void main_demo_start(f32 x, f32 y, f32 z, s32 playerno);
s32 main_demo_next(void);
u8 main_demo_state(void);
u8 main_demo_finished(void);
void func_80014D34(f32 param1, f32 *outX, f32 *outY, f32 *outZ);

#endif
