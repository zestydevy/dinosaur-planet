#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/90_camstatic.h"
#include "dlls/objects/715_StaticCamera.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objtype.h"
#include "dll.h"

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
} BSS0;

/*0x0*/ static BSS0* bss_0;

static void dll_90_func_5D4(CamControl_Data* arg0, Vec3f* arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5);
static s32 dll_90_func_798(CamControl_Data* arg0, u8 arg1);
static Object* dll_90_func_C04(f32 x, f32 y, f32 z, s32 arg3, s32 controlNo);

// offset: 0x0 | ctor
void dll_90_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_90_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_90_func_18(CamControl_Data* camData, s32 arg1, CamStatic_Params* action) {
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

    temp_s0 = camData->player;
    bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, ALLOC_NAME("camstatic"));
    bss_0->unkF4 = 1;
    bss_0->unkF5 = 0;
    sp4C = dll_90_func_C04(temp_s0->srt.transl.x, temp_s0->srt.transl.y, temp_s0->srt.transl.z, action->unk0, 0x12);
    if (sp4C == NULL) {
        bss_0->unkF5 = 1;
        return;
    }
    bss_0->unk0 = sp4C;
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
    if (action->unk4 == 0) {
        dll_90_func_5D4(camData, &sp4C->globalPosition, sp36, var_a3, var_v0, var_fv0);
        return;
    }
    camData->srt.transl.x = sp4C->globalPosition.x;
    camData->srt.transl.y = sp4C->globalPosition.y;
    camData->srt.transl.z = sp4C->globalPosition.z;
    camData->srt.yaw = sp36;
    camData->srt.pitch = var_a3;
    camData->srt.roll = var_v0;
    camData->fov = var_fv0;
}

// offset: 0x278 | func: 1 | export: 1
void dll_90_func_278(CamControl_Data* camData) {
    DLL715_Setup* sp4C;
    s32 var_v0;
    s32 var_v1;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    Object* sp34;
    s32 temp_v0;

    if (bss_0->unkF5 != 0) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, 0, 1, 0, NULL, 0, 0xFF);
        return;
    }
    sp34 = camData->player;
    sp4C = (DLL715_Setup*)bss_0->unk0->setup;
    if (!(sp4C->unk1B & 1)) {
        camData->srt.yaw = sp4C->unk1C + 0x8000;
    }
    if (!(sp4C->unk1B & 2)) {
        camData->srt.pitch = sp4C->unk1E;
    }
    if (!(sp4C->unk1B & 4)) {
        camData->srt.roll = sp4C->unk20;
    }
    camData->srt.transl.x = bss_0->unk0->globalPosition.x;
    camData->srt.transl.y = bss_0->unk0->globalPosition.y;
    camData->srt.transl.z = bss_0->unk0->globalPosition.z;
    camData->fov = (f32) sp4C->unk1A;
    if (bss_0->unkF4 == 0) {
        temp_v0 = dll_90_func_798(camData, sp4C->unk1B);
        if (temp_v0 != 0) {
            bss_0->unkF4 = 1;
        }
    }
    sp40 = camData->srt.transl.x - sp34->globalPosition.x;
    sp3C = camData->srt.transl.y - sp34->globalPosition.y;
    sp38 = camData->srt.transl.z - sp34->globalPosition.z;
    if (sp4C->unk1B & 1) {
        camData->srt.yaw = 0x8000 - arctan2_f(sp40, sp38);
    }
    if (sp4C->unk1B & 2) {
        var_v1 = (arctan2_f(sp3C, sqrtf(SQ(sp40) + SQ(sp38))) - sp4C->unk1E) - (camData->srt.pitch & 0xFFFF);
        CIRCLE_WRAP(var_v1);
        camData->srt.pitch += ((s32) (var_v1 * gUpdateRate) >> 3);
    }
    if (sp4C->unk1B & 4) {
        var_v0 = camData->srt.roll - (sp34->srt.roll & 0xFFFF);
        CIRCLE_WRAP(var_v0);
        camData->srt.roll += ((s32) (var_v0 * gUpdateRate) >> 3);
    }
}

// offset: 0x584 | func: 2 | export: 2
void dll_90_func_584(CamControl_Data* camData) {
    mmFree(bss_0);
}

// offset: 0x5C4 | func: 3 | export: 3
void dll_90_func_5C4(CamStatic_Params* arg0, s32 arg1) {

}

