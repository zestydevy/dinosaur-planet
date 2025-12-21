#include "common.h"
#include "sys/voxmap.h"
#include "sys/map.h"

static const char str_80098510[] = "VOXMAP: Size overflow on load->Ignoring defined voxmap\n";
static const char str_80098548[] = "VOXMAP: attempt to load invalid object voxmap '%d'\n";
static const char str_8009857c[] = "VOXMAP: attempt to load invalid object voxmap '%d'\n";
static const char str_800985b0[] = "VOXMAP: Size overflow on object voxmap load->Ignoring defined voxmap\n";
static const char str_800985f8[] = "overflow in 'genVoxelCircle'\n";
static const char str_80098618[] = "****** GOAL FOUND Iterations=%d ******\n";
static const char str_80098640[] = "route: routepoints overflow\n";
static const char str_80098660[] = "Childnode Null\n";
static const char str_80098670[] = "**** HEAP INSERT ****\n";
static const char str_80098688[] = "VOXMAPS: route nodes list overflow\n";
static const char str_800986ac[] = "**** NODE FIND ****\n";

typedef struct {
    s32 pad0;
    s32 unk4;
    s32 pad8;
    s32 unkC;
    s32 pad10;
    s32 *unk14;
    s32 pad18;
    u8 *unk1C;
    s32 pad20;
    u8 *unk24;
} Unk800A7CA0Inner;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    Unk800A7CA0Inner *unk10;
} Unk800A7CA0;

Unk800A7CA0* func_80007A58(s16 *arg0);

s32* D_800A7C70;
s32* D_800A7C74;
void* D_800A7C78;
Unk800A7CA0* D_800A7C80[6];
s32* D_800A7C98;
// missing 0x800A7C9C
Unk800A7CA0 D_800A7CA0;
s32 D_800A7CB8[6];
s32 D_800A7CD0[6];
u8 D_800A7CE8[6];
s16 D_800A7CF0[6][2];
Object *D_800A7D08;
s32 D_800A7D0C;

#define SOME_VALUE 0x40000000
#define SOME_FACTOR 10

void func_80006F50(void) {
    s32 i;

    queue_alloc_load_file(&D_800A7C70, TEXPRE_TAB);
    queue_alloc_load_file(&D_800A7C74, VOXOBJ_TAB);
    for (i = 0; D_800A7C74[i] != -1; i++) {}

    D_800A7D0C = --i;
    D_800A7C78 = mmAlloc(0x280, 0x10, NULL);
    i = 0;
    while (i < 6) {
        D_800A7C80[i] = NULL;
        D_800A7CB8[i] = -2;
        D_800A7CD0[i] = SOME_VALUE;
        D_800A7CE8[i] = 0;
        D_800A7CF0[i][0] = 0;
        D_800A7CF0[i][1] = 0;
        i++;
    }
    D_800A7C98 = D_800A7C78;
    D_800A7D08 = NULL;
}

void func_800070FC(void) {
    Unk800A7CA0 *temp_a0;
    s32 i;

    for (i = 0; i < 6; i++) {
        temp_a0 = D_800A7C80[i];
        if (temp_a0 != NULL) {
            mmFree(temp_a0);
            D_800A7C80[i] = NULL;
        }
    }
    mmFree(D_800A7C70);
    mmFree(D_800A7C74);
    mmFree(D_800A7C78);
}

void func_80007178(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (SOME_VALUE-1 > D_800A7CD0[i]) {
            D_800A7CD0[i]++;
        }
    }
}

Unk800A7CA0 *func_80007224(void) {
    s32 var_v0;
    s32 var_v1;
    s32 i;

    var_v0 = -1;
    var_v1 = -1;
    for (i = 0; i < 6; i++) {
        if (D_800A7CE8[i] == 0 && var_v0 < D_800A7CD0[i]) {
            var_v1 = i;
            var_v0 = D_800A7CD0[i];
        }
    }

    if (var_v1 != -1) {
        D_800A7CD0[var_v1] = SOME_VALUE;
        D_800A7CB8[var_v1] = -1;
        D_800A7CE8[var_v1] = 1;
        return D_800A7C80[var_v1];
    }

    return NULL;
}

