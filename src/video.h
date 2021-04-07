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

typedef struct _UnkHeapVidStruct {
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
} UnkHeapVidStruct;

#define UNKNOWN_VIDEO_STRUCT_2_SIZE 0x14

typedef struct _UnkVidStruct2 {
    f32 unk0x0;
    f32 unk0x4;
    f32 unk0x8;
    u8 unk0xc;
    u8 unk0xd_padding[3];
    f32 *unk0x10;
} UnkVidStruct2;

#define UNKNOWN_VIDEO_STRUCT_SIZE 0x90

typedef struct _UnkVidStruct {
    struct _UnkHeapVidStruct *ptrToUnknown1;
    f32 *ptrToUnknown2;
    u8 unk0x8_pad[16];
    s16 unk0x18;
    s16 unk0x1a;
    s16 unk0x1c;
    u8 unk0x1e_pad[10];
    s16 unk0x28;
    s16 unk0x2a;
    s16 unk0x2c;
    u8 unk0x2e_pad[10];
    s16 unk0x38;
    s16 unk0x3a;
    s16 unk0x3c;
    u8 unk0x3e_pad[10];
    s16 unk0x48[4];
    u8 unk0x4c_pad[40];
    f32 unk0x78;
    f32 unk0x7c;
    u8 unk0x80_pad[4];
    OSViMode *viMode;
    s16 unk0x88;
    u8 unk0x8a_padd[6];
} UnkVidStruct;

typedef struct _UnkVidStruct3 {
    s32 unk0x0;
    s32 unk0x4;
} UnkVidStruct3;

// Length of gUnknownVideoStructs
#define UNKNOWN_VIDEO_STRUCTS_COUNT 40

extern UnkVidStruct gUnknownVideoStructs[UNKNOWN_VIDEO_STRUCTS_COUNT];
extern UnkHeapVidStruct *D_800bcc10;
extern UnkVidStruct2 D_800bcc18[4]; // size:0x50
extern s32 D_800bcc68;
extern s32 D_800bcc6c;
extern u32 D_800bcc70;
extern u8 D_800bcc78;

extern OSViMode gOSViModeCustom;

extern void *gFramebufferNext;
extern void *gFramebufferCurrent;
extern u16 *D_800bccb4;
extern u16 *D_800bccb0;

extern f32 D_80092A70;
extern f32 D_80092A74;

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

extern u16 *gFramebufferStart;
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
/**
 * Some boolean.
 */
extern int gSomeVideoFlag;

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

extern s8 D_80093060;
extern UnkVidStruct3 *D_80093068;

extern s32 **D_800BCE18;
extern u8 D_800BCE20; // index of D_800BCE22?
extern u8 *D_800BCE22;

#endif
