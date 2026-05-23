#include "dlls/engine/26_curves.h"
#include "dlls/engine/27.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/53_movelib.h"
#include "dlls/objects/common/sidekick.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "sys/curves.h"
#include "sys/gfx/animseq.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objexpr.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/rand.h"
#include "sys/joypad.h"
#include "dll.h"
#include "macros.h"

typedef struct {
/*000*/ MoveLibData unk0;
/*4B8*/ s8 unk4B8;
/*4B9*/ u8 unk4B9;
/*4BA*/ u8 unk4BA;
/*4BB*/ u8 mapAct;
/*4BC*/ s8 unk4BC;
/*4BD*/ u8 nextSeq; // next seq in rotation to play on interact
/*4BE*/ u8 _unk4BE[0x4C0 - 0x4BE];
/*4C0*/ u8* seqRotation;
/*4C4*/ u8 seqRotationCount; // number of seqs in rotation
/*4C5*/ u8 _unk4C5[0x4D0 - 0x4C5];
/*4D0*/ s16 unk4D0;
/*4D2*/ s16 unk4D2;
/*4D4*/ s16 unk4D4;
/*4D6*/ s16 unk4D6;
/*4D8*/ s16 unk4D8;
/*4DA*/ s16 unk4DA;
/*4DC*/ s16 unk4DC;
/*4E0*/ s32 unk4E0;
/*4E4*/ DLL27_Data unk4E4;
/*744*/ u8 _unk744[0x75C - 0x744];
/*75C*/ CurveSetup* unk75C;
/*760*/ CurveSetup* unk760;
/*764*/ CurveSetup* unk764;
/*768*/ u8 _unk768[0x804 - 0x768];
/*804*/ f32 unk804;
/*808*/ f32 unk808;
/*80C*/ u8 _unk80C[0x840 - 0x80C];
/*840*/ f32 playerDist;
/*844*/ f32 unk844;
/*848*/ f32 unk848;
/*84C*/ f32 unk84C;
/*850*/ HeadAnimation unk850;
/*874*/ u8 unk874;
/*875*/ u8 _unk875[0x878 - 0x875];
} SHthorntail_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18;
} SHthorntail_Setup;

enum ThorntailSeq {
    THORNTAILSEQ_0_HaveYouWokenTheSwapStone = 0, // 0x275
    THORNTAILSEQ_1_LeaveMeAloneStranger = 1, // 0x276
    THORNTAILSEQ_2_ALogForAFish = 2, // 0x277
    THORNTAILSEQ_3_IDontWantThat = 3, // 0x278
    THORNTAILSEQ_4_NoLogsToday = 4, // 0x279
    THORNTAILSEQ_5_NoLogsToday2 = 5, // 0x27A
    THORNTAILSEQ_6_HowsRandorn = 6, // 0x27B
    THORNTAILSEQ_7_ItsANiceDay = 7, // 0x27C
    THORNTAILSEQ_8_TheSharpClawDrainedOurRiver = 8, // 0x27D
    THORNTAILSEQ_9_Unknown = 9, // 0x27E
    THORNTAILSEQ_10_ThornsBlockingBurrows = 10, // 0x27F
    THORNTAILSEQ_11_ArentLikeTheyUsedToBe = 11 // 0x280
};

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
/*00*/ 0x0007, 0xffff, 
/*01*/ 0x0008, 0xffff, 
/*02*/ 0x0009, 0xffff, 
/*03*/ 0x000f, 0x0001, 
/*04*/ 0x000c, 0x0000, 
/*05*/ 0x000d, 0x0000, 
/*06*/ 0x0010, 0xffff, 
/*07*/ 0x0011, 0xffff,
/*08*/ 0x0012, 0xffff, 
/*09*/ 0x0000, 0xffff, 
/*10*/ 0x0004, 0xffff, 
/*11*/ 0x0005, 0xffff, 
/*12*/ 0x0013, 0xffff, 
/*13*/ 0x0004, 0xffff, 
/*14*/ 0x0003, 0xffff
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
/*0x1F4*/ static u8 data_1F4[1] = {
    THORNTAILSEQ_0_HaveYouWokenTheSwapStone
};
/*0x1F8*/ static u8 data_1F8[3] = {
    THORNTAILSEQ_1_LeaveMeAloneStranger, 
    THORNTAILSEQ_2_ALogForAFish, 
    THORNTAILSEQ_3_IDontWantThat
};
/*0x1FC*/ static u8 data_1FC[1] = {
    THORNTAILSEQ_8_TheSharpClawDrainedOurRiver
};
// probably other seqs?
/*0x200*/ static u32 data_200[] = {
    0x04050607, // these would be more log trader seqs if that was the intention
    0xffffffff, 
    0xffff0000, 
    0xffffffff, 
    0xffff0000, 
    0xffffffff, 
    0xffff0000, 
    0xffffffff, 
    0xffff0000
};

