#ifndef _SYS_GAME_OBJECTS_OBJECT_H
#define _SYS_GAME_OBJECTS_OBJECT_H

#include "PR/gbi.h"
#include "sys/gfx/model.h"
#include "sys/gfx/texture.h"
#include "sys/math.h"
#include "dll_def.h"
#include "object_def.h"
#include "hitbox.h"
#include "types.h"
#include "unktypes.h"

/** Game Object system
 */

struct Object;

//used for PlayerPosBuffer and something else
struct Vec3_Int{
	Vec3f f;
	u32 i; //seems to be a 32-bit bool. (for player pos buffer it's a frame count)
};

// base state of objects in group 16?
typedef struct {
/*0000*/ struct Object *unk0;
/*0004*/ u8 unk_0x4[0x9C - 0x4];
/*009C*/ u8 unk9C;
} ObjectAnimState;

// state of object in Object::unkC0
typedef struct {
/*0000*/    u8 unk_0x0[0x62 - 0x0];
/*0062*/    s8 unk_0x62;
} ObjectC0State;

// Base struct, objects "inherit" from this and add their own creation related info
// Curve objects use some of these parameters differently
typedef struct ObjCreateInfo {
/*00*/	s16 objId;
/*02*/	u8 quarterSize;
/*03*/	u8 setup; //bitfield of which Acts/setupIDs the object shouldn't appear in
/*04*/	u8 loadParamA;
/*05*/	u8 loadParamB;
/*06*/	u8 loadDistance;
/*07*/	u8 fadeDistance;
/*08*/	f32 x;
/*0c*/	f32 y;
/*10*/	f32 z;
/*14*/	s32 uID;
} ObjCreateInfo;

typedef struct {
/*0000*/    u8 unk_0x0[0x10 - 0x0];
/*0010*/	u8 *unk_0x10;
/*0014*/	u8 unk_0x14[0x44 - 0x14];
/*0044*/    s32 unk_0x44;
/*0048*/	u8 unk_0x48[0x5d - 0x48];
/*005D*/	s8 unk_0x5d;
/*005E*/    u8 unk_0x5e[0x71 - 0x5e];
/*0071*/    u8 unk_0x71;
/*0072*/	u8 unk_0x72;
} ObjectStruct50;

typedef struct {
/* 0x0 */ f32 unk0;
/* 0x4 */ Texture *unk4;
/* 0x8 */ Texture *unk8;
/* 0xC */ Gfx *gdl;
/* 0x10 */ s32 pad10;
/* 0x14 */ f32 maybeX;
/* 0x18 */ f32 maybeY;
/* 0x1C */ f32 maybeZ;
/* 0x20 */ Vec3f tr;
/* 0x2C */ f32 unk2c;
/* 0x30 */ u32 flags;
/* 0x34 */ u16 unk34;
/* 0x36 */ u16 unk36;
/* 0x38 */ u8 unk38;
/* 0x39 */ u8 unk39;
/* 0x3A */ u8 unk3A;
/* 0x3B */ u8 unk3B;
/* 0x3c */ u32 unk3c;
/* 0x40 */ u8 unk40;
/* 0x41 */ s8 unk41; //shadows toggle this each frame
} ObjectStruct64;

typedef struct {
	s32 unk0;
	UNK_PTR *unk4;
} ObjectStruct5C;

typedef struct {
	s32 unk0; // size of thing at unk4
	UNK_PTR *unk4;
} ObjectStruct60;

typedef struct {
	s32 sizeInBytes;
	UNK_PTR *ptr;
} WeaponDataPtr;

typedef struct {
/*0000*/    u8 unk0[0x80 - 0x00];
/*0080*/    MtxF unk80[2]; 				// probably length of 2
/*0100*/    UNK_PTR *unk100;
/*0104*/    u8 unk104[0x10c - 0x104];
/*010c*/    u8 unk10c; 					// index into field 0x80?
/*010d*/    UNK_TYPE_8 unk10d;
/*010e*/    UNK_TYPE_8 unk10e;
/*010f*/    UNK_TYPE_8 unk10f;
/*0110*/    u8 unk110[0x146 - 0x110];
/*0146*/    s16 unk146;
} ObjectStruct58;

typedef struct {
/*0000*/    u8 unk0;
/*0001*/    u8 unk1;
/*0002*/    u8 unk2;
/*0003*/    u8 unk3;
/*0004*/    u8 unk4;
} ObjectStruct78;

typedef struct {
/*0000*/	u32 id;
/*0004*/	struct Object *sender;
/*0008*/	void *arg;
} ObjMesgQueueMessage;

typedef struct {
/*0000*/    u32 count; // current number of messages in queue
/*0004*/    u32 capacity; // max number of messages
/*0008*/    ObjMesgQueueMessage queue[1]; // variable length
} ObjectMesgQueue;

