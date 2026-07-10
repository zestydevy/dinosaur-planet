#ifndef _SYS_DI_RCP_H
#define _SYS_DI_RCP_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"

void diRcpTraceInit(void);
void diRcpTraceReset(void);
void diRcpTrace(Gfx *gdl, u32 objID, const char *file, u32 line);
void diRcpTraceGetInfo2(Gfx* arg0, u32* arg1, const char** arg2, u32* arg3, Gfx** arg4, u32* arg5, const char** arg6, u32* arg7, Gfx** arg8);

#endif
