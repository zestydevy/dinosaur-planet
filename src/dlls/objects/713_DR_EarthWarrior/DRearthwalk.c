
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/53_movelib.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/713_DRearthwalk.h"
#include "game/gamebits.h"
#include "game/gametexts.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/objprint.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "sys/lfx.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "dll.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 rotation;
/*19*/ s8 unk19;
} DRearthwalk_Setup;

typedef struct {
/*000*/ ObjFSA_Data fsa;
/*34C*/ u8 _unk34C[0x370 - 0x34C];
/*370*/ HeadAnimation unk370;
/*394*/ HeadAnimation unk394;
/*3B8*/ MoveLibData movedata;
/*870*/ UnkCurvesStruct unk870;
/*978*/ Vec3f unk978[4];
/*9A8*/ u8 _unk9A8[0x9B4 - 0x9A8];
/*9B4*/ f32 unk9B4;
/*9B8*/ f32 unk9B8;
/*9BC*/ f32 unk9BC;
/*9C0*/ u8 _unk9C0[0xA52 - 0x9C0];
/*A52*/ s16 unkA52;
/*A54*/ u8 _unkA54[0xA56 - 0xA54];
/*A56*/ s16 energy;
/*A58*/ u16 unkA58;
/*A5A*/ u8 unkA5A;
/*A5B*/ u8 _unkA5B;
/*A5C*/ u8 unkA5C;
/*A5D*/ u8 _unkA5D;
/*A5E*/ u8 unkA5E;
/*A5F*/ u8 unkA5F;
/*A60_0*/ u8 unkA60_0 : 1;
/*A60_1*/ u8 unkA60_1 : 1;
/*A60_2*/ u8 unkA60_2 : 1;
/*A60_3*/ u8 unkA60_3 : 1;
/*A60_4*/ u8 rideable : 1;
/*A60_5*/ u8 unkA60_5 : 1;
/*A61*/ s8 unkA61;
/*A62*/ s8 talkSeq;
/*A63*/ s8 unkA63;
} DRearthwalk_Data;

enum DREWSeq {
    EWSEQ_0_EatBlueMushroom_Laying = 0,
    EWSEQ_1_EatBlueMushroom_Standing = 1,
    EWSEQ_2_LetsGoStopTheDragon = 2,
    EWSEQ_3_INeedEnergy = 3,
    EWSEQ_4 = 4,
    EWSEQ_5_SabreHopOn_Right = 5,
    EWSEQ_6_SabreHopOn_Left = 6,
    EWSEQ_7_SabreHopOff_Left = 7,
    EWSEQ_8_SabreHopOff_Right = 8,
    EWSEQ_9_IfICouldGetOutOfTheseChains = 9,
    EWSEQ_10_TeethChattering = 10
};

/*0x0*/ static u16 _data_0[] = {0x0854, 0x081a};
/*0x4*/ static u16 _data_4[] = {0x0853, 0x0852};
/*0x8*/ static s16 _data_8[][2] = {
    {0x0817, 0x0800}, 
    {0x0818, 0x0800}, 
    {0x0819, 0x1000}, 
    {0x081a, 0x1000}
};
/*0x18*/ static s16 _data_18[][2] = {
    {0x07bf, 0x1000}, 
    {0x0816, 0x1000}, 
    {0x081c, 0x1000}
};
/*0x24*/ static s16 _data_24[][2] = {
    {SOUND_8ED, 0x1000}
};
/*0x24*/ static s16 _data_28[] = {
    SOUND_855, 
    SOUND_856, 
    SOUND_857, 
    SOUND_858
};
/*0x30*/ static Vec3f _data_30[] = {
    VEC3F(-9.0f, 0.0f, -12.0f), 
    VEC3F(9.0f, 0.0f, -12.0f), 
    VEC3F(9.0f, 0.0f, 12.0f), 
    VEC3F(-9.0f, 0.0f, 12.0f)
};
/*0x60*/ static f32 _data_60[] = {
    0.0f, 0.0f, 0.0f, 0.0f
};
/*0x70*/ static Vec3f _data_70[] = {
    VEC3F(0.0f, 0.0f, 35.0f), 
    VEC3F(0.0f, 0.0f, -35.0f)
};
/*0x88*/ static f32 _data_88[] = {
    25.0f, 25.0f
};
/*0x90*/ static u32 _data_90 = 0x037b037b;
/*0x94*/ static u32 _data_94[] = {
    0x00000008, 0x00000007, 0x00000009, 0x0000000a, 0x01c80000
};
/*0xA8*/ static s16 _data_A8[] = {0x0018, 0x0000};
/*0xAC*/ static f32 _data_AC[] = {
    0.0031f, /*0x00030003*/2.7551e-40f // hmm
};
/*0xB4*/ static f32 _data_B4[][2] = {
    {0.0f, 0.03f}, 
    {0.05f, 0.7f}, 
    {0.6f, 1.2f}, 
    {0.8f, 1.93f}, 
    {1.72f, 2.7f}
};
/*0xDC*/ static s16 _data_DC[] = {
    0x0002, 0x0003, 0x0017, 0x0016, 0x0004, 0x0000
};

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[8];
/*0x20*/ static ObjFSA_StateCallback sLogicStateCallbacks[1];
/*0x28*/ static MtxF _bss_28;

static void DRearthwalk_func_ABC(Object* self);
static void DRearthwalk_func_B54(Object* self, s32 arg1, s32 arg2);
static void DRearthwalk_func_1684(Object* self, DRearthwalk_Data* objdata, ObjFSA_Data* fsa);
static int DRearthwalk_func_1EBC(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3);
static void DRearthwalk_func_3210(Object* self, s16 arg1, s16 arg2);
static s32 DRearthwalk_func_32EC(Object* self, u8 arg1);

