#include "PR/gbi.h"
#include "PR/os.h"
#include "sys/camera.h"
#include "sys/gfx/textable.h"
#include "sys/map.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "sys/rand.h"
#include "dll.h"
#include "macros.h"

typedef struct {
/*00*/ Vtx* vtxs[2];
/*08*/ DLTri* tris[2];
/*10*/ Mtx* mtxs[2];
/*18*/ s32 vtxSel;
/*1C*/ s32 triSel;
/*20*/ s32 mtxSel;
/*24*/ u8 _unk24[0xC4 - 0x24];
/*C4*/ void* membase;
} StarsState;

// size: 0x28
typedef struct {
/*00*/ s32 unk0;
/*04*/ s32 unk4; // legacy texture ID (textable)?
/*08*/ s32 unk8;
/*0C*/ Vec3f unkC;
/*18*/ f32 scale;
/*1C*/ s16 unk1C;
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
/*22*/ s16 unk22;
/*24*/ s16 numStars;
/*26*/ s16 unk26;
} StarCluster;

/*0x0*/ static s32 sNumStars = 0;
/*0x4*/ static Texture* data_4 = NULL;
/*0x8*/ static Texture* sStarTexture = NULL;

/*0x0*/ static StarsState* sState;
/*0x4*/ static u32 bss_4; // unused
/*0x8*/ static StarCluster sClusters[9];

static s32 newstars_func_119C(Vec3f* arg0, Vec3f* arg1, f32* arg2);

// offset: 0x0 | ctor
void newstars_ctor(void* dll) { }

