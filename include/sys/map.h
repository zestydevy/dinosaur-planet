#ifndef _SYS_MAP_H
#define _SYS_MAP_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"

#include "game/objects/object.h"
#include "sys/gfx/texture.h"
#include "sys/math.h"
#include "types.h"


#define RGBA8(r, g, b, a) (((u32)(r) << 24) | ((u32)(g) << 16) | ((u32)(b) << 8) | (u32)(a))


/* *********************************************** */
// MARK: DL Builder

#define DIRTY_FLAGS_COMBINE (1 << 0) // 1
#define DIRTY_FLAGS_OTHER_MODE (1 << 1) // 2
#define DIRTY_FLAGS_SET_PRIMARY_COLOR (1 << 2) // 4
#define DIRTY_FLAGS_SET_ENV_COLOR (1 << 3) // 8
#define DIRTY_FLAGS_SET_FILL_COLOR (1 << 4) // 0x10
#define DIRTY_FLAGS_SET_FOG_COLOR (1 << 5) // 0x20
#define DIRTY_FLAGS_GEOMETRY_MODE (1 << 6) // 0x40
#define DIRTY_FLAGS_BLEND_MODE (1 << 7) // 0x80
#define DIRTY_FLAGS_ALL 0xFF

typedef struct {
/*0000*/    Gfx combine;
/*0008*/    Gfx otherMode;
/*0010*/    u32 geometryMode;
/*0014*/    u32 primColor;
/*0018*/    u32 envColor;
/*001C*/    u32 fogColor;
/*0020*/    u32 fillColor;
/*0024*/    u32 blendColor;
/*0028*/    u8 needsPipeSync;
/*0029*/    u8 dirtyFlags;
} DLBuilder;

typedef struct {
/*0000*/    u8 unk0;
/*0001*/    u8 v0;
/*0002*/    u8 v1;
/*0003*/    u8 v2;
/*0004*/    u8 unk4[0x10 - 0x4];
} DLTri;


/* *********************************************** */
// MARK: Blocks

typedef struct {
/*0000*/    Texture *texture;
/*0004*/    u32 unk4;
/*0008*/    u32 flags; // RenderFlags
/*000C*/    s16 refCount;
/*000E*/    u8 unkE;
/*000F*/    u8 unkF;
} BlockTexture; // FIXME: better name

typedef struct {
/*00*/ s16 textureIndex;
/*02*/ u8 unk2;
/*03*/ u8 unk3;
} BlocksTextureIndexData;

//Shape bounding box - macros for extracting X/Z axes' extra precision values
#define SHAPE_BB_REMAINDER_X_MIN(shape) (shape->boundsRemainder & 3)
#define SHAPE_BB_REMAINDER_Z_MIN(shape) ((shape->boundsRemainder >> 2) & 3)
#define SHAPE_BB_REMAINDER_X_MAX(shape) ((shape->boundsRemainder >> 4) & 3)
#define SHAPE_BB_REMAINDER_Z_MAX(shape) ((shape->boundsRemainder >> 6) & 3)

typedef struct {
/*00*/    u32 flags; // RenderFlags
/*04*/    s16 vtxBase;
/*06*/    s16 triBase; // TODO: faceBase?
/*08*/    s16 Ymin;
/*0a*/    s16 Ymax;
/*0c*/    u8 Xmin; // Divided by 4
/*0d*/    u8 Xmax; // Divided by 4
/*0e*/    u8 Zmin; // Divided by 4
/*0f*/    u8 Zmax; // Divided by 4
/*10*/    s8 unk10;
/*11*/    s8 unk11;
/*12*/    u8 materialIndex;
/*13*/    u8 envColourMode; // 00 = use ambient envFX colour
                            // FE = ignore ambient envFX colour
                            // FF = use flickering vertices' colour
                            //     (used very rarely, in blocks 327 & 0843)
/*14*/    u8 animatorID; // Used to mark facebatches that can be removed/animated
/*15*/    u8 blendMaterialIndex; // Used for multitextured water
/*16*/    u8 unk16; // texture scroll effect handler index? (at runtime)
/*17*/    u8 boundsRemainder; // Extra precision for the X/Z axes of the shape's bounding box (2 bits for Xmin, Zmin, Xmax, Zmax respectively)
} BlockShape;

typedef struct {
/*0000*/    u32 d0;
/*0004*/    u32 d1;
} EncodedTri;

