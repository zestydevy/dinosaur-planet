#include "common.h"
#include "queue.h"
#include "video.h"

// Definitely something like rand_i2
int func_800155C4(int min, int max);
void func_8005324C(u16 *fb1, u16 *fb2, s32);

void weird_resize_copy(u16 *src, s32 srcWidth, s32 destWidth, u16 *dest);

#pragma GLOBAL_ASM("asm/nonmatchings/texture/init_textures.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003CD6C.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/texture/queue_load_texture_proxy.s")
#else
/**
 * Calls queue_load_texture with [param] as the second argument.
 * 
 * Returns the result of queue_load_texture.
 */
s32 queue_load_texture_proxy(s32 param) {
    s32 var1 = 0;
    queue_load_texture(&var1, param);

    return var1;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/texture/texture_load.s")

void load_texture_to_tmem2(Gfx **gdl, Texture *texture, u32 tile, u32 tmem, u32 palette);
Gfx *load_texture_to_tmem(Texture *texture, Gfx *gdl)
{
    Gfx *mygdl;
    u32 tile;
    u32 tmem;

    mygdl = gdl;
    texture->gdl = gdl;

    if (texture->flags & 0x8000) {
        tile = 1;
        tmem = 0x100;
    } else {
        tile = 0;
        tmem = 0;
    }

    load_texture_to_tmem2(&mygdl, texture, tile, tmem, 0);
    texture->gdlIdx = mygdl - texture->gdl;

    if ((texture->flags & 0xc000) == 0 && (texture->flags & 0x40) != 0)
    {
        if ((texture->format & 0xf) == 0) {
            load_texture_to_tmem2(&mygdl, texture, 1, (0x1000 - texture->unk_0x18) >> 3, 0);
        } else if ((texture->format & 0xf) == 7) {
            load_texture_to_tmem2(&mygdl, texture, 1, 0x80, 1);
        } else {
            load_texture_to_tmem2(&mygdl, texture, 1, 0x100, 0);
        }
    }

    return mygdl;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/texture/load_texture_to_tmem2.s")
#else
extern Gfx Gfx_ARRAY_80092880[8]; // wtf why doesn't this work
extern Gfx Gfx_ARRAY_800928c0[8];
extern Gfx Gfx_ARRAY_80092900[8];
extern Gfx Gfx_ARRAY_80092a00[8];
void _load_texture_to_tmem2(Gfx **gdl, Texture *texture, u32 tile, u32 tmem, u32 palette)
{
    Gfx *mygdl = *gdl;
    s32 fmt;
    s32 siz;
    s32 siz2;
    s32 width;
    s32 height;
    s32 sizincr;
    s32 sizshift;
    s32 line2;

    width = texture->width | (texture->unk_0x1b & 0xf0) << 4;
    height = texture->height | (texture->unk_0x1b & 0xf) << 8;

    // This implements a dynamic version of gDPLoadTextureBlock
    switch (texture->format & 0xf)
    {
    case 0:
        siz = G_IM_SIZ_32b;
        siz2 = G_IM_SIZ_32b;
        sizincr = G_IM_SIZ_32b_INCR;
        sizshift = G_IM_SIZ_32b_SHIFT;
        line2 = 2;
        break;
    case 1:
    case 4:
        siz = G_IM_SIZ_16b;
        siz2 = G_IM_SIZ_16b;
        sizincr = G_IM_SIZ_16b_INCR;
        sizshift = G_IM_SIZ_16b_SHIFT;
        line2 = 2;
        break;
    case 2:
    case 5:
        siz = G_IM_SIZ_16b;
        siz2 = G_IM_SIZ_8b;
        sizincr = G_IM_SIZ_8b_INCR;
        sizshift = G_IM_SIZ_8b_SHIFT;
        line2 = 1;
        break;
    default:
        siz = G_IM_SIZ_16b;
        siz2 = G_IM_SIZ_4b;
        sizincr = G_IM_SIZ_4b_INCR;
        sizshift = G_IM_SIZ_4b_SHIFT;
        line2 = 0;
        break;
    }

    switch (texture->format & 0xf)
    {
    case 0:
    case 1:
        fmt = G_IM_FMT_RGBA;
        if ((texture->format >> 4) == 0 || (texture->format >> 4) == 2) {
            texture->flags |= 0x4;
        }
        break;
    case 4:
    case 5:
    case 6:
        fmt = G_IM_FMT_IA;
        texture->flags |= 0x4;
        break;
    case 7:
        fmt = G_IM_FMT_CI;
        break;
    default:
        fmt = G_IM_FMT_I;
        break;
    }

    if (siz2 == G_IM_SIZ_4b) {
        line2 = width / 2;
    } else {
        line2 *= width;
    }

    if (texture->flags & 0xc000)
    {
        gDPSetTextureImage(mygdl++, fmt, siz, 0, OS_K0_TO_PHYSICAL((u8*)texture + sizeof(Texture)));
        gDPSetTile(mygdl++, fmt, siz, 0, tmem, 7, 0, texture->cmt, texture->maskt, 0, texture->cms, texture->masks, 0);
        gDPLoadSync(mygdl++);
        gDPLoadBlock(mygdl++, 7, 0, 0, ((width * height + sizincr) >> sizshift) - 1, 0);
        gDPPipeSync(mygdl++);
        gDPSetTile(mygdl++, fmt, siz2, (line2 + 7) / 8, tmem, tile, palette, texture->cmt, texture->maskt, 0, texture->cms, texture->masks, 0);
    }
    else
    {
        gDPSetTextureImage(mygdl++, fmt, siz, 0, OS_K0_TO_PHYSICAL((u8*)texture + sizeof(Texture)));
        gDPSetTile(mygdl++, fmt, siz, 0, tmem, 7, 0, texture->cmt, texture->maskt, 0, texture->cms, texture->masks, 0);
        gDPLoadSync(mygdl++);
        gDPLoadBlock(mygdl++, 7, 0, 0, ((width * height + sizincr) >> sizshift) - 1, 0);
        gDPPipeSync(mygdl++);

        if (!(texture->flags & 0x100)) {
            gDPSetTile(mygdl++, fmt, siz2, (line2 + 7) / 8, tmem, tile, palette, texture->cmt, texture->maskt, 0, texture->cms, texture->masks, 0);
            gDPSetTileSize(mygdl++, 0, 0, 0, (width - 1) * 2, (height - 1) * 2);
        }

        if (fmt == G_IM_FMT_CI) {
            gDPLoadTLUT(mygdl++, 0x3c, palette, OS_K0_TO_PHYSICAL((u8*)texture + sizeof(Texture) + width * height / 2));
        }

        if (texture->flags & 0x100)
        {
            if ((texture->format & 0xf) == 0)
            {
                _bcopy(Gfx_ARRAY_80092a00, mygdl, 8 * sizeof(Gfx));
                mygdl += 8;
            }
            else
            {
                Gfx *mygdl2 = mygdl;

                if ((texture->cmt & 0x2) != 0 && (texture->cms & 0x2) == 0) {
                    _bcopy(Gfx_ARRAY_800928c0, mygdl2, 8 * sizeof(Gfx));
                } else if ((texture->cmt & 0x2) == 0 && (texture->cms & 0x2) != 0) {
                    _bcopy(Gfx_ARRAY_80092900, mygdl2, 8 * sizeof(Gfx));
                } else {
                    _bcopy(Gfx_ARRAY_80092880, mygdl2, 8 * sizeof(Gfx));
                }

                mygdl += 8;
                
                mygdl2[0].words.w1 &= ~0xc0000;
                mygdl2[0].words.w1 |= (texture->cmt & 0x3) << 18;
                mygdl2[0].words.w1 &= ~0x300;
                mygdl2[0].words.w1 |= (texture->cms & 0x3) << 8;

                mygdl2[2].words.w1 &= ~0xc0000;
                mygdl2[2].words.w1 |= (texture->cmt & 0x3) << 18;
                mygdl2[2].words.w1 &= ~0x300;
                mygdl2[2].words.w1 |= (texture->cms & 0x3) << 8;

                mygdl2[4].words.w1 &= ~0xc0000;
                mygdl2[4].words.w1 |= (texture->cmt & 0x3) << 18;
                mygdl2[4].words.w1 &= ~0x300;
                mygdl2[4].words.w1 |= (texture->cms & 0x3) << 8;

                mygdl2[6].words.w1 &= ~0xc0000;
                mygdl2[6].words.w1 |= (texture->cmt & 0x3) << 18;
                mygdl2[6].words.w1 &= ~0x300;
                mygdl2[6].words.w1 |= (texture->cms & 0x3) << 8;
            }
        }
    }

    gSPEndDisplayList(mygdl++);

    *gdl = mygdl;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/texture/texture_destroy.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003DB5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003DB7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003DBCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003DC04.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/texture/set_textures_on_gdl.s")
#else
extern Texture *gCurrTex0;
extern Texture *gCurrTex1;
extern u32 UINT_80092a48;
typedef struct
{
/*0000*/    Gfx *combines;
/*0004*/    Gfx *otherModes;
/*0008*/    u32 mask;
/*000C*/    u32 flags;
} Foo;
void dl_set_env_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void dl_set_env_color2(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void _set_textures_on_gdl(Gfx **gdl, Texture *tex0, Texture *tex1, u32 flags, s32 level, u32 force, u32 setModes)
{
    Gfx *mygdl;
    Texture *tex0_;
    Texture *tex1_;
    s32 flags_;
    u8 isIndexed;
    u8 isTextured;
    Foo *foos;
    s32 cursor;
    s32 uVar4 = 0;
    u32 geomMode;
    s32 uStack12;
    Foo *foo;

    isIndexed = FALSE;
    mygdl = *gdl;

    if (tex0 != NULL)
    {
        s32 levelIdx = level >> 16;
        s32 levelCount;

        if (tex0->levels != 0) {
            levelCount = tex0->levels >> 8;
        } else {
            levelCount = 0;
        }

        tex0_ = tex0;
        tex1_ = tex1;

        if (levelCount > 1 && levelIdx < levelCount)
        {
            s32 i;
            
            for (i = 0; i < levelIdx && tex1_ != NULL; i++) {
                tex1_ = tex1_->next;
            }

            if (tex1_ != NULL) {
                tex0_ = tex1_;
            }
            tex1_ = tex0_;

            if (flags & 0x40)
            {
                s32 top;

                if (flags & 0x80000)
                {
                    top = levelIdx - 1;
                    if (top < 0)
                    {
                        if (flags & 0x40000) {
                            top = levelIdx + 1;
                        } else {
                            top = 0;
                        }
                    }
                }
                else
                {
                    top = levelIdx + 1;
                    if (top >= levelCount)
                    {
                        if (flags & 0x40000) {
                            top = levelCount - 1;
                        } else {
                            top = levelIdx - 1;
                        }
                    }
                }

                tex1_ = tex0;

                for (i = 0; i < top && tex1_ != NULL; i++) {
                    tex1_ = tex1_->next;
                }

                if (tex1_ == NULL) {
                    tex1_ = tex0;
                }
            }
        }

        flags_ = flags | (tex0->flags & 0xfebf);
        if (tex0_ != gCurrTex0 || tex1_ != gCurrTex1 || force)
        {
            gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(tex0_->gdl));

            gCurrTex0 = tex0_;
            gCurrTex1 = tex1_;

            if (tex1 != NULL)
            {
                gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(tex1_->gdl));
                dl_set_env_color(&mygdl, 0x7f, 0x7f, 0x7f, 0x7f);
            }
            else if ((tex0->flags & 0x40) && (flags & 0x40))
            {
                u8 lev = level >> 8;

                gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(tex1_->gdl + tex0_->gdlIdx));
                dl_set_env_color2(&mygdl, lev, lev, lev, 0);
            }
            else if (flags_ & 0x2000)
            {
                dl_set_env_color(&mygdl, 0xa0, 0xa0, 0xa0, 0xa0);
            }
        }

        uVar4 = tex0->flags & 0x100;
        isTextured = TRUE;
        foos = 0x80092540;
        isIndexed = (tex0->format & 0xf) == 7;
        flags = flags_;
    }
    else
    {
        isTextured = FALSE;
        foos = 0x800927a0;
    }

    if (setModes)
    {
        flags_ = flags & ~UINT_80092a48;
        cursor = (flags_ & 0x70) >> 4;
        if (isTextured)
        {
            if (flags_ & 0x400)
            {
                if (uVar4 != 0) {
                    cursor = 0x23;
                } else {
                    cursor = 0x22;
                }
            }
            else
            {
                if (flags_ & 0x20000)
                {
                    cursor += 8;
                }
                else
                {
                    if (flags_ & 0x80)
                    {
                        cursor += 0x10;
                    }
                    else if (flags_ & 0x200)
                    {
                        if (uVar4 != 0) {
                            cursor = 0x21;
                        } else {
                            cursor += 0x18;
                        }
                    }
                    else
                    {
                        if (uVar4) {
                            cursor = 0x20;
                        }
                    }
                }
            }
        }

        foo = foos + cursor;
        uStack12 = foo->flags | (flags_ & foo->mask);
        geomMode = G_SHADE | G_SHADING_SMOOTH;
        if (uStack12 & 0x2) {
            geomMode |= G_ZBUFFER;
        }
        if (uStack12 & 0x8) {
            geomMode |= G_FOG;
        }
        if (!(flags_ & 0x80000000)) {
            geomMode |= G_CULL_BACK;
        }

        gSPLoadGeometryMode(mygdl, geomMode);
        dl_apply_geometry_mode(&mygdl);

        mygdl->words.w0 = foo->combines[uStack12 >> 3].words.w0;
        mygdl->words.w1 = foo->combines[uStack12 >> 3].words.w1;
        dl_apply_combine(&mygdl);

        mygdl->words.w0 = foo->otherModes[uStack12].words.w0;
        mygdl->words.w1 = foo->otherModes[uStack12].words.w1;
        if (isIndexed) {
            mygdl->words.w0 |= G_TT_RGBA16;
        }
        dl_apply_other_mode(&mygdl);
    }

    *gdl = mygdl;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003E648.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003E904.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003E960.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003E9B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003E9D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003E9F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003EAC0.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003EBD4.s")
#else
/**
 * Does some sort of copy line-by-line from gFramebufferNext to gFramebufferCurrent.
 * 
 * Also see weird_resize_copy(), which this function uses to copy data.
 */
void func_8003EBD4(s32 hOffset) {
    s32 resEncoded = get_some_resolution_encoded();

    s32 hRes = resEncoded & 0xffff;
    s32 vRes = resEncoded >> 0x10;

    u16 *next = gFramebufferNext;
    u16 *cur = gFramebufferCurrent;

    s32 vIndex = vRes;

    hOffset = hRes - hOffset;

    while (vIndex--) {
        weird_resize_copy(
            next + hOffset, 
            hRes - (hOffset << 1), 
            hRes, 
            cur
        );

        next += hRes;
        cur += hRes;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003EC8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003ED00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003EF30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003F074.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/texture/weird_resize_copy.s")
#else
/**
 * Copies data from src to dest+destWidth taking into account differences in width
 * (e.g. if destWidth is half srcWidth, every other value will be copied from src).
 * 
 * Additionally, values copied to dest have the following applied first:
 *   (value & -0x843) >> 1
 * 
 * destWidth must be less than 644.
 * 
 * NOTE: Please see the note in the implementation, this function also reads 
 * undefined stack memory, which affects the copied values.
 */
void weird_resize_copy(u16 *src, s32 srcWidth, s32 destWidth, u16 *dest) {
    u16 buffer[644];

    s32 var1 = 0;
    u16 *bufferPtr = &buffer[0];
    u16 *srcPtr = src;
    s32 i = 0;

    if (destWidth > 0) {
        do {
            var1 += srcWidth % destWidth;

            // NOTE: This reads undefined stack memory!
            *bufferPtr = ((*srcPtr & -0x843) >> 1) + ((*bufferPtr & -0x843) >> 1);
            bufferPtr = bufferPtr + 1;

            while (destWidth < var1) {
                var1 = var1 - destWidth;
                srcPtr = srcPtr + 1;
            }

            i = i + 1;
            srcPtr += srcWidth / destWidth;
        } while (i != destWidth);
    }

    _bcopy(&buffer[0], destWidth + dest, destWidth << 1);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003F2C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/NOTosSetTime.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003F4C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003F660.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003F928.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003FA88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003FB88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003FC3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003FCBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003FD48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003FE70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_800402F8.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040378.s")
#else
void func_80040378(u16 *fb1, u16 *fb2, s32 width, s32 height) {
    func_8005324C(fb1, fb2, width * height >> 2);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_800403AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040468.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040590.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040754.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040870.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040920.s")
#else
// regalloc
void _func_80040920(u16 *tex, s32 width, s32 height, int count) {
    int i = 0;
    s32 offset1;
    s32 offset2;

    if (count > 0) {
        do {
            offset1 = func_800155C4(0, width);
            offset2 = func_800155C4(0, height);
            // Sets alpha to 0
            tex[offset2 * width + offset1] &= 0xFFFFFFFE;
        } while (++i != count);
    }
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_800409D0.s")
#else
void func_800409D0(u16 *fb, s32 width, s32 height) {
    // Sets center pixel alpha to 0
    fb[(width >> 1) + (height >> 1) * width] &= 0xFFFE;
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040A04.s")
#else
// regalloc
void _func_80040A04(u16 *tex, s32 width, s32 height, int count) {
    int i = 0;
    s32 offset;

    if (count > 0) {
        do {
            offset = func_800155C4(0, height) * width;
            // Sets alpha to 0
            tex[offset] &= 0xFFFFFFFE;
        } while (++i != count);
    }
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040A94.s")
#else
// regalloc
void _func_80040A94(u16 *tex, int width, int height, int count) {
    int i = 0;

    if (count > 0) {
        do {
            // Sets alpha to 0
            tex[width + (func_800155C4(0, height) * width)] &= 0xFFFFFFFE;
        } while (++i != count);
    }
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/texture/show_framebuffer_corners_kinda.s")
#else
/**
 * Sets the alpha channel of the following corners of the given buffer to 0:
 * X----------*X
 * |          |
 * |          |
 * |          |
 * *----------*
 * X           X
 * 
 * Alternatively:
 * X----------*
 * X          |
 * |          |
 * |          |
 * *----------*
 * X
 * X
 */
void show_framebuffer_corners_kinda(u16 *framebuffer, s32 width, s32 height) {
    // Top right + 1???
    (framebuffer + width)[0] &= -2;

    // Top left
    framebuffer[0] &= -2;

    // Bottom right + extra row???
    (framebuffer + width)[height * width] &= -2;

    // Bottom right + 1???
    framebuffer[height * width] &= -2;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040B8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040CD0.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040EFC.s")
#else
// Should be functionally equivalent, assembly is far off due to a loop being duplicated...
void _func_80040EFC(u16 *fb1, u16 *fb2, s32 width, s32 height) {
    s32 i = 0;
    s32 fbLength = width * height;
    s32 temp2 = fbLength & 3;

    if (fbLength > 0) {
        if (temp2 != 0) {
            // This loop should not unroll and shouldn't be duplicated, 
            // but currently does get duplicated?
            while (i++ != temp2) {
                fb1[i] |= 1;
                fb2[i] |= 1;
            }

            // This might be part of the next loop
            if (i == fbLength) {
                return;
            }
        }

        // This loop needs to unroll
        do {
            fb1[i] |= 1;
            fb2[i] |= 1;
        } while (++i != fbLength);
    }
}
#endif
