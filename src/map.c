#include "common.h"
#include "sys/rarezip.h"

extern f32 gWorldX;
extern f32 gWorldZ;
extern Plane gFrustumPlanes[5];
#define MAX_RENDER_LIST_LENGTH 400
extern u32 gRenderList[MAX_RENDER_LIST_LENGTH];
extern s16 gRenderListLength;
#define MAX_BLOCKS 40
extern Block *gBlocksToDraw[MAX_BLOCKS];
extern s16 gBlocksToDrawIdx;
extern BlockTexture *gBlockTextures;
extern u32 *gFile_HITS_TAB;

u32 hits_get_size(s32 id);
void block_setup_vertices(Block *block);
void block_setup_gdl_groups(Block *block);
void func_80048B14(Block *block);
void *block_setup_textures(Block *block);
void block_setup_xz_bitmap(Block *block);
void block_load_hits(Block *block, s32, s32, void*);
void block_emplace(Block *block, s32 id, s32 param_3, s32 globalMapIdx);

void dl_set_all_dirty(void) {
    gDLBuilder->dirtyFlags = 0xFF;
    gDLBuilder->needsPipeSync = 1;
}

extern DLBuilder D_800B4A20;

void func_80040FF8(void) {
    gDLBuilder = &D_800B4A20;
    gDLBuilder->dirtyFlags = 0xFF;
    gDLBuilder->needsPipeSync = 1;
}

extern DLBuilder D_800B49F0;

void func_80041028(void) {
    gDLBuilder = &D_800B49F0;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/dl_apply_combine.s")
#else
void _dl_apply_combine(Gfx **gdl)
{
    Gfx *currGfx = &gDLBuilder->gfx0;
    u8 dirty;

    if (gDLBuilder->dirtyFlags & 0x1)
    {
        gDLBuilder->dirtyFlags &= ~0x1;
        dirty = TRUE;
    }
    else
    {
        dirty = gDLBuilder->gfx0.words.w0 != (**gdl).words.w0 || gDLBuilder->gfx0.words.w1 != (**gdl).words.w1;
    }

    if (dirty)
    {
        *currGfx = **gdl;

        if (gDLBuilder->needsPipeSync)
        {
            gDLBuilder->needsPipeSync = FALSE;

            gDPPipeSync((*gdl)++);

            **gdl = *currGfx;
        }

        (*gdl)++;
    }
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/dl_apply_other_mode.s")
#else
void _dl_apply_other_mode(Gfx **gdl)
{
    Gfx *currGfx = &gDLBuilder->gfx1;
    u8 dirty;

    if (UINT_80092a98 & 0x2000) {
        (**gdl).words.w1 &= ~0x30;
    }

    if (gDLBuilder->dirtyFlags & 0x2) {
        gDLBuilder->dirtyFlags &= ~0x2;
        dirty = TRUE;
    } else {
        dirty = gDLBuilder->gfx1.words.w0 != (**gdl).words.w0 || gDLBuilder->gfx1.words.w1 != (**gdl).words.w1;
    }

    if (dirty)
    {
        *currGfx = **gdl;

        if (gDLBuilder->needsPipeSync) {
            gDLBuilder->needsPipeSync = FALSE;
            gDPPipeSync((*gdl)++);
            **gdl = *currGfx;
        }

        (*gdl)++;
    }
}
#endif

void dl_apply_geometry_mode(Gfx **gdl)
{
    u8 dirty;

    if (UINT_80092a98 & 0x2000) {
        (**gdl).words.w1 &= ~0x1;
    }

    if (gDLBuilder->dirtyFlags & 0x40) {
        gDLBuilder->dirtyFlags &= ~0x40;
        dirty = TRUE;
    } else {
        dirty = gDLBuilder->geometryMode != (**gdl).words.w1;
    }

    if (dirty) {
        gDLBuilder->geometryMode = (**gdl).words.w1;
        (*gdl)++;
    }
}

void dl_set_geometry_mode(Gfx **gdl, u32 mode)
{
    mode |= gDLBuilder->geometryMode;
    gSPLoadGeometryMode(*gdl, mode);
    dl_apply_geometry_mode(gdl);
}

void dl_clear_geometry_mode(Gfx **gdl, u32 mode)
{
    mode = gDLBuilder->geometryMode & ~mode;
    gSPLoadGeometryMode(*gdl, mode);
    dl_apply_geometry_mode(gdl);
}

void dl_set_prim_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a)
{
    u32 rgba = RGBA8(r, g, b, a);
    u8 dirty;

    if (gDLBuilder->dirtyFlags & 0x4)
    {
        gDLBuilder->dirtyFlags &= ~0x4;
        dirty = TRUE;
    }
    else
    {
        dirty = rgba != gDLBuilder->primColor;
    }

    if (dirty)
    {
        gDLBuilder->primColor = rgba;
        gDPSetPrimColor((*gdl)++, 0, 0, r, g, b, a);
    }
}

void dl_set_prim_color2(Gfx **gdl, u8 r, u8 g, u8 b, u8 a)
{
    gDLBuilder->dirtyFlags |= 0x4;
    gDLBuilder->needsPipeSync = FALSE;
    dl_set_prim_color(gdl, r, g, b, a);
}

void dl_set_env_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a)
{
    u32 rgba = RGBA8(r, g, b, a);
    u8 dirty;

    if (gDLBuilder->dirtyFlags & 0x8)
    {
        gDLBuilder->dirtyFlags &= ~0x8;
        dirty = TRUE;
    }
    else
    {
        dirty = rgba != gDLBuilder->envColor;
    }

    if (dirty)
    {
        if (gDLBuilder->needsPipeSync)
        {
            gDLBuilder->needsPipeSync = FALSE;
            gDPPipeSync((*gdl)++);
        }

        gDLBuilder->envColor = rgba;
        gDPSetEnvColor((*gdl)++, r, g, b, a);
    }
}

