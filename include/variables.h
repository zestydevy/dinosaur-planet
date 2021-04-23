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
/*0006*/    s16 flags;
/*0008*/    f32 scale;
/*000C*/	Vec3f transl;
} SRT;

typedef struct Texture
{
/*0000*/	u8 width;
/*0001*/	u8 height;
/*0002*/	u8 format;
/*0003*/	u8 unk_0x3;
/*0004*/	u16 unk_0x4;
/*0006*/	s16 flags;
/*0008*/	Gfx *gdl;
/*000C*/	u16 unk_0xc;
/*000E*/	u16 unk_0xe;
/*0010*/	u16 unk_0x10;
/*0012*/	s16 gdlIdx;
/*0014*/	struct Texture *next;
/*0018*/	s16 unk_0x18;
/*001A*/	u8 unk_0x1a;
/*001B*/	u8 unk_0x1b;
/*001C*/	u8 cms;
/*001D*/	u8 masks;
/*001E*/	u8 cmt;
/*001F*/	u8 maskt;
} Texture; // Size: 0x20, followed by texture data

typedef struct
{
/*0000*/    u8 unk_0x0;
/*0001*/	u8 unk_0x1;
/*0002*/	s16 unk_0x2;
} Animation;

typedef struct
{
/*0000*/    u32 unk_0x0;
/*0004*/    f32 unk_0x4[2];
/*000C*/    u32 unk_0xc;
/*0010*/    u32 unk_0x10;
/*0014*/    f32 unk_0x14[2];
/*001C*/    u8 *anims[2]; // Each item contains 0x80 bytes of amap data, followed by an Animation structure
/*0024*/    u8 *anims2[2];
/*002C*/	u8 *unk_0x2c[2];
/*0034*/    u8 *unk_0x34[2];
/*003C*/    u8 unk_0x3c[0x44 - 0x3c];
/*0044*/    u16 animIndexes[2];
/*0048*/	u32 unk_0x48;
/*004C*/	u16 unk_0x4c[2][3];
/*0058*/    s16 unk_0x58;
/*005A*/	s16 unk_0x5a;
/*005C*/	s16 unk_0x5c;
/*005E*/	u16 unk_0x5e;
/*0060*/    s8 unk_0x60[2];
/*0062*/    u8 unk_0x62;
/*0063*/    s8 unk_0x63;
} AnimState;

typedef struct
{
/*0000*/    Texture *texture;
/*0004*/    u32 unk_0x4;
} ModelTexture;

typedef struct
{
/*0000*/    u8 unk_0x0;
/*0001*/    u8 unk_0x1[0x10 - 0x1];
} ModelUnk0x8;

typedef struct
{
/*0000*/	u32 unk_0x0;
/*0004*/	f32 x;
/*0008*/	f32 y;
/*000C*/	f32 z;
} ModelUnk0x20;

typedef struct
{
/*0000*/	Gfx gfx;
/*0008*/	Gfx gfx2;
/*0010*/	s16 idx;
/*0012*/	u8 unk_0x12[0x18 - 0x12];
} ModelDLInfo;

typedef struct
{
    // TODO
/*0000*/    ModelTexture *textures;
/*0004*/    void *unk_0x4;
/*0008*/    ModelUnk0x8 *unk_0x8;
/*000C*/    Gfx *displayList;
/*0010*/    Animation **anims;
/*0014*/    void *unk_0x14;
/*0018*/    void *unk_0x18;
/*001C*/    void *unk_0x1c;
/*0020*/    ModelUnk0x20 *unk_0x20;
/*0024*/    u8 *amap;
/*0028*/    void *unk_0x28;
/*002C*/    void *unk_0x2c;
/*0030*/    u32 unk_0x30;
/*0034*/    void *unk_0x34;
/*0038*/    ModelDLInfo *dlInfos;
/*003C*/    void *unk_0x3c;
/*0040*/    u32 unk_0x40;
/*0044*/    u32 unk_0x44;
/*0048*/    u32 unk_0x48;
/*004C*/    u32 unk_0x4c;
/*0050*/    void *unk_0x50;
/*0054*/    void *unk_0x54;
/*0058*/    u32 unk_0x58;
/*005C*/    u32 unk_0x5c;
/*0060*/    u16 unk_0x60;
/*0062*/    s16 unk_0x62;
/*0064*/    u16 unk_0x64;
/*0066*/    s16 animCount;
/*0068*/    s16 unk_0x68;
/*006A*/    s16 modelId;
/*006C*/    s16 displayListLength;
/*006E*/    u8 unk_0x6e;
/*006F*/    u8 unk_0x6f;
/*0070*/    u8 unk_0x70;
/*0071*/    u8 unk_0x71;
/*0072*/    u8 refCount;
/*0073*/    u8 textureCount;
/*0074*/    u8 unk_0x74;
/*0075*/	u8 dlInfoCount;
} Model;

