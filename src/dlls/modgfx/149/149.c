#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "sys/rand.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    30, 0, 0, 0, 0, -30, 0, 0, 15, 0, -30, 1000, 0, 15, 15, 30,
    1000, 0, 0, 15
};
/*0x28*/ static s16 data_28[] = { 0, 1, 2, 0, 2, 3, 0, 0 };
/*0x38*/ static s16 data_38[] = { 0, 1 };
/*0x3C*/ static s16 data_3C[] = { 0, 1, 2, 3 };
/*0x44*/ static s16 data_44[] = { 2, 3 };
/*0x48*/ static s16 data_48[] = { 0, 10, 15, 80, 0, 0, 0, 0 };
/*0x58*/ static s16 data_58[] = {
    TEXTABLE_527, TEXTABLE_528, TEXTABLE_DF_StarBlue, TEXTABLE_DE, 
    TEXTABLE_DF_StarBlue, TEXTABLE_200, TEXTABLE_1FB, TEXTABLE_1FB, 
    TEXTABLE_DF_StarBlue, TEXTABLE_DE, 0, 0
};

// offset: 0x0 | ctor
void dll_149_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_149_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_149_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    s32 pad[1];
    s32 i;
    s32 var_v0;
    s32 var_fv0;

    temp = sp50;
    if (type == 4) {
        temp->unk16 = 0;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x400000;
        temp->unk4.f[0] = 10.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 0;
        temp->unk14 = 2;
        temp->unk10 = data_44;
        temp->unk0 = 2;
        temp->unk4.f[0] = 9.0f;
        temp->unk4.f[1] = 2.0f;
        temp->unk4.f[2] = 9.0f;
        temp++;

        temp->unk16 = 0;
        temp->unk14 = 4;
        temp->unk10 = data_44;
        temp->unk0 = 0x80;
        temp->unk4.f[0] = mathRnd(-32760, 32760);
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 16383.0f;
        temp++;
    } else {
        temp->unk16 = 0;
        temp->unk14 = 2;
        temp->unk10 = data_38;
        temp->unk0 = 2;
        temp->unk4.f[0] = obj->srt.scale * 190.0f;
        temp->unk4.f[1] = obj->srt.scale * 6.0f;
        temp->unk4.f[2] = 1.0f;
        temp++;

        temp->unk16 = 0;
        temp->unk14 = 2;
        temp->unk10 = data_44;
        temp->unk0 = 2;
        temp->unk4.f[0] = (obj->srt.scale / obj->def->scale) * 40.0f;
        temp->unk4.f[1] = (obj->srt.scale / obj->def->scale) * 6.0f;
        temp->unk4.f[2] = 1.0f;
        temp++;

        var_fv0 = mathRnd(0, 65534);
        temp->unk16 = 0;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x80;
        temp->unk4.f[0] = var_fv0;
        temp->unk4.f[1] = 1000.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }

    temp->unk16 = 0;
    temp->unk14 = 4;
    temp->unk10 = data_3C;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    var_v0 = mathRnd(0, 0xFFFE);
    temp->unk16 = 1;
    temp->unk14 = 2;
    temp->unk10 = data_38;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    if (type == 4) {
        temp->unk16 = 2;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 100.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else {
        temp->unk16 = 1;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x80;
        temp->unk4.f[0] = var_v0;
        temp->unk4.f[1] = 1000.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }

    var_v0 = mathRnd(0, 0xFFFE);
    if (type == 4) {
        temp->unk16 = 2;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 100.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else {
        temp->unk16 = 2;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x80;
        temp->unk4.f[0] = var_v0;
        temp->unk4.f[1] = 1000.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }

    if (type == 4) {
        temp->unk16 = 3;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 100.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else {
        temp->unk16 = 3;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x80;
        temp->unk4.f[0] = var_v0;
        temp->unk4.f[1] = 1000.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }

    temp->unk16 = 3;
    temp->unk14 = 2;
    temp->unk10 = data_38;
    temp->unk0 = 4;
    temp->unk4.f[0] = 100.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 4;
    temp->unk10 = data_3C;
    temp->unk0 = 2;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 0.1f;
    temp->unk4.f[2] = 1.0f;
    temp++;

    sp350.unk4 = obj;
    sp350.unk58 = 0;
    sp350.unk44 = type;
    sp350.unk2C.f[0] = 0.0f;
    sp350.unk2C.f[1] = 0.0f;
    sp350.unk2C.f[2] = 0.0f;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    sp350.unk40 = 2;
    sp350.unk3C = 0;
    sp350.unk38 = 1.0f;
    sp350.unk0 = sp50;
    sp350.unk59 = 4;
    sp350.unk5A = 0;
    sp350.unk5B = 0x20;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_48[i]; }
    sp350.unk0 = sp50;
    
    if (type == 4) {
        var_v0 = 0x04004400;
    } else {
        var_v0 = 0x04006410;
    }
    var_v0 |= flags;
    sp350.unk54 = var_v0;
    if (sp350.unk54 & 1) {
        if ((sp350.unk4 != NULL) && (transform != NULL)) {
            sp350.unk2C.f[0] += sp350.unk4->globalPosition.f[0] + transform->transl.f[0];
            sp350.unk2C.f[1] += sp350.unk4->globalPosition.f[1] + transform->transl.f[1];
            sp350.unk2C.f[2] += sp350.unk4->globalPosition.f[2] + transform->transl.f[2];
        } else if (sp350.unk4 != NULL) {
            sp350.unk2C.f[0] += sp350.unk4->globalPosition.f[0];
            sp350.unk2C.f[1] += sp350.unk4->globalPosition.f[1];
            sp350.unk2C.f[2] += sp350.unk4->globalPosition.f[2];
        } else if (transform != NULL) {
            sp350.unk2C.f[0] += transform->transl.f[0];
            sp350.unk2C.f[1] += transform->transl.f[1];
            sp350.unk2C.f[2] += transform->transl.f[2];
        }
    }

    return dll_modgfx->Func1(&sp350, 0, 4, data_0, 2, data_28, data_58[mathRnd(0, 1) + type * 2], NULL);
}
