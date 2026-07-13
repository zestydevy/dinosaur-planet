#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/53_movelib.h"
#include "sys/objtype.h"
#include "sys/newshadows.h"

typedef struct {
/*000*/ ObjFSA_Data fsa;
/*34C*/ MoveLibData movedata;
/*804*/ HeadAnimation headAnim;
/*828*/ UnkCurvesStruct unk828;
/*930*/ f32 playerDist;
/*934*/ f32 attackCooldown;
/*938*/ u8 flags;
} WCTrex_Data;

enum WCTrexModAnims {
    WCTREX_ANIM_0_Idle = 0,
    WCTREX_ANIM_1_Walk = 1,
    WCTREX_ANIM_2_BiteLeft_Near = 2,
    WCTREX_ANIM_3_BiteLeft_Far = 3,
    WCTREX_ANIM_4_BiteRight_Far = 4,
    WCTREX_ANIM_5_BiteLeft_Farther = 5,
    WCTREX_ANIM_6_BiteRight_Farther = 6,
    WCTREX_ANIM_7_KickLeft = 7,
    WCTREX_ANIM_8_KickRight = 8
};

/*0x0*/ static ObjFSA_StateCallback bss_0[4];
/*0x10*/ static ObjFSA_StateCallback bss_10[4];

static void WCTrex_func_4A8(Object* self);
static s32 WCTrex_func_584(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCTrex_func_59C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCTrex_func_608(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCTrex_func_728(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCTrex_func_7E8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCTrex_func_800(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCTrex_func_8DC(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCTrex_func_9E4(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void WCTrex_func_0(void) {
    bss_0[0] = WCTrex_func_584;
    bss_0[1] = WCTrex_func_59C;
    bss_0[2] = WCTrex_func_608;
    bss_0[3] = WCTrex_func_728;
    bss_10[0] = WCTrex_func_7E8;
    bss_10[1] = WCTrex_func_800;
    bss_10[2] = WCTrex_func_8DC;
    bss_10[3] = WCTrex_func_9E4;
}

// offset: 0x84 | ctor
void WCTrex_ctor(void *dll) {
    WCTrex_func_0();
}

// offset: 0xC4 | dtor
void WCTrex_dtor(void *dll) { }

// offset: 0xD0 | func: 1 | export: 0
void WCTrex_setup(Object* self, ObjSetup* setup, s32 reset) {
    WCTrex_Data* objdata = self->data;
    s16 sp4C[] = {5,  5, 10, 10, 10, 10, 10, 10};
    s16 sp3C[] = {5, 10, 20, 20, 20, 20, 20, 20};
    s32 sp38 = 2;

    mainCreateTempDLL(DLL_ID_53_MOVELIB);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objdata->movedata, -0x2AAA, 0x638D, 8);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func6(&objdata->movedata, sp3C, sp4C, 8);
    objdata->movedata.unk4A9 |= 0x22;
    gDLL_26_Curves->vtbl->func_4288(&objdata->unk828, self, 1000.0f, &sp38, -1);
    gDLL_18_objfsa->vtbl->func0(self, &objdata->fsa, 4, 4);
    objAddObjectType(self, OBJTYPE_Baddie);
}

// offset: 0x270 | func: 2 | export: 1
void WCTrex_control(Object* self) {
    WCTrex_Data* objdata = self->data;

    objdata->playerDist = vec3_distance_xz(&self->globalPosition, &objGetPlayer()->globalPosition);
    objdata->fsa.flags |= OBJFSA_FLAG_2000000;
    gDLL_18_objfsa->vtbl->tick(self, &objdata->fsa, gUpdateRateF, gUpdateRateF, bss_0, bss_10);
    if (objdata->flags & 1) {
        objdata->movedata.unk4A9 &= ~1;
    } else {
        objdata->movedata.unk4A9 |= 1;
    }
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func0(self, &objdata->movedata);
    objExprEyeIdle(self, &objdata->headAnim);
    WCTrex_func_4A8(self);
}

// offset: 0x3B0 | func: 3 | export: 2
void WCTrex_update(Object *self) { }

// offset: 0x3BC | func: 4 | export: 3
void WCTrex_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    WCTrex_Data* objdata = self->data;

    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func3(self, &objdata->movedata, 0);
    }
}

// offset: 0x444 | func: 5 | export: 4
void WCTrex_free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_Baddie);
}

// offset: 0x484 | func: 6 | export: 5
u32 WCTrex_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x494 | func: 7 | export: 6
u32 WCTrex_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(WCTrex_Data);
}

