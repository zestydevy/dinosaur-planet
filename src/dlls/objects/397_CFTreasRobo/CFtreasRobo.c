#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/engine/26_curves.h"
#include "dlls/objects/217_GuardClaw.h"
#include "dlls/objects/396_CFSupTreasureCh.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/textable.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/objects.h"
#include "sys/objexpr.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "sys/objhits.h"
#include "sys/rand.h"
#include "dll.h"
#include "macros.h"

/** @file official filename: CFtreasRobo.c */

typedef struct {
    ObjSetup base;
    u8 unk18;
} CFTreasRobo_Setup;

typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6[12];
    s16 unk1E[12];
    s16 unk36;
    s16 unk38;
    s16 unk3A;
} CFTreasRobo_Data_15C;

typedef struct {
    UnkCurvesStruct unk0;
    u8 _unk108[0x13C - 0x108];
    u32 unk13C;
    Vec3f unk140;
    f32 unk14C;
    Object* chest;
    Object* unk154;
    Object* beam;
    CFTreasRobo_Data_15C unk15C;
    u8 _unk198[0x19E - 0x198];
    s16 unk19E;    
    u8 state;
    u8 _unk1A1;
    u8 unk1A2;
    u8 unk1A3;
    u8 unk1A4;
    u8 unk1A5;
    u8 unk1A6;
    u8 unk1A7;
    u8 _unk1A8[0x1AC - 0x1A8];
} CFTreasRobo_Data;

static void CFTreasRobo_func_FAC(Vec3f* arg0, Object* arg1, s16* arg2);
static s32 CFTreasRobo_func_1168(Object* self);
static s32 CFTreasRobo_func_1214(Object* self, f32 x, f32 y, f32 z);
static void CFTreasRobo_func_13DC(Object*, s32, Vec3f*, s32);
static s32 CFTreasRobo_func_14AC(Object*);
static void CFTreasRobo_func_1544(Object*, UnkCurvesStruct*, s32, s32, f32);
static s32 CFTreasRobo_func_15D0(Object*, UnkCurvesStruct*);
static Object* CFTreasRobo_func_168C(Object*, s32);
static void CFTreasRobo_func_16A0(CFTreasRobo_Data_15C* arg0);
static void CFTreasRobo_func_16B0(Object*, CFTreasRobo_Data_15C*, f32, f32, f32, f32);
static s32 CFTreasRobo_func_1844(Object*, CFTreasRobo_Data_15C*);
static void CFTreasRobo_func_1B50(f32, f32, f32, s16*, s16*);
static void CFTreasRobo_func_1BEC(s16, s16, Vec3f*, f32);

// offset: 0x0 | ctor
void CFTreasRobo_ctor(void* dll) { }

// offset: 0xC | dtor
void CFTreasRobo_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFTreasRobo_obj_Setup(Object* self, CFTreasRobo_Setup* setup, s32 reset) {
    CFTreasRobo_Data* objdata;

    STUBBED_PRINTF(" ROBOT  ROBOT  CREATED \n\n\n");

    objdata = self->data;
    objdata->state = 0;
    objdata->chest = NULL;
    objdata->unk19E = 0;
    objdata->unk1A2 = 0;
    objdata->unk1A4 = 0;
    objdata->unk1A6 = 4;
    objdata->unk1A7 = 0;
    objAddObjectType(self, OBJTYPE_CFTreasRobo);
    if (setup->unk18 != 0) {
        objdata->state = 14;
        objdata->unk1A2 = 2;
        objdata->unk1A4 = 2;
    }
    CFTreasRobo_func_16A0(&objdata->unk15C);
    objdata->beam = NULL;
    self->srt.transl.x = setup->base.x;
    self->srt.transl.y = setup->base.y;
    self->srt.transl.z = setup->base.z;
    gDLL_1_cmdmenu->vtbl->energy_bar_create(0, objdata->unk1A6, TEXTABLE_56D, TEXTABLE_570, objdata->unk1A6);
}

