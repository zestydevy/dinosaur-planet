#include "dlls/engine/11_newlfx.h"
#include "dlls/engine/29_gplay.h"
#include "sys/lfx.h"
#include "sys/lighting.h"
#include "sys/lighting_asm.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/newshadows.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "dll.h"
#include "macros.h"

typedef struct {
/*00*/ u8 _unk0[0x6 - 0x0];
/*06*/ s8 unk6;
/*07*/ s8 unk7;
/*08*/ u8 unk8;
/*09*/ u8 unk9;
/*0A*/ u8 unkA;
/*0B*/ u8 unkB;
/*0C*/ u8 unkC;
/*0D*/ u8 unkD;
/*10*/ Vec3f pos;
/*1C*/ Object* unk1C;
/*20*/ Object* unk20;
/*24*/ s32 unk24;
/*28*/ s32 unk28;
/*2C*/ s32 unk2C;
/*30*/ u8 _unk30[0x34 - 0x30];
/*34*/ s32 unk34;
/*38*/ s32 unk38;
/*3C*/ s32 unk3C;
/*40*/ f32 unk40;
/*44*/ f32 unk44;
/*48*/ u8 _unk48[0x54 - 0x48];
/*54*/ f32 unk54;
/*58*/ f32 unk58;
/*5C*/ f32 unk5C;
/*60*/ u8 _unk60[0x68 - 0x60];
/*68*/ s16 unk68;
/*6A*/ u8 unk6A;
/*6B*/ u8 unk6B;
/*6C*/ u8 unk6C;
/*6D*/ u8 _unk6D[0x73 - 0x6D];
/*73*/ u8 unk73;
/*74*/ u8 unk74;
/*75*/ u8 unk75;
/*76*/ u8 unk76;
/*77*/ u8 unk77;
/*78*/ u8 unk78;
/*79*/ u8 paletteIdx;
/*7A*/ u8 unk7A;
/*7B*/ u8 unk7B;
/*7C*/ u8 unk7C;
/*7D*/ u8 unk7D;
/*7E*/ u8 unk7E;
/*7F*/ u8 unk7F;
} LfxLight;

typedef struct {
/*0*/ s32 type;
/*4*/ s32 refCount;
/*8*/ u8 pal[6];
/*E*/ u8 inUse;
} LfxPalette;

/*0x0*/ static s32 data_0 = 0;
/*0x4*/ static f32 data_4 = 0.0f;
/*0x8*/ static s16 data_8 = 0;
/*0xC*/ static u8 data_C[] = {0, 0, 0, 0};
/*0x10*/ static s32 sNumLights = 0;
/*0x14*/ static s32 _data_14 = 0; // unused
/*0x18*/ static u8 data_18 = 0;

/*0x0*/ static LfxLight* sLights[30];
/*0x78*/ static LfxPalette sPalettes[15];
/*0x168*/ static u8 _bss_168[0x10]; // unused
/*0x178*/ static NewLfxStruct bss_178[4];

static void newlfx_allocLight(LightAction*, f32, f32, f32, Object*, Object*);
static s32 newlfx_findLight(s32);
static void newlfx_freeLight(s32);
static void newlfx_updateLights(void);
static s32 newlfx_allocPalette(s32* paletteIdx, u8* pal, s32 type);

// offset: 0x0 | ctor
void newlfx_ctor(void* dll) { }

