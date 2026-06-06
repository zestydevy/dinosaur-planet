#include "common.h"

#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/332_FXEmit.h"
#include "dlls/objects/278_flameblast.h"
#include "sys/objexpr.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "sys/route.h"

#include "prevent_bss_reordering.h"

// size: 0x610
typedef struct {
    void *unk0; // holds a pointer to a (loaded) DLL
    u8 *unk4;
    Object *unk8;
    u8 padC[0x18 - 0xC];
    u8 unk18;
    u8 unk19;
    u8 unk1A; // some sort of state?
    u8 unk1B; //bitfield of available sidekick commands
    u8 unk1C;
    u8 unk1D;
    u16 pad1E;
    f32 unk20;
    f32 unk24;
    Object *unk28;
    Vec3f *unk2C;
    f32 unk30[2];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    s32 unk4C; // some sort of flag
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    s16 unk64;
    s16 unk66;
    Vec3f *unk68;
    s16 unk6C[4];
    Vec3f unk74[4];
    u16 unkA4;
    s16 unkA6;
    Vec3f unkA8;
    Vec3f unkB4;
    Vec3f unkC0;
    DLL27_Data unkCC;
    Object *unk32C;
    f32 unk330;
    s32 unk334;
    Object *unk338;
    u8 pad33C[0x360 - 0x33C];
    HeadAnimation unk360;
    Vec3f unk384[4];
    Vec3f unk3B4;
    CurveSetup *unk3C0;
    u8 unk3C4;
    UnkCurvesStruct unk3C8;
    CurveSetup *unk4D0;
    CurveSetup *unk4D4;
    u16 unk4D8;
    u8 unk4DA;
    u8 pad4DB;
    Route unk4DC[4];
    Route unk59C;
    CurveSetup *unk5CC;
    void *unk5D0;
    Vec3f *unk5D4;
    Vec3f unk5D8;
    Object *unk5E4[3];
    Object *unk5F0;
    Vec3f unk5F4;
    f32 unk600;
    Object *unk604;
    s32 unk608;
    u32 pad60C;
} DLL211_Data;

/* static */ void dll_211_func_514C(Object* arg0, DLL211_Data* arg1);
/* static */ void dll_211_func_52B8(Object* arg0, DLL211_Data* arg1);
/* static */ s32 dll_211_func_53E4(Object* arg0, f32 arg1, DLL211_Data* arg2);
/* static */ void dll_211_func_7794(Object* arg0, u8 *arg1, s16 arg2, CurveSetup** arg3);
/* static */ CurveSetup* dll_211_func_7A7C(DLL211_Data* arg0, CurveSetup* arg1, s32 arg2, u8 arg3);
/* static */ s8 dll_211_func_7BAC(DLL211_Data* arg0, CurveSetup** arg1, u8 *arg2, s32 arg3);
static CurveSetup* dll_211_func_7DB8(DLL211_Data* arg0, CurveSetup* arg1, void* arg2);
static CurveSetup* dll_211_func_7EFC(DLL211_Data* arg0, CurveSetup* arg1, s32 arg2, s32 arg3);
static CurveSetup* dll_211_func_8114(CurveSetup* arg0);
/* static */ f32 dll_211_func_81D8(Object* arg0);
/* static */ void dll_211_func_82B8(DLL211_Data* arg0);
static void dll_211_func_8470(Object* arg0, Vec3f* arg1);
static void dll_211_func_87E4(Object* arg0);
/* static */ void dll_211_func_885C(Object* arg0);
static void dll_211_func_88F4(Object* arg0, s16 arg1);
/* static */ void dll_211_func_8974(Object* arg0, UnkCurvesStruct* arg1, f32 arg2);
static void dll_211_func_8A94(Object* arg0, UnkCurvesStruct* arg1);
static void dll_211_func_8ED0(f32 arg0, f32 arg1, f32* arg2);
static void dll_211_func_8F84(Object* arg0, Vec3f* arg1, f32* arg2);
static void dll_211_func_9024(DLL211_Data* arg0, Vec3f *arg1);
/* static */ void dll_211_func_9050(Object* arg0, DLL211_Data* arg1);
/* static */ void dll_211_func_940C(Object* arg0, DLL211_Data* arg1);
/* static */ Object* dll_211_func_94BC(Object* arg0, f32 arg1);
static void dll_211_func_95E0(Object* arg0, DLL211_Data *arg1, s16* arg2);
static s32 dll_211_func_9668(DLL27_Data* arg0);

/*0x0*/ static s16 _data_0[][3] = {
    { 0x06d7, 0x0500, 0x0000 },
    { 0x01dc, 0x0500, 0x0000 },
    { 0x095b, 0x0100, 0x0000 },
    { 0x0037, 0x1000, 0x0000 },
    { 0x0038, 0x1000, 0x0000 },
    { 0x0039, 0x1000, 0x0000 },
    { 0x04b8, 0x1500, 0x0014 },
    { 0x002a, 0x0100, 0x0000 },
    { 0x002b, 0x0100, 0x0000 },
    { 0x002c, 0x0100, 0x0000 },
    { 0x0034, 0x0100, 0x0000 },
    { 0x0035, 0x0100, 0x0000 },
    { 0x0036, 0x0100, 0x0000 },
    { 0x003a, 0x0100, 0x0000 },
    { 0x003b, 0x0100, 0x0000 },
    { 0x003c, 0x0100, 0x0000 },
    { 0x003d, 0x0100, 0x0000 },
    { 0x003e, 0x0100, 0x0000 },
    { 0x003f, 0x1500, 0x0000 },
};
/*0x74*/ static u32 _data_74[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41880000, 0x00000000
};
/*0x8C*/ static u32 _data_8C[] = {
    0x00000000, 0x41080000
};
/*0x94*/ static u32 _data_94[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0xA0*/ static u32 _data_A0 = 0x41000000;
/*0xA4*/ static f32 _data_A4[] = {
    0.0f, 0.33f, 0.99f, 1.98f, 3.3f, 4.95f,
    6.93f, 9.24f, 11.88f, 14.85f, 18.18f, 21.84f,
    25.83f, 30.15f, 34.8f, 39.78f, 45.09f, 50.73f
};
/*0xEC*/ static u32 _data_EC[] = {
    0x00000200, 0x04000007, 0x07000a00, 0x000d0000, 0x00000013, 0x00000000, 0x00001a1c, 0x1c000000, 
    0x00000023, 0x00000000, 0x00000000, 0x00001313, 0x00000000, 0x02000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x164*/ static u32 _data_164 = 0x01020000;
/*0x168*/ static u32 _data_168[] = {
    0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005
};

/*0x0*/ static u8 _bss_0[0x8];
/*0x8*/ static u8 _bss_8[0x8];
/*0x10*/ static u8 _bss_10[0x40];

// offset: 0x0 | ctor
void dll_211_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_ctor.s")

// offset: 0xF8 | dtor
void dll_211_dtor(void *dll) { }

// offset: 0x104 | func: 0 | export: 0
void dll_211_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_setup.s")

// offset: 0x3FC | func: 1 | export: 1
void dll_211_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_control.s")

// offset: 0xD1C | func: 2 | export: 2
void dll_211_update(Object *self) { }

// offset: 0xD28 | func: 3 | export: 3
void dll_211_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_print.s")

// offset: 0xF20 | func: 4 | export: 4
void dll_211_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_free.s")

// offset: 0x1040 | func: 5 | export: 5
u32 dll_211_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1050 | func: 6 | export: 6
u32 dll_211_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL211_Data);
}

// offset: 0x1064 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1064.s")

// offset: 0x1074 | func: 8 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1074.s")

// offset: 0x108C | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_108C.s")

// offset: 0x109C | func: 10 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_109C.s")

// offset: 0x10B4 | func: 11 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_10B4.s")

// offset: 0x10C4 | func: 12 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_10C4.s")

// offset: 0x10D4 | func: 13 | export: 13
s32 dll_211_func_10D4(Object* self) {
    DLL211_Data* objData;
    s32 sideCmdBits;

    objData = self->data;

    if (main_get_bits(BIT_Tricky_Unlocked_Sidekick_Commands)) {
        sideCmdBits = objData->unk1B | Sidekick_Command_FLAG_01_Heel;

        //Check if Flame command should be shown
        if ((objData->unk18 == 0xB) || 
            ((objData->unk18 == 0x10) && (objData->unk1A == 1)) || 
            ((objData->unk18 == 0x11) && (objData->unk1A == 1))
        ) {
            sideCmdBits |= Sidekick_Command_FLAG_10_Flame;
        }

        //Check if Tricky's ball is unlocked
        if (main_get_bits(BIT_Tricky_Ball_Unlocked)) {
            sideCmdBits |= Sidekick_Command_FLAG_20_Play;
        }

        //Check if Guard isn't unlocked
        if (main_get_bits(BIT_Tricky_Learned_Guard) == FALSE) {
            sideCmdBits &= ~Sidekick_Command_FLAG_08_Guard;
        }

        //Check if Distract isn't unlocked
        if (main_get_bits(BIT_Tricky_Learned_Distract) == FALSE) {
            sideCmdBits &= ~Sidekick_Command_FLAG_04_Distract;
        }

        //Check if Flame isn't unlocked
        if (main_get_bits(BIT_Tricky_Learned_Flame) == FALSE) {
            sideCmdBits &= ~Sidekick_Command_FLAG_10_Flame;
        }
        
        objData->unk1B = 0;
        
        return sideCmdBits;
    }
    
    return NO_SIDEKICK_COMMAND;
}

// offset: 0x1230 | func: 14 | export: 26
int dll_211_func_1230(Object* self, s32* arg1) {
    DLL211_Data* objData = self->data;
    *arg1 = objData->unk1D;
    return TRUE;
}

// offset: 0x1248 | func: 15 | export: 14
/**
  * Enables a sidekick command (e.g. allowing Flame when near vines).
  */
void dll_211_func_1248(Object* self, s32 commandIndex) {
    DLL211_Data* objData = self->data;
    
    if (commandIndex < TOTAL_SIDEKICK_COMMANDS) {
        objData->unk1B |= (1 << commandIndex);
    }
}

// offset: 0x1270 | func: 16 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1270.s")

// offset: 0x1284 | func: 17 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1284.s")

// offset: 0x1298 | func: 18 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1298.s")

// offset: 0x12B4 | func: 19 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_12B4.s")

// offset: 0x12C8 | func: 20 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_12C8.s")

// offset: 0x12D8 | func: 21 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_12D8.s")

// offset: 0x12F0 | func: 22 | export: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_12F0.s")

// offset: 0x1398 | func: 23 | export: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1398.s")

// offset: 0x1408 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1408.s")

// offset: 0x14E8 | func: 25 | export: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_14E8.s")

// offset: 0x1550 | func: 26 | export: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1550.s")

// offset: 0x1578 | func: 27 | export: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1578.s")

// offset: 0x1624 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1624.s")

// offset: 0x183C | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_183C.s")

// offset: 0x18C4 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_18C4.s")

// offset: 0x1F3C | func: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1F3C.s")

// offset: 0x23B8 | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_23B8.s")

// offset: 0x2928 | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_2928.s")

// offset: 0x2938 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_2938.s")

// offset: 0x2B50 | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_2B50.s")

// offset: 0x2C64 | func: 36
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_2C64.s")

// offset: 0x3188 | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3188.s")

// offset: 0x3560 | func: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3560.s")

// offset: 0x3570 | func: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3570.s")

// offset: 0x3664 | func: 40
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3664.s")

