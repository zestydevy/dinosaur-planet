#ifndef _SYS_FRAMEBUFFER_FX_H
#define _SYS_FRAMEBUFFER_FX_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"

enum FramebufferFXID {
    FBFX_NONE = 0, // off
    FBFX_SINE_WAVES = 1,
    FBFX_FADE_OUT_FADE_IN = 2,
    FBFX_LERP = 3,
    FBFX_SLIDE = 4,
    FBFX_NOOP = 5,
    FBFX_BURN_PAPER_RANDOM = 6,
    FBFX_BURN_PAPER_CENTER = 7,
    FBFX_BURN_PAPER_RIGHT = 8,
    FBFX_BURN_PAPER_CORNERS = 9,
    FBFX_MOTION_BLUR = 10,
    FBFX_FADE_OUT_RIGHT_FADE_IN = 11,
    FBFX_FADE_OUT_LEFT_FADE_IN = 12,
    FBFX_FADE_OUT_DOWN_FADE_IN = 13,
    FBFX_FADE_OUT_UP_FADE_IN = 14,
    FBFX_FADE_OUT = 15
};

void fbfx_tick(Gfx **gdl, s32 updateRate);
void fbfx_play(s32 effectID, s32 duration);

#endif
