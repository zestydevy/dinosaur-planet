#include "PR/os.h"
#include "dlls/engine/27.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/unknown_setups.h"
#include "sys/gfx/modgfx.h"
#include "sys/controller.h"
#include "sys/dll.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "functions.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
} DFlog_Setup;

typedef struct {
    u8 _unk0[0x240 - 0x0];
    Vec3f unk240[2];
    Vec3f unk258[2];
    f32 unk270[2];
    f32 unk278[2];
    f32 unk280;
    f32 unk284;
    f32 unk288;
    DLL27_Data unk28C;
    u8 unk4EC;
    u8 unk4ED;
    u8 unk4EE;
    s16 unk4F0[2];
    Object *unk4F4; // dockpoint
} DFlog_Data;

/*0x0*/ static DLL_IModgfx *_data_0 = NULL;
/*0x4*/ static u32 _data_4[] = {
    0x00000000, 
    0x40000102, 0x00000000, 0x00000000, 
    0x00000000, 0x40010302, 0x00000000, 
    0x00000000, 0x00000000, 0x40020304, 
    0x00000000, 0x00000000, 0x00000000, 
    0x40030504, 0x00000000, 0x00000000, 
    0x00000000, 0x40040506, 0x00000000, 
    0x00000000, 0x00000000, 0x40050706, 
    0x00000000, 0x00000000, 0x00000000, 
    0x40060708, 0x00000000, 0x00000000, 
    0x00000000, 0x40070908, 0x00000000, 
    0x00000000, 0x00000000, 0x4008090a, 
    0x00000000, 0x00000000, 0x00000000, 
    0x40090b0a, 0x00000000, 0x00000000, 
    0x00000000, 0x400a0b0c, 0x00000000, 
    0x00000000, 0x00000000, 0x400b0d0c, 
    0x00000000, 0x00000000, 0x00000000
};
/*0xC8*/ static Vec3f _data_C8[] = {
    {0.0f, 0.0f, -30.0f}, 
    {0.0f, 0.0f, 30.0f}
};
/*0xE0*/ static f32 _data_E0[] = {8.0f, 8.0f};
/*0xE8*/ static u8 _data_E8[] = {1, 1};
/*0xEC*/ static Vec3f _data_EC[] = {
    {0.0f, 0.0f, -30.0f}, 
    {0.0f, 0.0f, 30.0f}
};
/*0x104*/ static f32 _data_104[] = {8.0f, 8.0f};

static int dll_417_func_3B8(Object *self, Object *a1, AnimObj_Data *a2, s8 a3);
static void dll_417_func_A90(Object *self);
static void dll_417_func_BC8(Object* self);
static void dll_417_func_E8C(Object* self);
static f32 dll_417_func_1E9C(Object* arg0, f32 arg1, f32 arg2, f32 arg3);

// offset: 0x0 | ctor
void dll_417_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_417_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_417_setup(Object* self, DFlog_Setup* setup, s32 arg2) {
    DFlog_Data* objdata;
    s32 i;

    objdata = (DFlog_Data*)self->data;
    _data_0 = (DLL_IModgfx*)dll_load_deferred(DLL_ID_136, 1);
    obj_add_object_type(self, OBJTYPE_11);
    self->animCallback = dll_417_func_3B8;
    self->srt.yaw = (setup->unk18 & 0xFFFF) << 8;
    gDLL_27->vtbl->init(&objdata->unk28C, 
        DLL27FLAG_NONE, 
        DLL27FLAG_40000 | DLL27FLAG_2, 
        DLL27MODE_1);
    gDLL_27->vtbl->setup_hits_collider(&objdata->unk28C, 2, _data_EC, _data_104, 8);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->unk28C, 2, _data_C8, _data_E0, _data_E8);
    gDLL_27->vtbl->reset(self, &objdata->unk28C);
    self->objhitInfo->unk58 |= 1;
    self->objhitInfo->unk58 |= 4;
    objdata->unk4EC = 0;
    objdata->unk4ED = 0;
    objdata->unk4EE = 0;
    for (i = 0; i < 2; i++) {
        objdata->unk240[i].x = self->srt.transl.x;
        objdata->unk240[i].y = self->srt.transl.y;
        objdata->unk240[i].z = self->srt.transl.z;
    }
    dll_417_func_A90(self);
    objdata->unk4F0[1] = 0;
    objdata->unk4F0[0] = 0;
    objdata->unk288 = 0.0f;
}