// offset: 0x12C | func: 1 | export: 1
void CFTreasRobo_obj_Control(Object* self) {
    CFTreasRobo_Data* objdata = self->data;
    ObjSetup* beamSetup;
    s32 sp68[] = {0x00000001, 0x00000003, 0x00000007, 0x0000000f};
    s32 sp60[] = {0x00000020, 0x00000007};
    Object* baddie;
    Vec3f sp50;
    f32 sp4C;
    f32 var_fv1;
    SeqJoint* sp44;
    s16 var_v1;
    s16 sp38;

    sp4C = 200.0f;
    if (objdata->unk154 == NULL) {
        objdata->unk154 = CFTreasRobo_func_168C(self, 0x147);
    }
    if (objdata->beam == NULL) {
        beamSetup = objAllocSetup(sizeof(ObjSetup), OBJ_RobottrackBeam);
        beamSetup->x = self->srt.transl.x;
        beamSetup->y = self->srt.transl.y;
        beamSetup->z = self->srt.transl.z;
        beamSetup->loadFlags = OBJSETUP_LOAD_MANUAL;
        beamSetup->fadeFlags = OBJSETUP_FADE_MANUAL;
        objdata->beam = objSetupObject(beamSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, self->parent);
    }
    if (objdata->unk154 != NULL) {
        objdata->unk154->srt.transl.x = self->srt.transl.x;
        objdata->unk154->srt.transl.y = self->srt.transl.y;
        objdata->unk154->srt.transl.z = self->srt.transl.z;
    }
    if (objdata->state == 0xE) {
        if (mainGetBits(BIT_334) != 0) {
            STUBBED_PRINTF(" First Robot Activated ");
            objdata->state = 0;
        }
        return;
    }

    if (CFTreasRobo_func_14AC(self) == 3) {
        mainSetBits(BIT_8C9, 1);
    }
    if (CFTreasRobo_func_14AC(self) == 2) {
        mainSetBits(BIT_8CA, 1);
    }
    if (objdata->chest != NULL) {
        baddie = objGetNearestTypeTo(OBJTYPE_Baddie, self, &sp4C);
        if ((baddie != NULL) && (baddie->id == OBJ_GuardClaw)) {
            STUBBED_PRINTF(" Asking to Stand Aside %i ", 0); // guessed location, unknown arg
            ((DLL_217_GuardClaw*)baddie->dll)->vtbl->Func10(baddie, sp4C < 150.0f);
        }
    }
    if (objdata->unk1A3 != 0) {
        if ((objdata->unk15C.unk4 < 0x28) && (CFTreasRobo_func_15D0(self, &objdata->unk0) != 0)) {
            objdata->unk1A3 = 0;
            objdata->unk140.f[0] = objdata->unk0.unk0.unk68.x;
            objdata->unk14C = objdata->unk0.unk0.unk68.y; // ?
            objdata->unk140.f[2] = objdata->unk0.unk0.unk68.z;
        }
    } else {
        switch (objdata->state) {
        case 0:
            STUBBED_PRINTF(" Get Treasure");
            CFTreasRobo_func_1544(self, &objdata->unk0, 0, sp68[objdata->unk1A4], 2000.0f);
            objdata->unk1A3 = 1;
            objdata->state = 1;
            break;
        case 1:
            if (CFTreasRobo_func_1168(self) != 0) {
                CFTreasRobo_func_13DC(self, sp68[objdata->unk1A4 + 1], &objdata->unk140, 0);
                objdata->state = 2;
                objdata->unk1A5 = sp68[objdata->unk1A4 + 1];
            }
            break;
        case 2:
            if (CFTreasRobo_func_1214(self, objdata->unk140.x, objdata->unk140.y + 70.0f, objdata->unk140.z) != 0) {
                objdata->state = 3;
            }
            break;
        case 3:
            CFTreasRobo_func_1544(self, &objdata->unk0, objdata->unk1A7, objdata->unk1A5, 2000.0f);
            objdata->unk1A3 = 1;
            objdata->state = 8;
            break;
        case 8:
            if (objdata->chest != NULL) {
                objdata->state = 9;
                objdata->unk1A2++;
                gDLL_1_cmdmenu->vtbl->energy_bar_set(objdata->unk1A6 - objdata->unk1A2);
                ((DLL_396_CFSupTreasureCh*)objdata->chest->dll)->vtbl->Func9(objdata->chest, self, objdata->unk14C);
                objdata->chest = NULL;
            }
            STUBBED_PRINTF(" Hello ");
            if (objdata->unk1A2 == 2) {
                objdata->unk1A4 = 2;
                STUBBED_PRINTF(" On next Nodes ");
            }
            if (objdata->unk1A2 >= objdata->unk1A6) {
                gDLL_1_cmdmenu->vtbl->energy_bar_free();
                objdata->state = 10;
            }
            CFTreasRobo_func_13DC(self, sp68[objdata->unk1A4], &objdata->unk140, 0);
            objdata->unk0.unk9C->pos.z -= 40.0f; //?????
            break;
        case 11:
            if (CFTreasRobo_func_1214(self, objdata->unk140.x, objdata->unk140.y + 70.0f, objdata->unk140.z) != 0) {
                CFTreasRobo_func_1544(self, &objdata->unk0, 1, objdata->unk1A5, 2000.0f);
                objdata->unk1A3 = 1;
                objdata->state = 1;
                STUBBED_PRINTF(" Tunnel is collapsing ");
                objdata->unk1A4 = 2;
                objdata->unk1A5 = 0xF;
                objdata->unk1A2 = 2;
            }
            break;
        case 9:
            if (CFTreasRobo_func_1214(self, objdata->unk140.x, objdata->unk140.y + 70.0f, objdata->unk140.z) != 0) {
                objdata->state = 0;
            }
            break;
        case 10:
            mainSetBits(BIT_8F7, 0);
            mainSetBits(BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away, 1);
            mainSetBits(BIT_CRF_Galleon_Fade_to_High_Detail, 1);
            mainSetBits(BIT_CRF_Galleon_Fade_to_Low_Detail, 0);
            dll_gplay->set_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, 30, 1);
            dll_gplay->set_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, 9, 1);
            STUBBED_PRINTF(" Picked Up Treasure ");
            STUBBED_PRINTF(" Sorry You Have Failed ");
            if (mapGetLayer() != 0) {
                mapIncrementLayer();
            }
            objdata->state = 15;
            break;
        case 12:
            STUBBED_PRINTF(" TP");
            mainSetBits(BIT_334, 1);
            if (objdata->unk14C < self->srt.transl.y) {
                self->velocity.y -= 0.05f;
                self->srt.transl.y += self->velocity.y;
            } else {
                self->srt.transl.y = objdata->unk14C;
            }
            self->srt.transl.y += 0.2f * mathSinfInterp(objdata->unk19E);
            break;
        case 13:
            if (objdata->unk14C < self->srt.transl.y) {
                self->velocity.y -= 0.05f;
                self->srt.transl.y += self->velocity.y;
            } else {
                self->srt.transl.y = objdata->unk14C;
            }
            self->srt.transl.y += 0.2f * mathSinfInterp(objdata->unk19E);
            break;
        case 15:
            break;
        default:
            break;
        }
    }
    if (objdata->chest != NULL) {
        ((DLL_396_CFSupTreasureCh*)objdata->chest->dll)->vtbl->Func8(objdata->chest, self);
    }
    var_fv1 = sqrtf(SQ(self->srt.transl.z - self->prevLocalPosition.z) + SQ(self->srt.transl.x - self->prevLocalPosition.x)) / 15.0f;
    var_fv1 = CLAMP_EXPR(var_fv1, -1.0f, 1.0f);;
    sp38 = var_fv1 * 5461.0f;
    self->srt.pitch = self->srt.pitch + ((sp38 - self->srt.pitch) >> 3);
    sp50.f[0] = self->srt.transl.x - self->prevLocalPosition.x;
    sp50.f[1] = self->srt.transl.y - self->prevLocalPosition.y;
    sp50.f[2] = self->srt.transl.z - self->prevLocalPosition.z;
    if ((sp50.f[0] != 0.0f) || (sp50.f[1] != 0.0f)) {
        var_v1 = mathAtan2(sp50.f[0], sp50.f[2]) & 0xFFFF;
        var_v1 = var_v1 - self->srt.yaw;
        CIRCLE_WRAP(var_v1);
        self->srt.yaw = self->srt.yaw + (var_v1 >> 2);
    }
    sp44 = objExpr_func_80034804(self, 0);
    if (sp44 != NULL) {
        sp44->pitch = (mathSinfInterp(objdata->unk19E) * (sp38 * 0.0625f)) + -(sp38 * 2);
    }
    if (CFTreasRobo_func_1844(self, &objdata->unk15C) != 0) {
        if (objdata->unk13C != 0) {
            gDLL_6_AMSFX->vtbl->Stop(objdata->unk13C);
            objdata->unk13C = 0;
        }
    }
}

