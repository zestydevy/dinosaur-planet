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

// Scale, rotation, translation. Rotations are applied in the order: Yaw -> Pitch -> Roll. (TODO: verify)
typedef struct
{
/*0000*/    s16 yaw; // 16-bit angle in the range [-32768..32768) => [-pi..pi)
/*0002*/    s16 pitch;
/*0004*/    s16 roll;
/*0006*/    // padding inserted by compiler
/*0008*/    f32 scale;
/*000C*/    f32 tx;
/*0010*/    f32 ty;
/*0014*/    f32 tz;
} SRT;

//prelimnary, lots of unknowns
//contains pointer-to-own-type fields, so `typedef struct _TActor {`
//must be used instead of `typedef struct {`
typedef struct TActor {
/*0000*/    SRT srt;
/*0018*/    Vec3f positionMirror; //local vs global?
/*0024*/    Vec3f speed;
/*0030*/    struct TActor *linkedActor;
/*0034*/    u8 unk_0x34;
/*0035*/    s8 matrixIdx;
/*0036*/    u8 unk_0x36;
/*0037*/    u8 unk_0x37;
/*0038*/    u32 unk_0x38;
/*003C*/    u8 unk0x3c[0x44 - 0x3c];
/*0044*/    UNK_TYPE_16 unk0x44;
/*0046*/    UNK_TYPE_16 unk0x46;
/*0048*/    void* ptr0x48;
/*004C*/    UNK_TYPE_32 unk0x4c;
/*0050*/    void* ptr0x50;
/*0054*/    void* ptr0x54;
/*0058*/    UNK_TYPE_32 unk0x58;
/*005C*/    void* ptr0x5c;
/*0060*/    void* ptr0x60;
/*0064*/    void* ptr0x64;
/*0068*/    void* ptr0x68;
/*006C*/    void* ptr0x6c;
/*0070*/    void* ptr0x70;
/*0074*/    UNK_TYPE_32 unk0x74;
/*0078*/    Vec3f positionMirror2; //gets copied twice.
/*0084*/    Vec3f positionMirror3; //not sure why.
/*0090*/    u32 unk_0x90;
/*0094*/    u32 unk_0x94;
/*0098*/    float unk0x98;
	u8 unk0x9c;
	u8 unk0x9d[3]; //aligning?
	s16 unk0xa0;
	u8 unk0xa2[11];
	u8 unk0xad;
	u8 unk0xae;
	u8 unk0xaf;
/*00B0*/    u16 unk0xb0;
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
/*0010*/	DLLTabEntry entries[1];
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
	u32 code;
	u32 data;
	s32 rodata;
	u16 exportCount;
	DLLFunc ctor;
	DLLFunc dtor;
	u32 exports[1]; // Exports table begins here
} DLLFile;

typedef enum
{
    AUDIO_TAB,
    AUDIO_BIN,
    SFX_TAB,
    SFX_BIN,
    AMBIENT_TAB,
    AMBIENT_BIN,
    MUSIC_TAB,
    MUSIC_BIN,
    MPEG_TAB,
    MPEG_BIN,
    MUSICACTIONS_BIN,
    CAMACTIONS_BIN,
    LACTIONS_BIN,
    ANIMCURVES_BIN,
    ANIMCURVES_TAB,
    OBJSEQ2CURVE_TAB,
    FONTS_BIN,
    CACHEFON_BIN,
    CACHEFON2_BIN,
    GAMETEXT_BIN,
    GAMETEXT_TAB,
    GLOBALMAP_BIN,
    TABLES_BIN,
    TABLES_TAB,
    SCREENS_BIN,
    SCREENS_TAB,
    VOXMAP_BIN,
    VOXMAP_TAB,
    TEXPRE_TAB,
    TEXPRE_BIN,
    WARPTAB_BIN,
    MAPS_BIN,
    MAPS_TAB,
    MAPINFO_BIN,
    MAPSETUP_IND,
    MAPSETUP_TAB,
    TEX1_BIN,
    TEX1_TAB,
    TEXTABLE_BIN,
    TEX0_BIN,
    TEX0_TAB,
    BLOCKS_BIN,
    BLOCKS_TAB,
    TRKBLK_BIN,
    HITS_BIN,
    HITS_TAB,
    MODELS_TAB,
    MODELS_BIN,
    MODELIND_BIN,
    MODANIM_TAB,
    MODANIM_BIN,
    ANIM_TAB,
    ANIM_BIN,
    AMAP_TAB,
    AMAP_BIN,
    BITTABLE_BIN,
    WEAPONDATA_BIN,
    VOXOBJ_TAB,
    VOXOBJ_BIN,
    MODLINES_BIN,
    MODLINES_TAB,
    SAVEGAME_BIN,
    SAVEGAME_TAB,
    OBJSEQ_BIN,
    OBJSEQ_TAB,
    OBJECTS_TAB,
    OBJECTS_BIN,
    OBJINDEX_BIN,
    OBJEVENT_BIN,
    OBJHITS_BIN,
    DLLS_BIN,
    DLLS_TAB,
    DLLSIMPORTTAB_BIN,
    ENVFXACT_BIN,
    
    NUM_FILES
} EFile;

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

extern DLLInst* gLoadedDLLList;
extern s32 gLoadedDLLCount;

extern DLLTab * gFile_DLLS_TAB;

#endif
