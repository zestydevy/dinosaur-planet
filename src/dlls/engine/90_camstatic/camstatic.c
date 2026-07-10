#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/90_camstatic.h"
#include "dlls/objects/715_StaticCamera.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objtype.h"
#include "dll.h"

// Positions camera at the location of a StaticCamera object

typedef struct {
    Object* unk0;
    u8 _unk4[0x8 - 0x4];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    Vec4f unk40;
    f32 unk50;
    f32 unk54;
    u8 _unk58[0xF4 - 0x58];
    u8 unkF4;
    u8 unkF5;
} CamStatic;

/*0x0*/ static CamStatic* sState;

static void camstatic_func_5D4(Cam* cam, Vec3f* arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5);
static s32 camstatic_func_798(Cam* cam, u8 arg1);
static Object* camstatic_func_C04(f32 x, f32 y, f32 z, s32 arg3, s32 controlNo);

// offset: 0x0 | ctor
void camstatic_ctor(void* dll) { }

// offset: 0xC | dtor
void camstatic_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void camstatic_func_18(Cam* cam, s32 arg1, CamStatic_Params* data) {
    Object* sp4C;
    DLL715_Setup* sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    Object* temp_s0;
    s16 sp36;
    s16 var_a3;
    s16 var_v0;
    f32 var_fv0;

    temp_s0 = cam->player;
    sState = mmAlloc(sizeof(CamStatic), ALLOC_TAG_CAM_COL, ALLOC_NAME("camstatic"));
    sState->unkF4 = 1;
    sState->unkF5 = 0;
    sp4C = camstatic_func_C04(temp_s0->srt.transl.x, temp_s0->srt.transl.y, temp_s0->srt.transl.z, data->unk0, 0x12);
    if (sp4C == NULL) {
        sState->unkF5 = 1;
        return;
    }
    sState->unk0 = sp4C;
    sp48 = (DLL715_Setup*)sp4C->setup;
    sp44 = sp4C->globalPosition.x - temp_s0->globalPosition.x;
    sp40 = sp4C->globalPosition.y - temp_s0->globalPosition.y;
    sp3C = sp4C->globalPosition.z - temp_s0->globalPosition.z;
    if (sp48->unk1B & 1) {
        sp36 = 0x8000 - arctan2_f(sp44, sp3C);
    } else {
        sp36 = sp48->unk1C + 0x8000;
    }
    if (sp48->unk1B & 2) {
        var_a3 = (arctan2_f(sp40, sqrtf(SQ(sp44) + SQ(sp3C))) & 0xFFFF & 0xFFFF) - sp48->unk1E;
    } else {
        var_a3 = sp48->unk1E;
    }
    if (sp48->unk1B & 4) {
        var_v0 = temp_s0->srt.roll;
    } else {
        var_v0 = sp48->unk20;
    }
    var_fv0 = (f32) sp48->unk1A;
    if (data->unk4 == 0) {
        camstatic_func_5D4(cam, &sp4C->globalPosition, sp36, var_a3, var_v0, var_fv0);
        return;
    }
    cam->srt.transl.x = sp4C->globalPosition.x;
    cam->srt.transl.y = sp4C->globalPosition.y;
    cam->srt.transl.z = sp4C->globalPosition.z;
    cam->srt.yaw = sp36;
    cam->srt.pitch = var_a3;
    cam->srt.roll = var_v0;
    cam->fov = var_fv0;
}

// offset: 0x278 | func: 1 | export: 1
void camstatic_func_278(Cam* cam) {
    DLL715_Setup* sp4C;
    s32 var_v0;
    s32 var_v1;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    Object* sp34;
    s32 temp_v0;

    if (sState->unkF5 != 0) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
        return;
    }
    sp34 = cam->player;
    sp4C = (DLL715_Setup*)sState->unk0->setup;
    if (!(sp4C->unk1B & 1)) {
        cam->srt.yaw = sp4C->unk1C + 0x8000;
    }
    if (!(sp4C->unk1B & 2)) {
        cam->srt.pitch = sp4C->unk1E;
    }
    if (!(sp4C->unk1B & 4)) {
        cam->srt.roll = sp4C->unk20;
    }
    cam->srt.transl.x = sState->unk0->globalPosition.x;
    cam->srt.transl.y = sState->unk0->globalPosition.y;
    cam->srt.transl.z = sState->unk0->globalPosition.z;
    cam->fov = (f32) sp4C->unk1A;
    if (sState->unkF4 == 0) {
        temp_v0 = camstatic_func_798(cam, sp4C->unk1B);
        if (temp_v0 != 0) {
            sState->unkF4 = 1;
        }
    }
    sp40 = cam->srt.transl.x - sp34->globalPosition.x;
    sp3C = cam->srt.transl.y - sp34->globalPosition.y;
    sp38 = cam->srt.transl.z - sp34->globalPosition.z;
    if (sp4C->unk1B & 1) {
        cam->srt.yaw = 0x8000 - arctan2_f(sp40, sp38);
    }
    if (sp4C->unk1B & 2) {
        var_v1 = (arctan2_f(sp3C, sqrtf(SQ(sp40) + SQ(sp38))) - sp4C->unk1E) - (cam->srt.pitch & 0xFFFF);
        CIRCLE_WRAP(var_v1);
        cam->srt.pitch += ((s32) (var_v1 * gUpdateRate) >> 3);
    }
    if (sp4C->unk1B & 4) {
        var_v0 = cam->srt.roll - (sp34->srt.roll & 0xFFFF);
        CIRCLE_WRAP(var_v0);
        cam->srt.roll += ((s32) (var_v0 * gUpdateRate) >> 3);
    }
}

