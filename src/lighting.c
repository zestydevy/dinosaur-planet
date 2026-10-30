#include "sys/camera.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/newshadows.h"
#include "sys/lighting.h"
#include "sys/lighting_asm.h"
#include "unktypes.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} Struct80090CE8;

// -------- .data start -------- // 80090c60
f32 D_80090C60 = 0.0f;
f32 D_80090C64 = 0.0f;
u8 D_80090C68 = 0;
u8 D_80090C6C = 0;
u8 D_80090C70 = 0;
u8 D_80090C74 = 0;
u8 D_80090C78 = 0;
u8 D_80090C7C = 0;
u8 D_80090C80 = 0;
u8 D_80090C84 = 0;
u8 D_80090C88 = 0;
u8 D_80090C8C = 0;
u8 D_80090C90 = 0;
u8 D_80090C94 = 0;
u8 D_80090C98 = 0;
s8 D_80090C9C = 0;
u8 gLastModLight = 0;
u8 gSkyLightAdditiveSet = 0;
s32 gDisableSkyLightUpdates = FALSE;
f32 gSkyLightPower = 1.0f;
u8 gInsideLighting = 0;
f32 gInsideLightT = 0.0f;
Struct80090CE8 D_80090CB8[16] = {0};
Struct80090CE8 D_80090CE8[16] = {0};
// func statics start here
// -------- .data end -------- // 80090D50

// -------- .bss start 800b1830 -------- //
u8 gSkyLightR;
u8 gSkyLightG;
u8 gSkyLightB;
f32 gSkyLightDirX;
f32 gSkyLightDirY;
f32 gSkyLightDirZ;
s8 D_800B1840;
s8 D_800B1841;
u8 D_800B1842;
u8 D_800B1843;
u8 D_800B1844;
u8 gNumModLights;
s8 D_800B1846;
u8 D_800B1847;
/**
 * 17 palettes of 256 RGB pairs that represent a lerp from one color to another.
 * There are 16 palettes used by different dynamic lights (newlfx, skylight, etc)
 * and a single special palette (index 0) used for the standard skylight color (newday).
 *
 * Note that the base skylight palette is usually referred to as slot -1 with slot 0
 * technically being at index 1. With that convention in mind, the slots are 
 * allocated as follows:
 *
 * -1: newday DLL base skylight (can be overriden by newlfx DLL)
 * 0, 1, 2, 3: unused
 * 4, 5, 6, 7, 8, 9: newlfx DLL lights
 * 10: initialized by skylight DLL but not used. normally used by newlfx DLL
 * 11: conflict! used by skylight DLL and newlfx DLL
 * 12, 13, 14: skylight DLL (note: newlfx can use 13, 14 but the code isn't wired up)
 * 15: unused
 */
u8* gModLightPalettes;
u8* gModLightSelectedPalette;
s16 sSkyLightAdditiveR;
s16 sSkyLightAdditiveG;
s16 sSkyLightAdditiveB;
ModelLight* gModLights;
Object* gModLightParents[30];
// -------- .bss end 800b18e0 -------- //

void lightSetModLightPalette2(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2, s32 slot, u8 baseSkylightAmt);

void lightInit(void) {
    s32 j;
    s32 i;
    void* ptr;

    // 30 gModLights slots
    // 17 gModLightPalettes slots (256 RGB pairs each)
    ptr = mmAlloc((sizeof(ModelLight) * 30) + (256 * 3 * 17), ALLOC_TAG_LFX_COL, NULL);
    D_800B1847 = 1;
    gModLights = ptr;
    gModLightPalettes = (u8 *) ((u32)ptr + (sizeof(ModelLight) * 30));
    for (i = -1, j = 0; i < 16; i++, j++) {
        lightSetModLightPalette2(
            D_80090CE8[j].unk0,
            D_80090CE8[j].unk1,
            D_80090CE8[j].unk2,
            D_80090CB8[j].unk0,
            D_80090CB8[j].unk1,
            D_80090CB8[j].unk2,
            i,
            0
        );
    }

    for (i = 0; i < 30; i++) {
        gModLightParents[i] = NULL;
    }
}