/*static*/ s32 DRearthwalk_func_21A8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 DRearthwalk_func_21DC(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 DRearthwalk_func_2454(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 DRearthwalk_func_2704(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 DRearthwalk_func_2860(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 DRearthwalk_func_2D80(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 DRearthwalk_func_2F90(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 DRearthwalk_func_311C(Object* self, ObjFSA_Data* fsa, f32 updateRate);

/*static*/ s32 DRearthwalk_func_31F8(Object *self, ObjFSA_Data *fsa, f32 updateRate);

// offset: 0x0 | func: 0
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/DRearthwalk_func_0.s")
#else
void DRearthwalk_func_0(void) {
    sAnimStateCallbacks[0] = DRearthwalk_func_21A8;
    sAnimStateCallbacks[1] = DRearthwalk_func_21DC;
    sAnimStateCallbacks[2] = DRearthwalk_func_2454;
    sAnimStateCallbacks[3] = DRearthwalk_func_2704;
    sAnimStateCallbacks[4] = DRearthwalk_func_2860;
    sAnimStateCallbacks[5] = DRearthwalk_func_2D80;
    sAnimStateCallbacks[6] = DRearthwalk_func_2F90;
    sAnimStateCallbacks[7] = DRearthwalk_func_311C;

    sLogicStateCallbacks[0] = DRearthwalk_func_31F8;
}
#endif

// offset: 0x8C | ctor
#ifndef NON_MATCHING
void DRearthwalk_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/DRearthwalk_ctor.s")
#else
void DRearthwalk_ctor(void* dll) {
    DRearthwalk_func_0();
}
#endif

// offset: 0xCC | dtor
void DRearthwalk_dtor(void *dll) { }

// offset: 0xD8 | func: 1 | export: 0
void DRearthwalk_setup(Object* self, DRearthwalk_Setup* setup, s32 arg2) {
    DRearthwalk_Data* objdata;
    s32 _pad[2];
    u8 sp50[] = {1, 1, 1, 1};
    s16 sp44[] = {0x000a, 0x000a, 0x0000, 0x0000, 0x0000};
    s16 sp38[] = {0x0014, 0x0014, 0x0000, 0x0000, 0x0000};

    self->srt.yaw = setup->rotation << 8;
    self->animCallback = DRearthwalk_func_1EBC;
    objAddObjectType(self, OBJTYPE_Vehicle);
    objdata = self->data;
    objdata->unkA5C = setup->unk19;
    objdata->unkA52 = 5;
    objdata->talkSeq = -1;
    if (self->shadow != NULL) {
        self->shadow->flags |= (OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR);
        self->shadow->maxDistScale = self->shadow->scale * 0.4f;
    }
    gDLL_18_objfsa->vtbl->func0(self, &objdata->fsa, 8, 1);
    objdata->fsa.unk29C = 0.17f;
    objdata->fsa.unk4.mode = DLL27MODE_DISABLED;
    gDLL_27->vtbl->init(&objdata->fsa.unk4, DLL27FLAG_4000000 | DLL27FLAG_2000000 | DLL27FLAG_4, DLL27FLAG_NONE, DLL27MODE_DISABLED);
    gDLL_27->vtbl->setup_hits_collider(&objdata->fsa.unk4, 2, _data_70, _data_88, 8);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->fsa.unk4, 4, _data_30, _data_60, sp50);
    gDLL_27->vtbl->reset(self, &objdata->fsa.unk4);
    func_8002674C(self);
    mainCreateTempDLL(53);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objdata->movedata, -4551, 5461, 2);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func5(&objdata->movedata, 300, 120);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func6(&objdata->movedata, sp38, sp44, 2);
    if (setup->unk19 == 1) {
        objdata->energy = 1;
        objdata->unkA58 |= 0x100;
        objdata->unkA60_1 = 1;
    } else {
        objdata->energy = 5;
        objdata->rideable = 1;
    }
    objdata->unkA61 = 0x1E;
}

// offset: 0x408 | func: 2 | export: 1
void DRearthwalk_control(Object* self) {
    DRearthwalk_Data* objdata;
    ObjFSA_Data* fsa;
    s32 sp4C;
    Object* player;
    s32 var_a2;
    s32 sp40;
    s16 plAngle;

    objdata = self->data;
    fsa = self->data;
    sp4C = 1;
    player = objGetPlayer();
    objdata->unkA52 = 5;
    diPrintf(" EARTHWALK ENERGY : %i ", objdata->energy);
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    if (objdata->unkA5A == 2) {
        // riding?
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        fsa->unk4.mode = DLL27MODE_1;
        self->objhitInfo->unk5B = 0xF4;
        self->objhitInfo->unk5C = 0xF4;
        var_a2 = 0;
        while (var_a2 < gUpdateRate) {
            DRearthwalk_func_B54(self, 1, var_a2);
            var_a2 += 1;
        }
    } else {
        if (!(objdata->unkA58 & 0x20) && (mainGetBits(BIT_DR_SavedEarthWarriorFromSkeetlas) == 0)) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            objGetAllOfType(OBJTYPE_Baddie, &sp40);
            diPrintf(" num %i ", sp40);
            if ((sp40 == 0) && (mainGetBits(BIT_DR_Lava_Pools_Cooled_Count) == 3)) {
                mainSetBits(BIT_DR_SavedEarthWarriorFromSkeetlas, 1);
                objdata->talkSeq = EWSEQ_9_IfICouldGetOutOfTheseChains;
            }
        }
        fsa->unk4.mode = DLL27MODE_1;
        gDLL_27->vtbl->reset(self, &fsa->unk4);
        self->objhitInfo->unk5B = 0;
        self->objhitInfo->unk5C = 0;
        DRearthwalk_func_B54(self, gUpdateRate, -1);
    }
    func_80028D2C(self);
    objExprEyeIdle(self, &objdata->unk370);
    objExpr_func_80034BC0(self, &objdata->unk394);
    if (!(objdata->unkA58 & 0x20) && (mainGetBits(BIT_5FE) != 0) && (objdata->energy > 0)) {
        mainSetBits(BIT_654, 1);
        objdata->talkSeq = EWSEQ_2_LetsGoStopTheDragon;
        objdata->unkA60_1 = 0;
    }
    if (!(objdata->unkA58 & 0x20) && (mainGetBits(BIT_655) != 0)) {
        objdata->unkA58 |= 0x40;
        objdata->talkSeq = EWSEQ_2_LetsGoStopTheDragon;
        objdata->unkA58 |= 0x20;
        objdata->movedata.unk4A9 |= 2;
        gDLL_26_Curves->vtbl->func_4288(&objdata->unk870, self, 1000.0f, &sp4C, -1);
    }
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func0(self, &objdata->movedata);
    objdata->unkA60_3 = 0;
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        objdata->unkA60_3 = 1;
        if ((objdata->unkA60_3) && (objdata->rideable)) {
            joyDisableButtons(0, A_BUTTON);
            plAngle = self->srt.yaw - (player->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(plAngle);
            if (plAngle > 0) {
                // sabre gets on, from right side
                objdata->unkA5F = 0;
                gDLL_3_Animation->vtbl->start_obj_sequence(EWSEQ_5_SabreHopOn_Right, self, -1);
            } else {
                // sabre gets on, from left side
                objdata->unkA5F = 1;
                gDLL_3_Animation->vtbl->start_obj_sequence(EWSEQ_6_SabreHopOn_Left, self, -1);
            }
            gDLL_5_AMSEQ2->vtbl->set(self, 0xF9, 0, 0, 0);
            objdata->unkA58 |= 4;
            objdata->movedata.unk4A9 |= 1;
        }
        if (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_Inventory_Blue_Mushrooms) != 0) {
            if (fsa->animState == 2) {
                gDLL_3_Animation->vtbl->start_obj_sequence(EWSEQ_0_EatBlueMushroom_Laying, self, -1);
                joyDisableButtons(0, A_BUTTON);
            } else {
                gDLL_3_Animation->vtbl->start_obj_sequence(EWSEQ_1_EatBlueMushroom_Standing, self, -1);
                joyDisableButtons(0, A_BUTTON);
            }
            objdata->energy += 12;
            mainSetBits(BIT_Inventory_Blue_Mushrooms, mainGetBits(BIT_Inventory_Blue_Mushrooms) - 1);
        } else if ((objdata->talkSeq != -1) && (gDLL_1_cmdmenu->vtbl->was_any_item_used() == 0)) {
            if (!objdata->rideable) {
                gDLL_3_Animation->vtbl->start_obj_sequence(objdata->talkSeq, self, -1);
                joyDisableButtons(0, A_BUTTON);
            } else {
                objdata->unkA60_3 = 1;
            }
        }
    }
    if ((objdata->unkA61 == 0) && (mainGetBits(BIT_7B6) == 0)) {
        DRearthwalk_func_ABC(self);
    }
    if (objdata->unkA61 != 0) {
        objdata->unkA61 -= gUpdateRate;
        if (objdata->unkA61 < 0) {
            objdata->unkA61 = 0;
        }
    }
}

// offset: 0xABC | func: 3
static void DRearthwalk_func_ABC(Object* self) {
    if (vec3DistanceXZ(&objGetPlayer()->globalPosition, &self->globalPosition) > 600.0f) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_DRAGON_ROCK_BOTTOM, 2, 0);
    }
}

