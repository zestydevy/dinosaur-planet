#include "common.h"

// -------- .data start -------- // 80090c60

extern u8 BYTE_80090cb0;

// -------- .data end -------- // 80090D50

// -------- .bss start -------- //

extern u8 D_800B1830;
extern u8 D_800B1831;
extern u8 D_800B1832;

// -------- .bss end -------- //

// TODO: or is this the bss for this file?
extern Vec3f D_800BB198;

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1D900/func_8001CD00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1D900/func_8001CDE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1D900/func_8001D2A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1D900/func_8001D548.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1D900/func_8001DF60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1D900/func_8001E818.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1D900/func_8001EAA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1D900/func_8001EB80.s")

void func_8001EBD0(s32 arg0) {
    BYTE_80090cb0 = (u8) arg0;
}

u8 func_8001EBE0() {
    return BYTE_80090cb0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_1D900/func_8001EBF0.s")

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
    ModelInstance_0x4* sp8C;
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
    sp8C = arg0->unk4[((s32) arg0->unk34 >> 1) & 1];
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
                sp8C[var_s2].unk8 = (temp_fa0 + 1.0f) * 256.0f;
                sp8C[var_s2].unkA = (pad + 1.0f) * 256.0f;
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