static void thorntail_func_584(Object* self);
static void thorntail_func_B40(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static int thorntail_func_8CC(Object *actor, Object *animObj, AnimObj_Data *animObjData, s8 a3);
static void thorntail_func_9B8(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_func_A04(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_func_BEC(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_func_19E0(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_func_1B9C(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_func_1E4C(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_func_1F28(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_func_1F3C(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_func_1F50(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_func_1F64(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_func_19F4(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_func_1A08(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_func_1A1C(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_func_1A58(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_func_1C48(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_func_1D70(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_func_1D84(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_func_1D98(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_func_1DAC(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_func_1DF8(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);

// offset: 0x0 | ctor
void thorntail_ctor(void *dll) { }

// offset: 0xC | dtor
void thorntail_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
// @bug: Missing 3rd param
#ifndef AVOID_UB
void thorntail_setup(Object *self, SHthorntail_Setup *setup/*, s32 reset*/) {
#else
void thorntail_setup(Object *self, SHthorntail_Setup *setup, s32 reset) {
#endif
    SHthorntail_Data *objdata = self->data;
    s32 _pad;
    u8 sp3C[] = {1, 1, 1, 1}; // data_224

    switch (setup->unk18) {
    case 1:
        thorntail_func_9B8(self, objdata, setup);
        break;
    case 2:
        thorntail_func_1A1C(self, objdata, setup);
        break;
    case 3:
        thorntail_func_1DAC(self, objdata, setup);
        break;
    }
    gDLL_27->vtbl->init(&objdata->unk4E4, DLL27FLAG_4000000 | DLL27FLAG_2000000, DLL27FLAG_400, DLL27MODE_1);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->unk4E4, 4, data_0, data_30, sp3C);
    gDLL_27->vtbl->reset(self, &objdata->unk4E4);
    self->shadow->flags |= (OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_OBJ_POS | OBJ_SHADOW_FLAG_CUSTOM_DIR);
    self->shadow->distFadeMaxOpacity = 128;
    self->shadow->distFadeMinOpacity = 90;
    self->animCallback = thorntail_func_8CC;
    objdata->unk4B8 = -1;
    create_temp_dll(53);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objdata->unk0, -0x1FFF, 0x2AAA, 3);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func5(&objdata->unk0, 0x190, 0x1E);
    objdata->unk0.unk4A9 &= ~0x8;
    obj_add_object_type(self, OBJTYPE_40);
}

// offset: 0x248 | func: 1 | export: 1
void thorntail_control(Object* self) {
    SHthorntail_Data* objdata;
    SHthorntail_Setup* setup;
    Object* player;

    objdata = self->data;
    setup = (SHthorntail_Setup*)self->setup;
    player = get_player();
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    if (func_80026DF4(self, data_40, 15, objdata->unk874, &objdata->unk84C) == 0) {
        objdata->unk874 = 0;
        objdata->mapAct = gDLL_29_Gplay->vtbl->get_act(self->mapID);
        objdata->playerDist = vec3_distance(&self->globalPosition, &player->globalPosition);
        switch (setup->unk18) {
        case 1:
            thorntail_func_B40(self, objdata, setup);
            break;
        case 2:
            thorntail_func_1B9C(self, objdata, setup);
            break;
        case 3:
            thorntail_func_1E4C(self, objdata, setup);
            break;
        }
        thorntail_func_584(self);
        gDLL_27->vtbl->func_1E8(self, &objdata->unk4E4, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, &objdata->unk4E4);
        gDLL_27->vtbl->func_624(self, &objdata->unk4E4, gUpdateRateF);
        if (!(data_1A8[objdata->unk4B8] & 2)) {
            func_80032A08(self, &objdata->unk850);
        }
    }
}

// offset: 0x474 | func: 2 | export: 2
void thorntail_update(Object *self) { }

// offset: 0x480 | func: 3 | export: 3
void thorntail_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    SHthorntail_Data *objdata;

    objdata = self->data;
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func3(self, &objdata->unk0, 0);
    }
}

// offset: 0x504 | func: 4 | export: 4
void thorntail_free(Object *self, s32 onlySelf) {
    remove_temp_dll(53);
    obj_free_object_type(self, OBJTYPE_40);
}

// offset: 0x560 | func: 5 | export: 5
u32 thorntail_get_model_flags(Object *self) {
    return MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x570 | func: 6 | export: 6
u32 thorntail_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(SHthorntail_Data);
}

// offset: 0x584 | func: 7
static void thorntail_func_584(Object *self) {
    MtxF mtx;

    matrix_from_srt(&mtx, &self->srt);
    vec3_transform(&mtx, 0.0f, 0.0f, 0.0f, &self->shadow->tr.x, &self->shadow->tr.y, &self->shadow->tr.z);
}

// offset: 0x608 | func: 8
static CurveSetup* thorntail_func_608(f32 arg0, f32 arg1, f32 arg2, s32 arg3) {
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
static int thorntail_func_8CC(Object *actor, Object *animObj, AnimObj_Data *animObjData, s8 a3) {
    SHthorntail_Data *objdata = actor->data;

    if (((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func4(actor, animObjData, &objdata->unk0, 1, 1) != 0) {
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
static void thorntail_func_9B8(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    objdata->unk4BA |= 0x40;
    objdata->unk4DC = BIT_SH_Move_Thorntail_Blocking_Hollow_Log;
    thorntail_func_A04(self, objdata, setup);
}

// offset: 0xA04 | func: 11
static void thorntail_func_A04(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    s32 var_a3;

    if (main_get_bits(objdata->unk4DC) == 0) {
        objdata->unk4BA |= 0x20;
        var_a3 = 3;
    } else {
        objdata->unk4DC = -1;
        var_a3 = 8;
    }
    objdata->unk760 = thorntail_func_608(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, var_a3);
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
    objdata->seqRotation = data_1F4;
    objdata->seqRotationCount = ARRAYCOUNT(data_1F4);
}

// offset: 0xB40 | func: 12
static void thorntail_func_B40(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup) {
    switch (objdata->mapAct) {
    default:
    case 1:
        thorntail_func_BEC(self, objdata, setup);
        return;
    case 2:
        thorntail_func_19E0(self, objdata, setup);
        return;
    case 3:
        thorntail_func_19F4(self, objdata, setup);
        return;
    case 4:
        thorntail_func_1A08(self, objdata, setup);
        return;
    }
}

/*0x20*/ static const char str_20[] = "THORNTAIL: help cannot find a node\n";

// offset: 0xBEC | func: 13
static void thorntail_func_BEC(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup) {
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
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        gDLL_3_Animation->vtbl->start_obj_sequence(objdata->seqRotation[objdata->nextSeq], self, -1);
        if (objdata->unk4BA & 0x40) {
            objdata->nextSeq++;
            if (objdata->nextSeq >= objdata->seqRotationCount) {
                objdata->nextSeq = 0;
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
            objdata->unk764 = thorntail_func_608(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, 0);
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
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
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
static void thorntail_func_19E0(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x19F4 | func: 15
static void thorntail_func_19F4(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1A08 | func: 16
static void thorntail_func_1A08(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1A1C | func: 17
static void thorntail_func_1A1C(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    thorntail_func_1A58(self, objdata, setup);
}

// offset: 0x1A58 | func: 18
static void thorntail_func_1A58(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    s32 var_a3;

    if (main_get_bits(BIT_14) == 0) {
        objdata->unk4DC = BIT_14;
        objdata->unk4BA |= 0x20;
        var_a3 = 3;
    } else {
        objdata->unk4DC = -1;
        var_a3 = 8;
    }
    objdata->unk760 = thorntail_func_608(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, var_a3);
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
    objdata->seqRotation = data_1F8;
    objdata->seqRotationCount = ARRAYCOUNT(data_1F8);
}

// offset: 0x1B9C | func: 19
static void thorntail_func_1B9C(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    switch (objdata->mapAct) {
    default:
    case 1:
        thorntail_func_1C48(self, objdata, setup);
        return;
    case 2:
        thorntail_func_1D70(self, objdata, setup);
        return;
    case 3:
        thorntail_func_1D84(self, objdata, setup);
        return;
    case 4:
        thorntail_func_1D98(self, objdata, setup);
        return;
    }
}

// offset: 0x1C48 | func: 20
static void thorntail_func_1C48(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    Object *sidekick;
    Object *player;

    sidekick = get_sidekick();
    player = get_player();
    thorntail_func_BEC(self, objdata, setup);
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
static void thorntail_func_1D70(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1D84 | func: 22
static void thorntail_func_1D84(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1D98 | func: 23
static void thorntail_func_1D98(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1DAC | func: 24
static void thorntail_func_1DAC(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    objdata->unk4BA |= 0x40;
    objdata->unk4DC = BIT_SH_Move_Thorntail_Blocking_Swapstone;
    thorntail_func_1DF8(self, objdata, setup);
}

// offset: 0x1DF8 | func: 25
static void thorntail_func_1DF8(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    thorntail_func_A04(self, objdata, setup);
    objdata->seqRotation = data_1FC;
    objdata->seqRotationCount = ARRAYCOUNT(data_1FC);
}

// offset: 0x1E4C | func: 26
static void thorntail_func_1E4C(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup) {
    thorntail_func_B40(self, objdata, setup);
    switch (objdata->mapAct) {
    case 1:
        thorntail_func_1F28(self, objdata, setup);
        return;
    case 2:
        thorntail_func_1F3C(self, objdata, setup);
        break;
    case 3:
        thorntail_func_1F50(self, objdata, setup);
        break;
    case 4:
        thorntail_func_1F64(self, objdata, setup);
        break;
    }
}

// offset: 0x1F28 | func: 27
static void thorntail_func_1F28(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1F3C | func: 28
static void thorntail_func_1F3C(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1F50 | func: 29
static void thorntail_func_1F50(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1F64 | func: 30
static void thorntail_func_1F64(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }
