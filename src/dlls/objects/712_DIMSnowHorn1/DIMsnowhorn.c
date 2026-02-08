#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "types.h"
#include "prevent_bss_reordering.h"

typedef struct {
    s32 unk0;
    s8 unk4[0x272 - 0x4];
    s8 unk272;
    f32 unk274;
    f32 unk278;
    f32 unk27C;
    s8 unk280[0x28C - 0x280];
    f32 unk28C;
    f32 unk290;
    f32 unk294;
    s8 unk298[0x2B0 - 0x298];
    f32 unk2B0;
    s8 unk2B4[0x310 - 0x2B4];
    s32 unk310;
    s8 unk314[0x328 - 0x314];
    s16 unk328;
    s16 unk32A;
    s8 unk32C[0x33A - 0x32C];
    s8 unk33A;
    s8 unk33B[0x900 - 0x33B];
    s16 unk900;
    s16 unk902;
    s16 unk904;
    s16 unk906;
    s16 unk908;
    s16 unk90A;
} DIMSnowHorn_Data;

/*0x0*/ static u16 _data_0 = 0x01c8;
/*0x4*/ static f32 _data_4[] = {
    -12, 0, -20,
    12, 0, -20,
    12, 0, 20,
    -12, 0, 20
};
/*0x34*/ static u32 _data_34[] = {
    0, 0, 0, 0
};
/*0x44*/ static f32 _data_44[] = {
    0, 0, 35, 0, 0, -35
};
/*0x5C*/ static u32 _data_5C[] = {
    0x41c80000, 0x41c80000, 0x037b037b
};
/*0x68*/ static u32 _data_68 = 0x01010101;
/*0x6C*/ static f32 _data_6C = 0.0;
/*0x70*/ static u32 _data_70 = 0x00000000;
/*0x74*/ static u32 _data_74 = 0x0103000b;
/*0x78*/ static f32 _data_78[] = {
    0.0031, 0.005
};
/*0x80*/ static s16 _data_80[2] = {0x0000, 0x0003}; //modAnimIndices?
/*0x84*/ static f32 _data_84[] = {
    0.0, 0.05, 0.03, 0.85, 0, 0, 0
};

/*0x0*/ static u8 _bss_0[0x34];
/*0x34*/ static u8 _bss_34[0x4];
/*0x38*/ static u8 _bss_38[0x4];
/*0x3C*/ static u8 _bss_3C[0x4];
/*0x40*/ static u8 _bss_40[0x40];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_0.s")

// offset: 0xC8 | ctor
void dll_712_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_ctor.s")

// offset: 0x15C | dtor
void dll_712_dtor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_dtor.s")

// offset: 0x1CC | func: 1 | export: 0
void dll_712_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_setup.s")

// offset: 0x43C | func: 2 | export: 1
void dll_712_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_control.s")

// offset: 0x7C0 | func: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_7C0.s")

// offset: 0x87C | func: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_87C.s")

// offset: 0xAD4 | func: 5 | export: 2
void dll_712_update(Object *self) { }

// offset: 0xAE0 | func: 6 | export: 3
void dll_712_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_print.s")

// offset: 0xE24 | func: 7 | export: 4
void dll_712_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_free.s")

