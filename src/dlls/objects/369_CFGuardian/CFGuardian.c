#include "common.h"
#include "dlls/engine/27.h"
#include "dlls/engine/53_movelib.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
} CFGuardian_Setup;

typedef struct {
    MoveLibData unk0;
    HeadAnimation unk4B8;
    HeadAnimation unk4DC;
    u8 _unk500[0x50C - 0x500];
    Object* unk50C[6];
    u8 _unk524[0x53C - 0x524];
    UnkCurvesStruct unk53C;
    u8 _unk644[0x67C - 0x644];
    f32 unk67C;
    DLL27_Data unk680;
    SRT unk8E0;
    u8 unk8F8;
    u8 _unk8F9[0x908 - 0x8F9];
    s32 unk908;
    s32 unk90C;
    u8 unk910;
    s8 unk911;
    u8 unk912;
} CFGuardian_Data;

// size: 0xC
typedef struct {
    s32 unk0;
    s32 unk4[2];
} UnkCFGuardianStruct;

/*0x0*/ static u16 data_0[] = {
    0x08dd, 0x08de, 0x08ea, 0x0000
};
/*0x8*/ static u32 data_8[] = {
    0x08dd1000, 0x08de1000, 0x08df1000, 0x08e01000, 0x08e11000
};
/*0x1C*/ static Vec3f data_1C[] = {
    VEC3F(0.0f, 0.0f, 0.0f), 
    VEC3F(0.0f, 22.0f, 0.0f), 
    VEC3F(0.0f, 24.0f, 25.0f), 
    VEC3F(0.0f, 30.0f, -25.0f)
};
/*0x4C*/ static f32 data_4C[] = {
    0.0f, 
    23.0f, 
    20.0f, 
    16.0f
};
/*0x5C*/ static UnkCFGuardianStruct data_5C[] = {
    {0x00000000, {0x00000001, 0xffffffff}}, 
    {0x00000001, {0x00000004, 0xffffffff}}, 
    {0x00000002, {0x00000005, 0xffffffff}}, 
    {0x00000003, {0x00000002, 0xffffffff}}, 
    {0x00000004, {0x00000008, 0x0000000c}}, 
    {0x00000005, {0x00000006, 0xffffffff}}, 
    {0x00000006, {0x00000005, 0xffffffff}}, 
    {0x00000007, {0x00000007, 0x00000006}}, 
    {0x00000008, {0x00000007, 0x00000006}}, 
    {0x00000009, {0x00000009, 0x00000006}}, 
    {0x0000000a, {0x00000005, 0xffffffff}}, 
    {0x0000000c, {0x0000000b, 0xffffffff}}, 
    {0x0000000d, {0x0000000a, 0xffffffff}}, 
    {0x00000000, {0x00000008, 0x00000001}}, 
    {0x00000008, {0x00000002, 0x00000008}}, 
    {0x00000003, {0x0000000a, 0x00000004}}, 
    {0x0000000a, {0x00000005, 0x0000000a}}, 
    {0x00000006, {0x0000000b, 0x00000007}}, 
    {0x0000000b, {0x00000008, 0x0000000c}}, 
    {0x00000009, {0x0000000c, 0x0000000a}}, 
    {0xffffffff, {0x0000000c, 0xffffffff}}, 
    {0x0000000d, {0xffffffff, 0x00000000}}
};
/*0x164*/ static u32 _data_unk164 = 0;
/*0x168*/ static UnkCFGuardianStruct data_168[] = {
    {0x00000000, {0x00000012, 0xffffffff}}, 
    {0x0000000e, {0x0000000a, 0x0000000c}}
};
/*0x180*/ static u32 data_180 = 0x0000000d;
/*0x184*/ static u32 data_184 = 0x00000002;
/*0x188*/ static s32 data_188[] = {
    0xffffffff, 0x00000000, 0x0000001a, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0x0000001a, 
    0x0000000e, 0x0000000e, 0x0000001a, 0x0000001a, 0x00000000, 0x00000000, 0xffffffff, 0x0000000a, 
    0x0000000b, 0x0000000c, 0x0000000d, 0x0000000e, 0x00000005
};

int dll_369_func_4D0(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8);
s32 dll_369_func_1D84(Object*, Object**, s16, s16, s16, s32);
static SRT* dll_369_func_1FF0(CurveSetup*, SRT*);
static CurveSetup* dll_369_func_2020(Object*, s32, Vec3f*, s32);
static s32 dll_369_func_2104(Object*, SRT*, f32, f32*);
s32 dll_369_func_2348(Object*, UnkCurvesStruct*, f32, u8, f32*);
s32 dll_369_func_2700(UnkCFGuardianStruct*, s32, s32, s32);
s32 dll_369_func_2790(Object*, UnkFunc_80024108Struct*, u16*);
static void dll_369_func_25AC(Object*, UnkCurvesStruct*, s32, s32, f32);
static s32 dll_369_func_2638(Object*, UnkCurvesStruct*, f32);

