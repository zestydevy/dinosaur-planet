#include "dlls/engine/84_camnormal.h"
#include "dlls/objects/210_player.h"
#include "sys/joypad.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "dll.h"

// This appears to be an alternative (older?) firstperson camera

typedef struct {
    s8 unk0;
} Cam96_Params;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10[4];
    f32 unk20[4];
    f32 unk30[4];
    f32 unk40[4];
    f32 unk50[4];
    u8 _unk60[0x70 - 0x60];
    f32 unk70;
    f32 unk74;
    CurvesStruct unk78;
    f32 unk114;
    f32 unk118;
    u8 unk11C;
} BSS0;

/*0x0*/ static BSS0* bss_0;
/*0x4*/ static s8 bss_4;

static s32 dll_96_func_9C4(CamControl_Data* arg0, Object* arg1);
static void dll_96_func_CE0(CamControl_Data* arg0);
static void dll_96_func_E60(CamControl_Data* arg0);
static void dll_96_func_137C(CamControl_Data* arg0, Object* arg1);

// offset: 0x0 | ctor
void dll_96_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_96_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_96_func_18(CamControl_Data* camData, s32 arg1, void* action) {
    f32 sp6C;
    f32 sp68;
    f32 temp;
    f32 sp60;
    CamControl_Module* camnormal;
    f32 sp58;
    Object* temp_s1;
    f32 sp50;
    s16 var_v1;
    s16 sp4C;
    f32 temp_fv0;
    f32 temp_fv1;

    temp_s1 = camData->player;
    bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, NULL);
    camnormal = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func7(&sp50, &bss_0->unk0, &bss_0->unk4, 0, &bss_0->unk114);
    var_v1 = (-camData->srt.yaw - temp_s1->srt.yaw) + 0x8000;
    if (var_v1 < 0) {
        sp4C = -var_v1;
    } else {
        sp4C = var_v1;
    }
    sp58 = var_v1 / 50.0f;
    temp_fv1 = sp4C / 90.0f;
    bss_0->unk78.unk84 = bss_0->unk10;
    bss_0->unk78.unk88 = bss_0->unk20;
    bss_0->unk78.unk8C = bss_0->unk30;
    bss_0->unk78.numControlPoints = 4;
    bss_0->unk78.unk80 = 0;
    bss_0->unk78.splineFunc = curves_hermite;
    bss_0->unk78.splineConverterFunc = curves_hermite_converter;
    sp6C = camData->srt.transl.x - temp_s1->srt.transl.x;
    sp68 = camData->srt.transl.z - temp_s1->srt.transl.z;
    temp_fv0 = sqrtf(SQ(sp6C) + SQ(sp68));
    sp6C /= temp_fv0;
    sp68 /= temp_fv0;
    sp60 = -fsin16_precise(temp_s1->srt.yaw);
    temp = -fcos16_precise(temp_s1->srt.yaw);
    bss_0->unk10[0] = camData->srt.transl.x;
    bss_0->unk10[1] = temp_s1->srt.transl.x;
    bss_0->unk10[2] = -sp68 * sp58;
    bss_0->unk10[3] = sp60 * temp_fv1;
    bss_0->unk20[0] = camData->srt.transl.y;
    bss_0->unk20[1] = temp_s1->srt.transl.y + 27.0f;
    bss_0->unk20[2] = 0.0f;
    bss_0->unk20[3] = 0.0f;
    bss_0->unk30[0] = camData->srt.transl.z;
    bss_0->unk30[1] = temp_s1->srt.transl.z;
    bss_0->unk30[2] = sp6C * sp58;
    bss_0->unk30[3] = temp * temp_fv1;
    if (1){} // @fake
    bss_0->unk10[2] = 0.0f;
    bss_0->unk10[3] = 0.0f;
    bss_0->unk20[2] = 0.0f;
    bss_0->unk20[3] = 0.0f;
    bss_0->unk30[2] = 0.0f;
    bss_0->unk30[3] = 0.0f;
    bss_0->unk10[1] = bss_0->unk10[1] + (sp60 * 5.0f);
    bss_0->unk30[1] = bss_0->unk30[1] + (temp * 5.0f);
    if (1){} // @fake
    curves_move(&bss_0->unk78);
    var_v1 = 0x8000 - arctan2_f(camData->srt.transl.x - bss_0->unk10[1], camData->srt.transl.z - bss_0->unk30[1]);\
    var_v1 = camData->srt.yaw - (u16)var_v1;\
    CIRCLE_WRAP(var_v1);
    bss_0->unk40[0] = var_v1;
    bss_0->unk40[1] = 0.0f;
    bss_0->unk40[2] = 0.0f;
    bss_0->unk40[3] = 0.0f;
    if (((bss_0->unk40[0] - bss_0->unk40[1]) > 32768.0f) || ((bss_0->unk40[0] - bss_0->unk40[1]) < -32768.0f)) {
        if (bss_0->unk40[0] < 0.0f) {
            bss_0->unk40[0] += 65535.0f;
        } else if (bss_0->unk40[1] < 0.0f) {
            bss_0->unk40[1] += 65535.0f;
        }
    }
    bss_0->unk50[0] = (f32) camData->srt.pitch;
    bss_0->unk50[1] = 0.0f;
    bss_0->unk50[2] = 0.0f;
    bss_0->unk50[3] = 0.0f;
    bss_0->unk11C = 0;
    camData->unk118 = 1;
    bss_4 = 0;
}

