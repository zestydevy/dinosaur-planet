#ifndef _SYS_MPEG_H
#define _SYS_MPEG_H

#include "PR/ultratypes.h"
#include "PR/libaudio.h"

void mpegInit(ALHeap *heap);
void mpegPlay(s32 id);

#endif
