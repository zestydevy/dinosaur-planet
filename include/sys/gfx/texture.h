/** Texture system
 */
#ifndef _SYS_GFX_TEXTURE_H
#define _SYS_GFX_TEXTURE_H
#include "PR/ultratypes.h"
#include "PR/gbi.h"

// Gets the texture type. E.g: rgba16
#define TEX_FORMAT(x) (x & 0xF)

// Taken from DKR, might be incorrect
#define TEX_FORMAT_RGBA32 0
#define TEX_FORMAT_RGBA16 1
#define TEX_FORMAT_I8 2
#define TEX_FORMAT_I4 3
#define TEX_FORMAT_IA16 4
#define TEX_FORMAT_IA8 5
#define TEX_FORMAT_IA4 6
#define TEX_FORMAT_CI4 7
#define TEX_FORMAT_CI8 8

typedef struct Texture
{
/*0000*/	u8 width;
/*0001*/	u8 height;
/*0002*/	u8 format;
/*0003*/	u8 unk3;
/*0004*/	u8 unk4;
/*0005*/	u8 unk5;
/*0006*/	s16 flags;
/*0008*/	Gfx *gdl;
/*000C*/	u16 levels;
/*000E*/	u16 unkE;
/*0010*/	u16 unk10;
/*0012*/	s16 gdlIdx;
/*0014*/	struct Texture *next;
/*0018*/	s16 unk18;
/*001A*/	u8 unk1A;
/*001B*/	u8 unk1B;
/*001C*/	u8 cms;
/*001D*/	u8 masks;
/*001E*/	u8 cmt;
/*001F*/	u8 maskt;
} Texture; // Size: 0x20, followed by texture data

/**
 * Loads a texture via queue_load_texture.
 */
Texture *queue_load_texture_proxy(s32 id);
Texture *texture_load(s32 id, s32 param2);
s32 func_8003DC04(Gfx** arg0, Texture* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_8003E9B4(s32 arg0);
void func_8003E9D0(s32 arg0);

#endif //_SYS_GFX_TEXTURE_H
