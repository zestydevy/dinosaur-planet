#include "dlls/objects/437_EWTrobotpatrol.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/rand.h"
#include "dll.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18[0x1A - 0x18];
/*1A*/ u8 unk1A;
/*1B*/ u8 unk1B;
} EWTrobotpatrolB_Setup;

typedef struct {
/*00*/ Object* unk0[12];
/*30*/ CurveSetup* unk30[24];
/*90*/ u8 unk90;
/*91*/ u8 unk91;
/*92*/ u8 unk92;
/*94*/ s16 unk94;
/*96*/ s16 unk96;
/*98*/ f32 unk98;
} EWTrobotpatrolB_Data;

/*0x0*/ static CurveSetup* bss_0[24];
/*0x60*/ static CurveSetup* bss_60;
/*0x64*/ static Object* bss_64;

static void EWTrobotpatrolB_func_30C(Object* base, Object* robot, s32 arg2, u32 uID);
static void EWTrobotpatrolB_func_3A4(Object* self, Object* robo, u32 uID);
static void EWTrobotpatrolB_func_470(Object* self, Object* arg1, u32 uID);
static CurveSetup* EWTrobotpatrolB_func_D7C(Object* self, u32 uID);
static s16 EWTrobotpatrolB_func_60C(s32, CurveSetup*, s32, s32);
static void EWTrobotpatrolB_func_A60(CurveSetup** arg0, u8* arg1, s32 arg2, s32 arg3, u8 arg4);
static void EWTrobotpatrolB_func_BA0(CurveSetup**, CurveSetup**, Object*);
static f32 EWTrobotpatrolB_func_D10(CurveSetup* node, Object* obj);

// offset: 0x0 | ctor
void EWTrobotpatrolB_ctor(void* dll) { }

// offset: 0xC | dtor
void EWTrobotpatrolB_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void EWTrobotpatrolB_obj_Setup(Object* self, ObjSetup* setup, s32 reset) { }

// offset: 0x2C | func: 1 | export: 1
void EWTrobotpatrolB_obj_Control(Object* self) { }

// offset: 0x38 | func: 2 | export: 2
void EWTrobotpatrolB_obj_Update(Object* self) {
    EWTrobotpatrolB_Data* objdata;
    EWTrobotpatrolB_Setup* setup;

    objdata = self->data;
    setup = (EWTrobotpatrolB_Setup*)self->setup;
    if (objdata->unk94 == objdata->unk96) {
        objdata->unk91 = 0;
    } else {
        objdata->unk91 = 1;
        objdata->unk96 = objdata->unk94;
    }
    if ((objdata->unk91 == 0) && (objdata->unk92 != 0)) {
        objdata->unk98 += gUpdateRateF;
        if ((f32) (setup->unk1B << 6) < objdata->unk98) {
            objdata->unk92 = 0U;
            objdata->unk98 = 0.0f;
        }
    } else {
        objdata->unk92 = objdata->unk91;
    }
}

// offset: 0xE0 | func: 3 | export: 3
void EWTrobotpatrolB_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x134 | func: 4 | export: 4
void EWTrobotpatrolB_obj_Free(Object* self, s32 onlySelf) {
    s32 i;
    EWTrobotpatrolB_Data* objdata;

    objdata = self->data;
    if (onlySelf == 0) {
        for (i = 0; i < 12; i++) {
            if (objdata->unk0[i] != NULL) {
                objFreeObject(objdata->unk0[i]);
            }
        }
    }
}

// offset: 0x1AC | func: 5 | export: 5
u32 EWTrobotpatrolB_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1BC | func: 6 | export: 6
u32 EWTrobotpatrolB_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(EWTrobotpatrolB_Data);
}

// offset: 0x1D0 | func: 7
void EWTrobotpatrolB_func_1D0(Object* arg0, CurveSetup* arg1) {
    EWTrobotpatrolB_Data* objdata;
    EWTrobotpatrolB_Setup* setup;
    ObjSetup* roboSetup;
    Object* robo;
    s32 i;

    objdata = arg0->data;
    setup = (EWTrobotpatrolB_Setup*)arg0->setup;
    i = 0;
    while (objdata->unk0[i] != NULL) {
        i++;
    }
    roboSetup = objAllocSetup(sizeof(EWTrobotpatrol_Setup), OBJ_EWTrobotpatrol);
    roboSetup->actExclusions1 = setup->base.actExclusions1;
    roboSetup->loadFlags = OBJSETUP_LOAD_MANUAL;
    roboSetup->fadeDistance = setup->unk1A;
    roboSetup->x = arg1->pos.x;
    roboSetup->y = arg1->pos.y;
    roboSetup->z = arg1->pos.z;
    robo = objSetupObject(roboSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, arg0->parent);
    objdata->unk0[i] = robo;
    ((DLL_437_EWTrobotpatrol*)robo->dll)->vtbl->Func_3AE4(robo, arg0, EWTrobotpatrolB_func_30C);
    EWTrobotpatrolB_func_3A4(arg0, robo, arg1->uID);
}

