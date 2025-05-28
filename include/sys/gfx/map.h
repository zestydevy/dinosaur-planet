#ifndef _SYS_GFX_MAP_H
#define _SYS_GFX_MAP_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "sys/gfx/texture.h"

#define RGBA8(r, g, b, a) (((u32)(r) << 24) | ((u32)(g) << 16) | ((u32)(b) << 8) | (u32)(a))

typedef struct
{
/*0000*/    s16 uls0;
/*0002*/    s16 ult0;
/*0004*/    u8 unk_0x4[0x10 - 0x4];
/*0010*/    s16 uls1;
/*0012*/    s16 ult1;
/*0014*/    u8 unk_0x14[0x22 - 0x14];
} Struct0x22;

typedef struct
{
/*0000*/    Texture *texture;
/*0004*/    u32 unk_0x4;
/*0008*/    u32 flags;
/*000C*/    s16 refCount;
/*000E*/    u8 unk_0x14;
/*000F*/    u8 unk_0x15;
} BlockTexture; // FIXME: better name

typedef struct
{
/*0000*/    Texture *texture;
/*0004*/    u8 unk_0x4[0xc - 0x4];
} Block_0x0Struct;

typedef struct
{
/*0000*/    s16 texIdx;
/*0002*/    u8 unk_0x2;
/*0003*/    u8 unk_0x3;
} Block_0x28Struct;

typedef struct
{
/*0000*/    u32 flags;
/*0004*/    s16 vtxBase;
/*0006*/    s16 triBase;
/*0008*/    u8 unk_0x8[0x12 - 0x8];
/*0012*/    u8 tileIdx0;
/*0013*/    u8 alpha;
/*0014*/    u8 unk_0x14;
/*0015*/    u8 tileIdx1;
/*0016*/    u8 unk_0x16;
/*0017*/    u8 unk_0x17;
} BlockShape;

typedef struct
{
/*0000*/    u32 d0;
/*0004*/    u32 d1;
} EncodedTri;

typedef struct
{
/*0000*/    Block_0x0Struct *tiles;
/*0004*/    Vtx_t *vertices;
/*0008*/    EncodedTri *encodedTris;
/*000C*/    BlockShape *shapes;
/*0010*/    void *unk_0x10;
/*0014*/    s16 *xzBitmap;
/*0018*/    u8 unk_0x18[0x20 - 0x18];
/*0020*/    Vtx_t *vertices2[2];
/*0028*/    Block_0x28Struct *unk_0x28;
/*002C*/    Gfx *gdlGroups; // In groups of 3 per shape; used to set up materials.
/*0030*/    s16 vtxFlags;
/*0032*/    s16 vtxCount;
/*0034*/    s16 unk_0x34;
/*0036*/    s16 shapeCount;
/*0038*/    u8 unk_0x38[0x3e - 0x38];
/*003E*/    s16 unk_0x3e;
/*0040*/    s16 elevation;
/*0042*/    u16 unk_0x42;
/*0044*/    u16 gdlGroupsOffset;
/*0046*/    u8 unk_0x46;
/*0047*/    u8 unk_0x47;
/*0048*/    u8 unk_0x48;
/*0049*/    u8 unk_0x49;
/*004A*/    u8 textureCount;
} Block;

typedef struct
{
/*0000*/    Gfx combine;
/*0008*/    Gfx otherMode;
/*0010*/    u32 geometryMode;
/*0014*/    u32 primColor;
/*0018*/    u32 envColor;
/*001C*/    u32 fogColor;
/*0020*/    u32 fillColor;
/*0024*/    u32 blendColor;
/*0028*/    u8 needsPipeSync;
/*0029*/    u8 dirtyFlags;
} DLBuilder;

typedef struct
{
/*0000*/    u8 unk_0x0;
/*0001*/    u8 v0;
/*0002*/    u8 v1;
/*0003*/    u8 v2;
/*0004*/    u8 unk_0x4[0x10 - 0x4];
} DLTri;

/** Flags
 * 0x10 - Whether the sky renders
 * 0x80 - Whether shadows render
 * 0x2000 - Freaks out depth testing?
 * 0x10000 - Runs initialize_framebuffers then toggles off, i guess
 * 0x20000 - Spawns an FXEmit object and turns the fog a tan-ish color, renders a different sun??
 */
extern u32 UINT_80092a98;
extern DLBuilder *gDLBuilder;

typedef struct
{
/*0000*/    f32 x;
/*0004*/    f32 y;
/*0008*/    f32 z;
/*000C*/    f32 d;
/*0010*/    u8 unk_0x14[4];
} Plane;

#endif
