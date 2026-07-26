#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/** @file baddie partfx */

/*0x0*/ static u32 data_0[] = {
    0x000000df, 0x000001fc, 0x00000200, 0x000001fb, 0x42b40000, 0x42960000, 0xc2b40000, 0x42960000, 
    0x42c80000, 0xc2aa0000, 0xc2c80000, 0xc2aa0000, 0x00000000, 0x43200000, 0x00000000, 0xc2e60000
};
/*0x40*/ static f32 data_40 = 0.1f;
/*0x44*/ static f32 data_44 = 0.3f;
/*0x48*/ static f32 data_48 = 0.1f;
/*0x4C*/ static f32 data_4C = 0.3f;
/*0x50*/ static s32 data_50 = 0;
/*0x54*/ static s32 data_54 = 0;
/*0x58*/ static f32 data_58 = 0.0;
/*0x5C*/ static f32 data_5C = 0.0;

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void badPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void badPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void badPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void badPartfx_Func_20(s32 arg0) {
    data_48 += (gUpdateRateF * 0.001f);
    if (data_48 > 1.0f) {
        data_48 = 0.1f;
    }
    data_4C += (gUpdateRateF * 0.001f);
    if (data_4C > 1.0f) {
        data_4C = 0.3f;
    }
    data_50 += (gUpdateRate * 100);
    if (data_50 >= 0x8000) {
        data_50 = 0;
    }
    data_5C = mathSinfInterp((s16) data_50);
    data_54 += (gUpdateRate * 50);
    if (data_54 >= 0x8000) {
        data_54 = 0;
    }
    data_58 = mathSinfInterp((s16) data_54);
}

