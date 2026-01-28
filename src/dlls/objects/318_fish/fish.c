#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object_id.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/314_foodbag.h"
#include "functions.h"
#include "dll.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC[2];
    f32 unk14;
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    s16 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s16 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    u8 unk7C;
} fish_Data;

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u16 unk1C;
    u16 unk1E;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u16 unk26;
    u8 unk28;
} fish_Setup;

void dll_318_func_10BC(Object* self);
static void dll_318_func_11C8(Object *self, s16 a1, u8 a2);
s32 dll_318_func_1978(Object* self);
s32 dll_318_func_1EF4(Object* self);
void dll_318_func_2150(Object* self);
static void dll_318_func_84(Object* self);
s32 dll_318_func_F74(Object* self);
static s32 dll_318_func_1880(Object *self, Vec3f* arg1, Vec3s16* arg2);

// offset: 0x0 | ctor
void dll_318_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_318_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_318_setup(Object *self, fish_Setup *setup, s32 arg2) {
    fish_Data *objdata = self->data;
    if (setup->unk28 == 0) {
        setup->unk28 = 1U;
    }
    objdata->unk7C = 0;
    dll_318_func_84(self);
    self->unkB0 |= 0x2000;
}

// offset: 0x84 | func: 1
static void dll_318_func_84(Object* self) {
    fish_Data* objdata;
    fish_Setup* setup;
    f32 temp_fa1;
    Vec4f sp2C;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv0_2;
    f32 temp_fv0_3;

    objdata = self->data;
    setup = (fish_Setup*)self->setup;
    objdata->unk4 = 0.0f;
    objdata->unk8 = 0.0f;
    objdata->unk14 = ((f32) setup->unk1B / SQ(SQ(SQ((f32) setup->unk26))));
    objdata->unk18 = 1;
    objdata->unk1A = 0;
    objdata->unk19 = 0;
    objdata->unk1C = 0;
    objdata->unk20 = ((f32) setup->unk20 / 100.0f);
    objdata->unk24 = ((f32) setup->unk20 / 1000.0f);
    objdata->unk28 = 0.0f;
    if (func_80058B1C(self, self->srt.transl.f[0], self->srt.transl.f[1], self->srt.transl.f[2], &objdata->unk2C, 0U) != 0) {
        objdata->unk2C = ((setup->base.y + objdata->unk2C) - (f32) setup->unk28);
        objdata->unkC[0] = ((f32) setup->unk28 / 0.05f);
        objdata->unkC[1] = (((f32) setup->unk19 / 100.0f) * 20.0f);
        objdata->unk44 = (f32) setup->unk21;
        objdata->unk48 = (f32) setup->unk22;
        objdata->unk50 = (f32) (objdata->unk44 / objdata->unk48);
        objdata->unk4C = ((objdata->unk48 * -0.04f) * objdata->unk48) / -objdata->unk48;
        temp_fa1 = objdata->unk48 * ((2.0f * objdata->unk4C) + (objdata->unk48 * -0.04f)) * 0.25f;
        temp_fv0_2 = temp_fa1 * 0.5f;
        sp2C.f[0] = temp_fv0_2;
        sp2C.f[1] = temp_fa1;
        sp2C.f[2] = temp_fv0_2 * temp_fv0_2;
        sp2C.f[3] = temp_fa1 * temp_fa1;
        objdata->unk70 = (f32) (16384.0f / ((2.0f * sp2C.f[2]) - sp2C.f[3]));
        objdata->unk78 = (f32) ((8192.0f - (objdata->unk70 * sp2C.f[2])) / sp2C.f[0]);
        temp_ft4 = SQ(objdata->unk4C);
        temp_ft5 = -((f32) setup->unk28) * -0.16f;
        if (!(temp_ft4 < temp_ft5)) {
            temp_fv0_3 = sqrtf(temp_ft4 - temp_ft5);
            objdata->unk64 = ((-objdata->unk4C + temp_fv0_3) / -0.08f);
            objdata->unk68 = ((-objdata->unk4C - temp_fv0_3) / -0.08f);
            objdata->unk34 = ((sqrtf(SQ(objdata->unk50) + SQ(objdata->unk4C)) - objdata->unk20) / (f32) setup->unk23);
            objdata->unk38 = ((objdata->unk34 * (f32) setup->unk23 * (f32) setup->unk23) + (objdata->unk20 * (f32) setup->unk23));
            objdata->unk40 = -((f32) setup->unk28 * 4.0f);
            objdata->unk40 = (objdata->unk40 / (objdata->unk38 * objdata->unk38));
            objdata->unk3C = (-objdata->unk40 * objdata->unk38);
            sp2C.f[2] = SQ((f32) setup->unk28 * 0.5f);
            sp2C.f[0] = (f32) setup->unk28 * 0.5f;
            sp2C.f[3] = (f32) setup->unk28 * (f32) setup->unk28;
            sp2C.f[1] = (f32) setup->unk28;
            objdata->unk6C = 8192.0f / ((2.0f * sp2C.f[2]) - sp2C.f[3]);
            objdata->unk74 = (4096.0f - (objdata->unk6C * sp2C.f[2])) / sp2C.f[0];
            self->srt.yaw = setup->unk18 << 8;
            self->srt.scale = self->def->scale * ((f32) setup->unk19 / 100.0f);
            objdata->unk30 = self->objhitInfo->unk58;
            objdata->unk7C = 1;
        }
    }
}

