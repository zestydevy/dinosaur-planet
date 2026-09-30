#include "common.h"
#include "sys/lfx.h"
#include "sys/segment_13D0.h"
#include "sys/lighting.h"
#include "sys/newshadows.h"
#include "sys/objects.h"
#include "sys/gfx/modgfx.h"
#include "dlls/objects/276_InvHit.h"
#include "dlls/engine/14_modgfx.h"
#include "macros.h"

// size: 0x140
typedef struct {
    Object* unk0;
    Object* unk4;
    s16* unk8;
    SRT unkC;
    Vec3f unk24;
    Vec3f unk30[4];
    f32 unk60;
    f32 unk64;
    f32 unk68;
    Vec3f unk6C;
    Vtx* unk78[3];
    DLTri* unk84[3];
    DLTri* unk90;
    DLTri* unk94;
    Texture* unk98;
    ModgfxStruct_0* unk9C;
    LightAction* unkA0;
    s32 unkA4;
    s32 unkA8;
    f32 unkAC[4];
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    u8 _unkD8[0xE6 - 0xD8];
    s16 unkE6;
    u8 padE8;
    s16 unkEA;
    s16 unkEC;
    s16 unkEE[7];
    s16 unkFC;
    s16 unkFE;
    s16 unk100;
    s16 unk102;
    s16 unk104;
    s16 unk106; // yaw
    s16 unk108; // pitch
    s16 unk10A; // roll
    s16 unk10C;
    s16 unk10E;
    s16 unk110;
    u16 pad112;
    s32 unk114[3];
    s16 unk120;
    s16 unk122;
    s16 unk124;
    u8 unk126;
    u8 _unk127[0x12C - 0x127];
    DLTri* unk12C;
    u8 unk130; // 0 or 1, indexes unk78
    u8 unk131;
    u8 unk132;
    u8 unk133;
    u8 unk134;
    u8 unk135;
    u8 unk136;
    u8 unk137;
    u8 unk138;
    s8 unk139; // keeps track of count for unk9C
    u8 unk13A;
    u8 unk13B;
    u8 unk13C;
    u8 unk13D;
    u8 unk13E;
    u8 unk13F;
} ModgfxInstance;

/*0x0*/ static u32 data_0 = 0;
/*0x4*/ static s16 data_4 = 0;
/*0x8*/ static u8 data_8 = 0;
/*0xC*/ static f32 data_C = 0.0f;
/*0x10*/ static f32 data_10 = 0.0f;

/*0x0*/ static ModgfxInstance* bss_0[496];
/*0x7C0*/ static ModgfxStruct_0 bss_7C0[32];
/*0xAC0*/ static ModgfxStruct_0 *bss_AC0;
/*0xAC4*/ static ModgfxStruct_0 *bss_AC4;
/*0xAC8*/ static s16 bss_AC8;
/*0xAD0*/ static ModgfxStruct bss_AD0;

/*0x0*/ static const char str_0[] = "warning in modgfx dll no spare memory available\n";

static void modgfx_func_4C0C(s16 arg0, s32 arg1);
static void modgfx_func_4EDC(ModgfxInstance* arg0, u8 arg1);
static s32 modgfx_func_4BA4(void);
static void modgfx_func_6424(ModgfxInstance* arg0, s32 arg1);
static void modgfx_func_64F0(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3);
static void modgfx_func_4E58(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2);
static void modgfx_func_4FF4(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3);
static void modgfx_func_538C(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3);
static void modgfx_func_6100(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3);
static void modgfx_func_6068(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3);
static void modgfx_func_584C(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3);
static void modgfx_func_6758(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, s32 arg3);
static void modgfx_func_56A0(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3);
static void modgfx_func_5E50(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3);
static void modgfx_func_5FFC(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3);

// offset: 0x0 | ctor
void modgfx_ctor(void* dll) {
    s32 i;
    for (i = 0; i < ARRAYCOUNT(bss_0); i++) { bss_0[i] = 0; }
}

// offset: 0x40 | dtor
void modgfx_dtor(void* dll) {
    modgfx_func_4C0C(0, 1);
}

// offset: 0x88 | func: 0 | export: 0
void modgfx_Func0(void) {
    s32 i;

    modgfx_func_4C0C(0, 1);
    for (i = 0; i < ARRAYCOUNT(bss_0); i++) { bss_0[i] = 0; }
}

// offset: 0xF4 | func: 1 | export: 1
s16 modgfx_Func1(ModgfxStruct* arg0, s32 arg1, s32 arg2, s16* arg3, s32 arg4, s16* arg5, s32 arg6, Texture* arg7) {
    s32 sp54;
    s32 var_t1;
    DLTri* var_v1;
    DLTri* var_v1_2;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_a3;
    s32 var_t0;
    s16* var_t1_3;
    s32 sp28;
    Vtx* var_v0_3;

    sp28 = 0;
    temp_v0 = modgfx_func_4BA4();
    if (temp_v0 == -1) {
        return 0;
    }
    sp54 = 0;
    if (!(arg0->unk54 & 0x800)) {
        sp54 += arg4 * 3 * sizeof(DLTri);
        sp54 += arg2 * 3 * sizeof(DLTri);
    }
    sp54 += sizeof(DLTri) * 16;
    bss_0[temp_v0] = NULL;
    bss_0[temp_v0] = mmAlloc(sizeof(ModgfxInstance), ALLOC_TAG_MODGFX_COL, NULL);
    var_v1 = mmAlloc(sp54, ALLOC_TAG_MODGFX_COL, NULL);
    if (var_v1 == NULL || bss_0[temp_v0] == NULL) {
        modgfx_func_4C0C(0, 0);
        return -1;
    }
    bss_0[temp_v0]->unk12C = var_v1;
    if (!(arg0->unk54 & 0x800)) {
        bss_0[temp_v0]->unk84[0] = var_v1;
        var_v1 += arg4;
        bss_0[temp_v0]->unk84[1] = var_v1;
        var_v1 += arg4;
        bss_0[temp_v0]->unk84[2] = var_v1;
        var_v1 += arg4;
        bss_0[temp_v0]->unk78[0] = (Vtx*) var_v1;
        var_v1 += arg2;
        bss_0[temp_v0]->unk78[1] = (Vtx*) var_v1;
        var_v1 += arg2;
        bss_0[temp_v0]->unk78[2] = (Vtx*) var_v1;
        var_v1 += arg2;
    }
    bss_0[temp_v0]->unk90 = var_v1;
    var_v1 += 8;
    bss_0[temp_v0]->unk94 = var_v1;
    if (arg0->unk40 != 0) {
        var_a3 = arg4 / arg0->unk40;
    } else {
        var_a3 = arg4;
    }
    if (!(arg0->unk54 & 0x800)) {
        for (var_t1 = 0; var_t1 < 3; var_t1++) {
            var_t0 = 0;
            var_a0 = 0;
            var_v1_2 = bss_0[temp_v0]->unk84[var_t1];
            for (var_a1 = 0; var_a1 < arg4; var_a1++, var_t0 += 3, var_v1_2++) {
                if ((arg0->unk54 & 0x8000000) && var_a1 == var_a3) {
                    var_a0 = arg0->unk3C;
                }
                var_v1_2->v0 = arg5[var_t0 + 0] - var_a0;
                var_v1_2->v1 = arg5[var_t0 + 1] - var_a0;
                var_v1_2->v2 = arg5[var_t0 + 2] - var_a0;
            }
        }
    }
    if (!(arg0->unk54 & 0x800)) {
        for (var_t1 = 0; var_t1 < 3; var_t1++) {
            var_v0_3 = bss_0[temp_v0]->unk78[var_t1];
            var_a2 = 0;
            for (var_a1 = 0; var_a1 < arg2; var_a1++, var_a2 += 5) {
                var_v0_3->v.ob[0] = arg3[var_a2 + 0];
                var_v0_3->v.ob[1] = arg3[var_a2 + 1];
                var_v0_3->v.ob[2] = arg3[var_a2 + 2];
                var_v0_3->v.tc[0] = arg3[var_a2 + 3] << 5;
                var_v0_3->v.tc[1] = arg3[var_a2 + 4] << 5;
                var_v0_3->v.cn[0] = 0xFF;
                var_v0_3->v.cn[1] = 0xFF;
                var_v0_3->v.cn[2] = 0xFF;
                var_v0_3->v.cn[3] = 0xFF;
                var_v0_3++;
            }
        }
    }
    bss_0[temp_v0]->unk139 = arg0->unk5D;
    bss_0[temp_v0]->unk114[0] = 0;
    bss_0[temp_v0]->unk114[1] = 0;
    bss_0[temp_v0]->unk114[2] = 0;
    bss_0[temp_v0]->unkA0 = NULL;
    bss_0[temp_v0]->unk13A = 0;
    bss_0[temp_v0]->unk13D = 0;
    bss_0[temp_v0]->unk110 = 0;
    bss_0[temp_v0]->unk10E = -1;
    bss_0[temp_v0]->unk13C = 0;
    for (var_t1 = 0; var_t1 < 7; var_t1++) {
         bss_0[temp_v0]->unkEE[var_t1] = arg0->unk46[var_t1];
    }
    bss_0[temp_v0]->unk9C = NULL;
    bss_0[temp_v0]->unk9C = mmAlloc(bss_0[temp_v0]->unk139 * sizeof(ModgfxStruct_0), ALLOC_TAG_MODGFX_COL, NULL);
    if (bss_0[temp_v0]->unk9C == NULL) {
        modgfx_func_4C0C(0, 0);
        return -1;
    }
    bss_0[temp_v0]->unk8 = NULL;
    for (var_t1 = 0; var_t1 < bss_0[temp_v0]->unk139; var_t1++) {
        if (!(arg0->unk0[var_t1].unk0 & 0xF7FFF180)) { // ~0x8000E7F
            if (arg0->unk0[var_t1].unk14 != 0) {
                sp28 += arg0->unk0[var_t1].unk14;
            }
        }
    }
    if (sp28 != 0) {
        bss_0[temp_v0]->unk8 = mmAlloc(sp28 * sizeof(s16), ALLOC_TAG_MODGFX_COL, NULL);
    }
    var_t1_3 = bss_0[temp_v0]->unk8;
    for (var_t1 = 0; var_t1 < bss_0[temp_v0]->unk139; var_t1++) {
        bss_0[temp_v0]->unk9C[var_t1].unk16 = arg0->unk0[var_t1].unk16;
        bss_0[temp_v0]->unk9C[var_t1].unk14 = arg0->unk0[var_t1].unk14;
        bss_0[temp_v0]->unk9C[var_t1].unk10 = 0;
        bss_0[temp_v0]->unk9C[var_t1].unk0 = arg0->unk0[var_t1].unk0;
        if (!(bss_0[temp_v0]->unk9C[var_t1].unk0 & 0xF7FFF180) && (bss_0[temp_v0]->unk9C[var_t1].unk14 != 0)) {
            bss_0[temp_v0]->unk9C[var_t1].unk10 = 0;
            bss_0[temp_v0]->unk9C[var_t1].unk10 = var_t1_3;
            var_t1_3 += bss_0[temp_v0]->unk9C[var_t1].unk14;
            if (bss_0[temp_v0]->unk9C[var_t1].unk10 == 0) {
                modgfx_func_4C0C(0, 0);
                return -1;
            }
            for (var_a1 = 0; var_a1 < bss_0[temp_v0]->unk9C[var_t1].unk14; var_a1++) {
                bss_0[temp_v0]->unk9C[var_t1].unk10[var_a1] = arg0->unk0[var_t1].unk10[var_a1];
            }
        }
        for (var_a1 = 0; var_a1 < 3; var_a1++) {
            bss_0[temp_v0]->unk9C[var_t1].unk4.f[var_a1] = arg0->unk0[var_t1].unk4.f[var_a1];
        }
    }
    bss_0[temp_v0]->unkFC = -1;
    bss_0[temp_v0]->unkFE = bss_0[temp_v0]->unkEE[bss_0[temp_v0]->unkFC];
    bss_0[temp_v0]->unkA4 = arg0->unk54;
    bss_0[temp_v0]->unk60 = arg0->unk2C.x;
    bss_0[temp_v0]->unk64 = arg0->unk2C.y;
    bss_0[temp_v0]->unk68 = arg0->unk2C.z;
    bss_0[temp_v0]->unkD4 = arg0->unk38;
    if (bss_0[temp_v0]->unkA4 & 1) {
        bss_0[temp_v0]->unkC.transl.x = arg0->unk2C.x;
        bss_0[temp_v0]->unkC.transl.y = arg0->unk2C.y;
        bss_0[temp_v0]->unkC.transl.z = arg0->unk2C.z;
    }
    bss_0[temp_v0]->unk24.x = 0.0f;
    bss_0[temp_v0]->unk24.y = 0.0f;
    bss_0[temp_v0]->unk24.z = 0.0f;
    bss_0[temp_v0]->unk30[0].x = 1.0f;
    bss_0[temp_v0]->unk30[0].y = 1.0f;
    bss_0[temp_v0]->unk30[0].z = 1.0f;
    bss_0[temp_v0]->unk30[1].y = 0.0f;
    bss_0[temp_v0]->unk30[1].z = 0.0f;
    bss_0[temp_v0]->unk30[1].x = 0.0f;
    bss_0[temp_v0]->unk30[2].z = 1.0f;
    bss_0[temp_v0]->unk30[2].x = 1.0f;
    bss_0[temp_v0]->unk30[2].y = 1.0f;
    bss_0[temp_v0]->unk30[3].z = 0.0f;
    bss_0[temp_v0]->unk30[3].x = 0.0f;
    bss_0[temp_v0]->unk30[3].y = 0.0f;
    bss_0[temp_v0]->unk106 = 0;
    bss_0[temp_v0]->unk108 = 0;
    bss_0[temp_v0]->unk10A = 0;
    bss_0[temp_v0]->unk120 = 0;
    bss_0[temp_v0]->unk122 = 0;
    bss_0[temp_v0]->unk124 = 0;
    bss_0[temp_v0]->unkAC[0] = 0.0f;
    bss_0[temp_v0]->unkAC[1] = 0.0f;
    bss_0[temp_v0]->unkAC[2] = 0.0f;
    bss_0[temp_v0]->unkAC[3] = 0.0f;
    bss_0[temp_v0]->unkBC = 0.0f;
    bss_0[temp_v0]->unkC0 = 0.0f;
    bss_0[temp_v0]->unkC4 = 0.0f;
    bss_0[temp_v0]->unkC8 = 0.0f;
    bss_0[temp_v0]->unkCC = 0.0f;
    bss_0[temp_v0]->unkD0 = 0.0f;
    bss_0[temp_v0]->unk6C.x = arg0->unk20;
    bss_0[temp_v0]->unk6C.y = arg0->unk24;
    bss_0[temp_v0]->unk6C.z = arg0->unk28;
    data_4++;
    if (data_4 > 20000) {
        data_4 = 0;
    }
    bss_0[temp_v0]->unk10C = data_4;
    bss_0[temp_v0]->unk126 = data_8;
    bss_0[temp_v0]->unkEA = arg2;
    bss_0[temp_v0]->unkEC = arg4;
    bss_0[temp_v0]->unk4 = arg0->unk4;
    bss_0[temp_v0]->unk0 = NULL;
    bss_0[temp_v0]->unk135 = arg0->unk5C;
    bss_0[temp_v0]->unk136 = arg0->unk40;
    bss_0[temp_v0]->unk137 = arg0->unk3C;
    bss_0[temp_v0]->unk138 = arg0->unk59;
    bss_0[temp_v0]->unkE6 = 0;
    bss_0[temp_v0]->unk130 = 0;
    bss_0[temp_v0]->unk13B = 0;
    bss_0[temp_v0]->unk13E = 0;
    bss_0[temp_v0]->unk98 = NULL;
    bss_0[temp_v0]->unk13F = 0;
    if (arg7 != NULL) {
        bss_0[temp_v0]->unk98 = arg7;
        bss_0[temp_v0]->unk13F = 1;
    } else if (arg6 != 0) {
        bss_0[temp_v0]->unk98 = texLoadTexture(arg6);
        bss_0[temp_v0]->unk13F = 0;
    }
    if (arg0->unk54 & 0x40000) {
        // @bug, this should not take a double pointer to ModgfxInstance!
        modgfx_func_4EDC((ModgfxInstance*)&bss_0[temp_v0], 1U);
    }
    bss_0[temp_v0]->unk132 = arg0->unk5B;
    if (bss_0[temp_v0]->unk132 != 0) {
        bss_0[temp_v0]->unk133 = (0x3C / bss_0[temp_v0]->unk132);
    } else {
        bss_0[temp_v0]->unk133 = 0;
    }
    if (bss_0[temp_v0]->unk133 != 0) {
        bss_0[temp_v0]->unk134 = (0xFF / bss_0[temp_v0]->unk133);
    } else {
        bss_0[temp_v0]->unk134 = 0;
    }
    bss_0[temp_v0]->unk131 = 0;
    bss_0[temp_v0]->unkA8 = arg0->unk44;
    return bss_0[temp_v0]->unk10C;
}