// offset: 0x5D4 | func: 4
static void dll_90_func_5D4(CamControl_Data* arg0, Vec3f* arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;

    bss_0->unkF4 = 0;
    bss_0->unk8 = arg0->srt.transl.x;
    bss_0->unk10 = arg0->srt.transl.y;
    bss_0->unk18 = arg0->srt.transl.z;
    bss_0->unk20 = (f32) arg0->srt.yaw;
    bss_0->unk28 = (f32) arg0->srt.pitch;
    bss_0->unk30 = (f32) arg0->srt.roll;
    bss_0->unk38 = arg0->fov;
    bss_0->unkC = arg1->x;
    bss_0->unk14 = arg1->y;
    bss_0->unk1C = arg1->z;
    bss_0->unk24 = (f32) arg2;
    bss_0->unk2C = (f32) arg3;
    bss_0->unk34 = (f32) arg4;
    bss_0->unk3C = arg5;
    bss_0->unk50 = 0.0f;
    temp_fv0 = bss_0->unkC - bss_0->unk8;
    temp_fv1 = bss_0->unk14 - bss_0->unk10;
    temp_fa1 = bss_0->unk1C - bss_0->unk18;
    bss_0->unk54 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa1));
    gDLL_2_Camera->vtbl->func12(bss_0->unk54, &bss_0->unk40, 100.0f, 0.1f, 0.1f, -5.0f);
}

// offset: 0x798 | func: 5
static s32 dll_90_func_798(CamControl_Data* arg0, u8 arg1) {
    f32 var_fs0;
    f32 var_fv1;

    bss_0->unkC = arg0->srt.transl.x;
    bss_0->unk14 = arg0->srt.transl.y;
    bss_0->unk1C = arg0->srt.transl.z;
    bss_0->unk24 = (f32) arg0->srt.yaw;
    bss_0->unk2C = (f32) arg0->srt.pitch;
    bss_0->unk34 = (f32) arg0->srt.roll;
    bss_0->unk3C = arg0->fov;
    var_fs0 = bss_0->unk50 / bss_0->unk54;
    if (var_fs0 > 1.0f) {
        var_fs0 = 1.0f;
    }
    var_fv1 = curves_hermite(&bss_0->unk40.x, var_fs0, NULL);
    if (var_fv1 < 0.2f) {
        var_fv1 = 0.2f;
    }
    bss_0->unk50 += var_fv1 * gUpdateRateF;
    var_fs0 = bss_0->unk50 / bss_0->unk54;
    if (var_fs0 > 1.0f) {
        var_fs0 = 1.0f;
    }
    arg0->srt.transl.x = curves_linear(&bss_0->unk8, var_fs0, NULL);
    arg0->srt.transl.y = curves_linear(&bss_0->unk10, var_fs0, NULL);
    arg0->srt.transl.z = curves_linear(&bss_0->unk18, var_fs0, NULL);
    if (((bss_0->unk20 - bss_0->unk24) > 32768.0f) || ((bss_0->unk20 - bss_0->unk24) < -32768.0f)) {
        if (bss_0->unk20 < 0.0f) {
            bss_0->unk20 += 65535.0f;
        } else if (bss_0->unk24 < 0.0f) {
            bss_0->unk24 += 65535.0f;
        }
    }
    if (((bss_0->unk28 - bss_0->unk2C) > 32768.0f) || ((bss_0->unk28 - bss_0->unk2C) < -32768.0f)) {
        if (bss_0->unk28 < 0.0f) {
            bss_0->unk28 += 65535.0f;
        } else if (bss_0->unk2C < 0.0f) {
            bss_0->unk2C += 65535.0f;
        }
    }
    if (((bss_0->unk30 - bss_0->unk34) > 32768.0f) || ((bss_0->unk30 - bss_0->unk34) < -32768.0f)) {
        if (bss_0->unk30 < 0.0f) {
            bss_0->unk30 += 65535.0f;
        } else if (bss_0->unk34 < 0.0f) {
            bss_0->unk34 += 65535.0f;
        }
    }
    if (!(arg1 & 1)) {
        arg0->srt.yaw = (s16) curves_linear(&bss_0->unk20, var_fs0, NULL);
    }
    if (!(arg1 & 2)) {
        arg0->srt.pitch = (s16) curves_linear(&bss_0->unk28, var_fs0, NULL);
    }
    if (!(arg1 & 4)) {
        arg0->srt.roll = (s16) curves_linear(&bss_0->unk30, var_fs0, NULL);
    }
    return var_fs0 >= 1.0f;
}

// offset: 0xC04 | func: 6
static Object* dll_90_func_C04(f32 x, f32 y, f32 z, s32 arg3, s32 controlNo) {
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
