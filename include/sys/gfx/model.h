/** Model system
 */

#ifndef _SYS_GFX_MODEL_H
#define _SYS_GFX_MODEL_H
#include "texture.h"
#include "animation.h"
#include "sys/math.h"

typedef struct {
/*0000*/    Texture *texture; //textureID in ROM
/*0004*/    u32 unk_0x4;
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
/*000c*/    s8 tagC; //0x80 = double-sided faces (no back culling)
/*000d*/    s8 tagD; //0x11 = decal, 0x20 = not in F3D block
/*000e*/    s16 tagE; //Upper 4 bits enable envMap, lower bits are baseFaceID
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
/*0000*/	Gfx gfx; //when opaque?
/*0008*/	Gfx gfx2; //when fading?
/*0010*/	s16 idx; //commandIndex to swap out with either gfx or gfx2
/*0012*/	u8 unk_0x12[0x18 - 0x12];
} ModelDLInfo;

typedef struct {
/*0000*/    s16 jointIndex;
/*0002*/    u8 pad0[0xA - 0x2];
/*000A*/    u16 unkA;
/*000C*/    s8 unkC;
/*000D*/    s8 unkD;
} HitSphere;

typedef struct{
    // TODO
/*0000*/    ModelTexture *materials; //materials
/*0004*/    Vtx *vertices; //vertexData
/*0008*/    ModelFacebatch *faces; //faces datablock (materialID ref, joint assignments, f3dex command index, base faceID)
/*000C*/    Gfx *displayList; //f3dex2 block
/*0010*/    Animation **anims;
/*0014*/    void *vertexGroupOffsets; //vertexGroup_offsets
/*0018*/    void *vertexGroups; //vertexGroups (used by blendshapes, lighting system - contains copy of group vertices' colour, plus normal vector)
/*001C*/    void *blendshapes; //blendshapes
/*0020*/    ModelJoint *joints; //joint_hierarchy
/*0024*/    u8 *amap;
/*0028*/    HitSphere *hitSpheres; //hitspheres
/*002C*/    void *edgeVectors; //edgevectors (only on mobile map models?)
/*0030*/    u32 unk_0x30;
/*0034*/    void *facebatchBounds; //bounding boxes for each facebatch
/*0038*/    ModelDLInfo *drawModes; //drawModes (stores command indices of f3dex2 block's EF command settings, plus 2 settings to swap between)
/*003C*/    void *textureAnimations; //textureAnimations (pupil UVs/eyelid flipbooks)
/*0040*/    u32 unk_0x40;
/*0044*/    u32 unk_0x44;
/*0048*/    u32 unk_0x48;
/*004C*/    u32 unk_0x4c;
/*0050*/    f32 *collisionA; //joint pushback collision A
/*0054*/    f32 *collisionB; //joint pushback collision B
/*0058*/    u32 decompressedSize; //length of decompressed model
/*005C*/    u32 unk_0x5c;
/*0060*/    s16 maxAnimatedVertDistance; //max radial distance (from model origin) to a vert throughout model's animations (used to simplify collision tests)
/*0062*/    s16 vertexCount; //vertex_count
/*0064*/    u16 faceCount; //face_count
/*0066*/    s16 animCount;
/*0068*/    s16 unk_0x68;
/*006A*/    s16 modelId;
/*006C*/    s16 displayListLength; //f3dex2 command count
/*006E*/    u8 hitSphereCount; //hitsphere_count
/*006F*/    u8 jointCount; //joints_count
/*0070*/    u8 unk_0x70; //mesh settings bitfield (bit0 allows simultaneous eye/mouth blendshapes, bit3 involved in texturing?)
/*0071*/    u8 unk_0x71; 
/*0072*/    u8 refCount; 
/*0073*/    u8 textureCount; //material_count
/*0074*/    u8 envMapCount; //envMap_material_count
/*0075*/	u8 drawModesCount; //drawModes_count
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
/*0008*/    s32 pad8;
/*000C*/    f32 *unkC;
/*0010*/    f32 *unk10;
/*0014*/    ModelInstance_0x14_0x14 *unk14;
/*0018*/    s8 *unk18;
} ModelInstance_0x14;

typedef struct {
/*0000*/    f32 unk0x0;
/*0004*/    f32 unk0x4;
/*0008*/    f32 unk0x8;
/*000C*/    s8 unk0xC;
/*000D*/    s8 unk0xD;
/*000E*/    u8 unk0xE;
} ModelInstance_0x30;

typedef struct {
    // TODO
/*0000*/    Model *model;
/*0004*/    void *unk_0x4[2];
/*000C*/    MtxF *matrices[2];
/*0014*/    ModelInstance_0x14 *unk_0x14;
/*0018*/    Gfx *displayList;
/*001C*/    s32 unk_0x1c[2];
/*0024*/    f32 *unk_0x24;
/*0028*/    AnimState *animState0;
/*002C*/    AnimState *animState1;
/*0030*/    ModelInstance_0x30 *unk_0x30;
/*0034*/    u16 unk_0x34;
} ModelInstance;

void func_8001AFCC(ModelInstance *modelInst, s32 param2, f32 param3);
void func_8001B084(ModelInstance *modelInst, f32 param2);

#endif //_SYS_GFX_MODEL_H
