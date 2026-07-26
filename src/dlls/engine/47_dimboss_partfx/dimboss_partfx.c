#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void dimbossPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void dimbossPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dimbossPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void dimbossPartfx_Func_20(s32 arg0) {

}

// offset: 0x2C | func: 2 | export: 1
s32 dimbossPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp44;
    SRT sp2C;
    s32 _pad;

    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX rexning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
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
    sp44.unk5E = (u8) id;
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
    switch (id) {
    case 0x4B0:
        if (data == NULL) {
            return 0;
        }
        sp44.unk60 = (u8) ((*(s32*)data) >> 1);
        sp44.unk3C = (f32) sp44.unk60 * 0.0003f;
        sp44.unk8 = 1;
        sp44.unk44 = 0x80010;
        sp44.unk42 = 0x17D;
        break;
    case 0x4B1:
        sp44.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.02f;
        sp44.unk24.y = (f32) mathRnd(-0x19, 0x96) * 0.01f;
        sp44.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.02f;
        sp44.unk8 = 0x64;
        sp44.unk44 = 0x01180200;
        sp44.unk42 = 0x167;
        sp44.unk58[0] = 0xFF00;
        sp44.unk58[1] = 0xFF00;
        sp44.unk58[2] = 0xFF00;
        sp44.unk4C[0] = 0xFF00;
        sp44.unk4C[1] = 0;
        sp44.unk4C[2] = 0;
        sp44.unk48 = 0x20;
        sp44.unk3C = 0.003f;
        break;
    case 0x4B2:
        sp44.unk8 = 0x46;
        sp44.unk44 = 0x100100;
        sp44.unk42 = 0x73;
        sp44.unk58[0] = 0xFF00;
        sp44.unk58[1] = 0xFF00;
        sp44.unk58[2] = 0xFF00;
        sp44.unk4C[0] = 0xFF00;
        sp44.unk4C[1] = 0;
        sp44.unk4C[2] = 0xFF00;
        sp44.unk48 = 0x20;
        sp44.unk60 = 0x7F;
        sp44.unk3C = 0.2f;
        break;
    case 0x4B3:
        sp44.unk8 = 0x23;
        sp44.unk44 = 0x100200;
        sp44.unk42 = 0x73;
        sp44.unk3C = 0.16f;
        break;
    case 0x4B4:
        sp44.unk30.x = (f32) mathRnd(-1, 1);
        sp44.unk30.y = (f32) mathRnd(-7, 7);
        sp44.unk30.z = (f32) mathRnd(-1, 1);
        sp44.unk24.x = (f32) mathRnd(-7, 7) * 0.01f;
        sp44.unk24.y = (f32) mathRnd(0, 0x1E) * 0.01f;
        sp44.unk24.z = (f32) mathRnd(-7, 7) * 0.01f;
        sp44.unk3C = 0.008f;
        sp44.unk8 = mathRnd(0x3C, 0x64);
        sp44.unk44 = 0x01180200;
        sp44.unk42 = 0x30;
        sp44.unk58[0] = 0;
        sp44.unk58[1] = mathRnd(0, 0xFFFF);
        sp44.unk58[2] = mathRnd(0, 0xFFFF);
        sp44.unk4C[0] = 0;
        sp44.unk4C[1] = 0xFF00;
        sp44.unk4C[2] = mathRnd(0, 0xFFFF);
        sp44.unk48 = 0x20;
        break;
    case 0x4B5:
        if (data != NULL) {
            sp44.unk24.x = ((Vec3f*)data)->x;
            sp44.unk24.y = ((Vec3f*)data)->y;
            sp44.unk24.z = ((Vec3f*)data)->z;
        }
        sp44.unk3C = 0.008f;
        sp44.unk8 = 0x5F;
        sp44.unk44 = 0x01180200;
        sp44.unk42 = 0x62;
        sp44.unk58[0] = 0;
        sp44.unk58[1] = mathRnd(0x8000, 0xFFFF);
        sp44.unk58[2] = 0;
        sp44.unk4C[0] = mathRnd(0, 0x8000);
        sp44.unk4C[1] = mathRnd(0, 0xFFFF);
        sp44.unk4C[2] = 0;
        sp44.unk48 = 0x20;
        break;
    case 0x4B6:
        if (data != NULL) {
            sp44.unk24.x = ((Vec3f*)data)->x;
            sp44.unk24.z = ((Vec3f*)data)->z;
        }
        sp44.unk60 = 0x7F;
        sp44.unk8 = 0x32;
        sp44.unk44 = 0x180110;
        sp44.unk42 = 0x62;
        sp44.unk24.y = 0.03f;
        sp44.unk3C = 0.025f;
        break;
    case 0x4B7:
        sp44.unk30.x = (f32) mathRnd(-0x14, 0x14);
        sp44.unk30.y = 5.0f;
        sp44.unk30.z = (f32) mathRnd(-0x14, 0x14);
        sp44.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.01f;
        sp44.unk24.y = (f32) mathRnd(0, 0x32) * 0.01f;
        sp44.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.01f;
        sp44.unk8 = 0x28;
        sp44.unk44 = 0x80200;
        sp44.unk42 = 0x5F;
        sp44.unk60 = 0x3F;
        sp44.unk3C = 0.01f;
        break;
    case 0x4B8:
        if (data != NULL) {
            sp44.unk24.x = ((Vec3f*)data)->x;
            sp44.unk24.y = ((Vec3f*)data)->y;
            sp44.unk24.z = ((Vec3f*)data)->z;
        }
        sp44.unk8 = 0x25;
        sp44.unk44 = 0x80200;
        sp44.unk3C = 0.0035f;
        sp44.unk42 = mathRnd(0x156, 0x158);
        break;
    case 0x4B9:
        sp44.unk3C = 0.015f;
        sp44.unk8 = mathRnd(0xA, 0xD);
        sp44.unk60 = 0x3F;
        sp44.unk44 = 0x80110;
        sp44.unk42 = 0xDB;
        sp44.unk61 = 0x1E;
        break;
    case 0x4BA:
        sp44.unk30.x = (f32) mathRnd(-7, 7);
        sp44.unk30.y = (f32) mathRnd(-7, 7);
        sp44.unk30.z = (f32) mathRnd(-7, 7);
        sp44.unk24.x = (f32) mathRnd(-0x32, 0x32) * 0.025f;
        sp44.unk24.y = (f32) mathRnd(-0x32, 0x32) * 0.025f;
        sp44.unk24.z = (f32) mathRnd(-0x32, 0x32) * 0.025f;
        sp44.unk8 = 0x28;
        sp44.unk60 = 0x96;
        sp44.unk44 = 0x01080200;
        sp44.unk42 = 0x62;
        sp44.unk58[0] = 0;
        sp44.unk58[1] = 0xB400;
        sp44.unk58[2] = 0;
        sp44.unk4C[0] = 0;
        sp44.unk4C[1] = 0;
        sp44.unk4C[2] = 0;
        sp44.unk48 = 0x20;
        sp44.unk3C = 0.01f;
        break;
    case 0x4BB:
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
        sp44.unk3C = 0.08f;
        break;
    case 0x4BC:
        if (data == NULL) {
            return 0;
        }
        sp44.unk30.y = 3.0f;
        sp44.unk60 = (u8) *(s32*)data;
        sp44.unk3C = ((f32) sp44.unk60 * 0.0002f) + 0.025f;
        sp44.unk8 = 1;
        sp44.unk44 = 0x0A040000;
        sp44.unk42 = 0xDB;
        break;
    case 0x4BD:
        sp44.unk30.x = (f32) mathRnd(-5, 5);
        sp44.unk30.y = (f32) mathRnd(0, 0xF);
        sp44.unk30.z = (f32) mathRnd(-5, 5);
        sp44.unk8 = 0x46;
        sp44.unk44 = 0x180200;
        sp44.unk42 = 0x5F;
        sp44.unk58[0] = 0xFF00;
        sp44.unk58[1] = 0xFF00;
        sp44.unk58[2] = 0;
        sp44.unk4C[0] = 0xFF00;
        sp44.unk4C[1] = 0;
        sp44.unk4C[2] = 0x8000;
        sp44.unk48 = 0x20;
        sp44.unk24.y = 0.25f;
        sp44.unk3C = 0.008f;
        break;
    case 0x4BE:
        sp44.unk30.x = (f32) mathRnd(-0x1C2, 0x1C2);
        sp44.unk30.y = 300.0f;
        sp44.unk30.z = (f32) mathRnd(-0x1C2, 0x1C2);
        sp44.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk24.y = (f32) mathRnd(0, 0x14) * 0.03f;
        sp44.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk3C = ((f32) mathRnd(0, 0xA) * 0.0005f) + 0.003945f;
        sp44.unk8 = mathRnd(0xBE, 0xFA);
        sp44.unk44 = 0x81488000;
        sp44.unk42 = mathRnd(0, 2) + 0x208;
        sp44.unk58[0] = 0x2000;
        sp44.unk58[1] = 0x8000;
        sp44.unk58[2] = 0xC000;
        sp44.unk4C[0] = 0xC000;
        sp44.unk4C[1] = 0xFF00;
        sp44.unk4C[2] = 0xFF00;
        sp44.unk48 = 0x20;
        break;
    case 0x4BF:
        sp44.unk30.x = (f32) mathRnd(-0x6E, 0x6E);
        sp44.unk30.y = 100.0f;
        sp44.unk30.z = (f32) mathRnd(-0x3C, 0x3C);
        sp44.unk8 = 0x64;
        sp44.unk44 = 0x11000004;
        sp44.unk42 = 0x151;
        sp44.unk58[0] = 0xFF00;
        sp44.unk58[1] = 0x4000;
        sp44.unk58[2] = 0;
        sp44.unk4C[0] = 0x4000;
        sp44.unk4C[1] = 0xC800;
        sp44.unk4C[2] = 0;
        sp44.unk4 = 0x4C0;
        sp44.unk48 = 0x20;
        sp44.unk3C = 0.0022f;
        break;
    case 0x4C0:
        sp44.unk8 = 0x4B;
        sp44.unk44 = 0x0A100200;
        sp44.unk42 = 0x56;
        sp44.unk30.y = -2.0f;
        sp44.unk3C = 75.0f;
        sp44.unk3C *= 0.00014f;
        break;
    case 0x4C1:
        sp44.unk24.x = (f32) mathRnd(-5, 5) * 0.01f;
        sp44.unk24.y = (f32) mathRnd(-5, 5) * 0.01f;
        sp44.unk24.z = (f32) mathRnd(-5, 5) * 0.01f;
        sp44.unk30.x = (f32) mathRnd(-0x78, 0x78);
        sp44.unk30.y = (f32) (mathRnd(-1, 1) * 0xC);
        sp44.unk30.z = (f32) mathRnd(-0x46, 0x46);
        sp44.unk8 = 0xC8;
        sp44.unk44 = 0x0A100100;
        sp44.unk42 = 0x62;
        sp44.unk58[0] = 0xFF00;
        sp44.unk58[1] = 0xFF00;
        sp44.unk58[2] = 0;
        sp44.unk4C[0] = 0xFF00;
        sp44.unk4C[1] = 0;
        sp44.unk4C[2] = 0x8000;
        sp44.unk48 = 0x20;
        sp44.unk3C = 0.02f;
        break;
    case 0x4C2:
        sp44.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk8 = 0x46;
        sp44.unk44 = 0x0A100200;
        sp44.unk42 = 0x5F;
        sp44.unk3C = 0.004f;
        break;
    case 0x4C3:
        sp44.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk30.x = (f32) mathRnd(-0x190, 0x190);
        sp44.unk30.z = (f32) mathRnd(-0x190, 0x190);
        sp44.unk8 = 0x258;
        sp44.unk60 = 0x7F;
        sp44.unk44 = 0x0A100100;
        sp44.unk42 = 0x62;
        sp44.unk3C = 0.1f;
        break;
    case 0x4C4:
        sp44.unk3C = 0.1f;
        sp44.unk8 = mathRnd(0x64, 0x12C);
        sp44.unk60 = 0xB4;
        sp44.unk44 = 0x80180208;
        sp44.unk42 = 0x62;
        break;
    case 0x4C5:
        if (transform == NULL) {
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
            bss_0.scale = 1.0f;
        }
        sp44.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk24.z = (f32) mathRnd(0xA, 0x1E) * -0.5f;
        sp2C.transl.x = 0.0f;
        sp2C.transl.y = 0.0f;
        sp2C.transl.z = 0.0f;
        sp2C.scale = 1.0f;
        sp2C.roll = obj->srt.roll;
        sp2C.pitch = obj->srt.pitch;
        sp2C.yaw = obj->srt.yaw;
        mathRotateRPY(&sp2C, sp44.unk24.f);
        sp44.unk44 = 0x03000000;
        sp44.unk48 = 0x200000;
        sp44.unk60 = 0xFF;
        sp44.unk8 = 0x32;
        sp44.unk42 = 0x151;
        sp44.unk3C = 0.01f;
        break;
    case 0x4C6:
        sp44.unk60 = 0x40;
        sp44.unk8 = 1;
        sp44.unk44 = 0x06000000;
        sp44.unk42 = 0x45B;
        sp44.transform.transl.x = 0.0f;
        sp44.transform.transl.y = 0.0f;
        sp44.transform.transl.z = 0.0f;
        sp44.unk3C = 0.05f;
        sp44.transform.scale = 1.0f;
        sp44.transform.roll = obj->srt.roll;
        sp44.transform.pitch = obj->srt.pitch;
        sp44.transform.yaw = obj->srt.yaw;
        break;
    case 0x4C7:
        sp44.unk60 = 0x40;
        sp44.unk8 = 1;
        sp44.unk44 = 0x06000000;
        sp44.unk42 = 0x45B;
        sp44.transform.transl.x = 0.0f;
        sp44.transform.transl.y = 0.0f;
        sp44.transform.transl.z = 0.0f;
        sp44.unk3C = 0.07f;
        sp44.transform.scale = 1.0f;
        sp44.transform.roll = obj->srt.roll;
        sp44.transform.pitch = obj->srt.pitch;
        sp44.transform.yaw = obj->srt.yaw;
        break;
    case 0x4C8:
        sp44.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.9f;
        sp44.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.9f;
        sp44.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.9f;
        sp44.unk3C = 0.005f;
        sp44.unk8 = mathRnd(0x4B, 0x64);
        sp44.unk60 = 0x7F;
        sp44.unk44 = 0x01080200;
        sp44.unk42 = 0x151;
        break;
    default:
        // STUBBED_PRINTF("PARTFX dimboss type < %d > not found.. returning\n", id); // default.dol
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
