#include "common.h"
#include "sys/rarezip.h"
#include "PR/gbi.h"

void func_8005324C(u16 *fb1, u16 *fb2, s32);

void weird_resize_copy(u16 *src, s32 srcWidth, s32 destWidth, u16 *dest);
void load_texture_to_tmem2(Gfx **gdl, Texture *texture, u32 tile, u32 tmem, u32 palette);
void dl_set_env_color_no_sync(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void func_8003EC8C(u16*, u16 *, s32);
void func_8003F2C4(u16*, u16, u16, s32);
u16 func_8003FD48(u16, u16, u16, u16);
u16 func_8003FB88(u16, u16, u16, u16);

typedef struct Unk800B49A8 {
    s32 unk0;
    Texture *unk4;
} Unk800B49A8;
extern Unk800B49A8* D_800B49A8;
extern s32 D_800B49B0;
extern s32 D_800B49B8[2];
extern void *D_800B49C0 ;
extern s32* gFile_TEX0_TAB[];
extern void* gFile_TEX1_TAB;
extern void* gFile_TEXTABLE;
extern s32 D_80092A40;
extern s32 UINT_80092a48;
extern Texture *gCurrTex0;
extern Texture *gCurrTex1;
extern Texture *D_800B49CC;
extern Texture *D_800B49D0;
extern s32 D_800B49D4;
extern s32 D_800B49D8;
extern s8 D_800B49DC;

extern s32 D_80092A50;
extern s32 D_80092A54[3];
extern s32 D_800B49E0;

void init_textures(void) {
    s32 var_v1;
    s32* temp_a1;
    s32 i;

    D_800B49A8 = mmAlloc(0x15E0, 6, NULL);
    D_800B49B0 = 0;
    queue_alloc_load_file((void **) &gFile_TEX0_TAB, 0x28);
    queue_alloc_load_file(&gFile_TEX1_TAB, 0x25);
    queue_alloc_load_file(&gFile_TEXTABLE, 0x26);
    for (i = 0; i < 2; i++) {
        temp_a1 = gFile_TEX0_TAB[i];
        for (var_v1 = 0; temp_a1[var_v1] != -1; var_v1++) {}
        D_800B49B8[i] = (var_v1 - 1);
    }
    D_800B49C0 = mmAlloc(0x108, 6, NULL);
}

void func_8003CD6C(s32 arg0) {
    D_80092A40 = arg0;
}

Texture *queue_load_texture_proxy(s32 id) {
    Texture *texture = NULL;
    queue_load_texture(&texture, id);

    return texture;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/texture/texture_load.s")
#else
// https://decomp.me/scratch/Uk770
Texture* texture_load(s32 id, s32 param2) {
    u32 sp74;
    s32 sp68;
    u32 sp58;
    Texture* sp44;
    Texture* sp40;
    Unk800B49A8* temp_v1;
    s32 temp_s4;
    s32 temp_s5;
    s32 temp_s5_2;
    s32 temp_s7;
    u8 temp_t0;
    u16 temp_t4;
    s32 temp_t7;
    s32 temp_t8;
    Texture *temp_v0;
    s32 temp_v0_4;
    s32 temp_v1_2;
    s32 temp_v1_4;
    s32 var_a0;
    s32 var_a2;
    s32 var_s1;
    s32 var_s6;
    s32 i;
    s32 var_v0_3;
    s32 var_v0_4;
    Unk800B49A8* temp_v0_2;
    u32 temp_s0;
    u8* temp_s3;
    Texture* temp_v0_3;
    u8* var_v0;
    Unk800B49C0* temp_v1_3;

    for (i = 0; i < D_800B49B0; i++) {
        temp_v1 = &D_800B49A8[i];
        if (id == temp_v1->unk0) {
            temp_v0 = temp_v1->unk4;
            ((u8*)temp_v0)[5]++;
            return temp_v0;
        }
    }
    sp68 = id;
    if (id < 0) {
        sp68 = -id;
    } else {
        sp68 = gFile_TEXTABLE[id];
    }
    temp_t4 = sp68 & 0xFFFF;
    var_a0 = temp_t4;
    if (temp_t4 & 0x8000) {
        var_v0_3 = 1;
        sp74 = 0x24;
        var_a0 = temp_t4 & 0x7FFF;
    } else {
        var_v0_3 = 0;
        sp74 = 0x27;
    }
    temp_v0_2 = (Unk800B49A8 *)&gFile_TEX0_TAB[var_v0_3][var_a0];
    temp_v1_2 = temp_v0_2->unk0;
    temp_t0 = temp_v1_2 >> 0x18;
    temp_s0 = temp_v1_2 & 0xFFFFFF;
    temp_s5 = ((u32)temp_v0_2->unk4 & 0xFFFFFF) - temp_s0;
    if (temp_t0 >= 2) {
        read_file_region(sp74, D_800B49C0, temp_s0, (temp_t0 + 1) * 8);
    } else {
        D_800B49C0->unk0 = 0;
        D_800B49C0->unk4 = rarezip_uncompress_size_rom(sp74, temp_s0, 1);
        D_800B49C0->unk8 = temp_s5;
    }
    sp44 = NULL;
    sp40 = NULL;
    for (var_s1 = 0; var_s1 < temp_t0; var_s1++) {
        temp_s4 = D_800B49C0[var_s1].unk4;
        temp_s7 = temp_s4 + 0xE4;
        temp_s5_2 = (u32)D_800B49C0[var_s1+1].unk0 - (u32)D_800B49C0[var_s1].unk0;
        sp44 = mmAlloc(temp_s7, D_80092A40, NULL);
        if (sp44 == NULL) {
            var_s1++;
            if ((var_s1) == 1) {
                return NULL;
            }
            var_s1 = temp_t0 + 1;
            sp44->levels = (temp_t0 << 8);
        } else {
            temp_v0_4 = (((u32) &sp44[temp_s4]) - temp_s5_2) + 0xE4;
            temp_s3 = temp_v0_4 - (temp_v0_4 % 16);
            read_file_region(sp74, temp_s3, D_800B49C0[var_s1].unk0 + temp_s0, temp_s5_2);
            rarezip_uncompress(temp_s3, (u8*)sp44, temp_s4);
            sp44->next = 0;
            if (sp40 != NULL) {
                sp40->next = sp44;
            }
            sp40 = sp44;
            if (var_s1 == 0) {
                sp44->levels = temp_t0 << 8;
            } else {
                sp44->levels = 1;
            }
            sp44->unk_0x10 = (u32) temp_s7 >> 2;
            mmRealloc(sp44, (load_texture_to_tmem(sp44, mmAlign16((u32) &sp44[temp_s4])) - (u32)sp44) + 1, 0);
        }
    }
    for (i = 0; i < D_800B49B0; i++) {
        temp_v1 = &D_800B49A8[i];
        if (temp_v1->unk0 == -1) {
            break;
        }
    }
    if (i == D_800B49B0) {
        D_800B49B0 += 1;
    }
    D_800B49A8[i].unk0 = id;
    D_800B49A8[i].unk4 = sp44;
    if (D_800B49B0 >= 0x2BD) {
        return NULL;
    }
    return sp44;
}
#endif

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
        if (TEX_FORMAT(texture->format) == TEX_FORMAT_RGBA32) {
            load_texture_to_tmem2(&mygdl, texture, 1, (0x1000 - texture->unk_0x18) >> 3, 0);
        } else if (TEX_FORMAT(texture->format) == TEX_FORMAT_CI4) {
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
    switch (TEX_FORMAT(texture->format))
    {
    case TEX_FORMAT_RGBA32:
        siz = G_IM_SIZ_32b;
        siz2 = G_IM_SIZ_32b;
        sizincr = G_IM_SIZ_32b_INCR;
        sizshift = G_IM_SIZ_32b_SHIFT;
        line2 = 2;
        break;
    case TEX_FORMAT_RGBA16:
    case TEX_FORMAT_IA16:
        siz = G_IM_SIZ_16b;
        siz2 = G_IM_SIZ_16b;
        sizincr = G_IM_SIZ_16b_INCR;
        sizshift = G_IM_SIZ_16b_SHIFT;
        line2 = 2;
        break;
    case TEX_FORMAT_I8:
    case TEX_FORMAT_IA8:
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

    switch (TEX_FORMAT(texture->format))
    {
    case TEX_FORMAT_RGBA32:
    case TEX_FORMAT_RGBA16:
        fmt = G_IM_FMT_RGBA;
        if ((texture->format >> 4) == 0 || (texture->format >> 4) == 2) {
            texture->flags |= 0x4;
        }
        break;
    case TEX_FORMAT_I8:
    case TEX_FORMAT_I4:
    case TEX_FORMAT_IA16:
        fmt = G_IM_FMT_IA;
        texture->flags |= 0x4;
        break;
    case TEX_FORMAT_CI4:
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
            if (TEX_FORMAT(texture->format) == TEX_FORMAT_RGBA32)
            {
                bcopy(Gfx_ARRAY_80092a00, mygdl, 8 * sizeof(Gfx));
                mygdl += 8;
            }
            else
            {
                Gfx *mygdl2 = mygdl;

                if ((texture->cmt & 0x2) != 0 && (texture->cms & 0x2) == 0) {
                    bcopy(Gfx_ARRAY_800928c0, mygdl2, 8 * sizeof(Gfx));
                } else if ((texture->cmt & 0x2) == 0 && (texture->cms & 0x2) != 0) {
                    bcopy(Gfx_ARRAY_80092900, mygdl2, 8 * sizeof(Gfx));
                } else {
                    _(Gfx_ARRAY_80092880, mygdl2, 8 * sizeof(Gfx));
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

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/texture/texture_destroy.s")
#else
void texture_destroy(Texture* texture) {
    Texture* temp_s1;
    Texture* var_s0;
    s32 i;

    if (texture == NULL) {
        return;
    }

    ((u8*)texture)[5]--;
    if ((((u8*)texture)[5]) > 0) {
        return;
    }

    for (i = 0; i < D_800B49B0; i++) {
        if (texture == (D_800B49A8 + i)->unk4) {
            var_s0 = texture->next;
            while (var_s0 != NULL) {
                if (((u32) var_s0 < 0x80000000U) || ((u32) var_s0 >= 0xA0000000U)) {
                    var_s0 = NULL;
                } else {
                    temp_s1 = var_s0->next;
                    mmFree(var_s0);
                    var_s0 = temp_s1;
                }
            }
            mmFree(texture);
            (D_800B49A8 + i)->unk0 = -1;
            (D_800B49A8 + i)->unk4 = -1;
            return;
        }
    }
}
#endif

void func_8003DB5C(void) {
    UINT_80092a48 = 0;
    gCurrTex0 = 0;
    gCurrTex1 = 0;
}

void func_8003DB7C(void) {
    D_800B49D4 = UINT_80092a48;
    D_800B49CC = gCurrTex0;
    D_800B49D0 = gCurrTex1;
    UINT_80092a48 = 0;
    gCurrTex0 = 0;
    gCurrTex1 = 0;
}

void func_8003DBCC(void) {
    UINT_80092a48 = D_800B49D4;
    gCurrTex0 = D_800B49CC;
    gCurrTex1 = D_800B49D0;
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003DC04.s")
#else
// https://decomp.me/scratch/fpYm1
s32 func_8003DC04(Gfx** arg0, Texture* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 pad_sp84;
    Texture* var_a3;
    s32 var_a2; // sp7c
    Texture* var_t0_2;
    Texture* var_v1;
    s32 temp_t4_3;
    s32 var_a1;
    s32 var_a0;
    s32 var_v0;
    s32 pad_sp60;
    struct PointersInts* var_a3_2;
    s32 var_v1_3;
    Gfx* sp54;
    s16 temp_v1;
    Gfx *temp_gdl;
    s32 var_t0; // sp48
    s32 temp;
    s32 pad[2];

    sp54 = *arg0;
    D_800B49D8 = 0;
    if (arg5 & 2) {
        D_800B49DC = 0;
        var_t0 = 0;
        if (arg1 != NULL) {
            var_a0 = arg3 >> 0x10;
            var_a2 = arg1->levels != 0 ? arg1->levels >> 8 : 0;
            var_a3 = arg1;
            var_t0_2 = arg1;
            if (var_a2 >= 2 && var_a0 < var_a2) {
                for (var_v1 = arg1, var_v0 = 0; var_v0 < var_a0 && var_v1 != NULL; var_v0++) {
                    var_v1 = var_v1->next;
                }
                if (var_v1 != NULL) {
                    var_a3 = var_v1;
                }
                if (arg2 & 0x40) {
                    if (arg2 & 0x80000) {
                        var_a0--;
                        if (var_a0 < 0) {
                            if (arg2 & 0x40000) {
                                var_a0 += 2;
                            } else {
                                var_a0 = 0;
                            }
                        }
                    } else {
                        var_a0++;
                        if (var_a0 >= var_a2) {
                            if (arg2 & 0x40000) {
                                var_a0 -= 2;
                            } else {
                                var_a0 = var_a2 - 1;
                            }
                        }
                    }
                    for (var_v1 = arg1, var_v0 = 0; var_v0 < var_a0 && var_v1 != NULL; var_v0++) {
                        var_v1 = var_v1->next;
                    }
                    if (var_v1 != NULL) {
                        var_t0_2 = var_v1;
                    }
                } else {
                    var_t0_2 = var_a3;
                }
            }
            temp_v1 = (arg1->flags & 0xFEBF);
            arg2 = temp_v1 | arg2;
            if ((var_a3 != gCurrTex0) || (var_t0_2 != gCurrTex1) || (arg4 != 0)) {
                gCurrTex0 = var_a3;
                gCurrTex1 = var_t0_2;
                temp_gdl = var_a3->gdl;
                gSPDisplayList(sp54++, OS_PHYSICAL_TO_K0(temp_gdl));
                if (arg1->flags & 0x40 && arg2 & 0x40) {
                    temp_gdl = var_t0_2->gdl;
                    gSPDisplayList(sp54++, OS_PHYSICAL_TO_K0(temp_gdl));
                    dl_set_env_color_no_sync(&sp54, (arg3 & 0xFFFF) >> 8, (arg3 & 0xFFFF) >> 8, (arg3 & 0xFFFF) >> 8, 0);
                }
                D_800B49DC = 1;
            }
            var_t0 = TEX_FORMAT(arg1->format) == TEX_FORMAT_CI4;
            var_a1 = arg1->flags & 0x100;
            var_a0 = 1;
            var_a3_2 = pointersIntsArray;
        } else {
            var_a1 = 0;
            var_a0 = 0;
            var_a3_2 = pointersIntsArray + 0x260;
        }
    } else {
        var_t0 = FALSE;
        var_a1 = 0;
        var_a0 = 0;
        if (arg1 != NULL) {
            temp_v1 = (arg1->flags & 0xFEBF);
            arg2 = temp_v1 | arg2;
            var_a0 = 1;
            var_a3_2 = pointersIntsArray;
            var_t0 = TEX_FORMAT(arg1->format) == TEX_FORMAT_CI4;
            var_a1 = arg1->flags & 0x100;
        } else {
            var_a3_2 = pointersIntsArray + 0x260;
        }
    }
    if (arg5 & 1) {
        arg2 &= ~UINT_80092a48;
        temp = (s32) (arg2 & 0x70) >> 4;
        if (var_a0 != 0) {
            if (arg2 & 0x400) {
                if (var_a1 != 0) {
                    temp = 0x23;
                } else {
                    temp = 0x22;
                }
            } else if (arg2 & 0x100000) {
                temp += 8;
            } else if (arg2 & 0x80) {
                temp += 0x10;
            } else if (arg2 & 0x40000000) {
                temp = 0x25;
            } else if (arg2 & 0x200) {
                if (var_a1 != 0) {
                    temp = 0x21;
                } else {
                    temp += 0x18;
                }
            } else if (var_a1 != 0) {
                temp = 0x20;
            }
        }
        var_a2 = (arg2 & var_a3_2[temp].valA) | var_a3_2[temp].valB;
        var_v1_3 = 0x200004;
        if (var_a2 & 2) {
            var_v1_3 |= 1;
        }
        if (var_a2 & 8) {
            var_v1_3 |= 0x10000;
        }
        if (!(arg2 & 0x80000000)) {
            var_v1_3 |= 0x400;
        }
        if (arg5 & 4) {
            gSPGeometryMode(sp54++, 0xFFFFFF, var_v1_3);
        } else {
            gSPGeometryMode(sp54, 0xFFFFFF, var_v1_3);
            dl_apply_geometry_mode(&sp54);
        }
        temp_t4_3 = (var_a2 >> 3);
        sp54->words.w0 = (((Gfx*)var_a3_2[temp].prts[0])[temp_t4_3]).words.w0;
        sp54->words.w1 = (((Gfx*)var_a3_2[temp].prts[0])[temp_t4_3]).words.w1;
        if (!(arg5 & 4)) {
            dl_apply_combine(&sp54);
        } else {
            sp54++;
        }
        sp54->words.w0 = (((Gfx*)var_a3_2[temp].prts[1])[var_a2]).words.w0;
        sp54->words.w1 = (((Gfx*)var_a3_2[temp].prts[1])[var_a2]).words.w1;
        if (var_t0 != 0) {
            sp54->words.w0 |= 0x8000;
        }
        if (!(arg5 & 4)) {
            dl_apply_other_mode(&sp54);
        } else {
            sp54++;
        }
        if ((var_a2 & 7) < 4) {
            var_a2 += 4;
            D_800B49D8 = (((Gfx*)var_a3_2[temp].prts[1])[var_a2]).words.w1;
        }
    }
    *arg0 = sp54;
    return 0;
}
#endif

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/texture/set_textures_on_gdl.s")
#else
// https://decomp.me/scratch/0L3c5
void set_textures_on_gdl(Gfx** gdl, Texture* tex0, Texture* tex1, u32 flags, s32 level, u32 force, u32 setModes) {
    Gfx* temp_v0_2;
    Texture* var_a3;
    Texture* var_t2;
    Texture* var_v1;
    s32 temp_a1;
    s32 temp_a2;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_t0;
    s32 var_v0;
    struct PointersInts* temp_v1;
    Gfx* sp4C;
    struct PointersInts* var_a3_2;
    s32 var_a2_2;
    s32 pad[2];

    var_t0 = 0;
    var_a2 = 0;
    sp4C = *gdl;
    if (tex0 != NULL) {
        temp_a1 = level >> 0x10;
        if (tex0->levels != 0) {
            var_a2_2 = tex0->levels >> 8;
        } else {
            var_a2_2 = level * 0;
        }
        
        var_t2 = tex0;
        var_a3 = tex0;
        if (var_a2_2 >= 2 && temp_a1 < var_a2_2) {
            for (var_v1 = tex0, var_v0 = 0; var_v0 < temp_a1 && var_v1 != NULL; var_v0++) {
                var_v1 = var_v1->next;
            }
            if (var_v1 != NULL) {
                var_t2 = var_v1;
            }
            if (flags & 0x40) {
                if (flags & 0x80000) {
                    temp_a1 = temp_a1 - 1;
                    if (temp_a1 < 0) {
                        if (flags & 0x40000) {
                            temp_a1 += 2;
                        } else {
                            temp_a1 = 0;
                        }
                    }
                } else {
                    temp_a1 = temp_a1 + 1;
                    if (temp_a1 >= var_a2_2) {
                        if (flags & 0x40000) {
                            temp_a1 -= 2;
                        } else {
                            temp_a1 = var_a2_2 - 1;
                        }
                    }
                }
                for (var_v1 = tex0, var_v0 = 0; var_v0 < temp_a1 && var_v1 != NULL; var_v0++) {
                    var_v1 = var_v1->next;
                }
                if (var_v1 != NULL) {
                    var_a3 = var_v1;
                }
            } else {
                var_a3 = var_t2;
            }
        }
        if (tex1 != NULL) {
            var_a3 = tex1;
        }
        var_v0 = (s16) (tex0->flags & 0xFEBF);
        flags |= var_v0;
        if ((var_t2 != gCurrTex0) || (var_a3 != gCurrTex1) || (force != 0)) {
            gCurrTex0 = var_t2;
            gCurrTex1 = var_a3;
            temp_v0_2 = var_t2->gdl;
            gSPDisplayList(sp4C++, OS_PHYSICAL_TO_K0(temp_v0_2));
            temp_v0_2 = var_a3->gdl;
            if (tex1 != NULL) {
                gSPDisplayList(sp4C++, OS_PHYSICAL_TO_K0(temp_v0_2));
                dl_set_env_color(&sp4C, 0x7F, 0x7F, 0x7F, 0x7F);
            } else {
                if ((tex0->flags & 0x40) && (flags & 0x40)) {
                    temp_v0_2 += var_t2->gdlIdx;
                    gSPDisplayList(sp4C++, OS_PHYSICAL_TO_K0(temp_v0_2));
                    level >>= 8;
                    dl_set_env_color_no_sync(&sp4C, level, level, level, 0);
                } else if (flags & 0x2000) {
                    dl_set_env_color(&sp4C, 0xA0, 0xA0, 0xA0, 0xA0);
                }
            }
        }
        var_a2 = tex0->flags & 0x100;
        var_a1 = 1;
        var_a3_2 = pointersIntsArray;
        var_t0 = TEX_FORMAT(tex0->format) == TEX_FORMAT_CI4;
    } else {
        var_a1 = 0;
        var_a3_2 = pointersIntsArray + 0x260;
    }
    if (setModes != 0) {
        flags &= ~UINT_80092a48;
        var_a0 = (s32) (flags & 0x70) >> 4;
        if (var_a1 != 0) {
            if (flags & 0x400) {
                if (var_a2 != 0) {
                    var_a0 = 0x23;
                } else {
                    var_a0 = 0x22;
                }
            } else if (flags & 0x100000) {
                var_a0 += 8;
            } else if (flags & 0x80) {
                var_a0 += 0x10;
            } else if (flags & 0x200) {
                if (var_a2 != 0) {
                    var_a0 = 0x21;
                } else {
                    var_a0 += 0x18;
                }
            } else if (var_a2 != 0) {
                var_a0 = 0x20;
            }
        }
        temp_v1 = &var_a3_2[var_a0];
        temp_a2 = temp_v1->valB | (flags & temp_v1->valA);
        var_v0 = 0x200004;
        if (temp_a2 & 2) {
            var_v0 = 0x200005;
        }
        if (temp_a2 & 8) {
            var_v0 |= 0x10000;
        }
        if (!(flags & 0x80000000)) {
            var_v0 |= 0x400;
        }
        gSPLoadGeometryMode(sp4C, var_v0);
        dl_apply_geometry_mode(&sp4C);
        sp4C->words.w0 = (((Gfx **)temp_v1->prts)[0][(temp_a2 >> 3)]).words.w0;
        sp4C->words.w1 = (((Gfx **)temp_v1->prts)[0][(temp_a2 >> 3)]).words.w1;
        dl_apply_combine(&sp4C);
        sp4C->words.w0 = (((Gfx **)temp_v1->prts)[1][temp_a2]).words.w0;
        sp4C->words.w1 = (((Gfx **)temp_v1->prts)[1][temp_a2]).words.w1;
        if (var_t0 != 0) {
            sp4C->words.w0 |= 0x8000;
        }
        dl_apply_other_mode(&sp4C);
    }
    *gdl = sp4C;
}
#endif

void func_8003E648(Texture* arg0, s32* arg1, s32* arg2) {
    s32 temp_a1;
    s32 temp_t1;
    s32 temp_t2;
    s32 var_a0;
    s32 y;

    temp_t2 = *arg1 & 0x80000;
    temp_t1 = *arg1 & 0x40000;
    y = *arg1 & 0x20000;

    if (y) {
        if (temp_t1 == 0) {
            if (rand_next(0, 1000) >= 986) {
                *arg1 &= 0xFFF7FFFF;
                *arg1 |= 0x40000;
            }
            return;
        }

        if (temp_t2 == 0) {
            *arg2 +=(arg0->unk_0xe * delayByte);
            if (*arg2 >= arg0->levels) {
                *arg2 = ((arg0->levels * 2) - *arg2) - 1;
                if (*arg2 < 0) {
                    *arg2 = 0;
                    *arg1 &= 0xFFF3FFFF;
                    return;
                }
                *arg1 |= 0x80000;
            }
            return;
        }

        *arg2 -= arg0->unk_0xe * delayByte;
        if (*arg2 < 0) {
            *arg2 = 0;
            *arg1 &= 0xFFF3FFFF;
        }
        return;
    }

    if (temp_t1) {
        if (temp_t2 == 0) {
            *arg2 += arg0->unk_0xe * delayByte;
        } else {
            *arg2 -= arg0->unk_0xe * delayByte;
        }
        do {
            var_a0 = 0;
            if ((s32) *arg2 < 0) {
                *arg2 = -*arg2;
                var_a0 = 1;
                *arg1 &= 0xFFF7FFFF;
            }
            if (arg0->flags & 0x40) {
                temp_a1 = arg0->levels - 0x100;
                if (*arg2 >= temp_a1) {
                    *arg2 = ((temp_a1 * 2) - (s32) *arg2) - 1;
                    var_a0 = 1;
                    *arg1 |= 0x80000;
                }
            } else if ((s32) *arg2 >= (s32) arg0->levels) {
                *arg2 = ((arg0->levels * 2) - (s32) *arg2) - 1;
                var_a0 = 1;
                *arg1 |= 0x80000;
            }
        } while (var_a0 != 0);
        return;
    }
    if (temp_t2 == 0) {
        *arg2 += arg0->unk_0xe * delayByte;
        while ((s32) *arg2 >= arg0->levels) {
            *arg2 -= arg0->levels;
        }
    } else {
        *arg2 -= arg0->unk_0xe * delayByte;
        while ((s32) *arg2 < 0) {
            *arg2 += arg0->levels;
        }
    }
}

Texture* func_8003E904(Texture* arg0, s32 arg1) {
    Texture* var_v0;
    Texture* var_v1;
    s32 i;

    var_v1 = arg0 + 1;
    if (arg1 > 0) {
        if (arg1 < arg0->levels) {
            arg1>>= 8;
            for (var_v0 = arg0, i = 0; i < arg1 && var_v0 != NULL; i++) {
                var_v0 = var_v0->next;
            }
            if (var_v0 != NULL) {
                var_v1 = var_v0 + 1;
            }
        }
    }
    return var_v1;
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003E960.s")
#else
Texture* func_8003E960(s32 arg0) {
    s32 i;

    for (i = 0; i < D_800B49B0; i++) {
        if (arg0 == ((D_800B49A8 + i)->unk0 & 0xFFFFFFFF)) {
            return (D_800B49A8 + i)->unk4;
        }
    }
    return NULL;
}
#endif

void func_8003E9B4(s32 arg0) {
    UINT_80092a48 |= arg0;
}

void func_8003E9D0(s32 arg0) {
    UINT_80092a48 &= ~arg0;
}

void func_8003E9F0(Gfx** arg0, s32 arg1) {
    s32 temp_v0;
    s32 *v0;

    if (D_80092A50 > 0) {
        // yeah I don't even know what this is
        if ((D_80092A50 == 0xA) && ((temp_v0 = D_800B49E0) == 0)) {
            D_800B49E0 = temp_v0 = *D_80092A54;
        }
        func_8003FE70(arg0, *D_80092A54, D_80092A50, func_80004A4C());
        v0 = &D_800B49E0;
        if (D_80092A50 == 0xA) {
            temp_v0 = D_800B49E0;
            temp_v0 -= arg1;
            *v0 = temp_v0;
            if (*v0 <= 0) {
                D_80092A50 = 0;
                *v0 = 0;
            }
        } else {
            D_80092A50 = 0;
        }
        return;
    } else {
        D_800B49E0 = 0;
    }
}

void func_8003EAC0(u16 *arg0, s32 arg1, s32 arg2, u16* arg3) {
    s32 var_a3;

    if (arg2 > 0) {
        func_8003EC8C(&arg3[arg2], arg0, arg1 - arg2);
        bzero(arg3, (arg2 + 1) << 1);
        var_a3 = arg2 >= 0xB ? 0xA : arg2;
        func_8003F2C4(&arg3[arg2] - var_a3, 0, arg3[arg2 + 1], var_a3);
        return;
    }

    arg2 = -arg2;
    func_8003EC8C(arg3, &arg0[arg2], arg1 - arg2);
    bzero(&arg3[arg1 - arg2], arg2 << 1);
    var_a3 = arg2 >= 0xB ? 0xA : arg2;
    func_8003F2C4(&arg3[arg1] - arg2, (&arg3[arg1] - arg2)[-2], 0, var_a3);
}

void func_8003EBD4(s32 hOffset) {
    s32 height;
    s32 resolution;
    s32 width;
    u16* nextFB;
    u16* currentFB;

    resolution = get_some_resolution_encoded();
    width = RESOLUTION_WIDTH(resolution);
    height = RESOLUTION_HEIGHT(resolution);
    nextFB = gFramebufferNext;
    currentFB = gFramebufferCurrent;
    hOffset = width - hOffset;
    while (height--) {
        weird_resize_copy(&nextFB[hOffset], width - (hOffset << 1), width, currentFB);
        nextFB += width;
        currentFB += width;
    }
}

void func_8003EC8C(u16* arg0, u16* arg1, s32 arg2) {
    if (arg0 < arg1) {
        while (arg2--) {
            arg0[0] = arg1[0];
            arg0++;
            arg1++;
        }
    } else {
        arg0 += arg2;
        arg1 += arg2;
        while (arg2--) {
            arg0[0] = arg1[0];
            arg0--;
            arg1--;
        }
    }
}

void func_8003ED00(u16* arg0, u16* arg1, s32 arg2) {
    // 0x842 = 2114
    // 0x18C6 = 6342 (2114 * 3)
    // 0x39CE = 14798 (2114 * 7)
    if (arg0 < arg1) {
        (arg0++)[0] = ((arg1++)[0] & ~0x39CE) >> 3;
        (arg0++)[0] = ((arg1++)[0] & ~0x18C6) >> 2;
        (arg0++)[0] = ((arg1++)[0] & ~0x18C6) >> 2;
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        arg2 -= 12;
        while (arg2--) {
            (arg0++)[0] = (arg1++)[0];
        }
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        (arg0++)[0] = ((arg1++)[0] & ~0x842) >> 1;
        (arg0++)[0] = ((arg1++)[0] & ~0x18C6) >> 2;
        (arg0++)[0] = ((arg1++)[0] & ~0x18C6) >> 2;
        (arg0++)[0] = ((arg1++)[0] & ~0x39CE) >> 3;
        return;
    }

    arg1 += arg2;
    arg0 += arg2;
    (arg0--)[0] = ((arg1--)[0] & ~0x39CE) >> 3;
    (arg0--)[0] = ((arg1--)[0] & ~0x18C6) >> 2;
    (arg0--)[0] = ((arg1--)[0] & ~0x18C6) >> 2;
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    arg2 -= 12;
    while (arg2--) {
        (arg0--)[0] = (arg1--)[0];
    }
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    (arg0--)[0] = ((arg1--)[0] & ~0x842) >> 1;
    (arg0--)[0] = ((arg1--)[0] & ~0x18C6) >> 2;
    (arg0--)[0] = ((arg1--)[0] & ~0x18C6) >> 2;
    (arg0--)[0] = ((arg1--)[0] & ~0x39CE) >> 3;
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003EF30.s")
#else
// https://decomp.me/scratch/KWLGK
void func_8003EF30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, void (*arg6)(u16*, s32, s32, u16*)) {
    s32 width;
    s32 height;
    s32 temp_s0;
    s32 resolution;
    s32 i;
    u16* currentFB;
    u16* nextFB;
    s32 s1;
    s32 s2;

    resolution = get_some_resolution_encoded();
    width = RESOLUTION_WIDTH(resolution);
    height = RESOLUTION_HEIGHT(resolution);
    currentFB = gFramebufferCurrent;
    nextFB = gFramebufferNext;
    i = 0;
    if (height > i) {
        s1 = arg0;
        s2 = arg3;

        do {
            temp_s0 = (sin16(s1) * arg1) / 32768;
            temp_s0 -= (sin16(s2) * arg4) / 32768;
            arg6(currentFB, width, temp_s0, nextFB);
            s1 += arg2;
            s2 += arg5 ^ 0;
            i++;
            currentFB += width;
            nextFB += width;
        } while (i < height);
    }
}
#endif

void func_8003F074(u16* arg0, s32 arg1, s32 arg2, u16* arg3) {
    u16 sp50[644];
    s32 max;
    s32 i;
    s32 var_v0;
    u16 *v1;
    u16 *a0;
    s32 var_a3;

    if (arg2 < 0) {
        arg2 = -arg2;
    }
    var_v0 = 0;
    a0 = sp50;
    v1 = arg0;
    var_a3 = arg1 - (arg2 * 2);
    max = arg1 - (arg2 * 2);
    for (i = 0; i < max; i++) {
        // We *should* re-use max / var_a3 here but that doesn't match
        var_v0 += arg1 % (arg1 - (arg2 * 2));
        a0[0] = v1[0];
        a0++;
        while (var_a3 < var_v0) {
            var_v0 -= var_a3;
            v1 += 1;
        }
        v1 += (arg1 / (var_a3));
    }
    bcopy(sp50, &arg3[arg2], var_a3 << 1);
    bzero(arg3, (arg2 - 1) << 1);
    bzero(&arg3[arg1 - arg2 - 1], arg2 << 1);
    var_a3 = arg2 >= 0xB ? 0xA : arg2;
    func_8003F2C4(&arg3[arg2] - var_a3, 0, arg3[arg2 + 1], var_a3);
    func_8003F2C4(&arg3[arg1] - arg2, (&arg3[arg1] - arg2)[-2], 0, var_a3);
}

/**
 * Copies data from src to dest+destWidth taking into account differences in width
 * (e.g. if destWidth is half srcWidth, every other value will be copied from src).
 *
 * Additionally, values copied to dest have the following applied first:
 *   (value & ~0x842) >> 1
 *
 * destWidth must be less than 644.
 *
 * NOTE: Please see the note in the implementation, this function also reads
 * undefined stack memory, which affects the copied values.
 */
void weird_resize_copy(u16* src, s32 srcWidth, s32 destWidth, u16* dest) {
    s32 i;
    u16 buffer[642];
    s32 var_v0;
    u16* bufferPtr;
    u16* srcPtr;

    var_v0 = 0;
    bufferPtr = buffer;
    srcPtr = src;
    i = 0;
    while (i < destWidth) {
        var_v0 += srcWidth % destWidth;
        // NOTE: This reads undefined stack memory!
        bufferPtr[0] = ((srcPtr[0] & ~0x842) >> 1) + ((bufferPtr[0] & ~0x842) >> 1);
        bufferPtr += 1;
        while (destWidth < var_v0) {
            var_v0 -= destWidth;
            srcPtr += 1;
        }
        i += 1;
        srcPtr += srcWidth / destWidth;
    }
    bcopy(buffer, &dest[destWidth], destWidth << 1);
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003F2C4.s")
#else
void func_8003F2C4(u16* arg0, u16 arg1, u16 arg2, s32 arg3) {
    u16* var_s3;
    s32 var_a3;
    u16 var_a1;
    u16 var_t0;
    u16 var_v1;
    u16 t1;
    u16 t2;
    u16 t3;

    if (arg3 == 0) {
        return;
    }
    var_s3 = arg0;
    var_a3 = arg3 - 1;
    if (arg3 != 0) {
        var_v1 = (((arg1 & 0xF800) >> 0xB) << 8);
        var_a1 = (((arg1 & 0x7C0) >> 6) << 8);
        var_t0 = (((arg1 & 0x3E) >> 1) << 8);
        do {
            t1 = ((((arg2 & 0xF800) >> 0xB) - ((arg1 & 0xF800) >> 0xB)) << 8) / arg3;
            var_v1 += t1;
            t2 = ((((arg2 & 0x7C0) >> 6) - ((arg1 & 0x7C0) >> 6)) << 8) / arg3;
            var_a1 += t2;
            t3 = ((((arg2 & 0x3E) >> 1) - ((arg1 & 0x3E) >> 1)) << 8) / arg3;
            var_t0 += t3;
            *var_s3 = ((var_v1 >> 8) << 0xB) + ((var_a1 >> 8) << 6) + ((var_t0 >> 8) << 1);
            var_s3 += 1;
        } while (var_a3--);
    }
}
#endif

void NOTosSetTime(s32 arg0, s32 arg1) {
    D_80092A50 = arg0;
    D_80092A54[0] = arg1;
}

void func_8003F4C0(s32 arg0) {
    s32 width;
    s32 resolution;
    s32 height;
    s32 i;
    s32 j;
    u16* nextFB;
    u16* currentFB;
    s32 v0;

    resolution = get_some_resolution_encoded();
    width = RESOLUTION_WIDTH(resolution);
    height = RESOLUTION_HEIGHT(resolution);
    if (arg0 == 0) {
        nextFB = gFramebufferNext;
        nextFB[-1] = 0;
        currentFB = gFramebufferCurrent;
        for (i = 0; i < height - 1; i++) {
            for (j = 0; j < width; j++) {
                currentFB[0] = func_8003FD48(currentFB[0], *(nextFB - width), nextFB[0], nextFB[width]);
                currentFB += 1;
                nextFB += 1;
            }
        }
    } else {
        nextFB = gFramebufferNext;
        nextFB[-1] = 0;
        currentFB = gFramebufferCurrent;
        for (i = 0; i < height - 1; i++) {
            for (j = 0; j < width; j++) {
                v0 = 0xFFFF >> (0x10 - arg0);
                // apparently this is v0 * 0x842
                v0 = ~((v0 << 0xB) + (v0 << 6) + (v0 << 1));
                currentFB[0] = (func_8003FD48(currentFB[0], *(nextFB - width), *nextFB, nextFB[width]) &  v0) >> arg0;
                currentFB += 1;
                nextFB += 1;
            }
        }
    }
}

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003F660.s")
#else
// https://decomp.me/scratch/4XcTd
void func_8003F660(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 height;
    s32 width;
    s32 resolution;
    s32 j;
    s32 var_v0;
    u16 temp_a1;
    u16 temp_v1;
    u16 t1;
    u16 t2;
    u16* currentFB;
    u16* currentFB2;
    u32 a2;
    s32 a3;
    s32 v0;
    s32 temp;
    s32 s8;

    resolution = get_some_resolution_encoded();
    width = RESOLUTION_WIDTH(resolution);
    height = RESOLUTION_HEIGHT(resolution);
    if (arg3 != 0) {
        var_v0 = (height - ((height * 0xB6) / 240)) >> 1;
        height -= var_v0;
    } else {
        var_v0 = 0;
    }
    if (arg0 == 0) {
        s8 = var_v0;
        currentFB = &(&(&gFramebufferCurrent[width * var_v0])[arg1])[arg2 * width];
        currentFB[-1] = 0;
        currentFB2 = &gFramebufferCurrent[width * var_v0];
        if (s8 < height) {
            do {
                j = 0;
                while (j < width) {
                    t1 = currentFB[width];
                    t2 = currentFB[-1];
                    temp_a1 = *(currentFB - width);
                    temp_v1 = currentFB[1];
                    j += 1;
                    temp = ((u32)((currentFB[width] & 0xF800) + (currentFB[-1] & 0xF800) + (temp_a1 & 0xF800) + (temp_v1 & 0xF800)) >> 2) & 0xF800;
                    temp += (((currentFB[width] & 0x7C0) + (currentFB[-1] & 0x7C0) + (temp_a1 & 0x7C0) + (temp_v1 & 0x7C0)) >> 2) & 0x7C0;
                    temp += ((((currentFB[width] & 0x3E) + (currentFB[-1] & 0x3E) + (temp_a1 & 0x3E) + (temp_v1 & 0x3E)) >> 2) & 0x3E);
                    *currentFB2 = temp;
                    currentFB2 += 1;
                    currentFB += 1;
                }
                s8 += 1;
            } while (s8 != height);
        }
    } else {
        s8 = var_v0;
        currentFB = &(&(&gFramebufferCurrent[width * var_v0])[arg1])[arg2 * width];
        currentFB[-1] = 0;
        currentFB2 = &gFramebufferCurrent[width * var_v0];
        if (s8 < height) {
            do {
                j = 0;
                while (j < width) {
                    v0 = 0xFFFF >> (0x10 - arg0);
                    // apparently this is v0 * 0x842
                    v0 = ~((v0 << 0xB) + (v0 << 6) + (v0 << 1));
                    temp = (func_8003FB88(currentFB[-1], *(currentFB - width), currentFB[1], currentFB[width]) & v0);
                    temp >>= arg0;
                    j += 1;
                    currentFB2[0] = temp;
                    currentFB2 += 1;
                    currentFB += 1;
                }
                s8 += 1;
            } while (s8 != height);
        }
    }
}
#endif

u16 func_8003F928(u16* arg0, s32 arg1) {
    u32 temp;
    u32 temp2;
    u32 temp3;

    temp = (arg0[0] & 0xF800) + (arg0[1] & 0xF800) + (arg0[2] & 0xF800);
    temp2 = (arg0[0] & 0x7C0) + (arg0[1] & 0x7C0) + (arg0[2] & 0x7C0);
    temp3 = (arg0[0] & 0x3E) + (arg0[1] & 0x3E) + (arg0[2] & 0x3E);

    arg0 += arg1;
    temp += (arg0[0] & 0xF800) + (arg0[1] & 0xF800) + (arg0[2] & 0xF800);
    temp2 += (arg0[0] & 0x7C0) + (arg0[1] & 0x7C0) + (arg0[2] & 0x7C0);
    temp3 += (arg0[0] & 0x3E) + (arg0[1] & 0x3E) + (arg0[2] & 0x3E);

    arg0 += arg1;
    temp += (arg0[0] & 0xF800) + (arg0[1] & 0xF800) + (arg0[2] & 0xF800);
    temp2 += (arg0[0] & 0x7C0) + (arg0[1] & 0x7C0) + (arg0[2] & 0x7C0);
    temp3 += (arg0[0] & 0x3E) + (arg0[1] & 0x3E) + (arg0[2] & 0x3E);

    temp /= 9;
    temp2 /= 9;
    temp3 /= 9;
    return (temp & 0xF800) + (temp2 & 0x7C0) + (temp3 & 0x3E);
}

u16 func_8003FA88(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4) {
    u32 temp;
    u32 temp2;
    u32 temp3;

    temp = arg0 & 0xF800;
    temp += (((((arg1 & 0xF800) >> 0xB) - ((arg2 & 0xF800) >> 0xB)) + ((arg3 & 0xF800) >> 0xB)) - ((arg4 & 0xF800) >> 0xB)) << 0xB;

    temp2 = arg0 & 0x7C0;
    temp2 += (((((arg1 & 0x7C0) >> 6) - ((arg2 & 0x7C0) >> 6)) + ((arg3 & 0x7C0) >> 6)) - ((arg4 & 0x7C0) >> 6)) << 6;

    temp3 = arg0 & 0x3E;
    temp3 += (((((arg1 & 0x3E) >> 1) - ((arg2 & 0x3E) >> 1)) + ((arg3 & 0x3E) >> 1)) - ((arg4 & 0x3E) >> 1)) << 1;

    return (((u16)temp & 0xF800) + ((u16)temp2 & 0x7C0) + ((u16)temp3 & 0x3E));
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003FB88.s")
#else
u16 func_8003FB88(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    u16 temp;
    u16 temp2;
    u16 temp3;
    temp = (((u32)arg0 & 0xF800) + (arg1 & 0xF800) + (arg2 & 0xF800) + (arg3 & 0xF800) >> 2) & 0xF800;
    temp2 = ((arg0 & 0x7C0) + (arg1 & 0x7C0) + (arg2 & 0x7C0) + (arg3 & 0x7C0) >> 2) & 0x7C0;
    temp3 = ((arg0 & 0x3E) + (arg1 & 0x3E) + (arg2 & 0x3E) + (arg3 & 0x3E) >> 2) & 0x3E;
    return temp + temp2 + temp3;
}
#endif

u16 func_8003FC3C(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    return (((((arg0 & ~0x842) >> 1) + ((arg1 & ~0x842) >> 1)) & ~0x842) >> 1) + (((((arg2 & ~0x842) >> 1) + ((arg3 & ~0x842) >> 1)) & ~0x842) >> 1);
}

u16 func_8003FCBC(u16 arg0, u16 arg1) {
    s32 temp_a3;
    s32 temp_v1;
    u16 var_a0;
    u16 var_a2;
    u16 var_a3;

    temp_v1 = arg1 & 0xF800;
    temp_a3 = arg0 & 0xF800;
    if (temp_v1 < temp_a3) {
        var_a0 = temp_a3;
    } else {
        var_a0 = temp_v1;
    }
    temp_v1 = arg1 & 0x7C0;
    temp_a3 = arg0 & 0x7C0;
    if (temp_v1 < temp_a3) {
        var_a3 = temp_a3;
    } else {
        var_a3 = temp_v1;
    }
    temp_v1 = arg1 & 0x3E;
    temp_a3 = arg0 & 0x3E;
    if (temp_v1 < temp_a3) {
        var_a2 = temp_a3;
    } else {
        var_a2 = temp_v1;
    }
    return (var_a0 + var_a3 + var_a2);
}

u16 func_8003FD48(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    s32 temp_t2;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_a3;
    s32 var_t2;
    s32 var_v0;
    s32 var_v1;

    temp_v1 = arg0 & 0xF800;
    temp_t2 = arg1 & 0xF800;
    if (temp_v1 < temp_t2) {
        var_v0 = (u16)temp_v1;
    } else {
        var_v0 = (u16)temp_t2;
    }

    temp_v1 = arg0 & 0x7C0;
    temp_t2 = arg1 & 0x7C0;
    if (temp_v1 < temp_t2) {
        var_v1 = (u16)temp_v1;
    } else {
        var_v1 = (u16)temp_t2;
    }

    temp_v1 = arg0 & 0x3E;
    temp_t2 = arg1 & 0x3E;
    if (temp_v1 < temp_t2) {
        var_a2 = (u16)temp_v1;
    } else {
        var_a2 = (u16)temp_t2;
    }

    temp_v1 = arg2 & 0xF800;
    temp_t2 = arg3 & 0xF800;
    if (temp_v1 < temp_t2) {
        var_a3 = (u16)temp_v1;
    } else {
        var_a3 = (u16)temp_t2;
    }

    temp_v1 = arg2 & 0x7C0;
    temp_t2 = arg3 & 0x7C0;
    if (temp_v1 < temp_t2) {
        var_t2 = (u16)temp_v1;
    } else {
        var_t2 = (u16)temp_t2;
    }

    temp_v1 = arg2 & 0x3E;
    temp_t2 = arg3 & 0x3E;
    if (temp_v1 < temp_t2) {
        var_a0 = (u16)temp_v1;
    } else {
        var_a0 = (u16)temp_t2;
    }

    var_a1 = var_v0;
    if (var_a3 < var_v0) {
        var_a1 = (u16)var_a3;
    }

    var_a3 = var_v1;
    if (var_t2 < var_v1) {
        var_a3 = (u16)var_t2;
    }

    var_v1 = var_a2;
    if (var_a0 < var_a2) {
        var_v1 = (u16)var_a0;
    }
    return (var_a1 + var_a3 + var_v1);
}

// Missing jtbl_8009A4A4
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_8003FE70.s")

void func_800402F8(u16* arg0, u16* arg1, s32 arg2, s32 arg3, s32 arg4) {
    u16 i;
    u16 j;

    for (i = 0; i < arg3; i++) {
        for (j = 0; j < arg2; j++) {
            arg0[0] = ((arg1[0] & (1 << arg4) | arg0[0] & ~(1 << arg4)));
            arg0 += 1;
            arg1 += 1;
        }
    }
}

void func_80040378(u16 *fb1, u16 *fb2, s32 width, s32 height) {
    func_8005324C(fb1, fb2, width * height >> 2);
}

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_800403AC.s")
#else
// https://decomp.me/scratch/0S8QE
void func_800403AC(u16* arg0, u16* arg1, u32 arg2, u32 arg3, s32 arg4) {
    s32 i;
    s32 j;
    u16 temp;
    u16 temp2;
    u16 temp3;
    u16 temp4;
    u16 temp5;

    for (i = 0; i != arg3; i++) {
        for (j = 0; j != arg2; j++) {
            temp4 = arg0[0];
            temp5 = arg1[0];
            temp = ((u32) ((((temp4 & 0xF800) << arg4) + (temp5 & 0xF800)) - (temp4 & 0xF800)) >> arg4);
            temp2 = ((u32) ((((temp4 & 0x7C0) << arg4) + (temp5 & 0x7C0)) - (temp4 & 0x7C0)) >> arg4);
            temp3 = ((u32) ((((temp4 & 0x3E) << arg4) + (temp5 & 0x3E)) - (temp4 & 0x3E)) >> arg4);
            arg0[0] = (temp & 0xF800) + (temp2 & 0x7C0) + (temp3 & 0x3E);
            arg0 += 1;
            arg1 += 1;
        }
    }
}
#endif

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/texture/func_80040468.s")
#else
// https://decomp.me/scratch/NGTZD
void func_80040468(u16* arg0, u16* arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_s2;
    s32 temp_s4;
    s32 temp_t0;
    s32 temp_t2;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_t5;
    s32 i;
    s32 j;
    u16 temp_a0;
    u16 temp_a1;

    for (i = 0; i != arg3; i++) {
        for (j = 0; j != arg2; j++) {
            temp_s4 = arg4 - 1;
            temp_a0 = arg0[0];
            temp_a1 = arg1[0];
            temp_t0 = temp_a0 & 0xF800;
            temp_t2 = temp_a1 & 0xF800;
            temp_t3 = temp_a0 & 0x7C0;
            temp_t4 = temp_a1 & 0x7C0;
            temp_t5 = temp_a0 & 0x3E;
            temp_s2 = temp_a1 & 0x3E;
            arg0[0] = (((u32) (((temp_t0 << arg4) + temp_t2) - temp_t0) >> arg4) & 0xF800) +
                (((u32) (((temp_t3 << arg4) + temp_t4) - temp_t3) >> arg4) & 0x7C0) +
                (((u32) (((temp_t5 << arg4) + temp_s2) - temp_t5) >> arg4) & 0x3E);
            arg1[0] = (((u32) (((temp_t2 << temp_s4) + temp_t0) - temp_t2) >> temp_s4) & 0xF800) +
                (((u32) (((temp_t4 << temp_s4) + temp_t3) - temp_t4) >> temp_s4) & 0x7C0) +
                (((u32) (((temp_s2 << temp_s4) + temp_t5) - temp_s2) >> temp_s4) & 0x3E);
            arg0 += 1;
            arg1 += 1;
        }
    }
}
#endif

void func_80040590(s32 arg0, s32 arg1) {
    s32 width;
    s32 height;
    s32 resolution;
    u16* currentFB;
    u16* nextFB;
    s32 var_s1;
    s32 var_s2;
    s32 i;

    resolution = get_some_resolution_encoded();
    width = RESOLUTION_WIDTH(resolution);
    height = RESOLUTION_HEIGHT(resolution);
    nextFB = gFramebufferNext;
    currentFB = gFramebufferCurrent;
    for (i = 0; i < height; i++) {
        var_s1 = arg0 - (cos16(((((arg0 >> 1) + i) << 0xF) / (u32)(height >> 2))) >> 0xC);
        if ((width >> 1) < var_s1) {
            var_s1 = (width >> 1);
        }
        if (var_s1 < 0) {
            var_s1 = -var_s1;
        }
        if (arg1 < var_s1) {
            resolution = var_s1 - arg1;
        } else {
            resolution = 0;
        }

        if (var_s1 < arg1) {
            // @fake
            if (1) {}
            var_s2 = var_s1;
        } else {
            var_s2 = arg1;
        }
        func_8003F2C4(&currentFB[resolution], currentFB[resolution], nextFB[var_s1], var_s2);
        func_8003F2C4(&currentFB[width] - var_s1, *(&nextFB[width] - var_s1), *(&currentFB[width] - resolution), var_s2);
        bcopy(&nextFB[var_s1], &currentFB[var_s1], (width - (var_s1 << 1)) << 1);
        currentFB += width;
        nextFB += width;
    }
}

void func_80040754(u16* arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 j;
    s32 i;
    s32 end;

    end = arg1 - arg3;
    for (i = 0; i < arg2; i++) {
        arg0 += arg3;
        for (j = 0; j < end; j++) {
            arg0[0] = ((arg0[0] & 0xF7BD) >> 1) + ((*(arg0 - arg3) & 0xF7BD) >> 1);
            arg0 += 1;
        }
    }
}

void func_80040870(u16* arg0, u16 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 j;
    s32 i;

    arg1 += (arg2 - arg4);
    for (i = 0; i < arg3; i++) {
        for (j = 0; j < arg4; j++) {
            arg0[0] = arg1[0];
            arg0++;
            arg1++;
        }
        arg1 += (arg2 - arg4);
        arg0 += (arg2 - arg4);
    }
}

void func_80040920(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_s2;
    s32 i;
    u16* temp_v1;

    for (i = 0; i < arg3; i++) {
        temp_s2 = rand_next(0, arg1);
        temp_v1 = (u16 *)((rand_next(0, arg2) * arg1 * 2) + arg0 + (temp_s2 * 2));
        temp_v1[0] &= ~1;
    }
}

void func_800409D0(u16 *fb, s32 width, s32 height) {
    // Sets center pixel alpha to 0
    fb[(width >> 1) + (height >> 1) * width] &= 0xFFFE;
}

void func_80040A04(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;
    u16* temp_v1;

    for (i = 0; i < arg3; i++) {
        temp_v1 = (u16 *)((rand_next(0, arg2) * arg1 * 2) + arg0);
        temp_v1[0] &= ~1;
    }
}

void func_80040A94(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;
    u16* temp_v1;

    for (i = 0; i < arg3; i++) {
        temp_v1 = (u16 *)((rand_next(0, arg2) * arg1 * 2) + arg0 + (arg1 * 2));
        temp_v1[0] &= ~1;
    }
}

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

void func_80040B8C(u16* arg0, u16* arg1, u16* arg2, s32 arg3, s32 arg4) {
    s32 i;
    s32 j;

    for (i = 0; i < arg3; i++) {
        for (j = 0; j < arg4; j++, arg0++, arg1++, arg2++) {
            if (~arg2[0] & 1) {
                *arg0 = arg1[0];
                *arg0 &= ~1;
            }
        }
    }
}

void func_80040CD0(u16* arg0, u16* arg1, s32 arg2, s32 arg3) {
    s32 temp_a2_2;
    s32 j;
    s32 var_fp;
    s32 var_s5;
    s32 i;
    u16 sp5C[2][0x280];
    s32 var_s7;

    var_fp = 0;
    var_s7 = rand_next(0, 3) - 1;
    if (!var_s7) {
        var_s7 = 1;
    }
    var_s5 = (rand_next(0, 3) - 1) * arg3;
    for (i = 0; i < arg2; i++) {
        for (j = 0; j < arg3; j++) {
            if ((~*(arg0 + var_s7) & 1) + (~*(arg0 + var_s5) & 1) + (~*((arg0 - var_s5) + var_s7) & 1)) {
                if (~arg1[0] & 1) {
                    sp5C[var_fp][j] = arg1[0];
                } else {
                    sp5C[var_fp][j] = ((arg0[0] & ~0x842) >> 1) + ((arg1[0] & ~0x842) >> 1);
                    arg1[0] &= ~1;
                }
                sp5C[var_fp][j] &= ~1;
            } else {
                sp5C[var_fp][j] = arg0[0];
            }
            arg0++;
            arg1++;
        }

        var_s7 = rand_next(0, 3) - 1;
        if (!var_s7) {
            var_s7 = 1;
        }
        temp_a2_2 = arg3 << 1;
        var_s5 = (rand_next(0, 3) - 1) * arg3;
        var_fp ^= 1;
        if (i != 0) {
            bcopy(&sp5C[var_fp], arg0 - temp_a2_2, temp_a2_2);
        }
    }
    var_fp ^= 1;
    bcopy(&sp5C[var_fp], arg0 - arg3, arg3 << 1);
}

void func_80040EFC(u16* arg0, u16* arg1, s32 arg2, s32 arg3) {
    s32 i;

    for (i = 0; i < (arg2 * arg3); i++) {
        arg0[0] |= 1;
        arg0++;
        arg1[0] |= 1;
        arg1++;
    }
}
