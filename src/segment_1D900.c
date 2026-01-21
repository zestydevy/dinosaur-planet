#include "common.h"
#include "unktypes.h"

typedef struct { u8 unk0[0x3300]; } Unk800B1848;

// size: 0x1C
typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[0x10-0x6];
    Vec3f pos;
} Unk800B1858;
// size: 0x1C?
typedef struct {
    s16 yaw;
    u8 pad6[0xC-0x2];
    Vec3f pos;
    u32 pad1C;
} Unk800B1860;

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
u8 D_80090CA0 = 0;
u8 D_80090CA4 = 0;
s32 D_80090CA8 = 0;
f32 D_80090CAC = 1.0f;
u8 BYTE_80090cb0 = 0;
f32 D_80090CB4 = 0.0f;
Struct80090CE8 D_80090CB8[16] = {0};
Struct80090CE8 D_80090CE8[16] = {0};
u8 D_80090D18 = 0;
u8 D_80090D1C = 0;
u8 D_80090D20 = 0;
Vec3s32 D_80090D24 = {0, 0, 0};
// func statics start here
// -------- .data end -------- // 80090D50

// -------- .bss start 800b1830 -------- //
u8 D_800B1830;
u8 D_800B1831;
u8 D_800B1832;
f32 D_800B1834;
f32 D_800B1838;
f32 D_800B183C;
s8 D_800B1840;
s8 D_800B1841;
u8 D_800B1842;
u8 D_800B1843;
u8 D_800B1844;
u8 D_800B1845;
s8 D_800B1846;
u8 D_800B1847;
Unk800B1848 *D_800B1848;
Unk800B1848 *D_800B184C;
s16 D_800B1850;
s16 D_800B1852;
s16 D_800B1854;
Unk800B1858 *D_800B1858;
Unk800B1860 *D_800B1860[30];
// -------- .bss end 800b18e0 -------- //

extern Vec3f D_800BB198;

void func_8001D548(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7); /* extern */
void func_8001F890(Vtx *, void *, s32, MtxF *, s32, s32, Unk800B1848*, void *, f32);