void func_80007380(Unk800A7CA0 *arg0) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (arg0 == D_800A7C80[i]) {
            D_800A7CE8[i] = 0;
            i = 6;
        }
    }
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_800073CC.s")
#else
// https://decomp.me/scratch/RcRgq
s32 func_800073CC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Unk800A7CA0 *temp_a0;

    if (D_800A7C70[arg0 + 1] == D_800A7C70[arg0]) {
        return 0;
    }
    temp_a0 = D_800A7C80[arg1];
    if (temp_a0 != 0) {
        mmFree((void* ) temp_a0);
    }
    D_800A7C80[arg1] = func_80007468(arg0, arg1, arg2, arg3);
    return 1;
}
#endif

u8 *func_80007468(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 temp_v1;
    u8* sp2C;
    u8* temp_v0;

    sp34 = D_800A7C70[arg0 + 0];
    sp3C = D_800A7C70[arg0 + 1] - sp34;
    if (sp3C == 0) {
        return 0;
    }
    sp38 = rarezip_uncompress_size_rom(TEXPRE_BIN, D_800A7C70[arg0 + 0], 1);
    if (sp38 > 0x5000) {
        return 0;
    }
    temp_v0 = mmAlloc(sp38 + 0x80, 0x10, NULL);
    temp_v1 = ((temp_v0 + sp38) - sp3C) + 0x80;
    sp2C = (u8*)(temp_v1 - (temp_v1 % 16));
    read_file_region(TEXPRE_BIN, sp2C, sp34, sp3C);
    rarezip_uncompress(sp2C, temp_v0, sp38 + 0x80);
    temp_v0 = mmRealloc(temp_v0, sp38, NULL);
    for (temp_v1 = 0; temp_v1 < 2; ) {
        ((s32*)temp_v0)[temp_v1 + 7] = ((s32*)temp_v0)[temp_v1 + 7] + temp_v0;
        ((s32*)temp_v0)[temp_v1 + 9] = ((s32*)temp_v0)[temp_v1 + 9] + temp_v0;
        ((s32*)temp_v0)[temp_v1 + 5] = ((s32*)temp_v0)[temp_v1 + 5] + temp_v0;
        temp_v1++;
    }
    return temp_v0;
}

s32 func_800075B0(s32 arg0, s32 arg1) {
    if ((arg0 < 0) || (arg0 >= D_800A7D0C)) {
        return 0;
    }
    func_80012584(0x3C, 6U, (u32* ) &D_800A7C80[arg1], (ObjSetup* ) arg0, 0, 0, NULL, 0);
    return 1;
}

u8 *func_80007620(s32 arg0, s32 arg1) {
    s32 sp34;
    u8* temp_v0;
    s32 sp2C;
    s32 temp_v0_3;
    u8* sp24;
    s32 temp_v1;

    if ((arg0 < 0) || (arg0 >= D_800A7D0C)) {
        return 0;
    }
    sp2C = D_800A7C74[arg0];
    sp34 = D_800A7C74[arg0 + 1] - sp2C;
    if (sp34 == 0) {
        return 0;
    }
    temp_v0_3 = rarezip_uncompress_size_rom(VOXOBJ_BIN, D_800A7C74[arg0], 1);
    if (temp_v0_3 > 0x5000) {
        return 0;
    }
    temp_v0 = mmAlloc(temp_v0_3 + 0x80, 0x10, NULL);
    temp_v1 = ((temp_v0 + temp_v0_3) - sp34) + 0x80;
    sp24 = (u8*)(temp_v1 - (temp_v1 % 16));
    read_file_region(VOXOBJ_BIN, sp24, (u32) sp2C, sp34);
    rarezip_uncompress(sp24, temp_v0, temp_v0_3);
    temp_v0 = mmRealloc(temp_v0, temp_v0_3, NULL);
    for (temp_v1 = 0; temp_v1 < 2; ) {
        ((s32*)temp_v0)[temp_v1 + 7] = ((s32*)temp_v0)[temp_v1 + 7] + temp_v0;
        ((s32*)temp_v0)[temp_v1 + 9] = ((s32*)temp_v0)[temp_v1 + 9] + temp_v0;
        ((s32*)temp_v0)[temp_v1 + 5] = ((s32*)temp_v0)[temp_v1 + 5] + temp_v0;
        temp_v1++;
    }
    return temp_v0;
}

