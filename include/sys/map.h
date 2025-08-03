#ifndef _SYS_MAP_H
#define _SYS_MAP_H

#include "PR/ultratypes.h"
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
/*0c*/    s8 Xmin; //Divided by 4
/*0d*/    s8 Xmax; //Divided by 4
/*0e*/    s8 Zmin; //Divided by 4
/*0f*/    s8 Zmax; //Divided by 4
/*10*/    s8 unk_10;
/*11*/    s8 unk_11;
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
/*0a*/    u8  height;

            //bits 6-7: unused
            //bit  5:   greyscale
            //bit  4:   opaque
            //bit  0-3: type (RGBA/CI/IA/etc.)
/*0b*/    u8  format;
/*0c*/    u8  terrain_type; //Affects footstep/weapon foley sfx 
                            //(some values create dust/snow particles too, 
                            //or force water behaviour - overriding facebatch water setting!)
} BlocksMaterial;

typedef struct {
/*00*/ s16 textureIndex;
/*02*/ u8 unk02;
/*03*/ u8 unk03;
} BlocksTextureIndexData;

typedef struct {
/*00*/    BlocksMaterial *ptr_materials;
/*04*/    u32 ptr_vertices;
/*08*/    u32 ptr_faces;
/*0c*/    FaceBatch *ptr_faceBatches;
/*10*/    u32 ptr_faceEdgeVectors;
/*14*/    u32 unk_14; // in ROM, sometimes matches grid row
/*18*/    HitsLine *ptr_hits_lines; // in ROM, sometimes matches grid column (HITS.bin data pointer at runtime)
/*1c*/    s32 unk_1c;
/*20*/    s16 unk_20;
/*22*/    s16 unk_22;
/*24*/    s32 unk_24;
/*28*/    BlocksTextureIndexData *unk_28; //textureIDs_ptr
/*2c*/    s16 unk_2c;
/*2e*/    s16 unk_2e;  
/*30*/    s16 flags; //Typical values: 0x10, 0x18 (affects malloc size - more needed needed for blocks with animated verts)
/*32*/    s16 vertex_count;
/*34*/    s16 face_count;
/*36*/    s16 faceBatch_count;
/*38*/    s16 hits_line_count; //value only present at runtime
/*3a*/    s8  unk_3a;
/*3b*/    s8  unk_3b;
/*3c*/    s16 unk_3c;
/*3e*/    s16 unk_3e; //pointer to animated vertices, maybe?
/*40*/    s16 minY; //lowest vertex elevation
/*42*/    s16 maxY; //highest vertex elevation
/*44*/    s16 modelSize; //length of decompressed model
    
//unconfirmed, but seems to be something like
//the number of times the texture/material settings
//change while going through the block's F3DEX2 commands
/*46*/    s16 textureLoadCount;

/*48*/    u8 unk_48; //texture_count (at runtime, distinct from material count since different materials can use same texture)
/*49*/    s8 unk_49;
/*4a*/    u8  material_count;
/*4b*/    s32 unk_4b;
/*4e*/    s8  unk_4e;
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
/*20*/  s32 *objectInstanceFile_ptr;
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
/*00*/ s16 unk0;
/*02*/ s16 unk2;
/*04*/ s16 unk4;
/*06*/ s16 unk6;
/*08*/ s16 unk8;
/*0A*/ s16 unkA;
/*0C*/ s16 unkC;
/*0E*/ s16 unkE;
/*10*/ s16 unk10;
/*12*/ s16 unk12;
/*14*/ s16 unk14;
/*16*/ s16 unk16;
/*18*/ s16 unk18;
/*1a*/ s16 unk1A;
/*1c*/ s16 unk1C;
/*1e*/ s16 unk1E;
/*20*/ u8 unk20;
/*21*/ u8 unk21;
} UnkTextureStruct;

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
    s16  *unk8;
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
    /*0x08*/ s16 unk8;
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

