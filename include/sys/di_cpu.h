#ifndef _SYS_DI_CPU_H_
#define _SYS_DI_CPU_H_

#include "PR/os.h"
#include "PR/ultratypes.h"

void diCpuTraceObject(s32 index, s32 objID);
void diCpuTraceInit(void);
/**
 * Stops all active application threads (those with priorities between 1 and OS_PRIORITY_APPMAX).
 */
void diCpuStopActiveAppThreads(void);
void diCpuDrawCpuInfo(OSThread **threads, s32 count, s32 offset);
void diCpuDrawStackTrace(OSThread**, s32, s32);
void diCpuDrawFpuInfo(OSThread **threads, s32 count, s32 threadIdx);
void diCpuCrashScreenInit(void);
void diCpuTraceFile(const char *cFileLabel, s32 line);

void* diCpuTraceCurrentStack(void);

#endif
