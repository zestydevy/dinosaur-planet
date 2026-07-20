#ifndef _SYS_ASSET_H
#define _SYS_ASSET_H

#include "PR/os.h"
#include "PR/ultratypes.h"
#include "sys/generic_stack.h"
#include "sys/generic_queue.h"
#include "game/objects/object.h"

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
            /* 20 */ AmapPlusAnimation* arg3;
            /* 24 */ Model* arg4;
        } animation;
    } p; //give it a name just to shut the compiler up
    //actual size is 0x30 but that includes padding
};

typedef struct {
    u8 unk0;
    u32 *unk4;
    union {
        s32 blockId;
        void *unk8ptr;
        ObjSetup *unk8ObjSetup;
    };
    u32 unkC;
    u32 unk10;
    union {
        Object *unk14;
        s32 unk14int;
    };
    u32 unk18;
} UnkStructAssetThreadSingle;

typedef struct {
    u8 unk0;
    union {
        u32 *unk4;
        void *unk4Ptr;
        Object *unk4Object;
    };
    union {
        u32 unk8;
        void *unk8ptr;
    };
    u32 unkC;
    u32 unk10;
} AssetThreadStackElement;

void assetInit(void);
void assetEnqueueLoad(s32 param1, u8 param2, u32 *param3, ObjSetup *param4, s32 param5, s32 param6, Object* param7, s32 param8);
void assetRomLoad(void **dest, s32 fileId);
void assetRomLoadToDest(void **dest, s32 fileId);
void assetRomLoadSection(void **dest, s32 fileId, s32 offset, s32 length);
void assetLoadObject(Object **dest, ObjSetup *setup, u32 initFlags, s32 mapID, s32 arg4, Object *parent, s32 arg6);
void assetLoadTexture(Texture **dest, s32 id);
void assetLoadDLL(void **dest, s32 idOrIdx, s32 exportCount);
void assetLoadModel(void **dest, s32 id, s32 arg2);
void assetLoadAnim(void **dest, s16 animID, s16 modAnimID, AmapPlusAnimation* amap, Model* model);
void assetQueueCompletedLoad(s32 param1, u32 *param2, u8 *param3, s32 param4, s32 param5);
void assetQueueTick();
void assetQueueClearMesgType(s32 param1, s32 param2);
u8 assetIsObjQueueEnabled(void);
void assetToggleObjQueue(u32 enabled);

#endif
