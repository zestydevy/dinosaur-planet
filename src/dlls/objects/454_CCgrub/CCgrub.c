#include "dlls/objects/210_player.h"
#include "dlls/objects/common/foodbag.h"
#include "dlls/objects/315_sidefoodbag.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/setups/121_CCgrub.h"
#include "sys/curves.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "dll.h"
#include "macros.h"

typedef struct {
    UnkCurvesStruct curves;
    u8 unk108;
    u8 state;
    u8 unk10A;
    f32 unk10C;
    f32 animSpeed;
    f32 unk114;
    f32 unk118;
    f32 unk11C;
    UnkFunc_80024108Struct unk120;
    s16 yaw;
    s16 unk13E;
    f32 unk140;
} CCgrub_Data;

typedef enum {
    CCgrub_STATE_0,
    CCgrub_STATE_1,
    CCgrub_STATE_2,
    CCgrub_STATE_3,
    CCgrub_STATE_4,
    CCgrub_STATE_5,
    CCgrub_STATE_6,
    CCgrub_STATE_7,
    CCgrub_STATE_8,
    CCgrub_STATE_9,
    CCgrub_STATE_10
} CCgrub_States;

static void CCgrub_func_AB0(Object* self, CCgrub_Data* objdata, f32 a2);
static void CCgrub_func_BE8(Object* self, CCgrub_Data* objdata, f32 a2, f32 a3);
static void CCgrub_func_CEC(Object* self, Vec3f* a1, f32 a2, f32* a3);
static void CCgrub_func_DC4(Object* self, CCgrub_Data* objdata);
static void CCgrub_func_104C(Object* self, CCgrub_Data* objdata);

// offset: 0x0 | ctor
void CCgrub_ctor(void* dll) { }

// offset: 0xC | dtor
void CCgrub_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCgrub_obj_Setup(Object* self, CCgrub_Setup* setup, s32 reset) {
    CCgrub_Data* objData;
    CurveSetup* curveSetupA;
    CurveSetup* curveSetupB;

    if (setup->base.uID == 0) {
        return;
    }

    if (setup->unk19 < setup->unk18) {
        setup->unk19 = setup->unk18;
    }

    objData = self->data;
    curveSetupA = gDLL_26_Curves->vtbl->func_39C(gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, NULL, -1, -1));
    curveSetupB = gDLL_26_Curves->vtbl->func_39C(gDLL_26_Curves->vtbl->func_438(curveSetupA, -1));
    gDLL_25->vtbl->func_1D30(&objData->curves, curveSetupA, curveSetupB, gDLL_26_Curves->vtbl->func_39C(gDLL_26_Curves->vtbl->func_438(curveSetupB, -1)));
    CCgrub_func_AB0(self, objData, 0.2f);
    curves_func_800053B0(&objData->curves.unk0, 0.1f);

    self->srt.transl.x = curveSetupA->pos.x;
    self->srt.transl.y = curveSetupA->pos.y;
    self->srt.transl.z = curveSetupA->pos.z;

    self->shadow->flags |= (OBJ_SHADOW_FLAG_20000 | OBJ_SHADOW_FLAG_USE_OBJ_YAW);

    objInitMesgQueue(self, 1);
}

