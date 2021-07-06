#ifndef _CRASH_H_
#define _CRASH_H_

#include "ultra64.h"

#define CRASH_THREAD_ID 100
#define CRASH_THREAD_PRIORITY 0x80

void start_crash_thread(OSSched* scheduler);

#endif
