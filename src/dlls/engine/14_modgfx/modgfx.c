#include "common.h"
#include "sys/lfx.h"
#include "sys/segment_13D0.h"
#include "sys/lighting.h"
#include "macros.h"

typedef struct {
    void* unk0;
    Object* unk4;
    u8 _unk8[0x20 - 0x8];
    f32 unk20;
    f32 unk24;
    f32 unk28;
    Vec3f unk2C;
    f32 unk38;
    s32 unk3C;
    s32 unk40;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s32 unk54;
    u8 _unk58;
    u8 unk59;
    u8 unk5A;
    u8 unk5B;
    u8 _unk5C;
    u8 unk5D;
} ModgfxStruct;

// size: 0x18
typedef struct {
    s32 unk0;
    f32 unk4;
    u8 _unk8[0x10 - 0x8];
    s32 unk10;
    s16 unk14;
    u8 unk16;
} BSS0_9C;

typedef struct {
    Object* unk0;
    Object* unk4;
    void* unk8;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 _unk12[0x18 - 0x12];
    Vec3f unk18;
    Vec3f unk24[5];
    f32 unk60;
    f32 unk64;
    f32 unk68;
    Vec3f unk6C;
    Vtx* unk78[2];
    u8 _unk80[0x84 - 0x80];
    DLTri* unk84[2];
    u8 _unk8C[0x98 - 0x8C];
    Texture* unk98;
    BSS0_9C* unk9C;
    LightAction* unkA0;
    s32 unkA4;
    u8 _unkA8[0xAC - 0xA8];
    f32 unkAC;
    f32 unkB0;
    f32 unkB4;
    f32 unkB8;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    u8 _unkD8[0xEA - 0xD8];
    s16 unkEA;
    s16 unkEC;
    u8 _unkEE[0xFC - 0xEE];
    s16 unkFC;
    s16 unkFE;
    u8 _unk100[0x106 - 0x100];
    s16 unk106;
    s16 unk108;
    s16 unk10A;
    s16 unk10C;
    s16 unk10E;
    s16 unk110;
    u8 _unk112[0x120 - 0x112];
    s16 unk120;
    s16 unk122;
    s16 unk124;
    u8 unk126;
    u8 _unk127[0x12C - 0x127];
    void* unk12C;
    u8 unk130;
    u8 unk131;
    u8 unk132;
    u8 unk133;
    u8 unk134;
    u8 unk135;
    u8 unk136;
    u8 unk137;
    u8 unk138;
    s8 unk139;
    u8 unk13A;
    u8 unk13B;
    u8 unk13C;
    u8 unk13D;
    u8 unk13E;
    u8 unk13F;
} ModgfxInstance;

/*0x0*/ static u32 data_0 = 0x00000000;
/*0x4*/ static u32 data_4 = 0x00000000;
/*0x8*/ static u32 data_8 = 0x00000000;
/*0xC*/ static f32 data_C = 0.0;
/*0x10*/ static f32 data_10 = 0.0;

/*0x0*/ static ModgfxInstance* bss_0[496];
/*0x7C0*/ static u8 bss_7C0[0x300];
/*0xAC0*/ static u8 bss_AC0[0x4];
/*0xAC4*/ static u8 bss_AC4[0x4];
/*0xAC8*/ static u8 bss_AC8[0x8];
/*0xAD0*/ static u8 bss_AD0[0x46];
/*0xB16*/ static u8 bss_B16[0x2];
/*0xB18*/ static u8 _bss_B18[0x8];
/*0xB20*/ static u8 _bss_B20[0x10];

/*0x0*/ static const char str_0[] = "warning in modgfx dll no spare memory available\n";

static void dll_14_func_4C0C(s16 arg0, s32 arg1);
static void dll_14_func_4EDC(ModgfxInstance* arg0, u8 arg1);

// offset: 0x0 | ctor
void dll_14_ctor(void* dll) {
    s32 i = 0;
    while (i < ARRAYCOUNT_S(bss_0)) {
        bss_0[i++] = 0;
    }
}

// offset: 0x40 | dtor
void dll_14_dtor(void* dll) {
    dll_14_func_4C0C(0, 1);
}

// offset: 0x88 | func: 0 | export: 0
void dll_14_func_88(void) {
    s32 i;

    dll_14_func_4C0C(0, 1);
    i = 0;
    while (i < ARRAYCOUNT_S(bss_0)) {
        bss_0[i++] = 0;
    }
}

// offset: 0xF4 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_F4.s")

// offset: 0xC90 | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_C90.s")

// offset: 0x21F0 | func: 3 | export: 3
void dll_14_func_21F0(void) {
    dll_14_func_4C0C(0, 1);
}

// offset: 0x2234 | func: 4 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_2234.s")

// offset: 0x2474 | func: 5 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_2474.s")

