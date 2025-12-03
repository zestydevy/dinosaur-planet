#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/engine/53.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/332_FXEmit.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/objects.h"
#include "sys/objanim.h"
#include "sys/objtype.h"

typedef struct {
    u32 unk0_0 : 7;
    u32 unk0_7 : 1;
    u32 unk0_8 : 1;
    u32 unk0_9 : 1;
    u32 unk0_10 : 1;
    u32 unk0_11: 1;
    u32 unk0_12: 1;
    u32 unk0_13: 1;
    u32 unk0_14 : 1;
    u32 unk0_15 : 8;
    u32 unk0_23 : 1;
    u32 pad0_32: 8;
    Object *player;
    Object *unk8[2];
    Object *unk10[6];
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    u16 unk30;
    u16 unk32;
    s16 unk34;
    u16 unk36;
    u16 unk38;
    u32 soundHandle1;
    u32 soundHandle2;
    u32 soundHandle3;
    Vec3f unk48[1]; // unk length (15?)
    u8 _unk54[0xF0 - 0x54];
    f32 unkF0; // part of unk48
    f32 unkF4; // part of unk48
    f32 unkF8; // part of unk48
    f32 unkFC;
} KamerianBoss_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18[0x2A-0x18];
/*2A*/ s8 yaw;
} KamerianBoss_Setup;

typedef struct {
    Texture *texture;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} BSS8;

// TODO: the 2nd arg is something else, see rcp.c
extern void func_800390A4(Gfx**, BSS8*, f32, f32, f32, f32, s32, s32, f32, f32, s32, s32);

/*0x0*/ static u8 _data_0[] = {0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x04};
/*0x8*/ static Model *_data_8 = NULL;
/*0xC*/ static s16 _data_C[2] = {0x042d, 0x0422};

/*0x0*/ static Texture *_bss_0[2];
/*0x8*/ static BSS8 _bss_8[2];
/*0x38*/ static s32 _bss_38;
/*0x3C*/ static u8 _bss_3C[4];
/*0x40*/ static u8 _bss_40[0x4c0];

// offset: 0x0 | ctor
void dll_707_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_707_dtor(void *dll) { }

// offset: 0x18 | func: 0
void dll_707_func_18(s32 arg0) {
    s32 sp4;

    switch (arg0) {
    case 0:
    case 1:
        sp4 = 0x18;
        break;
    case 2:
        sp4 = 0x2C;
        break;
    }
    _data_8->hitSpheres[arg0].unk2 = (s16) sp4;
}

// offset: 0x84 | func: 1
static void dll_707_func_84(s32 arg0) {
    _data_8->hitSpheres[arg0].unk2 = 0;
}

// offset: 0xB8 | func: 2
static Object* dll_707_func_B8(Object *arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    FXEmit_Setup *setup;
    Object *fxEmit;

    setup = obj_alloc_create_info(sizeof(FXEmit_Setup), OBJ_FXEmit);
    setup->base.loadDistance = 0xFF;
    setup->base.fadeDistance = 0xFF;
    setup->base.loadFlags = OBJSETUP_LOAD_FLAG2;
    setup->base.fadeFlags = OBJSETUP_FADE_DISABLE;
    setup->base.x = arg1;
    setup->base.y = arg2;
    setup->base.z = arg3;
    setup->toggleGamebit = -1;
    setup->disableGamebit = -1;
    setup->yaw = 0;
    setup->flagConfig = 1;
    setup->pitch = 0;
    setup->roll = 0;
    setup->rollSpeed = 0;
    setup->pitchSpeed = 0;
    setup->yawSpeed = 0;
    setup->activationRange = 0;
    setup->bank = 0;
    setup->indexInBank = (s16) arg4;
    setup->fxRate = 1;
    setup->interval = 0;
    fxEmit = obj_create((ObjSetup*)setup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, -1, -1, NULL);
    if (fxEmit != NULL) {
        fxEmit->unkC4 = arg0;
    }
    return fxEmit;
}

