#include "dlls/engine/29_gplay.h"
#include "sys/camera.h"
#include "sys/curves.h"
#include "sys/envfx.h"
#include "sys/lighting.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/rand.h"
#include "dll.h"

// size: 0x318
typedef struct {
/*000*/ s32 unk0;
/*004*/ u16 unk4;
/*006*/ u16 unk6;
/*008*/ s32 unk8;
/*00C*/ s32 unkC;
/*010*/ u8 _unk10[0x14 - 0x10];
/*014*/ s32 fogMin;
/*018*/ s32 fogMax;
/*01C*/ s32 unk1C;
/*020*/ s32 unk20;
/*024*/ s32 fogR;
/*028*/ s32 fogG;
/*02C*/ s32 fogB;
/*030*/ s32 unk30;
/*034*/ s32 unk34;
/*038*/ s32 unk38;
/*03C*/ s32 unk3C;
/*040*/ s32 unk40;
/*044*/ s32 unk44;
/*048*/ s32 unk48;
/*04C*/ u8 _unk4C[0x58 - 0x4C];
/*058*/ f32 unk58;
/*05C*/ f32 unk5C;
/*060*/ f32 unk60;
/*064*/ f32 unk64;
/*068*/ f32 unk68;
/*06C*/ f32 unk6C;
        // 11 values each (11 r, 11 g, 11 b)
/*070*/ f32 colorSpline[33];
/*0F4*/ f32 colorSplineTarget[33];
/*178*/ f32 colorSplineRate[33];
        // 11 values each (11 min, 11 max)
/*1FC*/ f32 posSpline[22];
/*254*/ f32 posSplineTarget[22];
/*2AC*/ f32 posSplineRate[22];
/*304*/ f32 unk304;
/*308*/ f32 unk308;
/*30C*/ f32 unk30C;
/*310*/ f32 unk310; // fog alpha? sorta?
/*314*/ s8 unk314;
/*315*/ s8 unk315;
/*316*/ s8 unk316;
/*317*/ s8 unk317;
} BSS0;

/*0x0*/ static s8 data_0 = 1;
/*0x4*/ static u8 data_4[] = {
    0x00, 0x00, 0x01, 0x02, 
    0x03, 0x04, 0x05, 0x06, 
    0x07, 0x00, 0x00, 0x00
};

/*0x0*/ static BSS0* bss_0[2];
/*0x8*/ static s8 bss_8;
/*0xC*/ static u32 bss_C;
/*0x10*/ static Vec3f bss_10[8];

static void newfog_func_1964(s32 sel);
static void newfog_func_1F58(EnvFxAction* action, u8 arg1);
void newfog_GetColor(s32* r, s32* g, s32* b, f32* arg3);

// offset: 0x0 | ctor
void newfog_ctor(void* dll) {
    D_80092A7C[0] = -1;
    D_80092A7C[1] = -1;
    if (bss_0[0] != NULL) {
        mmFree(bss_0[0]);
    }
    if (bss_0[1] != NULL) {
        mmFree(bss_0[1]);
    }
    bss_0[0] = NULL;
    bss_0[1] = NULL;
}

// offset: 0x94 | dtor
void newfog_dtor(void* dll) { }