// offset: 0x630 | func: 2 | export: 1
#ifndef NON_MATCHING
void dll_318_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/318_fish/dll_318_control.s")
#else
void dll_318_control(Object* self) {
    s32 temp_v0;
    fish_Setup* setup;
    f32 var_fa1;
    fish_Data* objdata;
    Object* player;
    f32 temp_fa0;
    f32 temp_fv1_2;
    f32 temp_fv1_3;

    setup = (fish_Setup*)self->setup;
    objdata = self->data;
    
    player = get_player();
    if (objdata->unk7C == 0) {
        dll_318_func_84(self);
    }
    if (player->id == 0x1F) {
        if (main_get_bits(BIT_2EA) == 0) {
            self->unkAF |= 8;
        } else {
            self->unkAF &= ~0x8;
        }
    } else {
        if (main_get_bits(BIT_3E1) == 0) {
            self->unkAF |= 8;
        } else {
            self->unkAF &= ~0x8;
        }
    }
    temp_v0 = func_80025F40(self, NULL, NULL, NULL);
    if ((temp_v0 != 0) && (temp_v0 == 0x11)) {
        dll_318_func_2150(self);
        return;
    }
    objdata->unk4 += gUpdateRateF;
    switch (objdata->unk18) {
    case 0:
        self->unkAF |= 8;
        if ((f32) setup->unk1E <= objdata->unk4) {
            objdata->unk18 = 1U;
            objdata->unk4 -= (f32) setup->unk1E;
        }
        break;
    case 1:
        if (objdata->unk4 <= 60.0f) {
            self->unkAF |= 8;
            self->opacity = (u8) (s32) ((objdata->unk4 / 60.0f) * 255.0f);
            dll_318_func_11C8(self, NULL, 0);
        } else {
            self->opacity = 0xFF;
            self->objhitInfo->unk58 = objdata->unk30;
            objdata->unk18 = 2U;
            objdata->unk4 -= 60.0f;
            dll_318_func_11C8(self, NULL, 0);
        }
        break;
    case 2:
        if (objdata->unk4 <= (f32) setup->unk1C) {
            dll_318_func_10BC(self);
        } else {
            self->srt.transl.f[1] = setup->base.y;
            objdata->unk18 = 3U;
            objdata->unk4 = (f32) (objdata->unk4 - (f32) setup->unk1C);
            dll_318_func_11C8(self, NULL, 0);
            func_80023D30(self, 2, 0.0f, 0U);
            func_80024D74(self, 0x1E);
        }
        break;
    case 3:
        if (objdata->unk4 <=  (f32) setup->unk1A) {
            self->srt.transl.f[1] = setup->base.y + (((objdata->unk2C - setup->base.y) * objdata->unk4) /  (f32) setup->unk1A);
            dll_318_func_10BC(self);
        } else {
            self->srt.transl.f[1] = objdata->unk2C;
            objdata->unk18 = 4U;
            objdata->unk4 = (f32) (objdata->unk4 - (f32) setup->unk1A);
            dll_318_func_11C8(self, NULL, 0);
            func_80023D30(self, 0, 0.0f, 0U);
            func_80024D74(self, 0x1E);
        }
        break;
    case 4:
        if ((f32) setup->unk26 <= objdata->unk4) {
            if (dll_318_func_F74(self) != 0) {
                objdata->unk4 = (f32) gUpdateRateF;
                objdata->unk18 = 5U;
                objdata->unk58 = (f32) self->srt.transl.f[0];
                objdata->unk5C = (f32) self->srt.transl.f[1];
                objdata->unk60 = (f32) self->srt.transl.f[2];
                objdata->unk54 = 0.0f;
                
                func_80023D30(self, 1, 0.0f, 0U);
                func_80024D74(self, 0x1E);
                dll_318_func_1EF4(self);
            } else {
                dll_318_func_11C8(self, NULL, 0);
            }
        } else {
            dll_318_func_10BC(self);
        }
        temp_fv1_2 = objdata->unk4 * objdata->unk4;
        temp_fa0 = temp_fv1_2 * temp_fv1_2;
        objdata->unk8 += gUpdateRateF;
        var_fa1 = temp_fa0 * temp_fa0 * objdata->unk14;
        if ((f32) setup->unk1B < var_fa1) {
            var_fa1 = (f32) setup->unk1B;
        }
        temp_fv1_3 = 60.0f / var_fa1;
        if (temp_fv1_3 <= objdata->unk8) {
            objdata->unk8 = (f32) (objdata->unk8 - temp_fv1_3);
            if ((player != NULL) && (vec3_distance_squared(&self->positionMirror, &player->positionMirror) < 10000.0f)) {
                gDLL_17_partfx->vtbl->spawn(self, 0x26, NULL, 0, -1, objdata->unkC);
            }
        }
        break;
    case 5:
        if (dll_318_func_1EF4(self) == 0) {
            objdata->unk58 = (f32) self->srt.transl.f[0];
            objdata->unk5C = (f32) self->srt.transl.f[1];
            objdata->unk60 = (f32) self->srt.transl.f[2];
            dll_318_func_1978(self);
        }
        break;
    case 6:
        if (dll_318_func_1978(self) == 0) {
            dll_318_func_11C8(self, NULL, 0);
        }
        break;
    case 7:
        if (objdata->unk4 <= (f32) setup->unk24) {
            self->srt.transl.f[1] = objdata->unk2C - (((objdata->unk2C - setup->base.y) * objdata->unk4) / (f32) setup->unk24);
            dll_318_func_10BC(self);
        } else {
            self->srt.transl.f[1] = setup->base.y;
            objdata->unk18 = 2U;
            objdata->unk4 = (f32) (objdata->unk4 - (f32) setup->unk24);
            dll_318_func_11C8(self, NULL, 0);
            func_80023D30(self, 0, 0.0f, 0U);
            func_80024D74(self, 0x1E);
        }
        break;
    }
    func_80024108(self, objdata->unk0, gUpdateRateF, NULL);
}
#endif