Unk800A7CA0* func_80007778(s16 *arg0) {
    s32 var_s1;
    s32 sp40;
    s32 pad2;
    s32 pad;
    s32 var_a1;
    s32 var_s0;
    s32 var_v0;
    s32 i;
    GlobalMapCell* mapCell;
    s8 arg3;

    if (D_800A7D08 != 0) {
        return func_80007A58(arg0);
    }
    var_v0 = ((arg0[0] * SOME_FACTOR) - D_80092A60) + 5;
    var_s1 = ((arg0[2] * SOME_FACTOR) - D_80092A64) + 5;
    var_s0 = floor_f(var_v0 / BLOCKS_GRID_UNIT_F);
    sp40 = floor_f(var_s1 / BLOCKS_GRID_UNIT_F);
    D_800A7CA0.unk0 = (var_s0 * BLOCKS_GRID_UNIT) + D_80092A60;
    D_800A7CA0.unk4 = (sp40 * BLOCKS_GRID_UNIT) + D_80092A64;
    D_800A7CA0.unk8 = D_800A7CA0.unk0 / SOME_FACTOR;
    D_800A7CA0.unkC = D_800A7CA0.unk4 / SOME_FACTOR;
    var_s1 = -1;
    if (func_80044B18(var_s0, sp40, 0) != 0) {
        mapCell = func_80046698(var_s0, sp40);
        var_s1 = mapCell->blockID;
    }
    if (var_s1 != -1) {
        i = 0;
        var_s0 = -1;
        for (; i < 6; i++) {
            if (var_s1 == D_800A7CB8[i]) {
                var_s0 = i;
                i = 6;
            }
        }
        if (var_s0 != -1) {
            D_800A7CD0[var_s0] = 0;
            D_800A7CA0.unk10 = D_800A7C80[var_s0];
        } else {
            var_s0 = -1;
            var_a1 = -1;
            i = 0;
            for (; i < 6; i++) {
                if (D_800A7CE8[i] == 0) {
                    if (var_a1 <  D_800A7CD0[i]) {
                        var_s0 = i;
                        var_a1 =  D_800A7CD0[i];
                    }
                }
            }
            if (func_800073CC(var_s1, var_s0, mapCell->trkBlkIndex, mapCell->loadedBlockIndex) != 0) {
                D_800A7CB8[var_s0] = var_s1;
                D_800A7CD0[var_s0] = 0;
                D_800A7CF0[var_s0][0] = D_800A7CA0.unk8;
                D_800A7CF0[var_s0][1] = D_800A7CA0.unkC;
            }
            D_800A7CA0.unk10 = 0;
        }
    } else {
        D_800A7CA0.unk10 = 0;
    }
    return &D_800A7CA0;
}

Unk800A7CA0* func_80007A58(s16 *arg0) {
    ObjDef* temp_v1;
    s32 temp_t5;
    s32 temp_t8;
    s32 var_a1;
    s32 i;
    s32 temp_s0;
    s32 sp24;

    temp_v1 = D_800A7D08->def;
    temp_t8 = (arg0[0] - temp_v1->unk80) / 64;
    temp_t5 = (arg0[2] - temp_v1->unk82) / 64;
    D_800A7CA0.unk0 = (temp_t8 * 0x280) + (temp_v1->unk80 * SOME_FACTOR);
    D_800A7CA0.unk4 = (temp_t5 * 0x280) + (temp_v1->unk82 * SOME_FACTOR);
    D_800A7CA0.unk8 = D_800A7CA0.unk0 / SOME_FACTOR;
    D_800A7CA0.unkC = D_800A7CA0.unk4 / SOME_FACTOR;
    temp_s0 = temp_v1->unk7E;
    if (temp_s0 == -1) {
        D_800A7CA0.unk10 = 0;
        return &D_800A7CA0;
    }
    i = 0;
    sp24 = -1;
    temp_s0 += (temp_t5 * (temp_v1->unk7F & 0xF)) + temp_t8;
    for (; i < 6; i++) {
        if ((temp_s0 + 0x7530) == D_800A7CB8[i]) {
            sp24 = i;
            i = 6;
        }
    }
    if (sp24 != -1) {
        D_800A7CD0[sp24] = 0;
        D_800A7CA0.unk10 = D_800A7C80[sp24];
    } else {
        i = 0;
        var_a1 = -1;
        sp24 = -1;
        for (; i < 6; i++) {
            if (var_a1 < D_800A7CD0[i]) {
                sp24 = i;
                var_a1 = D_800A7CD0[i];
            }
        }
        if (func_800075B0(temp_s0, sp24) != 0) {
            D_800A7CB8[sp24] = (temp_s0 + 30000);
            D_800A7CD0[sp24] = 0;
            D_800A7CA0.unk10 = D_800A7C80[sp24];
        } else {
            D_800A7CA0.unk10 = 0;
        }
    }
    return &D_800A7CA0;
}