// offset: 0xA0 | func: 0 | export: 0
void newfog_Func_A0(Object* objA, Object* objB, EnvFxAction* action, s32 arg3, u16 actionIndex) {
    u8 sp2F;
    u8 sel;
    u8 i;
    PlayerEnvActions* plEnvActions;

    sp2F = 0;
    plEnvActions = gDLL_29_Gplay->vtbl->get_current_player_envactions();
    if (action != NULL) {
        D_80092A7C[0] = action->unk24 - 1;
        D_80092A7C[1] = action->unk24 - 1;
        plEnvActions->unk8 = action->unk24 - 1;
        if (action->unk58 & 0x80) {
            sel = 1;
        } else {
            sel = 0;
        }
        if (bss_0[sel]->unk317 == 0) {
            if (action->unk58 & 0x40) {
                sp2F = 0x40;
            }
            newfog_func_1F58(action, sp2F);
            if (action->unk58 & 0x40) {
                bss_0[sel]->unk316 = 1;
            }
            bss_0[sel]->unk4 = action->unk58 | 0x100;
            bss_0[sel]->unk315 = 1;
            bss_0[sel]->unk304 = 0.0f;
            return;
        }
        if (action->unk58 & 0x20) {
            envfxAction(NULL, NULL, 9, 0);
            return;
        }
        bss_0[sel]->unk4 = action->unk58 | 0x100;
        bss_0[sel]->unk315 = 1;
        bss_0[sel]->unk304 = 0.0f;
        for (i = 0; i < 11; i++) {
            bss_0[sel]->colorSplineTarget[i] = (f32) action->r[data_4[i]];
            bss_0[sel]->colorSplineTarget[i + 11] = (f32) action->g[data_4[i]];
            bss_0[sel]->colorSplineTarget[i + 22] = (f32) action->b[data_4[i]];
            bss_0[sel]->posSplineTarget[i] = (f32) action->fogMin[data_4[i]];
            bss_0[sel]->posSplineTarget[i + 11] = (f32) action->fogMax[data_4[i]];
        }
        bss_0[sel]->unk3C = action->unk2A;
        bss_0[sel]->unk40 = action->unk2C;
        bss_0[sel]->unk314 = -1;
        if (action->unk59 & 0x20) {
            if (!(bss_0[sel]->unk6 & 0x20)) {
                bss_0[sel]->unk6 |= 0x20;
            }
        }
        if (!(action->unk59 & 0x20) && (bss_0[sel]->unk6 & 0x20)) {
            bss_0[sel]->unk6 ^= 0x20;
        }
        if (action->unk58 & 0x40) {
            bss_0[sel]->unk6 |= 0x40;
            bss_0[sel]->unk316 = 1;
        } else if (bss_0[sel]->unk6 & 0x40) {
            bss_0[sel]->unk6 ^= 0x40;
        }
        if ((action->unk59 & 0x40) && !(bss_0[sel]->unk6 & 0x40)) {
            bss_0[sel]->unk6 |= 0x40;
        } else if (!(action->unk59 & 0x40) && (bss_0[sel]->unk6 & 0x40)) {
            bss_0[sel]->unk6 ^= 0x40;
        }
    }
}

// offset: 0x464 | func: 1 | export: 1
void newfog_Func_464(void) {
    /*0x10*/ static s32 data_10 = 1;
    s32 i;

    D_80092A7C[0] = -1;
    D_80092A7C[1] = -1;
    for (i = 0; i < 2; i++) {
        if (bss_0[i] == NULL) {
            bss_0[i] = mmAlloc(sizeof(BSS0), ALLOC_TAG_SKY_COL, ALLOC_NAME("newfog"));
        }
        bzero(bss_0[i], sizeof(BSS0));
        bss_0[i]->fogR = 255;
        bss_0[i]->fogG = 255;
        bss_0[i]->fogB = 255;
        bss_0[i]->fogMin = 1150;
        bss_0[i]->fogMax = 1205;
        bss_0[i]->unk30 = 255;
        bss_0[i]->unk34 = 255;
        bss_0[i]->unk38 = 255;
        bss_0[i]->unk1C = 1150;
        bss_0[i]->unk20 = 1205;
        if (data_10 != 0) {
            envfxAction(NULL, NULL, 9, 0);
            data_10 = 0;
        }
    }
}

