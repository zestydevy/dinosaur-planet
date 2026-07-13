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
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objexpr.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/rand.h"
#include "sys/joypad.h"
#include "dll.h"
#include "macros.h"

typedef struct {
/*000*/ MoveLibData movedata;
/*4B8*/ s8 state;
/*4B9*/ u8 _unk4B9;
/*4BA*/ u8 flags;
/*4BB*/ u8 mapAct;
/*4BC*/ s8 grazingAltAnimSelector;
/*4BD*/ u8 nextSeq; // next seq in rotation to play on interact
/*4BE*/ u8 _unk4BE[0x4C0 - 0x4BE];
/*4C0*/ u8* talkSeqs;
/*4C4*/ u8 talkSeqsCount; // number of seqs in rotation
/*4C5*/ u8 _unk4C5[0x4D0 - 0x4C5];
/*4D0*/ s16 eatingTimer;
/*4D2*/ s16 grazingTimer;
/*4D4*/ s16 drinkTimer;
/*4D6*/ s16 targetAngle;
/*4D8*/ s16 angleToTarget;
/*4DA*/ s16 startAngle;
/*4DC*/ s16 progressionBlockerGamebit;
/*4E0*/ s32 unk4E0;
/*4E4*/ DLL27_Data collider;
/*744*/ u8 _unk744[0x75C - 0x744];
/*75C*/ CurveSetup* prevCurve;
/*760*/ CurveSetup* currentCurve;
/*764*/ CurveSetup* targetCurve;
/*768*/ u8 _unk768[0x804 - 0x768];
/*804*/ f32 modAnimDelta;
/*808*/ f32 walkSpeed;
/*80C*/ u8 _unk80C[0x840 - 0x80C];
/*840*/ f32 playerDist;
/*844*/ f32 distToTargetCurve;
/*848*/ f32 unk848;
/*84C*/ f32 unk84C;
/*850*/ HeadAnimation headAnim;
/*874*/ u8 unk874;
/*875*/ u8 _unk875[0x878 - 0x875];
} SHthorntail_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 thorntail; // which Thorntail this is
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

enum ThorntailFlags {
    THORNTAILFLAG_ModAnimDone = 0x1,
    THORNTAILFLAG_AtTarget = 0x2,
    THORNTAILFLAG_Walking = 0x4,
    THORNTAILFLAG_ForceModAnimChange = 0x8,
    THORNTAILFLAG_GoToNextNode = 0x10,
    THORNTAILFLAG_ProgressionBlocker = 0x20,
    THORNTAILFLAG_RotateTalkSeqs = 0x40
};

enum ThorntailState {
    THORNTAILSTATE_Grazing = 0,
    THORNTAILSTATE_Grazing_LookingAround = 1,
    THORNTAILSTATE_Grazing_Swallowing = 2,
    THORNTAILSTATE_MoveToTarget = 3,
    THORNTAILSTATE_TurnLeft = 4,
    THORNTAILSTATE_TurnRight = 5,
    THORNTAILSTATE_StartDrinking = 6,
    THORNTAILSTATE_Drinking = 7,
    THORNTAILSTATE_DoneDrinking = 8,
    THORNTAILSTATE_Idle = 9,
    THORNTAILSTATE_BlockingProgression = 10,
    THORNTAILSTATE_WakingUp = 11,
    THORNTAILSTATE_LookingAround = 12,
    THORNTAILSTATE_Sleeping = 13,
    THORNTAILSTATE_FallingAsleep = 14
};

enum ThorntailStateFlags {
    THORNTAILSTATEFLAG_NoTargeting = 0x1,
    THORNTAILSTATEFLAG_Asleep = 0x2
};

enum ThorntailIndex {
    THORNTAIL_1_Sleepy = 1,     // The sleepy-voiced ThornTail near the tree hollow/river crossing
    THORNTAIL_2_Log_Trader = 2, // The ThornTail near the burrows behind Rocky
    THORNTAIL_3_Elder = 3       // The elderly ThornTail near Willow Grove
};