// offset: 0xE64 | func: 8 | export: 5
u32 dll_712_get_model_flags(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_get_model_flags.s")

// offset: 0xE74 | func: 9 | export: 6
u32 dll_712_get_data_size(Object *self, u32 a1) {
    return sizeof(DIMSnowHorn_Data);
}

// offset: 0xE88 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_E88.s")

// offset: 0xFA0 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_FA0.s")

// offset: 0x1150 | func: 12 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1150.s")

// offset: 0x124C | func: 13 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_124C.s")

// offset: 0x1274 | func: 14 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1274.s")

// offset: 0x1298 | func: 15 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1298.s")

// offset: 0x1310 | func: 16 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1310.s")

// offset: 0x1338 | func: 17 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1338.s")

// offset: 0x13FC | func: 18 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_13FC.s")

// offset: 0x140C | func: 19 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_140C.s")

// offset: 0x141C | func: 20 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_141C.s")

// offset: 0x1444 | func: 21 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1444.s")

// offset: 0x148C | func: 22 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_148C.s")

// offset: 0x149C | func: 23 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_149C.s")

// offset: 0x14A8 | func: 24 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_14A8.s")

// offset: 0x159C | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_159C.s")

// offset: 0x1860 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1860.s")

// offset: 0x1C78 | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1C78.s")

// offset: 0x1D34 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1D34.s")

// offset: 0x1D68 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1D68.s")

// offset: 0x1EB0 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1EB0.s")

// offset: 0x2024 | func: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_2024.s")

// offset: 0x2174 | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_2174.s")

// offset: 0x22FC | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_22FC.s")

// offset: 0x2470 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_2470.s")

// offset: 0x25E4 | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_25E4.s")

// offset: 0x27D4 | func: 36
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_27D4.s")

// offset: 0x29B8 | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_29B8.s")

// offset: 0x2BA0 | func: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_2BA0.s")

// offset: 0x2D90 | func: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_2D90.s")

// offset: 0x2EEC | func: 40
s32 dll_712_func_2EEC(Object* self, DIMSnowHorn_Data* objData, f32 arg2) {
    u8 one;
    DIMSnowHorn_Data *objData2 = self->data;
    f32 temp_fv0;
    f32 var_fa0;
    f32 var_fv1;
    f32 animProgress;
    s16 curModAnimId;
    f32 new_var2;
    s32 animCondition1;
    s32 returnValue;
    s32 animCondition2;
    s32 animIndex;
    f32 *temp_v0;

    objData->unk0 |= 0x200000;

    if (objData->unk272){
        self->srt.yaw += objData->unk32A * 0xB6;
        objData->unk328 = 0;
        objData->unk32A = 0;
    }
    if (objData->unk290 < 0.05f){
        objData->unk290 = 0.0f;
        objData->unk328 = 0;
        objData->unk32A = 0;
    }

    if (objData->unk328 < 90){
        self->srt.yaw += ((objData->unk32A * arg2) / 36.0f) * 182.0f;
    } else {
        return 9;
    }

    var_fa0 = objData->unk290;
    if (objData->unk290 < 0.0f){
        var_fa0 = 0.0f;
    }
    if (var_fa0 > 1.0f){
        var_fa0 = 1.0f;
    }

    //Handle SnowHorn running out of energy 
    //(@bug: causes a softlock since there's no fail state cutscene to reset you back to before the blizzard)
    if (objData2->unk900 == 0){
        var_fa0 = 0; //set speed to zero?
    }

    var_fv1 = var_fa0 * 0.85f;
    if (var_fv1 < 0){
        var_fv1 = 0;
    }

    objData->unk28C += ((var_fv1 - objData->unk28C) / objData->unk2B0) * arg2;
    if (self->srt.pitch > 0){
        var_fv1 -= fsin16_precise(self->srt.pitch) * 0.3f;
    } else {
        var_fv1 -= fsin16_precise(self->srt.pitch) * 0.15f;
    }
    if (var_fv1 < _data_84[2]){
        var_fv1 = _data_84[2];
    }

    objData->unk278 += ((var_fv1 - objData->unk278) / objData->unk2B0) * arg2;

    animCondition1 = 0;
    one = 1;

    animProgress = self->animProgress;

    for (animIndex = 0; self->curModAnimId != _data_80[animIndex] && animIndex < 2; animIndex++);

    if (animIndex >= 2){
        animIndex = 0;
    }

    if (self->curModAnimId == 0x208){
        animIndex = 1;
    }

    temp_v0 = &_data_84[animIndex * 2];
    if (objData->unk28C < temp_v0[0]){
        animCondition1 = 1;
        if (animIndex == 1){
            return 9;
        }
        animIndex -= one;
    } else if (temp_v0[1] <= objData->unk28C){
        animCondition1 = 1;
        if (animIndex == 0){
            animProgress = 0.0f;
        }
        animIndex++;
    }

    animCondition2 = 1;
    if (objData->unk33A && self->curModAnimId == 0x208){
        animCondition1 = 1;
        animCondition2 = 0;
    }

    if (animCondition1){
        if ((animIndex == 1) && animCondition2){
            func_80023D30(self, 0x208, animProgress, 0);
        } else {
            func_80023D30(self, _data_80[animIndex], animProgress, 0);
        }
    }
    func_8002493C(self, objData->unk278, &objData->unk298);

    if (objData->unk310 & 0x8000){
        return 0xD;
    } else {
        return 0;
    }
}

// offset: 0x32C0 | func: 41
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_32C0.s")

// offset: 0x3430 | func: 42
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_3430.s")

// offset: 0x3448 | func: 43 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_3448.s")
