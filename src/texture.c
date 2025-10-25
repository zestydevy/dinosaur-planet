#include "common.h"
#include "sys/rarezip.h"
#include "PR/gbi.h"

static const char str_8009a370[] = "Error: Texture no %d out of range on load -> max=%d.!!\n";
static const char str_8009a3a8[] = "Multiple texture fail!!\n";
static const char str_8009a3c4[] = "TEX Error: TexTab overflow %d,%d!!\n";
static const char str_8009a3e8[] = "texFreeTexture: NULL tex!!\n";
static const char str_8009a404[] = "TEX Error: Tryed to deallocate non-existent texture!!\n";

void load_texture_to_tmem2(Gfx **gdl, Texture *texture, u32 tile, u32 tmem, u32 palette);
void dl_set_env_color_no_sync(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);

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


void init_textures(void) {
    s32 var_v1;
    s32* temp_a1;
    s32 i;

    D_800B49A8 = mmAlloc(0x15E0, ALLOC_TAG_TEX_COL, NULL);
    D_800B49B0 = 0;
    queue_alloc_load_file((void **) &gFile_TEX0_TAB, 0x28);
    queue_alloc_load_file(&gFile_TEX1_TAB, 0x25);
    queue_alloc_load_file(&gFile_TEXTABLE, 0x26);
    for (i = 0; i < 2; i++) {
        temp_a1 = gFile_TEX0_TAB[i];
        for (var_v1 = 0; temp_a1[var_v1] != -1; var_v1++) {}
        D_800B49B8[i] = (var_v1 - 1);
    }
    D_800B49C0 = mmAlloc(0x108, ALLOC_TAG_TEX_COL, NULL);
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
            sp44->unk10 = (u32) temp_s7 >> 2;
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
            load_texture_to_tmem2(&mygdl, texture, 1, (0x1000 - texture->unk18) >> 3, 0);
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

    width = texture->width | (texture->unk1B & 0xf0) << 4;
    height = texture->height | (texture->unk1B & 0xf) << 8;

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
            var_v0 |= 1;
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
            *arg2 +=(arg0->unkE * delayByte);
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

        *arg2 -= arg0->unkE * delayByte;
        if (*arg2 < 0) {
            *arg2 = 0;
            *arg1 &= 0xFFF3FFFF;
        }
        return;
    }

    if (temp_t1) {
        if (temp_t2 == 0) {
            *arg2 += arg0->unkE * delayByte;
        } else {
            *arg2 -= arg0->unkE * delayByte;
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
        *arg2 += arg0->unkE * delayByte;
        while ((s32) *arg2 >= arg0->levels) {
            *arg2 -= arg0->levels;
        }
    } else {
        *arg2 -= arg0->unkE * delayByte;
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
