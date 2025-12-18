#include "common.h"

// voxmap2.c ?
// unlike the other voxmap.c file, this one is used by kyte and tricky

static const char str_80099db0[] = "****** GOAL FOUND Iterations=%d ******\n";
static const char str_80099dd8[] = " VOXMAPS : Overflow in Route Points for Net Route Finding \n";
static const char str_80099e14[] = " Curve Node %i \n";
static const char str_80099e28[] = " Curve Node %i \n";
static const char str_80099e3c[] = "net_route: No support for this type of node %d x=%f y=%f z=%f\n";
static const char str_80099e7c[] = "**** HEAP INSERT ****\n";
static const char str_80099e94[] = "**** NODE FIND ****\n";

typedef struct {
    u16 unk0;
    u16 unk2;
    CurveSetup *unk4;
} UnkVoxmap2Struct;

// size: 0x10
typedef struct {
    CurveSetup *unk0;
    u32 unk4;
    u32 unk8;
    u8 unkC;
    u8 unkD;
    u8 unkE;
} Voxmap2Struct_0;

// size: 0x8
typedef struct {
    u32 unk0;
    u16 unk4;
    u8 _unk6[0x8 - 0x6];
} Voxmap2Struct_4;

// size: 0x30
typedef struct {
    Voxmap2Struct_0 *unk0; // len: 254
    Voxmap2Struct_4 *unk4; // len: 254
    CurveSetup **unk8; // len: 100
    Vec3f *unkC;
    void *unk10;
    u8 _unk14[0x18 - 0x14];
    CurveSetup *unk18;
    s32 unk1C;
    s16 unk20;
    s16 unk22;
    u32 unk24;
    u8 unk28;
    u8 _unk29[0x2A - 0x29];
    s16 unk2A;
    s16 unk2C;
    u8 _unk2E[0x30 - 0x2E];
} Voxmap2Struct;

extern CurveSetup* D_800B4920;

void func_8003997C(Voxmap2Struct*, Voxmap2Struct_0*, s32);
void func_80039C6C(Voxmap2Struct*, Voxmap2Struct_0*, s32, u32, CurveSetup*);
void func_80039DD0(Voxmap2Struct* arg0);
void func_80039FA4(Voxmap2Struct_4* arg0, s16* arg1, u16 arg2, s32 arg4);
void func_8003A010(Voxmap2Struct_4* arg0, s32 arg1, u16 arg2, u32 arg3);
s32 func_8003A0B8(Voxmap2Struct_4*, s16*);
Voxmap2Struct_0* func_8003A130(Voxmap2Struct* arg0, CurveSetup *arg1, u32 arg2, u16 arg3);
s32 func_8003A240(Voxmap2Struct*, CurveSetup*, s32*);
s32 func_8003A298(Voxmap2Struct*, Voxmap2Struct_0*);

void func_80039590(Voxmap2Struct *arg0) {
    arg0->unk0 = (Voxmap2Struct_0*)mmAlloc(
        (sizeof(Voxmap2Struct_0) * 254) + (sizeof(Voxmap2Struct_4) * 254) + (sizeof(CurveSetup*) * 100), 
        ALLOC_TAG_VOX_COL, 
        NULL);
    arg0->unk4 = (Voxmap2Struct_4*)(arg0->unk0 + 254);
    arg0->unk8 = (CurveSetup**)(arg0->unk4 + 254);
}

void func_800395D8(Voxmap2Struct *arg0) {
    if (arg0->unk0 != NULL) {
        mmFree(arg0->unk0);
        arg0->unk0 = NULL;
    }
}

s32 func_80039610(Voxmap2Struct* arg0, CurveSetup* arg1, Vec3f* arg2, void *arg3, s32 arg5) {
    Voxmap2Struct_0* temp_v0;

    func_80039DD0(arg0);
    arg0->unk18 = arg1;
    arg0->unkC = arg2;
    arg0->unk10 = arg3;
    arg0->unk28 = arg5 & 1;
    arg0->unk24 = 0x2710;
    temp_v0 = func_8003A130(arg0, arg1, 0, 0xFF);
    func_80039FA4(arg0->unk4, &arg0->unk22, (arg0->unk20 - 1), temp_v0->unk4 + temp_v0->unk8);
    return 0;
}

