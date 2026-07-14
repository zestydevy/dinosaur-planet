#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/84_camnormal.h"
#include "dlls/engine/85_attentioncam.h"
#include "dlls/engine/86_cam1stperson.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/curves.h"
#include "sys/joypad.h"
#include "sys/memory.h"
#include "sys/voxmap.h"
#include "dll.h"
#include "macros.h"

// The default camera used during most of gameplay

typedef struct {
    f32 unk0;
    f32 unk4;
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
    f32 unk7C;
    s16 unk80;
    s16 unk82;
    s16 unk84;
    s16 unk86;
    s16 unk88;
    s16 unk8A;
    f32 unk8C;
    f32 unk90;
    f32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    f32 unkA4;
    s32 unkA8;
    u8 unkAC;
    u8 unkAD;
    u8 unkAE;
    u8 _unkAF[0xB0 - 0xAF];
} CamNormal;

/*0x0*/ static u32 data_0[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static CamNormal* sState;
/*0x4*/ static f32 bss_4;

void camnormal_func_ED4(Cam* cam, Object* arg1);
void camnormal_func_11F0(Cam* cam, f32 arg1, f32 arg2);
void camnormal_func_12E8(Cam* cam, Object* arg1, f32 arg2, f32 arg3);
void camnormal_func_1A58(Cam* cam, s32 arg1, s32 arg2, f32* arg3, f32* arg4);
static void camnormal_func_1DEC(Cam* cam);
static void camnormal_func_1FFC(Cam* cam, Object* arg1);
static void camnormal_func_22D0(Cam* cam, f32 arg1, f32 arg2);
static void camnormal_func_26E4(Cam* cam, Object* arg1);
static s32 camnormal_func_2388(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, s32 arg3);
static s32 camnormal_func_24F0(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, f32 arg3);
static s32 camnormal_func_250C(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, f32 arg3);
static void camnormal_func_29B4(Cam* cam, Object* arg1, f32 arg2, f32 arg3, s16 arg4);
static void camnormal_func_2A5C(Cam* cam, Object* obj);

// offset: 0x0 | ctor
void camnormal_ctor(void* dll) {
    sState = mmAlloc(sizeof(CamNormal), ALLOC_TAG_CAM_COL, ALLOC_NAME("camnormal"));
    bzero(sState, sizeof(CamNormal));
}

// offset: 0x68 | dtor
void camnormal_dtor(void* dll) {
    mmFree(sState);
}

// offset: 0xA8 | func: 0 | export: 0
void camnormal_func_A8(Cam* cam, s32 arg1, CamNormal_Params* data) {
    Object* player;
    s32 pad[2];
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    player = cam->player;
    switch (arg1) {
    case 0:
        cam->srt.transl.x = player->srt.transl.x + (fsin16_precise(player->srt.yaw) * (f32) data->unk1A);
        cam->srt.transl.y = player->srt.transl.y + (f32) data->unk1F;
        cam->srt.transl.z = player->srt.transl.z + (fcos16_precise(player->srt.yaw) * (f32) data->unk1A);
        cam->srt.yaw = 0;
        cam->srt.pitch = 0;
        cam->srt.roll = 0;
        cam->fov = (f32) data->unk19;
        bzero(sState, sizeof(CamNormal));
        sState->unk30 = sState->unk0 = (f32) data->unk1C;
        sState->unk38 = sState->unk4 = (f32) data->unk1A;
        sState->unk40 = sState->unk8 = sState->unk98 = (f32) data->unk1F;
        sState->unk48 = sState->unkC = sState->unk9C = (f32) data->unk1F;
        sState->unk94 = sState->unk8C = (f32) data->unk1E;
        sState->unk50 = sState->unk10 = 0.09f;
        sState->unk58 = sState->unk14 = sState->unk54 = 0.04f;
        sState->unk60 = sState->unk18 = 50.0f;
        sState->unk68 = sState->unk1C = 30.0f;
        sState->unk24 = -100000.0f;
        sState->unk20 = 100000.0f;
        sState->unkAD = 1;
        sState->unk70 = (f32) cam->fov;
        break;
    case 4:
        camnormal_func_2A5C(cam, player);
        gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp48, &sp44, &sp40, &sp3C, sState->unk8C);
        sp44 = cam->srt.transl.y - (player->srt.transl.y + sState->unk8C);
        cam->srt.pitch = arctan2_f(sp44, sp3C);
        cam->srt.roll = 0;
        cam->positionMirror.x = cam->srt.transl.x;
        cam->positionMirror.y = cam->srt.transl.y;
        cam->positionMirror.z = cam->srt.transl.z;
        cam->fov = sState->unk70;
        sState->unk82 = 0;
        break;
    case 2:
        if (data != NULL) {
            sState->unk94 = (f32) data->unk5;
            sState->unk40 = sState->unk98 = (f32) data->unk6;
            sState->unk48 = sState->unk9C = (f32) data->unk8;
            sState->unk30 = (f32) data->unk3;
            sState->unk38 = (f32) data->unk4;
            sState->unk70 = (f32) data->unk2;
            sState->unk60 = (f32) data->unk9;
            sState->unk68 = (f32) data->unkA;
            if (data->unkB != 0) {
                sState->unk50 = (f32) data->unkB / 255.0f;
            } else {
                sState->unk50 = 0.09f;
            }
            if (data->unkC != 0) {
                sState->unk58 = (f32) data->unkC / 255.0f;
            } else {
                sState->unk58 = 0.09f;
            }
            sState->unk82 = data->unk1;
            sState->unk84 = data->unk1;
            cam->letterboxGoal = data->unk7;
        } else {
            sState->unk94 = sState->unk90;
            sState->unk40 = sState->unk98 = sState->unk3C;
            sState->unk48 = sState->unk9C = sState->unk44;
            sState->unk30 = sState->unk2C;
            sState->unk38 = sState->unk34;
            sState->unk70 = sState->unk6C;
            sState->unk60 = sState->unk5C;
            sState->unk68 = sState->unk64;
            sState->unk50 = sState->unk4C;
            sState->unk58 = sState->unk54;
            sState->unk82 = 0x3C;
            sState->unk84 = 0x3C;
        }
        sState->unk90 = sState->unk8C;
        sState->unk3C = sState->unk8;
        sState->unk44 = sState->unkC;
        sState->unk2C = sState->unk0;
        sState->unk34 = sState->unk4;
        sState->unk6C = cam->fov;
        sState->unk5C = sState->unk18;
        sState->unk64 = sState->unk1C;
        sState->unk4C = sState->unk10;
        sState->unk54 = sState->unk14;
        break;
    case 3:
        STUBBED_PRINTF(" CUT CAM ");
        cam->fov = sState->unk70;
        cam->srt.transl.x = sState->unk74;
        cam->srt.transl.y = sState->unk78;
        cam->srt.transl.z = sState->unk7C;
        cam->srt.yaw = sState->unk86;
        cam->srt.pitch = sState->unk88;
        cam->srt.roll = sState->unk8A;
        cam->positionMirror.x = cam->srt.transl.x;
        cam->positionMirror.y = cam->srt.transl.y;
        cam->positionMirror.z = cam->srt.transl.z;
        sState->unk82 = 0;
        break;
    case 1:
        cam->fov = sState->unk70;
        break;
    }
    cam->unk118 = 1;
}

