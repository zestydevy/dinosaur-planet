#ifndef _SYS_DEBUG_H
#define _SYS_DEBUG_H

#include "PR/ultratypes.h"

void debugToggle(s8 enabled);
s8 debugIsEnabled(void);
s32 debugTick(void);

#endif //_SYS_DEBUG_H
