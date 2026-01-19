/** Graphics backend (RSP/RDP, etc)
 */
#ifndef _SYS_GFX_GX_H
#define _SYS_GFX_GX_H

#include "PR/os.h"
#include "PR/sched.h"
#include "sys/gfx/map.h"
#include "sys/gfx/texture.h"
#include "game/objects/object.h"

/**
 * Memory address of the framebuffers when an N64 Expansion Pak IS NOT detected.
 */
#define FRAMEBUFFER_ADDRESS_NO_EXP_PAK 0x802d4000
/**
 * Memory address of the framebuffers when an N64 Expansion Pak IS detected.
 */
#define FRAMEBUFFER_ADDRESS_EXP_PAK 0x80119000

/**
 * Holds the horizontal and vertical resolution for video related functions.
 */
typedef struct VideoResolution {
    u32 h;
    u32 v;
} VideoResolution;

// size: 0x90
typedef struct _UnkVidStruct {
/*00*/ Gfx dl[3];
/*18*/ Vtx_t unk18[4];
/*58*/ DLTri unk58[2];
/*78*/ f32 unk78;
/*7C*/ f32 unk7C;
/*80*/ Object *obj;
/*84*/ s32 unk84;
/*88*/ s16 unk88;
} UnkVidStruct;

// size: 0x14
typedef struct UnkVidStruct2 {
/*0000*/    Vec3f unk0;
/*000C*/    u8 unkC;
/*0010*/    Object *unk10;
} UnkVidStruct2;

typedef struct UnkVidStruct3 {
    s32 unk0;
    s32 unk4;
} UnkVidStruct3;

// Length of gUnknownVideoStructs
#define UNKNOWN_VIDEO_STRUCTS_COUNT 40

// Length of gResolutionArray
#define VIDEO_RESOLUTIONS_COUNT 8

extern f32 gWorldX;
extern f32 gWorldZ;

extern UnkVidStruct3 D_80093068[];

extern OSViMode gTvViMode;

extern u16 *gBackFramebuffer; // next frame
extern u16 *gFrontFramebuffer; // current frame
extern u16 *gFrontDepthBuffer;
extern u16 *gBackDepthBuffer;

/**
 * Holds the horizontal resolution of each framebuffer.
 */
extern u32 gCurrentResolutionH[2];
/**
 * Holds the vertical resolution of each framebuffer.
 */
extern u32 gCurrentResolutionV[2];

/**
 * Pointers to each of the 2 framebuffers.
 *
 * {FBaddr,FBaddr*H*V*2} // TODO: <- what does that mean?
 */
extern u16 *gFramebufferPointers[2];

extern u16 *gDepthBuffer;
extern u16 *gFramebufferEnd;

/**
 * A framebuffer index into gFramebufferPointers.
 *
 * TODO: figure out more of what this var is used for
 */
extern u32 gCurrFramebufferIdx;
extern s32 gVideoMode;

extern VideoResolution gResolutionArray[VIDEO_RESOLUTIONS_COUNT];

extern f32 gViHeightRatio;

extern s8 gVScaleMod;
extern s8 gHStartMod;


extern u32 gViBlackTimer;
extern s32 gDisplayHertz;
extern u8 D_800BCE2C;

extern u8 gViUpdateRateTarget;
extern u8 D_800BCE58;
extern u8 gViUpdateRate;

extern u8 *D_800BCE18[];
extern u8 D_800BCE20; // index of D_800BCE22?
extern u8 D_800BCE22[2];

extern f32 gAspectRatio; //1.121212 for PAL, 1.333 for NTSC/MPAL.

extern OSScClient gVideoSched;

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

/**
 * The video width is the lower 16 bits of the returned 32 bit value
 */
#define GET_VIDEO_WIDTH(width_and_height) (width_and_height & 0xFFFF)
/**
 * The video width is the higher 16 bits of the returned 32 bit value
 */
#define GET_VIDEO_HEIGHT(width_and_height) (width_and_height >> 16)

void func_8005CA5C(u32 param1);

int vi_contains_point(s32 x, s32 y);

s32 vi_func_8005DD4C(s32 x, s32 y, s32 arg2);

#endif //_SYS_GFX_GX_H
