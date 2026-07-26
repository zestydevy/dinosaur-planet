#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void rexPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void rexPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void rexPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void rexPartfx_Func_20(s32 arg0) {

}

// offset: 0x2C | func: 2 | export: 1
s32 rexPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp44;
    SRT sp2C;
    s32 temp_v1_2;

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
    case 0x47E:
        sp44.unk3C = 0.075f;
        sp44.unk8 = mathRnd(0x32, 0x3C);
        sp44.unk60 = 0x4B;
        sp44.unk44 = 0x180110;
        sp44.unk42 = 0x159;
        break;
    case 0x483:
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
        sp44.unk30.x = (f32) mathRnd(-0xA, 0xA);
        sp44.unk30.z = (f32) mathRnd(-0xA, 0xA);
        sp44.unk24.x = (f32) mathRnd(-0x64, 0x64) * (0.02f * transform->scale);
        sp44.unk24.y = (f32) mathRnd(0x28, 0x50) * (0.02f * transform->scale);
        sp44.unk24.z = (f32) mathRnd(-0x64, 0x64) * (0.02f * transform->scale);
        sp44.unk8 = 0x3C;
        sp44.unk44 = 0x81080200;
        sp44.unk42 = 0x2B;
        sp44.unk60 = 0x3C;
        sp44.unk3C = 0.03f;
        break;
    case 0x484:
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
        sp44.unk24.x = (f32) mathRnd(-0x64, 0x64) * (0.03f * transform->scale);
        sp44.unk24.y = (f32) mathRnd(0x14, 0x50) * (0.03f * transform->scale);
        sp44.unk24.z = (f32) mathRnd(-0x64, 0x64) * (0.03f * transform->scale);
        sp44.unk3C = 0.01f;
        sp44.unk8 = 0x3C;
        sp44.unk48 = 0x200000;
        sp44.unk44 = 0x03000200;
        sp44.unk42 = 0x185;
        sp44.unk60 = 0x7F;
        break;
    case 0x485:
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
        sp44.unk30.x = (f32) mathRnd(-0xA, 0xA);
        sp44.unk30.z = (f32) mathRnd(-0xA, 0xA);
        sp44.unk24.x = (f32) mathRnd(-0x64, 0x64) * (0.02f * transform->scale);
        sp44.unk24.y = (f32) mathRnd(0x28, 0x50) * (0.02f * transform->scale);
        sp44.unk24.z = (f32) mathRnd(-0x64, 0x64) * (0.02f * transform->scale);
        sp44.unk3C = 0.03f;
        sp44.unk8 = 0x3C;
        sp44.unk44 = 0x81080200;
        sp44.unk42 = 0x2B;
        sp44.unk60 = 0x3C;
        break;
    case 0x486:
        sp44.unk24.y = (f32) mathRnd(0xA, 0x23) * 0.003f;
        if (mathRnd(0, 1) != 0) {
            sp44.unk24.y = -sp44.unk24.y;
        }
        sp44.unk42 = 0x64;
        sp44.unk30.x = ((f32) mathRnd(-0x28, 0x28) * 0.1f) + 27.5f;
        sp44.unk30.y = 11.0f;
        sp44.unk30.z = ((f32) mathRnd(-0x28, 0x28) * 0.1f) + 27.5f;
        sp44.unk3C = (f32) mathRnd(0xA, 0xF) * 0.00022f;
        sp44.unk8 = 0x3C;
        sp44.unk44 = 0x80102;
        sp44.unk48 = 0x10000;
        break;
    case 0x487:
        if (data == NULL) {
            return 0;
        }
        sp44.unk24.x = ((Vec3f*)data)->x;
        sp44.unk24.y = ((Vec3f*)data)->y;
        sp44.unk24.z = ((Vec3f*)data)->z;
        sp44.unk3C = 0.025f;
        sp44.unk60 = 0x40;
        sp44.unk8 = 0x64;
        sp44.unk44 = 0x03000200;
        sp44.unk42 = 0x62;
        sp44.unk48 = 0x200000;
        break;
    case 0x488:
        sp44.unk30.x = (f32) mathRnd(-0x18, 0x18) + 27.5f;
        sp44.unk30.y = 0.0f;
        sp44.unk30.z = (f32) mathRnd(-0x18, 0x18) + 27.5f;
        sp44.unk24.x = (f32) mathRnd(-5, 5) * 0.01f;
        sp44.unk24.y = (f32) mathRnd(2, 0xA) * 0.01f;
        sp44.unk24.z = (f32) mathRnd(-5, 5) * 0.01f;
        sp44.unk8 = 0x6E;
        sp44.unk44 = 0x80180200;
        sp44.unk42 = 0x2B;
        sp44.unk60 = 0xFF;
        sp44.unk3C = 0.02f;
        break;
    case 0x489:
        sp44.unk3C = 0.04f;
        sp44.unk8 = mathRnd(0x32, 0x64);
        sp44.unk60 = 0x7F;
        sp44.unk44 = 0x01180100;
        sp44.unk42 = 0x2B;
        break;
    case 0x48A:
        sp44.unk24.x = (f32) mathRnd(-0x32, 0x32) * 0.02f;
        sp44.unk24.y = (f32) mathRnd(0x1E, 0x32) * 0.02f;
        sp44.unk24.z = (f32) mathRnd(-0x32, 0x32) * 0.02f;
        sp44.unk3C = 0.06f;
        sp44.unk8 = mathRnd(0x32, 0x46);
        sp44.unk60 = 0x7F;
        sp44.unk44 = 0x01180100;
        sp44.unk42 = 0x2B;
        break;
    case 0x48B:
        sp44.unk30.x = (f32) mathRnd(-0x32, 0x32);
        sp44.unk30.y = 100.0f;
        sp44.unk30.z = (f32) mathRnd(-0x32, 0x32);
        sp44.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk24.y = (f32) mathRnd(-0x14, 0) * 0.03f;
        sp44.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp44.unk3C = ((f32) mathRnd(0, 0xA) * 0.00015f) + 0.002945f;
        sp44.unk8 = mathRnd(0xBE, 0xFA);
        sp44.unk44 = 0x81088000;
        sp44.unk42 = mathRnd(0, 2) + 0x208;
        sp44.unk58[0] = 0xB400;
        sp44.unk58[1] = 0x8000;
        sp44.unk58[2] = 0;
        sp44.unk4C[0] = 0xB400;
        sp44.unk4C[1] = 0xA000;
        sp44.unk4C[2] = 0;
        sp44.unk48 = 0x20;
        sp44.unk60 = 0xD2;
        break;
    case 0x48C:
        if (transform == NULL) {
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
            bss_0.scale = 1.0f;
        }
        if (data == NULL) {
            return -1;
        }
        temp_v1_2 = *(s32*)data;
        if (temp_v1_2 == 0) {
            sp44.unk3C = (f32) mathRnd(8, 0x11) * 0.002f;
            sp44.unk8 = mathRnd(5, 0xA);
            sp44.unk60 = 0x64;
            sp44.unk44 = 0x80110;
        } else if (temp_v1_2 == 1) {
            sp44.unk24.x = (f32) mathRnd(-0x32, 0x32) * 0.02f;
            sp44.unk24.y = (f32) mathRnd(-0x32, 0x32) * 0.02f;
            sp44.unk24.z = (f32) mathRnd(0, 0x32) * 0.02f;
            sp44.unk3C = (f32) mathRnd(0xA, 0x14) * 0.00035f;
            sp44.unk8 = 0x2D;
            sp44.unk60 = 0;
            sp44.unk44 = 0x880014;
            sp44.unk48 = 0x10008;
        } else {
            sp44.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.02f;
            sp44.unk24.y = (f32) mathRnd(-0xA, 0x1E) * 0.04f;
            sp44.unk24.z = (f32) mathRnd(0, 0x28) * 0.04f;
            sp2C.roll = 0;
            sp2C.pitch = 0;
            sp2C.transl.x = 0.0f;
            sp2C.transl.y = 0.0f;
            sp2C.transl.z = 0.0f;
            sp2C.scale = 1.0f;
            sp2C.yaw = obj->srt.yaw;
            mathRotateRPY(&sp2C, sp44.unk24.f);
            sp44.unk8 = 0x64;
            sp44.unk60 = 0xFF;
            sp44.unk48 = 0x300000;
            sp44.unk44 = 0x03000210;
            sp44.unk3C = 0.02f;
        }
        sp44.unk42 = mathRnd(0x156, 0x157);
        break;
    default:
        // STUBBED_PRINTF("PARTFX rex type < %d > not found.. returning\n", id); // default.dol
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