// offset: 0xC | dtor
void newlfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void newlfx_DoAction(Object* arg0, Object* arg1, LightAction* action, s32 arg3, s32 arg4, SRT* arg5) {
    s32 sp34;
    PlayerLightActions* plLightActions;

    sp34 = -1;
    plLightActions = gDLL_29_Gplay->vtbl->get_current_player_lactions();
    if (action != NULL) {
        if (action->unk10 != 0xFFFE) {
            sp34 = newlfx_findLight(action->unk10);
        }
        if (sp34 == -1) {
            if (!(action->unk12 & 2) && (action->unk12 & 1)) {
                if (!(action->unk12 & 0x40) && (arg0->def->unk87 & 0x10)) {
                    action->unk10 = data_18 + 11;
                    data_18++;
                    if (data_18 >= 16) {
                        data_18 = 0;
                    }
                    arg0->unkD6 = (u8) action->unk10;
                    newlfx_allocLight(action, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, arg1, arg0);
                    return;
                }
                if (action->unk10 == 0xFFFE) {
                    action->unk10 = data_18 + 11;
                    data_18++;
                    if (data_18 >= 16) {
                        data_18 = 0;
                    }
                    if (!(action->unk12 & 0x40)) {
                        arg0->unkD6 = (u8) action->unk10;
                        newlfx_allocLight(action, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, arg1, arg0);
                        return;
                    }
                    newlfx_allocLight(action, arg5->transl.x, arg5->transl.y, arg5->transl.z, arg1, arg0);
                    return;
                }
                if ((arg5 != NULL) && (action->unk12 & 0x40)) {
                    newlfx_allocLight(action, arg5->transl.x, arg5->transl.y, arg5->transl.z, arg1, arg0);
                    return;
                }
                if (arg0->parent != NULL) {
                    newlfx_allocLight(action, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, arg1, arg0);
                } else {
                    newlfx_allocLight(action, arg0->globalPosition.x, arg0->globalPosition.y, arg0->globalPosition.z, arg1, arg0);
                }
                if (action->unk1b == 0) {
                    plLightActions->unkC[1] = action->unke - 1;
                }
            }
        } else {
            if (action->unk12 & 1) {
                if (action->unk13 & 8) {
                    sLights[sp34]->unk54 = (f32) action->unk4;
                    sLights[sp34]->unk58 = (f32) action->unk6;
                    sLights[sp34]->unk5C = (f32) action->unk8;
                }
            } else if (action->unk12 & 2) {
                newlfx_freeLight(action->unk10);
                if (action->unk1b == 0) {
                    plLightActions->unkC[1] = action->unke - 1;
                }
            }
        }
    }
}

// offset: 0x384 | func: 1 | export: 1
void newlfx_Free(void) {
    s32 i;
    s32 j;

    for (i = 0; i < ARRAYCOUNT_S(sLights); i++) {
        if (sLights[i] != NULL) {
            mmFree(sLights[i]);
            sLights[i] = NULL;
        }
    }
    
    for (i = 0; i < 4; i++) {
        data_C[i] = 0;
    }

    for (i = 0; i < ARRAYCOUNT_S(sPalettes); i++) {        
        for (j = 0; j < 6; j++) {
            sPalettes[i].pal[j] = 0;
        }
        
        sPalettes[i].type = 0;
        sPalettes[i].inUse = FALSE;
        sPalettes[i].refCount = 0;
    }
    
    sNumLights = 0;
    data_0 = 0;
    D_80092C3C = 0.0f;
}

// offset: 0x488 | func: 2 | export: 2
void newlfx_Tick(void) {
    u8 var_a2;
    Object* temp_v0;
    s32 i;

    newlfx_updateLights();
    data_0 = 0;
    for (i = 0; i < ARRAYCOUNT_S(sLights); i++) {
        if (sLights[i] != NULL) {
            if (!(sLights[i]->unk6B & 1) && (sLights[i]->unk24 != 0) && (data_0 == 0)) {
                data_0 = 1;
                if (D_80092C3C <= 1.0f) {
                    D_80092C3C += data_4 * gUpdateRateF;
                }
                if (D_80092C3C > 1.0f) {
                    D_80092C3C = 1.0f;
                }
            }
            if (D_80091651 != 0) {
                var_a2 = mathRnd(0, 0xFF);
            } else {
                var_a2 = sLights[i]->unkC;
            }
            if (D_800B1845 < 30) {
                D_80090CA0 = D_800B1845;
                if (var_a2){} // @fake
                D_800B1858[D_80090CA0].pos.x = sLights[i]->pos.x;
                D_800B1858[D_80090CA0].pos.y = sLights[i]->pos.y;
                D_800B1858[D_80090CA0].pos.z = sLights[i]->pos.z;
                D_800B1858[D_80090CA0].unk6 = sLights[i]->unk6;
                D_800B1858[D_80090CA0].unk7 = sLights[i]->unk7;
                D_800B1858[D_80090CA0].unk9 = sLights[i]->unk9;
                D_800B1858[D_80090CA0].unk8 = sLights[i]->unk8;
                D_800B1858[D_80090CA0].unkA = sLights[i]->unkA;
                D_800B1858[D_80090CA0].unkB = sLights[i]->unkB;
                D_800B1858[D_80090CA0].unkC = var_a2;
                D_800B1858[D_80090CA0].unkD = sLights[i]->unkD + 1;
                D_800B1845 += 1;
            }
            temp_v0 = sLights[i]->unk20;
            if ((temp_v0 != NULL) && (temp_v0->parent != NULL)) {
                D_800B1860[i] = temp_v0->parent;
            } else {
                D_800B1860[i] = NULL;
            }
        } else {
            D_800B1860[i] = NULL;
        }
    }
    if ((data_0 == 0) && (D_80092C3C > 0.0f)) {
        D_80092C3C -= data_4 * gUpdateRateF;
        if (D_80092C3C <= 0.0f) {
            D_80092C3C = 0.0f;
            data_0 = 0;
        }
    }
}