// size: 0xA
typedef struct MapLayoutArg0 {
/*00*/ s16 unk0;
/*02*/ s16 unk2;
/*04*/ s16 unk4;
/*06*/ s16 unk6;
/*08*/ s8  unk8;
/*09*/ s8  unk9;
} MapLayoutArg0;

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

typedef struct Unk800B96B0 {
    union {
        s32 unk0;
        struct {
            s16 pad0;
            u8 unk2;
            u8 pad3;
        } otherUnk0;
    };
    s16 unk4;
    s16 unk6;
    f32 unk8;
    f32 unkC;
    f32 unk10;
} Unk800B96B0;

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

typedef struct Unk80092BC0 {
    s16 unk0[3];
} Unk80092BC0;

typedef struct Unk800B98A0 {
    u8 unk0[4000];
} Unk800B98A0;

typedef struct Unk800B98A8 {
    u8 unk0[8000];
} Unk800B98A8;

typedef struct Unk800B98B0 {
    u8 unk0[6400];
} Unk800B98B0;

typedef struct Unk800BB158 {
    u8 unk0[4800];
} Unk800BB158;

typedef struct Unk800BB160 {
    u8 unk0[9600];
} Unk800BB160;

typedef struct Unk800BB168 {
    u8 unk0[11200];
} Unk800BB168;

typedef struct Unk8004E540 {
    u8 pad0[0x18];
    f32 x;
    f32 y;
    f32 z;
    u8 pad24[0x37 - 0x24];
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
} Unk8004E540;


// // // Might be incorrect definition but size should be correct
typedef struct Unk8004FA58 {
    Vec4f pos;
    u8 unk10;
} Unk8004FA58;

// Might be incorrect definition but size should be correct
typedef struct Unk8004FA58_Arg5 {
    s16 x;
    s16 y;
    s16 z;
    s16 pad6;
    s16 unk8;
    s16 unkA;
    u8 padC;
    u8 padD;
    s8 padE;
    s8 unkF;
} Unk8004FA58_Arg5;

typedef struct Unk80092C48 {
    f32 unk0[8];
    u8 pad20[0xC];
} Unk80092C48;

typedef struct UnkFunc80051D68Arg3 {
        f32 unk0;
        s16 unk4;
        s16 unk6;
        s16 unk8;
        s16 unkA[6];
        s16 unk16[6];
        u8 pad22[0x2F - 0x22];
        s8 unk30;
        u8 pad31[3];
} UnkFunc80051D68Arg3;

typedef struct UnkFunc80052300Arg3 {
    Vec3f unk0[3];
} UnkFunc80052300Arg3;

typedef struct Unk8005341C {
    Object *unk0;
    s16 unk4;
    u8 pad8[0x10 - 0x6];
} Unk8005341C;

#define MAX_RENDER_LIST_LENGTH 400
#define MAX_BLOCKS 40

/** The X/Z size of the world grid cells, in vertex position units */
#define BLOCKS_GRID_UNIT 640
#define BLOCKS_GRID_UNIT_F 640.0f
#define BLOCKS_GRID_SPAN 16
#define BLOCKS_GRID_TOTAL_CELLS 256
#define MAP_ID_MAX 80
#define SOME_SIZE 5

#define GRID_INDEX(z, x) (((z) * BLOCKS_GRID_SPAN + (x)))

extern Camera* D_800B51E4;
extern Gfx* gMainDL;
extern Mtx* gWorldRSPMatrices;
extern Vertex* D_800B51D4;
extern Triangle* D_800B51D8;

extern f32 gWorldX;
extern f32 gWorldZ;
extern Plane gFrustumPlanes[SOME_SIZE];
extern u32 gRenderList[MAX_RENDER_LIST_LENGTH];
extern s16 gRenderListLength;
extern Block *gBlocksToDraw[MAX_BLOCKS];
extern s16 gBlocksToDrawIdx;
extern BlockTexture *gBlockTextures;
extern u32 *gFile_HITS_TAB;