// offset: 0xC90 | func: 2 | export: 2
void modgfx_Func2(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 sp108;
    s32 sp104;
    s32 var_fp;
    u32 spFC;
    s32 spF8;
    s32 spF4;
    s32 spF0;
    s32 spEC;
    InvHit_Setup* temp_v0_4;
    Object** temp_t0;
    SRT spCC;
    SRT spB4;
    f32 temp_fv0_3;
    s32 pad;
    s32 var_s0_2;
    Vec3f sp9C;
    s32 sp98;
    s32 sp94;
    DLL_IModgfx* modgfx;

    sp104 = 0;
    var_fp = 0;
    D_8008C504 = 2;
    if (func_80000824(-1) == 1) {
        return;
    }
    data_C = gUpdateRateF;
    for (i = 0; i < ARRAYCOUNT(bss_0); i++) {
        do {
            spEC = 0;
            if ((bss_0[i] != NULL) && (bss_0[i]->unk10C != -1)) {
                sp108 = i;
                spFC = 0;
                bss_0[sp108]->unk13E = 0;
                if ((bss_0[sp108]->unkFE < 0) || (bss_0[sp108]->unkFC == -1U)) {
                    bss_0[sp108]->unkFC++;
                    if (bss_0[sp108]->unkFC >= 7) {
                        modgfx_func_4C0C(bss_0[sp108]->unk10C, 0);
                        break;
                    }
                    bss_0[sp108]->unkFE = bss_0[sp108]->unkEE[bss_0[sp108]->unkFC];
                    spFC = 1;
                    modgfx_func_6424(bss_0[sp108], 0);
                } else if (bss_0[sp108]->unk13C != 0) {
                    bss_0[sp108]->unkFC = bss_0[sp108]->unk13C;
                    bss_0[sp108]->unk13C = 0;
                    if (bss_0[sp108]->unkFC >= 7) {
                        modgfx_func_4C0C(bss_0[sp108]->unk10C, 0);
                        break;
                    }
                    bss_0[sp108]->unkFE = bss_0[sp108]->unkEE[bss_0[sp108]->unkFC];
                    spFC = 1;
                    modgfx_func_6424(bss_0[sp108], 0);
                }
    
                spF8 = 0;
                spF4 = 0;
                modgfx_func_4E58(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], spFC);
                spF0 = 0;
                for (sp104 = 0; sp104 < bss_0[sp108]->unk139; sp104++) {
                    if (bss_0[sp108]->unk9C[sp104].unk16 == bss_0[sp108]->unkFC) {
                    if ((bss_0[sp108]->unk9C[sp104].unk0 & 0x1000) && (bss_0[sp108]->unk9C[sp104].unk4.x > 0.0f) && (bss_0[sp108]->unkFC > 0)) {
                        bss_0[sp108]->unkFC = bss_0[sp108]->unk9C[sp104].unk14;
                        bss_0[sp108]->unk9C[sp104].unk4.x = (f32) (bss_0[sp108]->unk9C[sp104].unk4.x - 1.0f);
                        bss_0[sp108]->unkFE = -1;
                        break;
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x2000) {
                        if (bss_0[sp108]->unk13A != 0) {
                            bss_0[sp108]->unk13A = 0;
                            bss_0[sp108]->unk9C[sp104].unk0 = 0;
                            bss_0[sp108]->unk9C[sp104].unk0 = 0x20;
                            bss_0[sp108]->unkFE = -1;
                            spEC = 1;
                            break;
                        }
                        if (bss_0[sp108]->unkFC > 0) {
                            spF0 = 1;
                            bss_0[sp108]->unkFC = bss_0[sp108]->unk9C[sp104].unk14;
                            bss_0[sp108]->unkFE = -1;
                            spEC = 1;
                            break;
                        }
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x10000000) {
                        spCC.transl.x = bss_0[sp108]->unk60;
                        spCC.transl.y = bss_0[sp108]->unk64;
                        spCC.transl.z = bss_0[sp108]->unk68;
                        spB4.transl.x = 0.0f;
                        spB4.transl.y = 0.0f;
                        spB4.transl.z = 0.0f;
                        spB4.scale = 1.0f;
                        if (!(bss_0[sp108]->unkA4 & 1)) {
                            spB4.yaw = bss_0[sp108]->unk4->srt.yaw;
                        } else {
                            spB4.yaw = bss_0[sp108]->unkC.yaw;
                        }
                        spB4.pitch = 0;
                        spB4.roll = 0;
                        mathRotateRPY(&spB4, &spCC.transl.x);
                        if (bss_0[sp108]->unk0 == NULL) {
                            if (!(bss_0[sp108]->unkA4 & 1)) {
                                spCC.transl.x = bss_0[sp108]->unk4->globalPosition.x + spCC.transl.x;
                                spCC.transl.y = bss_0[sp108]->unk4->globalPosition.y + spCC.transl.y;
                                spCC.transl.z = bss_0[sp108]->unk4->globalPosition.z + spCC.transl.z;
                            } else {
                                spCC.transl.x = bss_0[sp108]->unkC.transl.x + spCC.transl.x;
                                spCC.transl.y = bss_0[sp108]->unkC.transl.y + spCC.transl.y;
                                spCC.transl.z = bss_0[sp108]->unkC.transl.z + spCC.transl.z;
                            }
                            temp_v0_4 = objAllocSetup(0x20, 0x66);
                            temp_v0_4->unk19 = 0x10;
                            temp_v0_4->unk18 = 0xA;
                            temp_v0_4->unk1A = 1;
                            temp_v0_4->base.x = spCC.transl.x;
                            temp_v0_4->base.y = spCC.transl.y;
                            temp_v0_4->base.z = spCC.transl.z;
                            bss_0[sp108]->unk0 = objSetupObject(&temp_v0_4->base, 5, -1, -1, NULL);
                            bss_0[sp108]->unk0->unkE0 = 1;
                        } else if (bss_0[sp108]->unk0 != NULL) {
                            if (!(bss_0[sp108]->unkA4 & 1)) {
                                spCC.transl.x = bss_0[sp108]->unk4->globalPosition.x + spCC.transl.x;
                                spCC.transl.y = bss_0[sp108]->unk4->globalPosition.y + spCC.transl.y;
                                spCC.transl.z = bss_0[sp108]->unk4->globalPosition.z + spCC.transl.z;
                            } else {
                                spCC.transl.x = bss_0[sp108]->unkC.transl.x + spCC.transl.x;
                                spCC.transl.y = bss_0[sp108]->unkC.transl.y + spCC.transl.y;
                                spCC.transl.z = bss_0[sp108]->unkC.transl.z + spCC.transl.z;
                            }
                            bss_0[sp108]->unk0->globalPosition.x = spCC.transl.x;
                            bss_0[sp108]->unk0->globalPosition.y = spCC.transl.y;
                            bss_0[sp108]->unk0->globalPosition.z = spCC.transl.z;
                        }
                        if ((bss_0[sp108]->unk0 != NULL) && (bss_0[sp108]->unk0->objhitInfo->unk48 != NULL) && (bss_0[sp108]->unk0->objhitInfo->unk48->controlNo == (s32) bss_0[sp108]->unk9C[sp104].unk4.x)) {
                            objFreeObject(bss_0[sp108]->unk0);
                            bss_0[sp108]->unk0 = NULL;
                            bss_0[sp108]->unk9C[sp104].unk0 ^= 0x10000000;
                            if (bss_0[sp108]->unk9C[sp104].unk4.z >= 0.0f) {
                                if (bss_0[sp108]->unk4 != NULL) {
                                    dll_partfx->spawn(bss_0[sp108]->unk4, bss_0[sp108]->unk9C[sp104].unk4.z, &spCC, 0x200001, -1, NULL);
                                }
                            }
                            bss_0[sp108]->unk13C = bss_0[sp108]->unk9C[sp104].unk4.y;
                            break;
                        }
                    }
                    temp_t0 = objGetObjects(&sp94, &sp98);
                    if ((bss_0[sp108]->unk9C[sp104].unk0 & 0x20000000) && (bss_0[sp108]->unk4 != NULL) && (bss_0[sp108]->unk9C[sp104].unk14 != 0)) {
                        if (bss_0[sp108]->unk10E == -1) {
                            bss_0[sp108]->unk10E = (s16) bss_0[sp108]->unk9C[sp104].unk4.z;
                        }
                        spCC.transl.x = bss_0[sp108]->unk60;
                        spCC.transl.y = bss_0[sp108]->unk64;
                        spCC.transl.z = bss_0[sp108]->unk68;
                        if (!(bss_0[sp108]->unkA4 & 1)) {
                            spCC.transl.x = bss_0[sp108]->unk4->globalPosition.x + spCC.transl.x;
                            spCC.transl.y = bss_0[sp108]->unk4->globalPosition.y + spCC.transl.y;
                            spCC.transl.z = bss_0[sp108]->unk4->globalPosition.z + spCC.transl.z;
                        } else {
                            spCC.transl.x = bss_0[sp108]->unkC.transl.x;
                            spCC.transl.y = bss_0[sp108]->unkC.transl.y;
                            spCC.transl.z = bss_0[sp108]->unkC.transl.z;
                        }
                        if ((bss_0[sp108]->unk9C[sp104].unk4.x == 999.0f) && (bss_0[sp108]->unkA0 == NULL)) {
                            bss_0[sp108]->unkA0 = mmAlloc(sizeof(LightAction), ALLOC_TAG_LFX_COL, NULL);
                            assetRomLoadSection((void** ) bss_0[sp108]->unkA0, LACTIONS_BIN, (s16) bss_0[sp108]->unk9C[sp104].unk4.y * 0x28, 0x28);
                            if (bss_0[sp108]->unkA0 != NULL) {
                                dll_newlfx->DoAction(bss_0[sp108]->unk4, bss_0[sp108]->unk4, bss_0[sp108]->unkA0, 0, 0, NULL);
                            }
                            bss_0[sp108]->unk13D = 1;
                        } else {
                            for (var_fp = sp94; var_fp < sp98; var_fp++) {
                                if (temp_t0[var_fp]->controlNo == (s8) (s32) bss_0[sp108]->unk9C[sp104].unk4.x) {
                                    sp9C.f[0] = temp_t0[var_fp]->globalPosition.x - spCC.transl.x;
                                    sp9C.f[1] = temp_t0[var_fp]->globalPosition.y - spCC.transl.y;
                                    sp9C.f[2] = temp_t0[var_fp]->globalPosition.z - spCC.transl.z;
                                    temp_fv0_3 = sqrtf(SQ(sp9C.f[0]) + SQ(sp9C.f[1]) + SQ(sp9C.f[2]));
                                    if ((bss_0[sp108]->unk13D == 0) && (temp_fv0_3 < bss_0[sp108]->unk9C[sp104].unk14)) {
                                        lfxAction(bss_0[sp108]->unk4, temp_t0[var_fp], (s32) bss_0[sp108]->unk9C[sp104].unk4.y & 0xFFFF & 0xFFFF, 0, 0, NULL);
                                        bss_0[sp108]->unk13D = 1;
                                    } else {
                                        if ((bss_0[sp108]->unk13D == 1) && (bss_0[sp108]->unk9C[sp104].unk14 < temp_fv0_3)) {
                                            lfxAction(bss_0[sp108]->unk4, temp_t0[var_fp], (s32) bss_0[sp108]->unk9C[sp104].unk4.z & 0xFFFF & 0xFFFF, 0, 0, NULL);
                                            bss_0[sp108]->unk13D = 0;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    } else if (bss_0[sp108]->unk9C[sp104].unk0 & 0x20000000) {
                        if ((bss_0[sp108]->unk4 != NULL) && (bss_0[sp108]->unk9C[sp104].unk14 == 0)) {
                            lfxAction(bss_0[sp108]->unk4, temp_t0[var_fp], (s32) bss_0[sp108]->unk9C[sp104].unk4.z & 0xFFFF & 0xFFFF, 0, 0, NULL);
                            bss_0[sp108]->unk9C[sp104].unk0 ^= 0x20000000;
                        }
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 2) {
                        modgfx_func_4FF4(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], spFC, spF8);
                        spF8++;
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 4) {
                        modgfx_func_538C(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], spFC, spF4);
                        spF4++;
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 8) {
                        modgfx_func_6100(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], spFC, 0);
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x100) {
                        modgfx_func_6068(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], spFC, 0);
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x80) {
                        modgfx_func_584C(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], spFC, 0);
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x08000000) {
                        bss_0[sp108]->unk9C[sp104].unk4.z = mathRnd(0, 0xFFFF);
                        modgfx_func_584C(bss_0[sp108], bss_0[sp108]->unk9C + sp104, spFC, 0);
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x4000) {
                        modgfx_func_64F0(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], spFC, 0);
                    }
                    if ((bss_0[sp108]->unk9C[sp104].unk0 & 0x10000) && (spFC != 0)) {
                        gDLL_6_AMSFX->vtbl->Play(bss_0[sp108]->unk4, bss_0[sp108]->unk9C[sp104].unk14, 0x7F, NULL, NULL, 0, NULL);
                    }
                    if ((bss_0[sp108]->unk9C[sp104].unk0 & 0x20000) && (spFC != 0)) {
                        pad = bss_0[sp108]->unk114[(s32)bss_0[sp108]->unk9C[sp104].unk4.x];
                        if (pad == 0) {
                            gDLL_6_AMSFX->vtbl->Play(bss_0[sp108]->unk4, bss_0[sp108]->unk9C[sp104].unk14, 0x7F, &bss_0[sp108]->unk114[(s32)bss_0[sp108]->unk9C[sp104].unk4.x], NULL, 0, NULL);
                        } else {
                            gDLL_6_AMSFX->vtbl->Stop(pad);
                            bss_0[sp108]->unk114[(s32)bss_0[sp108]->unk9C[sp104].unk4.x] = 0;
                        }
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x42288000) {
                        modgfx_func_6758(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], bss_0[sp108]->unk9C[sp104].unk14, spFC);
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x100000) {
                        modgfx_func_56A0(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], spFC, 0);
                    }
                    // ?? why
                    if ((bss_0[sp108]->unk9C[sp104].unk0 << 9) < 0) {
                        modgfx_func_5E50(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], spFC, 0);
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x80000000) {
                        modgfx_func_5FFC(bss_0[sp108], &bss_0[sp108]->unk9C[sp104], spFC, 0);
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x800000) {
                        if ((bss_0[sp108]->unk9C[sp104].unk0 & 0x01000000) && (bss_0[sp108]->unk9C[sp104].unk4.y == 0.0f)) {
                            for (var_s0_2 = 0; var_s0_2 < (s32) bss_0[sp108]->unk9C[sp104].unk4.x; var_s0_2++) {
                                if (mathRnd(0, (s32) bss_0[sp108]->unk9C[sp104].unk4.z) == 0) {
                                    if (bss_0[sp108]->unkA4 & 1) {
                                        dll_partfx->spawn(bss_0[sp108]->unk4, bss_0[sp108]->unk9C[sp104].unk14, NULL, 0x10001, -1, NULL);
                                    } else {
                                        dll_partfx->spawn(bss_0[sp108]->unk4, bss_0[sp108]->unk9C[sp104].unk14, NULL, 0x10001, -1, NULL);
                                    }
                                }
                            }
                        } else {
                            if (bss_0[sp108]->unk9C[sp104].unk4.y == 0.0f) {
                                for (var_s0_2 = 0; var_s0_2 < (s32) bss_0[sp108]->unk9C[sp104].unk4.x; var_s0_2++) {
                                    if (bss_0[sp108]->unkA4 & 1) {
                                        dll_partfx->spawn(bss_0[sp108]->unk4, (s32) bss_0[sp108]->unk9C[sp104].unk14, (SRT* ) &bss_0[sp108]->unkC, 0x10002, -1, NULL);
                                    } else {
                                        dll_partfx->spawn(bss_0[sp108]->unk4, (s32) bss_0[sp108]->unk9C[sp104].unk14, NULL, 0x10002, -1, NULL);
                                    }
                                }
                            } else if (bss_0[sp108]->unk9C[sp104].unk4.y == 1.0f) {
                                if (!(bss_0[sp108]->unkA4 & 1)) {
                                    spCC.transl.x = bss_0[sp108]->unk4->globalPosition.x + bss_0[sp108]->unk60;
                                    spCC.transl.y = bss_0[sp108]->unk4->globalPosition.y + bss_0[sp108]->unk64;
                                    spCC.transl.z = bss_0[sp108]->unk4->globalPosition.z + bss_0[sp108]->unk68;
                                    if (bss_0[sp108]->unk4 != NULL) {
                                        dll_partfx->spawn(bss_0[sp108]->unk4, (s32) bss_0[sp108]->unk9C[sp104].unk14, &spCC, 0x10001, -1, NULL);
                                    }
                                } else {
                                    spCC.transl.x = bss_0[sp108]->unk60;
                                    spCC.transl.y = bss_0[sp108]->unk64;
                                    spCC.transl.z = bss_0[sp108]->unk68;
                                    if (bss_0[sp108]->unk4 != NULL) {
                                        dll_partfx->spawn(bss_0[sp108]->unk4, (s32) bss_0[sp108]->unk9C[sp104].unk14, &spCC, 0x10001, -1, NULL);
                                    }
                                }
                            }
                        }
                    }
                    if (bss_0[sp108]->unk9C[sp104].unk0 & 0x04000000) {
                        modgfx = dllLoad(bss_0[sp108]->unk9C[sp104].unk14 + 0x1000, 1);
                        if (bss_0[sp108]->unk9C[sp104].unk0 & 0x01000000) {
                            for (var_s0_2 = 0; var_s0_2 < (s32) bss_0[sp108]->unk9C[sp104].unk4.x; var_s0_2++) {
                                if (mathRnd(0, 5) == 0) {
                                    if (bss_0[sp108]->unkA4 & 1) {
                                        modgfx->vtbl->Func0(NULL, 0, &bss_0[sp108]->unkC, 1, -1, NULL);
                                    } else {
                                        modgfx->vtbl->Func0(bss_0[sp108]->unk4, 0, NULL, 1, -1, NULL);
                                    }
                                }
                            }
                        } else {
                            for (var_s0_2 = 0; var_s0_2 < (s32) bss_0[sp108]->unk9C[sp104].unk4.x; var_s0_2++) {
                                if (bss_0[sp108]->unkA4 & 1) {
                                    modgfx->vtbl->Func0(NULL, 0, &bss_0[sp108]->unkC, 1, -1, 0);
                                } else {
                                    modgfx->vtbl->Func0(bss_0[sp108]->unk4, 0, NULL, 1, -1, 0);
                                }
                            }
                        }
                        dllFree(modgfx);
                    }
                }
                }
                if (spF0 == 0) {
                    bss_0[i]->unkFE -= gUpdateRate;
                }
            }
        } while (spEC != 0);
    }
    D_8008C504 = 0;
}