Unk800A7CA0* func_80007CBC(void) {
    return &D_800A7CA0;
}

void func_80007CCC(s32 arg0) {
    rarezip_uncompress_size_rom(TEXPRE_BIN, D_800A7C70[arg0], 1);
}

s32 *func_80007D08(u8 *arg0, s32 *arg1, u8 *arg2, s32 arg3, s32 arg4, s32 arg5) {
    u32 var_v1;
    u8 *temp_v0;
    u8 *var_a1;
    s32 var_a0;
    s32 new_var;
    u8 new_var2;

    new_var = arg4 * 2 + arg4;
    if ((arg5 >> 3) != 0) {
        var_v1 = arg0[new_var + 1];
        var_v1 >>= 4;
        var_v1 |= arg0[new_var + 2] << 4;
        var_a1 = &arg2[(arg4 << 5) | 0x10];
    } else {
        var_v1 = arg0[new_var];
        var_v1 |= ((arg0[new_var + 1] & 0xF) << 8);
        var_a1 = &arg2[arg4 << 5];
    }
    temp_v0 = &arg2[(arg4 << 5) | ((arg5 << 1) + (arg3 >> 3))];
    while (var_a1 < temp_v0) {
        var_a0 = *var_a1;
        var_a1++;
        while (var_a0 != 0) {
            var_a0 &= (var_a0 - 1) & 0xFF;
            var_v1++;
        }
    }
    var_a0 = *var_a1;
    new_var2 = 0xFFU >> (8 - (arg3 & 7));
    var_a0 &= new_var2 & 0xFF;
    while (var_a0 != 0) {
        var_a0 &= (var_a0 - 1) & 0xFF;
        var_v1++;
    }
    return &arg1[var_v1];
}

void func_80007E2C(Vec3f* arg0, Vec3s16 *arg1) {
    arg0->x = (arg1->s[0] * SOME_FACTOR) + 5;
    arg0->y = (arg1->s[1] * SOME_FACTOR) + 5;
    arg0->z = (arg1->s[2] * SOME_FACTOR) + 5;
    if (D_800A7D08 != NULL) {
        transform_point_by_object(arg0->x, arg0->y, arg0->z, arg0->f, &arg0->y, &arg0->z, D_800A7D08);
    }
}

void func_80007EE0(Vec3f* arg0, Vec3s16 *arg1) {
    Vec3f sp44;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;

    sp44.z = arg0->x;
    sp44.y = arg0->y;
    sp44.x = arg0->z;
    if (D_800A7D08 != NULL) {
        inverse_transform_point_by_object(sp44.z, sp44.y, sp44.x, &sp44.z, &sp44.y, &sp44.x, D_800A7D08);
    }
    var_a0 = sp44.z;
    var_a1 = sp44.y;
    var_a2 = sp44.x;
    if (sp44.z < 0.0f) {
        var_a0 -= SOME_FACTOR;
    }
    if (sp44.y < 0.0f) {
        var_a1 -= SOME_FACTOR;
    }
    if (sp44.x < 0.0f) {
        var_a2 -= SOME_FACTOR;
    }
    arg1->s[0] = var_a0 / SOME_FACTOR;
    arg1->s[1] = var_a1 / SOME_FACTOR;
    arg1->s[2] = var_a2 / SOME_FACTOR;
}