// offset: 0x2618 | func: 6 | export: 6
s32 dll_14_func_2618(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, u8 arg3, Object* obj) {
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
    for (i = 0; i < ARRAYCOUNT_S(bss_0); i++) {
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
            dll_14_func_4EDC(bss_0[idx], 0);
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
                sp25C.x = bss_0[idx]->unk18.x;
                sp25C.y = bss_0[idx]->unk18.y;
                sp25C.z = bss_0[idx]->unk18.z;
                camTransformPointByObjectMatrix(&bss_0[idx]->unk18, &sp25C, bss_0[idx]->unk135);
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
            srt.roll = bss_0[idx]->unk106 + bss_0[idx]->unk10;
            srt.pitch = bss_0[idx]->unk108 + bss_0[idx]->unkE;
            srt.yaw = bss_0[idx]->unk10A + bss_0[idx]->unkC;
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
                bss_0[idx]->unk133 = (u8) (0x3C / (s32) bss_0[idx]->unk132);
                bss_0[idx]->unk131 += 1;
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
            dlSetPrimColor(gdl, sp233, sp232, sp231, 0xFF);
        } else if ((bss_0[idx]->unk4 != NULL) && (bss_0[idx]->unkA4 & 0x4000)) {
            dlSetPrimColor(gdl, 0xFF, 0xFF, 0xFF, bss_0[idx]->unk4->opacityWithFade);
        } else {
            dlSetPrimColor(gdl, 0xFF, 0xFF, 0xFF, 0xFF);
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_4854.s")

// offset: 0x4910 | func: 8 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_4910.s")

// offset: 0x4938 | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_4938.s")

// offset: 0x49E4 | func: 10 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_49E4.s")

// offset: 0x4A88 | func: 11 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_4A88.s")

// offset: 0x4BA4 | func: 12
s32 dll_14_func_4BA4(void) {
    s32 var_v0;
    s32 var_v1;

    var_v1 = 0;
    var_v0 = 0;
    while (var_v1 < ARRAYCOUNT_S(bss_0) && var_v0 == 0) {
        if (bss_0[var_v1] == NULL) {
            var_v0 = 1;
        }
        var_v1 += 1;
    }
    var_v1--;
    if (var_v0 == 0) {
        return -1;
    }
    return var_v1;
}

// offset: 0x4C0C | func: 13
static void dll_14_func_4C0C(s16 arg0, s32 arg1) {
    s32 _pad[6]; // really?
    s32 sp5C;
    s32 i;

    for (i = 0; i < ARRAYCOUNT_S(bss_0); i++) {
        if ((bss_0[i] != NULL) && ((arg0 == bss_0[i]->unk10C) || (arg1 != 0))) {
            if (bss_0[i]->unkA0 != NULL) {
                bss_0[i]->unkA0->unk12.asByte = 2;
                bss_0[i]->unkA0->unke = 0;
                bss_0[i]->unkA0->unk1b = 0;
                gDLL_11_Newlfx->vtbl->func0(bss_0[i]->unk4, bss_0[i]->unk4, bss_0[i]->unkA0, 0, 0, (s32) &sp5C);
                mmFree(bss_0[i]->unkA0);
            }
            if (bss_0[i]->unk10E != -1) {
                lfxAction(bss_0[i]->unk4, NULL, bss_0[i]->unk10E, 0, 0, 0);
            }
            if (bss_0[i]->unk0 != NULL) {
                objFreeObject(bss_0[i]->unk0);
            }
            mmFree(bss_0[i]->unk12C);
            bss_0[i]->unk12C = 0;
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_4E58.s")

// offset: 0x4EDC | func: 15
void dll_14_func_4EDC(ModgfxInstance* arg0, u8 arg1) {
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_4FF4.s")

// offset: 0x538C | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_538C.s")

// offset: 0x56A0 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_56A0.s")

// offset: 0x584C | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_584C.s")

// offset: 0x59B4 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_59B4.s")

// offset: 0x5E50 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_5E50.s")

// offset: 0x5FFC | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_5FFC.s")

// offset: 0x6068 | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6068.s")

// offset: 0x6100 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6100.s")

// offset: 0x6424 | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6424.s")

// offset: 0x64F0 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_64F0.s")

// offset: 0x6758 | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6758.s")

// offset: 0x6BE8 | func: 28 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6BE8.s")

// offset: 0x6CA0 | func: 29 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6CA0.s")

// offset: 0x6CD8 | func: 30 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6CD8.s")

// offset: 0x6D58 | func: 31 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6D58.s")

// offset: 0x6D80 | func: 32 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6D80.s")

// offset: 0x6DA8 | func: 33 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6DA8.s")

// offset: 0x6DE4 | func: 34 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6DE4.s")

// offset: 0x6E24 | func: 35 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6E24.s")

// offset: 0x6F88 | func: 36 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/14_modgfx/dll_14_func_6F88.s")

// offset: 0x6FB0 | func: 37 | export: 21
s16 dll_14_func_6FB0(void) {
    return data_2C;
}