// offset: 0x1B4 | func: 3
void dll_707_func_1B4(Object *arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16 arg5, f32 arg6, s32 arg7) {
    ObjSetup *setup;
    Object *projectile;

    setup = obj_alloc_create_info(0x24, arg7); // KamerianFlame/KamerianAcid
    setup->x = arg1;
    setup->y = arg2;
    setup->z = arg3;
    setup->loadFlags = OBJSETUP_LOAD_FLAG1;
    setup->fadeFlags = OBJSETUP_FADE_DISABLE;
    setup->loadDistance = 0xFF;
    setup->fadeDistance = 0xFF;
    projectile = obj_create(setup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, -1, -1, NULL);
    if (projectile != NULL) {
        projectile->srt.pitch = arg5;
        projectile->srt.yaw = arg4;
        projectile->speed.x = fcos16(arg5) * fsin16(arg4) * arg6;
        projectile->speed.y = fsin16(arg5) * arg6;
        projectile->speed.z = fcos16(arg5) * fcos16(arg4) * arg6;
        projectile->unkC4 = arg0;
    }
}

// offset: 0x320 | func: 4 | export: 0
void dll_707_setup(Object *self, KamerianBoss_Setup *setup, s32 arg2) {
    s32 i;
    Texture *texture;
    s32 var_s0_2;
    u32 temp_v0;
    KamerianBoss_Data *objdata;

    self->animCallback = NULL;
    objdata = self->data;
    self->srt.yaw = setup->yaw << 8;
    func_80023D30(self, 2, 0.0f, 0);
    bzero(objdata, sizeof(KamerianBoss_Data));
    objdata->unk0_15 = 0xA;
    objdata->unk28 = 0.0f;
    func_8002674C(self);
    _data_8 = (*self->modelInsts)->model;
    _bss_38 = 0;

    // init hitspheres
    dll_707_func_84(2);
    dll_707_func_84(0);
    dll_707_func_84(1);

    for (i = 0; i < 2; i++) {
        texture = queue_load_texture_proxy(_data_C[i]);
        _bss_0[i] = texture;
        _bss_8[i].texture = texture;
        _bss_8[i].unk4 = 0;
        _bss_8[i].unk8 = 0;
        _bss_8[i].unkA = 0;
        _bss_8[i].unkC = 0;
    }

    // load fxemit objects
    i = 1;
    do {
        var_s0_2 = i != 0 ? 163 : -163;        
        objdata->unk8[i] = dll_707_func_B8(
            self,
            var_s0_2 + self->positionMirror.x,
            self->positionMirror.y + 175.0f,
            self->positionMirror.z + 145.0f,
            0x691
        );
        var_s0_2 = i--;
    } while (var_s0_2 != 0);
}

// offset: 0x558 | func: 5
#ifndef NON_MATCHING
s16 dll_707_func_558(f32 a0, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, s32 a8);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/707_KamerianBoss/dll_707_func_558.s")
#else
s16 dll_707_func_558(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8) {
    f32 sp6C;
    f32 temp_fa0;
    f32 temp_fs3;
    f32 temp_fs5;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fa1;
    f32 temp;
    s32 var_s1;
    s32 var_s2;
    s16 sp7C[2];

    var_s2 = 0;
    temp_fv0 = arg0 - arg3;
    temp_fv1 = arg2 - arg5;
    temp_fs5 = arg4 - arg1;
    temp_fv0 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1));
    sp6C = ((temp_fs5 * arg7) + (arg6 * arg6));
    temp_fv1 = SQ(sp6C);
    temp_fa0 = SQ(arg7) * (SQ(temp_fv0) + SQ(temp_fs5));
    temp = temp_fv1 - temp_fa0;
    if (temp_fa0 <= temp_fv1) {
        var_s1 = 2;
        temp_fs3 = sqrtf(temp);
        while (var_s1--) {
            temp = ((SQ(temp_fs5) / (SQ(temp_fv0))) + 1.0f);
            var_fa1 = (((var_s1 != 0) ? temp_fs3 : -temp_fs3) + sp6C) / (2.0f * temp);
            if (var_fa1 >= 0.0f) {
                var_fa1 = sqrtf(var_fa1);
                if (temp_fv0 < 0.0f) {
                    var_fa1 = -var_fa1;
                }
                sp7C[var_s2] = arctan2_f(((temp_fs5 / temp_fv0) * var_fa1) - ((arg7 * temp_fv0) / (2.0f * var_fa1)), var_fa1);
                var_s2 += 1;
            }
        }
    }

    switch (var_s2) {
        case 2:
            if (sp7C[0] < sp7C[1]) {
                return (arg8 != 0) ? sp7C[1] : sp7C[0];
            } else {
                return (arg8 != 0) ? sp7C[0] : sp7C[1];
            }
            break;
        case 1:
            return sp7C[0];
        default:
            return 0x2000;
    }
}
#endif

