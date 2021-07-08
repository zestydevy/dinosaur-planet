/** Animation system
 */
#ifndef _SYS_GFX_ANIMATION_H
#define _SYS_GFX_ANIMATION_H

typedef struct
{
/*0000*/    u8 unk_0x0;
/*0001*/	u8 unk_0x1;
/*0002*/	s16 unk_0x2;
} Animation;

typedef struct
{
/*0000*/    u32 unk_0x0;
/*0004*/    f32 unk_0x4[2];
/*000C*/    u32 unk_0xc;
/*0010*/    u32 unk_0x10;
/*0014*/    f32 unk_0x14[2];
/*001C*/    u8 *anims[2]; // Each item contains 0x80 bytes of amap data, followed by an Animation structure
/*0024*/    u8 *anims2[2];
/*002C*/	u8 *unk_0x2c[2];
/*0034*/    u8 *unk_0x34[2];
/*003C*/    u8 unk_0x3c[0x44 - 0x3c];
/*0044*/    u16 animIndexes[2];
/*0048*/	u32 unk_0x48;
/*004C*/	u16 unk_0x4c[2][3];
/*0058*/    s16 unk_0x58;
/*005A*/	s16 unk_0x5a;
/*005C*/	s16 unk_0x5c;
/*005E*/	u16 unk_0x5e;
/*0060*/    s8 unk_0x60[2];
/*0062*/    u8 unk_0x62;
/*0063*/    s8 unk_0x63;
} AnimState;

#endif //_SYS_GFX_ANIMATION_H