// offset: 0xB54 | func: 4
static void DRearthwalk_func_B54(Object* self, s32 fsaUpdateRate, s32 arg2) {
    DRearthwalk_Data* objdata;
    s32 _pad3;
    Camera* sp54;
    s32 sp50;
    s32 sp4C;
    s32 _pad;
    f32 temp_fa1;
    f32 temp_fv0;
    UnkCurvesStruct* sp28;
    s16 sp3A;
    f32 dist;
    f32 sp30;
    Object* temp_v1;

    sp4C = 1;
    sp30 = 12.0f;
    temp_v1 = objGetPlayer();
    if (arg2 != -1) {
        sp50 = (arg2 + 1) == gUpdateRate;
    } else {
        sp50 = 1;
    }
    sp54 = camGetMain();
    objdata = self->data;
    objdata->fsa.hitpoints = 0;
    objdata->fsa.flags &= ~OBJFSA_FLAG_8000;
    if (objdata->unkA5A == 2) {
        objdata->fsa.xAnalogInput = (f32) joyGetStickXBuffered(0, arg2);
        objdata->fsa.yAnalogInput = (f32) joyGetStickYBuffered(0, arg2);
        objdata->fsa.unk310 = joyGetPressedBuffered(0, arg2);
        objdata->fsa.unk30C = joyGetButtonsBuffered(0, arg2);
        objdata->fsa.unk324 = sp54->srt.yaw;
    } else if (objdata->unkA58 & 0x40) {
        objdata->movedata.unk4A9 |= 2;
        objdata->rideable = 0;
        sp28 = &objdata->unk870;
        temp_fv0 = sp28->unk0.unk68.x - self->srt.transl.x;
        temp_fa1 = sp28->unk0.unk68.z - self->srt.transl.z;
        sqrtf(SQ(temp_fv0) + SQ(temp_fa1));
        if ((objdata->unkA58 & 0x80) && ((gDLL_11_Newlfx->vtbl->func4.withTwoArgsF32Custom(&self->srt.transl, 128.0f) < 0.0f) || (temp_v1->stateFlags & 0x1000))) {
            sp4C = 0;
        }
        temp_fv0 = sp28->unk0.unk68.x - temp_v1->srt.transl.x;
        temp_fa1 = sp28->unk0.unk68.z - temp_v1->srt.transl.z;
        temp_fa1 = SQ(temp_fv0) + SQ(temp_fa1);
        if (sqrtf(temp_fa1) < 22.0f) {
            sp4C = 0;
        }
        temp_fv0 = sp28->unk0.unk68.x - self->srt.transl.x;
        temp_fa1 = sp28->unk0.unk68.z - self->srt.transl.z;
        temp_fa1 = SQ(temp_fv0) + SQ(temp_fa1);
        dist = sqrtf(temp_fa1);
        if ((dist < 50.0f) && (sp4C != 0)) {
            if ((curves_func_800053B0(&sp28->unk0, 0.9f) != 0) || (sp28->unk0.unk10 != 0)) {
                if (DRearthwalk_func_32EC(self, (u8) sp28->unkA0->unk18) != 0) {
                    if (gDLL_26_Curves->vtbl->func_4704(sp28) != 0) {
                        objdata->unkA58 &= ~0x140;
                        objdata->movedata.unk4A9 &= ~0x2;
                        gDLL_18_objfsa->vtbl->set_anim_state(self, &objdata->fsa, 7);
                    }
                } else {
                    sp30 = 0.0f;
                }
            }
        } else if (dist < 50.0f) {
            sp30 = 0.0f;
        } else {
            sp30 = 11.0f;
        }
        sp3A = mathAtan2f(self->srt.transl.x - sp28->unk0.unk68.x, self->srt.transl.z - sp28->unk0.unk68.z) & 0xFFFF;
        objdata->fsa.xAnalogInput = -mathSinfInterp(-sp3A) * sp30;
        objdata->fsa.yAnalogInput = -mathCosfInterp(-sp3A) * sp30;
        objdata->fsa.unk324 = 0;
        objdata->fsa.unk310 = 0;
        objdata->fsa.unk30C = 0;
    } else {
        objdata->fsa.unk310 = 0;
        objdata->fsa.unk30C = 0;
        objdata->fsa.unk324 = 0;
        objdata->fsa.xAnalogInput = 0.0f;
        objdata->fsa.yAnalogInput = 0.0f;
    }
    objdata->fsa.flags |= OBJFSA_FLAG_400000;
    if (sp50 != 0) {
        objdata->fsa.flags &= ~OBJFSA_FLAG_400000;
    }
    gDLL_18_objfsa->vtbl->tick(self, &objdata->fsa, (f32) fsaUpdateRate, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    DRearthwalk_func_1684(self, objdata, &objdata->fsa);
}

// offset: 0x107C | func: 5 | export: 2
void DRearthwalk_update(Object* self) {
    DRearthwalk_Data* objdata;
    ObjFSA_Data* fsa;
    Object* hitBy;
    DLL27_Data* temp_v1;
    s16 angle;
    s32 dmgType;

    objdata = self->data;
    fsa = self->data;
    if (self->stateFlags & OBJSTATE_IN_SEQ) {
        return;
    }
    
    if (fsa->animState != 6) {
        dmgType = func_80025F40(self, &hitBy, NULL, NULL);
        if (dmgType != 0) {
            if ((dmgType == Damage_Type_Flame_Command) || (objGetPlayer() == hitBy) || (hitBy->id == OBJ_sword)) {
                return;
            }
            objExpr_func_80034B54(self, &objdata->unk394, _data_18[mathRnd(0, 2)], 1);
            angle = self->srt.yaw - (hitBy->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(angle);
            if ((angle > 0x4000) || (angle < -0x4000)) {
                objdata->unkA60_0 = 0;
            } else {
                objdata->unkA60_0 = 1;
            }
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 6);
        }
    }

    if (fsa->unk4.mode != 0) {
        temp_v1 = &fsa->unk4;
        if (temp_v1->hitsTouchBits != 0) {
            fsa->speed *= 0.5f;
            fsa->unk2B0 *= 1.5f;
        } else {
            if (fsa->unk2B0 != 18.0f) {
                fsa->unk2B0 *= 0.8f;
                if (fsa->unk2B0 <= 18.0f) {
                    fsa->unk2B0 = 18.0f;
                }
            }
        }
        if (temp_v1->unk140.unk0 != NULL) {
            hitBy = temp_v1->unk140.unk0;
            if ((hitBy->id == OBJ_DR_PushCart) && (objdata->unkA60_5) && (self->curModAnimId == 0x14)) {
                dll_amSfx->Play(self, _data_28[1], MAX_VOLUME, NULL, NULL, 0, NULL);
                ((DLL_Unknown*)hitBy->dll)->vtbl->func[7].withThreeArgsCustom(hitBy, self, 190.f);
                objdata->unkA60_5 = 0;
            }
            temp_v1->unk140.unk0 = NULL;
        }
    }
}

// offset: 0x138C | func: 6 | export: 3
void DRearthwalk_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DRearthwalk_Data* objdata = self->data;
    s32 numObjs;
    Object** objs;
    s32 i;

    if (visibility == -1) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        objGetAttachPointWorldSpace(self, 2, &objdata->unk9B4, &objdata->unk9B8, &objdata->unk9BC, 0);
        objGetAttachPointBoneWorldPositions(self, 3, 4, objdata->unk978);
    }
    if ((objdata->unkA5A != 2) && (visibility != 0)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        objGetAttachPointWorldSpace(self, 2, &objdata->unk9B4, &objdata->unk9B8, &objdata->unk9BC, 0);
        objGetAttachPointBoneWorldPositions(self, 3, 4, objdata->unk978);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func3(self, &objdata->movedata, 0);
        if (objdata->unkA60_1) {
            objs = objGetAllOfType(OBJTYPE_DR_Shackle, &numObjs);
            for (i = 0; i < numObjs; i++) {
                ((DLL_Unknown*)objs[i]->dll)->vtbl->func[7].objtype57Func7(objs[i], self, 
                    _data_94[((DLL_Unknown*)objs[i]->dll)->vtbl->func[8].withOneVoidArgS32(objs[i])], 
                    gdl, mtxs, vtxs, pols);
            }
        }
    }
}

