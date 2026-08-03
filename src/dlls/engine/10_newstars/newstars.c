#include "PR/os.h"
#include "common.h"

typedef struct {
    Vtx* unk0[2];
    DLTri* unk8[2];
    Mtx* unk10[2];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 _unk24[0xC4 - 0x24];
    void* unkC4;
} BSS0;

// size: 0x28
typedef struct {
    u8 _unk0[0xC - 0x0];
    Vec3f unkC;
    u8 _unk18[0x24 - 0x18];
    s16 unk24;
    u8 _unk26[0x28 - 0x26];
} BSS8;

/*0x0*/ static s32 data_0 = 0x00000000;
/*0x4*/ static s32 data_4 = 0x00000000;
/*0x8*/ static Texture* data_8 = NULL;
/*0xC*/ static u32 data_C[] = {
    0x00000000, 0xfffd0007, 0xfffd0003, 0xfffe0001, 0xfffffffe, 0x0002fffd, 0xfffefff8, 0x0001fff7, 
    0xfff40000, 0xfff60002, 0xfffa0004, 0xfffcffff, 0xfffd0003, 0xfffefffc, 0x00030003, 0x00030001, 
    0x0003ffff, 0x00070007, 0x0008fffa, 0xfffafff5, 0xfffefff5, 0x0001fff6, 0x0005fffe, 0xfff40004, 
    0xfff30002, 0xfff60001, 0xfffa0000, 0xfffc0001, 0xfff3fffb, 0xfff6fff8, 0xfff9fffa, 0xfffbfffb, 
    0x00030001, 0x0002fffa, 0xffff0007, 0x00030008, 0x00050006, 0x00070002, 0x00090004, 0x00080006, 
    0x00070007, 0x00090009, 0x000d000a, 0x000f0005, 0xffff000d, 0xfffa0000, 0x000dfffc, 0x0000fff2, 
    0xffeb0001, 0xfff1fff6, 0x0000000a, 0x0000fffe, 0x0003000c, 0x0004fffd, 0x00060000, 0x0004ffec, 
    0x0006fff2, 0xffedfffc, 0xfff7000a, 0xfff70004, 0x0006fff9, 0x00060007, 0xfff4fff9, 0xfffe0007, 
    0x0002fff9, 0x000c0007, 0x0006ffef, 0x00060007, 0x0009fffc, 0x0009000a, 0x00130004
};
/*0x128*/ static s32 data_128 = 0x00000001;
/*0x12C*/ static s32 data_12C = 0;

/*0x0*/ static BSS0* bss_0[2];
/*0x8*/ static BSS8 bss_8[9];

static s32 newstars_func_119C(Vec3f* arg0, Vec3f* arg1, f32* arg2);

// offset: 0x0 | ctor
void newstars_ctor(void* dll) { }

// offset: 0xC | dtor
void newstars_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/10_newstars/newstars_Func_18.s")

