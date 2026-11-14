#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    -500, -500, -500, 0, 0, 500, -500, -500, 32, 32, 500, -500, 500, 0, 0, -500,
    -500, 500, 32, 32, -500, 500, -500, 0, 0, 500, 500, -500, 32, 32, 500, 500,
    500, 0, 0, -500, 500, 500, 32, 32
};
/*0x50*/ static s16 data_50[] = {
    0, 4, 5, 0, 5, 1, 1, 5, 6, 1, 6, 2, 2, 6, 7, 2,
    7, 3, 3, 7, 4, 3, 4, 0, 0, 1, 2, 0, 2, 3, 4, 7,
    6, 4, 6, 5
};
/*0x98*/ static s16 data_98[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
/*0xA8*/ static s16 data_A8[] = { 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_154_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_154_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_154_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    temp->unk14 = 8;
    temp->unk10 = data_98;
    temp->unk0 = 2;
    temp->unk16 = 0;
    temp->unk4.f[0] = 0.5f;
    temp->unk4.f[1] = 0.5f;
    temp->unk4.f[2] = 0.5f;
    temp++;

    sp348.unk58 = 0;
    sp348.unk44 = type;
    sp348.unk4 = obj;
    sp348.unk40 = 1;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk38 = 1.0f;
    sp348.unk3C = 0;
    sp348.unk59 = 8;
    sp348.unk5A = 0;
    sp348.unk5B = 0x10;
    sp348.unk54 = 0x02000492;
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_A8[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 |= flags;
    if (sp348.unk54 & 1) {
        if (sp348.unk4 != NULL) {
            sp348.unk2C.f[0] += sp348.unk4->globalPosition.f[0];
            sp348.unk2C.f[1] += sp348.unk4->globalPosition.f[1];
            sp348.unk2C.f[2] += sp348.unk4->globalPosition.f[2];
        } else {
            sp348.unk2C.f[0] += transform->transl.f[0];
            sp348.unk2C.f[1] += transform->transl.f[1];
            sp348.unk2C.f[2] += transform->transl.f[2];
        }
    }

    return dll_modgfx->Func1(&sp348, 0, 8, data_0, 12, data_50, TEXTABLE_1FD, NULL);
}
