/** Texture system
 */
#ifndef _SYS_GFX_TEXTURE_H
#define _SYS_GFX_TEXTURE_H
#include "PR/ultratypes.h"
#include "PR/gbi.h"

typedef struct Texture
{
/*0000*/	u8 width;
/*0001*/	u8 height;
/*0002*/	u8 format;
/*0003*/	u8 unk_0x3;
/*0004*/	u16 unk_0x4;
/*0006*/	s16 flags;
/*0008*/	Gfx *gdl;
/*000C*/	u16 levels;
/*000E*/	u16 unk_0xe;
/*0010*/	u16 unk_0x10;
/*0012*/	s16 gdlIdx;
/*0014*/	struct Texture *next;
/*0018*/	s16 unk_0x18;
/*001A*/	u8 unk_0x1a;
/*001B*/	u8 unk_0x1b;
/*001C*/	u8 cms;
/*001D*/	u8 masks;
/*001E*/	u8 cmt;
/*001F*/	u8 maskt;
} Texture; // Size: 0x20, followed by texture data

#endif //_SYS_GFX_TEXTURE_H
