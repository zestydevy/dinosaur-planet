#include "common.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "dlls/objects/common/sidekick.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    Object* unk4; // krystal
    Object* unk8; // kyte
    Object *unkC;
    Object *unk10;
    f32 unk14;
    f32 unk18;
    u8 _unk1C[0x20 - 0x1C];
} CCsandwormBoss_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000003, 0x00000006, 0x00000007
};
/*0xC*/ static u32 _data_C[] = {
    0x0000005a, 0x0000005b, 0x0000032b, 0x00000335
};
/*0x1C*/ static u32 _data_1C = 0xffc09600;

void dll_461_func_1030(Object* a0, void* a1);
void dll_461_func_1090(Object* a0, void* a1, void* a2, s32 a3);
void dll_461_func_1250(Object* a0, void* a1);
void dll_461_func_12B0(Object* a0, void* a1);
void dll_461_func_1384(Object* a0, Vec3f* a1, f32 a2);
void dll_461_func_14B0(Object* a0, void* a1);
void dll_461_func_1540(Object* a0, void* a1);

// offset: 0x0 | ctor
void dll_461_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_461_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_461_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_setup.s")

// offset: 0x2C8 | func: 1 | export: 1
void dll_461_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_control.s")

// offset: 0x330 | func: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_330.s")

// offset: 0x564 | func: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_564.s")

/*0x0*/ static const char str_0[] = "need to prempt fire crystal into correct position\n";

// offset: 0x5E0 | func: 4
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_5E0.s")
#else
// needs called funcs to be static
void dll_461_func_5E0(Object *self, CCsandwormBoss_Data *objdata) {
    ObjSetup* setup;
    f32 dist;
  
    setup = self->setup;
    objdata->unk18 += gUpdateRateF;
    dist = 3.4028235e38f;
    obj_get_nearest_type_to(0x12, self, &dist);

    diPrintf("worm %d, barrel %d\n", (s32) vec3_distance_xz(&self->positionMirror, &objdata->unk4->positionMirror), (s32) dist);
    switch (objdata->unk0) {
    case 4:
        dll_461_func_12B0(self, objdata->unk4);
        if (vec3_distance_xz_squared(&self->positionMirror, &objdata->unk4->positionMirror) < 32400.0f) {
            dll_461_func_1090(self, objdata->unk4, objdata, 5);
        } else {
            if (((DLL_ISidekick*)objdata->unk8->dll)->vtbl->func24(objdata->unk8) != 0) {
                diPrintf("kyte dist %d interest range 50.0F\n", (s32) vec3_distance_xz(&self->positionMirror, &objdata->unk8->positionMirror));
                if (vec3_distance_xz_squared(&self->positionMirror, &objdata->unk8->positionMirror) < 3600.0f) {
                    dll_461_func_1250(self, objdata);
                    objdata->unk18 = 0.0f;
                }
            }
        }
        break;
    case 5:
        dll_461_func_12B0(self, objdata->unk4);
        if (self->animProgress > 0.95f) {
            dll_461_func_1030(self, objdata);
        }
        break;
    case 6:
        dll_461_func_12B0(self, objdata->unk4);
        if (self->animProgress > 0.95f) {
            dll_461_func_1250(self, objdata);
        }
        break;
    case 7:
        dll_461_func_12B0(self, objdata->unk8);
        if (self->animProgress > 0.95f) {
            dll_461_func_1250(self, objdata);
        }
        break;
    case 8:
        dll_461_func_12B0(self, objdata->unk4);
        if (self->animProgress > 0.95f) {
            objdata->unk0 = 0xD;
            objdata->unk14 = 0.005f;
            func_80023D30(self, 5, 0, 0);
        }
        dll_461_func_14B0(self, objdata);
        break;
    case 9:
        dll_461_func_12B0(self, objdata->unk8);
        if (objdata->unk18 > 300.0f) {
            main_set_bits(0x46E, 0x65U);
        } else {
            main_set_bits(0x46E, 0xC3U);
        }
        if (vec3_distance_xz_squared(&self->positionMirror, &objdata->unk4->positionMirror) < 32400.0f) {
            dll_461_func_1090(self, objdata->unk4, objdata, 6);
        } else if (vec3_distance_xz_squared(&self->positionMirror, &objdata->unk8->positionMirror) < 32400.0f) {
            dll_461_func_1090(self, objdata->unk8, objdata, 7);
        } else {
            if ((((DLL_ISidekick*)objdata->unk8->dll)->vtbl->func24(objdata->unk8) != 0) || (vec3_distance_xz_squared(&self->positionMirror, &objdata->unk8->positionMirror) < 90000.0f)) {
                func_8002493C(self, 1.5f, &objdata->unk14);
                dll_461_func_1384(self, &objdata->unk8->srt.transl, 1.5f);
            } else if (vec3_distance_xz_squared(&self->positionMirror, (Vec3f* ) &setup->x) < 10000.0f) {
                dll_461_func_1030(self, objdata);
            } else {
                objdata->unk0 = 0xA;
                objdata->unk14 = 0.01f;
                func_80023D30(self, 2, 0, 0);
            }
        }
        break;
    case 10:
        if (self->animProgress > 0.95f) {
            objdata->unk0 = 0xB;
        }
        break;
    case 11:
        if (((s32) setup->x == (s32) self->srt.transl.f[0]) && ((s32) setup->z == (s32) self->srt.transl.f[2])) {
            dist = vec3_distance_xz_squared(&self->positionMirror, &objdata->unk4->positionMirror);
            if (dist < 2500.0f) {
                objdata->unk3 = 0;
                gDLL_3_Animation->vtbl->func17(8, objdata->unkC, -1);
            } else if (dist < 32400.0f) {
                objdata->unk0 = 0xC;
                objdata->unk14 = 0.005f;
                func_80023D30(self, 8, 0, 0);
                gDLL_6_AMSFX->vtbl->play_sound(self, (u16)_data_C[rand_next(0, 3)], 0x7FU, NULL, NULL, 0, NULL);
                objdata->unk2 = 0;
                objdata->unk1 = 3;
            } else {
                dist = 50.0f;
                objdata->unk10 = obj_get_nearest_type_to(0x12, self, &dist);
                if ((objdata->unk10 != NULL) && (gDLL_54->vtbl->func4.withOneArgS32((s32)objdata->unk10->data) == 0)) {
                    objdata->unk0 = 0xD;
                    objdata->unk3 = 0;
                    objdata->unk18 = 0.0f;
                    gDLL_3_Animation->vtbl->func17(5, objdata->unkC, -1);
                } else {
                    objdata->unk0 = 0xC;
                    objdata->unk14 = 0.01f;
                    func_80023D30(self, 0xB, 0, 0);
                    objdata->unk2 = 0;
                    objdata->unk1 = 3;
                }
            }
        } else {
            dll_461_func_1384(self, (Vec3f* ) &setup->x, 3.0f);
            if (vec3_distance_xz_squared(&self->positionMirror, (Vec3f* ) &setup->x) < 10000.0f) {
                objdata->unk2 = 1;
            }
        }
        break;
    case 12:
        dll_461_func_12B0(self, objdata->unk4);
        if (self->animProgress > 0.95f) {
            dll_461_func_1030(self, objdata);
        }
        break;
    case 13:
        if (objdata->unk10 != NULL) {
            setup = objdata->unk10->setup;
            objdata->unk10->srt.transl.f[0] = setup->x;
            objdata->unk10->srt.transl.f[1] = setup->y;
            objdata->unk10->srt.transl.f[2] = setup->z;
            objdata->unk10 = NULL;
        }
        if (objdata->unk18 > 3000.0f) {
            objdata->unk0 = 4;
        } else if (vec3_distance_xz_squared(&self->positionMirror, &objdata->unk4->positionMirror) < 32400.0f) {
            dll_461_func_1090(self, objdata->unk4, objdata, 8);
        }
        dll_461_func_14B0(self, objdata);
        break;
    case 14:
        objdata->unk0 = 0xF;
        self->srt.flags |= 0x4000;
        func_800267A4(self);
        func_80026160(self);
        main_set_bits(0x3FB, 1);
        break;
    case 15:
        return;
    }

    func_80024108(self, objdata->unk14, gUpdateRateF, 0);
    dll_461_func_1540(self, objdata);
}