// offset: 0x828 | func: 3 | export: 3
NewLfxStruct* newlfx_Func_828(s32 arg0, s8* count) {
    if (data_8 != 0) {
        if (count != NULL) {
            *count = (s8) data_8;
        }
        return bss_178;
    }
    if (count != NULL) {
        *count = 0;
    }
    return NULL;
}

// offset: 0x874 | func: 4 | export: 4
f32 newlfx_FindClosestLight(Vec3f* pos, f32 maxDist) {
    f32 xDiff;
    f32 zDiff;
    f32 dist;
    f32 smallestDist;
    s32 found;
    s32 i;

    smallestDist = 10000.0f;
    if (pos == NULL) {
        return -1.0f;
    }
    found = FALSE;
    for (i = 0; i < ARRAYCOUNT_S(sLights); i++) {
        if (sLights[i] != NULL) {
            xDiff = pos->x - sLights[i]->pos.x;
            if (!(maxDist < xDiff)) {
                zDiff = pos->z - sLights[i]->pos.z;
                if (!(maxDist < zDiff)) {
                    dist = SQ(xDiff) + SQ(zDiff);
                    if (!(SQ(maxDist) < dist)) {
                        if (dist != 0.0f) {
                            dist = sqrtf(dist);
                        }
                        if (dist < smallestDist) {
                            smallestDist = dist;
                            found = TRUE;
                        }
                    }
                }
            }
        }
    }
    if (found) {
        return smallestDist;
    }
    return -1.0f;
}

// offset: 0x9D8 | func: 5 | export: 5
f32 newlfx_Func_9D8(s32 arg0, f32* arg1) {
    f32 temp_fv0;
    s32 i;

    if (arg1 == NULL) {
        return -1.0f;
    }
    for (i = 0; i < ARRAYCOUNT_S(sLights); i++) {
        if ((sLights[i] != NULL) && (arg0 == sLights[i]->unk34) && (sLights[i]->unk6A & 8)) {
            temp_fv0 = sLights[i]->unk40 / 180.0f;
            if (temp_fv0 <= 0.5f) {
                temp_fv0 = temp_fv0 / 0.5f;
                temp_fv0 = 1.0f - temp_fv0;
            } else if (temp_fv0 > 0.5f) {
                temp_fv0 = (temp_fv0 - 0.5f) / 0.5f;
            }
            *arg1 = (f32) sLights[i]->unk8 / (f32) sLights[i]->unk7B;
            return temp_fv0;
        }
    }
    return -1.0f;
}

// offset: 0xBB8 | func: 6 | export: 6
s32 newlfx_Func_BB8(s32 arg0, s16 arg1, s16 arg2) {
    s32 i;

    if (arg1 > 255) {
        arg1 = 255;
    }
    if (arg2 > 255) {
        arg2 = 255;
    }
    for (i = 0; i < ARRAYCOUNT_S(sLights); i++) {
        if ((sLights[i] != NULL) && (arg0 == sLights[i]->unk34)) {
            sLights[i]->unkB = (u8) arg1;
            sLights[i]->unkA = (u8) arg2;
            return 1;
        }
    }
    return 0;
}

