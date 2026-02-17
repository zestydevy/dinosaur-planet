#include "common.h"
#include "sys/objanim.h"
#include "sys/objtype.h"
#include "dlls/objects/common/foodbag.h"
#include "dlls/objects/210_player.h"

typedef struct {
    u8 _unk0[0x410];
} DLL428_Data;

// size:0x14
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u16 unk4;
    u8 unk6;
    u32 unk8;
    f32 unkC;
    f32 unk10;
} capy_data;

/*0x0*/ static u32 _data_0 = 0x00000002;
/*0x4*/ static u16 _data_4[] = {
    0x0537,0x0538, 0x0539,0x0000, 0x0000,0x0000
};

/*0x0*/ static  ObjFSA_StateCallback _bss_0[9];
/*0x28*/ static ObjFSA_StateCallback _bss_28[6];


static void dll_428_func_468(Object* arg0, Baddie* arg1, ObjFSA_Data* arg2);
static void dll_428_func_644(Object* arg0, s32 arg1, Baddie* arg2, ObjFSA_Data* arg3);
static void dll_428_func_704(Object* arg0, Baddie* arg1, ObjFSA_Data* arg2);
static int dll_428_func_44C(Object * arg0, Object * arg1, AnimObj_Data *  arg2, s8 arg3);
static s32 dll_428_func_7A0(Object* self, ObjFSA_Data* fsa, f32 arg2);
static s32 dll_428_func_97C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_428_func_A0C(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_A9C(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_BC4(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_D1C(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_F8C(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_10BC(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_1264(Object* arg0, ObjFSA_Data* arg1, f32 arg2);

static s32 dll_428_func_1408(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_1420(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_1438(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_1698(Object *arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_1728(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_428_func_19DC(Object* arg0, ObjFSA_Data* arg1, f32 arg2);

// offset: 0x0 | func: 0
static void dll_428_func_0(void) {
    _bss_0[0] = dll_428_func_7A0;
    _bss_0[1] = dll_428_func_97C;
    _bss_0[2] = dll_428_func_A0C;
    _bss_0[3] = dll_428_func_A9C;
    _bss_0[4] = dll_428_func_BC4;
    _bss_0[5] = dll_428_func_D1C;
    _bss_0[6] = dll_428_func_F8C;
    _bss_0[7] = dll_428_func_10BC;
    _bss_0[8] = dll_428_func_1264;
    
    _bss_28[0] = dll_428_func_1408;
    _bss_28[1] = dll_428_func_1420;
    _bss_28[2] = dll_428_func_1438;
    _bss_28[3] = dll_428_func_1698;
    _bss_28[4] = dll_428_func_1728;
    _bss_28[5] = dll_428_func_19DC;
}

// offset: 0xD8 | ctor
void dll_428_ctor(void *dll);
void dll_428_ctor(void* dll) {
  dll_428_func_0();
}

// offset: 0x118 | dtor
void dll_428_dtor(void *dll) { }

// offset: 0x124 | func: 1 | export: 0
void dll_428_setup(Object* self, Baddie_Setup* setup, s32 arg2) {
    s32 var_v0;
    Baddie* baddie;
    
    baddie = self->data;
    var_v0 = 0x16;
    
    if (arg2 != 0) {
        var_v0 = 0x17;
    }
    if (!(setup->unk2B & 1)) {
        var_v0 = (var_v0 | 8) & 0xFF;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, (Baddie_Setup* ) setup, (Baddie* ) baddie, 9, 6, 0x102, (u8) var_v0, 20.0f);
    self->animCallback = dll_428_func_44C;
    baddie ->fsa.animState = 1;
    baddie->fsa.logicState = 0;
    baddie->fsa.unk33A = 0;
}


// offset: 0x1DC | func: 2 | export: 1
void dll_428_control(Object *self);
void dll_428_control(Object* self) {
    Baddie* baddie;

    baddie = self->data;
    
    gDLL_18_objfsa->vtbl->tick(self, (ObjFSA_Data* ) baddie, 1.0f, 1.0f, (s32 (**)(Object*, ObjFSA_Data*, f32)) _bss_0, (s32 (**)(Object*, ObjFSA_Data*, f32)) _bss_28);
    dll_428_func_468(self, (Baddie* ) baddie, (ObjFSA_Data* ) baddie);
    if ((baddie->fsa.target != 0) || (baddie->fsa.hitpoints == 0)) { 
        dll_428_func_644(self, 0, (Baddie* ) baddie, (ObjFSA_Data* ) baddie);
        return;
    }
    dll_428_func_704(self, (Baddie* ) baddie, (ObjFSA_Data* ) baddie);
}

// offset: 0x2D4 | func: 3 | export: 2
void dll_428_update(Object* self) {
    gDLL_18_objfsa->vtbl->func2(self, (ObjFSA_Data* ) self->data, (s32 (**)(Object*, ObjFSA_Data*, f32)) _bss_0);
}

// offset: 0x324 | func: 4 | export: 3
void dll_428_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0x384 | func: 5 | export: 4
void dll_428_free(Object* self, s32 a1) {
    Baddie* sp24;

    sp24 = self->data;
    obj_free_object_type(self, 4);
    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }
    gDLL_33_BaddieControl->vtbl->func15(self, sp24, 1U);
}


// offset: 0x428 | func: 6 | export: 5
u32 dll_428_get_model_flags(Object* self) {
    return MODFLAGS_100 | MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x438 | func: 7 | export: 6
u32 dll_428_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL428_Data);
}

// offset: 0x44C | func: 8
static int dll_428_func_44C(Object * arg0, Object * arg1, AnimObj_Data *  arg2, s8 arg3) {
    return 0; 
}


// offset: 0x468 | func: 9
static void dll_428_func_468(Object* arg0, Baddie* arg1, ObjFSA_Data* arg2) {
    Vec3f sp44;
    Object* temp_v0_2;
    Object* sp3C;
    capy_data* capydata;
    Baddie* baddie;

    Object* temp_v0_3; 


    baddie = arg0->data;
    capydata = baddie->objdata;

    sp3C = NULL;

    if (arg0->linkedObject != NULL) {
        arg0->linkedObject->parent = arg0->parent;
    }
    if (arg2->target != NULL) {
        sp44.f[0] = arg2->target->positionMirror.f[0] - arg0->positionMirror.f[0];
        sp44.f[1] = arg2->target->positionMirror.f[1] - arg0->positionMirror.f[1];
        sp44.f[2] = arg2->target->positionMirror.f[2] - arg0->positionMirror.f[2];
        arg2->targetDist = sqrtf(SQ(sp44.f[0]) + SQ(sp44.f[1]) + SQ(sp44.f[2]));
    }
    gDLL_33_BaddieControl->vtbl->func20(arg0, arg2, &arg1 -> unk34C, arg1->unk39E, &arg1 -> unk3B4, 0, 0, 0);
    temp_v0_2 = get_player();
    if ((temp_v0_2 == arg2->target) || ((arg2->target != NULL) && (arg2->target->id == 0x313))) {
        temp_v0_3 = ((DLL_210_Player*)temp_v0_2->dll)->vtbl->func66(temp_v0_2, 15);
        if (temp_v0_3 != NULL) {
            sp3C = ((DLL_IFoodbag*)temp_v0_3->dll)->vtbl->get_nearest_placed_food_of_type(temp_v0_3, arg0, 0x20);
        }
        if ((sp3C != NULL) && (vec3_distance(&arg0->positionMirror, &sp3C->positionMirror) < 150.0f)) {
            arg2->target = sp3C;
            return;
        }
        arg2->target = NULL;
    }
}

// offset: 0x644 | func: 10
static void dll_428_func_644(Object* arg0, s32 arg1, Baddie* arg2, ObjFSA_Data* arg3) {
    Object* temp_v0;
    ObjectHitInfo* temp_v1;
    f32 var_ft0;
    u16 temp_t0;

    temp_v0 = get_player();
    temp_v1 = arg0->objhitInfo;
    temp_v1->unk58 |= 1;
    if (temp_v0 == arg3->target) {
        if (gDLL_33_BaddieControl->vtbl->func16(arg0, arg3, arg2->unk3E2, 1) != 0) {
            arg3->target = NULL;
        }
    }
}

// offset: 0x704 | func: 11
static void dll_428_func_704(Object* arg0, Baddie* arg1, ObjFSA_Data* arg2) {
    Object* temp_v0_2;
    ObjectHitInfo* hitinfo;
    u16 temp_t9;

    hitinfo = arg0->objhitInfo;
    hitinfo->unk58 &= 0xFFFE;
    
    temp_v0_2 = gDLL_33_BaddieControl->vtbl->func17(arg0, arg2, arg1->unk3E2, 0x8000);
    if (temp_v0_2 != NULL) {
        arg2->target = temp_v0_2;
        arg2->unk33D = 0;
    }
}


// offset: 0x7A0 | func: 12
s32 dll_428_func_7A0(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    s32 count;
    Object** objlist;
    s32 i;
    s32 distflag;
    Baddie* baddie;
    capy_data* capydata;

    baddie = self->data;
    capydata = baddie->objdata;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 3, 0.0f, 0U);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.03f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->func11(self, fsa, arg2, 5);
    if (fsa->enteredAnimState != 0) {
        capydata->unk4 = 0U;
    }
    capydata->unk4 = (u16) (capydata->unk4 + 1);
    if ((get_player() == fsa->target) && ((s32) capydata->unk4 >= 0x3D)) {
        if (capydata->unk1 != 0) {
            fsa->enteredAnimState = 1;
            fsa->unk33A = 0;
            fsa->logicState = 5;
            return 4;
        }
        objlist = obj_get_all_of_type(0x36, &count);
        distflag = 0;
        for (i = 0; i < count; i++) {
            if (!distflag && vec3_distance(&self->positionMirror, &objlist[i]->positionMirror) < 300.0f) {
                distflag = 1;
                fsa->target = objlist[i];
            }
        }
    }
    return 0;
}

// offset: 0x97C | func: 13
s32 dll_428_func_97C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie *objdata = self->data;
    capy_data* capydata;

    capydata = objdata->objdata;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 1, 0.0f, 0U);
        fsa->unk33A = 0;
    }
    if (capydata->unk4 != 0) {
        capydata->unk4 -= 1;
    }
    fsa->animTickDelta = 0.0f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0xA0C | func: 14
s32 dll_428_func_A0C(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    Baddie* baddie;
    capy_data* capydata;

    
    baddie = arg0->data;
    capydata = baddie->objdata;
    
    if (arg1->enteredAnimState != 0) {
        func_80023D30(arg0, 1, 0.0f, 0U);
        arg1->unk33A = 0;
    }
    
    arg1->animTickDelta = 0.04f;
    arg1->unk278 = 0.0f;
    arg1->unk27C = 0.0f;
    if (arg1->unk33A != 0) {
        arg1->logicState = 4;
    
    }
    return 0;
}


// offset: 0xA9C | func: 15
s32 dll_428_func_A9C(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    Baddie* baddie;
    capy_data* capydata;
    CurveSetup* temp_v0_2;
    s32 temp_v0;
    s32 sp34;

    baddie = arg0->data;
    capydata = baddie->objdata;
    if (arg1->enteredAnimState != 0) {
        func_80023D30(arg0, 0, 0.0f, 0U);
        arg1->unk33A = 0;
    }
    arg1->animTickDelta = 0.04f;
    arg1->unk278 = 0.0f;
    arg1->unk27C = 0.0f;
    if (arg1->unk33A != 0) {
        sp34 = 0x25;
        temp_v0 = gDLL_26_Curves->vtbl->func_1E4(arg0->srt.transl.f[0], arg0->srt.transl.f[1], arg0->srt.transl.f[2], &sp34, 1, 0);
        if (temp_v0 != -1) {
            temp_v0_2 = gDLL_26_Curves->vtbl->func_39C(temp_v0);
            arg0->srt.transl.f[0] = temp_v0_2->pos.f[0];
            arg0->srt.transl.f[2] = temp_v0_2->pos.f[2];
        }
        capydata->unk4 = 0x50;
        return 2;
    }
    return 0;
}

// offset: 0xBC4 | func: 16
s32 dll_428_func_BC4(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    Baddie* baddie;
    capy_data* capydata;
    s32 temp_v0;
    s32 var_v0;

    baddie = arg0->data;
    capydata = baddie->objdata;
    if (arg1->enteredAnimState != 0) {
        func_80023D30(arg0, 5, 0.0f, 0U);
        arg1->unk33A = 0;
    }
    temp_v0 = arg1->unk308;
    arg1->animTickDelta = 0.04f;
    arg1->unk278 = 0.0f;
    arg1->unk27C = 0.0f;
    if (temp_v0 & 1) {
        arg1->unk308 = (s32) (temp_v0 & ~1);
        capydata->unk8 = gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x77CU, 0x7FU, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->func_954(capydata->unk8, ((f32) rand_next(-0xA, 0xA) / 100.0f) + 1.0f);
    }
    var_v0 = 0;
    if (arg1->unk33A != 0) {
        return 6;
    }
    return var_v0;
}


// offset: 0xD1C | func: 17
s32 dll_428_func_D1C(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    Object* temp_s0;
    Object* temp_v0_2;
    Object* temp_v0_3;
    Baddie* baddie;
    capy_data* capydata;
    
    baddie = arg0->data;
    capydata = baddie->objdata;
    if (arg1->enteredAnimState != 0) {
        func_80023D30(arg0, 6, 0.0f, 0U);
        arg1->unk33A = 0;
    }
    temp_s0 = arg1->target;
    arg1->animTickDelta = 0.1f;
    arg1->unk278 = 0.0f;
    arg1->unk27C = 0.0f;
    if (temp_s0 != NULL) {
        if (arg1->enteredAnimState != 0) {
            func_8002635C(temp_s0, NULL, 7, 1, 0);
        }
        temp_s0->srt.scale *= 0.97f;
    }

    if (arg1->unk308 & 1) {
        arg1->unk308 = (s32) (arg1->unk308 & ~1);
        capydata->unk8 = gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x77DU, 0x7FU, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->func_954(capydata->unk8, ((f32) rand_next(-0xA, 0xA) / 100.0f) + 1.0f);
    }
    if (arg1->unk33A != 0) {
        if (temp_s0 != NULL) {
            if (temp_s0->srt.scale > 0.01f) {
                return 0;
            }
            if (temp_s0->id == 0x2C0) {
                capydata->unk1 = 1;
                obj_destroy_object(temp_s0);
            } else {
                capydata->unk0 = 1;
                temp_v0_2 = get_player();
                temp_v0_3 = ((DLL_210_Player*)temp_v0_2->dll)->vtbl->func66(temp_v0_2, 15);
                if (temp_v0_3 != NULL) {
                    ((DLL_IFoodbag*)temp_v0_3->dll)->vtbl->destroy_placed_food(temp_v0_3, temp_s0);
                }
            }
            arg1->target = NULL;
            
        }
        return 7;
    }
    return 0;
}

// offset: 0xF8C | func: 18
s32 dll_428_func_F8C(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    Baddie* baddie;

    baddie = arg0->data;
    if (arg1->enteredAnimState != 0) {
        func_80023D30(arg0, 7, 0.0f, 0U);
        arg1->unk33A = 0;
    }
    arg1->animTickDelta = 0.08f;
    arg1->unk278 = 0.0f;
    arg1->unk27C = 0.0f;
    if (arg1->unk33A != 0) {
        if ((gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, arg0, 150.0f, (s32* ) &_data_0, -1) == 0) && (main_get_bits((s32) _data_4[baddie->unk3F8->unk9C->unk18]) == 0)) {
            arg1->logicState = 2;
            return 9;
        }
        arg1->logicState = 4;
        return 1;
    }
    return 0;
}


// offset: 0x10BC | func: 19
s32 dll_428_func_10BC(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    Baddie* baddie;
    capy_data* capydata;
    s32 temp_v0;

    baddie = arg0->data;
    capydata = baddie->objdata;
    if (arg1->enteredAnimState != 0) {
        func_80023D30(arg0, 2, 0.0f, 0U);
        arg1->unk33A = 0;
    }
    arg1->animTickDelta = 0.015f;
    arg1->unk278 = 0.0f;
    arg1->unk27C = 0.0f;
    if (arg1->unk308 & 1) {
        arg1->unk308 = (s32) (arg1->unk308 & ~1);
        capydata->unk8 = gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x77DU, 0x7FU, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->func_954(capydata->unk8, ((f32) rand_next(-0xA, 0xA) / 100.0f) + 1.0f);
    }
    if (arg1->unk308 & 0x200) {
        main_set_bits((s32) _data_4[baddie->unk3F8->unk9C->unk18], 1U);
    }
    if (arg1->unk33A != 0) {
        arg1->logicState = 4;
     return 1;
    }
    return 0;
}


// offset: 0x1264 | func: 20
s32 dll_428_func_1264(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    Baddie* baddie;
    capy_data* capydata;
    f32 temp_fa0;
    f32 temp_fv1;
    

    baddie = arg0->data;
    capydata = baddie->objdata;
    if (arg1->enteredAnimState != 0) {
        func_80023D30(arg0, 4, 0.0f, 0U);
        arg1->unk33A = 0;
    }
    if (arg1->enteredAnimState != 0) {
        capydata->unk4 = 0U;
        capydata->unkC = (f32) arg0->srt.transl.f[0];
        capydata->unk10 = (f32) arg0->srt.transl.f[2];
    }
    temp_fv1 = arg0->srt.transl.f[0] - capydata->unkC;
    temp_fa0 = arg0->srt.transl.f[2] - capydata->unk10;
    capydata->unkC = arg0->srt.transl.f[0];
    capydata->unk10 = (f32) arg0->srt.transl.f[2];
    if (((SQ(temp_fv1) + SQ(temp_fa0)) < 0.1f) && (arg1->logicState != 2)) {
        capydata->unk4 = (u16) (capydata->unk4 + 1);
    } else {
        capydata->unk4 = 0U;
    }
    if ((s32) capydata->unk4 >= 0xD) {
        arg1->enteredAnimState = 1;
        arg1->unk33A = 0;
        arg1->logicState = 5;
        return 4;
    }
    arg1->animTickDelta = 0.08f;
    gDLL_18_objfsa->vtbl->func7(arg0, arg1, arg2, 1);
    gDLL_33_BaddieControl->vtbl->func3(arg0, (Baddie* ) arg1, (u32) baddie, 2.0f, 12.0f);
    return 0;
}


// offset: 0x1408 | func: 21
s32 dll_428_func_1408(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    return 6;
}


// offset: 0x1420 | func: 22
s32 dll_428_func_1420(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    return 0;
}


// offset: 0x1438 | func: 23
s32 dll_428_func_1438(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    s32 temp2;
    s32 temp3;
    capy_data* sp3C;
    UnkCurvesStruct* baddieCurves;
    Baddie* baddie;
    f32 var_ft0;
    f32 var_fv0;
    f32 temp;
    s32 temp4;
    s32 tempYaw;

    baddie = arg0->data;
    baddieCurves = baddie->unk3F8;
    sp3C = baddie->objdata;
    temp2 = func_800053B0(baddieCurves, 5.0f / (SQ(baddieCurves->unk68.f[2] - arg0->srt.transl.f[2]) + SQ(baddieCurves->unk68.f[0] - arg0->srt.transl.f[0])));
    if (((temp2 != 0) || (baddieCurves->unk10 != 0)) && (gDLL_26_Curves->vtbl->func_4704(baddieCurves) != 0)) {
        arg1->animState = 7;
        arg1->enteredAnimState = 0;
        arg1->unk33A = 0;
        arg1->logicState = 1;
        if (arg0){} // @fake
        return 0;
    } else {
        temp4 = (arctan2_f(baddieCurves->unk74, baddieCurves->unk7C) & 0xFFFF);
        var_fv0 = (f32) (((temp4 - ((u16)arg0->srt.yaw & 0xFFFF))) + 0x8000);
        if (var_fv0 > 32768.0f) {
            var_fv0 = -65535.0f + var_fv0;
        }
        if (var_fv0 < -32768.0f) {
            var_fv0 = 65535.0f + var_fv0;
        }
        if (var_fv0 < 0.0f) {
            temp = -var_fv0;
        } else {
            temp = var_fv0;
        }
        arg1->unk278 = 1.0f - (temp / 32767.0f);
        if (arg1->unk278 < 0.01f) {
            arg1->unk278 = 0.01f;
        }
        arg1->unk278 *= (f32) sp3C->unk2 / 100.0f;
        arg1->speed = arg1->unk278;
        gDLL_18_objfsa->vtbl->func6(arg0, arg1, baddieCurves->unk68.f[0], baddieCurves->unk68.f[2], 0, 0, 60.0f);
        arg1->animState = 8;
        return 0;
    }
}

// offset: 0x1698 | func: 24
s32 dll_428_func_1698(Object *arg0, ObjFSA_Data* arg1, f32 arg2) {
    if ((arg1->animState != 6) && ((arg1->target == 0) || (get_player() == arg1->target))) {
        arg1->animState = 3;
        arg1->enteredAnimState = 1;
        arg1->unk33A = 0;
        return 6;
    }
    return 0;
}

// offset: 0x1728 | func: 25
s32 dll_428_func_1728(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    Baddie* sp44;
    Object* temp_v1;
    capy_data* sp3C;
    u8 sp3B;
    Unk80009024* temp_s1;
    f32 temp_fv0;
 
    

    sp44 = arg0->data;
    sp3C = sp44->objdata;
    if (arg1->target != 0) {
        sp3B = get_player() == arg1->target;
        arg1->animState = 8;
        arg1->unk288 = 0.0f;
        arg1->unk284 = 0.0f;
        temp_s1 = &sp44->unk34C;
        bcopy(&arg0-> srt.transl , temp_s1, 0xC);
        bcopy(&arg1-> target -> srt.transl , &sp44 -> unk34C.unkC, 0xC);
        func_80009024(temp_s1, &sp44 -> unk374);
        if (sp3B != 0) {
            if ((arg1->targetDist < 50.0f) || (sp3C->unk0 == 0)) {
                arg1->animState = 0;
            }
        
        } else { 
            temp_fv0 = arg1->targetDist;
            if ((temp_fv0 < 20.0f) || (((s32) sp3C->unk4 >= 0xB) && (temp_fv0 < 30.0f))) {
                temp_v1 = arg1->target;
                arg1->animState = 4;
                temp_v1->unkAF = (u8) (temp_v1->unkAF | 8);
                return 4;
            }
       }
        if (temp_s1->unk25 == 0) {
            gDLL_18_objfsa->vtbl->func6(arg0, arg1, temp_s1->unk18.f[0], temp_s1->unk18.f[2], 0.0f, 0.0f, 60.0f);
        } else if (sp3B != 0) {
            gDLL_18_objfsa->vtbl->func6(arg0, arg1, temp_s1->unk18.f[0], temp_s1->unk18.f[2], 20.0f, 30.0f, 60.0f);
        } else {
            gDLL_18_objfsa->vtbl->func6(arg0, arg1, temp_s1->unk18.f[0], temp_s1->unk18.f[2], 10.0f, 10.0f, 60.0f);
        }
    } else {
        arg1->animState = 3;
        arg1->enteredAnimState = 1;
        arg1->unk33A = 0;
        return 6;
    }
    return 0;
}

// offset: 0x19DC | func: 26
s32 dll_428_func_19DC(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    Baddie* baddie = arg0->data;
    capy_data* objdata = baddie ->objdata;
    
    if ((arg1->target != NULL) && (arg1->animState == 1) && (objdata->unk4 == 0)) {
        gDLL_18_objfsa->vtbl->func11(arg0, arg1, 3.0f, 1);
        arg1->animState = 2;
        arg1->enteredAnimState = 1;
        arg1->unk33A = 0;
    }
    return 0;
}
