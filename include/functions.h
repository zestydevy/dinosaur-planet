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

void free(s32);
DLLFile * dll_load_from_tab(u32, s32 *);
void dll_relocate(void * arg0);

void *malloc(s32 arg0, s32 arg1, s32 arg2);
void _bcopy(const void *src,void *dst,int length);

#endif
