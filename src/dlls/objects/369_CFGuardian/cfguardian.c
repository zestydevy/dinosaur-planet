#include "dlls/engine/27.h"
#include "dlls/engine/53_movelib.h"
#include "dlls/objects/common/collectable.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "sys/print.h"
#include "sys/joypad.h"
#include "sys/rand.h"
#include "dll.h"
#include "macros.h"

/** @file: official filename: cfguardian.c */

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw8;
} CFGuardian_Setup;

typedef struct {
/*000*/ MoveLibData movedata;
/*4B8*/ HeadAnimation exprHeadAnim;
/*4DC*/ HeadAnimation eyeIdleHeadAnim;
/*500*/ u32 unk500; // unused sound handle
/*504*/ u8 _unk504[0x50C - 0x504];
/*50C*/ Object* dustObjs[6];
/*524*/ Collectable_Setup* dustSetups[6];
/*53C*/ UnkCurvesStruct unk53C;
/*644*/ u8 _unk644[0x67C - 0x644];
/*67C*/ f32 animRate;
/*680*/ DLL27_Data collider;
/*8E0*/ SRT walkTarget;
/*8F8*/ u8 state;
/*8F9*/ u8 _unk8F9[0x908 - 0x8F9];
/*908*/ s32 unk908;
/*90C*/ s32 windLiftState;
/*910*/ u8 talkState;
/*911*/ s8 talkSeqSelector;
/*912*/ u8 flags;
} CFGuardian_Data;

// size: 0xC
typedef struct {
/*0*/ s32 key;
/*4*/ s32 values[2];
} CFGuardianMapStruct;

enum CFGuardianState {
    CFGUARDIAN_STATE_InCell = 0,
    CFGUARDIAN_STATE_WaitingToBeFreed = 1,
    CFGUARDIAN_STATE_LeavingCell = 2, // walking through prison
    CFGUARDIAN_STATE_WaitingAtWindLift = 3,
    CFGUARDIAN_STATE_WaitingForWindLiftPower = 4,
    CFGUARDIAN_STATE_5 = 5, // unused
    CFGUARDIAN_STATE_InWindShaft = 6,
    CFGUARDIAN_STATE_LeavingWindShaft = 7, // leaving top of wind shaft
    CFGUARDIAN_STATE_Courtyard_WaitingForKyte = 8, // waiting for kyte to be freed
    CFGUARDIAN_STATE_Courtyard_WaitingForCloudBaby = 9, // waiting for cloud baby rescue
    CFGUARDIAN_STATE_WalkingToRaceArea = 10, // walk to drained water
    CFGUARDIAN_STATE_Vanish = 11, // vanish (after reaching drained water)
    CFGUARDIAN_STATE_WaitingForRaceCompletion = 12,
    CFGUARDIAN_STATE_WaitingForSpellStone = 13,
    CFGUARDIAN_STATE_NoOp_SpellStoneActivated = 14,
    CFGUARDIAN_STATE_NoOp_Vanished = 15
};

enum CFGuardianModAnim {
    CFGUARDIAN_MODANIM_Idle = 0,
    CFGUARDIAN_MODANIM_1 = 1, // grunt? used when giving krystal the illusion spell?
    CFGUARDIAN_MODANIM_WalkSlow = 2,
    CFGUARDIAN_MODANIM_Run = 3,
    CFGUARDIAN_MODANIM_4 = 4, // charge (running with head down)
    CFGUARDIAN_MODANIM_5 = 5, // charge start?
    CFGUARDIAN_MODANIM_6 = 6, // charge end?
    CFGUARDIAN_MODANIM_7 = 7, // wobbly idle right
    CFGUARDIAN_MODANIM_8 = 8, // wobbly idle left
    CFGUARDIAN_MODANIM_Floating = 9,
    CFGUARDIAN_MODANIM_10 = 10, // look right
    CFGUARDIAN_MODANIM_11 = 11, // look right -> forward
    CFGUARDIAN_MODANIM_12 = 12, // head down, point
    CFGUARDIAN_MODANIM_13 = 13, // idle after giving something?
    CFGUARDIAN_MODANIM_14 = 14, // idle, head down a little to be sneaky (used in courtyard)
    CFGUARDIAN_MODANIM_SummoningStart = 15,
    CFGUARDIAN_MODANIM_SummoningLoop = 16,
    CFGUARDIAN_MODANIM_SummoningEnd = 17,
    CFGUARDIAN_MODANIM_Stretch = 18, // big stretch, right leg
    CFGUARDIAN_MODANIM_Jump = 19,
    CFGUARDIAN_MODANIM_DrinkingStart = 20,
    CFGUARDIAN_MODANIM_DrinkingLoop = 21,
    CFGUARDIAN_MODANIM_DrinkingStop = 22,
    CFGUARDIAN_MODANIM_23 = 23, // something with his hands
    CFGUARDIAN_MODANIM_24 = 24, // giving something from his invisible "bag"
    CFGUARDIAN_MODANIM_25 = 25, // grunt? other direction
    CFGUARDIAN_MODANIM_Walk = 26
};

enum CFGuardianSeq {
    CFGUARDIAN_SEQ_0 = 0,
    CFGUARDIAN_SEQ_GiveIllusionSpell = 1,
    CFGUARDIAN_SEQ_GivePowerRoomKey = 2,
    CFGUARDIAN_SEQ_3 = 3, // walk through prison? unused?
    CFGUARDIAN_SEQ_IllusionSpellHint = 4,
    CFGUARDIAN_SEQ_LetsGetOutOfHere = 5,
    CFGUARDIAN_SEQ_SummoningBoneDust = 6,
    CFGUARDIAN_SEQ_KyteLocationHint = 7,
    CFGUARDIAN_SEQ_WindShaftPowerHint = 8,
    CFGUARDIAN_SEQ_BabyCloudRunnerReminder = 9,
    CFGUARDIAN_SEQ_SpellStoneActivationReminder = 10,
    CFGUARDIAN_SEQ_SpellStoneRaceReminder = 11,
    CFGUARDIAN_SEQ_PowerCrystalsHint = 12
};

enum CFGuardianFlags {
    CFGUARDIAN_FLAG_1 = 0x1,
    CFGUARDIAN_FLAG_2 = 0x2,
    CFGUARDIAN_FLAG_WalkToTarget = 0x4,
};

enum CFGuardianTalkState {
    CFGUARDIAN_TALK_Disabled = 0,
    CFGUARDIAN_TALK_Enabled = 1,
    CFGUARDIAN_TALK_SpokenTo = 2
};

