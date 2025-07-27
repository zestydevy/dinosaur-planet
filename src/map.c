#include "common.h"
#include "sys/map.h"

static void func_8004D328();

// Can't be defined in .h file or else things unmatch
extern Camera* D_800B51E4;
extern Gfx* gMainDL;
extern Mtx* gWorldRSPMatrices;
extern Vertex* D_800B51D4;
extern Triangle* D_800B51D8;

void dl_set_all_dirty(void) {
    gDLBuilder->dirtyFlags = 0xFF;
    gDLBuilder->needsPipeSync = 1;
}

void func_80040FF8(void) {
    gDLBuilder = &D_800B4A20;
    gDLBuilder->dirtyFlags = 0xFF;
    gDLBuilder->needsPipeSync = 1;
}

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

// for some reason this function requires an extra or'ing of _SHIFTL(G_TRI2, 24, 8) in w1
#define gSP2Triangles(pkt, v00, v01, v02, flag0, v10, v11, v12, flag1)	\
{ \
	Gfx *_g = (Gfx *)(pkt); \
	_g->words.w0 = (_SHIFTL(G_TRI2, 24, 8)| __gsSP1Triangle_w1f(v00, v01, v02, flag0)); \
    _g->words.w1 = (_SHIFTL(G_TRI2, 24, 8)| __gsSP1Triangle_w1f(v10, v11, v12, flag1)); \
}
void dl_triangles(Gfx **gdl, DLTri *tris, s32 triCount)
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

//These all seem to get/set render-related bits!
void func_80041C30(s32 arg0) {
    if (arg0 != 0) {
        UINT_80092a98 |= 0x1000;
    } else {
        UINT_80092a98 &= ~0x1000;
    }
}

void func_80041C6C(s32 arg0) {
    if (arg0 != 0) {
        UINT_80092a98 |= 0x10;
    } else {
        UINT_80092a98 &= ~0x10;
    }
}

void func_80041CA8(s32 arg0) {
    if (arg0 != 0) {
        UINT_80092a98 |= 0x20;
        return;
    }
    UINT_80092a98 &= ~0x20;
}

void func_80041CE4(s32 arg0) {
    if (arg0 != 0) {
        UINT_80092a98 |= 0x40;
        return;
    }
    UINT_80092a98 &= ~0x40;
}

void func_80041D20(s32 arg0) {
    if (arg0 != 0) {
        UINT_80092a98 &= ~0x2000;
        return;
    }
    UINT_80092a98 |= 0x2000;
}

s32 func_80041D5C(void) {
    return (UINT_80092a98 & 0x2000) == 0;
}

u32 func_80041D74()
{
    return UINT_80092a98 & 0x10;
}

// Get widescreen-related bit?
u32 func_80041D8C()
{
    return UINT_80092a98 & 0x100;
}

u32 func_80041DA4()
{
    return UINT_80092a98 & 0x80;
}

s32 func_80041DBC(void) {
    return UINT_80092a98 & 8;
}

s32 func_80041DD4(void) {
    return UINT_80092a98 & 0x1000;
}

void func_80041DEC(void) {
    UINT_80092a98 |= 0x800;
}

/* 
 * Get a different widescreen-related bit? It can stack with the other one!
 * Maybe related to the unimplemented cinematic aspect vs. widescreen option
 * in the video settings, like in GE/PD?
 */
s32 func_80041E08(void) {
    return UINT_80092a98 & 0x10000;
}

void func_80041E24(s32 arg0) {
    if (arg0 != 0) {
        UINT_80092a98 |= 0x20000;
        return;
    }
    UINT_80092a98 &= 0xFFFDFFFF;
}

s32 func_80041E68(void) {
    return UINT_80092a98 & 0x20000;
}

void init_maps(void) {
    s32 i;

    UINT_80092a98 = 0;
    D_800B97C0 = malloc(0x9F6, 5, NULL);
    gLoadedBlocks = malloc(0xA0, 5, NULL);
    gLoadedBlockIds = malloc(0x50, 5, NULL);
    gBlockRefCounts = malloc(0x28, 5, NULL);
    gMapReadBuffer = malloc(0x2BC, 5, NULL);
    *gBlockIndices = malloc(0x500, 5, NULL);
    *gDecodedGlobalMap = malloc(0x3C00, 5, NULL);
    *D_800B9700 = malloc(0x500, 5, NULL);
    for (i = 1; i < 5; i++) {
        gBlockIndices[i] = gBlockIndices[i - 1] + 256;
        gDecodedGlobalMap[i] = gDecodedGlobalMap[i - 1] + 256;
        D_800B9700[i] = D_800B9700[i - 1] + 256;
    }
    queue_alloc_load_file((void **) &gFile_MAPS_TAB, 0x20);
    queue_alloc_load_file((void** ) &gFile_HITS_TAB, 0x2D);
    for (i = 0; i < 120; i++) { gLoadedMapsDataTable[i] = NULL; }
    queue_alloc_load_file((void** ) &gFile_TRKBLK, 0x2B);
    gNumTRKBLKEntries = 0;
    while (gFile_TRKBLK[gNumTRKBLKEntries] != 0xFFFF) {
        gNumTRKBLKEntries += 1;
    }
    gNumTRKBLKEntries -= 1;
    queue_alloc_load_file((void **) &gFile_BLOCKS_TAB, 0x2A);
    gNumTotalBlocks = 0;
    while (gFile_BLOCKS_TAB[gNumTotalBlocks] != -1) {
        gNumTotalBlocks++;
    }
    gNumTotalBlocks--;
    D_800B96B0 = malloc(0x7D0, 5, NULL);
    D_800B4A5C = -1;
    D_800B4A5E = -2;
    gBlockTextures = malloc(0x140, 5, NULL);
    bzero(gBlockTextures, 0x140);
    D_800B97A8 = malloc(0x7B4, 5, NULL);
    bzero(D_800B97A8, 0x7B4);
    bzero(gRenderList, 0x640);
    *gRenderList = -0x4000U;
}

void func_80042174(s32 arg0) {
    if (arg0 == 0) {
        if (UINT_80092a98 & 0x10) {
            gDLL_minic->exports->func2.asVoid();
        }
        gDLL_8->exports->func[2].asVoid();
        gDLL_7_newday->exports->func2.asVoid();
        gDLL_newclouds->exports->func[3].asVoid();
        func_80049D88();
        func_800499BC();
        func_8004D328();
        if (gDLL_76 != NULL) {
            gDLL_76->exports->func1.asVoid();
        }
        gDLL_minimap->exports->func[0].asVoid();
    }
    func_8001EB80();
}

void func_8004225C(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, Vertex** vtxs2, Triangle** pols2) {
    Mtx* temp_v0;

    gMainDL = *gdl;
    gWorldRSPMatrices = *mtxs;
    D_800B51D4 = *vtxs;
    D_800B51D8 = *pols;
    UINT_80092a98 |= 0x21;
    if ((D_800B96A8 == 1) || (D_800B96A8 == 3)) {
        UINT_80092a98 &= ~1;
    }
    gSPTexture(gMainDL++, -1, -1, 3, 0, 1);
    temp_v0 = get_some_model_view_mtx();
    gSPMatrix(gMainDL++, OS_K0_TO_PHYSICAL(temp_v0), G_MTX_LOAD);
    func_800021A0(&gMainDL, 0);
    func_80044BEC();
    if (func_80010048() != 0) {
        if (!(UINT_80092a98 & 8)) {
            UINT_80092a98 |= 8;
        }
        camera_set_aspect(D_8009A950);
    } else if (UINT_80092a98 & 8) {
        UINT_80092a98 &= ~8;
        camera_set_aspect(D_8009A954);
    }
    if (UINT_80092a98 & 0x10000) {
        if (UINT_80092a98 & 8) {
            camera_set_aspect(D_8009A958);
        } else {
            camera_set_aspect(D_8009A95C);
        }
        func_80001D58(get_camera_selector(), 0U);
        some_video_setup(0);
        UINT_80092a98 &= 0xFFFEFFFF;
    }
    if (UINT_80092a98 & 0x10) {
        setup_rsp_camera_matrices(&gMainDL, &gWorldRSPMatrices);
        gDLL_7_newday->exports->func13(&gMainDL, &gWorldRSPMatrices);

        if (UINT_80092a98 & 0x40) {
            gDLL_newstars->exports->func[1].withOnePointerArg(&gMainDL);
        }
        gDLL_7_newday->exports->func3(&gMainDL, &gWorldRSPMatrices, UINT_80092a98 & 0x40);
    } else {
        setup_rsp_camera_matrices(&gMainDL, &gWorldRSPMatrices);
    }
    gDLL_newlfx->exports->func[2].asVoid();
    gDLL_57->exports->func[3].asVoid();
    gDLL_58->exports->func[2].asVoid();
    if (UINT_80092a98 & 0x20000) {
        if (gDLL_7_newday->exports->func23(&gMainDL) == 0) {
            gDLL_8->exports->func[3].withOnePointerArg(&gMainDL);
        }
    } else {
        gDLL_8->exports->func[3].withOnePointerArg(&gMainDL);
    }
    D_800B51E4 = get_camera();
    func_80048F58();
    track_c_func();
    gDLL_newclouds->exports->func[4].withOnePointerArg(&gMainDL);
    func_8000302C(&gMainDL);
    *gdl = gMainDL;
    *mtxs = gWorldRSPMatrices;
    *vtxs = D_800B51D4;
    *pols = D_800B51D8;
    UINT_80092a98 &= ~2;
    // @fake
    if (1) { } if (1) { } if (1) { } if (1) { }
}

// https://decomp.me/scratch/9mgpH
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

void func_800436DC(Object* arg0, s32 arg1) {
    s8 sp37;
    u8 someBool;

    someBool = TRUE;
    if ((arg0->id == 0x72) || (arg0->id == 0x38C)) {
        someBool = TRUE;
        if (arg0->dll->exports->func13(arg0) != 0) {
            someBool = FALSE;
        }
    }
    if (someBool != FALSE) {
        sp37 = gDLL_expgfx->exports->func10(arg0);
    }
    gDLL_modgfx->exports->func[6].custom(&gMainDL, &gWorldRSPMatrices, &D_800B51D4, 1, arg0);
    if (sp37 >= 2) {
        if ((arg0->id != 0x72) && (arg0->id != 0x38C)) {
            gDLL_expgfx->exports->func6(arg0, &gMainDL, &gWorldRSPMatrices, &D_800B51D4, 1, 0, 0);
        }
    }
    objprint_func(&gMainDL, &gWorldRSPMatrices, &D_800B51D4, &D_800B51D8, arg0, arg1);
    if (sp37 != 0) {
        if ((arg0->id != 0x72) && (arg0->id != 0x38C)) {
            gDLL_expgfx->exports->func6(arg0, &gMainDL, &gWorldRSPMatrices, &D_800B51D4, 0, 0, 0);
        }
    }
    if ((arg0->linkedObject != NULL) && (arg1 != 0)) {
        sp37 = gDLL_expgfx->exports->func10(arg0->linkedObject);
        if (sp37 >= 2) {
            gDLL_expgfx->exports->func6(arg0->linkedObject, &gMainDL, &gWorldRSPMatrices, &D_800B51D4, 1, 0, 0);
        }
        if (sp37 != 0) {
            gDLL_expgfx->exports->func6(arg0->linkedObject, &gMainDL, &gWorldRSPMatrices, &D_800B51D4, 0, 0, 0);
        }
    }
}


#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80043950.s")
#else
typedef struct {
    /*0x0*/  u8 *data;
    /*0x4*/  s32 byteLength;
    /*0x8*/  s32 bitLength;
    /*0xC*/  s32 capacity;
    /*0x10*/ s32 bitPos;
} BitStream;
typedef struct Arg0Unk4 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 pad6[0xF - 0x6];
} Arg0Unk4;
typedef struct Arg0Unk8 {
    s32 unk0;
    s32 unk4;
} Arg0Unk8;
typedef struct Arg0UnkC {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
    u8 pad10[0x17 - 0x10];
    u8 unk17;
} Arg0UnkC;

typedef struct UnkArg0 {
    s32 pad0;
    Arg0Unk4 *unk4;
    Arg0Unk8 *unk8;
    Arg0UnkC *unkC;
    u8 pad10[0x36 - 0x10];
    s16 unk36;
} UnkArg0;
void func_80043950(UnkArg0* arg0, s16 arg1, s16 arg2, s16 arg3);
s32 func_80045600(s32, BitStream*, s16, s16, s32);          /* extern */
extern Plane D_800B4ADC; // ????? unsure about the type of this
extern BitStream D_800B9780;
extern u8 D_800B979C;
extern f32 D_800B97B8;
extern f32 D_800B97BC;
void func_80043950(UnkArg0* arg0, s16 arg1, s16 arg2, s16 arg3) {
    s32 i;
    Arg0Unk8* temp_v0;
    Arg0Unk8* var_a1;
    Arg0UnkC* var_s0;
    Plane* var_v0;
    Arg0Unk4* unk4;
    f32 var_fa0;
    f32 var_fs1;
    f32 var_fs2;
    f32 var_fs3;
    f32 var_fs4;
    f32 var_fv1;
    f32 var_f0;
    Arg0UnkC *sp98[1];
    Arg0Unk4* temp_v0_2;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    s32 var_s1;
    s32 pad[4];
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;

    temp_s2 = D_800B51E4->tx - gWorldX - D_800B97B8;
    temp_s3 = D_800B51E4->ty;
    temp_s4 = D_800B51E4->tz - gWorldZ - D_800B97BC;
    var_s0 = &arg0->unkC[0];
    if ((u32)var_s0 >= (u32)&arg0->unkC[arg0->unk36]) {
        return;
    }

    sp98[0] = &arg0->unkC[arg0->unk36];
    do {
        if (var_s0->unk0 & 0x200000) {
            var_s0->unk0 &= 0xEFFFFFFF;
            var_s0++;
            continue;
        }
        if (
            ((u8) D_800B9794 != 0) &&
            ((D_800B979C & 1) || !(var_s0->unk0 & 0x2404)) &&
            // var_s0 - arg0->unkC is just current "index"
            (func_80045600((var_s0 - arg0->unkC) / 24, &D_800B9780, arg1, arg2, arg3) == 0)
        ) {
            var_s0->unk0 &= 0xEFFFFFFF;
            var_s0++;
            continue;
        } 

        var_s1 = 1;
        sp84 = ((var_s0->unkD * 4) | ((var_s0->unk17 >> 4) & 3)) + D_800B97B8;
        sp90 = ((var_s0->unkC * 4) | (var_s0->unk17 & 3)) + D_800B97B8;
        sp8C = var_s0->unk8;
        sp80 = var_s0->unkA;
        sp7C = ((var_s0->unkF * 4) | ((var_s0->unk17 >> 6) & 3)) + D_800B97BC;
        sp88 = ((var_s0->unkE * 4) | ((var_s0->unk17 >> 2) & 3)) + D_800B97BC;
        for (i = 0; i < 5; i++) {
            var_v0 = &gFrustumPlanes[i];
            if (var_v0->unk_0x14[0] & 1) {
                var_fs1 = sp84;
                var_fs4 = sp90;
            } else {
                var_fs1 = sp90;
                var_fs4 = sp84;
            }
            if (var_v0->unk_0x14[0] & 2) {
                var_fs2 = sp80;
                var_fs3 = sp8C;
            } else {
                var_fs2 = sp8C;
                var_fs3 = sp80;
            }
            if (var_v0->unk_0x14[0] & 4) {
                var_fv1 = sp7C;
                var_fa0 = sp88;
            } else {
                var_fv1 = sp88;
                var_fa0 = sp7C;
            }
            var_f0 = (var_v0->d + ((var_fs1 * var_v0->x) + (var_fs2 * var_v0->y) + (var_fv1 * var_v0->z)));
            if (var_f0 < 0.0f) {
                var_f0 = (var_v0->d + ((var_fs4 * var_v0->x) + (var_fs3 * var_v0->y) + (var_fa0 * var_v0->z)));
                if (var_f0 < 0.0f) {
                    var_s1 = 0;
                    break;
                }
            }
        }
        if (var_s1 == 0) {
            var_s0->unk0 &= 0xEFFFFFFF;
            var_s0++;
            continue;
        } 
        if (!(var_s0->unk0 & 0x80000000)) {
            var_s1 = 0;
            unk4 = &arg0->unk4[var_s0->unk4];
            var_a1 = &arg0->unk8[var_s0->unk6];
            temp_v0 = arg0->unk8;
            temp_v0 += var_s0[1].unk6;
            while ((u32) var_a1 < (u32)temp_v0) {
                temp_v0_2 = &unk4[((var_a1->unk0 >> 0xD) & 0x1F)];
                if (
                    (
                        ((temp_v0_2->unk0 - temp_s2) * (var_a1->unk0 >> 0x12)) +
                        ((temp_v0_2->unk2 - temp_s3) * ((var_a1->unk4 << 0xE) >> 0x12)) +
                        ((temp_v0_2->unk4 - temp_s4) * (var_a1->unk4 >> 0x12))
                    ) < 0) {
                    var_a1->unk4 |=1;
                    var_s1 = 1;
                } else {
                    var_a1->unk4 &= ~1;
                }
                var_a1++;
            }
            if (var_s1 == 0) {
                var_s0->unk0 &= 0xEFFFFFFF;
                var_s0++;
                continue;
            } 
        }
        var_s0->unk0 |= 0x10000000;
        var_s0++;
    } while ((u32)var_s0 < (u32)sp98[0]);
}
#endif

