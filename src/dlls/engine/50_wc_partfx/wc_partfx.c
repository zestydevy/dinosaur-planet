#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/*0x0*/ static f32 data_0 = 0.1f;
/*0x4*/ static f32 data_4 = 0.3f;
/*0x8*/ static f32 data_8 = 0.1f;
/*0xC*/ static f32 data_C = 0.3f;
/*0x10*/ static s32 data_10 = 0;
/*0x14*/ static s32 data_14 = 0;
/*0x18*/ static f32 data_18 = 0.0f;
/*0x1C*/ static f32 data_1C = 0.0f;

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void wcPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void wcPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void wcPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void wcPartfx_Func_20(s32 arg0) {
    data_8 += (gUpdateRateF * 0.001f);
    if (data_8 > 1.0f) {
        data_8 = 0.1f;
    }
    data_C += (gUpdateRateF * 0.001f);
    if (data_C > 1.0f) {
        data_C = 0.3f;
    }
    data_10 += (gUpdateRate * 100);
    if (data_10 >= 0x8000) {
        data_10 = 0;
    }
    data_1C = mathSinfInterp((s16) data_10);
    data_14 += (gUpdateRate * 50);
    if (data_14 >= 0x8000) {
        data_14 = 0;
    }
    data_18 = mathSinfInterp((s16) data_14);
}

// offset: 0x194 | func: 2 | export: 1
s32 wcPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp3C;
    s32 temp_v0;
    s32 _pad;

    data_0 += 0.001f;
    if (data_0 > 1.0f) {
        data_0 = 0.1f;
    }
    data_4 += 0.0003f;
    if (data_4 > 1.0f) {
        data_4 = 0.3f;
    }
    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX wcning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
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
    sp3C.unk5E = (u8) id;
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
    case 0x73A:
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
        sp3C.unk30.y = 65.0f;
        break;
    case 0x73B:
        sp3C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.05f;
        sp3C.unk24.y = (f32) mathRnd(8, 0x14) * 0.05f;
        sp3C.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.05f;
        sp3C.unk3C = 0.003f;
        sp3C.unk8 = 0x32;
        sp3C.unk44 = 0x03000200;
        sp3C.unk48 = 0x200020;
        sp3C.unk42 = 0x33;
        sp3C.unk60 = 0xFF;
        sp3C.unk58[0] = 0xFFFF;
        sp3C.unk58[1] = 0xFFFF;
        sp3C.unk58[2] = 0xFFFF;
        sp3C.unk4C[0] = 0xFFFF;
        sp3C.unk4C[1] = sp3C.unk4C[2] = mathRnd(0, 0x8000);
        sp3C.unk30.y = 20.0f;
        break;
    case 0x73C:
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
            sp3C.unk30.z = transform->transl.x;
            sp3C.unk30.y = transform->transl.y;
        } else {
            sp3C.unk30.z = 15.0f;
            sp3C.unk30.y = 10.0f;
        }
        sp3C.unk24.z = (f32) mathRnd(0x1E, 0x28) * 0.018f;
        sp3C.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.01f;
        sp3C.unk24.y = (f32) mathRnd(-4, 4) * 0.002f;
        sp3C.unk3C = (f32) mathRnd(0x28, 0x32) * 0.00002f;
        sp3C.unk8 = mathRnd(0, 0x3C) + 0x50;
        sp3C.unk60 = 0xFF;
        sp3C.unk42 = 0x8F;
        sp3C.unk48 = 0x200000;
        sp3C.unk44 = 0x42000100;
        break;
    case 0x73D:
        sp3C.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp3C.unk30.y = (f32) mathRnd(-0xA, 0x64) * 0.05f;
        sp3C.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp3C.unk3C = (f32) mathRnd(7, 9) * 0.0009f;
        sp3C.unk8 = 0x3C;
        sp3C.unk44 = 0x80110;
        sp3C.unk61 = 0x10;
        sp3C.unk42 = 0xDE;
        break;
    case 0x73E:
        sp3C.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp3C.unk30.y = (f32) mathRnd(-0xA, 0x64) * 0.05f;
        sp3C.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp3C.unk3C = (f32) mathRnd(7, 9) * 0.0009f;
        sp3C.unk8 = 0x3C;
        sp3C.unk44 = 0x80110;
        sp3C.unk61 = 0x10;
        sp3C.unk42 = 0xDF;
        break;
    case 0x73F:
        sp3C.unk30.x = ((f32) mathRnd(-0xA, 0xA) * 0.1f) + -2.0f;
        sp3C.unk30.y = ((f32) mathRnd(-0xA, 0x64) * 0.05f) + 103.5f;
        sp3C.unk30.z = ((f32) mathRnd(-0xA, 0xA) * 0.1f) + 5.5f;
        sp3C.unk3C = (f32) mathRnd(7, 9) * 0.0009f;
        sp3C.unk8 = 0x3C;
        sp3C.unk44 = 0x80110;
        sp3C.unk61 = 0x10;
        sp3C.unk42 = 0xDE;
        break;
    case 0x740:
        sp3C.unk30.x = ((f32) mathRnd(-0xA, 0xA) * 0.1f) + 2.0f;
        sp3C.unk30.y = ((f32) mathRnd(-0xA, 0x64) * 0.05f) + 103.5f;
        sp3C.unk30.z = ((f32) mathRnd(-0xA, 0xA) * 0.1f) + 5.5f;
        sp3C.unk3C = (f32) mathRnd(7, 9) * 0.0009f;
        sp3C.unk8 = 0x3C;
        sp3C.unk44 = 0x80110;
        sp3C.unk61 = 0x10;
        sp3C.unk42 = 0xDF;
        break;
    case 0x741:
        if (transform != NULL) {
            sp3C.unk30.y = transform->transl.y;
        }
        sp3C.unk3C = 0.03f;
        sp3C.unk8 = mathRnd(0, 0x1E) + 0x50;
        sp3C.unk60 = 0x60;
        sp3C.unk44 = 0x80110;
        sp3C.unk42 = 0x7B;
        sp3C.unk61 = 0x20;
        break;
    case 0x742:
        sp3C.unk24.z = -0.5f;
        sp3C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.009f;
        sp3C.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.009f;
        sp3C.unk3C = 0.0047f;
        sp3C.unk8 = mathRnd(0x46, 0x50);
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x82000104;
        sp3C.unk48 = 0x400;
        sp3C.unk42 = 0x3F4;
        break;
    case 0x743:
        sp3C.unk24.z = -0.5f;
        sp3C.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.009f;
        sp3C.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.009f;
        sp3C.unk3C = 0.0047f;
        sp3C.unk8 = mathRnd(0x46, 0x50);
        sp3C.unk60 = 0xFF;
        sp3C.unk44 = 0x82000104;
        sp3C.unk48 = 0x400;
        sp3C.unk42 = 0x500;
        break;
    case 0x744:
        if (mathRnd(0, 4) == 4) {
            sp3C.unk60 = 0x9B;
            sp3C.unk44 = 0x480000;
            sp3C.unk3C = 0.0147f;
            sp3C.unk8 = mathRnd(0x1E, 0x28);
        } else {
            sp3C.unk60 = 0x7D;
            sp3C.unk44 = 0x180000;
            sp3C.unk3C = 0.0347f;
            sp3C.unk8 = 0x50;
        }
        sp3C.unk48 = 0x02000000;
        sp3C.unk42 = 0x88;
        break;
    default:
        // STUBBED_PRINTF("PARTFX wcning type < %d > not found.. returning\n", id); // default.dol
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
