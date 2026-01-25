#include "dlls/objects/210_player.h"
#include "game/objects/unknown_setups.h"
#include "sys/joypad.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/print.h"
#include "dll.h"
#include "functions.h"

typedef struct {
    DLL27_Data unk0;
    Vec3f unk260[2];
    Vec3f unk278[2];
    f32 unk290[2];
    f32 unk298[2];
    Vec4f unk2A0;
    f32 unk2B0; // wobble (smoothed)
    f32 unk2B4;
    f32 unk2B8; // move timer (when > 0, move forward)
    f32 unk2BC;
    f32 unk2C0; // wobble
    f32 unk2C4;
    f32 unk2C8;
    f32 unk2CC;
    f32 unk2D0[2];
    f32 unk2D8[2];
    f32 unk2E0[2];
    u8 _unk2E8[0x2F8 - 0x2E8];
    f32 unk2F8;
    f32 unk2FC;
    f32 unk300[2];
    f32 unk308;
    f32 unk30C;
    f32 unk310;
    u32 unk314;
    s32 unk318; // when rolling, the current roll rotation
    u16 unk31C[2];
    u16 unk320; // controller buttons pressed
    s16 unk322; // joystick x
    s16 unk324; // joystick y
    s16 unk326;
    s16 unk328;
    u8 unk32A; // roll state (0 = not rolling, 1 = left, 2 = right)
    u8 unk32B;
    u8 unk32C; // a pressed (turns off automatically after a time or if a is pressed again)
    u8 unk32D; // bitfield of which side of the log is touching terrain (0x1 = front, 0x2 = back, 0x3 = both)
    u8 unk32E; // 0 = off log, 1 = hopping on log, 2 = on log, 3 = hopping off log
    u8 _unk32F[0x338 - 0x32F];
    Object *unk338; // dockpoint
} BWlog_Data;

/*0x0*/ static Vec3f _data_0[] = {
    {0.0f, 0.0f, -30.0f}, 
    {0.0f, 0.0f, 30.0f}
};
/*0x18*/ static f32 _data_18[] = {8.0f, 8.0f};
/*0x20*/ static u8 _data_20[] = {1, 1};
/*0x24*/ static f32 _data_24[] = {500.0f, 500.0f, 0.0f, 0.0f};

static void dll_793_func_EB0(Object* self, BWlog_Data* objdata, s32 arg2);
static void dll_793_func_1600(Object* self, BWlog_Data* objdata);
static void dll_793_func_178C(Object* self, BWlog_Data* objdata, s32 arg2);
static void dll_793_func_1858(Object* self, BWlog_Data* objdata);
static void dll_793_func_1A4C(Object* self, BWlog_Data* objdata);
static void dll_793_func_1A5C(Object* self, BWlog_Data* objdata);
static void dll_793_func_1C18(Object* self, BWlog_Data* objdata);
static void dll_793_func_2020(Object* arg0, BWlog_Data* arg1);
static void dll_793_func_2444(Object* self, BWlog_Data* objdata);

// offset: 0x0 | ctor
void dll_793_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_793_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_793_setup(Object *self, ObjSetup *setup, s32 arg2) {
    BWlog_Data *objdata = (BWlog_Data*)self->data;
    s32 i;

    gDLL_27->vtbl->init(&objdata->unk0, 
        DLL27FLAG_NONE, 
        DLL27FLAG_8000000 | DLL27FLAG_40000 | DLL27FLAG_20 | DLL27FLAG_2 | DLL27FLAG_1, 
        DLL27MODE_1);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->unk0, 2, _data_0, _data_18, _data_20);
    objdata->unk0.boundsYExtension = 100;
    obj_add_object_type(self, OBJTYPE_11);
    objdata->unk31C[1] = 0x2000;
    objdata->unk2B4 = 15.0f;
    
    self->shadow->flags |= (OBJ_SHADOW_FLAG_WATER_SURFACE | OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_COLOR | OBJ_SHADOW_FLAG_CUSTOM_DIR);
    self->shadow->r = 0xFF;
    self->shadow->g = 0xFF;
    self->shadow->b = 0xFF;
    self->shadow->a = 0x7F;

    for (i = 0; i < 2; i++) {
        objdata->unk260[i].x = self->srt.transl.x;
        objdata->unk260[i].y = self->srt.transl.y;
        objdata->unk260[i].z = self->srt.transl.z;
    }
}

