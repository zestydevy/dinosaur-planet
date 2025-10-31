#include "dlls/objects/210_player.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"

typedef struct {
    UnkCurvesStruct unk0;
    u8 unk108;
    u8 unk109;
    u8 unk10A;
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    f32 unk118;
    f32 unk11C;
    UnkFunc_80024108Struct unk120;
    s16 unk13C;
    s16 unk13E;
    f32 unk140;
} CCgrub_Data;

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
} CCgrub_Setup;

static void CCgrub_func_AB0(Object *self, CCgrub_Data *objdata, f32 a2);
static void CCgrub_func_BE8(Object* self, CCgrub_Data* objdata, f32 a2, f32 a3);
static void CCgrub_func_CEC(Object* self, f32* a1, f32 a2, f32* a3);
static void CCgrub_func_DC4(Object *self, CCgrub_Data *objdata);
static void CCgrub_func_104C(Object* self, CCgrub_Data* objdata);

// offset: 0x0 | ctor
void CCgrub_ctor(void *dll) { }

// offset: 0xC | dtor
void CCgrub_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCgrub_setup(Object* self, CCgrub_Setup* setup, s32 arg2) {
    CCgrub_Data* sp3C;
    s32 _pad;
    CurveSetup* sp34;
    CurveSetup* temp_v0_2;
    CurveSetup* temp_v0_3;

    if (setup->base.uID != 0) {
        if (setup->unk19 < setup->unk18) {
            setup->unk19 = setup->unk18;
        }
        sp3C = self->data;
        temp_v0_2 = gDLL_26_Curves->vtbl->curves_func_39c(gDLL_26_Curves->vtbl->curves_func_1e4(self->srt.transl.f[0], self->srt.transl.f[1], self->srt.transl.f[2], NULL, -1, -1));
        sp34 = gDLL_26_Curves->vtbl->curves_func_39c(gDLL_26_Curves->vtbl->curves_func_438(temp_v0_2, -1));
        gDLL_25->vtbl->func_1D30(&sp3C->unk0, temp_v0_2, sp34, gDLL_26_Curves->vtbl->curves_func_39c(gDLL_26_Curves->vtbl->curves_func_438(sp34, -1)));
        CCgrub_func_AB0(self, sp3C, 0.2f);
        func_800053B0(&sp3C->unk0, 0.1f);
        self->srt.transl.x = temp_v0_2->base.x;
        self->srt.transl.y = temp_v0_2->base.y;
        self->srt.transl.z = temp_v0_2->base.z;
        self->ptr0x64->flags |= 0x20200;
        obj_init_mesg_queue(self, 1);
    }
}

