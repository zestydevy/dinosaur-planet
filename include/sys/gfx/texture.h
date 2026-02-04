/** Texture system
 */
#ifndef _SYS_GFX_TEXTURE_H
#define _SYS_GFX_TEXTURE_H
#include "PR/ultratypes.h"
#include "PR/gbi.h"

// Gets the texture type. E.g: rgba16
#define TEX_FORMAT(x) (x & 0xF)

enum TextureFormat {
    TEX_FORMAT_RGBA32 = 0,
    TEX_FORMAT_RGBA16 = 1,
    TEX_FORMAT_I8 = 2,
    TEX_FORMAT_I4 = 3,
    TEX_FORMAT_IA16 = 4,
    TEX_FORMAT_IA8 = 5,
    TEX_FORMAT_IA4 = 6,
    TEX_FORMAT_CI4 = 7
};

enum TextureFlags {
    TEX_FLAG_1 = 0x1,
    TEX_FLAG_2 = 0x2,
    TEX_FLAG_4 = 0x4,
    TEX_FLAG_8 = 0x8,
    TEX_FLAG_10 = 0x10,
    TEX_FLAG_20 = 0x20,
    TEX_FLAG_40 = 0x40,
    TEX_FLAG_80 = 0x80,
    TEX_FLAG_MIPMAPS = 0x100,
    TEX_FLAG_200 = 0x200,
    TEX_FLAG_400 = 0x400,
    TEX_FLAG_800 = 0x800,
    TEX_FLAG_1000 = 0x1000,
    TEX_FLAG_2000 = 0x2000,
    TEX_FLAG_4000 = 0x4000,
    TEX_FLAG_8000 = 0x8000
};

typedef struct Texture {
       /**
        * Width/height are split across two fields. The first width/height fields
        * specify the lower 8 bits of the width/height and the widthHeightHi field
        * specifies the upper 4 bits of the width/height, for a total of 12 bits.
        *
        * i.e. width  = width  | ((widthHeightHi & 0xF0) << 4)
        *      height = height | ((widthHeightHi & 0x0F) << 8)
        */
/*00*/ u8 width;
/*01*/ u8 height;
/*02*/ u8 format; // lower 4 bits = TEX_FORMAT_*
/*03*/ u8 unk3;
/*04*/ u8 unk4;
/*05*/ u8 refCount;
/*06*/ s16 flags; // TextureFlags
/*08*/ Gfx *gdl;
/*0C*/ u16 animDuration;
/*0E*/ u16 animSpeed;
/*10*/ u16 unk10;
/*12*/ s16 gdlIdx;
/*14*/ struct Texture *next;
/*18*/ s16 unk18;
/*1A*/ u8 unk1A;
/*1B*/ u8 widthHeightHi; // upper 4 bits = width hi, lower 4 bits = height hi
/*1C*/ u8 cms;
/*1D*/ u8 masks;
/*1E*/ u8 cmt;
/*1F*/ u8 maskt;
} Texture; // Size: 0x20, followed by texture data

/**
 * Loads a texture via queue_load_texture.
 */
Texture *queue_load_texture_proxy(s32 id);
Texture *texture_load(s32 id, s32 param2);
s32 func_8003DC04(Gfx** arg0, Texture* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_8003E9B4(s32 arg0);
void func_8003E9D0(s32 arg0);
void* func_8003E904(Texture* arg0, s32 arg1);

#endif //_SYS_GFX_TEXTURE_H