// offset: 0x0 | ctor
void dll_369_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_369_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
#ifndef NON_MATCHING
/*0x1DC*/ static u8 data_1DC[] = {0x00, 0x01, 0x06, 0x06};
/*0x1E0*/ static u32 data_1E0[] = {
    0x0005000f, 0x000f0000, 0x00000000
};
/*0x1EC*/ static u8 data_1EC[] = {1, 1, 1, 1};
void dll_369_obj_Setup(Object* self, ObjSetup* setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_obj_Setup.s")
#else
void dll_369_obj_Setup(Object* self, CFGuardian_Setup* setup, s32 reset) {
    CFGuardian_Data* objdata;
    s32 _pad;
    s32 _pad2;
    u8 sp48[] = {0x00, 0x01, 0x06, 0x06}; // data_1DC
    s16 sp3C[] = {0x0005, 0x000f, 0x000f, 0x0000, 0x0000}; // data_1E0
    u8 sp38[4] = {1, 1, 1, 1}; // data_1EC

    objdata = self->data;
    if (objdata != NULL) {
        objInitMesgQueue(self, 4);
        objAddObjectType(self, OBJTYPE_24);
        objdata->unk8F8 = mainGetBits(BIT_4B);
        self->srt.transl.x = setup->base.x;
        self->srt.transl.y = setup->base.y;
        self->srt.transl.z = setup->base.z;
        self->unkDC = 1;
        self->animCallback = dll_369_func_4D0;
        self->srt.yaw = setup->unk18 << 8;
        objdata->unk90C = 0;
        objdata->unk908 = 6;
        objdata->unk912 = 0;
        objdata->unk67C = 0.0f;
        gDLL_27->vtbl->init(&objdata->unk680, 
            DLL27FLAG_2000000, 
            DLL27FLAG_40000 | DLL27FLAG_HAS_TERRAIN_COLLIDER | DLL27FLAG_80 | DLL27FLAG_2 | DLL27FLAG_1, 
            DLL27MODE_1);
        gDLL_27->vtbl->setup_terrain_collider(&objdata->unk680, 4, data_1C, data_4C, sp38);
        gDLL_27->vtbl->setup_hits_collider(&objdata->unk680, 4, data_1C, data_4C, 8);
        objdata->unk0.unk4A9 |= 0x28;
        objdata->unk910 = 1;
        objdata->unk911 = 0;
        if (mainGetBits(BIT_57) != 0) {
            objdata->unk8F8 = 6;
        }
        if (mainGetBits(BIT_4C1) != 0) {
            objdata->unk8F8 = 12;
        }
        func_8002674C(self);
        mainCreateTempDLL(DLL_ID_MOVELIB);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objdata->unk0, -0x1FFF, 0x2800, 3);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func5(&objdata->unk0, 0x12C, 0x78);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func6(&objdata->unk0, 0, sp3C, 3);
    }
}
#endif

/*0x1F0*/ static u32 data_1F0[] = {
    0x00000007, 0x00000008, 0x00000007, 0x00000008
};
/*0x200*/ static u32 data_200[] = {
    0x42340000, 0x42480000, 0x00000000, 0x00000000, 0x42480000, 0x41a00000, 0xc1a00000, 0x42480000, 
    0x00000000, 0x00000000, 0x42480000, 0xc1a00000, 0x41900000, 0x42480000, 0x41900000, 0xc1900000, 
    0x42480000, 0xc1900000
};
/*0x248*/ static u32 data_248[] = {
    0xff000000, 0xff000000, 0xffffff00, 0x00ffffff, 0xffff0000
};

// offset: 0x2EC | func: 1 | export: 1
void dll_369_obj_Control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_obj_Control.s")

// offset: 0x328 | func: 2 | export: 2
void dll_369_obj_Update(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_obj_Update.s")

// offset: 0x348 | func: 3 | export: 3
void dll_369_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_obj_Print.s")

// offset: 0x3CC | func: 4 | export: 4
void dll_369_obj_Free(Object* self, s32 onlySelf);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_obj_Free.s")

// offset: 0x4AC | func: 5 | export: 5
u32 dll_369_obj_GetModelFlags(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_obj_GetModelFlags.s")

// offset: 0x4BC | func: 6 | export: 6
u32 dll_369_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFGuardian_Data);
}