// offset: 0xBD4 | func: 2 | export: 2
void CFTreasRobo_obj_Update(Object* self) {
    CFTreasRobo_Data* objdata;
    Object* hitBy;
    s32 damageType;

    objdata = self->data;
    damageType = func_80025F40(self, &hitBy, NULL, NULL);
    if ((damageType == Damage_Type_Projectile) || (damageType == Damage_Type_Explosion)) {
        CFTreasRobo_func_16B0(self, &objdata->unk15C, 
            hitBy->srt.transl.x - self->srt.transl.x, 
            hitBy->srt.transl.y - self->srt.transl.y, 
            hitBy->srt.transl.z - self->srt.transl.z, 
            15.0f);
        if (objdata->unk13C != 0) {
            gDLL_6_AMSFX->vtbl->Stop(objdata->unk13C);
        }
        gDLL_6_AMSFX->vtbl->Play(self, SOUND_B16, MAX_VOLUME, &objdata->unk13C, NULL, 0, NULL);
    }
}

// offset: 0xD04 | func: 3 | export: 3
void CFTreasRobo_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CFTreasRobo_Data* objdata;
    TextureAnimator* temp_v0_3;
    Vec3f sp54;
    Object* beam;
    SRT sp38;

    objdata = self->data;
    beam = objdata->beam;
    objdata->unk19E += 0x800;
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        if ((objdata->state != 12) && (objdata->state != 13) && ((objdata->unk19E % 5632) == 0)) {
            sp38.roll = 0xFF;
            objGetAttachPointWorldSpace(self, 0, &sp38.transl.x, &sp38.transl.y, &sp38.transl.z, 0);
            sp38.transl.x -= self->srt.transl.x;
            sp38.transl.y -= self->srt.transl.y;
            sp38.transl.z -= self->srt.transl.z;
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_35E, &sp38, 0, -1, NULL);
        }
        if (beam != NULL) {
            if (objdata->chest != NULL) {
                if (objdata->chest != NULL) {
                    sp54.f[0] = self->srt.transl.x - objdata->chest->srt.transl.x;
                    sp54.f[1] = self->srt.transl.y - objdata->chest->srt.transl.y;
                    sp54.f[2] = self->srt.transl.z - objdata->chest->srt.transl.z;
                    CFTreasRobo_func_FAC(&sp54, beam, &beam->srt.pitch);
                }
                temp_v0_3 = objExprGetTexAnimator(beam, 0, 0);
                if (temp_v0_3 != NULL) {
                    temp_v0_3->positionV = (s16) ((temp_v0_3->positionV + 64) % 2048);
                    temp_v0_3->positionU = (s16) ((temp_v0_3->positionU + 8) % 2048);
                }
                beam->opacity = (s32) (mathSinfInterp(objdata->unk19E) * 64.0f) + 191;
                beam->srt.transl.x = self->srt.transl.x;
                beam->srt.transl.y = self->srt.transl.y;
                beam->srt.transl.z = self->srt.transl.z;
                objprintDrawModel(beam, gdl, mtxs, vtxs, pols, 1.0f);
            }
        }
    }
}