/*0x0*/ static u16 sModAnimSfx[] = {
    SOUND_8DD_BoneHead_Grunt1, 
    SOUND_8DE_BoneHead_Grunt2, 
    SOUND_8EA
};
/*0x8*/ static s16 sExprSfx[][2] = {
    {SOUND_8DD_BoneHead_Grunt1, 0x1000}, 
    {SOUND_8DE_BoneHead_Grunt2, 0x1000}, 
    {SOUND_8DF_BoneHead_Grunt3, 0x1000}, 
    {SOUND_8E0_BoneHead_Grunt4, 0x1000}, 
    {SOUND_8E1_BoneHead_Grunt5, 0x1000}
};
/*0x1C*/ static Vec3f sColliderTestPoints[] = {
    VEC3F(0.0f, 0.0f, 0.0f), 
    VEC3F(0.0f, 22.0f, 0.0f), 
    VEC3F(0.0f, 24.0f, 25.0f), 
    VEC3F(0.0f, 30.0f, -25.0f)
};
/*0x4C*/ static f32 sColliderTestRadii[] = {
    0.0f, 
    23.0f, 
    20.0f, 
    16.0f
};
/*0x5C*/ static CFGuardianMapStruct sTalkSeqStateMap[] = {
    {CFGUARDIAN_STATE_InCell,                           {CFGUARDIAN_SEQ_GiveIllusionSpell,            -1}}, 
    {CFGUARDIAN_STATE_WaitingToBeFreed,                 {CFGUARDIAN_SEQ_IllusionSpellHint,            -1}}, 
    {CFGUARDIAN_STATE_LeavingCell,                      {CFGUARDIAN_SEQ_LetsGetOutOfHere,             -1}}, 
    {CFGUARDIAN_STATE_WaitingAtWindLift,                {CFGUARDIAN_SEQ_GivePowerRoomKey,             -1}}, 
    {CFGUARDIAN_STATE_WaitingForWindLiftPower,          {CFGUARDIAN_SEQ_WindShaftPowerHint,           CFGUARDIAN_SEQ_PowerCrystalsHint}}, 
    {CFGUARDIAN_STATE_5,                                {CFGUARDIAN_SEQ_SummoningBoneDust,            -1}}, 
    {CFGUARDIAN_STATE_InWindShaft,                      {CFGUARDIAN_SEQ_LetsGetOutOfHere,             -1}}, 
    {CFGUARDIAN_STATE_LeavingWindShaft,                 {CFGUARDIAN_SEQ_KyteLocationHint,             CFGUARDIAN_SEQ_SummoningBoneDust}}, 
    {CFGUARDIAN_STATE_Courtyard_WaitingForKyte,         {CFGUARDIAN_SEQ_KyteLocationHint,             CFGUARDIAN_SEQ_SummoningBoneDust}}, 
    {CFGUARDIAN_STATE_Courtyard_WaitingForCloudBaby,    {CFGUARDIAN_SEQ_BabyCloudRunnerReminder,      CFGUARDIAN_SEQ_SummoningBoneDust}}, 
    {CFGUARDIAN_STATE_WalkingToRaceArea,                {CFGUARDIAN_SEQ_LetsGetOutOfHere,             -1}}, 
    {CFGUARDIAN_STATE_WaitingForRaceCompletion,         {CFGUARDIAN_SEQ_SpellStoneRaceReminder,       -1}}, 
    {CFGUARDIAN_STATE_WaitingForSpellStone,             {CFGUARDIAN_SEQ_SpellStoneActivationReminder, -1}}, 
    // ? data for a different variable?
    {0,  {8, 1}}, 
    {8,  {2, 8}}, 
    {3,  {10, 4}}, 
    {10, {5, 10}}, 
    {6,  {11, 7}}, 
    {11, {8, 12}}, 
    {9,  {12, 10}}, 
    {-1, {12, -1}}, 
    {13, {-1, 0}}
};
/*0x164*/ static u32 _data_unk164 = 0; // unused
/*0x168*/ static CFGuardianMapStruct sAnimTransitionMap[] = {
    {CFGUARDIAN_MODANIM_Idle,  {CFGUARDIAN_MODANIM_Stretch, -1}}, 
    {CFGUARDIAN_MODANIM_14,    {CFGUARDIAN_MODANIM_10,      CFGUARDIAN_MODANIM_12}}
};
/*0x180*/ static s32 sTalkSeqStateMapLength = 13;
/*0x184*/ static s32 sAnimTransitionMapLength = 2;
/*0x188*/ static s32 sStateAnimMap[] = {
/*0*/ -1, 
/*1*/ CFGUARDIAN_MODANIM_Idle, 
/*2*/ CFGUARDIAN_MODANIM_Walk, 
/*3*/ CFGUARDIAN_MODANIM_Idle, 
/*4*/ CFGUARDIAN_MODANIM_Idle, 
/*5*/ -1, 
/*6*/ -1, 
/*7*/ CFGUARDIAN_MODANIM_Walk, 
/*8*/ CFGUARDIAN_MODANIM_14, 
/*9*/ CFGUARDIAN_MODANIM_14, 
/*10*/ CFGUARDIAN_MODANIM_Walk, 
/*11*/ CFGUARDIAN_MODANIM_Walk, 
/*12*/ CFGUARDIAN_MODANIM_Idle, 
/*13*/ CFGUARDIAN_MODANIM_Idle, 
/*14*/ -1, 
/*15*/ CFGUARDIAN_MODANIM_10, // might not be a real state -> anim mapping
    // ? data for a different variable?
    CFGUARDIAN_MODANIM_11, 
    CFGUARDIAN_MODANIM_12, 
    CFGUARDIAN_MODANIM_13, 
    CFGUARDIAN_MODANIM_14, 
    CFGUARDIAN_MODANIM_5
};

#ifndef AVOID_UB
static int CFGuardian_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData/*, s8*/);
#else
static int CFGuardian_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8);
#endif
static s32 CFGuardian_control(Object* self);
static void CFGuardian_spawnBoneDust(Object* self, Object** dustObjs, Collectable_Setup** dustSetups);
static s32 CFGuardian_updateBoneDust(Object* self, Object** dustObjs, s16 rotX, s16 rotY, s16 rotZ, s32 count);
static SRT* CFGuardian_curveToWalkTarget(CurveSetup* curve, SRT* srt);
static CurveSetup* CFGuardian_findCurveNode(Object* self, s32 curveTag, Vec3f* pos, s32 arg3);
static s32 CFGuardian_walkTo(Object* self, SRT* target, f32 speed, f32* animChange);
static s32 CFGuardian_followCurvePath(Object* self, UnkCurvesStruct* arg1, f32 speed, u8 curveTag, f32* animChange);
static s32 CFGuardian_mapLookup(CFGuardianMapStruct* map, s32 key, s32 mapLength, s32 selector);
static s32 CFGuardian_doModAnimSfx(Object* self, UnkFunc_80024108Struct* animState, u16* sounds);
static void CFGuardian_func_25AC(Object* self, UnkCurvesStruct* arg1, s32 arg2, s32 arg3, f32 arg4);
static s32 CFGuardian_func_2638(Object* self, UnkCurvesStruct* curve, f32 arg2);