// offset: 0x170 | func: 1 | export: 1
void dll_793_control(Object* self) {
    BWlog_Data* objdata;
    f32 var_fv1;
    f32 sp184[3];
    Vec3f sp178;
    SRT sp160;
    MtxF sp120;
    MtxF spE0;
    MtxF spA0;
    f32 sp9C;
    f32 sp98;
    ObjType23Setup* temp_s0;
    s32 i;

    objdata = (BWlog_Data*)self->data;
    sp98 = 10000.0f;
    objdata->unk338 = obj_get_nearest_type_to(OBJTYPE_23, self, &sp98);
    if (objdata->unk338 != NULL) {
        temp_s0 = (ObjType23Setup*)objdata->unk338->setup;
        sp98 = vec3_distance(&self->positionMirror, &objdata->unk338->positionMirror);
        if (objdata->unk32E == 2) {
            var_fv1 = 0.95f;
        } else {
            var_fv1 = 0.5f;
        }
        if (sp98 < (f32) temp_s0->unk1A) {
            for (i = 0; i < 2; i++) {
                objdata->unk278[i].x *= var_fv1;
                objdata->unk278[i].z *= var_fv1;
            }
        } else {
            objdata->unk338 = NULL;
        }
    }
    dll_793_func_1C18(self, objdata);
    if (objdata->unk32E == 2) {
        dll_793_func_1600(self, objdata);
        switch (objdata->unk32A) {
        case 1:
        case 2:
            dll_793_func_1858(self, objdata);
            break;
        case 3:
        case 4:
            dll_793_func_1A4C(self, objdata);
            break;
        default:
            dll_793_func_1A5C(self, objdata);
            break;
        }
        self->srt.yaw -= (s32) ((f32) objdata->unk322 * (60.0f - ((f32) objdata->unk324 * 0.05f)) * 0.2f)  & 0xFFFF & 0xFFFF;
    }
    sp160.yaw = self->srt.yaw;
    sp160.pitch = self->srt.pitch;
    sp160.roll = self->srt.roll;
    sp160.scale = 1.0f;
    sp160.transl.x = self->srt.transl.x;
    sp160.transl.y = self->srt.transl.y;
    sp160.transl.z = self->srt.transl.z;
    matrix_from_srt(&sp120, &sp160);
    sp160.roll = 0;
    sp160.transl.x = 0;
    sp160.transl.y = 0;
    sp160.transl.z = 0;
    matrix_from_srt(&spE0, &sp160);
    sp160.yaw = -sp160.yaw;
    sp160.pitch = -sp160.pitch;
    matrix_from_srt_reversed(&spA0, &sp160);
    for (i = 0; i < 2; i++) {
        vec3_transform(&sp120, 
                       _data_0[i].x, _data_0[i].y, _data_0[i].z, 
                       &objdata->unk260[i].x, &objdata->unk260[i].y, &objdata->unk260[i].z);
        dll_793_func_EB0(self, objdata, i);
        vec3_transform(&spA0, 
                       objdata->unk2D0[i], 0.0f, objdata->unk2E0[i], 
                       &sp184[0], &sp184[1], &sp184[2]);
        sp184[0] *= -0.5f;
        sp184[2] *= 0.5f;
        objdata->unk310 = sqrtf(SQ(sp184[2]) + SQ(sp184[0]));
        sp184[2] += objdata->unk2FC;
        objdata->unk290[i] += ((sp184[2] - objdata->unk290[i]) * gUpdateRateF * 0.1f);
        if (objdata->unk32A == 0) {
            objdata->unk298[i] += ((sp184[0] - objdata->unk298[i]) * gUpdateRateF * 0.1f);
        }
        vec3_transform(&spE0, 
                       objdata->unk298[i], 0.0f, -objdata->unk290[i], 
                       &objdata->unk278[i].x, &sp9C, &objdata->unk278[i].z);
        sp184[0] = objdata->unk278[i].x * gUpdateRateF;
        sp184[1] = objdata->unk278[i].y * gUpdateRateF;
        sp184[2] = objdata->unk278[i].z * gUpdateRateF;
        objdata->unk260[i].x = objdata->unk260[i].x + sp184[0];
        objdata->unk260[i].y = objdata->unk260[i].y + sp184[1];
        objdata->unk260[i].z = objdata->unk260[i].z + sp184[2];
    }
    VECTOR_ADD(objdata->unk260[0], objdata->unk260[1], sp178);
    self->srt.transl.x = sp178.f[0] * 0.5f;
    self->srt.transl.y = sp178.f[1] * 0.5f;
    self->srt.transl.z = sp178.f[2] * 0.5f;
    VECTOR_SUBTRACT(objdata->unk260[1], objdata->unk260[0], sp178);
    self->srt.pitch = -arctan2_f(sp178.f[1], sqrtf(SQ(sp178.f[2]) + SQ(sp178.f[0])));
    gDLL_27->vtbl->func_1E8(self, &objdata->unk0, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(self, &objdata->unk0);
    gDLL_27->vtbl->func_624(self, &objdata->unk0, gUpdateRateF);
    dll_793_func_2020(self, objdata);
    dll_793_func_2444(self, objdata);
}

// offset: 0x85C | func: 2 | export: 2
void dll_793_update(Object *self) { }

// offset: 0x868 | func: 3 | export: 3
void dll_793_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x8BC | func: 4 | export: 4
void dll_793_free(Object *self, s32 a1) {
    BWlog_Data *objdata;

    objdata = self->data;
    obj_free_object_type(self, OBJTYPE_11);
    if (objdata->unk314 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk314);
    }
}


