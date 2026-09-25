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
/*18*/ s8 yaw8;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
} CFTreasSharpy_Setup;

typedef struct {
    Object* chuckObj;
    f32 unk4;
    f32 unk8;
    Vec3f unkC;
    Vec3f unk18;
    f32 unk24;
    u8 _unk28[0x2C - 0x28];
    f32 unk2C;
    UnkCurvesStruct unk30;
    s16 unk138;
    u8 state;
    u8 unk13B;
    u8 unk13C;
    u8 unk13D_0 : 1;
} CFTreasSharpy_Data;

static int CFTreasSharpy_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8);
static s32 CFTreasSharpy_func_8E4(Object*, f32, f32);
static s32 CFTreasSharpy_func_9E8(Object*);
static s32 CFTreasSharpy_func_118C(Object* self, Object* arg1, f32 speed);
static void CFTreasSharpy_func_1460(Object* self, s32 arg1, Vec3f* arg2, s32 arg3);
static s32 CFTreasSharpy_func_1530(Object* self, UnkCurvesStruct* arg1, f32 arg2, u8 arg3, Vec3f* arg4, f32* arg5) ;

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
        objdata->state = 4;
    } else {
        objdata->state = 0;
    }
    self->srt.yaw = setup->yaw8 << 8;
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
void CFTreasSharpy_obj_Control(Object* self) {
    CFTreasSharpy_Setup* setup = (CFTreasSharpy_Setup*)self->setup;
    CFTreasSharpy_Data* objdata = self->data;
    CurveSetup* curve;
    Object* player = objGetPlayer();
    
    switch (objdata->state) {
    case 0:
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        objdata->state = 1;
        break;
    case 1:
        if (((setup->unk1A + 200.0f) < vec3Distance(&self->globalPosition, &player->globalPosition)) || (mainGetBits(setup->unk20) != 0)) {
            STUBBED_PRINTF(" HELLO ");
            objdata->state = 0;
        } else if (vec3Distance(&self->globalPosition, &player->globalPosition) < 30.0f) {
            STUBBED_PRINTF(" GOODBYE ");
            objdata->state = 3;
        } else if (CFTreasSharpy_func_8E4(self, 400.0f, objdata->unk8 * 0.01f) != 0) {
            objdata->state = 2;
            objdata->unk138 = self->srt.yaw;
        }
        break;
    case 2:
        if (CFTreasSharpy_func_9E8(self) != 0) {
            objdata->state = 1;
        }
        break;
    case 3:
        if (CFTreasSharpy_func_1530(self, &objdata->unk30, objdata->unk8 * 0.01f, objdata->unk13C, &objdata->unk18, &objdata->unk2C) != 0) {
            objdata->state = 4;
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
            objdata->state = 4;
            if (setup->unk1E != -1) {
                mainSetBits(setup->unk1E, 1);
            }
        }
        break;
    case 4:
        objdata->unk4 = 0.0f;
        objDisable(self);
        self->objhitInfo->unk58 &= ~1;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        self->srt.flags |= OBJFLAG_INVISIBLE;
        break;
    }
    objdata->unk13B = objAnimAdvance(self, objdata->unk2C, (f32) gUpdateRate, NULL);
}

// offset: 0x4AC | func: 2 | export: 2
void CFTreasSharpy_obj_Update(Object* self) {
    CFTreasSharpy_Data* objdata = self->data;
    Object* hitBy;
    s32 damageType;
    s32 _pad;

    damageType = func_80025F40(self, &hitBy, NULL, NULL);
    if (damageType != 0) {
        objdata->unk13D_0 = 0;
    }
    if ((damageType != 0) && (damageType != Damage_Type_Bullet)) {
        self->unkAF &= ARROW_FLAG_8_No_Targetting; // @bug
        objdata->state = 5;
        gDLL_6_AMSFX->vtbl->Play(self, SOUND_23A_SharpClaw_Ugh_Snort, MAX_VOLUME, NULL, NULL, 0, NULL);
        objAnimSet(self, 0x101, 0.0f, 0);
        if (objdata->chuckObj != NULL) {
            objSendMesg(objdata->chuckObj, 0x11, objdata->chuckObj, (void* ) (s32) objdata->unk8);
        }
    }
}

// offset: 0x5BC | func: 3 | export: 3
void CFTreasSharpy_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CFTreasSharpy_Data* objdata = self->data;
    ModelInstance* modelInst;
    MtxF* mtx;
    s32 bone;
    f32 x;
    f32 y;
    f32 z;
    
    if ((visibility != 0) && (objdata->state != 4)) {
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
    CFTreasSharpy_Setup* setup = (CFTreasSharpy_Setup*)self->setup;
    Vec3f* playerPos = &objGetPlayer()->globalPosition;
    CFTreasSharpy_Data* objdata = self->data;
    
    if ((vec3Distance(&self->globalPosition, playerPos) < setup->unk1A) 
            && (mainGetBits(setup->unk20) == 0)) {
        STUBBED_PRINTF("Near ");
        return 4;
    }
    return objdata->unk13D_0;
}

