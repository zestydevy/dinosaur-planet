#include "common.h"
#include "sys/newshadows.h"

static const char str_8009a9d0[] = "shadows: group overflow error\n";
static const char str_8009a9f0[] = "newshadows.c: max lift planes exceeded\n";

// -------- .bss start 800b97e0 -------- //
f32 D_800B97E0[24];
f32 D_800B9840[24];
Gfx *D_800B98A0[2];
Unk800B98A8 *D_800B98A8[2];
Unk8004FA58_Arg5 *D_800B98B0[2];
s16 D_800B98B8[300];
u8 D_800B9B10[80];
s16 D_800B9B60[800];
Unk8004FA58 D_800BA1A0[200];
Unk800B98A8 *D_800BB140;
Unk800B98A8 *D_800BB144;
Unk8004FA58_Arg5 *D_800BB148;
Unk8004FA58_Arg5 *D_800BB14C;
Gfx *D_800BB150;
Gfx *D_800BB154;
Gfx *D_800BB158[2];
Unk8004FA58_Arg5 *D_800BB160[2];
Unk800BB168 *D_800BB168[2];
s8 D_800BB170;
Unk800BB168* D_800BB174;
Unk800BB168* D_800BB178;
Unk8004FA58_Arg5* D_800BB17C;
Unk8004FA58_Arg5* D_800BB180;
Gfx* D_800BB184;
Gfx* D_800BB188;
f32 D_800BB18C;
Texture* D_800BB190;
Vec3f D_800BB198;
Vec3f D_800BB1A8;
u8 _bss_800bb1b8[0x48];
// -------- .bss end 800bb200 -------- //

void func_8004D470(void) {
    void *temp_v0;

    D_80092BE8 = 0xA;
    temp_v0 = (void *) mmAlloc(0x8020, ALLOC_TAG_SHAD_COL, NULL);
    D_800B98A0[0] = (Gfx *)                    temp_v0;
    D_800B98A0[1] = (Gfx *)              ((u32)temp_v0 + sizeof(Unk800B98A0));
    D_800B98A8[0] = (Unk800B98A8 *)      ((u32)temp_v0 + sizeof(Unk800B98A0) * 2);
    D_800B98A8[1] = (Unk800B98A8 *)      ((u32)temp_v0 + sizeof(Unk800B98A0) * 2 + sizeof(Unk800B98A8));
    D_800B98B0[0] = (Unk8004FA58_Arg5 *) ((u32)temp_v0 + sizeof(Unk800B98A0) * 2 + sizeof(Unk800B98A8) * 2);
    D_800B98B0[1] = (Unk8004FA58_Arg5 *) ((u32)temp_v0 + sizeof(Unk800B98A0) * 2 + sizeof(Unk800B98A8) * 2 + sizeof(Unk800B98B0));

    temp_v0 = (void *) mmAlloc(0xC800, ALLOC_TAG_SHAD_COL, NULL);
    D_800BB158[0] = (Gfx *)                    temp_v0;
    D_800BB158[1] = (Gfx *)              ((u32)temp_v0 + sizeof(Unk800BB158));
    D_800BB168[0] = (Unk800BB168 *)      ((u32)temp_v0 + sizeof(Unk800BB158) * 2);
    D_800BB168[1] = (Unk800BB168 *)      ((u32)temp_v0 + sizeof(Unk800BB158) * 2 + sizeof(Unk800BB168));
    D_800BB160[0] = (Unk8004FA58_Arg5 *) ((u32)temp_v0 + sizeof(Unk800BB158) * 2 + sizeof(Unk800BB168) * 2);
    D_800BB160[1] = (Unk8004FA58_Arg5 *) ((u32)temp_v0 + sizeof(Unk800BB158) * 2 + sizeof(Unk800BB168) * 2 + sizeof(Unk800BB160));

    D_800B9840[0] = -8.0f;
    D_800B9840[1] = 0.0f;
    D_800B9840[3] = -8.0f;
    D_800B9840[2] = 0.0f;
    D_800B9840[4] = 16.0f;
    D_800B9840[5] = 0.0f;
    D_800B9840[6] = 8.0f;
    D_800B9840[7] = 16.0f;
    D_800B9840[8] = 0.0f;
    D_800B9840[9] = 8.0f;
    D_800B9840[10] = 0.0f;
    D_800B9840[11] = 0.0f;

    D_800B97E0[0] = D_800B9840[0];
    D_800B97E0[1] = D_800B9840[1];
    D_800B97E0[2] = D_800B9840[2];
    D_800B97E0[3] = D_800B9840[3];
    D_800B97E0[4] = D_800B9840[4];
    D_800B97E0[5] = D_800B9840[5];
    D_800B97E0[6] = D_800B9840[6];
    D_800B97E0[7] = D_800B9840[7];
    D_800B97E0[8] = D_800B9840[8];
    D_800B97E0[9] = D_800B9840[9];
    D_800B97E0[10] = D_800B9840[10];
    D_800B97E0[11] = D_800B9840[11];

    D_800B97E0[12] = -8.0f;
    D_800B97E0[13] = 0.0f;
    D_800B97E0[14] = 100.0f;
    D_800B97E0[15] = -8.0f;
    D_800B97E0[16] = 16.0f;
    D_800B97E0[17] = 100.0f;
    D_800B97E0[18] = 8.0f;
    D_800B97E0[19] = 16.0f;
    D_800B97E0[20] = 100.0f;
    D_800B97E0[21] = 8.0f;
    D_800B97E0[22] = 0.0f;
    D_800B97E0[23] = 100.0f;

    D_800B9840[12] = -6.0f;
    D_800B9840[13] = 0.0f;
    D_800B9840[14] = 55.0f;
    D_800B9840[15] = -6.0f;
    D_800B9840[16] = 16.0f;
    D_800B9840[17] = 55.0f;
    D_800B9840[18] = 6.0f;
    D_800B9840[19] = 16.0f;
    D_800B9840[20] = 55.0f;
    D_800B9840[21] = 6.0f;
    D_800B9840[22] = 0.0f;
    D_800B9840[23] = 55.0f;
    func_8005B870();
    D_800BB190 = queue_load_texture_proxy(0xD8);
}

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004D698.s")
#else
extern f32 D_800BB1B0;
void func_8004D698(f32 arg0, f32 arg1, f32 arg2, s32 arg3) {
    Vec3f sp1C;
    f32 var_fv0;

    if (func_8001EBE0() != 0) {
        arg1 = 198.0f;
        arg0 = 0.0f;
        arg2 = 19.0f;
    }
    sp1C.x = arg0;
    sp1C.y = arg1;
    sp1C.z = arg2;
    D_80092C18 = arg3;
    D_80092BD0 = arg3 * arg0;
    D_80092BD4 = arg3 * arg1;
    if (arg1 < 0.0f) {
        D_80092BE4 = 1.0f + arg1;
        if (1) {
            D_80092BE4 = 1.0f;
        }
    } else {
        D_80092BE4 = 1.0f;
    }
    if (D_80092BD4 < 80.0f) {
        D_80092BD4 = 80.0f;
    }
    D_80092BD8 = arg3 * arg2;
    func_80052230(&sp1C, &D_800BB1A8, &D_80092C44);
    var_fv0 = D_80092C44;
    if (var_fv0 < 0.0f) {
        var_fv0 *= -1.0f;
    }
    D_80092C44 = var_fv0;
    if (var_fv0 <= 0.9961900115f) { // probably .99f
        D_80092C40 = 1;
    }
    if (D_80092C40 != 0) {
        D_800BB1A8.x = sp1C.x;
        D_800BB1A8.y = sp1C.y;
        D_800BB1A8.z = sp1C.z;
        D_80092C40 = 0;
        func_8004D974(1);
    }
}
#endif

void func_8004D844(Object *obj, f32 x, f32 y, f32 z)
{
    ObjectStruct64 *temp_v0 = obj->unk64;

    if (temp_v0) {
        temp_v0->tr.x = x;
        temp_v0->tr.y = y;
        temp_v0->tr.z = z;
        temp_v0->flags |= 0x20;
    }
}

void func_8004D880(Object *arg0) {
    ObjectStruct64* temp_v0;

    if (arg0->unk64 != NULL) {
        arg0->unk64->flags &= ~0x20;
    }
}

u32 func_8004D8A4(Object* obj, u32 addr, s32 arg2) {
    ObjectStruct64 *obj64;

    obj64 = (ObjectStruct64 *) mmAlign4(addr);
    obj->unk64 = obj64;
    addr = (u32)(obj64 + 1);
    obj64->unk4 = texture_load((s32) -obj->def->shadowTexture, 0);
    obj64->unk0 = obj->def->unk00;
    obj64->unk2c = obj64->unk0;
    obj64->unk14.x = D_80092BD0;
    obj64->unk14.y = D_80092BD4;
    obj64->unk14.z = D_80092BD8;
    obj64->unk36 = 0x40;
    obj64->flags = 4;
    obj64->unk38 = 0x19;
    obj64->unk39 = 0x4B;
    obj64->unk3A = 0x96;
    obj64->unk3B = 0x64;
    func_8004D974(1);
    return addr;
}


void func_8004D974(s32 arg0) {
    D_80092BE8 = (s8) arg0;
}

void func_8004D984(s32 arg0) {
    D_80092BE8 = 1;
}

void func_8004D99C(s32 arg0) {
    D_80092BF8 = arg0;
}

void func_8004D9AC(s32 arg0) {
}

void func_8004D9B8(void) {
    D_80092C2C = 0;
    D_80092C34 = 0;
    D_80092C24 = 0;
    D_80092C00 = 1 - D_80092C00;
    D_80092C04 = 1 - D_80092C04;
    D_80092C08 = 1 - D_80092C08;
    D_800BB150 = D_800B98A0[D_80092C00];
    D_800BB140 = D_800B98A8[D_80092C04];
    D_800BB148 = D_800B98B0[D_80092C08];
    D_800BB154 = D_800BB150;
    D_800BB144 = D_800BB140;
    D_800BB14C = D_800BB148;
    D_80092BF8 = 0;
    D_80092BF4 = func_8004FA4C();
}