// offset: 0x214 | func: 1 | export: 1
void CCgrub_control(Object* self) {
    Object* temp_v0;
    CCgrub_Setup* objsetup;
    Object* temp_v0_4;
    s32 sp60;
    u32 sp5C;
    f32 var_ft5;
    Object* sp54;
    f32 var_fv0;
    s32 var_v0_2;
    s16 var_v1;
    f32 sp40[2];
    CCgrub_Data* objdata;

    sp60 = -1;
    objdata = (CCgrub_Data*)self->data;
    objsetup = (CCgrub_Setup*)self->setup;
    while (obj_recv_mesg(self, &sp5C, NULL, NULL) != 0) {
        if (sp5C == 0x7000B) {
            temp_v0 = get_player();
            sp54 = ((DLL_210_Player*)temp_v0->dll)->vtbl->func66(temp_v0, 0x10);
            if (((DLL_Unknown*)sp54->dll)->vtbl->func[7].withOneArgS32((s32)sp54) != 0) {
                ((DLL_Unknown*)sp54->dll)->vtbl->func[11].withTwoArgs((s32)sp54, 8);
            } else {
                gDLL_1_UI->vtbl->func_69F8(0x23, 0x12c, main_increment_bits(0x23));
            }
            objdata->unk109 = 0xA;
        }
    }
    if ((func_80025F40(self, NULL, NULL, NULL) != 0) && (objdata->unk109 != 0)) {
        func_80023D30(self, 0, 0.0f, 0);
        objdata->unk110 = 0.005f;
        objdata->unk109 = 0;
    }
    if ((objdata->unk0.unk80 == 0) && (objdata->unk0.unk10 != 0)) {
        sp60 = gDLL_26_Curves->vtbl->curves_func_438(objdata->unk0.unkA4, -1);
    } else if ((objdata->unk0.unk80 != 0) && (objdata->unk0.unk10 == 0)) {
        sp60 = gDLL_26_Curves->vtbl->curves_func_590(objdata->unk0.unkA4, -1);
    }
    if (sp60 >= 0) {
        gDLL_25->vtbl->func_21F4(&objdata->unk0, gDLL_26_Curves->vtbl->curves_func_39c(sp60));
    }
    switch (objdata->unk109) {
    case 0:
    case 8:
    case 9:
    case 10:
        break;
    case 1:
        CCgrub_func_DC4(self, objdata);
        break;
    case 2:
        if (self->animProgress >= 0.9f) {
            objdata->unk10C = ((f32) rand_next(objsetup->unk18, objsetup->unk19) / 100.0f);
            objdata->unk110 = 0.02f;
            func_80023D30(self, 5, 0.0f, 0);
            objdata->unk109 = 4;
        }
        break;
    case 3:
        if (self->animProgress > 0.05f) {
            if (self->animProgress >= 0.95f) {
                CCgrub_func_AB0(self, objdata, (f32) objsetup->unk19 / 50.0f);
                CCgrub_func_DC4(self, objdata);
            } else if (self->animProgress > 0.8f) {
                self->srt.yaw = objdata->unk13C;
            } else if (self->animProgress > 0.5f) {
                var_v1 = (objdata->unk13C & 0xFFFF) - objdata->unk13E;
                CIRCLE_WRAP(var_v1);
                var_v0_2 = var_v1 >= 0 ? var_v1 : -var_v1;
                if (var_v0_2 < 0x4000) {
                    objdata->unk13C += 0x8000;
                    if (objdata->unk0.unk80 != 0) {
                        var_fv0 = 10.0f;
                    } else {
                        var_fv0 = -10.0f;
                    }
                    func_800053B0(&objdata->unk0, var_fv0);
                    var_v1 = (objdata->unk13C) - (objdata->unk13E & 0xFFFF);
                    CIRCLE_WRAP(var_v1);
                }
                var_fv0 = (self->animProgress - 0.5f) / 0.3f;
                self->srt.yaw = objdata->unk13E + (var_fv0 * var_v1);
            } else if (self->animProgress > 0.35f) {
                self->srt.yaw = objdata->unk13E;
                if (objdata->unk10A == 0) {
                    objdata->unk10A = 1;
                }
            } else {
                if (objdata->unk13E == objdata->unk13C) {
                    temp_v0_4 = get_player();
                    sp40[0] = temp_v0_4->srt.transl.x - self->srt.transl.x;
                    sp40[1] = temp_v0_4->srt.transl.z - self->srt.transl.z;
                    objdata->unk13E = arctan2_f(-sp40[0], -sp40[1]);
                }
                var_v1 = objdata->unk13C - (objdata->unk13E & 0xFFFF);
                CIRCLE_WRAP(var_v1);
                var_fv0 = (self->animProgress - 0.05f) / 0.3f;
                self->srt.yaw = objdata->unk13C - (var_fv0 * var_v1);
            }
        }
        break;
    case 4:
        if (self->animProgress > 0.9f) {
            CCgrub_func_AB0(self, objdata, objdata->unk10C);
            CCgrub_func_DC4(self, objdata);
        }
        break;
    case 5:
        if (self->animProgress > 0.8f) {

        }
        break;
    case 6:
        if (self->animProgress > 0.95f) {
            objdata->unk109 = 7;
            objdata->unk10C = 0.5f;
            objdata->unk11C = 0.0f;
            CCgrub_func_104C(self, objdata);
        }
        break;
    case 7:
        CCgrub_func_104C(self, objdata);
        break;
    }
    func_80024108(self, objdata->unk110, gUpdateRateF, &objdata->unk120);
    objdata->unk120.unk0[2] = -(objdata->unk120.unk0[2] / gUpdateRateF);
    if (objdata->unk109 != 6) {
        CCgrub_func_BE8(self, objdata, objdata->unk10C, 1.0f);
        CCgrub_func_CEC(self, &objdata->unk0.unk68, objdata->unk120.unk0[2], &objdata->unk114);
    }
    if (self->unkAF & 1) {
        // @bug: the gamebit used here is also used for disabling particles!
        obj_send_mesg(get_player(), 0x7000A, self, (void* )BIT_5);
        if (self->curModAnimId != 0) {
            func_80023D30(self, 0, 0.0f, 0);
            objdata->unk110 = 0.005f;
        }
        objdata->unk109 = 9;
    }
}

// offset: 0xAB0 | func: 2
static void CCgrub_func_AB0(Object* self, CCgrub_Data* objdata, f32 a2) {
    if (a2 < 1.0f) {
        func_80023D30(self, 1, 0.0f, 0);
    } else {
        func_80023D30(self, 2, 0.0f, 0);
    }
    objdata->unk109 = 1;
    objdata->unk10C = a2;
    objdata->unk11C = 0.0f;
    objdata->unk140 = 0.0f;
}

// offset: 0xB54 | func: 3 | export: 2
void CCgrub_update(Object *self) { }

// offset: 0xB60 | func: 4 | export: 3
void CCgrub_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xBB4 | func: 5 | export: 4
void CCgrub_free(Object *self, s32 a1) { }

// offset: 0xBC4 | func: 6 | export: 5
u32 CCgrub_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xBD4 | func: 7 | export: 6
u32 CCgrub_get_data_size(Object *self, u32 a1) {
    return sizeof(CCgrub_Data);
}

