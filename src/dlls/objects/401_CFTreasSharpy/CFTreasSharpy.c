#include "dlls/engine/26_curves.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/animseq.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "dll.h"
#include "macros.h"

/** @file official name: CFTreasSharpy.c */

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
} CFTreasSharpy_Setup;

typedef struct {
    Object* unk0;
    f32 unk4;
    f32 unk8;
    Vec3f unkC;
    Vec3f unk18;
    f32 unk24;
    u8 _unk28[0x2C - 0x28];
    f32 unk2C;
    UnkCurvesStruct unk30;
    s16 unk138;
    u8 unk13A;
    u8 unk13B;
    u8 unk13C;
    u8 unk13D_0 : 1;
} CFTreasSharpy_Data;

static int CFTreasSharpy_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8);
s32 CFTreasSharpy_func_8E4(Object*, f32, f32);
s32 CFTreasSharpy_func_9E8(Object*);
static s32 CFTreasSharpy_func_118C(Object* self, Object* arg1, f32 speed);
static void CFTreasSharpy_func_1460(Object* self, s32 arg1, Vec3f* arg2, s32 arg3);
s32 CFTreasSharpy_func_1530(Object* arg0, UnkCurvesStruct* arg1, f32 arg2, u8 arg3, Vec3f* arg4, f32* arg5) ;

// offset: 0x0 | ctor
void CFTreasSharpy_ctor(void* dll) { }

// offset: 0xC | dtor
void CFTreasSharpy_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFTreasSharpy_obj_Setup(Object* self, CFTreasSharpy_Setup* setup, s32 reset) {
    CFTreasSharpy_Data* objdata = self->data;
    
    objInitMesgQueue(self, 4);
    self->animCallback = CFTreasSharpy_animCallback;
    if ((setup->unk1E != -1) && (mainGetBits(setup->unk1E) != 0)) {
        objdata->unk13A = 4;
    } else {
        objdata->unk13A = 0;
    }
    self->srt.yaw = setup->unk18 << 8;
    objdata->unk13C = setup->unk19;
    objdata->unk13B = 0;
    if (setup->unk1C != 0) {
        objdata->unk8 = setup->unk1C;
    } else {
        objdata->unk8 = 50.0f;
    }
    objdata->unk2C = objdata->unk8 * 0.0001f;
}

// offset: 0x12C | func: 1 | export: 1
#ifndef NON_MATCHING
/*0x0*/ static const char str_0[] = " HELLO ";
/*0x8*/ static const char str_8[] = " GOODBYE ";
void CFTreasSharpy_obj_Control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/401_CFTreasSharpy/CFTreasSharpy_obj_Control.s")
#else
void CFTreasSharpy_obj_Control(Object* self) {
    CFTreasSharpy_Setup* setup;
    CFTreasSharpy_Data* objdata;
    CurveSetup* curve;
    Object* player;

    setup = (CFTreasSharpy_Setup*)self->setup;
    objdata = self->data;
    player = objGetPlayer();
    switch (objdata->unk13A) {
    case 0:
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        objdata->unk13A = 1;
        break;
    case 1:
        if (((setup->unk1A + 200.0f) < vec3Distance(&self->globalPosition, &player->globalPosition)) || (mainGetBits(setup->unk20) != 0)) {
            STUBBED_PRINTF(" HELLO ");
            objdata->unk13A = 0;
        } else if (vec3Distance(&self->globalPosition, &player->globalPosition) < 30.0f) {
            STUBBED_PRINTF(" GOODBYE ");
            objdata->unk13A = 3;
        } else if (CFTreasSharpy_func_8E4(self, 400.0f, objdata->unk8 * 0.01f) != 0) {
            objdata->unk13A = 2;
            objdata->unk138 = self->srt.yaw;
        }
        break;
    case 2:
        if (CFTreasSharpy_func_9E8(self) != 0) {
            objdata->unk13A = 1;
        }
        break;
    case 3:
        if (CFTreasSharpy_func_1530(self, &objdata->unk30, objdata->unk8 * 0.01f, objdata->unk13C, &objdata->unk18, &objdata->unk2C) != 0) {
            objdata->unk13A = 4;
            if (setup->unk1E != -1) {
                mainSetBits(setup->unk1E, 1);
            }
        }
        curve = objdata->unk30.unk9C;
        if ((curve != NULL) && (curve->type2.unk31 != 0)) {
            mainSetBits(setup->unk1E, 1);
        }
        break;
    case 5:
        func_800267A4(self);
        objdata->unk4 += (gUpdateRateF * 0.02f);
        objdata->unk2C = 0.008f;
        if (objdata->unk13B != 0) {
            objdata->unk13A = 4;
            if (setup->unk1E != -1) {
                mainSetBits(setup->unk1E, 1);
            }
        }
        break;
    case 4:
        objdata->unk4 = 0.0f;
        objDisable(self);
        self->objhitInfo->unk58 &= ~1;
        self->unkAF |= 8;
        self->srt.flags |= 0x4000;
        break;
    }
    objdata->unk13B = objAnimAdvance(self, objdata->unk2C, (f32) gUpdateRate, NULL);
}
#endif