// offset: 0x860 | func: 1 | export: 1
void camnormal_func_860(Cam* cam) {
    Object* player;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 sp38;

    player = cam->player;
    if (player != NULL) {
        if (player->controlNo == OBJCONTROL_Player) {
            ((DLL_210_Player*)player->dll)->vtbl->func36(player, &sp38);
            bss_4 = gUpdateRateF * sp38;
            switch (((DLL_210_Player*)player->dll)->vtbl->func42(player)) {
            default:
                sState->unk14 = sState->unk58;
                sState->unkAE = 8;
                break;
            case 1:
                sState->unk14 = 0.0f;
                sState->unkAE = 0xFF;
                break;
            case 2:
                sState->unk14 = 0.005f;
                sState->unkAE = 0xC;
                break;
            case 3:
                sState->unk14 = 0.055f;
                sState->unkAE = 8;
                break;
            }
        } else {
            bss_4 = gUpdateRateF;
        }
        cam->unk118 = 0;
        camnormal_func_1DEC(cam);
        camnormal_func_ED4(cam, player);
        camnormal_func_1FFC(cam, player);
        camnormal_func_1A58(cam, 3, 3, &sState->unkA0, &sState->unkA4);
        cam->positionMirror.x = cam->srt.transl.x;
        cam->positionMirror.y = cam->srt.transl.y;
        cam->positionMirror.z = cam->srt.transl.z;
        camnormal_func_12E8(cam, player, sState->unkA0, sState->unkA4);
        camnormal_func_1A58(cam, 1, 5, NULL, NULL);
        gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp48, &sp44, &sp40, &sp3C, sState->unk8C);
        camnormal_func_22D0(cam, sp48, sp40);
        camnormal_func_11F0(cam, player->srt.transl.y, sp3C);
        camnormal_func_26E4(cam, player);
        cam->srt.roll -= ((s32) ((f32) cam->srt.roll * gUpdateRateF) >> 3);
    }
}

