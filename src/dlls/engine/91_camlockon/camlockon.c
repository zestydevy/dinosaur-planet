#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/91_camlockon.h"
#include "dlls/objects/210_player.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "sys/joypad.h"
#include "sys/memory.h"
#include "dll.h"

// Camera used during Z-lock

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 _unkC[0x10 - 0xC];
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    f32 unk18;
} CamLockOn;

/*0x0*/ static CamLockOn* sState;
/*0x4*/ static s32 bss_4;

static void camlockon_func_BC0(Cam* cam);
static void camlockon_func_C58(Cam* cam, f32 arg1, f32 arg2, f32 arg3);
static void camlockon_func_D34(Cam* cam, f32*, f32*, f32*, f32*);

// offset: 0x0 | ctor
void camlockon_ctor(void* dll) { }

// offset: 0xC | dtor
void camlockon_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void camlockon_func_18(Cam* cam, s32 arg1, CamLockOn_Params* data) {
    Object* target;
    Object* sp20;
    f32 xDist;
    f32 zDist;

    cam->target = data->unk0;
    sp20 = cam->player;
    sState = mmAlloc(sizeof(CamLockOn), ALLOC_TAG_CAM_COL, ALLOC_NAME("camlockon"));
    sState->unk4 = 0.0f;
    sState->unk8 = 1.0f;
    sState->unk12 = 0;
    sState->unk11 = 0;
    sState->unk13 = 1;
    sState->unk14 = 1;
    sState->unk18 = 0.0f;
    if (sp20->controlNo != OBJCONTROL_Player) {
        sState->unk12 = 1;
        return;
    }
    target = cam->target;
    if (target == NULL) {
        sState->unk12 = 1;
        return;
    }
    if (target->unk74 == NULL) {
        xDist = sp20->srt.transl.x - target->globalPosition.x;
        zDist = sp20->srt.transl.z - target->globalPosition.z;
    } else {
        xDist = target->unk74[target->unkD4].refPoint.x - sp20->globalPosition.x;
        zDist = target->unk74[target->unkD4].refPoint.z - sp20->globalPosition.z;
    }
    if (target->controlNo != OBJCONTROL_KTRex) {
        sState->unk0 = sqrtf(SQ(xDist) + SQ(zDist));
    } else {
        sState->unk0 = 200.0f;
    }
    sState->unk10 = 0;
}

