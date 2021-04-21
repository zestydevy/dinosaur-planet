#ifndef QUEUE_H
#define QUEUE_H

struct UnkMesg800AE270 {
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

extern struct UnkMesg800AE270 D_800AE270;

extern OSMesgQueue D_800ACB48, D_800ACB68, D_800ACB98;

#define QUEUE_FILE 0
#define QUEUE_ALLOCATED_FILE 1
#define QUEUE_FILE_REGION 2
#define QUEUE_TEXTURE 3
#define QUEUE_OBJECT 4
#define QUEUE_DLL 5
#define QUEUE_MODEL 6
#define QUEUE_ANIMATION 7

void queue_load_texture(s32 *arg0, s32 arg1);

#endif