// offset: 0x3A78 | func: 41
s32 dll_211_func_3A78(DLL211_Data* arg0) {
    s32 count;
    Object* var_s2;
    Object** temp_s5;
    f32 sp58;
    f32 temp_fs0;
    s16 i;

    var_s2 = NULL;
    temp_s5 = obj_get_all_of_type(OBJTYPE_Baddie, &count);
    for (i = 0; i < count; i++) {
        temp_fs0 = vec3_distance_xz_squared(&temp_s5[i]->globalPosition, &arg0->unk5F4);
        if (var_s2 == NULL) {
            if (gDLL_25->vtbl->func_1158(&temp_s5[i]->globalPosition.x, NULL) == arg0->unk608) {
                sp58 = temp_fs0;
                var_s2 = temp_s5[i];
            }
        } else if ((temp_fs0 < sp58) && (gDLL_25->vtbl->func_1158(&temp_s5[i]->globalPosition.x, NULL) == arg0->unk608)) {
            sp58 = temp_fs0;
            var_s2 = temp_s5[i];
        }
    }
    if (var_s2 != NULL) {
        arg0->unk604 = var_s2;
        dll_211_func_9024(arg0, &var_s2->globalPosition);
        arg0->unk1A = 4;
        return 1;
    }

    return 0;
}

