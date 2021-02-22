#ifndef _COMMON_STRUCTS_H
#define _COMMON_STRUCTS_H

#include "ultra64.h"

typedef f32 quartic[5];

typedef enum geoPayloadType {
    axis=23,
    axisDEG=19,
    axisDEG_translate=20,
    axis_translate=24,
    axis_translate_scale=25,
    bone=51,
    euler=29,
    eulerDEG_XYZ=21,
    eulerDEG_translate=22,
    eulerTAB=26,
    eulerTAB_translate=27,
    eulerTAB_translate_scale=28,
    euler_scale_translate=31,
    euler_translate=30,
    euler_translate_conjScale=54,
    scale=32,
    scale_proj=44,
    scaled_viewproj=50,
    set_viewproj=42,
    tempScale=53,
    translate=18,
    xform_eulerTAB_trans_scale=63,
    zrot_viewproj=46
} geoPayloadType;

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x000 */ s32 regs[50];
    /* 0x0C8 */ char unk_0C8[4];
    /* 0x0CC */ s32 gp;
    /* 0x0D0 */ char unk_0D0[4];
    /* 0x0D4 */ s32 sp_upper;
    /* 0x0D8 */ void* sp;
    /* 0x0DC */ s32 s8_up;
    /* 0x0E0 */ s32 s8;
    /* 0x0E4 */ u32 ra_up;
    /* 0x0E8 */ void* ra;
    /* 0x0EC */ s32 lo_up;
    /* 0x0F0 */ s32 lo;
    /* 0x0F4 */ s32 hi_up;
    /* 0x0F8 */ u32 hi;
    /* 0x0FC */ u32 sr;
    /* 0x100 */ void* pc;
    /* 0x104 */ u32 cause;
    /* 0x108 */ u32 badvaddr;
    /* 0x10C */ u32 rcp;
    /* 0x110 */ u32 fpcsr;
    /* 0x114 */ f32 fregs[32];
} OSThreadContext; // size = 0x194

struct OSThread {
    /* 0x000 */ OSThread* next;
    /* 0x004 */ s32 priority;
    /* 0x008 */ OSThread** queue;
    /* 0x00C */ OSThread* tlnext;
    /* 0x010 */ u16 state;
    /* 0x012 */ u16 flags;
    /* 0x014 */ s32 id;
    /* 0x018 */ s32 fp;
    /* 0x01C */ OSThreadContext context;
    /* 0x1B0 */ char unk_1B0[0x8];
    /* 0x1B8 */ void* stackInfo;
};

typedef struct { /* OMMtx */
    /* 0x00 */ char unk_00[0x4];
    /* 0x04 */ geoPayloadType type;
} nodePayload; // size = 0x44;

typedef struct xformData {
    /* 0x00 */ char unk_00[0x4];
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ Vec3f translation;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ f32 angle;
    /* 0x1C */ Vec3f euler;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ Vec3f scale;
} xformData; // size = 0x38

typedef struct {
    /* 0x00 */ s8 type; /* 0=linear;1=bezier;2=hermite/6 */
    /* 0x01 */ char unk_01[0x1];
    /* 0x02 */ s16 length;
    /* 0x04 */ f32 invSegTime;
    /* 0x08 */ Vec3f* pts;
    /* 0x0C */ f32 duration;
    /* 0x10 */ f32* times;
    /* 0x14 */ quartic* quartics;
} pathSpline;

typedef struct {
    /* 0x00 */ struct uvOP* next;
    /* 0x04 */ s8 value;
    /* 0x05 */ s8 op;
    /* 0x06 */ char unk_06[0x2];
    /* 0x08 */ f32 ref;
    /* 0x0C */ f32 t;
    /* 0x10 */ f32 p0;
    /* 0x14 */ f32 p1;
    /* 0x18 */ f32 v0;
    /* 0x1C */ f32 v1;
    /* 0x20 */ struct pathSpline* path;
} uvOP;

typedef struct {
    /* 0x00 */ u32 type;
    /* 0x04 */ u32 flags;
    /* 0x08 */ s32 inc;
} uvState;

typedef struct { /* MObj s32ernally */
    /* 0x00 */ struct uvScroll* next;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ u16 h_8;
    /* 0x0A */ s8 fmt1;
    /* 0x0B */ s8 siz1;
    /* 0x0C */ u32* textures;
    /* 0x10 */ u16 stretch;
    /* 0x12 */ u16 sharedOffset;
    /* 0x14 */ u16 t0w;
    /* 0x16 */ u16 t0h;
    /* 0x18 */ s32 halve;
    /* 0x1C */ f32 xFrac0;
    /* 0x20 */ f32 yFrac0;
    /* 0x24 */ f32 xScale;
    /* 0x28 */ f32 yScale;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ u32* palettes;
    /* 0x38 */ u16 flags;
    /* 0x3A */ s8 fmt2;
    /* 0x3B */ s8 siz2;
    /* 0x3C */ u16 w2;
    /* 0x3E */ u16 h2;
    /* 0x40 */ u16 t1w;
    /* 0x42 */ u16 t1h;
    /* 0x44 */ f32 xFrac1;
    /* 0x48 */ f32 yFrac1;
    /* 0x4C */ f32 unk_4C;
    /* 0x50 */ s32 unk_50;
    /* 0x54 */ s32 unk_54;
    /* 0x58 */ s8 primR;
    /* 0x59 */ s8 primG;
    /* 0x5A */ s8 primB;
    /* 0x5B */ s8 primA;
    /* 0x5C */ char unk_5C[0x1];
    /* 0x5D */ s8 minLOD;
    /* 0x5E */ char unk_5E[0x2];
    /* 0x60 */ s8 envR;
    /* 0x61 */ s8 envG;
    /* 0x62 */ s8 envB;
    /* 0x63 */ s8 envA;
    /* 0x64 */ s8 blendR;
    /* 0x65 */ s8 blendG;
    /* 0x66 */ s8 blendB;
    /* 0x67 */ s8 blendA;
    /* 0x68 */ u32 lightColor1;
    /* 0x6C */ u32 lightColor2;
    /* 0x70 */ char unk_70[0x10];
    /* 0x80 */ u16 texIndex1;
    /* 0x82 */ u16 texIndex2;
    /* 0x84 */ f32 primLOD;
    /* 0x88 */ f32 palIndex;
    /* 0x8C */ char unk_8C[0x4];
    /* 0x90 */ uvOP* ops;
    /* 0x94 */ uvState* states;
    /* 0x98 */ f32 offset;
    /* 0x9C */ f32 increment;
    /* 0xA0 */ f32 unk_A0;
    /* 0xA4 */ char unk_A4[0x4];
} uvScroll; // size = 0xA8;

typedef struct geoNode { /* top level rendering object */
    /* 0x00 */ struct geoNode* unk_00;
    /* 0x04 */ struct GObj* gobj;
    /* 0x08 */ struct geoNode* next;
    /* 0x0C */ struct geoNode* prev;
    /* 0x10 */ struct geoNode* child;
    /* 0x14 */ struct geoNode* parent;
    /* 0x18 */ nodePayload* newPayload;
    /* 0x1C */ Vec3f translation;
    /* 0x28 */ char unk_28[0x4];
    /* 0x2C */ f32 degrees;
    /* 0x30 */ Vec3f euler;
    /* 0x3C */ f32 unk_3C;
    /* 0x40 */ Vec3f scale;
    /* 0x4C */ xformData* xform;
    /* 0x50 */ s32 nodeGraphics;
    /* 0x54 */ s8 stateFlags;
    /* 0x55 */ s8 firstState;
    /* 0x56 */ s8 nextIndex;
    /* 0x57 */ char unk_57[0x1];
    /* 0x58 */ nodePayload* payloads[5];
    /* 0x6C */ uvOP* ops;
    /* 0x70 */ uvState* uvState;
    /* 0x74 */ f32 untilUpdate; /* see uvScroll offset */
    /* 0x78 */ f32 inc;
    /* 0x7C */ f32 currFrame;
    /* 0x80 */ uvScroll* uvScrolls;
    /* 0x84 */ char unk_84[0x4];
} geoNode; // size = 0x88;

typedef struct {
    /* 0x00 */ struct signalLL* next;
    /* 0x04 */ struct GObj* gobj;
    /* 0x08 */ s32 data;
} signalLL;

typedef struct {
    /* 0x00 */ f32 nodeInc;
    /* 0x04 */ f32 field_0x4;
    /* 0x08 */ uvState** animList;
    /* 0x0C */ uvState*** matAnims;
    /* 0x10 */ s32* ids;
} animationHeader;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ void* state;
    /* 0x08 */ f32 radius;
    /* 0x0C */ void* aux;
} idFuncStruct;

typedef struct {
    /* 0x00 */ animationHeader* animations;
    /* 0x04 */ void* func;
    /* 0x08 */ s8 kind;
    /* 0x09 */ char unk_09[0x3];
    /* 0x0C */ void* unk_0C;
    /* 0x10 */ idFuncStruct* idfuncs;
} animalAnimationSetup;

typedef struct {
    /* 0x0 */ s8 r;
    /* 0x1 */ s8 g;
    /* 0x2 */ s8 b;
    /* 0x3 */ s8 a;
} color;

typedef struct {
    /* 0x00 */ u16 h_0;
    /* 0x02 */ s8 fmt1;
    /* 0x03 */ s8 siz1;
    /* 0x04 */ u32 * textures;
    /* 0x08 */ u16 stretch;
    /* 0x0A */ u16 sharedOffset;
    /* 0x0C */ u16 t0_w;
    /* 0x0E */ u16 t0_h;
    /* 0x10 */ s32 halve;
    /* 0x14 */ f32 t0_xShift;
    /* 0x18 */ f32 t0_yShift;
    /* 0x1C */ f32 xScale;
    /* 0x20 */ f32 yScale;
    /* 0x24 */ f32 unk_24;
    /* 0x28 */ f32 unk_28;
    /* 0x2C */ void* palettes;
    /* 0x30 */ u16 flags;
    /* 0x32 */ s8 fmt2;
    /* 0x33 */ s8 siz2;
    /* 0x34 */ u16 w2;
    /* 0x36 */ u16 h2;
    /* 0x38 */ u16 t1_w;
    /* 0x3A */ u16 t1_h;
    /* 0x3C */ f32 t1_xShift;
    /* 0x40 */ f32 t1_yShift;
    /* 0x44 */ f32 unk_44;
    /* 0x48 */ f32 unk_48;
    /* 0x4C */ s32 unk_4C;
    /* 0x50 */ color prim;
    /* 0x54 */ s8 primLODFrac;
    /* 0x55 */ s8 unk_55;
    /* 0x56 */ s8 unk_56;
    /* 0x57 */ char unk_57[0x1];
    /* 0x58 */ color env;
    /* 0x5C */ color blend;
    /* 0x60 */ color light1;
    /* 0x64 */ color light2;
    /* 0x68 */ s32 unk_68;
    /* 0x6C */ s32 unk_6C;
    /* 0x70 */ s32 unk_70;
    /* 0x74 */ s32 unk_74;
} uvScrollData;

typedef union { /* lower word of an f3d command */
    void* field0;
    u32 field1;
} cmdLow;

typedef struct {
    u32 upper;
    cmdLow lower;
} displayCommand;

typedef struct {
    /* 0x00 */ u32 index;
    /* 0x04 */ displayCommand* dl;
    /* 0x08 */ Vec3f trans;
    /* 0x14 */ Vec3f euler;
    /* 0x20 */ Vec3f scale;
} nodeTreeEntry;

typedef struct { /* room data pos32ers */
    /* 0x00 */ void* dList;
    /* 0x04 */ uvScrollData*** scrollList;
    /* 0x08 */ uvState*** field_0x8;
    /* 0x0C */ void (*renderer)(struct GObj*);
    /* 0x10 */ nodeTreeEntry* treeEntries;
    /* 0x14 */ u32 smallNum;
    /* 0x18 */ uvState** states;
    /* 0x1C */ u32 hundred;
    /* 0x20 */ u32 zero;
} room;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f euler;
    /* 0x1C */ Vec3f scale;
} otherObj;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 behavior;
    /* 0x08 */ Vec3f translation;
    /* 0x14 */ Vec3f euler;
    /* 0x20 */ Vec3f scale;
    /* 0x2C */ pathSpline* path;
} objectSpawn;

typedef struct {
    /* 0x00 */ room* room;
    /* 0x04 */ Vec3f offset; /* divided by 100 */
    /* 0x10 */ f32 roomYaw;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ otherObj* unk_18; /* -1 id terminates */
    /* 0x1C */ objectSpawn* objects;
    /* 0x20 */ otherObj* unk_20;
} roomDescriptor;

typedef struct {
    /* 0x00 */ geoNode* unk_00;
    /* 0x04 */ void* unk_04;
    /* 0x08 */ f32 unk_08;
} roomgfx_18;

typedef struct {
    /* 0x00 */ s32 index;
    /* 0x04 */ roomDescriptor* roomDesc;
    /* 0x08 */ struct roomGFX* prev;
    /* 0x0C */ struct roomGFX* next;
    /* 0x10 */ struct GObj* blockModel;
    /* 0x14 */ struct GObj* blockUV;
    /* 0x18 */ roomgfx_18* unk_18;
} roomGFX;

typedef union {
    f32 field0;
    s32 field1;
} misc;

typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ Vec3f normal;
    /* 0x10 */ s32 type;
} groundResult; // size = 0x14

typedef struct {
    /* 0x00 */ nodeTreeEntry* tree;
    /* 0x04 */ uvScrollData*** materials;
    /* 0x08 */ void (*renderer)(struct GObj*);
    /* 0x0C */ animalAnimationSetup* animSetup;
    /* 0x10 */ Vec3f scale;
    /* 0x1C */ Vec3f scaleNumerator;
    /* 0x28 */ f32 radius;
    /* 0x2C */ u16 flags;
    /* 0x2E */ geoPayloadType payload1;
    /* 0x2F */ geoPayloadType payload2;
    /* 0x30 */ geoPayloadType payload3;
    /* 0x31 */ char unk_31[0x3];
    /* 0x34 */ f32 unk_34;
} animalInitData;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 flag;
    /* 0x08 */ s16 s;
    /* 0x0A */ s16 t;
    /* 0x0C */ s8 r;
    /* 0x0D */ s8 g;
    /* 0x0E */ s8 b;
    /* 0x0F */ s8 a;
} vertex;

typedef struct {
    /* 0x00 */ vertex* data;
    /* 0x04 */ vertex** tempBuffer;
    /* 0x08 */ s32 vertexCount;
    /* 0x0C */ s32 end;
    /* 0x10 */ f32 timer;
} eggStruct;

typedef struct {
    /* 0x000 */ s8 id;
    /* 0x001 */ char unk_01[0x1];
    /* 0x002 */ s8 appleID;
    /* 0x003 */ char unk_03[0x5];
    /* 0x008 */ u16 flags;
    /* 0x00A */ char unk_0A[0x6];
    /* 0x010 */ s32 tangible;
    /* 0x014 */ s32 loopCount;
    /* 0x018 */ char unk_18[0x4];
    /* 0x01C */ Vec3f pos1;
    /* 0x028 */ Vec3f targetPos;
    /* 0x034 */ Vec3f dirToNeighbors;
    /* 0x040 */ Vec3f euler;
    /* 0x04C */ Vec3f collisionOffset;
    /* 0x058 */ f32 collisionRadius;
    /* 0x05C */ animalAnimationSetup* animSetup; /* Created by retype action */
    /* 0x060 */ struct GObj* s32eractionProc; /* Created by retype action */
    /* 0x064 */ struct GObj* apple;
    /* 0x068 */ f32 s32eractionDist;
    /* 0x06C */ f32 playerDist;
    /* 0x070 */ struct GObj* s32eractionTarget;
    /* 0x074 */ roomGFX* someRoom;
    /* 0x078 */ char unk_78[0x8];
    /* 0x080 */ uvState** animators; /* Created by retype action */
    /* 0x084 */ uvState*** matAnims; /* Created by retype action */
    /* 0x088 */ s32 behavior;
    /* 0x08C */ u32 processFlags; /* Created by retype action */
    /* 0x090 */ s32 counter;
    /* 0x094 */ struct GObj* pathProcess; /* Created by retype action */
    /* 0x098 */ f32 hSpeed;
    /* 0x09C */ f32 jumpVel;
    /* 0x0A0 */ f32 facingYaw; /* Created by retype action */
    /* 0x0A4 */ s32 animalLoopTarget;
    /* 0x0A8 */ f32 lastAnimationFrame;
    /* 0x0AC */ idFuncStruct* transitionGraph; /* Created by retype action */
    /* 0x0B0 */ misc miscVars[7];
    /* 0x0CC */ f32* forbiddenGround;
    /* 0x0D0 */ groundResult currGround; /* Created by retype action */
    /* 0x0E4 */ char unk_E4[0x4];
    /* 0x0E8 */ pathSpline* path;
    /* 0x0EC */ f32 pathParam; /* Created by retype action */
    /* 0x0F0 */ animalInitData* initData;
    /* 0x0F4 */ animationHeader* animHeader;
    /* 0x0F8 */ eggStruct* eggGeo;
    /* 0x0FC */ char unk_FC[0x4];
    /* 0x100 */ Vec3f collPosition;
    /* 0x10C */ char unk_10C[0x2];
    /* 0x100 */ s16 field_0x10e;
} animal; // size = 0x110

typedef struct {
    /* 0x00 */ char unk_00[0x1];
    /* 0x01 */ s8 oob;
    /* 0x02 */ s8 unk_02;
    /* 0x03 */ s8 flags;
    /* 0x04 */ f32 timer;
    /* 0x08 */ Vec3f vel;
    /* 0x14 */ Vec3f oldVel;
    /* 0x20 */ Vec3f prevPos;
    /* 0x2C */ char unk_2C[0x4];
} projectileData; // size = 0x30

typedef union {
    animal* animal;
    projectileData* projectileData;
} gobjData;

typedef struct GObj {
    /* 0x00 */ struct GObj* next;
    /* 0x04 */ struct GObj* prev;
    /* 0x08 */ OSThread* nextThread;
    /* 0x0C */ s8 link;
    /* 0x0D */ s8 dlLink;
    /* 0x0E */ char unk_0E[0x2];
    /* 0x10 */ u32 sortKey;
    /* 0x14 */ void* somefunc;
    /* 0x18 */ struct GObj * child;
    /* 0x1C */ OSThread* thread;
    /* 0x20 */ void* dlFunc;
    /* 0x24 */ struct GObj * dlNext;
    /* 0x28 */ u32 dlSortKey;
    /* 0x2C */ void (*renderFn)(struct GObj*);
    /* 0x30 */ void* linkFlags;
    /* 0x34 */ char unk_34[0x4];
    /* 0x38 */ s8 specialLinkRender; /* Created by retype action */
    /* 0x39 */ char unk_39[0x3];
    /* 0x3C */ signalLL* signals;
    /* 0x40 */ signalLL* lastSignal;
    /* 0x44 */ s32 LLCount;
    /* 0x48 */ struct geoNode* rootNode;
    /* 0x4C */ f32 currFrame;
    /* 0x50 */ s32 activeFlags;
    /* 0x54 */ void (*nodeFunc)(struct geoNode*, s32 undefined4);
    /* 0x58 */ gobjData data;
} GObj;

typedef GObj* (*animalInit)(s32 arg0, s32 id, struct roomGFX* roomA, struct roomGFX* roomB, struct objectSpawn* spawn);
typedef void (*gfxFunc)(struct GObj*);

typedef struct {
    s32 value;
    void (*func)(GObj*);
} randomTransition;

typedef struct {
    /* 0x00 */ u32 id;
    /* 0x04 */ animalInit* init;
    /* 0x08 */ void* update;
    /* 0x0C */ void* kill;
} animalDef;

#endif