// offset: 0x4AC | func: 2 | export: 2
void CFTreasSharpy_obj_Update(Object* self) {
    CFTreasSharpy_Data* temp_s0;
    Object* sp40;
    s32 damageType;
    s32 _pad;

    temp_s0 = self->data;
    damageType = func_80025F40(self, &sp40, NULL, NULL);
    if (damageType != 0) {
        temp_s0->unk13D_0 = 0;
    }
    if ((damageType != 0) && (damageType != Damage_Type_Bullet)) {
        self->unkAF &= ARROW_FLAG_8_No_Targetting; // @bug
        temp_s0->unk13A = 5;
        gDLL_6_AMSFX->vtbl->Play(self, SOUND_23A_SharpClaw_Ugh_Snort, MAX_VOLUME, NULL, NULL, 0, NULL);
        objAnimSet(self, 0x101, 0.0f, 0);
        if (temp_s0->unk0 != NULL) {
            objSendMesg(temp_s0->unk0, 0x11, temp_s0->unk0, (void* ) (s32) temp_s0->unk8);
        }
    }
}

// offset: 0x5BC | func: 3 | export: 3
void CFTreasSharpy_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CFTreasSharpy_Data* objdata;
    ModelInstance* modelInst;
    MtxF* mtx;
    s32 bone;
    f32 x;
    f32 y;
    f32 z;

    objdata = self->data;
    if ((visibility != 0) && (objdata->unk13A != 4)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        if (self->def->numAttachPoints != 0) {
            modelInst = self->modelInsts[self->modelInstIdx];
            bone = self->def->pAttachPoints[0].bones[self->modelInstIdx];
            mtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
            x = mtx->m[3][0] + gWorldX;
            y = mtx->m[3][1];
            z = mtx->m[3][2] + gWorldZ;
            bone = self->def->pAttachPoints[1].bones[self->modelInstIdx];
            mtx = (MtxF*) &((f32*)modelInst->matrices[modelInst->unk34 & 1])[bone << 4];
            objdata->unkC.f[0] = (mtx->m[3][0] + gWorldX);
            objdata->unkC.f[1] = (mtx->m[3][1]);
            objdata->unkC.f[2] = (mtx->m[3][2] + gWorldZ);
            objdata->unkC.f[0] = ((objdata->unkC.f[0] + x) * 0.5f);
            objdata->unkC.f[1] = ((objdata->unkC.f[1] + y) * 0.5f);
            objdata->unkC.f[2] = ((objdata->unkC.f[2] + z) * 0.5f);
            
        }
        if (objdata->unk4 > 0.0f) {
            if (objdata->unk4 < 2.3f) {
                gDLL_32_modelfx->vtbl->func2(self, 0x32F, &objdata->unk4);
            }
            if (objdata->unk4 > 1.2f) {
                gDLL_32_modelfx->vtbl->func2(self, 0x330, &objdata->unk4);
                gDLL_32_modelfx->vtbl->func2(self, 0x330, &objdata->unk4);
            }
        }
    }
}

// offset: 0x7E8 | func: 4 | export: 4
void CFTreasSharpy_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x7F8 | func: 5 | export: 5
u32 CFTreasSharpy_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x808 | func: 6 | export: 6
u32 CFTreasSharpy_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFTreasSharpy_Data);
}

