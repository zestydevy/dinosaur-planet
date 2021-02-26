#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "ultra64.h"
#include "common_structs.h"

void bootproc(void);
void idle(void * arg);
void mainproc(void * arg);

void testWrite(void);
void initMemory(void);
void main_no_expPak(void);
void main_expPak(void);
void threadTimerTick(void);
void checkDongle(void);

s32  find_heap_block(void *ptr);
void set_heap_block(void * heap, s32 size, s32 max);
void func_80017254(u32 arg0);

#endif