// offset: 0xD44 | func: 7
static void newlfx_updateLights(void) {
    s32 i;
    f32 var_fs0;
    SRT sp88;
    Vec3f sp7C;
    f32 var_fs4;
    f32 temp;

    sp88.transl.f[0] = 0.0f;
    sp88.transl.f[1] = 0.0f;
    sp88.transl.f[2] = 0.0f;
    sp88.scale = 1.0f;
    sp88.roll = 0;
    data_8 = 0;
    for (i = 0; i < ARRAYCOUNT_S(sLights); i++) {
        if (sLights[i] != NULL) {
            var_fs0 = 1.0f;
            if (sLights[i]->unk2C != 0) {
                var_fs0 = 1.0f - ((f32) sLights[i]->unk2C / (f32) sLights[i]->unk28);
                if (var_fs0 < 0.0f) {
                    var_fs0 = 0.001f;
                } else if (var_fs0 > 1.0f) {
                    var_fs0 = 1.0f;
                }
                temp = (f32) sLights[i]->unk7B * var_fs0;
                sLights[i]->unk8 = (u8) temp;
                temp = (f32) sLights[i]->unk7C * var_fs0;
                sLights[i]->unk9 = (u8) temp;
                sLights[i]->unk2C -= gUpdateRate;
                if (sLights[i]->unk2C < 0) {
                    sLights[i]->unk2C = 0;
                }
            }
            if (sLights[i]->unk6A & 8) {
                sLights[i]->unk40 += (180.0f * sLights[i]->unk44) * (f32) gUpdateRate;
                if (sLights[i]->unk40 > 180.0f) {
                    sLights[i]->unk40 = 0.0f;
                    if (!(sLights[i]->unk6B & 1)) {
                        if (sLights[i]->unk7D == 0) {
                            sLights[i]->unk7E = (u8) ((f32) (sLights[i]->unk7B - mathRnd(sLights[i]->unk7C, sLights[i]->unk7B)) * 0.5f);
                        } else {
                            sLights[i]->unk7E = sLights[i]->unk7C + mathRnd(sLights[i]->unk7C, sLights[i]->unk7D);
                        }
                    } else {
                        if (sLights[i]->unk7D == 0) {
                            sLights[i]->unk7E = (u8) ((f32) (sLights[i]->unk7B - mathRnd(sLights[i]->unk7C, sLights[i]->unk7B)) * 0.5f);
                        } else {
                            sLights[i]->unk7E = sLights[i]->unk7C + mathRnd(sLights[i]->unk7C, sLights[i]->unk7D);
                        }
                    }
                }
                var_fs4 = (f32) mathCosInterp((s16) (sLights[i]->unk40 * 182.04166f)) / 65535.0f;
                if (var_fs4 < 0.0f) {
                    var_fs4 *= -1.0f;
                    sLights[i]->unk8 = (u8) ((f32) sLights[i]->unk7B - ((f32) sLights[i]->unk7E * (1.0f - var_fs4)));
                    if (sLights[i]->unk8 < sLights[i]->unk9) {
                        sLights[i]->unk8 = sLights[i]->unk9;
                    }
                }
            }
            if (sLights[i]->unk6A & 0x40) {
                sLights[i]->pos.x += sLights[i]->unk54;
                sLights[i]->pos.y += sLights[i]->unk58;
                sLights[i]->pos.z += sLights[i]->unk5C;
            } else if ((sLights[i]->unk6A & 0x10) && (sLights[i]->unk3C != 0)) {
                if (sLights[i]->unk1C != NULL) {
                    if (sLights[i]->unk6A & 4) {
                        sp88.yaw = sLights[i]->unk1C->srt.yaw;
                        sp88.pitch = sLights[i]->unk1C->srt.pitch;
                        sp88.roll = sLights[i]->unk1C->srt.roll;
                        sp7C.f[0] = sLights[i]->unk54;
                        sp7C.f[1] = sLights[i]->unk58;
                        sp7C.f[2] = sLights[i]->unk5C;
                        mathRotateRPY(&sp88, sp7C.f);
                        sLights[i]->pos.x = sLights[i]->unk1C->globalPosition.x + sp7C.f[0];
                        sLights[i]->pos.y = sLights[i]->unk1C->globalPosition.y + sp7C.f[1];
                        sLights[i]->pos.z = sLights[i]->unk1C->globalPosition.z + sp7C.f[2];
                    } else {
                        sLights[i]->pos.x = sLights[i]->unk1C->globalPosition.x + sLights[i]->unk54;
                        sLights[i]->pos.y = sLights[i]->unk1C->globalPosition.y + sLights[i]->unk58;
                        sLights[i]->pos.z = sLights[i]->unk1C->globalPosition.z + sLights[i]->unk5C;
                    }
                    D_800BB198.x = sLights[i]->pos.x;
                    D_800BB198.y = sLights[i]->pos.y;
                    D_800BB198.z = sLights[i]->pos.z;
                    if ((sLights[i]->unk6B & 0x40) && (data_8 < 4)) {
                        data_8 += 1;
                        bss_178[data_8 - 1].unk0.f[0] = sLights[i]->pos.x - gWorldX;
                        bss_178[data_8 - 1].unk0.f[1] = sLights[i]->pos.y;
                        bss_178[data_8 - 1].unk0.f[2] = sLights[i]->pos.z - gWorldZ;
                        bss_178[data_8 - 1].unk18 = sLights[i]->unkB * 2;
                        bss_178[data_8 - 1].unk1C = sLights[i]->unkA * 2;
                        D_800BB198.x = sLights[i]->pos.x;
                        D_800BB198.y = sLights[i]->pos.y;
                        D_800BB198.z = sLights[i]->pos.z;
                        if (sLights[i]->unk6A & 8) {
                            bss_178[data_8 - 1].unkC = (f32) sLights[i]->unk76 - ((f32) sLights[i]->unk7E * (1.0f - var_fs4));
                            bss_178[data_8 - 1].unk10 = (f32) sLights[i]->unk77 - ((f32) sLights[i]->unk7E * (1.0f - var_fs4));
                            bss_178[data_8 - 1].unk14 = (f32) sLights[i]->unk78 - ((f32) sLights[i]->unk7E * (1.0f - var_fs4));
                        } else {
                            bss_178[data_8 - 1].unkC = (f32) sLights[i]->unk76;
                            bss_178[data_8 - 1].unk10 = (f32) sLights[i]->unk77;
                            bss_178[data_8 - 1].unk14 = (f32) sLights[i]->unk78;
                        }
                        if (var_fs0 != 1.0f) {
                            bss_178[data_8 - 1].unkC = (f32) (s32) (bss_178[data_8 - 1].unkC * var_fs0);
                            bss_178[data_8 - 1].unk10 = (f32) (s32) (bss_178[data_8 - 1].unk10 * var_fs0);
                            bss_178[data_8 - 1].unk14 = (f32) (s32) (bss_178[data_8 - 1].unk14 * var_fs0);
                        }
                        if (bss_178[data_8 - 1].unkC > 255.0f) {
                            bss_178[data_8 - 1].unkC = 255.0f;
                        } else if (bss_178[data_8 - 1].unkC < 0.0f) {
                            bss_178[data_8 - 1].unkC = 0.0f;
                        }
                        if (bss_178[data_8 - 1].unk10 > 255.0f) {
                            bss_178[data_8 - 1].unk10 = 255.0f;
                        } else if (bss_178[data_8 - 1].unk10 < 0.0f) {
                            bss_178[data_8 - 1].unk10 = 0.0f;
                        }
                        if (bss_178[data_8 - 1].unk14 > 255.0f) {
                            bss_178[data_8 - 1].unk14 = 255.0f;
                        } else if (bss_178[data_8 - 1].unk14 < 0.0f) {
                            bss_178[data_8 - 1].unk14 = 0.0f;
                        }
                    } else if (sLights[i]->unk6B & 0x40) {
                        diPrintf("too many tmoves\n");
                    }
                }
            }
        }
    }
}