// offset: 0x81C | func: 7
static int CFTreasSharpy_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    CFTreasSharpy_Setup* setup;
    Vec3f* playerPos;
    CFTreasSharpy_Data* objdata;

    setup = (CFTreasSharpy_Setup*)self->setup;
    playerPos = &objGetPlayer()->globalPosition;
    objdata = self->data;
    if ((vec3Distance(&self->globalPosition, playerPos) < setup->unk1A) 
            && (mainGetBits(setup->unk20) == 0)) {
        STUBBED_PRINTF("Near ");
        return 4;
    }
    return objdata->unk13D_0;
}

// offset: 0x8E4 | func: 8
s32 CFTreasSharpy_func_8E4(Object* self, f32 arg1, f32 arg2) {
    s32 _pad;
    CFTreasSharpy_Data* objdata;
    s32 sp24;
    Object* chuckobj;

    objdata = self->data;
    sp24 = 0;
    chuckobj = objGetNearestTypeTo(OBJTYPE_CFChuckObj, self, &arg1);
    if (chuckobj != NULL) {
        sp24 = CFTreasSharpy_func_118C(self, chuckobj, arg2);
        if (self->curModAnimId != 1) {
            objAnimSet(self, 1, 0.0f, 0);
        } else {
            objGetAnimChange(self, arg2, &objdata->unk2C);
        }
    } else {
        objdata->unk13A = 3;
        CFTreasSharpy_func_1460(self, objdata->unk13C, &objdata->unk18, 2);
        STUBBED_PRINTF(" nx %f ny %f hz %f\n", &objdata->unk18.x, &objdata->unk18.y, &objdata->unk18.z);
    }
    return sp24;
}