void light_func_8001CDE4(u8 r, u8 g, u8 b, u8 arg3, u8 arg4) {
    if (arg4 == 1) {
        D_80090C64 = 1.0f / arg3;
        D_80090C60 = 0.0f;
        if (D_80090C98 == 0) {
            D_80090C80 = gSkyLightR;
            D_80090C84 = gSkyLightG;
            D_80090C88 = gSkyLightB;
            D_80090C68 = gSkyLightR;
            D_80090C6C = gSkyLightG;
            D_80090C70 = gSkyLightB;
        }
        D_80090C74 = r;
        D_80090C78 = g;
        D_80090C7C = b;
        D_80090C98 = 1;
        return;
    }

    if (arg4 == 0 && (D_80090C98 == 1 || D_80090C98 == 3) && D_80090C60 <= 1.0f) {
        D_80090C68 = D_80090C80 + (D_80090C60 * (D_80090C74 - D_80090C80));
        D_80090C6C = D_80090C84 + (D_80090C60 * (D_80090C78 - D_80090C84));
        D_80090C70 = D_80090C88 + (D_80090C60 * (D_80090C7C - D_80090C88));
        D_80090C60 += D_80090C64;
        D_800B1847 = 1;
        if (D_80090C60 > 1.0f) {
            D_800B1847 = 0;
            if (D_80090C98 == 1) {
                D_80090C98 = 2;
                return;
            }
            if (D_80090C98 == 3) {
                D_80090C98 = 0;
            }
        }
        return;
    }

    if (arg4 == 3 && (D_80090C98 != 0)) {
        D_80090C64 = 1.0f / arg3;
        D_80090C60 = 0.0f;
        D_80090C74 = D_80090C8C;
        D_80090C78 = D_80090C90;
        D_80090C7C = D_80090C94;
        D_80090C80 = D_80090C68;
        D_80090C84 = D_80090C6C;
        D_80090C88 = D_80090C70;
        D_80090C98 = 3;
    }
}

void lightSetModLightPalette1(u8 r, u8 g, u8 b, s32 slot) {
    static u8 D_80090D18 = 0;
    static u8 D_80090D1C = 0;
    static u8 D_80090D20 = 0;

    s32 ci;
    s32 i;

    slot++;
    if (slot == 0) {
        if (D_80090D18 == r && D_80090D1C == g && D_80090D20 == b) {
            return;
        }

        D_80090D18 = r;
        D_80090D1C = g;
        D_80090D20 = b;
        D_800B1847 = 1;
    }

    ci = slot * (256 * 3);
    for (i = 0; i < 256; i++) {
        gModLightPalettes[ci++] = (r * i) / 256;
        gModLightPalettes[ci++] = (g * i) / 256;
        gModLightPalettes[ci++] = (b * i) / 256;
    }
}

void lightSetModLightPalette2(
        u8 r1, u8 g1, u8 b1, 
        u8 r2, u8 g2, u8 b2, 
        s32 slot, u8 baseSkylightAmt) {
    f32 skylightPct;
    f32 tValue;
    s32 v;
    s32 ci; // component index
    s32 i;

    // Precompute lerp from color 1 to 2
    v = 0;
    tValue = 0.00390625f; // (1.0f / 256.0f)
    slot++;
    ci = slot * (256 * 3);
    if (baseSkylightAmt != 0) {
        skylightPct = baseSkylightAmt / 255.0f;
        v = (gModLightPalettes[0] * r1) >> 8;
        if (v < 0) { v = 0; }
        r1 += skylightPct * (v - r1);

        v = (gModLightPalettes[1] * g1) >> 8;
        if (v < 0) { v = 0; }
        // @bug? shouldn't this be using g1?
        r1 += skylightPct * (v - r1);

        v = (gModLightPalettes[2] * b1) >> 8;
        if (v < 0) { v = 0; }
        // @bug? shouldn't this be using b1?
        r1 += skylightPct * (v - r1);

        v = (gModLightPalettes[(256 * 3) - 3] * r2) >> 8;
        if (v < 0) { v = 0; }
        r2 += skylightPct * (v - r2);

        v = (gModLightPalettes[(256 * 3) - 2] * g2) >> 8;
        if (v < 0) { v = 0; }
        g2 += skylightPct * (v - g2);

        v = (gModLightPalettes[(256 * 3) - 1] * b2) >> 8;
        if (v < 0) { v = 0; }
        b2 += skylightPct * (v - b2);
    }
    for (i = 0; i < 256; i++) {
        gModLightPalettes[ci++] = r1 + (tValue * (r2 - r1));
        gModLightPalettes[ci++] = g1 + (tValue * (g2 - g1));
        gModLightPalettes[ci++] = b1 + (tValue * (b2 - b1));
        tValue += 0.0039025f; // (1.0f / 256.0f)
    }
}

Vec3s32 D_80090D24 = {{0, 0, 0}};
void lightDynamicModelLighting(Object* obj, ModelInstance* modelInst) {
    s32 pad;
    s32 jointCount;
    s32 i;
    u8 spA3;
    u8 spA2;
    u8 spA1;
    s8 spA0;
    s8 sp9F;
    u8 sp9E;
    u8 sp9D;
    f32 skylightPower;
    Vec3f pos;
    Vec3s32 sp80 = D_80090D24;
    SRT srt;

    skylightPower = gSkyLightPower;
    if (skylightPower < 0.5f) {
        skylightPower = 0.5f;
    }
    // @fake
    if (modelInst->model) {}
    jointCount = modelInst->model->jointCount;
    if (jointCount == 0) {
        jointCount = 1;
    }
    // @fake
    if (1) {}
    obj->srt.transl.x -= gWorldX;
    obj->srt.transl.z -= gWorldZ;
    for (i = 0; i < gNumModLights; i++) {
        if (obj->parent != NULL) {
            if (gModLightParents[i] != NULL) {
                pos.x = gModLights[i].pos.x - gModLightParents[i]->srt.transl.x;
                pos.y = gModLights[i].pos.y - gModLightParents[i]->srt.transl.y;
                pos.z = gModLights[i].pos.z - gModLightParents[i]->srt.transl.z;
                srt.transl.x = 0.0f;
                srt.transl.y = 0.0f;
                srt.transl.z = 0.0f;
                srt.roll = 0;
                srt.pitch = 0;
                srt.scale = 1.0f;
                srt.yaw = -gModLightParents[i]->srt.yaw;
                mathRotateRPY(&srt, pos.f);
                sp80.x = pos.x;
                sp80.y = pos.y;
                sp80.z = pos.z;
                gModLights[i].x = sp80.x;
                gModLights[i].y = sp80.y;
                gModLights[i].z = sp80.z;
            } else {
                camInverseTransformPointByObject(
                    gModLights[i].pos.x, gModLights[i].pos.y, gModLights[i].pos.z, 
                    &pos.x, &pos.y, &pos.z, 
                    obj->parent);
                gModLights[i].x = pos.x;
                gModLights[i].y = pos.y;
                gModLights[i].z = pos.z;
            }
        } else if (gModLightParents[i] != NULL) {
            pos.x = gModLights[i].pos.x;
            pos.y = gModLights[i].pos.y;
            pos.z = gModLights[i].pos.z;
            sp80.x = pos.x;
            sp80.y = pos.y;
            sp80.z = pos.z;
            sp80.x -= gWorldX;
            sp80.z -= gWorldZ;
            gModLights[i].x = sp80.x;
            gModLights[i].y = sp80.y;
            gModLights[i].z = sp80.z;
        } else {
            gModLights[i].x = gModLights[i].pos.x - gWorldX;
            gModLights[i].y = gModLights[i].pos.y;
            gModLights[i].z = gModLights[i].pos.z - gWorldZ;
        }
    }
    D_800B1846 = 0;\
    if (1) {}
    // ^ @fake
    gModLightSelectedPalette = gModLightPalettes;
    if (obj->def->unk87 & 0xF) {
        // @bug: As far as I can tell, these are supposed to be indexing elements 15, 14, 13, and 12 but
        //       are indexing slightly too low. This is nearly impossible to notice in practice.
        if (obj->def->unk87 & 1) {
            // use slot 14 (skylight, 1-indexed)
            gModLightSelectedPalette += (14 * (256 * 3)) + (241 * 3); // colors[14][241] (should be colors[15][0])
        } else if (obj->def->unk87 & 2) {
            // use slot 13 (skylight, 1-indexed)
            gModLightSelectedPalette += (13 * (256 * 3)) + (242 * 3); // colors[13][242] (should be colors[14][0])
        } else if (obj->def->unk87 & 4) {
            // use slot 12 (skylight, 1-indexed)
            gModLightSelectedPalette += (12 * (256 * 3)) + (243 * 3); // colors[12][243] (should be colors[13][0])
        } else if (obj->def->unk87 & 8) {
            // use slot 11 (skylight, 1-indexed)
            gModLightSelectedPalette += (11 * (256 * 3)) + (244 * 3); // colors[11][244] (should be colors[12][0])
        }
        spA1 = D_800B1844;
        spA0 = D_800B1841;
        sp9F = D_800B1840;
        D_800B1844 = 1;
        sp9E = D_800B1842;
        sp9D = D_800B1843;
        if (gInsideLightT != 0.0f) {
            skylightPower = ((1.0f - skylightPower) * gInsideLightT) + skylightPower;
        }

        D_800B1843 = obj->def->unk86 * skylightPower;
        D_800B1842 = obj->def->unk85 * skylightPower;
        D_800B1840 = obj->def->unk84;
        D_800B1841 = obj->def->unk83;
    }
    if (obj->unkD5 & 0x20) {
        spA3 = D_800B1842;
        spA2 = D_800B1843;
        D_800B1843 = (D_800B1843 * obj->unkD7) >> 8;
        if (!(obj->def->unk87 & 0xF)) {
            D_800B1842 = (D_800B1842 * obj->unkD7) >> 8;
        }
    } else {
        if ((obj->unkD5 & 4) && (obj->unkD5 & 8)) {
            spA3 = D_800B1842;
            spA2 = D_800B1843;
            D_800B1843 = (D_800B1843 * obj->unkD7) >> 8;
            D_800B1842 = (D_800B1842 * obj->unkD7) >> 8;
        } else if (obj->unkD5 & 4) {
            spA3 = D_800B1842;
            spA2 = D_800B1843;
            D_800B1843 = (D_800B1843 * (255 - obj->unkD7)) >> 8;
            D_800B1842 = (D_800B1842 * (255 - obj->unkD7)) >> 8;
        } else if (obj->unkD5 & 8) {
            spA3 = D_800B1842;
            spA2 = D_800B1843;
            D_800B1843 = 0;
            D_800B1842 = 0;
        }
    }
    func_8001F890(
        modelInst->vertices[(modelInst->unk34 >> 1) & 1], 
        modelInst->model->vertexGroups, 
        jointCount, 
        modelInst->matrices[modelInst->unk34 & 1], 
        D_800B1842, 
        D_800B1843, 
        gModLightPalettes, 
        gModLights, 
        obj->visRadius);
    if (obj->def->unk87 & 0xF) {
        D_800B1844 = spA1;
        D_800B1843 = sp9D;
        D_800B1842 = sp9E;
        D_800B1841 = spA0;
        D_800B1840 = sp9F;
        gModLightSelectedPalette = NULL;
    }
    D_80090C9C = 0;
    if (obj->unkD5 & 0x20) {
        D_800B1843 = spA2;
        D_800B1842 = spA3;
    } else if (obj->unkD5 & 4) {
        D_800B1843 = spA2;
        D_800B1842 = spA3;
    } else if (obj->unkD5 & 8) {
        D_800B1843 = spA2;
        D_800B1842 = spA3;
    }
    obj->srt.transl.x += gWorldX;
    obj->srt.transl.z += gWorldZ;
}

