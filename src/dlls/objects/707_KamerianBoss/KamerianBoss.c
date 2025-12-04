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
/*00:0*/ u32 pad0_0 : 8;
/*00:8*/ u32 animFinished : 1;
/*00:9*/ u32 rightWingOpened : 1;
/*00:10*/ u32 leftWingOpened : 1;
/*00:11*/ u32 rightPipeDetached: 1;
/*00:12*/ u32 leftPipeDetached: 1;
/*00:13*/ u32 hatchOpened: 1;
/*00:14*/ u32 flameDebounce : 1;
/*00:15*/ u32 health : 8;
/*00:23*/ u32 loadedTempDLL : 1;
/*00:24*/ u32 pad0_24: 8;
/*04*/ Object *player;
/*08*/ Object *unk8[2];
/*10*/ Object *unk10[6];
/*28*/ f32 animTickDelta; // anim progress per tick (60hz)
/*2C*/ s16 rightPipeYOffset;
/*2E*/ s16 leftPipeYOffset;
/*30*/ u16 rightAcidAttackTimer;
/*32*/ u16 leftAcidAttackTimer;
/*34*/ s16 flameAttackTimer;
/*36*/ u16 rightPipeTimer;
/*38*/ u16 leftPipeTimer;
/*3C*/ u32 soundHandle1;
/*40*/ u32 soundHandle2;
/*44*/ u32 soundHandle3;
/*48*/ Vec3f attachmentPositions[15];
/*FC*/ f32 playerStartY;
} KamerianBoss_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18[0x2A-0x18];
/*2A*/ s8 yaw;
} KamerianBoss_Setup;

enum KDModAnims {
    KD_MODANIM_DETATCH_LEFT_PIPE = 0,
    KD_MODANIM_DETATCH_LEFT_PIPE_ALT = 1, // With right wing open
    KD_MODANIM_DETATCH_RIGHT_PIPE = 2,
    KD_MODANIM_DETATCH_RIGHT_PIPE_ALT = 3, // With left wing open
    KD_MODANIM_OPEN_LEFT_WING = 4,
    KD_MODANIM_OPEN_LEFT_WING_ALT = 5, // With right wing open
    KD_MODANIM_OPEN_RIGHT_WING = 6,
    KD_MODANIM_OPEN_RIGHT_WING_ALT = 7, // With left wing open
    KD_MODANIM_OPEN_HATCH = 8,
    KD_MODANIM_ATTACK = 9,
    KD_MODANIM_HURT = 10,
    KD_MODANIM_MELT = 11
};

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
/*0x8*/ static Model *sModel = NULL;
/*0xC*/ static s16 sHealthBarTextureIDs[2] = {0x042d, 0x0422};

/*0x0*/ static Texture *sHealthBarTextures[2];
/*0x8*/ static BSS8 _bss_8[2];
/*0x38*/ static s32 sHealthBarAlpha;
/*0x3C*/ static u8 _bss_3C[4];
/*0x40*/ static u8 _bss_40[0x4c0];

// offset: 0x0 | ctor
void KamerianBoss_ctor(void *dll) { }

// offset: 0xC | dtor
void KamerianBoss_dtor(void *dll) { }

// offset: 0x18 | func: 0
void KamerianBoss_enable_hit_sphere(s32 hitSphereIdx) {
    s32 sp4;

    switch (hitSphereIdx) {
    case 0:
    case 1:
        sp4 = 24;
        break;
    case 2:
        sp4 = 44;
        break;
    }
    sModel->hitSpheres[hitSphereIdx].unk2 = (s16) sp4;
}

// offset: 0x84 | func: 1
static void KamerianBoss_disable_hit_sphere(s32 hitSphereIdx) {
    sModel->hitSpheres[hitSphereIdx].unk2 = 0;
}

