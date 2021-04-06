#include "common.h"

#define ALIGN16(a) (((u32) (a) & ~0xF) + 0x10)

typedef struct
{
    // TODO
/*0000*/    u8 unk_0x0[0x1c];
/*001C*/    u32 unk_0x1c;
/*0020*/    u32 unk_0x20;
/*0024*/    u8 unk_0x24[0x50 - 0x24];
/*0050*/    u32 unk_0x50;
/*0054*/    u32 unk_0x54;
/*0058*/    u32 unk_0x58;
/*005C*/    u32 unk_0x5c;
/*0060*/    u16 unk_0x60;
/*0062*/    s16 unk_0x62;
/*0064*/    u16 unk_0x64;
/*0066*/    s16 unk_0x66;
/*0068*/    s16 unk_0x68;
/*006a*/    u16 unk_0x6a;
/*006C*/    u8 unk_0x6c;
/*006D*/    u8 unk_0x6d;
/*006E*/    u8 unk_0x6e;
/*006F*/    u8 unk_0x6f;
/*0070*/    u8 unk_0x70;
/*0071*/    u8 unk_0x71;
} Model;

typedef struct
{
    Model *model;
    // TODO
} ModelInstance;

typedef struct
{
    // TODO
/*0000*/    u32 unk_0x0;
/*0004*/    u32 unk_0x4;
/*0008*/    u32 unk_0x8;
/*000C*/    u32 unk_0xc;
/*0010*/    u32 unk_0x10;
/*0014*/    u32 unk_0x14;
/*0018*/    u32 unk_0x18;
} ModelStats;


#pragma GLOBAL_ASM("asm/nonmatchings/model/init_models.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80017D2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/model_load_create_instance.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/createModelInstance.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800186CC.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/model/model_get_stats.s")
#else
// Write information about model in stats argument, then return total size of ModelInstance
u32 _model_get_stats(Model *model, u32 flags, ModelStats *stats, u32 param_4)
{
    u32 totalSize;

    stats->unk_0x18 = (model->unk_0x66 != 0) ? (model->unk_0x6f * 0x80) : 0x80;

    stats->unk_0x0 = model->unk_0x62 * 0x20;
    stats->unk_0x4 = model->unk_0x6e * 0x20;
    stats->unk_0xc = 0;

    if (model->unk_0x71 & 0x40)
    {
        stats->unk_0x14 = model->unk_0x68;
        while (stats->unk_0x14 & 0x7) {
            stats->unk_0x14++;
        }

        stats->unk_0xc = stats->unk_0x14 * 4;
    }

    stats->unk_0x10 = 0x68;

    if (flags & 0x80)
    {
        stats->unk_0x10 = 0xd0;
        stats->unk_0xc *= 2;
    }

    if ((flags & 0x1) || model->unk_0x1c != 0 || param_4 != 0) {
        totalSize = stats->unk_0xc + stats->unk_0x0 + stats->unk_0x10 + stats->unk_0x18 + 0x30 + stats->unk_0x4;
        stats->unk_0x10 += 0x30;
    } else {
        totalSize = stats->unk_0x10 + stats->unk_0xc + stats->unk_0x18 + stats->unk_0x4;
    }

    totalSize += 0x40;

    if (model->unk_0x20 != 0 && model->unk_0x6f != 0)
    {
        if (model->unk_0x50 != 0) {
            totalSize += model->unk_0x6f * 0x1c + model->unk_0x6f * 2 + 0x1c;
        }

        totalSize += 0x2f;
    }
    else
    {
        totalSize += 0x6f;
    }

    return ALIGN16(totalSize);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80018950.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800189A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80018A00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80018B14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/model_load_anim_remap_table.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/modanim_load.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/model_setup_anim_playback.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019118.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/anim_load.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019388.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001943C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800195F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019730.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_800199A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_80019FC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A1D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A3FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A640.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001A8EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001AC44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001AE74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001AF04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001AFCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B010.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B084.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B100.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B49C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B4E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B4F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001B6F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001BC00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001BCF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001BF70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001C8D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/model/func_8001CAA4.s")