// offset: 0x208 | func: 1 | export: 1
void dll_417_control(Object* self) {
    DFlog_Data* objdata;
    Vec3f sp2C[2];

    objdata = self->data;
    func_8002674C(self);
    dll_417_func_A90(self);
    dll_417_func_BC8(self);
    dll_417_func_E8C(self);
    // below is nonsense (and probably not whatever the original was)
    sp2C[1].x = objdata->unk240[0].x;
    sp2C[1].y = objdata->unk240[0].y;
    sp2C[1].z = objdata->unk240[0].z;
    sp2C[0].x = -objdata->unk258[0].z * 10.0f;
    sp2C[0].y = 0.0f;
    sp2C[0].y = -objdata->unk258[0].y * 10.0f;
}

// offset: 0x2D8 | func: 2 | export: 2
void dll_417_update(Object *self) { }

// offset: 0x2E4 | func: 3 | export: 3
void dll_417_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x338 | func: 4 | export: 4
void dll_417_free(Object *self, s32 a1) {
    dll_unload(_data_0);
    obj_free_object_type(self, OBJTYPE_11);
}

// offset: 0x394 | func: 5 | export: 5
u32 dll_417_get_model_flags(Object *self) {
    return MODFLAGS_1;
}

// offset: 0x3A4 | func: 6 | export: 6
u32 dll_417_get_data_size(Object *self, u32 a1) {
    return sizeof(DFlog_Data);
}

// offset: 0x3B8 | func: 7
static int dll_417_func_3B8(Object *self, Object *a1, AnimObj_Data *a2, s8 a3) {
    func_800267A4(self);
    return 0;
}

// offset: 0x400 | func: 8 | export: 7
s32 dll_417_func_400(Object *self, Object *a1) {
    DFlog_Data *objdata = (DFlog_Data*)self->data;
    if ((objdata->unk4EC == 0) && (objdata->unk4F4 != NULL)) {
        return vec3_distance(&a1->positionMirror, &self->positionMirror) < 50.0f;
    }
    return 0;
}