// offset: 0xEA8 | func: 3 | export: 2
void dll_318_update(Object *self) { }

// offset: 0xEB4 | func: 4 | export: 3
void dll_318_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    fish_Data* objdata = self->data;
    if ((objdata->unk18 != 0) && (visibility != 0)) {
        self->srt.yaw -= 0x8000;
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        self->srt.yaw -= 0x8000;
    }
}

// offset: 0xF40 | func: 5 | export: 4
void dll_318_free(Object *self, s32 a1) { }

// offset: 0xF50 | func: 6 | export: 5
u32 dll_318_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF60 | func: 7 | export: 6
u32 dll_318_get_data_size(Object *self, u32 a1) {
    return sizeof(fish_Data);
}

// offset: 0xF74 | func: 8
s32 dll_318_func_F74(Object *self) {
    fish_Data* objdata;
    Vec3s16 sp64;
    s32 _pad[3];
    Vec3f sp4C;
    Vec3f sp40;
    s32 _pad2;
    
    objdata = self->data;
    if (objdata->unk28 != objdata->unk20) {
        return 0;
    }
    sp64.x = self->srt.yaw;
    sp64.y = 0;
    sp64.z = 0;
    sp4C.x = objdata->unk38;
    sp4C.x = objdata->unk38 + objdata->unk44 + 10.0f;
    sp4C.y = 0.0f;
    sp4C.z = 0.0f;
    rotate_vec3(&sp64, sp4C.f);
    sp4C.x += self->srt.transl.f[0];
    sp4C.y += self->srt.transl.f[1];
    sp4C.z += self->srt.transl.f[2];
    sp40.x = self->srt.transl.f[0];
    sp40.y = self->srt.transl.f[1];
    sp40.z = self->srt.transl.f[2];
    if (func_80059C40(&sp40, &sp4C, 0.1f, 1, NULL, self, 4, -1, 0xFFU, 0) != 0) {
        return 0;
    }
    return 1;
}

