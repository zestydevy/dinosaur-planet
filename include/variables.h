#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "ultra64.h"
#include <PR/sched.h>

#include "constants.h"
#include "common_structs.h"

typedef u8 UNK_TYPE_8;
typedef u16 UNK_TYPE_16;
typedef u32 UNK_TYPE_32;
typedef void UNK_PTR;

typedef float Vec2[2];
typedef float Vec3[3];
typedef float Vec4[4];
typedef short Vec3s[3];

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
/*000C*/	u16 levels;
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

//I'm copying most of these from SFA and default.dol.
//A lot of fields will need some tweaking to be correct for this version.
//default.dol should be much closer.

//A point on a model where something like a weapon can be attached.
typedef struct AttachPoint {
/*00*/ Vec3f pos; //offset from bone
/*0C*/ Vec3s rot; //offset from bone
/*12*/ s8 bone; //bone idx to use
/*13*/ s8 unk13; //always same as bone?
/*14*/ s8 unk14; //always same as bone?
/*15*/ u8 unk15; //always 0xCD?
/*16*/ u8 unk16; //always 0xCD?
/*17*/ u8 unk17; //always 0xCD?
} AttachPoint;

typedef struct AButtonInteraction {
/*00*/ s16 unk00;
/*02*/ s16 unk02;
/*04*/ s16 unk04;
/*06*/ UNK_TYPE_8 unk06;
/*07*/ UNK_TYPE_8 unk07;
/*08*/ UNK_TYPE_8 unk08;
/*09*/ UNK_TYPE_8 unk09;
/*0a*/ UNK_TYPE_8 unk0a;
/*0b*/ UNK_TYPE_8 unk0b;
/*0c*/ UNK_TYPE_8 unk0c;
/*0d*/ UNK_TYPE_8 unk0d;
/*0e*/ UNK_TYPE_8 unk0e;
/*0f*/ UNK_TYPE_8 unk0f;
/*10*/ u8 unk10;
/*11*/ UNK_TYPE_8 unk11;
/*12*/ UNK_TYPE_8 unk12;
/*13*/ UNK_TYPE_8 unk13;
/*14*/ UNK_TYPE_8 unk14;
/*15*/ UNK_TYPE_8 unk15;
/*16*/ UNK_TYPE_8 unk16;
/*17*/ UNK_TYPE_8 unk17;
} AButtonInteraction;

enum ObjDataFlags44 {
    OBJDATA_FLAG44_HaveModels               = 0x00000001,
    OBJDATA_FLAG44_DifferentLightColor      = 0x00000010,
    OBJDATA_FLAG44_ModelRelated             = 0x00000020,
    OBJDATA_FLAG44_HasChildren              = 0x00000040,
    OBJDATA_FLAG44_EnableCulling            = 0x00000400,
    OBJDATA_FLAG44_UseDifferentModelLoading = 0x00000800,
    OBJDATA_FLAG44_DifferentCulling         = 0x00080000,
    OBJDATA_FLAG44_KeepHitboxWhenInvisible  = 0x00200000,
    OBJDATA_FLAG44_HasEvent                 = 0x00400000,
    OBJDATA_FLAG44_DidLoadModels            = 0x00800000
};

enum ObjShadowType {
    OBJ_SHADOW_NONE = 0x0,
    OBJ_SHADOW_BOX = 0x1,
    OBJ_SHADOW_GEOM = 0x2,
    OBJ_SHADOW_UNK03 = 0x3,
    OBJ_SHADOW_BLUE_GLOWING_RECT = 0x4
};

enum HitboxFlags60 {
    HITBOX_DISABLED        = 0x0001,
    HITBOX_NEED_POS_UPDATE = 0x0040,
    HITBOX_LOCK_ROT_Y      = 0x0800,
    HITBOX_LOCK_ROT_Z      = 0x1000
};

enum HitboxFlags62 {
    HITBOX_SCALE_BY_SIZE      = 0x01,
    HITBOX_SIZE_FLAG_02       = 0x02,
    HITBOX_DONT_UPDATE        = 0x08,
    HITBOX_USE_MODEL_FIELD_14 = 0x20
};

enum ObjDataFlags5F {
    OBJDATA_FLAG5F_CrazyTranslucentEffect   = 0x01, //for HagabonMk2?
    OBJDATA_FLAG5F_ShadowUsesNoTexture      = 0x02,
    OBJDATA_FLAG5F_ShadowUsesDepthTest      = 0x04,
    OBJDATA_FLAG5F_DontFollowParentRotation = 0x08,
    OBJDATA_FLAG5F_NoShadow                 = 0x10, //force depth test if 0x01 also set
    OBJDATA_FLAG5F_Visible                  = 0x20,
    OBJDATA_FLAG5F_DifferentTextures        = 0x80  //Super dark
};