static const char str_2[] = "warning in newlfx dll no spare lights available\n";
static const char str_3[] = "warning in newlfx dll no spare memory for light available\n";

// offset: 0x1890 | func: 8
static void newlfx_allocLight(LightAction* action, f32 x, f32 y, f32 z, Object* arg4, Object* arg5) {
    s32 found;
    s32 i;
    s32 paletteIdx;

    i = 0;
    found = FALSE;
    while ((i < (ARRAYCOUNT_S(sLights) - 1)) && !found) {
        if (sLights[i] == NULL) {
            found = TRUE;
        }
        i++;
    }
    i--;
    if (found) {
        sLights[i] = mmAlloc(sizeof(LfxLight), ALLOC_TAG_LFX_COL, NULL);
        if (sLights[i] != NULL) {
            sLights[i]->unk68 = action->unk1b;
            sLights[i]->unk34 = action->unk10;
            sLights[i]->unk38 = D_800B1845;
            sLights[i]->unk20 = arg5;
            sLights[i]->unk6A = action->unk12;
            sLights[i]->unk6B = action->unk13;
            sLights[i]->unk6C = action->unk14;
            sLights[i]->unk28 = action->unk0;
            sLights[i]->unk2C = action->unk0;
            sLights[i]->unk54 = (f32) action->unk4;
            sLights[i]->unk58 = (f32) action->unk6;
            sLights[i]->unk5C = (f32) action->unk8;
            sLights[i]->unk73 = action->unk15;
            sLights[i]->unk74 = action->unk16;
            sLights[i]->unk75 = action->unk17;
            sLights[i]->unk76 = action->unk18;
            sLights[i]->unk77 = action->unk19;
            sLights[i]->unk78 = action->unk1a;
            sLights[i]->unk3C = action->unk1c;
            if (arg4 != NULL) {
                sLights[i]->unk1C = arg4;
            }
            sLights[i]->unk40 = (f32) mathRnd(0, 180);
            sLights[i]->unk7E = mathRnd(action->unk1e, action->unk1d);
            if (action->unk22 == 10) {
                sLights[i]->unk44 = 0.16666667f;
            } else if (action->unk22) {
                sLights[i]->unk44 = 1.0f / ((f32) action->unk22 * 60.0f);
            } else {
                sLights[i]->unk44 = 1.0f / ((f32) mathRnd(1, 10) * 60.0f);
            }
            sLights[i]->unk7F = action->unk22;
            sLights[i]->unk7B = action->unk1d;
            sLights[i]->unk7C = action->unk1e;
            sLights[i]->unk6 = (s8) action->unka;
            sLights[i]->unk7 = (s8) action->unkC;
            sLights[i]->unk8 = action->unk1d;
            sLights[i]->unk9 = action->unk1e;
            sLights[i]->unkA = action->unk1f;
            sLights[i]->unkB = action->unk20;
            sLights[i]->unkC = 0;
            sLights[i]->unk7D = action->unk23;
            if (action->unk12 & 0x40) {
                sLights[i]->pos.x = x;
                sLights[i]->pos.y = y;
                sLights[i]->pos.z = z;
            } else if ((action->unk12 & 0x10) && (arg4 != NULL)) {
                sLights[i]->pos.x = arg4->srt.transl.x + (f32) action->unk4;
                sLights[i]->pos.y = arg4->srt.transl.y + (f32) action->unk6;
                sLights[i]->pos.z = arg4->srt.transl.z + (f32) action->unk8;
            } else {
                sLights[i]->pos.x = (f32) action->unk4 + x;
                sLights[i]->pos.y = (f32) action->unk6 + y;
                sLights[i]->pos.z = (f32) action->unk8 + z;
            }
            if (action->unk0 > 0) {
                if (D_80092C3C <= 0.0f) {
                    data_0 = 1;
                    D_80092C3C = 0.0f;
                    data_4 = 1.0f / (f32) action->unk0;
                    D_80092C3C += data_4;
                    D_800BB198.x = sLights[i]->pos.x;
                    D_800BB198.y = sLights[i]->pos.y;
                    D_800BB198.z = sLights[i]->pos.z;
                    sLights[i]->unk24 = action->unk0;
                }
            } else {
                sLights[i]->unk24 = 0;
            }
            if (action->unk12 & 0x20) {
                light_func_8001D548(action->unk15, action->unk16, action->unk17, action->unk18, action->unk19, action->unk1a, -1, 0);
                D_80090CA8 = 1;
            } else if (!(sLights[i]->unk6B & 1)) {
                if (newlfx_allocPalette(&paletteIdx, &action->unk15, 2) != 0) {
                    light_func_8001D548(action->unk15, action->unk16, action->unk17, action->unk18, action->unk19, action->unk1a, paletteIdx, 0);
                    sLights[i]->unkD = (u8) paletteIdx;
                    sLights[i]->paletteIdx = (u8) paletteIdx;
                }
            }
            sNumLights += 1;
        }
    }
}

