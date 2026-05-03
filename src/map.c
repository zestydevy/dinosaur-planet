#include "sys/map.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/objects/common/vehicle.h"
#include "game/objects/object_id.h"
#include "sys/gfx/texture.h"
#include "sys/asset_thread.h"
#include "sys/bitstream.h"
#include "sys/dl_debug.h"
#include "sys/fs.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/newshadows.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/footsteps.h"
#include "sys/menu.h"
#include "sys/objprint.h"
#include "sys/print.h"
#include "sys/rarezip.h"
#include "sys/segment_1D900.h"
#include "sys/segment_53F00.h"
#include "sys/segment_1050.h"
#include "sys/segment_1460.h"
#include "dll.h"
#include "macros.h"
#include "gbi_extra.h"
#include "prevent_bss_reordering.h"

#define READ_MAPS_TAB(mapID, fileID) ((gFile_MAPS_TAB + (mapID * 7))[fileID])

typedef struct UnkStruct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} UnkStruct;

// -------- .bss start 800b49f0 -------- //
DLBuilder sMainDLBuilder;
DLBuilder sAltDLBuilder; // Used for building DLs other than the main GDL
s32 D_800B4A50;
s32 D_800B4A54;
s8 D_800B4A58; //gStartWarp?
s8 D_800B4A59;
s16 D_800B4A5C;
s16 D_800B4A5E; //gFadeDelayTimerStarted
Warp D_800B4A60;
s16 gMobileMapID;
s16 gMobileMapUnknown;
Plane gFrustumPlanes[MAP_LAYER_COUNT];
u32 gRenderList[MAX_RENDER_LIST_LENGTH];
s16 gRenderListLength;
Block *gBlocksToDraw[MAX_BLOCKS];
s16 gBlocksToDrawIdx;
Gfx* gMainDL;
Mtx* gWorldRSPMatrices;
Vertex* D_800B51D4;
Triangle* D_800B51D8;
s16 SHORT_800b51dc;
s32 UINT_800b51e0;
Camera* D_800B51E4;
struct Vec3_Int Vec3_Int_array[20];
MapHeader* gLoadedMapsDataTable[120];
MapObjSetupList gMapObjSetupLists[120];
s8 gMapType;
MapHeader* gMapActiveStreamMap;
SavedObject* D_800B96B0;
Block **gLoadedBlocks;
u8 gLoadedBlockCount;
s16 *gLoadedBlockIds;
s16 gNumTRKBLKEntries;
u8 *gBlockRefCounts;
s32* gFile_BLOCKS_TAB; // unknown pointer type
s32 gNumTotalBlocks;
/** An array of 5 pointers to 16*16 blockIndex arrays, one for each map layer (maybe called visGrids based on print strings?) */
s8 *gBlockIndices[MAP_LAYER_COUNT];
GlobalMapCell *gDecodedGlobalMap[MAP_LAYER_COUNT]; //16*16 grid of GlobalMapCell structs, one for each layer!
s8 *D_800B9700[MAP_LAYER_COUNT];
s8 *D_800B9714;
u16 *gFile_TRKBLK;
u32 *gFile_HITS_TAB;
s32* gFile_MAPS_TAB; // unknown pointer type
u8 *gMapReadBuffer;
StreamMap gMapStreamMapTable[8];
Struct_D_800B9768 D_800B9768;
BitStream D_800B9780;
u8 D_800B9794;
u8 *D_800B9798;
u8 D_800B979C;
s16 D_800B979E;
s16 *D_800B97A0;
BlockTextureAnim *gBlockTexAnimTable;
BlockTextureScroller *sBlockTexScrollTable;
f32 D_800B97AC; //x
f32 D_800B97B0; //y
f32 D_800B97B4; //z
f32 D_800B97B8;
f32 D_800B97BC;
BlockColorTableEntry *gBlockColorTable; // 255 items
s16 gBlockColorTableLength;
u8 _bss_800b97c8[0x8];
// -------- .bss end 800b97e0 -------- //

// -------- .data start 80092a60 -------- //
s32 D_80092A60 = 0;
s32 D_80092A64 = 0;
s32 gMapCurrentStreamCoordsX = 0;
s32 gMapCurrentStreamCoordsZ = 0;
f32 gWorldX = 0.0f;
f32 gWorldZ = 0.0f;
s8 D_80092A78 = 0; //gFadeDelayTimer
s32 D_80092A7C[2] = {0};
s32 D_80092A84[2] = {0};
s8 gMapLayer = 0;
DLBuilder *gDLBuilder = &sMainDLBuilder;
s32 D_80092A94 = -1;
// See TrackFlags enum
u32 gTrackFlags = 0;
s8 D_80092A9C[MAP_LAYER_COUNT] = {0, -2, -1, 1, 2};
s8 gMapNumStreamMaps = 0;
s32 D_80092AA8 = 0; // unused
f32 D_80092AAC[24] = {
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f
};
s8 D_80092B0C[16] = {
    0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};
s8 D_80092B1C[] = {
    0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 
    0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 
    0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x38, 
    0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 
    0x38, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 
    0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 
    0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 
    0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 
    0x34, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 
    0x00, 0x38, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x34, 0x00, 
    0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x34, 
    0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 
    0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 
    0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 
    0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x34
};
s32 D_80092BBC = -1;
Unk80092BC0 D_80092BC0 = {0};
// -------- .data end 80092bd0 -------- //

void map_handle_transition(void);
void map_restore_saved_objects(MapHeader* arg0, s32 mapID);
HitsLine* block_load_hits(Block *block, s32 blockID, u8 unused, HitsLine* hits_ptr);
void track_sort_render_list(u32* arg0, s32 arg1);
void block_color_table_add_block(Block *block);
void block_color_table_free_block(Block *block);
u32 hits_get_size(s32 id);
void block_setup_vertices(Block *block);
void block_setup_gdl_groups(Block *block);
s32 block_setup_texture_anims(Block *block);
void block_setup_xz_bitmap(Block *block);
void block_compute_vertex_colors(Block*,s32,s32,s32);
void block_texscroll_free(u32 id);
void block_free_texture_anims(Block*);
void block_texscroll_tick(void);
void block_texanim_tick(void);
void track_update_frustum(void);
void block_color_table_tick(void);
void track_draw_main(void);
u8 is_sphere_in_frustum(Vec3f *v, f32 radius);
void map_convert_objpositions_to_ws(MapHeader *map, f32 X, f32 Z);
void map_init_obj_setup_list(MapHeader* map, MapObjSetupList* setupList, s32 mapID, s32 curvesOnly);
MapHeader *map_load_streammap(s32, s32);
void map_read_layout(Struct_D_800B9768_unk4 *arg0, u8 *arg1, s16 arg2, s16 arg3, s32 maptabindex);
void map_update_objects_streaming(s32);
s32 map_func_800485FC(s32, s32, s32, s32, s32);
void map_check_block_grid(s32 gridX, s32 gridZ, s32* arg2, s32* arg3, s32* arg4, s32* arg5, s32 layer, s32 checkVis, s32 streamMapIdx);
void block_free(s32 blockIndex);
s32 block_texanim_add(Texture* tex, u32 renderFlags, s32 animatorID);
s32 map_should_obj_unload(Object*);
void func_8004B548(MapHeader*, s32, s32, Object*);
s32 map_should_stream_load_object(ObjSetup*, s8, s32);
s32 map_check_some_mapobj_flag(s32, u32);
void func_8004B710(s32 cellIndex_plusBitToCheck, u32 mapIndex, u32 arg2);
s32 func_8004AEFC(s32 mapID, s16 *arg1, s16 searchLimit);
s32 func_8004B4A0(ObjSetup* obj, s32 mapno);
void block_add_to_render_list(Block *block, f32 x, f32 z);
void track_draw_object(Object* obj, s32 visibility);
s32 func_80045DC0(s32, s32, s32);
s32 map_find_streammap_index(s32);
s32 map_load_streammap_add_to_table(s32);
s32 block_color_table_add(u8 r, u8 g, u8 b, u8 a);
void block_texanim_free(Texture *tex, s32 animatorID);
void draw_render_list(Mtx *rspMtxs, s8 *visibilities);
void block_calc_shape_visibility(Block*, s16, s16, s16);
void track_add_visible_objects(s8* objVisibilities);
s32 block_frustum_check(s32 xPos, s32 zPos, Block* block);
void some_cell_func(BitStream* stream);
BlockTextureScroller* block_texscroll_get(s32 id);
s32 func_80045600(s32 arg0, BitStream *stream, s16 arg2, s16 arg3, s16 arg4);

void dl_set_all_dirty(void) {
    gDLBuilder->dirtyFlags = DIRTY_FLAGS_ALL;
    gDLBuilder->needsPipeSync = TRUE;
}

void dl_use_alt_builder(void) {
    gDLBuilder = &sAltDLBuilder;
    gDLBuilder->dirtyFlags = DIRTY_FLAGS_ALL;
    gDLBuilder->needsPipeSync = TRUE;
}

void dl_use_main_builder(void) {
    gDLBuilder = &sMainDLBuilder;
}

void dl_apply_combine(Gfx **gdl) {
    Gfx *currGfx = &gDLBuilder->combine;
    u8 dirty;

    if (gDLBuilder->dirtyFlags & DIRTY_FLAGS_COMBINE) {
        gDLBuilder->dirtyFlags &= ~DIRTY_FLAGS_COMBINE;
        dirty = TRUE;
    } else {
        dirty = currGfx->words.w0 != (*gdl)->words.w0 || currGfx->words.w1 != (*gdl)->words.w1;
    }

    if (dirty) {
        currGfx->words.w0 = (*gdl)->words.w0;
        currGfx->words.w1 = (*gdl)->words.w1;

        if (gDLBuilder->needsPipeSync) {
            gDLBuilder->needsPipeSync = FALSE;

            gDPPipeSync((*gdl)++);

            (*gdl)->words.w0 = (currGfx)->words.w0;
            (*gdl)->words.w1 = (currGfx)->words.w1;
        }

        (*gdl)++;
    }
}

void dl_apply_other_mode(Gfx **gdl) {
    Gfx *currGfx;
    u8 dirty;

    if (gTrackFlags & TRACKFLAG_DISABLE_Z_BUFFER) {
        (*gdl)->words.w1 &= ~(Z_CMP | Z_UPD);
    }

    currGfx = &gDLBuilder->combine + 1;
    if (gDLBuilder->dirtyFlags & DIRTY_FLAGS_OTHER_MODE) {
        gDLBuilder->dirtyFlags &= ~DIRTY_FLAGS_OTHER_MODE;
        dirty = TRUE;
    } else {
        dirty = currGfx->words.w0 != (*gdl)->words.w0 || currGfx->words.w1 != (*gdl)->words.w1;
    }

    if (dirty) {
        currGfx->words.w0 = (*gdl)->words.w0;
        currGfx->words.w1 = (*gdl)->words.w1;

        if (gDLBuilder->needsPipeSync) {
            gDLBuilder->needsPipeSync = FALSE;

            gDPPipeSync((*gdl)++);

            (*gdl)->words.w0 = (currGfx)->words.w0;
            (*gdl)->words.w1 = (currGfx)->words.w1;
        }

        (*gdl)++;
    }
}

void dl_apply_geometry_mode(Gfx **gdl) {
    u8 dirty;

    if (gTrackFlags & TRACKFLAG_DISABLE_Z_BUFFER) {
        (**gdl).words.w1 &= ~G_ZBUFFER;
    }

    if (gDLBuilder->dirtyFlags & DIRTY_FLAGS_GEOMETRY_MODE) {
        gDLBuilder->dirtyFlags &= ~DIRTY_FLAGS_GEOMETRY_MODE;
        dirty = TRUE;
    } else {
        dirty = gDLBuilder->geometryMode != (**gdl).words.w1;
    }

    if (dirty) {
        gDLBuilder->geometryMode = (**gdl).words.w1;
        (*gdl)++;
    }
}

void dl_set_geometry_mode(Gfx **gdl, u32 mode) {
    mode |= gDLBuilder->geometryMode;
    gSPLoadGeometryMode(*gdl, mode);
    dl_apply_geometry_mode(gdl);
}

void dl_clear_geometry_mode(Gfx **gdl, u32 mode) {
    mode = gDLBuilder->geometryMode & ~mode;
    gSPLoadGeometryMode(*gdl, mode);
    dl_apply_geometry_mode(gdl);
}

