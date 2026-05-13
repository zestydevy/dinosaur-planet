#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/53.h"
#include "dlls/objects/common/sidekick.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/animseq.h"
#include "sys/objtype.h"

typedef struct {
    u8 _unk0[0x4A9 - 0x0];
    u8 unk4A9;
    u8 _unk4AA[0x4B8 - 0x4AA];
    s8 unk4B8;
    u8 unk4B9;
    u8 unk4BA;
    u8 unk4BB;
    s8 unk4BC;
    u8 unk4BD;
    u8 _unk4BE[0x4C0 - 0x4BE];
    u8* unk4C0;
    u8 unk4C4;
    u8 _unk4C5[0x4D0 - 0x4C5];
    s16 unk4D0;
    s16 unk4D2;
    s16 unk4D4;
    s16 unk4D6;
    s16 unk4D8;
    s16 unk4DA;
    s16 unk4DC;
    s32 unk4E0;
    DLL27_Data unk4E4;
    u8 _unk744[0x75C - 0x744];
    CurveSetup* unk75C;
    CurveSetup* unk760;
    CurveSetup* unk764;
    u8 _unk768[0x804 - 0x768];
    f32 unk804;
    f32 unk808;
    u8 _unk80C[0x840 - 0x80C];
    f32 unk840;
    f32 unk844;
    f32 unk848;
    f32 unk84C;
    HeadAnimation unk850;
    u8 unk874;
    u8 _unk875[0x878 - 0x875];
} SHthorntail_Data;

typedef struct {
    ObjSetup base;
    u8 unk18;
} SHthorntail_Setup;

/*0x0*/ static Vec3f data_0[] = {
    VEC3F(-8.0f, 0.0f, -8.0f), 
    VEC3F(8.0f, 0.0f, -8.0f), 
    VEC3F(8.0f, 0.0f, 8.0f), 
    VEC3F(-8.0f, 0.0f, 8.0f)
};
/*0x30*/ static f32 data_30[] = {0.0f, 0.0f, 0.0f, 0.0f};
/*0x40*/ static Unk80026DF4 data_40[] = {
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}, 
    {SOUND_377_Metal_Smack, -1, -1, -1, 0, 0.0f, 0.0f}
};
/*0x16C*/ static s16 data_16C[] = {
    0x0007, 0xffff, 
    0x0008, 0xffff, 
    0x0009, 0xffff, 
    0x000f, 0x0001, 
    0x000c, 0x0000, 
    0x000d, 0x0000, 
    0x0010, 0xffff, 
    0x0011, 0xffff,
    0x0012, 0xffff, 
    0x0000, 0xffff, 
    0x0004, 0xffff, 
    0x0005, 0xffff, 
    0x0013, 0xffff, 
    0x0004, 0xffff, 
    0x0003, 0xffff
};
/*0x1A8*/ static u8 data_1A8[] = {
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x03, 0x01, 
    0x00, 0x03, 0x03, 0x00
};
/*0x1B8*/ static f32 data_1B8[] = {
    0.006f, 0.004f, 0.006f, 0.0065f, 0.007f, 0.007f, 0.006f, 0.006f, 
    0.006f, 0.006f, 0.006f, 0.006f, 0.003f, 0.006f, 0.006f
};
/*0x1F4*/ static u32 data_1F4 = 0x00000000;
/*0x1F8*/ static u32 data_1F8 = 0x01020300;
/*0x1FC*/ static u32 data_1FC[] = {
    0x08000000, 0x04050607, 0xffffffff, 0xffff0000, 0xffffffff, 0xffff0000, 0xffffffff, 0xffff0000, 
    0xffffffff, 0xffff0000
};