// offset: 0x21F0 | func: 3 | export: 3
void modgfx_Func3(void) {
    modgfx_func_4C0C(0, 1);
}

// offset: 0x2234 | func: 4 | export: 4
void modgfx_Func4(Object* arg0) {
    s32 i;
    SRT sp5C;

    for (i = 0; i < ARRAYCOUNT(bss_0); i++) {
        if (bss_0[i] != NULL && arg0 == bss_0[i]->unk4) {
            if (bss_0[i]->unkA0 != NULL) {
                bss_0[i]->unkA0->unk12 = 2;
                bss_0[i]->unkA0->unke = 0;
                bss_0[i]->unkA0->unk1b = 0;
                bss_0[i]->unkA0->unk10 = bss_0[i]->unk4->unkD6;
                dll_newlfx->DoAction(bss_0[i]->unk4, bss_0[i]->unk4, bss_0[i]->unkA0, 0, 0, &sp5C);
                mmFree(bss_0[i]->unkA0);
            }
            if (bss_0[i]->unk10E != -1) {
                lfxAction(bss_0[i]->unk4, NULL, bss_0[i]->unk10E, 0, 0, NULL);
            }
            if (bss_0[i]->unk0 != NULL) {
                objFreeObject(bss_0[i]->unk0);
            }
            mmFree(bss_0[i]->unk12C);
            bss_0[i]->unk12C = NULL;
            if (bss_0[i]->unk13F == 0) {
                if (bss_0[i]->unk98 != NULL) {
                    texFreeTexture(bss_0[i]->unk98);
                }
            }
            if (bss_0[i]->unk13F == 0) {
                bss_0[i]->unk98 = NULL;
            }
            if (bss_0[i]->unk8 != NULL) {
                mmFree(bss_0[i]->unk8);
            }
            if (bss_0[i]->unk9C != NULL) {
                mmFree(bss_0[i]->unk9C);
            }
            mmFree(bss_0[i]);
            bss_0[i] = NULL;
        }
    }
}

