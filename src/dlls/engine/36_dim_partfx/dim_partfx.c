#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/13_expgfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/** @file official filename: dim_partfx.c */

/*0x0*/ static SRT bss_0;

// offset: 0x0 | ctor
void dimPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void dimPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dimPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void dimPartfx_Func_20(s32 arg0) {

}

// offset: 0x2C | func: 2 | export: 1
s32 dimPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp64;
    f32 temp_fv0_17;
    s32 _pad[3];
    f32 sp50;
    s32 _pad2[4];

    if (obj == NULL) {
        // STUBBED_PRINTF("\nPARTFX dimning NULL no longer allowed for object...returning type %d\n\n", id); // default.dol
        return -1;
    }
    if (flags & 0x200000) {
        if (transform == NULL) {
            // STUBBED_PRINTF("ERROR partfx EXF_NOOWNER & no attrib %d\n", id); // default.dol
            return -1;
        }
        sp64.transform.transl.x = transform->transl.x;
        sp64.transform.transl.y = transform->transl.y;
        sp64.transform.transl.z = transform->transl.z;
        sp64.transform.scale = transform->scale;
        sp64.transform.roll = transform->roll;
        sp64.transform.pitch = transform->pitch;
        sp64.transform.yaw = transform->yaw;
        sp64.unk62 = arg4;
    }
    sp64.unk44 = 0;
    sp64.unk48 = 0;
    sp64.unk5E = id;
    sp64.unk0 = obj;
    sp64.unk8 = 0;
    sp64.unk4 = -1;
    sp64.unk60 = 0xFF;
    sp64.unk61 = 0;
    sp64.unk42 = 0;
    sp64.unk30.x = 0.0f;
    sp64.unk30.y = 0.0f;
    sp64.unk30.z = 0.0f;
    sp64.unk24.x = 0.0f;
    sp64.unk24.y = 0.0f;
    sp64.unk24.z = 0.0f;
    sp64.unk3C = 0.0f;
    sp64.unk58[0] = 0xFFFF;
    sp64.unk58[1] = 0xFFFF;
    sp64.unk58[2] = 0xFFFF;
    sp64.unk4C[0] = 0xFFFF;
    sp64.unk4C[1] = 0xFFFF;
    sp64.unk4C[2] = 0xFFFF;
    sp64.unk40 = 0;
    switch (id) {
    case 0x1F4:
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
        sp64.unk30.x = (f32) mathRnd(-0x14, -0xA) * 0.1f;
        sp64.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp64.unk30.z = (f32) mathRnd(-0xA, 0) * 0.1f;
        if (transform != NULL) {
            sp64.unk30.x += transform->transl.x;
            sp64.unk30.y += transform->transl.y;
            sp64.unk30.z += transform->transl.z;
        }
        sp64.unk3C = (f32) mathRnd(0xD, 0x14) * 0.0007f;
        sp64.unk8 = 0x19;
        sp64.unk60 = 0xFF;
        sp64.unk44 = 0x80200;
        sp64.unk42 = 0x184;
        sp64.unk61 = mathRnd(0, 0x14) + 4;
        break;
    case 0x1F5:
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
        sp64.unk30.x = (f32) mathRnd(-0x14, -0xA) * 0.1f;
        sp64.unk30.y = (f32) mathRnd(-0xA, 0xA) * 0.1f;
        sp64.unk30.z = (f32) mathRnd(-0xA, 0) * 0.1f;
        if (transform != NULL) {
            sp64.unk30.x += transform->transl.x;
            sp64.unk30.y += transform->transl.y;
            sp64.unk30.z += transform->transl.z;
        }
        sp64.unk3C = (f32) mathRnd(1, 4) * 0.003f;
        sp64.unk8 = 0x19;
        sp64.unk60 = 0xFF;
        sp64.unk44 = 0x80200;
        sp64.unk42 = 0x184;
        sp64.unk61 = mathRnd(0, 0x14) + 4;
        break;
    case 0x1F6:
        sp64.unk3C = (f32) mathRnd(0xA, 0x14) * 0.00022f;
        sp64.unk8 = 0x14;
        sp64.unk60 = 0x40;
        sp64.unk44 = 0x80000;
        sp64.unk48 = 0x80;
        sp64.unk42 = 0x16D;
        sp64.unk61 = mathRnd(0, 0x14) + 4;
        break;
    case 0x1F7:
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
            sp64.unk30.y = transform->transl.y;
        }
        sp64.unk3C = 0.03f;
        sp64.unk8 = mathRnd(0, 0x1E) + 0x46;
        sp64.unk60 = 0x7F;
        sp64.unk44 = 0x80110;
        sp64.unk42 = 0x65;
        sp64.unk61 = 0x20;
        break;
    case 0x1F8:
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
            sp64.unk3C = transform->scale * 0.03f;
        } else {
            sp64.unk3C = 0.03f;
        }
        sp64.unk8 = mathRnd(0, 0x1E) + 0x46;
        sp64.unk60 = 0x64;
        sp64.unk44 |= 0x80100;
        sp64.unk42 = 0x206;
        sp64.unk61 = 0;
        sp64.unk58[0] = 0xE600;
        sp64.unk58[1] = 0x8800;
        sp64.unk58[2] = 0xA100;
        sp64.unk4C[0] = 0xE600;
        sp64.unk4C[1] = 0x8800;
        sp64.unk4C[2] = 0xA100;
        sp64.unk48 = 0x20;
        break;
    case 0x1F9:
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
        sp64.unk30.x = ((f32) mathRnd(-0xF, 0xF) * 0.1f) + (transform->transl.x - obj->globalPosition.x);
        sp64.unk30.z = ((f32) mathRnd(-0xF, 0xF) * 0.1f) + (transform->transl.z - obj->globalPosition.z);
        sp64.unk30.y = 2.0f;
        sp64.unk24.y = (f32) mathRnd(0xF, 0x23) * 0.0015f;
        sp64.unk3C = (f32) mathRnd(6, 0xA) * transform->scale;
        sp64.unk8 = 0x32;
        sp64.unk60 = 0x9B;
        sp64.unk44 = 0x80180100;
        sp64.unk42 = 0x5F;
        sp64.unk58[0] = 0xFFFF;
        sp64.unk58[1] = 0xFFFF;
        sp64.unk58[2] = 0xFFFF;
        sp64.unk4C[0] = 0xD8EF;
        sp64.unk4C[1] = 0xD8EF;
        sp64.unk4C[2] = 0xFFFF;
        sp64.unk48 = 0x20;
        break;
    case 0x1FA:
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
        sp64.unk30.x = ((f32) mathRnd(-0xF, 0xF) * 0.1f) + (transform->transl.x - obj->globalPosition.x);
        sp64.unk30.z = ((f32) mathRnd(-0xF, 0xF) * 0.1f) + (transform->transl.z - obj->globalPosition.z);
        sp64.unk30.y = 2.0f;
        sp64.unk24.y = (f32) mathRnd(0x19, 0x23) * 0.019f;
        sp64.unk3C = (f32) mathRnd(6, 0xA) * 0.00005f;
        sp64.unk8 = 0x32;
        sp64.unk60 = 0xFF;
        sp64.unk44 = 0x81080100;
        sp64.unk42 = 0x5F;
        sp64.unk58[0] = 0xFFFF;
        sp64.unk58[1] = 0xFFFF;
        sp64.unk58[2] = 0xFFFF;
        sp64.unk4C[0] = 0xD8EF;
        sp64.unk4C[1] = 0xD8EF;
        sp64.unk4C[2] = 0xFFFF;
        sp64.unk48 = 0x20;
        break;
    case 0x1FB:
        sp64.unk8 = 0x10;
        sp64.unk60 = 0xFF;
        sp64.unk44 = 0x100114;
        sp64.unk42 = 0x17C;
        sp64.unk3C = 0.035f;
        break;
    case 0x1FC:
        sp64.unk8 = 0x44;
        sp64.unk44 = 0x100201;
        sp64.unk42 = 0x4C;
        sp64.unk3C = 0.03f;
        break;
    case 0x1FD:
        sp64.unk30.x = 0.0f;
        sp64.unk30.y = (f32) mathRnd(-3, 3);
        sp64.unk30.z = (f32) mathRnd(-3, 3);
        sp64.unk24.x = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp64.unk24.y = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp64.unk24.z = (f32) mathRnd(-0x14, 0x14) * 0.002f;
        sp64.unk8 = 0x1E;
        sp64.unk60 = 0xC8;
        sp64.unk44 = 0x140101;
        sp64.unk3C = 0.02f;
        if (mathRnd(0, 1) != 0) {
            sp64.unk42 = 0x33;
        } else {
            sp64.unk42 = 0x17D;
        }
        break;
    case 0x1FE:
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
        if (data == NULL) {
            return -1;
        }
        if (transform != NULL) {
            sp64.unk30.x = transform->transl.x;
            sp64.unk30.y = transform->transl.y;
            sp64.unk30.z = transform->transl.z;
        }
        if (data != NULL) {
            sp64.unk24.x = ((f32*)data)[0];
            sp64.unk24.y = (f32) mathRnd(0, 0x14) * 0.03f;
            sp64.unk24.z = ((f32*)data)[1];
        }
        sp64.unk3C = ((f32) mathRnd(0, 0xA) * 0.0002f) + 0.000945f;
        sp64.unk8 = mathRnd(0xBE, 0xFA);
        sp64.unk60 = 0x9B;
        sp64.unk44 = 0x81088000;
        sp64.unk42 = 0x23C;
        break;
    case 0x1FF:
        sp64.unk8 = 0xC8;
        sp64.unk44 = 0x11000004;
        sp64.unk42 = 0x151;
        sp64.unk4 = 0x200;
        sp64.unk30.y = 410.0f;
        sp64.unk3C = 0.0023f;
        break;
    case 0x200:
        dll_amSfx->Play(NULL, mathRnd(0x3FE, 0x401), MAX_VOLUME, NULL, NULL, 0, NULL);
        sp64.unk8 = 0x64;
        sp64.unk44 = 0x0A100201;
        sp64.unk42 = 0x56;
        sp64.unk3C = 100.0f;
        sp64.unk3C *= 0.0003f;
        break;
    case 0x201:
        sp64.unk30.x = (f32) mathRnd(-0x64, 0x64) / 20.0f;
        sp64.unk30.y = (f32) mathRnd(-0x32, 0x32) / 10.0f;
        sp64.unk30.z = (f32) mathRnd(-0x64, 0x64) / 20.0f;
        sp64.unk24.y = (f32) mathRnd(1, 5) * 0.03f;
        sp64.unk8 = 0x64;
        sp64.unk61 = 0;
        sp64.unk44 = 0x100201;
        sp64.unk42 = 0x63;
        sp64.unk3C = 0.001f;
        break;
    case 0x202:
        sp64.unk30.x = (f32) mathRnd(-0x64, 0x64) / 20.0f;
        sp64.unk30.y = (f32) mathRnd(-0x32, 0x32) / 10.0f;
        sp64.unk30.z = (f32) mathRnd(-0x64, 0x64) / 20.0f;
        sp64.unk24.y = (f32) mathRnd(1, 4) * 0.035f;
        sp64.unk3C = ((f32) mathRnd(0, 0xA) * 0.0001f) + 0.0015f;
        sp64.unk8 = 0xA0;
        sp64.unk61 = 0;
        sp64.unk44 = 0x100201;
        sp64.unk42 = 0x63;
        break;
    case 0x203:
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
        sp64.unk30.y = transform->transl.y;
        sp64.unk24.y = 0.005f;
        switch (mathRnd(0, 3)) {
        case 0:
            sp64.unk30.x = transform->transl.x;
            sp64.unk30.z = (f32) mathRnd((s32) (s16) (s32) -transform->transl.z, (s32) (s16) (s32) transform->transl.z);
            break;
        case 1:
            sp64.unk30.x = -transform->transl.x;
            sp64.unk30.z = (f32) mathRnd((s32) (s16) (s32) -transform->transl.z, (s32) (s16) (s32) transform->transl.z);
            break;
        case 2:
            sp64.unk30.z = transform->transl.z;
            sp64.unk30.x = (f32) mathRnd((s32) (s16) (s32) -transform->transl.x, (s32) (s16) (s32) transform->transl.x);
            break;
        case 3:
            sp64.unk30.z = -transform->transl.z;
            sp64.unk30.x = (f32) mathRnd((s32) (s16) (s32) -transform->transl.x, (s32) (s16) (s32) transform->transl.x);
            break;
        }
        sp64.unk8 = 0x3C;
        sp64.unk44 = 0x100210;
        sp64.unk42 = 0x184;
        sp64.unk60 = 0xC4;
        sp64.unk3C = 0.01f;
        break;
    case 0x204:
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
        sp64.unk30.y = transform->transl.y;
        sp64.unk24.y = 0.005f;
        switch (mathRnd(0, 3)) {
        case 0:
            sp64.unk30.x = transform->transl.x;
            sp64.unk30.z = (f32) mathRnd((s32) (s16) (s32) -transform->transl.z, (s32) (s16) (s32) transform->transl.z);
            break;
        case 1:
            sp64.unk30.x = -transform->transl.x;
            sp64.unk30.z = (f32) mathRnd((s32) (s16) (s32) -transform->transl.z, (s32) (s16) (s32) transform->transl.z);
            break;
        case 2:
            sp64.unk30.z = transform->transl.z;
            sp64.unk30.x = (f32) mathRnd((s32) (s16) (s32) -transform->transl.x, (s32) (s16) (s32) transform->transl.x);
            break;
        case 3:
            sp64.unk30.z = -transform->transl.z;
            sp64.unk30.x = (f32) mathRnd((s32) (s16) (s32) -transform->transl.x, (s32) (s16) (s32) transform->transl.x);
            break;
        }
        sp64.unk24.y = (f32) mathRnd(0x28, 0x50) * 0.0045f;
        sp64.unk3C = (f32) mathRnd(0x28, 0x50) * 0.00002f;
        sp64.unk8 = 0x78;
        sp64.unk61 = 0;
        sp64.unk44 = 0x80400110;
        sp64.unk42 = 0x47;
        break;
    case 0x205:
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
        sp64.unk30.y = transform->transl.y;
        sp64.unk24.y = 0.005f;
        switch (mathRnd(0, 3)) {
        case 0:
            sp64.unk30.x = transform->transl.x;
            sp64.unk30.z = (f32) mathRnd((s32) (s16) (s32) -transform->transl.z, (s32) (s16) (s32) transform->transl.z);
            break;
        case 1:
            sp64.unk30.x = -transform->transl.x;
            sp64.unk30.z = (f32) mathRnd((s32) (s16) (s32) -transform->transl.z, (s32) (s16) (s32) transform->transl.z);
            break;
        case 2:
            sp64.unk30.z = transform->transl.z;
            sp64.unk30.x = (f32) mathRnd((s32) (s16) (s32) -transform->transl.x, (s32) (s16) (s32) transform->transl.x);
            break;
        case 3:
            sp64.unk30.z = -transform->transl.z;
            sp64.unk30.x = (f32) mathRnd((s32) (s16) (s32) -transform->transl.x, (s32) (s16) (s32) transform->transl.x);
            break;
        }
        sp64.unk24.y = (f32) mathRnd(0x28, 0x50) * 0.0045f;
        sp64.unk3C = (f32) mathRnd(0x1E, 0x32) * 0.0002f;
        sp64.unk8 = 0x96;
        sp64.unk60 = 0x9B;
        sp64.unk48 = 0x20;
        sp64.unk44 = 0x180210;
        sp64.unk58[0] = mathRnd(0, 0x7530) + 0x63BF;
        sp64.unk58[1] = (u16) ((s32) sp64.unk58[0] / mathRnd(1, 3));
        sp64.unk58[2] = 0;
        sp64.unk4C[0] = mathRnd(0, 0x2710);
        sp64.unk4C[1] = sp64.unk4C[0] / mathRnd(1, 3);
        sp64.unk4C[2] = 0;
        sp64.unk42 = 0x60;
        break;
    case 0x206:
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
        sp64.unk30.y = transform->transl.y - 12.0f;
        sp64.unk24.y = 0.005f;
        switch (mathRnd(0, 3)) {
        case 0:
            sp64.unk30.x = transform->transl.x;
            sp64.unk30.z = (f32) mathRnd((s32) (s16) (s32) -transform->transl.z, (s32) (s16) (s32) transform->transl.z);
            break;
        case 1:
            sp64.unk30.x = -transform->transl.x;
            sp64.unk30.z = (f32) mathRnd((s32) (s16) (s32) -transform->transl.z, (s32) (s16) (s32) transform->transl.z);
            break;
        case 2:
            sp64.unk30.z = transform->transl.z;
            sp64.unk30.x = (f32) mathRnd((s32) (s16) (s32) -transform->transl.x, (s32) (s16) (s32) transform->transl.x);
            break;
        case 3:
            sp64.unk30.z = -transform->transl.z;
            sp64.unk30.x = (f32) mathRnd((s32) (s16) (s32) -transform->transl.x, (s32) (s16) (s32) transform->transl.x);
            break;
        }
        sp64.unk24.y = (f32) mathRnd(0x50, 0x64) * 0.0025f;
        sp64.unk3C = (f32) mathRnd(0x1E, 0x32) * 0.0004f;
        sp64.unk8 = 0x96;
        sp64.unk60 = 0xFF;
        sp64.unk44 = 0x80080110;
        sp64.unk42 = 0x60;
        break;
    case 0x208:
        sp64.unk30.x = (f32) mathRnd(-0xBB8, 0xBB8) * 0.1f;
        sp64.unk30.y = 200.0f;
        sp64.unk30.z = (f32) mathRnd(-0xBB8, 0xBB8) * 0.1f;
        sp64.unk24.y = (f32) mathRnd(0x190, 0x258) * -0.003f;
        sp64.unk24.x = (f32) mathRnd(-0x64, 0x64) * 0.0003f;
        sp64.unk24.z = (f32) mathRnd(-0x64, 0x64) * 0.0003f;
        sp64.unk3C = ((f32) mathRnd(0, 0xA) * 0.0001f) + 0.0035f;
        sp64.unk8 = 0xB4;
        sp64.unk60 = 0xFF;
        sp64.unk44 = 0x80080000;
        sp64.unk48 = 0x100000;
        sp64.unk42 = 0xE7;
        break;
    case 0x209:
        sp64.unk30.y = (f32) mathRnd(1, 5);
        sp64.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.04f;
        sp64.unk3C = 2.0f * (((f32) mathRnd(0, 0xA) * 0.0002f) + 0.0004245f);
        sp64.unk8 = mathRnd(0x73, 0x8C);
        sp64.unk60 = 0xFF;
        sp64.unk44 = 0x80480200;
        sp64.unk42 = 0x47;
        break;
    case 0x20A:
        if (transform == NULL) {
            bss_0.transl.x = 0.0f;
            bss_0.transl.y = 0.0f;
            bss_0.transl.z = 0.0f;
            bss_0.yaw = 0;
            bss_0.pitch = 0;
            bss_0.roll = 0;
            bss_0.scale = 1.0f;
        }
        sp64.unk30.x = (f32) mathRnd(-5, 5);
        sp64.unk30.y = (f32) mathRnd(1, 5);
        sp64.unk30.z = (f32) mathRnd(-5, 5);
        sp50 = ((f32) mathRnd(0, 0x258) * 0.003f) + 2.3f;
        sp64.unk24.y = ((f32) mathRnd(0, 0xC8) * 0.001f) + 1.0f;
        sp64.unk24.x = mathSinfInterp(obj->srt.yaw);
        sp64.unk24.z = mathCosfInterp(obj->srt.yaw);
        temp_fv0_17 = ((f32) mathRnd(0, 0x14) * 0.015f * sp50) + 0.1f;
        sp64.unk24.x *= temp_fv0_17;
        sp64.unk24.z *= temp_fv0_17;
        sp64.unk24.y *= sp50;
        sp64.unk3C = ((f32) mathRnd(0, 0xA) * 0.00006f) + 0.0048f;
        sp64.unk8 = mathRnd(0xB4, 0xC8);
        sp64.unk60 = 0xFF;
        sp64.unk44 = 0x03000120;
        sp64.unk48 = 0x200000;
        sp64.unk42 = 0x8F;
        sp64.unk4 = 0x20B;
        break;
    case 0x20B:
        sp64.unk24.y = (f32) mathRnd(2, 0x14) * 0.002f;
        sp64.unk8 = 0x1E;
        sp64.unk60 = 0x9B;
        sp64.unk44 = 0x180100;
        sp64.unk42 = 0x5F;
        sp64.unk3C = 0.008445f;
        sp64.unk58[0] = 0xFFFF;
        sp64.unk58[1] = (mathRnd(0, 0xC350) + 0x3CAF);
        sp64.unk58[2] = 0;
        sp64.unk4C[0] = sp64.unk58[0];
        sp64.unk4C[1] = sp64.unk58[1];
        sp64.unk4C[2] = 0;
        sp64.unk48 = 0x20;
        break;
    case 0x20C:
        sp64.unk30.x = (f32) mathRnd(-0x37, 0x37);
        sp64.unk30.y = (f32) mathRnd(0xA, 0xF);
        sp64.unk30.z = (f32) mathRnd(-0x37, 0x37);
        sp64.unk24.x = (f32) mathRnd(-8, 8) * 0.01f;
        sp64.unk24.y = (f32) mathRnd(0xA, 0x14) * 0.1f;
        sp64.unk24.z = (f32) mathRnd(-8, 8) * 0.01f;
        sp64.unk3C = ((f32) mathRnd(0, 0xA) * 0.0002f) + 0.0028945f;
        sp64.unk8 = mathRnd(0x78, 0x8C);
        sp64.unk60 = 0xFF;
        sp64.unk4 = 0x20B;
        sp64.unk48 = 0x200000;
        sp64.unk44 = 0x01001100;
        sp64.unk42 = 0x8F;
        break;
    case 0x20D:
        sp64.unk24.x = (f32) mathRnd(-0x32, 0x32) * 0.007f;
        sp64.unk24.y = (f32) mathRnd(-0xA, 0xA) * 0.0017f;
        sp64.unk24.z = (f32) mathRnd(-0x32, 0x32) * 0.007f;
        sp64.unk30.y = (f32) mathRnd(0, 0x190) * 0.1f;
        sp64.unk3C = (f32) mathRnd(0xF, 0x19) * 0.0003f;
        sp64.unk8 = 0x64;
        sp64.unk44 = 0x4A0104;
        sp64.unk48 = 0x40008;
        sp64.transform.yaw = 0x46;
        sp64.transform.pitch = 0;
        sp64.transform.roll = 0;
        sp64.unk42 = 0xE0;
        sp64.transform.transl.x = 0.0f;
        sp64.transform.transl.y = 0.0f;
        sp64.transform.transl.z = 0.0f;
        sp64.transform.scale = 1.0f;
        break;
    default:
        // STUBBED_PRINTF("PARTFX dim type < %d > not found.. returning\n", id); // default.dol
        return -1;
    }
    sp64.unk44 |= flags;
    if ((sp64.unk44 & 1) && (sp64.unk44 & 2)) {
        // STUBBED_PRINTF("error in partfx ABS and REL %d\n", id); // default.dol
        sp64.unk44 ^= 2;
    }
    if (sp64.unk44 & 1) {
        if (flags & 0x200000) {
            sp64.unk30.x += sp64.transform.transl.x;
            sp64.unk30.y += sp64.transform.transl.y;
            sp64.unk30.z += sp64.transform.transl.z;
        } else if (sp64.unk0 != NULL) {
            sp64.unk30.x += sp64.unk0->globalPosition.x;
            sp64.unk30.y += sp64.unk0->globalPosition.y;
            sp64.unk30.z += sp64.unk0->globalPosition.z;
        }
    }
    return gDLL_13_Expgfx->vtbl->func1(&sp64, -1);
}