/*0x0*/ static const char str_0[] = " Initalise Guardian State %i ";
/*0x20*/ static const char str_20[] = " GUARDIAN POS : %f %f %f \n";
/*0x3C*/ static const char str_3C[] = " FREEING GUARDIAN ";

// offset: 0x4D0 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_func_4D0.s")

// offset: 0x678 | func: 8
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_func_678.s")
#else
s32 dll_369_func_678(Object* arg0) {
    Object* sp90;
    f32 sp88;
    f32 sp84;
    s32 sp80;
    u32 sp74;
    void* sp70;
    s32 sp6C;
    UnkFunc_80024108Struct sp50;
    Vec3f sp44;
    Object* temp_v0_2;
    Object* temp_v0_4;
    f32 temp_fv0_2;
    f32 temp_fv0_3;
    f32 var_fa0;
    f32 var_fa0_2;
    s32 temp_v0_6;
    s32 temp_v0_7;
    s32 temp_a1_2;
    u8 temp_a1_3;
    CFGuardian_Data* temp_s1;

    sp74 = 0;
    sp70 = NULL;
    sp6C = 1;
    sp88 = 1000.0f;
    sp84 = 1.0f;
    temp_s1 = arg0->data;
    temp_s1->unk912 &= ~0x2;
    diPrintf("Guardian ");
    temp_s1->unk67C = 0.005f;
    sp90 = objGetPlayer();
    switch (temp_s1->unk8F8) {                              /* switch 1 */
    case 0:                                         /* switch 1 */
        if (temp_s1->unk910 == 2) {
            temp_s1->unk8F8 = 1;
            temp_s1->unk910 = 1;
        }
        sp6C = 0;
        break;
    case 1:                                         /* switch 1 */
        if (temp_s1->unk910 == 2) {
            temp_s1->unk910 = 1;
        }
        if (mainGetBits(0x257) != 0) {
            temp_s1->unk8F8 = 2;
            objAnimSet(arg0, 0x1A, 0.0f, 0);
            arg0->unkDC = 0;
            mainSetBits(0x48, 1);
        }
        sp6C = 0;
        break;
    case 2:                                         /* switch 1 */
        if (temp_s1->unk910 == 2) {
            temp_s1->unk910 = 1;
        }
        temp_s1->unk912 |= 2;
        if (dll_369_func_2348(arg0, &temp_s1->unk53C, 0.7f, 0, &temp_s1->unk67C) != 0) {
            temp_s1->unk8F8 = 3;
        }
        break;
    case 3:                                         /* switch 1 */
        if ((temp_s1->unk0.unk498 == 1) && (sp90 == temp_s1->unk0.prevLookat) && (vec3Distance(&arg0->globalPosition, &sp90->globalPosition) < 80.0f)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(2, arg0, -1);
            mainSetBits(0x60, 1);
            temp_s1->unk8F8 = 4;
        }
        break;
    case 4:                                         /* switch 1 */
        if (mainGetBits(0x57) != 0) {
            temp_s1->unk8F8 = 6;
            temp_s1->unk911 = 0;
        } else if (temp_s1->unk910 == 2) {
            temp_s1->unk910 = 1;
            temp_s1->unk911 = (s8) ((s32) (temp_s1->unk911 + 1) % 2);
        }
        break;
    case 6:                                         /* switch 1 */
        diPrintf(" UpWind Lift \0 LANDING \0Guardian Out of WindLIft Boyo !!! ");
        if (temp_s1->unk90C != 0) {
            if (temp_s1->unk90C >= 2) {
                arg0->velocity.x = 0.0f;
                arg0->velocity.z = 0.0f;
                arg0->srt.transl.y += arg0->velocity.y * gUpdateRateF;
                trackGetHeightNearest(arg0, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, &sp84, 0);
                arg0->srt.yaw = (s16) ((s32) (0xC0 << (arg0->srt.yaw + 8)) >> 1);
                arg0->objhitInfo->unk58 &= ~0x400;
                if (sp84 <= 1.0f) {
                    temp_s1->unk90C = 2;
                    arg0->srt.transl.y -= sp84;
                    temp_s1->unk910 = 0;
                    arg0->unkDC = 0;
                    objAnimSet(arg0, 0, 0.0f, 0);
                    dll_369_func_1FF0(dll_369_func_2020(arg0, 0, NULL, 2), &temp_s1->unk8E0);
                    if (arg0->srt.transl.y <= temp_s1->unk8E0.transl.y) {
                        var_fa0 = temp_s1->unk8E0.transl.y - arg0->srt.transl.y;
                    } else {
                        var_fa0 = -(temp_s1->unk8E0.transl.y - arg0->srt.transl.y);
                    }
                    if (var_fa0 < 150.0f) {
                        objAddObjectType(arg0, 0x18);
                        temp_s1->unk8F8 = 7;
                        objAnimSet(arg0, 0x1A, 0.0f, 0);
                    }
                }
                arg0->velocity.y -= 0.12f;
            } else {
                temp_fv0_2 = arg0->velocity.y * 400.0f;
                if (temp_fv0_2 >= 0.0f) {
                    var_fa0_2 = temp_fv0_2;
                } else {
                    var_fa0_2 = -temp_fv0_2;
                }
                arg0->srt.yaw = (s16) (s32) ((f32) arg0->srt.yaw + var_fa0_2);
                temp_s1->unk67C = 0.04f;
                if (mainGetBits(0x8E9) != 0) {
                    objAnimSet(arg0, 0, 0.0f, 0);
                    objAnim_func_80024D74(arg0, 0x32);
                    arg0->velocity.y = 0.0f;
                    objFreeObjectType(arg0, 0x18);
                    arg0->velocity.x = 0.0f;
                    arg0->velocity.z = 0.0f;
                    arg0->velocity.y = -0.001f;
                    temp_s1->unk90C = 2;
                    temp_s1->unk912 &= ~0x1;
                }
            }
            if (temp_s1->unk90C < 2) {
                arg0->srt.transl.x += gUpdateRateF * arg0->velocity.x;
                arg0->srt.transl.z += gUpdateRateF * arg0->velocity.z;
                gDLL_27->vtbl->func_1E8(arg0, &temp_s1->unk680, gUpdateRateF);
                gDLL_27->vtbl->func_5A8(arg0, &temp_s1->unk680);
                gDLL_27->vtbl->func_624(arg0, &temp_s1->unk680, gUpdateRateF);
                if (temp_s1->unk680.hitsTouchBits != 0) {
                    arg0->velocity.x = -arg0->velocity.x * 0.8f;
                    arg0->velocity.z = -arg0->velocity.z * 0.8f;
                }
                sp44.f[0] = arg0->srt.transl.x - arg0->prevLocalPosition.x;
                sp44.f[1] = arg0->srt.transl.y - arg0->prevLocalPosition.y;
                sp44.f[2] = arg0->srt.transl.z - arg0->prevLocalPosition.z;
                temp_fv0_3 = 0.95f * (1.0f / gUpdateRateF);
                sp44.f[0] *= temp_fv0_3;
                sp44.f[1] *= temp_fv0_3;
                sp44.f[2] *= temp_fv0_3;
                arg0->velocity.x += sp44.f[0];
                arg0->velocity.y += sp44.f[1];
                arg0->velocity.z += sp44.f[2];
                arg0->velocity.x *= 0.3f;
                arg0->velocity.y *= 0.3f;
                arg0->velocity.z *= 0.3f;
                diPrintf(" Xvel %f Zvel %f \n\0 Stand \0 Idle Tow \0 Stand \0 Idle Tow \0GD\0 Guardian In Elevatoe \0Guardian Out of WindLIft \0 Set Anim \0 OVeride Set \0 animnum %i \0 Make Sound \0 Set State %i \0 Error Could not find node \0Got Curve \0 PathId %i Loc id %i \n\0 Tangent 1 %f %f %f \n\0 Tangent 2 %f %f %f \n\0t value %f ", 
                    &arg0->velocity.x, &arg0->velocity.z);
            }
        } else if (temp_s1->unk910 == 2) {
            temp_s1->unk910 = 1;
        }
        break;
    case 7:                                         /* switch 1 */
        if (temp_s1->unk910 == 2) {
            temp_s1->unk910 = 1;
        }
        temp_s1->unk912 |= 2;
        if (dll_369_func_2348(arg0, &temp_s1->unk53C, 0.3f, 1, &temp_s1->unk67C) != 0) {
            temp_s1->unk8F8 = 8U;
            objAnim_func_80024D74(arg0, 0x32);
        }
        break;
    case 8:                                         /* switch 1 */
        temp_v0_2 = objGetNearestTypeTo(4, arg0, &sp88);
        if ((temp_v0_2 != NULL) && (sp88 < 300.0f)) {
            ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func1(&temp_s1->unk0, temp_v0_2);
            arg0->unkAF |= 0x10;
        }
        if ((sp88 > 300.0f) && (vec3DistanceXZ(&sp90->globalPosition, &arg0->globalPosition) < 80.0f)) {
            arg0->unkAF &= ~0x10;
            if (!(temp_s1->unk912 & 4) && (data_188[temp_s1->unk8F8] != 0)) {
                ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func9(0xF, &temp_s1->unk8E0);
                temp_s1->unk912 |= 5;
                data_188[temp_s1->unk8F8] = 0;
            }
            if (temp_s1->unk910 == 2) {
                temp_s1->unk910 = 1;
                temp_s1->unk911 = (s8) ((s32) (temp_s1->unk911 + 1) % 2);
            }
        } else {
            if (!(temp_s1->unk912 & 4) && (data_188[temp_s1->unk8F8] != 0xE)) {
                temp_s1->unk910 = 2;
                temp_s1->unk912 |= 5;
                ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func7(0xE, &temp_s1->unk8E0);
                data_188[temp_s1->unk8F8] = 0xE;
            }
        }
        if ((temp_s1->unk912 & 4) && (dll_369_func_2104(arg0, &temp_s1->unk8E0, 0.5f, &temp_s1->unk67C) != 0)) {
            objAnimSet(arg0, 0x1A, 0.0f, 0);
            temp_s1->unk912 &= ~0x5;
        }
        if (mainGetBits(0x43) != 0) {
            temp_s1->unk8F8 = 9;
            temp_s1->unk911 = 0;
        }
        break;
    case 9:                                         /* switch 1 */
        temp_v0_4 = objGetNearestTypeTo(4, arg0, &sp88);
        if ((temp_v0_4 != NULL) && (sp88 < 300.0f)) {
            ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func1(&temp_s1->unk0, temp_v0_4);
        }
        if ((sp88 > 300.0f) && (vec3DistanceXZ(&sp90->globalPosition, &arg0->globalPosition) < 80.0f)) {
            if (!(temp_s1->unk912 & 4) && (data_188[temp_s1->unk8F8] != 0)) {
                ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func9(0xF, &temp_s1->unk8E0);
                temp_s1->unk912 |= 5;
                data_188[temp_s1->unk8F8] = 0;
            }
            if (temp_s1->unk910 == 2) {
                temp_s1->unk910 = 1;
                temp_s1->unk911 = (s8) ((s32) (temp_s1->unk911 + 1) % 2);
            }
        } else {
            if (!(temp_s1->unk912 & 4) && (data_188[temp_s1->unk8F8] != 0xE)) {
                temp_s1->unk910 = 2;
                temp_s1->unk912 |= 5;
                ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func7(0xE, &temp_s1->unk8E0);
                data_188[temp_s1->unk8F8] = 0xE;
            }
        }
        if ((temp_s1->unk912 & 4) && (dll_369_func_2104(arg0, &temp_s1->unk8E0, 0.5f, &temp_s1->unk67C) != 0)) {
            objAnimSet(arg0, 0x1A, 0.0f, 0);
            temp_s1->unk912 &= ~0x5;
        }
        if (mainGetBits(0x4BE) != 0) {
            temp_s1->unk8F8 = 0xA;
            objAnimSet(arg0, 0x1A, 0.0f, 0);
            arg0->unkDC = 0;
        }
        break;
    case 10:                                        /* switch 1 */
        if (temp_s1->unk910 == 2) {
            temp_s1->unk910 = 1;
        }
        temp_s1->unk912 |= 2;
        if (dll_369_func_2348(arg0, &temp_s1->unk53C, 0.6f, 2, &temp_s1->unk67C) != 0) {
            temp_s1->unk8F8 = 0xB;
        }
        break;
    case 11:                                        /* switch 1 */
        if (temp_s1->unk910 == 2) {
            temp_s1->unk910 = 1;
        }
        arg0->opacity = 0;
        arg0->objhitInfo->unk58 &= ~0x1;
        objDisable(arg0);
        arg0->srt.flags |= 0x4000;
        temp_s1->unk8F8 = 0xF;
        break;
    case 12:                                        /* switch 1 */
        if (temp_s1->unk910 == 2) {
            temp_s1->unk910 = 1;
        }
        if (mainGetBits(0x4B7) != 0) {
            gDLL_2_Camera->vtbl->set_target_object(arg0);
            gDLL_3_Animation->vtbl->start_obj_sequence(0xB, arg0, -1);
            mainSetBits(0x4B7, 0);
        }
        if (mainGetBits(0x49A) != 0) {
            temp_s1->unk8F8 = 0xD;
        }
        break;
    case 13:                                        /* switch 1 */
        if (temp_s1->unk910 == 2) {
            temp_s1->unk910 = 1;
        }
        if (mainGetBits(0x4B7) != 0) {
            gDLL_2_Camera->vtbl->set_target_object(arg0);
            gDLL_3_Animation->vtbl->start_obj_sequence(0xA, arg0, -1);
            mainSetBits(0x4B7, 0);
        }
        if (mainGetBits(0x4AA) != 0) {
            temp_s1->unk8F8 = 0xE;
        }
        break;
    }
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func0(arg0, &temp_s1->unk0);
    while (objRecvMesg(arg0, &sp74, NULL, &sp70) != 0) {
        switch (sp74) {                         /* switch 2; irregular */
        case 15:                                /* switch 2 */
            temp_s1->unk8F8 = 6;
            objAnimSet(arg0, 9, 0.0f, 0);
            objAnim_func_80024D74(arg0, 0xFA);
            temp_s1->unk90C = 1;
            arg0->velocity.z = 0.0f;
            arg0->velocity.y = 0.0f;
            arg0->velocity.x = 0.0f;
            arg0->objhitInfo->unk58 |= 0x400;
            temp_s1->unk910 = 0;
            temp_s1->unk912 |= 1;
            break;
        case 16:                                /* switch 2 */
            objAnimSet(arg0, 0, 0.0f, 0);
            objAnim_func_80024D74(arg0, 0x32);
            arg0->velocity.x = 0.0f;
            arg0->velocity.z = 0.0f;
            arg0->velocity.y = -0.001f;
            temp_s1->unk90C = 2;
            temp_s1->unk912 &= ~0x1;
            break;
        }
    }
    if (arg0->unkAF & 1) {
        joyDisableButtons(0, 0x8000);
        if (gDLL_1_cmdmenu->vtbl->was_this_item_used(0x2E8) != 0) {
            mainSetBits(0x4AB, 1);
        } else if (temp_s1->unk910 == 1) {
            temp_v0_6 = dll_369_func_2700(data_5C, temp_s1->unk8F8, data_180, temp_s1->unk911);
            if (temp_v0_6 != -1) {
                temp_s1->unk910 = 2;
                gDLL_3_Animation->vtbl->start_obj_sequence(temp_v0_6, arg0, -1);
            }
        }
    }
    if (mainGetBits(0x902) != 0) {
        temp_v0_7 = dll_369_func_2700(data_5C, temp_s1->unk8F8, data_180, temp_s1->unk911);
        if (temp_v0_7 != -1) {
            temp_s1->unk910 = 2;
            gDLL_3_Animation->vtbl->start_obj_sequence(temp_v0_7, arg0, -1);
            mainSetBits(0x902, 0);
        }
    }
    temp_a1_2 = data_188[temp_s1->unk8F8];
    if ((temp_a1_2 != -1) && !(temp_s1->unk912 & 1) && (arg0->curModAnimId != temp_a1_2)) {
        objAnimSet(arg0, temp_a1_2, 0.0f, 0);
        objAnim_func_80024D74(arg0, 0x50);
    }
    if (objAnimAdvance(arg0, temp_s1->unk67C, (f32) gUpdateRate, &sp50) != 0) {
        if (temp_s1->unk912 & 1) {
            if ((arg0->curModAnimId != 0x1A) && (arg0->curModAnimId != 9)) {
                temp_s1->unk912 &= ~0x1;
            }
        } else if ((mathRnd(0, 6) == 0) && (sp6C != 0)) {
            sp80 = dll_369_func_2700(data_168, (u8) arg0->curModAnimId, data_184, mathRnd(0, 1));
            if (sp80 != -1) {
                objAnim_func_80024D74(arg0, 0x28);
                objAnimSet(arg0, sp80, 0.0f, 0);
                temp_s1->unk912 |= 1;
            } else {
                objAnimSet(arg0, data_188[temp_s1->unk8F8], 0.0f, 0);
            }
        }
    }
    dll_369_func_2790(arg0, &sp50, data_0);
    if (mathRnd(0, 0x3C) == 0) {
        objExpr_func_80034B54(arg0, &temp_s1->unk4B8, (s16* ) &data_8[mathRnd(0, 4)], 0);
    }
    objExpr_func_80034BC0(arg0, &temp_s1->unk4B8);
    objExprEyeIdle(arg0, &temp_s1->unk4DC);
    dll_369_func_1D84(arg0, temp_s1->unk50C, 0x500, 0, 0, 6);
    temp_a1_3 = temp_s1->unk8F8;
    if (mainGetBits(0x4B) != temp_a1_3) {
        mainSetBits(0x4B, (u32) temp_a1_3);
    }
    mapSaveObject(arg0->setup, arg0->mapID, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z);
    return 0;
}
#endif

