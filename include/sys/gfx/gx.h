/** Graphics backend (RSP/RDP, etc)
 */
#ifndef _SYS_GFX_GX_H
#define _SYS_GFX_GX_H

#include "PR/os.h"
#include "PR/sched.h"
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

#define UNKNOWN_HEAP_VIDEO_STRUCT_SIZE 0x60

typedef struct _UnkHeapVidStruct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
} UnkHeapVidStruct;

// size: 0x90
typedef struct _UnkVidStruct {
    Gfx dl;
    Gfx *dl2;
    u8 unkC_pad[12];
    Vtx_t unk18[4];
    u8 unk58_pad[0x20];
    f32 unk78;
    f32 unk7C;
    Object *obj;
    OSViMode *viMode;
    s16 unk88;
    u8 unk8A_padd[6];
} UnkVidStruct;

// size: 0x14
typedef struct UnkVidStruct2 {
/*0000*/    f32 unk0;
/*0004*/    f32 unk4;
/*0008*/    f32 unk8;
/*000C*/    u8 unkC;
/*000D*/    // padding
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

extern void *D_80092EC4;
extern void *D_80092F0C;
extern void *D_80092F54;
extern void *D_80092F9C;
extern void *D_80092FE4;

extern s8 D_80093060;
extern UnkVidStruct3 D_80093068[];

extern UnkVidStruct gUnknownVideoStructs[UNKNOWN_VIDEO_STRUCTS_COUNT];
extern Texture *D_800BCC68;
extern Texture *D_800BCC6C;
extern Texture *D_800BCC70;
extern u8 D_800BCC78;
extern f32 D_800BCC80[2];

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
u32 get_some_resolution_encoded(void);

u16 *get_framebuffer_end();

void func_8005DA00(u32 param1);

s32 video_func_returning_delay(s32);

#define RESOLUTION_WIDTH(encoded) (encoded & 0xFFFF)
#define RESOLUTION_HEIGHT(encoded) (encoded >> 16)

void func_8005CA5C(u32 param1);

#endif //_SYS_GFX_GX_H
