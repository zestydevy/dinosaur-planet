#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/*0x0*/ static u32 data_0[] = {
    0x000000df, 0x000001fc, 0x00000200
};
/*0xC*/ static u32 data_C = 0x000001fb;
/*0x10*/ static f32 data_10[] = {
    90.0f, 75.0f, 
    -90.0f, 75.0f, 
    100.0f, -85.0f, 
    -100.0f, -85.0f, 
    0.0f, 160.0f, 
    0.0f, -115.0f
};
/*0x40*/ static f32 data_40 = 0.1f;
/*0x44*/ static f32 data_44 = 0.3f;
/*0x48*/ static f32 data_48 = 0.10000000149011612;
/*0x4C*/ static f32 data_4C = 0.3f;
/*0x50*/ static s32 data_50 = 0;
/*0x54*/ static s32 data_54 = 0;
/*0x58*/ static f32 data_58 = 0.0;
/*0x5C*/ static f32 data_5C = 0.0;

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void shrinePartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void shrinePartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void shrinePartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void shrinePartfx_Func_20(s32 arg0) {
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
s32 shrinePartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp3C;
    s32 temp_v0;
    s32 _pad[2];

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
        sp3C.transform.transl.x = transform->transl.x;
        sp3C.transform.transl.y = transform->transl.y;
        sp3C.transform.transl.z = transform->transl.z;
        sp3C.transform.scale = transform->scale;
        sp3C.transform.roll = transform->roll;
        sp3C.transform.pitch = transform->pitch;
        sp3C.transform.yaw = transform->yaw;
        sp3C.unk62 = arg4;
    }
    sp3C.unk44 = 0;
    sp3C.unk48 = 0;
    sp3C.unk5E = id;
    sp3C.unk0 = obj;
    sp3C.unk8 = 0;
    sp3C.unk4 = -1;
    sp3C.unk60 = 0xFF;
    sp3C.unk61 = 0;
    sp3C.unk42 = 0;
    sp3C.unk30.x = 0.0f;
    sp3C.unk30.y = 0.0f;
    sp3C.unk30.z = 0.0f;
    sp3C.unk24.x = 0.0f;
    sp3C.unk24.y = 0.0f;
    sp3C.unk24.z = 0.0f;
    sp3C.unk3C = 0.0f;
    sp3C.unk58[0] = 0xFFFF;
    sp3C.unk58[1] = 0xFFFF;
    sp3C.unk58[2] = 0xFFFF;
    sp3C.unk4C[0] = 0xFFFF;
    sp3C.unk4C[1] = 0xFFFF;
    sp3C.unk4C[2] = 0xFFFF;
    sp3C.unk40 = 0;
    switch (id) {
    case 0x2AF:
        sp3C.unk8 = 0x30;
        sp3C.unk61 = 0;
        sp3C.unk3C = 0.2f;
        if (mathRnd(0, 1) != 0) {
            sp3C.unk44 = 0x08100210;
        } else {
            sp3C.unk44 = 0x180210;
        }
        sp3C.unk48 = 0x02000000;
        sp3C.unk42 = 0x205;
        break;
    case 0x2AE:
        sp3C.unk30.y = 25.0f;
        sp3C.unk8 = 0x30;
        sp3C.unk61 = 0;
        sp3C.unk44 = 0x08100210;
        sp3C.unk48 = 0x02000000;
        sp3C.unk42 = 0x205;
        sp3C.unk3C = 0.2f;
        break;
    case 0x2AD:
        sp3C.unk30.x = (f32) mathRnd(-0x64, 0x64) * 0.1f;
        sp3C.unk24.z = (f32) mathRnd(0x28, 0x3C) * -0.05f;
        sp3C.unk8 = 0x82;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x400200;
        sp3C.unk48 = 0x100;
        sp3C.unk42 = 0x156;
        sp3C.unk3C = 0.007f;
        break;
    case 0x2AC:
        sp3C.unk30.y = (f32) mathRnd(0x3E8, 0x640) * 0.1f;
        sp3C.unk24.y = (f32) mathRnd(0x28, 0x3C) * -0.02f;
        sp3C.unk8 = 0x82;
        sp3C.unk60 = 0x9B;
        sp3C.unk44 = 0x400100;
        sp3C.unk42 = 0x158;
        sp3C.unk3C = 0.007f;
        break;
    case 0x2AB:
        sp3C.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.001f;
        sp3C.unk24.y = (f32) mathRnd(0x64, 0x96) * 0.008f;
        sp3C.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.001f;
        sp3C.unk8 = 0x32;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x80000200;
        sp3C.unk48 = 0x200000;
        sp3C.unk42 = 0x23B;
        sp3C.unk3C = 0.0035f;
        break;
    case 0x2AA:
        sp3C.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.001f;
        sp3C.unk24.y = (f32) mathRnd(0x64, 0x96) * 0.006f;
        sp3C.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.001f;
        sp3C.unk8 = 0x32;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x80000200;
        sp3C.unk48 = 0x200000;
        sp3C.unk42 = 0x23B;
        sp3C.unk3C = 0.0035f;
        break;
    case 0x2A9:
        sp3C.unk30.y = (f32) mathRnd(0, 0x1F4) * 0.1f;
        sp3C.unk8 = 0x32;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x08100200;
        sp3C.unk42 = 0x26D;
        sp3C.unk3C = 0.025f;
        break;
    case 0x2A8:
        sp3C.unk24.x = (f32) mathRnd(-0x10, 0x10) * 0.26f;
        sp3C.unk24.y = (f32) mathRnd(5, 0x10) * 0.56f;
        sp3C.unk24.z = (f32) mathRnd(-0x10, 0x10) * 0.236f;
        sp3C.unk8 = 0x12;
        sp3C.unk60 = 0x9B;
        sp3C.unk44 = 0x02000000;
        sp3C.unk48 = 0x200000;
        sp3C.unk42 = 0x201;
        sp3C.unk3C = 0.00325f;
        break;
    case 0x2A7:
        sp3C.unk30.x = (f32) mathRnd(-0x14, 0x14);
        sp3C.unk30.y = (f32) mathRnd(0, 0x14) * 0.1f;
        sp3C.unk30.z = (f32) mathRnd(-0x3C, 0x14);
        sp3C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.001f;
        sp3C.unk24.z = (f32) mathRnd(7, 0xA) * -0.01f;
        sp3C.unk24.y = (f32) mathRnd(-0x28, -0x1E) * 0.005f;
        sp3C.unk3C = (f32) mathRnd(5, 0x19) * 0.00005f;
        sp3C.unk8 = mathRnd(0x186, 0x1C2);
        sp3C.unk60 = 0xFF;
        sp3C.transform.yaw = mathRnd(0, 0xFFFF);
        sp3C.transform.pitch = mathRnd(0, 0xFFFF);
        sp3C.transform.yaw = mathRnd(0, 0xFFFF);
        sp3C.transform.transl.x = (f32) mathRnd(0xE6, 0x320);
        sp3C.transform.transl.y = (f32) mathRnd(0xE6, 0x320);
        sp3C.transform.transl.z = (f32) mathRnd(0xE6, 0x320);
        sp3C.unk4C[0] = sp3C.unk58[0] = mathRnd(0, 0x9C40) + 0x63BF;
        sp3C.unk4C[1] = sp3C.unk58[1] = mathRnd(0, 0x9C40) + 0x3CAF;
        sp3C.unk4C[2] = sp3C.unk58[2] = mathRnd(0, 0x2710) + 0x159F;
        sp3C.unk48 = 0x01000020;
        sp3C.unk44 = 0x86000000;
        sp3C.unk42 = 0x3A2;
        break;
    case 0x2A6:
        sp3C.unk30.x = (f32) mathRnd(-0x14, 0x14);
        sp3C.unk30.y = (f32) mathRnd(0, 0x14) * 0.1f;
        sp3C.unk30.z = (f32) mathRnd(-0x3C, 0x14);
        sp3C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.001f;
        sp3C.unk24.z = (f32) mathRnd(7, 0xA) * -0.01f;
        sp3C.unk24.y = (f32) mathRnd(-0x28, -0x1E) * 0.003f;
        sp3C.unk3C = (f32) mathRnd(0x64, 0x78) * 0.00001f;
        sp3C.unk8 = 0x3B6;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x80080100;
        sp3C.unk42 = 0x5C;
        break;
    case 0x2A5:
        sp3C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 0.1f;
        sp3C.unk30.y = (f32) mathRnd(0, 0x3C) * 0.1f;
        sp3C.unk30.z = (f32) mathRnd(-0x78, 0x78) * 0.1f;
        sp3C.unk24.z = (f32) mathRnd(-2, 2) * -0.05f;
        sp3C.unk24.y = (f32) mathRnd(2, 5) * 0.03f;
        sp3C.unk24.z = (f32) mathRnd(-2, 2) * -0.05f;
        sp3C.unk3C = (f32) mathRnd(0x50, 0x78) * 0.0002f;
        sp3C.unk8 = 0x50;
        sp3C.unk44 = 0x180208;
        sp3C.unk48 = 0x01000000;
        sp3C.unk42 = 0x5F;
        break;
    case 0x2A4:
        sp3C.unk30.x = (f32) mathRnd(-0x5A, 0x5A) * 0.1f;
        sp3C.unk30.y = (f32) mathRnd(0, 0x64) * 0.1f;
        sp3C.unk30.z = (f32) mathRnd(-0x2BC, 0x2BC) * 0.1f;
        sp3C.unk24.x = (f32) mathRnd(-2, 2) * 0.05f;
        sp3C.unk24.y = (f32) mathRnd(2, 5) * 0.07f;
        sp3C.unk24.z = (f32) mathRnd(-2, 2) * -0.1f;
        sp3C.unk3C = (f32) mathRnd(0x50, 0xC8) * 0.0003f;
        sp3C.unk8 = 0x50;
        sp3C.unk44 = 0x180208;
        sp3C.unk48 = 0x01000000;
        sp3C.unk42 = 0x5F;
        break;
    case 0x2A3:
        sp3C.unk30.y = (f32) mathRnd(-0x190, 0x190) * 0.1f;
        sp3C.unk30.x = (f32) mathRnd(-0xC8, 0xC8) * 0.1f;
        sp3C.unk24.z = (f32) mathRnd(0x46, 0x64) * -0.03f;
        sp3C.unk3C = (f32) mathRnd(1, 0xA) * 0.003f;
        sp3C.unk8 = 0x32;
        sp3C.unk60 = 0x2D;
        sp3C.unk44 = 0x100;
        sp3C.unk42 = 0x16C;
        break;
    case 0x2A2:
        sp3C.unk30.x = (f32) mathRnd(-0xC8, 0xC8) * 0.1f;
        sp3C.unk30.y = 140.0f;
        sp3C.unk30.z = (f32) mathRnd(-0x2BC, 0x2BC) * 0.1f;
        sp3C.unk24.y = (f32) mathRnd(0xC, 0x10) * -0.16f;
        sp3C.unk24.z = (f32) mathRnd(0xC, 0x10) * 0.0136f;
        sp3C.unk8 = 0x82;
        sp3C.unk60 = 0x9B;
        sp3C.unk44 = 0x02000000;
        sp3C.unk48 = 0x200000;
        sp3C.unk42 = 0x5C;
        sp3C.unk3C = 0.0005f;
        break;
    case 0x29D:
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
        sp3C.transform.yaw = 0x3E8;
        sp3C.transform.pitch = 0x3E8;
        sp3C.transform.roll = 0x3E8;
        sp3C.transform.transl.x = 0/*.0f*/;
        sp3C.transform.transl.y = 0/*.0f*/;
        sp3C.transform.transl.z = 0/*.0f*/;
        if (transform != NULL) {
            sp3C.unk30.x = transform->transl.x;
            sp3C.unk30.y = transform->transl.y;
            sp3C.unk30.z = transform->transl.z;
        }
        sp3C.unk8 = 6;
        sp3C.unk60 = 0xE1;
        sp3C.unk44 = 0x4A0010;
        if (mathRnd(0, 1) != 0) {
            sp3C.unk48 = 0x202;
        } else {
            sp3C.unk48 = 0x102;
        }
        if (transform->scale == 0/*.0f*/) {
            sp3C.unk3C = ((f32) mathRnd(0, 3) * 0.0003f) + 0.001f;
            sp3C.unk42 = 0x32;
        } else {
            sp3C.unk3C = ((f32) mathRnd(0, 3) * 0.0003f) + 0.0013f;
            sp3C.unk42 = 0x32;
        }
        break;
    case 0x29E:
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
            sp3C.unk30.x = transform->transl.x;
            sp3C.unk30.y = transform->transl.y;
            sp3C.unk30.z = transform->transl.z;
        }
        sp3C.unk8 = 0x3C;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x480010;
        if (transform->scale == 0/*.0f*/) {
            sp3C.unk42 = 0x74;
            sp3C.unk3C = 0.0042f;
        } else {
            sp3C.unk42 = 0x74;
            sp3C.unk3C = 0.0046f;
        }
        sp3C.unk48 = 2;
        break;
    case 0x29F:
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
            sp3C.unk30.x = transform->transl.x;
            sp3C.unk30.y = transform->transl.y;
            sp3C.unk30.z = transform->transl.z;
        }
        sp3C.unk8 = 0x3C;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x480010;
        sp3C.unk48 = 2;
        if (transform->scale == 0/*.0f*/) {
            sp3C.unk42 = 0xDC;
            sp3C.unk3C = 0.008f;
        } else {
            sp3C.unk42 = 0xDC;
            sp3C.unk3C = 0.012f;
        }
        break;
    case 0x2A0:
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
        sp3C.unk8 = 0x1E;
        sp3C.unk61 = 0;
        sp3C.unk60 = 0x37;
        sp3C.unk44 = 0x180010;
        if (transform->scale == 0/*.0f*/) {
            sp3C.unk3C = (f32) mathRnd(0x14, 0x32) * 0.00013f;
            sp3C.unk42 = 0x73;
        } else {
            sp3C.unk3C = (f32) mathRnd(0x14, 0x32) * 0.00021f;
            sp3C.unk42 = 0x73;
        }
        break;
    case 0x2A1:
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
        sp3C.unk8 = 0x3C;
        sp3C.unk61 = 0;
        sp3C.unk60 = 0x37;
        sp3C.unk44 = 0x480010;
        sp3C.unk48 = 2;
        if (transform->scale == 0/*.0f*/) {
            sp3C.unk3C = (f32) mathRnd(0x46, 0x50) * 0.0001f;
            sp3C.unk42 = 0x73;
        } else {
            sp3C.unk3C = (f32) mathRnd(0x46, 0x50) * 0.00012f;
            sp3C.unk42 = 0x73;
        }
        break;
    case 0x297:
        sp3C.unk24.x = (f32) mathRnd(-0x10, 0x10) * 0.16f;
        sp3C.unk24.y = (f32) mathRnd(5, 0x10) * 0.46f;
        sp3C.unk24.z = (f32) mathRnd(-0x10, 0x10) * 0.136f;
        sp3C.unk8 = 0x54;
        sp3C.unk60 = 0x9B;
        sp3C.unk44 = 0x02000000;
        sp3C.unk48 = 0x200000;
        sp3C.unk42 = 0x1FE;
        sp3C.unk3C = 0.0125f;
        break;
    case 0x25B:
        sp3C.unk8 = 0x3C;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x02000104;
        sp3C.unk48 = 0x400;
        sp3C.unk42 = 0x7B;
        sp3C.unk3C = 0.0032f;
        break;
    case 0x25C:
    case 0x269:
    case 0x272:
    case 0x27D:
        sp3C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 0.2f;
        sp3C.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.03f;
        sp3C.unk24.z = (f32) mathRnd(0xA, 0x14) * 0.0045f;
        sp3C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp3C.unk24.y = (f32) mathRnd(0xE, 0x12) * 0.02f;
        sp3C.unk3C = 0.0017f;
        sp3C.unk8 = mathRnd(0x28, 0x50);
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x02000104;
        sp3C.unk48 = 0x400;
        if (id == 0x25C) {
            sp3C.unk42 = 0x7A;
            sp3C.unk4 = 0x25D;
        } else if (id == 0x272) {
            sp3C.unk42 = 0x202;
            sp3C.unk4 = 0x273;
        } else if (id == 0x27D) {
            sp3C.unk42 = 0x7A;
            sp3C.unk4 = 0x27E;
        } else {
            sp3C.unk42 = 0x1FE;
            sp3C.unk4 = 0x26A;
        }
        break;
    case 0x25D:
    case 0x26A:
    case 0x273:
    case 0x27E:
        sp3C.unk8 = 0x3C;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x02000104;
        sp3C.unk48 = 0x400;
        sp3C.unk3C = 0.0017f;
        if (id == 0x25D) {
            sp3C.unk42 = 0x7A;
        } else if (id == 0x273) {
            sp3C.unk42 = 0x202;
        } else if (id == 0x27E) {
            sp3C.unk42 = 0x7A;
        } else {
            sp3C.unk42 = 0x1FE;
        }
        break;
    case 0x25E:
    case 0x26B:
    case 0x274:
    case 0x27B:
        sp3C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 0.2f;
        sp3C.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.03f;
        sp3C.unk24.z = (f32) mathRnd(0xA, 0x14) * 0.0045f;
        sp3C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.005f;
        sp3C.unk24.y = (f32) mathRnd(0xE, 0x12) * 0.01f;
        sp3C.unk3C = 0.0011f;
        sp3C.unk8 = mathRnd(0x28, 0x50);
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x02000104;
        sp3C.unk48 = 0x400;
        if (id == 0x25E) {
            sp3C.unk42 = 0x79;
            sp3C.unk4 = 0x25D;
        } else if (id == 0x27B) {
            sp3C.unk42 = 0x1FB;
            sp3C.unk4 = 0x27C;
        } else if (id == 0x274) {
            sp3C.unk42 = 0x202;
            sp3C.unk4 = 0x275;
        } else {
            sp3C.unk42 = 0x1FF;
            sp3C.unk4 = 0x26C;
        }
        break;
    case 0x25F:
    case 0x26C:
    case 0x275:
    case 0x27C:
        sp3C.unk8 = 0x3C;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x02000104;
        sp3C.unk48 = 0x400;
        sp3C.unk3C = 0.0011f;
        if (id == 0x25F) {
            sp3C.unk42 = 0x79;
        } else if (id == 0x275) {
            sp3C.unk42 = 0x202;
        } else if (id == 0x27C) {
            sp3C.unk42 = 0x1FB;
        } else {
            sp3C.unk42 = 0x1FF;
        }
        break;
    case 0x260:
    case 0x261:
    case 0x262:
    case 0x278:
        sp3C.unk30.x = (f32) mathRnd(-0x26, 0x26);
        sp3C.unk30.y = (f32) mathRnd(0xA, 0x50);
        sp3C.unk30.z = (f32) mathRnd(-0x6C, 0x6C);
        sp3C.unk24.x = (f32) mathRnd(-3, 3) * 0.005f;
        sp3C.unk24.y = (f32) mathRnd(-6, 6) * 0.01f;
        sp3C.unk24.z = (f32) mathRnd(-3, 3) * 0.01f;
        sp3C.unk8 = 0x64;
        sp3C.unk60 = 0xFF;
        sp3C.unk61 = 0x10;
        sp3C.unk44 = 0x80480110;
        sp3C.unk3C = 0.0015f;
        if (id == 0x278) {
            sp3C.unk42 = (s16) data_C;
        } else {
            sp3C.unk42 = (s16) data_0[id - 0x260];
        }
        break;
    case 0x263:
    case 0x264:
    case 0x265:
    case 0x276:
        sp3C.unk30.x = (f32) mathRnd(-8, 8);
        sp3C.unk30.y = (f32) mathRnd(0, 0x50);
        sp3C.unk30.z = (f32) mathRnd(-8, 8);
        sp3C.unk24.y = (f32) mathRnd(-3, 3) * 0.05f;
        sp3C.unk8 = 0x64;
        sp3C.unk60 = 0xFF;
        sp3C.unk61 = 0x10;
        sp3C.unk44 = 0x480110;
        sp3C.unk3C = 0.0015f;
        if (id == 0x276) {
            sp3C.unk42 = (s16) data_C;
        } else {
            sp3C.unk42 = (s16) data_0[id - 0x263];
        }
        break;
    case 0x266:
    case 0x267:
    case 0x268:
    case 0x277:
        sp3C.unk30.x = (f32) mathRnd(-8, 8);
        sp3C.unk30.y = (f32) mathRnd(0, 0x50);
        sp3C.unk30.z = (f32) mathRnd(-8, 8);
        sp3C.unk24.y = (f32) mathRnd(-3, 3) * 0.05f;
        sp3C.unk8 = 0x64;
        sp3C.unk60 = 0xFF;
        sp3C.unk61 = 0x10;
        sp3C.unk44 = 0x480100;
        sp3C.unk3C = 0.0015f;
        if (id == 0x277) {
            sp3C.unk42 = (s16) data_C;
        } else {
            sp3C.unk42 = (s16) data_0[id - 0x266];
        }
        break;
    case 0x26D:
        sp3C.unk30.x = (f32) mathRnd(-0x3C, 0x3C);
        sp3C.unk30.y = (f32) mathRnd(-0x3C, 0x3C);
        sp3C.unk30.z = (f32) mathRnd(-0x12, 0x12);
        sp3C.unk24.z = (f32) mathRnd(0x28, 0x50) * 0.06f;
        sp3C.unk8 = 0xC8;
        sp3C.unk60 = 0xFF;
        sp3C.unk61 = 0x10;
        sp3C.unk44 = 0x02000200;
        sp3C.unk48 = 0x200000;
        sp3C.unk42 = 0x1FE;
        sp3C.unk3C = 0.0055f;
        break;
    case 0x26E:
        sp3C.unk8 = 0x55;
        sp3C.unk60 = 0xFF;
        sp3C.unk61 = 0x10;
        sp3C.unk44 = 0x02000200;
        sp3C.unk48 = 0x200000;
        sp3C.unk42 = 0x1FE;
        sp3C.unk3C = 0.0055f;
        break;
    case 0x26F:
        sp3C.unk24.y = (f32) mathRnd(0x28, 0x50) * 0.01f;
        sp3C.unk8 = 0x7D;
        sp3C.unk60 = 0xFF;
        sp3C.unk61 = 0x10;
        sp3C.unk44 = 0x80200;
        sp3C.unk42 = 0x125;
        sp3C.unk3C = 0.015f;
        break;
    case 0x270:
        sp3C.unk30.y = (f32) mathRnd(0, 5) * 0.1f;
        sp3C.unk8 = 0x46;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x0810020C;
        sp3C.unk42 = 0x167;
        sp3C.unk3C = 0.032f;
        break;
    case 0x279:
        temp_v0 = mathRnd(0, 5);
        sp3C.unk30.x = data_10[temp_v0 * 2];
        sp3C.unk30.y = 30.0f;
        sp3C.unk30.z = data_10[temp_v0 * 2 + 1];
        sp3C.unk24.y = (f32) mathRnd(0x28, 0x50) * 0.0035f;
        sp3C.unk8 = 0x46;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x08100204;
        sp3C.unk42 = 0x206;
        sp3C.unk3C = 0.042f;
        break;
    case 0x27A:
        temp_v0 = mathRnd(0, 5);
        sp3C.unk30.x = data_10[temp_v0 * 2];
        sp3C.unk30.y = (f32) mathRnd(0, 5) * 0.1f;
        sp3C.unk30.z = data_10[temp_v0 * 2 + 1];
        sp3C.unk30.y = (f32) mathRnd(0, 5) * 0.1f;
        sp3C.unk8 = 0x46;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x0810020C;
        sp3C.unk42 = 0x206;
        sp3C.unk3C = 0.022f;
        break;
    case 0x271:
        sp3C.unk30.y = 0.0f;
        sp3C.unk24.y = (f32) mathRnd(0x28, 0x50) * 0.01f;
        sp3C.unk8 = 0x46;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x08100204;
        sp3C.unk42 = 0x167;
        sp3C.unk3C = 0.062f;
        break;
    case 0x286:
    case 0x287:
    case 0x288:
        sp3C.unk30.y = (f32) mathRnd(-6, 2);
        sp3C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.0015f;
        sp3C.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.0015f;
        sp3C.unk8 = 0x50;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x80480208;
        sp3C.unk3C = 0.00195f;
        if (id == 0x286) {
            sp3C.unk42 = 0x160;
        } else if (id == 0x287) {
            sp3C.unk42 = 0x200;
        } else if (id == 0x288) {
            sp3C.unk42 = 0xDD;
        }
        break;
    case 0x27F:
        sp3C.unk3C = obj->srt.scale * 0.0645f;
        sp3C.unk8 = 0x28;
        sp3C.unk60 = 0x9B;
        sp3C.unk44 = 0x80080208;
        sp3C.unk42 = 0x5F;
        sp3C.unk58[0] = 0x6400;
        sp3C.unk58[1] = 0x3200;
        sp3C.unk58[2] = 0xA000;
        sp3C.unk4C[0] = 0x1F4;
        sp3C.unk4C[1] = 0;
        sp3C.unk4C[2] = 0x3E8;
        sp3C.unk48 = 0x20;
        break;
    case 0x280:
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
            sp3C.unk30.x = transform->transl.x;
            sp3C.unk30.y = transform->transl.y + 100.0f;
            sp3C.unk30.z = transform->transl.z;
        } else {
            sp3C.unk30.x = (f32) mathRnd(-0x14, 0x14);
            sp3C.unk30.y = 100.0f;
            sp3C.unk30.z = (f32) mathRnd(-0x14, 0x14);
        }
        sp3C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp3C.unk24.y = (f32) mathRnd(0, 0x14) * 0.03f;
        sp3C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp3C.unk3C = ((f32) mathRnd(0, 0xA) * 0.0004f) + 0.000945f;
        sp3C.unk8 = mathRnd(0xBE, 0xFA);
        sp3C.unk60 = 0x9B;
        sp3C.unk4 = 0x281;
        sp3C.unk44 = 0x81488000;
        sp3C.unk42 = mathRnd(0, 2) + 0x208;
        break;
    case 0x281:
        sp3C.unk24.y = (f32) mathRnd(2, 0x14) * 0.002f;
        sp3C.unk3C = 0.018445f;
        sp3C.unk8 = mathRnd(0, 0x1E) + 0xA;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x180200;
        sp3C.unk42 = 0x5F;
        sp3C.unk58[0] = 0x5000;
        sp3C.unk58[1] = 0x1E00;
        sp3C.unk58[2] = 0x7800;
        sp3C.unk4C[0] = 0x5000;
        sp3C.unk4C[1] = 0x1E00;
        sp3C.unk4C[2] = 0x7800;
        sp3C.unk48 = 0x20;
        break;
    case 0x282:
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
            sp3C.unk30.x = transform->transl.x;
            sp3C.unk30.y = transform->transl.y;
            sp3C.unk30.z = transform->transl.z;
        } else {
            sp3C.unk30.x = (f32) mathRnd(-5, 5);
            sp3C.unk30.y = (f32) mathRnd(1, 0xA);
            sp3C.unk30.z = (f32) mathRnd(-0x96, 0x96);
        }
        sp3C.unk24.x = (f32) mathRnd(-0x1E, 0x1E) * 0.01f;
        sp3C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.06f;
        sp3C.unk24.z = (f32) mathRnd(4, 4) * 0.01f;
        sp3C.unk3C = ((f32) mathRnd(0, 0xA) * 0.0002f) + 0.0002945f;
        sp3C.unk8 = mathRnd(0xE6, 0x118);
        sp3C.unk60 = 0xFF;
        sp3C.unk4 = 0x284;
        sp3C.unk44 = 0x81488200;
        sp3C.unk42 = 0x8F;
        break;
    case 0x283:
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
            sp3C.unk30.x = transform->transl.x;
            sp3C.unk30.y = transform->transl.y;
            sp3C.unk30.z = transform->transl.z;
        } else {
            sp3C.unk30.x = (f32) mathRnd(-5, 5);
            sp3C.unk30.y = (f32) mathRnd(1, 0xA);
            sp3C.unk30.z = (f32) mathRnd(-0x96, 0x96);
        }
        sp3C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.02f;
        sp3C.unk3C = ((f32) mathRnd(0, 0xA) * 0.0002f) + 0.0002945f;
        sp3C.unk8 = mathRnd(0xE6, 0x118);
        sp3C.unk60 = 0x9B;
        sp3C.unk44 = 0x80480200;
        sp3C.unk42 = 0x47;
        break;
    case 0x284:
        sp3C.unk24.y = (f32) mathRnd(2, 0x14) * 0.002f;
        sp3C.unk8 = 0x1E;
        sp3C.unk60 = 0x9B;
        sp3C.unk44 = 0x180200;
        sp3C.unk42 = 0x5F;
        sp3C.unk58[0] = 0xFF00;
        sp3C.unk58[1] = 0xFF00;
        sp3C.unk58[2] = 0x9B00;
        sp3C.unk4C[0] = 0x9600;
        sp3C.unk4C[1] = 0x1400;
        sp3C.unk4C[2] = 0x1400;
        sp3C.unk48 = 0x20;
        sp3C.unk3C = 0.004445f;
        break;
    case 0x285:
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
            sp3C.unk30.x = transform->transl.x;
            sp3C.unk30.y = transform->transl.y;
            sp3C.unk30.z = transform->transl.z;
        } else {
            sp3C.unk30.x = (f32) mathRnd(-5, 5);
            sp3C.unk30.y = (f32) mathRnd(1, 0xA);
            sp3C.unk30.z = (f32) mathRnd(-0x96, 0x96);
        }
        sp3C.unk24.y = (f32) mathRnd(2, 4) * 0.002f;
        sp3C.unk24.z = (f32) mathRnd(2, 4) * 0.006f;
        sp3C.unk3C = ((f32) mathRnd(0, 0xA) * 0.001f) + 0.01245f;
        sp3C.unk8 = mathRnd(0, 0x32) + 0x32;
        sp3C.unk60 = 0x9B;
        sp3C.unk44 = 0x180200;
        sp3C.unk42 = 0x8F;
        break;
    case 0x258:
        sp3C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp3C.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp3C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp3C.unk3C = 0.00087f;
        sp3C.unk8 = mathRnd(0x50, 0x82);
        sp3C.unk60 = 0x9B;
        sp3C.unk44 = 0x180200;
        sp3C.unk42 = 0x7B;
        break;
    case 0x259:
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
        sp3C.unk30.x = ((f32) mathRnd(-0xF, 0xF) * 0.1f) + (transform->transl.x - obj->srt.transl.x);
        sp3C.unk30.z = ((f32) mathRnd(-0xF, 0xF) * 0.1f) + (transform->transl.z - obj->srt.transl.z);
        sp3C.unk30.y = 2.0f;
        sp3C.unk24.y = (f32) mathRnd(0xF, 0x23) * 0.0015f;
        sp3C.unk3C = (f32) mathRnd(6, 0xA) * 0.00045f;
        sp3C.unk8 = 0x32;
        sp3C.unk60 = 0x9B;
        sp3C.unk44 = 0x80180100;
        sp3C.unk42 = 0x5F;
        sp3C.unk58[0] = 0xFFFF;
        sp3C.unk58[1] = 0xFFFF;
        sp3C.unk58[2] = 0xFFFF;
        sp3C.unk4C[0] = 0;
        sp3C.unk4C[1] = 0;
        sp3C.unk4C[2] = 0x3E8;
        sp3C.unk48 = 0x20;
        break;
    case 0x25A:
        sp3C.unk30.x = (f32) mathRnd(-0x14, 0x14) * 0.2f;
        sp3C.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.2f;
        sp3C.unk24.z = (f32) mathRnd(0xA, 0x14) * 0.045f;
        sp3C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp3C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.02f;
        sp3C.unk3C = 0.0032f;
        sp3C.unk8 = mathRnd(0x28, 0x50);
        sp3C.unk60 = 0xFF;
        sp3C.unk4 = 0x25B;
        sp3C.unk44 = 0x02000104;
        sp3C.unk48 = 0x400;
        sp3C.unk42 = 0x7B;
        break;
    case 0x289:
        sp3C.unk30.x = (f32) mathRnd(-0x28, 0x28) * 0.2f;
        sp3C.unk30.z = (f32) mathRnd(-0x28, 0x28) * 0.2f;
        sp3C.unk24.y = ((f32) mathRnd(0x28, 0x3C) * 0.01f) + 0.3f;
        sp3C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.0001f;
        sp3C.unk8 = mathRnd(0x14, 0x8C);
        sp3C.unk44 = 0x80400209;
        sp3C.unk61 = 0;
        sp3C.unk42 = 0x23B;
        break;
    case 0x28A:
        sp3C.unk30.x = 0.0f;
        sp3C.unk30.y = 0.0f;
        sp3C.unk30.z = -40.0f;
        sp3C.unk3C = 0.05f;
        sp3C.unk60 = 0x55;
        sp3C.unk8 = mathRnd(0x32, 0x40);
        sp3C.unk44 = 0x200;
        sp3C.unk42 = 0x5C;
        break;
    case 0x28B:
        sp3C.unk30.y = (f32) mathRnd(0, 0x12C) * 0.1f;
        sp3C.unk8 = 0x14;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x08100200;
        sp3C.unk42 = 0x159;
        sp3C.unk3C = 0.015f;
        break;
    case 0x28C:
        sp3C.unk30.x = (f32) mathRnd(-0x32, 0x32) * 0.1f;
        sp3C.unk30.y = (f32) mathRnd(0, 0xC8) * 0.1f;
        sp3C.unk30.z = (f32) mathRnd(-0x32, 0x32) * 0.1f;
        sp3C.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.001f;
        sp3C.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.001f;
        sp3C.unk3C = (f32) mathRnd(0x32, 0x64) * 0.000005f;
        sp3C.unk8 = mathRnd(0, 0x1E) + 0x64;
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x88108;
        sp3C.unk42 = 0x159;
        break;
    case 0x28D:
        sp3C.unk3C = (f32) mathRnd(0x5A, 0x64) * 0.0001f;
        sp3C.unk8 = mathRnd(0, 0x14) + 0xA;
        sp3C.unk60 = 0x7D;
        sp3C.unk44 = 0x500200;
        sp3C.unk42 = 0x159;
        break;
    case 0x28E:
        sp3C.unk30.x = (f32) mathRnd(-0x3E8, 0x3E8) * 0.1f;
        sp3C.unk30.y = (f32) mathRnd(0x12C, 0x708) * 0.1f;
        sp3C.unk30.z = (f32) mathRnd(-0x3E8, 0x3E8) * 0.1f;
        sp3C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.06f * data_48;
        sp3C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.06f * -data_48;
        sp3C.unk8 = 0x118;
        sp3C.unk60 = 0xFF;
        sp3C.unk48 = 0x300020;
        sp3C.unk44 = 0x02008000;
        sp3C.unk58[0] = 0xFFFF;
        sp3C.unk58[1] = 0xFFFF;
        sp3C.unk58[2] = 0xFFFF;
        sp3C.unk4C[0] = 0x63BF;
        sp3C.unk4C[1] = 0x9E7;
        sp3C.unk4C[2] = 0x3E8;
        sp3C.unk42 = 0x23B;
        sp3C.unk3C = 0.0015f;
        break;
    case 0x28F:
    case 0x290:
    case 0x291:
    case 0x292:
        sp3C.unk30.x = (f32) mathRnd(-0x190, 0x190) * 0.1f;
        sp3C.unk30.y = (f32) mathRnd(0, 0x64);
        sp3C.unk30.z = (f32) mathRnd(-0x190, 0x190) * 0.1f;
        sp3C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp3C.unk24.y = (f32) mathRnd(0x64, 0xC8) * 0.002f;
        sp3C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp3C.unk3C = (f32) mathRnd(5, 0x19) * 0.0001f;
        sp3C.unk8 = 0x168;
        sp3C.unk60 = 0xFF;
        sp3C.transform.yaw = mathRnd(0, 0xFFFF);
        sp3C.transform.pitch = mathRnd(0, 0xFFFF);
        sp3C.transform.yaw = mathRnd(0, 0xFFFF);
        sp3C.transform.transl.x = (f32) mathRnd(0xE6, 0x320);
        sp3C.transform.transl.y = (f32) mathRnd(0xE6, 0x320);
        sp3C.transform.transl.z = (f32) mathRnd(0xE6, 0x320);
        sp3C.unk48 = 0x20;
        sp3C.unk44 = 0x86000008;
        sp3C.unk58[0] = mathRnd(0, 0x9C40) + 0x63BF;
        sp3C.unk58[1] = mathRnd(0, 0x9C40) + 0x3CAF;
        sp3C.unk58[2] = mathRnd(0, 0x2710) + 0x159F;
        sp3C.unk4C[0] = sp3C.unk58[0];
        sp3C.unk4C[1] = sp3C.unk58[1];
        sp3C.unk4C[2] = sp3C.unk58[2];
        sp3C.unk42 = id + 0x113;
        break;
    case 0x293:
    case 0x294:
    case 0x295:
    case 0x296:
        sp3C.unk30.x = (f32) mathRnd(-0x190, 0x190) * 0.1f;
        sp3C.unk30.y = 0.5f;
        sp3C.unk30.z = (f32) mathRnd(-0x190, 0x190) * 0.1f;
        sp3C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp3C.unk24.y = (f32) mathRnd(0x64, 0xC8) * 0.001f;
        sp3C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp3C.unk3C = (f32) mathRnd(5, 0x19) * 0.0001f;
        sp3C.unk8 = 0x7D0;
        sp3C.unk60 = 0x9B;
        sp3C.transform.yaw = mathRnd(0, 0xFFFF);
        sp3C.transform.pitch = mathRnd(0, 0xFFFF);
        sp3C.transform.yaw = mathRnd(0, 0xFFFF);
        sp3C.transform.transl.x = (f32) mathRnd(0xE6, 0x320);
        sp3C.transform.transl.y = (f32) mathRnd(0xE6, 0x320);
        sp3C.transform.transl.z = (f32) mathRnd(0xE6, 0x320);
        sp3C.unk48 = 0x31000020;
        sp3C.unk44 = 0x8E000108;
        sp3C.unk58[0] = mathRnd(0, (id - 0x292) * 0x2710) + 0x63BF;
        sp3C.unk58[1] = mathRnd(0, (id - 0x292) * 0x2710) + 0x3CAF;
        sp3C.unk58[2] = mathRnd(0, 0x2710) + 0x159F;
        sp3C.unk4C[0] = sp3C.unk58[0];
        sp3C.unk4C[1] = sp3C.unk58[1];
        sp3C.unk4C[2] = sp3C.unk58[2];
        sp3C.unk42 = id + 0x10F;
        break;
    default:
        // STUBBED_PRINTF("PARTFX warning type < %d > not found.. returning\n", id); // default.dol
        return -1;
    }
    sp3C.unk44 |= flags;
    if ((sp3C.unk44 & 1) && (sp3C.unk44 & 2)) {
        // STUBBED_PRINTF("error in partfx ABS and REL %d\n", id); // default.dol
        sp3C.unk44 ^= 2;
    }
    if (sp3C.unk44 & 1) {
        if (flags & 0x200000) {
            sp3C.unk30.x += sp3C.transform.transl.x;
            sp3C.unk30.y += sp3C.transform.transl.y;
            sp3C.unk30.z += sp3C.transform.transl.z;
        } else if (sp3C.unk0 != NULL) {
            sp3C.unk30.x += sp3C.unk0->globalPosition.x;
            sp3C.unk30.y += sp3C.unk0->globalPosition.y;
            sp3C.unk30.z += sp3C.unk0->globalPosition.z;
        }
    }
    return gDLL_13_Expgfx->vtbl->func1(&sp3C, -1);
}
