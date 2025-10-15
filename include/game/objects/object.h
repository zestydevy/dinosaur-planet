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

// This could be bit fields? so 0, 1, 2, 4, 8 (0 << 0, 0 << 1, 0 << 2 etc)
enum ObjectGroup {
	GROUP_NONE = 0,
	GROUP_UNK1 = 1,
	GROUP_UNK16 = 16
};

//used for PlayerPosBuffer and something else
struct Vec3_Int{
	Vec3f f;
	u32 i; //seems to be a 32-bit bool. (for player pos buffer it's a frame count)
};

// base objdata of objects in group 16?
typedef struct {
/*0000*/ struct Object *unk0;
/*0004*/ u8 unk_0x4[0x9C - 0x4];
/*009C*/ u8 unk9C;
} ObjectAnim_Data;

// objdata of object in Object::unkC0
typedef struct {
/*0000*/    u8 unk_0x0[0x62 - 0x0];
/*0062*/    s8 unk_0x62;
} ObjectC0_Data;

typedef enum {
	OBJSETUP_FLAG_1 = 0x1,
	OBJSETUP_FLAG_2 = 0x2,
	OBJSETUP_FLAG_4 = 0x4
} ObjSetupFlags;

// Base struct, objects "inherit" from this and add their own setup info.
// Curve objects use some of these parameters differently
typedef struct ObjSetup {
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
} ObjSetup;

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
/* 0x10 */ Gfx *gdl2;
/* 0x14 */ Vec3f unk14;
/* 0x20 */ Vec3f tr;
/* 0x2C */ f32 unk2c;
/* 0x30 */ u32 flags;
/* 0x34 */ u16 unk34;
/* 0x36 */ s16 unk36;
/* 0x38 */ u8 unk38;
/* 0x39 */ u8 unk39;
/* 0x3A */ u8 unk3A;
/* 0x3B */ u8 unk3B;
/* 0x3c */ u8 unk3C;
/* 0x3d */ u8 unk3D;
/* 0x3e */ u8 unk3E;
/* 0x3f */ u8 unk3F;
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
} ObjectEvent;

typedef struct {
	s32 sizeInBytes;
	UNK_PTR *ptr;
} WeaponDataPtr;

typedef struct {
/*0000*/    u8 unk0[0x80 - 0x00];
/*0080*/    MtxF unk80[2]; 				// probably length of 2
/*0100*/    s32 unk100[3];
/*010c*/    u8 unk10c; 					// index into field 0x80?
/*010d*/    u8 unk10d;
/*010e*/    UNK_TYPE_8 unk10e;
/*010f*/    s8 unk10f;
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
//
// These initial 7 functions are inherited and implemented by all object DLLs.
// Any exports referenced with a higher index are specific to an object or subtype.
DLL_INTERFACE(DLL_IObject) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
	/*0*/ void (*setup)(struct Object *obj, ObjSetup *setup, s32);
	/*1*/ void (*control)(struct Object *obj);
	/*2*/ void (*update)(struct Object *obj);
	/*3*/ void (*print)(struct Object *obj, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
	/*4*/ void (*free)(struct Object *obj, s32); // (idk what param2 is, FALSE when from deferred free, TRUE when from non-deferred free)
	/*5*/ u32 (*get_model_flags)(struct Object *obj);
	/*6*/ u32 (*get_data_size)(struct Object *obj, u32);
};

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
/*0034*/    u8 unk_0x34; //self-mapID for mobile map objects? (e.g. Galleon)
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
/*004C*/    ObjSetup *setup; // exact type depends on object
/*0050*/    ObjDef* def;
/*0054*/    ObjectHitInfo* objhitInfo;
/*0058*/    ObjectStruct58 *unk0x58;
/*005C*/    ObjectStruct5C *ptr0x5c;
/*0060*/    ObjectEvent *curEvent;
/*0064*/    ObjectStruct64* ptr0x64; //ShadowData?
/*0068*/    DLL_IObject *dll;
/*006C*/    s16 (*ptr0x6c)[9];
/*0070*/    void* ptr0x70;
/*0074*/    Vec3f* unk0x74;
/*0078*/    ObjectStruct78 *unk_0x78; // related to ObjDef.unk40
/*007C*/    ModelInstance **modelInsts;
/*0080*/    Vec3f positionMirror2; //gets copied twice.
/*008C*/    Vec3f positionMirror3; //not sure why.
/*0098*/    f32 animProgress;
/*009C*/    f32 animProgressLayered;
/*00A0*/    s16 curModAnimId;
/*00A2*/	s16 curModAnimIdLayered;
/*00A4*/	f32 unk_0xa4;
/*00A8*/	f32 unk_0xa8;
/*00AC*/    s8 mapID;
/*00AD*/    s8 modelInstIdx;
/*00AE*/    s8 updatePriority;
/*00AF*/    u8 unk0xaf; //Target arrow-related flags? (Changes when A pressed on highlighted objects)
/*00B0*/    u16 unk0xb0; //Animation flags? (Animation updating can be switched off here)
/*00B2*/    s16 unk0xb2;
/*00B4*/    s16 unk0xb4;
/*00B6*/	u8 unk0xb6[2];
/*00B8*/	void* data; //type depends on object
/*00BC*/	ObjectCallback unk0xbc; // some kind of cutscene anim callback?
/*00C0*/	struct Object *unk0xc0; // related to group 16 objects?
/*00C4*/	struct Object* unk0xc4; // parent object
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

typedef struct {
/*00*/ s8 unk0[0x84 - 0];
/*84*/ u8 unk84;
} ObjectAnim_Data_2;

/** Used to load Tricky/Kyte's object 
  * 
*/
typedef struct {
/*00*/	ObjSetup base;
/*18*/	u8 unk18;
/*19*/	u8 unk19;
} SidekickSetup;

extern struct Object * object_pointer_array[]; //first is always player character.
extern u16 objectCount;
extern struct Vec3_Int Vec3_Int_array[20];
extern SidekickSetup D_80091688;

#endif
