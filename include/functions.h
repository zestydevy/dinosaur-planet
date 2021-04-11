#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "ultra64.h"
#include "common_structs.h"
#include "variables.h"

void bootproc(void);
void idle(void * arg);
void mainproc(void * arg);

void test_write(void);
void init_memory(void);
void main_no_expPak(void);
void main_expPak(void);
void threadTimerTick(void);
void check_dongle(void);

void func_80017254(u32 arg0);
void func_800175D4(s32 a0);
void update_PlayerPosBuffer(void);
void * func_80023914(void);

void some_crash_setter(DLLInst arg0[], s32 arg1);

s32 read_file_region(u32 id, void *dst, u32 offset, s32 size);

void free(void* p);
DLLFile * dll_load_from_tab(u16, u32 *);
void dll_relocate(DLLFile* dll);

void *malloc(s32 arg0, s32 arg1, s32 arg2);
void _bcopy(const void *src,void *dst,int length);

void matrix_from_srt(MtxF *mf, SRT *srt);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
s32 cos16_precise(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
s32 sin16_precise(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
s32 cos16(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
s32 sin16(s16 theta);

#endif