// offset: 0x4BC | func: 1 | export: 1
void dll_96_func_4BC(CamControl_Data* camData) {
    Object* temp_v0_4;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fv0;
    s32 var_a0;
    s16 var_v1;
    Object* temp_v0_5;
    CamControl_Module* sp3C;
    s32 temp_v0;

    switch (bss_0->unk11C) {
    case 0:
        bss_0->unk11C = dll_96_func_9C4(camData, camData->player);
        camData->unk118 = 1;
        break;
    case 1:
        if (curves_func_800053B0(&bss_0->unk78, 1000.0f) != 0) {
            bss_0->unk8 = camData->srt.transl.x - camData->player->srt.transl.x;
            bss_0->unkC = camData->srt.transl.z - camData->player->srt.transl.z;
            bss_0->unk11C = 2;
        }
        camData->srt.yaw = (s16) bss_0->unk78.unk68.x;
        camData->unk118 = 1;
        break;
    case 2:
        dll_96_func_CE0(camData);
        if (bss_4 != 0) {
            dll_96_func_E60(camData);
            bss_0->unk11C = 3;
        }
        camData->unk118 = 0;
        break;
    case 3:
        temp_v0 = curves_func_800053B0(&bss_0->unk78, 1000.0f);
        camData->srt.yaw = (s16) bss_0->unk78.unk68.x;
        camData->srt.pitch = (s16) bss_0->unk78.unk68.y;
        if (temp_v0 != 0) {
            dll_96_func_137C(camData, camData->player);
            bss_0->unk11C = 4;
        }
        camData->unk118 = 0;
        break;
    case 4:
        var_fv0 = bss_0->unk78.unk0 - 0.2f;
        if (var_fv0 < 0.0f) {
            var_fv0 = 0.0f;
        }
        var_fv0 *= 1.25f;
        camData->player->opacity = (u8) (s32) (var_fv0 * 255.0f);
        if (curves_func_800053B0(&bss_0->unk78, 5.0f) != 0) {
            gDLL_2_Camera->vtbl->change_camera_module(0x54, 0, 1, 0, NULL, 0, 0xFF);
            camData->player->opacity = 0xFF;
        }
        camData->srt.transl.x = bss_0->unk78.unk68.x;
        camData->srt.transl.z = bss_0->unk78.unk68.z;
        sp3C = gDLL_2_Camera->vtbl->get_camnormal_module();
        ((DLL_84_camnormal*)sp3C->dll)->vtbl->func8(camData, 2, 0, &bss_0->unk70, &bss_0->unk74);
        ((DLL_84_camnormal*)sp3C->dll)->vtbl->func6(camData, camData->player, bss_0->unk70, bss_0->unk74);
        temp_fv0 = camData->srt.transl.x - camData->player->srt.transl.x;
        temp_fv1 = camData->srt.transl.z - camData->player->srt.transl.z;
        temp_fa0 = SQ(temp_fv0) + SQ(temp_fv1);
        if (temp_fa0 < 100.0f) {
            camData->srt.pitch = 0;
        } else {
            ((DLL_84_camnormal*)sp3C->dll)->vtbl->func5(camData, camData->player->srt.transl.y, sqrtf(temp_fa0));
        }
        camData->unk118 = 1;
        break;
    }
    temp_v0_4 = get_player();
    temp_v0_5 = ((DLL_210_Player*)temp_v0_4->dll)->vtbl->get_vehicle(temp_v0_4);
    if (temp_v0_5 != NULL) {
        var_v1 = (camData->srt.yaw & 0xFFFF) - ((-temp_v0_5->srt.yaw) & 0xFFFF);
        CIRCLE_WRAP(var_v1);
        if (var_v1 >= 0) {
            var_a0 = var_v1;
        } else {
            var_a0 = -var_v1;
        }
        if (var_a0 < 0x3FFF) {
            if (var_v1 > 0) {
                var_a0 = 0x3FFF;
            } else {
                var_a0 = -0x3FFF;
            }
            // FAKE ^ 0
            camData->srt.yaw = (var_a0^0) + -temp_v0_5->srt.yaw;
        }
    }
}