// offset: 0x10BC | func: 9
void dll_318_func_10BC(Object *self) {
    fish_Data *objdata = self->data;

    if (objdata->unk19 == 0) {
        if (rand_next(0, 0x63) == 0) {
            dll_318_func_11C8(self, rand_next(-0x100, 0x100), rand_next(1, 0x64));
        } else {
            dll_318_func_11C8(self, rand_next(-0x100, 0x100), 0U);
        }
    } else {
        dll_318_func_11C8(self, 0, 0U);
    }
}

// offset: 0x11C8 | func: 10
static void dll_318_func_11C8(Object *self, s16 a1, u8 a2) {
    fish_Data *objdata;
    Vec3s16 sp5C;
    Vec3f sp50;
    Vec3f sp44;

    objdata = self->data;
    objdata->unk1A += ((f32) a1 * gUpdateRateF);
    if (a2 != 0) {
        objdata->unk28 -= ((objdata->unk20 * (f32) a2) / 100.0f);
        if (objdata->unk28 < 0.0f) {
            objdata->unk28 = 0.0f;
        }
    }
    if (objdata->unk1A > 0) {
        if (objdata->unk28 == 0.0f) {
            objdata->unk1C += gUpdateRateF;
        } else {
            objdata->unk1C += (5.0f * gUpdateRateF);
        }
        if (objdata->unk1C > 0x100) {
            objdata->unk1C = 0x100;
        }
        self->srt.yaw += ((f32) objdata->unk1C * gUpdateRateF);
        objdata->unk1A -= ((f32) objdata->unk1C * gUpdateRateF);
        if (objdata->unk1A <= 0) {
            objdata->unk1A = 0;
            objdata->unk1C = 0;
        }
    } else if (objdata->unk1A < 0) {
        if (objdata->unk28 == 0.0f) {
            objdata->unk1C -= gUpdateRateF;
        } else {
            objdata->unk1C -= (5.0f * gUpdateRateF);
        }
        if (objdata->unk1C < -0x100) {
            objdata->unk1C = -0x100;
        }
        self->srt.yaw += ((f32) objdata->unk1C * gUpdateRateF);
        objdata->unk1A -= ((f32) objdata->unk1C * gUpdateRateF);
        if (objdata->unk1A >= 0) {
            objdata->unk1A = 0;
            objdata->unk1C = 0;
        }
    }
    sp5C.x = self->srt.yaw;
    sp5C.y = 0;
    sp5C.z = 0;
    sp50.x = gUpdateRateF * 10.0f;
    sp50.y = 0.0f;
    sp50.z = 0.0f;
    rotate_vec3(&sp5C, sp50.f);
    sp50.x += self->srt.transl.f[0];
    sp50.y += self->srt.transl.f[1];
    sp50.z += self->srt.transl.f[2];
    if (func_80059C40(&self->srt.transl, &sp50, 1.0f, 0, NULL, self, 4, -1, 0xFFU, 0) != 0) {
        objdata->unk0 = 0.06f;
        if (objdata->unk1C > 0) {
            self->srt.yaw += (gUpdateRateF * 512.0f);
            objdata->unk1A = 0x500;
        } else {
            self->srt.yaw -= (gUpdateRateF * 512.0f);
            objdata->unk1A = -0x500;
        }
        objdata->unk28 = 0.0f;
        return;
    }
    if (objdata->unk28 == objdata->unk20) {
        func_800240BC(self, 0.0f);
        objdata->unk0 = 0.0f;
    } else {
        objdata->unk0 = (f32) ((objdata->unk28 / objdata->unk20) * 0.06f);
    }
    sp44.x = gUpdateRateF * 30.0f * 5.0f;
    sp44.y = 0.0f;
    sp44.z = 0.0f;
    rotate_vec3(&sp5C, sp44.f);
    sp44.x += self->srt.transl.f[0];
    sp44.y += self->srt.transl.f[1];
    sp44.z += self->srt.transl.f[2];
    sp50.x = objdata->unk28 * gUpdateRateF;
    sp50.y = 0.0f;
    sp50.z = 0.0f;
    rotate_vec3(&sp5C, sp50.f);
    self->srt.transl.f[0] += sp50.x;
    self->srt.transl.f[1] += sp50.y;
    self->srt.transl.f[2] += sp50.z;
    if (func_80059C40(&self->srt.transl, &sp44, 1.0f, 0, NULL, self, 4, -1, 0xFFU, 0) != 0) {
        dll_318_func_1880(self, &sp44, &sp5C);
        if (objdata->unk28 < objdata->unk24) {
            objdata->unk28 += ((objdata->unk20 / 100.0f) * gUpdateRateF);
            return;
        }
        objdata->unk28 -= ((objdata->unk20 / 50.0f) * gUpdateRateF);
        if (objdata->unk28 < 0.0f) {
            objdata->unk28 = 0.0f;
        }
    } else {
        objdata->unk19 = 0;
        objdata->unk28 += ((objdata->unk20 / 100.0f) * gUpdateRateF);
        if (objdata->unk20 < objdata->unk28) {
            objdata->unk28 = objdata->unk20;
        }
    }
}

