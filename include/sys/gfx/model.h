/** Model system
 */

#ifndef _SYS_GFX_MODEL_H
#define _SYS_GFX_MODEL_H
#include "texture.h"
#include "animation.h"
#include "sys/math.h"

typedef struct
{
/*0000*/    Texture *texture;
/*0004*/    u32 unk_0x4;
} ModelTexture;

typedef struct
{
/*0000*/    u8 unk_0x0;
/*0001*/    u8 unk_0x1[0x10 - 0x1];
} ModelUnk0x8;

typedef struct
{
/*0000*/	u32 unk_0x0;
/*0004*/	f32 x;
/*0008*/	f32 y;
/*000C*/	f32 z;
} ModelUnk0x20;

typedef struct
{
/*0000*/	Gfx gfx;
/*0008*/	Gfx gfx2;
/*0010*/	s16 idx;
/*0012*/	u8 unk_0x12[0x18 - 0x12];
} ModelDLInfo;

typedef struct
{
    // TODO
/*0000*/    ModelTexture *textures;
/*0004*/    void *unk_0x4;
/*0008*/    ModelUnk0x8 *unk_0x8;
/*000C*/    Gfx *displayList;
/*0010*/    Animation **anims;
/*0014*/    void *unk_0x14;
/*0018*/    void *unk_0x18;
/*001C*/    void *unk_0x1c;
/*0020*/    ModelUnk0x20 *unk_0x20;
/*0024*/    u8 *amap;
/*0028*/    void *unk_0x28;
/*002C*/    void *unk_0x2c;
/*0030*/    u32 unk_0x30;
/*0034*/    void *unk_0x34;
/*0038*/    ModelDLInfo *dlInfos;
/*003C*/    void *unk_0x3c;
/*0040*/    u32 unk_0x40;
/*0044*/    u32 unk_0x44;
/*0048*/    u32 unk_0x48;
/*004C*/    u32 unk_0x4c;
/*0050*/    void *unk_0x50;
/*0054*/    void *unk_0x54;
/*0058*/    u32 unk_0x58;
/*005C*/    u32 unk_0x5c;
/*0060*/    u16 unk_0x60;
/*0062*/    s16 unk_0x62;
/*0064*/    u16 unk_0x64;
/*0066*/    s16 animCount;
/*0068*/    s16 unk_0x68;
/*006A*/    s16 modelId;
/*006C*/    s16 displayListLength;
/*006E*/    u8 unk_0x6e;
/*006F*/    u8 unk_0x6f;
/*0070*/    u8 unk_0x70;
/*0071*/    u8 unk_0x71;
/*0072*/    u8 refCount;
/*0073*/    u8 textureCount;
/*0074*/    u8 unk_0x74;
/*0075*/	u8 dlInfoCount;
} Model;

typedef struct
{
    // TODO
/*0000*/    Model *model;
/*0004*/    void *unk_0x4[2];
/*000C*/    MtxF *matrices[2];
/*0014*/    void *unk_0x14;
/*0018*/    Gfx *displayList;
/*001C*/    u8 unk_0x1c[0x28 - 0x1c];
/*0028*/    AnimState *animState0;
/*002C*/    AnimState *animState1;
/*0030*/    u32 unk_0x30;
/*0034*/    u16 unk_0x34;
} ModelInstance;

#endif //_SYS_GFX_MODEL_H