// offset: 0x5AC | func: 2 | export: 2
void newfog_Func_5AC(void) {
    /*0x30*/ static u8 data_30 = 1;
    f32 r;
    s32 sel;
    f32 g;
    f32 b;
    f32 min;
    f32 max;
    f32 var_fv0;
    f32 camDir[3];
    SRT srt;
    Camera* camera;
    s32 var_v0;
    Vec3f spCC = VEC3F(0.0f, 0.0f, 1.0f);
    f32 tmp;
    f32 spBC[] = {-1000.0f, -1000.0f, -1000.0f};
    f32 tValue;
    s32 temp_s1;
    s32 temp_a0;
    s16 temp_ft1;
    s32 var_v1;
    s32 i;
    f32 spA0 = 0.0f;
    u8 sp9C[] = {0, 0, 0};
    u8 ambientR;
    u8 ambientG;
    u8 ambientB;

    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
    min = 0.0f;
    max = 0.0f;

    lightGetAmbient(&ambientR, &ambientG, &ambientB);
    if (data_30 != 0) {
        // @decomp: must be on one line:
        bss_10[0].x = 0.0f;    bss_10[0].y = 0.0f; bss_10[0].z = 1.0f;
        bss_10[1].x = -0.707f; bss_10[1].y = 0.0f; bss_10[1].z = 0.707f;
        bss_10[2].x = -1.0f;   bss_10[2].y = 0.0f; bss_10[2].z = 0.0f;
        bss_10[3].x = -0.707f; bss_10[3].y = 0.0f; bss_10[3].z = -0.707f;
        bss_10[4].x = 0.0f;    bss_10[4].y = 0.0f; bss_10[4].z = -1.0f;
        bss_10[5].x = 0.707f;  bss_10[5].y = 0.0f; bss_10[5].z = -0.707f;
        bss_10[6].x = 1.0f;    bss_10[6].y = 0.0f; bss_10[6].z = 0.0f; 
        bss_10[7].x = 0.707f;  bss_10[7].y = 0.0f; bss_10[7].z = 0.707f;
        
        data_30 = 0;
    }
    camera = camGet();
    camDir[0] = 0;
    camDir[1] = 0.0f;
    camDir[2] = -1.0f;
    srt.transl.x = 0.0f;
    srt.transl.y = 0.0f;
    srt.transl.z = 0.0f;
    srt.scale = 1.0f;
    srt.yaw = -camera->srt.yaw;
    srt.roll = 0;
    srt.pitch = 0;
    mathRotateRPY(&srt, camDir);
    for (sel = 0; sel < 2; sel++) {
        if ((bss_0[sel] != NULL) && (bss_0[sel]->unk317 != 0)) {
            data_0 = 0;
            if (bss_0[sel]->unk48 != 0) {
                if (!(bss_0[sel]->unk4 & 1)) {
                    bss_0[sel]->unk310 = bss_0[sel]->unk30C * 255.0f;
                    if (bss_0[sel]->unk310 > 255.0f) {
                        bss_0[sel]->unk310 = 255.0f;
                    }
                }
            } else if (bss_0[sel]->unk44 != 0) {
                bss_0[sel]->unk30C = bss_0[sel]->unk310 / 255.0f;
                if (!(bss_0[sel]->unk4 & 1)) {
                    bss_0[sel]->unk310 -= bss_0[sel]->unk58 * gUpdateRateF;
                    if (bss_0[sel]->unk310 < 0.0f) {
                        bss_0[sel]->unk310 = 0.0f;
                    }
                }
            }
            if (bss_0[sel]->unk4 & 0x100) {
                newfog_func_1964(sel);
            }
            if (bss_0[sel]->unk4 & 0x10) {
                // Constant color/pos
                r = bss_0[sel]->colorSpline[0];
                g = bss_0[sel]->colorSpline[11];
                b = bss_0[sel]->colorSpline[22];
                min = bss_0[sel]->posSpline[0];
                max = bss_0[sel]->posSpline[11];
            } else if (bss_0[sel]->unk6 & 0x20) {
                // Interpolate color/pos based on current time of day
                gDLL_7_Newday->vtbl->func4(&spA0);
                var_fv0 = spA0 / 86400.0f;
                if (var_fv0 < 0.0f) {
                    var_fv0 = 0.0f;
                }
                if (var_fv0 > 1.0f) {
                    var_fv0 = 1.0f;
                }
                if (var_fv0 <= 0.125f) {
                    var_v0 = 0;
                    tValue = var_fv0 / 0.125f;
                } else if (var_fv0 <= 0.25f) {
                    var_v0 = 1;
                    tValue = (var_fv0 - 0.125f) / 0.125f;
                } else if (var_fv0 <= 0.375f) {
                    var_v0 = 2;
                    tValue = (var_fv0 - 0.25f) / 0.125f;
                } else if (var_fv0 <= 0.5f) {
                    var_v0 = 3;
                    tValue = (var_fv0 - 0.375f) / 0.125f;
                } else if (var_fv0 <= 0.625f) {
                    var_v0 = 4;
                    tValue = (var_fv0 - 0.5f) / 0.125f;
                } else if (var_fv0 <= 0.75f) {
                    var_v0 = 5;
                    tValue = (var_fv0 - 0.625f) / 0.125f;
                } else if (var_fv0 <= 0.875f) {
                    var_v0 = 6;
                    tValue = (var_fv0 - 0.75f) / 0.125f;
                } else {
                    var_v0 = 7;
                    tValue = (var_fv0 - 0.875f) / 0.125f;
                }
                r = curvesCatmullRom(&bss_0[sel]->colorSpline[var_v0], tValue, NULL);
                g = curvesCatmullRom(&bss_0[sel]->colorSpline[var_v0 + 11], tValue, NULL);
                b = curvesCatmullRom(&bss_0[sel]->colorSpline[var_v0 + 22], tValue, NULL);
                min = curvesCatmullRom(&bss_0[sel]->posSpline[var_v0], tValue, NULL);
                max = curvesCatmullRom(&bss_0[sel]->posSpline[var_v0 + 11], tValue, NULL);
            } else {
                // Fog color/pos varies based on camera XZ dir. Can be used to make fog match 
                // the sky color gradient at sunset/sunrise.
                for (i = 0; i < 8; i++) {
                    temp_s1 = mathAtan2f(bss_10[i].x, bss_10[i].z);
                    temp_a0 = temp_s1 - mathAtan2f(camDir[0], camDir[2]);
                    var_v1 = temp_a0;
                    if (temp_a0 < 0) {
                        var_v1 = temp_a0 * -1;
                    }
                    if (var_v1 >= 0x8000) {
                        var_v1 = 0xFFFF - var_v1;
                    }
                    tmp = (((32767.0f - (f32) var_v1) / 32767.0f) - 0.75f) / 0.25f;
                    if (spBC[0] < tmp) {
                        if (spBC[1] < spBC[0]) {
                            spBC[1] = spBC[0];
                            sp9C[1] = sp9C[0];
                        }
                        sp9C[0] = i;
                        spBC[0] = tmp;
                    } else if (spBC[1] < tmp) {
                        spBC[1] = tmp;
                        sp9C[1] = i;
                    }
                }
                for (i = 0; i < 2; i++) {
                    if (spBC[i] > 0.0f) {
                        r += bss_0[sel]->colorSpline[sp9C[i] + 0] * spBC[i];
                        g += bss_0[sel]->colorSpline[sp9C[i] + 11] * spBC[i];
                        b += bss_0[sel]->colorSpline[sp9C[i] + 22] * spBC[i];
                        min += bss_0[sel]->posSpline[sp9C[i]] * spBC[i];
                        max += bss_0[sel]->posSpline[sp9C[i] + 11] * spBC[i];
                    }
                }
            }
            if (r > 255.0f) {
                r = 255.0f;
            } else if (r < 0.0f) {
                r = 0.0f;
            }
            if (g > 255.0f) {
                g = 255.0f;
            } else if (g < 0.0f) {
                g = 0.0f;
            }
            if (b > 255.0f) {
                b = 255.0f;
            } else if (b < 0.0f) {
                b = 0.0f;
            }
            if (bss_0[sel]->unk6 & 0x40) {
                if (bss_0[sel]->unk314 == -1) {
                    bss_0[sel]->unk314 = 1;
                    bss_0[sel]->unk6C = 0.0f;
                    tmp = max - min;
                    bss_0[sel]->unk68 = (f32) mathRnd((s32) (-tmp / 2), (s32) (tmp / 2));
                    bss_0[sel]->unk64 = (f32) mathRnd(1, 0xA) * 0.05f;
                } else if (bss_0[sel]->unk314 == 1) {
                    min += bss_0[sel]->unk6C;
                    bss_0[sel]->unk6C += bss_0[sel]->unk64;
                    if (bss_0[sel]->unk68 < bss_0[sel]->unk6C) {
                        bss_0[sel]->unk314 = 1 - bss_0[sel]->unk314;
                    }
                } else {
                    min += bss_0[sel]->unk6C;
                    bss_0[sel]->unk6C -= bss_0[sel]->unk64;
                    if (bss_0[sel]->unk6C < 0.0f) {
                        temp_ft1 = (s16) (max - min);
                        bss_0[sel]->unk314 = 1 - bss_0[sel]->unk314;
                        bss_0[sel]->unk6C = 0.0f;
                        bss_0[sel]->unk68 = (f32) mathRnd(-temp_ft1 / 2, temp_ft1 / 2);
                        bss_0[sel]->unk64 = (f32) mathRnd(1, 0xA) * 0.05f;
                    }
                }
            }
            if (max > 2000.0f) {
                max = 2000.0f;
            } else if (max < 100.0f) {
                max = 100.0f;
            }
            if (max < min) {
                min = max - 1.0f;
            } else if (min < 100.0f) {
                min = 100.0f;
            }
            if (!(bss_0[sel]->unk4 & 8)) {
                r *= ((f32) (ambientR + ambientG + ambientB) / (255.0f * 3));
                g *= ((f32) (ambientR + ambientG + ambientB) / (255.0f * 3));
                b *= ((f32) (ambientR + ambientG + ambientB) / (255.0f * 3));
            }
            if (bss_0[sel]->unk4 & 1) {
                bss_0[sel]->fogR = (s32) r;
                bss_0[sel]->fogG = (s32) g;
                bss_0[sel]->fogB = (s32) b;
                bss_0[sel]->fogMin = (s32) min;
                bss_0[sel]->fogMax = (s32) max;
                if (!(bss_0[sel]->unk4 & 0x80)) {
                    bss_0[sel]->unk30 = 255;
                    bss_0[sel]->unk34 = 255;
                    bss_0[sel]->unk38 = 255;
                    bss_0[sel]->unk1C = 1950;
                    bss_0[sel]->unk20 = 2005;
                }
            } else if (bss_0[sel]->unk4 & 4) {
                bss_0[sel]->unk30 = (s32) r;
                bss_0[sel]->unk34 = (s32) g;
                bss_0[sel]->unk38 = (s32) b;
                bss_0[sel]->unk1C = (s32) min;
                bss_0[sel]->unk20 = (s32) max;
                if (!(bss_0[sel]->unk4 & 0x80)) {
                    bss_0[sel]->fogR = 255;
                    bss_0[sel]->fogG = 255;
                    bss_0[sel]->fogB = 255;
                    bss_0[sel]->fogMin = 1950;
                    bss_0[sel]->fogMax = 2005;
                }
            } else {
                bss_0[sel]->fogR = (s32) r;
                bss_0[sel]->fogG = (s32) g;
                bss_0[sel]->fogB = (s32) b;
                bss_0[sel]->fogMin = (s32) min;
                bss_0[sel]->fogMax = (s32) max;
                bss_0[sel]->unk30 = (s32) r;
                bss_0[sel]->unk34 = (s32) g;
                bss_0[sel]->unk38 = (s32) b;
                bss_0[sel]->unk1C = (s32) min;
                bss_0[sel]->unk20 = (s32) max;
            }
        }
    }
}