extern u8 *gMapReadBuffer;
extern s8 gMapNumStreamMaps;
extern StreamMap gMapStreamMapTable[];
extern MapHeader* gMapActiveStreamMap;
extern MapHeader* gLoadedMapsDataTable[120];

extern UnkTextureStruct *D_800B97A8;

/** An array of 5 pointers to 16*16 blockIndex arrays, one for each map layer (maybe called visGrids based on print strings?) */
extern s8 *gBlockIndices[SOME_SIZE];
extern u8 gLoadedBlockCount;
extern BlocksModel **gLoadedBlocks;
extern s16 *gLoadedBlockIds;
extern u8 *gBlockRefCounts;

extern u16 *gFile_TRKBLK;
extern s16 gNumTRKBLKEntries;

extern s32 gMapCurrentStreamCoordsX;
extern s32 gMapCurrentStreamCoordsZ;

extern GlobalMapCell *gDecodedGlobalMap[SOME_SIZE]; //16*16 grid of GlobalMapCell structs, one for each layer!
extern s8 *D_800B9700[SOME_SIZE];
extern s32 gNumTotalBlocks;
extern Unk800B96B0* D_800B96B0;
extern s32* gFile_BLOCKS_TAB; // unknown pointer type
extern MapsTabStruct* gFile_MAPS_TAB; // unknown pointer type
extern MapsUnk_800B97C0 *D_800B97C0; // 255 items

extern s8 D_80092A8C;
// D_800B5508 & D_800B5590 should probably use the same struct since they
// multiply their index by 0x8C however no idea what the struct looks like
extern s8 D_800B5508;
extern s8 D_800B5590;

extern DLBuilder D_800B4A20;
extern DLBuilder D_800B49F0;

extern u8 D_800B9794;
extern s16 D_800B979E;

extern s32 D_80092A94;

extern s32 D_80092BBC;

extern f32 D_800B97AC; //x
extern f32 D_800B97B0; //y
extern f32 D_800B97B4; //z

extern s16 D_800B4A70;
extern s16 D_800B4A72;
extern s8 D_800B96A8;

extern s8 D_80092A78; //gFadeDelayTimer
extern s8 D_800B4A58; //gStartWarp?
extern s8 D_800B4A59;
extern s16 D_800B4A5C;
extern s16 D_800B4A5E; //gFadeDelayTimerStarted
extern Warp *D_800B4A60;

extern s8 D_80092BE8;
extern s8 D_80092BF8;
extern s8 D_80092BFC;

extern f32 D_8009A950;
extern f32 D_8009A954;
extern f32 D_8009A958;
extern f32 D_8009A95C;

extern s32 D_800B9798;
extern s16 D_800B97C4;

extern Struct_D_800B9768 D_800B9768;
extern Unk80092BC0 D_80092BC0;
extern u8 D_800B1847;

extern f32 D_8009AA28;
extern f32 D_8009AA2C;
extern f32 D_800B97E0[24];
extern f32 D_800B9840[24];

extern Unk80092BC0 D_80092CA8;
extern Unk80092BC0 D_80092CB0;

extern Unk800B98A0 *D_800B98A0[2];
extern Unk800B98A8 *D_800B98A8[2];
extern Unk8004FA58_Arg5 *D_800B98B0[2];

extern s8 D_80092BF4;

// Some sort of toggle list
extern s8 D_80092C00;
extern s8 D_80092C04;
extern s8 D_80092C08;
extern s8 D_80092C0C;
extern s8 D_80092C10;
extern s8 D_80092C14;

extern s16 D_80092C24;
extern s16 D_80092C28;
extern s16 D_80092C2C;
extern s16 D_80092C30;
extern s16 D_80092C34;
extern s16 D_80092C38;