// offset: 0x490 | func: 9 | export: 8
s32 dll_417_func_490(Object *self) {
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

// offset: 0x5D8 | func: 10 | export: 9
void dll_417_func_5D8(Object *self, f32 *a1, f32 *a2, f32 *a3) {
    *a1 = self->srt.transl.x;
    *a2 = self->srt.transl.y;
    *a3 = self->srt.transl.z;
}

// offset: 0x5F8 | func: 11 | export: 10
s32 dll_417_func_5F8(Object* arg0, Object* arg1) {
    DFlog_Data* objdata;
    f32 var_fs0;
    s32 temp_v0;
    s32 i;

    objdata = (DFlog_Data*)arg0->data;
    if ((objdata->unk4F4 != 0) && (objdata->unk4EC == 2)) {
        temp_v0 = gDLL_2_Camera->vtbl->func3();
        if ((temp_v0 != 0x56) && (temp_v0 != 0x60) && (gDLL_1_UI->vtbl->func_DC4() == 0) && (get_masked_button_presses(0) & B_BUTTON)) {
            var_fs0 = 0.0f;
            for (i = 0; i < 2; i++) {
                var_fs0 += sqrtf(SQ(objdata->unk258[i].x) + SQ(objdata->unk258[i].z));
            }
            
            return var_fs0 < 2.0f;
        }
    }
    ((DLL_210_Player*)arg1->dll)->vtbl->func28(arg1, 1);
    return 0;
}

// offset: 0x75C | func: 12 | export: 11
UNK_TYPE_32 dll_417_func_75C(Object *self) {
    return 0;
}

// offset: 0x76C | func: 13 | export: 12
void dll_417_func_76C(Object *self, f32 *a1, f32 *a2, f32 *a3) {
    MtxF sp48;
    SRT sp30;

    sp30.transl.x = self->srt.transl.x;
    sp30.transl.y = self->srt.transl.y;
    sp30.transl.z = self->srt.transl.z;
    sp30.yaw = self->srt.yaw;
    sp30.pitch = self->srt.pitch;
    sp30.roll = self->srt.roll;
    sp30.scale = 1.0f;
    matrix_from_srt(&sp48, &sp30);
    vec3_transform(&sp48, 0.0f, 1.5f, -1.0f, a1, a2, a3);
    *a1 = self->srt.transl.x;
    *a2 = self->srt.transl.y + 30.0f;
    *a3 = self->srt.transl.z;
}

// offset: 0x85C | func: 14 | export: 13
UNK_TYPE_8 dll_417_func_85C(Object *self) {
    DFlog_Data *objdata = (DFlog_Data*)self->data;
    return objdata->unk4EC;
}

// offset: 0x86C | func: 15 | export: 14
void dll_417_func_86C(Object *self, s32 a1) {
    DFlog_Data *objdata = (DFlog_Data*)self->data;
    Object *player;

    player = get_player();
    objdata->unk4EC = (s8) a1;
    if (a1 != 0) {
        ((DLL_210_Player*)player->dll)->vtbl->func28(player, 1);
        gDLL_2_Camera->vtbl->func8(0, 0x2B);
    } else {
        ((DLL_210_Player*)player->dll)->vtbl->func29(player, 1);
        gDLL_2_Camera->vtbl->func8(0, 1);
    }
}

// offset: 0x944 | func: 16 | export: 15
void dll_417_func_944(Object *self, f32 *a1, u32 *a2) {
    f32 temp_fv0;

    temp_fv0 = fsin16_precise(self->srt.pitch);
    if (self->srt.pitch > 0) {
        *a2 = 0;
    } else {
        *a2 = 1;
    }
    if (temp_fv0 >= 0.0f) {
        *a1 = temp_fv0;
        return;
    }
    *a1 = -temp_fv0;
}

// offset: 0x9D8 | func: 17 | export: 16
f32 dll_417_func_9D8(Object *self, f32 *a1) {
    DFlog_Data *objdata = (DFlog_Data*)self->data;
    f32 var_fa0;
    f32 var_fv1;

    var_fv1 = objdata->unk288 * 1.2f;
    if (objdata->unk288 >= 0.0f) {
        var_fa0 = objdata->unk288;
    } else {
        var_fa0 = -objdata->unk288;
    }
    *a1 = var_fa0 * 0.02f;
    if (var_fv1 < -1.0f) {
        var_fv1 = -1.0f;
    }
    if (var_fv1 > 1.0f) {
        var_fv1 = 1.0f;
    }
    return var_fv1;
}


// offset: 0xA64 | func: 18 | export: 17
s32 dll_417_func_A64(UNK_TYPE_32 a0) {
    return 0;
}

// offset: 0xA74 | func: 19 | export: 18
void dll_417_func_A74(UNK_TYPE_32 a0) { }

// offset: 0xA80 | func: 20 | export: 19
void dll_417_func_A80(UNK_TYPE_32 a0, UNK_TYPE_32 a1) { }

// offset: 0xA90 | func: 21
static void dll_417_func_A90(Object *self) {
    DFlog_Data *objdata;
    DLL27_Data *d27data;
    SRT spA8;
    MtxF sp68;
    s32 i;
    s32 j;

    objdata = (DFlog_Data*)self->data;

    spA8.yaw = self->srt.yaw;
    spA8.pitch = self->srt.pitch;
    spA8.roll = self->srt.roll;
    spA8.scale = 1.0f;
    spA8.transl.x = self->srt.transl.x;
    spA8.transl.y = self->srt.transl.y;
    spA8.transl.z = self->srt.transl.z;
    matrix_from_srt(&sp68, &spA8);

    d27data = &objdata->unk28C;

    for (i = 0, j = 0; i < (d27data->numTestPoints & 0xF); i++, j++) {
        vec3_transform(&sp68, 
            _data_EC[j].x, _data_EC[j].y, _data_EC[j].z, 
            &objdata->unk240[i].x, &objdata->unk240[i].y, &objdata->unk240[i].z);
    }
}

// offset: 0xBC8 | func: 22
static void dll_417_func_BC8(Object* arg0) {
    DFlog_Data* objdata;
    s32 i;
    s32 k;
    s32 sp9C[2];
    s32 objListLength;
    Object* obj;
    Object** objList;
    f32 temp_fs0;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv1;

    objdata = (DFlog_Data*)arg0->data;

    for (i = 0; i < 2; i++) {
        objdata->unk270[i] = 0.0f;
        objdata->unk278[i] = 0.0f;
        sp9C[i] = 0;
    }

    objList = obj_get_all_of_type(OBJTYPE_22, &objListLength);

    for (i = 0; i < objListLength; i++) {
        obj = objList[i];
        if (((ObjType22Setup*)obj->setup)->unk1A & 1) {
            for (k = 0; k < 2; k++) {
                temp_fv0 = obj->srt.transl.y - objdata->unk240[k].y;
                if ((temp_fv0 <= 200.0f) && (temp_fv0 >= -200.0f)) {
                    temp_fs0 = obj->srt.transl.x - objdata->unk240[k].x;
                    temp_fv0_2 = obj->srt.transl.z - objdata->unk240[k].z;
                    temp_fs0 = sqrtf(SQ(temp_fs0) + SQ(temp_fv0_2));
                    temp_fv1 = (f32) ((ObjType22Setup*)obj->setup)->unk19 * 1.5f;
                    if (temp_fs0 < temp_fv1) {
                        temp_fs0 = ((temp_fv1 - temp_fs0) / temp_fv1);
                        temp_fs0 *= (obj->srt.scale * 10.0f);
                        objdata->unk270[k] += (fsin16_precise(obj->srt.yaw) * temp_fs0);
                        objdata->unk278[k] += (fcos16_precise(obj->srt.yaw) * temp_fs0);
                        sp9C[k] += 1;
                    }
                }
            }
        }
    }

    for (i = 0; i < 2; i++) {
        if (sp9C[i] != 0) {
            objdata->unk270[i] /= sp9C[i];
            objdata->unk278[i] /= sp9C[i];
        }
    }
}

// offset: 0xE8C | func: 23
static void dll_417_func_E8C(Object* arg0) {
    f32 var_fa0;
    f32 var_fa1;
    SRT spE0;
    f32 spDC;
    f32 spD8;
    f32 spD4;
    f32 spD0;
    f32 spC8[2];
    f32 spC4;
    ObjType23Setup* temp_s0;
    f32 spBC;
    Object* temp_a0;
    Vec3f spAC;
    Vec3f spA0;
    f32 sp9C;
    f32 var_fv1;
    s32 sp90[2];
    u8 temp_s1_5;
    u8 temp_s2;
    u8 sp8D;
    s32 i;
    DFlog_Data* objdata;
    f32 pad;

    objdata = (DFlog_Data*)arg0->data;
    sp8D = 0;
    spBC = 10000.0f;
    objdata->unk4F4 = obj_get_nearest_type_to(OBJTYPE_23, arg0, &spBC);
    if (objdata->unk4F4 != NULL) {
        temp_s0 = (ObjType23Setup*)objdata->unk4F4->setup;
        spBC = vec3_distance(&arg0->positionMirror, &objdata->unk4F4->positionMirror);
        if (objdata->unk4EC == 2) {
            var_fv1 = 0.95f;
        } else {
            var_fv1 = 0.5f;
        }
        if (spBC < (f32) temp_s0->unk1A) {
            for (i = 0; i < 2; i++) {
                objdata->unk258[i].x *= var_fv1;
                objdata->unk258[i].z *= var_fv1;
            }
        } else {
            objdata->unk4F4 = NULL;
        }
    }
    if ((objdata->unk4EC == 2) && (gDLL_2_Camera->vtbl->func3() != 0x60)) {
        objdata->unk280 = (f32) ((f32) get_joystick_x(0) * 0.01f);
        if (get_masked_button_presses(0) & A_BUTTON) {
            var_fa0 = ((1.3f - objdata->unk288) / 1.3f) * 0.8f;
        } else {
            var_fa0 = -0.05f;
        }
        objdata->unk288 = (f32) (objdata->unk288 + var_fa0);
        if (get_masked_button_presses(0) & A_BUTTON) {
            objdata->unk258[0].y = (f32) (objdata->unk258[0].y + 0.08f);
            objdata->unk258[1].y = (f32) (objdata->unk258[1].y - 0.06f);
        }
        if (get_masked_button_presses(0) & A_BUTTON) {
            var_fa0 = 0.08f;
        } else {
            var_fa0 = -0.02f;
        }
        objdata->unk284 = (f32) (objdata->unk284 - var_fa0);
        if (objdata->unk288 < 0.0f) {
            objdata->unk288 = 0.0f;
        } else {
            if (objdata->unk288 > 1.3f) {
                var_fa0 = 1.3f;
            } else {
                var_fa0 = objdata->unk288;
            }
            objdata->unk288 = var_fa0;
        }
        if (objdata->unk284 < -0.1f) {
            objdata->unk284 = -0.1f;
        } else {
            if (objdata->unk284 > 0.0f) {
                var_fa0 = 0.0f;
            } else {
                var_fa0 = objdata->unk284;
            }
            objdata->unk284 = var_fa0;
        }
    }
    spC4 = fsin16_precise(arg0->srt.yaw) * objdata->unk284;
    var_fv1 = fcos16_precise(arg0->srt.yaw) * objdata->unk284;
    objdata->unk258[0].x = (f32) (objdata->unk258[0].x + spC4);
    objdata->unk258[0].z = (f32) (objdata->unk258[0].z + var_fv1);
    objdata->unk258[1].x = (f32) (objdata->unk258[1].x + spC4);
    objdata->unk258[1].z = (f32) (objdata->unk258[1].z + var_fv1);
    spC4 = fsin16_precise((s16) (arg0->srt.yaw + 0x4000)) * objdata->unk280;
    var_fv1 = fcos16_precise((s16) (arg0->srt.yaw + 0x4000)) * objdata->unk280;
    objdata->unk258[0].x = (f32) (objdata->unk258[0].x + (spC4 * 0.05f));
    objdata->unk258[0].z = (f32) (objdata->unk258[0].z + (var_fv1 * 0.05f));
    objdata->unk258[1].x = (f32) (objdata->unk258[1].x - (spC4 * 0.025f));
    objdata->unk258[1].z = (f32) (objdata->unk258[1].z - (var_fv1 * 0.025f));
    spD0 = dll_417_func_1E9C(arg0, arg0->srt.transl.y, arg0->srt.transl.x, arg0->srt.transl.z);
    for (i = 0; i < 2; i++) {
        objdata->unk258[i].x -= (objdata->unk270[i] * 0.05f);
        objdata->unk258[i].z -= (objdata->unk278[i] * 0.050f);
        objdata->unk258[i].x *= 0.99f;
        objdata->unk258[i].z *= 0.99f;
        spBC = sqrtf(SQ(objdata->unk258[i].x) + SQ(objdata->unk258[i].z));
        if (spBC > 0.95f) {
            spBC = 0.95f / spBC;
            objdata->unk258[i].x = (f32) (objdata->unk258[i].x * spBC);
            objdata->unk258[i].z = (f32) (objdata->unk258[i].z * spBC);
        }
        spC8[i] = dll_417_func_1E9C(arg0, objdata->unk240[i].y, objdata->unk240[i].x, objdata->unk240[i].z);
        if (((spC8[i] - objdata->unk240[i].y) > -2.0f) && ((spC8[i] - objdata->unk240[i].y) < 2.0f)) {
            var_fa0 = spD0 - arg0->srt.transl.y;
            if ((var_fa0 > -2.0f) && (var_fa0 < 2.0f)) {
                objdata->unk258[i].y += ((spC8[i] - objdata->unk240[i].y) * 0.02f);
                objdata->unk258[i].y *= 0.95f;
                sp90[i] = 1;
                if (objdata->unk4F0[i] > 0) {
                    sp90[i] = (s32) objdata->unk4F0[i];
                }
                objdata->unk4F0[i] = (s16) (objdata->unk4F0[i] >> 1);
            }
        } else {
            if ((spC8[i] - objdata->unk240[i].y) > 2.0f) {
                if (objdata->unk258[i].y > 0.0f) {
                    var_fa0 = objdata->unk258[i].y;
                } else {
                    var_fa0 = -objdata->unk258[i].y;
                }
                objdata->unk258[i].y -= objdata->unk258[i].y * var_fa0 * 0.1f;
                objdata->unk258[i].y += (spC8[i] - objdata->unk240[i].y) * 0.01f;
                sp90[i] = 1;
                if (objdata->unk4F0[i] > 0) {
                    sp90[i] = (s32) objdata->unk4F0[i];
                }
                objdata->unk4F0[i] = (s16) (objdata->unk4F0[i] >> 1);
            } else {
                if ((spC8[i] - objdata->unk240[i].y) < -2.0f) {
                    objdata->unk258[i].y = (f32) (objdata->unk258[i].y * 0.95f);
                    objdata->unk258[i].y = (f32) (objdata->unk258[i].y - 0.18f);
                    objdata->unk4F0[i] = (s16) (objdata->unk4F0[i] + gUpdateRate);
                } else {
                    objdata->unk240[i].y = spC8[i];
                    sp90[i] = 1;
                    if (objdata->unk4F0[i] > 0) {
                        sp90[i] = (s32) objdata->unk4F0[i];
                    }
                    objdata->unk4F0[i] = (s16) (objdata->unk4F0[i] >> 1);
                }
            }
        }
        objdata->unk240[i].x += objdata->unk258[i].x * gUpdateRateF;
        objdata->unk240[i].y += objdata->unk258[i].y * gUpdateRateF;
        objdata->unk240[i].z += objdata->unk258[i].z * gUpdateRateF;
    }
    arg0->srt.transl.x = (objdata->unk240[0].x + objdata->unk240[1].x) * 0.5f;
    arg0->srt.transl.y = (objdata->unk240[0].y + objdata->unk240[1].y) * 0.5f;
    arg0->srt.transl.z = (objdata->unk240[0].z + objdata->unk240[1].z) * 0.5f;
    spD4 = objdata->unk240[1].x - objdata->unk240[0].x;
    spD8 = objdata->unk240[1].y - objdata->unk240[0].y;
    spDC = objdata->unk240[1].z - objdata->unk240[0].z;
    spBC = sqrtf((spD4 * spD4) + (spDC * spDC));
    arg0->srt.pitch = -arctan2_f(spD8, spBC);
    arg0->srt.yaw = arctan2_f(spD4, spDC);
    gDLL_27->vtbl->func_1E8(arg0, &objdata->unk28C, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(arg0, &objdata->unk28C);
    gDLL_27->vtbl->func_624(arg0, &objdata->unk28C, gUpdateRateF);
    temp_s2 = (objdata->unk28C.hitsTouchBits | objdata->unk28C.unk25C) & 3;
    temp_s1_5 = temp_s2 & (temp_s2 ^ objdata->unk4EE);
    if (temp_s1_5 & 1) {
        sp8D = (u8) (u32) ((sqrtf(SQ(objdata->unk258[0].x) + SQ(objdata->unk258[0].z)) * 127.0f) / 0.95f);
    }
    if (temp_s1_5 & 2) {
        sp9C = sqrtf(SQ(objdata->unk258[1].x) + SQ(objdata->unk258[1].z));
        if (((sp9C * 127.0f) / 0.95f) < (f32) sp8D) {
            sp8D = (u8) (f32) sp8D;
        } else {
            sp8D = (u8) ((sqrtf(SQ(objdata->unk258[1].x) + SQ(objdata->unk258[1].z)) * 127.0f) / 0.95f);
        }
    }
    if ((s32) sp8D >= 0xB) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_76D, sp8D, NULL, NULL, 0, NULL);
    }
    objdata->unk4EE = temp_s2;
    if ((objdata->unk4EC == 2) && (sp90[0] != 0) && (sp90[1] != 0)) {
        temp_a0 = get_player()->linkedObject;
        if (temp_a0 != NULL) {
            ((DLL_Unknown*)temp_a0->dll)->vtbl->func[17].withThreeArgs((s32)temp_a0, (s32)&spAC, (s32)&spA0);
            sp9C = (spAC.y - spD0) / (spAC.y - spA0.y);
            if ((sp9C >= 0.0f) && (sp9C <= 1.0f)) {
                if (objdata->unk4ED == 0) {
                    gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_8F, 0x7F, NULL, NULL, 0, NULL);
                    objdata->unk4ED = 2;
                }
                spE0.transl.x = ((spA0.x - spAC.x) * sp9C) + spAC.x;
                spE0.transl.y = spC8[0];
                spE0.transl.z = ((spA0.z - spAC.z) * sp9C) + spAC.z;
                var_fv1 = sqrtf(SQ(objdata->unk258[i].x) + SQ(objdata->unk258[i].z)) * 5.0f;
                if (var_fv1 < 0.0f) {
                    spE0.scale = 0.0f;
                } else {
                    if (var_fv1 > 1.0f) {
                        var_fa0 = 1.0f;
                    } else {
                        var_fa0 = var_fv1;
                    }
                    spE0.scale = var_fa0;
                }
                spE0.yaw = (arctan2_f(objdata->unk240[0].x - objdata->unk240[1].x, objdata->unk240[0].z - objdata->unk240[1].z) + 0x8000) & 0xFFFF & 0xFFFF;
                spE0.transl.x -= arg0->srt.transl.x;
                spE0.transl.y -= arg0->srt.transl.y;
                spE0.transl.z -= arg0->srt.transl.z;
                gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_3C4, &spE0, PARTFXFLAG_NONE, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_3C5, &spE0, PARTFXFLAG_NONE, -1, NULL);
            } else {
                if (objdata->unk4ED != 0) {
                    objdata->unk4ED >>= 1;
                }
            }
        }
    }
    
    for (i = 0; i < 2; i++) {
        if (sp90[i] != 0) {
            if (sp90[i] >= 0xB) {
                if (arg0->srt.transl.y != objdata->unk240[i].y) {
                    sp9C = (objdata->unk240[i].y - spC8[i]) / (objdata->unk240[i].y - arg0->srt.transl.y);
                } else {
                    sp9C = 1.0f;
                }
                if (sp9C < 0.0f){
                    sp9C = 0.0f;
                }
                if (sp9C > 1.0f) {
                    sp9C = 1.0f;
                }
            }
            if (rand_next(0, 5) == 0) {
                spE0.transl.x = objdata->unk240[i].x;
                spE0.scale = sqrtf(SQ(objdata->unk258[i].x) + SQ(objdata->unk258[i].z));
                if (spE0.scale > 0.1f) {
                    spE0.transl.y = arg0->srt.transl.y;
                    spE0.transl.z = objdata->unk240[i].z;
                    spE0.yaw = (arctan2_f(objdata->unk258[i].x, objdata->unk258[i].z) + 0x8000) & 0xFFFF & 0xFFFF;
                    spE0.transl.x -= arg0->srt.transl.x;
                    spE0.transl.y -= arg0->srt.transl.y;
                    spE0.transl.z -= arg0->srt.transl.z;
                    gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_3C4, &spE0, PARTFXFLAG_NONE, -1, NULL);
                }
            }
        }
    }
}

