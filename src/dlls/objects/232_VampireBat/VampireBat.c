#include "common.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"
#include "sys/objtype.h"
#include "types.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u16 _unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s32 _unk28;
} Bat_Data;

/*0x0*/ static s32 _data_0[] = {
    0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001
};
/*0x70*/ static s8 _data_70[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

/*0x0*/ static ObjFSA_StateCallback _bss_0[2];
/*0x8*/ static ObjFSA_StateCallback _bss_8[5];

static s32 dll_232_func_95C(Object* self, ObjFSA_Data* fsa, f32 arg2);
static s32 dll_232_func_978(Object* self, ObjFSA_Data* fsa, f32 arg2);
static s32 dll_232_func_A88(Object* self, ObjFSA_Data* fsa, f32 arg2);
static s32 dll_232_func_CF0(Object* self, ObjFSA_Data* fsa, f32 arg2);
static s32 dll_232_func_E3C(Object* self, ObjFSA_Data* fsa, f32 arg2);
static s32 dll_232_func_EC8(Object* self, ObjFSA_Data* fsa, f32 arg2);
static s32 dll_232_func_100C(Object* self, ObjFSA_Data* fsa, f32 arg2);

// offset: 0x0 | func: 0
static void dll_232_func_0(void) {
    _bss_0[0] = dll_232_func_95C;
    _bss_0[1] = dll_232_func_978;
    
    _bss_8[0] = dll_232_func_A88;
    _bss_8[1] = dll_232_func_CF0;
    _bss_8[2] = dll_232_func_E3C;
    _bss_8[3] = dll_232_func_EC8;
    _bss_8[4] = dll_232_func_100C;
}

// offset: 0x78 | ctor
void dll_232_ctor(void* dll) {
    dll_232_func_0();
}

// offset: 0xB8 | dtor
void dll_232_dtor(void *dll) { }

// offset: 0xC4 | func: 1 | export: 0
void dll_232_setup(Object* self, Baddie_Setup* setup, s32 arg2) {
    u8 var_v0;
    Bat_Data* bat;
    Baddie* baddie;

    baddie = self->data;
    var_v0 = 6;
    if (arg2 != 0) {
        var_v0 = 7;
    }
    if (!(setup->unk2B & 0x20)) {
        var_v0 |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, setup, baddie, 2, 5, 0x108, var_v0, 20.0f);
    self->animCallback = NULL;
    bat = baddie->objdata;
    bzero(bat, sizeof(Bat_Data));
    bat->unk0 = setup->base.x;
    bat->unk4 = setup->base.y;
    bat->unk8 = setup->base.z;
    bat->unk18 = 0;
    bat->unk1C = 0;
    func_80023D30(self, 0, 0, 0);
    baddie->fsa.animState = 0;
    baddie->fsa.logicState = 0;
    baddie->fsa.flags |= OBJFSA_FLAG_1000000;
    baddie->fsa.hitpoints = 1;
    baddie->unk3B6 = 0;
    baddie->unk3B4 = 0;
    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_8000 | OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_OBJ_POS | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }
    baddie->fsa.unk4.mode = 0;
    func_800267A4(self);
}