// offset: 0xFAC | func: 4
static void CFTreasRobo_func_FAC(Vec3f* arg0, Object* arg1, s16* arg2) {
    arg1->srt.yaw = mathAtan2(arg0->x, arg0->z);
    *arg2 = mathAtan2(sqrtf(SQ(arg0->z) + SQ(arg0->x)), arg0->y);
}

// offset: 0x1068 | func: 5 | export: 4
void CFTreasRobo_obj_Free(Object* self, s32 onlySelf) {
    CFTreasRobo_Data* objdata = self->data;
    
    if (onlySelf == 0) {
        if (objdata->beam != NULL) {
            objFreeObject(objdata->beam);
        }
    }
    if ((objdata->unk154 != NULL) && (onlySelf == 0)) {
        objFreeObject(objdata->unk154);
    }
    objFreeObjectType(self, OBJTYPE_CFTreasRobo);
    gDLL_1_cmdmenu->vtbl->energy_bar_free();
    gDLL_13_Expgfx->vtbl->func5(self);
    STUBBED_PRINTF("ROBOT KILLED ");
}

// offset: 0x1144 | func: 6 | export: 5
u32 CFTreasRobo_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1154 | func: 7 | export: 6
u32 CFTreasRobo_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFTreasRobo_Data);
}

// offset: 0x1168 | func: 8
s32 CFTreasRobo_func_1168(Object* self) {
    f32 dist;
    Object* chest;
    CFTreasRobo_Data* objdata;

    dist = 400.0f;
    objdata = self->data;
    chest = objGetNearestTypeTo(OBJTYPE_CFSupTreasureChest, self, &dist);
    if (chest != NULL) {
        if (CFTreasRobo_func_1214(self, chest->srt.transl.x, chest->srt.transl.y + 70.0f, chest->srt.transl.z) != 0) {
            STUBBED_PRINTF(" GT");
            objdata->chest = chest;
            return TRUE;
        }
    }
    return FALSE;
}

