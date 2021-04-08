#include "common.h"
#include "video.h"

void possibly_resize_copy_line(u16 *a0, s32 a1, s32 a2, u16 *a3);

#pragma GLOBAL_ASM("asm/nonmatchings/texture/init_textures.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003CD6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003CD7C.s")

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
void _load_texture_to_tmem2(Gfx **gdl, Texture *texture, u32 tile, u32 tmem, u32 palette)
{
    Gfx *mygdl = *gdl;
    s32 fmt;
    s32 siz;
    s32 siz2;
    s32 width;
    s32 height;
    s32 iVar2;
    s32 iVar3;
    s32 line2;

    width = texture->width | (texture->unk_0x1b & 0xf0) << 4;
    height = texture->height | (texture->unk_0x1b & 0xf) << 8;

    switch (texture->format & 0xf)
    {
    case 0:
        siz = 3;
        siz2 = 3;
        iVar2 = 0;
        iVar3 = 0;
        line2 = 2;
        break;
    case 1:
    case 4:
        siz = 2;
        siz2 = 2;
        iVar2 = 0;
        iVar3 = 0;
        line2 = 2;
        break;
    case 2:
    case 5:
        siz = 2;
        siz2 = 1;
        iVar2 = 1;
        iVar3 = 1;
        line2 = 1;
        break;
    default:
        siz = 2;
        siz2 = 0;
        iVar2 = 3;
        iVar3 = 2;
        line2 = 0;
        break;
    }

    switch (texture->format & 0xf)
    {
    case 0:
    case 1:
        fmt = 0;
        if ((texture->format >> 4) == 0 || (texture->format >> 4) == 2) {
            texture->flags |= 0x4;
        }
        break;
    case 4:
    case 5:
    case 6:
        fmt = 3;
        texture->flags |= 0x4;
        break;
    case 7:
        fmt = 2;
        break;
    default:
        fmt = 4;
        break;
    }

    if (siz2 != 0) {
        line2 *= width;
    } else {
        line2 = width / 2;
    }

    if (texture->flags & 0xc000)
    {
        gDPSetTextureImage(mygdl++, fmt, siz, 0, OS_K0_TO_PHYSICAL((u8*)texture + sizeof(Texture)));
        gDPSetTile(mygdl++, fmt, siz, 0, tmem, 7, 0, texture->cmt, texture->maskt, 0, texture->cms, texture->masks, 0);
        gDPLoadSync(mygdl++);
        gDPLoadBlock(mygdl++, 7, 0, 0, ((width * height + iVar2) >> iVar3) - 1, 0);
        gDPPipeSync(mygdl++);
        gDPSetTile(mygdl++, fmt, siz2, (line2 + 7) / 8, tmem, tile, palette, texture->cmt, texture->maskt, 0, texture->cms, texture->masks, 0);
    }
    else
    {
        gDPSetTextureImage(mygdl++, fmt, siz, 0, OS_K0_TO_PHYSICAL((u8*)texture + sizeof(Texture)));
        gDPSetTile(mygdl++, fmt, siz, 0, tmem, 7, 0, texture->cmt, texture->maskt, 0, texture->cms, texture->masks, 0);
        gDPLoadSync(mygdl++);
        gDPLoadBlock(mygdl++, 7, 0, 0, ((width * height + iVar2) >> iVar3) - 1, 0);
        gDPPipeSync(mygdl++);

        if (!(texture->flags & 0x100)) {
            gDPSetTile(mygdl++, fmt, siz2, (line2 + 7) / 8, tmem, tile, palette, texture->cmt, texture->maskt, 0, texture->cms, texture->masks, 0);
            gDPSetTileSize(mygdl++, 0, 0, 0, (width - 1) * 2, (height - 1) * 2);
        }

        if (fmt == 2) {
            gDPLoadTLUT(mygdl++, 0x3c, palette, OS_K0_TO_PHYSICAL((u8*)texture + sizeof(Texture) + width * height / 2));
        }

        if (texture->flags & 0x100)
        {
            if ((texture->format & 0xf) == 0)
            {
                _bcopy(0x80092a00, mygdl, 8 * sizeof(Gfx));
                mygdl += 8;
            }
            else
            {
                Gfx *mygdl2 = mygdl;

                if ((texture->cmt & 0x2) != 0 && (texture->cms & 0x2) == 0) {
                    _bcopy(0x800928c0, mygdl2, 8 * sizeof(Gfx));
                } else if ((texture->cmt & 0x2) == 0 && (texture->cms & 0x2) != 0) {
                    _bcopy(0x80092900, mygdl2, 8 * sizeof(Gfx));
                } else {
                    _bcopy(0x80092880, mygdl2, 8 * sizeof(Gfx));
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
typedef struct
{
/*0000*/    Gfx *gdl0;
/*0004*/    Gfx *gdl1;
/*0008*/    u32 mask;
/*000C*/    u32 flags;
} Foo;
void dl_set_env_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void dl_set_env_color2(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void _set_textures_on_gdl(Gfx **gdl, Texture *tex0, Texture *tex1, u32 flags, s32 level, u32 force, u32 param_7)
{
    Gfx *mygdl;
    Texture *tex0_;
    Texture *tex1_;
    u32 flags_;
    u8 bVar2;
    u8 bVar3;
    Foo *foos;
    s32 cursor;
    s32 uVar4 = 0;
    u32 geomMode;
    u32 uStack12;
    Foo *foo;

    bVar2 = FALSE;
    mygdl = *gdl;

    if (tex0 != NULL)
    {
        s32 levelIdx = level >> 16;
        s32 levelCount;

        if (tex0->unk_0xc != 0) {
            levelCount = tex0->unk_0xc >> 8;
        } else {
            levelCount = 0;
        }

        tex0_ = tex0;
        tex1_ = tex1;

        if (levelCount > 1 && levelIdx < levelCount)
        {
            if (levelIdx > 0 && tex0 != NULL)
            {
                s32 i = 0;
                do {
                    i++;
                    tex1_ = tex1_->next;
                } while (i < levelIdx && tex1_ != NULL);
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
                if (top > 0 && tex0 != NULL)
                {
                    s32 i = 0;
                    do {
                        i++;
                        tex1_ = tex1->next;
                    } while (i < top && tex1_ != NULL);
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

                gSPDisplayList(mygdl++, OS_K0_TO_PHYSICAL(tex1->gdl + tex0_->gdlIdx));
                dl_set_env_color2(&mygdl, lev, lev, lev, 0);
            }
            else if (flags_ & 0x2000)
            {
                dl_set_env_color(&mygdl, 0xa0, 0xa0, 0xa0, 0xa0);
            }
        }

        uVar4 = tex0->flags & 0x100;
        bVar3 = TRUE;
        foos = 0x80092540;
        bVar2 = (tex0->unk_0x2 & 0xf) == 7;
        flags = flags_;
    }
    else
    {
        bVar3 = FALSE;
        foos = 0x800927a0;
    }

    if (param_7 != 0)
    {
        flags_ = flags & ~*(u32*)0x80092a48;
        cursor = (flags_ & 0x70) >> 4;
        if (bVar3)
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
        geomMode = 0x200004;
        if (uStack12 & 0x2) {
            geomMode = 0x200005;
        }
        if (uStack12 & 0x8) {
            geomMode |= 0x10000;
        }
        if (!(flags_ & 0x80000000)) {
            geomMode |= 0x400;
        }

        // gSPGeometryMode
        {
            Gfx *_g = mygdl;

            _g->words.w0 = 0xd9000000;
            _g->words.w1 = geomMode;
        }
        func_80041210(&mygdl);

        *mygdl = foo->gdl0[uStack12 >> 3];
        func_80041040(&mygdl);

        *mygdl = foo->gdl1[uStack12];
        if (bVar2) {
            mygdl->words.w0 |= 0x8000;
        }
        func_80041118(&mygdl);
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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003EBD4.s")
#else
// Functionally equivalent, extremely close to matching
void func_8003EBD4(s32 a0) {
    s32 resEncoded = get_some_resolution_encoded();

    s32 hRes = resEncoded & 0xffff;
    s32 vRes = resEncoded >> 0x10;

    u16 *next = gFramebufferNext;
    u16 *cur = gFramebufferCurrent;

    s32 hOffset = hRes - a0;
    s32 vIndex = vRes;

    while (vIndex--) {
        possibly_resize_copy_line(
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
#pragma GLOBAL_ASM("asm/nonmatchings/texture/possibly_resize_copy_line.s")
#else
void possibly_resize_copy_line(u16 *src, s32 srcWidth, s32 destWidth, u16 *dest) {
    u16 buffer[644];

    s32 var1 = 0;
    u16 *bufferPtr = &buffer[0];
    u16 *srcPtr = src;
    s32 i = 0;

    if (destWidth > 0) {
        do {
            var1 += srcWidth % destWidth;

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

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040378.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_800403AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040468.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040590.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040754.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040870.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040920.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_800409D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040A04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040A94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040B34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040B8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040CD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040EFC.s")