// offset: 0xB70 | func: 2 | export: 2
void camnormal_func_B70(Cam* cam) {
    sState->unk74 = cam->srt.transl.x;
    sState->unk78 = cam->srt.transl.y;
    sState->unk7C = cam->srt.transl.z;
    sState->unk86 = cam->srt.yaw;
    sState->unk88 = cam->srt.pitch;
    sState->unk8A = cam->srt.roll;
}

// offset: 0xBD4 | func: 3 | export: 3
void camnormal_func_BD4(CamNormal_Params* arg0, s32 arg1) {
    Cam* cam = gDLL_2_Camera->vtbl->get_cam();
    sState->unk90 = sState->unk8C;
    sState->unk3C = sState->unk8;
    sState->unk44 = sState->unkC;
    sState->unk2C = sState->unk0;
    sState->unk34 = sState->unk4;
    sState->unk6C = cam->fov;
    sState->unk5C = sState->unk18;
    sState->unk64 = sState->unk1C;
    sState->unk54 = sState->unk14;
    sState->unk4C = sState->unk10;
    sState->unk94 = sState->unk8C = (f32) arg0->unk5;
    sState->unk40 = sState->unk98 = sState->unk8 = (f32) arg0->unk6;
    sState->unk48 = sState->unk9C = sState->unkC = (f32) arg0->unk8;
    sState->unk30 = sState->unk0 = (f32) arg0->unk3;
    sState->unk38 = sState->unk4 = (f32) arg0->unk4;
    sState->unk70 = cam->fov = (f32) arg0->unk2;
    sState->unk60 = sState->unk18 = (f32) arg0->unk9;
    sState->unk68 = sState->unk1C = (f32) arg0->unkA;
    if (arg0->unkB != 0) {
        sState->unk50 = sState->unk10 = (f32) arg0->unkB / 255.0f;
    } else {
        sState->unk50 = 0.09f;
    }
    if (arg0->unkC != 0) {
        sState->unk58 = sState->unk14 = (f32) arg0->unkC / 255.0f;
    } else {
        sState->unk58 = 0.09f;
    }
    sState->unk82 = 0;
    sState->unk84 = 0;
}

// offset: 0xED4 | func: 4 | export: 4
void camnormal_func_ED4(Cam* cam, Object* arg1) {
    f32 temp_fv0_3;
    f32 var_fv1;
    f32 temp;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    f32 sp30;

    sp30 = bss_4;
    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp40, &sp3C, &sp38, &sp34, sState->unk8C);
    sp34 = SQ(sp40) + SQ(sp3C) + SQ(sp38);
    if (sp34 > 0.0f) {
        sp34 = sqrtf(sp34);
    }
    if (sp34 < 5.0f) {
        sp34 = 5.0f;
    }
    if ((2.0f * sState->unk4) < sp34) {
        camnormal_func_2A5C(cam, arg1);
        cam->positionMirror.x = cam->srt.transl.x;
        cam->positionMirror.y = cam->srt.transl.y;
        cam->positionMirror.z = cam->srt.transl.z;
        gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp40, &sp3C, &sp38, &sp34, sState->unk8C);
        sp34 = SQ(sp40) + SQ(sp3C) + SQ(sp38);
        if (sp34 > 0.0f) {
            sp34 = sqrtf(sp34);
        }
        if (sp34 < 5.0f) {
            sp34 = 5.0f;
        }
    }
    if (sState->unk4 < sp34) {
        var_fv1 = sState->unk4;
    } else if (sp34 < sState->unk0) {
        var_fv1 = sState->unk0;
    } else {
        var_fv1 = sp34;
    }
    if ((var_fv1 != sp34) && (sState->unk10 != 0.0f)) {
        if (sp34 < 1.0f) {
            sp34 = 1.0f;
        }
        var_fv1 = (sp34 - var_fv1);
        sp30 *= sState->unk10;
        if (sp30 > 1.0f) {
            sp30 = 1.0f;
        }
        var_fv1 = var_fv1 * sp30;
        temp_fv0_3 = ((sp34 + var_fv1) / sp34);
        if (temp_fv0_3 > 0.0f) {
            cam->srt.transl.x = (sp40 / temp_fv0_3) + arg1->srt.transl.x;
            cam->srt.transl.z = (sp38 / temp_fv0_3) + arg1->srt.transl.z;
        }
    }
}