// offset: 0x0 | ctor
void CFGuardian_ctor(void* dll) { }

// offset: 0xC | dtor
void CFGuardian_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFGuardian_obj_Setup(Object* self, CFGuardian_Setup* setup, s32 reset) {
    CFGuardian_Data* objdata;
    s32 _pad;
    s32 _pad2;
    u8 sp48[] = {0x00, 0x01, 0x06, 0x06};
    s16 sp3C[] = {0x0005, 0x000f, 0x000f, 0x0000, 0x0000};
    u8 sp38[4] = {1, 1, 1, 1};

    objdata = self->data;
    if (objdata != NULL) {
        objInitMesgQueue(self, 4);
        objAddObjectType(self, OBJTYPE_WindLiftable);
        objdata->state = mainGetBits(BIT_CFGuardian_State);
        STUBBED_PRINTF(" Initalise Guardian State %i ", objdata->state);
        STUBBED_PRINTF(" GUARDIAN POS : %f %f %f \n", &setup->base.x, &setup->base.y, &setup->base.z);
        self->srt.transl.x = setup->base.x;
        self->srt.transl.y = setup->base.y;
        self->srt.transl.z = setup->base.z;
        self->unkDC = 1;
        self->animCallback = (AnimationCallback)CFGuardian_animCallback;
        self->srt.yaw = setup->yaw8 << 8;
        objdata->windLiftState = 0;
        objdata->unk908 = 6;
        objdata->flags = 0;
        objdata->animRate = 0.0f;
        gDLL_27->vtbl->init(&objdata->collider, 
            DLL27FLAG_2000000, 
            DLL27FLAG_40000 | DLL27FLAG_HAS_TERRAIN_COLLIDER | DLL27FLAG_80 | DLL27FLAG_2 | DLL27FLAG_1, 
            DLL27MODE_1);
        gDLL_27->vtbl->setup_terrain_collider(&objdata->collider, 4, sColliderTestPoints, sColliderTestRadii, sp38);
        gDLL_27->vtbl->setup_hits_collider(&objdata->collider, 4, sColliderTestPoints, sColliderTestRadii, 8);
        objdata->movedata.unk4A9 |= 0x28;
        objdata->talkState = CFGUARDIAN_TALK_Enabled;
        objdata->talkSeqSelector = 0;
        if (mainGetBits(BIT_57) != 0) {
            objdata->state = CFGUARDIAN_STATE_InWindShaft;
        }
        if (mainGetBits(BIT_4C1) != 0) {
            objdata->state = CFGUARDIAN_STATE_WaitingForRaceCompletion;
        }
        func_8002674C(self);
        mainCreateTempDLL(DLL_ID_MOVELIB);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objdata->movedata, -0x1FFF, 0x2800, 3);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func5(&objdata->movedata, 0x12C, 0x78);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func6(&objdata->movedata, 0, sp3C, 3);
    }
}

// offset: 0x2EC | func: 1 | export: 1
void CFGuardian_obj_Control(Object* self) {
    CFGuardian_control(self);
}

// offset: 0x328 | func: 2 | export: 2
void CFGuardian_obj_Update(Object* self) {
    self->prevLocalPosition.x = self->srt.transl.x;
    self->prevLocalPosition.y = self->srt.transl.y;
    self->prevLocalPosition.z = self->srt.transl.z;
}

// offset: 0x348 | func: 3 | export: 3
void CFGuardian_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CFGuardian_Data* objdata = self->data;

    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func3(self, &objdata->movedata, 0);
    }
}

// offset: 0x3CC | func: 4 | export: 4
void CFGuardian_obj_Free(Object* self, s32 onlySelf) {
    CFGuardian_Data* objdata = self->data;
    s32 i;

    if (objdata->unk500 != 0) {
        dll_amSfx->Stop(objdata->unk500);
        objdata->unk500 = 0;
    }
    if (onlySelf == 0) {
        for (i = 0; i < 6; i++) {
            if (objdata->dustObjs[i] != NULL) {
                objFreeObject(objdata->dustObjs[i]);
            }
        }
    }
    STUBBED_PRINTF(" FREEING GUARDIAN ");
    objFreeObjectType(self, OBJTYPE_WindLiftable);
    mainRemoveTempDLL(DLL_ID_MOVELIB);
}

// offset: 0x4AC | func: 5 | export: 5
u32 CFGuardian_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_EVENTS;
}

// offset: 0x4BC | func: 6 | export: 6
u32 CFGuardian_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFGuardian_Data);
}

typedef struct {
    s16 _unk0;
    s16 unk2;
    s32 unk4;
} Func4D0Struct;

