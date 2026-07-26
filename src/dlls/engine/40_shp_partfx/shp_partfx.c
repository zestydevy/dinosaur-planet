#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "sys/objects.h"
#include "dll.h"

/** @file Ship battle partfx */

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
void shpPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void shpPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void shpPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void shpPartfx_Func_20(s32 arg0) {
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
s32 shpPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp4C;
    Object* temp_v0;
    SRT sp30;
    s32 _pad;
    s32 sp28;

    temp_v0 = objGetPlayer();
    sp28 = -1;
    data_40 += 0.001f;
    if (data_40 > 1.0f) {
        data_40 = 0.1f;
    }
    data_44 += 0.0003f;
    if (data_44 > 1.0f) {
        data_44 = 0.3f;
    }
    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX shpning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
        return -1;
    }
    if (flags & 0x200000) {
        if (transform == NULL) {
            // STUBBED_PRINTF("ERROR partfx EXF_NOOWNER & no attrib %d\n", id); // default.dol
            return -1;
        }
        sp4C.transform.transl.x = transform->transl.x;
        sp4C.transform.transl.y = transform->transl.y;
        sp4C.transform.transl.z = transform->transl.z;
        sp4C.transform.scale = transform->scale;
        sp4C.transform.roll = transform->roll;
        sp4C.transform.pitch = transform->pitch;
        sp4C.transform.yaw = transform->yaw;
        sp4C.unk62 = arg4;
    }
    sp4C.unk44 = 0;
    sp4C.unk48 = 0;
    sp4C.unk5E = id;
    sp4C.unk0 = obj;
    sp4C.unk8 = 0;
    sp4C.unk4 = -1;
    sp4C.unk60 = 0xFF;
    sp4C.unk61 = 0;
    sp4C.unk42 = 0;
    sp4C.unk30.x = 0.0f;
    sp4C.unk30.y = 0.0f;
    sp4C.unk30.z = 0.0f;
    sp4C.unk24.x = 0.0f;
    sp4C.unk24.y = 0.0f;
    sp4C.unk24.z = 0.0f;
    sp4C.unk3C = 0.0f;
    sp4C.unk58[0] = 0xFFFF;
    sp4C.unk58[1] = 0xFFFF;
    sp4C.unk58[2] = 0xFFFF;
    sp4C.unk4C[0] = 0xFFFF;
    sp4C.unk4C[1] = 0xFFFF;
    sp4C.unk4C[2] = 0xFFFF;
    sp4C.unk40 = 0;
    switch (id) {
    case 0xAE:
        sp4C.unk24.x = (f32) mathRnd(-0x1E, 0x1E) * 0.0004f;
        sp4C.unk24.z = (f32) mathRnd(0x1E, 0x28) * -0.0025f;
        sp4C.unk24.y = (f32) mathRnd(-0x1E, 0x1E) * 0.0004f;
        sp4C.unk3C = (f32) mathRnd(0x1E, 0x50) * 0.00007f;
        sp4C.unk8 = 0x46;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x100200;
        sp4C.unk42 = 0x88;
        break;
    case 0xAF:
        sp4C.unk24.x = (f32) mathRnd(-0x1E, 0x1E) * 0.0024f;
        sp4C.unk24.z = (f32) mathRnd(0x1E, 0x28) * -0.0025f;
        sp4C.unk24.y = (f32) mathRnd(-0x1E, 0x1E) * 0.0024f;
        sp4C.unk3C = (f32) mathRnd(0x3C, 0x50) * 0.0000042f;
        sp4C.unk8 = 0x46;
        sp4C.unk60 = 0x9B;
        sp4C.unk44 = 0x400000;
        sp4C.unk48 = 8;
        sp4C.unk42 = 0xE4;
        break;
    case 0xAD:
        sp4C.unk24.x = (f32) mathRnd(-0x1E, 0x1E) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(6, 0x16) * 0.09f;
        sp4C.unk24.z = (f32) mathRnd(-0x1E, 0x1E) * 0.01f;
        sp4C.unk30.x = (f32) mathRnd(-0x32, 0x32) * 0.1f;
        sp4C.unk30.y = 0.0f;
        sp4C.unk30.z = (f32) mathRnd(-0x32, 0x32) * 0.1f;
        sp4C.unk8 = 0x91;
        sp4C.unk60 = 0xFF;
        sp4C.unk58[0] = 0xFFFF;
        sp4C.unk3C = 0.0042f;
        sp4C.unk58[1] = mathRnd(0, 0x2710) + 0x3CAF;
        sp4C.unk58[2] = 0x3CAF;
        sp4C.unk4C[0] = 0xF52F;
        sp4C.unk4C[1] = 0xF52F;
        sp4C.unk4C[2] = 0xF52F;
        sp4C.unk44 = 0x03000020;
        sp4C.unk48 = 0x02600020;
        sp4C.unk42 = 0xE4;
        break;
    case 0xAC:
        sp4C.unk30.x = (f32) mathRnd(-0x32, 0x32) * 0.1f;
        sp4C.unk30.y = 0.0f;
        sp4C.unk30.z = (f32) mathRnd(-0x32, 0x32) * 0.1f;
        sp4C.unk24.x = (f32) mathRnd(-8, 8) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(9, 0xC) * 0.03f;
        sp4C.unk24.z = (f32) mathRnd(-8, 8) * 0.01f;
        sp4C.unk3C = (f32) mathRnd(0xA, 0xF) * 0.002f;
        sp4C.unk8 = mathRnd(0, 0x14) + 0x5F;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0x60;
        sp4C.unk58[0] = 0x3CAF;
        sp4C.unk58[1] = 0x3CAF;
        sp4C.unk58[2] = 0x3CAF;
        sp4C.unk4C[0] = 0xA70F;
        sp4C.unk4C[1] = 0xA70F;
        sp4C.unk4C[2] = 0xA70F;
        sp4C.unk61 = 0;
        sp4C.unk44 = 0x80180100;
        sp4C.unk48 = 0x20;
        break;
    case 0x84:
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.015f;
        sp4C.unk24.y = (f32) mathRnd(4, 0xA) * 0.01f;
        sp4C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.025f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.0005f;
        sp4C.unk8 = 0x46;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x01400211;
        sp4C.unk42 = 0xDF;
        break;
    case 0x85:
        if (data == NULL) {
            return 0;
        }
        sp4C.unk30.x = temp_v0->globalPosition.x;
        sp4C.unk30.y = temp_v0->globalPosition.y;
        sp4C.unk30.z = temp_v0->globalPosition.z;
        sp4C.unk8 = 0x28;
        sp4C.unk60 = 0xFF;
        sp4C.unk3C = 4.55f;
        sp4C.unk44 = 0x110;
        sp4C.unk42 = transform->roll + 0x170;
        break;
    case 0x8A:
        sp4C.unk30.x = -750.0f;
        sp4C.unk30.y = (f32) mathRnd(-0x64, 0x64);
        sp4C.unk30.z = (f32) mathRnd(-0x64, 0x64);
        sp4C.unk24.x = 6.0f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.0015f;
        sp4C.unk8 = 0x10E;
        sp4C.unk61 = 0x10;
        sp4C.unk60 = 0xF;
        sp4C.unk44 = 0x02000011;
        sp4C.unk42 = 0x5F;
        break;
    case 0x8B:
        sp4C.unk30.x = (f32) mathRnd(-0x78, 0x78);
        sp4C.unk30.y = (f32) mathRnd(-0x78, 0x78);
        sp4C.unk30.z = (f32) mathRnd(-0x78, 0x78);
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.035f;
        sp4C.unk24.y = (f32) mathRnd(4, 0xA) * 0.035f;
        sp4C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.035f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.00055f;
        sp4C.unk8 = 0x46;
        sp4C.unk60 = 0xFF;
        sp4C.unk4 = 0x378;
        sp4C.unk44 = 0x80000119;
        sp4C.unk42 = 0x125;
        break;
    case 0x8C:
        if (transform == NULL) {
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.scale = 1.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
        }
        sp4C.unk30.x = 0.0f;
        sp4C.unk30.y = -9.0f;
        sp4C.unk30.z = -2.0f;
        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.scale = 1.0f;
        sp30.roll = obj->srt.roll;
        sp30.pitch = 0;
        sp30.yaw = 0;
        mathRotateRPY(&sp30, sp4C.unk30.f);
        sp4C.unk8 = 0x5A;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x40200;
        sp4C.unk42 = 0x65;
        sp4C.unk3C = 0.0022f;
        break;
    case 0x8D:
        if (transform == NULL) {
            transform = &bss_0;
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.scale = 1.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
        }
        if (transform != NULL) {
            sp4C.unk30.x = transform->transl.x;
            sp4C.unk30.y = transform->transl.y;
            sp4C.unk30.z = transform->transl.z;
        } else {
            sp4C.unk30.x = 0.0f;
            sp4C.unk30.y = -9.0f;
            sp4C.unk30.z = -2.0f;
        }
        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.scale = 1.0f;
        sp30.roll = obj->srt.roll;
        sp30.pitch = 0;
        sp30.yaw = 0;
        mathRotateRPY(&sp30, sp4C.unk30.f);
        sp4C.unk8 = 0xA;
        sp4C.unk60 = 0x41;
        sp4C.unk44 = 0x80110;
        sp4C.unk42 = 0x65;
        sp4C.unk3C = 0.03f;
        break;
    case 0x8E:
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.005f;
        sp4C.unk24.y = (f32) mathRnd(-0x28, 0x28) * 0.005f;
        sp4C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.005f;
        sp4C.unk8 = 0x50;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x100110;
        sp4C.unk42 = 0x30;
        sp4C.unk3C = 0.005f;
        break;
    case 0x8F:
        sp4C.unk30.x = (f32) mathRnd(-6, 6);
        sp4C.unk30.y = (f32) mathRnd(-6, 6);
        sp4C.unk30.z = (f32) mathRnd(-6, 6);
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.025f;
        sp4C.unk24.y = (f32) mathRnd(-0x28, 0x28) * 0.025f;
        sp4C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.025f;
        if (mathRnd(0, 0xC) == 0) {
            sp4C.unk3C = (f32) mathRnd(0xF, 0x1E) * 0.00155f;
            sp4C.unk60 = 0x5F;
        } else {
            sp4C.unk3C = (f32) mathRnd(0xF, 0x1E) * 0.000115f;
            sp4C.unk60 = 0xFF;
        }
        sp4C.unk8 = 0x1E;
        sp4C.unk44 = 0x400108;
        sp4C.unk42 = 0x33;
        break;
    case 0x9A:
        sp4C.unk30.x = 100.0f;
        sp4C.unk30.y = (f32) mathRnd(-0x42, 0x42) + 135.0f;
        sp4C.unk30.z = (f32) mathRnd(-0x42, 0x42);
        sp4C.unk3C = (f32) mathRnd(1, 0xA) * 0.01f;
        sp4C.unk8 = mathRnd(0x50, 0x78);
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x100210;
        sp4C.unk42 = 0x125;
        sp4C.unk61 = 5;
        break;
    case 0x9B:
        sp4C.unk30.x = (f32) mathRnd(-0x42, 0x42);
        sp4C.unk30.y = 135.0f - (f32) mathRnd(0, 0x42);
        sp4C.unk30.z = (f32) mathRnd(-0x60, 0x60);
        sp4C.unk24.y = (f32) mathRnd(0, 0x28) * 0.055f;
        sp4C.unk3C = (f32) mathRnd(0xA, 0x28) * 0.006f;
        sp4C.unk8 = mathRnd(0, 0x1E) + 0x1E;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x100200;
        sp4C.unk42 = 0x125;
        break;
    case 0x9C:
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.055f;
        sp4C.unk24.y = (f32) mathRnd(-0x28, 0x28) * 0.055f;
        sp4C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.055f;
        sp4C.unk8 = 0x1E;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x110;
        sp4C.unk42 = 0xDD;
        sp4C.unk3C = 0.0035f;
        break;
    case 0x9D:
        sp4C.unk30.x = (f32) mathRnd(-0x14, 0x14);
        sp4C.unk30.y = (f32) mathRnd(-0x14, 0x14);
        sp4C.unk30.z = (f32) mathRnd(-0x14, 0x14);
        sp4C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.0065f;
        sp4C.unk8 = 0x3C;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x200;
        sp4C.unk42 = 0x185;
        break;
    case 0x9E:
        sp4C.unk30.x = (f32) mathRnd(-0x14, 0x14);
        sp4C.unk30.y = (f32) mathRnd(0, 0x28);
        sp4C.unk30.z = (f32) mathRnd(-0x14, 0x14);
        sp4C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.0065f;
        sp4C.unk8 = 0x3C;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x100;
        sp4C.unk42 = 0x17E;
        break;
    case 0x9F:
        if (transform == NULL) {
            transform = &bss_0;
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.scale = 1.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
        }
        sp4C.unk30.x = (f32) mathRnd(-0x50, 0x50) * 0.1f;
        sp4C.unk30.y = (f32) mathRnd(-0x50, 0x50) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0x50, 0x50) * 0.1f;
        if (transform != NULL) {
            sp4C.unk30.x += transform->transl.x;
            sp4C.unk30.y += transform->transl.y;
            sp4C.unk30.z += transform->transl.z;
        }
        sp4C.unk3C = (f32) mathRnd(1, 4) * 0.01f;
        sp4C.unk8 = 0x19;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x200;
        sp4C.unk48 = 0x80;
        sp4C.unk42 = 0x16D;
        break;
    case 0xA0:
        if (transform == NULL) {
            transform = &bss_0;
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.scale = 1.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
        }
        sp4C.unk30.x = (f32) mathRnd(-0x14, -0xA) * 0.1f;
        sp4C.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0xA, 0) * 0.1f;
        sp4C.unk60 = 0xFF;
        if (transform != NULL) {
            sp4C.unk30.x += transform->transl.x;
            sp4C.unk30.y += transform->transl.y;
            sp4C.unk30.z += transform->transl.z;
            if (transform->scale != 1.0f) {
                sp4C.unk60 = ((s32) (transform->scale * 255.0f)) & 0xFF & 0xFF;
            }
        }
        sp4C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.00018f;
        sp4C.unk8 = 0x2D;
        sp4C.unk44 = 0x200;
        sp4C.unk42 = 0x125;
        sp4C.unk61 = mathRnd(0, 0x14) + 4;
        break;
    case 0xA1:
        if (transform == NULL) {
            transform = &bss_0;
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.scale = 1.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
        }
        sp4C.unk30.x = (f32) mathRnd(-0x32, 0x32) * 1.1f;
        sp4C.unk30.y = (f32) mathRnd(-0x32, 0x32) * 1.1f;
        sp4C.unk30.z = (f32) mathRnd(-0x32, 0x32) * 1.1f;
        sp4C.unk24.x = transform->transl.x;
        sp4C.unk24.y = transform->transl.y;
        sp4C.unk24.z = transform->transl.z;
        sp4C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.00028f;
        sp4C.unk8 = 0x4B;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x81000210;
        sp4C.unk42 = 0x125;
        sp4C.unk61 = 0;
        break;
    case 0xA3:
        sp4C.unk30.x = (f32) mathRnd(-0x32, 0x32) * 0.3f;
        sp4C.unk30.y = (f32) mathRnd(-0x32, 0x32) * 0.3f;
        sp4C.unk30.z = (f32) mathRnd(-0x32, 0x32) * 0.3f;
        sp4C.unk24.x = (f32) mathRnd(8, 0x12) * 0.1f;
        sp4C.unk24.y = (f32) mathRnd(1, 0xF) * 0.1f;
        sp4C.unk24.z = 0.0f;
        sp4C.unk3C = (f32) mathRnd(0xA, 0xF) * 0.01f;
        sp4C.unk8 = 0x7D;
        sp4C.unk60 = 0xCD;
        sp4C.unk61 = mathRnd(0, 0xA) + 0xA;
        sp4C.unk42 = 0x184;
        sp4C.unk44 = 0x180100;
        sp28 = 0x13;
        break;
    case 0xA4:
        sp4C.unk30.x = (f32) mathRnd(-0x32, 0x32) * 0.1f;
        sp4C.unk30.y = 0.0f;
        sp4C.unk30.z = (f32) mathRnd(-0x32, 0x32) * 0.1f;
        sp4C.unk24.x = (f32) mathRnd(8, 0x12) * 0.03f;
        sp4C.unk24.y = (f32) mathRnd(1, 0xF) * 0.03f;
        sp4C.unk24.z = 0.0f;
        sp4C.unk3C = (f32) mathRnd(0xA, 0xF) * 0.005f;
        sp4C.unk8 = 0x5F;
        sp4C.unk60 = 0x5F;
        sp4C.unk61 = mathRnd(0, 0xA) + 0xA;
        sp4C.unk42 = 0x1A1;
        sp4C.unk44 = 0x180100;
        break;
    case 0xA5:
        sp4C.unk8 = 6;
        sp4C.unk60 = 0xFF;
        sp4C.unk61 = 0;
        sp4C.unk3C = 0.0023f;
        sp4C.unk42 = mathRnd(0, 2) + 0x156;
        sp4C.unk44 = 0x180010;
        break;
    case 0xA6:
        sp4C.unk30.y = 100.0f;
        sp4C.unk24.x = (f32) mathRnd(8, 0x12) * 0.15f;
        sp4C.unk24.y = (f32) mathRnd(8, 0xF) * 0.06f;
        sp4C.unk24.z = (f32) mathRnd(-0x1E, 0x1E) * 0.01f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.00005f;
        sp4C.unk8 = 0x244;
        sp4C.unk61 = 0;
        sp4C.unk44 = 0x80480208;
        sp4C.unk42 = 0x47;
        break;
    case 0xA7:
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.002f;
        sp4C.unk8 = mathRnd(0, 0xA) + 0xA;
        sp4C.unk61 = 0;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x180210;
        sp4C.unk42 = 0x159;
        break;
    case 0xA8:
        sp4C.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.5f;
        sp4C.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.5f;
        sp4C.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.5f;
        sp4C.unk8 = mathRnd(0, 0x14) + 0x1E;
        sp4C.unk60 = 0xA5;
        sp4C.unk61 = 0;
        sp4C.unk44 = 0x180100;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.0006f;
        sp4C.unk42 = 0x167;
        break;
    case 0xA9:
        sp4C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.05f;
        sp4C.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.05f;
        sp4C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.05f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.000032f;
        sp4C.unk8 = 0x50;
        sp4C.unk61 = 0;
        sp4C.unk44 = 0x480208;
        sp4C.unk48 = 0x100000;
        if (mathRnd(0, 1) != 0) {
            sp4C.unk42 = 0x47;
        } else {
            sp4C.unk42 = 0x33;
            sp4C.unk5E = 0xA2;
        }
        break;
    case 0xAA:
        sp4C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.07f;
        sp4C.unk24.y = (f32) mathRnd(-0xA, 0xA) * 0.07f;
        sp4C.unk24.z = (f32) mathRnd(-0x1E, 0x1E) * 0.07f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.00004f;
        sp4C.unk8 = 0x78;
        sp4C.unk61 = 0;
        sp4C.unk44 = 0x480201;
        if (mathRnd(0, 1) != 0) {
            sp4C.unk42 = 0x156;
        } else {
            sp4C.unk42 = 0x159;
        }
        break;
    case 0xAB:
        sp4C.unk30.x = (f32) mathRnd(-0x1E, 0x1E);
        sp4C.unk30.y = (f32) mathRnd(-0x1E, 0x1E);
        sp4C.unk30.z = (f32) mathRnd(-0x1E, 0x1E);
        sp4C.unk3C = (f32) mathRnd(1, 7) * 0.03f;
        sp4C.unk8 = mathRnd(0, 0x1E) + 0x1E;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x100200;
        sp4C.unk42 = 0x125;
        break;
    default:
        // STUBBED_PRINTF("PARTFX shpning type < %d > not found.. returning\n", id); // default.dol
        return -1;
    }
    sp4C.unk44 |= flags;
    if ((sp4C.unk44 & 1) && (sp4C.unk44 & 2)) {
        // STUBBED_PRINTF("error in partfx ABS and REL %d\n", id); // default.dol
        sp4C.unk44 ^= 2;
    }
    if (sp4C.unk44 & 1) {
        if (flags & 0x200000) {
            sp4C.unk30.x += sp4C.transform.transl.x;
            sp4C.unk30.y += sp4C.transform.transl.y;
            sp4C.unk30.z += sp4C.transform.transl.z;
        } else if (sp4C.unk0 != NULL) {
            sp4C.unk30.x += sp4C.unk0->globalPosition.x;
            sp4C.unk30.y += sp4C.unk0->globalPosition.y;
            sp4C.unk30.z += sp4C.unk0->globalPosition.z;
        }
    }
    return gDLL_13_Expgfx->vtbl->func1(&sp4C, sp28);
}
