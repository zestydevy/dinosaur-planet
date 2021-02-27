#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "ultra64.h"
#include <PR/sched.h>

#include "constants.h"
#include "common_structs.h"

typedef u8 UNK_TYPE_8;
typedef u16 UNK_TYPE_16;
typedef u32 UNK_TYPE_32;

typedef float Vec2[2];
typedef float Vec3[3];
typedef float Vec4[4];

//prelimnary, lots of unknowns
struct objectStruct{
	s16 rotation[3]; //why short?
	s16 unk0x6;
	float scale;
	Vec3f position; //note: >300 unit drop causes fall damage.
	Vec3f positionMirror; //local vs global?
	Vec3f speed;
	void* ptr0x30;
	UNK_TYPE_32 unk0x34;
	void* ptr0x38;
	u8 unk0x3c[3];
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
	Vec3f positionMirror2; //gets copied twice.
	Vec3f positionMirror3; //not sure why.
	float unk0x98;
	u8 unk0x9c;
	u8 unk0x9d[3]; //aligning?
	s16 unk0xa0;
	u8 unk0xa2[11];
	u8 unk0xad;
	u8 unk0xae;
	u8 unk0xaf;
	s16 unk0xb0;
	u8 unk0xb2[6];
	void* CharData; //0x8c1+ struct, has various player data.
	UNK_TYPE_32 unk0xbc;
	UNK_TYPE_32 unk0xc0;
	UNK_TYPE_32 unk0xc4;
	void* ptr0xc8;
	void* ptr0xcc;
}; //may be bigger, will know when constructor is understood.

//found a 3-array of these, not sure what they're for.
struct Vec3_Int{
	Vec3f f;
	u32 i; //seems to be a 32-bit bool.
};//used for camera?


//pointer at 0x34c of "CharData" struct
struct CharacterStats{
	u8 unk0x0;
	s8 HPCurr;
	s8 HPMax; //only shows up to 104, code caps it at 80.
	u8 unk0x3; //aligning?
	s16 ManaCurr; //only mods when byte at "CharData"0x8bb is set.
	s16 ManaMax; //capped at 100.
	s16 Scarabs; //capped at 999.
	u8 unk0xa;
	u8 unk0xb;
};//seemingly consistent addrs: Fox/Sabre at 0x805c3964, Krystal 0x805c3970.

struct HeapBlock {
    s32 items_max;
    s32 items_count;
    struct HeapBlock *ptr;
    s32 mem_allocated;
    // s16 index;
    s32 mem_used;
};

//unknown What this is for.
struct PointersInts{
	void *prts[2];
	u32 valA;
	u32 valB;
};


//use for crash screens
struct ErrString{
	u32 code1;
	u32 code2;
	char* text;
};

extern OSThread* __osRunningThread;
extern OSThread* __osRunQueue;
// this needs double checking. its address is within gMainThreadStack....
extern u8 gIdleThreadStack[IDLE_THREAD_SIZE];
extern OSThread gIdleThread;
extern OSThread gMainThread;
extern u64 gMainThreadStack[];        // some sort of data

extern u8 D_8008C940;
extern struct UnkStruct80014614
            **D_8008C970, **D_8008C974, **D_8008C978, **D_8008C97C,
            **D_8008C980, **D_8008C984, **D_8008C988, **D_8008C98C,
            **D_8008C990, **D_8008C994, **D_8008C998, **D_8008C99C,
            **D_8008C9A0, **D_8008C9A4, **D_8008C9A8, **D_8008C9AC,
            **D_8008C9B0, **D_8008C9B4, **D_8008C9B8, **D_8008C9BC,
            **D_8008C9C0, **D_8008C9C4, **D_8008C9C8, **D_8008C9CC,
            **D_8008C9D0, **D_8008C9D4, **D_8008C9D8, **D_8008C9DC,
            **D_8008C9E0, **D_8008C9E4, **D_8008C9E8, **D_8008C9EC,
            **D_8008C9F0, **D_8008C9F4, **D_8008C9F8, **D_8008C9FC,
            **D_8008CA00, **D_8008CA04, **D_8008CA08, **D_8008CA0C,
            **D_8008CA10, **D_8008CA14;
extern u8 alSyn_flag;

extern OSSched *osscheduler_;
extern s32 D_800AE2A0;
extern s32 D_800AE2A4;
extern s16 D_800AE2A8;
extern s32 D_800AE678[], D_800AE688[], D_800AE698[], D_800AE6A8[];
extern Gfx *D_800AE680;

extern u32 * bss_end;
extern s16 D_800B1798;
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
extern u32 SixtyByFour_Array[60][4];

extern u16 button_input_1[];
extern u16 button_input_2[];
extern u8 controller_port_list[];
extern s8 joy_x_mirror[];
extern s8 joy_y_mirror[];
extern u8 joy_x_hold_timer[];
extern u8 joy_y_hold_timer[];
extern s8 joy_x_sign[];
extern s8 joy_y_sign[];
extern OSThread controller_thread;

extern struct objectStruct * object_pointer_array[]; //first is always player character.
extern u16 object_count;
extern struct Vec3_Int Vec3_Int_array[];
extern struct ErrString err_string_array_cause[];
extern struct ErrString err_string_array_fpsr[];
extern struct PointersInts pointers_ints_array[];
extern struct UnkStruct_800175D4 pointer_int_array_0[]; // length?

#endif
