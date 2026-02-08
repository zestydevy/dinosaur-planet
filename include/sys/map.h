#ifndef _SYS_MAP_H
#define _SYS_MAP_H

#include "PR/ultratypes.h"
#include "dlls/engine/29_gplay.h"
#include "ultra64.h"
#include "PR/gbi.h"

#include "types.h"
#include "sys/dl_debug.h"
#include "sys/math.h"
#include "sys/camera.h"
#include "sys/rarezip.h"
#include "sys/gfx/map.h"
#include "sys/bitstream.h"

#include "sys/map_enums.h"

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
    WARP_SWAPSTONE_SHOP_ENTRANCE = 86
} WarpID;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 mapSetupID;
/*19*/ u8 unk19;
/*1C*/ u32 objGroupStatusBits;
} SetupPoint_Setup;

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
/*00*/    u32 renderSettingBitfield; //TODO: split into bits
/*04*/    s16 baseVertexID;
/*06*/    s16 baseFaceID;
/*08*/    s16 Ymin;
/*0a*/    s16 Ymax;
/*0c*/    u8 Xmin; //Divided by 4
/*0d*/    u8 Xmax; //Divided by 4
/*0e*/    u8 Zmin; //Divided by 4
/*0f*/    u8 Zmax; //Divided by 4
/*10*/    s8 unk10;
/*11*/    s8 unk11;
/*12*/    u8 materialID;
/*13*/    s8 envColourMode;     //00 = use ambient envFX colour
                                //FE = ignore ambient envFX colour
                                //FF = use flickering vertices' colour
                                //    (used very rarely, in blocks 327 & 0843)
/*14*/    u8 animatorID;        //Used to mark facebatches that can be removed/animated
/*15*/    u8 blendMaterialID;   //Used for multitextured water
/*16*/    s8 runtimeValue;      //0 in ROM, but -1 gets written here at runtime
/*17*/    s8 unk_cull;          //The function responsible for face culling reads this
} FaceBatch; //NOTE: this is describing the same thing as the "BlockShape" struct (TODO: consolidate!)

typedef struct{
/*00*/    Texture *textureID; //textureID in ROM, but a pointer to the Texture object at runtime
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

typedef struct {
/*00*/ s16 textureIndex;
/*02*/ u8 unk02;
/*03*/ u8 unk03;
} BlocksTextureIndexData;

typedef struct {
    s32 unk0;
    s32 unk4;
} PtrFace;

typedef struct {
/*00*/    BlocksMaterial *ptr_materials;
/*04*/    u32 ptr_vertices;
/*08*/    PtrFace *ptr_faces;
/*0c*/    FaceBatch *ptr_faceBatches;
/*10*/    s16 *ptr_faceEdgeVectors;
/*14*/    s16 *unk14; // in ROM, sometimes matches grid row
/*18*/    HitsLine *ptr_hits_lines; // in ROM, sometimes matches grid column (HITS.bin data pointer at runtime)
/*1c*/    s32 unk1C;
/*20*/    Vtx *unk20[2]; // unknnown size, 2 is probably correct
/*28*/    BlocksTextureIndexData *unk28; //textureIDs_ptr
/*2c*/    s16 unk2C;
/*2e*/    s16 unk2E;  
/*30*/    s16 flags; //Typical values: 0x10, 0x18 (affects malloc size - more needed needed for blocks with animated verts)
/*32*/    s16 vertex_count;
/*34*/    s16 face_count;
/*36*/    s16 faceBatch_count;
/*38*/    s16 hits_line_count; //value only present at runtime
/*3a*/    s8  unk3A;
/*3b*/    s8  unk3B;
/*3c*/    s16 unk3C;
/*3e*/    s16 unk3E; //pointer to animated vertices, maybe?
/*40*/    s16 minY; //lowest vertex elevation
/*42*/    s16 maxY; //highest vertex elevation
/*44*/    s16 modelSize; //length of decompressed model
    
//unconfirmed, but seems to be something like
//the number of times the texture/material settings
//change while going through the block's F3DEX2 commands
/*46*/    s16 textureLoadCount;

/*48*/    u8 unk48; //texture_count (at runtime, distinct from material count since different materials can use same texture)
/*49*/    s8 unk49;
/*4a*/    u8  material_count;
/*4b*/    s32 unk4B;
/*4e*/    s8  unk4E;
} BlocksModel; //NOTE: this is describing the same thing as the "Block" struct (TODO: consolidate!)

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
/*0x8*/ s16 unk8; // some sort of index?
} MapsUnk_800B97C0;