s32 func_800396B0(Voxmap2Struct* arg0, s32 arg1) {
    Voxmap2Struct_0* temp_s1;
    s32 temp_v0;
    s32 var_s3;
    s32 var_s5;

    var_s3 = 0;
    var_s5 = 0;
    while ((var_s3 == 0) && (arg1 != 0)) {
        temp_v0 = func_8003A0B8(arg0->unk4, &arg0->unk22);
        if (temp_v0 >= 0) {
            arg0->unk1C = temp_v0;
            temp_s1 = &arg0->unk0[temp_v0];
            if (func_8003A298(arg0, temp_s1) != 0) {
                var_s3 = 1;
                var_s5 = 1;
            } else {
                temp_s1->unkE = 1;
                func_8003997C(arg0, temp_s1, temp_v0);
            }
        } else {
            var_s3 = 1;
            var_s5 = -1;
        }
        arg1 -= 1;
    }
    return var_s5;
}

s32 func_80039798(Voxmap2Struct* arg0) {
    CurveSetup** temp_t6;
    Voxmap2Struct_0* var_v1;
    Voxmap2Struct_0* var_v1_2;
    s32 var_v0;
    s32 var_a1;
    s32 var_a2;

    var_v0 = arg0->unk1C;
    var_v1 = &arg0->unk0[var_v0];
    var_v1->unkD = 0xFF;
    while (var_v1->unkC != 0xFF) {
        var_a2 = var_v1->unkC;
        var_v1 = &arg0->unk0[var_v1->unkC];
        var_v1->unkD = var_v0;
        var_v0 = var_a2;
        var_a2 = var_v1->unkC;
    }
    if (var_v1->unkD == 0xFF) {
        var_v1_2 = NULL;
    } else {
        var_v1_2 = &arg0->unk0[var_v1->unkD];
    }
    var_a1 = 0;
    while (var_v1_2 != NULL) {
        arg0->unk8[var_a1] = var_v1_2->unk0;
        var_a1 += 1;
        if (var_a1 >= 100) {
            var_v1_2 = NULL;
        } else if (var_v1_2->unkD == 0xFF) {
            var_v1_2 = NULL;
        } else {
            var_v1_2 = &arg0->unk0[var_v1_2->unkD];
        }
    }
    arg0->unk2A = var_a1;
    arg0->unk2C = 0;
    return var_a1;
}

CurveSetup* func_80039870(Voxmap2Struct* arg0) {
    if (arg0->unk2C < arg0->unk2A) {
        return arg0->unk8[arg0->unk2C++];
    }
    return NULL;
}

s32 func_800398AC(CurveSetup* arg0, UnkVoxmap2Struct *arg1) {
    if ((arg0->type22.unk30 == -1 || main_get_bits(arg0->type22.unk30) != 0) && 
        (arg0->type22.usedBit == -1 || main_get_bits(arg0->type22.usedBit) == 0)) {
        if (arg0->base_type22.unk4 == arg1->unk0) {
            return 1;
        }
        if (arg0->unk1A < 3) {
            return 1;
        }
        if (arg1->unk4 != 0) {
            if (arg0 == arg1->unk4) {
                return 1;
            }
        } else {
            if (arg1->unk2 == arg0->base_type22.unk4) {
                return 1;
            }
        }
    }
    return 0;
}

void func_8003997C(Voxmap2Struct* arg0, Voxmap2Struct_0* arg1, s32 arg2) {
    CurveSetup* temp_s5;
    CurveSetup* temp_v0;
    s32 temp_a0;
    s32 var_s1;
    u8 var_fp;

    temp_s5 = arg1->unk0;
    if (arg0->unk28 != 0) {
        var_fp = temp_s5->unk1B;
    } else {
        var_fp = ~temp_s5->unk1B;
    }
    
    for (var_s1 = 0; var_s1 < 4; var_s1++) {
        temp_a0 = temp_s5->unk1C[var_s1];
        if ((temp_a0 >= 0) && (var_fp & (1 << var_s1))) {
            temp_v0 = gDLL_26_Curves->vtbl->func_39C(temp_a0);
            if (temp_v0 != NULL) {
                switch (temp_v0->unk19) {
                case 0x22: // Kyte
                    if (func_800398AC(temp_v0, (UnkVoxmap2Struct*)arg0->unk10) != 0) {
                        func_80039C6C(arg0, arg1, arg2, 
                            (u32) (vec3_distance_squared(&temp_s5->pos, &temp_v0->pos) + (f32) arg1->unk8), 
                            temp_v0);
                    }
                    break;
                case 0x24: // Tricky
                    main_get_bits(BIT_4E2);
                    if ((temp_v0->type22.unk30 == -1 || main_get_bits(temp_v0->type22.unk30) != 0) &&
                           (temp_v0->type22.usedBit == -1 || main_get_bits(temp_v0->type22.usedBit) == 0)) {
                        func_80039C6C(arg0, arg1, arg2, 
                            (u32) (vec3_distance_squared(&temp_s5->pos, &temp_v0->pos) + (f32) arg1->unk8), 
                            temp_v0);
                    }
                    break;
                default:
                    D_800B4920 = temp_v0;
                }
            }
        }
    }
}

