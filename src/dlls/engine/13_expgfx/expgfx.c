#include "common.h"
#include "prevent_bss_reordering.h"

typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
} UnkBss0Struct_Unk0;

typedef struct {
    UnkBss0Struct_Unk0 unk0[4];
    SRT unk40;
    Vec3f unk58;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    u32 unk7C;
    u32 unk80;
    u16 unk84;
    s16 unk86;
    u16 unk88;
    u16 unk8A;
    u8 unk8C;
    u8 unk8D;
    u8 unk8E;
} UnkBss0Struct;

typedef struct {
    Object *unk0;
    Object *unk4;
    Texture *unk8;
    u16 unkC;
} UnkBss190Struct;

typedef struct {
    Texture *unk0;
    s32 unk4; // lifetime?
    s32 unk8; // texture ID
    s32 unkC;
} UnkBss370Struct;

/*0x0*/ static s16 _data_0[] = {
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
/*0x3C*/ static u32 _data_3C[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x5C*/ static u32 _data_5C = 0x00000000;
/*0x60*/ static u32 _data_60 = 0x00000000;
/*0x64*/ static u32 _data_64 = 0x00000000;
/*0x68*/ static u32 _data_68[] = {
    0x00000000, 0x00000000
};
/*0x70*/ static u32 _data_70[] = {
    0x40000102, 0x00000000, 0x00000000, 0x00000000, 0x40000203, 0x00000000, 0x00000000, 0x00000000
};
/*0x90*/ static u32 _data_90 = 0x00000000;
/*0x94*/ static u32 _data_94 = 0x00000000;
/*0x98*/ static u32 _data_98 = 0x00000000;
/*0x9C*/ static u32 _data_9C[] = {
    0xfc180000, 0x03e803e8, 0x000003e8, 0x03e80000, 0xfc18fc18, 0x0000fc18
};
/*0xB4*/ static u32 _data_B4[] = {
    0xfc18fc18, 0x000003e8, 0xfc180000, 0x03e803e8, 0x0000fc18, 0x03e80000, 0xfc18fc18, 0x000003e8, 
    0xfc180000, 0x03e803e8, 0x0000fc18, 0x03e80000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static UnkBss0Struct *_bss_0[30];
/*0x78*/ static u32 _bss_78[30];
/*0xF0*/ static s8 _bss_F0[30];
/*0x110*/ static u8 _bss_110[0x8];
/*0x118*/ static Object *_bss_118[30];
/*0x190*/ static UnkBss190Struct _bss_190[30];
/*0x370*/ static UnkBss370Struct _bss_370[8];

// offset: 0x0 | ctor
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_ctor.s")

// offset: 0xE0 | dtor
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_dtor.s")

// offset: 0x158 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_158.s")

// offset: 0x228 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_228.s")

// offset: 0xC18 | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_C18.s")

// offset: 0xCEC | func: 3 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_CEC.s")

// offset: 0xED4 | func: 4 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_ED4.s")

// offset: 0x1048 | func: 5 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1048.s")

// offset: 0x1080 | func: 6 | export: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1080.s")

// offset: 0x1EEC | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1EEC.s")

// offset: 0x1EF4 | func: 8 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1EF4.s")

// offset: 0x1F0C | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1F0C.s")

// offset: 0x1F44 | func: 10 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_1F44.s")

// offset: 0x2060 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_2060.s")

// offset: 0x2254 | func: 12
static void dll_13_func_2254(u8 arg0) {
    if (_bss_190[arg0].unkC != 0) {
        _bss_190[arg0].unkC -= 1;
    }
}

// offset: 0x2298 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_2298.s")

// offset: 0x4684 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/13_expgfx/dll_13_func_4684.s")

// offset: 0x4A18 | func: 15
s32 dll_13_func_4A18(s16* arg0, s16* arg1, s16 arg2, s32 arg3, Object *arg4) {
    s32 var_t0;
    s32 var_t1_2;
    s16 var_v1;
    s32 var_t1;

    var_v1 = -1;
    var_t1 = 0;
    var_t0 = 0;
    for (var_t0 = 0; var_t0 < 30; var_t0++) {
        if ((arg4 == _bss_118[var_t0]) && (arg2 == _data_0[var_t0]) && (_bss_F0[var_t0] < 30)) {
            var_v1 = var_t0;
            var_t1 = 1;
            break;
        }
    }
    
    if (var_t1 != 0) {
        for (var_t1_2 = 0; var_t1_2 < 30; var_t1_2++) {
            if (!(_bss_78[var_v1] & (1 << var_t1_2))) {
                *arg1 = var_t1_2;
                *arg0 = var_v1;
                _bss_78[var_v1] |= (1 << var_t1_2);
                _bss_F0[var_v1]++;
                return 1;
            }
        }
    }
    
    var_t1 = 0;
    if (arg3 == -1) {
        for (var_t0 = 0; var_t0 < 29; var_t0++) {
            if (_bss_F0[var_t0] <= 0) {
                var_v1 = var_t0;
                var_t1 = 1;
                _bss_F0[var_t0] = 0;
                break;
            }
        }
    } else if (arg3 != -1) {
        var_t0 = arg3;
        if (_bss_F0[arg3] < 30) {
            var_v1 = arg3;
            var_t1 = 1;
        }
    }
    
    if (var_t1 != 0) {
        for (var_t1_2 = 0; var_t1_2 < 30; var_t1_2++) {
            if (arg2){} // fake

            if (!(_bss_78[var_v1] & (1 << var_t1_2))) {
                *arg1 = var_t1_2;
                *arg0 = var_v1;
                _bss_78[var_v1] |= (1 << var_t1_2);
                _data_0[var_t0] = arg2;
                _bss_F0[var_v1]++;
                return 1;
            }
        }

        return -1;
    }
    
    return -1;
}

// offset: 0x4DCC | func: 16
void dll_13_func_4DCC(void) {
    UnkBss0Struct* var_s0;
    s32 i;
    s32 k;

    for (i = 0; i < 30; i++) {
        var_s0 = _bss_0[i];
        for (k = 0; k < 30; k++) {
            if (_bss_78[i] & (1 << k)) {
                if (_bss_190[(u32)var_s0->unk8A >> 9].unk8 != NULL) {
                    _bss_190[(u32)var_s0->unk8A >> 9].unk8->unk5 -= 1;
                }
                dll_13_func_2254((u32)var_s0->unk8A >> 9);
                var_s0->unk0[2].unk6 = -1;
                _bss_78[i] &= ~(1 << k);
            }
            var_s0 += 1;
        }
        _bss_F0[i] = 0;
        _data_0[i] = -1;
    }
}

// offset: 0x4F2C | func: 17
void dll_13_func_4F2C(s32 arg0, s32 arg1, s32 arg2) {
    UnkBss0Struct* temp_v1;

    if (_bss_78[arg0] & (1 << arg1)) {
        temp_v1 = _bss_0[arg0];
        temp_v1 += arg1;
        temp_v1->unk7C = 0;
        if (arg2 == 0) {
            if (_bss_190[(u32)temp_v1->unk8A >> 9].unk8 != NULL) {
                _bss_190[(u32)temp_v1->unk8A >> 9].unk8->unk5 -= 1;
                dll_13_func_2254((u32)temp_v1->unk8A >> 9);
            }
        }
        temp_v1->unk0[2].unk6 = -1;
        _bss_78[arg0] &= ~(1 << arg1);
        _bss_F0[arg0] -= 1;
        if (_bss_F0[arg0] == 0) {
            _data_0[arg0] = -1;
        }
    }
}

// offset: 0x5068 | func: 18
s16 dll_13_func_5068(s32 textureID) {
    s32 i;
    s32 var_a3;
    s32 var_a0;
    
    i = 0;
    while (TRUE) {
        if (textureID == _bss_370[i].unk8) {
            // Existing found
            if ((_bss_370[i].unk0 != NULL) && (_bss_370[i].unk0->unk5 >= 0xFE)) {
                return -1;
            }
            _bss_370[i].unk4 = 1000;
            return i;
        }
        i += 1;
        if (i >= 8) {
            i = 0;
            var_a3 = 0;
            while (TRUE) {
                if (_bss_370[i].unk0 == NULL) {
                    // Empty slot found
                    _bss_370[i].unk0 = queue_load_texture_proxy(textureID);
                    if ((_bss_370[i].unk0 != NULL) && (_bss_370[i].unk0->unk5 >= 0xFE)) {
                        if (_bss_370[i].unk0 != NULL) {
                            texture_destroy(_bss_370[i].unk0);
                        }
                        _bss_370[i].unk0 = NULL;
                        return -1;
                    }
                    if (_bss_370[i].unk0 != NULL) {
                        _bss_370[i].unk4 = 1000;
                        _bss_370[i].unk8 = textureID;
                        _bss_370[i].unkC = (s32) (_bss_370[i].unk0->levels * _bss_370[i].unk0->unk18) >> 3;
                        return (s16) i;
                    }
                    return -2;
                }
                i += 1;
                if (i >= 8) {
                    // No room, replace the oldest
                    var_a0 = 64000;
                    for (i = 0; i < 8; i++) {
                        if (_bss_370[i].unk4 < var_a0) {
                            var_a0 = _bss_370[i].unk4;
                            var_a3 = i;
                        }
                    }
                    if (_bss_370[var_a3].unk0 != NULL) {
                        texture_destroy(_bss_370[var_a3].unk0);
                    }
                    _bss_370[var_a3].unk0 = NULL;
                    _bss_370[var_a3].unk0 = queue_load_texture_proxy(textureID);
                    if (_bss_370[var_a3].unk0 != NULL) {
                        _bss_370[var_a3].unk4 = 1000;
                        _bss_370[var_a3].unk8 = textureID;
                        _bss_370[var_a3].unkC = (s32) (_bss_370[var_a3].unk0->levels * _bss_370[var_a3].unk0->unk18) >> 3;
                        return (s16) var_a3;
                    }
                    return -3;
                }
            }
        }
    }
}

// offset: 0x52B4 | func: 19
void dll_13_func_52B4(s32 arg0) {
    UnkBss370Struct* var_s0;
    s32 i;

    for (i = 0; i < 8; i++) {
        var_s0 = &_bss_370[i];
        if (var_s0->unk8 != 0) {
            var_s0->unk4 -= gUpdateRate;
            if (var_s0->unk4 <= 0) {
                var_s0->unk8 = 0;
                var_s0->unk4 = 0;
                var_s0->unkC = 0;
                texture_destroy(var_s0->unk0);
                var_s0->unk0 = NULL;
            }
        }
    }
}

// offset: 0x5358 | func: 20
void dll_13_func_5358(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3) {
    Vec3f spC;
    Vec3f sp0;

    spC.x = arg2->x - arg0->x;
    spC.y = arg2->y - arg0->y;
    spC.z = arg2->z - arg0->z;
    sp0.x = arg1->x - arg0->x;
    sp0.y = arg1->y - arg0->y;
    sp0.z = arg1->z - arg0->z;
    arg3->x = (sp0.y * spC.z) - (sp0.z * spC.y);
    arg3->y = -((sp0.x * spC.z) - (sp0.z * spC.x));
    arg3->z = (sp0.x * spC.y) - (sp0.y * spC.x);
}

/*0x0*/ static const char str_0[] = "WARN no free particle\n";
/*0x18*/ static const char str_18[] = "error no texture\n";
/*0x2C*/ static const char str_2C[] = "expgfx.c: invalid tabindex\n";
/*0x48*/ static const char str_48[] = "expgfx.c: scale overflow\n";
/*0x64*/ static const char str_64[] = "expgfx.c: Error on owner free\n";
/*0x84*/ static const char str_84[] = "\t exp x %f\n";
/*0x90*/ static const char str_90[] = "\t exp z %f\n";
/*0x9C*/ static const char str_9C[] = "expgfx.c: addToTable usage overflow\n";
/*0xC4*/ static const char str_C4[] = "expgfx.c: exptab is FULL\n";
/*0xE0*/ static const char str_E0[] = "expgfx.c: mismatch in add/remove in exptab\n";
/*0x10C*/ static const char str_10C[] = "notexture \n";
/*0x118*/ static const char str_118[] = "notexture \n";