// size: 0x10
typedef struct Unk8005341C {
    Object *unk0;
    s16 unk4;
    MtxF *unk8;
    MtxF *unkC;
} Unk8005341C;

typedef struct Unk80092BC0 {
    s16 unk0[3];
} Unk80092BC0;

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

typedef struct UnkFunc80051D68Arg3 {
    f32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA[3]; // x pos?
    s16 unk10[3]; // y pos?
    s16 unk16[3]; // z pos?
    s16 unk1C[8];
    UNK_TYPE_8 unk2E;
    UNK_TYPE_8 unk2F;
    s8 unk30;
    s8 unk31;
    u8 unk32;
    u8 pad33;
} UnkFunc80051D68Arg3;

#define MAX_RENDER_LIST_LENGTH 400
#define MAX_BLOCKS 40

/** The X/Z size of the world grid cells, in vertex position units */
#define BLOCKS_GRID_UNIT 640
#define BLOCKS_GRID_UNIT_F 640.0f
#define BLOCKS_GRID_SPAN 16
#define BLOCKS_GRID_TOTAL_CELLS 256
#define MAP_ID_MAX 80
// The map grid always has 5 layers: -2, -1, 0, 1, 2
#define MAP_LAYER_COUNT 5
#define MAX_TEXTURE_SCROLLERS 58

#define GRID_INDEX(z, x) (((z) * BLOCKS_GRID_SPAN + (x)))

extern s16 D_800B4A5E; //gFadeDelayTimerStarted

extern f32 gWorldX;
extern f32 gWorldZ;

extern s8 gMapNumStreamMaps;

extern s8 gMapLayer;

extern s32 D_80092BBC;

extern s8 D_80092A78; //gFadeDelayTimer

extern f32 D_8009A950;
extern f32 D_8009A954;
extern f32 D_8009A958;
extern f32 D_8009A95C;

extern Unk80092BC0 D_80092BC0;
extern u8 D_800B1847;

extern f32 D_8009AA28;
extern f32 D_8009AA2C;

extern f32 D_8009AA38; // = -0.200000003f;

extern f32 D_8009AA20;
extern f32 D_8009AA24;
extern u8 D_8009A614;
extern f32 D_80092AAC[24];

extern f32 D_8009AA1C;
extern f32 D_8009AA30;
extern f32 D_8009AA34;
extern f32 D_8009AA3C;
extern s32 D_80092A60; // something x-coordinate related
extern s32 D_80092A64; // something z-coordinate related

extern s32 D_80092A7C[2];
extern s32 D_80092A84[2];

// Used in other .c files
void dl_triangles(Gfx **gdl, DLTri *tris, s32 triCount);
s32 func_80041DBC(void);
s32 func_80041E08(void);
void *func_80044A20(f32 worldX, f32 worldZ, s32* objectsFileLength);
MapHeader** func_80044A10(void);
BlocksModel* func_80044B18(s32 visGridX, s32 visGridZ, s32 mapLayer);
GlobalMapCell* func_80046698(s32 gridX, s32 gridZ);

