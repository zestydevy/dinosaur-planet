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
#define TEX_FORMAT2(x) ((x >> 4) & 0xF)

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

enum RenderFlags {
       RENDER_NONE = 0x0,
/*0*/  RENDER_ANTI_ALIASING = 0x1,
/*1*/  RENDER_Z_COMPARE = 0x2,
/*2*/  RENDER_SEMI_TRANSPARENT = 0x4,
/*3*/  RENDER_FOG_ACTIVE = 0x8,
/*4*/  RENDER_UNK10 = 0x10,
/*5*/  RENDER_UNK20 = 0x20,
/*6*/  RENDER_TEX_BLEND = 0x40, // lerp tile 0 -> tile 1
/*7*/  RENDER_CUTOUT = 0x80, // includes billboards
/*8*/  RENDER_MIPMAPS = 0x100,
/*9*/  RENDER_SUBSURFACE = 0x200,
/*10*/ RENDER_DECAL_SIMPLE = 0x400,

/*13*/ RENDER_UNK2000 = 0x2000,
/*14*/ RENDER_COMPOSITE_BASE = 0x4000,
/*15*/ RENDER_COMPOSITE_OVERLAY = 0x8000,
/*16*/ RENDER_UNK10000 = 0x10000,
/*17*/ RENDER_TEXANIM_RANDOM_PINGPONG = 0x20000,
/*18*/ RENDER_TEXANIM_PINGPONG = 0x40000,
/*19*/ RENDER_TEXANIM_REVERSE = 0x80000,
/*20*/ RENDER_DECAL = 0x100000,

/*24*/ RENDER_UNK1000000 = 0x1000000,

/*28*/ RENDER_UNK10000000 = 0x10000000,

/*30*/ RENDER_UNK_40000000 = 0x40000000,
/*31*/ RENDER_NO_CULL = 0x80000000
};

// Option flags for tex_gdl_set_texture_simple
enum TexSetOptions {
    TEXOPT_INVISIBLE = 0x0,
    TEXOPT_SET_MODES = 0x1, // Update geometry, combine, and other modes
    TEXOPT_VISIBLE = 0x2, // Whether the texture is visible
    TEXOPT_SKIP_MODE_CACHE = 0x4 // Ignore cached modes from DLBuilder
};
// frameOptions builders for tex_gdl_set_texture*
#define TEX_FRAME(frame) (frame << 16)
#define TEX_FRAMEBLEND(frame, blendProgress) ((frame << 16) | blendProgress)

// Size: 0x20, followed by pixel data, palette (if present), then by its display list(s)
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
/*03*/ s8 spriteX; // related to images that are split into multiple textures
/*04*/ s8 spriteY; // related to images that are split into multiple textures
/*05*/ u8 refCount;
/*06*/ s16 flags; // RenderFlags (note: textures only specify a subset of these)
/*08*/ Gfx *gdl;
/*0C*/ u16 animDuration; // frame count * 256
/*0E*/ u16 animSpeed; // frames per 60th of a second * 256
/*10*/ u16 unk10;
/*12*/ s16 gdl2Offset; // offset (in 64-bit words) from gdl to the second gdl that places the texture in tile 1
/*14*/ struct Texture *next;
       // The image data size in bytes (width * height * bytesPerPx), 16-byte aligned. 
       // Does not include the Texture header or palette. Does include all mipmap levels.
/*18*/ s16 sizeBytes;
/*1A*/ u8 unk1A;
/*1B*/ u8 widthHeightHi; // upper 4 bits = width hi, lower 4 bits = height hi
/*1C*/ u8 cms;
/*1D*/ u8 masks;
/*1E*/ u8 cmt;
/*1F*/ u8 maskt;
} Texture;

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
/**
 * @param tex - Texture to draw (or NULL to unset the texture (TEXOPT_SET_MODES required to unset tex))
 * @param renderFlags - RenderFlags enum
 * @param frameOptions - Upper 16-bits = frame to draw, lower 16-bits = blend progress
 * @param force - Add texture DL to main DL even if the last texture(s) set on the main DL are the same
 * @param options - TexSetOptions enum
 */
s32 tex_gdl_set_texture_simple(Gfx **gdl, Texture *tex, s32 renderFlags, s32 frameOptions, s32 force, s32 options);
/**
 * @param tex0 - Base texture to draw (or NULL to unset the texture (setModes required to unset tex))
 * @param tex1 - Overlay texture to draw (if not blending, this will be drawn at half opacity overlaid)
 * @param renderFlags - RenderFlags enum
 * @param frameOptions - Upper 16-bits = frame to draw, lower 16-bits = blend progress
 * @param force - Add texture DL to main DL even if the last texture(s) set on the main DL are the same
 * @param setModes - If true, update geometry, combine, and other modes
 */
void tex_gdl_set_textures(Gfx **gdl, Texture *tex0, Texture *tex1, u32 renderFlags, s32 frameOptions, u32 force, u32 setModes);
void tex_animate(Texture *tex, s32 *, s32 *);
void* tex_get_frame_img(Texture *tex, s32 arg1);
Texture *tex_get_cached(s32 id);
void tex_disable_modes(s32 modes);
void tex_enable_modes(s32 modes);

#endif //_SYS_GFX_TEXTURE_H