// offset: 0x92C | func: 5 | export: 5
u32 dll_793_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x93C | func: 6 | export: 6
u32 dll_793_get_data_size(Object *self, u32 a1) {
    return sizeof(BWlog_Data);
}

// offset: 0x950 | func: 7 | export: 7
s32 dll_793_func_950(Object *self, Object *rider) {
    BWlog_Data *objdata = (BWlog_Data*)self->data;
    if ((objdata->unk32E == 0) && (objdata->unk338 != NULL)) {
        return vec3_distance(&rider->positionMirror, &self->positionMirror) < 50.0f;
    }
    return 0;
}

// offset: 0x9E0 | func: 8 | export: 8
s32 dll_793_func_9E0(Object *self) {
    SRT sp88;
    MtxF sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 temp;
    f32 temp2;
    Object *player;

    player = get_player();
    if (player != NULL) {
        sp88.yaw = self->srt.yaw + 0x4000;
        sp88.pitch = self->srt.pitch;
        sp88.roll = self->srt.roll;
        sp88.transl.x = 0.0f;
        sp88.transl.y = 0.0f;
        sp88.transl.z = 0.0f;
        sp88.scale = 1.0f;
        matrix_from_srt(&sp48, &sp88);
        vec3_transform(&sp48, 0.0f, 0.0f, 1.0f, &sp44, &sp40, &sp3C);
        temp2 = -((self->srt.transl.x * sp44) + (sp40 * self->srt.transl.y) + (sp3C * self->srt.transl.z));
        temp = (player->srt.transl.x * sp44) + (sp40 * player->srt.transl.y) + (sp3C * player->srt.transl.z) + temp2;
        if (temp < 0) {
            return 1;
        }
    }
    return 2;
}

// offset: 0xB28 | func: 9 | export: 9
void dll_793_func_B28(Object *self, f32 *a1, f32 *a2, f32 *a3) {
    *a1 = self->srt.transl.x;
    *a2 = self->srt.transl.y;
    *a3 = self->srt.transl.z;
}

// offset: 0xB48 | func: 10 | export: 10
s32 dll_793_func_B48(Object *self, Object *rider) {
    BWlog_Data *objdata;
    f32 var_fs0;
    s32 i;

    objdata = (BWlog_Data*)self->data;
    if ((objdata->unk338 != NULL) && (objdata->unk32E == 2) && (joy_get_pressed(0) & B_BUTTON)) {
        var_fs0 = 0.0f;
        for (i = 0; i < 2; i++) {
            var_fs0 += sqrtf(SQ(objdata->unk278[i].x) + SQ(objdata->unk278[i].z));
        }
        return var_fs0 < 2.0f;
    }
    return 0;
}

// offset: 0xC3C | func: 11 | export: 11
s32 dll_793_func_C3C(Object *self) {
    return 0;
}

