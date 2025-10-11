#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/object.h"
#include "types.h"
#include "common.h"

typedef struct {
    u8 _unk0[0xC8 - 0x0];
    f32 unkC8;
    f32 unkCC;
    u8 _unkD0[0xDC - 0xD0];
} DLL274_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000000, 0x00000001, 0x00000002, 0x00000003
};
/*0x10*/ static u32 _data_10[] = {
    0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x30*/ static u32 _data_30[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x40*/ static u32 _data_40[] = {
    0x00000000, 0x00000000, 0x3dcccccd, 0x00000000
};

// offset: 0x0 | ctor
void dll_274_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_274_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_274_setup(Object *self, ObjSetup* setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_setup.s")

// offset: 0x638 | func: 1 | export: 1
void dll_274_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_control.s")

// offset: 0x87C | func: 2 | export: 2
void dll_274_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_update.s")

// offset: 0xEE4 | func: 3 | export: 3
void dll_274_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_print.s")

// offset: 0x1058 | func: 4 | export: 4
void dll_274_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_free.s")

// offset: 0x116C | func: 5 | export: 5
u32 dll_274_get_model_flags(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_get_model_flags.s")

// offset: 0x117C | func: 6 | export: 6
u32 dll_274_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL274_Data);
}

// offset: 0x1190 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_1190.s")

// offset: 0x176C | func: 8 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_176C.s")

// offset: 0x1814 | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_1814.s")

// offset: 0x1834 | func: 10 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_1834.s")

// offset: 0x184C | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_184C.s")

// offset: 0x18C8 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_18C8.s")

// offset: 0x1A24 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_1A24.s")

// offset: 0x1D18 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_1D18.s")

// offset: 0x2020 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_2020.s")

// offset: 0x20A0 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_20A0.s")

// offset: 0x225C | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_225C.s")

// offset: 0x27C8 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_27C8.s")

// offset: 0x2A74 | func: 19
s32 dll_274_func_2A74(Object* arg0, DLL274_Data* arg1) {
    if ((arg1->unkCC == 0.0f) && (arg1->unkC8 > 0.0f)) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_3D8, MAX_VOLUME, NULL, NULL, 0, NULL);
        main_set_bits(BIT_DIM_Pushed_Ice_Block_Into_Lake, 1);
    }
    return 0;
}

/*0x0*/ static const char str_0[] = "PUSHPULL OBJECT: hitpoint overflow\n";
/*0x24*/ static const char str_24[] = "handle is %d\n";
