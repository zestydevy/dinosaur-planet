/** Crash handler system
 */
#ifndef _SYS_CRASH_H
#define _SYS_CRASH_H
#include "PR/ultratypes.h"

//use for crash screens
struct ErrString{
	u32 code1;
	u32 code2;
	char* text;
};

#endif //_SYS_CRASH_H
