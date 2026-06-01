#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/27.h"
#include "dlls/engine/33_BaddieControl.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "sys/rand.h"
#include "dll.h"

typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u16 unkC;
    u8 unkE_0 : 1;
    u8 unkE_1 : 1;
    u8 unkE_2 : 1;
} ScorpionRobot_Data;

/*0x0*/ static s8 data_0[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1
};
/*0x1C*/ static u16 data_1C[] = {
    SOUND_25B_Magic_Attack_Deflected, 
    SOUND_25C_Melee_Attack_Deflected, 
    SOUND_375_Smack1, 
    SOUND_376_Smack2
};
/*0x24*/ static s32 data_24[] = {2};

/*0x0*/ static ObjFSA_StateCallback bss_0[8];
/*0x20*/ static ObjFSA_StateCallback bss_20[1];

static s32 dll_234_func_B44(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_234_func_100C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_234_func_1100(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_234_func_12F4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_234_func_16E0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_234_func_18D4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_234_func_1A28(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_234_func_1B94(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_234_func_1D08(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void dll_234_func_0(void) {
    bss_0[0] = dll_234_func_B44;
    bss_0[1] = dll_234_func_100C;
    bss_0[2] = dll_234_func_1100;
    bss_0[3] = dll_234_func_12F4;
    bss_0[4] = dll_234_func_16E0;
    bss_0[5] = dll_234_func_18D4;
    bss_0[6] = dll_234_func_1A28;
    bss_0[7] = dll_234_func_1B94;
    
    bss_20[0] = dll_234_func_1D08;
}

// offset: 0x8C | ctor
void dll_234_ctor(void* dll) {
    dll_234_func_0();
}

// offset: 0xCC | dtor
void dll_234_dtor(void *dll) { }

// offset: 0xD8 | func: 1 | export: 0
void dll_234_setup(Object* self, Baddie_Setup* setup, s32 reset) {
    u8 var_v0;
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata;
    
    var_v0 = 0x16;
    if (reset != 0) {
        var_v0 |= 1;
    }
    if (!(setup->unk2B & 0x20)) {
        var_v0 |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, setup, baddie, 8, 1, 0x108, var_v0, 20.0f);
    
    self->animCallback = NULL;

    objdata = baddie->objdata;
    bzero(objdata, sizeof(ScorpionRobot_Data));
    objdata->unk8 = 0;
    objdata->unkE_0 = 1;
    objdata->unkA = 0x10E1;

    func_80023D30(self, 2, 1.0f, 0);
    baddie->fsa.animState = 0;
    baddie->fsa.logicState = 0;
    baddie->fsa.flags |= OBJFSA_FLAG_1000000;
    baddie->fsa.hitpoints = 12;
    baddie->unk3B6 = 0;
    baddie->unk3B4 = 0;
    baddie->fsa.unk4.mode = DLL27MODE_DISABLED;
    func_800267A4(self);
}

// offset: 0x218 | func: 2 | export: 1
void dll_234_control(Object* self) {
    s16 sp90[] = {0x206, 0x167, 0x165, 0x206}; // texture IDs
    Baddie_Setup* setup = (Baddie_Setup*)self->setup;
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;
    s32 var_v1;
    Object* player = get_player();
    f32 sp70[3];
    SRT sp58;
    s32 sp54;
    s32 _pad;
    
    if (self->unkDC == 0) {
        if (self->unkE0 == 0) {
            self->srt.transl.x = setup->base.x;
            self->srt.transl.y = setup->base.y;
            self->srt.transl.z = setup->base.z;
            gDLL_3_Animation->vtbl->start_obj_sequence(setup->unk2E, self, -1);
            self->unkE0 = 1;
            return;
        }
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 0) != 0) {
            objdata->unk8 += objdata->unkA;
            objdata->unkE_2 = func_80024108(self, objdata->unk0, gUpdateRateF, NULL);
            if (gUpdateRate < objdata->unkC) {
                objdata->unkC -= gUpdateRate;
            } else {
                objdata->unkC = 0;
            }
            if (objdata->unkE_2) {
                if (self->curModAnimId == 2) {
                    func_800267A4(self);
                } else if (self->curModAnimId == 0) {
                    func_8002674C(self);
                }
            }
            if (baddie->fsa.target != NULL) {
                sp70[0] = baddie->fsa.target->globalPosition.x - self->globalPosition.x;
                sp70[1] = baddie->fsa.target->globalPosition.y - self->globalPosition.y;
                sp70[2] = baddie->fsa.target->globalPosition.z - self->globalPosition.z;
                baddie->fsa.targetDist = sqrtf(SQ(sp70[0]) + SQ(sp70[1]) + SQ(sp70[2]));
            }
            if (!(baddie->unk3B0 & 0x20)) {
                gDLL_33_BaddieControl->vtbl->func14(self, baddie, &baddie->unk3B2, -1, -1,  baddie->unk3A6, baddie->unk3A4);
            }
            gDLL_33_BaddieControl->vtbl->func20(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 0);
            if (baddie->fsa.hitpoints > 0) {
                if (gDLL_33_BaddieControl->vtbl->check_hit(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, NULL, data_0, 0, &baddie->unk3A8, &sp58) != 0) {
                    var_v1 = ((DLL_Unknown*)player->linkedObject->dll)->vtbl->func[19].withOneVoidArgS32(player->linkedObject);
                    if (var_v1 >= 4) {
                        var_v1 = 3;
                    }
                    sp58.scale = (f32) sp90[var_v1];
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &sp58, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                    sp58.transl.x -= self->srt.transl.x;
                    sp58.transl.y -= self->srt.transl.y;
                    sp58.transl.z -= self->srt.transl.z;
                    sp58.scale = (f32) sp90[var_v1];
                    sp54 = 4;
                    while (sp54--) {
                        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &sp58, PARTFXFLAG_2, -1, NULL);
                    }
                    gDLL_6_AMSFX->vtbl->play(self, data_1C[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
                    gDLL_6_AMSFX->vtbl->play(self, SOUND_6E7_ScorpionRobot_Damaged, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
            } else {
                if ((baddie->fsa.animState != 7) && (baddie->fsa.animState != 6)) {
                    gDLL_6_AMSFX->vtbl->play(self, SOUND_6F8_ScorpionRobot_Destroyed, MAX_VOLUME, NULL, NULL, 0, NULL);
                    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 6);
                    objdata->unkE_0 = 1;
                    baddie->fsa.target = NULL;
                }
            }
            gDLL_33_BaddieControl->vtbl->func10(self, &baddie->fsa, 0.0f, -1);
            baddie->unk3AC = self->animObj;
            self->animObj = NULL;
            gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, gUpdateRateF, gUpdateRateF, bss_0, bss_20);
            self->animObj = baddie->unk3AC;
        }
    }
}

// offset: 0x85C | func: 3 | export: 2
void dll_234_update(Object *self) { }

// offset: 0x868 | func: 4 | export: 3
void dll_234_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    s32 _pad;
    ScorpionRobot_Data* objdata = baddie->objdata;

    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if ((objdata->unkE_1) && (baddie->fsa.target != NULL)) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_115_ScorpionRobot_LaserFire, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        objdata->unkE_1 = 0;
    }
}

// offset: 0x948 | func: 5 | export: 4
void dll_234_free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    obj_free_object_type(self, OBJTYPE_Baddie);
    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x9EC | func: 6 | export: 5
u32 dll_234_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_8;
}

// offset: 0x9FC | func: 7 | export: 6
u32 dll_234_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(Baddie) + sizeof(ScorpionRobot_Data);
}