// offset: 0x1B8C | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_func_1B8C.s")

// offset: 0x1D84 | func: 10
s32 dll_369_func_1D84(Object* arg0, Object** arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5) {
    s16 spB0[3];
    Vec3f spA4;
    s32 var_s4;
    s32 sp9C = 0;
    f32 temp_fs0;
    f32 temp_fv0;
    SRT sp7C;
    u8 sp68[][3] = { // data_25C
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
    temp_fs0 = arg0->srt.transl.y + 3.0f;
    for (var_s4 = 0; var_s4 < arg5; var_s4++) {
        if (arg1[var_s4] != NULL) {
            if (arg1[var_s4]->unkDC != 0) {
                objFreeObject(arg1[var_s4]);
                arg1[var_s4] = NULL;
            } else {
                spA4.f[0] = arg1[var_s4]->srt.transl.x - arg0->srt.transl.x;
                spA4.f[1] = arg1[var_s4]->srt.transl.y - arg0->srt.transl.y;
                spA4.f[2] = arg1[var_s4]->srt.transl.z - arg0->srt.transl.z;
                spB0[0] = arg2;
                spB0[1] = arg3;
                spB0[2] = arg4;
                mathRotateRPY((SRT* ) &spB0, spA4.f);
                arg1[var_s4]->srt.transl.x = spA4.f[0] + arg0->srt.transl.x;
                arg1[var_s4]->srt.transl.y = spA4.f[1] + arg0->srt.transl.y;
                temp_fv0 = arg1[var_s4]->srt.transl.y;
                if (temp_fv0 < temp_fs0) {
                    arg1[var_s4]->srt.transl.y = (temp_fs0 - temp_fv0) + temp_fs0;
                }
                arg1[var_s4]->srt.transl.z = spA4.f[2] + arg0->srt.transl.z;
                sp7C.roll = sp68[var_s4][0];
                sp7C.pitch = sp68[var_s4][1];
                sp7C.yaw = sp68[var_s4][2];
                gDLL_17_partfx->vtbl->spawn(arg1[var_s4], 0x357, &sp7C, 0, -1, NULL);
                sp9C = 1;
            }
        }
    }

    return sp9C;
}

// offset: 0x1FF0 | func: 11
static SRT* dll_369_func_1FF0(CurveSetup* arg0, SRT* arg1) {
    arg1->transl.x = arg0->pos.x;
    arg1->transl.y = arg0->pos.y;
    arg1->transl.z = arg0->pos.z;
    arg1->yaw = arg0->unk2C << 8;
    return arg1;
}

// offset: 0x2020 | func: 12
static CurveSetup* dll_369_func_2020(Object* arg0, s32 arg1, Vec3f* arg2, s32 arg3) {
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
    temp_v0 = gDLL_26_Curves->vtbl->func_1E4(
        arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, 
        sp2C, ARRAYCOUNT(sp2C), arg1);
    if (temp_v0 >= 0) {
        sp28 = gDLL_26_Curves->vtbl->func_39C(temp_v0);
        if (arg2 != NULL) {
            arg2->x = sp28->pos.x;
            arg2->y = sp28->pos.y;
            arg2->z = sp28->pos.z;
        }
    }
    return sp28;
}

// offset: 0x2104 | func: 13
static s32 dll_369_func_2104(Object* arg0, SRT* arg1, f32 arg2, f32* arg3) {
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    s16 var_v0;
    s32 _pad;

    if (arg1 == NULL) {
        return 0;
    }
    sp4C = arg1->transl.x - arg0->srt.transl.x;
    sp48 = arg1->transl.y - arg0->srt.transl.y;
    sp44 = arg1->transl.z - arg0->srt.transl.z;
    sp40 = sqrtf(SQ(sp4C) + SQ(sp48) + SQ(sp44));
    if (sp40 < (arg2 * 5.0f)) {
        return 1;
    }
    guNormalize(&sp4C, &sp48, &sp44);
    arg0->velocity.x = sp4C * arg2 * gUpdateRateF;
    arg0->velocity.y = sp48 * arg2 * gUpdateRateF;
    arg0->velocity.z = sp44 * arg2 * gUpdateRateF;
    var_v0 = (arg1->yaw - (arg0->srt.yaw & 0xFFFF)) + 0x8000;
    CIRCLE_WRAP(var_v0);
    arg0->srt.yaw += ((((f32) var_v0 + 0.5f) * (arg2 * gUpdateRateF)) / sp40);
    objMove(arg0, arg0->velocity.x, arg0->velocity.y, arg0->velocity.z);
    if (arg0->curModAnimId != 0x1A) {
        objAnimSet(arg0, 0x1A, 0.0f, 0);
    }
    objGetAnimChange(arg0, arg2, arg3);
    return 0;
}

// offset: 0x2348 | func: 14
s32 dll_369_func_2348(Object* arg0, UnkCurvesStruct* arg1, f32 arg2, u8 arg3, f32* arg4) {
    s32 _pad;
    s16 var_v1;
    s32 sp54;
    f32 sp50;
    SRT sp38;

    sp54 = 0;
    sp50 = 0.0f;
    if (arg0->unkDC == -1) {
        return 1;
    }
    if (arg0->unkDC == 0) {
        dll_369_func_1FF0(dll_369_func_2020(arg0, arg3, NULL, 2), &sp38);
        if (dll_369_func_2104(arg0, &sp38, arg2, arg4) != 0) {
            dll_369_func_25AC(arg0, arg1, 2, arg3, 200.0f);
            arg0->unkDC = 1;
        }
    } else {
        sp54 = dll_369_func_2638(arg0, arg1, arg2);
        if (sp54 != 0) {
            arg0->unkDC = -1;
        }
        if (trackGetHeightNearest(arg0, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, &sp50, 0) == 0) {
            arg0->srt.transl.y -= sp50;
        }
    }
    objGetAnimChange(arg0, arg2, arg4);
    if ((arg0->srt.transl.x != arg0->prevLocalPosition.x) || (arg0->srt.transl.z != arg0->prevLocalPosition.z)) {
        var_v1 = (mathAtan2f(arg0->srt.transl.x - arg0->prevLocalPosition.x, arg0->srt.transl.z - arg0->prevLocalPosition.z) + 0x8000);
        var_v1 -= (arg0->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(var_v1);
        arg0->srt.yaw += (var_v1 >> 3);
    }
    if (arg0->curModAnimId != 0x1A) {
        objAnimSet(arg0, 0x1A, 0.0f, 0);
    }
    return sp54;
}

// offset: 0x25AC | func: 15
static void dll_369_func_25AC(Object* arg0, UnkCurvesStruct* arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 sp28[2];

    if (arg2 == 1) {
        sp28[0] = 0;
        sp28[1] = 0;
    } else {
        sp28[0] = 0x19;
        sp28[1] = 0x15;
    }
    gDLL_26_Curves->vtbl->func_4288(arg1, arg0, arg4, sp28, arg3);
}

// offset: 0x2638 | func: 16
static s32 dll_369_func_2638(Object* arg0, UnkCurvesStruct* arg1, f32 arg2) {
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

// offset: 0x26F0 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_func_26F0.s")

// offset: 0x26F8 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_func_26F8.s")

// offset: 0x2700 | func: 19
s32 dll_369_func_2700(UnkCFGuardianStruct* arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 i;

    i = 0;
    while ((i < arg2) && (arg1 != arg0[i].unk0)) {
        i += 1;
    }
    if (i != arg2) {
        return arg0[i].unk4[arg3];
    }
    return -1;
}

// offset: 0x2770 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_func_2770.s")

// offset: 0x2778 | func: 21 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/369_CFGuardian/dll_369_Func_2778.s")

// offset: 0x2790 | func: 22
s32 dll_369_func_2790(Object* arg0, UnkFunc_80024108Struct* arg1, u16* arg2) {
    s32 var_s2;
    u8 temp_t6;
    s32 i;

    var_s2 = 0;
    for (i = 0; i < arg1->unk1B; i++) {
        switch (arg1->unk13[i]) {
        case 0:
            if (arg2 != NULL) {
                gDLL_6_AMSFX->vtbl->Play(arg0, arg2[0], MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            continue;
        case 7:
            if (arg2 != NULL) {
                gDLL_6_AMSFX->vtbl->Play(arg0, arg2[1], MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            continue;
        case 1:
            var_s2 = 1;
            continue;
        case 2:
            var_s2 = 2;
            continue;
        case 3:
            var_s2 = 3;
            continue;
        case 4:
            var_s2 = 4;
            continue;
        case 9:
            gDLL_6_AMSFX->vtbl->Play(arg0, 0x8EB, MAX_VOLUME, NULL, NULL, 0, NULL);
            continue;
        }
    }
    if ((var_s2 != 0) && (arg2 != NULL)) {
        gDLL_6_AMSFX->vtbl->Play(arg0, arg2[2], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    return var_s2;
}