// offset: 0x8E4 | func: 8
static s32 CFTreasSharpy_func_8E4(Object* self, f32 arg1, f32 arg2) {
    s32 _pad;
    CFTreasSharpy_Data* objdata = self->data;
    s32 sp24 = 0;
    Object* chuckobj;
    
    chuckobj = objGetNearestTypeTo(OBJTYPE_CFChuckObj, self, &arg1);
    if (chuckobj != NULL) {
        sp24 = CFTreasSharpy_func_118C(self, chuckobj, arg2);
        if (self->curModAnimId != 1) {
            objAnimSet(self, 1, 0.0f, 0);
        } else {
            objGetAnimChange(self, arg2, &objdata->unk2C);
        }
    } else {
        objdata->state = 3;
        CFTreasSharpy_func_1460(self, objdata->unk13C, &objdata->unk18, 2);
        STUBBED_PRINTF(" nx %f ny %f hz %f\n", &objdata->unk18.x, &objdata->unk18.y, &objdata->unk18.z);
    }
    return sp24;
}

// offset: 0x9E8 | func: 9
static s32 CFTreasSharpy_func_9E8(Object* self) {
    CFTreasSharpy_Data* objdata = self->data;
    f32 xDiff;
    f32 zDiff;
    f32 dist;
    f32 distSq;
    f32 chuckObjDist;
    s16 sp46;
    s16 sp44;
    s16 var_a0;
    Object* player;
    s32 mesgArg;
    
    chuckObjDist = 50.0f;
    objdata->unk2C = objdata->unk8 * 0.0002f;
    if ((objdata->chuckObj == NULL) && (self->curModAnimId != 0x302)) {
        objdata->chuckObj = objGetNearestTypeTo(OBJTYPE_CFChuckObj, self, &chuckObjDist);
        if (objdata->chuckObj == NULL) {
            objdata->state = 3;
            CFTreasSharpy_func_1460(self, objdata->unk13C, &objdata->unk18, 2);
            STUBBED_PRINTF(" nx %f ny %f hz %f\n", &objdata->unk18.x, &objdata->unk18.y, &objdata->unk18.z);
            return 1;
        }
    }
    switch (self->curModAnimId) {
    case 0x1:
        player = objGetPlayer();
        xDiff = self->srt.transl.x - player->srt.transl.x;
        zDiff = self->srt.transl.z - player->srt.transl.z;
        distSq = SQ(xDiff) + SQ(zDiff);
        xDiff = objdata->chuckObj->srt.transl.x - player->srt.transl.x;
        zDiff = objdata->chuckObj->srt.transl.z - player->srt.transl.z;
        sp44 = mathAtan2f(xDiff, zDiff) & ((s16)0xFFFF);
        if ((ABS_EXPR((objdata->unk138 - sp44)) < 0x400) && ((SQ(xDiff) + SQ(zDiff)) < distSq)) {
            objdata->chuckObj->srt.yaw = objdata->unk138;
            objAnimSet(self, 0x2B, 0.0f, 0);
        } else {
            sp46 = objdata->unk138 - (sp44 & 0xFFFF);
            CIRCLE_WRAP(sp46);
            if (sp46 < 0) {
                objdata->unk138 += ((s16) objdata->unk8 * 8);
                if (self->curModAnimId != 1) {
                    objAnimSet(self, 1, self->animProgress, 0);
                }
            } else {
                if (self->curModAnimId != 1) {
                    objAnimSet(self, 1, self->animProgress, 0);
                }
                objdata->unk138 -= ((s16) objdata->unk8 * 8);
            }
            dist = vec3DistanceXZ(&self->globalPosition, &objdata->chuckObj->globalPosition);
            xDiff = self->srt.transl.x;
            zDiff = self->srt.transl.z;
            self->srt.transl.x = (mathSinfInterp(objdata->unk138) * dist) + objdata->chuckObj->srt.transl.x;
            self->srt.transl.z = (mathCosfInterp(objdata->unk138) * dist) + objdata->chuckObj->srt.transl.z;
            xDiff = self->srt.transl.x - xDiff;
            zDiff = self->srt.transl.z - zDiff;
            dist = sqrtf(SQ(xDiff) + SQ(zDiff));
            objGetAnimChange(self, dist / 3.0f, &objdata->unk2C);
            var_a0 = mathAtan2f(xDiff, zDiff) + 0x8000;
            if (sp46 > 0) {
                var_a0 = ((var_a0 * sp46) / 0x8000) + ((sp44 * (0x8000 - sp46)) / 0x8000);
            } else {
                sp46 = sp46 + 0xFFFF;
                var_a0 = ((var_a0 * sp46) / 0x8000) + ((sp44 * (0x8000 - sp46)) / 0x8000);
            }
            self->srt.yaw = var_a0;
        }
        break;
    case 0x2B:
        if (self->animProgress > 0.4f) {
            if (objdata->unk24 == -655370.0f) {
                objdata->unk24 = objdata->unkC.y - objdata->chuckObj->srt.transl.y;
            }
            objdata->chuckObj->srt.transl.x = objdata->unkC.x;
            objdata->chuckObj->srt.transl.y = objdata->unkC.y - objdata->unk24;
            objdata->chuckObj->srt.transl.z = objdata->unkC.z;
        } else if (self->animProgress > 0.2f) {
            objdata->unk24 = -655370.0f;
            objdata->chuckObj->srt.transl.x = (objdata->unkC.x + objdata->chuckObj->srt.transl.x) * 0.5f;
            objdata->chuckObj->srt.transl.z = (objdata->unkC.z + objdata->chuckObj->srt.transl.z) * 0.5f;
        }
        if (objdata->unk13B != 0) {
            objAnimSet(self, 0x302, 0.0f, 0);
        }
        break;
    case 0x302:
        if (self->animProgress < .5f) {
            objdata->chuckObj->srt.transl.x = objdata->unkC.x;
            objdata->chuckObj->srt.transl.y = objdata->unkC.y - objdata->unk24;
            objdata->chuckObj->srt.transl.z = objdata->unkC.z;
        } else if (objdata->chuckObj != NULL) {
            mesgArg = (s32) objdata->unk8;
            objSendMesg(objdata->chuckObj, 0x11, objGetPlayer(), (void* ) mesgArg);
            objdata->chuckObj = NULL;
        }
        if (objdata->unk13B != 0) {
            objdata->state = 1;
            objAnimSet(self, 1, 0.0f, 0);
        }
        break;
    }
    return 0;
}