// offset: 0x7C0 | func: 6
#ifndef NON_MATCHING
void dll_707_func_7C0(Object *self, KamerianBoss_Data *objdata);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/707_KamerianBoss/dll_707_func_7C0.s")
#else
// needs dll_707_func_558 to be static

void dll_707_func_7C0(Object *self, KamerianBoss_Data *objdata) {
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    s32 pad;
    s16 sp42;
    s16 temp_t7;

    sp5C = objdata->unkF0;
    sp58 = objdata->unkF4;
    sp54 = objdata->unkF8;
    if (!(objdata->player->srt.transl.y < (objdata->unkFC - 20.0f))) {
        func_80014D34(0.5f, &sp50, &sp4C, &sp48);
        sp4C += 20.0f;
        sp42 = atan2f_to_s(sp50 - sp5C, sp48 - sp54);
        temp_t7 = dll_707_func_558(sp5C, sp58, sp54, sp50, sp4C, sp48, 7.8f, -0.046f, 0);
        if ((self->curModAnimId == 9) && (self->animProgress >= 0.18f) && !(objdata->unk0_14)) {
            dll_707_func_1B4(self, sp5C, sp58, sp54, sp42, temp_t7, 7.8f, 0x4E3);
            if (1) {} // @fake
            objdata->unk0_14 = 1;
        }
        if ((s32) objdata->unk34 >= (s32) gUpdateRate) {
            objdata->unk34 -= gUpdateRate;
            return;
        }
        objdata->unk34 = rand_next(270, 372);
        if ((self->curModAnimId != 9) || (objdata->unk28 == 0.0f)) {
            func_80023D30(self, 9, 0.0f, 0);
            objdata->unk28 = 0.0097f;
            objdata->unk0_14  = 0;
        }
    }
}
#endif

// offset: 0xA0C | func: 7
#ifndef NON_MATCHING
void dll_707_func_A0C(Object *arg0, KamerianBoss_Data *arg1, s32 arg2, u16 *arg3);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/707_KamerianBoss/dll_707_func_A0C.s")
#else
// needs dll_707_func_558 to be static

