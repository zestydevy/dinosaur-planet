/** Graphics backend (RSP/RDP, etc)
 */
#ifndef _SYS_GFX_GX_H
#define _SYS_GFX_GX_H

#include "PR/os.h"
#include "PR/sched.h"
#include "sys/gfx/texture.h"

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

// size: 0x14
typedef struct _UnkVidStruct2 {
/*0000*/    f32 unk0x0;
/*0004*/    f32 unk0x4;
/*0008*/    f32 unk0x8;
/*000C*/    u8 unk0xc;
/*000D*/    // padding
/*0010*/    f32 *unk0x10;
} UnkVidStruct2;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 unk6;
    s16 unk8;
    s16 unka;
    s8 unkc;
    s8 unkd;
    s8 unke;
    s8 unkf;
} UnkVidStruct_0x18;

typedef struct {
    u8 _unk0x0[0x46];
    s16 unk0x46;
} UnkVidStruct4;

// size: 0x90
typedef struct _UnkVidStruct {
    Gfx dl;
    u8 unk0x8_pad[16];
    UnkVidStruct_0x18 unk0x18[4];
    u8 unk0x58_pad[0x20];
    f32 unk0x78;
    f32 unk0x7c;
    UnkVidStruct4 *unk0x80;
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

// Length of gResolutionArray
#define VIDEO_RESOLUTIONS_COUNT 8

extern f32 gWorldX;
extern f32 gWorldZ;

extern void *D_80092EC4;
extern void *D_80092F0C;
extern void *D_80092F54;
extern void *D_80092F9C;
extern void *D_80092FE4;

extern s8 D_80093060;
extern UnkVidStruct3 *D_80093068;

extern f32 gPalAspectRatio;  // 1.212121_ (4/3.3)
extern f32 D_8009AD68;       // 1.1
extern f32 gMPalAspectRatio; // 1.333333_ (4/3)
extern f32 gNtscAspectRatio; // 1.333333_ (4/3)

extern UnkVidStruct gUnknownVideoStructs[UNKNOWN_VIDEO_STRUCTS_COUNT];
extern Texture *D_800BCC68;
extern Texture *D_800BCC6C;
extern Texture *D_800BCC70;
extern u8 D_800BCC78;
extern f32 D_800BCC80;
extern f32 D_800BCC84;

extern OSViMode gOSViModeCustom;

extern u16 *gFramebufferNext;
extern u16 *gFramebufferCurrent;
extern u16 *D_800BCCB4;
extern u16 *D_800BCCB0;

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

extern u16 *gFramebufferStart;
extern u16 *gFramebufferEnd;

/**
 * A framebuffer index into gFramebufferPointers.
 *
 * TODO: figure out more of what this var is used for
 */
extern u32 gFramebufferChoice;
extern s32 gVideoMode;

extern VideoResolution gResolutionArray[VIDEO_RESOLUTIONS_COUNT];

extern s32 gPossiblyScreenWidthOrHeight;
/**
 * Some boolean.
 */
extern int gSomeVideoFlag;

extern f32 D_800BCCB8;

extern s8 gVScaleMod;
extern s8 gHStartMod;

extern OSMesg D_800bccc0[8];
extern OSMesgQueue OSMesgQueue_800bcce0;

extern u32 D_800BCE14;
extern s32 gDisplayHertz;
extern u8 D_800BCE2C;

extern u8 D_800BCE34;
extern u8 D_800BCE58;
extern u8 D_800BCE59;

extern u8 *D_800BCE18[];
extern u8 D_800BCE20; // index of D_800BCE22?
extern u8 D_800BCE22[2];

extern s32 gMainGfx[], gMainMtx[], gMainVtx[], gMainPol[]; //likely pointers
extern Gfx *gCurGfx;
extern u8 gFrameBufIdx;
extern float aspectRatioFloat; //1.121212 for PAL, 1.333 for NTSC/MPAL.

extern OSScClient D_800bce60;

void func_8005D410(s32 videoMode, OSSched* scheduler, s32 someBool);

/**
 * Returns gVideoMode.
 */
s32 get_video_mode();

/**
 * Returns a video resolution encoded as 0xVVVV_HHHH.
 *
 * If the result of func_8005BC38 is 0, then it will be the current framebuffer's resolution.
 */
u32 get_some_resolution_encoded();

u16 *get_framebuffer_end();

#define RESOLUTION_WIDTH(encoded) (encoded & 0xFFFF)
#define RESOLUTION_HEIGHT(encoded) (encoded >> 16)

#endif //_SYS_GFX_GX_H