//Entry in OBJECTS.BIN, aka ObjectFileStruct (made-up name)
//debug messages imply ObjData is the correct name or close to it
typedef struct ObjData {
/*00*/ float unk00; //copied to shadow field 0
/*04*/ float scale;
/*08*/ u32 *pModelList; //-> list of model IDs
/*0c*/ UNK_PTR *textures;
/*10*/ UNK_PTR *unk10;
/*14*/ UNK_PTR *unk14;
/*18*/ u32 *offset_0x18; //[OPTIONAL] a file containing functions
/*1c*/ u16 *pSeq; //[OPTIONAL] -> seq IDs
/*20*/ UNK_TYPE_8 *pEvent; //[OPTIONAL] offset into the file. changed to pointer on load
/*24*/ UNK_TYPE_8 *pHits; //[OPTIONAL]
/*28*/ UNK_TYPE_8 *pWeaponDa; //[OPTIONAL] 
/*2c*/ AttachPoint *pAttachPoints;
/*30*/ s16 *pModLines; //ignored in file (zeroed on load)
/*34*/ UNK_PTR *pIntersectPoints; //ignored in file (zeroed on load) (wObjList?)
/*38*/ UNK_PTR *nextIntersectPoint; 
/*3c*/ UNK_PTR *nextIntersectLine; 
/*40*/ AButtonInteraction *aButtonInteraction;
/*44*/ u32 flags; //ObjDataFlags44
/*48*/ s16 shadowType; //ObjShadowType
/*4a*/ s16 shadowTexture;
/*4c*/ UNK_TYPE_8 unk4C;
/*4d*/ UNK_TYPE_8 unk4D;
/*4e*/ u16 hitbox_flags60; //HitboxFlags60
/*50*/ s16 dllNo; //if not -1, load this DLL; func 0 is a model callback
/*52*/ UNK_TYPE_8 unk52;
/*53*/ UNK_TYPE_8 unk53;
/*54*/ UNK_TYPE_8 unk54;
/*55*/ u8 nModels;
/*56*/ u8 numPlayerObjs; //if > 0, objAddObjectType(obj, 8)
/*57*/ u8 unk57; //never read?
/*58*/ u8 nAttachPoints;
/*59*/ u8 nTextures; //-> 0x10 bytes
/*5a*/ s16 objId;
/*5c*/ s8 modLinesSize; //ignored in file
/*5d*/ s8 modLinesIdx;
/*5e*/ u8 numSeqs;
/*5f*/ u8 flags_0x5f; //ObjDataFlags5F
/*60*/ u8 hitbox_fieldB0;
/*61*/ u8 hasHitbox; //or # hitboxes, but should only be 1
/*62*/ u8 hitboxSizeXY;
/*63*/ u8 hitbox_field6A;
/*64*/ u8 hitbox_field6B;
/*65*/ u8 hitbox_flags62;
/*66*/ u8 unk66;
/*67*/ u8 hitbox_fieldB5;
/*68*/ s16 hitboxSizeX1;
/*6a*/ s16 hitboxSizeY1; // > 0x169 = no shadow; also hitbox related
/*6c*/ s16 hitboxSizeZ1; 
/*6e*/ s16 hitboxSizeZ2; 
/*70*/ u8 hitbox_fieldB4; //related to hitbox (height?)
/*71*/ u8 flags_0x71; //related to hitbox
/*72*/ u8 numAButtonInteractions;
/*73*/ u8 stateVar73; //1=translucent; 3=invincible - not flags
/*74*/ u8 unk74;
/*75*/ u8 unk75;
/*76*/ s16 modLineCount;
/*78*/ s16 modLineNo;
/*7a*/ u8 unk7A; 
/*7b*/ u8 unk7B; 
/*7c*/ s16 helpTexts[4]; //one per model (GameTextId)
/*84*/ s16 unk84;
/*86*/ s16 unk86; 
/*88*/ float lagVar88; //causes lag at ~65536.0; GPU hang at much more; related to shadow; maybe causing excessive map loads?
/*8c*/ u8 nLights; 
/*8d*/ u8 lightIdx; 
/*8e*/ u8 colorIdx; //related to textures; 1=dark, 2=default, 3+=corrupt, 77=crash, 0=normal
/*8f*/ u8 unk8F; //related to hitbox
/*90*/ u8 hitbox_flagsB6; // < 0xE = invincible (HitboxFlags62)
/*91*/ char name[11];
} ObjData;

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
/*0044*/    UNK_TYPE_16 objId; //guessed from SFA
/*0046*/    s16 unk0x46;
/*0048*/    void* ptr0x48;
/*004C*/    UNK_TYPE_32 unk0x4c;
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
/*00AE*/    u8 unk0xae;
/*00AF*/    u8 unk0xaf;
/*00B0*/    u16 unk0xb0;
	u8 unk0xb2[6];
	void* state; //type depends on object
	UNK_TYPE_32 unk0xbc;
	UNK_TYPE_32 unk0xc0;
	UNK_TYPE_32 unk0xc4;
/*00C8*/    struct TActor *linkedActor2;
/*00CC*/    void* ptr0xcc;
/*00D0*/    u8 unk_0xd0[0xe4 - 0xd0];
} TActor; // size is 0xe4; other actor-related data is placed in the following memory