void func_8001CD00(void) {
    s32 j;
    s32 i;
    UNK_PTR* temp_v0;

    temp_v0 = mmAlloc(0x3648, 0x12, NULL);
    D_800B1847 = 1;
    D_800B1858 = temp_v0;
    D_800B1848 = (Unk800B1848 *) ((u32)temp_v0 + 0x348);
    for (i = -1, j = 0; i < 16; i++, j++) {
        func_8001D548(
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
        D_800B1860[i] = NULL;
    }
}

void func_8001CDE4(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    if (arg4 == 1) {
        D_80090C64 = 1.0f / arg3;
        D_80090C60 = 0.0f;
        if (D_80090C98 == 0) {
            D_80090C80 = D_800B1830;
            D_80090C84 = D_800B1831;
            D_80090C88 = D_800B1832;
            D_80090C68 = D_800B1830;
            D_80090C6C = D_800B1831;
            D_80090C70 = D_800B1832;
        }
        D_80090C74 = arg0;
        D_80090C78 = arg1;
        D_80090C7C = arg2;
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

void func_8001D2A8(u8 arg0, u8 arg1, u8 arg2, s32 arg3) {
    s32 var_t1;
    s32 var_t2;
    u8 *temp;

    arg3++;
    if (arg3 == 0) {
        // @fake?
        temp = &D_80090D18;
        if (*temp == arg0 && D_80090D1C == arg1 && D_80090D20 == arg2) {
            return;
        }

        *temp = arg0;
        D_80090D1C = arg1;
        D_80090D20 = arg2;
        D_800B1847 = 1;
    }

    var_t1 = arg3 * 0x300;
    for (var_t2 = 0; var_t2 < 0x100; var_t2++) {
        D_800B1848->unk0[var_t1++] = (arg0 * var_t2) / 0x100;
        D_800B1848->unk0[var_t1++] = (arg1 * var_t2) / 0x100;
        D_800B1848->unk0[var_t1++] = (arg2 * var_t2) / 0x100;
    }
}

void func_8001D548(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7) {
    f32 temp_fv1;
    f32 var_fv0;
    s32 i;
    s32 var_v1;

    // @fake
    i = 0;
    arg6++;
    var_fv0 = 0.00390625f;
    var_v1 = arg6 * 0x300;
    if (arg7 != 0) {
        temp_fv1 =  arg7 / 255.0f;
        i = (D_800B1848->unk0[0] * arg0) >> 8;
        if (i < 0) { i = 0; }
        arg0 += temp_fv1 * (i - arg0);

        i = (D_800B1848->unk0[1] * arg1) >> 8;
        if (i < 0) { i = 0; }
        arg0 += temp_fv1 * (i - arg0);

        i = (D_800B1848->unk0[2] * arg2) >> 8;
        if (i < 0) { i = 0; }
        arg0 += temp_fv1 * (i - arg0);

        i = (D_800B1848->unk0[0x2FD] * arg3) >> 8;
        if (i < 0) { i = 0; }
        arg3 += temp_fv1 * (i - arg3);

        i = (D_800B1848->unk0[0x2FE] * arg4) >> 8;
        if (i < 0) { i = 0; }
        arg4 += temp_fv1 * (i - arg4);

        i = (D_800B1848->unk0[0x2FF] * arg5) >> 8;
        if (i < 0) { i = 0; }
        arg5 += temp_fv1 * (i - arg5);
    }
    for (i = 0; i < 0x100; i++) {
        D_800B1848->unk0[var_v1++] = arg0 + (var_fv0 * (arg3 - arg0));
        D_800B1848->unk0[var_v1++] = arg1 + (var_fv0 * (arg4 - arg1));
        D_800B1848->unk0[var_v1++] = arg2 + (var_fv0 * (arg5 - arg2));
        var_fv0 += 0.0039025f;
    }
}

void func_8001DF60(Object* obj, ModelInstance* modelInst) {
    s32 pad;
    s32 spA8;
    s32 i;
    u8 spA3;
    u8 spA2;
    u8 spA1;
    s8 spA0;
    s8 sp9F;
    u8 sp9E;
    u8 sp9D;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    Vec3s32 sp80 = D_80090D24;
    SRT sp68;

    sp98 = D_80090CAC;
    if (sp98 < 0.5f) {
        sp98 = 0.5f;
    }
    // @fake
    if (modelInst->model) {}
    spA8 = modelInst->model->jointCount;
    if (spA8 == 0) {
        spA8 = 1;
    }
    // @fake
    if (1) {}
    obj->srt.transl.x -= gWorldX;
    obj->srt.transl.z -= gWorldZ;
    for (i = 0; i < D_800B1845; i++) {
        if (obj->parent != NULL) {
            if (D_800B1860[i] != 0) {
                sp8C = D_800B1858[i].pos.x - D_800B1860[i]->pos.x;
                sp90 = D_800B1858[i].pos.y - D_800B1860[i]->pos.y;
                sp94 = D_800B1858[i].pos.z - D_800B1860[i]->pos.z;
                sp68.transl.x = 0.0f;
                sp68.transl.y = 0.0f;
                sp68.transl.z = 0.0f;
                sp68.roll = 0;
                sp68.pitch = 0;
                sp68.scale = 1.0f;
                sp68.yaw = -D_800B1860[i]->yaw;
                rotate_vec3(&sp68, &sp8C);
                sp80.x = sp8C;
                sp80.y = sp90;
                sp80.z = sp94;
                D_800B1858[i].x = sp80.x;
                D_800B1858[i].y = sp80.y;
                D_800B1858[i].z = sp80.z;
            } else {
                inverse_transform_point_by_object(D_800B1858[i].pos.x, D_800B1858[i].pos.y, D_800B1858[i].pos.z, &sp8C, &sp90, &sp94, obj->parent);
                D_800B1858[i].x = sp8C;
                D_800B1858[i].y = sp90;
                D_800B1858[i].z = sp94;
            }
        } else if (D_800B1860[i] != 0) {
            sp8C = D_800B1858[i].pos.x;
            sp90 = D_800B1858[i].pos.y;
            sp94 = D_800B1858[i].pos.z;
            sp80.x = sp8C;
            sp80.y = sp90;
            sp80.z = sp94;
            sp80.x -= gWorldX;
            sp80.z -= gWorldZ;
            D_800B1858[i].x = sp80.x;
            D_800B1858[i].y = sp80.y;
            D_800B1858[i].z = sp80.z;
        } else {
            D_800B1858[i].x = D_800B1858[i].pos.x - gWorldX;
            D_800B1858[i].y = D_800B1858[i].pos.y;
            D_800B1858[i].z = D_800B1858[i].pos.z - gWorldZ;
        }
    }
    D_800B1846 = 0;\
    if (1) {}
    // ^ @fake
    D_800B184C = D_800B1848;
    if (obj->def->unk87 & 0xF) {
        if (obj->def->unk87 & 1) {
            // wtf is this
            D_800B184C = (Unk800B1848*)&D_800B184C->unk0[0x2CD3];
        } else if (obj->def->unk87 & 2) {
            // wtf is this
            D_800B184C = (Unk800B1848*)&D_800B184C->unk0[0x29D6];
        } else if (obj->def->unk87 & 4) {
            // wtf is this
            D_800B184C = (Unk800B1848*)&D_800B184C->unk0[0x26D9];
        } else if (obj->def->unk87 & 8) {
            // wtf is this
            D_800B184C = (Unk800B1848*)&D_800B184C->unk0[0x23DC];
        }
        spA1 = D_800B1844;
        spA0 = D_800B1841;
        sp9F = D_800B1840;
        D_800B1844 = 1;
        sp9E = D_800B1842;
        sp9D = D_800B1843;
        if (D_80090CB4 != 0.0f) {
            sp98 = ((1.0f - sp98) * D_80090CB4) + sp98;
        }

        D_800B1843 = obj->def->unk86 * sp98;
        D_800B1842 = obj->def->unk85 * sp98;
        D_800B1840 = obj->def->unk84;
        D_800B1841 = obj->def->unk83;
    }
    if (obj->unkD5[0] & 0x20) {
        spA3 = D_800B1842;
        spA2 = D_800B1843;
        D_800B1843 = (D_800B1843 * obj->unkD7[0]) >> (0, 8);
        if (!(obj->def->unk87 & 0xF)) {
            D_800B1842 = (D_800B1842 * obj->unkD7[0]) >> (0, 8);
        }
    } else {
        if ((obj->unkD5[0] & 4) && (obj->unkD5[0] & 8)) {
            spA3 = D_800B1842;
            spA2 = D_800B1843;
            D_800B1843 = (D_800B1843 * obj->unkD7[0]) >> (0, 8);
            D_800B1842 = (D_800B1842 * obj->unkD7[0]) >> (0, 8);
        } else if (obj->unkD5[0] & 4) {
            spA3 = D_800B1842;
            spA2 = D_800B1843;
            D_800B1843 = (D_800B1843 * (0xFF - obj->unkD7[0])) >> (0, 8);
            D_800B1842 = (D_800B1842 * (0xFF - obj->unkD7[0])) >> (0, 8);
        } else if (obj->unkD5[0] & 8) {
            spA3 = D_800B1842;
            spA2 = D_800B1843;
            D_800B1843 = 0;
            D_800B1842 = 0;
        }
    }
    func_8001F890(modelInst->vertices[(modelInst->unk34 >> 1) & 1], modelInst->model->vertexGroups, spA8, modelInst->matrices[modelInst->unk34 & 1], (s32) D_800B1842, (s32) D_800B1843, D_800B1848, D_800B1858, obj->unkA8);
    if (obj->def->unk87 & 0xF) {
        D_800B1844 = spA1;
        D_800B1843 = sp9D;
        D_800B1842 = sp9E;
        D_800B1841 = spA0;
        D_800B1840 = sp9F;
        D_800B184C = NULL;
    }
    D_80090C9C = 0;
    if (obj->unkD5[0] & 0x20) {
        D_800B1843 = spA2;
        D_800B1842 = spA3;
    } else if (obj->unkD5[0] & 4) {
        D_800B1843 = spA2;
        D_800B1842 = spA3;
    } else if (obj->unkD5[0] & 8) {
        D_800B1843 = spA2;
        D_800B1842 = spA3;
    }
    obj->srt.transl.f[0] += gWorldX;
    obj->srt.transl.f[2] += gWorldZ;
}

typedef struct {
    u8 pad0;
    u8 unk1;
} UnkSp30;

void func_8001E818(Object* arg0, Model* arg1, ModelInstance* arg2) {
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
                temp = (D_800B1830 + r) >> 1;
                temp2 = (D_800B1831 + g) >> 1;
                temp3 = (D_800B1832 + b) >> 1;
                for (; fromVtxID < toVtxID; fromVtxID++) {
                    if ((arg1->vertices[fromVtxID].n.a == 0) || (arg1->vertices[fromVtxID].n.a == 1)) {
                        continue;
                    }
                    if ((arg1->vertices[fromVtxID].n.a == 4) || (arg1->vertices[fromVtxID].n.a == 5)) {
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

void func_8001EAA4(u8 arg0, u8 arg1, u8 arg2, f32 arg3) {
    D_800B1850 = (s16) (s32) -((f32) arg0 * arg3);
    D_800B1852 = (s16) (s32) -((f32) arg1 * arg3);
    D_800B1854 = (s16) (s32) -((f32) arg2 * arg3);
    D_80090CA4 = 1;
}

void func_8001EB80(void) {
    s32 i;

    D_800B1845 = 0;
    D_800B1846 = 1;
    for (i = 0; i < 30; i++) {
        D_800B1860[i] = NULL;
    }
}

void func_8001EBD0(s32 arg0) {
    BYTE_80090cb0 = (u8) arg0;
}

u8 func_8001EBE0(void) {
    return BYTE_80090cb0;
}

#ifndef NON_MATCHING
Vec3f D_80090D30 = { 0.0f, 198.0f, 19.0f };
s16 D_80090D3C = 0;
u8 D_80090D40 = 0;
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1D900/func_8001EBF0.s")
#else
// https://decomp.me/scratch/j3stU
void func_8001EBF0(f32 arg0, f32 arg1, f32 arg2, u8 arg3, u8 arg4, s8 arg5, s8 arg6, u8 arg7, u8 arg8, u8 arg9) {
    static s16 D_80090D3C = 0;
    static u8 D_80090D40 = 0;
    s16 var_a0;
    s16 var_v0;
    s16 var_v1;
    Vec3f sp2C = { 0.0f, 198.0f, 19.0f }; // D_80090D30

    if (D_80090CA8 != 0) {
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
    if (D_80090D40 != BYTE_80090cb0) {
        D_80090D40 = BYTE_80090cb0;
    }
    if ((BYTE_80090cb0) && (D_80090CB4 != 1.0f)) {
        D_80090CB4 += (gUpdateRateF * 0.003f);
        if (D_80090CB4 > 1.0f) {
            D_80090CB4 = 1.0f;
        }
        D_800B1834 = ((sp2C.x - arg0) * D_80090CB4) + arg0;
        D_800B1838 = ((sp2C.y - arg1) * D_80090CB4) + arg1;
        D_800B183C = ((sp2C.z - arg2) * D_80090CB4) + arg2;
    } else if (!BYTE_80090cb0 && (D_80090CB4 != 0.0f)) {
        D_80090CB4 -= gUpdateRateF * 0.003f;
        if (D_80090CB4 < 0.0f) {
            D_80090CB4 = 0.0f;
        }
        D_800B1834 = ((sp2C.x - arg0) * D_80090CB4) + arg0;
        D_800B1838 = ((sp2C.y - arg1) * D_80090CB4) + arg1;
        D_800B183C = ((sp2C.z - arg2) * D_80090CB4) + arg2;
    } else if (BYTE_80090cb0) {
        D_800B1834 = sp2C.x;
        D_800B1838 = sp2C.y;
        D_800B183C = sp2C.z;
    } else {
        D_800B1834 = arg0;
        D_800B1838 = arg1;
        D_800B183C = arg2;
    }
    D_800B1842 = arg3;
    D_800B1843 = arg4;
    D_800B1840 = arg5;
    D_800B1841 = arg6;
    D_800B1830 = arg7;
    D_800B1831 = arg8;
    D_800B1832 = arg9;
    D_80090C8C = D_800B1830;
    D_80090C90 = D_800B1831;
    D_80090C94 = D_800B1832;
    func_8001CDE4(0, 0, 0, 0, 0);
    if (D_80090C98 != 0) {
        D_800B1830 = D_80090C68;
        D_800B1831 = D_80090C6C;
        D_800B1832 = D_80090C70;
    } else {
        D_800B1830 = arg7;
        D_800B1831 = arg8;
        D_800B1832 = arg9;
    }
    if (D_80090CA4 != 0) {
        var_v0 = D_800B1830 + D_800B1850;
        var_v1 = D_800B1831 + D_800B1852;
        var_a0 = D_800B1832 + D_800B1854;
        var_v0 = MIN(0xFF, var_v0);
        var_v1 = MIN(0xFF, var_v1);
        var_a0 = MIN(0xFF, var_a0);
        var_v0 = MAX(0, var_v0);
        var_v1 = MAX(0, var_v1);
        var_a0 = MAX(0, var_a0);
        D_800B1830 = var_v0;
        D_800B1831 = var_v1;
        D_800B1832 = var_a0;
    }
    // @fake?
    if (D_800B1830) {}
    if (D_800B1831) {}
    if (D_800B1832) {}
    if ((D_800B1830 != 0xFF) || (D_800B1831 != 0xFF) || (D_800B1832 != 0xFF)) {
        D_800B1844 = 1;
        func_8001D2A8(D_800B1830, D_800B1831, D_800B1832, -1);
    } else {
        D_800B1844 = 0;
    }
    D_80090CA4 = 0;
    D_800B1850 = 0;
    D_800B1852 = 0;
    D_800B1854 = 0;
}
#endif

void func_8001F094(ModelInstance* arg0) {
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

    if (func_80041D8C() == 0) {
        return;
    }

    camera = get_main_camera();
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
                vec3_transform_no_translate(spBC, &spD8, &spD8);
                temp_fv0 = sqrtf(SQ(spD8.f[0]) + SQ(spD8.f[1]) + SQ(spD8.f[2]));
                if (temp_fv0 != 0.0f) {
                    var_fv1 = 1.0f / temp_fv0;
                } else {
                    var_fv1 = 0.0f;
                }
                // @fake
                if (1) {}
                spD8.f[0] *= var_fv1;
                spD8.f[1] *= var_fv1;
                spD8.f[2] *= var_fv1;
                var_fv1 = 2.0f * ((spD8.f[0] * spD4) + (spD8.f[1] * spD0) + (spD8.f[2] * spCC));
                temp_fa0 = ((spD8.f[0] * var_fv1) - spD4);
                pad = ((spD8.f[1] * var_fv1) - spD0);
                sp8C[var_s2].n.tc[0] = (temp_fa0 + 1.0f) * 256.0f;
                sp8C[var_s2].n.tc[1] = (pad + 1.0f) * 256.0f;
                var_s2 += 1;
                var_s7 += 1;
            }
        }
    }
}

// update sphere mapping (e.g. ice reflections) for block
void func_8001F4C0(Block* arg0, s32 arg1, s32 arg2) {
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

    camera = get_main_camera();
    
    sp9C.f[2] = camera->srt.transl.f[0] - ((arg1 * 640.0f) + gWorldX);
    sp9C.f[1] = camera->srt.transl.f[1];
    sp9C.f[0] = camera->srt.transl.f[2] - ((arg2 * 640.0f) + gWorldZ);
    
    temp_fp = arg0->vertices2[(arg0->vtxFlags & 1) ^ 1];
    for (i = 0; i < arg0->shapeCount; i++) {
        shape = &arg0->shapes[i];
        if ((shape->flags & 0x4000) && (shape->flags & 0x10000000)) {
            vertexIndex = shape->vtxBase;
            
            targetVertexIndex = shape[1].vtxBase;
            while (vertexIndex < targetVertexIndex) {
                sp88.f[0] = 0.0f;
                sp88.f[1] = 1.0f;
                sp88.f[2] = 0.0f;
                temp_fs3 = arg0->vertices[vertexIndex].ob[0];
                temp_fs4 = arg0->vertices[vertexIndex].ob[1];
                temp_fs5 = arg0->vertices[vertexIndex].ob[2];
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
                temp_fv0 = sqrtf(SQ(sp88.f[0]) + SQ(sp88.f[1]) + SQ(sp88.f[2]));
                if (temp_fv0 != 0.0f) {
                    var_fv1 = 1.0f / temp_fv0;
                } else {
                    var_fv1 = 0.0f;
                }
                // @fake
                if (1) {}
                sp88.f[0] *= var_fv1;
                sp88.f[1] *= var_fv1;
                sp88.f[2] *= var_fv1;

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

void func_8001F81C(u8 *arg0, u8 *arg1, u8 *arg2) {
    *arg0 = D_800B1830;
    *arg1 = D_800B1831;
    *arg2 = D_800B1832;
}

void func_8001F848(Gfx **gdl) {
    dl_set_prim_color(gdl, D_800B1830, D_800B1831, D_800B1832, 0xFF);
}