void block_add_to_render_list(Block *block, f32 x, f32 z)
{
    s32 unused;
    s32 oldRenderListLength;
    s32 i;
    s32 param;
    s32 pad;
    MtxF mf;
    MtxF mf2;
    
    oldRenderListLength = gRenderListLength;

    for (i = 0; i < block->shapeCount; i++) {
        if ((block->shapes[i].flags & 0x10000000) && gRenderListLength < MAX_RENDER_LIST_LENGTH) {
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

    if (((oldRenderListLength & 0xFFFFFFFFu) != gRenderListLength) && gBlocksToDrawIdx < MAX_BLOCKS) {
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

void func_80043FD8(s8* arg0) {
    Object* object;
    Object** objects;
    s32 sp5C;
    s32 sp58;
    s32 i;
    s32 var_v0;
    s8* var_s2;

    objects = get_world_objects(0, 0);
    sp58 = func_80020DA0(&sp5C);
    if (sp5C >= 0xB5) {
        sp5C = 0xB4;
    }
    func_80020EE4(sp58, sp5C - 1);
    for (i = 0; i < sp5C; i++) {
        object = objects[i];
        var_s2 = &arg0[i];
        if (i < sp58) {
            var_s2[0] = 0;
        } else {
            var_s2[0] = func_800456AC(object);
            if ((var_s2[0] != 0) && (object->ptr0x64 != NULL) && (object->def->shadowType == OBJ_SHADOW_GEOM)) {
                func_8004DBAC(object, 0, 0, delayByte);
            }
            if ((object->ptr0x64 != NULL) && (object->def->shadowType == OBJ_SHADOW_BOX))  {
                func_8004E7A8(object);
            }
            if (gRenderListLength < MAX_RENDER_LIST_LENGTH) {
                if (object->def->flags & 0x100000) {
                    var_v0 = 0x249F0 - i;
                } else if ((object->unk_0x37 == 0xFF) && !(object->srt.flags & 0x80)) {
                    var_v0 = 0x249F0 - i;
                } else {
                    var_v0 = i + 0xC350;
                }
                gRenderList[gRenderListLength] = (var_v0 << 14) | (i << 7) | 0x40;
                gRenderListLength += 1;
            }
        }
    }
    if (gRenderListLength >= 2) {
        func_800441F4(gRenderList, gRenderListLength);
    }
}

void func_800441F4(u32* arg0, s32 arg1) {
    u32 temp_a0;
    s32 var_t0;
    s32 var_v0;
    s32 var_v1;

    for (var_v0 = 1; (arg1 - 1) / 9 >= var_v0; var_v0++) {
        var_v0 *= 3;
        // @fake
        var_v1 = var_v0 + 1;
    }

    while (var_v0 > 0) {
        var_v1 = var_v0 + 1;
        while (var_v1 < arg1) {
            temp_a0 = arg0[var_v1];
            var_t0 = var_v1;
            if (var_v0 < var_t0) {
                while (arg0[var_t0 - var_v0] < temp_a0) {
                    arg0[var_t0] = arg0[var_t0 - var_v0];
                    var_t0 -= var_v0;
                    if (var_v0 >= var_t0) {
                        break;
                    }
                }
            }
            var_v1++;
            arg0[var_t0] = temp_a0;
        }
        var_v0 /= 3;
    }
}

/** are_worldCoords_in_current_map */
s32 func_80044320(f32 worldX, f32 worldZ) {
    s32 localGridX;
    s32 localGridZ;
    s32 temp;

    temp = floor_f((worldX - gWorldX) / BLOCKS_GRID_UNIT);
    localGridX = temp + gMapCurrentStreamCoordsX + gMapActiveStreamMap->originOffsetX - floor_f(gMapActiveStreamMap->originWorldX / BLOCKS_GRID_UNIT);
    temp = floor_f((worldZ - gWorldZ) / BLOCKS_GRID_UNIT);
    localGridZ = temp + gMapCurrentStreamCoordsZ + gMapActiveStreamMap->originOffsetZ - floor_f(gMapActiveStreamMap->originWorldZ / BLOCKS_GRID_UNIT);
    
    
    if (localGridX < 0 || localGridZ < 0 || localGridX >= gMapActiveStreamMap->gridSizeX || localGridZ >= gMapActiveStreamMap->gridSizeZ) {
        return 0;
    }
    return 1;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044448.s")
#else
extern u32 D_800B5468; //?

/** Search all loaded maps for object with uID? */
ObjCreateInfo* func_80044448(s32 match_uID, s32* match_indexInMap, s32* match_mapID, s32* arg3, s32* arg4) {
    MAPSHeader **map_ptr;
    s32 mapID;
    MAPSHeader *map;
    s32 object_offset;
    s32 object_indexInMap;
    ObjCreateInfo *obj;
    
    for (mapID = 0; mapID < 0x78; mapID++){
        map_ptr = &gLoadedMapsDataTable[mapID];
        map = gLoadedMapsDataTable[mapID];
        if (!map)
            continue;
    
        gMapActiveStreamMap = map;
        obj = (ObjCreateInfo*)map->objectInstanceFile_ptr;
        
        for (object_indexInMap = 0, object_offset = 0; object_offset < map->objectInstancesFileLength; object_indexInMap++){
            if (match_uID == obj->unk14){
                if (match_indexInMap){
                    *match_indexInMap = object_indexInMap;
                }

                if (match_mapID){
                    *match_mapID = mapID;
                }

                if (arg3){
                    *arg3 = gMapActiveStreamMap->unk19;
                }
                if (arg4){
                    if ((u32) map_ptr >= (u32)&D_800B5468){ //If beyond mapID 80??
                        *arg4 = 1;
                        return obj;
                    }
                    *arg4 = 0;
                }
                
                return obj;
            }

            object_offset += obj->unk2 << 2;
            obj = (ObjCreateInfo *) (map->objectInstanceFile_ptr + object_offset);
        }
    }
    return 0;
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004454C.s")
#else

extern u32 D_800B96E4; // end of gBlockIndices which should be iterated over in a for (i = 0; i < 5; i++) loop

#define BLOCKS_TOLERANCE_Y 50

//map_get_blockIndex_at_worldCoords?
s32 func_8004454C(f32 x, f32 y, f32 z) {
    s32 blockIndex;
    s8 loadedBlockIndex1;
    s8 loadedBlockIndex2;
    s8 loadedBlockIndex3;
    BlocksModel *block1;
    BlocksModel *block2;
    BlocksModel *block3;
    s32 gridX;
    s32 gridZ;
    s8 *nextLayer;
    s8 *blocksLayerIndices;
    s8 *temp_v1_4;
    s8 **layerB;
    
    gridX = floor_f(x / BLOCKS_GRID_UNIT) - gMapCurrentStreamCoordsX;
    gridZ = floor_f(z / BLOCKS_GRID_UNIT) - gMapCurrentStreamCoordsZ;
    
    if (gridX < 0 || gridX >= BLOCKS_GRID_SPAN){
        return -1;
    }
    if (gridZ < 0 || gridZ >= BLOCKS_GRID_SPAN){
        return -1;
    }
    
    blocksLayerIndices = gBlockIndices[0];    
    if (blocksLayerIndices[(gridZ * 16) + gridX] >= 0){
        loadedBlockIndex1 = blocksLayerIndices[blockIndex];
        block1 = gLoadedBlocks[loadedBlockIndex1];
        
        //Check if within bounds of block (along Y axis)
        if ((f32) (block1->minY - BLOCKS_TOLERANCE_Y) < y && y < (f32) (block1->maxY + BLOCKS_TOLERANCE_Y)) {
            return (s32) loadedBlockIndex1;
        }
    }
    
    layerB = &gBlockIndices[1]; //layer1?
    
    while (1){
        temp_v1_4 = *layerB;
        if (temp_v1_4[(gridZ * 16) + gridX] >= 0){
            loadedBlockIndex3 = temp_v1_4[blockIndex];
            block3 = gLoadedBlocks[loadedBlockIndex3];
            
            //Check if within bounds of block (along Y axis)
            if ((f32) (block3->minY - BLOCKS_TOLERANCE_Y) < y && y < (f32) (block3->maxY + BLOCKS_TOLERANCE_Y)) {
                return (s32) loadedBlockIndex3;
            }
        }
        
        *nextLayer = layerB + 4;
        layerB += 8;
        loadedBlockIndex2 = *(nextLayer + blockIndex);
        if (loadedBlockIndex2 >= 0){
            block2 = gLoadedBlocks[loadedBlockIndex2];
            
            //Check if within bounds of block (along Y axis)
            if ((f32) (block2->minY - BLOCKS_TOLERANCE_Y) < y && y < (f32) (block2->maxY + BLOCKS_TOLERANCE_Y)){
                return (s32) loadedBlockIndex2;
            }
        }
        
        if ((void *)layerB == &D_800B96E4){
            return -1;
        }
    }
}

#endif

/** get_block_world_space_origin? */
void func_8004478C(f32 worldX, f32 worldY, f32 worldZ, f32* blockWorldOriginX, f32* blockWorldOriginZ) {
    s32 worldGridX;
    s32 worldGridZ;

    worldGridX = floor_f(worldX / BLOCKS_GRID_UNIT);
    worldGridZ = floor_f(worldZ / BLOCKS_GRID_UNIT);
    
    *blockWorldOriginX = (f32) worldGridX * BLOCKS_GRID_UNIT;
    *blockWorldOriginZ = (f32) worldGridZ * BLOCKS_GRID_UNIT;
}

s16 map_get_map_id_from_xz_ws(f32 worldX, f32 worldZ){
    s32 gridX;
    s32 gridZ;
    GlobalMapCell *layer;

    gridX = floor_f(worldX / BLOCKS_GRID_UNIT) - gMapCurrentStreamCoordsX;
    gridZ = floor_f(worldZ / BLOCKS_GRID_UNIT) - gMapCurrentStreamCoordsZ;
    
    if (gridX < 0 || gridX >= 0x10){
        return -1;
    }
    if (gridZ < 0 || gridZ >= 0x10){
        return -1;
    }

    layer = gDecodedGlobalMap[0];
    return layer[gridZ*16 + gridX].mapIDs[0];
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800448D0.s")
#else
s16 func_800448D0(s32 arg0) {
    s16 mapID;
    s16 var_v0;
    s16* var_a0;
    GlobalMapCell *cellInfo;
    s16* var_t2;
    GlobalMapCell *layer;
    s32 cellIndex;
    s32 cellIndex_2;
    s32 var_a2;
    s32 mapID_wasFound;
    s32 var_t1;
    u32 objects_data_end;
    u8 *object_ptr;
    ObjCreateInfo *obj;
    MAPSHeader* temp_v1;
    

    var_v0 = 0;
    layer = gDecodedGlobalMap[0];

    for (cellIndex = 0; cellIndex < BLOCKS_GRID_TOTAL_CELLS; cellIndex++){
        var_a2 = 0;
        cellInfo = &(layer[cellIndex]);
        while (var_a2 != 6){
            mapID = cellInfo->mapIDs[0];
            var_a2 += 2;
            if (mapID >= 0 && mapID < MAP_ID_MAX) {
                mapID_wasFound = 0;
                if (&gLoadedMapsDataTable[mapID] != 0) {
                    var_t1 = 0;
                    if (var_v0 > 0) {
                        // var_t2 = &sp20[0];

                        while(var_t1 != var_v0){
                            var_t1 += 1;
                            if (*var_t2 == mapID) {
                                mapID_wasFound = 1;
                                break;
                            }
                            var_t2 += 2;
                        }
                        
                    }
                    if (mapID_wasFound == 0) {
                        //(&sp20[0])[var_v0] = mapID;
                        var_v0 += 1;
                    }
                }
            }
            cellInfo += 2;
        }
    }
    
    cellIndex_2 = 0;
    if (var_v0 > 0) {
        //var_a0 = &sp20[0];
        while (cellIndex_2 != var_v0){
            mapID = *var_a0;
            cellIndex_2 += 1;
            temp_v1 = gLoadedMapsDataTable[mapID];
            if (temp_v1 != NULL) {
                object_ptr = (u8*)temp_v1->objectInstanceFile_ptr;
                objects_data_end = (u32)(temp_v1->objectInstancesFileLength + object_ptr);
    
                while (*object_ptr < objects_data_end){ //iterate through objects list
                    obj = (ObjCreateInfo*)object_ptr;
                    if (*object_ptr == arg0) //checking for particular object/offset in the object instance file?
                        return mapID;
                    object_ptr += obj->unk2 << 2; //move to next object in list
                }
                
            }
            var_a0 += 2;
        }
    }
    return -1;
}
#endif

MapHeader* func_80044A10(void) {
    return (MapHeader*)gLoadedMapsDataTable;
}

/** Assign object instance file length and get object instance file from map */
s32 func_80044A20(f32 worldX, f32 worldZ, s32* objectsFileLength) {
    s32 mapID;
    MapHeader *map;

    mapID = map_get_map_id_from_xz_ws(worldX, worldZ);
    if (mapID != -1){
      *objectsFileLength = gLoadedMapsDataTable[mapID]->objectInstancesFileLength;
      return (s32)gLoadedMapsDataTable[mapID]->objectInstanceFile_ptr;
    }
    return 0;
}

/** Assign blockIndex from worldX/Z */
s32 func_80044A7C(s32 worldX, s32 worldZ, s32* blockIndex) {
    s8 *blocksLayer;
    s32 *new_var2;
      
    blocksLayer = gBlockIndices[0];
      
    worldX = floor_f((f32) worldX / BLOCKS_GRID_UNIT);
    worldZ = floor_f((f32) worldZ / BLOCKS_GRID_UNIT);
  
    new_var2 = &gMapCurrentStreamCoordsX;
      
    *blockIndex = blocksLayer[(worldZ - gMapCurrentStreamCoordsZ) * 16 + (worldX - *new_var2)];
    return 1;
}

/** Get Block from visGrid cell */
BlocksModel* func_80044B18(s32 visGridX, s32 visGridZ, s32 mapLayer) { 
    s8 *blocksLayer;
    s8 blockIndex;

    blocksLayer = gBlockIndices[mapLayer];
    
    if (visGridX < 0 || visGridZ < 0 || visGridX >= 0x10 || visGridZ >= 0x10){
        return 0;
    }

    blockIndex = blocksLayer[visGridZ*16 + visGridX];
    
    if (blockIndex < 0 || blockIndex >= gLoadedBlockCount)
        return 0;
    return (BlocksModel*)gLoadedBlocks[blockIndex];
}

/** Get visGrid layer */
s8* func_80044B98(s32 arg0) {
    return gBlockIndices[arg0];
}

/** Get Block from blockIndex */
BlocksModel* func_80044BB0(s32 blockIndex) {
    if (blockIndex < 0 || blockIndex >= gLoadedBlockCount) {
        return 0;
    }
    return gLoadedBlocks[blockIndex];
}

//Camera and frustum related?
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80044BEC.s")

s32 func_800451A0(s32 xPos, s32 zPos, BlocksModel* blocks) {
    Plane* currentPlane;
    f32 scaledXPos;
    f32 scaledZPos;
    f32 planeZ;
    f32 planeX;
    f32 planeY;
    f32 planeD;
    f32 var_fs2;
    f32 minY;
    f32 maxY;
    s32 stop;
    s32 i;
    s32 j;

    if (blocks != NULL) {
        minY = blocks->minY;
        maxY = blocks->maxY;
    } else {
        minY = D_8009A9B8;
        maxY = D_8009A9BC;
    }

    for (i = 0; i < 5; i++) {
        currentPlane = &gFrustumPlanes[i];

        scaledXPos = xPos * 640.0f;
        scaledZPos = zPos * 640.0f;
        planeX = currentPlane->x;
        planeY = currentPlane->y;
        planeZ = currentPlane->z;
        planeD = currentPlane->d;
        for (j = 0, stop = FALSE; j < 8 && stop == FALSE; j++) {
            if (j & 1) {
                var_fs2 = (xPos * 640.0f) * planeX;
            } else {
                var_fs2 = ((xPos * 640.0f) + 640.0f) * planeX;
            }
            if (j & 2) {
                var_fs2 += (scaledZPos * planeZ);
            } else {
                var_fs2 += ((scaledZPos + 640.0f) * planeZ);
            }
            if (j & 4) {
                var_fs2 += (minY * planeY);
            } else {
                var_fs2 += (maxY * planeY);
            }
            var_fs2 += planeD;
            if (var_fs2 > 0.0f) {
                stop = TRUE;
            }
        }
        if ((j == 8) && (stop == 0)) {
            return 0;
        }
    }
    return 1;
}

void func_8004530C(void) {
    D_800B979E = 0;
    D_800B9794 = 0;
}

void some_cell_func(BitStream* stream) {
    f32 var_fv1;
    s32 var_v0;
    s32 temp_t3;
    s32 var_t0;
    s32 var_v1;
    s32 temp_t2;
    s32 sp3C;
    s32 sp20;
    s32 sp1C;
    s32 temp;
    s32 sp18;
    BlocksModel* sp28;

    if ((u8) D_800B9794 == 0) {
        return;
    }

    sp3C = floor_f((D_800B51E4->srt.transl.x - gWorldX) / 640.0f);
    sp28 = func_80044B18(sp3C, floor_f((D_800B51E4->srt.transl.z - gWorldZ) / 640.0f), 0);

    sp3C = floor_f(D_800B51E4->srt.transl.x / 640.0f) * 0x280;
    temp= (floor_f(D_800B51E4->srt.transl.z / 640.0f) * 0x280);
    temp_t2 = (D_800B51E4->srt.transl.x - sp3C);
    temp_t3 = (D_800B51E4->srt.transl.z - temp);
    if (sp28 == NULL) {
        return;
    }

    var_v0 = sp28->minY;
    if (sp28->minY & 1) {
        var_v0 -= 1;
    }
    if (sp28->maxY < D_800B51E4->srt.transl.y) {
        var_fv1 = sp28->maxY - 1;
    } else {
        var_fv1 = D_800B51E4->srt.transl.y;
    }
    var_v0 = ((s32)var_fv1 - var_v0);
    if (((sp28->maxY - sp28->minY) / 80) < 8) {
        var_t0 = (sp28->maxY - sp28->minY) / 8;
    } else {
        var_t0 = 0x50;
    }
    sp20 = var_v0 / var_t0;;
    sp1C = temp_t2 / 80;
    sp18 = temp_t3 / 80;
    diPrintf(&D_8009A5C0, sp1C, sp20, sp18);
    var_v1 = D_800B979E >> 3;
    if (D_800B979E & 7) {
        var_v1 += 1;
    }
    temp = ((sp20 << 6) + (sp18 * 8) + sp1C);
    bitstream_init(stream, (var_v1 * temp) + D_800B9798, D_800B979E, D_800B979E);
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045600.s")
#else
extern char D_8009A614;
extern s32 *D_800B9700;
extern s16 *D_800B97A0;

s32 func_80045600(s32 arg0, BitStream *stream, s16 arg2, s16 arg3, s16 arg4) {
    s8 bitPosIndex;
    s8 *new_var;

    new_var = D_800B9700;
    new_var += arg4 * 4;
    bitPosIndex = new_var[arg2 + arg3 * 16];
    
    if (bitPosIndex >= 0){
        bitstream_set_pos(stream, D_800B97A0[bitPosIndex] + arg0);
        return bitstream_read(stream, 1);
    }
    diPrintf(&D_8009A614);
    return 0;
}
#endif

u8 func_800456AC(Object* obj) {
    f32 temp_ft4;
    Object* playerObj;
    f32 var_fv0;
    f32 temp_fv1;
    f32 temp;
    s32 var_v0;

    if (obj->unk_0x36 == 0) {
        obj->unk_0x37 = 0;
        return 0;
    }
    if ((obj->createInfo != NULL) && (obj->createInfo->loadParamB & 1)) {
        obj->unk_0x37 = ((obj->unk_0x36 * 0xFF) + 0xFF) >> 8;
        // Ugly goto but too lazy to fix
        goto block_17;
    }
    temp_ft4 = obj->unk0x40;
    if (temp_ft4 < 40.0f) {
        obj->unk_0x37 = 0;
        return 0;
    }
    if ((obj->createInfo != NULL) && (obj->createInfo->loadParamB & 2) && (playerObj = get_player(), (playerObj != NULL))) {
        var_fv0 = vec3_distance(&obj->positionMirror, &playerObj->positionMirror);
    } else {
        var_fv0 = func_80001884(obj->positionMirror.x, obj->positionMirror.y, obj->positionMirror.z);
    }
    if (temp_ft4 < var_fv0) {
        obj->unk_0x37 = 0;
        return 0;
    }
    var_v0 = 0xFF;
    temp_fv1 = temp_ft4 - 40.0f;
    if (temp_fv1 < var_fv0) {
        temp_ft4 -= temp_fv1;
        var_fv0 = var_fv0 - temp_fv1;
        temp = 1.0f - ((var_fv0) / temp_ft4);
        var_v0 = 255.0f * temp;
    }
    obj->unk_0x37 = (u8) ((s32) ((obj->unk_0x36 + 1) * var_v0) >> 8);
block_17:
    if (obj->unk_0x37 == 0) {
        return 0;
    }
    if (obj->id == 0xD4) {
        return 1;
    }
    return is_sphere_in_frustum(&obj->positionMirror, obj->unk_0xa8);
}

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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/map_load_streammap.s")
#else
void func_8004BD40(MAPSHeader*, s32, s32, s32); //Unsure of argument types, especially last one
extern s32 *gFile_MAPS_TAB;
extern MAPSHeader *gMapActiveStreamMap;

typedef struct {
/*00*/ u32 header;
/*04*/ u32 blockIDs;
/*08*/ u32 gridA1;
/*0c*/ u32 gridA2;
/*10*/ u32 objCreateInfo;
/*14*/ u32 gridB1;
/*18*/ u32 gridB2;
/*1c*/ u32 end;
} MapTab;

/* 
    arg1 = 0 when I was testing!
*/
MAPSHeader* map_load_streammap(s32 mapID, s32 arg1) {
    s32 new_var;
    s32 map_size;
    s32 temp_a0;
    s32 map_start;
    s32 objectsCount_oneEighth;
    s32 temp_t4;
    s32 temp_t6;
    s32 gridB_size;
    int objectsMallocSize;
    s32 var_v0;
    MapTab *mapTab;
    s8 *temp_t7_2;
    u8 *map;
    void *temp_t3;


    
    mapTab = (MapTab *)(&gFile_MAPS_TAB + (mapID * 7));
    
    map_start = mapTab->header;
    map_size = mapTab->end - map_start;
    queue_load_file_region_to_ptr((void *) gMapReadBuffer, MAPS_BIN, map_start, sizeof(MAPSHeader));
    gMapActiveStreamMap = (MAPSHeader *) gMapReadBuffer;
    
    gridB_size = gMapActiveStreamMap->gridB_sixteenthSize * 16;
    objectsCount_oneEighth = gMapActiveStreamMap->objectInstanceCount >> 3;
    objectsMallocSize = objectsCount_oneEighth + 1;
    map = malloc(((gridB_size << 1) + map_size) + objectsMallocSize, 5, 0);
    
    gMapActiveStreamMap = (MAPSHeader *) map;
    queue_load_file_region_to_ptr((void *) map, MAPS_BIN, map_start, map_size);
    
    temp_a0 = objectsCount_oneEighth + 1;
    temp_t4 = objectsMallocSize & 3;

    //Setting up pointers to the 7 MAPS files (excluding the header) (and EOF)
    gMapActiveStreamMap->blockIDs_ptr = (u32 *) gMapActiveStreamMap + mapTab->blockIDs;
    gMapActiveStreamMap->grid_A1_ptr = (s8 *) gMapActiveStreamMap + mapTab->gridA1;
    gMapActiveStreamMap->grid_A2_ptr = (s8 *) gMapActiveStreamMap + mapTab->gridA2;
    gMapActiveStreamMap->objectInstanceFile_ptr = (s32 *) gMapActiveStreamMap + mapTab->objCreateInfo;
    new_var = temp_t4;
    gMapActiveStreamMap->grid_B1_ptr = (s8 *) gMapActiveStreamMap + mapTab->gridB1;
    gMapActiveStreamMap->grid_B2_ptr = (s8*)(gMapActiveStreamMap->grid_B1_ptr + gridB_size);    
    gMapActiveStreamMap->end_ptr = (s8*)(gMapActiveStreamMap->grid_B2_ptr + gridB_size);

    //Ack, this section needs reworking
    for (var_v0 = 0; var_v0 != temp_a0; var_v0++){
        gMapActiveStreamMap->end_ptr[var_v0] = 0;
    }
    
    gMapActiveStreamMap->originWorldX = 0.0f;
    gMapActiveStreamMap->originWorldZ = 0.0f;
    gMapActiveStreamMap->unk18 = 0;
    gMapActiveStreamMap->unk19 = 0;
    //It ignores the asset's stored length and calculates it instead
    gMapActiveStreamMap->objectInstancesFileLength = (u32)gMapActiveStreamMap->grid_B2_ptr - (u32)gMapActiveStreamMap->objectInstanceFile_ptr;
    gLoadedMapsDataTable[mapID] = gMapActiveStreamMap;
    
    if (arg1 == 0){
        func_80045FC4(gMapActiveStreamMap, (mapID * 0x8C) + (&D_800B5508), mapID, 0);
        gDLL_29_gplay->exports->func_15B8(mapID);
    }
    else{
        func_8004BD40(gMapActiveStreamMap, mapID, mapID, 0);
    }
    return gMapActiveStreamMap;
}

#endif

#ifndef NON_MATCHING
s32 map_load_streammap_add_to_table(s32);
#pragma GLOBAL_ASM("asm/nonmatchings/map/map_load_streammap_add_to_table.s")
#else
typedef struct Unk800B9768_Unk4 {
    s16 unk0;
    s16 pad2;
    s16 unk4;
    u8 pad6[4];
} Unk800B9768_Unk4;
typedef struct Unk800B9768{
    s32 pad0;
    Unk800B9768_Unk4 *unk4;
    s32 pad8;
    s8 *unkC;
}Unk800B9768;
void func_8004BD40(MapHeader*, s32);                   /* extern */
MapHeader* map_load_streammap(s32, s32);         /* extern */
extern Unk800B9768 D_800B9768;
void map_convert_objpositions_to_ws(MapHeader *map, f32 X, f32 Z);
s32 map_load_streammap_add_to_table(s32 arg0) {
    s32 sp2C;
    Unk800B9768_Unk4* temp_a3;

    sp2C = 0;
    for (sp2C = 0; sp2C < gMapNumStreamMaps && gMapStreamMapTable[sp2C].header != NULL; sp2C++) {}
    if (sp2C == gMapNumStreamMaps) {
        gMapNumStreamMaps += 1;
    }
    gMapActiveStreamMap = map_load_streammap(arg0, 0);
    gMapStreamMapTable[sp2C].header = gMapActiveStreamMap;
    gMapStreamMapTable[sp2C].mapID = arg0;
    temp_a3 = &D_800B9768.unk4[arg0];
    gMapStreamMapTable[sp2C].header->unk19 = D_800B9768.unkC[arg0];
    gMapActiveStreamMap->originWorldX = (temp_a3->unk0 + gMapActiveStreamMap->originOffsetX) * 640.0f;
    gMapActiveStreamMap->originWorldZ = (temp_a3->unk4 + gMapActiveStreamMap->originOffsetZ) * 640.0f;
    map_convert_objpositions_to_ws(gMapActiveStreamMap, gMapActiveStreamMap->originWorldX, gMapActiveStreamMap->originWorldZ);
    func_8004BD40(gMapActiveStreamMap, arg0);
    D_80092A94 = arg0;
    return sp2C;
}
#endif

/** Returns one of the loaded maps' mapID (as defined in MAPINFO.bin) */
s32 func_80045D58(void) {
    return D_80092A94;
}

s32 map_find_streammap_index(s32 mapID_to_find) {
    s32 index;
    
    for (index = 0; index < gMapNumStreamMaps; index++){
        if ((gMapStreamMapTable[index].header != NULL) && (mapID_to_find == gMapStreamMapTable[index].mapID)) {
            return index;
        }
    }

    return -1;
}

s32 func_80045DC0(s32, s32, s32);
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045DC0.s")

/** free_mapID? */
void func_80045F48(s32 mapID) {
    if (gLoadedMapsDataTable[mapID]){
        func_80045FC4(gLoadedMapsDataTable[mapID], (mapID * 0x8C) + (&D_800B5508), mapID, 1);
        free(gLoadedMapsDataTable[mapID]);
        gLoadedMapsDataTable[mapID] = 0;
    }
}

void func_80045FC4(MapHeader* arg0, s32* arg1, s32 arg2, s32 arg3) {
    s32 pad;
    s32 sp48;
    s32 var_a0;
    s8 sp42[4];
    s32 var_s3;
    s32 var_s4;
    UnkObjectInstanceFileStruct* var_s0;
    s32 i;

    sp42[3] = FALSE;
    var_s4 = 0;
    var_s0 = (UnkObjectInstanceFileStruct *) arg0->objectInstanceFile_ptr;
    sp48 = arg0->objectInstancesFileLength;
    if (sp48 == 0) {
        return;
    }
    var_s3 = 0;

    if (arg3 == 0) {
        arg1[33] = -1;
        for (i = 0; i < 32; i++) { arg1[i] = -1; }
    }
    while (var_s3 < sp48) {
        if (arg3 != 0) {
            if (var_s0->unk0 == 0x6E) {
                gDLL_CURVES->exports->curves_func_10c.withOneArg(var_s0);
            }
            if (var_s0->unk0 == 5) {
                gDLL_Race->exports->func[2].withOneArg(var_s0);
            }
        } else {
            if ((0x6E == var_s0->unk0) || (5 == var_s0->unk0)) {
                if (0x6E == var_s0->unk0) {
                    gDLL_CURVES->exports->curves_func_34.withOneArg(var_s0);
                } else {
                    gDLL_Race->exports->func[1].withOneArg(var_s0);
                }
                if ((u8)sp42[3] == FALSE) {
                    arg1[33] = ((u32)var_s0 - (u32)arg0->objectInstanceFile_ptr);
                    sp42[3] = TRUE;
                }
            } else if (var_s0->unk4 & 0x10) {
                if (!((1 << var_s0->unk6) & var_s4)) {
                    arg1[var_s0->unk6] = (u32)var_s0 - (u32)arg0->objectInstanceFile_ptr;
                    var_s4 |= 1 << var_s0->unk6;
                }
            }
        }
        var_s3 += var_s0->unk2 * 4;
        var_s0 = (UnkObjectInstanceFileStruct *) &((s8 *)var_s0)[var_s0->unk2 * 4];
    }
    if (arg3 == 0) {
        var_a0 = sp48;
        if ((arg1[33] != -1) && (arg1[33] < sp48)) {
            var_a0 = arg1[33];
        }
        for (i = 0; i < 32; i++) {
            if ((arg1[i] != -1) && (arg1[i] < var_a0)) {
                var_a0 = arg1[i];
            }
        }
        arg1[34] = var_a0;
        if (arg1[33] != -1) {
            arg1[32] = arg1[33];
            return;
        }
        arg1[32] = sp48;
    }
}

void func_800462B0(){
}

/** Iterates over all the ObjCreateInfo in a map's object instance file, transforming the objects' coordinates from local coordinates to world-space coordinates */
void map_convert_objpositions_to_ws(MapHeader *map, f32 X, f32 Z) {
    u8 *ptr;
    s32 offset;
    ObjCreateInfo *obj;

    if (!map){
        return;
    }
    
    ptr = (u8*)map->objectInstanceFile_ptr;
    offset = 0; // current offset in MAPS object instance file data
    
    while (offset < map->objectInstancesFileLength){
        obj = (ObjCreateInfo*)ptr;
        obj->x += X;
        obj->z += Z;

        offset += obj->quarterSize << 2;
        ptr += obj->quarterSize << 2;
    }
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046320.s")
#else
extern s32 D_800B4A50;
extern s32 D_800B5468;

void func_80046320(s32 arg0, Object *obj) {
    s32 sp24;
    s8 var_t0;
    s32 var_a2;
    MapHeader *sp18;
    s32* var_v1;

    sp24 = D_800B4A50;
    sp18 = map_load_streammap(arg0, 1);
    gLoadedMapsDataTable[arg0] = 0;

    var_t0 = FALSE;
    var_v1 = &D_800B5468;
    var_a2 = 0x50;
    while (var_a2 < 0x78){
        if (*var_v1 == 0) {
            var_t0 = TRUE;
            *var_v1 = sp18;
            break;
        } 
        
        var_a2++;
        var_v1++;
    }

    func_80045FC4(sp18, (var_a2 * 0x8C) + (u32)&D_800B5508, var_a2, 0);
    gDLL_29_gplay->exports->func_15B8(var_a2);
    obj->unk_0x34 = var_a2;
    gDLL_29_gplay->exports->func_1378(arg0, var_a2);
    D_800B4A50 = sp24;
}

#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046428.s")

void func_80046688(s32 arg0, s32 arg1) {
}

GlobalMapCell* func_80046698(s32 gridX, s32 gridZ) {
    GlobalMapCell *layer;    
    s32 cellIndex;
    
    layer = gDecodedGlobalMap[0];
    cellIndex = (gridZ * 16) + gridX;
    
    return &layer[cellIndex];
}

MapHeader* func_800466C0() {
    MapHeader* map;
    GlobalMapCell *layer;
    s32 mapID;

    layer = gDecodedGlobalMap[0];
    mapID = layer[119].mapIDs[0]; //There should be 16*16 cells, so why this one specifically... centre cell?
    if (mapID < 0) {
        mapID = D_80092BBC;
    }
    if (mapID < 0) {
        return NULL;
    }
    
    map = gLoadedMapsDataTable[mapID];
    if (map != NULL) {
        D_80092BBC = (s32) mapID;
        gMapActiveStreamMap = map;
    }
    return map;
}

extern s16 gNumTRKBLKEntries;
s16 func_80046718() {
    return gNumTRKBLKEntries;
}

/** map_get_mapID_blocks_count? (Calculated by quickly comparing base blockID for this map and next map) */
s32 func_80046728(s32 mapID) {
    if (mapID < 0 || mapID >= gNumTRKBLKEntries)
        return 0;
    return gFile_TRKBLK[mapID + 1] - gFile_TRKBLK[mapID];
}


void init_global_map(void)
{
    #define SOME_LENGTH     64
    s32 i;
    s32 size;
    StructBuf *buf = NULL;
    Struct_D_800B9768_unk4 *thisunk4;

    size = get_file_size(GLOBALMAP_BIN);

    queue_alloc_load_file((void**)&buf, GLOBALMAP_BIN);

    size /= sizeof(StructBuf);

    D_800B9768.unk0  = -1;
    D_800B9768.unk4  = malloc(sizeof(Struct_D_800B9768_unk4)  * SOME_LENGTH, ALLOC_TAG_TRACK_COL, NULL);
    D_800B9768.unk8  = malloc(sizeof(s16) * 2  * SOME_LENGTH, ALLOC_TAG_TRACK_COL, NULL);
    D_800B9768.unkC  = malloc(sizeof(Struct_D_800B9768_unkC)  * SOME_LENGTH, ALLOC_TAG_TRACK_COL, NULL);
    D_800B9768.unk10 = malloc(sizeof(Struct_D_800B9768_unk10) * SOME_LENGTH, ALLOC_TAG_TRACK_COL, NULL);

    bzero(D_800B9768.unk10, sizeof(Struct_D_800B9768_unk10) * SOME_LENGTH);

    // loop 64 times and set all fields of unk4, unk8 and unkC
    for (i = 0; i < SOME_LENGTH; i++) {
        thisunk4 = D_800B9768.unk4 + i;
        D_800B9768.unkC[i].unk0 = 0x80;
        thisunk4->unk0 = 0x8000;
        thisunk4->unk2 = 0x8000;
        thisunk4->unk4 = 0x8000;
        thisunk4->unk6 = 0x8000;
        thisunk4->unk8 = 0x80;
        thisunk4->unk9 = 0x80;

        D_800B9768.unk8[(i << 1) + 0] = -1;
        D_800B9768.unk8[(i << 1) + 1] = -1;
    }
    for (i = 0; i < size && (buf[i].unk6 >= 0); i++) {
        D_800B9768.unkC[buf[i].unk6].unk0 = buf[i].unk4;

        map_read_layout(
            &D_800B9768.unk4[buf[i].unk6],
            &D_800B9768.unk10[buf[i].unk6],
            buf[i].unk0,
            buf[i].unk2,
            buf[i].unk6
        );

        D_800B9768.unk8[(buf[i].unk6 << 1) + 0] = buf[i].unk8;
        D_800B9768.unk8[(buf[i].unk6 << 1) + 1] = buf[i].unkA;
    }

    func_80048034();

    free(buf);
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/map/map_read_layout.s")
#else

// regalloc
extern MapsTabStruct  *gFile_MAPS_TAB;
void map_read_layout(MapLayoutArg0 *arg0, u8 *arg1, s16 arg2, s16 arg3, s32 maptabindex)
{
    s32 temp_v1;
    MapsTabStruct* maptabstruct;
    MapsTabStruct* maptabstructagain;
    MapsBinStruct* mapbinstruct;
    s32 var_v0;

    maptabstruct = &gFile_MAPS_TAB[maptabindex];
    
    mapbinstruct = (MapsBinStruct*)gMapReadBuffer;
    
    queue_load_file_region_to_ptr((void**)mapbinstruct, MAPS_BIN, maptabstruct->unk0, maptabstruct->unk8 - maptabstruct->unk0);
    
    maptabstructagain = &gFile_MAPS_TAB[maptabindex];
    mapbinstruct->unkC = (s32 *) (((s8 *)mapbinstruct + maptabstructagain->unk4) - maptabstructagain->unk0);

    arg0->unk0 = arg2 - mapbinstruct->unk4;
    arg0->unk4 = arg3 - mapbinstruct->unk6;

    arg0->unk2 = (arg0->unk0 + mapbinstruct->unk0) - 1;
    arg0->unk6 = (arg0->unk4 + mapbinstruct->unk2) - 1;
    arg0->unk8 = mapbinstruct->unk4;
    arg0->unk9 = mapbinstruct->unk6;

    for (arg3 = 0; arg3 < mapbinstruct->unk2; arg3++) {
        for (arg2 = 0; arg2 < mapbinstruct->unk0; arg2++) {
            temp_v1 = arg2 + (arg3 * mapbinstruct->unk0);
            var_v0 = mapbinstruct->unkC[temp_v1];
            if (((var_v0 >> 0x17) & 0x3F) != 0x3F) {
                arg1[(temp_v1 >> 3)] |= 1 << (temp_v1 & 7);
            }
        }
    }
}
#endif



void func_80046B58(f32 x, f32 y, f32 z) {
    u32 temp_t8;
    
    temp_t8 = UINT_80092a98;
    if (!(temp_t8 & 2) || temp_t8 & 0x800){
        D_800B97AC = x;
        D_800B97B0 = y;
        D_800B97B4 = z;
        UINT_80092a98 = temp_t8 | 2;
        if (UINT_80092a98 & 0x800){
            map_update_streaming();
        }
    }
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/map/map_update_streaming.s")
#else
extern s32 D_80092A60;
extern s32 D_80092A64;
extern s32 D_800B4A50;
extern s32 D_800B4A54;
extern s32 *D_800B9700;
extern s8 *D_800B9714;
typedef struct UnkStruct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} UnkStruct;

// Size TBD
extern UnkObjectInstanceFileStruct gMapStreamMapIDTable[16];
void map_update_streaming(void) {
    GlobalMapCell **var_a1;
    f32 sp308;
    GlobalMapCell *var_v1;
    StreamMap* var_s0_2;
    f32 tempX;
    f32 tempZ;
    s32 sp2F4;
    s32 sp2F0;
    s32 temp_s1;
    s32 var_s7;
    s32 var_s3;
    s32 var_s5;
    s8* temp_a3;
    s8* var_s1;
    s32 sp2C8[4];
    s32 sp2B8[4];
    s32 sp2A8[4];
    s32 sp298[4];
    s32 sp294;
    s32 var_fp;
    s32 var_s0;
    s32 var_s2;
    s32 sp284;
    UnkStruct sp84[64]; // Unknown size, although 64 sounds reasonable
    f32 f0;
    f32 f2;
    f32 f14;
    s32 pad;
    s8 **sp70;
    s32 **sp6C;

    if (!(UINT_80092a98 & 2)) {
        return;
    }

    UINT_80092a98 &= ~2;
    f0 = D_800B97AC;
    f2 = D_800B97B4;
    f14 = f0 - gWorldX;
    sp308 = f2 - gWorldZ;
    sp2F4 = floor_f(f14 / 640.0f);
    sp2F0 = floor_f(sp308 / 640.0f);
    sp294 = UINT_80092a98 & 0x800;
    UINT_80092a98 &= ~0x800;
    if ((sp2F4 != 7) || (sp2F0 != 7) || (sp294 != 0) || (UINT_80092a98 & 0x4000)) {
        func_8004D974(1);
        func_80012B54(1, 0);
        var_fp = 0;
        var_a1 = (GlobalMapCell **) &gDecodedGlobalMap;
        sp70 = gBlockIndices;\
        sp6C = &D_800B9700;\
        for (var_s7 = 0; var_s7 < 5; ) {
            var_s1 = *sp70;
            D_800B9714 = (s8 *) *sp6C;
            var_v1 = &*var_a1[var_s7];
            var_s3 = 0;
            for (var_s2 = 0; var_s2 < 16; var_s2++) {
                for (var_s0 = 0; var_s0 != 16; var_s0++) {
                    if (var_s1[0] >= 0) {
                        sp84[var_fp].unk6 = var_s7;
                        sp84[var_fp].unk0 = gMapCurrentStreamCoordsX + var_s0;
                        sp84[var_fp].unk2 = gMapCurrentStreamCoordsZ + var_s2;
                        sp84[var_fp].unk4 = var_s1[0];
                        var_fp++;
                    }
                    var_s1[0] = -2;
                    D_800B9714[var_s3] = -1;
                    var_v1[var_s3].blockID = -3;
                    var_v1[var_s3].mapID = -1;
                    var_v1[var_s3].unk2 = -1;
                    var_v1[var_s3].unk4 = -1;
                    var_s3++;
                    var_s1++;
                }
            }
            sp70++;\
            sp6C++;
            var_s7++;
        }
        tempX = gWorldX;
        tempZ = gWorldZ;
        gMapCurrentStreamCoordsX = (gMapCurrentStreamCoordsX + sp2F4) - 7;
        gMapCurrentStreamCoordsZ = (gMapCurrentStreamCoordsZ + sp2F0) - 7;
        gWorldX = gMapCurrentStreamCoordsX * 640.0f;
        gWorldZ = gMapCurrentStreamCoordsZ * 640.0f;
        D_80092A60 = gWorldX;
        D_80092A64 = gWorldZ;
        func_800307C4(tempX - gWorldX, tempZ - gWorldZ);
        for (var_s0 = 0; var_s0 < gMapNumStreamMaps; var_s0++) {
            gMapStreamMapTable[var_s0].unk06 = 0;
        }
        D_800B4A50 = func_80045DC0(gMapCurrentStreamCoordsX + 7, gMapCurrentStreamCoordsZ + 7, 0);
        D_800B4A54 = -1;
        if (D_800B4A50 != -1) {
            sp284 = map_find_streammap_index(D_800B4A50);
            if (sp284 == -1) {
                sp284 = map_load_streammap_add_to_table(D_800B4A50);
            }
            gMapStreamMapIDTable[sp284].unk2 = 1;
            D_800B4A54 = sp284;
            sp70 = gBlockIndices;\
            sp6C = &D_800B9700;
            for (var_s7 = 0; var_s7 < 5; ) {
                func_80047404(gMapCurrentStreamCoordsX + 7, gMapCurrentStreamCoordsZ + 7, sp2C8, sp2B8, sp2A8, sp298, var_s7, 0, sp284);
                temp_a3 = *sp70;
                var_s2 = sp2C8[2];
                D_800B9714 = (s8 *) *sp6C;
                while (sp2C8[3] >= var_s2) {
                    var_s0 = sp2C8[0];
                    while (sp2C8[1] >= var_s0) {
                        (&temp_a3[var_s0 + (((var_s2 + 7) << 4))])[7] = -3;
                        var_s0++;
                    }
                    var_s2++;
                }
                var_s2 = sp2B8[2];
                var_s1 = temp_a3;
                while (sp2B8[3] >= var_s2) {
                    var_s0 = sp2B8[0];
                    while (sp2B8[1] >= var_s0) {
                        (&temp_a3[var_s0 + (((var_s2 + 7) << 4))])[7] = -3;
                        var_s0++;
                    }
                    var_s2++;
                }
                var_s2 = sp2A8[2];
                while (sp2A8[3] >= var_s2) {
                    var_s0 = sp2A8[0];
                    while (sp2A8[1] >= var_s0) {
                        (&temp_a3[var_s0 + (((var_s2 + 7) << 4))])[7] = -3;
                        var_s0++;
                    }
                    var_s2++;
                }
                var_s2 = sp298[2];
                while (sp298[3] >= var_s2) {
                    var_s0 = sp298[0];
                    while (sp298[1] >= var_s0) {
                        (&temp_a3[var_s0 + (((var_s2 + 7) << 4))])[7] = -3;
                        var_s0++;
                    }
                    var_s2++;
                }
                var_s3 = 0;
                var_s5 = 0;
                for (var_s2 = 0; var_s2 < 16; var_s2++) {
                    for (var_s0 = 0; var_s0 != 16; ) {
                        if (var_s1[0] == -3) {
                            if (func_800485FC(var_s0, var_s2, gMapCurrentStreamCoordsX + var_s0, gMapCurrentStreamCoordsZ + var_s2, var_s7) == 0) {
                                var_s1[0] = -2;
                            } else {
                                D_800B9714[var_s3] = var_s5;
                                var_s5++;
                            }
                        }
                        var_s3++;
                        var_s1++;
                        var_s0++;
                    }
                }
                sp70++;\
                var_s7++;\
                sp6C++;
            }
        }
        var_s2 = TRUE;
        for (var_s3 = gMapNumStreamMaps - 1; var_s3 >= 0; var_s3--) {
            var_s0_2 = &gMapStreamMapTable[var_s3];
            if ((s8) var_s0_2->unk06 == 0) {
                if (var_s0_2->header != NULL) {
                    temp_s1 = var_s0_2->mapID;
                    func_80045FC4(var_s0_2->header, &D_800B5508 + (temp_s1 * 0x8C), temp_s1, 1);
                    free(var_s0_2->header);
                    gLoadedMapsDataTable[temp_s1] = NULL;
                }
                var_s0_2->header = NULL;
                var_s0_2->mapID = -1;
            }
            if (var_s2 != FALSE) {
                if (var_s0_2->header == NULL) {
                    gMapNumStreamMaps -= 1;
                } else {
                    var_s2 = FALSE;
                }
            }
        }
        for (var_s3 = 0; var_s3 < var_fp; var_s3++) {
            func_800496E4(sp84[var_s3].unk4);
        }
        func_8004530C();
    }
    map_update_objects_streaming(sp294);
    UINT_80092a98 &= ~0x4000;
}
#endif

/** Increment something */
void func_80047374(void) {
    D_80092A8C += 1;
    if (D_80092A8C >= 3) {
        D_80092A8C = 2;
    }
    UINT_80092a98 |= 0x4000;
}

/** Decrement something */
void func_800473BC(void) {
    D_80092A8C -= 1;
    if (D_80092A8C < -2) {
        D_80092A8C = -2;
    }
    UINT_80092a98 |= 0x4000;
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80047404.s")

void func_80047710(s32 arg0, s32 arg1, s32 arg2) {
}

void func_80047724(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
}

extern s32 D_80092A60;
extern s32 D_80092A64;
extern f32 D_8009A9C0;
extern s32 D_800B4A50;
extern s32 D_800B4A54;
extern s16 D_800B97C4;

void func_8004773C(void) {
    s32 i;
    s32 j;
    s32 sp13C;
    GplayStruct5* sp138;
    Camera* camera;
    Object* sp130;
    Object sp4C;
    GlobalMapCell *currentCell;
    s8 *currentT1;
    GplayStruct6* sp40;
    GplayStruct12* sp3C;
    s16* sp38;

    if (D_800B4A5E == -1) {
        D_800B4A5E = -2;
        D_80092A78 = 8;
    }
    gDLL_ANIM->exports->func0.asVoid();
    func_80001A3C();
    func_80001A3C();
    func_80053300();

    for (i = 0; i < 5; i++) {
        currentT1 = gBlockIndices[i];
        currentCell = gDecodedGlobalMap[i];
        for (j = 0; j < 256; j++) {
            currentT1[j] = -1;
            currentCell[j].trkBlkIndex = -1;
        }
    }

    for (i = 0; i < 40; i++) {
        gLoadedBlockIds[i] = -1;
        gLoadedBlocks[i] = 0;
    }

    gLoadedBlockCount = 0;
    gMapNumStreamMaps = 0;
    D_800B97C4 = 0;
    sp13C = gDLL_29_gplay->exports->func_E90(&gLoadedBlockIds, &gLoadedBlocks);
    sp138 = gDLL_29_gplay->exports->func_F04();
    gMapCurrentStreamCoordsX = floor_f(sp138->vec.x / 640.0f);
    gMapCurrentStreamCoordsZ = floor_f(sp138->vec.z / 640.0f);
    Vec3_Int_array->f.x = sp138->vec.x;
    Vec3_Int_array->f.y = sp138->vec.y;
    Vec3_Int_array->f.z = sp138->vec.z;
    Vec3_Int_array->i = 1;
    D_80092A60 = gMapCurrentStreamCoordsX * 0x280;
    D_80092A64 = gMapCurrentStreamCoordsZ * 0x280;
    gWorldX = D_80092A60;
    // @fake
    if (0) { }
    gWorldZ = D_80092A64;
    D_800B4A50 = -1;
    D_800B4A54 = -1;
    D_80092A8C = sp138->mapLayer;
    gDLL_waterfx->exports->func_24C();
    gDLL_newlfx->exports->func[1].asVoid();
    gDLL_57->exports->func[0].asVoid();
    gDLL_58->exports->func[0].asVoid();
    gDLL_projgfx->exports->func[0].asVoid();
    gDLL_modgfx->exports->func[0].asVoid();
    gDLL_expgfx->exports->func0();
    gDLL_17->exports->func[0].asVoid();
    gDLL_minic->exports->func4();
    gDLL_minic->exports->func1();
    gDLL_8->exports->func[1].asVoid();
    gDLL_7_newday->exports->func1.asVoid();
    gDLL_newclouds->exports->func[1].asVoid();
    gDLL_newstars->exports->func[0].asVoid();
    func_8005C780();
    UINT_80092a98 &= 0x2010;
    UINT_80092a98 |= 0x81E0;
    UINT_80092a98 |= 0x804;
    func_80046B58(sp138->vec.x, sp138->vec.y, sp138->vec.z);
    UINT_80092a98 &= ~4;
    func_800591EC();
    func_80023628();
    D_800B4A58 = 0;

    camera = get_main_camera();
    camera->srt.transl.x = sp138->vec.x;
    camera->srt.transl.y = sp138->vec.y;
    camera->srt.transl.z = sp138->vec.z;

    sp130 = get_player();
    if ((D_800B4A5E == -2) && (sp130 != NULL) && ((sp13C == 0) || (sp13C == 1))) {
        sp40 = gDLL_29_gplay->exports->func_F60();
        sp3C = gDLL_29_gplay->exports->func_FA8();
        sp38 = (s16 *) gDLL_29_gplay->exports->func_FE8();
        if (D_800B4A5E == -2) {
            if (sp40->unk0x0 != -1) {
                func_80000608(sp130, sp130, sp40->unk0x0, 0, 0, 0);
            }
            if (sp40->unk0x2 != -1) {
                func_80000608(sp130, sp130, sp40->unk0x2, 0, 0, 0);
            }
            if (sp40->unk0x4 != -1) {
                func_80000608(sp130, sp130, sp40->unk0x4, 0, 0, 0);
            }
            if (sp40->unk0x6 != -1) {
                func_80000608(sp130, sp130, sp40->unk0x6, 0, 0, 0);
            }
            if (sp40->unk0x8 != -1) {
                func_80000608(sp130, sp130, sp40->unk0x8, 0, 0, 0);
            }
            if (sp40->unk0xa != -1) {
                func_80000608(sp130, sp130, sp40->unk0xa, 0, 0, 0);
            }
            if (sp40->unk0xc != -1) {
                func_80000608(sp130, sp130, sp40->unk0xc, 0, 0, 0);
            }
            if (sp40->unk0xe != -1) {
                func_80000608(sp130, sp130, sp40->unk0xe, 0, 0, 0);
            }
            func_8001EBD0(sp40->unk0x10 & 1);
            if (sp3C->unk0x4 != -1) {
                func_800009C8(sp130, sp130, sp3C->unk0x4, 0);
            }
            if (sp3C->unk0x6 != -1) {
                func_800009C8(sp130, sp130, sp3C->unk0x6, 0);
            }
            if (sp3C->unk0x8 != -1) {
                func_800009C8(sp130, sp130, sp3C->unk0x8, 0);
            }
            gDLL_minic->exports->func6(sp3C->unk0x3c & 1);
            for (i = 0; i < 4; i++) {
                if (sp38[i] != -1) {
                    gDLL_AMSEQ2->exports->func0(NULL, sp38[i], 0, 0, 0);
                }
            }
            if (sp40->unk0x12 != -1) {
                gDLL_Camera->exports->func8(0, sp40->unk0x12);
            }
            sp4C.parent = NULL;
            sp4C.srt.transl.x = 0.0f;
            sp4C.srt.transl.y = 0.0f;
            sp4C.srt.transl.z = 0.0f;
            sp4C.positionMirror.x = 0.0f;
            sp4C.positionMirror.y = 0.0f;
            sp4C.positionMirror.z = 0.0f;
            if (sp3C->unk0xa != -1) {
                sp4C.srt.transl.x = sp3C->unk10;
                sp4C.srt.transl.y = sp3C->unk14;
                sp4C.srt.transl.z = sp3C->unk18;
                func_80000860(&sp4C, sp130, sp3C->unk0xa, 0);
            }
            if (sp3C->unk0xc != -1) {
                sp4C.srt.transl.x = sp3C->unk1C;
                sp4C.srt.transl.y = sp3C->unk20;
                sp4C.srt.transl.z = sp3C->unk24;
                func_80000860(&sp4C, sp130, sp3C->unk0xc, 0);
            }
            if (sp3C->unk0xe != -1) {
                sp4C.srt.transl.x = sp3C->unk28;
                sp4C.srt.transl.y = sp3C->unk2C;
                sp4C.srt.transl.z = sp3C->unk30;
                func_80000860(&sp4C, sp130, sp3C->unk0xe, 0);
            }
            gDLL_7_newday->exports->func9(sp3C->unk0x0);
        }
    } else {
        gDLL_7_newday->exports->func9(D_8009A9C0);
        gDLL_minic->exports->func6(1);
    }
}

s32 func_80048024() {
    return D_800B96A8;
}

void func_80048034(void) {
    D_800B96A8 = 0;
    D_800B4A70 = 0;
    D_800B4A72 = 0;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048054.s")
#else

typedef struct Unk800B9768_Unk4 {
    s16 unk0;
    s16 pad2;
    s16 unk4;
    u8 pad6[4];
} Unk800B9768_Unk4;
typedef struct Unk800B9768{
    s32 pad0;
    Unk800B9768_Unk4 *unk4;
    s32 pad8;
    s8 *unkC;
    u8 *unk10;
}Unk800B9768;
extern Unk800B9768 D_800B9768;

typedef struct UnkV0 {

    s32 pad0;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    u8 pad[0x20 - 0xC];
    s32 unk20;
    f32 unk24;
    f32 unk28;
} UnkV0;

typedef struct UnkS1 {
    s16 unk0;
    u8 unk2;
    s32 pad4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s32 pad14;
    u8 unk18;
    u8 unk19;
    s16 pad1A;
    u32 unk1C;
} UnkS1;
void func_80048054(s32 arg0, s32 arg1, f32* arg2, f32* arg3, f32* arg4, s8* arg5) {
    MapsTabStruct* temp_v0;
    s32 temp_s1;
    s32 temp_s4;
    s32 temp_t8;
    s32 var_s0;
    s32 var_s0_2;
    s8 var_v0;
    u16 temp_v1;
    s32 temp_t1;
    u8* var_s0_3;
    UnkS1* var_s1;
    UnkV0 *temp_v0_2;

    var_v0 = D_800B9768.unkC[arg0];
    var_s0 = 0;
    if (var_v0 == -0x80) {
        map_read_layout((MapLayoutArg0* ) &D_800B9768.unk4[arg0], &D_800B9768.unk10[arg0], 0, 0, arg0);
        D_800B9768.unkC[arg0] = 0;
        var_v0 = D_800B9768.unkC[arg0];
    }
    *arg5 = var_v0;
    if (arg0 == 1) {
        *arg2 = 0.0f;
        *arg3 = 0.0f;
        *arg4 = 0.0f;
    } else {
        temp_v0 = &gFile_MAPS_TAB[arg0];
        temp_s4 = temp_v0->unk0;
        temp_s1 = temp_v0[1].unk0 - temp_s4;
        temp_v0_2 = malloc(temp_s1, 5, NULL);
        queue_load_file_region_to_ptr((void *)temp_v0_2, 0x1F, temp_s4, temp_s1);
        temp_v0_2->unk20 = temp_v0->unk10 + temp_v0_2 - temp_s4;
        temp_v1 = temp_v0_2->unk8;
        var_s1 = temp_v0_2->unk20;
        temp_v0_2->unk24 = (D_800B9768.unk4[arg0].unk0 + temp_v0_2->unk4) * 640.0f;
        temp_v0_2->unk28 = (D_800B9768.unk4[arg0].unk4 + temp_v0_2->unk6) * 640.0f;
        while (var_s0 < temp_v1) {
            if ((var_s1->unk0 == 0xD) && (arg1 == var_s1->unk19)) {
                *arg2 = var_s1->unk8 + temp_v0_2->unk24;
                *arg3 = var_s1->unkC;
                *arg4 = var_s1->unk10 + temp_v0_2->unk28;
                gDLL_29_gplay->exports->func_139C(arg0, (s32) var_s1->unk18);
                for (var_s0 = 0; var_s0 < 32; var_s0++) {
                    if ((var_s1->unk1C >> var_s0_2) & 1) {
                        gDLL_29_gplay->exports->func_16C4(arg0, var_s0_2, -1);
                    } else {
                        gDLL_29_gplay->exports->func_16C4(arg0, var_s0_2, -2);
                    }
                }
                break;
            }
            temp_t8 = var_s1->unk2 * 4;
            var_s0 += temp_t8;
            var_s1 = (s8*)var_s1 + temp_t8;
        }
        free(temp_v0_2);
    }
    temp_t1 = (u32)get_file_size(0x21U) >> 5;
    if ((arg0 < 0) || (arg0 >= temp_t1)) {
        D_800B96A8 = 0;
        // var_s0_3 = sp64;
    } else {
        var_s0_3 = gMapReadBuffer;
        queue_load_file_region_to_ptr((void** ) var_s0_3, 0x21, arg0 << 5, 0x20);
        D_800B96A8 = ((s8*)var_s0_3)[0x1C];
    }
    D_800B4A72 = 0;
    if (D_800B96A8 == 1) {
        D_800B4A70 = (s16) arg0;
        D_800B4A72 = ((s16*)var_s0_3)[0xF];
    }
}
#endif

/** read_mapinfo_of_map_at_xz */
void func_800483BC(f32 worldX, f32 worldY, f32 worldZ) {
    s32 mapID;
    s32 mapInfoCount;
    MapInfo* mapInfo;

    mapID = map_get_map_id_from_xz_ws(worldX, worldZ);
    mapInfoCount = get_file_size(MAPINFO_BIN) / sizeof(MapInfo);

    if (mapID < 0 || !(mapID < mapInfoCount)) {
        D_800B96A8 = 0;
    } else {
        mapInfo = (MapInfo *)gMapReadBuffer;
        queue_load_file_region_to_ptr((void *) mapInfo, MAPINFO_BIN, mapID * (sizeof(MapInfo)), sizeof(MapInfo));
        D_800B96A8 = mapInfo->type;
    } 
    
    D_800B4A72 = 0;
    
    //Set values if it's a "mobile map" (the Galleon, "wctemplelift", etc)
    if (D_800B96A8 == 1) {
        D_800B4A70 = mapID;
        D_800B4A72 = mapInfo->mobileMapUnknown;
    }
}

s16 func_80048478(s32* arg0) {
    if (arg0) {
        *arg0 = (s32) D_800B4A70;
    }
    return D_800B4A72;
}

s8 func_80048498(void) {
    return D_80092A8C;
}

void func_800484A8(void) {
    s32 i;
    s32 j;
    s8* var_s1;

    func_80017254(0);
    func_80012B54(1, 0);
    for (i = 0; i < 5; i++) {
        var_s1 = gBlockIndices[i];
        for (j = 0; j < 256; j++) {
            func_800496E4(var_s1[j]);
        }
    }
    gLoadedBlockCount = 0;
    obj_free_all();
    for (j = 0; j < 120; j++) {
        if (gLoadedMapsDataTable[j] != NULL) {
            free(gLoadedMapsDataTable[j]);
            gLoadedMapsDataTable[j] = NULL;
        }
    }
    gDLL_Race->exports->func[0].asVoid();
    gDLL_CURVES->exports->curves_func_18.asVoid();
    gMapNumStreamMaps = 0;
    gWorldX = 0.0f;
    gWorldZ = 0.0f;
    gDLL_newclouds->exports->func[2].withTwoArgs(-1, 0);
    gDLL_minic->exports->func4();
    func_80017254(2);
}

s32 func_800485FC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GlobalMapCell* currentMap;
    s16 blockID;
    s32 fieldIndex;
    s32 i;
    s8* currentBlockIndices;

    fieldIndex = (arg1 * 0x10) + arg0;
    currentBlockIndices = gBlockIndices[arg4];
    currentMap = gDecodedGlobalMap[arg4];
    currentMap += fieldIndex;
    func_80046428(arg2, arg3, currentMap, arg4);
    blockID = currentMap->blockID;
    if (blockID < 0) {
        blockID = -1;
    }
    if (blockID < 0) {
        currentBlockIndices[fieldIndex] = blockID;
        return 0;
    }
    currentBlockIndices[fieldIndex] = -1;
    for (i = 0; i < gLoadedBlockCount; i++) {
        if (blockID == gLoadedBlockIds[i]) {
            gBlockRefCounts[i] += 1;
            currentBlockIndices[fieldIndex] = i;
            return 1;
        }
    }
    block_load(blockID, fieldIndex, arg4, 0);
    return 1;
}

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

void func_80048B14(Block* block) {
    s32 targetVertexIndex;
    s32 vertexIndex;
    s32 result;
    s32 i;
    BlockShape* currentShape;

    for (i = 0; i < block->shapeCount; i++) {
        currentShape = &block->shapes[i];
        if (currentShape->flags & 0x47C02120) {
            vertexIndex = currentShape->vtxBase;
            targetVertexIndex = currentShape[1].vtxBase;
            while (vertexIndex < targetVertexIndex) {
                result = func_80048E04(
                    block->vertices[vertexIndex].cn[0],
                    block->vertices[vertexIndex].cn[1],
                    block->vertices[vertexIndex].cn[2],
                    block->shapes[i].alpha
                );
                if (result != -1) {
                    block->vertices[vertexIndex].flag =  ((s16)block->vertices[vertexIndex].flag | ((result & 0xFF) * 4));
                }
                vertexIndex++;
            }
        }
    }
}

void func_80048C24(Block* block) {
    s32 index;
    s32 i;
    s32 targetVertexIndex;
    s32 vertexIndex;

    for (i = 0; i < block->shapeCount; i++) {
        if (block->shapes[i].flags & 0x47C02120) {
            vertexIndex = block->shapes[i].vtxBase;
            targetVertexIndex = block->shapes[i + 1].vtxBase;
            while (vertexIndex < targetVertexIndex) {
                index = ((s16)block->vertices[vertexIndex].flag >> 2) & 0xFF;
                if (D_800B97C0[index].unk8 != 0) {
                    D_800B97C0[index].unk8--;
                    if (D_800B97C0[index].unk8 == 0 && (index + 1) == D_800B97C4) {
                        while (D_800B97C0[index].unk8 == 0 && index >= 0) {
                            D_800B97C4 -= 1;
                            index--;
                        }
                    }
                }
                vertexIndex++;
            }
        }
    }
}

s32 func_80048D58(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {
    MapsUnk_800B97C0 *temp;
    s32 index;
    
    for (index = 0; index < D_800B97C4; index++){
        if ((arg0 == D_800B97C0[index].unk0) && 
            (arg1 == D_800B97C0[index].unk1) && 
            (arg2 == D_800B97C0[index].unk2) && 
            (arg3 == D_800B97C0[index].unk6)){
            return index;
        }
    }
    
    return -1;
}

s32 func_80048E04(u8 arg0, u8 arg1, u8 arg2, u8 arg3)
{
    s32 i;

    // try to get the index from another function
    i = func_80048D58(arg0, arg1, arg2, arg3);

    // if we got something valid use it to increment unk8
    // then return
    if (i != -1) {
        D_800B97C0[i].unk8++;
    } else {
        // otherwise loop through SomeArray until
        // we find the first unk8 that is zero, or the final index
        for (i = 0; i < D_800B97C4; i++) {
            if (D_800B97C0[i].unk8 == 0) {
                break;
            }
        }

        D_800B97C0[i].unk3 = arg0;
        D_800B97C0[i].unk0 = D_800B97C0[i].unk3;
        D_800B97C0[i].unk4 = arg1;
        D_800B97C0[i].unk1 = D_800B97C0[i].unk4;
        D_800B97C0[i].unk5 = arg2;
        D_800B97C0[i].unk2 = D_800B97C0[i].unk5;

        D_800B97C0[i].unk6 = arg3;
        D_800B97C0[i].unk8 = 1;

        if (i == D_800B97C4) {
            D_800B97C4++;
        }
    }

    return i;
}

void func_80048F58(void)
{
    s32 i;
    u8 r, g, b;

    func_8001F81C(&r, &g, &b);

    for (i = 0; i < D_800B97C4; i++) {
        if (D_800B97C0[i].unk6 != 0xFE) {
            D_800B97C0[i].unk3 = (D_800B97C0[i].unk0 * r) >> 8;
            D_800B97C0[i].unk4 = (D_800B97C0[i].unk1 * g) >> 8;
            D_800B97C0[i].unk5 = (D_800B97C0[i].unk2 * b) >> 8;
        }
    }
}

void block_emplace(BlocksModel *block, s32 id, s32 param_3, s32 globalMapIdx)
{
    s32 slot;
    s8 *ptr;

    for (slot = 0; slot < gLoadedBlockCount; slot++) {
        if (gLoadedBlockIds[slot] == -1) {
            break;
        }
    }


    if (slot == gLoadedBlockCount) {
        gLoadedBlockCount++;
    }

    ptr = gBlockIndices[globalMapIdx];
    ptr[param_3] = slot;
    
    gLoadedBlocks[slot] = block;
    gLoadedBlockIds[slot] = id;
    gBlockRefCounts[slot] = 1;

    if (block->unk_3e != 0) {
        block_compute_vertex_colors(block, 0, 0, 1);
    }

    func_80058F3C();
}

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

/*
 * blocks_free?
 */
void func_800496E4(s32 blockIndex) {
    BlocksModel *block;
    s32 i;
    u8 runtimeValue;
    s32* temp_a0_2;

    if (blockIndex < 0) {
        return;
    }
    
    gBlockRefCounts[blockIndex] -= 1;
    if (gBlockRefCounts[blockIndex] == 0) {
        block = gLoadedBlocks[blockIndex];
        func_80048C24((Block *) block);
        gLoadedBlockIds[blockIndex] = -1;
        gLoadedBlocks[blockIndex] = NULL;
        if (block->unk_48 != 0) {
            func_80049FA8(block);
        }

        //Loop over facebatches and free them
        for (i = 0; i < block->faceBatch_count; i++){
            runtimeValue = (&block->ptr_faceBatches[i])->runtimeValue;
            if (runtimeValue != 0xFF) {
                func_80049D38(runtimeValue);
            }
        }

        //Loop over materials and free their textures
        for (i = 0; i < block->material_count; i++){
            texture_destroy((&block->ptr_materials[i])->textureID);
        }
        
        if ((u32*)block->unk_1c != NULL) {
            free((u32*)block->unk_1c);
        }
        
        func_80058F3C();
        free(block);
    }
}

u32 hits_get_size(s32 id) {
    u32 size = gFile_HITS_TAB[id + 1] - gFile_HITS_TAB[id];
    return size;
}

HitsLine* block_load_hits(BlocksModel *block, s32 blockID, u32 unused, HitsLine* hits_ptr) {
    s32 hits_start;
    s32 hits_size;
    s32 lineIndex;
    HitsLine *line;

    hits_start = gFile_HITS_TAB[blockID];
    hits_size = gFile_HITS_TAB[blockID + 1] - hits_start;
    
    if (hits_size > 0) {
        block->ptr_hits_lines = hits_ptr;
        read_file_region(HITS_BIN, hits_ptr, hits_start, hits_size);        
        hits_ptr = (HitsLine*)((u8*)hits_ptr + hits_size);
    }
    block->hits_line_count = hits_size / sizeof(HitsLine);

    //This loops over the lines, checking whether their points' X/Z coordinates are in-bounds of their parent BLOCK
    //Potential optimisation: skip this by pre-processing the HITS.bin folder & ensuring all points are inbounds? (They are in Dec 2000's files, I think!)
    for (lineIndex = 0; lineIndex < block->hits_line_count; lineIndex++){
        line = &block->ptr_hits_lines[lineIndex];

        if (line->Ax < 0 || line->Bx < 0 || line->Ax > BLOCKS_GRID_UNIT || line->Bx > BLOCKS_GRID_UNIT) {
            line->settingsB = 0x40;
            line = &block->ptr_hits_lines[lineIndex];
        } 
        if (line->Az < 0 || line->Bz < 0 || line->Az > BLOCKS_GRID_UNIT || line->Bz > BLOCKS_GRID_UNIT) {
            line->settingsB = 0x40;
        }
    }
                
    block->unk_1c = 0;
    block->unk_3c = 0;
    block->flags &= 0xFFBF;
    
    return hits_ptr;
}

void func_800499B4(){
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800499BC.s")
#else
void func_800499BC(void) {
    s32 var_a1;
    s32 var_a2;
    s32 var_a3;
    s32 var_t1;
    s32 i;
    UnkTextureStruct *temp_a0;

    for (i = 0; i < 58; i++){
        temp_a0 = &D_800B97A8[i];
        if (temp_a0->unk20 != 0) {
            var_a1 = temp_a0->unk8;
            var_a3 = temp_a0->unk4 * delayByte;
            var_t1 = temp_a0->unk6 * delayByte;
            temp_a0->unk8 = (var_a1 + var_a3) & 3;
            temp_a0->unk0 += (var_a1 + var_a3) >> 2;
            var_a2 = temp_a0->unkA;
            temp_a0->unkA = (var_a2 + var_t1) & 3;
            temp_a0->unk2 += (var_a2 + var_t1) >> 2;
            if (temp_a0->unkC < temp_a0->unk0) {
                temp_a0->unk0 -= temp_a0->unkC;
            } else if (temp_a0->unk0 < -temp_a0->unkC) {
                temp_a0->unk0 += temp_a0->unkC;
            }
            if (temp_a0->unkE < temp_a0->unk2) {
                temp_a0->unk2 -= temp_a0->unkE;
            } else if (temp_a0->unk2 < -temp_a0->unkE) {
                temp_a0->unk2 += temp_a0->unkE;
            }
            var_a1 = temp_a0->unk18;
            var_a3 = temp_a0->unk14 * delayByte;
            var_t1 = temp_a0->unk16 * delayByte;
            temp_a0->unk18 = (var_a1 + var_a3) & 3;
            temp_a0->unk10 += (var_a1 + var_a3) >> 2;
            var_a2 = temp_a0->unk1A;
            temp_a0->unk1A = (var_a2 + var_t1) & 3;
            temp_a0->unk12 += (var_a2 + var_t1) >> 2;
            if (temp_a0->unk1C < temp_a0->unk10) {
                temp_a0->unk10 -= temp_a0->unk1C;
            } else if (temp_a0->unk10 < -temp_a0->unk1C) {
                temp_a0->unk10 += temp_a0->unk1C;
            }
            if (temp_a0->unk1E < temp_a0->unk12) {
                temp_a0->unk12 -= temp_a0->unk1E;
            } else if (temp_a0->unk12 < -temp_a0->unk1E) {
                temp_a0->unk12 += temp_a0->unk1E;
            }
        }
    }
}
#endif

// unused?
s32 func_80049B84(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    UnkTextureStruct* temp;
    s32 index;
    s32 i;

    i = 0;
    while (1) {
        temp = &D_800B97A8[i];
        if (
            (arg0 == temp->unk4) &&
            (arg1 == temp->unk6) &&
            (arg2 == temp->unkC) &&
            (arg3 == temp->unkE) &&
            (arg4 == temp->unk14) &&
            (arg5 == temp->unk16) &&
            (arg6 == temp->unk1C) &&
            (arg7 == temp->unk1E)
        ) {
            temp->unk20++;
            return i;
        }
        i++;
        if (i >= 0x3A) {
            index = -1;
            for (i = 0; i < 0x3A; i++) {
                if (D_800B97A8[i].unk20 == 0) {
                    index = i;
                    break;
                }
            }
            if (index == -1) {
                return -1;
            }
            temp = &D_800B97A8[index];
            temp->unk4 = arg0;
            temp->unk6 = arg1;
            temp->unkC = arg2;
            temp->unkE = arg3;
            temp->unk0 = 0;
            temp->unk2 = 0;
            temp->unk8 = 0;
            temp->unkA = 0;
            temp->unk14 = arg4;
            temp->unk16 = arg5;
            temp->unk1C = arg6;
            temp->unk1E = arg7;
            temp->unk10 = 0;
            temp->unk12 = 0;
            temp->unk18 = 0;
            temp->unk1A = 0;
            temp->unk20 += 1;
            return index;
        }
    }
}

void func_80049CE4(u32 a0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8){
    UnkTextureStruct *temp_v0;

    temp_v0 = &D_800B97A8[a0];
    temp_v0->unk4 = arg1;
    temp_v0->unk6 = arg2;
    temp_v0->unkC = arg3;
    temp_v0->unkE = (s16) arg4; 
    temp_v0->unk14 = (s16) arg5; 
    temp_v0->unk16 = (s16) arg6; 
    temp_v0->unk1C = (s16) arg7; 
    temp_v0->unk1E = (s16) arg8; 
}

void func_80049D38(u32 arg0) {
    if ((s32) D_800B97A8[arg0].unk20 > 0) {
        D_800B97A8[arg0].unk20 -= 1;
    }
}

s32 func_80049D68(s32 arg0) {
    return (s32)&D_800B97A8[arg0];
}

void func_80049D88(void)
{
    s32 i;
    Texture *texture;

    for (i = 0; i != 20; i++) {
        if (gBlockTextures[i].refCount != 0) {
            texture = gBlockTextures[i].texture;

            if ((texture != NULL) && (texture->levels != 0x100)) {
                if (texture->unk_0xe != 0) {
                    func_8003E648(texture, &gBlockTextures[i].flags, &gBlockTextures[i].unk_0x4);
                }
            }
        }
    }
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/map/block_setup_textures.s")
#else
void* block_setup_textures(Block* block) {
    s32 var_a1;
    s32 var_s1;
    s32 i;
    s32 j;
    u32 var_t0;
    s32 var_s6;
    s32 var_a2;
    BlockShape* temp_a3;

    var_s1 = 0;
    var_s6 = 0;
    for (i = 0; i < block->shapeCount; i++) {
        temp_a3 = &block->shapes[i];
        if (temp_a3->flags & 0x4000) {
            var_s6++;
        }
        if (temp_a3->flags & 0x10000 && temp_a3->unk_0x14 != 0) {
            var_a1 = FALSE;
            for (j = 0; j < var_s1; j++) {
                if (block->unk_0x28[j].unk_0x2 == temp_a3->unk_0x14) {
                    var_a1 = TRUE;
                    break;
                }
            }
            var_a2 = temp_a3->unk_0x14;
            if (var_a1 == FALSE) {
                var_a1 = temp_a3->flags;
                block->unk_0x28[var_s1].texIdx = func_8004A058(block->tiles[temp_a3->tileIdx0].texture, var_a1, var_a2);
                block->unk_0x28[var_s1].unk_0x2 = block->shapes[i].unk_0x14;
                var_s1++;
            } else {
                var_a1 = temp_a3->flags;
                func_8004A058(block->tiles[temp_a3->tileIdx0].texture, var_a1, var_a2);
            }
        }
    }
    block->unk_0x49 = var_s6;
    block->unk_0x48 = var_s1;
    return var_s1 * 4;
}
#endif

void func_8004A164(Texture*, s32);

void func_80049FA8(BlocksModel* block) {
    s32 index;
    u8 animatorID;
    FaceBatch* facebatch;

    for (index = 0; index < block->faceBatch_count; index++){
        facebatch = &block->ptr_faceBatches[index];
        if (facebatch->renderSettingBitfield & 0x10000) {
            animatorID = facebatch->animatorID;
            if (animatorID){
                func_8004A164(block->ptr_materials[facebatch->materialID].textureID, animatorID);
            }
        }
    }
}

s32 func_8004A058(Texture* tex, u32 flags, s32 arg2) {
    s32 index;
    s32 indexOfUnref;
    
    indexOfUnref = -1;
    for (index = 0; index < 0x14; index++){
        if ((&gBlockTextures[index])->refCount != 0 && 
            tex == (&gBlockTextures[index])->texture && 
            arg2 == (&gBlockTextures[index])->unk_0x14){
            
            indexOfUnref = index;
            break;
        }
    }
    
    if (indexOfUnref != -1){
        (&gBlockTextures[indexOfUnref])->refCount += 1;
        return indexOfUnref;
    }
    
    indexOfUnref = -1;
    for (index = 0; index < 0x14; index++){
        if ((&gBlockTextures[index])->refCount == 0){
            indexOfUnref = index;
            break;
        }
    }
    
    if (indexOfUnref != -1){
        gBlockTextures[indexOfUnref].refCount = 1;
        gBlockTextures[indexOfUnref].unk_0x4 = 0;
        gBlockTextures[indexOfUnref].flags = flags;
        gBlockTextures[indexOfUnref].texture = tex;
        gBlockTextures[indexOfUnref].unk_0x14 = arg2;
        return indexOfUnref;
    }
    return 0;
}

void func_8004A164(Texture *matchTexture, s32 matchParam) {
    s32 index;
    for (index = 0; index < 0x14; index++){
        if (matchTexture == gBlockTextures[index].texture && matchParam == gBlockTextures[index].unk_0x14){
            if (gBlockTextures[index].refCount > 0){
                gBlockTextures[index].refCount--;
            }
        }
    }
}

/** blocks_get_texture_by_some_value? */
Texture* func_8004A1E8(s32 match_value) {
    s32 blockIndex;
    BlocksModel *block;
    s32 textureIndex;
    
    for (blockIndex = 0; blockIndex < gLoadedBlockCount; blockIndex++){
        block = gLoadedBlocks[blockIndex];
        
        if (block){
            for (textureIndex = 0; textureIndex < block->unk_48; textureIndex++){
                if (match_value == (&block->unk_28[textureIndex])->unk02){
                    textureIndex = (&block->unk_28[textureIndex])->textureIndex;
                    return gBlockTextures[textureIndex].texture;
                }
            }
        }
    }
    return NULL;
}

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

// https://decomp.me/scratch/cLba8
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/block_setup_xz_bitmap.s")
#else
void block_setup_xz_bitmap(Block* block) {
    Vtx_t** temp_s3;
    s32 triIndex;
    s32 temp_a2;
    s32 nextTriIndex;
    s16 temp_s4;
    s16 var_s0;
    s16 var_s1;
    s16 var_s2;
    s16 var_s5;
    s16 var_t3;
    s16 var_t4;
    s16 var_t5;
    s32 var_t2;
    BlockShape* blockShape;
    s32 var_v1;
    s32 i;
    Vtx_t *sp38[3];

    for (var_t2 = 0; var_t2 < block->shapeCount; var_t2++) {
        blockShape = &block->shapes[var_t2];
        triIndex = blockShape->triBase;
        nextTriIndex = blockShape[1].triBase;
        temp_a2 = blockShape->vtxBase;
        while (triIndex < nextTriIndex) {
            var_t3 = -0x7D00;
            var_t4 = 0x7D00;
            var_t5 = -0x7D00;
            var_s0 = 0x7D00;
            sp38[0] = &block->vertices[(((s32)block->encodedTris[triIndex].d0 >> 0xD) & 0x1F) + temp_a2];
            sp38[1] = &block->vertices[(((s32)block->encodedTris[triIndex].d0 >> 7) & 0x1F) + temp_a2];
            sp38[2] = &block->vertices[(((s32)block->encodedTris[triIndex].d0 >> 1) & 0x1F) + temp_a2];
            i = 0;
            while (i < 3) {
                var_s1 = (*sp38)[i].ob[0];
                temp_s4 = (*sp38)[i].ob[2];
                if (var_t3 < var_s1) {
                    var_t3 = var_s1;
                }
                if (var_s1 < var_t4) {
                    var_t4 = var_s1;
                }
                if (var_t5 < temp_s4) {
                    var_t5 = temp_s4;
                }
                if (temp_s4 < var_s0) {
                    var_s0 = temp_s4;
                }
                i++;
            }
            for (var_s5 = 0, var_s1 = 1, var_s2 = 0; var_s2 < 0x280; var_s2 += 0x50) {
                if (((var_s2 + 0x50) >= var_t4) && (var_t3 >= var_s2)) {
                    var_s5 |= var_s1;
                }
                var_s1 *= 2;
            }
            for (var_s2 = 0; var_s2 < 0x280; var_s2 += 0x50) {
                if (((var_s2 + 0x50) >= var_s0) && (var_t5 >= var_s2)) {
                    var_s5 |= var_s1;
                }
                var_s1 *= 2;
            }
            block->xzBitmap[triIndex] = var_s5;
            triIndex++;
        }
    }
}
#endif

/**
  * block_get_animator_vertex_count?
  *
  * (uses the animator object's world position to find the relevant block)
  */
s32 func_8004A528(Object* obj, u8 animatorID) {
    s32 index;
    s32 anim_vertex_count;
    BlocksModel *block;
    FaceBatch *facebatches;

    block = func_80044BB0(func_8004454C(obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z));
    if (!block || !(block->flags & 8)){
        return 0;
    }
    
    anim_vertex_count = 0;
    facebatches = block->ptr_faceBatches;
    for (index = 0; index < block->faceBatch_count; index++){
        if (animatorID == (facebatches[index]).animatorID){
            anim_vertex_count += facebatches[index + 1].baseVertexID - facebatches[index].baseVertexID;
        }
    }
    
    return anim_vertex_count;
}

s32 func_8004A5D8(Object* obj, u8 arg1) {
    Block* block;
    s32 out;
    s32 i;
    BlockShape *shapes;

    block = func_80044BB0(func_8004454C(obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z));
    if ((block == NULL) || !(block->vtxFlags & 8)) {
        return 0;
    }

    for (i = 0, out = 0, shapes = block->shapes; i < block->shapeCount; i++) {
        if (arg1 == shapes[i].unk_0x14) {
            out += 1;
        }
    }
    return out;
}

void func_8004A67C(void) {
    s32 pad;
    s32 i;
    s32 id;
    s32 count;
    f32 xx;
    f32 yy;
    f32 zz;
    Camera* camera;
    Object* obj;
    Object** actors;

    actors = obj_get_all_of_type(7, &count);
    camera = get_camera();
    update_camera_for_object(camera);

    for (i = 0; i < 20;) {
        Vec3_Int_array[i++].i = 0;
    }

    // set first element
    Vec3_Int_array->f.x = camera->tx;
    Vec3_Int_array->f.y = camera->ty;
    Vec3_Int_array->f.z = camera->tz;
    Vec3_Int_array->i = 1;

    // loop through the rest
    for (i = 0; i < count; i++) {
        obj = actors[i];

        id = obj->matrixIdx + 1;

        // if actor 7 is the camera actor
        // use camera x/y/z
        if (obj == camera->object) {
            Vec3_Int_array[id].f.x = camera->srt.transl.x;
            Vec3_Int_array[id].f.y = camera->srt.transl.y;
            Vec3_Int_array[id].f.z = camera->srt.transl.z;
        } else {
            inverse_transform_point_by_object(
                camera->tx,
                camera->ty,
                camera->tz,
                &xx,
                &yy,
                &zz,
                obj
            );

            Vec3_Int_array[id].f.x = xx;
            Vec3_Int_array[id].f.y = yy;
            Vec3_Int_array[id].f.z = zz;
        }
        Vec3_Int_array[id].i = 1;
    }
}

void map_update_objects_streaming(s32 arg0) {
    GlobalMapCell* var_a3;
    s32 spB8;
    MapHeader* temp_s6;
    ObjCreateInfo* temp_s2;
    Object* temp_s0;
    Object* temp_s5;
    s16 temp_v0_3;
    s16 var_s4;
    s32 var_a1;
    s32 sp9C;
    s32 var_a2;
    s32 var_s1;
    s32 var_s2;
    s32 var_s3;
    s32 var_v0;
    ObjCreateInfo* var_s1_2;
    s32 temp_s7;
    s32 temp_s4;
    u32 var_s0;
    s16 sp70[3]; // unknown size however 3 feels way to small?
    void* temp_fp;
    Object** sp68;
    s16 mapID;

    var_s4 = 0;
    for (spB8 = 0; spB8 < 5; spB8++) {
        var_a2 = 0;
        var_a3 = gDecodedGlobalMap[spB8] + 119;
        for (var_a2 = 0; var_a2 < 3; var_a2++) {
            if (var_a3->mapIDs[var_a2] >= 0 && var_a3->mapIDs[var_a2] < 0x50) {
                var_a1 = 0;
                if (gLoadedMapsDataTable[var_a3->mapIDs[var_a2]] != NULL) {
                    for (var_v0 = 0; var_v0 < var_s4; var_v0++) {
                        if (sp70[var_v0] == var_a3->mapIDs[var_a2]) {
                            var_a1 = 1;
                            break;
                        }
                    }
                    if (var_a1 == 0) {
                        sp70[var_s4++] = var_a3->mapIDs[var_a2];
                    }
                }
            }
        }
    }
    
    sp68 = get_world_objects(&spB8, &sp9C);
    while (spB8 < sp9C) {
        var_s1 = 0;
        temp_s0 = sp68[spB8];
        temp_s2 = temp_s0->createInfo;
        spB8++;
        if (temp_s0->mapID >= 0) {
            if (!(temp_s2->loadParamA & 2)) {
                if (temp_s2->loadParamA & 0x10) {
                    if ((temp_s0->group >= 0) && (func_8004B190(temp_s0) != 0)) {
                        var_s1 = 1;
                    } else if ((temp_s0->mapID < 0x50) && (gLoadedMapsDataTable[temp_s0->mapID] == NULL)) {
                        var_s1 = 1;
                    }
                } else {
                    if ((temp_s0->group >= 0) && (func_8004B190(temp_s0) != 0)) {
                        var_s1 = 1;
                    } else if ((temp_s0->mapID < 0x50) && (func_8004AEFC(temp_s0->mapID, sp70, var_s4) == 0)) {
                        var_s1 = 1;
                    }
                }
            }
        }
        if (var_s1 != 0) {
            if (gLoadedMapsDataTable[temp_s0->mapID] != NULL) {
                if (temp_s0->unk0xb2 >= 0) {
                    func_8004B710(temp_s0->unk0xb2, temp_s0->mapID, 0U);
                }
            }
            if (temp_s0->id == 0x72) {
                func_8004AEFC(temp_s0->mapID, sp70, var_s4);
            }
            obj_destroy_object(temp_s0);
            spB8 -= 1;
            sp9C -= 1;
        }
    }
    for (spB8 = 0; spB8 < 0x50; spB8++) {
        if (gLoadedMapsDataTable[spB8] != NULL) {
            var_s0 = gDLL_29_gplay->exports->func_163C(spB8);
            if (var_s0 != 0) {
                gDLL_29_gplay->exports->func_1680(spB8);
                var_s2 = 0;
                while (var_s0 != 0) {
                    if (var_s0 & 1) {
                        func_8004B548(gLoadedMapsDataTable[spB8], spB8, var_s2, NULL);
                    }
                    var_s0 >>= 1;
                    var_s2++;
                }
            }
        }
    }
    for (spB8 = 0; spB8 < var_s4; spB8++) {
        temp_v0_3 = sp70[spB8];
        temp_s6 = gLoadedMapsDataTable[temp_v0_3];
        if (temp_s6 != NULL) {
            var_s1_2 = (ObjCreateInfo* ) temp_s6->objectInstanceFile_ptr;
            temp_s7 = temp_s6->unk19;
            var_s3 = 0;
            // mmmmmm yummmy casting shenanigans
            temp_fp = *((s32*)&((s8 *)&D_800B5590)[temp_v0_3 * 0x8C]) + (s32)var_s1_2;
            while ((u32) var_s1_2 < (u32) temp_fp) {
                temp_s2 = var_s1_2;
                if ((var_s1_2->loadParamA & 0x10)) {
                    break;
                }

                if ((map_check_some_mapobj_flag(var_s3, (u32) sp70[spB8]) == 0) && (map_should_stream_load_object(var_s1_2, 0, sp70[spB8]) != 0)) {
                    func_8004B710(var_s3, (u32) sp70[spB8], 1U);
                    if (arg0 != 0) {
                        obj_create(var_s1_2, 1U, sp70[spB8], var_s3, NULL);
                    } else if (map_get_is_object_streaming_disabled() != 0) {
                        func_80012584(0x3E, 4U, NULL, (UnkStructAssetThreadSingle_0x8* ) var_s1_2, sp70[spB8], var_s3, 0, temp_s7);
                    } else {
                        obj_create(var_s1_2, 1U, sp70[spB8], var_s3, NULL);
                    }
                }
                var_s3++;
                var_s1_2 = (ObjCreateInfo* ) &((s32 *)var_s1_2)[temp_s2->quarterSize];
            }
        }
    }
    sp68 = obj_get_all_of_type(7, &sp9C);
    for (spB8 = 0; spB8 < sp9C; spB8++) {
        temp_s5 = sp68[spB8];
        var_s3 = 0;
        temp_s4 = temp_s5->unk_0x34;
        temp_s6 = gLoadedMapsDataTable[temp_s4];
        if (temp_s6 != NULL) {
            temp_s7 = temp_s5->matrixIdx + 1;
            var_s1_2 = (ObjCreateInfo* ) temp_s6->objectInstanceFile_ptr;
            // mmmmmm yummmy casting shenanigans
            temp_fp = *((s32*)&((s8 *)&D_800B5590)[temp_s4 * 0x8C]) + (s32)var_s1_2;
            var_s0 = gDLL_29_gplay->exports->func_163C((s32) temp_s4);
            if (var_s0 != 0) {
                gDLL_29_gplay->exports->func_1680((s32) temp_s4);
                var_s2 = 0;
                while (var_s0 != 0) {
                    if (var_s0 & 1) {
                        func_8004B548(temp_s6, (s32) temp_s4, var_s2, temp_s5);
                    }
                    var_s0 >>=1;
                    var_s2++;
                }
            }
            while ((u32) var_s1_2 < (u32) temp_fp) {
                temp_s2 = var_s1_2;
                if ((map_check_some_mapobj_flag(var_s3, temp_s4) == 0) && (map_should_stream_load_object((ObjCreateInfo* ) var_s1_2, temp_s7, temp_s4) != 0)) {
                    func_8004B710(var_s3, temp_s4, 1U);
                    if (arg0 != 0) {
                        obj_create((ObjCreateInfo* ) var_s1_2, 1U, temp_s4, var_s3, temp_s5);
                    } else if (map_get_is_object_streaming_disabled() != 0) {
                        func_80012584(0x3D, 4U, NULL, (UnkStructAssetThreadSingle_0x8* ) var_s1_2, temp_s4, var_s3, temp_s5, temp_s7);
                    } else {
                        obj_create((ObjCreateInfo* ) var_s1_2, 1U, temp_s4, var_s3, temp_s5);
                    }
                }
                var_s3++;
                var_s1_2 = (ObjCreateInfo* ) &((s32 *)var_s1_2)[temp_s2->quarterSize];
            }
        }
    }
}

s32 func_8004AEFC(s32 mapID, s16 *arg1, s16 searchLimit) {
    s16 searchIndex;

    for (searchIndex = 0; searchIndex < searchLimit; searchIndex++){
        if (mapID == *(arg1 + searchIndex)){
            return 1;
        }
    }
    
    return 0;
}

s32 map_should_stream_load_object(ObjCreateInfo* arg0, s8 arg1, s32 arg2) {
    s32 scaledX;
    Object* player;
    s32 scaledZOrFlag;
    f32 zDiff;
    f32 xDiff;
    f32 scaledLoadDistance;
    f32 yDiff;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    s8 stop;
    s8 i;
    s8 *currentBlockIndices;

    if (func_8004B4A0(arg0, arg2) == 0) {
        return 0;
    }

    if (arg0->loadParamA & 1) {
        return 1;
    }

    if (arg0->loadParamA & 2) {
        return 0;
    }

    if (arg1 == 0) {
        scaledX = floor_f((arg0->x - gWorldX) / 640.0f);
        scaledZOrFlag = floor_f((arg0->z - gWorldZ) / 640.0f);
        if (scaledX < 0 || scaledZOrFlag < 0 || scaledX >= 0x10 || scaledZOrFlag >= 0x10) {
            return 0;
        }

        stop = 0;
        scaledX = (scaledZOrFlag * 0x10) + scaledX;
        for (i = 0; i < 5; i++) {
            currentBlockIndices = gBlockIndices[i];
            if (currentBlockIndices[scaledX] >= 0) {
                stop = 1;
            }
        }
        if (stop == 0) {
            return 0;
        }
    }

    if (arg0->loadParamA & 0x20) {
        return 1;
    }

    scaledZOrFlag = 0;
    if ((arg0->loadParamA & 4) && (arg1 == 0)) {
        player = get_player();
        if (player != NULL) {
            sp20 = player->positionMirror.x;
            sp1C = player->positionMirror.y;
            sp18 = player->positionMirror.z;
        } else {
            scaledZOrFlag = 1;
        }
    } else {
        scaledZOrFlag = 1;
    }

    if (scaledZOrFlag != 0) {
        sp20 = Vec3_Int_array[arg1].f.x;
        sp1C = Vec3_Int_array[arg1].f.y;
        sp18 = Vec3_Int_array[arg1].f.z;
    }

    scaledLoadDistance = arg0->loadDistance * 8;
    xDiff = sp20 - arg0->x;
    yDiff = sp1C - arg0->y;
    zDiff = sp18 - arg0->z;
    xDiff = ((xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff));
    if (xDiff < (scaledLoadDistance * scaledLoadDistance)) {
        return 1;
    }

    return 0;
}

s32 func_8004B190(Object* arg0) {
    s32 sp54;
    s32 pad[6];
    f32 sp38;
    s32 pad2[6];
    ObjCreateInfo* sp1C;
    s32 temp_v0_2;
    Object* player;
    f32 temp_fv1;
    f32 var_fa0;
    f32 var_fa1;
    f32 var_ft4;
    f32 var_ft5;
    f32 var_fv0;
    f32 var_fv1;
    s32 var_a0;
    s32 var_a1;
    s8 *currentBlockIndices;
    s32 i;

    sp1C = arg0->createInfo;
    if (sp1C == NULL) {
        return 0;
    }
    if (func_8004B4A0(sp1C, arg0->mapID) == 0) {
        return 1;
    }
    if (sp1C->loadParamA & 1) {
        return 0;
    }
    if (sp1C->loadParamA & 0x10) {
        if (gDLL_29_gplay->exports->func_14F0(arg0->mapID, sp1C->loadDistance) != 0) {
            return 0;
        }
        return 1;
    }
    if (sp1C->loadParamA & 2) {
        return 0;
    }
    if ((arg0->unk0xc0 != NULL) && (arg0->unk0xb4 < 0)) {
        return 0;
    }
    if (arg0->parent == NULL) {
        sp54 = floor_f((arg0->srt.transl.x - gWorldX) / 640.0f);
        temp_v0_2 = floor_f((arg0->srt.transl.z - gWorldZ) / 640.0f);
        if ((sp54 < 0) || (temp_v0_2 < 0) || (sp54 >= 0x10) || (temp_v0_2 >= 0x10)) {
            return 1;
        }
        sp54 = (temp_v0_2 * 0x10) + sp54;
        for (var_a1 = FALSE, i = 0; i < 5; i++) {
            currentBlockIndices = gBlockIndices[i];
            if (currentBlockIndices[sp54] >= 0) {
                var_a1 = TRUE;
            }
        }
        if (var_a1 == FALSE) {
            return 1;
        }
    }
    if (sp1C->loadParamA & 0x20) {
        return 0;
    }
    if ((sp1C->loadParamA & 4) && (player = get_player(), (player != NULL)) && arg0->parent == NULL) {
        var_fv1 = player->positionMirror.x;
        var_fa0 = player->positionMirror.y;
        var_fa1 = player->positionMirror.z;
    } else {
        if (arg0->parent != NULL) {
            var_a0 = arg0->parent->matrixIdx + 1;
        } else {
            var_a0 = 0;
        }
        var_fv1 = Vec3_Int_array[var_a0].f.x;
        var_fa0 = Vec3_Int_array[var_a0].f.y;
        var_fa1 = Vec3_Int_array[var_a0].f.z;
    }
    sp38 = arg0->unk0x3c;
    if (arg0->parent != NULL) {
        var_fv0 = var_fv1 - arg0->srt.transl.x;
        var_ft4 = var_fa0 - arg0->srt.transl.y;
        var_ft5 = var_fa1 - arg0->srt.transl.z;
    } else {
        var_fv0 = var_fv1 - arg0->positionMirror.x;
        var_ft4 = var_fa0 - arg0->positionMirror.y;
        var_ft5 = var_fa1 - arg0->positionMirror.z;
    }
    temp_fv1 = sp38 + 40.0f;
    var_fv0 = ((var_fv0 * var_fv0) + (var_ft4 * var_ft4) + (var_ft5 * var_ft5));
    if (var_fv0 < (temp_fv1 * temp_fv1)) {
        return 0;
    }
    return 1;
}

/** map_should_object_load? */
s32 func_8004B4A0(ObjCreateInfo* obj, s32 arg1) {
    u8 gplayValue;

    gplayValue = gDLL_29_gplay->exports->func_143C(arg1);
    if (gplayValue == -1) {// NOLINT
        return 0;
    }
    if (gplayValue != 0) {
        if (gplayValue < 9) {
            if ((obj->setup >> (gplayValue + 0x1F)) & 1) { //bitshift by 0x1f?? But setup is a byte value... hmm.
                return 0;
            }
        }
        else if ((obj->loadParamB >> (0x10 - gplayValue)) & 1) {
            return 0;
        }
    }
    return 1;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B548.s")
#else
void func_80012584(s32, u8, u32, ObjCreateInfo*, s32, s32, Object*, s32);
s32 func_8004B4A0(ObjCreateInfo*, s32);

void func_8004B548(MapHeader* map, s32 mapID, s32 arg2, Object* arg3) {
    u32 new_var;
    s32 objects_filesize;
    s32 object_count;
    s32 var_v1;
    s8 var_s6;
    u32 objects_offset;
    u32 objects_end;
    ObjCreateInfo *obj;
    u32 *temp_t1;
    
    temp_t1 = (u32 *) ((mapID * 0x8C) + (&D_800B5508));
    objects_filesize = temp_t1[arg2];
    
    if (objects_filesize != -1){
        if (arg3 != NULL){
            var_s6 = arg3->matrixIdx + 1;
        } else {
            var_s6 = map->unk19;
        }
        
        objects_offset = map->objectInstanceFile_ptr;
        objects_end = objects_filesize + objects_offset;
        obj = objects_offset;
        object_count = 0;

        while ((u32)&obj < objects_end){
            object_count ++;
            obj += obj->quarterSize * 4;
        }

        var_v1 = arg2 + 1;
        while (var_v1 < 0x21){
            if (temp_t1[var_v1] != -1){
                break;
            }
            var_v1++;
        }
        
        obj = objects_end;
        objects_end = temp_t1[var_v1] + objects_offset;
        while (new_var < objects_end){
            if ((map_check_some_mapobj_flag(object_count, mapID) == 0) && (func_8004B4A0((ObjCreateInfo *) obj, mapID) != 0)){
                func_8004B710(object_count, mapID, 1);
                if (map_get_is_object_streaming_disabled() != 0){
                    func_80012584(0x3E, 4, 0, (ObjCreateInfo *) obj, mapID, object_count, arg3, (s32) var_s6);
                } else {
                    obj_create((ObjCreateInfo *) obj, 1U, mapID, object_count, arg3);
                }
            }
            object_count++;
            obj += obj->quarterSize * 4;
        }
    }
}
#endif

/**
 * Seems to be called when camera moves between grid cells?
 */
 void func_8004B710(s32 cellIndex_plusBitToCheck, u32 mapIndex, u32 arg2) {
    s32 cell_offset;
    s32 bit_at_index;
    MapHeader *map;

    if (cellIndex_plusBitToCheck < 0) {
        return;
    }
    
    map = gLoadedMapsDataTable[mapIndex];
    
    cell_offset = cellIndex_plusBitToCheck >> 3; //upper part of arg0 used to choose cell within grid?
    bit_at_index = 1 << (cellIndex_plusBitToCheck & 7); //lower part of arg0 used to choose a bit from 0-7
    map->end_ptr[cell_offset] &= ~bit_at_index; //masks something to do with the grid cell?
    
    if (arg2 != 0) {
        map->end_ptr[cell_offset] |= bit_at_index; //sets a bit to do with the grid cell?
    }
}

s32 map_check_some_mapobj_flag(s32 cellIndex_plusBitToCheck, u32 mapIndex) {
    MapHeader *map;
    s32 bit_at_index;
    u32 cell_offset;
    
    if (cellIndex_plusBitToCheck < 0){
        return 0;
    }
    
    map = gLoadedMapsDataTable[mapIndex];
    cell_offset = cellIndex_plusBitToCheck >> 3;
    bit_at_index = 1 << (cellIndex_plusBitToCheck & 7);
    if (map->end_ptr[cell_offset] & bit_at_index){
        return 1;
    }
    if (cell_offset){
        //Could be related to "trackSetLoaded bit" print strings in RODATA?
    }
    return 0;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B7D0.s")
#else
/** Find curve by uID 
    arg1 unused?
*/
ObjCreateInfo *func_8004B7D0(s32 search_uID, u32 arg1){
    MapHeader **maps;
    MapHeader *map;
    s32 index;
    
    u8 *ptr;
    s32 offset;
    s16 objID;
    
    ObjCreateInfo *obj;
    
    maps = gLoadedMapsDataTable;
    index = 0;
    do {
        s32 offset;
        map = maps[index];
        
        if (map != NULL){
            ptr = (u8*)map->objectInstanceFile_ptr;
            offset = 0;
            
            while (offset < map->objectInstancesFileLength){       
                objID = ((ObjCreateInfo*)ptr)->objId;
                if (objID == OBJ_curve && ((ObjCreateInfo*)ptr)->uID == search_uID){
                    return ((ObjCreateInfo*)ptr);
                }
        
                offset += ((ObjCreateInfo*)ptr)->quarterSize << 2;
                ptr += ((ObjCreateInfo*)ptr)->quarterSize << 2;
            }
        } 
        index++;
    } while ((u32)map != (u32)&D_800B5508);
    
    return 0;
}
#endif

/** map_find_checkpoint4_by_uID? 
  * "search_mapID == -1" searches through all maps instead of a specific mapID!
  */
ObjCreateInfo* func_8004B85C(s32 search_uID, s32 search_mapID) {
    MapHeader* map;
    s32 search_endID;

    u8 *ptr;
    s32 offset;
    
    s16 objID;
    
    if (search_mapID == -1) {
        search_mapID = 0;
        search_endID = 0x78;
    } else {
        search_endID = search_mapID + 1;
    }
    
    //Iterating over maps
    while (search_mapID < search_endID) {
        map = gLoadedMapsDataTable[search_mapID];

        //Iterating over map objects
        if (map) {
            
            ptr = (u8*)map->objectInstanceFile_ptr;
            offset = 0;
            
            while (offset < map->objectInstancesFileLength){       
                objID = ((ObjCreateInfo*)ptr)->objId;
                if (objID == OBJ_checkpoint4 && ((ObjCreateInfo*)ptr)->uID == search_uID){
                    return ((ObjCreateInfo*)ptr);
                }
        
                offset += ((ObjCreateInfo*)ptr)->quarterSize << 2;
                ptr += ((ObjCreateInfo*)ptr)->quarterSize << 2;
            }
        }

        search_mapID++;
    }

    return NULL;
}

void func_8004B914(s32 mapID) {
    MapHeader *map;

    if ((mapID >= 0) && (mapID < 0x78)) {
        map = gLoadedMapsDataTable[mapID];
        if (map != NULL) {
            map->unk18 = 1;
        }
    }
}

void func_8004B948(s32 arg0) {
    if (arg0 != 0) {
        UINT_80092a98 |= 0x200;
        return;
    }
    UINT_80092a98 &= ~0x200;
}

void func_8004B984(Unk800B96B0* arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    s16 temp_s0;
    s8 var_a1;
    s32 i;
    Unk800B96B0* temp_v0;

    temp_s0 = gDLL_29_gplay->exports->func_121C();
    temp_v0 = gDLL_29_gplay->exports->func_1254();
    if (arg0[1].unk0 == -1) {
        return;
    }
    for (var_a1 = FALSE, i = 0; i < temp_s0; i++) {
        if (arg0[1].unk0 == temp_v0[i].unk0) {
            var_a1 = TRUE;
            temp_v0[i].unk8 = arg2;
            temp_v0[i].unkC = arg3;
            temp_v0[i].unk10 = arg4;
            arg0->unk8 = arg2;\
            arg0->unkC = arg3;\
            arg0->unk10 = arg4;\
        }
    }
    if (var_a1 == FALSE) {
        D_800B96B0[temp_s0].unk8 = arg0->unk8;\
        D_800B96B0[temp_s0].unkC = arg0->unkC;\
        D_800B96B0[temp_s0].unk10 = arg0->unk10;
        D_800B96B0[temp_s0].unk0 =  arg0[1].unk0;
        D_800B96B0[temp_s0].unk4 = arg1;
        i = temp_s0;
        temp_v0[temp_s0].unk0 = arg0[1].unk0;
        temp_v0[i].unk4 = arg1;
        temp_v0[i].unk8 = arg2;
        temp_v0[i].unkC = arg3;
        // @fake
        if ((!temp_v0) && (!temp_v0)) {}
        temp_v0[i].unk10 = arg4;
        temp_s0++;
        arg0->unk8 = arg2;\
        arg0->unkC = arg3;\
        arg0->unk10 = arg4;
        gDLL_29_gplay->exports->func_1238(temp_s0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004BC20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004BD40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004C040.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/block_compute_vertex_colors.s")

/** 
  * Warps the player to coordinates stored in WARPTAB.bin 
  */
void warpPlayer(s32 warpID, s8 fadeToBlack) {
    Warp *warp;
    Warp *mostRecentWarp;

    warp = (Warp*)gMapReadBuffer;
    queue_load_file_region_to_ptr((void*)warp, WARPTAB_BIN, warpID << 4, sizeof(Warp));
    mostRecentWarp = (Warp*)&D_800B4A60;
  
    mostRecentWarp->coord.x = warp->coord.x;
    mostRecentWarp->coord.y = warp->coord.y;
    mostRecentWarp->coord.z = warp->coord.z;
    mostRecentWarp->layer = warp->layer;
    
    D_800B4A5C = (s16) warpID;
    D_800B4A58 = 1;
    D_800B4A59 = fadeToBlack;
    
    if (D_800B4A59 != 0) {
        gDLL_28_screen_fade->exports->fade(0x28, SCREEN_FADE_BLACK);
    }
    gDLL_minic->exports->func4();
    gDLL_minic->exports->func1();
    gDLL_8->exports->func[1].asVoid();
    gDLL_7_newday->exports->func1.asVoid();
    gDLL_newclouds->exports->func[1].asVoid();
    gDLL_newstars->exports->func[0].asVoid();
}

/** 
    Called every frame!
    Seems to start a fade-out followed by a warp
*/
static void func_8004D328() {
    SimilarToWarp* var_a2;
    Warp* var_v0;
    u8 temp2;
    u8 temp1;

    var_a2 = (SimilarToWarp*)gDLL_29_gplay->exports->func_F04();
    
    //Start fade?
    if (D_800B4A5E != -1) { //timer started?
        
        D_80092A78 -= 1; //Decrement timer
        if (D_80092A78 < 0) { //When timer less than zero, fade to black
            if ((D_800B4A5E >= 0) && (D_800B4A59 != 0)) {
                gDLL_28_screen_fade->exports->fade_reversed(0x1E, 1);
            }
            D_800B4A5E = -1; //stop timer?
        }
    }
    
    //Warp after fade?
    if (D_800B4A58 == 0)
        return;
        
    if (gDLL_28_screen_fade->exports->is_complete() || D_800B4A59 == 0){
        var_v0 = (Warp*)&D_800B4A60;
        D_800B4A58 = 0;
        var_a2->coord.x = var_v0->coord.x;
        var_a2->coord.y = var_v0->coord.y;
        var_a2->coord.z = var_v0->coord.z;
        var_a2->layer[1] = (s8)var_v0->layer;
        func_800143A4();
        D_800B4A5E = D_800B4A5C;
        D_800B4A5C = -1;
        D_80092A78 = 8;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D470.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D698.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D844.s")

void func_8004D880(Object *arg0) {
    ObjectStruct64* temp_v0;

    if (arg0->ptr0x64 != NULL) {
        arg0->ptr0x64->flags &= ~0x20;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004D8A4.s")

void func_8004D974(s32 arg0) {
    D_80092BE8 = (s8) arg0;
}

void func_8004D984(s32 arg0) {
    D_80092BE8 = 1;
}

void func_8004D99C(s32 arg0) {
    D_80092BF8 = arg0;
}

void func_8004D9AC(s32 arg0) {
}

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

s32 func_8004FA3C(s32 arg0) {
    return 0;
}

s32 func_8004FA4C(void) {
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004FA58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800502AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80050B88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800511E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800516BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051944.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051C54.s")
#else
void func_80051C54(Vec3f* A, Vec3f* B, Vec3f* C, Vec3f* D) {
    f32 sp8;
    f32 sp4;
    f32 sp0;
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_ft0;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 temp_fv1;
    
    temp_fv0 = C->x - A->x;
    temp_fa0 = C->y - A->y;
    temp_ft4 = C->z - A->z;
    sp0 = B->x - A->x;
    sp4 = B->y - A->y;
    sp8 = B->z - A->z;
    
    D->x = (sp4 * temp_ft4) - (sp8 * temp_fa0);
    D->y = -((sp0 * temp_ft4) - (sp8 * temp_fv0));
    D->z = (sp0 * temp_fa0) - (sp4 * temp_fv0);

    // D->x = (((B->y - A->y) * (C->z - A->z)) - ((B->z - A->z) * (C->y - A->y)));
    // D->y = -(((B->x - A->x) * (C->z - A->z)) - ((B->z - A->z) * (C->x - A->x)));
    // D->z = (((B->x - A->x) * (C->y - A->y)) - ((B->y - A->y) * (C->x - A->x)));
}
#endif

s32 func_80051CFC(Vec3f* arg0, Vec3f* arg1) {
    s32 var_v1;
    float product;

    var_v1 = 1;
    if (D_80092BFC != 0) {
        return 1;
    }
    
    //Check for negative dot product?
    product = (arg0->x * arg1->x) + (arg0->y * arg1->y) + (arg0->z * arg1->z);
    if (product < 0.0f) {
        var_v1 = -1;
    }
    return var_v1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051D68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80051F64.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052148.s")

/** 
  * Seems to have something to do with getting the dot product of two vectors?
  */
void func_80052230(Vec3f *A, Vec3f *B, f32 *arg2)
{
    f32 AdotB;
    f32 AdotA;
    f32 BdotB;
    f32 product;
    Vec3f *Acopy;
    
    Acopy = A;
    AdotB = ((A->x * B->x) + (A->y * B->y)) + (A->z * B->z);
    product = (A->x * Acopy->x) + (A->y * A->y) + (A->z * A->z);
    AdotA = product;
    BdotB = ((B->x * B->x) + (B->y * B->y)) + (B->z * B->z);
    product = AdotA * BdotB;
    
    if (product){
        BdotB = sqrtf(product);
    }
    if (BdotB != 0){
        *arg2 = AdotB / BdotB;
        return;
    }
    *arg2 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052300.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80052644.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800528AC.s")
