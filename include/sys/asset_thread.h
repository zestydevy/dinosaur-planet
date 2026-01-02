#ifndef _SYS_ASSET_THREAD_H
#define _SYS_ASSET_THREAD_H

#include "PR/os.h"
#include "PR/ultratypes.h"
#include "sys/generic_stack.h"
#include "sys/generic_queue.h"
#include "game/objects/object.h"

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
            /* 0C */ s32 length; //for ASSET_TYPE_FILE_REGION
            /* 10 */ s32 offset; //for ASSET_TYPE_FILE_REGION
        } file;
        struct {
            /* 04 */ s32 id;
            /* 08 */ Object **dest;
            /* 0C */ s32 unused0C; //debug?
            /* 10 */ s32 unused10;
            /* 14 */ Object *parent;
            /* 18 */ ObjSetup *setup;
            /* 1C */ u32 initFlags;
            /* 20 */ s32 arg4;
            /* 24 */ s32 mapID;
            /* 28 */ s32 arg6;
        } object;
        struct {
            /* 04 */ s32 id;
            /* 08 */ Texture **dest;
        } texture;
        struct {
            /* 04 */ s32 idOrIdx;
            /* 08 */ void **dest;
            /* 0C */ s32 exportCount;
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

typedef struct {
    u8 unk0;
    u32 *unk4;
    ObjSetup *unk8;
    u32 unkC;
    u32 unk10;
    Object* unk14;
    u32 unk18;
} UnkStructAssetThreadSingle;

typedef struct {
    u8 unk0;
    u32 *unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
} AssetThreadStackElement;

void create_asset_thread(void);
void asset_thread_main(void *arg);
void func_80012584(s32 param1, u8 param2, u32 *param3, ObjSetup *param4, s32 param5, s32 param6, Object* param7, s32 param8);
void queue_alloc_load_file(void **dest, s32 fileId);
void queue_load_file_to_ptr(void **dest, s32 fileId);
void queue_load_file_region_to_ptr(void **dest, s32 fileId, s32 offset, s32 length);
void queue_load_map_object(Object **dest, ObjSetup *setup, u32 initFlags, s32 mapID, s32 arg4, Object *parent, s32 arg6);
void queue_load_texture(Texture **dest, s32 id);
void queue_load_dll(void **dest, s32 idOrIdx, s32 exportCount);
void queue_load_model(void **dest, s32 id, s32 arg2);
void queue_load_anim(void **dest, s16 animID, s16 modAnimID, s32 amap, s32 model);
void queue_block_emplace(s32 param1, u32 *param2, s32 param3, s32 param4, s32 param5);
void func_800129E4();

#endif //_SYS_ASSET_THREAD_H
