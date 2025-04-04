#ifndef _SYS_DEBUG_H
#define _SYS_DEBUG_H

#include "PR/ultratypes.h"

void debug_toggle(s8 enabled);
s8 debug_is_enabled();
s32 debug_tick();

#endif //_SYS_DEBUG_H