// defined in map.c but used before declared
void func_800441F4(u32* arg0, s32 arg1);
void block_load(s32 id, s32 param_2, s32 globalMapIdx, u8 queue);
void func_80048B14(Block *block);
void func_80048C24(Block *block);
u32 hits_get_size(s32 id);
void block_setup_vertices(Block *block);
void block_setup_gdl_groups(Block *block);
s32 block_setup_textures(Block *block);
void block_setup_xz_bitmap(Block *block);
void block_emplace(BlocksModel *block, s32 id, s32 param_3, s32 globalMapIdx);
void block_compute_vertex_colors(Block*,s32,s32,s32);
void func_80049D38(u32 arg0);
void func_80049FA8(BlocksModel*);
void func_800499BC(void);
void func_80049D88(void);
void func_80044BEC(void);
void func_80048F58(void);
void track_c_func(void);
u8 func_800456AC(Object* obj);
u8 is_sphere_in_frustum(Vec3f *v, f32 radius);
void map_convert_objpositions_to_ws(MapHeader *map, f32 X, f32 Z);
void map_init_obj_setup_list(MapHeader* map, MapObjSetupList* setupList, s32 mapID, s32 curvesOnly);
MapHeader *map_load_streammap(s32, s32);
void map_read_layout(Struct_D_800B9768_unk4 *arg0, u8 *arg1, s16 arg2, s16 arg3, s32 maptabindex);
void map_func_80048034(void);
void map_update_objects_streaming(s32);
s32 map_func_800485FC(s32, s32, s32, s32, s32);
void func_80047404(s32, s32, s32*, s32*, s32*, s32*, s32, s32, s32);
void func_800496E4(s32 blockIndex);
void map_update_streaming(void);
s32 func_8004A058(Texture* tex, u32 flags, s32 arg2);
s32 map_should_obj_unload(Object*);
void func_8004B548(MapHeader*, s32, s32, Object*);
s32 map_should_stream_load_object(ObjSetup*, s8, s32);
s32 map_check_some_mapobj_flag(s32, u32);
void func_8004B710(s32 cellIndex_plusBitToCheck, u32 mapIndex, u32 arg2);
s32 func_8004AEFC(s32 mapID, s16 *arg1, s16 searchLimit);
s32 func_8004B4A0(ObjSetup* obj, s32 mapID);
void block_add_to_render_list(Block *block, f32 x, f32 z);
void func_800436DC(Object* arg0, s32 arg1);
s32 func_80045DC0(s32, s32, s32); //unsure of last arg
s32 map_find_streammap_index(s32);
s32 map_load_streammap_add_to_table(s32);  //unsure of worldGridZ here

// other funcs that are used
void func_80058F3C(void);
void func_800143A4(void);
void func_8001EB80(void);
s32 func_80010048(void);
void vi_some_video_setup(s32 param1);
void func_80012B54(s32 param1, s32 param2);
void func_800307C4(f32, f32);
s32 func_80000608(Object*, Object*, u16, s8, s32, s32);
s32 func_800009C8(Object *arg0, Object *arg1, u16 arg2, s32 arg3);
void func_80023628(void);
s32 func_80048E04(u8, u8, u8, u8);
u8 map_get_is_object_streaming_disabled(void);
void objprint_func(Gfx**, Mtx**, Vertex**, Triangle**, Object*, s8);
void func_8001F81C(u8*, u8*, u8*);
u8 map_get_is_object_streaming_disabled(void);
void fit_aabb_around_cubes(AABBs32 *aabb, Vec3f *posArray1, Vec3f *posArray2, f32 *cubeRadiusArray, s32 arrayLength);
void func_800533D8(s32*, UnkFunc80051D68Arg3**);
void func_80053408(Vec3s32 **);
void func_80053750(Object*, AABBs32*, u8);
Unk8005341C *func_8005341C(s32*);
void warpPlayer(s32 warpID, s8 fadeToBlack);
Block* func_80044BB0(s32 blockIndex);
Texture* func_8004A1E8(s32 match_value);
ObjSetup* func_80044448(s32 match_uID, s32* match_indexInMap, s32* match_mapID, s32* arg3, s32* arg4);
void func_8004478C(f32 worldX, f32 worldY, f32 worldZ, f32* blockWorldOriginX, f32* blockWorldOriginZ);
void map_save_object(ObjSetup* objsetup, s32 mapID, f32 x, f32 y, f32 z);
u32 func_80041D8C(void);
void dl_set_prim_color_no_sync(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void dl_set_env_color_no_sync(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
u32 func_80041DA4(void);
void func_80041028(void);
void func_80040FF8(void);

#endif
