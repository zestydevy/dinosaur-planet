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
void swcPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void swcPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void swcPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void swcPartfx_Func_20(s32 arg0) {
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
s32 swcPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp3C;
    s32 temp_v0;
    s32 pad;

    data_40 += 0.001f;
    if (data_40 > 1.0f) {
        data_40 = 0.1f;
    }
    data_44 += 0.0003f;
    if (data_44 > 1.0f) {
        data_44 = 0.3f;
    }
    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX swcning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
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
    case 0x41A:
    case 0x41B:
    case 0x41C:
    case 0x41D:
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
        sp3C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp3C.unk24.y = (f32) mathRnd(0xA, 0x50) * 0.002f;
        sp3C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp3C.unk3C = (f32) mathRnd(5, 0x19) * 0.0001f;
        sp3C.unk8 = mathRnd(0x122, 0x15E);
        sp3C.unk60 = 0xFF;
        sp3C.transform.yaw = mathRnd(0, 0xFFFF);
        sp3C.transform.pitch = mathRnd(0, 0xFFFF);
        sp3C.transform.yaw = mathRnd(0, 0xFFFF);
        sp3C.transform.transl.x = (f32) mathRnd(0xE6, 0x320);
        sp3C.transform.transl.y = (f32) mathRnd(0xE6, 0x320);
        sp3C.transform.transl.z = (f32) mathRnd(0xE6, 0x320);
        sp3C.unk48 = 0x01000020;
        sp3C.unk44 = 0x86000008;
        if (transform != NULL) {
            sp3C.unk30.x = transform->transl.x;
            sp3C.unk30.y = transform->transl.y;
            sp3C.unk30.z = transform->transl.z;
            if (transform->flags != 0) {
                sp3C.unk44 = 0x86800008;
                sp3C.unk60 = 0x41;
            }
        }
        sp3C.unk4C[0] = sp3C.unk58[0] = mathRnd(0, 0x9C40) + 0x63BF;
        sp3C.unk4C[1] = sp3C.unk58[1] = mathRnd(0, 0x9C40) + 0x3CAF;
        sp3C.unk4C[2] = sp3C.unk58[2] = mathRnd(0, 0x2710) + 0x159F;
        sp3C.unk42 = id - 0x78;
        break;
    case 0x41E:
    case 0x41F:
    case 0x420:
    case 0x421:
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
        sp3C.unk30.x = transform->transl.x;
        sp3C.unk30.y = transform->transl.y;
        sp3C.unk30.z = transform->transl.z;
        sp3C.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp3C.unk24.y = (f32) mathRnd(0x64, 0xC8) * 0.001f;
        sp3C.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.004f;
        sp3C.unk3C = (f32) mathRnd(5, 0x19) * 0.0001f;
        sp3C.unk8 = mathRnd(0x708, 0x7D0);
        sp3C.unk60 = 0x9B;
        sp3C.transform.yaw = mathRnd(0, 0xFFFF);
        sp3C.transform.pitch = mathRnd(0, 0xFFFF);
        sp3C.transform.yaw = mathRnd(0, 0xFFFF);
        sp3C.transform.transl.x = (f32) mathRnd(0xE6, 0x320);
        sp3C.transform.transl.y = (f32) mathRnd(0xE6, 0x320);
        sp3C.transform.transl.z = (f32) mathRnd(0xE6, 0x320);
        sp3C.unk48 = 0x31000020;
        sp3C.unk44 = 0x8E000008;
        sp3C.unk4C[0] = sp3C.unk58[0] = mathRnd(0, (id - 0x41D) * 0x2710) + 0x63BF;
        sp3C.unk4C[1] = sp3C.unk58[1] = mathRnd(0, (id - 0x41D) * 0x2710) + 0x3CAF;
        sp3C.unk4C[2] = sp3C.unk58[2] = mathRnd(0, 0x2710) + 0x159F;
        sp3C.unk42 = id - 0x7C;
        break;
    case 0x422:
        if (data == NULL) {
            return 0;
        }
        sp3C.unk3C = 0.02f;
        sp3C.unk8 = mathRnd(0xA, 0xD);
        sp3C.unk60 = (u8) *(u16*)data;
        sp3C.unk44 = 0x80100;
        sp3C.unk44 = 0x80100;
        sp3C.unk42 = 0x64;
        sp3C.unk61 = 0x1E;
        break;
    case 0x423:
        sp3C.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.3f;
        sp3C.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.3f;
        sp3C.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.3f;
        sp3C.unk3C = (f32) mathRnd(5, 0xB) * 0.001f;
        sp3C.unk8 = 0x3C;
        sp3C.unk44 = 0x80110;
        sp3C.unk61 = 0x10;
        sp3C.unk42 = 0xDE;
        break;
    case 0x424:
        sp3C.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.3f;
        sp3C.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.3f;
        sp3C.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.3f;
        sp3C.unk24.x = (f32) mathRnd(-5, 5) * 0.1f;
        sp3C.unk24.y = (f32) mathRnd(3, 0xA) * 0.1f;
        sp3C.unk24.z = (f32) mathRnd(-5, 5) * 0.1f;
        sp3C.unk3C = (f32) mathRnd(5, 0xB) * 0.0005f;
        sp3C.unk8 = 0x3C;
        sp3C.unk44 = 0x01480200;
        sp3C.unk61 = 0x10;
        sp3C.unk42 = 0xDE;
        break;
    case 0x425:
        sp3C.unk24.y = (f32) mathRnd(8, 0xA) * 0.05f;
        if (mathRnd(0, 0x28) != 0) {
            sp3C.unk3C = (f32) mathRnd(8, 0x14) * 0.001f;
            sp3C.unk8 = mathRnd(0x5A, 0x78);
        } else {
            sp3C.unk3C = (f32) mathRnd(0x15, 0x29) * 0.001f;
            sp3C.unk8 = 0x1CC;
        }
        sp3C.unk44 = 0x80180200;
        sp3C.unk48 = 0x01000020;
        sp3C.unk42 = 0x162;
        sp3C.unk60 = 0x7F;
        sp3C.unk58[0] = sp3C.unk58[1] = sp3C.unk58[2] = 0x3FFF;
        sp3C.unk4C[0] = sp3C.unk4C[1] = sp3C.unk4C[2] = 0xFFFF;
        break;
    case 0x426:
        sp3C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.05f;
        sp3C.unk24.y = (f32) mathRnd(8, 0x14) * 0.05f;
        sp3C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.05f;
        sp3C.unk8 = 0x32;
        sp3C.unk44 = 0x03000200;
        sp3C.unk48 = 0x200020;
        sp3C.unk42 = 0x33;
        sp3C.unk60 = 0xFF;
        sp3C.unk58[0] = 0xFFFF;
        sp3C.unk58[1] = 0xFFFF;
        sp3C.unk58[2] = 0xFFFF;
        sp3C.unk4C[0] = 0xFFFF;
        sp3C.unk3C = 0.003f;
        sp3C.unk4C[1] = sp3C.unk4C[2] = mathRnd(0, 0x8000);
        break;
    case 0x427:
        sp3C.unk30.x = (f32) mathRnd(-0x64, 0x64) / 20.0f;
        sp3C.unk30.y = (f32) mathRnd(-0x32, 0x32) / 10.0f;
        sp3C.unk30.z = (f32) mathRnd(-0x64, 0x64) / 20.0f;
        sp3C.unk24.y = (f32) mathRnd(1, 4) * 0.035f;
        sp3C.unk3C = ((f32) mathRnd(0, 0xA) * 0.0001f) + 0.0015f;
        sp3C.unk8 = 0xA0;
        sp3C.unk61 = 0;
        sp3C.unk44 = 0x100200;
        sp3C.unk42 = 0x63;
        break;
    case 0x42B:
        if (data == NULL) {
            return 0;
        }
        sp3C.unk3C = 0.013f;
        sp3C.unk8 = mathRnd(0xA, 0xD);
        sp3C.unk60 = (u8) *(u16*)data;
        sp3C.unk44 = 0x80100;
        sp3C.unk44 = 0x80100;
        sp3C.unk42 = 0x17D;
        sp3C.unk61 = 0x1E;
        break;
    case 0x42C:
        sp3C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.025f;
        sp3C.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.02f;
        sp3C.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.025f;
        sp3C.unk8 = 0x6E;
        sp3C.unk44 = 0x8A100208;
        sp3C.unk48 = 0x20;
        sp3C.unk42 = 0x5F;
        sp3C.unk58[0] = 0xFFFF;
        sp3C.unk58[1] = 0xFFFF;
        sp3C.unk58[2] = 0xFFFF;
        sp3C.unk4C[0] = 0x400;
        sp3C.unk4C[1] = 0xEA60;
        sp3C.unk4C[2] = 0x1000;
        sp3C.unk3C = 0.01f;
        break;
    case 0x42D:
        sp3C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp3C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp3C.unk8 = 0x258;
        sp3C.unk60 = 0x7F;
        sp3C.unk44 = 0x0A100100;
        sp3C.unk48 = 0x20;
        sp3C.unk42 = 0x62;
        sp3C.unk58[0] = 0x400;
        sp3C.unk58[1] = 0xEA60;
        sp3C.unk58[2] = 0x1000;
        sp3C.unk4C[0] = 0;
        sp3C.unk4C[1] = 0xC350;
        sp3C.unk4C[2] = 0;
        sp3C.unk3C = 0.1f;
        break;
    default:
        // STUBBED_PRINTF("PARTFX swcning type < %d > not found.. returning\n", id); // default.dol
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
