#include "dlls/engine/2_camcontrol.h"
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
} BSS0;

typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    u8 unk3;
    u8 unk4;
    s8 unk5;
    u8 unk6;
    s8 unk7;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 _unkD[0x19 - 0xD];
    u8 unk19;
    u16 unk1A;
    u16 unk1C;
    u8 unk1E;
    u8 unk1F;
} CamNormal_Params;

/*0x0*/ static u32 data_0[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static BSS0* bss_0;
/*0x4*/ static f32 bss_4;

void dll_84_func_ED4(Cam* cam, Object* arg1);
void dll_84_func_11F0(Cam* cam, f32 arg1, f32 arg2);
void dll_84_func_12E8(Cam* cam, Object* arg1, f32 arg2, f32 arg3);
void dll_84_func_1A58(Cam* cam, s32 arg1, s32 arg2, f32* arg3, f32* arg4);
static void dll_84_func_1DEC(Cam* cam);
static void dll_84_func_1FFC(Cam* cam, Object* arg1);
static void dll_84_func_22D0(Cam* cam, f32 arg1, f32 arg2);
static void dll_84_func_26E4(Cam* cam, Object* arg1);
static s32 dll_84_func_2388(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, s32 arg3);
static s32 dll_84_func_24F0(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, f32 arg3);
static s32 dll_84_func_250C(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, f32 arg3);
static void dll_84_func_29B4(Cam* cam, Object* arg1, f32 arg2, f32 arg3, s16 arg4);
static void dll_84_func_2A5C(Cam* cam, Object* obj);

// offset: 0x0 | ctor
void dll_84_ctor(void* dll) {
    bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, ALLOC_NAME("camnormal"));
    bzero(bss_0, sizeof(BSS0));
}

// offset: 0x68 | dtor
void dll_84_dtor(void* dll) {
    mmFree(bss_0);
}