// offset: 0x584 | func: 2 | export: 2
void camstatic_func_584(Cam* cam) {
    mmFree(sState);
}

// offset: 0x5C4 | func: 3 | export: 3
void camstatic_func_5C4(void* arg0, s32 arg1) {

}

// offset: 0x5D4 | func: 4
static void camstatic_func_5D4(Cam* cam, Vec3f* arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;

    sState->unkF4 = 0;
    sState->unk8 = cam->srt.transl.x;
    sState->unk10 = cam->srt.transl.y;
    sState->unk18 = cam->srt.transl.z;
    sState->unk20 = (f32) cam->srt.yaw;
    sState->unk28 = (f32) cam->srt.pitch;
    sState->unk30 = (f32) cam->srt.roll;
    sState->unk38 = cam->fov;
    sState->unkC = arg1->x;
    sState->unk14 = arg1->y;
    sState->unk1C = arg1->z;
    sState->unk24 = (f32) arg2;
    sState->unk2C = (f32) arg3;
    sState->unk34 = (f32) arg4;
    sState->unk3C = arg5;
    sState->unk50 = 0.0f;
    temp_fv0 = sState->unkC - sState->unk8;
    temp_fv1 = sState->unk14 - sState->unk10;
    temp_fa1 = sState->unk1C - sState->unk18;
    sState->unk54 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa1));
    gDLL_2_Camera->vtbl->func12(sState->unk54, &sState->unk40, 100.0f, 0.1f, 0.1f, -5.0f);
}

// offset: 0x798 | func: 5
static s32 camstatic_func_798(Cam* cam, u8 arg1) {
    f32 var_fs0;
    f32 var_fv1;

    sState->unkC = cam->srt.transl.x;
    sState->unk14 = cam->srt.transl.y;
    sState->unk1C = cam->srt.transl.z;
    sState->unk24 = (f32) cam->srt.yaw;
    sState->unk2C = (f32) cam->srt.pitch;
    sState->unk34 = (f32) cam->srt.roll;
    sState->unk3C = cam->fov;
    var_fs0 = sState->unk50 / sState->unk54;
    if (var_fs0 > 1.0f) {
        var_fs0 = 1.0f;
    }
    var_fv1 = curvesHermite(&sState->unk40.x, var_fs0, NULL);
    if (var_fv1 < 0.2f) {
        var_fv1 = 0.2f;
    }
    sState->unk50 += var_fv1 * gUpdateRateF;
    var_fs0 = sState->unk50 / sState->unk54;
    if (var_fs0 > 1.0f) {
        var_fs0 = 1.0f;
    }
    cam->srt.transl.x = curvesLinear(&sState->unk8, var_fs0, NULL);
    cam->srt.transl.y = curvesLinear(&sState->unk10, var_fs0, NULL);
    cam->srt.transl.z = curvesLinear(&sState->unk18, var_fs0, NULL);
    if (((sState->unk20 - sState->unk24) > 32768.0f) || ((sState->unk20 - sState->unk24) < -32768.0f)) {
        if (sState->unk20 < 0.0f) {
            sState->unk20 += 65535.0f;
        } else if (sState->unk24 < 0.0f) {
            sState->unk24 += 65535.0f;
        }
    }
    if (((sState->unk28 - sState->unk2C) > 32768.0f) || ((sState->unk28 - sState->unk2C) < -32768.0f)) {
        if (sState->unk28 < 0.0f) {
            sState->unk28 += 65535.0f;
        } else if (sState->unk2C < 0.0f) {
            sState->unk2C += 65535.0f;
        }
    }
    if (((sState->unk30 - sState->unk34) > 32768.0f) || ((sState->unk30 - sState->unk34) < -32768.0f)) {
        if (sState->unk30 < 0.0f) {
            sState->unk30 += 65535.0f;
        } else if (sState->unk34 < 0.0f) {
            sState->unk34 += 65535.0f;
        }
    }
    if (!(arg1 & 1)) {
        cam->srt.yaw = (s16) curvesLinear(&sState->unk20, var_fs0, NULL);
    }
    if (!(arg1 & 2)) {
        cam->srt.pitch = (s16) curvesLinear(&sState->unk28, var_fs0, NULL);
    }
    if (!(arg1 & 4)) {
        cam->srt.roll = (s16) curvesLinear(&sState->unk30, var_fs0, NULL);
    }
    return var_fs0 >= 1.0f;
}

// offset: 0xC04 | func: 6
static Object* camstatic_func_C04(f32 x, f32 y, f32 z, s32 arg3, s32 controlNo) {
    Object** staticCams;
    s32 numObjs;
    Object* closestStaticCam;
    Object* staticCam;
    f32 zDist;
    f32 xDist;
    f32 dist;
    f32 yDist;
    f32 closestDist;
    s32 i;
    DLL715_Setup* staticCamSetup;

    closestStaticCam = NULL;
    closestDist = 100000.0f;
    staticCams = obj_get_all_of_type(OBJTYPE_StaticCamera, &numObjs);
    for (i = 0; i < numObjs; i++) {
        staticCam = staticCams[i];
        if (controlNo == staticCam->controlNo) {
            staticCamSetup = (DLL715_Setup*)staticCam->setup;
            if (arg3 == staticCamSetup->unk18) {
                xDist = x - staticCam->srt.transl.x;
                yDist = y - staticCam->srt.transl.y;
                zDist = z - staticCam->srt.transl.z;
                dist = sqrtf(SQ(xDist) + SQ(yDist) + SQ(zDist));
                if (dist < closestDist) {
                    closestDist = dist;
                    closestStaticCam = staticCam;
                }
            }
        }
    }
    return closestStaticCam;
}