// offset: 0x1214 | func: 9
static s32 CFTreasRobo_func_1214(Object* self, f32 x, f32 y, f32 z) {
    f32 xDir;
    f32 yDir;
    f32 zDir;
    f32 magnitude;
    f32 speed;

    xDir = x - self->srt.transl.x;
    yDir = y - self->srt.transl.y;
    zDir = z - self->srt.transl.z;
    speed = 2.0f;
    magnitude = sqrtf(SQ(xDir) + SQ(yDir) + SQ(zDir));
    if (magnitude < 5.0f) {
        self->srt.transl.x = x;
        self->srt.transl.y = y;
        self->srt.transl.z = z;
        return 1;
    }
    if (magnitude < 4.0f) {
        speed = magnitude * 0.125f;
    }
    if ((xDir != 0.0f) || (yDir != 0.0f) || (zDir != 0.0f)) {
        guNormalize(&xDir, &yDir, &zDir);
    }
    self->srt.transl.x += xDir * speed * gUpdateRateF;
    self->srt.transl.y += yDir * speed * gUpdateRateF;
    self->srt.transl.z += zDir * speed * gUpdateRateF;
    return 0;
}

// offset: 0x13DC | func: 10
void CFTreasRobo_func_13DC(Object* self, s32 arg1, Vec3f* pos, s32 arg3) {
    s32 uid;
    s32 curveTypes[2];
    CurveSetup* curve;

    if (arg3 == 1) {
        curveTypes[0] = 0;
        curveTypes[1] = 0;
    } else {
        curveTypes[0] = 0x19;
        curveTypes[1] = 0x15;
    }
    uid = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, curveTypes, ARRAYCOUNT(curveTypes), arg1);
    if (uid >= 0) {
        curve = gDLL_26_Curves->vtbl->func_39C(uid);
        pos->x = curve->pos.x;
        pos->y = curve->pos.y;
        pos->z = curve->pos.z;
    } else {
        STUBBED_PRINTF(" Error Could not find node ");
    }
    STUBBED_PRINTF(" PathId %i Loc id %i \n", arg1, curve->unk18);
}

// offset: 0x14A4 | func: 11
void CFTreasRobo_func_14A4(void) {

}

// offset: 0x14AC | func: 12
s32 CFTreasRobo_func_14AC(Object* self) {
    s32 _pad;
    s32 curveTypes[1];
    s32 uid;

    curveTypes[0] = 0;
    uid = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, curveTypes, ARRAYCOUNT(curveTypes), -1);
    if (uid >= 0) {
        return gDLL_26_Curves->vtbl->func_39C(uid)->type0.unk31;
    }
    STUBBED_PRINTF(" Error Could not find node ");
    return 0;
}

// offset: 0x1544 | func: 13
void CFTreasRobo_func_1544(Object* arg0, UnkCurvesStruct* arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 sp28[2];

    if (arg2 == 1) {
        sp28[0] = 0;
        sp28[1] = 0;
    } else {
        sp28[0] = 0x19;
        sp28[1] = 0x15;
    }
    gDLL_26_Curves->vtbl->func_4288(arg1, arg0, arg4, sp28, arg3);
    STUBBED_PRINTF(" PathId %i Loc id %i \n", arg3, arg1->unk9C->unk18);
}

// offset: 0x15D0 | func: 14
s32 CFTreasRobo_func_15D0(Object* arg0, UnkCurvesStruct* arg1) {
    s32 sp24;

    sp24 = 0;
    if ((curves_func_800053B0(&arg1->unk0, 1.0f) != 0) || (arg1->unk0.unk10 != 0)) {
        sp24 = gDLL_26_Curves->vtbl->func_4704(arg1);
    }
    arg0->srt.transl.x = arg1->unk0.unk68.x;
    arg0->srt.transl.y = arg1->unk0.unk68.y + 70.0f;
    arg0->srt.transl.z = arg1->unk0.unk68.z;
    return sp24;
}

