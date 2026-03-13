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

void vi_init(s32 videoMode, OSSched* scheduler, s32 someBool);
/**
 * Returns gVideoMode.
 */
s32 vi_get_mode();
/**
 * Returns a video resolution encoded as 0xVVVV_HHHH.
 *
 * If the result of shadowtex_get_status is 0, then it will be the current framebuffer's resolution.
 * Otherwise, it will be the active shadow framebuffer texture's resolution.
 */
u32 vi_get_current_size(void);
u16 *vi_get_framebuffer_end();
void vi_set_update_rate_target(u32 target);
s32 vi_frame_sync(s32);
int vi_contains_point(s32 x, s32 y);
s32 vi_obj_depth(s32 x, s32 y, Object *arg2);
void vi_some_video_setup(s32 param1);

#endif //_SYS_GFX_GX_H