// Same as Vtx_t, but flag is signed
typedef struct {
	short ob[3];	/* x, y, z */
	short flag;
	short tc[2];	/* texture coord */
	unsigned char cn[4];	/* color & alpha */
} BlockVertex;

typedef struct{
/*00*/    Texture *texture; //textureID in ROM, but a pointer to the Texture object at runtime
/*04*/    u32 pad4;
/*08*/    u8  width;
/*09*/    u8  height;

            //bits 6-7: unused
            //bit  5:   greyscale
            //bit  4:   opaque
            //bit  0-3: type (RGBA/CI/IA/etc.)
/*0A*/    u8  format;
/*0B*/    u8  terrain_type; //Affects footstep/weapon foley sfx 
                            //(some values create dust/snow particles too, 
                            //or force water behaviour - overriding facebatch water setting!)
} BlocksMaterial;

typedef struct{
/*00*/    s16 Ax;
/*02*/    s16 Bx;
/*04*/    s16 Ay;
/*06*/    s16 By;
/*08*/    s16 Az;
/*0a*/    s16 Bz;
/*0c*/    s8 heightA; //height can be treated as a single value "(heightA << 8) + heightB", depending on top settings bit
/*0d*/    s8 heightB;
/*0e*/    s8 settingsA;
/*0f*/    s8 settingsB;
/*10*/    s16 animatorID;
/*12*/    s16 pad;
} HitsLine;

typedef struct {
/*0000*/    BlocksMaterial *materials;
/*0004*/    BlockVertex *vertices;
/*0008*/    EncodedTri *encodedTris;
/*000C*/    BlockShape *shapes;
/*0010*/    s16 *ptr_faceEdgeVectors;
/*0014*/    s16 *xzBitmap;
/*0018*/    HitsLine *ptr_hits_lines; // in ROM, sometimes matches grid column (HITS.bin data pointer at runtime)
/*001C*/    s32 unk1C;
/*0020*/    Vtx_t *vertices2[2];
/*0028*/    BlocksTextureIndexData *unk28;
/*002C*/    Gfx *gdlGroups; // In groups of 3 per shape; used to set up materials.
/*0030*/    s16 vtxFlags;
/*0032*/    s16 vtxCount;
/*0034*/    s16 unk34;
/*0036*/    s16 shapeCount;
/*0038*/    s16 hits_line_count; //value only present at runtime
/*003A*/    s8 unk3A;
/*003B*/    s8 unk3B;
/*003C*/    s16 unk3C;
/*003E*/    s16 unk3E; // pointer to animated vertices, maybe?
/*0040*/    s16 minY; // lowest vertex elevation
/*0042*/    s16 maxY; // highest vertex elevation
/*0044*/    u16 modelSize; // length of decompressed model
            // unconfirmed, but seems to be something like
            // the number of times the texture/material settings
            // change while going through the block's F3DEX2 commands
/*0046*/    s16 textureLoadCount;
/*0048*/    u8 unk48; // texture_count (at runtime, distinct from material count since different materials can use same texture)
/*0049*/    u8 unk49;
/*004A*/    u8 materialCount;
/*004B*/    s32 unk4B;
/*004E*/    s8 unk4E;
} Block;

// Size: 0x22
typedef struct {
/** Primary scroll material */
/*00*/ s16 uOffsetA;    //scroll position for U component
/*02*/ s16 vOffsetA;    //scroll position for V component
/*04*/ s16 uSpeedA;     //scroll speed for U component
/*06*/ s16 vSpeedA;     //scroll speed for V component
/*08*/ s16 uRemainderA; //accumulated dU remainders from previous ticks
/*0A*/ s16 vRemainderA; //accumulated dV remainders from previous ticks
/*0C*/ s16 widthA;      //texture tile width
/*0E*/ s16 heightA;     //texture tile height

/** Secondary scroll material (used by texture-blended water, etc.)*/
/*10*/ s16 uOffsetB;    //scroll position for U component 
/*12*/ s16 vOffsetB;    //scroll position for V component 
/*14*/ s16 uSpeedB;     //scroll speed for U component
/*16*/ s16 vSpeedB;     //scroll speed for V component
/*18*/ s16 uRemainderB; //accumulated dU remainders from previous ticks
/*1A*/ s16 vRemainderB; //accumulated dV remainders from previous ticks
/*1C*/ s16 widthB;      //texture tile width
/*1E*/ s16 heightB;     //texture tile height

/** Stats */
/*20*/ u8 refCount;     //how many BlockShapes make use of this scroll handler
/*21*/ u8 animating;    //Boolean - scrolling can be paused
} BlockTextureScroller;

