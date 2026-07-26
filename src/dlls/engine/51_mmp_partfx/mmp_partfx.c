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

// offset: 0x0 | ctor
void mmpPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void mmpPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void mmpPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void mmpPartfx_Func_20(s32 arg0) {
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
s32 mmpPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp34;
    s32 var_a2_3;
    s32 pad;

    data_0 += 0.001f;
    if (data_0 > 1.0f) {
        data_0 = 0.1f;
    }
    data_4 += 0.0003f;
    if (data_4 > 1.0f) {
        data_4 = 0.3f;
    }
    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX mmpning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
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
    switch (id) {
    case 0x708:
        sp34.unk24.x = (f32) mathRnd(0xA, 0x19) * 0.03f;
        sp34.unk3C = 0.1f;
        sp34.unk8 = mathRnd(0x15E, 0x190);
        sp34.unk44 = 0x0A100100;
        sp34.unk48 = 0x01000000;
        sp34.unk42 = 0x62;
        break;
    case 0x709:
        sp34.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.01f;
        if (mathRnd(0, 1) != 0) {
            sp34.unk24.y = -sp34.unk24.y;
        }
        sp34.unk8 = 0x78;
        sp34.unk3C = 0.001f;
        sp34.unk60 = mathRnd(0x7F, 0xFF);
        sp34.unk44 = 0x80480000;
        sp34.unk48 = 0x440000;
        sp34.unk42 = mathRnd(0x525, 0x528);
        break;
    case 0x70A:
        sp34.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp34.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp34.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp34.unk8 = 0x32;
        sp34.unk44 = 0x480100;
        sp34.unk3C = 0.0012f;
        sp34.unk42 = mathRnd(0x525, 0x528);
        break;
    case 0x70B:
        sp34.unk8 = 0x64;
        sp34.unk44 = 0x180210;
        sp34.unk42 = 0x208;
        sp34.unk48 = 0x01000000;
        sp34.unk3C = 0.08f;
        break;
    case 0x70C:
        sp34.unk8 = mathRnd(0x19, 0x4B);
        sp34.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.01f;
        sp34.unk24.y = (f32) sp34.unk8 * 0.015f;
        sp34.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.01f;
        sp34.unk3C = (f32) mathRnd(0x32, 0x64) * 0.00003f;
        sp34.unk44 = 0x01082000;
        sp34.unk42 = mathRnd(0x208, 0x20A);
        sp34.unk48 = 0x01400000;
        break;
    case 0x70D:
        sp34.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp34.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp34.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp34.unk8 = 0x32;
        sp34.unk44 = 0x480110;
        sp34.unk3C = 0.0012f;
        sp34.unk42 = mathRnd(0x525, 0x528);
        break;
    case 0x70E:
        if (data != NULL) {
            sp34.unk60 = *(u8*)data;
        } else {
            sp34.unk60 = 0x7F;
        }
        sp34.unk30.y = 5.0f;
        sp34.unk8 = 1;
        sp34.unk44 = 0x80010;
        sp34.unk42 = 0x525;
        sp34.unk3C = 0.015f;
        break;
    case 0x70F:
        sp34.unk8 = mathRnd(0xF, 0x2D);
        sp34.unk30.x = (f32) mathRnd(-5, 5);
        sp34.unk30.z = (f32) mathRnd(-5, 5);
        sp34.unk24.x = (f32) mathRnd(-0x28, 0x28) * 0.01f;
        sp34.unk24.y = (f32) sp34.unk8 * 0.015f;
        sp34.unk24.z = (f32) mathRnd(-0x28, 0x28) * 0.01f;
        sp34.unk3C = (f32) mathRnd(0x32, 0x46) * 0.00002f;
        sp34.unk60 = 0xA0;
        sp34.unk48 = 0x01400000;
        sp34.unk44 = 0x01082010;
        sp34.unk42 = mathRnd(0x208, 0x20A);
        break;
    case 0x710:
        sp34.unk8 = mathRnd(0xF, 0x4B);
        sp34.unk30.y = 15.0f;
        sp34.unk30.z = -5.0f;
        sp34.unk24.x = (f32) mathRnd(-0x1E, 0x1E) * 0.01f;
        sp34.unk24.y = (f32) sp34.unk8 * 0.015f;
        sp34.unk24.z = (f32) mathRnd(0x14, 0x46) * -0.01f;
        sp34.unk3C = (f32) mathRnd(0x28, 0x3C) * 0.00002f;
        sp34.unk60 = mathRnd(0x3C, 0xA0);
        sp34.unk44 = 0x81080200;
        sp34.unk42 = 0x32;
        break;
    case 0x711:
        sp34.unk8 = mathRnd(0x23, 0x4B);
        sp34.unk30.y = 15.0f;
        sp34.unk30.z = -5.0f;
        sp34.unk24.x = (f32) mathRnd(-0x32, 0x32) * 0.01f;
        sp34.unk24.y = (f32) sp34.unk8 * 0.013f;
        sp34.unk24.z = (f32) mathRnd(0x14, 0x3C) * -0.01f;
        sp34.unk3C = (f32) mathRnd(0x28, 0x3C) * 0.00002f;
        sp34.unk60 = mathRnd(0x64, 0xC8);
        sp34.unk44 = 0x81080200;
        sp34.unk42 = 0x32;
        break;
    case 0x712:
        sp34.unk8 = mathRnd(0x32, 0x64);
        sp34.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp34.unk24.y = (f32) sp34.unk8 * 0.0018f;
        sp34.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.01f;
        sp34.unk3C = 0.007f;
        if (mathRnd(0, 2) != 0) {
            sp34.unk44 = 0x0A100008;
        } else {
            sp34.unk44 = 0x180008;
        }
        sp34.unk48 = 0x01400000;
        sp34.unk42 = 0x5F;
        break;
    case 0x713:
        sp34.unk8 = 0x32;
        sp34.unk60 = 0x60;
        sp34.unk44 = 0x0A100100;
        sp34.unk42 = 0x74;
        sp34.unk30.y = 13.0f;
        sp34.unk3C = 0.075f;
        break;
    case 0x714:
        sp34.unk60 = mathRnd(0x3C, 0xA0);
        if (data != NULL) {
            sp34.unk60 = (u8) (u32) (((f32) (*(u8*)data) / 255.0f) * (f32) sp34.unk60);
        }
        sp34.unk3C = (f32) mathRnd(0x32, 0x64) * 0.00009f;
        sp34.unk8 = 1;
        sp34.unk44 = 0x80010;
        sp34.unk42 = 0x32;
        break;
    case 0x715:
        if (data != NULL) {
            sp34.unk24.x = (f32) mathRnd(-0x19, 0x19) * 0.005f;
            sp34.unk24.y = (f32) mathRnd(5, 0x32) * 0.005f;
            sp34.unk24.z = (f32) mathRnd(-0x19, 0x19) * 0.005f;
            sp34.unk3C = 0.0015f;
            sp34.unk8 = mathRnd(0x28, 0x78);
            sp34.unk44 = 0x80480000;
            sp34.unk48 = 0x400000;
        } else {
            sp34.unk3C = (f32) mathRnd(0x32, 0x64) * 0.00005f;
            sp34.unk8 = 0x78;
            sp34.unk44 = 0x80580210;
        }
        sp34.unk60 = 0xFF;
        sp34.unk42 = 0x32;
        break;
    case 0x716:
        sp34.unk30.x = (f32) mathRnd(-0x14, 0x14);
        sp34.unk30.y = (f32) mathRnd(-0x14, 0x14);
        sp34.unk30.z = (f32) mathRnd(-0x14, 0x14);
        sp34.unk24.y = (f32) mathRnd(0x5A, 0x64) * 0.03f;
        sp34.unk61 = 0xF;
        sp34.unk3C = (f32) mathRnd(0x5A, 0x64) * 0.001f;
        sp34.unk44 = 0x80080100;
        sp34.unk48 = 0x01000000;
        sp34.unk60 = mathRnd(0x96, 0xC8);
        sp34.unk8 = mathRnd(0x32, 0x46);
        sp34.unk42 = 0x1A1;
        break;
    case 0x717:
        sp34.unk30.x = (f32) mathRnd(-0x96, 0x96) * 0.1f;
        sp34.unk30.y = (f32) mathRnd(0x64, 0x12C) * 0.1f;
        sp34.unk30.z = (f32) mathRnd(-0x96, -0x32) * 0.1f;
        sp34.unk3C = 0.0012f;
        sp34.unk8 = mathRnd(0x32, 0x96);
        sp34.unk44 = 0x80480110;
        sp34.unk42 = mathRnd(0x527, 0x528);
        break;
    case 0x718:
        sp34.unk24.y = (f32) mathRnd(8, 0xA) * 0.05f;
        if (data != NULL) {
            sp34.unk24.y *= (1.0f + (*(f32*)data / 70.0f));
        }
        sp34.unk3C = (f32) mathRnd(6, 0xC) * 0.002f;
        sp34.unk8 = mathRnd(0x3C, 0x64);
        sp34.unk44 = 0x80180000;
        sp34.unk48 = 0x01440000;
        sp34.unk42 = 0x162;
        sp34.unk60 = 0x7F;
        break;
    case 0x719:
        sp34.unk30.x = (f32) mathRnd(-0x32, 0x32);
        sp34.unk30.y = (f32) mathRnd(-0x32, 0x32);
        sp34.unk30.z = (f32) mathRnd(-0x32, 0x32);
        sp34.unk24.y = (f32) mathRnd(0x5A, 0x64) * 0.03f;
        if (mathRnd(0, 1) != 0) {
            sp34.unk24.y = 0.0f - sp34.unk24.y;
        }
        sp34.unk61 = 0xF;
        sp34.unk3C = (f32) mathRnd(0x5A, 0x64) * 0.0001f;
        sp34.unk44 = 0x80480000;
        sp34.unk48 = 0x400000;
        sp34.unk8 = mathRnd(0x46, 0x64);
        sp34.unk42 = 0x47;
        break;
    case 0x71A:
        sp34.unk30.z = 8.0f;
        sp34.unk3C = (f32) mathRnd(0x4B, 0x64) * 0.0021f;
        sp34.unk8 = 1;
        sp34.unk44 = 0x80010;
        sp34.unk42 = 0x17D;
        sp34.unk60 = 0x7F;
        break;
    case 0x71B:
        sp34.unk8 = 0x64;
        sp34.unk44 = 0x180200;
        sp34.unk42 = 0x73;
        sp34.unk60 = 0xFF;
        sp34.unk3C = 0.5f;
        break;
    case 0x71C:
        sp34.unk8 = mathRnd(0x28, 0x78);
        sp34.unk24.x = (f32) mathRnd(-0x32, 0x32) * 0.05f;
        sp34.unk24.y = (f32) sp34.unk8 * 0.035f;
        sp34.unk24.z = (f32) mathRnd(-0x32, 0x32) * 0.05f;
        sp34.unk44 = 0x03000000;
        sp34.unk48 = 0x600020;
        sp34.unk42 = 0x20D;
        sp34.unk60 = 0xFF;
        sp34.unk4C[0] = sp34.unk58[0] = sp34.unk58[1] = sp34.unk58[2] = 0xFFFF;
        sp34.unk4C[1] = sp34.unk4C[2] = 0;
        sp34.unk3C = 0.005f;
        break;
    case 0x71D:
        sp34.unk30.x = (f32) mathRnd(-0x14, 0x14);
        sp34.unk30.y = (f32) mathRnd(-0x14, 0x14);
        sp34.unk30.z = (f32) mathRnd(-0x14, 0x14);
        sp34.unk61 = 0xF;
        sp34.unk3C = (f32) mathRnd(0x78, 0xC8) * 0.001f;
        sp34.unk44 = 0x80180100;
        sp34.unk60 = mathRnd(0x32, 0x64);
        sp34.unk8 = mathRnd(0x64, 0x8C);
        sp34.unk42 = 0x185;
        break;
    case 0x71E:
        sp34.unk30.x = (f32) mathRnd(-0x23, 0x23);
        sp34.unk30.y = (f32) mathRnd(0, 0x1E);
        sp34.unk30.z = (f32) mathRnd(-0x23, 0x23);
        sp34.unk24.y = (f32) mathRnd(8, 0xA) * 0.05f;
        sp34.unk3C = (f32) mathRnd(6, 0xC) * 0.002f;
        sp34.unk8 = mathRnd(0x64, 0x96);
        sp34.unk44 = 0x80180000;
        sp34.unk48 = 0x01440000;
        sp34.unk42 = 0x564;
        sp34.unk60 = 0x7F;
        break;
    case 0x71F:
        sp34.unk24.y = (f32) mathRnd(8, 0xA) * 0.05f;
        sp34.unk3C = (f32) mathRnd(6, 0xC) * 0.0015f;
        sp34.unk8 = mathRnd(0x3C, 0x50);
        sp34.unk44 = 0x80180000;
        sp34.unk48 = 0x01440000;
        sp34.unk42 = 0x564;
        sp34.unk60 = 0x7F;
        break;
    case 0x720:
        sp34.unk24.y = (f32) mathRnd(8, 0xA) * 0.02f;
        sp34.unk3C = (f32) mathRnd(6, 0xC) * 0.0015f;
        sp34.unk8 = mathRnd(0x3C, 0x50);
        sp34.unk44 = 0x80180200;
        sp34.unk48 = 0x01000000;
        sp34.unk42 = 0x564;
        sp34.unk60 = 0x7F;
        break;
    case 0x721:
        sp34.unk3C = (f32) mathRnd(6, 0xC) * 0.0005f;
        sp34.unk8 = mathRnd(0xFA, 0x15E);
        sp34.unk44 = 0x80480008;
        sp34.unk48 = 0x400000;
        sp34.unk42 = 0x47;
        break;
    case 0x722:
        sp34.unk30.y = -45.0f;
        sp34.unk8 = mathRnd(0x32, 0x64);
        sp34.unk24.x = (f32) mathRnd(-0x3C, 0x3C) * 0.01f;
        sp34.unk24.z = (f32) mathRnd(-0x3C, 0x3C) * 0.01f;
        sp34.unk24.y = sqrtf(SQ(sp34.unk24.x) + SQ(sp34.unk24.z)) * 0.25f;
        sp34.unk3C = 0.01f;
        sp34.unk44 = 0x80010;
        sp34.unk48 = 0x01400000;
        sp34.unk42 = 0x564;
        sp34.unk60 = mathRnd(0x46, 0xBE);
        break;
    case 0x723:
        sp34.unk8 = mathRnd(0x23, 0x2D);
        if (data != NULL) {
            var_a2_3 = (s32) (*(s32*)data) + 5;
        } else {
            var_a2_3 = 5;
        }
        sp34.unk24.y = ((f32) mathRnd(8, 0xC) * 0.2f) * ((f32) var_a2_3 / 50.0f);
        var_a2_3 = 0x41 - var_a2_3;
        sp34.unk24.x = (f32) mathRnd(-var_a2_3,var_a2_3) * 0.005f;
        sp34.unk24.z = (f32) mathRnd(-var_a2_3,var_a2_3) * 0.005f;
        sp34.unk3C = (f32) mathRnd(6, 0xC) * 0.002f;
        sp34.unk44 = 0x80080000;
        sp34.unk60 = mathRnd(0x40, 0x7F);
        sp34.unk48 = 0x01400000;
        sp34.unk42 = 0x564;
        break;
    case 0x724:
        sp34.unk24.y = (f32) mathRnd(8, 0xA) * 0.05f;
        sp34.unk3C = (f32) mathRnd(6, 0xC) * 0.002f;
        sp34.unk8 = mathRnd(0x1E, 0x3C);
        sp34.unk44 = 0x80180000;
        sp34.unk48 = 0x01440000;
        sp34.unk42 = 0x162;
        sp34.unk60 = 0x7F;
        break;
    default:
        // STUBBED_PRINTF("PARTFX mmp type < %d > not found.. returning\n", id); // default.dol
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