// offset: 0x1234 | func: 3 | export: 3
void newfog_Func_1234(Gfx** gdl) {
    if (bss_0[0] != NULL) {
        bss_8 = 2;
        dlSetFogColor(gdl, bss_0[0]->fogR, bss_0[0]->fogG, bss_0[0]->fogB, 55);
        if (bss_0[0]->fogMin == bss_0[0]->fogMax) {
            bss_0[0]->fogMin -= 20;
        }
        if (bss_0[0]->fogMax < bss_0[0]->fogMin) {
            bss_0[0]->fogMin = bss_0[0]->fogMax - 20;
        }
        gSPFogPosition((*gdl)++, bss_0[0]->fogMin, bss_0[0]->fogMax);
    }
}

// offset: 0x139C | func: 4 | export: 4
s32 newfog_Func_139C(s32 arg0) {
    return 0;
}

// offset: 0x13AC | func: 5 | export: 5
void newfog_Func_13AC(Gfx** gdl) {
    s16 alpha;
    f32 min;

    if (bss_0[0] != NULL) {
        if ((data_0 == 0) && !(bss_0[0]->unk4 & 1)) {
            if (bss_0[0]->fogMin < 850) {
                alpha = 255;
            } else if (bss_0[0]->fogMin > 1010) {
                alpha = 0;
            } else {
                min = (f32) bss_0[0]->fogMin;
                alpha = (s16) (255.0f - (((min - 850.0f) / 200.0f) * 255.0f));
            }
            dlSetPrimColor(gdl, bss_0[0]->fogR, bss_0[0]->fogG, bss_0[0]->fogB, (u8) alpha);
        } else {
            dlSetPrimColor(gdl, 255, 255, 255, 0);
        }
    }
}