s32 func_80008048(Vec3s16 *arg0, Vec3s16 *arg1, Vec3s16 *arg2, u8* arg3, u8 arg4) {
    s32 spD4;
    s32 spD0;
    s32 spCC;
    s32 temp_a3;
    s32 temp_s0;
    s32 temp_t1;
    s32 var_s1;
    s32 var_s2;
    s32 var_s3;
    s32 spB0;
    s32 spAC;
    s32 spA8;
    s32 spA4;
    Unk800A7CA0Inner *var_s4;
    s32 sp9C;
    s32 var_s5;
    s32 var_s7;
    s32 var_v0;
    s32 var_v1;
    s32 sp88;
    Vec3s16 sp80 = arg0[0];
    Vec3s16 sp78;
    Unk800A7CA0Inner *tempObj;
    s32 pad;
    s32 temp2;
    u8* sp68;
    u8 sp67;
    u8 temp;

    spD4 = 1;
    spB0 = arg1->s[0] - sp80.s[0];
    if (spB0 < 0) {
        spD4 = -1;
        spB0 = -spB0;
    }
    spD0 = 1;
    spAC = arg1->s[1] - sp80.s[1];
    if (spAC < 0) {
        spD0 = -1;
        spAC = -spAC;
    }
    spCC = 1;
    spA8 = arg1->s[2] - sp80.s[2];
    if (spA8 < 0) {
        spCC = -1;
        spA8 = -spA8;
    }
    var_s7 = spAC - spB0;
    var_s2 = spA8 - spB0;
    var_s3 = spAC - spA8;
    spA4 = spB0 + spAC + spA8;
    func_80007778(&sp80);
    var_s5 = (sp80.s[0] - D_800A7CA0.unk8);
    var_s5 &= 0x3F;
    var_s1 = (sp80.s[2] - D_800A7CA0.unkC);
    var_s1 &= 0x3F;
    sp78 = sp80;
    var_s4 = NULL;
    sp67 = 1;
    while (spA4--) {
        if (arg4 && sp67 != 0) {
            sp67 = 0;
        } else {
            tempObj = D_800A7CA0.unk10;
            if (tempObj != 0) {
                if ((tempObj != var_s4) || (sp80.s[1] != sp78.s[1])) {
                    sp88 = 1;
                    if (tempObj->unk4 > sp80.s[1]) {
                        sp9C = 0;
                        sp78.s[1] = sp80.s[1];
                    } else {
                        if (sp80.s[1] >= tempObj->unkC) {
                            sp9C = (tempObj->unkC - tempObj->unk4) - 1;
                            sp78.s[1] = sp80.s[1];
                        } else {
                            sp9C = sp80.s[1] - tempObj->unk4;
                            sp78.s[1] = sp80.s[1];
                        }
                    }
                    var_s4 = tempObj;
                }
                temp_a3 = var_s5 >> 2;
                temp_t1 = var_s1 >> 2;
                var_v0 = tempObj->unk24[((sp9C << 5) | ((temp_t1 * 2) + (temp_a3 >> 3)))];
                // @fake
                if (!var_s5) {}
                if ((var_v0 >> (temp_a3 & 7)) & 1) {
                    // var_s1 * 0 is @fake
                    if (sp88 != (var_s1 * 0)) {
                        sp68 = func_80007D08(tempObj->unk1C, tempObj->unk14, tempObj->unk24, temp_a3, sp9C, temp_t1);
                        sp88 = 0;
                    }
                    temp2 = sp68[var_s1 & 3];
                    // @fake
                    if (temp2>>((var_s5 & 3) << 1)) {}
                    temp = (temp2>>((var_s5 & 3) << 1)) & 3;
                    if (temp & 3) {
                        if (arg3 != NULL) {
                            arg3[0] = temp & 3;
                        }
                        if (arg2 != NULL) {
                            arg2[0] = sp78;
                        }
                        return 0;
                    }
                }
            }
        }
        if (var_s7 < 0) {
            if (var_s2 < 0) {
                var_s7 += spAC * 2;
                sp78.s[0] = sp80.s[0];
                sp80.s[0] += spD4;\
                var_s2 += spA8 * 2;
                temp_s0 = var_s5 >> 2;
                var_v1 = sp80.s[0] - D_800A7CA0.unk8;
                if ((var_v1 >> 6) != 0) {
                    func_80007778(&sp80);
                    var_s4 = NULL;
                }
                var_v1 = sp80.s[0] - D_800A7CA0.unk8;
                var_s5 = var_v1 & 0x3F;
                if (temp_s0 != (var_s5 >> 2)) {
                    sp88 = 1;
                }
            } else {
                sp78.s[2] = sp80.s[2];
                sp80.s[2] += spCC;\
                var_s2 -= spB0 * 2;
                var_s3 += spAC * 2;
                temp_s0 = var_s1 >> 2;
                var_v1 = sp80.s[2] - D_800A7CA0.unkC;
                if ((var_v1 >> 6) != 0) {
                    func_80007778(&sp80);
                    var_s4 = NULL;
                }
                var_v1 = sp80.s[2] - D_800A7CA0.unkC;
                var_s1 = var_v1 & 0x3F;
                if (temp_s0 != (var_s1 >> 2)) {
                    sp88 = 1;
                }
            }
        } else {
            if (var_s3 < 0) {
                sp78.s[2] = sp80.s[2];
                sp80.s[2] += spCC;\
                var_s2 -= spB0 * 2;\
                var_s3 += spAC * 2;
                var_v1 = sp80.s[2] - D_800A7CA0.unkC;
                temp_s0 = var_s1 >> 2;
                if ((var_v1 >> 6) != 0) {
                    func_80007778(&sp80);
                    var_s4 = NULL;
                }
                var_v1 = sp80.s[2] - D_800A7CA0.unkC;
                var_s1 = var_v1 & 0x3F;
                if (temp_s0 != (var_s1 >> 2)) {
                    sp88 = 1;
                }
            } else {
                sp78.s[1] = sp80.s[1];
                var_s7 -= spB0 * 2;\
                sp80.s[1] += spD0;\
                var_s3 -= spA8 * 2;
            }
        }
    }
    if (arg2 != NULL) {
        arg2[0] = arg1[0];
    }
    return 1;
}

