#ifndef _SYS_MPEG_H
#define _SYS_MPEG_H

#include "PR/ultratypes.h"
#include "PR/libaudio.h"

void mpeg_init(ALHeap *heap);
void mpeg_play(s32 id);

#endif
