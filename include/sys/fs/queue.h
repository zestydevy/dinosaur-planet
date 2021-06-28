enum AssetLoadType {
    QUEUE_FILE = 0,
    QUEUE_ALLOCATED_FILE = 1,
    QUEUE_FILE_REGION = 2,
    QUEUE_TEXTURE = 3,
    QUEUE_OBJECT = 4,
    QUEUE_DLL = 5,
    QUEUE_MODEL = 6,
    QUEUE_ANIMATION = 7
};

struct AssetLoadThreadMsg {
    u8 loadCategory; // loading single or asset?
    u8 loadType;
    s32 unk4;  // offset in LUT?
    s32 *unk8;  // LUT?
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
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
extern OSMesgQueue assetLoadThreadSendQueue, D_800ACB68, assetLoadThreadRecvQueue;

void queue_alloc_load_file(void *dest, s32 fileId);
void queue_load_file_to_ptr(void *dest, s32 fileId);
void queue_load_file_region_to_ptr(void *dest, s32 arg1, s32 arg2, s32 arg3);