void dl_set_prim_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a) {
    u32 rgba = RGBA8(r, g, b, a);
    u8 dirty;

    if (gDLBuilder->dirtyFlags & DIRTY_FLAGS_SET_PRIMARY_COLOR)
    {
        gDLBuilder->dirtyFlags &= ~DIRTY_FLAGS_SET_PRIMARY_COLOR;
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

void dl_set_prim_color_no_sync(Gfx **gdl, u8 r, u8 g, u8 b, u8 a) {
    gDLBuilder->dirtyFlags |= DIRTY_FLAGS_SET_PRIMARY_COLOR;
    gDLBuilder->needsPipeSync = FALSE;
    dl_set_prim_color(gdl, r, g, b, a);
}

void dl_set_env_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a) {
    u32 rgba = RGBA8(r, g, b, a);
    u8 dirty;

    if (gDLBuilder->dirtyFlags & DIRTY_FLAGS_SET_ENV_COLOR)
    {
        gDLBuilder->dirtyFlags &= ~DIRTY_FLAGS_SET_ENV_COLOR;
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

void dl_set_env_color_no_sync(Gfx **gdl, u8 r, u8 g, u8 b, u8 a) {
    gDLBuilder->dirtyFlags |= DIRTY_FLAGS_SET_ENV_COLOR;
    gDLBuilder->needsPipeSync = FALSE;
    dl_set_env_color(gdl, r, g, b, a);
}

void dl_set_blend_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a) {
    u32 rgba = RGBA8(r, g, b, a);
    u8 dirty;

    if (gDLBuilder->dirtyFlags & DIRTY_FLAGS_BLEND_MODE)
    {
        gDLBuilder->dirtyFlags &= ~DIRTY_FLAGS_BLEND_MODE;
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

void dl_set_fill_color(Gfx **gdl, u32 color) {
    u8 dirty;

    if (gDLBuilder->dirtyFlags & DIRTY_FLAGS_SET_FILL_COLOR)
    {
        gDLBuilder->dirtyFlags &= ~DIRTY_FLAGS_SET_FILL_COLOR;
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

void dl_set_fog_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a) {
    u32 rgba = RGBA8(r, g, b, a);
    u8 dirty;

    if (gDLBuilder->dirtyFlags & DIRTY_FLAGS_SET_FOG_COLOR)
    {
        gDLBuilder->dirtyFlags &= ~DIRTY_FLAGS_SET_FOG_COLOR;
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

void dl_triangles(Gfx **gdl, DLTri *tris, s32 triCount) {
    s32 n;
    DLTri *tri;

    for (n = triCount >> 1; n != 0; n--, tris += 2)
    {
        tri = tris;
        gSP2Triangles2((*gdl)++, tri[0].v0, tri[0].v1, tri[0].v2, 0, tri[1].v0, tri[1].v1, tri[1].v2, 0);
    }

    if (triCount & 1) {
        tri = tris;
        gSP1Triangle((*gdl)++, tri[0].v0, tri[0].v1, tri[0].v2, 0);
    }

    gDLBuilder->needsPipeSync = TRUE;
}

void track_func_80041C30(s32 on) {
    if (on != 0) {
        gTrackFlags |= TRACKFLAG_UNK1000;
    } else {
        gTrackFlags &= ~TRACKFLAG_UNK1000;
    }
}

void track_set_sky_on(s32 on) {
    if (on != 0) {
        gTrackFlags |= TRACKFLAG_SKY;
    } else {
        gTrackFlags &= ~TRACKFLAG_SKY;
    }
}

void track_set_anti_alias_on(s32 on) {
    if (on != 0) {
        gTrackFlags |= TRACKFLAG_ANTI_ALIAS;
    } else {
        gTrackFlags &= ~TRACKFLAG_ANTI_ALIAS;
    }
}

void track_set_sky_objects_on(s32 on) {
    if (on != 0) {
        gTrackFlags |= TRACKFLAG_SKY_OBJECTS;
    } else {
        gTrackFlags &= ~TRACKFLAG_SKY_OBJECTS;
    }
}

void track_set_z_buffer_on(s32 on) {
    if (on != 0) {
        gTrackFlags &= ~TRACKFLAG_DISABLE_Z_BUFFER;
    } else {
        gTrackFlags |= TRACKFLAG_DISABLE_Z_BUFFER;
    }
}

s32 track_is_z_buffer_on(void) {
    return (gTrackFlags & TRACKFLAG_DISABLE_Z_BUFFER) == FALSE;
}

u32 track_is_sky_on(void) {
    return gTrackFlags & TRACKFLAG_SKY;
}

u32 track_func_80041D8C(void) {
    return gTrackFlags & TRACKFLAG_UNK100;
}

u32 track_get_shadows_on(void) {
    return gTrackFlags & TRACKFLAG_SHADOWS;
}

s32 track_func_80041DBC(void) {
    return gTrackFlags & TRACKFLAG_UNK8;
}

s32 track_func_80041DD4(void) {
    return gTrackFlags & TRACKFLAG_UNK1000;
}

void track_func_80041DEC(void) {
    gTrackFlags |= TRACKFLAG_UPDATE_STREAMING_IMMEDIATE;
}

/* 
 * Get a different widescreen-related bit? It can stack with the other one!
 * Maybe related to the unimplemented cinematic aspect vs. widescreen option
 * in the video settings, like in GE/PD?
 */
s32 track_func_80041E08(void) {
    return gTrackFlags & TRACKFLAG_UNK10000;
}

void track_set_sun_glare_on(s32 on) {
    if (on != 0) {
        gTrackFlags |= TRACKFLAG_SUN_GLARE;
    } else {
        gTrackFlags &= ~TRACKFLAG_SUN_GLARE;
    }
}

s32 track_get_sun_glare_on(void) {
    return gTrackFlags & TRACKFLAG_SUN_GLARE;
}

void init_maps(void) {
    s32 i;

    gTrackFlags = 0;
    gBlockColorTable = mmAlloc(sizeof(BlockColorTableEntry) * 255, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:cblocks"));
    gLoadedBlocks = mmAlloc(sizeof(Block*) * MAX_BLOCKS, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:blknos"));
    gLoadedBlockIds = mmAlloc(sizeof(s16) * MAX_BLOCKS, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:blkusage"));
    gBlockRefCounts = mmAlloc(sizeof(u8) * MAX_BLOCKS, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:mapinfo"));
    gMapReadBuffer = mmAlloc(sizeof(u8) * 700, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:tempbuf"));
    *gBlockIndices = mmAlloc(BLOCKS_GRID_TOTAL_CELLS * MAP_LAYER_COUNT, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:blkmaps"));
    *gDecodedGlobalMap = mmAlloc(sizeof(GlobalMapCell) * 1280, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:entrymaps"));
    *D_800B9700 = mmAlloc(BLOCKS_GRID_TOTAL_CELLS * MAP_LAYER_COUNT, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:vismap"));
    for (i = 1; i < MAP_LAYER_COUNT; i++) {
        gBlockIndices[i] = gBlockIndices[i - 1] + BLOCKS_GRID_TOTAL_CELLS;
        gDecodedGlobalMap[i] = gDecodedGlobalMap[i - 1] + BLOCKS_GRID_TOTAL_CELLS;
        D_800B9700[i] = D_800B9700[i - 1] + BLOCKS_GRID_TOTAL_CELLS;
    }
    queue_alloc_load_file((void **) &gFile_MAPS_TAB, MAPS_TAB);
    queue_alloc_load_file((void** ) &gFile_HITS_TAB, HITS_TAB);
    for (i = 0; i < 120; i++) { gLoadedMapsDataTable[i] = NULL; }
    queue_alloc_load_file((void** ) &gFile_TRKBLK, TRKBLK_BIN);
    gNumTRKBLKEntries = 0;
    while (gFile_TRKBLK[gNumTRKBLKEntries] != 0xFFFF) {
        gNumTRKBLKEntries++;
    }
    gNumTRKBLKEntries--;
    queue_alloc_load_file((void **) &gFile_BLOCKS_TAB, BLOCKS_TAB);
    gNumTotalBlocks = 0;
    while (gFile_BLOCKS_TAB[gNumTotalBlocks] != -1) {
        gNumTotalBlocks++;
    }
    gNumTotalBlocks--;
    D_800B96B0 = mmAlloc(sizeof(SavedObject) * 100, ALLOC_TAG_TRACK_COL, ALLOC_NAME("objdef_store"));
    D_800B4A5C = -1;
    D_800B4A5E = -2;
    gBlockTexAnimTable = mmAlloc(sizeof(BlockTextureAnim) * MAX_TEXTURE_ANIMS, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:texanim"));
    bzero(gBlockTexAnimTable, sizeof(BlockTextureAnim) * MAX_TEXTURE_ANIMS);
    sBlockTexScrollTable = mmAlloc(sizeof(BlockTextureScroller) * MAX_TEXTURE_SCROLLERS, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:texscroll"));
    bzero(sBlockTexScrollTable, sizeof(BlockTextureScroller) * MAX_TEXTURE_SCROLLERS);
    bzero(gRenderList, sizeof(u32) * MAX_RENDER_LIST_LENGTH);
    gRenderList[0] = -0x4000;
}

void track_tick(s32 arg0) {
    if (arg0 == 0) {
        if (gTrackFlags & TRACKFLAG_SKY) {
            gDLL_12_Minic->vtbl->func2();
        }
        gDLL_8->vtbl->func2();
        gDLL_7_Newday->vtbl->func2();
        gDLL_9_Newclouds->vtbl->func3();
        block_texanim_tick();
        block_texscroll_tick();
        map_handle_transition();
        if (gDLL_76 != NULL) {
            gDLL_76->vtbl->func1();
        }
        gDLL_59_Minimap->vtbl->func0();
    }
    func_8001EB80();
}

void track_draw(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, Vertex** vtxs2, Triangle** pols2) {
    Mtx* mtx;

    gMainDL = *gdl;
    gWorldRSPMatrices = *mtxs;
    D_800B51D4 = *vtxs;
    D_800B51D8 = *pols;
    gTrackFlags |= (TRACKFLAG_ANTI_ALIAS | TRACKFLAG_UNK1);
    if ((gMapType == MAPTYPE_MOBILE) || (gMapType == MAPTYPE_3)) {
        gTrackFlags &= ~TRACKFLAG_UNK1;
    }
    gSPTexture(gMainDL++, -1, -1, 3, 0, 1);
    mtx = get_some_model_view_mtx();
    gSPMatrix(gMainDL++, OS_K0_TO_PHYSICAL(mtx), G_MTX_MODELVIEW | G_MTX_LOAD);
    camera_setup_viewport_and_matrices(&gMainDL, 0);
    track_update_frustum();
    if (func_80010048() != 0) {
        if (!(gTrackFlags & TRACKFLAG_UNK8)) {
            gTrackFlags |= TRACKFLAG_UNK8;
        }
        camera_set_aspect(1.7777778f);
    } else {
        if (gTrackFlags & TRACKFLAG_UNK8) {
            gTrackFlags &= ~TRACKFLAG_UNK8;
            camera_set_aspect(1.3333334f);
        }
    }
    if (gTrackFlags & TRACKFLAG_UNK10000) {
        if (gTrackFlags & TRACKFLAG_UNK8) {
            camera_set_aspect(1.7777778f);
        } else {
            camera_set_aspect(1.3333334f);
        }
        viewport_disable(get_camera_selector(), 0U);
        vi_some_video_setup(0);
        gTrackFlags &= ~TRACKFLAG_UNK10000;
    }
    if (gTrackFlags & TRACKFLAG_SKY) {
        setup_rsp_camera_matrices(&gMainDL, &gWorldRSPMatrices);
        gDLL_7_Newday->vtbl->func13(&gMainDL, &gWorldRSPMatrices);

        if (gTrackFlags & TRACKFLAG_SKY_OBJECTS) {
            gDLL_10_Newstars->vtbl->func1(&gMainDL);
        }
        gDLL_7_Newday->vtbl->func3(&gMainDL, &gWorldRSPMatrices, gTrackFlags & TRACKFLAG_SKY_OBJECTS);
    } else {
        setup_rsp_camera_matrices(&gMainDL, &gWorldRSPMatrices);
    }
    gDLL_11_Newlfx->vtbl->func2();
    gDLL_57->vtbl->func3();
    gDLL_58->vtbl->func2();
    if (gTrackFlags & TRACKFLAG_SUN_GLARE) {
        if (gDLL_7_Newday->vtbl->func23(&gMainDL) == 0) {
            gDLL_8->vtbl->func3(&gMainDL);
        }
    } else {
        gDLL_8->vtbl->func3(&gMainDL);
    }
    D_800B51E4 = get_camera();
    block_color_table_tick();
    track_draw_main();
    gDLL_9_Newclouds->vtbl->func4(&gMainDL);
    camera_setup_fullscreen_viewport(&gMainDL);
    *gdl = gMainDL;
    *mtxs = gWorldRSPMatrices;
    *vtxs = D_800B51D4;
    *pols = D_800B51D8;
    gTrackFlags &= ~TRACKFLAG_UPDATE_STREAMING;
    // @fake
    if (1) { } if (1) { } if (1) { } if (1) { }
    // diProfEnd("Trackdraw") (default.dol)
}

void track_draw_main(void) {
    s32 xIdx;
    Block* block;
    s32 gridIdx;
    s8 *blockIdxMap;
    s32 blockIdx;
    s32 sp274[4];
    s32 sp264[4];
    s32 sp254[4];
    s32 sp244[4];
    s32 layer;
    s32 z;
    s32 zIdx;
    s32 x;
    s8* sp230;
    u8 blockVisibilities[BLOCKS_GRID_TOTAL_CELLS];
    s8 objVisibilities[MAX_VISIBLE_OBJECTS];
    Mtx* rspMtxs;

    dl_add_debug_info(gMainDL, 0, "track/track.c", 1323);
    some_cell_func(&D_800B9780);
    shadows_func_8004D9B8();
    shadows_func_8004DABC();
    gRenderListLength = 1;
    gBlocksToDrawIdx = 0;
    dl_add_debug_info(gMainDL, 0, "track/track.c", 1341);
    gDLL_9_Newclouds->vtbl->func6(&gMainDL, gUpdateRate, 0);
    gDLL_15_Projgfx->vtbl->func5(&gMainDL, &gWorldRSPMatrices, &D_800B51D4, 3);
    if (gTrackFlags & TRACKFLAG_SKY) {
        gDLL_12_Minic->vtbl->func3(&gMainDL, &gWorldRSPMatrices);
    }
    dl_add_debug_info(gMainDL, 0, "track/track.c", 1349);
    blockIdxMap = D_80092B0C;
    rspMtxs = gWorldRSPMatrices;
    layer = MAP_LAYER_COUNT;
    while (--layer >= 0) {
        // Determine which blocks are visible from the current stream coords
        sp230 = gBlockIndices[layer];
        D_800B9714 = D_800B9700[layer];
        map_check_block_grid(gMapCurrentStreamCoordsX + 7, gMapCurrentStreamCoordsZ + 7, 
            sp274, sp264, sp254, sp244, layer, /*checkVis*/TRUE, D_800B4A54);
        for (gridIdx = 0; gridIdx < ARRAYCOUNT_S(blockVisibilities); gridIdx++) { blockVisibilities[gridIdx] = 0; }
        
        for (z = sp274[2]; sp274[3] >= z; z++) {
            for (x = sp274[0]; sp274[1] >= x; x++) {
                blockVisibilities[(x + 7) + ((z + 7) << 4)] = 1;
            }
        }
        for (z = sp264[2]; sp264[3] >= z; z++) {
            for (x = sp264[0]; sp264[1] >= x; x++) {
                blockVisibilities[(x + 7) + ((z + 7) << 4)] = 1;
            }
        }
        for (z = sp254[2]; sp254[3] >= z; z++) {
            for (x = sp254[0]; sp254[1] >= x; x++) {
                blockVisibilities[(x + 7) + ((z + 7) << 4)] = 1;
            }
        }
        for (z = sp244[2]; sp244[3] >= z; z++) {
            for (x = sp244[0]; sp244[1] >= x; x++) {
                blockVisibilities[(x + 7) + ((z + 7) << 4)] = 1;
            }
        }
        // @fake
        if (layer){}

        // Add visible blocks to render list
        for (xIdx = 0; xIdx < BLOCKS_GRID_SPAN; xIdx++) {
            x = blockIdxMap[xIdx];
            for (zIdx = 0; zIdx < BLOCKS_GRID_SPAN; zIdx++) {
                z = blockIdxMap[zIdx];
                gridIdx = GRID_INDEX(z, x);
                blockIdx = sp230[gridIdx];
                if (blockIdx < 0) {
                    block = NULL;
                } else {
                    block = gLoadedBlocks[blockIdx];
                    block->vtxFlags ^= 1;
                    if (blockVisibilities[gridIdx] == 0) {
                        // Block is not visible according to visgrid
                        continue;
                    }
                }
                if (blockIdx < 0 || block_frustum_check(x, z, block) == FALSE) {
                    // Block not loaded or failed frustum check
                    continue;
                }
                // Calculate visible shapes/triangles
                D_800B97B8 = x * BLOCKS_GRID_UNIT_F;
                D_800B97BC = z * BLOCKS_GRID_UNIT_F;
                block_calc_shape_visibility(block, x, z, layer);
                // Update block lighting
                if (gTrackFlags & TRACKFLAG_BLOCK_LIGHTING) {
                    if (block->unk3E != 0) {
                        block_compute_vertex_colors(block, x, z, 0);
                    }
                    if ((block->numSphereMappedShapes != 0) && (gTrackFlags & TRACKFLAG_UNK100)) {
                        func_8001F4C0(block, x, z);
                    }
                }
                // Add to render list
                block_add_to_render_list(block, D_800B97B8, D_800B97BC);
            }
        }
    }
    // Add visible objects to render list
    track_add_visible_objects(objVisibilities);
    // Draw blocks and objects
    draw_render_list(rspMtxs, objVisibilities);
    dl_add_debug_info(gMainDL, 0, "track/track.c", 1458);
    gDLL_15_Projgfx->vtbl->func5(&gMainDL, &gWorldRSPMatrices, &D_800B51D4, 2);
    gDLL_15_Projgfx->vtbl->func5(&gMainDL, &gWorldRSPMatrices, &D_800B51D4, 1);
    gDLL_14_Modgfx->vtbl->func11(objVisibilities);
    gDLL_14_Modgfx->vtbl->func6(&gMainDL, &gWorldRSPMatrices, &D_800B51D4, 0, 0);
    gDLL_24_Waterfx->vtbl->func_C7C(&gMainDL, &gWorldRSPMatrices);
    gDLL_15_Projgfx->vtbl->func5(&gMainDL, &gWorldRSPMatrices, &D_800B51D4, 0);
    gDLL_2_Camera->vtbl->lock_icon_print(&gMainDL, &gWorldRSPMatrices, &D_800B51D4, &D_800B51D8);
    gDLL_59_Minimap->vtbl->func1(&gMainDL, &gWorldRSPMatrices);
    shadows_func_8004D974(0);
    D_800B1847 = 0;
    dl_add_debug_info(gMainDL, 0, "track/track.c", 1478);
}

void draw_render_list(Mtx* rspMtxs, s8* visibilities) {
    BlockShape* shape;
    Vtx_t *tempVtx;
    s32 shapeIdx;
    BlockTextureScroller* texScroller;
    s32 i;
    BlockTextureAnimInstance* temp_v0_4;
    u32 forceTexSet;
    s32 spE0;
    s32 spDC;
    s32 spD8;
    s32 spD4;
    s32 spD0;
    s32 spCC;
    EncodedTri* temp_a1;
    s32 lastBlockIdx;
    EncodedTri* var_a0;
    EncodedTri* var_s2;
    Gfx* temp_s5;
    Texture* tex1;
    Texture* tex0;
    s32 temp_s0_2;
    s32 blockIdx;
    Mtx* blockMtxList;
    s8 lastBlockMtx;
    s8 temp2;
    s32 idx;
    s32 renderFlags;
    s32 frameOptions;
    Block* block;
    Object** objList;
    Object *obj;

    lastBlockIdx = -1;
    objList = get_world_objects(NULL, NULL);
    gDLL_57->vtbl->func2(&spE0, &spDC, &spD8, &spD4, &spD0, &spCC);
    for (i = 1; i < gRenderListLength; i++) {
        idx = shapeIdx = (gRenderList[i] & 0x3F80) >> 7;
        if (gRenderList[i] & 0x40) {
            obj = objList[idx];
            track_draw_object(obj, visibilities[idx]);
            lastBlockMtx = 0;
        } else {
            // @fake
            if (i) {}
            blockIdx = gRenderList[i] & 0x3F;
            forceTexSet = FALSE;
            if (blockIdx != lastBlockIdx) {
                lastBlockMtx = -1;
                SHORT_800b51dc = -1;
                lastBlockIdx = blockIdx;
                UINT_800b51e0 = TEX_FRAME(0);
                blockMtxList = (blockIdx * 2) + rspMtxs;
                block = gBlocksToDraw[blockIdx];
            }
            shape = &block->shapes[idx];
            if (shape->flags & RENDER_UNK20000000) {
                if (lastBlockMtx != 2) {
                    gSPMatrix(gMainDL++, OS_K0_TO_PHYSICAL(&blockMtxList[1]), G_MTX_MODELVIEW | G_MTX_LOAD);
                    lastBlockMtx = 2;
                }
            } else {
                if (lastBlockMtx != 1) {
                    gSPMatrix(gMainDL++, OS_K0_TO_PHYSICAL(&blockMtxList[0]), G_MTX_MODELVIEW | G_MTX_LOAD);
                    lastBlockMtx = 1;
                }
            }
            if (shape->materialIndex == 0xFF) {
                tex0 = NULL;
            } else {
                tex0 = block->materials[shape->materialIndex].texture;
            }
            if (shape->flags & RENDER_UNK2000) {
                if (tex0->flags & (RENDER_COMPOSITE_BASE | RENDER_COMPOSITE_OVERLAY)) {
                    dl_set_prim_color(&gMainDL, 0xFF, 0xFF, 0xFF, 0xA0);
                } else {
                    dl_set_prim_color(&gMainDL, 0xFF, 0xFF, 0xFF, 0x64);
                }
            } else {
                if (shape->envColourMode == 0xFF) {
                    dl_set_prim_color(&gMainDL, spE0, spDC, spD8, 0xFF);
                } else if (shape->envColourMode == 0xFE) {
                    dl_set_prim_color(&gMainDL, 0xFF, 0xFF, 0xFF, 0xFF);
                } else if (shape->flags & (RENDER_UNK40000000 | RENDER_UNK4000000 | RENDER_UNK2000000 | RENDER_UNK800000 | RENDER_UNK400000)) {
                    dl_set_prim_color(&gMainDL, 0xFF, 0xFF, 0xFF, 0xFF);
                } else {
                    func_8001F848(&gMainDL);
                }
            }
            renderFlags = shape->flags;
            if (renderFlags & RENDER_SHAPE_ANIMATED) {
                temp_v0_4 = block_texanim_get_instance(block, shape->animatorID);
                if (temp_v0_4 != NULL) {
                    frameOptions = gBlockTexAnimTable[temp_v0_4->texanimID].unk4 << 8;
                    renderFlags |= gBlockTexAnimTable[temp_v0_4->texanimID].flags;
                } else {
                    frameOptions = 0;
                }
                if ((shape->animatorID != SHORT_800b51dc) || (frameOptions != UINT_800b51e0)) {
                    SHORT_800b51dc = shape->animatorID;
                    UINT_800b51e0 = frameOptions;
                    forceTexSet = TRUE;
                }
            } else {
                SHORT_800b51dc = -1;
                frameOptions = 0;
            }
            if (shape->blendMaterialIndex != 0xFF) {
                tex1 = block->materials[shape->blendMaterialIndex].texture;
            } else {
                tex1 = NULL;
            }
            tex_gdl_set_textures(&gMainDL, tex0, tex1, renderFlags, frameOptions, forceTexSet, FALSE);
            if (shape->texScrollerID != 0xFF) {
                texScroller = block_texscroll_get(shape->texScrollerID);
                gDPSetTileSize(gMainDL++, 0, texScroller->uOffsetA, texScroller->vOffsetA, (tex0->width - 1) << 2, (tex0->height - 1) << 2);
                if (tex1 != NULL) {
                    gDPSetTileSize(gMainDL++, 1, texScroller->uOffsetB, texScroller->vOffsetB, (tex1->width - 1) << 2, (tex1->height - 1) << 2);
                }
            } else if ((tex0 != NULL) && (tex0->flags & (RENDER_COMPOSITE_BASE | RENDER_COMPOSITE_OVERLAY))) {
                gDPSetTileSize(gMainDL++, 0, 0, 0, (tex0->width - 1) << 2, (tex0->height - 1) << 2);
                if (tex1 != NULL) {
                    gDPSetTileSize(gMainDL++, 1, 0, 0, (tex1->width - 1) << 2, (tex1->height - 1) << 2);
                }
            }
            shapeIdx = (shape - block->shapes) * 3;
            gMainDL->words.w0 = block->gdlGroups[shapeIdx].words.w0;
            gMainDL->words.w1 = block->gdlGroups[shapeIdx].words.w1;
            shapeIdx++;
            dl_apply_geometry_mode(&gMainDL);
            gMainDL->words.w0 = block->gdlGroups[shapeIdx].words.w0;
            gMainDL->words.w1 = block->gdlGroups[shapeIdx].words.w1;
            shapeIdx++;
            dl_apply_combine(&gMainDL);
            gMainDL->words.w0 = block->gdlGroups[shapeIdx].words.w0;
            gMainDL->words.w1 = block->gdlGroups[shapeIdx].words.w1;
            dl_apply_other_mode(&gMainDL);
            tempVtx = block->vertices2[(block->vtxFlags & 1) ^ 1];
            var_a0 = block->encodedTris;
            var_a0 += shape->triBase;
            temp_a1 = block->encodedTris;
            temp_a1 += shape[1].triBase;
            temp_s5 = gMainDL;

            gSPVertex2(gMainDL++, OS_K0_TO_PHYSICAL(&tempVtx[shape->vtxBase]), shape[1].vtxBase - shape->vtxBase, 0);

            var_s2 = NULL;
            while ((u32) var_a0 < (u32) temp_a1) {
                if (var_a0->d1 & 1) {
                    if (var_s2 == NULL) {
                        var_s2 = var_a0;
                    } else {
                        gSP2TrianglesBlock(gMainDL, var_s2->d0, var_a0->d0);
                        gMainDL++;
                        var_s2 = NULL;
                    }
                }
                var_a0++;
            }
            if ((var_s2 != NULL) && (var_s2->d1 & 1)) {
                gSP1TriangleBlock(gMainDL, var_s2->d0);
                gMainDL++;
            }
            gDLBuilder->needsPipeSync = 1;
            if ((renderFlags & (RENDER_FOG_ACTIVE | RENDER_DECAL_SIMPLE | RENDER_DECAL)) == (RENDER_FOG_ACTIVE | RENDER_DECAL_SIMPLE | RENDER_DECAL)) {
                temp_s0_2 = gMainDL - temp_s5;
                dl_set_geometry_mode(&gMainDL, G_FOG);
                if (renderFlags & (RENDER_UNK2000 | RENDER_SEMI_TRANSPARENT)) {
                    gDPSetCombineLERP(gMainDL, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1);
                    dl_apply_combine(&gMainDL);
                    gDPSetOtherMode(
                        gMainDL, 
                        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
                        G_AC_NONE | G_ZS_PIXEL | Z_CMP | IM_RD | CVG_DST_FULL | ZMODE_XLU | FORCE_BL | GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_MEM, G_BL_1MA) | GBL_c2(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_MEM, G_BL_1MA)
                    );
                    dl_apply_other_mode(&gMainDL);
                } else {
                    gDPSetCombineLERP(gMainDL, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1);
                    dl_apply_combine(&gMainDL);
                    
                    gDPSetOtherMode(
                        gMainDL, 
                        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
                        G_AC_NONE | G_ZS_PIXEL | AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_DEC | FORCE_BL | GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_MEM, G_BL_1MA) | GBL_c2(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_MEM, G_BL_1MA)
                    );
                    dl_apply_other_mode(&gMainDL);
                }
                bcopy(temp_s5, gMainDL, temp_s0_2 * sizeof(Gfx));
                gMainDL += temp_s0_2;
                gDLBuilder->needsPipeSync = 1;
            }
        }
    }
}

void track_draw_object(Object* obj, s32 visibility) {
    s8 sp37;
    u8 someBool;

    someBool = TRUE;
    if ((obj->id == OBJ_IMSnowBike) || (obj->id == OBJ_CRSnowBike)) {
        someBool = TRUE;
        if (((DLL_IVehicle*)obj->dll)->vtbl->func13(obj) != 0) {
            someBool = FALSE;
        }
    }
    // @bug: sp37 is uninitialized if someBool is false
#ifdef AVOID_UB
    sp37 = 0; // TODO: is this an ok default?
#endif
    if (someBool != FALSE) {
        sp37 = gDLL_13_Expgfx->vtbl->func10(obj);
    }
    gDLL_14_Modgfx->vtbl->func6(&gMainDL, &gWorldRSPMatrices, &D_800B51D4, 1, obj);
    if (sp37 >= 2) {
        if ((obj->id != OBJ_IMSnowBike) && (obj->id != OBJ_CRSnowBike)) {
            gDLL_13_Expgfx->vtbl->func6(obj, &gMainDL, &gWorldRSPMatrices, &D_800B51D4, 1, 0, 0);
        }
    }
    objprint_func(&gMainDL, &gWorldRSPMatrices, &D_800B51D4, &D_800B51D8, obj, visibility);
    if (sp37 != 0) {
        if ((obj->id != OBJ_IMSnowBike) && (obj->id != OBJ_CRSnowBike)) {
            gDLL_13_Expgfx->vtbl->func6(obj, &gMainDL, &gWorldRSPMatrices, &D_800B51D4, 0, 0, 0);
        }
    }
    if ((obj->linkedObject != NULL) && (visibility != 0)) {
        sp37 = gDLL_13_Expgfx->vtbl->func10(obj->linkedObject);
        if (sp37 >= 2) {
            gDLL_13_Expgfx->vtbl->func6(obj->linkedObject, &gMainDL, &gWorldRSPMatrices, &D_800B51D4, 1, 0, 0);
        }
        if (sp37 != 0) {
            gDLL_13_Expgfx->vtbl->func6(obj->linkedObject, &gMainDL, &gWorldRSPMatrices, &D_800B51D4, 0, 0, 0);
        }
    }
}

/** Calculates frustum culling for a Block's shapes, and for each shape's faces. */
void block_calc_shape_visibility(Block* block, s16 arg1, s16 arg2, s16 arg3) {
    BlockShape* shape;
    EncodedTri* triEnd;
    EncodedTri* tri;
    f32 pad[5];
    f32 Xmax;
    f32 Fx;
    f32 Fy;
    f32 Fz;
    f32 V2z;
    BlockShape* shapesEnd;
    f32 V1x;
    f32 Xmin;
    f32 Ymin;
    f32 Zmin;
    f32 V1y;
    f32 Ymax;
    f32 Zmax;
    f32 V2y;
    f32 V2x;
    f32 V1z;
    s32 Wx;
    s32 Wy;
    s32 Wz;
    s32 visible;
    u32 i;
    f32 dotProduct;
    f32 Fd;
    BlockVertex  *shapeVtx;

    Wx = (D_800B51E4->tx - gWorldX) - D_800B97B8;
    Wy = D_800B51E4->ty;
    Wz = (D_800B51E4->tz - gWorldZ) - D_800B97BC;

    //Shape-level culling (based on bounds)
    shape = block->shapes;
    shapesEnd = &block->shapes[block->shapeCount];
    while ((u32) shape < (u32) shapesEnd) {
        //Check shape flags
        if (shape->flags & RENDER_SHAPE_HIDE) {
            shape->flags &= ~RENDER_SHAPE_VISIBLE;
            shape++;
            continue;
        }

        if ((D_800B9794 != 0) && 
            ((D_800B979C & 1) || !(shape->flags & (RENDER_UNK2000 | RENDER_DECAL_SIMPLE | RENDER_SEMI_TRANSPARENT))) && 
            (func_80045600((shape - block->shapes), &D_800B9780, arg1, arg2, arg3) == 0)
        ) {
            shape->flags &= ~RENDER_SHAPE_VISIBLE;
            shape++;
            continue;
        }

        visible = TRUE;

        //Get shape's bounding box
        Xmax = ((shape->Xmax * 4) | SHAPE_BB_REMAINDER_X_MAX(shape)) + D_800B97B8;
        Xmin = ((shape->Xmin * 4) | SHAPE_BB_REMAINDER_X_MIN(shape)) + D_800B97B8;
        Ymin = shape->Ymin;
        Ymax = shape->Ymax;
        Zmax = ((shape->Zmax * 4) | SHAPE_BB_REMAINDER_Z_MAX(shape)) + D_800B97BC;
        Zmin = ((shape->Zmin * 4) | SHAPE_BB_REMAINDER_Z_MIN(shape)) + D_800B97BC;

        for (i = 0; i < ARRAYCOUNT_S(gFrustumPlanes); i++) {
            if (gFrustumPlanes[i].unk14[0] & 1) {
                V1x = Xmax;
                V2x = Xmin;
            } else {
                V1x = Xmin;
                V2x = Xmax;
            }

            if (gFrustumPlanes[i].unk14[0] & 2) {
                V1y = Ymax;
                V2y = Ymin;
            } else {
                V1y = Ymin;
                V2y = Ymax;
            }

            if (gFrustumPlanes[i].unk14[0] & 4) {
                V1z = Zmax;
                V2z = Zmin;
            } else {
                V1z = Zmin;
                V2z = Zmax;
            }

            Fd = gFrustumPlanes[i].d; // not used but required to be loaded here
            Fx = gFrustumPlanes[i].x;
            Fy = gFrustumPlanes[i].y;
            Fz = gFrustumPlanes[i].z;

            dotProduct = gFrustumPlanes[i].d + ((V1x * Fx) + (V1y * Fy) + (V1z * Fz));
            if (dotProduct < 0.0f) {
                dotProduct = gFrustumPlanes[i].d + ((V2x * Fx) + (V2y * Fy) + (V2z * Fz));
                if (dotProduct < 0.0f) {
                    visible = FALSE;
                    break;
                }
            }
        }

        //Set flags
        if (visible == FALSE) {
            shape->flags &= ~RENDER_SHAPE_VISIBLE;
            shape++;
            continue;
        }

        //Tri-level culling (based on vertex coords)
        if (!(shape->flags & RENDER_NO_CULL)) {
            visible = FALSE;
            shapeVtx = &block->vertices[shape->vtxBase];
            tri = block->encodedTris;
            tri += shape->triBase;
            triEnd = block->encodedTris;
            triEnd += shape[1].triBase;

            while ((u32) tri < (u32) triEnd) {
                s32 a2 = (s32) tri->d0 >> 0xD;
                s32 Ax, Ay, Az;
                s32 Bx, By, Bz;

                Ax = shapeVtx[a2 & 0x1F].ob[0] - Wx;
                Ay = shapeVtx[a2 & 0x1F].ob[1] - Wy;
                Az = shapeVtx[a2 & 0x1F].ob[2] - Wz;
                Bx = (s32) tri->d0 >> 0x12;
                By = (s32) (tri->d1 << 0xE) >> 0x12;
                Bz = (s32) tri->d1 >> 0x12;

                //Get dot product of vectors (check if seeing back of face?)
                if ((Ax * Bx) + (Ay * By) + (Az * Bz) < 0) {
                    tri->d1 |= 1;
                    visible = TRUE;
                } else {
                    tri->d1 &= ~1;
                }

                tri++;
            }

            if (visible == FALSE) {
                shape->flags &= ~RENDER_SHAPE_VISIBLE;
                shape++;
                continue;
            }
        }

        shape->flags |= RENDER_SHAPE_VISIBLE;
        shape++;
    }
}


void block_add_to_render_list(Block *block, f32 x, f32 z) {
    s32 unused;
    s32 oldRenderListLength;
    s32 i;
    s32 param;
    s32 pad;
    MtxF mf;
    MtxF mf2;
    
    oldRenderListLength = gRenderListLength;

    for (i = 0; i < block->shapeCount; i++) {
        if ((block->shapes[i].flags & RENDER_SHAPE_VISIBLE) && gRenderListLength < MAX_RENDER_LIST_LENGTH) {
            if (block->shapes[i].flags & RENDER_SEMI_TRANSPARENT) {
                param = 100000 - (gBlocksToDrawIdx * 400) - i;

                if (block->shapes[i].flags & RENDER_UNK2000) {
                    param -= 200;
                }
            } else {
                param = 200000 - (gBlocksToDrawIdx * 400) - i;
            }

            gRenderList[gRenderListLength] = (param << 14) | (i << 7) | gBlocksToDrawIdx;
            gRenderListLength++;
        }
    }

    if (((oldRenderListLength & 0xFFFFFFFF) != (u32) gRenderListLength) && gBlocksToDrawIdx < MAX_BLOCKS) {
        gBlocksToDraw[gBlocksToDrawIdx] = block;
        gBlocksToDrawIdx++;

        matrix_translation(&mf, x, 0.0f, z);
        matrix_f2l_4x3(&mf, gWorldRSPMatrices);

        gWorldRSPMatrices++;

        mf.m[3][1] = block->minY;

        matrix_scaling(&mf2, 1.0f, 0.05f, 1.0f);
        matrix_concat(&mf2, &mf, &mf);
        matrix_f2l_4x3(&mf, gWorldRSPMatrices);

        gWorldRSPMatrices++;
    }
}

void track_add_visible_objects(s8* objVisibilities) {
    Object* object;
    Object** objects;
    s32 numObjs;
    s32 visibleStartIdx;
    s32 i;
    s32 var_v0;
    s8* vis;

    objects = get_world_objects(NULL, NULL);
    // Separate invisible objects from visible objects
    visibleStartIdx = obj_visibility_sort_objects(&numObjs);
    if (numObjs > MAX_VISIBLE_OBJECTS) {
        STUBBED_PRINTF("depthSortObjects: MAX_VISIBLE_OBJECTS exceeded\n");
        numObjs = MAX_VISIBLE_OBJECTS;
    }
    // Depth sort just the visible objects
    obj_depth_sort_objects(visibleStartIdx, numObjs - 1);
    for (i = 0; i < numObjs; i++) {
        object = objects[i];
        vis = &objVisibilities[i];
        if (i < visibleStartIdx) {
            // Object is always invisible due to its definition
            *vis = FALSE;
        } else {
            *vis = track_obj_vis_check(object);
            if (*vis && (object->shadow != NULL) && (object->def->shadowType == OBJ_SHADOW_GEOM)) {
                shadows_update_obj_geom(object, 0, 0, gUpdateRate);
            }
            if ((object->shadow != NULL) && (object->def->shadowType == OBJ_SHADOW_BOX))  {
                shadows_update_obj_box(object);
            }
            if (gRenderListLength < MAX_RENDER_LIST_LENGTH) {
                if (object->def->flags & OBJDEF_FORCE_OPAQUE_DRAW_ORDER) {
                    var_v0 = 150000 - i;
                } else if ((object->opacityWithFade == 0xFF) && !(object->srt.flags & OBJFLAG_FORCE_TRANSPARENT_DRAW_ORDER)) {
                    var_v0 = 150000 - i;
                } else {
                    var_v0 = i + 50000;
                }
                gRenderList[gRenderListLength] = (var_v0 << 14) | (i << 7) | 0x40;
                gRenderListLength += 1;
            }
            /* default.dol
            else {
                STUBBED_PRINTF("DrawGroups overflow\n"); // (default.dol)
            }
            */
        }
    }
    if (gRenderListLength >= 2) {
        track_sort_render_list(gRenderList, gRenderListLength);
    }
}

void track_sort_render_list(u32* list, s32 length) {
    u32 temp_a0;
    s32 var_t0;
    s32 var_v0;
    s32 var_v1;

    for (var_v0 = 1; (length - 1) / 9 >= var_v0; var_v0++) {
        var_v0 *= 3;
        // @fake
        var_v1 = var_v0 + 1;
    }

    while (var_v0 > 0) {
        var_v1 = var_v0 + 1;
        while (var_v1 < length) {
            temp_a0 = list[var_v1];
            var_t0 = var_v1;
            if (var_v0 < var_t0) {
                while (list[var_t0 - var_v0] < temp_a0) {
                    list[var_t0] = list[var_t0 - var_v0];
                    var_t0 -= var_v0;
                    if (var_v0 >= var_t0) {
                        break;
                    }
                }
            }
            var_v1++;
            list[var_t0] = temp_a0;
        }
        var_v0 /= 3;
    }
}

// Note: This does not check every loaded map and probably won't behave how you'd expect!
s32 map_are_world_coords_in_map(f32 worldX, f32 worldZ) {
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

ObjSetup* map_find_obj_setup(s32 searchUID, s32* outIndexInMap, s32* outMapID, s32* arg3, s32* outIsMobileMap) {
    s32 mapID;
    s32 offset;
    s32 object_indexInMap;
    u16 objsSubfileEnd;
    ObjSetup *obj;
    
    for (mapID = 0; mapID < 120; mapID++) {
        // @fake
        if (!gMapActiveStreamMap) {}
        if (!gLoadedMapsDataTable[mapID]) {
            continue;
        }
    
        gMapActiveStreamMap = gLoadedMapsDataTable[mapID];
        obj = (ObjSetup*)gLoadedMapsDataTable[mapID]->objectInstanceFile_ptr;
        objsSubfileEnd = gLoadedMapsDataTable[mapID]->objectInstancesFileLength;
        for (object_indexInMap = 0, offset = 0; offset < objsSubfileEnd; object_indexInMap++) {
            if (searchUID == obj->uID) {
                if (outIndexInMap){
                    *outIndexInMap = object_indexInMap;
                }

                if (outMapID) {
                    *outMapID = mapID;
                }

                if (arg3 != NULL) {
                    *arg3 = gMapActiveStreamMap->unk19;
                }

                if (outIsMobileMap != NULL) {
                    if (mapID >= MAP_ID_MAX) {
                        *outIsMobileMap = TRUE;
                        // @fake
                        if ((!searchUID) && (!searchUID)) {}
                        return obj;
                    }
                    *outIsMobileMap = FALSE;
                }
                
                return obj;
            }

            offset += obj->quarterSize * 4;
            obj = (ObjSetup*)&((s8 *)obj)[obj->quarterSize * 4];
        }
    }

    return NULL;
}

s32 map_world_coords_to_block_index(f32 x, f32 y, f32 z) {
    s32 gridX;
    s32 gridZ;
    Block *currentBlock;
    s32 i;
    s8 *temp;
    
    gridX = floor_f(x / BLOCKS_GRID_UNIT) - gMapCurrentStreamCoordsX;
    gridZ = floor_f(z / BLOCKS_GRID_UNIT) - gMapCurrentStreamCoordsZ;
    
    if (gridX < 0 || gridX >= BLOCKS_GRID_SPAN) {
        return -1;
    }
    if (gridZ < 0 || gridZ >= BLOCKS_GRID_SPAN) {
        return -1;
    }
    gridX = GRID_INDEX(gridZ, gridX);
    for (i = 0; i < MAP_LAYER_COUNT; i++) {
        temp = gBlockIndices[i];
        if (temp[gridX] >= 0) {
            currentBlock = gLoadedBlocks[temp[gridX]];
            
            //Check if within bounds of block (along Y axis)
            if ((currentBlock->minY - 50) < y && y < (currentBlock->maxY + 50)) {
                return temp[gridX];
            }
        }
    }

    return -1;
}

void map_world_to_block_world_coords(f32 worldX, f32 worldY, f32 worldZ, f32* blockWorldX, f32* blockWorldZ) {
    s32 gridX;
    s32 gridZ;

    gridX = floor_f(worldX / BLOCKS_GRID_UNIT);
    gridZ = floor_f(worldZ / BLOCKS_GRID_UNIT);
    
    *blockWorldX = (f32) gridX * BLOCKS_GRID_UNIT;
    *blockWorldZ = (f32) gridZ * BLOCKS_GRID_UNIT;
}

s16 map_world_xz_to_map_id(f32 worldX, f32 worldZ) {
    s32 gridX;
    s32 gridZ;
    GlobalMapCell *layer;

    gridX = floor_f(worldX / BLOCKS_GRID_UNIT) - gMapCurrentStreamCoordsX;
    gridZ = floor_f(worldZ / BLOCKS_GRID_UNIT) - gMapCurrentStreamCoordsZ;
    
    if (gridX < 0 || gridX >= BLOCKS_GRID_SPAN) {
        return -1;
    }
    if (gridZ < 0 || gridZ >= BLOCKS_GRID_SPAN) {
        return -1;
    }

    layer = gDecodedGlobalMap[0];
    return layer[GRID_INDEX(gridZ, gridX)].mapIDs[0];
}

s32 map_find_map_id_of_obj_setup(ObjSetup *setup) {
    GlobalMapCell* var_a3;
    MapHeader* temp_v1;
    s16 var_v0;
    s32 var_a1;
    s32 var_a2;
    s32 var_t0;
    s32 var_t1;
    ObjSetup* obj;
    s16 sp20[4];
    ObjSetup* obj2;
    u32 objSubfileEnd;

    var_v0 = 0;
    var_a3 = *gDecodedGlobalMap;
    for (var_a1 = 0; var_a1 < BLOCKS_GRID_TOTAL_CELLS; var_a1++, var_a3++) {
        for (var_a2 = 0; var_a2 < 3; var_a2++) {
            if ((var_a3->mapIDs[var_a2] >= 0) && (var_a3->mapIDs[var_a2] < MAP_ID_MAX)) {
                var_t0 = FALSE;
                if (gLoadedMapsDataTable[var_a3->mapIDs[var_a2]] != NULL) {
                    for (var_t1 = 0; var_t1 < var_v0; var_t1++) {
                        if (sp20[var_t1] == var_a3->mapIDs[var_a2]) {
                            var_t0 = TRUE;
                            break;
                        }
                    }
                    if (var_t0 == FALSE) {
                        sp20[var_v0++] = var_a3->mapIDs[var_a2];
                    }
                }
            }
        }
    }

    for (var_a1 = 0; var_a1 < var_v0; var_a1++) {
        temp_v1 = gLoadedMapsDataTable[sp20[var_a1]];
        if (temp_v1 != NULL) {
            obj = (ObjSetup *) temp_v1->objectInstanceFile_ptr;
            objSubfileEnd = (u32)(temp_v1->objectInstancesFileLength + (s8 *)obj);
            while ((u32)obj < objSubfileEnd) {
                obj2 = obj;
                if (obj == setup) {
                    STUBBED_PRINTF("found on map %d\n", sp20[var_a1]);
                    return sp20[var_a1];
                }
                obj = (ObjSetup*)&((s8 *)obj)[obj2->quarterSize * 4];
            }
        }
    }

    STUBBED_PRINTF("mapno not found\n");
    return -1;
}

MapHeader** map_get_loaded_maps_table(void) {
    return gLoadedMapsDataTable;
}

/** Assign object instance file length and get object instance file from map */
ObjSetup* map_world_xz_to_map_obj_setup_list(f32 worldX, f32 worldZ, s32* objectsFileLength) {
    s32 mapID;
    MapHeader *map;

    mapID = map_world_xz_to_map_id(worldX, worldZ);
    if (mapID != -1){
      *objectsFileLength = gLoadedMapsDataTable[mapID]->objectInstancesFileLength;
      return (void *)gLoadedMapsDataTable[mapID]->objectInstanceFile_ptr;
    }
    return 0;
}

/** Assign blockIndex from worldX/Z */
s32 map_world_xz_to_block_index(s32 worldX, s32 worldZ, s32* blockIndex) {
    s8 *blocksLayer;
    s32 *tempX;
      
    blocksLayer = gBlockIndices[0];
      
    worldX = floor_f((f32) worldX / BLOCKS_GRID_UNIT);
    worldZ = floor_f((f32) worldZ / BLOCKS_GRID_UNIT);
  
    tempX = &gMapCurrentStreamCoordsX;
      
    *blockIndex = blocksLayer[GRID_INDEX(worldZ - gMapCurrentStreamCoordsZ, worldX - *tempX)];
    return 1;
}

/** Get Block from visGrid cell */
Block* map_get_block_from_grid(s32 visGridX, s32 visGridZ, s32 mapLayer) { 
    s8 *blocksLayer;
    s8 blockIndex;

    blocksLayer = gBlockIndices[mapLayer];
    
    if (visGridX < 0 || visGridZ < 0 || visGridX >= BLOCKS_GRID_SPAN || visGridZ >= BLOCKS_GRID_SPAN){
        return 0;
    }

    blockIndex = blocksLayer[GRID_INDEX(visGridZ, visGridX)];
    
    if (blockIndex < 0 || blockIndex >= gLoadedBlockCount)
        return 0;
    return gLoadedBlocks[blockIndex];
}

/** Get visGrid layer */
s8* map_get_block_grid_layer(s32 layer) {
    return gBlockIndices[layer];
}

/** Get Block from blockIndex */
Block* map_get_block_by_index(s32 blockIndex) {
    if (blockIndex < 0 || blockIndex >= gLoadedBlockCount) {
        return 0;
    }
    return gLoadedBlocks[blockIndex];
}

void track_update_frustum(void) {
    f32 spDC;
    f32 spD8;
    f32 spD4;
    f32 spD0;
    f32 spCC;
    f32 spC8;
    Camera* camera;
    MtxF sp84;
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    Plane *plane;
    f32 var_fv1;
    s16 sp6E;
    s16 var_s0;
    s16 var_v0;
    s16 sp68;
    SRT srt;

    camera = get_camera();
    if ((gTrackFlags & TRACKFLAG_UNK8) || (gTrackFlags & TRACKFLAG_UNK10000)) {
        sp80 = camera_get_fov() / 1.5f;
    } else {
        sp80 = camera_get_fov() * 0.5f;
    }
    spD0 = camera->tx - gWorldX;
    spCC = camera->ty;
    spC8 = camera->tz - gWorldZ;
    srt.transl.x = 0.0f;
    srt.transl.y = 0.0f;
    srt.transl.z = 0.0f;
    srt.scale = 1.0f;
    srt.yaw = 0x8000 - camera->yaw;
    srt.pitch = -camera->pitch;
    srt.roll = camera->roll;
    var_s0 = 0;
    matrix_from_srt(&sp84, &srt);
    vec3_transform(&sp84, 0.0f, 0.0f, -1.0f, &spDC, &spD8, &spD4);
    plane = &gFrustumPlanes[var_s0];
    plane->x = spDC;
    plane->y = spD8;
    plane->z = spD4;
    plane->d = -((spD0 * spDC) + (spCC * spD8) + (spC8 * spD4));
    var_s0++;
    sp78 = fcos16_precise(sp80 * 182.0f);
    sp6E = arctan2_f(160.0f, 120.0f / (fsin16_precise(sp80 * 182.0f) / sp78));
    sp7C = fsin16_precise(sp6E);
    sp78 = fcos16_precise(sp6E);
    vec3_transform(&sp84, -(-sp78), 0.0f, -sp7C, &spDC, &spD8, &spD4);
    plane = &gFrustumPlanes[var_s0];
    plane->x = spDC;
    plane->y = spD8;
    plane->z = spD4;
    plane->d = -((spD0 * spDC) + (spCC * spD8) + (spC8 * spD4));
    var_s0++;
    vec3_transform(&sp84, -sp78, 0.0f, -sp7C, &spDC, &spD8, &spD4);
    plane = &gFrustumPlanes[var_s0];
    plane->x = spDC;
    plane->y = spD8;
    plane->z = spD4;
    plane->d = -((spD0 * spDC) + (spCC * spD8) + (spC8 * spD4));
    var_s0++;
    vec3_transform(&sp84, 0.0f, -sp78, -sp7C, &spDC, &spD8, &spD4);
    plane = &gFrustumPlanes[var_s0];
    plane->x = spDC;
    plane->y = spD8;
    plane->z = spD4;
    plane->d = -((spD0 * spDC) + (spCC * spD8) + (spC8 * spD4));
    var_s0++;
    vec3_transform(&sp84, 0.0f, -(-sp78), -sp7C, &spDC, &spD8, &spD4);
    plane = &gFrustumPlanes[var_s0];
    plane->x = spDC;
    plane->y = spD8;
    plane->z = spD4;
    plane->d = -((spD0 * spDC) + (spCC * spD8) + (spC8 * spD4));
    for (var_s0 = 0; var_s0 < MAP_LAYER_COUNT; var_s0++) {
        var_fv1 = 0.0f;
        var_v0 = 0;
        while (var_v0 < 24) {
            spDC = gFrustumPlanes[var_s0].x * D_80092AAC[var_v0++];
            spDC += gFrustumPlanes[var_s0].y * D_80092AAC[var_v0++];
            spDC += gFrustumPlanes[var_s0].z * D_80092AAC[var_v0++];
            if (var_fv1 < spDC) {
                var_fv1 = spDC;
                sp68 = var_v0 - 3;
            }
        }
        // @bug, sp68 could be unassigned
        switch (sp68) {
        case 0:
            gFrustumPlanes[var_s0].unk14[0] = 0;
            break;
        case 3:
            gFrustumPlanes[var_s0].unk14[0] = 2;
            break;
        case 6:
            gFrustumPlanes[var_s0].unk14[0] = 4|1;
            break;
        case 9:
            gFrustumPlanes[var_s0].unk14[0] = 4|2|1;
            break;
        case 12:
            gFrustumPlanes[var_s0].unk14[0] = 1;
            break;
        case 15:
            gFrustumPlanes[var_s0].unk14[0] = 2|1;
            break;
        case 18:
            gFrustumPlanes[var_s0].unk14[0] = 4;
            break;
        case 21:
            gFrustumPlanes[var_s0].unk14[0] = 4|2;
            break;
        default:
            STUBBED_PRINTF("error\n");
            break;
        }
    }
}

s32 block_frustum_check(s32 xPos, s32 zPos, Block* block) {
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

    if (block != NULL) {
        minY = block->minY;
        maxY = block->maxY;
    } else {
        minY = -100000.f;
        maxY = 100000.f;
    }

    for (i = 0; i < MAP_LAYER_COUNT; i++) {
        currentPlane = &gFrustumPlanes[i];

        scaledXPos = xPos * BLOCKS_GRID_UNIT_F;
        scaledZPos = zPos * BLOCKS_GRID_UNIT_F;
        planeX = currentPlane->x;
        planeY = currentPlane->y;
        planeZ = currentPlane->z;
        planeD = currentPlane->d;
        for (j = 0, stop = FALSE; j < 8 && stop == FALSE; j++) {
            if (j & 1) {
                var_fs2 = (xPos * BLOCKS_GRID_UNIT_F) * planeX;
            } else {
                var_fs2 = ((xPos * BLOCKS_GRID_UNIT_F) + BLOCKS_GRID_UNIT_F) * planeX;
            }
            if (j & 2) {
                var_fs2 += (scaledZPos * planeZ);
            } else {
                var_fs2 += ((scaledZPos + BLOCKS_GRID_UNIT_F) * planeZ);
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
    s32 cellY;
    s32 cellX;
    s32 temp;
    s32 cellZ;
    Block* sp28;

    if (D_800B9794 == 0) {
        return;
    }

    sp3C = floor_f((D_800B51E4->srt.transl.x - gWorldX) / BLOCKS_GRID_UNIT_F);
    sp28 = map_get_block_from_grid(sp3C, floor_f((D_800B51E4->srt.transl.z - gWorldZ) / BLOCKS_GRID_UNIT_F), 0);

    sp3C = floor_f(D_800B51E4->srt.transl.x / BLOCKS_GRID_UNIT_F) * BLOCKS_GRID_UNIT;
    temp = (floor_f(D_800B51E4->srt.transl.z / BLOCKS_GRID_UNIT_F) * BLOCKS_GRID_UNIT);
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
        var_t0 = 80;
    }
    cellY = var_v0 / var_t0;
    cellX = temp_t2 / 80;
    cellZ = temp_t3 / 80;
    diPrintf(" cellx %i celly %i cellz %i ", cellX, cellY, cellZ);
    var_v1 = D_800B979E >> 3;
    if (D_800B979E & 7) {
        var_v1 += 1;
    }
    temp = ((cellY * (8 * 8)) + (cellZ * 8) + cellX);
    bitstream_init(stream, &D_800B9798[(var_v1 * temp)], D_800B979E, D_800B979E);
}

static const char str_8009a5e0[] = " ERROR: visible index greater than no vis blocks ";

s32 func_80045600(s32 arg0, BitStream *stream, s16 arg2, s16 arg3, s16 arg4) {
    s8 bitPosIndex;
    s8 *new_var;

    new_var = D_800B9700[arg4];
    bitPosIndex = new_var[arg2 + arg3 * 16];
    
    if (bitPosIndex >= 0){
        bitstream_set_pos(stream, D_800B97A0[bitPosIndex] + arg0);
        return bitstream_read(stream, 1);
    }
    diPrintf("SW ");
    return 0;
}

/** 
 * Updates the object's distance-based fade and returns whether the object is visible 
 * according to the distance fade and frustum culling.
 */
u8 track_obj_vis_check(Object* obj) {
    f32 fadeDist;
    Object* playerObj;
    f32 dist;
    f32 temp_fv1;
    f32 temp;
    s32 fadeOpacity;

    if (obj->opacity == 0) {
        obj->opacityWithFade = 0;
        return FALSE;
    }
    if ((obj->setup != NULL) && (obj->setup->fadeFlags & OBJSETUP_FADE_MANUAL)) {
        obj->opacityWithFade = ((obj->opacity + 1) * 255) >> 8;
    } else {
        fadeDist = obj->fadeDistance;
        if (fadeDist < 40.0f) {
            obj->opacityWithFade = 0;
            return FALSE;
        }
        if ((obj->setup != NULL) && (obj->setup->fadeFlags & OBJSETUP_FADE_MAIN) && (playerObj = get_player(), (playerObj != NULL))) {
            dist = vec3_distance(&obj->globalPosition, &playerObj->globalPosition);
        } else {
            dist = camera_get_distance_to_point(obj->globalPosition.x, obj->globalPosition.y, obj->globalPosition.z);
        }
        if (fadeDist < dist) {
            obj->opacityWithFade = 0;
            return FALSE;
        }
        // Fade from 255 opacity -> 0 from (fadeDist - 40) -> (fadeDist)
        fadeOpacity = 255;
        temp_fv1 = fadeDist - 40.0f;
        if (temp_fv1 < dist) {
            fadeDist -= temp_fv1;
            dist = dist - temp_fv1;
            temp = 1.0f - (dist / fadeDist);
            fadeOpacity = 255.0f * temp;
        }
        obj->opacityWithFade = ((obj->opacity + 1) * fadeOpacity) >> 8;
    }
    if (obj->opacityWithFade == 0) {
        return FALSE;
    }
    if (obj->id == OBJ_FXEmit) {
        return TRUE;
    }
    return is_sphere_in_frustum(&obj->globalPosition, obj->visRadius);
}

u8 is_sphere_in_frustum(Vec3f *v, f32 radius) {
    u8 i;

    for (i = 0; i < MAP_LAYER_COUNT; i++)
    {
        if ((gFrustumPlanes[i].x * (v->x - gWorldX) +
            gFrustumPlanes[i].y * v->y +
            gFrustumPlanes[i].z * (v->z - gWorldZ) +
            gFrustumPlanes[i].d +
            radius)
            < 0.0f)
        {
            return FALSE;
        }
    }

    return TRUE;
}

MapHeader* map_load_streammap(s32 mapID, s32 arg1) {
    s32 map_start;
    s32 map_size; // sp48
    s32 instanceCount;
    MapHeader *map;
    s32 var_v0;
    s32 size; // sp38
    
    map_start = READ_MAPS_TAB(mapID, 0);
    map_size = READ_MAPS_TAB(mapID, 7) - map_start;
    queue_load_file_region_to_ptr((void *) gMapReadBuffer, MAPS_BIN, map_start, sizeof(MapHeader));
    gMapActiveStreamMap = (MapHeader*)gMapReadBuffer;

    size = gMapActiveStreamMap->gridB_sixteenthSize;
    instanceCount = gMapActiveStreamMap->objectInstanceCount;
    gMapActiveStreamMap = mmAlloc(((size * 0x20) + map_size) + (instanceCount >> 3) + 1, 
        ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:map"));

    queue_load_file_region_to_ptr((void*)gMapActiveStreamMap, MAPS_BIN, map_start, map_size);
    
    //Setting up pointers to the 7 MAPS files (excluding the header) (and EOF)
    gMapActiveStreamMap->blockIDs_ptr = (u32*)(((u8*)gMapActiveStreamMap) + READ_MAPS_TAB(mapID, 1) - map_start);
    gMapActiveStreamMap->grid_A1_ptr =  (s8*) (((u8*)gMapActiveStreamMap) + READ_MAPS_TAB(mapID, 2) - map_start);
    gMapActiveStreamMap->grid_A2_ptr =  (s8*) (((u8*)gMapActiveStreamMap) + READ_MAPS_TAB(mapID, 3) - map_start);
    gMapActiveStreamMap->objectInstanceFile_ptr = (((u8*)gMapActiveStreamMap) + READ_MAPS_TAB(mapID, 4) - map_start);
    gMapActiveStreamMap->grid_B1_ptr =  (s8*) (((u8*)gMapActiveStreamMap) + READ_MAPS_TAB(mapID, 5) - map_start);
    gMapActiveStreamMap->grid_B2_ptr =  (s8*) (gMapActiveStreamMap->grid_B1_ptr + size * 16);
    gMapActiveStreamMap->end_ptr =      (s8*) (gMapActiveStreamMap->grid_B2_ptr + size * 16);

    for (var_v0 = 0; var_v0 < (instanceCount >> 3) + 1; var_v0++){
        gMapActiveStreamMap->end_ptr[var_v0] = 0;
    }
    
    gMapActiveStreamMap->originWorldX = 0.0f;
    gMapActiveStreamMap->originWorldZ = 0.0f;
    gMapActiveStreamMap->unk18 = 0;
    gMapActiveStreamMap->unk19 = 0;
    //It ignores the asset's stored length and calculates it instead
    gMapActiveStreamMap->objectInstancesFileLength = (u32)gMapActiveStreamMap->grid_B1_ptr - (u32)gMapActiveStreamMap->objectInstanceFile_ptr;
    gLoadedMapsDataTable[mapID] = gMapActiveStreamMap;
    
    if (arg1 == 0){
        map_init_obj_setup_list(gMapActiveStreamMap, &gMapObjSetupLists[mapID], mapID, 0);
        gDLL_29_Gplay->vtbl->world_load_obj_group_bits(mapID);
    }
    else
    {
        map_restore_saved_objects(gMapActiveStreamMap, mapID);
    }
    return gMapActiveStreamMap;
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/map/map_load_streammap_add_to_table.s")
#else
s32 map_load_streammap_add_to_table(s32 arg0) {
    s32 sp2C;
    Struct_D_800B9768_unk4* temp_a3;

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
    gMapActiveStreamMap->originWorldX = (temp_a3->xMin + gMapActiveStreamMap->originOffsetX) * BLOCKS_GRID_UNIT_F;
    gMapActiveStreamMap->originWorldZ = (temp_a3->zMin + gMapActiveStreamMap->originOffsetZ) * BLOCKS_GRID_UNIT_F;
    map_convert_objpositions_to_ws(gMapActiveStreamMap, gMapActiveStreamMap->originWorldX, gMapActiveStreamMap->originWorldZ);
    map_restore_saved_objects(gMapActiveStreamMap, arg0);
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

s32 func_80045DC0(s32 arg0, s32 arg1, s32 arg2) {
    Struct_D_800B9768_unk4* var_v0;
    s32 temp_a2;
    s32 temp_t3;
    Struct_D_800B9768_unk10 * var_v1;
    s32 j;

    var_v0 = &D_800B9768.unk4[0];
    var_v1 = &D_800B9768.unk10[0];
    arg2 = D_80092A9C[arg2] + gMapLayer;
    for (j = 0; j < 64; j += 1) {
        if (arg2 == D_800B9768.unkC[j]) {
            if (arg0 >= var_v0->xMin && var_v0->xMax >= arg0) {
                if ((arg1 >= var_v0->zMin) && (var_v0->zMax >= arg1)) {
                    temp_t3 = (arg0 - var_v0->xMin) + ((arg1 - var_v0->zMin) * ((var_v0->xMax - var_v0->xMin) + 1));
                    if (((temp_t3 >> 3) + var_v1->unk0)[0] & (1 << (temp_t3 & 7))) {
                        return j;
                    }
                }
            }
        }

        var_v0++;
        var_v1++;
    }
    return -1;
}

// track_free_map
void map_free(s32 mapID) {
    if (gLoadedMapsDataTable[mapID]){
        map_init_obj_setup_list(gLoadedMapsDataTable[mapID], &gMapObjSetupLists[mapID], mapID, 1);
        mmFree(gLoadedMapsDataTable[mapID]);
        gLoadedMapsDataTable[mapID] = 0;
    } else {
        STUBBED_PRINTF("trackFreeMap: Error no map!!\n");
    }
}

void map_init_obj_setup_list(MapHeader* map, MapObjSetupList* setupList, s32 mapID, s32 param4) {
    s32 _pad;
    s32 objSetupFileLength;
    s32 groupsStart;
    u8 foundCurves;
    s8 _sp42;
    s8 _sp41;
    s8 _sp40;
    s32 fileOffset;
    s32 foundGroups;
    ObjSetup *objSetup;
    s32 i;

    foundCurves = FALSE;
    foundGroups = 0;
    objSetup = (ObjSetup *) map->objectInstanceFile_ptr;
    objSetupFileLength = map->objectInstancesFileLength;
    if (objSetupFileLength == 0) {
        return;
    }
    fileOffset = 0;

    if (!param4) {
        setupList->curvesOffset = -1;
        for (i = 0; i < 32; i++) { setupList->groups[i] = -1; }
    }
    while (fileOffset < objSetupFileLength) {
        if (param4) {
            if (objSetup->objId == OBJ_curve) {
                gDLL_26_Curves->vtbl->func_10C((CurveSetup*)objSetup);
            }
            if (objSetup->objId == OBJ_checkpoint4) {
                gDLL_4_Race->vtbl->func2((RaceCheckpointSetup*)objSetup);
            }
        } else {
            if ((OBJ_curve == objSetup->objId) || (OBJ_checkpoint4 == objSetup->objId)) {
                if (OBJ_curve == objSetup->objId) {
                    gDLL_26_Curves->vtbl->func_34((CurveSetup*)objSetup);
                } else {
                    gDLL_4_Race->vtbl->func1((RaceCheckpointSetup*)objSetup);
                }
                if (!foundCurves) {
                    setupList->curvesOffset = ((u32)objSetup - (u32)map->objectInstanceFile_ptr);
                    foundCurves = TRUE;
                }
            } else if (objSetup->loadFlags & OBJSETUP_LOAD_IN_MAP_OBJGROUP) {
                if (!((1 << objSetup->mapObjGroup) & foundGroups)) {
                    setupList->groups[objSetup->mapObjGroup] = (u32)objSetup - (u32)map->objectInstanceFile_ptr;
                    foundGroups |= 1 << objSetup->mapObjGroup;
                }
            }
        }
        fileOffset += objSetup->quarterSize * 4;
        objSetup = (ObjSetup *) &((s8 *)objSetup)[objSetup->quarterSize * 4];
    }
    if (!param4) {
        groupsStart = objSetupFileLength;
        if ((setupList->curvesOffset != -1) && (setupList->curvesOffset < objSetupFileLength)) {
            groupsStart = setupList->curvesOffset;
        }
        for (i = 0; i < 32; i++) {
            if ((setupList->groups[i] != -1) && (setupList->groups[i] < groupsStart)) {
                groupsStart = setupList->groups[i];
            }
        }
        setupList->groupsStart = groupsStart;
        if (setupList->curvesOffset != -1) {
            setupList->objsListSize = setupList->curvesOffset;
            return;
        }
        setupList->objsListSize = objSetupFileLength;
    }
}

// this is probably checkMap in default.dol
void func_800462B0(){
}

/** Iterates over all the ObjSetup in a map's object instance file, transforming the objects' coordinates from local coordinates to world-space coordinates */
void map_convert_objpositions_to_ws(MapHeader *map, f32 X, f32 Z) {
    u8 *ptr;
    s32 offset;
    ObjSetup *obj;

    if (!map){
        return;
    }
    
    ptr = (u8*)map->objectInstanceFile_ptr;
    offset = 0; // current offset in MAPS object instance file data
    
    while (offset < map->objectInstancesFileLength){
        obj = (ObjSetup*)ptr;
        obj->x += X;
        obj->z += Z;

        offset += obj->quarterSize << 2;
        ptr += obj->quarterSize << 2;
    }
}

void map_load_mobile_map(s32 id, Object *obj) {
    s32 sp24;
    s32 foundEmptySlot;
    s32 mapID;
    MapHeader *sp18;

    sp24 = D_800B4A50;
    sp18 = map_load_streammap(id, 1);
    gLoadedMapsDataTable[id] = 0;
    foundEmptySlot = FALSE;

    for (mapID = 80; mapID < 120; mapID++) {
        if (gLoadedMapsDataTable[mapID] == 0) {
            foundEmptySlot = TRUE;
            gLoadedMapsDataTable[mapID] = sp18;
            break;
        } 
    }

    map_init_obj_setup_list(sp18, &gMapObjSetupLists[mapID], mapID, 0);
    gDLL_29_Gplay->vtbl->world_load_obj_group_bits(mapID);
    obj->mobileMapID = mapID;
    gDLL_29_Gplay->vtbl->func_1378(id, mapID);
    if (!foundEmptySlot) {
        STUBBED_PRINTF("WORLD MAP LIST OVERFLOW\n");
    }
    D_800B4A50 = sp24;
}

void func_80046428(s32 worldGridX, s32 worldGridZ, GlobalMapCell* cell, s32 arg3) {
    Struct_D_800B9768_unk4 *temp;
    s32 sp30;
    s32 temp_v1_2;
    s32 mapIndex;
    MapHeader* sp24;
    s8 sp20[2];
    s16 *temp2;

    sp30 = func_80045DC0(worldGridX, worldGridZ, arg3);
    if (sp30 != -1) {
        mapIndex = map_find_streammap_index(sp30);
        if (mapIndex == -1) {
            mapIndex = map_load_streammap_add_to_table(sp30);
        }
        gMapStreamMapTable[mapIndex].unk06 = 1;
        sp24 = gMapStreamMapTable[mapIndex].header;
        temp2 = (s16*)(((u8*)D_800B9768.unk8) + (sp30 << 1 << 1));
        sp20[0] = temp2[0];
        sp20[1] = temp2[1];
        cell->mapIDs[0] = sp30;
        cell->mapIDs[1] = sp20[0];
        cell->mapIDs[2] = sp20[1];
        if (sp20[0] != -1) {
            mapIndex = map_find_streammap_index(sp20[0]);
            if (mapIndex == -1) {
                mapIndex = map_load_streammap_add_to_table(sp20[0]);
            }
            ((s8 *)&gMapStreamMapTable[mapIndex])[6] = 1;
        }
        if (sp20[1] != -1) {
            mapIndex = map_find_streammap_index(sp20[1]);
            if (mapIndex == -1) {
                mapIndex = map_load_streammap_add_to_table(sp20[1]);
            }
            ((s8 *)&gMapStreamMapTable[mapIndex])[6] = 1;
        }

        temp = &D_800B9768.unk4[sp30];
        worldGridX -= temp->xMin;
        worldGridZ -= temp->zMin;
        temp_v1_2 = ((s32*) &((s8 *)sp24->blockIDs_ptr)[worldGridX * 4 + ((worldGridZ *  sp24->gridSizeX) * 4)])[0];
        cell->loadedBlockIndex = (temp_v1_2 >> 17) & 0x3F;
        cell->trkBlkIndex = (temp_v1_2 >> 23) & 0x3F;
        if (cell->trkBlkIndex == 0x3F) {
            cell->trkBlkIndex = -1;
        }
        if (cell->trkBlkIndex == -1) {
            cell->blockID = -1;
            return;
        }
        if (cell->trkBlkIndex >= gNumTRKBLKEntries) {
            cell->trkBlkIndex = gNumTRKBLKEntries - 1;
            STUBBED_PRINTF("entry->gamno >= max_gam\n");
            // "WARNING track.c() ---- entry->gamno >= max_gam\n" (default.dol)
        }
        cell->blockID = gFile_TRKBLK[cell->trkBlkIndex] + cell->loadedBlockIndex;
        if (cell->blockID >= gFile_TRKBLK[gNumTRKBLKEntries]) {
            cell->blockID = gFile_TRKBLK[gNumTRKBLKEntries] - 1;
            STUBBED_PRINTF("entry->block > max gam\n");
        }
    } else {
        cell->mapIDs[0] = -1;
        cell->mapIDs[1] = -1;
        cell->mapIDs[2] = -1;
        cell->blockID = -2;
        cell->trkBlkIndex = -1;
        cell->loadedBlockIndex = 0;
    }
}

void func_80046688(s32 arg0, s32 arg1) {
}

GlobalMapCell* func_80046698(s32 gridX, s32 gridZ) {
    GlobalMapCell *layer;    
    s32 cellIndex;
    
    layer = gDecodedGlobalMap[0];
    cellIndex = GRID_INDEX(gridZ, gridX);
    
    return &layer[cellIndex];
}

MapHeader* func_800466C0(void) {
    MapHeader* map;
    GlobalMapCell *layer;
    s32 mapID;

    layer = gDecodedGlobalMap[0];
    mapID = layer[GRID_INDEX(7, 7)].mapIDs[0]; //There should be 16*16 cells, so why this one specifically... centre cell?
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

s16 func_80046718(void) {
    return gNumTRKBLKEntries;
}

/** map_get_mapID_blocks_count? (Calculated by quickly comparing base blockID for this map and next map) */
s32 func_80046728(s32 mapID) {
    if (mapID < 0 || mapID >= gNumTRKBLKEntries)
        return 0;
    return gFile_TRKBLK[mapID + 1] - gFile_TRKBLK[mapID];
}

void init_global_map(void) {
    #define SOME_LENGTH     64
    s32 i;
    s32 size;
    StructBuf *buf = NULL;
    Struct_D_800B9768_unk4 *thisunk4;

    size = get_file_size(GLOBALMAP_BIN);

    queue_alloc_load_file((void**)&buf, GLOBALMAP_BIN);

    size /= sizeof(StructBuf);

    D_800B9768.unk0  = -1;
    D_800B9768.unk4  = mmAlloc(sizeof(Struct_D_800B9768_unk4)  * SOME_LENGTH, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:glmap"));
    D_800B9768.unk8  = mmAlloc(sizeof(s16) * 2  * SOME_LENGTH, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:glmap"));
    D_800B9768.unkC  = mmAlloc(sizeof(Struct_D_800B9768_unkC)  * SOME_LENGTH, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:glmap"));
    D_800B9768.unk10 = mmAlloc(sizeof(Struct_D_800B9768_unk10) * SOME_LENGTH, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:glmap"));

    bzero(D_800B9768.unk10, sizeof(Struct_D_800B9768_unk10) * SOME_LENGTH);

    for (i = 0; i < SOME_LENGTH; i++) {
        thisunk4 = D_800B9768.unk4 + i;
        D_800B9768.unkC[i] = 0x80;
        thisunk4->xMin = 0x8000;
        thisunk4->xMax = 0x8000;
        thisunk4->zMin = 0x8000;
        thisunk4->zMax = 0x8000;
        thisunk4->unk8 = 0x80;
        thisunk4->unk9 = 0x80;

        D_800B9768.unk8[0][(i << 1) + 0] = -1;
        D_800B9768.unk8[0][(i << 1) + 1] = -1;
    }
    for (i = 0; i < size && (buf[i].unk6 >= 0); i++) {
        D_800B9768.unkC[buf[i].unk6] = buf[i].unk4;

        map_read_layout(
            &D_800B9768.unk4[buf[i].unk6],
            (u8 *)&D_800B9768.unk10[buf[i].unk6].unk0,
            buf[i].unk0,
            buf[i].unk2,
            buf[i].unk6
        );

        D_800B9768.unk8[0][(buf[i].unk6 << 1) + 0] = buf[i].unk8;
        D_800B9768.unk8[0][(buf[i].unk6 << 1) + 1] = buf[i].unkA;
    }

    map_func_80048034();

    mmFree(buf);
}

void map_read_layout(Struct_D_800B9768_unk4 *arg0, u8 *arg1, s16 arg2, s16 arg3, s32 mapID) {
    MapsBinStruct* mapbinstruct;
    s32 temp_v1;
    s32 var_v0;
    s32 temp;
    s32 size;

    mapbinstruct = (MapsBinStruct*)gMapReadBuffer;
    size = READ_MAPS_TAB(mapID, 2) - READ_MAPS_TAB(mapID, 0);

    /* default.dol
    if (size > 700) {
        STUBBED_PRINTF("track.c: getdim(): TEMP_BUFFER_SIZE overflow\n");
    }
    */
    
    queue_load_file_region_to_ptr((void**)mapbinstruct, MAPS_BIN, READ_MAPS_TAB(mapID, 0), size);
    
    mapbinstruct->unkC = (s32 *) 
        (((s8*)mapbinstruct) + READ_MAPS_TAB(mapID, 1) - READ_MAPS_TAB(mapID, 0));

    arg0->xMin = arg2 - mapbinstruct->unk4;
    arg0->zMin = arg3 - mapbinstruct->unk6;

    arg0->xMax = (arg0->xMin + mapbinstruct->unk0) - 1;
    arg0->zMax = (arg0->zMin + mapbinstruct->unk2) - 1;
    arg0->unk8 = mapbinstruct->unk4;
    arg0->unk9 = mapbinstruct->unk6;

    for (arg3 = 0; arg3 < mapbinstruct->unk2; arg3++) {
        for (arg2 = 0; arg2 < mapbinstruct->unk0; arg2++) {
            temp_v1 = arg2 + (arg3 * mapbinstruct->unk0);
            var_v0 = mapbinstruct->unkC[temp_v1];
            if (((var_v0 >> 23) & 0x3F) != 0x3F) {
                temp = 1 << (temp_v1 & 7);
                arg1[(temp_v1 >> 3)] |= temp;
            }
        }
    }
}

void map_func_80046B58(f32 x, f32 y, f32 z) {
    u32 temp_t8;
    
    temp_t8 = gTrackFlags;
    if (!(temp_t8 & TRACKFLAG_UPDATE_STREAMING) || (temp_t8 & TRACKFLAG_UPDATE_STREAMING_IMMEDIATE)) {
        D_800B97AC = x;
        D_800B97B0 = y;
        D_800B97B4 = z;
        gTrackFlags = temp_t8 | TRACKFLAG_UPDATE_STREAMING;
        if (gTrackFlags & TRACKFLAG_UPDATE_STREAMING_IMMEDIATE){
            map_update_streaming();
        }
    }
}

void map_update_streaming(void) {
    f32 f14;
    f32 sp308;
    GlobalMapCell *var_v1;
    s32 pad2;
    f32 tempX;
    f32 tempZ;
    s32 sp2F4;
    s32 sp2F0;
    s32 var_s1;
    s32 var_s7;
    s32 var_s3;
    s32 var_s5;
    s8* temp_a3;
    f32 f2;
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
    s32 xTemp;
    s32 zTemp;

    if (!(gTrackFlags & TRACKFLAG_UPDATE_STREAMING)) {
        return;
    }

    gTrackFlags &= ~TRACKFLAG_UPDATE_STREAMING;
    f0 = D_800B97AC;
    f2 = D_800B97B4;
    f14 = f0 - gWorldX;
    sp308 = f2 - gWorldZ;
    sp2F4 = floor_f(f14 / BLOCKS_GRID_UNIT_F);
    sp2F0 = floor_f(sp308 / BLOCKS_GRID_UNIT_F);
    sp294 = gTrackFlags & TRACKFLAG_UPDATE_STREAMING_IMMEDIATE;
    gTrackFlags &= ~TRACKFLAG_UPDATE_STREAMING_IMMEDIATE;
    if ((sp2F4 != 7) || (sp2F0 != 7) || (sp294 != 0) || (gTrackFlags & TRACKFLAG_LAYER_CHANGED)) {
        shadows_func_8004D974(1);
        func_80012B54(1, 0);
        var_fp = 0;
        for (var_s7 = 0; var_s7 < 5; var_s7++) {
            var_v1 = gDecodedGlobalMap[var_s7];
            temp_a3 = gBlockIndices[var_s7];
            D_800B9714 = D_800B9700[var_s7];
            var_s3 = 0;
            for (var_s2 = 0; var_s2 < BLOCKS_GRID_SPAN; var_s2++) {
                for (var_s0 = 0; var_s0 < BLOCKS_GRID_SPAN; var_s0++) {
                    if (temp_a3[var_s3] >= 0) {
                        sp84[var_fp].unk6 = var_s7;
                        sp84[var_fp].unk0 = gMapCurrentStreamCoordsX + var_s0;
                        sp84[var_fp].unk2 = gMapCurrentStreamCoordsZ + var_s2;
                        sp84[var_fp].unk4 = temp_a3[var_s3];
                        var_fp++;
                    }
                    temp_a3[var_s3] = -2;
                    D_800B9714[var_s3] = -1;
                    var_v1->blockID = -3;
                    var_v1->mapIDs[0] = -1;
                    var_v1->mapIDs[1] = -1;
                    var_v1->mapIDs[2] = -1;
                    var_v1++;
                    var_s3++;
                }
            }
        }
        tempX = gWorldX;
        tempZ = gWorldZ;
        gMapCurrentStreamCoordsX = (gMapCurrentStreamCoordsX + sp2F4) - 7;
        gMapCurrentStreamCoordsZ = (gMapCurrentStreamCoordsZ + sp2F0) - 7;
        gWorldX = gMapCurrentStreamCoordsX * BLOCKS_GRID_UNIT_F;
        gWorldZ = gMapCurrentStreamCoordsZ * BLOCKS_GRID_UNIT_F;
        D_80092A60 = gWorldX;
        D_80092A64 = gWorldZ;
        func_800307C4(tempX - gWorldX, tempZ - gWorldZ);
        for (var_s3 = 0; var_s3 < gMapNumStreamMaps; var_s3++) {
            gMapStreamMapTable[var_s3].unk06 = 0;
        }
        D_800B4A50 = func_80045DC0(gMapCurrentStreamCoordsX + 7, gMapCurrentStreamCoordsZ + 7, 0);
        D_800B4A54 = -1;
        if (D_800B4A50 != -1) {
            sp284 = map_find_streammap_index(D_800B4A50);
            if (sp284 == -1) {
                sp284 = map_load_streammap_add_to_table(D_800B4A50);
            }
            gMapStreamMapTable[sp284].unk06 = 1;
            D_800B4A54 = sp284;
            for (var_s7 = 0; var_s7 < ARRAYCOUNT_S(gBlockIndices); var_s7++) {
                map_check_block_grid(gMapCurrentStreamCoordsX + 7, gMapCurrentStreamCoordsZ + 7, sp2C8, sp2B8, sp2A8, sp298, var_s7, 0, sp284);
                temp_a3 = gBlockIndices[var_s7];
                D_800B9714 = D_800B9700[var_s7];
                for (var_s2 = sp2C8[2]; sp2C8[3] >= var_s2; var_s2++) {
                    for (var_s0 = sp2C8[0]; sp2C8[1] >= var_s0; var_s0++) {
                        temp_a3[var_s0 + (((var_s2 + 7) << 4)) + 7] = -3;
                    }
                }

                for (var_s2 = sp2B8[2]; sp2B8[3] >= var_s2; var_s2++) {
                    for (var_s0 = sp2B8[0]; sp2B8[1] >= var_s0; var_s0++) {
                        temp_a3[var_s0 + (((var_s2 + 7) << 4)) + 7] = -3;
                    }
                }

                for (var_s2 = sp2A8[2]; sp2A8[3] >= var_s2; var_s2++) {
                    for (var_s0 = sp2A8[0]; sp2A8[1] >= var_s0; var_s0++) {
                        temp_a3[var_s0 + (((var_s2 + 7) << 4)) + 7] = -3;
                    }
                }

                for (var_s2 = sp298[2]; sp298[3] >= var_s2; var_s2++) {
                    for (var_s0 = sp298[0]; sp298[1] >= var_s0; var_s0++) {
                if (var_s2) {}
                        temp_a3[var_s0 + (((var_s2 + 7) << 4)) + 7] = -3;
                    }
                }

                var_s3 = 0;
                var_s5 = 0;
                for (var_s2 = 0; var_s2 < BLOCKS_GRID_SPAN; var_s2++) {
                    for (var_s0 = 0; var_s0 < BLOCKS_GRID_SPAN; var_s0++) {
                        xTemp = gMapCurrentStreamCoordsX + var_s0;
                        zTemp = gMapCurrentStreamCoordsZ + var_s2;
                        if (temp_a3[var_s3] == -3) {
                            if (map_func_800485FC(var_s0, var_s2, xTemp, zTemp, var_s7) == 0) {
                                temp_a3[var_s3] = -2;
                            } else {
                                D_800B9714[var_s3] = var_s5++;
                            }
                        }
                        var_s3++;
                    }
                }
            }
        }
        var_s2 = TRUE;
        for (var_s3 = gMapNumStreamMaps - 1; var_s3 >= 0; var_s3--) {
            if ((s8) gMapStreamMapTable[var_s3].unk06 == 0) {
                if (gMapStreamMapTable[var_s3].header != NULL) {
                    var_s1 = gMapStreamMapTable[var_s3].mapID;
                    map_init_obj_setup_list(gMapStreamMapTable[var_s3].header, &gMapObjSetupLists[var_s1], var_s1, 1);
                    mmFree(gMapStreamMapTable[var_s3].header);
                    gLoadedMapsDataTable[var_s1] = NULL;
                }
                gMapStreamMapTable[var_s3].header = NULL;
                gMapStreamMapTable[var_s3].mapID = -1;
            }
            if (var_s2 != FALSE) {
                if (gMapStreamMapTable[var_s3].header == NULL) {
                    gMapNumStreamMaps -= 1;
                } else {
                    var_s2 = FALSE;
                }
            }
        }
        for (var_s3 = 0; var_s3 < var_fp; var_s3++) {
            block_free(sp84[var_s3].unk4);
        }
        func_8004530C();
    }
    map_update_objects_streaming(sp294);
    gTrackFlags &= ~TRACKFLAG_LAYER_CHANGED;
}

void map_increment_layer(void) {
    gMapLayer += 1;
    if (gMapLayer >= 3) {
        gMapLayer = 2;
    }
    gTrackFlags |= TRACKFLAG_LAYER_CHANGED;
}

void map_decrement_layer(void) {
    gMapLayer -= 1;
    if (gMapLayer < -2) {
        gMapLayer = -2;
    }
    gTrackFlags |= TRACKFLAG_LAYER_CHANGED;
}

void map_check_block_grid(s32 gridX, s32 gridZ, s32* arg2, s32* arg3, s32* arg4, s32* arg5, s32 layer, s32 checkVis, s32 streamMapIdx) {
    MapHeader* mapHeader;
    s32 temp_t6;
    s32 temp_v1_2;
    u32 temp;
    u32* var_t2;
    u32* var_t3;
    u32 *var_v0;
    Struct_D_800B9768_unk4* temp_v1;

    temp_v1 = &D_800B9768.unk4[gMapStreamMapTable[streamMapIdx].mapID];
    mapHeader = gMapStreamMapTable[streamMapIdx].header;
    gridX -= temp_v1->xMin;
    gridZ -= temp_v1->zMin;
    if (streamMapIdx == -1) {
        temp_v1_2 = 1;
        arg2[1] = temp_v1_2;
        arg2[3] = temp_v1_2;
        arg2[2] = -1;
        arg2[0] = -1;
        arg3[3] = -1;
        arg3[2] = 0;
        arg3[1] = 0;
        arg3[0] = 0;
        arg4[3] = -1;
        arg4[2] = 0;
        arg4[1] = 0;
        arg4[0] = 0;
        arg5[3] = -1;
        arg5[2] = 0;
        arg5[1] = 0;
        arg5[0] = 0;
        if (layer != 0) {
            arg2[3] = -2;
        }

        return;
    }

    if (checkVis != 0) {
        var_t2 = (u32 *)mapHeader->grid_A2_ptr;
        var_t3 = (u32 *)mapHeader->grid_B2_ptr;
    } else {
        var_t2 = (u32 *)mapHeader->grid_A1_ptr;
        var_t3 = (u32 *)mapHeader->grid_B1_ptr;
    }
    temp_t6 = ((mapHeader->gridSizeX * (gridZ)) + (gridX)) << 1;
    if (layer == 0) {
        var_v0 = var_t2;
        var_v0 += temp_t6;
        temp_v1_2 = var_v0[0];
        arg2[0] = ((temp_v1_2 >> 0xC) & 0xF) - 7;
        arg2[2] = ((temp_v1_2 >> 8) & 0xF) - 7;
        arg2[1] = ((temp_v1_2 >> 4) & 0xF) - 7;
        arg2[3] = (temp_v1_2 & 0xF) - 7;
        temp_v1_2 >>= 0x10;
        arg3[0] = ((temp_v1_2 >> 0xC) & 0xF);
        arg3[0] -= 7;
        arg3[2] = ((temp_v1_2 >> 8) & 0xF) - 7;
        arg3[1] = ((temp_v1_2 >> 4) & 0xF) - 7;
        arg3[3] = (temp_v1_2 & 0xF) - 7;
        temp_v1_2 = var_v0[1];
        arg4[0] = ((temp_v1_2 >> 0xC) & 0xF) - 7;
        arg4[2] = ((temp_v1_2 >> 8) & 0xF) - 7;
        arg4[1] = ((temp_v1_2 >> 4) & 0xF) - 7;
        arg4[3] = (temp_v1_2 & 0xF) - 7;
        temp_v1_2 >>= 0x10;
        arg5[0] = ((temp_v1_2 >> 0xC) & 0xF) - 7;
        arg5[2] = ((temp_v1_2 >> 8) & 0xF) - 7;
        arg5[1] = ((temp_v1_2 >> 4) & 0xF) - 7;
        arg5[3] = (temp_v1_2 & 0xF) - 7;
        return;
    }
    arg2[3] = -1;
    arg2[2] = 0;
    arg2[1] = -1;
    arg2[0] = 0;
    arg3[3] = -1;
    arg3[2] = 0;
    arg3[1] = -1;
    arg3[0] = 0;
    arg4[3] = -1;
    arg4[2] = 0;
    arg4[1] = -1;
    arg4[0] = 0;
    arg5[3] = -1;
    arg5[2] = 0;
    arg5[1] = -1;
    arg5[0] = 0;
    temp_v1_2 = mapHeader->blockIDs_ptr[temp_t6 >> 1];
    if ((temp_v1_2 & 0x7F) != 0x7F) {
        temp_v1_2 = var_t3[(((temp_v1_2 & 0x7F) << 2) + layer) - 1];
        arg2[0] = ((temp_v1_2 >> 0xC) & 0xF) - 7;
        arg2[2] = ((temp_v1_2 >> 8) & 0xF) - 7;
        arg2[1] = ((temp_v1_2 >> 4) & 0xF) - 7;
        arg2[3] = (temp_v1_2 & 0xF) - 7;
        temp_v1_2 >>= 0x10;
        arg3[0] = ((temp_v1_2 >> 0xC) & 0xF) - 7;
        arg3[2] = ((temp_v1_2 >> 8) & 0xF) - 7;
        arg3[1] = ((temp_v1_2 >> 4) & 0xF) - 7;
        arg3[3] = (temp_v1_2 & 0xF) - 7;
    }
}

void func_80047710(s32 arg0, s32 arg1, s32 arg2) {
}

void func_80047724(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
}

void map_func_8004773C(void) {
    s32 i;
    s32 j;
    s32 playerno;
    PlayerLocation* savedPlayerLocation;
    Camera* camera;
    Object* player;
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
    gDLL_3_Animation->vtbl->func0();
    camera_apply_alternate_trigger();
    camera_apply_alternate_trigger();
    func_80053300();

    for (i = 0; i < MAP_LAYER_COUNT; i++) {
        currentT1 = gBlockIndices[i];
        currentCell = gDecodedGlobalMap[i];
        for (j = 0; j < BLOCKS_GRID_TOTAL_CELLS; j++) {
            currentT1[j] = -1;
            currentCell[j].trkBlkIndex = -1;
        }
    }

    for (i = 0; i < MAX_BLOCKS; i++) {
        gLoadedBlockIds[i] = -1;
        gLoadedBlocks[i] = 0;
    }

    gLoadedBlockCount = 0;
    gMapNumStreamMaps = 0;
    gBlockColorTableLength = 0;
    playerno = gDLL_29_Gplay->vtbl->get_playerno();
    savedPlayerLocation = gDLL_29_Gplay->vtbl->get_player_saved_location();
    gMapCurrentStreamCoordsX = floor_f(savedPlayerLocation->vec.x / BLOCKS_GRID_UNIT_F);
    gMapCurrentStreamCoordsZ = floor_f(savedPlayerLocation->vec.z / BLOCKS_GRID_UNIT_F);
    Vec3_Int_array->f.x = savedPlayerLocation->vec.x;
    Vec3_Int_array->f.y = savedPlayerLocation->vec.y;
    Vec3_Int_array->f.z = savedPlayerLocation->vec.z;
    Vec3_Int_array->i = 1;
    D_80092A60 = gMapCurrentStreamCoordsX * BLOCKS_GRID_UNIT;
    D_80092A64 = gMapCurrentStreamCoordsZ * BLOCKS_GRID_UNIT;
    gWorldX = D_80092A60;
    // @fake
    if (0) { }
    gWorldZ = D_80092A64;
    D_800B4A50 = -1;
    D_800B4A54 = -1;
    gMapLayer = savedPlayerLocation->mapLayer;
    gDLL_24_Waterfx->vtbl->func_24C();
    gDLL_11_Newlfx->vtbl->func1();
    gDLL_57->vtbl->func0();
    gDLL_58->vtbl->func0();
    gDLL_15_Projgfx->vtbl->func0();
    gDLL_14_Modgfx->vtbl->func0();
    gDLL_13_Expgfx->vtbl->func0();
    gDLL_17_partfx->vtbl->func0();
    gDLL_12_Minic->vtbl->func4();
    gDLL_12_Minic->vtbl->func1();
    gDLL_8->vtbl->func1();
    gDLL_7_Newday->vtbl->func1();
    gDLL_9_Newclouds->vtbl->func1();
    gDLL_10_Newstars->vtbl->func0();
    footsteps_init();
    gTrackFlags &= (TRACKFLAG_DISABLE_Z_BUFFER | TRACKFLAG_SKY);
    gTrackFlags |= (TRACKFLAG_BLOCK_LIGHTING | TRACKFLAG_UNK100 | TRACKFLAG_SHADOWS | TRACKFLAG_SKY_OBJECTS | TRACKFLAG_ANTI_ALIAS);
    gTrackFlags |= (TRACKFLAG_UPDATE_STREAMING_IMMEDIATE | TRACKFLAG_UNK4);
    map_func_80046B58(savedPlayerLocation->vec.x, savedPlayerLocation->vec.y, savedPlayerLocation->vec.z);
    gTrackFlags &= ~TRACKFLAG_UNK4;
    func_800591EC();
    func_80023628();
    D_800B4A58 = 0;

    camera = get_main_camera();
    camera->srt.transl.x = savedPlayerLocation->vec.x;
    camera->srt.transl.y = savedPlayerLocation->vec.y;
    camera->srt.transl.z = savedPlayerLocation->vec.z;

    player = get_player();
    if ((D_800B4A5E == -2) && (player != NULL) && ((playerno == PLAYER_SABRE) || (playerno == PLAYER_KRYSTAL))) {
        sp40 = gDLL_29_Gplay->vtbl->func_F60();
        sp3C = gDLL_29_Gplay->vtbl->func_FA8();
        sp38 = gDLL_29_Gplay->vtbl->func_FE8()->actionNums;
        if (D_800B4A5E == -2) {
            if (sp40->unk0 != -1) {
                func_80000608(player, player, sp40->unk0, 0, 0, 0);
            }
            if (sp40->unk2 != -1) {
                func_80000608(player, player, sp40->unk2, 0, 0, 0);
            }
            if (sp40->unk4 != -1) {
                func_80000608(player, player, sp40->unk4, 0, 0, 0);
            }
            if (sp40->unk6 != -1) {
                func_80000608(player, player, sp40->unk6, 0, 0, 0);
            }
            if (sp40->unk8 != -1) {
                func_80000608(player, player, sp40->unk8, 0, 0, 0);
            }
            if (sp40->unkA != -1) {
                func_80000608(player, player, sp40->unkA, 0, 0, 0);
            }
            if (sp40->unkC != -1) {
                func_80000608(player, player, sp40->unkC, 0, 0, 0);
            }
            if (sp40->unkE != -1) {
                func_80000608(player, player, sp40->unkE, 0, 0, 0);
            }
            func_8001EBD0(sp40->unk10 & 1);
            if (sp3C->unk4 != -1) {
                func_800009C8(player, player, sp3C->unk4, 0);
            }
            if (sp3C->unk6 != -1) {
                func_800009C8(player, player, sp3C->unk6, 0);
            }
            if (sp3C->unk8 != -1) {
                func_800009C8(player, player, sp3C->unk8, 0);
            }
            gDLL_12_Minic->vtbl->func6(sp3C->unk3C & 1);
            for (i = 0; i < 4; i++) {
                if (sp38[i] != -1) {
                    // last 3 args: "track.c", 0x17b8, "plMusicactions->actions[i]" (default.dol)
                    gDLL_5_AMSEQ2->vtbl->set(NULL, sp38[i], 0, 0, 0);
                }
            }
            if (sp40->unk12 != -1) {
                gDLL_2_Camera->vtbl->change_mode(0, sp40->unk12);
            }
            sp4C.parent = NULL;
            sp4C.srt.transl.x = 0.0f;
            sp4C.srt.transl.y = 0.0f;
            sp4C.srt.transl.z = 0.0f;
            sp4C.globalPosition.x = 0.0f;
            sp4C.globalPosition.y = 0.0f;
            sp4C.globalPosition.z = 0.0f;
            if (sp3C->unkA != -1) {
                sp4C.srt.transl.x = sp3C->unk10;
                sp4C.srt.transl.y = sp3C->unk14;
                sp4C.srt.transl.z = sp3C->unk18;
                func_80000860(&sp4C, player, sp3C->unkA, 0);
            }
            if (sp3C->unkC != -1) {
                sp4C.srt.transl.x = sp3C->unk1C;
                sp4C.srt.transl.y = sp3C->unk20;
                sp4C.srt.transl.z = sp3C->unk24;
                func_80000860(&sp4C, player, sp3C->unkC, 0);
            }
            if (sp3C->unkE != -1) {
                sp4C.srt.transl.x = sp3C->unk28;
                sp4C.srt.transl.y = sp3C->unk2C;
                sp4C.srt.transl.z = sp3C->unk30;
                func_80000860(&sp4C, player, sp3C->unkE, 0);
            }
            gDLL_7_Newday->vtbl->func9(sp3C->unk0);
        }
    } else {
        gDLL_7_Newday->vtbl->func9(43000.0f);
        gDLL_12_Minic->vtbl->func6(1);
    }
}

s32 map_get_type(void) {
    return gMapType;
}

void map_func_80048034(void) {
    gMapType = MAPTYPE_REGULAR;
    gMobileMapID = 0;
    gMobileMapUnknown = 0;
}

void map_func_80048054(s32 mapID, s32 arg1, f32* arg2, f32* arg3, f32* arg4, s8* arg5) {
    MapInfo *sp64;
    s32 mapBinSize;
    u32 mapBinOffset;
    s32 offset;
    s32 group;
    s32 var_v0;
    SetupPoint_Setup* objsetup;
    MapHeader *map;
    MapsTabStruct *mapsTab;
    s32 mapInfoListLength;

    offset = 0;
    if (D_800B9768.unkC[mapID] == -0x80) {
        map_read_layout(&D_800B9768.unk4[mapID], &D_800B9768.unk10->unk0[mapID], 0, 0, mapID);
        D_800B9768.unkC[mapID] = 0;
    }
    *arg5 = D_800B9768.unkC[mapID];
    if (mapID == 1) {
        *arg2 = 0.0f;
        *arg3 = 0.0f;
        *arg4 = 0.0f;
    } else {
        mapBinOffset = READ_MAPS_TAB(mapID, 0);
        mapBinSize = READ_MAPS_TAB(mapID, 7) - mapBinOffset;
        map = mmAlloc(mapBinSize, ALLOC_TAG_TRACK_COL, ALLOC_NAME("trk:map"));
        queue_load_file_region_to_ptr((void *)map, MAPS_BIN, mapBinOffset, mapBinSize);
        map->objectInstanceFile_ptr = (ObjSetup *) (READ_MAPS_TAB(mapID, 4) + (s8 *)map - mapBinOffset);
        map->originWorldX = (D_800B9768.unk4[mapID].xMin + map->originOffsetX) * BLOCKS_GRID_UNIT_F;
        map->originWorldZ = (D_800B9768.unk4[mapID].zMin + map->originOffsetZ) * BLOCKS_GRID_UNIT_F;
        objsetup = (SetupPoint_Setup*)map->objectInstanceFile_ptr;
        while (offset < map->objectInstancesFileLength) {
            if ((objsetup->base.objId == OBJ_setuppoint) && (arg1 == objsetup->unk19)) {
                *arg2 = objsetup->base.x + map->originWorldX;
                *arg3 = objsetup->base.y;
                *arg4 = objsetup->base.z + map->originWorldZ;
                gDLL_29_Gplay->vtbl->set_map_setup(mapID, objsetup->mapSetupID);
                for (group = 0; group < 32; group++) {
                    if ((objsetup->objGroupStatusBits >> group) & 1) {
                        // enable
                        gDLL_29_Gplay->vtbl->set_obj_group_status(mapID, group, -1);
                    } else {
                        // disable
                        gDLL_29_Gplay->vtbl->set_obj_group_status(mapID, group, -2);
                    }
                }
                break;
            }
            offset += objsetup->base.quarterSize * 4;
            objsetup = (SetupPoint_Setup*)((s8*)objsetup + objsetup->base.quarterSize * 4);
        }
        mmFree(map);
    }
    mapInfoListLength = (u32)get_file_size(MAPINFO_BIN) / sizeof(MapInfo);
    if ((mapID < 0) || (mapID >= mapInfoListLength)) {
        gMapType = MAPTYPE_REGULAR;
    } else {
        sp64 = (MapInfo*)gMapReadBuffer;
        queue_load_file_region_to_ptr((void** ) sp64, MAPINFO_BIN, mapID * sizeof(MapInfo), sizeof(MapInfo));
        gMapType = sp64->type;
    }
    gMobileMapUnknown = 0;
    if (gMapType == MAPTYPE_MOBILE) {
        gMobileMapID = mapID;
        gMobileMapUnknown = sp64->mobileMapUnknown;
    }
}

/** read_mapinfo_of_map_at_xz */
void map_func_800483BC(f32 worldX, f32 worldY, f32 worldZ) {
    s32 mapID;
    s32 mapInfoCount;
    MapInfo* mapInfo;

    mapID = map_world_xz_to_map_id(worldX, worldZ);
    mapInfoCount = get_file_size(MAPINFO_BIN) / sizeof(MapInfo);

    if (mapID < 0 || !(mapID < mapInfoCount)) {
        gMapType = MAPTYPE_REGULAR;
    } else {
        mapInfo = (MapInfo *)gMapReadBuffer;
        queue_load_file_region_to_ptr((void *) mapInfo, MAPINFO_BIN, mapID * (sizeof(MapInfo)), sizeof(MapInfo));
        gMapType = mapInfo->type;
    } 
    
    gMobileMapUnknown = 0;
    
    //Set values if it's a "mobile map" (the Galleon, "wctemplelift", etc)
    if (gMapType == MAPTYPE_MOBILE) {
        gMobileMapID = mapID;
        gMobileMapUnknown = mapInfo->mobileMapUnknown;
    }
}

s16 map_get_mobile_map_vars(s32* outMapID) {
    if (outMapID) {
        *outMapID = (s32) gMobileMapID;
    }
    return gMobileMapUnknown;
}

s8 map_get_layer(void) {
    return gMapLayer;
}

void map_func_800484A8(void) {
    s32 i;
    s32 j;
    s8* var_s1;

    mmSetDelay(0);
    func_80012B54(1, 0);
    for (i = 0; i < MAP_LAYER_COUNT; i++) {
        var_s1 = gBlockIndices[i];
        for (j = 0; j < 256; j++) {
            block_free(var_s1[j]);
        }
    }
    gLoadedBlockCount = 0;
    obj_free_all();
    for (j = 0; j < 120; j++) {
        if (gLoadedMapsDataTable[j] != NULL) {
            mmFree(gLoadedMapsDataTable[j]);
            gLoadedMapsDataTable[j] = NULL;
        }
    }
    gDLL_4_Race->vtbl->func0();
    gDLL_26_Curves->vtbl->func_18();
    gMapNumStreamMaps = 0;
    gWorldX = 0.0f;
    gWorldZ = 0.0f;
    gDLL_9_Newclouds->vtbl->func2(-1, 0);
    gDLL_12_Minic->vtbl->func4();
    mmSetDelay(2);
}

s32 map_func_800485FC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GlobalMapCell* currentMap;
    s16 blockID;
    s32 fieldIndex;
    s32 i;
    s8* currentBlockIndices;

    fieldIndex = GRID_INDEX(arg1, arg0);
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

static const char str_8009a688[] = "Blocksize error(1): %d should be %d\n";
void block_load(s32 id, s32 param_2, s32 globalMapIdx, u8 queue) {
    s32 texIdx;
    s32 binOffset;
    s32 binSize;
    s32 size;
    s32 vtxIdx;
    Vtx_t* verts;
    Block* block;
    BlockShape* shape;
    BlockVertex* fileVerts;
    BlockVertex* fileVertsEnd;
    u32 addr;
    s32 pad[3];
    s32 tempLoadAddr;

    binOffset = gFile_BLOCKS_TAB[id];
    binSize = gFile_BLOCKS_TAB[id + 1] - binOffset;
    read_file_region(BLOCKS_BIN, gMapReadBuffer, binOffset, 0x10);
    size = ((s32*)gMapReadBuffer)[0];
    size += hits_get_size(id) + 8;
    block = mmAlloc(size, ALLOC_TAG_TRACK_COL, NULL);
    if (block == NULL) {
        return;
    }

    if (0) { if ((s32)&size) {} } // @fake

    tempLoadAddr = (((u32)block + size) - binSize) - 0x10;
    tempLoadAddr -= tempLoadAddr % 16;
    read_file_region(BLOCKS_BIN, (void*)tempLoadAddr, binOffset, binSize);
    rarezip_uncompress(((u8*)tempLoadAddr) + 4, (u8*)block, size);
    block->vertices = (BlockVertex*)((u32)block->vertices + (u32)block);
    block->encodedTris = (EncodedTri*)((u32)block->encodedTris + (u32)block);
    block->shapes = (BlockShape*)((u32)block->shapes + (u32)block);
    block->ptr_faceEdgeVectors = (s16*)((u32)block->ptr_faceEdgeVectors + (u32)block);
    block->materials = (BlocksMaterial*)((u32)block->materials + (u32)block);
    tex_set_alloc_tag(ALLOC_TAG_TRACKTEX_COL);
    for (texIdx = 0; texIdx < block->materialCount; texIdx++) {
        block->materials[texIdx].texture = tex_load(-((u32)block->materials[texIdx].texture | 0x8000), queue);
    }
    tex_set_alloc_tag(ALLOC_TAG_TEX_COL);
    block_setup_vertices(block);
    addr = (u32)block;
    addr += block->modelSize;
    block->gdlGroups = (Gfx*)addr;
    block_setup_gdl_groups(block);
    addr += (3 * block->shapeCount * sizeof(Gfx));
    block_color_table_add_block(block);
    if (block->vtxFlags & 8) {
        addr = mmAlign8(addr);
        fileVerts = block->vertices;
        block->vertices2[0] = (Vtx_t*)addr;
        addr += (sizeof(Vtx_t) * block->vtxCount);
        block->vertices2[1] = (Vtx_t*)addr;
        addr += (sizeof(Vtx_t) * block->vtxCount);
        
        verts = block->vertices2[0];
        vtxIdx = 0;
        shape = block->shapes;
        fileVertsEnd = fileVerts;
        if (block->vertices) {} // @fake
        if (block->vtxCount && block->vtxCount){} // @fake
        fileVertsEnd += block->vtxCount;
        while (fileVerts < fileVertsEnd) {
            if (shape->flags & RENDER_UNK20000000) {
                verts->ob[0] = (f32) fileVerts->ob[0];
                verts->ob[1] = (fileVerts->ob[1] - block->minY) * 20.0f;
                verts->ob[2] = (f32) fileVerts->ob[2];
            } else {
                verts->ob[0] = fileVerts->ob[0];
                verts->ob[1] = fileVerts->ob[1];
                verts->ob[2] = fileVerts->ob[2];
            }
            verts->cn[0] = fileVerts->cn[0];
            verts->cn[1] = fileVerts->cn[1];
            verts->cn[2] = fileVerts->cn[2];
            verts->cn[3] = fileVerts->cn[3];
            verts->tc[0] = fileVerts->tc[0];
            verts->tc[1] = fileVerts->tc[1];
            verts->flag = fileVerts->flag;
            vtxIdx += 1;
            fileVerts += 1;
            verts += 1;
            if (vtxIdx >= (shape + 1)->vtxBase) {
                shape += 1;
            }
        }
        bcopy(block->vertices2[0], block->vertices2[1], sizeof(Vtx_t) * block->vtxCount);
    } else {
        block->vertices2[0] = (Vtx_t*)block->vertices;
        block->vertices2[1] = (Vtx_t*)block->vertices;
    }
    addr = mmAlign4(addr);
    block->texAnims = (BlockTextureAnimInstance*)addr;
    addr += block_setup_texture_anims(block);
    addr = mmAlign2(addr);
    block->xzBitmap = (s16*)addr;
    addr += block->unk34 * 2;
    block_setup_xz_bitmap(block);
    addr = mmAlign8(addr);
    addr = (u32)block_load_hits(block, id, queue, (HitsLine*)addr);

    if (queue != 0) {
        queue_block_emplace(1, (u32* ) block, (u8*) id, param_2, globalMapIdx);
    } else {
        block_emplace(block, id, param_2, globalMapIdx);
    }
}

void block_color_table_add_block(Block* block) {
    s32 targetVertexIndex;
    s32 vertexIndex;
    s32 result;
    s32 i;
    BlockShape* currentShape;

    for (i = 0; i < block->shapeCount; i++) {
        currentShape = &block->shapes[i];
        if (currentShape->flags & (RENDER_UNK20 | RENDER_MIPMAPS | RENDER_UNK2000 | RENDER_UNK400000 | RENDER_UNK800000 |
                RENDER_UNK1000000 | RENDER_UNK2000000 | RENDER_UNK4000000 | RENDER_UNK40000000)) {
            vertexIndex = currentShape->vtxBase;
            targetVertexIndex = currentShape[1].vtxBase;
            while (vertexIndex < targetVertexIndex) {
                result = block_color_table_add(
                    block->vertices[vertexIndex].cn[0],
                    block->vertices[vertexIndex].cn[1],
                    block->vertices[vertexIndex].cn[2],
                    block->shapes[i].envColourMode
                );
                if (result != -1) {
                    block->vertices[vertexIndex].flag =  ((s16)block->vertices[vertexIndex].flag | ((result & 0xFF) * 4));
                }
                vertexIndex++;
            }
        }
    }
}

static const char str_8009a6b0[] = "COLOUR TABLE: Attempt to free invalid entry\n";
void block_color_table_free_block(Block* block) {
    s32 index;
    s32 i;
    s32 targetVertexIndex;
    s32 vertexIndex;

    for (i = 0; i < block->shapeCount; i++) {
        if (block->shapes[i].flags & (RENDER_UNK20 | RENDER_MIPMAPS | RENDER_UNK2000 | RENDER_UNK400000 | RENDER_UNK800000 |
                RENDER_UNK1000000 | RENDER_UNK2000000 | RENDER_UNK4000000 | RENDER_UNK40000000)) {
            vertexIndex = block->shapes[i].vtxBase;
            targetVertexIndex = block->shapes[i + 1].vtxBase;
            while (vertexIndex < targetVertexIndex) {
                index = ((s16)block->vertices[vertexIndex].flag >> 2) & 0xFF;
                if (gBlockColorTable[index].refCount != 0) {
                    gBlockColorTable[index].refCount--;
                    if (gBlockColorTable[index].refCount == 0 && (index + 1) == gBlockColorTableLength) {
                        while (gBlockColorTable[index].refCount == 0 && index >= 0) {
                            gBlockColorTableLength -= 1;
                            index--;
                        }
                    }
                }
                vertexIndex++;
            }
        }
    }
}

s32 block_color_table_get(u8 r, u8 g, u8 b, u8 a) {
    s32 index;
    
    for (index = 0; index < gBlockColorTableLength; index++){
        if ((r == gBlockColorTable[index].r) &&
            (g == gBlockColorTable[index].g) &&
            (b == gBlockColorTable[index].b) &&
            (a == gBlockColorTable[index].a)){
            return index;
        }
    }
    
    return -1;
}

s32 block_color_table_add(u8 r, u8 g, u8 b, u8 a) {
    s32 i;

    // try to get the index from another function
    i = block_color_table_get(r, g, b, a);

    // if we got something valid use it to increment unk8
    // then return
    if (i != -1) {
        gBlockColorTable[i].refCount++;
    } else {
        // otherwise loop through SomeArray until
        // we find the first unk8 that is zero, or the final index
        for (i = 0; i < gBlockColorTableLength; i++) {
            if (gBlockColorTable[i].refCount == 0) {
                break;
            }
        }

        gBlockColorTable[i].r2 = r;
        gBlockColorTable[i].r = gBlockColorTable[i].r2;
        gBlockColorTable[i].g2 = g;
        gBlockColorTable[i].g = gBlockColorTable[i].g2;
        gBlockColorTable[i].b2 = b;
        gBlockColorTable[i].b = gBlockColorTable[i].b2;

        gBlockColorTable[i].a = a;
        gBlockColorTable[i].refCount = 1;

        if (i == gBlockColorTableLength) {
            gBlockColorTableLength++;
        }
    }

    return i;
}

void block_color_table_tick(void) {
    s32 i;
    u8 r, g, b;

    func_8001F81C(&r, &g, &b);

    for (i = 0; i < gBlockColorTableLength; i++) {
        if (gBlockColorTable[i].a != 0xFE) {
            gBlockColorTable[i].r2 = (gBlockColorTable[i].r * r) >> 8;
            gBlockColorTable[i].g2 = (gBlockColorTable[i].g * g) >> 8;
            gBlockColorTable[i].b2 = (gBlockColorTable[i].b * b) >> 8;
        }
    }
}

void block_emplace(Block *block, s32 id, s32 param_3, s32 globalMapIdx) {
    s32 slot;
    s8 *ptr;

    for (slot = 0; slot < gLoadedBlockCount; slot++) {
        if (gLoadedBlockIds[slot] == -1) {
            break;
        }
    }

    if (slot == gLoadedBlockCount) {
        gLoadedBlockCount++;
        if (gLoadedBlockCount == MAX_BLOCKS) {
            STUBBED_PRINTF("trackLoadBlockEnd: track block overrun\n");
        }
    }

    ptr = gBlockIndices[globalMapIdx];
    ptr[param_3] = slot;
    
    gLoadedBlocks[slot] = block;
    gLoadedBlockIds[slot] = id;
    gBlockRefCounts[slot] = 1;

    if (block->unk3E != 0) {
        block_compute_vertex_colors(block, 0, 0, 1);
    }

    func_80058F3C();
}

void block_setup_gdl_groups(Block *block) {
    s32 i;

    for (i = 0; i < block->shapeCount; i++) {
        BlockShape *shape;
        Texture *texture;
        s32 texFlags = 0;
        s32 aa;
        s32 flags;
        Gfx *mygdl;

        block->shapes[i].texScrollerID = 0xff;
        
        shape = &block->shapes[i];
        flags = shape->flags;
        aa = flags & RENDER_ANTI_ALIASING;

        if (shape->materialIndex == 0xff) {
            texture = NULL;
        } else {
            texture = block->materials[shape->materialIndex].texture;
            if (texture != NULL) {
                texFlags = texture->flags;
            }
        }

        if ((flags & RENDER_DECAL_SIMPLE) == 0) {
            if ((flags & RENDER_UNK1000000) != 0) {
                flags |= (RENDER_Z_COMPARE | RENDER_FOG_ACTIVE | RENDER_UNK10);
            }
        }

        if (texFlags & RENDER_CUTOUT) {
            flags |= RENDER_Z_COMPARE;
        }

        if (aa != 0) {
            flags &= ~RENDER_ANTI_ALIASING;
        } else {
            flags |= RENDER_ANTI_ALIASING;
        }

        flags |= RENDER_Z_COMPARE;

        if (flags & RENDER_DECAL_SIMPLE) {
            if (flags & RENDER_SUBSURFACE) {
                flags &= ~RENDER_SUBSURFACE;
                tex_disable_modes(RENDER_SEMI_TRANSPARENT);
            } else {
                if ((flags & RENDER_UNK2000) || (flags & RENDER_SEMI_TRANSPARENT) || (flags & RENDER_DECAL)) {
                    flags |= RENDER_SEMI_TRANSPARENT;
                } else {
                    tex_disable_modes(RENDER_SEMI_TRANSPARENT);
                }
            }
        }

        mygdl = &block->gdlGroups[i * 3];
        // only set modes, don't set texture!
        tex_gdl_set_texture_simple(&mygdl, texture, 
            flags | RENDER_NO_CULL, 
            TEX_FRAME(0),
            TRUE, // force
            TEXOPT_INVISIBLE | TEXOPT_SET_MODES | TEXOPT_SKIP_MODE_CACHE);

        if ((flags & RENDER_UNK2000) && texture != NULL && (texture->flags & (RENDER_COMPOSITE_BASE | RENDER_COMPOSITE_OVERLAY))) {
            mygdl = &block->gdlGroups[i * 3];
            gSPLoadGeometryMode(mygdl++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
            if (texture->flags & (RENDER_COMPOSITE_BASE | RENDER_COMPOSITE_OVERLAY)) {
                gDPSetCombineMode(
                    mygdl++, 
                    G_CC_DINO_BLENDTEX_ENV,
                    G_CC_DINO_LERP_FROM_SHADE2
                );
            } else {
                gDPSetCombineMode(
                    mygdl++, 
                    G_CC_DINO_BLEND_TEX_SHADE_ENVA,
                    G_CC_DINO_LERP_FROM_SHADE_INVA2
                );
            }
            gDPSetOtherMode(
                mygdl++,
                G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_AA_ZB_XLU_SURF2
            );
        }

        if (flags & RENDER_DECAL_SIMPLE) {
            tex_enable_modes(RENDER_SEMI_TRANSPARENT);
        }
    }
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/map/block_setup_vertices.s")
#else
void block_setup_vertices(Block *block) {
    s32 i;
    BlockShape *shape;
    s32 v0[3]; // spC4 - spCC
    BlockVertex *pverts;
    EncodedTri *ptri;
    EncodedTri *ptriend;
    s32 pad;
    f32 nx, ny, nz;
    s16 vx[3]; // spA0 - spA6 / spA8
    s16 vy[3]; // sp98 - sp9C
    s16 vz[3]; // sp90 - sp96
    s16 inx, iny, inz;
    s32 j;
    BlockVertex *verts[3];
    f32 mag;

    for (i = 0; i < block->shapeCount; i++)
    {
        shape = &block->shapes[i];
        pverts = shape->vtxBase + block->vertices;
        ptri = shape->triBase + block->encodedTris;
        if ((s32)verts) {}
        ptriend = &block->encodedTris[shape[1].triBase];

        while (ptri < ptriend)
        {
            v0[0] = ((u8*)ptri)[1];
            v0[1] = ((u8*)ptri)[2];
            v0[2] = ((u8*)ptri)[3];

            ptri->d0 = (v0[0] << 13) | (v0[1] << 7) | (v0[2] << 1);
            ptri->d1 = 0;

            verts[0] = &pverts[v0[0]];
            verts[1] = &pverts[v0[1]];
            verts[2] = &pverts[v0[2]];

            for (j = 0; j < 3; j++)
            {
                vx[j] = verts[j]->ob[0];
                vy[j] = verts[j]->ob[1];
                vz[j] = verts[j]->ob[2];
            }

            nx = (vy[0] * (vz[1] - vz[2])) + (vy[1] * (vz[2] - vz[0])) + (vy[2] * (vz[0] - vz[1]));
            ny = (vz[0] * (vx[1] - vx[2])) + (vz[1] * (vx[2] - vx[0])) + (vz[2] * (vx[0] - vx[1]));
            nz = (vx[0] * (vy[1] - vy[2])) + (vx[1] * (vy[2] - vy[0])) + (vx[2] * (vy[0] - vy[1]));
            mag = sqrtf(nx * nx + ny * ny + nz * nz);
            if (mag > 0.0f)
            {
                nx /= mag;
                ny /= mag;
                nz /= mag;
            }

            inx = nx * 8191.0f;
            iny = ny * 8191.0f;
            inz = nz * 8191.0f;

            ptri->d0 |= inx << 18;
            ptri->d1 |= (iny & 0x3fff) << 4;
            ptri->d1 |= inz << 18;
            ptri->d1 |= 0x1;
            ptri++;
        }

        shape = &block->shapes[i];
        if (shape->flags & RENDER_UNK10) {
            shape->flags |= RENDER_UNK800;
        }
    }
}
#endif

void block_free(s32 blockIndex) {
    Block *block;
    s32 i;
    u8 runtimeValue;
    s32* temp_a0_2;

    if (blockIndex < 0) {
        return;
    }
    
    gBlockRefCounts[blockIndex] -= 1;
    if (gBlockRefCounts[blockIndex] == 0) {
        block = gLoadedBlocks[blockIndex];
        block_color_table_free_block(block);
        gLoadedBlockIds[blockIndex] = -1;
        gLoadedBlocks[blockIndex] = NULL;
        if (block->numTexAnims != 0) {
            block_free_texture_anims(block);
        }

        //Loop over shapes and free them
        for (i = 0; i < block->shapeCount; i++){
            runtimeValue = (&block->shapes[i])->texScrollerID;
            if (runtimeValue != 0xFF) {
                block_texscroll_free(runtimeValue);
            }
        }

        //Loop over materials and free their textures
        for (i = 0; i < block->materialCount; i++){
            tex_free(block->materials[i].texture);
        }
        
        if ((u32*)block->unk1C != NULL) {
            mmFree((u32*)block->unk1C);
        }
        
        func_80058F3C();
        mmFree(block);
    }
}

u32 hits_get_size(s32 id) {
    u32 size = gFile_HITS_TAB[id + 1] - gFile_HITS_TAB[id];
    return size;
}

HitsLine* block_load_hits(Block *block, s32 blockID, u8 unused, HitsLine* hits_ptr) {
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
        }

        line = &block->ptr_hits_lines[lineIndex];
        if (line->Az < 0 || line->Bz < 0 || line->Az > BLOCKS_GRID_UNIT || line->Bz > BLOCKS_GRID_UNIT) {
            line->settingsB = 0x40;
        }
    }
                
    block->unk1C = 0;
    block->unk3C = 0;
    block->vtxFlags &= ~0x40;
    
    return hits_ptr;
}

void func_800499B4(){
}

/** Updates all the block texture scrollers */
void block_texscroll_tick(void) {
    BlockTextureScroller *scroll;
    s32 i;
    s32 dU;
    s32 dV;
    s32 dU_adjusted;
    s32 dV_adjusted;

    for (i = 0; i < MAX_TEXTURE_SCROLLERS; i++){
        if (sBlockTexScrollTable[i].refCount != 0) {
            scroll = &sBlockTexScrollTable[i];

            //Primary material
            dU_adjusted = scroll->uRemainderA;
            dV_adjusted = scroll->vRemainderA;
            dU = scroll->uSpeedA * gUpdateRate;
            dV = scroll->vSpeedA * gUpdateRate;
            dU_adjusted += dU;
            dV_adjusted += dV;
            //Store remainders to account for scroll progress lost to integer division
            scroll->uRemainderA = dU_adjusted & 3;
            scroll->vRemainderA = dV_adjusted & 3;
            scroll->uOffsetA += dU_adjusted >> 2; //dividing delta by 4 before it's applied
            scroll->vOffsetA += dV_adjusted >> 2;
            
            if (scroll->widthA < scroll->uOffsetA) {
                scroll->uOffsetA -= scroll->widthA;
            } else if (scroll->uOffsetA < -scroll->widthA) {
                scroll->uOffsetA += scroll->widthA;
            }
    
            if (scroll->heightA < scroll->vOffsetA) {
                scroll->vOffsetA -= scroll->heightA;
            } else if (scroll->vOffsetA < -scroll->heightA) {
                scroll->vOffsetA += scroll->heightA;
            }
    
            //Secondary blend material
            dU_adjusted = scroll->uRemainderB;
            dV_adjusted = scroll->vRemainderB;
            dU = scroll->uSpeedB * gUpdateRate;
            dV = scroll->vSpeedB * gUpdateRate;
            dU_adjusted += dU;
            dV_adjusted += dV;
            //Store remainders to account for scroll progress lost to integer division
            scroll->uRemainderB = dU_adjusted & 3;
            scroll->vRemainderB = dV_adjusted & 3;
            scroll->uOffsetB += dU_adjusted >> 2; //dividing delta by 4 before it's applied
            scroll->vOffsetB += dV_adjusted >> 2;
            
            if (scroll->widthB < scroll->uOffsetB) {
                scroll->uOffsetB -= scroll->widthB;
            } else if (scroll->uOffsetB < -scroll->widthB) {
                scroll->uOffsetB += scroll->widthB;
            }
    
            if (scroll->heightB < scroll->vOffsetB) {
                scroll->vOffsetB -= scroll->heightB;
            } else if (scroll->vOffsetB < -scroll->heightB) {
                scroll->vOffsetB += scroll->heightB;
            }
        }
    }
}

/** Setup texture scroll handler (reusing existing handlers where possible) 
  *
  * Block texture scrollers can scroll two materials independently.
  * MaterialA is the primary texture,
  * MaterialB is an optional blend texture, mostly used for texture-blended animated water
  */
s32 block_texscroll_add(s32 uSpeedA, s32 vSpeedA, s32 widthA, s32 heightA, s32 uSpeedB, s32 vSpeedB, s32 widthB, s32 heightB) {
    BlockTextureScroller* scroll;
    s32 index;
    s32 scrollHandlerID;

    //Iterate through scroll handlers
    scrollHandlerID = 0;
    while (1) {
        scroll = &sBlockTexScrollTable[scrollHandlerID];

        //Reuse an existing scroll handler if it has the same animation config as what's specified in args
        if (
            (uSpeedA == scroll->uSpeedA) &&
            (vSpeedA == scroll->vSpeedA) &&
            (widthA == scroll->widthA) &&
            (heightA == scroll->heightA) &&
            (uSpeedB == scroll->uSpeedB) &&
            (vSpeedB == scroll->vSpeedB) &&
            (widthB == scroll->widthB) &&
            (heightB == scroll->heightB)
        ) {
            scroll->refCount++;
            return scrollHandlerID;
        }
        scrollHandlerID++;

        //If no reusable handler was found, search through the handlers again to find an unused slot
        if (scrollHandlerID >= MAX_TEXTURE_SCROLLERS) {
            index = -1;
            for (scrollHandlerID = 0; scrollHandlerID < MAX_TEXTURE_SCROLLERS; scrollHandlerID++) {
                if (sBlockTexScrollTable[scrollHandlerID].refCount == 0) {
                    index = scrollHandlerID;
                    break;
                }
            }
            //Bail if no unreferenced scroller slot was found
            if (index == -1) {
                STUBBED_PRINTF("TEXSCROLL: table is full\n");
                return -1;
            }

            //If an unused slot was found, store the scroll's anim params and return its ID
            scroll = &sBlockTexScrollTable[index];
            scroll->uSpeedA = uSpeedA;
            scroll->vSpeedA = vSpeedA;
            scroll->widthA = widthA;
            scroll->heightA = heightA;
            scroll->uOffsetA = 0;
            scroll->vOffsetA = 0;
            scroll->uRemainderA = 0;
            scroll->vRemainderA = 0;
            scroll->uSpeedB = uSpeedB;
            scroll->vSpeedB = vSpeedB;
            scroll->widthB = widthB;
            scroll->heightB = heightB;
            scroll->uOffsetB = 0;
            scroll->vOffsetB = 0;
            scroll->uRemainderB = 0;
            scroll->vRemainderB = 0;
            scroll->refCount++;
            return index;
        }
    }
}

/** Setup texture scroll handler (using specific handler index) 
  *
  * Block texture scrollers can scroll two materials independently.
  * MaterialA is the primary texture,
  * MaterialB is an optional blend texture, mostly used for texture-blended animated water
  */
void block_texscroll_set(u32 scrollerID, s32 uSpeedA, s32 vSpeedA, s32 widthA, s32 heightA, s32 uSpeedB, s32 vSpeedB, s32 widthB, s32 heightB) {
    BlockTextureScroller *scroll;

    scroll = &sBlockTexScrollTable[scrollerID];

    //Primary material
    scroll->uSpeedA = uSpeedA;
    scroll->vSpeedA = vSpeedA;
    scroll->widthA = widthA;
    scroll->heightA = heightA; 

    //Secondary blend material
    scroll->uSpeedB = uSpeedB; 
    scroll->vSpeedB = vSpeedB; 
    scroll->widthB = widthB; 
    scroll->heightB = heightB; 
}

void block_texscroll_free(u32 id) {
    if (sBlockTexScrollTable[id].refCount > 0) {
        sBlockTexScrollTable[id].refCount--;
    } else {
        STUBBED_PRINTF("MISMATCH on global texscroll free\n");
    }
}

BlockTextureScroller* block_texscroll_get(s32 id) {
    return &sBlockTexScrollTable[id];
}

void block_texanim_tick(void) {
    s32 i;
    Texture *texture;

    for (i = 0; i != MAX_TEXTURE_ANIMS; i++) {
        if (gBlockTexAnimTable[i].refCount != 0) {
            texture = gBlockTexAnimTable[i].texture;

            if ((texture != NULL) && (texture->animDuration != 0x100)) {
                if (texture->animSpeed != 0) {
                    tex_animate(texture, (s32 *)&gBlockTexAnimTable[i].flags, (s32 *)&gBlockTexAnimTable[i].unk4);
                }
            }
        }
    }
}

s32 block_setup_texture_anims(Block* block) {
    s32 var_a1;
    s32 numTexAnims;
    s32 i;
    s32 j;
    s32 var_t0;
    s32 numSphereMappedShapes;
    s32 animatorID;
    s32 new_var;
    BlockShape* shape;
    BlockShape* temp_a3_2;

    numTexAnims = 0;
    numSphereMappedShapes = 0;
    for (i = 0; i < block->shapeCount; i++) {
        shape = &block->shapes[i];
        if (shape->flags & RENDER_SHAPE_SPHERE_MAP) {
            numSphereMappedShapes++;
        }
        if (shape->flags & RENDER_SHAPE_ANIMATED) {
            if (shape->animatorID != 0) {
                temp_a3_2 = shape;
                var_a1 = FALSE;
                for (j = 0; j < numTexAnims; j++) {
                    animatorID = temp_a3_2->animatorID;
                    if (block->texAnims[j].animatorID == (new_var = animatorID ^ 0 )) {
                        var_a1 = TRUE;
                        break;
                    }
                }

                animatorID = shape->animatorID;
                if (var_a1 == FALSE) {
                    var_a1 = shape->flags;
                    block->texAnims[numTexAnims].texanimID = block_texanim_add(block->materials[shape->materialIndex].texture, var_a1, animatorID);
                    block->texAnims[numTexAnims].animatorID = block->shapes[i].animatorID;
                    numTexAnims++;
                } else {
                    var_a1 = shape->flags;
                    block_texanim_add(block->materials[shape->materialIndex].texture, var_a1, animatorID);
                }
            }
        }
    }
    block->numSphereMappedShapes = numSphereMappedShapes;
    block->numTexAnims = numTexAnims;
    return numTexAnims * sizeof(BlockTextureAnimInstance);
}

void block_free_texture_anims(Block* block) {
    s32 index;
    u8 animatorID;
    BlockShape* shape;

    for (index = 0; index < block->shapeCount; index++) {
        shape = &block->shapes[index];
        if (shape->flags & RENDER_SHAPE_ANIMATED) {
            animatorID = shape->animatorID;
            if (animatorID) {
                block_texanim_free(block->materials[shape->materialIndex].texture, animatorID);
            }
        }
    }
}

s32 block_texanim_add(Texture* tex, u32 renderFlags, s32 animatorID) {
    s32 index;
    s32 indexOfUnref;
    
    // Check if an existing slot can be reused
    indexOfUnref = -1;
    for (index = 0; index < MAX_TEXTURE_ANIMS; index++) {
        if ((&gBlockTexAnimTable[index])->refCount != 0 && 
            tex == (&gBlockTexAnimTable[index])->texture && 
            animatorID == (&gBlockTexAnimTable[index])->animatorID) {
            
            indexOfUnref = index;
            break;
        }
    }
    
    if (indexOfUnref != -1) {
        // Already exists on list
        (&gBlockTexAnimTable[indexOfUnref])->refCount += 1;
        return indexOfUnref;
    }
    
    // Add to first empty slot
    indexOfUnref = -1;
    for (index = 0; index < MAX_TEXTURE_ANIMS; index++) {
        if ((&gBlockTexAnimTable[index])->refCount == 0) {
            indexOfUnref = index;
            break;
        }
    }
    
    if (indexOfUnref != -1) {
        gBlockTexAnimTable[indexOfUnref].refCount = 1;
        gBlockTexAnimTable[indexOfUnref].unk4 = 0;
        gBlockTexAnimTable[indexOfUnref].flags = renderFlags;
        gBlockTexAnimTable[indexOfUnref].texture = tex;
        gBlockTexAnimTable[indexOfUnref].animatorID = animatorID;
        return indexOfUnref;
    }

    // No empty slots :(
    STUBBED_PRINTF("TRACK ERROR: Global texanim overflow\n");
    return 0;
}

void block_texanim_free(Texture *tex, s32 animatorID) {
    s32 index;
    for (index = 0; index < MAX_TEXTURE_ANIMS; index++) {
        if (tex == gBlockTexAnimTable[index].texture && animatorID == gBlockTexAnimTable[index].animatorID) {
            if (gBlockTexAnimTable[index].refCount > 0) {
                gBlockTexAnimTable[index].refCount--;
            } else {
                STUBBED_PRINTF("MISMATCH on global texanim free\n");
            }
        }
    }
}

Texture* block_texanim_get_tex(s32 animatorID) {
    s32 blockIndex;
    Block *block;
    s32 textureIndex;
    
    for (blockIndex = 0; blockIndex < gLoadedBlockCount; blockIndex++) {
        block = gLoadedBlocks[blockIndex];
        
        if (block) {
            for (textureIndex = 0; textureIndex < block->numTexAnims; textureIndex++) {
                if (animatorID == (&block->texAnims[textureIndex])->animatorID) {
                    textureIndex = (&block->texAnims[textureIndex])->texanimID;
                    return gBlockTexAnimTable[textureIndex].texture;
                }
            }
        }
    }
    return NULL;
}

BlockTextureAnimInstance *block_texanim_get_instance(Block *block, s32 animatorID) {
    s32 i;

    for (i = 0; i < block->numTexAnims; i++) {
        if (block->texAnims[i].animatorID == animatorID) {
            return &block->texAnims[i];
        }
    }

    return NULL;
}

BlockTextureAnim *block_texanim_get(s32 idx) {
    return &gBlockTexAnimTable[idx];
}

void block_setup_xz_bitmap(Block* block) {
    s16 tempTriBase;
    s16 pad;
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
    s32 pad2;
    s32 i;
    BlockVertex *sp38[3];

    for (var_t2 = 0; var_t2 < block->shapeCount; var_t2++) {
        tempTriBase = block->shapes[var_t2 + 0].triBase;
        temp_a2 = block->shapes[var_t2 + 0].vtxBase;
        for (triIndex = tempTriBase, nextTriIndex = block->shapes[var_t2 + 1].triBase; triIndex < nextTriIndex; triIndex++) {
            var_t3 = -0x7D00;
            var_t4 = 0x7D00;
            var_t5 = -0x7D00;
            var_s0 = 0x7D00;
            sp38[0] = &block->vertices[(((s32)block->encodedTris[triIndex].d0 >> 0xD) & 0x1F) + temp_a2];
            sp38[1] = &block->vertices[(((s32)block->encodedTris[triIndex].d0 >> 7) & 0x1F) + temp_a2];
            sp38[2] = &block->vertices[(((s32)block->encodedTris[triIndex].d0 >> 1) & 0x1F) + temp_a2];
            for (i = 0; i < 3 ; i++) {
                pad = sp38[i]->ob[0];
                var_s1 = pad;
                temp_s4 = sp38[i]->ob[2];
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
            }
            for (var_s5 = 0, var_s1 = 1, var_s2 = 0; var_s2 < BLOCKS_GRID_UNIT; var_s2 += 80) {
                if (((var_s2 + 80) >= var_t4) && (var_t3 >= var_s2)) {
                    var_s5 |= var_s1;
                }
                var_s1 <<= 1;
            }
            for (var_s2 = 0; var_s2 < BLOCKS_GRID_UNIT; var_s2 += 80) {
                if (((var_s2 + 80) >= var_s0) && (var_t5 >= var_s2)) {
                    var_s5 |= var_s1;
                }
                var_s1 <<= 1;
            }
            block->xzBitmap[triIndex] = var_s5;
        }
    }
}

/**
  * uses the animator object's world position to find the relevant block
  */
s32 block_get_animator_vertex_count(Object* obj, u8 animatorID) {
    s32 i;
    s32 vertexCount;
    Block *block;
    BlockShape *shapes;

    block = map_get_block_by_index(map_world_coords_to_block_index(obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z));
    if (!block || !(block->vtxFlags & 8)) {
        return 0;
    }
    
    vertexCount = 0;
    shapes = block->shapes;
    for (i = 0; i < block->shapeCount; i++) {
        if (animatorID == (shapes[i]).animatorID) {
            vertexCount += shapes[i + 1].vtxBase - shapes[i].vtxBase;
        }
    }
    
    return vertexCount;
}

s32 block_get_animator_shape_count(Object* obj, u8 animatorID) {
    Block* block;
    s32 matches;
    s32 i;
    BlockShape *shapes;

    //Get object's local Blocks model
    block = map_get_block_by_index(map_world_coords_to_block_index(obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z));
    if ((block == NULL) || !(block->vtxFlags & 8)) {
        return 0;
    }

    //Iterate over Block's shapes, counting those with matching animatorID
    for (i = 0, matches = 0, shapes = block->shapes; i < block->shapeCount; i++) {
        if (animatorID == shapes[i].animatorID) {
            matches++;
        }
    }
    return matches;
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
    Object** mobileMapObjs;

    mobileMapObjs = obj_get_all_of_type(OBJTYPE_MOBILE_MAP, &count);
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
        obj = mobileMapObjs[i];

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

// official name: trackObjectControl
void map_update_objects_streaming(s32 arg0) {
    GlobalMapCell* var_a3;
    s32 i;
    MapHeader* temp_s6;
    ObjSetup* objSetup;
    Object* obj;
    Object* temp_s5;
    u32 temp_fp;
    s32 var_a1;
    s32 count;
    s32 var_a2;
    s32 unloadObj;
    s32 group;
    s32 var_s3;
    s32 var_v0;
    ObjSetup* var_s1_2;
    s32 temp_s7;
    s32 temp_s4;
    s16 var_s4;
    s16 sp70[5];
    u32 objGroupBits;
    Object** objList;
    s16 mapID;

    var_s4 = 0;
    for (i = 0; i < MAP_LAYER_COUNT; i++) {
        var_a2 = 0;
        var_a3 = gDecodedGlobalMap[i] + GRID_INDEX(7, 7);
        for (var_a2 = 0; var_a2 < 3; var_a2++) {
            if (var_a3->mapIDs[var_a2] >= 0 && var_a3->mapIDs[var_a2] < MAP_ID_MAX) {
                var_a1 = 0;
                if (gLoadedMapsDataTable[var_a3->mapIDs[var_a2]] != NULL) {
                    for (var_v0 = 0; var_v0 < var_s4; var_v0++) {
                        if (sp70[var_v0] == var_a3->mapIDs[var_a2]) {
                            var_a1 = 1;
                            break;
                        }
                    }
                    if (var_a1 == 0) {
                        /* default.dol
                        if (var_s4 >= 5) {
                            STUBBED_PRINTF("trackObjectControl: map overflow\n");
                        } else
                        */
                        sp70[var_s4++] = var_a3->mapIDs[var_a2];
                    }
                }
            }
        }
    }
    
    objList = get_world_objects(&i, &count);
    while (i < count) {
        unloadObj = FALSE;
        obj = objList[i];
        objSetup = obj->setup;
        i++;
        if (obj->mapID >= 0) {
            if (!(objSetup->loadFlags & OBJSETUP_LOAD_MANUAL)) {
                if (objSetup->loadFlags & OBJSETUP_LOAD_IN_MAP_OBJGROUP) {
                    if ((obj->group >= GROUP_NONE) && (map_should_obj_unload(obj) != 0)) {
                        unloadObj = TRUE;
                    } else if ((obj->mapID < MAP_ID_MAX) && (gLoadedMapsDataTable[obj->mapID] == NULL)) {
                        unloadObj = TRUE;
                    }
                } else {
                    if ((obj->group >= GROUP_NONE) && (map_should_obj_unload(obj) != 0)) {
                        unloadObj = TRUE;
                    } else if ((obj->mapID < MAP_ID_MAX) && (func_8004AEFC(obj->mapID, sp70, var_s4) == 0)) {
                        unloadObj = TRUE;
                    }
                }
            }
        }
        if (unloadObj) {
            if (gLoadedMapsDataTable[obj->mapID] != NULL) {
                if (obj->unkB2 >= 0) {
                    func_8004B710(obj->unkB2, obj->mapID, 0);
                }
            }
            if (obj->id == OBJ_IMSnowBike) {
                STUBBED_PRINTF(" Map not Loaded %i ", func_8004AEFC(obj->mapID, sp70, var_s4) == 0);
            }
            obj_destroy_object(obj);
            i -= 1;
            count -= 1;
        }
    }
    for (i = 0; i < MAP_ID_MAX; i++) {
        if (gLoadedMapsDataTable[i] != NULL) {
            objGroupBits = gDLL_29_Gplay->vtbl->world_get_obj_group_bits(i);
            if (objGroupBits != 0) {
                gDLL_29_Gplay->vtbl->world_disable_all_obj_groups(i);
                group = 0;
                while (objGroupBits != 0) {
                    if (objGroupBits & 1) {
                        func_8004B548(gLoadedMapsDataTable[i], i, group, NULL);
                    }
                    objGroupBits >>= 1;
                    group++;
                }
            }
        }
    }
    for (i = 0; i < var_s4; i++) {
        temp_s6 = gLoadedMapsDataTable[sp70[i]];
        if (temp_s6 != NULL) {
            var_s1_2 = (ObjSetup* ) temp_s6->objectInstanceFile_ptr;
            temp_s7 = temp_s6->unk19;
            var_s3 = 0;
            temp_fp = gMapObjSetupLists[sp70[i]].groupsStart + (s32)var_s1_2;
            while ((u32) var_s1_2 < (u32) temp_fp) {
                objSetup = var_s1_2;
                if ((var_s1_2->loadFlags & OBJSETUP_LOAD_IN_MAP_OBJGROUP)) {
                    break;
                }

                if ((map_check_some_mapobj_flag(var_s3, (u32) sp70[i]) == 0) && (map_should_stream_load_object(var_s1_2, 0, sp70[i]) != 0)) {
                    func_8004B710(var_s3, (u32) sp70[i], 1);
                    if (arg0 != 0) {
                        obj_create(var_s1_2, OBJINIT_STANDALONE, sp70[i], var_s3, NULL);
                    } else if (map_get_is_object_streaming_disabled() != 0) {
                        func_80012584(0x3E, 4, NULL, var_s1_2, sp70[i], var_s3, 0, temp_s7);
                    } else {
                        obj_create(var_s1_2, OBJINIT_STANDALONE, sp70[i], var_s3, NULL);
                    }
                }
                var_s3++;
                var_s1_2 = (ObjSetup* ) &((s32 *)var_s1_2)[objSetup->quarterSize];
                /* default.dol
                if (objSetup->quarterSize == 0) {
                    STUBBED_PRINTF(" romlist %x ", objSetup);
                    break;
                }
                */
            }
        }
    }
    objList = obj_get_all_of_type(OBJTYPE_MOBILE_MAP, &count);
    for (i = 0; i < count; i++) {
        temp_s5 = objList[i];
        var_s3 = 0;
        temp_s4 = temp_s5->mobileMapID;
        temp_s6 = gLoadedMapsDataTable[temp_s4];
        if (temp_s6 != NULL) {
            temp_s7 = temp_s5->matrixIdx + 1;
            var_s1_2 = (ObjSetup* ) temp_s6->objectInstanceFile_ptr;
            temp_fp = gMapObjSetupLists[temp_s4].groupsStart + (s32)var_s1_2;
            objGroupBits = gDLL_29_Gplay->vtbl->world_get_obj_group_bits(temp_s4);
            if (objGroupBits != 0) {
                gDLL_29_Gplay->vtbl->world_disable_all_obj_groups(temp_s4);
                group = 0;
                while (objGroupBits != 0) {
                    if (objGroupBits & 1) {
                        func_8004B548(temp_s6, (s32) temp_s4, group, temp_s5);
                    }
                    objGroupBits >>=1;
                    group++;
                }
            }
            while ((u32) var_s1_2 < (u32) temp_fp) {
                objSetup = var_s1_2;
                if ((map_check_some_mapobj_flag(var_s3, temp_s4) == 0) && (map_should_stream_load_object(var_s1_2, temp_s7, temp_s4) != 0)) {
                    func_8004B710(var_s3, temp_s4, 1U);
                    if (arg0 != 0) {
                        obj_create(var_s1_2, OBJINIT_STANDALONE, temp_s4, var_s3, temp_s5);
                    } else if (map_get_is_object_streaming_disabled() != 0) {
                        func_80012584(0x3D, 4U, NULL, var_s1_2, temp_s4, var_s3, temp_s5, temp_s7);
                    } else {
                        obj_create(var_s1_2, OBJINIT_STANDALONE, temp_s4, var_s3, temp_s5);
                    }
                }
                var_s3++;
                var_s1_2 = (ObjSetup* ) &((s32 *)var_s1_2)[objSetup->quarterSize];
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

s32 map_should_stream_load_object(ObjSetup* arg0, s8 arg1, s32 arg2) {
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

    if (arg0->loadFlags & OBJSETUP_LOAD_LEVEL) {
        return 1;
    }

    if (arg0->loadFlags & OBJSETUP_LOAD_MANUAL) {
        return 0;
    }

    if (arg1 == 0) {
        scaledX = floor_f((arg0->x - gWorldX) / BLOCKS_GRID_UNIT_F);
        scaledZOrFlag = floor_f((arg0->z - gWorldZ) / BLOCKS_GRID_UNIT_F);
        if (scaledX < 0 || scaledZOrFlag < 0 || scaledX >= BLOCKS_GRID_SPAN || scaledZOrFlag >= BLOCKS_GRID_SPAN) {
            return 0;
        }

        stop = 0;
        scaledX = GRID_INDEX(scaledZOrFlag, scaledX);
        for (i = 0; i < MAP_LAYER_COUNT; i++) {
            currentBlockIndices = gBlockIndices[i];
            if (currentBlockIndices[scaledX] >= 0) {
                stop = 1;
            }
        }
        if (stop == 0) {
            return 0;
        }
    }

    if (arg0->loadFlags & OBJSETUP_LOAD_FLAG20) {
        return 1;
    }

    scaledZOrFlag = 0;
    if ((arg0->loadFlags & OBJSETUP_LOAD_MAIN) && (arg1 == 0)) {
        player = get_player();
        if (player != NULL) {
            sp20 = player->globalPosition.x;
            sp1C = player->globalPosition.y;
            sp18 = player->globalPosition.z;
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

s32 map_should_obj_unload(Object *obj) {
    s32 sp54;
    s32 pad[6];
    f32 loadDist;
    s32 pad2[6];
    ObjSetup* objSetup;
    s32 temp_v0_2;
    Object* player;
    f32 loadDistAdjusted;
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

    objSetup = obj->setup;
    if (objSetup == NULL) {
        STUBBED_PRINTF("OBJECT error: obj %d, has no romdef\n", obj->id);
        return FALSE;
    }
    if (func_8004B4A0(objSetup, obj->mapID) == 0) {
        return TRUE;
    }
    if (objSetup->loadFlags & OBJSETUP_LOAD_LEVEL) {
        return FALSE;
    }
    if (objSetup->loadFlags & OBJSETUP_LOAD_IN_MAP_OBJGROUP) {
        if (gDLL_29_Gplay->vtbl->get_obj_group_status(obj->mapID, objSetup->mapObjGroup) != 0) {
            return FALSE;
        }
        return TRUE;
    }
    if (objSetup->loadFlags & OBJSETUP_LOAD_MANUAL) {
        return FALSE;
    }
    if ((obj->unkC0 != NULL) && (obj->unkB4 < 0)) {
        return FALSE;
    }
    if (obj->parent == NULL) {
        sp54 = floor_f((obj->srt.transl.x - gWorldX) / BLOCKS_GRID_UNIT_F);
        temp_v0_2 = floor_f((obj->srt.transl.z - gWorldZ) / BLOCKS_GRID_UNIT_F);
        if ((sp54 < 0) || (temp_v0_2 < 0) || (sp54 >= BLOCKS_GRID_SPAN) || (temp_v0_2 >= BLOCKS_GRID_SPAN)) {
            return TRUE;
        }
        sp54 = GRID_INDEX(temp_v0_2, sp54);
        for (var_a1 = FALSE, i = 0; i < MAP_LAYER_COUNT; i++) {
            currentBlockIndices = gBlockIndices[i];
            if (currentBlockIndices[sp54] >= 0) {
                var_a1 = TRUE;
            }
        }
        if (var_a1 == FALSE) {
            return TRUE;
        }
    }
    if (objSetup->loadFlags & OBJSETUP_LOAD_FLAG20) {
        return FALSE;
    }
    if ((objSetup->loadFlags & OBJSETUP_LOAD_MAIN) && (player = get_player(), (player != NULL)) && obj->parent == NULL) {
        var_fv1 = player->globalPosition.x;
        var_fa0 = player->globalPosition.y;
        var_fa1 = player->globalPosition.z;
    } else {
        if (obj->parent != NULL) {
            var_a0 = obj->parent->matrixIdx + 1;
        } else {
            var_a0 = 0;
        }
        var_fv1 = Vec3_Int_array[var_a0].f.x;
        var_fa0 = Vec3_Int_array[var_a0].f.y;
        var_fa1 = Vec3_Int_array[var_a0].f.z;
    }
    loadDist = obj->loadDistance;
    if (obj->parent != NULL) {
        var_fv0 = var_fv1 - obj->srt.transl.x;
        var_ft4 = var_fa0 - obj->srt.transl.y;
        var_ft5 = var_fa1 - obj->srt.transl.z;
    } else {
        var_fv0 = var_fv1 - obj->globalPosition.x;
        var_ft4 = var_fa0 - obj->globalPosition.y;
        var_ft5 = var_fa1 - obj->globalPosition.z;
    }
    loadDistAdjusted = loadDist + 40.0f;
    var_fv0 = ((var_fv0 * var_fv0) + (var_ft4 * var_ft4) + (var_ft5 * var_ft5));
    if (var_fv0 < (loadDistAdjusted * loadDistAdjusted)) {
        return FALSE;
    }
    return TRUE;
}

/** map_should_object_load? */
// is_obj_in_current_map_setup
// track_check_object_load
s32 func_8004B4A0(ObjSetup* obj, s32 mapno) {
    s32 actno;

    actno = gDLL_29_Gplay->vtbl->get_map_setup(mapno);
    if (actno == -1) {
        return 0;
    }
    /* default.dol
    if (mapID < 0 || mapID >= 120) {
        STUBBED_PRINTF("Warning: object being checked doesn't have a valid mapno in trackCheckObjectLoad(mapno: %d)\n", mapID);
    }
    if (setupID > 11) {
        STUBBED_PRINTF("Warning: act number is not valid (actno: %d)\n", setupID);
    }
    */
    if (actno != 0) {
        if (actno < 9) {
            if ((obj->actExclusions1 >> (actno - 1)) & 1) {
                return 0;
            }
        }
        else if ((obj->actExclusions2 >> (16 - actno)) & 1) {
            return 0;
        }
    }
    return 1;
}

void func_8004B548(MapHeader* map, s32 mapID, s32 objGroupIdx, Object* arg3) {
    s8 *s4;
    s8 *s3;
    s8 *s0;
    s32 someVar;
    s32 someIndex;
    s32 var_s6;
    MapObjSetupList* objSetupList;

    objSetupList = &gMapObjSetupLists[mapID];
    if (objSetupList->groups[objGroupIdx] == -1) {
        return;
    }

    if (arg3 != NULL) {
        var_s6 = arg3->matrixIdx + 1;
    } else {
        var_s6 = map->unk19;
    }
    s0 = (s8 *) map->objectInstanceFile_ptr;
    someVar = 0;
    s4 =  &((s8*)s0)[objSetupList->groups[objGroupIdx]];
    while ((u32) s0 < (u32) s4) {
        someVar += 1;
        /* default.dol
        if (((ObjSetup *)s0)->quarterSize == 0) {
            STUBBED_PRINTF(" romlist %i ", s0);
        }
        */
        s0 += ((ObjSetup *)s0)->quarterSize * 4;
    }

    for (someIndex = objGroupIdx + 1; someIndex < 33; someIndex++) {
        if (objSetupList->groups[someIndex] != -1) {
            break;
        }
    }

    s0 = s4;
    s4 = &((s8 *)map->objectInstanceFile_ptr)[objSetupList->groups[someIndex]];
    while ((u32) s0 < (u32) s4) {
        s3 = s0;
        if ((map_check_some_mapobj_flag(someVar, mapID) == 0) && (func_8004B4A0((ObjSetup *)s0, mapID) != 0)) {
            func_8004B710(someVar, mapID, 1U);
            if (map_get_is_object_streaming_disabled() != 0) {
                func_80012584(0x3E, 4U, NULL, (ObjSetup *)s0, mapID, someVar, arg3, var_s6);
            } else {
                obj_create((ObjSetup* )s0, OBJINIT_STANDALONE, mapID, someVar, arg3);
            }
        }
        someVar += 1;
        s0 += ((ObjSetup *)s3)->quarterSize * 4;
    }
}

/**
 * Seems to be called when camera moves between grid cells?
 */
// track_set_loaded
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

    if (cell_offset > 127) {
        STUBBED_PRINTF("WARNING: trackSetLoaded bit overflow\n");
    }
    
    if (arg2 != 0) {
        map->end_ptr[cell_offset] |= bit_at_index; //sets a bit to do with the grid cell?
    }
}

// track_get_loaded
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
    
    if (cell_offset > 127) {
        STUBBED_PRINTF("WARNING: trackGetLoaded bit overflow\n");
    }

    if (map->end_ptr[cell_offset] & bit_at_index) {
        return 1;
    }

    return 0;
}

ObjSetup *map_find_curve(s32 search_uID, u32 arg1){
    MapHeader *map;
    u8 *ptr;
    s32 offset;
    s16 objID;
    s32 i;

    for (i = 0; i < 120; i++) {
        map = gLoadedMapsDataTable[i];
        if (map != NULL){
            ptr = (u8*)map->objectInstanceFile_ptr;
            offset = 0;
            while (offset < map->objectInstancesFileLength){       
                objID = ((ObjSetup*)ptr)->objId;
                if (objID == OBJ_curve && ((ObjSetup*)ptr)->uID == search_uID){
                    return ((ObjSetup*)ptr);
                }
        
                offset += ((ObjSetup*)ptr)->quarterSize << 2;
                ptr += ((ObjSetup*)ptr)->quarterSize << 2;
            }
        } 
    }
    
    return 0;
}

/**
  * "search_mapID == -1" searches through all maps instead of a specific mapID!
  */
ObjSetup* map_find_checkpoint(s32 search_uID, s32 search_mapID) {
    MapHeader* map;
    s32 search_endID;
    u8 *ptr;
    s32 offset;
    s16 objID;
    
    if (search_mapID == -1) {
        search_mapID = 0;
        search_endID = 120;
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
                objID = ((ObjSetup*)ptr)->objId;
                if (objID == OBJ_checkpoint4 && ((ObjSetup*)ptr)->uID == search_uID){
                    return ((ObjSetup*)ptr);
                }
        
                offset += ((ObjSetup*)ptr)->quarterSize << 2;
                ptr += ((ObjSetup*)ptr)->quarterSize << 2;
            }
        }

        search_mapID++;
    }

    return NULL;
}

void map_is_loaded(s32 mapID) {
    MapHeader *map;

    if ((mapID >= 0) && (mapID < 120)) {
        map = gLoadedMapsDataTable[mapID];
        if (map != NULL) {
            map->unk18 = 1;
        }
    }
}

void track_func_8004B948(s32 on) {
    if (on != 0) {
        gTrackFlags |= TRACKFLAG_UNK200;
    } else {
        gTrackFlags &= ~TRACKFLAG_UNK200;
    }
}

void map_save_object(ObjSetup* objsetup, s32 mapID, f32 x, f32 y, f32 z) {
    s16 numSavedObjs;
    s8 found;
    s32 i;
    SavedObject* savedObjs;

    numSavedObjs = gDLL_29_Gplay->vtbl->get_num_saved_objects();
    savedObjs = gDLL_29_Gplay->vtbl->get_saved_objects();
    if (objsetup->uID == -1) {
        STUBBED_PRINTF("Cannot move object with an ident of -1!!!\n");
        return;
    }
    for (found = FALSE, i = 0; i < numSavedObjs; i++) {
        if (objsetup->uID == savedObjs[i].uID) {
            found = TRUE;
            savedObjs[i].x = x;
            savedObjs[i].y = y;
            savedObjs[i].z = z;
            objsetup->x = x;\
            objsetup->y = y;\
            objsetup->z = z;\
        }
    }
    if (found == FALSE) {
        if (numSavedObjs > 100) {
            STUBBED_PRINTF(" OVer FLOW FLOW in RD saves for moving romdefs ");
        }
        if (objsetup == NULL) {
            STUBBED_PRINTF(" Error in moving of romdef ");
        }
        STUBBED_PRINTF(" Saving Romdef for for %i tab no %i \n", objsetup->uID, numSavedObjs);
        D_800B96B0[numSavedObjs].x = objsetup->x;
        D_800B96B0[numSavedObjs].y = objsetup->y;
        D_800B96B0[numSavedObjs].z = objsetup->z;
        D_800B96B0[numSavedObjs].uID =  objsetup->uID;
        D_800B96B0[numSavedObjs].mapID = mapID;
        i = numSavedObjs;
        savedObjs[numSavedObjs].uID = objsetup->uID;
        savedObjs[i].mapID = mapID;
        savedObjs[i].x = x;
        savedObjs[i].y = y;
        // @fake
        if ((!savedObjs) && (!savedObjs)) {}
        savedObjs[i].z = z;
        numSavedObjs++;
        objsetup->x = x;\
        objsetup->y = y;\
        objsetup->z = z;
        gDLL_29_Gplay->vtbl->set_num_saved_objects(numSavedObjs);
    }
}

// unused
void func_8004BC20(SavedObject* arg0, s32 uID) {
    s16 numSavedObjs;
    s32 i;
    SavedObject* savedObjs;

    numSavedObjs = gDLL_29_Gplay->vtbl->get_num_saved_objects();
    savedObjs = gDLL_29_Gplay->vtbl->get_saved_objects();
    if (uID == -1) {
        STUBBED_PRINTF("Cannot move object with an ident of -1!!!\n");
        return;
    }
    for (i = 0; i < numSavedObjs; i++) {
        if (uID == savedObjs[i].uID) {
            bcopy(&D_800B96B0[i], &savedObjs[i], sizeof(SavedObject));
            arg0->x = D_800B96B0[i].x;
            arg0->y = D_800B96B0[i].y;
            arg0->z = D_800B96B0[i].z;
            // break
            i = numSavedObjs;
        }
    }
}

static const char str_8009a8ec[] = " Removed Restored Num %i ";
static const char str_8009a908[] = "%i ";

// official name: romdefMove_Set ?
void map_restore_saved_objects(MapHeader* arg0, s32 mapID) {
    s32 i;
    s32 uID;
    s32 numFound;
    s32 var_t3;
    ObjSetup* objsetup;
    s32 temp_t4;
    s16 numSavedObjs;
    s32 pad;
    s16 sp68[15]; // unknown size however should be the same as sp28
    SavedObject* savedObjs;
    s32 sp28[15]; // unknown size however should be the same as sp68

    numSavedObjs = gDLL_29_Gplay->vtbl->get_num_saved_objects();
    savedObjs = gDLL_29_Gplay->vtbl->get_saved_objects();
    numFound = 0;
    for (i = 0; i < numSavedObjs; i++) {
        if (mapID == savedObjs[i].mapID) {
            sp68[numFound] = i;
            sp28[numFound] = savedObjs[i].uID;
            numFound++;
            if (numFound > 15) {
                STUBBED_PRINTF("romdefMove_Set: Mapno overflow!!\n");
            }
        }
    }
    var_t3 = 0;
    temp_t4 = arg0->objectInstancesFileLength;
    objsetup = (ObjSetup *) arg0->objectInstanceFile_ptr;
    while (var_t3 < (s32) temp_t4) {
        uID = objsetup->uID;
        for (i = 0; i < numFound; i++) {
            if (uID == sp28[i]) {
                objsetup->x = savedObjs[sp68[i]].x;
                objsetup->y = savedObjs[sp68[i]].y;
                objsetup->z = savedObjs[sp68[i]].z;
            }
        }
        var_t3 += objsetup->quarterSize * 4;
        objsetup = (ObjSetup *) &((s8 *)objsetup)[objsetup->quarterSize * 4];
    }
}

void func_8004C040(u8* arg0, u8* arg1, s32 arg2) {
    u8* temp;

    if (arg2 == 0) {
        return;
    }
    if (arg2 > 0) {
        temp = &arg1[arg2];
        while (arg1 != arg0) {
            arg1 -= 1;
            temp -= 1;
            temp[0] = arg1[0];
        }
        while (temp != arg0) {
            temp -= 1;
            temp[0] = 0;
        }
    } else {
        temp = &arg0[arg2];
        while (arg0 != arg1) {
            temp[0] = (u8) arg0[0];
            arg0 += 1;
            temp += 1;
        }
        while (temp != arg1) {
            temp[0] = 0;
            temp += 1;
        }
    }
}

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/map/block_compute_vertex_colors.s")
#else
typedef struct UnkSp8C {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} UnkSp8C;

// https://decomp.me/scratch/FLOC0
void block_compute_vertex_colors(Block* arg0, s32 arg1, s32 arg2, s32 arg3) {
    UnkSp8C* var_v0;
    BlockColorTableEntry* temp_v0_7;
    s32 var_a0;
    s32 var_a3;
    Unk80092BC0 sp160;
    s32 sp15C[1]; // has to be an array of size 1 and not a pointer
    s32 sp158[1]; // has to be an array of size 1 and not a pointer
    s32 sp154[1]; // has to be an array of size 1 and not a pointer
    s32 sp150[1]; // has to be an array of size 1 and not a pointer
    s32 sp14C[1]; // has to be an array of size 1 and not a pointer
    s32 sp148[1]; // has to be an array of size 1 and not a pointer
    Vtx_t* var_s1;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fv1;
    s32 temp_t0;
    s16 var_s3;
    s16 var_a0_2;
    s32 renderFlags;
    s32 var_a1;
    Vtx_t *sp124;
    s8 pad_sp123;
    s8 pad_sp122;
    u8 var_a1_2;
    u8 sp120;
    UnkSp8C* var_s0; // sp11C
    f32 sp110[3];
    s32 var_a2;
    UnkSp8C sp8C[4];
    f32 sp7C[4];
    u8 sp7B;
    u8 sp7A;
    u8 sp79;
    u8 sp78;
    u8 sp77;
    u8 sp76;
    u8 sp75;
    s32 i; // sp64?
    s32 var_t1;
    s32 var_t2;
    s32 var_t3;
    BlockShape* blockShape;

    sp160 = D_80092BC0;
    sp78 = 0;
    func_8001F81C(&sp7B, &sp7A, &sp79);
    sp77 = sp7B;
    sp76 = sp7A;
    sp75 = sp79;
    arg1 *= BLOCKS_GRID_UNIT;
    arg2 *= BLOCKS_GRID_UNIT;
    var_s0 = gDLL_11_Newlfx->vtbl->func3(0, &sp78);
    gDLL_57->vtbl->func2(sp158, sp150, sp148, sp15C, sp154, sp14C);
    if (var_s0 != NULL) {
        for (i = 0; i < sp78; i++) {
            var_v0 = &sp8C[i];
            var_v0->unk0 = var_s0->unk0;
            var_v0->unk4 = var_s0->unk4;
            var_v0->unk8 = var_s0->unk8;
            var_v0->unkC = var_s0->unkC;
            var_v0->unk10 = var_s0->unk10;
            var_v0->unk14 = var_s0->unk14;
            var_v0->unk18 = var_s0->unk18;
            var_v0->unk1C = var_s0->unk1C;
            var_v0->unk0 -= arg1;
            var_v0->unk8 -= arg2;
            temp_fv0 = var_v0->unk18;
            temp_fv1 = var_v0->unk1C;
            if (temp_fv0 != temp_fv1) {
                sp7C[i] = 255.0f / (temp_fv1 - temp_fv0);
            } else {
                sp7C[i] = 255.0f;
            }
            var_s0++;
        }
    }
    for (i = 0; i < arg0->shapeCount; i++) {
        var_a1_2 = 0;
        blockShape = &arg0->shapes[i];
        renderFlags = blockShape->flags;
        if (arg0->materials[blockShape->materialIndex].texture == NULL) {
            continue;
        }
        sp120 = 0;
        if (((renderFlags & (RENDER_MIPMAPS | RENDER_UNK20)) || (arg0->materials[blockShape->materialIndex].texture->flags & (RENDER_COMPOSITE_BASE | RENDER_COMPOSITE_OVERLAY))) 
                && !(renderFlags & (RENDER_UNK400000 | RENDER_UNK800000 | RENDER_UNK2000000 | RENDER_UNK4000000 | RENDER_UNK40000000))) {
            var_a1_2 = 1;
        } else if (renderFlags & (RENDER_UNK400000 | RENDER_UNK2000000 | RENDER_UNK4000000 | RENDER_UNK40000000)) {
            var_a1_2 = 2;
        } else if (renderFlags & RENDER_UNK1000000) {
            var_a1_2 = 3;
        } else if (renderFlags & RENDER_UNK800000) {
            var_a1_2 = 4;
        }

        if (!(renderFlags & RENDER_SHAPE_VISIBLE)) {
            if ((var_a1_2 != 0) && (var_a1_2 != 3)) {
            }
            continue;
        }

        if (blockShape->envColourMode == 0xFF) {
            if (renderFlags & RENDER_UNK400000) {
                sp7B = sp158[0];
                sp7A = sp150[0];
                sp79 = sp148[0];
            } else if (renderFlags & RENDER_UNK40000000) {
                sp7B = sp15C[0];
                sp7A = sp154[0];
                sp79 = sp14C[0];
            }
        } else if (blockShape->envColourMode == 0xFE) {
            sp7B = 0xFF;
            sp7A = 0xFF;
            sp79 = 0xFF;
        } else {
            sp7B = sp77;
            sp7A = sp76;
            sp79 = sp75;
        }

        if (var_a1_2 != 0) {
            var_a0 = var_a1_2;
            var_s1 = arg0->vertices2[(arg0->vtxFlags & 1) ^ 1];
            blockShape = &arg0->shapes[i];
            sp124 = &var_s1[blockShape[1].vtxBase];
            var_s1 += blockShape->vtxBase;
            if ((var_a1_2 == 3) && (D_800B1847 != 0)) {
                while ((u32) var_s1 < (u32) sp124) {
                    var_s1->cn[0] = sp7B;
                    var_s1->cn[1] = sp7A;
                    var_s1->cn[2] = sp79;
                    var_s1++;
                }
            } else if (var_a0 == 1) {
                while ((u32) var_s1 < (u32) sp124) {
                    var_s1->cn[0] = gBlockColorTable[((s16)var_s1->flag >> 2) & 0xFF].r2;
                    var_s1->cn[1] = gBlockColorTable[((s16)var_s1->flag >> 2) & 0xFF].g2;
                    var_s1->cn[2] = gBlockColorTable[((s16)var_s1->flag >> 2) & 0xFF].b2;
                    var_s1++;
                }
            } else if (var_a0 == 4) {
                while ((u32) var_s1 < (u32) sp124) {
                    if (!((s16)var_s1->flag & 3)) {
                        var_s1->cn[0] = gBlockColorTable[((s16)var_s1->flag >> 2) & 0xFF].r2;
                        var_s1->cn[1] = gBlockColorTable[((s16)var_s1->flag >> 2) & 0xFF].g2;
                        var_s1->cn[2] = gBlockColorTable[((s16)var_s1->flag >> 2) & 0xFF].b2;
                    }
                    var_s1++;
                }
            } else {
                if (var_a0 == 2) {
                    sp160.unk0[0] = 0;
                    sp160.unk0[1] = 0;
                    sp160.unk0[2] = 0;
                    for (var_s3 = 0; var_s3 < sp78; var_s3++) {
                        var_s1 = arg0->vertices2[(arg0->vtxFlags & 1) ^ 1];
                        blockShape = &arg0->shapes[i];
                        sp124 = &var_s1[blockShape[1].vtxBase];
                        var_s1 = &var_s1[blockShape->vtxBase];
                        if (var_s0 != NULL) {
                            var_v0 = &sp8C[var_s3];
                            if (
                                ((blockShape->Xmin - 0x32) << 2) < var_v0->unk0 &&
                                var_v0->unk0 < ((blockShape->Xmax + 0x32) << 2) &&
                                ((blockShape->Zmin - 0x32) << 2) < var_v0->unk8 &&
                                var_v0->unk8 < ((blockShape->Zmax + 0x32) << 2)
                            ) {
                                sp120 = 1;
                            }
                        }
                        while ((u32) var_s1 < (u32) sp124) {
                            if (sp120 != 0) {
                                var_v0 = &sp8C[var_s3];
                                sp110[0] = var_s1->ob[0] - var_v0->unk0;
                                sp110[1] = var_s1->ob[1] - var_v0->unk4;
                                sp110[2] = var_s1->ob[2] - var_v0->unk8;
                                temp_fv0 = sqrtf(((sp110[0] * sp110[0]) + (sp110[1] * sp110[1])) + (sp110[2] * sp110[2]));
                                if (temp_fv0 < var_v0->unk18) {
                                    var_t1 = var_v0->unkC;
                                    var_t2 = var_v0->unk10;
                                    var_t3 = var_v0->unk14;
                                } else {
                                    if (var_v0->unk1C < temp_fv0) {
                                        var_t1 = 0;
                                        var_t2 = 0;
                                        var_t3 = 0;
                                    } else {
                                        var_fv1 = (temp_fv0 - var_v0->unk18);
                                        var_fv1 *= sp7C[var_s3];
                                        if (var_fv1 > 255.0f) {
                                            var_fv1 = 255.0f;
                                        }
                                        var_fv1 = 255.0f - var_fv1;
                                        var_t1 = ((s16)var_v0->unkC * (s16)var_fv1) >> 8;
                                        var_t2 = ((s16)var_v0->unk10 * (s16)var_fv1) >> 8;
                                        var_t3 = ((s16)var_v0->unk14 * (s16)var_fv1) >> 8;
                                    }
                                }
                                if (arg0->shapes[i].flags & (RENDER_UNK400000 | RENDER_UNK40000000)) {
                                    if (arg0->shapes[i].flags & RENDER_UNK400000) {
                                        var_a0_2 = 0;
                                    } else {
                                        var_a0_2 = 1;
                                    }
                                    if (var_s3 == 0) {
                                        temp_t0 = ((s16) var_s1->flag >> 2) & 0xFF;
                                        switch ((s16)var_s1->flag & 3) {
                                        case 0: /* switch 1 */
                                            var_a1 = gBlockColorTable[temp_t0].r2;
                                            var_a2 = gBlockColorTable[temp_t0].g2;
                                            var_a3 = gBlockColorTable[temp_t0].b2;
                                            break;
                                        default: /* switch 1 */
                                        case 1: /* switch 1 */
                                            var_a1 = sp158[var_a0_2];
                                            var_a2 = sp150[var_a0_2];
                                            var_a3 = sp148[var_a0_2];
                                            break;
                                        case 2: /* switch 1 */
                                            temp_v0_7 = &gBlockColorTable[temp_t0];
                                            var_a1 = (((sp158[var_a0_2] + temp_v0_7->r) >> 1) * sp7B) >> 8;
                                            var_a2 = (((sp150[var_a0_2] + temp_v0_7->g) >> 1) * sp7A) >> 8;
                                            var_a3 = (((sp148[var_a0_2] + temp_v0_7->b) >> 1) * sp79) >> 8;
                                            break;
                                        }
                                        var_a1 += var_t1;
                                        var_a2 += var_t2;
                                        var_a3 += var_t3;
                                        if (var_a1 >= 0x100) {
                                            var_a1 = 0xFF;
                                        }
                                        if (var_a2 >= 0x100) {
                                            var_a2 = 0xFF;
                                        }
                                        if (var_a3 >= 0x100) {
                                            var_a3 = 0xFF;
                                        }
                                        var_s1->cn[0] = var_a1;
                                        var_s1->cn[1] = var_a2;
                                        var_s1->cn[2] = var_a3;
                                    } else {
                                        var_a1 = var_s1->cn[0];
                                        var_a1 += var_t1;
                                        var_a2 = var_s1->cn[1];
                                        var_a2 += var_t2;
                                        var_a3 = var_s1->cn[2];
                                        var_a3 += var_t3;
                                        if (var_a1 >= 0x100) {
                                            var_a1 = 0xFF;
                                        }
                                        if (var_a2 >= 0x100) {
                                            var_a2 = 0xFF;
                                        }
                                        if (var_a3 >= 0x100) {
                                            var_a3 = 0xFF;
                                        }
                                        var_s1->cn[0] = var_a1;
                                        var_s1->cn[1] = var_a2;
                                        var_s1->cn[2] = var_a3;
                                    }
                                } else if (arg0->shapes[i].flags & RENDER_UNK2000000) {
                                    if (var_s3 == 0) {
                                        var_a1 = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].r2;
                                        var_a1 += var_t1;
                                        var_a2 = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].g2;
                                        var_a2 += var_t2;
                                        var_a3 = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].b2;
                                        var_a3 += var_t3;
                                        if (var_a1 >= 0x100) {
                                            var_a1 = 0xFF;
                                        }
                                        if (var_a2 >= 0x100) {
                                            var_a2 = 0xFF;
                                        }
                                        if (var_a3 >= 0x100) {
                                            var_a3 = 0xFF;
                                        }
                                        var_s1->cn[0] = var_a1;
                                        var_s1->cn[1] = var_a2;
                                        var_s1->cn[2] = var_a3;
                                    } else {
                                        var_a1 = var_s1->cn[0];
                                        var_a1 += var_t1;
                                        var_a2 = var_s1->cn[1];
                                        var_a2 += var_t2;
                                        var_a3 = var_s1->cn[2];
                                        var_a3 += var_t3;
                                        if (var_a1 >= 0x100) {
                                            var_a1 = 0xFF;
                                        }
                                        if (var_a2 >= 0x100) {
                                            var_a2 = 0xFF;
                                        }
                                        if (var_a3 >= 0x100) {
                                            var_a3 = 0xFF;
                                        }
                                        var_s1->cn[0] = var_a1;
                                        var_s1->cn[1] = var_a2;
                                        var_s1->cn[2] = var_a3;
                                    }
                                } else if (arg0->shapes[i].flags & RENDER_UNK4000000) {
                                    if (var_s3 == 0) {
                                        var_a1 = gBlockColorTable[((s16)var_s1->flag >> 2) & 0xFF].r2;
                                        var_a2 = gBlockColorTable[((s16)var_s1->flag >> 2) & 0xFF].g2;
                                        var_a3 = gBlockColorTable[((s16)var_s1->flag >> 2) & 0xFF].b2;
                                        if (((s16)var_s1->flag & 3) == 1) {
                                            var_a1 += var_t1;
                                            var_a2 += var_t2;
                                            var_a3 += var_t3;
                                        } else if (((s16)var_s1->flag & 3) == 2) {
                                            var_a1 += var_t1 >> 1;
                                            var_a2 += var_t2 >> 1;
                                            var_a3 += var_t3 >> 1;
                                        }
                                        if (var_a1 >= 0x100) {
                                            var_a1 = 0xFF;
                                        }
                                        if (var_a2 >= 0x100) {
                                            var_a2 = 0xFF;
                                        }
                                        if (var_a3 >= 0x100) {
                                            var_a3 = 0xFF;
                                        }
                                        var_s1->cn[0] = var_a1;
                                        var_s1->cn[1] = var_a2;
                                        var_s1->cn[2] = var_a3;
                                    } else {
                                        var_a1 = var_s1->cn[0];
                                        var_a2 = var_s1->cn[1];
                                        var_a3 = var_s1->cn[2];
                                        if (((s16) var_s1->flag & 3) == 1) {
                                            var_a1 += var_t1;
                                            var_a2 += var_t2;
                                            var_a3 += var_t3;
                                        } else if (((s16) var_s1->flag & 3) == 2) {
                                            var_a1 += var_t1 >> 1;
                                            var_a2 += var_t2 >> 1;
                                            var_a3 += var_t3 >> 1;
                                        }
                                        if (var_a1 >= 0x100) {
                                            var_a1 = 0xFF;
                                        }
                                        if (var_a2 >= 0x100) {
                                            var_a2 = 0xFF;
                                        }
                                        if (var_a3 >= 0x100) {
                                            var_a3 = 0xFF;
                                        }
                                        var_s1->cn[0] = var_a1;
                                        var_s1->cn[1] = var_a2;
                                        var_s1->cn[2] = var_a3;
                                    }
                                }
                            } else {
                                if ((var_s3 == 0) && (arg0->shapes[i].flags & (RENDER_UNK400000 | RENDER_UNK40000000))) {
                                    if (arg0->shapes[i].flags & RENDER_UNK400000) {
                                        var_a0_2 = 0;
                                    } else {
                                        var_a0_2 = 1;
                                    }
                                    temp_t0 = ((s16) var_s1->flag >> 2) & 0xFF;
                                    switch ((s16) var_s1->flag & 3) { /* irregular */
                                    case 0:
                                        var_s1->cn[0] = gBlockColorTable[temp_t0].r2;
                                        var_s1->cn[1] = gBlockColorTable[temp_t0].g2;
                                        var_s1->cn[2] = gBlockColorTable[temp_t0].b2;
                                        break;
                                    default:
                                    case 1:
                                        var_s1->cn[0] = sp158[var_a0_2];
                                        var_s1->cn[1] = sp150[var_a0_2];
                                        var_s1->cn[2] = sp148[var_a0_2];
                                        break;
                                    case 2:
                                        var_s1->cn[0] = (((sp158[var_a0_2] + gBlockColorTable[temp_t0].r) >> 1) * sp7B) >> 8;
                                        var_s1->cn[1] = (((sp150[var_a0_2] + gBlockColorTable[temp_t0].g) >> 1) * sp7A) >> 8;
                                        var_s1->cn[2] = (((sp148[var_a0_2] + gBlockColorTable[temp_t0].b) >> 1) * sp79) >> 8;
                                        break;
                                    }
                                } else if (arg0->shapes[i].flags & RENDER_UNK2000000) {
                                    var_s1->cn[0] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].r2;
                                    var_s1->cn[1] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].g2;
                                    var_s1->cn[2] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].b2;
                                } else if (arg0->shapes[i].flags & RENDER_UNK4000000) {
                                    var_s1->cn[0] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].r2;
                                    var_s1->cn[1] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].g2;
                                    var_s1->cn[2] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].b2;
                                }
                            }
                            var_s1++;
                        }
                    }
                    if (sp78 == 0) {
                       while ((u32) var_s1 < (u32) sp124) {
                            if (arg0->shapes[i].flags & (RENDER_UNK400000 | RENDER_UNK40000000)) {
                                if (arg0->shapes[i].flags & RENDER_UNK400000) {
                                    var_a0_2 = 0;
                                } else {
                                    var_a0_2 = 1;
                                }
                                temp_t0 = ((s16) var_s1->flag >> 2) & 0xFF;
                                switch ((s16) var_s1->flag & 3) { /* switch 2; irregular */
                                case 0:     /* switch 2 */
                                    var_s1->cn[0] = gBlockColorTable[temp_t0].r2;
                                    var_s1->cn[1] = gBlockColorTable[temp_t0].g2;
                                    var_s1->cn[2] = gBlockColorTable[temp_t0].b2;
                                    break;
                                default:    /* switch 2 */
                                case 1:     /* switch 2 */
                                    var_s1->cn[0] = sp158[var_a0_2];
                                    var_s1->cn[1] = sp150[var_a0_2];
                                    var_s1->cn[2] = sp148[var_a0_2];
                                    break;
                                case 2:     /* switch 2 */
                                    var_s1->cn[0] = (((sp158[var_a0_2] + gBlockColorTable[temp_t0].r) >> 1) * sp7B) >> 8;
                                    var_s1->cn[1] = (((sp150[var_a0_2] + gBlockColorTable[temp_t0].g) >> 1) * sp7A) >> 8;
                                    var_s1->cn[2] = (((sp148[var_a0_2] + gBlockColorTable[temp_t0].b) >> 1) * sp79) >> 8;
                                    break;
                                }
                            } else if (arg0->shapes[i].flags & RENDER_UNK2000000) {
                                var_s1->cn[0] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].r2;
                                var_s1->cn[1] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].g2;
                                var_s1->cn[2] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].b2;
                            } else if (arg0->shapes[i].flags & RENDER_UNK4000000) {
                                var_s1->cn[0] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].r2;
                                var_s1->cn[1] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].g2;
                                var_s1->cn[2] = gBlockColorTable[((s16) var_s1->flag >> 2) & 0xFF].b2;
                            }
                            var_s1 += 1;
                        }
                    }
                }
            }
        }
    }
}
#endif

static const char str_8009a930[] = "######  DOING WARP  ########\n";

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
        gDLL_28_ScreenFade->vtbl->fade(0x28, SCREEN_FADE_BLACK);
    }
    gDLL_12_Minic->vtbl->func4();
    gDLL_12_Minic->vtbl->func1();
    gDLL_8->vtbl->func1();
    gDLL_7_Newday->vtbl->func1();
    gDLL_9_Newclouds->vtbl->func1();
    gDLL_10_Newstars->vtbl->func0();
}

/** 
    Called every frame!
    Seems to start a fade-out followed by a warp
*/
void map_handle_transition(void) {
    PlayerLocation* playerLocation;
    Warp* warpLocation;
    u8 temp2;
    u8 temp1;

    playerLocation = gDLL_29_Gplay->vtbl->get_player_saved_location();
    
    //Start fade?
    if (D_800B4A5E != -1) { //timer started?
        D_80092A78 -= 1; //Decrement timer
        if (D_80092A78 < 0) { //When timer less than zero, fade to black
            if ((D_800B4A5E >= 0) && (D_800B4A59 != 0)) {
                gDLL_28_ScreenFade->vtbl->fade_reversed(30, SCREEN_FADE_BLACK);
            }
            D_800B4A5E = -1; //stop timer?
        }
    }
    
    //Warp after fade?
    if (D_800B4A58 == 0) {
        return;
    }
        
    if (gDLL_28_ScreenFade->vtbl->is_complete() || D_800B4A59 == 0){
        warpLocation = &D_800B4A60;
        D_800B4A58 = 0;
        playerLocation->vec.x = warpLocation->coord.x;
        playerLocation->vec.y = warpLocation->coord.y;
        playerLocation->vec.z = warpLocation->coord.z;
        playerLocation->mapLayer = (s8)warpLocation->layer;
        func_800143A4();
        D_800B4A5E = D_800B4A5C;
        D_800B4A5C = -1;
        D_80092A78 = 8;
    }
}
