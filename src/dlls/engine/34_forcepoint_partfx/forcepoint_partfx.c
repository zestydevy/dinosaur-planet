#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/*0x0*/ static u32 data_0[] = {
    0x000000df, 0x000001fc, 0x00000200
};
/*0xC*/ static u32 data_C[] = {
    0x000001fb, 0x42b40000, 0x42960000, 0xc2b40000, 0x42960000, 0x42c80000, 0xc2aa0000, 0xc2c80000, 
    0xc2aa0000, 0x00000000, 0x43200000, 0x00000000, 0xc2e60000
};
/*0x40*/ static f32 data_40 = 0.1f;
/*0x44*/ static f32 data_44 = 0.3f;
/*0x48*/ static f32 data_48 = 0.1f;
/*0x4C*/ static f32 data_4C = 0.3f;
/*0x50*/ static s32 data_50 = 0;
/*0x54*/ static s32 data_54 = 0;
/*0x58*/ static f32 data_58 = 0.0f;
/*0x5C*/ static f32 data_5C = 0.0f;

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void forcepointPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void forcepointPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void forcepointPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void forcepointPartfx_Func_20(s32 arg0) {
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
s32 forcepointPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp4C;
    s32 _pad2;
    SRT sp30;
    s32 _pad;

    data_40 += 0.001f;
    if (data_40 > 1.0f) {
        data_40 = 0.1f;
    }
    data_44 += 0.0003f;
    if (data_44 > 1.0f) {
        data_44 = 0.3f;
    }
    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX warning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
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
    sp4C.unk0 = obj;
    sp4C.unk44 = 0;
    sp4C.unk48 = 0;
    sp4C.unk5E = id;
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
    case 0x5FC:
        sp4C.unk8 = 0xA;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0x5C;
        sp4C.unk3C = 0.01f;
        break;
    case 0x5FB:
        sp4C.unk8 = 0xA;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0xE7;
        sp4C.unk3C = 0.01f;
        break;
    case 0x5FA:
        sp4C.unk30.x = (f32) mathRnd(-0x258, 0x258) * 0.06f;
        sp4C.unk30.z = (f32) mathRnd(-0x258, 0x258) * 0.06f;
        sp4C.unk24.y = (f32) mathRnd(0x320, 0x4B0) * 0.005f;
        sp4C.unk8 = 0x28;
        sp4C.unk60 = 0xFF;
        sp4C.unk48 = 0x200000;
        sp4C.unk42 = 0x26C;
        sp4C.unk3C = 0.00246f;
        break;
    case 0x5F9:
        sp4C.unk30.x = (f32) mathRnd(-0x258, 0x258) * 0.05f;
        sp4C.unk30.z = (f32) mathRnd(-0x258, 0x258) * 0.05f;
        sp4C.unk24.y = (f32) mathRnd(0x320, 0x4B0) * 0.00035f;
        sp4C.unk8 = 0xB4;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80480100;
        sp4C.unk48 = 0x02000000;
        sp4C.unk4 = 0x5E9;
        sp4C.unk42 = 0x26C;
        sp4C.unk3C = 0.0016f;
        break;
    case 0x5E9:
        sp4C.unk8 = 0x14;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x480200;
        sp4C.unk48 = 0x02000000;
        sp4C.unk42 = 0x26C;
        sp4C.unk3C = 0.0016f;
        break;
    case 0x3A7:
        sp4C.unk8 = 0x50;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x1C0100;
        sp4C.unk42 = 0x73;
        sp4C.unk3C = 0.026f;
        break;
    case 0x3A5:
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
            sp4C.unk30.z = transform->transl.x;
            sp4C.unk30.y = transform->transl.y;
        } else {
            sp4C.unk30.z = 15.0f;
            sp4C.unk30.y = 10.0f;
        }
        sp4C.unk24.z = (f32) mathRnd(0x1E, 0x28) * 0.012f;
        sp4C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(-4, 4) * 0.002f;
        sp4C.unk3C = (f32) mathRnd(0xA, 0x32) * 0.0002f;
        sp4C.unk8 = mathRnd(0, 0xA) + 0x50;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0x8E;
        sp4C.unk44 = 0x40180100;
        break;
    case 0x3A6:
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
            sp4C.unk30.z = transform->transl.x;
            sp4C.unk30.y = transform->transl.y;
        } else {
            sp4C.unk30.z = 15.0f;
            sp4C.unk30.y = 10.0f;
        }
        sp4C.unk24.z = (f32) mathRnd(0x1E, 0x28) * 0.018f;
        sp4C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(-4, 4) * 0.002f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x32) * 0.00002f;
        sp4C.unk8 = mathRnd(0, 0x3C) + 0x50;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0x8F;
        sp4C.unk48 = 0x200000;
        sp4C.unk44 = 0x42000100;
        break;
    case 0x3A3:
        sp4C.unk8 = 4;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 2;
        sp4C.unk42 = 0x64;
        sp4C.unk60 = 0x9B;
        sp4C.unk3C = 0.06f;
        break;
    case 0x3A4:
        sp4C.unk8 = 4;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 2;
        sp4C.unk42 = 0x64;
        sp4C.unk60 = 0xFF;
        sp4C.unk3C = 0.02f;
        break;
    case 0x3A2:
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
        sp4C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.001f;
        sp4C.unk24.y = (f32) mathRnd(0x3E8, 0x4B0) * 0.001f;
        sp4C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.001f;
        sp4C.unk3C = (f32) mathRnd(8, 0x14) * 0.002f * transform->scale;
        sp4C.unk8 = mathRnd(0x64, 0x78);
        sp4C.unk44 = 0x80180000;
        sp4C.unk48 = 0x400020;
        sp4C.unk42 = 0x60;
        sp4C.unk60 = 0x7F;
        sp4C.unk58[0] = 0xFFFF;
        sp4C.unk58[1] = 0xFFFF;
        sp4C.unk58[2] = 0x3CAF;
        sp4C.unk4C[0] = 0xFFFF;
        sp4C.unk4C[1] = 0xB1DF;
        sp4C.unk4C[2] = 0x3CAF;
        break;
    case 0x3A1:
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
        sp4C.unk30.x = transform->transl.x;
        sp4C.unk30.y = transform->transl.y + 20.0f;
        sp4C.unk30.z = transform->transl.z;
        sp4C.unk24.z = (f32) mathRnd(0x14, 0x1E) * 0.1f;
        sp4C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.02f;
        sp4C.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.02f;
        sp30.scale = 1.0f;
        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.roll = obj->srt.roll;
        sp30.pitch = obj->srt.pitch;
        sp30.yaw = obj->srt.yaw;
        mathRotateRPY(&sp30, sp4C.unk24.f);
        sp4C.unk8 = 0x32;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0x167;
        sp4C.unk48 = 0x200000;
        sp4C.unk44 = 0x02000110;
        sp4C.unk3C = 0.005f;
        break;
    case 0x3A0:
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
        sp4C.unk30.x = transform->transl.x;
        sp4C.unk30.y = transform->transl.y + 20.0f;
        sp4C.unk30.z = transform->transl.z;
        sp4C.unk24.z = (f32) mathRnd(0x14, 0x1E) * 0.04f;
        sp4C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.012f;
        sp4C.unk24.y = (f32) mathRnd(2, 6) * 0.015f;
        sp30.scale = 1.0f;
        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.roll = obj->srt.roll;
        sp30.pitch = obj->srt.pitch;
        sp30.yaw = obj->srt.yaw;
        mathRotateRPY(&sp30, sp4C.unk24.f);
        sp4C.unk3C = (f32) mathRnd(8, 0x14) * 0.002f;
        sp4C.unk8 = mathRnd(0x3C, 0x78);
        sp4C.unk44 = 0x80180000;
        sp4C.unk48 = 0x01400020;
        sp4C.unk42 = 0x162;
        sp4C.unk60 = 0x7F;
        sp4C.unk58[0] = 0xFFFF;
        sp4C.unk58[1] = 0xFFFF;
        sp4C.unk58[2] = 0xFFFF;
        sp4C.unk4C[0] = 0x3CAF;
        sp4C.unk4C[1] = 0x3CAF;
        sp4C.unk4C[2] = 0x3CAF;
        break;
    case 0x39E:
        sp4C.unk30.z = (f32) mathRnd(-0x14, 0x14) * 0.1f;
        sp4C.unk30.y = 0.0f;
        sp4C.unk30.x = (f32) mathRnd(-0x190, 0x190) * 0.1f;
        sp4C.unk24.y = (f32) mathRnd(0xA, 0xE) * 0.052f;
        sp4C.unk3C = 0.004245f;
        sp4C.unk8 = mathRnd(0x14A, 0x17C);
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80480208;
        sp4C.unk42 = 0x47;
        sp4C.unk4 = 0x39F;
        break;
    case 0x39F:
        sp4C.unk24.y = (f32) mathRnd(0xA, 0xE) * 0.042f;
        sp4C.unk8 = 1;
        sp4C.unk60 = 0x23;
        sp4C.unk48 = 2;
        sp4C.unk42 = 0x64;
        sp4C.unk3C = 0.007f;
        break;
    case 0x39A:
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0x12C;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 0x200;
        sp4C.unk42 = 0x17C;
        sp4C.unk3C = 0.00125f;
        break;
    case 0x39B:
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0x12C;
        sp4C.unk44 = 0x480000;
        sp4C.unk42 = 0x17C;
        sp4C.unk3C = 0.005f;
        break;
    case 0x39C:
        sp4C.unk60 = 0x37;
        sp4C.unk8 = 0x12C;
        sp4C.unk44 = 0x480000;
        sp4C.unk42 = 0x17C;
        sp4C.unk3C = 0.02f;
        break;
    case 0x39D:
        sp4C.unk60 = 0x87;
        sp4C.unk8 = 0x1E;
        sp4C.unk44 = 0x480200;
        sp4C.unk48 = 0x2000;
        sp4C.unk42 = 0x17C;
        sp4C.unk3C = 0.005f;
        break;
    case 0x399:
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
        sp4C.transform.pitch = 0;
        sp4C.transform.yaw = 0;
        sp4C.transform.transl.x = 0.0f;
        sp4C.transform.transl.y = 0.0f;
        sp4C.transform.transl.z = 0.0f;
        sp4C.transform.scale = 1.0f;
        if (transform != NULL) {
            sp4C.unk30.x = transform->transl.x;
            sp4C.unk30.y = transform->transl.y + 12.0f;
            sp4C.unk30.z = transform->transl.z;
            sp4C.transform.yaw = transform->yaw;
            sp4C.transform.roll = transform->roll;
        }
        sp4C.unk60 = 0xFF;
        sp4C.unk3C = 0.033f;
        sp4C.unk8 = mathRnd(0, 0xA) + 0x3C;
        sp4C.unk44 = 0x06100100;
        sp4C.unk48 = 0x02000000;
        sp4C.unk42 = 0x64;
        break;
    case 0x397:
        sp4C.unk30.x = (f32) mathRnd(-0x258, 0x258) * 0.01f;
        sp4C.unk30.z = (f32) mathRnd(-0x258, 0x258) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(0x320, 0x4B0) * 0.00015f;
        sp4C.unk8 = 0xB4;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80080110;
        sp4C.unk48 = 0x02000000;
        sp4C.unk4 = 0x398;
        sp4C.unk42 = 0x47;
        sp4C.unk3C = 0.0006f;
        break;
    case 0x398:
        sp4C.unk8 = 0x1E;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80210;
        sp4C.unk48 = 0x02000000;
        sp4C.unk42 = 0x47;
        sp4C.unk3C = 0.0006f;
        break;
    case 0x5F7:
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0x73;
        sp4C.unk44 = 0x08100110;
        sp4C.unk48 = 0x02000000;
        sp4C.unk42 = 0x77;
        sp4C.unk3C = 0.55f;
        break;
    case 0x5F6:
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0xA;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 0x202;
        sp4C.unk42 = 0x26C;
        sp4C.unk3C = 0.000725f;
        gDLL_13_Expgfx->vtbl->func1(&sp4C, 0);
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0xA;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 2;
        sp4C.unk42 = 0x528;
        sp4C.unk3C = 0.003f;
        gDLL_13_Expgfx->vtbl->func1(&sp4C, 0);
        sp4C.unk60 = 0x37;
        sp4C.unk8 = 0xA;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 2;
        sp4C.unk42 = 0x528;
        sp4C.unk3C = 0.015f;
        gDLL_13_Expgfx->vtbl->func1(&sp4C, 0);
        sp4C.unk60 = 0x87;
        sp4C.unk8 = 0xA;
        sp4C.unk44 = 0x480200;
        sp4C.unk48 = 0x2002;
        sp4C.unk42 = 0x528;
        sp4C.unk3C = 0.003f;
        break;
    case 0x5F5:
        sp4C.unk24.x = (f32) mathRnd(-0x384, 0x384) * 0.00025f;
        sp4C.unk24.z = (f32) mathRnd(-0x384, 0x384) * 0.00025f;
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0x3C;
        sp4C.unk44 = 0x110;
        sp4C.unk48 = 0x100;
        sp4C.unk42 = 0xE4;
        sp4C.unk3C = 0.0055f;
        break;
    case 0x5F4:
        sp4C.unk30.x = (f32) mathRnd(-0xC8, 0xC8) * 0.005f;
        sp4C.unk30.y = (f32) mathRnd(-0xC8, 0xC8) * 0.005f;
        sp4C.unk24.z = (f32) mathRnd(0x12C, 0x190) * 0.00025f;
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0x8C;
        sp4C.unk44 = 0x480100;
        sp4C.unk42 = 0x528;
        sp4C.unk3C = 0.00025f;
        break;
    case 0x5F0:
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0x12C;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 0x200;
        sp4C.unk42 = 0x26C;
        sp4C.unk3C = 0.00125f;
        break;
    case 0x5F1:
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0x12C;
        sp4C.unk44 = 0x480000;
        sp4C.unk42 = 0x528;
        sp4C.unk3C = 0.005f;
        break;
    case 0x5F2:
        sp4C.unk60 = 0x37;
        sp4C.unk8 = 0x12C;
        sp4C.unk44 = 0x480000;
        sp4C.unk42 = 0x528;
        sp4C.unk3C = 0.02f;
        break;
    case 0x5F3:
        sp4C.unk60 = 0x87;
        sp4C.unk8 = 0x1E;
        sp4C.unk44 = 0x480200;
        sp4C.unk48 = 0x2000;
        sp4C.unk42 = 0x528;
        sp4C.unk3C = 0.005f;
        break;
    case 0x5EF:
        sp4C.unk30.x = (f32) mathRnd(-0x190, 0x190) * 0.001f;
        sp4C.unk30.z = (f32) mathRnd(-0x190, 0x190) * 0.001f;
        sp4C.unk60 = 0x9B;
        sp4C.unk24.y = 3.0f;
        sp4C.unk3C = 0.0025f;
        sp4C.unk8 = mathRnd(0, 0xA) + 0x3C;
        sp4C.unk44 = 0x80100;
        sp4C.unk48 = 0x100;
        sp4C.unk42 = 0x3F2;
        break;
    case 0x5EE:
        sp4C.unk24.z = (f32) mathRnd(-0x28, 0x28) * -0.002f;
        sp4C.unk24.y = (f32) mathRnd(-0x28, 0x28) * -0.002f;
        sp4C.unk60 = 0xFF;
        sp4C.unk3C = 0.0015f;
        sp4C.unk8 = mathRnd(0, 0xA) + 0x3C;
        sp4C.unk44 = 0x02000100;
        sp4C.unk48 = 0x200;
        sp4C.unk42 = 0x33;
        break;
    case 0x5F8:
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * -0.002f;
        sp4C.unk24.y = (f32) mathRnd(-0x28, 0x28) * -0.002f;
        sp4C.unk60 = 0xFF;
        sp4C.unk3C = 0.0015f;
        sp4C.unk8 = mathRnd(0, 0xA) + 0x3C;
        sp4C.unk44 = 0x02000100;
        sp4C.unk48 = 0x400;
        sp4C.unk42 = 0x33;
        break;
    case 0x5ED:
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
        sp4C.transform.pitch = 0;
        sp4C.transform.yaw = 0;
        sp4C.transform.transl.x = 0.0f;
        sp4C.transform.transl.y = 0.0f;
        sp4C.transform.transl.z = 0.0f;
        sp4C.transform.scale = 1.0f;
        if (transform != NULL) {
            sp4C.unk30.x = transform->transl.x;
            sp4C.unk30.y = transform->transl.y + 12.0f;
            sp4C.unk30.z = transform->transl.z;
            sp4C.transform.yaw = transform->yaw;
            sp4C.transform.roll = transform->roll;
        }
        sp4C.unk60 = 0xFF;
        sp4C.unk3C = 0.033f;
        sp4C.unk8 = mathRnd(0, 0xA) + 0x3C;
        sp4C.unk44 = 0x06100100;
        sp4C.unk42 = 0x205;
        break;
    case 0x5EB:
        sp4C.unk24.z = (f32) mathRnd(0xB4, 0xC8) * -0.0035f;
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * -0.002f;
        sp4C.unk24.y = (f32) mathRnd(0, 0x28) * 0.005f;
        sp4C.unk60 = 0x9B;
        sp4C.unk8 = 0x8C;
        sp4C.unk44 = 0x81100200;
        sp4C.unk48 = 0x20;
        sp4C.unk3C = 0.04f;
        sp4C.unk58[0] = 0xD8EF;
        sp4C.unk58[1] = 0xB1DF;
        sp4C.unk58[2] = mathRnd(-0x2710, 0) + 0x5208;
        sp4C.unk4C[0] = 0xD8EF;
        sp4C.unk4C[1] = 0xB1DF;
        sp4C.unk4C[2] = mathRnd(-0x2710, 0) + 0x5208;
        sp4C.unk42 = 0x60;
        break;
    case 0x5EC:
        sp4C.unk24.z = (f32) mathRnd(0xB4, 0xC8) * -0.0035f;
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * -0.005f;
        sp4C.unk24.y = (f32) mathRnd(0, 0x28) * 0.005f;
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0x8C;
        sp4C.unk44 = 0x81000000;
        sp4C.unk48 = 0x02000000;
        sp4C.unk42 = 0x26D;
        sp4C.unk3C = 0.0013f;
        break;
    case 0x5EA:
        sp4C.unk30.x = (f32) mathRnd(-0xF, 0xF);
        sp4C.unk30.z = (f32) mathRnd(-0xF, 0xF);
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0x46;
        sp4C.unk44 = 0x81000000;
        sp4C.unk42 = 0x26D;
        sp4C.unk3C = 0.0043f;
        break;
    case 0x5E3:
        sp4C.unk3C = (f32) mathRnd(0x19, 0x23) * 0.000098f;
        sp4C.unk8 = 0xF0;
        sp4C.unk60 = 0x55;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 0x200;
        sp4C.unk42 = 0x156;
        break;
    case 0x5E4:
        sp4C.unk3C = (f32) mathRnd(0x19, 0x23) * 0.000098f;
        sp4C.unk8 = 0xF0;
        sp4C.unk60 = 0x55;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 0x100;
        sp4C.unk42 = 0x156;
        break;
    case 0x5E5:
        sp4C.unk8 = 0xF0;
        sp4C.unk60 = 0xB9;
        sp4C.unk44 = 0x480000;
        sp4C.unk42 = 0x156;
        sp4C.unk3C = 0.0198f;
        break;
    case 0x5E6:
        sp4C.unk3C = (f32) mathRnd(0x19, 0x23) * 0.000098f;
        sp4C.unk8 = 0x12C;
        sp4C.unk60 = 0x55;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 0x200;
        sp4C.unk42 = 0x156;
        break;
    case 0x5E7:
        sp4C.unk3C = (f32) mathRnd(0x19, 0x23) * 0.000098f;
        sp4C.unk8 = 6;
        sp4C.unk60 = 0x55;
        sp4C.unk44 = 0x480000;
        sp4C.unk48 = 0x100;
        sp4C.unk42 = 0x156;
        break;
    case 0x5E8:
        sp4C.unk8 = 6;
        sp4C.unk60 = 0x55;
        sp4C.unk44 = 0x480000;
        sp4C.unk42 = 0x156;
        sp4C.unk3C = 0.0198f;
        break;
    case 0x5DD:
        sp4C.unk30.z = (f32) mathRnd(-0xC, 0xC);
        sp4C.unk30.y = (f32) mathRnd(-0xC, 0xC);
        sp4C.unk24.x = (f32) mathRnd(5, 0xF) * 0.08f;
        sp4C.unk24.y = sp4C.unk30.y / 100.0f;
        sp4C.unk24.z = sp4C.unk30.z / 100.0f;
        sp4C.unk3C = (f32) mathRnd(5, 0xF) * 0.00018f;
        sp4C.unk8 = 0xFA;
        sp4C.unk60 = 0x9B;
        sp4C.unk44 = 0x480100;
        sp4C.unk42 = 0x206;
        break;
    case 0x5DE:
        sp4C.unk30.z = (f32) mathRnd(-0xC, 0xC);
        sp4C.unk30.y = (f32) mathRnd(-0xC, 0xC);
        sp4C.unk24.x = (f32) mathRnd(5, 0xF) * 0.08f;
        sp4C.unk24.y = sp4C.unk30.y / 100.0f;
        sp4C.unk24.z = sp4C.unk30.z / 100.0f;
        sp4C.unk3C = (f32) mathRnd(5, 0xF) * 0.00018f;
        sp4C.unk8 = 0xFA;
        sp4C.unk60 = 0x9B;
        sp4C.unk44 = 0x480100;
        sp4C.unk42 = 0x166;
        break;
    case 0x5DF:
        sp4C.unk30.z = (f32) mathRnd(-0xC, 0xC);
        sp4C.unk30.y = (f32) mathRnd(-0xC, 0xC);
        sp4C.unk24.x = (f32) mathRnd(5, 0xF) * 0.08f;
        sp4C.unk24.y = sp4C.unk30.y / 100.0f;
        sp4C.unk24.z = sp4C.unk30.z / 100.0f;
        sp4C.unk3C = (f32) mathRnd(5, 0xF) * 0.00018f;
        sp4C.unk8 = 0xFA;
        sp4C.unk60 = 0x9B;
        sp4C.unk44 = 0x480100;
        sp4C.unk42 = 0x528;
        break;
    case 0x5E0:
        sp4C.unk3C = (f32) mathRnd(5, 0xF) * 0.00018f;
        sp4C.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(5, 0xF) * 0.02f;
        sp4C.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.01f;
        sp4C.unk8 = 0x50;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x180208;
        sp4C.unk58[0] = 0x63BF;
        sp4C.unk58[1] = 0x63BF;
        sp4C.unk58[2] = mathRnd(-0x2710, 0) + 0x2AF8;
        sp4C.unk58[0] = 0x63BF;
        sp4C.unk58[1] = 0x63BF;
        sp4C.unk58[2] = mathRnd(-0x2710, 0) + 0x2AF8;
        sp4C.unk48 = 0xA0;
        sp4C.unk42 = 0x166;
        break;
    case 0x5E1:
        sp4C.unk3C = (f32) mathRnd(0x19, 0x23) * 0.00098f;
        sp4C.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.001f;
        sp4C.unk24.y = (f32) mathRnd(5, 0xF) * 0.02f;
        sp4C.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.001f;
        sp4C.unk8 = 0x118;
        sp4C.unk60 = 0x55;
        sp4C.unk44 = 0x180208;
        sp4C.unk58[0] = 0x63BF;
        sp4C.unk58[1] = 0x63BF;
        sp4C.unk58[2] = mathRnd(-0x2710, 0) + 0x2AF8;
        sp4C.unk58[0] = 0x63BF;
        sp4C.unk58[1] = 0x63BF;
        sp4C.unk58[2] = mathRnd(-0x2710, 0) + 0x2AF8;
        sp4C.unk48 = 0xA0;
        sp4C.unk42 = 0x166;
        break;
    case 0x5E2:
        sp4C.unk3C = (f32) mathRnd(0x19, 0x23) * 0.00068f;
        sp4C.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(5, 0xF) * 0.02f;
        sp4C.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.01f;
        sp4C.unk8 = 0x50;
        sp4C.unk60 = 0x55;
        sp4C.unk44 = 0x180208;
        sp4C.unk58[0] = 0x63BF;
        sp4C.unk58[1] = 0x63BF;
        sp4C.unk58[2] = mathRnd(-0x2710, 0) + 0x2AF8;
        sp4C.unk58[0] = 0x63BF;
        sp4C.unk58[1] = 0x63BF;
        sp4C.unk58[2] = mathRnd(-0x2710, 0) + 0x2AF8;
        sp4C.unk48 = 0xA0;
        sp4C.unk42 = 0x166;
        break;
    case 0x396:
        sp4C.unk8 = 0x50;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x1C0100;
        sp4C.unk42 = 0x159;
        sp4C.unk3C = 0.026f;
        break;
    case 0x394:
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
            sp4C.unk30.z = transform->transl.x;
            sp4C.unk30.y = transform->transl.y;
            sp4C.unk30.x = transform->transl.z;
        }
        sp4C.transform.yaw = mathRnd(0, 0xFFFF);
        sp4C.transform.pitch = mathRnd(0, 0xFFFF);
        sp4C.transform.yaw = mathRnd(0, 0xFFFF);
        sp4C.transform.transl.x = 0.0f;
        sp4C.transform.transl.y = 0.0f;
        sp4C.transform.transl.z = 0.0f;
        sp4C.unk3C = (f32) mathRnd(0x1E, 0x28) * 0.0005f;
        sp4C.unk8 = mathRnd(0x1E, 0x2F);
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x06100100;
        sp4C.unk42 = 0x206;
        break;
    case 0x395:
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
            sp4C.unk30.z = transform->transl.x;
            sp4C.unk30.y = transform->transl.y;
            sp4C.unk30.x = transform->transl.z;
        }
        sp4C.transform.yaw = mathRnd(0, 0xFFFF);
        sp4C.transform.pitch = mathRnd(0, 0xFFFF);
        sp4C.transform.yaw = mathRnd(0, 0xFFFF);
        sp4C.transform.transl.x = 0.0f;
        sp4C.transform.transl.y = 0.0f;
        sp4C.transform.transl.z = 0.0f;
        sp4C.unk3C = (f32) mathRnd(0x1E, 0x28) * 0.005f;
        sp4C.unk8 = mathRnd(0x50, 0x64);
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x06100110;
        sp4C.unk42 = 0x206;
        break;
    case 0x393:
        sp4C.unk30.z = (f32) mathRnd(-0xC8, 0xC8);
        sp4C.unk30.y = (f32) mathRnd(0, 0x14);
        sp4C.unk30.x = (f32) mathRnd(-0x190, 0x190) * 0.3f;
        sp4C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.042f;
        sp4C.unk3C = 0.004245f;
        sp4C.unk8 = mathRnd(0x212, 0x2A8);
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80480208;
        sp4C.unk42 = 0x47;
        break;
    case 0x392:
        sp4C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0x14, 0x14) * 0.1f;
        sp4C.unk24.x = (f32) mathRnd(-0x1E, 0x1E) * 0.02f;
        sp4C.unk24.y = (f32) mathRnd(-0x1E, 0x1E) * 0.02f;
        sp4C.unk24.z = (f32) mathRnd(-0x1E, 0x1E) * 0.02f;
        sp4C.unk3C = (f32) mathRnd(0xA, 0xF) * 0.00047f;
        sp4C.unk8 = mathRnd(0x5A, 0x8C);
        sp4C.unk44 = 0x80400201;
        sp4C.unk61 = 0;
        sp4C.unk42 = 0x23B;
        break;
    case 0x390:
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
            sp4C.unk30.z = transform->transl.x;
            sp4C.unk30.y = transform->transl.y;
        } else {
            sp4C.unk30.z = 15.0f;
            sp4C.unk30.y = 10.0f;
        }
        sp4C.unk24.z = (f32) mathRnd(0x1E, 0x28) * 0.012f;
        sp4C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(-4, 4) * 0.002f;
        sp4C.unk3C = (f32) mathRnd(0xA, 0x32) * 0.0002f;
        sp4C.unk8 = mathRnd(0, 0xA) + 0x50;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0x8E;
        sp4C.unk44 = 0x40180100;
        break;
    case 0x391:
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
            sp4C.unk30.z = transform->transl.x;
            sp4C.unk30.y = transform->transl.y;
        } else {
            sp4C.unk30.z = 15.0f;
            sp4C.unk30.y = 10.0f;
        }
        sp4C.unk24.z = (f32) mathRnd(0x1E, 0x28) * 0.018f;
        sp4C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(-4, 4) * 0.002f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x32) * 0.00002f;
        sp4C.unk8 = mathRnd(0, 0x3C) + 0x50;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0x8F;
        sp4C.unk48 = 0x200000;
        sp4C.unk44 = 0x42000100;
        break;
    case 0x38F:
        sp4C.unk30.x = (f32) mathRnd(-0x8C, 0x8C);
        sp4C.unk30.y = (f32) mathRnd(-0x28, 0x8C);
        sp4C.unk30.z = (f32) mathRnd(-0x8C, 0x8C);
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.06f;
        sp4C.unk24.y = (f32) mathRnd(-0x28, 0x28) * 0.023f;
        sp4C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.06f;
        sp4C.unk8 = 0x96;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0x167;
        sp4C.unk48 = 0x300000;
        sp4C.unk44 = 0x02000110;
        sp4C.unk3C = 0.0055f;
        break;
    case 0x38A:
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
        
        sp4C.unk30.x = (f32) mathRnd(-0xA, -0xA) * 0.1f;
        sp4C.unk30.y = (f32) mathRnd(-0x14, -0xA) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp4C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.003f;
        sp4C.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.003f;
        sp4C.unk60 = 0xFF;
        if (transform != NULL) {
            sp4C.unk30.x += transform->transl.x;
            sp4C.unk30.y += transform->transl.y;
            sp4C.unk30.z += transform->transl.z;
        }
        sp4C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.00088f;
        sp4C.unk8 = 0x55;
        sp4C.unk44 = 0x100200;
        sp4C.unk42 = 0x125;
        sp4C.unk61 = mathRnd(0, 0x14) + 4;
        sp4C.unk58[0] = 0xFFFF;
        sp4C.unk58[1] = (mathRnd(0, 0x2710) + 0xD8EF);
        sp4C.unk58[2] = 0;
        sp4C.unk4C[0] = sp4C.unk58[0] / 10;
        sp4C.unk4C[1] = sp4C.unk58[1] / 10;
        sp4C.unk4C[2] = 0;
        sp4C.unk48 = 0xA0;
        break;
    case 0x38B:
        sp4C.unk8 = 0x4B;
        sp4C.unk44 = 0x82000108;
        sp4C.unk48 = 0x80;
        sp4C.unk42 = 0x8F;
        sp4C.unk60 = 0xFF;
        sp4C.unk3C = 0.00138f;
        break;
    case 0x38C:
        sp4C.unk8 = 0x190;
        sp4C.unk48 = 0x100;
        sp4C.unk42 = 0x167;
        sp4C.unk60 = 0x9B;
        sp4C.unk30.y = 40.0f;
        sp4C.unk3C = 0.0068f;
        break;
    case 0x38D:
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
            sp4C.unk30.z = transform->transl.z;
        }
        sp4C.unk30.y = 400.0f;
        sp4C.unk24.x = ((f32) mathRnd(-0xA, 0xA) * 0.015f) + 0.01f;
        sp4C.unk24.y = (f32) mathRnd(0x32, 0x64) * 0.01f;
        sp4C.unk24.z = ((f32) mathRnd(-0xA, 1) * 0.015f) + 0.01f;
        sp4C.unk8 = 0xC8;
        sp4C.unk44 = 0x03008000;
        sp4C.unk48 = 0x200000;
        sp4C.unk42 = 0x167;
        sp4C.unk60 = 0xFF;
        sp4C.unk3C = 0.0038f;
        break;
    case 0x38E:
        sp4C.unk24.x = ((f32) mathRnd(-0xA, 0xA) * 0.15f) + 0.01f;
        sp4C.unk24.y = (f32) mathRnd(0x32, 0x64) * 0.02f;
        sp4C.unk24.z = ((f32) mathRnd(-0xA, 1) * 0.15f) + 0.01f;
        sp4C.unk8 = 0x50;
        sp4C.unk44 = 0x03000000;
        sp4C.unk48 = 0x200000;
        sp4C.unk42 = 0x167;
        sp4C.unk60 = 0xFF;
        sp4C.unk3C = 0.0038f;
        break;
    case 0x389:
        if (transform == NULL) {
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.scale = 1.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
        }
        sp4C.unk30.x = (f32) mathRnd(-5, 5);
        sp4C.unk30.y = (f32) mathRnd(1, 5);
        sp4C.unk30.z = (f32) mathRnd(-5, 5);
        sp30.scale = ((f32) mathRnd(0, 0x258) * 0.003f) + 2.3f;
        sp4C.unk24.y = ((f32) mathRnd(0, 0xC8) * 0.001f) + 1.0f;
        sp4C.unk24.x = ((f32) mathRnd(0, 0x14) * 0.015f) + 0.1f;
        sp4C.unk24.y = sp4C.unk24.y * sp30.scale;
        sp4C.unk24.x = sp4C.unk24.x * sp30.scale;
        sp4C.unk3C = ((f32) mathRnd(0, 0xA) * 0.00006f) + 0.0048f;
        sp4C.unk8 = mathRnd(0xB4, 0xC8);
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x03000120;
        sp4C.unk48 = 0x200000;
        sp4C.unk42 = 0x8F;
        sp4C.unk4 = 0x385;
        break;
    case 0x388:
        sp4C.unk30.x = (f32) mathRnd(0, 0x10);
        sp4C.unk30.z = (f32) mathRnd(-0x2E, 0x2E);
        sp4C.unk24.y = (f32) mathRnd(0x10, 0x1E) * 0.05f;
        sp4C.unk8 = 0x64;
        sp4C.unk60 = 0x37;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x100;
        sp4C.unk48 = 0x100;
        sp4C.unk42 = (s16) *data_C;
        sp4C.unk3C = 0.0025f;
        break;
    case 0x384:
        sp4C.unk30.x = (f32) mathRnd(-0x37, 0x37);
        sp4C.unk30.y = (f32) mathRnd(0xA, 0xF);
        sp4C.unk30.z = (f32) mathRnd(-0x37, 0x37);
        sp4C.unk24.x = (f32) mathRnd(-8, 8) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.1f;
        sp4C.unk24.z = (f32) mathRnd(-8, 8) * 0.01f;
        sp4C.unk3C = ((f32) mathRnd(0, 0xA) * 0.0002f) + 0.0028945f;
        sp4C.unk8 = mathRnd(0x78, 0x8C);
        sp4C.unk60 = 0xFF;
        sp4C.unk4 = 0x385;
        sp4C.unk48 = 0x200000;
        sp4C.unk44 = 0x01001100;
        sp4C.unk42 = 0x8F;
        break;
    case 0x387:
        sp4C.unk30.x = (f32) mathRnd(-0x19, 0x19);
        sp4C.unk30.y = (f32) mathRnd(1, 5);
        sp4C.unk30.z = (f32) mathRnd(-0x19, 0x19);
        sp4C.unk24.x = (f32) mathRnd(-8, 8) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.1f;
        sp4C.unk24.z = (f32) mathRnd(-8, 8) * 0.01f;
        sp4C.unk3C = ((f32) mathRnd(0, 0xA) * 0.0002f) + 0.0028945f;
        sp4C.unk8 = mathRnd(0x78, 0x8C);
        sp4C.unk60 = 0xFF;
        sp4C.unk4 = 0x385;
        sp4C.unk48 = 0x200000;
        sp4C.unk44 = 0x81000120;
        sp4C.unk42 = 0x8F;
        break;
    case 0x385:
        sp4C.unk24.y = (f32) mathRnd(2, 0x14) * 0.002f;
        sp4C.unk8 = 0x1E;
        sp4C.unk60 = 0x9B;
        sp4C.unk44 = 0x180100;
        sp4C.unk42 = 0x5F;
        sp4C.unk3C = 0.008445f;
        sp4C.unk58[0] = 0xFFFF;
        sp4C.unk58[1] = (mathRnd(0, 0xC350) + 0x3CAF);
        sp4C.unk58[2] = 0;
        sp4C.unk4C[0] = sp4C.unk58[0];
        sp4C.unk4C[1] = sp4C.unk58[1];
        sp4C.unk4C[2] = 0;
        sp4C.unk48 = 0x20;
        break;
    case 0x386:
        sp4C.unk30.y = (f32) mathRnd(1, 5);
        sp4C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.02f;
        sp4C.unk3C = ((f32) mathRnd(0, 0xA) * 0.0002f) + 0.0004245f;
        sp4C.unk8 = mathRnd(0xE6, 0x118);
        sp4C.unk60 = 0x9B;
        sp4C.unk44 = 0x80480200;
        sp4C.unk42 = 0x47;
        break;
    default:
        // STUBBED_PRINTF("PARTFX warning type < %d > not found.. returning\n", id); // default.dol
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
    return gDLL_13_Expgfx->vtbl->func1(&sp4C, -1);
}
