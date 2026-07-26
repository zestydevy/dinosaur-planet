#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/** @file Drakor partfx */

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
void dakPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void dakPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dakPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void dakPartfx_Func_20(s32 arg0) {
    f32 temp_fv0;
    s32 var_v0;
    s32 var_v0_2;

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
s32 dakPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp2C;
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
        // STUBBED_PRINTF("\nPARTFX dakning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
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
    sp2C.unk44 = 0;
    switch (id) {
    case 0x6D7:
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
        sp2C.unk30.x = transform->transl.x;
        sp2C.unk30.y = transform->transl.y;
        sp2C.unk30.z = transform->transl.z;
        sp2C.unk3C = (f32) mathRnd(0xA, 0x1E) * 0.001f;
        sp2C.unk8 = mathRnd(0x118, 0x12C);
        sp2C.unk44 = 0x80180214;
        sp2C.unk42 = 0x5C;
        break;
    case 0x6D8:
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
        sp2C.unk30.x = transform->transl.x;
        sp2C.unk30.y = transform->transl.y;
        sp2C.unk30.z = transform->transl.z;
        sp2C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.001f;
        sp2C.unk8 = mathRnd(0x118, 0x12C);
        sp2C.unk44 = 0x80180214;
        sp2C.unk42 = 0x206;
        break;
    case 0x6D9:
        sp2C.unk24.x = (f32) mathRnd(-0x1E, 0x1E) * 0.02f;
        sp2C.unk24.y = (f32) mathRnd(-0x1E, 0x1E) * 0.02f;
        sp2C.unk24.z = (f32) mathRnd(-0x1E, 0x1E) * 0.02f;
        sp2C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.00035f;
        sp2C.unk8 = 0x64;
        sp2C.unk60 = 0xFF;
        sp2C.unk44 = 0x80114;
        sp2C.unk48 = 0x10008;
        sp2C.unk42 = mathRnd(0x157, 0x158);
        break;
    case 0x6DA:
        sp2C.unk8 = 0x14;
        sp2C.unk44 = 0x80480210;
        sp2C.unk42 = 0x206;
        sp2C.unk60 = 0x9D;
        sp2C.unk3C = 0.035f;
        break;
    case 0x6DB:
        if (data != NULL) {
            sp2C.unk24.x = (f32) mathRnd(-0x96, 0x96) * 0.01f;
            sp2C.unk24.z = (f32) mathRnd(-0x96, 0x96) * 0.01f;
            sp2C.unk24.y = (f32) mathRnd(0x64, 0x190) * 0.01f;
            sp2C.unk3C = (f32) mathRnd(0xF, 0x14) * 0.0005f;
            sp2C.unk8 = 0x32;
            sp2C.unk58[0] = 0xFFFF;
            sp2C.unk58[1] = 0xFFFF;
            sp2C.unk58[2] = 0xFFFF;
            sp2C.unk4C[0] = 0xFFFF;
            sp2C.unk4C[1] = 0;
            sp2C.unk4C[2] = 0;
            sp2C.unk44 = 0x03000200;
            sp2C.unk48 = 0x200022;
        } else {
            sp2C.unk3C = (f32) mathRnd(0xF, 0x14) * 0.002f;
            sp2C.unk8 = 1;
            sp2C.unk44 = 0x80000;
        }
        sp2C.unk60 = 0xFF;
        sp2C.unk42 = 0x206;
        break;
    case 0x6DC:
        sp2C.unk24.y = (f32) mathRnd(8, 0xA) * 0.05f;
        sp2C.unk3C = (f32) mathRnd(0x12, 0x1C) * 0.001f;
        sp2C.unk8 = mathRnd(0x32, 0x64);
        sp2C.unk44 = 0x80180200;
        sp2C.unk42 = 0x162;
        sp2C.unk60 = 0xFF;
        break;
    case 0x6DD:
        sp2C.unk8 = 0xA;
        sp2C.unk60 = 0xC3;
        sp2C.unk61 = 0x10;
        sp2C.unk44 = 0x580110;
        sp2C.unk42 = 0x206;
        sp2C.unk3C = 0.1f;
        break;
    case 0x6DE:
        sp2C.unk24.x = (f32) mathRnd(-0xF, 0xF) * (data_0 * 0.185f);
        sp2C.unk24.z = (f32) mathRnd(-0xF, 0xF) * (data_0 * 0.185f);
        sp2C.unk24.y = (f32) mathRnd(-0xF, 0xF) * (data_0 * 0.185f);
        sp2C.unk60 = 0x7D;
        sp2C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.000288f;
        sp2C.unk44 = 0x03000000;
        sp2C.unk48 = 0x300000;
        sp2C.unk8 = 0x14;
        sp2C.unk42 = 0x206;
        break;
    case 0x6DF:
        sp2C.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.01f;
        sp2C.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.01f;
        sp2C.unk24.y = (f32) mathRnd(-0xF, 0xF) * 0.01f;
        sp2C.unk60 = 0xFF;
        sp2C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.0001f;
        sp2C.unk44 = 0x80200;
        sp2C.unk48 = 0x100000;
        sp2C.unk8 = 0x64;
        sp2C.unk42 = 0x125;
        break;
    case 0x6E0:
        sp2C.unk24.x = (f32) mathRnd(-0xF, 0xF) * 0.5f;
        sp2C.unk24.z = (f32) mathRnd(-0xF, 0xF) * 0.5f;
        sp2C.unk24.y = (f32) mathRnd(-0xF, 0xF) * 0.5f;
        sp2C.unk60 = 0xFF;
        sp2C.unk3C = (f32) mathRnd(0xA, 0x14) * 0.000288f;
        sp2C.unk44 = 0x02000200;
        sp2C.unk48 = 0x300000;
        sp2C.unk8 = 0x1E;
        sp2C.unk42 = 0x33;
        break;
    case 0x6E1:
        sp2C.unk8 = 0x46;
        sp2C.unk58[0] = 0xFF00;
        sp2C.unk58[1] = 0xFF00;
        sp2C.unk58[2] = 0xFF00;
        sp2C.unk4C[0] = 0xFF00;
        sp2C.unk4C[1] = 0;
        sp2C.unk4C[2] = 0xFF00;
        sp2C.unk44 = 0x100100;
        sp2C.unk48 = 0x02000020;
        sp2C.unk60 = 0x7F;
        sp2C.unk42 = 0x72;
        sp2C.unk3C = 0.2f;
        break;
    default:
        // STUBBED_PRINTF("PARTFX dak type < %d > not found.. returning\n", id); // default.dol
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