// offset: 0x1880 | func: 11
static s32 dll_318_func_1880(Object *self, Vec3f* arg1, Vec3s16* arg2) {
    fish_Data* objdata;

    objdata = self->data;
    if (objdata->unk19 == 0) {
        arg1->f[0] -= self->srt.transl.f[0];
        arg1->f[1] -= self->srt.transl.f[1];
        arg1->f[2] -= self->srt.transl.f[2];
        arg2->x = -self->srt.yaw;
        rotate_vec3(arg2, arg1->f);
        objdata->unk19 = 1U;
        if (arg1->f[2] > 0.0f) {
            objdata->unk1A = -0x100;
        } else {
            objdata->unk1A = 0x100;
        }
    } else if (objdata->unk1A > 0) {
        objdata->unk1A = 0x100;
    } else {
        objdata->unk1A = -0x100;
    }
    return 1;
}

// offset: 0x1978 | func: 12
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/318_fish/dll_318_func_1978.s")
#else
s32 dll_318_func_1978(Object *self) {
    fish_Data* objdata;
    fish_Setup* setup;
    Vec3f sp6C;
    Vec3s16 sp64;
    s32 sp60;

    objdata = self->data;
    setup = (fish_Setup*)self->setup;
    sp60 = 1;
    sp64.x = self->srt.yaw;
    sp64.y = 0;
    sp64.z = 0;

    if (objdata->unk54 < objdata->unk64) {
        if (objdata->unk64 < objdata->unk4) {
            Vec3f sp54;
            sp54.f[0] = objdata->unk64 * objdata->unk50;
            sp54.f[1] = 0.0f;
            sp54.f[2] = 0.0f;
            rotate_vec3(&sp64, sp54.f);
            sp54.f[0] = sp54.f[0] + objdata->unk58;
            sp54.f[1] = (f32) setup->unk28 + objdata->unk2C;
            sp54.f[2] = sp54.f[2] + objdata->unk60;
            gDLL_24_Waterfx->vtbl->func_174C(sp54.f[0],        sp54.f[1], sp54.f[2], 0.0f);
            gDLL_24_Waterfx->vtbl->func_174C(sp54.f[0] + 5.0f, sp54.f[1], sp54.f[2], 0.0f);
            gDLL_24_Waterfx->vtbl->func_1CC8(sp54.f[0] - 5.0f, sp54.f[1], sp54.f[2] - 5.0f, self->srt.yaw, 0.0f, 4);
            gDLL_24_Waterfx->vtbl->func_1CC8(sp54.f[0] - 5.0f, sp54.f[1], sp54.f[2],        self->srt.yaw, 0.0f, 3);
            gDLL_24_Waterfx->vtbl->func_1CC8(sp54.f[0] - 5.0f, sp54.f[1], sp54.f[2] + 5.0f, self->srt.yaw, 0.0f, 4);
        }
    } else {
        if ((objdata->unk54 < objdata->unk68) && (objdata->unk68 < objdata->unk4)) {
            Vec3f sp48;
            sp48.f[0] = objdata->unk68 * objdata->unk50;
            sp48.f[1] = 0.0f;
            sp48.f[2] = 0.0f;
            rotate_vec3(&sp64, sp48.f);
            sp48.f[0] = sp48.f[0] + objdata->unk58;
            sp48.f[1] = (f32) setup->unk28 + objdata->unk2C;
            sp48.f[2] = sp48.f[2] + objdata->unk60;
            gDLL_24_Waterfx->vtbl->func_174C(sp48.f[0],        sp48.f[1], sp48.f[2], 0.0f);
            gDLL_24_Waterfx->vtbl->func_174C(sp48.f[0] - 5.0f, sp48.f[1], sp48.f[2], 0.0f);
            gDLL_24_Waterfx->vtbl->func_1CC8(sp48.f[0] + 5.0f, sp48.f[1], sp48.f[2] - 5.0f, self->srt.yaw, 0.0f, 4);
            gDLL_24_Waterfx->vtbl->func_1CC8(sp48.f[0] + 5.0f, sp48.f[1], sp48.f[2], self->srt.yaw, 0.0f, 3);
            gDLL_24_Waterfx->vtbl->func_1CC8(sp48.f[0] + 5.0f, sp48.f[1], sp48.f[2] + 5.0f, self->srt.yaw, 0.0f, 4);
        }
    }
    if (objdata->unk48 < objdata->unk4) {
        sp6C.f[0] = objdata->unk44;
        sp6C.f[1] = 0.0f;
        sp6C.f[2] = 0.0f;
        self->srt.roll = 0;
        objdata->unk18 = 7;
        objdata->unk4 = (f32) (objdata->unk4 - objdata->unk48);
        func_80023D30(self, 3, 0.0f, 0U);
        func_80024D74(self, 0x1E);
        sp60 = 0;
    } else {
        sp6C.f[0] = objdata->unk4 * objdata->unk50;
        sp6C.f[1] = (objdata->unk4C * objdata->unk4) + (-0.04f * objdata->unk4 * objdata->unk4);
        sp6C.f[2] = 0.0f;
        if (objdata->unk4 < (objdata->unk48 * 0.5f)) {
            self->srt.roll = (s16) (s32) -((sp6C.f[1] * objdata->unk78) + (objdata->unk70 * sp6C.f[1] * sp6C.f[1]));
        } else {
            self->srt.roll = (s16) (s32) ((sp6C.f[1] * objdata->unk78) + (objdata->unk70 * sp6C.f[1] * sp6C.f[1]));
        }
        objdata->unk54 = (f32) objdata->unk4;
    }
    rotate_vec3(&sp64, sp6C.f);
    self->srt.transl.f[0] = objdata->unk58 + sp6C.f[0];
    self->srt.transl.f[1] = objdata->unk5C + sp6C.f[1];
    self->srt.transl.f[2] = objdata->unk60 + sp6C.f[2];
    return sp60;
}
#endif