// offset: 0xBE8 | func: 8
static void CCgrub_func_BE8(Object* self, CCgrub_Data* objdata, f32 a2, f32 a3) {
    f32 temp_fs1;
    f32 var_fs0;
    f32 var_fv0;
    f32 var_fv1;
    s32 i;

    temp_fs1 = SQ(a2 * gUpdateRateF);
    var_fv0 = objdata->unk0.unk68 - self->srt.transl.x;
    var_fv1 = objdata->unk0.unk70 - self->srt.transl.z;

    var_fs0 = objdata->unk0.unk80 != 0 ? -a3 : a3;
    
    for (i = 0; i < 5; i++) {
        if ((temp_fs1 < (SQ(var_fv0) + SQ(var_fv1)))) {
            break;
        }

        func_800053B0(&objdata->unk0, var_fs0);

        var_fv0 = objdata->unk0.unk68 - self->srt.transl.x;
        var_fv1 = objdata->unk0.unk70 - self->srt.transl.z;
    }
}

// offset: 0xCEC | func: 9
static void CCgrub_func_CEC(Object* self, f32* a1, f32 a2, f32* a3) {
    f32 temp_fa1;
    f32 temp_ft4;
    f32 temp_fv0;
    f32 temp_fv1;

    temp_fa1 = a1[0] - self->srt.transl.x;
    temp_ft4 = a1[2] - self->srt.transl.z;
    temp_fv0 = sqrtf(SQ(temp_fa1) + SQ(temp_ft4));
    if (temp_fv0 != 0.0f) {
        temp_fv1 = temp_fa1 / temp_fv0;
        a3[0] = temp_fv1;
        a3[1] = (f32) (temp_ft4 / temp_fv0);
        self->srt.transl.x += temp_fv1 * a2 * gUpdateRateF;
        self->srt.transl.z += a3[1] * a2 * gUpdateRateF;
    }
}

// offset: 0xDC4 | func: 10
static void CCgrub_func_DC4(Object* self, CCgrub_Data* objdata) {
    CCgrub_Setup* objsetup;
    f32 temp_fv0;

    objsetup = (CCgrub_Setup*)self->setup;
    objdata->unk140 += (objdata->unk10C * gUpdateRateF);
    if (objdata->unk140 > 10.0f) {
        objdata->unk140 -= 10.0f;
    }
    func_8002493C(self, objdata->unk10C, &objdata->unk110);
    self->srt.transl.y = objdata->unk0.unk6C;
    self->srt.yaw = arctan2_f(-objdata->unk114, -objdata->unk118);
    objdata->unk11C += gUpdateRateF;
    if (objdata->unk11C >= 300.0f) {
        temp_fv0 = vec3_distance_xz_squared(&self->positionMirror, &get_player()->positionMirror);
        if ((f32) (objsetup->unk1B * objsetup->unk1B) <= temp_fv0) {
            if ((f32) (objsetup->unk1A * objsetup->unk1A) <= temp_fv0) {
                func_80023D30(self, 3, 0.0f, 0);
                objdata->unk110 = 0.01f;
                objdata->unk109 = 2;
                return;
            }
            objdata->unk13C = self->srt.yaw;
            objdata->unk13E = self->srt.yaw;
            func_80023D30(self, 6, 0.0f, 0);
            if (objdata->unk10C == ((f32) objsetup->unk19 / 50.0f)) {
                objdata->unk110 = 0.015f;
            } else {
                objdata->unk110 = 0.01f;
            }
            objdata->unk109 = 3;
            objdata->unk10A = 0;
            return;
        }
        self->unkAF |= 8;
        func_80023D30(self, 4, 0.0f, 0);
        objdata->unk110 = 0.02f;
        objdata->unk109 = 6;
    }
}

// offset: 0x104C | func: 11
void CCgrub_func_104C(Object* self, CCgrub_Data* objdata) {
    CCgrub_Setup *setup = (CCgrub_Setup*)self->setup;
    
    objdata->unk11C += gUpdateRateF;
    if (objdata->unk11C >= 300.0f) {
        self->unkAF &= ~0x8;
        CCgrub_func_AB0(self, objdata, (f32) rand_next(setup->unk18, setup->unk19) / 100.0f);
        CCgrub_func_DC4(self, objdata);
        return;
    }
    CCgrub_func_BE8(self, objdata, objdata->unk10C, 1.0f);
    CCgrub_func_CEC(self, &objdata->unk0.unk68, objdata->unk10C, &objdata->unk114);
    self->srt.transl.y = objdata->unk0.unk6C;
    self->srt.yaw = arctan2_f(-objdata->unk114, -objdata->unk118);
}

/*0x0*/ static const char str_0[] = "msg %d\n";
/*0x8*/ static const char str_8[] = "grub has been collected\n";
/*0x24*/ static const char str_24[] = "adding grub to foodbag\n";
/*0x3C*/ static const char str_3C[] = "increasing grub bit\n";
