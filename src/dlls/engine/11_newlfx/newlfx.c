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
/*0A*/ u8 maxRange;
/*0B*/ u8 minRange;
/*0C*/ u8 unkC;
/*0D*/ u8 lightSlot;
/*10*/ Vec3f pos;
/*1C*/ Object* unk1C;
/*20*/ Object* unk20;
/*24*/ s32 unk24;
/*28*/ s32 timer;
/*2C*/ s32 time;
/*30*/ u8 _unk30[0x34 - 0x30];
/*34*/ s32 slot;
/*38*/ s32 unk38;
/*3C*/ s32 unk3C;
/*40*/ f32 unk40;
/*44*/ f32 unk44;
/*48*/ u8 _unk48[0x54 - 0x48];
/*54*/ f32 velX;
/*58*/ f32 velY;
/*5C*/ f32 velZ;
/*60*/ u8 _unk60[0x68 - 0x60];
/*68*/ s16 type;
/*6A*/ u8 flags1;
/*6B*/ u8 flags2;
/*6C*/ u8 flags3;
/*6D*/ u8 _unk6D[0x73 - 0x6D];
/*73*/ u8 r1;
/*74*/ u8 g1;
/*75*/ u8 b1;
/*76*/ u8 r2;
/*77*/ u8 g2;
/*78*/ u8 b2;
/*79*/ u8 paletteIdx;
/*7A*/ u8 unk7A;
/*7B*/ u8 rndMax;
/*7C*/ u8 rndMin;
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
        if (action->slot != 0xFFFE) {
            sp34 = newlfx_findLight(action->slot);
        }
        if (sp34 == -1) {
            if (!(action->flags & 2) && (action->flags & 1)) {
                if (!(action->flags & 0x40) && (arg0->def->unk87 & 0x10)) {
                    action->slot = data_18 + 11;
                    data_18++;
                    if (data_18 >= 16) {
                        data_18 = 0;
                    }
                    arg0->unkD6 = (u8) action->slot;
                    newlfx_allocLight(action, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, arg1, arg0);
                    return;
                }
                if (action->slot == 0xFFFE) {
                    action->slot = data_18 + 11;
                    data_18++;
                    if (data_18 >= 16) {
                        data_18 = 0;
                    }
                    if (!(action->flags & 0x40)) {
                        arg0->unkD6 = (u8) action->slot;
                        newlfx_allocLight(action, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, arg1, arg0);
                        return;
                    }
                    newlfx_allocLight(action, arg5->transl.x, arg5->transl.y, arg5->transl.z, arg1, arg0);
                    return;
                }
                if ((arg5 != NULL) && (action->flags & 0x40)) {
                    newlfx_allocLight(action, arg5->transl.x, arg5->transl.y, arg5->transl.z, arg1, arg0);
                    return;
                }
                if (arg0->parent != NULL) {
                    newlfx_allocLight(action, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, arg1, arg0);
                } else {
                    newlfx_allocLight(action, arg0->globalPosition.x, arg0->globalPosition.y, arg0->globalPosition.z, arg1, arg0);
                }
                if (action->type == 0) {
                    plLightActions->newlfx = action->id - 1;
                }
            }
        } else {
            if (action->flags & 1) {
                if (action->flags2 & 8) {
                    sLights[sp34]->velX = (f32) action->x;
                    sLights[sp34]->velY = (f32) action->y;
                    sLights[sp34]->velZ = (f32) action->z;
                }
            } else if (action->flags & 2) {
                newlfx_freeLight(action->slot);
                if (action->type == 0) {
                    plLightActions->newlfx = action->id - 1;
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
    Object* obj;
    s32 i;

    newlfx_updateLights();
    data_0 = 0;
    for (i = 0; i < ARRAYCOUNT_S(sLights); i++) {
        if (sLights[i] != NULL) {
            if (!(sLights[i]->flags2 & 1) && (sLights[i]->unk24 != 0) && (data_0 == 0)) {
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
            if (gNumModLights < 30) {
                gLastModLight = gNumModLights;
                if (var_a2){} // @fake
                gModLights[gLastModLight].pos.x = sLights[i]->pos.x;
                gModLights[gLastModLight].pos.y = sLights[i]->pos.y;
                gModLights[gLastModLight].pos.z = sLights[i]->pos.z;
                gModLights[gLastModLight].unk6 = sLights[i]->unk6;
                gModLights[gLastModLight].unk7 = sLights[i]->unk7;
                gModLights[gLastModLight].unk9 = sLights[i]->unk9;
                gModLights[gLastModLight].unk8 = sLights[i]->unk8;
                gModLights[gLastModLight].unkA = sLights[i]->maxRange;
                gModLights[gLastModLight].unkB = sLights[i]->minRange;
                gModLights[gLastModLight].unkC = var_a2;
                gModLights[gLastModLight].unkD = sLights[i]->lightSlot + 1;
                gNumModLights += 1;
            }
            obj = sLights[i]->unk20;
            if ((obj != NULL) && (obj->parent != NULL)) {
                gModLightParents[i] = obj->parent;
            } else {
                gModLightParents[i] = NULL;
            }
        } else {
            gModLightParents[i] = NULL;
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
NewLfxStruct* newlfx_Func_828(s32 arg0, u8* count) {
    if (data_8 != 0) {
        if (count != NULL) {
            *count = data_8;
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
f32 newlfx_Func_9D8(s32 slot, f32* arg1) {
    f32 temp_fv0;
    s32 i;

    if (arg1 == NULL) {
        return -1.0f;
    }
    for (i = 0; i < ARRAYCOUNT_S(sLights); i++) {
        if ((sLights[i] != NULL) && (slot == sLights[i]->slot) && (sLights[i]->flags1 & 8)) {
            temp_fv0 = sLights[i]->unk40 / 180.0f;
            if (temp_fv0 <= 0.5f) {
                temp_fv0 = temp_fv0 / 0.5f;
                temp_fv0 = 1.0f - temp_fv0;
            } else if (temp_fv0 > 0.5f) {
                temp_fv0 = (temp_fv0 - 0.5f) / 0.5f;
            }
            *arg1 = (f32) sLights[i]->unk8 / (f32) sLights[i]->rndMax;
            return temp_fv0;
        }
    }
    return -1.0f;
}

// offset: 0xBB8 | func: 6 | export: 6
s32 newlfx_Func_BB8(s32 slot, s16 arg1, s16 arg2) {
    s32 i;

    if (arg1 > 255) {
        arg1 = 255;
    }
    if (arg2 > 255) {
        arg2 = 255;
    }
    for (i = 0; i < ARRAYCOUNT_S(sLights); i++) {
        if ((sLights[i] != NULL) && (slot == sLights[i]->slot)) {
            sLights[i]->minRange = (u8) arg1;
            sLights[i]->maxRange = (u8) arg2;
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
            if (sLights[i]->time != 0) {
                var_fs0 = 1.0f - ((f32) sLights[i]->time / (f32) sLights[i]->timer);
                if (var_fs0 < 0.0f) {
                    var_fs0 = 0.001f;
                } else if (var_fs0 > 1.0f) {
                    var_fs0 = 1.0f;
                }
                temp = (f32) sLights[i]->rndMax * var_fs0;
                sLights[i]->unk8 = (u8) temp;
                temp = (f32) sLights[i]->rndMin * var_fs0;
                sLights[i]->unk9 = (u8) temp;
                sLights[i]->time -= gUpdateRate;
                if (sLights[i]->time < 0) {
                    sLights[i]->time = 0;
                }
            }
            if (sLights[i]->flags1 & 8) {
                sLights[i]->unk40 += (180.0f * sLights[i]->unk44) * (f32) gUpdateRate;
                if (sLights[i]->unk40 > 180.0f) {
                    sLights[i]->unk40 = 0.0f;
                    if (!(sLights[i]->flags2 & 1)) {
                        if (sLights[i]->unk7D == 0) {
                            sLights[i]->unk7E = (u8) ((f32) (sLights[i]->rndMax - mathRnd(sLights[i]->rndMin, sLights[i]->rndMax)) * 0.5f);
                        } else {
                            sLights[i]->unk7E = sLights[i]->rndMin + mathRnd(sLights[i]->rndMin, sLights[i]->unk7D);
                        }
                    } else {
                        if (sLights[i]->unk7D == 0) {
                            sLights[i]->unk7E = (u8) ((f32) (sLights[i]->rndMax - mathRnd(sLights[i]->rndMin, sLights[i]->rndMax)) * 0.5f);
                        } else {
                            sLights[i]->unk7E = sLights[i]->rndMin + mathRnd(sLights[i]->rndMin, sLights[i]->unk7D);
                        }
                    }
                }
                var_fs4 = (f32) mathCosInterp((s16) (sLights[i]->unk40 * 182.04166f)) / 65535.0f;
                if (var_fs4 < 0.0f) {
                    var_fs4 *= -1.0f;
                    sLights[i]->unk8 = (u8) ((f32) sLights[i]->rndMax - ((f32) sLights[i]->unk7E * (1.0f - var_fs4)));
                    if (sLights[i]->unk8 < sLights[i]->unk9) {
                        sLights[i]->unk8 = sLights[i]->unk9;
                    }
                }
            }
            if (sLights[i]->flags1 & 0x40) {
                sLights[i]->pos.x += sLights[i]->velX;
                sLights[i]->pos.y += sLights[i]->velY;
                sLights[i]->pos.z += sLights[i]->velZ;
            } else if ((sLights[i]->flags1 & 0x10) && (sLights[i]->unk3C != 0)) {
                if (sLights[i]->unk1C != NULL) {
                    if (sLights[i]->flags1 & 4) {
                        sp88.yaw = sLights[i]->unk1C->srt.yaw;
                        sp88.pitch = sLights[i]->unk1C->srt.pitch;
                        sp88.roll = sLights[i]->unk1C->srt.roll;
                        sp7C.f[0] = sLights[i]->velX;
                        sp7C.f[1] = sLights[i]->velY;
                        sp7C.f[2] = sLights[i]->velZ;
                        mathRotateRPY(&sp88, sp7C.f);
                        sLights[i]->pos.x = sLights[i]->unk1C->globalPosition.x + sp7C.f[0];
                        sLights[i]->pos.y = sLights[i]->unk1C->globalPosition.y + sp7C.f[1];
                        sLights[i]->pos.z = sLights[i]->unk1C->globalPosition.z + sp7C.f[2];
                    } else {
                        sLights[i]->pos.x = sLights[i]->unk1C->globalPosition.x + sLights[i]->velX;
                        sLights[i]->pos.y = sLights[i]->unk1C->globalPosition.y + sLights[i]->velY;
                        sLights[i]->pos.z = sLights[i]->unk1C->globalPosition.z + sLights[i]->velZ;
                    }
                    D_800BB198.x = sLights[i]->pos.x;
                    D_800BB198.y = sLights[i]->pos.y;
                    D_800BB198.z = sLights[i]->pos.z;
                    if ((sLights[i]->flags2 & 0x40) && (data_8 < 4)) {
                        data_8 += 1;
                        bss_178[data_8 - 1].pos.f[0] = sLights[i]->pos.x - gWorldX;
                        bss_178[data_8 - 1].pos.f[1] = sLights[i]->pos.y;
                        bss_178[data_8 - 1].pos.f[2] = sLights[i]->pos.z - gWorldZ;
                        bss_178[data_8 - 1].unk18 = sLights[i]->minRange * 2;
                        bss_178[data_8 - 1].unk1C = sLights[i]->maxRange * 2;
                        D_800BB198.x = sLights[i]->pos.x;
                        D_800BB198.y = sLights[i]->pos.y;
                        D_800BB198.z = sLights[i]->pos.z;
                        if (sLights[i]->flags1 & 8) {
                            bss_178[data_8 - 1].r = (f32) sLights[i]->r2 - ((f32) sLights[i]->unk7E * (1.0f - var_fs4));
                            bss_178[data_8 - 1].g = (f32) sLights[i]->g2 - ((f32) sLights[i]->unk7E * (1.0f - var_fs4));
                            bss_178[data_8 - 1].b = (f32) sLights[i]->b2 - ((f32) sLights[i]->unk7E * (1.0f - var_fs4));
                        } else {
                            bss_178[data_8 - 1].r = (f32) sLights[i]->r2;
                            bss_178[data_8 - 1].g = (f32) sLights[i]->g2;
                            bss_178[data_8 - 1].b = (f32) sLights[i]->b2;
                        }
                        if (var_fs0 != 1.0f) {
                            bss_178[data_8 - 1].r = (f32) (s32) (bss_178[data_8 - 1].r * var_fs0);
                            bss_178[data_8 - 1].g = (f32) (s32) (bss_178[data_8 - 1].g * var_fs0);
                            bss_178[data_8 - 1].b = (f32) (s32) (bss_178[data_8 - 1].b * var_fs0);
                        }
                        if (bss_178[data_8 - 1].r > 255.0f) {
                            bss_178[data_8 - 1].r = 255.0f;
                        } else if (bss_178[data_8 - 1].r < 0.0f) {
                            bss_178[data_8 - 1].r = 0.0f;
                        }
                        if (bss_178[data_8 - 1].g > 255.0f) {
                            bss_178[data_8 - 1].g = 255.0f;
                        } else if (bss_178[data_8 - 1].g < 0.0f) {
                            bss_178[data_8 - 1].g = 0.0f;
                        }
                        if (bss_178[data_8 - 1].b > 255.0f) {
                            bss_178[data_8 - 1].b = 255.0f;
                        } else if (bss_178[data_8 - 1].b < 0.0f) {
                            bss_178[data_8 - 1].b = 0.0f;
                        }
                    } else if (sLights[i]->flags2 & 0x40) {
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
            sLights[i]->type = action->type;
            sLights[i]->slot = action->slot;
            sLights[i]->unk38 = gNumModLights;
            sLights[i]->unk20 = arg5;
            sLights[i]->flags1 = action->flags;
            sLights[i]->flags2 = action->flags2;
            sLights[i]->flags3 = action->flags3;
            sLights[i]->timer = action->timer;
            sLights[i]->time = action->timer;
            sLights[i]->velX = (f32) action->x;
            sLights[i]->velY = (f32) action->y;
            sLights[i]->velZ = (f32) action->z;
            sLights[i]->r1 = action->r1;
            sLights[i]->g1 = action->g1;
            sLights[i]->b1 = action->b1;
            sLights[i]->r2 = action->r2;
            sLights[i]->g2 = action->g2;
            sLights[i]->b2 = action->b2;
            sLights[i]->unk3C = action->unk1c;
            if (arg4 != NULL) {
                sLights[i]->unk1C = arg4;
            }
            sLights[i]->unk40 = (f32) mathRnd(0, 180);
            sLights[i]->unk7E = mathRnd(action->min, action->max);
            if (action->speed == 10) {
                sLights[i]->unk44 = 0.16666667f;
            } else if (action->speed) {
                sLights[i]->unk44 = 1.0f / ((f32) action->speed * 60.0f);
            } else {
                sLights[i]->unk44 = 1.0f / ((f32) mathRnd(1, 10) * 60.0f);
            }
            sLights[i]->unk7F = action->speed;
            sLights[i]->rndMax = action->max;
            sLights[i]->rndMin = action->min;
            sLights[i]->unk6 = (s8) action->unka;
            sLights[i]->unk7 = (s8) action->unkC;
            sLights[i]->unk8 = action->max;
            sLights[i]->unk9 = action->min;
            sLights[i]->maxRange = action->maxRange;
            sLights[i]->minRange = action->minRange;
            sLights[i]->unkC = 0;
            sLights[i]->unk7D = action->unk23;
            if (action->flags & 0x40) {
                sLights[i]->pos.x = x;
                sLights[i]->pos.y = y;
                sLights[i]->pos.z = z;
            } else if ((action->flags & 0x10) && (arg4 != NULL)) {
                sLights[i]->pos.x = arg4->srt.transl.x + (f32) action->x;
                sLights[i]->pos.y = arg4->srt.transl.y + (f32) action->y;
                sLights[i]->pos.z = arg4->srt.transl.z + (f32) action->z;
            } else {
                sLights[i]->pos.x = (f32) action->x + x;
                sLights[i]->pos.y = (f32) action->y + y;
                sLights[i]->pos.z = (f32) action->z + z;
            }
            if (action->timer > 0) {
                if (D_80092C3C <= 0.0f) {
                    data_0 = 1;
                    D_80092C3C = 0.0f;
                    data_4 = 1.0f / (f32) action->timer;
                    D_80092C3C += data_4;
                    D_800BB198.x = sLights[i]->pos.x;
                    D_800BB198.y = sLights[i]->pos.y;
                    D_800BB198.z = sLights[i]->pos.z;
                    sLights[i]->unk24 = action->timer;
                }
            } else {
                sLights[i]->unk24 = 0;
            }
            if (action->flags & 0x20) {
                // Override base skylight
                lightSetModLightPalette2(action->r1, action->g1, action->b1, action->r2, action->g2, action->b2, -1, 0);
                gDisableSkyLightUpdates = TRUE;
            } else if (!(sLights[i]->flags2 & 1)) {
                if (newlfx_allocPalette(&paletteIdx, &action->r1, 2) != 0) {
                    lightSetModLightPalette2(action->r1, action->g1, action->b1, action->r2, action->g2, action->b2, paletteIdx, 0);
                    sLights[i]->lightSlot = (u8) paletteIdx;
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
        start = 13; // skylight palettes (13, 14)
        end = 15;
    } else if (type == 2) {
        start = 4; // newlfx palettes (4, 5, 6, 7, 8, 9, 10, 11)
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
static s32 newlfx_findLight(s32 slot) {
    s32 found;
    s32 i;

    i = 0;
    found = FALSE;
    while (i < ARRAYCOUNT_S(sLights) && !found) {
        if (sLights[i] != NULL && slot == sLights[i]->slot) {
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
static void newlfx_freeLight(s32 slot) {
    s32 found;
    s32 i;

    i = 0;
    found = FALSE;
    while (i < ARRAYCOUNT_S(sLights) && !found) {
        if ((sLights[i] != NULL) && (slot == sLights[i]->slot)) {
            found = TRUE;
        }
        i++;
    }
    i--;
    if (found) {
        if (sLights[i]->flags1 & 0x20) {
            gDisableSkyLightUpdates = FALSE;
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