// offset: 0x160C | func: 7 | export: 4
void DRearthwalk_free(Object* self, s32 a1) {
    objFreeObjectType(self, OBJTYPE_Vehicle);
    mainRemoveTempDLL(53);
}

// offset: 0x1660 | func: 8 | export: 5
u32 DRearthwalk_get_model_flags(Object *self) {
    return MODFLAGS_EVENTS | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x1670 | func: 9 | export: 6
u32 DRearthwalk_get_data_size(Object *self, u32 a1) {
    return sizeof(DRearthwalk_Data);
}

// offset: 0x1684 | func: 10
static void DRearthwalk_func_1684(Object* self, DRearthwalk_Data* objdata, ObjFSA_Data* fsa) {
    u8 var_s7;
    u8 var_s0_2;
    u8 var_s5;
    u8 var_s0;
    SRT sp7C;
    f32 var_fv0;

    var_s7 = 0;
    var_s0 = 0;
    if (fsa->unk308 & 2) {
        var_s0 = 1;
    }
    if (fsa->unk308 & 4) {
        var_s0 |= 2;
    }
    if (fsa->unk308 & 8) {
        var_s0 |= 4;
    }
    if (fsa->unk308 & 0x10) {
        var_s0 |= 8;
    }
    if (fsa->unk308 & 0x81) {
        if (fsa->unk308 & 0x80) {
            var_s7 = 1;
        }
        dll_amSfx->Play(self, _data_0[var_s7], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    var_s5 = 0;
    while (var_s0) {
        if (var_s0 & 1) {
            if (fsa->speed > 1.0f) {
                var_s7 = 1;
            }
            dll_amSfx->Play(self, _data_4[var_s7], MAX_VOLUME, NULL, NULL, 0, NULL);
            sp7C.transl.x = objdata->unk978[var_s5].x;
            sp7C.transl.y = objdata->unk978[var_s5].y;
            sp7C.transl.z = objdata->unk978[var_s5].z;
            sp7C.scale = fsa->speed * 0.0005f;
            if (sp7C.scale < 0.0004f) {
                sp7C.scale = 0.0004f;
            } else {
                if (sp7C.scale > 0.004f) {
                    var_fv0 = 0.004f;
                } else {
                    var_fv0 = sp7C.scale;
                }
                sp7C.scale = var_fv0;
            }
            var_s0_2 = mathRnd(2, 6);
            while (var_s0_2 != 0) {
                gDLL_17_partfx->vtbl->spawn(self, mathRnd(0, 0) + PARTICLE_676, &sp7C, PARTFXFLAG_200000 | PARTFXFLAG_10000 | PARTFXFLAG_1, -1, NULL);
                var_s0_2--;
            }
        }
        var_s0 >>= 1;
        var_s5++;
    }
}

// offset: 0x197C | func: 11 | export: 7
s32 DRearthwalk_func_197C(Object* self, Object* rider) {
    return 0;
}

// offset: 0x1990 | func: 12 | export: 8
s32 DRearthwalk_func_1990(Object* self) {
    DRearthwalk_Data* objdata = self->data;

    return objdata->unkA5F != 0 ? 1 : 2;
}

// offset: 0x19B8 | func: 13 | export: 9
void DRearthwalk_func_19B8(Object* self, f32* a1, f32* a2, f32* a3) {
    DRearthwalk_Data* objdata = self->data;
    *a1 = objdata->unk9B4;
    *a2 = objdata->unk9B8;
    *a3 = objdata->unk9BC;
}

// offset: 0x19DC | func: 14 | export: 10
s32 DRearthwalk_func_19DC(Object* self, Object* rider) {
    DRearthwalk_Data* objdata = self->data;
    Vec3f sp48;
    
    if ((objdata->energy != 0) && (joyGetPressed(0) & B_BUTTON)) {
        gDLL_5_AMSEQ2->vtbl->set(self, 0xF6, 0, 0, 0);
        sp48.x = self->srt.transl.x;
        sp48.y = self->srt.transl.y;
        sp48.z = self->srt.transl.z;
        sp48.x += mathSinfInterp(self->srt.yaw - 0x4000) * 100.0f;
        sp48.z += mathCosfInterp(self->srt.yaw - 0x4000) * 100.0f;
        if (func_80059C40(&self->srt.transl, &sp48, 10.0f, 1, NULL, self, 8, -1, 0xFF, 0) != 0) {
            objdata->unkA5E = 1;
        } else {
            objdata->unkA5E = 0;
        }
        gDLL_3_Animation->vtbl->start_obj_sequence(objdata->unkA5E + 7, self, -1);
    }
    if (!objdata->energy) {
        gDLL_5_AMSEQ2->vtbl->set(self, 0xF6, 0, 0, 0);
    }
    return objdata->energy == 0;
}

// offset: 0x1BB8 | func: 15 | export: 11
s32 DRearthwalk_func_1BB8(Object* self) {
    DRearthwalk_Data* objdata = self->data;

    return objdata->unkA5E != 0 ? 2 : 1;
}

// offset: 0x1BE0 | func: 16 | export: 12
void DRearthwalk_func_1BE0(Object* self, f32* a1, f32* a2, f32* a3) {
    MtxF mtx;
    SRT srt;

    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.scale = 1.0f;
    mathYprXyzMtx(&mtx, &srt);
    mathMtxXFMF(&mtx, 0.0f, 50.0f, -35.0f, a1, a2, a3);
}

// offset: 0x1CA4 | func: 17 | export: 13
s32 DRearthwalk_func_1CA4(Object* self) {
    return 0;
}

// offset: 0x1CB4 | func: 18 | export: 14
void DRearthwalk_func_1CB4(Object* self, s32 arg1) {
    DRearthwalk_Data* objdata = self->data;
    objdata->unkA5A = arg1;
    if (arg1 == 0) {
        mainSetBits(BIT_7BC, 0);
        mainSetBits(BIT_7D4, 1);
        objdata->movedata.unk4A9 &= ~1;
    } else {
        mainSetBits(BIT_7BC, 1);
        mainSetBits(BIT_7D4, 0);
    }
}

// offset: 0x1D5C | func: 19 | export: 15
void DRearthwalk_func_1D5C(Object* self, f32* arg1, s32* arg2) {
    *arg1 = 0.0f;
    *arg2 = 0;
}

// offset: 0x1D84 | func: 20 | export: 16
f32 DRearthwalk_func_1D84(Object* self, f32* arg1) {
    *arg1 = 5.0f;
    return 0.0f;
}

// offset: 0x1DAC | func: 21 | export: 17
s32 DRearthwalk_func_1DAC(Object* self) {
    return 0;
}

// offset: 0x1DBC | func: 22 | export: 18
void DRearthwalk_func_1DBC(Object* self) { }

// offset: 0x1DC8 | func: 23 | export: 19
void DRearthwalk_func_1DC8(Object* self, f32 scale) {
    MtxF* sp4C;
    f32 atX;
    f32 atY;
    f32 atZ;
    SRT srt;

    sp4C = objGetAttachPointBoneMatrix(self, 2);
    objGetAttachPointLocalSpace(self, 2, &atX, &atY, &atZ);
    srt.yaw = 0;
    srt.pitch = 0;
    srt.roll = 0;
    srt.transl.x = atX;
    srt.transl.y = atY;
    srt.transl.z = atZ;
    srt.scale = scale / self->def->scale;
    mathYprXyzMtx(&_bss_28, &srt);
    mathMtxCat4x3F(&_bss_28, sp4C, &_bss_28);
    objprintSetModelMatrixOverride(&_bss_28);
}

// offset: 0x1EBC | func: 24
static int DRearthwalk_func_1EBC(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    DRearthwalk_Data* objdata;
    Object** objList;
    s32 i;
    s32 numObjs;
    s32 partIdx;
    DLL_IModgfx* modgfx;

    objdata = actor->data;
    actor->unkAF |= ARROW_FLAG_8_No_Targetting;
    if (((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func4(actor, animObjData, &objdata->movedata, 3, 3) != 0) {
        return 1;
    }
    if (objdata->unkA60_1 && !objdata->unkA60_2) {
        objList = objGetAllOfType(OBJTYPE_RopeNode, &numObjs);
        for (i = 0; i < numObjs; i++) {
            ((DLL_Unknown*)objList[i]->dll)->vtbl->func[15].withTwoArgsCustom2(objList[i], actor->srt.transl.y + 4.0f);
        }
        objdata->unkA60_2 = 1;
    }
    for (i = 0; i < animObjData->messageCount; i++) {
        switch (animObjData->messages[i]) {
        case 10:
            break;
        case 14:
            dll_amSfx->Play(actor, SOUND_57, MAX_VOLUME, NULL, NULL, 0, NULL);
            dll_amSfx->Play(actor, SOUND_9E, MAX_VOLUME, NULL, NULL, 0, NULL);
            for (partIdx = 0; partIdx != 10; partIdx++) {
                gDLL_17_partfx->vtbl->spawn(actor, PARTICLE_3C, NULL, 2, -1, NULL);
            }
            modgfx = dllLoad(DLL_ID_109, 1);
            modgfx->vtbl->func0(actor, 0, 0, 2, -1, 0);
            dllFree(modgfx);
            break;
        }
    }
    return 0;
}

// offset: 0x2174 | func: 25
static void DRearthwalk_func_2174(Object* self, ObjFSA_Data* fsa) {
    fsa->speed = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->unk278 = 0.0f;
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
}

// offset: 0x21A8 | func: 26 | anim state 0
s32 DRearthwalk_func_21A8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;
    if (objdata->unkA5C != 1) {
        objdata->unkA58 |= 0x20;
    }
    return 1 + 1;
}

// offset: 0x21DC | func: 27 | anim state 1
s32 DRearthwalk_func_21DC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;
    s16 var_a1;
    s32 temp_v0;
    
    DRearthwalk_func_2174(self, fsa);
    if (fsa->unk33A != 0) {
        // idle or idle attack(?) anims
        if (mathRnd(0, 5) == 0) {
            temp_v0 = mathRnd(0, 1);
            fsa->animTickDelta = _data_AC[temp_v0];
            var_a1 = _data_A8[temp_v0];
        } else {
            var_a1 = _data_DC[0];
        }
        fsa->animTickDelta = 0.01f;
        objAnimSet(self, var_a1, 0.0f, 0);
        objAnim_func_80024D74(self, 0x14);
    }
    if (fsa->analogInputPower < 0.1f) {
        fsa->unk328 = 0;
        fsa->unk32A = 0;
        fsa->analogInputPower = 0.0f;
    }
    if (objdata->energy <= 0) {
        return 2 + 1;
    }
    if (fsa->enteredAnimState != 0) {
        fsa->animStateTime = 0;
        fsa->animTickDelta = 0.01f;
        fsa->unk2B0 = 18.0f;
        if (self->curModAnimId != _data_DC[0]) {
            // standing idle anim?
            objAnimSet(self, _data_DC[0], 0.0f, 0);
            objAnim_func_80024D74(self, 0x14);
        }
    }
    if ((fsa->prevAnalogInputPower > 0.0f) && (fsa->analogInputPower > 0.0f)) {
        return 4 + 1;
    }
    if (fsa->unk310 & 0x8000) {
        return 5 + 1;
    }
    if (mathRnd(0, 60) == 0) {
        objExpr_func_80034B54(self, &objdata->unk394, _data_8[mathRnd(0, 2)], 0);
    }
    return 0;
}

// offset: 0x2454 | func: 28 | anim state 2
s32 DRearthwalk_func_2454(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;

    fsa->flags |= OBJFSA_FLAG_200000;
    objdata->talkSeq = EWSEQ_3_INeedEnergy;
    switch (self->curModAnimId) {
    case 10:
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        if (fsa->unk33A != 0) {
            // laying anim
            objAnimSet(self, 0xE, 0.0f, 0);
            fsa->animTickDelta = 0.01f;
        }
        if (objdata->energy > 0) {
            if ((fsa->animTickDelta > 0.0f) && ((fsa->unk310 != 0) || (fsa->xAnalogInput != 0.0f) || (fsa->yAnalogInput != 0.0f))) {
                fsa->animTickDelta = -fsa->animTickDelta;
            }
        }
        break;
    case 12:
    case 14:
    case 16:
    case 17:
    case 18:
    case 19:
        fsa->animTickDelta = 0.005f;
        if ((objdata->energy != 0) && (objdata->energy >= 5)) {
            // transition to standing up anim
            objAnimSet(self, 0xB, 0.0f, 0);
            fsa->animTickDelta = 0.01f;
        }
        break;
    case 11:
        if (fsa->unk33A != 0) {
            objdata->movedata.unk4A9 &= ~0x2;
            if (objdata->unkA58 & 0x20) {
                objdata->rideable = 1;
            } else {
                objdata->talkSeq = EWSEQ_9_IfICouldGetOutOfTheseChains;
            }
            objdata->movedata.unk4A9 &= ~1;
            return 1 + 1;
        }
        break;
    default:
        objdata->rideable = 0;
        // transition to laying down anim
        objAnimSet(self, 0xA, 0.0f, 0);
        objdata->movedata.unk4A9 |= 2;
        DRearthwalk_func_2174(self, fsa);
        fsa->animTickDelta = 0.01f;
        break;
    }

    if (mathRnd(0, 30) == 0) {
        objExpr_func_80034B54(self, &objdata->unk394, _data_8[mathRnd(0, 4)], 0);
    }
    return 0;
}

// offset: 0x2704 | func: 29 | anim state 3
s32 DRearthwalk_func_2704(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;
    s16 temp_ft1;
    
    fsa->flags |= OBJFSA_FLAG_200000;
    if ((fsa->unk328 < (objdata->unkA52 / 3)) || (fsa->analogInputPower == 0.0f)) {
        return 4 + 1;
    }
    if (fsa->unk32A < -0xAF) {
        fsa->unk32A = -fsa->unk32A;
    }
    if (self->curModAnimId != 5) {
        // running attack?
        objAnimSet(self, 5, 0.0f, 0);
    }
    temp_ft1 = (f32) fsa->unk32A * updateRate * 182.04f;
    self->srt.yaw += temp_ft1 >> 4;
    DRearthwalk_func_3210(self, temp_ft1, 0);
    fsa->animTickDelta = fsa->analogInputPower * 0.08f;
    if (fsa->unk310 & 0x8000) {
        return 5 + 1;
    }
    return 0;
}

// offset: 0x2860 | func: 30 | anim state 4
#ifndef NON_MATCHING
s32 DRearthwalk_func_2860(Object *self, ObjFSA_Data *fsa, f32 updateRate);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/DRearthwalk_func_2860.s")
#else
s32 DRearthwalk_func_2860(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    u32 var_v1_3;
    s16 temp2;
    u32 var_t0;
    f32 var_fa0;
    f32 temp_fv1;
    s32 _pad2;
    DRearthwalk_Data* sp2C;
    s32 _pad3;
    s32 _pad4;
    s32 sp20;
    f32 var_fv0;
    s32 temp;

    sp2C = self->data;
    sp20 = 1;
    fsa->flags |= OBJFSA_FLAG_200000;
    if (fsa->enteredAnimState != 0) {
        fsa->unk32A = 0;
        fsa->unk328 = 0;
    }
    if (fsa->analogInputPower < 0.08f) {
        fsa->unk328 = 0;
        fsa->unk32A = 0;
        fsa->analogInputPower = 0.0f;
    }
    fsa->unk32A = CLAMP_EXPR(fsa->unk32A, -0x1000, 0x1000);
    if (fsa->unk32A >= 0) {
        temp = fsa->unk32A;
    } else {
        temp = -fsa->unk32A;
    }
    if (sp2C->unkA52 < temp) {
        temp2 = (f32) fsa->unk32A * updateRate * 182.04f;
        self->srt.yaw += temp2 >> 5;
    } else {
        self->srt.yaw += ((((f32) fsa->unk32A * updateRate) / 36.0f) * 182.0f);
    }
    DRearthwalk_func_3210(self, (s16) ((f32) fsa->unk32A * updateRate * 182.04f), 0);
    var_fv0 = fsa->analogInputPower;
    if (var_fv0 < 0.0f) {
        var_fv0 = 0.0f;
    }
    if (var_fv0 > 1.0f) {
        var_fv0 = 1.0f;
    }
    var_fa0 = var_fv0 * 2.7f;
    if (var_fa0 < 0) {
        var_fa0 = 0;
    }
    fsa->speed += (((var_fa0 - fsa->speed) / fsa->unk2B0) * updateRate);
    if (self->srt.pitch > 0) {
        var_fa0 -= (mathSinfInterp(self->srt.pitch) * 0.3f);
    } else {
        var_fa0 -= (mathSinfInterp(self->srt.pitch) * 0.15f);
    }
    fsa->unk278 += (((var_fa0 - fsa->unk278) / fsa->unk2B0) * updateRate);
    var_t0 = 0;
    temp_fv1 = self->animProgress;
    var_v1_3 = 0;
    while (self->curModAnimId != _data_DC[var_v1_3] && var_v1_3 < 5) {
        var_v1_3++;
    }
    if (var_v1_3 >= 5) {
        var_v1_3 = 0;
    }
    if (mathRnd(0, 0x46 - (s32) ((fsa->speed / 2.7f) * 50.0f)) == 0) {
        objExpr_func_80034B54(self, &sp2C->unk394, _data_8[mathRnd(0, 3)], 0);
    }

    while (sp20) {
        if (fsa->speed < _data_B4[var_v1_3][0]) {
            if (var_v1_3 == 1) {
                return 1 + 1;
            }
            var_v1_3 -= 1;
            var_t0 = 1;
        } else if (_data_B4[var_v1_3][1] <= fsa->speed) {
            if (var_v1_3 == 0) {
                temp_fv1 = 0.0f;
            }
            var_v1_3 += 1;
            var_t0 = 1;
        } else {
            sp20 = 0;
        }
    }
    if (var_t0 != 0) {
        // walking/running anims
        objAnimSet(self, _data_DC[var_v1_3], temp_fv1, 0);
        objAnim_func_80024D74(self, 0xA);
    }
    objGetAnimChange(self, fsa->unk278, &fsa->animTickDelta);
    if (fsa->unk310 & 0x8000) {
        joyDisableButtons(0, A_BUTTON);
        return 5 + 1;
    }
    return 0;
}
#endif

// offset: 0x2D80 | func: 31 | anim state 5
s32 DRearthwalk_func_2D80(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    ObjectHitInfo* hit;
    DRearthwalk_Data* objdata;

    hit = self->objhitInfo;
    objdata = self->data;
    fsa->flags |= OBJFSA_FLAG_200000;
    if (fsa->enteredAnimState != 0) {
        objdata->unkA58 &= ~0x8;
        hit->unk58 |= 0x200;
        if (fsa->unk278 > 0.45f) {
            // running attack anim
            objAnimSet(self, 0x14, 0.0f, 0);
            fsa->animTickDelta = 0.021f;
            hit->unk60 = fsa->unk278 > 1.93f ? 3 : 2;
        } else {
            // standing attack anim
            objAnimSet(self, 0x15, 0.0f, 0);
            fsa->animTickDelta = 0.011f;
            hit->unk60 = 1;
        }
    }
    if (self->animProgress < 0.4f) {
        if ((self->animProgress + (fsa->animTickDelta * (f32) gUpdateRate)) > 0.4f) {
            objdata->unkA60_5 = 1;
        }
    }
    hit->unk5F = 5;
    if (fsa->unk33A != 0) {
        objdata->unkA60_5 = 0;
        return 1 + 1;
    }
    if (self->curModAnimId == 0x14) {
        gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    }
    return 0;
}

// offset: 0x2F90 | func: 32 | anim state 6
s32 DRearthwalk_func_2F90(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;

    DRearthwalk_func_2174(self, fsa);
    if (fsa->enteredAnimState != 0) {
        if (objdata->energy > 0) {
            // hurt while standing anims
            if (objdata->unkA60_0 != 0) {
                objAnimSet(self, 7, 0.0f, 0);
            } else {
                objAnimSet(self, 8, 0.0f, 0);
            }
        } else {
            // hurt while laying anims
            if (objdata->unkA60_0 != 0) {
                objAnimSet(self, 0x12, 0.0f, 0);
            } else {
                objAnimSet(self, 0x13, 0.0f, 0);
            }
        }
        fsa->animTickDelta = 0.02f;
    }
    if (self->animProgress > 0.9f) {
        objdata->energy--;
        if (objdata->energy <= 0) {
            if (objdata->energy < -20) {
                mainSetBits(BIT_5DE, 1);
            }
            return 2 + 1;
        }
        return 1 + 1;
    }
    return 0;
}

// offset: 0x311C | func: 33 | anim state 7
s32 DRearthwalk_func_311C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;
    
    if (fsa->enteredAnimState != 0) {
        DRearthwalk_func_2174(self, fsa);
        objAnimSet(self, _data_DC[0], 0.0f, 0);
    }
    if ((mainGetBits(BIT_5CF) == 0) && (mainGetBits(BIT_5CD) != 0)) {
        mainSetBits(BIT_DR_EarthWarriorBroughtToSurface, 1);
        objdata->rideable = 1;
        return 1 + 1;
    }
    return 0;
}

// offset: 0x31F8 | func: 34 | logic state 0
s32 DRearthwalk_func_31F8(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    return 0;
}

// offset: 0x3210 | func: 35
static void DRearthwalk_func_3210(Object* self, s16 arg1, s16 arg2) {
    s16* temp_v0;

    temp_v0 = objExpr_func_80034804(self, 9);
    if (temp_v0 != NULL) {
        temp_v0[1] += ((arg1 - temp_v0[1]) >> 5);
        temp_v0[0] += ((arg2 - temp_v0[0]) >> 5);
        
        // @bug? should be clamping [0]?
        temp_v0[1] = CLAMP_EXPR(temp_v0[1], -0x1555, 0x1555);
        temp_v0[1] = CLAMP_EXPR(temp_v0[1], -0x1555, 0x1555);
    }
}

// offset: 0x32EC | func: 36
static s32 DRearthwalk_func_32EC(Object* self, u8 arg1) {
    DRearthwalk_Data* objdata;

    objdata = self->data;
    switch (arg1) {
        case 5:
            objdata->unkA58 |= 0x80;
            objdata->talkSeq = EWSEQ_10_TeethChattering;
            //Set Mind Read text
            if (!gDLL_22_Subtitles->vtbl->func_21C0(self->id, GAMETEXT_0CF_DR_Mind_Read_messages_4)) {
                break;
            }
        // fallthrough
        case 1:
            objdata->talkSeq = EWSEQ_2_LetsGoStopTheDragon;
            lfxAction(self, self, 0x22C, 0, 0, 0);
            break;
        case 2:
            lfxAction(self, self, 0x22E, 0, 0, 0);
            objdata->unkA58 &= ~0x80;
            //Set Mind Read text
            gDLL_22_Subtitles->vtbl->func_21C0(self->id, GAMETEXT_0D0_DR_Mind_Read_messages_5);
    }

    return 1;
}

// offset: 0x3420 | func: 37 | export: 20
void DRearthwalk_func_3420(Object* self, s32 arg1, s32 arg2) {
    DRearthwalk_Data* objdata = self->data;
    if (arg1 == 1) {
        objdata->energy += 12;
        objExpr_func_80034B54(self, &objdata->unk394, _data_24[0], 1);
    }
}