// offset: 0x1E9C | func: 24
static f32 dll_417_func_1E9C(Object* arg0, f32 arg1, f32 arg2, f32 arg3) {
    Func_80057F1C_Struct** sp74;
    Func_80057F1C_Struct* temp_v0_4;
    f32 temp_fv0;
    f32 sp68;
    f32 temp_fv0_9;
    f32 var_fa1;
    f32 var_ft4;
    f32 var_fv0_2;
    f32 var_fv1;
    s32 temp_v0;
    s32 var_a1;
    s32 var_a3;
    s32 var_v1;

    temp_fv0 = 2.0f * arg1;
    if (temp_fv0 >= 0.0f) {
        sp68 = -temp_fv0;
    } else {
        sp68 = -(-temp_fv0);
    }
    temp_v0 = func_80057F1C(arg0, arg2, arg0->srt.transl.y, arg3, &sp74, 0, 0);
    if (temp_v0 != 0) {
        var_a3 = 0;
        var_ft4 = 0.0f;
        var_a1 = -1;
        if (sp74[0]->unk0[0] <= arg1) {
            var_fa1 = arg1 - sp74[0]->unk0[0];
        } else {
            var_fa1 = -(arg1 - sp74[0]->unk0[0]);
        }

        for (var_v1 = 0; var_v1 < temp_v0; var_v1++) {
            if (sp74[var_v1]->unk14 == 0xE) {
                if (sp74[var_v1]->unk0[0] <= arg1) {
                    var_fv1 = arg1 - sp74[var_v1]->unk0[0];
                } else {
                    var_fv1 = -(arg1 - sp74[var_v1]->unk0[0]);
                }
                if ((var_fv1 < var_ft4) || (var_a1 == -1)) {
                    var_ft4 = var_fv1;
                    var_a1 = var_v1;
                }
            }
            var_fv1 = arg1 - sp74[var_v1]->unk0[0];
            if (sp74[var_v1]->unk0[0] <= arg1) {
                var_fv0_2 = var_fv1;
            } else {
                var_fv0_2 = -var_fv1;
            }
            if (var_fv0_2 < var_fa1) {
                var_fa1 = var_fv0_2;
                var_a3 = var_v1;
            }
        }
        
        if (var_a1 != -1) {
            sp68 = sp74[var_a1]->unk0[0];
        }
        temp_v0_4 = sp74[var_a3];
        temp_fv0_9 = arg1 - temp_v0_4->unk0[0];
        if (sp68 < temp_v0_4->unk0[0]) {
            if (temp_fv0_9 >= 0.0f) {
                var_fv1 = temp_fv0_9;
            } else {
                var_fv1 = -temp_fv0_9;
            }
            if ((var_fv1 < 10.0f) && (temp_v0_4->unk14 != 0xE)) {
                sp68 = temp_v0_4->unk0[0] + 4.0f;
            }
        }
    } else {
        sp68 = arg1;
    }
    return sp68;
}

// offset: 0x21E4 | func: 25 | export: 20
void dll_417_func_21E4(UNK_TYPE_32 a0, UNK_TYPE_32 a1, UNK_TYPE_32 a2) { }
