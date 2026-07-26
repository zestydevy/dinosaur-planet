#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void swhPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void swhPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void swhPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void swhPartfx_Func_20(s32 arg0) {

}

// offset: 0x2C | func: 2 | export: 1
s32 swhPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp34;
    s32 _pad;
    s32 _pad2;

    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX rexning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
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
    sp34.unk5E = id;
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
    switch (id) {
    case 0x3E8:
        sp34.unk3C = (f32) mathRnd(0x5A, 0x64) * 0.000015f;
        sp34.unk24.x = (f32) mathRnd(-0x32, 0x32) * 0.005f;
        sp34.unk24.y = 0.0f;
        sp34.unk24.z = (f32) mathRnd(-0x32, 0x32) * 0.005f;
        sp34.unk8 = 0x28;
        sp34.unk44 |= 0x80218;
        sp34.unk48 = 0x20;
        sp34.unk42 = mathRnd(0, 2) + 0x156;
        sp34.unk58[0] = 0xFFFF;
        sp34.unk58[1] = 0xD6D8;
        sp34.unk58[2] = 0xFFFF;
        sp34.unk4C[0] = 0xFFFF;
        sp34.unk4C[1] = 0x7530;
        sp34.unk4C[2] = 0xFFFF;
        sp34.unk60 = 0xFF;
        break;
    case 0x3E9:
        if (transform == NULL) {
            transform = &bss_0;
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
            bss_0.scale = 1.0f;
        }
        sp34.unk30.x = transform->transl.x;
        sp34.unk30.y = transform->transl.y;
        sp34.unk30.z = transform->transl.z;
        sp34.unk3C = 0.05f;
        sp34.unk44 |= 0x180110;
        sp34.unk48 = 0x20;
        sp34.unk8 = 0x12;
        sp34.unk60 = 0xFF;
        sp34.unk42 = 0x159;
        sp34.unk58[0] = 0xFFFF;
        sp34.unk58[1] = 0xFFFF;
        sp34.unk58[2] = 0xFFFF;
        sp34.unk4C[0] = 0xFFFF;
        sp34.unk4C[1] = 0xC350;
        sp34.unk4C[2] = 0xFFFF;
        break;
    case 0x3EA:
        if (transform == NULL) {
            transform = &bss_0;
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
            bss_0.scale = 1.0f;
        }
        sp34.unk30.x = (f32) mathRnd(-0x64, 0x64) / 50.0f;
        sp34.unk30.y = (f32) -mathRnd(0x64, 0x96) / 100.0f;
        sp34.unk30.z = (f32) mathRnd(-0x64, 0x64) / 50.0f;
        sp34.unk44 |= 0x80208;
        sp34.unk48 = 0x10000;
        sp34.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.001f;
        sp34.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.001f;
        sp34.unk60 = 0xFF;
        sp34.unk8 = 0x3C;
        sp34.unk42 = 0x7B;
        sp34.unk3C = ((f32) mathRnd(0x32, 0x64) * 0.01f * 0.0003f * transform->scale) + 0.001f;
        break;
    case 0x3EB:
        sp34.unk24.x = (f32) mathRnd(-0x1E, 0x1E) * 0.02f;
        sp34.unk24.y = (f32) mathRnd(-5, 5) * 0.01f;
        sp34.unk24.z = (f32) mathRnd(-0x1E, 0x1E) * 0.02f;
        sp34.unk30.x = 0;
        sp34.unk30.y = (f32) mathRnd(-6, 2);
        sp34.unk30.z = 0;
        sp34.unk3C = 0.01f;
        sp34.unk8 = 0x32;
        sp34.unk44 = 0x80080208;
        sp34.unk42 = 0x60;
        sp34.unk58[0] = 0x7F00;
        sp34.unk58[1] = 0x6400;
        sp34.unk58[2] = 0;
        sp34.unk4C[0] = 0x5A00;
        sp34.unk4C[1] = 0;
        sp34.unk4C[2] = 0;
        sp34.unk48 = 0x20;
        sp34.unk60 = 0x7F;
        break;
    case 0x3EC:
        return -1;
    case 0x3ED:
        sp34.unk24.x = (f32) mathRnd(-0x32, 0x32) * 0.02f;
        sp34.unk24.y = (f32) mathRnd(-0x32, 0x32) * 0.05f;
        sp34.unk24.z = (f32) mathRnd(-0x32, 0x32) * 0.02f;
        sp34.unk3C = (f32) mathRnd(0xA, 0x14) * 0.00005f;
        sp34.unk8 = 0x32;
        sp34.unk44 = 0x80210;
        if ((transform == NULL) || (transform->scale == 0)) {
            sp34.unk42 = 0x65;
        } else if (transform->scale == 1.0f) {
            sp34.unk42 = 0x1FE;
        } else {
            sp34.unk42 = 0x7B;
        }
        break;
    case 0x3EE:
        sp34.unk30.x += (f32) mathRnd(-0xA, 0xA) / 3.0f;
        sp34.unk30.y += (f32) mathRnd(-0x1E, 0) / 10.0f;
        sp34.unk30.z += (f32) mathRnd(-0xA, 0xA) / 3.0f;
        sp34.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.001f;
        sp34.unk24.y = (f32) -mathRnd(0x28, 0x64) * 0.0022f;
        sp34.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.001f;
        sp34.unk3C = (f32) mathRnd(0xA, 0x14) * 0.001f;
        sp34.unk8 = 0x1F4;
        sp34.unk44 = 0x180100;
        sp34.unk42 = 0x61;
        sp34.unk60 = mathRnd(0x96, 0xFA);
        break;
    case 0x3EF:
        sp34.unk30.x = (f32) mathRnd(-0x4B0, 0x4B0) / 100.0f;
        sp34.unk30.z = (f32) mathRnd(-0x4B0, 0x4B0) / 100.0f;
        sp34.unk24.y = (f32) mathRnd(0x1E, 0x46) * 0.0022f;
        sp34.unk3C = ((f32) mathRnd(0, 0x14) * 0.00012f) + 0.002f;
        sp34.unk8 = 0xC8;
        sp34.unk44 = 0x80100;
        sp34.unk42 = 0x33;
        sp34.unk60 = 0xB4;
        sp34.unk48 = 0x100000;
        break;
    case 0x3F0:
        sp34.unk30.x = (f32) mathRnd(-0x3E8, 0x3E8) / 100.0f;
        sp34.unk30.z = (f32) mathRnd(-0x3E8, 0x3E8) / 100.0f;
        sp34.unk24.y = (f32) mathRnd(0x1E, 0x46) * 0.0021f;
        sp34.unk3C = ((f32) mathRnd(0, 0x14) * 0.00012f) + 0.0015f;
        sp34.unk8 = 0xFA;
        sp34.unk44 = 0x80100;
        sp34.unk42 = 0x33;
        sp34.unk60 = 0xB4;
        break;
    case 0x3F1:
        sp34.unk44 = 0x80800;
        sp34.unk42 = 0x76;
        sp34.unk60 = 0xD2;
        sp34.unk8 = 0x64;
        sp34.unk30.x = 0;
        sp34.unk30.y = 0;
        sp34.unk30.z = 0;
        sp34.unk3C = 0.0075f;
        break;
    case 0x3F2:
        if (data == NULL) {
            return 0;
        }
        if (transform == NULL) {
            transform = &bss_0;
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
            bss_0.scale = 1.0f;
        }
        if (transform != NULL) {
            sp34.unk30.x = transform->transl.x;
            sp34.unk30.y = transform->transl.y;
            sp34.unk30.z = transform->transl.z;
        }
        if (data != NULL) {
            sp34.unk24.x = ((f32*)data)[0];
            sp34.unk24.y = (f32) mathRnd(0, 0x14) * 0.03f;
            sp34.unk24.z = ((f32*)data)[1];
        }
        sp34.unk3C = 2.0f * (((f32) mathRnd(0, 0xA) * 0.0002f) + 0.000945f);
        sp34.unk8 = mathRnd(0xBE, 0xFA);
        sp34.unk60 = 0xFF;
        sp34.unk44 = 0x81088000;
        sp34.unk42 = 0x23C;
        break;
    case 0x3F3:
        sp34.unk30.x = (f32) mathRnd(-0x32, 0x32) / 100.0f;
        sp34.unk30.y = (f32) mathRnd(-0x32, 0x32) / 100.0f;
        sp34.unk30.z = (f32) mathRnd(-0x32, 0x32) / 100.0f;
        sp34.unk24.x = (f32) mathRnd(0x1E, 0x3C) * 0.005f;
        if (mathRnd(0, 1) != 0) {
            sp34.unk24.x = -sp34.unk24.x;
        }
        sp34.unk24.y = (f32) mathRnd(0x1E, 0x3C) * 0.005f;
        if (mathRnd(0, 1) != 0) {
            sp34.unk24.y = -sp34.unk24.y;
        }
        sp34.unk24.z = (f32) mathRnd(0x1E, 0x3C) * 0.005f;
        if (mathRnd(0, 1) != 0) {
            sp34.unk24.z = -sp34.unk24.z;
        }
        sp34.unk3C = ((f32) mathRnd(0, 0xA) * 0.00012f) + 0.001f;
        sp34.unk8 = 0x46;
        sp34.unk44 = 0x80208;
        sp34.unk42 = 0x76;
        sp34.unk60 = 0xB4;
        sp34.unk48 = 0x100000;
        break;
    case 0x3F4:
    case 0x3F5:
    case 0x3F6:
        if (transform != NULL) {
            sp34.unk30.x = transform->transl.x;
            sp34.unk30.y = transform->transl.y;
            sp34.unk30.z = transform->transl.z;
            sp34.unk30.x -= sp34.unk0->globalPosition.x;
            sp34.unk30.y -= sp34.unk0->globalPosition.y;
            sp34.unk30.z -= sp34.unk0->globalPosition.z;
        }
        if (mathRnd(0, 0x28) == 0) {
            sp34.unk3C = 0.0003f;
        } else {
            sp34.unk3C = 0.0015f;
        }
        sp34.unk8 = 0x14;
        sp34.unk60 = 0xFF;
        sp34.unk44 = 0x80210;
        sp34.unk42 = id - 0x29E;
        break;
    case 0x3F7:
    case 0x3F8:
    case 0x3F9:
        if (transform != NULL) {
            sp34.unk30.x = transform->transl.x;
            sp34.unk30.y = transform->transl.y;
            sp34.unk30.z = transform->transl.z;
            sp34.unk30.x -= sp34.unk0->globalPosition.x;
            sp34.unk30.y -= sp34.unk0->globalPosition.y;
            sp34.unk30.z -= sp34.unk0->globalPosition.z;
            sp34.unk24.z = 0.3f;
        }
        sp34.unk8 = 0x64;
        sp34.unk60 = 0xFF;
        sp34.unk44 = 0x480210;
        sp34.unk48 = 0x100000;
        sp34.unk42 = id + 0x104;
        sp34.unk3C = 0.0015f;
        break;
    default:
        // STUBBED_PRINTF("PARTFX swh type < %d > not found.. returning\n", id); // default.dol
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