extern Unk800B98A8 *D_800BB140;
extern Unk800B98A8 *D_800BB144;
extern Unk8004FA58_Arg5 *D_800BB148;
extern Unk8004FA58_Arg5 *D_800BB14C;
extern Gfx *D_800BB150;
extern Unk800B98A0 *D_800BB154;
extern Unk800BB158 *D_800BB158[2];
extern Unk800BB160 *D_800BB160[2];
extern Unk800BB168 *D_800BB168[2];
extern Unk800BB168* D_800BB174;
extern Unk800BB168* D_800BB178;
extern Unk8004FA58_Arg5* D_800BB17C;
extern Unk8004FA58_Arg5* D_800BB180;
extern Gfx* D_800BB184;
extern Unk800BB158* D_800BB188;

extern Texture* D_800BB190;

extern f32 D_8009AA38; // = -0.200000003f;

// xyz pair
extern f32 D_80092BD0;
extern f32 D_80092BD4;
extern f32 D_80092BD8;

extern s16 D_80092C20;
extern s32 D_80092CA0;
extern f32 D_8009AA20;
extern s16 D_800B98B8[300];
extern s16 D_800B9B60[800];
extern f32 D_80092BE4;
extern s32 D_80092CA4;
extern u8 D_800B9B10[80];
extern f32 D_8009AA24;
extern s8 D_800BB170;
extern u8 D_8009A614;
extern s16 *D_800B97A0;
extern f32 D_80092AAC[24];

extern s16 (*D_800B9770)[2];
extern s16 (*gMapGridTable)[5];
extern s16 gNumTRKBLKEntries;

extern s32 D_80092BDC;
extern s32 D_80092BE0;
extern s16 D_80092C1C;
extern f32 D_8009AA1C;
extern Unk8004FA58 D_800BA1A0;
extern Unk80092C48 D_80092C48;
extern Unk80092C48 D_80092C74;
extern s16 D_80092C18;
extern f32 D_800BB18C;
extern f32 D_8009AA30;
extern f32 D_8009AA34;
extern f32 D_8009AA3C;
extern s8 D_80092A9C[8];
extern s32 D_80092A60;
extern s32 D_80092A64;
extern s32 D_800B4A50;
extern s32 D_800B4A54;
extern s16 D_800B97C4;

// Used in other .c files
void dl_triangles(Gfx **gdl, DLTri *tris, s32 triCount);

