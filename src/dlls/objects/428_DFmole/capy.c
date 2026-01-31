#include "common.h"
#include "sys/objanim.h"

typedef struct {
    u8 _unk0[0x410];
} DLL428_Data;

// size:0x14
typedef struct {
    u8 unk0; 
    u8 _unk1 [3];
    u16 unk4;
} capy_data;

/*0x0*/ static u32 _data_0 = 0x00000002;
/*0x4*/ static u32 _data_4[] = {
    0x05370538, 0x05390000, 0x00000000
};

/*0x0*/ static u8 _bss_0[0x28];
/*0x28*/ static u8 _bss_28[0x8];
/*0x30*/ static u8 _bss_30[0x10];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_0.s")

// offset: 0xD8 | ctor
void dll_428_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_ctor.s")

// offset: 0x118 | dtor
void dll_428_dtor(void *dll) { }

// offset: 0x124 | func: 1 | export: 0
void dll_428_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_setup.s")

// offset: 0x1DC | func: 2 | export: 1
void dll_428_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_control.s")

// offset: 0x2D4 | func: 3 | export: 2
void dll_428_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_update.s")

// offset: 0x324 | func: 4 | export: 3
void dll_428_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_print.s")

// offset: 0x384 | func: 5 | export: 4
void dll_428_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_free.s")

// offset: 0x428 | func: 6 | export: 5
u32 dll_428_get_model_flags(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_get_model_flags.s")

// offset: 0x438 | func: 7 | export: 6
u32 dll_428_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL428_Data);
}

// offset: 0x44C | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_44C.s")

// offset: 0x468 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_468.s")

// offset: 0x644 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_644.s")

// offset: 0x704 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_704.s")

// offset: 0x7A0 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_7A0.s")

// offset: 0x97C | func: 13
s32 dll_428_func_97C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DLL33_Data *objdata = self->data;
    capy_data* capydata;

    capydata = objdata->unk3F4;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 1, 0.0f, 0U);
        fsa->unk33A = 0;
    }
    if (capydata->unk4 != 0) {
        capydata->unk4 -= 1;
    }
    fsa->animTickDelta = 0.0f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0xA0C | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_A0C.s")

// offset: 0xA9C | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_A9C.s")

// offset: 0xBC4 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_BC4.s")

// offset: 0xD1C | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_D1C.s")

// offset: 0xF8C | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_F8C.s")

// offset: 0x10BC | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_10BC.s")

// offset: 0x1264 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_1264.s")

// offset: 0x1408 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_1408.s")

// offset: 0x1420 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_1420.s")

// offset: 0x1438 | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_1438.s")

// offset: 0x1698 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/428_DFmole/dll_428_func_1698.s")

// offset: 0x1728 | func: 25
s32 dll_428_func_1728(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    DLL33_Data* sp44;
    Object* temp_v1;
    capy_data* sp3C;
    u8 sp3B;
    Unk80009024* temp_s1;
    f32 temp_fv0;
 
    

    sp44 = arg0->data;
    sp3C = sp44->unk3F4;
    if (arg1->target != 0) {
        sp3B = get_player() == arg1->target;
        arg1->animState = 8;
        arg1->unk288 = 0.0f;
        arg1->unk284 = 0.0f;
        temp_s1 = &sp44 -> unk34C;
        bcopy(&arg0-> srt.transl , temp_s1, 0xC);
        bcopy(&arg1-> target -> srt.transl , &sp44 -> unk34C.unkC, 0xC);
        func_80009024(temp_s1, &sp44 -> unk374);
        if (sp3B != 0) {
            if ((arg1->targetDist < 50.0f) || (sp3C->unk0 == 0)) {
                arg1->animState = 0;
            }
        
        } else { 
            temp_fv0 = arg1->targetDist;
            if ((temp_fv0 < 20.0f) || (((s32) sp3C->unk4 >= 0xB) && (temp_fv0 < 30.0f))) {
                temp_v1 = arg1->target;
                arg1->animState = 4;
                temp_v1->unkAF = (u8) (temp_v1->unkAF | 8);
                return 4;
            }
       }
        if (temp_s1->unk25 == 0) {
            gDLL_18_objfsa->vtbl->func6(arg0, arg1, temp_s1->unk18.f[0], temp_s1->unk18.f[2], 0.0f, 0.0f, 60.0f);
        } else if (sp3B != 0) {
            gDLL_18_objfsa->vtbl->func6(arg0, arg1, temp_s1->unk18.f[0], temp_s1->unk18.f[2], 20.0f, 30.0f, 60.0f);
        } else {
            gDLL_18_objfsa->vtbl->func6(arg0, arg1, temp_s1->unk18.f[0], temp_s1->unk18.f[2], 10.0f, 10.0f, 60.0f);
        }
    } else {
        arg1->animState = 3;
        arg1->enteredAnimState = 1;
        arg1->unk33A = 0;
        return 6;
    }
    return 0;
}

// offset: 0x19DC | func: 26
s32 dll_428_func_19DC(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    DLL33_Data* baddie = arg0->data;
    capy_data* objdata = baddie ->unk3F4;
    
    if ((arg1->target != NULL) && (arg1->animState == 1) && (objdata->unk4 == 0)) {
        gDLL_18_objfsa->vtbl->func11(arg0, arg1, 3.0f, 1);
        arg1->animState = 2;
        arg1->enteredAnimState = 1;
        arg1->unk33A = 0;
    }
    return 0;
}
