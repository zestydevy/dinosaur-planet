#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

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

// offset: 0x0 | ctor
void wmPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void wmPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void wmPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void wmPartfx_Func_20(s32 arg0) {
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
s32 wmPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp4C;
    s32 _pad;
    SRT sp30;
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
    sp4C.unk24.y = 0.0f;
    sp4C.unk24.z = 0.0f;
    sp4C.unk3C = 0.0f;
    sp4C.unk24.x = 0.0f;
    sp4C.unk58[0] = 0xFFFF;
    sp4C.unk58[1] = 0xFFFF;
    sp4C.unk58[2] = 0xFFFF;
    sp4C.unk4C[0] = 0xFFFF;
    sp4C.unk4C[1] = 0xFFFF;
    sp4C.unk4C[2] = 0xFFFF;
    sp4C.unk40 = 0;
    switch (id) {
    case 0x1C8:
        sp4C.unk30.y = (f32) mathRnd(0, 0x64) * 0.1f;
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.022f;
        sp4C.unk24.z = (f32) mathRnd(-0x1E, 0x1E) * 0.022f * sp4C.unk24.x;
        sp4C.unk3C = (f32) mathRnd(0xC8, 0x118) * 0.0000035f;
        sp4C.unk8 = 0x32;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80118;
        sp4C.unk48 = 8;
        sp4C.unk42 = 0x566;
        break;
    case 0x1C9:
        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.scale = 1.0f;
        sp30.roll = 0;
        sp30.pitch = 0;
        sp4C.unk30.z = 12.0f;
        sp30.yaw = mathRnd(0, 0xFFFF);
        mathRotateRPY(&sp30, &sp4C.unk30.x);
        sp4C.unk3C = (f32) mathRnd(0xC8, 0x118) * 0.0000175f;
        sp4C.unk8 = 0x14;
        sp4C.unk60 = 0xE1;
        sp4C.unk44 = 0x400000;
        sp4C.unk42 = 0x4F9;
        break;
    case 0x1CA:
        sp4C.unk24.x = (f32) mathRnd(-0x1C, 0x1C) * 0.01f;
        sp4C.unk24.z = (f32) mathRnd(-0x1C, 0x1C) * 0.01f;
        sp4C.unk3C = (f32) mathRnd(0xC8, 0x118) * 0.00005f;
        sp4C.unk8 = 0xC8;
        sp4C.unk60 = 0xE1;
        sp4C.unk44 = 0x400110;
        if (mathRnd(0, 2) == 0) {
            sp4C.unk48 |= 0x100;
        } else {
            sp4C.unk48 |= 0x400;
        }
        sp4C.unk42 = 0x4F9;
        break;
    case 0x1C7:
        sp4C.unk24.x = (f32) mathRnd(-0x1C, 0x1C) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(-0x1C, 0x1C) * 0.01f;
        sp4C.unk24.z = (f32) mathRnd(-0x1C, 0x1C) * 0.01f;
        sp4C.unk30.x = (f32) mathRnd(-0x46, 0x46);
        sp4C.unk30.y = (f32) mathRnd(0x82, 0xAA);
        sp4C.unk30.z = (f32) mathRnd(-0x46, 0x46);
        sp4C.unk8 = 0x190;
        sp4C.unk60 = 0xFF;
        sp4C.unk58[0] = 0;
        sp4C.unk58[1] = 0;
        sp4C.unk58[2] = 0;
        sp4C.unk4C[0] = 0;
        sp4C.unk4C[1] = 0;
        sp4C.unk4C[2] = 0;
        sp4C.unk44 = 0x80480108;
        sp4C.unk48 = 0x20;
        sp4C.unk42 = 0x33;
        sp4C.unk3C = 0.01f;
        break;
    case 0x1C5:
        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.scale = 1.0f;
        sp4C.unk30.x = 450.0f;
        sp30.roll = mathRnd(0, 0xFFFF);
        sp30.pitch = mathRnd(0, 0xFFFF);
        sp30.yaw = mathRnd(0, 0xFFFF);
        mathRotateRPY(&sp30, sp4C.unk30.f);
        sp4C.unk8 = 0xC8;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x480100;
        sp4C.unk42 = 0x33;
        sp4C.unk3C = 0.005f;
        break;
    case 0x1C4:
        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.scale = 1.0f;
        sp4C.unk30.x = 350.0f;
        sp30.roll = mathRnd(0, 0xFFFF);
        sp30.pitch = mathRnd(0, 0xFFFF);
        sp30.yaw = mathRnd(0, 0xFFFF);
        mathRotateRPY(&sp30, sp4C.unk30.f);
        sp4C.unk8 = 0xC8;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x480100;
        sp4C.unk42 = 0x26C;
        sp4C.unk3C = 0.008f;
        break;
    case 0x1C6:
        sp4C.unk30.x = (f32) mathRnd(0, 0x5A) + 250.0f;
        sp4C.unk30.y = (f32) mathRnd(-0xA, 0xA);
        sp30.roll = 0;
        sp30.pitch = 0;
        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.scale = 1.0f;
        sp30.yaw = mathRnd(0, 0xFFFF);
        mathRotateRPY(&sp30, sp4C.unk30.f);
        sp4C.unk3C = (f32) mathRnd(1, 0x14) * 0.0002f;
        sp4C.unk8 = 0xC8;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x480100;
        sp4C.unk48 = 0x02000000;
        sp4C.unk42 = 0x23C;
        break;
    case 0x1C3:
        sp4C.unk8 = 0x3C;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x0A100110;
        sp4C.unk42 = 0x23B;
        sp4C.unk24.y = 0.1f;
        sp4C.unk3C = 0.008f;
        break;
    case 0x190:
        sp4C.unk3C = (f32) mathRnd(1, 5) * 0.0037f;
        sp4C.unk8 = mathRnd(0xA, 0x14);
        sp4C.unk48 = 2;
        sp4C.unk61 = 0;
        sp4C.unk42 = 0xDF;
        break;
    case 0x191:
        sp4C.unk30.x = (f32) mathRnd(-8, 8);
        sp4C.unk30.y = (f32) mathRnd(0, 0x50);
        sp4C.unk30.z = (f32) mathRnd(-8, 8);
        sp4C.unk24.y = (f32) mathRnd(-3, 3) * 0.05f;
        sp4C.unk8 = 0x64;
        sp4C.unk60 = 0x7D;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x110;
        sp4C.unk42 = 0xDE;
        sp4C.unk3C = 0.001f;
        break;
    case 0x192:
        sp4C.unk30.x = (f32) mathRnd(-0x9E, 0x9E);
        sp4C.unk30.y = (f32) mathRnd(0, 0x78);
        sp4C.unk30.z = (f32) mathRnd(-0xD0, 0xD0);
        sp4C.unk24.y = (f32) mathRnd(-3, 3) * 0.04f;
        sp4C.unk8 = 0xC8;
        sp4C.unk60 = 0x7D;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x80080112;
        sp4C.unk42 = 0x1DD;
        sp4C.unk3C = 0.065f;
        break;
    case 0x193:
        sp4C.unk30.x = (f32) mathRnd(-0x9E, 0x9E);
        sp4C.unk30.y = (f32) mathRnd(0, 0x78);
        sp4C.unk30.z = (f32) mathRnd(-0x3A, 0x3A);
        sp4C.unk24.y = (f32) mathRnd(-3, 3) * 0.05f;
        sp4C.unk8 = 0x64;
        sp4C.unk60 = 0x7D;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x80080112;
        sp4C.unk42 = 0xDE;
        sp4C.unk3C = 0.065f;
        break;
    case 0x194:
        sp4C.unk24.x = (f32) mathRnd(-0x3A, 0x3A) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(0, 0x78) * 0.01f;
        sp4C.unk24.z = (f32) mathRnd(-0x3A, 0x3A) * 0.01f;
        sp4C.unk30.x = (f32) mathRnd(-5, 5);
        sp4C.unk30.y = (f32) mathRnd(0, 0x50);
        sp4C.unk30.z = (f32) mathRnd(-5, 5);
        sp4C.unk8 = 0x96;
        sp4C.unk60 = 0x7D;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x80480110;
        sp4C.unk48 = 8;
        sp4C.unk42 = 0xDE;
        sp4C.unk3C = 0.0015f;
        break;
    case 0x195:
        sp4C.unk8 = 0x14;
        sp4C.unk60 = 0x9B;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x80480214;
        sp4C.unk42 = 0xDE;
        sp4C.unk3C = 0.0018f;
        break;
    case 0x196:
        sp4C.unk30.x = (f32) mathRnd(-0x1E, 0x1E) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0x1E, 0x1E) * 0.1f;
        sp4C.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.0005f;
        sp4C.unk24.y = (f32) mathRnd(0xF, 0x23) * 0.045f;
        sp4C.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.0005f;
        sp4C.unk8 = 0x78;
        sp4C.unk60 = 0xFF;
        sp4C.unk61 = 0x10;
        sp4C.unk58[0] = 0xFFFF;
        sp4C.unk58[1] = 0xFFFF;
        sp4C.unk58[2] = 0x8ACF;
        sp4C.unk4C[0] = 0xAFC8;
        sp4C.unk4C[1] = 0x3A98;
        sp4C.unk4C[2] = 0x5DC;
        sp4C.unk44 = 0x81080200;
        sp4C.unk48 = 0x24;
        sp4C.unk42 = 0x1DD;
        sp4C.unk3C = 0.013f;
        break;
    case 0x197:
        sp4C.unk30.x = (f32) mathRnd(-0x1E, 0x1E) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0x1E, 0x1E) * 0.1f;
        sp4C.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.006f;
        sp4C.unk24.y = (f32) mathRnd(0xF, 0x23) * 0.015f;
        sp4C.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.006f;
        sp4C.unk8 = 0x50;
        sp4C.unk60 = 0xFF;
        sp4C.unk61 = 0x10;
        sp4C.unk58[0] = 0xF82F;
        sp4C.unk58[1] = 0xF447;
        sp4C.unk58[2] = 0xFFFF;
        sp4C.unk4C[0] = 0xA7F8;
        sp4C.unk4C[1] = 0;
        sp4C.unk4C[2] = 0;
        sp4C.unk44 = 0x80080610;
        sp4C.unk48 = 0x24;
        sp4C.unk42 = 0x1DE;
        sp4C.unk3C = 0.01f;
        break;
    case 0x198:
        sp4C.unk30.y = (f32) mathRnd(0, 0x3C) * 0.8f;
        sp4C.unk8 = 0x1E;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x08100200;
        sp4C.unk42 = 0x91;
        sp4C.unk3C = 0.035f;
        break;
    case 0x199:
        sp4C.unk3C = ((f32) mathRnd(0, 0x32) * 0.000002f) + 0.0007f;
        sp4C.unk8 = 0;
        sp4C.unk60 = mathRnd(0, 0x37) + 0xC8;
        sp4C.unk61 = 0;
        sp4C.unk42 = mathRnd(0, 2) + 0x156;
        sp4C.unk44 = 0x80011;
        sp4C.unk48 = 2;
        break;
    case 0x19A:
        sp4C.unk3C = ((f32) mathRnd(0, 0x32) * 0.000002f) + 0.0012f;
        sp4C.unk8 = 0xC;
        sp4C.unk60 = 0x37;
        sp4C.unk61 = 0;
        sp4C.unk42 = 0x153;
        sp4C.unk44 = 0x180011;
        sp4C.unk48 = 2;
        break;
    case 0x19B:
        sp4C.unk3C = ((f32) mathRnd(0, 0x32) * 0.000002f) + 0.0012f;
        sp4C.unk8 = 0;
        sp4C.unk60 = 0x9B;
        sp4C.unk61 = 0;
        sp4C.unk42 = 0x153;
        sp4C.unk44 = 0x80011;
        sp4C.unk48 = 2;
        break;
    case 0x19C:
        sp4C.unk8 = 2;
        sp4C.unk60 = 0x9B;
        sp4C.unk61 = 0;
        sp4C.unk3C = 0.0006f;
        sp4C.unk42 = mathRnd(0, 2) + 0x156;
        sp4C.unk44 = 0x480001;
        break;
    case 0x19D:
        sp4C.unk8 = 0xF;
        sp4C.unk60 = 0x9B;
        sp4C.unk61 = 0;
        sp4C.unk42 = 0x153;
        sp4C.unk44 = 0x180201;
        sp4C.unk3C = 0.0016f;
        break;
    case 0x19E:
        sp4C.unk8 = 0xF;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0x32;
        sp4C.unk44 = 0x480100;
        sp4C.unk3C = 0.006f;
        break;
    case 0x19F:
        sp4C.unk8 = 0xA5;
        sp4C.unk60 = 0x37;
        sp4C.unk42 = 0xDB;
        sp4C.unk44 = 0x80010;
        sp4C.unk48 = 0x400000;
        sp4C.unk3C = 0.0022f;
        break;
    case 0x1A0:
        sp4C.unk60 = 0x37;
        sp4C.unk8 = 0xF;
        sp4C.unk61 = 0x10;
        sp4C.unk42 = 0xDB;
        sp4C.unk44 = 0x480100;
        sp4C.unk3C = 0.008f;
        break;
    case 0x1BB:
        sp4C.unk8 = 0xF;
        sp4C.unk60 = 0xFF;
        sp4C.unk42 = 0x64;
        sp4C.unk44 = 0x480100;
        sp4C.unk3C = 0.006f;
        break;
    case 0x1BC:
        sp4C.unk8 = 0xA5;
        sp4C.unk60 = 0x37;
        sp4C.unk42 = 0x167;
        sp4C.unk44 = 0x80010;
        sp4C.unk48 = 0x400000;
        sp4C.unk3C = 0.0022f;
        break;
    case 0x1BD:
        sp4C.unk60 = 0x37;
        sp4C.unk8 = 0xF;
        sp4C.unk61 = 0x10;
        sp4C.unk42 = 0x64;
        sp4C.unk44 = 0x480100;
        sp4C.unk3C = 0.008f;
        break;
    case 0x1A1:
        sp4C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 0.2f;
        sp4C.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.2f;
        sp4C.unk24.x = (f32) mathRnd(0xA, 0x14) * 0.045f;
        sp4C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.02f;
        sp4C.unk3C = 0.0032f;
        sp4C.unk8 = mathRnd(0x28, 0x50);
        sp4C.unk60 = 0xFF;
        sp4C.unk4 = 0x1A2;
        sp4C.unk44 = 0x02000104;
        sp4C.unk48 = 0x200;
        sp4C.unk42 = 0x7B;
        break;
    case 0x1A2:
        sp4C.unk8 = 0x3C;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x02000104;
        sp4C.unk48 = 0x200;
        sp4C.unk42 = 0x7B;
        sp4C.unk3C = 0.0032f;
        break;
    case 0x1A3:
        sp4C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0x14, 0x14) * 0.1f;
        sp4C.unk24.y = ((f32) mathRnd(0, 0x1E) * 0.01f) + 0.2f;
        sp4C.unk3C = (f32) mathRnd(1, 0xA) * 0.00047f;
        sp4C.unk8 = mathRnd(0x5A, 0x8C);
        sp4C.unk44 = 0x80500209;
        sp4C.unk61 = 0;
        sp4C.unk42 = 0x23B;
        break;
    case 0x1A4:
        if (transform != NULL) {
            sp4C.unk30.x = transform->transl.x;
            sp4C.unk30.y = transform->transl.y + 100.0f;
            sp4C.unk30.z = transform->transl.z;
        } else {
            sp4C.unk30.x = (f32) mathRnd(-0xA, 0xA);
            sp4C.unk30.y = 260.0f;
            sp4C.unk30.z = (f32) mathRnd(-0xA, 0xA);
        }
        sp4C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(0, 0x14) * 0.03f;
        sp4C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp4C.unk3C = ((f32) mathRnd(0, 0xA) * 0.0004f) + 0.000945f;
        sp4C.unk8 = mathRnd(0xBE, 0xFA);
        sp4C.unk60 = 0x9B;
        sp4C.unk4 = 0x281;
        sp4C.unk44 = 0x81488000;
        sp4C.unk42 = mathRnd(0, 2) + 0x208;
        break;
    case 0x1A5:
        if (transform != NULL) {
            if (transform->scale <= 0.01f) {
                transform->scale = 0.01f;
            }
            sp4C.unk24.y = -transform->scale;
        } else {
            sp4C.unk24.y = (f32) mathRnd(0, 0x14) * -0.03f;
        }
        sp4C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp4C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp4C.unk3C = (f32) mathRnd(2, 0xA) * 0.0001545f;
        sp4C.unk8 = mathRnd(0x3C, 0x46);
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80480108;
        sp4C.unk42 = 0x65;
        break;
    case 0x1A6:
        if (transform != NULL) {
            sp4C.unk30.x = transform->transl.x;
            sp4C.unk30.y = transform->transl.y;
            sp4C.unk30.z = transform->transl.z;
        } else {
            sp4C.unk30.x = (f32) mathRnd(-0xA, 0xA);
            sp4C.unk30.y = 260.0f;
            sp4C.unk30.z = (f32) mathRnd(-0xA, 0xA);
        }
        sp4C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(0, 0x14) * 0.03f;
        sp4C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp4C.unk3C = ((f32) mathRnd(0, 0xA) * 0.0004f) + 0.000945f;
        sp4C.unk8 = mathRnd(0xBE, 0xFA);
        sp4C.unk60 = 0x9B;
        sp4C.unk4 = 0x281;
        sp4C.unk44 = 0x81488000;
        sp4C.unk42 = mathRnd(0, 2) + 0x208;
        sp4C.unk58[0] = 0x3200;
        sp4C.unk58[1] = 0x3200;
        sp4C.unk58[2] = 0x7800;
        sp4C.unk4C[0] = 0x3200;
        sp4C.unk4C[1] = 0x3200;
        sp4C.unk4C[2] = 0x7800;
        sp4C.unk48 = 0x20;
        break;
    case 0x1B6:
        if (transform != NULL) {
            sp4C.unk24.y = transform->scale;
        } else {
            sp4C.unk24.y = (f32) mathRnd(-3, 3) * 0.04f;
        }
        sp4C.unk8 = 0x32;
        sp4C.unk60 = 0xFF;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x88100200;
        sp4C.unk42 = 0x206;
        sp4C.unk3C = 0.035f;
        break;
    case 0x1A7:
        sp4C.unk3C = 0.016f;
        sp4C.unk8 = mathRnd(0, 0xFA) + 0x96;
        sp4C.unk61 = 0;
        sp4C.unk4 = 0x1A8;
        sp4C.unk44 = 0x80490008;
        sp4C.unk42 = 0x167;
        break;
    case 0x1A8:
        sp4C.unk8 = 0xA;
        sp4C.unk61 = 0;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80480100;
        sp4C.unk42 = 0x167;
        sp4C.unk3C = 0.026f;
        break;
    case 0x1A9:
        if (mathRnd(0, 0x50) == 0) {
            sp4C.unk8 = 0xF0;
            sp4C.unk24.x = 1.35f;
        } else {
            sp4C.unk8 = 0x78;
            sp4C.unk24.x = 0.45f;
        }
        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.scale = 1.0f;
        sp30.roll = mathRnd(0, 0xFFFF);
        sp30.pitch = mathRnd(0, 0xFFFF);
        sp30.yaw = mathRnd(0, 0xFFFF);
        mathRotateRPY(&sp30, sp4C.unk24.f);
        sp4C.unk61 = 0x10;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80100;
        sp4C.unk42 = 0xDF;
        sp4C.unk3C = 0.005f;
        break;
    case 0x1B3:
        if (transform == NULL) {
            return -1;
        }
        sp4C.unk24.x = ((f32) mathRnd(-0xF, 0xF) * 0.0001f) + 0.001f;
        sp4C.unk24.y = ((f32) mathRnd(-0xF, 0xF) * 0.0001f) + 0.001f;
        sp4C.unk24.z = ((f32) mathRnd(-0xF, 0xF) * 0.0001f) + 0.001f;
        sp4C.unk30.y = 35.0f;
        mathRotateRPY(transform, sp4C.unk24.f);
        sp4C.unk3C = (f32) mathRnd(0x14, 0x1E) * 0.00004f;
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = 0x64;
        sp4C.unk61 = 0x10;
        sp4C.unk4 = 0x1B4;
        sp4C.unk44 = 0x480200;
        sp4C.unk48 = 0x100000;
        sp4C.unk42 = 0x159;
        break;
    case 0x1B4:
        sp4C.unk3C = (f32) mathRnd(0x14, 0x1E) * 0.00006f;
        sp4C.unk60 = 0x37;
        sp4C.unk8 = 0x14;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x80201;
        sp4C.unk48 = 2;
        sp4C.unk42 = 0x159;
        break;
    case 0x1AA:
        if (transform == NULL) {
            return -1;
        }
        sp4C.unk24.x = ((f32) mathRnd(0, 0x640) * 0.001f) + 0.25f;
        mathRotateRPY(transform, sp4C.unk24.f);
        if (mathRnd(0, 1) != 0) {
            sp4C.unk60 = 0xFF;
            sp4C.unk3C = 0.005f;
        } else {
            sp4C.unk60 = 0x9B;
            sp4C.unk3C = 0.015f;
        }
        sp4C.unk8 = 0xF0;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x80480200;
        sp4C.unk42 = 0xDF;
        break;
    case 0x1AF:
        if (transform == NULL) {
            return -1;
        }
        sp4C.unk24.x = (f32) mathRnd(-1, 1) * transform->transl.x;
        sp4C.unk24.y = (f32) mathRnd(-1, 1) * transform->transl.x;
        sp4C.unk24.z = (f32) mathRnd(-1, 1) * transform->transl.x;
        sp4C.unk3C = (f32) mathRnd(0x190, 0x1F4) * 0.000013f;
        sp4C.unk60 = 0xFF;
        sp4C.unk8 = mathRnd(0, 0x14) + 0xA0;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x80080404;
        sp4C.unk42 = 0x5C;
        sp4C.unk58[0] = 0xFFFE;
        sp4C.unk58[1] = 0x8ACE;
        sp4C.unk58[2] = 0;
        sp4C.unk4C[0] = 0x4E20;
        sp4C.unk4C[1] = 0x9C40;
        sp4C.unk4C[2] = 0xFFFE;
        sp4C.unk48 = 0x20;
        break;
    case 0x1B0:
        if (transform == NULL) {
            return -1;
        }
        sp4C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 0.4f;
        sp4C.unk30.z = (f32) mathRnd(-0x14, 0x14) * 0.4f;
        sp4C.unk60 = 0xFF;
        sp4C.unk3C = 0.5f;
        sp4C.transform.yaw = mathRnd(0, 0xFFFF);
        sp4C.transform.pitch = mathRnd(0, 0xFFFF);
        sp4C.transform.yaw = mathRnd(0, 0xFFFF);
        sp4C.unk8 = 0xA0;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x06100214;
        sp4C.unk42 = 0x167;
        sp4C.transform.transl.x = 0.0f;
        sp4C.transform.transl.y = 0.0f;
        sp4C.transform.transl.z = 0.0f;
        break;
    case 0x1B1:
        if (transform == NULL) {
            return -1;
        }
        sp4C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 0.4f;
        sp4C.unk30.z = (f32) mathRnd(-0x14, 0x14) * 0.4f;
        sp4C.unk3C = (f32) mathRnd(1, 5) * 0.113f * transform->transl.x;
        sp4C.unk60 = 0xFF;
        sp4C.transform.yaw = mathRnd(0, 0xFFFF);
        sp4C.transform.pitch = mathRnd(0, 0xFFFF);
        sp4C.transform.yaw = mathRnd(0, 0xFFFF);
        sp4C.unk8 = 0xA0;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x06100214;
        sp4C.unk42 = 0x30;
        sp4C.transform.transl.x = 0.0f;
        sp4C.transform.transl.y = 0.0f;
        sp4C.transform.transl.z = 0.0f;
        break;
    case 0x1B2:
        sp4C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.12f;
        sp4C.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.12f;
        sp4C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.12f;
        sp4C.unk3C = (f32) mathRnd(0xC8, 0x3E8) * 0.000013f;
        sp4C.unk60 = mathRnd(0x64, 0xC8) + 0x37;
        sp4C.unk8 = mathRnd(0, 0x28) + 0x3C;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x81480204;
        sp4C.unk42 = 0x30;
        break;
    case 0x1AE:
        sp4C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.12f;
        sp4C.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.12f;
        sp4C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.12f;
        sp4C.unk3C = (f32) mathRnd(0xC8, 0x3E8) * 0.000013f;
        sp4C.unk60 = mathRnd(0x64, 0xC8) + 0x37;
        sp4C.unk8 = mathRnd(0, 0x28) + 0x3C;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x80480104;
        sp4C.unk48 = 8;
        sp4C.unk42 = 0x30;
        break;
    case 0x1AB:
        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.scale = 1.0f;
        sp4C.unk30.x = 45.0f;
        sp30.roll = mathRnd(0, 0xFFFF);
        sp30.pitch = mathRnd(0, 0xFFFF);
        sp30.yaw = mathRnd(0, 0xFFFF);
        mathRotateRPY(&sp30, sp4C.unk30.f);
        sp4C.unk24.x = sp4C.unk30.x / 100.0f;
        sp4C.unk24.y = sp4C.unk30.y / 100.0f;
        sp4C.unk24.z = sp4C.unk30.z / 100.0f;
        sp4C.unk3C = (f32) mathRnd(0xC8, 0x3E8) * 0.000007f;
        sp4C.unk60 = mathRnd(0x64, 0xC8) + 0x37;
        sp4C.unk8 = 0x50;
        sp4C.unk61 = 0x10;
        sp4C.unk44 = 0x80480504;
        sp4C.unk42 = 0x30;
        break;
    case 0x1AC:
        sp4C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 1.2f;
        sp4C.unk30.y = (f32) mathRnd(-0x14, 0x14) * 1.2f;
        sp4C.unk30.z = (f32) mathRnd(-0x14, 0x14) * 1.2f;
        sp4C.unk3C = (f32) mathRnd(0x1F4, 0x3E8) * 0.000068f;
        sp4C.unk60 = mathRnd(0x9B, 0xFF);
        sp4C.unk8 = mathRnd(0, 0x28) + 0x1E;
        sp4C.unk61 = 0;
        sp4C.unk44 = 0x80180104;
        sp4C.unk42 = 0x60;
        sp4C.unk4C[0] = 0x6400;
        sp4C.unk4C[1] = (mathRnd(0, 0x55) + 0xAA) << 8;
        sp4C.unk4C[2] = (mathRnd(0, 0x37) + 0xC8) << 8;
        sp4C.unk58[0] = 0xFF00;
        sp4C.unk58[1] = 0xFF00;
        sp4C.unk58[2] = 0xFF00;
        sp4C.unk48 = 0x20;
        break;
    case 0x1AD:
        sp4C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 0.4f;
        sp4C.unk30.y = (f32) mathRnd(-0x14, 0x14) * 0.4f;
        sp4C.unk30.z = (f32) mathRnd(-0x14, 0x14) * 0.4f;
        sp4C.unk3C = (f32) mathRnd(0xC8, 0x5DC) * 0.00006f;
        sp4C.unk8 = mathRnd(0, 0x28) + 0x1E;
        sp4C.unk60 = mathRnd(0xB4, 0xC8) + 0x37;
        sp4C.unk61 = 0;
        sp4C.unk44 = 0x80580104;
        sp4C.unk42 = 0xDC;
        sp4C.unk4C[0] = 0xC800;
        sp4C.unk4C[1] = (mathRnd(0, 0x37) + 0xC8) << 8;
        sp4C.unk4C[2] = (mathRnd(0, 0x19) + 0xE6) << 8;
        sp4C.unk58[0] = 0xFF00;
        sp4C.unk58[1] = 0xFF00;
        sp4C.unk58[2] = 0xFF00;
        sp4C.unk48 = 0x20;
        break;
    case 0x1B9:
        sp4C.unk30.z = (f32) mathRnd(-0x2BC, 0x2BC) * 0.01f;
        sp4C.unk30.x = ((f32) mathRnd(0, 0x3E8) * -0.1f) + 10.0f;
        sp4C.unk30.y = sp4C.unk30.x * 0.69f;
        sp4C.unk24.x = ((f32) mathRnd(0, 0xA) * -0.01f) + -0.15f;
        sp4C.unk24.y = sp4C.unk24.x * 0.69f;
        sp4C.unk3C = (f32) mathRnd(1, 6) * 0.00015f;
        sp4C.unk8 = 0xBE;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x06000100;
        sp4C.unk42 = 0x20;
        sp4C.transform.roll = 0;
        sp4C.transform.pitch = 0x5FB4;
        sp4C.transform.yaw = -0x3FFF;
        sp4C.transform.transl.x = 0.0f;
        sp4C.transform.transl.y = 0.0f;
        sp4C.transform.transl.z = 0.0f;
        break;
    case 0x1BF:
        sp4C.unk30.x = (f32) mathRnd(-0x64, 0x64) * 0.1f;
        sp4C.unk30.y = (f32) mathRnd(0, 0x3E8) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0x64, 0x64) * 0.1f;
        sp4C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.03f;
        sp4C.unk24.y = (f32) mathRnd(0x1F4, 0x258) * 0.0065f;
        sp4C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.03f;
        sp4C.unk8 = 0x15E;
        sp4C.unk60 = 0xFF;
        sp4C.unk48 = 0x300020;
        sp4C.unk44 = 0x03008000;
        sp4C.unk58[0] = 0xFFFF;
        sp4C.unk58[1] = 0xFFFF;
        sp4C.unk58[2] = 0xFFFF;
        sp4C.unk4C[0] = 0x63BF;
        sp4C.unk4C[1] = 0x9E7;
        sp4C.unk4C[2] = 0x3E8;
        sp4C.unk42 = 0x23B;
        sp4C.unk3C = 0.0105f;
        break;
    case 0x1C0:
        sp4C.unk30.x = (f32) mathRnd(-0x2BC, 0x2BC) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0x2BC, 0x2BC) * 0.1f;
        sp4C.unk24.y = (f32) mathRnd(0x1F4, 0x258) * 0.0065f;
        sp4C.unk8 = 0x96;
        sp4C.unk60 = 0xFF;
        sp4C.unk48 = 0x200000;
        sp4C.unk44 = 0x02000200;
        sp4C.unk42 = 0x23B;
        sp4C.unk3C = 0.0105f;
        break;
    case 0x1C1:
        sp4C.unk30.x = (f32) mathRnd(-0x2BC, 0x2BC) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0x2BC, 0x2BC) * 0.1f;
        sp4C.unk24.y = (f32) mathRnd(0x1F4, 0x258) * 0.0025f;
        sp4C.unk3C = (f32) mathRnd(0x1E, 0x32) * 0.002f;
        sp4C.unk8 = 0x96;
        sp4C.unk60 = 0x9B;
        sp4C.unk48 = 0x20;
        sp4C.unk44 = 0x80100;
        sp4C.unk58[0] = mathRnd(0, 0x7530) + 0x63BF;
        sp4C.unk58[1] = (u16) ((s32) sp4C.unk58[0] / mathRnd(1, 3));
        sp4C.unk58[2] = 0;
        sp4C.unk4C[0] = mathRnd(0, 0x2710);
        sp4C.unk4C[1] = sp4C.unk4C[0] / mathRnd(1, 3);
        sp4C.unk4C[2] = 0;
        sp4C.unk42 = 0x60;
        break;
    case 0x1C2:
        sp4C.unk30.z = (f32) mathRnd(-0xC8, 0xC8) * 0.1f;
        sp4C.unk30.y = (f32) mathRnd(-0xC8, 0xC8) * 0.1f;
        sp4C.unk24.z = (f32) mathRnd(0xC8, 0x320) * 0.0065f;
        if (mathRnd(0, 1) != 0) {
            sp4C.unk24.z *= -1.0f;
        }
        sp4C.unk24.y = (f32) mathRnd(0xC8, 0x320) * 0.0065f;
        if (mathRnd(0, 1) != 0) {
            sp4C.unk24.y *= -1.0f;
        }
        sp4C.unk3C = 0.008f;
        sp4C.unk8 = mathRnd(0, 0x1E) + 0x14;
        sp4C.unk60 = 0xFF;
        sp4C.unk48 = 0x200000;
        sp4C.unk44 = 0x02000200;
        sp4C.unk42 = 0x23B;
        break;
    case 0x1BA:
        sp4C.unk30.y = 130.0f;
        sp4C.unk30.x = (f32) mathRnd(-0x3E8, 0x3E8) * 0.1f;
        sp4C.unk30.z = (f32) mathRnd(-0xC8, 0xC8) * 0.01f;
        sp4C.unk30.y = sp4C.unk30.x * 0.69f;
        sp4C.unk3C = (f32) mathRnd(1, 6) * 0.00035f;
        sp4C.unk8 = 0x82;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x01000000;
        sp4C.unk48 = 0x200000;
        sp4C.unk42 = 0x20;
        break;
    case 0x1B8:
        sp4C.unk30.x = (f32) mathRnd(-0xBB8, 0xBB8) * 0.01f;
        sp4C.unk30.z = (f32) mathRnd(-0xBB8, 0xBB8) * 0.01f;
        sp4C.unk3C = (f32) mathRnd(1, 4) * 0.003f;
        sp4C.unk8 = 0x5A;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x0A100100;
        sp4C.unk42 = 0x56;
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