typedef struct ObjListItem {
	s16 count;
	s16 size;
	TActor *obj;
} ObjListItem;

//found a 3-array of these, not sure what they're for.
struct Vec3_Int{
	Vec3f f;
	u32 i; //seems to be a 32-bit bool.
};//used for camera?


//pointer at 0x34c of "state" struct
struct CharacterStats{
	u8 unk0x0;
	s8 HPCurr;
	s8 HPMax; //only shows up to 104, code caps it at 80.
	u8 unk0x3; //aligning?
	s16 ManaCurr; //only mods when byte at "state"0x8bb is set.
	s16 ManaMax; //capped at 100.
	s16 Scarabs; //capped at 999.
	u8 unk0xa;
	u8 unk0xb;
};//seemingly consistent addrs: Fox/Sabre at 0x805c3964, Krystal 0x805c3970.

typedef struct {
/*0000*/    u32 * tail;
/*0004*/    s32 maxSize;
/*0008*/    s16 m0008;
/*000C*/    s16 m000C;
/*000E*/    s16 index;
/*0010*/    s32 m0010;
} Heap;

struct HeapBlock {
/*0000*/    s32 maxItems;
/*0004*/    s32 itemCount;
/*0008*/    Heap * ptr;
/*000C*/    s32 memAllocated;
/*0010*/    s32 memUsed;
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

enum DLL_ID {
    DLL_UI        = 0x01,
    DLL_CAMERA    = 0x02,
    DLL_ANIM      = 0x03,
    DLL_RACE      = 0x04,
    DLL_AMSEQ     = 0x05,
    DLL_AMSFX     = 0x06,
    DLL_SKY       = 0x07,
    DLL_NEWCLOUDS = 0x09,
    DLL_NEWSTARS  = 0x0A,
    DLL_NEWLFX    = 0x0B,
    DLL_MINIC     = 0x0C,
    DLL_EXPGFX    = 0x0D,
    DLL_MODGFX    = 0x0E,
    DLL_PROJGFX   = 0x0F,
    DLL_SCREENS   = 0x14,
    DLL_TEXT      = 0x15,
    DLL_SUBTITLES = 0x16,
    DLL_WATERFX   = 0x18,
    DLL_CURVES    = 0x1A,
    DLL_GPLAY     = 0x1D,
    DLL_SAVEGAME  = 0x1F,
    DLL_MINIMAP   = 0x3B,
    DLL_LINK      = 0x4A
};

extern OSThread* __osRunningThread;
extern OSThread* __osRunQueue;
// this needs double checking. its address is within gMainThreadStack....
extern u8 gIdleThreadStack[IDLE_THREAD_SIZE];
extern OSThread gIdleThread;
extern OSThread gMainThread;
extern u64 gMainThreadStack[];        // some sort of data

extern u8 D_8008C940;
extern void ***gDLL_57;
extern struct DLLInstance
    **D_8008C970, 
    **gDLL_1C, 
    **gDLL_Camera, 
    **gDLL_ANIM,
    **gDLL_Sky, 
    **gDLL_08, 
    **gDLL_newclouds, 
    **gDLL_newstars,
    **gDLL_minic, 
    **gDLL_UI, 
    **gDLL_Race, 
    **gDLL_AMSEQ,
    **gDLL_AMSEQ2, 
    **gDLL_AMSFX, 
    **gDLL_newlfx, 
    **gDLL_39,
    **gDLL_3A, 
    **gDLL_expgfx, 
    **gDLL_modgfx, 
    **gDLL_projgfx,
    **gDLL_10, 
    **gDLL_11, 
    **gDLL_12, 
    **gDLL_SCREENS,
    **gDLL_text, 
    **gDLL_subtitles, 
    **gDLL_17, 
    **gDLL_waterfx,
    **gDLL_19, 
    **gDLL_CURVES, 
    **gDLL_Link, 
    **gDLL_4B,
    **gDLL_1B, 
    **gDLL_gplay, 
    **gDLL_38, 
    **gDLL_1E,
    **gDLL_savegame, 
    **gDLL_4C, 
    **gDLL_20, 
    **gDLL_21,
    **gDLL_3B, 
    **gDLL_36;
extern u8 alSynFlag;

extern OSSched *osscheduler_;
extern struct CharacterStats * charStats_pointer;
extern s32 gFile_BITTABLE;
extern s16 gSizeBittable;
extern s32 D_800AE678[], D_800AE688[], D_800AE698[], D_800AE6A8[]; //likely pointers
extern Gfx *gCurGfx;

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
/**
 * The index of the last inserted controller.
 * 
 * For example, if one controller is inserted then this will be 0.
 * If no controllers are inserted, this will be -1.
 */
extern s8 gLastInsertedControllerIndex;
extern s32 PlayerPosBuffer_index;
extern struct Vec3_Int PlayerPosBuffer[60]; //seems to buffer player coords with "timestamp"


extern int func_printing_null_nil ( char * str, const char * format, ... );

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
