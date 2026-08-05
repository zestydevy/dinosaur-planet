#include "common.h"
#include "dlls/objects/437_EWTrobotpatrol.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    u8 unk18[0x1A - 0x18];
    u8 unk1A;
    u8 unk1B;
} DLL439_Setup;

typedef struct {
    Object* unk0[12];
    CurveSetup* unk30[1];
    u8 _unk34[0x90 - 0x34];
    u8 unk90;
    u8 unk91;
    u8 unk92;
    s16 unk94;
    s16 unk96;
    f32 unk98;
} DLL439_Data;

// objsetup/curve setup?
typedef struct {
    u8 _unk0[0x8 - 0x0];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s32 unk14;
} Func1D0_Arg1;

/*0x0*/ static u32 data_0[] = {
    0xffffffff, 0xffff0000
};
/*0x8*/ static u32 data_8[] = {
    0x0000001e, 0x00000000
};

/*0x0*/ static u8 bss_0[0x60];
/*0x60*/ static u8 bss_60[0x4];
/*0x64*/ static u8 bss_64[0x4];
/*0x68*/ static u8 _bss_68[0x8];

void dll_439_func_30C(Object* base, Object* robot, s32 arg2, s32 arg3);
void dll_439_func_3A4(Object* self, Object* robo, u32 uID);
static CurveSetup* dll_439_func_D7C(Object* self, u32 uID);

// offset: 0x0 | ctor
void dll_439_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_439_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_439_obj_Setup(Object* self, ObjSetup* setup, s32 reset) { }

// offset: 0x2C | func: 1 | export: 1
void dll_439_obj_Control(Object* self) { }

// offset: 0x38 | func: 2 | export: 2
void dll_439_obj_Update(Object* self) {
    DLL439_Data* objdata;
    DLL439_Setup* setup;

    objdata = self->data;
    setup = (DLL439_Setup*)self->setup;
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
void dll_439_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x134 | func: 4 | export: 4
void dll_439_obj_Free(Object* self, s32 onlySelf) {
    s32 i;
    DLL439_Data* objdata;

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
u32 dll_439_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1BC | func: 6 | export: 6
u32 dll_439_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DLL439_Data);
}

// offset: 0x1D0 | func: 7
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/439_EWTrobotpatrolB/dll_439_func_1D0.s")
#else
void dll_439_func_1D0(Object* arg0, Func1D0_Arg1* arg1) {
    DLL439_Data* objdata;
    DLL439_Setup* setup;
    ObjSetup* roboSetup;
    Object* robo;
    s32 i;

    objdata = arg0->data;
    setup = (DLL439_Setup*)arg0->setup;
    i = 0;
    while (objdata->unk0[i] != NULL) {
        i++;
    }
    roboSetup = objAllocSetup(sizeof(EWTrobotpatrol_Setup), OBJ_EWTrobotpatrol);
    roboSetup->actExclusions1 = setup->base.actExclusions1;
    roboSetup->loadFlags = OBJSETUP_LOAD_MANUAL;
    roboSetup->fadeDistance = setup->unk1A;
    roboSetup->x = arg1->unk8;
    roboSetup->y = arg1->unkC;
    roboSetup->z = arg1->unk10;
    robo = objSetupObject(roboSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, arg0->parent);
    objdata->unk0[i] = robo;
    ((DLL_437_EWTrobotpatrol*)robo->dll)->vtbl->Func_3AE4(robo, arg0, dll_439_func_30C);
    dll_439_func_3A4(arg0, robo, arg1->unk14);
}
#endif

// offset: 0x30C | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/439_EWTrobotpatrolB/dll_439_func_30C.s")

// offset: 0x3A4 | func: 9
void dll_439_func_3A4(Object* self, Object* robo, u32 uID) {
    CurveSetup* node;
    u8 max;

    max = 0;
    node = dll_439_func_D7C(self, uID);
    while (node->links[max + 1] != -1 && max != 3) {
        max++;
    }
    ((DLL_437_EWTrobotpatrol*)robo->dll)->vtbl->Func_3AF8(robo, node->links[mathRnd(0, max)]);
}

// offset: 0x470 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/439_EWTrobotpatrolB/dll_439_func_470.s")

// offset: 0x60C | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/439_EWTrobotpatrolB/dll_439_func_60C.s")

// offset: 0x88C | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/439_EWTrobotpatrolB/dll_439_func_88C.s")

// offset: 0xA60 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/439_EWTrobotpatrolB/dll_439_func_A60.s")

// offset: 0xBA0 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/439_EWTrobotpatrolB/dll_439_func_BA0.s")

// offset: 0xD10 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/439_EWTrobotpatrolB/dll_439_func_D10.s")

// offset: 0xD7C | func: 16
static CurveSetup* dll_439_func_D7C(Object* self, u32 uID) {
    DLL439_Data* objdata = self->data;
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
