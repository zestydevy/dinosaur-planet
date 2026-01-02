/** Model system
 */

#ifndef _SYS_GFX_MODEL_H
#define _SYS_GFX_MODEL_H
#include "texture.h"
#include "animation.h"
#include "sys/math.h"

typedef enum {
    MODFLAGS_NONE = 0x0,
    MODFLAGS_1 = 0x1,
    MODFLAGS_SHADOW = 0x2,
    MODFLAGS_4 = 0x4,
    MODFLAGS_8 = 0x8,
    MODFLAGS_10 = 0x10,
    MODFLAGS_20 = 0x20,
    MODFLAGS_EVENTS = 0x40,
    MODFLAGS_80 = 0x80,
    MODFLAGS_100 = 0x100,
    // Don't load any model, even if the object defines some.
    MODFLAGS_DONT_LOAD_MODEL = 0x200,
    // Only load the model specified by MODFLAGS_MODEL_INDEX.
    MODFLAGS_LOAD_SINGLE_MODEL = 0x400
} ModelFlags;

// Add to model flags to specify which model to load when LOAD_SINGLE_MODEL is set.
#define MODFLAGS_MODEL_INDEX(index) (index << 11)
#define MODFLAGS_GET_MODEL_INDEX(flags) (((s32)flags >> 11) & 0xF)

typedef struct {
/*0000*/    Texture *texture; //textureID in ROM
/*0004*/    u32 unk4;
} ModelTexture;

typedef struct {
/*0000*/    Texture *texture; //textureID in ROM
/*0004*/    u8 width;
/*0005*/    u8 height;
/*0006*/    u8 format;
/*0007*/    u8 unk7;
/*0008*/    u32* unk8;
/*000c*/    u32* unkC;
/*0010*/    s16 unk10;
/*0012*/    s16 unk12;
} ModelTextureAlt;

typedef struct {
/*0000*/    u8 materialID;
/*0001*/    s8 jointID_A;
/*0002*/    s8 jointID_B;
/*0003*/    s8 jointID_C;
/*0004*/    s8 jointID_B_firstVertexBufferIndex;
/*0005*/    s8 jointID_C_firstVertexBufferIndex;
/*0006*/    s16 baseVertexID;
/*0008*/    s16 baseF3DCommandIndex;
/*000a*/    s8 tagA; //0xFF = glow marker (though not what enables effect)
/*000b*/    s8 tagB; //UV-animated faces tend to have 1 or 2 here, other values for flipbooks
/*000c*/    s32 tagC; // @ 0xC: 0x80 = double-sided faces (no back culling)
                      // @ 0xD: 0x11 = decal, 0x20 = not in F3D block
                      // @ 0xE: Upper 4 bits enable envMap, lower bits are baseFaceID
} ModelFacebatch;

typedef struct {
/*0000*/	s8 parentJointID;
/*0001*/	s8 jointLayer_plusID; //upper bit determines layering (usually upper torso joints, say for layering weapon stowing anim over walk cycle anim dynamically)
/*0002*/	s8 jointID_duplicate1;
/*0003*/	s8 jointID_duplicate2;
/*0004*/	f32 x;
/*0008*/	f32 y;
/*000C*/	f32 z;
} ModelJoint;

typedef struct {
/*00*/ s16 totalInfluences; //The number of vertexGroups affected by each blendshape
/*02*/ s16 headerLength;
/*04*/ s16 totalBlendshapes;
/*06*/ s16 influences; //Array of vertexGroupIDs
} BlendshapeHeader;

enum BlendshapeLayers {
    BLENDSHAPE_LAYER_EYES = 0,
    BLENDSHAPE_LAYER_UNK = 1,
    BLENDSHAPE_LAYER_MOUTH = 2,
    BLENDSHAPE_LAYER_LIMIT = 3
};

typedef struct {
/*0000*/	Gfx gfx; //when opaque?
/*0008*/	Gfx gfx2; //when fading?
/*0010*/	s16 idx; //commandIndex to swap out with either gfx or gfx2
/*0012*/	u8 unk12[0x18 - 0x12];
} ModelDLInfo;

typedef struct {
/*0000*/    s16 jointIndex;
/*0002*/    s16 unk2; // radius?
/*0004*/    s16 x;
/*0006*/    s16 y;
/*0008*/    s16 z;
/*000A*/    u16 unkA;
/*000C*/    s8 unkC;
/*000D*/    s8 unkD;
} HitSphere;