// offset: 0x11F0 | func: 5 | export: 5
void camnormal_func_11F0(Cam* cam, f32 arg1, f32 arg2) {
    s32 var_v1 = arctan2_f(cam->srt.transl.y - (arg1 + sState->unk8C), arg2) - (cam->srt.pitch & 0xFFFF);
    CIRCLE_WRAP(var_v1);
    cam->srt.pitch += ((s32) ((f32) var_v1 * bss_4) / sState->unkAE);
}

// offset: 0x12E8 | func: 6 | export: 6
void camnormal_func_12E8(Cam* cam, Object* arg1, f32 arg2, f32 arg3) {
    f32 spD4;
    f32 spD0;
    f32 spCC;
    f32 spC8;
    f32 spC4;
    f32 spC0;
    s32 _pad;
    f32 spB8;
    s32 _pad2;
    f32 spB0;
    DLL27_Data* collider;
    MtxF sp6C;
    f32 sp68;
    f32 sp64;
    f32 sp60;
    f32 var_fv1;
    SRT sp44;
    Player_Data* sp40;

    spB0 = bss_4;
    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &spD4, &spD0, &spCC, &spC8, sState->unk8C);
    spC8 = SQ(spD4) + SQ(spD0) + SQ(spCC);
    if (spC8 > 0.0f) {
        spC8 = sqrtf(spC8);
    }
    if (spC8 < 5.0f) {
        spC8 = 5.0f;
    }
    spC4 = arg1->srt.transl.y + sState->unk8C + sState->unkC;
    spC0 = arg1->srt.transl.y + sState->unk8C + sState->unk8;
    if (arg1->controlNo == OBJCONTROL_Player) {
        sp40 = arg1->data;
        collider = &sp40->unk0.unk4;
        sp44.yaw = 0x8000 - arctan2_f(spD4, spCC);
        sp44.pitch = 0;
        sp44.roll = 0;
        sp44.scale = 1.0f;
        sp44.transl.x = 0.0f;
        sp44.transl.y = 0.0f;
        sp44.transl.z = 0.0f;
        matrix_from_srt_reversed(&sp6C, &sp44);
        vec3_transform(&sp6C, 
            collider->floorNormalX, collider->floorNormalY, collider->floorNormalZ, 
            &sp68, &sp64, &sp60);
        sState->unkA8 += ((s32) (((-arctan2_f(sp64, sp60) - sState->unkA8) + 0x4000) * gUpdateRate) >> 5);
    } else {
        sState->unkA8 -= ((s32) (sState->unkA8 * gUpdateRate) >> 5);
    }
    if (sState->unkA8 < 0) {
        var_fv1 = fsin16_precise((s16) sState->unkA8) * sState->unk1C;
    } else if (sState->unkA8 > 0) {
        var_fv1 = fsin16_precise((s16) sState->unkA8) * sState->unk18;
    } else {
        var_fv1 = 0.0f;
    }
    spC0 += var_fv1;
    spC4 += var_fv1;
    spB8 = sState->unk0 - 25.0f;
    if (spB8 < 30.0f) {
        spB8 = 30.0f;
    }
    if (arg1->controlNo == OBJCONTROL_Player) {
        if (((DLL_210_Player*)arg1->dll)->vtbl->func79(arg1) <= 30.0f) {
            spD0 = (sState->unk4 * 0.8f) - sState->unk8;
            spD0 *= 0.05f;
            if (spD0 > 10.0f) {
                spD0 = 10.0f;
            }
            sState->unk8 += spD0;
            if (sState->unk4 < sState->unk8) {
                sState->unk8 = sState->unk4;
            }
            spD0 = (sState->unk4 * 0.8f) - sState->unkC;
            spD0 *= 0.05f;
            if (spD0 > 10.0f) {
                spD0 = 10.0f;
            }
            sState->unkC += spD0;
            if (sState->unk4 < sState->unkC) {
                sState->unkC = sState->unk4;
            }
        } else {
            spD0 = sState->unk98 - sState->unk8;
            spD0 *= 0.05f;
            if (spD0 > 10.0f) {
                spD0 = 10.0f;
            }
            sState->unk8 += spD0;
            if (sState->unk8 < sState->unk98) {
                sState->unk8 = sState->unk98;
            }
            spD0 = sState->unk9C - sState->unkC;
            spD0 *= 0.05f;
            if (spD0 > 10.0f) {
                spD0 = 10.0f;
            }
            sState->unkC += spD0;
            if (sState->unkC < sState->unk9C) {
                sState->unkC = sState->unk9C;
            }
            if (spC8 > 30.0f) {
                if (spC8 <= spB8) {
                    spC8 = (spC8 - 30.0f) / (spB8 - 30.0f);
                    if (spC8 < 0.0f) { spC8 = 0.0f; } else if (spC8 > 1.0f) { spC8 = 1.0f; }
                    spC0 = arg1->srt.transl.y + sState->unk8C + (sState->unk8 * spC8);
                    spC4 = arg1->srt.transl.y + sState->unk8C + (sState->unkC * spC8);
                }
            } else {
                spC0 = spC4 = arg1->srt.transl.y + sState->unk8C + ((30.0f - spC8) * 0.8f);
            }
        }
    }
    if (cam->srt.transl.y < spC0) {
        spD0 = spC0 - cam->srt.transl.y;
    } else if (spC4 < cam->srt.transl.y) {
        spD0 = spC4 - cam->srt.transl.y;
    } else {
        spD0 = 0.0f;
    }
    spB0 *= sState->unk14;
    if (spB0 > 1.0f) {
        spB0 = 1.0f;
    }
    spD0 *= spB0;
    cam->srt.transl.y += spD0;
    if (cam->srt.transl.y < (arg2 + 10.0f)) {
        cam->srt.transl.y = (arg2 + 10.0f);
    }
    if ((arg3 - 10.0f) < cam->srt.transl.y) {
        cam->srt.transl.y = (arg3 - 10.0f);
    }
}

