#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/intersect.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objexpr.h"
#include "sys/objprint.h"
#include "sys/rand.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 _unk19[0x1C - 0x19];
    f32 unk1C;
} CFForceField_Setup;

typedef struct {
    f32 unk0;
    u8 _unk4[0xC - 0x4];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 unk26;
    u8 _unk27;
    s16 unk28;
    u8 _unk2A[0x2C - 0x2A];
} CFForceField_Data;

static f32 CFForceField_func_55C(f32);
static void CFForceField_func_5A4(CFForceField_Data* objdata);
static f32 CFForceField_func_66C(Object* self);

// offset: 0x0 | ctor
void CFForceField_ctor(void* dll) { }

// offset: 0xC | dtor
void CFForceField_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFForceField_obj_Setup(Object* self, CFForceField_Setup* setup, s32 reset) {
    CFForceField_Data* objdata = self->data;
    TextureAnimator* texAnimator;
    f32 temp_fv0;
    f32 var_fv0;
    f32 temp;
    
    texAnimator = objExprGetTexAnimator(self, 0, 0);
    if (texAnimator != NULL) {
        texAnimator->frame = 0;
    }
    temp_fv0 = CFForceField_func_66C(self);
    if (setup->unk1C != 0.0f) {
        temp = setup->unk1C / 255.0f;
        self->srt.scale = self->def->scale * temp;
    }
    if ((setup->unk18 < temp_fv0) && (setup->unk18 != 0)) {
        var_fv0 = setup->unk18;
    } else {
        var_fv0 = 10.0f;
    }
    objdata->unk1C = 0;
    objdata->unk0 = var_fv0;
    objdata->unkC = var_fv0;
    objdata->unk10 = self->srt.transl.y - var_fv0;
    objdata->unk24 = 255;
    objdata->unk26 = 1;
    objdata->unk14 = 0.0f;
    objdata->unk18 = 0.005f;
    CFForceField_func_5A4(objdata);
}

// offset: 0x168 | func: 1 | export: 1
void CFForceField_obj_Control(Object* self) {
    CFForceField_Data* objdata = self->data;
    TextureAnimator* texAnimator;
    f32 temp_fv1;
    s32 var_v1;
    
    if ((mainGetBits(BIT_CF_Floor_Destroyed) != 0) && (objdata->unk26 != 0)) {
        objdata->unk26 = 0;
        objdata->unk28 = 100;
    }
    texAnimator = objExprGetTexAnimator(self, 0, 0);
    if (texAnimator != NULL) {
        var_v1 = texAnimator->frame + (gUpdateRate << 6);
        if (var_v1 > 0x800) {
            var_v1 = 0;
        }
        texAnimator->frame = var_v1;
        texAnimator->positionU -= 0x10;
        if (texAnimator->positionU < -0x3E0) {
            texAnimator->positionU = 0;
        }
    }
    objdata->unk1C += 0x200;
    objdata->unk0 -= (objdata->unk14 * (f32) gUpdateRate);
    objdata->unk14 += objdata->unk18;
    if ((objdata->unk0 < 0.0f) && (objdata->unk26 != 0)) {
        objdata->unk14 = 0.0f;
        CFForceField_func_5A4(objdata);
        objdata->unk0 = objdata->unkC - 1.0f;
    }
    objdata->unk24 += objdata->unk1E;
    if (objdata->unk22 < objdata->unk24) {
        objdata->unk24 = objdata->unk22;
        objdata->unk1E = -objdata->unk1E;
    }
    if (objdata->unk24 < objdata->unk20) {
        objdata->unk24 = objdata->unk20;
        objdata->unk1E = -objdata->unk1E;
    }
    if ((objdata->unk0 / objdata->unkC) < 0.08f) {
        temp_fv1 = (objdata->unk0 / objdata->unkC) * 5.0f * 127.0f;
        objdata->unk24 = (s16) temp_fv1;
    }
    if (objdata->unk26 == 0) {
        objdata->unk28 -= gUpdateRate;
        objdata->unk24 = objdata->unk28;
        if (objdata->unk28 < 0) {
            mainSetBits(BIT_34F, 1);
            objFreeObject(self);
        }
    }
}

// offset: 0x3E0 | func: 2 | export: 2
void CFForceField_obj_Update(Object* self) { }

// offset: 0x3EC | func: 3 | export: 3
void CFForceField_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CFForceField_Data* objdata = self->data;
    f32 scale = self->srt.scale;
    
    if ((objdata->unk26 != 0) || (objdata->unk28 >= 0)) {
        self->opacity = (u8) objdata->unk24;
        if (objdata->unk26 == 0) {
            self->srt.pitch = objdata->unk1C;
            scale *= (1.0f + (gUpdateRateF * 0.01f));
        } else {
            self->srt.scale *= CFForceField_func_55C(objdata->unk0 / objdata->unkC);
        }
        self->srt.yaw = objdata->unk1C;
        self->srt.transl.y = objdata->unk10 + objdata->unk0;
        if (visibility != 0) {
            objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
        self->srt.scale = scale;
    }
}

// offset: 0x528 | func: 4 | export: 4
void CFForceField_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x538 | func: 5 | export: 5
u32 CFForceField_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x548 | func: 6 | export: 6
u32 CFForceField_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFForceField_Data);
}

// offset: 0x55C | func: 7
static f32 CFForceField_func_55C(f32 arg0) {
    return sqrtf(1.001f - SQ(arg0));
}

// offset: 0x5A4 | func: 8
static void CFForceField_func_5A4(CFForceField_Data* objdata) {
    objdata->unk1E = mathRnd(0, 15) + 8;
    objdata->unk22 = mathRnd(0, 254) + 127;
    if (objdata->unk22 > 255) {
        objdata->unk22 = 255;
    }
    objdata->unk20 = 126 - mathRnd(0, 64);
    if (objdata->unk20 < 0) {
        objdata->unk20 = 0;
    }
    objdata->unk24 = objdata->unk20;
}

// offset: 0x66C | func: 9
static f32 CFForceField_func_66C(Object* self) {
    TrackHeightResult** results;
    f32 smallestHeight;
    f32 count; // @bug: should be an integer
    f32 height;
    s32 i;

    count = trackGetHeight(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &results, 0, 0);
    // @bug: smallestHeight is undefined if trackGetHeight finds nothing
    if (count != 0) {
        smallestHeight = 10000.0f;
        for (i = 0; i < count; i++) {
            height = self->srt.transl.y - results[i]->y;
            if (height > 0.0f && height < ABS_EXPR(smallestHeight)) {
                smallestHeight = height;
            }
        }
    }
    return smallestHeight;
}