typedef struct Unk80092BC0 {
    s16 unk0[3];
} Unk80092BC0;


/* *********************************************** */
// MARK: Map

typedef struct {
/*00*/  s16 gridSizeX;
/*02*/  s16 gridSizeZ;
/*04*/  s16 originOffsetX; // in grid units
/*06*/  s16 originOffsetZ; // in grid units
/*08*/  u16 objectInstancesFileLength;
/*0a*/  u16 unkA;
/*0c*/  u32 *blockIDs_ptr;
/*10*/  s8 *end_ptr;
/*14*/  s8 *grid_A1_ptr;
/*18*/  s8 unk18;
/*19*/  s8 unk19; //matrixIdx maybe?
/*1a*/  s8 unk1a;
/*1b*/  s8 unk1b;
/*1c*/  s16 objectInstanceCount; //malloc-related
/*1e*/  s16 gridB_sixteenthSize; //malloc-related ((size_of_gridB)/16 (rounded up and ignoring zero padding?)
/*20*/  void *objectInstanceFile_ptr;
/*24*/  f32 originWorldX;
/*28*/  f32 originWorldZ;
/*2c*/  s8 *grid_B1_ptr;
/*30*/  s8 *grid_A2_ptr;
/*34*/  s8 *grid_B2_ptr;
} MapHeader;

typedef struct {
/*00*/ MapHeader *header;
/*04*/ s16 mapID;
/*06*/ u8 unk06; // usually 0 or 1?
/*07*/ u8 unk07; // usually 0?
} StreamMap;

enum MapTypes {
    MAPTYPE_REGULAR = 0,
    MAPTYPE_MOBILE = 1,
    MAPTYPE_2 = 2,
    MAPTYPE_3 = 3
};

typedef struct {
/*00*/ char name[0x1c];
/*1c*/ s8 type; //0 = regular map, 1 = mobile map, 3 = debug map?
/*1d*/ s8 padding;
/*1e*/ s16 mobileMapUnknown; //unknown purpose, but only used by "mobile maps" (the Galleon, "wctemplelift", etc) 
} MapInfo;

typedef struct {
/*00*/  s16 mapIDs[3];
/*06*/  s16 blockID;
/*08*/  s8 loadedBlockIndex;
/*09*/  s8 trkBlkIndex; //Used to index into TRKBLK.bin, in order to get the base blockID for the map
/*0a*/  s8 unkA;
/*0b*/  s8 unkB;
} GlobalMapCell; //size 0xC

typedef struct {
  Vec3f coord;
  s32 layer;
} Warp;

typedef struct {
  Vec3f coord;
  s8 layer[2];
} SimilarToWarp;

// size: 0xA
typedef struct Struct_D_800B9768_unk4 {
    s16 xMin;
    s16 xMax;
    s16 zMin;
    s16 zMax;
    s8  unk8;
    s8  unk9;
} Struct_D_800B9768_unk4;

// size: 0x1
typedef struct Struct_D_800B9768_unkC {
    s8 unk0;
} Struct_D_800B9768_unkC;

// size: 0x20
typedef struct Struct_D_800B9768_unk10 {
    u8 unk0[0x20];
} Struct_D_800B9768_unk10;

typedef struct Struct_D_800B9768 {
    s32                      unk0;
    Struct_D_800B9768_unk4  *unk4;
    s16  (*unk8)[2];
    s8  *unkC;
    Struct_D_800B9768_unk10 *unk10;
} Struct_D_800B9768;

// size: 0xC?
typedef struct StructBuf {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} StructBuf;

// size: 0x1C
typedef struct {
/*00*/ s32 unk0;
/*04*/ s32 unk4;
/*08*/ s32 unk8;
/*0C*/ s32 unkC;
/*10*/ s32 unk10;
/*14*/ s32 unk14;
/*18*/ s32 _pad18;
} MapsTabStruct;

typedef struct MapsBinUnk20 {
    s16   unk0;
    u8    unk2;
    s32   _unk4;
    f32   unk8;
    f32   unkC;
    f32   unk10;
    s32   _unk14;
    u8    unk18;
    u8    unk19;
    u32   unk1C;
} MapsBinUnk20;

