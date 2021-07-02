#include "PR/os.h"
#include "PR/ultratypes.h"

enum AssetLoadType {
    ASSET_TYPE_FILE = 0,
    ASSET_TYPE_ALLOCATED_FILE = 1,
    ASSET_TYPE_FILE_REGION = 2,
    ASSET_TYPE_TEXTURE = 3,
    ASSET_TYPE_OBJECT = 4,
    ASSET_TYPE_DLL = 5,
    ASSET_TYPE_MODEL = 6,
    ASSET_TYPE_ANIMATION = 7
};

struct AssetLoadThreadMsg {
    u8 loadCategory; // loading single or asset?
    u8 loadType; //AssetLoadType
    union {
        struct {
            /* 04 */ s32 id; //XXX type (can it be an enum?)
            /* 08 */ void **dest; //should be actually void**, unlike the others
            /* 0C */ s32 offset; //for ASSET_TYPE_FILE_REGION
            /* 10 */ s32 length; //for ASSET_TYPE_FILE_REGION
        } file;
        struct {
            /* 04 */ s32 id;
            /* 08 */ void **dest; //XXX type
            /* 0C */ s32 unused0C; //debug?
            /* 10 */ s32 unused10;
            /* 14 */ s32 arg5;
            /* 18 */ s32 arg1;
            /* 1C */ s32 arg2;
            /* 20 */ s32 arg4;
            /* 24 */ s32 arg3;
            /* 28 */ s32 arg6;
        } object;
        struct {
            /* 04 */ s32 id;
            /* 08 */ void **dest;
        } texture;
        struct {
            //XXX verify these
            /* 04 */ s32 id;
            /* 08 */ void **dest;
            /* 0C */ s32 param;
        } dll;
        struct {
            /* 04 */ s32 id;
            /* 08 */ void **dest;
            /* 0C */ s32 unkC;
        } model;
        struct {
            /* 04 */ s32 id;
            /* 08 */ void **dest;
            /* 0C */ s32 arg2;
            /* 10 */ s32 unused10; //maybe originally wasn't a union
            /* 14 */ s32 unused14; //and these were params for other types?
            /* 18 */ s32 unused18;
            /* 1C */ s32 unused1C;
            /* 20 */ s32 arg3;
            /* 24 */ s32 arg4;
        } animation;
    } p; //give it a name just to shut the compiler up
    //actual size is 0x30 but that includes padding
};

struct UnkStructAssetThreadSingle {
    u8 unk0;
    u32 *unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
};

struct UnkStruct8000B010 {
    s16 unk0;
};

struct UnkStruct8000ADF0 {
    s16 unk0;
};

extern struct AssetLoadThreadMsg assetLoadMsg;
extern OSMesgQueue assetLoadThreadSendQueue, //send load requests to asset thread
    D_800ACB68, 
    assetLoadThreadRecvQueue; //receive acknowledgement from asset thread
extern u8 D_800AE29D, D_800AE29E;
extern struct UnkStruct8000ADF0 *D_800ACBC8;
extern struct UnkStruct8000B010 *D_800AE1D0;
extern u8 gDisableObjectStreamingFlag;
extern u8 D_800AE29D, D_800AE29E;
extern s32 *D_800ACBB8, *D_800ACBD0;
extern s32 *D_800AE1C0, *D_800AE1D8;
extern u64 *assetThreadStackEnd; // end of stack
extern OSThread assetThread;

struct UnkStruct8000ADF0 *func_8000ADF0(s32 *, s32 *, s32, s32);
struct UnkStruct8000B010 *func_8000B010(s32 *, s32 *, s32, s32);
void create_asset_thread(void);
void asset_thread_main(void*);