// offset: 0xA10 | func: 8
static s32 dll_234_func_A10(Object* arg0, Object* arg1, f32 arg2) {
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;

    if ((arg0 == NULL) || (arg1 == NULL)) {
        return 0;
    }
    xDiff = arg0->srt.transl.x - arg1->srt.transl.x;
    yDiff = arg0->srt.transl.y - arg1->srt.transl.y;
    zDiff = arg0->srt.transl.z - arg1->srt.transl.z;

    return (SQ(xDiff) + SQ(yDiff) + SQ(zDiff)) <= SQ(arg2);
}

// offset: 0xA8C | func: 9 | export: 7
s32 dll_234_func_A8C(Object* self) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    return objdata->unk8;
}

// offset: 0xAA0 | func: 10
static void dll_234_func_AA0(ScorpionRobot_Data* objdata) {
    if (objdata->unkA < 0x10E1) {
        objdata->unkA += (gUpdateRate * 0x46);
        if (objdata->unkA > 0x10E1) {
            objdata->unkA = 0x10E1;
        }
    }
}

// offset: 0xB00 | func: 11
static void dll_234_func_B00(ScorpionRobot_Data* objdata) {
    if (objdata->unkA >= (gUpdateRate * 40)) {
        objdata->unkA -= (gUpdateRate * 40);
    } else {
        objdata->unkA = 0;
    }
}