// offset: 0x30C | func: 8
static void EWTrobotpatrolB_func_30C(Object* base, Object* robot, s32 arg2, u32 uID) {
    EWTrobotpatrolB_Data* objdata = base->data;
    
    if (arg2 == 0) {
        if ((objdata->unk91 != 0) || (objdata->unk92 != 0)) {
            EWTrobotpatrolB_func_470(base, robot, uID);
        } else {
            EWTrobotpatrolB_func_3A4(base, robot, uID);
        }
    } else if (arg2 == 1) {
        objdata->unk94++;
    }
}

// offset: 0x3A4 | func: 9
static void EWTrobotpatrolB_func_3A4(Object* self, Object* robo, u32 uID) {
    CurveSetup* node;
    u8 max;

    max = 0;
    node = EWTrobotpatrolB_func_D7C(self, uID);
    while (node->links[max + 1] != -1 && max != 3) {
        max++;
    }
    ((DLL_437_EWTrobotpatrol*)robo->dll)->vtbl->Func_3AF8(robo, node->links[mathRnd(0, max)]);
}

// offset: 0x470 | func: 10
static void EWTrobotpatrolB_func_470(Object* self, Object* arg1, u32 uID) {
    EWTrobotpatrolB_Data* objdata = self->data;
    CurveSetup* var_s1;
    CurveSetup* sp58[2];
    s16 sp50[4];
    s16 var_a0;
    s32 var_a2;
    s32 i;

    var_s1 = EWTrobotpatrolB_func_D7C(self, uID);
    EWTrobotpatrolB_func_BA0(sp58, objdata->unk30, objGetPlayer());
    bss_0[0] = var_s1;
    bss_64 = self;
    if (var_s1 == sp58[0]) {
        bss_60 = sp58[1];
    } else {
        bss_60 = sp58[0];
    }
    for (i = 0; i < 4; i++) {
        if (var_s1->links[i] != -1) {
            sp50[i] = EWTrobotpatrolB_func_60C(1, var_s1, var_s1->links[i], 1);
        } else {
            sp50[i] = -1;
        }
    }
    var_a0 = sp50[0];
    var_a2 = 0;
    i = 1;
    while (i < 4) {
        if ((sp50[i] != -1) && ((var_a0 == -1) || (sp50[i] < var_a0))) {
            var_a0 = sp50[i];
            var_a2 = i;
        }
        i += 1;
    }
    ((DLL_437_EWTrobotpatrol*)arg1->dll)->vtbl->Func_3AF8(arg1, var_s1->links[var_a2]);
}

// offset: 0x60C | func: 11
static s16 EWTrobotpatrolB_func_60C(s32 arg0, CurveSetup* arg1, s32 arg2, s32 arg3) {
    CurveSetup* temp_v0 = EWTrobotpatrolB_func_D7C(bss_64, arg2);
    s16 sp4C[] = {-1, -1, -1};
    s16 var_v1;
    u8 var_s0;
    u8 var_s1;
    
    var_s0 = 0;
    var_s1 = 0;
    if (temp_v0 == bss_60) {
        while (arg1->uID != (u32)temp_v0->links[var_s0]) {
            var_s0++;
        }
        return temp_v0->type1E.unk34[var_s0];
    }
    if (arg3 == 6) {
        return -1;
    }
    while (var_s0 < arg0) {
        if (temp_v0 == bss_0[var_s0]) {
            return -1;
        }
        var_s0++;
    }
    bss_0[arg0++] = temp_v0;
    var_s0 = 0;
    while (var_s0 < 4) {
        if ((arg1->uID != (u32)temp_v0->links[var_s0]) && (temp_v0->links[var_s0] != -1)) {
            sp4C[var_s1] = EWTrobotpatrolB_func_60C(arg0, temp_v0, temp_v0->links[var_s0], arg3 + 1);
            var_s1++;
        }
        var_s0++;
    }
    var_v1 = sp4C[0];
    var_s0 = 1;
    while (var_s0 < 3) {
        if ((sp4C[var_s0] != -1) && ((var_v1 == -1) || (sp4C[var_s0] < var_v1))) {
            var_v1 = sp4C[var_s0];
        }
        var_s0++;
    }
    if (var_v1 == -1) {
        return -1;
    }
    var_s0 = 0;
    while (arg1->uID != (u32)temp_v0->links[var_s0]) {
        var_s0++;
    }
    return temp_v0->type1E.unk34[var_s0] + var_v1;
}