// offset: 0xC | dtor
void newstars_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void newstars_Init(void) {
    u8* mem;
    s32 var_s1;
    s32 var_s4;
    s32 var_s7;
    DLTri* var_v0;
    Vec3f sp1C8;
    Vtx* sp1C4;
    Vtx* sp1C0;
    s32 i;
    SRT sp1A4;
    s32 temp_a0;
    s16 sp84[] = {
        0x0000, 0x0000, 0xfffd, 0x0007, 0xfffd, 0x0003, 0xfffe, 0x0001, 0xffff, 0xfffe, 0x0002, 0xfffd, 0xfffe, 0xfff8, 0x0001, 0xfff7, 
        0xfff4, 0x0000, 0xfff6, 0x0002, 0xfffa, 0x0004, 0xfffc, 0xffff, 0xfffd, 0x0003, 0xfffe, 0xfffc, 0x0003, 0x0003, 0x0003, 0x0001, 
        0x0003, 0xffff, 0x0007, 0x0007, 0x0008, 0xfffa, 0xfffa, 0xfff5, 0xfffe, 0xfff5, 0x0001, 0xfff6, 0x0005, 0xfffe, 0xfff4, 0x0004, 
        0xfff3, 0x0002, 0xfff6, 0x0001, 0xfffa, 0x0000, 0xfffc, 0x0001, 0xfff3, 0xfffb, 0xfff6, 0xfff8, 0xfff9, 0xfffa, 0xfffb, 0xfffb, 
        0x0003, 0x0001, 0x0002, 0xfffa, 0xffff, 0x0007, 0x0003, 0x0008, 0x0005, 0x0006, 0x0007, 0x0002, 0x0009, 0x0004, 0x0008, 0x0006, 
        0x0007, 0x0007, 0x0009, 0x0009, 0x000d, 0x000a, 0x000f, 0x0005, 0xffff, 0x000d, 0xfffa, 0x0000, 0x000d, 0xfffc, 0x0000, 0xfff2, 
        0xffeb, 0x0001, 0xfff1, 0xfff6, 0x0000, 0x000a, 0x0000, 0xfffe, 0x0003, 0x000c, 0x0004, 0xfffd, 0x0006, 0x0000, 0x0004, 0xffec, 
        0x0006, 0xfff2, 0xffed, 0xfffc, 0xfff7, 0x000a, 0xfff7, 0x0004, 0x0006, 0xfff9, 0x0006, 0x0007, 0xfff4, 0xfff9, 0xfffe, 0x0007, 
        0x0002, 0xfff9, 0x000c, 0x0007, 0x0006, 0xffef, 0x0006, 0x0007, 0x0009, 0xfffc, 0x0009, 0x000a, 0x0013, 0x0004
    };
    /*0x128*/ static s32 sNeedsInit = TRUE;

    if (!sNeedsInit) {
        return;
    }
    sNeedsInit = FALSE;
    data_4 = texLoadTexture(TEXTABLE_22_WaterSplashParticle);
    sStarTexture = texLoadTexture(TEXTABLE_DF_StarBlue);
    sClusters[0].unk1C = 0;
    sClusters[0].unk20 = 0;
    sClusters[0].unk0 = 1;
    sClusters[0].unk1E = 0;
    sClusters[0].unk22 = 0;
    sClusters[0].numStars = 1;
    sClusters[0].scale = 0.5f;
    sClusters[0].unk26 = 0x258;
    sClusters[0].unk8 = 0;
    sClusters[0].unk4 = 0x22;
    sClusters[0].unkC.x = 0.0f;
    sClusters[0].unkC.y = 0.0f;
    sClusters[0].unkC.z = 1.0f;
    sNumStars += sClusters[0].numStars;
    sClusters[1].unk1C = 0x2710;
    sClusters[1].unk20 = 1;
    sClusters[1].unk0 = 0;
    sClusters[1].unk1E = 0;
    sClusters[1].unk22 = 0;
    sClusters[1].numStars = 7;
    sClusters[1].scale = 0.5f;
    sClusters[1].unk26 = 0x1F4;
    sClusters[1].unk8 = 0;
    sClusters[1].unk4 = 0x23;
    sClusters[1].unkC.x = 0.0f;
    sClusters[1].unkC.y = 0.0f;
    sClusters[1].unkC.z = 1.0f;
    sNumStars += sClusters[1].numStars;
    sClusters[2].unk1C = 0x2710;
    sClusters[2].unk20 = 0x3E80;
    sClusters[2].unk0 = 1;
    sClusters[2].unk1E = 0;
    sClusters[2].unk22 = 0;
    sClusters[2].numStars = 15;
    sClusters[2].scale = 0.5f;
    sClusters[2].unk26 = 0x190;
    sClusters[2].unk8 = 0;
    sClusters[2].unk4 = 0x23;
    sClusters[2].unkC.x = 0.0f;
    sClusters[2].unkC.y = 0.0f;
    sClusters[2].unkC.z = 1.0f;
    sNumStars += sClusters[2].numStars;
    sClusters[3].unk1C = 0x1B58;
    sClusters[3].unk20 = 0x7D00;
    sClusters[3].unk0 = 1;
    sClusters[3].unk1E = 0;
    sClusters[3].unk22 = 0;
    sClusters[3].numStars = 21;
    sClusters[3].scale = 0.5f;
    sClusters[3].unk26 = 0x190;
    sClusters[3].unk8 = 0;
    sClusters[3].unk4 = 0x23;
    sClusters[3].unkC.x = 0.0f;
    sClusters[3].unkC.y = 0.0f;
    sClusters[3].unkC.z = 1.0f;
    sNumStars += sClusters[3].numStars;
    sClusters[4].unk1C = 0x1F40;
    sClusters[4].unk20 = -0x4480;
    sClusters[4].unk0 = 1;
    sClusters[4].unk1E = 0;
    sClusters[4].unk22 = 0;
    sClusters[4].numStars = 28;
    sClusters[4].scale = 0.5f;
    sClusters[4].unk26 = 0xFA;
    sClusters[4].unk8 = 0;
    sClusters[4].unk4 = 0x23;
    sClusters[4].unkC.x = 0.0f;
    sClusters[4].unkC.y = 0.0f;
    sClusters[4].unkC.z = 1.0f;
    sNumStars += sClusters[4].numStars;
    for (i = 5; i < 9; i++) {
        sClusters[i].unk1C = 0x7D0;
        sClusters[i].unk20 = ((i - 5) * 0x3FFF) + 0x1F40;
        sClusters[i].unk0 = 1;
        sClusters[i].unk1E = 0;
        sClusters[i].unk22 = 0;
        sClusters[i].numStars = 12;
        sClusters[i].scale = 1.0f;
        sClusters[i].unk26 = 0x28A;
        sClusters[i].unk8 = 0;
        sClusters[i].unk4 = 0x23;
        sClusters[i].unkC.x = 0.0f;
        sClusters[i].unkC.y = 0.0f;
        sClusters[i].unkC.z = 1.0f;
        sNumStars += sClusters[i].numStars;
    }
    if (sState != NULL) {
        if (sState->membase != NULL) {
            mmFree(sState->membase);
            sState->membase = NULL;
        }
    }
    if (sState != NULL) {
        mmFree(sState);
        sState = NULL;
    }
    sState = NULL;
    sState = mmAlloc(sizeof(StarsState), ALLOC_TAG_SKY_COL, ALLOC_NAME("stars:sfield"));
    if (sState == NULL) {
        STUBBED_PRINTF("warning in newstars dll no spare memory available\n");
        return;
    }
    sState->mtxs[0] = NULL;
    sState->mtxs[1] = NULL;
    sState->vtxs[0] = NULL;
    sState->vtxs[1] = NULL;
    sState->tris[0] = NULL;
    sState->tris[1] = NULL;
    sState->triSel = 0;
    sState->vtxSel = 0;
    sState->mtxSel = 0;
    temp_a0 = (3 * sNumStars * sizeof(Vtx)) + (2 * 18 * sizeof(DLTri));
    temp_a0 += (3 * sNumStars * sizeof(Vtx)) + (2 * 2 * sizeof(Mtx));
    mem = mmAlloc(temp_a0, ALLOC_TAG_SKY_COL, ALLOC_NAME("stars:membase"));
    sState->membase = mem;
    if (mem == NULL) {
        STUBBED_PRINTF("warning in newstars dll no spare memory available\n");
        return;
    }
    sState->tris[0] = (DLTri*)mem;
    mem += (18 * sizeof(DLTri));
    sState->tris[1] = (DLTri*)mem;
    mem += (18 * sizeof(DLTri));
    sState->vtxs[0] = (Vtx*)mem;
    mem += (3 * sNumStars * sizeof(Vtx));
    sState->vtxs[1] = (Vtx*)mem;
    mem += (3 * sNumStars * sizeof(Vtx));
    sState->mtxs[0] = (Mtx*)mem;
    mem += (2 * sizeof(Mtx));
    sState->mtxs[1] = (Mtx*)mem;
    sp1C4 = sState->vtxs[sState->vtxSel];
    sp1C0 = sState->vtxs[sState->vtxSel];

    for (i = 0; i < 2; i++) {
        var_v0 = sState->tris[sState->triSel];
        var_v0->v0 = 0;
        var_v0->v1 = 2;
        var_v0->v2 = 1;
        var_v0++;
        var_s1 = 0;
        for (var_s4 = 0; var_s4 < 8; var_s4++) {
            var_v0->v0 = var_s1 + 0;
            var_v0->v1 = var_s1 + 2;\
            var_v0->v2 = var_s1 + 1;
            var_s1 += 3;
            var_v0++;
        }
        sState->triSel = 1 - sState->triSel;
    }
    var_s7 = 0;
    var_s1 = 0;
    sp1A4.transl.x = 0.0f;
    sp1A4.transl.y = 0.0f;
    sp1A4.transl.z = 0.0f;

    for (i = 0; i < 9; i++) {
        sp1A4.roll = 0;
        sp1A4.pitch = 0;
        sp1A4.yaw = sClusters[i].unk20;
        mathRotateRPY(&sp1A4, sClusters[i].unkC.f);
        if (0) {}
        for (var_s4 = 0; var_s4 < sClusters[i].numStars; var_s4++) {
            if (i < 5) {
                sp1A4.roll = (sClusters[i].unk26 * sp84[var_s7]) + sClusters[i].unk1C;
                sp1A4.pitch = (sClusters[i].unk26 * sp84[var_s7 + 1]) + sClusters[i].unk1E;
                sp1A4.yaw = sClusters[i].unk20;
                var_s7 += 2;
            } else {
                sp1A4.roll = (sClusters[i].unk26 * mathRnd(0, 0x14)) + sClusters[i].unk1C;
                sp1A4.pitch = (sClusters[i].unk26 * (0x14 - mathRnd(0, 0x28))) + sClusters[i].unk1E;
                sp1A4.yaw = sClusters[i].unk20;
            }
            sp1C8.f[0] = sClusters[i].scale * -20.0f;
            sp1C8.f[1] = 2000.0f;
            sp1C8.f[2] = sClusters[i].scale * -25.0f;
            mathRotateRPY(&sp1A4, sp1C8.f);
            sp1C4[var_s1 + 0].v.ob[0] = (s16) sp1C8.f[0];
            sp1C4[var_s1 + 0].v.ob[1] = (s16) sp1C8.f[1];
            sp1C4[var_s1 + 0].v.ob[2] = (s16) sp1C8.f[2];
            sp1C4[var_s1 + 0].v.cn[0] = 0xFF;
            sp1C4[var_s1 + 0].v.cn[1] = 0xFF;
            sp1C4[var_s1 + 0].v.cn[2] = 0xFF;
            sp1C4[var_s1 + 0].v.cn[3] = 0xFF;
            sp1C4[var_s1 + 0].v.tc[0] = 0;
            sp1C4[var_s1 + 0].v.tc[1] = 0;
            sp1C8.f[0] = sClusters[i].scale * 20.0f;
            sp1C8.f[1] = 2000.0f;
            sp1C8.f[2] = 0.0f;
            mathRotateRPY(&sp1A4, sp1C8.f);
            sp1C4[var_s1 + 1].v.ob[0] = (s16) sp1C8.f[0];
            sp1C4[var_s1 + 1].v.ob[1] = (s16) sp1C8.f[1];
            sp1C4[var_s1 + 1].v.ob[2] = (s16) sp1C8.f[2];
            sp1C4[var_s1 + 1].v.cn[0] = 0xFF;
            sp1C4[var_s1 + 1].v.cn[1] = 0xFF;
            sp1C4[var_s1 + 1].v.cn[2] = 0xFF;
            sp1C4[var_s1 + 1].v.cn[3] = 0xFF;
            sp1C4[var_s1 + 1].v.tc[0] = 0xE0;
            sp1C4[var_s1 + 1].v.tc[1] = 0x1E0;
            sp1C8.f[0] = sClusters[i].scale * -20.0f;
            sp1C8.f[1] = 2000.0f;
            sp1C8.f[2] = sClusters[i].scale * 25.0f;
            mathRotateRPY(&sp1A4, sp1C8.f);
            sp1C4[var_s1 + 2].v.ob[0] = (s16) sp1C8.f[0];
            sp1C4[var_s1 + 2].v.ob[1] = (s16) sp1C8.f[1];
            sp1C4[var_s1 + 2].v.ob[2] = (s16) sp1C8.f[2];
            sp1C4[var_s1 + 2].v.cn[0] = 0xFF;
            sp1C4[var_s1 + 2].v.cn[1] = 0xFF;
            sp1C4[var_s1 + 2].v.cn[2] = 0xFF;
            sp1C4[var_s1 + 2].v.cn[3] = 0xFF;
            sp1C4[var_s1 + 2].v.tc[0] = 0x1E0;
            sp1C4[var_s1 + 2].v.tc[1] = 0;
            var_s1 += 3;
        }
    }
    sState->vtxSel = 1 - sState->vtxSel;
    sp1C4 = sState->vtxs[sState->vtxSel];
    for (i = 0; i < (sNumStars * 3); i++) {
        sp1C4->v.ob[0] = sp1C0->v.ob[0];
        sp1C4->v.ob[1] = sp1C0->v.ob[1];
        sp1C4->v.ob[2] = sp1C0->v.ob[2];
        sp1C4->v.cn[0] = sp1C0->v.cn[0];
        sp1C4->v.cn[1] = sp1C0->v.cn[1];
        sp1C4->v.cn[2] = sp1C0->v.cn[2];
        sp1C4->v.cn[3] = sp1C0->v.cn[3];
        temp_a0 = i % 3;
        if (temp_a0 == 0) {
            sp1C4->v.tc[0] = 0;
            sp1C4->v.tc[1] = 0;
        } else if (temp_a0 == 1) {
            sp1C4->v.tc[0] = 0xE0;
            sp1C4->v.tc[1] = 0x1E0;
        } else if (temp_a0 == 2) {
            sp1C4->v.tc[0] = 0x1E0;
            sp1C4->v.tc[1] = 0;
        }
        sp1C4 += 1;
        sp1C0 += 1;
    }
}