// offset: 0xC4C | func: 12 | export: 12
void dll_793_func_C4C(Object *self, f32 *a1, f32 *a2, f32 *a3) {
    MtxF sp48;
    SRT sp30;

    sp30.yaw = self->srt.yaw;
    sp30.pitch = 0;
    sp30.roll = 0;
    sp30.scale = 1.0f;
    sp30.transl.x = self->srt.transl.x;
    sp30.transl.y = self->srt.transl.y;
    sp30.transl.z = self->srt.transl.z;
    matrix_from_srt(&sp48, &sp30);
    vec3_transform(&sp48, 0.0f, 0.0f, -10.0f, a1, a2, a3);
}

// offset: 0xD08 | func: 13 | export: 13
s32 dll_793_func_D08(Object *self) {
    BWlog_Data *objdata = (BWlog_Data*)self->data;
    return objdata->unk32E;
}

// offset: 0xD18 | func: 14 | export: 14
void dll_793_func_D18(Object *self, s32 a1) {
    Object *player = get_player();
    BWlog_Data *objdata = (BWlog_Data*)self->data;

    if (a1 != 0) {
        obj_clear_map_id(self);
        ((DLL_210_Player*)player->dll)->vtbl->func28(player, 1);
        gDLL_2_Camera->vtbl->func8(0, 0x2B);
    } else {
        obj_infer_map_id(self);
        ((DLL_210_Player*)player->dll)->vtbl->func29(player, 1);
        gDLL_2_Camera->vtbl->func8(0, 1);
    }

    objdata->unk32E = a1;
}

// offset: 0xE2C | func: 15 | export: 15
void dll_793_func_E2C(Object *self, f32 *a1, s32 *a2) {
    BWlog_Data *objdata = (BWlog_Data*)self->data;
    *a1 = objdata->unk2CC;
    *a2 = objdata->unk32B;
}

// offset: 0xE48 | func: 16 | export: 16
f32 dll_793_func_E48(Object *self, f32 *a1) {
    *a1 = -0.006000000052f;
    return 0.0f;
}

// offset: 0xE70 | func: 17 | export: 17
s32 dll_793_func_E70(Object *self) {
    return 0;
}

// offset: 0xE80 | func: 18 | export: 18
void dll_793_func_E80(Object *self) { }

// offset: 0xE8C | func: 19 | export: 19
void dll_793_func_E8C(Object *self, f32 scale) { }

// offset: 0xE9C | func: 20 | export: 20
void dll_793_func_E9C(UNK_TYPE_32 a0, UNK_TYPE_32 a1, UNK_TYPE_32 a2) { }