// offset: 0x168C | func: 15
Object* CFTreasRobo_func_168C(Object* self, s32 arg1) {
    return NULL;
}

// offset: 0x16A0 | func: 16
static void CFTreasRobo_func_16A0(CFTreasRobo_Data_15C* arg0) {
    arg0->unk4 = 0;
    arg0->unk38 = 0;
}

// offset: 0x16B0 | func: 17
static void CFTreasRobo_func_16B0(Object* arg0, CFTreasRobo_Data_15C* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    s32 var_v1;
    SRT sp2C;

    if ((arg2 != 0.0f) || (arg3 != 0.0f) || (arg4 != 0.0f)) {
        guNormalize(&arg2, &arg3, &arg4);
    }
    arg1->unk36 = mathRnd(8, 12);
    CFTreasRobo_func_1B50(arg2, arg3, arg4, &arg1->unk6[0], &arg1->unk1E[0]);
    for (var_v1 = 1; var_v1 < arg1->unk36; var_v1++) {
        arg1->unk6[var_v1] = arg1->unk6[0];
        arg1->unk1E[var_v1] = arg1->unk1E[0];
    }
    arg1->unk4 = 400;
    arg1->unk38 += 0x500;
    arg1->unk3A = 0xFF;
    arg1->unk0 = arg5;
    sp2C.transl.x = arg2 * arg5;
    sp2C.transl.y = arg3 * arg5;
    sp2C.transl.z = arg4 * arg5;
    gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_35B, &sp2C, 0, -1, NULL);
}

// offset: 0x1844 | func: 18
s32 CFTreasRobo_func_1844(Object* arg0, CFTreasRobo_Data_15C* arg1) {
    SRT sp80;
    s16 temp_s2;
    s16 temp_s3;
    s32 var_s1;
    s32 _pad;
    s32 sp70;

    if (arg1->unk4 == 0) {
        return 1;
    }
    if (arg1->unk4 > 255) {
        sp80.roll = 255;
    } else {
        sp80.roll = arg1->unk4;
    }
    // @bug: sp80.pitch is undefined
    if (arg1->unk3A > 0) {
        sp80.yaw = arg1->unk3A;
    } else {
        sp80.yaw = 0;
    }
    arg1->unk3A -= gUpdateRate;
    sp70 = 0;
    while (sp70 < ((s32) gUpdateRate / 2)) {
        for (var_s1 = 0; var_s1 < arg1->unk36; var_s1++) {
            CFTreasRobo_func_1BEC(arg1->unk6[var_s1], arg1->unk1E[var_s1], &sp80.transl, arg1->unk0);
            gDLL_17_partfx->vtbl->spawn(arg0, (var_s1 % 2) + PARTICLE_35C, &sp80, 0, -1, NULL);
            temp_s2 = ((0xFFFF / arg1->unk36) * var_s1) + arg1->unk38;
            temp_s3 = ((mathSinfInterp(temp_s2) * 1792.0f) + mathRnd(0, 0x600)) - 768.0f;
            temp_s2 = ((mathCosfInterp(temp_s2) * 1792.0f) + mathRnd(0, 0x600)) - 768.0f;
            arg1->unk6[var_s1] += temp_s3;
            arg1->unk1E[var_s1] += temp_s2;
        }
        sp70 += 1;
    }
    arg1->unk4 -= gUpdateRate;
    if (arg1->unk4 < 0) {
        arg1->unk4 = 0;
    }
    return 0;
}

// offset: 0x1B50 | func: 19
static void CFTreasRobo_func_1B50(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4) {
    f32 sp24;

    sp24 = sqrtf(SQ(arg0) + SQ(arg2));
    *arg3 = mathAtan2f(arg2, arg0);
    *arg4 = mathAtan2f(sp24, arg1);
}

// offset: 0x1BEC | func: 20
void CFTreasRobo_func_1BEC(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3) {
    arg2->x = mathSinfInterp(arg1) * mathCosfInterp(arg0) * arg3;
    arg2->y = (mathCosfInterp(arg1) * arg3) + 3.0f;
    arg2->z = mathSinfInterp(arg1) * mathSinfInterp(arg0) * arg3;
}