// offset: 0x4D0 | func: 7
#ifndef AVOID_UB
static int CFGuardian_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData/*, s8 arg3*/) {
#else
static int CFGuardian_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
#endif
    CFGuardian_Data* objdata = actor->data;
    Func4D0Struct sp3C[] = {
        {0, 7, 8},
        {0, 7, 8}
    };
    Func4D0Struct* var_v1;

    if (actor->seqSlot < 0) {
        mapSaveObject(actor->setup, actor->mapID, actor->srt.transl.x, actor->srt.transl.y, actor->srt.transl.z);
        return 0;
    }
    if (objdata->state != CFGUARDIAN_STATE_InWindShaft) {
        var_v1 = &sp3C[0];
    } else {
        var_v1 = &sp3C[1];
    }
    diPrintf(" TURN NECK ");
    if (((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func4(actor, animObjData, &objdata->movedata, var_v1->unk2, var_v1->unk4) != 0) {
        return 1;
    }
    if (animObjData->lastMessage == 2) {
        CFGuardian_spawnBoneDust(actor, objdata->dustObjs, objdata->dustSetups);
        animObjData->unk9D |= 8;
    }
    if (animObjData->lastMessage == 3) {
        animObjData->unk9D |= 4;
    }
    CFGuardian_updateBoneDust(actor, objdata->dustObjs, 0x500, 0, 0, 6);
    return 0;
}

// offset: 0x678 | func: 8
static s32 CFGuardian_control(Object* self) {
    CFGuardian_Data* objdata;
    Object* player;
    Object* nearbyBaddie;
    f32 baddieDist;
    f32 trackHeight;
    s32 modAnimId;
    s32 seqno2;
    s32 seqno;
    u32 mesgID;
    void* mesgArg;
    s32 sp6C;
    UnkFunc_80024108Struct animState;
    Vec3f sp44;
    f32 var_fa0;

    mesgID = 0;
    mesgArg = NULL;
    sp6C = 1;
    baddieDist = 1000.0f;
    trackHeight = 1.0f;
    objdata = self->data;
    objdata->flags &= ~CFGUARDIAN_FLAG_2;
    diPrintf("Guardian ");
    objdata->animRate = 0.005f;
    player = objGetPlayer();
    switch (objdata->state) {
    case CFGUARDIAN_STATE_InCell:
        if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
            objdata->state = CFGUARDIAN_STATE_WaitingToBeFreed;
            objdata->talkState = CFGUARDIAN_TALK_Enabled;
        }
        sp6C = 0;
        break;
    case CFGUARDIAN_STATE_WaitingToBeFreed:
        if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
            objdata->talkState = CFGUARDIAN_TALK_Enabled;
        }
        if (mainGetBits(BIT_257) != 0) {
            objdata->state = CFGUARDIAN_STATE_LeavingCell;
            objAnimSet(self, CFGUARDIAN_MODANIM_Walk, 0, 0);
            self->unkDC = 0;
            mainSetBits(BIT_CRF_BoneHead_Guardian_Freed, 1);
        }
        sp6C = 0;
        break;
    case CFGUARDIAN_STATE_LeavingCell:
        if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
            objdata->talkState = CFGUARDIAN_TALK_Enabled;
        }
        objdata->flags |= CFGUARDIAN_FLAG_2;
        if (CFGuardian_followCurvePath(self, &objdata->unk53C, 0.7f, 0, &objdata->animRate) != 0) {
            objdata->state = CFGUARDIAN_STATE_WaitingAtWindLift;
        }
        break;
    case CFGUARDIAN_STATE_WaitingAtWindLift:
        if ((objdata->movedata.unk498 == 1) && (player == objdata->movedata.prevLookat) && (vec3Distance(&self->globalPosition, &player->globalPosition) < 80.0f)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(CFGUARDIAN_SEQ_GivePowerRoomKey, self, -1);
            mainSetBits(BIT_CRF_Power_Room_Key, 1);
            objdata->state = CFGUARDIAN_STATE_WaitingForWindLiftPower;
        }
        break;
    case CFGUARDIAN_STATE_WaitingForWindLiftPower:
        if (mainGetBits(BIT_57) != 0) {
            objdata->state = CFGUARDIAN_STATE_InWindShaft;
            objdata->talkSeqSelector = 0;
        } else if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
            objdata->talkState = CFGUARDIAN_TALK_Enabled;
            objdata->talkSeqSelector = (objdata->talkSeqSelector + 1) % 2;
        }
        break;
    case CFGUARDIAN_STATE_InWindShaft:
        diPrintf(" UpWind Lift ");
        if (objdata->windLiftState != 0) {
            if (objdata->windLiftState >= 2) {
                self->velocity.x = 0;
                self->velocity.z = 0;
                self->srt.transl.y += self->velocity.y * gUpdateRateF;
                trackGetHeightNearest(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &trackHeight, 0);
                self->srt.yaw = (0xC0 << (self->srt.yaw + 8)) >> 1;
                self->objhitInfo->unk58 &= ~0x400;
                if (trackHeight <= 1.0f) {
                    STUBBED_PRINTF(" LANDING ");
                    objdata->windLiftState = 2;
                    self->srt.transl.y -= trackHeight;
                    objdata->talkState = CFGUARDIAN_TALK_Disabled;
                    self->unkDC = 0;
                    objAnimSet(self, CFGUARDIAN_MODANIM_Idle, 0, 0);
                    CFGuardian_curveToWalkTarget(CFGuardian_findCurveNode(self, 0, NULL, 2), &objdata->walkTarget);
                    if (self->srt.transl.y <= objdata->walkTarget.transl.y) {
                        var_fa0 = objdata->walkTarget.transl.y - self->srt.transl.y;
                    } else {
                        var_fa0 = -(objdata->walkTarget.transl.y - self->srt.transl.y);
                    }
                    if (var_fa0 < 150.0f) {
                        objAddObjectType(self, OBJTYPE_WindLiftable);
                        objdata->state = CFGUARDIAN_STATE_LeavingWindShaft;
                        objAnimSet(self, CFGUARDIAN_MODANIM_Walk, 0, 0);
                    }
                }
                self->velocity.y -= 0.12f;
            } else {
                var_fa0 = ABS_EXPR(self->velocity.y * 400.0f);
                self->srt.yaw += var_fa0;
                objdata->animRate = 0.04f;
                if (mainGetBits(BIT_8E9) != 0) {
                    STUBBED_PRINTF("Guardian Out of WindLIft Boyo !!! ");
                    objAnimSet(self, CFGUARDIAN_MODANIM_Idle, 0, 0);
                    objAnim_func_80024D74(self, 0x32);
                    self->velocity.y = 0;
                    objFreeObjectType(self, OBJTYPE_WindLiftable);
                    self->velocity.x = 0;
                    self->velocity.y = -0.001f;
                    self->velocity.z = 0;
                    objdata->windLiftState = 2;
                    objdata->flags &= ~CFGUARDIAN_FLAG_1;
                }
            }
            if (objdata->windLiftState < 2) {
                self->srt.transl.x += gUpdateRateF * self->velocity.x;
                self->srt.transl.z += gUpdateRateF * self->velocity.z;
                gDLL_27->vtbl->func_1E8(self, &objdata->collider, gUpdateRateF);
                gDLL_27->vtbl->func_5A8(self, &objdata->collider);
                gDLL_27->vtbl->func_624(self, &objdata->collider, gUpdateRateF);
                if (objdata->collider.hitsTouchBits != 0) {
                    self->velocity.x = -self->velocity.x * 0.8f;
                    self->velocity.z = -self->velocity.z * 0.8f;
                }
                sp44.f[0] = self->srt.transl.x - self->prevLocalPosition.x;
                sp44.f[1] = self->srt.transl.y - self->prevLocalPosition.y;
                sp44.f[2] = self->srt.transl.z - self->prevLocalPosition.z;
                sp44.f[0] *= 0.95f * (1.0f / gUpdateRateF);
                sp44.f[1] *= 0.95f * (1.0f / gUpdateRateF);
                sp44.f[2] *= 0.95f * (1.0f / gUpdateRateF);
                self->velocity.x += sp44.f[0];
                self->velocity.y += sp44.f[1];
                self->velocity.z += sp44.f[2];
                self->velocity.x *= 0.3f;
                self->velocity.y *= 0.3f;
                self->velocity.z *= 0.3f;
                diPrintf(" Xvel %f Zvel %f \n", &self->velocity.x, &self->velocity.z);
            }
        } else if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
            objdata->talkState = CFGUARDIAN_TALK_Enabled;
        }
        break;
    case CFGUARDIAN_STATE_LeavingWindShaft:
        if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
            objdata->talkState = CFGUARDIAN_TALK_Enabled;
        }
        objdata->flags |= CFGUARDIAN_FLAG_2;
        if (CFGuardian_followCurvePath(self, &objdata->unk53C, 0.3f, 1, &objdata->animRate) != 0) {
            objdata->state = CFGUARDIAN_STATE_Courtyard_WaitingForKyte;
            objAnim_func_80024D74(self, 0x32);
        }
        break;
    case CFGUARDIAN_STATE_Courtyard_WaitingForKyte:
        nearbyBaddie = objGetNearestTypeTo(OBJTYPE_Baddie, self, &baddieDist);
        if ((nearbyBaddie != NULL) && (baddieDist < 300.0f)) {
            ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func1(&objdata->movedata, nearbyBaddie);
            self->unkAF |= ARROW_FLAG_10_Greyed_Out;
        }
        if ((baddieDist > 300.0f) && (vec3DistanceXZ(&player->globalPosition, &self->globalPosition) < 80.0f)) {
            self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
            if (!(objdata->flags & CFGUARDIAN_FLAG_WalkToTarget) && (sStateAnimMap[objdata->state] != CFGUARDIAN_MODANIM_Idle)) {
                STUBBED_PRINTF(" Stand ");
                ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func9(15, &objdata->walkTarget);
                objdata->flags |= (CFGUARDIAN_FLAG_WalkToTarget | CFGUARDIAN_FLAG_1);
                sStateAnimMap[objdata->state] = CFGUARDIAN_MODANIM_Idle;
            }
            if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
                objdata->talkState = CFGUARDIAN_TALK_Enabled;
                objdata->talkSeqSelector = (objdata->talkSeqSelector + 1) % 2;
            }
        } else {
            if (!(objdata->flags & CFGUARDIAN_FLAG_WalkToTarget) && (sStateAnimMap[objdata->state] != CFGUARDIAN_MODANIM_14)) {
                STUBBED_PRINTF(" Idle Tow ");
                objdata->talkState = CFGUARDIAN_TALK_SpokenTo;
                objdata->flags |= (CFGUARDIAN_FLAG_WalkToTarget | CFGUARDIAN_FLAG_1);
                ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func7(14, &objdata->walkTarget);
                sStateAnimMap[objdata->state] = CFGUARDIAN_MODANIM_14;
            }
        }
        if ((objdata->flags & CFGUARDIAN_FLAG_WalkToTarget) && (CFGuardian_walkTo(self, &objdata->walkTarget, 0.5f, &objdata->animRate) != 0)) {
            objAnimSet(self, CFGUARDIAN_MODANIM_Walk, 0, 0);
            objdata->flags &= ~(CFGUARDIAN_FLAG_WalkToTarget | CFGUARDIAN_FLAG_1);
        }
        if (mainGetBits(BIT_CF_Floor_Destroyed) != 0) {
            objdata->state = CFGUARDIAN_STATE_Courtyard_WaitingForCloudBaby;
            objdata->talkSeqSelector = 0;
        }
        break;
    case CFGUARDIAN_STATE_Courtyard_WaitingForCloudBaby:
        nearbyBaddie = objGetNearestTypeTo(OBJTYPE_Baddie, self, &baddieDist);
        if ((nearbyBaddie != NULL) && (baddieDist < 300.0f)) {
            ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func1(&objdata->movedata, nearbyBaddie);
            // @bug: if the target arrow was greyed out due to nearby baddies in the previous state,
            //       the arrow will remain greyed out even if the baddies move away as this state
            //       never clears the flag. this makes it impossible to initiate this state's talk seq
        }
        if ((baddieDist > 300.0f) && (vec3DistanceXZ(&player->globalPosition, &self->globalPosition) < 80.0f)) {
            if (!(objdata->flags & CFGUARDIAN_FLAG_WalkToTarget) && (sStateAnimMap[objdata->state] != CFGUARDIAN_MODANIM_Idle)) {
                STUBBED_PRINTF(" Stand ");
                ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func9(15, &objdata->walkTarget);
                objdata->flags |= (CFGUARDIAN_FLAG_WalkToTarget | CFGUARDIAN_FLAG_1);
                sStateAnimMap[objdata->state] = CFGUARDIAN_MODANIM_Idle;
            }
            if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
                objdata->talkState = CFGUARDIAN_TALK_Enabled;
                objdata->talkSeqSelector = (objdata->talkSeqSelector + 1) % 2;
            }
        } else {
            if (!(objdata->flags & CFGUARDIAN_FLAG_WalkToTarget) && (sStateAnimMap[objdata->state] != CFGUARDIAN_MODANIM_14)) {
                STUBBED_PRINTF(" Idle Tow ");
                objdata->talkState = CFGUARDIAN_TALK_SpokenTo;
                objdata->flags |= (CFGUARDIAN_FLAG_WalkToTarget | CFGUARDIAN_FLAG_1);
                ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func7(14, &objdata->walkTarget);
                sStateAnimMap[objdata->state] = CFGUARDIAN_MODANIM_14;
            }
        }
        if ((objdata->flags & CFGUARDIAN_FLAG_WalkToTarget) && (CFGuardian_walkTo(self, &objdata->walkTarget, 0.5f, &objdata->animRate) != 0)) {
            objAnimSet(self, CFGUARDIAN_MODANIM_Walk, 0, 0);
            objdata->flags &= ~(CFGUARDIAN_FLAG_WalkToTarget | CFGUARDIAN_FLAG_1);
        }
        if (mainGetBits(BIT_4BE) != 0) {
            objdata->state = CFGUARDIAN_STATE_WalkingToRaceArea;
            objAnimSet(self, CFGUARDIAN_MODANIM_Walk, 0, 0);
            self->unkDC = 0;
        }
        break;
    case CFGUARDIAN_STATE_WalkingToRaceArea:
        if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
            objdata->talkState = CFGUARDIAN_TALK_Enabled;
        }
        objdata->flags |= CFGUARDIAN_FLAG_2;
        if (CFGuardian_followCurvePath(self, &objdata->unk53C, 0.6f, 2, &objdata->animRate) != 0) {
            objdata->state = CFGUARDIAN_STATE_Vanish;
        }
        break;
    case CFGUARDIAN_STATE_Vanish:
        if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
            objdata->talkState = CFGUARDIAN_TALK_Enabled;
        }
        self->opacity = 0;
        self->objhitInfo->unk58 &= ~0x1;
        objDisable(self);
        self->srt.flags |= OBJSTATE_PRINT_DISABLED;
        objdata->state = CFGUARDIAN_STATE_NoOp_Vanished;
        break;
    case CFGUARDIAN_STATE_WaitingForRaceCompletion:
        if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
            objdata->talkState = CFGUARDIAN_TALK_Enabled;
        }
        if (mainGetBits(BIT_CRF_Race_Guardian_Reminder) != 0) {
            // player trying to leave!
            gDLL_2_Camera->vtbl->set_target_object(self);
            gDLL_3_Animation->vtbl->start_obj_sequence(CFGUARDIAN_SEQ_SpellStoneRaceReminder, self, -1);
            mainSetBits(BIT_CRF_Race_Guardian_Reminder, 0);
        }
        if (mainGetBits(BIT_Play_Seq_02A9_CF_Race_End) != 0) {
            objdata->state = CFGUARDIAN_STATE_WaitingForSpellStone;
        }
        break;
    case CFGUARDIAN_STATE_WaitingForSpellStone:
        if (objdata->talkState == CFGUARDIAN_TALK_SpokenTo) {
            objdata->talkState = CFGUARDIAN_TALK_Enabled;
        }
        if (mainGetBits(BIT_CRF_Race_Guardian_Reminder) != 0) {
            // player trying to leave!
            gDLL_2_Camera->vtbl->set_target_object(self);
            gDLL_3_Animation->vtbl->start_obj_sequence(CFGUARDIAN_SEQ_SpellStoneActivationReminder, self, -1);
            mainSetBits(BIT_CRF_Race_Guardian_Reminder, 0);
        }
        if (mainGetBits(BIT_4AA) != 0) {
            objdata->state = CFGUARDIAN_STATE_NoOp_SpellStoneActivated;
        }
        break;
    case CFGUARDIAN_STATE_NoOp_SpellStoneActivated:
        break;
    }
    STUBBED_PRINTF("GD"); // unknown location
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func0(self, &objdata->movedata);
    while (objRecvMesg(self, &mesgID, NULL, &mesgArg) != 0) {
        switch (mesgID) {
        case 15:
            // enter windlift
            STUBBED_PRINTF(" Guardian In Elevatoe "); // guessed location
            objdata->state = CFGUARDIAN_STATE_InWindShaft;
            objAnimSet(self, CFGUARDIAN_MODANIM_Floating, 0, 0);
            objAnim_func_80024D74(self, 0xFA);
            objdata->windLiftState = 1;
            self->velocity.z = 0.0f;
            self->velocity.y = 0.0f;
            self->velocity.x = 0.0f;
            self->objhitInfo->unk58 |= 0x400;
            objdata->talkState = CFGUARDIAN_TALK_Disabled;
            objdata->flags |= CFGUARDIAN_FLAG_1;
            break;
        case 16:
            // exit windlift
            STUBBED_PRINTF("Guardian Out of WindLIft "); // guessed location
            objAnimSet(self, CFGUARDIAN_MODANIM_Idle, 0, 0);
            objAnim_func_80024D74(self, 0x32);
            self->velocity.x = 0.0f;
            self->velocity.y = -0.001f;
            self->velocity.z = 0.0f;
            objdata->windLiftState = 2;
            objdata->flags &= ~CFGUARDIAN_FLAG_1;
            break;
        }
    }
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        joyDisableButtons(0, A_BUTTON);
        if (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_SpellStone_CRF) != 0) {
            mainSetBits(BIT_4AB, 1);
        } else if (objdata->talkState == CFGUARDIAN_TALK_Enabled) {
            seqno = CFGuardian_mapLookup(sTalkSeqStateMap, objdata->state, sTalkSeqStateMapLength, objdata->talkSeqSelector);
            if (seqno != -1) {
                objdata->talkState = CFGUARDIAN_TALK_SpokenTo;
                gDLL_3_Animation->vtbl->start_obj_sequence(seqno, self, -1);
            }
        }
    }
    if (mainGetBits(BIT_Force_CFGuardian_TalkSeq) != 0) {
        seqno2 = CFGuardian_mapLookup(sTalkSeqStateMap, objdata->state, sTalkSeqStateMapLength, objdata->talkSeqSelector);
        if (seqno2 != -1) {
            objdata->talkState = CFGUARDIAN_TALK_SpokenTo;
            gDLL_3_Animation->vtbl->start_obj_sequence(seqno2, self, -1);
            mainSetBits(BIT_Force_CFGuardian_TalkSeq, 0);
        }
    }
    if ((sStateAnimMap[objdata->state] != -1) && !(objdata->flags & CFGUARDIAN_FLAG_1) && (self->curModAnimId != sStateAnimMap[objdata->state])) {
        objAnimSet(self, sStateAnimMap[objdata->state], 0, 0);
        objAnim_func_80024D74(self, 0x50);
        STUBBED_PRINTF(" Set Anim ");
    }
    if (objAnimAdvance(self, objdata->animRate, (f32) gUpdateRate, &animState) != 0) {
        if (objdata->flags & CFGUARDIAN_FLAG_1) {
            if ((self->curModAnimId != CFGUARDIAN_MODANIM_Walk) && (self->curModAnimId != CFGUARDIAN_MODANIM_Floating)) {
                objdata->flags &= ~CFGUARDIAN_FLAG_1;
                STUBBED_PRINTF(" OVeride Set ");
            }
        } else if ((mathRnd(0, 6) == 0) && (sp6C != 0)) {
            modAnimId = CFGuardian_mapLookup(sAnimTransitionMap, self->curModAnimId, sAnimTransitionMapLength, mathRnd(0, 1));
            if (modAnimId != -1) {
                objAnim_func_80024D74(self, 0x28);
                objAnimSet(self, modAnimId, 0, 0);
                objdata->flags |= CFGUARDIAN_FLAG_1;
            } else {
                objAnimSet(self, sStateAnimMap[objdata->state], 0, 0);
            }
            STUBBED_PRINTF(" animnum %i "); // unknown location
        }
    }
    STUBBED_PRINTF(" Make Sound "); // guessed location
    CFGuardian_doModAnimSfx(self, &animState, sModAnimSfx);
    if (mathRnd(0, 60) == 0) {
        objExpr_func_80034B54(self, &objdata->exprHeadAnim, sExprSfx[mathRnd(0, 4)], 0);
    }
    objExpr_func_80034BC0(self, &objdata->exprHeadAnim);
    objExprEyeIdle(self, &objdata->eyeIdleHeadAnim);
    CFGuardian_updateBoneDust(self, objdata->dustObjs, 0x500, 0, 0, 6);
    if (mainGetBits(BIT_CFGuardian_State) != objdata->state) {
        mainSetBits(BIT_CFGuardian_State, objdata->state);
        STUBBED_PRINTF(" Set State %i ");
    }
    mapSaveObject(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
    return 0;
}