static const char str_1[] = "setting flight group to %d\n";
static const char str_2[] = "setting flight group to %d\n";
static const char str_3[] = "eat player sequence\n";
static const char str_4[] = "attack player from under ground\n";
static const char str_5[] = "eat barrel\n";
static const char str_6[] = "barrel %x put to %f %f %f\n";
static const char str_7[] = "worm at %f %f %f\n";
static const char str_8[] = "boss dead\n";
static const char str_9[] = "turning on parts\n";
static const char str_10[] = "Parts all over\n";
static const char str_11[] = "yaw to player\n";
#endif

// offset: 0x1030 | func: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_1030.s")

// offset: 0x1090 | func: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_1090.s")

// offset: 0x1250 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_1250.s")

// offset: 0x12B0 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_12B0.s")

// offset: 0x1384 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_1384.s")

// offset: 0x14B0 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_14B0.s")

// offset: 0x1540 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_1540.s")

// offset: 0x1634 | func: 12 | export: 2
void dll_461_update(Object *self) { }

// offset: 0x1640 | func: 13 | export: 3
void dll_461_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_print.s")

// offset: 0x1A20 | func: 14 | export: 4
void dll_461_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_free.s")

// offset: 0x1AC0 | func: 15 | export: 5
u32 dll_461_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1AD0 | func: 16 | export: 6
u32 dll_461_get_data_size(Object *self, u32 a1) {
    return sizeof(CCsandwormBoss_Data);
}

// offset: 0x1AE4 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/461_CCsandwormBoss/dll_461_func_1AE4.s")
