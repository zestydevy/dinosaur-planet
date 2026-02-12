/** Texture system
 */
#ifndef _SYS_GFX_TEXTURE_H
#define _SYS_GFX_TEXTURE_H
#include "PR/ultratypes.h"
#include "PR/gbi.h"

/** Custom render mode. RM_AA_ZB_XLU_INTER with Z_UPD */
#define	RM_DINO_AA_ZB_XLU_INTER_Z_UPD(clk)					\
	AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | CLR_ON_CVG |	\
	FORCE_BL | ZMODE_INTER |				\
	GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA)
#define G_RM_DINO_AA_ZB_XLU_INTER_Z_UPD RM_DINO_AA_ZB_XLU_INTER_Z_UPD(1)
#define G_RM_DINO_AA_ZB_XLU_INTER_Z_UPD2 RM_DINO_AA_ZB_XLU_INTER_Z_UPD(2)

/** Custom render mode. 
 * For cutouts. No anti-aliasing? Sorta? Doesn't write coverage.
 * Cheaper version of RM_TEX_EDGE?
 */
#define	RM_DINO_ZB_CUTOUT(clk)					\
	AA_EN | Z_CMP | Z_UPD | CVG_DST_SAVE | ZMODE_OPA | CVG_X_ALPHA | \
	GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
#define	G_RM_DINO_ZB_CUTOUT RM_DINO_ZB_CUTOUT(1)
#define	G_RM_DINO_ZB_CUTOUT2 RM_DINO_ZB_CUTOUT(2)

/** Custom render mode. 
 * For cutouts that optionally have pre-antialiased edges in the texture.
 * Cheaper version of RM_AA_TEX_EDGE?
 */
#define	RM_DINO_AA_ZB_CUTOUT(clk)					\
	AA_EN | Z_CMP | Z_UPD | CVG_DST_CLAMP | ZMODE_OPA | CVG_X_ALPHA | \
	GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)
#define	G_RM_DINO_AA_ZB_CUTOUT RM_DINO_AA_ZB_CUTOUT(1)
#define	G_RM_DINO_AA_ZB_CUTOUT2 RM_DINO_AA_ZB_CUTOUT(2)


/**
 * RGB = TEXEL0 * SHADE
 * A   = TEXEL0 * PRIM
 */
#define G_CC_DINO_MODULATERGB_PRIMA TEXEL0, 0, SHADE, 0,  TEXEL0, 0, PRIMITIVE, 0
/**
 * RGB = COMBINED * PRIM
 * A   = COMBINED
 */
#define G_CC_DINO_MODULATE_PRIM_PASS_A2 COMBINED, 0, PRIMITIVE, 0,  0, 0, 0, COMBINED
/**
 * RGB = SHADE * PRIM
 * A   = PRIM
 */
#define G_CC_DINO_SHADEPRIMRGB_PRIMA SHADE, 0, PRIMITIVE, 0,  0, 0, 0, PRIMITIVE
/**
 * RGB = SHADE * PRIM
 * A   = SHADE * PRIM
 */
#define G_CC_DINO_SHADEPRIM SHADE, 0, PRIMITIVE,  0, SHADE, 0, PRIMITIVE, 0
#define G_CC_DINO_SHADEPRIM2 G_CC_DINO_SHADEPRIM
/**
 * RGB = COMBINED * SHADE_ALPHA
 * A   = COMBINED
 */
#define G_CC_DINO_FADERGB2 COMBINED, 0, SHADE_ALPHA, 0,  0, 0, 0, COMBINED
/**
 * RGB = SHADE * SHADE_ALPHA
 * A   = PRIM
 */
#define G_CC_DINO_FADESHADE_PRIMA SHADE, 0, SHADE_ALPHA, 0,  0, 0, 0, PRIMITIVE
/**
 * RGB = (PRIM - COMBINED) * SHADE_ALPHA + COMBINED
 * A   = COMBINED
 */
#define G_CC_DINO_LERP_TO_PRIM2 PRIMITIVE, COMBINED, SHADE_ALPHA, COMBINED,  0, 0, 0, COMBINED
// @bug? cycle 1 mode using COMBINED!?
/**
 * RGB = (PRIM - SHADE) * SHADE_ALPHA + COMBINED
 * A   = PRIM
 */
#define G_CC_DINO_BLEND_PRIMSHADE_MAYBE_BROKEN PRIMITIVE, SHADE, SHADE_ALPHA, COMBINED,  0, 0, 0, PRIMITIVE
/**
 * RGB = COMBINED * SHADE
 * A   = COMBINED * PRIM
 */
#define G_CC_DINO_MODULATERGB_PRIMA2 COMBINED, 0, SHADE, 0,  COMBINED, 0, PRIMITIVE, 0
/**
 * RGB = (TEXEL1 - TEXEL0) * ENVIRONMENT + TEXEL0
 * A   = (TEXEL1 - TEXEL0) * ENVIRONMENT + TEXEL0
 */