// offset: 0x93C | func: 2 | export: 2
void dll_96_func_93C(CamControl_Data* camData) {
    camData->player->srt.flags &= ~OBJFLAG_INVISIBLE;
    mmFree(bss_0);
}

// offset: 0x990 | func: 3 | export: 3
void dll_96_func_990(Cam96_Params* arg0, s32 arg1) {
    if ((arg1 == 1) && (arg0->unk0 == 1)) {
        bss_4 = 1;
    }
}

// offset: 0x9C4 | func: 4
static s32 dll_96_func_9C4(CamControl_Data* arg0, Object* arg1) {
    s32 sp2C;
    s16 temp_a0;
    f32 temp_fv0;
    f32 var_fv1;

    sp2C = curves_func_800053B0(&bss_0->unk78, 5.0f);
    arg0->srt.transl.x = bss_0->unk78.unk68.x;
    arg0->srt.transl.y = bss_0->unk78.unk68.y;
    arg0->srt.transl.z = bss_0->unk78.unk68.z;
    arg0->srt.pitch = (s16) curves_hermite(bss_0->unk50, bss_0->unk78.unk0, NULL);
    temp_a0 = (s16) curves_hermite(bss_0->unk40, bss_0->unk78.unk0, NULL);
    var_fv1 = bss_0->unk78.unk0 - 0.2f;
    if (var_fv1 < 0.0f) {
        var_fv1 = 0.0f;
    }
    var_fv1 *= 1.25f;
    arg0->player->opacity = 0xFF - (s32) (var_fv1 * 255.0f);
    if (sp2C != 0) {
        bss_0->unk78.unk84 = bss_0->unk40;
        bss_0->unk78.unk88 = NULL;
        bss_0->unk78.unk8C = NULL;
        bss_0->unk78.numControlPoints = 4;
        bss_0->unk78.splineFunc = curves_hermite;
        bss_0->unk78.splineConverterFunc = curves_hermite_converter;
        bss_0->unk78.unk80 = 0;
        bss_0->unk40[0] = (f32) arg0->srt.yaw;
        bss_0->unk40[1] = (f32) (s16) (0x8000 - arg1->srt.yaw);
        temp_fv0 = (bss_0->unk40[0] - bss_0->unk40[1]);
        if ((temp_fv0 < 1820.0f) && (temp_fv0 > -1820.0f)) {
            bss_0->unk40[1] = bss_0->unk40[0];
        } else {
            if (((bss_0->unk40[0] - bss_0->unk40[1]) > 32768.0f) || ((bss_0->unk40[0] - bss_0->unk40[1]) < -32768.0f)) {
                if (bss_0->unk40[0] < 0.0f) {
                    bss_0->unk40[0] += 65535.0f;
                } else if (bss_0->unk40[1] < 0.0f) {
                    bss_0->unk40[1] += 65535.0f;
                }
            }
        }
        bss_0->unk40[2] = 0.0f;
        bss_0->unk40[3] = 0.0f;
        curves_move(&bss_0->unk78);
        arg0->player->srt.flags |= OBJFLAG_INVISIBLE;
        return 1;
    }
    arg0->srt.yaw = (temp_a0 - arctan2_f(arg0->srt.transl.x - bss_0->unk10[1], arg0->srt.transl.z - bss_0->unk30[1])) + 0x8000;
    return 0;
}