void func_8004DABC(void) {
    if (D_80092BE8 != 0) {
        D_80092C30 = 0;
        D_80092C38 = 0;
        D_80092C0C = 1 - D_80092C0C;
        D_80092C10 = 1 - D_80092C10;
        D_80092C14 = 1 - D_80092C14;
        D_800BB184 = D_800BB158[D_80092C0C];
        D_80092C28 = 0;
        D_800BB17C = D_800BB160[D_80092C10];
        D_800BB174 = D_800BB168[D_80092C14];
        D_800BB188 = D_800BB184;
        D_800BB178 = D_800BB174;
        D_800BB180 = D_800BB17C;
    }
}

s32 func_8004DBAC(Object* arg0, s32 arg1, s32 arg2, s32 updateRate) {
    s32 sp2B4 = 0;
    s32 sp2B0;
    s32 sp2A4; // end of loop
    s32 pad2[1];
    s32 j;
    Vec3f sp244[8];
    s32 pad3[3];
    Vec3f sp22C; // obj -> positionMirror copy
    Vec3f sp220; // obj -> srt -> transl copy
    Vec3f sp1C0[8];
    u8 pad[0x110];
    ObjectStruct64* temp_s1;
    u32 temp_v0;
    Unk8004FA58 sp94;
    s32 sp90;
    Vec3s32 *sp8C;
    s32 sp88 = 0;
    s32 temp_t0;
    s32 i;
    AABBs32 sp68;
    f32 sp48[8] = D_80092C48.unk0;

    // @fake
    if (1) {}
    if (func_80041DA4() == 0) {
        arg0->unk64->gdl = NULL;
        return 0;
    }
    if (D_80092C24 >= 0x1C3) {
        arg0->unk64->gdl = NULL;
        return 0;
    }
    if (D_80092C2C >= 0x1A5) {
        arg0->unk64->gdl = NULL;
        return 0;
    }
    if (D_80092C34 >= 0x141) {
        arg0->unk64->gdl = NULL;
        return 0;
    }
    D_800BB140 = D_800BB144;
    D_800BB148 = D_800BB14C;
    D_800BB170 = 0;
    temp_s1 = arg0->unk64;
    if (temp_s1->flags & 0x1000) {
        temp_s1->unk36 -= updateRate * 2;
        if (temp_s1->unk36 <= 0) {
            temp_s1->unk36 = 0;
        }
        if (temp_s1->unk36 == 0) {
            temp_s1->gdl = NULL;
            return 0;
        }
    } else if (arg0->group == GROUP_UNK1) {
        if (arg2 == 0) {
            if (D_800BB170 != 0) {
                if (!(temp_s1->flags & 4)) {
                    temp_s1->unk36 += updateRate * 2;
                    if (temp_s1->unk36 >= 0x40) {
                        temp_s1->unk36 = 0x40;
                    }
                } else {
                    temp_s1->unk36 -= updateRate * 2;
                    if (temp_s1->unk36 <= 0) {
                        temp_s1->unk36 = 0;
                        temp_s1->flags &= ~4;
                    }
                }
            } else if (!(temp_s1->flags & 4)) {
                temp_s1->unk36 -= updateRate * 2;
                if (temp_s1->unk36 <= 0) {
                    temp_s1->unk36 = 0;
                    temp_s1->flags |= 4;
                }
            } else {
                temp_s1->unk36 += updateRate * 2;
                if (temp_s1->unk36 >= 0x40) {
                    temp_s1->unk36 = 0x40;
                }
            }
        }
    } else if (!(temp_s1->flags & 0x10000)) {
        temp_s1->unk36 += updateRate * 2;
        if (temp_s1->unk36 >= 0x40) {
            temp_s1->unk36 = 0x40;
        }
    }
    if (!(temp_s1->flags & 4)) {
        if (arg2 == 0) {
            arg0->unk64->gdl = NULL;
            return 1;
        }
        D_800BB170 = 1;
    } else {
        D_800BB170 = 0;
    }
    if (temp_s1->flags & 0x20) {
        bcopy(&arg0->srt.transl, &sp220, 0xC);
        bcopy(&arg0->positionMirror, &sp22C, 0xC);
        if (arg0->parent != NULL) {
            transform_point_by_object(
                temp_s1->tr.x,
                temp_s1->tr.y,
                temp_s1->tr.z,
                &arg0->positionMirror.x,
                &arg0->positionMirror.y,
                &arg0->positionMirror.z,
                arg0->parent
            );
        } else {
            bcopy(&temp_s1->tr, &arg0->positionMirror, 0xC);
        }
        bcopy(&temp_s1->tr, &arg0->srt.transl, 0xC);
    }
    temp_s1->gdl = D_800BB150;
    if (!(temp_s1->flags & 0x10)) {
        temp_s1->unk14.x = D_80092BD0;
        temp_s1->unk14.y = D_80092BD4;
        temp_s1->unk14.z = D_80092BD8;
    } else if (temp_s1->flags & 0x800) {
        temp_s1->unk14.x = 0.0f;
        temp_s1->unk14.z = 0.0f;
        temp_s1->unk14.y = 1.0f;
    }
    if (D_800BB170 != 0) {
        func_800516BC(arg0, sp244, temp_s1->unk0 * 0.7f);
    } else {
        func_80051944(0, arg0, sp244, temp_s1->unk0, 0);
    }
    for (i = 0, j = 0; i < 8; i++, j++) {
        sp1C0[j].x = sp244[i].x + arg0->positionMirror.x;
        sp1C0[j].y = sp244[i].y + arg0->positionMirror.y;
        sp1C0[j].z = sp244[i].z + arg0->positionMirror.z;
    }
    fit_aabb_around_cubes(&sp68, sp1C0, sp1C0, sp48, 8);
    func_80053750(arg0, &sp68, 1);
    func_80053408(&sp8C);
    func_800533D8(&sp2B4, &sp88);
    sp90 = sp88;
    sp2B4 = func_80052300(
        arg0,
        (UnkFunc80051D68Arg3 *) sp88,
        D_800BA1A0,
        (UnkFunc80052300Arg3 *) D_800BB140,
        sp2B4,
        // This is just wrong but matches?
        sp8C->x,
        sp8C->z,
        arg1,
        temp_s1->flags & 0x40000
    );
    D_80092BDC = sp90;
    D_80092C1C = sp2B4;
    D_80092BE0 = sp8C;
    func_800511E8(arg0, 0, sp244, &sp94);
    temp_t0 = ((s8 *)D_800BB148 - (s8 *)D_800B98B0[D_80092C08]) >> 4;
    if (D_800BB170 != 0) {
        if (func_800502AC(arg0, sp244, &sp94, sp2B4, (Vec3f *)D_800BB144, D_800BB148, D_800BA1A0, 0x18F - temp_t0) == 0) {
            temp_s1->gdl = NULL;
            D_80092C34 = 0x190;
            if (temp_s1->flags & 0x20) {
                bcopy(&sp220, &arg0->srt.transl, 0xC);
                bcopy(&sp22C, &arg0->positionMirror, 0xC);
            }
            return 0;
        }
        D_800BB150 += func_8004EEC0(D_800BB148, D_800BB150, temp_s1, arg0, D_80092C20, &sp2B0);
        goto block_59;
    } else if (func_8004FA58(arg0, sp244, &sp94, sp2B4, (Vec3f* ) D_800BB144, (Unk8004FA58_Arg5* ) D_800BB148, D_800BA1A0, 0x18F - temp_t0) == 0) {
        temp_s1->gdl = NULL;
        D_80092C34 = 0x190;
        if (temp_s1->flags & 0x20) {
            bcopy(&sp220, &arg0->srt.transl, 0xC);
            bcopy(&sp22C, &arg0->positionMirror, 0xC);
        }

        return 0;
    }

    D_800BB150 += func_8004F378(D_800BB148, D_800BB150, temp_s1, arg0, D_80092C20, &sp2B0);
block_59:
    D_800BB14C += sp2B0;
    D_800BB144 = D_800BB140;
    D_80092C24 = ((Gfx *) D_800BB150 - (Gfx *) D_800B98A0[D_80092C00]) ;
    D_80092C2C = ((s8 *) D_800BB140 - (s8 *) D_800B98A8[D_80092C04]) / 12;
    D_80092C34 = ((s8 *) D_800BB148 - (s8 *) D_800B98B0[D_80092C08]) >> 4;
    if (temp_s1->flags & 0x20) {
        bcopy(&sp220, &arg0->srt.transl, 0xC);
        bcopy(&sp22C, &arg0->positionMirror, 0xC);
    }
    return 0;
}


s32 func_8004E540(Object* arg0, ObjectStruct64* arg1) {
    s32 sp1C;
    s32 sp18;
    f32 var_fv1;

    sp1C = arg1->unk38 * 4;
    sp18 = arg1->unk39 * 4;
    var_fv1 = (func_80001884(arg0->positionMirror.x, arg0->positionMirror.y, arg0->positionMirror.z) - sp1C) / (sp18 - sp1C);
    if (var_fv1 < 0.0f) {
        var_fv1 = 0.0f;
    } else if (var_fv1 > 1.0f) {
        var_fv1 = 1.0f;
    }
    sp1C = (arg1->unk3A + ((arg1->unk3B - arg1->unk3A) * var_fv1));
    sp1C = (arg0->opacityWithFade + 1) * (s32)sp1C;
    return sp1C >> 8;
}

void func_8004E64C(Object* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    ObjectStruct64* temp_s0;
    Vec3f sp50;
    Vec3f sp44;

    temp_s0 = arg0->unk64;
    if (temp_s0->gdl == NULL) {
        return;
    }

    if (temp_s0->flags & 0x20) {
        bcopy(&arg0->srt.transl, &sp44, 0xC);
        bcopy(&arg0->positionMirror, &sp50, 0xC);
        bcopy(&temp_s0->tr, &arg0->srt.transl, 0xC);
        if (arg0->parent != NULL) {
            transform_point_by_object(temp_s0->tr.x, temp_s0->tr.y, temp_s0->tr.z, &temp_s0->tr.x, &temp_s0->tr.y, &temp_s0->tr.z, arg0->parent);
        } else {
            bcopy(&temp_s0->tr, &arg0->positionMirror, 0xC);
        }
    }
    if (temp_s0->flags & 8) {
        func_8005BCE0(arg0, D_80092C18, arg1, arg2, arg3, arg4, (s32) D_800BB170, D_800BB18C, temp_s0->flags & 3);
    }
    if (temp_s0->flags & 0x20) {
        bcopy(&sp44, &arg0->srt.transl, 0xC);
        bcopy(&sp50, &arg0->positionMirror, 0xC);
    }
}