// offset: 0x2474 | func: 5 | export: 5
void modgfx_Func5(Object* arg0) {
    s32 i;
    s32 newI;

    for (i = 0; i < ARRAYCOUNT(bss_0); i++) {
        newI = i;
        if (bss_0[newI] != NULL && arg0 == bss_0[newI]->unk4) {
            if (bss_0[newI]->unkA4 & 0x10000) {
                modgfx_func_4C0C(bss_0[newI]->unk10C, 0);
            } else {
                bss_0[newI]->unkC.transl.x = bss_0[newI]->unk4->globalPosition.x;
                bss_0[newI]->unkC.transl.y = bss_0[newI]->unk4->globalPosition.y;
                bss_0[newI]->unkC.transl.z = bss_0[newI]->unk4->globalPosition.z;
                bss_0[newI]->unkC.scale = bss_0[newI]->unk4->srt.scale;
                bss_0[newI]->unkC.roll = bss_0[newI]->unk4->srt.roll;
                bss_0[newI]->unkC.pitch = bss_0[newI]->unk4->srt.pitch;
                bss_0[newI]->unkC.yaw = bss_0[newI]->unk4->srt.yaw;
                if (bss_0[newI]->unkA4 & 2) {
                    bss_0[newI]->unk6C.x += bss_0[newI]->unk4->velocity.x;
                    bss_0[newI]->unk6C.y += bss_0[newI]->unk4->velocity.y;
                    bss_0[newI]->unk6C.z += bss_0[newI]->unk4->velocity.z;
                }
                if (!(bss_0[newI]->unkA4 & 0x200000)) {
                    bss_0[newI]->unkA4 |= 0x200000;
                }
                bss_0[newI]->unk4 = NULL;
            }
        }
    }
}