// offset: 0x19E8 | func: 7 | export: 7
void camnormal_func_19E8(f32* arg0, f32* arg1, f32* arg2, f32* arg3, f32* arg4) {
    *arg0 = sState->unk0;
    *arg1 = sState->unk4;
    if (arg2 != NULL) {
        *arg2 = sState->unk8;
    }
    if (arg3 != NULL) {
        *arg3 = sState->unkC;
    }
    if (arg4 != NULL) {
        *arg4 = sState->unk8C;
    }
}

// offset: 0x1A58 | func: 8 | export: 8
void camnormal_func_1A58(Cam* cam, s32 arg1, s32 arg2, f32* arg3, f32* arg4) {
    f32 var_ft4;
    f32 var_ft5;
    f32 var_fv0;
    s32 temp_v0;
    s32 var_a1;
    Func_80057F1C_Struct** sp80;
    AABBs32 sp68;
    Object* sp64;

    sp64 = cam->player;
    if (arg1 & 1) {
        cam->unk34.unk50[0] = -1;
        cam->unk34.unk40[0] = 4.5f;
        cam->unk34.unk54[0] = arg2;
        fit_aabb_around_cubes(&sp68, &cam->positionMirror, &cam->srt.transl, cam->unk34.unk40, 1);
        func_80053750(sp64, &sp68, 1);
        func_8005509C(sp64, &cam->positionMirror.x, &cam->srt.transl.x, 1, &cam->unk34, 0);
    }
    if (arg1 & 2) {
        temp_v0 = func_80057F1C(sp64, cam->srt.transl.x, cam->srt.transl.y, cam->srt.transl.z, &sp80, 0, 0);
        *arg3 = -100000.0f;
        *arg4 = 100000.0f;
        var_ft5 = 100000.0f;
        var_ft4 = 100000.0f;
        for (var_a1 = 0; var_a1 < temp_v0; var_a1++) {
            if (sp80[var_a1]->unk0[2] < -0.707f) {
                if ((cam->srt.transl.y - 10.0f) < sp80[var_a1]->unk0[0]) {
                    var_fv0 = cam->srt.transl.y - sp80[var_a1]->unk0[0];
                    if (var_fv0 < 0.0f) {
                        var_fv0 = -var_fv0;
                    }
                    if (var_fv0 < var_ft4) {
                        *arg4 = sp80[var_a1]->unk0[0];
                        var_ft4 = var_fv0;
                    }
                }
            }
        }
        for (var_a1 = 0; var_a1 < temp_v0; var_a1++) {
            if (sp80[var_a1]->unk0[2] > 0.707f) {
                if (sp80[var_a1]->unk0[0] < (cam->srt.transl.y + 10.0f)) {
                    var_fv0 = cam->srt.transl.y - sp80[var_a1]->unk0[0];
                    if (var_fv0 < 0.0f) {
                        var_fv0 = -var_fv0;
                    }
                    if (var_fv0 < var_ft5) {
                        *arg3 = sp80[var_a1]->unk0[0];
                        var_ft5 = var_fv0;
                    }
                }
            }
        }
    }
}