// offset: 0xB44 | func: 12 | anim state 0
static s32 dll_234_func_B44(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;
    UnkCurvesStruct* sp54 = baddie->unk3F8;
    f32 sp50 = self->velocity.x;
    f32 sp4C = self->velocity.z;
    f32 temp;
    f32 temp_fs0;
    f32 temp_fa1;
    Object* player = get_player();
    
    fsa->unk341 = 0;
    if (objdata->unkE_0) {
        objdata->unkE_0 = 0;
        baddie->unk3B4 = 0;
        baddie->unk3B6 = 0;
        if ((baddie->unk3F8 != NULL) && (gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, self, 500.0f, data_24, -1) != 0)) {
            baddie->unk3B2 &= ~8;
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
            objdata->unkE_0 = 1;
            return 0;
        }
        baddie->unk3B2 |= 8;
    }

    if (baddie->unk3B2 & 8) {
        temp_fs0 = sp54->unk68.x - self->srt.transl.x;
        temp_fa1 = sp54->unk68.z - self->srt.transl.z;
        temp = 10.0f / sqrtf(SQ(temp_fs0) + SQ(temp_fa1));
        if (((func_800053B0(sp54, temp) != 0) || (sp54->unk10 != 0)) 
                && (gDLL_26_Curves->vtbl->func_4704(sp54) != 0) 
                && (gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, self, 500.0f, data_24, -1) != 0)) {
            baddie->unk3B2  &= ~8;
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
            objdata->unkE_0 = 1;
            return 0;
        }
        temp_fs0 = (sp54->unk68.x - self->srt.transl.x) * 0.02f;
        temp_fa1 = (sp54->unk68.z - self->srt.transl.z) * 0.02f;
        if (temp_fs0 > 0.25f) {
            temp_fs0 = 0.25f;
        } else if (temp_fs0 < -0.25f) {
            temp_fs0 = -0.25f;
        }
        if (temp_fa1 > 0.25f) {
            temp_fa1 = 0.25f;
        } else if (temp_fa1 < -0.25f) {
            temp_fa1 = -0.25f;
        }
        self->velocity.x += temp_fs0;
        self->velocity.z += temp_fa1;
    }

    self->velocity.x *= 0.97f;
    self->velocity.z *= 0.97f;
    fsa->flags |= 0x4000;
    temp_fs0 = self->velocity.x - sp50;
    temp_fa1 = self->velocity.z - sp4C;
    self->srt.yaw = atan2f_to_s(temp_fs0, temp_fa1);
    self->srt.pitch = (s16) (sqrtf((temp_fs0 * temp_fs0) + (temp_fa1 * temp_fa1)) * 10000.0f);
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    obj_move(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if (dll_234_func_A10(self, player, (f32) baddie->unk3E2) != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
        objdata->unkE_0 = 1;
        fsa->target = player;
    }
    dll_234_func_AA0(objdata);
    return 0;
}

// offset: 0x100C | func: 13 | anim state 1
static s32 dll_234_func_100C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;
    Object* player = get_player();
    
    fsa->unk341 = 0;
    if (dll_234_func_A10(self, player, (f32) baddie->unk3E2) != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
        objdata->unkE_0 = 1;
        fsa->target = player;
    }
    objdata->unkA = 0;
    return 0;
}

