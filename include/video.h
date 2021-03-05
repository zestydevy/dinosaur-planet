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

typedef struct _UnknownVideoStruct {
    f32* ptrToUnknown1;
    f32* ptrToUnknown2;
    struct _UnknownVideoStruct* ptrToUnknown3;
    u32 unk0xc, unk0x10;
    u8 unk0x14;
    u8 unk0x15_padding[3];
    u8 unk0x18[64];
    u8 unk0x58_padding[24]; // probably not padding
    f32 unk0x70;
    f32 unk0x74;
    u8 unk0x78_padding[8]; // probably not padding
    f32 unk0x80;
    OSViMode* viMode;
    s16 unk0x88;
    u8 unk0x8a_padding[6]; // probably not padding
} UnknownVideoStruct;

extern UnknownVideoStruct gUnknownVideoStructs[40];
extern UnknownVideoStruct gMaybeCurrentUnknownVideoStruct;

extern OSViMode gOSViModeCustom;

extern void *gFramebufferNext;
extern void *gFramebufferCurrent;

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
extern void *gFramebufferPointers[2];

extern u32 framebufferAddress_ExpPak;
extern u32 framebufferAddress_NoExpPak;

extern u32 *gFramebufferStart;
extern u32 *gFramebufferEnd;

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
