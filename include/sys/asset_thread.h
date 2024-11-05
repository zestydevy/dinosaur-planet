#ifndef _SYS_ASSET_THREAD_H
#define _SYS_ASSET_THREAD_H

#include "PR/os.h"
#include "PR/ultratypes.h"
#include "sys/generic_stack.h"

#define ASSET_THREAD_ID 99
#define ASSET_THREAD_PRIORITY 11

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

extern struct AssetLoadThreadMsg assetLoadMsg;

extern OSMesgQueue assetLoadThreadSendQueue, //send load requests to asset thread
    D_800ACB68,
    assetLoadThreadRecvQueue; //receive acknowledgement from asset thread

typedef struct {
    u8 unk0;
    u32 *unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
} UnkStructFunc80012A4C;

struct UnkStruct8000ADF0 {
    s16 unk0;
};
extern struct UnkStruct8000ADF0 *D_800ACBC8;
struct UnkStruct8000ADF0 *func_8000ADF0(s32 *, s32 *, s32, s32);

extern GenericStack *D_800AE1D0;

extern u8 gDisableObjectStreamingFlag;
extern u8 D_800AE29D, D_800AE29E;
extern s32 *D_800ACBB8, *D_800ACBD0;
extern u64 *assetThreadStackEnd; // end of stack
extern OSThread *assetThread;

void create_asset_thread(void);
void asset_thread_main(void);
void queue_alloc_load_file(void **dest, s32 fileId);
void queue_load_file_to_ptr(void **dest, s32 fileId);
void queue_load_file_region_to_ptr(void **dest, s32 fileId, s32 length, s32 offset);
void queue_load_map_object(void **dest, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void queue_load_texture(void **dest, s32 id);
void queue_load_dll(void **dest, s32 id, s32 param);
void queue_load_model(void **dest, s32 id, s32 arg2);
void queue_load_anim(void **dest, s16 id, s16 arg2, s32 arg3, s32 arg4);

#endif //_SYS_ASSET_THREAD_H