// offset: 0x2618 | func: 6 | export: 6
s32 modgfx_Func6(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, u8 arg3, Object* obj) {
    s32 idx;
    s32 i;
    DLTri* var_s6;
    s32 texFormat = 0;
    s32 counter = 0;
    s32 tmem = 0;
    f32 magnitude;
    s32 var_s0;
    s32 var_s3;
    Vtx* var_s7;
    SRT srt;
    f32 sp284[3];
    f32 sp26C[6];
    s32 frameIdx;
    Vec3f sp25C;
    Camera* camera;
    u8 mixFrameNo = 0;
    u8 baseFrameNo = 0;
    u8 maskt = 0;
    u8 masks = 0;
    s32 someVar;
    Texture *baseTex;
    Texture* mixTex;
    s16 sp234[] = {
        0x0003, 0x0002, 0x0401, 0x0400, 0x0302, 0x0301, 0x0300, 0x0200, 0x0201
    };
    u8 sp233;
    u8 sp232;
    u8 sp231;

    lightGetAmbient(&sp233, &sp232, &sp231);
    if (func_80000824(-1) == 1) {
        return 1;
    }
    camera = camGet();
    for (i = 0; i < ARRAYCOUNT(bss_0); i++) {
        if (bss_0[i] == NULL) {
            continue;
        }
        if (bss_0[i]->unk10C == -1) {
            continue;
        }
        if (arg3 && !(bss_0[i]->unkA4 & 0x2000)) {
            continue;
        }
        if (arg3 && (obj != bss_0[i]->unk4)) {
            continue;
        }
        if (!arg3 && (bss_0[i]->unkA4 & 0x2000)) {
            continue;
        }
        
        if (bss_0[i]->unkA4 & 0x800) {
            bss_0[i]->unk13E = 0;
        }

        counter++;

        idx = i;
        someVar = 0;

        var_s7 = bss_0[idx]->unk78[bss_0[idx]->unk130];
        if (bss_0[idx]->unkA4 & 0x40000) {
            modgfx_func_4EDC(bss_0[idx], 0);
        }
        var_s6 = bss_0[idx]->unk84[bss_0[idx]->unk130];
        srt.transl.x = 0.0f;
        srt.transl.y = 0.0f;
        srt.transl.z = 0.0f;
        srt.scale = 1.0f;
        srt.roll = 0;
        srt.pitch = 0;
        sp284[0] = bss_0[idx]->unk60;
        sp284[1] = bss_0[idx]->unk64;
        sp284[2] = bss_0[idx]->unk68;
        if ((bss_0[idx]->unkA4 & 4) && ((sp284[0] + sp284[1] + sp284[2]) == 0.0f)) {
            someVar = 1;
        }
        if ((bss_0[idx]->unkA4 & 4) && (someVar == 0)) {
            if (bss_0[idx]->unk4 != NULL) {
                srt.yaw = bss_0[idx]->unk4->srt.yaw;
                srt.pitch = bss_0[idx]->unk4->srt.pitch;
                srt.roll = bss_0[idx]->unk4->srt.roll;
                mathRotateRPY(&srt, sp284);
            }
        }
        sp25C.x = 0.0f;
        sp25C.y = 0.0f;
        sp25C.z = 0.0f;
        if (!(bss_0[idx]->unkA4 & 1)) {
            if (bss_0[idx]->unk4 != NULL) {
                sp25C.x = bss_0[idx]->unk4->globalPosition.x;
                sp25C.y = bss_0[idx]->unk4->globalPosition.y;
                sp25C.z = bss_0[idx]->unk4->globalPosition.z;
            } else {
                sp25C.x = bss_0[idx]->unkC.transl.x;
                sp25C.y = bss_0[idx]->unkC.transl.y;
                sp25C.z = bss_0[idx]->unkC.transl.z;
                camTransformPointByObjectMatrix(&bss_0[idx]->unkC.transl, &sp25C, bss_0[idx]->unk135);
            }
        }
        if ((sp25C.x > 65534.0f) || (sp25C.x < -65534.0f)) {
            STUBBED_PRINTF("\t modgfx x %f\n", &sp25C.x);
            sp25C.x = -gWorldX;
        }
        if ((sp25C.y > 65534.0f) || (sp25C.y < -65534.0f)) {
            sp25C.y = 0.0f;
        }
        if ((sp25C.z > 65534.0f) || (sp25C.z < -65534.0f)) {
            STUBBED_PRINTF("\t modgfx z %f\n", &sp25C.z);
            sp25C.z = -gWorldZ;
        }
        srt.transl.x = sp284[0] + sp25C.x;
        srt.transl.y = sp284[1] + sp25C.y;
        srt.transl.z = sp284[2] + sp25C.z;
        if (bss_0[idx]->unkA4 & 0x400000) {
            srt.scale = (bss_0[idx]->unkD4 * 0.5f) + ((bss_0[idx]->unkD4 * 0.5f) / (f32) mathRnd(1, 10));
        } else {
            srt.scale = bss_0[idx]->unkD4 * 0.01f;
        }
        if (bss_0[idx]->unkA4 & 0x80000) {
            srt.roll = bss_0[idx]->unk4->srt.roll;
            srt.pitch = bss_0[idx]->unk4->srt.pitch;
            srt.yaw = bss_0[idx]->unk4->srt.yaw;
        } else if (someVar != 0 && bss_0[idx]->unk4 != NULL) {
            srt.roll = bss_0[idx]->unk106 + bss_0[idx]->unk4->srt.roll;
            srt.pitch = bss_0[idx]->unk108 + bss_0[idx]->unk4->srt.pitch;
            srt.yaw = bss_0[idx]->unk10A + bss_0[idx]->unk4->srt.yaw;
        } else if (someVar != 0) {
            srt.roll = bss_0[idx]->unk106 + bss_0[idx]->unkC.roll;
            srt.pitch = bss_0[idx]->unk108 + bss_0[idx]->unkC.pitch;
            srt.yaw = bss_0[idx]->unk10A + bss_0[idx]->unkC.yaw;
        } else {
            srt.roll = bss_0[idx]->unk106;
            srt.pitch = bss_0[idx]->unk108;
            srt.yaw = bss_0[idx]->unk10A;
        }
        if ((bss_0[idx]->unkA4 & 0x1000) && (bss_0[idx]->unk4 != NULL)) {
            sp26C[3] = 0.0f; // wat
            sp26C[4] = 0.0f;
            sp26C[5] = -1.0f;
            sp26C[0] = camera->tx - bss_0[idx]->unk4->globalPosition.x;
            sp26C[1] = 0.0f;
            sp26C[2] = camera->tz - bss_0[idx]->unk4->globalPosition.z;
            magnitude = sqrtf(SQ(sp26C[0]) + SQ(sp26C[2]));
            if (magnitude != 0/*.0f*/) {
                sp26C[0] /= magnitude;
                sp26C[2] /= magnitude;
            }
            srt.yaw += (s16) (f32) mathAtan2f(sp26C[0], sp26C[2]);
        }
        camSetupObjectSRTMatrix(gdl, mtxs, &srt, 1.0f, 0/*.0f*/, NULL);
        if (bss_0[idx]->unk98 != NULL && bss_0[idx]->unk98->next != NULL && bss_0[idx]->unk132 != 0) {
            bss_0[idx]->unk133 -= 1;
            if (bss_0[idx]->unk133 <= 0) {
                bss_0[idx]->unk133 = 0x3C / (s32) bss_0[idx]->unk132;
                bss_0[idx]->unk131++;
                if (bss_0[idx]->unk131 >= (bss_0[idx]->unk98->animDuration >> 8)) {
                    bss_0[idx]->unk131 = 0;
                }
            }
        }
        if (bss_0[idx]->unk98 != NULL) {
            texFormat = TEX_FORMAT(bss_0[idx]->unk98->format);
        }
        gSPLoadGeometryMode(*gdl, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
        dlApplyGeometryMode(gdl);
        if (bss_0[idx]->unkA4 & 0x10000000) {
            dlSetPrimColor(gdl, sp233, sp232, sp231, 255);
        } else if ((bss_0[idx]->unk4 != NULL) && (bss_0[idx]->unkA4 & 0x4000)) {
            dlSetPrimColor(gdl, 255, 255, 255, bss_0[idx]->unk4->opacityWithFade);
        } else {
            dlSetPrimColor(gdl, 255, 255, 255, 255);
        }
        if (bss_0[idx]->unk98 != NULL) {
            tmem = bss_0[idx]->unk98->sizeBytes >> 3;
            baseFrameNo = bss_0[idx]->unk131;
            mixFrameNo = baseFrameNo + 1;
            if (mixFrameNo >= (bss_0[idx]->unk98->animDuration >> 8)) {
                mixFrameNo = 0;
            }
            if (bss_0[idx]->unk98->sizeBytes > 0x800) {
                if (bss_0[idx]->unkA4 & 0x01000000) {
                    // STUBBED_PRINTF("modgfx can't TEXMIX framesize %d\n", 0); // default.dol
                    bss_0[idx]->unkA4 ^= 0x01000000;
                    bss_0[idx]->unkA4 |= 0x04000000;
                }
            }
            if (bss_0[idx]->unkA4 & 0x05000000) {
                switch (bss_0[idx]->unk98->width) {
                    case 128:
                        masks = 7;
                        break;
                    case 64:
                        masks = 6;
                        break;
                    case 32:
                        masks = 5;
                        break;
                    case 16:
                        masks = 4;
                        break;
                    case 8:
                        masks = 3;
                        break;
                }
                switch (bss_0[idx]->unk98->height) {
                    case 128:
                        maskt = 7;
                        break;
                    case 64:
                        maskt = 6;
                        break;
                    case 32:
                        maskt = 5;
                        break;
                    case 16:
                        maskt = 4;
                        break;
                    case 8:
                        maskt = 3;
                    break;
                }
            }
        }
        if ((bss_0[idx]->unkA4 & 0x01000000) && ((bss_0[idx]->unk13E != 0) || (bss_0[idx]->unkA4 & 0x400))) {
            mixTex = bss_0[idx]->unk98;
            for (frameIdx = 0; frameIdx < mixFrameNo; frameIdx++) {
                mixTex = mixTex->next;
            }
            dlSetEnvColor(gdl, 0xFF, 0xFF, 0xFF, 0xFF - (bss_0[idx]->unk133 * bss_0[idx]->unk134));
            gDPSetCombineLERP(*gdl,
                TEXEL1, TEXEL0, ENV_ALPHA, TEXEL0, TEXEL1, TEXEL0, ENVIRONMENT, TEXEL0, 
                COMBINED, 0, SHADE, 0, COMBINED, 0, SHADE, 0);
            dlApplyCombine(gdl);
            switch (texFormat) {
            case TEX_FORMAT_RGBA32:
                gDPLoadMultiBlockS((*gdl)++,
                    /*timg*/mixTex + 1,
                    /*tmem*/tmem,
                    /*rtile*/1,
                    /*fmt*/G_IM_FMT_RGBA,
                    /*siz*/G_IM_SIZ_32b,
                    /*width*/mixTex->width,
                    /*height*/mixTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_RGBA16:
                gDPLoadMultiBlockS((*gdl)++,
                    /*timg*/mixTex + 1,
                    /*tmem*/tmem,
                    /*rtile*/1,
                    /*fmt*/G_IM_FMT_RGBA,
                    /*siz*/G_IM_SIZ_16b,
                    /*width*/mixTex->width,
                    /*height*/mixTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_I8:
                gDPLoadMultiBlockS((*gdl)++,
                    /*timg*/mixTex + 1,
                    /*tmem*/tmem,
                    /*rtile*/1,
                    /*fmt*/G_IM_FMT_I,
                    /*siz*/G_IM_SIZ_8b,
                    /*width*/mixTex->width,
                    /*height*/mixTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_I4:
                gDPLoadMultiBlockS((*gdl)++,
                    /*timg*/mixTex + 1,
                    /*tmem*/tmem,
                    /*rtile*/1,
                    /*fmt*/G_IM_FMT_I,
                    /*siz*/G_IM_SIZ_4b,
                    /*width*/mixTex->width,
                    /*height*/mixTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_IA16:
                gDPLoadMultiBlockS((*gdl)++,
                    /*timg*/mixTex + 1,
                    /*tmem*/tmem,
                    /*rtile*/1,
                    /*fmt*/G_IM_FMT_IA,
                    /*siz*/G_IM_SIZ_16b,
                    /*width*/mixTex->width,
                    /*height*/mixTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_IA8:
                gDPLoadMultiBlockS((*gdl)++,
                    /*timg*/mixTex + 1,
                    /*tmem*/tmem,
                    /*rtile*/1,
                    /*fmt*/G_IM_FMT_IA,
                    /*siz*/G_IM_SIZ_8b,
                    /*width*/mixTex->width,
                    /*height*/mixTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_IA4:
                gDPLoadMultiBlockS((*gdl)++,
                    /*timg*/mixTex + 1,
                    /*tmem*/tmem,
                    /*rtile*/1,
                    /*fmt*/G_IM_FMT_IA,
                    /*siz*/G_IM_SIZ_4b,
                    /*width*/mixTex->width,
                    /*height*/mixTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            }
        } else if (bss_0[idx]->unkA4 & 0x02000000) {
            gDPSetCombineLERP(*gdl, 1, 0, SHADE, 0, 1, 0, SHADE, 0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0);
            dlApplyCombine(gdl);
        } else if (bss_0[idx]->unkA4 & 0x04000000) {
            gDPSetCombineMode(*gdl, G_CC_MODULATEIA, G_CC_MODULATEIA_PRIM2);
            dlApplyCombine(gdl);
        }
        var_s3 = 0;
        if ((bss_0[idx]->unkA4 & 0x05000000) && ((bss_0[idx]->unk13E != 0) || (bss_0[idx]->unkA4 & 0x400))) {
            baseTex = bss_0[idx]->unk98;
            for (frameIdx = 0; frameIdx < baseFrameNo; frameIdx++) {
                baseTex = baseTex->next;
            }
            switch (texFormat) {
            case TEX_FORMAT_RGBA32:
                gDPLoadTextureBlockS((*gdl)++,
                    /*timg*/baseTex + 1,
                    /*fmt*/G_IM_FMT_RGBA,
                    /*siz*/G_IM_SIZ_32b,
                    /*width*/baseTex->width,
                    /*height*/baseTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_RGBA16:
                gDPLoadTextureBlockS((*gdl)++,
                    /*timg*/baseTex + 1,
                    /*fmt*/G_IM_FMT_RGBA,
                    /*siz*/G_IM_SIZ_16b,
                    /*width*/baseTex->width,
                    /*height*/baseTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_I8:
                gDPLoadTextureBlockS((*gdl)++,
                    /*timg*/baseTex + 1,
                    /*fmt*/G_IM_FMT_I,
                    /*siz*/G_IM_SIZ_8b,
                    /*width*/baseTex->width,
                    /*height*/baseTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_I4:
                gDPLoadTextureBlockS((*gdl)++,
                    /*timg*/baseTex + 1,
                    /*fmt*/G_IM_FMT_I,
                    /*siz*/G_IM_SIZ_4b,
                    /*width*/baseTex->width,
                    /*height*/baseTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_IA16:
                gDPLoadTextureBlockS((*gdl)++,
                    /*timg*/baseTex + 1,
                    /*fmt*/G_IM_FMT_IA,
                    /*siz*/G_IM_SIZ_16b,
                    /*width*/baseTex->width,
                    /*height*/baseTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_IA8:
                gDPLoadTextureBlockS((*gdl)++,
                    /*timg*/baseTex + 1,
                    /*fmt*/G_IM_FMT_IA,
                    /*siz*/G_IM_SIZ_8b,
                    /*width*/baseTex->width,
                    /*height*/baseTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            case TEX_FORMAT_IA4:
                gDPLoadTextureBlockS((*gdl)++,
                    /*timg*/baseTex + 1,
                    /*fmt*/G_IM_FMT_IA,
                    /*siz*/G_IM_SIZ_4b,
                    /*width*/baseTex->width,
                    /*height*/baseTex->height,
                    /*pal*/0,
                    /*cms*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*cmt*/G_TX_NOMIRROR | G_TX_WRAP,
                    /*masks*/masks,
                    /*maskt*/maskt,
                    /*shifts*/G_TX_NOLOD,
                    /*shiftt*/G_TX_NOLOD
                );
                break;
            default:
                texDPTextures(gdl, baseTex, NULL, 0, 0, 0, TRUE);
                var_s3 = 1;
                break;
            }
        }
        if (var_s3 == 0) {
            if (bss_0[idx]->unkA4 & 0x100) {
                gDPSetOtherMode(
                    *gdl,
                    G_AD_PATTERN | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_AA_ZB_XLU_INTER2
                );
                dlApplyOtherMode(gdl);
            } else if ((bss_0[idx]->unkA4 & 0x10) && (bss_0[idx]->unkA4 & 0x80)) {
                gDPSetOtherMode(
                    *gdl,
                    G_AD_PATTERN | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_AA_XLU_SURF2
                );
                dlApplyOtherMode(gdl);
            } else if (bss_0[idx]->unkA4 & 0x80) {
                gDPSetOtherMode(
                    *gdl,
                    G_AD_PATTERN | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_AA_ZB_XLU_SURF2
                );
                dlApplyOtherMode(gdl);
            } else if (bss_0[idx]->unkA4 & 0x10) {
                gDPSetOtherMode(
                    *gdl,
                    G_AD_PATTERN | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_CLD_SURF2
                );
                dlApplyOtherMode(gdl);
            } else {
                gDPSetOtherMode(
                    *gdl,
                    G_AD_PATTERN | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_ZB_CLD_SURF2
                );
                dlApplyOtherMode(gdl);
            }
        }
        if ((bss_0[idx]->unk13E != 0) || (bss_0[idx]->unkA4 & 0x400)) {
            for (var_s0 = 0; var_s0 < bss_0[idx]->unk136; var_s0++) {
                gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(var_s7), bss_0[idx]->unk138, 0);
                if (bss_0[idx]->unkA4 & 0x08000000) {
                    dlTriangles(gdl, var_s6, bss_0[idx]->unkEC / bss_0[idx]->unk136);
                } else {
                    dlTriangles(gdl, var_s6, bss_0[idx]->unkEC);
                }
                var_s7 += bss_0[idx]->unk137;
                if (bss_0[idx]->unkA4 & 0x08000000) {
                    var_s6 += bss_0[idx]->unkEC / bss_0[idx]->unk136;
                }
            }
        }
        texRenderReset();
        bss_0[idx]->unk130 = 1 - bss_0[idx]->unk130;
    }
    return 0;
}

static s32 data_28 = 0; // unused?
static s16 data_2C = 0;

// offset: 0x4854 | func: 7 | export: 7
void modgfx_Func7(s16* arg0) {
    s32 i;

    for (i = 0; i < ARRAYCOUNT(bss_0); i++) {
        if (bss_0[i] != NULL && bss_0[i]->unk10C == *arg0) {
            bss_0[i]->unk13A = 1;
        }
    }

    *arg0 = -1;
}

// offset: 0x4910 | func: 8 | export: 8
void modgfx_Func8(void) {
    data_8++;
}

// offset: 0x4938 | func: 9 | export: 9
void modgfx_Func9(Object* arg0, u8 arg1) {
    s32 i;

    for (i = 0; i < ARRAYCOUNT(bss_0); i++) {
        if (bss_0[i] != NULL && arg0 == bss_0[i]->unk4) {
            bss_0[i]->unk13B = arg1;
        }
    }
}

// offset: 0x49E4 | func: 10 | export: 10
void modgfx_Func10(Object* arg0) {
    s32 i;

    for (i = 0; i < ARRAYCOUNT(bss_0); i++) {
        if (bss_0[i] != NULL && arg0 == bss_0[i]->unk4) {
            bss_0[i]->unk13A = 1;
        }
    }
}

// offset: 0x4A88 | func: 11 | export: 11
void modgfx_Func11(s8* arg0) {
    s32 i;
    s32 newI;
    s32 objectCount;
    s32 j;
    s32 stop;

    objectCount = objGetNumObjects();
    for (i = 0; i < ARRAYCOUNT(bss_0); i++) {
        newI = i;
        if (bss_0[newI] != NULL && bss_0[newI]->unk4 != NULL) {
            j = 0;
            stop = TRUE;
            while (j < objectCount && stop != FALSE) {
                if (objGetObject(j) == bss_0[newI]->unk4) {
                    stop = FALSE;
                }
                j++;
            }

            j--;
            if (stop == FALSE && arg0[j] != 0) {
                bss_0[newI]->unk13E = 1;
            }
        }
    }
}

// offset: 0x4BA4 | func: 12
static s32 modgfx_func_4BA4(void) {
    s32 isFree;
    s32 freeIdx;

    freeIdx = 0;
    isFree = FALSE;
    while (freeIdx < ARRAYCOUNT_S(bss_0) && isFree == FALSE) {
        if (bss_0[freeIdx] == NULL) {
            isFree = TRUE;
        }
        freeIdx++;
    }
    freeIdx--;
    if (isFree == FALSE) {
        return -1;
    }
    return freeIdx;
}

// offset: 0x4C0C | func: 13
static void modgfx_func_4C0C(s16 arg0, s32 arg1) {
    s32 i;
    SRT sp5C;

    for (i = 0; i < ARRAYCOUNT(bss_0); i++) {
        if ((bss_0[i] != NULL) && ((arg0 == bss_0[i]->unk10C) || (arg1 != 0))) {
            if (bss_0[i]->unkA0 != NULL) {
                bss_0[i]->unkA0->unk12 = 2;
                bss_0[i]->unkA0->unke = 0;
                bss_0[i]->unkA0->unk1b = 0;
                dll_newlfx->DoAction(bss_0[i]->unk4, bss_0[i]->unk4, bss_0[i]->unkA0, 0, 0, &sp5C);
                mmFree(bss_0[i]->unkA0);
            }
            if (bss_0[i]->unk10E != -1) {
                lfxAction(bss_0[i]->unk4, NULL, bss_0[i]->unk10E, 0, 0, 0);
            }
            if (bss_0[i]->unk0 != NULL) {
                objFreeObject(bss_0[i]->unk0);
            }
            mmFree(bss_0[i]->unk12C);
            bss_0[i]->unk12C = NULL;
            if (bss_0[i]->unk13F == 0) {
                if (bss_0[i]->unk98 != NULL) {
                    texFreeTexture(bss_0[i]->unk98);
                }
            }
            if (bss_0[i]->unk13F == 0) {
                bss_0[i]->unk98 = NULL;
            }
            if (bss_0[i]->unk8 != NULL) {
                mmFree(bss_0[i]->unk8);
            }
            if (bss_0[i]->unk9C != NULL) {
                mmFree(bss_0[i]->unk9C);
            }
            mmFree(bss_0[i]);
            bss_0[i] = NULL;
        }
    }
}

// offset: 0x4E58 | func: 14
static void modgfx_func_4E58(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2) {
    Vtx* var_v0;
    Vtx* var_v1;
    s32 i;

    var_v0 = arg0->unk78[arg0->unk130];
    var_v1 = arg0->unk78[2];
    for (i = 0; i < arg0->unkEA; i++) {
        var_v0->v.ob[0] = var_v1->v.ob[0];
        var_v0->v.ob[1] = var_v1->v.ob[1];
        var_v0->v.ob[2] = var_v1->v.ob[2];
        var_v0->v.cn[0] = var_v1->v.cn[0];
        var_v0->v.cn[1] = var_v1->v.cn[1];
        var_v0->v.cn[2] = var_v1->v.cn[2];
        var_v0->v.cn[3] = var_v1->v.cn[3];
        var_v0++;
        var_v1++;
    }
}

// offset: 0x4EDC | func: 15
static void modgfx_func_4EDC(ModgfxInstance* arg0, u8 arg1) {
    Vtx* var_s1;
    s32 var_s0;
    SRT sp38;

    var_s1 = arg0->unk78[arg0->unk130];
    sp38.transl.x = 0.0f;
    sp38.transl.y = 0.0f;
    sp38.transl.z = 0.0f;
    sp38.scale = 1.0f;
    sp38.yaw = 0;
    sp38.pitch = 0;
    sp38.roll = 0;
    if (arg0->unk4 != NULL) {
        sp38.yaw = 0;
        sp38.pitch = 0;
        if (!arg1) {
            sp38.roll = arg0->unk4->srt.yaw - arg0->unk110;
        } else {
            sp38.roll = arg0->unk4->srt.yaw;
        }
    }
    var_s0 = 0;
    while (var_s0 < arg0->unkEA) {
        mathYprCat(&sp38, var_s1->v.ob);
        var_s1++;
        var_s0++;
    }
    if (arg0->unk4 != NULL) {
        arg0->unk110 = arg0->unk4->srt.yaw;
    }
}

// offset: 0x4FF4 | func: 16
static void modgfx_func_4FF4(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3) {
    Vtx* temp_v0;
    Vtx* temp_v1;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 temp = arg3 * 2;

    if (arg2 == 1) {
        temp_fv0 = arg1->unk4.x;
        temp_fv1 = arg1->unk4.y;
        temp_fa0 = arg1->unk4.z;
        if (arg0->unkFE != 0) {
            arg0->unk30[temp + 1].x = (temp_fv0 - arg0->unk30[temp].x) / arg0->unkFE;
            arg0->unk30[temp + 1].y = (temp_fv1 - arg0->unk30[temp].y) / arg0->unkFE;
            arg0->unk30[temp + 1].z = (temp_fa0 - arg0->unk30[temp].z) / arg0->unkFE;
        } else {
            temp_v0 = arg0->unk78[arg0->unk130];
            temp_v1 = arg0->unk78[2];
            for (arg2 = 0; arg2 < arg1->unk14; arg2++) {
                temp_v1[arg1->unk10[arg2]].v.ob[0] *= temp_fv0;
                temp_v1[arg1->unk10[arg2]].v.ob[1] *= temp_fv1;
                temp_v1[arg1->unk10[arg2]].v.ob[2] *= temp_fa0;
                temp_v0[arg1->unk10[arg2]].v.ob[0] = temp_v1[arg1->unk10[arg2]].v.ob[0];
                temp_v0[arg1->unk10[arg2]].v.ob[1] = temp_v1[arg1->unk10[arg2]].v.ob[1];
                temp_v0[arg1->unk10[arg2]].v.ob[2] = temp_v1[arg1->unk10[arg2]].v.ob[2];
            }
            return;
        }
    }

    arg0->unk30[temp].x += arg0->unk30[temp + 1].x * data_C;
    arg0->unk30[temp].y += arg0->unk30[temp + 1].y * data_C;
    arg0->unk30[temp].z += arg0->unk30[temp + 1].z * data_C;
    temp_v1 = arg0->unk78[2];
    temp_v0 = arg0->unk78[arg0->unk130];
    for (arg2 = 0; arg2 < arg1->unk14; arg2++) {
        if (arg0->unk30[temp].x != 1.0f) {
            temp_v0[arg1->unk10[arg2]].v.ob[0] = temp_v1[arg1->unk10[arg2]].v.ob[0] * arg0->unk30[temp].x;
        }
        if (arg0->unk30[temp].y != 1.0f) {
            temp_v0[arg1->unk10[arg2]].v.ob[1] = temp_v1[arg1->unk10[arg2]].v.ob[1] * arg0->unk30[temp].y;
        }
        if (arg0->unk30[temp].z != 1.0f) {
            temp_v0[arg1->unk10[arg2]].v.ob[2] = temp_v1[arg1->unk10[arg2]].v.ob[2] * arg0->unk30[temp].z;
        }
    }
}

// offset: 0x538C | func: 17
static void modgfx_func_538C(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3) {
    Vtx* temp_v0;
    Vtx* temp_v1;
    f32 temp_fv0;
    s32 i;
    s32 temp = arg3 * 2;

    temp_v0 = arg0->unk78[arg0->unk130];
    temp_v1 = arg0->unk78[2];
    if (arg2 == 1) {
        temp_fv0 = arg1->unk4.x;
        if (arg0->unkFE != 0) {
            arg0->unkAC[temp] = (temp_fv0 - temp_v1[arg1->unk10[0]].v.cn[3]) / arg0->unkFE;
            arg0->unkAC[temp + 1] = temp_v1[arg1->unk10[0]].v.cn[3];
        } else {
            for (i = 0; i < arg1->unk14; i++) {
                temp_v1[arg1->unk10[i]].v.cn[3] = temp_fv0;
                temp_v0[arg1->unk10[i]].v.cn[3] = temp_v1[arg1->unk10[i]].v.cn[3];
            }
            return;
        }
    }

    arg0->unkAC[temp + 1] += arg0->unkAC[temp] * data_C;
    if (arg0->unkAC[temp + 1] < 0.0f) {
        arg0->unkAC[temp + 1] = 0.0f;
    } else if (arg0->unkAC[temp + 1] > 255.0f) {
        arg0->unkAC[temp + 1] = 255.0f;
    }

    for (i = 0; i < arg1->unk14; i++) {
        temp_v0[*(arg1->unk10 + i)].v.cn[3] = arg0->unkAC[temp + 1];
        temp_v1[arg1->unk10[i]].v.cn[3] = temp_v0[arg1->unk10[i]].v.cn[3];
    }
}

// offset: 0x56A0 | func: 18
static void modgfx_func_56A0(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3) {
    if (arg2 == 1) {
        if (arg0->unkFE != 0) {
            arg0->unkBC = (arg1->unk4.x - arg0->unk4->opacity) /  arg0->unkFE;
            arg0->unkC0 = arg0->unk4->opacity;
        } else {
            arg0->unkBC = arg1->unk4.x - arg0->unk4->opacity;
            arg0->unkC0 = 0.0f;
        }
    }

    arg0->unkC0 += arg0->unkBC;
    if (arg0->unkC0 > 255.0f) {
        arg0->unkC0 = 255.0f;
    } else if (arg0->unkC0 < 0.0f) {
        arg0->unkC0 = 0.0f;
    }

    arg0->unk4->opacity = arg0->unkC0;
}

// offset: 0x584C | func: 19
static void modgfx_func_584C(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3) {
    s16 temp_ft1;
    s16 temp_ft3;
    s16 temp_ft5;

    if (arg2 == 1) {
        temp_ft1 = arg1->unk4.x;
        temp_ft3 = arg1->unk4.y;
        temp_ft5 = arg1->unk4.z;
        if (arg0->unkFE != 0) {
            arg0->unk100 = (temp_ft1 - arg0->unk106) / arg0->unkFE;
            arg0->unk102 = (temp_ft3 - arg0->unk108) / arg0->unkFE;
            arg0->unk104 = (temp_ft5 - arg0->unk10A) / arg0->unkFE;
        } else {
            arg0->unk106 = temp_ft1;
            arg0->unk100 = 0;
            arg0->unk108 = temp_ft3;
            arg0->unk102 = 0;
            arg0->unk10A = temp_ft5;
            arg0->unk104 = 0;
        }
    }

    arg0->unk106 += arg0->unk100;
    arg0->unk108 += arg0->unk102;
    arg0->unk10A += arg0->unk104;
}

// offset: 0x59B4 | func: 20
static void modgfx_func_59B4(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3) {
    f32 var_fa1;
    Vec3f sp40;
    SRT sp28;

    if (arg2 == 1) {
        arg0->unk120 = arg0->unkA0->unk4;
        arg0->unk122 = arg0->unkA0->unk6;
        arg0->unk124 = arg0->unkA0->unk8;
        data_10 = 0.0f;
        if (arg0->unkFE != 0) {
            data_10 = 1.0f / arg0->unkFE;
        }
    }

    if (arg0->unkEE[arg0->unkFC] == 0) {
        var_fa1 = 1.0f;
    } else {
        var_fa1 = 1.0f - ((f32) arg0->unkFE / (f32) arg0->unkEE[arg0->unkFC]);
    }

    arg0->unkA0->unk4 = (((arg1->unk4.x - arg0->unk120) * var_fa1) + arg0->unk120);
    arg0->unkA0->unk6 = (((arg1->unk4.y - arg0->unk122) * var_fa1) + arg0->unk122);
    arg0->unkA0->unk8 = (((arg1->unk4.z - arg0->unk124) * var_fa1) + arg0->unk124);
    if (D_80092C3C > 0.0f && D_80092C3C <= 1.0f) {
        if (arg0->unkA4 & 4) {
            sp40.x = arg0->unkA0->unk4;
            sp40.y = arg0->unkA0->unk6;
            sp40.z = arg0->unkA0->unk8;
            sp28.transl.x = 0.0f;
            sp28.transl.y = 0.0f;
            sp28.transl.z = 0.0f;
            sp28.roll = 0;
            sp28.pitch = 0;
            sp28.scale = 1.0f;
            sp28.yaw = arg0->unk4->srt.yaw;
            mathRotateRPY(&sp28, sp40.f);
            arg0->unkA0->unk4 = sp40.x;
            arg0->unkA0->unk6 = sp40.y;
            arg0->unkA0->unk8 = sp40.z;
        }
        D_800BB198.x = arg0->unkA0->unk4 + arg0->unk4->globalPosition.x;
        D_800BB198.y = arg0->unkA0->unk6 + arg0->unk4->globalPosition.y;
        D_800BB198.z = arg0->unkA0->unk8 + arg0->unk4->globalPosition.z;
        D_80092C3C = D_80092C3C - (data_10 * data_C);
        return;
    }

    if (D_80092C3C == 2.0f) {
        if (arg0->unkA4 & 4) {
            sp40.x = arg0->unkA0->unk4;
            sp40.y = arg0->unkA0->unk6;
            sp40.z = arg0->unkA0->unk8;
            sp28.transl.x = 0.0f;
            sp28.transl.y = 0.0f;
            sp28.transl.z = 0.0f;
            sp28.roll = 0;
            sp28.pitch = 0;
            sp28.scale = 1.0f;
            sp28.yaw = arg0->unk4->srt.yaw;
            mathRotateRPY(&sp28, sp40.f);
            arg0->unkA0->unk4 = sp40.x;
            arg0->unkA0->unk6 = sp40.y;
            arg0->unkA0->unk8 = sp40.z;
        }
        D_800BB198.x = arg0->unkA0->unk4 + arg0->unk4->globalPosition.x;
        D_800BB198.y = arg0->unkA0->unk6 + arg0->unk4->globalPosition.y;
        D_800BB198.z = arg0->unkA0->unk8 + arg0->unk4->globalPosition.z;
    }
}

// offset: 0x5E50 | func: 21
void modgfx_func_5E50(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3) {
    SRT sp28;
    f32 temp_fv0;
    s32 temp_v0;

    if (arg2 == 1) {
        if (arg0->unkEE[arg0->unkFC] == 0) {
            temp_v0 = arg0->unkA4;
            if ((temp_v0 & 4) || (temp_v0 & 0x80000)) {
                sp28.transl.x = 0.0f;
                sp28.transl.y = 0.0f;
                sp28.transl.z = 0.0f;
                sp28.scale = 1.0f;
                sp28.yaw = arg0->unk4->srt.yaw;
                sp28.pitch = arg0->unk4->srt.yaw;
                sp28.roll = arg0->unk4->srt.yaw;
                mathRotateRPY(&sp28, arg1->unk4.f);
            }
            arg0->unk24.x = arg1->unk4.x;
            arg0->unk24.y = arg1->unk4.y;
            arg0->unk24.z = arg1->unk4.z;
        } else {
            arg0->unk24.x = arg1->unk4.x / arg0->unkFE;
            arg0->unk24.y = arg1->unk4.y / arg0->unkFE;
            arg0->unk24.z = arg1->unk4.z / arg0->unkFE;
        }
        arg0->unk60 += arg0->unk24.x;
        arg0->unk64 += arg0->unk24.y;
        arg0->unk68 += arg0->unk24.z;
    } else {
        arg0->unk60 += arg0->unk24.x * data_C;
        arg0->unk64 += arg0->unk24.y * data_C;
        arg0->unk68 += arg0->unk24.z * data_C;
    }
}

// offset: 0x5FFC | func: 22
void modgfx_func_5FFC(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3) {
    arg0->unk24.x += arg1->unk4.x * data_C;
    arg0->unk24.y += arg1->unk4.y * data_C;
    arg0->unk24.z += arg1->unk4.z * data_C;
}

// offset: 0x6068 | func: 23
void modgfx_func_6068(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3) {
    arg0->unk106 += (s16) (arg1->unk4.x * data_C);
    arg0->unk108 += (s16) (arg1->unk4.y * data_C);
    arg0->unk10A += (s16) (arg1->unk4.z * data_C);
}

// offset: 0x6100 | func: 24
void modgfx_func_6100(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3) {
    Vtx* temp_v0;
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 i;

    temp_v0 = arg0->unk78[arg0->unk130];
    if (arg2 == 1) {
        temp_fv0 = arg1->unk4.x;
        temp_fv1 = arg1->unk4.y;
        temp_fa0 = arg1->unk4.z;
        if (arg0->unkFE != 0) {
            arg0->unkBC = temp_v0[arg1->unk10[0]].v.cn[0];
            arg0->unkC0 = temp_v0[arg1->unk10[0]].v.cn[1];
            arg0->unkC4 = temp_v0[arg1->unk10[0]].v.cn[2];
            arg0->unkC8 = (temp_fv0 - temp_v0[arg1->unk10[0]].v.cn[0]) / arg0->unkFE;
            arg0->unkCC = (temp_fv1 - temp_v0[arg1->unk10[0]].v.cn[1]) / arg0->unkFE;
            arg0->unkD0 = (temp_fa0 - temp_v0[arg1->unk10[0]].v.cn[2]) / arg0->unkFE;
        } else {
            arg0->unkBC = temp_fv0;
            arg0->unkC0 = temp_fv1;
            arg0->unkC4 = temp_fa0;
            arg0->unkC8 = 0.0f;
            arg0->unkCC = 0.0f;
            arg0->unkD0 = 0.0f;
        }
    }

    arg0->unkBC += arg0->unkC8;
    arg0->unkC0 += arg0->unkCC;
    arg0->unkC4 += arg0->unkD0;

    if (arg0->unkBC < 0.0f) {
        arg0->unkBC = 0.0f;
    } else if (arg0->unkBC > 255.0f) {
        arg0->unkBC = 255.0f;
    }

    if (arg0->unkC0 < 0.0f) {
        arg0->unkC0 = 0.0f;
    } else if (arg0->unkC0 > 255.0f) {
        arg0->unkC0 = 255.0f;
    }

    if (arg0->unkC4 < 0.0f) {
        arg0->unkC4 = 0.0f;
    } else if (arg0->unkC4 > 255.0f) {
        arg0->unkC4 = 255.0f;
    }

    for (i = 0; i < arg1->unk14; i++) {
        temp_v0[arg1->unk10[i]].v.cn[0] = (s16) arg0->unkBC;
        temp_v0[arg1->unk10[i]].v.cn[1] = (s16) arg0->unkC0;
        temp_v0[arg1->unk10[i]].v.cn[2] = (s16) arg0->unkC4;
    }
}

// offset: 0x6424 | func: 25
static void modgfx_func_6424(ModgfxInstance* arg0, s32 arg1) {
    s32 i;
    Vtx* var_v1;
    Vtx* var_v0;

    var_v0 = arg0->unk78[1-arg0->unk130];
    var_v1 = arg0->unk78[2];
    for (i = 0; i < arg0->unkEA; i++) {
        var_v1->v.ob[0] = var_v0->v.ob[0];
        var_v1->v.ob[1] = var_v0->v.ob[1];
        var_v1->v.ob[2] = var_v0->v.ob[2];
        var_v1->v.cn[0] = var_v0->v.cn[0];
        var_v1->v.cn[1] = var_v0->v.cn[1];
        var_v1->v.cn[2] = var_v0->v.cn[2];
        var_v1->v.cn[3] = var_v0->v.cn[3];
        var_v1++;
        var_v0++;
    }

    arg0->unk30[0].x = 1.0f;
    arg0->unk30[0].y = 1.0f;
    arg0->unk30[0].z = 1.0f;

    arg0->unk30[1].x = 0.0f;
    arg0->unk30[1].y = 0.0f;
    arg0->unk30[1].z = 0.0f;

    arg0->unk30[2].x = 1.0f;
    arg0->unk30[2].y = 1.0f;
    arg0->unk30[2].z = 1.0f;

    arg0->unk30[3].x = 0.0f;
    arg0->unk30[3].y = 0.0f;
    arg0->unk30[3].z = 0.0f;
}

// offset: 0x64F0 | func: 26
static void modgfx_func_64F0(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, u8 arg3) {
    Vtx* var_v0;
    s32 temp_ft1;
    s32 temp_ft2;
    s32 temp_t6;
    s32 temp_t7;
    s32 pad;
    s32 i;
    u8 var_t0;
    u8 var_t1;
    Vtx* var_a2;
    f32 var_fa0;
    f32 var_ft4;

    var_t0 = 0;
    temp_ft1 = arg1->unk4.x * data_C;
    var_fa0 = (arg1->unk4.x - temp_ft1) * 32.0f;
    temp_ft2 = arg1->unk4.y * data_C;
    var_ft4 = (arg1->unk4.y - temp_ft2) * 32.0f;

    var_v0 = arg0->unk78[arg0->unk130];
    var_a2 = arg0->unk78[1 - arg0->unk130];
    temp_t6 = arg0->unk98->width << 6;
    temp_t7 = arg0->unk98->height << 6;
    var_t1 = 0;

    for (i = 0; i < arg0->unkEA; i++) {
        var_v0->v.tc[0] = var_a2->v.tc[0];
        var_v0->v.tc[1] = var_a2->v.tc[1];

        var_v0->v.tc[0] += (temp_ft1 << 5) + (s32)var_fa0;
        if (temp_t6 < var_v0->v.tc[0]) {
            var_t0++;
        }
        if (var_v0->v.tc[0] < -temp_t6) {
            var_t0++;
        }

        var_v0->v.tc[1] += (temp_ft2 << 5) + (s32)var_ft4;
        if (temp_t7 < var_v0->v.tc[1]) {
            var_t1++;
        }
        if (var_v0->v.tc[1] < -temp_t7) {
            var_t1++;
        }
        var_v0++;
        var_a2++;
    }

    var_v0 = arg0->unk78[arg0->unk130];
    for (i = 0; i < arg0->unkEA; i++) {
        if (var_t0 == arg0->unkEA) {
            if (temp_t6 < var_v0->v.tc[0]) {
                var_v0->v.tc[0] -= temp_t6;
            } else {
                var_v0->v.tc[0] += temp_t6;
            }
        }
        if (var_t1 == arg0->unkEA) {
            if (temp_t7 < var_v0->v.tc[1]) {
                var_v0->v.tc[1] -= temp_t7;
            } else {
                var_v0->v.tc[1] += temp_t7;
            }
        }
        var_v0++;
    }
}

// offset: 0x6758 | func: 27
static void modgfx_func_6758(ModgfxInstance* arg0, ModgfxStruct_0* arg1, s32 arg2, s32 arg3) {
    Object* temp_v0;
    SRT sp34;

    if (arg0->unk4 == NULL) {
        return;
    }

    temp_v0 = arg0->unk4;
    if ((arg2 == 1) && ((s32) arg3 != 0) && (arg1->unk0 & 0x8000)) {
        arg0->unkA0 = NULL;
        arg0->unkA0 = mmAlloc(sizeof(LightAction), ALLOC_TAG_LFX_COL, NULL);
        if (arg0->unkA0 != NULL) {
            if (arg1->unk0 & 0x02000000) {
                arg0->unkA0->unk12 = 0x19;
            } else {
                arg0->unkA0->unk12 = 0x1D;
            }
            arg0->unkA0->unk13 = 0;
            arg0->unkA0->unke = 0;
            arg0->unkA0->unk15 = 0;
            arg0->unkA0->unk16 = 0;
            arg0->unkA0->unk17 = 0;
            arg0->unkA0->unk18 = (s16) arg1->unk4.x;
            arg0->unkA0->unk19 = (s16) arg1->unk4.y;
            arg0->unkA0->unk1a = (s16) arg1->unk4.z;
            if (temp_v0->unkD6 == 0) {
                arg0->unkA0->unk10 = ~1;
            } else {
                arg0->unkA0->unk10 = temp_v0->unkD6;
            }
            arg0->unkA0->unk1b = 0;
            arg0->unkA0->unk1c = 1;
            arg0->unkA0->unk0 = 0;
            arg0->unkA0->unk2 = 0;
            arg0->unkA0->unk4 = 0;
            arg0->unkA0->unk6 = 0;
            arg0->unkA0->unk8 = 0;
            arg0->unkA0->unka = 0x51;
            arg0->unkA0->unkC = -0x15;
            arg0->unkA0->unk1d = 0xFF;
            arg0->unkA0->unk1e = 0x32;
            arg0->unkA0->unk22 = 8;
            arg0->unkA0->unk1f = 0xC;
            arg0->unkA0->unk20 = 4;
            arg0->unkA0->unk21 = 0;
            dll_newlfx->DoAction(temp_v0, temp_v0, arg0->unkA0, 0, 0, &sp34);
            temp_v0->unkD6 = arg0->unkA0->unk10;
            if (arg1->unk0 & 0x02000000) {
                D_80092C3C = 1.0f;
            }
        }
    } else {
        if (arg1->unk0 & 0x80000) {
            // FAKE
            if (1) {}
            if (arg0->unkA0 != NULL) {
                modgfx_func_59B4(arg0, arg1, arg3, 0U);
                arg0->unkA0->unk1b = 0;
                arg0->unkA0->unk12 = 1;
                arg0->unkA0->unk13 = 8;
                dll_newlfx->DoAction(temp_v0, temp_v0, arg0->unkA0, 0, 0, &sp34);
                arg0->unkA0->unk13 ^= 8;
                if (arg1->unk0 & 0x02000000) {
                    D_80092C3C = 2.0f;
                }
            }
        } else if (arg1->unk0 & 0x40000000) {
            if (arg0->unkA0 != NULL) {
                modgfx_func_59B4(arg0, arg1, arg3, 0U);
                arg0->unkA0->unk1b = 0;
                arg0->unkA0->unk12 = 1;
                arg0->unkA0->unk13 = 8;
                dll_newlfx->DoAction(temp_v0, temp_v0, arg0->unkA0, 0, 0, &sp34);
                arg0->unkA0->unk13 ^= 8;
            }
        } else if ((arg2 == 0) && (arg3 != 0) && (arg1->unk0 & 0x8000)) {
            // FAKE
            if (1) {}
            if (arg0->unkA0 != NULL) {
                arg0->unkA0->unk12 = 2;
                arg0->unkA0->unke = 0;
                arg0->unkA0->unk1b = 0;
                arg0->unkA0->unk10 = (u16) temp_v0->unkD6;
                dll_newlfx->DoAction(temp_v0, temp_v0, arg0->unkA0, 0, 0, &sp34);
                mmFree(arg0->unkA0);
                arg0->unkA0 = NULL;
                if (arg1->unk0 & 0x02000000) {
                    D_80092C3C = 0.0f;
                    D_800BB198.x = 0.0f;
                }
            }
        }
    }
}

// offset: 0x6BE8 | func: 28 | export: 12
void modgfx_Func12(Object* arg0, u8 arg1, u8 arg2, s32 arg3, s32 arg4) {
    bzero(&bss_AD0, sizeof(bss_AD0));
    bss_AD0.unk5A = 0;
    bss_AD0.unk5B = 0;
    bss_AD0.unk58 = arg1;
    bss_AD0.unk44 = arg1;
    bss_AD0.unk2C.x = 0.0f;
    bss_AD0.unk2C.y = 0.0f;
    bss_AD0.unk2C.z = 0.0f;
    bss_AD0.unk20 = 0.0f;
    bss_AD0.unk24 = 0.0f;
    bss_AD0.unk28 = 0.0f;
    bss_AD0.unk4 = arg0;
    bss_AD0.unk38 = 1.0f;
    bss_AD0.unk40 = arg3;
    bss_AD0.unk3C = arg4;
    bss_AD0.unk59 = arg2;
}

// offset: 0x6CA0 | func: 29 | export: 13
void modgfx_Func13(void) {
    bss_AC4 = bss_AC0 = bss_7C0;
    bss_AC8 = 0;
}

// offset: 0x6CD8 | func: 30 | export: 14
void modgfx_Func14(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16* arg5) {
    bss_AC4->unk16 = bss_AC8;
    bss_AC4->unk14 = arg4;
    bss_AC4->unk10 = arg5;
    bss_AC4->unk0 = arg0;
    bss_AC4->unk4.x = arg1;
    bss_AC4->unk4.y = arg2;
    bss_AC4->unk4.z = arg3;
    bss_AC4++;
}

// offset: 0x6D58 | func: 31 | export: 15
void modgfx_Func15(void) {
    bss_AC8++;
}

// offset: 0x6D80 | func: 32 | export: 16
void modgfx_Func16(s16 arg0) {
    bss_AC8 = arg0;
}

// offset: 0x6DA8 | func: 33 | export: 17
void modgfx_Func17(s16 arg0) {
    bss_AD0.unk46[bss_AC8] = arg0;
}

// offset: 0x6DE4 | func: 34 | export: 18
void modgfx_Func18(s16* arg0) {
    bcopy(arg0, &bss_AD0.unk46, sizeof(bss_AD0.unk46));
}

// offset: 0x6E24 | func: 35 | export: 19
void modgfx_Func19(SRT* arg0, s16* arg1, s32 arg3, s16* arg4, s32 arg5, s32 arg6, Texture* arg7) {
    bss_AD0.unk0 = bss_7C0;
    bss_AD0.unk5D = bss_AC4 - bss_AC0;
    if (arg7 == 0 && arg6 == 0) {
        bss_AD0.unk54 |= 0x2000000;
    } else {
        bss_AD0.unk54 |= 0x4000000;
    }

    if (bss_AD0.unk54 & 1) {
        if (bss_AD0.unk4 != NULL) {
            bss_AD0.unk2C.x += bss_AD0.unk4->globalPosition.x;
            bss_AD0.unk2C.y += bss_AD0.unk4->globalPosition.y;
            bss_AD0.unk2C.z += bss_AD0.unk4->globalPosition.z;
        } else {
            bss_AD0.unk2C.x += arg0->transl.x;
            bss_AD0.unk2C.y += arg0->transl.y;
            bss_AD0.unk2C.z += arg0->transl.z;
        }
    }

    data_2C = modgfx_Func1(&bss_AD0, 0, arg3, arg1, arg5, arg4, arg6, arg7);
}

// offset: 0x6F88 | func: 36 | export: 20
void modgfx_Func20(s32 arg0) {
    bss_AD0.unk54 |= arg0;
}

// offset: 0x6FB0 | func: 37 | export: 21
s16 modgfx_Func21(void) {
    return data_2C;
}