// offset: 0x3C1C | func: 42
s32 dll_211_func_3C1C(Object *arg0) {
    s32 count;
    Object** objects;
    s16 i;

    objects = obj_get_all_of_type(OBJTYPE_Baddie, &count);
    for (i = 0; i < count; i++) {
        if (arg0 == objects[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

// offset: 0x3CAC | func: 43
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3CAC.s")

// offset: 0x4340 | func: 44
#ifndef NON_MATCHING
void dll_211_func_4340(Object* arg0, DLL211_Data* arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_4340.s")
#else
// matches but requires dll_211_func_53E4 to be static
void dll_211_func_4340(Object* arg0, DLL211_Data* arg1) {
    if (dll_211_func_53E4(arg0, 15.0f, arg1) == 0) {
        dll_211_func_83BC(arg0, arg1);
    }
}
#endif

// offset: 0x43AC | func: 45
#ifndef NON_MATCHING
void dll_211_func_43AC(Object* arg0, DLL211_Data* arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_43AC.s")
#else
// matches but requires dll_211_func_53E4 to be static
void dll_211_func_43AC(Object* arg0, DLL211_Data* arg1) {
    if (dll_211_func_53E4(arg0, 5.0f, arg1) == 0) {
        dll_211_func_83BC(arg0, arg1);
    }
}
#endif

// offset: 0x4418 | func: 46
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_4418.s")
void dll_211_func_4418(Object* arg0, DLL211_Data* arg1);
#else
// matches but requires dll_211_func_82B8 (matched) and dll_211_func_53E4 to be static
void dll_211_func_4418(Object* arg0, DLL211_Data* arg1) {
    s32 pad;
    f32 sp30;

    switch (arg1->unk1A) {
    case 0:
        arg1->unk5E4[0] = arg1->unk28;
        arg1->unk1A = 1;
        ((f32*)arg1->unk5E4)[1] = 180.0f;
        /* fallthrough */
    case 1:
        if (((DLL_Unknown*)arg1->unk5E4[0]->dll)->vtbl->func[9].withOneArgS32((s32)arg1->unk5E4[0]) != 0) {
            if (dll_211_func_53E4(arg0, 13.0f, arg1) == 0) {
                func_80023D30(arg0, 0x11, 0.0f, 0U);
                arg1->unk38 = 0.03f;
                arg1->unk4C |= 0x10;
                arg1->unk1A = 3;
                ((DLL_Unknown*)arg1->unk5E4[0]->dll)->vtbl->func[8].withTwoArgs(arg1->unk5E4[0], arg0);
            }
            return;
        }
        if (dll_211_func_53E4(arg0, 25.0f, arg1) == 0) {
            if (((f32*)arg1->unk5E4)[1] > 0.0f) {
                dll_211_func_83BC(arg0, arg1);
                arg1->unk38 = 0.005f;
                ((f32*)arg1->unk5E4)[1] -= gUpdateRateF;
                if (((f32*)arg1->unk5E4)[1] <= 0.0f) {
                    ((f32*)arg1->unk5E4)[2] = 60.0f;
                }
            } else {
                dll_211_func_8308(arg0, 0x10);
                arg1->unk38 = 0.02f;
                ((f32*)arg1->unk5E4)[2] -= gUpdateRateF;
                if (((f32*)arg1->unk5E4)[2] <= 0.0f) {
                    ((f32*)arg1->unk5E4)[1] = 180.0f;
                }
            }
        }
        return;
    default:
        return;
    case 6:
        if (arg0->animProgress >= 0.65f) {
            arg1->unk5E4[0]->srt.transl.f[1] += 5.0f;
            sp30 = fsin16_precise(arg0->srt.yaw);
            ((DLL_Unknown*)arg1->unk5E4[0]->dll)->vtbl->func[7].withFiveArgsCustom4(arg1->unk5E4[0], arg0, -sp30, 1.0f, -fcos16_precise(arg0->srt.yaw));
            arg1->unk1A = 2;
        }
        return;
    case 2:
        if (arg0->animProgress >= 0.95f) {
            dll_211_func_82B8(arg1);
        }
        return;
    case 3:
        if (arg0->animProgress >= 0.5f) {
            arg1->unk1A = 4;
        }
        return;
    case 4:
        if (arg0->animProgress >= 0.95f) {
            arg1->unk28 = arg1->unk8;
            dll_211_func_9024(arg1, &arg1->unk8->globalPosition);
            arg1->unk1A = 5;
            /* fallthrough */
        } else {
            return;
        }
    case 5:
        if (dll_211_func_53E4(arg0, 50.0f, arg1) == 0) {
            func_80023D30(arg0, 0x13, 0.0f, 0U);
            arg1->unk38 = 0.03f;
            arg1->unk1A = 6;
        }
        return;
    }
}
#endif

// offset: 0x47E8 | func: 47
#ifndef NON_MATCHING
void dll_211_func_47E8(Object* arg0, DLL211_Data* arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_47E8.s")
#else
// matches but requires dll_211_func_53E4 to be static
void dll_211_func_47E8(Object* arg0, DLL211_Data* arg1) {
    s32 sp24;
    s32 var_v0;
    s32 var_v1_2;

    sp24 = atan2f_to_s(arg1->unk28->srt.transl.x - arg0->srt.transl.x, arg1->unk28->srt.transl.f[2] - arg0->srt.transl.f[2]);
    if (arg1->unk1A == 0) {
        ((s32*)arg1->unk5E4)[0] = rand_next(0, 1);
        if (((s32*)arg1->unk5E4)[0] == 0) {
            ((s32*)arg1->unk5E4)[0] = -1;
        }
        ((s32*)arg1->unk5E4)[1] = sp24;
        arg1->unk1A = 1;
    }
    var_v0 = sp24 - (((s32*)arg1->unk5E4)[1] & 0xFFFF);
    CIRCLE_WRAP(var_v0);
    if (var_v0 >= 0) {
        var_v1_2 = var_v0;
    } else {
        var_v1_2 = -var_v0;
    }
    if (var_v1_2 < 0x2000) {
        ((s32*)arg1->unk5E4)[1] += (((s32*)arg1->unk5E4)[0] << 0xB);
    }
    ((f32*)arg1->unk5E4)[2] = arg1->unk28->srt.transl.x - (fsin16(((s32*)arg1->unk5E4)[1]) * 50.0f);
    ((f32*)arg1->unk5E4)[3] = arg1->unk28->srt.transl.f[1];
    arg1->unk5F4.x = arg1->unk28->srt.transl.f[2] - (fcos16(((s32*)arg1->unk5E4)[1]) * 50.0f);
    dll_211_func_53E4(arg0, 5.0f, arg1);
}
#endif

// offset: 0x4974 | func: 48
#ifndef NON_MATCHING
void dll_211_func_4974(Object* arg0, DLL211_Data* arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_4974.s")
#else
// matches but requires these as static:
// - dll_211_func_53E4
// - dll_211_func_82B8 (matched)
// - dll_211_func_885C (matched)
// - dll_211_func_9050 (matched)
// - dll_211_func_940C (matched)
// - dll_211_func_94BC (matched)
void dll_211_func_4974(Object* arg0, DLL211_Data* arg1) {
    arg1->unk28 = dll_211_func_94BC(arg1->unk8, 150.0f);
    if (arg1->unk28 != NULL) {
        dll_211_func_9024(arg1, &arg1->unk28->srt.transl);
    } else if (!(arg1->unk4C & 0x10)) {
        dll_211_func_82B8(arg1);
    }

    switch (arg1->unk1A) {
    case 0:
        if (dll_211_func_53E4(arg0, 50.0f, arg1) == 0) {
            arg1->unk1A = 1;
            arg1->unk5F4.y = 0;
            arg1->unk5F4.x = 0.0f;
            dll_211_func_83BC(arg0, arg1);
        }
        return;
    case 1:
        dll_211_func_885C(arg0);
        if (vec3_distance_xz_squared(&arg0->srt.transl, arg1->unk2C) > 5625.0f) {
            arg1->unk1A = 0;
            return;
        }
        if (arg1->unk4[1] != 0 && arg1->unk5F4.y != 0) {
            arg1->unk1A = 2;
            return;
        }
        arg1->unk5F4.x -= gUpdateRateF;
        if (arg1->unk5F4.x < 0.0f) {
            arg1->unk5F4.x = rand_next(200, 600) * 0.5f;
            dll_211_func_95E0(arg0, arg1, _data_0[6]);
        }
        break;
    case 2:
        if (dll_211_func_53E4(arg0, 25.0f, arg1) == 0) {
            dll_211_func_9050(arg0, arg1);
            arg1->unk4[1]--;
            func_80023D30(arg0, 0x205, 0.0f, 0U);
            arg1->unk38 = 0.005f;
            arg1->unk4C |= 0x10;
            arg1->unk1A = 3;
            arg1->unk5F4.y = 0.0f;
            return;
        }
        break;
    case 3:
        if (arg0->animProgress >= 0.95f) {
            dll_211_func_940C(arg0, arg1);
            arg1->unk4C &= ~0x10;
            arg1->unk1A = 0;
            return;
        }
        if (arg1->unk28 != NULL) {
            dll_211_func_885C(arg0);
        }
        break;
    }
}
#endif

// offset: 0x4C94 | func: 49
#ifndef NON_MATCHING
void dll_211_func_4C94(Object* arg0, DLL211_Data* arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_4C94.s")
#else
// matches but requires these as static:
// - dll_211_func_53E4
// - dll_211_func_82B8 (matched)
// - dll_211_func_885C (matched)
// - dll_211_func_9050 (matched)
// - dll_211_func_940C (matched)
void dll_211_func_4C94(Object* arg0, DLL211_Data* arg1) {
    switch (arg1->unk1A) {
    case 0:
        if (dll_211_func_53E4(arg0, 50.0f, arg1) == 0) {
            dll_211_func_95E0(arg0, arg1, _data_0[rand_next(0xA, 0xC)]);
            arg1->unk1A = 1;
            func_80023D30(arg0, 0x106, 0.0f, 0U);
            arg1->unk5F4.y = 0.0f;
            return;
        }
        return;
    case 1:
        if ((arg1->unk4[1] != 0) && (arg1->unk5F4.y != 0)) {
            arg1->unk1A = 2;
            return;
        }
        dll_211_func_885C(arg0);
        if (rand_next(0, 0xA) == 0) {
            dll_211_func_95E0(arg0, arg1, _data_0[rand_next(0xA, 0xC)]);
        }
        break;
    case 2:
        if (dll_211_func_53E4(arg0, 25.0f, arg1) == 0) {
            dll_211_func_9050(arg0, arg1);
            arg1->unk4[1]--;
            func_80023D30(arg0, 0x205, 0.0f, 0U);
            arg1->unk38 = 0.005f;
            arg1->unk4C |= 0x10;
            arg1->unk1A = 3;
            arg1->unk5F4.y = 0.0f;
            return;
        }
        break;
    case 3:
        if (arg0->animProgress >= 0.95f) {
            dll_211_func_940C(arg0, arg1);
            dll_211_func_82B8(arg1);
        } else {
            dll_211_func_885C(arg0);
        }
        break;
    }
}
#endif

// offset: 0x4F3C | func: 50
s32 dll_211_func_4F3C(Object* arg0, f32 arg1, DLL211_Data* arg2, Vec3f* arg3) {
    f32 temp2;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 temp;

    temp_fv1 = arg3->f[0] - arg0->srt.transl.f[0];
    temp_fa0 = arg3->f[2] - arg0->srt.transl.f[2];
    temp = arg2->unk20 * 10.0f;
    temp_fv0 = _data_A4[temp] + arg1;
    temp_fv0 *= temp_fv0;
    temp2 = SQ(temp_fv1) + SQ(temp_fa0);
    if (temp_fv0 < temp2) {
        dll_211_func_8ED0(arg2->unk20, 1.5f, &arg2->unk20);
    } else {
        arg2->unk20 += -0.03f * gUpdateRateF;
        if (arg2->unk20 < 0.0f) {
            arg2->unk20 = 0.0f;
            return 0;
        }
    }

    dll_211_func_8470(arg0, arg3);
    return 1;
}

// offset: 0x507C | func: 51
void dll_211_func_507C(Object* arg0, DLL211_Data* arg1) {
    dll_211_func_8F84(arg0, &arg1->unk3C8.unkA0->pos, arg1->unk30);
    arg1->unk20 = 1.5f;
    func_80023D30(arg0, 0x15, 0.0f, 0U);
    arg1->unk19 = 9;
    dll_211_func_95E0(arg0, arg1, _data_0[rand_next(7, 9)]);
}


// offset: 0x514C | func: 52
/* static */ void dll_211_func_514C(Object* arg0, DLL211_Data* arg1) {
    dll_211_func_8F84(arg0, &arg1->unk3C8.unkA0->pos, arg1->unk30);
    arg1->unk4C |= 0xC0;
    arg1->unk44 = (arg1->unk3C8.unkA0->pos.f[1] - arg0->srt.transl.f[1]) / 32.865f;
    arg1->unk40 = 1.0f;
    if (arg1->unk20 == 1.5f) {
        func_80023D30(arg0, 0x17, 0.0f, 0U);
        arg1->unk38 = 0.0135f;
    } else {
        func_80023D30(arg0, 0x18, 0.0f, 0U);
        arg1->unk38 = 0.00975f;
    }
    arg1->unk19 = 0xC;
    dll_211_func_8A94(arg0, &arg1->unk3C8);
    dll_211_func_95E0(arg0, arg1, _data_0[rand_next(7, 9)]);
}

// offset: 0x52B8 | func: 53
/* static */ void dll_211_func_52B8(Object* arg0, DLL211_Data* arg1) {
    dll_211_func_8F84(arg0, &arg1->unk3C8.unkA0->pos, arg1->unk30);
    arg1->unk4C |= 0xC0;
    arg1->unk44 = (arg0->srt.transl.f[1] - arg1->unk3C8.unkA0->pos.f[1]) / 33.114f;
    arg1->unk40 = 1.0f;
    func_80023D30(arg0, 0x19, 0.0f, 0U);
    arg1->unk38 = 0.0125f;
    arg1->unk19 = 0xE;
    dll_211_func_8A94(arg0, &arg1->unk3C8);
    dll_211_func_95E0(arg0, arg1, _data_0[rand_next(7, 9)]);
}

// offset: 0x53E4 | func: 54
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_53E4.s")
#else
// n64: https://decomp.me/scratch/2SdKO
// default.dol: https://decomp.me/scratch/cwvhT
/*
Requires these as static:
- dll_211_func_507C (matched)
- dll_211_func_514C (matched)
- dll_211_func_52B8 (matched)
- dll_211_func_7794 (matched)
- dll_211_func_7A7C (matched)
- dll_211_func_7BAC (matched)
- dll_211_func_81D8 (matched)
- dll_211_func_8974 (matched)
- dll_211_func_8A94 (matched)
*/
typedef struct {
    u8 pad0;
    u8 unk1;
    u16 unk2[4];
    u16 padA;
    f32 unkC;
    f32 unk10;
} UnkSp44;

s32 dll_211_func_53E4(Object* arg0, f32 arg1, DLL211_Data* arg2) {
    CurveSetup* temp_s1_5;
    s32 spC0;
    s32 spBC;
    u32 spB8;                                       /* compiler-managed */
    Vec3f* temp_s1;
    f32 temp_fv0;
    f32 var_fv1;
    u8 spA8[4];
    s16 var_a3;
    s16 var_s1;
    CurveSetup* spA0;
    s32 var_v1;
    s32 var_a0;
    CurveSetup* sp88[4];
    f32 sp84; // fv0?
    CurveSetup* setup;
    Vec3f sp74;
    f32 sp68[2];
    s32 pad2;
    s16 sp60[3];
    CurveSetup* setup2;
    u8 sp5B;
    u8 temp_s1_4;
    u8 temp_v1_3;
    u8 temp_v1_4;
    UnkSp44 sp44;

    temp_s1 = arg2->unk2C;
    spC0 = gDLL_25->vtbl->func_1158(arg0->srt.transl.f, NULL);
    if (spC0 != 0 && spC0 != arg2->unkA4) {
        arg2->unkA4 = spC0;
        arg2->unk4C &= ~0x400;
        for (var_a0 = 0; var_a0 < 4; var_a0++) { arg2->unk6C[var_a0] = 0; }
    }
    spBC = gDLL_25->vtbl->func_1158((f32*)temp_s1, &sp44);
    spB8 = main_get_bits(0x4E2);
    if (spBC != 0 && spBC != spB8) {
        spB8 = spBC;
        main_set_bits(0x4E2, spBC);
    }
    sp84 = arg2->unk20;
    arg2->unk20 += -0.03f * gUpdateRateF;
    if (arg2->unk20 < 0.0f) {
        arg2->unk20 = 0.0f;
    }
    temp_fv0 = vec3_distance_xz_squared(temp_s1, &arg0->srt.transl);
    var_a0 = arg2->unk20 * 10.0f;
    if (var_a0 >= 0x12) {
        var_fv1 = 100.0f;
    } else {
        var_fv1 = _data_A4[var_a0] + arg1;
        var_fv1 *= var_fv1;
    }
    if (var_fv1 < temp_fv0) {
        dll_211_func_8ED0(sp84, 1.5f, &arg2->unk20);
    } else if (temp_fv0 > 400.0f) {
        temp_fv0 = dll_211_func_81D8(arg0);
        if (arg2->unk20 < temp_fv0 && temp_fv0 < sp84) {
            arg2->unk20 = temp_fv0;
        }
    }
    if (spBC == arg2->unkA4) {
        arg2->unk4C |= 0x400;
        for (var_a0 = 0, temp_v1_3 = 1; var_a0 < 4; var_a0++, temp_v1_3 <<= 1) {
            if (sp44.unk1 & temp_v1_3) {
                arg2->unk6C[var_a0] = sp44.unk2[var_a0];
                arg2->unk74[var_a0].x = temp_s1->x;
                arg2->unk74[var_a0].y = temp_s1->y;
                arg2->unk74[var_a0].z = temp_s1->z;
            }
        }
    }
    if (spBC != 0 && spBC == arg2->unkA4) {
        arg2->unkA6 = 0;
    } else {
        var_a3 = arg2->unkA4 * spBC;
        if (var_a3 != 0) {
            for (var_a0 = 0; var_a0 < 4; var_a0++) {
                if ((var_a3 == sp44.unk2[var_a0]) && (sp44.unk1 & (1 << var_a0))) {
                    arg2->unkA6 = var_a3;
                    arg2->unkA8.x = temp_s1->x;
                    arg2->unkA8.y = temp_s1->y;
                    arg2->unkA8.z = temp_s1->z;
                }
            }
        }
    }
    gDLL_25->vtbl->func_12FC((f32*)temp_s1);
    gDLL_25->vtbl->func_1474((f32*)temp_s1, arg2->unkA4);
    if (arg2->unk4C & 0x400) {
        // @the fakest of fakes (this is an empty loop in default.dol)
        for (var_a0 = 0; var_a0 < 4; var_a0++) {
            // FAKE
            do { } while (0);
            if (sp44.unk1 & (1 << var_a0)) {
            }
        }
    }
    var_a3 = gDLL_25->vtbl->func_1474((f32*)temp_s1, arg2->unkA4);
    if (arg2->unk19 != 0xF && ((spBC != 0 && spC0 == spBC) || (var_a3 != 0 && gDLL_25->vtbl->func_15B0((f32*)(&arg0->srt.transl), arg2->unkA4, var_a3) != 0))) {
        arg2->unk19 = 1;
    } else if (arg2->unk19 < 5) {
        var_a3 = gDLL_25->vtbl->func_1474((f32*)temp_s1, arg2->unkA4);\
        if (var_a3 != 0) {
            if (spBC == 0) {
                for (var_a0 = 0; var_a0 < 4; var_a0++) {
                    if (var_a3 == arg2->unk6C[var_a0]) {
                        sp5B = var_a0;
                        arg2->unk19 = 2;
                        break;
                    }
                }
                if (var_a0 == 4) {
                    arg2->unk19 = 0;
                }
            } else if (spC0 != 0) {
                for (var_a0 = 0; var_a0 < 4; var_a0++) {
                    if (var_a3 == arg2->unk6C[var_a0]) {
                        sp5B = var_a0;
                        arg2->unk19 = 2;
                        break;
                    }
                }
                if (var_a0 == 4) {
                    arg2->unk19 = 5;
                }
            } else {
                if (spC0 == 0 && (var_a3 = gDLL_25->vtbl->func_1474((f32*)(&arg0->srt.transl), arg2->unkA4), (var_a3 != 0))) {
                    if (var_a3 == arg2->unkA6) {
                        arg2->unk19 = 3;
                    } else {
                        gDLL_25->vtbl->func_16D4(&arg2->unk8->srt.transl, &arg2->unkC0, var_a3);
                        arg2->unk19 = 4;
                    }
                } else {
                    arg2->unk19 = 0;
                    gDLL_25->vtbl->func_15B0((f32*)(&arg0->srt.transl), arg2->unkA4, var_a3);
                }
            }
        } else if (spBC == 0) {
            arg2->unk19 = 0;
        } else if (spC0 != 0) {
            var_a3 = spBC * spC0;
            if (gDLL_25->vtbl->func_15B0((f32*)(&arg0->srt.transl), arg2->unkA4, var_a3) != 0) {
                if (var_a3 == arg2->unkA6) {
                    arg2->unk19 = 3;
                } else {
                    arg2->unk19 = 5;
                }
            } else {
                for (var_a0 = 0; var_a0 < 4; var_a0++) {
                    if (var_a3 == arg2->unk6C[var_a0]) {
                        sp5B = var_a0;
                        arg2->unk19 = 2;
                        break;
                    }
                }
                if (var_a0 == 4 || var_a3 != arg2->unkA6) {
                    arg2->unk19 = 5;
                }
            }
        } else {
            var_a3 = gDLL_25->vtbl->func_1474((f32*)(&arg0->srt.transl), arg2->unkA4);\
            if (var_a3 != 0) {
                if (spBC == arg2->unkA4) {
                    for (var_a0 = 0; var_a0 < 4; var_a0++) {
                        if (var_a3 == arg2->unk6C[var_a0]) {
                            sp5B = var_a0;
                            arg2->unk19 = 2;
                            break;
                        }
                    }
                    if (var_a0 == 4) {
                        gDLL_25->vtbl->func_16D4(&arg2->unk8->srt.transl, &arg2->unkC0, var_a3);
                        arg2->unk19 = 4;
                    }
                } else if (var_a3 == arg2->unkA6) {
                    arg2->unk19 = 3;
                } else {
                    gDLL_25->vtbl->func_16D4(&arg2->unk8->srt.transl, &arg2->unkC0, var_a3);
                    arg2->unk19 = 4;
                }
            } else {
                // arg2->unk19 = 0; // default.dol
            }
        }
    }
    switch (arg2->unk19) {                             /* switch 1 */
    case 0:                                         /* switch 1 */
        arg2->unk20 = (-0.03f * gUpdateRateF) + sp84;
        if (arg2->unk20 < 0.0f) {
            arg2->unk20 = 0.0f;
        }
        dll_211_func_8470(arg0, temp_s1);
        break;
    default:                                        /* switch 1 */
        break;
    case 1:                                         /* switch 1 */
        if (sp84 != 0.0f) {
            dll_211_func_8470(arg0, temp_s1);
            break;
        }
        /* fallthrough */
    case 2:                                     /* switch 1 */
        if (sp84 != 0.0f) {
            dll_211_func_8470(arg0, &arg2->unk74[sp5B]);
            break;
        }
        /* fallthrough */
    case 4:                                 /* switch 1 */
        if (sp84 != 0.0f) {
            dll_211_func_8470(arg0, &arg2->unkC0);
        }
        /* fallthrough */
        break;
    case 3:                                         /* switch 1 */
        if (sp84 != 0.0f) {
            dll_211_func_8470(arg0, &arg2->unkA8);
            break;
        }

        if (dll_211_func_9668(&arg2->unkCC) == 0) {
            sp68[0] = arg2->unk2C->x - arg0->srt.transl.x;
            sp68[1] = arg2->unk2C->z - arg0->srt.transl.z;
            var_s1 = (u16)arctan2_f(-sp68[0], -sp68[1]);
            var_s1 = arg0->srt.yaw - (var_s1 & 0xFFFF);
            CIRCLE_WRAP(var_s1);
            var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
            if (var_v1 >= 0x1A17) {
                var_s1 = (var_s1 * 5) / 4;
                if (var_s1 > 0) {
                    if (var_s1 >= 0x342F) {
                        func_80023D30(arg0, 12, 0.0f, 0U);
                        var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                        arg2->unk48 = var_v1 / 24490.0f;
                        arg2->unk38 = 0.01f;
                    } else {
                        func_80023D30(arg0, 10, 0.0f, 0U);
                        var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                        arg2->unk48 = var_v1 / 6679.0f;
                        arg2->unk38 = 0.06f;
                    }
                } else if (var_s1 >= 0x342F) {
                    func_80023D30(arg0, 11, 0.0f, 0U);
                    var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                    arg2->unk48 = var_v1 / 24490.0f;
                    arg2->unk38 = 0.01f;
                } else {
                    func_80023D30(arg0, 9, 0.0f, 0U);
                    var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                    arg2->unk48 = var_v1 / 6679.0f;
                    arg2->unk38 = 0.06f;
                }
                arg2->unk19 = 0xF;
                arg2->unk38 /= arg2->unk48;
                arg2->unk3C = 1.0f;
                arg2->unk40 = 1.0f;
                arg2->unk4C |= 0x20;
                arg2->unk4C |= 0x40;
                arg2->unk4C |= 0x100;
            }
        }
        break;
    case 6:                                         /* switch 1 */
        vec3_distance_xz_squared(&arg2->unk3C0->pos, (&arg0->srt.transl));
        if (vec3_distance_xz_squared(&arg2->unk3C0->pos, (&arg0->srt.transl)) < 60.0f) {
            setup = arg2->unk3C0;
            arg2->unk3C8.unk80 = arg2->unk3C4;
            spA0 = dll_211_func_7A7C(arg2, setup, spB8, arg2->unk3C4);
            gDLL_25->vtbl->func_1D30(&arg2->unk3C8, setup, spA0, dll_211_func_7A7C(arg2, spA0, spB8, arg2->unk3C4));
            func_800053B0(&arg2->unk3C8, 0.1f);
            dll_211_func_8974(arg0, &arg2->unk3C8, arg2->unk20);
            dll_211_func_8470(arg0, &arg2->unk3C8.unk68);
            switch (setup->unk1A) {
            case 1:
                dll_211_func_507C(arg0, arg2);
                break;
            case 5:
                dll_211_func_514C(arg0, arg2);
                break;
            case 6:
                dll_211_func_52B8(arg0, arg2);
                break;
            case 2:
            case 7:
                arg2->unk4C |= 0x2000;
                /* fallthrough */
            default:
                arg2->unk19 = 7;
                break;
            }
        } else {
            dll_211_func_8470(arg0, &arg2->unk3C0->pos);
        }
        break;
    case 5:                                         /* switch 1 */
        dll_211_func_7794(arg0, spA8, spC0, sp88);
        
        // NOTE: the return value here is var_a0 (the iterator var) in default.dol
        var_a0 = dll_211_func_7BAC(arg2, sp88, spA8, spB8);
        if (var_a0 == -1) {
            arg2->unk20 = sp84;
            return 2;
        }
        arg2->unk3C4 = spA8[var_a0];
        arg2->unk3C0 = sp88[var_a0];

        // FAKE
        if (1) {
            dll_211_func_8470(arg0, &arg2->unk3C0->pos);
        }
        arg2->unk19 = 6;
        break;
    case 7:                                         /* switch 1 */
        sp74.f[0] = arg2->unk2C->x - arg0->srt.transl.x;
        sp74.f[1] = arg2->unk2C->y - arg0->srt.transl.y;
        sp74.f[2] = arg2->unk2C->z - arg0->srt.transl.z;
        sp60[0] = -arg0->srt.yaw;
        sp60[1] = 0;
        sp60[2] = 0;
        rotate_vec3(&sp60, sp74.f);
        if (sp74.f[2] > 0.0f && arg2->unk20 != 0.0f) {
            if (!(arg2->unk4C & 2) || (arg2->unk64 != spB8) || (arg2->unk68 != arg2->unk2C) || (arg2->unk66 != arg2->unk3C8.unk80)) {
                arg2->unk4C |= 2;
                arg2->unk4C &= ~4;
                arg2->unk64 = spB8;
                arg2->unk68 = arg2->unk2C;
                arg2->unk66 = arg2->unk3C8.unk80;
                route_setup(arg2->unk4DC, arg2->unk3C8.unkA0, arg2->unk2C, (void* ) spB8, arg2->unk3C8.unk80 ^ 1);
                route_setup(&arg2->unk4DC[1], arg2->unk3C8.unkA0, arg2->unk2C, (void* ) spB8, arg2->unk3C8.unk80);
            }
            if (!(arg2->unk4C & 4)) {
                temp_v1_4 = route_find(&arg2->unk4DC[1], 10);
                switch (temp_v1_4) {
                case 1:
                    arg2->unk4C &= ~2;
                    break;
                case -1:
                    arg2->unk4C |= 4;
                    break;
                default:
                    temp_v1_4 = route_find(arg2->unk4DC, 10);
                    if (temp_v1_4 != -1) {
                        if (temp_v1_4 == 1) {
                            arg2->unk4C |= 4;
                        }
                    } else {
                        arg2->unk4C &= ~2;
                    }
                    break;
                }
            }
            arg2->unk50 -= gUpdateRateF;
            if (arg2->unk50 <= 0.0f && arg2->unk4C & 4) {
                temp_s1_4 = arg2->unk3C8.unk80 ^ 1;
                arg2->unk50 = 120.0f;
                if (!temp_s1_4) {
                    func_800053B0(&arg2->unk3C8, 2.0f);
                } else {
                    func_800053B0(&arg2->unk3C8, -2.0f);
                }
                arg2->unk3C8.unk80 = temp_s1_4;
                gDLL_25->vtbl->func_29FC(&arg2->unk3C8);
            }
        } else {
            arg2->unk50 = 120.0f;
            arg2->unk4C &= ~2;
        }
        if ((arg2->unk3C8.unk80 == 0 && arg2->unk3C8.unk10 != 0) || (arg2->unk3C8.unk80 != 0 && arg2->unk3C8.unk10 == 0)) {
            setup2 = dll_211_func_7A7C(arg2, arg2->unk3C8.unkA4, spB8, arg2->unk3C8.unk80);
            if (setup2 != NULL) {
                dummy:
                gDLL_25->vtbl->func_21F4(&arg2->unk3C8, setup2);
                if ((arg2->unk3C8.unk9C->unk1A == 2) || (arg2->unk3C8.unk9C->unk1A == 7)) {
                    if (arg2->unk4C & 0x2000) {
                        arg2->unk4C &= ~0x2000;
                    } else {
                        arg2->unk4C |= 0x2000;
                    }
                }
            } else {
                if (arg2->unk3C8.unk80 != 0) {
                    func_800053B0(&arg2->unk3C8, 2.0f);
                } else {
                    func_800053B0(&arg2->unk3C8, -2.0f);
                }
                arg2->unk3C8.unk9C = (CurveSetup *)((s32)arg2->unk3C8.unk9C ^ (s32)arg2->unk3C8.unkA4);\
                arg2->unk3C8.unkA4 = (CurveSetup *)((s32)arg2->unk3C8.unkA4 ^ (s32)arg2->unk3C8.unk9C);\
                arg2->unk3C8.unk9C = (CurveSetup *)((s32)arg2->unk3C8.unk9C ^ (s32)arg2->unk3C8.unkA4);
            }
        } else {
            setup2 = dll_211_func_7A7C(arg2, arg2->unk3C8.unkA0, spB8, arg2->unk3C8.unk80);
            if (setup2 != arg2->unk3C8.unkA4) {
                dummy2:
                gDLL_25->vtbl->func_27A8(&arg2->unk3C8, setup2);
            }
        }
        dll_211_func_8974(arg0, &arg2->unk3C8, arg2->unk20);
        dll_211_func_8470(arg0, &arg2->unk3C8.unk68);
        switch (arg2->unk3C8.unkA0->unk1A) {
        case 1:
            arg2->unk19 = 8;
            break;
        case 5:
            arg2->unk19 = 11;
            break;
        case 6:
            arg2->unk19 = 0xD;
            break;
        }
        break;
    case 8:                                         /* switch 1 */
        dll_211_func_8ED0(sp84, 1.5f, &arg2->unk20);
        dll_211_func_8974(arg0, &arg2->unk3C8, arg2->unk20);
        dll_211_func_8470(arg0, &arg2->unk3C8.unk68);
        if ((arg2->unk3C8.unk80 == 0 && arg2->unk3C8.unk10 != 0) || (arg2->unk3C8.unk80 != 0 && arg2->unk3C8.unk10 == 0)) {
            gDLL_25->vtbl->func_21F4(&arg2->unk3C8, dll_211_func_7A7C(arg2, arg2->unk3C8.unkA4, spB8, arg2->unk3C8.unk80));
            dll_211_func_507C(arg0, arg2);
        }
        break;
    case 9:                                         /* switch 1 */
        dll_211_func_8ED0(sp84, 2.3f, &arg2->unk20);
        dll_211_func_87E4(arg0);
        if (arg0->animProgress < 0.5f) {
            func_8002493C(arg0, arg2->unk20, &sp44.unk10);
            arg0->srt.transl.x += arg2->unk30[0] * arg2->unk20 * gUpdateRateF;
            arg0->srt.transl.z += arg2->unk30[1] * arg2->unk20 * gUpdateRateF;
        } else {
            func_8002493C(arg0, arg2->unk20 * 0.25f, &sp44.unk10);
            arg0->srt.transl.x += arg2->unk30[0] * (arg2->unk20 * 0.25f) * gUpdateRateF;
            arg0->srt.transl.z += arg2->unk30[1] * (arg2->unk20 * 0.25f) * gUpdateRateF;
        }
        if (func_80024108(arg0, sp44.unk10, gUpdateRateF, NULL) != 0) {
            setup = arg2->unk3C8.unkA0;
            temp_fv0 = setup->pos.x - arg0->srt.transl.x;
            var_fv1 = setup->pos.z - arg0->srt.transl.z;
            temp_fv0 = sqrtf(SQ(temp_fv0) + SQ(var_fv1));
            arg2->unk54 = temp_fv0 / 2.3f;
            arg2->unk58 = 0.0f;
            arg2->unk60 = arg0->srt.transl.y;
            arg2->unk5C = (((setup->pos.y - arg0->srt.transl.y) - (-0.017f * arg2->unk54 * arg2->unk54)) / arg2->unk54);
            func_80023D30(arg0, 0x16, 0.0f, 0U);
            func_800240BC(arg0, arg2->unk58 / arg2->unk54);
            arg2->unk20 = 2.3f;
            arg2->unk19 = 10;
            dll_211_func_8A94(arg0, &arg2->unk3C8);
        }
        arg2->unk38 = 0.0f;
        break;
    case 10:                                        /* switch 1 */
        arg2->unk58 += gUpdateRateF;
        if (arg2->unk54 <= arg2->unk58) {
            arg0->srt.transl.y = arg2->unk3C8.unkA0->pos.y;
            func_800240BC(arg0, 1.0f);
            arg2->unk19 = 7;
            arg2->unk38 = 0.0f;
        } else {
            arg0->srt.transl.y = arg2->unk60 + (arg2->unk5C * arg2->unk58) + (-0.017f * arg2->unk58 * arg2->unk58);
            var_fv1 = arg2->unk54;
            if (var_fv1 <= 24.0f) {
                func_800240BC(arg0, arg2->unk58 / var_fv1);
            } else {
                sp44.unkC = 1.0f;
                if (arg2->unk58 <= 6.0f) {
                    func_800240BC(arg0, arg2->unk58 / 24.0f);
                } else if ((var_fv1 - 6.0f) <= arg2->unk58) {
                    func_800240BC(arg0, ((24.0f - var_fv1) + arg2->unk58) / 24.0f);
                    sp44.unkC = (((arg2->unk54 - arg2->unk58) / 6.0f) * 0.75f) + 0.25f;
                } else {
                    func_800240BC(arg0, (((arg2->unk58 - 6.0f) / (var_fv1 - 12.0f)) * 0.5f) + 0.25f);
                }
            }
            arg2->unkCC.mode = 0;
            arg2->unk38 = 0.0f;
            arg0->srt.transl.x += arg2->unk30[0] * 2.3f * gUpdateRateF * sp44.unkC;
            arg0->srt.transl.z += arg2->unk30[1] * 2.3f * gUpdateRateF * sp44.unkC;
        }
        break;
    case 11:                                        /* switch 1 */
        dll_211_func_8ED0(sp84, 1.5f, &arg2->unk20);
        dll_211_func_8974(arg0, &arg2->unk3C8, arg2->unk20);
        dll_211_func_8470(arg0, &arg2->unk3C8.unk68);
        if ((arg2->unk3C8.unk80 == 0 && arg2->unk3C8.unk10 != 0) || (arg2->unk3C8.unk80 != 0 && arg2->unk3C8.unk10 == 0)) {
            gDLL_25->vtbl->func_21F4(&arg2->unk3C8, dll_211_func_7A7C(arg2, arg2->unk3C8.unkA4, spB8, arg2->unk3C8.unk80));
            dll_211_func_514C(arg0, arg2);
        }
        break;
    case 12:                                        /* switch 1 */
    case 14:                                        /* switch 1 */
        arg2->unkCC.mode = 0;
        dll_211_func_8974(arg0, &arg2->unk3C8, arg2->unk20);
        dll_211_func_87E4(arg0);
        if (arg0->animProgress >= 0.95f) {
            arg2->unk20 = 0.375f;
            dll_211_func_8470(arg0, &arg2->unk3C8.unk68);
            arg2->unk19 = 7;
        }
        break;
    case 13:                                        /* switch 1 */
        dll_211_func_8ED0(sp84, 1.5f, &arg2->unk20);
        dll_211_func_8974(arg0, &arg2->unk3C8, arg2->unk20);
        dll_211_func_8470(arg0, &arg2->unk3C8.unk68);
        if ((arg2->unk3C8.unk80 == 0 && arg2->unk3C8.unk10 != 0) || (arg2->unk3C8.unk80 != 0 && arg2->unk3C8.unk10 == 0)) {
            gDLL_25->vtbl->func_21F4(&arg2->unk3C8, dll_211_func_7A7C(arg2, arg2->unk3C8.unkA4, spB8, arg2->unk3C8.unk80));
            dll_211_func_52B8(arg0, arg2);
        }
        break;
    case 15:                                        /* switch 1 */
        if (arg0->animProgress >= 0.95f) {
            arg2->unk4C &= ~0x20;
            arg2->unk4C &= ~0x40;
            arg2->unk4C &= ~0x100;
            arg2->unk20 = 0.0f;
            dll_211_func_8F84(arg0, temp_s1, arg2->unk30);
            arg2->unk19 = 1;
            return 1;
        }
        break;
    }

    if (arg2->unk19 < 5 && (gDLL_25->vtbl->func_12FC((f32*)(&arg0->srt.transl)) == 0)) {
        arg0->srt.transl.x = arg2->unkB4.x;
        arg0->srt.transl.y = arg2->unkB4.y;
        arg0->srt.transl.z = arg2->unkB4.z;
    }
    arg2->unkB4.x = arg0->srt.transl.x;
    arg2->unkB4.y = arg0->srt.transl.y;
    arg2->unkB4.z = arg0->srt.transl.z;
    if (arg2->unk19 == 0) {
        return 2;
    }
    if (arg2->unk20 == 0.0f && arg2->unk19 < 8) {
        return 0;
    }
    return 1;
}
#endif

// offset: 0x7188 | func: 55
void dll_211_func_7188(Object* arg0) {
    s32 i;
    DLL211_Data* data;

    data = arg0->data;
    for (i = 0; i < 4; i++) {
        func_80031F6C(arg0, i + 1, &data->unk384[i].x, &data->unk384[i].y, &data->unk384[i].z, 0);
    }

    func_80031F6C(arg0, 5, &data->unk3B4.x, &data->unk3B4.y, &data->unk3B4.z, 0);
}

// offset: 0x726C | func: 56
void dll_211_func_726C(Object* arg0) {
    DLL211_Data* data; // sp4C
    Object* temp_a1;
    SRT sp30;

    data = arg0->data;
    temp_a1 = data->unk28;
    sp30.transl.x = data->unk384[0].x;
    sp30.transl.y = data->unk384[0].y;
    sp30.transl.z = data->unk384[0].z;
    sp30.yaw = arg0->srt.yaw;
    if (temp_a1->id == OBJ_GroundAnimator) {
        sp30.roll = ((DLL_Unknown*)temp_a1->dll)->vtbl->func[9].withOneArgS32((s32)temp_a1);
    } else if (temp_a1->id == OBJ_WallAnimator) {
        sp30.roll = ((DLL_Unknown*)temp_a1->dll)->vtbl->func[9].withOneArgS32((s32)temp_a1);
    } else {
        sp30.roll = 0;
    }
    if (rand_next(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(arg0, 0xCA, &sp30, 1, -1, NULL);
    }
    if (rand_next(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(arg0, 0xCB, &sp30, 1, -1, NULL);
    }
    sp30.transl.x = data->unk384[1].x;
    sp30.transl.y = data->unk384[1].y;
    sp30.transl.z = data->unk384[1].z;
    sp30.yaw = arg0->srt.yaw;
    if (rand_next(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(arg0, 0xCA, &sp30, 1, -1, NULL);
    }
    if (rand_next(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(arg0, 0xCB, &sp30, 1, -1, NULL);
    }
}

/*0x0*/ static const char str_0[] = "%d %d %d %d\n";
/*0x10*/ static const char str_10[] = "hits: %d %d %d %d %d %d %d %d";
/*0x30*/ static const char str_30[] = "\nEnergy: %d, Skill %d\n";
/*0x48*/ static const char str_48[] = "find command used on the wrong object\n";
/*0x70*/ static const char str_70[] = "find command used on the wrong object\n";
/*0x98*/ static const char str_98[] = "sideCommandEnable error, command number too high\n";
/*0xCC*/ static const char str_CC[] = "sideFind 1\n";
/*0xD8*/ static const char str_D8[] = "sideFind 2\n";
/*0xE4*/ static const char str_E4[] = "Couldn't find a distract node\n";
/*0x104*/ static const char str_104[] = "DISTRACT_SPOTFINDING (1)\n";
/*0x120*/ static const char str_120[] = "DISTRACT_SPOTTOSPOT (1)\n";
/*0x13C*/ static const char str_13C[] = "DISTRACT_SPOTDISTRACTING (1)\n";
/*0x15C*/ static const char str_15C[] = "DISTRACT_SPOTRETURING (1)\n";
/*0x178*/ static const char str_178[] = "DISTRACT_SPOTFINDING (2)\n";
/*0x194*/ static const char str_194[] = "DISTRACT_SPOTDISTRACTING (2)\n";
/*0x1B4*/ static const char str_1B4[] = "FLAME_NONE\n";
/*0x1C0*/ static const char str_1C0[] = "FLAME_FINDING_OUT\n";
/*0x1D4*/ static const char str_1D4[] = "FLAME_GOINGTOEDGE\n";
/*0x1E8*/ static const char str_1E8[] = "FLAME_TOSTART\n";
/*0x1F8*/ static const char str_1F8[] = "FLAME_OUT\n";
/*0x204*/ static const char str_204[] = "FLAME_FINDING_IN\n";
/*0x218*/ static const char str_218[] = "FLAME_IN\n";
/*0x224*/ static const char str_224[] = "FLAME_TOEND\n";
/*0x234*/ static const char str_234[] = "GUARD_INIT\n";
/*0x240*/ static const char str_240[] = "GUARD_FINDING\n";
/*0x250*/ static const char str_250[] = "GUARD_TOSPOT\n";
/*0x260*/ static const char str_260[] = "GUARD_TOFRONT\n";
/*0x270*/ static const char str_270[] = "GUARD_TOBADDIE\n";
/*0x280*/ static const char str_280[] = "GUARD_FLAME\n";
/*0x290*/ static const char str_290[] = "GUARD_DOWNTOGROWL\n";
/*0x2A4*/ static const char str_2A4[] = "GUARD_GROWL\n";
/*0x2B4*/ static const char str_2B4[] = "GUARD_UPFROMGROWL\n";
/*0x2C8*/ static const char str_2C8[] = "error tricky should never stop when circling\n";
/*0x2F8*/ static const char str_2F8[] = "BADDIEALERT_GOTO\n";
/*0x30C*/ static const char str_30C[] = "BADDIEALERT_BARK %d %d\n";
/*0x324*/ static const char str_324[] = "BADDIEALLERT_GOTOFLAME\n";
/*0x33C*/ static const char str_33C[] = "BADDIEALLERT_FLAME\n";
/*0x350*/ static const char str_350[] = "GROWLAT_GOTO\n";
/*0x360*/ static const char str_360[] = "GROWLAT_GROWLING\n";
/*0x374*/ static const char str_374[] = "GROWLAT_GOTOFLAME\n";
/*0x388*/ static const char str_388[] = "GROWLAT_FLAME\n";
/*0x398*/ static const char str_398[] = "tricky wg %d->%d target wg %d, dest wg %d\n";
/*0x3C4*/ static const char str_3C4[] = "tricky last walk group is zero. Has he been loaded within a walk group? %f %f %f\n";
/*0x418*/ static const char str_418[] = "target is within a walkGroup or its patch\n";
/*0x444*/ static const char str_444[] = "target is not within a walkGroup or any patches\n";
/*0x478*/ static const char str_478[] = "target is within patch group %d\n";
/*0x49C*/ static const char str_49C[] = "Patch %d: Last xyz %f %f %f\n";
/*0x4BC*/ static const char str_4BC[] = "Last Patch Point %f %f %f\n";
/*0x4D8*/ static const char str_4D8[] = "tricky error, target patch %d, targetWalkGroup %d, trickyWalkGroup %d, tricky last walkGroup %d, tricky in patch %d\n";
/*0x550*/ static const char str_550[] = "tricky error 2!!!!!\n";
/*0x568*/ static const char str_568[] = "walk wait\n";
/*0x574*/ static const char str_574[] = "walk free\n";
/*0x580*/ static const char str_580[] = "walk start patch\n";
/*0x594*/ static const char str_594[] = "walk patch exit\n";
/*0x5A8*/ static const char str_5A8[] = "walk end patch\n";
/*0x5B8*/ static const char str_5B8[] = "about to turn\n";
/*0x5C8*/ static const char str_5C8[] = "large turn\n";
/*0x5D4*/ static const char str_5D4[] = "large turn\n";
/*0x5E0*/ static const char str_5E0[] = "standing\n";
/*0x5EC*/ static const char str_5EC[] = "walk to node %d %d\n";
/*0x600*/ static const char str_600[] = "walk types are %d %d %d\n";
/*0x61C*/ static const char str_61C[] = "curve setup\n";
/*0x62C*/ static const char str_62C[] = "walk nodes\n";
/*0x638*/ static const char str_638[] = "No route exists in the forward direction\n";
/*0x664*/ static const char str_664[] = "Reverse direction is quicker\n";
/*0x684*/ static const char str_684[] = "Reversing direction\n";
/*0x69C*/ static const char str_69C[] = "Jump run up\n";
/*0x6AC*/ static const char str_6AC[] = "Jump prep\n";
/*0x6B8*/ static const char str_6B8[] = "Jumping\n";
/*0x6C4*/ static const char str_6C4[] = "Jump up run up\n";
/*0x6D4*/ static const char str_6D4[] = "i before %d\n";
/*0x6E4*/ static const char str_6E4[] = "i now %d\n";
/*0x6F0*/ static const char str_6F0[] = "JUMPDOWN or JUMPUP\n";
/*0x704*/ static const char str_704[] = "JUMPDOWN_RUNUP\n";
/*0x714*/ static const char str_714[] = "TURNING\n";
/*0x720*/ static const char str_720[] = "entered a non valid movementstate\n";
/*0x744*/ static const char str_744[] = "tricky left a valid area, pushing him back...\n";
/*0x774*/ static const char str_774[] = "";
/*0x778*/ static const DLTri _rodata_778[] = {
    MASK_TRI(0x40, 2, 1, 0),
    MASK_TRI(0x40, 3, 1, 2),
    MASK_TRI(0x40, 4, 5, 6),
    MASK_TRI(0x40, 6, 5, 7),
    MASK_TRI(0x40, 6, 3, 2),
    MASK_TRI(0x40, 3, 6, 7),
    MASK_TRI(0x40, 3, 7, 1),
    MASK_TRI(0x40, 7, 5, 1),
    MASK_TRI(0x40, 4, 0, 1),
    MASK_TRI(0x40, 4, 1, 5),
    MASK_TRI(0x40, 0, 4, 2),
    MASK_TRI(0x40, 2, 4, 6)
};
/*0x838*/ static const Vec3f _rodata_838[] = {
    VEC3F(-12.0f, 20.0f, -20.0f),
    VEC3F(12.0f, 20.0f, -20.0f),
    VEC3F(-12.0f, 20.0f, 20.0f),
    VEC3F(12.0f, 20.0f, 20.0f),
    VEC3F(-12.0f, -7.0f, -20.0f),
    VEC3F(12.0f, -7.0f, -20.0f),
    VEC3F(-12.0f, -7.0f, 20.0f),
    VEC3F(12.0f, -7.0f, 20.0f)
};
/*0x898*/ static const char str_898[] = "Reached end of network, curve %d at (%f,%f)\n";
/*0x8C8*/ static const char str_8C8[] = "in water\n";
/*0x8D4*/ static const char str_8D4[] = "out of water\n";
/*0x8E4*/ static const char str_8E4[] = "Vel %f\n";
/*0x8EC*/ static const char str_8EC[] = "in water\n";
/*0x8F8*/ static const char str_8F8[] = "out of water\n";
/*0x908*/ static const char str_908[] = "in water\n";
/*0x914*/ static const char str_914[] = "out of water\n";

// offset: 0x74C4 | func: 57
void dll_211_func_74C4(Object* arg0, Gfx** arg1, Mtx** arg2, Vtx** arg3) {
    Vtx* sp6C;
    DLL211_Data* data;
    s32 i;
    f32 sp60;
    SRT sp48;
    Vtx* var_s0;

    data = arg0->data;
    // unk5E4 is an array of Object pointers? Unless the given objects data is not Tricky?
    sp60 = ((f32*)data->unk5E4)[0];
    sp6C = *arg3;
    sp48.transl.x = arg0->srt.transl.x;
    var_s0 = sp6C;
    sp48.transl.y = arg0->srt.transl.y + sp60;
    if (data->unk1A != 2) {
        sp48.transl.y += 5.0f;
    }
    sp48.transl.z = arg0->srt.transl.z;
    sp48.scale = 0.001f;
    gDLL_17_partfx->vtbl->spawn(arg0, 0x1B6, &sp48, 0x200001, -1, NULL);
    dl_set_prim_color(arg1, 0xFF, 0xFF, 0xFF, 0x80);
    gSPGeometryMode(*arg1, 0xFFFFFF, 0x210405);
    dl_apply_geometry_mode(arg1);
    gDPSetCombineLERP((*arg1), TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED);
    dl_apply_combine(arg1);
    for (i = 0; i < 8; i++) {
        var_s0->v.ob[0] = _rodata_838[i].x * 20.0f;
        if (i < 4) {
            var_s0->v.ob[1] = sp60 * 20.0f;
        } else {
            var_s0->v.ob[1] = -0x8C;
        }
        var_s0->v.ob[2] = _rodata_838[i].z * 20.0f;
        var_s0->v.cn[0] = 0xFF;
        var_s0->v.cn[1] = 0;
        var_s0->v.cn[2] = 0;
        var_s0->v.cn[3] = 0x40;
        var_s0++;
    }
    gDPSetOtherMode(
        (*arg1),
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
        G_AC_NONE | G_ZS_PIXEL | Z_CMP | Z_UPD | IM_RD | CVG_DST_SAVE | ZMODE_XLU | FORCE_BL | G_RM_FOG_SHADE_A | GBL_c2(G_BL_CLR_IN, G_BL_0, G_BL_CLR_MEM, G_BL_1MA)
    );
    dl_apply_other_mode(arg1);
    sp48.transl.x = arg0->srt.transl.x;
    sp48.transl.y = arg0->srt.transl.y;
    sp48.transl.z = arg0->srt.transl.z;
    sp48.yaw = arg0->srt.yaw;
    sp48.pitch = 0;
    sp48.roll = 0;
    sp48.scale = 0.05f;
    camera_setup_object_srt_matrix(arg1, arg2, &sp48, 1.0f, 0.0f, NULL);
    gSPVertex((*arg1)++, OS_PHYSICAL_TO_K0(sp6C), 8, 0);
    dl_triangles(arg1, _rodata_778, 0xC);
    *arg3 = var_s0;
}

// offset: 0x7794 | func: 58
/* static */ void dll_211_func_7794(Object* arg0, u8 *arg1, s16 arg2, CurveSetup** arg3) {
    s32 i;
    f32 sp7C[4];
    CurveNode* temp_a1;
    s32 sp74;
    CurveSetup* temp_s1;
    f32 temp_fs0;
    u8 var_a1;
    u8 sp6A;
    u8 var_a2;
    DLL211_Data* data;

    data = arg0->data;
    temp_a1 = gDLL_26_Curves->vtbl->func_1BC(&sp74);
    for (i = 0; i < 4; i++) {
        sp7C[i] = 3.4028235e38f;
        arg3[i] = 0;   
    }

    if (arg2 == 0) {
        return;
    }

    for (i = 0; i < sp74; i++) {
        temp_s1 = temp_a1[i].setup;
        if (temp_s1->curveType == 0x24 && temp_s1->unk3 == 0) {
            if (
                (temp_s1->type22.unk30 == -1 || main_get_bits(temp_s1->type22.unk30) != 0) &&
                (temp_s1->type22.usedBit == -1 || main_get_bits(temp_s1->type22.usedBit) == 0)
            ) {
                temp_fs0 = 
                    SQ(data->unk2C->z - temp_s1->pos.z) + (
                        (SQ(arg0->globalPosition.x - temp_s1->pos.x) + SQ(arg0->globalPosition.z - temp_s1->pos.z)) +
                        SQ(data->unk2C->x - temp_s1->pos.x)
                    );
                if (temp_fs0 < sp7C[3]) {
                    for (var_a1 = 0; var_a1 < 4; var_a1++) {
                        if ((temp_s1->links[var_a1] >= 0) && (arg2 == temp_s1->unk4_u8[var_a1])) {
                            sp6A = temp_s1->unk1B >> var_a1;
                            break;
                        }
                    }

                    if (var_a1 != 4) {
                        for (var_a1 = 0; var_a1 < 4; var_a1++) {
                            if (temp_fs0 < sp7C[var_a1]) {
                                for (var_a2 = 3; var_a2 > var_a1; var_a2--) {
                                    arg1[var_a2] = arg1[var_a2 - 1];
                                    arg3[var_a2] = arg3[var_a2 - 1];
                                    sp7C[var_a2] = sp7C[var_a2 - 1];
                                }
                                arg1[var_a1] = (sp6A & 1) ^ 1;
                                arg3[var_a1] = temp_s1;
                                sp7C[var_a1] = temp_fs0;
                                break;
                            }    
                        }
                    }
                }
            }
        }
    }
}

// offset: 0x7A7C | func: 59
/* static */ CurveSetup* dll_211_func_7A7C(DLL211_Data* arg0, CurveSetup* arg1, s32 arg2, u8 arg3) {
    CurveSetup* var_v1;

    var_v1 = NULL;
    if (arg0->unk4D0 == arg1 && arg0->unk4D8 == arg2 && arg0->unk4DA == arg3) {
        var_v1 = dll_211_func_8114(arg0->unk4D4);
    }
    if (var_v1 == NULL) {
        var_v1 = dll_211_func_7EFC(arg0, arg1, arg2, arg3);
        if (var_v1 == NULL) {
            var_v1 = dll_211_func_7DB8(arg0, arg1, (void* ) arg2);
            if (var_v1 == NULL) {
                var_v1 = dll_211_func_7EFC(arg0, arg1, 0, arg3);
                if (var_v1 == NULL) {
                    return NULL;
                }
            }
        }
    }
    arg0->unk4D0 = arg1;
    arg0->unk4D4 = var_v1;
    arg0->unk4D8 = arg2;
    arg0->unk4DA = arg3;
    return var_v1;
}

// offset: 0x7BAC | func: 60
/* static */ s8 dll_211_func_7BAC(DLL211_Data* arg0, CurveSetup** arg1, u8 *arg2, s32 arg3) {
    s8 sp5C[4];
    s8 sp5B;
    s8 temp_v0;
    s8 var_s0;
    s8 var_s2;

    for (sp5B = 0; sp5B < 4; sp5B++) {
        if (arg1[sp5B] != NULL) {
            route_setup(&arg0->unk4DC[sp5B], arg1[sp5B], arg0->unk2C, arg3, arg2[sp5B]);
        }
    }

    for (sp5B = 0; sp5B < 0x64; sp5B++) {
        var_s2 = 0;
        for (var_s0 = 0; var_s0 < 4; var_s0++) {
            if (arg1[var_s0] != NULL) {
                sp5C[var_s0] = route_find(&arg0->unk4DC[var_s0], 5);
            } else {
                sp5C[var_s0] = -1;
            }
            temp_v0 = sp5C[var_s0];
            if (temp_v0 != -1) {
                if (temp_v0 == 1) {
                    return var_s0;
                }
            } else {
                arg1[var_s0] = NULL;
                var_s2++;
            } 
        }

        switch (var_s2) {
            case 3:
                if (*arg1 != NULL) {
                    sp5C[0] = route_find(arg0->unk4DC, 500);
                }
            
                if (sp5C[0] == 1) {
                    return 0;
                }
            
                return -1;
            case 4:
                return -1;
            default:
                break;
        }    
    }

    return -1;
}

// offset: 0x7DB8 | func: 61
static CurveSetup* dll_211_func_7DB8(DLL211_Data* arg0, CurveSetup* arg1, void* arg2) {
    if (arg2 == NULL) {
        return NULL;
    }

    if (arg2 == arg0->unk5D0 && arg1 == arg0->unk5CC) {
        arg0->unk5CC = route_next(&arg0->unk59C);
        if (arg0->unk5CC == NULL) {
            return NULL;
        }

        arg0->unk5CC = dll_211_func_8114(arg0->unk5CC);
        if (arg0->unk5CC != NULL) {
            return arg0->unk5CC;
        }
    }

    route_setup(&arg0->unk59C, arg1, arg0->unk2C, arg2, arg0->unk3C8.unk80);
    if (route_find(&arg0->unk59C, 500) != 1) {
        return NULL;
    }

    route_reconstruct(&arg0->unk59C);
    arg0->unk5CC = route_next(&arg0->unk59C);
    arg0->unk5D0 = arg2;
    return arg0->unk5CC;
}

// offset: 0x7EFC | func: 62
static CurveSetup* dll_211_func_7EFC(DLL211_Data* arg0, CurveSetup* arg1, s32 arg2, s32 arg3) {
    CurveSetup *sp68[4];
    f32 temp_fv0;
    f32 var_fs0;
    u16 var_s0;
    u16 var_s1;
    u16 var_s2;
    u16 var_s3;

    for (var_s0 = 0, var_s2 = 0, var_s3 = 1; var_s0 < 4; var_s0++, var_s3 <<= 1, arg3 <<= 1) {
        if (arg1->links[var_s0] >= 0 && ((arg3^0) != (arg1->unk1B & var_s3)) == 0) {
            sp68[var_s2] = gDLL_26_Curves->vtbl->func_39C(arg1->links[var_s0]);
            // FAKE
            if (1) { }
            if (sp68[var_s2] != NULL) {
                if ((arg2 == 0 || arg2 == (arg1->unk4_u8[var_s2]))) {
                    if (
                        (sp68[var_s2]->type22.unk30 == -1 || main_get_bits(sp68[var_s2]->type22.unk30) != 0) &&
                        (sp68[var_s2]->type22.usedBit == -1 || main_get_bits(sp68[var_s2]->type22.usedBit) == 0)
                    ) {
                        var_s2++;
                    }
                }
            }
        }
    }

    if (var_s2) {
        var_s0 = 0;
        var_fs0 = vec3_distance_xz_squared(&arg0->unk8->globalPosition, &sp68[var_s0]->pos);
        var_s1 = var_s0;
        for (var_s0 = 1; var_s0 < var_s2; var_s0++) {
            temp_fv0 = vec3_distance_xz_squared(&arg0->unk8->globalPosition, &sp68[var_s0]->pos);
            if (temp_fv0 < var_fs0) {
                var_fs0 = temp_fv0;
                var_s1 = var_s0;
            }
        }
        return sp68[var_s1];
    }

    return NULL;
}

// offset: 0x8114 | func: 63
static CurveSetup* dll_211_func_8114(CurveSetup* arg0) {
    if ((arg0->type22.unk30 == -1 || main_get_bits(arg0->type22.unk30) != 0) && (arg0->type22.usedBit == -1 || main_get_bits(arg0->type22.usedBit) == 0)) {
        return arg0;
    }
    return NULL;
}

// offset: 0x81A8 | func: 64
void dll_211_func_81A8(Object* arg0) {
    DLL211_Data* temp_v0;
    Vec3f* temp_v1;

    temp_v0 = arg0->data;
    temp_v1 = temp_v0->unk2C;
    temp_v0->unk5D4 = temp_v1;
    if (temp_v1 != NULL) {
        temp_v0->unk5D8.x = temp_v1->x;
        temp_v0->unk5D8.y = temp_v1->y;
        temp_v0->unk5D8.z = temp_v1->z;
    }
}

// offset: 0x81D8 | func: 65
/* static */ f32 dll_211_func_81D8(Object* arg0) {
    DLL211_Data* data;
    Vec3f* sp28;
    f32 sp24;
    f32 temp_fa1;
    f32 temp_fv1;

    data = arg0->data;
    sp28 = data->unk2C;
    if (sp28 == data->unk5D4) {
        temp_fv1 = data->unk5D8.x - arg0->srt.transl.x;
        temp_fa1 = data->unk5D8.z - arg0->srt.transl.z;
        sp24 = sqrtf(SQ(temp_fv1) + SQ(temp_fa1)) / gUpdateRateF;
        temp_fv1 = sp28->x - arg0->srt.transl.x;
        temp_fa1 = sp28->z - arg0->srt.transl.z;
        temp_fv1 = sqrtf(SQ(temp_fv1) + SQ(temp_fa1)) / gUpdateRateF;
        return temp_fv1 - sp24;
    }

    return 0.0f;
}

// offset: 0x82B8 | func: 66
/* static */ void dll_211_func_82B8(DLL211_Data* arg0) {
    arg0->unk18 = 1;
    arg0->unk1A = 0;
    arg0->unk5F4.x = 0.0f;
    arg0->unk5F4.y = 0.0f;
    arg0->unk5F4.z = 0.0f;
    arg0->unk600 = 600.0f;
    arg0->unk4C &= ~0x10;
    arg0->unk1D = 0;
}

// offset: 0x8308 | func: 67
static void dll_211_func_8308(Object* arg0, s32 arg1) {
    DLL211_Data* data;

    data = arg0->data;
    if (arg1 == arg0->curModAnimId) {
        return;
    }

    if (arg0->curModAnimId > 0 && arg0->curModAnimId < 6 && arg1 > 0 && arg1 < 6) {
        if (data->unk24 > 20.0f) {
            data->unk24 = 0.0f;
            func_80023D30(arg0, arg1, arg0->animProgress, 0U);
        }
    } else {
        data->unk24 = 0.0f;
        func_80023D30(arg0, arg1, arg0->animProgress, 0U);
    }
}

// offset: 0x83BC | func: 68
static void dll_211_func_83BC(Object* arg0, DLL211_Data* arg1) {
    if (dll_211_func_9668(&arg1->unkCC) != 0) {
        dll_211_func_8308(arg0, 8);
        arg0->srt.transl.f[1] = arg1->unkCC.waterY;
        arg1->unk38 = 0.02f;
    } else {
        dll_211_func_8308(arg0, 0);
        arg1->unk38 = 0.005f;
    }
}

// offset: 0x8470 | func: 69
static void dll_211_func_8470(Object* arg0, Vec3f* arg1) {
    s32 temp;
    f32 var_f0;
    f32* temp_a2;
    DLL211_Data* data;
    s16 sp2E;
    DLL27_Data* sp24;

    data = arg0->data;
    sp24 = &data->unkCC;
    var_f0=  data->unk20;
    temp_a2 = data->unk30;
    dll_211_func_8F84(arg0, arg1, data->unk30);
    temp = sp2E - (arg0->srt.yaw & 0xFFFF);
    if (var_f0 > 0.0f) {
        sp2E = arg0->srt.yaw;
        dll_211_func_87E4(arg0);

        // FAKE code that probably got optimized out?
        temp = sp2E - (arg0->srt.yaw & 0xFFFF);
        // FAKE
        if (1)
        CIRCLE_WRAP(temp);
        if (dll_211_func_9668(&data->unkCC) != 0) {
            dll_211_func_8308(arg0, 7);
            arg0->srt.transl.f[1] = sp24->waterY;
        } else if (var_f0 > 1.0f) {
            dll_211_func_8308(arg0, 5);
        } else if (var_f0 > 0.66f) {
            dll_211_func_8308(arg0, 4);
        } else if (var_f0 > 0.33f) {
            dll_211_func_8308(arg0, 2);
        } else {
            dll_211_func_8308(arg0, 1);
        }
        arg0->srt.transl.x += temp_a2[0] * var_f0 * gUpdateRateF;
        arg0->srt.transl.z += temp_a2[1] * var_f0 * gUpdateRateF;
        func_8002493C(arg0, var_f0, &data->unk38);
        return;
    }
    if (dll_211_func_9668(&data->unkCC) != 0) {
        dll_211_func_8308(arg0, 7);
        arg0->srt.transl.f[1] = sp24->waterY;
    } else {
        dll_211_func_8308(arg0, 0);
        data->unk38 = 0.005f;
    }
}

// offset: 0x871C | func: 70
void dll_211_func_871C(Object* arg0, s8* arg1) {
    s16 temp_a2;
    s16 var_a1;
    s16 var_v0;
    s32 var_v1;

    temp_a2 = arg1[0x2C] << 8;
    var_v0 = temp_a2 - (arg0->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(var_v0);
    var_a1 = temp_a2;
    if (var_v0 >= 0) {
        var_v1 = var_v0;
    } else {
        var_v1 = -var_v0;
    }
    if (var_v1 >= 0x4000) {
        var_a1 = temp_a2 + 0x8000;
    }
    dll_211_func_88F4(arg0, var_a1);
}

// offset: 0x87E4 | func: 71
static void dll_211_func_87E4(Object* arg0) {
    f32* data;
    f32 x, z;

    data = arg0->data;
    data += 12;
    z = -data[1];
    x = -data[0];
    dll_211_func_88F4(arg0, arctan2_f(x, z));
}

// offset: 0x885C | func: 72
/* static */ void dll_211_func_885C(Object* arg0) {
    DLL211_Data* data;
    f32 sp24[2];

    data = arg0->data;
    sp24[0] = data->unk2C->x - arg0->srt.transl.x;
    sp24[1] = data->unk2C->z - arg0->srt.transl.z;
    dll_211_func_88F4(arg0, arctan2_f(-sp24[0], -sp24[1]));
}

// offset: 0x88F4 | func: 73
static void dll_211_func_88F4(Object* arg0, s16 arg1) {
    s32 temp_a2;
    s32 var_v1;

    temp_a2 = arg0->srt.yaw;
    var_v1 = arg0->srt.yaw - (arg1 & 0xFFFF);
    // can't use the CIRCLE_WRAP macro here as we need the repeated calc of var_v1
    if ((arg0->srt.yaw - (arg1 & 0xFFFF)) > 0x8000) {
        var_v1 += 0xFFFF0001;
    }
    if (var_v1 < -0x8000) {
        var_v1 += 0xFFFF;
    }
    temp_a2 = var_v1 >> 8;
    if (temp_a2 > 0x10) {
        arg0->srt.yaw -= 0x1000;
        return;
    }
    if (temp_a2 < -0x10) {
        arg0->srt.yaw += 0x1000;
        return;
    }
    arg0->srt.yaw = arg1;
}

// offset: 0x8974 | func: 74
/* static */ void dll_211_func_8974(Object* arg0, UnkCurvesStruct* arg1, f32 arg2) {
    f32 square;
    f32 var_fs0;
    f32 distanceSquared;
    s32 i;
    
    distanceSquared = (arg2 * gUpdateRateF) * 1.5f;
    square = distanceSquared * distanceSquared;
    distanceSquared = vec3_distance_xz_squared((Vec3f *) (&arg1->unk68), &arg0->srt.transl);
    
    if (arg1->unk80 != 0){
        var_fs0 = -2.0f;
    } else {
        var_fs0 = 2.0f;
    }
    
    for (i = 0; i < 5; i++){
        if (square < distanceSquared){
            break;
        }
        func_800053B0(arg1, var_fs0);
        distanceSquared = vec3_distance_xz_squared((Vec3f *) (&arg1->unk68), &arg0->srt.transl);
    }
}

// offset: 0x8A94 | func: 75
static void dll_211_func_8A94(Object* arg0, UnkCurvesStruct* arg1) {
    if (arg1->unk80 != 0) {
        while (arg1->unk10 != 0) {
            func_800053B0(arg1, -2.0f);
        }
    } else{
        while (arg1->unk10 == 0) {
            func_800053B0(arg1, 2.0f);
        }
    }
}

// offset: 0x8B5C | func: 76
void dll_211_func_8B5C(Object* arg0, s32 arg1) {
    DLL211_Data* data;

    data = arg0->data;
    data->unk4C &= ~1;
    data->unk4C |= 0x4000;
    dll_211_func_83BC(arg0, data);
    gDLL_3_Animation->vtbl->start_obj_sequence(arg1, arg0, -1);
}

// offset: 0x8BEC | func: 77
void dll_211_func_8BEC(Object* arg0) {
    DLL211_Data* data;
    f32 sp38;
    Object* sp34;

    data = arg0->data;
    if (map_world_coords_to_block_index(arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z) == -1) {
        data->unkCC.mode = 0;
        if (data->unk28 != NULL) {
            arg0->srt.transl.y = data->unk28->srt.transl.y;
        }
    }
    if (data->unk4C & 0x2000) {
        func_80058680(arg0, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, &sp38, 0U);
        arg0->srt.transl.y -= sp38;
        data->unkCC.mode = 0;
    }
    if (data->unkCC.mode != 0) {
        arg0->velocity.y += -0.17f * gUpdateRateF;
        arg0->srt.transl.y += arg0->velocity.y * gUpdateRateF;
    } else {
        arg0->velocity.y = 0.0f;
    }
    if (arg0->objhitInfo->unk58 & 8) {
        sp34 = arg0->objhitInfo->unk0;
    } else {
        sp34 = NULL;
    }
    if (data->unk4C & 8) {
        data->unk330 += gUpdateRateF;
        if (data->unk330 >= 40.0f) {
            data->unk330 -= 40.0f;
            func_8002674C(arg0);
            data->unk4C &= ~8;
        }
    } else {
        if ((data->unk32C != NULL) && (sp34 == data->unk32C)) {
            data->unk330 += gUpdateRateF;
            if (data->unk330 >= 20.0f) {
                data->unk330 -= 20.0f;
                data->unk4C |= 8;
                func_800267A4(arg0);
            }
        } else {
            data->unk330 = 0.0f;
        }
    }
    data->unk32C = sp34;
    data->unk334 = func_80025F40(arg0, &data->unk338, NULL, NULL);
    gDLL_27->vtbl->func_1E8(arg0, &data->unkCC, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(arg0, &data->unkCC);
    gDLL_27->vtbl->func_624(arg0, &data->unkCC, gUpdateRateF);
    arg0->srt.pitch = data->unkCC.relativeFloorPitchSmooth;
    arg0->srt.roll = data->unkCC.relativeFloorRollSmooth;
}


// offset: 0x8ED0 | func: 78
static void dll_211_func_8ED0(f32 arg0, f32 arg1, f32* arg2) {
    *arg2 = (0.02f * gUpdateRateF) + arg0;
    if (arg1 < *arg2) {
        *arg2 = arg1;
    }
}

// offset: 0x8F18 | func: 79
void dll_211_func_8F18(DLL211_Data* arg0) {
    Object* temp_v0;

    temp_v0 = obj_get_nearest_type_to(0x33, arg0->unk8, NULL);
    arg0->unk28 = temp_v0;
    dll_211_func_9024(arg0, &temp_v0->globalPosition);
    arg0->unk1A = 0;
}


// offset: 0x8F84 | func: 80
// arg2 might be a Vec3f as well
static void dll_211_func_8F84(Object* arg0, Vec3f* arg1, f32* arg2) {
    f32 temp_fv0;

    arg2[0] = arg1->x - arg0->srt.transl.x;
    arg2[1] = arg1->z - arg0->srt.transl.z;
    temp_fv0 = sqrtf(SQ(arg2[0]) + SQ(arg2[1]));
    if (temp_fv0 != 0.0f) {
        arg2[0] /= temp_fv0;
        arg2[1] /= temp_fv0;
    }
}

// offset: 0x9024 | func: 81
static void dll_211_func_9024(DLL211_Data* arg0, Vec3f *arg1) {
    if (arg1 != arg0->unk2C) {
        arg0->unk2C = arg1;
        arg0->unkA6 = 0;
        arg0->unk4C &= ~0x400;
    }
}

// offset: 0x9050 | func: 82
/* static */ void dll_211_func_9050(Object* arg0, DLL211_Data* arg1) {
    FlameBlast_Setup* temp_v0;
    FXEmit_Setup* temp_v0_2;
    s32 i;

    arg1->unk4C |= 0x800;
    arg1->unk0 = dll_load_deferred(DLL_ID_178, 1U);
    for (i = 0; i < 3; i++) {
        temp_v0 = obj_alloc_setup(sizeof(FlameBlast_Setup), OBJ_flameblast);
        temp_v0->base.x = arg0->srt.transl.x;
        temp_v0->base.y = arg0->srt.transl.y;
        temp_v0->base.z = arg0->srt.transl.z;
        temp_v0->base.loadFlags = 2;
        temp_v0->base.byte5 = 1;
        temp_v0->timer = i * 10;
        arg1->unk5E4[i] = obj_create(&temp_v0->base, 5U, arg0->mapID, -1, arg0->parent);
    }
    temp_v0_2 = obj_alloc_setup(sizeof(FXEmit_Setup), OBJ_FXEmit);
    temp_v0_2->base.loadFlags = 2;
    temp_v0_2->base.byte5 = 1;
    temp_v0_2->base.x = arg0->srt.transl.x;
    temp_v0_2->base.y = arg0->srt.transl.y;
    temp_v0_2->base.z = arg0->srt.transl.z;
    temp_v0_2->toggleGamebit = -1;
    temp_v0_2->disableGamebit = -1;
    temp_v0_2->yaw = 0;
    temp_v0_2->flagConfig = 1;
    temp_v0_2->pitch = 0;
    temp_v0_2->roll = 0;
    temp_v0_2->rollSpeed = 0;
    temp_v0_2->pitchSpeed = 0;
    temp_v0_2->yawSpeed = 0;
    temp_v0_2->activationRange = 0;
    temp_v0_2->bank = 1;
    temp_v0_2->indexInBank = 0x4A;
    temp_v0_2->fxRate = -0x1E;
    arg1->unk5F0 = obj_create(&temp_v0_2->base, 5U, arg0->mapID, -1, arg0->parent);
}

// offset: 0x9200 | func: 83
#ifndef NON_MATCHING
void dll_211_func_9200(Object* arg0, DLL211_Data* arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_9200.s")
#else
// https://decomp.me/scratch/zZ2y5
void dll_211_func_9200(Object* arg0, DLL211_Data* arg1) {
    SRT sp40;
    u8 temp_v0;

    if (arg1->unk4C & 0x1800) {
        sp40.transl.x = arg1->unk3B4.x - arg0->srt.transl.f[0];
        sp40.transl.y = arg1->unk3B4.y - arg0->srt.transl.f[1];
        sp40.transl.z = arg1->unk3B4.z - arg0->srt.transl.f[2];
        sp40.scale = 1.0f;
        sp40.yaw = arg0->srt.yaw;
        sp40.pitch = arg0->srt.pitch;
        sp40.roll = arg0->srt.roll;
        if (arg1->unk4C & 0x800) {
            arg1->unk5F0->srt.transl.x = arg1->unk3B4.x;
            arg1->unk5F0->srt.transl.y = arg1->unk3B4.y;
            arg1->unk5F0->srt.transl.z = arg1->unk3B4.z;
            arg1->unk5F0->srt.roll = arg0->srt.roll;
            arg1->unk5F0->srt.pitch = arg0->srt.pitch;
            arg1->unk5F0->srt.yaw = arg0->srt.yaw;
            gDLL_17_partfx->vtbl->spawn(arg0, 0x535, &sp40, 2, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(arg0, 0x532, &sp40, 2, -1, NULL);
            return;
        }
        dll_211_func_95E0(arg0, arg1, _data_0[2]);
        for (temp_v0 = 20; temp_v0 != 0; temp_v0--) { gDLL_17_partfx->vtbl->spawn(arg0, 0x533, &sp40, 2, -1, NULL); }
        arg1->unk4C &= ~0x1000;
    }
}
#endif

// offset: 0x940C | func: 84
/* static */ void dll_211_func_940C(Object* arg0, DLL211_Data* arg1) {
    s32 i;

    arg1->unk4C &= ~0x800;
    arg1->unk4C |= 0x1000;
    dll_unload(arg1->unk0);
    obj_destroy_object(arg1->unk5F0);
    for (i = 0; i < 3; i++) { obj_destroy_object(arg1->unk5E4[i]); }
}

// offset: 0x94BC | func: 85
/* static */ Object* dll_211_func_94BC(Object* arg0, f32 arg1) {
    Object* var_s3;
    Object** temp_v0;
    f32 temp_fv0;
    s32 sp48;
    s32 i;

    var_s3 = NULL;
    temp_v0 = obj_get_all_of_type(OBJTYPE_Baddie, &sp48);
    arg1 = SQ(arg1);
    for (i = 0; i < sp48; i++) {
        if ((obj_is_object_type(temp_v0[i], 0x33) == 0) && (gDLL_33_BaddieControl->vtbl->get_health_ratio(temp_v0[i]) > 0.0f)) {
            temp_fv0 = vec3_distance_squared(&arg0->globalPosition, &temp_v0[i]->globalPosition);
            if (temp_fv0 < arg1) {
                arg1 = temp_fv0;
                var_s3 = temp_v0[i];
            }
        }
    }
    return var_s3;
}

// offset: 0x95E0 | func: 86
static void dll_211_func_95E0(Object* arg0, DLL211_Data *arg1, s16* arg2) {
    if (arg2[2] != 0) {
        func_800349C0(arg0, &arg1->unk360, arg2[0], arg2[1], arg2[2], 1U);
    } else {
        func_80034B54(arg0, &arg1->unk360, arg2, 1U);
    }
}

// offset: 0x9668 | func: 87
static s32 dll_211_func_9668(DLL27_Data* arg0) {
    if (arg0->underwaterDist == 0.0f) {
        return 0;
    }

    if (arg0->floorY == -100000.0f) {
        return 1;
    }

    if ((arg0->underwaterDist - arg0->floorY) > 5.0f) {
        return 1;
    }

    return 0;
}