#ifndef NON_EQUIVALENT
void* func_80008524(Vec3s16 *arg0, Vec3s16 *arg1, s32* arg2, s32* arg3, s32* arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80008524.s")
#else
// https://decomp.me/scratch/x1DTn
void* func_80008524(Vec3s16 *arg0, Vec3s16 *arg1, s32* arg2, s32* arg3, s32* arg4) {
    f32 temp_fv0;
    s32 temp_a2;
    s32 temp_t2;
    s32 temp_t3;
    u16 temp_v0_3;
    u16 temp_v1_3;
    s32 var_a0;
    s32 var_a1;
    s32 var_a3;
    s32 var_t0;
    s32 var_t1;
    s32 var_v0;
    s32 var_v1;
    s32 sp138;
    s32 spDC[0x14];
    s32 sp8C[0x14];

    temp_fv0 = sqrtf(SQ(arg0[0] - arg1->s) + SQ(arg0[2] - arg1[2]));
    var_a3 = 0;
    var_v0 = temp_fv0;
    if (var_v0 < (temp_fv0 - 0.5f)) {
        var_v0 = var_v0 + 1;
    }
    var_a0 = (var_v0 * -2) + 5;
    var_t0 = var_v0;
    var_v1 = 1 - var_t0;
    var_a1 = 3;
    sp138 = 0;
    spDC[sp138] = var_a3;
    sp8C[sp138] = var_t0;
    while (var_t0 > var_a3) {
        if (var_v1 > 0) {
            var_v1 += var_a0;
            var_a0 += 4;
            var_t0 -= 1;
        } else {
            var_v1 += var_a1;
            var_a0 += 2;
        }
        var_a3++;
        spDC[sp138] = var_a3;
        sp8C[sp138] = var_t0;
        var_a1 += 2;
        sp138++;
        if (sp138 >= 0x14) {
            return NULL;
        }
    }
    var_t1 = 0;
    temp_a2 = sp138 * 2;
    while (var_t1 <= sp138) {
        s32 var_s2 = ((arg0[1] + sp8C[var_t1]) << 0x10);
        s32 var_s0 = ((arg0[1] + spDC[var_t1]) << 0x10);
        s32 temp = ((arg0[1] - spDC[var_t1]) << 0x10);
        s32 temp2 = ((arg0[1] - sp8C[var_t1]) << 0x10);
        temp_t3 = temp_a2 - var_t1;
        var_a0 = var_t1 + temp_a2;
        var_a1 = temp_t3 + temp_a2;
        temp_v0_3 = arg0[0] + spDC[var_t1];
        temp_v1_3 = arg0[0] + sp8C[var_t1];
        D_800A7C98[var_t1] = temp_v0_3 | var_s2;
        D_800A7C98[temp_t3] = temp_v1_3 | var_s0;
        D_800A7C98[var_a0] = temp_v1_3 | temp;
        D_800A7C98[var_a1] = temp_v0_3 | temp2;
        var_a0 += temp_a2;
        var_a1 += temp_a2;
        temp_v0_3 = arg0[0] - spDC[var_t1];
        temp_v1_3 = arg0[0] - sp8C[var_t1];
        D_800A7C98[var_a0] = temp_v0_3 | temp2;
        D_800A7C98[var_a1] = temp_v1_3 | temp;
        D_800A7C98[var_a0 + temp_a2] = temp_v1_3 | var_s0;
        D_800A7C98[var_a1 + temp_a2] = temp_v0_3 | var_s2;
        var_t1 += 1;
    }
    *arg2 = sp138 * 8;
    *arg3 = -2;
    *arg4 = -2;
    temp_a2 = (arg1->s & 0xFFFF) | (arg1[2] << 0x10);
    var_t1 = 0;
    while (var_t1 < *arg2 && (temp_a2 != D_800A7C98[var_t1])) {
        var_t1 += 1;
    }
    if (var_t1 == *arg2) {
        temp_t2 = (arg1->s & 0xFFFF) |  ((arg1[2] - 1) << 0x10);
        temp_a2 = (arg1->s & 0xFFFF) |  ((arg1[2] + 1) << 0x10);
        var_t1 = 0;
        while (var_t1 < *arg2 && *arg3 == -2 && *arg4 == -2) {
            if (temp_t2 == D_800A7C98[var_t1]) {
                *arg3 = var_t1;
                *arg4 = var_t1 + 1;
            } else if (temp_a2 == D_800A7C98[var_t1]) {
                *arg3 = var_t1 - 1;
                *arg4 = var_t1;
            }
            var_t1 += 1;
        }
    } else {
        *arg3 = var_t1 - 1;
        *arg4 = var_t1 + 1;
    }
    if (*arg3 < 0) {
        *arg3 += *arg2;
    }
    if (*arg4 >= *arg2) {
        *arg4 = *arg4 - *arg2;
    }
    return D_800A7C98;
}
#endif

s32 func_8000896C(Vec3s16 *arg0, Vec3s16 *arg1, Vec3s16 *arg2) {
    s32 pad;
    s32 sp80;
    s32 sp7C;
    s32 sp78;
    s32 var_s0;
    s32 var_s2;
    s32 sp6C;
    s32 var_s3;
    s32 var_s5;
    Vec3s16 sp5C;
    Vec3s16 sp54;
    s32* temp_v0;

    temp_v0 = func_80008524(arg0, arg1, &sp6C, &sp80, &sp7C);
    if (temp_v0 == NULL) {
        return 0;
    }
    sp78 = (sp6C >> 1) + sp7C;
    if (sp78 >= sp6C) {
        sp78 -= sp6C;
    }
    var_s0 = FALSE;
    var_s5 = 0;
    arg2[0] = arg1[0];
    var_s2 = FALSE;
    var_s3 = FALSE;
    sp5C.unk2 = arg1->unk2;
    while (var_s0 == FALSE) {
        sp5C.unk0 = temp_v0[sp7C] & 0xFFFF & 0xFFFF;
        sp5C.unk4 = temp_v0[sp7C] >> 0x10;
        if ((var_s2 == FALSE) && func_80008048(&sp5C, arg0, &sp54, NULL, 0)) {
            var_s5 = -1;
            var_s0 = TRUE;
            arg2[0] = sp5C;
        } else {
            if (sp7C++ == sp78) {
                var_s0 = TRUE;
            } else if (sp7C >= sp6C) {
                sp7C = 0;
            }
            if ((sp54.unk0 == sp5C.unk0) && (sp54.unk2 == sp5C.unk2) && (sp54.unk4 == sp5C.unk4)) {
                var_s2 = TRUE;
            }
        }
        if (var_s0 == FALSE) {
            sp5C.unk0 = temp_v0[sp80] & 0xFFFF & 0xFFFF;
            sp5C.unk4 = temp_v0[sp80] >> 0x10;
            if ((var_s3 == FALSE) && func_80008048(&sp5C, arg0, &sp54, NULL, 0)) {
                var_s0 = TRUE;
                var_s5 = 1;
                arg2[0] = sp5C;
            } else {
                sp80--;
                if (sp80 < 0) {
                    sp80 = sp6C - 1;
                }
                if ((sp54.unk0 == sp5C.unk0) && (sp54.unk2 == sp5C.unk2) && (sp54.unk4 == sp5C.unk4)) {
                    var_s3 = TRUE;
                }
            }
        }
    }
    return var_s5;
}

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80008BE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80008D90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80008DC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80008E08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80008E40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80008F34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80009024.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000921C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80009528.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_800095B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_800099D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80009F5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000A078.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000A640.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000A650.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000A6D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000A76C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000A80C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000A884.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000A934.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000A9AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000AB18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000AB84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000ACD4.s")