typedef struct{
    // TODO
/*0000*/    ModelTexture *materials;
/*0004*/    Vtx *vertices;
/*0008*/    ModelFacebatch *faces; //contains materialID ref, joint assignments, f3dex command index, base faceID, etc.
/*000C*/    Gfx *displayList; //f3dex2 block
/*0010*/    Animation **anims;
/*0014*/    void *vertexGroupOffsets;
/*0018*/    void *vertexGroups; //used by blendshapes, lighting system - contains copy of group vertices' colour, plus normal vector
/*001C*/    BlendshapeHeader *blendshapes;
/*0020*/    ModelJoint *joints;
/*0024*/    u8 *amap;
/*0028*/    HitSphere *hitSpheres;
/*002C*/    void *edgeVectors; //only on mobile map models?
/*0030*/    s16 *modAnim;
/*0034*/    void *facebatchBounds; //bounding boxes for each facebatch
/*0038*/    ModelDLInfo *drawModes; //stores command indices of f3dex2 block's EF command settings, plus 2 settings to swap between
/*003C*/    void *textureAnimations; //for animating pupil UVs/eyelid flipbooks
/*0040*/    s16 modAnimBankBases[8]; //the base modanim index of each modanim bank
/*0050*/    f32 *collisionA; //joint pushback collision A
/*0054*/    f32 *collisionB; //joint pushback collision B
/*0058*/    u32 decompressedSize;
/*005C*/    u32 unk5C; //amap-related?
/*0060*/    s16 maxAnimatedVertDistance; //max radial distance (from model origin) to a vert throughout model's animations (used to simplify collision tests)
/*0062*/    s16 vertexCount;
/*0064*/    u16 faceCount;
/*0066*/    s16 animCount;
/*0068*/    s16 unk68;
/*006A*/    s16 modelId;
/*006C*/    s16 displayListLength; //f3dex2 command count
/*006E*/    u8 hitSphereCount; //hitsphere_count
/*006F*/    u8 jointCount; //joints_count
/*0070*/    u8 unk70; //mesh settings bitfield (bit0 allows simultaneous eye/mouth blendshapes, bit3 involved in texturing?)
/*0071*/    u8 unk71; //animation-related bitfield?
/*0072*/    u8 refCount; 
/*0073*/    u8 textureCount; //material_count
/*0074*/    u8 envMapCount;
/*0075*/	u8 drawModesCount;
/*0076*/	u8 textureAnimationCount;
/*0077*/	u8 unk77;
/*0078*/	u32 unk78;
/*007c*/	u32 unk7c;
} Model;

// size: 0x34
typedef struct ModelInstance_0x14_0x14 {
/*0000*/    s32 unk0[3];
/*000C*/    s32 unkC[3];
/*0018*/    Vec3f unk18;
/*0024*/    Vec3f unk24;
/*0030*/    s32 unk30;
} ModelInstance_0x14_0x14;

// size: 0x1C
typedef struct ModelInstance_0x14 {
/*0000*/    Vec3f *unk0;
/*0004*/    f32 *unk4;
/*0008*/    f32 *unk8;
/*000C*/    f32 *unkC;
/*0010*/    f32 *unk10;
/*0014*/    ModelInstance_0x14_0x14 *unk14;
/*0018*/    s8 *unk18;
} ModelInstance_0x14;

typedef struct {
/*0000*/    f32 strength;
/*0004*/    f32 unk4;
/*0008*/    f32 unk8;
/*000C*/    s8 unkC;
/*000D*/    s8 id;
/*000E*/    u8 unkE;
} ModelInstanceBlendshape;

typedef struct {
    // TODO
/*0000*/    Model *model;
/*0004*/    Vtx *vertices[2];
/*000C*/    MtxF *matrices[2];
/*0014*/    ModelInstance_0x14 *unk14;
/*0018*/    Gfx *displayList;
/*001C*/    void *unk1C[2];
/*0024*/    Vec4f *unk24;
/*0028*/    AnimState *animState0;
/*002C*/    AnimState *animState1;
/*0030*/    ModelInstanceBlendshape *blendshapes;
/*0034*/    u16 unk34; // stores index for matrices / vertices, first bit = matrix index, second bit = vertex index, third bit = unk1C index
} ModelInstance;

typedef struct{
/*0000*/    s32 vertexMalloc;
/*0004*/    s32 hitSphereMalloc;
/*0008*/    s32 unk8;
/*000C*/    s32 unkC;
/*0010*/    s32 blendShapeMalloc;
/*0014*/    s32 unk14;
/*0018*/    s32 jointsMalloc;
} ModelStats;

typedef struct {
/*0000*/    s16 animCount;
/*0002*/    s16 largestAnimSize;
/*0004*/    s16 unk4; //malloc-related Boolean (often set on character models)
/*0006*/    s16 unk6; //always 0 in ROM
/*0008*/    u32 uncompressedSize; // CAUTION: little-endian
} ModelHeader;

typedef struct {
/*0000*/    s32 id;
/*0004*/    Model *model;
} ModelSlot;

void func_8001AF04(ModelInstance *modelInst, s32 param2, s32 param3, f32 param4, s32 param5, s32 param6);
void func_8001AFCC(ModelInstance *modelInst, s32 param2, f32 param3);
void func_8001B084(ModelInstance *modelInst, f32 updateRate);
Animation* anim_load(s16 animId, s16 modanimId, AmapPlusAnimation* anim, Model* model);
s32 modanim_load(Model* model, s32 id, u8* data);
void anim_destroy(Animation*);
void func_800199A8(MtxF *arg0, ModelInstance *modelInst, AnimState *animState, f32 arg3, u32 arg4);
void func_80019FC0(MtxF *arg0, ModelInstance *modelInst, AnimState *animState, f32 arg3, u32 arg4, u8 animIdx0, u8 arg6, u8 animIdx1, u8 flags, s16 arg9);
void func_8001A3FC(ModelInstance *modelInst, u32 selector, s32 idx, f32 arg3, f32 scale, Vec3f *arg5, s16 *arg6);
void func_8001B100(ModelInstance* modelInst);
void load_model_display_list(Model *model, ModelInstance *modelInst);
void patch_model_display_list_for_textures(Model* model);
s32 model_load_anim_remap_table(s32 modelID, s32 arg1, s32 animCount);
void model_setup_anim_playback(ModelInstance* arg0, AnimState* animState);
ModelInstance* createModelInstance(Model* model, s32 flags, s32 arg2);
u32 model_get_stats(Model* model, s32 settingsBitfield, ModelStats* stats, s32 blendshapeSetting);

#endif //_SYS_GFX_MODEL_H