// offset: 0x214 | func: 1 | export: 1
void CCgrub_obj_Control(Object* self) {
    Object* player;
    CCgrub_Setup* objSetup;
    Object* krystal;
    s32 sp60;
    u32 outMesgID;
    f32 var_ft5;
    Object* foodbag;
    f32 var_fv0;
    s32 angleAbs;
    s16 angle;
    f32 delta[2];
    CCgrub_Data* objData;

    sp60 = -1;
    objData = self->data;
    objSetup = (CCgrub_Setup*)self->setup;

    while (objRecvMesg(self, &outMesgID, NULL, NULL)) {
        STUBBED_PRINTF("msg %d\n", outMesgID);

        if (outMesgID == 0x7000B) {
            STUBBED_PRINTF("grub has been collected\n");

            player = objGetPlayer();
            foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 16);

            if (((DLL_IFoodbag*)foodbag->dll)->vtbl->is_obtained(foodbag)) {
                STUBBED_PRINTF("adding grub to foodbag\n");

                ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, SIDEFOOD_Blue_Grubs);
            } else {
                STUBBED_PRINTF("increasing grub bit\n");

                //Show an item collection pop-up
                gDLL_1_cmdmenu->vtbl->info_show(
                    BIT_CloudRunner_Grubs, 
                    300, 
                    mainIncrementBits(BIT_CloudRunner_Grubs)
                );
            }

            objData->state = CCgrub_STATE_10;
        }
    }
    
    if (func_80025F40(self, NULL, NULL, NULL) && (objData->state != CCgrub_STATE_0)) {
        objAnimSet(self, 0, 0.0f, 0);
        objData->animSpeed = 0.005f;
        objData->state = CCgrub_STATE_0;
    }

    if ((objData->curves.unk0.unk80 == FALSE) && objData->curves.unk0.unk10) {
        sp60 = gDLL_26_Curves->vtbl->func_438(objData->curves.unkA4, -1);
    } else if (objData->curves.unk0.unk80 && (objData->curves.unk0.unk10 == FALSE)) {
        sp60 = gDLL_26_Curves->vtbl->func_590(objData->curves.unkA4, -1);
    }

    if (sp60 >= 0) {
        gDLL_25->vtbl->func_21F4(&objData->curves, gDLL_26_Curves->vtbl->func_39C(sp60));
    }

    switch (objData->state) {
    case CCgrub_STATE_0:
    case CCgrub_STATE_8:
    case CCgrub_STATE_9:
    case CCgrub_STATE_10:
        break;
    case CCgrub_STATE_1:
        CCgrub_func_DC4(self, objData);
        break;
    case CCgrub_STATE_2:
        if (self->animProgress >= 0.9f) {
            objData->unk10C = mathRnd(objSetup->unk18, objSetup->unk19) / 100.0f;
            objData->animSpeed = 0.02f;
            objAnimSet(self, 5, 0.0f, 0);
            objData->state = CCgrub_STATE_4;
        }
        break;
    case CCgrub_STATE_3:
        if (self->animProgress > 0.05f) {
            if (self->animProgress >= 0.95f) {
                CCgrub_func_AB0(self, objData, objSetup->unk19 / 50.0f);
                CCgrub_func_DC4(self, objData);
            } else if (self->animProgress > 0.8f) {
                self->srt.yaw = objData->yaw;
            } else if (self->animProgress > 0.5f) {
                angle = (objData->yaw & 0xFFFF) - objData->unk13E;
                PRAGMA_IGNORE_PUSH("-Wtype-limits")
                CIRCLE_WRAP(angle);
                angleAbs = angle >= 0 ? angle : -angle;
                if (angleAbs < M_90_DEGREES) {
                    objData->yaw += M_180_DEGREES;
                    if (objData->curves.unk0.unk80 != 0) {
                        var_fv0 = 10.0f;
                    } else {
                        var_fv0 = -10.0f;
                    }

                    curves_func_800053B0(&objData->curves.unk0, var_fv0);
                    angle = (objData->yaw) - (objData->unk13E & 0xFFFF);
                    CIRCLE_WRAP(angle);
                }
                var_fv0 = (self->animProgress - 0.5f) / 0.3f;
                self->srt.yaw = objData->unk13E + (var_fv0 * angle);
            } else if (self->animProgress > 0.35f) {
                self->srt.yaw = objData->unk13E;
                if (objData->unk10A == 0) {
                    objData->unk10A = 1;
                }
            } else {
                if (objData->unk13E == objData->yaw) {
                    krystal = objGetPlayer();
                    delta[0] = krystal->srt.transl.x - self->srt.transl.x;
                    delta[1] = krystal->srt.transl.z - self->srt.transl.z;
                    objData->unk13E = mathAtan2f(-delta[0], -delta[1]);
                }
                angle = objData->yaw - (objData->unk13E & 0xFFFF);
                CIRCLE_WRAP(angle);
                PRAGMA_IGNORE_POP()
                var_fv0 = (self->animProgress - 0.05f) / 0.3f;
                self->srt.yaw = objData->yaw - (var_fv0 * angle);
            }
        }
        break;
    case CCgrub_STATE_4:
        if (self->animProgress > 0.9f) {
            CCgrub_func_AB0(self, objData, objData->unk10C);
            CCgrub_func_DC4(self, objData);
        }
        break;
    case CCgrub_STATE_5:
        if (self->animProgress > 0.8f) {

        }
        break;
    case CCgrub_STATE_6:
        if (self->animProgress > 0.95f) {
            objData->state = CCgrub_STATE_7;
            objData->unk10C = 0.5f;
            objData->unk11C = 0.0f;
            CCgrub_func_104C(self, objData);
        }
        break;
    case CCgrub_STATE_7:
        CCgrub_func_104C(self, objData);
        break;
    }

    objAnimAdvance(self, objData->animSpeed, gUpdateRateF, &objData->unk120);

    objData->unk120.unk0[2] = -(objData->unk120.unk0[2] / gUpdateRateF);

    if (objData->state != CCgrub_STATE_6) {
        CCgrub_func_BE8(self, objData, objData->unk10C, 1.0f);
        CCgrub_func_CEC(self, &objData->curves.unk0.unk68, objData->unk120.unk0[2], &objData->unk114);
    }

    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        // @bug: the gamebit used here is also used for disabling particles!
        objSendMesg(objGetPlayer(), 0x7000A, self, (void* )BIT_5);

        if (self->curModAnimId != 0) {
            objAnimSet(self, 0, 0.0f, 0);
            objData->animSpeed = 0.005f;
        }

        objData->state = CCgrub_STATE_9;
    }
}

// offset: 0xAB0 | func: 2
static void CCgrub_func_AB0(Object* self, CCgrub_Data* objdata, f32 a2) {
    if (a2 < 1.0f) {
        objAnimSet(self, 1, 0.0f, 0);
    } else {
        objAnimSet(self, 2, 0.0f, 0);
    }

    objdata->state = CCgrub_STATE_1;
    objdata->unk10C = a2;
    objdata->unk11C = 0.0f;
    objdata->unk140 = 0.0f;
}

