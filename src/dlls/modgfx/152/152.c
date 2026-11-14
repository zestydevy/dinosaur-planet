#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    -500, 500, 0, 0, 31, -250, 500, -80, 7, 31, 0, 500, -120, 16, 31, 250,
    500, -80, 24, 31, 500, 500, 0, 31, 31, -500, 250, -80, 0, 24, -250, 250,
    -160, 7, 24, 0, 250, -200, 16, 24, 250, 250, -160, 24, 24, 500, 250, -80,
    31, 24, -500, 0, -120, 0, 16, -250, 0, -200, 7, 16, 0, 0, -240, 16,
    16, 250, 0, -200, 24, 16, 500, 0, -120, 31, 16, -500, -250, -80, 0, 7,
    -250, -250, -160, 7, 7, 0, -250, -200, 16, 7, 250, -250, -160, 24, 7, 500,
    -250, -80, 31, 7, -500, -500, 0, 0, 0, -250, -500, -80, 7, 0, 0, -500,
    -120, 16, 0, 250, -500, -80, 24, 0, 500, -500, 0, 31, 0, 0
};
/*0xFC*/ static s16 data_FC[] = {
    5, 1, 0, 5, 6, 1, 6, 2, 1, 6, 7, 2, 7, 3, 2, 7,
    8, 3, 8, 4, 3, 8, 9, 4, 10, 6, 5, 10, 11, 6, 11, 7,
    6, 11, 12, 7, 12, 8, 7, 12, 13, 8, 13, 9, 8, 13, 14, 9,
    15, 11, 10, 15, 16, 11, 16, 12, 11, 16, 17, 12, 17, 13, 12, 17,
    18, 13, 18, 14, 13, 18, 19, 14, 20, 16, 15, 20, 21, 16, 21, 17,
    16, 21, 22, 17, 22, 18, 17, 22, 23, 18, 23, 19, 18, 23, 24, 19
};
/*0x1BC*/ static s16 data_1BC[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 0
};
/*0x1F0*/ static s16 data_1F0[] = { 0, 30, 30, 80, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_152_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_152_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_152_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;
    
    temp = sp48;
    temp->unk16 = 0;
    temp->unk14 = 0x19;
    temp->unk10 = data_1BC;
    temp->unk0 = 2;
    temp->unk4.f[0] = 20.7f;
    temp->unk4.f[1] = 20.7f;
    temp->unk4.f[2] = 20.7f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x19;
    temp->unk10 = data_1BC;
    temp->unk0 = 0x80;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x353;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x19;
    temp->unk10 = data_1BC;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x19;
    temp->unk10 = data_1BC;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x19;
    temp->unk10 = data_1BC;
    temp->unk0 = 2;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 2.0f;
    temp->unk4.f[2] = 1.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x19;
    temp->unk10 = data_1BC;
    temp->unk0 = 2;
    temp->unk4.f[0] = 1.5f;
    temp->unk4.f[1] = 1.5f;
    temp->unk4.f[2] = 1.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x19;
    temp->unk10 = data_1BC;
    temp->unk0 = 2;
    temp->unk4.f[0] = 1.5f;
    temp->unk4.f[1] = 1.5f;
    temp->unk4.f[2] = 1.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x19;
    temp->unk10 = data_1BC;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
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
    sp348.unk3C = 0x19;
    sp348.unk59 = 0x19;
    sp348.unk5A = 0xFF;
    sp348.unk5B = 0x10;
    sp348.unk5D = temp - sp48;
    sp348.unk54 = 0x04000480;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_1F0[i]; }
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

    return dll_modgfx->Func1(&sp348, 0, 0x19, data_0, 0x20, data_FC, TEXTABLE_205, NULL);
}
