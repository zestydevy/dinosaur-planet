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
void bwcPartfx_ctor(void* dll) { }

// offset: 0xC | dtor
void bwcPartfx_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void bwcPartfx_Func_18(void) {

}

// offset: 0x20 | func: 1 | export: 2
void bwcPartfx_Func_20(s32 arg0) {
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
s32 bwcPartfx_Spawn(Object* obj, s32 id, SRT* transform, s32 flags, s8 arg4, void* data) {
    ExpgfxStruct sp34;
    s32 _pad;
    s32 _pad2;
    s32 _pad3;

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
    sp34.unk40 = 0;
    switch (id) {
        case 0x76C:
            sp34.unk24.x = (f32) mathRnd(0x1E, 0x64) * 0.002f;
            if (transform->transl.x > 0) {
                sp34.unk24.x = -sp34.unk24.x;
            }
            sp34.unk24.y = ((f32) mathRnd(0, 0x64) * 0.001f) + 0.1f;
            sp34.unk30.z = (f32) mathRnd((s32) ((f32*)data)[0], (s32) ((f32*)data)[1]) * 0.1f;
            sp34.unk30.x = 5.0f;
            if (transform->transl.x > 0) {
                sp34.unk30.x = -5.0f;
            }
            sp34.unk3C = ((f32*)data)[2] + ((f32) mathRnd(-0x64, 0x64) * 0.00003f);
            sp34.unk8 = 0x23;
            sp34.unk44 = 0x80108;
            sp34.unk42 = 0x60;
            sp34.unk60 = 0xC4;
            break;
        default:
            // STUBBED_PRINTF("PARTFX wcning type < %d > not found.. returning\n", id); // default.dol
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
