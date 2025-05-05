#include "common.h"
#include "sys/map.h"

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

extern u32 D_800B96E4; //gBlockIndices_layerArrayEnd?

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
    return layer[gridZ*16 + gridX].mapID;
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
            mapID = cellInfo->mapID;
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

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800451A0.s")

void func_8004530C(void) {
    D_800B979E = 0;
    D_800B9794 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/some_cell_func.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045600.s")
#else
typedef struct {
    /*0x0*/  u8 *data;
    /*0x4*/  s32 byteLength;
    /*0x8*/  s32 bitLength;
    /*0xC*/  s32 capacity;
    /*0x10*/ s32 bitPos;
} BitStream;

s32 bitstream_read(s32, u8);
void bitstream_set_pos(s32, s32, s16, s16);
extern char D_8009A614;
extern s8 *D_800B9700;
extern s16 *D_800B97A0;

s32 func_80045600(s32 arg0, BitStream *stream, s16 arg2, s16 arg3, s16 arg4) {
    s8 bitPosIndex;

    
    bitPosIndex = *(arg2 + (arg3 * 0x10) + &D_800B9700[arg4]);
    
    if (bitPosIndex >= 0) {
        bitstream_set_pos((s32)stream, D_800B97A0[bitPosIndex] + arg0, arg2, arg3);
        return bitstream_read((s32)stream, 1);
    }
    diPrintf(&D_8009A614, arg2, arg3);
    return 0;
}
#endif

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

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_load_streammap_add_to_table.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045DC0.s")

/** free_mapID? */
void func_80045F48(s32 mapID) {
    if (gLoadedMapsDataTable[mapID]){
        func_80045FC4(gLoadedMapsDataTable[mapID], (mapID * 0x8C) + (&D_800B5508), mapID, 1);
        free(gLoadedMapsDataTable[mapID]);
        gLoadedMapsDataTable[mapID] = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80045FC4.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046320.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80046428.s")

void func_80046688(s32 arg0, s32 arg1) {
}

GlobalMapCell* func_80046698(s32 gridX, s32 gridZ) {
    GlobalMapCell *layer;    
    s32 cellIndex;
    
    layer = (GlobalMapCell *)&gDecodedGlobalMap[0];
    cellIndex = (gridZ * 16) + gridX;
    
    return &layer[cellIndex];
}

MapHeader* func_800466C0() {
    MapHeader* map;
    GlobalMapCell *layer;
    s32 mapID;

    layer = gDecodedGlobalMap[0];
    mapID = layer[119].mapID; //There should be 16*16 cells, so why this one specifically... centre cell?
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

#pragma GLOBAL_ASM("asm/nonmatchings/map/init_global_map.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_read_layout.s")

void map_update_streaming();

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

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_update_streaming.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004773C.s")

s32 func_80048024() {
    return D_800B96A8;
}

void func_80048034(void) {
    D_800B96A8 = 0;
    D_800B4A70 = 0;
    D_800B4A72 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048054.s")

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

typedef struct {
/*00*/ u8 unk0;
/*01*/ u8 unk1;
/*02*/ u8 unk2;
/*03*/ u8 unk3;
/*04*/ u8 unk4;
/*05*/ u8 unk5;
/*06*/ u8 unk6;
/*07*/ u8 unk7;
/*08*/ u8 unk8;
/*09*/ u8 unk9;
} MapsUnk_800B97C0;

extern MapsUnk_800B97C0 *D_800B97C0;
extern s16 D_800B97C4;

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

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048E04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80048F58.s")

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
        func_80048C24(block);
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

void* block_load_hits(BlocksModel *block, s32 blockID, u32 unused, HitsLine* hits_ptr) {
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

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_800499BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049B84.s")

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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/map/func_80049D88.s")
#else
void func_8003E648(Texture*, u32, u32);

void func_80049D88() {
    s32 index;
    Texture *tex;
    BlockTexture *blockTex;
    
    for (index = 0; index < 0x100; index++){

        if ((&gBlockTextures[index])->refCount == 0){
            continue;
        }
        
        tex = (&gBlockTextures[index])->texture;
        if (tex && tex->levels != 0x14 && tex->unk_0xe != 0){
            blockTex = &gBlockTextures[index];
            func_8003E648(tex, blockTex->flags, blockTex->unk_0x4);
        }
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/map/block_setup_textures.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/map/block_setup_xz_bitmap.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A5D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004A67C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_update_objects_streaming.s")

s32 func_8004AEFC(s32 mapID, s16 *arg1, s16 searchLimit) {
    s16 searchIndex;

    for (searchIndex = 0; searchIndex < searchLimit; searchIndex++){
        if (mapID == *(arg1 + searchIndex)){
            return 1;
        }
    }
    
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/map/map_should_stream_load_object.s")

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B190.s")

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
void func_8004B710(s32, u32, u32);
s32 map_get_is_object_streaming_disabled();         /* extern */
s32 map_check_some_mapobj_flag(s32, u32);

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

#pragma GLOBAL_ASM("asm/nonmatchings/map/func_8004B984.s")

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
    gDLL_minic->exports->func[4].asVoid();
    gDLL_minic->exports->func[1].asVoid();
    gDLL_8->exports->func[1].asVoid();
    gDLL_7_newday->exports->func1.asVoid();
    gDLL_newclouds->exports->func[1].asVoid();
    gDLL_newstars->exports->func[0].asVoid();
}

/** 
    Called every frame!
    Seems to start a fade-out followed by a warp
*/
void func_8004D328() {
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