void dll_707_func_A0C(Object *arg0, KamerianBoss_Data *arg1, s32 arg2, u16 *arg3) {
    Object *player;
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    s32 pad;
    s32 pad2;
    f32 temp_fv0_2;
    f32 temp_fv1_2;
    s16 sp52;
    s16 temp2;
    s32 var_v0;

    player = arg1->player;
    
    var_v0 = ((arg2 != 0) ? 163 : -163);
    if (1){} // @fake
    sp80 = (f32) var_v0 + arg0->positionMirror.x;
    sp7C = arg0->positionMirror.z + 145.0f;
    
    var_v0 = ((arg2 != 0) ? 163 : -163);
    sp78 = (f32) var_v0 + arg0->positionMirror.x;
    sp74 = arg0->positionMirror.y + 175.0f;
    sp70 = arg0->positionMirror.z + 145.0f;
    
    if ((s32) *arg3 >= (s32) gUpdateRate) {
        *arg3 -= gUpdateRate;
        return;
    }
    
    *arg3 = rand_next(90, 270);
    if (1){} // @fake
    
    temp_fv0_2 = player->positionMirror.x - sp80;
    temp_fv1_2 = player->positionMirror.z - sp7C;
    if (sqrtf(SQ(temp_fv0_2) + SQ(temp_fv1_2)) < 500.0f) {
        func_80014D34(1.2f, &sp6C, &sp68, &sp64);
        sp68 += 20.0f;
    } else {
        if (arg2 != 0) {
            sp52 = 10000;
        } else {
            sp52 = -10000;
        }
        sp6C = (fsin16(sp52) * 400.0f) + sp80;
        sp68 = player->positionMirror.y;
        sp64 = (fcos16(sp52) * 400.0f) + sp7C;
    }
    sp52 = atan2f_to_s(sp6C - sp78, sp64 - sp70);
    temp2 = dll_707_func_558(sp78, sp74, sp70, sp6C, sp68, sp64, 8.5f, -0.22f, 1);
    dll_707_func_1B4(arg0, sp78, sp74, sp70, sp52, temp2, 8.5f, 0x4DF);
}
#endif

// offset: 0xCBC | func: 8
void dll_707_func_CBC(Object *arg0, s32 arg1) {
    KamerianBoss_Data* objdata;
    TextureAnimator* temp_v0;
    f32 temp_fv0;
    s32 i;
    s32 var_v1;

    objdata = arg0->data;
    for (i = 0; i < 5; i++) {
        temp_v0 = func_800348A0(arg0, i + 5, 0);
        if (temp_v0 != NULL) {
            temp_fv0 = 570.0f - (f32) objdata->unk36;
            if (temp_fv0 < 0.0f) {
                temp_v0->frame = 0xFF - (s32) (temp_fv0 * -2.0f);
            } else {
                var_v1 = objdata->unk36 - (i * 0x32);
                if (var_v1 < 0) {
                    var_v1 = 0;
                } else if (var_v1 >= 0x100) {
                    var_v1 = 0xFF;
                }
                temp_v0->frame = var_v1;
            }
        }
        temp_v0 = func_800348A0(arg0, i, 0);
        if (temp_v0 != NULL) {
            temp_fv0 = 570.0f - (f32) objdata->unk38;
            if (temp_fv0 < 0.0f) {
                temp_v0->frame = 0xFF - (s32) (temp_fv0 * -2.0f);
            } else {
                var_v1 = objdata->unk38 - (i * 0x32);
                if (var_v1 < 0) {
                    var_v1 = 0;
                } else if (var_v1 >= 0x100) {
                    var_v1 = 0xFF;
                }
                temp_v0->frame = var_v1;
            }
        }
    }
}

// offset: 0xE94 | func: 9
void dll_707_func_E94(Object *arg0, s32 arg1) {
    KamerianBoss_Data* objdata;
    Object* temp_t0;
    s32 sp44;
    s32 sp40;
    f32 temp_fv0;
    s32 temp_v1;
    f32 f18;
    f32 f12;
    f32 f2;
    f32 f14;
    f32 f10;
    s32 var_a2;
    f32 f16;

    objdata = arg0->data;
    temp_t0 = objdata->unk10[arg1];
    if (arg1 & 1) {
        var_a2 = 0;
    } else {
        var_a2 = 6;
    }
    temp_v1 = rand_next(0, 4) + var_a2;
    sp40 = temp_v1 + 1;
    sp44 = temp_v1;
    temp_fv0 = (f32) rand_next(0, 1000) * 0.001f;
    f2 = objdata->unk48[sp44].x;
    f12 = objdata->unk48[sp44].y;
    f18 = objdata->unk48[sp44].z;
    f14 = objdata->unk48[sp40].x;
    f16 = objdata->unk48[sp40].y;
    f10 = objdata->unk48[sp40].z;
    temp_t0->srt.transl.x = f2 + ((f14 - f2) * temp_fv0);
    temp_t0->srt.transl.y = f12 + ((f16 - f12) * temp_fv0);
    temp_t0->srt.transl.z = f18 + ((f10 - f18) * temp_fv0);
}