#define G_CC_DINO_BLENDTEX_ENV TEXEL1, TEXEL0, ENVIRONMENT, TEXEL0,  TEXEL1, TEXEL0, ENVIRONMENT, TEXEL0
/**
 * RGB = (TEXEL1 - TEXEL0) * SHADE_ALPHA + TEXEL0
 * A   = (TEXEL1 - TEXEL0) * SHADE + TEXEL0
 */
#define G_CC_DINO_BLENDTEX_SHADE_ALPHA TEXEL1, TEXEL0, SHADE_ALPHA, TEXEL0,  TEXEL1, TEXEL0, SHADE, TEXEL0
/**
 * RGB = (ENVIRONMENT - COMBINED) * ENV_ALPHA + COMBINED
 * A   = COMBINED
 */
#define G_CC_DINO_BLEND_ENV_ALPHA2 ENVIRONMENT, COMBINED, ENV_ALPHA, COMBINED,  0, 0, 0, COMBINED
/**
 * RGB = (PRIMITIVE - ENVIRONMENT) * ENV_ALPHA + ENVIRONMENT
 * A   = TEXEL0
 */
#define G_CC_DINO_BLEND_PRIMENV_TEXA PRIMITIVE, ENVIRONMENT, ENV_ALPHA, ENVIRONMENT,  0, 0, 0, TEXEL0
/**
 * RGB = ENVIRONMENT * COMBINED
 * A   = COMBINED * PRIM
 */
#define G_CC_DINO_MODULATE_ENVRGB_PRIMA2 ENVIRONMENT, 0, COMBINED,  0, COMBINED, 0, PRIMITIVE, 0
/**
 * RGB = (ENVIRONMENT - TEXEL0) * SHADE_ALPHA + TEXEL0
 * A   = 1
 */
#define G_CC_DINO_LERP_TO_ENV_1A ENVIRONMENT, TEXEL0, SHADE_ALPHA, TEXEL0,  0, 0, 0, 1
/**
 * RGB = COMBINED * SHADE
 * A   = 1
 */
#define G_CC_DINO_MODULATERGB_1A2 COMBINED, 0, SHADE, 0,  0, 0, 0, 1

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

enum RenderFlags {
    RENDER_NONE,
    RENDER_ANTI_ALIASING = (1 << 0), // 0x1
    RENDER_Z_COMPARE = (1 << 1), // 0x2
    RENDER_SEMI_TRANSPARENT = (1 << 2), // 0x4
    RENDER_FOG_ACTIVE = (1 << 3), // 0x8
    RENDER_UNK10 = (1 << 4), // 0x10
    RENDER_UNK20 = (1 << 5), // 0x20
    RENDER_TEX_BLEND = (1 << 6), // 0x40
    RENDER_CUTOUT = (1 << 7), // 0x80

    RENDER_SUBSURFACE = (1 << 9), // 0x200
    RENDER_DECAL_SIMPLE = (1 << 10), // 0x400

    RENDER_DECAL = (1 << 20), // 0x100000

    RENDER_NO_CULL = (1 << 31) // 0x80000000
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
/*03*/ s8 unk3; // related to images that are split into multiple textures
/*04*/ s8 unk4; // related to images that are split into multiple textures
/*05*/ u8 refCount;
/*06*/ s16 flags; // TextureFlags
/*08*/ Gfx *gdl;
/*0C*/ u16 animDuration;
/*0E*/ u16 animSpeed;
/*10*/ u16 unk10;
/*12*/ s16 gdl2Offset;
/*14*/ struct Texture *next;
/*18*/ s16 unk18; // TODO: usually image data size in bytes (width * height * bytesPerPx)
/*1A*/ u8 unk1A;
/*1B*/ u8 widthHeightHi; // upper 4 bits = width hi, lower 4 bits = height hi
/*1C*/ u8 cms;
/*1D*/ u8 masks;
/*1E*/ u8 cmt;
/*1F*/ u8 maskt;
} Texture; // Size: 0x20, followed by texture data, then followed by its display list

void tex_init(void);
void tex_set_alloc_tag(s32 tag);
/**
 * Loads a texture via the asset thread.
 */
Texture *tex_load_deferred(s32 id);
Texture *tex_load(s32 id, s32 param2);
void tex_free(Texture *texture);
void tex_render_reset(void);
void tex_render_save_state(void);
void tex_render_restore_state(void);
s32 tex_gdl_set_texture_simple(Gfx **gdl, Texture *tex, s32 arg2, s32 arg3, s32 force, s32 arg5);
void tex_gdl_set_textures(Gfx **gdl, Texture *tex0, Texture *tex1, u32 flags, s32 arg5, u32 force, u32 setModes);
void tex_animate(Texture *tex, s32 *, s32 *);
void* tex_get_frame_img(Texture *tex, s32 arg1);
Texture *tex_get_cached(s32 id);
void tex_disable_modes(s32 modes);
void tex_enable_modes(s32 modes);

#endif //_SYS_GFX_TEXTURE_H