// offset: 0xB8 | func: 2
static Object* KamerianBoss_create_fx_emit(Object *self, f32 x, f32 y, f32 z, s32 arg4) {
    FXEmit_Setup *setup;
    Object *fxEmit;

    setup = obj_alloc_create_info(sizeof(FXEmit_Setup), OBJ_FXEmit);
    setup->base.loadDistance = 0xFF;
    setup->base.fadeDistance = 0xFF;
    setup->base.loadFlags = OBJSETUP_LOAD_FLAG2;
    setup->base.fadeFlags = OBJSETUP_FADE_DISABLE;
    setup->base.x = x;
    setup->base.y = y;
    setup->base.z = z;
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
        fxEmit->unkC4 = self;
    }
    return fxEmit;
}

// offset: 0x1B4 | func: 3
void KamerianBoss_create_projectile(Object *self, f32 x, f32 y, f32 z, s16 arg4, s16 arg5, f32 arg6, s32 objID) {
    ObjSetup *setup;
    Object *projectile;

    setup = obj_alloc_create_info(0x24, objID); // KamerianFlame/KamerianAcid
    setup->x = x;
    setup->y = y;
    setup->z = z;
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
        projectile->unkC4 = self;
    }
}

// offset: 0x320 | func: 4 | export: 0
void KamerianBoss_setup(Object *self, KamerianBoss_Setup *setup, s32 arg2) {
    s32 i;
    Texture *texture;
    s32 var_s0_2;
    u32 temp_v0;
    KamerianBoss_Data *objdata;

    self->animCallback = NULL;
    objdata = self->data;
    self->srt.yaw = setup->yaw << 8;
    func_80023D30(self, KD_MODANIM_DETATCH_RIGHT_PIPE, 0.0f, 0);
    bzero(objdata, sizeof(KamerianBoss_Data));
    objdata->health = 10;
    objdata->animTickDelta = 0.0f;
    func_8002674C(self);
    sModel = self->modelInsts[0]->model;
    sHealthBarAlpha = 0;

    // init hitspheres
    KamerianBoss_disable_hit_sphere(2);
    KamerianBoss_disable_hit_sphere(0);
    KamerianBoss_disable_hit_sphere(1);

    for (i = 0; i < 2; i++) {
        texture = queue_load_texture_proxy(sHealthBarTextureIDs[i]);
        sHealthBarTextures[i] = texture;
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
        objdata->unk8[i] = KamerianBoss_create_fx_emit(
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
s16 KamerianBoss_func_558(f32 a0, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, s32 a8);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/707_KamerianBoss/KamerianBoss_func_558.s")
#else
s16 KamerianBoss_func_558(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, s32 arg8) {
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
void KamerianBoss_do_flame_attack(Object *self, KamerianBoss_Data *objdata);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/707_KamerianBoss/KamerianBoss_do_flame_attack.s")
#else
// needs KamerianBoss_func_558 to be static

void KamerianBoss_do_flame_attack(Object *self, KamerianBoss_Data *objdata) {
    f32 headX;
    f32 headY;
    f32 headZ;
    f32 playerX;
    f32 playerY;
    f32 playerZ;
    s32 pad;
    s16 sp42;
    s16 temp_t7;

    headX = objdata->attachmentPositions[14].x;
    headY = objdata->attachmentPositions[14].y;
    headZ = objdata->attachmentPositions[14].z;
    if (!(objdata->player->srt.transl.y < (objdata->playerStartY - 20.0f))) {
        func_80014D34(0.5f, &playerX, &playerY, &playerZ);
        playerY += 20.0f;
        sp42 = atan2f_to_s(playerX - headX, playerZ - headZ);
        temp_t7 = KamerianBoss_func_558(headX, headY, headZ, playerX, playerY, playerZ, 7.8f, -0.046f, 0);
        if ((self->curModAnimId == KD_MODANIM_ATTACK) && (self->animProgress >= 0.18f) && !(objdata->flameDebounce)) {
            KamerianBoss_create_projectile(self, headX, headY, headZ, sp42, temp_t7, 7.8f, OBJ_KamerianFlame);
            if (1) {} // @fake
            objdata->flameDebounce = 1;
        }
        if ((s32) objdata->flameAttackTimer >= (s32) gUpdateRate) {
            objdata->flameAttackTimer -= gUpdateRate;
            return;
        }
        objdata->flameAttackTimer = rand_next(270, 372);
        if ((self->curModAnimId != KD_MODANIM_ATTACK) || (objdata->animTickDelta == 0.0f)) {
            func_80023D30(self, KD_MODANIM_ATTACK, 0.0f, 0);
            objdata->animTickDelta = 0.0097f;
            objdata->flameDebounce = 0;
        }
    }
}
#endif

// offset: 0xA0C | func: 7
#ifndef NON_MATCHING
void KamerianBoss_do_acid_attack(Object *arg0, KamerianBoss_Data *arg1, s32 arg2, u16 *arg3);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/707_KamerianBoss/KamerianBoss_do_acid_attack.s")
#else
// needs KamerianBoss_func_558 to be static

void KamerianBoss_do_acid_attack(Object *self, KamerianBoss_Data *objdata, s32 side, u16 *timer) {
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

    player = objdata->player;
    
    var_v0 = ((side != 0) ? 163 : -163);
    if (1){} // @fake
    sp80 = (f32) var_v0 + self->positionMirror.x;
    sp7C = self->positionMirror.z + 145.0f;
    
    var_v0 = ((side != 0) ? 163 : -163);
    sp78 = (f32) var_v0 + self->positionMirror.x;
    sp74 = self->positionMirror.y + 175.0f;
    sp70 = self->positionMirror.z + 145.0f;
    
    if ((s32) *timer >= (s32) gUpdateRate) {
        *timer -= gUpdateRate;
        return;
    }
    
    *timer = rand_next(90, 270);
    if (1){} // @fake
    
    temp_fv0_2 = player->positionMirror.x - sp80;
    temp_fv1_2 = player->positionMirror.z - sp7C;
    if (sqrtf(SQ(temp_fv0_2) + SQ(temp_fv1_2)) < 500.0f) {
        func_80014D34(1.2f, &sp6C, &sp68, &sp64);
        sp68 += 20.0f;
    } else {
        if (side != 0) {
            sp52 = 10000;
        } else {
            sp52 = -10000;
        }
        sp6C = (fsin16(sp52) * 400.0f) + sp80;
        sp68 = player->positionMirror.y;
        sp64 = (fcos16(sp52) * 400.0f) + sp7C;
    }
    sp52 = atan2f_to_s(sp6C - sp78, sp64 - sp70);
    temp2 = KamerianBoss_func_558(sp78, sp74, sp70, sp6C, sp68, sp64, 8.5f, -0.22f, 1);
    KamerianBoss_create_projectile(self, sp78, sp74, sp70, sp52, temp2, 8.5f, OBJ_KamerianAcid);
}
#endif

// offset: 0xCBC | func: 8
void KamerianBoss_do_pipe_texture_anim(Object *self, s32 updateRate) {
    KamerianBoss_Data* objdata;
    TextureAnimator* texAnimator;
    f32 temp_fv0;
    s32 i;
    s32 frame;

    objdata = self->data;
    for (i = 0; i < 5; i++) {
        texAnimator = func_800348A0(self, i + 5, 0);
        if (texAnimator != NULL) {
            temp_fv0 = 570.0f - (f32) objdata->rightPipeTimer;
            if (temp_fv0 < 0.0f) {
                texAnimator->frame = 0xFF - (s32) (temp_fv0 * -2.0f);
            } else {
                frame = objdata->rightPipeTimer - (i * 0x32);
                if (frame < 0) {
                    frame = 0;
                } else if (frame > 0xFF) {
                    frame = 0xFF;
                }
                texAnimator->frame = frame;
            }
        }
        texAnimator = func_800348A0(self, i, 0);
        if (texAnimator != NULL) {
            temp_fv0 = 570.0f - (f32) objdata->leftPipeTimer;
            if (temp_fv0 < 0.0f) {
                texAnimator->frame = 0xFF - (s32) (temp_fv0 * -2.0f);
            } else {
                frame = objdata->leftPipeTimer - (i * 0x32);
                if (frame < 0) {
                    frame = 0;
                } else if (frame > 0xFF) {
                    frame = 0xFF;
                }
                texAnimator->frame = frame;
            }
        }
    }
}

// offset: 0xE94 | func: 9
void KamerianBoss_func_E94(Object *self, s32 arg1) {
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

    objdata = self->data;
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
    f2 = objdata->attachmentPositions[sp44].x;
    f12 = objdata->attachmentPositions[sp44].y;
    f18 = objdata->attachmentPositions[sp44].z;
    f14 = objdata->attachmentPositions[sp40].x;
    f16 = objdata->attachmentPositions[sp40].y;
    f10 = objdata->attachmentPositions[sp40].z;
    temp_t0->srt.transl.x = f2 + ((f14 - f2) * temp_fv0);
    temp_t0->srt.transl.y = f12 + ((f16 - f12) * temp_fv0);
    temp_t0->srt.transl.z = f18 + ((f10 - f18) * temp_fv0);
}

// offset: 0xFC4 | func: 10 | export: 1
#ifndef NON_MATCHING
/*0x10*/ static u16 _data_10[] = {0x0002, 0x0000, 0x0000};
/*0x18*/ static u16 _data_18[] = {0x0018, 0x0014, 0x0008};
void KamerianBoss_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/707_KamerianBoss/KamerianBoss_control.s")
#else
// matches, needs called funcs to be static

void KamerianBoss_control(Object *self) {
    ObjectHitInfo *temp_t6_3;
    s32 var_s0;
    s32 i;
    s32 j;
    s32 hitSphereIdx;
    s32 collisionType;
    s32 var_a0;
    s32 var_v1;
    KamerianBoss_Data *objdata;

    objdata = self->data;
    if (objdata->player == NULL) {
        objdata->player = get_player();
        objdata->playerStartY = objdata->player->srt.transl.y;
    }
    if (objdata->player != NULL) {
        ((DLL_210_Player*)objdata->player->dll)->vtbl->add_magic(objdata->player, 10);
        if (objdata->rightPipeTimer) {
            if (gUpdateRate < objdata->rightPipeTimer) {
                objdata->rightPipeTimer -= gUpdateRate;
                for (i = 0; i < 6; i += 2) {
                    if (objdata->rightPipeTimer > 120.0f) {
                        KamerianBoss_func_E94(self, i);
                    } else {
                        objdata->unk10[i]->srt.transl.x = 0.0f;
                        objdata->unk10[i]->srt.transl.y = 0.0f;
                        objdata->unk10[i]->srt.transl.z = 0.0f;
                    }
                }
            } else {
                objdata->rightPipeTimer = 0;
                for (i = 0; i < 6; i += 2) {
                    obj_destroy_object(objdata->unk10[i]);
                    objdata->unk10[i] = NULL;
                }
            }
        }
        if (objdata->leftPipeTimer) {
            if (gUpdateRate < objdata->leftPipeTimer) {
                objdata->leftPipeTimer -= gUpdateRate;
                for (i = 1; i < 7; i += 2) {
                    if (objdata->leftPipeTimer > 120.0f) {
                        KamerianBoss_func_E94(self, i);
                    } else {
                        objdata->unk10[i]->srt.transl.x = 0.0f;
                        objdata->unk10[i]->srt.transl.y = 0.0f;
                        objdata->unk10[i]->srt.transl.z = 0.0f;
                    }
                };
            } else {
                objdata->leftPipeTimer = 0;
                for (i = 1; i < 7; i += 2) {
                    obj_destroy_object(objdata->unk10[i]);
                    objdata->unk10[i] = NULL;
                }
            }
        }
        KamerianBoss_do_pipe_texture_anim(self, gUpdateRate);
        objdata->animFinished = func_80024108(self, objdata->animTickDelta, gUpdateRateF, NULL);
        if (objdata->rightPipeYOffset != 0) {
            if (objdata->rightPipeYOffset < 15000) {
                objdata->rightPipeYOffset += gUpdateRate * 50;
            }
            func_80034804(self, 4)[7] = objdata->rightPipeYOffset;
        }
        if (objdata->leftPipeYOffset != 0) {
            if (objdata->leftPipeYOffset < 15000) {
                objdata->leftPipeYOffset += gUpdateRate * 50;
            }
            func_80034804(self, 3)[7] = objdata->leftPipeYOffset;
        }
        // Useless assignment of v1? required to match
        var_v1 = 0;
        if (objdata->rightWingOpened != 0) {
            var_v1 |= 1;
        }
        if (objdata->leftWingOpened != 0) {
            var_v1 |= 2;
        }
        if (objdata->hatchOpened != 0) {
            var_v1 |= 4;
        }
        if ((objdata->animFinished != 0) && (objdata->animTickDelta != 0.0f)) {
            switch (self->curModAnimId) {
            case KD_MODANIM_DETATCH_RIGHT_PIPE:
            case KD_MODANIM_DETATCH_RIGHT_PIPE_ALT:
                KamerianBoss_enable_hit_sphere(0);
                break;
            case KD_MODANIM_DETATCH_LEFT_PIPE:
            case KD_MODANIM_DETATCH_LEFT_PIPE_ALT:
                KamerianBoss_enable_hit_sphere(1);
                break;
            case KD_MODANIM_OPEN_RIGHT_WING:
            case KD_MODANIM_OPEN_RIGHT_WING_ALT:
                objdata->rightWingOpened = TRUE;
                var_a0 = 1;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AE, MAX_VOLUME, NULL, NULL, 0, NULL);
                if (objdata->soundHandle1 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle1);
                    objdata->soundHandle1 = 0;
                }
                break;
            case KD_MODANIM_OPEN_LEFT_WING:
            case KD_MODANIM_OPEN_LEFT_WING_ALT:
                objdata->leftWingOpened = TRUE;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AE, MAX_VOLUME, NULL, NULL, 0, NULL);
                if (objdata->soundHandle2 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle2);
                    objdata->soundHandle2 = 0;
                }
                break;
            case KD_MODANIM_OPEN_HATCH:
                objdata->hatchOpened = TRUE;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AE, MAX_VOLUME, NULL, NULL, 0, NULL);
                if (objdata->soundHandle3 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle3);
                    objdata->soundHandle3 = 0;
                }
                break;
            }
            objdata->animTickDelta = 0.0f;
        }
        var_s0 = 0;
        if (objdata->rightWingOpened != 0) {
            var_s0 |= 1;
        }
        if (objdata->leftWingOpened != 0) {
            var_s0 |= 2;
        }
        if (objdata->hatchOpened != 0) {
            var_s0 |= 4;
        }
        if ((var_s0 == 3) && (self->curModAnimId < KD_MODANIM_OPEN_HATCH)) {
            func_80023D30(self, KD_MODANIM_OPEN_HATCH, 0.0f, 0);
            objdata->animTickDelta = 0.01f;
            sHealthBarAlpha = gUpdateRate;
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AD, MAX_VOLUME, &objdata->soundHandle3, NULL, 0, NULL);
        }
        func_800269CC(self, self->objhitInfo, _data_0[var_s0]);
        i = self->objhitInfo->unk62;
        while (i--) {
            hitSphereIdx = self->objhitInfo->unk63[i];
            collisionType = self->objhitInfo->unk66[i];
            if (objdata->animTickDelta == 0.0f) {
                switch (hitSphereIdx) {
                case 13:
                case 14:
                    if (objdata->rightPipeTimer == 0) {
                        objdata->rightPipeTimer = 600;
                        for (j = 0; j < 6; j += 2) {
                            objdata->unk10[j] = KamerianBoss_create_fx_emit(self, self->positionMirror.x - 163.0f, self->positionMirror.y + 175.0f, self->positionMirror.z + 145.0f, 0x693);
                        }
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AA, MAX_VOLUME, NULL, NULL, 0, NULL);
                    } else if ((collisionType == 0xF) && (objdata->rightPipeTimer > 50)) {
                        KamerianBoss_disable_hit_sphere(13);
                        KamerianBoss_disable_hit_sphere(14);
                        func_80023D30(self, 
                            objdata->leftWingOpened ? KD_MODANIM_DETATCH_RIGHT_PIPE_ALT : KD_MODANIM_DETATCH_RIGHT_PIPE, 
                            0.0f, 0);
                        objdata->animTickDelta = 0.005f;
                        objdata->rightPipeYOffset = 1;
                        objdata->rightPipeDetached = TRUE;
                        obj_destroy_object(objdata->unk8[0]);
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AB, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                    break;
                case 8:
                case 9:
                    if (objdata->leftPipeTimer == 0) {
                        objdata->leftPipeTimer = 600;
                        for (j = 1; j < 7; j += 2) {
                            objdata->unk10[j] = KamerianBoss_create_fx_emit(self, self->positionMirror.x + 163.0f, self->positionMirror.y + 175.0f, self->positionMirror.z + 145.0f, 0x693);
                        }
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AA, MAX_VOLUME, NULL, NULL, 0, NULL);
                    } else if ((collisionType == 0xF) && (objdata->leftPipeTimer > 50)) {
                        KamerianBoss_disable_hit_sphere(8);
                        KamerianBoss_disable_hit_sphere(9);
                        func_80023D30(self, 
                            objdata->rightWingOpened ? KD_MODANIM_DETATCH_LEFT_PIPE_ALT : KD_MODANIM_DETATCH_LEFT_PIPE, 
                            0.0f, 0);
                        objdata->animTickDelta = 0.005f;
                        objdata->leftPipeYOffset = 1;
                        objdata->leftPipeDetached = TRUE;
                        obj_destroy_object(objdata->unk8[1]);
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AB, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                    break;
                case 0:
                    if (collisionType == 0xF) {
                        if ((objdata->leftPipeDetached) && (objdata->rightPipeDetached)) {
                            KamerianBoss_disable_hit_sphere(0);
                            func_80023D30(self, 
                                objdata->leftWingOpened ? KD_MODANIM_OPEN_RIGHT_WING_ALT : KD_MODANIM_OPEN_RIGHT_WING, 
                                0.0f, 0);
                            objdata->animTickDelta = 0.005f;
                            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AC, MAX_VOLUME, NULL, NULL, 0, NULL);
                            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AD, MAX_VOLUME, &objdata->soundHandle1, NULL, 0, NULL);
                        }
                    }
                    break;
                case 1:
                    if (collisionType == 0xF) {
                        if ((objdata->leftPipeDetached) && (objdata->rightPipeDetached)) {
                            KamerianBoss_disable_hit_sphere(1);
                            func_80023D30(self, 
                                objdata->rightWingOpened ? KD_MODANIM_OPEN_LEFT_WING_ALT : KD_MODANIM_OPEN_LEFT_WING, 
                                0.0f, 0);
                            objdata->animTickDelta = 0.005f;
                            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AC, MAX_VOLUME, NULL, NULL, 0, NULL);
                            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9AD, MAX_VOLUME, &objdata->soundHandle2, NULL, 0, NULL);
                        }
                    }
                    /* fallthrough */
                default:
                    objdata->health--;
                    break;
                }
            }
        }
        if (objdata->hatchOpened != 0) {
            KamerianBoss_do_flame_attack(self, objdata);
        }
        if ((objdata->rightPipeDetached) && !(objdata->rightWingOpened)) {
            KamerianBoss_do_acid_attack(self, objdata, 0, &objdata->rightAcidAttackTimer);
        }
        if ((objdata->leftPipeDetached) && !(objdata->leftWingOpened)) {
            KamerianBoss_do_acid_attack(self, objdata, 1, &objdata->leftAcidAttackTimer);
        }
        if ((sHealthBarAlpha != 0) && (sHealthBarAlpha < 0xFF)) {
            sHealthBarAlpha += gUpdateRate;
            if (sHealthBarAlpha > 0xFF) {
                sHealthBarAlpha = 0xFF;
            }
        }
        if ((objdata->hatchOpened) && (objdata->player != NULL)) {
            if (!objdata->loadedTempDLL) {
                u16 sp8C[3] = {0x0002, 0x0000, 0x0000}; // _data_10
                u16 sp84[3] = {0x0018, 0x0014, 0x0008}; // _data_18
                create_temp_dll(53);
                ((DLL_53*)gTempDLLInsts[1])->vtbl->func2(self, _bss_40, -18000, 9800, 3);
                ((DLL_53*)gTempDLLInsts[1])->vtbl->func6(_bss_40, &sp84, &sp84, 3);
                _bss_40[0x4A9] |= 8;
                objdata->loadedTempDLL = TRUE;
            }
            ((DLL_53*)gTempDLLInsts[1])->vtbl->func1(_bss_40, objdata->player);
            ((DLL_53*)gTempDLLInsts[1])->vtbl->func0(self, _bss_40);
        }
    }
}
#endif