// offset: 0xFC4 | func: 10 | export: 1
#ifndef NON_MATCHING
/*0x10*/ static u16 _data_10[] = {0x0002, 0x0000, 0x0000};
/*0x18*/ static u16 _data_18[] = {0x0018, 0x0014, 0x0008};
void dll_707_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/707_KamerianBoss/dll_707_control.s")
#else
// matches, needs called funcs to be static

void dll_707_control(Object *self) {
    ObjectHitInfo *temp_t6_3;
    s32 var_s0;
    s32 i;
    s32 j;
    s32 temp_a0_6;
    s32 temp_v1_2;
    s32 var_a0;
    s32 var_v1;
    KamerianBoss_Data *objdata;

    objdata = self->data;
    if (objdata->player == NULL) {
        objdata->player = get_player();
        objdata->unkFC = objdata->player->srt.transl.y;
    }
    if (objdata->player != NULL) {
        ((DLL_210_Player*)objdata->player->dll)->vtbl->add_magic(objdata->player, 10);
        if (objdata->unk36) {
            if (gUpdateRate < objdata->unk36) {
                objdata->unk36 -= gUpdateRate;
                for (i = 0; i < 6; i += 2) {
                    if (objdata->unk36 > 120.0f) {
                        dll_707_func_E94(self, i);
                    } else {
                        objdata->unk10[i]->srt.transl.x = 0.0f;
                        objdata->unk10[i]->srt.transl.y = 0.0f;
                        objdata->unk10[i]->srt.transl.z = 0.0f;
                    }
                }
            } else {
                objdata->unk36 = 0;
                for (i = 0; i < 6; i += 2) {
                    obj_destroy_object(objdata->unk10[i]);
                    objdata->unk10[i] = NULL;
                }
            }
        }
        if (objdata->unk38) {
            if (gUpdateRate < objdata->unk38) {
                objdata->unk38 -= gUpdateRate;
                for (i = 1; i < 7; i += 2) {
                    if (objdata->unk38 > 120.0f) {
                        dll_707_func_E94(self, i);
                    } else {
                        objdata->unk10[i]->srt.transl.x = 0.0f;
                        objdata->unk10[i]->srt.transl.y = 0.0f;
                        objdata->unk10[i]->srt.transl.z = 0.0f;
                    }
                };
            } else {
                objdata->unk38 = 0;
                for (i = 1; i < 7; i += 2) {
                    obj_destroy_object(objdata->unk10[i]);
                    objdata->unk10[i] = NULL;
                }
            }
        }
        dll_707_func_CBC(self, gUpdateRate);
        objdata->unk0_8 = func_80024108(self, objdata->unk28, gUpdateRateF, NULL);
        if (objdata->unk2C != 0) {
            if (objdata->unk2C < 15000) {
                objdata->unk2C += gUpdateRate * 50;
            }
            func_80034804(self, 4)[7] = objdata->unk2C;
        }
        if (objdata->unk2E != 0) {
            if (objdata->unk2E < 15000) {
                objdata->unk2E += gUpdateRate * 50;
            }
            func_80034804(self, 3)[7] = objdata->unk2E;
        }
        // Useless assignment of v1? required to match
        var_v1 = 0;
        if (objdata->unk0_9 != 0) {
            var_v1 |= 1;
        }
        if (objdata->unk0_10 != 0) {
            var_v1 |= 2;
        }
        if (objdata->unk0_13 != 0) {
            var_v1 |= 4;
        }
        if (((objdata->unk0_8) != 0) && (objdata->unk28 != 0.0f)) {
            switch (self->curModAnimId) {
            case 2:
            case 3:
                dll_707_func_18(0);
                break;
            case 0:
            case 1:
                dll_707_func_18(1);
                break;
            case 6:
            case 7:
                objdata->unk0_9 = 1;
                var_a0 = 1;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AE, MAX_VOLUME, NULL, NULL, 0, NULL);
                if (objdata->soundHandle1 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle1);
                    objdata->soundHandle1 = 0;
                }
                break;
            case 4:
            case 5:
                objdata->unk0_10 = 1;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AE, MAX_VOLUME, NULL, NULL, 0, NULL);
                if (objdata->soundHandle2 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle2);
                    objdata->soundHandle2 = 0;
                }
                break;
            case 8:
                objdata->unk0_13 = 1;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AE, MAX_VOLUME, NULL, NULL, 0, NULL);
                if (objdata->soundHandle3 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle3);
                    objdata->soundHandle3 = 0;
                }
                break;
            }
            objdata->unk28 = 0.0f;
        }
        var_s0 = 0;
        if (objdata->unk0_9 != 0) {
            var_s0 |= 1;
        }
        if (objdata->unk0_10 != 0) {
            var_s0 |= 2;
        }
        if (objdata->unk0_13 != 0) {
            var_s0 |= 4;
        }
        if ((var_s0 == 3) && (self->curModAnimId < 8)) {
            func_80023D30(self, 8, 0.0f, 0);
            objdata->unk28 = 0.01f;
            _bss_38 = gUpdateRate;
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AD, MAX_VOLUME, &objdata->soundHandle3, NULL, 0, NULL);
        }
        func_800269CC(self, self->objhitInfo, _data_0[var_s0]);
        i = self->objhitInfo->unk62;
        while (i--) {
            temp_a0_6 = self->objhitInfo->unk63[i];
            temp_v1_2 = self->objhitInfo->unk66[i];
            if (objdata->unk28 == 0.0f) {
                switch (temp_a0_6) {
                case 13:
                case 14:
                    if (objdata->unk36 == 0) {
                        objdata->unk36 = 600;
                        for (j = 0; j < 6; j += 2) {
                            objdata->unk10[j] = dll_707_func_B8(self, self->positionMirror.x - 163.0f, self->positionMirror.y + 175.0f, self->positionMirror.z + 145.0f, 0x693);
                        }
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AA, MAX_VOLUME, NULL, NULL, 0, NULL);
                    } else if ((temp_v1_2 == 0xF) && (objdata->unk36 > 50)) {
                        dll_707_func_84(13);
                        dll_707_func_84(14);
                        func_80023D30(self, objdata->unk0_10 ? 3 : 2, 0.0f, 0);
                        objdata->unk28 = 0.005f;
                        objdata->unk2C = 1;
                        objdata->unk0_11 = 1;
                        obj_destroy_object(objdata->unk8[0]);
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AB, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                    break;
                case 8:
                case 9:
                    if (objdata->unk38 == 0) {
                        objdata->unk38 = 600;
                        for (j = 1; j < 7; j += 2) {
                            objdata->unk10[j] = dll_707_func_B8(self, self->positionMirror.x + 163.0f, self->positionMirror.y + 175.0f, self->positionMirror.z + 145.0f, 0x693);
                        }
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AA, MAX_VOLUME, NULL, NULL, 0, NULL);
                    } else if ((temp_v1_2 == 0xF) && (objdata->unk38 > 50)) {
                        dll_707_func_84(8);
                        dll_707_func_84(9);
                        func_80023D30(self, objdata->unk0_9 ? 1 : 0, 0.0f, 0);
                        objdata->unk28 = 0.005f;
                        objdata->unk2E = 1;
                        objdata->unk0_12 = 1;
                        obj_destroy_object(objdata->unk8[1]);
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AB, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                    break;
                case 0:
                    if (temp_v1_2 == 0xF) {
                        if ((objdata->unk0_12) && (objdata->unk0_11)) {
                            dll_707_func_84(0);
                            func_80023D30(self, objdata->unk0_10 ? 7 : 6, 0.0f, 0);
                            objdata->unk28 = 0.005f;
                            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AC, MAX_VOLUME, NULL, NULL, 0, NULL);
                            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AD, MAX_VOLUME, &objdata->soundHandle1, NULL, 0, NULL);
                        }
                    }
                    break;
                case 1:
                    if (temp_v1_2 == 0xF) {
                        if ((objdata->unk0_12) && (objdata->unk0_11)) {
                            dll_707_func_84(1);
                            func_80023D30(self, objdata->unk0_9 ? 5 : 4, 0.0f, 0);
                            objdata->unk28 = 0.005f;
                            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AC, MAX_VOLUME, NULL, NULL, 0, NULL);
                            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AD, MAX_VOLUME, &objdata->soundHandle2, NULL, 0, NULL);
                        }
                    }
                    /* fallthrough */
                default:
                    objdata->unk0_15--;
                    break;
                }
            }
        }
        if (objdata->unk0_13 != 0) {
            dll_707_func_7C0(self, objdata);
        }
        if ((objdata->unk0_11) && !(objdata->unk0_9)) {
            dll_707_func_A0C(self, objdata, 0, &objdata->unk30);
        }
        if ((objdata->unk0_12) && !(objdata->unk0_10)) {
            dll_707_func_A0C(self, objdata, 1, &objdata->unk32);
        }
        if ((_bss_38 != 0) && (_bss_38 < 0xFF)) {
            _bss_38 += gUpdateRate;
            if (_bss_38 > 0xFF) {
                _bss_38 = 0xFF;
            }
        }
        if ((objdata->unk0_13) && (objdata->player != NULL)) {
            if (!(objdata->unk0_23)) {
                u16 sp8C[3] = {0x0002, 0x0000, 0x0000}; // _data_10
                u16 sp84[3] = {0x0018, 0x0014, 0x0008}; // _data_18
                create_temp_dll(53);
                ((DLL_53*)gTempDLLInsts[1])->vtbl->func2(self, _bss_40, -18000, 9800, 3);
                ((DLL_53*)gTempDLLInsts[1])->vtbl->func6(_bss_40, &sp84, &sp84, 3);
                _bss_40[0x4A9] |= 8;
                objdata->unk0_23 = 1;
            }
            ((DLL_53*)gTempDLLInsts[1])->vtbl->func1(_bss_40, objdata->player);
            ((DLL_53*)gTempDLLInsts[1])->vtbl->func0(self, _bss_40);
        }
    }
}
#endif