static const char str_4[] = " Warning .. ! no spare palettes in dll newlfx.c\n";

// offset: 0x1EB0 | func: 9
static s32 newlfx_allocPalette(s32* paletteIdx, u8* pal, s32 type) {
    s32 i;
    s32 start;
    s32 end;

    if (type == 1) {
        start = 13;
        end = 15;
    } else if (type == 2) {
        start = 4;
        end = 12;
    }
    // @bug: start and end vars might be uninitialized
    for (i = start; i < end; i++) {
        if (!sPalettes[i].inUse) {
            *paletteIdx = i;
            sPalettes[i].inUse = TRUE;
            sPalettes[i].pal[0] = pal[0];
            sPalettes[i].pal[1] = pal[1];
            sPalettes[i].pal[2] = pal[2];
            sPalettes[i].pal[3] = pal[3];
            sPalettes[i].pal[4] = pal[4];
            sPalettes[i].pal[5] = pal[5];
            sPalettes[i].type = type;
            sPalettes[i].refCount = 1;
            return 1;
        }
    }
    for (i = start; i < end; i++) {
        if (   (((pal[0] - 10) < sPalettes[i].pal[0]) && (sPalettes[i].pal[0] < (pal[0] + 10)))
            && (((pal[1] - 10) < sPalettes[i].pal[1]) && (sPalettes[i].pal[1] < (pal[1] + 10)))
            && (((pal[2] - 10) < sPalettes[i].pal[2]) && (sPalettes[i].pal[2] < (pal[2] + 10)))
            && (((pal[3] - 10) < sPalettes[i].pal[3]) && (sPalettes[i].pal[3] < (pal[3] + 10)))
            && (((pal[4] - 10) < sPalettes[i].pal[4]) && (sPalettes[i].pal[4] < (pal[4] + 10)))
            && (((pal[5] - 10) < sPalettes[i].pal[5]) && (sPalettes[i].pal[5] < (pal[5] + 10)))
        ) {
            sPalettes[i].refCount += 1;
            *paletteIdx = i;
            return 1;
        }
    }
    return 0;
}