/*0x12C*/ static s32 sStarTextureProgress = 0;

// offset: 0xA1C | func: 1 | export: 1
void newstars_Print(Gfx** gdl) {
    Mtx* temp_s4;
    Camera* camera;
    s32 dlNeedsTex;
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
    s32 nstars;
    SRT sp8C;
    f32 sp88;
    f32 temp_s0;
    u32 newvar;
    s32 texAnimFlags;
    s32 vtxIdx;
    s32 _pad;

    dlNeedsTex = TRUE;
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
    sState->mtxSel = 1 - sState->mtxSel;
    temp_s4 = sState->mtxs[sState->mtxSel];
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
    tris = sState->tris[sState->triSel];
    vtxs = sState->vtxs[sState->vtxSel];
    vtxIdx = 0;
    sStarTexture->animSpeed = 0;
    // @bug: texAnimFlags is undefined here!
    texAnimateTexture(sStarTexture, &texAnimFlags, &sStarTextureProgress);
    for (i = 0; i < 9; i++) {
        if (1) {} // @fake
        newstars_func_119C(&spB8, &sClusters[i].unkC, &spA8);
        if (spA8 > 0.5f) {
            if (dlNeedsTex) {
                texDPTextures(gdl, sStarTexture, NULL, RENDER_NO_CULL, TEX_FRAMEBLEND(0, sStarTextureProgress << 8), FALSE, TRUE);
                dlNeedsTex = FALSE;
            }
            nstars = sClusters[i].numStars;
            while (nstars != 0) {
                if (nstars >= 9) {
                    if (nstars % 4) {
                        dlSetPrimColor(gdl, 255, 255, 255, (u8) ((mathRnd(0, 255) * (s16) sp88) >> 8));
                    } else {
                        dlSetPrimColor(gdl, 255, 255, 255, 155);
                    }
                    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(&vtxs[vtxIdx]), 24, 0);
                    dlTriangles(gdl, tris + 1, 8);
                    vtxIdx += 24;
                    nstars -= 8;
                } else {
                    dlSetPrimColor(gdl, 255, 255, 255, (u8) ((mathRnd(0, 255) * (s16) sp88) >> 8));
                    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(&vtxs[vtxIdx]), nstars * 3, 0);
                    dlTriangles(gdl, tris + 1, nstars);
                    vtxIdx += nstars * 3;
                    nstars = 0;
                }
            }
        } else {
            vtxs += 3 * sClusters[i].numStars;
        }
    }
    // FAKE
    i = 0;
    sState->triSel = 1 - sState->triSel;
    sState->vtxSel = 1 - sState->vtxSel;
}

// offset: 0x1118 | func: 2 | export: 2
void newstars_Free(void) {
    // @bug: missing null check!
    if (sState->membase != NULL) {
        mmFree(sState->membase);
        sState->membase = NULL;
    }
    if (sState != NULL) {
        mmFree(sState);
        sState = NULL;
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