// offset: 0x1B8C | func: 9
static void CFGuardian_spawnBoneDust(Object* self, Object** dustObjs, Collectable_Setup** dustSetups) {
    Vec3f positions[] = {
        VEC3F(45.0f, 50.0f, 0.0f), 
        VEC3F(0.0f, 50.0f, 20.0f), 
        VEC3F(-20.0f, 50.0f, 0.0f), 
        VEC3F(0.0f, 50.0f, -20.0f), 
        VEC3F(18.0f, 50.0f, 18.0f), 
        VEC3F(-18.0f, 50.0f, -18.0f)
    };
    u8 colors[][3] = {
        {0xff, 0x00, 0x00}, 
        {0x00, 0xff, 0x00}, 
        {0x00, 0x00, 0xff}, 
        {0xff, 0xff, 0x00}, 
        {0x00, 0xff, 0xff}, 
        {0xff, 0xff, 0xff}
    };
    s32 i;

    for (i = 0; i < 6; i++) {
        dustSetups[i] = objAllocSetup(sizeof(Collectable_Setup), OBJ_BoneDust);
        dustSetups[i]->objHitsValue = 2;
        dustSetups[i]->gamebitCount = -1;
        dustSetups[i]->gamebitCollected = -1;
        dustSetups[i]->base.x = positions[i].x + self->srt.transl.x;
        dustSetups[i]->base.y = positions[i].y + self->srt.transl.y;
        dustSetups[i]->base.z = positions[i].z + self->srt.transl.z;
        dustSetups[i]->gamebitSecondary = -1;
        dustSetups[i]->base.loadFlags = OBJSETUP_LOAD_MANUAL;
        dustSetups[i]->base.fadeFlags = OBJSETUP_FADE_CAMERA;
        dustSetups[i]->base.loadDistance = 255;
        dustSetups[i]->base.fadeDistance = 255;
        dustSetups[i]->applyColourMultiplier = 1;
        dustSetups[i]->multiplyR = colors[i][0];
        dustSetups[i]->multiplyG = colors[i][1];
        dustSetups[i]->multiplyB = colors[i][2];
        dustObjs[i] = objSetupObject(&dustSetups[i]->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, NULL);
    }
}