// offset: 0xEB0 | func: 21
static void dll_793_func_EB0(Object* self, BWlog_Data* objdata, s32 arg2) {
    f32 temp_fa1;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 sp60;
    s32 sp50;
    f32 var_fv0;
    f32 var_fv1;
    s32 var_v1;
    f32 temp;

    temp = objdata->unk2B4 + objdata->unk0.waterYList[arg2];
    if ((objdata->unk260[arg2].y + 30.0f) < temp) {
        diPrintf("Water too high\n");
    }
    temp += (fsin16_precise(objdata->unk31C[arg2]) * 1.5f);
    objdata->unk31C[arg2] += (gUpdateRateF * 512.0f);
    sp60 = temp - objdata->unk260[arg2].y;
    if ((sp60 > 0.0f) && (objdata->unk300[arg2] < 0.0f)) {
        var_fv0 = objdata->unk278[arg2].y * 127.0f;
        if (var_fv0 < 0.0f) {
            var_fv0 = -var_fv0;
        }
        if (var_fv0 > 127.0f) {
            var_fv0 = 127.0f;
        }
        if (var_fv0 > 20.0f) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A75, (u8) var_fv0, NULL, NULL, 0, NULL);
        }
    } else if ((sp60 < 0.0f) && (objdata->unk300[arg2] > 0.0f)) {
        var_fv0 = objdata->unk278[arg2].y * 127.0f;
        if (var_fv0 < 0.0f) {
            var_fv0 = -var_fv0;
        }
        if (var_fv0 > 127.0f) {
            var_fv0 = 127.0f;
        }
        if (var_fv0 > 20.0f) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A74, (u8) var_fv0, NULL, NULL, 0, NULL);
        }
    }
    objdata->unk300[arg2] = sp60;
    if (sp60 > 25.0f) {
        sp60 = 25.0f;
    }
    if (sp60 < 0.0f) {
        sp60 = 0.0f;
    }
    objdata->unk278[arg2].y += ((sp60 / 15.0f) * 0.15f * gUpdateRateF);
    objdata->unk278[arg2].y -= (0.1f * gUpdateRateF);
    var_v1 = (s32) gUpdateRate;
    diPrintf("[%d]=%f\n", arg2, &sp60);
    if (sp60 > 0.0f) {
        if (objdata->unk278[arg2].y < 0.0f) {
            var_fv1 = sp60 / 25.0f;
            temp_ft5 = 1.0f + var_fv1;
            if (var_fv1 > 1.0f) {
                var_fv1 = 1.0f;
            }
            var_fv1 = (1.0f - var_fv1);
            temp_fa1 = (0.007000029f * var_fv1) + 0.988f;
            while (var_v1--) {
                temp_fv0 = objdata->unk278[arg2].y;
                if (temp_fv0 > 0/*.0f*/) {
                    var_fv1 = temp_fv0;
                } else {
                    var_fv1 = -temp_fv0;
                }
                objdata->unk278[arg2].y = (f32) (temp_fv0 - (temp_fv0 * var_fv1 * 0.1f * temp_ft5));
                objdata->unk290[arg2] = (f32) (objdata->unk290[arg2] * temp_fa1);
                objdata->unk298[arg2] = (f32) (objdata->unk298[arg2] * temp_fa1);
                objdata->unk2FC *= 0.99f;
            }
            return;
        }
        
        var_fv1 = sp60 / 25.0f;
        temp_ft5 = 1.0f + var_fv1;
        if (var_fv1 > 1.0f) {
            var_fv1 = 1.0f;
        }
        var_fv1 = (1.0f - var_fv1);
        temp_fa1 = (0.007000029f * var_fv1) + 0.988f;
        while (var_v1--) {
            temp_fv0 = objdata->unk278[arg2].y;
            if (temp_fv0 > 0/*.0f*/) {
                var_fv1 = temp_fv0;
            } else {
                var_fv1 = -temp_fv0;
            }
            objdata->unk278[arg2].y = (f32) (temp_fv0 - (temp_fv0 * var_fv1 * 0.1f * temp_ft5));
            objdata->unk290[arg2] = (f32) (objdata->unk290[arg2] * temp_fa1);
            objdata->unk298[arg2] = (f32) (objdata->unk298[arg2] * temp_fa1);
            objdata->unk2FC *= 0.99f;
        }
        return;
    }
    
    while (var_v1--) {
        objdata->unk278[arg2].y = (f32) (objdata->unk278[arg2].y * 0.994f);
        objdata->unk290[arg2] = (f32) (objdata->unk290[arg2] * 0.995f);
        objdata->unk298[arg2] = (f32) (objdata->unk298[arg2] * 0.99f);
        objdata->unk2FC *= 0.99f;
    }
}

// offset: 0x1600 | func: 22
static void dll_793_func_1600(Object* self, BWlog_Data* objdata) {
    s32 doubleTappedA;

    objdata->unk320 = joy_get_pressed(0);
    objdata->unk322 = joy_get_stick_x(0);
    objdata->unk324 = joy_get_stick_y(0);
    objdata->unk2F8 -= gUpdateRateF;
    if (objdata->unk2F8 <= 0.0f) {
        objdata->unk32C = 0;
        objdata->unk2F8 = 0.0f;
    }
    doubleTappedA = FALSE;
    if (objdata->unk320 & A_BUTTON) {
        if (objdata->unk32C != 0) {
            doubleTappedA = TRUE;
            objdata->unk32C = 0;
            objdata->unk2F8 = 0.0f;
        } else {
            objdata->unk32C = 1;
            objdata->unk2F8 = 15.0f;
        }
    }
    if (doubleTappedA) {
        if (objdata->unk322 >= 0x15) {
            dll_793_func_178C(self, objdata, 0);
            objdata->unk32A = 2;
            return;
        }
        if (objdata->unk322 < -0x14) {
            dll_793_func_178C(self, objdata, 1);
            objdata->unk32A = 1;
        }
    } else if (objdata->unk320 & A_BUTTON) {
        objdata->unk2B8 = 30.0f;
    }
}