// offset: 0x1B8 | func: 1 | export: 1
void camlockon_func_1B8(Cam* cam) {
    Object* sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 temp_fa1;
    f32 sp80;
    f32 temp_ft4;
    f32 sp78;
    f32 var_fv0;
    u16 temp_a0;
    f32 sp6C;
    f32 sp68;
    s32 var_v1;
    ObjDefLockData* lockdata;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 pad_sp50;
    f32 pad_sp4C;
    s32 pad_sp48;
    ObjectStruct74* sp44;
    s32 btns;
    Object* sp3C;
    f32 pad_sp38;
    f32 pad_sp34;

    if (sState->unk12 != 0) {
        camlockon_func_BC0(cam);
        return;
    }
    sp3C = cam->player;
    if ((sp3C->controlNo == OBJCONTROL_Player) && (((DLL_210_Player*)sp3C->dll)->vtbl->func60(sp3C) == 0)) {
        camlockon_func_BC0(cam);
        return;
    }
    sp94 = cam->target;
    if ((sp94 == NULL) || (sp94->stateFlags & 0x40) || (sp94->unkAF & 0x28)) {
        camlockon_func_BC0(cam);
        return;
    }
    sp44 = sp94->unk74;
    if (sp44 == NULL) {
        return;
    }

    lockdata = sp94->def->lockdata;
    sp68 = (f32) (lockdata->lockExitRadius * 4);
    temp_a0 = ((s32) (cam->srt.flags & 0x18) >> 3);
    if (temp_a0 == 1) {
        btns = joyGetPressed(0);
    } else if (temp_a0 == 2) {
        btns = ~joyGetButtons(0);
    } else if (sState->unk10 >= 0xD) {
        btns = joyGetReleased(0);
    } else {
        btns = 0;
        sState->unk10 += gUpdateRate;
    }
    if (!(btns & Z_TRIG) || (camlockon_func_BC0(cam), ((cam->targetFlags & 2) != 0))) {
        sp5C = sp3C->globalPosition.y + 20.0f;
        if ((sp94->controlNo == OBJCONTROL_Baddie) || (sp94->controlNo == OBJCONTROL_KTRex)) {
            var_fv0 = gDLL_33_BaddieControl->vtbl->func8(sp94);
            if (var_fv0 > 20.0f) {
                sp5C = sp3C->globalPosition.y + var_fv0;
            }
            if (sp94->def->numLockdata >= 2) {
                camlockon_func_D34(cam, &sp58, &sp80, &sp54, &sp5C);
            } else {
                sp58 = sp44[sp94->unkD4].refPoint.x - sp3C->globalPosition.x;
                sp80 = sp44[sp94->unkD4].refPoint.y - sp5C;
                sp54 = sp44[sp94->unkD4].refPoint.z - sp3C->globalPosition.z;
            }
        } else {
            sp5C = sp3C->globalPosition.y + 20.0f;
            sp58 = sp44[sp94->unkD4].refPoint.x - sp3C->globalPosition.x;
            sp80 = sp44[sp94->unkD4].refPoint.y - sp5C;
            sp54 = sp44[sp94->unkD4].refPoint.z - sp3C->globalPosition.z;
        }
        sp78 = sqrtf(SQ(sp58) + SQ(sp54));
        cam->letterboxGoal = 24;
        cam->letterboxSpeed = 1;
        if (sp68 < sp78) {
            camlockon_func_BC0(cam);
            return;
        }
        sp90 = sp3C->globalPosition.x + (sp58 * 0.35f);
        sp8C = sp5C + 5.0f;
        sp88 = sp3C->globalPosition.z + (sp54 * 0.35f);
        var_v1 = cam->srt.yaw - (-mathAtan2f(sp58, sp54) & 0xFFFF);
        CIRCLE_WRAP(var_v1);
        if (var_v1 >= 0x2329) {
            cam->srt.yaw -= ((s32) ((var_v1 - 0x2328) * gUpdateRate) / 12);
        } else if (var_v1 < -0x2328) {
            cam->srt.yaw -= ((s32) ((var_v1 + 0x2328) * gUpdateRate) / 12);
        }
        if ((var_v1 < 0xBB8) && (var_v1 > 0)) {
            if ((bss_4 < 0xBB8) && (var_v1 < 0x3E8) && (var_v1 < bss_4)) {
                cam->srt.yaw += ((s32) ((-0xBB8 - var_v1) * gUpdateRate) >> 7);
            } else {
                cam->srt.yaw += ((s32) ((0xBB8 - var_v1) * gUpdateRate) >> 7);
            }
        } else if ((var_v1 >= -0xBB7) && (var_v1 < 0)) {
            if ((bss_4 >= -0xBB7) && (var_v1 >= -0x3E7) && (bss_4 < var_v1)) {
                cam->srt.yaw += ((s32) ((0xBB8 - var_v1) * gUpdateRate) >> 7);
            } else {
                cam->srt.yaw += ((s32) ((-0xBB8 - var_v1) * gUpdateRate) >> 7);
            }
        }
        bss_4 = var_v1;
        if (var_v1 < 0) {
            var_v1 = -var_v1;
        }
        if (var_v1 >= 0x2329) {
            var_v1 = 0x2328;
        }
        sState->unk4 += ((30.0f - sState->unk4) * gUpdateRateF * 0.04f);
        var_v1 = 0x2328 - var_v1;
        sState->unk8 += (((((1.0f - (var_v1 / 9000.0f)) + 0.8f) / 1.8f) - sState->unk8) * gUpdateRateF * 0.1f);
        sp6C = mathSinfInterp(cam->srt.yaw);
        var_fv0 = mathCosfInterp(cam->srt.yaw);
        temp_fa1 = sState->unk0 * sp6C;
        temp_ft4 = sState->unk0 * var_fv0;
        cam->srt.transl.x = sp90 + temp_fa1;
        cam->srt.transl.z = sp88 - temp_ft4;
        sp80 *= 0.6f;
        sp80 = sp5C - sp80;
        sp80 += sState->unk4;
        cam->srt.transl.y -= ((cam->srt.transl.y - sp80) * gUpdateRateF * 0.05f);
        camlockon_func_C58(cam, sp90, cam->srt.transl.y, sp88);
        sp90 = sp3C->globalPosition.x + (sp58 * 0.1f);
        sp88 = sp3C->globalPosition.z + (sp54 * 0.1f);
        temp_fa1 = cam->srt.transl.x - sp90;
        sp80 = cam->srt.transl.y - sp8C;
        temp_ft4 = cam->srt.transl.z - sp88;
        var_fv0 = sqrtf(SQ(temp_fa1) + SQ(temp_ft4));
        var_v1 = mathAtan2f(sp80, var_fv0) - (cam->srt.pitch & 0xFFFF);
        CIRCLE_WRAP(var_v1);
        cam->srt.pitch += (var_v1 * gUpdateRate) >> 3;
        var_fv0 = sp78 + 10.0f;
        if (var_fv0 < 70.0f) {
            var_fv0 = 70.0f;
        }
        if (var_fv0 > 220.0f) {
            var_fv0 = 220.0f;
        }
        var_fv0 -= sState->unk0;
        var_fv0 *= gUpdateRateF * 0.04f;
        if (var_fv0 > 5.0f) {
            var_fv0 = 5.0f;
        } else if (var_fv0 < -5.0f) {
            var_fv0 = -5.0f;
        }
        if ((var_fv0 > -0.2f) && (var_fv0 < 0.2f)) {
            var_fv0 = 0.0f;
        }
        sState->unk0 += var_fv0;
    }
}

