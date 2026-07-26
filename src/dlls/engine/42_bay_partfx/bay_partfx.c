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

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void bayPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void bayPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void bayPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void bayPartfx_Func_20(s32 arg0) {
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
s32 bayPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp34;
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
        // STUBBED_PRINTF("\nPARTFX bayning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
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
    case 0x3B6:
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
            sp34.unk30.x = transform->transl.x;
            sp34.unk30.z = transform->transl.z;
        } else {
            sp34.unk30.x = 0;
            sp34.unk30.z = 0;
        }
        sp34.unk30.y = 0.0f;
        sp34.unk24.y = (f32) mathRnd(0xF, 0x23) * 0.008f;
        sp34.unk3C = (f32) mathRnd(6, 0xA) * 0.00155f;
        sp34.unk8 = 0x3C;
        sp34.unk60 = 0xFF;
        sp34.unk44 = 0x80180100;
        sp34.unk42 = 0x5F;
        sp34.unk58[0] = 0xFFFF;
        sp34.unk58[1] = 0xFFFF;
        sp34.unk58[2] = 0x63BF;
        sp34.unk4C[0] = 0xFFFF;
        sp34.unk4C[1] = 0xFFFF;
        sp34.unk4C[2] = 0xB1DF;
        sp34.unk48 = 0x20;
        break;
    case 0x3B5:
        sp34.unk30.x = (f32) mathRnd(-0xA, 0xA);
        sp34.unk30.z = (f32) mathRnd(-0xA, 0xA);
        sp34.unk30.y = (f32) mathRnd(0x1E, 0x64) + 50.0f;
        sp34.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp34.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp34.unk24.y = (f32) mathRnd(0, 0x32) * 0.000001f;
        sp34.unk3C = (f32) mathRnd(0x14, 0x50) * 0.00005f;
        sp34.unk8 = mathRnd(0, 0x118) + 0xB4;
        sp34.unk60 = 0xFE;
        sp34.unk44 = 0x81008000;
        sp34.unk4 = 0x284;
        sp34.unk42 = 0x208;
        break;
    case 0x3BB:
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
            sp34.unk30.x = transform->transl.x;
            sp34.unk30.y = transform->transl.y;
            sp34.unk30.z = transform->transl.z;
        }
        sp34.unk8 = 0x96;
        sp34.unk60 = 0xFF;
        sp34.unk44 = 0x08000201;
        sp34.unk42 = 0x62;
        sp34.unk3C = 0.003f;
        break;
    case 0x3BA:
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
        sp34.unk24.y = (f32) mathRnd(1, 4) * 0.035f;
        sp34.unk3C = (f32) mathRnd(0, 0xA) * 0.0001f + 0.0015f;
        sp34.unk8 = 0xA0;
        sp34.unk61 = 0;
        sp34.unk44 = 0x100201;
        sp34.unk42 = 0x63;
        break;
    case 0x3CC:
        sp34.unk24.x = (f32) mathRnd(-0x32, 0x32) * 0.005f;
        sp34.unk24.y = (f32) mathRnd(0x1E, 0x32) * 0.005f;
        sp34.unk24.z = (f32) mathRnd(-0x32, 0x32) * 0.005f;
        sp34.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.5f;
        sp34.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.5f;
        sp34.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.5f;
        sp34.unk8 = mathRnd(0, 0x14) + 0x1E;
        sp34.unk61 = 0;
        sp34.unk60 = 0xA5;
        sp34.unk44 = 0x180108;
        sp34.unk3C = (f32) mathRnd(0x28, 0x50) * 0.0006f;
        sp34.unk42 = 0x167;
        break;
    case 0x3CB:
        sp34.unk3C = 0.01f;
        sp34.unk8 = mathRnd(0x32, 0x64);
        sp34.unk60 = 0x7F;
        sp34.unk44 = 0x01180100;
        sp34.unk42 = 0x2B;
        break;
    case 0x3CA:
        sp34.unk24.x = (f32) mathRnd(-0x32, 0x32) * 0.02f;
        sp34.unk24.y = (f32) mathRnd(0x1E, 0x32) * 0.02f;
        sp34.unk24.z = (f32) mathRnd(-0x32, 0x32) * 0.02f;
        sp34.unk3C = ((f32) mathRnd(0, 0x64) * 0.0001f) + 0.01f;
        sp34.unk8 = mathRnd(0x32, 0x46);
        sp34.unk60 = 0x7F;
        sp34.unk44 = 0x01180100;
        sp34.unk42 = 0x2B;
        break;
    case 0x3C7:
        if (transform != NULL) {
            sp34.unk30.y = transform->transl.y;
        }
        if (transform != NULL) {
            sp34.unk3C = transform->scale * 0.03f;
        } else {
            sp34.unk3C = 0.11f;
        }
        sp34.unk8 = 0xF;
        sp34.unk60 = 0x7F;
        sp34.unk44 = 0x80210;
        sp34.unk42 = 0x4F9;
        sp34.unk61 = 0x20;
        sp34.unk58[0] = 0xFF00;
        sp34.unk58[1] = 0xFF00;
        sp34.unk58[2] = 0xFF00;
        sp34.unk4C[0] = 0xFF00;
        sp34.unk4C[1] = 0xFF00;
        sp34.unk4C[2] = 0xFF00;
        sp34.unk48 = 0x02000020;
        break;
    case 0x3C2:
        if (transform != NULL) {
            sp34.unk30.x = transform->transl.x;
            sp34.unk30.z = transform->transl.z;
        } else {
            sp34.unk30.x = 0;
            sp34.unk30.z = 0;
        }
        sp34.unk30.y = 0.0f;
        sp34.unk3C = (f32) mathRnd(6, 0x14) * 0.00155f;
        sp34.unk8 = 0x3C;
        sp34.unk60 = 0xFF;
        sp34.unk44 = 0x80180108;
        sp34.unk42 = 0x5F;
        sp34.unk58[0] = 0xFFFF;
        sp34.unk58[1] = 0xFFFF;
        sp34.unk58[2] = 0x63BF;
        sp34.unk4C[0] = 0xFFFF;
        sp34.unk4C[1] = 0xFFFF;
        sp34.unk4C[2] = 0xB1DF;
        sp34.unk48 = 0x20;
        break;
    case 0x3C0:
    case 0x3C1:
        if (transform != NULL) {
            sp34.unk30.x = transform->transl.x;
            sp34.unk30.z = transform->transl.z;
        } else {
            sp34.unk30.x = 0;
            sp34.unk30.z = 0;
        }
        sp34.unk24.z = (f32) mathRnd(-0x28, 0x28) * -0.005f;
        sp34.unk24.x = (f32) mathRnd(-0x28, 0x28) * -0.005f;
        sp34.unk24.y = (f32) mathRnd(0, 0x28) * 0.005f;
        sp34.unk60 = 0xFF;
        sp34.unk8 = 0x8C;
        sp34.unk44 = 0x81000000;
        sp34.unk48 = 0x200000;
        sp34.unk42 = 0x26D;
        sp34.unk3C = 0.0053f;
        if (mathRnd(0, 3) == 3) {
            sp34.unk3C = (f32) mathRnd(1, 4) * 0.013f;
            sp34.unk44 |= 0x100100;
            sp34.unk42 = 0x2B;
            sp34.unk60 = 0x9B;
        }
        break;
    case 0x3C6:
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
            sp34.unk24.x = transform->transl.x;
            sp34.unk24.y = transform->transl.y;
            sp34.unk24.z = transform->transl.z;
        } else {
            sp34.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.001f;
            sp34.unk24.y = (f32) mathRnd(5, 0x64) * 0.01f;
            sp34.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.001f;
        }
        sp34.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp34.unk30.z = (f32) mathRnd(-0x258, 0x258) * 0.1f;
        mathRnd(-0xA, 0xA);
        sp34.unk8 = 0x28;
        sp34.unk44 = 0x01080006;
        sp34.unk42 = 0x60;
        sp34.unk60 = 0xA0;
        sp34.unk30.x = 0.0f;
        sp34.unk3C = 0.02f;
        break;
    case 0x3C5:
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
            sp34.unk30.x = transform->transl.x;
            sp34.unk30.y = transform->transl.y;
            sp34.unk30.z = transform->transl.z;
        }
        sp34.unk8 = 0x64;
        sp34.unk60 = 0xFF;
        sp34.unk44 = 0x08100201;
        sp34.unk42 = 0x60;
        sp34.unk3C = 0.02f;
        break;
    case 0x3C4:
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
        sp34.unk8 = (s32) ((transform->scale * 50.0f) + 20.0f);
        sp34.unk3C = (f32) sp34.unk8 * 0.0008f;
        sp34.unk44 = 0x0E100200;
        sp34.unk42 = 0x57;
        sp34.unk30.x = transform->transl.x;
        sp34.unk30.y = transform->transl.y;
        sp34.unk30.z = transform->transl.z;
        sp34.transform.transl.x = 0;
        sp34.transform.transl.y = 0;
        sp34.transform.transl.z = 0;
        sp34.transform.yaw = transform->yaw;
        sp34.transform.pitch = 0;
        sp34.transform.roll = 0;
        break;
    case 0x3C3:
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
            sp34.unk24.x = transform->transl.x;
            sp34.unk24.y = transform->transl.y;
            sp34.unk24.z = transform->transl.z;
        }
        sp34.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp34.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp34.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp34.unk8 = 0x3C;
        sp34.unk44 = 0x01080006;
        sp34.unk42 = 0x60;
        sp34.unk60 = 0xA0;
        sp34.unk3C = 0.01f;
        break;
    case 0x3C9:
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
        sp34.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.03f;
        sp34.unk24.y = (f32) mathRnd(0x14, 0x1E) * 0.05f;
        sp34.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.03f;
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
        sp34.unk30.y += 0.1f * (f32) mathRnd(-0x32, 0x32);
        sp34.unk30.x += 0.1f * (f32) mathRnd(-0x32, 0x32);
        sp34.unk30.z += 0.1f * (f32) mathRnd(-0x32, 0x32);
        sp34.unk8 = 0x14;
        sp34.unk44 = 0x01080006;
        sp34.unk42 = 0x60;
        sp34.unk60 = 0xA0;
        sp34.unk3C = 0.02f;
        break;
    case 0x3BE:
        sp34.unk24.y = (f32) mathRnd(1, 4) * 0.0035f;
        sp34.unk3C = ((f32) mathRnd(0, 0x3C) * 0.0001f) + 0.0035f;
        sp34.unk8 = 0xA0;
        sp34.unk61 = 0;
        sp34.unk44 = 0x80100201;
        sp34.unk42 = 0x63;
        break;
    case 0x3BD:
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
        sp34.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.01f;
        sp34.unk24.y = (f32) mathRnd(0x14, 0x1E) * 0.02f;
        sp34.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.01f;
        sp34.unk30.x = (f32) mathRnd(-0x96, 0x96) * 0.1f;
        if (transform != NULL) {
            sp34.unk30.z = transform->transl.z;
        } else {
            sp34.unk30.z = 0.0f;
        }
        if (transform != NULL) {
            sp34.unk30.y = transform->transl.y;
        } else {
            sp34.unk30.y = -2.0f;
        }
        sp34.unk30.z += 0.1f * (f32) mathRnd(-0x32, -0xA);
        sp34.unk8 = 0x1E;
        sp34.unk44 = 0x0108000E;
        sp34.unk42 = 0x60;
        sp34.unk60 = 0xBE;
        sp34.unk3C = 0.007f;
        break;
    case 0x3BC:
        if (transform == NULL) {
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.scale = 1.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
        }
        sp34.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.005f;
        sp34.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.005f;
        sp34.unk24.z = (f32) mathRnd(0, 0x12C) * 0.001f;
        sp34.unk30.x = (f32) mathRnd(-0xC8, 0xC8) * 0.1f;
        sp34.unk30.z = (f32) mathRnd(-0xC8, 0xC8) * 0.1f;
        sp34.unk3C = (f32) mathRnd(4, 8) * 0.003f;
        sp34.unk8 = 0x46;
        sp34.unk60 = 0x64;
        sp34.unk61 = 0;
        sp34.unk44 = 0x180108;
        sp34.unk42 = 0x2B;
        break;
    default:
        // STUBBED_PRINTF("PARTFX bayning type < %d > not found.. returning\n", id); // default.dol
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