// offset: 0x178C | func: 23
static void dll_793_func_178C(Object* self, BWlog_Data* objdata, s32 arg2) {
    if (arg2 != 0) {
        objdata->unk2A0.x = 1500.0f;
        objdata->unk2A0.y = 500.0f;
        objdata->unk2A0.z = 2000.0f;
        objdata->unk2A0.w = 4000.0f;
        objdata->unk32B = 2;
        objdata->unk298[0] = -2.5f;
        objdata->unk298[1] = -2.5f;
    } else {
        objdata->unk2A0.x = -1500.0f;
        objdata->unk2A0.y = -500.0f;
        objdata->unk2A0.z = -2000.0f;
        objdata->unk2A0.w = -4000.0f;
        objdata->unk32B = 3;
        objdata->unk298[0] = 2.5f;
        objdata->unk298[1] = 2.5f;
    }
    objdata->unk318 = 0;
    objdata->unk2BC = 0.0f;
    objdata->unk2B0 = objdata->unk2A0.x;
    objdata->unk2C8 = 0.0f;
    objdata->unk2CC = 0.0f;
}

// offset: 0x1858 | func: 24
static void dll_793_func_1858(Object* self, BWlog_Data* objdata) {
    s32 var_v0;

    objdata->unk2C8 = (f32) (objdata->unk2BC / 100.0f);
    objdata->unk2BC += gUpdateRateF;
    if (objdata->unk2C8 < 0.0f) {
        objdata->unk2C8 = 0.0f;
    } else if (objdata->unk2C8 > 1.0f) {
        objdata->unk32A = 0;
        objdata->unk2C8 = 1.0f;
    }
    objdata->unk2B0 = func_80004C5C(&objdata->unk2A0, objdata->unk2C8, NULL);
    var_v0 = gUpdateRate;
    while (var_v0--) {
        objdata->unk298[0] *= 0.985f;
        objdata->unk298[1] *= 0.985f;
    }
    objdata->unk318 = (s32) ((f32) objdata->unk318 + (gUpdateRateF * objdata->unk2B0));
    objdata->unk2CC = (f32) objdata->unk318 / 65535.0f;
    if (objdata->unk2CC < 0.0f) {
        objdata->unk2CC = -objdata->unk2CC;
    }
    if (objdata->unk2CC > 1.0f) {
        objdata->unk2CC = 1.0f;
    }
    if ((objdata->unk318 > 0xFFFF) || (objdata->unk318 < -0xFFFF)) {
        objdata->unk32A = 0;
    }
    if (objdata->unk2C8 > 0.7f) {
        objdata->unk2B4 = 15.0f;
    } else {
        objdata->unk2B4 = -2.0f;
    }
    self->srt.roll = (s16) objdata->unk318;
}

// offset: 0x1A4C | func: 25
static void dll_793_func_1A4C(Object* self, BWlog_Data* objdata) { }

// offset: 0x1A5C | func: 26
static void dll_793_func_1A5C(Object* self, BWlog_Data* objdata) {
    s32 var_v0;

    objdata->unk2B4 = 15.0f;
    objdata->unk2B0 += (objdata->unk2C0 * gUpdateRateF);
    var_v0 = gUpdateRate;
    while (var_v0--) {
        objdata->unk2B0 *= 0.97f;
    }
    self->srt.roll += (s32) (objdata->unk2B0 * gUpdateRateF);
    objdata->unk2B8 -= gUpdateRateF;
    if (objdata->unk2B8 < 0.0f) {
        objdata->unk2B8 = 0.0f;
    }
    objdata->unk2C0 = (f32) ((f32) -self->srt.roll / 100.0f);
    if (objdata->unk2B8 > 0.0f) {
        objdata->unk32B = 1;
        objdata->unk2B4 = 13.5f;
        objdata->unk2C0 = (f32) (objdata->unk2C0 - 5.0f);
        objdata->unk2FC = 2.0f;
    } else {
        objdata->unk32B = 0;
    }
    objdata->unk2C0 -= ((f32) objdata->unk322 * 0.5f);
    objdata->unk298[0] += (((objdata->unk290[0] * (f32) objdata->unk322 * 0.03f) - objdata->unk298[0]) * gUpdateRateF * 0.02f);
    objdata->unk298[1] += (((-objdata->unk290[1] * (f32) objdata->unk322 * 0.05f) - objdata->unk298[1]) * gUpdateRateF * 0.02f);
}