// offset: 0xCE0 | func: 5
static void dll_96_func_CE0(CamControl_Data* arg0) {
    s8 sp27;
    s8 var_a0;
    Object* temp_v0_2;

    sp27 = joy_get_stick_x(0);
    var_a0 = joy_get_stick_y(0);
    if ((sp27 < 8) && (sp27 > -8)) {
        sp27 = 0;
    }
    if ((var_a0 < 8) && (var_a0 > -8)) {
        var_a0 = 0;
    }
    arg0->srt.yaw += gUpdateRate * sp27 * 5;
    arg0->srt.pitch += gUpdateRate * var_a0 * 5;
    if (arg0->srt.pitch > 0x2000) {
        arg0->srt.pitch = 0x2000;
    }
    if (arg0->srt.pitch < -0x2800) {
        arg0->srt.pitch = -0x2800;
    }
    arg0->player->srt.yaw = 0x8000 - arg0->srt.yaw;
    temp_v0_2 = arg0->player;
    arg0->srt.transl.x = temp_v0_2->srt.transl.x + bss_0->unk8;
    arg0->srt.transl.y = temp_v0_2->srt.transl.y + 27.0f;
    arg0->srt.transl.z = temp_v0_2->srt.transl.z + bss_0->unkC;
}

// offset: 0xE60 | func: 6
static void dll_96_func_E60(CamControl_Data* arg0) {
    f32 temp_ft4;
    f32 spF8;
    f32 temp_fv0;
    f32 spF0;
    f32 spEC;
    f32 spE8;
    Object* player;
    Vec3f spD8;
    Vec3f spCC;
    Unk80027934 sp60;
    AABBs32 sp48;

    player = arg0->player;
    bss_0->unk10[0] = arg0->srt.transl.x;
    bss_0->unk10[2] = 0.0f;
    bss_0->unk10[3] = 0.0f;
    bss_0->unk20[0] = arg0->srt.transl.y;
    bss_0->unk20[2] = 0.0f;
    bss_0->unk20[3] = 0.0f;
    bss_0->unk30[0] = arg0->srt.transl.z;
    bss_0->unk30[2] = 0.0f;
    bss_0->unk30[3] = 0.0f;
    spEC = fsin16_precise(player->srt.yaw);
    spE8 = fcos16_precise(player->srt.yaw);
    temp_fv0 = sqrtf(SQ(bss_0->unk0) - SQ(bss_0->unk4));
    bss_0->unk10[1] = player->globalPosition.x + (spEC * temp_fv0);
    bss_0->unk20[1] = player->globalPosition.y + bss_0->unk114 + bss_0->unk4;
    bss_0->unk30[1] = player->globalPosition.z + (spE8 * temp_fv0);
    spF8 = bss_0->unk10[1] - bss_0->unk10[0];
    temp_ft4 = bss_0->unk20[1] - bss_0->unk20[0];
    spF0 = bss_0->unk30[1] - bss_0->unk30[0];
    temp_fv0 = sqrtf(SQ(spF8) + SQ(temp_ft4) + SQ(spF0));
    if (temp_fv0 != 0.0f) {
        spF8 *= (1.0f / temp_fv0);
        spF0 *= (1.0f / temp_fv0);
    }
    spCC.x = bss_0->unk10[1];
    spCC.y = bss_0->unk20[1];
    spCC.z = bss_0->unk30[1];
    spD8.x = bss_0->unk10[0] - (spF8 * 20.0f);
    spD8.y = bss_0->unk20[1];
    spD8.z = bss_0->unk30[0] - (spF0 * 20.0f);
    sp60.unk50[0] = -1;
    sp60.unk54[0] = 4;
    sp60.unk40[0] = 4.5f;
    fit_aabb_around_cubes(&sp48, (Vec3f* ) bss_0->unk10, &spCC, &sp60.unk40[0], 1);
    func_80053750(player, &sp48, 0);
    func_8005509C(player, spD8.f, spCC.f, 1, &sp60, 0);
    bss_0->unk10[1] = spCC.x;
    bss_0->unk20[1] = spCC.y;
    bss_0->unk30[1] = spCC.z;
    spF8 = bss_0->unk10[1] - bss_0->unk10[0];
    spF0 = bss_0->unk30[1] - bss_0->unk30[0];
    bss_0->unk118 = sqrtf(SQ(spF8) + SQ(spF0));
    bss_0->unk78.unk84 = bss_0->unk40;
    bss_0->unk78.unk88 = bss_0->unk50;
    bss_0->unk78.unk8C = NULL;
    bss_0->unk78.numControlPoints = 4;
    bss_0->unk78.unk80 = 0;
    bss_0->unk78.splineFunc = curves_hermite;
    bss_0->unk78.splineConverterFunc = curves_hermite_converter;
    bss_0->unk40[0] = (f32) arg0->srt.yaw;
    bss_0->unk40[1] = (f32) (s16) (0x8000 - arctan2_f(
        bss_0->unk10[1] - player->globalPosition.x, 
        bss_0->unk30[1] - player->globalPosition.z));
    bss_0->unk40[2] = 0.0f;
    bss_0->unk40[3] = 0.0f;
    if (((bss_0->unk40[0] - bss_0->unk40[1]) > 32768.0f) || ((bss_0->unk40[0] - bss_0->unk40[1]) < -32768.0f)) {
        if (bss_0->unk40[0] < 0.0f) {
            bss_0->unk40[0] += 65535.0f;
        } else if (bss_0->unk40[1] < 0.0f) {
            bss_0->unk40[1] += 65535.0f;
        }
    }
    bss_0->unk50[0] = (f32) arg0->srt.pitch;
    bss_0->unk50[1] = 0.0f;
    bss_0->unk50[2] = 0.0f;
    bss_0->unk50[3] = 0.0f;
    if (((bss_0->unk50[0] - bss_0->unk50[1]) > 32768.0f) || ((bss_0->unk50[0] - bss_0->unk50[1]) < -32768.0f)) {
        if (bss_0->unk50[0] < 0.0f) {
            bss_0->unk50[0] += 65535.0f;
        } else if (bss_0->unk50[1] < 0.0f) {
            bss_0->unk50[1] += 65535.0f;
        }
    }
    curves_move(&bss_0->unk78);
}

// offset: 0x137C | func: 7
static void dll_96_func_137C(CamControl_Data* arg0, Object* arg1) {
    bss_0->unk78.unk84 = bss_0->unk10;
    bss_0->unk78.unk88 = bss_0->unk20;
    bss_0->unk78.unk8C = bss_0->unk30;
    bss_0->unk78.numControlPoints = 4;
    bss_0->unk78.unk80 = 0;
    bss_0->unk78.splineFunc = curves_hermite;
    bss_0->unk78.splineConverterFunc = curves_hermite_converter;
    curves_move(&bss_0->unk78);
    arg0->player->srt.flags &= ~OBJFLAG_INVISIBLE;
}

/*0x0*/ static const char str_0[] = " %x %i \n";
