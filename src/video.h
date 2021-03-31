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

#define UNKNOWN_HEAP_VIDEO_STRUCT_SIZE 0x60

typedef struct _UnknownHeapVideoStruct {
    f32 unk0x0;
    f32 unk0x4;
    f32 unk0x8;
    f32 unk0xc;
    f32 unk0x10;
    f32 unk0x14;
    f32 unk0x18;
    f32 unk0x1c;
    f32 unk0x20;
    f32 unk0x24;
    f32 unk0x28;
    f32 unk0x2c;
    f32 unk0x30;
    f32 unk0x34;
    f32 unk0x38;
    f32 unk0x3c;
    f32 unk0x40;
    f32 unk0x44;
    f32 unk0x48;
    f32 unk0x4c;
    f32 unk0x50;
    f32 unk0x54;
    f32 unk0x58;
    f32 unk0x5c;
} UnknownHeapVideoStruct;

#define UNKNOWN_VIDEO_STRUCT_2_SIZE 0x14

typedef struct _UnknownVideoStruct2 {
    f32 unk0x0;
    f32 unk0x4;
    f32 unk0x8;
    u8 unk0xc;
    u8 unk0xd_padding[3];
    f32 *unk0x10;
} UnknownVideoStruct2;

#define UNKNOWN_VIDEO_STRUCT_SIZE 0x90

typedef struct _UnknownVideoStruct {
    struct _UnknownHeapVideoStruct *ptrToUnknown1;
    f32 *ptrToUnknown2;
    u8 unk0x8_padding[124]; // definitely not padding
    OSViMode *viMode;
    s16 unk0x88;
    u8 unk0x8a_padding[6]; // probably not padding
} UnknownVideoStruct;

// Length of gUnknownVideoStructs
#define UNKNOWN_VIDEO_STRUCTS_COUNT 40

extern UnknownVideoStruct gUnknownVideoStructs[UNKNOWN_VIDEO_STRUCTS_COUNT];
extern UnknownHeapVideoStruct *D_800bcc10;
extern UnknownVideoStruct2 D_800bcc18[4]; // size:0x50
extern s32 D_800bcc68;
extern s32 D_800bcc6c;
extern u32 D_800bcc70;
extern u8 D_800bcc78;

extern OSViMode gOSViModeCustom;

extern void *gFramebufferNext;
extern void *gFramebufferCurrent;
extern u32 *D_800bccb4;
extern u32 *D_800bccb0;

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

extern s8 gVScaleMod;
extern s8 gHStartMod;

extern OSMesgQueue OSMesgQueue_800bcce0;

extern u8 D_800bce34;
extern u8 D_800bce58;
extern u8 D_800bce59;
extern s32 D_800bce28;

extern u32 D_800bce14;
extern u8 D_800bce2c;
extern s32 D_80092FF8;

#endif