// offset: 0x14B0 | func: 6 | export: 10
void newfog_Func_14B0(Gfx** gdl) {
    if ((bss_0[0] != NULL) && (bss_8 != 2)) {
        bss_8 = 2;
        dlSetFogColor(gdl, bss_0[0]->fogR, bss_0[0]->fogG, bss_0[0]->fogB, 255);
        if (bss_0[0]->fogMin == bss_0[0]->fogMax) {
            bss_0[0]->fogMin--;
        }
        gSPFogPosition((*gdl)++, bss_0[0]->fogMin, bss_0[0]->fogMax);
    }
}

// offset: 0x1608 | func: 7 | export: 11
void newfog_Func_1608(Gfx** gdl) {
    if ((bss_0[0] != NULL) && (bss_8 != 1)) {
        bss_8 = 1;
        dlSetFogColor(gdl, bss_0[1]->fogR, bss_0[1]->fogG, bss_0[1]->fogB, 255);
        if (bss_0[1]->fogMin == bss_0[1]->fogMax) {
            bss_0[1]->fogMin--;
        }
        gSPFogPosition((*gdl)++, bss_0[1]->fogMin, bss_0[1]->fogMax);
    }
}

// offset: 0x1764 | func: 8 | export: 6
void newfog_Func_1764(s32* arg0, s32* arg1, s32* arg2) {
    s32 fogR;
    s32 fogG;
    s32 fogB;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    f32 sp2C;
    s32 _pad;

    sp2C = 0.0f;
    if ((bss_0[0] != NULL) && ((bss_0[0] == NULL) || (bss_0[0]->unk316 != 0))) {
        sp38 = *arg0;
        sp34 = *arg1;
        sp30 = *arg2;
        newfog_GetColor(&fogR, &fogG, &fogB, &sp2C);
        *arg0 = (s32) ((f32) sp38 + ((f32) (fogR - sp38) * (0.25f * sp2C)));
        *arg1 = (s32) ((f32) sp34 + ((f32) (fogG - sp34) * (0.25f * sp2C)));
        *arg2 = (s32) ((f32) sp30 + ((f32) (fogB - sp30) * (0.25f * sp2C)));
    }
}