// offset: 0x9E8 | func: 9
#ifndef NON_MATCHING
/*0x30*/ static const char str_30[] = " nx %f ny %f hz %f\n";
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/401_CFTreasSharpy/CFTreasSharpy_func_9E8.s")
#else
s32 CFTreasSharpy_func_9E8(Object* self) {
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    s16 sp46;
    s16 sp44;
    s32 sp38;
    Object* temp_v0_3;
    s32 temp_v0_5;
    s16 temp_v1_4;
    s16 var_a0;
    s32 temp_v1_2;
    s32 var_v0;
    CFTreasSharpy_Data* temp_s0;

    temp_s0 = self->data;
    sp48 = 50.0f;
    temp_s0->unk2C = (f32) (temp_s0->unk8 * 0.0002f);
    if ((temp_s0->unk0 == NULL) && (self->curModAnimId != 0x302)) {
        temp_s0->unk0 = objGetNearestTypeTo(OBJTYPE_CFChuckObj, self, &sp48);
        if (temp_s0->unk0 == NULL) {
            temp_s0->unk13A = 3;
            CFTreasSharpy_func_1460(self, temp_s0->unk13C, &temp_s0->unk18, 2);
            STUBBED_PRINTF(" nx %f ny %f hz %f\n", &temp_s0->unk18.x, &temp_s0->unk18.y, &temp_s0->unk18.z);
            return 1;
        }
    }
    switch (self->curModAnimId) {
    case 0x1:
        temp_v0_3 = objGetPlayer();
        sp58 = self->srt.transl.x - temp_v0_3->srt.transl.x;
        sp54 = self->srt.transl.z - temp_v0_3->srt.transl.z;
        sp4C = SQ(sp58) + SQ(sp54);
        sp58 = temp_s0->unk0->srt.transl.x - temp_v0_3->srt.transl.x;
        sp54 = temp_s0->unk0->srt.transl.z - temp_v0_3->srt.transl.z;
        sp44 = mathAtan2f(sp58, sp54) & 0xFFFF;
        temp_v1_2 = temp_s0->unk138;
        temp_v1_2 = temp_v1_2 - sp44;
        if (temp_v1_2 >= 0) {
            var_v0 = temp_v1_2;
        } else {
            var_v0 = -temp_v1_2;
        }
        if ((var_v0 < 0x400) && ((SQ(sp58) + SQ(sp54)) < sp4C)) {
            temp_s0->unk0->srt.yaw = temp_s0->unk138;
            objAnimSet(self, 0x2B, 0.0f, 0);
        } else {
            sp46 = temp_s0->unk138 - (u16)sp44;
            CIRCLE_WRAP(sp46);
            if (sp46 < 0) {
                temp_s0->unk138 += ((s16) temp_s0->unk8 * 8);
                if (self->curModAnimId != 1) {
                    objAnimSet(self, 1, self->animProgress, 0);
                }
            } else {
                if (self->curModAnimId != 1) {
                    objAnimSet(self, 1, self->animProgress, 0);
                }
                temp_s0->unk138 -= ((s16) temp_s0->unk8 * 8);
            }
            sp50 = vec3DistanceXZ(&self->globalPosition, &temp_s0->unk0->globalPosition);
            sp58 = self->srt.transl.x;
            sp54 = self->srt.transl.z;
            self->srt.transl.x = (mathSinfInterp(temp_s0->unk138) * sp50) + temp_s0->unk0->srt.transl.x;
            self->srt.transl.z = (mathCosfInterp(temp_s0->unk138) * sp50) + temp_s0->unk0->srt.transl.z;
            sp58 = self->srt.transl.x - sp58;
            sp54 = self->srt.transl.z - sp54;
            objGetAnimChange(self, sqrtf(SQ(sp58) + SQ(sp54)) / 3.0f, &temp_s0->unk2C);
            var_a0 = mathAtan2f(sp58, sp54) + 0x8000;
            if (sp46 > 0) {
                var_a0 = (((s32) (var_a0 * sp46)) / 32768) + (((s32) (sp44 * (0x8000 - sp46))) / 32768);;
            } else {
                sp46 = sp46 + 0xFFFF;
                var_a0 = ((s32) ((var_a0) * sp46) / 32768) + ((s32) (sp44 * (0x8000 - sp46)) / 32768);
            }
            self->srt.yaw = var_a0;
        }
        break;
    case 0x2B:
        if (self->animProgress > 0.4f) {
            if (temp_s0->unk24 == -655370.0f) {
                temp_s0->unk24 = (f32) (temp_s0->unkC.y - temp_s0->unk0->srt.transl.y);
            }
            temp_s0->unk0->srt.transl.x = temp_s0->unkC.x;
            temp_s0->unk0->srt.transl.y = temp_s0->unkC.y - temp_s0->unk24;
            temp_s0->unk0->srt.transl.z = temp_s0->unkC.z;
        } else if (self->animProgress > 0.2f) {
            temp_s0->unk24 = -655370.0f;
            temp_s0->unk0->srt.transl.x = (temp_s0->unkC.x + temp_s0->unk0->srt.transl.x) * 0.5f;
            temp_s0->unk0->srt.transl.z = (temp_s0->unkC.z + temp_s0->unk0->srt.transl.z) * 0.5f;
        }
        if (temp_s0->unk13B != 0) {
            objAnimSet(self, 0x302, 0.0f, 0);
        }
        break;
    case 0x302:
        if (self->animProgress < .5f) {
            temp_s0->unk0->srt.transl.x = temp_s0->unkC.x;
            temp_s0->unk0->srt.transl.y = temp_s0->unkC.y - temp_s0->unk24;
            temp_s0->unk0->srt.transl.z = temp_s0->unkC.z;
        } else if (temp_s0->unk0 != NULL) {
            sp38 = (s32) temp_s0->unk8;
            objSendMesg(temp_s0->unk0, 0x11, objGetPlayer(), (void* ) sp38);
            temp_s0->unk0 = NULL;
        }
        if (temp_s0->unk13B != 0) {
            temp_s0->unk13A = 1;
            objAnimSet(self, 1, 0.0f, 0);
        }
        break;
    }
    return 0;
}
#endif

// offset: 0x1040 | func: 10
static void CFTreasSharpy_func_1040(Object* arg0, UnkCurvesStruct* arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 sp28[2];

    if (arg2 == 2) {
        sp28[0] = 2;
        sp28[1] = 0x15;
    } else {
        sp28[0] = 2;
        sp28[1] = 0x15;
    }
    gDLL_26_Curves->vtbl->func_4288(arg1, arg0, arg4, sp28, arg3);
    STUBBED_PRINTF(" PathId %i Loc id %i \n", arg3, arg1->unk9C->unk18);
}

// offset: 0x10D4 | func: 11
static s32 CFTreasSharpy_func_10D4(Object* arg0, UnkCurvesStruct* arg1, f32 arg2) {
    s32 sp24;

    sp24 = 0;
    if ((curves_func_800053B0(&arg1->unk0, arg2) != 0) || (arg1->unk0.unk10 != 0)) {
        sp24 = gDLL_26_Curves->vtbl->func_4704(arg1);
    }
    arg0->srt.transl.x = arg1->unk0.unk68.x;
    arg0->srt.transl.y = arg1->unk0.unk68.y;
    arg0->srt.transl.z = arg1->unk0.unk68.z;
    return sp24;
}