typedef struct {
    u8 pad0;
    u8 unk1;
} UnkSp30;

void light_func_8001E818(Object* arg0, Model* arg1, ModelInstance* arg2) {
    ModelFacebatch* faceBatch;
    Vtx* vtx;
    s32 sp5C;
    s32 toVtxID;
    s32 fromVtxID;
    s32 j;
    s32 temp;
    s32 temp2;
    s32 temp3;
    s32 r, g, b;
    Vtx* sp34;
    UnkSp30 *sp30;
    s32 i;
    u8 *new_var;

    sp34 = arg0->unk70;
    sp30 = arg0->def->pTextures;
    sp5C = arg0->def->numAnimatedFrames;
    vtx = arg2->vertices[(arg2->unk34 >> 1) & 1];
    for (i = 0; i < sp5C; i++) {
        new_var = &(sp30 = sp30)[i].unk1;
        for (j = 0; j < arg1->textureAnimationCount; j += 1) {
            faceBatch = &arg1->faces[((TextureAnimation*)arg1->textureAnimations)[j].unkB];
            if (faceBatch->tagB == *new_var) {
                fromVtxID = faceBatch->baseVertexID;
                toVtxID = faceBatch[1].baseVertexID;
                r = sp34[i].v.cn[0];
                g = sp34[i].v.cn[1];
                b = sp34[i].v.cn[2];
                if (r == 0 && g == 0 && b == 0) {
                    continue;
                }
                temp = (gSkyLightR + r) >> 1;
                temp2 = (gSkyLightG + g) >> 1;
                temp3 = (gSkyLightB + b) >> 1;
                for (; fromVtxID < toVtxID; fromVtxID++) {
                    if ((arg1->vertices[fromVtxID].v.cn[3] == 0) || (arg1->vertices[fromVtxID].v.cn[3] == 1)) {
                        continue;
                    }
                    if ((arg1->vertices[fromVtxID].v.cn[3] == 4) || (arg1->vertices[fromVtxID].v.cn[3] == 5)) {
                        vtx[fromVtxID].v.cn[0] = (temp * arg1->vertices[fromVtxID].v.cn[0]) >> 8;
                        vtx[fromVtxID].v.cn[1] = (temp2 * arg1->vertices[fromVtxID].v.cn[1]) >> 8;
                        vtx[fromVtxID].v.cn[2] = (temp3 * arg1->vertices[fromVtxID].v.cn[2]) >> 8;
                    } else {
                        vtx[fromVtxID].v.cn[0] = (arg1->vertices[fromVtxID].v.cn[0] * r) >> 8;
                        vtx[fromVtxID].v.cn[1] = (arg1->vertices[fromVtxID].v.cn[1] * g) >> 8;
                        vtx[fromVtxID].v.cn[2] = (arg1->vertices[fromVtxID].v.cn[2] * b) >> 8;
                    }
                }
            }
        }
        // @fake
        if (arg1 && arg1) {}
    }
}