// offset: 0x18C0 | func: 9 | export: 9
void newfog_Func_18C0(void) {

}

// offset: 0x18C8 | func: 10 | export: 12
s16 newfog_Func_18C8(void) {
    s16 alpha;
    f32 min;

    if (bss_0[0] == NULL) {
        return 0xFF;
    }
    if (bss_0[0]->fogMin < 950) {
        alpha = 0;
    } else if (bss_0[0]->fogMin > 1210) {
        alpha = 255;
    } else {
        min = (f32) bss_0[0]->fogMin;
        alpha = (s16) (((min - 950.0f) / 200.0f) * 255.0f);
    }
    return alpha;
}

// offset: 0x1964 | func: 11
static void newfog_func_1964(s32 sel) {
    f32 var_fv0;
    s32 i;

    i = 0;
    if (bss_0[sel]->unk304 >= 1.0f) {
        bss_0[sel]->unk4 &= ~0x100;
        bss_0[sel]->unk308 = 0.0f;
        bss_0[sel]->unk304 = 0.0f;
        bss_0[sel]->unk30C = 1.0f;
        if ((bss_0[sel]->unk316 != 0) && !(bss_0[sel]->unk6 & 0x40)) {
            bss_0[sel]->unk316 = 0;
        }
        for (i = 0; i < 33; i++) {
            bss_0[sel]->colorSpline[i] = bss_0[sel]->colorSplineTarget[i];
        }
        for (i = 0; i < 22; i++) {
            bss_0[sel]->posSpline[i] = bss_0[sel]->posSplineTarget[i];
        }
        return;
    }
    if (bss_0[sel]->unk315 != 0) {
        var_fv0 = ((f32) bss_0[sel]->unk3C / 10.0f) * 60.0f;
        if (var_fv0 == 0.0f) {
            var_fv0 = 1.0f;
        }
        bss_0[sel]->unk308 = 1.0f / var_fv0;
        for (i = 0; i < 33; i++) {
            bss_0[sel]->colorSplineRate[i] = (bss_0[sel]->colorSplineTarget[i] - bss_0[sel]->colorSpline[i]) / var_fv0;
        }
        for (i = 0; i < 22; i++) {
            bss_0[sel]->posSplineRate[i] = (bss_0[sel]->posSplineTarget[i] - bss_0[sel]->posSpline[i]) / var_fv0;
        }
        bss_0[sel]->unk315 = 0;
    }
    for (i = 0; i < 33; i++) {
        bss_0[sel]->colorSpline[i] += (bss_0[sel]->colorSplineRate[i] * gUpdateRateF);
    }
    for (i = 0; i < 22; i++) {
        bss_0[sel]->posSpline[i] += bss_0[sel]->posSplineRate[i] * gUpdateRateF;
    }
    bss_0[sel]->unk304 += (bss_0[sel]->unk308 * gUpdateRateF);
    if ((bss_0[sel]->unk4 & 1) && (bss_0[sel]->unk310 > 0.0f)) {
        bss_0[sel]->unk310 -= (255.0f * bss_0[sel]->unk304);
        if (bss_0[sel]->unk310 < 0.0f) {
            bss_0[sel]->unk310 = 0.0f;
            data_0 = 1;
        }
    } else if ((bss_0[sel]->unk4 & 4) && (bss_0[sel]->unk310 < 255.0f)) {
        bss_0[sel]->unk310 = bss_0[sel]->unk304 * 255.0f;
        if (bss_0[sel]->unk310 > 255.0f) {
            bss_0[sel]->unk310 = 255.0f;
        }
    } else if (!(bss_0[sel]->unk4 & 1) && (bss_0[sel]->unk310 < 255.0f)) {
        bss_0[sel]->unk310 = bss_0[sel]->unk304 * 255.0f;
        if (bss_0[sel]->unk310 > 255.0f) {
            bss_0[sel]->unk310 = 255.0f;
        }
    }
    bss_0[sel]->unk30C = bss_0[sel]->unk304;
}