void func_8004E7A8(Object* arg0) {
    s32 sp2B4 = 0;
    s32 sp2B0;
    s32 sp2AC;
    Vec3f sp24C[8];
    Vec3f sp1EC[8];
    Vec3f sp1E0;
    Vec3f sp1D4;
    u8 pad[0x128];
    Vec3s32 *spA0[3];
    s32 sp9C = 0;
    ObjectStruct64* temp_s1;
    s32 alpha;
    s32 i;
    s32 j;
    s32 temp_t0;
    AABBs32 sp70;
    f32 sp50[8] = D_80092C74.unk0;

    if (func_80041DA4() == 0) {
        arg0->unk64->gdl = NULL;
        return;
    }
    D_800BB170 = 0;
    temp_s1 = arg0->unk64;
    if (D_80092BE8 != 0) {
        temp_s1->gdl2 = NULL;
        D_80092BFC = 1;
        if (D_80092C28 >= 0x227) {
            arg0->unk64->gdl = NULL;
            return;
        }
        if (D_80092C30 >= 0x26D) {
            arg0->unk64->gdl = NULL;
            return;
        }
        if (D_80092C38 >= 0x209) {
            arg0->unk64->gdl = NULL;
            return;
        }
        D_800BB174 = D_800BB178;
        D_800BB17C = D_800BB180;
        if (temp_s1->flags & 0x20) {
            bcopy(&arg0->srt.transl, &sp1D4, 0xC);
            bcopy(&arg0->positionMirror, &sp1E0, 0xC);
            if (arg0->parent != NULL) {
                transform_point_by_object(
                    temp_s1->tr.x,
                    temp_s1->tr.y,
                    temp_s1->tr.z,
                    &arg0->positionMirror.x,
                    &arg0->positionMirror.y,
                    &arg0->positionMirror.z,
                    arg0->parent
                );
            } else {
                bcopy(&temp_s1->tr, &arg0->positionMirror, 0xC);
            }
            bcopy(&temp_s1->tr, &arg0->srt.transl, 0xC);
        }
        temp_s1->gdl = D_800BB184;
        if (!(temp_s1->flags & 0x10)) {
            temp_s1->unk14.x = D_80092BD0;
            temp_s1->unk14.y = D_80092BD4;
            temp_s1->unk14.z = D_80092BD8;
        } else if (temp_s1->flags & 0x800) {
            temp_s1->unk14.x = 0.0f;
            temp_s1->unk14.z = 0.0f;
            temp_s1->unk14.y = 1.0f;
        }
        func_80051944(0, arg0, sp24C, temp_s1->unk0 * 0.5f, 0);
        for (i = 0, j = 0; i < 8; i++, j++) {
            sp1EC[j].x = sp24C[i].x + arg0->positionMirror.x;
            sp1EC[j].y = sp24C[i].y + arg0->positionMirror.y;
            sp1EC[j].z = sp24C[i].z + arg0->positionMirror.z;
        }
        fit_aabb_around_cubes(&sp70, sp1EC, sp1EC, sp50, 8);
        func_80053750(arg0, &sp70, 1);
        func_80053408(spA0);
        func_800533D8(&sp2B4, &sp9C);
        sp2B4 = func_80052300(arg0, (UnkFunc80051D68Arg3 *)sp9C, D_800BA1A0, (UnkFunc80052300Arg3* ) D_800BB174, sp2B4, spA0[0]->x, spA0[0]->z, 0, temp_s1->flags & 0x40000);
        D_800BB174 = (Unk800BB168 *)((s8 *)D_800BB174 + sp2B4 * 0x24);
        func_800511E8(arg0, 0, sp24C, (Unk8004FA58* ) &spA0[2]);
        temp_t0 = ((s8 *)D_800BB17C - (s8*)D_800BB160[D_80092C10]) >> 4;
        if (func_8004FA58(arg0, sp24C, (Unk8004FA58* ) &spA0[2], sp2B4, (Vec3f* ) D_800BB178, (Unk8004FA58_Arg5* ) D_800BB17C, D_800BA1A0, 0x257 - temp_t0) == 0) {
            temp_s1->gdl = NULL;
            D_80092C38 = 0x258;
            if (temp_s1->flags & 0x20) {
                bcopy(&sp1D4, &arg0->srt.transl, 0xC);
                bcopy(&sp1E0, &arg0->positionMirror, 0xC);
            }
            return;
        } else {
            D_800BB184 += func_8004F378(D_800BB17C, D_800BB184, temp_s1, arg0, D_80092C20, &sp2B0);
            D_800BB180 += sp2B0;
            D_800BB178 = D_800BB174;
            D_80092C28 = (Gfx *) D_800BB184 - (Gfx *)D_800BB158[D_80092C0C];
            D_80092C30 = ((s8 *) D_800BB174 - (s8 *)D_800BB168[D_80092C14]) / 12;
            D_80092C38 = ((s8 *) D_800BB17C - (s8 *)D_800BB160[D_80092C10]) >> 4;
            D_80092BFC = 0;
            if (temp_s1->flags & 0x20) {
                bcopy(&sp1D4, &arg0->srt.transl, 0xC);
                bcopy(&sp1E0, &arg0->positionMirror, 0xC);
            }
        }
    }

    if ((temp_s1->gdl != NULL) && (temp_s1->gdl2 != NULL)) {
        if (!(temp_s1->flags & 0x400)) {
            alpha = func_8004E540(arg0, temp_s1);
        } else {
            alpha = temp_s1->unk40;
        }
        gDPSetPrimColor(temp_s1->gdl2, 0, 0, 0, 0, 0, alpha);
    }
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004EEC0.s")
#else
s32 func_8004EEC0(Unk8004FA58_Arg5 *arg0, Gfx* gdl, ObjectStruct64* arg2, Object *arg3, s32 arg4, s32* arg5) {
    DLTri* currentTri;
    s16 temp_v0_2;
    s16* var_s0;
    s32 temp_v0;
    s32 var_a2;
    s32 var_a3;
    s32 var_s1;
    s32 var_s2;
    s32 var_s5;
    Gfx* sp1C0;
    s32 var_s6;
    s32 sp1B8;
    s32 sp1B4;
    s32 var_v1;
    s32 var_a0;
    DLTri spA8[16];

    sp1C0 = gdl;
    // macro correct, invalid params
    gSPLoadGeometryMode(gdl, G_SHADE | G_ZBUFFER | G_SHADING_SMOOTH |0x400);
    // gdl->words.w0 = 0xD9000000;
    // gdl->words.w1 = 0x10405;
    dl_apply_geometry_mode(&gdl);
    // macro correct, invalid params
    gDPSetCombineLERP(gdl,
        0, 0, 0, ENVIRONMENT,
        0, 0, 0, ENVIRONMENT,
        0, 0, 0, ENVIRONMENT,
        0, 0, 0, ENVIRONMENT);
    dl_apply_combine(&gdl);
    if (arg2->unk30 & 0x80) {
        // TODO: Find/use correct flags
        gDPSetOtherMode(gdl, 0xEF182C00, 0xC8104340);
        dl_apply_other_mode(&gdl);
    } else {
        // TODO: Find/use correct flags
        gDPSetOtherMode(gdl, 0xEF182C00, 0xC8104B50);
        dl_apply_other_mode(&gdl);
    }
    arg2->gdl = gdl;
    dl_set_prim_color_no_sync(&gdl, 0xFFU, 0xFFU, 0xFFU, 0x96U);
    func_8005C740(arg2->unk41 ^ 1, &sp1B8, &sp1B4, arg2->unk30 & 3);
    var_s5 = 0;
    for (var_s6 = 0; var_s6 < 5; var_s6++) {
        var_s2 = arg0;
        if (var_s6 == 0) {
            var_a2 = 0;
            var_a3 = 0xF;
        } else {
            if (var_s6 == 3) {
                var_a2 = 0x2F;
                var_a3 = 0x3F;
            } else {
                temp_v0 = var_s6 * 0x10;
                var_a2 = temp_v0 - 1;
                var_a3 = temp_v0 + 0x10;
            }
        }
        {
            // gDPSetTextureImage
            Gfx *_g = gdl++;\
            _g->words.w0 = 0xFD10003F;\
            _g->words.w1 = sp1B8 + 0x80000000;
        }
        {
            // gDPSetTile(gdl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0);
            Gfx *_g = gdl++;\
            _g->words.w0 = 0xF5102000;\
            _g->words.w1 = 0x07080200;
        }

        gDPLoadSync(gdl++);

        {
            // gDPLoadTile(gdl++, 0x070F, 1, var_a2 * 4, 12, var_a3 * 4);
            Gfx *_g = gdl++;\
            _g->words.w0 = ((var_a2 * 4) & 0xFFF) | 0xF4000000;\
            _g->words.w1 = ((var_a3 * 4) & 0xFFF) | 0x070FC000;
        }

        gDPPipeSync(gdl++);
        gDPSetTile(gdl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 0, 0, 0, 2, 0, 0, 2, 0, 0);
        {
            // gDPSetTileSize(gdl++, 100, var_a2 * 4, 12, var_a3 * 4, 0);
            Gfx *_g = gdl++;\
            _g->words.w0 = ((var_a2 * 4) & 0xFFF) | 0xF2000000;\
            _g->words.w1 = ((var_a3 * 4) & 0xFFF) | 0xFC000;
        }
        if (!(var_s6 & 1)) {
            {
                // gDPSetTextureImage
                Gfx *_g = gdl++;\
                _g->words.w0 = 0xFD88001F;\
                _g->words.w1 = sp1B4 + 0x80000000;
            }
            {
                // gDPSetTile
                Gfx *_g = gdl++;\
                _g->words.w0 = 0xF5880940;\
                _g->words.w1 = 0x07080200;
            }
            gDPLoadSync(gdl++);
            {
                // gDPLoadTile
                Gfx *_g = gdl++;\
                _g->words.w0 = ((var_a2 * 4) & 0xFFF) | 0xF4000000;\
                _g->words.w1 = (((var_a3 + 0x10) * 4) & 0xFFF) | 0x0707E000;
            }
            gDPPipeSync(gdl++);
            {
                // gDPSetTile
                Gfx *_g = gdl++;\
                _g->words.w0 = 0xF5800940;\
                _g->words.w1 = 0x01080200;
            }
            {
                // gDPSetTileSize
                Gfx *_g = gdl++;\
                _g->words.w0 = ((var_a2 * 4) & 0xFFF) | 0xF2000000;\
                _g->words.w1 = (((var_a3 + 0x10) * 4) & 0xFFF) | 0x010FC000;
            }
        }
        var_s0 = &D_800B98B8;
        for (var_s1 = 0; var_s1 < arg4; var_s1++) {
            var_v1 = 0;
            if ((((s32) *(&D_800B9B10 + (var_s1 >> 2)) >> ((var_s1 & 3) * 2)) & 3) == var_s6) {
                var_v1 = 1;
            }
            if (var_v1 != 0) {
                // maybe correct?
                gSPVertex(gdl++, OS_K0_TO_PHYSICAL(var_s2), *var_s0, 0)
                // {
                //     Gfx *_g = gdl++;\
                //     _g->words.w0 = ((*var_s0 & 0x7F) * 2) | 0x01000000 | ((*var_s0 & 0xFF) << 0xC);\
                //     _g->words.w1 = var_s2 + 0x80000000;
                // }

                for (var_a0 = 0; var_a0 < (*var_s0 - 2); var_a0++) {
                    currentTri = &spA8[var_a0];
                    currentTri->unk0 = 0;
                    currentTri->v0 = 0;
                    currentTri->v1 = var_a0 + 1;
                    currentTri->v2 = var_a0 + 2;
                }
                dl_triangles(&gdl, spA8, (*var_s0 - 2));
                var_s5 += *var_s0;
            }
            temp_v0_2 = *var_s0;
            var_s0 += 1;
            var_s2 += temp_v0_2 * 0x10;
        }
    }
    *arg5 = var_s5;
    return (gdl - sp1C0) >> 3;
}
#endif

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004F378.s")
#else
s32 func_8004F378(Unk8004FA58_Arg5* arg0, Gfx* arg1, ObjectStruct64* arg2, Object* arg3, s32 arg4, s32* arg5) {
    s32 sp1E4;
    s32 var_s0;
    s32 var_s4;
    s32 var_a0;
    s32 sp1D4;
    s32 sp1D0;
    s16 temp_ft3;
    Gfx* sp1C8;
    Texture* sp1C4;
    f32 sp1C0;
    u8 pad[0xf0];
    DLTri* var_v1;
    Unk8004FA58_Arg5* var_s3;
    f32 var_ft5;
    s16 temp_a1;
    DLTri *spB8[2];
    s32 spB4;
    s32 spB0;

    var_s3 = arg0;
    sp1C4 = NULL;
    sp1C8 = arg1;
    var_s4 = 0;
    sp1C0 = (f32) arg2->unk36 * 0.015625f;
    if (arg2->flags & 0x40) {
        sp1C0 = arg2->unk3F;
    } else if ((arg3->group == GROUP_UNK1) && (D_80092C3C > 0.0f)) {
        sp1C0 *= ((1.0f - D_80092BE4) * D_80092C3C) + D_80092BE4;
    } else {
        sp1C0 *= D_80092BE4;
    }
    if (arg2->flags & 8) {
        func_8005C740(arg2->unk41 ^ 1, &spB4, &spB0, arg2->flags & 3);
        sp1D4 = 0;
    } else {
        sp1D4 = 1;
        sp1C4 = arg2->unk4;
    }
    if (arg2->flags & 0x40) {
        if ((arg3->def->shadowType == 2) && !(arg2->flags & 0x400)) {
            var_s0 = func_8004E540( arg3, arg2);
        } else {
            var_s0 = (s32) arg2->unk40;
        }
        sp1D0 = (s16) ((((arg3->opacityWithFade + 1) * var_s0) >> 8) * sp1C0);
        if (sp1D0 >= 0x100) {
            sp1D0 = 0xFF;
        } else if (sp1D0 < 0) {
            sp1D0 = 0;
        }
    }
    sp1E4 = 0;
    func_80040FF8();
    func_8003DB7C();
    gSPGeometryMode(arg1, 0xFFFFFF, G_FOG| G_CULL_BACK | G_SHADE | G_ZBUFFER);
    dl_apply_geometry_mode(&arg1);
    if (sp1D4 != 0) {
        gDPSetCombineLERP(arg1, 0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0, 0, 0, 0, COMBINED, COMBINED, 0, PRIMITIVE, 0)
        dl_apply_combine(&arg1);
        if (arg2->flags & 0x80) {
            gDPSetOtherMode(
                arg1,
                G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_CLD_SURF2
            )
            dl_apply_other_mode(&arg1);
        } else {
            gDPSetOtherMode(
                arg1,
                G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_ZB_CLD_SURF2
            );
            dl_apply_other_mode(&arg1);
        }
        set_textures_on_gdl(&arg1, sp1C4, NULL, 0U, 0, 0U, 0U);
    } else {
        gDPSetCombineLERP(arg1, 0, 0, 0, PRIMITIVE, TEXEL0, 0, TEXEL1, 0, 0, 0, 0, COMBINED, COMBINED, 0, PRIMITIVE, 0)
        dl_apply_combine(&arg1);
        if (arg2->flags & 0x80) {
            gDPSetOtherMode(
                arg1,
                G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_CLD_SURF2
            )
            dl_apply_other_mode(&arg1);
        } else {
            gDPSetOtherMode(
                arg1,
                G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_ZB_CLD_SURF2
            )
            dl_apply_other_mode(&arg1);
        }
        // This should be
        // gDPLoadTextureBlock(arg1++, OS_PHYSICAL_TO_K0(spB4), G_IM_FMT_I, G_IM_SIZ_4b, 64, 64, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD)
        // But the height/width isn't lining up for gDPLoadBlock
        gDPSetTextureImage(arg1++, G_IM_FMT_I, G_IM_SIZ_16b, 1, OS_PHYSICAL_TO_K0(spB4))
        gDPSetTile(arg1++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD)
        gDPLoadSync(arg1++)
        gDPLoadBlock(arg1++, G_TX_LOADTILE, 0, 0, 1023, 512)
        gDPPipeSync(arg1++)
        gDPSetTile(arg1++, G_IM_FMT_I, G_IM_SIZ_4b, 4, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD)
        gDPSetTileSize(arg1++, G_TX_RENDERTILE, 0, 0, 252, 252)
        // This should also probably be gDPLoadTextureBlock
        gDPSetTextureImage(arg1++, G_IM_FMT_I, G_IM_SIZ_16b, 1, (void *)(((u32)(D_800BB190)+0x80000020)))
        gDPSetTile(arg1++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0x0100, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD)
        gDPLoadSync(arg1++);
        gDPLoadBlock(arg1++, G_TX_LOADTILE, 0, 0, 1023, 0)
        gDPPipeSync(arg1++);
        gDPSetTile(arg1++, G_IM_FMT_I, G_IM_SIZ_4b, 4, 0x0100, 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD)
        gDPSetTileSize(arg1++, 1, 0, 0, 252, 252)
    }
    arg2->gdl2 = arg1;
    if (arg2->flags & 0x40) {
        dl_set_prim_color_no_sync(&arg1, arg2->unk3C, arg2->unk3D, arg2->unk3E, (s16)(sp1C0 * 255.0f));
    } else {
        dl_set_prim_color_no_sync(&arg1, 0, 0, 0, sp1D0);
    }
    for (; sp1E4 < arg4; sp1E4++) {
        temp_a1 = D_800B98B8[sp1E4];
        {
            Gfx *_g = (Gfx *) (arg1++);
            _g->words.w0 = ((temp_a1 & 0x7F) * 2) | 0x01000000 | ((temp_a1 & 0xFF) << 0xC);\
            _g->words.w1 = OS_PHYSICAL_TO_K0(var_s3);
        }
        var_v1 = spB8;
        for (var_a0 = 0; var_a0 < D_800B98B8[sp1E4] - 2; ) {
            var_v1->v0 = 0;
            var_v1->v1 = var_a0 + 1;
            var_v1->v2 = var_a0 + 2;
            var_v1++;
            var_a0++;
        }
        dl_triangles(&arg1, spB8, D_800B98B8[sp1E4] - 2);

        var_s3 = (s8 *)var_s3 +  D_800B98B8[sp1E4] * 0x10;
        var_s4 += D_800B98B8[sp1E4];
    }
    gSPEndDisplayList(arg1++);
    func_80041028();
    func_8003DBCC();
    *arg5 = var_s4;
    return arg1 - sp1C8;
}
#endif

s32 func_8004FA3C(s32 arg0) {
    return 0;
}

s32 func_8004FA4C(void) {
    return 0;
}

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_8004FA58.s")
#else
// https://decomp.me/scratch/1Xf9d
s32 func_8004FA58(Object* arg0, Vec3f *arg1, Unk8004FA58 *arg2, s32 arg3, Vec3f *arg4, Unk8004FA58_Arg5 *arg5, Unk8004FA58* arg6, s32 max) {
    s32 sp28C;
    Camera* camera;
    Vec3f* var_v0;
    s32 sp280;
    f32 temp_fs0_2;
    f32 temp_fv0;
    f32 var_fa1;
    f32 var_fs2;
    s32 sp26C;
    u8 pad[0x20];
    Vec3f sp1A4[14]; // unknown size, should be equal
    f32 var_fs3;
    f32 var_fv0;
    s16 var_a0;
    s32 var_s2;
    s32 var_s7;
    s32 var_a3;
    s32 var_s4;
    Vec3f spE4[14]; // unknown size, should be equal
    f32 spE0;
    f32 spDC;
    f32 spD8;
    s16 spD6;
    s16 spD4;
    s16 spD2;
    Unk8004FA58_Arg5* temp_v0_3;
    Vec3f* temp_v1_3;
    Unk8004FA58_Arg5* var_s0;
    Unk8004FA58* var_s3;
    s32 var_v0_2;
    ObjectStruct64* spAC;
    s32 spA0;
    s32 sp90;

    sp280 = 0;
    sp26C = 0;
    spAC = arg0->unk64;
    camera = get_camera();
    if (arg0->parent != NULL) {
        inverse_transform_point_by_object(camera->tx, camera->ty, camera->tz, &spD8, &spDC, &spE0, arg0->parent);
    } else {
        spD8 = camera->tx;
        spDC = camera->ty;
        spE0 = camera->tz;
    }
    spD8 -= arg0->srt.transl.x;
    spDC -= arg0->srt.transl.y;
    spE0 -= arg0->srt.transl.z;
    temp_fv0 = sqrtf((spD8 * spD8) + (spDC * spDC) + (spE0 * spE0));
    var_fs2 = temp_fv0 * 0.1f;
    var_a0 = arctan2_f(spDC, sqrtf((spD8 * spD8) + (spE0 * spE0)));
    CLAMP(var_a0, 0, 0x2000)
    var_a3 = 0;
    var_fv0 = (f32)var_a0 / 8192;
    if (var_fv0 > 1.0f) {
        var_fv0 = 1.0f;
    }
    var_fv0 = 1.0f - var_fv0;
    var_fs3 = var_fv0 * var_fv0;
    if (spAC->flags & 0x80) {
        var_fs3 = 0.0f;
        var_fs2 = 0.0f;
    }
    if (temp_fv0 != 0.0f) {
        temp_fv0 = var_fs2 / temp_fv0;
        spD8 *= temp_fv0;
        spDC *= temp_fv0;
        spE0 *= temp_fv0;
    }
    if (spAC->flags & 0x40000) {
        var_fs3 = 0.0f;
        spDC = 0.0f;
        spD8 = 0.0f;
        spE0 = 0.0f;
    }
    D_80092C20 = 0;
    for (sp28C = 0; sp28C < arg3; sp28C++) {
        for (var_s4 = 0; var_s4 < 3; var_s4++) {
            var_v0 = &arg4[sp280 + var_s4];
            sp1A4[var_s4].x = var_v0->x;
            sp1A4[var_s4].y = var_v0->y;
            sp1A4[var_s4].z = var_v0->z;
        }
        if (arg6[sp28C].unk10 & 1) {
            if (func_80051CFC(&spAC->unk14, (Vec3f *)&arg6[sp28C].pos) == 1) {
                var_s7 = 3;
                for (sp90 = 0; sp90 < 6 && var_s7 != 0; sp90++) {
                    var_s3 = &arg2[sp90];
                    var_s2 = 0;
                    var_v0_2 = var_s7 - 1;
                    temp_v1_3 = &sp1A4[var_v0_2];
                    var_fa1 = (temp_v1_3->z * var_s3->pos.z) + ((temp_v1_3->x * var_s3->pos.x) + (temp_v1_3->y * var_s3->pos.y)) + var_s3->pos.w;
                    for (var_s4 = 0; var_s4 < var_s7; var_s4++) {
                        temp_fs0_2 = (sp1A4[var_s4].x * var_s3->pos.x) + (sp1A4[var_s4].y * var_s3->pos.y) + (sp1A4[var_s4].z * var_s3->pos.z) + var_s3->pos.w;
                        if (temp_fs0_2 <= 0.0f) {
                            if (var_fa1 <= 0.0f) {
                                spE4[var_s2].x = sp1A4[var_s4].x;
                                spE4[var_s2].y = sp1A4[var_s4].y;
                                spE4[var_s2].z = sp1A4[var_s4].z;
                                var_s2++;
                            } else {
                                func_800528AC(&sp1A4[var_v0_2], &sp1A4[var_s4], &spE4[var_s2++], &var_s3->pos);
                                spE4[var_s2].x = sp1A4[var_s4].x;
                                spE4[var_s2].y = sp1A4[var_s4].y;
                                spE4[var_s2].z = sp1A4[var_s4].z;
                                var_s2++;
                            }
                        } else if (var_fa1 <= 0.0f) {
                            func_800528AC(&sp1A4[var_v0_2], &sp1A4[var_s4], &spE4[var_s2++], &var_s3->pos);
                        }
                        var_fa1 = temp_fs0_2;
                        var_v0_2 = var_s4;
                    }
                    var_s7 = var_s2;
                    for (var_s4 = 0; var_s4 < var_s2; var_s4++) {\
                        sp1A4[var_s4].x = spE4[var_s4].x;\
                        sp1A4[var_s4].y = spE4[var_s4].y;\
                        sp1A4[var_s4].z = spE4[var_s4].z;
                    }
                }
                if (var_s7 != 0) {
                    D_800B98B8[D_80092C20] = var_s7;
                    D_80092C20++;
                }
                if (var_s7 > 0) {
                    var_s0 = &arg5[sp26C];
                    for (var_s4 = 0; var_s4 != var_s7; var_s4++) {
                        if (arg0->def->shadowType == 2) {
                            func_80050B88(arg0, &sp1A4[var_s4], &arg2[5], arg2, 0, &arg2[1], &spD4, &spD6, spAC->unk0, &spD2, 0);
                        } else {
                            func_80050B88(arg0, &sp1A4[var_s4], &arg2[5], &arg2[2], 0, &arg2[3], &spD4, &spD6, spAC->unk0, &spD2, 1);
                        }
                        if (spAC->flags & 0x8000) {
                            spD2 = 0xFF;
                        }
                        D_800B9B60[sp26C * 2 + 0] = spD6;
                        D_800B9B60[sp26C * 2 + 1] = spD4;
                        if (arg0->def->shadowType == 2) {
                            var_s0->x = (sp1A4[var_s4].x + spD8) * 20.0f;
                            var_s0->y = (sp1A4[var_s4].y + spDC + var_fs3) * 20.0f;
                            var_s0->z = (sp1A4[var_s4].z + spE0) * 20.0f;
                        } else {
                            var_s0->x = sp1A4[var_s4].x * 20.0f;
                            var_s0->y = sp1A4[var_s4].y * 20.0f;
                            if (!(spAC->flags & 0x80)) {
                                var_s0->y += 0x1E;
                            }
                            var_s0->z = sp1A4[var_s4].z * 20.0f;
                        }
                        temp_v0_3 = &arg5[sp26C];
                        temp_v0_3->unk8 = spD6;
                        temp_v0_3->unkA = spD4;
                        var_s0++;
                        sp26C++;
                        if (sp26C >= max) {
                            return 0;
                        }
                    }
                }
            }
        }
        sp280 += 3;
    }
    D_80092CA0 = D_80092CA0 == 1;
    return 1;
}
#endif

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_800502AC.s")
#else
// https://decomp.me/scratch/W9sJo
s32 func_800502AC(Object* arg0, Vec3f *arg1, Unk8004FA58* arg2, s32 arg3, Vec3f *arg4, Unk8004FA58_Arg5 *arg5, Unk8004FA58* arg6, s32 arg7) {
    s32 sp354;
    s32 var_s3;
    s32 sp34C;
    Vec3f *temp_v1;
    s32 sp344;
    Unk8004FA58* var_s2;
    s32 sp33C;
    s32 sp338;
    Camera* camera;
    s32 sp330;
    f32 temp_fs0;
    f32 temp_fs4;
    Vec3f sp268[16]; // size TBD
    Vec3f sp1A8[16]; // size TBD
    Vec3f spE8[16]; // size TBD
    Vec3f spDC;
    Vec3f spD0;
    s16 spCE;
    s16 spCC;
    s16 spCA;
    f32 var_fa1;
    f32 var_fv1;
    s32 var_s4;
    s32 var_s5;
    s32 var_v0_2;
    Unk8004FA58_Arg5* var_s0;
    ObjectStruct64* spAC;

    sp344 = 0;
    spAC = arg0->unk64;
    bzero(&D_800B9B10, 0x4C);
    temp_fs4 = (f32) spAC->unk36 * 0.015625f;
    camera = get_camera();
    spD0.x = camera->tx - arg0->positionMirror.x;
    spD0.y = camera->ty - arg0->positionMirror.y;
    spD0.z = camera->tz - arg0->positionMirror.z;
    var_fv1 = sqrtf((spD0.x * spD0.x) + (spD0.y * spD0.y) + (spD0.z * spD0.z));
    if (spAC->flags & 0x80) {
        var_fv1 = 0.0f;
    }
    if (var_fv1 != 0.0f) {
        var_fv1 = 1.0f / var_fv1;
        spD0.x *= var_fv1;
        spD0.y *= var_fv1;
        spD0.z *= var_fv1;
    }
    D_80092C20 = 0;
    sp330 = 0;
    for (sp354 = 0; sp354 < arg3; sp344 += 3, sp354++) {
        for (var_s4 = 0; var_s4 < 3; var_s4++) {
            temp_v1 = &arg4[sp344 + var_s4];
            sp268[var_s4].x = arg4[sp344 + var_s4].x;
            sp268[var_s4].y = arg4[sp344 + var_s4].y;
            sp268[var_s4].z = arg4[sp344 + var_s4].z;
        }

        spDC.x = arg6[sp354].pos.x;
        spDC.y = arg6[sp354].pos.y;
        spDC.z = arg6[sp354].pos.z;
        if (func_80052148((Vec3f* ) &spD0, (Vec3f* ) &spDC) != 1) {
            continue;
        }

        var_s3 = 0;
        sp33C = 3;
        while (var_s3 < 6 && sp33C != 0) {
            var_s5 = 0;
            var_s2 = &arg2[var_s3];
            var_v0_2 = sp33C - 1;
            var_fa1 = (sp268[var_v0_2].x * var_s2->pos.x) + (sp268[var_v0_2].y * var_s2->pos.y) + (sp268[var_v0_2].z * var_s2->pos.z) + var_s2->pos.w;
            for (var_s4 = 0; var_s4 < sp33C; ) {
                temp_fs0 = (sp268[var_s4].x * var_s2->pos.x) + (sp268[var_s4].y * var_s2->pos.y) + (sp268[var_s4].z * var_s2->pos.z) + var_s2->pos.w;
                if (temp_fs0 <= 0.0f) {
                    if (var_fa1 <= 0.0f) {
                        sp1A8[var_s5].x = sp268[var_s4].x;
                        sp1A8[var_s5].y = sp268[var_s4].y;
                        sp1A8[var_s5].z = sp268[var_s4].z;
                        var_s5++;
                    } else {
                        func_800528AC(&sp268[var_v0_2], &sp268[var_s4], &sp1A8[var_s5++], &var_s2->pos);
                        sp1A8[var_s5].x = sp268[var_s4].x;
                        sp1A8[var_s5].y = sp268[var_s4].y;
                        sp1A8[var_s5].z = sp268[var_s4].z;
                        var_s5++;
                    }
                } else if (var_fa1 <= 0.0f) {
                    func_800528AC(&sp268[var_v0_2], &sp268[var_s4], &sp1A8[var_s5++], &var_s2->pos);
                }
                var_v0_2 = var_s4;
                var_fa1 = temp_fs0;
                var_s4++;
            }
            sp33C = var_s5;
            for (var_s4 = 0; var_s4 < var_s5; var_s4++) {\
                sp268[var_s4].x = sp1A8[var_s4].x;\
                sp268[var_s4].y = sp1A8[var_s4].y;\
                sp268[var_s4].z = sp1A8[var_s4].z;
            }
            var_s3++;
        }

        if (sp33C != 0) {
            sp34C = 0;
        } else {
            sp34C = 4;
        }

        while (sp34C < 4) {
            if (sp34C == 0) {
                func_80052644(sp1A8, spE8, sp33C, &sp338, &arg2[11].pos, 0xC, NULL, 0U);
            } else if (sp34C == 1) {
                func_80052644(sp1A8, sp268, sp33C, &sp33C, &arg2[10].pos, 0xC, NULL, 0U);
                func_80052644(sp268, spE8, sp33C, &sp338, &arg2[9].pos, 0xC, NULL, 0U);
            } else if (sp34C == 2) {
                func_80052644(sp1A8, sp268, sp33C, &sp33C, &arg2[8].pos, 0xC, NULL, 0U);
                func_80052644(sp268, spE8, sp33C, &sp338, &arg2[7].pos, 0xC, NULL, 0U);
            } else {
                func_80052644(sp1A8, spE8, sp33C, &sp338, &arg2[6].pos, 0xC, NULL, 0U);
            }
            if (sp338 != 0) {
                D_800B98B8[D_80092C20] = sp338;
                D_800B9B10[D_80092C20 >> 2] |= sp34C << ((D_80092C20 & 3) * 2);
                D_80092C20++;
            }
            for (var_s4 = 0; var_s4 < sp338; var_s4++) {
                var_s0 = &arg5[sp330];

                func_80050B88(arg0, &spE8[var_s4], &arg2[4], &arg2[2], &arg2[0], &arg2[1], &spCC, &spCE, spAC->unk0 * 0.75f, &spCA, 0);
                if (spAC->flags & 0x8000) {
                    spCA = 0xFF;
                }

                D_800B9B60[sp330 * 2 + 0] = spCE;
                D_800B9B60[sp330 * 2 + 1] = spCC;

                var_s0++;
                var_s0[-1].x = spE8[var_s4].x * 20.0f;
                var_s0[-1].y = spE8[var_s4].y * 20.0f;
                var_s0[-1].z = spE8[var_s4].z * 20.0f;
                var_s0[-1].unkF = spCA * D_80092BE4 * temp_fs4;
                var_s0[-1].unk8 = spCE;
                arg5[sp330].unkA = spCC;
                sp330++;
                if (sp330 >= arg7) {
                    return 0;
                }
            }
            sp34C++;
        }
    }
    D_80092CA4 ^= 1;
    return 1;
}

#endif

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80050B88.s")
#else
// https://decomp.me/scratch/IurnK
void func_80050B88(Object* arg0, Vec3f* arg1, Unk8004FA58* arg2, Unk8004FA58* arg3, Unk8004FA58* arg4, Unk8004FA58* arg5, s16* arg6, s16* arg7, f32 arg8, s16* arg9, s32 argA) {
    s32 pad;
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_ft3;
    f32 temp_ft4;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_ft4;
    f32 f0;
    f32 f2;
    f32 sp2C;

    sp2C = arg0->unk64->unk2c;
    if (D_800BB170 != 0) {
        f0 = (arg1->x * arg2->pos.x) + (arg1->z * arg2->pos.z) + arg2->pos.w;
        f0 = -f0;
        f0 /= (55.0f * (sp2C * 0.3333f));
        f0 *= 63.0f;
        if (f0 < 0.0f) {
            f0 = 0.0f;
        }
        if (f0 > 63.0f) {
            f0 = 63.0f;
        }
        arg6[0] = (s32) f0 << 5;
        arg6[0] += (f0 - (s32) f0) * 32.0f;
        f0 = (63.0f - (s32) f0);
        f0 *= 16.0f;
        arg9[0] = f0;
        if (arg9[0] >= 0x100) {
            arg9[0] = 0xFF;
        }
        f2 = (arg4->pos.x * -arg2->pos.z) + (arg4->pos.z * arg2->pos.x);
        if (f2 != 0.0f) {
            f2 = -((arg4->pos.x * arg1->x) + (arg4->pos.z * arg1->z) + arg4->pos.w) / f2;
        }
        f0 = (arg3->pos.x * -arg2->pos.z) + (arg3->pos.z * arg2->pos.x);
        if (f0 != 0.0f) {
            f0 = -((arg3->pos.x * arg1->x) + (arg3->pos.z * arg1->z) + arg3->pos.w) / f0;
        }
        f0 = -f0 / (f2 - f0);
        f0 *= 63.0f;
        if (f0 < 0.0f) {
            f0 = 0.0f;
        }
        if (f0 > 63.0f) {
            f0 = 63.0f;
        }
        arg7[0] = ((s32) f0 + 1) << 5;
        arg7[0] += (f0 - (s32) f0) * 32.0f;
        return;
    }
    temp_fv0 = -arg2->pos.w - ((arg2->pos.x * arg1->x) + (arg2->pos.y * arg1->y) + (arg2->pos.z * arg1->z));
    temp_fv1 = arg2->pos.x * temp_fv0;
    temp_fa0 = arg2->pos.y * temp_fv0;
    temp_fa1 = arg2->pos.z * temp_fv0;
    temp_ft3 = (temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0) + (temp_fa1 * temp_fa1);
    if (temp_ft3 != 0.0f) {
        temp_ft3 = (sqrtf(temp_ft3) / (100.0f * (sp2C / 3.0f))) * 255.0f;
    }
    if (arg0->unk64->flags & 0x40) {
        arg9[0] = 0xFF;
    } else {
        arg9[0] = 255.0f - (s16) temp_ft3;
    }
    if (arg9[0] < 0) {
        arg9[0] = 0;
    } else if (arg9[0] > 255.0f) {
        arg9[0] = 0xFF;
    }
    temp_fv0 = -arg5->pos.w - ((arg5->pos.x * arg1->x) + (arg5->pos.y * arg1->y) + (arg5->pos.z * arg1->z));
    temp_fv1 = arg5->pos.x * temp_fv0;
    temp_fa0 = arg5->pos.y * temp_fv0;
    temp_fa1 = arg5->pos.z * temp_fv0;
    temp_ft3 = (temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0) + (temp_fa1 * temp_fa1);
    if (temp_ft3 != 0.0f) {
        temp_fv0 = sqrtf(temp_ft3);
        if (argA == 0) {
            var_ft4 = 2.0f * temp_fv0;
        } else {
            var_ft4 = temp_fv0 * 4.0f;
        }
        var_ft4 = var_ft4 / arg8;
        arg6[0] = (s32)var_ft4 << 6;
        arg6[0] += (var_ft4 - (s32)var_ft4) * 32.0f;
    } else {
        arg6[0] = 0;
    }
    temp_fv0 = -arg3->pos.w - ((arg3->pos.x * arg1->x) + (arg3->pos.y * arg1->y) + (arg3->pos.z * arg1->z));
    temp_fv1 = arg3->pos.x * temp_fv0;
    temp_fa0 = arg3->pos.y * temp_fv0;
    temp_fa1 = arg3->pos.z * temp_fv0;
    temp_ft3 = (temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0) + (temp_fa1 * temp_fa1);
    if (temp_ft3 != 0.0f) {
        temp_fv0 = sqrtf(temp_ft3);
        if (argA == 0) {
            var_ft4 = 2.0f * temp_fv0;
        } else {
            var_ft4 = temp_fv0 * 4.0f;
        }
        var_ft4 = var_ft4 / arg8;
        arg7[0] = (s32) var_ft4 << 6;
        arg7[0] += (var_ft4 - (s32) var_ft4) * 32.0f;
        return;
    }
    arg7[0] = 0;
}
#endif

void func_800511E8(Object *arg0, s32 arg1, Vec3f* arg2, Unk8004FA58* arg3) {
    Vec3f normalizedPos;
    s32 i;
    f32 wSum;

    if (arg1 != 0) {
        for (i = 0; i < 8; i++) { arg2[i].y += arg1; }
    }

    func_80051C54(&arg2[3], &arg2[7], &arg2[2], &normalizedPos);
    guNormalize(&normalizedPos.x, &normalizedPos.y, &normalizedPos.z);
    arg3[0].pos.x = -normalizedPos.x;
    arg3[0].pos.y = -normalizedPos.y;
    arg3[0].pos.z = -normalizedPos.z;
    arg3[0].pos.w = -(((arg3[0].pos.x * arg2[3].x) + (arg3[0].pos.y * arg2[3].y)) + (arg3[0].pos.z * arg2[3].z));

    func_80051C54(&arg2[5], &arg2[1], &arg2[6], &normalizedPos);
    guNormalize(&normalizedPos.x, &normalizedPos.y, &normalizedPos.z);
    arg3[1].pos.x = -normalizedPos.x;
    arg3[1].pos.y = -normalizedPos.y;
    arg3[1].pos.z = -normalizedPos.z;
    arg3[1].pos.w = -(((arg3[1].pos.x * arg2[5].x) + (arg3[1].pos.y * arg2[5].y)) + (arg3[1].pos.z * arg2[5].z));

    func_80051C54(&arg2[4], &arg2[0], &arg2[5], &normalizedPos);
    guNormalize(&normalizedPos.x, &normalizedPos.y, &normalizedPos.z);
    arg3[2].pos.x = -normalizedPos.x;
    arg3[2].pos.y = -normalizedPos.y;
    arg3[2].pos.z = -normalizedPos.z;
    arg3[2].pos.w = -(((arg3[2].pos.x * arg2[4].x) + (arg3[2].pos.y * arg2[4].y)) + (arg3[2].pos.z * arg2[4].z));

    func_80051C54(&arg2[0], &arg2[4], &arg2[3], &normalizedPos);
    guNormalize(&normalizedPos.x, &normalizedPos.y, &normalizedPos.z);
    arg3[3].pos.x = -normalizedPos.x;
    arg3[3].pos.y = -normalizedPos.y;
    arg3[3].pos.z = -normalizedPos.z;
    arg3[3].pos.w = -(((arg3[3].pos.x * arg2[0].x) + (arg3[3].pos.y * arg2[0].y)) + (arg3[3].pos.z * arg2[0].z));

    func_80051C54(&arg2[7], &arg2[4], &arg2[6], &normalizedPos);
    guNormalize(&normalizedPos.x, &normalizedPos.y, &normalizedPos.z);
    arg3[4].pos.x = -normalizedPos.x;
    arg3[4].pos.y = -normalizedPos.y;
    arg3[4].pos.z = -normalizedPos.z;
    arg3[4].pos.w = -(((arg3[4].pos.x * arg2[7].x) + (arg3[4].pos.y * arg2[7].y)) + (arg3[4].pos.z * arg2[7].z));

    func_80051C54(&arg2[0], &arg2[3], &arg2[1], &normalizedPos);
    guNormalize(&normalizedPos.x, &normalizedPos.y, &normalizedPos.z);
    arg3[5].pos.x = -normalizedPos.x;
    arg3[5].pos.y = -normalizedPos.y;
    arg3[5].pos.z = -normalizedPos.z;
    arg3[5].pos.w = -(((arg3[5].pos.x * arg2[0].x) + (arg3[5].pos.y * arg2[0].y)) + (arg3[5].pos.z * arg2[0].z));
    if (D_800BB170 != 0) {
        wSum = arg3[5].pos.w + arg3[4].pos.w;
        arg3[6].pos.x = arg3[4].pos.x;
        arg3[6].pos.y = arg3[4].pos.y;
        arg3[6].pos.z = arg3[4].pos.z;
        arg3[6].pos.w = -(arg3[5].pos.w - (wSum * 0.25f));

        arg3[7].pos.x = arg3[5].pos.x;
        arg3[7].pos.y = arg3[5].pos.y;
        arg3[7].pos.z = arg3[5].pos.z;
        arg3[7].pos.w = -arg3[6].pos.w;

        arg3[8].pos.x = arg3[4].pos.x;
        arg3[8].pos.y = arg3[4].pos.y;
        arg3[8].pos.z = arg3[4].pos.z;
        arg3[8].pos.w = -(arg3[5].pos.w - (wSum * 0.5f));

        arg3[9].pos.x = arg3[5].pos.x;
        arg3[9].pos.y = arg3[5].pos.y;
        arg3[9].pos.z = arg3[5].pos.z;
        arg3[9].pos.w = -arg3[8].pos.w;

        arg3[10].pos.x = arg3[4].pos.x;
        arg3[10].pos.y = arg3[4].pos.y;
        arg3[10].pos.z = arg3[4].pos.z;
        arg3[10].pos.w = -(arg3[5].pos.w - (wSum * 0.75f));

        arg3[11].pos.x = arg3[5].pos.x;
        arg3[11].pos.y = arg3[5].pos.y;
        arg3[11].pos.z = arg3[5].pos.z;
        arg3[11].pos.w = -arg3[10].pos.w;
    }
}

void func_800516BC(Object* obj, Vec3f* arg1, f32 arg2) {
    f32 temp_fv0;
    f32 temp_fa1;
    Camera* camera;
    f32 sp40[3];
    f32 sp34[3];
    f32 sp30;
    s32 i;
    s16 sp2A;

    set_camera_selector(0);
    camera = get_camera();
    sp2A = arctan2_f(camera->srt.transl.x - obj->srt.transl.x, camera->srt.transl.z - obj->srt.transl.z) ^ 0;
    sp30 = fsin16_precise(-sp2A);
    temp_fv0 = fcos16_precise(-sp2A);
    temp_fa1 = obj->unk64->unk2c * 0.3333f;
    for (i = 0; i < 8; i++) {
        sp40[0] = D_800B9840[i * 3 + 0] * arg2;
        sp40[1] = D_800B9840[i * 3 + 1] * arg2;
        sp40[2] = D_800B9840[i * 3 + 2] * temp_fa1;
        sp34[0] = (sp40[0] * temp_fv0) - (sp40[2] * sp30);
        sp34[2] = (sp40[0] * sp30) + (sp40[2] * temp_fv0);
        arg1->x = sp34[0];
        arg1->y = sp40[1] - 5.0f;
        arg1->z = sp34[2];
        arg1++;
    }
}

void func_80051944(s32 arg0, Object* arg1, Vec3f* arg2, f32 arg3, s16 arg4) {
    f32 temp_fa1;
    f32 temp_fs1;
    f32 temp_fs2;
    f32 temp_fs4;
    s32 temp_fv0;
    f32 var_fa0;
    Vec3f pos; // spAC
    s32 s8;
    s32 var_v0;
    SRT srt; // sp8C
    s32 temp_s7;
    s32 var_s1;
    f32 var_fv1;
    s32 i;

    temp_s7 = D_800B97E0[4] - D_800B97E0[1];
    temp_s7 >>= 1;
    temp_s7 *= arg3;

    srt.transl.x = 0.0f;
    srt.transl.y = 0.0f;
    srt.transl.z = 0.0f;
    srt.scale = 1.0f;
    srt.roll = 0;

    temp_fs1 = arg1->unk64->unk14.x;
    temp_fa1 = arg1->unk64->unk14.y;
    temp_fs2 = arg1->unk64->unk14.z;
    s8 = 8;
    if (temp_fs1 < 0.0f) {
        var_fv1 = -temp_fs1;
    } else {
        var_fv1 = temp_fs1;
    }
    if (temp_fs2 < 0.0f) {
        var_fa0 = -temp_fs2;
    } else {
        var_fa0 = temp_fs2;
    }
    if (arg1->def->shadowType != 2) {
        arg4 = -5;
    }
    if (var_fa0 < var_fv1) {
        var_v0 = arctan2s(var_fv1, temp_fa1);
    } else {
        var_v0 = arctan2_f(var_fa0, temp_fa1);
    }
    if (arg1->unk64->flags & 0x200) {
        srt.yaw = arg1->srt.yaw;
    } else {
        srt.yaw = arctan2_f(-temp_fs1, -temp_fs2);
    }
    // This needs to be u16???
    srt.pitch = (0x4000 - var_v0);
    if (srt.pitch < 0x1B58) {
        srt.pitch = 0x1B58;
    }
    temp_fs4 = 0.3333f;
    var_s1 = 0;
    for (i = 0; var_s1 < s8; var_s1++, arg2++, i++) {
        if (arg1->unk64->flags & 0x100) {
            pos.x = D_800B97E0[i * 3 + 0] * arg3;
            pos.y = (D_800B97E0[i * 3 + 1] - 8.0f) * arg3;
            pos.z = D_800B97E0[i * 3 + 2] * 1.5f;
        } else {
            pos.x = D_800B97E0[i * 3 + 0] * arg3;
            temp_fv0 = temp_s7 * ((0x4000 - var_v0) / 16384.0f);
            pos.y = (D_800B97E0[i * 3 + 1] * arg3) - temp_fv0;
            pos.z = (D_800B97E0[i * 3 + 2] * (arg1->unk64->unk2c * temp_fs4)) - temp_fv0;
        }
        rotate_vec3(&srt, pos.f);
        arg2->x = pos.x;
        arg2->y = pos.y - arg4;
        arg2->z = pos.z;
    }
}

void func_80051C54(Vec3f* A, Vec3f* B, Vec3f* C, Vec3f* D) {
    f32 temp_fa0;
    f32 temp_ft4;
    f32 temp_fv0;
    f32 sp8;
    f32 sp4;
    f32 sp0;
    
    temp_fv0 = C->x - A->x;
    temp_fa0 = C->y - A->y;
    temp_ft4 = C->z - A->z;
    sp0 = B->x - A->x;
    sp4 = B->y - A->y;
    sp8 = B->z - A->z;
    
    D->x = (sp4 * temp_ft4) - (sp8 * temp_fa0);
    D->y = -(((sp0) * temp_ft4) - (sp8 * temp_fv0));
    D->z = ((sp0) * temp_fa0) - (sp4 * temp_fv0);
}


s32 func_80051CFC(Vec3f* arg0, Vec3f* arg1) {
    s32 var_v1;
    float product;

    var_v1 = 1;
    if (D_80092BFC != 0) {
        return 1;
    }
    
    //Check for negative dot product?
    product = (arg0->x * arg1->x) + (arg0->y * arg1->y) + (arg0->z * arg1->z);
    if (product < 0.0f) {
        var_v1 = -1;
    }
    return var_v1;
}

s32 func_80051D68(Object* arg0, s16 arg1, s16 arg2, UnkFunc80051D68Arg3* arg3, s32 arg4, Vec4f* arg5) {
    UnkFunc80051D68Arg3* var_s3;
    s32 i;
    f32 f0;
    Vec3f sp70;
    Vec3f sp64;
    Vec3f sp58;
    s32 pad;
    Vec4f sp44;

    for (i = 0; i < arg4; i++) {
        var_s3 = &arg3[i];
        if (func_80051F64(arg1, arg2, var_s3->unkA, var_s3->unk16) == 1) {
            sp70.x = arg1;
            sp70.y = arg0->srt.transl.y;
            sp70.z = arg2;
            sp64.x = arg1;
            sp64.y = arg0->srt.transl.y + 100.0f;
            sp64.z = arg2;
            if (arg5 != NULL) {
                f0 = 0.00012208521f;
                arg5->x = var_s3->unk4 * f0;
                arg5->y = var_s3->unk6 * f0;
                arg5->z = var_s3->unk8 * f0;
                arg5->w = var_s3->unk0;
                return 1;
            }
            sp44.x = var_s3->unk4 * 0.00012208521f;
            sp44.y = var_s3->unk6 * 0.00012208521f;
            sp44.z = var_s3->unk8 * 0.00012208521f;
            sp44.w = var_s3->unk0;
            func_800528AC(&sp70, &sp64, &sp58, &sp44);
            return sp58.y - sp70.y;
        }
    }

    return 0;
}

s32 func_80051F64(s16 arg0, s16 arg1, s16 *arg2, s16 *arg3) {
    s16 temp_t1;
    s16 temp_t3;
    s16 temp_t4;
    s16 temp_v1;
    Unk80092CA8 sp38;
    Unk80092CA8 sp30;
    s16 var_v0;

    sp38 = D_80092CA8;
    sp30 = D_80092CB0;
    for (var_v0 = 0; var_v0 < 3; var_v0++) {
        temp_v1 = arg2[sp30.unk0[var_v0]] - arg2[sp38.unk0[var_v0]];
        temp_t1 = arg3[sp30.unk0[var_v0]] - arg3[sp38.unk0[var_v0]];
        temp_t3 = arg0 - arg2[sp38.unk0[var_v0]];
        temp_t4 = arg1 - arg3[sp38.unk0[var_v0]];
        if ((temp_t1 * temp_t3) < (temp_v1 * temp_t4)) {
            return 0;
        }
        if ((temp_v1 * temp_t4) >= (temp_t1 * temp_t3)) {
            if (((temp_v1 * temp_t3) < 0) || ((temp_t1 * temp_t4) < 0)) {
                return 0;
            }
            if (((temp_v1 * temp_v1) + (temp_t1 * temp_t1)) < ((temp_t3 * temp_t3) + (temp_t4 * temp_t4))) {
                return 0;
            }
            return 1;
        }
    }
    return 1;
}


s32 func_80052148(Vec3f* arg0, Vec3f* arg1) {
    f32 sp44;
    s32 pad;
    f32 sp3C;
    f32 temp;
    s32 var_v1;

    var_v1 = 1;
    if (D_80092BFC != 0) {
        return 1;
    }
    sp44 = (arg0->x * arg1->x) + (arg0->y * arg1->y) + (arg0->z * arg1->z);
    if (sp44 < 0.0f) {
        temp = ((arg0->x * arg0->x) + (arg0->y * arg0->y) + (arg0->z * arg0->z));
        sp3C = (arg1->x * arg1->x) + (arg1->y * arg1->y) + (arg1->z * arg1->z);
        temp *= sp3C;
        var_v1 = 1;
        if ((sp44 / sqrtf(temp)) < -0.2f) {
            var_v1 = -1;
        }
    }
    return var_v1;
}

/** 
  * Seems to have something to do with getting the dot product of two vectors?
  */
void func_80052230(Vec3f *A, Vec3f *B, f32 *arg2)
{
    f32 AdotB;
    f32 AdotA;
    f32 BdotB;
    f32 product;
    Vec3f *Acopy;
    
    Acopy = A;
    AdotB = ((A->x * B->x) + (A->y * B->y)) + (A->z * B->z);
    product = (A->x * Acopy->x) + (A->y * A->y) + (A->z * A->z);
    AdotA = product;
    BdotB = ((B->x * B->x) + (B->y * B->y)) + (B->z * B->z);
    product = AdotA * BdotB;
    
    if (product){
        BdotB = sqrtf(product);
    }
    if (BdotB != 0){
        *arg2 = AdotB / BdotB;
        return;
    }
    *arg2 = 0;
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80052300.s")
#else
s32 func_80052300(Object* arg0, UnkFunc80051D68Arg3 *arg1, Unk8004FA58* arg2, UnkFunc80052300Arg3* arg3, s32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8) {
    s32 sp44;
    s32 var_a2;
    Unk8005341C* temp_t2;
    s8 var_t1;
    s8 var_v1;
    f32 sp34;
    f32 sp30;
    Unk8005341C* temp_v0;
    f32 f0;

    sp34 = arg0->srt.transl.x;
    sp30 = arg0->srt.transl.z;
    if (arg0->parent == NULL) {
        sp34 -= arg5;
        sp30 -= arg6;
    }
    temp_v0 = func_8005341C(&sp44);
    temp_t2 = &temp_v0[sp44];
    sp44 = 0;
    var_a2 = 0;
    if (arg8) {
        var_t1 = 4;
    } else {
        var_t1 = 8;
    }
    if (temp_v0 < temp_t2) {
        f0 = 0.00012208521f;
        do {
            if (temp_v0->unk0 == arg0->parent) {
                sp44 = temp_v0->unk4;
                while (sp44 < temp_v0[1].unk4) {
                    var_v1 = TRUE;
                    if (!(arg1[sp44].unk30 & var_t1)) {
                        var_v1 = FALSE;
                    }
                    if (var_v1 != FALSE) {
                        arg3->unk0[0].x = arg1[sp44].unkA[0] - sp34;
                        arg3->unk0[0].y = arg1[sp44].unkA[3] - arg0->srt.transl.y;
                        arg3->unk0[0].z = arg1[sp44].unk16[0] - sp30;
                        arg3->unk0[1].x = arg1[sp44].unkA[1] - sp34;
                        arg3->unk0[1].y = arg1[sp44].unkA[4] - arg0->srt.transl.y;
                        arg3->unk0[1].z = arg1[sp44].unk16[1] - sp30;
                        arg3->unk0[2].x = arg1[sp44].unkA[2] - sp34;
                        arg3->unk0[2].y = arg1[sp44].unkA[5] - arg0->srt.transl.y;
                        arg3->unk0[2].z = arg1[sp44].unk16[2] - sp30;
                        arg3 += 1;
                        arg2[var_a2].pos.x = arg1[sp44].unk4 * f0;
                        arg2[var_a2].pos.y = arg1[sp44].unk6 * f0;
                        arg2[var_a2].pos.z = arg1[sp44].unk8 * f0;
                        arg2[var_a2].unk10 = arg1[sp44].unk30;
                        var_a2++;
                    }
                    sp44++;
                }
            }
            temp_v0++;
        } while (temp_v0 < temp_t2);
    }
    return var_a2;
}
#endif

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/newshadows/func_80052644.s")
#else
// https://decomp.me/scratch/0NfF9
void func_80052644(u8* source, u8* dest, s32 arg2, s32* outCount, Vec4f* arg4, s32 length, void (*arg6)(Vec3f*, Vec3f*, Vec3f*, f32), u8 someFlag) {
    Vec3f* var_s3;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fv0;
    f32 var_fv0;
    s32 var_s7;
    f32 new_var;
    Vec3f *sourceAsVec;
    Vec3f *destAsVec;

    new_var = 0.001000000047f;
    *outCount = 0;
    if (arg2 != 0) {
        if (1) {}
        var_s3 = (Vec3f *)((arg2 - 1) * length + (s8 *)source);
        var_fv0 = (var_s3->x * arg4->x) + (var_s3->y * arg4->y) + (var_s3->z * arg4->z) + arg4->w;
        for (var_s7 = 0; var_s7 < arg2; var_s7++) {
            sourceAsVec = (Vec3f *) source;
            destAsVec = (Vec3f *) dest;
            temp_fs0 = (sourceAsVec->x * arg4->x) + (sourceAsVec->y * arg4->y) + (sourceAsVec->z * arg4->z) + arg4->w;
            if (temp_fs0 < new_var) {
                if (var_fv0 < new_var) {
                    bcopy(source, dest, length);
                    dest += length;
                    *outCount += 1;
                } else {
                    temp_fv0 = func_800528AC(var_s3, sourceAsVec, destAsVec, arg4);
                    if (arg6 != NULL) {
                        arg6(var_s3, sourceAsVec, destAsVec, temp_fv0);
                    }
                    dest += length;
                    bcopy(source, dest, length);
                    dest += length;
                    *outCount += 2;
                }
            } else if (var_fv0 < new_var) {
                temp_fv0 = func_800528AC(sourceAsVec, var_s3, destAsVec, arg4);
                if (arg6 != NULL) {
                    arg6(sourceAsVec, var_s3, destAsVec, temp_fv0);
                }
                dest += length;
                *outCount += 1;
                if (someFlag) {
                    bcopy(source, dest, length);
                    dest += length;
                    *outCount += 1;
                }
            } else if (someFlag) {
                bcopy(source, dest, length);
                dest += length;
                *outCount += 1;
            }
            var_fv0 = temp_fs0;
            var_s3 = sourceAsVec;
            source += length;
        }
    }
}
#endif

f32 func_800528AC(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec4f* arg3) {
    f32 zDiff;
    f32 yDiff;
    f32 xDiff;
    f32 out;
    f32 temp;
    f32 temp2;

    xDiff = arg1->x - arg0->x;
    yDiff = arg1->y - arg0->y;
    zDiff = arg1->z - arg0->z;
    temp = (arg0->x * arg3->x) + (arg0->y * arg3->y) + (arg0->z * arg3->z) + arg3->w;
    temp2 = (arg3->x * xDiff) + (yDiff * arg3->y) + (zDiff * arg3->z);
    out = 0.0f;
    if (temp2 != 0.0f) {
        out = -temp / temp2;
        arg2->x = arg0->x + (out * xDiff);
        arg2->y = arg0->y + (out * yDiff);
        arg2->z = arg0->z + (out * zDiff);
    } else {
        arg2->x = arg0->x;
        arg2->y = arg0->y;
        arg2->z = arg0->z;
    }

    return out;
}