// offset: 0x4A8 | func: 8
static void WCTrex_func_4A8(Object* self) {
    SRT srt;
    MtxF mtx;
    f32 x;
    f32 y;
    f32 z;

    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.scale = 1.0f;
    matrix_from_srt(&mtx, &srt);
    vec3_transform(&mtx, 0.0f, 0.0f, 0.0f, &x, &y, &z);
    shadowsSetCustomObjPos(self, x, y, z);
}

// offset: 0x584 | func: 9 | anim state 0
static s32 WCTrex_func_584(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 1 + 1;
}

// offset: 0x59C | func: 10 | anim state 1
static s32 WCTrex_func_59C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        objAnimSet(self, 0, 0.0f, 0);
        fsa->animTickDelta = 0.005f;
    }
    return 0;
}

// offset: 0x608 | func: 11 | anim state 2
static s32 WCTrex_func_608(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    WCTrex_Data* objdata = self->data;
    UnkCurvesStruct* curveStruct = &objdata->unk828;
    
    if (fsa->enteredAnimState != 0) {
        objAnimSet(self, 1, 0.0f, 0);
    }
    self->velocity.x = (curveStruct->unk0.unk68.x - self->srt.transl.x) * gUpdateRateInverseF;
    self->velocity.z = (curveStruct->unk0.unk68.z - self->srt.transl.z) * gUpdateRateInverseF;
    self->srt.transl.x = curveStruct->unk0.unk68.x;
    self->srt.transl.z = curveStruct->unk0.unk68.z;
    self->srt.yaw = arctan2_f(-curveStruct->unk0.unk74, -curveStruct->unk0.unk7C);
    objGetAnimChange(self, sqrtf(SQ(self->velocity.x) + SQ(self->velocity.z)), &fsa->animTickDelta);
    return 0;
}

// offset: 0x728 | func: 12 | anim state 3
static s32 WCTrex_func_728(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* player = objGetPlayer();
    s32 angle;
    f32 xDiff, zDiff;

    if (fsa->enteredAnimState != 0) {
        fsa->animTickDelta = 0.012f;
        xDiff = self->srt.transl.x - player->srt.transl.x;
        zDiff = self->srt.transl.z - player->srt.transl.z;
        angle = arctan2_f(xDiff, zDiff) - (self->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(angle);
        if (angle > 0x4000) {} // required to match, from default.dol
    }
    return 0;
}

// offset: 0x7E8 | func: 13 | logic state 0
static s32 WCTrex_func_7E8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 1 + 1;
}

// offset: 0x800 | func: 14 | logic state 1
static s32 WCTrex_func_800(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    WCTrex_Data* objdata = self->data;
    UnkCurvesStruct* temp_a0 = &objdata->unk828;

    if (fsa->enteredLogicState != 0) {
        objdata->flags &= ~1;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
    }
    if ((curves_func_800053B0(&temp_a0->unk0, 0.5f) != 0) || (temp_a0->unk0.unk10 != 0)) {
        if (gDLL_26_Curves->vtbl->func_4704(temp_a0) != 0) {
            STUBBED_PRINTF("ERROR: end of curve reached\n");
        }
    }
    if (objdata->playerDist < 200.0f) {
        return 2 + 1;
    }
    return 0;
}

// offset: 0x8DC | func: 15 | logic state 2
static s32 WCTrex_func_8DC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    WCTrex_Data* objdata = self->data;

    if (fsa->enteredLogicState != 0) {
        objdata->flags |= 1;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
    }
    objdata->attackCooldown -= gUpdateRateF;
    if (objdata->playerDist > 220.0f) {
        return 1 + 1;
    }
    if ((objdata->playerDist < 100.0f) && (objdata->attackCooldown <= 0.0f)) {
        objdata->attackCooldown = (f32) rand_next(120, 250);
        return 3 + 1;
    }
    return 0;
}

// offset: 0x9E4 | func: 16 | logic state 3
static s32 WCTrex_func_9E4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    WCTrex_Data* objdata = self->data;

    if (fsa->enteredLogicState != 0) {
        objdata->flags &= ~1;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
    } else {
        if (fsa->unk33A != 0) {
            return 2 + 1;
        }
    }
    return 0;
}
