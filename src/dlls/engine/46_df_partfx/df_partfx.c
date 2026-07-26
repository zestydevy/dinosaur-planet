#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/13_expgfx.h"
#include "dlls/engine/24_waterfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/** @file official filename: df_partfx.c */

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void dfPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void dfPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dfPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void dfPartfx_Func_20(s32 arg0) {

}

// offset: 0x2C | func: 2 | export: 1
s32 dfPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp44;
    s32 _pad;

    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX dfning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
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
    // @bug? sp44.unk40 is not initialized
    switch (id) {
    case 0x44C:
        sp44.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.025f;
        sp44.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.02f;
        sp44.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.025f;
        sp44.unk8 = 0x6E;
        sp44.unk44 = 0x8A100208;
        sp44.unk48 = 0x20;
        sp44.unk42 = 0x5F;
        sp44.unk58[0] = 0xFFFF;
        sp44.unk58[1] = 0xFFFF;
        sp44.unk58[2] = 0xFFFF;
        sp44.unk4C[0] = 0x400;
        sp44.unk4C[1] = 0xEA60;
        sp44.unk4C[2] = 0x1000;
        sp44.unk3C = 0.01f;
        break;
    case 0x44D:
        sp44.unk24.x = (f32) mathRnd(-0xA, 0xA) * 0.01f;
        sp44.unk24.z = (f32) mathRnd(-0xA, 0xA) * 0.01f;
        sp44.unk8 = 0x258;
        sp44.unk60 = 0x7F;
        sp44.unk44 = 0x0A100100;
        sp44.unk48 = 0x20;
        sp44.unk42 = 0x62;
        sp44.unk58[0] = 0x400;
        sp44.unk58[1] = 0xEA60;
        sp44.unk58[2] = 0x1000;
        sp44.unk4C[0] = 0;
        sp44.unk4C[1] = 0xC350;
        sp44.unk4C[2] = 0;
        sp44.unk3C = 0.08f;
        break;
    case 0x44E:
        sp44.unk8 = 0xC8;
        sp44.unk44 = 0x11000004;
        sp44.unk42 = 0x151;
        sp44.unk4 = 0x44F;
        sp44.unk30.y = 610.0f;
        sp44.unk3C = 0.0023f;
        break;
    case 0x44F:
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
        gDLL_24_Waterfx->vtbl->spawn_splash(transform->transl.x, transform->transl.y, transform->transl.z, 4.0f);
        dll_amSfx->Play(obj, mathRnd(0x3FE, 0x401), MAX_VOLUME, NULL, NULL, 0, NULL);
        sp44.unk8 = 1;
        sp44.unk44 = 0x0A000001;
        sp44.unk42 = 0x56;
        sp44.unk3C = 0.0001f;
        break;
    case 0x450:
        sp44.unk8 = 0xC8;
        sp44.unk44 = 0x11000004;
        sp44.unk42 = 0x151;
        sp44.unk4 = 0x451;
        sp44.unk30.y = 110.0f;
        sp44.unk3C = 0.0023f;
        break;
    case 0x451:
        dll_amSfx->Play(obj, mathRnd(0x3FE, 0x401), MAX_VOLUME, NULL, NULL, 0, NULL);
        sp44.unk8 = 0x64;
        sp44.unk44 = 0x0A100201;
        sp44.unk42 = 0x56;
        sp44.unk3C = 100.0f;
        sp44.unk3C *= 0.0003f;
        break;
    default:
        // STUBBED_PRINTF("PARTFX dfalls type < %d > not found.. returning\n", id); // default.dol
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