// offset: 0x1100 | func: 14 | anim state 2
static s32 dll_234_func_1100(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 1;
    if (objdata->unkE_0) {
        objdata->unkE_0 = 0;
        func_80023D30(self, 0, 0.0f, 0);
        objdata->unk0 = 0.02f;
        gDLL_6_AMSFX->vtbl->play(self, SOUND_6E4_ScorpionRobot_Activate, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    self->velocity.x *= 0.97f;
    self->velocity.z *= 0.97f;
    fsa->flags |= 0x4000;
    self->srt.pitch -= (self->srt.pitch >> 2);
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    obj_move(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if ((self->curModAnimId == 0) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
        objdata->unkE_0 = 1;
    }
    dll_234_func_B00(objdata);
    return 0;
}

// offset: 0x12F4 | func: 15 | anim state 3
static s32 dll_234_func_12F4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;
    s16 temp_v0_4;
    s32 var_a1;
    s16 temp;

    fsa->unk341 = 1;
    if (objdata->unkE_0) {
        objdata->unkE_0 = 0;
    }
    self->velocity.x *= 0.85f;
    self->velocity.z *= 0.85f;
    fsa->flags |= 0x4000;
    self->srt.pitch -= (self->srt.pitch >> 2);
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    obj_move(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if (objdata->unkE_2) {
        func_80023D30(self, 0, 1.0f, 0);
        objdata->unk0 = 0.0f;
    }
    if ((self->curModAnimId == 7) || (self->curModAnimId == 6)) {
        self->srt.yaw = (s16) ((f32) objdata->unk4 + (5461.3335f * (f32) objdata->unk6 * self->animProgress));
    }
    if ((self->curModAnimId == 0) && (self->animProgress == 1.0f)) {
        if (fsa->target != NULL) {
            temp = (u16)arctan2_f(
                    self->srt.transl.x - fsa->target->srt.transl.x, 
                    self->srt.transl.z - fsa->target->srt.transl.z);
            temp_v0_4 = (u16)rotation16_sub_wrap(
                temp, 
                self->srt.yaw + objdata->unk8);
            if (temp_v0_4 > 0x2000) {
                var_a1 = 6;
                objdata->unk6 = -1;
            } else if (temp_v0_4 < -0x2000) {
                var_a1 = 7;
                objdata->unk6 = 1;
            } else {
                var_a1 = -1;
            }
            if (var_a1 != -1) {
                objdata->unk0 = 0.04f;
                objdata->unk4 = self->srt.yaw;
                func_80023D30(self, var_a1, 0.0f, 0);
                gDLL_6_AMSFX->vtbl->play(self, SOUND_6E5_ScorpionRobot_Moving, MAX_VOLUME, NULL, NULL, 0, NULL);
            } else if (objdata->unkC == 0) {
                objdata->unkC = rand_next(0x5A, 0xD2);
                objdata->unkE_1 = 1;
                objdata->unk0 = 0.04f;
                func_80023D30(self, 3, 0.0f, 0);
            }
        }
    }
    if (dll_234_func_A10(self, fsa->target, (f32) baddie->unk3E2 * 1.5f) == 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 4);
        objdata->unkE_0 = 1;
        fsa->target = NULL;
    }
    dll_234_func_B00(objdata);
    return 0;
}

// offset: 0x16E0 | func: 16 | anim state 4
static s32 dll_234_func_16E0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 0;
    if (objdata->unkE_0) {
        objdata->unkE_0 = 0;
        func_80023D30(self, 2, 0.0f, 0);
        objdata->unk0 = 0.01f;
        gDLL_6_AMSFX->vtbl->play(self, SOUND_6E4_ScorpionRobot_Activate, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    self->velocity.x *= 0.97f;
    self->velocity.z *= 0.97f;
    fsa->flags |= 0x4000;
    self->srt.pitch -= (self->srt.pitch >> 2);
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    obj_move(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if ((self->curModAnimId == 2) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
        objdata->unkE_0 = 1;
        fsa->target = NULL;
    }
    dll_234_func_AA0(objdata);
    return 0;
}

// offset: 0x18D4 | func: 17 | anim state 5
static s32 dll_234_func_18D4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 0;
    if (objdata->unkE_0) {
        objdata->unkE_0 = 0;
        func_80023D30(self, 4, 0.0f, 0);
        objdata->unk0 = 0.02f;
    }
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    if ((self->curModAnimId == 4) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
        objdata->unkE_0 = 1;
    }
    dll_234_func_B00(objdata);
    return 0;
}

// offset: 0x1A28 | func: 18 | anim state 6
static s32 dll_234_func_1A28(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 0;
    if (objdata->unkE_0) {
        objdata->unkE_0 = 0;
        func_80023D30(self, 5, 0.0f, 0);
        objdata->unk0 = 0.0042f;
    }
    self->velocity.x *= 0.97f;
    self->velocity.z *= 0.97f;
    fsa->flags |= OBJSTATE_PRINT_DISABLED;
    self->srt.pitch -= (self->srt.pitch >> 2);
    obj_move(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if ((self->curModAnimId == 5) && (self->animProgress == 1.0f)) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 7);
        objdata->unkE_0 = 1;
        fsa->target = NULL;
    }
    dll_234_func_B00(objdata);
    return 0;
}

// offset: 0x1B94 | func: 19 | anim state 7
static s32 dll_234_func_1B94(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    ScorpionRobot_Data* objdata = baddie->objdata;

    fsa->unk341 = 3;
    if (objdata->unkE_0) {
        objdata->unkE_0 = 0;
        gDLL_33_BaddieControl->vtbl->func18(self, baddie->unk3E0, -1, 0);
        gDLL_18_objfsa->vtbl->func21(self, fsa, 0x3C, 0xA, 0);
        baddie->unk3B4 = 0;
        main_set_bits(baddie->unk39C, 1);
        if (self->setup == NULL) {
            obj_destroy_object(self);
        }
        obj_send_mesg_many(0, 3, self, 0xE0000, self);
    }

    if (self->opacity >= (gUpdateRate * 3)) {
        self->opacity -= (gUpdateRate * 3);
    } else {
        self->opacity = 0;
    }
    dll_234_func_B00(objdata);
    return 0;
}

// offset: 0x1D08 | func: 20 | logic state 0
static s32 dll_234_func_1D08(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}