void dl_set_env_color2(Gfx **gdl, u8 r, u8 g, u8 b, u8 a)
{
    gDLBuilder->dirtyFlags |= 0x8;
    gDLBuilder->needsPipeSync = FALSE;
    dl_set_env_color(gdl, r, g, b, a);
}

void dl_set_blend_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a)
{
    u32 rgba = RGBA8(r, g, b, a);
    u8 dirty;

    if (gDLBuilder->dirtyFlags & 0x80)
    {
        gDLBuilder->dirtyFlags &= ~0x80;
        dirty = TRUE;
    }
    else
    {
        dirty = rgba != gDLBuilder->blendColor;
    }

    if (dirty)
    {
        if (gDLBuilder->needsPipeSync)
        {
            gDLBuilder->needsPipeSync = FALSE;
            gDPPipeSync((*gdl)++);
        }

        gDLBuilder->blendColor = rgba;
        gDPSetBlendColor((*gdl)++, r, g, b, a);
    }
}

void dl_set_fill_color(Gfx **gdl, u32 color)
{
    u8 dirty;

    if (gDLBuilder->dirtyFlags & 0x10)
    {
        gDLBuilder->dirtyFlags &= ~0x10;
        dirty = TRUE;
    }
    else
    {
        dirty = color != gDLBuilder->fillColor;
    }

    if (dirty)
    {
        if (gDLBuilder->needsPipeSync)
        {
            gDLBuilder->needsPipeSync = FALSE;
            gDPPipeSync((*gdl)++);
        }

        gDLBuilder->fillColor = color;
        gDPSetFillColor((*gdl)++, color);
    }
}

void dl_set_fog_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a)
{
    u32 rgba = RGBA8(r, g, b, a);
    u8 dirty;

    if (gDLBuilder->dirtyFlags & 0x20)
    {
        gDLBuilder->dirtyFlags &= ~0x20;
        dirty = TRUE;
    }
    else
    {
        dirty = rgba != gDLBuilder->fogColor;
    }

    if (dirty)
    {
        if (gDLBuilder->needsPipeSync)
        {
            gDLBuilder->needsPipeSync = FALSE;
            gDPPipeSync((*gdl)++);
        }

        gDLBuilder->fogColor = rgba;
        gDPSetFogColor((*gdl)++, r, g, b, a);
    }
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/dl_triangles.s")
#else
void _dl_triangles(Gfx **gdl, DLTri *tris, s32 triCount)
{
    s32 n;
    DLTri *tri;

    for (n = triCount >> 1; n != 0; n--, tris += 2)
    {
        tri = tris;
        gSP2Triangles((*gdl)++, tri[0].v0, tri[0].v1, tri[0].v2, 0, tri[1].v0, tri[1].v1, tri[1].v2, 0);
    }

    if (triCount & 1) {
        tri = tris;
        gSP1Triangle((*gdl)++, tri[0].v0, tri[0].v1, tri[0].v2, 0);
    }

    gDLBuilder->needsPipeSync = TRUE;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041C30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041C6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041CA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041CE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041D20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041D5C.s")

u32 func_80041D74()
{
    return UINT_80092a98 & 0x10;
}

u32 func_80041D8C()
{
    return UINT_80092a98 & 0x100;
}

u32 func_80041DA4()
{
    return UINT_80092a98 & 0x80;
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041DBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041DD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041DEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041E08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041E24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80041E68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/init_maps.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80042174.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004225C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/track_c_func.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/draw_render_list.s")
#else
extern Gfx *gMainDL;
extern s16 SHORT_800b51dc;
extern u32 UINT_800b51e0;
extern BlockTexture *gBlockTextures;
typedef void (*DLL57Func)(u32*, u32*, u32*, u32*, u32*, u32*);
void _draw_render_list(Mtx *rspMtxs, s8 *visibilities)
{
    u32 oldBlockIdx = (u32)-1;
    Object **objects = get_world_objects(NULL, NULL);
    s32 i;
    u32 r, g, b;
    u32 unk0, unk1, unk2;
    s8 matrixStatus;

    ((DLL57Func)(*gDLL_57)[3])(&r, &g, &b, &unk0, &unk1, &unk2);

    for (i = 1; i < gRenderListLength; i++)
    {
        u32 renderItem = gRenderList[i];
        u32 index = (renderItem & 0x3f80) >> 7;

        if (renderItem & 0x40)
        {
            // Draw object
            func_800436DC(objects[index], visibilities[index]);
        }
        else
        {
            // Draw block
            Block *block;
            BlockShape *shape;
            Mtx *rspMtx;
            Texture *tex0;
            Texture *tex1;
            EncodedTri *ptri;
            EncodedTri *ptriend;
            Gfx *mygdl;
            u32 shapeIdx;
            u32 flags;
            s32 level;
            Vtx_t *pVerts;
            u32 blockIdx = renderItem & 0x3f;
            u32 force = 0;
            EncodedTri *ptrilast = NULL;

            if (blockIdx != oldBlockIdx)
            {
                matrixStatus = -1;
                block = gBlocksToDraw[blockIdx];
                oldBlockIdx = blockIdx;
                rspMtx = &rspMtxs[blockIdx * 2];
                SHORT_800b51dc = -1;
                UINT_800b51e0 = 0;
            }

            shape = &block->shapes[index];

            if (shape->flags & 0x20000000)
            {
                if (matrixStatus != 2) {
                    gSPMatrix(gMainDL++, OS_K0_TO_PHYSICAL(&rspMtx[1]), G_MTX_LOAD);
                    matrixStatus = 2;
                }
            }
            else
            {
                if (matrixStatus != 1) {
                    gSPMatrix(gMainDL++, OS_K0_TO_PHYSICAL(rspMtx), G_MTX_LOAD);
                    matrixStatus = 1;
                }
            }

            if (shape->tileIdx0 == 0xff) {
                tex0 = NULL;
            } else {
                tex0 = block->tiles[shape->tileIdx0].texture;
            }

            if (shape->flags & 0x2000)
            {
                if (tex0->flags & 0xc000) {
                    dl_set_prim_color(&gMainDL, 0xff, 0xff, 0xff, 0xa0);
                } else {
                    dl_set_prim_color(&gMainDL, 0xff, 0xff, 0xff, 0x64);
                }
            }
            else
            {
                if (shape->alpha == 0xff) {
                    dl_set_prim_color(&gMainDL, r, g, b, 0xff);
                } else if (shape->alpha == 0xfe) {
                    dl_set_prim_color(&gMainDL, 0xff, 0xff, 0xff, 0xff);
                } else if (shape->flags & 0x46c00000) {
                    dl_set_prim_color(&gMainDL, 0xff, 0xff, 0xff, 0xff);
                } else {
                    func_8001F848(&gMainDL);
                }
            }

            flags = shape->flags;
            level = 0;
            if (shape->flags & 0x10000)
            {
                Block_0x28Struct *bs = func_8004A284(block, shape->unk_0x14);
                if (bs != NULL) {
                    level = gBlockTextures[bs->texIdx].unk_0x4 << 8;
                    flags |= gBlockTextures[bs->texIdx].flags;
                } else {
                    level = 0;
                }

                if (SHORT_800b51dc != shape->unk_0x14 || level != UINT_800b51e0)
                {
                    force = 1;
                    SHORT_800b51dc = shape->unk_0x14;
                    UINT_800b51e0 = level;
                }
            }
            else
            {
                SHORT_800b51dc = -1;
            }

            if (shape->tileIdx1 != 0xff) {
                tex1 = block->tiles[shape->tileIdx1].texture;
            } else {
                tex1 = NULL;
            }

            set_textures_on_gdl(&gMainDL, tex0, tex1, flags, level, force, 0);

            if (shape->unk_0x16 != 0xff)
            {
                Struct0x22 *s = func_80049D68(shape->unk_0x16);
                gDPSetTileSize(gMainDL++, 0, s->uls0, s->ult0, tex0->width - 1, (tex0->height - 1) * 4);
                if (tex1 != NULL) {
                    gDPSetTileSize(gMainDL++, 1, s->uls1, s->ult1, tex1->width - 1, (tex1->height - 1) * 4);
                }
            }
            else
            {
                if (tex0 != NULL && (tex0->flags & 0xc000))
                {
                    gDPSetTileSize(gMainDL++, 0, 0, 0, tex0->width - 1, (tex0->height - 1) * 4);
                    if (tex1 != NULL) {
                        gDPSetTileSize(gMainDL++, 1, 0, 0, tex1->width - 1, (tex1->height - 1) * 4);
                    }
                }
            }

            shapeIdx = (shape - block->shapes) * 3;
            *gMainDL = block->gdlGroups[shapeIdx++];
            func_80041210(&gMainDL);
            *gMainDL = block->gdlGroups[shapeIdx++];
            dl_apply_combine(&gMainDL);
            *gMainDL = block->gdlGroups[shapeIdx++];
            dl_apply_other_mode(&gMainDL);

            mygdl = gMainDL;
            pVerts = block->vertices2[(block->vtxFlags & 0x1) ^ 0x1];
            ptri = &block->encodedTris[shape->triBase];
            ptriend = &block->encodedTris[shape[1].triBase];

            gSPVertex(gMainDL++, OS_K0_TO_PHYSICAL(&pVerts[shape->vtxBase]), shape[1].vtxBase - shape->vtxBase, 0);

            for (; ptri < ptriend; ptri++)
            {
                if (ptri->d1 & 0x1)
                {
                    if (ptrilast == NULL)
                    {
                        ptrilast = ptri;
                    }
                    else
                    {
                        // This appears to be a hand-optimized version of gSP2Triangles.
                        gMainDL->words.w0 = (G_TRI2 << 24) | ((ptrilast->d0 & 0x3f000) << 4) | ((ptrilast->d0 & 0xfc0) << 2) | (ptrilast->d0 & 0x3f);
                        gMainDL->words.w1 = ((ptri->d0 & 0x3f000) << 4) | ((ptri->d0 & 0xfc0) << 2) | (ptri->d0 & 0x3f);
                        gMainDL++;
                        ptrilast = NULL;
                    }
                }
            }

            if (ptrilast != NULL && (ptrilast->d1 & 0x1))
            {
                // This appears to be a hand-optimized version of gSP1Triangle.
                gMainDL->words.w0 = (G_TRI1 << 24) | ((ptrilast->d0 & 0x3f000) << 4) | ((ptrilast->d0 & 0xfc0) << 2) | (ptrilast->d0 & 0x3f);
                gMainDL++;
            }

            gDLBuilder->needsPipeSync = TRUE;

            if ((flags & 0x100408) == 0x100408)
            {
                u32 gfxCount = gMainDL - mygdl;

                dl_set_geometry_mode(&gMainDL, 0x10000);
                if (flags & 0x2004)
                {
                    gDPSetCombine(gMainDL, 0xffffff, 0xffff7dbe);
                    dl_apply_combine(&gMainDL);
                    gDPSetOtherMode(gMainDL, 0x080c00, 0xfa504a50);
                    dl_apply_other_mode(&gMainDL);
                }
                else
                {
                    gDPSetCombine(gMainDL, 0xffffff, 0xffff7dbe);
                    dl_apply_combine(&gMainDL);
                    gDPSetOtherMode(gMainDL, 0x080c00, 0xfa504dd8);
                    dl_apply_other_mode(&gMainDL);
                }

                bcopy(mygdl, gMainDL, gfxCount * sizeof(Gfx));
                gMainDL += gfxCount;

                gDLBuilder->needsPipeSync = TRUE;
            }
        }
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800436DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80043950.s")

// very close
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/block_add_to_render_list.s")
#else
extern Mtx *gWorldRSPMatrices;
void _block_add_to_render_list(Block *block, f32 x, f32 z)
{
    s32 oldRenderListLength = gRenderListLength;
    s32 i;
    MtxF mf, mf2;

    for (i = 0; i < block->shapeCount; i++)
    {
        if ((block->shapes[i].flags & 0x10000000) && gRenderListLength < MAX_RENDER_LIST_LENGTH)
        {
            s32 param;
            if (block->shapes[i].flags & 0x4) {
                param = 100000 - gBlocksToDrawIdx * 400 - i;
                if (block->shapes[i].flags & 0x2000) {
                    param -= 200;
                }
            } else {
                param = 200000 - gBlocksToDrawIdx * 400 - i;
            }

            gRenderList[gRenderListLength] = (param << 14) | (i << 7) | gBlocksToDrawIdx;
            gRenderListLength++;
        }
    }

    if (oldRenderListLength != gRenderListLength && gBlocksToDrawIdx < MAX_BLOCKS)
    {
        gBlocksToDraw[gBlocksToDrawIdx] = block;
        gBlocksToDrawIdx++;
        matrix_translation(&mf, x, 0.0f, z);
        matrix_f2l_4x3(&mf, gWorldRSPMatrices);
        gWorldRSPMatrices++;
        mf.m[3][1] = block->elevation;
        matrix_scaling(&mf2, 1.0f, 0.05f, 1.0f);
        matrix_concat(&mf2, &mf, &mf);
        matrix_f2l_4x3(&mf, gWorldRSPMatrices);
        gWorldRSPMatrices++;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80043FD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800441F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044320.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044448.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004454C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004478C.s")


#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/map_get_map_id_from_xz_ws.s")
#else
extern s32 gMapCurrentStreamCoords;
extern s32 D_80092A6C;
extern u32 *gDecodedGlobalMap;
s16 _map_get_map_id_from_xz_ws(f32 arg0, f32 arg1) {
    s32 temp_a1;
    s32 temp_v1;

    temp_a1 = floor_f(arg0 / 640.0f) - gMapCurrentStreamCoords;
    temp_v1 = floor_f(arg1 / 640.0f) - D_80092A6C;
    if ((temp_a1 < 0) || (temp_a1 >= 0x10)) {
        return -1;
    }
    if ((temp_v1 < 0) || (temp_v1 >= 0x10)) {
        return -1;
    }
    return *(gDecodedGlobalMap + (temp_v1 * 0x30 * 4) + (temp_a1 * 0xC));
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800448D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044A10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044A20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044A7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044B18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044B98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044BB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044BEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800451A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004530C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/some_cell_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045600.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800456AC.s")

u8 is_sphere_in_frustum(Vec3f *v, f32 radius)
{
    u8 i;

    for (i = 0; i < 5; i++)
    {
        if (gFrustumPlanes[i].x * (v->x - gWorldX) +
            gFrustumPlanes[i].y * v->y +
            gFrustumPlanes[i].z * (v->z - gWorldZ) +
            gFrustumPlanes[i].d +
            radius
            < 0.0f)
        {
            return FALSE;
        }
    }

    return TRUE;
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_load_streammap.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_load_streammap_add_to_table.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045D58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_find_streammap_index.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045DC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045F48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045FC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800462B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_convert_objpositions_to_ws.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046320.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046428.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046688.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046698.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800466C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046718.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046728.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/init_global_map.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_read_layout.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046B58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_update_streaming.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80047374.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800473BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80047404.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80047710.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80047724.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004773C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048024.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048034.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048054.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800483BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048478.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048498.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800484A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800485FC.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/block_load.s")
#else
extern u32 *gFile_BLOCKS_TAB;
extern u8 *gMapReadBuffer;
void block_load(s32 id, s32 param_2, s32 globalMapIdx, u8 queue)
{
    u32 allocSize;
    u32 offset;
    u32 compressedSize;
    u32 uncompressedSize;
    u8 *compressedData;
    Block *block;
    u8 *p;
    s32 n;
    s32 i;

    offset = gFile_BLOCKS_TAB[id];
    compressedSize = gFile_BLOCKS_TAB[id + 1] - offset;
    read_file_region(BLOCKS_BIN, gMapReadBuffer, offset, 0x10);

    uncompressedSize = *(u32*)gMapReadBuffer;
    allocSize = uncompressedSize + hits_get_size(id) + 0x8;
    block = malloc(allocSize, 5, NULL);
    if (block == NULL) {
        return;
    }

    compressedData = (u8*)block + allocSize - compressedSize - 0x10;
    compressedData = (u8*)((s32)compressedData - ((s32)compressedData % 16));
    // if ((s32)compressedData < 0) {
    //     // Align to 16 bytes
    //     u32 align = (u32)compressedData & 0xf;
    //     if (align != 0) {
    //         align -= 16;
    //     }
    //     compressedData -= align;
    // }

    read_file_region(BLOCKS_BIN, compressedData, offset, compressedSize);
    rarezip_uncompress(compressedData + 4, (u8*)block, allocSize);

    // Convert offsets to pointers
    block->vertices = (Vtx_t*)((u32)block->vertices + (u32)block);
    block->encodedTris = (EncodedTri*)((u32)block->encodedTris + (u32)block);
    block->shapes = (BlockShape*)((u32)block->shapes + (u32)block);
    block->unk_0x10 = (void*)((u32)block->unk_0x10 + (u32)block);
    block->tiles = (Block_0x0Struct*)((u32)block->tiles + (u32)block);

    func_8003CD6C(7);

    for (i = 0; i < block->textureCount; i++) {
        block->tiles[i].texture = texture_load(-((s32)block->tiles[i].texture | 0x8000), queue);
    }

    func_8003CD6C(6);

    block_setup_vertices(block);

    block->gdlGroups = (Gfx*)(block->gdlGroupsOffset + (u32)block);
    block_setup_gdl_groups(block);

    p = block->gdlGroups + block->shapeCount * 3;
    func_80048B14(block);

    if (block->vtxFlags & 0x8)
    {
        Vtx_t *vtx = align_8(p);
        Vtx_t *srcvtx = block->vertices;
        BlockShape *shape;

        block->vertices2[0] = vtx;
        block->vertices2[1] = &vtx[block->vtxCount];

        p = vtx + block->vtxCount * 2;

        shape = block->shapes;
        for (i = 0; i < block->vtxCount; i++)
        {
            if (shape->flags & 0x20000000) {
                vtx[i].ob[0] = (f32)srcvtx[i].ob[0];
                vtx[i].ob[1] = (srcvtx[i].ob[1] - block->elevation) * 20.0f;
                vtx[i].ob[2] = (f32)srcvtx[i].ob[2];
            } else {
                vtx[i].ob[0] = srcvtx[i].ob[0];
                vtx[i].ob[1] = srcvtx[i].ob[1];
                vtx[i].ob[2] = srcvtx[i].ob[2];
            }

            vtx[i].cn[0] = srcvtx[i].cn[0];
            vtx[i].cn[1] = srcvtx[i].cn[1];
            vtx[i].cn[2] = srcvtx[i].cn[2];
            vtx[i].cn[3] = srcvtx[i].cn[3];
            vtx[i].tc[0] = srcvtx[i].tc[0];
            vtx[i].tc[1] = srcvtx[i].tc[1];
            vtx[i].flag = srcvtx[i].flag;

            if (i >= shape[1].vtxBase) {
                shape++;
            }
        }

        bcopy(block->vertices2[0], block->vertices2[1], block->vtxCount * sizeof(Vtx_t));
    }
    else
    {
        block->vertices2[0] = block->vertices;
        block->vertices2[1] = block->vertices;
    }

    p = align_4(p);
    block->unk_0x28 = p;

    n = block_setup_textures(block);

    p = align_2(p + n);
    block->xzBitmap = p;
    block_setup_xz_bitmap(block);

    p = align_8(p + block->unk_0x34 * sizeof(s16));
    block_load_hits(block, id, queue, p);

    if (queue) {
        queue_block_emplace(1, block, id, param_2, globalMapIdx);
    } else {
        block_emplace(block, id, param_2, globalMapIdx);
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048B14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048C24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048D58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048E04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048F58.s")

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/block_emplace.s")
#else
extern u8 gLoadedBlockCount;
extern s16 *gLoadedBlockIds;
extern s8 *gBlockIndices[2]; // FIXME: how many?
extern u8 *gBlockRefCounts;
extern Block **gLoadedBlocks;
void _block_emplace(Block *block, s32 id, s32 param_3, s32 globalMapIdx)
{
    s32 slot;

    for (slot = 0; slot < gLoadedBlockCount; slot++) {
        if (gLoadedBlockIds[slot] == -1) {
            break;
        }
    }

    if (slot == gLoadedBlockCount) {
        gLoadedBlockCount++;
    }

    gBlockIndices[globalMapIdx][param_3] = slot;
    gLoadedBlocks[slot] = block;
    gLoadedBlockIds[slot] = id;
    gBlockRefCounts[slot] = 1;

    if (block->unk_0x3e != 0) {
        block_compute_vertex_colors(block, 0, 0, 1);
    }

    func_80058F3C();
}
#endif

// close
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/block_setup_gdl_groups.s")
#else
void _block_setup_gdl_groups(Block *block)
{
    s32 i;

    for (i = 0; i < block->shapeCount; i++)
    {
        BlockShape *shape;
        Texture *texture;
        s16 texFlags = 0;
        u32 flags;
        Gfx *mygdl;

        block->shapes[i].unk_0x16 = 0xff;
        
        shape = &block->shapes[i];
        flags = shape->flags;

        if (shape->tileIdx0 == 0xff) {
            texture = NULL;
        } else {
            texture = block->tiles[shape->tileIdx0].texture;
            if (texture != NULL) {
                texFlags = texture->flags;
            }
        }

        if (!(flags & 0x400) && (flags & 0x1000000)) {
            flags |= 0x1a;
        }

        if (texFlags & 0x80) {
            flags |= 0x2;
        }

        if (flags != 0) {
            flags &= ~0x1;
        } else {
            flags |= 0x1;
        }

        flags |= 0x2;

        if (flags & 0x400)
        {
            if (flags & 0x200)
            {
                flags &= ~0x200;
                func_8003E9B4(0x4);
            }
            else
            {
                if ((flags & 0x2000) || (flags & 0x4) || (flags & 0x100000)) {
                    flags |= 0x4;
                } else {
                    func_8003E9B4(0x4);
                }
            }
        }

        mygdl = &block->gdlGroups[i * 3];
        func_8003DC04(&mygdl, texture, flags | 0x80000000, 0, 1, 5);

        if ((flags & 0x2000) && texture != NULL && (texture->flags & 0xc000))
        {
            mygdl = &block->gdlGroups[i * 3];
            gSPLoadGeometryMode(mygdl++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
            if (texture->flags & 0xc000) {
                // TODO: decode constants
                gDPSetCombine(mygdl++, 0x22aa07, 0x1410933f);
            } else {
                gDPSetCombine(mygdl++, 0x45d207, 0x140cff36);
            }
            gDPSetOtherMode(mygdl++,
                G_PM_NPRIMITIVE | G_CYC_2CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | G_TT_NONE | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_MAGICSQ | G_AD_PATTERN,
                0x1049d8);
        }

        if (flags & 0x400) {
            func_8003E9D0(0x4);
        }
    }
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/block_setup_vertices.s")
#else
extern f32 FLOAT_8009a9c4; // 8191.0f
void _block_setup_vertices(Block *block)
{
    s32 i;

    for (i = 0; i < block->shapeCount; i++)
    {
        BlockShape *shape = &block->shapes[i];
        EncodedTri *ptri = &block->encodedTris[shape->triBase];
        EncodedTri *ptriend = &block->encodedTris[shape[1].triBase];
        Vtx_t *pverts = &block->vertices[shape->vtxBase];

        for (; ptri < ptriend; ptri++)
        {
            Vtx_t *verts[3];
            s16 vx[3];
            s16 vy[3];
            s16 vz[3];
            f32 nx, ny, nz;
            s32 inx, iny, inz;
            s32 j;
            f32 mag;
            u8 v0, v1, v2;

            v0 = ((u8*)ptri)[1];
            verts[0] = &pverts[v0];
            v1 = ((u8*)ptri)[2];
            verts[1] = &pverts[v1];
            v2 = ((u8*)ptri)[3];
            verts[2] = &pverts[v2];

            ptri->d0 = (v2 << 13) | (v1 << 7) | (v0 << 1);
            ptri->d1 = 0;


            for (j = 0; j < 3; j++)
            {
                vx[j] = verts[j]->ob[0];
                vy[j] = verts[j]->ob[1];
                vz[j] = verts[j]->ob[2];
            }

            nx = (vz[0] - vz[1]) * vy[2] + vy[0] * (vz[1] - vz[2]) + vy[1] * (vz[2] - vz[0]);
            ny = (vx[0] - vx[1]) * vz[2] + vz[0] * (vx[1] - vx[2]) + vz[1] * (vx[2] - vx[0]);
            nz = (vy[0] - vy[1]) * vx[2] + vx[0] * (vy[1] - vy[2]) + vx[1] * (vy[2] - vy[0]);
            mag = sqrtf(nx * nx + ny * ny + nz * nz);
            if (mag > 0.0f)
            {
                nx /= mag;
                ny /= mag;
                nz /= mag;
            }

            inx = nx * FLOAT_8009a9c4; /* 8191.0f */
            iny = ny * FLOAT_8009a9c4; /* 8191.0f */
            inz = nz * FLOAT_8009a9c4; /* 8191.0f */

            ptri->d0 |= inx << 18;
            ptri->d1 |= (iny & 0x3fff) << 4;
            ptri->d1 |= inz << 18;
            ptri->d1 |= 0x1;
        }

        if (shape->flags & 0x100000) {
            shape->flags |= 0x800;
        }
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800496E4.s")

u32 hits_get_size(s32 id) {
    u32 size = gFile_HITS_TAB[id + 1] - gFile_HITS_TAB[id];
    return size;
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/block_load_hits.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800499B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800499BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049B84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049CE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049D38.s")

// regalloc
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049D68.s")
#else
Struct0x22 *_func_80049D68(s32 idx)
{
    return &(*(Struct0x22**)0x800b97a8)[idx];
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049D88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/block_setup_textures.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049FA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A058.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A164.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A1E8.s")

Block_0x28Struct *func_8004A284(Block *block, u32 param_2)
{
    s32 i;

    for (i = 0; i < block->unk_0x48; i++)
    {
        if (block->unk_0x28[i].unk_0x2 == param_2) {
            return &block->unk_0x28[i];
        }
    }

    return NULL;
}

BlockTexture *func_8004A2CC(s32 idx)
{
    return &gBlockTextures[idx];
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/block_setup_xz_bitmap.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A528.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A5D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A67C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_update_objects_streaming.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004AEFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_should_stream_load_object.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B190.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B4A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B548.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B710.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_check_some_mapobj_flag.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B7D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B85C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B914.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B948.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B984.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004BC20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004BD40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004C040.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/block_compute_vertex_colors.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/warpPlayer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D328.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D470.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D698.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D844.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D880.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D8A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D974.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D984.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D99C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D9AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D9B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004DABC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004DBAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004E540.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004E64C.s")
#else
void func_8005BCE0(Object *object, u32 param_2, Gfx **gdl, Mtx **rspMtxs, u32 param_5, u32 param_6, u32 param_7, f32 y, u32 idx);
void _func_8004E64C(Object *object, Gfx **gdl, Mtx **rspMtxs, u32 param_4, u32 param_5)
{
    ObjectStruct64 *unk;
    Vec3f v0;
    Vec3f v1;

    unk = object->ptr0x64;
    if (unk->gdl != NULL)
    {
        if (unk->flags & 0x20)
        {
            bcopy(&object->srt.transl, &v0, sizeof(Vec3f));
            bcopy(&object->positionMirror, &v1, sizeof(Vec3f));
            bcopy(&unk->tr, &object->srt.transl, sizeof(Vec3f));

            if (object->parent != NULL) {
                transform_point_by_object(unk->tr.x, unk->tr.y, unk->tr.z, &unk->tr.x, &unk->tr.y, &unk->tr.z, object->parent);
            } else {
                bcopy(&unk->tr, &object->positionMirror, sizeof(Vec3f));
            }
        }

        if (unk->flags & 0x8) {
            // This function seems to be responsible for drawing shadows.
            func_8005BCE0(object, *(s16*)0x80092c18, gdl, rspMtxs, param_4, param_5, *(s8*)0x800bb170, *(f32*)0x800bb18c, unk->flags & 0x3);
        }

        if (unk->flags & 0x20) {
            bcopy(&v0, &object->srt.transl, sizeof(Vec3f));
            bcopy(&v1, &object->positionMirror, sizeof(Vec3f));
        }
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004E7A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004EEC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004F378.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004FA3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004FA4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004FA58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800502AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80050B88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800511E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800516BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051944.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051C54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051CFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051D68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051F64.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052148.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052230.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052300.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052644.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800528AC.s")
