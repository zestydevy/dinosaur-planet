#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/** @file CloudRunner Fortress partfx */

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
void clfPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void clfPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void clfPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void clfPartfx_Func_20(s32 arg0) {
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
s32 clfPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp34;
    s32 _pad;
    s32 _pad2;

    data_40 += 0.001f;
    if (data_40 > 1.0f) {
        data_40 = 0.1f;
    }
    data_44 += 0.0003f;
    if (data_44 > 1.0f) {
        data_44 = 0.3f;
    }
    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX clfning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
        return -1;
    }
    if (flags & 0x200000) {
        if (transform == NULL) {
            // STUBBED_PRINTF("ERROR partfx EXF_NOOWNER & no attrib %d\n", id); // default.dol
            return -1;
        }
        sp34.transform.transl.x = transform->transl.x;
        sp34.transform.transl.y = transform->transl.y;
        sp34.transform.transl.z = transform->transl.z;
        sp34.transform.scale = transform->scale;
        sp34.transform.roll = transform->roll;
        sp34.transform.pitch = transform->pitch;
        sp34.transform.yaw = transform->yaw;
        sp34.unk62 = arg4;
    }
    sp34.unk44 = 0;
    sp34.unk48 = 0;
    sp34.unk5E = (u8) id;
    sp34.unk0 = obj;
    sp34.unk8 = 0;
    sp34.unk4 = -1;
    sp34.unk60 = 0xFF;
    sp34.unk61 = 0;
    sp34.unk42 = 0;
    sp34.unk30.x = 0.0f;
    sp34.unk30.y = 0.0f;
    sp34.unk30.z = 0.0f;
    sp34.unk24.x = 0.0f;
    sp34.unk24.y = 0.0f;
    sp34.unk24.z = 0.0f;
    sp34.unk3C = 0.0f;
    sp34.unk58[0] = 0xFFFF;
    sp34.unk58[1] = 0xFFFF;
    sp34.unk58[2] = 0xFFFF;
    sp34.unk4C[0] = 0xFFFF;
    sp34.unk4C[1] = 0xFFFF;
    sp34.unk4C[2] = 0xFFFF;
    sp34.unk40 = 0;
    switch (id) {
    case 0x361:
        sp34.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp34.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp34.unk30.x = (f32) mathRnd(-0x32, 0x32);
        sp34.unk30.z = (f32) mathRnd(-0x32, 0x32);
        sp34.unk8 = 0x258;
        sp34.unk60 = 0xC8;
        sp34.unk44 = 0x0A100100;
        sp34.unk42 = 0x62;
        sp34.unk3C = 0.1f;
        break;
    case 0x362:
        sp34.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp34.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp34.unk30.x = (f32) mathRnd(-0x32, 0x32);
        sp34.unk30.z = (f32) mathRnd(-0xA, 0xA);
        sp34.unk8 = 0x258;
        sp34.unk60 = 0xC8;
        sp34.unk44 = 0x0A100100;
        sp34.unk42 = 0x62;
        sp34.unk3C = 0.1f;
        break;
    case 0x35F:
        sp34.unk30.x = (f32) mathRnd(-0x64, 0x64) * 0.2f;
        sp34.unk30.z = (f32) mathRnd(-0x64, 0x64) * 0.2f;
        sp34.unk30.y = (f32) mathRnd(-0xA, 0x78) * 0.2f;
        sp34.unk24.y = (f32) mathRnd(2, 0x64) * 0.002f;
        sp34.unk8 = 0x3C;
        sp34.unk60 = 0x9B;
        sp34.unk44 = 0x180201;
        sp34.unk42 = 0x5F;
        sp34.unk58[0] = 0xFF00;
        sp34.unk58[1] = 0xFF00;
        sp34.unk58[2] = 0x9B00;
        sp34.unk4C[0] = 0x9600;
        sp34.unk4C[1] = 0x1400;
        sp34.unk4C[2] = 0x1400;
        sp34.unk48 = 0x20;
        sp34.unk3C = 0.1f;
        break;
    case 0x360:
        sp34.unk30.x = (f32) mathRnd(-0x1E, 0x1E);
        sp34.unk30.z = (f32) mathRnd(-0x1E, 0x1E);
        sp34.unk30.y = (f32) mathRnd(0x1E, 0x28) + 70.0f;
        sp34.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.006f;
        sp34.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.006f;
        sp34.unk24.y = (f32) mathRnd(0, 0x64) * 0.005f;
        sp34.unk3C = (f32) mathRnd(0x14, 0x50) * 0.00005f;
        sp34.unk8 = mathRnd(0, 0x118) + 0xB4;
        sp34.unk60 = 0xFE;
        sp34.unk44 = 0x81008000;
        sp34.unk4 = 0x284;
        sp34.unk42 = 0x208;
        break;
    case 0x357:
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
        if (transform == NULL) {
            return -1;
        }
        sp34.unk58[0] = (u8)transform->roll << 8;
        sp34.unk58[1] = (u8)transform->pitch << 8;
        sp34.unk58[2] = (u8)transform->yaw << 8;
        sp34.unk4C[0] = 0xFE00;
        sp34.unk4C[1] = 0xFE00;
        sp34.unk4C[2] = 0xFE00;
        sp34.unk8 = 0x1E;
        sp34.unk60 = 0x78;
        sp34.unk44 = 0x08000201;
        sp34.unk48 = 0x20;
        sp34.unk42 = 0x71;
        sp34.unk3C = 0.009f;
        break;
    case 0x359:
        sp34.unk30.x = (f32) mathRnd(-0x1E, 0x1E);
        sp34.unk30.z = (f32) mathRnd(-0x1E, 0x1E);
        sp34.unk30.y = (f32) mathRnd(0x1E, 0x28) + 70.0f;
        sp34.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.006f;
        sp34.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.006f;
        sp34.unk24.y = (f32) mathRnd(0, 0x64) * 0.005f;
        sp34.unk3C = (f32) mathRnd(0x14, 0x50) * 0.00005f;
        sp34.unk8 = mathRnd(0, 0x118) + 0xB4;
        sp34.unk60 = 0xFE;
        sp34.unk44 = 0x81008000;
        sp34.unk4 = 0x284;
        sp34.unk42 = 0x208;
        break;
    case 0x352:
        sp34.unk8 = 0x64;
        sp34.unk61 = 0;
        sp34.unk44 = 0x0A100208;
        sp34.unk42 = 0x91;
        sp34.unk3C = 0.6f;
        break;
    case 0x353:
        sp34.unk30.x = (f32) mathRnd(-2, 2);
        sp34.unk30.z = (f32) mathRnd(-2, 2);
        sp34.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.0025f;
        sp34.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.0025f;
        sp34.unk24.y = (f32) mathRnd(0, 0x50) * 0.01f;
        sp34.unk3C = (f32) mathRnd(0x28, 0x50) * 0.00003f;
        sp34.unk8 = mathRnd(0, 0x17C) + 0xB4;
        sp34.unk60 = 0xFF;
        sp34.unk44 = 0x80400109;
        sp34.unk42 = 0x47;
        break;
    case 0x354:
        sp34.unk30.x = (f32) mathRnd(-4, 4);
        sp34.unk30.z = (f32) mathRnd(-4, 4);
        sp34.unk30.y = (f32) mathRnd(0xA, 0x14);
        sp34.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.006f;
        sp34.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.006f;
        sp34.unk24.y = (f32) mathRnd(0, 0x64) * 0.005f;
        sp34.unk3C = (f32) mathRnd(0x14, 0x50) * 0.00005f;
        sp34.unk8 = mathRnd(0, 0x118) + 0xB4;
        sp34.unk60 = 0xFE;
        sp34.unk44 = 0x01000001;
        sp34.unk4 = 0x284;
        sp34.unk42 = 0x208;
        break;
    case 0x355:
        sp34.unk3C = 0.1f;
        sp34.unk8 = 0x46;
        sp34.unk60 = 0xFF;
        sp34.unk44 = 0x580101;
        sp34.unk42 = 0x17C;
        break;
    case 0x356:
        sp34.unk8 = 0x96;
        sp34.unk60 = 0xFF;
        sp34.unk3C = 0.005f;
        sp34.unk24.y = (f32) mathRnd(0, 0x14) * -0.0001f;
        sp34.unk44 = 0x80201;
        sp34.unk42 = 0x62;
        break;
    case 0x35A:
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
        if (transform == NULL) {
            return -1;
        }
        sp34.unk30.x = transform->transl.x;
        sp34.unk30.y = transform->transl.y;
        sp34.unk30.z = transform->transl.z;
        sp34.unk3C = (f32) transform->roll * 0.007f * 0.01f;
        sp34.unk8 = 0x3C;
        sp34.unk58[0] = 0xFF00;
        sp34.unk58[1] = 0xFF00;
        sp34.unk58[2] = 0xFF00;
        sp34.unk4C[0] = transform->roll << 8;
        sp34.unk4C[1] = transform->roll << 8;
        sp34.unk4C[2] = 0xFF00;
        sp34.unk48 = 0x60;
        sp34.unk60 = (u8) transform->roll;
        sp34.unk44 = 0x201;
        sp34.unk42 = 0x76;
        break;
    case 0x35B:
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
        sp34.unk30.x = transform->transl.x;
        sp34.unk30.y = transform->transl.y;
        sp34.unk30.z = transform->transl.z;
        sp34.unk3C = 0.1f;
        sp34.unk8 = 0xA;
        sp34.unk60 = 0xFF;
        sp34.unk44 = 0x580101;
        sp34.unk42 = 0xDC;
        break;
    case 0x35C:
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
        if (transform == NULL) {
            return -1;
        }
        sp34.unk30.x = transform->transl.x;
        sp34.unk30.y = transform->transl.y;
        sp34.unk30.z = transform->transl.z;
        sp34.unk3C = ((f32) transform->yaw + 255.0f) * 0.006f * 0.003f;
        sp34.unk8 = 0xA;
        sp34.unk58[0] = transform->yaw << 8;
        sp34.unk58[1] = transform->yaw << 8;
        sp34.unk58[2] = 0xFF00;
        sp34.unk4C[0] = transform->yaw << 8;
        sp34.unk4C[1] = transform->yaw << 8;
        sp34.unk4C[2] = 0xFF00;
        sp34.unk48 = 0x20;
        sp34.unk60 = (u8) transform->roll;
        sp34.unk42 = 0x5C;
        break;
    case 0x35D:
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
        if (transform == NULL) {
            return -1;
        }
        sp34.unk30.x = transform->transl.x;
        sp34.unk30.y = transform->transl.y;
        sp34.unk30.z = transform->transl.z;
        sp34.unk3C = ((f32) transform->yaw + 255.0f) * 0.006f * 0.003f;
        sp34.unk8 = 0xA;
        sp34.unk58[0] = 0xFF00;
        sp34.unk58[1] = transform->yaw << 8;
        sp34.unk58[2] = 0xFF00;
        sp34.unk4C[0] = 0xFF00;
        sp34.unk4C[1] = transform->yaw << 8;
        sp34.unk4C[2] = 0xFF00;
        sp34.unk48 = 0x20;
        sp34.unk60 = (u8) transform->roll;
        sp34.unk42 = 0x5C;
        break;
    case 0x35E:
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
        sp34.unk8 = 0x46;
        sp34.unk3C = 0.06f;
        sp34.unk30.y = 7.0f;
        if (transform != NULL) {
            sp34.unk60 = (u8) transform->roll;
        } else {
            sp34.unk60 = 0xFF;
        }
        sp34.unk61 = 0;
        if (transform != NULL) {
            sp34.unk30.x = transform->transl.x;
        } else {
            sp34.unk30.x = 0.0f;
        }
        if (transform != NULL) {
            sp34.unk30.y = transform->transl.y;
        } else {
            sp34.unk30.y = 0.0f;
        }
        if (transform != NULL) {
            sp34.unk30.z = transform->transl.z;
        } else {
            sp34.unk30.z = 0.0f;
        }
        sp34.unk44 = 0x0A100200;
        sp34.unk42 = 0x7D;
        break;
    case 0x367:
        sp34.unk30.x = (f32) mathRnd(-0x190, 0x190) * 0.1f;
        sp34.unk30.y = 0.9f;
        sp34.unk30.z = (f32) mathRnd(-0x190, 0x190) * 0.1f;
        sp34.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp34.unk24.y = (f32) mathRnd(0x64, 0xC8) * 0.006f;
        sp34.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp34.unk3C = (f32) mathRnd(5, 0x19) * 0.0001f;
        sp34.unk8 = 0x7D0;
        sp34.unk60 = 0xE6;
        sp34.transform.yaw = mathRnd(0, 0xFFFF);
        sp34.transform.pitch = mathRnd(0, 0xFFFF);
        sp34.transform.yaw = mathRnd(0, 0xFFFF);
        sp34.transform.transl.x = (f32) mathRnd(0xE6, 0x320);
        sp34.transform.transl.y = (f32) mathRnd(0xE6, 0x320);
        sp34.transform.transl.z = (f32) mathRnd(0xE6, 0x320);
        sp34.unk48 = 0x10000000;
        sp34.unk44 = 0x8F000000;
        sp34.unk42 = 0x56E;
        break;
    case 0x366:
        sp34.unk24.y = (f32) mathRnd(0x1F4, 0x3E8) * 0.01f;
        sp34.unk30.z = (f32) mathRnd(-0x12C, 0x12C) * 0.1f;
        sp34.unk30.x = (f32) mathRnd(-0x12C, 0x12C) * 0.1f;
        sp34.unk8 = 0x3C;
        sp34.unk44 = 0x400000;
        sp34.unk48 = 0x100;
        sp34.unk42 = 0x62;
        sp34.unk60 = 0x50;
        sp34.unk30.y = 50.0f;
        sp34.unk3C = 0.01f;
        break;
    case 0x365:
        sp34.unk24.y = (f32) mathRnd(0x6E, 0xC8) * 0.008f;
        sp34.unk30.z = (f32) mathRnd(-0x12C, 0x12C) * 0.1f;
        sp34.unk30.x = (f32) mathRnd(-0x12C, 0x12C) * 0.1f;
        sp34.unk3C = ((f32) mathRnd(1, 0x14) * 0.00015f) + 0.001f;
        sp34.unk60 = 0xFF;
        sp34.transform.yaw = mathRnd(0, 0xFFFF);
        sp34.transform.pitch = mathRnd(0, 0xFFFF);
        sp34.transform.yaw = mathRnd(0, 0xFFFF);
        sp34.transform.transl.x = (f32) mathRnd(0, 0x258);
        sp34.transform.transl.y = (f32) mathRnd(0, 0x258);
        sp34.transform.transl.z = (f32) mathRnd(0, 0x258);
        sp34.unk58[0] = mathRnd(0, 0x9C40) + 0x63BF;
        sp34.unk58[1] = mathRnd(0, 0x9C40) + 0x3CAF;
        sp34.unk58[2] = 0x3CAF;
        sp34.unk4C[0] = sp34.unk58[0];
        sp34.unk4C[1] = sp34.unk58[1];
        sp34.unk4C[2] = sp34.unk58[2];
        sp34.unk48 = 0x20;
        sp34.unk8 = mathRnd(0, 0x3C) + 0x15E;
        sp34.unk61 = 0x10;
        sp34.unk44 = 0x86000008;
        sp34.unk42 = 0x3A2;
        break;
    case 0x364:
        sp34.unk24.y = (f32) mathRnd(5, 0x64) * 0.01f;
        sp34.unk8 = 0x50;
        sp34.unk3C = 0.02f;
        sp34.unk58[0] = mathRnd(0, 0x2710) + 0x63BF;
        sp34.unk58[1] = mathRnd(0, 0x2710) + 0x3CAF;
        sp34.unk58[2] = 0x3CAF;
        sp34.unk4C[0] = sp34.unk58[0];
        sp34.unk4C[1] = sp34.unk58[1];
        sp34.unk4C[2] = sp34.unk58[2];
        sp34.unk48 = 0x20;
        sp34.unk44 = 0x80080100;
        sp34.unk42 = 0x62;
        sp34.unk60 = 0xA0;
        break;
    default:
        // STUBBED_PRINTF("PARTFX clfning type < %d > not found.. returning\n", id); // default.dol
        return -1;
    }
    sp34.unk44 |= flags;
    if ((sp34.unk44 & 1) && (sp34.unk44 & 2)) {
        // STUBBED_PRINTF("error in partfx ABS and REL %d\n", id); // default.dol
        sp34.unk44 ^= 2;
    }
    if (sp34.unk44 & 1) {
        if (flags & 0x200000) {
            sp34.unk30.x += sp34.transform.transl.x;
            sp34.unk30.y += sp34.transform.transl.y;
            sp34.unk30.z += sp34.transform.transl.z;
        } else if (sp34.unk0 != NULL) {
            sp34.unk30.x += sp34.unk0->globalPosition.x;
            sp34.unk30.y += sp34.unk0->globalPosition.y;
            sp34.unk30.z += sp34.unk0->globalPosition.z;
        }
    }
    return gDLL_13_Expgfx->vtbl->func1(&sp34, -1);
}
