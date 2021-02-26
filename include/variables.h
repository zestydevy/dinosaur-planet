#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "ultra64.h"

#include "constants.h"
#include "common_structs.h"

typedef u8 UNK_TYPE_8;
typedef u16 UNK_TYPE_16;
typedef u32 UNK_TYPE_32;

typedef float[2] Vec2;
typedef float[3] Vec3;
typedef float[4] Vec4;

//prelimnary, lots of unknowns
struct objectStruct{
	s16[3] rotation; //why short?
	s16 unk0x6;
	float scale;
	Vec3 position; //note: >300 unit drop causes fall damage.
	Vec3 positionMirror; //local vs global?
	Vec3 speed;
	void* ptr0x30;
	UNK_TYPE_32 unk0x34;
	void* ptr0x38;
	byte[8] unk0x3c;
	UNK_TYPE_16 unk0x44;
	UNK_TYPE_16 unk0x46;
	void* ptr0x48;
	UNK_TYPE_32 unk0x4c;
	void* ptr0x50;
	void* ptr0x54;
	UNK_TYPE_32 unk0x58;
	void* ptr0x5c;
	void* ptr0x60;
	void* ptr0x64;
	void* ptr0x68;
	void* ptr0x6c;
	void* ptr0x70;
	UNK_TYPE_32 unk0x74;
	Vec3 positionMirror2; //gets copied twice.
	Vec3 positionMirror3; //not sure why.
	byte[21] unk0x98;
	byte unk0xad;
	byte unk0xae;
	byte unk0xaf;
	s16 unk0xb0;
	byte[6] unk0xb2;
	void* CharData; //0x8c1+ struct, has various player data.
	UNK_TYPE_32 unk0xbc;
	UNK_TYPE_32 unk0xc0;
	UNK_TYPE_32 unk0xc4;
	void* ptr0xc8;
	void* ptr0xcc;
} //may be bigger, will known when constructor is understood.

//found a 3-array of these, not sure what they're for.
struct Vec3_Int{
	Vec3 f;
	u32 i; //seems to be a 32-bit bool.
}//used for camera?


//pointer at 0x34c of "CharData" struct
struct CharacterStats{
	u8 unk0x0;
	s8 HPCurr;
	s8 HPMax; //only shows up to 104, code caps it at 80.
	s16 ManaCurr; //only mods when byte at "CharData"0x8bb is set.
	s16 ManaMax; //capped at 100.
	s16 Scarabs; //capped at 999.
	u8 unk0xa;
	u8 unk0xb;
}//seemingly consistent addrs: Fox/Sabre at 0x805c3964, Krystal 0x805c3970.

struct HeapBlock {
    s32 items_max;
    s32 items_count;
    void *ptr;
    s32 mem_allocated;
    s32 mem_used;
};


extern OSThread* __osRunningThread;
extern OSThread* __osRunQueue;
// this needs double checking. its address is within gMainThreadStack....
extern u8 gIdleThreadStack[IDLE_THREAD_SIZE];
extern OSThread gIdleThread;
extern OSThread gMainThread;
extern u64 gMainThreadStack[];        // some sort of data

extern u8 D_8008C940;
extern s32 D_8008C9EC;
extern UnkStruct80014614** D_8008C9F4;
extern s32 D_8008CA04;

extern s32 D_800AE2A0;
extern s32 D_800AE2A4;
extern s16 D_800AE2A8;
extern s32 D_800AE678[];
extern s32 D_800AE680;

extern u32 * bss_end;
extern u16 D_800B1798;
extern u8 heap_block_array_size;
extern struct HeapBlock heap_block_array[];
extern s8 ossceduler_stack;
extern u8 D_800B09C1;
extern u8 D_800B09C3;
extern u8 D_800B09C2;
extern u8 D_800B09C4;
extern u8 D_8008C94C;
extern s32 D_800B179C;
extern s8 some_controller_init_val;
extern u32 D_800AE670;
extern UNK_TYPE_8 D_800AE2B0;
extern objectStruct*[] object_pointer_array; //first is always player character.
extern u16 object_count;
extern Vec3_Int[3] Vec3_Int_array;


#endif
