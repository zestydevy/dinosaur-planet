#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "ultra64.h"

#include "constants.h"
#include "common_structs.h"
#include "variables.h"

/**
 * Holds the horizontal and vertical resolution for video related functions.
 */
typedef struct VideoResolution {
    u32 h;
    u32 v;
} VideoResolution;

extern OSViMode gOSViModeCustom;

extern void* gFramebufferNext;
extern void* gFramebufferCurrent;

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
extern void* gFramebufferPointers[2];

extern u32 framebufferAddress_ExpPak;
extern u32 framebufferAddress_NoExpPak;

extern u32* gFramebufferStart;
extern u32* gFramebufferEnd;

/**
 * A framebuffer index into gFramebufferPointers.
 * 
 * TODO: figure out more of what this var is used for
 */
extern u32 gFramebufferChoice; 
extern u32 gVideoMode;

extern VideoResolution gResolutionArray[];

extern u32 gPossiblyScreenWidthOrHeight;
extern bool gSomeVideoFlag;

#endif