// offset: 0x1DEC | func: 9
static void camnormal_func_1DEC(Cam* cam) {
    f32 spline[4];
    f32 t;

    if (sState->unk82 != 0) {
        sState->unk82 -= gUpdateRate;
        if (sState->unk82 < 0) {
            sState->unk82 = 0;
        }
        t = (f32) (sState->unk84 - sState->unk82) / (f32) sState->unk84;
        spline[0] = 0.0f;
        spline[2] = 0.0f;
        spline[3] = 0.0f;
        spline[1] = 1.0f;
        t = curvesHermite(spline, t, NULL);
        sState->unk8C = sState->unk90 + ((sState->unk94 - sState->unk90) * t);
        sState->unk0 = sState->unk2C + ((sState->unk30 - sState->unk2C) * t);
        sState->unk4 = sState->unk34 + ((sState->unk38 - sState->unk34) * t);
        sState->unk8 = sState->unk3C + ((sState->unk40 - sState->unk3C) * t);
        sState->unkC = sState->unk44 + ((sState->unk48 - sState->unk44) * t);
        sState->unk10 = sState->unk4C + ((sState->unk50 - sState->unk4C) * t);
        sState->unk14 = sState->unk54 + ((sState->unk58 - sState->unk54) * t);
        sState->unk18 = sState->unk5C + ((sState->unk60 - sState->unk5C) * t);
        sState->unk1C = sState->unk64 + ((sState->unk68 - sState->unk64) * t);
        cam->fov = sState->unk6C + ((sState->unk70 - sState->unk6C) * t);
    }
}

// offset: 0x1FFC | func: 10
static void camnormal_func_1FFC(Cam* cam, Object* arg1) {
    s32 temp;
    u8 sp63;
    Vec3s16 sp5C;
    Vec3s16 sp54;
    Vec3s16 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    s32 _pad;
    s16 temp_ft2;

    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp48, &sp44, &sp40, &sp3C, sState->unk8C);
    vox_func_80007EE0(&cam->srt.transl, &sp5C);
    vox_func_80007EE0(&arg1->srt.transl, &sp54);
    sp54.s[1] += 2;
    temp = vox_func_80008048(&sp5C, &sp54, &sp4C, &sp63, 0) == 0;
    sState->unkAC = temp;
    // @FAKE
    if (temp){}
    sState->unk24 = 100000.0f;
    sState->unk20 = 100000.0f;
    if ((sState->unkAC != 0) && (sState->unkAD != 0)) {
        if (sp63 & 1) {
            if (camnormal_func_24F0(cam, &sp5C, &sp54, arg1->srt.transl.y) == 0) {
                camnormal_func_2388(cam, &sp5C, &sp54, (s32) arg1->srt.yaw);
            }
        } else if (camnormal_func_2388(cam, &sp5C, &sp54, (s32) arg1->srt.yaw) == 0) {
            camnormal_func_24F0(cam, &sp5C, &sp54, arg1->srt.transl.y);
        }
    } else {
        camnormal_func_250C(cam, &sp5C, &sp54, arg1->srt.transl.y);
        sState->unkAD = 1;
    }
    if (sState->unk28 != 0.0f) {
        temp_ft2 = sState->unk28;
        if ((temp_ft2 < -0x1E) || (temp_ft2 >= 0x1F)) {
            camnormal_func_29B4(cam, arg1, sp48, sp40, temp_ft2);
        }
        sState->unk28 *= 0.9f;
        if ((sState->unk28 < 0.5f) && (sState->unk28 > -0.5f)) {
            sState->unk28 = 0.0f;
        }
    }
}