// offset: 0xA8 | func: 0 | export: 0
void dll_84_func_A8(Cam* cam, s32 arg1, CamNormal_Params* action) {
    Object* player;
    s32 pad[2];
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    player = cam->player;
    switch (arg1) {
    case 0:
        cam->srt.transl.x = player->srt.transl.x + (fsin16_precise(player->srt.yaw) * (f32) action->unk1A);
        cam->srt.transl.y = player->srt.transl.y + (f32) action->unk1F;
        cam->srt.transl.z = player->srt.transl.z + (fcos16_precise(player->srt.yaw) * (f32) action->unk1A);
        cam->srt.yaw = 0;
        cam->srt.pitch = 0;
        cam->srt.roll = 0;
        cam->fov = (f32) action->unk19;
        bzero(bss_0, sizeof(BSS0));
        bss_0->unk30 = bss_0->unk0 = (f32) action->unk1C;
        bss_0->unk38 = bss_0->unk4 = (f32) action->unk1A;
        bss_0->unk40 = bss_0->unk8 = bss_0->unk98 = (f32) action->unk1F;
        bss_0->unk48 = bss_0->unkC = bss_0->unk9C = (f32) action->unk1F;
        bss_0->unk94 = bss_0->unk8C = (f32) action->unk1E;
        bss_0->unk50 = bss_0->unk10 = 0.09f;
        bss_0->unk58 = bss_0->unk14 = bss_0->unk54 = 0.04f;
        bss_0->unk60 = bss_0->unk18 = 50.0f;
        bss_0->unk68 = bss_0->unk1C = 30.0f;
        bss_0->unk24 = -100000.0f;
        bss_0->unk20 = 100000.0f;
        bss_0->unkAD = 1;
        bss_0->unk70 = (f32) cam->fov;
        break;
    case 4:
        dll_84_func_2A5C(cam, player);
        gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp48, &sp44, &sp40, &sp3C, bss_0->unk8C);
        sp44 = cam->srt.transl.y - (player->srt.transl.y + bss_0->unk8C);
        cam->srt.pitch = arctan2_f(sp44, sp3C);
        cam->srt.roll = 0;
        cam->positionMirror.x = cam->srt.transl.x;
        cam->positionMirror.y = cam->srt.transl.y;
        cam->positionMirror.z = cam->srt.transl.z;
        cam->fov = bss_0->unk70;
        bss_0->unk82 = 0;
        break;
    case 2:
        if (action != NULL) {
            bss_0->unk94 = (f32) action->unk5;
            bss_0->unk40 = bss_0->unk98 = (f32) action->unk6;
            bss_0->unk48 = bss_0->unk9C = (f32) action->unk8;
            bss_0->unk30 = (f32) action->unk3;
            bss_0->unk38 = (f32) action->unk4;
            bss_0->unk70 = (f32) action->unk2;
            bss_0->unk60 = (f32) action->unk9;
            bss_0->unk68 = (f32) action->unkA;
            if (action->unkB != 0) {
                bss_0->unk50 = (f32) action->unkB / 255.0f;
            } else {
                bss_0->unk50 = 0.09f;
            }
            if (action->unkC != 0) {
                bss_0->unk58 = (f32) action->unkC / 255.0f;
            } else {
                bss_0->unk58 = 0.09f;
            }
            bss_0->unk82 = action->unk1;
            bss_0->unk84 = action->unk1;
            cam->letterboxGoal = action->unk7;
        } else {
            bss_0->unk94 = bss_0->unk90;
            bss_0->unk40 = bss_0->unk98 = bss_0->unk3C;
            bss_0->unk48 = bss_0->unk9C = bss_0->unk44;
            bss_0->unk30 = bss_0->unk2C;
            bss_0->unk38 = bss_0->unk34;
            bss_0->unk70 = bss_0->unk6C;
            bss_0->unk60 = bss_0->unk5C;
            bss_0->unk68 = bss_0->unk64;
            bss_0->unk50 = bss_0->unk4C;
            bss_0->unk58 = bss_0->unk54;
            bss_0->unk82 = 0x3C;
            bss_0->unk84 = 0x3C;
        }
        bss_0->unk90 = bss_0->unk8C;
        bss_0->unk3C = bss_0->unk8;
        bss_0->unk44 = bss_0->unkC;
        bss_0->unk2C = bss_0->unk0;
        bss_0->unk34 = bss_0->unk4;
        bss_0->unk6C = cam->fov;
        bss_0->unk5C = bss_0->unk18;
        bss_0->unk64 = bss_0->unk1C;
        bss_0->unk4C = bss_0->unk10;
        bss_0->unk54 = bss_0->unk14;
        break;
    case 3:
        STUBBED_PRINTF(" CUT CAM ");
        cam->fov = bss_0->unk70;
        cam->srt.transl.x = bss_0->unk74;
        cam->srt.transl.y = bss_0->unk78;
        cam->srt.transl.z = bss_0->unk7C;
        cam->srt.yaw = bss_0->unk86;
        cam->srt.pitch = bss_0->unk88;
        cam->srt.roll = bss_0->unk8A;
        cam->positionMirror.x = cam->srt.transl.x;
        cam->positionMirror.y = cam->srt.transl.y;
        cam->positionMirror.z = cam->srt.transl.z;
        bss_0->unk82 = 0;
        break;
    case 1:
        cam->fov = bss_0->unk70;
        break;
    }
    cam->unk118 = 1;
}

// offset: 0x860 | func: 1 | export: 1
void dll_84_func_860(Cam* cam) {
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
                bss_0->unk14 = bss_0->unk58;
                bss_0->unkAE = 8;
                break;
            case 1:
                bss_0->unk14 = 0.0f;
                bss_0->unkAE = 0xFF;
                break;
            case 2:
                bss_0->unk14 = 0.005f;
                bss_0->unkAE = 0xC;
                break;
            case 3:
                bss_0->unk14 = 0.055f;
                bss_0->unkAE = 8;
                break;
            }
        } else {
            bss_4 = gUpdateRateF;
        }
        cam->unk118 = 0;
        dll_84_func_1DEC(cam);
        dll_84_func_ED4(cam, player);
        dll_84_func_1FFC(cam, player);
        dll_84_func_1A58(cam, 3, 3, &bss_0->unkA0, &bss_0->unkA4);
        cam->positionMirror.x = cam->srt.transl.x;
        cam->positionMirror.y = cam->srt.transl.y;
        cam->positionMirror.z = cam->srt.transl.z;
        dll_84_func_12E8(cam, player, bss_0->unkA0, bss_0->unkA4);
        dll_84_func_1A58(cam, 1, 5, NULL, NULL);
        gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp48, &sp44, &sp40, &sp3C, bss_0->unk8C);
        dll_84_func_22D0(cam, sp48, sp40);
        dll_84_func_11F0(cam, player->srt.transl.y, sp3C);
        dll_84_func_26E4(cam, player);
        cam->srt.roll -= ((s32) ((f32) cam->srt.roll * gUpdateRateF) >> 3);
    }
}