// offset: 0x23C | func: 2 | export: 1
void dll_232_control(Object* self) {
    s16 sp90[4] = {0x0206, 0x0167, 0x0165, 0x0206};
    Baddie_Setup* setup;
    Baddie* baddie;
    ObjectShadow* sp84;
    f32 var_fv0;
    Bat_Data* bat;
    Object* sp78;
    Vec3f sp6C;
    SRT sp54;
    s32 sp50;
    f32 temp_fv0_2;
    s32 var_v1;

    baddie = self->data;
    setup = (Baddie_Setup*)self->setup;
    sp84 = self->shadow;
    bat = baddie->objdata;
    sp78 = get_player();
    if (self->unkDC == 0) {
        if (self->unkE0 == 0) {
            self->srt.transl.f[0] = setup->base.x;
            self->srt.transl.f[1] = setup->base.y;
            self->srt.transl.f[2] = setup->base.z;
            gDLL_3_Animation->vtbl->func17(setup->unk2E, self, -1);
            self->unkE0 = 1;
            return;
        }
        if (baddie->fsa.logicState != 4) {
            var_fv0 = self->speed.f[1] - bat->unk18;
            bat->unk18 = self->speed.f[1];
            if (var_fv0 < 0.0f) {
                var_fv0 *= -0.04f;
            } else {
                var_fv0 *= 0.08f;
            }
            if (var_fv0 > 0.1f) {
                var_fv0 = 0.1f;
            }
            func_80024108(self, var_fv0, gUpdateRateF, NULL);
            sp6C.f[0] = bat->unk0 - sp78->srt.transl.f[0];
            sp6C.f[1] = bat->unk4 - sp78->srt.transl.f[1];
            sp6C.f[2] = bat->unk8 - sp78->srt.transl.f[2];
            temp_fv0_2 = sqrtf(SQ(sp6C.f[0]) + SQ(sp6C.f[1]) + SQ(sp6C.f[2]));
            if (baddie->unk3B6 == 0) {
                if (temp_fv0_2 < (f32) baddie->unk3E2) {
                    baddie->fsa.target = sp78;
                    baddie->unk3B6 = 1;
                }
            } else {
                if (baddie->unk3E2 < temp_fv0_2) {
                    baddie->fsa.target = NULL;
                    baddie->unk3B6 = 0;
                }
            }
            sp84->tr.f[0] = self->srt.transl.f[0];
            sp84->tr.f[2] = self->srt.transl.f[2];
            if (func_800588D4(self, self->srt.transl.f[0], self->srt.transl.f[1], self->srt.transl.f[2], &bat->unk1C, 0) != 0) {
                sp84->tr.f[1] = self->srt.transl.f[1] - bat->unk1C;
            }
            if (baddie->fsa.target != NULL) {
                sp6C.f[0] = baddie->fsa.target->positionMirror.f[0] - self->positionMirror.f[0];
                sp6C.f[1] = baddie->fsa.target->positionMirror.f[1] - self->positionMirror.f[1];
                sp6C.f[2] = baddie->fsa.target->positionMirror.f[2] - self->positionMirror.f[2];
                baddie->fsa.targetDist = sqrtf(SQ(sp6C.f[0]) + SQ(sp6C.f[1]) + SQ(sp6C.f[2]));
            }
            if (!(baddie->unk3B0 & 0x20)) {
                gDLL_33_BaddieControl->vtbl->func14(self, baddie, &baddie->unk3B2, -1, -1, baddie->unk3A6, baddie->unk3A4);
            }
            gDLL_33_BaddieControl->vtbl->func20(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0);
            if ((baddie->fsa.hitpoints > 0) && 
                    (gDLL_33_BaddieControl->vtbl->check_hit(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, _data_0, _data_70, 1, &baddie->unk3A8, &sp54) != 0)) {
                var_v1 = ((DLL_Unknown*)sp78->linkedObject->dll)->vtbl->func[19].withOneVoidArgS32(sp78->linkedObject);
                if (var_v1 >= 4) {
                    var_v1 = 3;
                }
                sp54.scale = sp90[var_v1];
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &sp54, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                sp54.transl.f[0] -= self->srt.transl.f[0];
                sp54.transl.f[1] -= self->srt.transl.f[1];
                sp54.transl.f[2] -= self->srt.transl.f[2];
                sp54.scale = sp90[var_v1];
                sp50 = 4;
                while (sp50--) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &sp54, PARTFXFLAG_2, -1, NULL);
                }
            }
            gDLL_33_BaddieControl->vtbl->func10(self, &baddie->fsa, 0.0f, -1);
            baddie->unk3AC = self->unkC0;
            self->unkC0 = NULL;
            gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, gUpdateRateF, gUpdateRateF, _bss_0, _bss_8);
            self->unkC0 = baddie->unk3AC;
        }
    }
}

// offset: 0x828 | func: 3 | export: 2
void dll_232_update(Object *self) { }

// offset: 0x834 | func: 4 | export: 3
void dll_232_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x894 | func: 5 | export: 4
void dll_232_free(Object* self, s32 a1) {
    Baddie* baddie;
    Object* temp_a0;

    baddie = self->data;
    obj_free_object_type(self, OBJTYPE_4);
    temp_a0 = self->linkedObject;
    if (temp_a0 != NULL) {
        obj_destroy_object(temp_a0);
        self->linkedObject = NULL;
    }
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20U);
}