// Base interface for object DLLs
DLL_INTERFACE_BEGIN(Object)
	/*0*/ void (*create)(struct Object *obj, ObjCreateInfo *createInfo, s32);
	/*1*/ void (*update)(struct Object *obj);
	/*2*/ void (*func3)(struct Object *obj); // update_child?
	/*3*/ void (*draw)(struct Object *obj, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
	/*4*/ void (*destroy)(struct Object *obj, s32); // (idk what param2 is, FALSE when from deferred free, TRUE when from non-deferred free)
	/*5*/ u32 (*func6)(struct Object *obj);
	/*6*/ u32 (*get_state_size)(struct Object *obj, u32);
	/*7*/ UnknownDLLFunc func7;
	/*8*/ UnknownDLLFunc func8;
	/*9*/ UnknownDLLFunc func9;
	/*10*/ UnknownDLLFunc func10;
	/*11*/ UnknownDLLFunc func11;
	/*12*/ UnknownDLLFunc func12;
	/*13*/ s32 (*func13)(struct Object *obj);
DLL_INTERFACE_END()

// arg3 and arg4 are unknown types
// always called by DLL 3 "ANIM" during cutscenes?
typedef s32 (*ObjectCallback)(struct Object *, struct Object *, void *, void *);

/**
 * Game object instance.
 * 
 * Size: 0xe4, other object-related data is placed in the following memory
 */
typedef struct Object {
/*0000*/    SRT srt;
/*0018*/    Vec3f positionMirror; //local vs global?
/*0024*/    Vec3f speed; // rename to velocity?
/*0030*/    struct Object *parent; // transform is relative to this object. doesn't form a strict hierarchy
/*0034*/    u8 unk_0x34;
/*0035*/    s8 matrixIdx;
/*0036*/    u8 unk_0x36; // transparency
/*0037*/    u8 unk_0x37; // also transparency? (set to unk_0x36)
/*0038*/    struct Object *next; // the object after this in a linked list
/*003C*/    f32 unk0x3c;
/*0040*/    f32 unk0x40;
/*0044*/    s16 group; // complete guess at a name, needs more investigation
/*0046*/    s16 id;
/*0048*/    s16 tabIdx; // index of ObjDef in OBJECTS.TAB
/*004A*/    u8 unk0x4a[0x4c - 0x4a];
/*004C*/    ObjCreateInfo *createInfo; // exact type depends on object
/*0050*/    ObjDef* def;
/*0054*/    ObjectHitInfo* objhitInfo;
/*0058*/    ObjectStruct58 *unk0x58;
/*005C*/    ObjectStruct5C *ptr0x5c;
/*0060*/    ObjectStruct60 *ptr0x60;
/*0064*/    ObjectStruct64* ptr0x64; //ShadowData?
/*0068*/    DLLInst_Object *dll;
/*006C*/    u16 (*ptr0x6c)[9];
/*0070*/    void* ptr0x70;
/*0074*/    u32 unk0x74;
/*0078*/    ObjectStruct78 *unk_0x78; // related to ObjDef.unk40
/*007C*/    ModelInstance **modelInsts;
/*0080*/    Vec3f positionMirror2; //gets copied twice.
/*008C*/    Vec3f positionMirror3; //not sure why.
/*0098*/    float animTimer; //guessed from SFA
/*009C*/    f32 unk0x9c;
/*00A0*/    s16 curAnimId;
/*00A2*/	s16 unk_0xa2;
/*00A4*/	f32 unk_0xa4;
/*00A8*/	f32 unk_0xa8;
/*00AC*/    s8 mapID;
/*00AD*/    s8 modelInstIdx;
/*00AE*/    s8 updatePriority;
/*00AF*/    u8 unk0xaf;
/*00B0*/    u16 unk0xb0;
/*00B2*/    s16 unk0xb2;
/*00B4*/    s16 unk0xb4;
/*00B6*/	u8 unk0xb6[2];
/*00B8*/	void* state; //type depends on object
/*00BC*/	ObjectCallback unk0xbc; // some kind of cutscene anim callback?
/*00C0*/	struct Object *unk0xc0; // related to group 16 objects?
/*00C4*/	UNK_TYPE_32 unk0xc4;
/*00C8*/    struct Object *linkedObject; // child? the linked object's parent is not necessarily set to the current object
/*00CC*/    ObjectMesgQueue *mesgQueue;
/*00D0*/    u8 unk_0xd0[0xd4 - 0xd0];
/*00D4*/    u8 unk_0xd4; // index into ObjDef.unk40
/*00D5*/    u8 unk_0xd5[0xd6 - 0xd5];
/*00D6*/    u8 unk_0xd6;
/*00D5*/    u8 unk_0xd7[0xd8 - 0xd7];
/*00D8*/    u8 unk_0xd8;
/*00D9*/    u8 unk_0xd9;
/*00DA*/    u8 unk_0xda;
/*00DB*/    u8 unk_0xdb[0xdc - 0xdb];
/*00DC*/    s32 unk0xdc;
/*00E0*/    s32 unk_0xe0; // lifetime?
} Object;

typedef struct ObjListItem {
	s16 count;
	s16 size;
	Object *obj;
} ObjListItem;

extern struct Object * object_pointer_array[]; //first is always player character.
extern u16 objectCount;
extern struct Vec3_Int Vec3_Int_array[20];

#endif