void lightDimSkyLight(u8 r, u8 g, u8 b, f32 t) {
    sSkyLightAdditiveR = (s16) (s32) -((f32) r * t);
    sSkyLightAdditiveG = (s16) (s32) -((f32) g * t);
    sSkyLightAdditiveB = (s16) (s32) -((f32) b * t);
    gSkyLightAdditiveSet = 1;
}

void lightClearEmitters(void) {
    s32 i;

    gNumModLights = 0;
    D_800B1846 = 1;
    for (i = 0; i < 30; i++) {
        gModLightParents[i] = NULL;
    }
}

// official name: newlightInside
void lightSetInside(s32 inside) {
    gInsideLighting = (u8) inside;
}

s32 lightGetInside(void) {
    return gInsideLighting;
}

void lightUpdateSkyLight(f32 dirX, f32 dirY, f32 dirZ, u8 arg3, u8 arg4, s8 arg5, s8 arg6, u8 r, u8 g, u8 b) {
    static s16 D_80090D3C = 0;
    static u8 sPrevInsideLighting = 0;
    s16 sumB;
    s16 sumR;
    s16 sumG;
    u8 temp;
    Vec3f insideLightDir = VEC3F(0.0f, 198.0f, 19.0f);
    f32 insideLightTransitionRate;

    if (gDisableSkyLightUpdates) {
        return;
    }
    D_80090D3C += 600.0f;
    if (arg4 < arg3) {
        arg3 = arg4;
    }
    if (arg6 >= arg5) {
        if (arg5 < 0x7F) {
            arg5 += 1;
        } else {
            arg6 -= 1;
        }
    }
    if (sPrevInsideLighting != gInsideLighting) {
        sPrevInsideLighting = gInsideLighting;
    }
    insideLightTransitionRate = 0.003f;
    if ((gInsideLighting) && (gInsideLightT != 1.0f)) {
        // Transition into inside lighting
        gInsideLightT += (gUpdateRateF * insideLightTransitionRate);
        if (gInsideLightT > 1.0f) {
            gInsideLightT = 1.0f;
        }
        gSkyLightDirX = ((insideLightDir.x - dirX) * gInsideLightT) + dirX;
        gSkyLightDirY = ((insideLightDir.y - dirY) * gInsideLightT) + dirY;
        gSkyLightDirZ = ((insideLightDir.z - dirZ) * gInsideLightT) + dirZ;
    } else if (!gInsideLighting && (gInsideLightT != 0.0f)) {
        // Transition out of inside lighting
        gInsideLightT -= gUpdateRateF * insideLightTransitionRate;
        if (gInsideLightT < 0.0f) {
            gInsideLightT = 0.0f;
        }
        gSkyLightDirX = ((insideLightDir.x - dirX) * gInsideLightT) + dirX;
        gSkyLightDirY = ((insideLightDir.y - dirY) * gInsideLightT) + dirY;
        gSkyLightDirZ = ((insideLightDir.z - dirZ) * gInsideLightT) + dirZ;
    } else if (gInsideLighting) {
        // Fully in inside lighting
        gSkyLightDirX = insideLightDir.x;
        gSkyLightDirY = insideLightDir.y;
        gSkyLightDirZ = insideLightDir.z;
    } else {
        // Fully in outside lighting
        gSkyLightDirX = dirX;
        gSkyLightDirY = dirY;
        gSkyLightDirZ = dirZ;
    }
    D_800B1842 = arg3;
    D_800B1843 = arg4;
    D_800B1840 = arg5;
    D_800B1841 = arg6;
    gSkyLightR = r;
    gSkyLightG = g;
    gSkyLightB = b;
    D_80090C8C = gSkyLightR;
    D_80090C90 = gSkyLightG;
    D_80090C94 = gSkyLightB;
    light_func_8001CDE4(0, 0, 0, 0, 0);
    if (D_80090C98 != 0) {
        gSkyLightR = D_80090C68;
        gSkyLightG = D_80090C6C;
        gSkyLightB = D_80090C70;
    } else {
        gSkyLightR = r;
        gSkyLightG = g;
        gSkyLightB = b;
    }
    if (gSkyLightAdditiveSet) {
        sumR = gSkyLightR + sSkyLightAdditiveR;
        sumG = gSkyLightG + sSkyLightAdditiveG;
        sumB = gSkyLightB + sSkyLightAdditiveB;
        sumR = MIN(255, sumR);
        sumG = MIN(255, sumG);
        sumB = MIN(255, sumB);
        sumR = MAX(0, sumR);
        sumG = MAX(0, sumG);
        sumB = MAX(0, sumB);
        gSkyLightR = sumR;
        gSkyLightG = sumG;
        gSkyLightB = sumB;
    }
    // @fake?
    if (gSkyLightR) {}
    if (gSkyLightG) {}
    temp = gSkyLightB;
    if (temp) {}
    if ((gSkyLightR != 0xFF) || (gSkyLightG != 0xFF) || (temp != 0xFF)) {
        D_800B1844 = 1;
        lightSetModLightPalette1(gSkyLightR, gSkyLightG, temp, -1);
    } else {
        D_800B1844 = 0;
    }
    gSkyLightAdditiveSet = 0;
    sSkyLightAdditiveR = 0;
    sSkyLightAdditiveG = 0;
    sSkyLightAdditiveB = 0;
}

