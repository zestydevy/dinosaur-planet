#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"
#include "sys/rand.h"

/*0x0*/ static s16 data_0[] = { 0, 155, 200, 155, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_133_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_133_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_133_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp358;
    ModgfxStruct_0 sp58[32];
    ModgfxStruct_0* temp;
    s32 pad[2];
    s32 var_v1;
    s32 i;
    f32 var_fa0;
    f32 var_fv1;

    var_fv1 = 81.0f;
    var_fa0 = 82.0f;
    var_v1 = 0x64;
    if (type == 0) {
        var_v1 = 0x8C;
    } else if (type == 1) {
        var_fv1 = 100.0f;
        var_fa0 = 101.0f;
        var_v1 = 0x8C;
    } else if (type == 2) {
        var_fv1 = 102.0f;
        var_fa0 = 103.0f;
        var_v1 = 0x8C;
    } else if (type == 3) {
        var_fv1 = 104.0f;
        var_fa0 = 105.0f;
        var_v1 = 0x8C;
    } else if (type == 4) {
        var_fv1 = 106.0f;
        var_fa0 = 107.0f;
        var_v1 = 0x154;
    } else if (type == 5) {
        var_fv1 = 111.0f;
        var_fa0 = 112.0f;
        var_v1 = 0x280;
        data_0[2] = 800;
    } else if (type == 6) {
        var_fv1 = 125.0f;
        var_fa0 = 126.0f;
        data_0[2] = 20;
    } else if (type == 7) {
        var_fv1 = 133.0f;
        var_fa0 = 134.0f;
        var_v1 = 0xC8;
        data_0[1] = 20;
        data_0[2] = 20;
        data_0[3] = 20;
    } else if (type == 8) {
        var_fv1 = 157.0f;
        var_fa0 = 158.0f;
        var_v1 = 0x41;
        data_0[1] = 20;
        data_0[2] = 20;
        data_0[3] = 20;
    }
    temp = sp58;
    temp->unk16 = 0;
    temp->unk14 = var_v1;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000000;
    temp->unk4.f[0] = 999.0f;
    temp->unk4.f[1] = var_fv1;
    temp->unk4.f[2] = var_fa0;
    temp++;

    if (type == 0) {
        temp->unk16 = 0;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x80000;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 200.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 1;
        temp->unk14 = 0;
        temp->unk10 = 0;
        temp->unk0 = 0x80000;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
        
        temp->unk16 = 3;
        temp->unk14 = 0;
        temp->unk10 = 0;
        temp->unk0 = 0x80000;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 200.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else if (type == 6) {
        temp->unk16 = 3;
        temp->unk14 = 1;
        temp->unk10 = NULL;
        temp->unk0 = 0x2000;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else if (type == 8) {
        temp->unk16 = 3;
        temp->unk14 = 1;
        temp->unk10 = NULL;
        temp->unk0 = 0x2000;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }

    temp->unk16 = 4;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000000;
    temp->unk4.f[0] = 999.0f;
    temp->unk4.f[1] = var_fv1;
    temp->unk4.f[2] = var_fa0;
    temp++;

    sp358.unk44 = type;
    sp358.unk4 = obj;
    sp358.unk58 = 0;
    sp358.unk40 = 0;
    sp358.unk3C = 0;
    sp358.unk59 = 0;
    sp358.unk5A = 0;
    sp358.unk5B = 0;
    sp358.unk38 = 1.0f;
    sp358.unk2C.f[0] = 0.0f;
    sp358.unk2C.f[1] = 0.0f;
    sp358.unk2C.f[2] = 0.0f;
    sp358.unk20 = 0.0f;
    sp358.unk24 = 0.0f;
    sp358.unk28 = 0.0f;
    sp358.unk5D = temp - sp58;
    for (i = 0; i < 7; i++) { sp358.unk46[i] = data_0[i]; }
    sp358.unk0 = sp58;
    sp358.unk54 = flags | 0x10800;
    if (sp358.unk54 & 1) {
        if (sp358.unk4 != NULL) {
            sp358.unk2C.f[0] += sp358.unk4->globalPosition.f[0];
            sp358.unk2C.f[1] += sp358.unk4->globalPosition.f[1];
            sp358.unk2C.f[2] += sp358.unk4->globalPosition.f[2];
        } else {
            sp358.unk2C.f[0] += transform->transl.f[0];
            sp358.unk2C.f[1] += transform->transl.f[1];
            sp358.unk2C.f[2] += transform->transl.f[2];
        }
    }

    return dll_modgfx->Func1(&sp358, 0, 0, NULL, 0, NULL, 0, NULL);
}
