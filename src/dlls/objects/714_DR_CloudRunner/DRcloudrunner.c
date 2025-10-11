#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/rand.h"
#include "sys/main.h"
#include "dll.h"
#include "functions.h"

#include "dlls/objects/745_DR_Cage.h"

typedef struct {
s32 unk0;
s8 unk4[0x25B - 4];
s8 unk25B;
s8 unk25C[0x272 - 0x25C];
s8 unk272;
s8 unk273[0x910 - 0x273];
s16 unk910;
s8 unk912[0x920 - 0x912];
u8 unk920;
} DRCloudRunnerState;

typedef struct {
ObjSetup base;
s16 unk18;
s16 unk1A;
s16 unk1C;
s16 unk1E;
} DRCloudRunner_Setup;

/*0x0*/ static u16 _data_0[] = {
    0x091d, 0x091e, 0x091f, 0x0000
};
/*0x8*/ static u32 _data_8[] = {
    0x09391000, 0x093a1000, 0x093b1000, 0x093c1000
};
/*0x18*/ static u32 _data_18 = 0x00000000;
/*0x1C*/ static u32 _data_1C = 0x00000000;
/*0x20*/ static u32 _data_20[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x2C*/ static f32 _data_2C = 40;
/*0x30*/ static u32 _data_30[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x3C*/ static f32 _data_3C = 40;
/*0x40*/ static u32 _data_40[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x4C*/ static f32 _data_4C = 20;
/*0x50*/ static u32 _data_50[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x5C*/ static f32 _data_5C = 20;
/*0x60*/ static u32 _data_60[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x6C*/ static u32 _data_6C = 0x00000000;
/*0x70*/ static f32 _data_70[] = {
    0, 0, 20, 0, 0, -20
};
/*0x88*/ static f32 _data_88[] = {
    15, 15
};
/*0x90*/ static f32 _data_90[] = {
    0, 0, 15
};
/*0x9C*/ static u32 _data_9C[] = {
    0x00000000, 0x00000000, 0x41f00000, 0x000d000d, 0x3b4b295f, 0x3ba3d70a
};
/*0xB4*/ static u16 _data_B4[] = {
    0x0117, 0x011c, 0x0118, 0x011b
};
/*0xBC*/ static u16 _data_BC[] = {
    0x0116, 0x011e, 0x0117, 0x0118, 0x011d, 0x0000, 0x000d, 0x0001, 0x238e, 0xf8e4, 0x038e, 0xce3a
};
/*0xD4*/ static u32 _data_D4[] = {
    0xf556b8e4, 0x00000000, 0x3fc3d70a, 0x3fc3d70a, 0x40228f5c, 0x4019999a, 0x40966666
};
/*0xF0*/ static u32 _data_F0[] = {
    0x071c0e38, 0x38e30000
};
/*0xF8*/ static f32 _data_F8[] = {
    0.02, 0.03, 0.06
};
/*0x104*/ static f32 _data_104[] = {
    0.02, 0.03, 0.1
};
/*0x110*/ static f32 _data_110[] = {
    1.5, 4.2, 4.6
};
/*0x11C*/ static f32 _data_11C[] = {
    0, 0.7, 1
};
/*0x128*/ static f32 _data_128[] = {
    0.02, 0.02, 0.04, 0.01
};
/*0x138*/ static f32 _data_138[] = {
    0, 0.35, 0
};
/*0x144*/ static f32 _data_144[] = {
    0, 1, 0
};
/*0x150*/ static f32 _data_150[] = {
    0, 0, 1
};
/*0x15C*/ static u32 _data_15C = 0x00010101;

/*0x0*/ static u8 _bss_0[0x20];
/*0x20*/ static u8 _bss_20[0x8];
/*0x28*/ static u8 _bss_28[0x8];
/*0x30*/ static u8 _bss_30[0x40];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_0.s")

// offset: 0x8C | ctor
void dll_714_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_ctor.s")

// offset: 0xCC | dtor
void dll_714_dtor(void *dll) { }

// offset: 0xD8 | func: 1 | export: 0
void dll_714_setup(Object *self, ObjSetup* setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_setup.s")

// offset: 0x318 | func: 2 | export: 1
void dll_714_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_control.s")

// offset: 0x85C | func: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_85C.s")

// offset: 0x8F8 | func: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_8F8.s")

// offset: 0x984 | func: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_984.s")

// offset: 0xB70 | func: 6 | export: 2
void dll_714_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_update.s")

// offset: 0xC8C | func: 7 | export: 3
void dll_714_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_print.s")

// offset: 0xE60 | func: 8 | export: 4
void dll_714_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_free.s")

// offset: 0xF50 | func: 9 | export: 5
u32 dll_714_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_EVENTS;
}

// offset: 0xF60 | func: 10 | export: 6
u32 dll_714_get_state_size(Object *self, u32 a1) {
    return sizeof(DRCloudRunnerState);
}

// offset: 0xF74 | func: 11 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_F74.s")

// offset: 0x10BC | func: 12 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_10BC.s")

// offset: 0x10E4 | func: 13 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_10E4.s")

// offset: 0x1108 | func: 14 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_1108.s")

// offset: 0x1318 | func: 15 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_1318.s")

// offset: 0x1340 | func: 16 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_1340.s")

// offset: 0x1404 | func: 17 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_1404.s")

// offset: 0x1414 | func: 18 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_1414.s")

// offset: 0x1488 | func: 19 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_1488.s")

// offset: 0x14B0 | func: 20 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_14B0.s")

// offset: 0x14D8 | func: 21 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_14D8.s")

// offset: 0x14E8 | func: 22 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_14E8.s")

// offset: 0x14F4 | func: 23 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_14F4.s")

// offset: 0x15E8 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_15E8.s")

// offset: 0x18AC | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_18AC.s")

// offset: 0x18E0 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_18E0.s")

// offset: 0x1968 | func: 27
s32 dll_714_func_1968(Object* self, DRCloudRunnerState* state, s32 arg2) {
    DRCloudRunnerState* state2;
    u32 new_var;
    DRCloudRunner_Setup* setup;
    Object* parent;

    setup = (DRCloudRunner_Setup*)self->setup;
    if (state->unk272) {
        func_800267A4(self);
        state->unk25B = 0;
        return 0;
    }

    state2 = self->state;

    //Randomly call out to player if in cage (Object parent hierarchy: DR_Cage -> DRPerch -> DR_CloudRunner)
    if (rand_next(0, 120) == 0) {
        parent = self->unk0xc4; //DRPerch
        if (parent) {
            parent = parent->unk0xc4; //DR_Cage
            if (parent && (parent->id == OBJ_DR_Cage)) {
                gDLL_3_Animation->vtbl->func17(rand_next(0, 1), self, -1);
            }
        }
    } else {
        parent = self->unk0xc4; //DRPerch
        if (parent) {
            parent = parent->unk0xc4; //DR_Cage
            if (parent && (parent->id == OBJ_DR_Cage) && 
                ((DLL_745_DR_Cage*)parent->dll)->vtbl->func0(parent)) {
                gDLL_3_Animation->vtbl->func17(2, self, -1);
            }
        }
    }

    new_var = 4;
    if (main_get_bits(setup->unk1E)) {
        self->unk0xc4 = NULL;
        state2->unk920 = (((new_var * (state2->unk910 > 0)) * 4) & 0x10) | (state2->unk920 & 0xFFEF);
        return 3;
    }

    return 0;
}

// offset: 0x1B20 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_1B20.s")

// offset: 0x1C6C | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_1C6C.s")

// offset: 0x1DE0 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_1DE0.s")

// offset: 0x2E0C | func: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_2E0C.s")

// offset: 0x303C | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_303C.s")

// offset: 0x31CC | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_31CC.s")

// offset: 0x3268 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_3268.s")

// offset: 0x3280 | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_3280.s")

// offset: 0x3560 | func: 36 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_3560.s")

// offset: 0x3574 | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_3574.s")

// offset: 0x37F4 | func: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_37F4.s")

// offset: 0x38A0 | func: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/714_DR_CloudRunner/dll_714_func_38A0.s")