void func_80039C6C(Voxmap2Struct* arg0, Voxmap2Struct_0* arg1, s32 arg2, u32 arg3, CurveSetup* arg4) {
    s32 sp3C;
    s32 temp_v0;
    Voxmap2Struct_0* temp_v0_2;
    Voxmap2Struct_0* temp_v1;

    if (func_8003A298(arg0, arg1) != 0) {
        temp_v0 = arg0->unk20;
        func_8003A130(arg0, arg4, arg3, (u16) arg2);
        func_80039FA4(arg0->unk4, &arg0->unk22, temp_v0, 1);
    }
    temp_v0 = func_8003A240(arg0, arg4, &sp3C);
    if ((temp_v0 >= 0) && (sp3C == 0)) {
        temp_v1 = &arg0->unk0[temp_v0];
        if (arg3 < temp_v1->unk8) {
            temp_v1->unkC = (u8) arg2;
            temp_v1->unk8 = arg3;
            func_8003A010(arg0->unk4, arg0->unk22, temp_v0, temp_v1->unk4 + arg3);
        }
    } else if (temp_v0 < 0) {
        temp_v0 = arg0->unk20;
        temp_v1 = func_8003A130(arg0, arg4, arg3, (u16) arg2);
        if (temp_v1 != NULL) {
            if (arg0->unk24 < temp_v1->unk4) {
                func_80039FA4(arg0->unk4, &arg0->unk22, temp_v0, temp_v1->unk4 + temp_v1->unk8);
            } else {
                if (temp_v1->unk4 < arg0->unk24) {
                    arg0->unk24 = temp_v1->unk4;
                }
                func_80039FA4(arg0->unk4, &arg0->unk22, temp_v0, temp_v1->unk4 + temp_v1->unk8);
            }
        }
    }
}

void func_80039DD0(Voxmap2Struct* arg0) {
    s32 i;

    arg0->unk22 = 0;
    arg0->unk20 = 0;
    for (i = 0; i < 254; i++) {
        arg0->unk4[i].unk0 = 0;
        arg0->unk0[i].unkE = 0;
    }
}

void func_80039E78(Voxmap2Struct_4* arg0, s32 arg1) {
    u32 temp_v0;
    s32 var_a3;
    s32 temp_a2;

    temp_v0 = arg0[arg1].unk0;
    temp_a2 = arg0[arg1].unk4;
    arg0[0].unk0 = -1;
    var_a3 = arg1 >> 1;
    while (arg0[var_a3].unk0 < temp_v0) {
        arg0[arg1].unk4 = arg0[var_a3].unk4;
        arg0[arg1].unk0 = arg0[var_a3].unk0;
        arg1 = var_a3;
        var_a3 = var_a3 >> 1;
    }
    arg0[arg1].unk0 = temp_v0;
    arg0[arg1].unk4 = temp_a2;
}

void func_80039F08(Voxmap2Struct_4* arg0, s32 arg1, s32 arg2) {
    s32 var_t1;
    s32 temp_a3;
    u32 temp_v0;

    temp_v0 = arg0[arg2].unk0;
    temp_a3 = arg0[arg2].unk4;
    while ((arg1 >> 1) >= arg2) {
        var_t1 = arg2 + arg2;
        if (var_t1 < arg1) {
            if (arg0[var_t1].unk0 < arg0[var_t1 + 1].unk0) {
                var_t1 = var_t1 + 1;
            }
        }

        if (temp_v0 >= arg0[var_t1].unk0) {
            break;
        }

        arg0[arg2].unk0 = arg0[var_t1].unk0;
        arg0[arg2].unk4 = arg0[var_t1].unk4;
        arg2 = var_t1;
    }
    arg0[arg2].unk0 = temp_v0;
    arg0[arg2].unk4 = temp_a3;
}