// defined in map.c but used before declared
void func_800441F4(u32* arg0, s32 arg1);
void func_80048B14(Block *block);
void func_80048C24(Block *block);
u32 hits_get_size(s32 id);
void block_setup_vertices(Block *block);
void block_setup_gdl_groups(Block *block);
void *block_setup_textures(Block *block);
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
void func_80045FC4(MapHeader* arg0, s32* arg1, s32 arg2, s32 arg3);
MapHeader *map_load_streammap(s32, s32);
void map_read_layout(MapLayoutArg0 *arg0, u8 *arg1, s16 arg2, s16 arg3, s32 maptabindex);
void func_80048034(void);
void map_update_objects_streaming(s32);
s32 func_800485FC(s32, s32, s32, s32, s32);
void func_80047404(s32, s32, s32*, s32*, s32*, s32*, s32, s32, s32);
void func_800496E4(s32 blockIndex);
void map_update_streaming(void);
s32 func_8004A058(Texture* tex, u32 flags, s32 arg2);
s32 func_8004B190(Object*);
void func_8004B548(MapHeader*, s32, s32, Object*);
s32 map_should_stream_load_object(ObjCreateInfo*, s8, s32);
s32 map_check_some_mapobj_flag(s32, u32);
void func_8004B710(s32 cellIndex_plusBitToCheck, u32 mapIndex, u32 arg2);
s32 func_8004AEFC(s32 mapID, s16 *arg1, s16 searchLimit);
s32 func_8004B4A0(ObjCreateInfo* obj, s32 arg1);
void func_80052230(Vec3f *A, Vec3f *B, f32 *arg2);
s32 func_8004FA4C(void);
s32 func_80052148(Vec3f* arg0, Vec3f* arg1);
void func_80052644(u8* source, u8* dest, s32 arg2, s32* outCount, Vec4f* arg4, s32 length, void (*arg6)(Vec3f*, Vec3f*, Vec3f*, f32), u8 someFlag);
f32 func_800528AC(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec4f* arg3);
s32 func_8004FA58(Object* arg0, s32 arg1, Unk8004FA58 *arg2, s32 arg3, Vec3f *arg4, Unk8004FA58_Arg5 *arg5, Unk8004FA58* arg6, s32 max);
void block_add_to_render_list(Block *block, f32 x, f32 z);
void func_800436DC(Object* arg0, s32 arg1);
s32 func_80045DC0(s32, s32, s32); //unsure of last arg
s32 map_find_streammap_index(s32);
s32 map_load_streammap_add_to_table(s32);  //unsure of worldGridZ here
void func_80051944(s32 arg0, Object* arg1, Vec3f* arg2, f32 arg3, s16 arg4);
void func_800516BC(Object* obj, Vec3f* arg1, f32 arg2);
s32 func_8004EEC0(Unk800B98B0*, Unk800B98A0*, ObjectStruct64*, Object*, s32, s32*);
s32 func_8004F378(Unk800B98B0* arg0, Gfx* arg1, ObjectStruct64* arg2, Object* arg3, s32 arg4, s32* arg5);
s32 func_800502AC(Object* arg0, Vec3f *arg1, Unk8004FA58* arg2, s32 arg3, Vec3f *arg4, Unk8004FA58_Arg5 *arg5, Unk8004FA58* arg6, s32 arg7);
void func_800511E8(Object*, s32, Vec3f*, Unk8004FA58*);
s32 func_80052300(Object* arg0, UnkFunc80051D68Arg3 *arg1, Unk8004FA58* arg2, UnkFunc80052300Arg3* arg3, s32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8);
s32 func_8004DBAC(Object* arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8004E7A8(Object* arg0);
void func_80051C54(Vec3f* A, Vec3f* B, Vec3f* C, Vec3f* D);
s32 func_80051F64(s16 arg0, s16 arg1, s16 *arg2, s16 *arg3);
s32 func_80051CFC(Vec3f* arg0, Vec3f* arg1);
void func_80050B88(Object* arg0, Vec3f* arg1, Unk8004FA58* arg2, Unk8004FA58* arg3, Unk8004FA58* arg4, Unk8004FA58* arg5, s16* arg6, s16* arg7, f32 arg8, s16* arg9, s32 argA);

// other funcs that are used
void func_80058F3C(void);
void func_800143A4(void);
void func_8001EB80(void);
s32 func_80010048(void);
void some_video_setup(s32 param1);
void func_80012B54(s32 param1, s32 param2);
void func_800307C4(f32, f32);
void func_80000608(Object*, Object*, u16, s32, s32, s32);
void func_800009C8(Object*, Object*, u16, s32);
void func_80023628(void);
s32 func_80048E04(u8, u8, u8, u8);
void func_8003E648(Texture*, s32, s32);
Object **obj_get_all_of_type(s32 idx, s32 *count);
u8 map_get_is_object_streaming_disabled(void);
void objprint_func(Gfx**, Mtx**, Vertex**, Triangle**, Object*, s32);
void func_8001F81C(u8*, u8*, u8*);
void func_8005C740(s32, s32*, s32*, s32);
void func_8003DB7C(void);
void func_8003DBCC(void);
void func_8005B870(void);
u8 map_get_is_object_streaming_disabled(void);
void fit_aabb_around_cubes(AABBs32 *aabb, Vec3f *posArray1, Vec3f *posArray2, f32 *cubeRadiusArray, s32 arrayLength);
void func_800533D8(s32*, s32*);
void func_80053408(Vec3f *);
void func_80053750(Object*, Vec3f**, s32);
void func_8005BCE0(Object*, s16, s32, s32, s32, s32, s32, f32, s32);
Unk8005341C *func_8005341C(s32*);

#endif
