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
void nwPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void nwPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void nwPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void nwPartfx_Func_20(s32 arg0) {
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
s32 nwPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp4C;
    s32 _pad;
    s32 _pad2;
    SRT sp2C;
    s32 _pad3;

    data_40 += 0.001f;
    if (data_40 > 1.0f) {
        data_40 = 0.1f;
    }
    data_44 += 0.0003f;
    if (data_44 > 1.0f) {
        data_44 = 0.3f;
    }
    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX nwaning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
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
    case 0xC8:
        sp4C.unk30.x = (f32) mathRnd(-6, 6);
        sp4C.unk30.y = (f32) mathRnd(-6, 6);
        sp4C.unk30.z = (f32) mathRnd(-6, 6);
        sp4C.unk3C = (f32) mathRnd(4, 8) * 0.009f;
        sp4C.unk8 = 0x24;
        sp4C.unk60 = 0x41;
        sp4C.unk44 = 0x100111;
        sp4C.unk42 = 0x61;
        break;
    case 0xC9:
        if (data == NULL) {
            return -1;
        }
        sp4C.unk24.x = ((Vec3f*)data)->x;
        sp4C.unk24.y = ((Vec3f*)data)->y;
        sp4C.unk24.z = ((Vec3f*)data)->z;
        sp4C.unk3C = (f32) mathRnd(4, 8) * 0.001f;
        sp4C.unk8 = 0x46;
        sp4C.unk60 = 0x78;
        sp4C.unk61 = 0x24;
        sp4C.unk44 = 0x180100;
        sp4C.unk42 = 0x5F;
        break;
    case 0xCA:
        if (transform == NULL) {
            return 0;
        }
        sp4C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp4C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.01f;
        sp4C.unk24.z = (f32) mathRnd(0x14, 0x1E) * 0.02f;
        sp2C.roll = 0;
        sp2C.pitch = 0;
        sp2C.transl.x = 0.0f;
        sp2C.transl.y = 0.0f;
        sp2C.transl.z = 0.0f;
        sp2C.scale = 1.0f;
        sp2C.yaw = transform->yaw;
        mathRotateRPY(&sp2C, sp4C.unk24.f);
        sp4C.unk3C = (f32) mathRnd(4, 8) * 0.002f;
        sp4C.unk8 = 0x46;
        sp4C.unk60 = 0x64;
        sp4C.unk61 = 0;
        sp4C.unk44 = 0x180108;
        if (transform->roll == 0) {
            sp4C.unk42 = 0x2B;
        } else if (transform->roll == 1) {
            sp4C.unk42 = 0x1A1;
        } else if (transform->roll == 2) {
            sp4C.unk42 = 0x162;
        } else {
            sp4C.unk42 = 0x2B;
        }
        break;
    case 0xCB:
        if (transform == NULL) {
            return 0;
        }
        sp4C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.025f;
        sp4C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.045f;
        sp4C.unk24.z = (f32) mathRnd(0x14, 0x1E) * 0.025f;
        sp2C.roll = 0;
        sp2C.pitch = 0;
        sp2C.transl.x = 0.0f;
        sp2C.transl.y = 0.0f;
        sp2C.transl.z = 0.0f;
        sp2C.scale = 1.0f;
        sp2C.yaw = transform->yaw;
        mathRotateRPY(&sp2C, sp4C.unk24.f);
        sp4C.unk3C = (f32) mathRnd(4, 8) * 0.0001f;
        sp4C.unk8 = 0x46;
        sp4C.unk60 = 0xFF;
        sp4C.unk61 = 0;
        sp4C.unk44 = 0x01080100;
        if (transform->roll == 0) {
            sp4C.unk42 = 0x2B;
        } else if (transform->roll == 1) {
            sp4C.unk42 = 0x1A1;
        } else if (transform->roll == 2) {
            sp4C.unk42 = 0x162;
        } else {
            sp4C.unk42 = 0x2B;
        }
        break;
    case 0xCC:
        sp4C.unk30.x = (f32) mathRnd(-0x28, 0x28);
        sp4C.unk30.y = (f32) mathRnd(1, 2) * 180.0f;
        sp4C.unk30.z = (f32) mathRnd(-0x28, 0x28);
        sp4C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.04f;
        sp4C.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.04f;
        sp4C.unk3C = (f32) mathRnd(4, 8) * 0.0004f;
        sp4C.unk8 = 0xFA;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80108;
        sp4C.unk42 = 0x5C;
        break;
    case 0xCD:
        sp4C.unk30.x = (f32) mathRnd(0, 0xFA);
        sp4C.unk30.y = (f32) mathRnd(-5, 5) + ((sp4C.unk30.x / 20.0f) + 20.0f);
        sp4C.unk30.z = sp4C.unk30.x * 0.7f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.00004f;
        sp4C.unk8 = 0xFA;
        sp4C.unk60 = 0x7D;
        sp4C.unk44 = 0x80080118;
        sp4C.unk42 = 0x5C;
        break;
    case 0xCE:
        sp4C.unk30.x = (f32) mathRnd(-0xA, 0xA) + 290.0f;
        sp4C.unk30.y = (f32) mathRnd(-8, 8) + 40.0f;
        sp4C.unk30.z = (f32) mathRnd(-0xA, 0xA) + 175.0f;
        sp4C.unk24.y = (f32) mathRnd(0, 0xA) * 0.03f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.0003f;
        sp4C.unk8 = (s32) ((f32) mathRnd(0, 0x14) + 80.0f);
        sp4C.unk60 = 0x37;
        sp4C.unk44 = 0x180100;
        sp4C.unk42 = 0x4C;
        break;
    case 0xCF:
        sp4C.unk30.x = -(f32) mathRnd(0, 0xFA);
        sp4C.unk30.y = (f32) mathRnd(-5, 5) + ((sp4C.unk30.x / 20.0f) + 20.0f);
        sp4C.unk30.z = -sp4C.unk30.x;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.00004f;
        sp4C.unk8 = 0xFA;
        sp4C.unk60 = 0x7D;
        sp4C.unk44 = 0x80080118;
        sp4C.unk42 = 0x5C;
        break;
    case 0xD0:
        sp4C.unk30.x = (f32) mathRnd(-0xA, 0xA) + -305.0f;
        sp4C.unk30.y = (f32) mathRnd(-8, 8) + 40.0f;
        sp4C.unk30.z = (f32) mathRnd(-0xA, 0xA) + 300.0f;
        sp4C.unk24.y = (f32) mathRnd(0, 0xA) * 0.03f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.0003f;
        sp4C.unk8 = (s32) ((f32) mathRnd(0, 0x14) + 80.0f);
        sp4C.unk60 = 0x37;
        sp4C.unk44 = 0x180100;
        sp4C.unk42 = 0x4C;
        break;
    case 0xD1:
        sp4C.unk3C = (f32) mathRnd(0x46, 0x50) * 0.0003f;
        sp4C.unk8 = mathRnd(0, 0xF) + 0x14;
        sp4C.unk61 = 0;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x180210;
        sp4C.unk42 = 0x159;
        break;
    case 0xD2:
        sp4C.unk8 = 0x50;
        sp4C.unk44 = 0x400000;
        sp4C.unk42 = 0x159;
        sp4C.unk3C = 0.01f;
        break;
    case 0xD3:
        sp4C.unk30.x = -(f32) mathRnd(0, 0xFA);
        sp4C.unk30.y = (f32) mathRnd(-5, 5) + 10.0f;
        sp4C.unk30.z = (f32) mathRnd(-5, 5);
        sp4C.unk24.z = (f32) mathRnd(-5, 5) * 0.1f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.00006f;
        sp4C.unk8 = 0xA0;
        sp4C.unk60 = 0x7D;
        sp4C.unk44 = 0x180108;
        sp4C.unk42 = 0x5C;
        break;
    case 0xD4:
        sp4C.unk30.x = (f32) mathRnd(-0xA, 0x14);
        sp4C.unk30.y = (f32) mathRnd(0, 0x1C);
        sp4C.unk30.z = (f32) mathRnd(-0x14, 0x14);
        sp4C.unk24.y = (f32) mathRnd(0, 0xA) * 0.05f;
        sp4C.unk3C = (f32) mathRnd(0x28, 0x50) * 0.0013f;
        sp4C.unk8 = (s32) ((f32) mathRnd(0, 0x14) + 110.0f);
        sp4C.unk60 = 0x37;
        sp4C.unk44 = 0x180100;
        sp4C.unk42 = 0x4C;
        break;
    case 0xD5:
        sp4C.unk4 = 0xD6;
        sp4C.unk8 = 0x64;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80000;
        sp4C.unk42 = 0x159;
        sp4C.unk3C = 0.004f;
        break;
    case 0xD6:
        sp4C.unk8 = 0x28;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80100;
        sp4C.unk42 = 0x159;
        sp4C.unk3C = 0.004f;
        break;
    case 0xD7:
        sp4C.unk30.x = (f32) mathRnd(-0x8C, 0x8C) * 0.08f;
        sp4C.unk30.y = (f32) mathRnd(-0x32, 0xA) * 0.08f;
        sp4C.unk30.z = (f32) mathRnd(-0x8C, 0x8C) * 0.08f;
        sp4C.unk24.y = (f32) mathRnd(0xF, 0x23) * 0.0035f;
        sp4C.unk3C = (f32) mathRnd(1, 0xA) * 0.0015f;
        sp4C.unk8 = 0x8C;
        sp4C.unk60 = 0xFF;
        sp4C.unk44 = 0x80180100;
        sp4C.unk42 = 0x5F;
        break;
    default:
        // STUBBED_PRINTF("PARTFX nwaning type < %d > not found.. returning\n", id); // default.dol
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