void func_80039FA4(Voxmap2Struct_4* arg0, s16* arg1, u16 arg2, s32 arg4) {
    *arg1 += 1;
    arg0[*arg1].unk4 = arg2;
    arg0[*arg1].unk0 = -1 - arg4;
    func_80039E78(arg0, *arg1);
}

void func_8003A010(Voxmap2Struct_4* arg0, s32 arg1, u16 arg2, u32 arg3) {
    s32 sp1C;
    s32 var_v0;
    u32 temp_v1;

    for (var_v0 = 0; arg1 >= var_v0; var_v0++) {
        if (arg2 == arg0[var_v0].unk4) {
            sp1C = var_v0;
            var_v0 = arg1 + 1;
        }
    }
    temp_v1 = arg0[sp1C].unk0;
    arg0[sp1C].unk0 = arg3;
    if (arg3 < temp_v1) {
        func_80039F08(arg0, arg1, sp1C);
    } else if (temp_v1 < arg3) {
        func_80039E78(arg0, sp1C);
    }
}

s32 func_8003A0B8(Voxmap2Struct_4* arg0, s16* arg1) {
    u16 sp1E;
    
    if (*arg1 == 0) {
        return -1;
    }

    sp1E = arg0[1].unk4;
    arg0[1].unk0 = arg0[*arg1].unk0;
    arg0[1].unk4 = arg0[*arg1].unk4;
    *arg1 -= 1;
    func_80039F08(arg0, *arg1, 1);
    return (s32) sp1E;
}

Voxmap2Struct_0* func_8003A130(Voxmap2Struct* arg0, CurveSetup *arg1, u32 arg2, u16 arg3) {
    Voxmap2Struct_0* temp_v1;

    if (arg0->unk20 == 254) {
        return NULL;
    }
    temp_v1 = &arg0->unk0[arg0->unk20++];
    temp_v1->unk0 = arg1;
    temp_v1->unk8 = arg2;
    temp_v1->unkC = arg3;
    temp_v1->unk4 = (u32) vec3_distance_squared(&arg1->pos, arg0->unkC);
    return temp_v1;
}

s32 func_8003A240(Voxmap2Struct* arg0, CurveSetup* arg1, s32* arg2) {
    Voxmap2Struct_0* temp_a0;
    s32 i;

    for (i = 0; i < arg0->unk20; i++) {
        temp_a0 = &arg0->unk0[i];
        if (arg1 == temp_a0->unk0) {
            *arg2 = temp_a0->unkE;
            return i;
        }
    }
    return -1;
}

s32 func_8003A298(Voxmap2Struct* arg0, Voxmap2Struct_0* arg1) {
    UnkVoxmap2Struct *new_var;
    CurveSetup* temp_a1;
    void *temp_v0;
    s32 temp3;

    temp_v0 = arg0->unk10;
    switch (arg1->unk0->unk19) {
    case 0x22: // Kyte
        new_var = (UnkVoxmap2Struct*)arg0->unk10;
        if (new_var->unk4 != NULL) {
            return new_var->unk4 == arg1->unk0;
        }
        return new_var->unk2 == arg1->unk0->base_type22.unk4;
    case 0x24: // Tricky
        if (!(arg1->unkC & 0x80)) {
            temp3 = arg1->unk0->unk3;
            if (temp3 != 0) {
                return (s32)temp_v0 == arg1->unk0->unk3;
            }

            if (arg0->unk0[arg1->unkC].unk0) {}

            temp_a1 = arg0->unk0[arg1->unkC].unk0;
            if (arg1->unk0->uID == temp_a1->unk1C[0]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk4;
            }
            if (arg1->unk0->uID == temp_a1->unk1C[1]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk5;
            }
            if (arg1->unk0->uID == temp_a1->unk1C[2]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk6;
            }
            if (arg1->unk0->uID == temp_a1->unk1C[3]) {
                return (s32)temp_v0 == temp_a1->base_type24.unk7;
            }
        }
        return 0;
    default:
      return ((CurveSetup*)temp_v0) == (arg1->unk0);
    }
}