// size: 0x38
typedef struct MapsBinStruct {
    /*0x00*/ s16 unk0;
    /*0x02*/ s16 unk2;
    /*0x04*/ s16 unk4;
    /*0x06*/ s16 unk6; // new
    /*0x08*/ u16 unk8;
    /*0x0A*/ s16 _padA;
    /*0x0C*/ s32 *unkC;
    /*0x10*/ s32 unk10;
    /*0x14*/ s32 unk14;
    /*0x18*/ s8  unk18;
    /*0x19*/ s8  unk19;
    /*0x1A*/ s16 _pad1A;
    /*0x1C*/ s16 unk1C;
    /*0x1E*/ s16 unk1E;
    /*0x20*/ MapsBinUnk20 *unk20;
    /*0x24*/ f32 unk24;
    /*0x28*/ f32 unk28;
    /*0x2C*/ s32 unk2C;
    /*0x30*/ s32 unk30;
    /*0x34*/ s32 unk34;
} MapsBinStruct;

typedef struct {
/*0x0*/ u8 r;
/*0x1*/ u8 g;
/*0x2*/ u8 b;
/*0x3*/ u8 r2;
/*0x4*/ u8 g2;
/*0x5*/ u8 b2;
/*0x6*/ u8 a;
/*0x7*/ u8 unk7;
/*0x8*/ s16 refCount;
} BlockColorTableEntry;

typedef struct {
/*0000*/    f32 x;
/*0004*/    f32 y;
/*0008*/    f32 z;
/*000C*/    f32 d;
/*0010*/    u8 unk14[4];
} Plane;

typedef enum {
    WARP_ICE_MOUNTAIN_CAMPSITE = 2, // Sabre's campsite cave
    WARP_WM_SABRE_SIDE = 5,
    WARP_WM_KRYSTAL_SIDE = 6,
    WARP_SC_RUBBLE_PODIUM = 14, // SwapStone Circle
    WARP_SH_ROCKY_PODIUM = 15, // SwapStone Hollow
    WARP_IM_RACE_START = 26,
    WARP_WM_SABRE_KRAZOA_CORRIDOR = 57, // Warlock Mountain | Sabre-side | Krazoa statue corridor
    WARP_SC_TOTEM_POLE = 80,
    WARP_VFP_CALDERA_LOWER = 81, // Volcano Force Point Temple | Caldera (Lower Floor) 
    WARP_SC_DISCOVERY_FALLS_ENTRANCE_POND = 85, // SwapStone Circle | Pond outside Discovery Falls
    WARP_SWAPSTONE_SHOP_ENTRANCE = 86
} WarpID;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 mapSetupID;
/*19*/ u8 unk19;
/*1C*/ u32 objGroupStatusBits;
} SetupPoint_Setup;

typedef struct MapObjSetupList {
       // Byte offset to the list of each object setup group
/*00*/ s32 groups[32];
       // Byte size of object setup list, up to and not including curves/race checkpoints
/*80*/ s32 objsListSize;
       // Byte offset of when curve/race checkpoint setups start
/*84*/ s32 curvesOffset;
       // Byte offset to the first setups that are in object groups
/*88*/ s32 groupsStart;
} MapObjSetupList;

#define MAX_RENDER_LIST_LENGTH 400
#define MAX_BLOCKS 40

/** The X/Z size of the world grid cells, in vertex position units */
#define BLOCKS_GRID_UNIT 640
#define BLOCKS_GRID_UNIT_F 640.0f
#define BLOCKS_GRID_UNIT_HALF 320
#define BLOCKS_GRID_UNIT_HALF_F 320.0f
#define BLOCKS_GRID_SPAN 16
#define BLOCKS_GRID_TOTAL_CELLS 256
#define MAP_ID_MAX 80
// The map grid always has 5 layers: -2, -1, 0, 1, 2
#define MAP_LAYER_COUNT 5
#define MAX_TEXTURE_SCROLLERS 58

#define GRID_INDEX(z, x) (((z) * BLOCKS_GRID_SPAN + (x)))


/* *********************************************** */
// MARK: Globals

extern DLBuilder *gDLBuilder;

extern s16 D_800B4A5E; //gFadeDelayTimerStarted

extern f32 gWorldX;
extern f32 gWorldZ;

extern s8 D_80092A78; //gFadeDelayTimer

extern s32 D_80092A60; // something x-coordinate related
extern s32 D_80092A64; // something z-coordinate related

extern s32 D_80092A7C[2];
extern s32 D_80092A84[2];


