#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/rand.h"
#include "dll.h"

/*0x0*/ static u32 data_0 = 0;
/*0x4*/ static s16 data_4[] = {
    0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// offset: 0x0 | ctor
void dll_150_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_150_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_150_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp360;
    ModgfxStruct_0 sp60[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;
    f32 sp48[2];
    f32 sp44;
    f32 sp40;
    s16 var_v0;

    sp44 = 81.0f;
    sp40 = 82.0f;
    var_v0 = 0x64;
    if (type == 0) {
        sp44 = 18.0f;
        sp40 = 8.0f;
        var_v0 = 0x410;
    } else if (type == 1) {
        sp44 = 19.0f;
        sp40 = 9.0f;
        var_v0 = 0x410;
    } else if (type == 2) {
        sp44 = 20.0f;
        sp40 = 15.0f;
        var_v0 = 0x410;
    } else if (type == 3) {
        sp44 = 20.0f;
        sp40 = 15.0f;
        var_v0 = 0x410;
    }

    temp = sp60;
    temp->unk16 = 0;
    temp->unk14 = var_v0;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000000;
    temp->unk4.f[0] = 999.0f;
    temp->unk4.f[1] = sp44;
    temp->unk4.f[2] = sp40;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = mathRnd(-100, 100);
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = mathRnd(-1200, -800);
    sp48[0] = temp->unk4.f[0];
    sp48[1] = temp->unk4.f[1];
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x40000000;
    temp->unk4.f[0] = sp48[0];
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = sp48[1];
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x65;
    temp->unk10 = NULL;
    temp->unk0 = 0x800000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000000;
    temp->unk4.f[0] = 999.0f;
    temp->unk4.f[1] = sp44;
    temp->unk4.f[2] = sp40;
    temp++;

    sp360.unk4 = obj;
    sp360.unk44 = type;
    sp360.unk58 = 0;
    sp360.unk2C.f[0] = mathRnd(-100, 100);
    sp360.unk2C.f[1] = 0.0f;
    sp360.unk2C.f[2] = 0.0f;
    sp360.unk40 = 0;
    sp360.unk3C = 0;
    sp360.unk59 = 0;
    sp360.unk5A = 0;
    sp360.unk5B = 0;
    sp360.unk38 = 1.0f;
    sp360.unk20 = 0.0f;
    sp360.unk24 = 0.0f;
    sp360.unk28 = 0.0f;
    
    sp360.unk5D = temp - sp60;
    for (i = 0; i < 7; i++) { sp360.unk46[i] = data_4[i]; }
    sp360.unk0 = sp60;
    sp360.unk54 = flags | 0x10400;
    if (sp360.unk54 & 1) {
        if (sp360.unk4 != NULL) {
            sp360.unk2C.f[0] += sp360.unk4->globalPosition.f[0];
            sp360.unk2C.f[1] += sp360.unk4->globalPosition.f[1];
            sp360.unk2C.f[2] += sp360.unk4->globalPosition.f[2];
        } else {
            sp360.unk2C.f[0] += transform->transl.f[0];
            sp360.unk2C.f[1] += transform->transl.f[1];
            sp360.unk2C.f[2] += transform->transl.f[2];
        }
    }

    return dll_modgfx->Func1(&sp360, 0, 0, NULL, 0, NULL, 0, NULL);
}
