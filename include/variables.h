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
//contains pointer-to-own-type fields, so `typedef struct _TActor {`
//must be used instead of `typedef struct {`
typedef struct TActor {
/*0000*/  s16 rotation[3]; //why short?
/*0006*/  s16 unk0x6;
/*0008*/  float scale;
/*000C*/  Vec3f position; //note: >300 unit drop causes fall damage.
	Vec3f positionMirror; //local vs global?
	Vec3f speed;
	void* ptr0x30;
	UNK_TYPE_32 unk0x34;
	struct TActor* linkedActor;
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
} TActor; //may be bigger, will know when constructor is understood.

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
    s32 maxItems;
    s32 itemCount;
    struct HeapBlock *ptr;
    s32 memAllocated;
    // s16 index;
    s32 memUsed;
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

typedef struct
{
	s32 offset;
	s32 bssSize;
} DLLTabEntry;


typedef struct
{
/*0000*/	s32 bank0;
/*0004*/	s32 bank1;
/*0008*/	s32 reserved;
/*000C*/	s32 bank2;
/*0010*/	DLLTabEntry * entries;
} DLLTab;

typedef struct
{
/*0000*/	u32 id;
/*0004*/	s32 refCount;
/*0008*/	u32 * exports;
/*000C*/	u32 * end;
} DLLInst;

typedef void (*DLLFunc)(u32 a);

typedef struct
{
	u32 header;
	u32 * data;
	u32 * rodata;
	s16 exportCount;
	DLLFunc ctor;
	DLLFunc dtor;
	u32 * reserved;
} DLLFile;



extern OSThread* __osRunningThread;
extern OSThread* __osRunQueue;
// this needs double checking. its address is within gMainThreadStack....
extern u8 gIdleThreadStack[IDLE_THREAD_SIZE];
extern OSThread gIdleThread;
extern OSThread gMainThread;
extern u64 gMainThreadStack[];        // some sort of data

extern u8 D_8008C940;
extern struct UnkStruct80014614
            **D_8008C970, **D_8008C974, **D_8008C978, **gDLL_ANIM,
            **D_8008C980, **D_8008C984, **gDLL_newclouds, **gDLL_newstars,
            **gDLL_minic, **D_8008C994, **gDLL_Race, **gDLL_AMSEQ,
            **gDLL_AMSEQ2, **gDLL_AMSFX, **gDLL_newlfx, **D_8008C9AC,
            **D_8008C9B0, **gDLL_expgfx, **gDLL_modgfx, **gDLL_projgfx,
            **D_8008C9C0, **D_8008C9C4, **D_8008C9C8, **gDLL_SCREENS,
            **gDLL_text, **gDLL_subtitles, **D_8008C9D8, **gDLL_waterfx,
            **D_8008C9E0, **gDLL_CURVES, **D_8008C9E8, **D_8008C9EC,
            **D_8008C9F0, **gDLL_gplay, **D_8008C9F8, **D_8008C9FC,
            **gDLL_savegame, **D_8008CA04, **D_8008CA08, **D_8008CA0C,
            **D_8008CA10, **D_8008CA14;
extern u8 alSynFlag;

extern OSSched *osscheduler_;
extern struct CharacterStats * charStats_pointer;
extern s32 gFile_BITTABLE;
extern s16 gSizeBittable;
extern s32 D_800AE678[], D_800AE688[], D_800AE698[], D_800AE6A8[]; //likely pointers
extern Gfx *D_800AE680;

extern s32 D_800AE674;

extern u32 * bss_end;
extern u8 gHeapBlkListSize;
extern struct HeapBlock gHeapBlkList[];
extern char * D_800991E0;
extern s32 memMonVal0;
extern s32 memMonVal1;
extern s32 memMonVal2;
extern s8 ossceduler_stack;
extern u8 D_800B09C1;
extern u8 D_800B09C3;
extern u8 D_800B09C2;
extern u8 D_800B09C4;
extern u8 D_8008C94C;
extern s32 D_800B179C;
extern s32 D_800A7D14;
extern s32 D_800A7D10;
extern s8 some_controller_init_val;
extern s32 PlayerPosBuffer_index;
extern struct Vec3_Int PlayerPosBuffer[60]; //seems to buffer player coords with "timestamp"


extern int func_printing_null_nil ( char * str, const char * format, ... );

extern u16 buttonInput0[4];
extern u16 buttonInput1[4];
extern u16 buttonMask[4]; // starts as 0xFFFF, buttons sometimes get AND'ed with it.
extern u8 controllerPortList[4]; // inits to 0,1,2,3. used in input getters.
extern s8 joyXMirror[4];
extern s8 joyYMirror[4];
extern u8 joyXHoldTimer[4]; // resets after hitting MenuInputDelay's value.
extern u8 joyYHoldTimer[4];
extern s8 joyXSign[4]; //1, 0, or -1 based on input.
extern s8 joyYSign[4];
extern u8 MenuInputDelay; //init'd to 5.
extern OSThread controller_thread;

extern void* framebufferNext;
extern void* framebufferCurrent;
extern OSViMode OSViMode_Custom;
extern u32 CurrentResolutionH[2]; //picked from resolutionArray[video_mode % 7]
extern u32 CurrentResolutionV[2]; //one for each FB, apparently
extern void* framebufferPointers[2]; //{FBaddr,FBaddr*H*V*2}
extern u32 framebufferChoice; 
extern u32 video_mode;
extern float aspectRatioFloat; //1.121212 for PAL, 1.333 for NTSC/MPAL.


extern u8 delayByte; //60/int(x) FPS x<=6
extern u8 delayByteMirror;
extern float delayFloat;
extern float delayFloatMirror;
extern float inverseDelay; // 1/delayByte
extern float inverseDelayMirror; // why the mirrors, if they aren't used?

extern struct TActor * object_pointer_array[]; //first is always player character.
extern u16 objectCount;
extern struct Vec3_Int Vec3_Int_array[];
/* not variables
extern struct ErrString errStringArray_cause[];
extern struct ErrString errStringArray_fpsr[];
*/
extern struct PointersInts pointersIntsArray[];
extern struct UnkStruct_800175D4 pointerIntArray0[]; // possible 420 length. counter for array fits this size.
extern u16 pointerIntArrayCounter; //yeah, this needs a better name.

extern DLLInst (*gLoadedDLLList)[];
extern s32 gLoadedDLLCount;

extern DLLTab * gFile_DLLS_TAB;

#endif