// offset: 0xB64 | func: 2 | export: 2
void camlockon_func_B64(Cam* cam) {
    if (cam->target != NULL) {
        cam->target = NULL;
    }
    mmFree(sState);
}

// offset: 0xBB0 | func: 3 | export: 3
void camlockon_func_BB0(CamLockOn_Params* arg0, s32 arg1) {

}

// offset: 0xBC0 | func: 4
static void camlockon_func_BC0(Cam* cam) {
    if (cam->target != NULL) {
        if (!(cam->target->unkAF & ARROW_FLAG_40_Unlock_Disabled) && !(cam->targetFlags & 2)) {
            cam->target = NULL;
        } else {
            return;
        }
    }
    gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, 0, 1, 0, NULL, 0x3C, Cam_Ease_All & ~Cam_Ease_Yaw);
}

// offset: 0xC58 | func: 5
static void camlockon_func_C58(Cam* cam, f32 arg1, f32 arg2, f32 arg3) {
    TrackIntersectResult sp54;
    AABBs32 sp3C;
    Vec3f sp30;

    sp30.x = arg1;
    sp30.y = arg2;
    sp30.z = arg3;
    sp54.unk50[0] = -1;
    sp54.unk54[0] = 4;
    sp54.unk40[0] = 4.5f;
    trackIntersectBuildAABB(&sp3C, &sp30, &cam->srt.transl, &sp54.unk40[0], 1);
    trackIntersectBroadphase(cam->player, &sp3C, 0);
    trackGetIntersect(cam->player, sp30.f, cam->srt.transl.f, 1, &sp54, 0);
}

// offset: 0xD34 | func: 6
static void camlockon_func_D34(Cam* cam, f32* arg1, f32* arg2, f32* arg3, f32* arg4) {
    Object* temp_v0;
    Object* temp_v1;
    ObjectStruct74* temp_t0;
    f32 var_f2;
    f32 var_f14;
    f32 var_f18;

    temp_v0 = cam->target;
    temp_v1 = cam->player;
    temp_t0 = temp_v0->unk74;
    if (temp_v0->unkD4 != sState->unk14) {
        sState->unk13 = sState->unk14;
        sState->unk18 = 1.0f;
    }
    if (sState->unk18 > 0.0f) {
        sState->unk18 -= (0.02f * gUpdateRateF);
        if (sState->unk18 < 0.0f) {
            sState->unk18 = 0.0f;
            sState->unk13 = temp_v0->unkD4;
        }
        var_f2 = temp_t0[sState->unk13].refPoint.x - temp_t0[temp_v0->unkD4].refPoint.x;
        var_f2 *= sState->unk18;
        var_f2 += temp_t0[temp_v0->unkD4].refPoint.x;
        var_f14 = temp_t0[sState->unk13].refPoint.y - temp_t0[temp_v0->unkD4].refPoint.y;
        var_f14 *= sState->unk18;
        var_f14 += temp_t0[temp_v0->unkD4].refPoint.y;
        var_f18 = temp_t0[sState->unk13].refPoint.z - temp_t0[temp_v0->unkD4].refPoint.z;
        var_f18 *= sState->unk18;
        var_f18 += temp_t0[temp_v0->unkD4].refPoint.z;
        *arg1 = var_f2 - temp_v1->globalPosition.x;
        *arg2 = var_f14 - *arg4;
        *arg3 = var_f18 - temp_v1->globalPosition.z;
    } else {
        *arg1 = temp_t0[temp_v0->unkD4].refPoint.x - temp_v1->globalPosition.x;
        *arg2 = temp_t0[temp_v0->unkD4].refPoint.y - *arg4;
        *arg3 = temp_t0[temp_v0->unkD4].refPoint.z - temp_v1->globalPosition.z;
    }
    sState->unk14 = temp_v0->unkD4;
}