// offset: 0x1E14 | func: 11 | export: 2
void KamerianBoss_update(Object *self) { }

// offset: 0x1E20 | func: 12 | export: 3
void KamerianBoss_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    KamerianBoss_Data *objdata;
    s32 hpBarWidth;
    s32 i;

    objdata = self->data;
    hpBarWidth = (objdata->health / 10) * 32;
    if ((visibility != 0) && (self->unkDC == 0)) {
        // Draw self
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        // Draw health bar
        if (sHealthBarAlpha != 0) {
            func_800390A4(gdl, &_bss_8[0], 
                /*x*/96.0f, 
                /*y*/24.0f, 
                /*width*/(f32) hpBarWidth, 
                /*height*/10.0f, 
                /*s*/0, 
                /*t*/0, 
                /*xScale*/4.0f, 
                /*yScale*/1.0f, 
                /*color*/sHealthBarAlpha - 256, 
                /*flags*/0x4002);
            
            func_800390A4(gdl, &_bss_8[1], 
                /*x*/(f32) ((hpBarWidth * 4) + 96), 
                /*y*/24.0f, 
                /*width*/(f32) (32 - hpBarWidth), 
                /*height*/10.0f, 
                /*s*/hpBarWidth * 32, 
                /*t*/0, 
                /*xScale*/4.0f, 
                /*yScale*/1.0f, 
                /*color*/sHealthBarAlpha - 256, 
                /*flags*/0x4002);
        }
        // Get attachment positions
        i = 15;
        while(i--) {
            func_80031F6C(self, 
                i, 
                &objdata->attachmentPositions[i].x, 
                &objdata->attachmentPositions[i].y, 
                &objdata->attachmentPositions[i].z, 
                0);
        }
        // Head lookat
        if (objdata->loadedTempDLL) {
            ((DLL_53*)gTempDLLInsts[1])->vtbl->func3(self, _bss_40, 2);
        }
    }
}

// offset: 0x2068 | func: 13 | export: 4
void KamerianBoss_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_4);
    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }
}

// offset: 0x20D8 | func: 14 | export: 5
u32 KamerianBoss_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_8;
}

// offset: 0x20E8 | func: 15 | export: 6
u32 KamerianBoss_get_data_size(Object *self, u32 a1) {
    return sizeof(KamerianBoss_Data);
}

/*0x0*/ static const char str_0[] = "health:%d\n";
/*0xC*/ static const char str_C[] = "initialising neck stuff\n";