// offset: 0x1C18 | func: 27
static void dll_793_func_1C18(Object* self, BWlog_Data* objdata) {
    s32 i;
    s32 k;
    s32 sp120[2];
    s32 objListLength;
    Object* obj;
    Object** objList;
    f32 temp_fs0;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv1;
    f32 sp100;
    f32 spFC;
    f32 spF8;
    SRT spE0;
    MtxF spA0;

    for (i = 0; i < 2; i++) {
        objdata->unk2D0[i] = 0.0f;
        objdata->unk2E0[i] = 0.0f;
        sp120[i] = 0;
    }

    // grabs DFriverflow instances (and possibly more)
    objList = obj_get_all_of_type(OBJTYPE_22, &objListLength);

    for (i = 0; i < objListLength; i++) {
        obj = objList[i];
        for (k = 0; k < 2; k++) {
            temp_fv0 = obj->srt.transl.y - objdata->unk260[k].y;
            if ((temp_fv0 <= 200.0f) && (temp_fv0 >= -200.0f)) {
                temp_fs0 = obj->srt.transl.x - objdata->unk260[k].x;
                temp_fv0_2 = obj->srt.transl.z - objdata->unk260[k].z;
                temp_fs0 = sqrtf(SQ(temp_fs0) + SQ(temp_fv0_2));
                temp_fv1 = (f32) ((ObjType22Setup*)obj->setup)->unk19 * 1.5f;
                if (temp_fs0 < temp_fv1) {
                    temp_fs0 = ((temp_fv1 - temp_fs0) / temp_fv1);
                    temp_fs0 *= (obj->srt.scale * 10.0f);
                    objdata->unk2D0[k] += fsin16_precise(obj->srt.yaw) * temp_fs0;
                    objdata->unk2E0[k] += fcos16_precise(obj->srt.yaw) * temp_fs0;
                    sp120[k] += 1;
                }
            }
        }
    }

    for (i = 0; i < 2; i++) {
        if (sp120[i] != 0) {
            objdata->unk2D0[i] /= sp120[i];
            objdata->unk2E0[i] /= sp120[i];
        }
    }
    
    for (i = 0; i < 2; i++) {
        spE0.yaw = arctan2_f(objdata->unk2D0[i], objdata->unk2E0[i]);
        spE0.pitch = 0;
        spE0.roll = 0;
        spE0.scale = 1.0f;
        spE0.transl.x = 0;
        spE0.transl.y = 0;
        spE0.transl.z = 0;
        matrix_from_srt_reversed(&spA0, &spE0);
        vec3_transform(&spA0, 
            objdata->unk0.waterNormalXList[i], objdata->unk0.waterNormalYList[i], objdata->unk0.waterNormalZList[i], 
            &sp100, &spFC, &spF8);
        spE0.yaw = 0;
        spE0.pitch = 0x4000 - arctan2_f(spFC, spF8);
        spE0.roll = -(0x4000 - arctan2_f(spFC, sp100));
        matrix_from_srt_reversed(&spA0, &spE0);
        vec3_transform(&spA0, 
            objdata->unk2D0[i], 0.0f, objdata->unk2E0[i], 
            &objdata->unk2D0[i], &objdata->unk2D8[i], &objdata->unk2E0[i]);
    }
}