// offset: 0x118C | func: 12
static s32 CFTreasSharpy_func_118C(Object* self, Object* arg1, f32 speed) {
    f32 xDir;
    f32 yDir;
    f32 zDir;
    f32 magnitude;

    xDir = arg1->srt.transl.x - self->srt.transl.x;
    yDir = arg1->srt.transl.y - self->srt.transl.y;
    zDir = arg1->srt.transl.z - self->srt.transl.z;
    magnitude = SQ(xDir) + SQ(yDir) + SQ(zDir);
    if (!(self->def->flags & 1)) {
        if (self->objhitInfo != NULL) {
            if (arg1->objhitInfo != NULL) {
                magnitude -= (SQ(self->objhitInfo->unk52) + SQ(arg1->objhitInfo->unk52));
            }
        }
    }
    if (magnitude < 5.0f) {
        return 1;
    }
    guNormalize(&xDir, &yDir, &zDir);
    self->srt.transl.x += xDir * speed * gUpdateRateF;
    self->srt.transl.y += yDir * speed * gUpdateRateF;
    self->srt.transl.z += zDir * speed * gUpdateRateF;
    self->srt.yaw = mathAtan2f(xDir, zDir) + 0x8000;
    return 0;
}

// offset: 0x1338 | func: 13
static s32 CFTreasSharpy_func_1338(Object* self, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;

    sp2C = arg1 - self->srt.transl.x;
    sp28 = arg2 - self->srt.transl.y;
    sp24 = arg3 - self->srt.transl.z;
    if ((SQ(sp2C) + SQ(sp28) + SQ(sp24)) < 5.0f) {
        return 1;
    }
    guNormalize(&sp2C, &sp28, &sp24);
    self->srt.transl.x += (sp2C * arg4 * gUpdateRateF);
    self->srt.transl.y += (sp28 * arg4 * gUpdateRateF);
    self->srt.transl.z += (sp24 * arg4 * gUpdateRateF);
    return 0;
}

// offset: 0x1460 | func: 14
static void CFTreasSharpy_func_1460(Object* self, s32 arg1, Vec3f* arg2, s32 arg3) {
    CurveSetup* setup;
    s32 curveTypes[2];
    s32 uid;

    if (arg3 == 2) {
        curveTypes[0] = 2;
        curveTypes[1] = 0x15;
    } else {
        curveTypes[0] = 0x19;
        curveTypes[1] = 0x15;
    }
    uid = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, curveTypes, ARRAYCOUNT(curveTypes), arg1);
    if (uid >= 0) {
        setup = gDLL_26_Curves->vtbl->func_39C(uid);
        arg2->x = setup->pos.x;
        arg2->y = setup->pos.y;
        arg2->z = setup->pos.z;
    } else {
        STUBBED_PRINTF(" Error Could not find node ");
    }
    STUBBED_PRINTF(" PathId %i Loc id %i \n", arg1, setup->unk18);
}

// offset: 0x1530 | func: 15
s32 CFTreasSharpy_func_1530(Object* arg0, UnkCurvesStruct* arg1, f32 arg2, u8 arg3, Vec3f* arg4, f32* arg5) {
    s32 sp2C;

    sp2C = 0;
    if (arg0->unkDC == -1) {
        return 1;
    }
    if (arg0->unkDC == 0) {
        if (CFTreasSharpy_func_1338(arg0, arg4->x, arg4->y, arg4->z, arg2) != 0) {
            CFTreasSharpy_func_1040(arg0, arg1, 2, arg3, 400.0f);
            arg0->unkDC = 1;
        }
    } else {
        sp2C = CFTreasSharpy_func_10D4(arg0, arg1, arg2);
        if (sp2C != 0) {
            arg0->unkDC = -1;
        }
    }
    arg0->srt.yaw = mathAtan2f(
        arg0->srt.transl.x - arg0->prevLocalPosition.x, 
        arg0->srt.transl.z - arg0->prevLocalPosition.z) + 0x8000;
    objGetAnimChange(arg0, arg2, arg5);
    return sp2C;
}
