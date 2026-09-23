#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/26_curves.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/common/foodbag.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/textable.h"
#include "sys/joypad.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "sys/objlib.h"
#include "sys/print.h"
#include "dll.h"
#include "macros.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    u8 unk20;
    s16 unk22;
} CFCloudBaby_Setup;

typedef struct {
    Vec3f unk0;
    Vec3f unkC;
    Vec3f unk18;
    Vec3f unk24;
    f32 unk30;
    f32 unk34;
} CFCloudBaby_Data_4;

typedef struct {
    s32 unk0;
    CFCloudBaby_Data_4 unk4;
    HeadAnimation unk3C;
    HeadAnimation unk60;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    f32 unk90;
    f32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    s32 unkAC;
    u8 unkB0;
    s32 unkB4;
    s16 unkB8;
    s16 unkBA;
    u8 _unkBC[0xFC - 0xBC];
    Object* unkFC;
    f32 unk100;
    f32 unk104;
    f32 unk108;
    UnkCurvesStruct unk10C;
    u8 unk214;
} CFCloudBaby_Data;

/*0x0*/ static s16 data_0[] = {
    0x00ba, 0x00bb, 0x00bc, 0x00bd 
};
/*0x8*/ static s16 data_8[] = {
    0x0000, 0x0005, 0x0000, 0x0007, 
    0x0000, 0x0000, 0x0000, 0x003c, 
    0x0000, 0x003c, 0x0000, 0x0078, 
    0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000
};

