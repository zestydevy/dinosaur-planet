#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "ultra64.h"
#include "common_structs.h"
#include "variables.h"

void bootproc(void);
void idle(void * arg);
void mainproc(void * arg);

void test_write(void);
void main_no_expPak(void);
void main_expPak(void);
void threadTimerTick(void);
void check_dongle(void);

void func_80017254(u32 arg0);
void func_800175D4(s32 a0);
void update_PlayerPosBuffer(void);
void * func_80023914(void);

void some_crash_setter(DLLInst arg0[], s32 arg1);

void _bcopy(const void *src,void *dst,int length);

#endif