// offset: 0x2184 | func: 10
static s32 newlfx_findLight(s32 arg0) {
    s32 found;
    s32 i;

    i = 0;
    found = FALSE;
    while (i < ARRAYCOUNT_S(sLights) && !found) {
        if (sLights[i] != NULL && arg0 == sLights[i]->unk34) {
            found = TRUE;
        }
        i++;
    }
    i--;
    if (!found) {
        return -1;
    } else {
        return i;
    }
}

// offset: 0x21FC | func: 11
static void newlfx_freeLight(s32 arg0) {
    s32 found;
    s32 i;

    i = 0;
    found = FALSE;
    while (i < ARRAYCOUNT_S(sLights) && !found) {
        if ((sLights[i] != NULL) && (arg0 == sLights[i]->unk34)) {
            found = TRUE;
        }
        i++;
    }
    i--;
    if (found) {
        if (sLights[i]->unk6A & 0x20) {
            D_80090CA8 = 0;
        } else {
            sPalettes[sLights[i]->paletteIdx].refCount--;
            if (sPalettes[sLights[i]->paletteIdx].refCount <= 0) {
                sPalettes[sLights[i]->paletteIdx].inUse = FALSE;
            }
        }
        mmFree(sLights[i]);
        sLights[i] = NULL;
        sNumLights -= 1;
    }
}