// offset: 0x1040 | func: 10
static void CFTreasSharpy_func_1040(Object* self, UnkCurvesStruct* arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 sp28[2];

    if (arg2 == 2) {
        sp28[0] = 2;
        sp28[1] = 0x15;
    } else {
        sp28[0] = 2;
        sp28[1] = 0x15;
    }
    gDLL_26_Curves->vtbl->func_4288(arg1, self, arg4, sp28, arg3);
    STUBBED_PRINTF(" PathId %i Loc id %i \n", arg3, arg1->unk9C->unk18);
}

// offset: 0x10D4 | func: 11
static s32 CFTreasSharpy_func_10D4(Object* self, UnkCurvesStruct* arg1, f32 arg2) {
    s32 sp24 = 0;

    if ((curves_func_800053B0(&arg1->unk0, arg2) != 0) || (arg1->unk0.unk10 != 0)) {
        sp24 = gDLL_26_Curves->vtbl->func_4704(arg1);
    }
    self->srt.transl.x = arg1->unk0.unk68.x;
    self->srt.transl.y = arg1->unk0.unk68.y;
    self->srt.transl.z = arg1->unk0.unk68.z;
    return sp24;
}

// offset: 0x118C | func: 12
static s32 CFTreasSharpy_func_118C(Object* self, Object* obj, f32 speed) {
    f32 xDir = obj->srt.transl.x - self->srt.transl.x;
    f32 yDir = obj->srt.transl.y - self->srt.transl.y;
    f32 zDir = obj->srt.transl.z - self->srt.transl.z;
    f32 magnitude = SQ(xDir) + SQ(yDir) + SQ(zDir);
    if (!(self->def->flags & OBJDEF_INVISIBLE)) {
        if (self->objhitInfo != NULL) {
            if (obj->objhitInfo != NULL) {
                magnitude -= (SQ(self->objhitInfo->unk52) + SQ(obj->objhitInfo->unk52));
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
static s32 CFTreasSharpy_moveTo(Object* self, f32 targX, f32 targY, f32 targZ, f32 speed) {
    f32 xDir = targX - self->srt.transl.x;
    f32 yDir = targY - self->srt.transl.y;
    f32 zDir = targZ - self->srt.transl.z;
    if ((SQ(xDir) + SQ(yDir) + SQ(zDir)) < 5.0f) {
        return 1;
    }
    guNormalize(&xDir, &yDir, &zDir);
    self->srt.transl.x += (xDir * speed * gUpdateRateF);
    self->srt.transl.y += (yDir * speed * gUpdateRateF);
    self->srt.transl.z += (zDir * speed * gUpdateRateF);
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
static s32 CFTreasSharpy_func_1530(Object* self, UnkCurvesStruct* arg1, f32 arg2, u8 arg3, Vec3f* arg4, f32* arg5) {
    s32 sp2C = 0;

    if (self->unkDC == -1) {
        return 1;
    }
    if (self->unkDC == 0) {
        if (CFTreasSharpy_moveTo(self, arg4->x, arg4->y, arg4->z, arg2) != 0) {
            CFTreasSharpy_func_1040(self, arg1, 2, arg3, 400.0f);
            self->unkDC = 1;
        }
    } else {
        sp2C = CFTreasSharpy_func_10D4(self, arg1, arg2);
        if (sp2C != 0) {
            self->unkDC = -1;
        }
    }
    self->srt.yaw = mathAtan2f(
        self->srt.transl.x - self->prevLocalPosition.x, 
        self->srt.transl.z - self->prevLocalPosition.z) + 0x8000;
    objGetAnimChange(self, arg2, arg5);
    return sp2C;
}