enum ThorntailCurveSubtype {
    // trader middle nodes, can always navigate to.
    THORNTAILCURVE_0 = 0,
    // in front of well (sleepy), in front of burrows (trader), spore patch (elder).
    // never returned to.
    THORNTAILCURVE_3 = 3,
    // river deadend (for drinking) (sleepy).
    // can navigate to if unk4E0 == 9
    THORNTAILCURVE_4 = 4,
    // elder nodes, most of sleepy nodes, trader dead end.
    // can navigate to if unk4E0 != 9
    THORNTAILCURVE_8 = 8,
    // some of sleepy's nodes are this.
    // can always navigate to.
    THORNTAILCURVE_9 = 9
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
// Model animations per state (one base and layered anim per state)
/*0x16C*/ static s16 sModAnimMap[] = {
//     base    layered
/*00*/ 0x0007, 0xffff, // eating grass
/*01*/ 0x0008, 0xffff, // after eating grass (looking around)
/*02*/ 0x0009, 0xffff, // after eating grass (swallowing?)
/*03*/ 0x000f, 0x0001, // walking (layered includes looking down?)
/*04*/ 0x000c, 0x0000, // turning left
/*05*/ 0x000d, 0x0000, // turning right
/*06*/ 0x0010, 0xffff, // starting to drink
/*07*/ 0x0011, 0xffff, // drinking from river
/*08*/ 0x0012, 0xffff, // after drinking from river
/*09*/ 0x0000, 0xffff, // idle
/*10*/ 0x0004, 0xffff, // sleeping
/*11*/ 0x0005, 0xffff, // waking up
/*12*/ 0x0013, 0xffff, // looking around
/*13*/ 0x0004, 0xffff, // sleeping
/*14*/ 0x0003, 0xffff  // falling asleep
};
// Flags per state
/*0x1A8*/ static u8 sStateFlagMap[] = {
/*00*/ 0, 
/*01*/ 0, 
/*02*/ 0, 
/*03*/ 0, 
/*04*/ 0, 
/*05*/ 0, 
/*06*/ 0, 
/*07*/ 0, 
/*08*/ 0, 
/*09*/ 0, 
/*10*/ THORNTAILSTATEFLAG_NoTargeting | THORNTAILSTATEFLAG_Asleep, 
/*11*/ THORNTAILSTATEFLAG_NoTargeting, 
/*12*/ 0, 
/*13*/ THORNTAILSTATEFLAG_NoTargeting | THORNTAILSTATEFLAG_Asleep, 
/*14*/ THORNTAILSTATEFLAG_NoTargeting | THORNTAILSTATEFLAG_Asleep
};
/*0x1B8*/ static f32 data_1B8[] = {
    0.006f, 0.004f, 0.006f, 0.0065f, 0.007f, 0.007f, 0.006f, 0.006f, 
    0.006f, 0.006f, 0.006f, 0.006f, 0.003f, 0.006f, 0.006f
};
/*0x1F4*/ static u8 sSleepyTalkSeqs[1] = {
    THORNTAILSEQ_0_HaveYouWokenTheSwapStone
};
/*0x1F8*/ static u8 sTraderTalkSeqs[3] = {
    THORNTAILSEQ_1_LeaveMeAloneStranger, 
    THORNTAILSEQ_2_ALogForAFish, 
    THORNTAILSEQ_3_IDontWantThat
};
/*0x1FC*/ static u8 sElderTalkSeqs[1] = {
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

static void thorntail_update_shadow(Object* self);
static int thorntail_anim_callback(Object *actor, Object *animObj, AnimObj_Data *animObjData, s8 a3);
static void thorntail_sleepy_setup(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_sleepy_init(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_sleepy_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_common_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_sleepy_act2_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_sleepy_act3_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_sleepy_act4_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_trader_setup(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_trader_init(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_trader_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_trader_act1_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_trader_act2_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_trader_act3_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_trader_act4_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_elder_setup(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_elder_init(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup);
static void thorntail_elder_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_elder_act1_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_elder_act2_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_elder_act3_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);
static void thorntail_elder_act4_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup);

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

    switch (setup->thorntail) {
    case THORNTAIL_1_Sleepy:
        thorntail_sleepy_setup(self, objdata, setup);
        break;
    case THORNTAIL_2_Log_Trader:
        thorntail_trader_setup(self, objdata, setup);
        break;
    case THORNTAIL_3_Elder:
        thorntail_elder_setup(self, objdata, setup);
        break;
    }
    gDLL_27->vtbl->init(&objdata->collider, DLL27FLAG_4000000 | DLL27FLAG_2000000, DLL27FLAG_400, DLL27MODE_1);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->collider, 4, data_0, data_30, sp3C);
    gDLL_27->vtbl->reset(self, &objdata->collider);
    self->shadow->flags |= (OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_OBJ_POS | OBJ_SHADOW_FLAG_CUSTOM_DIR);
    self->shadow->distFadeMaxOpacity = 128;
    self->shadow->distFadeMinOpacity = 90;
    self->animCallback = thorntail_anim_callback;
    objdata->state = -1;
    mainCreateTempDLL(DLL_ID_53_MOVELIB);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objdata->movedata, -0x1FFF, 0x2AAA, 3);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func5(&objdata->movedata, 0x190, 0x1E);
    objdata->movedata.unk4A9 &= ~0x8;
    objAddObjectType(self, OBJTYPE_40);
}

// offset: 0x248 | func: 1 | export: 1
void thorntail_control(Object* self) {
    SHthorntail_Data* objdata;
    SHthorntail_Setup* setup;
    Object* player;

    objdata = self->data;
    setup = (SHthorntail_Setup*)self->setup;
    player = objGetPlayer();
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    if (func_80026DF4(self, data_40, 15, objdata->unk874, &objdata->unk84C) == 0) {
        objdata->unk874 = 0;
        objdata->mapAct = gDLL_29_Gplay->vtbl->get_act(self->mapID);
        objdata->playerDist = vec3_distance(&self->globalPosition, &player->globalPosition);
        switch (setup->thorntail) {
        case THORNTAIL_1_Sleepy:
            thorntail_sleepy_control(self, objdata, setup);
            break;
        case THORNTAIL_2_Log_Trader:
            thorntail_trader_control(self, objdata, setup);
            break;
        case THORNTAIL_3_Elder:
            thorntail_elder_control(self, objdata, setup);
            break;
        }
        thorntail_update_shadow(self);
        gDLL_27->vtbl->func_1E8(self, &objdata->collider, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, &objdata->collider);
        gDLL_27->vtbl->func_624(self, &objdata->collider, gUpdateRateF);
        if (!(sStateFlagMap[objdata->state] & THORNTAILSTATEFLAG_Asleep)) {
            objExprEyeIdle(self, &objdata->headAnim);
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
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func3(self, &objdata->movedata, 0);
    }
}

// offset: 0x504 | func: 4 | export: 4
void thorntail_free(Object *self, s32 onlySelf) {
    mainRemoveTempDLL(DLL_ID_53_MOVELIB);
    objFreeObjectType(self, OBJTYPE_40);
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
static void thorntail_update_shadow(Object *self) {
    MtxF mtx;

    matrix_from_srt(&mtx, &self->srt);
    vec3_transform(&mtx, 0.0f, 0.0f, 0.0f, &self->shadow->tr.x, &self->shadow->tr.y, &self->shadow->tr.z);
}

// offset: 0x608 | func: 8
static CurveSetup* thorntail_find_closest_curve(f32 x, f32 y, f32 z, s32 curveSubtype) {
    CurveNode* nodes;
    CurveSetup* curve;
    CurveSetup* closestCurve;
    f32 closestDist;
    s32 i;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 numNodes;

    nodes = gDLL_26_Curves->vtbl->func_1BC(&numNodes);
    closestCurve = NULL;
    closestDist = 2.5e9f;
    for (i = 0; i < numNodes; i++) {
        curve = nodes[i].setup;
        if ((curve->curveType == 0x1D) && (curveSubtype == curve->type1D.unk34)) {
            dx = curve->pos.x - x;
            dy = curve->pos.y - y;
            dz = curve->pos.z - z;
            dx = SQ(dx) + SQ(dy) + SQ(dz);
            if (dx < closestDist) {
                closestDist = dx;
                closestCurve = curve;
            }
        }
    }
    return closestCurve;
}

// offset: 0x8CC | func: 9
static int thorntail_anim_callback(Object *actor, Object *animObj, AnimObj_Data *animObjData, s8 a3) {
    SHthorntail_Data *objdata = actor->data;

    if (((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func4(actor, animObjData, &objdata->movedata, 1, 1) != 0) {
        return 1;
    }
    if (a3 != 0) {
        objAnimAdvance(actor, data_1B8[objdata->state], gUpdateRateF, NULL);
    }
    animObjData->unk62 = 0;
    animObjData->unk7A &= ~(ANIM7AFLAG_OVERRIDE_EYES | ANIM7AFLAG_OVERRIDE_HEAD);
    actor->unkAF |= ARROW_FLAG_8_No_Targetting;
    return 0;
}

// offset: 0x9B8 | func: 10
static void thorntail_sleepy_setup(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    objdata->flags |= THORNTAILFLAG_RotateTalkSeqs;
    objdata->progressionBlockerGamebit = BIT_SH_Move_Thorntail_Blocking_Hollow_Log;
    thorntail_sleepy_init(self, objdata, setup);
}

// offset: 0xA04 | func: 11
static void thorntail_sleepy_init(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    s32 curveSubtype;

    if (mainGetBits(objdata->progressionBlockerGamebit) == 0) {
        objdata->flags |= THORNTAILFLAG_ProgressionBlocker;
        curveSubtype = THORNTAILCURVE_3;
    } else {
        objdata->progressionBlockerGamebit = -1;
        curveSubtype = THORNTAILCURVE_8;
    }
    objdata->currentCurve = thorntail_find_closest_curve(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, curveSubtype);
    if (objdata->currentCurve == NULL) {
        STUBBED_PRINTF("THORNTAIL: cannot find a node\n");
    }
    objdata->flags |= THORNTAILFLAG_AtTarget;
    self->srt.transl.x = objdata->currentCurve->pos.x;
    self->srt.transl.y = objdata->currentCurve->pos.y;
    self->srt.transl.z = objdata->currentCurve->pos.z;
    self->srt.yaw = objdata->currentCurve->unk2C << 8;
    objdata->eatingTimer = rand_next(500, 800);
    objdata->grazingTimer = rand_next(2000, 3000);
    objdata->grazingAltAnimSelector = rand_next(1, 2);
    objdata->talkSeqs = sSleepyTalkSeqs;
    objdata->talkSeqsCount = ARRAYCOUNT(sSleepyTalkSeqs);
}

// offset: 0xB40 | func: 12
static void thorntail_sleepy_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup) {
    switch (objdata->mapAct) {
    default:
    case 1:
        thorntail_common_control(self, objdata, setup);
        break;
    case 2:
        thorntail_sleepy_act2_control(self, objdata, setup);
        break;
    case 3:
        thorntail_sleepy_act3_control(self, objdata, setup);
        break;
    case 4:
        thorntail_sleepy_act4_control(self, objdata, setup);
        break;
    }
}

// offset: 0xBEC | func: 13
static void thorntail_common_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup) {
    s32 ignoreUID;
    s32 allowBranch;
    s32 numBranches;
    s32 numNodeOptions;
    s32 branches[4];
    s32 i;
    CurveSetup* branch;
    CurveSetup* nodeOptions[4];
    s32 animIdx;
    f32 vz;
    f32 vx;
    f32 time;
    UnkFunc_80024108Struct sp3C;
    TextureAnimator* eyelidR;
    TextureAnimator* eyelidL;

    objdata->flags &= ~THORNTAILFLAG_Walking;
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        gDLL_3_Animation->vtbl->start_obj_sequence(objdata->talkSeqs[objdata->nextSeq], self, -1);
        if (objdata->flags & THORNTAILFLAG_RotateTalkSeqs) {
            objdata->nextSeq++;
            if (objdata->nextSeq >= objdata->talkSeqsCount) {
                objdata->nextSeq = 0;
            }
        }
        return;
    }
    if (objdata->flags & THORNTAILFLAG_GoToNextNode) {
        // If at navigable deadend, don't ignore the node we came from so we don't get stuck
        if ((objdata->prevCurve != NULL) && (objdata->currentCurve->type1D.unk34 != THORNTAILCURVE_4)) {
            ignoreUID = objdata->prevCurve->uID;
        } else {
            ignoreUID = -1;
        }
        // Lookup branches for the current curve node
        numBranches = gDLL_26_Curves->vtbl->func_4F0(objdata->currentCurve, ignoreUID, branches);
        numNodeOptions = 0;
        for (i = 0; i < numBranches; i++) {
            branch = gDLL_26_Curves->vtbl->func_39C(branches[i]);
            if (branch != NULL) {
                allowBranch = FALSE;
                switch (objdata->unk4E0) {
                case 9:
                    if ((branch->type1D.unk34 == THORNTAILCURVE_9) || 
                        (branch->type1D.unk34 == THORNTAILCURVE_4) || 
                        (branch->type1D.unk34 == THORNTAILCURVE_0)) {
                        allowBranch = TRUE;
                    }
                    break;
                case 8:
                default:
                    if ((branch->type1D.unk34 == THORNTAILCURVE_8) || 
                        (branch->type1D.unk34 == THORNTAILCURVE_9) || 
                        (branch->type1D.unk34 == THORNTAILCURVE_0)) {
                        allowBranch = TRUE;
                    }
                    break;
                }
                if (allowBranch) {
                    nodeOptions[numNodeOptions] = branch;
                    numNodeOptions += 1;
                }
            }
        }
        if (numNodeOptions != 0) {
            // Choose random eligible branch
            objdata->targetCurve = nodeOptions[rand_next(0, numNodeOptions - 1)];
            objdata->prevCurve = objdata->currentCurve;
            objdata->flags &= ~THORNTAILFLAG_AtTarget;
        } else {
            // No eligible branches, fallback to the closest subtype 0 node.
            // @bug: This is why the Thorntail by Willow Grove walks up the wall into the SwapStone area
            //       sometimes. Subtype 0 nodes are only found on the log trader's curve network.
            STUBBED_PRINTF("THORNTAIL: help cannot find a node\n");
            // STUBBED_PRINTF("Thorntail %d, is on a network with a deadend\n", setup->base.uID); // default.dol
            objdata->targetCurve = thorntail_find_closest_curve(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, THORNTAILCURVE_0);
            if (objdata->targetCurve != NULL) {
                objdata->prevCurve = objdata->currentCurve;
                objdata->flags &= ~THORNTAILFLAG_AtTarget;
            }
        }
        objdata->flags &= ~THORNTAILFLAG_GoToNextNode;
    }
    if (objdata->flags & THORNTAILFLAG_AtTarget) {
        // Currently at target node
        objdata->grazingTimer -= gUpdateRate;
        if (objdata->grazingTimer < 0) {
            objdata->grazingTimer = 0;
        }
        if (!(sStateFlagMap[objdata->state] & THORNTAILSTATEFLAG_Asleep) && (gDLL_7_Newday->vtbl->func8(&time) != 0)) {
            // Time changed to night, force state switch to falling asleep
            objdata->state = THORNTAILSTATE_FallingAsleep;
        }
        switch (objdata->state) {
        case THORNTAILSTATE_Grazing:
            objdata->eatingTimer -= gUpdateRate;
            if (objdata->eatingTimer < 0) {
                objdata->eatingTimer = 0;
            }
            if ((objdata->flags & THORNTAILFLAG_ModAnimDone) && (objdata->eatingTimer <= 0)) {
                // After eating (but not necessarily done grazing), play an alternate animation for a bit
                if (objdata->grazingAltAnimSelector <= 0) {
                    objdata->state = THORNTAILSTATE_Grazing_Swallowing;
                } else {
                    objdata->state = THORNTAILSTATE_Grazing_LookingAround;
                }
            }
            if (objdata->grazingTimer <= 0) {
                if (objdata->unk4E0 == 9) {
                    objdata->flags |= THORNTAILFLAG_GoToNextNode;
                } else {
                    objdata->state = THORNTAILSTATE_LookingAround;
                }
            } else if (joyGetPressed(0) & D_JPAD) {
                objdata->unk4E0 = 9;
                objdata->flags |= THORNTAILFLAG_GoToNextNode;
            }
            break;
        case THORNTAILSTATE_LookingAround:
            // Delays movement to the next node
            if (objdata->flags & THORNTAILFLAG_ModAnimDone) {
                objdata->unk4E0 = 8;
                objdata->flags |= THORNTAILFLAG_GoToNextNode;
            }
            break;
        case THORNTAILSTATE_Grazing_LookingAround:
            if (objdata->flags & THORNTAILFLAG_ModAnimDone) {
                objdata->state = THORNTAILSTATE_Grazing;
                objdata->grazingAltAnimSelector = objdata->grazingAltAnimSelector - 1;
                objdata->eatingTimer = rand_next(500, 800);
            }
            break;
        case THORNTAILSTATE_Grazing_Swallowing:
            if (objdata->flags & THORNTAILFLAG_ModAnimDone) {
                objdata->state = THORNTAILSTATE_Grazing;
                objdata->grazingAltAnimSelector = rand_next(1, 2);
                objdata->eatingTimer = rand_next(500, 800);
            }
            break;
        case THORNTAILSTATE_StartDrinking:
            if (objdata->flags & THORNTAILFLAG_ModAnimDone) {
                objdata->state = THORNTAILSTATE_Drinking;
                objdata->drinkTimer = rand_next(1000, 2000);
            }
            break;
        case THORNTAILSTATE_Drinking:
            objdata->drinkTimer -= gUpdateRate;
            if (objdata->drinkTimer < 0) {
                objdata->drinkTimer = 0;
            }
            if ((objdata->flags & THORNTAILFLAG_ModAnimDone) && (objdata->drinkTimer <= 0)) {
                objdata->state = THORNTAILSTATE_DoneDrinking;
            }
            break;
        case THORNTAILSTATE_DoneDrinking:
            if (objdata->flags & THORNTAILFLAG_ModAnimDone) {
                objdata->state = THORNTAILSTATE_Idle;
            }
            break;
        case THORNTAILSTATE_Idle:
            if (objdata->flags & THORNTAILFLAG_ModAnimDone) {
                objdata->unk4E0 = 8;
                objdata->flags |= THORNTAILFLAG_GoToNextNode;
            }
            break;
        case THORNTAILSTATE_BlockingProgression:
            // Sleep regardless of time of day to block player progression
            if (mainGetBits(objdata->progressionBlockerGamebit) != 0) {
                objdata->flags &= ~THORNTAILFLAG_ProgressionBlocker;
                objdata->flags |= THORNTAILFLAG_ForceModAnimChange;
                objdata->state = THORNTAILSTATE_WakingUp;
            }
            break;
        case THORNTAILSTATE_WakingUp:
            if (objdata->flags & THORNTAILFLAG_ModAnimDone) {
                objdata->unk4E0 = 8;
                objdata->flags |= THORNTAILFLAG_GoToNextNode;
            }
            break;
        case THORNTAILSTATE_FallingAsleep:
            if (objdata->flags & THORNTAILFLAG_ModAnimDone) {
                objdata->state = THORNTAILSTATE_Sleeping;
            }
            break;
        case THORNTAILSTATE_Sleeping:
            if (gDLL_7_Newday->vtbl->func8(&time) == 0) {
                objdata->state = THORNTAILSTATE_WakingUp;
            }
            break;
        default:
            if (objdata->flags & THORNTAILFLAG_ProgressionBlocker) {
                // Acting as a progression blocker
                objdata->state = THORNTAILSTATE_BlockingProgression;
            } else if (objdata->currentCurve->type1D.unk34 == THORNTAILCURVE_4) {
                // At river
                objdata->state = THORNTAILSTATE_StartDrinking;
            } else if (gDLL_7_Newday->vtbl->func8(&time) != 0) {
                // Nighttime
                objdata->state = THORNTAILSTATE_FallingAsleep;
            } else {
                // Eat that lovely SwapStone Hollow grass
                objdata->state = THORNTAILSTATE_Grazing;
                objdata->eatingTimer = rand_next(500, 800);
                objdata->grazingTimer = rand_next(2000, 3000);
            }
            break;
        }
    } else {
        // Moving toward next curve node
        objdata->distToTargetCurve = vec3_distance_xz(&self->globalPosition, &objdata->targetCurve->pos);
        if ((objdata->distToTargetCurve < 30.0f) && ((objdata->flags & THORNTAILFLAG_ModAnimDone) != 0)) {
            // Reached target curve
            objdata->flags |= THORNTAILFLAG_AtTarget;
            objdata->currentCurve = objdata->targetCurve;
            if (((objdata->unk4E0 == 9) && (objdata->currentCurve->type1D.unk34 != THORNTAILCURVE_4)) 
                    || (objdata->currentCurve->type1D.unk34 == THORNTAILCURVE_0)) {
                objdata->state = -1;
                objdata->flags |= THORNTAILFLAG_GoToNextNode;
            }
        } else {
            // Not at target curve yet
            switch (objdata->state) {
            case THORNTAILSTATE_Grazing:
            default:
                vx = objdata->targetCurve->pos.x - objdata->prevCurve->pos.x;
                vz = objdata->targetCurve->pos.z - objdata->prevCurve->pos.z;
                objdata->targetAngle = arctan2_f(-vx, -vz);
                objdata->startAngle = self->srt.yaw;
                objdata->angleToTarget = self->srt.yaw - (objdata->targetAngle & 0xFFFF);
                CIRCLE_WRAP(objdata->angleToTarget);
                if ((objdata->angleToTarget < 500) && (objdata->angleToTarget > -500)) {
                    objdata->state = THORNTAILSTATE_MoveToTarget;
                } else {
                    if (objdata->angleToTarget > M_90_DEGREES) {
                        objdata->angleToTarget = M_90_DEGREES;
                    } else if (objdata->angleToTarget < -M_90_DEGREES) {
                        objdata->angleToTarget = -M_90_DEGREES;
                    }
                    if (objdata->angleToTarget < 0) {
                        objdata->state = THORNTAILSTATE_TurnLeft;
                        objdata->unk848 = (f32) -objdata->angleToTarget;
                    } else {
                        objdata->state = THORNTAILSTATE_TurnRight;
                        objdata->unk848 = (f32) objdata->angleToTarget;
                    }
                    objdata->unk848 = (f32) (objdata->unk848 / M_90_DEGREES_F);
                    if (objdata->unk848 > 1.0f) {
                        objdata->unk848 = 1.0f;
                    } else if (objdata->unk848 < 0.0f) {
                        objdata->unk848 = 0.0f;
                    }
                }
                break;
            case THORNTAILSTATE_MoveToTarget:
                if ((objdata->unk4E0 == 8) && (objdata->targetCurve->type1D.unk34 != THORNTAILCURVE_0)) {
                    objdata->unk848 = (objdata->distToTargetCurve - 35.0f) / 45.0f;
                    if (objdata->unk848 < 0.0f) {
                        objdata->unk848 = 0.0f;
                    } else if (objdata->unk848 > 1.0f) {
                        objdata->unk848 = 1.0f;
                    }
                } else {
                    objdata->unk848 = 1.0f;
                }
                vx = objdata->targetCurve->pos.x - self->srt.transl.x;
                vz = objdata->targetCurve->pos.z - self->srt.transl.z;
                if (objdata->distToTargetCurve != 0.0f) {
                    vx /= objdata->distToTargetCurve;
                    vz /= objdata->distToTargetCurve;
                }
                objdata->flags |= THORNTAILFLAG_Walking;
                objdata->walkSpeed = 0.28f;
                self->velocity.x = objdata->walkSpeed * vx;
                self->velocity.z = objdata->walkSpeed * vz;
                objMove(self, self->velocity.x * gUpdateRateF, 0.0f, self->velocity.z * gUpdateRateF);
                objGetAnimChange(self, objdata->walkSpeed, &objdata->modAnimDelta);
                break;
            case THORNTAILSTATE_TurnLeft:
            case THORNTAILSTATE_TurnRight:
                if (objdata->flags & THORNTAILFLAG_ModAnimDone) {
                    objdata->startAngle = self->srt.yaw;
                    objdata->angleToTarget = self->srt.yaw - (objdata->targetAngle & 0xFFFF);
                    CIRCLE_WRAP(objdata->angleToTarget);
                    if ((objdata->angleToTarget < 500) && (objdata->angleToTarget > -500)) {
                        objdata->state = THORNTAILSTATE_MoveToTarget;
                        objdata->unk848 = 1.0f;
                    } else {
                        if (objdata->angleToTarget > M_90_DEGREES) {
                            objdata->angleToTarget = M_90_DEGREES;
                        } else if (objdata->angleToTarget < -M_90_DEGREES) {
                            objdata->angleToTarget = M_90_DEGREES;
                        }
                        if (objdata->angleToTarget < 0) {
                            objdata->unk848 = (f32) -objdata->angleToTarget;
                        } else {
                            objdata->unk848 = (f32) objdata->angleToTarget;
                        }
                        objdata->unk848 = (f32) (objdata->unk848 / M_90_DEGREES_F);
                        if (objdata->unk848 > 1.0f) {
                            objdata->unk848 = 1.0f;
                        } else if (objdata->unk848 < 0.0f) {
                            objdata->unk848 = 0.0f;
                        }
                        objdata->flags |= THORNTAILFLAG_ForceModAnimChange;
                    }
                } else {
                    self->srt.yaw = (s16) ((f32) objdata->startAngle - (self->animProgress * (f32) objdata->angleToTarget));
                }
                break;
            }
        }
    }
    if (objdata->state != -1) {
        animIdx = objdata->state * 2;
        // Update model animation
        if ((sModAnimMap[animIdx] != self->curModAnimId) || (objdata->flags & THORNTAILFLAG_ForceModAnimChange)) {
            objAnimSet(self, sModAnimMap[animIdx], 0.0f, 0);
            if (!(objdata->flags & THORNTAILFLAG_Walking)) {
                objdata->modAnimDelta = data_1B8[objdata->state];
            }
            objdata->flags &= ~(THORNTAILFLAG_ForceModAnimChange | THORNTAILFLAG_ModAnimDone);
        }
        // Set layered model animation(?), if one is defined
        if (sModAnimMap[animIdx + 1] != -1) {
            objAnimSetBlend(self, sModAnimMap[animIdx + 1], (s32) (objdata->unk848 * 1023.0f));
        }
    }
    // Update z lock state
    if (sStateFlagMap[objdata->state] & THORNTAILSTATEFLAG_NoTargeting) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }
    // Update eyes
    eyelidR = objExprGetTexAnimator(self, HEAD_ANIMATION_TAG_Eyelid_R, 0);
    eyelidL = objExprGetTexAnimator(self, HEAD_ANIMATION_TAG_Eyelid_L, 0);
    if (sStateFlagMap[objdata->state] & THORNTAILSTATEFLAG_Asleep) {
        if (eyelidR != NULL) {
            eyelidR->frame = (2 << 8);
        }
        if (eyelidL != NULL) {
            eyelidL->frame = (2 << 8);
        }
    } else {
        if (eyelidR != NULL) {
            eyelidR->frame = 0;
        }
        if (eyelidL != NULL) {
            eyelidL->frame = 0;
        }
    }
    // Check if model animation completed
    if (objAnimAdvance(self, objdata->modAnimDelta, gUpdateRateF, &sp3C) != 0) {
        objdata->flags |= THORNTAILFLAG_ModAnimDone;
    } else {
        objdata->flags &= ~THORNTAILFLAG_ModAnimDone;
    }
}

// offset: 0x19E0 | func: 14
static void thorntail_sleepy_act2_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x19F4 | func: 15
static void thorntail_sleepy_act3_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1A08 | func: 16
static void thorntail_sleepy_act4_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1A1C | func: 17
static void thorntail_trader_setup(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    thorntail_trader_init(self, objdata, setup);
}

// offset: 0x1A58 | func: 18
static void thorntail_trader_init(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    s32 curveSubtype;

    if (mainGetBits(BIT_14) == 0) {
        objdata->progressionBlockerGamebit = BIT_14;
        objdata->flags |= THORNTAILFLAG_ProgressionBlocker;
        curveSubtype = THORNTAILCURVE_3;
    } else {
        objdata->progressionBlockerGamebit = -1;
        curveSubtype = THORNTAILCURVE_8;
    }
    objdata->currentCurve = thorntail_find_closest_curve(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, curveSubtype);
    if (objdata->currentCurve == NULL) {
        STUBBED_PRINTF("THORNTAIL: cannot find a node\n");
    }
    objdata->flags |= THORNTAILFLAG_AtTarget;
    self->srt.transl.x = objdata->currentCurve->pos.x;
    self->srt.transl.y = objdata->currentCurve->pos.y;
    self->srt.transl.z = objdata->currentCurve->pos.z;
    self->srt.yaw = objdata->currentCurve->unk2C << 8;
    objdata->eatingTimer = rand_next(500, 800);
    objdata->grazingTimer = rand_next(2000, 3000);
    objdata->grazingAltAnimSelector = rand_next(1, 2);
    objdata->talkSeqs = sTraderTalkSeqs;
    objdata->talkSeqsCount = ARRAYCOUNT(sTraderTalkSeqs);
}

// offset: 0x1B9C | func: 19
static void thorntail_trader_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    switch (objdata->mapAct) {
    default:
    case 1:
        thorntail_trader_act1_control(self, objdata, setup);
        break;
    case 2:
        thorntail_trader_act2_control(self, objdata, setup);
        break;
    case 3:
        thorntail_trader_act3_control(self, objdata, setup);
        break;
    case 4:
        thorntail_trader_act4_control(self, objdata, setup);
        break;
    }
}

// offset: 0x1C48 | func: 20
static void thorntail_trader_act1_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    Object *sidekick;
    Object *player;