// offset: 0xB54 | func: 3 | export: 2
void CCgrub_obj_Update(Object* self) { }

// offset: 0xB60 | func: 4 | export: 3
void CCgrub_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xBB4 | func: 5 | export: 4
void CCgrub_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0xBC4 | func: 6 | export: 5
u32 CCgrub_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xBD4 | func: 7 | export: 6
u32 CCgrub_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CCgrub_Data);
}

// offset: 0xBE8 | func: 8
static void CCgrub_func_BE8(Object* self, CCgrub_Data* objdata, f32 a2, f32 a3) {
    f32 deltaSq;
    f32 var_fs0;
    f32 dx;
    f32 dz;
    s32 i;

    deltaSq = SQ(a2 * gUpdateRateF);
    dx = objdata->curves.unk0.unk68.x - self->srt.transl.x;
    dz = objdata->curves.unk0.unk68.z - self->srt.transl.z;

    var_fs0 = objdata->curves.unk0.unk80 != 0 ? -a3 : a3;
    
    for (i = 0; i < 5; i++) {
        if ((deltaSq < (SQ(dx) + SQ(dz)))) {
            break;
        }

        curves_func_800053B0(&objdata->curves.unk0, var_fs0);

        dx = objdata->curves.unk0.unk68.x - self->srt.transl.x;
        dz = objdata->curves.unk0.unk68.z - self->srt.transl.z;
    }
}

// offset: 0xCEC | func: 9
static void CCgrub_func_CEC(Object* self, Vec3f* target, f32 speed, f32* oUnitVector2D) {
    f32 dx;
    f32 dz;
    f32 distance;

    dx = target->x - self->srt.transl.x;
    dz = target->z - self->srt.transl.z;
    distance = sqrtf(SQ(dx) + SQ(dz));
    if (distance != 0.0f) {
        oUnitVector2D[0] = dx / distance;
        oUnitVector2D[1] = dz / distance;
        self->srt.transl.x += oUnitVector2D[0] * speed * gUpdateRateF;
        self->srt.transl.z += oUnitVector2D[1] * speed * gUpdateRateF;
    }
}

// offset: 0xDC4 | func: 10
static void CCgrub_func_DC4(Object* self, CCgrub_Data* objdata) {
    CCgrub_Setup* objSetup;
    f32 distanceSq;

    objSetup = (CCgrub_Setup*)self->setup;

    objdata->unk140 += (objdata->unk10C * gUpdateRateF);
    if (objdata->unk140 > 10.0f) {
        objdata->unk140 -= 10.0f;
    }

    objGetAnimChange(self, objdata->unk10C, &objdata->animSpeed);

    self->srt.transl.y = objdata->curves.unk0.unk68.y;
    self->srt.yaw = mathAtan2f(-objdata->unk114, -objdata->unk118);

    objdata->unk11C += gUpdateRateF;
    if (objdata->unk11C >= 300.0f) {
        distanceSq = vec3DistanceXZSquared(&self->globalPosition, &objGetPlayer()->globalPosition);
        if (SQ(objSetup->rangeOuterSq) <= distanceSq) {
            if (SQ(objSetup->rangeInnerSq) <= distanceSq) {
                objAnimSet(self, 3, 0.0f, 0);
                objdata->animSpeed = 0.01f;
                objdata->state = CCgrub_STATE_2;
                return;
            }

            objdata->yaw = self->srt.yaw;
            objdata->unk13E = self->srt.yaw;
            objAnimSet(self, 6, 0.0f, 0);

            if (objdata->unk10C == (objSetup->unk19 / 50.0f)) {
                objdata->animSpeed = 0.015f;
            } else {
                objdata->animSpeed = 0.01f;
            }

            objdata->state = CCgrub_STATE_3;
            objdata->unk10A = 0;
            return;
        }

        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        objAnimSet(self, 4, 0.0f, 0);
        objdata->animSpeed = 0.02f;
        objdata->state = CCgrub_STATE_6;
    }
}

// offset: 0x104C | func: 11
void CCgrub_func_104C(Object* self, CCgrub_Data* objdata) {
    CCgrub_Setup* setup = (CCgrub_Setup*)self->setup;
    
    objdata->unk11C += gUpdateRateF;
    if (objdata->unk11C >= 300.0f) {
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        CCgrub_func_AB0(self, objdata, mathRnd(setup->unk18, setup->unk19) / 100.0f);
        CCgrub_func_DC4(self, objdata);
        return;
    }

    CCgrub_func_BE8(self, objdata, objdata->unk10C, 1.0f);
    CCgrub_func_CEC(self, &objdata->curves.unk0.unk68, objdata->unk10C, &objdata->unk114);

    self->srt.transl.y = objdata->curves.unk0.unk68.y;
    self->srt.yaw = mathAtan2f(-objdata->unk114, -objdata->unk118);
}
