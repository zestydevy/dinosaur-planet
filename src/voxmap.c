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

extern s32* D_800A7C70;
extern s32* D_800A7C74;
extern void* D_800A7C78;
extern void* D_800A7C80[6];
extern void* D_800A7C98;
// missing 0x800A7C9C
extern s32 D_800A7CA0[5];
extern s32 D_800A7CB8[6];
extern s32 D_800A7CD0[6];
extern u8 D_800A7CE8[6];
extern s16 D_800A7CF0[6][2];
extern s32 D_800A7D08;
extern s32 D_800A7D0C;

#define SOME_VALUE 0x40000000

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80006F50.s")
#else
// https://decomp.me/scratch/aGcnb
void func_80006F50(void) {
    s32 i;

    queue_alloc_load_file(&D_800A7C70, TEXPRE_TAB);
    queue_alloc_load_file(&D_800A7C74, VOXOBJ_TAB);
    for (i = 0; D_800A7C74[i] != -1; i++) {}

    D_800A7D0C = --i;
    D_800A7C78 = mmAlloc(0x280, 0x10, NULL);
    i = 0;
    while (i < 6) {
        D_800A7C80[i] = 0;
        D_800A7CB8[i] = -2;
        D_800A7CD0[i] = SOME_VALUE;
        D_800A7CE8[i] = 0;
        D_800A7CF0[i][0] = 0;
        D_800A7CF0[i][1] = 0;
        i++;
    }
    D_800A7C98 = D_800A7C78;
    D_800A7D08 = 0;
}
#endif

void func_800070FC(void) {
    void *temp_a0;
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

void *func_80007224(void) {
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

void func_80007380(void *arg0) {
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
    s32 temp_a0;

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

void* func_80007778(s16* arg0) {
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
        return func_80007A58();
    }
    var_v0 = ((arg0[0] * 0xA) - D_80092A60) + 5;
    var_s1 = ((arg0[2] * 0xA) - D_80092A64) + 5;
    var_s0 = floor_f(var_v0 / BLOCKS_GRID_UNIT_F);
    sp40 = floor_f(var_s1 / BLOCKS_GRID_UNIT_F);
    D_800A7CA0[0] = (var_s0 * BLOCKS_GRID_UNIT) + D_80092A60;
    D_800A7CA0[1] = (sp40 * BLOCKS_GRID_UNIT) + D_80092A64;
    D_800A7CA0[2] = D_800A7CA0[0] / 10;
    D_800A7CA0[3] = D_800A7CA0[1] / 10;
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
            D_800A7CA0[4] = (s32) D_800A7C80[var_s0];
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
                D_800A7CF0[var_s0][0] = D_800A7CA0[2];
                D_800A7CF0[var_s0][1] = D_800A7CA0[3];
            }
            D_800A7CA0[4] = 0;
        }
    } else {
        D_800A7CA0[4] = 0;
    }
    return &D_800A7CA0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80007A58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80007CBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80007CCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80007D08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80007E2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80007EE0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80008048.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_80008524.s")

#pragma GLOBAL_ASM("asm/nonmatchings/voxmap/func_8000896C.s")

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