    sidekick = objGetSidekick();
    player = objGetPlayer();
    thorntail_common_control(self, objdata, setup);
    if ((objdata->state == THORNTAILSTATE_BlockingProgression) && (sidekick != NULL)) {
        if (vec3_distance_squared(&player->globalPosition, &self->globalPosition) < SQ(70.0f)) {
            // Allow distract command
            ((DLL_ISidekick*)sidekick->dll)->vtbl->enable_command(sidekick, Sidekick_Command_INDEX_2_Distract);
        }
        if (((DLL_ISidekick*)sidekick->dll)->vtbl->func24(sidekick) != 0) {
            // Distract successfully used
            mainSetBits(objdata->progressionBlockerGamebit, 1);
        }
    }
}

// offset: 0x1D70 | func: 21
static void thorntail_trader_act2_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1D84 | func: 22
static void thorntail_trader_act3_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1D98 | func: 23
static void thorntail_trader_act4_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1DAC | func: 24
static void thorntail_elder_setup(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    objdata->flags |= THORNTAILFLAG_RotateTalkSeqs;
    objdata->progressionBlockerGamebit = BIT_SH_Move_Thorntail_Blocking_Swapstone;
    thorntail_elder_init(self, objdata, setup);
}

// offset: 0x1DF8 | func: 25
static void thorntail_elder_init(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) {
    thorntail_sleepy_init(self, objdata, setup);
    objdata->talkSeqs = sElderTalkSeqs;
    objdata->talkSeqsCount = ARRAYCOUNT(sElderTalkSeqs);
}

// offset: 0x1E4C | func: 26
static void thorntail_elder_control(Object* self, SHthorntail_Data* objdata, SHthorntail_Setup* setup) {
    thorntail_sleepy_control(self, objdata, setup);
    switch (objdata->mapAct) {
    case 1:
        thorntail_elder_act1_control(self, objdata, setup);
        return;
    case 2:
        thorntail_elder_act2_control(self, objdata, setup);
        break;
    case 3:
        thorntail_elder_act3_control(self, objdata, setup);
        break;
    case 4:
        thorntail_elder_act4_control(self, objdata, setup);
        break;
    }
}

// offset: 0x1F28 | func: 27
static void thorntail_elder_act1_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1F3C | func: 28
static void thorntail_elder_act2_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1F50 | func: 29
static void thorntail_elder_act3_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }

// offset: 0x1F64 | func: 30
static void thorntail_elder_act4_control(Object *self, SHthorntail_Data *objdata, SHthorntail_Setup *setup) { }
