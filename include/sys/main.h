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


void mainThreadEntry(void *arg);

void main_func_80013FB4(void);

void mainChangeMap(s32 mapID, s32 setupID, s32 playerno, s32 menuID);
void main_func_800142A0(f32 arg0, f32 arg1, f32 arg2);
void mainStartGame(f32 x, f32 y, f32 z, s32 playerno);
void main_func_800143A4(void);

s8 mainGetPauseState(void);
void mainUnpause();
void mainSetPauseState(s32 state);

void main_func_80014508(s8 arg0);

s32 main_ret1_8001454c(void);

OSSched *mainGetScheduler(void);

void mainSetBits(s32 entry, u32 value);
u32 mainGetBits(s32 entry);
s32 mainIncrementBits(s32 entry);
s32 mainDecrementBits(s32 entry);
s32 mainCreateTempDLL(s32 id);
s32 mainRemoveTempDLL(s32 id);
void mainLoadFrontend(void);
void mainUnloadFrontend(void);
void mainDemoReset(void);
void mainDemoStart(f32 x, f32 y, f32 z, s32 playerno);
s32 mainDemoNext(void);
u8 mainDemoState(void);
u8 mainDemoFinished(void);
void mainGetBufferedPlayerPos(f32 param1, f32 *outX, f32 *outY, f32 *outZ);
void main_func_800143A4(void);

#endif