static int CFCloudBaby_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8);
static void CFCloudBaby_func_12EC(Object* self, Object* obj, CFCloudBaby_Data* objdata);
static s32 CFCloudBaby_func_15A8(Object* self, f32 targX, f32 targY, f32 targZ, f32 speed);
static s32 CFCloudBaby_func_180C(Object* self, UnkCurvesStruct* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
static s32 CFCloudBaby_func_1B88(Object* self, CurveSetup* arg1, CFCloudBaby_Data_4* arg2, f32* arg3, f32 arg4);
static f32 CFCloudBaby_func_1DB0(CFCloudBaby_Data_4* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, s32 arg4);
static void CFCloudBaby_func_1FA0(Object* self, f32 arg1);
static s32 CFCloudBaby_func_2050(Object* self, CFCloudBaby_Data* objdata);

// offset: 0x0 | ctor
void CFCloudBaby_ctor(void* dll) { }

// offset: 0xC | dtor
void CFCloudBaby_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFCloudBaby_obj_Setup(Object* self, CFCloudBaby_Setup* setup, s32 reset) {
    CFCloudBaby_Data* objdata;

    func_8002674C(self);
    objInitMesgQueue(self, 4);
    self->animCallback = CFCloudBaby_animCallback;
    self->srt.yaw = setup->unk1D << 8;
    objAddObjectType(self, OBJTYPE_CFCloudBaby);
    objAddObjectType(self, OBJTYPE_Baddie);
    objdata = self->data;
    objdata->unk98 = 0;
    objdata->unk9C = 0;
    objdata->unkA0 = 0;
    objdata->unkA4 = 0;
    objdata->unkA8 = 0;
    objdata->unkAC = setup->unk1C;
    objdata->unkB4 = 0;
    objdata->unk0 = 0;
    objdata->unkFC = 0;
    objdata->unkB8 = self->srt.yaw;
    objdata->unk214 = 0;
    if (mainGetBits(setup->unk22) != 0) {
        func_800267A4(self);
        self->srt.flags |= OBJFLAG_INVISIBLE;
        objdata->unk214 &= ~1;
        objDisable(self);
        objFreeObjectType(self, OBJTYPE_CFCloudBaby);
        objFreeObjectType(self, OBJTYPE_Baddie);
    }
}

/*0x0*/ static const char str_0[] = " Goung to nearest cuve ";

// offset: 0x1A0 | func: 1 | export: 1
void CFCloudBaby_obj_Control(Object* self) {
    CFCloudBaby_Data* objdata;
    CFCloudBaby_Setup* setup;
    Object* player;
    Object* sidekick;
    Object* bean;
    f32 magnitude;
    s32 isPlayerClose;
    Object* foodbag;

    setup = (CFCloudBaby_Setup*)self->setup;
    objdata = self->data;
    player = objGetPlayer();
    sidekick = objGetSidekick();
    if (objdata->unk0 != 0) {
        objdata->unk214 |= 1;
        if (objdata->unk0 == 480) {
            CFCloudBaby_func_1FA0(self, 3.0f);
        }
        objdata->unkAC = 0;
        if (objdata->unk0 > 0) {
            objdata->unk0 -= gUpdateRate;
        }
        if (objdata->unk0 <= 0) {
            if ((mainGetBits(BIT_50B) == 0) && !(player->stateFlags & OBJSTATE_IN_SEQ)) {
                if (setup->unk22 != -1) {
                    mainSetBits(setup->unk22, 1);
                }
                STUBBED_PRINTF(" The Birdy End is Nigh ");
                objdata->unk0 = 0;
                func_800267A4(self);
                self->srt.flags |= OBJFLAG_INVISIBLE;
                objdata->unk214 &= ~0x1;
                objDisable(self);
                objFreeObjectType(self, OBJTYPE_CFCloudBaby);
                objFreeObjectType(self, OBJTYPE_Baddie);
                gDLL_1_cmdmenu->vtbl->energy_bar_fadeout();
            }
            self->srt.flags |= OBJFLAG_INVISIBLE;
            return;
        }
        diPrintf("Bird Timer %i ", objdata->unk0);
        if (CFCloudBaby_func_180C(self, &objdata->unk10C, 3.0f, objdata->unk84, objdata->unk8C, objdata->unk88) != 0) {
            self->opacity = 0;
        }
    } else if (objdata->unkAC != 6) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        if ((objdata->unk9C == 0) && (objdata->unk98 != 0) && (mainGetBits(setup->unk1E) != 0)) {
            objdata->unk9C = 1;
            objdata->unkAC = setup->unk20;
        }
        isPlayerClose = vec3Distance(&self->globalPosition, &player->globalPosition) < (f32) setup->unk18;
        if (isPlayerClose || ((objdata->unkAC == 4)) || (objdata->unkAC == 5) || (objdata->unkAC == 0)) {
            switch (objdata->unkAC) {
            case 0:
                objdata->unk98 = 0;
                /* fallthrough */
            case 1:
                if (isPlayerClose) {
                    gDLL_3_Animation->vtbl->start_obj_sequence(objdata->unkAC + 1, self, -1);
                }
                CFCloudBaby_func_2050(self, objdata);
                objdata->unk98 = 1;
                break;
            case 2:
            case 8:
            case 9:
            case 10:
            case 11:
                gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
                objdata->unkAC = 8;
                break;
            case 4:
                if (objdata->unkA4 != 0) {
                    objdata->unkA8 = 0;
                    objdata->unkA4 = (s32) (objAnimAdvance(self, 0.0064f, gUpdateRateF, NULL) == 0);
                } else if ((mathRnd(0, 10) == 1) && (vec3Distance(&self->globalPosition, &player->globalPosition) < (f32) setup->unk1A)) {
                    objAnimSet(self, 0x12, 0.0f, 0);
                    dll_amSfx->Play(self, SOUND_8B, 0x7E, NULL, NULL, 0, NULL);
                    objdata->unkA4 = 1;
                } else {
                    CFCloudBaby_func_12EC(self, player, objdata);
                }
                if ((objdata->unkA4 != 0) && (self->animProgress > 0.5f)) {
                    func_80026128(self, 9, 1, 0);
                } else {
                    func_80026160(self);
                }
                /* fallthrough */
            case 3:
                CFCloudBaby_func_2050(self, objdata);
                break;
            case 7:
                gDLL_1_cmdmenu->vtbl->energy_bar_create(0, 5, TEXTABLE_571, TEXTABLE_572, 5);
                mainIncrementBits(BIT_901);
                gDLL_1_cmdmenu->vtbl->energy_bar_set(5 - mainGetBits(BIT_901));
                self->srt.yaw = objdata->unkB8;
                gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
                objdata->unk0 = 1;
                mainSetBits(setup->unk1E, 1);
                self->unkDC = 0;
                break;
            case 5:
                func_80026160(self);
                self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                if ((vec3Distance(&self->globalPosition, &player->globalPosition) < (f32) setup->unk1A) 
                        && (self->unkAF & ARROW_FLAG_1_Interacted)) {
                    joyDisableButtons(0, A_BUTTON);
                    self->srt.yaw = objdata->unkB8;
                    gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
                    objdata->unk0 = 1;
                    gDLL_1_cmdmenu->vtbl->energy_bar_create(0, 5, TEXTABLE_571, TEXTABLE_572, 5);
                    mainIncrementBits(BIT_901);
                    gDLL_1_cmdmenu->vtbl->energy_bar_set(5 - mainGetBits(BIT_901));
                    objdata->unkAC = 0xC;
                    mainSetBits(setup->unk1E, 1);
                    self->unkDC = 0;
                    break;
                } else if (objdata->unkB4 != 0) {
                    if (((DLL_ISidekick*)sidekick->dll)->vtbl->Func24(sidekick) == 0) {
                        objGetAnimChange(self, 0.5f, &objdata->unk90);
                        if (CFCloudBaby_func_15A8(self, objdata->unk100, objdata->unk104, objdata->unk108, 0.5f) != 0) {
                            objdata->unkB4 = 0;
                            objdata->unkAC = objdata->unkB0;
                        }
                        objAnimAdvance(self, objdata->unk90, (f32) gUpdateRate, NULL);
                    } else {
                        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                        CFCloudBaby_func_12EC(self, sidekick, objdata);
                    }
                    break;
                }
                /* fallthrough */
            case 12:
            default:
                CFCloudBaby_func_12EC(self, player, objdata);
                break;
            }
        } else if (objdata->unkAC >= 8) {
            switch (objdata->unkAC) {
            case 11:
                objdata->unkAC = 8;
                /* fallthrough */
            case 8:
                foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
                // @bug: bean will be uninitialized if the foodbag is null
                if (foodbag != NULL) {
                    bean = ((DLL_IFoodbag*)foodbag->dll)->vtbl->get_nearest_placed_food_of_type(foodbag, self, 
                        FOOD_Red_Bean | FOOD_Brown_Bean | FOOD_Blue_Bean);
                }
                if ((bean != NULL) && (vec3Distance(&self->globalPosition, &bean->globalPosition) < (f32) setup->unk18)) {
                    objdata->unkAC = 9;
                    objdata->unkB8 = objAngleToObjectXZ(self, bean, NULL) + self->srt.yaw;
                    objdata->unk84 = bean->srt.transl.x - self->srt.transl.x;
                    objdata->unk88 = bean->srt.transl.z - self->srt.transl.z;
                    if ((objdata->unk84 != 0.0f) || (objdata->unk88 != 0.0f)) {
                        magnitude = sqrtf(SQ(objdata->unk84) + SQ(objdata->unk88));
                        objdata->unkBA = (s16) ((magnitude / 0.5f) - 18.0f);
                        objdata->unk84 *= (0.5f / magnitude);
                        objdata->unk88 *= (0.5f / magnitude);
                        objAnimSet(self, 9, 0.0f, 0);
                        objGetAnimChange(self, 0.5f, &objdata->unk90);
                        objdata->unkFC = bean;
                        objdata->unk94 = bean->srt.scale;
                    } else {
                        return;
                    }
                }
                break;
            case 9:
                objdata->unkBA -= 1;
                if (objdata->unkBA < 0) {
                    objdata->unkAC = 10;
                    self->srt.yaw = objdata->unkB8;
                } else {
                    self->srt.yaw += (objdata->unkB8 - self->srt.yaw) / 8;
                    self->srt.transl.x += objdata->unk84;
                    self->srt.transl.z += objdata->unk88;
                }
                objAnimAdvance(self, objdata->unk90, (f32) gUpdateRate, NULL);
                break;
            case 10:
                gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
                break;
            default:
                CFCloudBaby_func_12EC(self, player, objdata);
                break;
            }
        } else {
            if (objdata->unk98 != 0) {
                CFCloudBaby_func_12EC(self, player, objdata);
            }
        }

        if (mathRnd(0, 0x1E) == 0) {
            objExpr_func_80034B94(self, &objdata->unk60, data_0[mathRnd(0, 3)]);
        }
        objExpr_func_80034BC0(self, &objdata->unk60);
    }
}