// offset: 0xB70 | func: 2 | export: 2
void dll_84_func_B70(Cam* cam) {
    bss_0->unk74 = cam->srt.transl.x;
    bss_0->unk78 = cam->srt.transl.y;
    bss_0->unk7C = cam->srt.transl.z;
    bss_0->unk86 = cam->srt.yaw;
    bss_0->unk88 = cam->srt.pitch;
    bss_0->unk8A = cam->srt.roll;
}

// offset: 0xBD4 | func: 3 | export: 3
void dll_84_func_BD4(CamNormal_Params* arg0, s32 arg1) {
    Cam* cam = gDLL_2_Camera->vtbl->get_cam();
    bss_0->unk90 = bss_0->unk8C;
    bss_0->unk3C = bss_0->unk8;
    bss_0->unk44 = bss_0->unkC;
    bss_0->unk2C = bss_0->unk0;
    bss_0->unk34 = bss_0->unk4;
    bss_0->unk6C = cam->fov;
    bss_0->unk5C = bss_0->unk18;
    bss_0->unk64 = bss_0->unk1C;
    bss_0->unk54 = bss_0->unk14;
    bss_0->unk4C = bss_0->unk10;
    bss_0->unk94 = bss_0->unk8C = (f32) arg0->unk5;
    bss_0->unk40 = bss_0->unk98 = bss_0->unk8 = (f32) arg0->unk6;
    bss_0->unk48 = bss_0->unk9C = bss_0->unkC = (f32) arg0->unk8;
    bss_0->unk30 = bss_0->unk0 = (f32) arg0->unk3;
    bss_0->unk38 = bss_0->unk4 = (f32) arg0->unk4;
    bss_0->unk70 = cam->fov = (f32) arg0->unk2;
    bss_0->unk60 = bss_0->unk18 = (f32) arg0->unk9;
    bss_0->unk68 = bss_0->unk1C = (f32) arg0->unkA;
    if (arg0->unkB != 0) {
        bss_0->unk50 = bss_0->unk10 = (f32) arg0->unkB / 255.0f;
    } else {
        bss_0->unk50 = 0.09f;
    }
    if (arg0->unkC != 0) {
        bss_0->unk58 = bss_0->unk14 = (f32) arg0->unkC / 255.0f;
    } else {
        bss_0->unk58 = 0.09f;
    }
    bss_0->unk82 = 0;
    bss_0->unk84 = 0;
}

