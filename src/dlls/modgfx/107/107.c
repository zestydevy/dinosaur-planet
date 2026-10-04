#include "common.h"

/*0x0*/ static s16 data_0[] = {
    0x0000, 0x0258, 0x0000, 0x000f, 0x001f, 0x0258, 0x0000, 0x0000, 0x0000, 0x0000, 0xfda8, 0x0000, 0x0258, 0x000f, 0x0000, 0xfda8, 
    0x0000, 0xfda8, 0x001f, 0x0000
};
/*0x28*/ static s16 data_28[] = {
    0x0000, 0x0001, 0x0002, 0x0000, 0x0002, 0x0003, 0x0000, 0x0003, 0x0001, 0x0001, 0x0003, 0x0002
};
/*0x40*/ static s16 data_40[] = {
    0x0000, 0x0000, 0x0001, 0x0002, 0x0003, 0x0000
};
/*0x4C*/ static s16 data_4C[] = {
    0x0000, 0x0001, 0x0002, 0x0003
};
/*0x54*/ static s16 data_54[] = { 0x0000, 0x0046, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

// offset: 0x0 | ctor
void dll_107_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_107_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_107_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, s16* data) {
    ModgfxStruct sp3B8;
    ModgfxStruct_0 spB8[32];
    ModgfxStruct_0* var_s0;
    s32 var_s1;
    s16 spAE = 0;
    u8 padA0[0xAE - 0xA4];
    ModelInstance* modelInstance = obj->modelInsts[obj->modelInstIdx];
    Texture* sp9C;
    Model* sp98;
    SRT sp80;
    u32 pad7C;
    s16 sp78[2] = { 5, 20 }; // data_64;
    s32 i;

    if (data != NULL) {
        sp78[0] = data[0];
        sp78[1] = data[1];
    }

    if (obj == NULL) {
        STUBBED_PRINTF("!!!! This modgfx needs an owner object\n");
        return -1;
    }

    sp80.roll = 0;
    sp80.transl.f[0] = 0.0f;
    sp80.transl.f[1] = 0.0f;
    sp80.transl.f[2] = 0.0f;
    sp80.scale = 1.0f;
    sp98 = modelInstance->model;
    if (sp98->textureCount == 0) {
        return -1;
    }

    sp3B8.unk40 = 1;
    sp3B8.unk58 = type;
    sp3B8.unk4 = obj;
    sp3B8.unk44 = type;
    sp3B8.unk2C.f[0] = 0.0f;
    sp3B8.unk2C.f[1] = 0.0f;
    sp3B8.unk2C.f[2] = 0.0f;
    sp3B8.unk20 = 0.0f;
    sp3B8.unk24 = 0.0f;
    sp3B8.unk28 = 0.0f;
    sp3B8.unk38 = 1.0f;
    sp3B8.unk3C = 0;
    sp3B8.unk59 = 4;
    sp3B8.unk5A = 0;
    sp3B8.unk5B = 0;
    for (i = 0; i < 7; i++) {
        sp3B8.unk46[i] = data_54[i];
    }

    var_s1 = mathRnd(sp78[0], sp78[1]);
    if (type == 0xC) {
        var_s1 = mathRnd(2, 6);
    } else if (type == 0xD) {
        var_s1 = mathRnd(2, 6);
    } else if (type == 0x11) {
        var_s1 = 5;
    }

    while (var_s1 != 0) {
        sp9C = sp98->materials[mathRnd(0, sp98->textureCount - 1)].texture;
        var_s0 = spB8;
        var_s0->unk16 = 0;
        var_s0->unk14 = 1;
        var_s0->unk10 = data_40;
        var_s0->unk0 = 8;
        var_s0->unk4.f[0] = 0.0f;
        var_s0->unk4.f[1] = 0.0f;
        var_s0->unk4.f[2] = 0.0f;
        var_s0++;
        if (type == 0xC || type == 5) {
            var_s0->unk16 = 0;
            var_s0->unk14 = 4;
            var_s0->unk10 = data_4C;
            var_s0->unk0 = 2;
            var_s0->unk4.f[0] = mathRnd(1, 6) * 0.15f;
            var_s0->unk4.f[1] = mathRnd(1, 6) * 0.15f;
            var_s0->unk4.f[2] = mathRnd(1, 6) * 0.15f;
            var_s0++;
        } else if (type == 0xD) {
            var_s0->unk16 = 0;
            var_s0->unk14 = 4;
            var_s0->unk10 = data_4C;
            var_s0->unk0 = 2;
            var_s0->unk4.f[0] = mathRnd(1, 6) * 0.15f;
            var_s0->unk4.f[1] = mathRnd(1, 6) * 0.15f;
            var_s0->unk4.f[2] = mathRnd(1, 6) * 0.15f;
            var_s0++;
        } else if (type == 0x14) {
            var_s0->unk16 = 0;
            var_s0->unk14 = 4;
            var_s0->unk10 = data_4C;
            var_s0->unk0 = 2;
            var_s0->unk4.f[0] = mathRnd(3, 6) * 0.25f;
            var_s0->unk4.f[1] = mathRnd(3, 6) * 0.25f;
            var_s0->unk4.f[2] = mathRnd(3, 6) * 0.25f;
            var_s0++;
        } else if (type == 0x11) {
            var_s0->unk16 = 0;
            var_s0->unk14 = 4;
            var_s0->unk10 = data_4C;
            var_s0->unk0 = 2;
            var_s0->unk4.f[0] = mathRnd(3, 6) * 0.25f;
            var_s0->unk4.f[1] = mathRnd(3, 6) * 0.25f;
            var_s0->unk4.f[2] = mathRnd(3, 6) * 0.25f;
            var_s0++;
        } else if (type == 0x10) {
            var_s0->unk16 = 0;
            var_s0->unk14 = 4;
            var_s0->unk10 = data_4C;
            var_s0->unk0 = 8;
            var_s0->unk4.f[1] = 0.0f;
            var_s0->unk4.f[0] = 255.0f;
            var_s0->unk4.f[2] = 255.0f;
            var_s0++;
            var_s0->unk16 = 0;
            var_s0->unk14 = 4;
            var_s0->unk10 = data_4C;
            var_s0->unk0 = 2;
            var_s0->unk4.f[0] = mathRnd(3, 6) * 2.5f;
            var_s0->unk4.f[1] = mathRnd(3, 6) * 2.5f;
            var_s0->unk4.f[2] = mathRnd(3, 6) * 2.5f;
            var_s0++;
        } else {
            var_s0->unk16 = 0;
            var_s0->unk14 = 4;
            var_s0->unk10 = data_4C;
            var_s0->unk0 = 2;
            var_s0->unk4.f[0] = mathRnd(1, 6) * 0.15f;
            var_s0->unk4.f[1] = mathRnd(1, 6) * 0.15f;
            var_s0->unk4.f[2] = mathRnd(1, 6) * 0.15f;
            var_s0++;
        }

        var_s0->unk16 = 1;
        var_s0->unk14 = 0;
        var_s0->unk10 = NULL;
        var_s0->unk0 = 0x80000000;
        var_s0->unk4.f[0] = 0.0f;
        var_s0->unk4.f[1] = -0.07f;
        var_s0->unk4.f[2] = 0.0f;
        var_s0++;
        var_s0->unk16 = 1;
        var_s0->unk14 = 0;
        var_s0->unk10 = 0;
        var_s0->unk0 = 0x100;
        var_s0->unk4.f[0] = 0.0f;
        var_s0->unk4.f[1] = mathRnd(-0xA, 0xA) * 300.0f;
        var_s0->unk4.f[2] = mathRnd(-0xA, 0xA) * 300.0f;
        var_s0++;

        if (type == 0x10) {
            var_s0->unk16 = 1;
            var_s0->unk14 = 0;
            var_s0->unk10 = NULL;
            var_s0->unk0 = 0x400000;
            var_s0->unk4.f[0] = 0.0f;
            var_s0->unk4.f[1] = 0.0f;
            var_s0->unk4.f[2] = mathRnd(0, 0x12C) + 300.0f;
            sp80.pitch = mathRnd(-0x7FFF, -0xFA0);
            sp80.yaw = mathRnd(0, 0xFFFF);
            mathRotateRPY(&sp80, &var_s0->unk4.f[0]);
            var_s0++;
        } else if (type == 0x11) {
            var_s0->unk16 = 1;
            var_s0->unk14 = 0;
            var_s0->unk10 = NULL;
            var_s0->unk0 = 0x400000;
            var_s0->unk4.f[0] = 0.0f;
            var_s0->unk4.f[1] = 0.0f;
            var_s0->unk4.f[2] = mathRnd(0, 0x12C) + 300.0f;
            sp80.pitch = mathRnd(-0x7FFF, -0xFA0);
            sp80.yaw = mathRnd(0, 0xFFFF);
            mathRotateRPY(&sp80, &var_s0->unk4.f[0]);
            var_s0++;
        } else {
            var_s0->unk16 = 1;
            var_s0->unk14 = 0;
            var_s0->unk10 = NULL;
            var_s0->unk0 = 0x400000;
            var_s0->unk4.f[0] = 0.0f;
            var_s0->unk4.f[1] = 0.0f;
            var_s0->unk4.f[2] = mathRnd(0, 0x64) + 100.0f;
            sp80.pitch = mathRnd(-0x7FFF, -0xFA0);
            sp80.yaw = mathRnd(0, 0xFFFF);
            mathRotateRPY(&sp80, &var_s0->unk4.f[0]);
            var_s0++;
        }

        var_s0->unk16 = 1;
        var_s0->unk14 = 4;
        var_s0->unk10 = data_4C;
        var_s0->unk0 = 4;
        var_s0->unk4.f[0] = 0.0f;
        var_s0->unk4.f[1] = 0.0f;
        var_s0->unk4.f[2] = 0.0f;
        var_s0++;
        sp3B8.unk0 = spB8;
        sp3B8.unk5D = var_s0 - spB8;
        sp3B8.unk54 = 0;
        if (1) {
            sp3B8.unk54 |= 0x04000000;
        }
        sp3B8.unk54 |= flags;
        spAE = dll_modgfx->Func1(&sp3B8, 0, 4, data_0, 4, data_28, 0, sp9C);
        var_s1--;
    }

    var_s1 = mathRnd(2, 6);
    if (type == 7) {
        type = mathRnd(4, 6);
    }

    if (type == 0xB) {
        type = mathRnd(8, 0xA);
    }

    if (type == 0xC) {
        var_s1 = mathRnd(1, 3);
    }

    switch (type) {
    case 0:
    case 20:
        sp80.roll = 0x2A;
        while (var_s1 != 0) {
            gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
            var_s1--;
        }
        break;
    case 1:
        sp80.roll = 0x2B;
        gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
        break;
    case 2:
        sp80.roll = 0x184;
        while (var_s1 != 0) {
            gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
            var_s1--;
        }
        break;
    case 3:
        sp80.roll = 0x1A1;
        while (var_s1 != 0) {
            gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
            var_s1--;
        }
        break;
    case 4:
        sp80.roll = 0x60;
        while (var_s1 != 0) {
            gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
            var_s1--;
        }
        sp80.roll = 0x159;
        gDLL_17_partfx->vtbl->spawn(obj, 3, &sp80, 1U, -1, NULL);
        break;
    case 5:
        sp80.roll = 0x60;
        while (var_s1 != 0) {
            gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
            var_s1--;
        }
        sp80.roll = 0x91;
        gDLL_17_partfx->vtbl->spawn(obj, 3, &sp80, 1U, -1, NULL);
        break;
    case 6:
        sp80.roll = 0x60;
        while (var_s1 != 0) {
            gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
            var_s1--;
    }
        sp80.roll = 0x74;
        gDLL_17_partfx->vtbl->spawn(obj, 3, &sp80, 1U, -1, NULL);
        break;
    case 8:
        sp80.roll = 0x60;
        while (var_s1 != 0) {
                gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
                var_s1--;
        }
        var_s1 = 0x14;
        sp80.roll = 0xDF;
        while (var_s1 != 0) {
            gDLL_17_partfx->vtbl->spawn(obj, 7, &sp80, 1U, -1, NULL);
            var_s1--;
        }
        sp80.roll = 0x159;
        gDLL_17_partfx->vtbl->spawn(obj, 3, &sp80, 1U, -1, NULL);
        break;
    case 9:
        sp80.roll = 0x60;
        while (var_s1 != 0) {
                gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
                var_s1--;
        }
        var_s1 = 0x14;
        sp80.roll = 0xDE;
        while (var_s1 != 0) {
            gDLL_17_partfx->vtbl->spawn(obj, 7, &sp80, 1U, -1, NULL);
            var_s1--;
        }
        sp80.roll = 0x91;
        gDLL_17_partfx->vtbl->spawn(obj, 3, &sp80, 1U, -1, NULL);
        break;
    case 10:
        sp80.roll = 0x60;
        while (var_s1 != 0) {
                gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
                var_s1--;
        }
        var_s1 = 0x14;
        sp80.roll = 0x160;
        while (var_s1 != 0) {
            gDLL_17_partfx->vtbl->spawn(obj, 7, &sp80, 1U, -1, NULL);
            var_s1--;
        }
        sp80.roll = 0x74;
        gDLL_17_partfx->vtbl->spawn(obj, 3, &sp80, 1U, -1, NULL);
        break;
    case 13:
        sp80.roll = 0x4C;
        gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
        break;
    case 14:
        sp80.roll = 0x60;
        while (var_s1 != 0) {
                gDLL_17_partfx->vtbl->spawn(obj, 0x135, &sp80, 1U, -1, NULL);
                var_s1--;
        }
        break;
    case 15:
        gDLL_17_partfx->vtbl->spawn(obj, 0x51B, NULL, 2U, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(obj, 0x51B, NULL, 2U, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(obj, 0x51B, NULL, 2U, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(obj, 0x51B, NULL, 2U, -1, NULL);
        break;
    case 16:
    case 17:
        sp80.roll = 0x4C;
        gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
        break;
    default:
        sp80.roll = 0x2A;
        var_s1 = 5;
        while (var_s1 != 0) {
            gDLL_17_partfx->vtbl->spawn(obj, 5, &sp80, 1U, -1, NULL);
            var_s1--;
        }
        break;
    case 12:
        break;
    }

    return spAE;
}
