#ifndef _SYS_RAND_H
#define _SYS_RAND_H

#include "PR/ultratypes.h"

void rand_set_seed(s32 seed);
void rand_save_seed();
void rand_load_seed();
s32 rand_get_seed();
s32 rand_next(s32 min, s32 max);

#endif