static const char str_1[] = " Distracted ";
static const char str_2[] = "about to do dist check on %d\n";
static const char str_3[] = " of ";
static const char str_4[] = " FLYING AWAY";
static const char str_5[] = " PathId %i Loc id %i \n";
static const char str_6[] = " Error Could not find node ";
static const char str_7[] = " PathId %i Loc id %i \n";
static const char str_8[] = " Tangent 1 %f %f %f \n";
static const char str_9[] = " Tangent 2 %f %f %f \n";
static const char str_10[] = "t value %f ";
static const char str_11[] = " Linking Object ";

// offset: 0xDB8 | func: 2 | export: 2
void CFCloudBaby_obj_Update(Object* self) { }

// offset: 0xDC4 | func: 3 | export: 3
void CFCloudBaby_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xE18 | func: 4 | export: 4
void CFCloudBaby_obj_Free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_CFCloudBaby);
    objFreeObjectType(self, OBJTYPE_Baddie);
}

// offset: 0xE78 | func: 5 | export: 5
u32 CFCloudBaby_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xE88 | func: 6 | export: 6
u32 CFCloudBaby_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFCloudBaby_Data);
}

// offset: 0xE9C | func: 7
static int CFCloudBaby_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    Object* player;
    Object* foodbag;
    f32 xDist;
    f32 zDist;
    CFCloudBaby_Setup* setup;
    s8 sp3B;
    s16 angle;
    CFCloudBaby_Data* objdata;

    setup = (CFCloudBaby_Setup*)self->setup;
    objdata = self->data;
    if (self->seqSlot == 4) {
        return 0;
    }
    animObjData->unk62 = 0;
    player = objGetPlayer();
    xDist = player->srt.transl.x - setup->base.x;
    zDist = player->srt.transl.z - setup->base.z;
    if ((SQ(xDist) + SQ(zDist)) < (f32) SQ(setup->unk18)) {
        sp3B = TRUE;
    } else {
        sp3B = FALSE;
    }
    if (sp3B && (mathRnd(0, 10) == 1)) {
        dll_amSfx->Play(self, SOUND_8C_Baby_CloudRunner, 0x7E, NULL, NULL, 0, NULL);
    }
    switch (objdata->unkAC) {
    case 10:
    case 11:
        if ((animObjData->lastMessage == 1) && (objdata->unkFC != NULL)) {
            foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->destroy_placed_food(foodbag, objdata->unkFC);
            objFreeObject(objdata->unkFC);
            objdata->unkFC = NULL;
        }
        if (objdata->unkFC != NULL) {
            objdata->unk94 *= 0.995f;
            objdata->unkFC->srt.scale = objdata->unk94;
        }
        objdata->unkAC = 11;
        if ((vec3Distance(&self->globalPosition, &player->globalPosition) < setup->unk1A) 
                && (self->unkAF & ARROW_FLAG_1_Interacted)) {
            if (objdata->unkFC != NULL) {
                foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
                ((DLL_IFoodbag*)foodbag->dll)->vtbl->destroy_placed_food(foodbag, objdata->unkFC);
                objFreeObject(objdata->unkFC);
                objdata->unkFC = NULL;
            }
            objdata->unkAC = 7;
            return 4;
        }
    default:
        break;
    case 0:
    case 8:
        animObjData->unk7A &= ~2;
        angle = objAngleToObjectXZ(self, player, NULL);
        objExpr_func_80032CF8(self, player, &objdata->unk3C, 0x28);
        self->srt.yaw += angle / 8;
        if (sp3B != 0) {
            animObjData->unk9D |= 4;
        } else {
            animObjData->unk9D = 8;
        }
        break;
    case 5:
        animObjData->unk7A &= ~2;
        angle = objAngleToObjectXZ(self, objGetSidekick(), NULL);
        objExpr_func_80032CF8(self, objGetSidekick(), &objdata->unk3C, 0x28);
        self->srt.yaw += angle / 8;
        break;
    }
    return 0;
}