// offset: 0x22D0 | func: 11
static void camnormal_func_22D0(Cam* cam, f32 arg1, f32 arg2) {
    s32 temp_a0;
    s32 temp;

    temp_a0 = 0x8000 - arctan2_f(arg1, arg2);
    temp = temp_a0 - (cam->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(temp);
    sState->unk80 -= ((s32) ((f32) sState->unk80 * bss_4) >> 3);
    cam->srt.yaw = temp_a0 - sState->unk80;
}

// offset: 0x2388 | func: 12
static s32 camnormal_func_2388(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, s32 arg3) {
    s32 temp_v0;
    s32 sp38;
    f32 temp_fv1;
    f32 var_fa1;
    s32 var_v1_2;
    Vec3s16 sp24;

    sp38 = 0;
    temp_v0 = vox_func_8000896C(arg2, arg1, &sp24);
    if (temp_v0 != 0) {
        var_v1_2 = (-cam->srt.yaw - (arg3 & 0xFFFF)) + 0x8000;
        CIRCLE_WRAP(var_v1_2);
        if (var_v1_2 < 0) {
            var_v1_2 = -var_v1_2;
        }
        temp_fv1 = SQ(cam->speedAverage);
        temp_fv1 *= 4.0f;
        var_fa1 = temp_fv1;
        var_fa1 += ((f32) var_v1_2 / 500.0f);
        if (var_fa1 > 100.0f) {
            var_fa1 = 100.0f;
        }
        if (temp_v0 == -1) {
            var_fa1 = -var_fa1;
        }
        temp_fv1 = sState->unk28 + (var_fa1 * bss_4);
        if (temp_fv1 > 1000.0f) {
            temp_fv1 = 1000.0f;
        } else if (temp_fv1 < -1000.0f) {
            temp_fv1 = -1000.0f;
        }
        sState->unk28 = temp_fv1;
        sp38 = 1;
    }
    return sp38;
}

// offset: 0x24F0 | func: 13
static s32 camnormal_func_24F0(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, f32 arg3) {
    return 0;
}

// offset: 0x250C | func: 14
static s32 camnormal_func_250C(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, f32 arg3) {
    Vec3s16 sp68;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    s32 var_s3;
    s32 var_s4;
    Vec3f sp48;

    sp68 = *arg1;
    var_s1 = 0;
    var_s4 = 0;
    
    if ((arg3 + sState->unk8) <= cam->srt.transl.f[1]) {
        var_s2 = -1;
    } else {
        var_s2 = 1;
    }
    if (var_s2 == -1) {
        var_s3 = sp68.s[1] - 0xA;
    } else {
        var_s3 = sp68.s[1] + 0xA;
    }
    var_s0 = sp68.s[1] + var_s2;
    do {
        sp68.s[1] = var_s0;
        if (vox_func_80008048(&sp68, arg2, NULL, NULL, 1) == 0) {
            var_s4 = var_s2;
            var_s1 = 1;
        } else {
            var_s0 += var_s2;
            if (var_s0 == var_s3) {
                var_s1 = 1;
            }
        }
    } while (var_s1 == 0);
    if (var_s4 == 1) {
        sp68.s[1] = var_s0;
        vox_func_80007E2C(&sp48, &sp68);
        sState->unk24 = 100000.0f;
        sState->unk20 = sp48.f[1];
    } else if (var_s4 == -1) {
        sp68.s[1] = var_s0 + 1;
        vox_func_80007E2C(&sp48, &sp68);
        sState->unk24 = sp48.f[1];
        sState->unk20 = 100000.0f;
    } else {
        sState->unk24 = 100000.0f;
        sState->unk20 = 100000.0f;
    }
    return 0;
}

// offset: 0x26E4 | func: 15
static void camnormal_func_26E4(Cam* cam, Object* arg1) {
    u16 btns;
    AttentionCam_Params attentionCam;
    Cam1stPerson_Params cam1stPerson;
    u8 temp_v1;

    if (arg1->animObj == NULL) {
        btns = joyGetPressed(0);
        if (((cam->highlight != NULL) || (cam->srt.flags & OBJFLAG_UNK_2)) && 
                !(cam->highlightFlags & 2) && 
                (((temp_v1 = mainGetBits(BIT_4AD), (temp_v1 == 0)) && (btns & Z_TRIG)) || ((temp_v1 != 0) && (btns & R_TRIG)) || (cam->targetFlags & 2)) && 
                (arg1->controlNo == OBJCONTROL_Player) && 
                ((((DLL_210_Player*)arg1->dll)->vtbl->func60(arg1) != 0))) {
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMLOCKON, TRUE, 0, sizeof(&cam->highlight), &cam->highlight, 0x3C, Cam_Ease_All);
        } else if ((btns & U_CBUTTONS) && !(cam->highlightFlags & 1)) {
            cam1stPerson.unk0 = sState->unk4;
            cam1stPerson.unk4 = sState->unk8;
            cam1stPerson.unk8 = (s16) (u32) sState->unk8C;
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAM1STPERSON, TRUE, 0, sizeof(cam1stPerson), &cam1stPerson, 0, Cam_Ease_All);
        } else if ((btns & Z_TRIG) && !(cam->srt.flags & OBJFLAG_UNK_4)) {
            attentionCam.unk0 = 5;
            attentionCam.unk2 = 1;
            attentionCam.unk3 = 1;
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_ATTENTIONCAM, TRUE, 0, sizeof(attentionCam), &attentionCam, 0, Cam_Ease_All);
        }
    }
}

