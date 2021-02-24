#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "ultra64.h"
#include "common_structs.h"

void bootproc(void);
void idle(void * arg);
void mainproc(void * arg);

void testWrite(void);
void some_init_func(void);
void init_hasnt_expPak(void);
void has_exppak_func(void);
void tick_two_timers(void);
void checkDongle(void);

#endif