// offset: 0x2020 | func: 28
static void dll_793_func_2020(Object* arg0, BWlog_Data* arg1) {
    u8 var_a1;
    u8 sp3E;
    s32 var_v1;

    if (arg1->unk314 == 0) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_A77, 0x7F, &arg1->unk314, NULL, 0, NULL);
    } else {
        arg1->unk30C = arg1->unk310 * 127.0f;
        arg1->unk30C += fsin16_precise(arg1->unk328) * 30.0f;
        if (arg1->unk30C < 30.0f) {
            arg1->unk30C = 30.0f;
        } else if (arg1->unk30C > 127.0f) {
            arg1->unk30C = 127.0f;
        }
        gDLL_6_AMSFX->vtbl->func_860(arg1->unk314, arg1->unk30C);
        arg1->unk308 = ((arg1->unk300[0] + arg1->unk300[1]) * 0.5f) / 25.0f;
        if (arg1->unk308 < 0.0f) {
            arg1->unk308 = 0.0f;
        }
        arg1->unk308 = 1.0f - arg1->unk308;
        arg1->unk308 = (arg1->unk308 * 0.2f) + 0.2f;
        arg1->unk308 += (fsin16_precise(arg1->unk326) * 0.1f);
        gDLL_6_AMSFX->vtbl->func_954(arg1->unk314, arg1->unk308);
        arg1->unk326 = arg1->unk326 + (gUpdateRate << 8);
        arg1->unk328 = arg1->unk328 + (gUpdateRate << 9);
    }
    var_v1 = 0;
    sp3E = arg1->unk0.unk25C & 3;
    var_a1 = sp3E & (sp3E ^ arg1->unk32D);
    if (var_a1 & 1) {
        var_v1 = (s32) ((sqrtf(SQ(arg1->unk290[0]) + SQ(arg1->unk298[0])) * 127.0f) / 0.95f);
    }
    if (var_a1 & 2) {
        if (var_v1 > ((sqrtf(SQ(arg1->unk290[1]) + SQ(arg1->unk298[1])) * 127.0f) / 0.95f)) {
            var_v1 = (s32) (f32) var_v1; // what
        } else {
            var_v1 = (s32) ((sqrtf(SQ(arg1->unk290[1]) + SQ(arg1->unk298[1])) * 127.0f) / 0.95f);
        }
    }
    if (var_v1 >= 0xB) {
        if (var_v1 >= 0x80) {
            var_v1 = 0x7F;
        }
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_76D_Log_Bump, var_v1, NULL, NULL, 0, NULL);
    }
    arg1->unk32D = sp3E;
}

// offset: 0x2444 | func: 29
static void dll_793_func_2444(Object* self, BWlog_Data* objdata) {
    f32 var_fs0;
    f32 var_fs1;
    f32 spF4;
    f32 spF0;
    f32 spEC;
    Vec3f spE0;
    Vec3f particleConfig;
    Vec3f spC8;
    f32 temp_fv0;
    s32 temp_ft5;
    SRT spA8;
    MtxF sp68;
    s32 i;

    var_fs0 = (objdata->unk2D0[0] + objdata->unk2D0[1]) * 0.5f;
    var_fs1 = (objdata->unk2E0[0] + objdata->unk2E0[1]) * 0.5f;
    spEC = sqrtf(SQ(var_fs0) + SQ(var_fs1));
    if (spEC != 0.0f) {
        var_fs0 /= spEC;
        var_fs1 /= spEC;
    }
    spF4 = fsin16_precise(self->srt.yaw);
    spE0.y = -fcos16_precise(self->srt.yaw);
    spE0.x = spF4;
    particleConfig.x = -400.0f;
    particleConfig.y = 400.0f;
    spA8.yaw = arctan2_f(var_fs0, var_fs1);
    spA8.pitch = 0;
    spA8.roll = 0;
    spA8.scale = 1.0f;
    spA8.transl.x = 0.0f;
    spA8.transl.y = 0.0f;
    spA8.transl.z = 0.0f;
    spC8.y = (objdata->unk278[0].x + objdata->unk278[1].x) * 0.5f;
    spC8.x = (objdata->unk278[0].z + objdata->unk278[1].z) * 0.5f;
    matrix_from_srt_reversed(&sp68, &spA8);
    vec3_transform(&sp68, spC8.y, 0, spC8.x, &spF4, &spC8.z, &spE0.z);
    spEC = spEC * 1.5f;
    spEC = spEC - spE0.z;
    if (spEC < 0.0f) {
        spEC = 0.0f;
    }
    temp_ft5 = (s32) spEC;
    particleConfig.z = (0.001f * spEC) + 0.004f;
    spA8.transl.x = (var_fs0 * spE0.y) + (var_fs1 * spE0.x);
    if (spA8.transl.x < 0.0f) {
        for (i = 0; i < temp_ft5; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_76C, &spA8, PARTFXFLAG_4, -1, &particleConfig);
        }
    } else {
        for (i = 0; i < temp_ft5; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_76C, &spA8, PARTFXFLAG_4, -1, &particleConfig);
        }
    }
}