// offset: 0x12A4 | func: 8 | export: 7
s32 CFCloudBaby_Func_12A4(Object* self) {
    CFCloudBaby_Data* objdata = self->data;
    return (objdata->unk214 & 1) == FALSE;
}

// offset: 0x12BC | func: 9 | export: 8
s32 CFCloudBaby_Func_12BC(Object* self) {
    CFCloudBaby_Data* objdata = self->data;

    if (objdata->unkAC == 6) {
        objdata->unk0 = 1;
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x12EC | func: 10
static void CFCloudBaby_func_12EC(Object* self, Object* obj, CFCloudBaby_Data* objdata) {
    s16 angle;
    HeadAnimation headanim;
    s16 var_v0;

    objExpr_func_80032C0C(self, obj, &headanim, 0x5A);
    angle = objAngleToObjectXZ(self, obj, NULL) >> 3;
    self->srt.yaw += angle;
    if ((angle > -0xC8) && (angle < 0xC8)) {
        if (objdata->unkA8 != 0) {
            objdata->unkA8 = 0;
            objAnimSet(self, 0, 0.0f, 0);
            return;
        }
        objAnimAdvance(self, 0.0064f, gUpdateRateF, NULL);
        return;
    }
    if (objdata->unkA8 == 0) {
        objdata->unkA8 = 1;
        objAnimSet(self, 9, 0.0f, 0);
        return;
    }
    if (angle > 0) {
        var_v0 = angle >> 2;
    } else {
        var_v0 = -angle >> 2;
    }
    objAnimAdvance(self, (f32) var_v0 / 10000.0f, gUpdateRateF, NULL);
}

// offset: 0x1464 | func: 11
static void CFCloudBaby_func_1464(Object* self, UnkCurvesStruct* arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 sp28[2];

    if (arg2 == 1) {
        sp28[0] = 0;
        sp28[1] = 0;
    } else {
        sp28[0] = 0x19;
        sp28[1] = 0x15;
    }
    gDLL_26_Curves->vtbl->func_4288(arg1, self, arg4, sp28, arg3);
}

// offset: 0x14F0 | func: 12
static s32 CFCloudBaby_func_14F0(Object* self, UnkCurvesStruct* arg1, f32 arg2) {
    s32 sp24;

    sp24 = 0;
    if ((curves_func_800053B0(&arg1->unk0, arg2) != 0) || (arg1->unk0.unk10 != 0)) {
        sp24 = gDLL_26_Curves->vtbl->func_4704(arg1);
    }
    self->srt.transl.x = arg1->unk0.unk68.x;
    self->srt.transl.y = arg1->unk0.unk68.y;
    self->srt.transl.z = arg1->unk0.unk68.z;
    return sp24;
}

// offset: 0x15A8 | func: 13
static s32 CFCloudBaby_func_15A8(Object* self, f32 targX, f32 targY, f32 targZ, f32 speed) {
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 dist;

    dirX = targX - self->srt.transl.x;
    dirY = targY - self->srt.transl.y;
    dirZ = targZ - self->srt.transl.z;
    dist = SQ(dirX) + SQ(dirY) + SQ(dirZ);
    if (dist < 15.0f) {
        self->srt.transl.x = targX;
        self->srt.transl.y = targY;
        self->srt.transl.z = targZ;
        return 1;
    }
    if (dist < 20.0f) {
        speed = dist * (1.0f / 64.0f);
    }
    guNormalize(&dirX, &dirY, &dirZ);
    self->srt.transl.x += dirX * speed * gUpdateRateF;
    self->srt.transl.y += dirY * speed * gUpdateRateF;
    self->srt.transl.z += dirZ * speed * gUpdateRateF;
    if (self->curModAnimId != 9) {
        objAnimSet(self, 9, 0.0f, 0);
    }
    return 0;
}

// offset: 0x1730 | func: 14
static CurveSetup* CFCloudBaby_func_1730(Object* self, s32 arg1, Vec3f* arg2, s32 arg3) {
    s32 temp_v0;
    s32 sp2C[2];
    CurveSetup* sp28;

    sp28 = NULL;
    if (arg3 == 1) {
        sp2C[0] = 0;
        sp2C[1] = 0;
    } else {
        sp2C[0] = 0x19;
        sp2C[1] = 0x15;
    }
    temp_v0 = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, 
        sp2C, ARRAYCOUNT(sp2C), arg1);
    if (temp_v0 >= 0) {
        sp28 = gDLL_26_Curves->vtbl->func_39C(temp_v0);
        arg2->x = sp28->pos.x;
        arg2->y = sp28->pos.y;
        arg2->z = sp28->pos.z;
    }
    return sp28;
}

// offset: 0x180C | func: 15
static s32 CFCloudBaby_func_180C(Object* self, UnkCurvesStruct* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    s16 sp46;
    s32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    CFCloudBaby_Data* objdata;
    f32 sp2C;
    f32 var_fv0;

    sp40 = 0;
    objdata = self->data;
    if (self->unkDC == -1) {
        return 1;
    }
    sp46 = self->srt.yaw;
    sp34 = self->srt.transl.y;
    sp3C = self->srt.transl.x;
    sp38 = self->srt.transl.z;
    if (self->unkDC == 0) {
        if (CFCloudBaby_func_1B88(self, NULL, &objdata->unk4, &objdata->unk4.unk30, arg2) != 0) {
            CFCloudBaby_func_1464(self, arg1, 0, 0, 200.0f);
            self->unkDC = 1;
        }
    } else {
        sp40 = CFCloudBaby_func_14F0(self, arg1, arg2);
    }
    sp3C = self->srt.transl.x - sp3C;
    sp38 = self->srt.transl.z - sp38;
    sp34 = self->srt.transl.y - sp34;
    sp2C = sqrtf(SQ(sp3C) + SQ(sp38));
    self->srt.yaw = mathAtan2f(sp3C, sp38) + 0x8000;
    self->srt.pitch = mathAtan2f(sp34, sp2C);
    self->srt.roll = (self->srt.roll + ((self->srt.yaw - sp46) * 0x10)) / 2;
    if (self->srt.roll > 0x2000) {
        self->srt.roll = 0x3000;
    }
    if (self->srt.roll < -0x2000) {
        self->srt.roll = -0x3000;
    }
    if (sp40 != 0) {
        self->unkDC = -1;
    }
    if ((self->curModAnimId != 5) && (self->curModAnimId != 13)) {
        objAnimSet(self, 13, self->animProgress, 0);
    }
    if ((self->curModAnimId == 5) && (sp34 > 0.01f)) {
        objAnimSet(self, 13, self->animProgress, 0);
    }
    if ((self->curModAnimId == 13) && (sp34 < 0.0f)) {
        objAnimSet(self, 5, self->animProgress, 0);
    }
    var_fv0 = (sp34 * 0.01f) + 0.07f;
    if (self->curModAnimId == 13) {
        var_fv0 *= 0.5f;
    }
    if (var_fv0 < 0) {
        var_fv0 = 0.0f;
    }
    if (var_fv0 > 0.5f) {
        var_fv0 = 0.5f;
    }
    objAnimAdvance(self, var_fv0, (f32) gUpdateRate, NULL);
    return sp40;
}

// offset: 0x1B88 | func: 16
static s32 CFCloudBaby_func_1B88(Object* self, CurveSetup* arg1, CFCloudBaby_Data_4* arg2, f32* arg3, f32 arg4) {
    f32 sp48[4];
    s16 sp40[3];
    s32 sp3C;

    sp3C = 0;
    if (arg1 != NULL) {
        arg2->unk18.f[1] = 0.0f;
        arg2->unk18.f[2] = 0.0f;
        arg2->unk24.f[1] = 0.0f;
        arg2->unk24.f[2] = 0.0f;
        arg2->unk18.f[0] = -200.0f;
        arg2->unk24.f[0] = -200.0f;
        mathRotateYPR(&self->srt, &arg2->unk18);
        sp40[2] = 0;
        sp40[1] = arg1->unk2D;
        sp40[0] = arg1->unk2C;
        mathRotateYPR((SRT* )&sp40, &arg2->unk24);
        *arg3 = 0.0f;
        arg2->unk34 = CFCloudBaby_func_1DB0(arg2, &arg2->unk18, &arg2->unkC, &arg2->unk24, 0xA);
    } else {
        *arg3 += (arg4 * (f32) gUpdateRate) / arg2->unk34;
        if (*arg3 >= 1.0f) {
            sp3C = 1;
            *arg3 = 1.0f;
        }
    }
    sp48[0] = arg2->unk0.f[0];
    sp48[1] = arg2->unkC.f[0];
    sp48[2] = arg2->unk18.f[0];
    sp48[3] = arg2->unk24.f[0];
    self->srt.transl.x = curvesHermite(sp48, *arg3, NULL);
    sp48[0] = arg2->unk0.f[1];
    sp48[1] = arg2->unkC.f[1];
    sp48[2] = arg2->unk18.f[1];
    sp48[3] = arg2->unk24.f[1];
    self->srt.transl.y = curvesHermite(sp48, *arg3, NULL);
    sp48[0] = arg2->unk0.f[2];
    sp48[1] = arg2->unkC.f[2];
    sp48[2] = arg2->unk18.f[2];
    sp48[3] = arg2->unk24.f[2];
    self->srt.transl.z = curvesHermite(sp48, *arg3, NULL);
    return sp3C;
}

// offset: 0x1DB0 | func: 17
static f32 CFCloudBaby_func_1DB0(CFCloudBaby_Data_4* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, s32 arg4) {
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fs2;
    f32 temp_fv0;
    f32 spA4;
    f32 spA0;
    f32 temp_fv0_2;
    f32 temp_fv0_3;
    f32 temp_fv1;
    f32 var_fs2;
    f32 var_fs5;
    f32 sp7C[4];
    s32 var_s1;

    var_fs2 = arg0->unk0.x;
    spA4 = arg0->unk0.y;
    spA0 = arg0->unk0.z;

    var_fs5 = 0.0f;

    for (var_s1 = 1; var_s1 < arg4 + 1; var_s1++) {
        temp_fs0 = (f32) var_s1 / (f32) arg4;

        sp7C[0] = arg0->unk0.x;
        sp7C[1] = arg2->x;
        sp7C[2] = arg1->x;
        sp7C[3] = arg3->x;
        temp_fv0 = curvesHermite(sp7C, temp_fs0, NULL);
        temp_fs1 = temp_fv0 - var_fs2;

        sp7C[0] = arg0->unk0.f[1];
        sp7C[1] = arg2->f[1];
        sp7C[2] = arg1->f[1];
        sp7C[3] = arg3->f[1];
        temp_fv0_2 = curvesHermite(sp7C, temp_fs0, NULL);
        temp_fs2 = temp_fv0_2 - spA4;

        sp7C[0] = arg0->unk0.f[2];
        sp7C[1] = arg2->f[2];
        sp7C[2] = arg1->f[2];
        sp7C[3] = arg3->f[2];
        temp_fv0_3 = curvesHermite(sp7C, temp_fs0, NULL);
        temp_fv1 = temp_fv0_3 - spA0;

        var_fs5 += sqrtf(SQ(temp_fs1) + SQ(temp_fs2) + SQ(temp_fv1));
        var_fs2 = temp_fv0;
        spA4 = temp_fv0_2;
        spA0 = temp_fv0_3;
    }

    return var_fs5;
}

// offset: 0x1FA0 | func: 18
static void CFCloudBaby_func_1FA0(Object* self, f32 arg1) {
    CFCloudBaby_Data* objdata = self->data;
    
    bcopy(&self->srt.transl, &objdata->unk4, sizeof(Vec3f));
    self->unkDC = 0;
    CFCloudBaby_func_1B88(self, 
        CFCloudBaby_func_1730(self, 0, &objdata->unk4.unkC, 0), 
        &objdata->unk4, 
        &objdata->unk4.unk30, 
        arg1);
}

// offset: 0x2050 | func: 19
static s32 CFCloudBaby_func_2050(Object* self, CFCloudBaby_Data* objdata) {
    Object* sidekick = objGetSidekick();
    
    if ((sidekick != NULL) && (((DLL_ISidekick*)sidekick->dll)->vtbl->Func24(sidekick) != 0)) {
        objdata->unk100 = self->srt.transl.x;
        objdata->unk104 = self->srt.transl.y;
        objdata->unk108 = self->srt.transl.z;
        objdata->unkB0 = (u8) objdata->unkAC;
        objdata->unkAC = 5;
        objdata->unkB4 = 1;
        self->srt.yaw = -0x3300;
        if (objdata->unk98 != 0) {
            gDLL_3_Animation->vtbl->start_obj_sequence(5, self, -1);
        }
        return 1;
    }
    return 0;
}
