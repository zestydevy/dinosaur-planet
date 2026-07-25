#ifndef _SYS_RAND_H
#define _SYS_RAND_H

#include "PR/ultratypes.h"

// official name: mathSeed
void mathSeed(s32 seed);
void mathSaveSeed(void);
void mathLoadSeed(void);
s32 mathGetSeed(void);
// official name: mathRnd
s32 mathRnd(s32 min, s32 max);

#endif