// offset: 0x938 | func: 6 | export: 5
u32 dll_232_get_model_flags(Object* self) {
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x948 | func: 7 | export: 6
u32 dll_232_get_data_size(Object *self, u32 a1) {
    return sizeof(Baddie) + sizeof(Bat_Data);
}

// offset: 0x95C | func: 8
static s32 dll_232_func_95C(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    fsa->unk341 = 1;
    return 0;
}

// offset: 0x978 | func: 9
static s32 dll_232_func_978(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    Baddie* baddie;
    Bat_Data* bat;


    baddie = self->data;
    bat = baddie->objdata;
    fsa->unk341 = 3;
    if (fsa->enteredAnimState != 0) {
        func_800267A4(self);
        self->speed.f[0] = -self->speed.f[0];
        self->speed.f[1] += 5.0f;
        self->speed.f[2] = -self->speed.f[2];
        bat->unk24 = rand_next(-0xFA0, 0xFA0);
        bat->unk22 = rand_next(-0xFA0, 0xFA0);
        bat->unk26 = rand_next(-0xFA0, 0xFA0);
        fsa->logicState = 3;
    }
    self->objhitInfo->unk5E = 0;
    return 0;
}

// offset: 0xA88 | func: 10
static s32 dll_232_func_A88(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    fsa->unk27C = 0.0f;
    fsa->unk278 = 0.0f;
    fsa->animState = 0;
    return 2;
}

// offset: 0xAB8 | func: 11
static void dll_232_func_AB8(Object* self, Bat_Data* bat) {
    s16 temp_t0;
    s32 res;

    if (self->srt.transl.f[0] < bat->unkC) {
        self->speed.f[0] += 1.2f;
    } else {
        self->speed.f[0] -= 1.2f;
    }
    if (self->srt.transl.f[1] < bat->unk10) {
        self->speed.f[1] += 1.0f;
    } else {
        self->speed.f[1] -= 0.75f;
    }
    if (self->srt.transl.f[2] < bat->unk14) {
        self->speed.f[2] += 1.2f;
    } else {
        self->speed.f[2] -= 1.2f;
    }
    if (bat->unk1C < 25.0f) {
        self->speed.f[1] = (self->speed.f[1] * 0.9f) + 0.1f;
    }
    self->speed.f[0] *= 0.985f;
    self->speed.f[1] *= 0.945f;
    self->speed.f[2] *= 0.985f;
    if (self->speed.f[0] > 6.0f) {
        self->speed.f[0] = 6.0f;
    } else if (self->speed.f[0] < -6.0f) {
        self->speed.f[0] = -6.0f;
    }
    if (self->speed.f[2] > 6.0f) {
        self->speed.f[2] = 6.0f;
    } else if (self->speed.f[2] < -6.0f) {
        self->speed.f[2] = -6.0f;
    }
    temp_t0 = self->srt.yaw;
    res = atan2f_to_s(self->speed.f[0], self->speed.f[2]);
    self->srt.yaw = res;
    temp_t0 = self->srt.yaw - temp_t0;
    self->srt.roll += ((self->srt.roll - (temp_t0)) >> 2);
    obj_integrate_speed(self, self->speed.f[0], self->speed.f[1], self->speed.f[2]);
}

// offset: 0xCF0 | func: 12
static s32 dll_232_func_CF0(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    Baddie* baddie;
    Bat_Data* bat;
    s16 sp26;
    f32 sp20;

    baddie = self->data;
    bat = baddie->objdata;
    sp26 = rand_next(-0x8000, 0x7FFF);
    sp20 = baddie->unk3E2 * 0.75f;
    bat->unkC = (fsin16(sp26) * sp20) + bat->unk0;
    bat->unk10 = rand_next(0x1E, 0x64) + bat->unk4;
    bat->unk14 = (fcos16(sp26) * sp20) + bat->unk8;
    dll_232_func_AB8(self, bat);

    if (baddie->unk3B6 == 1) {
        return 3;
    }
    return 0;
}

// offset: 0xE3C | func: 13
static s32 dll_232_func_E3C(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    Bat_Data* bat;
    Baddie* baddie;

    baddie = self->data;
    bat = baddie->objdata;
    if (fsa->target == NULL) {
        return 2;
    }
    bat->unkC = fsa->target->srt.transl.x;
    bat->unk10 = fsa->target->srt.transl.y + 30.0f;
    bat->unk14 = fsa->target->srt.transl.z;
    dll_232_func_AB8(self, bat);
    return 0;
}

// offset: 0xEC8 | func: 14
static s32 dll_232_func_EC8(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    Baddie* baddie;
    Bat_Data* bat;

    baddie = self->data;
    bat = baddie->objdata;
    self->speed.f[0] *= 0.985f;
    self->speed.f[1] = (self->speed.f[1] - 0.4f) * 0.945f;
    self->speed.f[2] *= 0.985f;
    fsa->flags |= OBJFSA_FLAG_4000;
    self->srt.yaw += bat->unk24;
    self->srt.pitch += bat->unk22;
    self->srt.roll += bat->unk26;
    obj_integrate_speed(self, self->speed.f[0], self->speed.f[1], self->speed.f[2]);
    if (bat->unk1C <= 0.0f) {
        obj_send_mesg_many(0, OBJMSG_SEND_ALL | OBJMSG_SEND_IGNORE_SENDER, self, 0xE0000U, self);
        obj_destroy_object(self);
        return 5;
    }
    return 0;
}

// offset: 0x100C | func: 15
static s32 dll_232_func_100C(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    Baddie* baddie;

    baddie = self->data;
    if (fsa->enteredLogicState != 0) {
        gDLL_33_BaddieControl->vtbl->func18(self, baddie->unk3E0, -1, 0U);
        gDLL_18_objfsa->vtbl->func21(self, fsa, PARTICLE_3C, 0xA, 0U);
        baddie->unk3B4 = 0;
    }
    return 0;
}
