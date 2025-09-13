#include "common.h"

static const char str_80099c90[] = " WARNING EXPR: This Object has no Head ";
static const char str_80099cb8[] = " WARNING EXPR: Obj Has No Joint %i ";
static const char str_80099cdc[] = " WARNING: Expr Contrl Flag does not exist \n";

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800328F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80032A08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80032B44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80032C0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80032CF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80032EBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80033044.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80033224.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800332A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80033350.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800333C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800334A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800339E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80033AA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80033B68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80033C54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80033FD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80034250.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800343B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80034518.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80034678.s")

/** object_find_seq_bone_data_by_bone_id? */
u16* func_80034804(Object* obj, s32 sequenceBoneID) {
    ObjDef* romdef;
    u8* seqBones;
    s32 index;
    s32 listPosition;
    u32 jointID;
    u16* sequenceBoneData;

    romdef = obj->def;
    sequenceBoneData = NULL;
    if (romdef) {
        listPosition = 0;
        for (index = 0; index < romdef->numSequenceBones; index++){
            jointID = romdef->pSequenceBones[(listPosition + 1) + obj->modelInstIdx];
            if (jointID != 0xFF && sequenceBoneID == romdef->pSequenceBones[listPosition])
                sequenceBoneData = obj->ptr0x6c[index];

            listPosition += 1 + romdef->numModels;
        }
    }
    return sequenceBoneData;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800348A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800349B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_800349C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80034B54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80034B94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80034BC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_334F0/func_80034D94.s")