static void dll_508_func_584(Object* self);
static void dll_508_func_B40(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static int dll_508_func_8CC(Object *actor, Object *animObj, AnimObj_Data *animObjData, s8 a3);
static void dll_508_func_9B8(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void dll_508_func_A04(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void dll_508_func_BEC(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void dll_508_func_19E0(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void dll_508_func_1B9C(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void dll_508_func_1E4C(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void dll_508_func_1F28(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void dll_508_func_1F3C(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void dll_508_func_1F50(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void dll_508_func_1F64(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void dll_508_func_19F4(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void dll_508_func_1A08(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void dll_508_func_1A1C(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void dll_508_func_1A58(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void dll_508_func_1C48(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void dll_508_func_1D70(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void dll_508_func_1D84(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void dll_508_func_1D98(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void dll_508_func_1DAC(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void dll_508_func_1DF8(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);

// offset: 0x0 | ctor
void dll_508_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_508_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
// @bug: Missing 3rd param
#ifndef AVOID_UB
void dll_508_setup(Object *self, SHthorntail_Setup *setup/*, s32 reset*/) {
#else
void dll_508_setup(Object *self, SHthorntail_Setup *setup, s32 reset) {
#endif
    SHthorntail_Data *objdata = self->data;
    s32 _pad;
    u8 sp3C[] = {1, 1, 1, 1}; // data_224

    switch (setup->unk18) {
    case 1:
        dll_508_func_9B8(self, objdata, setup);
        break;
    case 2:
        dll_508_func_1A1C(self, objdata, setup);
        break;
    case 3:
        dll_508_func_1DAC(self, objdata, setup);
        break;
    }
    gDLL_27->vtbl->init(&objdata->unk4E4, 0x06000000, 0x400, 1);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->unk4E4, 4, data_0, data_30, sp3C);
    gDLL_27->vtbl->reset(self, &objdata->unk4E4);
    self->shadow->flags |= 0xA30;
    self->shadow->distFadeMaxOpacity = 0x80;
    self->shadow->distFadeMinOpacity = 0x5A;
    self->animCallback = dll_508_func_8CC;
    objdata->unk4B8 = -1;
    create_temp_dll(53);
    ((DLL_53*)gTempDLLInsts[1])->vtbl->func2(self, objdata, -0x1FFF, 0x2AAA, 3);
    ((DLL_53*)gTempDLLInsts[1])->vtbl->func5(objdata, 0x190, 0x1E);
    objdata->unk4A9 &= ~0x8;
    obj_add_object_type(self, OBJTYPE_40);
}

// offset: 0x248 | func: 1 | export: 1
void dll_508_control(Object* self) {
    SHthorntail_Data* objdata;
    SHthorntail_Setup* setup;
    Object* player;

    objdata = self->data;
    setup = (SHthorntail_Setup*)self->setup;
    player = get_player();
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    if (func_80026DF4(self, data_40, 15, objdata->unk874, &objdata->unk84C) == 0) {
        objdata->unk874 = 0;
        objdata->unk4BB = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
        objdata->unk840 = vec3_distance(&self->globalPosition, &player->globalPosition);
        switch (setup->unk18) {
        case 1:
            dll_508_func_B40(self, objdata, setup);
            break;
        case 2:
            dll_508_func_1B9C(self, objdata, setup);
            break;
        case 3:
            dll_508_func_1E4C(self, objdata, setup);
            break;
        }
        dll_508_func_584(self);
        gDLL_27->vtbl->func_1E8(self, &objdata->unk4E4, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, &objdata->unk4E4);
        gDLL_27->vtbl->func_624(self, &objdata->unk4E4, gUpdateRateF);
        if (!(data_1A8[objdata->unk4B8] & 2)) {
            func_80032A08(self, &objdata->unk850);
        }
    }
}

// offset: 0x474 | func: 2 | export: 2
void dll_508_update(Object *self) { }

// offset: 0x480 | func: 3 | export: 3
void dll_508_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    SHthorntail_Data *objdata;

    objdata = self->data;
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        ((DLL_53*)gTempDLLInsts[1])->vtbl->func3(self, objdata, 0);
    }
}

// offset: 0x504 | func: 4 | export: 4
void dll_508_free(Object *self, s32 onlySelf) {
    remove_temp_dll(53);
    obj_free_object_type(self, OBJTYPE_40);
}

// offset: 0x560 | func: 5 | export: 5
u32 dll_508_get_model_flags(Object *self) {
    return MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x570 | func: 6 | export: 6
u32 dll_508_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(SHthorntail_Data);
}

// offset: 0x584 | func: 7
static void dll_508_func_584(Object *self) {
    MtxF mtx;

    matrix_from_srt(&mtx, &self->srt);
    vec3_transform(&mtx, 0.0f, 0.0f, 0.0f, &self->shadow->tr.x, &self->shadow->tr.y, &self->shadow->tr.z);
}

// offset: 0x608 | func: 8
static CurveSetup* dll_508_func_608(f32 arg0, f32 arg1, f32 arg2, s32 arg3) {
    CurveNode* temp_v0;
    CurveSetup* temp_a0_2;
    CurveSetup* var_t0;
    f32 var_fa1;
    s32 var_a2;
    f32 var_fv0;
    f32 var_fv1;
    f32 var_fa0;
    s32 sp4C;

    temp_v0 = gDLL_26_Curves->vtbl->func_1BC(&sp4C);
    var_t0 = NULL;
    var_fa1 = 2.5e9f;
    for (var_a2 = 0; var_a2 < sp4C; var_a2++) {
        temp_a0_2 = temp_v0[var_a2].setup;
        if ((temp_a0_2->curveType == 0x1D) && (arg3 == temp_a0_2->type1D.unk34)) {
            var_fv0 = temp_a0_2->pos.x - arg0;
            var_fv1 = temp_a0_2->pos.y - arg1;
            var_fa0 = temp_a0_2->pos.z - arg2;
            var_fv0 = SQ(var_fv0) + SQ(var_fv1) + SQ(var_fa0);
            if (var_fv0 < var_fa1) {
                var_fa1 = var_fv0;
                var_t0 = temp_a0_2;
            }
        }
    }
    return var_t0;
}

// offset: 0x8CC | func: 9
static int dll_508_func_8CC(Object *actor, Object *animObj, AnimObj_Data *animObjData, s8 a3) {
    SHthorntail_Data *objdata = actor->data;

    if (((DLL_53*)gTempDLLInsts[1])->vtbl->func4(actor, animObjData, objdata, 1, 1) != 0) {
        return 1;
    }
    if (a3 != 0) {
        func_80024108(actor, data_1B8[objdata->unk4B8], gUpdateRateF, NULL);
    }
    animObjData->unk62 = 0;
    animObjData->unk7A &= ~(ANIM7AFLAG_OVERRIDE_EYES | ANIM7AFLAG_OVERRIDE_HEAD);
    actor->unkAF |= ARROW_FLAG_8_No_Targetting;
    return 0;
}

// offset: 0x9B8 | func: 10
static void dll_508_func_9B8(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    objdata->unk4BA |= 0x40;
    objdata->unk4DC = 0xBF;
    dll_508_func_A04(self, objdata, setup);
}

// offset: 0xA04 | func: 11
static void dll_508_func_A04(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    s32 var_a3;

    if (main_get_bits(objdata->unk4DC) == 0) {
        objdata->unk4BA |= 0x20;
        var_a3 = 3;
    } else {
        objdata->unk4DC = -1;
        var_a3 = 8;
    }
    objdata->unk760 = dll_508_func_608(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, var_a3);
    if (objdata->unk760 == NULL) {
        STUBBED_PRINTF("THORNTAIL: cannot find a node\n");
    }
    objdata->unk4BA |= 2;
    self->srt.transl.x = objdata->unk760->pos.x;
    self->srt.transl.y = objdata->unk760->pos.y;
    self->srt.transl.z = objdata->unk760->pos.z;
    self->srt.yaw = objdata->unk760->unk2C << 8;
    objdata->unk4D0 = rand_next(0x1F4, 0x320);
    objdata->unk4D2 = rand_next(0x7D0, 0xBB8);
    objdata->unk4BC = rand_next(1, 2);
    objdata->unk4C0 = (u8 *) &data_1F4;
    objdata->unk4C4 = 1;
}

// offset: 0xB40 | func: 12
static void dll_508_func_B40(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup) {
    switch (objdata->unk4BB) {
    default:
    case 1:
        dll_508_func_BEC(self, objdata, setup);
        return;
    case 2:
        dll_508_func_19E0(self, objdata, setup);
        return;
    case 3:
        dll_508_func_19F4(self, objdata, setup);
        return;
    case 4:
        dll_508_func_1A08(self, objdata, setup);
        return;
    }
}

/*0x20*/ static const char str_20[] = "THORNTAIL: help cannot find a node\n";

// offset: 0xBEC | func: 13
static void dll_508_func_BEC(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup) {
    s32 var_a1;
    s32 var_a0;
    s32 var_a3;
    s32 sp90;
    s32 sp80[4];
    s32 i;
    CurveSetup* temp_v0_3;
    CurveSetup* sp68[4];
    TextureAnimator* temp_v0_6;
    f32 var_fv1;
    f32 var_fv0;
    f32 sp58;
    UnkFunc_80024108Struct sp3C;
    TextureAnimator* sp38;
    s32 temp;

    objdata->unk4BA &= ~0x4;
    if (self->unkAF & 1) {
        gDLL_3_Animation->vtbl->start_obj_sequence(objdata->unk4C0[objdata->unk4BD], self, -1);
        if (objdata->unk4BA & 0x40) {
            objdata->unk4BD++;
            if (objdata->unk4BD >= objdata->unk4C4) {
                objdata->unk4BD = 0;
            }
        }
        return;
    }
    if (objdata->unk4BA & 0x10) {
        if ((objdata->unk75C != NULL) && (objdata->unk760->type1D.unk34 != 4)) {
            var_a1 = objdata->unk75C->uID;
        } else {
            var_a1 = -1;
        }
        var_a3 = gDLL_26_Curves->vtbl->func_4F0(objdata->unk760, var_a1, sp80);
        sp90 = 0;
        for (i = 0; i < var_a3; i++) {
            temp_v0_3 = gDLL_26_Curves->vtbl->func_39C(sp80[i]);
            if (temp_v0_3 != NULL) {
                var_a0 = 0;
                switch (objdata->unk4E0) {
                case 9:
                    if ((temp_v0_3->type1D.unk34 == 9) || (temp_v0_3->type1D.unk34 == 4) || (temp_v0_3->type1D.unk34 == 0)) {
                        var_a0 = 1;
                    }
                    break;
                case 8:
                default:
                    if ((temp_v0_3->type1D.unk34 == 8) || (temp_v0_3->type1D.unk34 == 9) || (temp_v0_3->type1D.unk34 == 0)) {
                        var_a0 = 1;
                    }
                    break;
                }
                if (var_a0 != 0) {
                    sp68[sp90] = temp_v0_3;
                    sp90 += 1;
                }
            }
        }
        if (sp90 != 0) {
            objdata->unk764 = sp68[rand_next(0, sp90 - 1)];
            objdata->unk75C = objdata->unk760;
            objdata->unk4BA &= ~0x2;
        } else {
            // STUBBED_PRINTF("Thorntail %d, is on a network with a deadend\n", setup->base.uID); // default.dol
            objdata->unk764 = dll_508_func_608(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, 0);
            if (objdata->unk764 != NULL) {
                objdata->unk75C = objdata->unk760;
                objdata->unk4BA &= ~0x2;
            }
        }
        objdata->unk4BA &= ~0x10;
    }
    if (objdata->unk4BA & 2) {
        objdata->unk4D2 -= gUpdateRate;
        if (objdata->unk4D2 < 0) {
            objdata->unk4D2 = 0;
        }
        if (!(data_1A8[objdata->unk4B8] & 2) && (gDLL_7_Newday->vtbl->func8(&sp58) != 0)) {
            objdata->unk4B8 = 0xE;
        }
        switch (objdata->unk4B8) {
        case 0:
            objdata->unk4D0 -= gUpdateRate;
            if (objdata->unk4D0 < 0) {
                objdata->unk4D0 = 0;
            }
            if ((objdata->unk4BA & 1) && (objdata->unk4D0 <= 0)) {
                if (objdata->unk4BC <= 0) {
                    objdata->unk4B8 = 2;
                } else {
                    objdata->unk4B8 = 1;
                }
            }
            if (objdata->unk4D2 <= 0) {
                if (objdata->unk4E0 == 9) {
                    objdata->unk4BA |= 0x10;
                } else {
                    objdata->unk4B8 = 0xC;
                }
            } else if (joy_get_pressed(0) & D_JPAD) {
                objdata->unk4E0 = 9;
                objdata->unk4BA |= 0x10;
            }
            break;
        case 12:
            if (objdata->unk4BA & 1) {
                objdata->unk4E0 = 8;
                objdata->unk4BA |= 0x10;
            }
            break;
        case 1:
            if (objdata->unk4BA & 1) {
                objdata->unk4B8 = 0;
                objdata->unk4BC = objdata->unk4BC - 1;
                objdata->unk4D0 = rand_next(0x1F4, 0x320);
            }
            break;
        case 2:
            if (objdata->unk4BA & 1) {
                objdata->unk4B8 = 0;
                objdata->unk4BC = rand_next(1, 2);
                objdata->unk4D0 = rand_next(0x1F4, 0x320);
            }
            break;
        case 6:
            if (objdata->unk4BA & 1) {
                objdata->unk4B8 = 7;
                objdata->unk4D4 = rand_next(0x3E8, 0x7D0);
            }
            break;
        case 7:
            objdata->unk4D4 -= gUpdateRate;
            if (objdata->unk4D4 < 0) {
                objdata->unk4D4 = 0;
            }
            if ((objdata->unk4BA & 1) && (objdata->unk4D4 <= 0)) {
                objdata->unk4B8 = 8;
            }
            break;
        case 8:
            if (objdata->unk4BA & 1) {
                objdata->unk4B8 = 9;
            }
            break;
        case 9:
            if (objdata->unk4BA & 1) {
                objdata->unk4E0 = 8;
                objdata->unk4BA |= 0x10;
            }
            break;
        case 10:
            if (main_get_bits(objdata->unk4DC) != 0) {
                objdata->unk4BA &= ~0x20;
                objdata->unk4BA |= 8;
                objdata->unk4B8 = 0xB;
            }
            break;
        case 11:
            if (objdata->unk4BA & 1) {
                objdata->unk4E0 = 8;
                objdata->unk4BA |= 0x10;
            }
            break;
        case 14:
            if (objdata->unk4BA & 1) {
                objdata->unk4B8 = 0xD;
            }
            break;
        case 13:
            if (gDLL_7_Newday->vtbl->func8(&sp58) == 0) {
                objdata->unk4B8 = 0xB;
            }
            break;
        default:
            if (objdata->unk4BA & 0x20) {
                objdata->unk4B8 = 0xA;
            } else if (objdata->unk760->type1D.unk34 == 4) {
                objdata->unk4B8 = 6;
            } else if (gDLL_7_Newday->vtbl->func8(&sp58) != 0) {
                objdata->unk4B8 = 0xE;
            } else {
                objdata->unk4B8 = 0;
                objdata->unk4D0 = rand_next(0x1F4, 0x320);
                objdata->unk4D2 = rand_next(0x7D0, 0xBB8);
            }
            break;
        }
    } else {
        objdata->unk844 = vec3_distance_xz(&self->globalPosition, &objdata->unk764->pos);
        if ((objdata->unk844 < 30.0f) && ((objdata->unk4BA & 1) != 0)) {
            objdata->unk4BA |= 2;
            objdata->unk760 = objdata->unk764;
            if (((objdata->unk4E0 == 9) && ((u8) objdata->unk760->type1D.unk34 != 4)) || ((u8) objdata->unk760->type1D.unk34 == 0)) {
                objdata->unk4B8 = -1;
                objdata->unk4BA |= 0x10;
            }
        } else {
            switch (objdata->unk4B8) {
            case 0:
            default:
                var_fv0 = objdata->unk764->pos.x - objdata->unk75C->pos.x;
                var_fv1 = objdata->unk764->pos.z - objdata->unk75C->pos.z;
                objdata->unk4D6 = arctan2_f(-var_fv0, -var_fv1);
                objdata->unk4DA = self->srt.yaw;
                objdata->unk4D8 = self->srt.yaw - (objdata->unk4D6 & 0xFFFF);
                CIRCLE_WRAP(objdata->unk4D8);
                if ((objdata->unk4D8 < 0x1F4) && (objdata->unk4D8 >= -0x1F3)) {
                    objdata->unk4B8 = 3;
                } else {
                    if (objdata->unk4D8 > 0x4000) {
                        objdata->unk4D8 = 0x4000;
                    } else if (objdata->unk4D8 < -0x4000) {
                        objdata->unk4D8 = -0x4000;
                    }
                    if (objdata->unk4D8 < 0) {
                        objdata->unk4B8 = 4;
                        objdata->unk848 = (f32) -objdata->unk4D8;
                    } else {
                        objdata->unk4B8 = 5;
                        objdata->unk848 = (f32) objdata->unk4D8;
                    }
                    objdata->unk848 = (f32) (objdata->unk848 / 16384.0f);
                    if (objdata->unk848 > 1.0f) {
                        objdata->unk848 = 1.0f;
                    } else if (objdata->unk848 < 0.0f) {
                        objdata->unk848 = 0.0f;
                    }
                }
                break;
            case 3:
                if ((objdata->unk4E0 == 8) && ((u8) objdata->unk764->type1D.unk34 != 0)) {
                    objdata->unk848 = (objdata->unk844 - 35.0f) / 45.0f;
                    if (objdata->unk848 < 0.0f) {
                        objdata->unk848 = 0.0f;
                    } else if (objdata->unk848 > 1.0f) {
                        objdata->unk848 = 1.0f;
                    }
                } else {
                    objdata->unk848 = 1.0f;
                }
                var_fv0 = objdata->unk764->pos.x - self->srt.transl.x;
                var_fv1 = objdata->unk764->pos.z - self->srt.transl.z;
                if (objdata->unk844 != 0.0f) {
                    var_fv0 /= objdata->unk844;
                    var_fv1 /= objdata->unk844;
                }
                objdata->unk4BA |= 4;
                objdata->unk808 = 0.28f;
                self->velocity.x = objdata->unk808 * var_fv0;
                self->velocity.z = objdata->unk808 * var_fv1;
                obj_move(self, self->velocity.x * gUpdateRateF, 0.0f, self->velocity.z * gUpdateRateF);
                func_8002493C(self, objdata->unk808, &objdata->unk804);
                break;
            case 4:
            case 5:
                if (objdata->unk4BA & 1) {
                    objdata->unk4DA = self->srt.yaw;
                    objdata->unk4D8 = self->srt.yaw - (objdata->unk4D6 & 0xFFFF);
                    CIRCLE_WRAP(objdata->unk4D8);
                    if ((objdata->unk4D8 < 0x1F4) && (objdata->unk4D8 >= -0x1F3)) {
                        objdata->unk4B8 = 3;
                        objdata->unk848 = 1.0f;
                    } else {
                        if (objdata->unk4D8 > 0x4000) {
                            objdata->unk4D8 = 0x4000;
                        } else if (objdata->unk4D8 < -0x4000) {
                            objdata->unk4D8 = 0x4000;
                        }
                        if (objdata->unk4D8 < 0) {
                            objdata->unk848 = (f32) -objdata->unk4D8;
                        } else {
                            objdata->unk848 = (f32) objdata->unk4D8;
                        }
                        objdata->unk848 = (f32) (objdata->unk848 / 16384.0f);
                        if (objdata->unk848 > 1.0f) {
                            objdata->unk848 = 1.0f;
                        } else if (objdata->unk848 < 0.0f) {
                            objdata->unk848 = 0.0f;
                        }
                        objdata->unk4BA |= 8;
                    }
                } else {
                    self->srt.yaw = (s16) ((f32) objdata->unk4DA - (self->animProgress * (f32) objdata->unk4D8));
                }
                break;
            }
        }
    }
    if (objdata->unk4B8 != -1) {
        temp = objdata->unk4B8 * 2;
        if ((data_16C[temp] != self->curModAnimId) || (objdata->unk4BA & 8)) {
            func_80023D30(self, data_16C[temp], 0.0f, 0U);
            if (!(objdata->unk4BA & 4)) {
                objdata->unk804 = data_1B8[objdata->unk4B8];
            }
            objdata->unk4BA &= ~0x9;
        }
        if (data_16C[temp + 1] != -1) {
            func_80025540(self, data_16C[temp + 1], (s32) (objdata->unk848 * 1023.0f));
        }
    }
    if (data_1A8[objdata->unk4B8] & 1) {
        self->unkAF |= 8;
    }
    sp38 = func_800348A0(self, 5, 0);
    temp_v0_6 = func_800348A0(self, 4, 0);
    if (data_1A8[objdata->unk4B8] & 2) {
        if (sp38 != NULL) {
            sp38->frame = 0x200;
        }
        if (temp_v0_6 != NULL) {
            temp_v0_6->frame = 0x200;
        }
    } else {
        if (sp38 != NULL) {
            sp38->frame = 0;
        }
        if (temp_v0_6 != NULL) {
            temp_v0_6->frame = 0;
        }
    }
    if (func_80024108(self, objdata->unk804, gUpdateRateF, &sp3C) != 0) {
        objdata->unk4BA |= 1;
        return;
    }
    objdata->unk4BA &= ~0x1;
}

// offset: 0x19E0 | func: 14
static void dll_508_func_19E0(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x19F4 | func: 15
static void dll_508_func_19F4(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1A08 | func: 16
static void dll_508_func_1A08(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1A1C | func: 17
static void dll_508_func_1A1C(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    dll_508_func_1A58(self, objdata, setup);
}

// offset: 0x1A58 | func: 18
static void dll_508_func_1A58(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    s32 var_a3;

    if (main_get_bits(0x14) == 0) {
        objdata->unk4DC = 0x14;
        objdata->unk4BA |= 0x20;
        var_a3 = 3;
    } else {
        objdata->unk4DC = -1;
        var_a3 = 8;
    }
    objdata->unk760 = dll_508_func_608(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, var_a3);
    if (objdata->unk760 == NULL) {
        STUBBED_PRINTF("THORNTAIL: cannot find a node\n");
    }
    objdata->unk4BA |= 2;
    self->srt.transl.x = objdata->unk760->pos.x;
    self->srt.transl.f[1] = objdata->unk760->pos.f[1];
    self->srt.transl.f[2] = objdata->unk760->pos.f[2];
    self->srt.yaw = objdata->unk760->unk2C << 8;
    objdata->unk4D0 = rand_next(0x1F4, 0x320);
    objdata->unk4D2 = rand_next(0x7D0, 0xBB8);
    objdata->unk4BC = rand_next(1, 2);
    objdata->unk4C0 = (u8 *) &data_1F8;
    objdata->unk4C4 = 3;
}

// offset: 0x1B9C | func: 19
static void dll_508_func_1B9C(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    switch (objdata->unk4BB) {
    default:
    case 1:
        dll_508_func_1C48(self, objdata, setup);
        return;
    case 2:
        dll_508_func_1D70(self, objdata, setup);
        return;
    case 3:
        dll_508_func_1D84(self, objdata, setup);
        return;
    case 4:
        dll_508_func_1D98(self, objdata, setup);
        return;
    }
}

// offset: 0x1C48 | func: 20
static void dll_508_func_1C48(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    Object *sidekick;
    Object *player;

    sidekick = get_sidekick();
    player = get_player();
    dll_508_func_BEC(self, objdata, setup);
    if ((objdata->unk4B8 == 0xA) && (sidekick != NULL)) {
        if (vec3_distance_squared(&player->globalPosition, &self->globalPosition) < 4900.0f) {
            ((DLL_ISidekick*)sidekick->dll)->vtbl->func14(sidekick, 2);
        }
        if (((DLL_ISidekick*)sidekick->dll)->vtbl->func24(sidekick) != 0) {
            main_set_bits(objdata->unk4DC, 1);
        }
    }
}

// offset: 0x1D70 | func: 21
static void dll_508_func_1D70(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1D84 | func: 22
static void dll_508_func_1D84(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1D98 | func: 23
static void dll_508_func_1D98(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1DAC | func: 24
static void dll_508_func_1DAC(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    objdata->unk4BA |= 0x40;
    objdata->unk4DC = 0x8D4;
    dll_508_func_1DF8(self, objdata, setup);
}

// offset: 0x1DF8 | func: 25
static void dll_508_func_1DF8(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    dll_508_func_A04(self, objdata, setup);
    objdata->unk4C0 = (u8 *) data_1FC;
    objdata->unk4C4 = 1;
}

// offset: 0x1E4C | func: 26
static void dll_508_func_1E4C(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup) {
    dll_508_func_B40(self, objdata, setup);
    switch (objdata->unk4BB) {
    case 1:
        dll_508_func_1F28(self, objdata, setup);
        return;
    case 2:
        dll_508_func_1F3C(self, objdata, setup);
        break;
    case 3:
        dll_508_func_1F50(self, objdata, setup);
        break;
    case 4:
        dll_508_func_1F64(self, objdata, setup);
        break;
    }
}

// offset: 0x1F28 | func: 27
static void dll_508_func_1F28(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1F3C | func: 28
static void dll_508_func_1F3C(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1F50 | func: 29
static void dll_508_func_1F50(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1F64 | func: 30
static void dll_508_func_1F64(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }
