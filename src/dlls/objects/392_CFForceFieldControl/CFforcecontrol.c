#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "dlls/objects/391_CFForceField.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/rand.h"
#include "dll.h"
#include "macros.h"

/** @file official filename: CFforcecontrol.c */

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 _unk19[0x1C - 0x19];
    f32 unk1C;
} CFForceControl_Setup;

typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6[9];
    s16 unk18[9];
    s16 unk2A;
    s16 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    f32 unk3C;
    u8 _unk40[0x4C - 0x40];
    f32 unk4C;
    u8 _unk50[0x54 - 0x50];
    u32 unk54;
} CFForceControl_Data;

static void CFForceControl_func_544(Object*);
static void CFForceControl_func_628(CFForceControl_Data* objdata);
static void CFForceControl_func_638(Object*, CFForceControl_Data*, f32, f32, f32, f32);
static void CFForceControl_func_7D4(Object*, CFForceControl_Data*);
static void CFForceControl_func_AF4(f32, f32, f32, s16*, s16*);
static void CFForceControl_func_B90(s16, s16, Vec3f*, f32);
static Object* CFForceControl_func_CF0(Object* self, f32 arg1, f32 arg2, f32 arg3);

// offset: 0x0 | ctor
void CFForceControl_ctor(void* dll) { }

// offset: 0xC | dtor
void CFForceControl_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFForceControl_obj_Setup(Object* self, CFForceControl_Setup* setup, s32 reset) {
    CFForceControl_Data* objdata = self->data;
    f32 temp;
    
    func_8002674C(self);
    func_80026128(self, 7, 0, -1);
    func_80026724(self);
    if (setup->unk1C != 0.0f) {
        temp = setup->unk1C / 255.0f;
        self->srt.scale = self->def->scale * temp;
        self->visRadius = 400.0f;
    }
    objdata->unk3C = setup->unk18;
    objdata->unk38 = (s32) setup->unk1C;
    objdata->unk34 = 1;
    objdata->unk30 = 0x51;
    objdata->unk4C = 90.0f;
    mainSetBits(BIT_30B, 1);
    CFForceControl_func_628(objdata);
}

// offset: 0x16C | func: 1 | export: 1
void CFForceControl_obj_Control(Object* self) {
    CFForceControl_Data* objdata = self->data;
    
    if (mainGetBits(BIT_2E1) != 0) {
        mainSetBits(BIT_30B, 0);
        objDisable(self);
        gDLL_6_AMSFX->vtbl->Play(self, SOUND_5B9, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (objdata->unk54 != 0) {
            gDLL_6_AMSFX->vtbl->Stop(objdata->unk54);
        }
    } else {
        if (objdata->unk30 != 0) {
            if ((objdata->unk30 % 20) == 0) {
                CFForceControl_func_544(self);
            }
            objdata->unk30--;
            if (objdata->unk30 == 0) {
                self->srt.transl.y -= objdata->unk3C;
            }
        }
        if (objdata->unk54 == 0) {
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_5B7, MAX_VOLUME, &objdata->unk54, NULL, 0, NULL);
        }
        func_80026128(self, 8, 1, 0);
        CFForceControl_func_7D4(self, objdata);
    }
}

// offset: 0x324 | func: 2 | export: 2
void CFForceControl_obj_Update(Object* self) {
    CFForceControl_Data* objdata;
    Object* sp40;
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;
    ObjectHitInfo *hit;

    objdata = self->data;
    hit = self->objhitInfo;
    if (hit->unk48 != NULL) {
        sp40 = hit->unk48;
        if (sp40 != NULL) {
            STUBBED_PRINTF(" Side %x ObHit %x Romdef %i", objGetSidekick(), sp40, sp40->id);
            xDiff = sp40->srt.transl.x - self->srt.transl.x;
            yDiff = sp40->srt.transl.y - self->srt.transl.y;
            zDiff = sp40->srt.transl.z - self->srt.transl.z;
            if (objdata->unk4 == 0) {
                CFForceControl_func_CF0(self, xDiff / 100.0f, yDiff / 100.0f, zDiff / 100.0f);
            }
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_5B8, MAX_VOLUME, NULL, NULL, 0, NULL);
            CFForceControl_func_638(self, objdata, xDiff, yDiff, zDiff, 40.0f);
        }
    }
}

// offset: 0x480 | func: 3 | export: 3
void CFForceControl_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x498 | func: 4 | export: 4
void CFForceControl_obj_Free(Object* self, s32 onlySelf) {
    CFForceControl_Data* objdata = self->data;

    if (objdata->unk54 != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objdata->unk54);
    }
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x520 | func: 5 | export: 5
u32 CFForceControl_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x530 | func: 6 | export: 6
u32 CFForceControl_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFForceControl_Data);
}

// offset: 0x544 | func: 7
static void CFForceControl_func_544(Object* self) {
    CFForceField_Setup* forcefield;
    ObjSetup* setup;
    CFForceControl_Data* objdata;

    setup = self->setup;
    objdata = self->data;
    forcefield = objAllocSetup(sizeof(CFForceField_Setup), OBJ_CFForceField);
    forcefield->base.x = self->srt.transl.x;
    forcefield->base.y = self->srt.transl.y;
    forcefield->base.z = self->srt.transl.z;
    forcefield->base.loadFlags = setup->loadFlags;
    forcefield->base.byte5 = setup->byte5;
    forcefield->base.byte6 = setup->byte6;
    forcefield->base.fadeDistance = setup->fadeDistance;
    forcefield->unk1C = 90.0f;
    forcefield->unk18 = (s8) objdata->unk3C ^ 0; // @fake (partially)
    forcefield->base.objId = OBJ_CFForceField;
    objSetupObject(&forcefield->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, NULL);
}

// offset: 0x628 | func: 8
static void CFForceControl_func_628(CFForceControl_Data* objdata) {
    objdata->unk4 = 0;
    objdata->unk2C = 0;
}

// offset: 0x638 | func: 9
void CFForceControl_func_638(Object* arg0, CFForceControl_Data* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    s32 i;
    SRT sp2C;

    if ((arg2 != 0.0f) || (arg3 != 0.0f) || (arg4 != 0.0f)) {
        guNormalize(&arg2, &arg3, &arg4);
    }
    if (arg1->unk4 == 0) {
        arg1->unk2A = mathRnd(3, 9);
        CFForceControl_func_AF4(arg2, arg3, arg4, &arg1->unk6[0], &arg1->unk18[0]);        
        for (i = 1; i < arg1->unk2A; i++) {
            arg1->unk6[i] = arg1->unk6[0];
            arg1->unk18[i] = arg1->unk18[0];
        }
        arg1->unk4 = 0x32;
        arg1->unk2C += 0x500;
        arg1->unk0 = arg5;
    }
    sp2C.transl.x = arg2 * arg5;
    sp2C.transl.y = arg3 * arg5;
    sp2C.transl.z = arg4 * arg5;
    gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_35B, &sp2C, 0, -1, NULL);
}

// offset: 0x7D4 | func: 10
static void CFForceControl_func_7D4(Object* arg0, CFForceControl_Data* arg1) {
    SRT sp80;
    f32 temp_fs0;
    s16 temp_ft2;
    s16 temp_s1;
    s16 temp_s3;
    s32 sp70;
    s32 sp6C;
    s32 temp_t8;
    s32 var_s2;

    if (arg1->unk4 == 0) {
        return;
    }

    sp6C = gUpdateRateF * 0.5f;
    for (sp70 = 0; sp70 < sp6C; sp70++) {
        sp80.roll = MIN(0xFF, arg1->unk4 * 2);
        for (var_s2 = 0; var_s2 < arg1->unk2A; var_s2++) {
            CFForceControl_func_B90(arg1->unk6[var_s2], arg1->unk18[var_s2], &sp80.transl, arg1->unk0);
            gDLL_17_partfx->vtbl->spawn(arg0, 0x35A, &sp80, 0, -1, NULL);
            temp_s1 = ((0xFFFF / arg1->unk2A) * var_s2) + arg1->unk2C;
            temp_fs0 = mathSinfInterp(temp_s1);
            temp_s3 = mathRnd(0, 0x350) + (temp_fs0 * 592.0f) - 424.0f;
            temp_fs0 = mathCosfInterp(temp_s1);
            temp_ft2 = mathRnd(0, 0x350) + (temp_fs0 * 592.0f) - 424.0f;
            arg1->unk6[var_s2] += (2.0f * temp_s3);
            arg1->unk18[var_s2] += (2.0f * temp_ft2);
        }
    }

    arg1->unk4 -= gUpdateRate;
    if (arg1->unk4 < 0) {
        arg1->unk4 = 0;
    }
}

// offset: 0xAF4 | func: 11
static void CFForceControl_func_AF4(f32 arg0, f32 arg1, f32 arg2, s16* arg3, s16* arg4) {
    f32 sp24;

    sp24 = sqrtf(SQ(arg0) + SQ(arg2));
    *arg3 = mathAtan2f(arg2, arg0);
    *arg4 = mathAtan2f(sp24, arg1);
}

// offset: 0xB90 | func: 12
static void CFForceControl_func_B90(s16 arg0, s16 arg1, Vec3f* arg2, f32 arg3) {
    f32 sp2C;

    sp2C = mathSinfInterp(arg1);
    arg2->x = mathCosfInterp(arg0) * sp2C * arg3;
    if ((mathCosfInterp(arg1) * arg3) >= 0.0f) {
        arg2->y = mathCosfInterp(arg1) * arg3;
    } else {
        arg2->y = -(mathCosfInterp(arg1) * arg3);
    }
    sp2C = mathSinfInterp(arg1);
    arg2->z = mathSinfInterp(arg0) * sp2C * arg3;
}

// offset: 0xCF0 | func: 13
Object* CFForceControl_func_CF0(Object* self, f32 arg1, f32 arg2, f32 arg3) {
    LFXEmitter_Setup* emitter;

    STUBBED_PRINTF(" Creating Light ");
    emitter = objAllocSetup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
    emitter->base.loadDistance = 255;
    emitter->base.fadeDistance = 255;
    emitter->base.x = self->srt.transl.x + (arg1 * 100.0f);
    emitter->base.y = self->srt.transl.y + (arg2 * 100.0f);
    emitter->base.z = self->srt.transl.z + (arg3 * 100.0f);
    emitter->unk20 = 0x3C;
    emitter->unk1E = 0x19C;
    emitter->unk18 = mathRnd(-4000, 4000);
    emitter->unk1A = 0;
    emitter->unk1C = mathRnd(-4000, 4000);
    emitter->unk24 = 0;
    emitter->unk22 = 0x95;
    return objSetupObject(&emitter->base, OBJINIT_STANDALONE, self->mapID, -1, self->parent);
}