// offset: 0x194 | func: 2 | export: 1
s32 badPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp44;
    s32 var_v0_2;
    s32 var_v1;
    f32 var_fv0;

    data_40 += 0.001f;
    if (data_40 > 1.0f) {
        data_40 = 0.1f;
    }
    data_44 += 0.0003f;
    if (data_44 > 1.0f) {
        data_44 = 0.3f;
    }
    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX badning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
        return -1;
    }
    if (flags & 0x200000) {
        if (transform == NULL) {
            // STUBBED_PRINTF("ERROR partfx EXF_NOOWNER & no attrib %d\n", id); // default.dol
            return -1;
        }
        sp44.transform.transl.x = transform->transl.x;
        sp44.transform.transl.y = transform->transl.y;
        sp44.transform.transl.z = transform->transl.z;
        sp44.transform.scale = transform->scale;
        sp44.transform.roll = transform->roll;
        sp44.transform.pitch = transform->pitch;
        sp44.transform.yaw = transform->yaw;
        sp44.unk62 = arg4;
    }
    sp44.unk44 = 0;
    sp44.unk48 = 0;
    sp44.unk5E = id;
    sp44.unk0 = obj;
    sp44.unk8 = 0;
    sp44.unk4 = -1;
    sp44.unk60 = 0xFF;
    sp44.unk61 = 0;
    sp44.unk42 = 0;
    sp44.unk30.x = 0.0f;
    sp44.unk30.y = 0.0f;
    sp44.unk30.z = 0.0f;
    sp44.unk24.x = 0.0f;
    sp44.unk24.y = 0.0f;
    sp44.unk24.z = 0.0f;
    sp44.unk3C = 0.0f;
    sp44.unk58[0] = 0xFFFF;
    sp44.unk58[1] = 0xFFFF;
    sp44.unk58[2] = 0xFFFF;
    sp44.unk4C[0] = 0xFFFF;
    sp44.unk4C[1] = 0xFFFF;
    sp44.unk4C[2] = 0xFFFF;
    sp44.unk40 = 0;
    if (transform == NULL) {
        transform = &bss_0;
        transform->transl.x = 0.0f;
        transform->transl.y = 0.0f;
        transform->transl.z = 0.0f;
        transform->scale = 1.0f;
        transform->yaw = 0;
        transform->pitch = 0;
        transform->roll = 0;
    }
    switch (id) {
    case 0x32A:
        sp44.unk8 = (s32) ((transform->scale * 50.0f) + 20.0f);
        sp44.unk3C = (f32) sp44.unk8 * 0.0008f;
        sp44.unk44 = 0x08100200;
        sp44.unk42 = 0x57;
        sp44.transform.transl.x = transform->transl.x;
        sp44.transform.transl.y = transform->transl.y;
        sp44.transform.transl.z = transform->transl.z;
        sp44.transform.scale = 1.0f;
        sp44.transform.roll = 0;
        sp44.transform.pitch = 0;
        sp44.transform.yaw = transform->yaw;
        sp44.unk60 = 0xFF;
        break;
    case 0x32B:
        sp44.unk8 = (s32) (((f32) mathRnd(0x96, 0xC8) * transform->scale) + 50.0f);
        sp44.unk3C = (f32) sp44.unk8 * 0.00014f;
        sp44.unk44 = 0x08100200;
        sp44.unk42 = 0x56;
        sp44.transform.transl.x = transform->transl.x;
        sp44.transform.transl.y = transform->transl.y;
        sp44.transform.transl.z = transform->transl.z;
        sp44.transform.scale = 1.0f;
        sp44.transform.roll = 0;
        sp44.transform.pitch = 0;
        sp44.transform.yaw = 0;
        sp44.unk60 = 0xFF;
        break;
    case 0x32C:
        sp44.unk3C = (f32) mathRnd(2, 4) * 0.02f;
        sp44.unk8 = 0xC8;
        sp44.unk44 = 0x08100200;
        sp44.unk42 = 0x56;
        sp44.transform.transl.x = transform->transl.x;
        sp44.transform.transl.y = transform->transl.y;
        sp44.transform.transl.z = transform->transl.z;
        sp44.transform.scale = 1.0f;
        sp44.transform.roll = 0;
        sp44.transform.pitch = 0;
        sp44.transform.yaw = 0;
        sp44.unk60 = 0xFF;
        break;
    case 0x32D:
        sp44.unk8 = 0x32;
        sp44.unk44 = 0x180200;
        sp44.unk48 = 0x01000000;
        sp44.unk42 = 0x60;
        sp44.unk60 = 0xFF;
        sp44.unk3C = 0.025f;
        break;
    case 0x32E:
        sp44.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp44.unk24.y = (f32) mathRnd(0xA, 0x50) * 0.002f;
        sp44.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp44.unk3C = (f32) mathRnd(5, 0x19) * 0.0001f;
        sp44.unk8 = mathRnd(0x64, 0x78);
        sp44.transform.yaw = mathRnd(0, 0xFFFF);
        sp44.transform.pitch = mathRnd(0, 0xFFFF);
        sp44.transform.yaw = mathRnd(0, 0xFFFF);
        sp44.transform.transl.x = (f32) mathRnd(0xE6, 0x320);
        sp44.transform.transl.y = (f32) mathRnd(0xE6, 0x320);
        sp44.transform.transl.z = (f32) mathRnd(0xE6, 0x320);
        sp44.unk48 = 0x01000020;
        sp44.unk44 = 0x86000008;
        sp44.unk4C[0] = sp44.unk58[0] = mathRnd(0x8000, 0xFFFF);
        sp44.unk4C[1] = sp44.unk58[1] = 0xFFFF;
        sp44.unk4C[2] = sp44.unk58[2] = 0xFFFF;
        sp44.unk42 = 0x3A3;
        break;
    case 0x32F:
        sp44.unk30.x = transform->transl.x;
        sp44.unk30.y = transform->transl.y;
        sp44.unk30.z = transform->transl.z;
        sp44.unk8 = 0xA;
        sp44.unk60 = 0x40;
        sp44.unk61 = 0x10;
        sp44.unk44 = 0x80440202;
        sp44.unk42 = 0x17C;
        sp44.unk3C = 0.01465f;
        if (data != NULL) {
            var_fv0 = *(f32*)data;
            sp44.unk3C *= var_fv0;
            if (var_fv0 > 1.5f) {
                sp44.unk44 &= ~0x40000;
            }
            if (var_fv0 > 1.75f) {
                sp44.unk44 |= 0x10;
            }
            if (var_fv0 > 1.9f) {
                sp44.unk8 = 0x64;
            }
        }
        break;
    case 0x330:
        sp44.unk30.x = transform->transl.x;
        sp44.unk30.y = transform->transl.y;
        sp44.unk30.z = transform->transl.z;
        sp44.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.005f;
        sp44.unk3C = 0.001f;
        sp44.unk8 = mathRnd(0x96, 0xC8);
        sp44.unk60 = mathRnd(0x7F, 0xFF);
        sp44.unk44 = 0x80480000;
        sp44.unk48 = 0x440000;
        sp44.unk42 = 0x17C;
        break;
    case 0x331:
        sp44.unk3C = 0.08f;
        sp44.unk8 = 0x24;
        sp44.unk44 = 0x100200;
        sp44.unk42 = 0x27;
        sp44.unk58[0] = 0xFF00;
        sp44.unk58[1] = 0xFF00;
        sp44.unk58[2] = 0xFF00;
        sp44.unk4C[0] = 0;
        sp44.unk4C[1] = 0xFF00;
        sp44.unk4C[2] = 0;
        sp44.unk48 = 0x20;
        break;
    case 0x332:
        sp44.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk24.y = 0.01f;
        sp44.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk3C = 0.0052f;
        sp44.unk8 = 0x96;
        sp44.unk44 = 0x0A100100;
        sp44.unk42 = 0x62;
        break;
    case 0x333:
        sp44.unk30.x = transform->transl.x;
        sp44.unk30.y = transform->transl.y;
        sp44.unk30.z = transform->transl.z;
        sp44.unk3C = 0.01465f;
        sp44.unk60 = 0x40;
        sp44.unk61 = 0x10;
        sp44.unk44 = 0x80400212;
        sp44.unk42 = 0x4F9;
        sp44.unk8 = 0x64;
        break;
    case 0x334:
        sp44.unk30.x = transform->transl.x;
        sp44.unk30.y = transform->transl.y;
        sp44.unk30.z = transform->transl.z;
        sp44.unk3C = 0.0015f;
        sp44.unk8 = mathRnd(0x96, 0xC8);
        sp44.unk60 = mathRnd(0x7F, 0xFF);
        sp44.unk44 = 0x80480000;
        sp44.unk48 = 0x440000;
        sp44.unk42 = 0x156;
        break;
    case 0x335:
        sp44.unk30.x = (f32) mathRnd(-0x14, 0x14);
        sp44.unk30.y = -20.0f;
        sp44.unk30.z = (f32) mathRnd(-0x14, 0x14);
        sp44.unk3C = 0.005f;
        sp44.unk8 = 0x64;
        sp44.unk44 = 0x01080000;
        sp44.unk48 = 0x01400020;
        sp44.unk42 = 0x151;
        sp44.unk58[0] = 0xFFFF;
        sp44.unk58[1] = 0xFFFF;
        sp44.unk58[2] = 0;
        sp44.unk4C[0] = 0x7FF5;
        sp44.unk4C[1] = 0xFFFF;
        sp44.unk4C[2] = 0;
        break;
    case 0x336:
        if (data != NULL) {
            var_fv0 = *(f32*)data;
        } else {
            var_fv0 = 1.0f;
        }
        sp44.unk30.x = (f32) mathRnd(-0xA, 0xA) * var_fv0;
        sp44.unk30.y = (f32) mathRnd(-0xA, 0xA) * var_fv0;
        sp44.unk30.z = (f32) mathRnd(-0xA, 0xA) * var_fv0;
        sp44.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.02f * var_fv0;
        sp44.unk24.y = (f32) mathRnd(-0xF, 0xF) * 0.02f * var_fv0;
        sp44.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.02f * var_fv0;
        sp44.unk3C = (f32) mathRnd(8, 0xA) * 0.0004f;
        sp44.unk8 = 0x50;
        sp44.unk44 = 0x80480404;
        sp44.unk48 = 0x20;
        sp44.unk4C[0] = sp44.unk58[0] = 0;
        sp44.unk4C[0] = sp44.unk4C[1] = sp44.unk4C[2] = sp44.unk58[0] = sp44.unk58[1] = sp44.unk58[2] = 0;
        sp44.unk58[1] = 0;
        sp44.unk42 = 0x5C;
        break;
    case 0x337:
        if (data != NULL) {
            var_v0_2 = *(s32*)data;
        } else {
            var_v0_2 = 0;
        }
        if (var_v0_2 == 0) {
            sp44.unk8 = 1;
            sp44.unk44 = 0x480000;
            sp44.unk3C = 0.02f;
        } else if (var_v0_2 == 1) {
            sp44.unk8 = 1;
            sp44.unk3C = 0.04f;
            sp44.unk44 = 0x480000;
            sp44.unk60 = 0x32;
        } else if (var_v0_2 == 2) {
            sp44.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.02f;
            sp44.unk24.y = (f32) mathRnd(-0xF, 0xF) * 0.02f;
            sp44.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.02f;
            sp44.unk3C = 0.005f;
            sp44.unk8 = mathRnd(0x1E, 0x28);
            sp44.unk44 = 0x03000000;
            sp44.unk48 = 0x600000;
        } else if (var_v0_2 == 3) {
            sp44.unk30.x = (f32) mathRnd(-0xA, 0xA);
            sp44.unk30.y = (f32) mathRnd(-0xA, 0xA);
            sp44.unk30.z = (f32) mathRnd(-0xA, 0xA);
            sp44.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.02f;
            sp44.unk24.y = (f32) mathRnd(-0xF, 0xF) * 0.02f;
            sp44.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.02f;
            sp44.unk3C = (f32) mathRnd(8, 0xA) * 0.0004f;
            sp44.unk8 = 0x1E;
            sp44.unk60 = 0xB4;
            sp44.unk44 = 0x80480404;
        } else {
            sp44.unk30.x = (f32) mathRnd(-3, 3);
            sp44.unk30.y = (f32) mathRnd(-3, 3);
            sp44.unk30.z = (f32) mathRnd(-3, 3);
            sp44.unk3C = 0.003f;
            sp44.unk8 = 0x64;
            sp44.unk44 = 0x80480000;
            sp44.unk48 = 0x400000;
            sp44.unk60 = 0x7F;
        }
        sp44.unk42 = 0x17D;
        break;
    case 0x338:
        if (data != NULL) {
            var_v0_2 = *(s32*)data;
        } else {
            var_v0_2 = 0;
        }
        if (var_v0_2 == 0) {
            sp44.unk8 = 1;
            sp44.unk44 = 0x480000;
            sp44.unk3C = 0.02f;
        } else if (var_v0_2 == 1) {
            sp44.unk8 = 1;
            sp44.unk3C = 0.04f;
            sp44.unk44 = 0x480000;
            sp44.unk60 = 0x32;
        } else if (var_v0_2 == 2) {
            sp44.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.02f;
            sp44.unk24.y = (f32) mathRnd(-0xF, 0xF) * 0.02f;
            sp44.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.02f;
            sp44.unk3C = 0.005f;
            sp44.unk8 = mathRnd(0x1E, 0x28);
            sp44.unk44 = 0x03000000;
            sp44.unk48 = 0x600000;
        } else if (var_v0_2 == 3) {
            sp44.unk30.x = (f32) mathRnd(-0xA, 0xA);
            sp44.unk30.y = (f32) mathRnd(-0xA, 0xA);
            sp44.unk30.z = (f32) mathRnd(-0xA, 0xA);
            sp44.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.02f;
            sp44.unk24.y = (f32) mathRnd(-0xF, 0xF) * 0.02f;
            sp44.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.02f;
            sp44.unk3C = (f32) mathRnd(8, 0xA) * 0.0004f;
            sp44.unk8 = 0x1E;
            sp44.unk60 = 0xB4;
            sp44.unk44 = 0x80480404;
        } else {
            sp44.unk30.x = (f32) mathRnd(-3, 3);
            sp44.unk30.y = (f32) mathRnd(-3, 3);
            sp44.unk30.z = (f32) mathRnd(-3, 3);
            sp44.unk3C = 0.003f;
            sp44.unk8 = 0x64;
            sp44.unk44 = 0x80480000;
            sp44.unk48 = 0x400000;
            sp44.unk60 = 0x7F;
        }
        sp44.unk42 = 0x4F9;
        break;
    case 0x340:
        sp44.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.02f;
        sp44.unk24.y = (f32) mathRnd(0xA, 0xC8) * 0.02f;
        sp44.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.02f;
        sp44.unk3C = (f32) mathRnd(8, 0xB) * 0.0001f;
        sp44.unk8 = 0x4B;
        sp44.unk44 = 0x01080000;
        sp44.unk42 = 0x32;
        break;
    case 0x342:
        sp44.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.02f;
        sp44.unk24.y = (f32) mathRnd(0x14, 0x64) * 0.012f;
        sp44.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.02f;
        sp44.unk3C = 0.0015f;
        sp44.unk8 = 0x28;
        sp44.unk44 = 0x01080200;
        sp44.unk42 = 0x32;
        break;
    case 0x343:
        sp44.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.02f;
        sp44.unk24.y = (f32) mathRnd(0xA, 0xC8) * 0.02f;
        sp44.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.02f;
        sp44.unk3C = (f32) mathRnd(8, 0xB) * 0.00015f;
        sp44.unk8 = 0x4B;
        sp44.unk44 = 0x01080000;
        sp44.unk42 = 0x77;
        sp44.unk60 = 0x7F;
        break;
    case 0x344:
        sp44.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.02f;
        sp44.unk24.y = (f32) mathRnd(0x14, 0x64) * 0.012f;
        sp44.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.02f;
        sp44.unk3C = (f32) mathRnd(5, 0xA) * 0.00015f;
        sp44.unk8 = 0x28;
        sp44.unk44 = 0x01080200;
        sp44.unk42 = 0x77;
        sp44.unk60 = 0x7F;
        break;
    case 0x345:
        sp44.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.02f;
        sp44.unk24.y = (f32) mathRnd(0x14, 0x28) * 0.02f;
        sp44.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.02f;
        sp44.unk30.x = (f32) mathRnd(-5, 5);
        sp44.unk30.y = -4.0f;
        sp44.unk30.z = (f32) mathRnd(-5, 5);
        sp44.unk3C = 0.005f;
        sp44.unk8 = 0x28;
        sp44.unk44 = 0x01080200;
        sp44.unk42 = 0x60;
        sp44.unk60 = 0x5A;
        break;
    case 0x346:
        sp44.unk30.x = transform->transl.x;
        sp44.unk30.y = transform->transl.y;
        sp44.unk30.z = transform->transl.z;
        sp44.unk3C = ((f32) mathRnd(5, 0x19) * 0.001f) + transform->scale;
        sp44.unk8 = 0x1E0;
        sp44.unk61 = 0;
        sp44.unk44 = 0x480014;
        sp44.unk42 = 0xDF;
        break;
    case 0x347:
        sp44.unk24.x = (f32) mathRnd(-0x1E, 0x1E) * 0.02f;
        sp44.unk24.y = (f32) mathRnd(-5, 0xA) * 0.02f;
        sp44.unk24.z = (f32) mathRnd(-0x1E, 0x1E) * 0.02f;
        sp44.unk30.x = 0/*.0f*/;
        sp44.unk30.y = (f32) mathRnd(0xA, 0x1E);
        sp44.unk30.z = 0/*.0f*/;
        sp44.unk3C = 0.01f;
        sp44.unk8 = 0x32;
        sp44.unk44 = 0x8A000208;
        sp44.unk42 = 0x60;
        sp44.unk58[0] = 0x7F00;
        sp44.unk58[1] = 0x6400;
        sp44.unk58[2] = 0;
        sp44.unk4C[0] = 0x5A00;
        sp44.unk4C[1] = 0;
        sp44.unk4C[2] = 0;
        sp44.unk48 = 0x20;
        sp44.unk60 = 0x7F;
        break;
    case 0x34C:
        sp44.unk3C = 0.025f;
        sp44.unk8 = 0x32;
        sp44.unk44 = 0x180200;
        sp44.unk48 = 0x01000000;
        sp44.unk42 = 0x2B;
        sp44.unk60 = 0x9D;
        break;
    case 0x34D:
        sp44.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp44.unk24.y = (f32) mathRnd(0xA, 0x50) * 0.002f;
        sp44.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp44.unk3C = (f32) mathRnd(5, 0x19) * 0.0001f;
        sp44.unk8 = mathRnd(0x64, 0x78);
        sp44.transform.yaw = mathRnd(0, 0xFFFF);
        sp44.transform.pitch = mathRnd(0, 0xFFFF);
        sp44.transform.yaw = mathRnd(0, 0xFFFF);
        sp44.transform.transl.x = (f32) mathRnd(0xE6, 0x320);
        sp44.transform.transl.y = (f32) mathRnd(0xE6, 0x320);
        sp44.transform.transl.z = (f32) mathRnd(0xE6, 0x320);
        sp44.unk48 = 0x01000020;
        sp44.unk44 = 0x86000008;
        sp44.unk4C[0] = sp44.unk58[0] = mathRnd(0, 0x2EE0) + 0x3CAF;
        sp44.unk4C[1] = sp44.unk58[1] = (sp44.unk4C[0] - mathRnd(0, 0x2710));
        sp44.unk4C[2] = sp44.unk58[2] = (sp44.unk4C[0] - mathRnd(0x2710, 0x3CAF));
        sp44.unk42 = 0x3A3;
        break;
    case 0x34E:
        sp44.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp44.unk24.y = (f32) mathRnd(0xA, 0x50) * 0.002f;
        sp44.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp44.unk30.y = (f32) mathRnd(5, 0x1E);
        sp44.unk3C = (f32) mathRnd(5, 0x19) * 0.0001f;
        sp44.unk8 = mathRnd(0x64, 0x78);
        sp44.transform.yaw = mathRnd(0, 0xFFFF);
        sp44.transform.pitch = mathRnd(0, 0xFFFF);
        sp44.transform.yaw = mathRnd(0, 0xFFFF);
        sp44.transform.transl.x = (f32) mathRnd(0xE6, 0x320);
        sp44.transform.transl.y = (f32) mathRnd(0xE6, 0x320);
        sp44.transform.transl.z = (f32) mathRnd(0xE6, 0x320);
        sp44.unk48 = 0x01000020;
        sp44.unk44 = 0x86000008;
        sp44.unk4C[0] = sp44.unk58[0] = mathRnd(0, 0x2EE0) + 0x3CAF;
        sp44.unk4C[1] = sp44.unk58[1] = 0x7530;
        sp44.unk4C[2] = sp44.unk58[2] = (sp44.unk4C[0] - mathRnd(0x2710, 0x3CAF));
        sp44.unk42 = 0x3A3;
        break;
    default:
        // STUBBED_PRINTF("PARTFX badning type < %d > not found.. returning\n", id); // default.dol
        return -1;
    }
    sp44.unk44 |= flags;
    if ((sp44.unk44 & 1) && (sp44.unk44 & 2)) {
        // STUBBED_PRINTF("error in partfx ABS and REL %d\n", id); // default.dol
        sp44.unk44 ^= 2;
    }
    if (sp44.unk44 & 1) {
        if (flags & 0x200000) {
            sp44.unk30.x += sp44.transform.transl.x;
            sp44.unk30.y += sp44.transform.transl.y;
            sp44.unk30.z += sp44.transform.transl.z;
        } else if (sp44.unk0 != NULL) {
            sp44.unk30.x += sp44.unk0->globalPosition.x;
            sp44.unk30.y += sp44.unk0->globalPosition.y;
            sp44.unk30.z += sp44.unk0->globalPosition.z;
        }
    }
    return gDLL_13_Expgfx->vtbl->func1(&sp44, -1);
}