// offset: 0xA1C | func: 1 | export: 1
void newstars_Func_A1C(Gfx** gdl) {
    Mtx* temp_s4;
    Camera* camera;
    s32 sp1DC;
    Vtx* vtxs;
    DLTri* tris;
    MtxF sp194;
    s32 i;
    MtxF sp150;
    MtxF sp110;
    MtxF spD0;
    s32 _pad2;
    Mtx* projMtx;
    MtxF* temp_s0_2;
    Vec3f spB8;
    Vec3f spAC;
    f32 spA8;
    s32 var_s0;
    SRT sp8C;
    f32 sp88;
    f32 temp_s0;
    u32 newvar;
    s32 sp7C;
    s32 vtxIdx;
    s32 _pad;

    sp1DC = 1;
    spA8 = 0.0f;
    sp88 = 0.0f;
    temp_s0 = 0.0f;
    if (gDLL_7_Newday->vtbl->func8(&sp88) != 0) {
        if (sp88 > 4000.0f) {
            sp88 = 255.0f;
        } else {
            sp88 = (sp88 / 4000.0f) * 255.0f;
        }
    } else if ((sp88 > 12000.0f) || (sp88 == 0.0f)) {
        return;
    } else {
        sp88 = 255.0f - ((sp88 / 12000.0f) * 255.0f);
    }

    camera = camGet();
    spB8.x = 0.0f;
    spB8.y = 0.0f;
    spB8.z = 1.0f;
    spAC.x = 0.0f;
    spAC.y = 0.0f;
    spAC.z = 1.0f;
    sp8C.transl.x = 0.0f;
    sp8C.transl.y = 0.0f;
    sp8C.transl.z = 0.0f;
    sp8C.roll = 0;
    sp8C.pitch = 0;
    sp8C.yaw = 0x4000 - camera->srt.yaw;
    mathRotateRPY(&sp8C, spB8.f);
    newstars_func_119C(&spB8, &spAC, &spA8);
    bss_0[0]->unk20 = 1 - bss_0[0]->unk20;
    temp_s4 = bss_0[0]->unk10[bss_0[0]->unk20];
    newvar = (u32)temp_s4;
    temp_s0_2 = &spD0;
    for (i = 0; i < (4*4); i++) {
        ((f32*)temp_s0_2)[i] = 0.0f;
    }
    spD0.m[1][1] = 1.0f;
    spD0.m[3][3] = 1.0f;
    spD0.m[0][0] = mathCosfInterp(-(s16)temp_s0);
    spD0.m[0][2] = mathSinfInterp(-(s16)temp_s0);
    spD0.m[2][0] = -mathSinfInterp(-(s16)temp_s0);
    spD0.m[2][2] = mathCosfInterp(-(s16)temp_s0);
    spD0.m[3][0] = camera->srt.transl.x - gWorldX;
    spD0.m[3][1] = camera->srt.transl.y;
    spD0.m[3][2] = camera->srt.transl.z - gWorldZ;
    projMtx = camGetRSPProjectionMtx();
    temp_s0_2 = camGetViewMtx();
    guMtxL2F(sp194.m, projMtx);
    mathMtxCatF(temp_s0_2, &sp194, &sp150);
    mathMtxCatF(&spD0, &sp150, &sp110);
    mathMtxF2L(&sp110, temp_s4);
    gSPForceMatrix((*gdl)++, OS_PHYSICAL_TO_K0(newvar));
    tris = bss_0[0]->unk8[bss_0[0]->unk1C];
    vtxs = bss_0[0]->unk0[bss_0[0]->unk18];
    vtxIdx = 0;
    data_8->animSpeed = 0;
    texAnimateTexture(data_8, &sp7C, &data_12C);
    for (i = 0; i < 9; i++) {
        if (1) {} // @fake
        newstars_func_119C(&spB8, &bss_8[i].unkC, &spA8);
        if (spA8 > 0.5f) {
            if (sp1DC != 0) {
                texDPTextures(gdl, data_8, NULL, RENDER_NO_CULL, TEX_FRAMEBLEND(0, data_12C << 8), FALSE, TRUE);
                sp1DC = 0;
            }
            var_s0 = bss_8[i].unk24;
            while (var_s0 != 0) {
                if (var_s0 >= 9) {
                    if (var_s0 % 4) {
                        dlSetPrimColor(gdl, 255, 255, 255, (u8) ((mathRnd(0, 255) * (s16) sp88) >> 8));
                    } else {
                        dlSetPrimColor(gdl, 255, 255, 255, 155);
                    }
                    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(&vtxs[vtxIdx]), 24, 0);
                    dlTriangles(gdl, tris + 1, 8);
                    vtxIdx += 24;
                    var_s0 -= 8;
                } else {
                    dlSetPrimColor(gdl, 255, 255, 255, (u8) ((mathRnd(0, 255) * (s16) sp88) >> 8));
                    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(&vtxs[vtxIdx]), var_s0 * 3, 0);
                    dlTriangles(gdl, tris + 1, var_s0);
                    vtxIdx += var_s0 * 3;
                    var_s0 = 0;
                }
            }
        } else {
            vtxs += 3 * bss_8[i].unk24;
        }
    }
    // FAKE
    i = 0;
    bss_0[0]->unk1C = 1 - bss_0[0]->unk1C;
    bss_0[0]->unk18 = 1 - bss_0[0]->unk18;
}

// offset: 0x1118 | func: 2 | export: 2
void newstars_Func_1118(void) {
    // @bug: missing null check!
    if (bss_0[0]->unkC4 != NULL) {
        mmFree(bss_0[0]->unkC4);
        bss_0[0]->unkC4 = NULL;
    }
    if (bss_0[0] != NULL) {
        mmFree(bss_0[0]);
        bss_0[0] = NULL;
    }
}

// offset: 0x119C | func: 3
static s32 newstars_func_119C(Vec3f* arg0, Vec3f* arg1, f32* arg2) {
    f32 sp44;
    f32 temp_ft4;
    f32 var_fv1;
    s32 sp38;
    
    sp38 = -1;
    sp44 = (arg0->f[0] * arg1->f[0]) + (arg0->f[1] * arg1->f[1]) + (arg0->f[2] * arg1->f[2]);
    if (sp44 >= 0.0f) {
        sp38 = 1;
    }
    var_fv1 = SQ(arg0->f[0]) + SQ(arg0->f[1]) + SQ(arg0->f[2]);
    temp_ft4 = SQ(arg1->f[0]) + SQ(arg1->f[1]) + SQ(arg1->f[2]);
    var_fv1 *= temp_ft4;
    if (var_fv1 != 0.0f) {
        temp_ft4 = sqrtf(var_fv1);
    }
    if (temp_ft4 != 0.0f) {
        *arg2 = sp44 / temp_ft4;
    } else {
        *arg2 = 0.0f;
    }
    return sp38;
}

/*0x0*/ static const char str_0[] = "warning in newstars dll no spare memory available\n";
/*0x34*/ static const char str_34[] = "warning in newstars dll no spare memory available\n";