// offset: 0x1F58 | func: 12
static void newfog_func_1F58(EnvFxAction* action, u8 arg1) {
    s32 i;
    s32 sel;
    f32 temp;
    
    if (action->unk58 & 0x80) {
        sel = 1;
    } else {
        sel = 0;
    }
    /* default.dol
    if (bss_0[sel] == NULL) {
        STUBBED_PRINTF("warning in newfog dll no spare memory for available\n");
    }
    */
    bss_0[sel]->unk0 = 0;
    bss_0[sel]->unk317 = 1;
    for (i = 0; i < 33; i++) {
        bss_0[sel]->colorSplineRate[i] = 0.0f;
    }
    for (i = 0; i < 33; i++) {
        bss_0[sel]->colorSpline[i] = 0.0f;
    }
    for (i = 0; i < 22; i++) {
        bss_0[sel]->posSplineRate[i] = 0.0f;
    }
    for (i = 0; i < 11; i++) {
        bss_0[sel]->posSpline[i] = 1400.0f;
        bss_0[sel]->posSpline[i + 11] = 1600.0f;
    }
    for (i = 0; i < 11; i++) {
        bss_0[sel]->colorSplineTarget[i] = (f32) action->r[data_4[i]];
        bss_0[sel]->colorSplineTarget[i + 11] = (f32) action->g[data_4[i]];
        bss_0[sel]->colorSplineTarget[i + 22] = (f32) action->b[data_4[i]];
        bss_0[sel]->posSplineTarget[i] = (f32) action->fogMin[data_4[i]];
        bss_0[sel]->posSplineTarget[i + 11] = (f32) action->fogMax[data_4[i]];
    }
    bss_0[sel]->unk4 = action->unk58;
    bss_0[sel]->unk6 = action->unk59;
    bss_0[sel]->unk64 = 0.0f;
    bss_0[sel]->unk68 = 0.0f;
    bss_0[sel]->unk314 = -1;
    bss_0[sel]->unk6C = 0.0f;
    if (action->unk2A == 0) {
        action->unk2A = 1;
    }
    if (action->unk2A != 0) {
        bss_0[sel]->unk3C = action->unk2A;
        bss_0[sel]->unk48 = 1;
        bss_0[sel]->unk8 = action->fogMax[0];
        bss_0[sel]->unk5C = 1.0f / (f32) action->unk2A;
    } else {
        bss_0[sel]->unk3C = 0;
        bss_0[sel]->unk5C = 1.0f;
    }
    if (action->unk2C == 0) {
        action->unk2C = 1;
    }
    if (action->unk2C != 0) {
        bss_0[sel]->unk40 = action->unk2C;
        temp = ((f32) action->unk2C / 10.0f) * 60.0f;
        bss_0[sel]->unk58 = 255.0f / temp;
        bss_0[sel]->unkC = 1500;
        bss_0[sel]->unk60 = 1.0f / (f32) action->unk2C;
    } else {
        bss_0[sel]->unk40 = 0;
        bss_0[sel]->unk60 = 1.0f;
    }
    bss_0[sel]->unk44 = 0;
}

// offset: 0x22FC | func: 13 | export: 7
void newfog_GetColor(s32* r, s32* g, s32* b, f32* arg3) {
    if (bss_0[0] != NULL) {
        *r = bss_0[0]->fogR;
        *g = bss_0[0]->fogG;
        *b = bss_0[0]->fogB;
        *arg3 = bss_0[0]->unk30C;
    }
}

// offset: 0x2350 | func: 14 | export: 8
void newfog_GetPosition(s32* min, s32* max) {
    if (bss_0[0] != NULL) {
        *min = bss_0[0]->fogMin;
        *max = bss_0[0]->fogMax;
    }
}