// offset: 0x1E14 | func: 11 | export: 2
void dll_707_update(Object *self) { }

// offset: 0x1E20 | func: 12 | export: 3
void dll_707_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    KamerianBoss_Data *objdata;
    s32 temp_s0;
    s32 i;

    objdata = self->data;
    temp_s0 = (objdata->unk0_15 / 10) << 5;
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if (_bss_38 != 0) {
            func_800390A4(gdl, &_bss_8[0], 96.0f, 
                24.0f, (f32) temp_s0, 10.0f, 0, 0, 4.0f, 1.0f, _bss_38 - 0x100, 0x4002);
            
            func_800390A4(gdl, &_bss_8[1], (f32) ((temp_s0 * 4) + 0x60), 
                24.0f, (f32) (0x20 - temp_s0), 10.0f, temp_s0 << 5, 0, 4.0f, 1.0f, _bss_38 - 0x100, 0x4002);
        }
        i = 15;
        while(i--) {
            func_80031F6C(self, i, &objdata->unk48[i].x, &objdata->unk48[i].y, &objdata->unk48[i].z, 0);
        }
        if (objdata->unk0_23) {
            ((DLL_53*)gTempDLLInsts[1])->vtbl->func3(self, _bss_40, 2);
        }
    }
}

// offset: 0x2068 | func: 13 | export: 4
void dll_707_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_4);
    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }
}

// offset: 0x20D8 | func: 14 | export: 5
u32 dll_707_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_8;
}

// offset: 0x20E8 | func: 15 | export: 6
u32 dll_707_get_data_size(Object *self, u32 a1) {
    return sizeof(KamerianBoss_Data);
}

/*0x0*/ static const char str_0[] = "health:%d\n";
/*0xC*/ static const char str_C[] = "initialising neck stuff\n";
