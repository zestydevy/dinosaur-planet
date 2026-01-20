#ifndef _SYS_RSP_SEGMENT_H
#define _SYS_RSP_SEGMENT_H

#include "PR/gbi.h"

enum RSPSegments {
    SEGMENT_MAIN,
    SEGMENT_FRAMEBUFFER,
    SEGMENT_ZBUFFER,
    SEGMENT_3,
    SEGMENT_4,
    SEGMENT_COUNT
};

void rsp_segment(Gfx **gdl, u32 segment, void *base);

#endif