// offset: 0xED4 | func: 4 | export: 4
void dll_84_func_ED4(Cam* cam, Object* arg1) {
    f32 temp_fv0_3;
    f32 var_fv1;
    f32 temp;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    f32 sp30;

    sp30 = bss_4;
    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp40, &sp3C, &sp38, &sp34, bss_0->unk8C);
    sp34 = SQ(sp40) + SQ(sp3C) + SQ(sp38);
    if (sp34 > 0.0f) {
        sp34 = sqrtf(sp34);
    }
    if (sp34 < 5.0f) {
        sp34 = 5.0f;
    }
    if ((2.0f * bss_0->unk4) < sp34) {
        dll_84_func_2A5C(cam, arg1);
        cam->positionMirror.x = cam->srt.transl.x;
        cam->positionMirror.y = cam->srt.transl.y;
        cam->positionMirror.z = cam->srt.transl.z;
        gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp40, &sp3C, &sp38, &sp34, bss_0->unk8C);
        sp34 = SQ(sp40) + SQ(sp3C) + SQ(sp38);
        if (sp34 > 0.0f) {
            sp34 = sqrtf(sp34);
        }
        if (sp34 < 5.0f) {
            sp34 = 5.0f;
        }
    }
    if (bss_0->unk4 < sp34) {
        var_fv1 = bss_0->unk4;
    } else if (sp34 < bss_0->unk0) {
        var_fv1 = bss_0->unk0;
    } else {
        var_fv1 = sp34;
    }
    if ((var_fv1 != sp34) && (bss_0->unk10 != 0.0f)) {
        if (sp34 < 1.0f) {
            sp34 = 1.0f;
        }
        var_fv1 = (sp34 - var_fv1);
        sp30 *= bss_0->unk10;
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
void dll_84_func_11F0(Cam* cam, f32 arg1, f32 arg2) {
    s32 var_v1 = arctan2_f(cam->srt.transl.y - (arg1 + bss_0->unk8C), arg2) - (cam->srt.pitch & 0xFFFF);
    CIRCLE_WRAP(var_v1);
    cam->srt.pitch += ((s32) ((f32) var_v1 * bss_4) / bss_0->unkAE);
}

// offset: 0x12E8 | func: 6 | export: 6
void dll_84_func_12E8(Cam* cam, Object* arg1, f32 arg2, f32 arg3) {
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
    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &spD4, &spD0, &spCC, &spC8, bss_0->unk8C);
    spC8 = SQ(spD4) + SQ(spD0) + SQ(spCC);
    if (spC8 > 0.0f) {
        spC8 = sqrtf(spC8);
    }
    if (spC8 < 5.0f) {
        spC8 = 5.0f;
    }
    spC4 = arg1->srt.transl.y + bss_0->unk8C + bss_0->unkC;
    spC0 = arg1->srt.transl.y + bss_0->unk8C + bss_0->unk8;
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
        bss_0->unkA8 += ((s32) (((-arctan2_f(sp64, sp60) - bss_0->unkA8) + 0x4000) * gUpdateRate) >> 5);
    } else {
        bss_0->unkA8 -= ((s32) (bss_0->unkA8 * gUpdateRate) >> 5);
    }
    if (bss_0->unkA8 < 0) {
        var_fv1 = fsin16_precise((s16) bss_0->unkA8) * bss_0->unk1C;
    } else if (bss_0->unkA8 > 0) {
        var_fv1 = fsin16_precise((s16) bss_0->unkA8) * bss_0->unk18;
    } else {
        var_fv1 = 0.0f;
    }
    spC0 += var_fv1;
    spC4 += var_fv1;
    spB8 = bss_0->unk0 - 25.0f;
    if (spB8 < 30.0f) {
        spB8 = 30.0f;
    }
    if (arg1->controlNo == OBJCONTROL_Player) {
        if (((DLL_210_Player*)arg1->dll)->vtbl->func79(arg1) <= 30.0f) {
            spD0 = (bss_0->unk4 * 0.8f) - bss_0->unk8;
            spD0 *= 0.05f;
            if (spD0 > 10.0f) {
                spD0 = 10.0f;
            }
            bss_0->unk8 += spD0;
            if (bss_0->unk4 < bss_0->unk8) {
                bss_0->unk8 = bss_0->unk4;
            }
            spD0 = (bss_0->unk4 * 0.8f) - bss_0->unkC;
            spD0 *= 0.05f;
            if (spD0 > 10.0f) {
                spD0 = 10.0f;
            }
            bss_0->unkC += spD0;
            if (bss_0->unk4 < bss_0->unkC) {
                bss_0->unkC = bss_0->unk4;
            }
        } else {
            spD0 = bss_0->unk98 - bss_0->unk8;
            spD0 *= 0.05f;
            if (spD0 > 10.0f) {
                spD0 = 10.0f;
            }
            bss_0->unk8 += spD0;
            if (bss_0->unk8 < bss_0->unk98) {
                bss_0->unk8 = bss_0->unk98;
            }
            spD0 = bss_0->unk9C - bss_0->unkC;
            spD0 *= 0.05f;
            if (spD0 > 10.0f) {
                spD0 = 10.0f;
            }
            bss_0->unkC += spD0;
            if (bss_0->unkC < bss_0->unk9C) {
                bss_0->unkC = bss_0->unk9C;
            }
            if (spC8 > 30.0f) {
                if (spC8 <= spB8) {
                    spC8 = (spC8 - 30.0f) / (spB8 - 30.0f);
                    if (spC8 < 0.0f) { spC8 = 0.0f; } else if (spC8 > 1.0f) { spC8 = 1.0f; }
                    spC0 = arg1->srt.transl.y + bss_0->unk8C + (bss_0->unk8 * spC8);
                    spC4 = arg1->srt.transl.y + bss_0->unk8C + (bss_0->unkC * spC8);
                }
            } else {
                spC0 = spC4 = arg1->srt.transl.y + bss_0->unk8C + ((30.0f - spC8) * 0.8f);
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
    spB0 *= bss_0->unk14;
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
void dll_84_func_19E8(f32* arg0, f32* arg1, f32* arg2, f32* arg3, f32* arg4) {
    *arg0 = bss_0->unk0;
    *arg1 = bss_0->unk4;
    if (arg2 != NULL) {
        *arg2 = bss_0->unk8;
    }
    if (arg3 != NULL) {
        *arg3 = bss_0->unkC;
    }
    if (arg4 != NULL) {
        *arg4 = bss_0->unk8C;
    }
}

// offset: 0x1A58 | func: 8 | export: 8
void dll_84_func_1A58(Cam* cam, s32 arg1, s32 arg2, f32* arg3, f32* arg4) {
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
static void dll_84_func_1DEC(Cam* cam) {
    f32 spline[4];
    f32 t;

    if (bss_0->unk82 != 0) {
        bss_0->unk82 -= gUpdateRate;
        if (bss_0->unk82 < 0) {
            bss_0->unk82 = 0;
        }
        t = (f32) (bss_0->unk84 - bss_0->unk82) / (f32) bss_0->unk84;
        spline[0] = 0.0f;
        spline[2] = 0.0f;
        spline[3] = 0.0f;
        spline[1] = 1.0f;
        t = curves_hermite(spline, t, NULL);
        bss_0->unk8C = bss_0->unk90 + ((bss_0->unk94 - bss_0->unk90) * t);
        bss_0->unk0 = bss_0->unk2C + ((bss_0->unk30 - bss_0->unk2C) * t);
        bss_0->unk4 = bss_0->unk34 + ((bss_0->unk38 - bss_0->unk34) * t);
        bss_0->unk8 = bss_0->unk3C + ((bss_0->unk40 - bss_0->unk3C) * t);
        bss_0->unkC = bss_0->unk44 + ((bss_0->unk48 - bss_0->unk44) * t);
        bss_0->unk10 = bss_0->unk4C + ((bss_0->unk50 - bss_0->unk4C) * t);
        bss_0->unk14 = bss_0->unk54 + ((bss_0->unk58 - bss_0->unk54) * t);
        bss_0->unk18 = bss_0->unk5C + ((bss_0->unk60 - bss_0->unk5C) * t);
        bss_0->unk1C = bss_0->unk64 + ((bss_0->unk68 - bss_0->unk64) * t);
        cam->fov = bss_0->unk6C + ((bss_0->unk70 - bss_0->unk6C) * t);
    }
}

// offset: 0x1FFC | func: 10
static void dll_84_func_1FFC(Cam* cam, Object* arg1) {
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

    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp48, &sp44, &sp40, &sp3C, bss_0->unk8C);
    func_80007EE0(&cam->srt.transl, &sp5C);
    func_80007EE0(&arg1->srt.transl, &sp54);
    sp54.s[1] += 2;
    temp = func_80008048(&sp5C, &sp54, &sp4C, &sp63, 0) == 0;
    bss_0->unkAC = temp;
    // @FAKE
    if (temp){}
    bss_0->unk24 = 100000.0f;
    bss_0->unk20 = 100000.0f;
    if ((bss_0->unkAC != 0) && (bss_0->unkAD != 0)) {
        if (sp63 & 1) {
            if (dll_84_func_24F0(cam, &sp5C, &sp54, arg1->srt.transl.y) == 0) {
                dll_84_func_2388(cam, &sp5C, &sp54, (s32) arg1->srt.yaw);
            }
        } else if (dll_84_func_2388(cam, &sp5C, &sp54, (s32) arg1->srt.yaw) == 0) {
            dll_84_func_24F0(cam, &sp5C, &sp54, arg1->srt.transl.y);
        }
    } else {
        dll_84_func_250C(cam, &sp5C, &sp54, arg1->srt.transl.y);
        bss_0->unkAD = 1;
    }
    if (bss_0->unk28 != 0.0f) {
        temp_ft2 = bss_0->unk28;
        if ((temp_ft2 < -0x1E) || (temp_ft2 >= 0x1F)) {
            dll_84_func_29B4(cam, arg1, sp48, sp40, temp_ft2);
        }
        bss_0->unk28 *= 0.9f;
        if ((bss_0->unk28 < 0.5f) && (bss_0->unk28 > -0.5f)) {
            bss_0->unk28 = 0.0f;
        }
    }
}

// offset: 0x22D0 | func: 11
static void dll_84_func_22D0(Cam* cam, f32 arg1, f32 arg2) {
    s32 temp_a0;
    s32 temp;

    temp_a0 = 0x8000 - arctan2_f(arg1, arg2);
    temp = temp_a0 - (cam->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(temp);
    bss_0->unk80 -= ((s32) ((f32) bss_0->unk80 * bss_4) >> 3);
    cam->srt.yaw = temp_a0 - bss_0->unk80;
}

// offset: 0x2388 | func: 12
static s32 dll_84_func_2388(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, s32 arg3) {
    s32 temp_v0;
    s32 sp38;
    f32 temp_fv1;
    f32 var_fa1;
    s32 var_v1_2;
    Vec3s16 sp24;

    sp38 = 0;
    temp_v0 = func_8000896C(arg2, arg1, &sp24);
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
        temp_fv1 = bss_0->unk28 + (var_fa1 * bss_4);
        if (temp_fv1 > 1000.0f) {
            temp_fv1 = 1000.0f;
        } else if (temp_fv1 < -1000.0f) {
            temp_fv1 = -1000.0f;
        }
        bss_0->unk28 = temp_fv1;
        sp38 = 1;
    }
    return sp38;
}

// offset: 0x24F0 | func: 13
static s32 dll_84_func_24F0(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, f32 arg3) {
    return 0;
}

// offset: 0x250C | func: 14
static s32 dll_84_func_250C(Cam* cam, Vec3s16* arg1, Vec3s16* arg2, f32 arg3) {
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
    
    if ((arg3 + bss_0->unk8) <= cam->srt.transl.f[1]) {
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
        if (func_80008048(&sp68, arg2, NULL, NULL, 1) == 0) {
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
        func_80007E2C(&sp48, &sp68);
        bss_0->unk24 = 100000.0f;
        bss_0->unk20 = sp48.f[1];
    } else if (var_s4 == -1) {
        sp68.s[1] = var_s0 + 1;
        func_80007E2C(&sp48, &sp68);
        bss_0->unk24 = sp48.f[1];
        bss_0->unk20 = 100000.0f;
    } else {
        bss_0->unk24 = 100000.0f;
        bss_0->unk20 = 100000.0f;
    }
    return 0;
}

// offset: 0x26E4 | func: 15
static void dll_84_func_26E4(Cam* cam, Object* arg1) {
    u16 btns;
    AttentionCam_Params attentionCam;
    Cam1stPerson_Params cam1stPerson;
    u8 temp_v1;

    if (arg1->animObj == NULL) {
        btns = joy_get_pressed(0);
        if (((cam->highlight != NULL) || (cam->srt.flags & OBJFLAG_UNK_2)) && 
                !(cam->highlightFlags & 2) && 
                (((temp_v1 = main_get_bits(BIT_4AD), (temp_v1 == 0)) && (btns & Z_TRIG)) || ((temp_v1 != 0) && (btns & R_TRIG)) || (cam->targetFlags & 2)) && 
                (arg1->controlNo == OBJCONTROL_Player) && 
                ((((DLL_210_Player*)arg1->dll)->vtbl->func60(arg1) != 0))) {
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMLOCKON, TRUE, 0, sizeof(&cam->highlight), &cam->highlight, 0x3C, Cam_Ease_All);
        } else if ((btns & U_CBUTTONS) && !(cam->highlightFlags & 1)) {
            cam1stPerson.unk0 = bss_0->unk4;
            cam1stPerson.unk4 = bss_0->unk8;
            cam1stPerson.unk8 = (s16) (u32) bss_0->unk8C;
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
static void dll_84_func_29B4(Cam* cam, Object* arg1, f32 arg2, f32 arg3, s16 arg4) {
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
static void dll_84_func_2A5C(Cam* cam, Object* obj) {
    f32 temp_fv0;
    f32 spD0;
    f32 spCC;
    Vec3f p1[1];
    Vec3f p2[1];
    AABBs32 aabb;
    Unk80027934 sp30;

    spD0 = fsin16_precise(obj->srt.yaw);
    spCC = fcos16_precise(obj->srt.yaw);
    temp_fv0 = sqrtf(SQ(bss_0->unk4) - SQ(bss_0->unk8));
    p2[0].x = obj->globalPosition.x + (spD0 * temp_fv0);
    p2[0].y = obj->globalPosition.y + bss_0->unk8C + bss_0->unk8;
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