// offset: 0x1D84 | func: 10
static s32 CFGuardian_updateBoneDust(Object* self, Object** dustObjs, s16 rotX, s16 rotY, s16 rotZ, s32 count) {
    s16 spB0[3];
    Vec3f spA4;
    s32 i;
    s32 sp9C = 0;
    f32 temp_fs0;
    f32 temp_fv0;
    SRT sp7C;
    u8 sp68[][3] = {
        {0xff, 0x00, 0x00}, 
        {0x00, 0xff, 0x00}, 
        {0x00, 0x00, 0xff}, 
        {0xff, 0xff, 0x00}, 
        {0x00, 0xff, 0xff}, 
        {0xff, 0xff, 0xff}
    };
    s32 _pad;
    s32 _pad2;

    objGetPlayer();
    temp_fs0 = self->srt.transl.y + 3.0f;
    for (i = 0; i < count; i++) {
        if (dustObjs[i] != NULL) {
            if (dustObjs[i]->unkDC != 0) {
                objFreeObject(dustObjs[i]);
                dustObjs[i] = NULL;
            } else {
                spA4.f[0] = dustObjs[i]->srt.transl.x - self->srt.transl.x;
                spA4.f[1] = dustObjs[i]->srt.transl.y - self->srt.transl.y;
                spA4.f[2] = dustObjs[i]->srt.transl.z - self->srt.transl.z;
                spB0[0] = rotX;
                spB0[1] = rotY;
                spB0[2] = rotZ;
                mathRotateRPY((SRT* ) &spB0, spA4.f);
                dustObjs[i]->srt.transl.x = spA4.f[0] + self->srt.transl.x;
                dustObjs[i]->srt.transl.y = spA4.f[1] + self->srt.transl.y;
                temp_fv0 = dustObjs[i]->srt.transl.y;
                if (temp_fv0 < temp_fs0) {
                    dustObjs[i]->srt.transl.y = (temp_fs0 - temp_fv0) + temp_fs0;
                }
                dustObjs[i]->srt.transl.z = spA4.f[2] + self->srt.transl.z;
                sp7C.roll = sp68[i][0];
                sp7C.pitch = sp68[i][1];
                sp7C.yaw = sp68[i][2];
                dll_partfx->spawn(dustObjs[i], PARTICLE_357, &sp7C, 0, -1, NULL);
                sp9C = 1;
            }
        }
    }

    return sp9C;
}