typedef struct
{
    // TODO
/*0000*/    Model *model;
/*0004*/    void *unk_0x4[2];
/*000C*/    MtxF *matrices[2];
/*0014*/    void *unk_0x14;
/*0018*/    Gfx *displayList;
/*001C*/    u8 unk_0x1c[0x28 - 0x1c];
/*0028*/    AnimState *animState0;
/*002C*/    AnimState *animState1;
/*0030*/    u32 unk_0x30;
/*0034*/    u16 unk_0x34;
} ModelInstance;

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
/*0000*/    u8 unk_0x0[0x5a - 0x0];
/*005A*/    u8 unk_0x5a;
/*005B*/    u8 unk_0x5b[0x9f - 0x5b];
/*009F*/    s8 unk_0x9f;
} ActorObjhitInfo;

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
/*0046*/    s16 unk0x46;
/*0048*/    void* ptr0x48;
/*004C*/    UNK_TYPE_32 unk0x4c;
/*0050*/    ActorUnk0x50* ptr0x50;
/*0054*/    ActorObjhitInfo* objhitInfo;
/*0058*/    UNK_TYPE_32 unk0x58;
/*005C*/    void* ptr0x5c;
/*0060*/    void* ptr0x60;
/*0064*/    ActorUnk0x64* ptr0x64;
/*0068*/    void **dll;
/*006C*/    u16 *ptr0x6c;
/*0070*/    void* ptr0x70;
/*0074*/    u32 unk0x74;
/*0078*/    u32 unk_0x78;
/*007C*/    ModelInstance **modelInsts;
/*0080*/    u32 unk_0x80;
/*0084*/    Vec3f positionMirror3; //not sure why.
/*0090*/    u32 unk_0x90;
/*0094*/    u32 unk_0x94;
/*0098*/	f32 unk0x98;
/*009C*/    f32 unk0x9c;
/*00A0*/    s16 unk0xa0;
/*00A2*/	s16 unk_0xa2;
/*00A4*/    u8 unk0xa4[0xad - 0xa4];
/*00AD*/    s8 modelInstIdx;
/*00AE*/    u8 unk0xae;
/*00AF*/    u8 unk0xaf;
/*00B0*/    u16 unk0xb0;
	u8 unk0xb2[6];
	void* CharData; //0x8c1+ struct, has various player data.
	UNK_TYPE_32 unk0xbc;
	UNK_TYPE_32 unk0xc0;
	UNK_TYPE_32 unk0xc4;
/*00C8*/    struct TActor *linkedActor2;
/*00CC*/    void* ptr0xcc;
/*00D0*/    u8 unk_0xd0[0xe4 - 0xd0];
} TActor; // size is 0xe4; other actor-related data is placed in the following memory

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
/*00*/    AUDIO_TAB,
/*01*/    AUDIO_BIN,
/*02*/    SFX_TAB,
/*03*/    SFX_BIN,
/*04*/    AMBIENT_TAB,
/*05*/    AMBIENT_BIN,
/*06*/    MUSIC_TAB,
/*07*/    MUSIC_BIN,
/*08*/    MPEG_TAB,
/*09*/    MPEG_BIN,
/*0A*/    MUSICACTIONS_BIN,
/*0B*/    CAMACTIONS_BIN,
/*0C*/    LACTIONS_BIN,
/*0D*/    ANIMCURVES_BIN,
/*0E*/    ANIMCURVES_TAB,
/*0F*/    OBJSEQ2CURVE_TAB,
/*10*/    FONTS_BIN,
/*11*/    CACHEFON_BIN,
/*12*/    CACHEFON2_BIN,
/*13*/    GAMETEXT_BIN,
/*14*/    GAMETEXT_TAB,
/*15*/    GLOBALMAP_BIN,
/*16*/    TABLES_BIN,
/*17*/    TABLES_TAB,
/*18*/    SCREENS_BIN,
/*19*/    SCREENS_TAB,
/*1A*/    VOXMAP_BIN,
/*1B*/    VOXMAP_TAB,
/*1C*/    TEXPRE_TAB,
/*1D*/    TEXPRE_BIN,
/*1E*/    WARPTAB_BIN,
/*1F*/    MAPS_BIN,
/*20*/    MAPS_TAB,
/*21*/    MAPINFO_BIN,
/*22*/    MAPSETUP_IND,
/*23*/    MAPSETUP_TAB,
/*24*/    TEX1_BIN,
/*25*/    TEX1_TAB,
/*26*/    TEXTABLE_BIN,
/*27*/    TEX0_BIN,
/*28*/    TEX0_TAB,
/*29*/    BLOCKS_BIN,
/*2A*/    BLOCKS_TAB,
/*2B*/    TRKBLK_BIN,
/*2C*/    HITS_BIN,
/*2D*/    HITS_TAB,
/*2E*/    MODELS_TAB,
/*2F*/    MODELS_BIN,
/*30*/    MODELIND_BIN,
/*31*/    MODANIM_TAB,
/*32*/    MODANIM_BIN,
/*33*/    ANIM_TAB,
/*34*/    ANIM_BIN,
/*35*/    AMAP_TAB,
/*36*/    AMAP_BIN,
/*37*/    BITTABLE_BIN,
/*38*/    WEAPONDATA_BIN,
/*39*/    VOXOBJ_TAB,
/*3A*/    VOXOBJ_BIN,
/*3B*/    MODLINES_BIN,
/*3C*/    MODLINES_TAB,
/*3D*/    SAVEGAME_BIN,
/*3E*/    SAVEGAME_TAB,
/*3F*/    OBJSEQ_BIN,
/*40*/    OBJSEQ_TAB,
/*41*/    OBJECTS_TAB,
/*42*/    OBJECTS_BIN,
/*43*/    OBJINDEX_BIN,
/*44*/    OBJEVENT_BIN,
/*45*/    OBJHITS_BIN,
/*46*/    DLLS_BIN,
/*47*/    DLLS_TAB,
/*48*/    DLLSIMPORTTAB_BIN,
/*49*/    ENVFXACT_BIN,
    
/*4A*/    NUM_FILES
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

extern int gSomeCrashVideoFlag;
extern int D_800937F0;

extern int gPiManagerArray[];
extern u8 gPiManagerThreadStack[OS_PIM_STACKSIZE];
extern OSThread gPiManagerThread;

#endif