void lightModelSphereMapping(ModelInstance* arg0) {
    Camera* camera;
    Model* spF0;
    f32 spEC;
    f32 spE8;
    f32 spE4;
    Vec3f spD8;
    f32 spD4; // fs0
    f32 spD0; // fs1
    f32 spCC; // fs2
    f32 temp_fa1;
    f32 temp_fv0;
    Vec3f *temp_s8;
    MtxF* spBC; // s0
    f32 var_fv1;
    s32 var_s2;
    s32 var_s4;
    s32 var_s7;
    s32 spA8;
    s32 spA4;
    s8 var_s3;
    s32 var_s5;
    s32 sp98;
    f32 temp_fa0;
    s8* temp_v0_4;
    Vtx* sp8C;
    f32 temp_ft4;
    f32 pad;

    if (track_func_80041D8C() == 0) {
        return;
    }

    camera = camGetMain();
    spF0 = arg0->model;
    spEC = camera->srt.transl.f[0] - gWorldX;
    spE8 = camera->srt.transl.f[1];
    spE4 = camera->srt.transl.f[2] - gWorldZ;
    sp98 = -1;
    sp8C = arg0->vertices[((s32) arg0->unk34 >> 1) & 1];
    for (spA4 = 0; spA4 < spF0->unk70; spA4++) {
        var_s7 = 0;
        if (spF0->faces[spA4].tagC & 0x4000) {
            var_s2 = spF0->faces[spA4].baseVertexID;
            spA8 = spF0->faces[spA4 + 1].baseVertexID;
            var_s3 = spF0->faces[spA4].jointID_A;
            var_s4 = 0;
            var_s5 = spF0->faces[spA4].jointID_B_firstVertexBufferIndex;
            while (var_s2 < spA8) {
                if (var_s7 >= var_s5) {
                    var_s3 = ((ModelFacebatch*)((u8*)&spF0->faces[spA4] + var_s4))->jointID_A;
                    var_s4 += 1;
                    if (var_s4 < 2) {
                        var_s5 = spF0->faces[spA4].jointID_C_firstVertexBufferIndex;
                    } else {
                        var_s5 = spA8 - var_s2;
                        // @fake
                        if (1) {}
                    }
                }
                temp_v0_4 = (s8*)spF0->vertexGroups + *((s16*)spF0->vertexGroupOffsets + var_s2);
                spD8.f[0] = temp_v0_4[0];
                temp_v0_4 += 2;
                spD8.f[1] = temp_v0_4[-1];
                spD8.f[2] = temp_v0_4[0];
                if (var_s3 != sp98) {
                    spBC = arg0->matrices[arg0->unk34 & 1];
                    spBC = (MtxF*) ((f32*)spBC + (var_s3 << 4));
                    spD4 = spEC - spBC->m[3][0];
                    spD0 = spE8 - spBC->m[3][1];
                    spCC = spE4 - spBC->m[3][2];
                    temp_fv0 = sqrtf(SQ(spD4) + SQ(spD0) + SQ(spCC));
                    sp98 = var_s3;
                    if (temp_fv0 != 0.0f) {
                        var_fv1 = 1.0f / temp_fv0;
                    } else {
                        var_fv1 = 0.0f;
                    }
                    spD4 *= var_fv1;
                    spD0 *= var_fv1;
                    spCC *= var_fv1;

                    temp_fv0 = D_800BB198.f[0] - spBC->m[3][0];
                    temp_fa1 = D_800BB198.f[1] - spBC->m[3][1];
                    temp_ft4 = D_800BB198.f[2] - spBC->m[3][2];
                    temp_fv0 = sqrtf(SQ(temp_fv0) + SQ(temp_fa1) + SQ(temp_ft4));
                }
                mathMtxFastXFMF(spBC, &spD8, &spD8);
                temp_fv0 = VECTOR_MAGNITUDE(spD8);
                if (temp_fv0 != 0.0f) {
                    var_fv1 = 1.0f / temp_fv0;
                } else {
                    var_fv1 = 0.0f;
                }
                // @fake
                if (1) {}
                VECTOR_SCALE(spD8, var_fv1);
                var_fv1 = 2.0f * ((spD8.f[0] * spD4) + (spD8.f[1] * spD0) + (spD8.f[2] * spCC));
                temp_fa0 = ((spD8.f[0] * var_fv1) - spD4);
                pad = ((spD8.f[1] * var_fv1) - spD0);
                sp8C[var_s2].v.tc[0] = (temp_fa0 + 1.0f) * 256.0f;
                sp8C[var_s2].v.tc[1] = (pad + 1.0f) * 256.0f;
                var_s2 += 1;
                var_s7 += 1;
            }
        }
    }
}