// offset: 0x1FF0 | func: 11
static SRT* CFGuardian_curveToWalkTarget(CurveSetup* curve, SRT* srt) {
    srt->transl.x = curve->pos.x;
    srt->transl.y = curve->pos.y;
    srt->transl.z = curve->pos.z;
    srt->yaw = curve->unk2C << 8;
    return srt;
}

// offset: 0x2020 | func: 12
static CurveSetup* CFGuardian_findCurveNode(Object* self, s32 curveTag, Vec3f* pos, s32 arg3) {
    s32 curveUID;
    s32 curveTypes[2];
    CurveSetup* curveNode;

    curveNode = NULL;
    if (arg3 == 1) {
        curveTypes[0] = 0;
        curveTypes[1] = 0;
    } else {
        curveTypes[0] = 0x19;
        curveTypes[1] = 0x15;
    }
    curveUID = gDLL_26_Curves->vtbl->func_1E4(
        self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, 
        curveTypes, ARRAYCOUNT(curveTypes), curveTag);
    if (curveUID >= 0) {
        curveNode = gDLL_26_Curves->vtbl->func_39C(curveUID);
        if (pos != NULL) {
            pos->x = curveNode->pos.x;
            pos->y = curveNode->pos.y;
            pos->z = curveNode->pos.z;
        }
    } else {
        STUBBED_PRINTF(" Error Could not find node ");
    }
    return curveNode;
}

