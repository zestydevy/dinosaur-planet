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
void diCpuDrawCpuInfo(OSThread** threads, s32 threadsCount, s32 threadsIdx);
void diCpuDrawStackTrace(OSThread** threads, s32 threadsCount, s32 threadsIdx);
void diCpuDrawFpuInfo(OSThread **threads, s32 threadsCount, s32 threadsIdx);
void diCpuCrashScreenInit(void);
void diCpuTraceFile(const char *filename, s32 lineno);

void* diCpuTraceCurrentStack(void);

#endif