// update sphere mapping (e.g. ice reflections) for block
void lightBlockSphereMapping(Block* block, s32 blockX, s32 blockZ) {
    u8 pad[0x18];
    Vec3f sp9C;
    Vec3f sp88;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    f32 sp70;
    Camera* camera;
    Vtx_t* temp_fp;
    f32 temp_fa1;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fs2;
    f32 temp_fs3;
    f32 temp_fs4;
    f32 temp_fs5;
    f32 temp_ft4;
    f32 temp_fv0;
    f32 var_fv1;
    s32 targetVertexIndex;
    s32 vertexIndex;
    s32 i;
    BlockShape* shape;
    f32 temp_fa0;
    f32 temp;
    f32 temp2;

    camera = camGetMain();
    
    sp9C.f[2] = camera->srt.transl.f[0] - ((blockX * 640.0f) + gWorldX);
    sp9C.f[1] = camera->srt.transl.f[1];
    sp9C.f[0] = camera->srt.transl.f[2] - ((blockZ * 640.0f) + gWorldZ);
    
    temp_fp = block->vertices2[(block->vtxFlags & 1) ^ 1];
    for (i = 0; i < block->shapeCount; i++) {
        shape = &block->shapes[i];
        if ((shape->flags & RENDER_SHAPE_SPHERE_MAP) && (shape->flags & RENDER_SHAPE_VISIBLE)) {
            vertexIndex = shape->vtxBase;
            
            targetVertexIndex = shape[1].vtxBase;
            while (vertexIndex < targetVertexIndex) {
                sp88.f[0] = 0.0f;
                sp88.f[1] = 1.0f;
                sp88.f[2] = 0.0f;
                temp_fs3 = block->vertices[vertexIndex].ob[0];
                temp_fs4 = block->vertices[vertexIndex].ob[1];
                temp_fs5 = block->vertices[vertexIndex].ob[2];
                temp_fs0 = sp9C.f[2] - temp_fs3;
                temp_fs1 = sp9C.f[1] - temp_fs4;
                temp_fs2 = sp9C.f[0] - temp_fs5;
                temp_fv0 = sqrtf(SQ(temp_fs0) + SQ(temp_fs1) + SQ(temp_fs2));
                if (temp_fv0 != 0.0f) {
                    var_fv1 = 1.0f / temp_fv0;
                } else {
                    var_fv1 = 0.0f;
                }

                sp84 = temp_fs0 * var_fv1;
                sp80 = temp_fs1 * var_fv1;
                sp7C = temp_fs2 * var_fv1;
                temp_fv0 = sqrtf(SQ(D_800BB198.x - temp_fs3) + SQ(D_800BB198.y - temp_fs4) + SQ(D_800BB198.z - temp_fs5));

                // @fake
                if (1) {}
                temp_fv0 = VECTOR_MAGNITUDE(sp88);
                if (temp_fv0 != 0.0f) {
                    var_fv1 = 1.0f / temp_fv0;
                } else {
                    var_fv1 = 0.0f;
                }
                // @fake
                if (1) {}
                VECTOR_SCALE(sp88, var_fv1);

                var_fv1 = 2.0f * ((sp88.f[0] * sp84) + (sp88.f[1] * sp80) + (sp88.f[2] * sp7C));
                temp_fa0 = ((sp88.f[0] * var_fv1) - sp84);
                temp_fa1 = ((sp88.f[1] * var_fv1) - sp80);
                temp_fp[vertexIndex].tc[0] = (temp_fa0 + 1.0f) * 1024.0f;
                temp_fp[vertexIndex].tc[1] = (temp_fa1 + 1.0f) * 1024.0f;
                vertexIndex += 1;
            }
            // @fake
            do {} while (0);
        }
    }
}

void lightGetSkyLight(u8* r, u8* g, u8* b) {
    *r = gSkyLightR;
    *g = gSkyLightG;
    *b = gSkyLightB;
}

void lightSkyLightDL(Gfx** gdl) {
    dlSetPrimColor(gdl, gSkyLightR, gSkyLightG, gSkyLightB, 255);
}