// offset: 0x2104 | func: 13
static s32 CFGuardian_walkTo(Object* self, SRT* target, f32 speed, f32* animChange) {
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 dist;
    s16 angle;
    s32 _pad;

    if (target == NULL) {
        return 0;
    }
    dirX = target->transl.x - self->srt.transl.x;
    dirY = target->transl.y - self->srt.transl.y;
    dirZ = target->transl.z - self->srt.transl.z;
    dist = sqrtf(SQ(dirX) + SQ(dirY) + SQ(dirZ));
    if (dist < (speed * 5.0f)) {
        return 1;
    }
    guNormalize(&dirX, &dirY, &dirZ);
    self->velocity.x = dirX * speed * gUpdateRateF;
    self->velocity.y = dirY * speed * gUpdateRateF;
    self->velocity.z = dirZ * speed * gUpdateRateF;
    angle = (target->yaw - (self->srt.yaw & 0xFFFF)) + 0x8000;
    CIRCLE_WRAP(angle);
    self->srt.yaw += ((((f32) angle + 0.5f) * (speed * gUpdateRateF)) / dist);
    objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
    if (self->curModAnimId != CFGUARDIAN_MODANIM_Walk) {
        objAnimSet(self, CFGUARDIAN_MODANIM_Walk, 0.0f, 0);
    }
    objGetAnimChange(self, speed, animChange);
    return 0;
}

// offset: 0x2348 | func: 14
static s32 CFGuardian_followCurvePath(Object* self, UnkCurvesStruct* arg1, f32 speed, u8 curveTag, f32* animChange) {
    s32 _pad;
    s16 angle;
    s32 sp54;
    f32 height;
    SRT walkTarget;

    sp54 = 0;
    height = 0.0f;
    if (self->unkDC == -1) {
        return 1;
    }
    if (self->unkDC == 0) {
        CFGuardian_curveToWalkTarget(CFGuardian_findCurveNode(self, curveTag, NULL, 2), &walkTarget);
        if (CFGuardian_walkTo(self, &walkTarget, speed, animChange) != 0) {
            STUBBED_PRINTF("Got Curve ");
            CFGuardian_func_25AC(self, arg1, 2, curveTag, 200.0f);
            self->unkDC = 1;
        }
    } else {
        sp54 = CFGuardian_func_2638(self, arg1, speed);
        if (sp54 != 0) {
            self->unkDC = -1;
        }
        if (trackGetHeightNearest(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &height, 0) == 0) {
            self->srt.transl.y -= height;
        }
    }
    objGetAnimChange(self, speed, animChange);
    /* default.dol
    if (self->srt.yaw == -1) {
        STUBBED_PRINTF(" Object Yaw Reset ");
    }
    */
    if ((self->srt.transl.x != self->prevLocalPosition.x) || (self->srt.transl.z != self->prevLocalPosition.z)) {
        angle = (mathAtan2f(self->srt.transl.x - self->prevLocalPosition.x, self->srt.transl.z - self->prevLocalPosition.z) + 0x8000);
        // STUBBED_PRINTF(" Values MGetAngle %i Object Yaw  %i ", angle, self->srt.yaw); // default.dol
        angle -= (self->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(angle);
        // STUBBED_PRINTF(" Angle Diff %i ", angle); // default.dol
        self->srt.yaw += (angle >> 3);
        // STUBBED_PRINTF(" New Obj Yaw %i  \n", self->srt.yaw); // default.dol
    }
    if (self->curModAnimId != CFGUARDIAN_MODANIM_Walk) {
        objAnimSet(self, CFGUARDIAN_MODANIM_Walk, 0.0f, 0);
    }
    return sp54;
}


// offset: 0x25AC | func: 15
static void CFGuardian_func_25AC(Object* self, UnkCurvesStruct* arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 curveTypes[2];

    if (arg2 == 1) {
        curveTypes[0] = 0;
        curveTypes[1] = 0;
    } else {
        curveTypes[0] = 0x19;
        curveTypes[1] = 0x15;
    }
    gDLL_26_Curves->vtbl->func_4288(arg1, self, arg4, curveTypes, arg3);
    STUBBED_PRINTF(" PathId %i Loc id %i \n", arg3, arg1->unk9C->unk18);
}

static const char str_1[] = " Tangent 1 %f %f %f \n";
static const char str_2[] = " Tangent 2 %f %f %f \n";
static const char str_3[] = "t value %f ";

// offset: 0x2638 | func: 16
static s32 CFGuardian_func_2638(Object* self, UnkCurvesStruct* curve, f32 arg2) {
    s32 sp24;

    sp24 = 0;
    if ((curves_func_800053B0(&curve->unk0, arg2) != 0) || (curve->unk0.unk10 != 0)) {
        sp24 = gDLL_26_Curves->vtbl->func_4704(curve);
    }
    self->srt.transl.x = curve->unk0.unk68.x;
    self->srt.transl.y = curve->unk0.unk68.y;
    self->srt.transl.z = curve->unk0.unk68.z;
    return sp24;
}

// offset: 0x26F0 | func: 17
void CFGuardian_func_26F0(void) {

}

// offset: 0x26F8 | func: 18
void CFGuardian_func_26F8(void) {

}

// offset: 0x2700 | func: 19
static s32 CFGuardian_mapLookup(CFGuardianMapStruct* map, s32 key, s32 mapLength, s32 selector) {
    s32 i;

    i = 0;
    while ((i < mapLength) && (key != map[i].key)) {
        i += 1;
    }
    if (i != mapLength) {
        return map[i].values[selector];
    }
    return -1;
}

// offset: 0x2770 | func: 20
void CFGuardian_func_2770(void) {

}

// offset: 0x2778 | func: 21 | export: 7
s32 CFGuardian_Func_2778(Object* self) {
    CFGuardian_Data* objdata = self->data;
    return (objdata->flags & CFGUARDIAN_FLAG_2) == FALSE;
}

// offset: 0x2790 | func: 22
static s32 CFGuardian_doModAnimSfx(Object* self, UnkFunc_80024108Struct* animState, u16* sounds) {
    s32 ret;
    s32 i;

    ret = 0;
    for (i = 0; i < animState->unk1B; i++) {
        switch (animState->unk13[i]) {
        case 0:
            if (sounds != NULL) {
                dll_amSfx->Play(self, sounds[0], MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            continue;
        case 7:
            if (sounds != NULL) {
                dll_amSfx->Play(self, sounds[1], MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            continue;
        case 1:
            ret = 1;
            continue;
        case 2:
            ret = 2;
            continue;
        case 3:
            ret = 3;
            continue;
        case 4:
            ret = 4;
            continue;
        case 9:
            dll_amSfx->Play(self, SOUND_8EB, MAX_VOLUME, NULL, NULL, 0, NULL);
            continue;
        }
    }
    if ((ret != 0) && (sounds != NULL)) {
        dll_amSfx->Play(self, sounds[2], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    return ret;
}