// offset: 0x88C | func: 12
void EWTrobotpatrolB_func_88C(Object* arg0) {
    EWTrobotpatrolB_Data* objdata;
    s8 i;
    s32 sp54[] = {0x0000001e};
    s32 temp_v0;
    s8 var_v1;
    CurveSetup* temp_a2;

    objdata = arg0->data;    
    for (i = 0; i < 24; i++) {
        objdata->unk30[i] = 0;
    }
    temp_v0 = gDLL_26_Curves->vtbl->func_1E4(arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, sp54, 1, -1);
    objdata->unk30[0] = gDLL_26_Curves->vtbl->func_39C(temp_v0);
    if ((temp_v0 != -1) && (objdata->unk30[0] != NULL)) {
        objdata->unk90 = 1;
        for (i = 0; i < 4; i++) {
            if (objdata->unk30[0]->links[i] != -1) {
                EWTrobotpatrolB_func_A60(objdata->unk30, &objdata->unk90, temp_v0, objdata->unk30[0]->links[i], 1);
            }
        }
        for (i = 0; i < (objdata->unk90 - 1); i++) {
            var_v1 = objdata->unk90 - 2;
            while (var_v1 >= i) {
                if (objdata->unk30[var_v1 + 1]->uID < objdata->unk30[var_v1]->uID) {
                    temp_a2 = objdata->unk30[var_v1];
                    objdata->unk30[var_v1] = objdata->unk30[var_v1 + 1];
                    objdata->unk30[var_v1 + 1] = temp_a2;
                }
                var_v1 -= 1;
            }
        }
    }
}

// offset: 0xA60 | func: 13
static void EWTrobotpatrolB_func_A60(CurveSetup** arg0, u8* arg1, s32 arg2, s32 arg3, u8 arg4) {
    CurveSetup* temp_a1;
    s32 i;

    temp_a1 = gDLL_26_Curves->vtbl->func_39C(arg3);
    for (i = 0; i < 24; i++) {
        if (temp_a1 == arg0[i]) {
            return;
        }
    }
    arg0[*arg1] = temp_a1;
    *arg1 += 1;
    if (arg4 != 24) {
        for (i = 0; i != 4; i++) {
            if ((arg2 != temp_a1->links[i]) && (temp_a1->links[i] != -1)) {
                EWTrobotpatrolB_func_A60(arg0, arg1, arg3, temp_a1->links[i], arg4 + 1);
            }
        }
    }
}

// offset: 0xBA0 | func: 14
static void EWTrobotpatrolB_func_BA0(CurveSetup** arg0, CurveSetup** arg1, Object* arg2) {
    s32 var_s0;
    s32 sp3C[2];
    f32 temp_fv1;
    f32 sp30[2];
    f32 temp_fv0;

    sp30[0] = EWTrobotpatrolB_func_D10(arg1[0], arg2);
    sp30[1] = EWTrobotpatrolB_func_D10(arg1[1], arg2);
    sp3C[0] = 0;
    sp3C[1] = 1;
    temp_fv1 = sp30[0];
    if (sp30[1] < sp30[0]) {
        sp3C[0] = 1;
        sp3C[1] = 0;
        sp30[0] = sp30[1];
        sp30[1] = temp_fv1;
    }
    var_s0 = 2;
    while (var_s0 != 24) {
        if (arg1[var_s0] != NULL) {
            temp_fv0 = EWTrobotpatrolB_func_D10(arg1[var_s0], arg2);
            temp_fv1 = sp30[0];
            if (temp_fv0 < sp30[0]) {
                sp3C[1] = sp3C[0];
                sp3C[0] = var_s0;
                sp30[0] = temp_fv0;
                sp30[1] = temp_fv1;
            } else if (temp_fv0 < sp30[1]) {
                sp3C[1] = var_s0;
                sp30[1] = temp_fv0;
            }
        }
        var_s0 += 1;
    }
    arg0[0] = arg1[sp3C[0]];
    arg0[1] = arg1[sp3C[1]];
}

// offset: 0xD10 | func: 15
static f32 EWTrobotpatrolB_func_D10(CurveSetup* node, Object* obj) {
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;

    xDiff = obj->globalPosition.x - node->pos.x;
    yDiff = obj->globalPosition.y - node->pos.y;
    zDiff = obj->globalPosition.z - node->pos.z;
    return sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
}

// offset: 0xD7C | func: 16
static CurveSetup* EWTrobotpatrolB_func_D7C(Object* self, u32 uID) {
    EWTrobotpatrolB_Data* objdata = self->data;
    u8 currentIdx;
    u8 min;
    u8 max;
    
    // binary search
    max = objdata->unk90 - 1;
    min = 0;
    while (1) {
        currentIdx = (max + min) >> 1;
        if (objdata->unk30[currentIdx]->uID < uID) {
            min = currentIdx + 1;
        } else if (objdata->unk30[currentIdx]->uID > uID) {
            max = currentIdx - 1;
        } else {
            return objdata->unk30[currentIdx];
        }
    }
}

/*0x0*/ static const char str_0[] = " Could Not find a nearest node ";
/*0x20*/ static const char str_20[] = " Could Not get Nearest node ";
