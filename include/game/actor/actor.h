#ifndef _SYS_GAME_ACTOR_ACTOR_H
#define _SYS_GAME_ACTOR_ACTOR_H

#include <PR/gbi.h>
#include "sys/gfx/model.h"
#include "sys/dll.h"
#include "sys/math.h"
#include "objdata.h"
#include "hitbox.h"
#include "unktypes.h"

/** Actor/Game Object system
 */

//used for PlayerPosBuffer and something else
struct Vec3_Int{
	Vec3f f;
	u32 i; //seems to be a 32-bit bool. (for player pos buffer it's a frame count)
};

typedef struct {
/*0000*/	u8 unk_0x0[0x1a - 0x0];
/*001A*/	s16 unk0x1a;
} ActorUnk0x1a;

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
} ActorUnk0x50;

typedef struct {
/*0000*/    u8 unk_0x0[0xc - 0x0];
/*000C*/    Gfx *gdl;
/*0010*/    u8 unk_0x10[0x20 - 0x10];
/*0020*/    Vec3f tr;
/*002C*/    u32 unk_0x2c;
/*0030*/    u32 flags;
} ActorUnk0x64;

typedef struct {
/*0000*/    u8 unk_0x0[0x62 - 0x0];
/*0062*/    s8 unk_0x62;
} ActorUnk0xc0_0xb8;

typedef struct {
/*0000*/    u8 unk_0x0[0xb8 - 0x0];
/*00b8*/    ActorUnk0xc0_0xb8 *unk_0xb8;
} ActorUnk0xc0;

//prelimnary, lots of unknowns
//contains pointer-to-own-type fields, so `typedef struct _TActor {`
//must be used instead of `typedef struct {`
typedef struct TActor {
/*0000*/    SRT srt;
/*0018*/    Vec3f positionMirror; //local vs global?
/*0024*/    Vec3f speed;
/*0030*/    struct TActor *linkedActor; //only used by update_objects?
/*0034*/    u8 unk_0x34;
/*0035*/    s8 matrixIdx;
/*0036*/    u8 unk_0x36;
/*0037*/    u8 unk_0x37;
/*0038*/    u32 unk_0x38;
/*003C*/    u8 unk0x3c[0x44 - 0x3c];
/*0044*/    UNK_TYPE_16 objId; //guessed from SFA
/*0046*/    s16 unk0x46;
/*0048*/    void* ptr0x48;
/*004C*/    ActorUnk0x1a *unk0x4c;
/*0050*/    ObjData* data;
/*0054*/    ActorObjhitInfo* objhitInfo;
/*0058*/    UNK_TYPE_32 unk0x58;
/*005C*/    void* ptr0x5c;
/*0060*/    void* ptr0x60;
/*0064*/    ActorUnk0x64* ptr0x64;
/*0068*/    DLLInstance **dll; //same in SFA
/*006C*/    u16 *ptr0x6c;
/*0070*/    void* ptr0x70;
/*0074*/    u32 unk0x74;
/*0078*/    u32 unk_0x78;
/*007C*/    ModelInstance **modelInsts;
/*0080*/    Vec3f positionMirror2; //gets copied twice.
/*008C*/    Vec3f positionMirror3; //not sure why.
/*0098*/    float animTimer; //guessed from SFA
/*009C*/    f32 unk0x9c;
/*00A0*/    s16 curAnimId;
/*00A2*/	s16 unk_0xa2;
/*00A4*/    u8 unk0xa4[0xad - 0xa4];
/*00AD*/    s8 modelInstIdx;
/*00AE*/    s8 unk0xae;
/*00AF*/    u8 unk0xaf;
/*00B0*/    u16 unk0xb0;
/*00B2*/	u8 unk0xb2[6];
/*00B8*/	void* state; //type depends on object
/*00BC*/	UNK_TYPE_32 unk0xbc;
/*00C0*/	ActorUnk0xc0 *unk0xc0;
/*00C4*/	UNK_TYPE_32 unk0xc4;
/*00C8*/    struct TActor *linkedActor2;
/*00CC*/    void* ptr0xcc;
/*00D0*/    u8 unk_0xd0[0xdc - 0xd0];
/*00DC*/    s32 unk0xdc;
/*00E0*/    u8 unk_0xe0[4];
} TActor; // size is 0xe4; other actor-related data is placed in the following memory

typedef struct ObjListItem {
	s16 count;
	s16 size;
	TActor *obj;
} ObjListItem;


extern u8 delayByte; //60/int(x) FPS x<=6
extern u8 delayByteMirror;
extern float delayFloat;
extern float delayFloatMirror;
extern float inverseDelay; // 1/delayByte
extern float inverseDelayMirror; // why the mirrors, if they aren't used?

extern struct TActor * object_pointer_array[]; //first is always player character.
extern u16 objectCount;
extern struct Vec3_Int Vec3_Int_array[];

#endif //_SYS_GAME_ACTOR_ACTOR_H
