#include "common.h"
#include "sys/gfx/modgfx.h"

/*0x0*/ static s16 data_0[] = {
    0, 0, 0, 15, 0,
    150, 400, 900, 0, 127,
    -50, 400, 1000, 31, 127,
    50, 530, -1000, 0, 127,
    0xff6a, 530, -850, 31, 127,
    1000, 100, 150, 0, 127,
    1200, 100, -50, 31, 127,
    -1000, 270, 50, 0, 127,
    -1000, 270, -50, 31, 127,
    620, 550, 780, 0, 127,
    780, 550, 920, 31, 127,
    -820, 210, 780, 0, 127,
    -580, 210, 820, 31, 127,
    820, 100, -780, 0, 127,
    780, 100, -620, 31, 127,
    -920, 470, -780, 0, 127,
    -780, 470, -820, 31, 127
};
/*0xAC*/ static s16 data_AC[] = { 0, 1, 2, 0, 3, 4, 0, 5, 6, 0, 7, 8, 0, 9, 10, 0, 11, 12, 0, 13, 14, 0, 15, 16 };
/*0xDC*/ static s16 data_DC[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0 };
/*0x100*/ static s16 data_100[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, 0 };
/*0x124*/ static s16 data_124[] = { 0, 90, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_105_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_105_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_105_modgfx_Func0(Object* arg0, s32 arg1, SRT* arg2, u32 arg3, s32 arg4, void* arg5) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    temp->unk16 = 1;
    temp->unk14 = 0x11;
    temp->unk10 = data_DC;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -3.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x10;
    temp->unk10 = data_100;
    temp->unk0 = 2;
    temp->unk4.f[0] = 35.0f;
    temp->unk4.f[1] = 35.0f;
    temp->unk4.f[2] = 35.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x11;
    temp->unk10 = data_DC;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 1500.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 2;
    temp->unk10 = NULL;
    temp->unk0 = 0x04000000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 2;
    temp->unk10 = NULL;
    temp->unk0 = 0x04000000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x11;
    temp->unk10 = data_DC;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -3.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x11;
    temp->unk10 = data_DC;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x11;
    temp->unk10 = data_DC;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = -1000.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x10;
    temp->unk10 = data_100;
    temp->unk0 = 2;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 2.0f;
    temp->unk4.f[2] = 2.0f;
    temp++;

    sp348.unk40 = 1;
    sp348.unk58 = 0;
    sp348.unk59 = 0x11;
    sp348.unk5B = 0x10;
    sp348.unk44 = arg1;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 135.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk5D = temp - sp48;
    sp348.unk4 = arg0;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk38 = 1.0f;
    sp348.unk3C = 0;
    sp348.unk5A = 0;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_124[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = arg3 | 0x04000000;
    if (sp348.unk54 & 1) {
        if (sp348.unk4 != NULL) {
            sp348.unk2C.f[0] += sp348.unk4->globalPosition.f[0];
            sp348.unk2C.f[1] += sp348.unk4->globalPosition.f[1];
            sp348.unk2C.f[2] += sp348.unk4->globalPosition.f[2];
        } else {
            sp348.unk2C.f[0] += arg2->transl.f[0];
            sp348.unk2C.f[1] += arg2->transl.f[1];
            sp348.unk2C.f[2] += arg2->transl.f[2];
        }
    }
    return dll_modgfx->Func1(&sp348, 0, 0x11, data_0, 8, data_AC, 0x47, NULL);
}