// offset: 0x1EF4 | func: 13
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/318_fish/dll_318_func_1EF4.s")
#else
s32 dll_318_func_1EF4(Object* self) {
    fish_Data* objdata;
    fish_Setup* setup;
    Vec3f sp34;
    Vec3s16 sp2C;
    s32 var_a2;
    f32 temp;

    setup = (fish_Setup*)self->setup;
    objdata = self->data;
    sp2C.x = self->srt.yaw;
    sp2C.y = 0;
    sp2C.z = 0;
    var_a2 = 1;
    if ((f32) setup->unk23 < objdata->unk4) {
        sp34.f[0] = objdata->unk38;
        sp34.f[1] = 0.0f;
        sp34.f[2] = 0.0f;
        self->srt.roll = 0;
        func_80023D30(self, 0, 0.0f, 0U);
        func_80024D74(self, 0x1E);
        objdata->unk0 = 0.0f;
        objdata->unk4 = (f32) (objdata->unk4 - (f32) setup->unk23);
        objdata->unk18 = 6;
        objdata->unk54 = 0.0f;
        var_a2 = 0;
    } else {
        sp34.f[0] = (objdata->unk34 * objdata->unk4 * objdata->unk4) + (objdata->unk20 * objdata->unk4);
        sp34.f[1] = (sp34.f[0] * (objdata->unk40 * sp34.f[0])) + (objdata->unk3C * sp34.f[0]);
        sp34.f[2] = 0.0f;
        if (sp34.f[0] < (objdata->unk38 * 0.5f)) {
            self->srt.roll = (s16) (s32) ((sp34.f[1] * objdata->unk74) + (objdata->unk6C * sp34.f[1] * sp34.f[1]));
        } else {
            temp =  sp34.f[1] * sp34.f[1];
            self->srt.roll = (s16) (s32) -((sp34.f[1] * objdata->unk74) + (objdata->unk6C * temp));
        }
        objdata->unk54 = (f32) objdata->unk4;
        objdata->unk0 = 0.12f;
    }
    rotate_vec3(&sp2C, sp34.f);
    self->srt.transl.f[0] = objdata->unk58 + sp34.f[0];
    self->srt.transl.f[1] = objdata->unk5C - sp34.f[1];
    self->srt.transl.f[2] = objdata->unk60 + sp34.f[2];
    return var_a2;
}
#endif

// offset: 0x2150 | func: 14
void dll_318_func_2150(Object *self) {
    fish_Data *objdata;
    s32 _pad;
    Object* player;
    ObjSetup *setup;
    Object* foodbag;

    objdata = self->data;
    setup = self->setup;
    if (self->id == OBJ_DF_Lantern) { // ????????
        main_set_bits(BIT_Gold_Nugget_LFV, 1);
    } else {
        player = get_player();
        foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
        ((DLL_314_Foodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Fish);
    }
    objdata->unk18 = 0;
    objdata->unk4 = 0.0f;
    self->srt.transl.x = setup->x;
    self->srt.transl.y = setup->y;
    self->srt.transl.z = setup->z;
    self->srt.roll = 0;
    self->srt.yaw = 0;
    func_800267A4(self);
}