// offset: 0x29B4 | func: 16
static void camnormal_func_29B4(Cam* cam, Object* arg1, f32 arg2, f32 arg3, s16 arg4) {
    f32 sp24;
    f32 temp_fv0;

    sp24 = fsin16_precise((s16) arg4);
    temp_fv0 = fcos16_precise((s16) arg4);
    arg2 = (arg2 * temp_fv0) - (arg3 * sp24);
    arg3 = ((arg2 * sp24) + (arg3 * temp_fv0));
    cam->srt.transl.x = arg1->srt.transl.x + arg2;
    cam->srt.transl.z = arg1->srt.transl.z + arg3;
}

// offset: 0x2A5C | func: 17
static void camnormal_func_2A5C(Cam* cam, Object* obj) {
    f32 temp_fv0;
    f32 spD0;
    f32 spCC;
    Vec3f p1[1];
    Vec3f p2[1];
    AABBs32 aabb;
    Unk80027934 sp30;

    spD0 = fsin16_precise(obj->srt.yaw);
    spCC = fcos16_precise(obj->srt.yaw);
    temp_fv0 = sqrtf(SQ(sState->unk4) - SQ(sState->unk8));
    p2[0].x = obj->globalPosition.x + (spD0 * temp_fv0);
    p2[0].y = obj->globalPosition.y + sState->unk8C + sState->unk8;
    p2[0].z = obj->globalPosition.z + (spCC * temp_fv0);
    p1[0].x = obj->globalPosition.x;
    p1[0].y = p2[0].y;
    p1[0].z = obj->globalPosition.z;
    sp30.unk50[0] = -1;
    sp30.unk54[0] = 3;
    sp30.unk40[0] = 4.5f;
    fit_aabb_around_cubes(&aabb, p1, p2, &sp30.unk40[0], 1);
    func_80053750(obj, &aabb, 0);
    func_8005509C(obj, p1[0].f, p2[0].f, 1, &sp30, 0);
    cam->srt.transl.x = p2[0].x;
    cam->srt.transl.y = p2[0].y;
    cam->srt.transl.z = p2[0].z;
}
