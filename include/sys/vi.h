#ifndef _SYS_VI_H
#define _SYS_VI_H

#include "PR/ultratypes.h"
#include "PR/sched.h"
#include "game/objects/object.h"

/**
 * The video width is the lower 16 bits of the returned 32 bit value
 */
#define GET_VIDEO_WIDTH(width_and_height) (width_and_height & 0xFFFF)
/**
 * The video width is the higher 16 bits of the returned 32 bit value
 */
#define GET_VIDEO_HEIGHT(width_and_height) (width_and_height >> 16)

extern u16 *gBackFramebuffer; // next frame
extern u16 *gFrontFramebuffer; // current frame
extern u16 *gFrontDepthBuffer;
extern u16 *gBackDepthBuffer;

extern s32 gDisplayHertz;
extern f32 gAspectRatio; //1.121212 for PAL, 1.333 for NTSC/MPAL.

void viInit(s32 videoMode, OSSched* scheduler, s32 someBool);
/**
 * Returns gVideoMode.
 */
s32 viGetMode(void);
/**
 * Returns a video resolution encoded as 0xVVVV_HHHH.
 *
 * If the result of shadowtexGetStatus is 0, then it will be the current framebuffer's resolution.
 * Otherwise, it will be the active shadow framebuffer texture's resolution.
 */
u32 viGetCurrentSize(void);
u16 *viGetFramebufferEnd(void);
void viSetUpdateRateTarget(u32 target);
s32 viFrameSync(s32);
int viContainsPoint(s32 x, s32 y);
s32 viObjDepth(s32 x, s32 y, Object *arg2);
void viSomeVideoSetup(s32 param1);

#endif //_SYS_GFX_GX_H