/* *********************************************** */
// MARK: Functions

// DL Builder

void dl_set_all_dirty(void);
void func_80040FF8(void);
void func_80041028(void);
void dl_apply_combine(Gfx **gdl);
void dl_apply_other_mode(Gfx **gdl);
void dl_apply_geometry_mode(Gfx **gdl);
void dl_set_geometry_mode(Gfx **gdl, u32 mode);
void dl_clear_geometry_mode(Gfx **gdl, u32 mode);
void dl_set_prim_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void dl_set_prim_color_no_sync(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void dl_set_env_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void dl_set_env_color_no_sync(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void dl_set_blend_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void dl_set_fill_color(Gfx **gdl, u32 color);
void dl_set_fog_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void dl_triangles(Gfx **gdl, DLTri *tris, s32 triCount);

// Render Settings

void func_80041C30(s32 arg0);
void func_80041C6C(s32);
void func_80041CA8(s32 arg0);
void func_80041CE4(s32 arg0);
void func_80041D20(s32);
s32 func_80041D5C(void);
u32 func_80041D74(void);
u32 func_80041D8C(void);
u32 func_80041DA4(void);
s32 func_80041DBC(void);
s32 func_80041DD4(void);
void func_80041DEC(void);
s32 func_80041E08(void);
void func_80041E24(s32 arg0);
s32 func_80041E68(void);

// Map/Track

void init_maps(void);
void func_80042174(s32);
void func_8004225C(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, Vertex **vtxs2, Triangle **pols2);
ObjSetup* func_80044448(s32 match_uID, s32* match_indexInMap, s32* match_mapID, s32* arg3, s32* arg4);
s32 func_8004454C(f32 x, f32 y, f32 z);
void func_8004478C(f32 worldX, f32 worldY, f32 worldZ, f32* blockWorldOriginX, f32* blockWorldOriginZ);
s16 map_get_map_id_from_xz_ws(f32 arg0, f32 arg1);
MapHeader** func_80044A10(void);
void *func_80044A20(f32 worldX, f32 worldZ, s32* objectsFileLength);

// Blocks (map)

Block* func_80044B18(s32 visGridX, s32 visGridZ, s32 mapLayer);
s8* func_80044B98(s32 arg0);
Block* func_80044BB0(s32 blockIndex);

// Map/Track (again)

u8 track_obj_vis_check(Object* obj); // check if obj is visible (wrt. fade, camera frustum)
u8 is_sphere_in_frustum(Vec3f *v, f32 radius);

// streaming stuff starts here

s32 func_80045D58(void);
void map_free(s32);
void map_load_mobile_map(s32 id, Object *obj);
GlobalMapCell* func_80046698(s32 gridX, s32 gridZ);

// Global Map

void init_global_map(void);
void map_func_80046B58(f32 x, f32 y, f32 z);
void map_update_streaming(void);
void map_increment_layer(void);
void map_decrement_layer(void);
void map_func_8004773C(void);
s32 map_get_type(void);
void map_func_80048034(void);
void map_func_80048054(s32 mapID, s32, f32 *, f32 *, f32 *, s8 *);
void map_func_800483BC(f32, f32, f32);
s8 map_get_layer(void);

// something else

void map_func_800484A8(void);

// Blocks (again)

void block_load(s32 id, s32 param_2, s32 globalMapIdx, u8 queue);
void block_emplace(Block *block, s32 id, s32 param_3, s32 globalMapIdx);
s32 func_80049B84(s32 uSpeedA, s32 vSpeedA, s32 widthA, s32 heightA, s32 uSpeedB, s32 vSpeedB, s32 widthB, s32 heightB);
void func_80049CE4(u32 scrollerID, s32 uSpeedA, s32 vSpeedA, s32 widthA, s32 heightA, s32 uSpeedB, s32 vSpeedB, s32 widthB, s32 heightB);
Texture* func_8004A1E8(s32 match_value);
BlocksTextureIndexData *func_8004A284(Block *block, s32 param_2);
BlockTexture *func_8004A2CC(s32 idx);
s32 func_8004A528(Object* obj, u8 animatorID);
s32 func_8004A5D8(Object* obj, u8 animatorID);

// something else

void func_8004A67C(void);
void func_8004B948(s32 arg0);
void map_save_object(ObjSetup* objsetup, s32 mapID, f32 x, f32 y, f32 z);

// Warp

void warpPlayer(s32 warpID, s8 fadeToBlack);

#endif
