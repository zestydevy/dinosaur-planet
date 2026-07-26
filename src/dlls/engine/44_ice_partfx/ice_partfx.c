#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void icePartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void icePartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void icePartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void icePartfx_Func_20(s32 arg0) {

}

// offset: 0x2C | func: 2 | export: 1
s32 icePartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp2C;
    s32 _pad;

    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX icening NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
        return -1;
    }
    if (flags & 0x200000) {
        if (transform == NULL) {
            // STUBBED_PRINTF("ERROR partfx EXF_NOOWNER & no attrib %d\n", id); // default.dol
            return -1;
        }
        sp2C.transform.transl.x = transform->transl.x;
        sp2C.transform.transl.y = transform->transl.y;
        sp2C.transform.transl.z = transform->transl.z;
        sp2C.transform.scale = transform->scale;
        sp2C.transform.roll = transform->roll;
        sp2C.transform.pitch = transform->pitch;
        sp2C.transform.yaw = transform->yaw;
        sp2C.unk62 = arg4;
    }
    sp2C.unk44 = 0;
    sp2C.unk48 = 0;
    sp2C.unk5E = (u8) id;
    sp2C.unk0 = obj;
    sp2C.unk8 = 0;
    sp2C.unk4 = -1;
    sp2C.unk60 = 0xFF;
    sp2C.unk61 = 0;
    sp2C.unk42 = 0;
    sp2C.unk30.x = 0.0f;
    sp2C.unk30.y = 0.0f;
    sp2C.unk30.z = 0.0f;
    sp2C.unk24.x = 0.0f;
    sp2C.unk24.y = 0.0f;
    sp2C.unk24.z = 0.0f;
    sp2C.unk3C = 0.0f;
    sp2C.unk58[0] = 0xFFFF;
    sp2C.unk58[1] = 0xFFFF;
    sp2C.unk58[2] = 0xFFFF;
    sp2C.unk4C[0] = 0xFFFF;
    sp2C.unk4C[1] = 0xFFFF;
    sp2C.unk4C[2] = 0xFFFF;
    sp2C.unk40 = 0;
    switch (id) {
    case 0x12C:
        sp2C.unk8 = 0xA;
        sp2C.unk60 = 0xFF;
        sp2C.unk44 = 0x40200;
        sp2C.unk42 = 0xDB;
        sp2C.unk3C = 0.0016f;
        break;
    case 0x12D:
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
        sp2C.unk3C = 0.025f;
        sp2C.unk8 = mathRnd(0, 0x1E) + 0x46;
        sp2C.unk60 = transform->scale > 0.0f ? 0x50 : 0x41;
        sp2C.unk44 = 0x80110;
        sp2C.unk42 = transform->scale > 0.0f ? 0x7B : 0xDB;
        break;
    case 0x12E:
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
        sp2C.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.6f;
        sp2C.unk30.y = 0.0f;
        sp2C.unk30.z = 15.0f;
        sp2C.unk24.y = (f32) mathRnd(1, 3) * 0.1f;
        sp2C.unk24.x = transform->transl.x * 0.6f;
        sp2C.unk24.z = -transform->transl.z * 0.6f;
        sp2C.unk3C = (f32) mathRnd(1, 3) * 0.0016f;
        sp2C.unk8 = 0x19;
        sp2C.unk60 = 0x55;
        sp2C.unk44 = 0x80118;
        sp2C.unk42 = 0x5F;
        break;
    case 0x12F:
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
        sp2C.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.6f;
        sp2C.unk30.y = 0.0f;
        sp2C.unk30.z = 15.0f;
        sp2C.unk24.y = (f32) mathRnd(1, 3) * 0.1f;
        sp2C.unk24.x = transform->transl.x * 0.4f;
        sp2C.unk24.z = -transform->transl.z * 0.4f;
        sp2C.unk3C = (f32) mathRnd(1, 3) * 0.0018f;
        sp2C.unk8 = 0x19;
        sp2C.unk60 = 0x55;
        sp2C.unk44 = 0x80118;
        sp2C.unk42 = 0x5F;
        break;
    case 0x130:
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
        sp2C.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.6f;
        sp2C.unk30.y = 0.0f;
        sp2C.unk30.z = 15.0f;
        sp2C.unk24.y = (f32) mathRnd(1, 3) * 0.1f;
        sp2C.unk24.x = transform->transl.x * 0.2f;
        sp2C.unk24.z = -transform->transl.z * 0.2f;
        sp2C.unk3C = (f32) mathRnd(1, 3) * 0.0022f;
        sp2C.unk8 = 0x19;
        sp2C.unk60 = 0x55;
        sp2C.unk44 = 0x80118;
        sp2C.unk42 = 0x5F;
        break;
    case 0x131:
        sp2C.unk30.x = (f32) mathRnd(-0xC, 0xC) * 0.1f;
        sp2C.unk30.y = ((f32) mathRnd(-0xC, 0xC) * 0.1f) + 6.0f;
        sp2C.unk30.z = 15.0f;
        sp2C.unk24.z = (f32) mathRnd(5, 0xA) * 0.0045f;
        sp2C.unk8 = 0x64;
        sp2C.unk60 = 0xFF;
        sp2C.unk44 = 0x100;
        sp2C.unk42 = 0x33;
        sp2C.unk3C = 0.00165f;
        break;
    case 0x132:
        sp2C.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.18f;
        sp2C.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.18f;
        sp2C.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.18f;
        sp2C.unk3C = 0.02f;
        sp2C.unk8 = mathRnd(0x78, 0x96);
        sp2C.unk61 = 0x1E;
        sp2C.unk60 = 0xFF;
        sp2C.unk44 = 0x11;
        sp2C.unk42 = 0x5F;
        break;
    case 0x133:
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
        sp2C.unk30.x = transform->transl.x;
        sp2C.unk30.y = transform->transl.y;
        sp2C.unk30.z = transform->transl.z;
        sp2C.unk3C = 0.02f;
        sp2C.unk8 = 5;
        sp2C.unk60 = 0x80;
        sp2C.unk44 |= 0x80210;
        sp2C.unk42 = 0x26D;
        break;
    case 0x134:
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
        sp2C.unk30.x = ((f32) mathRnd(-0xC8, 0xC8) * 0.001f) + transform->transl.x;
        sp2C.unk30.y = transform->transl.y;
        sp2C.unk30.z = ((f32) mathRnd(-0xC8, 0xC8) * 0.001f) + transform->transl.z;
        sp2C.unk3C = (f32) mathRnd(5, 0xC) * 0.0001f;
        sp2C.unk8 = 0xC;
        sp2C.unk60 = mathRnd(0x96, 0xFA);
        sp2C.unk44 |= 0x80210;
        sp2C.unk42 = 0xE0;
        break;
    case 0x135:
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
        sp2C.unk30.x = (f32) mathRnd(-0xA, 0xA) * 0.18f;
        sp2C.unk30.y = (f32) mathRnd(-0x1E, 0) * 0.18f;
        sp2C.unk30.z = (f32) mathRnd(-0xA, 0xA) * 0.18f;
        sp2C.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.02f;
        sp2C.unk24.y = (f32) mathRnd(0xF, 0x23) * 0.0015f;
        sp2C.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.02f;
        sp2C.unk3C = (f32) mathRnd(0x64, 0x96) * 0.00012f;
        sp2C.unk8 = mathRnd(0x32, 0x50);
        sp2C.unk61 = mathRnd(0xA, 0x1E);
        sp2C.unk44 = 0x218;
        sp2C.unk42 = transform->roll;
        break;
    case 0x136:
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
        sp2C.unk30.x = (f32) mathRnd((s32) -transform->pitch, (s32) transform->pitch) / 10.0f;
        sp2C.unk30.y = (f32) mathRnd((s32) -transform->pitch, (s32) transform->pitch) / 10.0f;
        sp2C.unk30.z = (f32) mathRnd((s32) -transform->pitch, (s32) transform->pitch) / 10.0f;
        sp2C.unk3C = 0.005f;
        sp2C.unk8 = mathRnd(0x14, 0x1E);
        sp2C.unk44 = 0x100200;
        sp2C.unk42 = transform->roll;
        break;
    case 0x137:
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
        if (transform == NULL) {
            return -1;
        }
        sp2C.unk24.x = ((f32) mathRnd(0, 0x64) * 0.003f) + 0.25f;
        sp2C.unk24.y = ((f32) mathRnd(0, 0x64) * 0.002f) + 0.02f;
        sp2C.unk24.z = ((f32) mathRnd(0, 0x64) * 0.002f) + 0.02f;
        mathRotateRPY(transform, sp2C.unk24.f);
        sp2C.unk3C = (f32) mathRnd(0x14, 0x1E) * 0.00004f;
        sp2C.unk60 = 0xFF;
        sp2C.unk8 = 0xF0;
        sp2C.unk61 = 0x10;
        sp2C.unk4 = 0x138;
        sp2C.unk44 = 0x480200;
        sp2C.unk48 = 0x100000;
        sp2C.unk42 = 0x167;
        break;
    case 0x138:
        sp2C.unk3C = (f32) mathRnd(0x14, 0x1E) * 0.0001f;
        sp2C.unk60 = 0x37;
        sp2C.unk8 = 4;
        sp2C.unk61 = 0x10;
        sp2C.unk44 = 0x80201;
        sp2C.unk48 = 2;
        sp2C.unk42 = 0x167;
        break;
    default:
        // STUBBED_PRINTF("PARTFX ice type < %d > not found.. returning\n", id); // default.dol
        return -1;
    }
    sp2C.unk44 |= flags;
    if ((sp2C.unk44 & 1) && (sp2C.unk44 & 2)) {
        // STUBBED_PRINTF("error in partfx ABS and REL %d\n", id); // default.dol
        sp2C.unk44 ^= 2;
    }
    if (sp2C.unk44 & 1) {
        if (flags & 0x200000) {
            sp2C.unk30.x += sp2C.transform.transl.x;
            sp2C.unk30.y += sp2C.transform.transl.y;
            sp2C.unk30.z += sp2C.transform.transl.z;
        } else if (sp2C.unk0 != NULL) {
            sp2C.unk30.x += sp2C.unk0->globalPosition.x;
            sp2C.unk30.y += sp2C.unk0->globalPosition.y;
            sp2C.unk30.z += sp2C.unk0->globalPosition.z;
        }
    }
    return gDLL_13_Expgfx->vtbl->func1(&sp2C, -1);
}
